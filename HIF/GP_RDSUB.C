/*******************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_rdsub.c
*
* DESCRIPTION:
*   This file contains command handler of read sub-channel command
*
* NOTES:
*
*
* $Revision: 23 $
* $Date: 09/07/08 3:40p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\constant.h"
#include ".\common\error.h"
#include ".\common\comMath.h"
#include ".\hif\gp_equ.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_rdsub.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\al\reg_intctrl.h"

static vBOOL ReadSubQSubcode2(void);
static vBOOL ReadSubQSubcode3(void);
static void TransferSubQDataToHost(void);
static void ChangeToASCII(BYTE d);
static void SaveISRCInASCII(BYTE d);
static void GetI1ThruI5(BYTE *ptr);
static BYTE GetAudioStatus(void);

static void WriteSubQData(BYTE flag);
static void TransferSubchannelDataToHost(void);

/*************************************************************************
* ReadSubChannelCommand
*
* Read sub channel command handler
*
* Returns:  void
* Params:   none
*************************************************************************/
void ReadSubChannelCommand(void)
{
	BYTE temp;

	if (pdbDiscLoaded() != CD_DISC)
    {
		BUILD_SENSE(0x05,0x30,0x02,0x03);                   // CANNOT READ MEDIUM - INCOMPATIBLE FORMAT
		AutoCommandComplete();
		return;
	}

    /* if current command is read sub-channel command, if we are playing
        audio, do not pause audio command. if servo status is busy,
        just abort current servo command and a pause will be issued to servo.*/
    /* Backgroud may be still doing seek operation.
        Check if servo command is in process (servo is BUSY)
        if it is, issue pause command to abort current servo command */
    if ((Iram_Svr_player_mode != PLAY_TRACK_MODE) && (GetHIFToPlayerStatus() == BUSY))
		ServoPause();

    if(Iram_Cmd_block.bf[3] != 0xF0){
        if((Iram_Cmd_block.bf[3] == 0)||(Iram_Cmd_block.bf[3] > 3)){    /* sub_ch data format */
            BUILD_SENSE(0x05,0x24,0x00,0x4b);            /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
        /* Track Number only valid in format 1 or 2, if value <= max track.*/
        if((Iram_Cmd_block.bf[6] != 0) && (Iram_Cmd_block.bf[3] != 3)){
            BUILD_SENSE(0x05,0x24,0x00,0x4c);
            AutoCommandComplete();
            return;
        }
    }

    if((Iram_Cmd_block.bf[3] == 3) || (Iram_Cmd_block.bf[3] == 0xF0))
    {                           /* subchannel format 3 */   /* Vendor specific 0xF0 (RID) */
        Iram_Buf_dram_addr = TOC_DRAM_HI + MAX_TRACK;
        temp  = ReadSDRAM();                              /* get max track */
        if((Iram_Cmd_block.bf[6] > temp)||(Iram_Cmd_block.bf[6] == 0)){                /* if request track greater than max track, error*/
            BUILD_SENSE(0x05,0x24,0x00,0x4d);                /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
    }

    /* check allocation length, return command completed if equal zero */
    if(((Iram_Cmd_block.bf[8] & 0xfe) | Iram_Cmd_block.bf[7]) == 0)
    {                                                   /* check for zero length */
        AutoCommandComplete();
        return;
    }

    if((Iram_Rw_read_mode == eCDDAMode)||(St_cdrom_audio == TRUE))
    {
        if(GetSpeedValue(Current_read_sp) > GetSpeedValue(PLAY_AUDIO_MSF_SPD))  //Only allow using lower speed
        {
            SetReadSpeed(PLAY_AUDIO_MSF_SPD);
        }
        SpeedCount = (CD_SPEED_UP_READY + CD_SPEED_DOWN_READY)/2;               //Prevent changing speed
    }

#if (USE_TWO_KEY_FUNCTION==1)
    gHw_play_no = HW_PLAY_STOP_OFF;
#endif

    /* process the read sub-channel command, data header first */
    Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR;               /* get sub-channel SDRAM location */

    WriteSDRAM( 0 );                                       /* reserved BYTE */
    WriteSDRAM( GetAudioStatus() );                         /* write audio status */
    WriteSDRAM( 0 );                                       /* write hi-BYTE data length to SDRAM */

    if( !TEST_BIT(Iram_Cmd_block.bf[2], 6) )
    {                                                   /* if subQ bit */
        /* subQ bit not set, no sub-channel data be returned */
        WriteSDRAM( 0 );                                   /* write lo-BYTE of data length */
        TransferSubQDataToHost();
    }else
    {
        /* subQ bit set, program lo-BYTE data length base on data format */
        /* Added cases of format 1, and subq read error */
        switch( Iram_Cmd_block.bf[3] ){
            case 1:                                     /* Format 1, Current position data */
                if(ReadSubChannelFormat1(Iram_Cmd_block.bf[1] & 0x02) == TRUE){
                    if ( St_nx_audio )
                        TransferSubchannelDataToHost();
                    else                                /* in LBA format */
                        TransferSubQDataToHost();
                }
                break;

            case 2:                                     /* Format 2, UPC */
                if((St_nx_audio ==TRUE)||(Iram_Svr_player_mode == PLAY_TRACK_MODE)){
                /* if in play operation, can not access sub-ch data */
                    BUILD_SENSE(0x05,0x20,0x00,0x2f);        /* INVALID COMMAND OPERATION CODE */
                    AutoCommandComplete();
                    return;
                }

                if(ReadSubQSubcode2() == TRUE){
                    TransferSubQDataToHost();
                }
                break;

            case 3:                                     /* Format 3, ISRC */
            case 0xF0:                                  /* Format F0, RID   */
                if((St_nx_audio == TRUE)||(Iram_Svr_player_mode == PLAY_TRACK_MODE)){
                    BUILD_SENSE(0x05,0x20,0x00,0x30);        /* INVALID COMMAND OPERATION CODE */
                    AutoCommandComplete();
                    return;
                }

                if(ReadSubQSubcode3() == TRUE){
                    TransferSubQDataToHost();
                }
                break;

            default:                    /* not valid data format */
                BUILD_SENSE(0x05,0x24,0x00,0x4e);            /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                break;
        }
    }
}

/*********************************************************************
* TransferSubchannelDataToHost
*
* Setup registers to transfer Sub-Channel data to host
*********************************************************************/
static void TransferSubchannelDataToHost(void)
{
    USHORT act_len, alloc_len;

    BLOW (alloc_len) = Iram_Cmd_block.bf[8];
    BHIGH(alloc_len) = Iram_Cmd_block.bf[7];

    act_len=0x10;   /* for correct display related MSF under DOS's AP */
    SendParameterDataToHost(RD_SUBQ_LBA_ADR,MINIMUM( act_len, alloc_len) , TRUE);   // Send data and Command Complete.

}
/*********************************************************************
* ReadSubChannelFormat1()
*
* Read Sub-Channel Format 1 returns CD-ROM current position
*
* Returns:  TRUE - if need followed with TransferSubQDataToHost()
*           FALSE
*********************************************************************/
BYTE     SOTCstopSubQ[10];

vBOOL ReadSubChannelFormat1(BYTE flag)
{
    ULONG temp_lba;
    BYTE tno,a;
    TimeS temp_time;
    ADDRESS_T   addr;

    Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 3;           /* get sub-channel SDRAM location */

    WriteSDRAM(0x0C);                                      /* save data length lo-BYTE to SDRAM */
    if ( St_nx_audio )
    {
        WriteSubQData(flag);                             /* Check MSF Flag */
        return TRUE;
    }

    if ( Iram_Rw_SOTC_st == TRUE ) {
        WriteSDRAM( 1 );                                   /* write format 1 to SDRAM */
        WriteSDRAM( SWAP( SOTCstopSubQ[0] ));              /* assume CTL & ADD = 11h */
        WriteSDRAM( SOTCstopSubQ[1] );                     /* write track num */
        WriteSDRAM( SOTCstopSubQ[2] );                     /* index */
        WriteSDRAM( 0x00 );
        if (flag) {                                     /* Check MSF Flag */
            WriteSDRAM( SOTCstopSubQ[7] );                 /* write abs_min to SDRAM*/
            WriteSDRAM( SOTCstopSubQ[8] );                 /* write abs_sec to SDRAM*/
            WriteSDRAM( SOTCstopSubQ[9] );                 /* write_abs_frame to SDRAM*/
            WriteSDRAM( 0x00 );
            WriteSDRAM( SOTCstopSubQ[3] );                 /* write rel_min to SDRAM */
            WriteSDRAM( SOTCstopSubQ[4] );                 /* write rel_sec to SDRAM */
            WriteSDRAM( SOTCstopSubQ[5] );                 /* write rel_frm to SDRAM */
            return( TRUE );
        } else {
            addr.lba = MSF2LBA(&SOTCstopSubQ[7]);
            WriteSDRAM( (BYTE)(((ULONG)addr.lba >> 16) & 0xff) );  /* Write abs LBA */
            WriteSDRAM( (BYTE)(((ULONG)addr.lba >>  8) & 0xff) );
            WriteSDRAM( (BYTE)(addr.lba & 0xff) );
            WriteSDRAM( 0x00 );
            addr.lba = MSF2LBA(&SOTCstopSubQ[3]);
            WriteSDRAM( (BYTE)(((ULONG)addr.lba >> 16) & 0xff) );  /* Write Rel LBA */
            WriteSDRAM( (BYTE)(((ULONG)addr.lba >> 8) & 0xff) );
            WriteSDRAM( (BYTE)(addr.lba & 0xff) );
            return( TRUE );
        }
    }

    WriteSDRAM(1);                                         /* write format 1 to SDRAM */

    tno = pdbGetHighestRecordedTrkNumber();
    while(tno){
        temp_time = pdbGetStartTime(tno);
        temp_lba  = MSF2LBA_Plus(&temp_time);
        if (temp_lba <= Tg_endlba){
            temp_lba = Tg_endlba - temp_lba;
            break;
        }
        --tno;
    }

    /* ContAddr */
    a = pdbGetTrackContAddr(tno);
    WriteSDRAM( SWAP (a));                                 /* Ctrl/Addr */
    /* Tno */
    WriteSDRAM(tno);
    /* Index */
    WriteSDRAM(0);
    /* abs */
    WriteSDRAM(0);                                         /* MSB */
    if (flag)
    {                                                   /* MSF mode */
        temp_time = LBA2MSF(Tg_endlba);
        WriteSDRAM(temp_time.Minute);
        WriteSDRAM(temp_time.Second);
        WriteSDRAM(temp_time.Frame);
    }
    else
    {                                                   /* LBA mode */
        WriteSDRAM((BYTE)(((ULONG)Tg_endlba >> 16)&0xff));
        WriteSDRAM((BYTE)(((ULONG)Tg_endlba >> 8)&0xff));
        WriteSDRAM((BYTE)(Tg_endlba)&0xff);
    }
    /* Track Rel. */
    WriteSDRAM(0);                                         /* MSB */
    if (flag)
    {                                                   /* MSF mode */
        temp_time = ADR2MSF(temp_lba);
        WriteSDRAM(temp_time.Minute);
        WriteSDRAM(temp_time.Second);
        WriteSDRAM(temp_time.Frame);
    }
    else
    {                                                   /* LBA mode */
        WriteSDRAM((BYTE)(((ULONG)temp_lba >> 16)&0xff));
        WriteSDRAM((BYTE)(((ULONG)temp_lba >> 8)&0xff));
        WriteSDRAM((BYTE)(temp_lba)&0xff);
    }
    return(TRUE);
}
/**********************************************************************
* WriteSubQData()
*
* This routine gets the ABS & REL MSF of the current block being
* played to the SDRAM
* This is in BCD so convert to Hex before writing to SDRAM
*
* Returns:     void
* Params: The address of current aux block we want to write.
**********************************************************************/
extern USHORT   Iram_Aud_xfer_subq_ptr;
BYTE pre_tno;
ULONG Pre_RelativeTime_lba,Pre_AbsoluteTime_lba;
extern  CMD_BLOCK_T Play_Cmd_block; /* for first page protection  */

void WriteSubQData(BYTE flag)
{
    USHORT subq_ptr;
    ADDRESS_T addr, RelativeTime, AbsoluteTime;
    BYTE  i,subq[10];
    ULONG work_addr;
    BYTE tno;
    TimeS Start_Time;
    vBOOL temp_IE; /* write register protect */


    if ( ! St_audten_int )
    {
        subq_ptr = READ_REG(HAL_AUDREG_MAP->ACURPGL);
    }
    else
    {
        subq_ptr = Iram_Aud_xfer_subq_ptr;
    }

    BHIGH(subq_ptr) = (BHIGH(subq_ptr) & 0x1F);

    work_addr = PAGE_AUX_START_ADDR(subq_ptr);

    for (i = 0; i < 10; i++)
    {
        subq[i] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + i];  /* read Ctrl Addr */
    }    
    i = 0;
    while(i < 10)
    {
        i++;

        if( ((pre_tno != 0xff) && ( (pre_tno > BCD2HEX(subq[1])) || ((BCD2HEX(subq[1]) - pre_tno) > 1) ))
             || ((subq[0] & 0x0F) != 0x01) 
            )
        {
            /* for first page protection */
            if (pre_tno == 0xff && St_nx_audio)
            {
              Start_Time.Minute = Play_Cmd_block.bf[3];
              Start_Time.Second = Play_Cmd_block.bf[4];
              Start_Time.Frame  = Play_Cmd_block.bf[5];
              tno = FindTrackNumber(MSF2LBA_Plus(&Start_Time));

              // return play command start time
              subq[0] = 0x21;
              subq[1] = tno;
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 0] = 0x21;              /* Ctrl + Addr */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 1] = tno;               /* Track */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 2] = 0x01;              /* Index */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 3] = 0;                 /* Relative Minute */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 4] = 0;                 /* Relative Second */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 5] = 0;                 /* Relative Frame  */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 7] = HEX2BCD(Start_Time.Minute); /* Absolute Minute */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 8] = HEX2BCD(Start_Time.Second); /* Absolute Second */
              SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 9] = HEX2BCD(Start_Time.Frame);  /* Absolute Frame  */
              for (i = 2; i < 10; i++)
              {
                  subq[i] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + i];  /* read Ctrl Addr */
              }
              break;
            }
            
            if (St_nx_audio) //Tno is error,But MSF is continuity.
            {  
	         subq[3] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 3];                 /* Relative Minute */
                subq[4] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 4];                 /* Relative Second */
                subq[5] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 5];                 /* Relative Frame  */
                RelativeTime.lba = MSF2LBA(&subq[3]);
                subq[7] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 7];                 /* Absolute Minute */
                subq[8] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 8];                 /* Absolute Second */
                subq[9] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 9];                 /* Absolute Frame  */
                AbsoluteTime.lba = MSF2LBA(&subq[7]);
		        if((RelativeTime.lba == (Pre_RelativeTime_lba+1))||(AbsoluteTime.lba == (Pre_AbsoluteTime_lba +1)))
		        {
		            break;
		        }
		    }
                
            if (subq_ptr == 0)
                subq_ptr = RING_END_PAGE;
            else
                subq_ptr--;
            work_addr = PAGE_AUX_START_ADDR(subq_ptr);
            subq[0] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR];
            subq[1] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 1];
            continue;
        }
        break;
    }
    pre_tno = BCD2HEX(subq[1]);
    RelativeTime.lba = MSF2LBA(&subq[3]);
    AbsoluteTime.lba = MSF2LBA(&subq[7]);
    
    Pre_RelativeTime_lba = RelativeTime.lba;
    Pre_AbsoluteTime_lba = AbsoluteTime.lba;
/* Check tno and mode_1 */

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 4;           /* get sub-channel SDRAM location */

    WriteSDRAM(0x01);                                       /* sub channel data format */
    WriteSDRAM(SWAP(subq[0]));                              /* assume CTL & ADD = 11h */
    WriteSDRAM(BCD2HEX(subq[1]));                           /* write track num */
    WriteSDRAM(subq[2]);                                    /* index */
    WriteSDRAM(0x00);
    if (flag)                                               /* Check MSF Flag */
    {
        WriteSDRAM(BCD2HEX(subq[7]));                               /* write abs_min to SDRAM*/
        WriteSDRAM(BCD2HEX(subq[8]));                               /* write abs_sec to SDRAM*/
        WriteSDRAM(BCD2HEX(subq[9]));                               /* write_abs_frame to SDRAM*/
        WriteSDRAM(0x00);
        WriteSDRAM(BCD2HEX(subq[3]));                               /* write rel_min to SDRAM */
        WriteSDRAM(BCD2HEX(subq[4]));                               /* write rel_sec to SDRAM */
        WriteSDRAM(BCD2HEX(subq[5]));                               /* write rel_frm to SDRAM */
    }
    else
    {
        subq[7] = BCD2HEX(subq[7]);
        subq[8] = BCD2HEX(subq[8]);
        subq[9] = BCD2HEX(subq[9]);

        addr.lba = MSF2LBA(&subq[7]);
        WriteSDRAM((BYTE)(((ULONG)addr.lba >> 16) & 0xff));/* Write abs LBA */
        WriteSDRAM((BYTE)(((ULONG)addr.lba >> 8) & 0xff));
        WriteSDRAM((BYTE)(addr.lba & 0xff));
        WriteSDRAM(0x00);
        addr.lba = MSF2LBA(&subq[3]);
        WriteSDRAM((BYTE)(((ULONG)addr.lba >> 16) & 0xff));/* Write Rel LBA */
        WriteSDRAM((BYTE)(((ULONG)addr.lba >> 8) & 0xff));
        WriteSDRAM((BYTE)(addr.lba & 0xff));
    }
    SET_INT_ENABLE_MASK(temp_IE);

}

/*********************************************************************
* ReadSubQSubcode2()
*
* Read Sub-Channel Format 2 returns Media catalogue number
*
* Returns:  TRUE - if need followed with TransferSubQDataToHost()
*           FALSE
*********************************************************************/
static vBOOL ReadSubQSubcode2(void)
{
BYTE i;
BYTE *subc_ptr;

    WriteSDRAM(20);                                        /* data length lo-BYTE */
    WriteSDRAM(2);                                         /* format 2 */

    /* initialize media catalogue number data block  */

    for( i = 0 ; i < 19 ; i++ ){
        WriteSDRAM(0);
    }
    /* start searching UPC code from the beginning of cd (0:2:0 location) */

    /* call servo to get sub-channel data format 2 */

    if(ServoReadSubcode(0x02,0xFF) == FAIL){            /* UPC code not found */
                                                        /* write invalid data to host */
        Svr_pause_reqire = TRUE;
    }else{
        /* UPC code found */
        Svr_pause_reqire = TRUE;
        Iram_Buf_dram_addr    = RD_SUBQ_LBA_ADR + 8;            /* point to MCVal BYTE */
        WriteSDRAM(0x80);                                  /* write valid MCVal into SDRAM */
        subc_ptr         = (BYTE *)PlayerAddrsPtr;

        /* looping 6 times to return 12 bytes of N1(msb)..N12, and write data to SDRAM */

        subc_ptr++;
        for( i = 0 ; i < 6 ; i++,subc_ptr++){
            ChangeToASCII(*subc_ptr);                 /* encoded as ASCII charactes */
        }

        WriteSDRAM(((*subc_ptr) >> 4) + 0x30);             /* N13 */
        subc_ptr++;
        WriteSDRAM(*subc_ptr);                             /* "Zero" */
        subc_ptr++;
        WriteSDRAM(*subc_ptr);                             /* AFrame */
    }
    return(TRUE);
}


/*********************************************************************
* ReadSubQSubcode3()
*
* Read Sub-Channel Format 3 returns raw ISRC format on the CD-ROM disc
*
* Returns:  TRUE - if need followed with TransferSubQDataToHost()
*           FALSE
*********************************************************************/
static vBOOL ReadSubQSubcode3(void)
{
    BYTE i;
    BYTE *subc_ptr;
    BYTE I1toI5[4];

    WriteSDRAM(20);                                    /* data lenghth */
    WriteSDRAM(3);                                     /* write format 3 into SDRAM */

    Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 5;
                                                    /* initialize 20 bytes of ISRC data */
    for( i = 0 ; i < 19 ; i++ ){
        WriteSDRAM(0);
    }

    /* start searching ISRC code for the requested track */
    /* call servo to get sub-channel data format 3 */
    if(ServoReadSubcode(Iram_Cmd_block.bf[3],Iram_Cmd_block.bf[6]) == FAIL)
    {   /* ISRC code not found, write invalid data to host */
        /* ERROR CODE id Player_interface.param1 */
        Svr_pause_reqire = TRUE;

        Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 6;   /* Track number */
        WriteSDRAM(Iram_Cmd_block.bf[6]);
    }else{
                                                    /* ISRC code found */
        Svr_pause_reqire = TRUE;

        Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 5;   /* ADR/control */

        subc_ptr = (BYTE *)PlayerAddrsPtr;
        WriteSDRAM( (*subc_ptr));
        WriteSDRAM(Iram_Cmd_block.bf[6]);

        Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR + 8;   /* set TCVal */

        WriteSDRAM(0x80);

        /* prepare and save the ISRC data into SDRAM */
        subc_ptr++;
        for(i = 0;i< 4;i++,subc_ptr++){
            I1toI5[i] = *subc_ptr;                  /* I1~I5 */
        }
        GetI1ThruI5(I1toI5);                          /* Changed I1~I5 to ASCII  & save to SDRAM */

        for(i = 0;i < 3;i++,subc_ptr++){
            SaveISRCInASCII(*subc_ptr);           /* Changed I6~I11 to ASCII & save to SDRAM */
        }
        WriteSDRAM( ((*subc_ptr) >> 4) | 0x30);        /* Changed I12 to ASCII & save to SDRAM */
        WriteSDRAM(0);
                                                    /* Zero */
        subc_ptr++;
        WriteSDRAM( BCD2HEX(*subc_ptr) );                       /* Aframe */
    }
    return(TRUE);
}


/*********************************************************************
* TransferSubQDataToHost
*
* Setup registers to transfer Sub-Channel data to host
*********************************************************************/
static void TransferSubQDataToHost(void)
{
    USHORT act_len, alloc_len;
    vUSHORT Temp;

    if( !TEST_BIT(Iram_Cmd_block.bf[2], 6) ){
                                                    /* SubQ bit not set */
        Temp = 0x04;
    }else{
        /* SubQ bit set, send complete sub-ch data info */
        /* compare actual data length with allocation data length,return
           whichever smaller length */

        if(Iram_Cmd_block.bf[3] == 1){
            BLOW (act_len) = 0x10;                  /* data length for format 1 */
        }else{
            BLOW (act_len) = 0x18;                  /* data length for format 2&3 */
        }
        BHIGH(act_len) = 0;
        BHIGH(alloc_len) = Iram_Cmd_block.bf[7];
        BLOW (alloc_len) = Iram_Cmd_block.bf[8];
        Temp = MINIMUM( act_len, alloc_len);  // Number of bytes to xfer. Compare, return minimum.
    }

    SendParameterDataToHost(RD_SUBQ_LBA_ADR,Temp , TRUE); // Send data and Command Complete.
}


/**********************************************************************
* ChangeToASCII
*
* This routine is used to change one BYTE of subcode data to two
* bytes that encoded as ASCII characters.
*
* Returns:  void
* Params:   d - data from subcode
**********************************************************************/
static void ChangeToASCII(BYTE d)
{
    WriteSDRAM( (d >> 4 ) + 0x30);
    WriteSDRAM( (d & 0x0F) + 0x30);
}


/*********************************************************************
* SaveISRCInASCII
*
* This routine is used to prepare BYTE 14..20 for format 3 (ISRC)
*
* Returns:  vois
* Params:   d - data from subcode
*********************************************************************/
static void SaveISRCInASCII(BYTE d)
{
    WriteSDRAM( (d >> 4) | 0x30);
    WriteSDRAM( (d & 0x0F) | 0x30);
}


/*********************************************************************
* GetI1ThruI5
*
* Get I1-I5 data code for format 3 (ISRC)
*
* Returns:  void
* Params:   data from subcode
*********************************************************************/
static void GetI1ThruI5(BYTE *ptr)
{
    /* send 6 bits at a time and return the ISRC bytes 1 - 5 in ASCII */

    WriteSDRAM((  ptr[0] >> 2)+'0' );
    WriteSDRAM((((ptr[0] & 0x03) << 4) | (ptr[1] >> 4))+'0' );
    WriteSDRAM((((ptr[1] & 0x0F) << 2) | (ptr[2] >> 6))+'0' );
    WriteSDRAM((  ptr[2] & 0x3F)+'0' );
    WriteSDRAM((  ptr[3] >> 2)+'0' );
}


/*********************************************************************
* GetAudioStatus
*
* This function check and return the play audio status.
*
* Returns:  audio status
* Params:   none
*********************************************************************/
BYTE GetAudioStatus(void)
{
    BYTE status;

    if ( St_nx_audio )
    {
        if ( St_audio_pause )
        {
            return (AUD_PAUSED);                        /* mark audio pause mode */
        }
        else
        {
            return (AUD_IN_PROGRESS);
        }
    }

    if (St_cdrom_audio)
       status = 0x15;       /* No current audio status to return */
    else
    status = 0x00;                                      /* Audio status BYTE not supported or not valid */

    if(Iram_Svr_player_mode == AUDIO_ERROR_MODE){
        status = AUD_ERR_STOP;                          /* mark audio error */
        Iram_Svr_player_mode = PAUSING_MODE;            /* change as normal mode */
    }
    else if(Iram_Svr_player_mode == PLAY_TRACK_MODE){
        status = AUD_IN_PROGRESS;                       /* mark pause in process */
    }
    else if(St_audio_complete == TRUE){
        /* audio_complete bit set means audio has been successfully played
           and stopped */

        status = AUD_COMPLETED;                         /* mark suceesfully */
        St_audio_complete = FALSE;                      /* reset audio complete */
    }
    else if(Iram_Svr_player_mode == AUDIO_PAUSE_MODE){
        status = AUD_PAUSED;                            /* mark audio pause mode */
    }

    return (status);
}
