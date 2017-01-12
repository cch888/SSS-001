/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
*
* FILENAME:   nx_audio.c
*
* DESCRIPTION
* This file contains buffer audio data in NX and plays back at 1x
*
* $Revision: 3 $
* $Date: 09/07/07 5:58p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/* Audio Cache Point */
#define AUDBUF_POINT 0x100

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\hif\gp_mac.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_ram.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_equ.h"
#include ".\hif\nx_audio.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\gp_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_int.h"
#include ".\hif\rd_int.h"
#include ".\hif\mmc.h"
#include ".\hif\sv_util.h"

#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#if (AUDIO_PLAYBACK == 1)
#include ".\hif\gp_audio.h"
#endif
#include ".\hif\app_if.h"
#if (EN_POWER_CONTROL == 1)
#include ".\hif\powerctl.h"
#endif
#if (AUDIO_PLAYBACK == 1)
static vBOOL SeekToTargetAndReadSomeDataIntoBuffer(void);
static void AbortTransfer(void);
static void MonitorAudioScan(void);
#endif

extern void MuteOn(void);
extern void MuteOff(void);

#if (USE_TWO_KEY_FUNCTION==1)
extern void KeyPlayAudio(BYTE flg);
#endif /* USE_TWO_KEY_FUNCTION */

extern BYTE    SOTCstopSubQ[10];

#if (AUDIO_PLAYBACK == 1)
/**********************************************************************
* EnterNxTo1x
*
* Entry point of Nx audio
*
* Returns:  void
* Params:   none
**********************************************************************/
BYTE    nowDecode_Tno, oldDecode_Tno;
extern BYTE pre_tno; /* Check tno protect */

void EnterNxTo1x(void)
{
    vBOOL temp_IE,flg;
    long tmpLBA = 0;

    #if (EN_POWER_CONTROL == 1)
        if (CurrentPowerMode == eStandbyPower)
        {
            SetDiscReadPowerMode();
        }
        PowerControl(ePlayAudioPower);
    #endif
    /* Avoid the noise, when the drive is under SCAN mode and accept the Stop Play/Scan command. */
    if( Iram_Cmd_block.bf[0] != 0x4E)
        {
    // Disable Nx audio, Aux = 512
    WRITE_FIELD(HAL_AUD_APLAYEN, 0);
    WRITE_FIELD(HAL_AUD_A1KAUX, 0);

    WRITE_FIELD(HAL_AUD_ABLKIEN, 0);    /* Disable Audio Block Transfer End Interrupt */
    CLEAR_INT_FIELD(HAL_AUD_ABLKINT, 0);      /* Clear Audio Block Transfer End Status bit */
        }
#if (USE_TWO_KEY_FUNCTION==1)
        gHw_play_no = HW_PLAY_STOP_OFF;
#endif

    /* save host request LBA */
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    HostXfer.StartBlock.lba = MSF2LBA(&Play_Cmd_block.bf[3]);
    Buf_scan_lba = HostXfer.StartBlock.lba;
    SET_INT_ENABLE_MASK(temp_IE);

    /* calculate the xfer length */
    tmpLBA  = (ULONG)Play_Cmd_block.bf[6] << 16;
    tmpLBA += (ULONG)Play_Cmd_block.bf[7] << 8;
    tmpLBA += (ULONG)Play_Cmd_block.bf[8] ;
    LBA_SUB(tmpLBA, HostXfer.StartBlock.lba, Iram_Audio_blk2_play, flg);
    if (flg)
    {   /* start lba greater then end */
        Svr_pause_reqire = TRUE;        /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
        {
        BUILD_SENSE(0x05,0x21,0x00,0x47);    /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        }
        return;
    }

    Iram_Audbuf_blkcnt = 0;
    Rw_cdda_disc  = 1;
    Rw_seek_blk.lba = HostXfer.StartBlock.lba;          /* initialize Seek blk to HostXfer.StartBlock.lba*/
    MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba, 0x1F);    /* seek to 1F blocks ahead */

    StopCDDADecoder();
    SetAudioChannelMode(0x00);         /* call player to set default output mode */
    ClearDataBuffer();
    Rw_subc_data=TRUE;
    svoIramVar.subQEnableFlag = 1;

    if ( !SeekToTargetAndReadSomeDataIntoBuffer() )           // Seek to specified location and read a few blocks of data.
    {   // Seek was successful, a number of audio blocks have been read into the Buffer.
        if ( (Iram_Rw_SOTC_st == TRUE) && (!Rw_dec_stop) ) {
            oldDecode_Tno = BCD2HEX( pdbGetSubQTno() );
        }
        St_nx_audio = TRUE;
        PlayAudioNx1x();             /* Actual Play Audio - Turn on all DAC features*/
        return;
    }

    // Seek/ Buffering Error, Return
    Iram_audio_scan_st = FALSE;
    svoIramVar.subQEnableFlag = 0;
    St_nx_audio = FALSE;
    pre_tno = 0xff; /* Check tno protect */

    St_audio_extract = FALSE;
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    AutoCommandComplete();
}

//  *********************************************************************
//  PlayAudioNx1x
//
//  This routine is used to set the registers for playing
//  audio with data from the buffer
//  Also sets the volume, channel & output format mode.
//    Function is entered after seek has been completed,
//     Target address has been reached
//       and 20 blocks of data has been read into NX Buffer
//
//  Returns:  void
//  Params:   none
//  ********************************************************************/
void PlayAudioNx1x(void)
{
    vBOOL temp_IE;

    // Seek is complete, target address has been reached and there are
    //  already 20 blocks of audio in the buffer ready to be routed to the Audio DAC

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    // Disable Nx audio, Aux = 512
    WRITE_FIELD(HAL_AUD_APLAYEN, 0);
    WRITE_FIELD(HAL_AUD_A1KAUX, 0);
                                            // Do Not Reset NX-to_1X Module
    WRITE_FIELD(HAL_AUD_ABLKIEN, 0);    /* Disable Audio Block Transfer End Interrupt */
    CLEAR_INT_FIELD(HAL_AUD_ABLKINT, 0);      /* Clear Audio Block Transfer End Status bit */
    WRITE_FIELD(HAL_AUD_ABUFPG, 0);             // Initial audio block counter to first data block area in buffer memory.

    WRITE_FIELD(HAL_AUD_APLAYEN, 1);                // Enable Audio DAC and digital Audio Output Logic.Automatic change from 2.5K to 3k page for C2PO
    WRITE_FIELD(HAL_AUD_A1KAUX, halAUD_AAUXSize_512B);

    WRITE_FIELD(HAL_AUD_ABLKIEN, 1);              // Enable Audio Block Transfer End Interrupt

/* For CD-G, use DataBuffer.SectorCount to record the counter of R-W subcode */
    DataBuffer.SectorCount = 0;                            // init counter to 0
    DataBuffer.CurHostPtr = 0;                          // init host xfr pointer

    WRITE_FIELD(HAL_AUD_DACMUTE, 1); //Mute Audio
    DelaymS(1);
    WRITE_REG(HAL_AUDREG_MAP->AMUTE , 0x00);                  // Take off all "mute" bits and enable Digital Attenuator.
    WRITE_REG(HAL_AUDREG_MAP->AROUTE, 0x00);                  //   Enable Audio left and right channel. (Just like function  SetAudioChannelMode(0x03) below)
                                                    //     Shouldn't this setting be taken from Player Data Base ???
    Channel_ctrl_mode = 0x03;                       // Turn on L & R in shadow variable */

#if (USE_TWO_KEY_FUNCTION==1)
    opAMute = 1;
#endif

    SET_INT_ENABLE_MASK(temp_IE);
    //Customer request LED rule
    #if PA0_WITH_LED
    ReadyLED(LED_FLASH120mS);
    #else
    ReadyLED(LED_PLAY_FLASH);
    #endif

    RampUpVolume();                    // Take off "Mute" and ramp up Audio to default volume.

#if (USE_TWO_KEY_FUNCTION==1)
        if (gSt_play_sw)
                gHw_play_no = HW_PLAY_STOP_ON;
        else
#endif

    AutoCommandComplete();
}

/**********************************************************************
* MonitorAudioNx
*
* Monitor the audio during play back from buffer.
*     1. Need to seek and turn on buffering when the
*        buffered blocks < ceratin amount (this amount need to be adjusted)
*        we set this certain amount is 30
*
* Returns:  void
* Params:   none
**********************************************************************/
BYTE    Lout_st;
void MonitorAudioNx(void)
{
TimeS start_time,stop_time;
ULONG lba;
BYTE sing, Cur_Tno;
BYTE    q;

    if (((Iram_Svr_player_mode == PLAY_TRACK_MODE) || (Iram_Svr_player_mode == PAUSING_MODE))
        && (Iram_Audio_blk2_play != 0)){
        if (!Rw_dec_stop)
        {

            WRITE_FIELD(HAL_AUD_APLAYEN, 1);  /* Enable digital Audio Output Logic.Automatic change from 2.5K to 3k page for C2PO*/

            if(BCD2HEX(pdbGetSubQTno()) < 100 ){
                Cur_Tno = BCD2HEX(pdbGetSubQTno());
            }
            if(pdbGetSubQTno() == 0xAA){
                Svr_pause_reqire = TRUE;
                StopDecoder();
                Lout_st=TRUE;
            }else{
                Lout_st=FALSE;
            }
            nowDecode_Tno = BCD2HEX( pdbGetSubQTno() );
            /* If Iram_Rw_SOTC_st is TRUE, Play-Audio will stop playing next track. */
            if ( Iram_Rw_SOTC_st == TRUE ) {
                if ( (oldDecode_Tno != nowDecode_Tno) && (oldDecode_Tno != 0) ) {
                    Iram_Audio_blk2_play = Iram_Audbuf_blkcnt;
                    StopDecoder();
                    Iram_Knl_timer_counter = 2;                     /* Wait about 40 ms */
                    Knl_timeout = FALSE;                            /* Assume no time out */
                    while ( Knl_timeout == FALSE )
                    {
                        ExitProcess();
                    }
                    Svr_pause_reqire = TRUE;                        /* pause required */

                    RequestSubQInfo();
                    SOTCstopSubQ[0] = Iram_Player_output[0];
                    for ( q = 1; q < 10; q++ )
                    {
                        SOTCstopSubQ[q] = BCD2HEX( Iram_Player_output[q]);
                    }
                }
            }
            oldDecode_Tno = nowDecode_Tno;
        }else{
            /* for add buffering  */
            if ((Iram_Audbuf_blkcnt < AUDBUF_POINT) && (Iram_Audio_blk2_play > Iram_Audbuf_blkcnt))
            {
                if(Lout_st){
                    stop_time = pdbGetStopTime(Cur_Tno);
                    ++ Cur_Tno;
                    if ((Cur_Tno > pdbGetHighestRecordedTrkNumber())
                        || ((pdbGetTrackContAddr(Cur_Tno) & 0xF0) == 0x40)){
                        Iram_Audio_blk2_play = Iram_Audbuf_blkcnt;
                        MonitorAudioScan();
                        return;
                    }
                    start_time = pdbGetStartTime(Cur_Tno);
                    sing = mathSubtractMSFValues(&start_time, &stop_time, &stop_time);
                    if(sing == ePlus)
                        lba  = MSF2Count(&stop_time);
                    else
                        lba  = MSF2ADR(&stop_time);

                    if(Iram_Audio_blk2_play > lba)
                    {
                        Iram_Audio_blk2_play = Iram_Audio_blk2_play - lba;
                        DataBuffer.CacheEndLBA  = MSF2LBA(&start_time);
                    }else{
                        Iram_Audio_blk2_play = Iram_Audbuf_blkcnt;
                        MonitorAudioScan();
                        return;
                    }
                }
                /* Seek to target location */
                Rw_cdda_disc  = 1;
                HostXfer.StartBlock.lba = DataBuffer.CacheEndLBA;
                /* Point start block to the next block to be buffered */
                ADD_LBA_HEADER_1_FRAME(HostXfer.StartBlock.lba, 1);
                Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba, 0x1F);
                SeekToTargetAndReadSomeDataIntoBuffer();      // Seek to specified location and read a few blocks of data.
            }
        }
    }
    /* for Audio Scan */
    MonitorAudioScan();

#if (USE_TWO_KEY_FUNCTION==1)
    if (gSt_play_sw)
    {
        KeyPlayAudio(1);
        gSt_play_sw = 0;
    }
#endif

}
/**********************************************************************
* SeekToTargetAndReadSomeDataIntoBuffer
*
* Seeks to the specified location & turns on buffering
* Returns:  void
* Params:   none
**********************************************************************/
/* fix the bug that Q code with 0 is returned */ // TBD? Was this bug fixed in Deneb?
BYTE    Zero_Qcode_cnt = 0;
SubQCodeS tracking_subq;

static vBOOL SeekToTargetAndReadSomeDataIntoBuffer(void)
{
    BYTE a, dec_int_retry, rw_seek_retry;
    vBOOL temp_IE;
    TimeS msf;
    USHORT bptr;
    SubQCodeS  SubCode;
    long int  Qcode_PTime,Diff_Time;
    TimeS  ATIP;
    BYTE  Diff_Time_Retry;
    Diff_Time_Retry = 0;
    DisableCdAudioBuffering();     // Disable audio buffering during Seek
    dec_int_retry = 0;
    rw_seek_retry = 0;

nx_start_seek:

    while (1)
    {
        rw_seek_retry++;

        if ( SeekToTarget() == PASS)     /* seek ready */
            break;

        if ((St_nx_audio == TRUE) && (rw_seek_retry > 1)){
            if (Iram_Audio_blk2_play >= 32){
                HostXfer.StartBlock.lba += 32;
                Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                Iram_Audio_blk2_play -= 32;
            }else{
                Iram_Audio_blk2_play = Iram_Audbuf_blkcnt;

                WRITE_FIELD(HAL_AUD_APLAYEN, 1);  /* Enable digital Audio Output Logic.Automatic change from 2.5K to 3k page for C2PO*/

                return 1;
            }
        }

        /* seek fail, increment retry counter by 1
           if not exceed retry limit, reseek again
           if exceed retry limit, abort this command */
        if (!svoIramVar.knlTimeupCount)
        {
            AudioTransferDone();              /* Seek error , return */
            ClearDataBuffer();
            Svr_pause_reqire = TRUE;        /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x03,0x02,0x00,0x21);    /* MEDIUM ERROR, NO SEEK COMPLETE */
            return 1;
        }
    }

// ****************************************************************************
//
//   Seek successfully completed */
//
// ****************************************************************************
// The next step is to read a few blocks into the buffer

    EnableCdAudioBuffering();      //  Make sure data read from the buffer does get into the Buffer
    Iram_Svr_player_mode = PLAY_TRACK_MODE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    ResetCDDecoder();

    bptr = DataBuffer.CurDiscPtr;

    if (bptr == 0)
        bptr = RING_END_PAGE;
    else
        bptr --;

    WRITE_FIELD(HAL_CDD_DECPG, bptr);               /* Init decoder buffer page pointer */
    WRITE_FIELD(HAL_CDD_DSUBPG, bptr);              /* Init Subcode page pointer */
    WRITE_FIELD(HAL_HST_HSTBUFPG, bptr);            /* Init Host buffer page pointer */
    WRITE_FIELD(HAL_HST_BUFPGCMP, bptr);

    Period_pg = 0;
    WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);

    WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RDMISSEN | HAL_CDD_BFULLEN) );

    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, (HAL_CDD_HDRERREN | HAL_CDD_DIFERREN) );

    WRITE_REG(HAL_CDDREG_MAP->DINTEN2, (HAL_CDD_TFNDEN | HAL_CDD_DPRDEN) );

    WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_DAPLAY | HAL_CDD_SDIFERR) );

    msf = LBA2MSF(HostXfer.StartBlock.lba);

    a = HEX2BCD(msf.Minute);
    WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);       /* set target address MIN */

    a = HEX2BCD(msf.Second);
    WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);       /* set target address SEC */

    a = HEX2BCD(msf.Frame);
    WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);       /* set target address BLK */

    WRITE_REG(HAL_CDDREG_MAP->C2THLD, C2_THRESHOLD_CDDA3);       /* write C2 threshold register */

    fRStart = FALSE;
    fRdMiss = FALSE;

    Knl_deci_set = FALSE;
    Rw_dec_stop = FALSE;
    St_audio_extract = TRUE;
    Rw_tblk_reach = FALSE;
    Iram_AddBufferingFlg      = TRUE;

    WRITE_FIELD(HAL_CDD_BUFSBC, 1);
    WRITE_FIELD (HAL_CDD_AEWAEN, 1);        /* Enable auto page offset load */
    WRITE_FIELD(HAL_CDD_SCEN, 1);           /* Enable SUBC */

    /* 3 symbol C2 correction */
    EccCtrlMode(eAudioCD);

    WRITE_FIELD(HAL_CDD_IHDRW, 0);
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON | HAL_CDD_SCDDA) );

    if(DecTimeCnt < SPECIAL_SPD_MAX_TIME ){
        DecTimeCnt++;
    }

    Iram_Knl_timer_counter = 10;                /* Wait about 200 ms */  /* Work Around for bug that Q code with 0 is returned */
    Knl_timeout = FALSE;                        /* Assume no time out */
    SET_INT_ENABLE_MASK(temp_IE);

    /* fix the bug that Q code with 0 is returned */
    pdbSetSubQIntStatus(FALSE);
    pdbSetSubCodeOK(FALSE);
    Zero_Qcode_cnt = 0;

    /* wait for Read start interrupt*/
    while (1)
    {
        if (fRStart == TRUE)
            break;

/* Work Around for bug that Q code with 0 is returned */
        if((pdbGetSubQIntStatus() == TRUE) && (pdbGetSubcodeOK() == TRUE))
        {
            pdbSetSubQIntStatus(FALSE);
            pdbSetSubCodeOK(FALSE);

            pdbGetSubCode(&tracking_subq);
            if ((tracking_subq.PMinute == 0) && (tracking_subq.PSecond == 0) && (tracking_subq.PFrame == 0))
            {
                Zero_Qcode_cnt++;
                if(Zero_Qcode_cnt > 3)
                {
                    Zero_Qcode_cnt = 0;
                    WRITE_FIELD(HAL_CDD_DPCLR, 1);          /* Set Decoder partial clear */
                    WRITE_FIELD(HAL_CDD_DPCLR, 0);          /* Clear Decoder partial clear */
                }
            }
        }

        if (Knl_timeout)
        {
            if(dec_int_retry==5)
            {
                Knl_timeout = FALSE;
                Svr_pause_reqire = TRUE;

    #if (USE_TWO_KEY_FUNCTION==1)
                if (!gSt_play_sw)
    #endif
                BUILD_SENSE(0x05,0x24,0x00,0x47);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
                AbortTransfer();
                return 1;
            }
            else
            {
                dec_int_retry++;
                rw_seek_retry = 0;
                goto nx_start_seek;
            }
        }

        if (fRdMiss == TRUE)
        {
            /* Overshoot error */
            fRdMiss = FALSE;

            if (dec_int_retry == 5)
            {
                if (St_nx_audio == TRUE){
                    if (Iram_Audio_blk2_play >= 32){
                        HostXfer.StartBlock.lba += 32;
                        Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                        Iram_Audio_blk2_play -= 32;

                        dec_int_retry = 0;
                        rw_seek_retry = 0;
                        /* for ATIP Time and Qcode PTime different */
               Diff_Time_Retry++;
               if(Diff_Time_Retry == 2)
                Iram_Diff_Time_Flag = TRUE;
                        goto nx_start_seek;
                    }else{
                        Iram_Audio_blk2_play = Iram_Audbuf_blkcnt;
                        WRITE_FIELD(HAL_AUD_APLAYEN, 1);  /* Enable digital Audio Output Logic.Automatic change from 2.5K to 3k page for C2PO*/
                        return 1;
                    }
                }
                /* cdda_int_err: */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
                BUILD_SENSE(0x05,0x24,0x00,0x48);    /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
                AbortTransfer();
                return 1;
            }

            dec_int_retry++;
            rw_seek_retry = 0;
            /*for ATIP Time and Qcode PTime different*/
         if( Iram_Diff_Time_Flag == TRUE ){
               pdbGetSubCode(&SubCode);
              ATIP.Minute = SubCode.PMinute;
              ATIP.Second = SubCode.PSecond;
              ATIP.Frame  = SubCode.PFrame ;
              ATIP = BCD2MSF(&ATIP);
           Qcode_PTime = (ATIP.Minute*4500) + (ATIP.Second*75) + ATIP.Frame -150;
           Diff_Time = Qcode_PTime - Rw_seek_blk.lba;
           Rw_seek_blk.lba -= (Diff_Time + 16);
           if(Rw_seek_blk.lba<0)
            Rw_seek_blk.lba = 0;
           }else{
            Rw_seek_blk.lba -= 9;
            if(Rw_seek_blk.lba<0)
                Rw_seek_blk.lba = 0;
           }
           //Rw_seek_blk.lba -= 9;
        /*for ATIP Time and Qcode PTime different*/

            goto nx_start_seek;
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            dec_int_retry++;
            rw_seek_retry = 0;
            goto nx_start_seek;
        }

        if(EjectKeyInfo.PressedValid)
        {
            /* eject key is pushed, terminate this command and exit to
                process eject */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x02,0x3A,0x00,0x10);        /* NOT READY, MEDIUM NOT PRESENT */
            AbortTransfer();                   /* abort data xfr */
            return 1;
        }

        ExitProcess();
    } /* end while wait for 1st subcode */

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    fRStart = FALSE;
    Knl_deci_set = FALSE;

    Iram_Knl_timer_counter = 5;                 /* Wait about 100 ms */
    Knl_timeout = FALSE;                        /* Assume no time out */

    DataBuffer.CacheEndLBA = HostXfer.StartBlock.lba -1;        /* Update teh end of cache */

    SET_INT_ENABLE_MASK(temp_IE);

    while (1)                                   /* wait for next decoder int */
    {
        if (Knl_deci_set == TRUE)
            break;

        if ( Knl_timeout )
        {
            /* We are wait about 140ms and cd-da dec int never occurs.
               Set up error code and terminate this command now. */
            Knl_timeout = FALSE;
            Svr_pause_reqire = TRUE;            /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x05,0x24,0x00,0x49);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
            AbortTransfer();
            return 1;
        }

        if(EjectKeyInfo.PressedValid)
        {
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x02,0x3A,0x00,0x11);        /* NOT READY, MEDIUM NOT PRESENT */
            AbortTransfer();
            return 1;
        }

        ExitProcess();
    }

// Ttarget MSF reached: first block has been successfully read into buffer
//  Wait for 20blks in buffer so that nx to 1x has some blocks to output

    Knl_deci_set = FALSE;
    Iram_Knl_timer_counter = 5;                 /* Wait about 100 ms */
    Knl_timeout = FALSE;                        /* Assume no time out */

    while (1)
    {
        if (Knl_deci_set == TRUE)
        {
            Knl_deci_set = FALSE;
            Iram_Knl_timer_counter = 5;         /* Wait about 100 ms */
            Knl_timeout = FALSE;                /* Assume no time out */
        }

        if ( Knl_timeout )
        {
            /* Timeout Error */
            Knl_timeout = FALSE;
            Svr_pause_reqire = TRUE;            /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x05,0x24,0x00,0x4a);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
            AbortTransfer();
            return 1;
        }

        if(EjectKeyInfo.PressedValid)
        {
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            BUILD_SENSE(0x02,0x3A,0x00,0x12);        /* NOT READY, MEDIUM NOT PRESENT */
            AbortTransfer();
            return 1;
        }

        if ((Iram_Audbuf_blkcnt >= 20) || (Iram_Audbuf_blkcnt > Iram_Audio_blk2_play))
            // Finished reading 20 blks into the buffer, go to start play now
            break;

        ExitProcess();
    }/* Buffered 20 blks into the buffer, go to start play now */

    Knl_deci_set = FALSE;
    return 0;
}

/********************************************************************
* AbortTransfer();
*
* Aborts transfer
*
*********************************************************************/
static void AbortTransfer(void)
{
    StopDecoder();
    AbortHostTransfer();

    /* status not post yet */
    St_send_blkdata = FALSE;            /* reset send blk data */
    DataBuffer.SectorCount = 0;
    St_cmd_inproc = FALSE;
    HostXfer.BlocksLeft = 0;

    /* Entry point for exit ReadAudioSubCode command */
    Rw_subc_data = FALSE;               /* mark read subcode data */
    Rw_rd_monitor = FALSE;              /* unmark read monitor */
    ENABLE_INTERRUPT();
}

/**********************************************************************
* AudioIntProc
* Entry point for Audio interrupt service routine
* Audio interrupt occurs whenever 1 audio blk is played out to the dac
*
* Returns:  void
* Params:   none
**********************************************************************/
extern void MuteOn(void);
extern void MuteOff(void);
extern USHORT   Iram_Aud_xfer_subq_ptr;

void AudioIntProc(void)
{
    BYTE CtrlAddr;
    ULONG work_addr;
    BYTE curPlaySubQ[3];

    /* Transfer done for
    sending 1 block of audio data via the serial port, clear the int */
    //For Green function Item 3 -> Standby timer set to 10 mins
    //when playback mode detected by SmartX
    if (InPlaybackMode == TRUE)
        Iram_Knl_save_counter  = 6000;
    else
        Iram_Knl_save_counter  =STANDBYCNT;
    CLEAR_INT_FIELD(HAL_AUD_ABLKINT, 0);      /* Clear audio xfer interrupt */
    St_audten_int = 1;                      /* indicate audio xfer int. */

    /* Now Playing page number save for WriteSubQData() */

    Iram_Aud_xfer_subq_ptr = READ_FIELD(HAL_AUD_ACURPG);
    work_addr = PAGE_AUX_START_ADDR(Iram_Aud_xfer_subq_ptr);

    CtrlAddr = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR];
    /* Get current audio playback absolute position */

    curPlaySubQ[0] = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 7]);
    curPlaySubQ[1] = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 8]);
    curPlaySubQ[2] = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 9]);

    if ( (CtrlAddr & 0x0F) == 0x01 )        /* Check SubQ data mode */
    {
        /* Set current audio playback absolute position */
        CurPlayTime.Minute = curPlaySubQ[0];
        CurPlayTime.Second = curPlaySubQ[1];
        CurPlayTime.Frame  = curPlaySubQ[2];
    }

    if (CtrlAddr & 0x10)
    {
            WRITE_FIELD(HAL_AUD_EMPH, 1);   /* emphasis: Enable */
    }
    else
    {
            WRITE_FIELD(HAL_AUD_EMPH, 0);   /* emphasis: Disable */
    }

    if ((CtrlAddr & 0xF0) < 0x40 )
    {
        MuteOff();                         /* Audio Disc */
    }
    else
    {
        MuteOn();                          /* Data Disc */
    }

    Iram_Audbuf_blkcnt--;
    if ( Iram_Audbuf_blkcnt == 0xffff)
    {
        Iram_Audbuf_blkcnt = 0;                 /* Initialise buf_blkcnt*/
        WRITE_FIELD(HAL_AUD_APLAYEN, 0);    /* Disable digital Audio Output Logic */
    }

    //Verify CDG function
    /* check if blks remaining to be played is equal to Zero:
       If equal to zero, it means cmd completed
       If not equal to zero, it needs to check buffer available */

    if (Iram_audio_scan_st == TRUE){
        return;
    }

    if (Iram_Audio_blk2_play < 2)
    {
        AudioTransferDone();                 // We are almost finished playing audio
        Iram_Audio_blk2_play = 0;            //   There is only on block left to play.
                                             //    For a smooth ending, ramp down the volume.
    }

    if (Iram_Audio_blk2_play != 0)
    {                                   // Command not finished yet.
        Iram_Audio_blk2_play--;         //  update remaining blks to play.
        return;
    }

    /* audio_xfer_complete*/
    St_audio_complete = 1;
    St_cmd_inproc = 0;
    St_nx_audio = FALSE;
    pre_tno = 0xff; /* Check tno protect */
    AudioTransferDone();
    ClearDataBuffer();
    Svr_pause_reqire = TRUE;        /* pause required */

    StopDecoder();
}

/**********************************************************************
* MonitorAudioScan()
*
* Audio Scan command  position is represented
*
* Returns:  none
* Params:   none
**********************************************************************/
#define SCAN_PLAY   16
#define SCAN_JUMP   150

ULONG play_lba;
static void MonitorAudioScan(void)
{
    ULONG temp_lba = 0;
    TimeS last_time;
    BYTE last_tno;

    play_lba = DataBuffer.CacheEndLBA - Buf_scan_lba;

    if((Iram_audio_scan_st == TRUE) && (play_lba > SCAN_PLAY))
    {
        if(!gAudioScanDirection){  /* 0:foward 1:reversed */
            Buf_scan_lba += SCAN_JUMP;
            last_tno = pdbGetHighestRecordedTrkNumber();
            last_time = pdbGetStopTime(last_tno);
            temp_lba  = MSF2LBA_Plus(&last_time);

            if ((Buf_scan_lba + SCAN_PLAY) > temp_lba){
                Iram_audio_scan_st = FALSE;         /* Audio Scan end */
                Iram_Audio_blk2_play    = Iram_Audbuf_blkcnt;
                return;
            }
        }else{
            if (Buf_scan_lba < SCAN_JUMP){
                Iram_audio_scan_st = FALSE;         /* Audio Scan end */
                Iram_Audio_blk2_play    = Iram_Audbuf_blkcnt;
                return;
            }
            Buf_scan_lba -= SCAN_JUMP;
            last_tno = 1;
            last_time = pdbGetStartTime(last_tno);
            temp_lba  = MSF2LBA_Plus(&last_time);
            if (Buf_scan_lba < temp_lba){
                Iram_audio_scan_st = FALSE;         /* Audio Scan end */
                Iram_Audio_blk2_play    = Iram_Audbuf_blkcnt;
                return;
            }

        }
        HostXfer.StartBlock.lba = Buf_scan_lba;

        /* Seek to target location */
        Rw_cdda_disc  = 1;
        /* Point start block to the next block to be buffered */
        ADD_LBA_HEADER_1_FRAME(HostXfer.StartBlock.lba, 1);
        Rw_seek_blk.lba = HostXfer.StartBlock.lba;
        MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba, 0x1F);
        SeekToTargetAndReadSomeDataIntoBuffer();
    }
}

#endif // AUDIO_PLAYBACK
