/****************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* FILENAME:   SVOVAR.H
*
* DESCRIPTION
*   Servo Module global variable and constant definitions.
*
*   $Revision: 270 $
*   $Date: 11-05-03 15:30 $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVOVAR_
#define _SVOVAR_

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svo_calib.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\al\REG_afe.h"

#if (HORROR_DISC_IMPROVEMENT == 1)
#include ".\al\REG_dfe.h"
#endif

#if (HORROR_DISC_IMPROVEMENT == 1)
typedef enum
{
    PLLKP = 0,
    PLLKI,
    EQTAP1,
    EQTAP2,
    SLICERBW,
    LEQTAPGAIN,
    LEQCLIPLVL,
    MAHPFBW,
    AGCINTBW,
    AGCWINPRD,
    AOCINTBW,
    AOCWINPRD,    
    ParaNR
}SamrtRetryPara;

typedef struct reg_field_s
{
    unsigned char   index;
    unsigned char   updated_after_spdchg;
    unsigned char   reg_length;
    unsigned long   max_value;
    unsigned long   min_value;
    unsigned long   addr;
    unsigned long   mask;
    unsigned char   start_bit;
}reg_field_t;


#define Smart_Retry_Para_NO ParaNR
#define MaxRetryNR          12
#define FebcRetryNR         11

extern reg_field_t const Smart_Retry_Info[Smart_Retry_Para_NO];
extern SBYTE const Smart_Retry_Para_CD[MaxRetryNR][Smart_Retry_Para_NO];
extern SBYTE const Smart_Retry_Para_DVD[MaxRetryNR][Smart_Retry_Para_NO];
extern SBYTE const Smart_Retry_Para_BDROM[MaxRetryNR][Smart_Retry_Para_NO];
extern SBYTE const Smart_Retry_Para_BDR[MaxRetryNR][Smart_Retry_Para_NO];
extern ULONG     SmartRtyNormalValue[Smart_Retry_Para_NO];

extern USHORT   read_retry_option,read_retry_status;
extern ULONG  read_retry_restartTime;
extern BYTE     read_retry,dfe_retry,FEBC_retry,Tilt_retry;
extern BYTE    fgNormalValueSaved;
extern BYTE    fgNormalValueChgAgain;
extern SHORT    initFEBC,tempFEBC,currZone;
extern SHORT    initTilt,tempTilt;
#endif

#ifdef RETRY_CHG_EQTAPA12
extern BYTE    retry_DFE_EqualizerStrength;
#endif

        #if ((EN_BD_ADEQ == 1) || (EN_ADEQ == 1))
extern BYTE     Disable_ADEQ;
        #endif

#if (WOB_BASED_DEMODULATOR == 1)
extern BYTE    wob_based_demod;
#endif

#if (KFGTG_VAL_WI_BOUNDARY ==1 )
extern BYTE     bsamplecnt;
extern LONG     LadTE,LadFE;
extern USHORT   KFG_OK_LEVEL_mv,KTG_OK_LEVEL_mv;

#define KFG_OK_MES_WIN       8 
#define KTG_OK_MES_WIN       8
#define KFG_THRE_FACTOR	     4
#define KTG_THRE_FACTOR	     4   
#define KFG_mv_LOW_LIMIT	 400
#define KTG_mv_LOW_LIMIT	 400 
#endif


typedef enum
{
   RAMPING_END,
   RAMPING_UP,
   RAMPING_DOWN,
   RAMPING_FAIL,
   RAMPING_RETRY
} RAMP_MODE_t;

typedef enum
{
   SPD_MODE_CAV,
   SPD_MODE_CLV,
   SPD_MODE_PCAV,
   SPD_MODE_ZCLV,
} SPD_MODE_t;

struct  svoMiscVarT {
//
//  Variables whose name starting with "svoVar_" are defined by servo code
//  but used by non-servo code.
//  For the best servo code architecture, these should be included in this
//  structure, and shouldn't be used by non-servo code.
//  At this moment, these are defined in SVO_VAR.C and SVO.H files.
//  Let's keep them here with "//" at the front for the reference.
//  Later on we have move them into the svoVar structure.
//

/*--------------------------------------------------------------------------*/
/* Variables for the state machine                                          */
/*--------------------------------------------------------------------------*/
SEQ_ARG_TBL     *pSeqArgs;          // *seq_arg
USHORT          svoInStateSaved;    // store_svoInState
BYTE            leadInOutFlag;      // F_lead;
#if (ENABLE_HDDVD == 1)
BYTE            HD_System_leadInOutFlag;      // F_system_lead;
#endif

/*--------------------------------------------------------------------------*/
/* Variables for speed / spindle                                            */
/*--------------------------------------------------------------------------*/
BYTE            atipdisc_subq_diei_flag;
BYTE            zoneAreaCheckFlag;  // zone_area_check_flag;

/*--------------------------------------------------------------------------*/
/* Variables for CAV control                                                */
/*--------------------------------------------------------------------------*/
//BYTE          cavRegTableIndex;   // now_cav_tbl_no;

/*--------------------------------------------------------------------------*/
/* Variables for timers                                                     */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* Variables for error                                                      */
/*--------------------------------------------------------------------------*/
BYTE            searchRFCount;
BYTE            trackSlipCount;
BYTE            focusRetryCount;
BYTE            focusRampCount;
BYTE            LayerjumpRetryCount;
BOOL            LayerJumpErrorFlag;
#if(DISC_MIS_JUDGE_RECOVERY)
BYTE            retryMediaType;
#endif
#if(FDP_TRKSLIP_MONITOR == 1 )
BYTE            focusDropCount;
BOOL            forceFocusOffFlag;
BOOL            forceTrackOffFlag;
#endif
#if(ERROR_CAL_START_UP_MONITOR == 1)
BYTE           calibrationErrorCount;
#endif
};

/*--------------------------------------------------------------------------*/
/* Variables for servo command                                              */
/*--------------------------------------------------------------------------*/
struct  svoCmdVarT {
USHORT          svoCommand;         // cmd_no
BYTE            cmdParam[10];       // cmd_para
BYTE            playerSpeed;        // player_speed;
BYTE            outputFormat;       // output_format;
SVOTIDInfoU     targetID;           // TargetID;
SHORT           commandedTrksToGo;  // svoCmdJumpTrk;
eDemodMode      demodMode;          // demod mode

/*--------------------------------------------------------------------------*/
/* Variables for error                                                      */
/*--------------------------------------------------------------------------*/
BYTE            servoError;         // ErrorInfo;
BYTE            discError;          // DiscError;
BYTE            playerError;        // PlayerError;
BYTE            commandError;       // CommandError;
BYTE            trayError;          // TrayError;
};

/*--------------------------------------------------------------------------*/
/* Varibale for Servo Calibrations                                          */
/*--------------------------------------------------------------------------*/
struct  svoCalVarT
{
BYTE            discChkDoneBits;    // disc kind/type detection done flag!!
BYTE            calDoneBits;        // adjust_end_flag;
USHORT          calGainDoneBits;    // afe gain calibration done flag!!
USHORT          calOfsDoneBits;     // afe offset calibration done flag!!
svoWordU        ceOfsData;          //  ceofData;
svoWordU        teOfsData;          //  teofData;
svoWordU        feOfsData;          //  feofData;
svoWordU        rpOfsData;          //  riofData;
svoWordU        bsOfsData;          //  bsofData;
svoWordU        fmThrData;          // FrontMonitor Threshold Adjustment
svoWordU        fmDelData;          // FrontMonitor Delta Adjustment
};

struct  svoCalOfsT1VarT
{
BYTE            write_offset;
#if (CHIP_REV >= 0xC0)
    BYTE            land_write_offset;
#endif
BYTE            li_offset;
BYTE            groove_offset[2];
BYTE            land_offset[2];
};

struct  svoCalOfsT2VarT
{
BYTE            write_offset;
BYTE            normal_offset;
};

struct  svoCalOfsT3VarT
{
BYTE            normal_offset;
};

struct  svoCalOfsT4VarT
{
BYTE            normal_offset;
};

#if (MPPBG_SPPBG_CALIB != 0)
struct  svoCalBalOfsT2VarT
{
SBYTE           normal_offset;
};
#endif

struct  svoCalGnT1VarT
{
BYTE            write_gain;
BYTE            li_gain;
BYTE            groove_gain[2];
BYTE            land_gain[2];
};

struct  svoCalGnT2VarT
{
BYTE            write_gain;
BYTE            normal_gain;
};

struct  svoCalGnT3VarT
{
BYTE            normal_gain;
};

#if (AFE_NORMALIZER == 1)
struct  svoCalBsNorVarT
{
BYTE            normal_bs;
};
#endif

struct svoCalMisc1VarT
{
    USHORT          feGainDSP;
    USHORT          teGainDSP;
    SHORT           feSetPoint;
    SHORT           teSetPoint;
};

struct svoCalMisc2VarT
{
SHORT           fooos;
SBYTE           troos;
SHORT           pdic_gn;
SHORT           laser_gn;
};

struct svoCalSvoVarT
{
SHORT           opt_value;
USHORT          sled_pos;
ULONG           cal_timer;
BYTE            status;
};

#if (COLLI_SWITCH == 1)
struct svoCalColliVarT
{
SHORT           opt_value;
ULONG           cal_timer;
BYTE            status;
    #if (COLLI_TEMP_SLOPE != 0)
SBYTE           temperature;
    #endif
#if (COLLI_POS_RECOVERY==1)
SHORT           startup_value;
#endif
};
#endif

#if (DPD_COMPENSATE_OFFSET==1)
struct svoCalDpdVarT
{
BYTE            status;
SBYTE           dpd_offset;
SBYTE           dpd_gain;//add DPD gain compensation
};
#endif

#if (BD_ENABLE == 1)
struct svoCalDlVarT
{
USHORT          dpp_thr;
USHORT          fe_factor;  //unit: 1/1024
    #if (NEW_BDDL_START_FLOW == 1)
USHORT          fe_0db;
USHORT          srfo_0db;
    #endif
};
#endif

struct svoRRFVarT
{
USHORT          step_size[0x20];
SHORT           ofs_ldoff_cur;
BYTE            modulation[2];
USHORT          rfcos_step_size[(2 << (HAL_AFE_RFGN_MSB-HAL_AFE_RFGN_LSB))];
#if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 1)
USHORT          rfpos_step_size;
#endif
BYTE            rfcos_shift_cur[2];
BYTE            rfcos_shift_max;
SBYTE           ofs_ldoff[(2 << (HAL_AFE_RFGN_MSB-HAL_AFE_RFGN_LSB))];
//RF Gain offset calibration
BYTE            ok_cnt;
USHORT          RRFpp_pre;
SBYTE           adj_gn_pre;
#if (NEW_RFPOS_CALIB == 1)
SBYTE           rfagc_ref_LPF[3][32];
SHORT           rfagc_ref;
    #if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
SHORT           rfaoc_ref[2];
    #endif
#endif
};

struct svoRampMeasVarT
{
SHORT           refSRFO;     //unit GPADC
SHORT           refFE;       //unit DSPADC
#if (STREAM_STM == 0)
SHORT           refFEO;      //unit GPADC
#else
SHORT           refFEO;      //unit GPADC
SHORT           refBS;       //unit DSPADC
#endif
USHORT          SRFOpp[2];   //unit GPADC
USHORT          FEpp[2];     //unit DSPADC
USHORT          BSpp[2];     //unit DSPADC
USHORT          LayerTime[2];//unit mSec
SHORT           SubstFoo;    //unit FOO dac
USHORT          SubstSRFO;   //unit GPADC
SHORT           LayerFoo;    //unit FOO dac
SHORT           SRFO_gain;   //unit 0.1dB
USHORT          STM;         //unit um
USHORT          timeout;
USHORT          pre_timeout;
BOOL            saturated;
};

#if (STREAM_STM == 1)
struct streamMeasVarT
{
USHORT          SRFOpp[3];   //unit GPADC
USHORT          FEpp[3];     //unit DSPADC
USHORT          BSpp[3];     //unit DSPADC
USHORT          LayerTime[3];//unit mSec
SHORT           SubstFoo;    //unit FOO dac
USHORT          SubstSRFO;   //unit GPADC
USHORT          SubstTime;   //unit is stream sampling time
USHORT          SubstFEpp;   //unit GPADC
SHORT           LayerFoo;    //unit FOO dac
SHORT           RampStartfoo;//unit id FOO dac
BOOL            saturated;
};
#endif
struct svoCurvefitCalVarT
{
float  cof[3];
float  item[20];
float  cost[20];
float  delta_cost;  //this decide how deep a curve we want.
float  worst_cost;  //the worst cost we allowed
float  opt_cost;    //the best measured cost in the calibration
SHORT  item_step;   //item value difference between samples
SHORT  item_max;    //maximum available item value
SHORT  item_min;    //minimum available item value
SHORT  orig_item;   //the item before calibration
BYTE   up_curve;    //True for jitter, False for TE, RRFpp
BYTE   cal_index;   //number of measured samples
BYTE   calDoneBits;
RAMP_MODE_t ramp_mode;
};

struct svoSTMVarT
{
USHORT  STM_maxFEpp[3];
USHORT  STM_maxSRFO[3];
SHORT   LayerFoo[3];
BYTE    STM_LayerNumber[3];
BYTE    STM_BypassRamp;
USHORT  STM[3];
};

typedef struct svoDpdBankFcVarT
{
BYTE    DpdBwBank;
SBYTE   DpdBwFc;
BYTE    DpdHpfBw;
}svoDpdBankFcVar_t;

struct svoLpfVarT
{
LONG    InPrv;
LONG    OutPrv;
LONG    coefA1;
LONG    coefB1B2;
};

#if (OPU_BEAM_LANDING_MEAS == 1)
struct svoBeamLandingVarT
{
    USHORT segment_ref_mv[4];//A,B,C,D
};
#endif
/*--------------------------------------------------------------------------*/
/* Variables for Sled / Step motor                                          */
/*--------------------------------------------------------------------------*/
struct  svoStepVarT {
USHORT          StepHomePos;
BYTE            StepPosCal;
BYTE            stepHomeCheck;      // step_home_check;
USHORT          SledCounter;
SBYTE           StepThresholdPosTarget;
SBYTE           StepThresholdNegTarget;
SHORT           StepThresholdPos;
SHORT           StepThresholdNeg;
BYTE            StepThresholdDelayTarget;
BYTE            StepThresholdDelay;
};

/*--------------------------------------------------------------------------*/
/* Variables for spindle motor                                              */
/*--------------------------------------------------------------------------*/
struct  svoSpeedVarT
{
    vULONG          fg_pulse_rev;
    vULONG          fgTotalTime;
    vUSHORT         fgPulseTime;            
    vUSHORT         fgCount;   
    vUSHORT         fg_pulse[DISK_NFGPLS];
    LONG            Kd10Fg;
    #if (NEW_SPINDLE_CONTROL == 1)
    LONG            Kd16Fg;
    #endif
    USHORT          Kd14Fg;
    USHORT          LostFg;
    USHORT          Kd10FgMin;
    USHORT          MaximumRpm;
    USHORT          CavSwitchPos;
    vUSHORT         TargetRpm;
    #if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
    vUSHORT         TargetRpmReal;
    #endif
    vUSHORT         CurrentRpm;
    vUSHORT         MinimumRpm;
    vUSHORT         SpdOverspd;
    vUSHORT         SpdOldRevcnt;
    vUSHORT         SpdWaitCnt;
    #if (NEW_SPINDLE_CONTROL == 1)
    vSHORT          SpdIntPara3;
    vBYTE           SpdDE4Ratio;
    vBYTE           SpdInRangeCnt;
    #endif
    BYTE            SpdAfcg;
    vBYTE           SpdMode:2;
    vBYTE           SpdHold:1;
    vBYTE           SpdInRange:1;
    vBYTE           SpdMonitor:1;
    vBYTE           SpdUp:1;
    vBYTE           SpdDown:1;
    vBYTE           SpdSRCEnable:1;
    vBYTE           SpdSetMaxRPM:1;
    vBYTE           SpdPresetNeeded:1;
    vBYTE           SpdOldRevcntChk:1;
    #if (SPD_MAX_BRAKE_RPM > 0)
    vBYTE           SpdFreeRun:1;
    #endif
    ESpindleSpeed   SpdCurrentSpeed;
};

typedef struct signed_int_readT
{
    BOOL        succ;
    SHORT       val;
} signed_int_read_t;

/*--------------------------------------------------------------------------*/
/* Variables for TXSPEED monitor                                            */
/*--------------------------------------------------------------------------*/
struct  svoTXSpdVarT {
USHORT sum;
#if (CHIP_REV <= 0xB3)
USHORT pre_trk;
BYTE   pre;
#endif
#if(BD_ECC_IMPROVEMENT == 1)
BYTE   monitor_cnt;
#endif
};

#if (DEFECT_LENGTH_MEASURE == 1)
struct svoDfMeasVarT
{
    USHORT  temp_length;
    USHORT  defect_length;
    BYTE    temp_count;
    BYTE    defect_count;
    BYTE    duration;
};
#endif

/*--------------------------------------------------------------------------*/
/* Variables for AFE                                                        */
/*--------------------------------------------------------------------------*/
extern BYTE svoPrvtVar_LD_ON_Flag;
extern struct   svoMiscVarT     svoMiscVar;
extern struct   svoCmdVarT      svoCmdVar;
extern struct   svoCalVarT      svoCalVar;
extern struct   svoCalMisc1VarT svoCalMisc1Var[2];
extern struct   svoCalMisc2VarT svoCalMisc2Var;
extern struct   svoCalSvoVarT   svoCalFebcVar[2][CAL_RADIUS_ZONE_NR];
extern struct   svoCalSvoVarT   svoCalTiltVar[2][CAL_RADIUS_ZONE_NR];
#if COLLI_SWITCH ==1
extern struct   svoCalColliVarT svoCalColliVar[2];
#endif
#if (DPD_COMPENSATE_OFFSET==1)
extern const BYTE svoDpdReg2Bank[4];
extern struct   svoCalDpdVarT   svoCalDpdVar[2][3][21];
#endif//#if (DPD_COMPENSATE_OFFSET==1)
extern struct   svoDpdBankFcVarT DesriedDpdSetting;
#if (BD_ENABLE == 1)
extern struct   svoCalDlVarT    svoCalDlVar[2];
#endif
extern struct   svoCurvefitCalVarT svoCurvefitCalVar;
extern struct   svoLpfVarT      svoLpfVar;
extern struct   svoStepVarT volatile svoStepVar;
extern struct   svoSpeedVarT    svoSpeedVar;
extern struct   svoTXSpdVarT    svoTXSpdVar;
extern BYTE     last_cal_recd;
extern BYTE     last_cal_result;
extern BYTE     last_cal_result2;
extern BYTE     cal_retry;
extern BYTE     fepp_wrong_layer_chkcnt;
extern BOOL     svoVar_abort_pause_if_applyable;
#if (APP_TE_SUPPORT == 1)
extern BYTE     svoVar_mpp_os; //store the normal MPP offset for APP OPU
#endif
extern USHORT   vref_fm_mux3_mV;
extern USHORT   v21_fm_mux1_mV;
extern USHORT   TE_step_size[32];
#if (OPU_BEAM_LANDING_MEAS == 1)
extern struct   svoBeamLandingVarT svoBeamLandingVar;
#endif
/*--------------------------------------------------------------------------*/
/* Variables for OPU table                                                  */
/*--------------------------------------------------------------------------*/
#ifdef EN_DISK_EQ_HOLD
extern BYTE fgDiskFgEQHold;
#endif

extern USHORT svo_bReadZoneSpeedX10;
extern BYTE   svoGetIDErrCount;
extern BYTE   svoSeekErrCnt;
extern BYTE   fgseektime;
extern ULONG  seektime;
extern BOOL   SeekBusy;
extern BYTE   svoSlipSign;
#if (DeadLoop_Solution == 1)
extern SBYTE svoWblErrCount;
extern SBYTE svo_ERR_STARTUP;
#endif
    #if (DVD_RAM_READ == 1)
extern USHORT WblPllFreqPreset;
extern BYTE   TrackPitchChange;
extern SBYTE  zone_change;
extern BYTE   current_zone;
extern BYTE   target_zone;
extern BOOL   DVDRAM_embossed;
extern BOOL   DVDRAM_rewritable;
    #endif
extern LONG svoLIBoundary;
extern BOOL svo_BoundaryDet;
extern BYTE Layer_Index;
extern BOOL bSetSpeedAgain;
extern BYTE LJ_FailCnt;
    #if (CHIP_REV >= 0xC0) //This implemented code is for bug 5555 of C0 chip.
extern BYTE     land_flywheel_state;
extern BYTE     slip_confidence_level;
extern BYTE     HDRFLYCFD_pre;
    #endif

    #if (CHIP_REV >= 0xC0)
extern BYTE     svoPrvtVar_FEDG_thr;
extern SHORT    svoPrvtVar_ProtFoos;
    #endif

#if Monitor_FPDO_Linearity ==1
extern BYTE bEnMonitorDac;
extern USHORT VWDCDAC;
#endif
extern SBYTE RfHeaderOffset;
extern const USHORT  CalTestPos[5];
extern SHORT Recorded_stp_Position[2];
#if (OPEN_SESSION == 1)
extern BYTE RF_search_count;
#endif

#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
extern BYTE DVDHightJitter;

#define DVDROM_HIGHJITTER_THLD  100
#define JITTER_SAMPLE_CNT  8
#define DVDROMSL_HIGH_JITTER 0x01
#endif

#if (ENABLE_BDIH == 1)
extern BYTE bd_hybrid;
#endif

#if(CDDVDNewBlankChk == 1)
extern BYTE track_off_rf_dif_fail;
extern BYTE RecordedCount;
#endif
#if (DEFECT_LENGTH_MEASURE == 1)
extern struct   svoDfMeasVarT svoDfMeasVar;
#endif
#if (FE_SLEW_RATE_LIMITER == 2)
extern svoDWordU SlewRateShift;
#endif

#endif   /* #ifndef _SVOVAR_ */
/*--------------------------------------------------------------------------*/
/* The end of file                                                          */
/*--------------------------------------------------------------------------*/
