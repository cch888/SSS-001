/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_init.c
*
* DESCRIPTION
*   This file contains the initialization routine which is executed
*   on Power or reset stage.
*
* NOTES:
*
*
*   $Revision: 167 $
*   $Date: 11/03/28 2:37p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\oem\oem_tbl.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\servo\svo.h"
#include ".\hif\gp_equ.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_inqry.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\gp_init.h"
#include ".\hif\gp_util.h"
#include ".\hif\mmc.h"
#include ".\hif\app_if.h"
#include ".\dvd\dvinit.h"
#include ".\al\REG_global.h"
#include ".\al\REG_sdram.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif

#if (E_DIAGS == 1)
#include ".\hif\diag.h"
#endif

#if ((EPO_SLOT_IN == 1)&&(SUN_FUNC_TEST == 1))
#include ".\hif\gp_key.h"
#endif

static void InitializeIdentifyDriveReturnData(void);    /* Init identify data in SDRAM */
static void InitializeInquiryReturnData(void);          /* Init inquiry data in SDRAM */
static void InitializeRequestSenseReturnData(void);     /* Init request sense data in SDRAM */
static void InitializeModeSenseReturnData (void);       /* Init mode sense data in SDRAM */
static void InitializeSubQData(void);                   /* Init subq header data in SDRAM */

extern void copy_mode5_data_to_sram(void);
extern void initNotificationData(void);
extern USHORT GetTransferSpeed(BYTE spd);
extern BYTE CheckUnbalancedEccentricityDisc(BYTE Curspeed);
extern USHORT   timeUp_Group1;
extern USHORT   timeUp;

extern BYTE pre_tno;                /* Check tno protect */
extern ULONG Pre_RelativeTime_lba;
extern ULONG Pre_AbsoluteTime_lba;
extern BYTE const PersistentTable[SIZE_PERSISTENT_TABLE];    /* Use read_power_offset to be a unique_serial_number */
extern struct dvdDiscS     volatile dvdDisc;
#if(DeadLoop_Solution==1)           /* for servo DeadLoop Solution */
extern SBYTE svo_ERR_STARTUP;
#endif

#if (CEProduct == 1)
extern USHORT TimeUpForRead;
#endif
extern BYTE CurTblOnDram;
#if (SUN710_TRAY_PROFILE == 1) || ( EEPROM_TRAY_PROFILE ==1) 
extern BYTE fTuneMode;
#endif
/*********************************************************************
* InitializeChip
*
* Initialize Non ARM related Registers
*********************************************************************/
void InitializeChip(void)
{
    LAL_InitHif();
}

/*********************************************************************
* InitializeVariables()
*
* Initialize internal memory variables and flags
*********************************************************************/
void InitializeVariables(void)
{
    HostXfer.StartBlock.lba = 0;            /* Init. start block as zero */
    Channel_ctrl_mode = 0xF3;               /* set dac mode as mute-is-off on L/R*/
                                            /* Init. previous MSF */
    Iram_Sh_wbk = 0;
    Iram_shATB = 0;

    Period_pg = 2;                          /* init period page number */
    pre_tno = 0xff;
    Pre_RelativeTime_lba = 0xffffffff;
    Pre_AbsoluteTime_lba = 0xffffffff;

    Knl_cmd_avail       = FALSE;            /* mark no ATAPI command available */
    Knl_atcmd_avail     = FALSE;            /* mark no ATA command available */
    St_cmd_inproc       = FALSE;            /* mark no command in process */
    St_cd_prevent       = FALSE;            /* clear cd prevent flag */
    St_drv_ready        = FALSE;            /* assume drive not ready */
    St_cmd_inproc       = FALSE;            /* mark no cmd in process */
    St_send_blkdata     = FALSE;            /* mark no data xfr to host */
    St_send_pardata     = FALSE;            /* mark no para xfr to host */
    st_disc_change      = FALSE;            /* assume no cd change */
    St_audio_complete   = FALSE;            /* init audio complete flag */
    St_audio_pause      = FALSE;            /* init audio pausing flag */
    St_nx_audio         = FALSE;
    Iram_audio_scan_st  = FALSE;
    St_audio_extract    = FALSE;
    St_method_2         = FALSE;
    St_run_in_out       = FALSE;
    St_become_ready     = FALSE;
    Read_subc_only  = FALSE;    /*read subcode only */
#if(DeadLoop_Solution == 1)      /* for servo DeadLoop Solution */
        svo_ERR_STARTUP     = FALSE;
#endif                         /* for servo DeadLoop Solution */

#if ((EPO_SLOT_IN == 1)&&(SUN_FUNC_TEST == 1))
    TrayIn_Error = 0;
#endif

#if (USE_TWO_KEY_FUNCTION == 1)
    opAMute = 0;
    gSt_play_sw = 0;
    gHw_play_no = HW_PLAY_STOP_OFF;
    gPause_By_SeekCmd_flag = FALSE;
#endif /* USE_TWO_KEY_FUNCTION */
    Iram_Rd_subc_flag = FALSE;  /* read data with subcode */

    Iram_Diff_Time_Flag = FALSE;

    /*Automatic change from 2.5K to 3k page for C2PO*/
    Iram_Page_Size_3K_Flag = FALSE;
    DRAM_AUX_SIZE = DRAM_AUX_SIZE_2p5K; /* DRAM AUX Size = 0x200(= 0.5KB) */
    RING_END_PAGE = RING_END_PAGE_2p5K; /* last page the SP can access*/
    BUF_TOTAL_BLK_COUNT = BUF_TOTAL_BLK_COUNT_2p5K; /* total 672 pages */
    DATA_START_ADDR = DATA_START_ADDR_2p5K; /* Data start address */
    AUX_START_ADDR = AUX_START_ADDR_2p5K;   /* Aux start address  */
    AUX_HEADER_ADDR1 = AUX_HEADER_ADDR1_2p5K;   /* M1 block header offset in page */
    AUX_HEADER_ADDR2 = AUX_HEADER_ADDR2_2p5K;   /* M2F1 block header offset in page */
    /*Automatic change from 2.5K to 3k page for C2PO*/

    Rw_cdda_disc        = FALSE;
    Iram_Rw_read_mode   = eUnknownMode;     /* rw_read_mode initial to 0 */
    Ata_sub_burst       = FALSE;            /* clear less than 2k flag */
    ataPowerMode     = IDLE_MODE;           /* set idle mode  */

    timeUp_Group1       = TIMEUP_6S;        /* ATAPI Cmd Time-Out Timer */
    timeUp              = timeUp_Group1;
#if (CEProduct == 1)
    TimeUpForRead       = timeUp_Group1;
#endif

    Management_Max_rate = MAX_READ_SPEED;

   #ifdef BYPASS_POWERSAVE_ISR
      svoFgBaseOnPowerSaveFlag = TRUE;
   #endif
    CurTblOnDram = 0xFF;
    switch (READ_FIELD(HAL_GLOBAL_EMBEDMEMS))   // Check for the type of embedded SDRAM that is installed.
    {
        case SDRAM_M12L16161AFK1:     // Here set up SDRAM regs for detected SDRAM
            SdramRefreshInterval      = M12L16161AFK1_REFINTV;      // Save value for Power Control function
            SdramRefreshIntervalSleep = M12L16161AFK1_REFINTV_SLEEP;
            break;
        case SDRAM_M12L32321ADM1:     // Here set up SDRAM regs for detected SDRAM
            SdramRefreshInterval      = M12L32321ADM1_REFINTV;      // Save value for Power Control function
            SdramRefreshIntervalSleep = M12L32321ADM1_REFINTV_SLEEP;
            break;
        case SDRAM_M12L32162A:     // Here set up SDRAM regs for detected SDRAM
            SdramRefreshInterval      = M12L32162A_REFINTV;         // Save value for Power Control function
            SdramRefreshIntervalSleep = M12L32162A_REFINTV_SLEEP;
            break;
        case SDRAM_EXTERNAL:      // Here set up SDRAM regs for specified External SDRAM (not embedded !)
            SdramRefreshInterval      = EXTERNALSDRAM_REFINTV;      // Save value for Power Control function
            SdramRefreshIntervalSleep = EXTERNALSDRAM_REFINTV_SLEEP;
            break;
        default:
            send_msg5S(1,"unsupported sdram!!");
    }
#if (SUN710_TRAY_PROFILE == 1)  || ( EEPROM_TRAY_PROFILE ==1) 
    fTuneMode = FALSE;
#endif
}


/*********************************************************************
* InitializeTables()
*
* Initialize identify, inquiry, request_sense, and mode_sense data by
* moving data from code memory to dram space.
*********************************************************************/
void InitializeTables(void)
{
    InitializeIdentifyDriveReturnData();    /* Identify Device Command */
    InitializeInquiryReturnData();          /* Inquiry Command */
    InitializeRequestSenseReturnData();     /* Request Sense Command */
    InitializeModeSenseReturnData();        /* Mode Sense/Select Commands */
    InitializeSubQData();                   /* SubQ data */
    initNotificationData();
    SetDefaultReadSpeed();
}

/*********************************************************************
* InitializeIdentifyDriveReturnData
*
* Moving identify data from code memory to dram space
*********************************************************************/
static void InitializeIdentifyDriveReturnData(void)
{
    UINT i;
    BYTE const *code_ptr;

    code_ptr = Identify_tbl;            /* initialize code pointer */
    Iram_Buf_dram_addr = IDENTIFY_ADR;

    for(i = 0;i < IDENTIFY_LEN_BYTE;i++,code_ptr++){

        WriteSDRAM(*code_ptr);
    }

#if (MODEL_NAME_FROM_EEPROM == 1)
/* Firmware ID ( 16bytes) */
    Iram_Buf_dram_addr = IDENTIFY_ADR + 54;
    for(i=0;i<8;i+=2)
    {
        SCSDRAM[Iram_Buf_dram_addr+i] = GetPersistentTable1B(VenderID_ADDR+1+i);
        SCSDRAM[Iram_Buf_dram_addr+i+1] = GetPersistentTable1B(VenderID_ADDR+i);

    }
    Iram_Buf_dram_addr = IDENTIFY_ADR + 62;
    for(i=0;i<16;i+=2)
    {
        SCSDRAM[Iram_Buf_dram_addr+i] = GetPersistentTable1B(ProductID_ADDR+1+i);
        SCSDRAM[Iram_Buf_dram_addr+i+1] = GetPersistentTable1B(ProductID_ADDR+i);
    }
#endif

    Iram_Buf_dram_addr = IDENTIFY_ADR + 20;
    SCSDRAM[Iram_Buf_dram_addr] = GetPersistentTable1B(CD_CHECK_SUM_LO);   /* CD Check Sum of PersistentTable */
    SCSDRAM[Iram_Buf_dram_addr+1] = GetPersistentTable1B(CD_CHECK_SUM_HI);
}

/*********************************************************************
* InitializeInquiryReturnData()
*
* Moving inquiry data from code memory to dram space
*********************************************************************/
static void InitializeInquiryReturnData(void)
{
    BYTE i;
    BYTE const *tbl_ptr;

    tbl_ptr = Inquiry_tbl;              /* pointer=code addr */
    Iram_Buf_dram_addr = INQUIRY_ADR;

    for(i=0;i<INQUIRY_LEN_BYTE;i++,tbl_ptr++){

        WriteSDRAM(*tbl_ptr);
    }


#if (MODEL_NAME_FROM_EEPROM == 1)
/* Vender ID  ( 8bytes) */
    Iram_Buf_dram_addr = INQUIRY_ADR + 8;
    for(i=0;i<8;i++)
        WriteSDRAM(GetPersistentTable1B(VenderID_ADDR+i));

/* Product ID  (16bytes) */
    Iram_Buf_dram_addr = INQUIRY_ADR + 16;
    for(i=0;i<16;i++)
        WriteSDRAM(GetPersistentTable1B(ProductID_ADDR+i));

/* Serial Number  (16bytes)*/
    Iram_Buf_dram_addr = INQUIRY_ADR + 36;
    for(i=0;i<16;i++)
        WriteSDRAM(GetPersistentTable1B(SerialNumber_ADDR+i));
#endif

}

/*********************************************************************
* InitializeRequestSenseReturnData()
*
* Moving request_sense data from code memory to dram space
*********************************************************************/
static void InitializeRequestSenseReturnData(void)
{
    BYTE i;
    BYTE const *tbl_ptr;
                                        /* locate starting address */
    tbl_ptr = Request_sense_tbl;        /* pointer=code addr */
    Iram_Buf_dram_addr = REQ_SENSE_ADR;

    for(i=0;i<REQ_SENSE_LEN_BYTE;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }
}

/*********************************************************************
* InitializeModeSenseReturnData()
*
* Moving mode_sense data from code memory to dram space
*********************************************************************/
static void InitializeModeSenseReturnData(void)
{
    BYTE i;
    BYTE const *tbl_ptr;
#if (AUDIO_PLAYBACK == 1)
    BYTE l_vol,r_vol;
#endif
    tbl_ptr = Mode_sense_def_tbl;   /* pointer=code addr */
    Iram_Buf_dram_addr = MSENSE_ADR;

    for(i=0;i<8;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_1;     /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_1_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_5;     /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_5_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_8;        /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_8_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_d;     /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_D_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_e;     /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_E_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_1a;        /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_1A_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_1d;        /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_1D_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

    tbl_ptr = mode_sense_def_2a;        /* pointer=code addr */
    for(i=0;i<MSENSE_PAGE_2A_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }

#if (EN_LS == 1)
    tbl_ptr = mode_sense_def_31;
    for(i=0;i<MSENSE_PAGE_31_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }
    tbl_ptr = mode_sense_def_32;
    for(i=0;i<MSENSE_PAGE_32_LEN;i++,tbl_ptr++)
    {
        WriteSDRAM(*tbl_ptr);
    }
#endif // End of (EN_LS == 1)

#if (AUDIO_PLAYBACK == 1)
    Iram_Buf_dram_addr = MSENSE_PAGE_E + 9;
    l_vol = ReadSDRAM();
    Iram_Buf_dram_addr = MSENSE_PAGE_E + 11;
    r_vol = ReadSDRAM();
    SetDefaultVolume(l_vol,r_vol);
#endif
    copy_mode5_data_to_sram();
}

/*********************************************************************
* InitializeSubQData()
*
* Initialize the first 4+2 bytes of the read subchannel data
* (format code 01, audio status=11) into the dram space
*********************************************************************/
static void InitializeSubQData(void)
{
    Iram_Buf_dram_addr = RD_SUBQ_LBA_ADR;
    WriteSDRAM(0x00);
    WriteSDRAM(0x11);
    WriteSDRAM(0x00);
    WriteSDRAM(0x0c);
    WriteSDRAM(0x01);
    WriteSDRAM(0x10);
}

/*********************************************************************
* InitializeReadSpeed()
*
* Initialize default read speed,
* This routine will get the default Read Speed based on Media type.
* The routine then will modify the speed based on different disc
* characteristics such as eccentrity, balance ...
*********************************************************************/
extern BYTE dvdVideoFlag;   /* for DVD video */
#if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
extern BOOL  SpeedState;
#endif
void InitializeReadSpeed(void)
{
USHORT  spdt;
BYTE    speed, media, tmp_speed;
#if (SEQ_SEEK_SPEED_DOWN == 1)
BYTE    seekspeed;
#endif
TimeS   time;

/*** WARNING: If you change this function, you have to change "Set_SpeedMDCont(void)" ***/

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            media = CDReturnMediaOffsetForRead();                   /* Determine the row offset based on the media type */

            /* Initialize Read Speed control variables for CD */
            ptrReadSpeedTablePointer = &CDSelSpeed_tbl_R[media][0];
            SpeedUpReady = CD_SPEED_UP_READY;
            SpeedDownReady = CD_SPEED_DOWN_READY;
            SpeedTableColumnSize = CD_SPD_COLMNS;
            speed = ptrReadSpeedTablePointer[CD_SPD_COLMNS - 1];

            #if (SEQ_SEEK_SPEED_DOWN == 1)
            seekspeed = CD_SEEK_DOWN_SPEED;
            #endif
#if (SUPPORT_DPD == 1)
            if (svoIramVar.do_cd_dpd_disc == TRUE)
            {
                if(speed > MAX_MCD_SPEED)
                      speed = MAX_MCD_SPEED;
            }
#endif
#if (DETECT_ECC_DISC == 1)
            if (svoVar_eccDiscNum != eECC_normal)         /* Check if Eccentric Disc is in system */
            {
               if (speed > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_eccDiscNum])
                  speed = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_eccDiscNum];  /* Get Initial Speed based on Eccentricity */
            }
#endif // DETECT_ECC_DISC

 #if (CD_UNBALANCE_DISC_CHECK == 1)
            if (svoVar_unbalanceDiscNum != 0)         /* Check if Unbalanced Disc is in system */
            {
               if (speed > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_unbalanceDiscNum])
                  speed = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
            }
 #endif

            /* Supported Automatic Acoustic Management */
            if (speed > Management_Max_rate)
            {
                speed = Management_Max_rate;
            }

#if DISC_8CM == 1
            /* 8cm DISC */
            if ( (speed > MAX_8CM_CD_DISC_SPD) && (St_12_cm == FALSE) && (St_Fashion == FALSE) )
            {
                speed = MAX_8CM_CD_DISC_SPD;        /* Normal 8cm Disc maximum read speed */
            }
            else if ( (speed > MAX_FASHION_DISC_SPD) && (St_12_cm == FALSE) && (St_Fashion == TRUE) )
            {
                speed = MAX_FASHION_DISC_SPD;    /* 8 cm Fashion Disc maximum speed */
            }
#endif /* DISC_8CM */

            if(pdbGetFinalSessCloseInfo() == eSessionFinalized)
            {
                time = pdbGetFinalSessLeadoutTime();
            }
            else
            {
                time = pdbGetNextSessStartTime();
            }

            /* 80 - 99 Minute Disc - limit speed to max CD speed at OD */
            if(time.Minute >= 80)
            {
                if(speed > MAX_80_MIN_DISC_SPD) // 80 minute disc maximum read speed
                {
                    speed = MAX_80_MIN_DISC_SPD;
                }
            }

            /* Double check that read speed does not exceed maximum from table */
            media = CDReturnMediaOffsetForRead();                       /* Determine the row offset based on the media type */
            tmp_speed = CDSelSpeed_tbl_R[media][CD_SPD_COLMNS - 1];  /* Get Maximum Read Speed for media */

            if(speed > tmp_speed)
                speed = tmp_speed;    // Speed exceeded maximum speed from speed table. Change to maximum from speed table

            break;

        case DVD_DISC:
            media = DVDReturnMediaOffsetForRead();                      /* Determine the row offset based on the media type */

            /* Initialize Read Speed control variables for DVD */
            ptrReadSpeedTablePointer = &DVDSelSpeed_tbl_R[media][0];
            SpeedUpReady = DVD_SPEED_UP_READY;
            SpeedDownReady = DVD_SPEED_DOWN_READY;
            SpeedTableColumnSize = DVD_SPD_COLMNS;
            #if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
            SpeedState = 0;
            #endif
            speed = ptrReadSpeedTablePointer[DVD_SPD_COLMNS - 1];    /* Get Maximum Read speed for media */

            #if (SEQ_SEEK_SPEED_DOWN == 1)
            seekspeed = DVD_SEEK_DOWN_SPEED;
            #endif
            /* DVD Video disc handling */
#if 0  //Do not Remove! because 8x read ability is better than 6x for DVD9 video
            if( dvdVideoFlag == TRUE)
            {
                if(speed > eReadDVDPlaySpeed)
                    speed = eReadDVDPlaySpeed;  // Limit Speed for DVD Video disc
            }
#endif

#if (DETECT_ECC_DISC == 1)
            /* Eccentricity disc handling */
            if (svoVar_eccDiscNum != eECC_normal)               /* Check if Eccentric Disc is in system */
            {
                if(pdbGetDiscKind() == eDVDRAMDisc)
                {
                    if (speed > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_eccDiscNum])
                        speed = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_eccDiscNum];  /* Get Speed based on Eccentricity */
                }
                else
                {
                   if (speed > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_eccDiscNum])
                        speed = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_eccDiscNum];  /* Get Speed based on Eccentricity */
                }
            }
#endif

#if (DVD_UNBALANCE_DISC_CHECK == 1)
            /* Unbalanced disc handling */
            if (svoVar_unbalanceDiscNum != 0)         /* Check if unbalanced Disc is in system */
            {
                if(pdbGetDiscKind() == eDVDRAMDisc)
                {
                    if (speed > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_unbalanceDiscNum])
                        speed = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_unbalanceDiscNum];  /* Get Speed based on unbalance */
                }
                else
                {
                    if (speed > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_unbalanceDiscNum])
                        speed = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_unbalanceDiscNum];  /* Get Speed based on unbalance */
                }
            }
#endif

#if DISC_8CM == 1
            /* 8cm DISC */
            if ( (speed > MAX_8CM_DVD_DISC_SPD) && (St_12_cm == FALSE) && (St_Fashion == FALSE) )
            {
                speed = MAX_8CM_DVD_DISC_SPD;        /* Normal 8cm Disc maximum read speed */
            }
#endif /* DISC_8CM */

            if (GetSpeedValue(speed) > GetSpeedValue(MAX_READ_DVD_SPEED))
            {
                speed = MAX_READ_DVD_SPEED;        /* Change speed to maximum Accoustic Management Speed */
            }
            break;

#if(BD_ENABLE)
        case BD_DISC:
            media = BDReturnMediaOffsetForRead();                      /* Determine the row offset based on the media type */

            /* Initialize Read Speed control variables for DVD */
            ptrReadSpeedTablePointer = &BDSelSpeed_tbl_R[media][0];
            SpeedUpReady = BD_SPEED_UP_READY;
            SpeedDownReady = BD_SPEED_DOWN_READY;
            SpeedTableColumnSize = BD_SPD_COLMNS;
            speed = ptrReadSpeedTablePointer[BD_SPD_COLMNS - 1];    /* Get Maximum Read speed for media */
            #if (SEQ_SEEK_SPEED_DOWN == 1)
            seekspeed = BD_SEEK_DOWN_SPEED;
            #endif
#if (DETECT_ECC_DISC == 1)
            if (svoVar_eccDiscNum != eECC_normal)         /* Check if Eccentric Disc is in system */
            {
                if (speed > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_eccDiscNum])
                    speed = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_eccDiscNum];  /* Get Initial Speed based on Eccentricity */
            }
#endif // DETECT_ECC_DISC

#if (BD_UNBALANCE_DISC_CHECK == 1)
            if (svoVar_unbalanceDiscNum != 0)         /* Check if Unbalanced Disc is in system */
            {
                if (speed > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_unbalanceDiscNum])
                    speed = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
            }
#endif

            if (GetSpeedValue(speed) > GetSpeedValue(MAX_READ_BD_SPEED))
            {
                speed = MAX_READ_BD_SPEED;        /* Change speed to maximum Accoustic Management Speed */
            }
            break;
#endif
#if(ENABLE_HDDVD)
        case HD_DISC:
            media = HDReturnMediaOffsetForRead();                      /* Determine the row offset based on the media type */

            /* Initialize Read Speed control variables for HDDVD */
            ptrReadSpeedTablePointer = &HDSelSpeed_tbl_R[media][0];
            SpeedUpReady = HD_SPEED_UP_READY;
            SpeedDownReady = HD_SPEED_DOWN_READY;
            SpeedTableColumnSize = HD_SPD_COLMNS;
            speed = ptrReadSpeedTablePointer[HD_SPD_COLMNS - 1];    /* Get Maximum Read speed for media */

            if (GetSpeedValue(speed) > GetSpeedValue(MAX_READ_HD_SPEED))
            {
                speed = MAX_READ_HD_SPEED;        /* Change speed to maximum Accoustic Management Speed */
            }
            break;
#endif
    }

    /*----------------------------------------------
    Speed has been determined for the inserted Disc
    Update SupportSpeed.Read and Mode Sense page
    ------------------------------------------------*/

    Disc_SupportSpeed.Read = Disc_Speed.Read = speed;   /* Update all the Speed variables */
#if (SEQ_SEEK_SPEED_DOWN == 1)
    Disc_Speed.SeekSpeed = seekspeed;
#endif


    send_msg5SValue(SEND_MRW_MSG,"Disc_Speed.Rd",1,Disc_Speed.Read);

    spdt = GetTransferSpeed(speed);                     /* Determine the Transfer rate from the Speed */
    Iram_Buf_dram_addr = MSENSE_PAGE_2A+8;              /* Update Mode Page for Read Speed - obsolete in MMC1+ */
    WriteSDRAM((BYTE)(spdt >> 8));
    WriteSDRAM((BYTE) spdt      );
    Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;              /* Update Mode Page for Write Speed - obsolete in MMC1+ */
    WriteSDRAM((BYTE)(spdt >> 8));
    WriteSDRAM((BYTE) spdt      );
}


/*********************************************************************
* SetDefaultReadSpeed()
*
* Initialize default read speed,
*********************************************************************/
void SetDefaultReadSpeed(void)
{
    USHORT spdt;

    PartialCAV_Speed = FALSE;

    Disc_SupportSpeed.Read = Disc_Speed.Read = Management_Max_rate;

    spdt = GetTransferSpeed(Disc_Speed.Read);
    Iram_Buf_dram_addr = MSENSE_PAGE_2A+8;
    WriteSDRAM((BYTE)(spdt >> 8));
    WriteSDRAM((BYTE) spdt      );
    Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
    WriteSDRAM((BYTE)(spdt >> 8));
    WriteSDRAM((BYTE) spdt      );

    Iram_Buf_dram_addr = MSENSE_PAGE_2A+18;
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    Iram_Buf_dram_addr = MSENSE_PAGE_2A+20;
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
}

