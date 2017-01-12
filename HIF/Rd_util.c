/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   rd_util.c
*
* DESCRIPTION
*   This file contains command handler of ATAPI command
*
*   $Revision: 154 $
*   $Date: 11/03/28 2:37p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\common\commath.h"
#include ".\common\error.h"
#include ".\common\constant.h"
#include ".\servo\svo.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_equ.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\s2b_main.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\app_if.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_mdsns.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvdiscrd.h"
#if (BD_ENABLE == 1)
#include ".\bd\bdMacro.h"
#endif
#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\reg_intctrl.h"
#include ".\al\HAL_demod.h"
#include ".\common\intctl.h"
#include ".\oem\oem_spd_tbl.h"

#ifdef READ10_SSI  // Read10 command support on SSI port
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif
#endif

#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif
#if (CHIP_REV <= 0xB2)
extern BYTE UseC1CG1;
#endif
static BYTE MonitorTargetInterrupt(void);

/*********************************************************************
* SeekToTarget
*
* This function seeks to the block specified by rw_seek_blk.
*
* Returns:      PASS or FAIL
* Params:       none
*********************************************************************/
vBOOL SeekToTarget(void)
{
    TimeS temp;


    StopDecoder();

    /* Issue seek command to player module. Target: rw_seek_blk */
    if(St_method_2 == FALSE)
    {
        temp  = LBA2MSF(Rw_seek_blk.lba);
    }
    else
    {
        temp = Method2LBAToMSF(Rw_seek_blk.lba);
    }

    Target_Timelba = MSF2LBA_Plus(&temp);

#if (SEQ_SEEK_SPEED_DOWN == 1)
    if (RandomRead)
    {
        if(SpeedCount > 0) // don't count the read retry seek.
        {
            RandomCount++;
            if (RandomCount == 0xFFFFFFFF)  //wrap back
                RandomCount = CONTINUE_RANDOM_READ_CNT + 1;

            if (RandomCount > CONTINUE_RANDOM_READ_CNT )
                SpeedMD = Special_for_SeekSpeed;
        }
    }
    else
    {   //if any sequential read coming

        if(SpeedMD == Special_for_SeekSpeed)
        {
            if(HostXfer.BlockCount.Long == 0x19)
            {
                SpeedMD = Special_for_CDSpeed_f;
                SpeedCount = CD_SPEED_UP_READY - 0x200;
            }
            else
            {
                SpeedMD = AutoMode;
                SpeedCount = 0;
            }
        }

        RandomCount = 0;
    }
#endif
    Set_SpeedMDCont();

    send_msg80(SEND_ATAPI_CMD,0x800001);   //Seek Start
    if ( ServoSeek(Target_Timelba, eDataSeek) == PASS )
    {                                       /* if seek done */
        Svr_pause_reqire = FALSE;
        Iram_Svr_player_mode = READING_MODE;        /* Change to reading_mode */
        /* if nx-1x audio, change to play_track_mode */
        if (St_nx_audio == TRUE)
            Iram_Svr_player_mode = PLAY_TRACK_MODE;

        SendMsgCn(SEND_ATAPI_CMD,1,0x800005, TRUE);       //Seek End, Success=%u
        return(PASS);
    }
    else /* ( ServoSeek(Target_Timelba) == FAIL ) */
    {
        SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);      //Seek End, Success=%u
        if(SpeedUpReady < SPEED_UP_READY_LIMIT)
            SpeedUpReady += 512;    // Delay speed up
    }

    return(FAIL);
}

/*********************************************************************
* AdjustSeekOffset
*
* This function adjust the rw_seek_blk with the offset
* between the header and subcode.
*
* Returns:      void
* Params:       none
*********************************************************************/
void AdjustSeekOffset(void)
{
}

/****************************************************************************
* WaitForCacheHit
*
* This routine will wait for start_block to be reached in decoder interrupt.
* Then,the start block should be just few blocks away form current decoding
* block. It is useful for not wasting time to issue a new seek.
*
* Returns:  same as start_buffering
* Params:   none
****************************************************************************/
BYTE WaitForCacheHit(void)
{
    long    lba;
    vBOOL   flg;

    Knl_deci_set = FALSE;

    while(Rw_dec_stop == FALSE)
    {
        DISABLE_INTERRUPT();            /* Disable all interrupt */

        if(St_run_in_out == TRUE)
            Iram_Knl_timer_counter = 10;        /* Wait about 200 ms */
        else
            Iram_Knl_timer_counter = 5;     /* Wait about 100 ms */

        Knl_timeout = FALSE;            /* Assume no time out */
        ENABLE_INTERRUPT();             /* Enable all interrupt */

        while (Knl_deci_set == FALSE)
        {
            if(Rw_dec_stop == TRUE)     /* if time out */
            {
                Knl_timeout = FALSE;
                return (0x80);
            }
            if(EjectKeyInfo.Pressed)
                return (0x80);
            if(Knl_timeout == TRUE)     /* if time out */
            {
                Knl_timeout = FALSE;
                return (0x80);
            }

            if(svoIramVar.focusDropFlag == TRUE)
            {
                svoIramVar.emergencyStopFlag = TRUE;
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
                //ServoStop();
                return (0x80);
            }

            ExitProcess();
        }                               /* wait next decoder int */

        DISABLE_INTERRUPT();            /* Disable all interrupt */
        Knl_deci_set = FALSE;
        /* compare start block's min with header min register */
        LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheEndLBA, lba, flg);
        if(flg)                         /* if start < dec */
        {
            /* if start_block <= xfred block  ===> cache miss */
            LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheStartLBA, lba, flg);
            if (!flg )                  /* if start > xfer */
            {
                if( (lba <= DataBuffer.SectorCount))
                {
                    if (lba == 0)
                    {
                        ENABLE_INTERRUPT(); /* Enable all interrupt */
                        return(0x80);
                    }
                    /* cache_hit */
                    lba--;
                    if (lba != 0)
                    {
                        DataBuffer.CurHostPtr += (USHORT) lba;
                        if ( DataBuffer.CurHostPtr > RING_END_PAGE)
                        {
                            DataBuffer.CurHostPtr %=BUF_TOTAL_BLK_COUNT;
                        }
                            DataBuffer.SectorCount -= (USHORT) lba;
                    }
                    DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
                    MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, 1);
                    ENABLE_INTERRUPT(); /* Enable all interrupt */
                    return (0);         /* clear acc to indicate no err */
                }
            }
            ENABLE_INTERRUPT();         /* Enable all interrupt */
            return(0x80);
        }
        else if (lba == 0)              /* if target reached */
        {
            /* entry point of buffering target block success */
            DataBuffer.SectorCount = 1;             /* mark only one block */
            DataBuffer.CurHostPtr = DataBuffer.CurDiscPtr;      /* ptr to host */
            if ( DataBuffer.CurHostPtr == 0)
                DataBuffer.CurHostPtr = RING_END_PAGE;
            else
                DataBuffer.CurHostPtr --;
            DataBuffer.CacheEndLBA = HostXfer.StartBlock.lba;
            DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
            MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, 1);
            ENABLE_INTERRUPT();         /* Enable all interrupt */
            return (0);                 /* indicate no err */
        }
        else if (lba > 128)             /* if target too far away */
        {
            ENABLE_INTERRUPT();         /* Enable all interrupt */
            return(0x80);
        }
        ENABLE_INTERRUPT();             /* Enable all interrupt */
    }                                   /* end while - retry loop */

    ENABLE_INTERRUPT();                 /* Enable all interrupt */
    return (0x80);
}

/****************************************************************************
* StartBufferingFromTarget
*
* This routine complete the following tasks
*    (1) Identify the mode of disc if not identified yet
*    (2) Obtain subcode and block header offset to achieve better
*            seek performance if not got the offset yet.
*    (3) start_buffering performs data decoding and buffering for
*            1st block.
*
* Returns:  1. MSB set on overshoot error.
*           2. Non-zero --> error code
* Params:   none
****************************************************************************/
extern vBYTE rd_method;
BYTE StartBufferingFromTarget(void)
{
    USHORT bptr;

    if (St_cdrom_audio)
    {                                       /* if it is cdda disc */
        Iram_Rw_read_mode = eUnknownMode;
        return(0x38);                       /* return error */
    }

    /* read data with subcode */
    if(Rw_subc_data == TRUE)
    {
        Iram_Knl_timer_counter = 150;           // Wait about 3 s
        Knl_timeout = FALSE;                    // Assume no time out

// TBD? PORT?    SCOR Logic was removed from CDDECODER see #4388 Some parts have moved to Demodulator module.
        while(1)
        {
                if (HAL_DEMOD_GetFePllLocked()) // If both inner and outer frequency lock
                break;                          //   then the Sync is good.

            // Sync was not good, now reset logic. TBD
            // TBD   Reset Demodulator. Do not use Demodulator Soft Reset. This would reset all Demodulator register to initial values.

            pdbSetSubQIntStatus(FALSE);

            while((pdbGetSubQIntStatus() != TRUE) && (Knl_timeout == FALSE))
            {
                ExitProcess();
            }
            if(Knl_timeout == TRUE)             // if time out
            {
                Knl_timeout = FALSE;
                break;
            }
        }
    }

    DISABLE_INTERRUPT();

    /* Entry point for new buffering. Abandon the previous content in the
       buffer. */
    ResetCDDecoder();

    Iram_Sh_wbk = 0;
    ClearDataBuffer();
    bptr = DataBuffer.CurDiscPtr;
    if (bptr == 0)
        bptr = RING_END_PAGE;
    else
        bptr --;

    WRITE_FIELD(HAL_CDD_DECPG, bptr);               /* Init decoder buffer page pointer */
    WRITE_FIELD(HAL_CDD_DSUBPG, bptr);
    WRITE_FIELD(HAL_HST_HSTBUFPG, bptr);            /* Init Host buffer page pointer */
    WRITE_FIELD(HAL_HST_BUFPGCMP, bptr);
    WRITE_REG(HAL_CDDREG_MAP->C2THLD, 0x00);

#if(CEProduct == 0)    // For IT Drives
    /* For Nero CD/DVD speed transfer rate test:
         If the Perod_pg is 0 or 2, the transfer rate is not smooth after 40x.
         Set Period_pg to MAX_CD_DECODE_PERIOD for smooth transfer rate curve. */
    if ( HostXfer.BlocksLeft <= 2 )
    {
        Period_pg = 0;
    }
    else if( ( ( Maxlba - HostXfer.StartBlock.lba ) <= MAX_CD_DECODE_PERIOD * 2  )  || ( HostXfer.BlocksLeft  < MAX_CD_DECODE_PERIOD ) )
    {
        Period_pg = 2;
    }
    else
    {
        Period_pg = MAX_CD_DECODE_PERIOD;
    }
#else // For AV Drives
    if ( HostXfer.BlocksLeft <= 2 ) {
        Period_pg = 0;
    }else {
        Period_pg = 2;
    }
#endif

    if ((rd_method == WR_METHOD_FP) || (rd_method == WR_METHOD_VP)){
        Period_pg = 0;
    }

    WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);

    if (Iram_Page_Size_3K_Flag == 1)    /*Automatic change from 2.5K to 3k page for C2PO*/
    {
        if(Iram_shHDDir & HAL_HST_HMLY)
        {
            Iram_shHDDir &= ~HAL_HST_HMLY;             /* 3K Host Layout*/
            WRITE_FIELD(HAL_CDD_MLY, e_Page3K); /* Memory layout 3K */
        }
    }

    ENABLE_INTERRUPT();

    return(MonitorTargetInterrupt());
}

/*********************************************************************
* MonitorTargetInterrupt(void)
*
* Returns:      same as start_buffering
* Params:       none
*********************************************************************/

extern vULONG Iram_Target_lba;
extern BYTE retry_flag; /* for read mix mode disc */
#if (SEEK_ADJ == 1)
extern LONG last_PBA;
#endif
static BYTE MonitorTargetInterrupt(void)
{
#if (SEEK_ADJ == 1)
    LONG    current_PBA;
#endif
    TimeS   msf;
    BYTE    a;

    DISABLE_INTERRUPT();                /* Disable all interrupt */

    Knl_deci_set   = FALSE;             /* ignore previous decoder interrupt */
    Rw_dec_stop    = FALSE;             /* mark decoder running  */
    Rw_tblk_reach  = FALSE;             /* mark no reach target blk */
    Rw_mode_change = FALSE;             /* mark no mode changed */
    St_run_in_out  = FALSE;

    Rw_ecc_error = FALSE;               /* assume no ecc error */
    Iram_RW_ECC_Status = 0x0;           /* initial ecc value */

    if( St_method_2 == FALSE)
    {
        msf = LBA2MSF(HostXfer.StartBlock.lba);
    }
    else
    {
        msf = Method2LBAToMSF(HostXfer.StartBlock.lba);
    }

    a = HEX2BCD(msf.Minute);
    WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);   /* set target address MIN */

    a = HEX2BCD(msf.Second);
    WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);   /* set target address SEC */

    a = HEX2BCD(msf.Frame);
    WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);   /* set target address BLK */

    Iram_Target_lba = MSF2LBA_Plus(&msf) - 1;

    DataBuffer.CacheEndLBA = HostXfer.StartBlock.lba;

    MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheEndLBA, 1);
    DataBuffer.CacheStartLBA = DataBuffer.CacheEndLBA;

    fRStart = FALSE;
    fRdMiss = FALSE;
    Knl_deci_set = FALSE;

    Rw_ecc_mode();                      /* set to Buffering ROM mode */
    Iram_Knl_timer_counter = 6;         /* Wait about 120 ms */
    if(Current_read_sp <= MIN_READ_SPEED){
        Iram_Knl_timer_counter = 15;    /* Wait about 300 ms */
    }
    Knl_timeout = FALSE;                /* Assume no time out */
    ENABLE_INTERRUPT();

    while (1)                           /* wait for next decoder int */
    {
#if (SEEK_ADJ == 1)//TBD:this is only used for data (Audio doesn't have header)
        if (last_PBA != 0x7FFFFFFF)
        {
            current_PBA = BCD2HEX(READ_REG(HAL_CDDREG_MAP->HEAD0)) * 4500L + BCD2HEX(READ_REG(HAL_CDDREG_MAP->HEAD1)) * 75L + BCD2HEX(READ_REG(HAL_CDDREG_MAP->HEAD2));
            if (current_PBA != 0)
            {
                // (SEEK_ADJ2:RPM,Pos,parfine,diff_PSN)%5d	%5d	%5d	%5d
                SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF58,B2B(spdGetCurrentRPM()),B2B(Iram_stp_CurrentPosition),B2B(svoSeekVar.parFine),B2B(current_PBA - last_PBA));
                last_PBA = 0x7FFFFFFF;
            }
        }
#endif
        if (fRStart == TRUE)
        {
            break;
        }

        if (fRdMiss == TRUE)
        {
            /* Overshoot error */
            fRdMiss = FALSE;
            return(0x80);               /* indicate error */
        }

        if(Rw_dec_stop) {
            return(0x80);               /* indicate error */
        }

#if WITHOUT_SERVO == 0
        if(Knl_timeout == TRUE)
        {
            send_msg80(SEND_HIF_CMD,0x710063);
            ResetCDDecoder();
            Knl_timeout = FALSE;
            return (0x81);
        }
#endif /* WITHOUT_SERVO */
        if(EjectKeyInfo.PressedValid)    /* if key pressed or time out */
        {
            return (0x80);
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            return (0x80);
        }

        ExitProcess();
    }
    /* read started */

/* read mix mode disc */
/*seek retry more than 3 times, and mix mode disc, clear bMSFCErrEn*/
    if((retry_flag == 1) && (St_cdrom_mixed == 1 || St_cdi))    //St_cdi for bad quality CD-I
        WRITE_FIELD(HAL_CDD_MSFCERREN, 0);

    Iram_Knl_timer_counter = 10;        /* Wait about 200 ms */
    Knl_timeout = FALSE;                /* Assume no time out */
    while (1)                           /* wait for the first Period Intr. */
    {
        if(Knl_deci_set == TRUE)
            break;

        if(Rw_dec_stop) {
            break;
        }

#if WITHOUT_SERVO == 0
        if(Knl_timeout == TRUE)         /* if time out */
        {
            send_msg80(SEND_HIF_CMD,0x710064);
            Knl_timeout = FALSE;
            return (0x80);
        }
#endif /* WITHOUT_SERVO */

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            return (0x80);
        }

        ExitProcess();
     }

    /* Decoder block just occurred. Check if is valid mode or mode
       has changed */
    if (DataBuffer.SectorCount == 0)
    {
        /* for mode change */
        if((Iram_RW_ECC_Status & MODERROR) == MODERROR)
            return(0x81);
        else
            return(0x80);
    }
    return(0x0);                        /* return OK */
}

/*************************************************************************
* EccCtrlMode
*
* This routine is used set ecc control register
*
* Returns:      void
* Params:       Disc type
*************************************************************************/
#if DEFECT_LENGTH_MEASURE == 1     
extern BYTE CD_Scratch_Disc;
#endif

void EccCtrlMode(BYTE disctype)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    #if (USING_C1 == 0) /* Disable C1 Interrupt */
    if(Iram_Cmd_block.bf[0] != DIAG_VENDOR_READ)
        WRITE_REG(HAL_CDDREG_MAP->C12CCR, 0);               // count any C1 error + any C2 error
    else
        WRITE_REG(HAL_CDDREG_MAP->C12CCR, HAL_CDD_C1UNC);   // count any C1 error + C2 uncorrectable only
    #endif
#endif

    switch(disctype)
    {
        case eDataCD:
#if (CHIP_REV <= 0xB2)
            if (UseC1CG1)
                WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL2<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG1|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);
            else
                WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL2<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);
#else
            WRITE_FIELD(HAL_CDD_DECOC2ERR, 1);
            if ( (Iram_Rw_read_mode == eMode1)||(Iram_Rw_read_mode == eMode2Form1) )
            {
                //HAL_CDD_C2NOC1FLG is set in EDCNoGoodIntProc()
                WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL0<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG1|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);  //0x07
            }
            else
            {
    #if DEFECT_LENGTH_MEASURE == 1            
                if(CD_Scratch_Disc==1)
                {
                    WRITE_FIELD(HAL_CDD_C2NOC1FLG, 0);
                    WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL0<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);    //0x03
                }
                else
    #endif               
                    WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL0<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG1|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);  //0x07

            }
#endif
            WRITE_FIELD(HAL_CDD_ITH1, 1);       //Disable Audio Concealment, default is enabled.
            break;
        case eAudioCD:
        default:
            WRITE_REG(HAL_CDDREG_MAP->ECCCTL,(halCDD_C2CG_SEL0<<HAL_CDD_C2CG_LSB)|HAL_CDD_C1CG0|HAL_CDD_C1C2EN);        //0x03
            WRITE_FIELD(HAL_CDD_DECOC2ERR, 1);  //TBD:Need to try!
            WRITE_FIELD(HAL_CDD_C2NOC1FLG, 0);  //TBD:Need to try!
            WRITE_FIELD(HAL_CDD_ITH1, 0);       //Enable Audio Concealment
            break;
    }

//Different Read speed may need to use different delay to detect Sub code sync (For Audio Disc DAE test)
#if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_CDD_SCDLYSEL, 0x01); //Change delay program by first audio byte
#endif
#if (SYSCLK == 125)
    if(GetSpeedValue(Current_read_sp) >= e32X)      //32XCAV~48XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0xC0);
    else                                            //4XCLV~24XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0x0);
#elif (SYSCLK == 122)
    if(GetSpeedValue(Current_read_sp) >= e32X)      //32XCAV~48XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0xC0);
    else                                            //4XCLV~24XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0x0);
#else//(SYSCLK == 100)
    if(GetSpeedValue(Current_read_sp) >= e48X)      //48XCAV use//TBD:This setting is also not good for 48X
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0x7C);
    else if(GetSpeedValue(Current_read_sp) >= e40X) //40XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0xC0);
    else                                            //4XCLV~32XCAV use
        WRITE_FIELD(HAL_CDD_SCVALDLY, 0x120);
#endif

}

/*************************************************************************
* Rw_ecc_mode(void)
*
* Program decoder to Real Time Correction Mode
*
* Returns:  void
* Params:   none
*
* rwBufCtl      0x0D6   : AUTOMATIC BUFFRING CONTROL REGISTER
*   bDAPlay     (bit7)  : *Data xfer or Audio play back selest
*   bAutoSBuf   (bit5)  : *Subcode auto bufferring
*   bSMSFCErr   (bit4)  : *Stop Buffring on MSF Continuity Error
*   bSC3Err     (bit3)  : *Stop Buffring on C3 EDAC Error
*   bSDIFErr    (bit2)  : *Stop Buffring on DSP Error (illegal sync or DSP FIFO over-run)
*   bSBFull     (bit1)  : *Stop Buffering on Buffer Full
*   bAutoPkt    (bit0)  : *Automatic Packet Mode Enable
*
* rwDIntEn0     0x0E5   : DECODER INTERRUPT GROUP 0 ENABLE REGISTER
*   bRO1ErrEn   (bit7)  : Run-Out 1 Missed Error Interrupt Enable
*   bR14ErrEn   (bit6)  : Run-In 4 Missed Error Interrupt Enable
*   bLnkAEn     (bit5)  : Link Area Block Interrupt Enable
*   bRdMissEn   (bit4)  : *Read Missed (Target Overshoot) Interrupt Enable
*   bBFullEn    (bit2)  : *Buffer Full Interrupt Enable
*   bMSFCErrEn  (bit1)  : *MSF Continuity Error Interrupt Enable
*   bEDACErrEn  (bit0)  : *ECC or EDC Error Interrupt Enable
*
* rwDIntEn1     0x0E6   : DECODER INTERRUPT GROUP 1 ENABLE REGISTER
*   bQAddr0En   (bit7)  : Subcode Q Block Address 0 Detected Interrupt Enable
*   bHdrErrEn   (bit6)  : MSF C2 or QCRC Error Interrupt Enable
*   bSRWErrEn   (bit5)  : Subcode R-W ECC Error Interrupt Enable
*   bMissSEn    (bit4)  : Missing Subcode Sync Interrupt Enable
*   bDC2ErrEn   (bit3)  : *Data C2 Error Interrupt Enable
*   bIlSSEn     (bit2)  : Illegal Subcode Sync Interrupt Enable
*   bDIfErrEn   (bit1)  : DSP Data Interface Error Interrupt Enable
*   bNoSyncEn   (bit0)  : No CD-ROM or Subcode Sync Detected Interrupt Enable
*
* rwDIntEn2     0x0E7   : DECODER INTERRUPT GROUP 2 ENABLE REGISTER
*   bBSyncEn    (bit7)  : Block Sync Interrupt Enable
*   bROut1En    (bit6)  : *Run-Out 1 Detected Interrupt Enable
*   bSubRdyEn   (bit5)  : Subcode Block Ready Interrupt Enable
*   bHdRdyEn    (bit4)  : *Header and Sub-header Ready Interrupt Enable
*   bRIn4En     (bit3)  : *Run-In 4 Detected Interrupt Enable
*   bTFndEn     (bit2)  : *Target Found Interrupt Enable
*   bDPrdEn     (bit1)  : *Periodic Decoder Interrupt Enable
*   bQAddr23En  (bit0)  : Subcode Q Block Address 2 or 3 Detected Interrupt Enable
*
*************************************************************************/
void Rw_ecc_mode(void)
{
    WRITE_FIELD(HAL_CDD_AEWAEN, 1);         /* Enable auto page offset load */

    if ( rd_method == WR_METHOD_FP )
    {
        WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RO1ERREN | HAL_CDD_R14ERREN |
            HAL_CDD_RDMISSEN | HAL_CDD_MSFCERREN  | HAL_CDD_EDACERREN) );

        WRITE_REG(HAL_CDDREG_MAP->DINTEN2, (HAL_CDD_ROUT1EN | HAL_CDD_RIN4EN |
            HAL_CDD_TFNDEN | HAL_CDD_DPRDEN | HAL_CDD_HDRDYEN) );

        WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_SC3ERR | HAL_CDD_SDIFERR | HAL_CDD_AUTOPKT) );  /*read data with subcode */
    }
    else
    {
        if TestBitsModeP1_B2(DCR) /*(((SCSDRAM[MSENSE_PAGE_1 + 2]) & 0x01) == 0x01) */ /* DCR == 1, Disable Correction */
        {
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RDMISSEN | HAL_CDD_MSFCERREN) );

            WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_SDIFERR) ); /* read data with subcode */
        }
        else
        {
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RDMISSEN | HAL_CDD_MSFCERREN  | HAL_CDD_EDACERREN) );

            WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_SC3ERR | HAL_CDD_SDIFERR) );    /* read data without subcode */
        }
        WRITE_REG(HAL_CDDREG_MAP->DINTEN2, (HAL_CDD_TFNDEN | HAL_CDD_DPRDEN | HAL_CDD_HDRDYEN) );
    }
    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, (HAL_CDD_DC2ERREN | HAL_CDD_DIFERREN) );

    WRITE_FIELD(HAL_CDD_YBM2DIS, 0);   /* YBM2 auto-detection enable */


  #if WITHOUT_SERVO
    Iram_Rw_read_mode = eMode1;     /* User must set ReadMode to correct mode depend on the data format feed in */
  #endif /* WITHOUT_SERVO */

#if (CDPlayability == 1)
    if((St_cdrom_video)&&(Iram_Cmd_block.bf[0] == MMC_READ_CD) && (Iram_Rw_read_mode == eUnknownMode))
        Iram_Rw_read_mode = eMode2Form2;
#endif

    /* Initialize C1 and C2 correction */
    EccCtrlMode(eDataCD);

    switch (Iram_Rw_read_mode)
    {
        case eYellowMode2:
        case eMode2Form2:
            WRITE_FIELD(HAL_CDD_SC3ERR, 0);     /* Buffering will not be stoped on C3 EDAC error */
            break;

        case eMode1:
        case eMode2Form1:
            WRITE_FIELD(HAL_CDD_YBM2DIS, 1);    /* YBM2 auto-detection disable */
            break;

        default:
            Iram_Rw_read_mode = eUnknownMode;
            WRITE_FIELD(HAL_CDD_SC3ERR, 0);     /* Buffering will not be stoped on C3 EDAC error */
            break;
    }

    WRITE_FIELD(HAL_CDD_BUFSBC, 0);
    WRITE_FIELD(HAL_CDD_BUFSBC, 1);
    WRITE_REG(HAL_CDDREG_MAP->SUBRWC, 0);
    WRITE_FIELD(HAL_CDD_SCEN, 1);

    WRITE_FIELD(HAL_CDD_IHDRW, 1);

    if ( Iram_Rw_read_mode == eYellowMode2 )
        WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON | HAL_CDD_M2EINH) );
    else if(Iram_Rw_read_mode == eCDDAMode)
        WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON | HAL_CDD_SCDDA) );
    else
        WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON) );

    if(DecTimeCnt < SPECIAL_SPD_MAX_TIME )
        DecTimeCnt++;
}

/*************************************************************************
* StopDecoder
*
* This routine is used to turn off decoder function
*
* Returns:      void
* Params:       none
*************************************************************************/
void StopDecoder(void)
{
    vBOOL temp_IE;

    /*------------------------------------------
        If it is a DVD Disc, it have to use
        DVD Disc stop buffering function!
    -------------------------------------------*/
    ReadyLED(LED_OFF);
    if( pdbDiscLoaded() == DVD_DISC )
    {
        if( DVD_BUFFERING() == ON )
        {
            dvdStopDiscTransfer();
        }
        return;
    }
#if (BD_ENABLE == 1)
    else if (pdbDiscLoaded() == BD_DISC)
    {
        if( BD_BUFFERING() == ON )
        {
            bdStopDiscTransfer();
        }
        return;
    }
#endif  /* BD_ENABLE */

    Iram_reload = FALSE;

    WRITE_FIELD(HAL_CDD_SCEN, 0);       /* Disable subcode logic for CD decoder */
    WRITE_FIELD(HAL_CDD_SCEN, 1);       /* Enable SUBC */

    if (Rw_dec_stop == TRUE)
        return;

    temp_IE = GET_INT_ENABLE_MASK();    /* Save org. interrupt */
    DISABLE_INTERRUPT();

    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, 0);  /* Decoder disable */

    DecTimeCnt -= 4L;
    if(DecTimeCnt < 0L ){
        if(St_cdrom_data == TRUE)
        {
            DecTimeCnt  = 0L;
        }
        else if(DecTimeCnt < PLAYBACK_SPD_MIN_TIME)
        {
            DecTimeCnt  = PLAYBACK_SPD_MIN_TIME;
        }
    }

    WRITE_REG(HAL_CDDREG_MAP->SUBRWC, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);

    WRITE_REG(HAL_CDDREG_MAP->DINT0, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINT1, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINT2, 0);

  #if (USING_C1 == 1) /* Disable C1 Interrupt */
    WRITE_FIELD(HAL_CDD_C1ERREN, 0);
    WRITE_REG(HAL_CDDREG_MAP->C12CCR, (HAL_CDD_C2UNC | HAL_CDD_C1UNC) );
  #endif


    Rw_dec_stop = TRUE;                 /* Mark decoder stop */
    Rw_tblk_reach = FALSE;              /* Mark no reach tblk */
    St_run_in_out  = FALSE;
    St_audio_extract = FALSE;
    Rw_init_buffer = FALSE;
                                        /* need clr DSP interface logic? */
    SET_INT_ENABLE_MASK(temp_IE);       /* Restore org. interrupt */
}

/*************************************************************************
* InitBuffering
*
* Program Decoder for parallel buffering
*
* Returns:      void
* Params:       none
*************************************************************************/
void InitBuffering(void)
{
    vBOOL   temp_IE;
    BYTE    a;
    TimeS   msf;
    long    tlba;
    USHORT  bptr;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

#if(BACKGROUND_READ_AHEAD == 1)
    if(Rw_cdda_disc)
    {
        ResetCDDecoder();

        if (Iram_Page_Size_3K_Flag == 1)        /*Automatic change from 2.5K to 3k page for C2PO*/
        {
            if(Iram_shHDDir & HAL_HST_HMLY)
            {
                Iram_shHDDir &= ~HAL_HST_HMLY;             /* 3K Host Layout*/
                WRITE_FIELD(HAL_CDD_MLY, e_Page3K); /* Memory layout 3K */
            }
        }

        bptr = DataBuffer.CurDiscPtr;

        if (bptr == 0)
            bptr = RING_END_PAGE;
        else
            bptr --;

        WRITE_FIELD(HAL_CDD_DECPG, bptr);           /* Init decoder buffer page pointer */
        WRITE_FIELD(HAL_CDD_DSUBPG, bptr);
        Iram_Sh_wbk = DataBuffer.CurDiscPtr;

        Period_pg = 0;
        WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);

        tlba = DataBuffer.CacheEndLBA;
        ADD_LBA_HEADER_1_FRAME(tlba, 1);

        msf = LBA2MSF(tlba);

        a = HEX2BCD(msf.Minute);
        WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);       /* set target address MIN */

        a = HEX2BCD(msf.Second);
        WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);       /* set target address SEC */

        a = HEX2BCD(msf.Frame);
        WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);       /* set target address BLK */

        WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RDMISSEN) );

        if((Iram_Rw_read_mode == eCDDAMode)||(St_cdrom_audio == TRUE))
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, (HAL_CDD_DC2ERREN | HAL_CDD_DIFERREN) );
        else
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, (HAL_CDD_DC2ERREN | HAL_CDD_HDRERREN | HAL_CDD_DIFERREN) );

        WRITE_REG(HAL_CDDREG_MAP->DINTEN2, (HAL_CDD_TFNDEN | HAL_CDD_DPRDEN) );

        WRITE_FIELD(HAL_CDD_BUFSBC, 1);
        WRITE_REG(HAL_CDDREG_MAP->SUBRWC, 0);
        WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_SDIFERR) ); // during host data transfer */
        WRITE_REG(HAL_CDDREG_MAP->C2THLD, C2_THRESHOLD_CDDA1);       /* write C2 threshold register */

        fRStart = FALSE;
        fRdMiss = FALSE;

        Knl_deci_set = FALSE;
        Rw_tblk_reach = FALSE;

        Rw_ecc_error = FALSE;               /* assume no ecc error */
        Iram_RW_ECC_Status = 0x0;           /* initial ecc value */

        St_audio_extract = TRUE;            /* mark CD-DA buffering */
        Rw_dec_stop = FALSE;
        Rw_init_buffer = TRUE;

        WRITE_FIELD(HAL_CDD_AEWAEN, 1);     /* Enable auto page offset load */
        WRITE_FIELD(HAL_CDD_SCEN, 1);

        WRITE_FIELD(HAL_CDD_IHDRW, 0);
        if(Iram_Rw_read_mode == eCDDAMode)
            WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON | HAL_CDD_SCDDA) );
        else
            WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON) );

        if(DecTimeCnt < SPECIAL_SPD_MAX_TIME ){
            DecTimeCnt++;
        }
    }
    else
#endif
    {
        if (Iram_Rw_read_mode != eUnknownMode)
        {
            tlba = DataBuffer.CacheEndLBA;
            ADD_LBA_HEADER_1_FRAME(tlba, 1);

            if (St_method_2 == FALSE)
            {
                msf = LBA2MSF(tlba);
            }
            else
            {
                msf = Method2LBAToMSF(tlba);
            }

            /* select decoder mode and enable C2PO input */
            ResetCDDecoder();

            if (Iram_Page_Size_3K_Flag == 1)    /*Automatic change from 2.5K to 3k page for C2PO*/
            {
                if(Iram_shHDDir & HAL_HST_HMLY)
                {
                    Iram_shHDDir &= ~HAL_HST_HMLY;             /* 3K Host Layout*/
                    WRITE_FIELD(HAL_CDD_MLY, e_Page3K); /* Memory layout 3K */
                }
            }

            bptr = DataBuffer.CurDiscPtr;

            if (bptr == 0)
                bptr = RING_END_PAGE;
            else
                bptr --;

            WRITE_FIELD(HAL_CDD_DECPG, bptr);   /* Init decoder buffer page pointer */
            WRITE_FIELD(HAL_CDD_DSUBPG, bptr);
            Iram_Sh_wbk = DataBuffer.CurDiscPtr;

            WRITE_REG(HAL_CDDREG_MAP->C2THLD, 0x00);

#if (CEProduct == 0)	//For IT production
    /* For Nero CD/DVD speed transfer rate test:
         If the Perod_pg is 0 or 2, the transfer rate is not smooth after 40x.
         Set Period_pg to MAX_CD_DECODE_PERIOD for smooth transfer rate curve. */
            if ( HostXfer.BlocksLeft <= 2 )
            {
                Period_pg = 0;
            }
            else if( ( ( Maxlba - HostXfer.StartBlock.lba ) <= MAX_CD_DECODE_PERIOD * 2  )  || ( HostXfer.BlocksLeft  < MAX_CD_DECODE_PERIOD ) )
            {
                Period_pg = 2;
            }
            else
            {
                Period_pg = MAX_CD_DECODE_PERIOD;
            }
#else
                Period_pg = 2;
#endif

            if ((rd_method == WR_METHOD_FP) || (rd_method == WR_METHOD_VP))
            {
                Period_pg = 0;
            }

            WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);

            a = HEX2BCD(msf.Minute);
            WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);   /* set target address MIN */

            a = HEX2BCD(msf.Second);
            WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);   /* set target address SEC */

            a = HEX2BCD(msf.Frame);
            WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);   /* set target address BLK */

            Iram_Target_lba = MSF2LBA_Plus(&msf) - 1;   /* Logical*/

            Knl_deci_set   = FALSE;                 /* ignore previous decoder interrupt */
            Rw_mode_change = FALSE;                 /* mark no mode changed */
            St_run_in_out  = FALSE;                 /* mork not in link area */

            Rw_ecc_error = FALSE;                   /* assume no ecc error */
            Iram_RW_ECC_Status = 0x0;                    /* initial ecc value */

            fRStart = FALSE;
            fRdMiss = FALSE;

            Rw_tblk_reach = FALSE;                  /* unmark target block reached */
            Rw_dec_stop = FALSE;                    /* mark auto search start */

            Rw_init_buffer = TRUE;
            Rw_ecc_mode();                          /* set to Buffering ROM mode */
        }
        else
            Svr_pause_reqire = TRUE;                /* mark pause request */
    }

    SET_INT_ENABLE_MASK(temp_IE);
}


/*************************************************************************
* Rw_get_blk_mode
*
* This function will get the sector type of current xfer block
* from the dram decoder block mask area
*
* Returns:      Block mode
* Params:       none
*************************************************************************/
BYTE Rw_get_blk_mode(void)
{
    BYTE a;

    Iram_Buf_dram_addr = DataBuffer.CurHostPtr + DECBLKMASK_ADDR;
    a = ReadSDRAM();

    if(Iram_Rw_read_mode != a)
    {
        if(St_audio_extract == TRUE)
            a = eCDDAMode;
    }
    return (a);
}

/*******************************************************************************

    FUNCTION        ClearDataBuffer

    DESCRIPTION     Clears the cache information and resets the disc and
                    host pointers to the start of the ring buffer

    GLOBAL UPDATES  DataBuffer

********************************************************************************/
void ClearDataBuffer(void)
{
    /*--------------------------------------------------
        Clear the buffer count and start of cache
    ---------------------------------------------------*/
    DataBuffer.SectorCount      = 0;
    DataBuffer.CacheStartLBA    = 0L;
    DataBuffer.CacheEndLBA      = 0L;

    /*--------------------------------------------------
        Reset the current disc pointers and end of cache
    ---------------------------------------------------*/
    switch(pdbDiscLoaded())
    {
#if (BD_ENABLE)
        case BD_DISC:
            DataBuffer.CurDiscPtr = (WORD)(HAL_BD_CODEC_GET_RING_BUFFER_START()/BD_DATA_BLK_PTR_RESOLUTION);
            HAL_BD_CODEC_SetDiscPtr(DataBuffer.CurDiscPtr);
            if ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                 pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
                BDInitRingBuffer();
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case HD_DISC:
#endif
        case DVD_DISC:
            DataBuffer.CurDiscPtr = READ_FIELD(HAL_DVDD_STARTBK);
            WRITE_FIELD(HAL_DVDD_CURBUF, DataBuffer.CurDiscPtr);
            break;

        case CD_DISC:
            DataBuffer.CurDiscPtr = READ_FIELD(HAL_CDD_DSKBUFSPG);
            WRITE_FIELD(HAL_CDD_DECPG, DataBuffer.CurDiscPtr);
            break;
    }

    /*--------------------------------------------------
        Reset the current host pointer
    ---------------------------------------------------*/
    DataBuffer.CurHostPtr = READ_REG(HAL_HSTREG_MAP->HSTBUFSPGL);
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, DataBuffer.CurHostPtr);

#if (DVD_RAM_READ == 1)
    if (pdbServoGetfDiscKind() == eDVDRAMDisc)
        RAMInitRingBuffer();  //Reset dvdram cache manager variables
#endif
}

/**********************************************************************
* Module: ResetCDDecoder
*
* This routine is used to reset Sector Processor.
*
* Input:    NONE
* Output:   NONE
*
*********************************************************************/
void ResetCDDecoder(void)
{
    WRITE_FIELD(HAL_CDD_SCEN, 0);       /* Disable SUBC */

    WRITE_FIELD(HAL_CDD_DPCLR, 1);      /* Set Decoder partial clear */
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, 0);  /* Clear Decoder partial clear */

    WRITE_FIELD(HAL_CDD_SCEN, 1);       /* Enable SUBC */

    CLEAR_DECODE_INTERRUPTS();
}

/*************************************************************************
* Rw_get_remain_xfer_blk
*
* This function will get the remain blks count that will xfer to host
*
* Returns:      Remain block count
* Params:       none
*************************************************************************/
USHORT Rw_get_remain_xfer_blk(void)
{
    USHORT a;
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    WRITE_FIELD(HAL_HST_CTRSSHOT, 1);
    a = READ_FIELD(HAL_HST_HSTPGCC);    /* read valid data block flag */
    WRITE_FIELD(HAL_HST_CTRSSHOT, 0);

    a++;                                /* Adj one block back */

    SET_INT_ENABLE_MASK(temp_IE);       /* Restore all interrupt */
    return (a);
}


/*************************************************************************
* CheckForReadAheadBufferingHit
*
* This function is Ckecking "Set Read Ahead" Command buffering start.
*
* Returns:      pg_no : periodic interrupt page number
* Params:       TRUE  : Read Ahead Trriger LBA Hit
*               FALSE : Read Ahead Trriger LBA NOT reach
*************************************************************************/
BYTE CheckForReadAheadBufferingHit( USHORT pg_no )
{
    TimeS   hdrMSF;
    USHORT  iHdrAddr;
    long    curLBA;
    USHORT  adjPtr;
    ULONG   work_addr;

    /*--------------------------------------------------------------------*/
    /* Get Current Buffering LBA from Header Area                         */
    /*--------------------------------------------------------------------*/
    work_addr = PAGE_AUX_START_ADDR(pg_no);
    iHdrAddr = AUX_HEADER_ADDR1;                    /* Mode 1 */

    /* Get Current MSF */
    hdrMSF.Minute = SCSDRAM[work_addr + iHdrAddr + 0 ];
    hdrMSF.Minute = BCD2HEX( hdrMSF.Minute);
    hdrMSF.Second = SCSDRAM[work_addr + iHdrAddr + 1 ];
    hdrMSF.Second = BCD2HEX( hdrMSF.Second);
    hdrMSF.Frame  = SCSDRAM[work_addr + iHdrAddr + 2 ];
    hdrMSF.Frame = BCD2HEX( hdrMSF.Frame);

    /* Convert MSF into LBA */
    curLBA = MSF2LBA_Plus( &hdrMSF );
    /*--------------------------------------------------------------------*/
    /* Check relation between Current Buffering LBA and ahead trigger LBA */
    /* If current LBA doesn't reach trigger LBA, wait to hit trigger.     */
    /*--------------------------------------------------------------------*/
    if ( curLBA > Iram_Buf_trg_blk.lba )
    {
        /*--- Decoder stop and adjustment buffer management variables ---*/
        StopDecoder();
        adjPtr = pg_no - DataBuffer.CurDiscPtr;
        Iram_Sh_wbk = DataBuffer.CurDiscPtr;
        DataBuffer.CurDiscPtr  += adjPtr;
        Iram_Buf_ahead_bptr = DataBuffer.CurDiscPtr;            /* next address is ahead first point */

        DataBuffer.CacheEndLBA = (Iram_Buf_ahead_blk.lba - 1);/* mark lba of blk processed */
        DataBuffer.SectorCount     += (USHORT)adjPtr;               /* adjust buffer counter */

        /*--- reload start_block with dec_block and start seek again ---*/
        Iram_reload = TRUE;
        Iram_rd_ahead_chk_req = RD_AHEAD_BUF_DONE;          /* mark ahead buffering initial done */

        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

/*----------------------------------------
  ClearReadAheadSettings

  This function is reset Read Ahead.
----------------------------------------*/
void ClearReadAheadSettings( void )
{
    /* Read Ahead Setting clear */
    Iram_Buf_trg_blk.lba   = 0;
    Iram_Buf_ahead_blk.lba = 0;
    Iram_Buf_ahead_bptr    = 0;
    Iram_rd_ahead_chk_req  = RD_AHEAD_BUF_NOP;
}

/*********************************************************************
* Function:     PrepareBufferingForTrkInfo()
*
* Description:  Set Register to read data.
*
* Returns:      void
* Params:       target LBA
*********************************************************************/
extern vBYTE rd_method;
void PrepareBufferingForTrkInfo(long target_lba)
{
    BYTE a;
    USHORT bptr;
    TimeS msf;

    DISABLE_INTERRUPT();
    ResetCDDecoder();

    Iram_Sh_wbk = 0;
    ClearDataBuffer();
    bptr = DataBuffer.CurDiscPtr;
    if (bptr == 0)
        bptr = RING_END_PAGE;
    else
        bptr --;

    WRITE_FIELD(HAL_CDD_DECPG, bptr);               /* init decoder buffer page pointer */
    WRITE_FIELD(HAL_CDD_DSUBPG, bptr);              /* init sub buffer page pointer */
    WRITE_FIELD(HAL_HST_HSTBUFPG, bptr);
    WRITE_FIELD(HAL_HST_BUFPGCMP, bptr);

    WRITE_REG(HAL_CDDREG_MAP->DPRDPG, 0);           /* Priod length        */

    if (Iram_Page_Size_3K_Flag == 1)    /*Automatic change from 2.5K to 3k page for C2PO*/
    {
        if(Iram_shHDDir & HAL_HST_HMLY)
        {
            Iram_shHDDir &= ~HAL_HST_HMLY;     /* 3K Host Layout*/
            WRITE_REG (HAL_CDDREG_MAP->MEMCF, halCDD_BufPage_3KB); /* Memory layout 3K */
        }
    }

    msf = LBA2MSF(target_lba);

    a = HEX2BCD(msf.Minute);
    WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);       /* set target address MIN */

    a = HEX2BCD(msf.Second);
    WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);       /* set target address SEC */

    a = HEX2BCD(msf.Frame);
    WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);       /* set target address BLK */

    Knl_deci_set   = FALSE;                 /* ignore previous decoder interrupt */
    Rw_dec_stop    = FALSE;                 /* mark decoder running  */
    Rw_tblk_reach  = FALSE;                 /* mark no reach target blk */
    Rw_mode_change = FALSE;                 /* mark no mode changed */
    St_run_in_out  = FALSE;

    Rw_ecc_error = FALSE;                   /* assume no ecc error */
    Iram_RW_ECC_Status = 0x0;               /* initial ecc value */
    St_method_2 = FALSE;

    fRStart = FALSE;
    fRdMiss = FALSE;

    rd_method = 0xFF;
    Rw_ecc_mode();                          /* set to Buffering ROM mode */
    ENABLE_INTERRUPT();
}

/*********************************************************************
* Function:     Buffering()
*
* Description:  Read the data from disc
*
* Returns:      number of blcoks in the buffer
* Params:       target LBA, read size
*********************************************************************/
USHORT Buffering(long target_lba, USHORT length)
{
    ULONG   clock;
    ULONG   timeout,time;
    USHORT  bf_cnt;

    PrepareBufferingForTrkInfo(target_lba);

    timeout = 1500L;                    /* About 13.3 msec * 50 length */
    StartTimer(&clock);

    while (1){                          /* wait for period int */
        if (fRStart == TRUE){
            break;
        }
        if (fRdMiss == TRUE)
        {
            /* Overshoot error */
            fRdMiss = FALSE;
            break;
        }

        time = ReadTimer(&clock);
        if(time > timeout){
            break;
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            break;
        }

        ExitProcess();
    }

    while(DataBuffer.SectorCount < length){
        time = ReadTimer(&clock);
        if(time > timeout){
            break;
        }
        if(Rw_dec_stop == TRUE){
            break;
        }
        ExitProcess();
    }

    bf_cnt = DataBuffer.SectorCount;
    StopDecoder();
    ClearDataBuffer();

    if (Iram_Page_Size_3K_Flag == 1)           /*Automatic change from 2.5K to 3k page for C2PO*/
    {
        if(!(Iram_shHDDir & HAL_HST_HMLY))
        {
            Iram_shHDDir |= HAL_HST_HMLY;                      /* 2.5K Host Layout*/
            WRITE_FIELD(HAL_CDD_MLY, e_Page2_HalfK);    /* Memory layout 2.5K */
        }
    }

    return bf_cnt;
}


/*********************************************************************
* Function:     CDCheckTrackDataMode()
*
* Description:  Check and update track data mode
*
* Returns:      void
* Params:       track number
*********************************************************************/
void CDCheckTrackDataMode(BYTE tno)
{
    ULONG   work_addr;
    long    tmplba;
    USHORT  bf_cnt;
    TimeS   time;
    BYTE    cont_addr, mode, retry_cnt, temp_IE;

    cont_addr = pdbGetTrackContAddr(tno);
    cont_addr = cont_addr & 0xF0;
    {
        if(cont_addr & 0x40){
            time = pdbGetStartTime(tno);
            tmplba = MSF2LBA_Plus(&time);
            HostXfer.StartBlock.lba = tmplba;
//            Rw_seek_retry = 0;                  /* initialize seek retry count */
            Rw_seek_blk.lba = tmplba - 20L;
            Svr_pause_reqire = FALSE;
            svoIramVar.subQEnableFlag = 1;
            retry_cnt = 0;

#if (CDPlayability == 1)
        /* for VCD huge defect disc, do not buffer each track to check data mode!*/
            if(St_cdrom_video)
            {
                mode = 2;
                pdbSetTrackDataMode(tno , mode);
                Svr_pause_reqire = TRUE;
                return;
            }
#endif

            while(retry_cnt < 6){
                if(SeekToTarget() == PASS){
                    bf_cnt = Buffering( HostXfer.StartBlock.lba,10);
                    if(bf_cnt != 0){
                        break;
                    }
                }
                retry_cnt ++;
            }
		    Svr_pause_reqire = TRUE;
            if(retry_cnt >= 6)
                return;

            svoIramVar.subQEnableFlag = 0;
            temp_IE = GET_INT_ENABLE_MASK();

            work_addr = AUX_START_ADDR + bf_cnt *0x200-0x800;
            mode = SCSDRAM[ work_addr + AUX_DATAINFO_ADDR ];

            mode = (mode & 0x30) >> 4;

            if ( mode == 2 )    /* Mode 2 */
                mode = SCSDRAM[ work_addr + AUX_HEADER_ADDR2 +3 ];
            else            /* Mode 1 */
                mode = SCSDRAM[ work_addr + AUX_HEADER_ADDR1 +3 ];

            pdbSetTrackDataMode(tno , mode);
        }
    }
}

/*********************************************************************
* Function:     void CDCheckReadAhead(void)
* Description:  Check if it needs to pre-buffer.
* Returns:
* Params:
*********************************************************************/
extern BYTE CDBufferAlmostEmpty;
void CDCheckReadAhead(void)
{
    ULONG CDBUFTIMER;

    if (St_cmd_inproc != TRUE)
    {
        if(START_BACKGROUND_BUFFER)
        {
            if(DataBuffer.SectorCount < (BUF_TOTAL_BLK_COUNT - 0x68))
            {
                START_BACKGROUND_BUFFER = FALSE;
                if(DataBuffer.CacheEndLBA <= Maxlba)
                {
                    Rw_seek_blk.lba = DataBuffer.CacheEndLBA;

#if (CDPlayability == 1)
                    /* If buffer is almost empty, by pass C2 error for keeping data in buffer*/
                    if((DataBuffer.SectorCount < 26) &&(St_cdrom_audio)&&(Iram_Cmd_block.bf[0] == MMC_READ_CD))
                        CDBufferAlmostEmpty = TRUE;
#endif
                    AdjustSeekOffset();                  /* execute adjust rw_seek_blk operation */
                    if(SeekToTarget()==PASS)        /* execute seek operation */
                    {
                        InitBuffering();               /* setup decoder for parallel buffering */
                        /*
                            On CD-DA disc.
                            Seek complete and over 20ms pass, but VMSF bit is not vaild.
                            Then reset Buffering again to avoid nothing happened in CD decoder.
                        */
                        StartTimer(&CDBUFTIMER);
                        while(Rw_cdda_disc)
                        {
                            if(READ_FIELD(HAL_CDD_VMSF))
                                break;

                            if(ReadTimer(&CDBUFTIMER) > 20L) //20 msec
                            {
                                //send_msg5S(SEND_HIF_CMD,"MSF NOT VAILD");
                                send_msg80(SEND_HIF_CMD,0x710048);
                                StopDecoder();
                                InitBuffering();               /* setup decoder for parallel buffering */
                                break;
                            }

                            ExitProcess();
                        }

                    }
                    else
                    {
                        Svr_pause_reqire = TRUE;        /* mark pause request */
                    }
                }
            }
        }
    }
}
