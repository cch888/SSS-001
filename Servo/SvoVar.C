/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 398 $
* $Date: 11/03/02 5:43p $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globtype.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_afe.h"
#include ".\servo\afe_tbl.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_calib.h"
#include ".\servo\svo_step.h"

#if ((OPU == KES410A)||(OPU == SOHBP4)||(OPU == DXX811)||(OPU == HOPB1300)||(OPU == SFBD412)||(OPU == SFBD414)||(OPU == BDPL2)||(OPU == SOHBP7))
struct   OPUBarcodeT  OPUBarcode;
#endif // ((OPU == KES410A)||(OPU == SOHBP4)||(OPU == DXX811))

struct   FOCSENST  FOCSENS;

/****************************************************************/
/* Put all servo variables here for better resource utilization */
/* Group them into either fast or slow access variables and     */
/* then into each functional sub groups                         */
/****************************************************************/
struct  svoMiscVarT     svoMiscVar;
struct  svoCmdVarT      svoCmdVar;
struct  svoCalVarT      svoCalVar;
struct  svoCalOfsT1VarT svoCalOfsT1Var[2][MAX_TYPE1_OFS_NO];
struct  svoCalOfsT2VarT svoCalOfsT2Var[2][MAX_TYPE2_OFS_NO];
struct  svoCalOfsT3VarT svoCalOfsT3Var[2][MAX_TYPE3_OFS_NO];
struct  svoCalOfsT4VarT svoCalOfsT4Var[MAX_TYPE4_OFS_NO];

#if (MPPBG_SPPBG_CALIB != 0)
struct  svoCalBalOfsT2VarT  svoCalBalOfsT2Var[MAX_TYPE2BAL_OFS_NO][2];
#endif

struct  svoCalGnT1VarT  svoCalGnT1Var[2][MAX_TYPE1_GN_NO];
struct  svoCalGnT2VarT  svoCalGnT2Var[2][MAX_TYPE2_GN_NO];
struct  svoCalGnT3VarT  svoCalGnT3Var[2][MAX_TYPE3_GN_NO];

#if (AFE_NORMALIZER == 1)
struct  svoCalBsNorVarT svoCalBsNorVar[2][MAX_NOR_BS_NO];
#endif // AFE_NORMALIZER == 1

struct  svoCalMisc1VarT svoCalMisc1Var[2];
struct  svoCalMisc2VarT svoCalMisc2Var;
struct  svoCalSvoVarT   svoCalFebcVar[2][CAL_RADIUS_ZONE_NR];
struct  svoCalSvoVarT   svoCalTiltVar[2][CAL_RADIUS_ZONE_NR];

#if COLLI_SWITCH == 1
struct  svoCalColliVarT svoCalColliVar[2];
#endif // COLLI_SWITCH == 1

#if (DPD_COMPENSATE_OFFSET==1)
const BYTE svoDpdReg2Bank[4] = {0,1,1,2};
struct  svoCalDpdVarT   svoCalDpdVar[2][3][21];//add DPD gain compensation
#endif//#if (DPD_COMPENSATE_OFFSET==1)
struct  svoDpdBankFcVarT DesriedDpdSetting;

#if (BD_ENABLE == 1)
struct  svoCalDlVarT    svoCalDlVar[2];
#endif

struct  svoCurvefitCalVarT svoCurvefitCalVar;
struct  svoLpfVarT      svoLpfVar;
#if (CALIBRATION_MANAGEMENT == 0)
BYTE    svoVar_TiltStepperZone; //index of stepper zone for tilt calibration
#endif // (CALIBRATION_MANAGEMENT == 0)

SHORT   svoPreTiltValue;
struct  svoSeekVarT     svoSeekVar;
struct  svoStepVarT volatile svoStepVar;
struct  svoSpeedVarT    svoSpeedVar;
struct  svoTXSpdVarT    svoTXSpdVar;

#if (OPU_BEAM_LANDING_MEAS == 1)
struct svoBeamLandingVarT svoBeamLandingVar;
#endif

/****************************************************************/
/* save the result of the last calibration. Should be read only */
/* except for the following functions.                          */
/* calCalibrateOffset                                           */
/* calCalibrateOffset_binary [obsolete]                         */
/* calCalibrateTEGnOs                                           */
/* calCalibrateRPG                                              */
/* calCalibrateRPBG                                             */
/* calCalibrateTEBGTROOS                                        */
/* calCalibrateRRF                                              */
/****************************************************************/
BYTE last_cal_recd;
BYTE last_cal_result;
BYTE last_cal_result2;
BYTE cal_retry;
BYTE fepp_wrong_layer_chkcnt;
USHORT  TE_step_size[32];
//  Variables whose name starting with "svoVar_" are defined by servo code
//  but used by non-servo code.
//  For the best servo code architecture, these should be moved into the
//  structure of svoVar, and shouldn't be exist here.
//  At this moment, these should be together here, and be monitored.
// TBD? COMMENT! Need to add definitions to all these variables

USHORT      Iram_system_timer_counter;

ULONG       Pre_Iram_system_timer;
ULONG       svoVar_TimeCodeCheckTimer;
BYTE        svo_Var_motorDriverState;
BYTE        Layer_Index;
eDiscDiameter   svoVar_discDiameter;
discKindT   svoVar_discKindT;
BYTE        LJ_FailCnt;


#if (Customer_A1 ==1)
BYTE 		PositionHomeErrorCount;
#endif
BYTE 		MediaRecovery;

FocActSensT svoVar_STM;

ESpindleSpeed svoVar_svoTargetSpeed;

BYTE    svoVar_powerSaveFlag;
ETrackingMethod svoVar_TrackingMethod;
BOOL    svoVar_getDVDFormatflag;

BYTE    svoVar_bByPassFOKCheck;
#if (PDIC_BASE_ON_SRFO == 1)
BYTE    svoVar_bPdicMode;
BYTE    svoVar_bLascMode;
#endif

BYTE    CALIBRATE_SYSTEM_LEAD_IN;
#if (ENABLE_HDDVD == 1)
BYTE    RP2GN;
BYTE    RP2OS;
#endif // (ENABLE_HDDVD == 1)

#if (DEFECT_LENGTH_MEASURE == 1)
struct svoDfMeasVarT svoDfMeasVar;
#endif


#if (DETECT_ECC_DISC == 1)
ecc_code_t  svoVar_eccDiscNum;
#endif // (DETECT_ECC_DISC == 1)

#if ((DVD_UNBALANCE_DISC_CHECK==1)||(CD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
BYTE    svoVar_unbalanceDiscNum;    // unbalance_disc_no
BYTE    fUnbalanceChk_SpdUp;        /*bit 0: measure complete at lower speed; bit 1: at middle speed, bit 2: at higher speed*/
USHORT  Unbalance_midspd;
USHORT  Unbalance_highspd;
USHORT  Unbalance_lowspd;
#endif // ((DVD_UNBALANCE_DISC_CHECK==1)||(CD_UNBALANCE_DISC_CHECK == 1))

vBOOL   svoVar_EccCheckEnable;
vUSHORT svoRunOut;              // (tracks)
vUSHORT svoRunOutCTN;                  // (tracks)

struct svoPcalVarT svoPcalVar;
BYTE    svoDiagStatus;
SHORT   svoDiagReturnValue1;
SHORT   svoDiagReturnValue2;
SHORT   svoDiagReturnValue3;

#if (EN_Persistent_table_8 == 1)
SHORT   svoDiagReturnValue4;
#endif

SHORT   ReadCAL_Dac;
SHORT   WriteCAL_Dac;
SHORT   PeakCAL_Dac;
SHORT   Channel_Mode;
SHORT   CHECK_POWER_MODE;

USHORT  fConut_NUM;
BOOL    fCheck_Pluse_Power;
BOOL    fAPC;

SHORT   svoVar_sPwrCalDac[15];
SHORT   svoVar_sPwrCalValue[15];

BYTE    svoVar_bPwrCalCount;

#ifdef  BYPASS_POWERSAVE_ISR
BOOL    svoFgBaseOnPowerSaveFlag;
#endif // BYPASS_POWERSAVE_ISR

#if LPP_AUTO_LEARN_TEST == 1
BYTE    svoVar_optLPPTH3;
BYTE    svoVar_optLPPTH4;
#endif // LPP_AUTO_LEARN_TEST == 1

BYTE    svo_bDiscTblNo;
BYTE    svo_bSMPTblNo;
BOOL    svo_fgDramStgyFill;

BYTE    bFocMoveReference;
BYTE    bLayerErrorCount;

/*--------------------------------------------------------------------------*/
/* Variables for AFE                                                        */
/*--------------------------------------------------------------------------*/
BYTE    svoPrvtVar_LD_ON_Flag;

//----------------------------------------------/
// Introduce table and index for error logging  /
//----------------------------------------------/
BYTE    tbl_error_log_idx;
BYTE    tbl_error_log[SIZE_ERR_LOG];

BOOL    svoVar_abort_pause_if_applyable;
#if (APP_TE_SUPPORT == 1)
BYTE    svoVar_mpp_os; //store the normal MPP offset for APP OPU
#endif
USHORT  vref_fm_mux3_mV;
USHORT  v21_fm_mux1_mV;

USHORT  svo_bReadZoneSpeedX10;               // read zone speed in tenths (X = 50 -> svo_bReadZoneSpeedX10 = 500)

BYTE    svoGetIDErrCount;
BYTE    svoSeekErrCnt;
BYTE    svoSlipSign;
BYTE    fgseektime;
ULONG   seektime;
#if (BD_ENABLE == 1)
BOOL    svo_BDR_RE_PICReading;
#endif
BOOL    SeekBusy = FALSE;

#if (DeadLoop_Solution == 1)
SBYTE   svoWblErrCount;
SBYTE   svo_ERR_STARTUP;
#endif // (DeadLoop_Solution == 1)

#if (DVD_RAM_READ == 1)
USHORT  WblPllFreqPreset;
BYTE    TrackPitchChange;
SBYTE   zone_change;
BYTE    current_zone;
BYTE    target_zone;
BOOL    DVDRAM_embossed;
BOOL    DVDRAM_rewritable;
    #if (CHIP_REV >= 0xC0)
BYTE            land_flywheel_state;
BYTE            slip_confidence_level;
BYTE            HDRFLYCFD_pre;
    #endif
#endif

LONG    svoLIBoundary;
BOOL    svo_BoundaryDet;

BOOL    bSetSpeedAgain;

#if (CHIP_REV >= 0xC0)
BYTE            svoPrvtVar_FEDG_thr;
SHORT           svoPrvtVar_ProtFoos;
#endif

BYTE    svoPrvtVar_focusP2PCount;                       // focus_pp_count;
SHORT   svoPrvtVar_avgFOO;
USHORT  svoPrvtVar_avgFEpp[2];                          // mV
USHORT  svoPrvtVar_maxFEpp[2];                          // mV
USHORT  svoPrvtVar_avgBSpp[2];                          // mV
USHORT  svoPrvtVar_avgSRFO[2];                          // mV
USHORT  svoPrvtVar_maxSRFO;                             // mV
USHORT  svoPrvtVar_maxSRFOpp_0dB;                       // mV
USHORT  svoPrvtVar_maxFEpp_0dB;
BYTE    svoPrvtVar_TEBSRatioX100;
#if (SERVOKEYPARAMETER == 1)
USHORT  svoPrvtVar_maxFEpp_0dB;
USHORT  svoPrvtVar_RFpp;                                // mV
USHORT  RSETPOINT;                                      //
USHORT  ADCVRPOW;                                       //
USHORT  VDELTAX;                                        //
USHORT  svoPrvtVar_MPPO_0dB;
BYTE    svoPrvtVar_MPPOSRFORatioX100;
USHORT  svoPrvtVar_SledHomeIndex;                       //20090305_s.22 == add sledge home info to key parameter
SHORT   LTHRPJGLVL;                                     // LTH RP judge level //20090324_f.26 == add FEPPL0/L1 and LTHRPJGLVL
BYTE    svoPrvtVar_troos;                               // 20090420_i.29 savekeyparameter troos;
BYTE    svoPrvtVar_wbllockcnt;                          //
BYTE    svoPrvtVar_jitter;                              //
SHORT   svoPrvtVar_pp_dpd_diff_mv;                      // TEpp(PP+DPD)-TEpp(PP-DPD)
#endif

BYTE    svoStmSubMissRty;

#if (EN_ADI == 1)
BYTE    servo_status;
#endif

#if (FE_SLEW_RATE_LIMITER == 2)
svoDWordU       SlewRateShift;
#endif

// added Vendor IOPTest command.
USHORT  ADCVRPOW_CD;                                    //
USHORT  VDELTAX_CD;                                     //
USHORT  ADCVRPOW_DVD;                                   //
USHORT  VDELTAX_DVD;                                    //
USHORT  ADCVRPOW_BD;                                    //
USHORT  VDELTAX_BD;                                     //
USHORT  FMTHROFFSET_CD;
USHORT  FMTHROFFSET_DVD;
USHORT  FMTHROFFSET_BD;
USHORT  FMD_PPLVL_CD;
USHORT  FMD_PPLVL_DVD;
USHORT  FMD_PPLVL_BD;
USHORT  FMD_DCLVL_CD;
USHORT  FMD_DCLVL_DVD;
USHORT  FMD_DCLVL_BD;
USHORT  FGPULSECOUNT;

#if (MONITOR_OPU_TEMPERATURE == 1)
ULONG   svoReadTempTimer;
SBYTE   OpuTemperature;
#endif /* MONITOR_OPU_TEMPERATURE=1 */

struct svoRRFVarT svoRRFVar;
struct svoRampMeasVarT svoRampMeasVar;
#if (STREAM_STM == 1)
struct streamMeasVarT streamMeasVar;
#endif
#if Monitor_FPDO_Linearity ==1
BYTE    bEnMonitorDac;
USHORT  VWDCDAC;
#endif
SBYTE   RfHeaderOffset;

const USHORT  CalTestPos[5] = {
    UM_2_SLED_STEP(CAL_RADIUS_TEST_POINT0),
    UM_2_SLED_STEP(CAL_RADIUS_TEST_POINT1),
    UM_2_SLED_STEP(CAL_RADIUS_TEST_POINT2),
    UM_2_SLED_STEP(CAL_RADIUS_TEST_POINT3),
    UM_2_SLED_STEP(CAL_RADIUS_TEST_POINT4)
};

#if (SUN710_TRAY_PROFILE)
BYTE    Tray_Phase;
USHORT  Tray_Out_S2_Time;
#endif

#if (OPEN_SESSION == 1)
BYTE    RF_search_count;
#endif
SHORT   Recorded_stp_Position[2];
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
BYTE    RetryDiscLaser;
#endif

#if (RP_AUTO_ADJUST == 1)
BYTE    RPOfsAdjust;
LONG    CalRP2Gain[2];
LONG    RPAcc;
LONG    RPAccPrv;
#endif

#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
BYTE DVDHightJitter;
#endif

#if(CDDVDNewBlankChk == 1)
BYTE track_off_rf_dif_fail;
BYTE RecordedCount;
#endif

#if (ENABLE_BDIH == 1)
BYTE bd_hybrid;
#endif
#ifdef RETRY_CHG_EQTAPA12
BYTE    retry_DFE_EqualizerStrength;
#endif

#if (EN_BD_ADEQ == 1) || (EN_ADEQ == 1)
BYTE    Disable_ADEQ;
#endif

#if (WOB_BASED_DEMODULATOR == 1)
BYTE    wob_based_demod;
#endif

#if (KFGTG_VAL_WI_BOUNDARY ==1 )
BYTE    bsamplecnt;
LONG    LadTE,LadFE;
USHORT  KFG_OK_LEVEL_mv,KTG_OK_LEVEL_mv;
#endif

#if (HORROR_DISC_IMPROVEMENT == 1)
USHORT    read_retry_option,read_retry_status;
ULONG   read_retry_restartTime;
BYTE    read_retry,dfe_retry,FEBC_retry,Tilt_retry;
BYTE    fgNormalValueSaved;
BYTE    fgNormalValueChgAgain;
BYTE    BufferErrorForRetry;
SHORT           initFEBC,tempFEBC,currZone;
SHORT           initTilt,tempTilt;

ULONG SmartRtyNormalValue[Smart_Retry_Para_NO];

reg_field_t const Smart_Retry_Info[Smart_Retry_Para_NO] =
{
    {PLLKP,    0,4,  0x06,  0x01,  HAL_DFE_FPLLKPCFG_ADR,   HAL_DFE_FPLLKPCFG_MASK,  HAL_DFE_FPLLKPCFG_LSB },  // 4
    {PLLKI,    0,4,  0x06,  0x01,  HAL_DFE_FPLLKICFG_ADR,   HAL_DFE_FPLLKICFG_MASK,  HAL_DFE_FPLLKICFG_LSB },   // 4
    {EQTAP1,   0,4,  0x06,  0x01,  HAL_DFE_EQTAPA1_ADR  ,   HAL_DFE_EQTAPA1_MASK  ,  HAL_DFE_EQTAPA1_LSB   },
    {EQTAP2,   0,4,  0x06,  0x01,  HAL_DFE_EQTAPA2_ADR  ,   HAL_DFE_EQTAPA2_MASK  ,  HAL_DFE_EQTAPA2_LSB   },
    {SLICERBW, 0,4,  0x08, 0x03, HAL_DFE_SLCBW_ADR,       HAL_DFE_SLCBW_MASK,        HAL_DFE_SLCBW_LSB  },
    {LEQTAPGAIN , 0,4,  0x3F, 0x00, HAL_DFE_TAPGAIN_ADR,      HAL_DFE_TAPGAIN_MASK,      HAL_DFE_TAPGAIN_LSB   },
    {LEQCLIPLVL , 0,4,  0x1FF, 0x00, HAL_DFE_CLIPLVL_ADR,       HAL_DFE_CLIPLVL_MASK,        HAL_DFE_CLIPLVL_LSB   },
    {MAHPFBW , 0,4,  0x07, 0x00, HAL_DFE_MAHPFBW_ADR,     HAL_DFE_MAHPFBW_MASK,     HAL_DFE_MAHPFBW_LSB   }, //RF Moving Average HPF
    {AGCINTBW, 0,4,  0x0F,  0x0A,  HAL_DFE_AGCINTBW_ADR ,   HAL_DFE_AGCINTBW_MASK ,  HAL_DFE_AGCINTBW_LSB  },
    {AGCWINPRD,0,4,  0x05,  0x00,  HAL_DFE_AGCWINPRD_ADR,   HAL_DFE_AGCWINPRD_MASK,  HAL_DFE_AGCWINPRD_LSB },
    {AOCINTBW, 0,4,  0x0F,  0x0A,  HAL_DFE_AOCINTBW_ADR ,   HAL_DFE_AOCINTBW_MASK ,  HAL_DFE_AOCINTBW_LSB  },
    {AOCWINPRD,0,4,  0x05,  0x00,  HAL_DFE_AOCWINPRD_ADR,   HAL_DFE_AOCWINPRD_MASK,  HAL_DFE_AOCWINPRD_LSB },
};

SBYTE const Smart_Retry_Para_CD[MaxRetryNR][Smart_Retry_Para_NO] =
{
   //PLLKP  PLLKI  EQTAP1  EQTAP2  SLICERBW  LEQTAPGAIN  LEQCLIPLVL  MAHPFBW  AGCINTBW  AGCWINPRD  AOCINTBW  AOCWINPRD
    {  0,     0,     0,      0,       0,        4,          8,         0,       0,        0,         0,       0}, //retry 1
    {  0,     1,     0,      0,       0,        4,          4,         0,       0,        0,         0,       0}, //retry 2
    {  1,     2,     0,      0,       0,        0,         -4,         0,       0,        0,         0,       0}, //retry 3
    {  0,     0,     0,      0,       0,        0,         -2,         0,       0,        0,         0,       0}, //retry 4
    {  0,    -1,     0,      0,       0,        4,         22,         0,       0,        0,         0,       0}, //retry 5
    { -1,    -1,     0,      0,       0,        4,         26,         0,       0,        0,         0,       0}, //retry 6
    { -1,    -1,     0,      0,       0,        0,          4,         0,       0,        0,         0,       0}, //retry 7
    { -1,    -3,     0,      0,       0,        0,          8,         0,       0,        0,         0,       0}, //retry 8
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        1,        -1,       0}, //retry 9
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,       -1,        -1,       0}, //retry 10
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 11
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 12
};

SBYTE const Smart_Retry_Para_DVD[MaxRetryNR][Smart_Retry_Para_NO] =
{
   //PLLKP  PLLKI  EQTAP1  EQTAP2  SLICERBW  LEQTAPGAIN  LEQCLIPLVL  MAHPFBW  AGCINTBW  AGCWINPRD  AOCINTBW  AOCWINPRD
    {  0,     0,     0,      0,       0,        4,          8,         0,       0,        0,         0,       0}, //retry 1
    {  0,     1,     0,      0,       0,        4,          4,         0,       0,        0,         0,       0}, //retry 2
    {  1,     2,     0,      0,       0,        0,         -4,         0,       0,        0,         0,       0}, //retry 3
    {  0,     0,     0,      0,       0,        0,         -2,         0,       0,        0,         0,       0}, //retry 4
    {  0,    -1,     0,      0,       0,        4,         22,         0,       0,        0,         0,       0}, //retry 5
    { -1,    -1,     0,      0,       0,        4,         26,         0,       0,        0,         0,       0}, //retry 6
    { -1,    -1,     0,      0,       0,        0,          4,         0,       0,        0,         0,       0}, //retry 7
    {  0,    -1,     0,      0,       0,        0,          8,         0,       0,        0,         0,       0}, //retry 8
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        1,        -1,       0}, //retry 9
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,       -1,        -1,       0}, //retry 10
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 11
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 12
};

SBYTE const Smart_Retry_Para_BDROM[MaxRetryNR][Smart_Retry_Para_NO] =
{
   //PLLKP  PLLKI  EQTAP1  EQTAP2  SLICERBW  LEQTAPGAIN  LEQCLIPLVL  MAHPFBW  AGCINTBW  AGCWINPRD  AOCINTBW  AOCWINPRD
    {  0,     0,     0,      0,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 1
    {  1,     1,     1,      1,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 2
    {  2,     2,     2,      2,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 3
    {  3,     3,     3,      3,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 4
    {  4,     4,     4,      4,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 5
    {  5,     5,     5,      5,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 6
    {  3,     3,     3,      3,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 7
    {  4,     4,     4,      4,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 8
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        1,        -1,       0}, //retry 9
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,       -1,        -1,       0}, //retry 10
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 11
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 12
};

SBYTE const Smart_Retry_Para_BDR[MaxRetryNR][Smart_Retry_Para_NO] =
{
   //PLLKP  PLLKI  EQTAP1  EQTAP2  SLICERBW  LEQTAPGAIN  LEQCLIPLVL  MAHPFBW  AGCINTBW  AGCWINPRD  AOCINTBW  AOCWINPRD
    {  0,     0,     0,      0,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 1
    {  1,     1,     1,      1,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 2
    {  2,     2,     2,      2,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 3
    {  3,     3,     3,      3,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 4
    {  4,     4,     4,      4,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 5
    {  5,     5,     5,      5,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 6
    {  3,     3,     3,      3,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 7
    {  4,     4,     4,      4,       0,        0,          0,         0,       0,        0,         0,       0}, //retry 8
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        1,        -1,       0}, //retry 9
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,       -1,        -1,       0}, //retry 10
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 11
    {  0,     0,     0,      0,       0,        0,          0,         0,      -1,        0,         0,       0}, //retry 12
};
#endif // (HORROR_DISC_IMPROVEMENT == 1)
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
