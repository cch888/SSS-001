/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:     gp_spin.c
*
* DESCRIPTION:  This file contains functions relating to spin-up of the currently
*               loaded media and command handling for the following ATAPI commands:
*
*               - Test Unit Ready
*               - Start Stop Unit
*               - Pause / Resume
*               - Prevent / Allow
*
*
*
*   $Revision: 448 $
*   $Date: 11/03/31 2:56p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\error.h"
#include ".\common\stdlib.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\sv_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_spin.h"
#include ".\hif\at_knl.h"
#include ".\hif\s2b_main.h"
#include ".\hif\bf_util.h"
#include ".\hif\app_if.h"
#include ".\hif\gp_util.h"
#include ".\hif\GP_RDTOC.h"
#include ".\hif\Flash.h"
#include ".\hif\diag.h"
#include ".\hif\gp_mdsns.h"//Get event status notification fail of Command set
#include ".\player\plrinfo.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvhostrd.h"
#include ".\dvd\dvinit.h"
#include ".\hif\gp_key.h"
#include ".\servo\svo_speed.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\common\constant.h"

#include ".\servo\svo_lib.h"

#if (EN_LS == 1)
#include ".\LS\ls_isr.h"
#endif // End of (EN_LS == 1)

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_def.h"
#include ".\defmgmt\common\mrw_ram.h"
extern void copy_mode5_data_to_sram(void);
#endif

#include ".\al\REG_sdram.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\REG_global.h"
#include ".\al\REG_audio.h"
#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvinit.h"
#include ".\dvd\dvdiscinit.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\common\constant.h"

#if (DPD_COMPENSATE_OFFSET==1)
#include ".\oem\coef_values.h"
#include ".\al\HAL_dsp.h"
#include ".\bd\bdDB.h"
#endif
#if (EN_POWER_CONTROL)
#include ".\hif\powerctl.h"
#endif

#include ".\bd\bdDiag.h"

#include ".\servo\svo_step.h"


extern  BYTE TextFlag;

#if (USE_TWO_KEY_FUNCTION==1)
BYTE sTrackNO, eTrackNO;
#endif

#if FLASH_BY_DISC == 1
BYTE IsFlashDisc(void);
BYTE ReadFlashDisc(void);
void UpdateFirmwareByDisc(void);
BYTE FlashByDisc_flag=0;
#endif

void  PageSize2P5K(void);

#if (ENABLE_AACS == 1)
BOOL fBypassAuth;
#endif

extern BYTE gDVDReAdjustOK;
extern BYTE synccache_process_num;
#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif
#if (CHIP_REV <= 0xB2)
extern BYTE UseC1CG1;
#endif
#if (READ_MODIFY_WRITE == 1)
extern BYTE gReadModifyWrite;
#endif
extern BYTE pre_tno; /* Check tno protect */

#if (BD_ENABLE == 1)
extern BYTE    bdVideoFlag;
#endif
extern  BYTE   Packet_Track;

#if (EN_DVD_HIGH_JITTER_DETECT  == 1)
BYTE	HIGH_JITTER_DISC = 0x02;    //Bit0:Is a high jitter Disc
                                    //Bit1:high jitter Disc should not speed up according data quantity, only set speed command(0xBB)/set streamimg command(0xB6) can speed up this Disc
#endif
#if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
extern BOOL  SpeedState;
#endif
/*--------------------------------------------------------------------
  Name: CheckForNormalVideoCD()
  Description:
    Check file structure at the LBA 0x12 and 0x13. If the "MPEGAV" and "VCD" or
    "MPEG2" and "SVCD" are present, the disc is video cd.
  Inputs:
    none
  Outputs:
    none
  Returns:
    TRUE
    FALSE
--------------------------------------------------------------------*/
vBOOL   CheckForNormalVideoCD(void)
{
#if 0
    vBYTE      check_result, i;
    vSHORT   j;
    ULONG    work_addr;

    check_result = 0;

    if(DataBuffer.CacheStartLBA == 0x0F)
    {
        for(i = 2; i <= 3; i++)
        {
            /* Check the file structure at the LBA 0x12 and LBA 0x13. */
            work_addr = DATA_START_ADDR + ( 2048 * i ) + USER_DATA_ADDR_MODE2;

            for( j = 0; j < 512; j++ )
            {
                /* Check if the MPEGAV directory is present for VIDEO CD */
                if( SCSDRAM[ work_addr     ] == 'M' )
                    if( SCSDRAM[ work_addr + 1 ] == 'P'  )
                        if( SCSDRAM[ work_addr + 2 ] == 'E' )
                            if( SCSDRAM[ work_addr + 3 ] == 'G'  )
                                if( SCSDRAM[ work_addr + 4 ] == 'A'  )
                                    if( SCSDRAM[ work_addr + 5 ] == 'V'  )
                                    {
                                        check_result |= 0x01;
                                    }
                /* Check if the VCD directory is present for VIDEO CD */
                if( SCSDRAM[ work_addr     ] == 'V' )
                    if( SCSDRAM[ work_addr + 1 ] == 'C'  )
                        if( SCSDRAM[ work_addr + 2 ] == 'D' )
                        {
                            check_result |= 0x02;
                        }
                /* Check if the MPEG2 directory is present for SUPER VIDEO CD */
                if( SCSDRAM[ work_addr     ] == 'M' )
                    if( SCSDRAM[ work_addr + 1 ] == 'P'  )
                        if( SCSDRAM[ work_addr + 2 ] == 'E' )
                            if( SCSDRAM[ work_addr + 3 ] == 'G'  )
                                if( SCSDRAM[ work_addr + 4 ] == '2'  )
                                {
                                    check_result |= 0x04;
                                }
                /* Check if the SVCD directory is present for SUPER VIDEO CD */
                if( SCSDRAM[ work_addr     ] == 'S' )
                    if( SCSDRAM[ work_addr + 1 ] == 'V'  )
                        if( SCSDRAM[ work_addr + 2 ] == 'C' )
                            if( SCSDRAM[ work_addr + 3 ] == 'D'  )
                            {
                                check_result |= 0x08;
                            }

                if( check_result == 0x03 )
                {
                    send_msg54(SEND_HIF_CMD,'V','C','D','.');
                    return TRUE;
                }

                if( check_result == 0x0C )
                {
                    send_msg54(SEND_HIF_CMD,'S','V','C','D');
                    return TRUE;
                }
                ++work_addr;
            }
        }
    }
#endif
    return FALSE;
}


/*--------------------------------------------------------------------
  Name: CheckForVideoCD()
  Description:
    Checks for VCD disc
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void    CheckForVideoCD(void)
{
BYTE result;
    result = SpecialDiscInformationParser(VideoCDInformationTable);

    if(result == eVideoCD)
    {
        if(pdbGetHighestRecordedTrkNumber() >= 2)
        {
            send_msg5SValue(SEND_HIF_CMD, "VideoCD", 1, result);
            St_cdrom_video = TRUE;
        }
    }
}


/*--------------------------------------------------------------------
  Name: TestUnitReadyCommand()
  Description:
    Processes ATAPI Test Unit Ready command to test the current status
    of the unit

    Q: Write In Process?
    Y: Iram_Sense = NOT READY - WRITE IN PROCESS
    N: Q: Existing Status?
       Y: Return existing status
       N: Q: Media Status Command?
          Y:Q: Eject Request
            Y: Iram_Sense = OPERATOR MEDIUM REMOVAL REQUEST
            N:Q: Media Change = Persist
              Y: Iram_Sense = MEDIA CHANGED
              N: No Error
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void TestUnitReadyCommand(void)
{

    if (Iram_Page_Size_3K_Flag == 1)
        PageSize2P5K();

    if (Iram_Svr_player_mode == START_UP_MODE)
    {
        BUILD_SENSE(0x00,0x00,0x00,0x0e);            /* LOGICAL UNIT READY */
        AutoCommandComplete();
        return;
    }

    if (Iram_Sense.key != 0)                            /* Return Previous Status if set */
    {
        AutoCommandComplete();
        return;
    }
    /* Check Test Unit Ready command */
    /* Standard TUR command */
    if((St_door_open == TRUE)||(St_no_disc == TRUE))
    {
        BUILD_SENSE(0x02,0x3A,0x00,0xb0); /* MEDIUM NOT PRESENT */
    }
    else if(st_disc_change ==TRUE)
    {
        BUILD_SENSE(0x06,0x28,0x00,0xb0); /* NOT READY TO READY CHANGE, MEDIUM MAY HAVE CHANGED */
    }
    #if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
    SpeedState = 0;
    #endif
    AutoCommandComplete();
}


/*******************************************************************************

    FUNCTION        InitializeCDSdramBuffer

    DESCRIPTION     Initialize the variables and registers about ring
                    buffer for CD disc

********************************************************************************/
void InitializeCDSdramBuffer(void)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_usiRingSP  = 0;
    RING_END_PAGE   = RING_END_PAGE_2p5K;
    Iram_usiRingEP  = RING_END_PAGE;
    BUF_TOTAL_BLK_COUNT = RING_END_PAGE + 1;

    WRITE_FIELD(HAL_HST_HSTBUFSPG, Iram_usiRingSP);         /* host start blk. */
    WRITE_FIELD(HAL_CDD_DSKBUFSPG, Iram_usiRingSP);         /* ring start blk. */
    WRITE_FIELD(HAL_CPYCMP_CPYSPG, Iram_usiRingSP);         /* Copy Buffer Start Page */

    WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);         /* Host end blk. */
    WRITE_FIELD(HAL_CDD_DSKBUFEPG, Iram_usiRingEP);         /* ring end blk. */
    WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);         /* Copy Buffer End Page */

    WRITE_FIELD(HAL_HST_HSTBYTAC, 0);
    WRITE_FIELD(HAL_HST_HSTBUFPG, 0);
    WRITE_FIELD(HAL_HST_BUFPGCMP, 0);

    PageSize2P5K();
    
    SET_INT_ENABLE_MASK(temp_IE);
}


/*******************************************************************************

    FUNCTION        TrayinInitial

    DESCRIPTION     On power up or a tray in spin up is initiated.  Upon a
                    successful spin up and disc detection this function
                    then determines what type of media is loaded and initializes
                    appropriate variables or functions.


********************************************************************************/
BYTE CmdSeekOk;
#if (MSG_OUT_MODE == MSG_USBPORT)
extern BYTE const Inquiry_tbl[];
#endif
extern BYTE DMD_CNT;// ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22)

extern BYTE     diaOPCMode;
#if  SERVOKEYPARAMETER == 1
extern SENSE_T KeyPara_Sense;            /* Request Sense return data */
#endif

#if (HORROR_DISC_IMPROVEMENT == 1)
BYTE    FixC2NOC1FLG;
BYTE    curC2pSec[2];
BYTE    FixADEQ;
#endif
extern ULONG SpdUpAtlowestSpdTime;

void TrayinInitial(void)
{
    ULONG * pData;
    USHORT i;
    BYTE rw_seek_retry;
    BYTE sp_md;
#if (DPD_COMPENSATE_OFFSET_DIAG==1)
    BYTE    TrackNum;
    StLongU TempL;
#endif
#if (MSG_OUT_MODE == MSG_USBPORT)
    char buf[12];
#endif
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
    USHORT uBakSvoInStateTmp;
    USHORT uBakSvoCommandTmp;
    BYTE   PreMotorDriverState;
    BYTE   PreSpindleStopFlag;
#endif
    pdbClearDiscSelect();

    pdbSetDiscKind(eUnknownKind);

    PreCurHostPtr = 0;
    Pre_Buf_hxfer_blk = 0;
#if (DVD_RAM_READ == 1)
#if (WORKAROUND_FOR_SEEDMISTHR == 1)
    ChangeSeedMisThr = 0;
    ResetSeedMisThr = 0;
    RestSeedCount = 0;
#endif
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    PrePID = 0;
    PreICEAdd = 0;
    CurICEAdd = 0;
    #endif
#endif
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)||(SEQ_SEEK_SPEED_DOWN == 1)
    RandomCount = 0;
#endif
#if (BY_PASS_C2Error_For_VCD)
    C2ErrorCount = 0;
#endif

#if (CDPlayability == 1)
    CDBufferAlmostEmpty = FALSE;
#endif
#if (CHIP_REV <= 0xB2)
    UseC1CG1 = FALSE;
#endif

    PreATAPICMD = 0x00;                         /* Initial previous ATAPI command */

    /* Initialize C1 and C2 correction */
    /* 4 symbol C2 correction if the SRCH bit isn't asserted.*/
    WRITE_REG(HAL_CDDREG_MAP->CDDSPARE, 0); //Set initial value
    EccCtrlMode(eDataCD);
#if (SUN_FUNC_TEST == 1)
    READ_C1_C2 = 0;
#endif
    
#if (HORROR_DISC_IMPROVEMENT == 1)
    FixC2NOC1FLG = 0xFF;                //initial value
    curC2pSec[0] = curC2pSec[1] = 0;    //initial value
    FixADEQ = 0xFF;
#endif
    /*-------------------------------------------------------
           Initialize system vars prior to spin up
    --------------------------------------------------------*/
    rw_seek_retry = 3;                  /* initialize retry count as 3 */
    ataPowerMode = IDLE_MODE;           /* set power_mode = IDLE_MODE */
#if (EN_POWER_CONTROL == 1)
    PowerControl(eStartUpPower);
#endif
    ClearDataBuffer();                  /* reset buffer related variables */

    #if (READ_MODIFY_WRITE == 1)
    gReadModifyWrite = FALSE;
    #endif

    /*-------------------------------------------------------
        Reset the DVD physical information page
        (becuase DVD/HD PHYSICAL FORMAT INFORMATION is a reserved
        page in CD sdram layout, reset it will not affect CD)
    --------------------------------------------------------*/
    Info.Media.DVD.PFInfoP = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS + DVD_PHYSICAL_FORMAT_INFORMATION);
    pData = (ULONG *) (SCSDRAM_ADDRESS + DVD_PHYSICAL_FORMAT_INFORMATION);
    for(i = 0; i< DRAM_PAGE_SIZE/4; i++)
        pData[i] = 0x00L;

#if (ENABLE_HDDVD==1)
    Info.Media.HD.PFInfoP = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS + HDDVD_PHYSICAL_FORMAT_INFORMATION);
    pData = (ULONG *) (SCSDRAM_ADDRESS + HDDVD_PHYSICAL_FORMAT_INFORMATION);
    for(i = 0; i< DRAM_PAGE_SIZE/4; i++)
        pData[i] = 0x00L;
#endif

    /* Initial variables, please add here for management */
    Iram_Rw_read_mode   = eUnknownMode;     /* Set the decoder mode to unkown mode */
    St_toc_ready        = FALSE;            /* mark toc not ready */
    St_CDText_ready     = FALSE;            /* mark no cd text  */
    TextFlag            = FALSE;            /* clear flag for Text CD */
    Rw_rd_monitor       = FALSE;            /* unmark read monitor */
    Rw_rdcd_cmd         = FALSE;            /* unmark read cd command */    
    Rw_mode_change      = FALSE;            /* unmark mode changed */    
    Rw_cdda_disc        = FALSE;
    Rw_subc_data        = FALSE;            /* unmark read subcode data */
    Rw_C2PO_data        = FALSE;            /* clear flag for read c2po data */
    PartialCAV_Speed    = FALSE;
    St_audio_pause      = FALSE;            /* init audio pausing flag */    
    St_method_2         = FALSE;    
    fRStart             = FALSE;
    fRdMiss             = FALSE;
    Read_subc_only      = FALSE;    
    Iram_Rd_subc_flag   = FALSE;
    Iram_Page_Size_3K_Flag  = FALSE;        /* Clear 3k page flag for C2PO */
    /* Initial variables */
    
#if MARGINAL_DISC

    #if FORCE_ATIP_TRIGGER
    CheckWGateAtip = FALSE;
    #endif

    #if CHECK_ATIP_QUALITY
    CheckAtipQuality = FALSE;
    #endif

#endif

    svo_bDiscTblNo = 0xFF;
    svo_bSMPTblNo = 0xFF;
    svo_fgDramStgyFill = FALSE;
#if (DVD_RAM_READ == 1)
    ANEW_SEEK = FALSE;
#endif
#if (EN_ADI == 1)
    StartTimer(&ADItimer);
    ADICounter = 0;
#endif


#if (MONITOR_OPU_TEMPERATURE==1)
    StartTimer(&svoReadTempTimer);          /* Init Measure Temperature Timer */
    OpuTemperature = GetOpuTemperature();
#endif /* MONITOR_OPU_TEMPERATURE=1 */

    PMANotOk = 0x00 ;                       /* for PMA Read Failed protect, flag & count initial */
    ClearDiscAttributes();                  /* Initialize disc attributes type flags */
    DecTimeCnt       = 0L;                  /* ?? */
    Iram_Diff_Time_Flag = FALSE;            /* for ATIP Time and Qcode PTime different */

    St_cdrom_video = FALSE;                 /* clear St_cdrom_video flag */
    St_blank_disc = TRUE;                   /* clear St_blank_disc flag */
    ptrReadSpeedTablePointer = 0;
    SpeedCount = 0;
    APLastAskedSpeed = 0;
    RandomRead = TRUE;
    SpeedUpAccumulation = 0;
#if (SEQ_SEEK_SPEED_DOWN == 1)
    RandomCount = 0;
#endif
    SpdUpAtlowestSpdTime = ~((ULONG)SpdChgLowest2HighTime);

    #if (ENABLE_HDDVD==1)
    RP2GN = 0;
    RP2OS = 0;
    #endif

#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
    RetryDiscLaser = eDiscUnknown;
#endif
    CmdSeekOk = 0   ;/* initial flag */

#if (BypassDecoderEr == 1)
    IGNORE_DECODER_ERROR = FALSE;
    RetryLimit=1;
#endif
    Iram_AtipAdipLppErrorCount = 0;
    Iram_ECCBlockCount = 0;

#if GET_LAST_TRACK_INFO_BASED_ON_RAI
    READ_RAI_EMPTY = TRUE;
#endif

#if (BD_ENABLE == 1)
    bdVideoFlag =  FALSE;
#endif
#if(Detect_AVCHD_DISC==1)
	AVCHDFlag = FALSE;
#endif
    dvdVideoFlag = FALSE;

    DvdVarsInit();                               /* initialize DVD variables prior to spin up */
    ENABLE_DVD_DECODER();                        /* Turn on DVD decoder */

    ENABLE_CDINT();

    ENABLE_DVD_DINT();

#if (EN_LS == 1)
    //lsReset();//need to create it on new .c file; Henry, 2008/07/14
#endif // End of (EN_LS == 1)

#if (DVD_RAM_READ == 1)
    DVDRAMVarsInit();
    WRITE_FIELD(HAL_DVDD_SEEDMISTHR, 2);
#endif

    gDVDReAdjustOK = FALSE;

#if (ENABLE_AACS == 1)
    fBypassAuth = FALSE;
#endif
#if  SERVOKEYPARAMETER == 1
    KeyPara_Sense.key = 0;
    KeyPara_Sense.cod = 0;
    KeyPara_Sense.qul = 0;
#endif

//start == Get event status notification fail of Command set
Iram_Buf_dram_addr = MSENSE_PAGE_1A + 6;
CMD_IDLE_CNT =  (USHORT)ReadSDRAM() << 8;
CMD_IDLE_CNT +=  (USHORT)ReadSDRAM();
Iram_Buf_dram_addr = MSENSE_PAGE_1A + 10;
STANDBYCNT =  (USHORT)ReadSDRAM() << 8;
STANDBYCNT +=  (USHORT)ReadSDRAM();
//end == Get event status notification fail of Command set
    /*-------------------------------------------------------
           Set LED for startup mode
    --------------------------------------------------------*/
#if (LED_NUM == 1)
    ReadyLED(LED_ON);                /* Turn On LED during Startup */
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
    ReadyLED(LED_FLASH);
#endif /* LED_NUM == 2 */

    /*-------------------------------------------------------
           Initiate spin up and disc detection
    --------------------------------------------------------*/
#if (MSG_OUT_MODE == MSG_USBPORT)
    buf[0] = '#';
    buf[1] = '#';
    buf[2] = ' ';
    buf[3] = (char)Inquiry_tbl[32];
    buf[4] = (char)Inquiry_tbl[33];
    buf[5] = (char)Inquiry_tbl[34];
    buf[6] = (char)Inquiry_tbl[35];
    buf[7] = ' ';
    buf[8] = '#';
    buf[9] = '#';
    buf[10] = 0x00;
    buf[11] = 0x00;
    SendMsgCn(SHOW_DEBUG_MSG,4,0x010100,A1B(buf[3]),A1B(buf[4]),A1B(buf[5]),A1B(buf[6]));
#endif

    TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;
    SpeedMD = StartUp_f;

#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
    for (i = 0; i < 4; i++)
    {
        if (ServoStartUp() == PASS)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x420102); //Servo OK
#if (FLASH_BY_DISC == 1)
//if flash by disc, after poweron disc should stop and tray!!!
            if(FlashByDisc_flag)
            {
                TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
                FlashByDisc_flag = 0;
                send_msg5S(1,"FlashByDisc");
                break;
            }
#endif
    #if (EN_LS == 1)
            /*-------------------------------------------------------
                   Check for ls disc
            --------------------------------------------------------*/
            if (pdbGetDiscKind() == eLSDisc)
            {
                // Initial print status
                mLSSetMediaIDValid();//need to create them on new .c files; Henry 2008/07/14
                //lsPrintInit();
                //lsStartUpSuccessful();
                return;
            }
    #endif // End of (EN_LS == 1)

    #if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
            SendMsgCn(SHOW_DEBUG_MSG,1,0x420103,pdbDiscLoaded()); //Disc Class
            St_drv_ready    = TRUE;     /* Indicate media is accessible */
            St_no_disc      = FALSE;    /* Clear no disc flag */
            st_disc_change    = TRUE;     /* Mark disc changed */
            St_become_ready = FALSE;
            return;
    #endif

            switch (pdbDiscLoaded())
            {
    #if (BD_ENABLE == 1)
                case BD_DISC:
                #if 1 //wait for modification
                    /* If physical memory is 2M, but program setting is 4M */
                    /* BD read will occur unexpected failure.              */
                    if ((READ_FIELD(HAL_GLOBAL_EMBEDMEMS) == SDRAM_M12L16161AFK1) &&
                        (SDRAM_SIZE_MBYTE == 4))
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x420104); //SDRAM CONFIG is No Good
                        break; // treat as mounting failure
                    }
                #endif
                    InitializeBdSdramBuffer();     /* initialize dynamic buffer pointer for BD disc */

                    if (BdDiscSetup() == PASS)
                    {
                        SpeedMD = AutoMode;
                        return;
                    }
                    break;
    #endif  /* BD_ENABLE */

    #if (ENABLE_HDDVD == 1)
                case HD_DISC:

                InitializeHDSdramBuffer();     /* initialize dynamic buffer pointer for HD disc */

                if (HDdvdDiscSetup() == PASS)
                {
                    send_msg5S(SEND_READ_DVD_MSG, "HDDiscSetupPASS");
                    SpeedMD = AutoMode;
                    return;
                }
                else    // FAIL status will imply that no disc is mounted
                {
                    send_msg5S(SEND_READ_DVD_MSG, "HDDiscSetupNG");
                    WRITE_FIELD(HAL_CDD_SCEN, 0);
                    WRITE_REG(HAL_CDDREG_MAP->SUBCD, HAL_CDD_SCEN);

                    /*-------------------------------------------------------
                        If reading the lead-in area fails disc will be
                        treated as if there is no disc (spin-up failed)
                    --------------------------------------------------------*/
                }
                break;
    #endif

                case DVD_DISC:
                    InitializeDVDSdramBuffer();     /* initialize dynamic buffer pointer for DVD disc */

                    if (dvdDiscSetup() == PASS)
                    {
    #if (FLASH_BY_DISC == 1)
                        if(IsFlashDisc())
                        {
                            FlashByDisc_flag = 1;

                            if(ReadFlashDisc() == PASS )
                            {
                                ServoStop();
                                UpdateFirmwareByDisc();
                            }

                            FlashByDisc_flag = 0;
                        }
                        else
    #endif
                        {
    #if (DPD_COMPENSATE_OFFSET_DIAG==1)
        #if (DVD_RAM_READ == 1)
                            if(pdbGetDiscKind() != eDVDRAMDisc)
        #endif
                            {
        #if (EN_DVD_HIGH_JITTER_DETECT  == 1)
                                JUDGE_HIGH_JITTER_DISC();
        #endif

                                TrackNum = pdbGetDVDHighestRecordedTrackNumber();
                                if(pdbGetDVDFinalSessCloseInfo() == eNotSessionClosed)
                                    TrackNum -= 1;
                                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                                if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
                                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0))+dvdConvertPSNtoLBA((pdbGetDVDMaxPSN(1)))+1;

                                if(TempL.Long == 0x22C51C)//00h 22h C5h 1Ch:Margin DVD
                                {
                                    ServoSetSpeed(MAX_READ_DVD_SPEED);        /* Set to Lowest CAV speed */
                                    //add Pre-do DPD                            calibration after change speed.
                                    ServoCalibWholeDisc();
                                }
                            }
    #endif
                        }

                        SpeedMD = AutoMode;
    #if (DVD_RAM_READ == 1)
                        if(pdbGetDiscKind() == eDVDRAMDisc)
                        {
                            ServoSetSpeed(eDVDRAMIdleSpeed);     /* Set to Lowest CLV speed */
                        }
                        else
    #endif
                        {
    #if (DPD_COMPENSATE_OFFSET_DIAG==1)
                            if(TempL.Long != 0x22C51C)
    #endif
                            {
    #if (STARTUP_AT_LOWEST_CAV == 1)
            #if (Detect_AVCHD_DISC == 1)
                                if (AVCHDFlag == TRUE)
                                {
                                    ServoSetSpeed(MAX_READ_AVCHD_SPEED);
                                }
                                else
            #endif
                                {
                                    ServoSetSpeed(eDVDIdleSpeed);        /* Set to Lowest CAV speed */
                                }
    #else
            #if (Detect_AVCHD_DISC == 1)
                                if (AVCHDFlag == TRUE)
                                {
                                    ServoSetSpeed(MAX_READ_AVCHD_SPEED);
                                }
                                else
            #endif
                                {
                                    ServoSetSpeed(eDVDSpinUpSpeed);
                                }
    #endif
                            }
                        }
                        dvdStartUpSuccessful();  /* Completes successful start up initializations */
                        return;
                    }
                    else    // FAIL status will imply that no disc is mounted
                    {
                        if (gDVDReAdjustOK == TRUE)
                        {
                            gDVDReAdjustOK = FALSE;
                            if (dvdDiscSetup() == PASS)
                            {
                                #if (FLASH_BY_DISC == 1)
                                if(IsFlashDisc())
                                {
                                    FlashByDisc_flag = 1;

                                    if (ReadFlashDisc() == PASS )
                                    {
                                        ServoStop();
                                        UpdateFirmwareByDisc();
                                    }
                                    FlashByDisc_flag = 0;
                                }
                                #endif

                                SpeedMD = AutoMode;
                                dvdStartUpSuccessful();  /* Completes successful start up initializations */
                                return;
                            }
                        }

                        ptrReadSpeedTablePointer = 0;

                        WRITE_FIELD(HAL_CDD_SCEN, 0);   /* Disable subcode logic for CD decoder */
                        WRITE_REG(HAL_CDDREG_MAP->SUBCD, HAL_CDD_SCEN);

                        /*-------------------------------------------------------
                            If reading the lead-in area fails disc will be
                            treated as if there is no disc (spin-up failed)
                        --------------------------------------------------------*/
                    }
                    break;

                case CD_DISC:
                    WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x18);//Set RXFIFO, RBSIZE 64 bytes
                    Packet_Track = TRKNO_INITVALID;    //  modify for read Q time out

                    InitializeCDSdramBuffer();      /* initialize dynamic buffer pointer for CD disc */
                    // CD disc is detected
                    DISABLE_DVD_DECODER();

    #if (STARTUP_AT_LOWEST_CAV == 1)
                    ServoSetSpeed(eCDIdleSpeed);        /* Set to Lowest CAV speed */
    #else
                    ServoSetSpeed(eCDSpinUpSpeed);    /* Sync Servo CD Start Up Speed */
    #endif

                    UpdateDiscInfoData();               /* init disc type, disc size ... *//* 8cm write speed limitation */

                    /* The macro below was defined from ATIP info, for a Reader_Only project, */
                    /* we need to re-define it here. */
                    pdbGetCDRW() = (pdbGetDiscKind() == eCDRWDisc) ? TRUE:FALSE;

                    #if (USE_TWO_KEY_FUNCTION==1)
                    sTrackNO = eTrackNO = 0;
                    #endif

                    if(St_blank_disc == TRUE)
                    {
                        GetCapacity();                              /* ret maxlba=cd_rom capacity */
                        St_toc_ready = FALSE;
                        StartUpSuccessful();

                        st_disc_change = TRUE;                      /* Mark cd changed */
#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
                    if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
                    {
                        if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
                        {
                            SATA_intf_pwr_state = 0;
                            WRITE_FIELD(HAL_HST_DWCLKON, 1);
                            WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                            DelaymS(1);
                        }
                         if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                         {
                            WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
                            WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
                         }
                    }
#endif
                        ataMediaIsChanged = TRUE;                   /* for ATA command ataGetMediaStatus() in at_cmds.c */
                        St_become_ready = FALSE;

                        SpeedMD = AutoMode;
                        return;
                    }
                    else
                    {
                        UpdateTOCData();                  /* Update toc data for all formats */
                        StoreTOCData();                   /* Store toc data to shadow DB */

                        #if (FLASH_BY_DISC == 1)
                        if(IsFlashDisc())
                        {
                            FlashByDisc_flag = 1;

                            if (ReadFlashDisc() == PASS )
                            {
                                ServoStop();
                                UpdateFirmwareByDisc();
                            }

                            FlashByDisc_flag = 0;
                        }
                        #endif

                        sp_md = UpdateDiscTypeSpeed();

                        GetCapacity();                          /* ret maxlba=cd_rom capacity */
                        StartUpSuccessful();
                        DMD_CNT=0;       // ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22)
                        st_disc_change = TRUE;                  /* Mark cd changed */
#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
                        if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
                        {
                            if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
                            {
                                SATA_intf_pwr_state = 0;
                                WRITE_FIELD(HAL_HST_DWCLKON, 1);
                                WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                                DelaymS(1);
                            }
                            if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                            {
                                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
                                WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
                            }
                        }
#endif
                        ataMediaIsChanged = TRUE;               /* for ATA command ataGetMediaStatus() in at_cmds.c */
                        St_become_ready = FALSE;

                        St_audio_complete = FALSE;

                        SpeedMD = sp_md;

                        #if (USE_TWO_KEY_FUNCTION==1)
                        {
                            BYTE i, trackNO;

                            trackNO = pdbGetHighestRecordedTrkNumber();
                            for(i=1; i<=trackNO; i++)
                            {
                                if (!((pdbGetTrackContAddr(i) & 0xF0) == 0x40))
                                {
                                    if (sTrackNO == 0)
                                            sTrackNO = eTrackNO = i;
                                    else
                                            eTrackNO = i;
                                }
                            }
                        }
                        #endif  /* USE_TWO_KEY_FUNCTION */

                        return;
                    }
                    /* start_up_successful is called also from Knl, so replaced with a routine */
                    break;
            }

            /* when non-empty disc is inserted, activate read ahead. */
            if(St_blank_disc == FALSE)
                dvdDecFlag.fDISABLE_READ_AHEAD = CLEAR;
        }
        else /* send read dvd msg */
        {
				#if (Customer_A1 ==1)
					if(((svoVar_discDiameter==e12cm)||(svoVar_discDiameter==e8cm))&&(svoVar_discKindT.Bit.DiscStd == eDiscUnknown))
					{
					///SendMsgCnB(SHOW_DEBUG_MSG,4,0x90,0x00,0x00,B4B(i));///display 16
					}
					else
					{
				#endif        
            send_msg80(1,0x710040);
            SpeedMD = Di_f;

            rw_seek_retry =0;
            while(ServoStop()==FAIL)
            {
                rw_seek_retry++;
                if((i==3) && (svoIramVar.knlTimeupCount == 0)) svoIramVar.knlTimeupCount = 100;
                if (rw_seek_retry > 2) break;
            }
				#if (Customer_A1 ==1)	
						if(CompleteStroke)
						{}
						else
						{
            	if ((EjectKeyInfo.PressedValid) || ((Iram_Cmd_block.bf[0] == SBC_START_STOP_UNIT) && (Iram_Cmd_block.bf[4] == 0x02)))
                break;
						}
				#else
            if ((EjectKeyInfo.PressedValid) || ((Iram_Cmd_block.bf[0] == SBC_START_STOP_UNIT) && (Iram_Cmd_block.bf[4] == 0x02)))
                break;
				#endif			


    #if (EN_POWER_CONTROL == 1)
            PowerControl(eIdlePower);
    #endif

            PreMotorDriverState = svo_Var_motorDriverState;
            PreSpindleStopFlag = svoIramVar.spindleStopFlag;

		#if (Customer_A1 ==1)
///		SendMsgCnB(SHOW_DEBUG_MSG,4,0x90,0x00,0x00,B4B(350));///display 16
		#else
            Iram_stp_Initialized = STEP_UNINITIALIZED;
		#endif	

            pdbSetDiscKind(eUnknownKind);
            svo_Adj_Data_Clr();

            if (RetryDiscLaser != eDiscUnknown)
            {
                vWakeUpFromPowerSaveMode();

                uBakSvoInStateTmp = svoIramVar.svoInState;
                uBakSvoCommandTmp = svoCmdVar.svoCommand;
                svoPowerOnInit();
                svoSetLDOff();
                svoIramVar.svoInState = uBakSvoInStateTmp;
                svoCmdVar.svoCommand  = uBakSvoCommandTmp;
            }

            svo_Var_motorDriverState = PreMotorDriverState;
            svoIramVar.spindleStopFlag = PreSpindleStopFlag;

            if (RetryDiscLaser == eDiscUnknown)
            {//eDiscUnknown indicates don't need retry
                i=5;
                break;
            }
            else if(i>0)
            {
                switch (RetryDiscLaser)
                {
                    case eDiscBD: //BD
                        RetryDiscLaser = eDiscDVD; //DVD
                        break;
                    case eDiscDVD: //DVD
                        RetryDiscLaser = eDiscCD; //CD
                        break;
                    case eDiscCD: //CD
                        RetryDiscLaser = eDiscBD; //BD
                        break;
                    default:
                        RetryDiscLaser = eDiscUnknown;
                        i=5;
                        break;
                }
            }
#if (Customer_A1 ==1)
        }
#endif
        }
    }
#else//#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
    if(ServoStartUp() == PASS)
    {
        send_msg80(1,0x71003E);

#if (EN_LS == 1)
        /*-------------------------------------------------------
               Check for ls disc
        --------------------------------------------------------*/
        if (pdbGetDiscKind() == eLSDisc)
        {
            // Initial print status
            mLSSetMediaIDValid();//need to create them on new .c files; Henry 2008/07/14
            //lsPrintInit();
            //lsStartUpSuccessful();
            return;
        }
#endif // End of (EN_LS == 1)

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
        send_msg5SValue(1, "Disc Class", 1, pdbDiscLoaded());
        St_drv_ready    = TRUE;     /* Indicate media is accessible */
        St_no_disc      = FALSE;    /* Clear no disc flag */
        st_disc_change    = TRUE;     /* Mark disc changed */
        St_become_ready = FALSE;
        return;
#endif

        switch (pdbDiscLoaded())
        {
#if (BD_ENABLE == 1)
            case BD_DISC:
            #if 1 //wait for modification
                /* If physical memory is 2M, but program setting is 4M */
                /* BD read will occur unexpected failure.              */
                if ((READ_FIELD(HAL_GLOBAL_EMBEDMEMS) == SDRAM_M12L16161AFK1) &&
                    (SDRAM_SIZE_MBYTE == 4))
                {
                    send_msg80(1,0x71003F);
                    break; // treat as mounting failure
                }
            #endif
                InitializeBdSdramBuffer();     /* initialize dynamic buffer pointer for BD disc */

                if (BdDiscSetup() == PASS)
                {
                    SpeedMD = AutoMode;
                    return;
                }
                break;
#endif  /* BD_ENABLE */

#if (ENABLE_HDDVD == 1)
            case HD_DISC:

            InitializeHDSdramBuffer();     /* initialize dynamic buffer pointer for HD disc */

            if (HDdvdDiscSetup() == PASS)
            {
                send_msg5S(SEND_READ_DVD_MSG, "HDDiscSetupPASS");
                SpeedMD = AutoMode;
                return;
            }
            else    // FAIL status will imply that no disc is mounted
            {
                send_msg5S(SEND_READ_DVD_MSG, "HDDiscSetupNG");
                WRITE_FIELD(HAL_CDD_SCEN, 0);
                WRITE_REG(HAL_CDDREG_MAP->SUBCD, HAL_CDD_SCEN);

                /*-------------------------------------------------------
                    If reading the lead-in area fails disc will be
                    treated as if there is no disc (spin-up failed)
                --------------------------------------------------------*/
            }
            break;
#endif

            case DVD_DISC:
                InitializeDVDSdramBuffer();     /* initialize dynamic buffer pointer for DVD disc */

                if (dvdDiscSetup() == PASS)
                {
#if (FLASH_BY_DISC == 1)
                    if(IsFlashDisc())
                    {
                        FlashByDisc_flag = 1;

                        if(ReadFlashDisc() == PASS )
                        {
                            ServoStop();
                            UpdateFirmwareByDisc();
                        }

                        FlashByDisc_flag = 0;
                    }
                    else
#endif
                    {
#if (DPD_COMPENSATE_OFFSET_DIAG==1)
    #if (DVD_RAM_READ == 1)
                        if(pdbGetDiscKind() != eDVDRAMDisc)
    #endif
                        {
    #if (EN_DVD_HIGH_JITTER_DETECT  == 1)
                           JUDGE_HIGH_JITTER_DISC();
    #endif

                            TrackNum = pdbGetDVDHighestRecordedTrackNumber();
                            if(pdbGetDVDFinalSessCloseInfo() == eNotSessionClosed)
                                TrackNum -= 1;
                            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                            if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
                                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0))+dvdConvertPSNtoLBA((pdbGetDVDMaxPSN(1)))+1;

                            if(TempL.Long == 0x22C51C)//00h 22h C5h 1Ch:Margin DVD
                            {
                                ServoSetSpeed(MAX_READ_DVD_SPEED);          /* Set to Lowest CAV speed */
                                ServoCalibWholeDisc();
                            }
                        }
#endif
                    }

                    SpeedMD = AutoMode;
#if (DVD_RAM_READ == 1)
                    if(pdbGetDiscKind() == eDVDRAMDisc)
                    {
                        ServoSetSpeed(eDVDRAMIdleSpeed);     /* Set to Lowest CLV speed */
                    }
                    else
#endif
                    {
#if (DPD_COMPENSATE_OFFSET_DIAG==1)
                        if(TempL.Long != 0x22C51C)
#endif
                        {
#if (STARTUP_AT_LOWEST_CAV == 1)
            #if (Detect_AVCHD_DISC == 1)
                            if (AVCHDFlag == TRUE)
                            {
                                ServoSetSpeed(MAX_READ_AVCHD_SPEED);
                            }
                            else
            #endif
                            {
                                ServoSetSpeed(eDVDIdleSpeed);        /* Set to Lowest CAV speed */
                            }
#else
            #if (Detect_AVCHD_DISC == 1)
                            if (AVCHDFlag == TRUE)
                            {
                                ServoSetSpeed(MAX_READ_AVCHD_SPEED);
                            }
                            else
            #endif
                            {
                                ServoSetSpeed(eDVDSpinUpSpeed);
                            }
#endif
                        }
                    }
                    dvdStartUpSuccessful();  /* Completes successful start up initializations */
                    return;
                }
                else    // FAIL status will imply that no disc is mounted
                {
                    if (gDVDReAdjustOK == TRUE)
                    {
                        gDVDReAdjustOK = FALSE;
                        if (dvdDiscSetup() == PASS)
                        {
                            #if (FLASH_BY_DISC == 1)
                            if(IsFlashDisc())
                            {
                                FlashByDisc_flag = 1;

                                if (ReadFlashDisc() == PASS )
                                {
                                    ServoStop();
                                    UpdateFirmwareByDisc();
                                }
                                FlashByDisc_flag = 0;
                            }
                            #endif

                            SpeedMD = AutoMode;
                            dvdStartUpSuccessful();  /* Completes successful start up initializations */
                            return;
                        }
                    }

                    ptrReadSpeedTablePointer = 0;

                    WRITE_FIELD(HAL_CDD_SCEN, 0);   /* Disable subcode logic for CD decoder */
                    WRITE_REG(HAL_CDDREG_MAP->SUBCD, HAL_CDD_SCEN);

                    /*-------------------------------------------------------
                        If reading the lead-in area fails disc will be
                        treated as if there is no disc (spin-up failed)
                    --------------------------------------------------------*/
                }
                break;

            case CD_DISC:
                WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x18);//Set RXFIFO, RBSIZE 64 bytes

                InitializeCDSdramBuffer();      /* initialize dynamic buffer pointer for CD disc */
                // CD disc is detected
                DISABLE_DVD_DECODER();

#if (STARTUP_AT_LOWEST_CAV == 1)
                ServoSetSpeed(eCDIdleSpeed);        /* Set to Lowest CAV speed */
#else
                ServoSetSpeed(eCDSpinUpSpeed);    /* Sync Servo CD Start Up Speed */
#endif

                UpdateDiscInfoData();               /* init disc type, disc size ... *//* 8cm write speed limitation */

                /* The macro below was defined from ATIP info, for a Reader_Only project, */
                /* we need to re-define it here. */
                pdbGetCDRW() = (pdbGetDiscKind() == eCDRWDisc) ? TRUE:FALSE;

                #if (USE_TWO_KEY_FUNCTION==1)
                sTrackNO = eTrackNO = 0;
                #endif

                if(St_blank_disc == TRUE)
                {
                    GetCapacity();                              /* ret maxlba=cd_rom capacity */
                    St_toc_ready = FALSE;
                    StartUpSuccessful();

                    st_disc_change = TRUE;                      /* Mark cd changed */
#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
                        if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
                        {
                            if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
                            {
                                SATA_intf_pwr_state = 0;
                                WRITE_FIELD(HAL_HST_DWCLKON, 1);
                                WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                                DelaymS(1);
                            }
                            if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                            {
                                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
                                WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
                            }
                        }
#endif
                    ataMediaIsChanged = TRUE;                   /* for ATA command ataGetMediaStatus() in at_cmds.c */
                    St_become_ready = FALSE;

                    SpeedMD = AutoMode;
                    return;
                }
                else
                {
                    UpdateTOCData();                  /* Update toc data for all formats */
                    StoreTOCData();                   /* Store toc data to shadow DB */

                    #if (FLASH_BY_DISC == 1)
                    if(IsFlashDisc())
                    {
                        FlashByDisc_flag = 1;

                        if (ReadFlashDisc() == PASS )
                        {
                            ServoStop();
                            UpdateFirmwareByDisc();
                        }

                        FlashByDisc_flag = 0;
                    }
                    #endif

                    sp_md = UpdateDiscTypeSpeed();

                    GetCapacity();                          /* ret maxlba=cd_rom capacity */
                    StartUpSuccessful();

                    st_disc_change = TRUE;                  /* Mark cd changed */
#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
                        if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
                        {
                            if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
                            {
                                SATA_intf_pwr_state = 0;
                                WRITE_FIELD(HAL_HST_DWCLKON, 1);
                                WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                                DelaymS(1);
                            }
                            if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                            {
                                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
                                WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
                            }
                        }
#endif
                    ataMediaIsChanged = TRUE;               /* for ATA command ataGetMediaStatus() in at_cmds.c */
                    St_become_ready = FALSE;

                    St_audio_complete = FALSE;

                    SpeedMD = sp_md;

                    #if (USE_TWO_KEY_FUNCTION==1)
                    {
                        BYTE i, trackNO;

                        trackNO = pdbGetHighestRecordedTrkNumber();
                        for(i=1; i<=trackNO; i++)
                        {
                            if (!((pdbGetTrackContAddr(i) & 0xF0) == 0x40))
                            {
                                if (sTrackNO == 0)
                                    sTrackNO = eTrackNO = i;
                                else
                                    eTrackNO = i;
                            }
                        }
                    }
                    #endif  /* USE_TWO_KEY_FUNCTION */

                    return;
                }
                /* start_up_successful is called also from Knl, so replaced with a routine */
                break;
        }

        /* when non-empty disc is inserted, activate read ahead. */
        if(St_blank_disc == FALSE)
            dvdDecFlag.fDISABLE_READ_AHEAD = CLEAR;
    }
    else /* send read dvd msg */
    {
        send_msg80(1,0x710040);
        SpeedMD = Di_f;
    }
#endif//#if (DISC_MIS_JUDGE_START_UP_RECOVERY)

#if (DEBUG_EJECT_ON_BLANK_DISK == 1)
    #if 1
//Method1
    if ((HIF.PlayerStatus == Error) &&
       (HIF.PlayerParameters[3] == eErrorUnknownDisc))
    {
      #if (SET_SLOT_IN == 1)
			if(EjectKeyInfo.Pressed) //[BTC][Q36]disc in/out continue issue
			{
				while(1)
				{
					if(!EjectKeyInfo.Pressed)
						break;
				}
			}
      #endif		 
        svoIramVar.emergencyStopFlag = TRUE;
        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
    }
    #endif

    #if 0
//Method2
    if ((HIF.PlayerStatus == Error) &&
       (HIF.PlayerParameters[3] == eErrorUnknownDisc))
    {
        St_door_open = FALSE;
        ServoTrayOut();
    }
    #endif
#endif //(DEBUG_EJECT_ON_BLANK_DISK == 1)

    /*-------------------------------------------------------
           Spin-up failed. Treat as no disc loaded
    --------------------------------------------------------*/
    St_become_ready = FALSE;                /* mark not becomig ready */
    St_drv_ready    = FALSE;                /* drive is not ready */
    St_no_disc      = TRUE;                 /* mark no disc */

    ServoStop();                            /* stop servo and turn LD OFF, for protected */
#if (STORE_MSG_TO_FLASHROM==1)
    if(( pdbServoGetDiscDiameter() == e12cm )||( pdbServoGetDiscDiameter() == e8cm ))
    {
        if(EnableAutoSaveLog == TRUE)
        {
            SaveLogToROMSwitch = TRUE;//CopyMsgFromDataAreaToROM();
        }
    }
#endif
#if (EN_POWER_CONTROL == 1)
    PowerControl(eStandbyPower);
#endif

    ReadyLED(LED_OFF);

    St_load_disc = FALSE;
}


/*--------------------------------------------------------------------
  Name: StartStopUnitCommand()
  Description:
    Processes ATAPI Start/Stop Unit command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
extern BOOL ResetDecoder;
void StartStopUnitCommand(void)
{
#define Start bit0
#define LoEj bit1

    BYTE immediate;
    WORD DiscKind;//FWtester2  Item13 No.6 DVD+RW compare error

    EjectKeyInfo.Disable = TRUE;// disable eject-key to avoid error.
    immediate = Iram_Cmd_block.bf[1] & bit0;

    switch(Iram_Cmd_block.bf[4] & (LoEj | Start))// ignore other bits
    {
        case 0:
            if(St_door_open)
            {/* Tray is out */
                BUILD_SENSE(0x02,0x3a,SQUAL_TRAY_OPEN,0x1c);// MEDIUM NOT PRESENT - TRAY OPEN
            }
            else if(St_no_disc == TRUE)
            {
                BUILD_SENSE(0x02,0x3a,SQUAL_TRAY_CLOSED,0x1d);// MEDIUM NOT PRESENT - TRAY CLOSED
            }
            else if(St_become_ready == TRUE)
            {
                BUILD_SENSE(0x02,0x04,0x01,0x1e);// LOGICAL UNIT IS IN PROCESS OF BECOMING READY
            }
            else
            {
                dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;// disable DVD read ahead.
                if(READ_PI_PO == 1)
                {
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    ResetDecoder = FALSE;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                }
#if (BD_ENABLE == 1)
                if(READ_LDC_BIS == 1)
                {
                    HAL_BD_CODEC_Disable_DiscardIce();
                    READ_LDC_BIS = 0;
                    Send_timef = 0;
                    ResetDecoder = FALSE;
                    bdStopDiscTransfer();
                    bdResetDiscFlags();
                }
#endif
                if(ServoStop() == PASS)
                {
                    TrayManagerInfo.EjectRequestEvent = FALSE;
                }
                else
                {
                    BUILD_SENSE(0x04,0x53,0x00,0x0e);// MEDIA LOAD or EJECT FAILED
                }
            }
        break;

        case Start:
            if(St_door_open)      /* Tray is out */
            {
                BUILD_SENSE(0x02,0x3a,SQUAL_TRAY_OPEN,0x1e);// MEDIUM NOT PRESENT - TRAY OPEN
            }
            else if(St_no_disc == TRUE)
            {
                BUILD_SENSE(0x02,0x3a,SQUAL_TRAY_CLOSED,0x1f);// MEDIUM NOT PRESENT - TRAY CLOSED
            }
            else
            {
                if(St_become_ready == FALSE)
                {
#if WITHOUT_SERVO == 0
                    if((St_no_disc == FALSE) && (ataPowerMode == STANDBY_MODE))
                    {
                        DiscKind = pdbGetDiscKind();//FWtester2  Item13 No.6 DVD+RW compare error

                        if (ServoStartUp() == PASS)
                        {
                            pdbSetDiscKind(DiscKind);//FWtester2  Item13 No.6 DVD+RW compare error

                            switch (pdbDiscLoaded())
                            {
    #if (BD_ENABLE == 1)
                                case BD_DISC:
                                    SetReadSpeed(eBDSpinUpSpeed);   //sync SVO speed
                                    break;
    #endif
    #if (ENABLE_HDDVD == 1)
                                case HD_DISC:
                                    break;
    #endif
                                case DVD_DISC:
    #if (DVD_RAM_READ == 1)
                                    if(pdbGetDiscKind() == eDVDRAMDisc)
                                    {
                                        ServoSetSpeed(eDVDRAMIdleSpeed);     /* Set to Lowest CLV speed */
                                    }
                                    else
    #endif
                                    {
    #if (STARTUP_AT_LOWEST_CAV == 1)
        #if (Detect_AVCHD_DISC == 1)
                                        if (AVCHDFlag == TRUE)
                                        {
                                            SetReadSpeed(MAX_READ_AVCHD_SPEED);
                                        }
                                        else
        #endif
                                        {
                                            SetReadSpeed(eDVDIdleSpeed);
                                        }
    #else
        #if (Detect_AVCHD_DISC == 1)
                                        if (AVCHDFlag == TRUE)
                                        {
                                            SetReadSpeed(MAX_READ_AVCHD_SPEED);
                                        }
                                        else
        #endif
                                        {
                                            SetReadSpeed(eDVDSpinUpSpeed);      //sync SVO speed
                                        }
    #endif
                                    }
                                    break;
                                case CD_DISC:
    #if (STARTUP_AT_LOWEST_CAV == 1)
                                    ServoSetSpeed(eCDIdleSpeed);        /* Sync Servo CD Start Up Speed */
    #else
                                    ServoSetSpeed(eCDSpinUpSpeed);      /* Sync Servo CD Start Up Speed */
    #endif
                                    break;
                            }
                        }
                    }
#else /* WITHOUT_SERVO */
                    St_become_ready = FALSE;
                    St_drv_ready  = TRUE;
                    Iram_Svr_player_mode = START_UP_MODE;
                    Maxlba = 0x10000;
#endif /* WITHOUT_SERVO */
                }

            }
        break;

        case LoEj:
            if(TrayManagerInfo.TrayPreventAllowState & TRAY_PREVENT_ALLOW_STATE_LOCKED)
            {
                BUILD_SENSE(0x05,0x53,0x02,0x48);// MEDIUM REMOVAL PREVENTED
            }
            else if(TrayManagerInfo.TrayState == TRAY_STATE_OUT_EXECUTING || TrayManagerInfo.TrayState == TRAY_STATE_OUT_OK)
            {
                break;
            }
            else
            {
                svoIramVar.emergencyStopFlag = TRUE;
            #if (SET_SLOT_IN == 1)
				   if(EjectKeyInfo.Pressed) //[BTC][Q36]disc in/out continue issue
				   {
					   while(1)
					   {
						if(!EjectKeyInfo.Pressed)
						break;
					   }
					}
            #endif	
                TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;// StartStopUnitCommand has the highest priority to TrayManager(), so it set tray-command directly

                if(immediate == FALSE)
                {
                    while(TrayManagerInfo.Executing)
                        ExitProcess();

                    TrayManager();

                    if(TrayManagerInfo.TrayState != TRAY_STATE_OUT_OK)
                        BUILD_SENSE(0x04,0x53,0x00,0x0f);// MEDIUM LOAD MECHANISM FAILED
                }
            }
        break;

        case (LoEj | Start):
            if(TrayManagerInfo.TrayState == TRAY_STATE_IN_EXECUTING || TrayManagerInfo.TrayState == TRAY_STATE_IN_OK)
                break;

            svoIramVar.emergencyStopFlag = TRUE;
            TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;// StartStopUnitCommand has the highest priority to TrayManager(), so it set tray-command directly

            if(immediate == FALSE)
            {
#if WITHOUT_SERVO == 0
                while(TrayManagerInfo.Executing)
                    ExitProcess();

                TrayManager();
                if(TrayManagerInfo.TrayState != TRAY_STATE_IN_OK)
                    BUILD_SENSE(0x04,0x53,0x00,0x10);// MEDIUM LOAD MECHANISM FAILED
#else /* WITHOUT_SERVO */
                St_become_ready = FALSE;
                St_drv_ready  = TRUE;
                Iram_Svr_player_mode = START_UP_MODE;
                Maxlba = 0x10000;
#endif /* WITHOUT_SERVO */
            }
        break;

        default:
            BUILD_SENSE(0x05,0x20,0x00,0x09);// INVALID COMMAND OP CODE
        break;
    }

    if((TrayManagerInfo.TrayPreventAllowState & TRAY_PREVENT_ALLOW_STATE_LOCKED) == 0)
        EjectKeyInfo.Disable = FALSE;
#if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
     SpeedState = 0;
#endif
    AutoCommandComplete();
}

/*--------------------------------------------------------------------
  Name: PauseResumeCommand()
  Description:
    Processes ATAPI Pause/Resume command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
#define OVERRUN_PAGE        0x60                        /* over run page numbers = 3 */
#define UNDERRUN_PAGE       0x20                        /* underrun page numbers = 1 */

void PauseResumeCommand(void)
{
    vBOOL temp_IE;
    USHORT subq_ptr;
    TimeS   seekTarget;
    ULONG   work_addr;

    if ( St_nx_audio != TRUE){
        AutoCommandComplete();
        return;
    }

    if ( !TEST_BIT(Iram_Cmd_block.bf[8],0) )                    /* if pause command */
    {
/* pause command */
        if ( !St_audio_pause )
        {
            StopDecoder();
#if (AUDIO_PLAYBACK == 1)
            AudioTransferDone();
#endif
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

#if (AUDIO_PLAYBACK == 1)
            WRITE_FIELD(HAL_AUD_APLAYEN, 0);  // Disable Nx audio
            WRITE_FIELD(HAL_AUD_A1KAUX , 0);  // Selects Aux = 512
            WRITE_FIELD(HAL_AUD_ABLKIEN, 0);  /* Disable Audio Transfer End Interrupt */
            CLEAR_INT_FIELD(HAL_AUD_ABLKINT, 0);  /* Clear Audio Transfer End Status */
            subq_ptr = READ_REG(HAL_AUDREG_MAP->ACURPGL);

            BHIGH(subq_ptr) = (BHIGH(subq_ptr) & 0x1F) ;
#endif

            work_addr = PAGE_AUX_START_ADDR(subq_ptr);
            seekTarget.Minute = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 7]); /* read rel min */
            seekTarget.Second = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 8]); /* read rel min */
            seekTarget.Frame  = BCD2HEX(SCSDRAM[work_addr + AUX_SUBQDATA_ADDR + 9]); /* read rel min */

            SET_INT_ENABLE_MASK(temp_IE);

            Rw_seek_blk.msf.Minute = seekTarget.Minute; /* read rel min */
            Rw_seek_blk.msf.Second = seekTarget.Second; /* read rel sec */
            Rw_seek_blk.msf.Frame  = seekTarget.Frame;  /* read rel frame */

            Iram_audio_scan_st = FALSE;
            St_nx_audio = FALSE;
            pre_tno = 0xff; /* Check tno protect */
            ServoPause();
            Iram_Knl_save_counter = 900;   /* continue the disc's rotation about 1m40s */
            St_nx_audio = TRUE;
            svoIramVar.subQEnableFlag = 1;

            St_audio_pause = TRUE;
            Iram_Svr_player_mode = AUDIO_PAUSE_MODE;            /* change mode */
            Svr_pause_reqire = FALSE;
        }
        AutoCommandComplete();
    }
#if (AUDIO_PLAYBACK == 1)
    else
    {
/* Play command */
        if (( St_audio_pause) || (Iram_audio_scan_st == TRUE))  /* Check if in pause mode*/
        {
            if ( Iram_Svr_player_mode == STOP_SCAN_MODE)    /* if stop/scan */
            {
                /* stop/scan mode has been entered, it is not allowed to perform
                resume in this mode */
                AutoCommandComplete();
                return;
            }

            St_audio_pause = FALSE;
            if (Iram_audio_scan_st == TRUE){
                Rw_seek_blk.msf = LBA2MSF(Buf_scan_lba);
            }
            Iram_Cmd_block.bf[3] = Rw_seek_blk.msf.Minute;
            Iram_Cmd_block.bf[4] = Rw_seek_blk.msf.Second;
            Iram_Cmd_block.bf[5] = Rw_seek_blk.msf.Frame;

            if ( Iram_audio_scan_st == TRUE ) {
                Iram_audio_scan_st = FALSE;
                /* If current address is greater than the former stop address,
                   the Logical Unit shall stop playing and return good status. */
                if ( Buf_scan_lba < PreCmd_endlba ) {
                    Iram_Cmd_block.bf[6] = (BYTE)(PreCmd_endlba >> 16) & 0xFF;
                    Iram_Cmd_block.bf[7] = (BYTE)(PreCmd_endlba >>  8) & 0xFF;
                    Iram_Cmd_block.bf[8] = (BYTE)PreCmd_endlba & 0xFF;
                    Tg_endlba = PreCmd_endlba;          /* load previous Cmd requested LBA */
                } else {
                    StopPlayScanCommand();
                    return;
                }
            }
            else
            {

                Iram_Buf_dram_addr = AUDIO_END_HI + AUDIO_END_MIN;   /* audio end blk (min) dram addr lo */
                Iram_Cmd_block.bf[6] = ReadSDRAM();
                Iram_Cmd_block.bf[7] = ReadSDRAM();
                Iram_Cmd_block.bf[8] = ReadSDRAM();
            }
            Play_Cmd_block = Iram_Cmd_block;
            EnterNxTo1x();
            return;
        }
        AutoCommandComplete();
    }
#endif
}


/*--------------------------------------------------------------------
  Name: PreventAllowMediumRemovalCommand()
  Description:
    Processes ATAPI Prevent/Allow command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void PreventAllowMediumRemovalCommand(void)
{
    TrayManagerInfo.EjectRequestEvent = FALSE;
    TrayManagerInfo.TrayPreventAllowState = TRAY_PREVENT_ALLOW_STATE_UNLOCKED;
    EjectKeyInfo.Disable = FALSE;
    if((Iram_Cmd_block.bf[1]&0x1F)==0x00)// for win98/me don't locked at PC power reset.
    {
        if (TEST_BIT(Iram_Cmd_block.bf[4],0))
        {
            TrayManagerInfo.TrayPreventAllowState = TRAY_PREVENT_ALLOW_STATE_LOCKED;
            EjectKeyInfo.Disable = TRUE;
        }
        else
        {
    #if 0  // following code will be used in th future. Please DON"T remove them!!!
    #if(CEProduct == 0)
            /*    For IT product: For change speed control from write to read.
                It will be adjust in the future. */
    #endif
    #endif
        }

        Iram_Buf_dram_addr = GESN_OPECHGREQ_ADR + 5;

        /* Update Persistent Prevent bit of GESN Operational Class Event */
        if(TEST_BIT(Iram_Cmd_block.bf[4],1) && TrayManagerInfo.TrayPreventAllowState == TRAY_PREVENT_ALLOW_STATE_LOCKED)
        {
            WriteSDRAM( 0x80 );    /* Persistent Prevent = 1 */
            TrayManagerInfo.TrayPreventAllowState = TRAY_PREVENT_ALLOW_STATE_PERSISTERN_PREVENT;
            //start == Get event status notification fail of Command set
            Iram_Buf_dram_addr = GESN_EXTERNAL_ADR + 5;
            WriteSDRAM( 0x80 );    /* Persistent Prevent = 1 */
            //end == Get event status notification fail of Command set
        }
        else
        {
            WriteSDRAM( 0x00 );    /* Persistent Prevent = 0 */
            //start == Get event status notification fail of Command set
            Iram_Buf_dram_addr = GESN_EXTERNAL_ADR + 5;
            WriteSDRAM( 0x00 );    /* Persistent Prevent = 0 */
            //end == Get event status notification fail of Command set
        }
    }
    AutoCommandComplete();
}


/*--------------------------------------------------------------------
  Name: StartUpSuccessful()
  Description:
    Seeks to 0:2:0 and initializes other variables depending of
    if a disc is present.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void StartUpSuccessful(void)
{

TimeS LeadOutStartTime;

    Tg_endlba = 0;

    SupportMaximumSpeed();

    Svr_pause_reqire = TRUE;

#if (LED_NUM == 1)
    ReadyLED(LED_OFF);
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
    ReadyLED(LED_ON);
#endif /* LED_NUM == 2 */

    St_drv_ready  = TRUE;
    St_no_disc    = FALSE;                      /* mark disc exist */
    Iram_Rd_subc_flag = FALSE;

    if(pdbGetFinalSessCloseInfo() == eSessionFinalized){
        LeadOutStartTime = pdbGetFinalSessLeadoutTime();
    }else{
        LeadOutStartTime = pdbGetNextSessStartTime();
    }

    #if (EN_POWER_CONTROL)
    SetDiscReadPowerMode();
    #endif

    if(St_load_disc == TRUE)
    {
        /* Load disc success go to start ready*/
        Iram_Svr_player_mode = START_UP_MODE;
        St_load_disc = FALSE;
    }
}


/*--------------------------------------------------------------------
  Name: UpdateDiscTypeSpeed()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    sp_md
--------------------------------------------------------------------*/
BYTE UpdateDiscTypeSpeed(void)
{
    BYTE sp_md;

    /* Set up Read Speed with 4x here, because this function may be called after Write processing. */

    SendMsgCn(DEBUG_SVO_SPD_MSG|DEBUG_CD_WRITE_MSG,0,0x331029); // ***UpdateDiscTypeSpeed***


    if (pdbDiscLoaded() == CD_DISC)
    {
            CheckForSpecialDisc();
    }

    CheckForVideoCD();

    if(Special_Disc)
        sp_md = Set_sp_f;
    else
        sp_md = AutoMode;

    return sp_md;
}

/*********************************************************************
*
* Function:    CheckForSpecialDisc(void)
*
* Description:
*********************************************************************/
void CheckForSpecialDisc(void)
{
    BYTE result;

    result = SpecialDiscInformationParser(SpecialDiscInformationTable);

    if(result == eCDI)
    {
        pdbSetDiscModeType(0,eCDIMode);
        St_cdi = TRUE;

        if(pdbGetHighestRecordedTrkNumber() <= 1)
            pdbSetTrackDataMode(1,0x0F);   /* For CD-I no audio only */
    }

    if(result >= eWinBench_99 && result <= eABEX702)
        Special_Disc = result;
    else
        Special_Disc = eNormalD;
}

#if (FLASH_BY_DISC == 1)
extern void FlashWriteFW( void );
extern void SaveCurrentTransferSettings(BYTE Mode);  // Mode 0: normal , Mode 1: disc flush
#if 0
const Pattern1[]={'C','D','0','0','1'};
const Pattern2[]={'S','U','N','6','6','1','B','I','N'};
#define Pattern2_Len    9
#endif
#define FLASH_PAGE_COUNT      0x200

extern BYTE const EndOfFirmWare[16];
/*********************************************************************
*
* Function:    IsFlashDisc(void)
*
* Description:
*********************************************************************/
BYTE IsFlashDisc(void)
{
    BYTE result;

    result = SpecialDiscInformationParser(FlashDiscInformationTable);

    if(result == eFlashDisc)
        return TRUE;
    else
        return FALSE;
}

/*********************************************************************
*
* Function:    ReadFlashDisc(void)
*
* Description:
*********************************************************************/
BYTE ReadFlashDisc(void)
{
    ULONG timeout_timer;
    BYTE *DataPtr;                         /* Data pointer to valid TOC data */
    long   addr;
    DiscAccessStatusE   DiscReadStatus ;     /* Status of the DVD disc access */
    BYTE result,i,offset;


    DataPtr = (BYTE*)(SCSDRAM_ADDRESS +0x1800L);
    result = *DataPtr;
    i = *(DataPtr + result + 2);

    send_msg54(SEND_ATAPI_CMD,'R','F',result,i);

    if(i<0x14)
    {
        i = *(DataPtr + result);
        offset = *(DataPtr + result + i + 2);
    }
    else
        offset = i;

    send_msg54(SEND_ATAPI_CMD,'F','D',i,offset);

    result = FAIL;
    if( pdbDiscLoaded() == DVD_DISC )
    {
        if ( DVD_BUFFERING() == ON )
            dvdStopDiscTransfer();

        ClearDataBuffer();

        dvdDiscRequest.TargetID.Long = 0x30010;
        dvdDiscRequest.Type            = INIT_READ;
        dvdDiscRequest.TargetLayer     = eLayer0;
        dvdDiscRequest.EccBlockCount   = 0x22L;

        DiscReadStatus = dvdStartDiscTransfer();
        if (DiscReadStatus == BUF_GOOD)
        {
            offset-=0x10;
            CopyBufferToBuffer(offset, 0, 0, 0, FLASH_PAGE_COUNT);
            result = PASS;
        }

    } //if( pdbDiscLoaded() == DVD_DISC )
    else if (pdbDiscLoaded() == CD_DISC)//CD_DISC
    {
        addr=-10L;
        i = 0;
        while(i<10)
        {
            i++;
            if(ServoSeek(addr,0) == PASS)
            {
                TimeS time;

                svoIramVar.subQEnableFlag = 1;
                svoEnTimeRead();
                pdbSetSubQIntStatus(FALSE);
                StartTimer(&timeout_timer);
                while(1)
                {
                    if(pdbGetSubQIntStatus())
                    {
                        pdbGetABSTime(&time);
                        pdbSetSubQIntStatus(FALSE);
                        if ((time.Minute == 0) && (time.Second <2))
                        {
                            ClearDataBuffer();
                            DataBuffer.SectorCount = 0;
                            DataBuffer.CacheEndLBA = offset - 1;

                            WRITE_FIELD(HAL_HST_BUFPGCMP, 0);

                            InitBuffering();
                            while ((Svr_pause_reqire == FALSE) && (DataBuffer.SectorCount < 0x210))
                            {
                                if(ReadTimer(&timeout_timer) > 4000L) break;
                            }

                            if(DataBuffer.SectorCount > 0x200) result = PASS;

                            break;
                        }
                        else if ((time.Minute == 0) && (time.Second >2)) break;
                    }//if(pdbGetSubQIntStatus())

                    if(ReadTimer(&timeout_timer) > 2000L) break;
                } //while(1)

                if(result == PASS) break;
            }//if(ServoSeek(addr,0) == PASS)
        }//while(i<3)
        svoIramVar.subQEnableFlag = 0;
        StopDecoder();
    } //CD_DISC

    if(result == PASS)
    {
        timeout_timer = 0xffff0L;
        for(i=0; i<9; i++)
        {
            if(SCSDRAM[timeout_timer + i] != EndOfFirmWare[i])
            {
                result = FAIL;
                break;
            }
        }
    }

    return result;
}

/*********************************************************************
*
* Function:    UpdateFirmwareByDisc(void)
*
* Description:
*********************************************************************/
void UpdateFirmwareByDisc(void)
{
    ULONG   Index;
    USHORT chksum_DRAM2;

    chksum_DRAM2 = 0;

    for (Index = PARA_BLK_START_OFFSET; Index < (PARA_BLK_START_OFFSET + PARA_BLK_SIZE); Index++)
        chksum_DRAM2 += (USHORT)SCSDRAM[Index];       // Calculate Checksum for area whrere Persistent Table resides.

    for (Index = MKB_BLK_START_OFFSET; Index < (MKB_BLK_START_OFFSET + MKB_BLK_SIZE); Index++)
        chksum_DRAM2 += (USHORT)SCSDRAM[Index];       // Calculate Checksum for area whrere MKB Table resides.

    Index = 0xffffeL;
    chksum_DRAM2 = chksum_DRAM2 + SCSDRAM[Index] + SCSDRAM[Index + 1L];

    SCSDRAM[Index] = (vBYTE)(chksum_DRAM2>>8);
    SCSDRAM[Index + 1L] = (vBYTE)chksum_DRAM2;

    SaveCurrentTransferSettings(1);

    DisableAllInterrupts();
    FlashByDisc_flag = 0;
    FlashWriteFW();
}
#endif //#if (FLASH_BY_DISC == 1)

#if (DPD_COMPENSATE_OFFSET == 1)
/*********************************************************************
*
* Function:    ServoCalibDpdWholeDisc(void)
*
* Description: Do DPD offset compensation for every possible DPD FC settings at current CAV speed
*********************************************************************/
void ServoCalibDpdWholeDisc(void)
{
    USHORT temp_radius,end_radius;
    ULONG temp_psn;//improve whole disc pre calibration

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        temp_radius = CAV_RPM_MAX;
        if (spdGetCavSwitchPos(svoVar_svoTargetSpeed, &temp_radius) > 0)
        {//CAV or PCAV
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscCD:
                    temp_radius = svoPBA2Radius(Maxlba + 150);
                    end_radius = svoPBA2Radius(150);
                    break;

                case eDiscDVD:
                    temp_psn = pdbGetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber());
                    if ((Layer_Index) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC))
                    {
                        temp_radius = svoPBA2Radius(pdbGetDVDStartPSN(Layer_Index));
                        end_radius = svoPBA2Radius(temp_psn);
                    }
                    else
                    {
                       if(temp_psn <= pdbGetDVDMaxPSN(0))
                       {
                           temp_radius = svoPBA2Radius(temp_psn);
                       }
                       else
                       {
                           temp_radius = svoPBA2Radius(pdbGetDVDMaxPSN(Layer_Index));
                       }
                       end_radius = svoPBA2Radius(pdbGetDVDStartPSN(Layer_Index));
                    }
                    break;
#if (BD_ENABLE == 1)
                case eDiscBD:
                    temp_psn = pdbGetBDTrackEndPSN(pdbGetBDHighestRecordedTrackNumber());
                    if (Layer_Index)
                    {
                        temp_radius = svoPBA2Radius(pdbGetBDStartPSN(Layer_Index));
                        end_radius = svoPBA2Radius(temp_psn);
                    }
                    else
                    {
                        if(temp_psn <= pdbGetBDMaxPSN(0))
                        {
                            temp_radius = svoPBA2Radius(temp_psn);
                        }
                        else
                        {
                            temp_radius = svoPBA2Radius(pdbGetBDMaxPSN(Layer_Index));
                        }
                        end_radius = svoPBA2Radius(pdbGetBDStartPSN(Layer_Index));
                    }
                    break;
#endif
            }
            if (temp_radius > UM_2_SLED_STEP(58000)) temp_radius = UM_2_SLED_STEP(58000);
            else if (temp_radius < UM_2_SLED_STEP(24000)) temp_radius = UM_2_SLED_STEP(24000);
            if (end_radius > UM_2_SLED_STEP(58000)) end_radius = UM_2_SLED_STEP(58000);
            else if (end_radius < UM_2_SLED_STEP(24000)) end_radius = UM_2_SLED_STEP(24000);
        }
        else
        {//CLV
            temp_radius = end_radius = Iram_stp_CurrentPosition;
        }
        temp_radius = calGetDPDCalRadius(temp_radius, end_radius);

        //SendMsg5SValue(SHOW_DEBUG_MSG,"r01",4,temp_radius);

        if (temp_radius != 0)
        {
            ServoPause();
            if (((Current_read_sp <= Disc_Speed.Read)||((Iram_Cmd_block.bf[0]==0xF4)&&(Iram_Cmd_block.bf[1]==0x00)))&&(FokStatusOK()))
            {
                while (temp_radius != 0)
                {
                    if(svoIramVar.emergencyStopFlag || EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                        return;

                    ServoBCASeek(temp_radius, OFF);
                    while ((!svoSpeedVar.SpdInRange) && (!svoSpeedVar.SpdDown))
                    {
                        ExitProcess();
                        if(svoIramVar.emergencyStopFlag || EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                            break;
                    }
                    UpdateReadFilters((svoSpeedVar.SpdOverspd + 5) / 10);
                    if (calGetCalRequired(Layer_Index,Iram_stp_CurrentPosition,DPD_GNOS,READ_MODE))
                    {
                        HAL_SERVO_CenterControlOFF();
                        WaitTimer(5);
                        if ((calCalibrateTEGnOs(COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV) * 10) == eNOERR) && (last_cal_result < 0x1F))
                        {
                            calSetCalResult(Layer_Index, Iram_stp_CurrentPosition, DPD_GNOS,
                                           ((last_cal_result - svoCalGnT1Var[Layer_Index][TE_GN].groove_gain[RECDH]) << 8) |
                                           ((calDacCount2SBYTE(last_cal_result2,8) - calDacCount2SBYTE(svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[RECDH],8)) & 0xFF),
                                           CAL_R_DONE);
                            calSetDPDTEOS(Layer_Index);
                        }
                        else
                        {
                            calSetCalResult(Layer_Index, Iram_stp_CurrentPosition, DPD_GNOS, 0, CAL_SKIP);
                        }
                    }
                    temp_radius = calGetDPDCalRadius(temp_radius, end_radius);
                }
                if(svoIramVar.emergencyStopFlag || EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                    return;

                ServoBCASeek(Iram_stp_CurrentPosition,ON);
                pdbSetServoState(svoStTracking);//fix no pause after start up
            }
        }
    }
}
#endif
#if (CALIBRATION_MANAGEMENT==1)
/*********************************************************************
*
* Function: ServoCalibWholeDisc(void)
*
* Description: Do tilt,colli calibration for the whole disc (5 points).
*********************************************************************/
void ServoCalibWholeDisc(void)
{
    BYTE   i,zone;
    USHORT start_radius,end_radius;
    LONG   addr;
    ULONG temp_psn;//improve whole disc pre calibration

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            start_radius = svoPBA2Radius(Maxlba + 150);
            end_radius = svoPBA2Radius(150);
            break;

//improve whole disc pre calibration
        case eDiscDVD:
            temp_psn = pdbGetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber());
            if ((Layer_Index) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC))
            {
                start_radius = svoPBA2Radius(pdbGetDVDStartPSN(Layer_Index));
                end_radius = svoPBA2Radius(temp_psn);
            }
            else
            {
               if(temp_psn <= pdbGetDVDMaxPSN(0))
               {
                   start_radius = svoPBA2Radius(temp_psn);
               }
               else
               {
                   start_radius = svoPBA2Radius(pdbGetDVDMaxPSN(Layer_Index));
               }
               end_radius = svoPBA2Radius(pdbGetDVDStartPSN(Layer_Index));
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            temp_psn = pdbGetBDTrackEndPSN(pdbGetBDHighestRecordedTrackNumber());
            if (Layer_Index)
            {
                start_radius = svoPBA2Radius(pdbGetBDStartPSN(Layer_Index));
                end_radius = svoPBA2Radius(temp_psn);
            }
            else
            {
                if(temp_psn <= pdbGetBDMaxPSN(0))
                {
                    start_radius = svoPBA2Radius(temp_psn);
                }
                else
                {
                    start_radius = svoPBA2Radius(pdbGetBDMaxPSN(Layer_Index));
                }
                end_radius = svoPBA2Radius(pdbGetBDStartPSN(Layer_Index));
            }
            break;
#endif
    }

    for (i = 0; i < 5; i++)
    {
        if(svoIramVar.emergencyStopFlag || EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
            return;

        if (start_radius < end_radius)//OTP Layer1
        {//ID to OD
            zone = i;
        }
        else
        {//OD to ID
            zone = 4 - i;
        }

        if ((CalTestPos[zone] > start_radius) ^ (CalTestPos[zone] > end_radius))
        {
            if (calGetCalRequired(Layer_Index, CalTestPos[zone], ITEM_ALL, READ_MODE))
            {
                addr = svoRadius2PBA(CalTestPos[zone],Layer_Index);

                if (ServoSeek(addr, Layer_Index) == PASS)
                {
                    ClearAddressRead();
                    //because each ServoReCalibrate() will only do one calibration to save time, do it mutiple ServoReCalibrate() to make sure all calibrations are done.
                    (void)ServoReCalibrate();
                    (void)ServoReCalibrate();
                    (void)ServoReCalibrate();
                    //always mark CAL_SKIP so the calibration will not redo for a while (for blank disc).
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,TILT_DAT,0,CAL_SKIP);
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,FEBC_DSP,0,CAL_SKIP);
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x420006); //cal SEEK-NG!!!
                }
            }
        }
    }
}
#endif//#if (CALIBRATION_MANAGEMENT==1)

#if(EN_DVD_HIGH_JITTER_DETECT ==1)
/*********************************************************************
*
* Function:    JUDGE_HIGH_JITTER_DISC(void)
*
* Description:
*********************************************************************/
#define	    DVDRDL_HIGHJITTER_THLD  120
#define     NORMAL_HIGHJITTER_THLD  1000    //set very high to ingore this judge
#define     READ_JITTER_SAMPLE_CNT  8
void JUDGE_HIGH_JITTER_DISC(void)
{
    USHORT i,tmpjitter;
    USHORT jitter_thld;

    switch (pdbGetDiscKind())
    {
        case eDVDPlusRDLDisc:
        case eDVDMinusRDLDisc:
        case eDVDPlusRWDLDisc:
        case eDVDMinusRWDLDisc:
            jitter_thld = DVDRDL_HIGHJITTER_THLD;
            break;

        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:

        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDMinusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRWDisc:

    #if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
    #endif

    #if (BD_ENABLE)
        case eBDROMDisc:
        case eBDROMDLDisc:
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
    #endif

    #if (ENABLE_HDDVD)
        case HD_DISC:
    #endif
        default:
            jitter_thld = NORMAL_HIGHJITTER_THLD;
            break;
    }

    tmpjitter =0;

    for (i=0; i<READ_JITTER_SAMPLE_CNT; i++)
    {
        tmpjitter += HAL_DFE_ReadJitterX10();
    }
    tmpjitter /= READ_JITTER_SAMPLE_CNT;

    //send_msgA4(1, 0, 1, 3, tmpjitter);

    if(tmpjitter >= jitter_thld)
    {
  	    HIGH_JITTER_DISC = 3;
  	    SendMsgCn(SHOW_DEBUG_MSG,2,0x33FF45,A2B(tmpjitter));
    }
    else
    {
  	    HIGH_JITTER_DISC = 2;
  	    SendMsgCn(SHOW_DEBUG_MSG,2,0x33FF46,A2B(tmpjitter));
  	}
}
#endif//#if (EN_DVD_HIGH_JITTER_DETECT ==1)

void PageSize2P5K(void)
{   /*Automatic change from 3k to 2.5k page*/
        StopDecoder(); 
        Iram_Page_Size_3K_Flag = FALSE;
        DRAM_AUX_SIZE = DRAM_AUX_SIZE_2p5K;         /* DRAM AUX Size = 0x200(= 0.5KB) */
        RING_END_PAGE = RING_END_PAGE_2p5K;         /* last page the SP can access*/
        BUF_TOTAL_BLK_COUNT = BUF_TOTAL_BLK_COUNT_2p5K; /* total 672 pages */
        DATA_START_ADDR = DATA_START_ADDR_2p5K;     /* Data start address */
        AUX_START_ADDR = AUX_START_ADDR_2p5K;       /* Aux start address  */
        AUX_HEADER_ADDR1 = AUX_HEADER_ADDR1_2p5K;   /* M1 block header offset in page */
        AUX_HEADER_ADDR2 = AUX_HEADER_ADDR2_2p5K;   /* M2F1 block header offset in page */

        Iram_usiRingEP  = RING_END_PAGE;

        WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);         /* Host end blk. */
        WRITE_FIELD(HAL_CDD_DSKBUFEPG, Iram_usiRingEP);         /* ring end blk. */

 #if (AUDIO_PLAYBACK == 1)
 #if (SUN_FUNC_TEST == 1)
        if(READ_FIELD(HAL_GLOBAL_AUDCKSTP))
        {
            BYTE  i;
            WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 0);                
            for (i=0;i<0x05;i++);
            WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
            WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 1);                
        }
        else
 #endif
            WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
 #endif

        if(READ_FIELD(HAL_GLOBAL_CPYCKSTP))
        {
            BYTE  i;
            WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);                /* Enable copy clock */
            for (i=0;i<0x05;i++);
            WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */
            WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 1);                /* Disable copy clock */
        }
        else
            WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */

        WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x00);       /*no Flash download*/
        WRITE_FIELD(HAL_CDD_MLY, e_Page2_HalfK);        /* Memory layout 2.5K */
        WRITE_FIELD(HAL_CDD_C2P, e_C2_ERROR_COUNT);
        WRITE_FIELD(HAL_CDD_C2WEN, CLEAR);

        Iram_shHDDir |= HAL_HST_HMLY;           /* 2.5K Host Layout*/
        WRITE_FIELD(HAL_HST_HMLY, 1);           /* 2.5Host Layout*/
}

