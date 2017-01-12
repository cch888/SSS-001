/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_rdcd.c
*
* DESCRIPTION:
*   This file contains read raw r-w subcode and read cdda data routine
*
*   $Revision: 95 $
*   $Date: 11/03/16 7:14p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/* Audio Control */
#define C2_READ     1       /* C2 Detect 1:EI  0:DI */

#define TARGET_OFFSET   0   /* 1: adjust target offset if target is defective */
                            /* 0  no adjustment */

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\comMath.h"
#include ".\common\system.h"
#include ".\hif\gp_equ.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_knl.h"
#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\reg_intctrl.h"
#include ".\al\HAL_demod.h"
#include ".\oem\oem_spd_tbl.h"
#ifdef READ10_SSI  // Read10 command support on SSI port
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif
#endif

static void ReadCDDAExit(void);
static void AbortCDDATransfer(void);
void EnableCDDABuffering(void);
#if (IGNORE_ERROR_FOR_AUDIO == 1)
BYTE fLastRetry;
#define BLOCKS_TO_SKIP_FOR_AUDIO_ERROR  (int)10
#endif // (IGNORE_ERROR_FOR_AUDIO == 1)

/*********************************************************************
*? EnableCDDABuffering
*
* Entry point for read cd-da...
* Returns:      void
* Params:       none
*********************************************************************/

void EnableCDDABuffering(void)
{
    USHORT bptr;

    ResetCDDecoder();

    if (Iram_Page_Size_3K_Flag == 1) /*Automatic change from 2.5K to 3k page for C2PO*/
    {
        if(Iram_shHDDir & HAL_HST_HMLY)
        {
            Iram_shHDDir &= ~HAL_HST_HMLY;             /* 3K Host Layout*/

            WRITE_FIELD(HAL_CDD_MLY, e_Page3K); /* Memory layout 3K */
        }
    }


    /* if decoder stop which may caused by buffer full
    we needs turn on subcode control logic again */
    bptr = DataBuffer.CurDiscPtr;

    if (bptr == 0)
        bptr = RING_END_PAGE;
    else
        bptr --;

    WRITE_FIELD(HAL_CDD_DECPG, bptr);               /* Init decoder buffer page pointer */
    WRITE_FIELD(HAL_CDD_DSUBPG, bptr);              /* Init decoder subcode page pointer */
    WRITE_FIELD(HAL_HST_BUFPGCMP, bptr);            /* Init Host Buffer page compare pointer */
    WRITE_FIELD(HAL_HST_HSTBUFPG, bptr);            /* Init Host buffer page pointer */

    Period_pg = 0;
    WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);   /* One period interupt per block */

    WRITE_REG(HAL_CDDREG_MAP->C2THLD, 0x00);        /* C2 Threshold set to 0 */

    /*read CD-TEXT subcode data error*/
    //WRITE_REG(HAL_CDDREG_MAP->DINTEN1, HAL_CDD_SRWERREN);/Set this in ReadCDText() for retry
    /*read CD-TEXT subcode data error*/
    WRITE_REG(HAL_CDDREG_MAP->DINTEN2, HAL_CDD_DPRDEN);

#ifdef READ10_SSI  // Read10 command support on SSI port
    #if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
        WRITE_REG(HAL_CDDREG_MAP->BUFCTL, 0);
    #endif
#endif

    WRITE_FIELD(HAL_CDD_AEWAEN, 1);     // Enable auto page offset load
    WRITE_FIELD(HAL_CDD_SCEN, 1);       // CD-Text mode 4 data in lead-in area is buffered
    WRITE_FIELD(HAL_CDD_BUFCDT, 1);     // CD-Text mode 4 data in lead-in area is buffered

    WRITE_FIELD(HAL_CDD_IHDRW, 0);
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDBUF | HAL_CDD_SCDDA) );

    if(DecTimeCnt < SPECIAL_SPD_MAX_TIME ){
        DecTimeCnt++;
    }

    fRStart = FALSE;
    fRdMiss = FALSE;

    Knl_deci_set = FALSE;

    Rw_dec_stop = FALSE;
    St_audio_extract = TRUE;
    Rw_tblk_reach = TRUE;
    return;
}

/*********************************************************************
* ReadCDDAData
*
* Entry point for read cd-da data(2352 bytes)
* Returns:      void
* Params:       none
*********************************************************************/
BYTE st_c2detect_EI;
#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif
#if (CHIP_REV <= 0xB2)
extern BYTE UseC1CG1;
#endif

void ReadCDDAData(void)
{
    ULONG cdda_dec_timer;
    vBOOL flg;
    BYTE a,rw_seek_retry;
    LONG lba;
#if (HORROR_DISC_IMPROVEMENT == 1)
    BYTE CDReadRetryCount;
#endif
#if TARGET_OFFSET == 0
    /*  for ATIP Time and Qcode PTime different */
    SubQCodeS  SubCode;
    long int  Qcode_PTime,Diff_Time;
    TimeS  ATIP;
#endif
    TimeS Targetmsf;
    BYTE  Diff_Time_Retry;
    /*****************************    FW Bug    ******************************/
    /* The read_timer is used instead of svoIramVar.knlTimeupCount.
        The voIramVar.knlTimeupCount is reset by some called functions in this function,
        and that cases voIramVar.knlTimeupCount is incorrect for this read command.*/

    ULONG read_timer;

    /**************************** End FW Bug  2007.05.15 ********************/

#if CEProduct == 1
    BYTE RdMisCntForSpecialDisc;
    RdMisCntForSpecialDisc = 0 ;
#endif

#if TARGET_OFFSET == 1
    BYTE TargetOffset;
    long TargetLBA;
#endif

  #if (IGNORE_ERROR_FOR_AUDIO == 1)
    USHORT SkippedBlkCnt;

    fLastRetry = FALSE;
    SkippedBlkCnt = 0;
  #endif //(IGNORE_ERROR_FOR_AUDIO == 1)
    svoIramVar.subQEnableFlag = 1;    /*Enable SubQ read*/
    rw_seek_retry = 0;
    Diff_Time_Retry = 0;
#if (CHIP_REV <= 0xB2)
    UseC1CG1 = FALSE;
#endif
#if (HORROR_DISC_IMPROVEMENT == 1)
    CDReadRetryCount=0;
#endif
#if TARGET_OFFSET == 1
    TargetOffset = 0;
#endif

#if (EN_POWER_CONTROL)
    SetDiscReadPowerMode();
#endif

    StartTimer( &read_timer );

check_cdda_hit:
    /*** check cache hit ***/
    while (1)                           /* use for breaking to prepare read cdda */
    {
        if ( Rw_cdda_disc == FALSE )
        {
            break;                      /* goto prepare_read_cdda */
        }

        DISABLE_INTERRUPT();
        LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheEndLBA, lba, flg);
        if ( !flg )                     /* HostXfer.StartBlock.lba >= DataBuffer.CacheEndLBA */
        {
            if ( lba != 0 )
            {
                if ( lba >= 20 || Rw_dec_stop )
                {
                    ENABLE_INTERRUPT();
                    break;              /* goto prepare_read_cdda */
                }

                if(DataBuffer.SectorCount != 0){
                    LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheStartLBA, lba, flg);
                    DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba - 1;
                    DataBuffer.CurHostPtr += lba-1;
                    if ( DataBuffer.CurHostPtr > RING_END_PAGE )        /* adjust bptr of next page */
                        DataBuffer.CurHostPtr -= (RING_END_PAGE + 1);
                DataBuffer.SectorCount = 0;
                }

                Iram_Knl_timer_counter = 5;
                Knl_timeout = FALSE;
                Knl_deci_set = FALSE;
                ENABLE_INTERRUPT();

                goto wait_cdda_int;
            }
        }
        /* chk_cdda_hit: start_blk < dec_blk */
        ENABLE_INTERRUPT();

        if ( DataBuffer.SectorCount == 0)
            break;                      /* go to prepare cdda read */

        /* calculate if first request block in inside buffer
        calculation start block - xfred block */
        /* if start_block - xfred block < 0  then cache miss */
        LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheStartLBA, lba, flg);

        if (( flg ) && (DataBuffer.CacheStartLBA < 0x80000000)) /*protect DataBuffer.CacheStartLBA < 0 case*/
        {
            DataBuffer.SectorCount = 0;
            break;                      /* go to prepare cdda read if start < xfred */
        }

        /* start_block-xfred block >=0 cases
        check start_block-xfred block=0
        if yes, it is cache miss */
        if ( lba > RING_END_PAGE)
             break;                     /* go to prepare cdda read if start < xfred */

        if (lba == 0)
            break;                      /* go to prepare cdda read if start < xfred */

        /* cdda_cache_frm_hit */
        lba--;

        if (lba != 0)
        {
            /* cdda_adj_buffer_point: */
            if (DataBuffer.SectorCount < lba )
                break;                  /* go to prepare cdda read if start < xfred */

            DISABLE_INTERRUPT();

            DataBuffer.CurHostPtr += (USHORT) lba;
            if (DataBuffer.CurHostPtr > RING_END_PAGE)
            {
                DataBuffer.CurHostPtr %=BUF_TOTAL_BLK_COUNT;
            }
            DataBuffer.SectorCount -= (USHORT) lba;
        }

        /* cdda_adj_xfer_to_start: */
        ENABLE_INTERRUPT();

        if ( DataBuffer.SectorCount == 0)
            break;                      /* go to prepare cdda read */

        DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
        MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, 1);

        SendHostBlocks();
        Rw_rd_monitor = TRUE;

        goto wait_cdda_finish;
    }

    /*** cache not hit, prepare read cdda data ***/
    while (1)
    {
        Rw_cdda_disc = TRUE;                        /* post reading cdda data */
        Rw_seek_blk.lba = HostXfer.StartBlock.lba;  /* initial seek block */

/*** seek to target location ***/
seek_oper:
    if(ReadTimer( &read_timer ) > TIME_6S)
    {
        Svr_pause_reqire = TRUE;
        BUILD_SENSE(0x03,0x02,0x11,0x03);    /* UNRECOVERED READ ERROR */
        AbortCDDATransfer();
        return;
    }
#if CEProduct == 1
/* To protect the CD audio can not play the 1st song, the CD disc contain error data in the 00:02.00.
    Once the retry count is out and the back end will give up the play operation.
    This protect will change the start lba to the place where can be buffered before back end terminated this command.
    And the play operation can work normally.
    Remark: For CE use only. */

if ((RdMisCntForSpecialDisc == 2)&&(HostXfer.StartBlock.lba < 26)&&(St_cdrom_audio)&&(Iram_Cmd_block.bf[9] ==0x10)){
        RdMisCntForSpecialDisc = 0;
        HostXfer.StartBlock.lba = 26;
        Rw_seek_blk.lba = HostXfer.StartBlock.lba;  /* change the seek block location*/
        MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba, 15);
}
#endif

#if (CDPlayability == 1)
    /* If buffer is almost empty, by pass C2 error for keeping data in buffer*/
    if ((DataBuffer.SectorCount < 26) &&(St_cdrom_audio))
        CDBufferAlmostEmpty = TRUE;
#endif
   #if (HORROR_DISC_IMPROVEMENT == 1)
      CDReadRetryCount++;

        if(CDReadRetryCount>=1)//from 720
        {
            (void)ServoChgRFParameter();//change RF EQ parameter //chung
        }
    #endif
        do
        {
          #if (WITHOUT_SERVO == 0)
            if ( SeekToTarget() == FAIL)
            {
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down

                /* seek fail, increment retry counter by 1
                   if not exceed retry limit, reseek again
                   if exceed retry limit, abort this command */

                if (ReadTimer(&read_timer) > TIME_6S)
                {
                  #if (IGNORE_ERROR_FOR_AUDIO == 0)
                /* for Nero read lead-out */
                    if((long) HostXfer.StartBlock.lba >= (long)(Maxlba+1))  /* lead-out start lba */
                    {
                        DataBuffer.SectorCount = HostXfer.BlocksLeft;
                        goto target_reached;
                    }

                    Svr_pause_reqire = TRUE;            /* mark pause request */
                    BUILD_SENSE(0x03,0x02,0x00,0x1d);        /* NO SEEK COMPLETE */
                    ReadCDDAExit();
                    return;
                  #else // #if (IGNORE_ERROR_FOR_AUDIO == 0)

                    if (HostXfer.BlocksLeft < BLOCKS_TO_SKIP_FOR_AUDIO_ERROR)
                    {
                        HostXfer.StartBlock.lba += (long)HostXfer.BlocksLeft ; /* Seek to next block */
                        SkippedBlkCnt += HostXfer.BlocksLeft;
                    }
                    else
                    {
                        HostXfer.StartBlock.lba += (long)BLOCKS_TO_SKIP_FOR_AUDIO_ERROR; /* Seek to next block */
                        SkippedBlkCnt += BLOCKS_TO_SKIP_FOR_AUDIO_ERROR;
                    }

                    Rw_seek_blk.lba = HostXfer.StartBlock.lba;      /* initial seek block */
                    MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba, 15);
                    if ( HostXfer.BlocksLeft <= SkippedBlkCnt )
                    {
                        Iram_Rw_read_mode = eCDDAMode;              /* set data mode to 5 */
                        DISABLE_INTERRUPT();

                        Iram_Sh_wbk = 0;
                        ClearDataBuffer();

                        DataBuffer.CurDiscPtr += SkippedBlkCnt;     /* Adjust buffer pointer */
                        DataBuffer.SectorCount += SkippedBlkCnt;        /* Adjust buffer counter */

                        ENABLE_INTERRUPT();
                        SendHostBlocks();
                        Rw_rd_monitor = TRUE;                   /* mark monitor read */

                        /*** wait for cmd finish ***/
                        while (1)
                        {
                            ExitProcess();

                            if(EjectKeyInfo.PressedValid)
                            {
                                /* cdda_key_in: */
                                /* eject key is pushed, terminate this command and exit to
                                   process eject */
                                BUILD_SENSE(0x02,0x3A,0x00,0x09);    /* MEDIUM NOT PRESENT */
                                AbortCDDATransfer();
                                return;
                            }

                            if ( St_send_blkdata == TRUE )
                            {
                                continue;
                            }

                            if ( St_cmd_inproc == FALSE )
                            {
                                ReadCDDAExit();
                                return;
                            }
                        }   /* end of while */
                    }
                  #endif    //#if (IGNORE_ERROR_FOR_AUDIO == 0)
                }
                if(EjectKeyInfo.PressedValid)
                {
                    BUILD_SENSE(0x02,0x3A,0x00,0x0a);    /* MEDIUM NOT PRESENT */
                    ReadCDDAExit();
                    return;
                }
            }
            else
          #endif    // WITHOUT_SERVO
            {
                break;
            }
        } while (1);

        /* seek successfully completed */
        Iram_Rw_read_mode = eCDDAMode;              /* set data mode to 5 */
//      DataBuffer.SectorCount = 0;                     /* mark no block avail */
        /* process CDDA data transfer using SUBCODE SYNC for synchronization */
        goto subc_cdda_wr_mode;

/**** wait for decoder int ***/
wait_cdda_int:
        while (1)
        {
            if ( Knl_deci_set == TRUE)
            {
                Knl_deci_set = FALSE;
                break;                          /*  decoder int. occurred */
            }

            if ( Rw_dec_stop == TRUE )
                goto seek_oper;

            if ( Knl_timeout == TRUE )
            {
                Knl_timeout = FALSE;
                /* We are wait about 100ms and decoder interrupt never occurs.
                   Retry maximum for 5 times. If after retry, decoder interrupt still not
                   happened, set up error code and terminate this command. */

                Svr_pause_reqire = TRUE;            /* mark pause request */
                if (ReadTimer(&read_timer) > TIME_6S)
                {
                    /* cdda_int_err: */
                    BUILD_SENSE(0x05,0x24,0x00,0x40);    /* INVALID FIELD IN COMMAND PACKET */
                    AbortCDDATransfer();
                    return;
                }

                goto seek_oper;
            }

            if(EjectKeyInfo.PressedValid)
            {
                /* cdda_key_in: */
                /* eject key is pushed, terminate this command and exit to
                    process eject */

                BUILD_SENSE(0x02,0x3A,0x00,0x0b);    /* MEDIUM NOT PRESENT */

                AbortCDDATransfer();
                return;
            }
        }

        /* decoder_int_occurred:*/
        /* first cd-da block is successfully loaded into fifo
        we can xfer it to host now */

        DISABLE_INTERRUPT();

        /* check target blk with respect to decoder block*/

        if (HostXfer.StartBlock.lba > DataBuffer.CacheEndLBA)   /* if not reach target */
        {
        /* decoder block is less than target block. wait for next dec int */
            /* buf_cdda_not_reached:*/
            Iram_Knl_timer_counter = 5;
            Knl_timeout = FALSE;
            ENABLE_INTERRUPT();
            if ( Rw_dec_stop == TRUE )
                goto seek_oper;
            else
                goto wait_cdda_int;                         /* wait next decoder int */
        }
        else if (HostXfer.StartBlock.lba < DataBuffer.CacheStartLBA) /* if overshoot error */
        {
            /* buf_cdda_overshoot: */
            ENABLE_INTERRUPT();
            goto seek_oper;
        }

/*** send to Host ***/
target_reached:
        ENABLE_INTERRUPT();
        SendHostBlocks();

/*** wait for cmd finish ***/
wait_cdda_finish:
        Rw_rd_monitor = TRUE;           /* mark monitor read */
        Iram_Knl_timer_counter = 40;    /* Wait about 800 ms */
        Knl_timeout = FALSE;            /* Assume no time out */

        StartTimer(&cdda_dec_timer); /*Start timer for CDDA DECODE TIMER*/
        while (1)
        {
            ExitProcess();

            if(EjectKeyInfo.PressedValid)
            {
                /* cdda_key_in: */
                /* eject key is pushed, terminate this command and exit to
                   process eject */
                BUILD_SENSE(0x02,0x3A,0x00,0x0c);    /* MEDIUM NOT PRESENT */
                AbortCDDATransfer();
                return;
            }

            /*1 sec timer protect for wait cdda finish hangup Issue */
            if(ReadTimer(&cdda_dec_timer) > 500L)   /* 500 msec potect*/
            {
                if(GetSpeedValue(Current_read_sp) == GetSpeedValue(MIN_READ_SPEED))
                {
                    /* Add Timer protect  to  avoid dead loop occured*/
                    BUILD_SENSE(0x05,0x24,0x00,0xC1);
                    AbortCDDATransfer();
                    return;
                }
                else
                {
                    SpeedCount = (CD_SPEED_DOWN_READY*10);
                    Set_SpeedMDCont();
                    //StartTimer(&cdda_dec_timer); /*Start timer for CDDA DECODE TIMER*/
                    //continue;
                    goto check_cdda_hit;
                }
            }

            if ( St_send_blkdata == TRUE )
            {
                continue;
            }

            if ( St_cmd_inproc == FALSE )
            {
                ReadCDDAExit();
            #if(BACKGROUND_READ_AHEAD == 1)
                if(Rw_dec_stop) {
                    START_BACKGROUND_BUFFER = TRUE;
                }
            #endif
                return;
            }

            if(Rw_init_buffer)
            {
                if ( Knl_timeout == TRUE )
                {
                    ResetCDDecoder();
                    break;
                }
            }

            if ( Rw_dec_stop == TRUE )
            {
                break;
            }

        }   /* end of while */

        /* set_reseek: */
        /* if decoder stop and command not finished, we need to
           reseek to the last xfered block and restart decoder
           sequence */

        HostXfer.StartBlock.lba = DataBuffer.CacheStartLBA;
        ADD_LBA_HEADER_1_FRAME(HostXfer.StartBlock.lba, 1);

        /* goto prepare_read_data */
    }/* end of while */

/*********************************************************************
* subc_cdda_wr_mode
*
* This routine is used to process CDDA data transfer using SUBCODE
* syncronization
*
* Returns:  void
* Params:   none
*********************************************************************/
subc_cdda_wr_mode:
    Iram_Knl_timer_counter = 150;           // Wait about 3 s
    Knl_timeout = FALSE;                    // Assume no time out

    while(1)    // checking for bad SCOR
    {
        if (HAL_DEMOD_GetFePllLocked())   // If both inner and outer frequency lock
            break;                         //  then the Sync is good.

        // Sync was not good, now reset logic. TBD    see also issue 4586

                                      // TBD   Reset Demodulator. Do not use Demodulator Soft Reset. This would reset all Demodulator register to initial values.
                                      //       In the case of CD Maybe one could use CDVDMDCFG.CDVFRCOLK to
                                      //        force lock detection to "out of lock". But then this maybe useful for test only.
                                      //       Once this section of code has been tested and is working please remove duplicate lines within compile conditional (CHIP == 6230 else block)
        pdbSetSubQIntStatus(FALSE);

        while((pdbGetSubQIntStatus() != TRUE) && (Knl_timeout == FALSE))
        {
            if(ReadTimer( &read_timer ) > TIME_6S)
            {
                Svr_pause_reqire = TRUE;
                BUILD_SENSE(0x03,0x02,0x11,0x04);    /* UNRECOVERED READ ERROR */
                AbortCDDATransfer();
                return;
            }
            ExitProcess();
        }

        if(Knl_timeout == TRUE)             // if time out
        {
            Knl_timeout = FALSE;
            break;
        }
    }

    DISABLE_INTERRUPT();

    Iram_Sh_wbk = 0;
    ClearDataBuffer();

  #if (IGNORE_ERROR_FOR_AUDIO == 1)
    DataBuffer.CurDiscPtr += SkippedBlkCnt;     /* Adjust buffer pointer */
    DataBuffer.SectorCount += SkippedBlkCnt;        /* Adjust buffer counter */
  #endif //#if (IGNORE_ERROR_FOR_AUDIO == 1)

    ResetCDDecoder();

    if (Iram_Page_Size_3K_Flag == 1)     /*Automatic change from 2.5K to 3k page for C2PO*/
    {
        if(Iram_shHDDir & HAL_HST_HMLY)
        {
            Iram_shHDDir &= ~HAL_HST_HMLY;             /* 3K Host Layout*/
            WRITE_FIELD(HAL_CDD_MLY, e_Page3K); /* Memory layout 3K */
        }
    }


    {
        USHORT bptr;

        bptr = DataBuffer.CurDiscPtr;

        if (bptr == 0)
            bptr = RING_END_PAGE;
        else
            bptr--;

        WRITE_FIELD(HAL_CDD_DECPG, bptr);       /* Init decoder buffer page pointer */
        WRITE_FIELD(HAL_CDD_DSUBPG, bptr);      /* Init Decoder Subcode page pointer */
        WRITE_FIELD(HAL_HST_BUFPGCMP, bptr);    /* Init Buffer Page Compare pointer */
        WRITE_FIELD(HAL_HST_HSTBUFPG, bptr);    /* Init Host buffer page pointer */
    }

    /* improve Nero disc quality test */
    if ( HostXfer.BlocksLeft <= 2 ) {
    Period_pg = 0;
    }else {
        Period_pg = 2;
    }

    WRITE_REG(HAL_CDDREG_MAP->DPRDPG, Period_pg);

    {

#if TARGET_OFFSET == 1
        TargetLBA = HostXfer.StartBlock.lba - TargetOffset;
        Targetmsf = LBA2MSF(TargetLBA);
#else
        Targetmsf = LBA2MSF(HostXfer.StartBlock.lba);
#endif

        a = HEX2BCD(Targetmsf.Minute);
        WRITE_REG(HAL_CDDREG_MAP->THEAD0, a);   /* set target address MIN */

        a = HEX2BCD(Targetmsf.Second);
        WRITE_REG(HAL_CDDREG_MAP->THEAD1, a);   /* set target address SEC */

        a = HEX2BCD(Targetmsf.Frame);
        WRITE_REG(HAL_CDDREG_MAP->THEAD2, a);   /* set target address BLK */
    }

#if TARGET_OFFSET == 1
    DataBuffer.CacheEndLBA = TargetLBA;
#else
    DataBuffer.CacheEndLBA = HostXfer.StartBlock.lba;
#endif

    MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheEndLBA, 1);
    DataBuffer.CacheStartLBA = DataBuffer.CacheEndLBA;

    WRITE_REG(HAL_CDDREG_MAP->DINTEN0, (HAL_CDD_RDMISSEN) );

    #if (C2_READ == 1)    /* C2 Detect 1:EI */
    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, (HAL_CDD_DC2ERREN | HAL_CDD_DIFERREN) );
    #else /* C2_READ */
    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, HAL_CDD_DIFERREN);
    #endif /* C2_READ */

    WRITE_REG(HAL_CDDREG_MAP->DINTEN2, (HAL_CDD_TFNDEN | HAL_CDD_DPRDEN) );

    WRITE_FIELD(HAL_CDD_BUFSBC, 1);
    WRITE_REG(HAL_CDDREG_MAP->SUBRWC, 0);
    WRITE_REG(HAL_CDDREG_MAP->BUFCTL, (HAL_CDD_SDIFERR) );     // during host data transfer */
    WRITE_REG(HAL_CDDREG_MAP->C2THLD, C2_THRESHOLD_CDDA1);       /* write C2 threshold register */

    if(GetSpeedValue(Current_read_sp) == GetSpeedValue(MIN_READ_SPEED))
    {
        rw_seek_retry++;
        if ((rw_seek_retry == 3)||(st_c2detect_EI == FALSE))
        {
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
            st_c2detect_EI = FALSE;
        }
    }else{
        st_c2detect_EI = TRUE;
    }

    fRStart = FALSE;
    fRdMiss = FALSE;

    Knl_deci_set = FALSE;
    Rw_tblk_reach = FALSE;

    Rw_rd_monitor = FALSE;                  /* unmark monitor write */

    Rw_ecc_error = FALSE;                   /* assume no ecc error */
    Iram_RW_ECC_Status = 0x0;               /* initial ecc value */

    St_audio_extract = TRUE;                /* mark CD-DA buffering */
    Rw_dec_stop = FALSE;

    WRITE_FIELD(HAL_CDD_AEWAEN, 1);         /* Enable auto page offset load */
    WRITE_FIELD(HAL_CDD_SCEN, 1);           /* Enable SUBC */

    /* 3 symbol C2 correction. */
    EccCtrlMode(eAudioCD);

    WRITE_REG(HAL_CDDREG_MAP->CONCTL, 0x00);

    WRITE_FIELD(HAL_CDD_IHDRW, 0);
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, (HAL_CDD_DECEN | HAL_CDD_SDMON | HAL_CDD_SCDDA) );

    if(DecTimeCnt < SPECIAL_SPD_MAX_TIME ){
        DecTimeCnt++;
    }

    Iram_Knl_timer_counter = 5;     /* Wait about 100 ms */
    Knl_timeout = FALSE;            /* Assume no time out */

    ENABLE_INTERRUPT();

    while (1)                       /* wait for next decoder int */
    {
        if (fRStart == TRUE)
            break;

        if (fRdMiss == TRUE)
        {

#if CEProduct == 1
           RdMisCntForSpecialDisc++;
#endif

            /* Overshoot error */
            fRdMiss = FALSE;

            if (ReadTimer(&read_timer) > TIME_6S)
            {
                /* for Nero readd lead-out */
                if((long) HostXfer.StartBlock.lba >= (long)(Maxlba+1))  /* lead-out start lba */
                {
                    DataBuffer.SectorCount = HostXfer.BlocksLeft;
                    goto target_reached;
                }

                /* cdda_int_err: */
                BUILD_SENSE(0x05,0x24,0x00,0x41);    /* INVALID FIELD IN CDB */
                AbortCDDATransfer();
                return;
            }

#if TARGET_OFFSET == 1
            TargetOffset++;
            Rw_seek_blk.lba -= TargetOffset;
#else
            /*for ATIP Time and Qcode PTime different*/
            if( Iram_Diff_Time_Flag == TRUE ){
                pdbGetSubCode(&SubCode);
                if(SubCode.Addr == eSubcodeMode1)
                {
                    ATIP.Minute = SubCode.PMinute;
                    ATIP.Second = SubCode.PSecond;
                    ATIP.Frame  = SubCode.PFrame ;
                    ATIP = BCD2MSF(&ATIP);

                    Qcode_PTime = (ATIP.Minute*4500) + (ATIP.Second*75) + ATIP.Frame -150;
                    Diff_Time = Qcode_PTime - Rw_seek_blk.lba;
                }
                else
                {
                    Diff_Time = 9*Diff_Time_Retry;
                }
                Rw_seek_blk.lba = HostXfer.StartBlock.lba - (Diff_Time + 16);
                if(Rw_seek_blk.lba<0)
                    Rw_seek_blk.lba = 0;
            }
            else
            {
                Diff_Time_Retry++;
                if(Diff_Time_Retry == 5)
                    Iram_Diff_Time_Flag = TRUE;
                Rw_seek_blk.lba = HostXfer.StartBlock.lba - (9*Diff_Time_Retry);
                if(Rw_seek_blk.lba<0)
                    Rw_seek_blk.lba = 0;
            }
             //Rw_seek_blk.lba = HostXfer.StartBlock.lba - 9;
            /*for ATIP Time and Qcode PTime different*/
#endif
            goto seek_oper;
        }

        if ( Rw_dec_stop == TRUE )
        {
#if TARGET_OFFSET == 1
          // The C2 error occurred, we should retry
            TargetOffset++;
            Rw_seek_blk.lba -= TargetOffset;
#else
            Rw_seek_blk.lba = HostXfer.StartBlock.lba - 9;
#endif
            goto seek_oper;
        }

#if (WITHOUT_SERVO == 0)
        if ( Knl_timeout == TRUE )
        {
            /* We are wait about 400ms and decoder int never occurs.
               Set up error code and terminate this command now. */
            Knl_timeout = FALSE;

            ResetCDDecoder();

            if (ReadTimer(&read_timer) > TIME_6S)
            {
                /* cdda_int_err: */
                Svr_pause_reqire = TRUE;        /* mark pause request */
                BUILD_SENSE(0x05,0x24,0x00,0x42);    /* INVALID FIELD IN CDB */
                AbortCDDATransfer();
                return;
            }

#if TARGET_OFFSET == 1
            TargetOffset++;
            Rw_seek_blk.lba -= TargetOffset;
#else
            Rw_seek_blk.lba = HostXfer.StartBlock.lba - 9;
#endif
            goto seek_oper;
        }
#endif  // WITHOUT_SERVO

        if(EjectKeyInfo.PressedValid)
        {
            BUILD_SENSE(0x02,0x3A,0x00,0x0d);        /* MEDIUM NOT PRESENT */
            AbortCDDATransfer();
            return;
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            goto seek_oper;
        }

            /* if decoder int never occurs and current MSF >= Target MSF,
               reset CD decoder and reseek without offset. */
        if((svoIramVar.currentTime.Time.Minute >= Targetmsf.Minute)&&(svoIramVar.currentTime.Time.Second>=Targetmsf.Second)&&(svoIramVar.currentTime.Time.Frame>=Targetmsf.Frame))
        {
            ResetCDDecoder();
            send_msg80(SEND_HIF_CMD,0x7100FC);
            if(READ_FIELD(HAL_CDD_VMSF))
            {
                send_msg5S(SEND_HIF_CMD,"VMSF!!");//for debug
            }
            if (ReadTimer(&read_timer) > TIME_6S)
            {
                /* cdda_int_err: */
                Svr_pause_reqire = TRUE;        /* mark pause request */
                BUILD_SENSE(0x05,0x24,0x00,0x42);    /* INVALID FIELD IN CDB */
                AbortCDDATransfer();
                return;
            }
            goto seek_oper;
        }
        ExitProcess();
    }


    DISABLE_INTERRUPT();

    Iram_Knl_timer_counter = 5;             /* Wait about 100 ms */
    Knl_timeout = FALSE;                    /* Assume no time out */

    ENABLE_INTERRUPT();

    while (1)                               /* wait for next decoder int */
    {
        if ( Knl_deci_set == TRUE ){
            Knl_deci_set = FALSE;
            Iram_Knl_timer_counter = 5;     /* Wait about 100 ms */
            Knl_timeout = FALSE;            /* Assume no time out */

#if TARGET_OFFSET == 1
            if (DataBuffer.SectorCount > TargetOffset)  /* Wait till all offset blocks and target block are bufferred */
            {
                break;
            }
#else
            if (DataBuffer.SectorCount >= 1) /* Wait till all offset blocks and target block are bufferred */
            {
                break;
            }
#endif
        }

        if ( Rw_dec_stop == TRUE )
        {
#if TARGET_OFFSET == 1
          // The C2 error occurred, we should retry
            TargetOffset++;
            Rw_seek_blk.lba -= TargetOffset;
#else
            Rw_seek_blk.lba = HostXfer.StartBlock.lba - 9;
#endif
            goto seek_oper;
        }

#if (WITHOUT_SERVO == 0)
        if ( Knl_timeout == TRUE )
        {
            /* We are wait about 100ms and decoder int never occurs.
               Set up error code and terminate this command now. */
            Knl_timeout = FALSE;
            if (ReadTimer(&read_timer) > TIME_6S)
            {
              #if (IGNORE_ERROR_FOR_AUDIO == 0)
                /* cdda_int_err: */
                Svr_pause_reqire = TRUE;        /* mark pause request */
                BUILD_SENSE(0x05,0x24,0x00,0x43);    /* INVALID FIELD IN CDB */
                AbortCDDATransfer();
                return;
              #else //#if (IGNORE_ERROR_FOR_AUDIO == 0)
                fLastRetry = TRUE;
              #endif //#if (IGNORE_ERROR_FOR_AUDIO == 0)
            }

#if TARGET_OFFSET == 1
          // The C2 error occurred, we should retry
            TargetOffset++;
            Rw_seek_blk.lba -= TargetOffset;
#else
            Rw_seek_blk.lba = HostXfer.StartBlock.lba - 9;
#endif
            goto seek_oper;
        }
#endif  // WITHOUT_SERVO
        if(EjectKeyInfo.PressedValid)
        {
            BUILD_SENSE(0x02,0x3A,0x00,0x0e);        /* MEDIUM NOT PRESENT */
            AbortCDDATransfer();
            return;
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            svoIramVar.emergencyStopFlag = TRUE;
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            //ServoStop();
            goto seek_oper;
        }

        ExitProcess();
    }

#if TARGET_OFFSET == 1
    /* Adjsut the buffer point to the correct page */
    DISABLE_INTERRUPT();
    DataBuffer.SectorCount -= TargetOffset;
    DataBuffer.CurHostPtr += TargetOffset;
    DataBuffer.CurHostPtr %=BUF_TOTAL_BLK_COUNT;

    ADD_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, TargetOffset);
    TargetOffset = 0;           //Target buffered, clear the offset
    ENABLE_INTERRUPT();
#endif

    goto target_reached;
} /* end of read_cdda_data */

/**********************************************************************/

static void AbortCDDATransfer(void)
{
    /* entry point for terminate command */
    StopCDDADecoder();
    AbortHostTransfer();
    St_send_blkdata = FALSE;
    ReadCDDAExit();
    return;
}

/**********************************************************************/
static void ReadCDDAExit(void)
{
    /* Entry point for exit read_cdda_data command */
    Rw_C2PO_data = FALSE;                /* mark no read c2po data */
    Rw_subc_data = FALSE;
    Read_subc_only = FALSE; /* read subcode only */
    Rw_rdcd_cmd = FALSE;

#if (CDPlayability == 1)
    CDBufferAlmostEmpty = FALSE;
#endif

    ENABLE_INTERRUPT(); // speed up audio transfer rate test

  #if (LED_NUM == 1)
    ReadyLED(LED_OFF);
  #endif /* (LED_NUM == 1) */

  #if (LED_NUM == 2)
    ReadyLED(LED_AXferOn);
  #endif /* LED_NUM == 2 */

    AutoCommandComplete();
    Rw_rd_monitor = FALSE;
}

/*********************************************************************
* StopCDDADecoder
*
* This routine is used to turn off decoder function for CDDA
*
*********************************************************************/
void StopCDDADecoder(void)
{
    StopDecoder();
}

#if (ENABLE_CDG == 1)
/*
 *----------------------- CD-G ----------------------------------------
 */

extern void ReadCDExit(void);

/**********************************************************************
* ReadNxAudioSubCode
*
* Process the Read_CD command to send R-W Subcode data to host while
* nx_1x audio is playing.
*
**********************************************************************/
static void ReadNxAudioSubCode()
{
    Rw_rd_monitor = FALSE;              /* unmark read monitor */

    while (1)
    {
        if ( DataBuffer.SectorCount > 0 )      /* subcode buffer is not empty */
        {
            DISABLE_INTERRUPT();        /* critical zone, disable int. */
            if ( ( St_send_blkdata == FALSE ) && /* no block data is xfring */
                 ( St_cmd_inproc == TRUE ) )      /* cmd not finish */
            {
                SendHostBlocks();       /* xfr blk to host */
                Rw_rd_monitor = TRUE;   /* mark monitor read */
            }
            ENABLE_INTERRUPT();

            while (1)
            {
                if (( St_cmd_inproc == FALSE ) || ( St_audio_complete == TRUE ))
                /* cmd is finished */
                {
                    ReadyLED(LED_ON);       /* keep led on */
                    Rw_subc_data = FALSE;   /* unmark read subcode data */
                    Rw_rd_monitor = FALSE;  /* unmark read monitor */
                    ENABLE_INTERRUPT();
                    AutoCommandComplete();
                    return;
                }

                if ( St_send_blkdata == TRUE )   /* xfr is in process */
                    continue;

                if ( (DataBuffer.SectorCount > 0) && (St_cmd_inproc == TRUE) )
                    break;

                ExitProcess();

                if ( EjectKeyInfo.PressedValid )
                {
                    BUILD_SENSE(0x02,0x3A,00,0x0f);  /* MEDIUM NOT PRESENT */
                    StopDecoder();     /* stop decoder */
                    AbortHostTransfer();     /* abort xfer */
                    AutoCommandComplete();
                    return;
                }
            }
        }

        if  ( (St_cmd_inproc == FALSE) || (St_audio_complete == TRUE) )
        /* cmd is finished */
        {
            ReadyLED(LED_ON);           /* keep led on */
            Rw_subc_data = FALSE;       /* unmark read subcode data */
            Rw_rd_monitor = FALSE;      /* unmark read monitor */
            ENABLE_INTERRUPT();
            AutoCommandComplete();
            return;
        }

        ExitProcess();
    }
}

/*********************************************************************
* ReadAudioSubCode
*
* Entry point for read raw r-w subcode during audio playing
* It is used for CD-G application
*
* Returns:      void
* Params:       none
*********************************************************************/
void ReadAudioSubCode(void)
{
    if ( HostXfer.BlocksLeft == 0 )         /* if xfer length is zero */
    {
        ReadCDExit();
        return;
    }

    ReadyLED(LED_FLASH);

    if (St_nx_audio == TRUE)
    {
        ReadNxAudioSubCode();
        return;
    }
}

/*----------------------------------CD-G----------------------------*/
#endif /* ENABLE_CDG */
