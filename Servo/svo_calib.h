/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_CALIB.H
*
* DESCRIPTION
*
*   $Revision: 144 $
*   $Date: 11/03/04 2:34p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_CALIB_
#define _SVO_CALIB_

#define PATH_SELECT_ALL  0xFF
#define COLLI_CALI_FOR_TEMPERATURE_RISE    1
#define COLLI_CALI_TEMP_THRESHOLD          8

/* signal selection for misc calibration */
#define FEGN_DSP            0 //layer dependent
#define TEGN_DSP            1 //layer dependent
#define FE_SPT              2 // layer dependent
#define TE_SPT              3 // layer dependent
#define TRO_OS              4
#define FOO_OS              5
#define PDIC_GN             6
#define LASER_PW            7

/* signal selection for servo calibration */
#define TE_RATIO            0 //radius & layer dependent
#define TILT_DAT            1 //radius dependent
#define FEBC_DSP            2 //layer dependent
#define FEOS_AFE            3
#define TE_SETPT            4 //TE set-point
#define COLLI_POS           5
#if (DPD_COMPENSATE_OFFSET == 1)
#define DPD_GNOS            6//add DPD gain compensation
#endif
#define ITEM_ALL            0xFF

#if (DVD_RAM_READ == 1)
#define GROOVE_TRACK        0
#define LAND_TRACK          1
#define ALL_TRACK           2
#endif

#define JITTER_CAL          0
#define TE_P2P_CAL          1
#define RF_P2P_CAL          2
#define TEST_WRITE_MODE     0xFF
#define WRITE_MODE          0xFE
#define READ_MODE           0xFD

#define CAL_RADIUS_ZONE_NR  20
#define CAL_R_DONE          0x01
//#define CAL_W_DONE          0x02
#define CAL_REQUIRED        0x04
#define CAL_SKIP            0x08
#define CAL_UNRELIABLE      0x80

//for calCalibrateOffset()
#define CALIB_FROM_0         0
#define CALIB_FROM_CURRENT   1

//for state_AFE_InputGainCal()
#define CALIB_ALL            0
#define CALIB_SERVO          1

//for calCalibrateTEBGTROOS()
#define CALIB_TEBG           0x01
#define CALIB_TROOS          0x02
#define CALIB_CEGN           0x04

//define phase number
#define GET_FIRST_SAMPLE            0
#define GET_SECOND_SAMPLE           1
#define FINE_ADJUST                 2
#define TARGET_FOUND                3

typedef enum
{
    /* Integral Revolutions */
    NO_REVOLUTION,
    ONE_REVOLUTION,
    TWO_REVOLUTIONS,
    THREE_REVOLUTIONS,
    FOUR_REVOLUTIONS,

    /* Fraction Revolutions */
    EIGHTH_REVOLUTION = 0xFB,
    QUARTER_REVOLUTION,
    HALF_REVOLUTION
} eRevT;

/****************************************************************
                         Prototypes
*****************************************************************/
schar_return_t adj_next_offset(SBYTE   cur_offset_cnt,          // reg offset in count
                               SBYTE   max_min_cnt,             // reg offset in count limit
                               SHORT   meas_level,              // unit: mV
                               SHORT   target_level,            // unit: mV
                               float   mV_per_cnt,              // unit: mV/count
                               LONG    cur_gain,                // unit: 2^-16
                               float   adj_factor,              // unit: NA
                               BOOL    pol);                    // TURE: same direction, FALSE: opposit direction
uchar_return_t adj_next_gain(BYTE   cur_gain_cnt,               // reg gain in count
                             BYTE   max_gain_cnt,               // reg gain in count
                             USHORT meas_p2p,                   // unit: mV
                             USHORT target_p2p,                 // unit: mV
                             float  dB_per_cnt,                 // unit: dB/count
                             float  adj_factor);                // unit:
uchar_return_t calAdjNextFineGain(BYTE    cur_gain_cnt,         // reg offset in count
                                  SBYTE   max_cnt,              // reg offset in count max limit
                                  SBYTE   min_cnt,              // reg offset in count min limit
                                  SHORT   cur_p2p,              // unit: mV
                                  SHORT   target_p2p);          // unit: mV
void calStoreAfeVar(void);
void calSetAfeOffset(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,BYTE new_value);
void calSaveAllAfeOffset(BYTE adjust_signal,BYTE type,BYTE offset_cnt);
void calSetAfeGain(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,SBYTE new_value);
void calSetMisc(BYTE layer, BYTE adjust_signal, LONG new_value);
#if (CALIBRATION_MANAGEMENT == 1)
SHORT   check_signal_Limit(BYTE signal, SHORT inputData);
#endif
void calSetCalResult(BYTE layer, USHORT sled_pos, BYTE adjust_signal, LONG new_value, BYTE status);
BYTE calGetCalStatus(BYTE layer, USHORT sled_pos, USHORT range, BYTE adjust_signal);
BOOL calGetCalRequired(BYTE layer, USHORT sled_pos, BYTE adjust_signal, BYTE mode);
void calPresetLayer1(void);
void calUpdateLayer(BYTE tolayer);
void calUpdateRadius(USHORT stp_position, BOOL force_update);
errcode_t calCalibrateTEBGTROOS(USHORT target_mv, USHORT *ce_sen_mv_mm, BYTE cal_en);
errcode_t calCalibrateOffset(BYTE    adjust_signal,             // adjusted signal
                             BYTE    type,                      // offset type
                             BYTE    path_selected,             // path switched by WLDON/RECD/NORMAL
                             SHORT   target_mv,                 // unit: mV
                             BOOL    polarity,                  // polarity between offset & signal
                             BYTE    starting_offset);          //'0' : offset cali. from 0 DAC ; '1' : offset cali. from current offset DAC
errcode_t calCalibrateTEGnOs(USHORT target_p2p);
SHORT    feg_stop(void);
SHORT    teg_stop(void);
USHORT   Read_BHO(BYTE num);
USHORT   Read_PHO(BYTE num);
BOOL     Plus_Minus_det(void);
void     CalWrTrkOffset_LdOff(void);

/****************************************************************
    Externed Variables
*****************************************************************/


/****************************************************************
    Externed Functions
*****************************************************************/
extern schar_return_t adj_next_offset(SBYTE   cur_offset_cnt,   // reg offset in count
                                      SBYTE   max_min_cnt,      // reg offset in count limit
                                      SHORT   meas_level,       // unit: mV
                                      SHORT   target_level,     // unit: mV
                                      float   mV_per_cnt,       // unit: mV/count
                                      LONG    cur_gain,         // unit: 2^-16
                                      float   adj_factor,       // unit: NA
                                      BOOL    pol);             // TURE: same direction, FALSE: opposit direction

extern uchar_return_t adj_next_gain(BYTE   cur_gain_cnt,        // reg gain in count
                                    BYTE   max_gain_cnt,        // reg gain in count
                                    USHORT meas_p2p,            // unit: mV
                                    USHORT target_p2p,          // unit: mV
                                    float  dB_per_cnt,          // unit: dB/count
                                    float  adj_factor);         // unit:

extern uchar_return_t calAdjNextFineGain(BYTE    cur_gain_cnt,  // reg offset in count
                                         SBYTE   max_cnt,       // reg offset in count max limit
                                         SBYTE   min_cnt,       // reg offset in count min limit
                                         SHORT   cur_p2p,       // unit: mV
                                         SHORT   target_p2p);   // unit: mV

extern schar_return_t calAdjRRFGain(USHORT    meas_p2p,     // unit: mV
                                    USHORT    target_p2p,   // unit: mV
                                    BYTE      rrf_gain);

#if (NEW_RFPOS_CALIB == 1)
extern schar_return_t calAdjRRFOffset(SHORT cur_middle, USHORT meas_p2p, USHORT target_p2p, BYTE rrf_gain);
#else
extern schar_return_t calAdjRRFOffset(SHORT cur_middle, BYTE rrf_gain);
#endif

extern BYTE calRadius2Zone(USHORT sled_pos);
extern BYTE calBestGain(USHORT *step_size, BYTE gain_orig, BYTE max_gain, USHORT current, USHORT target);
extern void calStoreAfeVar(void);
extern void calSetAfeOffset(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,BYTE new_value);
extern void calSaveAllAfeOffset(BYTE adjust_signal,BYTE type,BYTE offset_cnt);
extern void calSetAfeGain(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,SBYTE new_value);
#if (MPPBG_SPPBG_CALIB != 0)
extern BYTE calSetBalGain(BYTE adjust_signal, SBYTE new_value);
extern BYTE  calSetBalGain(BYTE adjust_signal, SBYTE new_value);
#endif
extern void calSetMisc(BYTE layer, BYTE adjust_signal, LONG new_value);
extern void calSetRfcosShift(SBYTE new_value);
#if (CALIBRATION_MANAGEMENT == 1)
extern SHORT   check_signal_Limit(BYTE signal, SHORT inputData);
#endif
extern void calClrCalResult(BYTE adjust_signal, BYTE layer_1_only);
extern void calSetCalResult(BYTE layer, USHORT sled_pos, BYTE adjust_signal, LONG new_value, BYTE status);
extern BYTE calGetCalStatus(BYTE layer, USHORT sled_pos, USHORT range, BYTE adjust_signal);
extern BOOL calGetCalRequired(BYTE layer, USHORT sled_pos, BYTE adjust_signal, BYTE mode);
extern SHORT calGetOptColliPos(BYTE layer);
extern void calPresetLayer1(void);
extern void calUpdateLayer(BYTE tolayer);
extern void calUpdateRadius(USHORT stp_position, BOOL force_update);
extern errcode_t calCalibrateTEBGTROOS(USHORT target_mv, USHORT *ce_sen_mv_mm, BYTE cal_en);
#if (MPPBG_SPPBG_CALIB == 2)
extern errcode_t calCalibrateMPPSPP1BG(USHORT target_mv);
#endif
extern errcode_t calCalibrateOffset(BYTE    adjust_signal,      // adjusted signal
                                    BYTE    type,               // offset type
                                    BYTE    path_selected,      // path switched by WLDON/RECD/NORMAL
                                    SHORT   target_mv,          // unit: mV
                                    BOOL    polarity,           // polarity between offset & signal
                                    BYTE    starting_offset);   //'0' : offset cali. from 0 DAC ; '1' : offset cali. from current offset DAC
extern errcode_t calCalibrateRPBG(USHORT target_mid);
extern errcode_t calCalibrateRPOS(void);
extern errcode_t calCalibrateRPGnOs(USHORT target_p2p);
errcode_t calCalibrateStepsize(BYTE adjust_offset, BYTE adjust_gain, BYTE type, BYTE max_gain, USHORT *stepsize);
extern errcode_t calCalibrateTEGnOs(USHORT target_p2p);
extern SHORT    feg_stop(void);
extern SHORT    teg_stop(void);

extern errcode_t calCalibrateCEBG(void);
extern errcode_t calCalibrateCEOS(void);
extern SHORT     calGetGainVref(ULONG addr,BYTE mask,BYTE start_bit,BYTE monitor_signal);
#if (DPD_COMPENSATE_OFFSET == 1)
extern void   calSetDPDTEOS(BYTE layer);
extern USHORT calGetDPDCalRadius(USHORT start_radius, USHORT end_radius);
#endif//#if (DPD_COMPENSATE_OFFSET==1)

extern ad_dsp_t       ReadDSPADCmV(eRevT Revolutions, BYTE selected_signal, BOOL need_fok);
extern ad_dsp_t       ReadFilteredDSPADCmV(eRevT Revolutions, BYTE selected_signal, BYTE selected_signal2, BOOL need_fok);
extern ad_dsp_t       ReadYerrRevs(eRevT Revolutions);
extern ad_dsp_t       ReadGPADCmV_Avg(eRevT Revolutions, BYTE channel, BOOL need_fok);
extern ad_dsp_t       ReadDSP_CE_mV_Avg(eRevT Revolutions, BOOL need_fok);
extern ad_dsp_t       ReadGPADCmV(eRevT Revolutions,BYTE selected_signal,BOOL need_fok);

typedef enum
{
    RF_ADC,
    WBL_ADC
} ADCSignalT;
extern ad_dsp_t       readADCmVPerRevs(eRevT Revolutions, BOOL InFocus, ADCSignalT sig);
ad_dsp_t  readPNmV(BYTE ch1, BYTE ch2, eRevT Revolutions, BOOL need_fok);
extern ad_dsp_t       readRFppFromAGC_1rev(BOOL need_fok);
extern ad_dsp_t       ReadAvgJitterPerRevs(eRevT revolutions);
extern void           init_SRC(void);

extern BOOL     Plus_Minus_det(void);
extern void     CalWrTrkOffset_LdOff(void);
#endif /* _SVO_CALIB_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
