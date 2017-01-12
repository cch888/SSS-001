/****************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* $Revision: 1664 $
* $Date: 11/04/27 11:03a $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"

#include ".\common\stdlib.h"
#include ".\servo\svo.h"
#include ".\common\commath.h"
#include ".\common\com_im.h"
#include ".\common\error.h"
#include ".\common\system.H"
#include ".\common\scomm.h"
#include ".\common\ADC.h"
#include ".\player\plrdb.h"
#include ".\servo\svo_afe.h"

#include ".\servo\svomacro.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_calib.h"
#include ".\servo\svo_speed.h"
#include ".\servo\svomath.h"
#include ".\hif\gp_ram.h"
#include ".\servo\svo_step.h"
#include ".\servo\svoVar.h"
#include ".\oem\oem_tbl.h"
#include ".\servo\afe_tbl.h"
#include ".\servo\svo_sta.h"
#include ".\hif\flash.h"

#include ".\dvd\dvDef.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvMacros.h"
#include ".\servo\dvsvo_tbl.h"
#if (DVD_RAM_READ == 1)
#include ".\servo\dvRamSvo_tbl.h"
#endif
#if (SRC_ENABLE == 1)
#include ".\al\HAL_dsp.h"
#endif // (SRC_ENABLE == 1)
#include ".\al\coefs.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\al\REG_bd_codec.h"
#include ".\al\REG_dfe.h"
#include ".\al\REG_servo.h"
#include ".\al\HAL_pll.h"
#include ".\al\HAL_bd_codec.h"
#include ".\al\hal_afe.h"
#include ".\al\HAL_servo.h"
#include ".\al\HAL_wobble.h"
#include ".\al\HAL_global.h"
#include ".\AL\HAL_lasc.h"
#include ".\al\LAL_dfe.h"
#include ".\AL\LAL_wobble.h"
#if (ENABLE_LEARN_DEFECT == 1)
#include ".\AL\LAL_dsp.h"
#endif
#include ".\al\hal_dfe.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_servodsp.h"
#include ".\al\reg_periph.h"
#include ".\al\REG_write_strategy.h"
#include ".\servo\svo_dsp.h"
#if (BD_ENABLE == 1)
#include ".\Servo\bdSvo_tbl.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdVars.h"
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
#include ".\Servo\hdSvo_tbl.h"
#include ".\hd\hdDef.h"
#include ".\hd\hdDB.h"
#endif // (ENABLE_HDDVD == 1)

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif

#if (OPU == KEM350AAA)
#include ".\OEM\OPU_KEM350AAA.h"
#endif //(OPU == KEM350AAA)
#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON==1)
#include ".\hif\diag.h"
#include ".\BD\bddiag.h"
#endif
#include ".\servo\svo_seek.h"

#include ".\AL\REG_ssi.h"

#if DEFECT_LENGTH_MEASURE == 1
extern BYTE CD_Scratch_Disc;//== modify for SCD-2383
#endif
void set_oscen(BYTE);

#if (OPU == KEM350AAA)
BOOL   opu_eeprom_valid;
#endif
/*-----------------------------------------------------------------------------
 *
 * Module: svoInitVar()
 * Description:
 *
 *----------------------------------------------------------------------------*/
extern BYTE     PLL_InLockaid;
void svoInitVar(void)
{
    BYTE i;
    WRITE_FIELD(HAL_GLOBAL_WSSRST, 1);  // Reset Write Strategy block
    WRITE_FIELD(HAL_GLOBAL_WSSRST, 0);

    DISABLE_WBLINT();
    SendMsg80(DEBUG_WOBBLE_FLOW_MSG,0x390011); //DISABLE_WBLINT()
    ENABLE_WBLINT();

#if (PCB == 710)||(PCB == 711)
    //DVD/CD photo diode sleep mode
    SW_INPUT();
    //BD photo diode sleep mode
    SWB1(1);
    SWB2(1);
#elif (PCB == 817)  //edwin
    //DVD/CD photo diode sleep mode
    DVDCD_STB_STB;
    //BD photo diode sleep mode
    BD_SLEEP_EN;
#endif

    svoIramVar.svoInState   = (SVO_INIT | SEQ_001); /* Status "INIT" */
    SetDelayTimer(TIMER_STOP);                      /* delay timer stop */
    SetTimeout(0, TIMER_STOP);                      /* time_out timer stop */
    SetTimeout(1, TIMER_STOP);                      /* time_out timer stop */

#if (ENABLE_BDRLTH == 1)
    if (svoIramVar.LTHDisc)
    {
        svoIramVar.LTHDisc = 0;
        LAL_SetAFEandDFEforLTH();
    }
    svoIramVar.LTHDisclikely = 0;
    svoIramVar.LTHDiscRPChecked = 0;
#endif
    svoSeekVar.rx_available         = FALSE;
    svoIramVar.subQReadyFlag        = TRUE;
    svoIramVar.atipReadyFlag        = FALSE;
    svoVar_svoTargetSpeed = SVO_CAL_SPEED_CD;                  /* Disc speed "4x" *///cchutest
    svoCmdVar.outputFormat          = eCDRom;
    svoVar_discDiameter             = eUnknownDiameter;
#if (FORCE_DISK_KIND==1)
    if(DiscForce==0)
    {
        svoVar_discKindT.fDiscKind  = eUnknownKind;
        svoCmdVar.demodMode         = eDemodETM;
    }
#else
    svoVar_discKindT.fDiscKind      = eUnknownKind;
    svoCmdVar.demodMode             = eDemodETM;
#endif
    svoCmdVar.servoError = 0x00;                 /* Error clear */
    svoCmdVar.discError  = 0x00;
    svoCmdVar.trayError  = 0x00;

    svo_preset_media_par();

    svoVar_bByPassFOKCheck = TRUE;

#if (DeadLoop_Solution == 1)
    svo_ERR_STARTUP = FALSE;
#endif // (DeadLoop_Solution == 1)

    svoCmdVar.svoCommand    = CNOP;

    /* Initial Parameter of Power Calibration */
    /* CD Part */
    svoPcalVar.cdp2rOff = 0.0f;
    svoPcalVar.cdp2rSlp = 0.001f;
    svoPcalVar.cdr2sOff = 0.0f;
    svoPcalVar.cdr2sSlp = 0.001f;
    svoPcalVar.cdv2eOff = 0.0f;
    svoPcalVar.cdv2eSlp = 0.001f;
    svoPcalVar.cdmw2fpdoOff = 0.0f;
    svoPcalVar.cdmw2fpdoSlp = 0.001f;
    svoPcalVar.cdmw2fpdo1Off = 0.0f;
    svoPcalVar.cdmw2fpdo1Slp = 0.001f;
    svoPcalVar.cdmw2fpdo2Off = 0.0f;
    svoPcalVar.cdmw2fpdo2Slp = 0.001f;

#if (PersistentTable_Revision  >= 7)
    svoPcalVar.cdp2w1Off = 0.0f;
    svoPcalVar.cdp2w1Slp = 0.001f;
    svoPcalVar.cdp2w2Off = 0.0f;
    svoPcalVar.cdp2w2Slp = 0.001f;
    svoPcalVar.cdp2e1Off = 0.0f;
    svoPcalVar.cdp2e1Slp = 0.001f;
    svoPcalVar.cdp2e2Off = 0.0f;
    svoPcalVar.cdp2e2Slp = 0.001f;
    svoPcalVar.cdp2pk1Off = 0.0f;
    svoPcalVar.cdp2pk1Slp = 0.001f;
    svoPcalVar.cdp2pk2Off = 0.0f;
    svoPcalVar.cdp2pk2Slp = 0.001f;
    svoPcalVar.cdp2c1Off = 0.0f;
    svoPcalVar.cdp2c1Slp = 0.001f;
    svoPcalVar.cdp2c2Off = 0.0f;
    svoPcalVar.cdp2c2Slp = 0.001f;
#endif // (PersistentTable_Revision  >= 7)
    svoPcalVar.cdDiffDac=0;

/* DVD Part */
#if (EN_Persistent_table_8 == 1)
    svoPcalVar.dvbp2Diffdacoff= 0.0f;
    svoPcalVar.dvbp2DiffdacSlp= 0.001f;
#endif // (EN_Persistent_table_8 == 1)
    svoPcalVar.dvp2rOff = 0.0f;
    svoPcalVar.dvp2rSlp = 0.001f;
    svoPcalVar.dvr2sOff = 0.0f;
    svoPcalVar.dvr2sSlp = 0.001f;
    svoPcalVar.dvp2w1Off = 0.0f;
    svoPcalVar.dvp2w1Slp = 0.001f;
    svoPcalVar.dvp2w2Off = 0.0f;
    svoPcalVar.dvp2w2Slp = 0.001f;
    svoPcalVar.dvp2e1Off = 0.0f;
    svoPcalVar.dvp2e1Slp = 0.001f;
    svoPcalVar.dvp2e2Off = 0.0f;
    svoPcalVar.dvp2e2Slp = 0.001f;
    svoPcalVar.dvv2e1Off = 0.0f;    //vwdc vs. hold Dac low gain
    svoPcalVar.dvv2e1Slp = 0.001f;
    svoPcalVar.dvv2e2Off = 0.0f;    //vwdc vs. hold Dac high gain
    svoPcalVar.dvv2e2Slp = 0.001f;
    svoPcalVar.dvvwdc12pk1dacOff = 0.0f;    //vwdc1 vs. pk1Dac
    svoPcalVar.dvvwdc12pk1dacSlp = 0.001f;
    svoPcalVar.dvvwdc12pk2dacOff = 0.0f;    //vwdc1 vs. pk2Dac
    svoPcalVar.dvvwdc12pk2dacSlp = 0.001f;
    svoPcalVar.dvp2pk1Off = 0.0f;
    svoPcalVar.dvp2pk1Slp = 0.001f;
    svoPcalVar.dvp2pk2Off = 0.0f;
    svoPcalVar.dvp2pk2Slp = 0.001f;
    svoPcalVar.dvp2c1Off = 0.0f;
    svoPcalVar.dvp2c1Slp = 0.001f;
    svoPcalVar.dvp2c2Off = 0.0f;
    svoPcalVar.dvp2c2Slp = 0.001f;
    svoPcalVar.dvvwdc22cdacOff = 0.0f;    //vwdc1 vs. cooling Dac
    svoPcalVar.dvvwdc22cdacSlp = 0.001f;
    svoPcalVar.dvp2c3Off = 0.0f;
    svoPcalVar.dvp2c3Slp = 0.001f;
    svoPcalVar.dvmw2fpdoOff = 0.0f;
    svoPcalVar.dvmw2fpdoSlp = 0.001f;
    svoPcalVar.dvmw2fpdo1Off = 0.0f;
    svoPcalVar.dvmw2fpdo1Slp = 0.001f;
    svoPcalVar.dvmw2fpdo2Off = 0.0f;
    svoPcalVar.dvmw2fpdo2Slp = 0.001f;

#if (EN_Persistent_table_8 == 1)
    //BD Part
    svoPcalVar.bdp2rOff =0.0f;
    svoPcalVar.bdp2rSlp =0.001f;
    svoPcalVar.bdr2sOff =0.0f;
    svoPcalVar.bdr2sSlp =0.001f;
    svoPcalVar.bdp2w1Off=0.0f;
    svoPcalVar.bdp2w1Slp=0.001f;
    svoPcalVar.bdp2w2Off=0.0f;
    svoPcalVar.bdp2w2Slp=0.001f;
    svoPcalVar.bdp2e1Off=0.0f;
    svoPcalVar.bdp2e1Slp=0.001f;
    svoPcalVar.bdp2e2Off=0.0f;
    svoPcalVar.bdp2e2Slp=0.001f;
    svoPcalVar.bdv2e1Off=0.0f;    //vwdc vs. Dac
    svoPcalVar.bdv2e1Slp=0.001f;
    svoPcalVar.bdv2e2Off=0.0f;
    svoPcalVar.bdv2e2Slp=0.001f;

    svoPcalVar.bdp2c1Slp=0.001f;
    svoPcalVar.bdp2c1Off=0.0f;
    svoPcalVar.bdp2c2Slp=0.001f;
    svoPcalVar.bdp2c2Off=0.0f;
#endif // (EN_Persistent_table_8 == 1)

#if (EN_Persistent_table_8 == 1)
    //HD Part
    svoPcalVar.hdp2rOff =0.0f;
    svoPcalVar.hdp2rSlp =0.001f;
    svoPcalVar.hdr2sOff =0.0f;
    svoPcalVar.hdr2sSlp =0.001f;
    svoPcalVar.hdp2w1Off=0.0f;
    svoPcalVar.hdp2w1Slp=0.001f;
    svoPcalVar.hdp2w2Off=0.0f;
    svoPcalVar.hdp2w2Slp=0.001f;
    svoPcalVar.hdp2e1Off=0.0f;
    svoPcalVar.hdp2e1Slp=0.001f;
    svoPcalVar.hdp2e2Off=0.0f;
    svoPcalVar.hdp2e2Slp=0.001f;
    svoPcalVar.hdv2e1Off=0.0f;    //vwdc vs. Dac
    svoPcalVar.hdv2e1Slp=0.001f;
    svoPcalVar.hdv2e2Off=0.0f;
    svoPcalVar.hdv2e2Slp=0.001f;

    svoPcalVar.hdp2c1Slp=0.001f;
    svoPcalVar.hdp2c1Off=0.0f;
    svoPcalVar.hdp2c2Slp=0.001f;
    svoPcalVar.hdp2c2Off=0.0f;
#endif // (EN_Persistent_table_8 == 1)

    svoPcalVar.dvv2tOff = 88.338f;
    svoPcalVar.dvv2tSlp = -0.0276f;
    svoPcalVar.dvDiffDac=0;

    svoPcalVar.CalDoneBits = 0x0000;

    svoPcalVar.cdFBDacValue = 0;
#if (EN_Persistent_table_8 == 1)
    svoPcalVar.cdFeoBGValue  = 0x10; //FEOBG = 0dB
    svoPcalVar.cdSPP2BGValue = 0x10; //SPP2BG = 0dB
#else /* (EN_Persistent_table_8 == 1) */
    svoPcalVar.cdFeBGValue  = 0x1010;  // SPP2BG = 0dB; FEOBG = 0dB
#endif /* (EN_Persistent_table_8 == 1) */
    svoPcalVar.cdRRFGValue  = 0x03;    // 0dB
    svoPcalVar.cdBetaOffset = 0;

    svoPcalVar.dvFBDac0Value = 0;
#if (EN_Persistent_table_8 == 1)
    svoPcalVar.dvFeoBG0Value  = 0x10; // FEOBG = 0dB
    svoPcalVar.dvSPP2BG0Value  = 0x10;// SPP2BG = 0dB
#else // (EN_Persistent_table_8 == 1)
    svoPcalVar.dvFeBG0Value  = 0x1010; // SPP2BG = 0dB; FEOBG = 0dB
#endif // (EN_Persistent_table_8 == 1)

    svoPcalVar.dvFBDac1Value = 0;
#if (EN_Persistent_table_8 == 1)
    svoPcalVar.dvFeoBG1Value  = 0x10; // FEOBG = 0dB
    svoPcalVar.dvSPP2BG1Value  = 0x10;// SPP2BG = 0dB
#else // (EN_Persistent_table_8 == 1)
    svoPcalVar.dvFeBG1Value  = 0x1010; // SPP2BG = 0dB; FEOBG = 0dB
#endif //(EN_Persistent_table_8 == 1)
    svoPcalVar.dvRRFGValue   = 0x00;   // 0dB
    svoPcalVar.dvBetaOffset  = 0;

    svoPcalVar.dvTiltDacValue[0] = 0x0200;
    svoPcalVar.dvTiltDacValue[1] = 0x0200;
    svoPcalVar.dvTiltDacValue[2] = 0x0200;
    svoPcalVar.dvTiltDacValue[3] = 0x0200;
    svoPcalVar.dvTiltDacValue[4] = 0x0200;

    fCheck_Pluse_Power = FALSE;    //flag for check power with pulse

//  Cal_LoadOfflineCalibrationParam();
    svoSpeedVar.fgPulseTime  = 0xFFFF;

    svoIramVar.subQEnableFlag   = 1;

    svoIramVar.spindleStopFlag  = DSTOPPED;
    svoSpeedVar.fgCount          = 0;
    svoIramVar.svoCurrentSpeed  = 0xFF;
    svoVar_abort_pause_if_applyable = FALSE;

    Iram_stp_MoveInterval   = 0xFFFF;

    svoMiscVar.zoneAreaCheckFlag= FALSE;
    svo_Var_motorDriverState = ALL_MUTE_ON;
    svoPrvtVar_LD_ON_Flag = OFF;

    vref_fm_mux3_mV = VREF_IN_MV;

    svoLIBoundary = 450000L;
    svoVar_getDVDFormatflag = 0;
    svoIramVar.currentID.Layer = 0;    /* winfinal DVD-9 test fail modify */
    svoSeekVar.svoTargetID.Layer = 0;
    Layer_Index = 0;
    LJ_FailCnt = 0;

    svo_BoundaryDet = 0;
    svoIramVar.SubqAtipBypass=0;

#if (EN_ADI == 1)
    servo_status = 0;
#endif

#if (FOOFFSET2_ENABLE == 1)
    WRITE_REG(HAL_SRVREG_MAP->FOOFFSET,0);
    WRITE_REG(HAL_SRVREG_MAP->TILTOFS,0);
#endif
    HAL_SERVO_SetFoos(0);
    svoCalMisc2Var.fooos = 0;
    svoCalMisc2Var.pdic_gn = 0;
    svoCalMisc2Var.laser_gn = 0;
    svoVar_bPdicMode = 0;
    svoVar_bLascMode = 0;
#if (DVD_RAM_READ == 1)
    svoIramVar.headerIDReadyFlag = FALSE;
#endif
#if DETECT_TE_OFFTRACK
    TOKLVL_BACKUP = halAFE_TOKLVL_VREF_450mV;
    Off_Track_Counter = 0;
#endif // DETECT_TE_OFFTRACK

#if Monitor_FPDO_Linearity ==1
    bEnMonitorDac = 0;
    VWDCDAC = 0;
#endif
    CALIBRATE_SYSTEM_LEAD_IN = 0;
#if (HORROR_DISC_IMPROVEMENT == 1)
    read_retry_option =0;
    read_retry_restartTime=0;
    read_retry =0;
    dfe_retry = 0;
    FEBC_retry = 0;
    Tilt_retry = 0;
    initFEBC = 0;
    initTilt = 0;
    fgNormalValueSaved = FALSE;
    fgNormalValueChgAgain = FALSE;
    read_retry_status = 0;
    BufferErrorForRetry = 0;
#endif

#ifdef RETRY_CHG_EQTAPA12
    retry_DFE_EqualizerStrength=lalDFE_EqualizerStrengthNormal;
#endif

#if (WOB_BASED_DEMODULATOR == 1)
   wob_based_demod = 0;
#endif

#if (OPEN_SESSION == 1)
    RF_search_count = 0;
#endif

#if (SUN710_TRAY_PROFILE == 1)  || ( EEPROM_TRAY_PROFILE ==1)
    Tray_Phase = 0;
#endif

#if (DETECT_ECC_DISC == 1)
    svoVar_eccDiscNum = eECC_normal;
    svoRunOut         = 0;
    svoRunOutCTN   = 0;
#else
    svoRunOut         = 0;
    svoRunOutCTN   = 0;
#endif
    svoRRFVar.modulation[0] = svoRRFVar.modulation[1] = 0;

#if (NEW_FDP_DETECTION != 0)
    svoIramVar.focusDropIsrFlag = 0;
#endif

    //For new DSP FEDG drop protection, it needs to set FOOoffset2 to be 0
    HAL_SERVO_SetProtFoos(0);

    cal_retry = 0;
    fepp_wrong_layer_chkcnt = 0;

#if (BD_ENABLE == 1)
    svoCalDlVar[0].dpp_thr = svoCalDlVar[1].dpp_thr = 0;
    svoCalDlVar[0].fe_factor = FE_FACTOR_BD_L0 * 1024;
    svoCalDlVar[1].fe_factor = FE_FACTOR_BD_L1 * 1024;
#endif

    svoStepVar.stepHomeCheck = FALSE;
    svoStepVar.StepHomePos = STEP_HOME_POSITION_INIT;

#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
    Unbalance_lowspd=Unbalance_midspd = Unbalance_highspd = 0;
    fUnbalanceChk_SpdUp = 0;
#endif

#if (NEW_RFPOS_CALIB == 1)
    for (i = 0; i < 32; i++)
    {
        svoRRFVar.rfagc_ref_LPF[0][i] = 0;
        svoRRFVar.rfagc_ref_LPF[1][i] = 0;
        svoRRFVar.rfagc_ref_LPF[2][i] = 0;
    }
    svoRRFVar.rfagc_ref = 0;
    #if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
    svoRRFVar.rfaoc_ref[0] = 0x7FFF;
    svoRRFVar.rfaoc_ref[1] = 0x7FFF;
    #else
    svoRRFVar.rfpos_step_size = 0xFFFF;
    #endif
#endif

#if ((EN_BD_ADEQ == 1) || (EN_ADEQ == 1))
    Disable_ADEQ = 0;
#endif


#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
    DVDHightJitter = 0;
#endif
#if (FDP_TRKSLIP_MONITOR == 1)
    svoMiscVar.forceTrackOffFlag = FALSE;
    svoMiscVar.forceFocusOffFlag = FALSE;
#endif
#if (ENABLE_BDIH == 1)
#if (FORCE_READ_BDIH == 1)
    bd_hybrid=1;
#endif
#endif
#if(CDDVDNewBlankChk == 1)
    track_off_rf_dif_fail = 0;
    RecordedCount = 0;
#endif
    PLL_InLockaid = 0;

#if (SUPPORT_DPD == 1) || (CD_MB_ONLY_OPU == 1)
    svoIramVar.do_cd_dpd_disc = FALSE;
#endif // SUPPORT_DPD == 1
#if (ERROR_CAL_START_UP_MONITOR == 1)
    svoMiscVar.calibrationErrorCount = 0;
#endif
    spdStop();
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoServoInit()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoServoInit(void)
{
    //-------------------------------------------;
    // init timer 0                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER0);
    SetTimerXValue(TIMER0, 0);
    ENABLE_TIMER_INT(TIMER0);
    START_TIMERS(TIMER0);
    ENABLE_T0INT();

    //-------------------------------------------;
    // init timer 1                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER1);
    SetTimerXValue(TIMER1, 0);
    ENABLE_T1INT();

    //-------------------------------------------;
    // init timer 2                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER2);
    SetTimerXValue(TIMER2, 0);
    ENABLE_T2INT();

    //-------------------------------------------;
    // init timer 3                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER3);
    SetTimerXValue(TIMER3, 0);
    ENABLE_TIMER_INT(TIMER3);
    ENABLE_T3INT();

    //-------------------------------------------;
    // init timer 5                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER5);
    SetTimerXValue(TIMER5, 0);

#if DO_HW_SUPPORT_STEP == 0
    START_TIMERS(TIMER5);
    ENABLE_T5INT();
#endif /* DO_HW_SUPPORT_STEP */

    //-------------------------------------------;
    // init timer 6                              ;
    //-------------------------------------------;
    STOP_TIMER(TIMER6);

    //-------------------------------------------
    // enable interrupts
    //-------------------------------------------
    ENABLE_TIMER_INT(TIMER5);
    ENABLE_EXT_INT();   /* Enable Eject Sw Int. */
    ENABLE_SINT();      /* Enable SINT */
    ENABLE_DSPINT();    /* Enable PINT */
    ENABLE_EINT();      /* Enable EINT */

    //ClrBitsSCReg_1B(rwINT1EN, bSCOREN);     /* Disable SUB-Q interrupt - Stop SubQ decoding */
    //ClrBitsSCReg_1B(rwWINTEN2, bAMSFRdyEn); /* Disable ATIP Interrupt - Stop ATIP decoding *///TBD:SE1
    WRITE_FIELD(HAL_CDD_SCOREN, 0);/* Disable SUB-Q interrupt - Stop SubQ decoding */

    StopFGTimer();
}

/*-----------------------------------------------------------------------------
 *
 * Module: InitSvoRegsForSpeed()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void InitSvoRegsForSpeed(discKindT DiscKind, ESpindleSpeed speed)
{
    USHORT  i;

    if ((DiscKind.Bit.DiscStd == eDiscCD) || (DiscKind.Bit.DiscStd == eDiscUnknown))
    {
        if (speed > MAX_SPEED_CD)
            return;

        if (svoRegInitTbl[speed] == NULL)
        {
            return;
        }

        for (i=0;;i++)
        {
            if (svoRegInitTbl[speed][i].addr == SVOREGTBL_END)
            {
                break;
            }
            setServoRegister(svoRegInitTbl[speed][i].addr,
                             svoRegInitTbl[speed][i].data,
                             svoRegInitTbl[speed][i].mode);
        }
    }
    else if (DiscKind.Bit.DiscStd == eDiscDVD)
    {
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            if (speed > MAX_SPEED_DVDRAM)
                return;

            if (dvRamSvoRegInitTbl[speed] == NULL)
            {
                return;
            }

            for (i=0;;i++)
            {
                if (dvRamSvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                {
                    break;
                }
                setServoRegister(dvRamSvoRegInitTbl[speed][i].addr,
                                 dvRamSvoRegInitTbl[speed][i].data,
                                 dvRamSvoRegInitTbl[speed][i].mode);
            }
        }
        else
#endif
        {
            if (speed > MAX_SPEED_DVD)
                return;

            if (dvsvoRegInitTbl[speed] == NULL)
            {
                return;
            }

            for (i=0;;i++)
            {
                if (dvsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                {
                    break;
                }
                setServoRegister(dvsvoRegInitTbl[speed][i].addr,
                                 dvsvoRegInitTbl[speed][i].data,
                                 dvsvoRegInitTbl[speed][i].mode);
            }
        }
    }
#if (BD_ENABLE == 1)
    else if (DiscKind.Bit.DiscStd == eDiscBD)
    {
        if (speed > MAX_SPEED_BD)
            return;

        if (bdsvoRegInitTbl[speed] == NULL)
        {
            return;
        }

        for (i=0;;i++)
        {
            if (bdsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
            {
                break;
            }
            setServoRegister(bdsvoRegInitTbl[speed][i].addr,
                             bdsvoRegInitTbl[speed][i].data,
                             bdsvoRegInitTbl[speed][i].mode);
        }
    }
#endif
#if (ENABLE_HDDVD == 1)
    else if (DiscKind.Bit.DiscStd == eDiscHD)
    {
        if (speed > MAX_SPEED_HD)
            return;

        if (hdsvoRegInitTbl[speed] == NULL)
        {
            return;
        }

        for (i=0;;i++)
        {
            if (hdsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
            {
                break;
            }
            setServoRegister(hdsvoRegInitTbl[speed][i].addr,
                             hdsvoRegInitTbl[speed][i].data,
                             hdsvoRegInitTbl[speed][i].mode);
        }
    }
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: setServoRegister()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void setServoRegister(ULONG addr, USHORT data, eSvoRegInitMode mode)
{
    BYTE temp;

    switch (mode)
    {
        case WORDSET:
            WRITE_ADR_2B(addr, data);
            break;
        case BYTESET:
            temp = (BYTE)data;
            WRITE_ADR_1B(addr, temp);
            break;
        case BIT_ON:
            temp = ((BYTE)READ_ADR_1B(addr)) | (BYTE)data;
            WRITE_ADR_1B(addr, temp);
            break;
        case BIT_OFF:
            temp = ((BYTE)READ_ADR_1B(addr)) & (BYTE)(~data);
            WRITE_ADR_1B(addr, temp);
            break;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoRegValueFromTbl()
 * Description:
 *
 *----------------------------------------------------------------------------*/
USHORT svoRegValueFromTbl(ESpindleSpeed speed, ULONG addr)
{
    int i;

    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscUnknown))
    {
        if (svoRegInitTbl[speed] == NULL)
            speed = NONE;

        while (1)
        {
            for (i=0;;i++)
            {
                if (svoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                   break;

                if (svoRegInitTbl[speed][i].addr == addr)
                   return(svoRegInitTbl[speed][i].data);
            }

            if (speed != NONE)
            {
                speed = NONE;
            }
            else
            {
                break;
            }
        }
        return(0);
    }
    else if (svoVar_discKindT.Bit.DiscStd == eDiscDVD) //DVD
    {
        if (dvsvoRegInitTbl[speed] == NULL)
            speed = NONE;

        while (1)
        {
            for (i=0;;i++)
            {
                if (dvsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                    break;

                if (dvsvoRegInitTbl[speed][i].addr == addr)
                    return(dvsvoRegInitTbl[speed][i].data);
            }

            if (speed != NONE)
            {
                speed = NONE;
            }
            else
            {
                break;
            }
        }
        return(0);
    }

#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD) //BD
    {
        if (bdsvoRegInitTbl[speed] == NULL)
            speed = NONE;

        while (1)
        {
            for (i=0;;i++)
            {
                if (bdsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                    break;

                if (bdsvoRegInitTbl[speed][i].addr == addr)
                    return(bdsvoRegInitTbl[speed][i].data);
            }

            if (speed != NONE)
            {
                speed = NONE;
            }
            else
            {
                break;
            }
        }
        return(0);
    }
#endif

#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD) //HD
    {
        if (hdsvoRegInitTbl[speed] == NULL)
            speed = NONE;

        while (1)
        {
            for (i=0;;i++)
            {
                if (hdsvoRegInitTbl[speed][i].addr == SVOREGTBL_END)
                    break;

                if (hdsvoRegInitTbl[speed][i].addr == addr)
                    return(hdsvoRegInitTbl[speed][i].data);
            }

            if (speed != NONE)
            {
                speed = NONE;
            }
            else
            {
                break;
            }
        }
        return(0);
    }
#endif

    else return 0;
}


/*-----------------------------------------------------------------------------
 *
 * Module: svoSetServoOff()
 * Description: set all servo off
 *
 *----------------------------------------------------------------------------*/
void    svoSetServoOff(void)
{
    SledStop();
    discstop();
    svoSetLDOff();      /* set LD off */

#if (SRC_ENABLE == 1)
    //----------------------------------------------------;
    // disable SRC                                        ;
    //----------------------------------------------------;
    HAL_DSP_SRCHold(SRC_LOOP_ALL, ON, ON, ON);
#endif

    WRITE_REG(HAL_SRVREG_MAP->FGA, 0x00);
    WRITE_REG(HAL_SRVREG_MAP->TGA, 0x00);
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoSetLDOff()
 * Description: set LD OFF
 *
 *----------------------------------------------------------------------------*/
void    svoSetLDOff(void)
{
    if (svoPrvtVar_LD_ON_Flag == OFF)
    {
        return;
    }

#if (SRC_ENABLE == 1)
    HAL_DSP_SRCHold(SRC_LOOP_ALL, ON, ON, ON);
    HAL_DSP_SRCEnable(SRC_LOOP_ALL,SRC_STATE_RESET);
#endif
//20101224_030U ==start== Fix HS-CDRW OPU lens hit disc issue, Frank modify Andy confirm
#if (NEW_FDP_DETECTION != 0)
    WRITE_FIELD(HAL_SRV_FNOKEN,0);      //turn off Focus drop interrupt
    #if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0);     //turn off New Focus drop interrupt
    #endif
#endif
    WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);   /* set all servo EQ and LD OFF */
    WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);    /* set LD ON  */
    ramp_FOOS(0);
#if (CHIP_REV >= 0xC0)
    HAL_SERVO_SetFcs(0);
    WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
    WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
#endif
    HAL_LASC_SetLaserOn(FALSE);
//20101224_030U ==end== Fix HS-CDRW OPU lens hit disc issue, Frank modify Andy confirm

#if (ENABLE_LEARN_DEFECT == 1)
    LAL_DSP_SetLearnDefect(lalDSP_Off);
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoFocusOff()
 * Description: set focus and tracking off
 *
 *----------------------------------------------------------------------------*/
void    svoFocusOff(void)
{
    if (svoPrvtVar_LD_ON_Flag == OFF)
    {
        return;
    }
    SendMsg80(SHOW_DEBUG_MSG,0x33FF04); // >>> FOCUS OFF <<<
#if (AFE_NORMALIZER == 1)
    afeSvognDisable();
#endif //  AFE_NORMALIZER == 1
#if (NEW_FDP_DETECTION != 0)
    WRITE_FIELD(HAL_SRV_FNOKEN,0);      //turn off Focus drop interrupt
    #if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0);     //turn off New Focus drop interrupt
    #endif
#endif
    WRITE_FIELD(HAL_SRV_LDOFF, 0x00);   /* set all servo EQ and LD OFF */
    WRITE_FIELD(HAL_SRV_LDON, 0x00);    /* set LD ON  */
#if (FOOFFSET2_ENABLE == 1)//bug fixed: this action must be done after LDOFF/ON when FOOFFSET2_ENABLE == 1
    WRITE_DSP_COEF(kf18,0);
    HAL_SERVO_SetFocDn(0);
#endif
    svoVar_bByPassFOKCheck = TRUE;
}

/*********************************************************
* Module: getTrackingMethod()
*
* Description: Gets tracking method for any media
* Input: discKindT
* Returns: ETrackingMethod tracking method
**********************************************************/
ETrackingMethod getTrackingMethod(discKindT discKind)
{
    ETrackingMethod tracking_method;
    switch (discKind.Bit.DiscStd)
    {
        case eDiscCD:
#if (SUPPORT_DPD == 1)
            if (svoIramVar.do_cd_dpd_disc)
            {
                tracking_method = TRK_METHOD_DPD;
            }
            else
#endif
            {
                tracking_method = TRK_METHOD_CD;
            }
            break;

        case eDiscDVD:
            if (discKind.Bit.Media == eMediaROM)
            {
                tracking_method = TRK_METHOD_DVD_ROM;
            }
            else
            {
                tracking_method = TRK_METHOD_DVD_R_RW;
            }
            break;

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (discKind.Bit.Media == eMediaROM)
            {
                tracking_method = TRK_METHOD_HDDVD_ROM;
            }
            else
            {
                tracking_method = TRK_METHOD_HDDVD_R_RW;
            }
            break;
#endif
        case eDiscBD:
        default:
            if (discKind.Bit.Media == eMediaROM)
            {
                tracking_method = TRK_METHOD_BD_ROM;
            }
            else
            {
                tracking_method = TRK_METHOD_BD_R_RE;
            }
            break;
    }
    return tracking_method;
}

#if (COLLI_SWITCH == 1)
/*********************************************************
* Module: getInitColliPos()
*
* Description: Gets Colli position setting from EEPROM
* Input: layer, media
* Returns: USHORT colli position
**********************************************************/
USHORT getInitColliPos(BYTE layer, eDiscStd media)
{
    USHORT position;

    #if (OPU == KEM350AAA)
    switch (media)
    {
        case eDiscCD:
            position = ReadOPU(ADDSAACTCD);
            break;

        case eDiscDVD:
            position = ((layer == 1)? (ReadOPU(ADDSAACTDVD) - (DVD_DL_COLLI_DIFF / 2)) : (ReadOPU(ADDSAACTDVD) + (DVD_DL_COLLI_DIFF / 2)));
            break;

        case eDiscBD:
        default:
            position = ReadOPU(ADDSAACTBD);
            if (layer == 1)
            {
                position -= ReadOPU(ADDSAACTBDL0L1);
            }
            break;
    }
    #elif (OPU == BDPL2)
    switch (media)
    {
        case eDiscCD:
            position = OPUBarcode.CDSA;
            break;

        case eDiscDVD:
            position = OPUBarcode.DVSA;
            break;

        case eDiscBD:
        default:
            position = ((layer == 1)? OPUBarcode.SA_ACT_BD_L1 : OPUBarcode.SA_ACT_BD_L0);
            break;
    }
    #else
    position = ((layer == 1)? OPUBarcode.SA_ACT_BD_L1 : OPUBarcode.SA_ACT_BD_L0);
    #endif // OPU

    return (32 * position);
}
#endif

/*******************************************************
SetEqRegsForASpeed

********************************************************/
void SetEqRegsForASpeed(discKindT DiscKind, ESpindleSpeed speed)
{
    BYTE           i, j;
    USHORT         TRKGainFactor,FCSGainFactor;
#if (SRC_ENABLE == 1)
    LONG           src_kx_val;
#endif
#if (SRC_ENABLE == 1) || (CHIP_REV >= 0xC0)
    LONG            temp_long;
#endif
    const svoWordU *ptrLoadDspCoefArray[EQ_TBL_MAX+1];
#if (CHIP_REV >= 0xC0)
    const SEEK_GAIN *KICKLVL_tbl;
    BYTE            kspdlim_ratio;
#endif

    switch (DiscKind.Bit.DiscStd)
    {
        case eDiscUnknown:
        case eDiscCD:
            if (speed > MAX_SPEED_CD)
                return;

            for (i = 0; i <= EQ_TBL_MAX; i++)
            {
                ptrLoadDspCoefArray[i] = svoEQCoefTbl[speed][i];
            }
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                // DVD-RAM
                if (speed > MAX_SPEED_DVDRAM)
                    return;

                for (i = 0; i <= EQ_TBL_MAX; i++)
                {
                    ptrLoadDspCoefArray[i] = dvRamSvoEQCoefTbl[speed][i];
                }
            }
            else
#endif // #if (DVD_RAM_READ == 1)
            {
                // not DVD-RAM
                if (speed > MAX_SPEED_DVD)
                    return;

                for (i = 0; i <= EQ_TBL_MAX; i++)
                {
                    if ((DiscKind.Bit.Media == eMediaR) || (DiscKind.Bit.Media == eMediaRW))/*wobble disc*/
                    {
                        ptrLoadDspCoefArray[i] = rdvSvoEQCoefTbl[speed][i];
                    }
                    else
                    {
                        ptrLoadDspCoefArray[i] = dvSvoEQCoefTbl[speed][i];
                    }
                }
            }/* Not DVD-RAM*/
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            if (speed > MAX_SPEED_BD)
                return;

            for (i = 0; i <= EQ_TBL_MAX; i++)
            {
                if ((DiscKind.Bit.Media == eMediaR) || (DiscKind.Bit.Media == eMediaRW))/*wobble disc*/
                {
                    ptrLoadDspCoefArray[i] = bdrSvoEQCoefTbl[speed][i];
                }
                else
                {
                    ptrLoadDspCoefArray[i] = bdSvoEQCoefTbl[speed][i];
                }
            }
            break;
#endif // #if (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (speed > MAX_SPEED_HD)
                return;

            for (i = 0; i <= EQ_TBL_MAX; i++)
            {
                if ((DiscKind.Bit.Media == eMediaR)||(DiscKind.Bit.Media == eMediaRW))/*wobble disc*/
                {
                    ptrLoadDspCoefArray[i] = hdrSvoEQCoefTbl[speed][i];
                }
                else
                {
                    ptrLoadDspCoefArray[i] = hdSvoEQCoefTbl[speed][i];
                }
            }
            break;
#endif // #if (ENABLE_HDDVD == 1)
    }

    // load the coefficient values to DSP. So far, the EQ_addr is the same for all kinds of discs
    for (i = 0; i <= EQ_TBL_MAX; i++)
    {
        if (ptrLoadDspCoefArray[i] == NULL)
        {
            continue;
        }
        for (j = 0; ; j++)
        {
            if ((EQ_addr[i][j] == EQ_END) && (EQ_addr[i][j+1] == EQ_END))
            {
                break;
            }

            if ((i == SLED_EQ) && (j == 0)) //set ks10. Because SLO EQ input = TELFG * KTG.
            {
                WRITE_DSP_COEF(EQ_addr[i][j],((LONG)ptrLoadDspCoefArray[i][j].all16 * READ_DSP_COEF(src_kt2)) / 0x8000);
            }
            else if (i == DISK_FG_EQ)
            {
                switch (j)
                {
                    case 0://kdf10
                    case 4://kdf14
                        //handle together with kdf16
                        break;

                    case 6://kdf16
                        //handle kdf10
#if (NEW_SPINDLE_CONTROL == 1)
                        svoSpeedVar.Kd10Fg = ptrLoadDspCoefArray[i][j-6].all16 * ptrLoadDspCoefArray[i][j-4].all16 / 32768; //kd10*kd12
#else
                        svoSpeedVar.Kd10Fg = ptrLoadDspCoefArray[i][j-6].all16;
#endif
#if (DISC_8CM == 1)
                        if (svoVar_discDiameter == e8cm)
                        {
                            ULONG kd10_ratio_8cm;

                            switch (DiscKind.Bit.DiscStd)
                            {
                                case eDiscCD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_8CM_CD;
                                    kd10_ratio_8cm = DISK_KD10_RATIO_8CM_CD;
                                    break;

                                case eDiscDVD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_8CM_DVD;
    #if (DVD_RAM_READ == 1)
                                    if (DVDRAM)
                                    {
                                        kd10_ratio_8cm = DISK_KD10_RATIO_8CM_DVDRAM;
                                    }
                                    else
    #endif
                                    {
                                        kd10_ratio_8cm = DISK_KD10_RATIO_8CM_DVD;
                                    }
                                    break;

    #if (BD_ENABLE == 1)
                                case eDiscBD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_8CM_BD;
                                    kd10_ratio_8cm = DISK_KD10_RATIO_8CM_BD;
                                    break;
    #endif

    #if (ENABLE_HDDVD == 1)
                                case eDiscHD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_8CM_HD;
                                    kd10_ratio_8cm = DISK_KD10_RATIO_8CM_HD;
                                    break;
    #endif
                            }
                            svoSpeedVar.Kd10Fg = (ULONG)svoSpeedVar.Kd10Fg * kd10_ratio_8cm / 0x8000;
                        }
                        else
#endif // #if (DISC_8CM == 1)
                        {
                            switch (DiscKind.Bit.DiscStd)
                            {
                                case eDiscCD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_CD;
                                    break;

                                case eDiscDVD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_DVD;
                                    break;

#if (BD_ENABLE == 1)
                                case eDiscBD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_BD;
                                    break;
#endif

#if (ENABLE_HDDVD == 1)
                                case eDiscHD:
                                    svoSpeedVar.SpdAfcg = DISK_AFCG_HD;
                                    break;
#endif
                            }
                        }
                        WRITE_FIELD(HAL_SRV_AFCG, svoSpeedVar.SpdAfcg);

                        //handle kdf14
                        svoSpeedVar.Kd14Fg = ptrLoadDspCoefArray[i][j-2].all16 * DMODAC_FACTOR;
                        if (svoSpeedVar.Kd14Fg > DMO_CLIP)
                        {
                            svoSpeedVar.Kd10Fg = svoSpeedVar.Kd10Fg * svoSpeedVar.Kd14Fg / DMO_CLIP;
                            svoSpeedVar.Kd14Fg = DMO_CLIP;
                        }
                        spdSetKd14(svoSpeedVar.Kd14Fg);
#if (DVD_RAM_READ == 1)
                        if (!DVDRAM)
#endif
                        {
                            if ((DMO_CLIP_SLED_MOVING < svoSpeedVar.Kd14Fg) && svoStp_busy())
                            {
                                spdSetKd14(DMO_CLIP_SLED_MOVING);
                            }
                        }

                        //handle kdf16
#if (NEW_SPINDLE_CONTROL == 1)
                        svoSpeedVar.Kd16Fg = ptrLoadDspCoefArray[i][j-6].all16 * ptrLoadDspCoefArray[i][j].all16 / 32768; //kd10*kd16
                        svoSpeedVar.LostFg = 0; // 32-bit integrator does not need LostFG compensation
                        svoSpeedVar.Kd10FgMin = (0x8000 - ptrLoadDspCoefArray[i][j-3].all16) * 4;
#else
                        svoSpeedVar.LostFg = ptrLoadDspCoefArray[i][j+1].all16;
                        svoSpeedVar.Kd10FgMin = (0x8000 - ptrLoadDspCoefArray[i][j-3].all16) * 0x8000 * 4 / ptrLoadDspCoefArray[i][j-4].all16;
#endif
                        if ((speed > 0) && (svoSpeedVar.TargetRpm > 0))
                        {
                            //update kd10
                            spdSetFGFreq(svoSpeedVar.TargetRpm);
                        }
                        break;

                    default:
                        WRITE_DSP_COEF(EQ_addr[i][j],(USHORT)ptrLoadDspCoefArray[i][j].all16);
                        break;
                }
            }
            else
            {
                WRITE_DSP_COEF(EQ_addr[i][j],(USHORT)ptrLoadDspCoefArray[i][j].all16);
            }
        }/*for j=0;;j++*/
    }/*for i=0;i<=EQ_TBL_MAX*/

    //Do not set EQ parameters which don't exist in none _speed_tbl. This will prevent wong LostFg to be used in spdSetFGFreq() and caculate wrong AFCG.
    if (speed == NONE)
    {
        SetSvoEqBank(Ce_Lpf_Eq, CELPF_1kHz, 128);
        return;
    }

    switch (DiscKind.Bit.DiscStd)
    {
        case eDiscUnknown:
        case eDiscCD:
#if (CHIP_REV >= 0xC0)
            KICKLVL_tbl = svoKICKLVLTbl[speed - 1];
            kspdlim_ratio = KSPDLIM_FACTOR_CD;
#endif
            TRKGainFactor = TRACKING_GAINUP_FACTOR_CD;
            FCSGainFactor = FOCUS_GAINUP_FACTOR_CD;
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
#if (CHIP_REV >= 0xC0)
                KICKLVL_tbl  = dvRamSvoKICKLVLTbl[speed - 1];
                kspdlim_ratio = KSPDLIM_FACTOR_DVDRAM;
#endif
                TRKGainFactor = TRACKING_GAINUP_FACTOR_DVDRAM;
                FCSGainFactor = FOCUS_GAINUP_FACTOR_DVDRAM;
            }
            else
#endif // #if (DVD_RAM_READ == 1)
            {
                if ((DiscKind.Bit.Media == eMediaR) || (DiscKind.Bit.Media == eMediaRW))
                {   // DVD R/RW
#if (CHIP_REV >= 0xC0)
                    KICKLVL_tbl  = rdvsvoKICKLVLTbl[speed - 1];
                    kspdlim_ratio = KSPDLIM_FACTOR_DVDR;
#endif
                    TRKGainFactor = TRACKING_GAINUP_FACTOR_DVDR;
                    FCSGainFactor = FOCUS_GAINUP_FACTOR_DVDR;
                }
                else
                {   // DVD-ROM
#if (CHIP_REV >= 0xC0)
                    KICKLVL_tbl  = dvsvoKICKLVLTbl[speed - 1];
                    kspdlim_ratio = KSPDLIM_FACTOR_DVDROM;
#endif
                    TRKGainFactor = TRACKING_GAINUP_FACTOR_DVDROM;
                    FCSGainFactor = FOCUS_GAINUP_FACTOR_DVDROM;
                }
            }/* Not DVD-RAM*/
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            if ((DiscKind.Bit.Media == eMediaR) || (DiscKind.Bit.Media == eMediaRW))
            {
                // BD-R/RE
#if (CHIP_REV >= 0xC0)
                KICKLVL_tbl  = bdrsvoKICKLVLTbl[speed - 1];
                kspdlim_ratio = KSPDLIM_FACTOR_BDR;
#endif
                TRKGainFactor = TRACKING_GAINUP_FACTOR_BDR; // TBD? Comment! M.H. note: are these factors needed?
                FCSGainFactor = FOCUS_GAINUP_FACTOR_BDR;
            }
            else
            {
                // BD-ROM
#if (CHIP_REV >= 0xC0)
                KICKLVL_tbl  = bdsvoKICKLVLTbl[speed - 1];
                kspdlim_ratio = KSPDLIM_FACTOR_BDROM;
#endif
                TRKGainFactor = TRACKING_GAINUP_FACTOR_BDROM;
                FCSGainFactor = FOCUS_GAINUP_FACTOR_BDROM;
            }
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if ((DiscKind.Bit.Media == eMediaR) || (DiscKind.Bit.Media == eMediaRW))
            {   // HD R/RW
    #if (CHIP_REV >= 0xC0)
                KICKLVL_tbl  = hdrsvoKICKLVLTbl[speed - 1];
                kspdlim_ratio = KSPDLIM_FACTOR_HDR;
    #endif
                TRKGainFactor = TRACKING_GAINUP_FACTOR_HDR; // TBD? Comment! note: are these factors needed?
                FCSGainFactor = FOCUS_GAINUP_FACTOR_HDR;
            }
            else
            {   // HD-ROM
    #if (CHIP_REV >= 0xC0)
                KICKLVL_tbl  = hdsvoKICKLVLTbl[speed - 1];
                kspdlim_ratio = KSPDLIM_FACTOR_HDROM;
    #endif
                TRKGainFactor = TRACKING_GAINUP_FACTOR_HDROM;
                FCSGainFactor = FOCUS_GAINUP_FACTOR_HDROM;
            }
            break;
#endif
    }
#if (SUPPORT_DPD == 1)
    // CD use DPD DC-GAIN and SRC_GAIN must be boosted
    if ((DiscKind.Bit.DiscStd == eDiscCD) && (svoIramVar.do_cd_dpd_disc == TRUE))
    {
        SetSvoEqBank(Tracking_Eq, ptrLoadDspCoefArray[TRACKING_EQ], TRACKING_FACTOR_CD_DPD);
        TRKGainFactor = TRACKING_GAINUP_FACTOR_CD * TRACKING_FACTOR_CD_DPD / 128;
    }
#endif

#if (CD_MB_ONLY_OPU == 1)
    if ((DiscKind.Bit.DiscStd == eDiscCD) && (svoIramVar.do_cd_dpd_disc == FALSE))
    {
        if (svoVar_discKindT.Bit.Media != eMediaROM)
        {
            SetSvoEqBank(Tracking_Eq, ptrLoadDspCoefArray[TRACKING_EQ], TRACKING_FACTOR_MB_ONLY);
        }
        TRKGainFactor = TRACKING_GAINUP_FACTOR_CD * TRACKING_FACTOR_MB_ONLY / 128;
    }
#endif


#if (DVD_RAM_READ == 1)
    if (DVDRAM && (DiscTypeIndex == LOC_DISC_TYPE_10))
    {
        SetSvoEqBank(Tracking_Eq, ptrLoadDspCoefArray[TRACKING_EQ], TRACKING_FACTOR_DVDRAM_VER_10);
        TRKGainFactor = TRACKING_GAINUP_FACTOR_DVDRAM * TRACKING_FACTOR_DVDRAM_VER_10 / 128;
    }
#endif

    if ((speed > 0) && (svoSpeedVar.TargetRpm > 0))
    {
        //update kd10
        spdSetFGFreq(svoSpeedVar.TargetRpm);
    }

    /* program gain up ktu/kfu bank */
    if (ptrLoadDspCoefArray[FOCUS_EQ] != NULL)
    {
        SetSvoEqBank(Focus_U_Eq, ptrLoadDspCoefArray[FOCUS_EQ], FCSGainFactor);
        //SetSvoEqBank(Focus_W_Eq, ptrLoadDspCoefArray[FOCUS_EQ], 128);
    }

    if (ptrLoadDspCoefArray[TRACKING_EQ] != NULL)
    {
        SetSvoEqBank(Tracking_U_Eq, ptrLoadDspCoefArray[TRACKING_EQ], TRKGainFactor);
        //SetSvoEqBank(Tracking_W_Eq, ptrLoadDspCoefArray[TRACKING_EQ], 128);
    }

#if (CHIP_REV >= 0xC0)
    // Set Kspdlim table for fine seek
    #if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        temp_long = READ_DSP_COEF(kspd_1024) & 0x00FF; //Get highest speed
        temp_long = temp_long * kspdlim_ratio / 128;
        for (i = 0; i < 10; i++)
        {
            WRITE_DSP_COEF(kspdlim_0 + i * 4, (USHORT)temp_long);
        }
    }
    else
    #endif
    {
        temp_long = 2;
        for (i = 1; i < 10; i++)
        {
            temp_long *= 2;
            for (;; KICKLVL_tbl++)
            {
                if (KICKLVL_tbl->jump_count > temp_long)
                {
                    WRITE_DSP_COEF(kspdlim_0 + i * 4,KICKLVL_tbl->gain_data * kspdlim_ratio / 128);
                    break;
                }
            }
        }
        WRITE_DSP_COEF(kspdlim_0,READ_DSP_COEF(kspdlim_4));
    }
#endif

#if (SRC_ENABLE == 1)
// In this modified approach SRC_XXX_PRESCALE is determined by svo_para file now.
// So, it does not to be fixed to 0x1000 (as determined by DSP init value).
// The consequence is that src_kt/src_kf will be calculated by FW in function of src_kt2/src_kf2
// instead of being read directly from svo_coef_xxx file.
// Also, it is advised to make SCR_XXX_PRESCALE smaller than 0x1000 in order
// to make src_kt/src_kf larger (however, without overflowing beyond 0x7FFF)
// This will decrease quantisation error in DC-blocking filter considerably,
// which at least will be beneficial for sledge tracking during play mode.
    src_kx_val = ((LONG)READ_DSP_COEF(src_kt2) * 0x8000) / SRC_TRK_PRESCALE;
    if (src_kx_val > 32767) src_kx_val = 32767;
    else if (src_kx_val < -32768) src_kx_val = -32768;
    WRITE_DSP_COEF(src_trk_prescale,(USHORT)src_kx_val);
    WRITE_DSP_COEF(src_kt, SRC_TRK_PRESCALE);
    src_kx_val = ((LONG)READ_DSP_COEF(src_kf2) * 0x8000) / SRC_FOC_PRESCALE;
    if (src_kx_val > 32767) src_kx_val = 32767;
    else if (src_kx_val < -32768) src_kx_val = -32768;
    WRITE_DSP_COEF(src_foc_prescale,(USHORT)src_kx_val);
    WRITE_DSP_COEF(src_kf, SRC_FOC_PRESCALE);
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: SetSvoEqBank()
 * Description: Set EQ bank based on input data and gain up factor!!
 *
 *----------------------------------------------------------------------------*/
void SetSvoEqBank(const WORD *eq_addr, const svoWordU *data, USHORT gain_ratio)
{
    BYTE i,j,length;
    LONG temp;

    for (length = 0;;length++)
    {
        if ((eq_addr[length] == EQ_END) && (eq_addr[length+1] == EQ_END))
            break;
    }

    if (eq_addr[10] == src_kt2)
    {
        j = 3;
    }
    else
    {
        j = 2;
    }

    for (i = 0; i < length; i++)
    {
        //if gain_ratio < 128, change the first 2 EQ_addr; if gain_ratio > 128, change the last 2 EQ_addr
        if (i >= (length - j))
        {
            temp = (LONG)((svoWordU*)(data+i)->all16s) * gain_ratio / 128;
            if (temp > 32767)
            {
                SendMsgCn(SHOW_DEBUG_MSG,3,0x33FF36,A1B(i),A2B(eq_addr[i]));
                temp = 32767;
            }
            if (temp < -32768)
            {
                SendMsgCn(SHOW_DEBUG_MSG,3,0x33FF37,A1B(i),A2B(eq_addr[i]));
                temp = -32768;
            }

            WRITE_DSP_COEF(eq_addr[i],temp);
        }
        else
        {
            WRITE_DSP_COEF(eq_addr[i],(data+i)->all16);
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: svo_Block_Eq_Data_Set()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    svo_Block_Eq_Data_Set(ESpindleSpeed speed,BYTE eq_type)
{
    int i;

    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscUnknown))
    {
        if (speed > MAX_SPEED_CD)
        {
            return;
        }
        if (svoEQCoefTbl[speed][eq_type] == NULL)
        {
            speed = 0;
        }
        for (i=0;;i++)
        {
            if ((EQ_addr[eq_type][i] == EQ_END)&&(EQ_addr[eq_type][i+1] == EQ_END))
            {
                break;
            }
            WRITE_DSP_COEF(EQ_addr[eq_type][i],(USHORT)svoEQCoefTbl[speed][eq_type][i].all16);
        }
    }
    else if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {   //DVD
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            if (speed > MAX_SPEED_DVDRAM)
            {
                return;
            }

            if (dvRamSvoEQCoefTbl[speed][eq_type] == NULL)
            {
                speed = 0;
            }
            for(i=0;;i++)
            {
                if ((dvEQ_addr[eq_type][i] == EQ_END)&&(dvEQ_addr[eq_type][i+1] == EQ_END))
                {
                    break;
                }
                WRITE_DSP_COEF(dvEQ_addr[eq_type][i],(USHORT)dvRamSvoEQCoefTbl[speed][eq_type][i].all16);
            }
        }
        else
#endif // #if (DVD_RAM_READ == 1)
        {
            //not DVDRAM
            if (speed > MAX_SPEED_DVD)
            {
                return;
            }

            if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
            {
                if (rdvSvoEQCoefTbl[speed][eq_type] == NULL)
                {
                    speed = 0;
                }
            }
            else /*ROM disc*/
            {
                if (dvSvoEQCoefTbl[speed][eq_type] == NULL)
                {
                    speed = 0;
                }
            }
            for (i=0;;i++)
            {
                if ((dvEQ_addr[eq_type][i] == EQ_END)&&(dvEQ_addr[eq_type][i+1] == EQ_END))
                {
                    break;
                }
                if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
                {
                    WRITE_DSP_COEF(dvEQ_addr[eq_type][i],(USHORT)rdvSvoEQCoefTbl[speed][eq_type][i].all16);
                }
                else /*ROM disc*/
                {
                    WRITE_DSP_COEF(dvEQ_addr[eq_type][i],(USHORT)dvSvoEQCoefTbl[speed][eq_type][i].all16);
                }
            }
        }
    }

#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD) //BD disc
    {
        if (speed > MAX_SPEED_BD)
        {
            return;
        }

        if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
        {
            if (bdrSvoEQCoefTbl[speed][eq_type] == NULL)
            {
                speed = 0;
            }
        }
        else /*ROM disc*/
        {
            if (bdSvoEQCoefTbl[speed][eq_type] == NULL)
            {
                speed = 0;
            }
        }
        for (i=0;;i++)
        {
            if ((bdEQ_addr[eq_type][i] == EQ_END)&&(bdEQ_addr[eq_type][i+1] == EQ_END))
            {
                break;
            }
            if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
            {
                WRITE_DSP_COEF(bdEQ_addr[eq_type][i],(USHORT)bdrSvoEQCoefTbl[speed][eq_type][i].all16);
            }
            else /*ROM disc*/
            {
                WRITE_DSP_COEF(bdEQ_addr[eq_type][i],(USHORT)bdSvoEQCoefTbl[speed][eq_type][i].all16);
            }
        }
    }// BD disc
#endif

#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD) //HD disc
    {
        if (speed > MAX_SPEED_HD)
        {
            return;
        }

        if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
        {
            if (hdrSvoEQCoefTbl[speed][eq_type] == NULL)
            {
                speed = 0;
            }
        }
        else /*ROM disc*/
        {
            if (hdSvoEQCoefTbl[speed][eq_type] == NULL)
            {
                speed = 0;
            }
        }
        for (i=0;;i++)
        {
            if ((hdEQ_addr[eq_type][i] == EQ_END)&&(hdEQ_addr[eq_type][i+1] == EQ_END))
            {
                break;
            }
            if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
            {
                WRITE_DSP_COEF(hdEQ_addr[eq_type][i],(USHORT)hdrSvoEQCoefTbl[speed][eq_type][i].all16);
            }
            else /*ROM disc*/
            {
                WRITE_DSP_COEF(hdEQ_addr[eq_type][i],(USHORT)hdSvoEQCoefTbl[speed][eq_type][i].all16);
            }
        }
    }// HD disc
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: svo_Eq_Tbl_Data()
 * Description:
 *
 *----------------------------------------------------------------------------*/
USHORT  svo_Eq_Tbl_Data(ESpindleSpeed speed,WORD eq_addr)
{
    int i, j;

    if (speed == 0xFF) //check error speed
    {
        speed = 0;
    }

    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscUnknown))
    {
        for (i = 0; i <= EQ_TBL_MAX; i++)
        {
            for (j = 0; ; j++)
            {
                if ((EQ_addr[i][j] == EQ_END) && (EQ_addr[i][j+1] == EQ_END))
                {
                    break;
                }
                if (EQ_addr[i][j] == eq_addr)
                {
                    if (svoEQCoefTbl[speed][i] == NULL)
                    {
                        speed = 0;
                    }
                    return((USHORT)svoEQCoefTbl[speed][i][j].all16);
                }
            }
        }
    }
    else if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    { //DVD
        for (i = 0; i <= EQ_TBL_MAX; i++)
        {
            for (j = 0; ; j++)
            {
                if ((dvEQ_addr[i][j] == EQ_END) && (dvEQ_addr[i][j+1] == EQ_END))
                {
                    break;
                }
                if (dvEQ_addr[i][j] == eq_addr)
                {
#if (DVD_RAM_READ == 1)
                    if (DVDRAM)
                    {
                        if (dvRamSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)dvRamSvoEQCoefTbl[speed][i][j].all16);
                    }
                    else
#endif
                    if ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
                    {
                        if (rdvSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)rdvSvoEQCoefTbl[speed][i][j].all16);
                    }
                    else
                    {
                        if (dvSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)dvSvoEQCoefTbl[speed][i][j].all16);
                    }
                }
            }
        }
    }
#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD) //BD
    {
        for(i = 0; i <= EQ_TBL_MAX; i++)
        {
            for (j = 0; ; j++)
            {
                if ((dvEQ_addr[i][j] == EQ_END) && (dvEQ_addr[i][j+1] == EQ_END))
                {
                    break;
                }
                if (dvEQ_addr[i][j] == eq_addr)
                {
                    if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
                    {
                        if(bdrSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)bdrSvoEQCoefTbl[speed][i][j].all16);
                    }
                    else //ROM
                    {
                        if(bdSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)bdSvoEQCoefTbl[speed][i][j].all16);
                    }
                }
            }
        }
    }
#endif

#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD) //HD
    {
        for(i = 0; i <= EQ_TBL_MAX; i++)
        {
            for (j = 0; ; j++)
            {
                if ((hdEQ_addr[i][j] == EQ_END) && (hdEQ_addr[i][j+1] == EQ_END))
                {
                    break;
                }
                if (hdEQ_addr[i][j] == eq_addr)
                {
                    if ((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW))/*wobble disc*/
                    {
                        if(hdrSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)hdrSvoEQCoefTbl[speed][i][j].all16);
                    }
                    else //ROM
                    {
                        if(hdSvoEQCoefTbl[speed][i] == NULL)
                        {
                            speed = 0;
                        }
                        return((USHORT)hdSvoEQCoefTbl[speed][i][j].all16);
                    }
                }
            }
        }
    }
#endif

    return 0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: svo_Adj_Data_Clr()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if (CD_DFE_IMPROVE_READ == 1)
extern BYTE DFE_CHG_CASE;
extern BYTE DFE_CHG_SEL;
#endif
void    svo_Adj_Data_Clr(void)
{
    WRITE_DSP_COEF(ktg, 0x4000);           /* ktg 0x4000 */
    WRITE_DSP_COEF(kfg, 0x4000);           /* kfg 0x4000 */

    WRITE_REG(HAL_SRVREG_MAP->INITR, HAL_SRV_BRES|HAL_SRV_RRES|HAL_SRV_FRES|HAL_SRV_TRES|HAL_SRV_FMTRES|HAL_SRV_FMDRES);

    svoCalVar.ceOfsData.all16 = 512;
    svoCalVar.teOfsData.all16 = 512;
    svoCalVar.feOfsData.all16 = 512;
    svoCalVar.rpOfsData.all16 = 512;
    svoCalVar.bsOfsData.all16 = 512;
    HAL_SERVO_SetFebc(0);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FMTHRESHO);  /* select Front Monitor Threshold Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET,svoCalVar.fmThrData.all16);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FMDELTAO);  /* select Front Monitor Delta Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET,svoCalVar.fmDelData.all16);
    WRITE_FIELD(HAL_AFE_APCVREFOS, svoCalOfsT4Var[APC_OS].normal_offset);
    afe_ajust_clr();

#if (CD_DFE_IMPROVE_READ == 1)
    DFE_CHG_CASE = 0;
    DFE_CHG_SEL = 0;
#endif

#if ((DVD_UNBALANCE_DISC_CHECK==1) || (CD_UNBALANCE_DISC_CHECK == 1) || (BD_UNBALANCE_DISC_CHECK == 1))
    svoVar_unbalanceDiscNum = 0;
#endif

    svoLIBoundary = 450000L;
#if(FORCE_DISK_KIND==1)
    if (DiscForce == TRUE)
    {
        svoCalVar.discChkDoneBits |= disc_chk_end |blank_chk_end|plus_minus_chk_end;
    }
    else
#endif
    {
#if (ENABLE_HDDVD == 1)
        if (svoVar_discKindT.Bit.DiscStd != eDiscHD)
#endif
        {
            svoCalVar.discChkDoneBits = 0;
        }
    }
    svoCalVar.calDoneBits = 0;
    svoCalVar.calGainDoneBits = 0;
    svoCalVar.calOfsDoneBits = 0;

    svoMiscVar.trackSlipCount = 0;
#if (FDP_TRKSLIP_MONITOR == 1)
    svoMiscVar.focusDropCount = 0;
#endif
#if(ERROR_CAL_START_UP_MONITOR ==1)
    svoMiscVar.calibrationErrorCount = 0;
#endif
#if (EN_WOBBLE == 1)
//TODO Wobble?    SetBitsSCReg_1B(rwATIPCtl,bOPSEARCH);//TBD:SE1
#endif

#if (CALIBRATION_MANAGEMENT == 0)
    svoVar_TiltStepperZone  = 0;
#endif
    svoPreTiltValue         = TILT_INIT_DAC;
    HAL_SERVO_SetTilt(svoPreTiltValue);

    svo_bReadZoneSpeedX10 = 0;

    bSetSpeedAgain = FALSE;

    svoSlipSign = OFF;

#if (DVD_RAM_READ == 1)
    svoIramVar.headerIDReadyFlag = FALSE;
    zone_change = 0;
#endif

//#if (DEFECT_LENGTH_MEASURE == 1)
//    libDisableDFCTInt();
//#endif
}

#if(FORCE_DISK_KIND==1)
/*-----------------------------------------------------------------------------
 *
 * Module: disc_kind_preset()
 * Description: Preset given disc type and let BE perform all necessary calibrations
 *
 *----------------------------------------------------------------------------*/
enum {
    eHostUnknownKind,
    eHostCDROMDisc,
    eHostCDRDisc,
    eHostCDRWDisc,
    eHostLSDisc,
    eHostDVDROMDisc,
    eHostDVDPlusRDisc,
    eHostDVDPlusRWDisc,
    eHostDVDMinusRDisc,
    eHostDVDMinusRWDisc,
    eHostDVDRAMDisc,
    eHostDVDROMDLDisc,
    eHostDVDPlusRDLDisc,
    eHostDVDPlusRWDLDisc,
    eHostDVDMinusRDLDisc,
    eHostDVDMinusRWDLDisc,
    eHostBDROMDisc,
    eHostBDRDisc,
    eHostBDREDisc,
    eHostBDROMDLDisc,
    eHostBDRDLDisc,
    eHostBDREDLDisc,
    eHostHDROMDisc,
    eHostCBHDROMDisc,
    eHostHDRDisc,
    eHostHDRWDisc,
    eHostHDRAMDisc,
    eHostHDROMDLDisc,
    eHostCBHDROMDLDisc,
    eHostHDRDLDisc,
    eHostHDRWDLDisc
};

void disc_kind_preset(USHORT eDisc)
{
    SendMsg80(SHOW_DEBUG_MSG,0x318000+eDisc);
    switch(eDisc)
    {
        case eHostCDROMDisc:                                /* CD-ROM DISC */
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscCD;
            break;

        case eHostCDRDisc:                                  /* CD-R DISC */
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscCD;
            break;

        case eHostCDRWDisc:                                 /* CD-RW DISC */
            svoVar_discKindT.Bit.Media    = eMediaRW;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscCD;
            break;

        case eHostDVDROMDisc:                               /* DVD-ROM SL DISC */
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDROMDLDisc:                             /* DVD-ROM DL PTP & DVD-ROM DL OTP DISC */
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDPlusRDisc:                             /* DVD+R DISC */
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblPlus;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDPlusRWDisc:                            /* DVD+RW DISC */
            svoVar_discKindT.Bit.Media    = eMediaRW;
            svoVar_discKindT.Bit.WblType  = eWblPlus;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDMinusRDisc:                            /* DVD-R DISC */
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblMinus;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDMinusRWDisc:                           /* DVD-RW DISC */
            svoVar_discKindT.Bit.Media    = eMediaRW;
            svoVar_discKindT.Bit.WblType  = eWblMinus;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDPlusRDLDisc:                           /* DVD+R DL DISC */
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblPlus;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostDVDMinusRDLDisc:                          /* DVD-R DL DISC */
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblMinus;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            break;

        case eHostHDROMDisc:                                /* HD-ROM DISC */
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscHD;
            break;

    #if (ENABLE_CBHD == 1)
        case eHostCBHDROMDisc:                                /* CBHD-ROM DISC */
            svoVar_discKindT.Bit.Media    = eMediaChina;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscHD;
            break;
    #endif

        case eHostHDROMDLDisc:                              /* HD-ROM DL DISC */
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscHD;
            break;

    #if (ENABLE_CBHD == 1)
        case eHostCBHDROMDLDisc:                              /* CBHD-ROM DL DISC */
            svoVar_discKindT.Bit.Media    = eMediaChina;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscHD;
            break;
    #endif

        case eHostDVDPlusRWDLDisc:                          /* DVD+RW DL DISC ??? */
        case eHostDVDMinusRWDLDisc:                         /* DVD-RW DL DISC ??? */
        case eHostBDROMDisc:
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        case eHostBDRDisc:
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        case eHostBDREDisc:
            svoVar_discKindT.Bit.Media    = eMediaRW;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        case eHostBDROMDLDisc:
            svoVar_discKindT.Bit.Media    = eMediaROM;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        case eHostBDRDLDisc:
            svoVar_discKindT.Bit.Media    = eMediaR;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        case eHostBDREDLDisc:
            svoVar_discKindT.Bit.Media    = eMediaRW;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersDouble;
            svoVar_discKindT.Bit.DiscStd  = eDiscBD;
            break;

        //case eHostHDROMDisc:
        case eHostHDRDisc:
        case eHostHDRWDisc:
        case eHostHDRAMDisc:
        //case eHostHDROMDLDisc:
        case eHostHDRDLDisc:
        case eHostHDRWDLDisc:
        case eHostUnknownKind:
        case eHostLSDisc:
        default:
            svoVar_discKindT.Bit.Media    = eMediaUnknown;
            svoVar_discKindT.Bit.WblType  = eWblUnknown;
            svoVar_discKindT.Bit.Layers   = eLayersSingle;
            svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
            //svoVar_discKindT.fDiscKind    = eUnknownKind;    //not needed
            break;
    }
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: svo_preset_media_par()
 * Description: preset media dependent parameters
 * TBD? COMMENT! Need formula for calculating numbers used for svoSeekVar.parNO, svoSeekVar.parFO, svoSeekVar.parFine, and svoSeekVar.parRough!
 *----------------------------------------------------------------------------*/
void svo_preset_media_par(void)
{
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
            svoSeekVar.parN0 = UM_2_SLED_STEP(24000) * UM_2_SLED_STEP(24000);
            svoSeekVar.parF0 = 0x30000;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {   //preset to DVDSL
#if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    if (DiscTypeIndex == LOC_DISC_TYPE_10)
                    {
                        svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(47084) * UM_2_SLED_STEP(47084) / 16384;
                        svoSeekVar.parFineInit = svoSeekVar.parFine = UM_2_SLED_STEP(45190);
                    }
                    else
                    {
                        svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(35432) * UM_2_SLED_STEP(35432) / 16384;
                        svoSeekVar.parFineInit = svoSeekVar.parFine = UM_2_SLED_STEP(30722);
                    }
                }
                else
#endif
                {
                    svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(35690) * UM_2_SLED_STEP(35690) / 16384;
                    svoSeekVar.parFineInit = svoSeekVar.parFine =UM_2_SLED_STEP(26290);
                }
            }
            else
            {   //preset to DVDDL
                svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(37428) * UM_2_SLED_STEP(37428) / 16384;
                svoSeekVar.parFineInit = svoSeekVar.parFine = UM_2_SLED_STEP(28927);
            }

            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_DVDROM;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_DVDROM;
            }
#if (DVD_RAM_READ == 1)
            else if (DVDRAM)
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_DVDRAM;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_DVDRAM;
            }
#endif
            else
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_DVDR_W;
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_DVDR_W;
#else
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_DVDROM;
#endif
            }

            svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            svoSeekVar.parN0       = UM_2_SLED_STEP(24000) * UM_2_SLED_STEP(24000);
            svoSeekVar.parF0       = 0x100000;
            svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(30873) * UM_2_SLED_STEP(30873) / 65536;
            svoSeekVar.parFineInit = svoSeekVar.parFine = UM_2_SLED_STEP(11363);

            svoSeekVar.parN0_Li    = UM_2_SLED_STEP(24000) * UM_2_SLED_STEP(24000);
            svoSeekVar.parF0_Li    = 0x100000;
            svoSeekVar.parRough_Li = UM_2_SLED_STEP(30873) * UM_2_SLED_STEP(30873) / 65536;
            svoSeekVar.parFine_Li  = UM_2_SLED_STEP(11363);

            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_BDROM;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_BDROM;
            }
            else
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_BDR_W;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_BDR_W;
            }
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            svoSeekVar.parN0       = UM_2_SLED_STEP(24100) * UM_2_SLED_STEP(24100);
            svoSeekVar.parF0       = 0x30000;
            svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(39758) * UM_2_SLED_STEP(39758) / 65536;
            svoSeekVar.parFineInit = svoSeekVar.parFine = UM_2_SLED_STEP(15075);

            svoSeekVar.parN0_Li    = UM_2_SLED_STEP(23794) * UM_2_SLED_STEP(23794);
            svoSeekVar.parF0_Li    = 0x1FFFF;
            svoSeekVar.parRough_Li = UM_2_SLED_STEP(36655) * UM_2_SLED_STEP(36655) / 16384;
            svoSeekVar.parFine_Li  = UM_2_SLED_STEP(30150);

            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_HDROM;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_HDROM;
            }
            else
            {
                svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_HDR_W;
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_HDR_W;
            }
            break;
#endif
        case eDiscCD:
        default:
            //preset to 80min disc
            svoSeekVar.parN0       = UM_2_SLED_STEP(25000) * UM_2_SLED_STEP(25000);
            svoSeekVar.parF0       = 0;

            //1.10m/s, 90min disc setting
            svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(42106) * UM_2_SLED_STEP(42106) / 4096;
            svoSeekVar.parFine     = ((ULONG)1280 * 1100 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100);
            svoSeekVar.parFineInit = ((ULONG)1280 * 1200 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100);

            svoSeekVar.parDelay2 = JUMP_DELAY_PER_TRACK_CD;
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (svoVar_discKindT.Bit.Media != eMediaROM)
            {
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_CDR_W;
            }
            else
#endif
            {
                svoSeekVar.parDelay3 = PLL_LOCK_LENGTH_CDROM;
            }

            svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
            svoIramVar.currentTime = svoADR2MSF(svoIramVar.currentID.SectorNum.all32s);
            break;
    }
    svoSeekVar.advance    = 0;
    svoSeekVar.jumpType   = 0;
    svoSeekVar.roughCount = 0;
    svoSeekVar.fineCount  = 0;
    svoStp_SetStepThreshold(FORW);
}

/*-----------------------------------------------------------------------------
 *
 * Module: svo_get_radius_from_pll()
 * Description: calculate radius(steps) based on motor RPM and PLL freq
 *
 *----------------------------------------------------------------------------*/
USHORT svo_get_radius_from_pll()
{
    ULONG  rpm, overspeedX100;
    USHORT radius;

    overspeedX100 = HAL_DFE_GetHfOverspeed(HAL_PLL_GetDfeInClkKHz());
#if (EN_WOBBLE == 1)
    if (svoVar_discKindT.Bit.Media != eMediaROM)
    {
        switch (svoVar_discKindT.Bit.DiscStd)
        {
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            case eDiscCD:
            case eDiscDVD:
    #endif
    #if (BD_ENABLE == 1)
            case eDiscBD:
    #endif
    #if (ENABLE_HDDVD == 1)
            case eDiscHD:
    #endif
                if (!HAL_DFE_GetPllInLock())
                {
                    overspeedX100 = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());
                }
                break;
        }
    }
#endif
    rpm = spdGetCurrentRPM();
    if ((overspeedX100 * rpm) == 0)
    {
        return 0;
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 75 (CD)                                                                     *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm / (45 * parFine)                                        *
            *                                                                                   *
            ************************************************************************************/

            radius = ((ULONG)overspeedX100 * (45 * svoSeekVar.parFineInit / 2)) / (16 * rpm);
            break;

        case eDiscDVD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 26156250 / 38688 = 676.08 (DVD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 25 * R_step * rpm / (317 * parFine)                                       *
            *                                                                                   *
            ************************************************************************************/

            radius = ((ULONG)overspeedX100 * 317 * svoSeekVar.parFineInit) / (25 * rpm);
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 66000000 / 29946 = 2203.97 (BD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 28  * R_step * rpm / (1157 * parFine)                                     *
            *                                                                                   *
            ************************************************************************************/

            radius = ((ULONG)overspeedX100 * 1157 * svoSeekVar.parFineInit) / (28 * rpm);
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 64800000 / 29016 = 2233.25 (HD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm  / (1340 * parFine)                                     *
            *                                                                                   *
            ************************************************************************************/
            radius = ((ULONG)overspeedX100 * 670 * svoSeekVar.parFineInit) / (16 * rpm);
            break;
#endif // (ENABLE_HDDVD == 1)

        default:
            radius = 0;
            break;
    }
    return radius;
}
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svo_get_parFine_from_pll()
 * Description: calculate ParFine based on motor RPM, radius and PLL freq
 *
 *----------------------------------------------------------------------------*/
USHORT svo_get_parFine_from_pll(USHORT radius)
{
    ULONG  rpm, overspeedX100;
    USHORT parFine;

    overspeedX100 = HAL_DFE_GetHfOverspeed(HAL_PLL_GetDfeInClkKHz());
#if (EN_WOBBLE == 1)
    if (svoVar_discKindT.Bit.Media != eMediaROM)
    {
        switch (svoVar_discKindT.Bit.DiscStd)
        {
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            case eDiscCD:
            case eDiscDVD:
    #endif
    #if (BD_ENABLE == 1)
            case eDiscBD:
    #endif
    #if (ENABLE_HDDVD == 1)
            case eDiscHD:
    #endif
                if (!HAL_DFE_GetPllInLock())
                {
                    overspeedX100 = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());
                }
                break;
        }
    }
#endif
    rpm = spdGetCurrentRPM();
    if ((overspeedX100 * rpm) == 0)
    {
        return 0;
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 75 (CD)                                                                     *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm / (45 * parFine)                                        *
            * parFine =  R_step * 32 * rpm / (100*N * 45)                                                                                  *
            ************************************************************************************/
            parFine = ((ULONG)radius * (32 * rpm)) / ( overspeedX100 * 45) ;

            break;

        case eDiscDVD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 26156250 / 38688 = 676.08 (DVD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 25 * R_step * rpm / (317 * parFine)                                       *
            * parFine =  R_step * 25 * rpm / (100*N * 317)                                                                                   *
            ************************************************************************************/
            parFine = ((ULONG)radius * (25 * rpm)) / ( overspeedX100 * 317) ;
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 66000000 / 29946 = 2203.97 (BD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 28  * R_step * rpm / (1157 * parFine)                                     *
            * parFine =  R_step * 28 * rpm / (100*N * 1157)                                                                                   *
            ************************************************************************************/
            parFine = ((ULONG)radius * (28 * rpm)) / ( overspeedX100 * 1157) ;
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 64800000 / 29016 = 2233.25 (HD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm  / (1340 * parFine)                                     *
            * parFine =  R_step * 16 * rpm / (100*N * 670)                                                                                   *
            ************************************************************************************/
            parFine = ((ULONG)radius * (16 * rpm)) / ( overspeedX100 * 670) ;
            break;
#endif // (ENABLE_HDDVD == 1)

        default:
            parFine = 0;
            break;
    }
    return parFine;
}
#endif
/*-----------------------------------------------------------------------------
 *
 * Module: svoEnTimeRead()
 * Description: enable SUBQ and ATIP read
 *
 *----------------------------------------------------------------------------*/
extern BYTE Send_timef;

void svoEnTimeRead(void)
{
    {
        svoEnSubqRead();
    #if (EN_WOBBLE == 1)
        #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        #endif
            HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
    #endif
    }

#if (MSG_OUT_MODE == MSG_USBPORT)
    if (Send_timef && (hifDebugMsgFlag & SEND_QCODE_DATAID))
    {
        if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
            svoEnSubqRead();
    }
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoEnAtipRead()
 * Description: enable ATIP read
 *
 *----------------------------------------------------------------------------*/
void svoEnAtipRead(void)
{
#if (EN_WOBBLE == 1)
    BYTE temp_IE;

    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    #endif //(WOBBLE_READ_ON_DVD_OR_CD == 0)
    {
        temp_IE = GET_INT_ENABLE_MASK();            /* disable interrupts */
        DISABLE_INTERRUPT();

        svoIramVar.atipReadyFlag = FALSE;
        if(!HAL_WOBBLE_GetNewAddIntEnable())
        {
            HAL_WOBBLE_ResetATIPNewAddInt();     /* ATIP interruput clear */
            HAL_WOBBLE_EnableNewWblAddInterrupt(TRUE);
        }

        SET_INT_ENABLE_MASK(temp_IE);               /* restore interrupts */
    }
#endif // (EN_WOBBLE == 1)
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoEnSubqRead()
 * Description: enable SUBQ read
 *
 *----------------------------------------------------------------------------*/
void svoEnSubqRead(void)
{
    BYTE temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();            /* disable interrupts */
    DISABLE_INTERRUPT();

    svoIramVar.subQReadyFlag = FALSE;
    if(READ_FIELD(HAL_CDD_SCOREN)!=1)    /* Sub-Q Interrunpt disabled ? */
    {
        WRITE_FIELD(HAL_CDD_SCOR,0);
        WRITE_FIELD(HAL_CDD_SCOREN,1);
        svoIramVar.subQEnableFlag = 1;
    }

    SET_INT_ENABLE_MASK(temp_IE);               /* restore interrupts */
}


/*-----------------------------------------------------------------------------
 *
 * Module: checkTimeData()
 * Description:  SUBQ and ATIP check
 *
 *----------------------------------------------------------------------------*/
#define ADR_FROM_RF  0x02
#define ADR_FROM_WOB 0x01
SHORT checkTimeData(void)
{
    BYTE        temp;
    SVOTimeS    atiptime;
    SubQCodeS   svoSubQData;
    long        lba;

    if((svoIramVar.testAreaReadEnableFlag == TRUE)&&(svoIramVar.opcIsrEnableFlag == 1)){
        return(TRUE);
    }
    if ((svoIramVar.atipReadyFlag == TRUE) || (svoIramVar.subQReadyFlag == TRUE)){
        DISABLE_INTERRUPT();
    }else{
        return(FALSE);
    }
#if (WOBBLE_READ_ON_DVD_OR_CD == 1) && (EN_WOBBLE == 1)  // Support ATIP Only when wobble is enabled
    if(svoVar_discKindT.fDiscKind == eCDROMDisc)
    {
        temp = ADR_FROM_RF;    /* if Stamp CD then SUB-Q Read */
    }
    else
    {
        HAL_WOBBLE_EnableNewWblAddInterrupt(TRUE);
        if(svoIramVar.svoCurrentSpeed > CLV_SPEED_CD)
        {
            if(svoCmdVar.cmdParam[4] != eWriteSeek)
            {
                temp = ADR_FROM_RF|ADR_FROM_WOB;
            }else
            {
                temp = ADR_FROM_WOB;
            }
        }
        else
        {
            if ((svoMiscVar.atipdisc_subq_diei_flag == TRUE) || (!(svoCalVar.discChkDoneBits & start_end)))
                temp = ADR_FROM_WOB;
            else
                temp = ADR_FROM_RF|ADR_FROM_WOB;
        }
    }
#else // (EN_WOBBLE == 1)
    temp = ADR_FROM_RF;
#endif // (EN_WOBBLE == 1)

    svoMiscVar.leadInOutFlag = LEAD_TRACK;

#if (EN_WOBBLE == 1) // Support ATIP Only when wobble is enabled
    if ((svoIramVar.atipReadyFlag == TRUE) && (temp & ADR_FROM_WOB))
    {
        pdbGetATIPTime(&atiptime.Time);
        if(CheckBCDTime(&atiptime.Time) == TRUE)
        {
            svoIramVar.currentTime.Time.Minute = BCD2HEX(atiptime.Time.Minute);    //faster
            svoIramVar.currentTime.Time.Second = BCD2HEX(atiptime.Time.Second);
            svoIramVar.currentTime.Time.Frame  = BCD2HEX(atiptime.Time.Frame);
            svoIramVar.currentTime.Direction = PLUS;
            if (svoIramVar.currentTime.Time.Minute >= LEAD_IN_MINUTE)
            {
                if (Iram_stp_CurrentPosition < UM_2_SLED_STEP(35000))
                {
                    svoIramVar.currentTime.Direction = MINUS;
                }
            }

            if((svoIramVar.subQEnableFlag & (~ SUBQ_EI_DI_SERVO)) == 0){
                HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
            }

            if (temp & ADR_FROM_RF)
            {
                WRITE_FIELD(HAL_CDD_SCOREN, 0); /* Disable SUB-Q interrupt - Stop SubQ decoding */
            }
            svoIramVar.currentID.Layer = 0;
            svoIramVar.currentID.SectorNum.all32s = svoIramVar.currentTime.Time.Minute * 4500L +
                                                    svoIramVar.currentTime.Time.Second * 75L +
                                                    svoIramVar.currentTime.Time.Frame;
            if (svoIramVar.currentTime.Direction == MINUS)
            {
                svoIramVar.currentID.SectorNum.all32s -= 450000L;
            }
            ENABLE_INTERRUPT();
            return(TRUE);
        }
    }
#endif // (EN_WOBBLE == 1)

    //--------------------------------------------------------------------------;
    // ROM or closed disc                                                       ;
    //--------------------------------------------------------------------------;
    if((svoIramVar.subQReadyFlag == TRUE) && (temp & ADR_FROM_RF))
    {
        svoIramVar.currentTime.Direction = PLUS;
        pdbGetSubQCode(&svoSubQData);

        if((svoSubQData.Addr != 0) && (svoSubQData.Zero == 0) &&
           ((svoSubQData.PMinute!=0) || (svoSubQData.PSecond!=0) || (svoSubQData.PFrame!=0)))
        {
            switch (svoSubQData.Tno)
            {
                case 0x00:    /* lead in area */
                    svoIramVar.currentTime.Time.Minute = BCD2HEX(svoSubQData.Minute);
                    svoIramVar.currentTime.Time.Second = BCD2HEX(svoSubQData.Second);
                    svoIramVar.currentTime.Time.Frame  = BCD2HEX(svoSubQData.Frame);
                    if ((Iram_stp_CurrentPosition > UM_2_SLED_STEP(52000)) && (svoIramVar.currentTime.Time.Minute <= 29))
                    {//support up to 129 min
                        if (svoPBA2Radius((svoIramVar.currentTime.Time.Minute + 100) * 4500) <= UM_2_SLED_STEP(60000))
                        {
                            svoIramVar.currentTime.Time.Minute += 100;
                        }
                    }
                    svoIramVar.currentID.Layer = 0;
                    svoIramVar.currentID.SectorNum.all32s = svoIramVar.currentTime.Time.Minute * 4500L +
                                                            svoIramVar.currentTime.Time.Second * 75L +
                                                            svoIramVar.currentTime.Time.Frame;
                    if(pdbGetReadInfoMethod() == eReadTOC)
                    {
                        switch(svoSubQData.Point)
                        {
                            case 1:  svoMiscVar.leadInOutFlag = LEAD_IN;
                                break;
                            case 0xA0:
                            case 0xA1:
                                if(pdbGetTrackSessionNumber(BCD2HEX(svoSubQData.PMinute)) == 1)
                                {
                                    svoMiscVar.leadInOutFlag = LEAD_IN;
                                }
                                break;
                            case 0xA2:  svoMiscVar.leadInOutFlag = LEAD_IN;
                                break;
                            default:
                                if(pdbGetTrackSessionNumber(BCD2HEX(svoSubQData.Point)) == 1)
                                {
                                    svoMiscVar.leadInOutFlag = LEAD_IN;
                                }
                                break;
                        }
                    }
                    else
                    {
                        svoMiscVar.leadInOutFlag = LEAD_IN;
                    }

                    if (((Iram_stp_CurrentPosition < (svoStepVar.StepPosCal? UM_2_SLED_STEP(26000) : UM_2_SLED_STEP(29000))) ||
                         (Iram_stp_CurrentPosition < svoPBA2Radius(svoIramVar.currentID.SectorNum.all32s) - UM_2_SLED_STEP(5000))) &&
                        (((svoCalVar.calDoneBits & boundary_end) == 0) || (svoIramVar.currentID.SectorNum.all32s + (8 * 4500) > svoLIBoundary)))
                    {
                        svoIramVar.currentTime.Direction = MINUS;
                    }
                    /* Treat all CDROM leadin address as 99:0:0 before svoLIBoundary is determined */
                    if (!svo_BoundaryDet)
                    {
                        if ((svoIramVar.currentTime.Direction == MINUS)&&
                            ((svoCalVar.calDoneBits & boundary_end)==0))
                        {
                            svoIramVar.currentTime.Time.Minute=99;
                            svoIramVar.currentTime.Time.Second=0;
                            svoIramVar.currentTime.Time.Frame=0;
                            svoIramVar.currentID.SectorNum.all32s = 445500;
                        }
                    }
                    //send_msg1('C','R',svoIramVar.currentTime.Direction,0);

                    if (svoIramVar.currentTime.Direction == MINUS)
                    {
                        svoIramVar.currentID.SectorNum.all32s -= svoLIBoundary;
                    }
                    if((svoIramVar.subQEnableFlag & (~ SUBQ_EI_DI_SERVO)) == 0)
                    {
                        WRITE_FIELD(HAL_CDD_SCOREN, 0); /* Disable SUB-Q interrupt - Stop SubQ decoding */
                    }
#if (EN_WOBBLE == 1)
                    if (temp & ADR_FROM_WOB)
                    {
                        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                            HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
                    }
#endif // (EN_WOBBLE == 1)

                    ENABLE_INTERRUPT();
                    return(TRUE);

                case 0xAA:    /* lead out area */
                    if(svoVar_discKindT.fDiscKind == eCDROMDisc)
                    {
                        atiptime.Time.Minute = BCD2HEX(svoSubQData.PMinute);
                        atiptime.Time.Second = BCD2HEX(svoSubQData.PSecond);
                        atiptime.Time.Frame  = BCD2HEX(svoSubQData.PFrame);
                        lba = atiptime.Time.Minute * 4500L + atiptime.Time.Second * 75L + atiptime.Time.Frame - 150L;
                        if (pdbGetEndTimeOfLastCloseSession() < lba)
                        {
                            svoMiscVar.leadInOutFlag = LEAD_OUT;
                        }
                    }
                    break;

                default :
                    break;
            }
            svoIramVar.currentTime.Time.Minute = BCD2HEX(svoSubQData.PMinute);
            svoIramVar.currentTime.Time.Second = BCD2HEX(svoSubQData.PSecond);
            svoIramVar.currentTime.Time.Frame  = BCD2HEX(svoSubQData.PFrame);
            if ((Iram_stp_CurrentPosition > UM_2_SLED_STEP(52000)) && (svoIramVar.currentTime.Time.Minute <= 29))
            {   //support up to 129 min
                if (svoPBA2Radius((svoIramVar.currentTime.Time.Minute + 100) * 4500) <= UM_2_SLED_STEP(60000))
                {
                    svoIramVar.currentTime.Time.Minute += 100;
                }
            }
            svoIramVar.currentID.Layer = 0;
            svoIramVar.currentID.SectorNum.all32s = svoIramVar.currentTime.Time.Minute * 4500L +
                                                    svoIramVar.currentTime.Time.Second * 75L +
                                                    svoIramVar.currentTime.Time.Frame;
            if (svoIramVar.currentTime.Direction == MINUS)
            {
                svoIramVar.currentID.SectorNum.all32s -= svoLIBoundary;
            }
            if((svoIramVar.subQEnableFlag & (~ SUBQ_EI_DI_SERVO)) == 0)
            {
                WRITE_FIELD(HAL_CDD_SCOREN, 0); /* Disable SUB-Q interrupt - Stop SubQ decoding */
            }

#if (EN_WOBBLE == 1)
            if (temp & ADR_FROM_WOB)
            {
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                    HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
            }
#endif // (EN_WOBBLE == 1)
            ENABLE_INTERRUPT();
            return(TRUE);
        }
    }
    ENABLE_INTERRUPT();
    return(FALSE);
}

/*-----------------------------------------------------------------------------
 *
 * Module: CheckHeaderID()
 * Description:  DVD/BD/HD header ID check
 *
 *----------------------------------------------------------------------------*/
#define CurrentIDIsReady    (((READ_REG(HAL_DVDDREG_MAP->DVDIRQS3)&HAL_DVDD_CIDI) == HAL_DVDD_CIDI) ? TRUE:FALSE)
#define CurrentIDIsValid    (((READ_REG(HAL_DVDDREG_MAP->DVDSTAT)&HAL_DVDD_CIDVLD) == HAL_DVDD_CIDVLD) ? TRUE:FALSE)
#if (ENABLE_HDDVD == 1)
#define MAX_NR_DECODER_FORMAT_TRY_OUT       32
static BYTE NrTryOutCheckDecodeFormat = 0;
#endif
SHORT CheckHeaderID(void)
{
    BYTE        temp;
    svoDWordU   currentID_temp;
    ULONG       PSN;
    SHORT       ret;
#if (BD_ENABLE == 1)
    ULONG       AUNStatus,AUNRegister,AUNEndRegister;
    ULONG       AUNFLYStatus,AUNFLYRegister,AUNFLYCFDStatus;
    BYTE        AUNDecodingStatus;
#endif

    SendMsg80(HEADER_ID_MSG,0x3B0600);//CheckHeaderID function entered

    temp = ADR_FROM_RF;
#if (EN_WOBBLE == 1)
    if ((svoVar_discKindT.Bit.Media != eMediaROM)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        && (svoVar_discKindT.Bit.DiscStd != eDiscDVD)
    #endif
    #if (BD_ENABLE == 1)
        && (svo_BDR_RE_PICReading == CLEAR)
    #endif
       )
    {
        temp |= ADR_FROM_WOB;
    }
#endif //(BD_ENABLE == 1)

    SendMsg80(HEADER_ID_MSG,0x3B0600+temp);//CheckHeaderID use WOB/DFE read
    ret = FALSE;

#if (EN_WOBBLE == 1)
    //--------------------------------------------------------------------------;
    // Wobble disc                                                              ;
    //--------------------------------------------------------------------------;
    if (pdbGetIDOk() && (temp & ADR_FROM_WOB))
    {
        DISABLE_INTERRUPT();
        switch (svoVar_discKindT.Bit.DiscStd)
        {
#if (BD_ENABLE == 1)
            case eDiscBD:
                if ((Iram_DiscInfo.BD.ClusterAddress > 0x400000) || (Iram_DiscInfo.BD.ClusterAddress < 0x10000))
                {  //prevent error get wobble address ,especially BDR/RE DL disc
                    pdbSetIDOk(FALSE);
                    ENABLE_INTERRUPT();
                    return(ret);
                }
                currentID_temp.all32 = (Iram_DiscInfo.BD.ClusterAddress << 3);
                PSN = svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMaskBD;
                SendMsgCn(DEBUG_WBL_MSG,4,0x410103,A4B(PSN));//BD wobble 2 PSN address
                svoIramVar.currentID.Layer = 0;
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    // bit 24 indicates layer in case BD
                    svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                }
                break;
#endif
            case eDiscDVD:
                currentID_temp.all32 = Iram_DiscInfo.DVD.ECCBLKAddress;
                PSN = svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMask;
                svoIramVar.currentID.Layer = 0;
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    svoIramVar.currentID.Layer = (currentID_temp.bytes.byte2 > 0x80)? 1 : 0;
                }
                break;

            default:
                pdbSetIDOk(FALSE);
                ENABLE_INTERRUPT();
                return(ret);
        }
#if (RESYNC_MESSAGES == 1)
        SendMsgCn(DEBUG_WBL_MSG,4,0x3FAA23,A4B(svoIramVar.currentID.SectorNum.all32));//decoder wobble address
#else
        SendMsgCn(DEBUG_WBL_MSG,4,0x410101,A4B(svoIramVar.currentID.SectorNum.all32));//BD decoder wobble address
#endif
        pdbSetIDOk(FALSE);
        ENABLE_INTERRUPT();
        ret = TRUE;
    }
    else
#endif
    if ((!DVDRAM) || (svoVar_TrackingMethod == TRK_METHOD_DPD))
    {
        //--------------------------------------------------------------------------;
        // ROM or RAM embossed area or closed disc                                                       ;
        //--------------------------------------------------------------------------;
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            WRITE_FIELD(HAL_DVDD_PIDEN,0);    // disable PID ready interrupt
        }
#endif
#if (BD_ENABLE == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            //read data and flywheel
            AUNFLYRegister    = (ULONG)READ_REG(HAL_BDCREG_MAP->BDAUNFLY); //BD AUN flywheel
            AUNFLYStatus      = (ULONG)READ_REG(HAL_BDCREG_MAP->BDAUNFLYS);//BD AUN flywheel status
            AUNStatus         = (ULONG)READ_REG(HAL_BDCREG_MAP->BDAUNS);   //BD AUN status
            AUNRegister       = (ULONG)READ_REG(HAL_BDCREG_MAP->BDAUN);    //BD AUN register
            AUNEndRegister    = (ULONG)READ_REG(HAL_BDCREG_MAP->BDAUNEND); //BD AUN end

            // check buffer full/ready
            if ((AUNStatus & HAL_BDC_AUNRDY) == HAL_BDC_AUNRDY)
            {
                AUNDecodingStatus = (BYTE)(AUNStatus & HAL_BDC_AUNDECOS);
                SendMsgCn(HEADER_ID_MSG,1,0x3B0607,A1B(AUNDecodingStatus));
                SendMsgCn(HEADER_ID_MSG,4,0x3B060B,A4B(AUNFLYStatus));
                AUNFLYCFDStatus = (BYTE)((AUNFLYStatus & HAL_BDC_AUNFLYCFD)>>2);
                SendMsgCn(HEADER_ID_MSG,4,0x3B060C,A4B(AUNFLYCFDStatus));
                if ( (AUNDecodingStatus == halBDC_ErrataFree) || (AUNDecodingStatus == halBDC_Correctable))
                {
                    if ((AUNFLYStatus & HAL_BDC_AUNFLYLCK) == 0)
                    {
                        return(ret);
                    }
                    //else if(!HAL_DFE_GetPllInLock())
                    else if(!HAL_DFE_GetPllLocK())
                    {
                        if(((AUNFLYStatus & HAL_BDC_AUNFLYLCK) == HAL_BDC_AUNFLYLCK)&&((AUNFLYStatus & HAL_BDC_DMDLCK) == HAL_BDC_DMDLCK))
                        {
                            currentID_temp.all32 = AUNFLYRegister; //flywheel AUN
                            SendMsgCn(HEADER_ID_MSG,4,0x3B060E,A4B(AUNFLYRegister));//CheckHeaderID BD AUN flywheel Address :
                        }
                        else
                        {
                            return(ret);
                        }
                    }
                    else
                    {
                        currentID_temp.all32 = AUNFLYRegister; //flywheel AUN
                        SendMsgCn(HEADER_ID_MSG,4,0x3B060E,A4B(AUNFLYRegister));//CheckHeaderID BD AUN flywheel Address :
                    }
                }
                else //uncorrectable or illegal state
                {
                    if((AUNFLYStatus & HAL_BDC_AUNFLYLCK) == HAL_BDC_AUNFLYLCK)
                    {
                        currentID_temp.all32 = AUNFLYRegister; //flywheel AUN
                        SendMsgCn(HEADER_ID_MSG,4,0x3B060E,A4B(AUNFLYRegister));//CheckHeaderID BD AUN flywheel Address :
                    }
                    else
                    {
                        return(ret);
                    }
                }
            }
            else
            {
                return(ret);
            }
            svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMaskBD;
            svoIramVar.currentID.Layer = 0;
            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                // bit 24 indicates layer in case BD
                svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
            }
        }
        else
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        // HD case and ID ready to be read
        if ((svoVar_discKindT.Bit.DiscStd == eDiscHD) && ((BOOL)READ_FIELD(HAL_DVDD_CIDI) == e_READY_CIDI))
        {
            DISABLE_INTERRUPT();
            if ((BOOL)READ_FIELD(HAL_DVDD_CIDVLD) == TRUE)
            {
                //send_msg80(HEADER_ID_MSG,0x5B0603); //CheckHeaderID CIDI interrupt and HAL_DVDD_CIDVLD valid
                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0);
                svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMask;
                CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                NrTryOutCheckDecodeFormat = 0;
            }
            else
            {
                //send_msg80(HEADER_ID_MSG,0x5B0604); //CheckHeaderID CIDI interrupt but HAL_DVDD_CIDVLD NOT valid
                //send_msg21(DEBUG_DFE_MSG,0x60000D, NrTryOutCheckDecodeFormat); //Try next decoder format count
    #if (ENABLE_CBHD == 1)
                if(!((svoCalVar.discChkDoneBits & cbhd_disc_chk_end) == cbhd_disc_chk_end)&&(NrTryOutCheckDecodeFormat>=MAX_NR_DECODER_FORMAT_TRY_OUT)) //Demod test
                {
                    SendMsg80(DEBUG_DFE_MSG,0x400010);//Try Modulation Perform
                    if (svoCmdVar.demodMode == eDemodETM)
                    {
                        SendMsg80(DEBUG_DFE_MSG,0x40000F);//Try FSM for CBHD
                        //force as CBHD disc
                        //WRITE_FIELD(HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
                        //WRITE_FIELD(HAL_GLOBAL_DL,0x00);
                        WRITE_FIELD(HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROMChina);
                        svoCmdVar.demodMode = eDemodFSM;
                        NrTryOutCheckDecodeFormat = 0;
                    }
                    else
                    {
                        SendMsg80(DEBUG_DFE_MSG,0x40000E);//Try ETM for HDDVD
                        //force as HDDVD disc
                        //WRITE_FIELD(HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
                        //WRITE_FIELD(HAL_GLOBAL_DL,0x00);
                        WRITE_FIELD(HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROM);
                        svoCmdVar.demodMode = eDemodETM;
                        NrTryOutCheckDecodeFormat = 0;
                    }
                }
    #endif
                NrTryOutCheckDecodeFormat++;

                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0);
                CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                ENABLE_INTERRUPT();
                return(ret);
            }

            //if (svoVar_getDVDFormatflag == 1)   // already formatted disc//TBD? this bit is set after getting Control data block while data is valid
            {
                //According current ID's information update system lead in/out flag
                if (currentID_temp.all32<HD_CONNECTION_ZONE_START_ADDR)
                {
                    svoMiscVar.HD_System_leadInOutFlag = HD_SYSTEM_LEAD_IN;
                }
                else if(currentID_temp.all32>HD_SYSTEM_LEADOUT_START_ADDR)
                {
                    svoMiscVar.HD_System_leadInOutFlag = HD_SYSTEM_LEAD_OUT;
                }
                else
                {
                    svoMiscVar.HD_System_leadInOutFlag = HD_SYSTEM_LEAD_TRACK;
                }
            }

            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                //According current ID's information and current ID's highest bit to judge HDs layer number
                if (svoVar_getDVDFormatflag == 1)   // already formatted disc
                {
                    if (pdbGetHDTrackPath() == DVDHD_OTP_DISC)   //HD OTP DL disc
                    {
                        if (currentID_temp.bytes.byte2 > 0x80)
                        {
                            svoIramVar.currentID.Layer = 1;
                        }
                        else
                        {
                            svoIramVar.currentID.Layer = 0;
                        }
                    }
                    else    //HD PTP DL disc
                    {
                        svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                    }
                }
                else
                {
                    if (currentID_temp.bytes.byte2 > 0x80)
                    {
                        svoIramVar.currentID.Layer = 1;
                    }
                    else
                    {
                        svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                    }
                }
            }
            ENABLE_INTERRUPT();
        }
        else
#endif // (ENABLE_HDDVD == 1)
        // DVD case and ID ready to be read
        if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && ((BOOL)READ_FIELD(HAL_DVDD_CIDI) == e_READY_CIDI))
        {
            DISABLE_INTERRUPT();
            if ((BOOL)READ_FIELD(HAL_DVDD_CIDVLD) == TRUE)
            {
                SendMsg80(HEADER_ID_MSG,0x3B0603);//CheckHeaderID CIDI interrupt and HAL_DVDD_CIDVLD valid
                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0);
                svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMask;
                CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                SendMsgCn(HEADER_ID_MSG,4,0x3B0610,A4B(svoIramVar.currentID.SectorNum.all32));//CheckHeaderID DVD Address
            }
            else
            {
                SendMsg80(HEADER_ID_MSG,0x3B0604);//CheckHeaderID CIDI interrupt but HAL_DVDD_CIDVLD NOT valid
                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0);
                CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                ENABLE_INTERRUPT();
                return(ret);
            }

            svoIramVar.currentID.Layer = 0;
            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                //According current ID's information and current ID's highest bit to judge DVDs layer number
                if (svoVar_getDVDFormatflag == 1)   // already formatted disc
                {
                    if (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)   //DVD OTP DL disc
                    {
                        if (currentID_temp.bytes.byte2 > 0x80)
                        {
                            svoIramVar.currentID.Layer = 1;
                        }
                        else
                        {
                            svoIramVar.currentID.Layer = 0;
                        }
                    }
                    else    //DVD PTP DL disc
                    {
                        svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                    }
                }
                else
                {
                    if (currentID_temp.bytes.byte2 > 0x80)
                    {
                        svoIramVar.currentID.Layer = 1;
                    }
                    else
                    {
                        svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                    }
                }
            }
            ENABLE_INTERRUPT();
        }
        else
        {
            return(ret);
        }

        // bits 23-0 represents Physical Sector Address in case of DVD
        // bits 23-1 represents Physical Sector Address in case of BD, bit 0 being always 0
        SendMsgCn(HEADER_ID_MSG,4,0x3B1602,A4B(svoIramVar.currentID.SectorNum.all32));//CheckHeaderID Address
        //DVD_MESSAGE2(PRN_W_O,"R ", 4, svoIramVar.currentID.SectorNum.all32, " ", 1, HAL_GLOBAL_GetRfPresent());
#if LPP_AUTO_LEARN_TEST == 1
        DVD_MESSAGE1(PRN_W_O,"R ", 4, svoIramVar.currentID.SectorNum.all32);
#endif

        if (svoIramVar.currentID.SectorNum.all32>0x00010000)    //prevent unreasonable decoded ID
        {
            ret = TRUE;
        }
    }
#if (DVD_RAM_READ == 1)
    //--------------------------------------------------------------------------;
    // DVD-RAM                                                                  ;
    //--------------------------------------------------------------------------;
    else
    {
        WRITE_FIELD(HAL_DVDD_PIDEN,1);    // Enable PID ready interrupt
        DISABLE_INTERRUPT();
        if ((svoIramVar.headerIDReadyFlag == TRUE) && (temp & ADR_FROM_RF))
        {
            svoIramVar.headerIDReadyFlag = FALSE;
            if ((RWDisc.dvRAM.CurrentPID >= 0x30000) && (RWDisc.dvRAM.CurrentPID <= 0x2741CF))
            {
                svoIramVar.currentID.SectorNum.all32 = RWDisc.dvRAM.CurrentPID;
                svoIramVar.currentID.Layer = 0;
                SendMsgCn(HEADER_ID_MSG,4,0x3B060F,A4B(svoIramVar.currentID.SectorNum.all32));
                ret = TRUE;
            }
        }
        ENABLE_INTERRUPT();
    }
#endif

    return(ret);

#if 0  // For Debug, please don't delete
    //--------------------------------------------------------------------------;
    // DVD-ROM/DVD+R/DVD-R/DVD+RW/DVD-RW                                        ;
    //--------------------------------------------------------------------------;
    if(svoVar_discKindT.fDiscKind != eDVDRAMDisc)
    {
        if(CurrentIDIsReady == TRUE)
        {
            if(CurrentIDIsValid == TRUE)
            {
                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0);
                CLEAR_INT_FIELD(HAL_DVDD_CIDI,0);
                svoIramVar.currentID.Layer = (currentID_temp.bytes.byte3 & LAYERMASK);
                svoIramVar.currentID.SectorNum.all32 = currentID_temp.all32 & PSNMask;

                ret = TRUE;
            }
            else
            {
                /* Error correction is failed, read the following registers to clear CIDI flag*/
                currentID_temp.all32 = READ_REG(HAL_DVDDREG_MAP->CURID0)
                CLEAR_INT_FIELD(HAL_DVDD_CIDI,0);
            }
        }
    }

    //--------------------------------------------------------------------------;
    // DVD-RAM                                                                  ;
    //--------------------------------------------------------------------------;
    else
    {
        ret = TRUE;
    }
    return(ret);
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: EFM_SMP_Control()
 * Description: Set EFM and SMP.
 *              Application Level
 *
 *----------------------------------------------------------------------------*/
void EFM_SMP_Control(BYTE EFMstate,BYTE SMPState)
{
    SendMsgCn(DEBUG_VENDOR_WRITE_MSG|DEBUG_CD_WRITE_MSG,0,0x350200+EFMstate);
    WRITE_FIELD(HAL_WS_WGEN,0); // Disable Write Gate
    WRITE_FIELD(HAL_WS_WGLVL,0); //Manual set Write Gate Control Level to low
}

/*-----------------------------------------------------------------------------
 *
 * Module: svo_Offset_Test_Write()
 * Description: set and Restore MPP SPP FE offset for RW Test Write
 * Input: 1: Set MPP SPP FE offset for RW Test Write
 *        0: Restore MPP SPP FE offset for RW Normal
 *
 *----------------------------------------------------------------------------*/
void svo_Offset_Test_Write(BYTE On_Off)
{
#if 0//TBD? disable this function
    if (On_Off == 1)
    {
        SetSCRegField_1B(f_rw_FEOSA,svoCalVar.feOffset[0]);
        SetSCRegField_1B(f_rw_MPPOSA,svoCalVar.mppOffset[0]);
        SetSCRegField_1B(f_rw_SPPOSA,svoCalVar.sppOffset[0]);
    }
    else
    {
        SetSCRegField_1B(f_rw_FEOSA,svoCalVar.feOffset[1]);
        SetSCRegField_1B(f_rw_MPPOSA,svoCalVar.mppOffset[1]);
        SetSCRegField_1B(f_rw_SPPOSA,svoCalVar.sppOffset[1]);
    }
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: timeout_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
extern  ULONG   svoPrvtVar_timeoutTimers[TIMEOUT_TIMER_MAX];    // time_out_timer[2];
private USHORT  svoPrvtVar_timeoutLimits[TIMEOUT_TIMER_MAX];    // svoTimeOut[2];
eTimerResult timeout_chk(BYTE no)
{
    ULONG temp_timer;

    if (svoPrvtVar_timeoutLimits[no] == TIMER_STOP)
    {
        return (eTimerStopped);
    }
    temp_timer = ReadTimer(&svoPrvtVar_timeoutTimers[no]);
    if (temp_timer >= (ULONG)svoPrvtVar_timeoutLimits[no])
    {
        svoPrvtVar_timeoutLimits[no] = TIMER_STOP;
        return (eTimerTimeout);
    }
    return (svoPrvtVar_timeoutLimits[no] - temp_timer);
}

/*-----------------------------------------------------------------------------
 *
 * Module: SetTimeout()
 * Description:
 *
 *----------------------------------------------------------------------------*/
__inline void SetTimeout(BYTE no,USHORT data)
{
    StartTimer(&svoPrvtVar_timeoutTimers[no]);
    svoPrvtVar_timeoutLimits[no] = data;
}

/*-----------------------------------------------------------------------------
 *
 * Module: SetDelayTimer()
 * Description: Delay servo SEQ change, focus lost detection is active during delay.
 *              If one need a very accurate and short delay use DelaymS() or Delay100uS() instead.
 *----------------------------------------------------------------------------*/
extern  vULONG      Iram_system_timer;
extern  ULONG       svoPrvtVar_delayTimer;          // delay_timer;
private USHORT      svoPrvtVar_svoTimeDelay;        // svoTimeDelay;
void SetDelayTimer(USHORT data)
{
    StartTimer(&svoPrvtVar_delayTimer);
    svoPrvtVar_svoTimeDelay = data;
    StartTimer(&Pre_Iram_system_timer);
    Iram_system_timer_counter = 0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: delay_timer_check()
 * Description:
 *
 *----------------------------------------------------------------------------*/
SHORT delay_timer_check(void)
{
    if(svoPrvtVar_svoTimeDelay == TIMER_STOP)
    {
        return(Ready);
    }
    if(ReadTimer(&svoPrvtVar_delayTimer) >= (ULONG)svoPrvtVar_svoTimeDelay)
    {
        svoPrvtVar_svoTimeDelay = TIMER_STOP;
        return(Ready);
    }

    if (ReadTimer(&Pre_Iram_system_timer) == 0L)
    {
        Iram_system_timer_counter++;
    }
    else
    {
        Iram_system_timer_counter = 0;
        StartTimer(&Pre_Iram_system_timer);
    }
    if (Iram_system_timer_counter >= 256)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33FF38); //SYStimer stop!
        Iram_system_timer_counter = 0;
        ENABLE_INTERRUPT();
        svoPrvtVar_svoTimeDelay = TIMER_STOP;
        return(Ready);
    }
    return(Busy);
}

/*-----------------------------------------------------------------------------
 *
 * Module: StartFGTimer()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void StartFGTimer(void)
{
    BYTE temp_index;

    if(TIMER2.TCNTRL.BIT.INTEN == 1)
    {
        return;
    }
    for (temp_index = 0; temp_index < DISK_NFGPLS; temp_index++)
    {
        svoSpeedVar.fg_pulse[temp_index] = 0;
    }
    svoSpeedVar.fg_pulse_rev = 0;
    svoSpeedVar.fgTotalTime = 0;
    svoSpeedVar.fgPulseTime = 0xFFFF;
    CLR_TIMER_STATUS(TIMER2);
    SetTimerXValue(TIMER2,0);
    STOP_TIMER(TIMER2);
    START_TIMERS(TIMER2);
    ENABLE_TIMER_INT(TIMER2);  //FG INT Enable
}

/*-----------------------------------------------------------------------------
 *
 * Module: StopFGTimer()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void StopFGTimer(void)
{
    if(TIMER2.TCNTRL.BIT.INTEN == 0)
    {
        return;
    }
    svoSpeedVar.fgCount = 0;
    DISABLE_TIMER_INT(TIMER2);
    STOP_TIMER(TIMER2);
}

/*-----------------------------------------------------------------------------
 *
 * Module: GetSubQCont()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE GetSubQCont(void)
{
    SubQCodeS   svoSubQData;

    pdbGetSubCode(&svoSubQData);
    return(svoSubQData.Con);
}

/*-----------------------------------------------------------------------------------------------------
    Name:       Defect_Enable
    Description:
                Enables/disables the applicable DEFECT function.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void Defect_Enable(BYTE status)
{
    static BYTE defect_status;
    BYTE temp_status;

    if (((svoCalVar.calGainDoneBits & dif_RF_end) == 0)
#if (NEW_BS_DEFECT_FEATURE == 1)
        || (St_drv_ready != TRUE)
#endif
#if (HORROR_DISC_IMPROVEMENT == 1) && (CEProduct == 1)
        || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
#endif
#if (BD_ENABLE == 1) && (ENABLE_BDRLTH == 1)
        || ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && ((svoIramVar.LTHDisclikely == 1) || (svoIramVar.LTHDisc == 1)))
       )
#endif
    {//DEFECT_SOURCE_DFE_RF is not allowed
        status = (status & (~DEFECT_SOURCE_MASK)) | DEFECT_SOURCE_SERVO_BS;
    }

    temp_status = (status & DEFECT_SOURCE_MASK);
    if ((temp_status != DEFECT_SOURCE_NO_CHANGE) && (temp_status != (defect_status & DEFECT_SOURCE_MASK)))
    {
        defect_status = ((defect_status & (~DEFECT_SOURCE_MASK)) | temp_status);
    }
    else
    {
        temp_status = DEFECT_SOURCE_NO_CHANGE;
    }

    switch (defect_status & DEFECT_SOURCE_SPECIAL_MASK)
    {
        case DEFECT_SOURCE_FREEZE_BS:
        case DEFECT_SOURCE_FREEZE_RF:
            temp_status = DEFECT_SOURCE_NO_CHANGE;
            break;
    }

    switch (status & DEFECT_SOURCE_SPECIAL_MASK)
    {
        case DEFECT_SOURCE_FREEZE_BS:
            SendMsg80(DEBUG_CALIB_RESULT_MSG,0x300002); //FREEZE_BS
            defect_status = ((defect_status & (~DEFECT_SOURCE_SPECIAL_MASK)) | DEFECT_SOURCE_FREEZE_BS);
            temp_status = DEFECT_SOURCE_SERVO_BS;
            break;

        case DEFECT_SOURCE_FREEZE_RF:
            SendMsg80(DEBUG_CALIB_RESULT_MSG,0x300003); //FREEZE_RF
            defect_status = ((defect_status & (~DEFECT_SOURCE_SPECIAL_MASK)) | DEFECT_SOURCE_FREEZE_RF);
            temp_status = DEFECT_SOURCE_DFE_RF;
            break;

        case DEFECT_SOURCE_UNFREEZE:
            SendMsg80(DEBUG_CALIB_RESULT_MSG,0x300001); //UNFREEZE
            defect_status &= (~DEFECT_SOURCE_SPECIAL_MASK);
            temp_status = (defect_status & DEFECT_SOURCE_MASK);
            break;
    }

    switch (temp_status)
    {
        case DEFECT_SOURCE_DFE_RF:
            //SendMsg80(SHOW_DEBUG_MSG,0x33FF39); //SOURCE_DfeRF
            ENABLE_EARLY_DEFECT;
#if ((CHIP_REV >= 0xC0) && (NEW_BS_DEFECT_FEATURE == 1))
            WRITE_FIELD(HAL_SRV_DEEPDFCT, TRUE);
            svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, FOCUS_DC_EQ);
            svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, TRACKING_DC_EQ);
#else
            WRITE_FIELD(HAL_SRV_AFEDFCT, TRUE);

            if (defect_status & DEFECT_STATE_ONLY_FOCUS_ON)
            {
                HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefectEnable);
            }
#endif
            break;

        case DEFECT_SOURCE_SERVO_BS:
            //SendMsg80(SHOW_DEBUG_MSG,0x33FF3A); //SOURCE_SvoBS

#if ((CHIP_REV >= 0xC0) && (NEW_BS_DEFECT_FEATURE == 1))
            DISABLE_EARLY_DEFECT;
            WRITE_FIELD(HAL_SRV_DEEPDFCT, FALSE);
            WRITE_DSP_COEF(kf20, 0x7FFF);
            WRITE_DSP_COEF(kf21, 0x7F4B);
            WRITE_DSP_COEF(kf22, 0x00D0);

            WRITE_DSP_COEF(kt20, 0x0500);
            WRITE_DSP_COEF(kt21, 0x7FF8);
            WRITE_DSP_COEF(kt22, 0x0080);

            WRITE_FIELD(HAL_SRV_DFCTEN, TRUE);
            WRITE_FIELD(HAL_SRV_EDCFTEN, TRUE);
#else
            WRITE_FIELD(HAL_SRV_AFEDFCT, FALSE);
    #if (BS_SOURCE_DEFECT_USE_TRUE_HOLD == 1)
            WRITE_FIELD(HAL_SRV_DFCTEN, TRUE);
            WRITE_FIELD(HAL_SRV_EDCFTEN, FALSE);
    #else
            DISABLE_EARLY_DEFECT;
    #endif
#endif
            break;

        case DEFECT_SOURCE_NO_CHANGE:
        default:
            break;
    }

    temp_status = (status & DEFECT_STATE_MASK);
    if ((temp_status != DEFECT_STATE_NO_CHANGE) && (temp_status != (defect_status & DEFECT_STATE_MASK)))
    {
        defect_status = ((defect_status & (~DEFECT_STATE_MASK)) | temp_status);
    }
    else
    {
        temp_status = DEFECT_STATE_NO_CHANGE;
    }

    switch (defect_status & DEFECT_STATE_SPECIAL_MASK)
    {
        case DEFECT_STATE_FREEZE_ALL_OFF:
            temp_status = DEFECT_SOURCE_NO_CHANGE;
            break;
    }

    switch (status & DEFECT_STATE_SPECIAL_MASK)
    {
        case DEFECT_STATE_FREEZE_ALL_OFF:
            defect_status = ((defect_status & (~DEFECT_STATE_SPECIAL_MASK)) | DEFECT_STATE_FREEZE_ALL_OFF);
            temp_status = DEFECT_STATE_ALL_OFF;
            break;

        case DEFECT_STATE_UNFREEZE:
            defect_status &= (~DEFECT_STATE_SPECIAL_MASK);
            temp_status = (defect_status & DEFECT_STATE_MASK);
            break;
    }

    switch(temp_status)
    {
        case DEFECT_STATE_ONLY_FOCUS_ON:
#if ((CHIP_REV < 0xC0) && (NEW_BS_DEFECT_FEATURE == 0))
            if (READ_FIELD(HAL_SRV_AFEDFCT)==FALSE)//BS source defect can't control AFE's TE,FE,CE hold action

            {
                HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefectDisable);
            }
            else
            {
                HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefectEnable);
            }
#endif
            ENABLE_SERVODEFECT;
            WRITE_FIELD(HAL_SRV_FHOLD, TRUE);
            WRITE_FIELD(HAL_SRV_THOLD, FALSE);                     /* Do not use DC filter for tracking error signal when detecting defects */

//#if ((CHIP_REV < 0xC0) && (NEW_BS_DEFECT_FEATURE == 0))
            if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (READ_FIELD(HAL_SRV_DEEPDFCT) == FALSE))
            {
                DISABLE_FE_ENABLE_TE_HOLDDEFECT;
            }
            else
            {
                DISABLE_TE_FE_HOLDDEFECT;
            }
//#endif
            break;

        case DEFECT_STATE_ALL_OFF:
            HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefectDisable);
            DISABLE_SERVODEFECT;
            DISABLE_FE_TE_DCFILTER;
            DISABLE_TE_FE_HOLDDEFECT;
            break;

        case DEFECT_STATE_ALL_ON:
            HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefectEnable);
            ENABLE_SERVODEFECT;
            ENABLE_FE_TE_DCFILTER;
            if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (READ_FIELD(HAL_SRV_DEEPDFCT) == FALSE))
            {
                DISABLE_FE_ENABLE_TE_HOLDDEFECT;
            }
            else
            {
                DISABLE_TE_FE_HOLDDEFECT;
            }
            break;

        case DEFECT_STATE_NO_CHANGE:
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: svotrkon()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    svotrkon(void)
{
#if DO_HW_SUPPORT_STEP == 1
    hw_step_motor_normal_play();
#else /* DO_HW_SUPPORT_STEP */
    svoStp_SetTimerMode( STEP_READ_TIMER );
#endif /* DO_HW_SUPPORT_STEP */

    WRITE_FIELD(HAL_SRV_SEEKCEN, FALSE);  /* Seek completion = Disable */
    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

    SetDfeWblFrontSemiSynchr();
    //TON_ON_OFF(ON);
}

/*-----------------------------------------------------------------------------
 *
 * Module: SledStop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    SledStop(void)
{
    svoStp_StopControl();
    HAL_SERVO_CenterControlOFF();
}

/*-----------------------------------------------------------------------------
 *
 * Module: discstop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    discstop(void)
{
    svoSpeedVar.fgPulseTime = 0xFFFF;
    WRITE_REG(HAL_SRVREG_MAP->DMOFF, 0x00);/* disk spindle motor stop (87H) */
#if (NEW_SPINDLE_CONTROL == 1)
    WRITE_DSP_COEF(DE4_msb, 0); //clear DMO integrator
#endif
    StopFGTimer();
}

/*-----------------------------------------------------------------------------
 *
 * Module: trayoff()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    trayoff(void)
{
    SetMotorDriverState(ALL_MUTE_ON);
    svoTray_OutputDac(0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: alltimer_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    alltimer_stop(void)
{
    SetDelayTimer(TIMER_STOP);      /* Delay Timer Stop */
    SetTimeout(0, TIMER_STOP);      /* time_out timer stop */
    SetTimeout(1, TIMER_STOP);      /* time_out timer stop */
}

/*-----------------------------------------------------------------------------
 *
 * Module: velo_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    velo_stop(void)
{
    svoIramVar.subQEnableFlag = 1;
    StopFGTimer();
}

const svoWordU LGA_Bandpass_Eq[4][5]={
    {0x4000, 0x7404, 0x8000, 0x7404, 0x2DA8},    // 1.38 kHz
    {0x4000, 0x693A, 0x8000, 0x693A, 0x52BA},    // 2.75 kHz
    {0x4000, 0x6F01, 0x8000, 0x6F01, 0x3F5F},    // 2.00 kHz
    {0x4000, 0x6043, 0x8000, 0x6043, 0x6E77},    // 4.00 kHz
};

/*-----------------------------------------------------------------------------
 *
 * Module: fgdsp_prepare()
 * Description: Determine the Focus Phase and Frequency settings for a
 *              specific disc type.
 * Input: none
 * Return: none
 *
 *----------------------------------------------------------------------------*/
void fgdsp_prepare(void)
{
    BYTE   freq;   //Focus SinWave Feq
    USHORT amp;    //Focus SinWave Amp
    BYTE   phase;  //Focus phase * 5.6 degress

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
            freq = FGA_FREQ_DVD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {   // DVD single layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_DVDROM;
                        phase = FGA_PHASE_DVDROM;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_DVDR;
                        phase = FGA_PHASE_DVDR;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_DVDRW;
                        phase = FGA_PHASE_DVDRW;
                        break;

#if (DVD_RAM_READ == 1)
                    case eMediaRAM:
                        freq  = FGA_FREQ_DVDRAM;
                        amp   = FGA_AMP_DVDRAM;
                        phase = FGA_PHASE_DVDRAM;
                        break;
#endif
                }
            }
            else
            {   // DVD dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_DVDROMDL;
                        phase = FGA_PHASE_DVDROMDL;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_DVDRDL;
                        phase = FGA_PHASE_DVDRDL;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_DVDRWDL;
                        phase = FGA_PHASE_DVDRWDL;
                        break;
                }
            }
            break;

        case eDiscCD:
            freq = FGA_FREQ_CD;
            amp  = FGA_AMP_CD;
            switch (svoVar_discKindT.Bit.Media)
            {
                case eMediaROM:
                    phase = FGA_PHASE_CDROM;
                    break;

                case eMediaR:
                    phase = FGA_PHASE_CDR;
                    break;

                case eMediaRW:
                    phase = FGA_PHASE_CDRW;
                    break;
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            freq = FGA_FREQ_BD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_BDROM;
                        phase = FGA_PHASE_BDROM;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_BDR;
                        phase = FGA_PHASE_BDR;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_BDRE;
                        phase = FGA_PHASE_BDRE;
                        break;
                }
            }
            else
            {
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_BDROMDL;
                        phase = FGA_PHASE_BDROMDL;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_BDRDL;
                        phase = FGA_PHASE_BDRDL;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_BDREDL;
                        phase = FGA_PHASE_BDREDL;
                        break;
                }
            }
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            freq = FGA_FREQ_HD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_HDROM;
                        phase = FGA_PHASE_HDROM;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_HDR;
                        phase = FGA_PHASE_HDR;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_HDRW;
                        phase = FGA_PHASE_HDRW;
                        break;
                }
            }
            else
            {   // HD Dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = FGA_AMP_HDROMDL;
                        phase = FGA_PHASE_HDROMDL;
                        break;

                    case eMediaR:
                        amp   = FGA_AMP_HDRDL;
                        phase = FGA_PHASE_HDRDL;
                        break;

                    case eMediaRW:
                        amp   = FGA_AMP_HDRWDL;
                        phase = FGA_PHASE_HDRWDL;
                        break;
                }
            }
            break;
#endif
    }

    /*** Update Focus Frequency, BPF and Phase ***/
    WRITE_FIELD(HAL_SRV_AGC,freq);
    WRITE_DSP_COEF(kf10,  amp);
    WRITE_DSP_COEF(klg10, LGA_Bandpass_Eq[freq][0].all16);
    WRITE_DSP_COEF(klg11, LGA_Bandpass_Eq[freq][1].all16);
    WRITE_DSP_COEF(klg12, LGA_Bandpass_Eq[freq][2].all16);
    WRITE_DSP_COEF(klg13, LGA_Bandpass_Eq[freq][3].all16);
    WRITE_DSP_COEF(klg14, LGA_Bandpass_Eq[freq][4].all16);

    WRITE_REG(HAL_SRVREG_MAP->FGASET,phase);
    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,1,0x317902+freq,B1B(READ_FIELD(HAL_SRV_FPHD)));
}

/*-----------------------------------------------------------------------------
 *
 * Module: tgdsp_prepare()
 * Description: Determine the tracking Phase and Frequency settings for a
 *              specific disc type.
 * Input: none
 * Return: none
 *----------------------------------------------------------------------------*/
void tgdsp_prepare(void)
{
    BYTE   freq;   //Tracking SinWave Feq
    USHORT amp;    //Focus SinWave Amp
    BYTE   phase;  //Tracking phase * 5.6 degress

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
            freq = TGA_FREQ_DVD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {   // DVD dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_DVDROM;
                        phase = TGA_PHASE_DVDROM;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_DVDR;
                        phase = TGA_PHASE_DVDR;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_DVDRW;
                        phase = TGA_PHASE_DVDRW;
                        break;

#if (DVD_RAM_READ == 1)
                    case eMediaRAM:
                        freq = TGA_FREQ_DVDRAM;
                        amp   = TGA_AMP_DVDRAM;
                        phase = TGA_PHASE_DVDRAM;
                        break;
#endif
                }
            }
            else
            {   // DVD dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_DVDROMDL;
                        phase = TGA_PHASE_DVDROMDL;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_DVDRDL;
                        phase = TGA_PHASE_DVDRDL;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_DVDRWDL;
                        phase = TGA_PHASE_DVDRWDL;
                        break;
                }
            }
            break;

        case eDiscCD:
            freq = TGA_FREQ_CD;
            amp  = TGA_AMP_CD;
            switch (svoVar_discKindT.Bit.Media)
            {
                case eMediaROM:
                    phase = TGA_PHASE_CDROM;
                    break;

                case eMediaR:
                    phase = TGA_PHASE_CDR;
                    break;

                case eMediaRW:
                    phase = TGA_PHASE_CDRW;
                    break;
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            freq = TGA_FREQ_BD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_BDROM;
                        phase = TGA_PHASE_BDROM;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_BDR;
                        phase = TGA_PHASE_BDR;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_BDRE;
                        phase = TGA_PHASE_BDRE;
                        break;
                }
            }
            else
            {   //  BD Dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_BDROMDL;
                        phase = TGA_PHASE_BDROMDL;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_BDRDL;
                        phase = TGA_PHASE_BDRDL;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_BDREDL;
                        phase = TGA_PHASE_BDREDL;
                        break;
                }
            }
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            freq = TGA_FREQ_HD;
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_HDROM;
                        phase = TGA_PHASE_HDROM;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_HDR;
                        phase = TGA_PHASE_HDR;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_HDRW;
                        phase = TGA_PHASE_HDRW;
                        break;
                }
            }
            else
            {   //  HD Dual layer
                switch (svoVar_discKindT.Bit.Media)
                {
                    case eMediaROM:
                        amp   = TGA_AMP_HDROMDL;
                        phase = TGA_PHASE_HDROMDL;
                        break;

                    case eMediaR:
                        amp   = TGA_AMP_HDRDL;
                        phase = TGA_PHASE_HDRDL;
                        break;

                    case eMediaRW:
                        amp   = TGA_AMP_HDRWDL;
                        phase = TGA_PHASE_HDRWDL;
                        break;
                }
            }
            break;
#endif
    }

    /*** Update Tracking Phase, BPF and Frequency ***/
    WRITE_FIELD(HAL_SRV_AGC,freq);
    WRITE_DSP_COEF(kt10,  amp);
    WRITE_DSP_COEF(klg10, LGA_Bandpass_Eq[freq][0].all16);
    WRITE_DSP_COEF(klg11, LGA_Bandpass_Eq[freq][1].all16);
    WRITE_DSP_COEF(klg12, LGA_Bandpass_Eq[freq][2].all16);
    WRITE_DSP_COEF(klg13, LGA_Bandpass_Eq[freq][3].all16);
    WRITE_DSP_COEF(klg14, LGA_Bandpass_Eq[freq][4].all16);

    WRITE_REG(HAL_SRVREG_MAP->TGASET,phase);

    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,1,0x317A02+freq,B1B(READ_FIELD(HAL_SRV_TPHD)));
}

/*-----------------------------------------------------------------------------
 *
 * Module: func_nop()
 * Description: This fucntion is intened to be empty.
 *
 *----------------------------------------------------------------------------*/
void func_nop(void)
{
}

/*-----------------------------------------------------------------------------
 *
 * Module: nodisc_set()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    nodisc_set(void)
{
    svoVar_discDiameter = eNoDisc;
    svoVar_discKindT.fDiscKind = eUnknownKind;
}

/*-----------------------------------------------------------------------------
 *
 * Module: get_zone_speed() //This function should be used only for ZCLV mode
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE    get_zone_speed(void)
{
    ESpindleSpeed zone_speed;

    zone_speed = svoVar_svoTargetSpeed;
    svoMiscVar.zoneAreaCheckFlag = FALSE;

    return(zone_speed);
}


/*-----------------------------------------------------------------------------
 *
 * Module: SetMotorDriverState()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    SetMotorDriverState(BYTE state)
{
    if(svo_Var_motorDriverState == state){
        return;
    }

    switch (state)
    {
        case ALL_MUTE_ON:
            DRIVER_ALL_MUTE;
#if (EN_ADI == 1)
            HAL_DSP_DisDissInt();
#endif
            break;

        case LODER_MUTE_OFF:

#if(MOTOR_DRIVER == R2S30208)
            DRIVER_ALL_MUTE;
            DelaymS(2);
#endif
            DRIVER_LOADER_ON;
#if DO_HW_SUPPORT_STEP == 1
            //----------------------------------------------------;
            // make DAC1, DAC controled by F/W, not by stepper HW ;
            //----------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->GPDACMUX,0x20); //TBD:SE1 hardcode
#endif // DO_HW_SUPPORT_STEP == 1
#if (EN_ADI == 1)
            HAL_DSP_DisDissInt();
#endif
            break;

        case SHORT_BRAKE:
            DRIVER_SHORT_BRAKE;
#if DO_HW_SUPPORT_STEP == 1
            //----------------------------------------------------;
            // make DAC1, DAC controled by stepper HW again       ;
            //----------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->GPDACMUX,0x14); //TBD:SE1 hardcode
#endif // DO_HW_SUPPORT_STEP == 1
#if (EN_ADI == 1)
            HAL_DSP_SetActFocDissInt();
            HAL_DSP_SetActTrkDissInt();
#endif
            break;

        case REVERSE_BRAKE:
            DRIVER_REVERSE_BRAKE;
#if DO_HW_SUPPORT_STEP == 1
            //----------------------------------------------------;
            // make DAC1, DAC controled by stepper HW again       ;
            //----------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->GPDACMUX,0x14); //TBD? Hard Coded Value! SE1 hardcode
#endif // DO_HW_SUPPORT_STEP == 1
#if (EN_ADI == 1)
            HAL_DSP_SetActFocDissInt();
            HAL_DSP_SetActTrkDissInt();
#endif
            break;

        case RESET_PROTECT:
#if (MOTOR_DRIVER == R2S30204) || (MOTOR_DRIVER == R2A30232)||(MOTOR_DRIVER == R2S30208)||(MOTOR_DRIVER == R2A30222)||(MOTOR_DRIVER == AM9268)||(MOTOR_DRIVER == TPIC1407)
            DRIVER_ALL_MUTE;
            DelaymS(3);
            DRIVER_SHORT_BRAKE;
            DelaymS(3);
            DRIVER_ALL_MUTE;
#endif
#if (EN_ADI == 1)
            HAL_DSP_DisDissInt();
#endif
            break;
    }
    svo_Var_motorDriverState = state;
}

/*-----------------------------------------------------------------------------
 *
 * Module: get_driver_state()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE    get_driver_state(void) // TBD? __inline
{
    return(svo_Var_motorDriverState);
}

/*-----------------------------------------------------------------------------
 *
 * Module: cancel_flag_check(void)
 * Description:[W0K]Walker
 *
 *----------------------------------------------------------------------------*/
BOOL  cancel_flag_check(void)
{
    if(svoIramVar.knlTimeupCountFlag == FALSE){
		
        if(svoIramVar.emergencyStopFlag == TRUE){
			
            return(NG);
        }
        if(svoIramVar.knlTimeupCount == 0){
            return(NG);
        }
    }	
    return(OK);
}

/********************************************************************/
/*                                                                  */
/* focus_shock_detection should be used to set up focus shock       */
/* detection algorithm                                              */
/*                                                                  */
/********************************************************************/
BOOL focus_shock_detection(eFocusShockInterrupt_t   en_irq,
                           eFocusShockAverage4Smp_t ave_4_smp,
                           eFocusShockWindow_t      reset_counter,
                           eFocusShockWidth_t       width,
                           BYTE            threshold,
                           BYTE            cnt_threshold)
{
    WRITE_FIELD(HAL_SRV_FSTHRES, threshold);      // assign the threshold
    WRITE_FIELD(HAL_SRV_CTTHRES, cnt_threshold);  // assign the number of occurs of over threshold
    /*---------------------------------------------------------------*/
    switch(reset_counter)
    {
        case FOCUS_SHOCK_WINDOW_RESET_PEAK:
            WRITE_FIELD(HAL_SRV_WINDOW, 1);
            break;

        case FOCUS_SHOCK_WINDOW_HOLD_PEAK:
            WRITE_FIELD(HAL_SRV_WINDOW, 0);
            break;

        default:
            err_log(ERR_FOCUS_SHOCK_WIN_INPUT);
            return(FALSE);
            break;
    } // end switch(reset_counter)

    /*---------------------------------------------------------------*/
    switch(width)
    {
        case FOCUS_SHOCK_WIDTH_1400US:
            WRITE_FIELD(HAL_SRV_WIDTH, halSRV_WIDTH_1P4ms);
            break;

        case FOCUS_SHOCK_WIDTH_700US:
            WRITE_FIELD(HAL_SRV_WIDTH, halSRV_WIDTH_P7ms);
            break;

        default:
            err_log(ERR_FOCUS_SHOCK_WIDTH_INPUT);
            return(FALSE);
            break;
    } // end switch(width)

    /*---------------------------------------------------------------*/
    switch(ave_4_smp)
    {
        case FOCUS_SHOCK_AVE_ENABLE:

            WRITE_FIELD(HAL_SRV_FEAVE, 1);
            break;

        case FOCUS_SHOCK_AVE_DISABLE:
            WRITE_FIELD(HAL_SRV_FEAVE, 0);
            break;

        default:
            err_log(ERR_FOCUS_SHOCK_AVE4_INPUT);
            return(FALSE);
    } // end switch(ave_4_smp)

    /*---------------------------------------------------------------*/
    /* should work on the enable/disable interrupt after all above   */
    /* setting                                                       */
    /*---------------------------------------------------------------*/
    switch(en_irq){
        case FOCUS_SHOCK_ENABLE_INTERRUPT:
            WRITE_FIELD(HAL_SRV_INTRACT, 1);  // enable focus shock
            WRITE_FIELD(HAL_SRV_FSHCEN, 1); // enable focus error disturbance interrupt
            break;

        case FOCUS_SHOCK_DISABLE_CLEAR_INTERRUPT:
            WRITE_FIELD(HAL_SRV_INTRACT, 0);  // disable focus shock
            WRITE_FIELD(HAL_SRV_FSHCEN, 0);   // disable focus error disturbance interrupt
            break;

        default:
            err_log(ERR_FOCUS_SHOCK_IRQ_INPUT);
            return(FALSE);
    } // end switch(en_irq)

    return(TRUE);
}// end of focus_shock_detection

/********************************************************************/
/*                                                                  */
/* focus_shock_detection should be used to set up focus shock       */
/* detection algorithm                                              */
/*                                                                  */
/********************************************************************/
void init_error_log_buffer(void)
{
    BYTE j;

    /*-----------------------------------------------------*/
    /* initial error log buffer                            */
    /*-----------------------------------------------------*/
    for(j = 0; j < SIZE_ERR_LOG; j++){
        tbl_error_log[j] = 0;
    }
    tbl_error_log_idx = SIZE_ERR_LOG - 1;
}//end of init_error_log_buffer

/********************************************************************/
/*                                                                  */
/* log the error code input error code buffer, a queue or a stack   */
/*                                                                  */
/********************************************************************/
void err_log(eLogErrorCode_t ec)
{
    if(tbl_error_log_idx != 0){
        tbl_error_log[tbl_error_log_idx--] = ec;
    }else{
        /* error buffer is almost full. no error logging process */
    }
}/* end of err_log */

#if DO_HW_SUPPORT_STEP == 1
#define MAX_STP_LOC_READ 10
/*********************************************************
* Module: read_stploc()
*
* Description: Read the accumulation step position
*
* Returns: signed_int_read_t
* Params:  void
**********************************************************/
signed_int_read_t read_stploc(void)
{
    USHORT    ui;
    svoWordU  prev_read, cur_read;
    signed_int_read_t res;

    //----------------------------------------------;
    // do the 1st location read                     ;
    //----------------------------------------------;
    USHORT temp;

#if (CHIP_REV <= 0xB3)
    temp = READ_REG(HAL_SRVREG_MAP->STPLOC);
#else
    temp = READ_FIELD(HAL_SRV_STPLOC);
#endif
    prev_read.bytes.Lbyte = (BYTE)(temp & 0x00FF);
    prev_read.bytes.Hbyte = (BYTE)((temp & 0x0FF00) >> 8);
    for(ui = 0; ui < MAX_STP_LOC_READ; ui++){
        //----------------------------------------------;
        // do the 2nd location read                     ;
        //----------------------------------------------;
#if (CHIP_REV <= 0xB3)
        temp = READ_REG(HAL_SRVREG_MAP->STPLOC);
#else
        temp = READ_FIELD(HAL_SRV_STPLOC);
#endif
        cur_read.bytes.Lbyte = (BYTE)(temp & 0x00FF);
        cur_read.bytes.Hbyte = (BYTE)((temp & 0xFF00) >> 8);

        //----------------------------------------------;
        // check if they are the same                   ;
        //----------------------------------------------;
        if(prev_read.all16s == cur_read.all16s)
        {
            res.succ = TRUE;
            res.val  = cur_read.all16s;
            return(res);
        }

        prev_read.all16s = cur_read.all16s;
    }

    res.succ = FALSE;
    res.val  = 0;
    return(res);
}// end of read_stploc
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: StartDiscStopWithTrayTimer()
 * Description:
 *
 *----------------------------------------------------------------------------*/
private BYTE svoPrvtVar_DiscStopWithTray;
private ULONG svoPrvtVar_DiscStopWithTrayTimer;
void StartDiscStopWithTrayTimer(void)
{
    StartTimer(&svoPrvtVar_DiscStopWithTrayTimer);
    svoPrvtVar_DiscStopWithTray = TRUE;
}

/*-----------------------------------------------------------------------------
 *
 * Module: CheckDiscStopWithTrayTimer()
 * Description:
 *
 *----------------------------------------------------------------------------*/
SHORT CheckDiscStopWithTrayTimer(USHORT timer)
{
    if(svoPrvtVar_DiscStopWithTray == FALSE){
        return(Ready);
    }
    if(ReadTimer(&svoPrvtVar_DiscStopWithTrayTimer) >= (ULONG)timer){
        svoPrvtVar_DiscStopWithTray = FALSE;
        return(Ready);
    }
    return(Busy);
}

/*********************************************************
* Module: wait_track_on()
*
* Description: wait slow flag and enable tracking servo
*
* Returns: void
* Params:  USHORT
**********************************************************/
void wait_track_on(USHORT timeout)
{
    ULONG  temp_timer;
    USHORT pre_track,cur_track,i,cnt_txspd_cg;
    BYTE   hyst_en,track_cnt_check;
    USHORT max_TE,min_TE,CE_values,temp_TXSPEED;
    SHORT TroDac;

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
#if (DPD_COMPENSATE_OFFSET==1)
        if (calGetCalRequired(Layer_Index,Iram_stp_CurrentPosition,DPD_GNOS,READ_MODE))
        {
            WaitTimer(5);
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
            HAL_SERVO_CenterControlON(Iram_stp_CurrentDirection);
        }
#endif//#if (DPD_COMPENSATE_OFFSET==1)
#if (CHIP_REV >= 0xC1)
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW3_400kHz);
#else
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW_400kHz);
#endif
        WRITE_FIELD(HAL_AFE_RXTOK, 1); //Disable XTOK mute RX
    }

    //Disable TX deglitch to prevent TEZC to be filted and cause wrong TXSPEED measurment
    if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (!DVDRAM))
    {
        WRITE_FIELD(HAL_SRV_TXRXFILT,0);//from 720
    }

#if (FJ_TRACKING_ON == 1)
    if (BDRDLDisc || BDREDLDisc)
    {
        fine_jump_tracking_on(FORW); //jump out
        return;
    }
#endif

#if (BD_ECC_IMPROVEMENT==1)

    if ((svoVar_eccDiscNum >= eECC_140_um) && (svoVar_discKindT.Bit.DiscStd == eDiscBD))
    {
        HAL_SERVO_CenterControlON(Iram_stp_CurrentDirection);
//        WRITE_FIELD(HAL_SRV_FVD,halSRV_TrackXFreq_12p6KHZ);
    }
#endif
#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        //
        // Disable header hold function because HEADERHLD is not correct.
        // And disabling TE-header-hold function can prevent SLOW flag from being not reliable for DVD-RAM.
        //
        DISABLE_HDR_HOLD_SERVO;                     // Disable header hold function for servo signals.
    }
#endif

    if ((ABS(ReadAverageAdcmV(ADC_CE, 5) - CONVERT_DSPOFS_TO_ADCMV(svoCalVar.ceOfsData.all16)) > TRACKING_ON_CE_WINDOW_MV))
    {//from720
        seekSetCEEq(eRoughSeek, 128);
        HAL_SERVO_CenterControlON(FORW);
        WaitTimer(8L);
        seekSetCEEq(eRoughSeekRatioOnly, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
    }
    seekSetCEEq(eDamper, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD: CE_GAIN_DOWN_RATIO_DVD);

    HAL_SERVO_CenterControlON(FORW);//from 720
    DelaymS(1);

    StartTimer(&temp_timer);
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
    Set_TOK_Mode(TOK_SEEK);
#else // WIDER_TOK_LVL_FOR_PLAY_MODE == 1
    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_300mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, TOKLVL_TRACK_OFF);
    #endif
#endif // WIDER_TOK_LVL_FOR_PLAY_MODE == 1
    WRITE_FIELD(HAL_AFE_RCINV,1);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);
    if ((svoSeekVar.rx_available == FALSE) && (svoMiscVar.trackSlipCount <= 1)
#if (DVD_RAM_READ == 1)
        && (!DVDRAM_rewritable)
#endif
       )
    {
        hyst_en = TRUE;
    }
    else
    {
        hyst_en = FALSE;
    }

    if (svoSeekVar.rx_available || hyst_en)
    {
        WRITE_REG(HAL_SRVREG_MAP->FVKICM, (HAL_SRV_FVKHYSC | HAL_SRV_FVKTGC));    /* set hysteresis and tracking gain up */
    }
    else
    {
        WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKTGC);    /* set tracking gain up */
    }
#if (DVD_RAM_READ == 1)
    if (!DVDRAM_rewritable)
#endif
        track_cnt_check = TRUE;

    if (svoMiscVar.trackSlipCount >= 10)//from 720
    {
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TRO);
        TroDac = READ_REG(HAL_SRVREG_MAP->SRCR) - 512;
        CE_values = ReadAverageAdcmV(ADC_CE, 5);

        HAL_SERVO_CenterControlOFF();
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_FIELD(HAL_SRV_PKBOT,1);

        // The delay time is the period of Peak/Bottom measurement.
        Delay100uS(1);  // 100us. The period of Peak/Bottom measurement.

        WRITE_FIELD(HAL_SRV_PKBOT,0);
        HAL_SERVO_CenterControlON(FORW);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEPEAK);
        max_TE = READ_REG(HAL_SRVREG_MAP->SRCR);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEBOTTOM);
        min_TE = READ_REG(HAL_SRVREG_MAP->SRCR);
        max_TE = ((max_TE * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;
        min_TE = ((min_TE * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;

        temp_TXSPEED = READ_FIELD(HAL_SRV_TXSPEED);
        if (temp_TXSPEED > 0)
        {
            temp_TXSPEED = (352800 / temp_TXSPEED);
        }
        else
        {
            temp_TXSPEED = 0xFFFF;
        }
        SendMsgCnB(SHOW_DEBUG_MSG, 10,0x3166C5,B2B(max_TE),B2B(min_TE),B2B(CE_values),B2B(TroDac),B2B(temp_TXSPEED));
    }
    while (1)
    {
        if ((svoVar_TrackingMethod != TRK_METHOD_DPD) && hyst_en && ((HAL_GLOBAL_GetRfPresent() == FALSE) || (READ_FIELD(HAL_DFE_GDDATGMON) == 31)))
        {
            track_cnt_check = FALSE;
            hyst_en = FALSE;
            WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKTGC);    /* set tracking gain up */
        }
        cnt_txspd_cg = 3;
        pre_track = READ_REG(HAL_SRVREG_MAP->SRCR);
        for (i = 0; i < 2048; i++)
        {
            cur_track = READ_REG(HAL_SRVREG_MAP->SRCR);
            if (!HAL_GLOBAL_GetRfPresent() && (svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                cnt_txspd_cg = 0;
            }

            if ((cur_track != pre_track) || ((!track_cnt_check) && (READ_REG(HAL_SRVREG_MAP->TXSPEED) & HAL_SRV_TXSPDUP)))
            {
                if((cnt_txspd_cg++ > 2) && READ_FIELD(HAL_SRV_SLOW)) 
                {
                    break;
                }
                pre_track = cur_track;
            }
        }
        if (i < 2048)                               // break while 1 loop
        {
             break;
        }

        //if (ReadTimer(&temp_timer) >= timeout / 2)
        //from 720
        if (track_cnt_check && (ReadTimer(&temp_timer) * 5 >= timeout)) //if over 10 ms, don't check the track counter.
        {
            //maybe the RP is saturated.
            track_cnt_check = FALSE;
            if (hyst_en)
            {
                hyst_en = FALSE;
                WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKTGC);    /* set tracking gain up */
            }
        }
            if (ReadTimer(&temp_timer) >= timeout)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x334004); //WARNING: SlowChk Timeout
                break;
            }
        }
    WRITE_FIELD(HAL_AFE_RCINV,0);
    HAL_SERVO_CenterControlOFF();
    WRITE_REG(HAL_SRVREG_MAP->FVKICK, 0x00);          /* set tracking servo ON only */
    WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);

    if (READ_FIELD(HAL_SRV_FVKHYSC) == TRUE)
    {
        WRITE_FIELD(HAL_AFE_RXTOK, 0); //Enable XTOK mute RX to prevent wrong hysteresis action which is caused by wrong RX.
    }

#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        WblPllFreqPreset = 0;
        LAL_WOBBLE_SetHeaderDetectMode(SlicedWobble);

        //
        // Disable header hold function during rough seek because HEADERHLD is not correct.
        // And disabling TE-header-hold function can prevent SLOW flag from being not reliable for DVDRAM.
        // Before turn on tracking servo, enable all header hold function for normal tracking.
        //
        ENABLE_HDR_HOLD_SERVO_NORMAL;      //Enable header hold function for servo signals.
        WRITE_FIELD(HAL_SRV_DVDRHHLD, 1);  //Enable header hold function for focus and tracking.
    }
#endif
    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_450mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, TOKLVL_WAIT_TRACK_ON);
    #endif
#if (FDP_TRKSLIP_MONITOR == 1)
    svoMiscVar.forceTrackOffFlag = FALSE;
#endif
}

#if (FJ_TRACKING_ON == 1)
/*********************************************************
* Module: fine_jump_tracking_on()
*
* Description: use fine jump tracking on after rough jump
*
* Returns: none
* Params:  none
**********************************************************/
void fine_jump_tracking_on(BYTE Direction)
{
    ULONG  TimerCnt;
    SHORT  tkic, TmpSpd, SpdVel;
    USHORT OriTESRCDB;
    LONG   SpdGain;
    SBYTE  SpdIndex,i;
    #if (CHIP_REV < 0xC0)
    BYTE   ReverseKickCheck;
    #endif
#if (WORKAROUND_FOR_BUG_5637 == 1)
    SBYTE OriginalTRKO;
    SBYTE OriginalTE2OSA;
#endif

    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);  // Clear Seek completion interrupt
    WRITE_FIELD(HAL_SRV_SEEKCEN, 0);    // Disable Seek completion interrupt

    // fake RX jump
#if (TRO_INVERSION == 1)
    if(Direction == BACK)
#else
    if(Direction == FORW)
#endif
    {
    #if (CHIP_REV <= 0xB3)
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_FAKETX_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_FAKERX|HAL_SRV_DIRPOL);
    #else
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_TXRXFILT_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_TXRXFILT|HAL_SRV_FAKERX|HAL_SRV_DIRPOL);
    #endif
    }
    else
    {
    #if (CHIP_REV <= 0xB3)
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_FAKETX_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_FAKERX);
    #else
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_TXRXFILT_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_TXRXFILT|HAL_SRV_FAKERX);
    #endif
    }

    WRITE_FIELD(HAL_SRV_FAKERX,1);
    WRITE_FIELD(HAL_SRV_FORBLK,1);
    WRITE_FIELD(HAL_SRV_RXMUTE,1);
    WRITE_FIELD(HAL_SRV_HYSL, halSRV_Hysteresis_2to3ms);
    READ_MOD_WRITE(HAL_SRVREG_MAP->SREG2,(HAL_SRV_GUP1|HAL_SRV_HYS1|HAL_SRV_HYS2|HAL_SRV_GUP2),HAL_SRV_GUP2);

    // FJ parameter setting
    seekSetFineJumpParas(FJTrackNumber);

    // speed profile setting
    for (SpdIndex=0;SpdIndex<9;SpdIndex++)
    {
        if ( FJTrackNumber < (1 << (SpdIndex+2)) )
            break;
    }

    TmpSpd = READ_DSP_COEF(kspd_0 + (4*SpdIndex));

    WRITE_DSP_COEF(kspd_start, TmpSpd);

    SpdGain = (TmpSpd&0xFF00) >> 8;
    SpdGain = (0x400/SpdGain) + 1;
    WRITE_REG(HAL_SRVREG_MAP->KICKLVL, SpdGain);

    for (i=SpdIndex;i>1;i--)
    {
        TmpSpd = READ_DSP_COEF(kspd_0 + (4*i));
        SpdGain = (TmpSpd&0xFF00) >> 8;
        SpdVel  = (TmpSpd&0x00FF);
        SpdGain = (SpdGain*SpdGainUpRatio) >> 8;
        if (SpdGain > 0x7F)
            SpdGain = 0x7F;
        TmpSpd  = (SpdGain<<8) | SpdVel;
        WRITE_DSP_COEF(kspd_0 + (4*i), TmpSpd);
    }

    //wait slow
    WRITE_FIELD(HAL_SRV_BFILT, halSRV_BFILT_RejectFilter_88KHz);
    StartTimer(&TimerCnt);
    //while(READ_FIELD(HAL_SRV_TXSPEED) < TXSpeedSlowThreshold)
    while(READ_FIELD(HAL_SRV_SLOW) == 0)
    {
        if ((ReadTimer(&TimerCnt) >= 10))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x334004);//WARNING: SlowChk Timeout
            break;
        }
    }
    HAL_SERVO_CenterControlOFF();

    // inhibit window setting
    OriTESRCDB = READ_FIELD(HAL_SRV_TESRCDB);
    WRITE_FIELD(HAL_SRV_INHIEN,1);
    WRITE_FIELD(HAL_SRV_BFILT, halSRV_BFILT_RejectFilter_44KHz);
    WRITE_FIELD(HAL_SRV_TESRCDB,FJTESRCDB);
    WRITE_FIELD(HAL_SRV_INHITX, InhibitTX); //200us/1.41us =141=0x8D

#if (WORKAROUND_FOR_BUG_5637 == 1)
    OriginalTRKO   = calDacCount2SBYTE(READ_FIELD(HAL_AFE_TRKO), 8);
    OriginalTE2OSA = calDacCount2SBYTE(READ_FIELD(HAL_AFE_TE2OSA), 8);
    WRITE_FIELD(HAL_AFE_BLANKSEL,FALSE);
#if (TRO_INVERSION == 1)
    if(Direction == BACK)
#else
    if (Direction == FORW)
#endif
    {
        WRITE_FIELD(HAL_AFE_TRKO,  calSBYTE2DacCount(-TRKOShift ,8));
        WRITE_FIELD(HAL_AFE_TE2OSA,calSBYTE2DacCount(-TRKOShift ,8));
    }
    else
    {
        WRITE_FIELD(HAL_AFE_TRKO,  calSBYTE2DacCount(TRKOShift ,8));
        WRITE_FIELD(HAL_AFE_TE2OSA,calSBYTE2DacCount(TRKOShift ,8));
    }
#endif

    #if (CHIP_REV < 0xC0)
    //kick pulse reverse detection
    HAL_DSP_SetDebug(DEBUG_CH0, DEBUG_SOURCE_RGLUE, 0x1C, 8);//TKIC
    //start fine jump
    ReverseKickCheck = TRUE;
    #endif

    svoIramVar.fineSeekDoneFlag = FALSE;
    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);      /* Clear Seek completion interrupt */
    WRITE_FIELD(HAL_SRV_SEEKCEN, 1);        /* Enable Seek completion interrupt */

#if (TRO_INVERSION == 1)
    if(Direction == BACK)
#else
    if(Direction == FORW)
#endif
    {
        WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKSURF|HAL_SRV_FVKBF);
    }
    else
    {
        WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKSURF);
    }

    WRITE_REG(HAL_SRVREG_MAP->FVKICK, FJTrackNumber);
    StartTimer(&TimerCnt);
    while(svoIramVar.fineSeekDoneFlag == FALSE)
    {
#if (CHIP_REV < 0xC0)
        if (ReverseKickCheck == TRUE)
        {
            tkic = (SHORT)READ_DSP_COEF(debug0);
            if (tkic != 0)
            {
               #if (TRO_INVERSION == 1)
                if ( ((Direction == FORW) && (tkic > 0)) ||
                     ((Direction == BACK) && (tkic < 0)) )
               #else
                if ( ((Direction == FORW) && (tkic < 0)) ||
                     ((Direction == BACK) && (tkic > 0)) )
               #endif
                {
                    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);  // Clear Seek completion interrupt
                    WRITE_FIELD(HAL_SRV_SEEKCEN, 0);    // Disable Seek completion interrupt
                    WRITE_FIELD(HAL_SRV_FAKERX, 0);
                    WRITE_FIELD(HAL_SRV_FORBLK, 0);
                    WRITE_FIELD(HAL_SRV_RXMUTE, 0);
                    WRITE_REG(HAL_SRVREG_MAP->FVKICM, 0);
                    WRITE_REG(HAL_SRVREG_MAP->FVKICK, 0);
                    WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);
    #if (WORKAROUND_FOR_BUG_5637 == 1)
                    WRITE_FIELD(HAL_AFE_BLANKSEL,TRUE);
                    WRITE_FIELD(HAL_AFE_TRKO,  calSBYTE2DacCount(OriginalTRKO,8));
                    WRITE_FIELD(HAL_AFE_TE2OSA,calSBYTE2DacCount(OriginalTE2OSA,8));
    #endif
                    SendMsg80(SHOW_DEBUG_MSG,0x334105); // WARNING: FineSeek wrong kick
                    break;
                }
                ReverseKickCheck = FALSE;
    #if (WORKAROUND_FOR_BUG_5637 == 1)
                WRITE_FIELD(HAL_AFE_BLANKSEL,TRUE);
                WRITE_FIELD(HAL_AFE_TRKO,  calSBYTE2DacCount(OriginalTRKO,8));
                WRITE_FIELD(HAL_AFE_TE2OSA,calSBYTE2DacCount(OriginalTE2OSA,8));
    #endif
            }
        }
#endif
        if ((ReadTimer(&TimerCnt) >= 10))
        {
            //seek time out
            WRITE_REG(HAL_SRVREG_MAP->FVKICM, 0);
            WRITE_REG(HAL_SRVREG_MAP->FVKICK, 0);
            WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);
            break;
        }
    }

    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);  // Clear Seek completion interrupt
    WRITE_FIELD(HAL_SRV_SEEKCEN, 0);    // Disable Seek completion interrupt

    //recovery the origional speed profile
    svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, SPEED_EQ);
    //recovery TESRCDB
    WRITE_FIELD(HAL_SRV_TESRCDB,OriTESRCDB);

    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_450mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_450mV);
    #endif
}
#endif  // #if (FJ_TRACKING_ON == 1)

/*********************************************************
* Module: track_slip_protection()
*
* Description: set track servo off to stop track slip and protect the coil,
*              if the OPU is close to disc edge, jump to the safe area.
*              sw = on: set track servo on before leaving
*              sw = off: track servo maintain off
* Returns: void
* Params:  BYTE
**********************************************************/
#define CE_WINDOW_MV  (CE_WINDOW_UM * CE_SEN_TARGET_MV_MM / 1000)
void track_slip_protection(BYTE slow_detection)
{
    SHORT jump_distance;
#if (BD_ECC_IMPROVEMENT==1)
    USHORT TargetRpm,next_rpm;
#endif

#if (DO_HW_SUPPORT_STEP == 0)
    svoMiscVar.trackSlipCount++;
    svoStp_SetTimerMode( STEP_DISABLE_TIMER );
    track_off();
    #if (SRC_ENABLE == 1)
    HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET); //reset Tracking SRC, because the bad memory is learned when track slip.
    #endif

    svoStepVar.SledCounter = 0;  //Reset Sled Counter
    seekSetCEEq(eRoughSeek, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
    HAL_SERVO_CenterControlON(FORW);

 #if (BD_ECC_IMPROVEMENT==1)
    if ((svoMiscVar.trackSlipCount > 1) && ((svoVar_eccDiscNum >= eECC_140_um) || (svoVar_unbalanceDiscNum != 0))
     && (svoVar_discKindT.Bit.DiscStd == eDiscBD))
    {
        spdUnhold();
        TargetRpm = spdGetTargetRPM(Iram_stp_CurrentPosition, svoSpeedVar.SpdCurrentSpeed);
        TargetRpm = (TargetRpm*3)/5; //not lower than 3/5 * TargetRpm
        //reduce RPM  when tracking slip
        next_rpm = (svoSpeedVar.CurrentRpm*9)/10;//svoSpeedVar.CurrentRpm*0.9. borrow jump_distance to calculate target RPM
        if (next_rpm < TargetRpm)
        {
            next_rpm = TargetRpm;
        }
        spdHoldRPM(next_rpm);
        spdWaitSpdInRange();
    }
#endif

    jump_distance = 0;
#if (BD_ENABLE == 1)
    if (((svoMiscVar.trackSlipCount > 1) || slow_detection) && (svo_BDR_RE_PICReading != SET))
#else
    if ((svoMiscVar.trackSlipCount > 1) || slow_detection)
#endif
    {
        jump_distance = svoStp_recovery_jump_dist();
    }

    if ((jump_distance == 0) && ((svoMiscVar.trackSlipCount & 0x01) == 0) &&
        ((svoCalVar.discChkDoneBits & start_end) || ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM) && (svo_BDR_RE_PICReading != SET))))
    {
        if (svoMiscVar.trackSlipCount == 6)
        {
    #if (OPEN_SESSION == 1)
            jump_distance = Recorded_stp_Position[Layer_Index] - Iram_stp_CurrentPosition;
    #else
            jump_distance = svoStepVar.StepHomePos - Iram_stp_CurrentPosition;
    #endif
            svoMiscVar.trackSlipCount = 8;
        }
#if (BD_ECC_IMPROVEMENT==1)
        else if (Iram_stp_CurrentPosition > UM_2_SLED_STEP(28000))
        {
            if (svoCalVar.discChkDoneBits & start_end)
                jump_distance = -UM_2_SLED_STEP(1000);
            else
                jump_distance = -UM_2_SLED_STEP(100);
        }
        else
        {
            if (svoCalVar.discChkDoneBits & start_end)
                jump_distance = UM_2_SLED_STEP(1000);
            else
                jump_distance = UM_2_SLED_STEP(100);
        }

        if ((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_eccDiscNum >= eECC_140_um)&&(svoVar_discKindT.Bit.Media != eMediaROM))
        {
            jump_distance = 0;
        }
#else
        else if (Iram_stp_CurrentPosition > UM_2_SLED_STEP(28000))
        {
            if (svoCalVar.discChkDoneBits & start_end)
                jump_distance = -UM_2_SLED_STEP(1000);
            else
                jump_distance = -UM_2_SLED_STEP(100);
        }
        else
        {
            if (svoCalVar.discChkDoneBits & start_end)
            {
                if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM) && (svo_BDR_RE_PICReading == SET))
                {
                    jump_distance = UM_2_SLED_STEP(50);
                }
                else
                {
                    jump_distance = UM_2_SLED_STEP(1000);
                }
            }
            else
                jump_distance = UM_2_SLED_STEP(100);
        }

        if ((svoMiscVar.trackSlipCount >= 10) && (svoVar_discKindT.Bit.DiscStd == eDiscBD)) /* sled stop moving */
        {
            jump_distance = 0;
        }
        else if (bdDecFlag.fREADING_LEADIN == TRUE)
        {
            jump_distance = 0;
        }
#endif
    } //== start == move sled when track recovery

    if ((ABS(ReadAverageAdcmV(ADC_CE, 5) - CONVERT_DSPOFS_TO_ADCMV(svoCalVar.ceOfsData.all16)) > CE_WINDOW_MV) ||
        (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD))
    {
        seekSetCEEq(eRoughSeekRatioOnly, 128);
        HAL_SERVO_CenterControlON(FORW);
        WaitTimer(8L);
        seekSetCEEq(eRoughSeekRatioOnly, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            // More delay for DVD-RAM disc to recover tracking servo
            WaitTimer(3L);
        }
#endif
    }

    if ((jump_distance != 0) && (READ_PI_PO != 1))
    {
        SendMsgCn(DEBUG_SEEK_MSG,2,0x3FAA37,A2B(jump_distance)); //Track Slip (move_sled %d)
        svoStp_JumpSteps(STEP_NORMAL_MODE, jump_distance);
        while (svoStp_busy())
        {
            ExitProcess();
        }
    }
    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);
    Defect_Enable(DEFECT_STATE_ALL_ON);  /* Enable black and white defect */
    svoStp_SetTimerMode( STEP_READ_TIMER );
#endif /* DO_HW_SUPPORT_STEP */
    svoSpeedVar.SpdPresetNeeded = TRUE;
    svoSeekVar.jumpType |= (LIMIT_MASK|SLIP_MASK);//skip seek parameter adjustment
}

/*********************************************************
* Module: TXSpdMonitorInit()
*
* Description: Initialize svoTXSpdVar
*
* Returns: void
* Params:  void
**********************************************************/
#define TXSPD_AVG_CNT                   4
#define TRACK_SLIP_TXSPEED_SUM     (352800 * TXSPD_AVG_CNT / TRACK_SLIP_TRACK_CROSS_FREQ)
#define TRACK_SLIP_TXSPEED_SUM_DVDRAM   (352800 * TXSPD_AVG_CNT / TRACK_SLIP_TRACK_CROSS_FREQ_DVDRAM)
#define TRACK_SLIP_TXSPEED_SUM_BD  (352800 * TXSPD_AVG_CNT / TRACK_SLIP_TRACK_CROSS_FREQ_BD)
void TXSpdMonitorInit(void)
{
    svoTXSpdVar.sum = (0xFF * TXSPD_AVG_CNT);
#if (CHIP_REV <= 0xB3)
    svoTXSpdVar.pre = 0xFF;
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);
    svoTXSpdVar.pre_trk = READ_REG(HAL_SRVREG_MAP->SRCR);
#endif

#if (BD_ECC_IMPROVEMENT==1)
    svoTXSpdVar.monitor_cnt = 0;
#endif

}

/*********************************************************
* Module: TXSpdMonitor
*
* Description: Check if track slipping happens by monitor TXSPEED
*              This function must run below 5000Hz; every 200us or more
*
* Returns: BYTE, NG-track slip detected; OK-tracking ok
* Params:  void
**********************************************************/
BYTE TXSpdMonitor(void)
{
    BYTE   tempTXSpd;
    USHORT TXSpdThreshold;
#if (CHIP_REV <= 0xB3)
    USHORT cur_trk;
#endif

#if (BD_ECC_IMPROVEMENT==1)
    if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_eccDiscNum >= eECC_140_um))
    {
        WRITE_FIELD(HAL_SRV_BFILT, halSRV_BFILT_RejectFilter_88KHz);
        svoTXSpdVar.monitor_cnt++;
    }
#endif

#if (CHIP_REV <= 0xB3)
    tempTXSpd = READ_FIELD(HAL_SRV_TXSPEED);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);
    cur_trk = READ_REG(HAL_SRVREG_MAP->SRCR);
    if ((((DVDPRW) && (svoIramVar.svoCurrentSpeed == _12AX_DVD)) || (BDROMDISC)) && HAL_GLOBAL_GetRfPresent())
    {
        if ((tempTXSpd != svoTXSpdVar.pre) && (cur_trk != svoTXSpdVar.pre_trk))
        {
            svoTXSpdVar.pre = tempTXSpd;
        }
        else
        {
            tempTXSpd = 0xFF;
        }
    }
    else
    {
    if (tempTXSpd != svoTXSpdVar.pre)
    {
        svoTXSpdVar.pre = tempTXSpd;
    }
       else
    {
        tempTXSpd = 0xFF;
    }
    }

    svoTXSpdVar.pre_trk = cur_trk;
    if (tempTXSpd != 0xFF)
    {
        svoTXSpdVar.sum = svoTXSpdVar.sum * (TXSPD_AVG_CNT - 1) / TXSPD_AVG_CNT + tempTXSpd;
    }
#else
    if (READ_FIELD(HAL_SRV_TXSPDUP))
    {
        tempTXSpd = READ_FIELD(HAL_SRV_TXSPEED);
        svoTXSpdVar.sum = svoTXSpdVar.sum * (TXSPD_AVG_CNT - 1) / TXSPD_AVG_CNT + tempTXSpd;
    }
#endif

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        TXSpdThreshold = TRACK_SLIP_TXSPEED_SUM_BD;
    }
    else
#endif
#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        TXSpdThreshold = TRACK_SLIP_TXSPEED_SUM_DVDRAM;
    }
    else
#endif
    {
        TXSpdThreshold = TRACK_SLIP_TXSPEED_SUM;
    }

    if (svoTXSpdVar.sum < TXSpdThreshold)
    {
        return(NG);
    }
    return(OK);
}

/*********************************************************
* Module: track_jump()
*
* Description: perform a track jump
*
* Returns: TRUE: jump ok, FALSE: jump fail
* Params:  type:      eFakeRX, eRealRX
*          direction: FORW, BACK
*          distance:  tracks to jump
**********************************************************/
BYTE track_jump(eJumpType type, BYTE direction, USHORT distance)
{
    ULONG temp_timer;
    LONG  jump_foc_gain/*kick pulse detection*/;
#if (CHIP_REV < 0xC0)
    LONG  tkic, tkic_prv;
    BYTE  over_kick_cnt, keep_kick_cnt;
#endif
#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
    SHORT sled_step_kfg;
#endif
    BYTE  a0_cmd, status;
#if (DVD_RAM_READ == 1)
    SHORT i;
    SHORT RemainTrkNo;
    BYTE  tempTXSpd;
    BYTE  tempTXSpd_prev;
#endif
#if (WORKAROUND_FOR_BUG_5637 == 1)
    SBYTE orig_TE2OS,temp_TE2OS,delta_DSPTEOS,DSPTEOS_step;
#endif

#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
    #if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        SBYTE slo_dac;
        LONG  slo_step;
        BYTE  slo_dir = FORW;

        if (svoSeekVar.svoTargetID.SectorNum.all32s >= 0x30000)
        {
            // In re-writable area, track-pitch = 1.23 um
            sled_step = UM_2_SLED_STEP(distance * 1.23);
        }
        else
        {
            // In embossed area, track-pitch = 0.74 um
            sled_step = UM_2_SLED_STEP(distance * 0.74);
        }
        sled_step = (sled_step > 3) ? (sled_step - 3) : 0;  //Dead zone (3*4.6875)/1.23 = 11.4 tracks

        // Get the current lens position from SLO dac to compesate active sled step.
        // For DVD-RAM, SLO sensitivity is 3 um/SLO.
        #if (TRO_INVERSION == 1)
        slo_dac = READ_FIELD(HAL_SRV_SLED);
        #else
        slo_dac = -READ_FIELD(HAL_SRV_SLED);
        #endif
        if (slo_dac < 0)
        {
            slo_dir = BACK;
            slo_dac = -slo_dac;
        }

        if (slo_dac > DVRAM_THRESHOLD_F)
        {
            slo_step = UM_2_SLED_STEP(slo_dac * 1.5);
        }
        else
        {
            slo_step = UM_2_SLED_STEP(slo_dac * 0.75);
        }

        if(slo_dir == direction)
        {  //Jump direction and lens position are same, need more steps
            sled_step = sled_step + slo_step;
        }
        else
        {  //Jump direction and lens position are not same, need less steps
            sled_step = (sled_step > slo_step) ? (sled_step - slo_step) : 0;
        }
    }
    else
    #endif // #if (DVD_RAM_READ == 1)
    {
        sled_step_kfg = distance * svoSeekVar.parRoughInit / (svoSeekVar.parFineInit * 4);//from 720
        //sled_step = distance * svoSeekVar.parRough / (svoSeekVar.parFine * 4);
    }

    if (direction == BACK)
    {
        sled_step_kfg = -sled_step_kfg;
    }
    //if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    if (0)
    {
#if (TRO_INVERSION == 1)
        sled_step_kfg -= (((LONG)ReadAverageAdcmV(ADC_CE,5) - vref_fm_mux3_mV) * (MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (DISTANCE_PER_MACH_REV * CE_SEN_TARGET_MV_MM / 1000));
#else
        sled_step_kfg += (((LONG)ReadAverageAdcmV(ADC_CE,5) - vref_fm_mux3_mV) * (MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (DISTANCE_PER_MACH_REV * CE_SEN_TARGET_MV_MM / 1000));
#endif
    }

    if ((ABS(sled_step_kfg) > UM_2_SLED_STEP(30)) && ((direction == BACK)? (sled_step_kfg < 0) : (sled_step_kfg > 0)))
    {
        svoStp_JumpSteps(STEP_NO_CE_SLOW_MODE,sled_step_kfg);
    }
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 1)

#if (EN_ADEQ == 1)
   if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW)) && (!DVDRAM)
   && (READ_FIELD(HAL_DFE_ADEQDIS) == 0) && (Disable_ADEQ == 0))
    {
        InitAdaptEq();//turn off ADEQ

    }
#endif

#if (TRO_INVERSION == 1)
    if (direction == BACK)
#else
    if (direction == FORW)
#endif
    {
#if (CHIP_REV <= 0xB3)
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_FAKETX_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_FAKERX|HAL_SRV_DIRPOL);
#else
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_TXRXFILT_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_TXRXFILT|HAL_SRV_FAKERX|HAL_SRV_DIRPOL);
#endif
    }
    else
    {
#if (CHIP_REV <= 0xB3)
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_FAKETX_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_FAKERX);
#else
        READ_MOD_WRITE(HAL_SRVREG_MAP->CDRWSEEK,HAL_SRV_FAKERX_MASK|HAL_SRV_DIRAUT_MASK|HAL_SRV_TXRXFILT_MASK|HAL_SRV_DIRPOL_MASK,HAL_SRV_TXRXFILT|HAL_SRV_FAKERX);
#endif
    }

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)//from 720
    {
        WRITE_FIELD(HAL_SRV_TESRCDB,0x60);  // TE SRC Debounce Contro=>88.KHz  =(33.8688 MHz /0x60)-264k :HW delay 264K
    }
    else
    {
        WRITE_FIELD(HAL_SRV_TESRCDB,0x40);  // TE SRC Debounce Contro=>265KHz  =(33.8688 MHz /0x40)-264k :HW delay 264K
    }
    WRITE_FIELD(HAL_SRV_BFILT, halSRV_BFILT_RejectFilter_44KHz);

    if (svoVar_discKindT.Bit.DiscStd != eDiscBD)
    {
        WRITE_FIELD(HAL_AFE_TXLP,halAFE_TXLPF_15kHz);
        WRITE_FIELD(HAL_AFE_TXLPSET,1);//Double=>15*2 = 30kHz

        WRITE_FIELD(HAL_AFE_TEFCSFSEL,halAFE_TELPF_15kHz);
        WRITE_FIELD(HAL_AFE_TELPSET,1);//Double=>15*2 = 30kHz
    }

#if (DVD_RAM_READ == 1)
    //Use deglitch function of fake TX to filter header noise for DVD-RAM
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_SRV_TXRXFILT,0);
    }
#endif
    WRITE_FIELD(HAL_SRV_FORBLK,1);
    WRITE_FIELD(HAL_SRV_RXMUTE,1);
    if ((type == eRealRX) || ((svoVar_discKindT.Bit.Media == eMediaROM)&&(svoMiscVar.trackSlipCount <= 2))) //use tracking hysteresis aftre fine seek tracking on .
    {
        WRITE_FIELD(HAL_SRV_HYSL, halSRV_Hysteresis_2to3ms);
        READ_MOD_WRITE(HAL_SRVREG_MAP->SREG2,(HAL_SRV_GUP1|HAL_SRV_HYS1|HAL_SRV_HYS2|HAL_SRV_GUP2),(HAL_SRV_HYS2|HAL_SRV_GUP2));
    }
    else
    {
        READ_MOD_WRITE(HAL_SRVREG_MAP->SREG2,(HAL_SRV_GUP1|HAL_SRV_HYS1|HAL_SRV_HYS2|HAL_SRV_GUP2),HAL_SRV_GUP2);
    }

#if FINE_SEEK_SURF_TELFG == 1
    WRITE_FIELD(HAL_SRV_PGTK, FALSE);
    a0_cmd = HAL_SRV_FVKSURF; /* SURF ON */
#else
    a0_cmd = 0;               /* SURF OFF*/
#endif

#if (TRO_INVERSION == 1)
    if (direction == BACK)
#else
    if (direction == FORW)
#endif
    {
        a0_cmd |= HAL_SRV_FVKBF;  /* reverse */
    }

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    if (distance > FEED_THRESHOLD)
    {
        a0_cmd |= HAL_SRV_FVKFDC; /* feed ON */
    }
#endif

    DISABLE_INTERRUPT();

#if (BD_ENABLE == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM))
    {
        //Leaving out the PIC zone
        if ((svo_BDR_RE_PICReading == SET) && (bdDecFlag.fREADING_PIC == CLEAR) && (svoSeekVar.svoTargetID.SectorNum.all32s >= PIC_L0_MAX_ADDR))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x334702); //track_jump: Exiting the PIC zone
            svo_BDR_RE_PICReading = CLEAR;
            LAL_PLL_SetPll();
            LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
            WRITE_FIELD(HAL_AFE_BLANKTRKG, svoCalGnT1Var[0][TE_GN].groove_gain[RECDL]);//from 720
            WRITE_FIELD(HAL_AFE_TRKG      ,svoCalGnT1Var[0][TE_GN].groove_gain[RECDH]);
            WRITE_FIELD(HAL_AFE_BLANKTRKO, svoCalOfsT1Var[0][TE_OS].groove_offset[RECDL]);//from 720
            WRITE_FIELD(HAL_AFE_TRKO      ,svoCalOfsT1Var[0][TE_OS].groove_offset[RECDH]);
        }
        else if ((svo_BDR_RE_PICReading == CLEAR) && (bdDecFlag.fREADING_PIC == SET) && (svoSeekVar.svoTargetID.SectorNum.all32s < PIC_L0_MAX_ADDR))
        {
            // Getting into the PIC zone
            SendMsg80(SHOW_DEBUG_MSG,0x334703); //track_jump: Entering the PIC zone
            Defect_Enable(DEFECT_SOURCE_FREEZE_BS);
            svo_BDR_RE_PICReading = SET;
            LAL_PLL_SetPll();
            LAL_SetResetPIC(eSET_PIC, svo_bReadZoneSpeedX10);
            WRITE_FIELD(HAL_AFE_BLANKTRKG, svoCalGnT1Var[0][TE_GN].li_gain);//from 720
            WRITE_FIELD(HAL_AFE_TRKG, svoCalGnT1Var[0][TE_GN].li_gain);
            WRITE_FIELD(HAL_AFE_BLANKTRKO, svoCalOfsT1Var[0][TE_OS].li_offset);//from 720
            WRITE_FIELD(HAL_AFE_TRKO, svoCalOfsT1Var[0][TE_OS].li_offset);
        }
    }
#endif //(BD_ENABLE == 1)

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
#if (CHIP_REV >= 0xC1)
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW3_400kHz);
#else
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW_400kHz);
#endif
    }
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
    Set_TOK_Mode(TOK_SEEK);
#else
    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_300mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_300mV);
    #endif
#endif

    /* Set CE EQ and relative parameters for fine seek action and enable HW brake pulse */
    seekSetFineJumpParas(distance);

    switch(svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            jump_foc_gain = FOCUS_FJ_GAIN_FACTOR_CD;
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            jump_foc_gain = FOCUS_FJ_GAIN_FACTOR_BD;
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            jump_foc_gain = FOCUS_FJ_GAIN_FACTOR_HD;
            break;
#endif

        default:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                jump_foc_gain = FOCUS_FJ_GAIN_FACTOR_DVDRAM;
            }
            else
#endif
            {
                jump_foc_gain = FOCUS_FJ_GAIN_FACTOR_DVD;
            }
            break;
    }
    sled_step_kfg = READ_DSP_COEF(kfg);//from 720
    //jump_foc_gain = jump_foc_gain * (SHORT)svoCalMisc1Var[Layer_Index].feGainDSP / 128;
    jump_foc_gain = jump_foc_gain * sled_step_kfg / 128;//from 720
    if (jump_foc_gain > 32767)
    {
        jump_foc_gain = 32767;
    }
    if (jump_foc_gain < -32768)
    {
        jump_foc_gain = -32768;
    }

    //WRITE_DSP_COEF(kfg, jump_foc_gain); //[S0A] Shin debug
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD) //[S07] for ready noise
    {
        WRITE_DSP_COEF(kfg, (jump_foc_gain)*0.6);
    }
    else
    {
        WRITE_DSP_COEF(kfg, (jump_foc_gain)*0.7);
    }
#if FINE_SEEK_SURF_TELFG == 0
    seekSetCEEq(eFineSeek, 128);
#endif
    //TON_ON_OFF(OFF);    /* disc servo mode "FG" *///from 720

#if DO_HW_SUPPORT_STEP == 1
    hw_step_motor_fine_seek();
#else /* DO_HW_SUPPORT_STEP */
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    svoStp_SetTimerMode( STEP_FINE_TIMER );
    #endif
#endif /* DO_HW_SUPPORT_STEP */

    WRITE_FIELD(HAL_SRV_GUPL, halSRV_GainUp_2to3ms);

//#if (DEFECT_LENGTH_MEASURE == 1)
//    libDisableDFCTInt();
//#endif

    /* Do not use DC filter for tracking error signal when detecting defects */
    #if (TRACKOFF_SEEK_USE_BS_DEFECT == 1)
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);
    #else
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_DFE_RF);
    #endif

#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        //
        // Synchronize with header to improve DVD-RAM fine seek performance.
        // Note: Please do not change the sequence of following statements.
        //

        // Save current wobble frequency to Wobble PLL and hold it on.
        WblPllFreqPreset = READ_FIELD(HAL_WBL_WBLFREQ);

        SetDfeWblFrontOnHold();

        WRITE_FIELD(HAL_WBL_WBLFREQ, WblPllFreqPreset);

        /* Synchronize header to improve DVD-RAM fine seek performance */

        // Wait for start of header
        i = 0;
        while ((READ_FIELD(HAL_DVDD_DVDRAMHDR) == 0) && (i < 5000)) i++ ;

        // Wait for end of header
        i = 0;
        while (READ_FIELD(HAL_DVDD_DVDRAMHDR) && (i < 1000)) i++;

        //Extend header hold length to prevent header noise into TE signal before fine seek.
        //Turn on mute TE function.
        WRITE_FIELD(HAL_AFE_TEREGHLD, 1); //Mute TE to Vref

        if (zone_change)
        {
            WblPllFreqPreset = 0;
            LAL_WOBBLE_SetHeaderDetectMode(SlicedWobble);
            //LAL_WOBBLE_SetHeaderDetectMode(SlicedRF);
        }

        ENABLE_HDR_HOLD_SERVO_JUMP;

        // Preset target type of track.
        if (READ_FIELD(HAL_DVDD_LAND))
        {
            WRITE_FIELD(HAL_DVDD_TRGTTRK,1);
        }
        else
        {
            WRITE_FIELD(HAL_DVDD_TRGTTRK,0);
        }

        //Extend header hold length to prevent header noise into TE signal before fine seek.
        //Turn off mute TE function.
        WRITE_FIELD(HAL_AFE_TEREGHLD, 0); //Don't mute TE to Vref
    }
    else
#endif // #if (DVD_RAM_READ == 1)
    {
        SetDfeWblFrontOnHold();
    }

    svoIramVar.fineSeekDoneFlag = FALSE;

    //kick pulse reverse detection >>>
    HAL_DSP_SetDebug(DEBUG_CH0, DEBUG_SOURCE_RGLUE, 0x1C, 8);//TKIC
    jump_foc_gain = ((READ_REG(HAL_SRVREG_MAP->KICKLVL) * (READ_DSP_COEF(kspd_start) >> 8) * 3) << 3); //3/4 of normal
    if (jump_foc_gain > 0x4000)
    {
        jump_foc_gain--;
    }
    jump_foc_gain = (jump_foc_gain > 32735)? 32735 : jump_foc_gain;
    if ((a0_cmd & HAL_SRV_FVKBF) == 0)
    {
        jump_foc_gain = -jump_foc_gain;
    }
    //kick pulse reverse detection <<<

#if (SRC_ENABLE == 1)
    HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif

#if (WORKAROUND_FOR_BUG_5637 == 1)
    if (READ_FIELD(HAL_SRV_INHIEN))
    {
        orig_TE2OS = calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][TE2_OS].normal_offset, 8);
        delta_DSPTEOS = 0;
    #if (TRO_INVERSION == 0)
        DSPTEOS_step = (direction == FORW) ? 20 : -20;
    #else
        DSPTEOS_step = (direction == BACK) ? 20 : -20;
    #endif

        temp_TE2OS = READ_FIELD(HAL_AFE_TEINV)? -1 : 1;
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);  /* read/write Tracking Offset Data data */
        while(ABS(delta_DSPTEOS) < 100)
        {
            delta_DSPTEOS += DSPTEOS_step;
            WRITE_REG(HAL_AFEREG_MAP->FCTS26,calSBYTE2DacCount(orig_TE2OS + (temp_TE2OS * delta_DSPTEOS * (GPADC_IN_RANGE * 2 / 5)) / (1 << GPADC_BITS),8));
            WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.teOfsData.all16 + svoCalMisc1Var[Layer_Index].teSetPoint + delta_DSPTEOS);
        }
        Delay100uS(1);
    }
#endif

    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);      /* Clear Seek completion interrupt */
    WRITE_FIELD(HAL_SRV_SEEKCEN, 1);    /* Enable Seek completion interrupt */
    WRITE_REG(HAL_SRVREG_MAP->FVKICM, a0_cmd);
    WRITE_REG(HAL_SRVREG_MAP->FVKICK, distance);

    //from 720
    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        WRITE_FIELD(HAL_AFE_RXTOK, 1); //Disable XTOK mute RX
    }

#if (CHIP_REV < 0xC0)
    //kick pulse reverse detection <<<
    for (temp_timer = 0; temp_timer < 64; temp_timer++)
    {
        tkic = READ_DSP_COEF(debug0);
        if (tkic != 0)
        {
            if ((tkic * jump_foc_gain * 2) < (jump_foc_gain * jump_foc_gain))
            {
                // if real kick (tkic) is less than half of normal.
                WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);
                WRITE_REG(HAL_SRVREG_MAP->FVKICM, 0);
                WRITE_REG(HAL_SRVREG_MAP->FVKICK, 0);
                SendMsg80(SHOW_DEBUG_MSG,0x334105);     // WARNING: FineSeek wrong kick
                svoSEEKIsr();
                svoSeekVar.jumpType |= LIMIT_MASK;
            }
            break;
        }
    }
    //kick pulse reverse detection <<<
    tkic_prv = tkic;
    over_kick_cnt = 0;
    keep_kick_cnt = 0;
#endif

    //kick pulse detection <<<
#if (WORKAROUND_FOR_BUG_5637 == 1)
    if (READ_FIELD(HAL_SRV_INHIEN) == 1)
    {
        Delay1uS(70);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);  /* read/write Tracking Offset Data data */
        WRITE_REG(HAL_AFEREG_MAP->FCTS26, svoCalOfsT2Var[Layer_Index][TE2_OS].normal_offset);
        WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.teOfsData.all16 + svoCalMisc1Var[Layer_Index].teSetPoint);
        WRITE_FIELD(HAL_SRV_RXMUTE,0);
    }
#endif

    jump_foc_gain = ABS(jump_foc_gain);
    ENABLE_INTERRUPT();

    /* Read the start time for fine seek timeout */
    StartTimer(&temp_timer);

    status = SEEK_BUSY;
    for(;;)
    {
#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);/* jump track number register           */
            RemainTrkNo=READ_REG(HAL_SRVREG_MAP->SRCR);

            if ((RemainTrkNo==0x1FFF)||(RemainTrkNo==0x0001))
            {
                tempTXSpd_prev = READ_FIELD(HAL_SRV_TXSPEED);
            }

            if(RemainTrkNo==0x0000)
            {
                tempTXSpd = READ_FIELD(HAL_SRV_TXSPEED);

                //if( (distance> 1)&&(( tempTXSpd < 0x30)||(tempTXSpd_prev == tempTXSpd )))      //352KHz/32=11KHz, if TXSPEED is higher than 11KHz, disable brake pulse
                if ((distance > 1)&&(tempTXSpd < 0x10))  //352.8KHz/16 = 22.05 KHz
                {
                    WRITE_REG(HAL_SRVREG_MAP->BRAKELVL, 0);
                }
            }
        }
#endif  // #if (DVD_RAM_READ == 1)

        if (svoIramVar.fineSeekDoneFlag == TRUE)
        {
            status = SEEK_OK;
            break;
        }

        if (ReadTimer(&temp_timer) >= (MS_20 + distance/8))
        {
            svoStp_SetTimerMode( STEP_DISABLE_TIMER );
            track_off();
            status = SEEK_TIMEOUT;
            if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (HAL_GLOBAL_GetRfPresent() == OFF))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x334108);     // WARNING: FineSeek Timeout due to touch no RF zone
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x334002);     // WARNING: FineSeek Timeout
            }
            break;
        }

        //servo monitor
        if (!FokStatusOK())
        {
            SendMsg80(SHOW_DEBUG_MSG,0x334009); //WARNING: FineSeek FocusDrop
            svoStp_SetTimerMode( STEP_DISABLE_TIMER );
            track_off();
            status = SEEK_FDROP;
            send_msg80(SHOW_DEBUG_MSG,0x534009);    // WARNING: FineSeek FocusDrop
            break;
        }

#if (CHIP_REV < 0xC0)
        if (ReadTimer(&temp_timer) >= 2)
        {
            tkic = READ_DSP_COEF(debug0);
            if (tkic != tkic_prv)
            {
                if (ABS(tkic) > jump_foc_gain) //if tkic > 3/4 * kick
                {
                    over_kick_cnt++;
                }
                else
                {
                    over_kick_cnt = 0;
                }
                keep_kick_cnt = 0;
                tkic_prv = tkic;
            }
            else
            {
                // the tkic doesn't update
                if (ABS(tkic) > jump_foc_gain)
                {
                    keep_kick_cnt = keep_kick_cnt + 4;
                }
                else if ( ABS(tkic) > 4096)
                {
                    keep_kick_cnt++;
                }
            }

            if (((over_kick_cnt > 1) || (keep_kick_cnt > 0x80))) //keep_kick_cnt 0x80 is about 500us
            {
                svoStp_SetTimerMode(STEP_DISABLE_TIMER);
                track_off();
                seekSetCEEq(eRoughSeek, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
                HAL_SERVO_CenterControlON(FORW);
                if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (HAL_GLOBAL_GetRfPresent() == OFF))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x334107);     // WARNING: FineSeek keep kick due to touch no RF zone
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x334106);     // WARNING: FineSeek keep kick
                }
                status = SEEK_POOR_CONTROL;  //bad velocity control
                break;
            }
        }
#endif
    }

    WRITE_FIELD(HAL_SRV_SEEKCEN, 0);
    LAL_WriteCoef(SRV_TESRCCTL_TESRCDB);
    if (svoVar_discKindT.Bit.DiscStd != eDiscBD)
    {
        LAL_WriteCoef(AFE_FCTS39_TXLP);
        LAL_WriteCoef(AFE_FCTS39_TXLPSET);

        LAL_WriteCoef(AFE_FCTS38_TEFCSFSEL);
        LAL_WriteCoef(AFE_FCTS38_TELPSET);    
    }    
#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        WRITE_FIELD(HAL_SRV_FAKETX,1);
        ENABLE_HDR_HOLD_SERVO_NORMAL;
    }
#endif

#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
    Set_TOK_Mode(TOK_SEEK);
#else
    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_450mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_450mV);
    #endif
#endif

    //WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);
    //WRITE_DSP_COEF(kfg, sled_step_kfg);//from 720  //[S0A] Shin debug
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD) //[S07] for ready noise
    {
        WRITE_DSP_COEF(kfg, (svoCalMisc1Var[Layer_Index].feGainDSP)*0.6);
    }
    else
    {
        WRITE_DSP_COEF(kfg, (svoCalMisc1Var[Layer_Index].feGainDSP)*0.7);
    }
    if (status == SEEK_OK)
    {
        //svoEnTimeRead();    // TBD? Comment! will be taken care in state_pll_Chk that comes after (no need in JUMP_TBL/RELSE_TBL)
        //TON_ON_OFF(ON);                         /* Reset Atip Decoder (for CD-R/CD-RW) *///from 720
        SetDfeWblFrontSemiSynchr();             /* For sure needed in case of JUMP_TBL and RELSE_TBL */

        /* Use DC filter for focus/tracking error signal when detecting defects */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);

#if DO_HW_SUPPORT_STEP == 1
        hw_step_motor_normal_play();
#else /* DO_HW_SUPPORT_STEP */
        svoStp_SetTimerMode( STEP_READ_TIMER );
#endif /* DO_HW_SUPPORT_STEP */

        return TRUE;
    }
    else
    {
        svoSeekVar.jumpType |= LIMIT_MASK;
        svoSEEKIsr();
        ClearAddressRead();
        SetDfeWblFrontOnHold();
        return FALSE;
    }
}

/*********************************************************
* Module: track_off()
*
* Description: TBD!
*
* Returns: void
* Params:  void
**********************************************************/
void track_off(void)
{
    BYTE gain_down_factor;

    SetDfeWblFrontOnHold();

    WRITE_FIELD(HAL_AFE_TOKLVL, TOKLVL_TRACK_OFF);

#if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0) && ((DVDRAM_FREEZE_FLYWHEEL == 1) || (DVDRAM_FREEZE_FLYWHEEL == 2))
    if (DVDRAM_rewritable && (land_flywheel_state == LAND_FLYWHEEL_FREEZE))
    {
        land_flywheel_state = LAND_FLYWHEEL_HUNT;
        WRITE_FIELD(HAL_DVDD_TRACKCTRL, 0);
        WRITE_FIELD(HAL_WBL_HDRFLYRD, 0);
    }
#endif

#if (SRC_ENABLE == 1)
    HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif
    //TON_ON_OFF(OFF);
    //switch off unconditional hysteresis and gainup, or velocity control (fine jump)
    WRITE_REG(HAL_SRVREG_MAP->FVKICM,0);
    WRITE_REG(HAL_SRVREG_MAP->FVKICK,0);

    WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);//track off

    /* Do not use DC filter for tracking error signal when detecting defects */
#if (TRACKOFF_SEEK_USE_BS_DEFECT == 1)
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);
#else
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON);
#endif
    switch(svoVar_discKindT.Bit.DiscStd)
    {
#if (BD_ENABLE == 1)
        case eDiscBD:
            gain_down_factor = FOCUS_TOFF_GAIN_FACTOR_BD;
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            gain_down_factor = FOCUS_TOFF_GAIN_FACTOR_HD;
            break;
#endif
        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
                gain_down_factor = FOCUS_FJ_GAIN_FACTOR_DVDRAM;
            else
#endif
                gain_down_factor = FOCUS_TOFF_GAIN_FACTOR_DVD;
            break;

        case eDiscCD:
        default:
            gain_down_factor = FOCUS_TOFF_GAIN_FACTOR_CD;
            break;
    }
    WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP * gain_down_factor / 128);    /* kfg gain down */
}

/**********************************************************
* Clear Address Read
***********************************************************/
void ClearAddressRead(void)
{
#if (BD_ENABLE == 1)
    BYTE Temp;
#endif

    DISABLE_INTERRUPT();

    if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {
        CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);    // reset CIDI: Current ID Ready Interrupt Flag
        if ((svoVar_discKindT.Bit.Media == eMediaR)  ||
            (svoVar_discKindT.Bit.Media == eMediaRW) ||
            (svoVar_discKindT.Bit.Media == eMediaRAM))
        {
            pdbSetIDOk(FALSE);
        }
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            svoIramVar.headerIDReadyFlag = FALSE;
        }
#endif
    }
#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        //Disable the header interrupt
        WRITE_FIELD(HAL_BDC_AUNDECE, HalBDC_HeaderInterrDisable);

        pdbSetIDOk(FALSE);

        //Clear sub data buffer by reading the AUNFLGBT field
        Temp = READ_FIELD(HAL_BDC_AUNFLGBT);

        //Reset AUN flywheel confidence level
        WRITE_REG(HAL_BDCREG_MAP->BDAUNFLY,0);

        //Reset Wobble flywheel
        //WRITE_REG(HAL_WBLREG_MAP->ADRFLYCFG,0x01);
        //WRITE_REG(HAL_WBLREG_MAP->ADRFLYCFG,0x00);

        //Enable the header interrupts
        //WRITE_FIELD(HAL_BDC_AUNDECE, HalBDC_HeaderInterrEnable);
    }
#endif
#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
    {
        CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);    // reset CIDI: Current ID Ready Interrupt Flag
        if ((svoVar_discKindT.Bit.Media == eMediaR)  ||
            (svoVar_discKindT.Bit.Media == eMediaRW) ||
            (svoVar_discKindT.Bit.Media == eMediaRAM))
        {
            pdbSetIDOk(FALSE);
        }
        if (svoVar_discKindT.Bit.Media == eMediaRAM)
        {
            svoIramVar.headerIDReadyFlag = FALSE;
        }
    }
#endif

#if (EN_WOBBLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        ENABLE_WBLINT();
    }
#endif

    ENABLE_INTERRUPT();
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: set_DVDRAM_mode()
 * Description:
 *      initial DVDRAM related setting
 *
 *----------------------------------------------------------------------------*/
void set_DVDRAM_mode()
{
    HAL_LASC_SetLaserOn(FALSE);
    HAL_DSP_SRCEnable(SRC_LOOP_ALL, SRC_STATE_OFF);
    HAL_DSP_SRCEnable(SRC_LOOP_ALL, SRC_STATE_RESET);

    svoVar_discKindT.fDiscKind = eDVDRAMDisc;
    //svoVar_discKindT.Bit.Media = eMediaRAM; //alread done by previous line. eDVDRAMDisc = MakeDiscKind(eDiscDVD, eLayersSingle, eWblUnknown, eMediaRAM)
    svo_preset_media_par();

#if (EN_POWER_CONTROL == 1)
    PowerControl(eDVDReadWblPower);
#endif
    ENABLE_DVD_DECODER();

    svoCalVar.discChkDoneBits |= stm_chk_end|disc_chk_end;
    svoCalVar.calGainDoneBits &= ~(febg_end|feg_bsg_end);
    svoCalVar.calGainDoneBits |= rpg_end;                      // DVDRAM always uses no-RX seek, skip RP gain and offset
    svoCalVar.calOfsDoneBits   = phbhof_end|rpof_end;          // calibration, and skip PH and BH offset calibraiton.

    svoVar_TrackingMethod = TRK_METHOD_UNKNOWN;
    afeSetTrackingMethod(TRK_METHOD_DPP);
    bSetSpeedAgain = TRUE;
    spdChangeDiscSpeed(SVO_CAL_SPEED_DVDRAM);
    preHeaderDetectMode = Unknown;
    target_zone = 1;
    zone_change = 1;

    set_DVDRAM_rewritable_mode();
    spdHoldRPM(FEpp_MEAS_RPM_DVDRAM);
    LAL_AFE_ClearAFEOffsets();
    LAL_AFE_InitAFEGains();
    calSetAfeGain(0,RFPGA_GN,TYPE3,0,0,COEF_GetValue(AFE_INIT_RF04_RFAGCREG));  // Re-load RFAGC
    calSetAfeGain(0,RF_GN,TYPE2,PATH_SELECT_ALL,0,COEF_GetValue(AFE_INIT_RF06_RFGN));  // Re-load RFGN

    HAL_WOBBLE_SetEnableHeaderOff();
    LAL_WOBBLE_SetHeaderDetectMode(MLH);
    HAL_WOBBLE_SetLandGrooveDetectMode(Normal);
    WRITE_FIELD(HAL_DVDD_BADPIDTH,2);
    WRITE_FIELD(HAL_DVDD_TRACKSIZE,0);
    //WRITE_FIELD(HAL_DVDD_WOBLAND,1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: set_DVDRAM_rewritable_mode()
 * Description:
 *      initial DVD-RAM related setting for rewritable area
 *
 *----------------------------------------------------------------------------*/
void set_DVDRAM_rewritable_mode()
{
    WRITE_FIELD(HAL_DVDD_MANLAND,0);     // Auto LAND control
    WRITE_FIELD(HAL_AFE_LANDSEL,1);      // Enable LAND select
    WRITE_FIELD(HAL_AFE_PDDPD,1);        // Power down DPD circuit
    WRITE_FIELD(HAL_SRV_DVDRHHLD,1);     // Enable DVDRAM header hold servo DSP TRO/FOO
    if (DiscTypeIndex == LOC_DISC_TYPE_10)
    {
        SetSvoEqBank(Tracking_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_FACTOR_DVDRAM_VER_10);
        SetSvoEqBank(Tracking_U_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_GAINUP_FACTOR_DVDRAM * TRACKING_FACTOR_DVDRAM_VER_10 / 128);
    }
    else
    {
        SetSvoEqBank(Tracking_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], 128);
        SetSvoEqBank(Tracking_U_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_GAINUP_FACTOR_DVDRAM);
    }
    svo_bReadZoneSpeedX10 = 0;//force UpdateReadFilters()
    SetReadFilters();
}

/*-----------------------------------------------------------------------------
 *
 * Module: set_DVDRAM_embossed_mode()
 * Description:
 *      initial DVDRAM related setting for embossed area
 *
 *----------------------------------------------------------------------------*/
void set_DVDRAM_embossed_mode()
{
    WRITE_FIELD(HAL_DVDD_MANLAND,1);     // Auto LAND control
    WRITE_FIELD(HAL_AFE_LANDSEL,0);      // Disable LAND select
    WRITE_FIELD(HAL_AFE_PDDPD,0);        // Power on DPD circuit
    WRITE_FIELD(HAL_SRV_DVDRHHLD, 0);    // Disable DVDRAM header hold servo DSP TRO/FOO
    SetSvoEqBank(Tracking_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_FACTOR_DVDRAM_EMBOSS);
    SetSvoEqBank(Tracking_U_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_GAINUP_FACTOR_DVDRAM * TRACKING_FACTOR_DVDRAM_EMBOSS / 128);
    svo_bReadZoneSpeedX10 = 0;//force UpdateReadFilters()
    SetReadFilters();
}

/*-----------------------------------------------------------------------------
 *
 * Module: trk32_calc_one_zone()
 * Description: Translate address difference[sector] to tracks difference[1track/32]
 * Return:
 *----------------------------------------------------------------------------*/
static LONG trk32_calc_one_zone(LONG DeltaPSN,BYTE zone)
{
    LONG sectors_per_track;

    if (zone == 0xFF)
    {   //Enbossed area
        sectors_per_track = 9;    //18/2
    }
    else
    {
        sectors_per_track = DVDRAMZoneTable[zone].SectorsPerRev;
    }
    return ((LONG)(DeltaPSN * 32) / sectors_per_track);
}

/*-----------------------------------------------------------------------------
 *
 * Module: trk32_calc_DVDRAM()
 * Description: Translate address difference[sector] to tracks difference[1track/32]
 * Return:
 *----------------------------------------------------------------------------*/
LONG trk32_calc_DVDRAM(LONG DeltaPSN,LONG TargetPSN)
{
    BYTE tar_zone,cur_zone;
    LONG delta_trks,temp_deltaPSN;

    if (TargetPSN < 0x30000)
    {
        tar_zone = 0xFF;
        if (DeltaPSN >= 0)
        {
            temp_deltaPSN = DeltaPSN;
        }
        else
        {
            cur_zone = 0;
            temp_deltaPSN = TargetPSN - 0x02FFFF;
            if (temp_deltaPSN < DeltaPSN)
            {
                temp_deltaPSN = DeltaPSN;
            }
        }
    }
    else
    {
        tar_zone = DVDRAM_PSN2ZONE(TargetPSN);
        if (DeltaPSN >= 0)
        {
            cur_zone = tar_zone - 1;
            if (cur_zone == 0xFF)
            {
                temp_deltaPSN = TargetPSN - 0x02FFFF;
            }
            else
            {
                temp_deltaPSN = TargetPSN - DVDRAMZoneTable[cur_zone].EndSectorPSN;
            }
            if (temp_deltaPSN > DeltaPSN)
            {
                temp_deltaPSN = DeltaPSN;
            }
        }
        else
        {
            if (tar_zone == (LocalZoneNumber-1))
                cur_zone = tar_zone;
            else
                cur_zone = tar_zone + 1;

            temp_deltaPSN = TargetPSN - DVDRAMZoneTable[tar_zone].EndSectorPSN;
            if (temp_deltaPSN < DeltaPSN)
            {
                temp_deltaPSN = DeltaPSN;
            }
        }
    }

    delta_trks = trk32_calc_one_zone(temp_deltaPSN,tar_zone);
    DeltaPSN -= temp_deltaPSN;
    if (DeltaPSN != 0)
    {
        delta_trks += trk32_calc_one_zone(DeltaPSN,cur_zone);
    }

    return (delta_trks);
}
#endif // #if (DVD_RAM_READ == 1)

const BYTE  DSP_ADC_ADDR[4][2] = {
    {halSRV_RSEL_TEBOTTOM, halSRV_RSEL_TEPEAK}, /* TE Peak Bottom */
    {halSRV_RSEL_RPBOTTOM, halSRV_RSEL_RPPEAK}, /* RP Peak Bottom */
    {halSRV_RSEL_FEBOTTOM, halSRV_RSEL_FEPEAK}, /* FE Peak Bottom */
    {halSRV_RSEL_BSBOTTOM, halSRV_RSEL_BSPEAK}  /* BS Peak Bottom */
};

/*-----------------------------------------------------------------------------
 *
 * Module: ReadDSPADCmV()
 * Description: Read ADC value from DSP registers (TE, RP, FE, BS peak/bottom hold).
 *
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadDSPADCmV(eRevT Revolutions, BYTE selected_signal, BOOL need_fok)
{
    ad_dsp_t res;
    ULONG    min_data, max_data;

    WRITE_FIELD(HAL_SRV_PKBOT, 0);
    WRITE_FIELD(HAL_SRV_PKBOT, 1);    /* reset Peak/Bottom */
    switch(Revolutions)
    {
#if (CHIP_REV < 0xC0)
        case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
        case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
        case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
        default:
            svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions;
            break;
#else
        case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
        case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
        case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
        default:
            svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
            break;
#endif
    }
    svoSpeedVar.fgCount++;
    StartFGTimer();

    while (svoSpeedVar.fgCount)
    {
        ExitProcess();
    }
    WRITE_FIELD(HAL_SRV_PKBOT, 0);
    if ((need_fok == TRUE) &&  (!FokStatusOK()))
    {
        res.errcode       = eERR_FOK_ad_dsp_read;
        res.top_saturated = FALSE;
        res.bot_saturated = FALSE;
        res.middle_mv     = 0;
        res.p2p_mv        = 0;
        res.top_mv        = 0;
        res.bot_mv        = 0;
        return(res);
    }
    //-------------------------------------------------;
    // collect selected bottom value                   ;
    //-------------------------------------------------;
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, DSP_ADC_ADDR[selected_signal][0]);    /* read bottom data */
    min_data = READ_REG(HAL_SRVREG_MAP->SRCR);

    //-------------------------------------------------;
    // collect selected top value                      ;
    //-------------------------------------------------;
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, DSP_ADC_ADDR[selected_signal][1]);    /* read Peak data */
    max_data = READ_REG(HAL_SRVREG_MAP->SRCR);

    max_data = ((max_data * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;// unit (mV)
    min_data = ((min_data * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;// unit (mV)

    if (max_data > TOP_SATURATE_LIM)
        res.top_saturated = TRUE;
    else
        res.top_saturated = FALSE;

    if (min_data < BOT_SATURATE_LIM)
        res.bot_saturated = TRUE;
    else
        res.bot_saturated = FALSE;

    //-------------------------------------------------;
    // calculate P2P and so-called LP values           ;
    //-------------------------------------------------;
    res.middle_mv = ((max_data + min_data) >> 1);
    res.p2p_mv    = (max_data - min_data);

    //-------------------------------------------------;
    // readjust the AFE ADC offest and DSP ADC offset  ;
    //-------------------------------------------------;
    res.errcode    = eNOERR;
    return(res);
}// end of ReadDSPADCmV

/*-----------------------------------------------------------------------------
 *
 * Module: ReadFilteredDSPADCmV()
 * Description: Read ADC value from DSP registers via FG sampling and screen
 *              out bad data outside window, and measure the average mV of another
 *              ADC
 *
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
#if (CHIP_REV < 0xC0)
SHORT  bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2)], top_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2)];  //Declare arrays here to save use of stack space.
#else
SHORT  bot_data[DISK_NFGPLS ], top_data[DISK_NFGPLS];  //Declare arrays here to save use of stack space.
#endif
ad_dsp_t ReadFilteredDSPADCmV(eRevT Revolutions, BYTE selected_signal, BYTE selected_signal2, BOOL need_fok)
{
    ad_dsp_t res;
    BYTE     i,j,num_ave,fg_num;
    USHORT   avg_bot, avg_top, p2p_win;
    ULONG    AdcAddr = 0;
    ULONG    AdcData = 0;
    ULONG    RecdData = 0;
    ULONG    AdcSamples = 0;

    AdcAddr = HAL_PERIPH_ADC0_ADR + (2 * selected_signal2);
    num_ave = 0;
    switch(Revolutions)
    {
        case NO_REVOLUTION:
        case EIGHTH_REVOLUTION:
        case QUARTER_REVOLUTION:
        case HALF_REVOLUTION:
            Revolutions = ONE_REVOLUTION;//at least one revolution

        default:
            if (Revolutions > 10) Revolutions = 10;//maximum 10 revolutions
#if (CHIP_REV < 0xC0)
            svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions + 1;
#else
            svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions + 1;
#endif
            StartFGTimer();
            break;
    }
#if (CHIP_REV < 0xC0)
    for (i = 0; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
#else
    for (i = 0; i < DISK_NFGPLS; i++)
#endif
    {
        top_data[i] = 0;
        bot_data[i] = 0;
    }

    //=========================================================;
    //               Sampling using FGint method               ;
    //=========================================================;
    WRITE_FIELD(HAL_SRV_PKBOT,0);
#if (CHIP_REV < 0xC0)
    for (i = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions; i > 0; i--)
#else
    for (i = DISK_NFGPLS * Revolutions; i > 0; i--)
#endif
    {
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)//CD track pitch are larger so track cross speed is slower.
        {
            i--;
        }
#if (CHIP_REV < 0xC0)
        if (num_ave >= (DISK_NFGPLS * (DISK_FR2X + 1) / 2))
#else
        if (num_ave >= DISK_NFGPLS)
#endif
        {
            num_ave = 0;
        }

        WRITE_FIELD(HAL_SRV_PKBOT,1);    /* reset Peak/Bottom */

        do
        {
            AdcData += (ULONG)READ_ADR_2B(AdcAddr);
            RecdData += HAL_GLOBAL_GetRfPresent();
            if (((AdcSamples & 0x3FF) == 0) && (need_fok == TRUE) && (!FokStatusOK()))
            {
                res.errcode       = eERR_FOK_ad_dsp_read;
                res.top_saturated = FALSE;
                res.bot_saturated = FALSE;
                res.middle_mv     = 0;
                res.p2p_mv        = 0;
                res.top_mv        = 0;
                res.bot_mv        = 0;
                return (res);
            }
            AdcSamples++;
        }
        while ((svoSpeedVar.fgCount >= i) && (AdcData < 0xFFFF0000));     // wait for next FG
        WRITE_FIELD(HAL_SRV_PKBOT,0);

        //-------------------------------------------------;
        // collect selected bottom value                   ;
        //-------------------------------------------------;
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, DSP_ADC_ADDR[selected_signal][0]);   /* read Bottom data */
        bot_data[num_ave] += (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);

        //-------------------------------------------------;
        // collect selected top value                      ;
        //-------------------------------------------------;
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, DSP_ADC_ADDR[selected_signal][1]);   /* read Peak data */
        top_data[num_ave] += (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);
        num_ave++;
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)//CD track pitch are larger so track cross speed is slower.
        {
            bot_data[num_ave] = bot_data[num_ave - 1];
            top_data[num_ave] = top_data[num_ave - 1];
            num_ave++;
        }
    }   // for loop

    //Convert the ADC digital value to mV unit.
    AdcData = (AdcData >> (GPADC_BITS - 5)) * (GPADC_IN_RANGE >> 5) / AdcSamples + GPADC_IN_MIN;

    //Convert the Recd result to %
    RecdData = RecdData * 100 / AdcSamples;

    //sort measured data.
#if (CHIP_REV < 0xC0)
    for (i = 1; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
#else
    for (i = 1; i < DISK_NFGPLS; i++)
#endif
    {
        for (j = 0; j < i; j++)
        {
            if ((top_data[i] - bot_data[i]) < (top_data[j] - bot_data[j]))
            {
                avg_top = top_data[i];
                avg_bot = bot_data[i];
                top_data[i] = top_data[j];
                bot_data[i] = bot_data[j];
                top_data[j] = avg_top;
                bot_data[j] = avg_bot;
            }
        }
    }

    /*
#if (CHIP_REV < 0xC0)
    for (i = 0; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
#else
    for (i = 0; i < DISK_NFGPLS; i++)
#endif
    {
        send_msgA2(SHOW_DEBUG_MSG,0x090300+i,((top_data[i]-bot_data[i]) * DSPADC_IN_RANGE) >> DSPADC_BITS);
    }*/

    //find consistent datas.
    num_ave = 0;
#if (CHIP_REV < 0xC0)
    for (i = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) - 1; i >= num_ave; i--)
#else
    for (i = DISK_NFGPLS - 1; i >= num_ave; i--)
#endif
    {
        p2p_win = (top_data[i] - bot_data[i]) * 4 / 5;//within 20%
        for (j = num_ave; j <= (i + 1); j++)
        {
            if ((j > i) || ((top_data[i - j] - bot_data[i - j]) < p2p_win))
            {
                if (j > num_ave)
                {
                    num_ave = j;
                    fg_num = i;
                }
                break;
            }
        }
    }

    avg_top = 0;
    avg_bot = 0;
    for (i = fg_num - num_ave + 1; i <= fg_num; i++)
    {
        avg_top += top_data[i];
        avg_bot += bot_data[i];
    }

    //-------------------------------------------------;
    //            calculate averaged value in mV       ;
    //-------------------------------------------------;
    avg_top = (USHORT)(((ULONG)avg_top * DSPADC_IN_RANGE) / ((num_ave * Revolutions) << DSPADC_BITS)) + DSPADC_IN_MIN;
    avg_bot = (USHORT)(((ULONG)avg_bot * DSPADC_IN_RANGE) / ((num_ave * Revolutions) << DSPADC_BITS)) + DSPADC_IN_MIN;

    //-------------------------------------------------;
    //             Saturation mechanism                ;
    //-------------------------------------------------;
    if (avg_top > TOP_SATURATE_LIM)
        res.top_saturated = TRUE;
    else
        res.top_saturated = FALSE;

    if (avg_bot < BOT_SATURATE_LIM)
        res.bot_saturated = TRUE;
    else
        res.bot_saturated = FALSE;

    //-------------------------------------------------;
    // calculate P2P and DC values                     ;
    //-------------------------------------------------;
    res.middle_mv = (avg_top + avg_bot) / 2;
    res.p2p_mv    = avg_top - avg_bot;
    res.errcode   = eNOERR;

    res.top_mv = AdcData;
    res.bot_mv = RecdData;

    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: ReadYerrRevs()
 * Description: Read TEpp value from DSP registers every FG sample and use track count to know the
 *              angles with maximum track crossing freq in and out.
 *
 * Return : unit : mV
 *          top_mv: TEpp with largest tracking crossing freq.
 *          bot_mv: TEpp with largest tracking crossing freq.
 *          middle_mv: BS_bottom
 *          p2p_mv: TEpp sum
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadYerrRevs(eRevT Revolutions)
{
    SHORT    track_init,cur_track;
    ad_dsp_t res;
    BYTE     i,index,orig_toklvl;

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x338510); //****Y-error_MEAS_START!!!****
    orig_toklvl = READ_FIELD(HAL_AFE_TOKLVL);
    #if (CHIP_REV <= 0xB3)
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_75mV);
    #else
    WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_100mV);
    #endif
    res.top_saturated = FALSE;
    res.bot_saturated = FALSE;
#if (CHIP_REV < 0xC0)
    for (index = 0; index < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); index++)
#else
    for (index = 0; index < DISK_NFGPLS; index++)
#endif
    {
        top_data[index] = 0;
        bot_data[index] = 0;
    }
    index = 0;
    res.bot_mv = (1 << DSPADC_BITS);
    res.middle_mv = 0;
    svoRunOut = 0;

    switch(Revolutions)
    {
        case NO_REVOLUTION:
        case EIGHTH_REVOLUTION:
        case QUARTER_REVOLUTION:
        case HALF_REVOLUTION:
        case ONE_REVOLUTION:
            Revolutions = TWO_REVOLUTIONS;//at least two revolutions
        default:
            if (Revolutions > 10) Revolutions = 10;//maximum 10 revolutions
#if (CHIP_REV < 0xC0)
            svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions + 1;
#else
            svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions + 1;
#endif
            StartFGTimer();
            break;
    }
    afeSetTrackingMethod(TRK_METHOD_MPP);
    WRITE_FIELD(HAL_AFE_TRKO,       svoCalOfsT1Var[Layer_Index][TE_OS].land_offset[RECDH]);
    WRITE_FIELD(HAL_AFE_BLANKTRKO,  svoCalOfsT1Var[Layer_Index][TE_OS].land_offset[RECDL]);
    svoVar_EccCheckEnable = 1;
    //=========================================================;
    //               Sampling using FGint method               ;
    //=========================================================;
    WRITE_FIELD(HAL_SRV_PKBOT,0);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);
    track_init = (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);
#if (CHIP_REV < 0xC0)
    for (i = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions; i > 0; i--)
    {
        if (index >= (DISK_NFGPLS * (DISK_FR2X + 1) / 2))
#else
    for (i = DISK_NFGPLS * Revolutions; i > 0; i--)
    {
        if (index >= DISK_NFGPLS)
#endif
        {
            if (svoVar_TrackingMethod == TRK_METHOD_MPP)
            {
                afeSetTrackingMethod(TRK_METHOD_DPP);
                WRITE_FIELD(HAL_AFE_TRKO,       svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[RECDH]);
                WRITE_FIELD(HAL_AFE_BLANKTRKO,  svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[RECDL]);
            }
            index = 0;
            res.middle_mv += res.bot_mv;
            res.bot_mv = (1 << DSPADC_BITS);
        }

        WRITE_FIELD(HAL_SRV_PKBOT,1);    /* reset Peak/Bottom */
        do
        {
            if (!FokStatusOK())  // stop if focus is lost
            {
                WRITE_FIELD(HAL_SRV_PKBOT,0);
                res.errcode       = eERR_FOK_ad_dsp_read;
                res.middle_mv     = 0;
                res.p2p_mv        = 0;
                return(res);
            }
        }while (svoSpeedVar.fgCount >= i);    // wait for next FG
        WRITE_FIELD(HAL_SRV_PKBOT,0);

        if (svoVar_TrackingMethod == TRK_METHOD_MPP)
        {
            //-------------------------------------------------;
            // collect remain tracks value                     ;
            //-------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_JUMP);
            cur_track = (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR) - track_init;
            if (cur_track & 0x1000)
            {
                cur_track |= 0xE000;
            }
            else
            {
                cur_track &= 0x0FFF;
            }
            bot_data[index] += (SHORT)cur_track;
        }
        else
        {
            //-------------------------------------------------;
            // collect TE PEAK value                           ;
            //-------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEPEAK);
            top_data[index] += (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);

            //-------------------------------------------------;
            // collect TE BOTTOM value                         ;
            //-------------------------------------------------;
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEBOTTOM);
            top_data[index] -= (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);
        }

        //measure BS bottom
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_BSBOTTOM);
        res.top_mv = (USHORT)READ_REG(HAL_SRVREG_MAP->SRCR);
        if (res.top_mv < res.bot_mv)
        {
            res.bot_mv = res.top_mv;
        }
        index++;
    }   // for loop
    Revolutions--;
    res.middle_mv += res.bot_mv;

    //find the angle
    res.bot_mv = 0;
    res.p2p_mv = 0;
#if (CHIP_REV < 0xC0)
    res.top_mv = bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) - 2] + bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) - 1] - bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2 - 2] - bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2 - 1];
#else
    res.top_mv = bot_data[DISK_NFGPLS - 2] + bot_data[DISK_NFGPLS - 1] - bot_data[DISK_NFGPLS / 2 - 2] - bot_data[DISK_NFGPLS / 2 - 1];
#endif
#if (CHIP_REV < 0xC0)
    for (i = 0; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
#else
    for (i = 0; i < DISK_NFGPLS; i++)
#endif
    {
        res.top_mv += bot_data[i];
#if (CHIP_REV < 0xC0)
        res.top_mv -=(i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2)? bot_data[i + (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2] : bot_data[i - (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2];
#else
        res.top_mv -=(i < DISK_NFGPLS / 2)? bot_data[i + DISK_NFGPLS / 2] : bot_data[i - DISK_NFGPLS / 2];
#endif
#if (TRO_INVERSION == 1)
        if (res.top_mv >= res.bot_mv)
#else
        if (res.top_mv <= res.bot_mv)
#endif
        {
            res.bot_mv = res.top_mv;
            index = i;
        }
#if (CHIP_REV < 0xC0)
        res.top_mv -= (i < 2)? bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) - 2 + i] : bot_data[i - 2];
        res.top_mv += (i < 2 + (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2)? bot_data[(DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2 - 2 + i] : bot_data[i - (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2 - 2];
#else
        res.top_mv -= (i < 2)? bot_data[DISK_NFGPLS - 2 + i] : bot_data[i - 2];
        res.top_mv += (i < 2 + DISK_NFGPLS / 2)? bot_data[DISK_NFGPLS / 2 - 2 + i] : bot_data[i - DISK_NFGPLS / 2 - 2];
#endif
        res.p2p_mv += top_data[i];
    }
#if (CHIP_REV < 0xC0)
    index = index + ((DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4) - 2;
#else
    index = index + (DISK_NFGPLS / 4) - 2;
#endif

    //get the pp_data
    res.top_mv = 0;
    res.bot_mv = 0;
    for (i = index; i < index + 4; i++)
    {
#if (CHIP_REV < 0xC0)
        res.top_mv += top_data[(i % (DISK_NFGPLS * (DISK_FR2X + 1) / 2))];
        res.bot_mv += top_data[((i + ((DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2)) % (DISK_NFGPLS * (DISK_FR2X + 1) / 2))];
#else
        res.top_mv += top_data[(i % DISK_NFGPLS)];
        res.bot_mv += top_data[((i + (DISK_NFGPLS / 2)) % DISK_NFGPLS)];
#endif
    }

    res.top_mv = res.top_mv * DSPADC_IN_RANGE / ((4 << DSPADC_BITS) * Revolutions);
    res.bot_mv = res.bot_mv * DSPADC_IN_RANGE / ((4 << DSPADC_BITS) * Revolutions);
#if (CHIP_REV < 0xC0)
    res.p2p_mv = res.p2p_mv * DSPADC_IN_RANGE / (((DISK_NFGPLS * (DISK_FR2X + 1) / 2) << DSPADC_BITS) * Revolutions);
#else
    res.p2p_mv = res.p2p_mv * DSPADC_IN_RANGE / ((DISK_NFGPLS << DSPADC_BITS) * Revolutions);
#endif
    res.middle_mv = res.middle_mv * DSPADC_IN_RANGE / (Revolutions << DSPADC_BITS) + DSPADC_IN_MIN;
    res.errcode = eNOERR;

    SendMsgCnB(SHOW_DEBUG_MSG,2,0x310120,B2B((DISTANCE_PER_MACH_REV / (4 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV)) * (svoRunOut * svoSeekVar.parRough) / svoSeekVar.parFine)); //ECC_DISC(um)
    SendMsgCnB(SHOW_DEBUG_MSG,2,0x338511,B2B((res.top_mv - res.bot_mv) * 160 / res.p2p_mv)); //Y-err meas result (< 10)
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,8,0x338512,A2B(res.top_mv),A2B(res.bot_mv),A2B(res.p2p_mv),A2B(res.middle_mv));

#if 0 //for debug
    #if (CHIP_REV < 0xC0)
    for (i = 0; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
    #else
    for (i = 0; i < DISK_NFGPLS; i++)
    #endif
    {
        SendMsgCnB(SHOW_DEBUG_MSG,3,0x338520,B1B(i),B2B(bot_data[i])); //FG[i] Track Count
    }
    #if (CHIP_REV < 0xC0)
    for (i = 0; i < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); i++)
    #else
    for (i = 0; i < DISK_NFGPLS; i++)
    #endif
    {
        SendMsgCnB(SHOW_DEBUG_MSG,3,0x338521,B1B(i),B2B((top_data[i] * DSPADC_IN_RANGE) / (Revolutions << DSPADC_BITS))); //FG[i] FEpp
    }
    SendMsgCn(SHOW_DEBUG_MSG,4,0x338522,A2B(res.top_mv),A2B(res.bot_mv)); // RF_top(mV) RF_bot(mV)
#endif
    WRITE_FIELD(HAL_AFE_TOKLVL, orig_toklvl);

    return(res);
}

#if (OPU_BEAM_LANDING_MEAS == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: readSegmentABCD()
 * Description: read SAN,SBN,SCN,SDN
 *
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
#define MAINBEAM_LANDING
ad_dsp_t readSegmentABCD(eRevT revolutions, BOOL need_fok)
{
    BYTE phase;
    #if (CHIP_REV > 0xB3)
    BYTE orig_mux3;
    #endif
    ad_dsp_t meas_signal,res;

    res.errcode = eNOERR;
    res.top_saturated = FALSE;
    res.bot_saturated = FALSE;
    WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXTOAD3O);
    #if (CHIP_REV <= 0xB3)
        #ifdef MAINBEAM_LANDING
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SAN); //SAN on MUX1AD
    WRITE_FIELD(HAL_AFE_MUX3ADSEL, halAFE_MUX3ADSEL_SBN); //SBN on MUX3AD
        #else
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SEN); //SEN on MUX1AD
    WRITE_FIELD(HAL_AFE_MUX3ADSEL, halAFE_MUX3ADSEL_SFN); //SFN on MUX3AD
        #endif
    #else
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_MUXOUT3); //MUX3 on MUX1AD
    orig_mux3 = READ_FIELD(HAL_AFE_MUX3SEL);
        #ifdef MAINBEAM_LANDING
    WRITE_FIELD(HAL_AFE_MUX3SEL,halAFE_MUX3SEL_SAN);          //SAN on MUX1AD
    WRITE_FIELD(HAL_AFE_MUX3ADSEL, halAFE_MUX3ADSEL_SBN);     //SBN on MUX3AD !!
        #else
    WRITE_FIELD(HAL_AFE_MUX3SEL,halAFE_MUX3SEL_SEN);          //SEN on MUX1AD
    WRITE_FIELD(HAL_AFE_MUX3ADSEL, halAFE_MUX3ADSEL_SFN);     //SFN on MUX3AD !!
        #endif
    #endif
    for (phase = 0; phase < 2; phase++)
    {
        if (phase == 1)
        {
    #if (CHIP_REV <= 0xB3)
        #ifdef MAINBEAM_LANDING
            WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_SCN);
            WRITE_FIELD(HAL_AFE_MUX3ADSEL,halAFE_MUX3ADSEL_SDN);
        #else
            WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_SGN);
            WRITE_FIELD(HAL_AFE_MUX3ADSEL,halAFE_MUX3ADSEL_SHN);
        #endif
    #else
        #ifdef MAINBEAM_LANDING
            WRITE_FIELD(HAL_AFE_MUX3SEL,halAFE_MUX3SEL_SCN);
            WRITE_FIELD(HAL_AFE_MUX3ADSEL,halAFE_MUX3ADSEL_SDN); //SDN on MUX3AD !!
        #else
            WRITE_FIELD(HAL_AFE_MUX3SEL,halAFE_MUX3SEL_SGN);
            WRITE_FIELD(HAL_AFE_MUX3ADSEL,halAFE_MUX3ADSEL_SHN); //SHN on MUX3AD !!
        #endif
    #endif
        }
        WaitTimer(1);
        meas_signal = readPNmV(ADC_MUXOUT1, ADC_MUXOUT3, revolutions, need_fok);
        if (meas_signal.errcode == eNOERR)
        {
            if (phase == 0)
            {
                res.middle_mv = meas_signal.top_mv; // A
                res.p2p_mv    = meas_signal.bot_mv; // B
            }
            else
            {
                res.top_mv    = meas_signal.top_mv; // C
                res.bot_mv    = meas_signal.bot_mv; // D
            }
            res.top_saturated |= meas_signal.top_saturated;
            res.bot_saturated |= meas_signal.bot_saturated;
        }
        else
        {
            res.errcode = meas_signal.errcode;
            res.top_saturated = FALSE;
            res.bot_saturated = FALSE;
            res.middle_mv     = 0;
            res.p2p_mv        = 0;
            res.top_mv        = 0;
            res.bot_mv        = 0;
            break;
        }
    }
    #if (CHIP_REV > 0xB3)
    WRITE_FIELD(HAL_AFE_MUX3SEL,orig_mux3);
    #endif

    return res;
}
#endif

#if (BD_USE_TE_PP_DPD == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: ReadMPPDPDPhase()
 * Description: Only work on written area
 *              top_mv = TEpp (PP+DPD)
 *              bot_mv = TEpp (PP-DPD)
 *              pp_mv = (top_mv + bot_mv) / 2
 *              middle_mv = top_mv - bot_mv
 *              middle_mv > 0:BDR(E) HTL
 *              middle_mv < 0:BDR(E) LTH or BDROM
 *
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadMPPDPDPhase(eRevT Revolutions)
{
    ad_dsp_t res;
    ETrackingMethod orig_te;
    BYTE     orig_dpd_bw,orig_dpd_inv,orig_mppopgn,orig_tepgn;
    LONG     tepp;

    orig_dpd_bw = READ_FIELD(HAL_AFE_DPDCPFC);
#if (CHIP_REV >= 0xC1)
    WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW3_400kHz);
#else
    WRITE_FIELD(HAL_AFE_DPDCPFC,halAFE_DPDCPFBW_400kHz);
#endif
    orig_te = svoVar_TrackingMethod;
    afeSetTrackingMethod(TRK_METHOD_DPD);
    orig_tepgn = READ_FIELD(HAL_AFE_TEPGN);
    if (orig_tepgn > 0)
    {
        WRITE_FIELD(HAL_AFE_TEPGN, orig_tepgn - 1);
    }
    orig_mppopgn = READ_FIELD(HAL_AFE_MPPOPGN);
    orig_dpd_inv = READ_REG(HAL_AFEREG_MAP->RF20);
#if (APP_TE_SUPPORT == 1)
    if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
        ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)) &&
        ((READ_REG(HAL_AFEREG_MAP->SB14) & (HAL_AFE_SPP1E|HAL_AFE_SPP1F|HAL_AFE_SPP1G|HAL_AFE_SPP1H)) == (HAL_AFE_SPP1E|HAL_AFE_SPP1F)))
    {
        READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TESP1_MASK, HAL_AFE_TEMPP|HAL_AFE_TESP1);
    }
    else
#endif
    {
        WRITE_FIELD(HAL_AFE_TEMPP, TRUE);
        if (orig_mppopgn < 3)
        {
            WRITE_FIELD(HAL_AFE_MPPOPGN,orig_mppopgn + 1);
        }
    }
    res = ReadGPADCmV_Avg(Revolutions, ADC_TE, TRUE);

    if (res.errcode == eNOERR)
    {
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338519,A2B(res.p2p_mv));//TEpp(PP+DPD) = %4u (mV)
        tepp = res.p2p_mv;
        READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, (HAL_AFE_DPD12INV_MASK|HAL_AFE_DPD34INV_MASK), ~orig_dpd_inv);
        res = ReadGPADCmV_Avg(Revolutions, ADC_TE, TRUE);

        if (res.errcode == eNOERR)
        {
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x33851A,A2B(res.p2p_mv));//TEpp(PP-DPD) = %4u (mV)
            res.top_mv = tepp;
            res.bot_mv = res.p2p_mv;
            res.middle_mv = res.top_mv - res.bot_mv;
            res.p2p_mv = (res.top_mv + res.bot_mv) / 2;
        }
    }

    WRITE_FIELD(HAL_AFE_MPPOPGN,orig_mppopgn);
    WRITE_FIELD(HAL_AFE_DPDCPFC,orig_dpd_bw);
    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TESP1_MASK, 0);
    afeSetTrackingMethod(orig_te);
    WRITE_FIELD(HAL_AFE_TEPGN, orig_tepgn);
    WRITE_REG(HAL_AFEREG_MAP->RF20,orig_dpd_inv);

    return(res);
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: readADCmVPerRevs()
 * Description: Read RRF/Wbl peak and bottom hold from DSP registers for revolutions
 *
 * input revolutions -> indicates nr of revolutions over which the average RRFmV/WblmV
 *                      is to be calculated
 *       inFocus     -> System must be in focus during calculation or not!
 *
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
ad_dsp_t readADCmVPerRevs(eRevT Revolutions, BOOL InFocus, ADCSignalT sig)
{
    union
    {
        ULONG v;
        struct
        {
            SBYTE MINPKPD;
            SBYTE MAXPKPD;
            BYTE  AMPLVLPD;
            SBYTE OFFLVLPD;
        } rf;
        struct
        {
            BYTE  WBLAMP  ;
            SBYTE WBLMAXPK ;
            SBYTE WBLMINPK ;
        } wb;
    } Smpl;

    ad_dsp_t   res;
    LONG       ave_off_mv, ave_top_mv, ave_bot_mv, ave_p2p_mv;
    LONG       num_ave;
    SHORT      p2p_win;
#if (CHIP_REV < 0xC0)
    SBYTE      i,fg_counter = (DISK_NFGPLS * (DISK_FR2X + 1) / 2);
#else
    SBYTE      i,fg_counter = DISK_NFGPLS;
#endif
    LONG       fg_pp_sum, fg_off_sum, fg_num;

#if (DVD_RAM_READ == 1)
    LONG       min_bot_mv, max_top_mv, ave_max_top, ave_min_bot;
    SHORT      previous, counter;
#endif

    ave_top_mv = ave_bot_mv = ave_off_mv = ave_p2p_mv = num_ave = 0;

#if (DVD_RAM_READ == 1)
    if (svoVar_discKindT.Bit.Media == eMediaRAM)
    {
        min_bot_mv = 127;
        max_top_mv = -128;
        ave_max_top = ave_min_bot = 0;
        previous = counter = 0;
    }
#endif
    if (Revolutions != NO_REVOLUTION)
    {
        switch(Revolutions)
        {
#if (CHIP_REV < 0xC0)
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
            case ONE_REVOLUTION:
                fg_pp_sum = fg_off_sum = fg_num = 0;
            default:
                svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions;
                break;
#else
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            case ONE_REVOLUTION:
                fg_pp_sum = fg_off_sum = fg_num = 0;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
                break;
#endif
        }
        StartFGTimer();
    }

#if (DVD_RAM_READ == 1)
    if (svoVar_discKindT.Bit.Media == eMediaRAM)
    {
        previous = svoSpeedVar.fgCount;
    }
#endif

    while (((Revolutions != NO_REVOLUTION) && (svoSpeedVar.fgCount) && (num_ave < 0x7FFFFF)) || (num_ave < 256))
    {
        if (sig == RF_ADC)
        {
            Smpl.v = READ_REG(HAL_DFEREG_MAP->HFAMPOFF);
            ave_p2p_mv += (BYTE)Smpl.rf.AMPLVLPD;
            ave_top_mv += (SBYTE)Smpl.rf.MAXPKPD;
            ave_bot_mv += (SBYTE)Smpl.rf.MINPKPD;
            ave_off_mv += (SBYTE)Smpl.rf.OFFLVLPD;
        }
        else if (sig == WBL_ADC)
        {
            Smpl.v = READ_REG(HAL_WBLREG_MAP->WBLAMP);
            ave_p2p_mv += (BYTE)Smpl.wb.WBLAMP;
            ave_top_mv += (SBYTE)Smpl.wb.WBLMAXPK;
            ave_bot_mv += (SBYTE)Smpl.wb.WBLMINPK;
            ave_off_mv += ((Smpl.wb.WBLMINPK + Smpl.wb.WBLMAXPK) / 2);

#if (DVD_RAM_READ == 1)
            if (svoVar_discKindT.Bit.Media == eMediaRAM)
            {
                if (min_bot_mv > (SBYTE)Smpl.wb.WBLMINPK)
                {
                    min_bot_mv = (SBYTE)Smpl.wb.WBLMINPK;
                }
                if (max_top_mv < (SBYTE)Smpl.wb.WBLMAXPK)
                {
                    max_top_mv = (SBYTE)Smpl.wb.WBLMAXPK;
                }

                if (svoSpeedVar.fgCount < (previous-1))
                {
                    previous = svoSpeedVar.fgCount;
                    counter++;
                    ave_max_top += max_top_mv;
                    ave_min_bot += min_bot_mv;
                    min_bot_mv = 127;
                    max_top_mv = -128;
                }
            }
#endif
        }
        num_ave++;

        if ((fg_counter > svoSpeedVar.fgCount) && (Revolutions == ONE_REVOLUTION))
        {
            fg_counter--;
            fg_num = num_ave - fg_num;
            top_data[fg_counter] = ((ave_p2p_mv - fg_pp_sum) / fg_num);
            bot_data[fg_counter] = ((ave_off_mv - fg_off_sum) / fg_num);
            fg_pp_sum = ave_p2p_mv;
            fg_off_sum = ave_off_mv;
            fg_num = num_ave;
        }

        if ((InFocus == TRUE) && (READ_FIELD(HAL_SRV_FOK) == 0))
        {
            res.errcode       = eERR_FOK_ad_dsp_read;
            res.top_saturated = FALSE;
            res.bot_saturated = FALSE;
            res.middle_mv     = 0;
            res.p2p_mv        = 0;
            res.top_mv        = 0;
            res.bot_mv        = 0;
            return res;
        }
    }

    if (Revolutions == ONE_REVOLUTION)
    {
        if (fg_counter > 0)
        {
            fg_num = num_ave - fg_num;
            if (fg_num == 0)
            {
                fg_pp_sum = top_data[fg_counter];
                fg_off_sum = bot_data[fg_counter];
            }
            else
            {
                fg_pp_sum = ((ave_p2p_mv - fg_pp_sum) / fg_num);
                fg_off_sum = ((ave_off_mv - fg_off_sum) / fg_num);
            }
            fg_counter--;
            for (; fg_counter >= 0; fg_counter--)
            {
                top_data[fg_counter] = fg_pp_sum;
                bot_data[fg_counter] = fg_off_sum;
            }
        }

        //sort measured data.
#if (CHIP_REV < 0xC0)
        for (fg_counter = 1; fg_counter < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); fg_counter++)
#else
        for (fg_counter = 1; fg_counter < DISK_NFGPLS; fg_counter++)
#endif
        {
            for (i = 0; i < fg_counter; i++)
            {
                if (top_data[fg_counter] < top_data[i])
                {
                    ave_p2p_mv = top_data[fg_counter];
                    ave_off_mv = bot_data[fg_counter];
                    top_data[fg_counter] = top_data[i];
                    bot_data[fg_counter] = bot_data[i];
                    top_data[i] = ave_p2p_mv;
                    bot_data[i] = ave_off_mv;
                }
            }
        }

        /*
#if (CHIP_REV < 0xC0)
        for (fg_counter = 0; fg_counter < (DISK_NFGPLS * (DISK_FR2X + 1) / 2); fg_counter++)
#else
        for (fg_counter = 0; fg_counter < DISK_NFGPLS; fg_counter++)
#endif
        {
            send_msgA2(1,9,3,fg_counter,top_data[fg_counter]);
        }*/

        //find consistent datas.
        num_ave = 0;
#if (CHIP_REV < 0xC0)
        for (fg_counter = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) - 1; fg_counter >= num_ave; fg_counter--)
#else
        for (fg_counter = DISK_NFGPLS - 1; fg_counter >= num_ave; fg_counter--)
#endif
        {
            p2p_win = top_data[fg_counter] * 4 / 5;//within 20%
            for (i = num_ave; i <= (fg_counter + 1); i++)
            {
                if ((i > fg_counter) || (top_data[fg_counter - i] < p2p_win))
                {
                    if (i > num_ave)
                    {
                        num_ave = i;
                        fg_num = fg_counter;
                    }
                    break;
                }
            }
        }

        fg_pp_sum = 0;
        fg_off_sum = 0;
        for (fg_counter = fg_num - num_ave + 1; fg_counter <= fg_num; fg_counter++)
        {
            fg_pp_sum += top_data[fg_counter];
            fg_off_sum += bot_data[fg_counter];
        }

        ave_p2p_mv = fg_pp_sum * RF_ADC_IN_RANGE / (num_ave << 8);
        ave_off_mv = fg_off_sum * RF_ADC_IN_RANGE / (num_ave << 8);
        ave_top_mv = ave_off_mv + (ave_p2p_mv / 2);
        ave_bot_mv = ave_off_mv - (ave_p2p_mv / 2);
    }
    else
    {
        ave_p2p_mv = ave_p2p_mv * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));
        ave_top_mv = ave_top_mv * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));
        ave_bot_mv = ave_bot_mv * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));
        ave_off_mv = ave_off_mv * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));
    }

#if (DVD_RAM_READ == 1)
    if ((sig == WBL_ADC) && (svoVar_discKindT.Bit.Media == eMediaRAM))
    {
        if ((max_top_mv != -128) && (min_bot_mv != 127))
        {
            counter++;
            ave_max_top += max_top_mv;
            ave_min_bot += min_bot_mv;
        }
        ave_max_top = ave_max_top * RF_ADC_IN_RANGE / (counter << 8);
        ave_min_bot = ave_min_bot * RF_ADC_IN_RANGE / (counter << 8);
    }
#endif

    res.top_saturated = (ave_top_mv > (RF_ADC_IN_TOP - 50));
    res.bot_saturated = (ave_bot_mv < (RF_ADC_IN_BOT + 50));

    //-------------------------------------------------;
    // calculate P2P and so-called LP values           ;
    //-------------------------------------------------;
    res.top_mv    = ave_top_mv;
    res.bot_mv    = ave_bot_mv;
    res.middle_mv = ave_off_mv;
#if (DVD_RAM_READ == 1)
    if ((sig == WBL_ADC) && (svoVar_discKindT.Bit.Media == eMediaRAM))
    {
        res.p2p_mv = (ave_max_top - ave_min_bot);
    }
    else
#endif
    {
        res.p2p_mv = ave_p2p_mv;
    }
    res.errcode = eNOERR;
    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: readPNmV()
 * Description: Read Selecte P and N from ADCMUX1 and ADCMUX2 and return (P - N)
 *
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
ad_dsp_t readPNmV(BYTE ch1, BYTE ch2, eRevT Revolutions, BOOL need_fok)
{
    ad_dsp_t res;
    ULONG    sum1,sum2,sum_ct,AdcAddr1,AdcAddr2;

    sum1 = 0;
    sum2 = 0;
    sum_ct = 0;
    AdcAddr1 = HAL_PERIPH_ADC0_ADR + (2 * ch1); // Selected P value
    AdcAddr2 = HAL_PERIPH_ADC0_ADR + (2 * ch2); // Selected N value

    if (Revolutions != NO_REVOLUTION)
    {
        switch(Revolutions)
        {
#if (CHIP_REV < 0xC0)
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
            default:
                svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions;
                break;
#else
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
                break;
#endif
        }
        StartFGTimer();
    }

    do
    {
        sum1 += READ_ADR_2B(AdcAddr1);
        sum2 += READ_ADR_2B(AdcAddr2);
        sum_ct++;
        if ((need_fok == TRUE) && (!FokStatusOK()))
        {
            res.errcode       = eERR_FOK_ad_dsp_read;
            res.top_saturated = FALSE;
            res.bot_saturated = FALSE;
            res.middle_mv     = 0;
            res.p2p_mv        = 0;
            return(res);
        }
    }
    while (((Revolutions != NO_REVOLUTION) && (svoSpeedVar.fgCount > 0) && (sum_ct < 0xFFFFFF)) || (sum_ct < 256));   // wait for next FG

    res.top_mv = sum1 * (GPADC_IN_RANGE >> 6) / (sum_ct << (GPADC_BITS - 6)) + GPADC_IN_MIN;
    res.bot_mv = sum2 * (GPADC_IN_RANGE >> 6) / (sum_ct << (GPADC_BITS - 6)) + GPADC_IN_MIN;

    res.middle_mv = res.top_mv - res.bot_mv;
    res.p2p_mv    = (res.top_mv + res.bot_mv) / 2;

    res.top_saturated = ((res.top_mv > TOP_SATURATE_LIM) || (res.bot_mv < BOT_SATURATE_LIM));
    res.bot_saturated = ((res.top_mv < BOT_SATURATE_LIM) || (res.bot_mv > TOP_SATURATE_LIM));
    res.errcode   = eNOERR;

    return(res);
}

#if (DVDDL_TILT_CAL_PRO == 1)
   #define  DVD_LOW_MODULATION  40
   #define  DVD_RECD_CNT  80
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: readRFppFromAGC_1rev()
 * Description: Read RP pp from DFE register, and consider RF gain to calculate RF pp
 *
 * Return : Average AGC result from Auto-AGC
 *----------------------------------------------------------------------------*/
ad_dsp_t readRFppFromAGC_1rev(BOOL need_fok)
{
    ad_dsp_t    res;
    LONG        rf_pp;
    ULONG       avg_AGC;
    LONG        num_ave;
    LONG        rf_top;
    LONG        avg_AOC;
    LONG        modulation;
    LONG        recd_cnt;
    avg_AOC = recd_cnt = modulation = 0;

    rf_pp = avg_AGC = num_ave = rf_top = 0;
#if (CHIP_REV < 0xC0)
    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2);
#else
    svoSpeedVar.fgCount = DISK_NFGPLS;
#endif
    StartFGTimer();
    do
    {
        rf_pp += READ_FIELD(HAL_DFE_AMPLVLPD);
        avg_AGC += READ_FIELD(HAL_DFE_GDDATGMON);

        rf_top += READ_FIELD(HAL_DFE_MAXPKPD);
        avg_AOC += READ_FIELD(HAL_DFE_ODDATGMON);
        recd_cnt += HAL_GLOBAL_GetRfPresent();

        if ((num_ave & 0xFF) == 0)
        {
            if ((need_fok == TRUE) && (!FokStatusOK()))
            {
                res.errcode       = eERR_FOK_ad_dsp_read;
                res.top_saturated = FALSE;
                res.bot_saturated = FALSE;
                res.middle_mv     = 0;
                res.p2p_mv        = 0;

                return (res);
            }
        }
        num_ave ++;
    }
    while ((svoSpeedVar.fgCount) && (num_ave < 0x7FFFFF));

    rf_pp = rf_pp * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));

    if (!READ_FIELD(HAL_AFE_RFAGCEN))//if AFE controls RF AGC
    {
        avg_AGC = svoRRFVar.step_size[READ_FIELD(HAL_AFE_RFAGCREG)];
    }
    else
    {
        modulation = avg_AGC / num_ave;
        avg_AGC = (ULONG)linear_interpolation((float)modulation, (float)svoRRFVar.step_size[modulation], (float)(modulation + 1), (float)svoRRFVar.step_size[modulation + 1], (float)avg_AGC / num_ave);
    }

    if (!READ_FIELD(HAL_AFE_RFAOCEN))//if AFE controls RF AOC
    {
        avg_AOC = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);
    }
    else
    {
        avg_AOC = RoundDivide(avg_AOC,num_ave);
    }

    rf_top = rf_top * (RF_ADC_IN_RANGE >> 3) / (num_ave << (8 - 3));
    recd_cnt = recd_cnt * 100 /num_ave;

    //borrow num_ave to store RF_dark//20101014_02PL ==start== remove duplicate variables and messages
    num_ave = RoundDivide(avg_AGC * (avg_AOC - svoRRFVar.ofs_ldoff_cur),256);
#if (NEW_RFPOS_CALIB == 1)
    num_ave += svoRRFVar.rfagc_ref;
#endif
    if (rf_top - num_ave)
    {
        modulation = rf_pp * 100 / (rf_top - num_ave);
    }
    else
    {
        modulation = 60;
    }

    //Less AGC result Larger RF signal
    res.p2p_mv = svoRRFVar.step_size[svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain] * rf_pp / avg_AGC;
    res.middle_mv = modulation;
    res.errcode = eNOERR;

#if (DVDDL_TILT_CAL_PRO == 1)
    if ((modulation <= DVD_LOW_MODULATION) && ( recd_cnt < DVD_RECD_CNT)
        && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers != eMediaROM))
    {
        res.errcode = eLIMIT_blank_calibrate_TILT;
    }
    else
#endif
    if (((svoRRFVar.modulation[Layer_Index] > 0) && (modulation < (svoRRFVar.modulation[Layer_Index]/2))) ||
        ((svoRRFVar.modulation[Layer_Index] == 0) && (modulation < (COEF_GetValue(DFE_CALC_M)/2))))
    {
        // RF: PP = %4d (mV), Top = %4d (mV), DARK = %5d (mV), Modulation = %2d (%%)
        SendMsgCn(SHOW_DEBUG_MSG,7,0x316693,A2B(res.p2p_mv),A2B(rf_top),
            A2B(num_ave),modulation);

        res.errcode = eLIMIT_blank_calibrate_TILT;
    }

    res.top_saturated = FALSE;
    res.bot_saturated = FALSE;

    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: ReadAvgJitterPerRevs()
 * Description: Read average jitter value over revolutions
 *
 * input revolutions -> indicates nr of revolutions over which the average jitter
 *                      is to be calculated
 *
 * Return : unit : 0.1%
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadAvgJitterPerRevs(eRevT revolutions)
{
    ad_dsp_t res;
    ULONG resultLand, resultGroove, temp;
    ULONG nrLands, nrGrooves, nrPllNg;

    res.top_saturated = FALSE;
    res.bot_saturated = FALSE;
    res.middle_mv     = 0;
    res.p2p_mv        = 0;
    resultLand = resultGroove = 0;
    nrLands = nrGrooves = nrPllNg = 0;

    if (revolutions != NO_REVOLUTION)
    {
        switch(revolutions)
        {
#if (CHIP_REV < 0xC0)
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
            default:
                svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * revolutions;
                break;
#else
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * revolutions;
                break;
#endif
        }
        StartFGTimer();
    }

    do
    {
        if (!FokStatusOK())
        {
            res.errcode = eERR_FOK_ad_dsp_read;
            return(res);
        }
        if (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)
        {
            res.errcode = eERR_Trk_skating;
            return(res);
        }

        if (HAL_DFE_GetPllInLock())
        {
            temp = (ULONG)READ_FIELD(HAL_DFE_FPLLJIT);
            if (temp != 0)
            {
#if (DVD_RAM_READ == 1)
                if ((DVDRAM) && (READ_FIELD(HAL_DVDD_LAND)))
                {
                    /* Land */
                    nrLands++;
                    resultLand += temp;
                }
                else
#endif
                {
                    nrGrooves++;
                    resultGroove += temp;
                }
            }
        }
        else
        {
            nrPllNg++;
        }
    }
    while ((revolutions != NO_REVOLUTION) && svoSpeedVar.fgCount && (nrGrooves < 0xFFFFFC00));

    if (nrPllNg * 4 <= (nrGrooves + nrLands))
    {
        res.middle_mv = (nrGrooves > 0)? convertRTJitter(resultGroove * 8 / nrGrooves): 0x7FFF;
#if (DVD_RAM_READ == 1)
        res.p2p_mv = (nrLands > 0)? convertRTJitter(resultLand * 8 / nrLands): 0x7FFF;
#endif
        res.errcode = eNOERR;
    }
    else
    {
        res.errcode = eERR_RF_not_present;
    }

    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: ReadGPADCmV_Avg()
 * Description: Read ADC value from GPADC
 *              The p2p_mv is calculated by averaging method, only works for sin wave singal (DPP TE)
                and must be around Vref
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadGPADCmV_Avg(eRevT Revolutions, BYTE channel, BOOL need_fok)
{
    ad_dsp_t res;
    BYTE     tries;
    LONG     adc_data,mid_data;
    ULONG    num_sum,abs_sum,num_ave,adc_addr,recd_sum;

    mid_data = 512;
    adc_addr = HAL_PERIPH_ADC0_ADR + (2 * channel);

    for (tries = 0; tries < 2; tries++)
    {
        num_sum  = 0;
        abs_sum  = 0;
        num_ave  = 0;
        recd_sum = 0;

        switch(Revolutions)
        {
#if (CHIP_REV < 0xC0)
            case NO_REVOLUTION: //treat NO_REVOLUTION as EIGHTH_REVOLUTION
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
            default:
                svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions;
                break;
#else
            case NO_REVOLUTION: //treat NO_REVOLUTION as EIGHTH_REVOLUTION
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
                break;
#endif
        }

        if (svoSpeedVar.fgCount > 0)
        {
            StartFGTimer();
        }

        do
        {
            adc_data = READ_ADR_2B(adc_addr);
            num_sum += adc_data;
            abs_sum += ABS(adc_data-mid_data);  // Read of data from requested channel
            num_ave++;
            if ((num_ave & 0xFF) == 1)
            {
                if ((need_fok == TRUE) && (!FokStatusOK()))
            {
                res.errcode       = eERR_FOK_ad_dsp_read;
                res.top_saturated = FALSE;
                res.bot_saturated = FALSE;
                res.middle_mv     = 0;
                res.p2p_mv        = 0;
                return(res);
            }
                recd_sum += HAL_GLOBAL_GetRfPresent();
        }
        }
        while ((svoSpeedVar.fgCount) && (num_sum < 0xFFFFFC00));

        //for sin wave only,
        //variance < +-5%. (For DVDRAM rewritable area the traditional P2P measurement variance is about +-9% due to PID)
        res.p2p_mv = abs_sum * (GPADC_IN_RANGE/8) / (num_ave * 40);    //theoretical 40.75
        res.middle_mv = num_sum * (GPADC_IN_RANGE >> 5) / (num_ave << (GPADC_BITS - 5)) + GPADC_IN_MIN;
        adc_data = ABS(res.middle_mv - ((mid_data * GPADC_IN_RANGE) >> GPADC_BITS) - GPADC_IN_MIN);
        if (adc_data <= res.p2p_mv / 4)
        {
            break;
        }
        else
        {
            adc_data = res.p2p_mv / 4;
        }
        mid_data = num_sum / num_ave;
    }
    res.p2p_mv = (res.p2p_mv + sqrt(res.p2p_mv * res.p2p_mv - 8 * adc_data * adc_data)) / 2;
    res.top_mv = res.middle_mv + (res.p2p_mv / 2);
    res.bot_mv = res.top_mv - res.p2p_mv;
    res.top_saturated = (res.top_mv > TOP_SATURATE_LIM);
    res.bot_saturated = (res.bot_mv < BOT_SATURATE_LIM);

    //output RECD (%) via bot_mv
    res.bot_mv = recd_sum * 100 / ((num_ave - 1) >> 8);

    res.errcode   = eNOERR;
    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: ReadDSP_CE_mV_Avg()
 * Description: Read value from DSP memory map (RFILE or RGLUE) (CE control must be enabled)
 *              The p2p_mv is calculated by averaging method, only works for sin wave singal (DPP TE)
                and must be around Vref
 * Return : unit : mV
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadDSP_CE_mV_Avg(eRevT Revolutions, BOOL need_fok)
{
    ad_dsp_t res;
    BYTE     tries;
    LONG     adc_data,mid_data,num_sum,num_ave,gain_factor;
    ULONG    abs_sum,recd_sum;

    mid_data = 0;
    HAL_DSP_SetDebug(DEBUG_CH0, DEBUG_SOURCE_RGLUE, 0x3A, 0x08); //CEDC2
    gain_factor = READ_DSP_COEF(kc22);
    if (READ_FIELD(HAL_SRV_TGSW) == 1)
    {
        gain_factor = (gain_factor + 16) / 32;
    }
    else
    {
        gain_factor = (gain_factor + 32) / 64;
    }

    for (tries = 0; tries < 2; tries++)
    {
        num_sum  = 0;
        abs_sum  = 0;
        num_ave  = 0;
        recd_sum = 0;

        switch (Revolutions)
        {
            case NO_REVOLUTION: //treat NO_REVOLUTION as EIGHTH_REVOLUTION
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
                break;
        }

        if (svoSpeedVar.fgCount > 0)
        {
            StartFGTimer();
        }


        do
        {
            adc_data = READ_DSP_COEF(debug0) * gain_factor / 0x8000;

            num_sum += adc_data;
            abs_sum += ABS(adc_data-mid_data);  // Read of data from requested channel
            num_ave++;
            if ((num_ave & 0xFF) == 1)
            {
                if ((need_fok == TRUE) && (!FokStatusOK()))
                {
                    res.errcode       = eERR_FOK_ad_dsp_read;
                    res.top_saturated = FALSE;
                    res.bot_saturated = FALSE;
                    res.middle_mv     = 0;
                    res.p2p_mv        = 0;
                    return (res);
                }
                recd_sum += HAL_GLOBAL_GetRfPresent();
            }
        }
        while ((svoSpeedVar.fgCount) && (num_ave < 0x7FFFFF));

        //for sin wave only,
        //variance < +-5%. (For DVDRAM rewritable area the traditional P2P measurement variance is about +-9% due to PID)
        res.p2p_mv = abs_sum * (GPADC_IN_RANGE / 8) / (num_ave * 40);    //theoretical 40.75
        res.middle_mv = num_sum * (GPADC_IN_RANGE >> 5) / (num_ave << (GPADC_BITS - 5));
        adc_data = ABS(res.middle_mv - ((mid_data * GPADC_IN_RANGE) >> GPADC_BITS));
        if (adc_data <= res.p2p_mv / 4)
        {
            break;
        }
        else
        {
            adc_data = res.p2p_mv / 4;
        }
        mid_data = num_sum / num_ave;
    }
    res.p2p_mv = (res.p2p_mv + sqrt(res.p2p_mv * res.p2p_mv - 8 * adc_data * adc_data)) / 2;
    res.middle_mv += CONVERT_DSPOFS_TO_ADCMV(svoCalVar.ceOfsData.all16);
    res.top_mv = res.middle_mv + (res.p2p_mv / 2);
    res.bot_mv = res.top_mv - res.p2p_mv;
    res.top_saturated = (res.top_mv > TOP_SATURATE_LIM);
    res.bot_saturated = (res.bot_mv < BOT_SATURATE_LIM);

    //output RECD (%) via bot_mv
    res.bot_mv = recd_sum * 100 / ((num_ave - 1) >> 8);

    res.errcode = eNOERR;

    return (res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: ReadGPADCmV()
 * Description: Read and return RAW ADC mV
 *
 *----------------------------------------------------------------------------*/
ad_dsp_t ReadGPADCmV(eRevT Revolutions, BYTE selected_signal, BOOL need_fok)
{
    ad_dsp_t res;
    SBYTE    i;
    USHORT   AdcCount,min,max,min_max_sum_ct;
    ULONG    sum,sum_ct,AdcAddr,min_sum,max_sum;

    if (Revolutions != NO_REVOLUTION)
    {
        min_sum = 0;
        max_sum = 0;
        min_max_sum_ct = 0;
        sum = 0;
        sum_ct = 0;
        AdcAddr = HAL_PERIPH_ADC0_ADR + (2 * selected_signal);

        switch(Revolutions)
        {
#if (CHIP_REV < 0xC0)
            case NO_REVOLUTION:  //treat NO_REVOLUTION as EIGHTH_REVOLUTION
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) / 2; break;
            default:
                svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2) * Revolutions;
                break;
#else
            case NO_REVOLUTION:  //treat NO_REVOLUTION as EIGHTH_REVOLUTION
            case EIGHTH_REVOLUTION:  svoSpeedVar.fgCount = DISK_NFGPLS / 8; break;
            case QUARTER_REVOLUTION: svoSpeedVar.fgCount = DISK_NFGPLS / 4; break;
            case HALF_REVOLUTION:    svoSpeedVar.fgCount = DISK_NFGPLS / 2; break;
            default:
                svoSpeedVar.fgCount = DISK_NFGPLS * Revolutions;
                break;
#endif
        }
        StartFGTimer();
        i = svoSpeedVar.fgCount - 1;
        for (; i > 0; i--)
        {
            max = 0;
            min = 65535;
            do
            {
                AdcCount = READ_ADR_2B(AdcAddr);
                sum += AdcCount;
                if (AdcCount > max)
                {
                    max = AdcCount;
                }
                if (AdcCount < min)
                {
                    min = AdcCount;
                }
                if (((sum_ct & 0x3FF) == 0) && (need_fok == TRUE) && (!FokStatusOK()))
            {
                res.errcode       = eERR_FOK_ad_dsp_read;
                res.top_saturated = FALSE;
                res.bot_saturated = FALSE;
                res.middle_mv     = 0;
                res.p2p_mv        = 0;
                return(res);
            }
                sum_ct++;
            }
            while ((svoSpeedVar.fgCount >= i) && (sum < 0xFFFFFC00));   // wait for next FG

            min_sum += min;
            max_sum += max;
            min_max_sum_ct++;
            if (svoVar_discKindT.Bit.DiscStd == eDiscCD)//CD track pitch are larger so track cross speed is slower.
            {
                i--;
            }
        }

        res.middle_mv = sum * (GPADC_IN_RANGE >> 6) / (sum_ct << (GPADC_BITS - 6)) + GPADC_IN_MIN;
        res.p2p_mv    = ((max_sum - min_sum) * GPADC_IN_RANGE) / (min_max_sum_ct << GPADC_BITS);
    }
    else
    {
        res.middle_mv = ReadAverageAdcmV(selected_signal, 7);
        res.p2p_mv    = 0;
    }
    res.top_mv = res.middle_mv + (res.p2p_mv / 2);
    res.bot_mv = res.top_mv - res.p2p_mv;
    res.top_saturated = (res.top_mv > TOP_SATURATE_LIM);
    res.bot_saturated = (res.bot_mv < BOT_SATURATE_LIM);

    res.errcode    = eNOERR;

    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: RampMeas()
 * Description: measure FEamp, BSamp, SRFOamp during ramping
 *
 *----------------------------------------------------------------------------*/
//#define FEPP_DEBUG_MSG //send msg for all detected rising and falling s-curve during ramp

void RampMeas(eRampMeasType type, USHORT duration, BYTE direction, discKindT DiscKind)
{
    ULONG   temp_timer,cur_time;
    SHORT   feo_cur,feo_thr,temp_pkbt,srfo_cur,srfo_prev,srfo_peak,subst_thr,fe_peak,fe_bottom;
#if (FE_SLEW_RATE_LIMITER == 1)
    SHORT   feo_prev;
    SHORT   SlewRateShift;
#endif
    USHORT  tempFEpp,SubstTime,LayerTime,tempBSpp,BSbottom,BSpeak,temp_dur,min_FEpp;
#if (FILTER_POINT > 1)
    SHORT   feo_data[FILTER_POINT],feo_sum;
    BYTE    data_cnt;
#else
    SHORT   feo_ref;
#endif
    BYTE    stage,feo_inv,orig_FELpfBW,ratio_to_maxfepp;

#ifdef FEPP_DEBUG_MSG
    SBYTE   fepp_debug_fe[10];
    BYTE    fepp_debug_index;
#endif

    StartTimer(&temp_timer);

#ifdef FEPP_DEBUG_MSG
    for (fepp_debug_index = 0; fepp_debug_index < 10; fepp_debug_index++)
    {
        fepp_debug_fe[fepp_debug_index] = 0;
    }
    fepp_debug_index = 0;
#endif

    svoRampMeasVar.FEpp[0] = 0;
    svoRampMeasVar.BSpp[0] = 0;
    svoRampMeasVar.SRFOpp[0] = 0;
    svoRampMeasVar.LayerTime[0] = 0;
    svoRampMeasVar.FEpp[1] = 0;
    svoRampMeasVar.BSpp[1] = 0;
    svoRampMeasVar.SRFOpp[1] = 0;
    svoRampMeasVar.LayerTime[1] = 0;
    svoRampMeasVar.saturated = FALSE;
    srfo_prev = 0;
    srfo_cur  = 0;
    BSbottom  = 0;
    BSpeak = 0;
    LayerTime = 0;

#if (BD_ENABLE == 1)
    min_FEpp = svoPrvtVar_maxFEpp[((DiscKind.Bit.DiscStd == eDiscBD) ^ (direction == DOWN))] / 2;
#else
    min_FEpp = svoPrvtVar_maxFEpp[(direction == DOWN)] / 2;
#endif

#if (FILTER_POINT > 1)
    for (data_cnt = 0; data_cnt < FILTER_POINT; data_cnt++)
    {
        feo_data[data_cnt] = svoRampMeasVar.refFEO;
    }

    feo_sum = 0;
    data_cnt = 0;
#else
    feo_ref = svoRampMeasVar.refFEO;//use local variable to replace global variable can increase the sample rate 6%.
#endif

#if (FE_SLEW_RATE_LIMITER == 1)
    feo_prev = 0;
    #if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        if (cal_retry <= 1)
            SlewRateShift = FE_MAX_SLEW_RATE_RAM;
        else
            SlewRateShift = (cal_retry-1)*(FE_MAX_SLEW_RATE_RAM >>1) + FE_MAX_SLEW_RATE_RAM;
    }
    else
    #endif
    {
        SlewRateShift = FE_MAX_SLEW_RATE;
    }
#endif

    temp_pkbt = 0;
    tempFEpp = 0;
    fe_bottom = 0;
    SubstTime = 0;
    srfo_peak = 0;

    svoRampMeasVar.LayerFoo = svoCalMisc2Var.fooos;

    // Change fc of FE LPF to 15kHz to suppress header noise.
    orig_FELpfBW = HAL_AFE_GetFELpfBW();
    HAL_AFE_SetFELpfBW(e_FELPF_15);

    // start measure BS bottom from servo 8bit peak bottom hold
    WRITE_FIELD(HAL_SRV_PKBOT,0);
    WRITE_FIELD(HAL_SRV_PKBOT,1);

    //set sample signal
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
#if (FE_SLEW_RATE_LIMITER == 1)
    if (DiscKind.Bit.DiscStd == eDiscDVD)
    {
        WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FE);

    #if (FOO_INVERSION == 0)
        feo_inv = (direction == DOWN);
    #else
        feo_inv = (direction == UP);
    #endif
    }
    else
#endif
    {
        WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FEO);

#if (FOO_INVERSION == 0)
        feo_inv = READ_FIELD(HAL_AFE_FEINV)? (direction == UP) : (direction == DOWN);
#else
        feo_inv = READ_FIELD(HAL_AFE_FEINV)? (direction == DOWN) : (direction == UP);
#endif
    }

    ratio_to_maxfepp = RATIO_TO_MAXFEPP;

    switch (type)
    {
        case eRampSTM:
            svoRampMeasVar.SubstFoo = svoRampMeasVar.LayerFoo;
            svoRampMeasVar.SubstSRFO = 0;
            feo_thr = COEF_GetValue(AFE_CALC_STM_FE_TH) * FILTER_POINT;
#if (BD_ENABLE == 1)
            if (DiscKind.Bit.DiscStd == eDiscBD)
            {
                feo_thr = feo_thr * 2048 / (svoCalDlVar[!Layer_Index].fe_factor + 1024);
            }
#endif

            subst_thr = COEF_GetValue(AFE_CALC_STM_PRE_TH);

            if (svoStmSubMissRty >= 1)//from 720
            {
                if (DiscKind.Bit.DiscStd == eDiscCD)
                {
                    subst_thr -= (svoStmSubMissRty * 2);
                    if (subst_thr < 1)
                    {
                        subst_thr = 1;
                    }
                }
                else
                {
                    subst_thr -= svoStmSubMissRty;
                }
            }
            //from 720
            if ((cal_retry == STM_MAX_RETRIES) && (DiscKind.Bit.DiscStd == eDiscDVD))
            {   //the most possible of failure reason is SRFO has smaller peak on substrate
                //new subst_thr should not be triggered by noise
                subst_thr = 5;
            }
            stage = 0xF0;
            break;

        case eRampFEpp:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                feo_thr = FE_SMSTEP_TH_RAM * FILTER_POINT;
                if (cal_retry > 2)
                {
                    BYTE  i,j;
                    SHORT k;

                    j = cal_retry-2;
                    k = FE_SMSTEP_TH * FILTER_POINT;

                    for (i = 1; i <= j; i++)
                    {
                        feo_thr = (feo_thr>>1)+(feo_thr>>2)+(feo_thr>>3);   // feo_thr*0.875
                    }
                    if (feo_thr<k)
                        feo_thr=k;
                }
            }
            else
#endif
            {
                if(DiscKind.Bit.DiscStd == eDiscBD)
                {
                    if(DiscKind.Bit.Layers == eLayersDouble)
                    {
                        feo_thr = FE_SMSTEP_TH_BDDL * FILTER_POINT;
                    }
                    else
                    {
                        feo_thr = FE_SMSTEP_TH_BD * FILTER_POINT;
                    }
                }
                else if (DiscKind.Bit.DiscStd == eDiscDVD)
                {
                    feo_thr = FE_SMSTEP_TH * FILTER_POINT;
                }
                else
                {
                    feo_thr = FE_SMSTEP_TH_CD * FILTER_POINT;
                }
            }
#if (FE_SLEW_RATE_LIMITER == 1)
            if (DiscKind.Bit.DiscStd != eDiscDVD)
#endif
            {
                switch (READ_FIELD(HAL_AFE_FEPGN))
                {
                    case halAFE_FEPGN_N6dB:
                        feo_thr *= 2;
                        break;

                    case halAFE_FEPGN_0dB:
                        break;

                    case halAFE_FEPGN_6dB:
                        feo_thr /= 2;
                        break;

                    case halAFE_FEPGN_12dB:
                        feo_thr /= 4;
                        break;
                }

                if (READ_FIELD(HAL_AFE_FESP2) || READ_FIELD(HAL_AFE_FESP2N))
                {
                    feo_thr /= 2;
                }
#if (FE_SLEW_RATE_LIMITER == 2)//from720
                if ((DiscKind.Bit.DiscStd == eDiscDVD) && (DiscKind.Bit.Layers == eLayersSingle))
                    WRITE_REG(HAL_SRVREG_MAP->SLEWRATE, SlewRateShift.all32);
#endif
            }
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
            if (DiscKind.Bit.DiscStd == eDiscBD)//when colli set to L0, the FEO amplitude of L1 will become half.
            {
                feo_thr = feo_thr * 1024 / svoCalDlVar[!Layer_Index].fe_factor;
                ratio_to_maxfepp = RATIO_TO_MAXFEPP * svoCalDlVar[!Layer_Index].fe_factor / 1024;
            }
#endif
            stage = 0;
            break;

        case eRampFEBG:
            temp_pkbt = 0;
            //set subst_thr = 1/4 * target_SRFO
            subst_thr = (COEF_GetValue(AFE_CALC_MbTargetX100) * (10 << GPADC_BITS) / GPADC_IN_RANGE);
            subst_thr *= CalculateRateFromXdB(ONEdB,READ_FIELD(HAL_AFE_SRFOBGA) - 16);
            subst_thr += svoRampMeasVar.refSRFO;
#if (FE_SLEW_RATE_LIMITER == 2)
            if ((DiscKind.Bit.DiscStd == eDiscDVD) && (DiscKind.Bit.Layers == eLayersSingle))
                WRITE_REG(HAL_SRVREG_MAP->SLEWRATE, SlewRateShift.all32);
#endif
            stage = 0xD0;
            break;
    }

    fe_peak = -32768;
    fe_bottom = 32767;
    cur_time = 0;
    temp_dur = duration;

    do
    {
        srfo_prev = srfo_cur;
        srfo_cur = READ_ADR_2B(HAL_PERIPH_ADC0_ADR + 2 * ADC_MUXOUT1); // SRFO (10bit muxADC)
        srfo_cur = (srfo_prev + srfo_cur)/2;
        if (srfo_cur > srfo_peak)
        {
            srfo_peak = srfo_cur;
        }

        feo_cur = READ_ADR_2B(HAL_PERIPH_ADC0_ADR + 2 * ADC_MUXOUT2);  // FEO (10bit muxADC)

#if (FILTER_POINT == 1)
        feo_cur -= feo_ref;
#else
        //FE noise filter
        feo_sum -= feo_data[data_cnt];
        feo_sum += feo_cur;
        feo_data[data_cnt] = feo_cur;
        data_cnt++;
        if (data_cnt == FILTER_POINT)
        {
            data_cnt = 0;
        }
        feo_cur = feo_sum;
#endif

#if (FE_SLEW_RATE_LIMITER == 1)
        if (DiscKind.Bit.DiscStd == eDiscDVD)
        {
    #ifdef DBG_FEP2P_USE_SLEWRATE
            if ((DebugFEOCnt < 200) && (stage != 0))
            {
                DebugFEOStage[DebugFEOCnt] = stage;
                DebugFEORaw[DebugFEOCnt] = feo_cur;
            }
    #endif
            if (feo_cur > (feo_prev + SlewRateShift))
            {
                feo_cur = feo_prev + SlewRateShift;
            }
            if (feo_cur < (feo_prev - SlewRateShift))
            {
                feo_cur = feo_prev - SlewRateShift;
            }
            feo_prev = feo_cur;
    #ifdef DBG_FEP2P_USE_SLEWRATE
            if ((DebugFEOCnt < 200)&&(stage != 0))
            {
                DebugFEOSlew[DebugFEOCnt] = feo_cur;
                DebugFEOCnt++;
            }
#endif
        }
#endif // #if (FE_SLEW_RATE_LIMITER == 1)

        if (feo_inv)
        {
            feo_cur = -feo_cur;
        }

        switch(stage)
        {
            case 0xD0://Threshold is added to ref (10bit ADC value)
                fe_peak = (fe_peak < feo_cur)? feo_cur : fe_peak;
                fe_bottom = (fe_bottom > feo_cur)? feo_cur : fe_bottom;
                if ((srfo_cur < subst_thr) && (srfo_peak > subst_thr) && ((srfo_peak - srfo_cur) > (subst_thr - svoRampMeasVar.refSRFO)))
                {
                    temp_dur = cur_time + F_PP_DELAY_T;

                    if (temp_dur > duration)
                    {
                        temp_dur = duration;
                    }

                    if (srfo_peak > temp_pkbt)
                    {
                        temp_pkbt = srfo_peak;
                        svoRampMeasVar.LayerFoo = HAL_SERVO_GetFooDac();
                    }
                    srfo_peak = 0;
                }
                break;

            case 0xF0://waiting for substrate  //use servo 8bit peak bottom hold, let BS is only made from SRFO
                WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSBOTTOM);
                BSbottom = READ_REG(HAL_SRVREG_MAP->SRCR);
                WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                BSpeak = READ_REG(HAL_SRVREG_MAP->SRCR);

                if (BSpeak > BSbottom + subst_thr)
                {
                    subst_thr = BSpeak - BSbottom;
                    SubstTime = cur_time;
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FOO);   //select FOO_DAC
                    temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);//temp store sub_foo_dac
                    stage = 0xF1;
                }
                break;

            case 0xF1://waiting for SRFO peak of substrate (8bit peak hold)
                WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                BSpeak = READ_REG(HAL_SRVREG_MAP->SRCR);
                if ((BSpeak - BSbottom) > subst_thr)//SRFO keep increase
                {
                    subst_thr = BSpeak - BSbottom;//subst_thr will be the substrate SRFO_PP
                    SubstTime = cur_time;
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FOO);   //select FOO_DAC
                    temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);
                }
                else if (cur_time - SubstTime > 1)//SRFO change to decrease, got substrate layer position
                {
                    temp_pkbt -= 512;
#if (FOOTILT_DIFFERENTIAL == 1)
                    temp_pkbt -= CONVERT_TILTDAT_TO_FOO(READ_FIELD(HAL_SRV_TILTDAT));
#endif
#if (FOO_INVERSION == 0)
                    svoRampMeasVar.SubstFoo = temp_pkbt;
#else
                    svoRampMeasVar.SubstFoo = -temp_pkbt;
#endif
                    svoRampMeasVar.LayerFoo = svoRampMeasVar.SubstFoo;
                    svoRampMeasVar.SubstSRFO = subst_thr * 4;//transfer 8bit SRFO to 10bit ADC DAC
                    srfo_peak = 0;
                    stage = 0;

                    if (DiscKind.Bit.DiscStd == eDiscBD)
                    {
                        temp_dur = cur_time + F_PP_DELAY_T;
                        if (temp_dur > duration)
                        {
                            temp_dur = duration;
                        }
                    }
                }
                break;

            case 6://waiting for S-curve falling or rising start (without BS meas) (feo_cur-->10bit ADC, BS-->8bit peak hold))
            case 3://waiting for S-curve falling or rising start (with BS meas) (feo_cur-->10bit ADC, BS-->8bit peak hold)
                if (feo_cur < -feo_thr) //Threshold is decreased to ref
                {
                    if (stage == 3)
                    {
                        //store BS peak (actually BS is forced to be made only from SRFO in STM) (8bit)
                        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSBOTTOM);
                        tempBSpp = READ_REG(HAL_SRVREG_MAP->SRCR);
                        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                        temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);

                        if ((temp_pkbt > (TOP_SATURATE_GPADC / 4)) && (type == eRampFEpp))
                        {
                            svoRampMeasVar.saturated = TRUE;
                        }
                        tempBSpp += temp_pkbt;
                        tempBSpp = tempBSpp / 2 - BSbottom;
                    }
                    // start measure FEO bottom
                    temp_pkbt = 0;
                    stage = 4;
                    break;
                }

            case 0://waiting for S-curve rising start
                if (feo_cur > feo_thr)
                {
                    if (BSbottom == 0)
                    {
                        //store BS bottom
                        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSBOTTOM);
                        BSbottom = READ_REG(HAL_SRVREG_MAP->SRCR);
                    }

                    if (stage == 6)
                    {
                        // start measure SRFO peak
                        srfo_peak = 0;
                        fe_peak = -32768;
                        fe_bottom = 32767;
                    }
                    // start measure FEO peak
                    temp_pkbt = 0;
                    stage = 1;
                    break;
                }

#if (SACD == 1)
                if ((type == eRampSTM) && (DiscKind.Bit.DiscStd == eDiscDVD)&&(svoRampMeasVar.LayerTime[0]>0)
                    &&((cur_time - (SubstTime+svoRampMeasVar.LayerTime[0]))>(svoRampMeasVar.LayerTime[0]*0.7))
                    && ((srfo_cur -svoRampMeasVar.refSRFO)>(svoRampMeasVar.SRFOpp[0]*0.7)))
                {
                    LayerTime = cur_time - SubstTime;
                    svoRampMeasVar.LayerTime[1] = LayerTime;
                    svoRampMeasVar.SRFOpp[1] = srfo_peak-svoRampMeasVar.refSRFO;
                    if (svoRampMeasVar.SRFOpp[1] > (svoRampMeasVar.SRFOpp[0]*1.2))
                            {
                                temp_dur = 0;
                                break;
                            }
                        }
#endif
                if ((type == eRampSTM) && (svoRampMeasVar.LayerTime[0] == 0))
                {
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                    if (READ_REG(HAL_SRVREG_MAP->SRCR) > BSpeak)
                    {
                        BSpeak = READ_REG(HAL_SRVREG_MAP->SRCR);
                        svoRampMeasVar.LayerFoo = HAL_SERVO_GetFooDac();
                    }
                }
                break;

            case 1://waiting for S-curve rising peak
                if (feo_cur > temp_pkbt)
                {
                    temp_pkbt = feo_cur;//find FEO peak
                }

                if ((temp_pkbt - feo_cur) > feo_thr)
                {
                    //measure FE peak
#if (FE_SLEW_RATE_LIMITER == 1)
                    if (DiscKind.Bit.DiscStd == eDiscDVD)
                    {
    #ifdef FEPP_DEBUG_MSG
                        fepp_debug_fe[fepp_debug_index] = temp_pkbt;
                        fepp_debug_index++;
    #endif
                        temp_pkbt = ((LONG)(temp_pkbt+svoRampMeasVar.refFEO)*GPADC_IN_RANGE)/DSPADC_IN_RANGE/4;//10bit-->8bit
                        if (temp_pkbt > fe_peak)
                        {
                            fe_peak = temp_pkbt;
                        }
                    }
                    else
#endif
                    {
#if (FOO_INVERSION == 0)
                        if (direction == UP)
#else
                        if (direction == DOWN)
#endif
                        {
                            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);
                            temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);//8bit
                            if (temp_pkbt > fe_peak)
                            {
                                fe_peak = temp_pkbt;
                            }
                        }
                        else
                        {
                            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);
                            temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);//8bit
                            if (temp_pkbt < fe_bottom)
                            {
                                fe_bottom = temp_pkbt;
                            }
                        }
#ifdef FEPP_DEBUG_MSG
                    fepp_debug_fe[fepp_debug_index] = temp_pkbt - svoRampMeasVar.refFE;
                    fepp_debug_index++;
#endif
                    }
                    stage = 2;
                }
                break;

            case 2://waiting for S-curve rising end
                if (feo_cur < feo_thr)
                {
                    // start measure BS peak
                    if (type != eRampSTM)
                    {
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                    LayerTime = cur_time - SubstTime;//got layer, store layer timing
                    stage = 3;
                }
                break;

            case 4://waiting for S-curve falling bottom
                if (feo_cur < temp_pkbt)
                {
                    temp_pkbt = feo_cur;    // find FEO bottom
                }
                if ((feo_cur - temp_pkbt) > feo_thr)
                {
                    stage = 5;

                    //measure FE bottom
#if (FE_SLEW_RATE_LIMITER == 1)
                    if (DiscKind.Bit.DiscStd == eDiscDVD)
                    {
#ifdef FEPP_DEBUG_MSG
                        fepp_debug_fe[fepp_debug_index] = temp_pkbt;
                        fepp_debug_index++;
#endif
                        temp_pkbt = ((LONG)(temp_pkbt+svoRampMeasVar.refFEO)*GPADC_IN_RANGE)/DSPADC_IN_RANGE/4;//10bit-->8bit
                        if (temp_pkbt >= fe_bottom)
                        {
                            break;
                        }
                        fe_bottom = temp_pkbt;
                    }
                    else
#endif
                    {
#if (FOO_INVERSION == 0)
                        if (direction == DOWN)
#else
                        if (direction == UP)
#endif
                        {
                            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);
                            temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);
#ifdef FEPP_DEBUG_MSG
                            fepp_debug_fe[fepp_debug_index] = temp_pkbt - svoRampMeasVar.refFE;
                            fepp_debug_index++;
#endif
                            if (temp_pkbt <= fe_peak)
                            {
                                break;
                            }
                            fe_peak = temp_pkbt;
                        }
                        else
                        {
                            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);
                            temp_pkbt = READ_REG(HAL_SRVREG_MAP->SRCR);
#ifdef FEPP_DEBUG_MSG
                            fepp_debug_fe[fepp_debug_index] = temp_pkbt - svoRampMeasVar.refFE;
                            fepp_debug_index++;
#endif
                            if (temp_pkbt >= fe_bottom)
                            {
                                break;
                            }
                            fe_bottom = temp_pkbt;
                        }
                    }
                    //TBD:need check ,720 use 7/8
                    if ((svoRampMeasVar.refFE >= (fe_peak + 7 * fe_bottom) / 8) &&
                        (svoRampMeasVar.refFE <= (7 * fe_peak + fe_bottom) / 8))
                    {
                        if ((((fe_peak > (TOP_SATURATE_GPADC / 4)) || (fe_bottom < (BOT_SATURATE_GPADC / 4))) && (type == eRampFEpp)) || (srfo_peak > TOP_SATURATE_GPADC))
                        {
                            svoRampMeasVar.saturated = TRUE;
                        }
                        tempFEpp = (fe_peak - fe_bottom);
                        srfo_peak -= svoRampMeasVar.refSRFO;

                        if (tempFEpp <= min_FEpp)
                        {
                            if ((tempFEpp * ratio_to_maxfepp) > ((svoPrvtVar_maxFEpp[1] > svoPrvtVar_maxFEpp[0])? svoPrvtVar_maxFEpp[1] : svoPrvtVar_maxFEpp[0]))
                            {
                                if (((svoRampMeasVar.FEpp[0] > 0) && (tempFEpp >= svoRampMeasVar.FEpp[0])) || (svoRampMeasVar.FEpp[0] == 0))
                                {
                                    min_FEpp = tempFEpp - 1;
                                }
                            }
                        }

                        if (tempFEpp > min_FEpp)
                        {
                            if ((svoRampMeasVar.FEpp[1] > 0) && (((svoRampMeasVar.FEpp[0] * ratio_to_maxfepp) <= tempFEpp) ||
                                 ((DiscKind.Bit.DiscStd == eDiscBD) && (tempFEpp > svoRampMeasVar.FEpp[0] / 2) && (LayerTime - svoRampMeasVar.LayerTime[1] < (svoRampMeasVar.LayerTime[1] - svoRampMeasVar.LayerTime[0] + ((direction == UP)? 1 : 0))))))
                            {
                                svoRampMeasVar.FEpp[0] = svoRampMeasVar.FEpp[1];
                                svoRampMeasVar.BSpp[0] = svoRampMeasVar.BSpp[1];
                                svoRampMeasVar.SRFOpp[0] = svoRampMeasVar.SRFOpp[1];
                                svoRampMeasVar.LayerTime[0] = svoRampMeasVar.LayerTime[1];
                                svoRampMeasVar.FEpp[1] = 0;
                            }

                            if (svoRampMeasVar.FEpp[1] == 0)
                            {
                                if (svoRampMeasVar.FEpp[0] > 0)
                                {
#if (SACD == 1)
                                    if (((svoRampMeasVar.FEpp[0] * ratio_to_maxfepp) <= tempFEpp) || (((LayerTime - svoRampMeasVar.LayerTime[0]) > LAYER_DIFF_T) && (type != eRampSTM) && (DiscKind.Bit.DiscStd != eDiscDVD)))
#else
                                    if (((svoRampMeasVar.FEpp[0] * ratio_to_maxfepp) <= tempFEpp) || ((LayerTime - svoRampMeasVar.LayerTime[0]) > LAYER_DIFF_T))
#endif
                                    {
                                        //Previous result is not good
                                        svoRampMeasVar.FEpp[0] = 0;
                                    }
                                    else
                                    {
                                        svoRampMeasVar.FEpp[1] = tempFEpp;
                                        svoRampMeasVar.BSpp[1] = tempBSpp;
                                        svoRampMeasVar.SRFOpp[1] = srfo_peak;
                                        svoRampMeasVar.LayerTime[1] = LayerTime;
                                    }
                                }

                                if (svoRampMeasVar.FEpp[0] == 0)
                                {
                                    svoRampMeasVar.FEpp[0] = tempFEpp;
                                    svoRampMeasVar.BSpp[0] = tempBSpp;
                                    svoRampMeasVar.SRFOpp[0] = srfo_peak;
                                    svoRampMeasVar.LayerTime[0] = LayerTime;
#if (BD_ENABLE == 1)
                                    min_FEpp = svoPrvtVar_maxFEpp[((DiscKind.Bit.DiscStd == eDiscBD) ^ (direction == UP))] / 2;
#else
                                    min_FEpp = svoPrvtVar_maxFEpp[(direction == UP)] / 2;
#endif
                                    if ((DiscKind.Bit.Layers == eLayersSingle) && (svoRampMeasVar.FEpp[0] > min_FEpp))
                                    {
                                        min_FEpp = svoPrvtVar_maxFEpp[0] / ratio_to_maxfepp;
                                    }
                                    if ((min_FEpp * ratio_to_maxfepp) < tempFEpp)
                                    {
                                        min_FEpp = tempFEpp / ratio_to_maxfepp;
                                    }
                                }

                                if (tempFEpp >= svoRampMeasVar.FEpp[0])
                                {
                                    svoRampMeasVar.LayerFoo = HAL_SERVO_GetFooDac();
                                }
                            }
#if (SACD == 1)
                            if ((type == eRampSTM) && (DiscKind.Bit.DiscStd == eDiscDVD))
                            {
                                temp_dur = cur_time + SACD_F_PP_DELAY_T;  //for SACD
                            }
                            else
#endif
                            if ((svoPrvtVar_focusP2PCount == 1) && (DiscKind.Bit.DiscStd == eDiscDVD) && (type == eRampFEpp) && (direction == UP))
                            {//from 720
                                //to prevent DVD take substrate as layer (only DVD needs this protection)
                                temp_dur = cur_time + (F_PP_DELAY_T * 4);
                            }
                            else
                            {
                                temp_dur = cur_time + F_PP_DELAY_T;
                            }

                            if (temp_dur > duration)
                            {
                                temp_dur = duration;
                            }
                        }
                    }
                }
                break;

            case 5://waiting for S-curve falling end
                if (feo_cur >= -feo_thr)
                {
                    // restart measure peak
                    if (type != eRampSTM)
                    {
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                    stage = 6;
                }
                break;
        }//switch
        cur_time = ReadTimer(&temp_timer);
    }
    while (cur_time <= temp_dur);

#ifdef FEPP_DEBUG_MSG
    for (stage = 0; stage < fepp_debug_index; stage++)
    {
        SendMsgCnB(SHOW_DEBUG_MSG,3,0x315020,B1B(stage),B2B((fepp_debug_fe[stage] * DSPADC_IN_RANGE) >> DSPADC_BITS));
    }
#endif

    if (type == eRampSTM)
    {
        //store BS peak/bottom
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
        BSpeak = READ_REG(HAL_SRVREG_MAP->SRCR);
        if (BSpeak > (TOP_SATURATE_GPADC >> 2))
        {
            svoRampMeasVar.saturated = TRUE;
        }
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSBOTTOM);
        BSbottom = READ_REG(HAL_SRVREG_MAP->SRCR);

        //store FE peak/bottom
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);
        fe_peak = READ_REG(HAL_SRVREG_MAP->SRCR);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);
        fe_bottom = READ_REG(HAL_SRVREG_MAP->SRCR);

        if (svoRampMeasVar.FEpp[0] >= svoRampMeasVar.FEpp[1])
        {
            svoRampMeasVar.FEpp[0] = fe_peak - fe_bottom;
            svoRampMeasVar.BSpp[0] = BSpeak - BSbottom;
            svoRampMeasVar.SRFOpp[0] = (svoRampMeasVar.BSpp[0] << 2);
        }
        else
        {
            svoRampMeasVar.FEpp[1] = fe_peak - fe_bottom;
            svoRampMeasVar.BSpp[1] = BSpeak - BSbottom;
            svoRampMeasVar.SRFOpp[1] = (svoRampMeasVar.BSpp[1] << 2);
        }
    }
    else if (type == eRampFEBG)
    {
        //store SRFOpp
        if (temp_pkbt > TOP_SATURATE_GPADC)
        {
            svoRampMeasVar.saturated = TRUE;
        }
        temp_pkbt -= svoRampMeasVar.refSRFO;
        svoRampMeasVar.SRFOpp[0] = temp_pkbt;
        svoRampMeasVar.SRFOpp[1] = temp_pkbt;

        //store FEO peak bottom
        if (direction == UP)
        {
            svoRampMeasVar.FEpp[0] = fe_peak;
            svoRampMeasVar.FEpp[1] = -fe_bottom;
        }
        else
        {
            svoRampMeasVar.FEpp[0] = -fe_bottom;
            svoRampMeasVar.FEpp[1] = fe_peak;
        }
        if (feo_inv)
        {
            temp_pkbt = fe_peak;
            fe_peak = -fe_bottom;
            fe_bottom = -temp_pkbt;
        }
#if (FILTER_POINT != 1)
        fe_peak /= FILTER_POINT;
        fe_bottom /= FILTER_POINT;
#endif
        fe_peak += feo_ref;
        fe_bottom += feo_ref;
        if ((fe_peak > TOP_SATURATE_GPADC) || (fe_bottom < BOT_SATURATE_GPADC))
        {
            svoRampMeasVar.saturated = TRUE;
        }

        //store SPP2 peak bottom
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);
        fe_peak = READ_REG(HAL_SRVREG_MAP->SRCR);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);
        fe_bottom = READ_REG(HAL_SRVREG_MAP->SRCR);
        if ((fe_peak > (TOP_SATURATE_GPADC / 4)) || (fe_bottom < (BOT_SATURATE_GPADC / 4)))
        {
            svoRampMeasVar.saturated = TRUE;
        }
        fe_peak = fe_peak - svoRampMeasVar.refFE;
        fe_bottom = svoRampMeasVar.refFE - fe_bottom;
        svoRampMeasVar.BSpp[0] = fe_peak;
        svoRampMeasVar.BSpp[1] = fe_bottom;
    }

    WRITE_FIELD(HAL_SRV_PKBOT,0);
#if (FE_SLEW_RATE_LIMITER == 2)
    WRITE_REG(HAL_SRVREG_MAP->SLEWRATE,0x0000FF);
#endif
    if (svoRampMeasVar.saturated)
    {
		#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
		#endif    
        SendMsg80(SHOW_DEBUG_MSG,0x31060C); //WARNING: RampMeas saturated
    }

    // Restore fc of FE LPF.
    HAL_AFE_SetFELpfBW(orig_FELpfBW);
}

/*********************************************************
* Module: focus_wait_stable()
*
* Description: after focus drop, wait the LENS to stable then ramp to F_SEARCH_T * ramp_velocity below layer
*
* Returns: void
* Params:  stable_time(ms)
**********************************************************/
void focus_wait_stable(USHORT stable_time)
{
    LONG   foo_os;
#if (CHIP_REV >= 0xC0) && (SCRATCH_PROT_TIME > 0)
    ULONG  temp_timer1,temp_timer2;
    SHORT  temp_protfoos;
#endif
#if (NEW_FDP_DETECTION != 0)
    BYTE   ramp_up_time;
#endif

#if 0
    // Start FG count down
    svoSpeedVar.fgCount = 3;
    StartFGTimer();
#endif

    foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
    if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;
    ramp_up_time = (foo_os - FDP_FOO_OS) * 0x200000 / (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR);
#if (CHIP_REV >= 0xC0)
    #if (SCRATCH_PROT_TIME > 0)
    StartTimer(&temp_timer1);
    StartTimer(&temp_timer2);
    while (ReadTimer(&temp_timer1) <= SCRATCH_PROT_TIME)
    {
        if (READ_DSP_COEF(FOO_above_layer) & 0x0004)
        {
            if (ReadTimer(&temp_timer2) > 2)
            {
                temp_protfoos = 32;
                while ((temp_protfoos > 0) && (READ_DSP_COEF(FOO_above_layer) & 0x0004))
                {
                    temp_protfoos--;
                    HAL_SERVO_SetProtFoos(svoPrvtVar_ProtFoos * temp_protfoos / 32);
                    Delay100uS(1);
                }
                WRITE_DSP_COEF(FEDG_pol, 0);
                WRITE_DSP_COEF(FEDG_thr, 0);
                while ((READ_DSP_COEF(FOO_above_layer) & 0x0004));
                WRITE_DSP_COEF(FEDG_thr, svoPrvtVar_FEDG_thr / 2);
        #if (FOO_INVERSION == 0)
                WRITE_DSP_COEF(FEDG_pol,0x7FFF);
        #else
                WRITE_DSP_COEF(FEDG_pol,0x8000);
        #endif
                HAL_SERVO_SetProtFoos(svoPrvtVar_ProtFoos);
            }
        }
        else
        {
            StartTimer(&temp_timer2);
        }
    }
    #endif
    HAL_SERVO_SetProtFoos(0);

    if (ramp_up_time < FDP_STABLE_TIME - SCRATCH_PROT_TIME)
    {
        WaitTimer(FDP_STABLE_TIME - SCRATCH_PROT_TIME - ramp_up_time);
    }
#else
    if (ramp_up_time < FDP_STABLE_TIME)
    {
        WaitTimer(FDP_STABLE_TIME - ramp_up_time);
    }
#endif
#if 0
    if (svoSpeedVar.fgCount) // no FG interrupt.
    {
        BYTE orig_state,cnt;

        SendMsg80(SHOW_DEBUG_MSG,0x010207);         // WARNING: Power-driver IC has shut-down!

        if (READ_REG(HAL_AHB_INTCTRLREG_MAP->ICITCR))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x31000F);     // WARNING: All interrupts are disabled
        }
        //set FOO output start from 0
    #if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((-HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
    #else
        HAL_SERVO_SetFoos(0);
    #endif
        //set sled,tilt,DMO output to 0 and rest for 1 sec
        SledStop();
        HAL_SERVO_SetTilt(TILT_INIT_DAC);
        WRITE_DSP_COEF(kdf14,0);
        orig_state = svo_Var_motorDriverState;
        SetMotorDriverState(ALL_MUTE_ON);
        WaitTimer(1000);

        // Hold DMO at full kick level
        WRITE_DSP_COEF(kdf14,(svoSpeedVar.Kd14Fg * 0x80)/DMODAC_FACTOR);
        WRITE_DSP_COEF(dmfbk_level, (short)DISC_KICK_LEVEL*DMODAC_FACTOR);
        WRITE_REG(HAL_SRVREG_MAP->DMFBK, HAL_SRV_DMFK); /* set to accelerate */

        // Wait for power-driver back from over-current/voltage protection.
        cnt = 0;
        svoSpeedVar.fgCount = 3;
        do
        {
            SetMotorDriverState(ALL_MUTE_ON);
            Delay100uS(1);
            SetMotorDriverState(REVERSE_BRAKE);
            Delay100uS(10);
        }
        while (svoSpeedVar.fgCount && (++cnt < 255));

        SetMotorDriverState(orig_state);

        spdSetKd14(svoSpeedVar.Kd14Fg);
        WRITE_REG(HAL_SRVREG_MAP->DMSV, 0x00);
        if (cnt == 255)
        {
            Power_driver_protected = 1;
#if(DRIVER_SHUT_DOWN_SPEED_DOWN == 1)
            servo_status |= SERVO_STATUS_DRIVER_SHUT_DOWN;
            svoVar_BD_SpdDown_cnt = 5;
#endif
        }
        else
        {
#if (CHIP_REV < 0xC0)
            spdSetFGFreq((60 * INPUT_CLOCK_HZ / ((DISK_FR2X + 1) * DISK_NFGPLS * FG_TIMER_PRESCALER)) / svoSpeedVar.fgPulseTime);
#else
            spdSetFGFreq((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / (READ_FIELD(HAL_SRV_MDIV) * svoSpeedVar.fgPulseTime));
#endif
            if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoSeekVar.jumpType & L_J_MASK))
            {
                svoColli_Homing();
                while (svoStp_busy())
                {
                    ExitProcess();
                }

                svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(Layer_Index));
                while (svoStp_busy())
                {
                    ExitProcess();
                }
            }
#if (DRIVER_SHUT_DOWN_SPEED_DOWN == 1)
            servo_status &= (~SERVO_STATUS_DRIVER_SHUT_DOWN);
#endif
        }
        SendMsgCn(SHOW_DEBUG_MSG,4,0x38001C,A1B(OpuTemperature),A2B(svoCalColliVar[Layer_Index].opt_value),A1B((OpuTemperature - svoCalColliVar[Layer_Index].temperature) * COLLI_TEMP_SLOPE));
        SendMsgCnB(SHOW_DEBUG_MSG,1,0x010208,B1B(cnt)); // Power-driver Protection Reset Count = %u

    #if (NEW_SPINDLE_CONTROL == 1)
        svoSpeedVar.SpdInRange = FALSE;
        spdSetFGFreq(svoSpeedVar.TargetRpm);
    #endif
    }

    #if (POWERDRIVER_SHUTDOWN_DETECTION == 1)
    if (PD_SHUTDOWN)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x010207); // WARNING: Power-driver has shut-down!
        #if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((-HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
        #else
        HAL_SERVO_SetFoos(0);
        #endif
        while (PD_SHUTDOWN) ExitProcess();
        spdMonitorRPM();
        spdWaitSpdInRange();
    }
    #endif
#endif // #if (MOTOR_DRIVER == R2A30209)

#if (CHIP_REV >= 0xC0)
    ramp_to(foo_os);
#else
    ramp_FOOS(foo_os);
#endif
    //always make sure sled and colli are stopped
    while (svoStp_busy())
    {
        ExitProcess();
    }
}

#if (STREAM_STM == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: StreamInitiate()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#define NR_OF_SIGNALS   3
ULONG StreamInitiate(halOPC_AOPCSampNum_t eNrOfSamples, ULONG Frequency, halOPC_AOPCAvgSamp_t eAvgSamples, LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Channel1, LAL_STREAM_AnalogFESignalT Channel2, LAL_STREAM_AnalogFESignalT Channel3)
{
    LAL_STREAM_InputSignalsDescriptorT InputSignalsDescriptor;
    LAL_STREAM_SamplingDescriptorT SamplingDescriptor;

    /* init */
    LAL_STREAM_Init();
    /* configure */
    if ( eFEType == LAL_STREAM_FE_DIGITAL )
    {
       WRITE_FIELD(HAL_DFE_AMAXLPFBW,0x01);
       WRITE_FIELD(HAL_DFE_AMINLPFBW,0x01);

       WRITE_FIELD(HAL_DFE_BETAMSEN,1); // Enable beta-measurement

       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_DIGITAL, &InputSignalsDescriptor); //InputSignalsDescriptor is not required for DIGITAL
    }
    else
    {
       InputSignalsDescriptor.Afe.NrOfSignals = NR_OF_SIGNALS;
       InputSignalsDescriptor.Afe.Signal[0] = Channel1;
       InputSignalsDescriptor.Afe.Signal[1] = Channel2;
       InputSignalsDescriptor.Afe.Signal[2] = Channel3;
       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_ANALOG, &InputSignalsDescriptor);
    }
    SamplingDescriptor.timer.eNrOfSamples = eNrOfSamples;
    SamplingDescriptor.timer.Frequency = Frequency;
    SamplingDescriptor.timer.eAvgSamples = eAvgSamples;
    LAL_STREAM_ConfigureSampling(LAL_STREAM_MODE_TIMER, &SamplingDescriptor);

    return SamplingDescriptor.timer.Frequency;
}

/*-----------------------------------------------------------------------------
 *
 * Module: GetStreamStatus()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void GetStreamStatus(eRampMeasType type, USHORT NrOfSamples, USHORT frequency, BYTE direction)
{
    LAL_STREAM_StatusT status;

    SetTimeout(2, SEC_2);
    do
    {
        status = LAL_STREAM_GetStatus();

        switch (status)
        {
            case LAL_STREAM_STATUS_BUSY:
                if (timeout_chk(2) == eTimerTimeout)     /* time out check */
                {
                    /* abnormal operation */
                    SendMsgCn(DEBUG_STREAM_MSG,2,0x64020B,A2B(NrOfSamples)); //Nr of Stream sample
                    LAL_STREAM_Analyse(type,NrOfSamples,frequency,direction);
                    LAL_STREAM_Abort();
                    send_msg80(SHOW_DEBUG_MSG,0x64000A); //Stream timeout
                }
                else
                {
                    send_msg80(DEBUG_STREAM_MSG,0x640106); //Streaming...
                    //send_msg84L(DEBUG_STREAM_MSG,0x640118,READ_FIELD(HAL_OPC_WBLSTM)); //OPC_WBLSTM
                    ExitProcess();
                }
                break;

            case LAL_STREAM_STATUS_READY:
                SendMsgCn(DEBUG_STREAM_MSG,2,0x64020B,A2B(NrOfSamples)); //Nr of Stream sample
                LAL_STREAM_Analyse(type,NrOfSamples,frequency,direction);
                LAL_STREAM_Stop();
                break;

            case LAL_STREAM_STATUS_ERROR:
                SendMsgCnB(SHOW_DEBUG_MSG,2,0x560710,B2B(LAL_STREAM_GetError())); //Stream error
                /* do something with the error here.. */
                LAL_STREAM_Stop();
                break;

            case LAL_STREAM_STATUS_IDLE:
            default:
                send_msg80(DEBUG_STREAM_MSG,0x64020C); //Stream unhandled situation...
                ExitProcess();
                break;
        }
    }
    while (status == LAL_STREAM_STATUS_BUSY);

    SetTimeout(2, TIMER_STOP);
}

/*-----------------------------------------------------------------------------
 *
 * Module: RampStream()
 * Description: measure FEamp, BSamp, SRFOamp during ramping
 *
 *----------------------------------------------------------------------------*/
void RampStream(eRampMeasType type, USHORT duration, BYTE direction, discKindT DiscKind)
{
    ULONG   temp_timer,cur_time,stream_frequency;
    USHORT  temp_dur;
    SHORT   fe_thr,fe_peak;
    BYTE    orig_pdopc;

    //power up OPC clock for STREAM
    orig_pdopc = READ_FIELD(HAL_GLOBAL_OPCCKSTP);
    if (orig_pdopc)
    {
        WRITE_FIELD(HAL_GLOBAL_OPCCKSTP,0);
    }

    if (type == eRampSTM)
    {
        fe_thr = COEF_GetValue(AFE_CALC_STM_FE_TH) >> (GPADC_BITS - DSPADC_BITS);

        //minimum duration for BD/HD  = distance_to_time(AFE_CALC_STM_CD_TH),
        //                     CD/DVD = distance_to_time(AFE_CALC_STM_CD_TH * 2)
        switch (DiscKind.Bit.DiscStd)
        {
#if (BD_ENABLE == 1)
            case eDiscBD:
                stream_frequency = (F_IDEN_SENSITIVITY_BD * FOCUS_SEARCH_KF18 / 0x1000 * FOC_RAMP_FACTOR);
                break;
#endif
#if (ENABLE_HDDVD == 1)
            case eDiscHD:
                stream_frequency = F_IDEN_SENSITIVITY_HD * FOCUS_SEARCH_KF18 / 0x1000 * FOC_RAMP_FACTOR;
                break;
#endif
            case eDiscDVD:
                stream_frequency = F_IDEN_SENSITIVITY_DVD * FOCUS_SEARCH_KF18 / 0x2000 * FOC_RAMP_FACTOR;
                break;

            case eDiscCD:
            default:
                stream_frequency = F_IDEN_SENSITIVITY_CD * FOCUS_SEARCH_KF18 / 0x2000 * FOC_RAMP_FACTOR;
                break;
        }
        stream_frequency /= COEF_GetValue(AFE_CALC_STM_CD_TH);
    }
    else
    {
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            fe_thr = FE_SMSTEP_TH_RAM >> (GPADC_BITS - DSPADC_BITS);
        }
        else
#endif
        {
            fe_thr = FE_SMSTEP_TH >> (GPADC_BITS - DSPADC_BITS);
        }
        stream_frequency = 1000 * 1024 / (F_PP_DELAY_T * 1.5);
    }

    stream_frequency = StreamInitiate(halOPC_AOPCSampNum_1024,stream_frequency,halOPC_AOPCAvg_4Samp,LAL_STREAM_FE_ANALOG,HAL_AFE_SRFO,HAL_AFE_FE,HAL_AFE_BS);
    //send_msgA4(DEBUG_DISK_ID_MSG,0x5100AC,stream_frequency);
    SendMsgCnB(DEBUG_DISK_ID_MSG,1,0x380406,B2B(stream_frequency)); 

    StartTimer(&temp_timer);

    svoRampMeasVar.LayerFoo = svoCalMisc2Var.fooos;

    temp_dur = duration;

    streamMeasVar.RampStartfoo = HAL_SERVO_GetFooDac();
    //send_msgA2(DEBUG_DISK_ID_MSG,0x51000A,streamMeasVar.RampStartfoo);
    SendMsgCnB(DEBUG_DISK_ID_MSG,1,0x380407,B2B(streamMeasVar.RampStartfoo)); // Power-driver Protection Reset Count = %u
    LAL_STREAM_Start(HAL_OPC_MODE_STM);
    WRITE_FIELD(HAL_SRV_PKBOT,0);
    do
    {
        // restart measure FEpp
        WRITE_FIELD(HAL_SRV_PKBOT,1);
        WaitTimer(1);
        cur_time = ReadTimer(&temp_timer);

        //store FE peak/bottom
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);
        fe_peak = READ_REG(HAL_SRVREG_MAP->SRCR);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);
        fe_peak -= READ_REG(HAL_SRVREG_MAP->SRCR);
        if (fe_peak > 2 * fe_thr)
        {
#if (SACD == 1)
            if ((type == eRampSTM) && (DiscKind.Bit.DiscStd == eDiscDVD))
            {
                temp_dur = cur_time + COEF_GetValue(AFE_CALC_STM_CD_TH) / (F_IDEN_SENSITIVITY_DVD * FOCUS_SEARCH_KF18 / 0x400000 * FOC_RAMP_FACTOR / 1000);  //for SACD
            }
            else
#endif
            {
                temp_dur = cur_time + F_PP_DELAY_T;
            }
            if (temp_dur > duration)
            {
                temp_dur = duration;
            }
            WRITE_FIELD(HAL_SRV_PKBOT,0);
        }
    }
    while (cur_time <= temp_dur);

    WRITE_FIELD(HAL_SRV_PKBOT,0);
    HAL_OPC_ConfigureGatedSampling(halOPC_AOPCSampNum_1);//stop streaming immediately
    GetStreamStatus(type, (stream_frequency * (2 * temp_dur + 1)) / 2000, stream_frequency, direction);
    if (orig_pdopc)
    {
        WRITE_FIELD(HAL_GLOBAL_OPCCKSTP,orig_pdopc);
    }
}
#endif
/*********************************************************
* Module: focus_on_chk()
*
* Description: FW based focus ok judgement. The HW based focus ok judgement can't work on defect disc at high speed (bug 5084)
*
* Returns: OK if focus on ok, NG if focus on fail
* Params:  void
**********************************************************/
BYTE focus_on_chk(USHORT *TEp2p)//== start == change BD wrong layer detection measurement from 1/8 rev to 1 rev
{
    LONG   foo_os;
    ULONG  sample_num;
    BYTE   i, ret, orig_TELpfBW;
    ULONG  TEp2p_sum;
#if (CHIP_REV < 0xC0)
    BYTE   fokl, BS_peak;
#endif

    //set BEAM_LOW_EQ to a higher bandwidth to catch up with BS when focus just on
    WRITE_DSP_COEF(kb10, 0x7FFF);
    WRITE_DSP_COEF(kb11, 0x7FFF - READ_DSP_COEF(kb12));

#if (CHIP_REV >= 0xC0)
    StartTimer(&sample_num);
    while ((!FokStatusOK()) && (!svoIramVar.focusDropIsrFlag) && (ReadTimer(&sample_num) < 4));
#else
    fokl = READ_FIELD(HAL_SRV_FOKL);
    WRITE_FIELD(HAL_SRV_FOKL, 0);
#endif
    ret = OK;
    TEp2p_sum = 0;
    foo_os = 0;
    sample_num = 0;
    WRITE_FIELD(HAL_SRV_PKBOT, 0);
#if (CHIP_REV < 0xC0)
    HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RFILE, 0x24, 0);//set debug CH1 as FE_13
    Delay100uS(10);
    InitLPFIIR(0,1000);
#endif
    orig_TELpfBW = HAL_AFE_GetTELpfBW();
    HAL_AFE_SetTELpfBW(e_TELPF_660);

    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        if ((svoCalVar.calGainDoneBits & dif_RF_end) == 1)
        {
            Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
        }
    #if (ENABLE_LEARN_DEFECT == 1)
        LAL_DSP_SetLearnDefect(lalDSP_On);
    #endif
    }

#if (CHIP_REV < 0xC0)
    svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2);
    i = (DISK_NFGPLS * (DISK_FR2X + 1) / 2);
#else
    svoSpeedVar.fgCount = DISK_NFGPLS;
    i = DISK_NFGPLS;
#endif
    StartFGTimer();
    do
    {
        WRITE_FIELD(HAL_SRV_PKBOT, 1);    /* reset Peak/Bottom */// fix a possible focus on fail problem over 6000RPM on defect disc
        if (i > 1)
        {
            i--;
            if (i == (DISK_NFGPLS * 3 / 4))
            {
                //restore RECD2 measurement
                GAIN_RECD;
            }
#if (CHIP_REV >= 0xC0)
            do
            {
                WRITE_FIELD(HAL_SRV_RSEL,halSRV_RSEL_FOO);   //select FOO_DAC
                foo_os += READ_FIELD(HAL_SRV_DATR);
                sample_num++;
                ret = (FokStatusOK()? OK : NG);
            }
            while ((ret == OK) && (svoSpeedVar.fgCount >= i) && (foo_os < 0x7FFFFC00));
#else
            while ((svoSpeedVar.fgCount >= i) && (foo_os < 0x7FFFFC00));
            {
    #if (FOO_INVERSION == 0)
                foo_os += LPFIIR(READ_DSP_COEF(debug1));//FE_13, FELFG output
    #else
                foo_os -= LPFIIR(READ_DSP_COEF(debug1));//FE_13, FELFG output
    #endif
                sample_num++;
            }
#endif
        }
#if (CHIP_REV < 0xC0)
        else
        {
            i = 0;
            DelaymS(1);
        }
#endif
        WRITE_FIELD(HAL_SRV_PKBOT, 0);
#if (CHIP_REV < 0xC0)
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_BSPEAK);    /* read BS Peak data */
        BS_peak = READ_REG(HAL_SRVREG_MAP->SRCR);
        if (BS_peak < fokl + (svoCalVar.bsOfsData.all16 >> 2))
        {
            ret = NG;
            break;
        }
#endif
        if (i > 0)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEPEAK);
            TEp2p_sum += READ_REG(HAL_SRVREG_MAP->SRCR);
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEBOTTOM);
            TEp2p_sum -= READ_REG(HAL_SRVREG_MAP->SRCR);
        }
    }
#if (CHIP_REV >= 0xC0)
    while ((ret == OK) && (i > 1));

    *TEp2p = TEp2p_sum * DSPADC_IN_RANGE / ((DISK_NFGPLS - i) << DSPADC_BITS);
#else
    while ((!FokStatusOK()) || (i > 1));

    *TEp2p = TEp2p_sum * DSPADC_IN_RANGE / ((DISK_NFGPLS - 1) << DSPADC_BITS);

    WRITE_FIELD(HAL_SRV_FOKL, fokl);  //FOK_WORK_AROUND bug 5084
#endif
    HAL_AFE_SetTELpfBW(orig_TELpfBW);


    if (ret == OK)
    {
#if (AFE_NORMALIZER == 1)
        afeSvognEnable();
#endif // AFE_NORMALIZER == 1

        //restore RECD2 measurement
        GAIN_RECD;

        foo_os /= sample_num;
#if (FOOTILT_DIFFERENTIAL == 1)
        foo_os -= CONVERT_TILTDAT_TO_FOO(READ_FIELD(HAL_SRV_TILTDAT));
#endif
        foo_os -= 512;
        if (foo_os > svoPrvtVar_avgFOO + (F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000))
        {
            SendMsgCn(SHOW_DEBUG_MSG,4,0x316673,A2B(foo_os),A2B(svoPrvtVar_avgFOO));//WARNING: FOO_OS abnormal:%4d, init:%4d
            foo_os = svoPrvtVar_avgFOO + (F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000);
        }
        if (foo_os < svoPrvtVar_avgFOO - (F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000))
        {
            SendMsgCn(SHOW_DEBUG_MSG,4,0x316673,A2B(foo_os),A2B(svoPrvtVar_avgFOO));//WARNING: FOO_OS abnormal:%4d, init:%4d
            foo_os = svoPrvtVar_avgFOO - (F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000);
        }
        apply_FOOS(foo_os);    //set FO_OFFSET to the average FOO
        calSetMisc(0,FOO_OS,foo_os);
        track_off();//update KFG for track off
    }
    else
    {
#if (NEW_FDP_DETECTION != 0)
        svoFDropIsr();
#endif
    }

    //set BEAM_LOW_EQ to the programed bandwidth
    svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, BEAM_LOW_EQ);

    return ret;
}

/*********************************************************
* Module: ramp_stop()
*
* Description: stop focus ramp and hold LENS at current height
*
* Returns: void
* Params:  void
**********************************************************/
SHORT ramp_stop(void)
{
    SHORT foo_dac;

#if (CHIP_REV >= 0xC0)
    HAL_SERVO_SetFocStp();
#endif

    foo_dac = HAL_SERVO_GetFooDac();

#if (CHIP_REV < 0xC0)
    if (foo_dac > FOO_OFFSET_LIMIT) foo_dac = FOO_OFFSET_LIMIT;
    if (foo_dac < -FOO_OFFSET_LIMIT) foo_dac = -FOO_OFFSET_LIMIT;

    WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
    HAL_SERVO_SetFoos(foo_dac);
    WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
    #if (FOOFFSET2_ENABLE == 1)
    WRITE_DSP_COEF(kf18,0);
    HAL_SERVO_SetFoos(foo_dac);
    #else
    WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
    HAL_SERVO_SetFoos(foo_dac);
    WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
    #endif
#endif
    return foo_dac;
}

/*********************************************************
* Module: ramp_FOOS()
*
* Description: change FOOS slowly, the ramp velocity is about twice of normal ramp.
*
* Returns: void
* Params:  valFOOS
**********************************************************/
void ramp_FOOS(SHORT valFOOS)
{
    SHORT valFOOS_curr;

    if (valFOOS > FOO_OFFSET_LIMIT) valFOOS = FOO_OFFSET_LIMIT;
    if (valFOOS < -FOO_OFFSET_LIMIT) valFOOS = -FOO_OFFSET_LIMIT;

    valFOOS_curr = HAL_SERVO_GetFoos();

    while (valFOOS_curr != valFOOS)
    {
        if (valFOOS > valFOOS_curr)
        {
            valFOOS_curr++;
        }
        else
        {
            valFOOS_curr--;
        }

        HAL_SERVO_SetFoos(valFOOS_curr);

        Delay100uS((0x200000 * 10) / (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR));
    }
}

#if (CHIP_REV >= 0xC0)
/*********************************************************
* Module: ramp_to()
*
* Description: ramp to given FOO_dac
*
* Returns: void
* Params:  valFOO
**********************************************************/
void ramp_to(SHORT valFOO)
{
    ULONG  temp_timer,ramp_time_out;
    SHORT  valFOO_curr;

    valFOO_curr = HAL_SERVO_GetFooDac();
    ramp_time_out = ABS(valFOO_curr - valFOO) * 0x200000 / (READ_DSP_COEF(kf18) * FOC_RAMP_FACTOR) + 2;

    if (valFOO_curr < valFOO)
    {
        HAL_SERVO_SetFocUp(HAL_SRV_HISRSET);
    }
    else
    {
        HAL_SERVO_SetFocDn(HAL_SRV_HISRSET);
    }

    if (valFOO_curr != valFOO)
    {
        StartTimer(&temp_timer);
        while(ReadTimer(&temp_timer) <= ramp_time_out)
        {
            if (HAL_SERVO_GetFooDac() == valFOO)
                break;
        }
    }
    HAL_SERVO_SetFocStp();
}
#endif

/*********************************************************
* Module: apply_FOOS()
*
* Description: change FOOS smoothly
*
* Returns: void
* Params:  valFOOS
**********************************************************/
void apply_FOOS(SHORT target)
{
    SHORT FOOS_curr;

    FOOS_curr = HAL_SERVO_GetFoos();

    while (FOOS_curr != target)
    {
        if (target > FOOS_curr)
        {
            FOOS_curr++;
        }
        else
        {
            FOOS_curr--;
        }

        HAL_SERVO_SetFoos(FOOS_curr);

        Delay100uS(1);
        //while(READ_FIELD(HAL_SRV_DFCTTBL));//don't change FOOS while defect is detected
    }
}

/*********************************************************
* Module: apply_TILT()
*
* Description: change TILT smoothly
*
* Returns: void
* Params:  target
**********************************************************/
void apply_TILT(SHORT target)
{
    SHORT TILT_curr;
    SHORT step;

    TILT_curr = HAL_SERVO_GetTilt();
    while (1)
    {
        step = target - TILT_curr;
        if (step > MAX_TILT_STEP_PER_100uS)
        {
            step = MAX_TILT_STEP_PER_100uS;
        }
        else if (step < -MAX_TILT_STEP_PER_100uS)
        {
            step = -MAX_TILT_STEP_PER_100uS;
        }
        TILT_curr += step;
        HAL_SERVO_SetTilt(TILT_curr);
        if (TILT_curr == target)
            break;
        Delay100uS(1);    //related to TILT_STEP_DAC in svo_sta.c
    }
}

/*********************************************************
* Module: apply_FEBC()
*
* Description: change FEBC smoothly
*
* Returns: void
* Params:  valFEBC
**********************************************************/
void apply_FEBC(SHORT target)
{
    SHORT FEBC_curr;
    LONG  step;
    BYTE  orgRGSEL;

    orgRGSEL = READ_REG(HAL_SRVREG_MAP->RGSEL);
    FEBC_curr = HAL_SERVO_GetFebc();
    while (1)
    {
        step = (LONG)target - (LONG)FEBC_curr;
        if (step > MAX_FEBC_STEP_PER_100uS)
        {
            step = MAX_FEBC_STEP_PER_100uS;
        }
        else if (step < -MAX_FEBC_STEP_PER_100uS)
        {
            step = -MAX_FEBC_STEP_PER_100uS;
        }

        FEBC_curr += step;
        HAL_SERVO_SetFebc(FEBC_curr);
        if (FEBC_curr == target)
            break;

        Delay100uS(1);
        //while(READ_FIELD(HAL_SRV_DFCTTBL));//don't change FEBC while defect is detected
    }
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,orgRGSEL);
}

#if (DVD_RAM_READ == 1)
void apply_RFAOCREG(SBYTE target)
{
    //because of header;when RF calibration,try to change RFAOCREG smoothly to measure RFpp/1rev
    SBYTE RFOOS_tmp;
    SBYTE step;

    RFOOS_tmp=calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);
    while (1)
    {
        Delay100uS(5);
        if (RFOOS_tmp == target)
            break;

        step = target - RFOOS_tmp;
        if (step > 2)
        {
            step = 2;
        }
        else if (step < -2)
        {
            step = -2;
        }
        RFOOS_tmp += step;
        WRITE_FIELD(HAL_AFE_RFAOCREG,calSBYTE2DacCount(RFOOS_tmp,8));
    }
}

void apply_RFAGCREG(BYTE target)
{
    //because of header;when RF calibration,try to change RFAGCREG smoothly to measure RFpp/1rev
    SBYTE RRFG_tmp;
    SBYTE step;
    RRFG_tmp=READ_FIELD(HAL_AFE_RFAGCREG);
    while (1)
    {
        Delay100uS(5);
        if (RRFG_tmp == target)
            break;

        step = target - RRFG_tmp;
        if (step > 2)
        {
            step = 2;
        }
        else if (step < -2)
        {
            step = -2;
        }
        RRFG_tmp += step;
        WRITE_FIELD(HAL_AFE_RFAGCREG,RRFG_tmp);
    }
}

/*-------------------------------------------------------
--------------------------------------------------------*/
void apply_WRFCSO(void)
{
    SHORT temp;
    //if (Iram_WGateUp == TRUE)
    //    return;

    if ((ram_fe_tilt_cal_done & WRFCSO_CALDone) == WRFCSO_CALDone
        &&(ram_fe_tilt_cal_done & LNDWRFCSO_CALDone) == LNDWRFCSO_CALDone)
    {
        WRITE_FIELD(HAL_AFE_WRFCSO,calSBYTE2DacCount(FEWriteOffset[GROOVE_TRACK], 8));
    #if (CHIP_REV >= 0xC0)
        WRITE_FIELD(HAL_AFE_WRLNDFCSO,calSBYTE2DacCount(FEWriteOffset[LAND_TRACK], 8));
    #endif
    }
    else if ((ram_fe_tilt_cal_done & WRFCSO_CALDone) == WRFCSO_CALDone)
    {
        WRITE_FIELD(HAL_AFE_WRFCSO,calSBYTE2DacCount(FEWriteOffset[GROOVE_TRACK], 8));
    #if (CHIP_REV >= 0xC0)
        WRITE_FIELD(HAL_AFE_WRLNDFCSO,calSBYTE2DacCount(FEWriteOffset[GROOVE_TRACK], 8));
    #endif
    }
    else if ((ram_fe_tilt_cal_done & LNDWRFCSO_CALDone) == LNDWRFCSO_CALDone)
    {
        WRITE_FIELD(HAL_AFE_WRFCSO,calSBYTE2DacCount(FEWriteOffset[LAND_TRACK], 8));
    #if (CHIP_REV >= 0xC0)
        WRITE_FIELD(HAL_AFE_WRLNDFCSO,calSBYTE2DacCount(FEWriteOffset[LAND_TRACK], 8));
    #endif
    }
    else
    {
    #if (DVDRAM_DMA_PICAL_SMP_RF == 1)
        float DMA12WrFCSO_L,DMA12WrFCSO_G;
        float DMA34WrFCSO_L,DMA34WrFCSO_G;
        float DMA12WrFCSOPos,DMA34WrFCSOPos;
        float curPos;

        //groove trk
        if (Iram_stp_CurrentPosition <= svoCalDVDRAMWrFCSOPos[0])
        {
            temp = svoCalDVDRAMWrFCSOVar[0][0];
        }
        else if (Iram_stp_CurrentPosition >= svoCalDVDRAMWrFCSOPos[1])
        {
            temp = svoCalDVDRAMWrFCSOVar[1][0];
        }
        else
        {
            DMA12WrFCSO_G = (float)(svoCalDVDRAMWrFCSOVar[0][0]*10);
            DMA34WrFCSO_G = (float)(svoCalDVDRAMWrFCSOVar[1][0]*10);
            DMA12WrFCSOPos = (float)svoCalDVDRAMWrFCSOPos[0];
            DMA34WrFCSOPos = (float)svoCalDVDRAMWrFCSOPos[1];
            curPos = (float)Iram_stp_CurrentPosition;
            DMA12WrFCSO_G = linear_interpolation(DMA12WrFCSOPos,DMA12WrFCSO_G,DMA34WrFCSOPos,DMA34WrFCSO_G,curPos);
            temp = float2short(DMA12WrFCSO_G/10);
        }
        WRITE_FIELD(HAL_AFE_WRFCSO,calSBYTE2DacCount(temp,8));//current wgate is down,preset it without delay

        #if (CHIP_REV >= 0xC0)
        //land trk
        if (Iram_stp_CurrentPosition <= svoCalDVDRAMWrFCSOPos[0])
        {
            temp = svoCalDVDRAMWrFCSOVar[0][1];
        }
        else if (Iram_stp_CurrentPosition >= svoCalDVDRAMWrFCSOPos[1])
        {
            temp = svoCalDVDRAMWrFCSOVar[1][1];
        }
        else
        {
            DMA12WrFCSO_L= (float)(svoCalDVDRAMWrFCSOVar[0][1]*10);
            DMA34WrFCSO_L = (float)(svoCalDVDRAMWrFCSOVar[1][1]*10);
            DMA12WrFCSOPos = (float)svoCalDVDRAMWrFCSOPos[0];
            DMA34WrFCSOPos = (float)svoCalDVDRAMWrFCSOPos[1];
            curPos = (float)Iram_stp_CurrentPosition;
            DMA12WrFCSO_L = linear_interpolation(DMA12WrFCSOPos,DMA12WrFCSO_L,DMA34WrFCSOPos,DMA34WrFCSO_L,curPos);
            temp = float2short(DMA12WrFCSO_L/10);
        }
        WRITE_FIELD(HAL_AFE_WRLNDFCSO,calSBYTE2DacCount(temp,8));
        #endif // #if (CHIP_REV >= 0xC0)
    #else // #if (DVDRAM_DMA_PICAL_SMP_RF == 1)
        WRITE_FIELD(HAL_AFE_WRFCSO,READ_FIELD(HAL_AFE_FCSO));
        #if (CHIP_REV >= 0xC0)
        WRITE_FIELD(HAL_AFE_WRLNDFCSO,READ_FIELD(HAL_AFE_LNDFCSO));
        #endif
    #endif // #if (DVDRAM_DMA_PICAL_SMP_RF == 1)
    }
}

/*********************************************************
* Module: apply_FCSO()
*
* Description: change FCSO smoothly to make RF signal
*              change smoothly, it is similar as apply_FEBC
*
* Returns: void
* Params:
**********************************************************/
void apply_FCSO(SHORT target,BYTE tracktype)
{
    SHORT  FCSO_curr;
    SHORT  step;
    BYTE   tmp;
    SHORT  FCSO_R_L_B,FCSO_R_L_W,FCSO_R_G_B,FCSO_R_G_W;

    //initialize
    FCSO_R_G_W = calDacCount2SBYTE(READ_FIELD(HAL_AFE_FCSO),8);
    FCSO_R_G_B = calDacCount2SBYTE(READ_FIELD(HAL_AFE_BLANKFCSO),8);
    FCSO_R_L_W = calDacCount2SBYTE(READ_FIELD(HAL_AFE_LNDFCSO),8);
    FCSO_R_L_B = calDacCount2SBYTE(READ_FIELD(HAL_AFE_BLNDFCSO),8);

    if (tracktype == GROOVE_TRACK)
    {
        //WRITE_FIELD(HAL_AFE_FCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
        if (FCSO_R_G_W != FCSO_R_G_B)
            WRITE_FIELD(HAL_AFE_BLANKFCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
    }
    else if (tracktype == LAND_TRACK)
    {
        //WRITE_FIELD(HAL_AFE_LNDFCSO,calSBYTE2DacCount(FCSO_R_L_W,8));
        if (FCSO_R_L_W != FCSO_R_L_B)
            WRITE_FIELD(HAL_AFE_BLNDFCSO,calSBYTE2DacCount(FCSO_R_L_W,8));
    }
    else
    {
        //WRITE_FIELD(HAL_AFE_FCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
        if (FCSO_R_G_W != FCSO_R_G_B)
            WRITE_FIELD(HAL_AFE_BLANKFCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
        if (FCSO_R_G_W != FCSO_R_L_W)
            WRITE_FIELD(HAL_AFE_LNDFCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
        if (FCSO_R_G_W != FCSO_R_L_B)
            WRITE_FIELD(HAL_AFE_BLNDFCSO,calSBYTE2DacCount(FCSO_R_G_W,8));
    }

    //get
    if (tracktype == GROOVE_TRACK)
    {
        FCSO_curr = calDacCount2SBYTE(READ_FIELD(HAL_AFE_FCSO),8);
    }
    else if (tracktype == LAND_TRACK)
    {
        FCSO_curr = calDacCount2SBYTE(READ_FIELD(HAL_AFE_LNDFCSO),8);
    }
    else//ALL_TRACK
    {
        FCSO_curr = calDacCount2SBYTE(READ_FIELD(HAL_AFE_FCSO),8);
    }

    //update
    while (1)
    {
       if (FCSO_curr == target)
           break;

        step = (SHORT)target - (SHORT)FCSO_curr;
        if (step > MAX_FCSO_STEP_PER_10uS)
        {
            step = MAX_FCSO_STEP_PER_10uS;
        }
        else if (step < -MAX_FCSO_STEP_PER_10uS)
        {
            step = -MAX_FCSO_STEP_PER_10uS;
        }

        FCSO_curr += step;
        tmp = calSBYTE2DacCount(FCSO_curr, 8);
        if ((tracktype == GROOVE_TRACK) || (tracktype == ALL_TRACK))
        {
            WRITE_FIELD(HAL_AFE_FCSO,tmp);
            WRITE_FIELD(HAL_AFE_BLANKFCSO,tmp);
        }
        if ((tracktype == LAND_TRACK) || (tracktype == ALL_TRACK))
        {
            WRITE_FIELD(HAL_AFE_LNDFCSO,tmp);
            WRITE_FIELD(HAL_AFE_BLNDFCSO,tmp);
        }
        Delay1uS(10);
    }
}
#endif // #if (DVD_RAM_READ == 1)

/*********************************************************
* Module: apply_FESPT()
*
* Description: Change FE set-point smoothly to make RF signal
*              change smoothly, it is same as changing FEBC
*
* Returns: void
* Params:  valFESPT
**********************************************************/
void apply_FESPT(SHORT valFESPT)
{
    BYTE  orgRGSEL;
    SHORT FESPT_curr,step;

    valFESPT &= 0xFFFC;
    orgRGSEL = READ_REG(HAL_SRVREG_MAP->RGSEL);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FEO);
    FESPT_curr = READ_REG(HAL_SRVREG_MAP->SRCR) * 4 - svoCalVar.feOfsData.all16;
    while (1)
    {
        if (FESPT_curr == valFESPT)
            break;

        step = valFESPT - FESPT_curr;
        if (step > MAX_FESPT_STEP_PER_10uS)
        {
            step = MAX_FESPT_STEP_PER_10uS;
        }
        else if (step < -MAX_FESPT_STEP_PER_10uS)
        {
            step = -MAX_FESPT_STEP_PER_10uS;
        }

        FESPT_curr += step;
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FEO);
        WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.feOfsData.all16 + FESPT_curr);
        Delay1uS(10);
    }
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,orgRGSEL);
}

/*********************************************************
* Module: apply_TESPT()
*
* Description: change TE set-point smoothly
*
* Returns: void
* Params:  valTESPT
**********************************************************/
void apply_TESPT(SHORT valTESPT)
{
    BYTE  orgRGSEL;
    SHORT TESPT_curr,step;

    valTESPT &= 0xFFFC;
    orgRGSEL = READ_REG(HAL_SRVREG_MAP->RGSEL);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);
    TESPT_curr = READ_REG(HAL_SRVREG_MAP->SRCR) * 4 - svoCalVar.teOfsData.all16;
    while (1)
    {
        if (TESPT_curr == valTESPT)
            break;

        step = valTESPT - TESPT_curr;
        if (step > MAX_TESPT_STEP_PER_100uS)
        {
            step = MAX_TESPT_STEP_PER_100uS;
        }
        else if (step < -MAX_TESPT_STEP_PER_100uS)
        {
            step = -MAX_TESPT_STEP_PER_100uS;
        }

        TESPT_curr += step;
        WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);
        WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.teOfsData.all16 + TESPT_curr);
        Delay100uS(1);
    }
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,orgRGSEL);
}

/*-----------------------------------------------------------------------------
 *
 * Module: CurveFit()
 * Description: second degree curve fitting
 *              y = cof[2] * x^2 + cof[1] * x + cof[0]
 *
 *----------------------------------------------------------------------------*/
void CurveFit(BYTE length, float *x, float *y, float *cof)
{
    float  y_shift,x_shift;
    float  temp,tempX;
    LONG   i,j,k;
    float  a[3][4];

    //shift x and y to prevent overflow during calculation.
    x_shift = x[0];
    y_shift = y[0];
    for (i=1;i<length;i++)
    {
        x_shift += x[i];
        y_shift += y[i];
    }
    temp = (float)length;
    x_shift = x_shift / temp;
    y_shift = y_shift / temp;
    for (i=0;i<length;i++)
    {
        x[i] -= x_shift;
        y[i] -= y_shift;
    }

    for (i=0;i<3;i++)
        for (j=0;j<=3;j++)
            a[i][j]=0.0;
    for (k=0;k<3;k++)
        cof[k]=0.0;

    //--- M11==c0+c1*Wp+c2*Wp^2
    for (i=0;i<length;i++)      // assign curve fitting matrix
    {
        a[0][3] += y[i];
        temp = tempX = x[i]; /* temp = x^1 */
        a[0][1] += temp;
        a[1][3] += (y[i] * temp);
        temp *= tempX;       /* temp = x^2 */
        a[0][2] += temp;
        a[2][3] += (y[i] * temp);
        temp *= tempX;       /* temp = x^3 */
        a[1][2] += temp;
        temp *= tempX;       /* temp = x^4 */
        a[2][2] += temp;
    }
    a[0][0]=(float)length; // skip sample 0 data
    a[1][0]=a[0][1];
    a[1][1]=a[2][0]=a[0][2];
    a[2][1]=a[1][2];

    //--- using Gauss direct elimination method
    //---  a is symmetric matrix and a[:][:]!=0
    for(i=0;i<3;i++)
        for(j=i+1;j<3;j++)
            for(k=3;k>=i;k--)
                a[j][k] = a[j][k] - a[i][k] * a[j][i] / a[i][i];

    for (j=2;j>=0;j--)
    {
        temp = 0.0;
        for(k=j+1;k<3;k++)
            temp = temp + a[j][k] * cof[k];
        cof[j] = (a[j][3] - temp) / a[j][j];   //  cof=[c0,c1,c2]
    }

    //remove x_shift & y_shift
    cof[0] = cof[0] + y_shift - cof[1] * x_shift + cof[2] * x_shift * x_shift;
    cof[1] = cof[1] - 2 * cof[2] * x_shift;
    for (i=0;i<length;i++)
    {
        x[i] += x_shift;
        y[i] += y_shift;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: LineFit()
 * Description: first degree line fitting
 *              y = cof[1] * x + cof[0]
 *
 *----------------------------------------------------------------------------*/
void LineFit(BYTE length, const float *x, const float *y, float *cof)
{
    float fNum1,fNum2,fNum3,fNum4,temp;
    BYTE i;

    fNum1 = fNum2 = fNum3 = fNum4 = 0;
    for(i = 0; i < length; i++)
    {
        temp = x[i];
        fNum1 += temp;
        fNum2 += y[i];
        fNum3 += temp * temp;
        fNum4 += temp * y[i];
    }//end for(i=0;i<bCount;i++);
    temp = (length * fNum3 - fNum1 * fNum1);
    cof[0] = (fNum2 * fNum3 - fNum1 * fNum4) / temp;
    cof[1] = (length * fNum4 - fNum1 * fNum2) / temp;
}

/*-----------------------------------------------------------------------------
 * Module: InitLPFIIR()
 * Description: FC(-3dB) = sample_rate / samples
 *----------------------------------------------------------------------------*/
void InitLPFIIR(SHORT LpfIIRInit, USHORT samples)
{
    svoLpfVar.InPrv  = LpfIIRInit;
    svoLpfVar.OutPrv = LpfIIRInit;
    svoLpfVar.coefA1 = ((2.0 - 2.0 * PI / samples) * 65536.0 /(2.0 + 2.0 * PI / samples) + 0.5);
    svoLpfVar.coefB1B2 = (65536 - svoLpfVar.coefA1) / 2;
}

#if (OPEN_SESSION == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: RFpp_chk()
 * Description: measure RF modulation to judge recorded or not
 *
 *----------------------------------------------------------------------------*/
BOOL  RFpp_chk(void)
{
    ad_dsp_t meas_signal;
    SBYTE    gain,offset;
    LONG     modulation;

    meas_signal = readADCmVPerRevs(NO_REVOLUTION, TRUE, RF_ADC);
    if (meas_signal.errcode == eNOERR)
    {
        if(READ_FIELD(HAL_AFE_RFAGCEN)) //DFE Controls RF AGC
        {
            gain = READ_FIELD(HAL_DFE_GDDATGMON);
        }
        else                            //AFE Controls RF AGC
        {
            gain = READ_FIELD(HAL_AFE_RFAGCREG);
        }

        if(READ_FIELD(HAL_AFE_RFAOCEN)) //DFE Controls RF AOC
        {
            offset = READ_FIELD(HAL_DFE_ODDATGMON);
        }
        else                            //AFE Controls RF AOC
        {
            offset = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);
        }
        modulation = (LONG)svoRRFVar.step_size[gain] * (offset - svoRRFVar.ofs_ldoff_cur);
        modulation = RoundDivide(modulation,256);//modulation = dark_level
#if (NEW_RFPOS_CALIB == 1)
        modulation += svoRRFVar.rfagc_ref;
#endif
        if (meas_signal.top_mv > (modulation + 50))
        {
            modulation = meas_signal.p2p_mv * 100 / (meas_signal.top_mv - modulation);
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x33FF10); //OPU RF no output!
            modulation = 60;
        }
    }

    if (((svoRRFVar.modulation[Layer_Index] > 0) && (modulation < (svoRRFVar.modulation[Layer_Index]/2))) ||
        ((svoRRFVar.modulation[Layer_Index] == 0) && (modulation < (COEF_GetValue(DFE_CALC_M)/2))))
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x316695,modulation);
		#if (Customer_A1 ==1)
	     	PositionHomeErrorCount++;	
		#endif		
        return(FALSE);
    }
    else
    {
        return(TRUE);
    }
}
#endif // (OPEN_SESSION == 1)

/*********************************************************************
* Module: GetOpuTemperature()
* Description:  Get Temperature on OPU
* Returns: Degree Celcius (-128~127)
* Params:
*********************************************************************/
SBYTE GetOpuTemperature(void)
{
    SHORT  Temperature;

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    Temperature = ReadAverageAdcmV(ADC_GPADCO1, 6);
    if (Temperature == GPADC_IN_MIN)
        Temperature = 25;
    else
        Temperature = (84021 - 37 * Temperature) / 1024;
#elif (PCB == 815)
    Temperature = ReadAverageAdcmV(ADC_GPADCO1, 6);
    //send_msg74(SHOW_DEBUG_MSG,0xEE001D,Temperature);

    if (Temperature == GPADC_IN_MIN)
        Temperature = 25;
    else
        Temperature = (94263 - 54 * Temperature) / 1024;
#else
    Temperature = 25;
#endif
    Temperature = (Temperature > 127)? 127 : Temperature;
    Temperature = (Temperature < -128)? -128 : Temperature;

    return (SBYTE)Temperature;
}

/*-----------------------------------------------------------------------------
 *
 * Module: linear_interpolation()
 * Description: do lineair interpolation
 *
 *----------------------------------------------------------------------------*/
float linear_interpolation(float point_x1 , float point_y1 , float point_x2 , float point_y2 , float point_x)
{
    float point_y;

    if (point_x2 != point_x1) // Division by zero check
    {
        point_y = point_y1 + (point_y2 - point_y1) * (point_x - point_x1) / (point_x2 - point_x1);
    }
    else
    {
        point_y = point_y1;
    }

    return point_y;
}

/*-----------------------------------------------------------------------------
 *
 * Module: curve_fit_data_filter()
 * Description: Sorting the samples and making the curve asymmetrical by adjust the boundary sample.
 *
 *----------------------------------------------------------------------------*/
BOOL curve_fit_data_filter(float *cfx, float *cfy, BYTE *cfsize_in, float limit_y, BOOL up_curve)
{
    BOOL  symmetry = FALSE;
    BYTE  i, j, k, index_start, index_count;
    float temp_x, temp_y;

    //make it up_curve
    if (!up_curve)
    {
        limit_y *= -1;
        for (i = 0; i < *cfsize_in; i++)
        {
            cfy[i] *= -1;
        }
    }

    //find points fit limit_y
    // example:
    // 13 point: y0 y1 y2 y3 y4 y5 y6 y7 y8 y9 y10 y11 y12
    // y0 y1 y9 y10 > limit_y
    // continue under limit_y (y2 y3 y4 y5 y6 y7 y8) number=7 is bigger than number=2 (y11 y12)
    // keep y1 y2 y3 y4 y5 y6 y7 y8 y9 to curvefitting
    j = k = index_start = 0;
    index_count = 0xFF;
    for (i = 0; i < *cfsize_in; i++)
    {
        if ((cfy[i] >= limit_y) || (i == *cfsize_in - 1))
        {
            if (k > index_count)
            {
                index_start = j;
                index_count = k;
            }
            j = i;
            k = 1;
        }
        else
        {
            if (index_count == 0xFF)
            {
                index_count = 0;
            }
            k++;
        }
    }

    *cfsize_in = index_count + 1;
    for (i = 0; i < *cfsize_in; i++)
    {
        cfx[i] = cfx[i + index_start];
        cfy[i] = cfy[i + index_start];
    }

    //make the curve symmetrical
    // example:
    // 10 point: y0 y1 y2 y3 y4 y5 y6 y7 y8 y9
    // start point:y0 end point:y9 and y9 > y0
    // if y9 > MAX(y0,limit_y) ==> y9' = linear_interpolation(y8,y9)
    if (*cfsize_in > 0)
    {
        if (cfy[0] < cfy[*cfsize_in - 1])
        {
            temp_y = cfy[0];
            if (temp_y < limit_y)
            {
                temp_y = limit_y;
            }
            else
            {
                symmetry = TRUE;
            }
            i = *cfsize_in - 1;
            j = *cfsize_in - 2;
        }
        else
        {
            temp_y = cfy[*cfsize_in - 1];
            if (temp_y < limit_y)
            {
                temp_y = limit_y;
            }
            else
            {
                symmetry = TRUE;
            }
            i = 0;
            j = 1;
        }

        if (cfy[i] > temp_y)
        {
            temp_x = linear_interpolation(cfy[j], cfx[j], cfy[i], cfx[i], temp_y);
            cfy[i] = linear_interpolation(cfx[j], cfy[j], cfx[i], cfy[i], temp_x);
            cfx[i] = temp_x;
        }
    }

    //restore original curve
    if (!up_curve)
    {
        for (i = 0; i < *cfsize_in; i++)
        {
            cfy[i] *= -1;
        }
    }

    return symmetry;
}

/*-----------------------------------------------------------------------------
 *
 * Module: calib_data_analyser()
 * Description: analyse existing samples and decide the value of the next sample
 *
 *----------------------------------------------------------------------------*/
RAMP_MODE_t calib_data_analyser( float *cfx, float *cfy, BYTE cfsize_in, BYTE min_sample, float min_x, float max_x, USHORT min_delta, BOOL up_curve)
{
    ULONG i, j;
    float temp_x;
    float temp_y, min_y;
    BYTE  min_rank = 0,max_rank = 0;
    BOOL  min_x_reached,max_x_reached;
    RAMP_MODE_t result;

    //make it up_curve
    if (!up_curve)
    {
        for (i = 0; i < cfsize_in; i++)
        {
            cfy[i] *= -1;
        }
    }

    //sort measured data. x:small --> big
    for (i = 1; i < cfsize_in; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (cfx[i] < cfx[j])
            {
                temp_x = cfx[i];
                temp_y = cfy[i];
                cfx[i] = cfx[j];
                cfy[i] = cfy[j];
                cfx[j] = temp_x;
                cfy[j] = temp_y;
            }
        }
    }

    //check how good are the max and min measured settings.(y axis)
    min_y = cfy[0];
    for (i = 0; i < cfsize_in; i++)
    {
        if (cfy[0] > cfy[i])
        {
            min_rank++;
        }
        if (cfy[cfsize_in - 1] > cfy[i])
        {
            max_rank++;
        }
        if (cfy[i] < min_y)
        {
            min_y = cfy[i];
        }
    }

    //to form a curve it need 3 sample point at least
    if (min_sample < 2)
    {
        min_sample = 0;
    }
    else
    {
        min_sample -= 2;//except start and end point how many points still needed to form a curve
    }

    //if cfx[0] or cfx[cfsize_in - 1] over limit and optimum is near that
    //check if samples are enough or not
    min_x_reached = max_x_reached = FALSE;
    if (cfx[0] <= min_x)
    {
        min_x_reached = TRUE;
        if (min_rank < max_rank)
        {
            min_sample++;
        }
    }
    if (cfx[cfsize_in - 1] >= max_x)
    {
        max_x_reached = TRUE;
        if (min_rank > max_rank)
        {
            min_sample++;
        }
    }

    //real curve depth must be bigger than desired curve depth and sample number must be enough ==> finished
    if ((min_x_reached || ((min_rank >= min_sample) && (cfy[            0] >= min_y + min_delta))) &&
        (max_x_reached || ((max_rank >= min_sample) && (cfy[cfsize_in - 1] >= min_y + min_delta))))
    {
        // finished
        if ((min_rank < min_sample) && (max_rank < min_sample))
        {
            result = RAMPING_FAIL;
        }
        else
        {
            result = RAMPING_END;
        }
    }
    else
    {
        if ((cfx[cfsize_in - 1] < max_x) && ((max_rank < min_rank) || (cfx[0] == min_x) || ((max_rank == min_rank) && ((cfx[0] + cfx[cfsize_in - 1]) < (min_x + max_x)))))
        {
            result = RAMPING_UP;
        }
        else if (cfx[0] > min_x)
        {
            result = RAMPING_DOWN;
        }
        else
        {
            result = RAMPING_FAIL;
        }
    }

    //restore original curve
    if (!up_curve)
    {
        for (i = 0; i < cfsize_in; i++)
        {
            cfy[i] *= -1;
        }
    }

    return result;
}

/*-----------------------------------------------------------------------------
 *
 * Module: enableRTJitterMeas()
 * Description: Program the registers to enable real time jitter measurement
 *
 *----------------------------------------------------------------------------*/
void enableRTJitterMeas(void)
{
// TBD? Empty Function!
}

/*-----------------------------------------------------------------------------
 *
 * Module: disableRTJitterMeas()
 * Description: Program the registers to disable real time jitter measurement
 *
 *----------------------------------------------------------------------------*/
void disableRTJitterMeas(void)
{
// TBD? Empty Function!
}

/*-----------------------------------------------------------------------------
 *
 * Module: convertRTJitter()
 * Description: convert jitter value to 0.1% units
 *              Jitter (%) = 100 * sqrt((FPLLJIT * 16 - 10.52) / 131072) (theoretical)
 * Input: average of register FPLLJIT * 8
 *
 *----------------------------------------------------------------------------*/
USHORT convertRTJitter(ULONG reg_value_x8)
{
    USHORT jitter;
    jitter = (USHORT)(sqrt(reg_value_x8 * 62500 - 328750) / 64);
    if (jitter > 325)
        jitter = 325;
    return jitter;
}


#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
/*-----------------------------------------------------------------------------
 *
 * Module: Set_TOK_Mode
 * Description: Set TOK to two different mode
 * Mode1: Seek mode: TOK detects track OK/Not
 * Mode2: Play mode: Force TOK to OK
 *
 *----------------------------------------------------------------------------*/
void Set_TOK_Mode(BYTE TOK_mode)
{
    if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {
        if (TOK_mode == TOK_SEEK)
        {
            #if (CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_300mV);  //DVD Seek
            #else
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_300mV);
            #endif
           //send_msg80(DEBUG_SEEK_MSG|DEBUG_AUTO_TUNING_MSG|DEBUG_DISK_ID_MSG,0x534600);      // TOK Mode: Seek Mode
        }
        else
        {
            #if (CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_1125mV);  //DVD Play
            #else
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_1100mV);
            #endif
          //send_msg80(DEBUG_SEEK_MSG|DEBUG_AUTO_TUNING_MSG|DEBUG_DISK_ID_MSG,0x534601);      // TOK Mode: Play Mode
        }
    }
    else
    {
        if (TOK_mode == TOK_SEEK)
        {
            #if (CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_225mV);  // CD Seek
            #else
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_250mV);
            #endif
          //send_msg80(DEBUG_SEEK_MSG|DEBUG_AUTO_TUNING_MSG|DEBUG_DISK_ID_MSG,0x534600);      // TOK Mode: Seek Mode
        }
        else
        {
            #if (CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_1125mV); // CD Play
            #else
            WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_1100mV);
            #endif
          //send_msg80(DEBUG_SEEK_MSG|DEBUG_AUTO_TUNING_MSG|DEBUG_DISK_ID_MSG,0x534601);      // TOK Mode: Play Mode
        }
    }
}
#endif



#if (DEFECT_LENGTH_MEASURE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: libEnableDFCTInt()
 * Description: Enable defect length interrupt
 *
 *----------------------------------------------------------------------------*/
void libEnableDFCTInt(void)
{
#if (DVD_RAM_READ == 1)
    if(DVDRAM)
        return;
#endif
        WRITE_FIELD(HAL_SRV_DLINTR,0);   /* Clear defect length interrupt */
        WRITE_FIELD(HAL_SRV_DLINTEN,1);  /* Enable defect length interrupt */

}

/*-----------------------------------------------------------------------------
 *
 * Module: libDisbleDFCTInt()
 * Description: Enable defect length interrupt
 *
 *----------------------------------------------------------------------------*/
void libDisableDFCTInt(void)
{
#if (DVD_RAM_READ == 1)
    if(DVDRAM)
        return;
#endif
    WRITE_FIELD(HAL_SRV_DLINTR, 0);         /* Clear defect length interrupt */
    WRITE_FIELD(HAL_SRV_DLINTEN, 0);           /* Disable defect length interrupt */


    }

#endif // #if (DEFECT_LENGTH_MEASURE == 1)

/*********************************************************
* Module: SetDfeWblFrontOnHold()
*
* Description: Set DFE on hold:
*     Hold data slicer, disable agc/aoc, and disable defect
*     detection.
*     This function should be called before a seek or
*     opening tracking loop
*
* Returns: void
* Params:  void
**********************************************************/
void SetDfeWblFrontOnHold(void) // before_non_tracking
{
    SendMsg80(DEBUG_DFE_MSG,0x3B1400);//SetDfeWblFrontOnHold function performed
    HAL_DFE_ResetFastPll(halDFE_FastPllReset);                  /* Reset Fast PLL */
    HAL_DFE_EnDisDataSlicer(halDFE_DataSlicerDisable);          /* Disable Data Slicer  */
    HAL_DFE_EnableFlywheels(FALSE);

#if (EN_BD_ADEQ == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (READ_FIELD(HAL_DFE_ADEQDIS) == 0))
    {
        InitAdaptEq();//turn off ADEQ
    }
#endif


#if (EN_ADEQ == 1)
   if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW)) && (!DVDRAM)
   && (READ_FIELD(HAL_DFE_ADEQDIS) == 0) && (Disable_ADEQ == 0))
    {
        InitAdaptEq();//turn off ADEQ
    }
#endif


#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if ((svoVar_discKindT.Bit.Media == eMediaROM)
        #if (DVD_RAM_READ == 1)
        || (DVDRAM_embossed)
        #endif
    #else
    if (((svoVar_discKindT.Bit.DiscStd != eDiscBD) || (svoVar_discKindT.Bit.Media == eMediaROM))
        #if (DVD_RAM_READ == 1)
        && (!DVDRAM_rewritable)
        #endif
    #endif
    )
#endif
    {
        HAL_DFE_HoldRfPll(halPLL_RfPllHold);                    /* RF PLL on hold */
    }
#if (EN_WOBBLE == 1)
    else
    {
        HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Disable);       /* Disable Wobble AGC */
        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);           /* Wobble PLL on hold */
    }
#endif

#if (BD_ENABLE == 1)
    if (svo_BDR_RE_PICReading == SET)
    {
        HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Disable);       /* Disable Wobble AGC */
        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);           /* Wobble PLL on hold */
    }
#endif

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            pdbSetSubCodeOK(FALSE);
#if (EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (svoVar_discKindT.Bit.Media != eMediaROM)
            {
                HAL_WOBBLE_EnableFlywheels(FALSE);
                pdbSetAtipOk(FALSE);
            }
#endif
            break;

#if (ENABLE_HDDVD == 1) //current HD not support wobble read
        case eDiscHD:
#endif
#if (BD_ENABLE == 1)
        case eDiscBD:
#endif
        case eDiscDVD:
#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 0)
            if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM))
    #elif (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 1)
            if (((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM)) || DVDRAM_rewritable)
    #elif (DVD_RAM_READ == 0)
            if (svoVar_discKindT.Bit.Media != eMediaROM)
    #else
            if ((svoVar_discKindT.Bit.Media != eMediaROM) && !(DVDRAM_embossed))
    #endif
            {
                //disable wobble_address_ok_flag
                pdbSetIDOk(FALSE);
                HAL_WOBBLE_EnableFlywheels(FALSE);
    #if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    //SBYTE RfOffsetPeakPeak;

                    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable);  /* Disable RF AGC/AOC */
                    //RfOffsetPeakPeak = calDacCount2SBYTE(svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset,8);
                    //HAL_DFE_SetRfAocCalOffset(RfOffsetPeakPeak, RfOffsetPeakPeak);
                }
    #endif
            }
#endif // #if (EN_WOBBLE == 1)
            break;

        default:
            break;
    }
}

/*********************************************************
* Module: SetDfeWblFrontOnHoldTracking()
*
* Description: Unhold the semi-synchronous PLL and
*              reset Fast PLL and  bypass the HPF
*              in the processing path.
*              It is called when there is no seek or jump,
*              the data slicer , AGC's, AOC's can still work.
*              This function should be called in a recovery
*              or in a speed change without opening tracking loop
* Returns: void
* Params:  void
**********************************************************/
void SetDfeWblFrontOnHoldTracking(void)
{
    SendMsg80(DEBUG_DFE_MSG,0x3B1401);//SetDfeWblFrontOnHoldTracking function performed
    HAL_DFE_ResetFastPll(halDFE_FastPllReset);                  /* Reset Fast PLL */
    HAL_DFE_EnableFlywheels(FALSE);

#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if ((svoVar_discKindT.Bit.Media == eMediaROM)
        #if (DVD_RAM_READ == 1)
        || (DVDRAM_embossed)
        #endif
    #else
    if (((svoVar_discKindT.Bit.DiscStd != eDiscBD) || (svoVar_discKindT.Bit.Media == eMediaROM))
        #if (DVD_RAM_READ == 1)
        && (!DVDRAM_rewritable)
        #endif
    #endif
    )
#endif
    {
        HAL_DFE_HoldRfPll(halPLL_RfPllHold);                    /* RF PLL on hold */
        HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass);              /* Bypass HPF */
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            pdbSetSubCodeOK(FALSE);
#if (EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (svoVar_discKindT.Bit.Media != eMediaROM)
            {
                HAL_WOBBLE_EnableFlywheels(FALSE);
                pdbSetAtipOk(FALSE);
            }
#endif
            break;

        case eDiscDVD:
#if (BD_ENABLE == 1)
        case eDiscBD:
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
#endif
#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 0)
            if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM))
    #elif (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 1)
            if (((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media != eMediaROM)) || DVDRAM_rewritable)
    #elif (DVD_RAM_READ == 0)
            if (svoVar_discKindT.Bit.Media != eMediaROM)
    #else
            if ((svoVar_discKindT.Bit.Media != eMediaROM) && !(DVDRAM_embossed))
    #endif
            {
                //disable wobble_address_ok_flag
                pdbSetIDOk(FALSE);
                HAL_WOBBLE_EnableFlywheels(FALSE);
            }
#endif
            break;

        default:
            break;
    }
}

/*********************************************************
* Module: SetDfeWblFrontSemiSynchr()
*
* Description:
*        Unhold data slicer,enable agc/aoc,data slicer.
*        Action on after seek.
*        This function should be executed as soon as
*        tracking loop is closed.
* Returns: void
* Params:  void
**********************************************************/
void SetDfeWblFrontSemiSynchr(void) // after_non_tracking
{
#if (EN_WOBBLE == 1)
    halWBL_ATIPPLLSettingT  ATIPPLLSetting;
#endif
#if (BD_ENABLE == 1)
    ULONG  ClkKHz;
    ULONG  InpFcN1Hz;
#endif

    SendMsg80(DEBUG_DFE_MSG,0x3B1402);//SetDfeWblFrontSemiSynchr function performed

    if ((svoVar_discKindT.Bit.Media == eMediaROM)
#if (DVD_RAM_READ == 1)
        || (DVDRAM_embossed)
#endif
#if (BD_ENABLE == 1)
        || (svo_BDR_RE_PICReading == SET)
#endif
    )
    {
        HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);               /* Enable RF AGC/AOC */
#if (BD_ENABLE == 1)
        if (svo_BDR_RE_PICReading == SET)
        {
            ClkKHz = HAL_PLL_GetDfeInClkKHz();
            InpFcN1Hz = COEF_GetValue(DFE_CALC_HFHighpass_FcN1_EMBOSSED_PIC); /* Get PIC coef */
            HAL_DFE_SetIIRHPFBw(InpFcN1Hz, svo_bReadZoneSpeedX10, ClkKHz);
            HAL_DFE_IIRHPFByPass(halDFE_IirHpfNotBypass);
        }
        else
#endif
        {
            HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass);              /* Bypass HPF */
        }
        HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
        HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
        HAL_DFE_PushOutOfLock();

#if (BD_ENABLE == 1)
        if (svo_BDR_RE_PICReading == SET)
        {
            HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Enable);        /* Enable Wobble AGC */
            LAL_WOBBLE_SetWobbleFreq(svo_bReadZoneSpeedX10);
        }
#endif
    }
    else
    {
        if (HAL_GLOBAL_GetRfPresent() || (DVDRAM))
        {
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);           /* Enable RF AGC/AOC */
#if (DVD_RAM_READ == 1)
            //if (DVDRAM)
            //{
            //    HAL_DFE_SetRfAocCalOffset(calDacCount2SBYTE(svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset,8), RfHeaderOffset);
            //}
#endif
        }
#if (EN_WOBBLE == 0)
        HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass);              /* Bypass HPF */
        HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
        HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
        HAL_DFE_PushOutOfLock();
#else // #if (EN_WOBBLE == 0)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        if ((svoVar_discKindT.Bit.DiscStd != eDiscBD)
        #if (DVD_RAM_READ == 1)
            && (!DVDRAM_rewritable)
        #endif
        )
        {
            HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass);              /* Bypass HPF */
            HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
            HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
            HAL_DFE_PushOutOfLock();
        }
        else
    #endif
        {
            HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Enable);        /* Enable Wobble AGC */

            if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            {
                ATIPPLLSetting.CDATIPPhaseGain = COEF_GetValue( WBL_CALC_CDATIPPhaseGain );
                ATIPPLLSetting.DTOOn           = COEF_GetValue( WBL_CALC_DTOOn );
                ATIPPLLSetting.CDATIPLPF       = COEF_GetValue( WBL_CALC_CDATIPLPF );
                ATIPPLLSetting.CDATIPKP        = COEF_GetValue( WBL_CALC_CDATIPKP );
                HAL_WOBBLE_SetATIPPLL(&ATIPPLLSetting);
            }

            LAL_WOBBLE_SetPLL(eHighGainMode);
            HAL_WOBBLE_EnDisLockAid(TRUE);

            WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModePresetFromWobble);     /* Set the semi-synchronous PLL from wobble */
    #if (DVD_RAM_READ == 1)
            if(DVDRAM_rewritable)
            {
                USHORT overspeedX100;

                overspeedX100 = spdGetDVDRAMOverspeedX100(target_zone);

                if((WblPllFreqPreset == 0) || zone_change ||
                   ((overspeedX100 * 10) > (svo_bReadZoneSpeedX10 * 105)) ||   // greater than 5%
                   ((overspeedX100 * 10) < (svo_bReadZoneSpeedX10 * 95)))      // less than 5%
                {
                    overspeedX100 = overspeedX100*11/10;

                    if (overspeedX100 > svo_bReadZoneSpeedX10*22)
                        overspeedX100 = svo_bReadZoneSpeedX10*22;

                    if (overspeedX100 < svo_bReadZoneSpeedX10*5)
                        overspeedX100 = svo_bReadZoneSpeedX10*5;

                    LAL_WOBBLE_SetWobbleFreq(overspeedX100/10);
                    WblPllFreqPreset = READ_FIELD(HAL_WBL_WBLFREQ);
                }
                else
                {
                    WRITE_FIELD(HAL_WBL_WBLFREQ, WblPllFreqPreset);
                }
                HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);         /* Unhold Wobble PLL */
            }
            else
    #endif
            {
                LAL_WOBBLE_SetWobbleFreq(svo_bReadZoneSpeedX10);
                HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);         /* Unhold Wobble PLL */
                ENABLE_WBLINT();
            }
        }
#endif // #if (EN_WOBBLE == 0)
    }
    //HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);               /* Enable HF defect */
    HAL_DFE_EnDisDataSlicer(halDFE_DataSlicerEnable);           /* Enable Data Slicer */
}

/*********************************************************
* Module: SetDfeWblFrontSemiSynchrAfterTracking()
*
* Description:  Unhold the semi-synchronous PLL and
*               push out of lock.
*               This function should be executed if
*               semi_synchronous PLL  is locked and the
*               tracking loop wasn't open before this locking
*
* Returns: void
* Params:  void
**********************************************************/
void SetDfeWblFrontSemiSynchrAfterTracking(void)
{
#if (EN_WOBBLE == 1)
    halWBL_ATIPPLLSettingT  ATIPPLLSetting;
#endif //(EN_WOBBLE == 1)

    SendMsg80(DEBUG_DFE_MSG,0x3B1403);//SetDfeWblFrontSemiSynchrAfterTracking function performed
    /* Preset the HF PLL */
    if ((svoVar_discKindT.Bit.Media == eMediaROM)
#if (DVD_RAM_READ == 1)
        || (DVDRAM_embossed)
#endif
#if (BD_ENABLE == 1)
        || (svo_BDR_RE_PICReading == SET)
#endif
    )
    {
        HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
        HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
        HAL_DFE_PushOutOfLock();
    }
    else
    {
#if (EN_WOBBLE == 0)
        HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
        HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
        HAL_DFE_PushOutOfLock();
#else // #if (EN_WOBBLE == 0)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        if ((svoVar_discKindT.Bit.DiscStd != eDiscBD)
        #if (DVD_RAM_READ == 1)
            && (!DVDRAM_rewritable)
        #endif
        )
        {
            HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
            HAL_DFE_HoldRfPll(halPLL_RfPllUnhold);                  /* Set the semi-synchronous PLL Automatic mode */
            HAL_DFE_PushOutOfLock();
        }
        else
    #endif
        {
            if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            {
                ATIPPLLSetting.CDATIPPhaseGain = COEF_GetValue( WBL_CALC_CDATIPPhaseGain );
                ATIPPLLSetting.DTOOn           = COEF_GetValue( WBL_CALC_DTOOn );
                ATIPPLLSetting.CDATIPLPF       = COEF_GetValue( WBL_CALC_CDATIPLPF );
                ATIPPLLSetting.CDATIPKP        = COEF_GetValue( WBL_CALC_CDATIPKP );
                HAL_WOBBLE_SetATIPPLL(&ATIPPLLSetting);
            }

            LAL_WOBBLE_SetPLL(eHighGainMode);
            HAL_WOBBLE_EnDisLockAid(TRUE);

            WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModePresetFromWobble);      /* Set the semi-synchronous PLL from wobble */

    #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                USHORT overspeedX100;

                overspeedX100 = spdGetDVDRAMOverspeedX100(target_zone);
                overspeedX100 = overspeedX100*11/10;

                if (overspeedX100 > svo_bReadZoneSpeedX10*22)
                    overspeedX100 = svo_bReadZoneSpeedX10*22;

                if (overspeedX100 < svo_bReadZoneSpeedX10*5)
                    overspeedX100 = svo_bReadZoneSpeedX10*5;

                LAL_WOBBLE_SetWobbleFreq(overspeedX100/10);
            }
            else
    #endif
            {
                LAL_WOBBLE_SetWobbleFreq(svo_bReadZoneSpeedX10);
            }
            HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);         /* Unhold Wobble PLL */
        }
#endif // #if (EN_WOBBLE == 0)
    }
}

/*********************************************************
* Module: SetDfeWblFrontBitSynchr()
*
* Description: unreset Fast PLL, enable Black and white
*                 defect detection and put back the HPF
*                 in the processing path.
*                 This function should be executed as soon
*                 as semi synchronous PLL is locked and the
*                 tracking loop was open before this locking.
* Returns: void
* Params:  void
**********************************************************/
void SetDfeWblFrontBitSynchr(void)
{
    SendMsg80(DEBUG_DFE_MSG,0x3B1404);//SetDfeWblFrontBitSynchr function performed
    HAL_DFE_ResetFastPll(halDFE_FastPllDefault);                /* Fast PLL to default*/
    HAL_DFE_FreezeFastPll(halDFE_FastPllUnFreeze);              /* not freeze fpll */

#if DFE_DEFECT_DETECTOR_ENABLE == 1
    //add defect on here because we can not enable it in state_fok_chk (DFE defect det not enabled yet)
    Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
#endif

    if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)||(svoVar_discKindT.Bit.DiscStd == eDiscCD)
#if (BD_ENABLE == 1)
        || (svoVar_discKindT.Bit.DiscStd == eDiscBD)
#endif
        )
    {
        HAL_DFE_IIRHPFByPass(halDFE_IirHpfNotBypass); /* Use HPF for DVD & BD */
    }
    else
    {
        HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass);  /*  bypass HPF for others */
    }

    HAL_DFE_EnableFlywheels(TRUE);

    //set normal bandwidth to the wobble PLL
#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 0)
    if ((svoVar_discKindT.Bit.Media != eMediaROM) && (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    #elif (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 1)
    if (((svoVar_discKindT.Bit.Media != eMediaROM) && (svoVar_discKindT.Bit.DiscStd == eDiscBD) || DVDRAM_rewritable)
    #elif (DVD_RAM_READ == 0)
    if ((svoVar_discKindT.Bit.Media != eMediaROM)
    #else
    if ((svoVar_discKindT.Bit.Media != eMediaROM) && !(DVDRAM_embossed)
    #endif
    #if (BD_ENABLE == 1)
       && (svo_BDR_RE_PICReading == CLEAR)
    #endif
    )
    {
        HAL_WOBBLE_EnDisLockAid(FALSE);

        // PLL settings should not be set in case of BD(RE) PIC read
        LAL_WOBBLE_SetPLL(eNormalGainMode);

        // enable flywheel
        HAL_WOBBLE_EnableFlywheels(TRUE);
    }
#endif // #if (EN_WOBBLE == 1)

    // include svoEnTimeRead in ClearAddressRead ????????
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            svoEnTimeRead();
            break;

        default:
            ClearAddressRead();
            break;
    }
}

/*********************************************************
* Module: EnableWobbleAdressReading()
*
* Description:  enable wobble address reading
                This function should be executed every time
*               after wobble PLL is in lock.
* Input:        OnOff: Is true if wobble address reading
*       should be enabled, id false otherwise.
* Returns: void
**********************************************************/
void EnableWobbleAdressReading(BOOL OnOff) // TBD? __inline
{
    HAL_WOBBLE_ClearAddressBuf();
    HAL_WOBBLE_EnableNewWblAddInterrupt(OnOff);
}

/*********************************************************
* Module: svoStartUpWrongLayer()
*
* Description: handling startup focus on wrong layer
* Input: void
* Returns: void
**********************************************************/
void svoStartUpWrongLayer(void)
{
    BYTE temp_gain;

    track_off();
    svoFocusOff();
    Recorded_stp_Position[Layer_Index] = svoStepVar.StepHomePos;
    svoCalVar.discChkDoneBits &= (~blank_chk_end);
    svoCalVar.calGainDoneBits &= (~(tebg_end | ceg_end | teg_end | rpg_end | dif_RF_end | dif_WOB_end | felg_end | telg_end));

    if (svoCalVar.calDoneBits & ((Layer_Index == 1)? L1_blank_det : L0_blank_det))
    {
        temp_gain = svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDL];
        svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDH] =
        svoCalGnT1Var[Layer_Index][FE_GN].land_gain[RECDH] = temp_gain;
        svoCalGnT2Var[Layer_Index][BS_GN].normal_gain = svoCalGnT2Var[Layer_Index][BS_GN].write_gain;
    }
    else
    {
        temp_gain = svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDH];
        svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDL] =
        svoCalGnT1Var[Layer_Index][FE_GN].land_gain[RECDL] =
        svoCalGnT1Var[Layer_Index][FE_GN].write_gain = temp_gain;
        svoCalGnT2Var[Layer_Index][BS_GN].write_gain = svoCalGnT2Var[Layer_Index][BS_GN].normal_gain;
    }

    if (Layer_Index == 1)
    {
        svoCalVar.calDoneBits &= (~L1_blank_det);
        calClrCalResult(FEBC_DSP,TRUE);
#if COLLI_SWITCH ==1
        calClrCalResult(COLLI_POS,TRUE);
#endif
    }
    else
    {
        svoCalVar.calDoneBits &= (~L0_blank_det);
        calClrCalResult(FEBC_DSP,FALSE);
#if COLLI_SWITCH ==1
        calClrCalResult(COLLI_POS,FALSE);
#endif
    }
#if (COLLI_SWITCH == 1)
    if (calGetCalRequired(Layer_Index, Iram_stp_CurrentPosition, COLLI_POS, READ_MODE) == TRUE)
    {
        svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(Layer_Index));
    }
#endif
}

/*********************************************************
* Module: SvoDiameterJudge()
*
* Description:
* Input:    Mode 0: quick kick, wake up kick
            Mode > 0: disckick judge nodisc/8cm/12cm
* Returns:  delta rpm per 100ms
            time out returns 0x8000
**********************************************************/
#define GET_DIAM_JUDGE_DATA     0       // 1 : correct  data for  judge threshold  0 : default
SHORT SvoDiameterJudge(BYTE mode)
{
#if (CHIP_REV > 0xB3)
    ULONG  cur_cavm;
#endif
    ULONG  dia_timer;
    float  delta_rpm_ms;          //delta RPM v.s ms
    USHORT pre_fgCount;
    USHORT temp_count;
    BYTE   temp_index;

#if (CHIP_REV > 0xB3)
    cur_cavm = READ_FIELD(HAL_SRV_MDIV);
#endif

    pre_fgCount = 0xFFFF;
    temp_count = 0;
    svoSpeedVar.fgCount = 0xFFFF;

    StartFGTimer();
    StartTimer(&dia_timer);
    while (1)
    {
        if (pre_fgCount != svoSpeedVar.fgCount)
        {
            pre_fgCount = svoSpeedVar.fgCount;

            if (pre_fgCount <= (0xFFFF - DISK_NFGPLS))
            {
                temp_count++;
                temp_index = temp_count % 20;
                svoCurvefitCalVar.item[temp_index] = (float)(cur_cavm * svoSpeedVar.fgTotalTime) / (SERVO_CLOCK / (1000 * (DISK_FR2X + 1)));    //ms
                svoCurvefitCalVar.cost[temp_index] = (float)(SERVO_CLOCK * 60L / (DISK_FR2X + 1)) / (cur_cavm * svoSpeedVar.fg_pulse_rev);      //RPM
#if (GET_DIAM_JUDGE_DATA ==1)
                send_msg74(1,0x090000,svoCurvefitCalVar.item[temp_index]);send_msg74(1,0x090001,svoCurvefitCalVar.cost[temp_index]);
#endif
                if (temp_count >= 20)
                {
                    LineFit(20, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
                    delta_rpm_ms = svoCurvefitCalVar.cof[1];
                    if (mode == 0)
                    { //quick kick
#if (GET_DIAM_JUDGE_DATA ==0)
                        SendMsgCn(SHOW_DEBUG_MSG,4,0x310008,A4B(delta_rpm_ms));//QUICKKICK disc acceleration = %3.1f RPM/mS
                        break;
#endif
                    }
                    if (svoCurvefitCalVar.cof[0] < (svoCurvefitCalVar.cost[temp_index] * 0.5))
                    {
                        CurveFit(20, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
                        if (ABS((svoCurvefitCalVar.cof[2] * 1024) / (1 + delta_rpm_ms * delta_rpm_ms)) < (DISC_8CM_TH * DISC_8CM_TH / 18))
                        {
#if (GET_DIAM_JUDGE_DATA == 0)
                            SendMsgCn(SHOW_DEBUG_MSG,4,0x310009,A4B(delta_rpm_ms));//DISKKICK disc acceleration = %3.1f RPM/mS
                            break;
#endif
                        }
                    }
                }
            }
            ExitProcess();
        }
        if (ReadTimer(&dia_timer) >= DISCK_TOUT)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x31000B+mode*2);     //Warning: Quickkick/DiscKick Timeout
            break;
        }
    }

    if (ReadTimer(&dia_timer) >= DISCK_TOUT)
    {
        if (temp_count < 20)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x310005);     //DiscKICK  too slow
            return ((SHORT)0x8000);
        }
        else
        {
            delta_rpm_ms = svoCurvefitCalVar.cost[temp_index]*sqrt((ULONG)svoCurvefitCalVar.cost[temp_index]) / ( 14 * svoCurvefitCalVar.item[temp_index]);
        }
    }
    return float2short(delta_rpm_ms * 100);
}

#if (DISC_MIS_JUDGE_RECOVERY)
/*********************************************************
* Module: svoGetRetryMediaType()
*
* Description:
* Input:
* Returns:
**********************************************************/
BYTE svoGetRetryMediaType(void)
{
    BYTE    i, media_type;

    media_type = eMediaUnknown;
//    send_msg5SValue(1,"start_end",1,(svoCalVar.discChkDoneBits & start_end));
//    send_msg5SValue(1,"Iram_Svr_player_mode",1,Iram_Svr_player_mode);
    if ((svoCalVar.discChkDoneBits & start_end) || Iram_Svr_player_mode!= EXECUTING_START_UP_MODE)
        return media_type;

    svoMiscVar.retryMediaType &= (~(1<<svoVar_discKindT.Bit.Media));
    if (svoMiscVar.retryMediaType)
    {
        for (i = 1; i < RetryMediaTypeLast; i++)
        {
            if ((svoMiscVar.retryMediaType >> i) & 0x01)
            {
                media_type = i;
                break;
            }
        }
    }

    return media_type;
}

/*********************************************************
* Module: svoReStartUp()
*
* Description:
* Input:
* Returns:
**********************************************************/
BYTE svoReStartUp(BYTE media_type)
{
    BYTE    do_restart;

    do_restart = FALSE;
#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd != eDiscBD)
    {
        media_type = eMediaUnknown;                 //temperary just retry BD disc. will support more disc later!!!
    }
    else
#endif
    if (media_type == eMediaUnknown)
    {
        media_type = svoGetRetryMediaType();
    }

    if (media_type != eMediaUnknown)
    {
#if (BD_ENABLE == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            track_off();
            HAL_LASC_SetLaserOn(FALSE);
            svoCalVar.discChkDoneBits |= disc_chk_end;
    #if (PDIC_BASE_ON_SRFO == 1)
            svoCalVar.calGainDoneBits &= ~(teg_end|felg_end|telg_end|dif_RF_end|dif_WOB_end);    // afe gain calibration done flag!!
    #else
            svoAFEInitOnly();
            svo_Adj_Data_Clr();
    #endif
            Layer_Index = 0;

            switch(media_type)
            {
                case eMediaR:
                case eMediaRW:
                    SendMsg80(SHOW_DEBUG_MSG,0x33FF3B); //Flip to BD-R/RE
                    svoVar_discKindT.Bit.Media = eMediaRW;
                    break;

                case eMediaROM:
                    SendMsg80(SHOW_DEBUG_MSG,0x33FF3C); //Flip to BD-ROM
                    svoVar_discKindT.Bit.Media = eMediaROM;
#if (ENABLE_BDRLTH == 1)
                    if (svoIramVar.LTHDisc)
                    {
                        svoIramVar.LTHDisc = 0;
                        LAL_SetAFEandDFEforLTH();
                        svoCalVar.calOfsDoneBits  &= ~(teof_end);
                    }
                    svoIramVar.LTHDisclikely = 0;
#endif
                    break;
            }
            LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
            LAL_SetReadMode();

            afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
            if (svoCalVar.calGainDoneBits & rpg_end)
            {
                svoSeekVar.rx_available = RX_AVAILABLE;
            }
            bSetSpeedAgain = TRUE;
            spdChangeDiscSpeed(SVO_CAL_SPEED_BD);
        }
        else
#endif
        if(svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
        }
        else    // svoVar_discKindT.Bit.DiscStd = eDiscCD
        {
        }

        do_restart = TRUE;
    }

    return do_restart;
}
#endif  //DISC_MIS_JUDGE_RECOVERY
#if(USE_EEPROM_COLLI_POS == 1)
/*********************************************************
* Module: UpdateColliPos()
*
* Description: Get collimator pos from EEPROM or use
*                default value
* Input:
* Returns: void
**********************************************************/
void UpdateColliPos(void)
{
    //check disc type is BD ROM or R/RE and load collimator position
    if(svoVar_discKindT.Bit.Media == eMediaRW)
    {
        svoCalColliVar[0].opt_value = (GetPersistentTable1B(COLLI_BDRRE_LO_ADR) == 0)? ((COLLI_BD_Position_Zero + COLLI_BD_R_RE_L0)*32) : (GetPersistentTable1B(COLLI_BDRRE_LO_ADR)*32);
        svoCalColliVar[1].opt_value = (GetPersistentTable2B(COLLI_BDRRE_L1_ADR) == 0)? ((COLLI_BD_Position_Zero + COLLI_BD_R_RE_L1)*32) : ((GetPersistentTable1B(COLLI_BDRRE_L1_ADR)|(GetPersistentTable1B(COLLI_BDRRE_L1_ADR+1)<<8))*32);
    }
    else
    {
        svoCalColliVar[0].opt_value = (GetPersistentTable1B(COLLI_BDROM_LO_ADR) == 0)? ((COLLI_BD_Position_Zero + COLLI_BD_ROM_L0)*32) : (GetPersistentTable1B(COLLI_BDROM_LO_ADR)*32);
        svoCalColliVar[1].opt_value = (GetPersistentTable2B(COLLI_BDROM_L1_ADR) == 0)? ((COLLI_BD_Position_Zero + COLLI_BD_ROM_L1)*32) : ((GetPersistentTable1B(COLLI_BDROM_L1_ADR)|(GetPersistentTable1B(COLLI_BDROM_L1_ADR+1)<<8))*32);
    }
}
#endif

/*------------------------------------------------------------------------------------
 *
 * Module: SaveColliPosIntoEEPROM()
 * Description: Write auto-calibrated collimator position into EEPROM(not finished)
 *              will move to HIF layer later
 *-----------------------------------------------------------------------------------*/
#if(USE_EEPROM_COLLI_POS == 1)
#define COLLI_TOLERANCE 50  //T.B.D
void SaveColliPosIntoEEPROM(void)
{
    ULONG UpdateColiDoneBits;
    USHORT COLLI_BDROM_L1_FLASH,COLLI_BDRRE_L1_FLASH;
    BYTE COLLI_BDROM_LO_FLASH,COLLI_BDRRE_LO_FLASH;
    eDiscKind disc;

    UpdateColiDoneBits = 0L;

    if (pdbDiscLoaded() == BD_DISC) //must startup ok and read disc structure successful
    {
        if( (svoPcalVar.CalDoneBits & bd_coli_end_mask) == bd_coli_end_mask )
        {   // all disc calibrate done already
            return;
        }

        //Get disc kind
        disc = pdbGetDiscKind();

        switch (disc)
        {
            /*----------------------------------------------
            BDROM
            --------------------------------------------- */
            case eBDROMDisc:
                if((svoPcalVar.CalDoneBits & bd_rom_colli_l0_end) == bd_rom_colli_l0_end)
                {// BD ROM disc calibrate done already
                    return;
                }
                COLLI_BDROM_LO_FLASH = (svoCalColliVar[0].opt_value)/32;
                COLLI_BDROM_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_ROM_L1 + COLLI_BDROM_LO_FLASH - COLLI_BD_ROM_L0;
                COLLI_BDRRE_LO_FLASH = COLLI_BD_Position_Zero + COLLI_BD_R_RE_L0 + COLLI_BDROM_LO_FLASH - COLLI_BD_ROM_L0;
                COLLI_BDRRE_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_R_RE_L1 + COLLI_BDROM_LO_FLASH - COLLI_BD_ROM_L0;
                UpdateColiDoneBits |= bd_rom_colli_l0_end;
                break;

            case eBDROMDLDisc:
                if((svoPcalVar.CalDoneBits & (bd_rom_colli_l0_end|bd_rom_colli_l1_end)) == (bd_rom_colli_l0_end|bd_rom_colli_l1_end))
                {// BD ROM DL disc calibrate done already
                    return;
                }
                COLLI_BDROM_LO_FLASH = (svoCalColliVar[0].opt_value)/32;
                COLLI_BDROM_L1_FLASH = (svoCalColliVar[1].opt_value)/32;
                COLLI_BDRRE_LO_FLASH = COLLI_BD_Position_Zero + COLLI_BD_R_RE_L0 + COLLI_BDROM_LO_FLASH - COLLI_BD_ROM_L0;
                COLLI_BDRRE_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_R_RE_L1 + COLLI_BDROM_L1_FLASH - COLLI_BD_ROM_L1;
                UpdateColiDoneBits |= bd_rom_colli_l0_end|bd_rom_colli_l1_end;
            break;

            /*----------------------------------------------
            BD-R/RE
            ---------------------------------------------- */
            case eBDRDisc:
            case eBDREDisc:
                if((svoPcalVar.CalDoneBits & bd_rre_colli_l0_end) == bd_rre_colli_l0_end)
                {// BD R/RE disc calibrate done already
                    return;
                }
                COLLI_BDRRE_LO_FLASH = (svoCalColliVar[0].opt_value)/32;
                COLLI_BDRRE_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_R_RE_L1 + COLLI_BDRRE_LO_FLASH - COLLI_BD_R_RE_L0;
                COLLI_BDROM_LO_FLASH = COLLI_BD_Position_Zero + COLLI_BD_ROM_L0 + COLLI_BDRRE_LO_FLASH - COLLI_BD_R_RE_L0;
                COLLI_BDROM_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_ROM_L1 + COLLI_BDRRE_LO_FLASH - COLLI_BD_R_RE_L0;
                UpdateColiDoneBits |= bd_rre_colli_l0_end;
                break;

            case eBDRDLDisc:
            case eBDREDLDisc:
                if((svoPcalVar.CalDoneBits & (bd_rre_colli_l0_end|bd_rre_colli_l1_end)) == (bd_rre_colli_l0_end|bd_rre_colli_l1_end))
                {// BD R/RE DL disc calibrate done already
                    return;
                }
                COLLI_BDRRE_LO_FLASH = (svoCalColliVar[0].opt_value)/32;
                COLLI_BDRRE_L1_FLASH = (svoCalColliVar[1].opt_value)/32;
                COLLI_BDROM_LO_FLASH = COLLI_BD_Position_Zero + COLLI_BD_ROM_L0 + COLLI_BDRRE_LO_FLASH - COLLI_BD_R_RE_L0;
                COLLI_BDROM_L1_FLASH = COLLI_BD_Position_Zero + COLLI_BD_ROM_L1 + COLLI_BDRRE_L1_FLASH - COLLI_BD_R_RE_L1;
                UpdateColiDoneBits |= bd_rre_colli_l0_end|bd_rre_colli_l1_end;
                break;
        }

        /* Set high/low bound to prevent unreasonable value */
        if( COLLI_BDROM_LO_FLASH >= (COLLI_BD_ROM_L0 + COLLI_TOLERANCE) )
            COLLI_BDROM_LO_FLASH = COLLI_BD_ROM_L0 + COLLI_TOLERANCE;
        if( COLLI_BDROM_LO_FLASH <= (COLLI_BD_ROM_L0 - COLLI_TOLERANCE) )
            COLLI_BDROM_LO_FLASH = COLLI_BD_ROM_L0 - COLLI_TOLERANCE;

        if( COLLI_BDROM_L1_FLASH >= (COLLI_BD_ROM_L1 + COLLI_TOLERANCE) )
            COLLI_BDROM_L1_FLASH = COLLI_BD_ROM_L1 + COLLI_TOLERANCE;
        if( COLLI_BDROM_L1_FLASH <= (COLLI_BD_ROM_L1 - COLLI_TOLERANCE) )
            COLLI_BDROM_L1_FLASH = COLLI_BD_ROM_L1 - COLLI_TOLERANCE;

        if( COLLI_BDRRE_LO_FLASH >= (COLLI_BD_R_RE_L0 + COLLI_TOLERANCE) )
            COLLI_BDRRE_LO_FLASH = COLLI_BD_R_RE_L0 + COLLI_TOLERANCE;
        if( COLLI_BDRRE_LO_FLASH <= (COLLI_BD_R_RE_L0 - COLLI_TOLERANCE) )
            COLLI_BDRRE_LO_FLASH = COLLI_BD_R_RE_L0 - COLLI_TOLERANCE;

        if( COLLI_BDRRE_L1_FLASH >= (COLLI_BD_R_RE_L1 + COLLI_TOLERANCE) )
            COLLI_BDRRE_L1_FLASH = COLLI_BD_R_RE_L1 + COLLI_TOLERANCE;
        if( COLLI_BDRRE_L1_FLASH <= (COLLI_BD_R_RE_L1 - COLLI_TOLERANCE) )
            COLLI_BDRRE_L1_FLASH = COLLI_BD_R_RE_L1 - COLLI_TOLERANCE;


        /* Write collimator position into EEPROM Map*/
        OpenWritableSegmentTable(ePersistent_Talbe);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

        if((svoPcalVar.CalDoneBits & bd_rom_colli_l0_end) != bd_rom_colli_l0_end)
        { // Update BD ROM L0 Coli value to EEPROM Map
            SetPersistentTable1B(COLLI_BDROM_LO_ADR,COLLI_BDROM_LO_FLASH);
            send_msg5SValue(1,"COLIROML0",2,COLLI_BDROM_LO_FLASH);
        }

        if((svoPcalVar.CalDoneBits & bd_rre_colli_l0_end) != bd_rre_colli_l0_end)
        { // Update BD R/RE L0 Coli value to EEPROM Map
            SetPersistentTable1B(COLLI_BDRRE_LO_ADR,COLLI_BDRRE_LO_FLASH);
            send_msg5SValue(1,"COLIRREL0",2,COLLI_BDRRE_LO_FLASH);
        }

        if((svoPcalVar.CalDoneBits & bd_rom_colli_l1_end) != bd_rom_colli_l1_end)
        { // Update BD ROM L1 Coli value to EEPROM Map
            SetPersistentTable2B(COLLI_BDROM_L1_ADR,COLLI_BDROM_L1_FLASH);
            send_msg5SValue(1,"COLIROML1",2,COLLI_BDROM_L1_FLASH);
        }

        if((svoPcalVar.CalDoneBits & bd_rre_colli_l1_end) != bd_rre_colli_l1_end)
        { // Update BD R/RE L1 Coli value to EEPROM Map
            SetPersistentTable2B(COLLI_BDRRE_L1_ADR,COLLI_BDRRE_L1_FLASH);
            send_msg5SValue(1,"COLIRREL1",2,COLLI_BDRRE_L1_FLASH);
        }

        // Update DoneBit to EEPROM Map
        svoPcalVar.CalDoneBits |= UpdateColiDoneBits;
        SetPersistentTable1B(COLLI_CAL_STATUS_ADR,(svoPcalVar.CalDoneBits >> 24));
        send_msg5SValue(1,"CALSTATUS",4,svoPcalVar.CalDoneBits);

        CloseWritableSegmentTable(ePersistent_Talbe);         /* Write to EEPROM */
    }
}
#endif

#if ((EN_ADEQ == 1)||(EN_BD_ADEQ == 1))
/*********************************************************
* Module: InitAdaptEq()
*
* Description: Initial Adaptive equalizer
**********************************************************/
void InitAdaptEq(void)
{
//20101021_02QU ===start=== show message of ADEQ init values, frank modify frank confirm
    BYTE tap_temp[5];
//20101021_02QU ===end=== show message of ADEQ init values
    HAL_DFE_EnDisAdaptEq(halDFE_AdaptEqDisable);
    HAL_DFE_SetAdaptEqCoef();
    HAL_DFE_SetAdaptEqPartRespCoef();
    HAL_DFE_EnDisAdaptEqOnDefect(halDFE_AdaptEqOnDefectEnable);

//20101021_02QU ===start=== show message of ADEQ init values, frank modify frank confirm
    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 0);
    tap_temp[0] = READ_FIELD(HAL_DFE_ADQCFBPRE);
    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 1);
    tap_temp[1] = READ_FIELD(HAL_DFE_ADQCFBPRE);
    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 2);
    tap_temp[2] = READ_FIELD(HAL_DFE_ADQCFBPRE);
    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 3);
    tap_temp[3] = READ_FIELD(HAL_DFE_ADQCFBPRE);
    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 4);
    tap_temp[4] = READ_FIELD(HAL_DFE_ADQCFBPRE);
    SendMsgCnB(SHOW_DEBUG_MSG,11,0x320A25,B1B(tap_temp[0]),B1B(tap_temp[1]),B1B(tap_temp[2]),B1B(tap_temp[3]),B1B(tap_temp[4]),B4B(READ_FIELD(HAL_DFE_INTEGRAT)),B2B(svo_bReadZoneSpeedX10));
//20101021_02QU ===end=== show message of ADEQ init values
}
#endif


#if  SERVOKEYPARAMETER == 1
/*********************************************************
* Module: InitServoKeyParameter()
*
* Description: set servo key parameter to flashROM
* Input:
* Returns: void
**********************************************************/
extern BYTE CurTblOnDram;
extern SENSE_T KeyPara_Sense;            /* Request Sense return data */
void InitServoKeyParameter(BYTE state)
{
    //OpenWritableSegmentTable(eKey_parameter_Table);
    CurTblOnDram = eKey_parameter_Table;
    LinearPatternFill((USHORT)((ULONG)DRVPARA_DATA/0x800),0,2,0x800/2,0); //clear
    SetPersistentTable1B(KEY_header, 'K');
    SetPersistentTable1B(KEY_header+1, 'E');
    SetPersistentTable1B(KEY_header+2, 'Y');
    SetPersistentTable1B(KEY_header+3, 'P');
    SetPersistentTable1B(KEY_header+4, 'A');
    SetPersistentTable1B(KEY_header+5, 'R');
    SetPersistentTable1B(KEY_header+6, 'A');
    SetPersistentTable1B(KEY_header_byte7, 0xDA);

    SetPersistentTable1B(CDROM_header, 'C');
    SetPersistentTable1B(CDROM_header+1, 'D');
    SetPersistentTable1B(CDROM_header+2, 'R');
    SetPersistentTable1B(CDROM_header+3, 'O');
    SetPersistentTable1B(CDROM_header+4, 'M');
    SetPersistentTable1B(CDROM_header+5, ' ');
    SetPersistentTable1B(CDROM_header+6, ' ');
    SetPersistentTable1B(CDROM_data_col, 0x09);
    SetPersistentTable1B(CDROM_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(CDR_header, 'C');
    SetPersistentTable1B(CDR_header+1, 'D');
    SetPersistentTable1B(CDR_header+2, 'R');
    SetPersistentTable1B(CDR_header+3, ' ');
    SetPersistentTable1B(CDR_header+4, ' ');
    SetPersistentTable1B(CDR_header+5, ' ');
    SetPersistentTable1B(CDR_header+6, ' ');
    SetPersistentTable1B(CDR_data_col, 0x09);
    SetPersistentTable1B(CDR_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(CDRW_header, 'C');
    SetPersistentTable1B(CDRW_header+1, 'D');
    SetPersistentTable1B(CDRW_header+2, 'R');
    SetPersistentTable1B(CDRW_header+3, 'W');
    SetPersistentTable1B(CDRW_header+4, ' ');
    SetPersistentTable1B(CDRW_header+5, ' ');
    SetPersistentTable1B(CDRW_header+6, ' ');
    SetPersistentTable1B(CDRW_data_col, 0x09);
    SetPersistentTable1B(CDRW_lastcol_bytenum, 0x0B);

    SetPersistentTable1B(DVD5_header, 'D');
    SetPersistentTable1B(DVD5_header+1, 'V');
    SetPersistentTable1B(DVD5_header+2, 'D');
    SetPersistentTable1B(DVD5_header+3, '5');
    SetPersistentTable1B(DVD5_header+4, ' ');
    SetPersistentTable1B(DVD5_header+5, ' ');
    SetPersistentTable1B(DVD5_header+6, ' ');
    SetPersistentTable1B(DVD5_data_col, 0x09);
    SetPersistentTable1B(DVD5_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(DVD9_header, 'D');
    SetPersistentTable1B(DVD9_header+1, 'V');
    SetPersistentTable1B(DVD9_header+2, 'D');
    SetPersistentTable1B(DVD9_header+3, '9');
    SetPersistentTable1B(DVD9_header+4, ' ');
    SetPersistentTable1B(DVD9_header+5, ' ');
    SetPersistentTable1B(DVD9_header+6, ' ');
    SetPersistentTable1B(DVD9_data_col, 0x0C);
    SetPersistentTable1B(DVD9_lastcol_bytenum, 0x01);
    SetPersistentTable1B(DVDPR_header, 'D');
    SetPersistentTable1B(DVDPR_header+1, 'V');
    SetPersistentTable1B(DVDPR_header+2, 'D');
    SetPersistentTable1B(DVDPR_header+3, '+');
    SetPersistentTable1B(DVDPR_header+4, 'R');
    SetPersistentTable1B(DVDPR_header+5, ' ');
    SetPersistentTable1B(DVDPR_header+6, ' ');
    SetPersistentTable1B(DVDPR_data_col, 0x09);
    SetPersistentTable1B(DVDPR_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(DVDMR_header, 'D');
    SetPersistentTable1B(DVDMR_header+1, 'V');
    SetPersistentTable1B(DVDMR_header+2, 'D');
    SetPersistentTable1B(DVDMR_header+3, '-');
    SetPersistentTable1B(DVDMR_header+4, 'R');
    SetPersistentTable1B(DVDMR_header+5, ' ');
    SetPersistentTable1B(DVDMR_header+6, ' ');
    SetPersistentTable1B(DVDMR_data_col, 0x09);
    SetPersistentTable1B(DVDMR_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(DVDPRW_header, 'D');
    SetPersistentTable1B(DVDPRW_header+1, 'V');
    SetPersistentTable1B(DVDPRW_header+2, 'D');
    SetPersistentTable1B(DVDPRW_header+3, '+');
    SetPersistentTable1B(DVDPRW_header+4, 'R');
    SetPersistentTable1B(DVDPRW_header+5, 'W');
    SetPersistentTable1B(DVDPRW_header+6, ' ');
    SetPersistentTable1B(DVDPRW_data_col, 0x09);
    SetPersistentTable1B(DVDPRW_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(DVDMRW_header, 'D');
    SetPersistentTable1B(DVDMRW_header+1, 'V');
    SetPersistentTable1B(DVDMRW_header+2, 'D');
    SetPersistentTable1B(DVDMRW_header+3, '-');
    SetPersistentTable1B(DVDMRW_header+4, 'R');
    SetPersistentTable1B(DVDMRW_header+5, 'W');
    SetPersistentTable1B(DVDMRW_header+6, ' ');
    SetPersistentTable1B(DVDMRW_data_col, 0x09);
    SetPersistentTable1B(DVDMRW_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(DVDPR9_header, 'D');
    SetPersistentTable1B(DVDPR9_header+1, 'V');
    SetPersistentTable1B(DVDPR9_header+2, 'D');
    SetPersistentTable1B(DVDPR9_header+3, '+');
    SetPersistentTable1B(DVDPR9_header+4, 'R');
    SetPersistentTable1B(DVDPR9_header+5, '9');
    SetPersistentTable1B(DVDPR9_header+6, ' ');
    SetPersistentTable1B(DVDPR9_data_col, 0x0C);
    SetPersistentTable1B(DVDPR9_lastcol_bytenum, 0x01);
    SetPersistentTable1B(DVDMR9_header, 'D');
    SetPersistentTable1B(DVDMR9_header+1, 'V');
    SetPersistentTable1B(DVDMR9_header+2, 'D');
    SetPersistentTable1B(DVDMR9_header+3, '-');
    SetPersistentTable1B(DVDMR9_header+4, 'R');
    SetPersistentTable1B(DVDMR9_header+5, '9');
    SetPersistentTable1B(DVDMR9_header+6, ' ');
    SetPersistentTable1B(DVDMR9_data_col, 0x0C);
    SetPersistentTable1B(DVDMR9_lastcol_bytenum, 0x01);

    SetPersistentTable1B(BDROM_header, 'B');
    SetPersistentTable1B(BDROM_header+1, 'D');
    SetPersistentTable1B(BDROM_header+2, 'R');
    SetPersistentTable1B(BDROM_header+3, 'O');
    SetPersistentTable1B(BDROM_header+4, 'M');
    SetPersistentTable1B(BDROM_header+5, 'S');
    SetPersistentTable1B(BDROM_header+6, 'L');
    SetPersistentTable1B(BDROM_data_col, 0x09);
    SetPersistentTable1B(BDROM_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(BDROMDL_header, 'B');
    SetPersistentTable1B(BDROMDL_header+1, 'D');
    SetPersistentTable1B(BDROMDL_header+2, 'R');
    SetPersistentTable1B(BDROMDL_header+3, 'O');
    SetPersistentTable1B(BDROMDL_header+4, 'M');
    SetPersistentTable1B(BDROMDL_header+5, 'D');
    SetPersistentTable1B(BDROMDL_header+6, 'L');
    SetPersistentTable1B(BDROMDL_data_col, 0x0C);
    SetPersistentTable1B(BDROMDL_lastcol_bytenum, 0x01);
    SetPersistentTable1B(BDR_header, 'B');
    SetPersistentTable1B(BDR_header+1, 'D');
    SetPersistentTable1B(BDR_header+2, 'R');
    SetPersistentTable1B(BDR_header+3, 'S');
    SetPersistentTable1B(BDR_header+4, 'L');
    SetPersistentTable1B(BDR_header+5, ' ');
    SetPersistentTable1B(BDR_header+6, ' ');
    SetPersistentTable1B(BDR_data_col, 0x09);
    SetPersistentTable1B(BDR_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(BDRDL_header, 'B');
    SetPersistentTable1B(BDRDL_header+1, 'D');
    SetPersistentTable1B(BDRDL_header+2, 'R');
    SetPersistentTable1B(BDRDL_header+3, 'D');
    SetPersistentTable1B(BDRDL_header+4, 'L');
    SetPersistentTable1B(BDRDL_header+5, ' ');
    SetPersistentTable1B(BDRDL_header+6, ' ');
    SetPersistentTable1B(BDRDL_data_col, 0x0C);
    SetPersistentTable1B(BDRDL_lastcol_bytenum, 0x01);
    SetPersistentTable1B(BDRE_header, 'B');
    SetPersistentTable1B(BDRE_header+1, 'D');
    SetPersistentTable1B(BDRE_header+2, 'R');
    SetPersistentTable1B(BDRE_header+3, 'E');
    SetPersistentTable1B(BDRE_header+4, 'S');
    SetPersistentTable1B(BDRE_header+5, 'L');
    SetPersistentTable1B(BDRE_header+6, ' ');
    SetPersistentTable1B(BDRE_data_col, 0x09);
    SetPersistentTable1B(BDRE_lastcol_bytenum, 0x0B);
    SetPersistentTable1B(BDREDL_header, 'B');
    SetPersistentTable1B(BDREDL_header+1, 'D');
    SetPersistentTable1B(BDREDL_header+2, 'R');
    SetPersistentTable1B(BDREDL_header+3, 'E');
    SetPersistentTable1B(BDREDL_header+4, 'D');
    SetPersistentTable1B(BDREDL_header+5, 'L');
    SetPersistentTable1B(BDREDL_header+6, ' ');
    SetPersistentTable1B(BDREDL_data_col, 0x0C);
    SetPersistentTable1B(BDREDL_lastcol_bytenum, 0x01);
    if(state == TRUE)
        CloseWritableSegmentTable(eKey_parameter_Table);
}
/*********************************************************
* Module: SetServoKeyParameter()
*
* Description: set servo key parameter to flashROM
* Input:
* Returns: void
**********************************************************/
void SetServoKeyParameter(void)
{
    OpenWritableSegmentTable(eKey_parameter_Table);
    switch(pdbGetDiscKind())
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
        case eDVDROMDisc:
        case eDVDPlusRDisc:
        case eDVDMinusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRWDisc:
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
                SetServoKeyParameterSL();
                break;
        case eDVDROMDLDisc:
        case eDVDPlusRDLDisc:
        case eDVDMinusRDLDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
                SetServoKeyParameterDL();
                break;
        default :
                break;

    }

    CloseWritableSegmentTable(eKey_parameter_Table);
}
void SetServoKeyParameterSL(void)
{
    BYTE  cur_zone;
    USHORT KeyIndex;

    cur_zone = calRadius2Zone(Iram_stp_CurrentPosition);

    switch(pdbGetDiscKind())
{
        case eCDROMDisc:
                KeyIndex = CDROM_header;
                break;
        case eCDRDisc:
                KeyIndex = CDR_header;
                break;
        case eCDRWDisc:
                KeyIndex = CDRW_header;
                break;
        case eDVDROMDisc:
                KeyIndex = DVD5_header;
                break;
        case eDVDPlusRDisc:
                KeyIndex = DVDPR_header;
                break;
        case eDVDMinusRDisc:
                KeyIndex = DVDMR_header;
                break;
        case eDVDPlusRWDisc:
                KeyIndex = DVDPRW_header;
                break;
        case eDVDMinusRWDisc:
                KeyIndex = DVDMRW_header;
                break;
        case eBDROMDisc:
                KeyIndex = BDROM_header;
                break;
        case eBDRDisc:
                KeyIndex = BDR_header;
                break;
        case eBDREDisc:
                KeyIndex = BDRE_header;
                break;
        case eDVDROMDLDisc:
        case eDVDPlusRDLDisc:
        case eDVDMinusRDLDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
                break;
        default :
                break;

}


    SetPersistentTable1B(KeyIndex+13  , KeyPara_Sense.key);
    SetPersistentTable1B(KeyIndex+14  , KeyPara_Sense.cod);
    SetPersistentTable1B(KeyIndex+15  , KeyPara_Sense.qul);

    SetPersistentTable1B(KeyIndex+32  , HBYTE(vref_fm_mux3_mV));
    SetPersistentTable1B(KeyIndex+33  , LBYTE(vref_fm_mux3_mV));
    SetPersistentTable1B(KeyIndex+34  , HBYTE(v21_fm_mux1_mV));
    SetPersistentTable1B(KeyIndex+35  , LBYTE(v21_fm_mux1_mV));
    SetPersistentTable1B(KeyIndex+36  , READ_FIELD(HAL_AFE_RFINGN));
    SetPersistentTable1B(KeyIndex+37  , READ_FIELD(HAL_AFE_RFCOSP));
    SetPersistentTable1B(KeyIndex+38  , READ_FIELD(HAL_AFE_RFCOSN));
    SetPersistentTable1B(KeyIndex+39  , READ_FIELD(HAL_AFE_RFGN));
    SetPersistentTable1B(KeyIndex+40  , READ_FIELD(HAL_AFE_RFAGCREG));
    SetPersistentTable1B(KeyIndex+41  , READ_FIELD(HAL_AFE_RFAOCREG));
    SetPersistentTable1B(KeyIndex+42  , HBYTE(svoPrvtVar_maxFEpp_0dB));//org:READ_FIELD(HAL_AFE_RFHSSEL)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+43  , LBYTE(svoPrvtVar_maxFEpp_0dB));//org:READ_FIELD(HAL_AFE_RFHSFC)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+44  , READ_FIELD(HAL_AFE_RFPOS));
    SetPersistentTable1B(KeyIndex+45  , READ_FIELD(HAL_AFE_MBVREFOSA));
    SetPersistentTable1B(KeyIndex+46  , READ_FIELD(HAL_AFE_SBVREFOSA));
    SetPersistentTable1B(KeyIndex+47  , READ_FIELD(HAL_AFE_MBGUPSEL));

    SetPersistentTable1B(KeyIndex+48  , READ_FIELD(HAL_AFE_SBGUPSEL));
    SetPersistentTable1B(KeyIndex+49  , READ_FIELD(HAL_AFE_FEOOSA));
    SetPersistentTable1B(KeyIndex+50  , READ_FIELD(HAL_AFE_FEOBG));
    SetPersistentTable1B(KeyIndex+51  , READ_FIELD(HAL_AFE_FEOPGN));
    SetPersistentTable1B(KeyIndex+52  , READ_FIELD(HAL_AFE_SPP1OSA));
    SetPersistentTable1B(KeyIndex+53  , READ_FIELD(HAL_AFE_SPP1PGNA));
    SetPersistentTable1B(KeyIndex+54  , READ_FIELD(HAL_AFE_SPP1BG));
    SetPersistentTable1B(KeyIndex+55  , READ_FIELD(HAL_AFE_SPP2OSA));
    SetPersistentTable1B(KeyIndex+56  , READ_FIELD(HAL_AFE_SPP2PGNA));
    SetPersistentTable1B(KeyIndex+57  , READ_FIELD(HAL_AFE_SPP2BG));
    SetPersistentTable1B(KeyIndex+58  , READ_FIELD(HAL_AFE_SBADOSA));
    SetPersistentTable1B(KeyIndex+59  , READ_FIELD(HAL_AFE_SBADGNA));
    SetPersistentTable1B(KeyIndex+60  , READ_FIELD(HAL_AFE_FCSO));
    SetPersistentTable1B(KeyIndex+61  , READ_FIELD(HAL_AFE_FCSG));
    SetPersistentTable1B(KeyIndex+62  , READ_FIELD(HAL_AFE_FEPGN));
    SetPersistentTable1B(KeyIndex+63  , READ_FIELD(HAL_AFE_FEBG));
    SetPersistentTable1B(KeyIndex+64  , READ_FIELD(HAL_AFE_FE2OSA));
    SetPersistentTable1B(KeyIndex+65  , READ_FIELD(HAL_AFE_FE2GNREG));

    SetPersistentTable1B(KeyIndex+66  , READ_FIELD(HAL_AFE_SRFOSA));
    SetPersistentTable1B(KeyIndex+67  , READ_FIELD(HAL_AFE_SRFOBGA));
    SetPersistentTable1B(KeyIndex+68  , READ_FIELD(HAL_AFE_MPPOSA));
    SetPersistentTable1B(KeyIndex+69  , READ_FIELD(HAL_AFE_MPPOBG));
    SetPersistentTable1B(KeyIndex+70  , READ_FIELD(HAL_AFE_MPPOPGN));
    SetPersistentTable1B(KeyIndex+71  , READ_FIELD(HAL_AFE_CEOS));
    SetPersistentTable1B(KeyIndex+72  , READ_FIELD(HAL_AFE_CEPGN));
    SetPersistentTable1B(KeyIndex+73  , READ_FIELD(HAL_AFE_CEGN));
    SetPersistentTable1B(KeyIndex+74  , READ_FIELD(HAL_AFE_CEBG));
    SetPersistentTable1B(KeyIndex+75  , READ_FIELD(HAL_AFE_TRKO));
    SetPersistentTable1B(KeyIndex+76  , READ_FIELD(HAL_AFE_TRKG));
    SetPersistentTable1B(KeyIndex+77  , READ_FIELD(HAL_AFE_TEPGN));
    SetPersistentTable1B(KeyIndex+78  , READ_FIELD(HAL_AFE_TEBG));
    SetPersistentTable1B(KeyIndex+79  , READ_FIELD(HAL_AFE_TE2OSA));
    SetPersistentTable1B(KeyIndex+80  , READ_FIELD(HAL_AFE_TE2GNREG));

    SetPersistentTable1B(KeyIndex+81  , READ_FIELD(HAL_AFE_BSKO));
    SetPersistentTable1B(KeyIndex+82  , READ_FIELD(HAL_AFE_BSKG));
    SetPersistentTable1B(KeyIndex+83  , HBYTE(svoPrvtVar_MPPO_0dB));//org:READ_FIELD(HAL_AFE_BSPGN)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+84  , LBYTE(svoPrvtVar_MPPO_0dB));//org:READ_FIELD(HAL_AFE_BSBG)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+85  , READ_FIELD(HAL_AFE_BS2OSA));
    SetPersistentTable1B(KeyIndex+86  , svoPrvtVar_MPPOSRFORatioX100);//org:READ_FIELD(HAL_AFE_BS2GNREG)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+87  , READ_FIELD(HAL_AFE_PHSOS));
    SetPersistentTable1B(KeyIndex+88  , READ_FIELD(HAL_AFE_BHSOS));

    SetPersistentTable1B(KeyIndex+89  , HBYTE(FMD_PPLVL_CD));
    SetPersistentTable1B(KeyIndex+90  , LBYTE(FMD_PPLVL_CD));
    SetPersistentTable1B(KeyIndex+91  , HBYTE(FMD_PPLVL_DVD));
    SetPersistentTable1B(KeyIndex+92  , LBYTE(FMD_PPLVL_DVD));
    SetPersistentTable1B(KeyIndex+93  , HBYTE(FMD_PPLVL_BD));
    SetPersistentTable1B(KeyIndex+94  , LBYTE(FMD_PPLVL_BD));
    SetPersistentTable1B(KeyIndex+95  , HBYTE(FMD_DCLVL_CD));
    SetPersistentTable1B(KeyIndex+96  , LBYTE(FMD_DCLVL_CD));
    SetPersistentTable1B(KeyIndex+97  , HBYTE(FMD_DCLVL_DVD));
    SetPersistentTable1B(KeyIndex+98  , LBYTE(FMD_DCLVL_DVD));
    SetPersistentTable1B(KeyIndex+99  , HBYTE(FMD_DCLVL_BD));
    SetPersistentTable1B(KeyIndex+100 , LBYTE(FMD_DCLVL_BD));

    SetPersistentTable1B(KeyIndex+101 , HBYTE(svoCalMisc1Var[0].feGainDSP));
    SetPersistentTable1B(KeyIndex+102 , LBYTE(svoCalMisc1Var[0].feGainDSP));
    SetPersistentTable1B(KeyIndex+103 , HBYTE(svoCalMisc1Var[0].teGainDSP));
    SetPersistentTable1B(KeyIndex+104 , LBYTE(svoCalMisc1Var[0].teGainDSP));
    SetPersistentTable1B(KeyIndex+105 , HBYTE(svoCalFebcVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+106 , LBYTE(svoCalFebcVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+107 , HBYTE(svoCalTiltVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+108 , LBYTE(svoCalTiltVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+109 , HBYTE(svoCalVar.ceOfsData.all16));
    SetPersistentTable1B(KeyIndex+110 , LBYTE(svoCalVar.ceOfsData.all16));
    SetPersistentTable1B(KeyIndex+111 , HBYTE(svoCalVar.feOfsData.all16));
    SetPersistentTable1B(KeyIndex+112 , LBYTE(svoCalVar.feOfsData.all16));
    SetPersistentTable1B(KeyIndex+113 , HBYTE(svoCalVar.teOfsData.all16));
    SetPersistentTable1B(KeyIndex+114 , LBYTE(svoCalVar.teOfsData.all16));
    SetPersistentTable1B(KeyIndex+115 , HBYTE(svoCalVar.rpOfsData.all16));
    SetPersistentTable1B(KeyIndex+116 , LBYTE(svoCalVar.rpOfsData.all16));

    SetPersistentTable1B(KeyIndex+117 , svoPrvtVar_troos);
    SetPersistentTable1B(KeyIndex+118 , svoRRFVar.modulation[0]);

    SetPersistentTable1B(KeyIndex+119 , READ_FIELD(HAL_AFE_DPDCH1OS));
    SetPersistentTable1B(KeyIndex+120 , READ_FIELD(HAL_AFE_DPDCH2OS));
    SetPersistentTable1B(KeyIndex+121 , READ_FIELD(HAL_AFE_DPDCH3OS));
    SetPersistentTable1B(KeyIndex+122 , READ_FIELD(HAL_AFE_DPDCH4OS));
    SetPersistentTable1B(KeyIndex+123 , READ_FIELD(HAL_AFE_DPDGN));
    SetPersistentTable1B(KeyIndex+124 , READ_FIELD(HAL_AFE_DPDVCGM));
    #if (CHIP_REV <= 0xB3)
    SetPersistentTable1B(KeyIndex+125 , READ_FIELD(HAL_AFE_RFGMDAC));
    #endif
    SetPersistentTable1B(KeyIndex+126 , READ_FIELD(HAL_AFE_DPDHSSEL));
    SetPersistentTable1B(KeyIndex+127 , READ_FIELD(HAL_AFE_DPDHSFCH));
    SetPersistentTable1B(KeyIndex+128 , READ_FIELD(HAL_AFE_DPDHSFCL));
    SetPersistentTable1B(KeyIndex+129 , READ_FIELD(HAL_AFE_DPDHSISCL));
    SetPersistentTable1B(KeyIndex+130 , READ_FIELD(HAL_AFE_DPDINFC));
    SetPersistentTable1B(KeyIndex+131 , READ_FIELD(HAL_AFE_DPDBST));

    SetPersistentTable1B(KeyIndex+132 , HBYTE(svoRampMeasVar.STM));
    SetPersistentTable1B(KeyIndex+133 , LBYTE(svoRampMeasVar.STM));
    SetPersistentTable1B(KeyIndex+134 , HBYTE(svoRampMeasVar.LayerTime[0]));
    SetPersistentTable1B(KeyIndex+135 , LBYTE(svoRampMeasVar.LayerTime[0]));
    SetPersistentTable1B(KeyIndex+136 , HBYTE(svoRampMeasVar.LayerTime[1]));
    SetPersistentTable1B(KeyIndex+137 , LBYTE(svoRampMeasVar.LayerTime[1]));
    SetPersistentTable1B(KeyIndex+138 , HBYTE(svoPrvtVar_maxSRFOpp_0dB));
    SetPersistentTable1B(KeyIndex+139 , LBYTE(svoPrvtVar_maxSRFOpp_0dB));
    SetPersistentTable1B(KeyIndex+140 , svoPrvtVar_TEBSRatioX100);
    SetPersistentTable1B(KeyIndex+141 , HBYTE(svoRunOut));
    SetPersistentTable1B(KeyIndex+142 , LBYTE(svoRunOut));
    #if COLLI_SWITCH == 1
    SetPersistentTable1B(KeyIndex+143 , HBYTE(svoCalColliVar[0].opt_value));
    SetPersistentTable1B(KeyIndex+144 , LBYTE(svoCalColliVar[0].opt_value));
    #endif
    SetPersistentTable1B(KeyIndex+145 , HBYTE(svoCalFebcVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+146 , LBYTE(svoCalFebcVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+147 , HBYTE(svoCalTiltVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+148 , LBYTE(svoCalTiltVar[0][cur_zone].opt_value));

    SetPersistentTable1B(KeyIndex+149 , HBYTE(ADCVRPOW));
    SetPersistentTable1B(KeyIndex+150 , LBYTE(ADCVRPOW));
    SetPersistentTable1B(KeyIndex+151 , HBYTE(VDELTAX));
    SetPersistentTable1B(KeyIndex+152 , LBYTE(VDELTAX));
    SetPersistentTable1B(KeyIndex+153 , HBYTE(RSETPOINT));
    SetPersistentTable1B(KeyIndex+154 , LBYTE(RSETPOINT));
    SetPersistentTable1B(KeyIndex+155 , HBYTE(svoPrvtVar_avgFEpp[0]));
    SetPersistentTable1B(KeyIndex+156 , LBYTE(svoPrvtVar_avgFEpp[0]));
    SetPersistentTable1B(KeyIndex+157 , HBYTE(svoPrvtVar_avgFEpp[1]));
    SetPersistentTable1B(KeyIndex+158 , LBYTE(svoPrvtVar_avgFEpp[1]));
    SetPersistentTable1B(KeyIndex+159 , HBYTE(LTHRPJGLVL));
    SetPersistentTable1B(KeyIndex+160 , LBYTE(LTHRPJGLVL));
    SetPersistentTable1B(KeyIndex+161 , HBYTE(svoPrvtVar_RFpp));
    SetPersistentTable1B(KeyIndex+162 , LBYTE(svoPrvtVar_RFpp));
    SetPersistentTable1B(KeyIndex+163 , HBYTE(svoPrvtVar_SledHomeIndex));
    SetPersistentTable1B(KeyIndex+164 , LBYTE(svoPrvtVar_SledHomeIndex));
#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
    SetPersistentTable1B(KeyIndex+165 , HBYTE(Unbalance_midspd));
    SetPersistentTable1B(KeyIndex+166 , LBYTE(Unbalance_midspd));
    SetPersistentTable1B(KeyIndex+167 , HBYTE(Unbalance_highspd));
    SetPersistentTable1B(KeyIndex+168 , LBYTE(Unbalance_highspd));
#endif
    SetPersistentTable1B(KeyIndex+169 , HBYTE(FGPULSECOUNT));
    SetPersistentTable1B(KeyIndex+170 , LBYTE(FGPULSECOUNT));

    SetPersistentTable1B(KeyIndex+171 , svoPrvtVar_wbllockcnt);
    SetPersistentTable1B(KeyIndex+172 , svoPrvtVar_jitter);
    SetPersistentTable1B(KeyIndex+173 , HBYTE(svoPrvtVar_pp_dpd_diff_mv));
    SetPersistentTable1B(KeyIndex+174 , LBYTE(svoPrvtVar_pp_dpd_diff_mv));
}
void SetServoKeyParameterDL(void)
{
    BYTE  cur_zone;
    USHORT KeyIndex;

    cur_zone = calRadius2Zone(Iram_stp_CurrentPosition);

    switch(pdbGetDiscKind())
{
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
        case eDVDROMDisc:
        case eDVDPlusRDisc:
        case eDVDMinusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRWDisc:
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
                break;
        case eDVDROMDLDisc:
                KeyIndex = DVD9_header;
                break;
        case eDVDPlusRDLDisc:
                KeyIndex = DVDPR9_header;
                break;
        case eDVDMinusRDLDisc:
                KeyIndex = DVDMR9_header;
                break;
        case eBDROMDLDisc:
                KeyIndex = BDROMDL_header;
                break;
        case eBDRDLDisc:
                KeyIndex = BDRDL_header;
                break;
        case eBDREDLDisc:
                KeyIndex = BDREDL_header;
                break;
        default :
                break;

}

    SetPersistentTable1B(KeyIndex+13  , KeyPara_Sense.key);
    SetPersistentTable1B(KeyIndex+14  , KeyPara_Sense.cod);
    SetPersistentTable1B(KeyIndex+15  , KeyPara_Sense.qul);

    SetPersistentTable1B(KeyIndex+32  , HBYTE(vref_fm_mux3_mV));
    SetPersistentTable1B(KeyIndex+33  , LBYTE(vref_fm_mux3_mV));
    SetPersistentTable1B(KeyIndex+34  , HBYTE(v21_fm_mux1_mV));
    SetPersistentTable1B(KeyIndex+35  , LBYTE(v21_fm_mux1_mV));
    SetPersistentTable1B(KeyIndex+36  , READ_FIELD(HAL_AFE_RFINGN));
    SetPersistentTable1B(KeyIndex+37  , READ_FIELD(HAL_AFE_RFCOSP));
    SetPersistentTable1B(KeyIndex+38  , READ_FIELD(HAL_AFE_RFCOSN));
    SetPersistentTable1B(KeyIndex+39  , READ_FIELD(HAL_AFE_RFGN));
    SetPersistentTable1B(KeyIndex+40  , READ_FIELD(HAL_AFE_RFAGCREG));
    SetPersistentTable1B(KeyIndex+41  , READ_FIELD(HAL_AFE_RFAOCREG));
    SetPersistentTable1B(KeyIndex+42  , HBYTE(svoPrvtVar_maxFEpp_0dB));//org:READ_FIELD(HAL_AFE_RFHSSEL)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+43  , LBYTE(svoPrvtVar_maxFEpp_0dB));//org:READ_FIELD(HAL_AFE_RFHSFC)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+44  , READ_FIELD(HAL_AFE_RFPOS));
    SetPersistentTable1B(KeyIndex+45  , READ_FIELD(HAL_AFE_MBVREFOSA));
    SetPersistentTable1B(KeyIndex+46  , READ_FIELD(HAL_AFE_SBVREFOSA));
    SetPersistentTable1B(KeyIndex+47  , READ_FIELD(HAL_AFE_MBGUPSEL));

    SetPersistentTable1B(KeyIndex+48  , READ_FIELD(HAL_AFE_SBGUPSEL));
    SetPersistentTable1B(KeyIndex+49  , READ_FIELD(HAL_AFE_FEOOSA));
    SetPersistentTable1B(KeyIndex+50  , READ_FIELD(HAL_AFE_FEOBG));
    SetPersistentTable1B(KeyIndex+51  , READ_FIELD(HAL_AFE_FEOPGN));
    SetPersistentTable1B(KeyIndex+52  , READ_FIELD(HAL_AFE_SPP1OSA));
    SetPersistentTable1B(KeyIndex+53  , READ_FIELD(HAL_AFE_SPP1PGNA));
    SetPersistentTable1B(KeyIndex+54  , READ_FIELD(HAL_AFE_SPP1BG));
    SetPersistentTable1B(KeyIndex+55  , READ_FIELD(HAL_AFE_SPP2OSA));
    SetPersistentTable1B(KeyIndex+56  , READ_FIELD(HAL_AFE_SPP2PGNA));
    SetPersistentTable1B(KeyIndex+57  , READ_FIELD(HAL_AFE_SPP2BG));
    SetPersistentTable1B(KeyIndex+58  , READ_FIELD(HAL_AFE_SBADOSA));
    SetPersistentTable1B(KeyIndex+59  , READ_FIELD(HAL_AFE_SBADGNA));
    SetPersistentTable1B(KeyIndex+60  , READ_FIELD(HAL_AFE_FCSO));
    SetPersistentTable1B(KeyIndex+61  , READ_FIELD(HAL_AFE_FCSG));
    SetPersistentTable1B(KeyIndex+62  , READ_FIELD(HAL_AFE_FEPGN));
    SetPersistentTable1B(KeyIndex+63  , READ_FIELD(HAL_AFE_FEBG));
    SetPersistentTable1B(KeyIndex+64  , READ_FIELD(HAL_AFE_FE2OSA));
    SetPersistentTable1B(KeyIndex+65  , READ_FIELD(HAL_AFE_FE2GNREG));

    SetPersistentTable1B(KeyIndex+66  , READ_FIELD(HAL_AFE_SRFOSA));
    SetPersistentTable1B(KeyIndex+67  , READ_FIELD(HAL_AFE_SRFOBGA));
    SetPersistentTable1B(KeyIndex+68  , READ_FIELD(HAL_AFE_MPPOSA));
    SetPersistentTable1B(KeyIndex+69  , READ_FIELD(HAL_AFE_MPPOBG));
    SetPersistentTable1B(KeyIndex+70  , READ_FIELD(HAL_AFE_MPPOPGN));
    SetPersistentTable1B(KeyIndex+71  , READ_FIELD(HAL_AFE_CEOS));
    SetPersistentTable1B(KeyIndex+72  , READ_FIELD(HAL_AFE_CEPGN));
    SetPersistentTable1B(KeyIndex+73  , READ_FIELD(HAL_AFE_CEGN));
    SetPersistentTable1B(KeyIndex+74  , READ_FIELD(HAL_AFE_CEBG));
    SetPersistentTable1B(KeyIndex+75  , READ_FIELD(HAL_AFE_TRKO));
    SetPersistentTable1B(KeyIndex+76  , READ_FIELD(HAL_AFE_TRKG));
    SetPersistentTable1B(KeyIndex+77  , READ_FIELD(HAL_AFE_TEPGN));
    SetPersistentTable1B(KeyIndex+78  , READ_FIELD(HAL_AFE_TEBG));
    SetPersistentTable1B(KeyIndex+79  , READ_FIELD(HAL_AFE_TE2OSA));
    SetPersistentTable1B(KeyIndex+80  , READ_FIELD(HAL_AFE_TE2GNREG));

    SetPersistentTable1B(KeyIndex+81  , READ_FIELD(HAL_AFE_BSKO));
    SetPersistentTable1B(KeyIndex+82  , READ_FIELD(HAL_AFE_BSKG));
    SetPersistentTable1B(KeyIndex+83  , HBYTE(svoPrvtVar_MPPO_0dB));//org:READ_FIELD(HAL_AFE_BSPGN)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+84  , LBYTE(svoPrvtVar_MPPO_0dB));//org:READ_FIELD(HAL_AFE_BSBG)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+85  , READ_FIELD(HAL_AFE_BS2OSA));
    SetPersistentTable1B(KeyIndex+86  , svoPrvtVar_MPPOSRFORatioX100);//org:READ_FIELD(HAL_AFE_BS2GNREG)//20090507_h.30 == start == Collect Key Parametersdeciding Disc mis Judge Level in the future
    SetPersistentTable1B(KeyIndex+87  , READ_FIELD(HAL_AFE_PHSOS));
    SetPersistentTable1B(KeyIndex+88  , READ_FIELD(HAL_AFE_BHSOS));

    SetPersistentTable1B(KeyIndex+89  , HBYTE(FMD_PPLVL_CD));
    SetPersistentTable1B(KeyIndex+90  , LBYTE(FMD_PPLVL_CD));
    SetPersistentTable1B(KeyIndex+91  , HBYTE(FMD_PPLVL_DVD));
    SetPersistentTable1B(KeyIndex+92  , LBYTE(FMD_PPLVL_DVD));
    SetPersistentTable1B(KeyIndex+93  , HBYTE(FMD_PPLVL_BD));
    SetPersistentTable1B(KeyIndex+94  , LBYTE(FMD_PPLVL_BD));
    SetPersistentTable1B(KeyIndex+95  , HBYTE(FMD_DCLVL_CD));
    SetPersistentTable1B(KeyIndex+96  , LBYTE(FMD_DCLVL_CD));
    SetPersistentTable1B(KeyIndex+97  , HBYTE(FMD_DCLVL_DVD));
    SetPersistentTable1B(KeyIndex+98  , LBYTE(FMD_DCLVL_DVD));
    SetPersistentTable1B(KeyIndex+99  , HBYTE(FMD_DCLVL_BD));
    SetPersistentTable1B(KeyIndex+100 , LBYTE(FMD_DCLVL_BD));

    SetPersistentTable1B(KeyIndex+101 , HBYTE(svoCalMisc1Var[0].feGainDSP));
    SetPersistentTable1B(KeyIndex+102 , LBYTE(svoCalMisc1Var[0].feGainDSP));
    SetPersistentTable1B(KeyIndex+103 , HBYTE(svoCalMisc1Var[0].teGainDSP));
    SetPersistentTable1B(KeyIndex+104 , LBYTE(svoCalMisc1Var[0].teGainDSP));
    SetPersistentTable1B(KeyIndex+105 , HBYTE(svoCalFebcVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+106 , LBYTE(svoCalFebcVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+107 , HBYTE(svoCalTiltVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+108 , LBYTE(svoCalTiltVar[0][0].opt_value));
    SetPersistentTable1B(KeyIndex+109 , HBYTE(svoCalVar.ceOfsData.all16));
    SetPersistentTable1B(KeyIndex+110 , LBYTE(svoCalVar.ceOfsData.all16));
    SetPersistentTable1B(KeyIndex+111 , HBYTE(svoCalVar.feOfsData.all16));
    SetPersistentTable1B(KeyIndex+112 , LBYTE(svoCalVar.feOfsData.all16));
    SetPersistentTable1B(KeyIndex+113 , HBYTE(svoCalVar.teOfsData.all16));
    SetPersistentTable1B(KeyIndex+114 , LBYTE(svoCalVar.teOfsData.all16));
    SetPersistentTable1B(KeyIndex+115 , HBYTE(svoCalVar.rpOfsData.all16));
    SetPersistentTable1B(KeyIndex+116 , LBYTE(svoCalVar.rpOfsData.all16));

    SetPersistentTable1B(KeyIndex+117 , svoPrvtVar_troos);
    SetPersistentTable1B(KeyIndex+118 , svoRRFVar.modulation[0]);

    SetPersistentTable1B(KeyIndex+119 , READ_FIELD(HAL_AFE_DPDCH1OS));
    SetPersistentTable1B(KeyIndex+120 , READ_FIELD(HAL_AFE_DPDCH2OS));
    SetPersistentTable1B(KeyIndex+121 , READ_FIELD(HAL_AFE_DPDCH3OS));
    SetPersistentTable1B(KeyIndex+122 , READ_FIELD(HAL_AFE_DPDCH4OS));
    SetPersistentTable1B(KeyIndex+123 , READ_FIELD(HAL_AFE_DPDGN));
    SetPersistentTable1B(KeyIndex+124 , READ_FIELD(HAL_AFE_DPDVCGM));
    #if (CHIP_REV <= 0xB3)
    SetPersistentTable1B(KeyIndex+125 , READ_FIELD(HAL_AFE_RFGMDAC));
    #endif
    SetPersistentTable1B(KeyIndex+126 , READ_FIELD(HAL_AFE_DPDHSSEL));
    SetPersistentTable1B(KeyIndex+127 , READ_FIELD(HAL_AFE_DPDHSFCH));
    SetPersistentTable1B(KeyIndex+128 , READ_FIELD(HAL_AFE_DPDHSFCL));
    SetPersistentTable1B(KeyIndex+129 , READ_FIELD(HAL_AFE_DPDHSISCL));
    SetPersistentTable1B(KeyIndex+130 , READ_FIELD(HAL_AFE_DPDINFC));
    SetPersistentTable1B(KeyIndex+131 , READ_FIELD(HAL_AFE_DPDBST));
    SetPersistentTable1B(KeyIndex+132 , HBYTE(svoRampMeasVar.STM));
    SetPersistentTable1B(KeyIndex+133 , LBYTE(svoRampMeasVar.STM));
    SetPersistentTable1B(KeyIndex+134 , HBYTE(svoRampMeasVar.LayerTime[0]));
    SetPersistentTable1B(KeyIndex+135 , LBYTE(svoRampMeasVar.LayerTime[0]));
    SetPersistentTable1B(KeyIndex+136 , HBYTE(svoRampMeasVar.LayerTime[1]));
    SetPersistentTable1B(KeyIndex+137 , LBYTE(svoRampMeasVar.LayerTime[1]));
    SetPersistentTable1B(KeyIndex+138 , HBYTE(svoPrvtVar_maxSRFOpp_0dB));
    SetPersistentTable1B(KeyIndex+139 , LBYTE(svoPrvtVar_maxSRFOpp_0dB));
    SetPersistentTable1B(KeyIndex+140 , svoPrvtVar_TEBSRatioX100);
    SetPersistentTable1B(KeyIndex+141 , HBYTE(svoRunOut));
    SetPersistentTable1B(KeyIndex+142 , LBYTE(svoRunOut));
    #if COLLI_SWITCH == 1
    SetPersistentTable1B(KeyIndex+143 , HBYTE(svoCalColliVar[0].opt_value));
    SetPersistentTable1B(KeyIndex+144 , LBYTE(svoCalColliVar[0].opt_value));
    #endif
    SetPersistentTable1B(KeyIndex+145 , HBYTE(svoCalFebcVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+146 , LBYTE(svoCalFebcVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+147 , HBYTE(svoCalTiltVar[0][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+148 , LBYTE(svoCalTiltVar[0][cur_zone].opt_value));

    SetPersistentTable1B(KeyIndex+149 , READ_FIELD(HAL_AFE_RFAGCREG));
    SetPersistentTable1B(KeyIndex+150 , READ_FIELD(HAL_AFE_RFAOCREG));
    SetPersistentTable1B(KeyIndex+151 , READ_FIELD(HAL_AFE_SPP1OSA));
    SetPersistentTable1B(KeyIndex+152 , READ_FIELD(HAL_AFE_SPP1PGNA));
    SetPersistentTable1B(KeyIndex+153 , READ_FIELD(HAL_AFE_SPP1BG));
    SetPersistentTable1B(KeyIndex+154 , READ_FIELD(HAL_AFE_MPPOSA));
    SetPersistentTable1B(KeyIndex+155 , READ_FIELD(HAL_AFE_MPPOBG));
    SetPersistentTable1B(KeyIndex+156 , READ_FIELD(HAL_AFE_MPPOPGN));
    SetPersistentTable1B(KeyIndex+157 , READ_FIELD(HAL_AFE_CEOS));
    SetPersistentTable1B(KeyIndex+158 , READ_FIELD(HAL_AFE_CEPGN));
    SetPersistentTable1B(KeyIndex+159 , READ_FIELD(HAL_AFE_CEGN));
    SetPersistentTable1B(KeyIndex+160 , READ_FIELD(HAL_AFE_CEBG));
    SetPersistentTable1B(KeyIndex+161 , READ_FIELD(HAL_AFE_TRKO));
    SetPersistentTable1B(KeyIndex+162 , READ_FIELD(HAL_AFE_TRKG));
    SetPersistentTable1B(KeyIndex+163 , READ_FIELD(HAL_AFE_TEPGN));
    SetPersistentTable1B(KeyIndex+164 , READ_FIELD(HAL_AFE_TEBG));
    SetPersistentTable1B(KeyIndex+165 , READ_FIELD(HAL_AFE_TE2OSA));
    SetPersistentTable1B(KeyIndex+166 , READ_FIELD(HAL_AFE_TE2GNREG));
    SetPersistentTable1B(KeyIndex+167 , READ_FIELD(HAL_AFE_RPOS));
    SetPersistentTable1B(KeyIndex+168 , READ_FIELD(HAL_AFE_RPPGN));
    SetPersistentTable1B(KeyIndex+169 , READ_FIELD(HAL_AFE_RPBG));
    SetPersistentTable1B(KeyIndex+170 , READ_FIELD(HAL_AFE_RPGN));
    SetPersistentTable1B(KeyIndex+171 , READ_FIELD(HAL_AFE_RP2OS));
    SetPersistentTable1B(KeyIndex+172 , READ_FIELD(HAL_AFE_RP2GN));
    SetPersistentTable1B(KeyIndex+173 , HBYTE(svoCalMisc1Var[1].feGainDSP));
    SetPersistentTable1B(KeyIndex+174 , LBYTE(svoCalMisc1Var[1].feGainDSP));
    SetPersistentTable1B(KeyIndex+175 , HBYTE(svoCalMisc1Var[1].teGainDSP));
    SetPersistentTable1B(KeyIndex+176 , LBYTE(svoCalMisc1Var[1].teGainDSP));
    SetPersistentTable1B(KeyIndex+177 , HBYTE(svoCalFebcVar[1][0].opt_value));
    SetPersistentTable1B(KeyIndex+178 , LBYTE(svoCalFebcVar[1][0].opt_value));
    SetPersistentTable1B(KeyIndex+179 , HBYTE(svoCalTiltVar[1][0].opt_value));
    SetPersistentTable1B(KeyIndex+180 , LBYTE(svoCalTiltVar[1][0].opt_value));
    #if COLLI_SWITCH == 1
    SetPersistentTable1B(KeyIndex+181 , HBYTE(svoCalColliVar[1].opt_value));
    SetPersistentTable1B(KeyIndex+182 , LBYTE(svoCalColliVar[1].opt_value));
    #endif
    SetPersistentTable1B(KeyIndex+183 , HBYTE(svoCalFebcVar[1][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+184 , LBYTE(svoCalFebcVar[1][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+185 , HBYTE(svoCalTiltVar[1][cur_zone].opt_value));
    SetPersistentTable1B(KeyIndex+186 , LBYTE(svoCalTiltVar[1][cur_zone].opt_value));

    SetPersistentTable1B(KeyIndex+187 , HBYTE(ADCVRPOW));
    SetPersistentTable1B(KeyIndex+188 , LBYTE(ADCVRPOW));
    SetPersistentTable1B(KeyIndex+189 , HBYTE(VDELTAX));
    SetPersistentTable1B(KeyIndex+190 , LBYTE(VDELTAX));
    SetPersistentTable1B(KeyIndex+191 , HBYTE(RSETPOINT));
    SetPersistentTable1B(KeyIndex+192 , LBYTE(RSETPOINT));
    SetPersistentTable1B(KeyIndex+193 , HBYTE(svoPrvtVar_avgFEpp[0]));
    SetPersistentTable1B(KeyIndex+194 , LBYTE(svoPrvtVar_avgFEpp[0]));
    SetPersistentTable1B(KeyIndex+195 , HBYTE(svoPrvtVar_avgFEpp[1]));
    SetPersistentTable1B(KeyIndex+196 , LBYTE(svoPrvtVar_avgFEpp[1]));
    SetPersistentTable1B(KeyIndex+197 , HBYTE(LTHRPJGLVL));
    SetPersistentTable1B(KeyIndex+198 , LBYTE(LTHRPJGLVL));
    SetPersistentTable1B(KeyIndex+199 , HBYTE(svoPrvtVar_RFpp));
    SetPersistentTable1B(KeyIndex+200 , LBYTE(svoPrvtVar_RFpp));
    SetPersistentTable1B(KeyIndex+201 , HBYTE(svoPrvtVar_SledHomeIndex));
    SetPersistentTable1B(KeyIndex+202 , LBYTE(svoPrvtVar_SledHomeIndex));
#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
    SetPersistentTable1B(KeyIndex+203 , HBYTE(Unbalance_midspd));
    SetPersistentTable1B(KeyIndex+204 , LBYTE(Unbalance_midspd));
    SetPersistentTable1B(KeyIndex+205 , HBYTE(Unbalance_highspd));
    SetPersistentTable1B(KeyIndex+206 , LBYTE(Unbalance_highspd));
#endif
    SetPersistentTable1B(KeyIndex+207 , HBYTE(FGPULSECOUNT));
    SetPersistentTable1B(KeyIndex+208 , LBYTE(FGPULSECOUNT));

    SetPersistentTable1B(KeyIndex+209 , svoPrvtVar_wbllockcnt);
    SetPersistentTable1B(KeyIndex+210 , svoPrvtVar_jitter);
    SetPersistentTable1B(KeyIndex+211 , HBYTE(svoPrvtVar_pp_dpd_diff_mv));
    SetPersistentTable1B(KeyIndex+212 , LBYTE(svoPrvtVar_pp_dpd_diff_mv));
}
#endif//#if SERVOKEYPARAMETER





/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
