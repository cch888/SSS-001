/****************************************************************************
*                (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
*
* FILENAME:   gp_seek.c
*
* DESCRIPTION
*
* This file contains command handler of ATAPI command
*
* NOTES:
*
*
* $Revision: 44 $
* $Date: 11/02/25 11:42a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_seek.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\Diag.h"
#include ".\common\commath.h"
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

extern BOOL    svoVar_abort_pause_if_applyable;

extern BYTE pre_tno; /* Check tno protect */


/*************************************************************************
* Seek10Command
*
* Seek to request MSF location with immediate feature
* By ATAPI Spec. 1.7 Seek always using immediate operation.
* This is also the feature which MicroSoft Wanted (12/06/93)
*
* Returns:  void
* Params:   none
*************************************************************************/

void CDSeek10Command(void)
{
    BYTE rw_seek_retry;

    svoVar_abort_pause_if_applyable = TRUE;
    /* LED Flash in  seek command */
    ReadyLED(LED_FLASH1S);

    if(SpeedMD == Set_sp_f)
    {
        SpeedMD = Special_f;
    }

    // Avoid that buffer is full and then speed down during executing seek10 command.
    SpeedCount = 0;

#if (AUDIO_PLAYBACK == 1)
    if(St_nx_audio == TRUE)
    {
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff; /* Check tno protect */
        AudioTransferDone();
        ClearDataBuffer();
    }
    SetAudioChannelMode(0);
#endif

    HostXfer.StartBlock.lba = PacketCommandToLBALEN3(3);
    Tg_endlba = HostXfer.StartBlock.lba;

#if (SUN_FUNC_TEST == 1)
    READ_C1_C2 = 0;
#endif

    if(pdbServoGetDiscType() == eATIPDisc)
    {
        if( CheckDiscMaxLBA(HostXfer.StartBlock.lba) )
        {
            BUILD_SENSE(0x05,0x21,0x00,0xad);            /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            ATASetDSC();
            AutoCommandComplete();
            return;
        }
    }
    if(Iram_Cmd_block.bf[2] != 0 || (HostXfer.StartBlock.lba > Maxlba))
    {
        BUILD_SENSE(0x05,0x21,0x00,0xae);                /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        ATASetDSC();
        AutoCommandComplete();
        return;
    }

    /* save first request lba */
    /* seek and prefetch operation is selected */
    /* check requested block's track writing method */
    if((CheckTrackWriteMethod(HostXfer.StartBlock.lba) &0xF) == 0x2)
    {
        if(St_method_2 == FALSE)
        {
            St_method_2 = TRUE;
            StopDecoder();
            ClearDataBuffer();
        }
    } else {
        if(St_method_2 == TRUE)
        {
            St_method_2 = FALSE;
            StopDecoder();
            ClearDataBuffer();
        }
    }

    //For Data and Audio both
    if( (READ_FIELD(HAL_CDD_SDBUF))&&(((HostXfer.StartBlock.lba > DataBuffer.CacheStartLBA)&&(HostXfer.StartBlock.lba < (DataBuffer.CacheEndLBA+4)))||( (HostXfer.StartBlock.lba <= DataBuffer.CacheEndLBA)&&(HostXfer.StartBlock.lba >= DataBuffer.CacheStartLBA) )) )
    {
        ATASetDSC();                              		/* set dsc bit */
        St_cmd_inproc = FALSE;                          /* mark no cmd in proc */
    	Ata_sub_burst = FALSE;

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
        if (s2b_atapi_cmd_received == TRUE)
        {
            AutoCommandComplete();
        }
        else
#endif
        {
		    AutoCommandComplete();
        }
        return;                                     	/* jmp  main_loop */
    }

    StopDecoder();
    Rw_seek_blk.lba = HostXfer.StartBlock.lba;          /* initial seek block */
    AdjustSeekOffset();                                 /* execute adjust rw_seek_blk operation */

    /* call player module for seek. Make seek 1 block ahead of target block */

    DecTimeCnt-=25L;

    rw_seek_retry = 0;

    while(SeekToTarget() == FAIL){               		/* if seek fail */
        rw_seek_retry++;
        if ( (svoIramVar.knlTimeupCount == 0) || (rw_seek_retry > 10))
        {
            /* Set dsc bit to indicate actual completion.
            No interrupt will be generated to host */
            St_cmd_inproc = FALSE;                      /* mark no cmd in proc */
            Svr_pause_reqire = TRUE;                    /* mark pause request */
            Ata_sub_burst = FALSE;

            BUILD_SENSE(0x03,0x02,0x00,0x13);           /* NO SEEK COMPLETE */
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
            if (s2b_atapi_cmd_received == TRUE)
            {
                AutoCommandComplete();
            }
            else
#endif
            {
                ATASetDSC();                              	/* set dsc bit */
			    AutoCommandComplete();
            }
            return;                                     /* jmp  main_loop */
        }

       SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down
    }


    /* clear all data blocks in the buffers */
    ClearDataBuffer ();

    /* Perform pre-fetch for CD-ROM data disc after seek operation
       dsc (disk seek complete) should have been reported

       read current Q subcode to identify yellow book disc
       if it is not, then do not do prefetch
       if it is data disc, do seek pre-fetch */

    if((ServoReadSubcode(1,0xFF)==PASS)&&(St_cdrom_data == TRUE))                          /* call player */
    {
        /* if read subcode no error, check if digital or audio track */
    
        if ( !TEST_BIT(((BYTE *)PlayerAddrsPtr)[0],6) )
        {
            /* if this is audio disc, do not perform seek prefetch */
            Svr_pause_reqire = TRUE;                        /* mark pause request */
        }
        else
        {
            Rw_cdda_disc = FALSE;
            /* this is a data disc, do perform seek prefetch */
            DataBuffer.CacheEndLBA = HostXfer.StartBlock.lba;       /* initial seek block */
            MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheEndLBA,1); /* seek to one block ahead */
            DataBuffer.CacheStartLBA = DataBuffer.CacheEndLBA;      /* initial seek block */
            InitBuffering();                                   /* initial to parallel buffering block */
        }
    }

    St_cmd_inproc = FALSE;                          /* mark no cmd in proc */
    Ata_sub_burst = FALSE;
    RandomRead = TRUE;
    ATASetDSC();
    AutoCommandComplete();
}
