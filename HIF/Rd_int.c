/******************************************************************************
*                 (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILENAME        rd_int1.c
*
*   DESCRIPTION     This file contains the interrupt-1 service routines for
*                   the decoder interrupt and data transfer done interrupt
*   $Revision: 157 $
*   $Date: 11/03/31 2:43p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_int.h"
#include ".\hif\rd_int.h"
#include ".\hif\at_knl.h"
#include ".\hif\app_if.h"
#include ".\hif\gp_mdsns.h"
#include ".\servo\Svo_com.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvmacros.h"
#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

#if (HORROR_DISC_IMPROVEMENT == 1)
#include ".\al\REG_dfe.h"
#include ".\servo\svoVar.h"
#endif

extern vBOOL Min_residue_xfr_allowed(void);
extern void IncATAPIByteCount(void);
extern LONG pdbGetEndTimeOfLastCloseSession(void);
extern BYTE GetSCRegField_1B(USHORT, BYTE, BYTE);
extern void SetSCRegField_1B(ULONG, BYTE, BYTE, BYTE);
static vBOOL ProcessCDDecoderPages(void);
static void BlockModeDetect(USHORT);

extern USHORT   timeUp_Group1;
/*  51 - TFND
    55 - EDC
    5A - OVERRUN
    53 - READMISS
    54 - MSB
    56 - HDC2
    58 - C2
*/

#if (IGNORE_ERROR_FOR_AUDIO == 1)
extern BYTE fLastRetry;
#endif //(IGNORE_ERROR_FOR_AUDIO == 1)

#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif
#if (CHIP_REV <= 0xB2)
extern BYTE UseC1CG1;
#endif
#if (BypassDecoderEr == 1)
extern BYTE IGNORE_DECODER_ERROR;
#endif
#if (CD_DFE_IMPROVE_READ == 1)
extern  BYTE    DFE_CHG_SEL;
extern  BYTE    DFE_CHG_CASE;
#endif
/*********************************************************************
* CDDecoderInt1()
*
* Moved here from HIF_IM.c to save space
*********************************************************************/
void CDDecoderInt1(BYTE);
void CDDecoderInt1(BYTE DSts0)
{
    if (DSts0 & HAL_CDD_RO1ERR) /* runout 1 missed error */
    {
        RunOut1MissedIntProc();
    }

    if (DSts0 & HAL_CDD_RI4ERR) /* Run-in 4 missed error */
    {
        RunIn4MissedIntProc();
    }

    if (DSts0 & HAL_CDD_RDMISS) /* Read Miss Interrupt */
    {
        ReadMissIntProc();
    }

    if (DSts0 & HAL_CDD_BFULL) /* Buffer full Interrupt */
    {
        OverRunIntProc();
    }

    if (DSts0 & HAL_CDD_MSFCERR)      /* MSF continuity error */
    {
        MSBErrorIntProc();
    }

    if (DSts0 & HAL_CDD_EDACERR)  /* ECC or EDC error, buffer full, link area block interrupt */
    {
        EDCNoGoodIntProc();
    }
}

/*********************************************************************
* CDDecoderInt2()
*
* Moved here from HIF_IM.c to save code space.
*********************************************************************/
void CDDecoderInt2(BYTE DSts1)
{
    if (DSts1 & HAL_CDD_HDRERR)/* header error */
    {
        HeaderErrorIntProc();
    }

    if (DSts1 & HAL_CDD_SRWERR)/* Subcode R-W ECC error */
    {
        SRWErrorIntProc();
    }

    if (DSts1 & HAL_CDD_DC2ERR)/* Data C2 error */
    {
        C2ErrorIntProc();
    }

    if (DSts1 & HAL_CDD_DIFERR)/* DSP-Data decoder error */
    {
        CDReadFailIntProc();
    }

    if (DSts1 & HAL_CDD_NOSYNC)  /* No CD-ROM sync error */
    {
        NoCDRomSyncIntProc();
    }
}

#if GETPACKETCOMMAND_IN_FLASH == 1
/*********************************************************************
* GetPacketCommand()
*
* Get 12 command packet bytes from ATAPIData FIFO and store all these
* bytes to command array
*********************************************************************/
extern void GetEventStatusNotificationCommand(void);
void GetPacketCommand()
{
    BYTE a;
    BYTE    wr_method;
    FLAG    FastCacheCheck;

    ataCheckDMA();                          /* check if host request DMA */
    /* read 1f4/1f5 ATA Byte Count regs:max data can be xfer in every drq */
    Iram_Xfr_allow = Iram_shATB = READ_REG(HAL_HSTREG_MAP->ATBLO) & 0xFFFE;

    /* get 12 bytes packet command */
    for (a=0; a<12; a++)                    /* get 12 bytes packet command */
    {
        Iram_Cmd_block.bf[a] = READ_REG(HAL_HSTREG_MAP->ATAPIDATA);
    }

    send_atapi_cmd(SEND_ATAPI_CMD);

    StartTimer(&Iram_atapi_timer);

    if(ataPowerMode == SLEEP_MODE){
        WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x10);  /* set DRDY, DSC */
        ATAClearBusy();
        return;
    }

    /*For doing offline power calibration command do not check Unit Attention to execute this vendor command*/
#if (SUN_FUNC_TEST == 1)
    if(((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) || (Iram_Cmd_block.bf[0] == DIAG_LASER)) && (St_unit_atten == TRUE))
#else
    if((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) && (St_unit_atten == TRUE))
#endif        
        St_unit_atten = FALSE;

    /* check if Write command is received during writing data  "MDF-1" */
    if ( (Iram_Cmd_block.bf[0] == MMC_WRITE_10) || (Iram_Cmd_block.bf[0] == MMC_WRITE_12))
    {
        if (Iram_RecordState == eRECORDING)
        {
            if (svoIramVar.focusDropFlag == FALSE)
            {
                if (!(synccache_process_num > 0 || Iram_close_process_num > 0 || ForceStopWriting == TRUE))
                {
                    if (READ_FIELD(HAL_WS_WGATEMON))
                    {
                        wr_method = GetWriteMethod();                               /* write method of write parameter */
                        if (wr_method != FPKT)
                        {
                            SetRecordPauseAddress();
                        }
                    }
                    else
                    {
                        if(GetWPM_Byte(2) & 0x10){                      /* Test write ? */
                            if(Iram_buff_valid_blocks < 80L)
                            {
                                Iram_RecordState = eRECORD_UNDERRUN;
                                WriteUnderRun = TRUE;
                            }
                        }
                    }

                }
            }

            if (READ_FIELD(HAL_WS_WGATEMON)||(GetWPM_Byte(2) & 0x10))
            {
                EntryInterceptProcess(pHIF);
            }
        }
    }

    if( Iram_Cmd_block.bf[0] == MMC_MECHANISM_STATUS)
    {
        MechanismStatusCommand();
        return;
    }

    if ( (Iram_Cmd_block.bf[0] == SBC_VERIFY_10) || (Iram_Cmd_block.bf[0] == SBC_VERIFY_12) )
    {
        Iram_Xfr_allow = 0xFFFE;
        Iram_shATB    = 0xFFFE;
    }

    if( Iram_Cmd_block.bf[0] != 0x00 || Iram_Cmd_block.bf[0] != 0x03)
    {
        a = READ_REG(HAL_HSTREG_MAP->ATAPIDATA);
    }

    /* Inquiry command */
    if( Iram_Cmd_block.bf[0] == SPC_INQUIRY)
    {
        InquiryCommand();
        return;
    }
    else if ( Iram_Cmd_block.bf[0] == MMC_GET_EVENT_STATUS_NOTIFICATION )
    {
        GetEventStatusNotificationCommand();
        return;
    }
    else if( Iram_Cmd_block.bf[0] == MMC_GET_CONFIGURATION)
    {
        GetConfigurationCommand();
        return;
    }
    /* Set Read Ahead operation Check */
    if ( (Iram_rd_ahead_chk_req == RD_AHEAD_BUF_REQ) || (Iram_rd_ahead_chk_req == RD_AHEAD_BUF_DONE) )
    {
        if ( (Iram_Cmd_block.bf[0] == SBC_READ_10) || (Iram_Cmd_block.bf[0] == SBC_READ_12) ||
             (Iram_Cmd_block.bf[0] == MMC_READ_CD) || (Iram_Cmd_block.bf[0] == MMC_READ_CD_MSF) )
            {
                CheckForReadAheadBufferingStart();
            }
    }

    /* If any disc that contains defect management, do not run Fast Cache Check */
    FastCacheCheck = TRUE;
#if(BD_ENABLE == 1)
    if (pdbDiscLoaded() == BD_DISC)
    {
        if (pdbGetBDType() == BD_R || pdbGetBDType() == BD_RE)
            FastCacheCheck = FALSE;
    }
    else
#endif
    if (pdbDiscLoaded() == DVD_DISC)
    {
        #if (DVD_RAM_READ == 1)
        if ( pdbGetDVDType() == DVD_RAM )
            FastCacheCheck = FALSE;
        #endif
    }

    if (FastCacheCheck)
    {
        if( CheckForFastCache() == TRUE)   return;
    }

    /* If it is seek, clear DSC & DRQ bits in Status register to
       indicates seek not completed yet. */
    if ( Iram_Cmd_block.bf[0] == SBC_SEEK )
        WRITE_REG(HAL_HSTREG_MAP->ATCMD, HAL_HST_DRDYW);  /* [627h] status reg, clear DSC*/
    else
        WRITE_REG(HAL_HSTREG_MAP->ATCMD, (HAL_HST_DRDYW|HAL_HST_DSCW)); /* [627h] status reg, Set DSC*/

    Knl_cmd_avail = TRUE;                       /* Indicate new command is available. */
}
#endif

#pragma arm section code = "MARK_BLKMODE"
#if RWMARKBLKMODE_IN_FLASH == 1
/*************************************************************************
* Rw_mark_blk_mode
*
* This function will save the data modes of several blks
* to the dram decoder block mask area
*
* Returns:      void
* Params:       PageCnt - number of blocks
* Pre-condition: Interrupt disabled, PageCnt is not equal to 0.
*                Iram_Sh_wbk points to the start page.
*************************************************************************/
//extern BYTE   Iram_AddBufferingFlg;
extern vBYTE leadout_cnt;

void Rw_mark_blk_mode(USHORT PageCnt)
{
    USHORT i, j;
    USHORT ptrq;
    BYTE  subq[10],q,tno;
    BYTE  tmpLOcnt;
    ULONG work_addr;
    TimeS MSF;

    tmpLOcnt = leadout_cnt;

    for (i = 0; i< PageCnt; i++)
    {
        ptrq = Iram_Sh_wbk+i;                   /* Calculate Current Block Address*/
        work_addr = PAGE_AUX_START_ADDR(ptrq);  /* locate starting address */

        tno = 0;
        subq[0] = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR]; /* read Ctrl Addr */

        if((subq[0] & 0x0F) == 0x01) {
            tno = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR+1];  /* read trk number */

            for (q = 1; q < 7; q++)
            {
                subq[q] = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q];
            }

            MSF = LBA2MSF((DataBuffer.CacheEndLBA -(PageCnt - 1) + i));/*for correct pmin, psec, pframe */

            subq[7] = HEX2BCD(MSF.Minute);
            subq[8] = HEX2BCD(MSF.Second);
            subq[9] = HEX2BCD(MSF.Frame);

            for (q = 0; q < 10; q++)
                 SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q] = subq[q];
        }
#if (SUN_FUNC_TEST == 1)
        /* copy C2PAR to AUX offset 0x9FC */
        if(Iram_Page_Size_3K_Flag == 1)
        {
            SCSDRAM[work_addr + 0x9FC] = SCSDRAM[work_addr + AUX_DATAINFO_C2PAR_ADDR];
            SCSDRAM[work_addr + 0x9FD] = 0;
        }
#endif
        if (tno == 0xAA)
            tmpLOcnt++;
        else
            tmpLOcnt = 0;

        /* for Nx Play Audio frist Buffring   (svoIramVar.subQEnableFlag = 1)*/
        if ( Iram_AddBufferingFlg )
        {
            /* check value ptrq=0 */
            if(ptrq == 0)
                ptrq= RING_END_PAGE;
            else
            ptrq--;

            /* locate previous address for ReWrite SubQ */
            work_addr = PAGE_AUX_START_ADDR(ptrq);

            if ( (subq[0] & 0x0F) != 0x01 )
            {
                RequestSubQInfo();
                for ( q = 0; q < 10; q++ )
                {
                    SCSDRAM[work_addr +AUX_SUBQDATA_ADDR + q] = Iram_Player_output[q];
                }
            }
            else
            {
                for ( q = 0; q < 10; q++ )
                    SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q] = subq[q];
            }
            Iram_AddBufferingFlg = FALSE;
        }
    }

    leadout_cnt = tmpLOcnt;

    j = Iram_Sh_wbk;
    Iram_Buf_dram_addr = Iram_Sh_wbk + DECBLKMASK_ADDR;

    for (i = 0; i< PageCnt; i++)
    {
        WriteSDRAM(Iram_Rw_read_mode);

        j++;
        if ( j > RING_END_PAGE)
        {
            j -= (RING_END_PAGE + 1);
            Iram_Buf_dram_addr = j + DECBLKMASK_ADDR;
        }
    }
}
#else
//extern BYTE   Iram_AddBufferingFlg;
extern vBYTE leadout_cnt;

void Rw_mark_blk_mode(USHORT PageCnt)
{
    USHORT i, j;
    USHORT ptrq;
    BYTE  subq[10],q,tno;
    BYTE  tmpLOcnt;
    ULONG work_addr;
    TimeS MSF;

    tmpLOcnt = leadout_cnt;

    for (i = 0; i< PageCnt; i++)
    {
        ptrq = Iram_Sh_wbk+i;                   /* Calculate Current Block Address*/
        work_addr = PAGE_AUX_START_ADDR(ptrq);  /* locate starting address */

        tno = 0;
        subq[0] = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR]; /* read Ctrl Addr */

        if((subq[0] & 0x0F) == 0x01) {
            tno = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR+1];  /* read trk number */

            for (q = 1; q < 7; q++)
            {
                subq[q] = SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q];
            }

            MSF = LBA2MSF((DataBuffer.CacheEndLBA -(PageCnt - 1) + i));/*for correct pmin, psec, pframe */

            subq[7] = HEX2BCD(MSF.Minute);
            subq[8] = HEX2BCD(MSF.Second);
            subq[9] = HEX2BCD(MSF.Frame);

            for (q = 0; q < 10; q++)
            {
                 SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q] = subq[q];
            }
        }
#if (SUN_FUNC_TEST == 1)
        /* copy C2PAR to AUX offset 0x9FC */
        if(Iram_Page_Size_3K_Flag == 1)
        {
            SCSDRAM[work_addr + 0x9FC] = SCSDRAM[work_addr + AUX_DATAINFO_C2PAR_ADDR];
            SCSDRAM[work_addr + 0x9FD] = 0;
        }
#endif
        if (tno == 0xAA)
            tmpLOcnt++;
        else
            tmpLOcnt = 0;

        /* for Nx Play Audio frist Buffring   (svoIramVar.subQEnableFlag = 1)*/
        if ( Iram_AddBufferingFlg )
        {
            /* check value ptrq=0 */
            if(ptrq == 0)
                ptrq= RING_END_PAGE;
            else
            ptrq--;

            /* locate previous address for ReWrite SubQ */
            work_addr = PAGE_AUX_START_ADDR(ptrq);

            if ( (subq[0] & 0x0F) != 0x01 )
            {
                RequestSubQInfo();
                for ( q = 0; q < 10; q++ )
                {
                    SCSDRAM[work_addr +AUX_SUBQDATA_ADDR + q] = Iram_Player_output[q];
                }
            }
            else
            {
                for ( q = 0; q < 10; q++ )
                    SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q] = subq[q];
            }
            Iram_AddBufferingFlg = FALSE;
        }
    }

    leadout_cnt = tmpLOcnt;

    j = Iram_Sh_wbk;
    Iram_Buf_dram_addr = Iram_Sh_wbk + DECBLKMASK_ADDR;

    for (i = 0; i< PageCnt; i++)
    {
        WriteSDRAM(Iram_Rw_read_mode);

        j++;
        if ( j > RING_END_PAGE)
        {
            j -= (RING_END_PAGE + 1);
            Iram_Buf_dram_addr = j + DECBLKMASK_ADDR;
        }
    }
}
#endif
#pragma arm section code


/*********************************************************************
* *CDDecoderIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* Period intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/

void CDDecoderIntProc(void)
{
    if(Rw_tblk_reach == FALSE){         /* if Buffering not start */
        return;
    }

    Knl_deci_set = TRUE;                /* set decode interrupt indicator */

    if(ProcessCDDecoderPages() == TRUE){
        Rw_ecc_error = FALSE;           /* assume no ecc error */

#if(BACKGROUND_READ_AHEAD == 1)
        if (Rw_init_buffer == TRUE) {
            Rw_init_buffer = FALSE;
        }
#endif
    }
    else
    {
        send_msg80(SEND_ATAPI_CMD,0x710054);
    }
}

/*********************************************************************
* ProcessCDDecoderPages()
*
*   1. Calculate the number of pages processed by SP since previous
*      Period Intr. or calling;
*   2. Detect the modes of these pages and mark them into dram;
*   3. Adjust the lba and buffer counter;
*   4. Send these blks to host if necessary.
*
* Returns:  FALSE - zero page processed
*           TRUE  - non-zero page(s) processed
* Params:   none
*
*********************************************************************/
extern BYTE Iram_yell_cnt;
//extern BYTE pdbGetServoState(void);
vBYTE leadout_cnt;

LONG pdbGetEndTimeOfLastCloseSession(void);
static vBOOL ProcessCDDecoderPages(void)
{
    SHORT   pg_cnt;
    BYTE    a;
    vBOOL   flg;
    USHORT  bptr;
    BYTE    aheadHitFlg;

    flg = TRUE;

    if (Read_subc_only == TRUE)
    {
        bptr = READ_FIELD(HAL_CDD_DSUBPG);
        BHIGH(bptr) &= 0x1F;                        /* mask unused bits */
    }
    else
    {
        bptr = READ_REG(HAL_CDDREG_MAP->LDECPGL);
        BHIGH(bptr) &= 0x1F;                        /* mask unused bits */
        if ( ++bptr > Iram_usiRingEP )              /* adjust bptr of next page */
            bptr = bptr - (Iram_usiRingEP + 1) + Iram_usiRingSP;
    }

    /* caculate the # of pages processed by SP after prev. Period intr. */

    pg_cnt = (SHORT) bptr - (SHORT) DataBuffer.CurDiscPtr;

    if ( pg_cnt < 0)
        pg_cnt += ((Iram_usiRingEP + 1) - Iram_usiRingSP);

    if (pg_cnt > 0)
    {
        if ( St_audio_extract == TRUE )         /* if CD-DA buffering */
        {
            /* improve Nero disc quality test */
            if(pg_cnt > (READ_REG(HAL_CDDREG_MAP->DPRDPG)+2))
            {
                StopDecoder();
                Svr_pause_reqire = TRUE;    /* pause required */
                return (FALSE);
            }
            Iram_Sh_wbk = DataBuffer.CurDiscPtr;
            DataBuffer.CurDiscPtr = bptr;

            Iram_Rw_read_mode = eCDDAMode;      /* set data mode */
            DataBuffer.CacheEndLBA += (long) pg_cnt;    /* mark lba of blk processed */
            Rw_mark_blk_mode((USHORT)pg_cnt);   /* mark mode to DRAM */

            if(pdbGetEndTimeOfLastCloseSession() <= DataBuffer.CacheEndLBA)
            {
                if (leadout_cnt > 20)
                {
                    StopDecoder();
                    Svr_pause_reqire = TRUE;    /* pause required */
                }
            }

            if (St_nx_audio)
            {
                Iram_Audbuf_blkcnt+= (USHORT) pg_cnt;

                if ( Iram_Audbuf_blkcnt >= (BUF_TOTAL_BLK_COUNT - 0x25) )
                {
                    StopDecoder();
                    Svr_pause_reqire = TRUE;    /* pause required */
                }
            }
            else
            {
                DataBuffer.SectorCount += (USHORT) pg_cnt;  /* adjust buffer counter */
                if ( DataBuffer.SectorCount >= ((Iram_usiRingEP + 1) - 0x20) )
                {
                    StopDecoder();
                    Svr_pause_reqire = TRUE;    /* pause required */
                }
            }

        }
        else    /* if CD-ROM buffering */
        {
            BYTE mode;
            USHORT pg;
            SHORT i;

            pg = DataBuffer.CurDiscPtr;                 /* save the start page addr */

            /* Video CD Mode Detect */
            if (St_cdrom_video)
                BlockModeDetect(pg);

            if(READ_FIELD(HAL_CDD_AUTOEM)== 0x00)//20090311_c.24 == fixed SONY Mix mode CD test.
            {
                mode = READ_FIELD(HAL_CDD_MODE);

                if(mode == 0)
                {
                    if(Iram_Rw_read_mode != eMode1)
                        Iram_Rw_read_mode = eUnknownMode;
                }
                else
                {
                    if(Iram_Rw_read_mode == eMode1)
                        Iram_Rw_read_mode = eUnknownMode;
                }
            }

            if (Iram_Rw_read_mode == eUnknownMode)  /* detect mode */
            {
                aheadHitFlg = FALSE;
                BlockModeDetect(pg);            /* identify current blk's mode */

                if (pg_cnt > 1)                 /* if next blk is buffered */
                {
                    a = Iram_Rw_read_mode;          /* save the curr. blk's mode */

                    if ( ++pg > Iram_usiRingEP )            /* adjust bptr of next page */
                        pg -= (Iram_usiRingEP + 1);

                    BlockModeDetect(pg);        /* identify next blk's mode */

                    if (a != Iram_Rw_read_mode) /* if next blk's mode is different */
                    {
                        Iram_Rw_read_mode = a;  /* use current blk's mode */
                    }
                }

                if ((Iram_Rw_read_mode == eYellowMode2) && (Iram_yell_cnt<5))
                {
                    Iram_yell_cnt++;
                    Iram_Rw_read_mode = eUnknownMode;   /* use current blk's mode */
                }
                else
                {
                    Iram_yell_cnt=0;
                }

                i = 0;                              /* mark this blk(s) invalid */
                StopDecoder();
                Svr_pause_reqire = TRUE;
                Iram_Svr_player_mode = PAUSING_MODE;
            }
            else
            {
                for (i = 0; i < pg_cnt; i++)
                {
                    a = Iram_Rw_read_mode;          /* save the prev. block's mode */
                    BlockModeDetect(pg);            /* mark current blk's mode */

                    if ((a != Iram_Rw_read_mode))
                    {
#if (CEProduct == 0)
                        if((READ_FIELD(HAL_CDD_AUTOEM)== 0x00)||(Iram_Rw_read_mode==eUnknownMode))//20090311_c.24 == fixed SONY Mix mode CD test.
#endif
                        {
                            if((long)(DataBuffer.CacheEndLBA + i) >= Maxlba)   /* for read data < 0:2:0 */
                            {
                                Iram_Rw_read_mode = a;
                                bptr = pg;                  /* save the ptr of blk */
                            }else{
                                Rw_mode_change = TRUE;      /* mark mode changed */
                                bptr = pg;                  /* save the ptr of blk */
                            }

                            StopDecoder();             /* stop decoder */
                            Svr_pause_reqire = TRUE;
                            Iram_Svr_player_mode = PAUSING_MODE;
                            break;
                        }
                    }
                    aheadHitFlg = FALSE;
                    if ( Iram_rd_ahead_chk_req == RD_AHEAD_BUF_CHK )
                    {
                        aheadHitFlg = CheckForReadAheadBufferingHit( pg );
                        if ( aheadHitFlg == TRUE )
                        {
                            break;
                        }
                    }

                    if ( ++pg > Iram_usiRingEP )            /* adjust bptr of next page */
                        pg -= (Iram_usiRingEP + 1);
                }
            }

            if ( aheadHitFlg == FALSE )
            {
                Iram_Sh_wbk = DataBuffer.CurDiscPtr;
                DataBuffer.CurDiscPtr = bptr;

                if ( i == 0 )
                {
                    Rw_ecc_error = TRUE;
                    flg = FALSE;
                }
                else
                {
#if (CHIP_REV <= 0xB2)
                    UseC1CG1 = FALSE;
#endif
                    DataBuffer.CacheEndLBA += (long) i; /* mark lba of blk processed */
                    DataBuffer.SectorCount += (USHORT) i;       /* adjust buffer counter */
#if(SUN_FUNC_TEST == 1)
                    if(FTInfo.DiableCDBufferFullStopDecoding == FALSE)
#endif                    
                    if ( DataBuffer.SectorCount >= ((Iram_usiRingEP + 1) - 0x25) )
                    {
                        StopDecoder();
                        Svr_pause_reqire = TRUE;    /* pause required */
                    }
                }
            }
        }

        if( pdbGetServoState() == svoStPausing){
            StopDecoder();                         /* stop decoder */
            Svr_pause_reqire = TRUE;
            Iram_Svr_player_mode = PAUSING_MODE;
        }

    }
    else    // pg_cnt = 0
        flg = FALSE;    /* indicates no blk in buffer since prev. call*/

    if((SpeedCount < SPEED_UP_COUNT_LIMIT - Period_pg)&&(!Iram_RW_ECC_Status))//cmc
    {
        if(SpeedCount >= 0)
        {
            if(Period_pg)
                SpeedCount += Period_pg;
            else
                SpeedCount++;
        }
        else
            SpeedCount = Period_pg;
    }

    if(SpeedUpReady > CD_SPEED_UP_READY)
        SpeedUpReady--;

    /* check if host remained block is equal to zero
       If equal to zero, it means xfer completely done
       If equal not zero, it needs to check xfer   */
    if (( St_nx_audio == FALSE ) && (HostXfer.BlocksLeft !=0 ))
    {
        /* Command not finished yet
           Check if there is any avail data in buffer */
        if (DataBuffer.SectorCount != 0)
        {
          #if (APPEND_RD_RVD==1)
            {
                /* There is data in buffer which is available to xfer to host */
                // Host appendable transfer count only works in DMA or UDMA mode
                if ( (St_ultra_dma ||  St_dma_mode)
                        && (Rw_rd_monitor && St_send_blkdata && St_cmd_inproc))
                {
                    /* host is waiting for this block,xfer it now */
                     if(HostXfer.BlocksLeft >= (ULONG)DataBuffer.SectorCount)
                     {
                         if(DataBuffer.SectorCount>0x0FF)
                         {
                            Iram_Buf_hxfer_blk=0x0FF;/* Load maxmum pages allowable to the register */
                         }
                         else
                         {
                            Iram_Buf_hxfer_blk=DataBuffer.SectorCount;/* Load new available pages to the register */
                         }
                     }
                     else if(HostXfer.BlocksLeft >0x0FF)   //HostXfer.BlocksLeft < DataBuffer.SectorCount
                     {
                         Iram_Buf_hxfer_blk=0x0FF;   /* Load maxmum pages allowable to the register */
                     }
                     else                             //if(HostXfer.BlocksLeft >0)
                     {
                         Iram_Buf_hxfer_blk=(USHORT)HostXfer.BlocksLeft;/* Load the rest new available pages to the register */

                     }

                     WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk);/* Load the rest new available pages to the register */
                     DataBuffer.SectorCount -= Iram_Buf_hxfer_blk;
                     HostXfer.BlocksLeft -= (ULONG)Iram_Buf_hxfer_blk;

                     /* update xfer_block=xfer_block+buf_hxfer_blk to indicate data xfered done */
                     DataBuffer.CacheStartLBA += Iram_Buf_hxfer_blk;

                     /* update buf_hst_bptr=buf_hst_bptr+buf_hxfer_blk block address to indicate data xfered done
                       update buf_blkcnt=buf_blkcnt-buf_hxfer_blk to indicate data xfered done */

                     DataBuffer.CurHostPtr += Iram_Buf_hxfer_blk;
                     DataBuffer.CurHostPtr %=(Iram_usiRingEP + 1);

                }
                else if ( Rw_rd_monitor && !St_send_blkdata && St_cmd_inproc)
                {
                    SendHostBlocks();
                }
            }
#else // #if (APPEND_RD_RVD==1)
            {
                /* There is data in buffer which is available to xfer to host */
                if ( Rw_rd_monitor && !St_send_blkdata && St_cmd_inproc)
                {
                    /* host is waiting for this block,xfer it now */
                    SendHostBlocks();
                }

            }
#endif // #if (APPEND_RD_RVD==1)
        } /*if (DataBuffer.SectorCount != 0)*/
    } /*if (( St_nx_audio == FALSE ) && (HostXfer.BlocksLeft !=0 ))*/

#if (BY_PASS_C2Error_For_VCD)
    if (St_cdrom_audio)
        C2ErrorCount = 0;
#endif

    return(flg);
}

/*********************************************************************
* BlockModeDetect()
*
* Detect the mode of the block specified by pg_no, and save it into
* the DRAM.
*
* Returns:  Iram_Rw_read_mode
* Params:   pg_no - page pointer
*********************************************************************/
struct block_mode_data{
    BYTE file;
    BYTE channel;
    BYTE submode;
    BYTE coding;
};

//TimeS Link;                   /* for Rout1's header */
//BYTE Link_Mode;               /* for Rout1's mode   */
extern vULONG Iram_Target_lba;
extern vULONG Iram_Cur_lba;
extern vULONG Iram_startlba_ck;
extern vULONG Iram_packetsize_ck;
static void BlockModeDetect(USHORT pg_no)
{
struct block_mode_data mode_data;
    SHORT iHeaderAddr;
    BYTE  a,i,q,subq[10];
    ULONG temp_lba, work_addr;
    TimeS Cur_msf;

    work_addr = PAGE_AUX_START_ADDR(pg_no);

    if (Rw_subc_data == TRUE)
    {
        subq[0] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR];
        for (q = 1; q < 10; q++)
        {
            subq[q] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR +q];
        }
        for (q = 0; q < 10; q++)
             SCSDRAM[work_addr +AUX_SUBQDATA_ADDR +q] = subq[q];
    }

    iHeaderAddr = AUX_HEADER_ADDR1;

    Cur_msf.Minute = SCSDRAM[work_addr + iHeaderAddr + 0 ];
    Cur_msf.Second = SCSDRAM[work_addr + iHeaderAddr + 1 ];
    Cur_msf.Frame = SCSDRAM[work_addr + iHeaderAddr + 2 ];
    a= SCSDRAM[work_addr + iHeaderAddr + 3 ];               /* Read mode BYTE */
#if (SUN_FUNC_TEST == 1)
    /* copy C2PAR to AUX offset 0x9FC */
    if(Iram_Page_Size_3K_Flag == 1)
    {
        SCSDRAM[work_addr + 0x9FC] = SCSDRAM[work_addr + AUX_DATAINFO_C2PAR_ADDR];
        SCSDRAM[work_addr + 0x9FD] = 0;
    }
    /* copy C2PAR to AUX offset 0x9FC */
#endif
    switch (a) {
        case 0x02:
            for(i = 0;i < 4;i++){               /* file channel,sub-header channel,sub-header submode,sub-header coding read */
                ((BYTE *)&mode_data)[i] = SCSDRAM[pg_no*0x800 + i];
            }
            {
            BYTE  flg;
                flg = FALSE;
                for(i = 0;i < 4;i++){
                    a = SCSDRAM[pg_no*0x800 + 4 + i];
                    if(a != ((BYTE *)&mode_data)[i]){
                        Iram_Rw_read_mode = eYellowMode2;
                        flg = TRUE;
                        break;
                    }
                }
                if (flg)
                    break;
            }


            /* subheader_equal */
            if((mode_data.file    == mode_data.channel) &&
               (mode_data.submode == mode_data.coding ) &&
               (mode_data.file    == mode_data.submode)){
            /* All four bytes subheaders has one single value.Check the CRCOK bit.
            If crc is o.k., means this sector has EDC and should be XA mode 2 form 1 */
                if((Iram_RW_ECC_Status & EDCERROR) == EDCERROR ){
                    /* Sometimes M2F1 block with C3 error misdetected as YBM2 */
                    if (READ_FIELD(HAL_CDD_YBM2) == 0)
                    {
                            Iram_Rw_read_mode = eMode2Form1;
                    }
                    else
                    Iram_Rw_read_mode = eYellowMode2;
                }else{
                    Iram_Rw_read_mode = eMode2Form1;
                }
                break;
            }

            if( TEST_BIT(mode_data.submode, 5) ){
                Iram_Rw_read_mode = eMode2Form2;
            }else{
                Iram_Rw_read_mode = eMode2Form1;
            }
            break;

        case 0x01:
            Iram_Rw_read_mode = eMode1;
            break;

        default:
            /* for mode change */
            //if ((SCSDRAM[work_addr + AUX_SUBQDATA_ADDR] & 0x40) == 0x00)    //Audio data
            if(St_cdrom_audio)//Set Audio data For Pure Audio
                Iram_Rw_read_mode = eCDDAMode;
            else
                /* use previous rw_read_mode */
                Iram_Rw_read_mode = eUnknownMode;
            Iram_RW_ECC_Status |= MODERROR;          /* mark as mode error */
            break;
    }

    if ((St_method_2 == TRUE) && (Iram_Rw_read_mode != eUnknownMode)){
        if (( a & 0xE0 ) > 0){                  /* if in link area */
            Iram_Rw_read_mode = eUnknownMode;
            Iram_RW_ECC_Status |= MODERROR;          /* mark as mode error */
        }else{
            Cur_msf = BCD2MSF(&Cur_msf);
            Iram_Cur_lba = MSF2LBA_Plus(&Cur_msf);
            temp_lba = Iram_Cur_lba - Iram_Target_lba;
            Iram_Target_lba = Iram_Cur_lba;

            if(temp_lba != 1)
            {
                if((temp_lba != 8) || ((Iram_Cur_lba-Iram_startlba_ck)%(Iram_packetsize_ck+7) != 0)){
                    Iram_Rw_read_mode = eUnknownMode;
                    Iram_RW_ECC_Status |= MODERROR;  /* mark as mode error */
                }
            }
        }
    }
#if (BY_PASS_C2Error_For_VCD)
    if ((Iram_Rw_read_mode != eMode2Form2)
        &&(St_cdrom_video) && (HostXfer.StartBlock.lba > 0x4500L))
    //VCD, address is over than 1 min, and disc is no good.
    {
        Iram_Rw_read_mode = eMode2Form2;
        SCSDRAM[pg_no*0x800 + 2] = 0x62; //VCD
        SCSDRAM[pg_no*0x800 + 6] = 0x62; //VCD
    }
#endif
    SCSDRAM[DECBLKMASK_ADDR + pg_no] = Iram_Rw_read_mode;
}

/*********************************************************************
* *EDCNoGoodIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* EDCNG intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
#if (HORROR_DISC_IMPROVEMENT == 1)
extern WORD DMD_value;  // ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22)
extern BYTE DMD_CNT;    //  change reason modify for DQA Disc to retry DFE parameter(modify f.22)
WORD DMDERACNT,Jitter;  // modify for DQA Disc to retry DFE parameter

ULONG CD_EDCError_LBA0 = 0xffffffff;

extern BYTE    FixC2NOC1FLG;
extern BYTE    curC2pSec[2];
#define FixC2_Margine  25
#define FixC2_X        3

#endif//HORROR_DISC_IMPROVEMENT

void EDCNoGoodIntProc(void)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    ULONG   edc_start_lba;
#endif

    send_msg80(SEND_ATAPI_CMD,0x710055);


    //== start ==  modify for DQA Disc to retry DFE parameter
#if (HORROR_DISC_IMPROVEMENT == 1)
    DMDERACNT=READ_FIELD(HAL_DMOD_DMDERACNT);
    DMD_CNT=0;
    DMDERACNT=READ_FIELD(HAL_DMOD_DMDERACNT)+ DMD_value;
    Jitter=READ_FIELD(HAL_DFE_FPLLJIT);

#endif
    //== end == modify for DQA Disc to retry DFE parameter



    if((Iram_Rw_read_mode != eMode1)&&(Iram_Rw_read_mode != eMode2Form1))
        send_msg80(SEND_ATAPI_CMD,0x710056);   //This message indicate : set wrong decode mode


    if((St_cdrom_video != TRUE)&& (Rw_tblk_reach == TRUE ))
    {
        Iram_RW_ECC_Status |= EDCERROR;
        switch (Iram_Rw_read_mode) {
            case eMode1:
            case eMode2Form1:
                ProcessCDDecoderPages();
                Knl_deci_set = TRUE;        /* set decode interrupt indicator */
                Rw_ecc_error = TRUE;
                RecoverDecoderError();
                StopDecoder();
#if (HORROR_DISC_IMPROVEMENT == 1)
                //***************************************************
                if (svoSpeedVar.SpdInRange == FALSE)
                {
                    //Skip Learning!
//                    FixC2NOC1FLG = 0xFF;
//                    FixC2NOC1FLG = 0xFF;
                }
                else if(FixC2NOC1FLG == 0xFF)
                {
                    edc_start_lba = HostXfer.StartBlock.lba;
                    if(edc_start_lba != CD_EDCError_LBA0)
                    {
                        read_retry-=1;
                        curC2pSec[0] = curC2pSec[1] = 0;
                        CD_EDCError_LBA0 = edc_start_lba;
                        curC2pSec[READ_FIELD(HAL_CDD_C2NOC1FLG)]=READ_FIELD(HAL_CDD_C2CNT);
                        WRITE_FIELD(HAL_CDD_C2NOC1FLG,~READ_FIELD(HAL_CDD_C2NOC1FLG));
                    }
                    else
                    {
                        curC2pSec[READ_FIELD(HAL_CDD_C2NOC1FLG)]=READ_FIELD(HAL_CDD_C2CNT);
                        SendMsgCnB(SHOW_DEBUG_MSG,2,0x800A18,B1B(curC2pSec[0]),B1B(curC2pSec[1]));
                        if(((curC2pSec[0]+FixC2_Margine) < curC2pSec[1])||((curC2pSec[0]*FixC2_X) < curC2pSec[1]) )
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x800A19);
                            CD_EDCError_LBA0 = 0xffffffff;
                            FixC2NOC1FLG = 0;
                            WRITE_FIELD(HAL_CDD_C2NOC1FLG,FixC2NOC1FLG);
                        }
                        else if(((curC2pSec[1]+FixC2_Margine) < curC2pSec[0])||((curC2pSec[1]*FixC2_X) < curC2pSec[0]) )
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x800A1A);
                            CD_EDCError_LBA0 = 0xffffffff;
                            FixC2NOC1FLG = 1;
                            WRITE_FIELD(HAL_CDD_C2NOC1FLG,FixC2NOC1FLG);
                        }
                        else
                        {

                                WRITE_FIELD(HAL_CDD_C2NOC1FLG,~READ_FIELD(HAL_CDD_C2NOC1FLG));
                        }

                    }
                }
                else
                {

                        WRITE_FIELD(HAL_CDD_C2NOC1FLG,FixC2NOC1FLG);


                }
#endif//(HORROR_DISC_IMPROVEMENT == 1)
                Svr_pause_reqire = TRUE;
                Iram_Svr_player_mode = PAUSING_MODE;
                break;

            case eMode2Form2:               /* ignore EDC error */
                break;
            case eUnknownMode:
            case eYellowMode2:
                break;
        }
    }
}

/*********************************************************************
* *ReadStartIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* RdStart intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void ReadStartIntProc(void)
{
    fRStart =  TRUE;                /* mark Read Start */
    Rw_tblk_reach = TRUE;       /* mark target block reached */

    fRdMiss = FALSE;
    St_run_in_out = FALSE;

#if (USING_C1 == 1) /* Enable C1 Interrupt */
        WRITE_REG(HAL_CDDREG_MAP->C1THRD, 28);
        WRITE_REG(HAL_CDDREG_MAP->C12CCR, (HAL_CDD_C2UNC | HAL_CDD_C1UNC) );
        WRITE_FIELD(HAL_CDD_C1ERREN, 1);
#endif
}

/*********************************************************************
* *RunOutIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* ROINT(WRTBRK) intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void RunOutIntProc(void)   /* RunOut1 */
{
    if (Rw_tblk_reach == FALSE)
        return;

    WRITE_FIELD(HAL_CDD_MSFCERREN, 0);          /* disable MSF continuity check */

    St_run_in_out = TRUE;
}

/*********************************************************************
* *RunInEndIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* Rointe intrpt (end of a run-out1 detection)
*
* Returns:  void
* Params:   none
*********************************************************************/
void RunInEndIntProc(void)  /* RunIn4 */
{
    if (Rw_tblk_reach == FALSE)
        return;

    if (St_run_in_out == TRUE)
    {
        St_run_in_out = FALSE;
    }

    WRITE_FIELD(HAL_CDD_MSFCERREN, 1);          /* enable MSF continuity check */
}

/*********************************************************************
* *OverRunIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* OverRun intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void OverRunIntProc(void)
{
    SpeedCount = 0;

    send_msg80(SEND_ATAPI_CMD,0x710057);

    if (Rw_tblk_reach == TRUE)
    {
        Knl_deci_set = TRUE;                    /* set decode interrupt indicator */
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
    }
}

/*********************************************************************
* *ReadMissIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* RdStart intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void ReadMissIntProc(void)
{

    send_msg80(SEND_ATAPI_CMD,0x710058);

  #if (WITHOUT_SERVO == 0)

    fRdMiss = TRUE;

    if (Rw_tblk_reach == TRUE)
    {
        Iram_RW_ECC_Status |= HEADC2ERROR;
        Rw_ecc_error = TRUE;
    }
    else    /* overshoot error */
        Iram_RW_ECC_Status |= OVERSHOOTERROR;

    RecoverDecoderError();
    StopDecoder();
    Svr_pause_reqire = TRUE;
    Iram_Svr_player_mode = PAUSING_MODE;
  #endif // WITHOUT_SERVO
}

/*********************************************************************
* *MSBErrorIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* MSBErr intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void MSBErrorIntProc(void)
{
    if (Rw_tblk_reach == TRUE)                  /* This is MSF header not continus interrupt */
    {
        send_msg80(SEND_ATAPI_CMD,0x710059);
        /* Since we have C2 error for M2/M2F2/CDDA and EDC error for M1/M2F1 */
        /* we can ignore MSB error here. Leave message sent as indicating.   */
        return;

      #if (WITHOUT_SERVO == 0)

        Knl_deci_set = TRUE;                    /* set decode interrupt indicator */
        Iram_RW_ECC_Status |= MSBERROR;
        Rw_ecc_error = TRUE;

        RecoverDecoderError();
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
      #endif /* WITHOUT_SERVO */
    }
}

/*********************************************************************
* *HeaderErrorIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* HDC2Err intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void HeaderErrorIntProc(void)
{
    USHORT bptr, bptr_cur ;
    BYTE  i, temp;
    BYTE  subq[10];
    ULONG work_addr;


    send_msg80(SEND_ATAPI_CMD,0x71005A);

    temp = READ_REG(HAL_CDDREG_MAP->STATS);

    if ( ! TEST_BIT( temp, 4 ) ) {                      /* if CRC NG */
        /*
         * If HDR Error occured and CRC is No Good,
         * NEXT period Interrupt page will include illegal SubQ Data!!
         */
        if ( (St_audio_extract) && (!Rw_dec_stop) ) {
            if (St_nx_audio)
            {
                if (Iram_Audbuf_blkcnt < 2)
                    return;
            }else{
                if (DataBuffer.SectorCount < 2)
                    return;
            }

            bptr = READ_FIELD(HAL_CDD_DSUBPG);
            bptr &= 0x1FFF;
            bptr_cur = bptr;

            if ( bptr == Iram_usiRingSP)
                bptr = Iram_usiRingEP;
            else
                bptr--;

            work_addr = PAGE_AUX_START_ADDR(bptr);
            for (i = 0; i < 10; i++)
                subq[i] = SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + i];

            work_addr = PAGE_AUX_START_ADDR(bptr_cur);
            for ( i = 0; i < 10; i++ )
                SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + i] = subq[i];
        }
    }
}


/*******************************************************************************

    FUNCTION        HostTransferEndIntProc

    DESCRIPTION     Data transfer end interrupt service routine (BCEnd int and
                    TCmp int). Data transfer end interrupt occurs when data block
                    has been trasnferred to/from host


********************************************************************************/
void HostTransferEndIntProc(void)
{
    ULONG   tmp_hxfer_blk;
    USHORT  HstBptr;
    long tmp;
    ULONG Time;

#if (AUTO_STATU_CPLT==1)
    if (MRW_CheckDisc()==FALSE)
        DISABLE_HINT();
#endif

    OutputStream += Iram_Buf_hxfer_blk;

    if (St_send_blkdata == TRUE )   /* determine transfer mode */
    {
    /*-------------------------------------------------------
           Transfer is in page mode
    --------------------------------------------------------*/
#if (APPEND_RD_RVD == 1)
      /* if not udma or dma mode */
      if (!St_ultra_dma && !St_dma_mode)
#endif /* APPEND_RD_RVD */
      {

        if (pdbDiscLoaded() == CD_DISC)
        {
            HstBptr = READ_FIELD(HAL_HST_HSTBUFCC);
            WRITE_FIELD(HAL_HST_BUFPGCMP, HstBptr);
        }

        /* decrease the buffer block count by number of blocks transferred*/
        DataBuffer.SectorCount -= Iram_Buf_hxfer_blk;

        /* update the start of the cache */
        DataBuffer.CacheStartLBA += Iram_Buf_hxfer_blk;

        /* update the host buffer pointer by the number of blocks transferred to the host */
        DataBuffer.CurHostPtr += Iram_Buf_hxfer_blk;

        /* Insure the host buffer pointer wraps accordingly if it excedes the end of buffer */
        DataBuffer.CurHostPtr %=(Iram_usiRingEP + 1);

        PreCurHostPtr = DataBuffer.CurHostPtr;
        Pre_Buf_hxfer_blk = Iram_Buf_hxfer_blk;

        /* decrease the count for remainig blocks to transfer by the number of blocks just transferred */
        HostXfer.BlocksLeft -= (ULONG)Iram_Buf_hxfer_blk;

      }

        /*-------------------------------------------------------
            Determine if all requested blocks have been transferred
        --------------------------------------------------------*/
        if ( HostXfer.BlocksLeft == 0 )
        {
#if (ENABLE_AACS_BEF == 1)
            if (pdbDiscLoaded() == BD_DISC || pdbDiscLoaded() == HD_DISC)
            {
                HAL_HOST_StopAesBusEncryption();
            }
#endif

            /*-------------------------------------------------------
                All requested blocks have been transferred
            --------------------------------------------------------*/

            /*-------------------------------------------------------
                The following firmware readjusts the the block count,
                start of the cache, and the host buffer pointer to
                preserve data that has been transferred for the
                current read command. This improves the transfer
                rate in the event that the next command requests the
                same data.

                NOTES:
                When BURST RATE MODE is on (=1) maximum
                adjustment is 32 (64K) blocks, else it is 16 blocks

                This may not apply for DVD
            --------------------------------------------------------*/
            HostXfer.State = TRANSFER_COMPLETE; /* set host transfer state to complete */
#if (BypassDecoderEr == 1)
            IGNORE_DECODER_ERROR = FALSE;
#endif
            if((pdbDiscLoaded() == CD_DISC ) || ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc ))
            {
                /*-------------------------------------------------------
                   Get the original transfer count of the current
                   read command. Adjust for the appropriate burst rate
                   mode if needed.
                --------------------------------------------------------*/
                tmp = HostXfer.BlockCount.Long;         /* Get original transfer count of the read command*/

                if (HostXfer.BlockCount.Long > 0x20)
                {
                    tmp = 0x20;             /* Set max adjustment to 32 sectors */
                }

                /*-------------------------------------------------------
                    Increase the buffer block count and readjust the
                    start of the cache
                --------------------------------------------------------*/
                DataBuffer.SectorCount += tmp;
                DataBuffer.CacheStartLBA -= tmp;

                /*-------------------------------------------------------
                    Set back the host buffer pointer position
                --------------------------------------------------------*/
                if ( ((SHORT)DataBuffer.CurHostPtr - (SHORT)tmp) < 0)
                {
                    /*-------------------------------------------------------
                        Insure the buffer pointer remains within the ring
                        buffer
                    --------------------------------------------------------*/
                    DataBuffer.CurHostPtr -= tmp;
                    DataBuffer.CurHostPtr += (Iram_usiRingEP + 1);
                }
                else
                {
                    DataBuffer.CurHostPtr -= tmp;
                }

                if( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                    pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
                    PreReadCMDLba = (DataBuffer.CacheStartLBA-HostXfer.BlockCount.Long)&0xFFFFFFE0;
            }
            else if ( pdbDiscLoaded() == BD_DISC )
            {   /* To store the previous request LBA */
                PreReadCMDLba = (DataBuffer.CacheStartLBA-HostXfer.BlockCount.Long)&0xFFFFFFE0;
            }
	        else
	        {  /* To store the previous request LBA */
	           PreReadCMDLba = (DataBuffer.CacheStartLBA-HostXfer.BlockCount.Long)&0xFFFFFFF0;
	        }

            /* Reset the current transfer count */
            Iram_Buf_hxfer_blk = 0;

            /* Reset the read verify commmand flag if necessary */
            if ( fReadVerifyCommand == TRUE )
            {
                fReadVerifyCommand = FALSE;
            }


            /*-------------------------------------------------------
                Complete command if from a fast cache process
            --------------------------------------------------------*/
            if (Iram_St_fast_cache_hit == TRUE)
            {

#if (AUTO_STATU_CPLT==1)
                if(fKernelSendCompletionStatus== FALSE)
                {
                    SendStatus();                          /* report end status */
                    fKernelSendCompletionStatus = TRUE;     /* mark send completion */
                }
#else
                SendStatus();                              /* report end status */
#endif// (AUTO_STATU_CPLT==1)

                /*-------------------------------------------------------
                   Update flags
                --------------------------------------------------------*/
                Knl_send_compl = TRUE;          /* mark send completion */
                St_send_blkdata = FALSE;        /* mark no data out */
                Iram_St_fast_cache_hit = FALSE; /* reset fast cache flag */
                St_cmd_inproc = FALSE;          /* mark command not in process */
                Ata_sub_burst = FALSE;          /* clear ata sub burst */

                /*-------------------------------------------------------
                  Set LED
                --------------------------------------------------------*/
#if (LED_NUM == 1)
                ReadyLED(LED_OFF);
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
                ReadyLED(LED_AXferOn);
#endif /* LED_NUM == 2 */

#if (AUTO_STATU_CPLT==1)
                /*-------------------------------------------------------
                  Set status completion
                --------------------------------------------------------*/
                if(fKernelSendCompletionStatus== FALSE)
                    {
                        WRITE_FIELD(HAL_HST_SCTRG, 1);          /* STATUS_COMPLETION   */
                        fKernelSendCompletionStatus = TRUE;     /*mark send completion */
                    }
                ENABLE_HINT();
#else
                Time = ReadTimer(&Iram_atapi_timer);
                SendMsgCn(SEND_ATAPI_CMD,8,0x800311,0,Iram_Sense.key,Iram_Sense.cod,Iram_Sense.qul,A4B(Time));
                WRITE_FIELD(HAL_HST_SCTRG, 1);                  /* STATUS_COMPLETION   */
#endif// (AUTO_STATU_CPLT==1)

                return;
            }

            /*-------------------------------------------------------
              Report end status
            --------------------------------------------------------*/
            if(Knl_send_compl == FALSE)
            {
                #if (AUTO_STATU_CPLT==1)
                if(fKernelSendCompletionStatus== FALSE)
                {
                    SendStatus();                          /* report end status */
                    fKernelSendCompletionStatus = TRUE;     /* mark send completion */
                }
                #else
                SendStatus();                              /* report end status */
                #endif// (AUTO_STATU_CPLT==1)
                Knl_send_compl = TRUE;                      /* mark send completion */
            }

            St_cmd_inproc = FALSE;              /* mark no cmd in proc */
            St_send_blkdata = FALSE;            /* mark no data out */

#if (BY_PASS_C2Error_For_VCD)
            C2ErrorCount = 0;
#endif

#if (CDPlayability == 1)
            if(DataBuffer.SectorCount > 26)
                CDBufferAlmostEmpty = FALSE;
#endif

        } /* End if transfer is complete */
        else
        {
            /*-------------------------------------------------------
              All requested blocks have not been transferred.
              (Command is not finished yet.) Determine if there
              are blocks in the buffer that can be transferred
            --------------------------------------------------------*/

            if (DataBuffer.SectorCount == 0)
            {
                /*-------------------------------------------------------
                    Buffer is empty so no transfer can be initiated.
                    Wait for the next decoder interrupt to start transfer
                --------------------------------------------------------*/
                St_send_blkdata = FALSE;    /* mark that no data is transferring */
                HostXfer.State = WAITING_FOR_TRANSFER;
            }
            else
            {
                /*-------------------------------------------------------
                    There is data in the buffer which is available to xfer
                    to host. Start transferring available blocks to host
                --------------------------------------------------------*/
                if(EjectKeyInfo.PressedValid)
                {
                    /* Eject is occuring so do not transfer */
                    St_send_blkdata = FALSE;    /* mark no data out */
                }
                else
                {
                    if ( fReadVerifyCommand == TRUE )
                    {
                        tmp_hxfer_blk = (ULONG)DataBuffer.SectorCount;

                        if ( HostXfer.BlocksLeft < tmp_hxfer_blk )
                        {

                            /* part of cache data block is transferred */
                            DataBuffer.SectorCount    = (WORD)(tmp_hxfer_blk - HostXfer.BlocksLeft);
                            tmp_hxfer_blk = HostXfer.BlocksLeft;
                            HostXfer.BlocksLeft  = 0;
                            Knl_send_compl = TRUE;      /* mark send completion */
                            if ( Iram_St_fast_cache_hit == TRUE )
                            {
                                Iram_St_fast_cache_hit = FALSE;
                            }
                            fReadVerifyCommand = FALSE;
                            AutoCommandComplete();             /* report end status */
                        }
                        else
                        {
                        /* whole data block is transferred */
                            DataBuffer.SectorCount    = 0;
                            HostXfer.BlocksLeft -= tmp_hxfer_blk;
                        }

                        /* update start of cache to indicate data xfered done */
                        DataBuffer.CacheStartLBA += tmp_hxfer_blk;

                        /* Advance the host buffer pointer */
                        DataBuffer.CurHostPtr += tmp_hxfer_blk;

                        /*-------------------------------------------------------
                            Insure the buffer pointer remains within the ring
                            buffer
                        --------------------------------------------------------*/
                        DataBuffer.CurHostPtr %=(Iram_usiRingEP + 1);
                        WRITE_FIELD(HAL_HST_HSTBUFPG, DataBuffer.CurHostPtr);       /* adjust Host buffer page pointer */
                        WRITE_FIELD(HAL_HST_BUFPGCMP, DataBuffer.CurHostPtr);
                        St_send_blkdata = FALSE;    /* mark no data out */
                    }
                    else
                    {
                        St_send_blkdata = FALSE;    /* mark no data out */
                        if (pdbGetDiscKind() != eBDRDisc && pdbGetDiscKind() != eBDRDLDisc
                            && pdbGetDiscKind() != eBDREDisc && pdbGetDiscKind() != eBDREDLDisc
                            && pdbGetDiscKind() != eDVDRAMDisc)
                            SendHostBlocks();           /* Start transferring next available blocks */
                    }
                }
            }
        }
    }
    else if (St_send_pardata == TRUE)
    {
        /*-------------------------------------------------------
               Transfer is linear mode
        --------------------------------------------------------*/
        if((Ata_sub_burst == TRUE) && !Residue_minus_xfr_byte() )
        {
            IncATAPIByteCount();                /* increment the linear address */
            Min_residue_xfr_allowed();          /* SH_ATBLO/HI = min (residue,xfer_allow) */
            TransferToHost();                   /* trigger xfer */

            #if (AUTO_STATU_CPLT==1)
            ENABLE_HINT();
            #endif

            return;
        }

        St_send_pardata = FALSE;                /* mark not sending data */

        if(Knl_IdentAvail)
        {
            /*-------------------------------------------------------
                  Identify information was transferred
            --------------------------------------------------------*/
            Knl_IdentAvail = FALSE;
            Ata_sub_burst = FALSE;              /* always not page case */
            Knl_atcmd_avail = FALSE;
            St_cmd_inproc = FALSE;

            WRITE_REG(HAL_HSTREG_MAP->ATFEA, 0);      /*[21h] error reg */
            WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14); /* BYTE count */
            //WRITE_REG(HAL_HST_DRDYW_REGISTER, 0x50);  /* [627h] status reg set DRDY, DSC bit*/ // chip auto set to 0x50 already
            ATAClearBusy();                         /* clear busy */
        }
        else if (Knl_Atapi_Avail)
        {
            /*-------------------------------------------------------
                  Inquiry information was transferred
            --------------------------------------------------------*/
            Knl_Atapi_Avail = FALSE;
            BUILD_SENSE(0x00,0x00,0x00,0x0f);        /* NO ADDITIONAL SENSE INFO */
            AutoCommandComplete();
        }
    }
    else if (Get_para_data == TRUE)
        Get_para_data = FALSE;              /* mark not getting parameter data */
    else if (St_diag_HIFTransData==TRUE)
        St_diag_HIFTransData = FALSE;       /* mark not transferring data (diagnostic) */


    #if (AUTO_STATU_CPLT==1)
        ENABLE_HINT();
    #endif
}

/*********************************************************************
* *C2ErrorIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* C2Err intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
#if (HORROR_DISC_IMPROVEMENT == 1)
BYTE VCD_Try_Case=0,C2CNT1=0;
BYTE    old_param[2];
#endif
void C2ErrorIntProc(void)
{
#ifdef Skip_C2
    BYTE skipC2error;
#endif

     send_msg80(SEND_ATAPI_CMD,0x71005B);


/* Definition for Iram_Rw_read_mode */
//#define eUnknownMode        0
//#define eMode1              1
//#define eMode2Form1         2
//#define eMode2Form2         3
//#define eYellowMode2        4
//#define eCDDAMode           5
    SendMsgCnB(SEND_HIF_CMD,1,0x80041D,B1B(Iram_Rw_read_mode)); // Iram_Rw_read_mode

    #if (CD_DFE_IMPROVE_READ == 1)
    if ((DFE_CHG_CASE&0x01)== 0)
    {
        DFE_CHG_SEL++;
        DFE_CHG_CASE |= 0x01;
    }
    #endif

  #if (IGNORE_ERROR_FOR_AUDIO == 1)

    if (( Iram_Rw_read_mode == eCDDAMode ) && ( fLastRetry == TRUE ))
    {
        fLastRetry = FALSE;
        return;
    }

  #endif //(IGNORE_ERROR_FOR_AUDIO == 1)

    if ( (Rw_tblk_reach == FALSE) || (St_run_in_out == TRUE) )
        return;

    /* for M2/M2F2/CDDA buffering, C2 uncorrectable errors
      (exceeds Threshold) need be checked, and reported as read failure */
    if ((Iram_Rw_read_mode != eMode1) && (Iram_Rw_read_mode != eMode2Form1) && (Iram_Rw_read_mode != eUnknownMode))
    {
#if 0//(HORROR_DISC_IMPROVEMENT == 1)
        curC2pSec[C2CNT1]=READ_FIELD(HAL_CDD_C2CNT);
        if(VCD_Try_Case==1 && C2CNT1==1)
        {
            if(curC2pSec[1]<(curC2pSec[0]/3))
            {
                send_msg5S(1, "try case 1 ready");
            }
            else
            {
                VCD_Try_Case=2;
                send_msg5S(1, "try case 1 fail");
                WRITE_FIELD(HAL_DFE_MAHPFBW, old_param[0]);
                WRITE_FIELD(HAL_DFE_OFFSETBND, old_param[1]);   //set AOC bound
            }
            C2CNT1=0;
                
         }       
        if((curC2pSec[0]>50) && (VCD_Try_Case==0))
        {
            C2CNT1++;
            CDBufferAlmostEmpty = FALSE;
            C2ErrorCount=0;
            send_msg5S(1, "VCD try case 1");
            VCD_Try_Case=1;
            WRITE_FIELD(HAL_DFE_ADQLKCOEF,  0);
            WRITE_FIELD(HAL_DFE_ADEQDIS,0);
            old_param[0]=READ_FIELD(HAL_DFE_MAHPFBW);
            //WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_104_adc_samples);//TBD:disable Moving average HPF retry, need check later
            old_param[1]=READ_FIELD(HAL_DFE_OFFSETBND);
            WRITE_FIELD(HAL_DFE_OFFSETBND, 0x0A);   //set AOC bound
        }
                
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x800A18,B1B(curC2pSec[0]),B1B(curC2pSec[1]));

#endif
#if (CDPlayability == 1)
        /* for M2/M2F2/CDDA buffering, if buffer is almost empty, then bypass C2 error*/
        if (((DataBuffer.SectorCount < 50) &&(St_cdrom_audio))||(CDBufferAlmostEmpty == TRUE))
        {
            SpeedCount = 0;
            send_msg80(SEND_ATAPI_CMD,0x71005C);
            return;
        }
#endif

#if (BY_PASS_C2Error_For_VCD)
        if (C2ErrorCount <= ByPassC2freq){
        	C2ErrorCount++;
#endif
        Knl_deci_set = TRUE;                    /* set decode interrupt indicator */
        Rw_ecc_error = TRUE;                    /* indicates error */
        RecoverDecoderError();
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
#if (BY_PASS_C2Error_For_VCD)
            }
        else
            send_msg80(SEND_ATAPI_CMD,0x71005C);
#endif

    }

}

/*********************************************************************
* RunOut1MissedIntProc(void)
*
* decoder interrupt service routine which occurs whenever there is from
* RunOut1 Missed intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void RunOut1MissedIntProc(void)
{
    send_msg80(SEND_ATAPI_CMD,0x71005D);

    if (Rw_tblk_reach == TRUE)
    {
        Rw_ecc_error = TRUE;                /* indicates error */
        Knl_deci_set = TRUE;                /* set decode interrupt indicator */
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
    }
}

/*********************************************************************
* RunIn4MissedIntProc(void)
*
* decoder interrupt service routine which occurs whenever there is from
* RunIn4 Missed intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void RunIn4MissedIntProc(void)
{
    send_msg80(SEND_ATAPI_CMD,0x71005E);

    if (Rw_tblk_reach == TRUE)
    {
        /* Since we have C2 error for M2/M2F2/CDDA and EDC error for M1/M2F1 */
        /* we can ignore this error here. Leave message sent as indicating.  */
        //return; // some disc can't detect C2 or EDC error when RUNI Miss.

        Rw_ecc_error = TRUE;                /* indicates error */
        Knl_deci_set = TRUE;                /* set decode interrupt indicator */
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
    }
}


/*********************************************************************
* CDReadFailIntProc()
*********************************************************************/
void CDReadFailIntProc(void)
{
    send_msg80(SEND_ATAPI_CMD,0x71005F);

    if (Rw_tblk_reach == TRUE)
    {
        Iram_RW_ECC_Status |= SYNCERROR;
        Rw_ecc_error = TRUE;        /* indicates error */
        Knl_deci_set = TRUE;        /* set decode interrupt indicator */
        //SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;//Force to speed down for Audio CD's DEA test
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
    }
}


/*********************************************************************
* NoCDRomSyncIntProc()
*********************************************************************/
void NoCDRomSyncIntProc(void)
{
    send_msg80(SEND_ATAPI_CMD,0x710060);

    if (Rw_tblk_reach == TRUE)
    {
        Rw_ecc_error = TRUE;        /* indicates error */
        Knl_deci_set = TRUE;        /* set decode interrupt indicator */
        StopDecoder();
        Svr_pause_reqire = TRUE;
        Iram_Svr_player_mode = PAUSING_MODE;
    }
}

/*********************************************************************
* SRWErrorIntProc()
*
* decoder interrupt service routine which occurs whenever there is from
* SRWErr intrpt
*
* Returns:  void
* Params:   none
*********************************************************************/
void SRWErrorIntProc(void)
{
     send_msg80(SEND_ATAPI_CMD,0x710061);

    Knl_deci_set = TRUE;                    /* set decode interrupt indicator */
    Rw_ecc_error = TRUE;                    /* indicates error */
    RecoverDecoderError();
    StopDecoder();
    Svr_pause_reqire = TRUE;
    Iram_Svr_player_mode = PAUSING_MODE;
}


#if (USING_C1 == 1)/* Enable C1 Interrupt */
/*******************************************************************/
void C1ErrorIntProc()
{
      send_msg80(SEND_ATAPI_CMD,0x710062);
}

#endif
/*******************************************************************/
