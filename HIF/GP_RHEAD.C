/******************************************************************************
*                (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_rhead.c
*
* DESCRIPTION:
*
*   This file contains read header command handler
*
* NOTES:
*
*
* $Revision: 6 $
* $Date: 06/05/25 2:50a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\constant.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_rhead.h"
#include ".\hif\at_knl.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"


/*************************************************************************
* ReadHeaderCommand
*
* Entry point for that the device return the CD devide data block address
* Header of the requested logical block.
*
*   rw_read_mode            data_mode           EDC/ECC
*   --------------------    ---------------     --------
*   1(yellow book mode1)    1                   y
*   2(XA m2 f1)             2                   y
*   3(XA m2 f2)             2                   n
*   4(yellow book mode2)    2                   n
*   0                       0 if not audio      n
*   0                       invalid if audio    n
* CDDA disc handling?
*************************************************************************/
void ReadHeaderCommand()
{
    BYTE    a,tmpmode;
    USHORT  w_alloc;
    long    tmplba;
    TimeS   tmpmsf;
    ULONG   iHeaderAddr;

#if (AUDIO_PLAYBACK == 1)
    /* if nx audio is playing, turn it off and clear buffer dram */
    if (St_nx_audio == TRUE)
    {
        AudioTransferDone();
        ClearDataBuffer();
    }
#endif
    tmplba = (long)PacketCommandToLBALEN4(2);

    if((tmplba < 0) || (tmplba > Maxlba))
    {                                               /* exceed LeadOut */
        BUILD_SENSE(0x05,0x21,0x00,0xaa);                /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return;
    }
                                                    /* check target's writing method */
    HostXfer.StartBlock.lba = tmplba;               /* get starting lba */
    Tg_endlba = tmplba;

    a = CheckTrackWriteMethod(tmplba);

    if( a != TRKNO_NOTVALID)
    {
        if (a == WR_METHOD_FP)
            St_method_2 = TRUE;
        else
            St_method_2 = FALSE;
    }

    ClearDataBuffer();
                                                    /* Check allocation length */
    if (((Iram_Cmd_block.bf[8] & 0xfe) | Iram_Cmd_block.bf[7]) == 0)
    {                                               /* if zero xfr len allocated */
        AutoCommandComplete();                             /* quit */
        return;
    }

#if (AUDIO_PLAYBACK == 1)
    /* turn off audio for all kinds of CD-DISK "mute" the audio */
    SetAudioChannelMode(0);
#endif

    Rw_seek_blk.lba= HostXfer.StartBlock.lba;       /* initial seek block */
    AdjustSeekOffset();                              /* Seek to xx blks before start address */

    while (svoIramVar.knlTimeupCount)
    {
        if (!SeekToTarget())
        {                                           /* if seek fail */
            a = StartBufferingFromTarget();

            if ((a & 0x7F) != 0)                    /* buffer first block */
            {
                /* returned with mode error, maybe audio or data mode 0 */
                /* Entry point for reading cdda disc */
                Svr_pause_reqire = TRUE;            /* mark pause request */
                BUILD_SENSE(0x05,0x64,0x00,0xab);        /* ILLEGAL MODE FOR THIS TRACK */
                AutoCommandComplete();
                return;
            }
            else if ((a & 0x80) == 0)
            {
                /* target block is successfully buffered pause the disc and begin
                   set up data header to xfer */
                Svr_pause_reqire = TRUE;        /* mark pause request */

                iHeaderAddr = AUX_START_ADDR + AUX_HEADER_ADDR1 - 0x800;
                tmpmsf.Minute = SCSDRAM[ iHeaderAddr + 0 ];
                tmpmsf.Second = SCSDRAM[ iHeaderAddr + 1 ];
                tmpmsf.Frame = SCSDRAM[ iHeaderAddr + 2 ];
                tmpmode = SCSDRAM[ iHeaderAddr + 3 ];   /* Read mode BYTE */

                if ( (tmpmode & 0x1F) > 2)          /* invalid mode */
                {
                    BUILD_SENSE(0x05,0x64,0x00,0xac);    /* ILLEGAL MODE FOR THIS TRACK */
                    AutoCommandComplete();
                    return;
                }

                Iram_Buf_dram_addr = BUFFER_SCRATCH;
                WriteSDRAM((tmpmode & 0x1F));          /* write header mode to dram */
                WriteSDRAM(0x0);
                WriteSDRAM(0x0);
                WriteSDRAM(0x0);
                WriteSDRAM(0x0);                       /* fill 4 reserved bytes */

                if (TEST_BIT(Iram_Cmd_block.bf[1],1) )
                {                                   /* MSF mode */
                    tmpmsf=BCD2MSF(&tmpmsf);
                    WriteSDRAM(tmpmsf.Minute);
                    WriteSDRAM(tmpmsf.Second);
                    WriteSDRAM(tmpmsf.Frame);
                }
                else
                {                                       /* LBA mode */
                    WriteSDRAM((BYTE)(HostXfer.StartBlock.lba>>16));/* get & write to dram */
                    WriteSDRAM((BYTE)(HostXfer.StartBlock.lba>>8));
                    WriteSDRAM((BYTE)(HostXfer.StartBlock.lba));
                }

                /* set up xfer parameter and xfer the header data to host */
                BHIGH(w_alloc)= Iram_Cmd_block.bf[7];   /* alloc length high-BYTE */
                BLOW(w_alloc) = Iram_Cmd_block.bf[8];   /* alloc length low-BYTE */
                                                        /* reader header return length = 8 */

#if (LED_NUM == 1)
                ReadyLED(LED_ON);
#endif /* LED_NUM */

#if (LED_NUM == 2)
                ReadyLED(LED_OFF);
#endif /* LED_NUM == 2 */

                SendParameterDataToHost(BUFFER_SCRATCH,MINIMUM( 8, w_alloc) , TRUE);   // Transfer data to host and Command Complete.

#if (LED_NUM == 1)
                ReadyLED(LED_OFF);
#endif /* LED_NUM */

#if (LED_NUM == 2)
                ReadyLED(LED_ON);
#endif /* LED_NUM == 2 */
                                                        /*command complete */
                return;
            }
        }
    }

    /* seek fail, report error and exit */
    Svr_pause_reqire = TRUE;                            /* mark pause request */
    BUILD_SENSE(0x03,0x02,0x00,0x12);                        /* NO SEEK COMPLETE */
    AutoCommandComplete();
}

