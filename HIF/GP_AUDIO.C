/******************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_audio.c
*
* DESCRIPTION:
*
*   $Revision: 28 $
*   $Date: 11/02/25 5:52p $
*
*   This file process ATAPI Audio commands
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\hif\gp_rdcd.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#if (AUDIO_PLAYBACK == 1)
#include ".\hif\gp_audio.h"
#endif
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\rd_util.h"
#if (AUDIO_PLAYBACK == 1)
#include ".\hif\nx_audio.h"
#endif
#include ".\oem\oem_spd_tbl.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_equ.h"

#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#if AUDIO_PLAYBACK == 1
static void PlayAudioCommon(void);
static vBOOL ReplacePositionByCurrentMSF(void);
static void CheckStartLimitation(void);
static void MultiSessionCalculation(void);

#if (USE_TWO_KEY_FUNCTION==1)
void KeyPlayAudio(BYTE flg);
#endif

/*************** Local Variables Start ******************************/
private LeftRightChannelVolumeS fLeftRightChannelvolume;
private AudioSourceT            fAudioSource;
/*************** Local Variables End ********************************/
#endif // AUDIO_PLAYBACK


/*********************************************************************
* Cmd_play_audio 10/12
*
* Play Audio 10/12 BYTE command, Start position is represented
* as LBA in CDB
*
* Returns:      void
* Params:       none
*********************************************************************/
void PlayAudio10Command(void)
{
#if (AUDIO_PLAYBACK == 0)
    BUILD_SENSE(0x05,0x21,0x00,0x3c);        /* ILLEGAL REQUEST */
    AutoCommandComplete();                     /* cmd complete */
    return;
#else
    BYTE * DramAddressP;
    BYTE    trackNo;
    TimeS   playLimitMSF, MSF;
    LONG    playLimitLBA;
    ULONG   start_lba = 0, req_len =0;
    vBOOL   temp_IE;

    /* check PLAY AUDIO 10 or 12 command: */
    if ( Iram_Cmd_block.bf[0] == MMC_PLAY_AUDIO_12 ) {  /* check if play audio 12 command */
        if ( Iram_Cmd_block.bf[6] != 0 ) {
            BUILD_SENSE(0x05,0x21,0x00,0xf1);        /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();                         /* cmd complete */
            return;
        }
        req_len = PacketCommandToLBALEN3( 7 );      /* audio 12 */
    } else {
        req_len = PacketCommandToLBALEN2( 7 );      /* audio 10 */
    }

    /* If request length is NOT zero, this routine checks drive's current status instead of CheckCommandAvail(). */
    if ( req_len == 0 )
    {
        AutoCommandComplete();
        return;
    }
    else
    {
        if ( St_become_ready == TRUE ) {
            BUILD_SENSE(0x02,0x04,0x01,0x80);        /* LOGICAL UNIT IS IN PROCESS OF BECOMING READY */
            AutoCommandComplete();
            return;
        } else if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) ) {
            BUILD_SENSE(0x02,0x3A,0x00,0x81);        /* MEDIUM NO PRESENT */
            AutoCommandComplete();
            return;
        }
    }

    if (St_blank_disc)                              /* if blank disc */
    {
        BUILD_SENSE(0x05,0x21,0x00,0x31);

        AutoCommandComplete();                             /* cmd complete */
        return;
    }

    St_zero_length = FALSE;                         /* assume not zero length request */

    /* function parameters: address of starting lba, and 4 blocks testing */
    if (CheckForFF((BYTE *)&Iram_Cmd_block.bf[2], 4) )   /* check special case: if 4 ff */
    {
        Iram_Cmd_block.bf[2] = 0xFF;                /* set mark for start from current */

        /* use current position as starting play address */
        if ( ReplacePositionByCurrentMSF() )            /* if current address is illegal */
        {
            BUILD_SENSE(0x05,0x21,0x00,0x32);            /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();
            return;
        }

        start_lba  = MSF2LBA(&Iram_Cmd_block.bf[3]);
    }
    else
    {
        start_lba = PacketCommandToLBALEN3(3);

        if ( (pdbServoGetDiscType() == eATIPDisc) && (pdbGetFinalSessCloseInfo() == eNotSessionClosed) )
        {   /* case of open session */
            trackNo = pdbGetHighestRecordedTrkNumber();
            playLimitMSF = pdbGetStopTime( trackNo );
            playLimitLBA = MSF2LBA_Plus( &playLimitMSF );
            if ( (LONG)start_lba >= playLimitLBA )
            {                                   /* we are on a writable medium and nrecorded area */
                BUILD_SENSE(0x05,0x21,0x00,0x33);    /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();                 /* cmd complete */
                return;
            }
        }
        else
        {   /* case of close session */
            if ( (LONG)start_lba >= Maxlba )
            {                                   /* we are in Lead-Out area */
                BUILD_SENSE(0x05,0x64,0x00,0x34);    /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
                AutoCommandComplete();                 /* cmd complete */
                return;
            }
        }

        MSF = LBA2MSF(start_lba);
        Iram_Cmd_block.bf[3] = MSF.Minute;
        Iram_Cmd_block.bf[4] = MSF.Second;
        Iram_Cmd_block.bf[5] = MSF.Frame;
    }

    /* Check if zero transfer length -> do not play audio and just goto complete */
    if (req_len == 0)
    {       /* Request length is zero, treat it as a seek command but stay there */
        St_zero_length = TRUE;              /* set zero length flag */
        Play_Cmd_block = Iram_Cmd_block;
        PlayAudioCommon();
        return;
    }

    req_len += start_lba;
    Tg_endlba = req_len;

    req_len --;

    /* Set ending address */
    Iram_Cmd_block.bf[6]    = (BYTE)((req_len >> 16) & 0xff);
    Iram_Cmd_block.bf[7]    = (BYTE)((req_len >> 8) & 0xff);
    Iram_Cmd_block.bf[8]    = (BYTE)req_len & 0xff;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    if ( Iram_Cmd_block.bf[2] == 0xFF )
    {
        if ( Iram_Audio_blk2_play > 0L )
        {
            Iram_Audio_blk2_play = Tg_endlba - start_lba;

            /* save audio end block to DRAM. */
            /* no need to ena/disable interrupt */

            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + AUDIO_END_HI + AUDIO_END_MIN);   /* audio end blk (MIN) dram addr lo */
            *DramAddressP++ = (Iram_Cmd_block.bf[6]);      /* save audio end min */
            *DramAddressP++ = (Iram_Cmd_block.bf[7]);      /* .............. sec */
            *DramAddressP++ = (Iram_Cmd_block.bf[8]);      /* .............. frame */

        SET_INT_ENABLE_MASK( temp_IE );
            AutoCommandComplete();                     /* exit */
            return;
        }
    }

    SET_INT_ENABLE_MASK( temp_IE );
    Play_Cmd_block = Iram_Cmd_block;
    PlayAudioCommon();
    return;
#endif // AUDIO_PLAYBACK
}

/*********************************************************************
* PlayAudioMSFCommand
*
* Play Audio command handler
*
* Returns:      void
* Params:       none
*********************************************************************/
void PlayAudioMSFCommand()
{
#if (AUDIO_PLAYBACK == 0)
    BUILD_SENSE(0x05,0x21,0x00,0x3c);        /* ILLEGAL REQUEST */
    AutoCommandComplete();                     /* cmd complete */
    return;
#else

    BYTE * DramAddressP;
    BYTE    trackNo;
    TimeS   playLimitMSF;
    LONG    playLimitLBA;
    vBOOL temp_IE;
    ULONG end_addr, start_addr;

#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
        Play_Cmd_block = Iram_Cmd_block;

    /* common entry point for play audio format set as CDB345=starting MSF
       CDB678=ending MSF */
    if( (Play_Cmd_block.bf[3] == Play_Cmd_block.bf[6]) &&
        (Play_Cmd_block.bf[4] == Play_Cmd_block.bf[7]) &&
        (Play_Cmd_block.bf[5] == Play_Cmd_block.bf[8]))
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
        AutoCommandComplete();
        return;
    }

    if (St_blank_disc)                  /* if blank disc */
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
        {
        BUILD_SENSE(0x05,0x21,0x00,0x35);
        AutoCommandComplete();                 /* cmd complete */
    }
        return;
    }

    /* Check valid ending MSF */
    if ( CheckValidMSF((BYTE *)&Play_Cmd_block.bf[6]) )
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x24,0x00,0x7b);    /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
        AutoCommandComplete();
        }
        return;
    }

    if ((Play_Cmd_block.bf[6]== 0) && (Play_Cmd_block.bf[7] < 2 ))
    {
        Play_Cmd_block.bf[7]=2;
        Play_Cmd_block.bf[8]=0;
    }

    if(GetSpeedValue(Current_read_sp) > GetSpeedValue(PLAY_AUDIO_MSF_SPD))  //Only allow using lower speed
    {
        SetReadSpeed(PLAY_AUDIO_MSF_SPD);
    }
    SpeedCount = (CD_SPEED_UP_READY + CD_SPEED_DOWN_READY)/2;               //Prevent changing speed

    /* Get ending LBA */
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    end_addr = MSF2LBA(&Play_Cmd_block.bf[6]);
    Tg_endlba = end_addr;
    SET_INT_ENABLE_MASK(temp_IE);

    Play_Cmd_block.bf[6]    = (BYTE)((end_addr >> 16) & 0xff);
    Play_Cmd_block.bf[7]    = (BYTE)((end_addr >> 8) & 0xff);
    Play_Cmd_block.bf[8]    = (BYTE)end_addr & 0xff;

    //temp_IE = GET_INT_ENABLE_MASK();
    //DISABLE_INTERRUPT();

    /* check if starting MSF is set to 0FFH. if Yes, get the current position */
    if ( CheckForFF((BYTE *)&Play_Cmd_block.bf[3], 3) )
    {
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        if ( Iram_Audio_blk2_play > 0L )
        {
            start_addr = MSF2LBA(&CurPlayTime);

            // if end_addr < start_addr return illegale
            if ( end_addr < start_addr )
            {
                SET_INT_ENABLE_MASK(temp_IE);

#if (USE_TWO_KEY_FUNCTION==1)
                if (!gSt_play_sw)
#endif
                {
                    BUILD_SENSE(0x05,0x24,0x00,0x7c);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
                    AutoCommandComplete();
                }
                return;
            }

            Iram_Audio_blk2_play = end_addr - start_addr;

            /* save audio end block to DRAM. */

            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + AUDIO_END_HI + AUDIO_END_MIN);   /* audio end blk (MIN) dram addr lo */
            *DramAddressP++ = (Play_Cmd_block.bf[6]);                      /* save audio end min */
            *DramAddressP++ = (Play_Cmd_block.bf[7]);                      /* save audio end sec */
            *DramAddressP++ = (Play_Cmd_block.bf[8]);                      /* save audio end frame */

            SET_INT_ENABLE_MASK(temp_IE);

#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
            AutoCommandComplete();                                     /* exit */

            return;
        }
        else
        {
            SET_INT_ENABLE_MASK(temp_IE);
            /* use current position as starting play address */
            if ( ReplacePositionByCurrentMSF() )                /* if current address is illegal */
            {
#if (USE_TWO_KEY_FUNCTION==1)
            if (!gSt_play_sw)
#endif
        {
                BUILD_SENSE(0x05,0x21,0x00,0x36);                /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();                             /* cmd complete */
            }
                return;
            }
        }
    }

    //SET_INT_ENABLE_MASK(temp_IE);

        /* Check for valid MSF in starting address fields */
    if ( CheckValidMSF((BYTE *)&Play_Cmd_block.bf[3]) )
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x24,0x00,0x7d);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
        AutoCommandComplete();
        }
        return;
    }

    /* check if starting address is less than 0:2:0 then init it to 0:2:0 */
    CheckStartLimitation();

    /* Set start LBA */
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    start_addr = MSF2LBA(&Play_Cmd_block.bf[3]);
    SET_INT_ENABLE_MASK(temp_IE);

    if ( (pdbServoGetDiscType() == eATIPDisc) && (pdbGetFinalSessCloseInfo() == eNotSessionClosed) )
    {   /* case of open session */
        trackNo = pdbGetHighestRecordedTrkNumber();
        playLimitMSF = pdbGetStopTime( trackNo );
        playLimitLBA = MSF2LBA_Plus( &playLimitMSF );
        if ( (LONG)start_addr >= playLimitLBA )
        {           /* we are on a writable medium and were in unrecorded area */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
        {
            BUILD_SENSE(0x05,0x21,0x00,0x37);    /* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();                 /* cmd complete */
            }
            return;
        }
    }
    else
    {   /* case of close session */
        if ( (LONG)start_addr >= Maxlba )
        {           /* we are in Lead-Out area */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
        {
            BUILD_SENSE(0x05,0x64,0x00,0x38);    /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
            AutoCommandComplete();                 /* cmd complete */
            }
            return;
        }
    }

    if ( end_addr < start_addr )
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x24,0x00,0x7e);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
        AutoCommandComplete();                     /* cmd complete with check condition */
    }
        return;
    }

    /* Now, it's legal ending address, continue to check if  starting address
       is the same as ending address. */
    St_zero_length = FALSE;                 /* mark not zero len */

    if (start_addr == end_addr )            /* if req xfer length is zero */
        St_zero_length = TRUE;              /* set zero lenth flag */

    PlayAudioCommon();
#endif // AUDIO_PLAYBACK
}

#if (AUDIO_PLAYBACK == 1)
/*********************************************************************
* PlayAudioCommon
*
* Returns:      void
* Params:       none
*********************************************************************/
extern BYTE pre_tno;

static void PlayAudioCommon(void)
{
    BYTE * DramAddressP;
    long temp_lba;
    TimeS subcd;

    MultiSessionCalculation();
{
    long    start_lba, end_lba;

    start_lba = MSF2LBA(&Play_Cmd_block.bf[3]);
    end_lba   = (ULONG)Play_Cmd_block.bf[6] << 16;
    end_lba  += (ULONG)Play_Cmd_block.bf[7] << 8;
    end_lba  += (ULONG)Play_Cmd_block.bf[8] ;

    if ( end_lba < start_lba ) {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x24,0x00,0xeb);        /* ILLEGAL REQUEST, INVALID FIELD IN CDB */
        AutoCommandComplete();                         /* cmd complete with check condition */
    }
        return;
    }
}
    Rw_dec_stop = FALSE;
    St_audio_complete = FALSE;                  /* Clear audio operation complete flag*/

    St_method_2 = FALSE;
    Rw_seek_blk.lba = MSF2LBA(&Play_Cmd_block.bf[3]);

    while (1)
    {
        if ( SeekToTarget() == PASS)         /* seek ready */
        {
           #if (PCB == 710)||(PCB == 711)||(PCB == 817)
           do{   //Drive receive PlayAudioCommand after Spin Down Command. Servo need to do Spin Up.
           	ExitProcess();
	    }while ((pdbGetPlayerStatus()==eStTrayIn) || (pdbGetPlayerStatus()==eStStartingUp));
           #endif
           break;
        }
        if (!svoIramVar.knlTimeupCount)
        {
            Svr_pause_reqire = TRUE;            /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
        {
            BUILD_SENSE(0x03,0x02,0x00,0x11);        /* MEDIUM ERROR, NO SEEK COMPLETE */
            AutoCommandComplete();
            }
            return;
        }
    }

    /* read current Q subcode to identify yellow book disc
       if it is not, then seek back to exact location and pause servo.
       if it is data disc, do seek and pre-fetch.  */

    while(1){
        if ( ServoReadSubcode(1,0xFF) == FAIL )
        {
#if (USE_TWO_KEY_FUNCTION==1)
        if (!gSt_play_sw)
#endif
        {
            BUILD_SENSE(0x05,0x64,0x00,0x39);        /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
            AutoCommandComplete();
            }
            return;
        }

        subcd.Minute = BCD2HEX(((BYTE *)PlayerAddrsPtr)[7]);
        subcd.Second = BCD2HEX(((BYTE *)PlayerAddrsPtr)[8]);
        subcd.Frame  = BCD2HEX(((BYTE *)PlayerAddrsPtr)[9]);
        temp_lba     = MSF2LBA_Plus(&subcd);

        if(temp_lba >= Target_Timelba){
            break;
        }
        ExitProcess();
    }

    svoIramVar.subQEnableFlag = 0;

    if ( ServoReadSubcode(1,0xFF) == FAIL )
    {
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x64,0x00,0x3a);        /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
        AutoCommandComplete();
    }
        return;
    }

    if ( TEST_BIT(((BYTE *)PlayerAddrsPtr)[0], 6) ||     /* if digital data */
        ((BYTE *)PlayerAddrsPtr)[1] == 0xAA )
    {
        /* post can not play on data track   */

        Svr_pause_reqire = TRUE; /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
    {
        BUILD_SENSE(0x05,0x64,0x00,0x3b);        /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
        AutoCommandComplete();                     /* exit */
    }
        return;
    }


    /* Check if Nx audio  */
    if (St_zero_length)
    {
        St_zero_length = FALSE;
        St_audio_complete = TRUE;           /* mark audio done */


        if (St_nx_audio == TRUE)
        {
            Iram_audio_scan_st = FALSE;
            St_nx_audio = FALSE;
            pre_tno = 0xff;
            AudioTransferDone();
            ClearDataBuffer();
            Knl_timeout = FALSE;
        }

        Svr_pause_reqire = TRUE;            /* mark pause request */
#if (USE_TWO_KEY_FUNCTION==1)
    if (!gSt_play_sw)
#endif
        AutoCommandComplete();                     /* exit */
        return;
    }

    else
    {
        St_nx_audio = 1;
        svoIramVar.subQEnableFlag = 1;

        DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + AUDIO_END_HI + AUDIO_END_MIN);   /* audio end blk (MIN) dram addr lo */
        *DramAddressP++ = (Play_Cmd_block.bf[6]);               /* save audio end min */
        *DramAddressP++ = (Play_Cmd_block.bf[7]);               /* save audio end sec */
        *DramAddressP++ = (Play_Cmd_block.bf[8]);               /* save audio end frame */

        EnterNxTo1x();
        return;
    }
}


/*********************************************************************
* ReplacePositionByCurrentMSF
*
* Returns:  0 - Pass 1- error
* Params:       none
* Globals:      (in) Prev_lba
*                       (out) Iram_Cmd_block[3..5]
*********************************************************************/
static vBOOL ReplacePositionByCurrentMSF(void)
{

    if ( ServoReadSubcode(1,0xFF) == FAIL )         /* read current subcode Q */
    {
        /* if read subcode failed, it may cause by the previously stop cmd,
        make a seek at previous location which is saved at rw_seek_blk, and
        read subcode again */

        Rw_seek_blk.lba = Iram_Player_output[7]*4500L + Iram_Player_output[8]*75 + Iram_Player_output[9] - 150;

        SeekToTarget();                          /* seek to Rw_seek_blk position */
        Svr_pause_reqire = TRUE;                    /* mark pause request */

        if ( ServoReadSubcode(1,0xFF) == FAIL )     /* read current subcode Q */
            return (1);                             /* if error occured */
    }

    if ( TEST_BIT(((BYTE *)PlayerAddrsPtr)[0], 6) ) /* if digital data */
        return (1);

    if ( ((BYTE *)PlayerAddrsPtr)[1] == 0xAA)       /* if reached lead out area */
        return (1);


    GetMsfFromServo((TimeS *)&Iram_Cmd_block.bf[3],
                    (BYTE *)PlayerAddrsPtr + 7);

    CheckStartLimitation();
    return (0);
}

/*********************************************************************
* CheckStartLimitation()
*
* Returns:      none
* Params:       none
*********************************************************************/
static void CheckStartLimitation(void)
{
    if ((Iram_Cmd_block.bf[3]== 0) && (Iram_Cmd_block.bf[4] < 2 ))
    {
        Iram_Cmd_block.bf[4]=2;
        Iram_Cmd_block.bf[5]=0;
    }
}
#endif // AUDIO_PLAYBACK

/*********************************************************************
* StopPlayScanCommand()
*
* When in play mode, stop playing. Turn on led.
*
* Returns:      void
* Params:       none
*********************************************************************/
void StopPlayScanCommand(void)
{
#if (AUDIO_PLAYBACK == 0)
    BUILD_SENSE(0x05,0x21,0x00,0x3c);        /* ILLEGAL REQUEST */
    AutoCommandComplete();                     /* cmd complete */
    return;
#else
    if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) ) {
        AutoCommandComplete();
        return;
    }

    /* if nx audio, turn off the audio and clear buffer dram */
    if (St_nx_audio == TRUE)
    {
        if (Iram_audio_scan_st == TRUE)
        {
            Iram_audio_scan_st = FALSE;

            if (Buf_scan_lba < PreCmd_endlba) {
                Rw_seek_blk.msf = LBA2MSF(Buf_scan_lba);

                Iram_Cmd_block.bf[3] = Rw_seek_blk.msf.Minute;
                Iram_Cmd_block.bf[4] = Rw_seek_blk.msf.Second;
                Iram_Cmd_block.bf[5] = Rw_seek_blk.msf.Frame;

                Iram_Cmd_block.bf[6] = (BYTE)(PreCmd_endlba >> 16) & 0xFF;
                Iram_Cmd_block.bf[7] = (BYTE)(PreCmd_endlba >>  8) & 0xFF;
                Iram_Cmd_block.bf[8] = (BYTE)PreCmd_endlba & 0xFF;
                Tg_endlba = PreCmd_endlba;

                Play_Cmd_block = Iram_Cmd_block;
                EnterNxTo1x();
                return;
            }
        }
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff;
        AudioTransferDone();
    }

    Svr_pause_reqire = TRUE;                /* mark pause request */
    Iram_Svr_player_mode = STOP_SCAN_MODE;  /* mark stop/scan */
    St_audio_complete = TRUE;               /* mark audio completed */
    St_audio_pause = FALSE;
    #if(PCB == 710)||(PCB == 711)||(PCB == 817) // Avoid Read CD command occur Buffer Full Hit after Stop Play Audio in IBM testing softwave.Fixed Data compare fail.
    ClearDataBuffer();
    #endif
    AutoCommandComplete();
#endif // AUDIO_PLAYBACK
}

#if (AUDIO_PLAYBACK == 1)
/**********************************************************************
* AudioTransferDone
*
* Register setting for stop play audio back in nx
*
* Returns:  void
* Params:   none
**********************************************************************/
void AudioTransferDone(void)
{
#if (LED_NUM == 1)
    ReadyLED(LED_OFF);
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
    ReadyLED(LED_AXferOn);
#endif /* LED_NUM == 2 */

#if (USE_TWO_KEY_FUNCTION==1)
    opAMute = 0;
    gHw_play_no = HW_PLAY_STOP_OFF;
#endif /* USE_TWO_KEY_FUNCTION */

    RampDownVolume();         // Ramp down Audio volume to level set in Sense Bytes

    St_audten_int = 0;              /* indicate audio xfer int. */

    WRITE_FIELD(HAL_AUD_APLAYEN, 0);// Disable Nx audio, Aux = 512
    WRITE_FIELD(HAL_AUD_A1KAUX, 0);

    /*TBD?  for preventing from endless audio pops after eject key is pressed. */
    //ClrBitsSCReg_1B(rwAIntEn, bABlkIEn);    /* Disable Audio Transfer End Interrupt */
    //ClrBitsSCReg_1B(rwAInt, bABlkInt);      /* Clear Audio Transfer End Status */
    if(Iram_Cmd_block.bf[0] != SBC_SEEK )  /* don't set DSC bit when SEEK_CD */
         { ATASetDSC(); }                  /* set DSC bit, indicates immediate mode cmd completed */
    svoIramVar.subQEnableFlag = 0;

}

/**********************************************************************
* MultiSessionCalculation
*
* Returns:  void
* Params:   none
**********************************************************************/
void MultiSessionCalculation(void)
{
    BYTE tno,total_sess,i;
    TimeS temp_time;
    ULONG temp_lba, start_lba, end_lba, lba_delta;

    start_lba = MSF2LBA(&Iram_Cmd_block.bf[3]);
    end_lba  = (ULONG)Iram_Cmd_block.bf[6] << 16;
    end_lba += (ULONG)Iram_Cmd_block.bf[7] << 8;
    end_lba += (ULONG)Iram_Cmd_block.bf[8] ;
    lba_delta = end_lba - start_lba;    /* lba_delta should be >= 0 */

    total_sess = pdbGetNumberOfSessionsOnDisc();

    /* Find which session (i) star_lba will be in */
    for(i = 1; i <= total_sess; i++){
        temp_time = pdbGetSessionLeadOutStart(i);
        temp_lba = MSF2LBA_Plus(&temp_time);
        if (start_lba < temp_lba){
            break;
        }
    }

    /* Adjust start_lba to the first track of session */
    tno = pdbGetFirstTrackOfSession(i);
    temp_time = pdbGetStartTime(tno);
    temp_lba = MSF2LBA_Plus(&temp_time);
    if (temp_lba > start_lba){
        start_lba = temp_lba;
        Iram_Cmd_block.bf[3] = temp_time.Minute;
        Iram_Cmd_block.bf[4] = temp_time.Second;
        Iram_Cmd_block.bf[5] = temp_time.Frame;
    }

    /* Adjust end_lba */
    end_lba = start_lba + lba_delta;

    /* Find which session (i) end_lba will be in */
    for(i = 1; i <= total_sess; i++){
        temp_time = pdbGetSessionLeadOutStart(i);
        temp_lba = MSF2LBA_Plus(&temp_time);
        if (end_lba < temp_lba){
            break;
        }
    }

    /* Adjust end_lba if it is in between session area */
    tno = pdbGetFirstTrackOfSession(i);
    temp_time = pdbGetStartTime(tno);
    temp_lba  = MSF2LBA_Plus(&temp_time);
    if ( end_lba < temp_lba) {
        /* end_lba is between session i-1 and session i */
        /* Adjust end_lba to end of session i-1 */
        temp_time = pdbGetSessionLeadOutStart(i-1);
        end_lba  = MSF2LBA_Plus(&temp_time)-1;
    }

    if ( i == (total_sess+1) )
    {
        /* end_lba is NOT in closed session area */
        /* Adjust end_lba within the last track end */
        temp_time = pdbGetStopTime( pdbGetHighestRecordedTrkNumber() );
        temp_lba  = MSF2LBA_Plus( &temp_time );
        if ( end_lba > temp_lba) {
            /* end_lba is NOT in recorded area */
            end_lba = temp_lba;
        }
    }

    /* Write back the end_lba */
    Iram_Cmd_block.bf[6] = (BYTE)((end_lba >> 16) & 0xff);
    Iram_Cmd_block.bf[7] = (BYTE)((end_lba >> 8) & 0xff);
    Iram_Cmd_block.bf[8] = (BYTE)end_lba & 0xff;

}
#endif // AUDIO_PLAYBACK

/**********************************************************************
* ScanCommand
*
* Audio Scan command  Start position is represented
*
* Returns:  none
* Params:   none
**********************************************************************/
#define SCAN_PLAY   16
void ScanCommand()
{
#if (AUDIO_PLAYBACK == 0)
    BUILD_SENSE(0x05,0x21,0x00,0x3c);        /* ILLEGAL REQUEST */
    AutoCommandComplete();                     /* cmd complete */
    return;
#else
    ULONG start_lba = 0, req_len =0;
    TimeS start_time;
    BYTE type, tno, last_tno;

    TimeS last_time;

    if (St_blank_disc)                      /* if blank disc */
    {
        BUILD_SENSE(0x05,0x21,0x00,0x3c);        /* ILLEGAL REQUEST */
        AutoCommandComplete();                     /* cmd complete */
        return;
    }

    if (St_cdrom_data)                      /* Check if data disc */
    {
        BUILD_SENSE(0x05,0x64,0x00,0x3d);        /* ILLEGAL REQUEST, ILLEGAL MODE FOR THIS TRACK */
        AutoCommandComplete();                     /* cmd complete */
        return;
    }

    St_zero_length = FALSE;                 /* assume not zero length request */

    /* address of starting */
    gAudioScanDirection = Iram_Cmd_block.bf[1] & 0x10; /* 0:foward 1:reversed */
    type = Iram_Cmd_block.bf[9] & 0xC0;     /* get type */
    switch (type){
        case 0x00:                          /* LBA format */
            start_lba = PacketCommandToLBALEN3(3);
            start_time = LBA2MSF(start_lba);
            Iram_Cmd_block.bf[3]    = start_time.Minute;
            Iram_Cmd_block.bf[4]    = start_time.Second;
            Iram_Cmd_block.bf[5]    = start_time.Frame;
            break;
        case 0x40:  /* MSF format */
            start_lba = MSF2LBA(&Iram_Cmd_block.bf[3]);
            break;
        case 0x80:  /* Tno format */
            tno = Iram_Cmd_block.bf[5];
            last_tno = pdbGetHighestRecordedTrkNumber();
            if (tno > last_tno){
                BUILD_SENSE(0x05,0x24,0x00,0x7f);    /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }
            if((pdbGetTrackContAddr(tno) & 0xF0) == 0x40){
                BUILD_SENSE(0x05,0x24,0x00,0x80);    /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }
            if(!gAudioScanDirection){  /* 0:foward 1:reversed */
                start_time = pdbGetStartTime(tno);
            }else{
                start_time = pdbGetStopTime(tno);
            }
            Iram_Cmd_block.bf[3]    = start_time.Minute;
            Iram_Cmd_block.bf[4]    = start_time.Second;
            Iram_Cmd_block.bf[5]    = start_time.Frame;
            start_lba = MSF2LBA(&Iram_Cmd_block.bf[3]);
            break;
        default:                                /* format illegal */
            BUILD_SENSE(0x05,0x24,0x00,0x81);        /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
    }

    /* play length */
    last_tno  = pdbGetHighestRecordedTrkNumber();
    last_time = pdbGetStopTime(last_tno);
    req_len   = MSF2LBA_Plus(&last_time);

    if((start_lba + SCAN_PLAY) > req_len ){
        start_time.Minute = Iram_Cmd_block.bf[3];
        start_time.Second = Iram_Cmd_block.bf[4];
        start_time.Frame  = Iram_Cmd_block.bf[5];
        mathSubtractAddressFromMSF(&start_time,SCAN_PLAY, &start_time);
        Iram_Cmd_block.bf[3]    = start_time.Minute;
        Iram_Cmd_block.bf[4]    = start_time.Second;
        Iram_Cmd_block.bf[5]    = start_time.Frame;
        start_lba = MSF2LBA(&Iram_Cmd_block.bf[3]);
    }

    /* Set ending address */
    /*  req_len += start_lba; */
    PreCmd_endlba = Tg_endlba;                  /* save previous Cmd requested LBA */
    if ( gAudioScanDirection ) {                /* Backward */
        Tg_endlba = 0L;
    } else {                                    /* Forward */
        Tg_endlba = req_len;
    }

    req_len --;
    Iram_Cmd_block.bf[6]    = (BYTE)((req_len >> 16) & 0xff);
    Iram_Cmd_block.bf[7]    = (BYTE)((req_len >> 8) & 0xff);
    Iram_Cmd_block.bf[8]    = (BYTE)req_len & 0xff;
    Play_Cmd_block = Iram_Cmd_block;
    PlayAudioCommon();
    Iram_audio_scan_st = TRUE;
#endif // AUDIO_PLAYBACK
}

#if (AUDIO_PLAYBACK == 1)
//****************************************************************************
//
//
//
//
//  Following are number of general purpose Audio Control functions.
//
//
//
//
//
//
//****************************************************************************


/*********************************************************************
* SetSpeakerVolume
*
* This routine check output mode request BYTE to decide which
* channel(s) to be enabled.If both or either one of the
* channels are enabled, then divides the values of R6 & R7
* by 2 respectively, then adds the two quotients, call the
* servo routine to set volume.
*
* Returns:  Volume value (= left+right volume/2)
* Params:   l_vol - Volume of output left channel
*           r_vol - Volume of output right channel
*********************************************************************/

/* resolves the problem when we swap the channel and set one
     of channel equal to 0 then 2 channels becomes no sound */
const BYTE CHANNEL_DATA[4][4] = {
/* ChA */   /*ChB mute          L ch          R ch           Mono   */
/* mute */  { eAMuteBMute,  eARightBLeft/*eAMuteBLeft*/,  eAMuteBRight,  eAMuteBMono  },
/* L ch */  { eALeftBMute,  eALeftBLeft,  eALeftBRight,  eALeftBMono  },
/* R ch */  { eARightBLeft/*eARightBMute*/, eARightBLeft, eARightBRight, eARightBMono },
/* Mono */  { eAMonoBMute,  eAMonoBLeft,  eAMonoBRight,  eAMonoBMono  }
};

void SetSpeakerVolume(BYTE l_vol, BYTE r_vol, BYTE l_sel, BYTE r_sel)
{
/* EFFECT control */
    fAudioSource = CHANNEL_DATA[l_sel][r_sel];

/* MUTE control */
    if ((l_vol == 0) && (r_vol == 0))
    {
        Channel_ctrl_mode = 0;
    }
    else if (l_vol == 0)
        {
            Channel_ctrl_mode = 01;
        }
        else if (r_vol == 0)
            {
                Channel_ctrl_mode = 02;
            }
            else
            {
                Channel_ctrl_mode = 03;
            }

#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio != TRUE)
    {
        Channel_ctrl_mode = 0;
        l_vol = r_vol = 0;
    }
    SetAudioChannelMode(Channel_ctrl_mode);
    SetAudioVolume(l_vol,r_vol);
#endif
}

/*********************************************************************
* Function:    SetAudioVolume
*
* Call player to set volume.
*
* Inputs: param1 - volume value
*
* Returns:     void
* Params: none
*********************************************************************/
void SetAudioVolume(BYTE LChVolume,BYTE RChVolume)
{
    if(( fLeftRightChannelvolume.LChVolume != LChVolume )||
       ( fLeftRightChannelvolume.RChVolume != RChVolume ))
    {
        fLeftRightChannelvolume.LChVolume = LChVolume;
        fLeftRightChannelvolume.RChVolume = RChVolume;
    }
    if (Iram_Audio_blk2_play > 0x14 ){  /* for prevent noise from create image disk by NERO */
        WRITE_REG(HAL_AUDREG_MAP->ATTLA, ((fLeftRightChannelvolume.RChVolume<<8)|fLeftRightChannelvolume.LChVolume));
    }
    if((fLeftRightChannelvolume.LChVolume == 0)&&
       (fLeftRightChannelvolume.RChVolume == 0))
    {
        MuteOn();
    }
    else
    {
        MuteOff();
    }


}

/*********************************************************************
* Function:    SetAudioDacMode()
*
* Description: Gets new Audio settings for Mute, Left, Right, Mono
*              from Player Database and updates system with these new settings.
*
* Returns:     READY
*
* Params:      none
*********************************************************************/
void SetAudioDacMode(AudioDACOutputControlT dac)
{
BYTE    temp;

    temp = READ_REG(HAL_AUDREG_MAP->AMUTE);
    temp &= (HAL_AUD_DACMUTE | HAL_AUD_MUTE4X);       // Mask out bits
#if (DIG_AUDIO == 1)
    temp |= (dac | eDOut);
#endif
    WRITE_REG(HAL_AUDREG_MAP->AMUTE, temp);
    if((temp&eMuteAB) == eMuteAB){
        MuteOn();
    }else{
        MuteOff();
    }
    temp = READ_REG(HAL_AUDREG_MAP->AROUTE);
    temp &= 0xF0;                      // Clear Channel Select bits
    temp |= fAudioSource;
    WRITE_REG(HAL_AUDREG_MAP->AROUTE, temp);
}

/**********************************************************************
* Function:    SetAudioChannelMode
*
* Description: _set_output_mode_byte are passed from the caller.
*
* Params:      output_mode
*                00: kill both left and right channel
*                01: kill left and enable right channel
*                10: kill right and enable left channel
*                11: enable both channel
*
* Returns:     1 = command failed
*              0 = command suceeded
**********************************************************************/
BOOL SetAudioChannelMode(BYTE output_mode)
{
    switch (output_mode & 0x0F)
    {
        case 0x00: /* case 0x00: */     // put case 0x00 to top for seek performance.
            output_mode = eMuteAB;      /* kill left and right */
            break;

        case 0x03:
            output_mode = eMuteOffAB;   /* enable both */
            break;
        case 0x01:
            output_mode = eMuteAOffB;   /* kill left only */
            break;
        case 0x02:
            output_mode = eMuteBOffA;   /* kill right only */
            break;

        default: /* case 0x00: */
            output_mode = eMuteAB;      /* kill left and right */
    }
    SetAudioDacMode(output_mode);
    return(PASS);
}

/*-----------------------------------------------------------------------------
 *
 * Module: MuteOn()
 * Description: MUTE ON
 *
 *----------------------------------------------------------------------------*/
void MuteOn(void)
{
    /* preventing from endless audio pops after eject key is pressed. */
    WRITE_FIELD(HAL_AUD_DACMUTE,1); // "Mute" Audio.
    WRITE_FIELD(HAL_AUD_MUTE4X,1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: MuteOff()
 * Description: MUTE OFF
 *
 *----------------------------------------------------------------------------*/
void MuteOff(void)
{
    /* for preventing from endless audio pops after eject key is pressed. */
    WRITE_FIELD(HAL_AUD_DACMUTE,0);         // Un-Mute Audio. Enable Sound.
    WRITE_FIELD(HAL_AUD_MUTE4X,0);
}

/*********************************************************************
* Function:   SetDefaultVolume()
*
* Description: Sets volume.
*
* Returns:     READY
*
* Params:      none
*********************************************************************/
void SetDefaultVolume(BYTE l_vol,BYTE r_vol)
{
    fLeftRightChannelvolume.LChVolume = l_vol;
    fLeftRightChannelvolume.RChVolume = r_vol;
    fAudioSource = eALeftBRight;
}


/*********************************************************************
* RampUpVolume
*
* This routine ramps up both left and right speaker volume.
*   to level contained in Sense Bytes
*
* Returns:  void
* Params:   none
*********************************************************************/
void RampUpVolume(void)
{
BYTE * pVolume;
BYTE LeftVolume,RightVolume;

    BYTE LeftCurrent,RightCurrent;

    if(St_zero_length == TRUE)
    {                            // If Host command requested a length of zero blocks of Audio to play.
        LeftVolume  = 0x00;   // Set  left volume to mute.
        RightVolume = 0x00;   // Set right volume to mute.
    }
    else  // Get Default volume from Mode Page E
    {
        pVolume = (BYTE *)SCSDRAM_START_ADDR+ MSENSE_PAGE_E + 9;    // Get Volume setting from Sense Bytes
        LeftVolume  = *pVolume;
        pVolume += 2;
        RightVolume = *pVolume;
    }

    LeftCurrent  =1;                                   // Use programmed stepping of attenuation to ramp up volume.
    RightCurrent =1;
    while ((LeftCurrent < LeftVolume) || (RightCurrent < RightVolume))
    {   /* prevent the analysis delay in  Nero Create audio image file */
            if ( (LeftVolume-LeftCurrent)>20){
                        LeftCurrent += 20;
                }
            else LeftCurrent = LeftVolume;

            if (( RightVolume-RightCurrent)>20){
                        RightCurrent += 20;
                }
            else RightCurrent = RightVolume;
        SetAudioVolume(LeftCurrent,RightCurrent);
        DelaymS(1);
    }
}

/*********************************************************************
* RampDownVolume
*
* This routine ramps down both left and right speaker volume.
*   to level contained in Sense Bytes
*
* Returns:  void
* Params:   none
*********************************************************************/
void RampDownVolume(void)
{

BYTE    * pVolume;
BYTE    LeftCurrentVolume,RightCurrentVolume;

    pVolume = (BYTE *)SCSDRAM_START_ADDR+ MSENSE_PAGE_E + 9;    // Get Volume setting from Sense Bytes
    LeftCurrentVolume  = *pVolume;
    pVolume += 2;
    RightCurrentVolume = *pVolume;

    while ((LeftCurrentVolume!=0)||(RightCurrentVolume!=0))       // Use programmed stepping of attenuation to ramp down volume to 0.
    {   /* for prevent the analysis delay in  Nero Create audio image file */
                if (LeftCurrentVolume -20 <=0)
                    LeftCurrentVolume = 0;
                else
                    LeftCurrentVolume -= 20;
            if (RightCurrentVolume - 20<=0)
                RightCurrentVolume = 0;
            else
                    RightCurrentVolume -= 20;

        /* Prevent from endless audio pops after eject key was pressed. */
        if((LeftCurrentVolume==0)&&(RightCurrentVolume==0))
        {
            WRITE_FIELD(HAL_AUD_ABLKIEN,0);     /* Disable Audio Transfer End Interrupt */
            CLEAR_INT_FIELD(HAL_AUD_ABLKINT,0);     /* Clear Audio Transfer End Status */
        }

        SetAudioVolume(LeftCurrentVolume, RightCurrentVolume);
        DelaymS(1);
    }
}


#if (USE_TWO_KEY_FUNCTION==1)
extern BYTE sTrackNO, eTrackNO;
/*********************************************************************
* KeyPlayAudio
*
*********************************************************************/
void KeyPlayAudio(BYTE flg)
{
    TimeS   start_time, stop_time;
    BYTE    left_vol,right_vol;
    BYTE    HW_Cur_Tno;

    if (!flg)
    {
        if (gPause_By_SeekCmd_flag == TRUE)
        {
            gPause_By_SeekCmd_flag = FALSE;
            start_time = LBA2MSF(Tg_endlba);
        }
        else
        {
            start_time = pdbGetStartTime(sTrackNO);
        }

            Play_Cmd_block.bf[3] = start_time.Minute;
                Play_Cmd_block.bf[4] = start_time.Second;
                Play_Cmd_block.bf[5] = start_time.Frame;
        }
    else
    {
        if (( St_audio_pause) || (Iram_audio_scan_st == TRUE))
        {
            St_audio_pause = FALSE;

            if (Iram_audio_scan_st == TRUE){
                Iram_audio_scan_st = FALSE;
                Rw_seek_blk.msf = LBA2MSF(Buf_scan_lba);
            }
            Play_Cmd_block.bf[3] = Rw_seek_blk.msf.Minute;
            Play_Cmd_block.bf[4] = Rw_seek_blk.msf.Second;
            Play_Cmd_block.bf[5] = Rw_seek_blk.msf.Frame;
        }
        else
        {
                    HW_Cur_Tno = BCD2HEX(pdbGetSubQTno());
                HW_Cur_Tno++;
                if (HW_Cur_Tno > eTrackNO)  HW_Cur_Tno = sTrackNO;
                start_time = pdbGetStartTime(HW_Cur_Tno);
                Play_Cmd_block.bf[3] = start_time.Minute;
                    Play_Cmd_block.bf[4] = start_time.Second;
                    Play_Cmd_block.bf[5] = start_time.Frame;
            }
    }

    stop_time = pdbGetStopTime(eTrackNO);
    Play_Cmd_block.bf[6] = stop_time.Minute;
    Play_Cmd_block.bf[7] = stop_time.Second;
    Play_Cmd_block.bf[8] = stop_time.Frame;

    if (flg)
    {
        Tg_endlba = MSF2LBA(&Play_Cmd_block.bf[6]);

        ReadyLED(LED_OFF);
        Iram_Buf_dram_addr = MSENSE_PAGE_E + 9;
        left_vol  = ReadSDRAM();
        Iram_Buf_dram_addr = MSENSE_PAGE_E + 11;
        right_vol = ReadSDRAM();
        while ((left_vol > 0)||(right_vol > 0))
        {
            if (left_vol!=0)
                left_vol--;
            if (right_vol!=0)
                right_vol--;
            SetAudioVolume(left_vol, right_vol);
            DelaymS(1);
        }
        //DISABLE_CONCEALMENT;
        EnterNxTo1x();
    }
    else
        PlayAudioMSFCommand();
}
#endif  /* USE_TWO_KEY_FUNCTION */
#endif // AUDIO_PLAYBACK
