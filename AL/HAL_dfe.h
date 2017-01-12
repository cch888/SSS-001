/****************************************************************************
*                (c) Copyright 2006 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_dfe.h
*
* DESCRIPTION
*   This file supports the HAL DFE funtions implemented in the DFE HAL layer
*          HAL_DFE_SetIIRHPFBw()
*          HAL_DFE_SetMAvHPFBw()
*          HAL_DFE_EnDisAgcAoc()
*          HAL_DFE_PresetPLLFreq()
*          HAL_DFE_EnDisDataSlicer()
*          HAL_DFE_ReadJitterX10()
*          HAL_DFE_SetAgcAocBoundary()
*          HAL_DFE_IsPllInLock()
*          HAL_DFE_HoldRfPll()
*          HAL_DFE_EnableFlywheels()
*          HAL_DFE_Init()
*
* NOTES:
*
* $Revision: 85 $
* $Date: 11/03/15 2:21p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_DFE_H_
#define _HAL_DFE_H_

#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\al\REG_dfe.h"
#include ".\al\REG_afe.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"

#define TWO_TO_POWER_13 8192 //((2*2*2*2*2*2*2*2*2*2*2*2*2))

//*** Global definitions regarding the DFE HAL functionality
#define GAIN_ZERO_VALUE     16
#define GAIN_BW_RFO_AND_LPF 24 // 0.5dB/dac

//define IIR HP filter Bypassing facility
typedef enum
{
    halDFE_IirHpfNotBypass  = 0,
    halDFE_IirHpfBypass = 1
} halDFE_IirHpfBypass_t;

//define HF Defect Detector enabling facility
typedef enum
{
    halDFE_HfDefectDisable,
    halDFE_HfDefectEnable
} halDFE_HfDefect_t;

//define AGC/AOC hold during defect enabling facility
typedef enum
{
    halDFE_AgcAocHoldDefectDisable,
    halDFE_AgcAocHoldDefectEnable
} halDFE_AgcAocHoldDefect_t;

//define Fast PLL reset facility
typedef enum
{
    halDFE_FastPllDefault,
    halDFE_FastPllReset
} halDFE_FastPllReset_t;

//define Fast PLL freeze facility
typedef enum
{
    halDFE_FastPllUnFreeze,
    halDFE_FastPllFreeze
} halDFE_FastPllFreeze_t;

//define IIR HPF reset facility
typedef enum
{
    halDFE_IIRHPFDefault,
    halDFE_ResetIIRHPFReset
} halDFE_IIRHPFReset_t;

//define black white defect detection enabling facility
typedef enum
{
    halDFE_BwDefectDisable,
    halDFE_BwDefectEnable
} halDFE_BwDefect_t;

//define adaptive Eq enabling facility
typedef enum
{
    halDFE_AdaptEqDisable,
    halDFE_AdaptEqEnable
} halDFE_AdaptEq_t;

//define adaptive Eq enabling while defect facility
typedef enum
{
    halDFE_AdaptEqOnDefectDisable,
    halDFE_AdaptEqOnDefectEnable
} halDFE_AdaptEqOnDefect_t;

//define blank detection enabling facility
typedef enum
{
    halDFE_BlankDetectDisable,
    halDFE_BlankDetectEnable
} halDFE_BlankDetect_t;

//define AGC/AOC enabling facility                  // Define a related enum in REG_dfe.h?
typedef enum
{
    halDFE_AgcAocDisable,
    halDFE_AgcAocEnable
} halDFE_AgcAocEnable_t;

//define DATA SLICER enabling facility              // Define a related enum in REG_dfe.h?
typedef enum
{
    halDFE_DataSlicerDisable,
    halDFE_DataSlicerEnable
} halDFE_DataSlicerEnable_t;

//define RF PLL holding facility
typedef enum
{
    halPLL_RfPllUnhold,
    halPLL_RfPllHold
} halDFE_RfPllHold_t;

//define in lock aid state                          // Define a related enum in REG_dfe.h?
enum
{
    e_NOT_INLOCKAID,
    e_INLOCKAID
};

// define PUSHOOL - Push Out-of-Lock state
typedef enum
{
    halDFE_PUSHOOLDisable,
    halDFE_PUSHOOLEnable
} halDFE_PUSHOOL_t;

// define HFPLLHOD - HF PLL Hold on Defect state
typedef enum
{
    halDFE_HFPLLHODDisable,
    halDFE_HFPLLHODEnable
} halDFE_HFPLLHOD_t;

// define SYNCDIS - Sync Lock Disable on Defect state
typedef enum
{
    halDFE_SYNCDISDisable,
    halDFE_SYNCDISEnable
} halDFE_SYNCDIS_t;

// define force out of lock flywheels facility
typedef enum
{
    e_FORCEFLY_NO_ACTION,       // No action
    e_FORCEFLY_OUT_OF_LOCK      // force the flywheel out of lock
} halDFE_FORCEFLY_t;

//*** External variable

//*** External function prototypes
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetIIRHPFBW

    Description:    Sets first order IIR HF high pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz
                                         BD     84000 Hz
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_IIRHPFBW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
#if 0//currently not be use
#if (CHIP_REV >= 0xC0)
void HAL_DFE_SetIIRBETABw (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10);
void HAL_DFE_SetBETAHIBW (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10);
void HAL_DFE_SetHPFHIBW (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10);
#endif //(CHIP_REV >= 0xC0)
extern void HAL_DFE_SetBetaLPFBw (ULONG HfLowPassFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz);
#endif //currently not be use

extern void HAL_DFE_SetIIRHPFBw (ULONG HfHighPassFcN1KHz, USHORT OverspeedX10, ULONG FDfeInClkKHz);

extern void HAL_DFE_SetBetaLPFBw (ULONG HfLowPassFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz);

extern SHORT HAL_DFE_CalculateBetaX10(SHORT A1, SHORT A2, SHORT CALF);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetMovAvHPFBw

    Description:    Sets bandwidth of the moving average high pass filter. The bandwidth of this filter
                    is calculated based on the real overspeed factor. The bandwidth is disc and speed
                    dependent.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz
                                         BD     84000 Hz
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_MAHPFBW field
    Returns:        void

    Note:           assume kHz units and max range for FDfeInClkKHz
-----------------------------------------------------------------------------------------------------*/
extern void HAL_DFE_SetMovAvHPFBw (ULONG CoefFcN1KHz, USHORT OverspeedX10, ULONG FDfeInClkKHz);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetMovAvHPFBwHeader

    Description:    Sets bandwidth of the moving average high pass filter for the header field. The bandwidth of this filter
                    is calculated based on the real overspeed factor. The bandwidth is disc and speed
                    dependent. This function is specific for DVDRAM disc.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz
                                         BD     84000 Hz
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_MAHPFBW field
    Returns:        void

    Note:           assume kHz units and max range for FDfeInClkKHz
-----------------------------------------------------------------------------------------------------*/
extern void HAL_DFE_SetMovAvHPFBwHeader (ULONG CoefFcN1KHz, USHORT OverspeedX10, ULONG FDfeInClkKHz);

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDvdRamAnalizeMode()

    Description:    Enables DVDRAM analize mode
    Inputs:
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnDvdRamAnalizeMode();
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_PresetPLLFreq()

    Description:    Presets PLL
    Inputs:         OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_INTEGRAT field(s)
    Returns:        void

    Note:           assume kHz units and max range for FDfeInClkKHz
-----------------------------------------------------------------------------------------------------*/
extern void HAL_DFE_PresetPLLFreq(USHORT OverspeedX10, ULONG FDfeInClkKHz, BYTE TrackingMethod);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ReadJitterx10()

    Description:    This function reads the fast pll jitter register and calculates the jitter in
                    tenths of a percent.

    Inputs:         None

    Outputs:        jitter in x10% (USHORT)
    Global Inputs:  HAL_DFE_FPLLJIT field(s). See note below
    Global Outputs:
    Returns:        jitter in x10% (USHORT)

    Note : READ_FIELD() does not work at the moment therefore use
               HAL_DFE_FPLLJIT_REGISTER
               HAL_DFE_FPLLJIT_MASK
               HAL_DFE_FPLLJIT_LSB
           to access field.
-----------------------------------------------------------------------------------------------------*/
extern USHORT HAL_DFE_ReadJitterX10(void);

#ifdef NOT_USED
#endif    /* NOT_USED */
extern errcode_t HAL_DFE_CalibrateDiffRfGain(USHORT TargetPeakPeakmV, BYTE ExpectedModX100, SBYTE* GainPeakPeakX2, SBYTE* OffsetPeakPeak4mV);
extern void HAL_DFE_EnableFlywheels(Bool EnableFlywheel);
extern void HAL_DFE_EnableICEDetect(void);
extern void HAL_DFE_DisableICEDetect(void);
extern void HAL_DFE_SetBCAFreq(BYTE factor_100);
extern void HAL_DFE_Init( void );

extern void HAL_DFE_EnDisHfDefect(halDFE_HfDefect_t EnableHfDefect);
extern void   HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable_t EnAgcAoc);
extern void   HAL_DFE_LoadCalRfAgcAoc(void);
extern void HAL_DFE_SetRfAgcCalGain(BYTE AgcGain);
extern void HAL_DFE_SetRfAocCalOffset(SBYTE AocOffset, SBYTE AocOffsetH);
#if (NEW_RFPOS_CALIB == 1)
extern void   HAL_DFE_ShiftRfAoc(SBYTE AocDiff, SBYTE AocOffset);
#endif
extern void HAL_DFE_SetBkDefectThr(SHORT RfAmplitude, BYTE RfModulation, BYTE BKDDUThr, BYTE BKDDLThr, BYTE EBKDUThr, BYTE EBKDLThr);
extern void HAL_DFE_SetAgcAocBoundary(BYTE MaxDynGain, BYTE MinDynGain, BYTE MaxDynOffset, BYTE MinDynOffset);
extern BOOL HAL_DFE_IsPllInLock(void);
extern USHORT HAL_DFE_GetHfOverspeed(ULONG FDfeInClkKHz);
extern void HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefect_t EnableBwDefect);
extern void HAL_DFE_SetIceDetectStAddr(ULONG PAA);
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
extern void HAL_DFE_SetBitDetCFG(USHORT OverspeedX10);
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisDataSlicer()

    Description:    Data slicer is enabled by enabling the slice level to change and allow level freezing
                    for defect occurences.

    Inputs:         EnableDataSlicer {TRUE|FALSE}

    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_FRCSLCHLD, HAL_DFE_SLCHOD field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_EnDisDataSlicer(halDFE_DataSlicerEnable_t EnableDataSlicer)
{
    WRITE_FIELD(HAL_DFE_FRCSLCHLD, !EnableDataSlicer);
    WRITE_FIELD(HAL_DFE_SLCHOD, EnableDataSlicer);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_HoldRfPll

    Description:    Un/hold the RF PLL
    Inputs:         HoldRfPll: Indicates whether the RF PLL should be put on hold, or whether the RF
                    pll should be put (back) in automatic mode
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern BYTE     PLL_InLockaid;
__inline extern void HAL_DFE_HoldRfPll(halDFE_RfPllHold_t HoldRfPll)
{
    if (PLL_InLockaid)
        WRITE_FIELD(HAL_DFE_PLLMODE, HoldRfPll? halDFE_PLLModeForceHold : halDFE_PLLModeForceInLockaid);
    else
        WRITE_FIELD(HAL_DFE_PLLMODE, HoldRfPll? halDFE_PLLModeForceHold : halDFE_PLLModeAutomatic);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetNoiseFilterBw

    Description:    Set Noise Filter Bandwidth
    Inputs:         halDFE_NoiseFIlterBW: Indicates the bandwidth for Noise Filter
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_SetNoiseFilterBw(halDFE_NoiseFilterBW_t NoiseFIlterBW)
{
    WRITE_FIELD(HAL_DFE_NOISEFBW,   NoiseFIlterBW);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisAgcAocHold()

    Description:    Enable/disable freezing of AGC Dynamic Gain/AOC Hold During Defect
    Inputs:         EnableAgcAocHold: Indicates whether freezing of AGC Dynamic Gain and AOC holding
                    during defect should be enabled or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefect_t EnableAgcAocHold)
{
    WRITE_FIELD(HAL_DFE_AGCHOLDEN,  EnableAgcAocHold);  // Enable Freezing of AGC Dynamic Gain During Defect
    WRITE_FIELD(HAL_DFE_AOCHOLDEN,  EnableAgcAocHold);  // Enable AOC Hold During Defect
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisBlankDetection()

    Description:    Enable/disable blank detection
    Inputs:         EnableBlankDetection: Indicates whether blank detection should be enabled or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_EnDisBlankDetection(halDFE_BlankDetect_t EnableBlankDetection)
{
    WRITE_FIELD(HAL_DFE_BLSDEN,  EnableBlankDetection);
#if (HOLD_TE_BLANK==1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))
    {
        WRITE_FIELD(HAL_AFE_TEBLNKHLD, EnableBlankDetection);
    }
    else
    {
        WRITE_FIELD(HAL_AFE_TEBLNKHLD,0);
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisAdaptEq()

    Description:    Enable/disable Adaptive equilizer
    Inputs:         EnableAdaptEq: Indicates whether adaptive equilizer should be enabled or
                    disabled
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_EnDisAdaptEq(halDFE_AdaptEq_t EnableAdaptEq)
{
    WRITE_FIELD(HAL_DFE_ADEQDIS,    !EnableAdaptEq);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisAdaptEqOnDefect()

    Description:    Enable/disable Adaptive equilizer On Defect
    Inputs:         EnableAdaptEq: Indicates whether adaptive equilizer should be enabled or
                    disabled on defect
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_DFE_EnDisAdaptEqOnDefect(halDFE_AdaptEqOnDefect_t EnableAdaptEqOnDefect)
{
    WRITE_FIELD(HAL_DFE_HLDADEQOD,  EnableAdaptEqOnDefect);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ResetFastPll

    Description:    Reset the Fast PLL
    Inputs:         ResetFastPll: Indicates whether the fast PLL should be reset or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_ResetFastPll(halDFE_FastPllReset_t ResetFastPll)
{
    WRITE_FIELD(HAL_DFE_FPLLRST,  ResetFastPll);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_FreezeFastPll

    Description:    Freeze the Fast PLL
    Inputs:         FreezeFastPll: Indicates whether the fast PLL should be freeze or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_FreezeFastPll(halDFE_FastPllFreeze_t FreezeFastPll)
{
    WRITE_FIELD(HAL_DFE_FPLLFHLD,  FreezeFastPll);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ResetIIRHPF

    Description:    Reset the IIR HPF
    Inputs:         ResetIIRHPF: Indicates whether the IIR HPF should be reset or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_ResetIIRHPF(halDFE_IIRHPFReset_t ResetIIRHPF)
{
    WRITE_FIELD(HAL_DFE_IIRHPFRST,  ResetIIRHPF);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_PushOutOfLock

    Description:    Push the HF PLL out of Lock
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_PushOutOfLock()
{
    WRITE_FIELD(HAL_DFE_PUSHOOL, TRUE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_IIRHPFByPass

    Description:    Bypasses or not the IIR HP Filter
    Inputs:         IirHpfBypassed: Indicates whether the IIR HP filter should be bypassed or not
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_IIRHPFByPass(halDFE_IirHpfBypass_t IirHpfBypassed)
{
    WRITE_FIELD(HAL_DFE_IIRHPFBYP, IirHpfBypassed);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetPllInLock()
    Description:    returns whether RF PLL is in lock
    Inputs:
    Outputs:        Is true when the RF PLL is in lock, is false otherwise
    Global Inputs:  HAL_DFE_LKAIDLK and HAL_DFE_LKAIDFLK(fields)
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline BOOL HAL_DFE_GetPllInLock(void)
{
    return ((READ_FIELD(HAL_DFE_LKAIDLK) == e_INLOCKAID) && (READ_FIELD(HAL_DFE_LKAIDFLK) == e_INLOCKAID));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetPllFLock()
    Description:    returns whether RF PLL is flock
    Inputs:
    Outputs:        Is true when the RF PLL is flock, is false otherwise
    Global Inputs:  HAL_DFE_LKAIDFLK(fields)
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline BOOL HAL_DFE_GetPllFLock(void)
{
    return (READ_FIELD(HAL_DFE_LKAIDFLK) == e_INLOCKAID);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetPllLock()
    Description:    returns whether RF PLL is lock
    Inputs:
    Outputs:        Is true when the RF PLL is lock, is false otherwise
    Global Inputs:  HAL_DFE_LKAIDLK(fields)
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline BOOL HAL_DFE_GetPllLocK(void)
{
    return (READ_FIELD(HAL_DFE_LKAIDLK) == e_INLOCKAID);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetPllOnHold()
    Description:    Returns whether RF PLL is on hold or out of hold
    Inputs:         None
    Outputs:        Is TRUE when the RF PLL is on hold
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline BOOL HAL_DFE_GetPllOnHold(void)
{
    return (READ_FIELD(HAL_DFE_PLLMODE) == halDFE_PLLModeForceHold);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkSetDelay
    Description:    Set the delay between the HF and the wobble
    Inputs:         delay
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkSetDelay(USHORT delay)
{
    WRITE_FIELD(HAL_DFE_BDRMDLY, delay);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkProgramSeed
    Description:    Program seed to the rom mark detector
    Inputs:         seed
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkProgramSeed(ULONG seed)
{
    WRITE_REG(HAL_DFEREG_MAP->BDRMDATA, seed);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkSetDetector
    Description:    Sets rom mark detector
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkSetDetector(void)
{
    WRITE_FIELD(HAL_DFE_BDRMSRST, FALSE);
    WRITE_FIELD(HAL_DFE_BDRMDCBW, 0x4);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkEnableDetection
    Description:    Enable the rom mark detector
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkEnableDetection(void)
{
    WRITE_FIELD(HAL_DFE_BDRMKDE, TRUE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkDisableDetection
    Description:    Disable the rom mark detector
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkDisableDetection(void)
{
    WRITE_FIELD(HAL_DFE_BDRMKDE, FALSE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkEnKeyDetectedInt
    Description:    Enable rom mark key detected interrupt
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkEnKeyDetectedInt(void)
{
    WRITE_FIELD(HAL_DFE_KEYDETE, TRUE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkDisKeyDetectedInt
    Description:    Disable rom mark key detected interrupt
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkDisKeyDetectedInt(void)
{
    WRITE_FIELD(HAL_DFE_KEYDETE, FALSE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_RomMarkGetKey
    Description:    Get the rom mark detected key
    Inputs:         ULONG *buf
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_RomMarkGetKey(ULONG *buf)
{
    WRITE_REG(HAL_DFEREG_MAP->BDRMKEY, 0x00);
    buf[0] = READ_REG(HAL_DFEREG_MAP->BDRMKEY);
    buf[1] = READ_REG(HAL_DFEREG_MAP->BDRMKEY);
    buf[2] = READ_REG(HAL_DFEREG_MAP->BDRMKEY);
    buf[3] = READ_REG(HAL_DFEREG_MAP->BDRMKEY);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ClusterStateEnableInt
    Description:    Enable cluster state detected interrupt
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_ClusterStateEnableInt(void)
{
    WRITE_FIELD(HAL_DFE_CLSTATEE, TRUE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ClusterStateDisableInt
    Description:    Disable cluster state detected interrupt
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_ClusterStateDisableInt(void)
{
    WRITE_FIELD(HAL_DFE_CLSTATEE, FALSE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetInterruptStatus
    Description:    Get the interrupt status of Digital Front End block
    Inputs:         None
    Outputs:        The DFE interrupt status
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline HAL_Reg32 HAL_DFE_GetInterruptStatus(void)
{
    return ((HAL_Reg32)READ_REG(HAL_DFEREG_MAP->DFEINT));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ClearInterruptStatus
    Description:    Clear the interrupt status of Digital Front End block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_DFE_ClearInterruptStatus(void)
{
    WRITE_REG(HAL_DFEREG_MAP->DFEINTS, 0x00);
}
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetRfAgcCalGain()

    Description:    Returns the RF AGC gain
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The AGC gain currently set
-----------------------------------------------------------------------------------------------------*/
extern __inline BYTE HAL_DFE_GetRfAgcCalGain(void)
{
     return READ_FIELD(HAL_DFE_GAIDYNDAG);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetRfAocCalOffset()

    Description:    Returns the RF AGC offset
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The AGC offset currently set
-----------------------------------------------------------------------------------------------------*/
extern __inline SBYTE HAL_DFE_GetRfAocCalOffset(void)
{
    return READ_FIELD(HAL_DFE_OFFDYNDAG);
}
#endif /* _HAL_DFE_H_ */

