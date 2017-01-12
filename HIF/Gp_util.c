/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   gp_util.c
*
* DESCRIPTION
*   This file contains general utility routines
*
* $Revision: 199 $
* $Date: 11/03/28 2:37p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\servo\svo.h"
#include ".\player\plrDb.h"
#include ".\player\plrServo.h"
#include ".\common\commath.h"
#include ".\common\constant.h"
#include ".\hif\mmc.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\bf_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_audio.h"
#include ".\oem\oem_tbl.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\hif\flash.h"
#include ".\DVD\dvMacros.h"
#include ".\DVD\dvUtil.h"
#include ".\DVD\dvGvars.h"

#include ".\al\REG_global.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\common\intctl.h"
#if(BD_ENABLE == 1)
#include ".\BD\bdMacro.h"
#include ".\BD\bdVars.h"
#include ".\BD\bdUtil.h"
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_oem_tbl.h"
#include ".\oem\sun710_function_test.h"
#endif

#include ".\servo\svoVar.h"

extern BYTE     RD_speed_setting;
extern ULONG    Iram_startlba_ck;
extern ULONG    Iram_packetsize_ck;
extern vSHORT   Iram_not_change_speed_timer;
BYTE   Packet_Track = TRKNO_NOTVALID;
extern DiscTransferS volatile dvdDiscRequest;
extern BYTE    dvdVideoFlag;
#if(BD_ENABLE == 1)
extern BDDiscTransferS  volatile bdDiscRequest;
extern BYTE    bdVideoFlag;
#endif
#if(Detect_AVCHD_DISC == 1)
extern BYTE AVCHDFlag;
#endif
BYTE  current_tno;
TDBInfo TDB;
BYTE CurTblOnDram;

#if (EN_DVD_HIGH_JITTER_DETECT  == 1)
extern BYTE	HIGH_JITTER_DISC;
#endif
#if (Customer_Dune_HD== 1)//[V08] Bios DVD 8x for high bit rate HDI
BYTE  SpeedState;
#endif
#if (SUN_FUNC_TEST == 1)
extern BYTE const Init_EEPROM_Table[];
void FillEEPROMWithInitialValue(void);
#endif
/*********************************************************************
* GetMsfFromServo
*
* Get MSF from servo.
*
* Returns:  void
* Params:   msfp - (out) pointer to an MSF structure to hold result in
*                   hex format
*           inbuf - (in) pointer for input.
*                   It could be pointed to abs. MSF or rel. MSF
*********************************************************************/
void GetMsfFromServo(TimeS *msfp, BYTE *inbuf)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    msfp->Minute = BCD2HEX(inbuf[0]);
    msfp->Second = BCD2HEX(inbuf[1]);
    msfp->Frame  = BCD2HEX(inbuf[2]);

    SET_INT_ENABLE_MASK(temp_IE);
}

/*********************************************************************
* CheckAudioOperation()
*
* DESCRIPTION: check opcode to see if it is a command that must
* terminate audio operation.
*
*
*********************************************************************/
extern BYTE pre_tno; /* Check tno protect */

void CheckAudioOperation(void)
{
    if( Knl_atcmd_avail && ( Iram_Cmd_block.bf[0] != 0xA2))
    {
        ServoPause();                    /* servo pause */
        St_audio_complete = TRUE;       /* mark audio ok */
        return;
    }

    switch(Iram_Cmd_block.bf[0])
    {
#if ENABLE_CDG == 1
        case MMC_READ_CD:
            /* check if CD-G application during audio playing */
            if ((Iram_Cmd_block.bf[10] != 0) && (Iram_Cmd_block.bf[9]==0))
            break;
#endif /* ENABLE_CDG */
        case MMC_PLAY_AUDIO_10:
        case MMC_PLAY_AUDIO_12:
            if ( CheckForFF((BYTE *)&Iram_Cmd_block.bf[2], 4) )
            {
                return;
            }

        case MMC_PLAY_AUDIO_MSF:
            /*before we cancel any operation, we need to verify if we're not getting a 4 times FF as start lba*/
            if ( CheckForFF((BYTE *)&Iram_Cmd_block.bf[3], 3) )
            {
                return;
            }
#if ENABLE_CDG != 1
        case MMC_READ_CD:
#endif /* ENABLE_CDG */
        case SBC_READ_10:
        case SBC_READ_12:
        case SBC_VERIFY_10:
        case SBC_VERIFY_12:
        case READ_HEADER:
        case MMC_READ_CD_MSF:
        case SBC_SEEK:
        case MMC_SET_CD_SPEED:
 //    case SBC_START_STOP_UNIT:    /*for continue play*/
        case MMC_SCAN:
        case MMC_WRITE_10:
        case MMC_CLOSE_SESSION:
        case MMC_READ_DISK_INFO:
        case MMC_RESERVE_TRACK:
        case MMC_READ_TRACK_INFO:
        case MMC_SYNC_CACHE:
        case MMC_BLANK:
        case MMC_FORMAT:
        case MMC_READ_BUF_CAP:
        case MMC_SEND_OPC_INFO:
            Svr_pause_reqire = TRUE;    /* mark pause request */
            St_audio_complete = TRUE;   /* mark audio ok */
            /* if nx audio, turn off the audio and clear buffer dram */
#if (AUDIO_PLAYBACK == 1)
            if (St_nx_audio == TRUE)
            {
                St_nx_audio = FALSE;
                pre_tno = 0xff; /* Check tno protect */
                AudioTransferDone();
                ClearDataBuffer();
    #if (USE_TWO_KEY_FUNCTION==1)
        if (Iram_Cmd_block.bf[0] == SBC_SEEK)
                gPause_By_SeekCmd_flag = TRUE;
    #endif /* USE_TWO_KEY_FUNCTION */
            }
#endif // AUDIO_PLAYBACK
            return;
    default:
        return;
    }
}

/*********************************************************************
* CheckForFF()
*
* Returns:  1 - current position req detected
*           0 - not current position req detected
* Params:   cmdp - start testing address of cmd_block
*           num - block number to test
*********************************************************************/
vBOOL CheckForFF(BYTE *cmdp, BYTE num)
{
    for ( ; num ; num--, cmdp++ )
        if (*cmdp != 0xff)
            return FALSE;

    return TRUE;
}

/*********************************************************************
* CheckValidMSF()
*
* Description: check the address fields: M = 0-99, S = 0-59, F = 0-74
* Returns:  1 - invlaid msf
*       0 - valid msf
* Params:   cmdp - start testing address of cmd_block
*********************************************************************/
vBOOL CheckValidMSF(BYTE *cmdp)
{
    if ( (*cmdp > 99) || (*(cmdp+1) > 59) || ( *(cmdp+2) > 74) )
        return FAIL;
    else
        return PASS;
}

/*********************************************************************
* FindTrackNumber
*
* Find track number from lba if it is within the range.
*
* Para:     target lba
* Return:   track number  if 0 then not found
*********************************************************************/
BYTE FindTrackNumber(long lba)
{
    BYTE  tno,last_tno;
    long  start_lba,stop_lba;
    TimeS start_time,stop_time;

    /* reduce process time for transfer rate test */
    /* original: search from 1st to last track, new: search from last to 1st track */
    last_tno = pdbGetFinalSessLastTrackNumber();

    for(tno = last_tno;tno >=1 ;tno--)
    {
        start_time = pdbGetStartTime(tno);
        stop_time  = pdbGetStopTime(tno);
        stop_lba   = MSF2LBA_Plus(&stop_time);

        //check from pre-gap/pause
        if(tno > 1)
            start_lba = MSF2LBA_Plus(&start_time)-150L;
        else
            start_lba = 0L;

        if((lba >= start_lba) && (lba <= stop_lba))
        {
            break;
        }
    }
    //if tno=0, this address is not in any trk data, the address is leadout or pregap.
    if((tno==0)&&(St_cdrom_audio))
    {
        for(tno = last_tno;tno >1 ;tno--)
        {
            start_time = pdbGetStartTime(tno-1);
            stop_time  = pdbGetStopTime(tno);
            stop_lba   = MSF2LBA_Plus(&stop_time);
            if(tno > 1)
                start_lba = MSF2LBA_Plus(&start_time)-150L;
            else
            start_lba = 0L;

            //check from pre-gap/pause
            if((lba >= start_lba) && (lba <= stop_lba))
            {
                tno = tno-1;
                send_msg5S(1,"Not in TRK");//tcliu
                break;
            }
        }
    }

    return tno;
}

/*********************************************************************
* CheckTrackWriteMethod()
*
*   1) Find the Trk_num from lba
*   2) Check the Track's Write Method via DRAM cache, and Pregap.
*   3) If Fixed Packet Return 0.
*   4) If Variable Packet or TAO return 1.
*
* Global vars changed:
*   Trk_num:
*   Rw_seek_blk
*
* Para:     lba
* Returns:  write method (1: TAO, 2: FP, 3: VP) or NOTVALID message
*********************************************************************/
BYTE Track_No;
BYTE CheckTrackWriteMethod(long lba)
{
    BYTE  tno,wr_method,status,rt;
    TimeS time;
    long  pregap_lba;
    BYTE  cont_addr;
    BYTE  ContAddr;

    /* read mixed rom disc */
    tno = FindTrackNumber(lba);
    Track_No = tno;

    current_tno = tno;  // reduce process time for audio tranfer rate test

#if (DETECT_CDR == 1)
    if(pdbServoGetDiscType() != eATIPDisc)
    {
        return (TRKNO_NOTVALID);
    }
#endif

    if( !(pdbGetTrackContAddr(tno) & 0x40) )
    {
        Packet_Track = AUDIO_TRK;
        return AUDIO_TRK;
    }
    if(Special_Disc == eABEX702)
    {
        return (TRKNO_NOTVALID);
    }

    if(tno == 0)
    {
        Packet_Track = TRKNO_NOTFOUND;
        return TRKNO_NOTFOUND;     /* could not find track number */
    }

    status = pdbGetTrackDiagStatus(tno);

    if(status & eTS_Diag_TDB_done)
    {
        if(status & eTS_Diag_TDB)
        {
#if (MAX_READ_SPEED >= MIN_CD_PACKET_READ_SPD)
            if((Packet_Track == WR_METHOD_FP)||(Packet_Track == WR_METHOD_VP))
            {
                if(GetSpeedValue(Current_read_sp) < GetSpeedValue(MIN_CD_PACKET_READ_SPD))  //Only allow using higher speed, which is because packet track need more time to decode
                {
                    SetReadSpeed(MIN_CD_PACKET_READ_SPD);
                    SpeedCount = (CD_SPEED_UP_READY + CD_SPEED_DOWN_READY)/2;               //Prevent changing speed
                }
            }
#endif
            wr_method = pdbGetTrackWriteMethod(tno);
        }
        else
        {
            wr_method = TRKNO_NOTVALID;
        }
    }
    else
    {
        time = pdbGetStartTime(tno);
        pregap_lba  = MSF2LBA_Plus(&time);
        pregap_lba -= 75L;
        ContAddr = pdbGetTrackContAddr(tno);
        cont_addr = ContAddr & 0xF0;
        if(ReadPreGapTDB(pregap_lba, tno) == PASS)
        {
            Svr_pause_reqire = TRUE;
            if(TDB.valid == TRUE)
            {                           /* TDB is valid */
                wr_method = TDB.WriteMethod;
                switch(TDB.WriteMethod)
                {
                    case 0x91:                              /* Fixed packet */
                        status  = pdbGetTrackStatus(tno);   /* Packet + Fixed */
                        status |= (eTS_Packet + eTS_FixedPacket);
                        pdbSetTrackStatus(tno,status);
                        break;
                    case 0x90:                              /* Variable Packet */
                        status  = pdbGetTrackStatus(tno);   /* Packet */
                        status |= eTS_Packet;
                        pdbSetTrackStatus(tno,status);
                        break;
                    default:
                        status  = pdbGetTrackStatus(tno);   /* TA,SAO */
                        status &= (~(eTS_Packet+eTS_FixedPacket));
                        pdbSetTrackStatus(tno,status);
                        break;
                }

                status = pdbGetTrackDiagStatus(tno);        /* TDB valid  */
                status |= eTS_Diag_TDB;
                pdbSetTrackDiagStatus(tno,status);

                pdbSetTrackContAddr(tno,TDB.ContAddr);      /* CONT/ADR in Q subcode */
                pdbSetTrackWriteMethod(tno,TDB.WriteMethod); /* write method */
                pdbSetTrackDataMode(tno,TDB.DataMode);      /* Data mode */
                pdbSetTrackPacketSize(tno,TDB.PacketSize);  /* Packet size */
            }
            else
            {
                wr_method = TRKNO_NOTVALID;
                if (TDB.buff_NG == FALSE)                   /* Not found "TDI" */
                {
                    if (ContAddr == 0x0F)                   /* Unknown */
                    {
                        cont_addr = TDB.ContAddr & 0xF0;
                    }
                    if ((cont_addr & 0x40) == 0x40)         /* data recorded track */
                    {
                        status = pdbGetTrackDiagStatus(tno);    /* TDB valid  */
                        status |= eTS_Diag_TDB;
                        pdbSetTrackDiagStatus(tno,status);
                        pdbSetTrackContAddr(tno,TDB.ContAddr);
                        pdbSetTrackDataMode(tno,TDB.DataMode);  /* Data mode */
                        pdbSetTrackPacketSize(tno,0x00);        /* Packet size */
                    }
                    if (cont_addr == 0x50 || cont_addr == 0x70) /* Packet ? */
                    {
                        status  = pdbGetTrackStatus(tno);
                        status |= eTS_Packet;                   /* Packet */
                        pdbSetTrackStatus(tno,status);
                        pdbSetTrackWriteMethod(tno,0x90);       /* write method is variable packet */
                        wr_method = 0x90;
                    }
                }
            }
        }
        else
        {
            wr_method = TRKNO_NOTVALID;
        }

        status |= eTS_Diag_TDB_done;
        pdbSetTrackDiagStatus(tno,status);
    }

    switch(wr_method)
    {
        case TRKNO_NOTVALID:
            rt = TRKNO_NOTVALID;
            break;
        case 0x91:
            rt = WR_METHOD_FP;
            break;
        case 0x90:
            rt = WR_METHOD_VP;
            break;
        case 0x80:
            rt = WR_METHOD_TAO;
            break;
        default:
            rt = TRKNO_NOTVALID;
            break;
    }

    Packet_Track = rt;

    return rt;
}

/*********************************************************************
* Method2LBAToMethod1LBA()
*
* Translate Method2 LBA to method1 lba
*
* Pre condition:
*           address (lba) must be in a FP track
*           track number in Trk_num
*           track start address in TDB
*           packet size in TDB
*           method_1_lba = ((meth2_lba-track_start)/packet_size) * 7 + meth2_lba
*
* Params:   method 2 lba - (in)lba information stored lba
*
* Return:   method 1 lba  in second para
*           0 : success
*           1 : fail
*********************************************************************/
vBOOL Method2LBAToMethod1LBA(long meth2_lba, long *meth1_lba)
{
    BYTE    tno;
    long    start_lba,packet_size,packet_num;
    TimeS   start_time;

    tno = FindTrackNumber(meth2_lba);

    if(tno == 0) return FAIL;

    if (!(pdbGetTrackStatus(tno) & eTS_BlankTrack))
    if(!(pdbGetTrackStatus(tno) & eTS_FixedPacket)) return FAIL;

    start_time  = pdbGetStartTime(tno);
    start_lba   = MSF2LBA_Plus(&start_time);
    packet_size = pdbGetTrackPacketSize(tno);

    if(packet_size == 0L)
    {
        packet_size = 32L;
    }

    Iram_startlba_ck = start_lba;
    Iram_packetsize_ck = packet_size;

    packet_num  = (meth2_lba - start_lba) / packet_size;
    *meth1_lba  = packet_num * 7L + meth2_lba;

    return PASS;
}

/*********************************************************************
* Module:  ReadPreGapTDB()
*
* Description: This routine is used for reserve trk, not close trk ...etc.
*              1.Seek to pregap area
*              2:start buffering TDB block data start from page 0
*              3:analyze the TDB block and read TDB information.
*
* Note: disable interrupt before do this function!
*
* Return :  update the track_packet_info into structure.
*           including:fixed packet track,TAO or Variable Packet or blank tdb
*           PASS: successfully retrieve TDB info.
*           FAIL: can not find TDB
*********************************************************************/
vBOOL ReadPreGapTDB(long tmplba, BYTE tno)
{
    BYTE page,bf_retry,sk_retry;
    BYTE i;
    BYTE bcd_tno;
    BYTE buff_data[32];
    BYTE data[4];
    long packet_size;
    ULONG clock;
    ULONG tmup;
    SubQCodeS subq;
    vBOOL   temp_INT;
    USHORT  buf_cnt;
    USHORT  user_addr;
    BYTE    tmpDataMode;
    ULONG   work_addr;

    HostXfer.StartBlock.lba = tmplba;
    Rw_seek_blk.lba = tmplba;
    svoIramVar.subQEnableFlag = 1;
    bf_retry = sk_retry = 0;

    SeekToTarget();
    StartTimer(&tmup);
    while(ReadTimer(&tmup) < 1500L)
    {

        TDB.buff_NG = TRUE;
        TDB.valid = FALSE;
        buf_cnt = 0;

        if(EjectKeyInfo.PressedValid)
        {
            break;
        }

        Rw_seek_blk.lba = HostXfer.StartBlock.lba;
        if (SeekToTarget() == PASS)
        {                               /* seek success, start buffering */
            StartTimer(&clock);
            pdbSetSubCodeOK(FALSE);
            while(1){
                if(pdbGetSubcodeOK() == TRUE)
                {
                    pdbGetSubcodeInformation(&subq);
                    if ((subq.Tno == 0xFF) && (subq.Point == 0xFF))
                    {   /* Blank ? */
                        break;
                    }
                    bcd_tno = HEX2BCD(tno);
                    if (subq.Tno == bcd_tno)
                    {
                        TDB.ContAddr = (subq.Con <<4) + subq.Addr;
                        TDB.buff_NG = FALSE;
                        break;
                    }
                }

                if(ReadTimer(&clock)>100L)
                {
                    break;
                }
            }

            if(TDB.buff_NG == FALSE)
            {
                buf_cnt = Buffering( HostXfer.StartBlock.lba,10);
                if(buf_cnt)
                {
                    bf_retry++;
                    for(page = 0; page < (BYTE)buf_cnt ; page++)
                    {
                        temp_INT = GET_INT_ENABLE_MASK();
                        DISABLE_INTERRUPT();

                        work_addr = DATA_START_ADDR + page *0x800;
                        user_addr = USER_DATA_ADDR_MODE1;
                        if ((Iram_Rw_read_mode == eMode2Form1) || (Iram_Rw_read_mode == eMode2Form2))
                        {
                            user_addr = USER_DATA_ADDR_MODE2;
                        }

                        for(i=0; i<32; i++)
                        {           /* get track descriptor table */
                            buff_data[i] = SCSDRAM[work_addr +user_addr + i];
                        }
                        work_addr = PAGE_AUX_START_ADDR(page);
                        tmpDataMode = SCSDRAM[work_addr + AUX_HEADER_ADDR1 + 3];

                        SET_INT_ENABLE_MASK(temp_INT);

                        TDB.DataMode = tmpDataMode;
                        TDB.ContAddr = (subq.Con <<4) + subq.Addr;
                        /* find "TDI" */
                        for(i=0; i<16; i++)
                        {
                            if((buff_data[i] == 0x54) && (buff_data[i+1] == 0x44) && (buff_data[i+2] == 0x49))
                            {
                                /* check write method */
                                if(buff_data[i+9] == 0x80 || buff_data[i+9] == 0x90 || buff_data[i+9] == 0x91 || buff_data[i+9] == 0x00)
                                {
                                    TDB.WriteMethod = buff_data[i+9];

                                    if(TDB.WriteMethod == 0x91)     /* fixed packet */
                                    {
                                        data[0] = BCD2HEX(buff_data[i+10]);
                                        data[1] = BCD2HEX(buff_data[i+11]);
                                        data[2] = BCD2HEX(buff_data[i+12]);
                                        packet_size  = (long)data[0] & 0xFFL;
                                        packet_size <<= 8;
                                        packet_size |= (long)data[1] & 0xFFL;
                                        packet_size <<= 8;
                                        packet_size |= (long)data[2] & 0xFFL;
                                        TDB.PacketSize = packet_size;
                                    }
                                    else
                                    {
                                        TDB.PacketSize = 0L;
                                    }
                                    TDB.valid = TRUE;
                                    svoIramVar.subQEnableFlag = 0;
                                    return PASS;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (bf_retry > 3)
        {
            break;
        }
        if ((sk_retry < 20) && (sk_retry & 0x01))
        {
            HostXfer.StartBlock.lba -= 4;
        }
        sk_retry++;
    } /* time up loop */
    svoIramVar.subQEnableFlag = 0;
    return FAIL;
}
/*********************************************************************
* Module:  CheckDiscMaxLBA()
*
* Description: .
*
* Input:tmplba
*
* Return :  TRUE
*           FALSE
*********************************************************************/
BOOL  CheckDiscMaxLBA(long tmplba)
{
    TimeS ttime;
    long maxlba;
    BYTE tno;
    TimeS tmpTime;
    long tmpmaxlba;

    tno = pdbGetHighestRecordedTrkNumber();
    tmpTime = pdbGetStopTime(tno);
    tmpmaxlba = MSF2Count(&tmpTime) - 150;    /* Logical*/

    ttime = pdbGetATIPLeadOut();
    maxlba = MSF2Count(&ttime) - 150;         /* Logical*/

    if(tmpmaxlba > maxlba)
    {
        maxlba = tmpmaxlba;
    }

    if (tmplba > maxlba)
        return FAIL;
    else
        return PASS;

}

/**********************************************************************
* Function Name: TrackCompareMSFAddresses
*
* DESCRIPTION:
* Input :track_start_time
*        track_stop_time
*        target_time
* RETURNS:
********************************************************************** */
public BYTE TrackCompareMSFAddresses(  TimeS *track_start_time,
                TimeS *track_stop_time, TimeS *target_time)
{
    BYTE cmp_start_target, cmp_stop_target;

    cmp_start_target = mathCompareMSFPlus150(track_start_time,target_time);
    cmp_stop_target = mathCompareMSFPlus150(track_stop_time,target_time);

    if(cmp_start_target != GREATER && cmp_stop_target == GREATER)
        return EQUAL;

    if(cmp_start_target == GREATER)
        return GREATER;

    return SMALLER;
}

/**********************************************************************
* Function Name: CheckTrackMode
*
* DESCRIPTION:
* Input :   None
* RETURNS:  TRUE
*           FALSE
********************************************************************** */
BOOL CheckTrackMode(void)
{

    if( (pdbGetTrackConAd(current_tno) & 0x04) == 0)
        return TRUE;
    else
        return FALSE;
}

/**********************************************************************
* Function Name: UpdateDiscInfoData
*
* DESCRIPTION:
* Input :   None
* RETURNS:  None
********************************************************************** */
void UpdateDiscInfoData(void)
{
    St_12_cm = 0;
    St_Fashion = 0;

    switch (pdbServoGetDiscDiameter())
    {
        case e12cm:
            St_12_cm = 1;
            break;
        case e8cm:
            break;
        case eFashion:
            St_Fashion = 1;
            break;
        case eNoDisc:
            St_no_disc = 1;
            break;
        default:
            St_12_cm = 1;
            break;
    }

    current_tno = 1;
}


/*********************************************************************
*   Function:    Set_SpeedMDCont()
*   Description: Please refer to Read Speed Control Algorithm document.
*   Exceptions:
*   Input:
*********************************************************************/
#if(MSG_OUT_MODE != MSG_DISABLE)
private SpeedMDT PreSpeedMD;    //for debug
#endif
//retry to high speed time. when retry at lowest speed, do the speed change from lowest speed to high speed at first time.
ULONG SpdUpAtlowestSpdTime = ~((ULONG)SpdChgLowest2HighTime);
void Set_SpeedMDCont(void)
{
    BYTE    next_speed, accumulation;
    BYTE    lower_speed_value;
#if(READ_SPEED_CONTROL == READ_SPEED_CONTROL_VERSION_ONE)
    SHORT decode_pages_per_second;
    ULONG required_stream, lower_speed_decode_stream;// Unit: 1 = 1 sector = 2kb
    ULONG time;
    float     ratio;
#endif

#if(MSG_OUT_MODE != MSG_DISABLE)
    if(SpeedMD != PreSpeedMD)
    {
        SendMsgCn(SEND_HIF_CMD,10,0x800500,A2B(SpeedUpReady),SpeedDownReady,Disc_Speed.Read,
        SpeedMD,PreSpeedMD,BackUpSpeed,Current_read_sp,A2B(SpeedCount));
        SendMsgCn(SEND_HIF_CMD,4,0x800501,A4B(DataBuffer.SectorCount));
        PreSpeedMD = SpeedMD;
    }
#endif

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    return;
#endif

    if(ptrReadSpeedTablePointer == 0)
    {
#if(MSG_OUT_MODE != MSG_DISABLE)
        if(ptrReadSpeedTablePointer == 0)
        {
            //send_msg80(SEND_HIF_CMD,0x710806);
        }
        //else
        //    send_msg5SValue(SEND_HIF_CMD,"Don't ch speed",2,Iram_not_change_speed_timer)// much output during recording.
#endif
        return;
    }

    if((Current_read_sp == eSpStop) && (svoIramVar.svoCurrentSpeed != 0xff))// standby -> wake up
    {
        SpeedMD = Idle_f;
#if (EN_POWER_CONTROL == 0)
        ataPowerMode= IDLE_MODE;        // set power_mode = IDLE_MODE
#endif
    }
//if speed is not in range, don't change to next speed.
    if (svoSpeedVar.SpdInRange == FALSE)
    {
        SendMsg80(DEBUG_SVO_SPD_MSG,0x33130C);//SpdNotInRange
        return;
    }

    next_speed = Current_read_sp;

    switch(SpeedMD)
    {
        case Di_f:
//        case Write_f:
        case Set_sp_f:
        case StartUp_f:
        case BypassSpeedMD:

            StartTimer(&RedLineTimer);
            OutputStream = 0;
        return;

        case Standby_f:
            next_speed = eSpStop;
        break;

        case Wait_f:
            if(SpeedCount >= 0)
            {
                next_speed = BackUpSpeed;
                SpeedMD = AutoMode;
            }
            else if(SpeedCount <= (SpeedDownReady - 2))
            {  //give up speeding up to the target speed because of Decode status is very BAD.
                SpeedMD = AutoMode;
            }
        break;

        case Idle_f:
            next_speed = ptrReadSpeedTablePointer[0];    //lowest speed
        break;

        case MiddleMode:
            /*--------------------------------------------------------------------------
               After recording, the speed control must be changed from write to read.
               This mode finds a speed out in the read speed table and then changes to the speed.
            --------------------------------------------------------------------------*/
            next_speed = FindSpeedFromWriteToRead();
        break;
#if (SEQ_SEEK_SPEED_DOWN == 1)
        case Special_for_SeekSpeed:
            /*--------------------------------------------------------------------------
               the speed control change to seek speed after random read conunt > 30
            --------------------------------------------------------------------------*/
            if ((GetSpeedValue(Current_read_sp) <= GetSpeedValue(Disc_Speed.Read))
                 && (GetSpeedValue(Current_read_sp)> GetSpeedValue(Disc_Speed.SeekSpeed)))
            {
                send_msg5S(1,"SEEKSPEED");
                next_speed = Disc_Speed.SeekSpeed;
            }
            break;
#endif
        case Special_f:
#if (PCB == 710)||(PCB ==711)||(PCB == 817)
            if (GetSpeedValue(Current_read_sp) < GetSpeedValue(Disc_Speed.Read))
            {
                    next_speed = GetNextSpeed(Current_read_sp, 1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
            }
            else if (GetSpeedValue(Current_read_sp) > GetSpeedValue(Disc_Speed.Read))
            {
                    next_speed = GetNextSpeed(Current_read_sp, -1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
            }
            else
            {
                SpeedMD = AutoMode;
            }
#else
            next_speed = Disc_Speed.Read;    // set cd speed or special disc
#endif
        break;
#if(CEProduct == 0)
        case Special_for_CDSpeed_f:
#endif

        case AutoMode:
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
            if (RandomCount > CONTINUE_RANDOM_READ_CNT)
                break;
#endif // RANDOM_READ_BYPASS_CHECK_METHOD
            if(RandomRead)
            {
                //RandomRead = FALSE;
                StartTimer(&RedLineTimer);
                OutputStream = 0;
                /*--------------------------------------------------------------------------
                   Check if the disk is video or audio when the random read occured.
                   If the disk is video or audio, clear the SpeedCount to avoid changing speed
                   unnecessarily. It may forward, backward or jump to next section/song.
                --------------------------------------------------------------------------*/
#if(BD_ENABLE == 1)
                if(dvdVideoFlag || St_cdrom_video || St_cdrom_audio || bdVideoFlag || St_cdrom_cdplus)
#else
                if(dvdVideoFlag || St_cdrom_video || St_cdrom_audio || St_cdrom_cdplus)
#endif
                {
                    if(SpeedCount > 0)
                    {
                        SpeedCount = 0;
                        SpeedUpAccumulation = 0;
                    }
                }
            }

            /*--------------------------------------------------------------------------
               Check if the driver needs to change speed.
            --------------------------------------------------------------------------*/
            if(pdbGetCDRW())
                accumulation = CDRW_SPEED_UP_ACCUMULATION; // TBD? COMMENT! this code was following old speed control, it may be omitted in the future.
#if(BD_ENABLE == 1)
            else if(pdbDiscLoaded()== BD_DISC)
                accumulation = 2;
#endif
            else
                accumulation = 1;

            if (GetSpeedValue(Current_read_sp) > GetSpeedValue(Disc_Speed.Read))
                SpeedCount = SpeedDownReady-1;

            if( SpeedCount <= SpeedDownReady)
            {
#if(BD_ENABLE == 1)
                if(!bdVideoFlag)
#endif
                {
#if (Detect_AVCHD_DISC == 1)
                    if(!AVCHDFlag)
#endif
                    {
                        if(GetSpeedValue(Current_read_sp) > GetSpeedValue(ptrReadSpeedTablePointer[0]))
                        {
                            lower_speed_value = GetNextSpeed(Current_read_sp, -1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
#if (PCB == 710)||(PCB ==711)||(PCB == 817)
                            if (SpeedCount != SPEED_DOWN_FOR_POWER_SAVE || (lower_speed_value & e_CAV))
                            {
                                send_msg80(SEND_HIF_CMD,0x710801);  // speed down
                                next_speed = lower_speed_value;
                            }
#else
                            //send_msg5S(SEND_HIF_CMD,"speed down");      // for debug
                            send_msg80(SEND_HIF_CMD,0x710801);
                            next_speed = lower_speed_value;
#endif
                        }
                        //Retry higher speed when drive read error at lowest speed
                        //check this situation every 180 seconds.
                        else if ((GetSpeedValue(Current_read_sp)==GetSpeedValue(ptrReadSpeedTablePointer[0]))
                            &&(ReadTimer(&SpdUpAtlowestSpdTime) > (ULONG)SpdChgLowest2HighTime))
                        {
                            lower_speed_value = GetNextSpeed(Current_read_sp, 1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
                            if (SpeedCount != SPEED_DOWN_FOR_POWER_SAVE || (lower_speed_value & e_CAV))
                            {
                                if(GetSpeedValue(Disc_Speed.Read) >= GetSpeedValue(lower_speed_value))
                                {
                                    send_msg80(SEND_HIF_CMD,0x710801);  // speed down
                                    StartTimer(&SpdUpAtlowestSpdTime); //reset Speed up at lowest speed time.
                                    next_speed = lower_speed_value;
                                }
                            }
                        }
                        //Retry higher speed when drive read error at lowest speed.
                        SpeedUpAccumulation = 0;
                    }
                }
            }
            else if(SpeedCount >= SpeedUpReady)
            {
                if(GetSpeedValue(Current_read_sp) < GetSpeedValue(Disc_Speed.Read))
                {
//#if(READ_SPEED_CONTROL == READ_SPEED_CONTROL_VERSION_ONE)
                    if(DataBuffer.SectorCount < BOUNDARY_BETWEEN_PLAY_AND_COPY_MODE)
//#else
//#if(BD_ENABLE == 1)
//                    if((!(dvdVideoFlag || St_cdrom_video || St_cdrom_audio || bdVideoFlag)) || DataBuffer.SectorCount < BOUNDARY_BETWEEN_PLAY_AND_COPY_MODE)
//#else
//                    if((!(dvdVideoFlag || St_cdrom_video || St_cdrom_audio)) || DataBuffer.SectorCount < BOUNDARY_BETWEEN_PLAY_AND_COPY_MODE)
//#endif
//#endif
                    {
                        /*--------------------------------------------------------------------------
                           SpeedUpAccumulation is used to avoid speeding up when the host raises stream suddenly
                           and then decrease required stream.
                        --------------------------------------------------------------------------*/
                        if(SpeedUpAccumulation >= accumulation)
                        {
                            send_msg80(SEND_HIF_CMD,0x710802);  // speed up
                            SpeedUpAccumulation = 0;
#if (PCB == 710)||(PCB ==711)||(PCB == 817)
                            next_speed = GetNextSpeed(Current_read_sp, 1, ptrReadSpeedTablePointer, SpeedTableColumnSize);// speed up
#else
                            next_speed = GetNextSpeed(Current_read_sp, 2, ptrReadSpeedTablePointer, SpeedTableColumnSize);// speed up
#endif
                        }
                        else
                        {
                            //send_msg5SValue(SEND_HIF_CMD,"speed up ac++",1,SpeedUpAccumulation);    // for debug
                            send_msg80(SEND_HIF_CMD,0x710803); //speed up ac++
                            SpeedUpAccumulation++;
                            SpeedCount = 0;
                        }
                        break;
                    }
#if(CEProduct == 0)
                    if(SpeedMD == Special_for_CDSpeed_f)
                    {
                        send_msg80(SEND_HIF_CMD,0x7108F5);  // speed up
                        SpeedUpAccumulation = 0;
                        next_speed = GetNextSpeed(Current_read_sp, 1, ptrReadSpeedTablePointer, SpeedTableColumnSize);// speed up
                    }
#endif
                }
                SpeedUpAccumulation = 0;

#if(READ_SPEED_CONTROL == READ_SPEED_CONTROL_VERSION_ONE)
                /*--------------------------------------------------------------------------
                   The driver will speed down when the stream that host required is small:
                   1. If the timer, RedLineTimer, is greater than 5200(5.2 seconds), the read condition may
                       be not good. RecoverDecoderError() sometimes isn't called when some kind of decode
                       error occurred(often for CD).
                   2. Check if the DataBuffer.SectorCount  is greater than BUF_TOTAL_BLK_COUNT*0.7
                       in a while. BUF_TOTAL_BLK_COUNT*0.7 is the Red Line.
                   3. Check if the stream that host required is smaller than BUF_TOTAL_BLK_COUNT.
                       If it is greater, the driver may speed down to cause some delays for playing.
                       Driver can't decode during servo monitor the speed change.
                   4. Check if the stream of lower speed is greater than current stream that host required

                   The driver will speed down whe upper conditions are true.
                --------------------------------------------------------------------------*/
#if(CEProduct == 0)
                if((SpeedMD != Special_for_CDSpeed_f))
#endif
                {
                    time = ReadTimer(&RedLineTimer);
                    if(time >= 5000)// 5 seconds
                    {
                        if(time < 5200)
                        {
                            if(DataBuffer.SectorCount > (BUF_TOTAL_BLK_COUNT * 0.7))
                            {
                                required_stream = OutputStream / (time / 1000);    // Host required stream per second.
                                SendMsgCnB(SEND_HIF_CMD,4,0x800505,B4B(required_stream)); //required_stream

                                // required_stream/2 --> speed down + seek + start buffing + tolerance = 0.5 second approximately.
                                if((required_stream/2) < DataBuffer.SectorCount)
                                {
                                    switch(pdbDiscLoaded())
                                    {
                                        case CD_DISC:
                                            decode_pages_per_second = SP1X / 2;
                                        break;

                                        case DVD_DISC:
                                            decode_pages_per_second = SP1XDVD / 2;
                                        break;

                                        case BD_DISC:
                                            decode_pages_per_second = SP1XBD / 2;
                                        break;

                                        case HD_DISC:
                                            decode_pages_per_second = SP1XHD / 2;
                                        break;

                                        default:
                                            decode_pages_per_second = 0;// unknown disc, avoid changing the speed.
                                        break;
                                    }

                                    next_speed = GetNextSpeed(Current_read_sp, -1, ptrReadSpeedTablePointer, SpeedTableColumnSize);// get lower speed

                                    if(next_speed & e_CAV)
                                       ratio = 0.4;    // CAV and Partial CAV: the inner stream is smaller than outer. inner / outer is 0.4 and it includes the tolerance.
                                    else
                                    {
        #if (PCB == 710)||(PCB ==711)||(PCB == 817)
                                        if (!(next_speed & e_CAV ) && ((pdbDiscLoaded() == CD_DISC) || (pdbDiscLoaded() == DVD_DISC)))
                                        {   //not speed down to CLV for CD / DVD by stream
                                            next_speed = Current_read_sp;
                                        }
        #endif
                                        if(next_speed & e_ZONE)
                                            next_speed = ptrReadSpeedTablePointer[0];    // for ZCLV

                                       ratio = 1.0;
                                    }

                                    lower_speed_value = GetSpeedValue(next_speed);

                                    lower_speed_decode_stream = decode_pages_per_second * lower_speed_value * ratio;    // calculate inner decode stream
                                    SendMsgCnB(SEND_HIF_CMD,4,0x800506,B4B(lower_speed_decode_stream)); //lower_speed_decode_stream

                                    if(lower_speed_decode_stream <= required_stream)
                                       next_speed = Current_read_sp;
                                }
                            }
                        }
                        StartTimer(&RedLineTimer);
                        OutputStream = 0;
                    }
                }//(SpeedMD != Special_for_CDSpeed_f)
#if(CEProduct == 0)
                else
                {
                        StartTimer(&RedLineTimer);
                        OutputStream = 0;
                }
#endif
#endif
            }
        break;

        default:
            SendMsgCnB(SEND_HIF_CMD,1,0x800507,B1B(SpeedMD)); //Unknown SpeedMD

            next_speed = GetNextSpeed(Current_read_sp, 0, ptrReadSpeedTablePointer, SpeedTableColumnSize);

            if(next_speed == eSpStop)
                next_speed = ptrReadSpeedTablePointer[0];
        break;
    }

    if(Current_read_sp != next_speed)
    {
#if(MSG_OUT_MODE != MSG_DISABLE)
        SendMsgCn(SEND_HIF_CMD,9,0x800504,A2B(SpeedUpReady),SpeedDownReady,Disc_Speed.Read,
        BackUpSpeed,Current_read_sp,next_speed,A2B(SpeedCount));
        SendMsgCn(SEND_HIF_CMD,8,0x800505,A4B(DataBuffer.SectorCount),A4B(HostXfer.StartBlock.lba));
#endif
#if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
      if (pdbDiscLoaded() == DVD_DISC) 
      {
       	if(SpeedState  < 2)
       	{
        	SetReadSpeed(next_speed);
        	if(next_speed == e4XDCAV)
        	{
         		SpeedState ++;
         		send_msg5S(1,"SpeedState ++"); 
        	}
        	else
        	{
          	send_msg5S(1,"Speed = e8XDCAV");
        	}
       }
       else
       {
       		send_msg5S(1,"SpeedState cant change"); 
       }
     }
     else 
#endif
     {
        SetReadSpeed(next_speed);
    }
    }

#if (PCB == 710)||(PCB ==711)||(PCB == 817)
    if ( (SpeedMD > Wait_f) && (SpeedMD != Special_f)
        #if(CEProduct == 0)
        &&(SpeedMD != Special_for_CDSpeed_f)
        #endif
        #if (SEQ_SEEK_SPEED_DOWN == 1)
        &&(SpeedMD != Special_for_SeekSpeed)
        #endif
        )
#else
    if( SpeedMD > Wait_f )
#endif
    {
        SpeedMD = AutoMode;
    }

}
/*********************************************************************
*   Function:    RecoverDecoderError()
*   Description:
*   Exceptions:
*   Input:
*********************************************************************/
void RecoverDecoderError(void)
{
#if (EN_DVD_HIGH_JITTER_DETECT)
    if((SpeedCount > SPEED_DOWN_COUNT_LIMIT)&&((HIGH_JITTER_DISC&0x03)!=0x03))
#else
    if(SpeedCount > SPEED_DOWN_COUNT_LIMIT)
#endif
    {
        if( SpeedCount >= 0 )
            SpeedCount = -1;
        else
            SpeedCount--;

        if(SpeedUpReady < SPEED_UP_READY_LIMIT)
        {
            // Delay speed up.
            if(SpeedCount <= SpeedDownReady)
                SpeedUpReady += 5220;
            else
                SpeedUpReady += 512;
        }
    }
}

/*********************************************************************
*   Function:   CheckFinalSessionClosed
*   Description:
*   Exceptions:
*   Return :  TRUE Closed Area,  FALSE Open Area
*********************************************************************/
BYTE CheckFinalSessionClosed(void)
{
    if ((pdbGetFinalSessCloseInfo() == eNotSessionClosed) || (St_blank_disc == TRUE)) {
        return FALSE;
    }
    return TRUE;
}

/************************************************************************
    Name:  CNVTPageToLinear()

    Description:  Converts Page Number/Offset to Linear Address

    Parameter: PageNumber
               PageOffset - Page Area: 0-7FFh,
                            Aux Area : MLY(11b) 800h - 9FFh (200h BYTES)
                                       MLY(10b) 800h - BFFh (400h BYTES)
               ModeType (HOST, ENCODER, DECODER)
               The basic difference here is that the Mode Type determines
               where the AUX location (Linear Address) will be. This will
               apply for any address offset that is greater than the total
               bytes per page (0x800). If the address to be converted is
               in the PAGE area, the Mode Type should and will not make a
               difference.

    Returns:   LinearAddress

************************************************************************/
ULONG CNVTPageToLinear( USHORT  PageNumber, USHORT  PageOffset,
                        BYTE    ModeType )
{
    ULONG LinearAddress;
    BYTE StartPage, EndPage, TotalPages;
    USHORT AuxSize;

    /* DETERMINE MEMORY LAYOUT CONFIGURATION */

        AuxSize = 0x200; /* 200h Bytes Auxiliary *//*Automatic change from 2.5K to 3k page for C2PO*/


    switch (ModeType)
    {
        case HOST:
            /* DETERMINE START PAGE */
            StartPage = READ_REG(HAL_HSTREG_MAP->HSTBUFSPGL);

            /* DETERMINE STOP PAGE */
            EndPage = READ_REG(HAL_HSTREG_MAP->HSTBUFEPGL);

            /* DETERMINE TOTAL PAGES */
            TotalPages = EndPage - StartPage + 1;

            /* IN PAGE AREA */
            if (PageOffset < BYTES_PER_PAGE)
                LinearAddress = (PageNumber * BYTES_PER_PAGE) + PageOffset;
            /* IN AUX AREA */
            else
                LinearAddress = ((EndPage + 1) * BYTES_PER_PAGE) +          /* end of page area = start of aux area */
                                ((PageNumber - StartPage) * AuxSize)    +   /* go to start of requested aux index in the aux area */
                                (PageOffset - BYTES_PER_PAGE);              /* offset into aux area */
            break;

        case DECODER:
            /* DETERMINE START PAGE */
            StartPage = READ_FIELD(HAL_CDD_DSKBUFSPG);

            /* DETERMINE STOP PAGE */
            EndPage = READ_FIELD(HAL_CDD_DSKBUFEPG);

            /* DETERMINE TOTAL PAGES */
            TotalPages = EndPage - StartPage + 1;

            /* IN PAGE AREA */
            if (PageOffset < BYTES_PER_PAGE)
                LinearAddress = (PageNumber * BYTES_PER_PAGE) + PageOffset;
            /* IN AUX AREA */
            else
                LinearAddress = ((EndPage + 1) * BYTES_PER_PAGE) +          /* end of page area = start of aux area */
                                ((PageNumber - StartPage) * AuxSize)    +   /* go to start of requested aux index in the aux area */
                                (PageOffset - BYTES_PER_PAGE);              /* offset into aux area */
            break;
    }

    return (LinearAddress);
}

/**********************************************************************
*  CopyPersistentTableToDram
*
* Description: Copy "Persistend Data" located in Flash memory to area in DRAM Buffer
*
* Returns:  NONE
* Params:   NONE
*********************************************************************/
void CopyPersistentTableToDram(BYTE eTable)
{
    vBYTE   *flash_addr;
    USHORT i;

    CurTblOnDram = eTable;
    switch( eTable)
    {
#if (SUN_FUNC_TEST == 1)
        case eEEPROM_Formal_Table:
            if(EEPROM_Table[FT_CHECK_SUM_INITIAL_PATTERN_OFFSET] == FT_CHECK_SUM_INITIAL_PATTERN)
            {
                if((CheckEEPROMTagProtection(eEEPROM_Formal_Table) == PASS) &&
                   (CheckEEPROMSumProtection(eEEPROM_Formal_Table) == PASS))
                {
                    send_msg80(1,0x800F06); //Read from EEPROM OK
                    //Load data from Formal EEPROM
                    flash_addr = (vBYTE *)FLASH_BASE + AGAIN_SL_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
                }
                else if((CheckEEPROMTagProtection(eEEPROM_Backup_Table) == PASS) &&
                        (CheckEEPROMSumProtection(eEEPROM_Backup_Table) == PASS))
                {
                    send_msg80(1,0x800F07); //Read from backup EEPROM OK
                    //Load data from Backup EEPROM
                    flash_addr = (vBYTE *)FLASH_BASE + AGAIN_SL_BKUP_OFFSET;
                }
                else
                {
                    send_msg80(1,0x800F08); //Fill EEPROM with Initial Value
                    //Fill DRVPARA_DATA with Initial Value
                    FillEEPROMWithInitialValue();
                    //Re-write to EEPROM
                    fgEEPROMChg = TRUE;
                    CloseWritableSegmentTable(eEEPROM_Formal_Table);
                    CloseWritableSegmentTable(eEEPROM_Backup_Table);
                    return;
                }
            }
            else
            {
                //First time to write EEPROM
                flash_addr = (vBYTE *)FLASH_BASE + AGAIN_SL_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            }
            break;
        case eEEPROM_Backup_Table:
            flash_addr = (vBYTE *)FLASH_BASE + AGAIN_SL_BKUP_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
        case eKey_parameter_Backup_Table:
            flash_addr = (vBYTE *)FLASH_BASE + KEY_PARA_BKUP_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
#endif
#if  SERVOKEYPARAMETER == 1
        case eKey_parameter_Table:
            flash_addr = (vBYTE *)FLASH_BASE + KEY_PARA_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
#endif
        case ePersistent_Talbe:
            flash_addr = (vBYTE *)FLASH_BASE + PARA_BLK_START_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
#if (SUN_FUNC_TEST == 1)            
        case ePersistent_Backup_Talbe:
            flash_addr = (vBYTE *)FLASH_BASE + PARA_BLK_BKUP_START_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
#endif            
#if (ENABLE_AACS == 1)
        case eMKB_Table:
            flash_addr = (vBYTE *)FLASH_BASE + MKB_BLK_START_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
        case eAACS_Table:
            flash_addr = (vBYTE *)FLASH_BASE + PARA_BLK_START_OFFSET;      // Flash Rom  Persistent data start address, 1 sector size = 0x1000
            break;
#endif
        default:
            //send_msg5S(1,"no support");
            send_msg80(1,0x710304);
            return;
    }
    Iram_Buf_dram_addr = DRVPARA_DATA;               /* start of the buffer scratch area */

    for(i = 0; i < 0x1000; i++)                  /* copy 4K bytes to SDRAM parameter area */
    {
        SCSDRAM[DRVPARA_DATA+i] = (BYTE)flash_addr[i];
    }

}

/**********************************************************************
*  CopyAacsKeysToDram
*
* Description: Copy "AACS Keys" located in Flash memory to area in DRAM Buffer
*
* Returns:  NONE
* Params:   NONE
*********************************************************************/
ULONG CopyAacsKeysToDram(BYTE *KeySize)
{
#if (ENABLE_AACS == 1)
    ULONG   Address;
    vBYTE   *flash_addr;
    BYTE    i;

    flash_addr = (vBYTE *)FLASH_BASE + PARA_BLK_START_OFFSET + PARA_AACSKEY_OFFSET;      // Flash Rom AACS key data start address

    Iram_Buf_dram_addr = DRVPARA_DATA + PARA_AACSKEY_OFFSET;               /* start of the buffer scratch area */

    for(i = 0; i < FLASH_AACSKEY_SIZE; i++)
    {
        SCSDRAM[DRVPARA_DATA+PARA_AACSKEY_OFFSET+i] = (BYTE)flash_addr[i];
    }

    *KeySize = FLASH_AACSKEY_SIZE;
    Address = (ULONG)(SCSDRAM + DRVPARA_DATA + PARA_AACSKEY_OFFSET);

    return Address;
#else
    return 0;
#endif  /* ENABLE_AACS == 1 */
}

/*********************************************************************
* Name:         LoadMkbFromFlash
*
* Description:  Load MKB from flash into the data buffer
*
* Params:       None
*
* Returns:      None
*
* Notes:
*
*********************************************************************/
void LoadMkbFromFlash(void)
{
#if (ENABLE_AACS == 1)
    vBYTE   *flash_addr;
    BYTE    *buf;
    USHORT  i;

    flash_addr = (vBYTE *)FLASH_BASE + MKB_BLK_START_OFFSET;
    buf = (BYTE *)(SCSDRAM_ADDRESS);

    for(i = 0; i < MKB_BLK_SIZE; i++)                  /* copy MKB to the ring buffer */
    {
        SCSDRAM[i] = (BYTE)flash_addr[i];
    }
#endif  /* ENABLE_AACS == 1 */
}

/********************************************************************************/
/*  CDReturnMediaOffsetForRead                                                  */
/* Description: Determines the row offset into the CDSelSpeed_tbl_R table       */
/* based on the media type in the drive.                                        */
/*                                                                              */
/* WARNING: If you change this function, you have to change "Set_SpeedMDCont()" */
/*                                                                              */
/* Input: None                                                                  */
/* Return: Row Offset                                                           */
/********************************************************************************/
BYTE CDReturnMediaOffsetForRead(void)
{
BYTE media;

    if ((St_cdrom_data)||(St_cdrom_mixed))
    {
        if (CheckFinalSessionClosed() == TRUE)
        {
            media = eCDROM_R;       // row 0
            if (pdbGetCDRW() == TRUE)
                media = eCDRW_R;    // row 1
        }
        else//Open sesson
        {
            media = eCDRWOpen_R; // row 3
        }
    }
    else //audio Extraction disc, St_cdrom_cdplus(cd-extra)
    {
        if(St_cdrom_cdplus)//St_cdrom_cdplus(cd-extra)
        {
            media = eCDRW_R;        //1,40x
        }
        else if (CheckFinalSessionClosed() == TRUE)
        {
            media = eCDDAExt_R;     // row 4
            //for CDRW extract audio speed should same to CDRW data
            if (pdbGetCDRW() == TRUE)
                media = eCDRW_R;    // row 1
        }
        else
        {
            media = eCDRWOpen_R;    //3,40x
        }
    }
    return(media);
}


/********************************************************************************/
/*  InitCopyStartEndPages                                                       */
/* Description: Initializes the Copy Start and End pages.  Must first check     */
/* check if the copy block is enabled. If not then it must be enabled before    */
/* loading the pointers                                                         */
/*                                                                              */
/* Input:  None                                                                 */
/* Return: None                                                                 */
/********************************************************************************/
void InitCopyStartEndPages(void)
{
    if(READ_FIELD(HAL_GLOBAL_CPYCKSTP))                     /* Is copy block currently enabled */
    {
        WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);                /* Enable copy clock */
        WRITE_FIELD(HAL_CPYCMP_CPYSPG, Iram_usiRingSP);     /* Copy Buffer Start Page */
        WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */
        WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 1);                /* Disable copy clock */
    }
    else
    {
        WRITE_FIELD(HAL_CPYCMP_CPYSPG, Iram_usiRingSP);     /* Copy Buffer Start Page */
        WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */
    }
}


/************************************************************************
    Name:       LinearPatternFill()

    Description: Function to fill data pattern with linear mode, no page wrap-around
    Returns:
            None
    note: If the page to be filled is outside User data area, you must use linear mode
*************************************************************************/
void LinearPatternFill(USHORT trgpage,USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
{
    BOOL IE;

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);  /* turn on Copy clock */
    WRITE_FIELD(HAL_CPYCMP_CPYTGT, trgpage);       /* page, target register set */
    WRITE_FIELD(HAL_CPYCMP_CPYTOF, trgofset);     /* arget offset register set*/
    WRITE_FIELD(HAL_CPYCMP_CPYPCNT, pgnum - 1);   /* page count set*/
    WRITE_FIELD(HAL_CPYCMP_CPYWCNT, wordnum);      /* set word count */
    WRITE_FIELD(HAL_CPYCMP_CPYPAT, pattern);       /* set pattern w. page number */

    St_pfil_done = FALSE;
    IE = GET_INT_ENABLE_MASK();
    ENABLE_INTERRUPT();

    WRITE_FIELD(HAL_CPYCMP_SFLAG, 0);     /* no wrap around */
    WRITE_FIELD(HAL_CPYCMP_TFLAG, 0);     /* no wrap around */
    WRITE_FIELD(HAL_CPYCMP_PFILSTRT, 1);  /* start pattern fill*/

    /* Make sure interrupt is enabled before calling */
    while (!St_pfil_done)
    {
//      ExitProcess();
    }
    St_pfil_done = FALSE;

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 1);  /* turn on Copy clock */

    SET_INT_ENABLE_MASK(IE);
}


/*********************************************************************
* Function:  GetNextSpeed()
* Description:This function finds out the next speed of the speed table.
                    If the parameter, level, is postive, it meas to find out the higher speed.
                    If the parameter, level, is negative, it meas to find out the lower speed.

* Exceptions: If the parameter, speed, doesn't exist in the speed table, it will return
                    the eSpStop. If the level is over the speed table boundary, it will return
                    the max speed or min speed in the speed table. If the speed_table
                    parameter doesn't point to a speed table, it will return eSpStop.

* Input: speed, level, speed_table.
* Output: Return the next speed.
*********************************************************************/

BYTE GetNextSpeed(BYTE speed, SBYTE level, const BYTE* speed_table, BYTE array_size)
{
    BYTE   array_boundary;
    SBYTE i;

    if(speed_table == 0)
        return eSpStop;

    if(level >= 0)
    {
        array_boundary = array_size - 1;
        do
        {
            for( i = array_boundary; i >= 0; i--)// find speed index out.
            {
                if(speed == speed_table[i])
                break;
            }

            if(i < 0)// The parameter, speed, doesn't exist in the speed table, so return the eSpStop.
                return eSpStop;

            if(i + level >= array_boundary)
            {
                i = array_boundary;
                break;
            }

            if(level <= 1)
            {
                i += level;
                break;
            }
            speed = speed_table[i + 1];
        }while(--level);
    }
    else
    {
        do
        {
            for( i = 0; i < array_size; i++ )// find speed index out.
            {
                if(speed == speed_table[i])
                    break;
            }

            if(i >= array_size)// The parameter, speed, doesn't exist in the speed table, so return the eSpStop.
                return eSpStop;

            if( i + level <= 0)
            {
                i = 0;
                break;
            }

            if(level >= -1)
            {
                i += level;
                break;
            }
            speed = speed_table[i - 1];
        }while(++level);
    }

    return speed_table[i];
}

/*********************************************************************
* Function:  FindSpeedFromWriteToRead()
* Description: After recording, the speed control must be changed from write to read.
                     This function depends on write speed to find the the same or lower speed
                     out, but it doesn't  consider the speed type(CAV, CLV...etc).

* Exceptions: If ptrReadSpeedTablePointer is null, this function will return eSpStop.

* Output: Return the speed.
*********************************************************************/
BYTE FindSpeedFromWriteToRead(void)
{
    BYTE  i, speed;

    if(ptrReadSpeedTablePointer == 0)
    {
        send_msg80(1,0x710806); //ptrReadSpeedTablePointer == 0
        return eSpStop;
    }

    speed = ptrReadSpeedTablePointer[0];

    for(i = 0; i < SpeedTableColumnSize; i++)
    {
        if(GetSpeedValue(speed) == GetSpeedValue(Disc_Speed.Write2))
            break;

        if(GetSpeedValue(speed) > GetSpeedValue(Disc_Speed.Write2))
        {
            speed = GetNextSpeed(speed, -1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
            break;
        }

        if(speed == ptrReadSpeedTablePointer[SpeedTableColumnSize - 1])
            break;

        speed = GetNextSpeed(speed, 1, ptrReadSpeedTablePointer, SpeedTableColumnSize);
    }
    send_msg80(SEND_HIF_CMD,0x710804); //Find a properly speed
    send_msg54(SEND_HIF_CMD, SpeedTableColumnSize, i, Disc_Speed.Write2, speed);

    return speed;
}

/*********************************************************************
* Function:  BYTE CompareStringInMemory(const BYTE* memory, const char* string, USHORT size)
* Description: This function checks if the string is existing in the memory location.

* Exceptions:

* Output: Return TRUE when the string is existing in the memory location
*********************************************************************/
BYTE CompareStringInMemory(const BYTE* memory, const char* string, USHORT size)
{
    const BYTE*     point_to_memory;
    const BYTE*     tmp_memory;
    const char*      point_to_string;
    const char*      tmp_string;
    USHORT   count, tmp_count;

    if(memory == 0 || string == 0 || size == 0)
        return FALSE;

    point_to_memory = memory;
    point_to_string = string;
    count = 0;

    while(count < size)
    {
        tmp_memory = point_to_memory;
        tmp_string = point_to_string;
        tmp_count = count;

        while(*tmp_memory == *tmp_string)
        {
            tmp_memory++;
            tmp_string++;
            tmp_count++;

            if((*tmp_string == 0) || (tmp_count >= size))
                goto end;
        }

        point_to_memory++;
        count++;
    }

end:
    return (!(*tmp_string));
}
/*********************************************************************
* Function:  BYTE SpecialDiscInformationParser(const BYTE* table)
* Description: This function checks if the disc is corresponding to one of  Special Disc Information
                    in the table.

* Exceptions:

* Output: Return the special disc ID when Special Disc Information is corresponding to the disc.
*********************************************************************/
BYTE SpecialDiscInformationParser(const BYTE* table)
{
    const BYTE* ptr_SDI;
    BYTE buffer_result, parser_result, seek_retry, checked_buffer_count;
    PSNs psn;
    TimeS track_time;
    ULONG timer, mem_address;

    BYTE disc_restriction, check_method, number_of_session, number_of_track, return_ID, indicator;
    BYTE buffer_count, check_size, track_number;
    const char* string;
    ADDRESS_T start_LBA, end_LBA;

    if(table == 0)
        return eNormalD;

#if (DVD_RAM_READ == 1)
    if (pdbServoGetfDiscKind() == eDVDRAMDisc)
        return eNormalD;
#endif

    ptr_SDI = table;
    parser_result = eNormalD;
    checked_buffer_count = 0;

    while(*ptr_SDI != END_SDI_CHECK)
    {
        indicator =*ptr_SDI;
        ptr_SDI++;

        disc_restriction = *ptr_SDI;
        ptr_SDI++;

        check_method = *ptr_SDI;
        ptr_SDI++;

        number_of_session = *ptr_SDI;
        ptr_SDI++;

        number_of_track = *ptr_SDI;
        ptr_SDI++;

        if(indicator == CONTINUE_SDI_CHECK)
        {
            if(return_ID != *ptr_SDI)
                indicator = PASS_THIS_ITEM;
        }

        return_ID = *ptr_SDI;
        ptr_SDI++;

        start_LBA.byte.u = *ptr_SDI;
        ptr_SDI++;
        start_LBA.byte.h = *ptr_SDI;
        ptr_SDI++;
        start_LBA.byte.m = *ptr_SDI;
        ptr_SDI++;
        start_LBA.byte.l = *ptr_SDI;
        ptr_SDI++;

        if(check_method == CHECK_STRING)
        {
            buffer_count = *ptr_SDI;
            ptr_SDI++;

            check_size = *ptr_SDI;
            ptr_SDI++;

            string = (const char*) ptr_SDI;
            while(*ptr_SDI != '\0')
                ptr_SDI++;
        }
        else
        {
            end_LBA.byte.u = *ptr_SDI;
            ptr_SDI++;
            end_LBA.byte.h = *ptr_SDI;
            ptr_SDI++;
            end_LBA.byte.m = *ptr_SDI;
            ptr_SDI++;
            end_LBA.byte.l = *ptr_SDI;
            ptr_SDI++;

            track_number = *ptr_SDI;
        }
        ptr_SDI++;// point to next indicator

        switch(indicator)
        {
            case PASS_THIS_ITEM:
            continue;

            case NEW_SDI_CHECK:
                checked_buffer_count = 0;
            break;

            case CONTINUE_SDI_CHECK:
                if(parser_result == eNormalD)
                    continue;

                checked_buffer_count--;// check the memory location of the previous SDI that result is true
            break;

            case END_SDI_CHECK:

            default:
            return eNormalD;
        }

        parser_result = eNormalD;
        seek_retry = 0;
        buffer_result = FALSE;

        if(pdbDiscLoaded() == CD_DISC)
        {// CD
            switch(disc_restriction)
            {
                case ALL_KINDS_OF_CD_DISCS:
                break;

                case NON_WOBBLE_DISC:
                    if(pdbServoGetDiscType() == eNonATIPDisc)
                        break;
                continue;

                case WOBBLE_DISC:
                    if(pdbServoGetDiscType() == eATIPDisc)
                        break;
                continue;

                case DATA_CD_DISC:
                    if(St_cdrom_data)
                        break;
                continue;

                case NON_WOBBLE_DATA_CD_DISC:
                    if(pdbServoGetDiscType() == eNonATIPDisc && St_cdrom_data)
                        break;
                continue;

                case WOBBLE_DATA_CD_DISC:
                    if(pdbServoGetDiscType() == eATIPDisc && St_cdrom_data)
                        break;
                continue;

                case AUDIO_CD_DISC:
                    if(St_cdrom_audio)
                        break;
                continue;

                case NON_WOBBLE_AUDIO_CD_DISC:
                    if(pdbServoGetDiscType() == eNonATIPDisc && St_cdrom_audio)
                        break;
                continue;

                case WOBBLE_AUDIO_CD_DISC:
                    if(pdbServoGetDiscType() == eATIPDisc && St_cdrom_audio)
                        break;
                continue;

                default:
                continue;
            }

            if(number_of_session != PASS_THIS_ITEM)
            {
                if( pdbGetSessionCounter() != number_of_session)
                    continue;
            }

            if(number_of_track != PASS_THIS_ITEM)
            {
                if(pdbGetHighestRecordedTrkNumber() != number_of_track)
                    continue;
            }

            if(check_method == CHECK_STRING)
            {//CHECK_STRING
                if(start_LBA.lba > DataBuffer.CacheStartLBA && ((start_LBA.lba + buffer_count - 1) <= DataBuffer.CacheEndLBA))// check buffer
                {
                    buffer_result = TRUE;
                }
                else
                {
                    StopDecoder();
                    ClearDataBuffer();

                    HostXfer.StartBlock.lba = start_LBA.lba;
                    Rw_seek_blk.lba = HostXfer.StartBlock.lba;

                    do
                    {
                        seek_retry++;
                        if (SeekToTarget() == PASS)
                        {
                            if (StartBufferingFromTarget() == 0)/* returns 0(OK), NonZero(ERROR) */
                                break;
                        }
                    }while(seek_retry < 3);

                    StartTimer(&timer);
                    while((DataBuffer.SectorCount < buffer_count) && (Rw_dec_stop == FALSE))
                    {
                        ExitProcess();
                        if(ReadTimer(&timer) > 100)
                            break;
                    }

                    if(DataBuffer.SectorCount >= buffer_count)// check buffer again
                        buffer_result = TRUE;
                }

                if(buffer_result)
                {
                    do
                    {
                        mem_address = DATA_START_ADDR + (2048 * (start_LBA.lba - DataBuffer.CacheStartLBA - 1 + checked_buffer_count)) + SCSDRAM_ADDRESS;

                        if(CompareStringInMemory((const BYTE*) mem_address, string, check_size * 8))
                            parser_result = return_ID;

                        checked_buffer_count++;
                    }while(checked_buffer_count < buffer_count &&  parser_result == eNormalD);
                }
                else
                    return eNormalD;
            }
            else if(check_method == CHECK_TRACK_POSITION)
            {//CHECK_TRACK_POSITION
                track_time = pdbGetStartTime(track_number);
                if(MSF2LBA_Plus(&track_time) == start_LBA.lba)
                {
                    track_time = pdbGetStopTime(track_number);
                    if(MSF2LBA_Plus(&track_time) == end_LBA.lba)
                        parser_result = return_ID;
                }
            }
            else
                return eNormalD;
        }
        else if(pdbDiscLoaded() == DVD_DISC)
        {// DVD
            switch(disc_restriction)
            {
                case ALL_KINDS_OF_DVD_DISCS:
                    if(pdbDiscLoaded() == DVD_DISC)
                        break;
                continue;

                case NON_WOBBLE_DVD_DISC:
                    if(pdbServoGetDiscType() == eNonADIPLPPDisc)
                        break;
                continue;

                case WOBBLE_DVD_DISC:
                    if(pdbServoGetDiscType() == eADIPDisc || pdbServoGetDiscType() == eLPPDisc)
                        break;
                continue;

                default:
                continue;
            }

            if(number_of_session != PASS_THIS_ITEM)
            {
                if(pdbGetDVDSessionCount() != number_of_session)
                    continue;
            }

            if(number_of_track != PASS_THIS_ITEM)
            {
                if(pdbGetDVDHighestRecordedTrackNumber() != number_of_track)
                    continue;
            }

            if(check_method == CHECK_STRING)
            {//CHECK_STRING
                if(start_LBA.lba >= DataBuffer.CacheStartLBA && ((start_LBA.lba + buffer_count - 1) <= DataBuffer.CacheEndLBA))// check buffer
                {
                    buffer_result = TRUE;
                }
                else
                {
                    if(DVD_BUFFERING() == ON)
                        dvdStopDiscTransfer();

                    ClearDataBuffer();

                    psn = dvdConvertLBAtoPSN(start_LBA.lba);

                    dvdDecFlag.fREADING_LEADIN = TRUE;
                    dvdDiscRequest.TargetID.Long = psn.PSN;
                    dvdDiscRequest.Type            = INIT_READ;
                    dvdDiscRequest.TargetLayer     = psn.Layer;
                    dvdDiscRequest.EccBlockCount   = (buffer_count / ONE_DVD_BLOCK) + 2;

                    dvdInitializeHostBuffer(start_LBA.lba);

                    if(dvdStartDiscTransfer() == BUF_GOOD)
                    {
                        StartTimer(&timer);
                        while((DVD_BUFFERING() == ON))
                        {
                            ExitProcess();
                            if(ReadTimer(&timer) > 100)
                            {
                                dvdStopDiscTransfer();
                                break;
                            }
                        }
                        buffer_result = TRUE;
                    }
                    dvdDecFlag.fREADING_LEADIN = FALSE;
                }

                if(buffer_result)
                {
                    do
                    {
                        mem_address = DVD_DATA_BUFFER_START + (2048 * (start_LBA.lba - DataBuffer.CacheStartLBA + checked_buffer_count)) + SCSDRAM_ADDRESS;

                        if(CompareStringInMemory((const BYTE*) mem_address, string, check_size * 8))
                            parser_result = return_ID;

                        checked_buffer_count++;
                    }while(checked_buffer_count < buffer_count &&  parser_result == eNormalD);
                }
                else
                    return eNormalD;
            }
            else  if(check_method == CHECK_TRACK_POSITION)
            {//CHECK_TRACK_POSITION
                if(dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(track_number)) == start_LBA.lba)
                {
                    if(dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(track_number)) == end_LBA.lba)
                        parser_result = return_ID;
                }
            }
            else
                return eNormalD;
        }
#if(BD_ENABLE == 1)
        else if(pdbDiscLoaded() == BD_DISC)
        {// BD
            switch(disc_restriction)
            {
                case ALL_KINDS_OF_BD_DISCS:
                    if(pdbDiscLoaded() == BD_DISC)
                        break;
                continue;

                case NON_WOBBLE_BD_DISC:
                    if(pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
                        break;
                continue;

                case WOBBLE_BD_DISC:
                    if(pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                        pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
                        break;
                continue;

                default:
                continue;
            }

            if(number_of_session != PASS_THIS_ITEM)
            {
                if(pdbGetBDSessionCount() != number_of_session)
                    continue;
            }

            if(number_of_track != PASS_THIS_ITEM)
            {
                if(pdbGetBDHighestRecordedTrackNumber() != number_of_track)
                    continue;
            }

            if(check_method == CHECK_STRING)
            {//CHECK_STRING
                if(start_LBA.lba >= DataBuffer.CacheStartLBA && ((start_LBA.lba + buffer_count - 1) <= DataBuffer.CacheEndLBA))// check buffer
                {
                    buffer_result = TRUE;
                }
                else
                {
                    if(BD_BUFFERING() == ON)
                        bdStopDiscTransfer();

                    ClearDataBuffer();

                    psn = bdConvertLBAtoPSN(pdbGetBDClusterStart(start_LBA.lba));

                    bdDiscRequest.TargetID.Long = psn.PSN;
                    bdDiscRequest.Type            = INIT_READ;
                    bdDiscRequest.TargetLayer     = psn.Layer;
                    bdDiscRequest.ClusterCount   = (buffer_count / ONE_BD_CLUSTER) + 2;

                    bdInitializeHostBuffer(start_LBA.lba);

                    if(bdStartDiscTransfer() == BUF_GOOD)
                    {
                        StartTimer(&timer);
                        while((DataBuffer.SectorCount < buffer_count) && (BD_BUFFERING() == ON))
                        {
                            ExitProcess();
                            if(ReadTimer(&timer) > 100)
                                break;
                        }
                    }

                    if(DataBuffer.SectorCount >= buffer_count)// check buffer again
                        buffer_result = TRUE;
                }

                if(buffer_result)
                {
                    do
                    {
                        mem_address = (2048 * (start_LBA.lba - pdbGetBDClusterStart(start_LBA.lba) + checked_buffer_count)) + SCSDRAM_ADDRESS;
                        if(CompareStringInMemory((const BYTE*) mem_address, string, check_size * 8))
                            parser_result = return_ID;

                        checked_buffer_count++;
                    }while(checked_buffer_count < buffer_count &&  parser_result == eNormalD);
                }
                else
                    return eNormalD;
            }
            else  if(check_method == CHECK_TRACK_POSITION)
            {//CHECK_TRACK_POSITION
                if(bdConvertPSNtoLBA(pdbGetBDTrackStartPSN(track_number)) == start_LBA.lba)
                {
                    if(bdConvertPSNtoLBA(pdbGetBDTrackEndPSN(track_number)) == end_LBA.lba)
                        parser_result = return_ID;
                }
            }
            else
                return eNormalD;
        }
#endif //BD
        if(*ptr_SDI != CONTINUE_SDI_CHECK && parser_result != eNormalD)
            break;
    }

    return parser_result;
}

/*--------------------------------------------------------------------
  Name: ZeroFillDRAM()
  Description:
    Zero fills SDRAM
  Inputs:
    start_page - starting page
    size - number of pages to zero fill
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ZeroFillDRAM(USHORT start_page, USHORT size)
{
    if (size == 0)  return;

        WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0        );        /* turn on Copy clock */
        WRITE_FIELD(HAL_CPYCMP_CPYTGT, start_page);     /* Target Start Page */
        WRITE_FIELD(HAL_CPYCMP_CPYTOF, 0x00);                /* Target Offset */
        WRITE_FIELD(HAL_CPYCMP_CPYPCNT, size-1);      /* Spec: Total Page Count - 1 */
        if(pdbDiscLoaded() == BD_DISC)
            WRITE_FIELD(HAL_CPYCMP_CPYWCNT, 0x400);              /* Target Word Count = 400h Page Words */
        else
            WRITE_FIELD(HAL_CPYCMP_CPYWCNT, 0x500);              /* Target Word Count = 400h Page Words + 100h Aux Words */

    #if (MODULE_TEST == 1)
    {
        USHORT pattern;
        pattern = RandomNumberGenerator(1) << 8;
        pattern += RandomNumberGenerator(0);
        send_msg54(SEND_ATAPI_CMD,'p','t', BHIGH(pattern), pattern);
        WRITE_FIELD(HAL_CPYCMP_CPYPAT, pattern);  /* Random Pattern */
    }
    #else
        WRITE_FIELD(HAL_CPYCMP_CPYPAT, 0x0000);  /* Zero Pattern */
    #endif

    WRITE_FIELD(HAL_CPYCMP_SFLAG, 1);                  /* Page/Aux Addressing SFlag = 1 */
    WRITE_FIELD(HAL_CPYCMP_TFLAG, 1);                  /* Page/Aux Addressing TFlag = 1 */

        St_pfil_done = FALSE;                                   /* reset flag to 0 */

    WRITE_FIELD(HAL_CPYCMP_PFILSTRT, 1);             /* start pattern fill */

        while (!St_pfil_done)                                   /* poll flag */
            ExitProcess();

        St_pfil_done = FALSE;                                   /* reset flag to 0 */

     WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,1);                 /* turn off Copy clock */
}

/*********************************************************************
* Function:
* Description:
* Input:
* Exceptions:
* Output:
*********************************************************************/
extern struct dvdDecFlagS      volatile    dvdDecFlag;
#if 0
BYTE DVDBufferData(BufferRequire_S br)
{
    BYTE result;
    ULONG timer;
    PSNs psn;

    StartTimer(&timer);
    while(ReadTimer(&timer) < br->TimeUp)
    {

    }
            dvdInitializeHostBuffer(start_lba);
            psn = dvdConvertLBAtoPSN(start_lba);

            dvdDecFlag.fREAD_AHEAD = TRUE;// avoid read error recovery
            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            dvdDiscRequest.TargetID.Long = psn.PSN;
            dvdDiscRequest.Type = INIT_READ_AHEAD;
            dvdDiscRequest.TargetLayer = psn.Layer;
            dvdDiscRequest.EccBlockCount = (length / ONE_DVD_BLOCK) + 1;

            if(dvdStartDiscTransfer() == BUF_GOOD)
            {
                while(DVD_BUFFERING() == ON)
                    ExitProcess();
            }

    return result;
}
#endif
/*********************************************************************
* Function:
* Description:
* Input:
* Exceptions:
* Output:
*********************************************************************/
BYTE BufferData(const ULONG start_lba, const ULONG length, const BYTE default_monitor)
{
//const BYTE waiting_for_start_buffering_time = 200;// 200ms

    BYTE result;
    PSNs psn;
    ULONG backup_SectorCount;
    ULONG dvd_emain_buffer_count, stop_buffering_timer;

    send_msg5SValue(1,"$sLBA",4,start_lba);
    send_msg5SValue(1,"$length",4,length);

    result = GEN_FAIL;
    dvd_emain_buffer_count = 0;

    StopDecoder();
    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            /* CDInitializeHostBuffer */
            ClearDataBuffer();
            DataBuffer.CacheStartLBA = start_lba - 1;
            DataBuffer.CacheEndLBA   =  start_lba - 1L;

            /* set seek */
            HostXfer.StartBlock.lba = start_lba;
            Rw_seek_blk.lba = HostXfer.StartBlock.lba;

            if(SeekToTarget() == PASS)
            {
                result = StartBufferingFromTarget();
                if(result != PASS)
                {
                    result = TID_FAIL;
                }
            }
            else
            {
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    //* speed down */
                result = SEEK_FAIL;
            }
        break;

        case DVD_DISC:
            dvdInitializeHostBuffer(start_lba);
            psn = dvdConvertLBAtoPSN(start_lba);

            dvdDecFlag.fREAD_AHEAD = TRUE;// avoid read error recovery and monitor
            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            dvdDiscRequest.TargetID.Long = psn.PSN;
            dvdDiscRequest.Type = INIT_READ_AHEAD;
            dvdDiscRequest.TargetLayer = psn.Layer;
            dvdDiscRequest.EccBlockCount = (length / ONE_DVD_BLOCK) + 1;

            if(dvdDiscRequest.EccBlockCount > 0x3FF)// 0x3FF = MAX_VALUE_OF_BKTOBUF
                dvd_emain_buffer_count = dvdDiscRequest.EccBlockCount - 0x3FF;

            result = dvdStartDiscTransfer();
        break;
#if (BD_ENABLE == 1)
        case BD_DISC:
            bdInitializeHostBuffer(start_lba);
            psn = bdConvertLBAtoPSN(pdbGetBDClusterStart(start_lba));

            bdDecFlag.fREAD_AHEAD = TRUE;// avoid read error recovery and monitor
            bdDiscRequest.TargetID.Long = psn.PSN;
            bdDiscRequest.Type            = INIT_READ_AHEAD;
            bdDiscRequest.TargetLayer     = psn.Layer;
            bdDiscRequest.ClusterCount   = (length / ONE_BD_CLUSTER) + 1;

            result = bdStartDiscTransfer();
        break;
#endif
        default:
        break;
    }

    if(result == PASS && default_monitor == TRUE)
    {
        send_msg5S(1,"$monitor")
            
#if (SUN_FUNC_TEST == 1)            
        if (fBlerRead == TRUE)
        {            
            fStartBuffer = TRUE;
            if (pdbDiscLoaded() == CD_DISC)
            {                
                WRITE_REG(HAL_CDDREG_MAP->C12CCR, HAL_CDD_C1UNC);   // count any C1 error + C2 uncorrectable
            }
        }
#endif
        
        StartTimer(&stop_buffering_timer);
        backup_SectorCount = DataBuffer.SectorCount;
        while(DataBuffer.SectorCount < length)
        {
            if(DataBuffer.SectorCount == backup_SectorCount)
            {// no new data come in
                if(ReadTimer(&stop_buffering_timer) >= 300)
                {
                    result = GEN_FAIL;
                    break;
                }
            }
            else
            {// new data came in
                if(dvd_emain_buffer_count)// only for DVD
                {// Need extend READ!!!!!!!!!!
                    WRITE_REG( HAL_DVDD_BLKINC_REGISTER, 1);
                    dvd_emain_buffer_count--;
                }
                backup_SectorCount = DataBuffer.SectorCount;
                StartTimer(&stop_buffering_timer);
            }

            if(EjectKeyInfo.PressedValid)
                break;

            ExitProcess();
        }
        StopDecoder();
    }

#if (SUN_FUNC_TEST == 1)            
    fStartBuffer = FALSE;
#endif
    send_msg5SValue(1,"$DB.sLBA",4,DataBuffer.CacheStartLBA);
    send_msg5SValue(1,"$DB.eLBA",4,DataBuffer.CacheEndLBA);
    send_msg5SValue(1,"$DB.SC",4,DataBuffer.SectorCount);
    send_msg1('$','R','S',result);

    return result;
}
#if (SUN_FUNC_TEST == 1)
void FillEEPROMWithInitialValue(void)
{
    BYTE i;

    LinearPatternFill((USHORT)((ULONG)DRVPARA_DATA/0x800),0,2,0x800/2,0); //clear //20090304_q.22 == Return same value with original bin file for ReadBuffer command.

    SCSDRAM[DRVPARA_DATA + 0] = SCSDRAM[DRVPARA_DATA+AGAIN_SL_SIZE - 2] = 'L';
    SCSDRAM[DRVPARA_DATA + 1] = SCSDRAM[DRVPARA_DATA+AGAIN_SL_SIZE - 1] = 'T';

    //Section A, offset = 0x40, length = 0x20
    for(i = 0; i < 0x20; i++)
        SCSDRAM[DRVPARA_DATA + 0x40 + i] = Init_EEPROM_Table[i];
    //Section B, offset = 0x70, length = 0x10
    for(i = 0; i < 0x10; i++)
        SCSDRAM[DRVPARA_DATA + 0x70 + i] = Init_EEPROM_Table[0x20 + i];
    //Section C, offset = 0x1E0, length = 0x10
    for(i = 0; i < 0x10; i++)
        SCSDRAM[DRVPARA_DATA + 0x1E0 + i] = Init_EEPROM_Table[0x30 + i];
    //Section D, offset = 0x3E0, length = 0x20
    for(i = 0; i < 0x20; i++)
        SCSDRAM[DRVPARA_DATA + 0x3E0 + i] = Init_EEPROM_Table[0x40 + i];
    //Section E, offset = 0x400, length = 0x78
    for(i = 0; i < 0x78; i++)
        SCSDRAM[DRVPARA_DATA + 0x400 + i] = Init_EEPROM_Table[0x60 + i];
}
#endif
#if 0
void FillEEPROMWithInitialValue(void)
{
    BYTE i;

    LinearPatternFill((USHORT)((ULONG)DRVPARA_DATA/0x800),0,2,0x800/2,0); //clear  Return same value with original bin file for ReadBuffer command.

    SCSDRAM[DRVPARA_DATA + 0] = SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 2] = 'S';
    SCSDRAM[DRVPARA_DATA + 1] = SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 1] = 'N';

}

#endif //pcb == 710
