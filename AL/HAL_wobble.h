/****************************************************************************
*               (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                             All Rights Reserved
*
*
* FILENAME:   HAL_wobble.h
*
* DESCRIPTION
*       This file supports the HAL WOBBLE funtions implemented in the WOBBLE HAL layer
*       HAL_WOBBLE_SetAddGain()         (Inline)
*       HAL_WOBBLE_GetMSBAtipInfoAvail  (Inline)
*       HAL_WOBBLE_SetPLLLock()         (Inline)
*       HAL_WOBBLE_ClearAddressBuf()    (Inline)
*       HAL_WOBBLE_EnDisLockAid()       (Inline)
*       HAL_WOBBLE_IsWordSyncInLock()   (Inline)
*       HAL_WOBBLE_IsBitSyncInLock()    (Inline)
*       HAL_WOBBLE_EnableWordSyncFwOutLockInt() (Inline)
*       HAL_WOBBLE_GetWblIntReg()       (Inline)
*       HAL_WOBBLE_SetWblIntReg()       (Inline)
*       HAL_WOBBLE_GetCdWblIntReg()     (Inline)
*       HAL_WOBBLE_SetCdWblIntReg()     (Inline)
*       HAL_WOBBLE_ResetATIPNewAddInt   (Inline)
*       HAL_WOBBLE_SetAGC()
*       HAL_WOBBLE_GetAGCSettings()
*       HAL_WOBBLE_SetAGCRange()
*       HAL_WOBBLE_SetPLL()
*       HAL_WOBBLE_SetATIPPLL()
*       HAL_WOBBLE_SetWobbleFreq()
*       HAL_WOBBLE_GetAddErrInfo()
*       HAL_WOBBLE_GetWblAdd()
*       HAL_WOBBLE_IsAddDetLocked()
*       HAL_WOBBLE_IsSyncDetLocked()
*       HAL_WOBBLE_IsWblAddReady()
*       HAL_WOBBLE_SetAtipAdipIntrAdd()
*       HAL_WOBBLE_GetOverspeed()
*       HAL_WOBBLE_GetPLLFreq()
*       HAL_WOBBLE_GetNewAddIntEnable()
*       HAL_WOBBLE_SetValidFlyWheelAddress()
*       HAL_WOBBLE_GetValidFlyWheelAddress()
*       HAL_WOBBLE_StoreParams()
*       HAL_WOBBLE_Init()
*       HAL_WOBBLE_SetConfLevBs()
*       HAL_WOBBLE_SetConfLevWs()
*       HAL_WOBBLE_SetConfLevId()
*       HAL_WOBBLE_EnableFlywheels()
*       HAL_WOBBLE_SetHPFInp()
*       HAL_WOBBLE_SetLPFInp()
*       HAL_WOBBLE_GetPllInLock()
*       HAL_WOBBLE_GetAddrFlyWheelInLock()
*       HAL_WOBBLE_GetSyncFlyWheelInLock()
*       HAL_WOBBLE_EnableNewWblAddInterrupt()
*       HAL_WOBBLE_SetLPFAmpMeas()
*       HAL_WOBBLE_SetAGCOnOff()
*       HAL_WOBBLE_SetPllOnHold()
*       HAL_WOBBLE_GetPllOnHold()
*       HAL_WOBBLE_CalibrateDiffWobbleGain()
*
* NOTES:
*
* $Revision: 43 $
* $Date: 11/03/15 2:21p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_WOBBLE_H_
#define _HAL_WOBBLE_H_

#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\al\REG_wobble.H"
#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#include ".\al\reg_global.h"
#endif

/*** Local structures ***/

//maximum likelyhood detectionactivation    // Define a related enum in REG_wobble.h?
enum
{
    e_ADBSYDET_DESACTIVATE,     // Desactivates the maximum likelyhood detection
    e_ADBSYDET_ACTIVATE         // Activates the maximum likelyhood detection
};


//DVD+R/RW error correction activation  // Define a related enum in REG_wobble.h?
enum
{
    e_ADERCOON_DESACTIVATE,     // Desactivates the DVD+R/RW error correction
    e_ADERCOON_ACTIVATE         // Activates the DVD+R/RW error correction
};

//Freeze flywheel mechanism activation  // Define a related enum in REG_wobble.h?
enum
{
    e_FRZFLYEN_DESACTIVATE,     // Desactivates the freeze flywheel mechanism for wobblebeat protection
    e_FRZFLYEN_ACTIVATE         // Activates the freeze flywheel mechanism for wobblebeat protection
};


//EnableFlywheels                // Define a related enum in REG_wobble.h?
enum
{
    e_RSTFLY_RESUME_OPER,       // Resume operation
    e_RSTFLY_RESET              // all the wobble address flywheels for the formats DVD+/-, BD, HDDVD RAM/RW, DVDRAM are reset
};

/*** External types */

//define DVDRAM header detection modes:
typedef enum
{
    Unknown,
    SlicedWobble,
    MLH,
    SlicedRF
} halWBLHdrDetMode_t;

//define DVDRAM header detection modes:
typedef enum
{
    Normal,
    Groove_only,
    Extend_LG
} halWBLLGDetMode_t;

//define wobble in lock facility:
typedef enum
{
    halWBL_Wobble_NotInLock,
    halWBL_Wobble_InLock
} halWBL_WobbleLock_t;

//define PLL wobble hold/unhold facility:
typedef enum
{
    halWBL_WblPllNotInHold,
    halWBL_WblPllInHold
} halWBL_WblPllHold_t;

//define wobble AGC enabling facility: needed for HAL_WOBBLE_SetAGCOnOff()
typedef enum
{
    halWBL_WobbleAGC_Disable,
    halWBL_WobbleAGC_Enable
} halWBL_AgcEnable_t;

typedef enum
{
    AMPL_MEAS_MODEL_LPF             = 0x02,
    AMPL_MEAS_MODEL_IMMEDIATE_DECAY = 0x03
} eAMPL_MEAS_MODEL;

struct WblAGCSettings
{
    halWBL_AGCAOCIntegratorBW_t   WblAGCIntBw;
    USHORT            WblAGCThreshMax;
    USHORT            WblAGCThreshMin;
    BOOL              WblInvert;
    eAMPL_MEAS_MODEL  WblAmpMeasMode;
    USHORT            WblAmpAttack;
    BOOL              WblAmpLPFBypass;
};

/* needed for HAL_WOBBLE_SetAGCRange() */
struct _PtrAGCDynLimits
{
    USHORT MaxWAGCDynGainRel;
    USHORT MinWAGCDynGainRel;
    USHORT MaxWAGCDynGainAbs;
    USHORT MinWAGCDynGainAbs;
};

typedef struct halWBL_PLLSettingS
{
    halWBL_LoopFilter_t          LoopKLPF;
    halWBL_KP_t                  LoopKP;
    halWBL_KI_t                  LoopKI;
} halWBL_PLLSettingT;

typedef struct halWBL_ATIPPLLSettingS
{
    halWBL_CDATIP_PHASEGAIN_t    CDATIPPhaseGain;
    BOOL                         DTOOn;
    halWBL_CD_KLPF_BANDWIDTH_t   CDATIPLPF;
    halWBL_CD_ATIPKP_BANDWIDTH_t CDATIPKP;
} halWBL_ATIPPLLSettingT;

/* needed for HAL_WOBBLE_GetAddErrInfo(); */

union WblErrInfoT
{
    struct
    {
        BYTE ATERRS4;
        BYTE ATERRS3;
        BYTE ATERRS2;
        BYTE ATERRS1;
    } cd;
    struct
    {
        BYTE WBLERRS4;
        BYTE WBLERRS3;
        BYTE WBLERRS2;
        BYTE WBLERRS1;
    } other;
};

/* needed for HAL_WOBBLE_SetWobbleFreq*/

typedef struct halWBL_LimitPllS
{
    USHORT Upper;
    USHORT Lower;
} halWBL_LimitPllT;

typedef enum   {ePowerDownMode = 0,
                ePresetMode,
                eNormalMode} eWPLLMode;

//*** External function prototypes
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAddGain()

    Description:    Sets additional gain for wobble signal in the digital domain before entering the
                    Wobble PLL loop
    Inputs:         eInputGain
    Outputs:
    Global Inputs:  wobble_DiscKind?
    Global Outputs: HAL_WBL_INGAINCFG  field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetAddGain(halWBL_INPUTGAIN_t InputGain)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    WRITE_FIELD(HAL_WBL_INGAINCFG, InputGain);
    return;
}
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetMSBAtipInfoAvail()

    Description:    Returns whether the MSB ATIP information is currently available. Is true if the MSB
                    ATIP information is currently available, is false otherwise.
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_CDSPCINFO  field
    Global Outputs:
    Returns:        BOOL
-----------------------------------------------------------------------------------------------------*/
__inline BOOL HAL_WOBBLE_GetMSBAtipInfoAvail(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return 0;
    }
#endif
    return((BOOL)READ_FIELD(HAL_WBL_CDSPCINFO));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGC()

    Description:    Sets GAIN, BANDWIDTH of AGC
    Inputs:         PtrWblAGCSettings pointer points to structure which contains the wobble AGC settings
                        WblAGCIntBw
                        WblAGCThreshMax
                        WblAGCThreshMin
                        WblInvert
                        WblAmpMeasMode
                        WblAmpAttack
                        WblAmpLPFBypass
    Outputs:
    Global Inputs:  wobble_DiscKind?
    Global Outputs: HAL_WBL_AGCINTBW,
                    HAL_WBL_AGCTHRHI,
                    HAL_WBL_AGCTHRLO,
                    HAL_WBL_INVWBL,
                    HAL_WBL_AMPMSMOD,
                    HAL_WBL_AMPATT,
                    HAL_WBL_AMPLPFBYP field(s)
    Returns:        void

    note:           NOT FINISHED YET
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetAGC(struct WblAGCSettings * PtrWblAGCSettings);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetAGCSettings()

    Inputs:
    Outputs:        PtrWblAGCSettings
    Global Inputs:  wobble_DiscKind?
                    HAL_WBL_AGCINTBW,
                    HAL_WBL_AGCTHRHI,
                    HAL_WBL_AGCTHRLO,
                    HAL_WBL_INVWBL,
                    HAL_WBL_AMPMSMOD,
                    HAL_WBL_AMPATT,
                    HAL_WBL_AMPLPFBYP field(s)
    Global Outputs:
    Returns:        void

    note:           NOT FINISHED YET
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_GetAGCSettings(struct WblAGCSettings * PtrWblAGCSettings);


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGCOnOff()

    Description:    Sets Wobble AGC loop on or off
    Inputs:         OnOff {TRUE|FALSE}
                        TRUE  = AGC loop on
                        FALSE = AGC loop off
    Outputs:
    Global Inputs:  wobble_DiscKind?
    Global Outputs: HAL_WBL_WBLAGCON field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetAGCOnOff(halWBL_AgcEnable_t OnOff);


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGCRange()

    Description:    Calculates and sets wobble dynamic gain of AGC
    Inputs:         MaxDynGain
                    MinDynGain
    Outputs:
    Global Inputs:  HAL_AFE_WOBVGA3GU, wobble_DiscKind?
    Global Outputs: HAL_WBL_MAXDYNGA, HAL_WBL_MINDYNGA field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetAGCRange(BYTE MaxDynGain, BYTE MinDynGain);


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetLPFAmpMeas()

    Description:    Sets LPF for AGC amplitude measurement
    Inputs:         eAMP_LPF_BW
    Outputs:
    Global Inputs:  wobble_DiscKind?
    Global Outputs: HAL_WBL_LPLPFBW field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetLPFAmpMeas(halWBL_AmplitudeLPFBW_t AmpLPFBw);


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetPLL()

    Description:    Sets bandwidth and integrator for wobble PLL
    Inputs:         pointer to wobble pll setting struct WobblePLLSettingT:
                        pWobblePLLSetting
    Outputs:
    Global Inputs:  wobble_DiscKind
    Global Outputs: HAL_WBL_CDLPKLCFG,
                    HAL_WBL_CDLPKPCFG,
                    HAL_WBL_CDLPKICFG
                    or
                    HAL_WBL_LPKLPFCFG,
                    HAL_WBL_LPKPCFG,
                    HAL_WBL_LPKICFG
                    field(s), depending on the disc kind
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetPLL(halWBL_PLLSettingT * pWobblePLLSetting);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetATIPPLL()

    Description:    Sets bandwidth and integrator for ATIP part in CD wobble PLL
    Inputs:         pointer to ATIP part in CD wobble pll setting struct WobbleATIPPLLSettingT:
                        pWobbleATIPPLLSetting
    Outputs:
    Global Inputs:
    Global Outputs: HAL_WBL_ATNDTOGS,
                    HAL_WBL_ATNDTOM,
                    HAL_WBL_ATKLPFCFG,
                    HAL_WBL_ATKPCFG,
                    field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetATIPPLL( halWBL_ATIPPLLSettingT * pWobbleATIPPLLSetting);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetPLLLock()

    Description:    Sets lock condition for wobble PLL
    Inputs:         WblLockDetPeriods
                    WblLockDetThreshold

    Outputs:
    Global Inputs:  wobble_DiscKind?
    Global Outputs: HAL_WBL_LCKDETPER, HAL_WBL_LCKDETTHR field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetPLLLock(USHORT WblLockDetPeriods, USHORT WblLockDetThreshold)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif
    WRITE_FIELD(HAL_WBL_LCKDETPER, WblLockDetPeriods);
    WRITE_FIELD(HAL_WBL_LCKDETTHR, WblLockDetThreshold);
    return;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetWobbleFreq()

    Description:    Sets presets wobble frequency
    Inputs:         OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FreqFactor        -> 80..120
                    LockAid           -> -128..127
                    FWobbleAdcClk     -> clock frequency of DFE module in KHz units
                                         CD      60000 KHz
                                         DVD    120000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
                    struct LimitPLL (Upper, Lower)
    Outputs:
    Global Inputs:  wobble_DiscKind
    Global Outputs: HAL_WBL_LCKAIDPH, HAL_WBL_ATPLLFRQ, HAL_WBL_WBLFREQ field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetWobbleFreq(USHORT OverspeedX10, BYTE FreqFactor, SBYTE LockAid, ULONG FWobbleAdcClk, halWBL_LimitPllT * LimitPLL);
extern USHORT HAL_WOBBLE_SetWobbleFreqLimit(USHORT OverspeedX10, ULONG FWobbleAdcClk, halWBL_LimitPllT * LimitPLL);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_ClearAddressBuf()
    Description:    Flushes address buffer, by just reading HAL_WBL_WBADRDAT2_REGISTER
    Inputs:         void
    Outputs:
    Global Inputs:  HAL_WBL_WBADRDAT2_REGISTER, wobble_DiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void  HAL_WOBBLE_ClearAddressBuf(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    (void)READ_REG(HAL_WBLREG_MAP->ADBFRDEND);
    return;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_EnDisLockAid()
    Description:    Enables / disables lock aid circuitry
    Inputs:         EnDisLockAid {TRUE|FALSE}
    Outputs:
    Global Inputs:  wobble_DiscKind
    Global Outputs: HAL_WBL_LCKAIDEN field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_EnDisLockAid(BOOL EnDisLockAid)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    WRITE_FIELD(HAL_WBL_LCKAIDEN, EnDisLockAid);
    return;
}

extern ULONG HAL_WOBBLE_GetWblAdd(BYTE* IDDATA, ULONG status);

#if  0 //currently not be use
extern void HAL_WOBBLE_GetAddErrInfo(union WblErrInfoT * PtrWblErrInfo);
extern BOOL HAL_WOBBLE_IsAddDetLocked(void);
extern BOOL HAL_WOBBLE_IsSyncDetLocked(void);
extern BOOL HAL_WOBBLE_IsWblAddReady(void);
extern void HAL_WOBBLE_SetAtipAdipIntrAdd(void);
extern void HAL_WOBBLE_SetConfLevBs(BYTE MaxConfidence, BYTE HiThresLa, BYTE HiThres, BYTE LoThres);
extern void HAL_WOBBLE_SetConfLevWs(BYTE MaxConfidence, BYTE HiThres, BYTE LoThres);
extern void HAL_WOBBLE_SetConfLevId(BYTE MaxConfidence, BYTE HiThres, BYTE LoThres);
#endif //currently not be use

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_IsWordSyncInLock()
    Description:    checks lock status word detection
    Inputs:         BOOL
    Outputs:
    Global Inputs:  HAL_WBL_WDSYILCKS,
                    wobble_DiscKind
    Global Outputs:
    Returns:        BOOL (TRUE = word detection in lock, FALSE = not in lock)
-----------------------------------------------------------------------------------------------------*/
__inline BOOL HAL_WOBBLE_IsWordSyncInLock(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return 0;
    }
#endif

    return (READ_FIELD(HAL_WBL_WDSYILCKS) != 0);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetOverspeed()
    Description:    calculates overspeed by means of measured wobble frequency
    Inputs:         FWobbleADCClkKHz  -> clock frequency of DFE module in KHz units
                                         CD      60000 KHz
                                         DVD    120000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        USHORT Overspeed  -> overspeed in tenths
                                         CD     200..5200
                                         DVD    200..2000
                                         BD     100..1400
                                         HDDVD  100..1000
    Global Inputs:  HAL_WBL_LCKAIDPH, (field) ???
                    HAL_WBL_ATPLLFRQ, (field) CD
                    HAL_WBL_WBLFREQ,  (field) OTHER
                    wobble_DiscKind
    Global Outputs:
    Returns:        OverspeedX100
-----------------------------------------------------------------------------------------------------*/
extern USHORT HAL_WOBBLE_GetOverspeed(ULONG FWobbleADCClkKHz);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetPLLFreq()
    Description:    gets pll frequency
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_ATPLLFRQ,     CD
                    HAL_WBL_WBLFREQ,      OTHER
                    wobble_DiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern USHORT HAL_WOBBLE_GetPLLFreq(void);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_IsBitSyncInLock()
    Description:    returns lock status of bit sync
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_BTSYILCKS (field)
                    wobble_DiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline BOOL HAL_WOBBLE_IsBitSyncInLock(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return 0;
    }
#endif

    return (READ_FIELD(HAL_WBL_BTSYILCKS) != 0);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_EnableWordSyncFwOutLockInt()
    Description:    Enables or disables the Word Sync Flywheel Out of Lock Interrupt
    Inputs:         OnOff: If true, the Word Sync Flywheel Out of Lock Interrupt should be enabled,
                    otherwise the Word Sync Flywheel Out of Lock Interrupt should be disabled
    Outputs:
    Global Inputs:
    Global Outputs: HAL_WBL_WDSYOLCKE
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_EnableWordSyncFwOutLockInt(BOOL OnOff)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    WRITE_FIELD(HAL_WBL_WDSYOLCKE,OnOff);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetWblIntReg()
    Description:    Returns the WOBBLE INTERRUPT REGISTER
    Inputs:
    Outputs:        the value of the WOBBLE INTERRUPT REGISTER
    Global Inputs:  WBLINT
    Global Outputs:
    Returns:        ULONG
-----------------------------------------------------------------------------------------------------*/
__inline ULONG HAL_WOBBLE_GetWblIntReg(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return 0;
    }
#endif

    return((ULONG)READ_REG(HAL_WBLREG_MAP->WBLINT));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetWblIntReg()
    Description:    Sets the WOBBLE INTERRUPT REGISTER
    Inputs:         WblIntRegVal: The value to be set
    Outputs:
    Global Inputs:
    Global Outputs: WBLINT
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetWblIntReg(ULONG WblIntRegVal)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    WRITE_REG(HAL_WBLREG_MAP->WBLINT, WblIntRegVal);
    WRITE_REG(HAL_WBLREG_MAP->WBLINTS, WblIntRegVal);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetCdWblIntReg()
    Description:    Returns the CD WOBBLE INTERRUPT REGISTER
    Inputs:
    Outputs:        the value of the CD WOBBLE INTERRUPT REGISTER
    Global Inputs:  CDWBLINT
    Global Outputs:
    Returns:        ULONG
-----------------------------------------------------------------------------------------------------*/
__inline ULONG HAL_WOBBLE_GetCdWblIntReg(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return 0;
    }
#endif

    return((ULONG)READ_REG(HAL_WBLREG_MAP->CDWBLINT));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetCdWblIntReg()
    Description:    Sets the CD WOBBLE INTERRUPT REGISTER
    Inputs:         CdWblIntRegVal: The value to be set
    Outputs:
    Global Inputs:
    Global Outputs: CDWBLINT
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetCdWblIntReg(ULONG CdWblIntRegVal)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    WRITE_REG(HAL_WBLREG_MAP->CDWBLINT, CdWblIntRegVal);
    WRITE_REG(HAL_WBLREG_MAP->CDWBLINTS, CdWblIntRegVal);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_ResetATIPNewAddInt()
    Description:    Resets the ATIP new address available interrupt
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_ATNEWADI
    Global Outputs:
    Returns:        BOOL
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_ResetATIPNewAddInt(void)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
#endif

    CLEAR_INT_FIELD(HAL_WBL_ATNEWADI, 0);
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           void HAL_WOBBLE_Set_Enable_Header_on()

    Description:    Enables signal-hold during the header
    Inputs:
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetEnableHeaderOn()
{
    #if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
    #endif

    WRITE_FIELD(HAL_WBL_HHEN, halWBL_HdrHoldEnable);
    WRITE_FIELD(HAL_WBL_HRFEN, halWBL_HdrRfEnable);
    WRITE_FIELD(HAL_WBL_INTHDRON, halWBL_IntHdrIndOn);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           void HAL_WOBBLE_Set_Enable_Header_off()

    Description:    Disables signal-hold during the header
    Inputs:
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_WOBBLE_SetEnableHeaderOff()
{
    #if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return ;
    }
    #endif

    WRITE_FIELD(HAL_WBL_HHEN, halWBL_HdrHoldDisable);
    WRITE_FIELD(HAL_WBL_HRFEN, halWBL_HdrRfDisable);
    WRITE_FIELD(HAL_WBL_INTHDRON, halWBL_IntHdrIndOff);
}
extern halWBLHdrDetMode_t preHeaderDetectMode;
#endif /* #if (DVD_RAM_READ == 1) */

extern void HAL_WOBBLE_Init( void );
extern void HAL_WOBBLE_EnableFlywheels(Bool EnableFlywheel);
extern errcode_t HAL_WOBBLE_CalibrateDiffWobbleGain(USHORT TargetAdcmV, BYTE* Table_Index);
extern void HAL_WOBBLE_SetPllOnHold(halWBL_WblPllHold_t WobbleHold);
extern BOOL HAL_WOBBLE_GetPllOnHold(void);
extern BOOL HAL_WOBBLE_GetPllInLock(ULONG WblClkKHz);
extern BOOL HAL_WOBBLE_GetAddrFlyWheelInLock(void);
extern BOOL HAL_WOBBLE_GetSyncFlyWheelInLock(void);
extern void HAL_WOBBLE_EnableNewWblAddInterrupt(BOOL OnOff);
extern BOOL HAL_WOBBLE_GetNewAddIntEnable(void);
extern void HAL_WOBBLE_SetValidFlyWheelAddress(BOOL Valid);
extern BOOL HAL_WOBBLE_GetValidFlyWheelAddress(void);
extern void HAL_WOBBLE_SetLPFInp(ULONG PllInpLowPassFcN1Hz, ULONG AgcInpLowPassFcN1Hz, ULONG LPPLowPassFcN1Hz, USHORT OverspeedX10, ULONG ADCWblClkKHz);
extern void HAL_WOBBLE_SetHPFInp(ULONG PllInpHighPassFcN1Hz, ULONG AgcInpHighPassFcN1Hz, ULONG LPPHighPassFcN1Hz, USHORT OverspeedX10, ULONG ADCWblClkKHz);

#endif /* _HAL_WOBBLE_H_ */
