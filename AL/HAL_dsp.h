/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_dsp.h
*
* DESCRIPTION
*   This file supports the HAL DSP funtions implemented in the DSP HAL layer
*
* NOTES:
*
* $Revision: 49 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_DSP_H_
#define _HAL_DSP_H_

#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\AL\REG_servodsp.h"
#include ".\servo\svo_dsp.h"
#include ".\AL\REG_servo.h"
//*** Global definitions regarding the DSP functionality

//define DSP operation mode
#define DSP_NORMAL_MODE                     1
#define DSP_BIST_MODE                       2

//define DSP error state
#define DSP_INITIAL_ERROR                   1
#define DSP_RESET_ERROR                     2
#define DSP_COEFRAM_BLANK_ERROR             3
#define DSP_NORMAL_COEFRAM_LOAD_ERROR       4
#define DSP_BIST_COEFRAM_LOAD_ERROR         5
#define DSP_INSTRRAM_BLANK_ERROR            6
#define DSP_INSTRRAM_LOAD_ERROR             7
#define DSP_BIST_TIMEOUT_ERROR              8
#define DSP_BIST_OUTPUT_ERROR               9

//define DSP power mode
#define NORMAL_MODE                         0
#define LOW_POWER1_MODE                     1
#define FULL_SPEED_MODE                     2
#define LOW_POWER2_MODE                     3

//define SRC memory depth
#define SRC_MEMORY_DEPTH                    36

//define SRC Tacho mode
#define SRC_TACHO_NORMAL                    0
#define SRC_TACHO_LABELFLASH                1
#define SRC_TACHO_LIGHTSCRIBE               2

//define SRC sync mode
#define SRC_SYNC_LEGACY                     0
#define SRC_SYNC_NEW_1X                     1
#define SRC_SYNC_NEW_3X                     2

//define disc motor configuration

//define SRC loop mode
#define SRC_LOOP_FOCUS                      0
#define SRC_LOOP_TRACKING                   1
#define SRC_LOOP_TE_OFFSET                  2
#define SRC_LOOP_ALL                        3

//define SRC strength
#define SRC_STRENGTH_HIGH                   1
#define SRC_STRENGTH_NORMAL                 2
#define SRC_STRENGTH_LOW                    4

//define SRC balance factor (nominal value = 100)
#define SRC_BALANCE_FACTOR                  100

//define SRC state
#define SRC_STATE_OFF                       0
#define SRC_STATE_ON                        1
#define SRC_STATE_RESET                     2

//define Debug channel
#define DEBUG_CH0                           0
#define DEBUG_CH1                           1
#define DEBUG_CH2                           2
#define DEBUG_CH3                           3

//define Debug source
#define DEBUG_SOURCE_RFILE                  0
#define DEBUG_SOURCE_RGLUE                  1


//should be defined in DSP.h
#define DSP_FREQ_PHASEA                     352800
#define DSP_FREQ_PHASEB                     176400
#define DSP_FREQ_PHASEC                     88200
#define DSP_FREQ_PHASED                     44100

#define SRCX_CONSTANT                       (ULONG)((512L * 32768L * SRC_MEMORY_DEPTH) / DSP_FREQ_PHASED)

#if (EN_ADI == 1)
//should be defined in DAC.h
#define PDMDAC_MAXVOLTAGE                   3300.0 //in mV (keep float for correct calculation)
#define PDMDAC_BITS                         10
#define PDMDAC_STEPVOLTAGE                  (float)(PDMDAC_MAXVOLTAGE/(PowerTo(2, PDMDAC_BITS)))

//define actuator dissipation measurement
#define ACTDISS_SUM_INT                     0x20
#define ACTDISS_TILT_INT                    0x10
#define ACTDISS_FOC_INT                     0x08
#define ACTDISS_TRK_INT                     0x04
#define DSPIEN_SEL                          ACTDISS_FOC_INT|ACTDISS_TRK_INT

//define automatic switch-off function
#define TLOFFSUM                            (0x02 << 28)
#define TLOFFTILT                           (0x01 << 28)
#define TOFFSUM                             (0x08 << 14)
#define TOFFTILT                            (0x04 << 14)
#define TOFFFOC                             (0x02 << 14)
#define TOFFTRK                             (0x01 << 14)
#define FOFFSUM                             0x08
#define FOFFTILT                            0x04
#define FOFFFOC                             0x02
#define FOFFTRK                             0x01
#define SWOFFEN_FO_SEL                      TOFFFOC|FOFFFOC
#define SWOFFEN_TO_SEL                      0   //TOFFFOC|FOFFFOC
#define SWOFFEN_TL_SEL                      0   //TOFFFOC|FOFFFOC
#define SWOFFEN_SEL                         0   //TLOFFSUM|TLOFFTILT|TOFFSUM|TOFFTILT|TOFFFOC|TOFFTRK|FOFFSUM|FOFFTILT|FOFFFOC|FOFFTRK
#endif

//*** External function prototypes
extern void HAL_DSP_Init(void);
extern void HAL_DSP_ClockInit(void);
extern void HAL_DSP_CoefRAMInit(BYTE DspMode);
extern void HAL_DSP_Load(BYTE DspMode);
extern BOOL HAL_DSP_BISTPass(void);
extern void HAL_DSP_TachoMultConfig(BYTE TachoMode, BYTE SyncMode, BYTE NrTachoPulses);
extern void HAL_DSP_TachoMultEnable(BOOL State);
extern void HAL_DSP_TachoMultReset(BOOL State);
extern void HAL_DSP_SRCConfig(BYTE SrcLoop, BYTE SrcStrength);
extern void HAL_DSP_SRCEnable(BYTE SrcLoop, BYTE SrcStatus);
extern void HAL_DSP_SRCHold(BYTE SrcLoop, BOOL HWState, BOOL DFCState, BOOL FWState);
extern void HAL_DSP_SetDebug(BYTE DebugChannel, BYTE DebugSource, USHORT Address, USHORT Gain_x_8);
extern void HAL_DSP_SetDebugMode(BOOL State);
extern void HAL_DSP_SetDebugForce(BOOL State);

#if(SUN_FUNC_TEST == 1)
extern void HAL_DSP_LED(BOOL State);
extern void HAL_DSP_INVERT_LED();
#endif

#if (EN_ADI == 1)
//extern void HAL_DSP_ACTDISSConfig(BYTE PowerTable, USHORT Time);
//extern void HAL_DSP_ACTDISSConfig(USHORT Time);
extern void HAL_DSP_ACTDISSConfig(void);
extern void HAL_DSP_ACTDISSReset(void);
#endif
extern void HAL_DSP_ClearSRCMemory(BYTE SrcLoop);
extern void HAL_DSP_EnableOutputs(void);
extern void HAL_DSP_FocusLockAid(BOOL enable, BOOL updown);
extern void HAL_DSP_LayerJumpSurf(BOOL enable);

enum
{
   eFCSLockAidThr,
   eFCSLockAidGn,
   eFCSLockAidLast = eFCSLockAidGn
};

/*-----------------------------------------------------------------------------
 *
 * Module: HAL_DSP_SetKspdStart()
 * Description: This is an extra action required for servo DSP in order to load
 * correct kspd gain and velocity profile for kick pulse & initial velocity loop
 * This function need to be executed at least 15usec before fine search cmd
 *----------------------------------------------------------------------------*/
extern __inline void HAL_DSP_SetKspdStart(USHORT TracksToGo)
{
    USHORT kspd_val, kspd_gain, kspd_spd;
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
    USHORT kspdlim_val;
    #endif

    if (TracksToGo < 4)
    {
        kspd_val = READ_DSP_COEF(kspd_0);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_0);
    #endif
    }
    else if (TracksToGo < 8)
    {
        kspd_val = READ_DSP_COEF(kspd_4);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_4);
    #endif
    }
    else if (TracksToGo < 16)
    {
        kspd_val = READ_DSP_COEF(kspd_8);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_8);
    #endif
    }
    else if (TracksToGo < 32)
    {
        kspd_val = READ_DSP_COEF(kspd_16);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_16);
    #endif
    }
    else if (TracksToGo < 64)
    {
        kspd_val = READ_DSP_COEF(kspd_32);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_32);
    #endif
    }
    else if (TracksToGo < 128)
    {
        kspd_val = READ_DSP_COEF(kspd_64);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_64);
    #endif
    }
    else if (TracksToGo < 256)
    {
        kspd_val = READ_DSP_COEF(kspd_128);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_128);
    #endif
    }
    else if (TracksToGo < 512)
    {
        kspd_val = READ_DSP_COEF(kspd_256);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_256);
    #endif
    }
    else if (TracksToGo < 1024)
    {
        kspd_val = READ_DSP_COEF(kspd_512);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_512);
    #endif
    }
    else
    {
        kspd_val = READ_DSP_COEF(kspd_1024);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspdlim_val = READ_DSP_COEF(kspdlim_1024);
    #endif
    }
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
    WRITE_DSP_COEF(kspdlim_start, kspdlim_val);
    #endif
    if (DVDRAM)
    {
        kspd_gain = kspd_val & 0xFF00;
        kspd_spd = (kspd_val & 0x00FF) * 3; //set speed target to 3x slower
        kspd_val = kspd_gain | ((kspd_spd > 0x7F)? 0x7F : kspd_spd);
        WRITE_DSP_COEF(kspd_start, kspd_val);
    }
    else
    {
        kspd_gain = kspd_val & 0xFF00;
        kspd_spd = (kspd_val & 0x00FF) * 9 / 4 + 6; //set first half track period longer
        kspd_val = kspd_gain | ((kspd_spd > 0x7F)? 0x7F : kspd_spd);
        WRITE_DSP_COEF(kspd_start, kspd_val);
    #if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        kspd_spd = kspd_spd * (0x60 * 2 / 3) / READ_FIELD(HAL_SRV_INHISMP);
    #else
        kspd_spd = kspd_spd * (0x60 * 2 / 3) / READ_FIELD(HAL_SRV_TESRCDB);
    #endif
        WRITE_FIELD(HAL_SRV_INHITX,((kspd_spd > 0x3FF)? 0x3FF : kspd_spd));
    }
}
#if (EN_ADI == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_EnAutoSWOff()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_DSP_EnAutoSWOff(ULONG swoff_sel)
{
    WRITE_REG(HAL_SRVREG_MAP->SWOFFEN, READ_REG(HAL_SRVREG_MAP->SWOFFEN)|swoff_sel);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_DisAutoSWOff()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_DSP_DisAutoSWOff(ULONG swoff_sel)
{
    WRITE_REG(HAL_SRVREG_MAP->SWOFFEN, READ_REG(HAL_SRVREG_MAP->SWOFFEN)&(~swoff_sel));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_EnDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_DSP_EnDissInt(void)
{
    WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
//    WRITE_FIELD(HAL_SRV_DSPI,  ~DSPIEN_SEL);
//    WRITE_FIELD(HAL_SRV_DSPIEN, DSPIEN_SEL);
    //WRITE_FIELD(HAL_SRV_DSPI5,  0);   //SUM
    //WRITE_FIELD(HAL_SRV_DSPI4,  0);   //TILT
    WRITE_FIELD(HAL_SRV_DSPI3,  0);   //FOC
    WRITE_FIELD(HAL_SRV_DSPI2,  0);   //TRK
    //WRITE_FIELD(HAL_SRV_DSPI5EN, 1);  //SUM
    //WRITE_FIELD(HAL_SRV_DSPI4EN, 1);  //TILT
    WRITE_FIELD(HAL_SRV_DSPI3EN, 1);  //FOC
    WRITE_FIELD(HAL_SRV_DSPI2EN, 1);  //TRK
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_DisDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_DSP_DisDissInt(void)
{
//  WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
//    WRITE_FIELD(HAL_SRV_DSPI, ~DSPIEN_SEL);
//    WRITE_FIELD(HAL_SRV_DSPIEN, 0);
    //WRITE_FIELD(HAL_SRV_DSPI5,  0);   //SUM
    //WRITE_FIELD(HAL_SRV_DSPI4,  0);   //TILT
    WRITE_FIELD(HAL_SRV_DSPI3,  0);   //FOC
    WRITE_FIELD(HAL_SRV_DSPI2,  0);   //TRK
    //WRITE_FIELD(HAL_SRV_DSPI5EN, 1);  //SUM
    //WRITE_FIELD(HAL_SRV_DSPI4EN, 1);  //TILT
    WRITE_FIELD(HAL_SRV_DSPI3EN, 0);  //FOC
    WRITE_FIELD(HAL_SRV_DSPI2EN, 0);  //TRK
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_SetActFocDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BYTE HAL_DSP_SetActFocDissInt(void)
{
    if((DSPIEN_SEL&ACTDISS_FOC_INT) && (!READ_FIELD(HAL_SRV_DSPI3EN)))
    {
//        WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
        HAL_DSP_ACTDISSReset();
        WRITE_FIELD(HAL_SRV_DSPI3,     0);
        WRITE_FIELD(HAL_SRV_DSPI3EN,   1);
//      HAL_DSP_EnAutoSWOff(SWOFFEN_FO_SEL);
        return TRUE;
    }
    else
        return FALSE;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_SetActTrkDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BYTE HAL_DSP_SetActTrkDissInt(void)
{
    if((DSPIEN_SEL&ACTDISS_TRK_INT) && (!READ_FIELD(HAL_SRV_DSPI2EN)))
    {
//        WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
        HAL_DSP_ACTDISSReset();
        WRITE_FIELD(HAL_SRV_DSPI2,     0);
        WRITE_FIELD(HAL_SRV_DSPI2EN,   1);
        return TRUE;
    }
    else
        return FALSE;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_SetActTiltDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BYTE HAL_DSP_SetActTiltDissInt(void)
{
    if((DSPIEN_SEL&ACTDISS_TILT_INT) && (!READ_FIELD(HAL_SRV_DSPI4EN)))
    {
//        WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
        HAL_DSP_ACTDISSReset();
        WRITE_FIELD(HAL_SRV_DSPI4,     0);
        WRITE_FIELD(HAL_SRV_DSPI4EN,   1);
        return TRUE;
    }
    else
        return FALSE;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_SetActSumDissInt()
    Description:
    Inputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BYTE HAL_DSP_SetActSumDissInt(void)
{
    if((DSPIEN_SEL&ACTDISS_SUM_INT) && (!READ_FIELD(HAL_SRV_DSPI5EN)))
    {
//        WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
        HAL_DSP_ACTDISSReset();
        WRITE_FIELD(HAL_SRV_DSPI5,     0);
        WRITE_FIELD(HAL_SRV_DSPI5EN,   1);
        return TRUE;
    }
    else
        return FALSE;
}
#endif

#endif /* _HAL_DSP_H_ */

