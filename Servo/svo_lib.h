/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   svo_lib.h
*
* DESCRIPTION
*
*
*
* NOTES:
*
*
*
* $Revision: 244 $
* $Date: 11/04/27 11:03a $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_LIB_
#define _SVO_LIB_

#include ".\servo\svo.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_dsp.h"
#include ".\common\timer.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_servodsp.h"
#if (STREAM_STM == 1)
#include ".\AL\LAL_stream.h"
#endif

typedef enum   {eTimerTimeout = 0,
                eTimerStopped = TIMER_STOP}    eTimerResult;
#if (PCB == 815)
#define FJ_TRACKING_ON                1
#else
#define FJ_TRACKING_ON                0
#endif

#define LEAD_TRACK  0
#define LEAD_IN     1
#define LEAD_OUT    2

//define for Defect_Enable()
#define DEFECT_STATE_SPECIAL_MASK   0xC0
#define DEFECT_SOURCE_SPECIAL_MASK  0x30
#define DEFECT_STATE_MASK           0x0C
#define DEFECT_SOURCE_MASK          0x03

#define DEFECT_SOURCE_FREEZE_BS     0x10
#define DEFECT_SOURCE_FREEZE_RF     0x20
#define DEFECT_SOURCE_UNFREEZE      0x30
#define DEFECT_STATE_FREEZE_ALL_OFF 0x40
#define DEFECT_STATE_SP_RESERVED1   0x80
#define DEFECT_STATE_UNFREEZE       0xC0

#define DEFECT_STATE_NO_CHANGE      0x00
#define DEFECT_STATE_ALL_OFF        0x04
#define DEFECT_STATE_ONLY_FOCUS_ON  0x08
#define DEFECT_STATE_ALL_ON         0x0C

#define DEFECT_SOURCE_NO_CHANGE     0x00
#define DEFECT_SOURCE_DFE_RF        0x01
#define DEFECT_SOURCE_SERVO_BS      0x02
#define DEFECT_SOURCE_RESERVED1     0x03

#if (FJ_TRACKING_ON == 1)
//speed profile gain up ration 1.35 Q8 = 0x159
#define SpdGainUpRatio    0x159
//The TESRCDB devicide the TX de-glitch period and inhibit window step
// Innibit window step 2.84 us = 33.8688 MHz/96(TESRCDB),
// TX de-glitch period = (33.8688MHz/96)/4 = 88KHz if HAL_SRV_BFILT == 1
// TX de-glitch period = (33.8688MHz/96)/8 = 44KHz if HAL_SRV_BFILT == 0
#define FJTESRCDB              48
#define InhibitWin            200 //unit us
#define InhibitTX             (InhibitWin*34)/FJTESRCDB  //InhibitTX = (InhibitWin*33.8688mHz)/TESRCDB
#define TXSpeedSlowThreshold   59  //352800/6000=58.8
#define FJTrackNumber          40

    #if (WORKAROUND_FOR_BUG_5637 == 1)
#define TRKOShift         127
    #endif
#endif
#if (ENABLE_HDDVD == 1)
#define HD_SYSTEM_LEAD_TRACK  0
#define HD_SYSTEM_LEAD_IN     1
#define HD_SYSTEM_LEAD_OUT    2
#define PRESET_HD_SYSTEM_LEAD_IN     3 //preset for system lead in target
#define PRESET_HD_SYSTEM_LEAD_OUT    4 //preset for system lead in target

#define HD_DEMOD_ETM                 0
#define HD_DEMOD_FSM                 1
#endif

#define CE_WINDOW_MV  (CE_WINDOW_UM * CE_SEN_TARGET_MV_MM / 1000)
#define TRACKING_ON_CE_WINDOW_MV  (CE_WINDOW_MV*9)/10
#define ALL_MUTE_ON         0
#define LODER_MUTE_OFF      1
#define SHORT_BRAKE         2
#define REVERSE_BRAKE       3
#define RESET_PROTECT       4

#define DSPADC_IN_RANGE     (GPADC_IN_RANGE)
#define DSPADC_IN_MIN       (GPADC_IN_MIN + ((1 * GPADC_IN_RANGE) >> GPADC_BITS))
#define DSPADC_BITS         8

#if (PCB == 691)
#define DXX811_HFM_OFF                  0
#define DXX811_HFM_MODE1                1
#define DXX811_HFM_MODE2                2
#endif

#define FokStatusOK() (READ_FIELD(HAL_SRV_FOK) == TRUE)

//#define TON_ON_OFF(x)  WRITE_FIELD(HAL_SRV_REGTON, x)

#define CONVERT_ADCMV_TO_DSPOFS(x)  (((((x) - GPADC_IN_MIN) << GPADC_BITS) / GPADC_IN_RANGE + 7) & 0x3FC)
#define CONVERT_DSPOFS_TO_ADCMV(x)  (((((x) - 5) * GPADC_IN_RANGE) >> GPADC_BITS) + GPADC_IN_MIN)

//-----------------------------------------------/
// define enum                                   /
//-----------------------------------------------/
//define enum w.r.t. CIDI(Current ID Ready Interrupt Flag)              // Define a related enum in REG_dvd_decoder.h?
enum
{
    e_NOT_READY_CIDI,
    e_READY_CIDI
};

/***** Prototypes *****/
/** svo_lib.c **/
void svoInitVar(void);
void svoServoInit(void);
void InitSvoRegsForSpeed(discKindT DiscKind, ESpindleSpeed speed);
void setServoRegister(ULONG addr, USHORT data, eSvoRegInitMode mode);
USHORT svoRegValueFromTbl(ESpindleSpeed speed, ULONG addr);
void svoSetServoOff(void);
void svoSetLDOff(void);
void SetEqRegsForASpeed(discKindT DiscKind, ESpindleSpeed speed);
void SetSvoEqBank(const WORD *eq_addr, const svoWordU *data, USHORT gain_ratio);
USHORT svo_Eq_Tbl_Data(ESpindleSpeed speed,WORD eq_addr);
void svo_Block_Eq_Data_Set(ESpindleSpeed speed,BYTE eq_type);
void svo_Adj_Data_Clr(void);
void svo_preset_media_par(void);
void svoEnTimeRead(void);
void svoEnAtipRead(void);
void svoEnSubqRead(void);
SHORT checkTimeData(void);
SHORT CheckHeaderID(void);
void Write_Gate_Control(BYTE status);
void EFM_SMP_Control(BYTE EFMstate,BYTE SMPState);
void svo_Offset_Test_Write(BYTE On_Off);
eTimerResult timeout_chk(BYTE no);
__inline void SetTimeout(BYTE no, USHORT data);
void SetDelayTimer(USHORT data);
SHORT delay_timer_check(void);
void StartFGTimer(void);
void StopFGTimer(void);
BYTE GetSubQCont(void);
void svotrkon(void);
void SledStop(void);
void discstop(void);
void trayoff(void);
void alltimer_stop(void);
#if DO_HW_SUPPORT_STEP == 1
signed_int_read_t read_stploc(void);
#endif
void velo_stop(void);
void fgdsp_prepare(void);
void tgdsp_prepare(void);
void func_nop(void);
void nodisc_set(void);
BYTE get_zone_speed(void);
void SetMotorDriverState(BYTE state);
BYTE get_driver_state(void);
BOOL cancel_flag_check(void);
extern BYTE tbl_error_log_idx;
extern BYTE tbl_error_log[SIZE_ERR_LOG];
void err_log(eLogErrorCode_t);
void init_error_log_buffer(void);
BOOL focus_shock_detection(eFocusShockInterrupt_t,
                           eFocusShockAverage4Smp_t,
                           eFocusShockWindow_t,
                           eFocusShockWidth_t,
                           BYTE,
                           BYTE);
void      track_off(void);
void      ClearAddressRead(void);
void      StartDiscStopWithTrayTimer(void);
SHORT     CheckDiscStopWithTrayTimer(USHORT timer);

void      track_slip_protection(BYTE slow_detection);
void      TXSpdMonitorInit(void);
BYTE      TXSpdMonitor(void);
SBYTE     calDacCount2SBYTE(BYTE dac_count,BYTE total_Bits);
BYTE      calSBYTE2DacCount(SHORT sbyte,BYTE total_Bits);
ad_dsp_t  ReadDSPADCmV(eRevT Revolutions, BYTE selected_signal, BOOL need_fok);
ad_dsp_t  ReadFilteredDSPADCmV(eRevT Revolutions, BYTE selected_signal, BYTE selected_signal2, BOOL need_fok);
ad_dsp_t  ReadGPADCmV(eRevT Revolutions,BYTE selected_signal,BOOL need_fok);
SBYTE     GetOpuTemperature(void);
void SetSCBPFforCAVZone (ULONG PSN_ID);
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
void      Set_TOK_Mode(BYTE TOK_mode);
#endif

#if (DEFECT_LENGTH_MEASURE == 1)
void      libEnableDFCTInt(void);
void      libDisableDFCTInt(void);
#endif

    #if (EN_ADEQ == 1)
void     InitAdaptEq(void);
    #endif
#if(FORCE_DISK_KIND==1)
void disc_kind_preset(USHORT eDisc);
#endif

/****************************************************************
    Externed Varibles
*****************************************************************/

/****************************************************************
    Externed Functions
*****************************************************************/
/*-----------------------------------------------------------------------------
 * Module: LPFIIR()
 * Description: LPF = (B1+B2*z^-1)/(1-A1*z^-1)
 *----------------------------------------------------------------------------*/
extern __inline SHORT LPFIIR(SHORT LpfIIRIn)
{
    svoLpfVar.OutPrv = ((svoLpfVar.OutPrv * svoLpfVar.coefA1 + (LpfIIRIn + svoLpfVar.InPrv) * svoLpfVar.coefB1B2) >> 16);
    svoLpfVar.InPrv  = LpfIIRIn;

    return (svoLpfVar.OutPrv);
}
extern void     InitLPFIIR(SHORT LpfIIRInit, USHORT samples);
extern void     svoInitVar(void);
extern void     svoServoInit(void);
extern void     Defect_Enable(BYTE status);
extern void     setServoRegister(ULONG addr, USHORT data, eSvoRegInitMode mode);
extern USHORT   svoRegValueFromTbl(ESpindleSpeed speed,ULONG addr);
extern void     svoSetServoOff(void);
extern void     svoFocusOff(void);
extern ETrackingMethod getTrackingMethod(discKindT discKind);
#if (COLLI_SWITCH == 1)
extern USHORT   getInitColliPos(BYTE layer, eDiscStd media);
#endif

#if  SERVOKEYPARAMETER == 1
extern void SetServoKeyParameter(void);
extern void SetServoKeyParameterSL(void);
extern void SetServoKeyParameterDL(void);
#endif

extern void     SetEqRegsForASpeed(discKindT DiscKind, ESpindleSpeed speed);
extern void     SetSvoEqBank(const WORD *eq_addr, const svoWordU *data, USHORT gain_ratio);
extern USHORT   svo_Eq_Tbl_Data(ESpindleSpeed speed,WORD eq_addr);
extern void     svo_Block_Eq_Data_Set(ESpindleSpeed speed,BYTE eq_type);
extern void     svo_Adj_Data_Clr(void);
extern void     svo_preset_media_par(void);
extern USHORT   svo_get_radius_from_pll();
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
extern USHORT  svo_get_parFine_from_pll(USHORT radius);
#endif
extern void     svoSetLDOff(void);
extern SHORT    checkTimeData(void);
extern SHORT    CheckHeaderID(void);
extern void     SetDfeWblFrontOnHold(void);
extern void     SetDfeWblFrontOnHoldTracking(void);
extern void     SetDfeWblFrontSemiSynchr(void);
extern void     SetDfeWblFrontSemiSynchrAfterTracking(void);
extern void     SetDfeWblFrontBitSynchr(void);
extern void     svoStartUpWrongLayer(void);
extern void     svo_Offset_Test_Write(BYTE On_Off);
extern void     Write_Gate_Control(BYTE status);
extern BYTE     GetSpindleControlBlock(void);
extern eTimerResult timeout_chk(BYTE no);
extern __inline void     SetTimeout(BYTE no, USHORT data);
extern void     SetDelayTimer(USHORT data);
extern SHORT    delay_timer_check(void);
extern void     StartFGTimer(void);
extern void     StopFGTimer(void);
extern BYTE     GetSubQCont(void);
extern void     SledStop(void);
extern void     discstop(void);
extern void     trayoff(void);
extern BYTE     track_jump(eJumpType type, BYTE direction, USHORT distance);
extern void     svoEnAtipRead(void);
extern void     svoEnSubqRead(void);
extern void     InitSvoRegsForSpeed(discKindT DiscKind, ESpindleSpeed speed);
extern void     fgdsp_prepare(void);
extern void     tgdsp_prepare(void);
extern void     func_nop(void);
extern void     svotrkon(void);
extern void     alltimer_stop(void);
extern void     velo_stop(void);
extern void     nodisc_set(void);
extern BYTE     get_zone_speed(void);
extern void     SetMotorDriverState(BYTE state);
extern BYTE     get_driver_state(void);
extern SHORT    SvoDiameterJudge(BYTE mode);

extern BOOL     cancel_flag_check(void);
#if (DVD_RAM_READ == 1)
extern void set_DVDRAM_mode();
extern void set_DVDRAM_rewritable_mode();
extern void set_DVDRAM_embossed_mode();
extern long trk32_calc_DVDRAM(LONG TargetPSN,LONG DeltaPSN);
#endif
extern void svoEncpllDividerControl(void);

extern void UART_isr(void);

extern SBYTE          calDacCount2SBYTE(BYTE dac_count,BYTE total_Bits);
extern BYTE           calSBYTE2DacCount(SHORT sbyte,BYTE total_Bits);
extern ad_dsp_t       ReadDSPADCmV(eRevT Revolutions, BYTE selected_signal, BOOL need_fok);
extern ad_dsp_t       ReadGPADCmV(eRevT Revolutions,BYTE selected_signal,BOOL need_fok);
extern ad_dsp_t       ReadFilteredDSPADCmV(eRevT Revolutions, BYTE selected_signal, BYTE selected_signal2, BOOL need_fok);
#if (OPU_BEAM_LANDING_MEAS == 1)
extern ad_dsp_t       readSegmentABCD(eRevT revolutions, BOOL need_fok);
#endif
#if (BD_USE_TE_PP_DPD == 1)
extern ad_dsp_t       ReadMPPDPDPhase(eRevT Revolutions);
#endif
extern const BYTE   DSP_ADC_ADDR[4][2];
extern SBYTE GetOpuTemperature(void);
extern BYTE focus_on_chk(USHORT *TEp2p);
extern void RampMeas(eRampMeasType type, USHORT duration, BYTE direction, discKindT DiscKind);
void     focus_wait_stable(USHORT stable_time);
#if (STREAM_STM == 1)
extern void RampStream(eRampMeasType type, USHORT duration, BYTE direction, discKindT DiscKind);
extern ULONG StreamInitiate(halOPC_AOPCSampNum_t eNrOfSamples, ULONG Frequency, halOPC_AOPCAvgSamp_t eAvgSamples, LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Channel1, LAL_STREAM_AnalogFESignalT Channel2, LAL_STREAM_AnalogFESignalT Channel3);
extern void GetStreamStatus(eRampMeasType type, USHORT NrOfSamples, USHORT frequency, BYTE direction);
#endif
extern SHORT ramp_stop(void);
extern void ramp_FOOS(SHORT valFOOS);    // prevent scratch disc during focus recovery
#if (CHIP_REV >= 0xC0)
void     ramp_to(SHORT valFOO);
#endif
extern void apply_FOOS(SHORT valFOOS);
extern void apply_TILT(SHORT valTILT);
extern void apply_FEBC(SHORT valFEBC);
extern void apply_TESPT(SHORT valTESPT);
extern void apply_FESPT(SHORT valFESPT);
extern void CurveFit(BYTE length, float *x, float *y, float *cof);
extern void LineFit(BYTE length, const float *x, const float *y, float *cof);
#if (OPEN_SESSION == 1)
extern BOOL RFpp_chk(void);
#endif
extern float linear_interpolation( float point_x1 , float point_y1 , float point_x2 , float point_y2 , float point_x );
extern BOOL curve_fit_data_filter( float *cfx, float *cfy, BYTE *cfsize_in, float limit_y, BOOL up_curve);
extern RAMP_MODE_t calib_data_analyser( float *cfx, float *cfy, BYTE cfsize_in, BYTE min_sample, float min_x, float max_x, USHORT min_delta, BOOL up_curve);
extern void enableRTJitterMeas(void);
extern void disableRTJitterMeas(void);
extern USHORT convertRTJitter(ULONG reg_value_x8);
extern void SetSCBPFforCAVZone (ULONG PSN_ID);
extern void wait_track_on(USHORT timeout);
#if (FJ_TRACKING_ON == 1)
extern void fine_jump_tracking_on(BYTE Direction);
#endif
extern void track_slip_protection(BYTE slow_detection);
extern void TXSpdMonitorInit(void);
extern BYTE TXSpdMonitor(void);
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
extern void Set_TOK_Mode(BYTE TOK_mode);
#endif

#if (DEFECT_LENGTH_MEASURE == 1)
extern void      libEnableDFCTInt(void);
extern void      libDisableDFCTInt(void);
#endif

#if(FORCE_DISK_KIND==1)
extern void disc_kind_preset(USHORT eDisc);
#endif

#if(DISC_MIS_JUDGE_RECOVERY)
extern BYTE svoReStartUp(BYTE media_type);
#endif
#if(PCB == 1600)
extern void UpdateColliPos(void);
#endif

#if (DVD_RAM_READ == 1)
void     set_DVDRAM_mode();
void     reset_DVDRAM_mode();
void     set_DVDRAM_rewritable_mode();
void     set_DVDRAM_embossed_mode();
void     set_DVDRAM_decoder_registers();
long     trk32_calc_DVDRAM(LONG TargetPSN, LONG DeltaPSN);
void     DVDRAM_header_RF_gain_calib();
BOOL     DVDRAM_header_detect();
BOOL     DVDRAM_is_embossed_area();
BOOL     DVDRAM_is_rewritable_area();
SHORT    get_FEBC();
void     showTiltandFEBCVar();
void     SvoDMACalibrationReset();
void     DMACal_ResetSeekTarget();
void     DMACal_ResetChannel(BYTE curChannel,BYTE curTrack);
void     show_pid_error_rate();
void     clear_pid_error_rate();
void     chkHdrOfsHoldedDuringDefect();
void     apply_RFAOCREG(SBYTE target);
void     apply_RFAGCREG(BYTE target);
SHORT    SortDMAPIData(short offset,short num);
void     apply_WRFCSO(void);
void     apply_FCSO(SHORT target,BYTE tracktype);
void     DVDRAM_synchronize_LG_transition(BOOL land);
#endif // #if (DVD_RAM_READ == 1)

#endif /* _SVO_LIB_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
