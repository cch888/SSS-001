/* ******************************************************************************************
*
*               (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                             All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   HAL_wobble.c
*
*  Description:
*   This file contains functions for the Wobble subsystem and includes
*   the following:
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
*       HAL_WOBBLE_Init()
*       HAL_WOBBLE_SetConfLevBs()
*       HAL_WOBBLE_SetConfLevWs()
*       HAL_WOBBLE_SetConfLevId()
*       HAL_WOBBLE_EnableFlywheels()
*       HAL_WOBBLE_SetHPFInp()
*       HAL_WOBBLE_SetLPFInp()
*       HAL_WOBBLE_GetPllInLock()
*       HAL_WOBBLE_GetSyncFlyWheelInLock()
*       HAL_WOBBLE_EnableNewWblAddInterrupt()
*       HAL_WOBBLE_SetLPFAmpMeas()
*       HAL_WOBBLE_SetAGCOnOff()
*       HAL_WOBBLE_SetPllOnHold()
*       HAL_WOBBLE_GetPllOnHold()
*       HAL_WOBBLE_CalibrateDiffWobbleGain()
*
* Notes:
*
*
*   $Revision: 124 $
*   $Date: 11/03/15 2:21p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_wobble.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_wobble.h"
#include ".\AL\HAL_pll.h"
#include ".\common\adc.h"
#include ".\Common\ComMath.h"
#include ".\DVD\dvDef.h"

#if (BD_ENABLE == 1)
#include ".\bd\bdDef.h"
#include ".\bd\bdVars.h"
#endif
#include ".\servo\SvoVar.H"
#include ".\servo\svo_calib.h"
#include ".\servo\SVO.H"

#if (DVD_RAM_READ == 1)
#include ".\Common\hwdef.h"
#include ".\Common\IRAM.h"
#endif

/*** Local defines ***/

/* local defines for HAL_WOBBLE_SetAGCRange() */
#define MAX_WAGC_DYN_GAIN_ABS   15
#define MIN_WAGC_DYN_GAIN_ABS    0
#define NR_SMPLS_FOR_AVG 10

/* local defines for HAL_WOBBLE_CalibrateDiffRfGain */
#define MAX_INDEX_GAIN_TABLE    4

/* local defines for HAL_WOBBLE_SetHPFInp */
#define ADC_WBL_CLK_KHZ_REF     8465
#define SIZE_CD_HPF_TABLE       4
#define CD_HPF_TABLEX10         {105, 209, 415, 819}
#define SIZE_NOT_CD_HPF_TABLE   10
#define NOT_CD_HPF_TABLE        {20, 41, 81, 163, 325, 647, 1284, 2530, 4920, 9349}

/* local defines for HAL_WOBBLE_SetLPFInp */
#define SIZE_LPF_TABLE          6
#define LPF_TABLE               {168, 349, 755, 1249, 2290, 4423}

/* local defines for HAL_WOBBLE_SetWobbleFreq */
#define ONE_PERCENT_FACTOR      100

#define TWO_TO_POWER_18 262144 /* 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2 */
#define TWO_TO_POWER_15 32768  /* 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2 */

/*** Local static variables */
static BOOL mValidFlyWheelAddress;


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGC()

    Inputs:         PtrWblAGCSettings pointer points to structure which contains the wobble AGC settings
                        WblAGCIntBw
                        WblAGCThreshMax
                        WblAGCThreshMin
                        WblInvert
                        WblAmpMeasMode
                        WblAmpAttack
                        WblAmpLPFBypass
    Inputs:
    Outputs:
    Global Inputs:  mDiscKind?
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
void HAL_WOBBLE_SetAGC(struct WblAGCSettings * PtrWblAGCSettings)
{
    struct WblAGCSettings * p;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif
    p = PtrWblAGCSettings;

    WRITE_FIELD(HAL_WBL_WAGCINTBW,  p->WblAGCIntBw);
    WRITE_FIELD(HAL_WBL_WAGCTHRHI,  p->WblAGCThreshMax);
    WRITE_FIELD(HAL_WBL_WAGCTHRLO,  p->WblAGCThreshMin);
    WRITE_FIELD(HAL_WBL_INVWBL,    p->WblInvert);
    WRITE_FIELD(HAL_WBL_AMPMSMOD,  p->WblAmpMeasMode);
    WRITE_FIELD(HAL_WBL_AMPATT,    p->WblAmpAttack);
    WRITE_FIELD(HAL_WBL_AMPLPFBYP, p->WblAmpLPFBypass);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetAGCSettings()

    Inputs:
    Outputs:        PtrWblAGCSettings
    Global Inputs:  mDiscKind?
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
void HAL_WOBBLE_GetAGCSettings(struct WblAGCSettings * PtrWblAGCSettings)
{
    struct WblAGCSettings * p;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    p = PtrWblAGCSettings;

    p->WblAGCIntBw     = READ_FIELD(HAL_WBL_WAGCINTBW);
    p->WblAGCThreshMax = READ_FIELD(HAL_WBL_WAGCTHRHI);
    p->WblAGCThreshMin = READ_FIELD(HAL_WBL_WAGCTHRLO);
    p->WblInvert       = READ_FIELD(HAL_WBL_INVWBL);
    p->WblAmpMeasMode  = READ_FIELD(HAL_WBL_AMPMSMOD);
    p->WblAmpAttack    = READ_FIELD(HAL_WBL_AMPATT);
    p->WblAmpLPFBypass = READ_FIELD(HAL_WBL_AMPLPFBYP);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGCRange()

    Description:    Calculates and sets wobble dynamic gain of AGC
    Inputs:         MaxDynGain
                    MinDynGain
    Outputs:
    Global Inputs:  HAL_AFE_WOBVGA3GU, mDiscKind?
    Global Outputs: HAL_WBL_MAXDYNGA, HAL_WBL_MINDYNGA field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern void HAL_WOBBLE_SetAGCRange(BYTE MaxDynGain, BYTE MinDynGain)
{
    USHORT i;
    USHORT Gain;
    SHORT  temp;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    /* initialising */
    Gain = 0;

    /* measurement and averaging over NR_SMPLS_FOR_AVG measure samples*/
    for (i = 0; i < NR_SMPLS_FOR_AVG; i++)
    {
        Gain += READ_FIELD(HAL_WBL_GNDYNMON);
    }
    Gain /= NR_SMPLS_FOR_AVG;

    /* calculate maximum wobble dynamic gain */
    temp = Gain + MaxDynGain;
    if (temp > MAX_WAGC_DYN_GAIN_ABS) temp = MAX_WAGC_DYN_GAIN_ABS;
    WRITE_FIELD(HAL_WBL_MAXDYNGA, temp);

    /* calculate minimum wobble dynamic gain */
    temp = Gain - MinDynGain;
    if (temp < MIN_WAGC_DYN_GAIN_ABS) temp = MIN_WAGC_DYN_GAIN_ABS;
    WRITE_FIELD(HAL_WBL_MINDYNGA, temp);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetPLL()

    Description:    Sets bandwidth and integrator for wobble PLL
    Inputs:         pointer to wobble pll setting struct WobblePLLSettingT:
                        pWobblePLLSetting
    Outputs:
    Global Inputs:  mDiscKind
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
void HAL_WOBBLE_SetPLL(halWBL_PLLSettingT * pWobblePLLSetting)
{

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            WRITE_FIELD(HAL_WBL_CDLPKLCFG, pWobblePLLSetting->LoopKLPF);
            WRITE_FIELD(HAL_WBL_CDLPKPCFG, pWobblePLLSetting->LoopKP);
            WRITE_FIELD(HAL_WBL_CDLPKICFG, pWobblePLLSetting->LoopKI);
            break;

        case eDiscDVD:
#if (BD_ENABLE == 1)
        case eDiscBD:
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
#endif
            WRITE_FIELD(HAL_WBL_LPKLPFCFG, pWobblePLLSetting->LoopKLPF);
            WRITE_FIELD(HAL_WBL_LPKPCFG,   pWobblePLLSetting->LoopKP);
            WRITE_FIELD(HAL_WBL_LPKICFG,   pWobblePLLSetting->LoopKI);
            break;

        default:
            break;
    }
}

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
void HAL_WOBBLE_SetATIPPLL( halWBL_ATIPPLLSettingT * pWobbleATIPPLLSetting)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    WRITE_FIELD(HAL_WBL_ATNDTOGS,  pWobbleATIPPLLSetting->CDATIPPhaseGain);
    WRITE_FIELD(HAL_WBL_ATNDTOM,   pWobbleATIPPLLSetting->DTOOn);
    WRITE_FIELD(HAL_WBL_ATKLPFCFG, pWobbleATIPPLLSetting->CDATIPLPF);
    WRITE_FIELD(HAL_WBL_ATKPCFG,   pWobbleATIPPLLSetting->CDATIPKP);
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
                                         BD     170000 KHz
                                         HDDVD  120000 KHz
                    struct LimitPLL (Upper, Lower)
    Outputs:
    Global Inputs:  mDiscKind
    Global Outputs: HAL_WBL_LCKAIDPH, HAL_WBL_ATPLLFRQ, HAL_WBL_WBLFREQ field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetWobbleFreq(USHORT OverspeedX10, BYTE FreqFactor, SBYTE LockAid, ULONG FWobbleAdcClk, halWBL_LimitPllT * LimitPLL)
{
    ULONG PresetFreq;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    // Lock Aid Phase
    WRITE_FIELD(HAL_WBL_LCKAIDPH, LockAid);

    HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);        /* Hold/Preset Wobble PLL */

    //determine the wobble PLL frequency
    PresetFreq = HAL_WOBBLE_SetWobbleFreqLimit(OverspeedX10, FWobbleAdcClk, LimitPLL);
    PresetFreq = PresetFreq * FreqFactor / ONE_PERCENT_FACTOR;
    if (PresetFreq > 0xFFFF)
        PresetFreq = 0xFFFF;
    if (mDiscKind.Bit.DiscStd == eDiscCD)
    {
        WRITE_FIELD(HAL_WBL_ATPLLFRQ, PresetFreq);
    }
    else
    {
        WRITE_FIELD(HAL_WBL_WBLFREQ, PresetFreq);
    }
    SendMsgCn(DEBUG_WBL_MSG,4,0x410000,A4B(PresetFreq)); // WobblePresetFreq
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetWobbleFreqLimit()

    Description:    Sets wobble frequency limit
    Inputs:         OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    FWobbleAdcClk     -> clock frequency of DFE module in KHz units
                                         CD      60000 KHz
                                         DVD    120000 KHz
                                         BD     170000 KHz
                                         HDDVD  120000 KHz
                    struct LimitPLL (Upper, Lower)
    Outputs:
    Global Inputs:  mDiscKind
    Global Outputs: HAL_WBL_LCKAIDPH, HAL_WBL_ATPLLFRQ, HAL_WBL_WBLFREQ field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
USHORT HAL_WOBBLE_SetWobbleFreqLimit(USHORT OverspeedX10, ULONG FWobbleAdcClk, halWBL_LimitPllT * LimitPLL)
{
    ULONG  PresetFreq;
    halWBL_LimitPllT * p = LimitPLL;
    USHORT lim;

#if EN_POWER_CONTROL
    if (WobbleCLKStop)
    {
        return 0;
    }
#endif

    //determine the wobble PLL frequency
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            PresetFreq = (((((ULONG)CD_WBL_FREQ_N1 * OverspeedX10) << 8) / 1000) << 10) / (FWobbleAdcClk * 10);
            break;

#if (BD_ENABLE == 1)
       case eDiscBD:
            PresetFreq = (((((((ULONG)BD_WBL_FREQ_N1 * OverspeedX10) << 4) / 1000 * 23) << 6) / 10) << 3) / (FWobbleAdcClk >> 2);
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            PresetFreq = (((((((ULONG)HDDVD_WBL_FREQ_N1 * OverspeedX10) << 5) / 1000 * 31) << 5) / 10) << 3) / (FWobbleAdcClk >> 2);
            break;
#endif

        case eDiscDVD:
            switch (mDiscKind.Bit.WblType)
            {
                case eWblUnknown:
#if (DVD_RAM_READ == 1)
                    if (DVDRAM)
                    {
                        PresetFreq = (((((((ULONG)DVDRAM_WBL_FREQ_N1 * OverspeedX10) << 7) / 1000 * 93) << 3) / 10) << 3) / (FWobbleAdcClk >> 2);
                        break;
                    }
#endif
                    return 0;

                case eWblPlus:
                    PresetFreq = (((((((ULONG)DVD_PR_PRW_WBL_FREQ_N1 * OverspeedX10) << 4) / 1000 * 16) << 6) / 10) << 3) / (FWobbleAdcClk >> 2);
                    break;

                case eWblMinus:
                    PresetFreq = (((((((ULONG)DVD_MR_MRW_WBL_FREQ_N1 * OverspeedX10) << 7) / 1000 * 93) << 3) / 10) << 3) / (FWobbleAdcClk >> 2);
                    break;

                default:
                    return 0;
            }
            break;

        default:
            return 0;
    }
    if (PresetFreq > 0xFFFF)
        PresetFreq = 0xFFFF;

    if (mDiscKind.Bit.DiscStd != eDiscCD)
    {
        //set the boundaries (not valid in CD case)
        lim = (PresetFreq >> 8) * (p->Upper) / ONE_PERCENT_FACTOR;
        WRITE_FIELD(HAL_WBL_PLLINTHIT, (lim > 0xFE)? 0xFE : lim);//if set to 0xFF the wobble freq does not jump to hi limit when it lower than lo limit.

        lim = (PresetFreq >> 8) * (p->Lower) / ONE_PERCENT_FACTOR;
        WRITE_FIELD(HAL_WBL_PLLINTLOT, (lim == 0x00)? 0x01 : lim);
    }

    return PresetFreq;
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetWblAdd()
    Description:    Gets wobble address and AUX Data
    Inputs:
    Outputs:        IDDATA: Address and Data information
    Global Inputs:  mDiscKind
    Global Outputs:
    Returns:        Address
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_WOBBLE_GetWblAdd(BYTE *IDDATA, ULONG status)
{
    ULONG Address = 0;
    ULONG Data;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return Address;
    }
#endif

    SendMsg80(DEBUG_WBL_MSG,0x410100);   // HAL_WOBBLE_GetWblAdd

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            Data = READ_REG(HAL_WBLREG_MAP->ATADBFRDD);
            IDDATA[0] = (BYTE)(Data>>16);      //CD ATIP Decoded Minute Value
            IDDATA[1] = (BYTE)(Data>>8);       //CD ATIP Decoded Second Value
            IDDATA[2] = (BYTE) Data;           //CD ATIP Decoded Frame Value

            // CD ATIP Decoded Address
            Address = READ_REG(HAL_WBLREG_MAP->ATADBFRDE);     //release the channel buffer
            break;

        case eDiscDVD:
            if (mDiscKind.Bit.WblType == eWblMinus)
            {
                // temporary ADBFRDDAT because minR flyadress not yet reliable
                Address = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                Address = (ULONG)(0x00FFFFFF - Address)<<4 ;
                // Address -= ONE_DVD_BLOCK;

                //send_msg5S(DEBUG_WBL_MSG, "FlyAdd-1");
                //send_msg5L(DEBUG_WBL_MSG, Address);

                IDDATA[0] = (BYTE)(Address>>16);      //LPPAddress(23:16)
                IDDATA[1] = (BYTE)(Address>>8);       //LPPAddress(15:8)
                IDDATA[2] = (BYTE) Address;           //LPPAddress(7:0)

                Address += ONE_DVD_BLOCK;

                //Data = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT); //dummy read to increase pointer to next part
                //Data = (ULONG)(0x00FFFFFF - Data)<<4;
                //send_msg5S(DEBUG_WBL_MSG, "DataAdress");
                //send_msg5L(DEBUG_WBL_MSG, Data);

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                IDDATA[3] = (BYTE)Data;        //Parity A byte 1

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                IDDATA[4] = (BYTE)(Data>>24);  //Parity A byte 2
                IDDATA[5] = (BYTE)(Data>>16);  //Parity A byte 3
                IDDATA[6] = (BYTE)(Data>>8);   //Field ID
                IDDATA[7] = (BYTE)Data;        //disc information byte 1

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                IDDATA[8] = (BYTE)(Data>>24);  //disc information byte 2
                IDDATA[9] = (BYTE)(Data>>16);  //disc information byte 3
                IDDATA[10] = (BYTE)(Data>>8);   //disc information byte 4
                IDDATA[11] = (BYTE)Data;        //disc information byte 5

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDEND);
                IDDATA[12] = (BYTE)(Data>>24);  //disc information byte 6
                IDDATA[13] = (BYTE)(Data>>16);  //Parity B byte 1
                IDDATA[14] = (BYTE)(Data>>8);   //Parity B byte 2
                IDDATA[15] = (BYTE)Data;        //Parity B byte 3
            }
            else if(mDiscKind.Bit.WblType == eWblPlus)
            {
                //read always address from flywheel
                Address = READ_REG(HAL_WBLREG_MAP->ADFLYADR);
                Address = (ULONG)(Address<<2);   //Flywheel Sector address
                Address -= SECTORS_PER_ADIPWORD; // (Flywheel_address-4) equal to buffered sector address

//                send_msg5S(DEBUG_WBL_MSG, "FlyAdd-1");
//                send_msg5L(DEBUG_WBL_MSG, Address);

                IDDATA[0] = (BYTE)(Address>>16);      //LPPAddress(23:16)
                IDDATA[1] = (BYTE)(Address>>8);       //LPPAddress(15:8)
                IDDATA[2] = (BYTE) Address;           //LPPAddress(7:0)

                Address += SECTORS_PER_ADIPWORD; //Next sector address from wobble address

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT); //dummy read to increase pointer to next part
                Data = (ULONG)(Data<<2);
//                send_msg5S(DEBUG_WBL_MSG, "DataAdress");
//                send_msg5L(DEBUG_WBL_MSG, Data);

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
                IDDATA[3] = (BYTE)Data;            //AUX data byte
            }
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            //send_msg5S(DEBUG_WBL_MSG, "Inside HAL_WOBBLE_GetWblAdd");
            //if(READ_FIELD(HAL_WBL_ERRFREE) == 1)
            //{
            //    Address = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
            //    send_msgA4(DEBUG_WBL_MSG,0x610101,Address);//BD decoder wobble address
            //}
            //else
            {
                Address = READ_REG(HAL_WBLREG_MAP->BDFLYADDR);
                SendMsgCn(DEBUG_WBL_MSG,4,0x410102,A4B(Address)); // BD flywheel wobble address
            }
            IDDATA[0] = (BYTE)(Address>>16);          //decoded ADIP word(23:16)
            IDDATA[1] = (BYTE)(Address>>8);           //decoded ADIP word(15:8)
            IDDATA[2] = (BYTE) Address;               //decoded ADIP word(7:0)
            //send_msg5L(DEBUG_WBL_MSG, Address);

            Data = READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
            IDDATA[3] = (BYTE)(Data>>8);           //AUX bits from the ADIP word(11:8)
            IDDATA[4] = (BYTE) Data;               //AUX bits from the ADIP word(7:0)
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if ((mDiscKind.Bit.Media == eMediaR)||(mDiscKind.Bit.Media == eMediaRW))
            {
                Address = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                IDDATA[0] = (BYTE)(Address>>16);          //RW WAP Address: PS Block(20:3)
                IDDATA[1] = (BYTE)(Address>>8);           //RW WAP Address: PS Block(20:3)
                IDDATA[2] = (BYTE) Address;               //RW WAP Address: PS Block(20:3)/Segment order(2:0)

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
                IDDATA[3] = (BYTE)(Data>>8);           //Segment information(11:9)/RW WAP address CRC(8:0)
                IDDATA[4] = (BYTE) Data;               //RW WAP address CRC(8:0)
            }
            else if (mDiscKind.Bit.Media == eMediaRAM)
            {
                Address = READ_REG(HAL_WBLREG_MAP->ADBFRDDAT);
                IDDATA[0] = (BYTE)(Address>>16);          //RAM WAP Address: Zone(22:18)/Track(17:6)
                IDDATA[1] = (BYTE)(Address>>8);           //RAM WAP Address: Track(17:6)
                IDDATA[2] = (BYTE) Address;               //RAM WAP Address: Track(17:6)/Segment(5:0)

                Data = READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
                IDDATA[3] = (BYTE) Data;               //RAM WAP address parity(3)/Segment information(2:0)
            }
            break;
#endif // (ENABLE_HDDVD == 1)

        case eDiscLS:   /* No Address in case of eDiscLS, return 0 */
        default:        /* OTHER */
            break;
    }
    return Address;
}

#if  0 //currently not be use
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetAddErrInfo()
    Description:    Gets address error information
    Inputs:         pointer to struct where results are to be stored (WblErrInfoT)
    Outputs:        struct filled
    Global Inputs:  HAL_WBL_ATERRS4_REGISTER,              (register address)
                    HAL_WBL_ATERRS4 (field)
                    HAL_WBL_ATERRS3 (field)
                    HAL_WBL_ATERRS2 (field)
                    HAL_WBL_ATERRS1 (field)
                    HAL_WBL_WBLERRS4_REGISTER              (register address)
                    HAL_WBL_WBLERRS4 (field)
                    HAL_WBL_WBLERRS3 (field)
                    HAL_WBL_WBLERRS2 (field)
                    HAL_WBL_WBLERRS1 (field)
                    mDiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_GetAddErrInfo(union WblErrInfoT * PtrWblErrInfo)
{
    union WblErrInfoT * p = PtrWblErrInfo;
    ULONG Buffer;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            Buffer = READ_REG(HAL_WBLREG_MAP->ATADERRAT);
            p->cd.ATERRS1 = READ_FIELDFROM(Buffer, HAL_WBL_ATERRS1);
            p->cd.ATERRS2 = READ_FIELDFROM(Buffer, HAL_WBL_ATERRS2);
            p->cd.ATERRS3 = READ_FIELDFROM(Buffer, HAL_WBL_ATERRS3);
            p->cd.ATERRS4 = READ_FIELDFROM(Buffer, HAL_WBL_ATERRS4);
            break;

        case eDiscLS:
            break;

        default:
            Buffer = READ_REG(HAL_WBLREG_MAP->WBADERRAT);
            p->other.WBLERRS1 = READ_FIELDFROM(Buffer, HAL_WBL_WBLERRS1);
            p->other.WBLERRS2 = READ_FIELDFROM(Buffer, HAL_WBL_WBLERRS2);
            p->other.WBLERRS3 = READ_FIELDFROM(Buffer, HAL_WBL_WBLERRS3);
            p->other.WBLERRS4 = READ_FIELDFROM(Buffer, HAL_WBL_WBLERRS4);
            break;
    }
}
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_IsAddDetLocked()
    Description:    returns lock status of address detection
    Inputs:
    Outputs:        BOOL
    Global Inputs:  HAL_WBL_ATADILCKS,
                    HAL_WBL_ADDRILCKS,
                    mDiscKind
    Global Outputs:
    Returns:        BOOL (TRUE inlock, FALSE out of lock)
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_IsAddDetLocked(void)
{
    BOOL InSync = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return InSync;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            InSync = (READ_FIELD(HAL_WBL_ATADILCKS) != 0);
            break;

        case eDiscLS:
            break;

        default:
            InSync = (READ_FIELD(HAL_WBL_ADDRILCKS) != 0);
            break;
    }

    return InSync;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_IsSyncDetLocked()
    Description:    checks lock status of of combined address-, word- and sync detection
    Inputs:
    Outputs:        BOOL
    Global Inputs:  HAL_WBL_ATADILCKS,   //CD
                    HAL_WBL_ATSYILCKS,
                    HAL_WBL_ADDRILCKS,   //OTHER
                    HAL_WBL_WDSYILCKS,
                    HAL_WBL_BTSYILCKS,
                    mDiscKind
    Global Outputs:
    Returns:        BOOL (TRUE inlock, FALSE out of lock)
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_IsSyncDetLocked(void)
{
    BOOL AddressWordSyncInLock = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return AddressWordSyncInLock;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            AddressWordSyncInLock = (READ_FIELD(HAL_WBL_ATADILCKS) != 0) &&
                                    (READ_FIELD(HAL_WBL_ATSYILCKS) != 0);
            break;

        case eDiscDVD:
            AddressWordSyncInLock = (READ_FIELD(HAL_WBL_ADDRILCKS) != 0) &&
                                    (READ_FIELD(HAL_WBL_WDSYILCKS) != 0) &&
                                    (READ_FIELD(HAL_WBL_BTSYILCKS) != 0);
            break;

        case eDiscLS:
            break;

        default:
            AddressWordSyncInLock = (READ_FIELD(HAL_WBL_ADDRILCKS) != 0) &&
                                    (READ_FIELD(HAL_WBL_WDSYILCKS) != 0) &&
                                    (READ_FIELD(HAL_WBL_BTSYILCKS) != 0);
            break;
    }

    return AddressWordSyncInLock;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_IsWblAddReady()
    Description:    checks wobble address ready
    Inputs:
    Outputs:        BOOL TRUE = ready, FALSE = not ready
    Global Inputs:  HAL_WBL_ATADBFRDY,
                    HAL_WBL_WBADBFRDY,
                    mDiscKind
    Global Outputs:
    Returns:        BOOL IsReady
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_IsWblAddReady(void)
{
    BOOL IsReady = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return IsReady;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            IsReady = (READ_FIELD(HAL_WBL_ATADBFRDY) != 0);
            break;

        case eDiscLS:
            break;

        default:
            IsReady = (READ_FIELD(HAL_WBL_WBADBFRDY) != 0);
            break;
    }

    return IsReady;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAtipAdipIntrAdd()
    Description:    Sets ADIP and ATIP interrupt address by reading registers or fields.
                    Is this correct???
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_ATADRCH_REGISTER,      CD
                    HAL_WBL_ADADRCH_REGISTER,      DVD +
                    HAL_WBL_LPADRCH_REGISTER,      DVD -
                    HAL_WBL_BDADRCH_REGISTER,      BD
                    HAL_WBL_ZONRCH,                HD (field)
                    HAL_WBL_TRKRCH,                HD (field)
                    HAL_WBL_SEGRCH,                HD (field)
                    HAL_WBL_PSBRCH,                HD (field)
                    HAL_WBL_SGORCH,                HD (field)
                    mDiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetAtipAdipIntrAdd(void)      //STILL TODO
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.fDiscKind)
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
        case eLSDisc:
            WRITE_REG(HAL_WBLREG_MAP->ATADRCFG, 0); //STILL TODO ???
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            (void)READ_REG(HAL_WBLREG_MAP->BDADRCFG);
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
    #endif
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDROMDLDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDLDisc:
    #endif
        case eHDRDLDisc:
        case eHDRWDLDisc:
            (void)READ_FIELD(HAL_WBL_ZONRCH);
            (void)READ_FIELD(HAL_WBL_TRKRCH);
            (void)READ_FIELD(HAL_WBL_SEGRCH);
            (void)READ_FIELD(HAL_WBL_PSBRCH);
            (void)READ_FIELD(HAL_WBL_SGORCH);
            break;
#endif /* (ENABLE_HDDVD == 1) */

        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            (void)READ_REG(HAL_WBLREG_MAP->ADADRCFG);
            break;

        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
            (void)READ_REG(HAL_WBLREG_MAP->LPADRCFG);
            break;

        default:
            break;
    }
}
#endif //currently not be use

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
                    mDiscKind
    Global Outputs:
    Returns:        OverspeedX100
-----------------------------------------------------------------------------------------------------*/
USHORT HAL_WOBBLE_GetOverspeed(ULONG FWobbleADCClkKHz)
{
    ULONG MeasWblFrq_factor;
    ULONG OverspeedX100 = 100;

#if EN_POWER_CONTROL
    if (WobbleCLKStop)
    {
        return (USHORT)OverspeedX100;
    }
#endif

    MeasWblFrq_factor = HAL_WOBBLE_GetPLLFreq();
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
        case eDiscLS:
            //CD_WBL_FREQ_N1= 22050,TWO_TO_POWER_18 = 262144
            //(TWO_TO_POWER_18*CD_WBL_FREQ_N1) will overflow
            //Use CD Max 54.0X to calculate: MeasWblFrq_factor*FWobbleADCClkKHz will not overflow
            //But (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10) will overflow
            //To prevent truncation and overflow:
            //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(TWO_TO_POWER_18*CD_WBL_FREQ_N1/100000);
            OverspeedX100 = (ULONG)MeasWblFrq_factor * FWobbleADCClkKHz * 8 / (TWO_TO_POWER_18 * (CD_WBL_FREQ_N1 / 50) / 250);
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            //BD_WBL_FREQ_N1=956522,TWO_TO_POWER_15=32768
            //(BD_WBL_FREQ_N1*TWO_TO_POWER_15) will overflow
            //Use BD Max 15.3X to calculate:(MeasWblFrq_factor*FWobbleADCClkKHz) will overflow
            //OverspeedX10 = (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10)(unit:Hz)/(TWO_TO_POWER_15*BD_WBL_FREQ_N1*23)(unit:Hz);
            //To prevent truncation and overflow:
            //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(23*TWO_TO_POWER_15*BD_WBL_FREQ_N1/100000);
            OverspeedX100 = (ULONG)(MeasWblFrq_factor / 8) * FWobbleADCClkKHz / ((ULONG)23 * (TWO_TO_POWER_15 / 256) * BD_WBL_FREQ_N1 / 3125);
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            //HDDVD_WBL_FREQ_N1=696774,TWO_TO_POWER_15=32768
            //(HDDVD_WBL_FREQ_N1*TWO_TO_POWER_15) will overflow
            //Use HD Max 12.2X to calculate:(MeasWblFrq_factor*FWobbleADCClkKHz)
            //OverspeedX10 = (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10)(unit:Hz)/(TWO_TO_POWER_15*HDDVD_WBL_FREQ_N1*31)(unit:Hz);
            //To prevent truncation and overflow:
            //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(31*TWO_TO_POWER_15*HDDVD_WBL_FREQ_N1/100000);
            OverspeedX100 = (ULONG)(MeasWblFrq_factor / 8) * FWobbleADCClkKHz / ((ULONG)31 * (TWO_TO_POWER_15 / 256) * HDDVD_WBL_FREQ_N1 / 3125);
            break;
#endif /* (ENABLE_HDDVD == 1) */

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                //DVDRAM_WBL_FREQ_N1=156882,TWO_TO_POWER_15=32768
                //(DVDRAM_WBL_FREQ_N1*TWO_TO_POWER_15) will overflow
                //Use DVD Max 20X to calculate:(MeasWblFrq_factor*FWobbleADCClkKHz) will overflow
                //OverspeedX10 = (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10)(unit:Hz)/(TWO_TO_POWER_15*DVDRAM_WBL_FREQ_N1*93)(unit:Hz);
                //To prevent truncation and overflow:
                //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(93*TWO_TO_POWER_15*DVDRAM_WBL_FREQ_N1/100000);
                OverspeedX100 = (ULONG)(MeasWblFrq_factor / 4) * FWobbleADCClkKHz / ((ULONG)93 * (TWO_TO_POWER_15 / 128) * DVDRAM_WBL_FREQ_N1 / 3125);
            }
            else
#endif
            if (mDiscKind.Bit.WblType == eWblPlus)
            {
                //DVD_PR_PRW_WBL_FREQ_N1=817383,TWO_TO_POWER_15=32768
                //(DVD_PR_PRW_WBL_FREQ_N1*TWO_TO_POWER_15) will overflow
                //Use DVD Max 20X to calculate:MeasWblFrq_factor*FWobbleADCClkKHz will overflow
                //OverspeedX10 = (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10)(unit:Hz)/(TWO_TO_POWER_15*DVD_PR_PRW_WBL_FREQ_N1*16)(unit:Hz);
                //To prevent truncation and overflow:
                //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(16*TWO_TO_POWER_15*DVD_PR_PRW_WBL_FREQ_N1/100000);
                OverspeedX100 = (ULONG)(MeasWblFrq_factor / 4) * FWobbleADCClkKHz / ((ULONG)16 * (TWO_TO_POWER_15 / 128) * DVD_PR_PRW_WBL_FREQ_N1 / 3125);
            }
            else
            {
                //DVD_MR_MRW_WBL_FREQ_N1=140625,TWO_TO_POWER_15=32768
                //(DVD_MR_MRW_WBL_FREQ_N1*TWO_TO_POWER_15) will overflow
                //Use DVD Max 20X to calculate:(MeasWblFrq_factor*FWobbleADCClkKHz) will overflow
                //OverspeedX10 = (MeasWblFrq_factor*FWobbleADCClkKHz*1000*10)(unit:Hz)/(TWO_TO_POWER_15*DVD_MR_MRW_WBL_FREQ_N1*93)(unit:Hz);
                //To prevent truncation and overflow:
                //OverspeedX100 = (MeasWblFrq_factor*FWobbleADCClkKHz)/(93*TWO_TO_POWER_15*DVD_MR_MRW_WBL_FREQ_N1/100000);
                OverspeedX100 = (ULONG)(MeasWblFrq_factor / 4) * FWobbleADCClkKHz / ((ULONG)93 * (TWO_TO_POWER_15 / 128) * DVD_MR_MRW_WBL_FREQ_N1 / 3125);
            }
            break;

        default:
            //send_msg5S(SHOW_DEBUG_MSG,"WblSpd Err");
            send_msg80(SHOW_DEBUG_MSG,0x710805); //Wobble Speed is Error
            break;
    }

    return (USHORT)OverspeedX100;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetPLLFreq()
    Description:    gets pll frequency
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_ATPLLFRQ,     CD
                    HAL_WBL_WBLFREQ,      OTHER
                    mDiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
USHORT HAL_WOBBLE_GetPLLFreq(void)
{
    USHORT PLLFreq = 0;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return PLLFreq;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            PLLFreq = ((USHORT)READ_FIELD(HAL_WBL_ATPLLFRQ));
            break;

        case eDiscLS:
            break;

        default:
            PLLFreq = ((USHORT)READ_FIELD(HAL_WBL_WBLFREQ));
            break;
    }

    return(PLLFreq);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetNewAddIntEnable()
    Description:    Returns wether the new wobble address interrupt is enabled
    Inputs:
    Outputs:
    Global Inputs:  HAL_WBL_ATNEWADE,     CD
                    HAL_WBL_NEWADDRE,     OTHER
                    mDiscKind
    Global Outputs:
    Returns:        BOOL
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetNewAddIntEnable(void)
{
    BOOL IntEnable = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return IntEnable;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            IntEnable = (READ_FIELD(HAL_WBL_ATNEWADE) != 0);
            break;

        case eDiscLS:
            break;

        default:
            IntEnable = (READ_FIELD(HAL_WBL_NEWADDRE) != 0);
            break;
    }

    return(IntEnable);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_Init

    Description:    Initalization of the WOBBLE block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_Init( void )
{
}

#if   0 //currently not be use
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetConfLevBs

    Description:    This function sets confidence level for bit sync detection
    Inputs:         MaxConfidence:  maximum confidence level for the flywheel
                    HiThresLa:      higher threshold level for the flywheel, the resulting lock flag is
                                    used to switch on/off wobble PLL lock aid
                    HiThres:        higher threshold level for the flywheel
                    LoThres:        lower threshold level for the flywheel
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetConfLevBs(BYTE MaxConfidence, BYTE HiThresLa, BYTE HiThres, BYTE LoThres)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.fDiscKind)
    {
        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            WRITE_FIELD( HAL_WBL_ADBTMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_ADBTHITLA, HiThresLa);
            WRITE_FIELD( HAL_WBL_ADBTHITHR, HiThres);
            WRITE_FIELD( HAL_WBL_ADBTLOTHR, LoThres);

            //the maximum likelyhood detection is enabled
            WRITE_FIELD( HAL_WBL_ADBSYDET, e_ADBSYDET_ACTIVATE);
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDROMDLDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            WRITE_FIELD( HAL_WBL_BDBTMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_BDBTHITLA, HiThresLa);
            WRITE_FIELD( HAL_WBL_BDBTHITHR, HiThres);
            WRITE_FIELD( HAL_WBL_BDBTLOTHR, LoThres);
            break;
#endif

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetConfLevWs

    Description:    This function sets confidence level for word sync detection
    Inputs:         MaxConfidence:  maximum confidence level for the flywheel
                    HiThres:        higher threshold level for the flywheel
                    LoThres:        lower threshold level for the flywheel
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetConfLevWs(BYTE MaxConfidence, BYTE HiThres, BYTE LoThres)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.fDiscKind)
    {
        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            WRITE_FIELD( HAL_WBL_ADWDMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_ADWDHITHR, HiThres);
            WRITE_FIELD( HAL_WBL_ADWDLOTHR, LoThres);

            //When set to '1', the DVD+R/RW error correction is activated
            WRITE_FIELD( HAL_WBL_ADERCOON, e_ADERCOON_ACTIVATE);
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDROMDLDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            WRITE_FIELD( HAL_WBL_BDWDMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_BDWDHITHR, HiThres);
            WRITE_FIELD( HAL_WBL_BDWDLOTHR, LoThres);

            //Activates the freeze flywheel mechanism for wobblebeat protection
            WRITE_FIELD( HAL_WBL_FRZFLYEN, e_FRZFLYEN_ACTIVATE);
            break;
#endif // (BD_ENABLE == 1)

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetConfLevId

    Description:    This function sets confidence level for address detection
    Inputs:         MaxConfidence:  maximum confidence level for the flywheel
                    HiThres:        higher threshold level for the flywheel
                    LoThres:        lower threshold level for the flywheel
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetConfLevId(BYTE MaxConfidence, BYTE HiThres, BYTE LoThres)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.fDiscKind)
    {
        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            WRITE_FIELD( HAL_WBL_ADADMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_ADADHITHR, HiThres);
            WRITE_FIELD( HAL_WBL_ADADLOTHR, LoThres);
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDROMDLDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            WRITE_FIELD( HAL_WBL_BDADMXCFD, MaxConfidence);
            WRITE_FIELD( HAL_WBL_BDADRHITH, HiThres);
            WRITE_FIELD( HAL_WBL_BDADRLOTH, LoThres);
            break;
#endif // (BD_ENABLE == 1)

        default:
            break;
    }
}
#endif //currently not be use

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_EnableFlywheels

    Description:    This function enable or disable the flywheels
    Inputs:         EnableFlywheel: Is true if the flywheel should be enable, is false otherwise.
    Outputs:        None
    Global Inputs:  mDiscKind
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (DVD_RAM_READ == 1)
SBYTE HeaderFlyWheelConfidence;
#endif
void HAL_WOBBLE_EnableFlywheels(Bool EnableFlywheel)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    if (EnableFlywheel)
    {
        switch (mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
                //This bit has to be written back to '0' to resume operation
                WRITE_FIELD(HAL_WBL_ATRSTFLY, e_RSTFLY_RESUME_OPER);
                break;

            case eDiscLS:
                break;

            default:
                //This bit has to be written back to '0' to resume operation
                WRITE_FIELD(HAL_WBL_RSTFLY, e_RSTFLY_RESUME_OPER);
                break;
        }
    }
    else
    {
        switch (mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
                //When set to '1', the ATIP flywheel on the ATIP block address and the ATIP flywheel are reset
                WRITE_FIELD(HAL_WBL_ATRSTFLY, e_RSTFLY_RESET);
                break;

            case eDiscLS:
                break;

            default:
                //When set to '1', all the wobble address flywheels for the formats DVD+/-, BD, HDDVD RAM/RW, DVDRAM are reset
                WRITE_FIELD(HAL_WBL_RSTFLY, e_RSTFLY_RESET);
#if (DVD_RAM_READ == 1)
                HeaderFlyWheelConfidence = 15;
#endif
                break;
        }
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetHPFInp()

    Description:    Sets the High Pass Filter at the input of the wobble circuit,
                    the High Pass Filter for DVD Minus LPP bit detection and also
                    the High Pass Filter for the DVD-RAM timing engine
    Inputs:         PllInpHighPassFcN1Hz -> cut off frequency of the input PLL HPF for overspeed 1X in Hz units
                    AgcInpHighPassFcN1Hz -> cut off frequency of the input AGC HPF for overspeed 1X in Hz units
                    LPP_DRHighPassFcN1Hz -> cut off frequency of the HPF for DVD minus LPP bit detection / DVD-RAM
                    timing engine for overspeed 1X in Hz units
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    ADCWblClkKHz      -> clock frequency of wobble module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     340000 KKz
                                         HDDVD  240000 KHz

    Outputs:
    Global Inputs:  mDiscKind
    Global Outputs: HAL_WBL_ATIHPFCFG
                    or
                    HAL_WBL_INHPFCFG,
                    HAL_WBL_LPIHPFCFG
                    HAL_WBL_DRIHPFCFG
                    field(s), depending on the disc kind
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
static USHORT CdHpfTableX10[SIZE_CD_HPF_TABLE] = CD_HPF_TABLEX10;
static USHORT NotCdHpfTable[SIZE_NOT_CD_HPF_TABLE] = NOT_CD_HPF_TABLE;
void HAL_WOBBLE_SetHPFInp(ULONG PllInpHighPassFcN1Hz, ULONG AgcInpHighPassFcN1Hz, ULONG LPP_DRHighPassFcN1Hz, USHORT OverspeedX10, ULONG ADCWblClkKHz)
{
    ULONG FcKHz;
    ULONG HpfBwKHz;
    SBYTE index;

#if (EN_POWER_CONTROL == 1)
    if (WobbleCLKStop)
    {
        return;
    }
#endif

    FcKHz = (PllInpHighPassFcN1Hz * OverspeedX10) / 1000 / 10;
    switch (mDiscKind.Bit.DiscStd) /* Disc standard specific calculations*/
    {
        case eDiscCD:
            // set the High Pass Filter at the input of the wobble circuit
            HpfBwKHz = FcKHz * (10 * ADC_WBL_CLK_KHZ_REF) / ADCWblClkKHz;
            for (index = (SIZE_CD_HPF_TABLE-1); index > 0; index--)
            {
                if (HpfBwKHz > CdHpfTableX10[index])
                {
                    break;
                }
            }
            WRITE_FIELD(HAL_WBL_ATIHPFCFG, index);

            //andy c temp solution
            if (OverspeedX10 <= 80){
                //WRITE_FIELD(HAL_WBL_ATIHPFCFG, 0);      //andy c add for A0 DFE HP
                WRITE_FIELD(HAL_AFE_WOBLPFBW, 1);   //set wob lpf before ADC Buf        //HAL_AFE_SetWobbleSpeed
                WRITE_FIELD(HAL_AFE_WOBLPFEN, 1);   //enable wob lpf before ADC Buf     //HAL_AFE_SetWobbleSpeed
                WRITE_FIELD(HAL_AFE_AGCHPFDF, 0);  //AGC high pass 1=1.41Khz, 0=5.68KHz //HAL_AFE_SetWobbleSpeed
            }
            break;

        case eDiscDVD:
#if (BD_ENABLE == 1)
        case eDiscBD:
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
#endif
            // set the PLL High Pass Filter at the input of the wobble circuit
            HpfBwKHz = (FcKHz << 18) / ADCWblClkKHz;
            for (index = SIZE_NOT_CD_HPF_TABLE-1; index > 0; index--)
            {
                if (HpfBwKHz > NotCdHpfTable[index])
                {
                    break;
                }
            }
            WRITE_FIELD(HAL_WBL_INHPFCFG, index);
            break;

       default:
            return;
    }

    switch (mDiscKind.fDiscKind)
    {
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
#endif
            FcKHz = (LPP_DRHighPassFcN1Hz * OverspeedX10) / 1000 / 10;
            HpfBwKHz = (FcKHz << 18) / ADCWblClkKHz;
            for (index = SIZE_NOT_CD_HPF_TABLE-1; index > 0; index--)
            {
                if (HpfBwKHz > NotCdHpfTable[index])
                {
                    break;
                }
            }

#if (DVD_RAM_READ == 1)
            if (mDiscKind.fDiscKind == eDVDRAMDisc)
            {
                // Set the High Pass Filter for DVD-RAM timing engine.
                WRITE_FIELD(HAL_WBL_DRIHPFCFG, index);
            }
            else
#endif
            {
                // Set the High Pass Filter for DVD Minus LPP bit detection.
                WRITE_FIELD(HAL_WBL_LPIHPFCFG, index);
            }
            break;

        case eCDRDisc:
        case eCDRWDisc:
            break;

        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            break;

#if (BD_ENABLE == 1)
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDRDLDisc:
        case eHDRWDLDisc:
            break;
#endif

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
            return;

    }

    // set the AGC High Pass Filter at the input of the wobble circuit for all disc types
    FcKHz = (AgcInpHighPassFcN1Hz * OverspeedX10) / 1000 / 10;
    HpfBwKHz = (FcKHz << 18) / ADCWblClkKHz;
    for (index = SIZE_NOT_CD_HPF_TABLE-1; index > 0; index--)
    {
        if (HpfBwKHz > NotCdHpfTable[index])
        {
            break;
        }
    }
    WRITE_FIELD(HAL_WBL_INHPFCFGA, index);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetLPFInp()

    Description:    Sets the Low Pass Filter at the input of the wobble circuit,
                    the Low Pass Filter for DVD Minus LPP bit detection and also
                    the Low Pass Filter for the DVD-RAM timing engine
    Inputs:         PllInpLowPassFcN1Hz -> cut off frequency of the input PLL LPF for overspeed 1X in Hz units
                    AgcInpLowPassFcN1Hz -> cut off frequency of the input AGC LPF for overspeed 1X in Hz units
                    LPPLowPassFcN1Hz -> cut off frequency of the LPF for DVD minus LPP bit detection for overspeed 1X in Hz units
                    OverspeedX10      -> overspeed in tenths
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
                    ADCWblClkKHz      -> clock frequency of wobble module in KHz units
                                         DVD     60000 KHz
                                         BD     340000 KKz
                                         HDDVD  240000 KHz

    Outputs:
    Global Inputs:  mDiscKind
    Global Outputs: HAL_WBL_INLPFCFG,
                    HAL_WBL_LPILPFCFG
                    HAL_WBL_DRILPFCFG
                    field(s), depending on the disc kind
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
static USHORT LpfTable[SIZE_LPF_TABLE] = LPF_TABLE;
void HAL_WOBBLE_SetLPFInp(ULONG PllInpLowPassFcN1Hz, ULONG AgcInpLowPassFcN1Hz, ULONG LPP_DRLowPassFcN1Hz, USHORT OverspeedX10, ULONG ADCWblClkKHz)
{
    ULONG FcKHz;
    ULONG LpfBwKHz;
    SBYTE  index;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    switch (mDiscKind.Bit.DiscStd) /* calculate disc standard specific LPF settings */
    {
        case eDiscDVD:
#if (BD_ENABLE == 1)
        case eDiscBD:
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
#endif
            // set the PLL Low Pass Filter at the input of the wobble circuit for all DVD disc kinds
            FcKHz = (PllInpLowPassFcN1Hz * OverspeedX10) / 1000 / 10;
            LpfBwKHz = (FcKHz << 14) / ADCWblClkKHz;
            for (index=0; index < SIZE_LPF_TABLE; index++)
            {
                if (LpfBwKHz <= LpfTable[index])
                {
                    break;
                }
            }
            index++;
            if (index > 6)
                index = 6;
            WRITE_FIELD(HAL_WBL_INLPFCFG, index);

            break;

        case eDiscCD:
            break;

        default:
            return;
    }

    switch (mDiscKind.fDiscKind) /* Calculate disc kind dependant LPF settings */
    {
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
#endif
            FcKHz = (LPP_DRLowPassFcN1Hz * OverspeedX10) / 1000 / 10;
            LpfBwKHz = (FcKHz << 14) / ADCWblClkKHz;
            for (index=0; index < SIZE_LPF_TABLE; index++)
            {
                if (LpfBwKHz <= LpfTable[index])
                {
                    break;
                }
            }

#if (DVD_RAM_READ == 1)
            if (mDiscKind.fDiscKind == eDVDRAMDisc)
            {
                // Set the Low Pass Filter for DVD RAM timing engine.
                WRITE_FIELD(HAL_WBL_DRILPFCFG, ++index);
            }
            else
#endif
            {
                // Set the Low Pass Filter for DVD Minus LPP bit detection.
                WRITE_FIELD(HAL_WBL_LPILPFCFG, ++index);
            }
            break;

        case eCDRDisc:
        case eCDRWDisc:
            break;

        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            break;

#if (BD_ENABLE == 1)
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDRDLDisc:
        case eHDRWDLDisc:
            break;
#endif

        default:
            //DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND ); //not need this information
            return;

    }

    // set the PLL Low Pass Filter at the input of the wobble circuit (all discs)
    FcKHz = (AgcInpLowPassFcN1Hz * OverspeedX10) / 1000 / 10;
    LpfBwKHz = (FcKHz << 14) / ADCWblClkKHz;
    for (index=0; index < SIZE_LPF_TABLE; index++)
    {
        if (LpfBwKHz <= LpfTable[index])
        {
             break;
        }
    }
    index++;
    if (index > 6)
        index = 6;
    WRITE_FIELD(HAL_WBL_INLPFCFGA, index);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetPllInLock()
    Description:    returns whether Wobble PLL is in lock
    Inputs:         WblClkKHz: the Wobble clock in kHz
    Outputs:        Is true when the Wobble PLL is in lock, is false otherwise
    Global Inputs:  svoSpeedVar.SpdOverspd
	                HAL_WBL_ATPLLFRQ or HAL_WBL_PLLLCKS field depending on the Disc Type
    Global Outputs:
    Returns:        True when the Wobble PLL is in lock, false otherwise
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetPllInLock(ULONG WblClkKHz)
{
    ULONG ExpectedFreq,OverspeedX100;
    ULONG MeasuredFreq;
    BOOL Inlock = FALSE;

#if (EN_POWER_CONTROL == 1)
    if (WobbleCLKStop) return Inlock;
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            // no status bit available
            // only check the frequency if it is in the expected frequency plus /minus 10%
            if (svoSpeedVar.SpdMonitor)
            {
                OverspeedX100 = spdGetOverSpdX100(spdGetCurrentRPM(),Iram_stp_CurrentPosition);
            }
            else
            {
                OverspeedX100 = svoSpeedVar.SpdOverspd;
            }
            ExpectedFreq = 57803 * OverspeedX100 / WblClkKHz;
            MeasuredFreq = (ULONG)READ_FIELD(HAL_WBL_ATPLLFRQ);
            Inlock = (BOOL)(((MeasuredFreq * 2) <= (ExpectedFreq * 5)) && ((MeasuredFreq * 5) >= (ExpectedFreq * 2)));
            break;

        case eDiscLS:
            break;

        default:
            Inlock = (BOOL)(READ_FIELD(HAL_WBL_PLLLCKS) != 0);
            break;
    }

    return(Inlock);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetPllLockAidStatus()
    Description:    returns whether Wobble PLL lock aid status
    Inputs:
    Outputs:        Is true when the Wobble PLL lock aid is enable, is false disable
    Global Inputs:
    Global Outputs:
    Returns:        True when the Wobble PLL lock aid is enable, false otherwise
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetPllLockAidStatus(void)
{
    BOOL Lockaid_Status = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return Lockaid_Status;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            break;

        case eDiscLS:
            break;

        default:
            Lockaid_Status = (BOOL)(READ_FIELD(HAL_WBL_PLLLKAS) == TRUE);
            break;
    }

    return(Lockaid_Status);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetSyncFlyWheelInLock()
    Description:    returns whether word Sync Flywheel and bit Sync Flywheel were in lock
    Inputs:
    Outputs:        Is true when the word Sync and bit Sync Flywheel were in lock, is false otherwise
    Global Inputs:
    Global Outputs: HAL_WBL_ATSYLCK, or HAL_WBL_LPWDLCK, HAL_WBL_LPBTLCK, or HAL_WBL_ADWDLCK
                    HAL_WBL_ADBTLCK, or HAL_WBL_BDWDLCK, HAL_WBL_BDBTLCK, or HAL_WBL_HDWDUSYLK field(s)
                    depending on the Disc Type
    Returns:        True when the the word Sync and bit Sync Flywheel were in lock, false otherwise
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetSyncFlyWheelInLock(void)
{
    BOOL InLock = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return InLock;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            InLock = READ_FIELD(HAL_WBL_ATSYLCK);
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                SBYTE confidence;

                // Check if confidence is increasing. If yes, header flywheel has been locked;
                // otherwise, header flywheel has not been locked yet.
                confidence = READ_FIELD(HAL_WBL_HDRFLYCFD);
                InLock = ((confidence>HeaderFlyWheelConfidence)||(confidence==15))/* && (READ_FIELD(HAL_WBL_AVEHD)==231)*/;
                HeaderFlyWheelConfidence = confidence;
            }
            else
#endif
            {
                if(mDiscKind.Bit.WblType == eWblMinus)
                {
                    InLock = ((READ_REG(HAL_WBLREG_MAP->ADRFLYS) & (HAL_WBL_LPWDLCK_MASK | HAL_WBL_LPBTLCK_MASK))
                              == (HAL_WBL_LPWDLCK | HAL_WBL_LPBTLCK));
                }
                else
                {
                    InLock = ((READ_REG(HAL_WBLREG_MAP->ADRFLYS) & (HAL_WBL_ADWDLCK_MASK | HAL_WBL_ADBTLCK_MASK))
                              == (HAL_WBL_ADWDLCK | HAL_WBL_ADBTLCK));
                }
            }
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            InLock = ((READ_REG(HAL_WBLREG_MAP->ADRFLYS) & (HAL_WBL_BDWDLCK_MASK | HAL_WBL_BDBTLCK_MASK))
                      == (HAL_WBL_BDWDLCK | HAL_WBL_BDBTLCK));
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            InLock = READ_FIELD(HAL_WBL_HDWDUSYLK);
            break;
#endif

        default:
            break;
    }

    return (InLock);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_EnableNewWblAddInterrupt()
    Description:    Enables or disables the New Wobble Adress Interrupt
    Inputs:         OnOff: If true, the New Wobble Adress Interrupt should be enabled, otherwise
                    the New Wobble Adress Interrupt should be disabled
    Outputs:
    Global Inputs:
    Global Outputs: NEWADDRE or ATNEWADE depending on the Disc Type
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_EnableNewWblAddInterrupt(BOOL OnOff)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

#if (DVD_RAM_READ == 1)
    if ((!DVDRAM)&&(OnOff))  // Never turn on NEWADDRE for DVD-RAM.
#else
    if (OnOff)
#endif
    {   switch (mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
                WRITE_FIELD(HAL_WBL_ATNEWADE,OnOff);
                CLEAR_INT_FIELD(HAL_WBL_NEWADDRI, 0);
                WRITE_FIELD(HAL_WBL_NEWADDRE,!OnOff);
                break;
            case eDiscLS:
                break;
            default:
                CLEAR_INT_FIELD(HAL_WBL_ATNEWADI, 0);
                WRITE_FIELD(HAL_WBL_ATNEWADE,!OnOff);
                WRITE_FIELD(HAL_WBL_NEWADDRE,OnOff);
                break;
        }
    }
    else
    {
        CLEAR_INT_FIELD(HAL_WBL_ATNEWADI, 0);
        WRITE_FIELD(HAL_WBL_ATNEWADE,OnOff);
        CLEAR_INT_FIELD(HAL_WBL_NEWADDRI, 0);
        WRITE_FIELD(HAL_WBL_NEWADDRE,OnOff);
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetLPFAmpMeas()

    Description:    Sets LPF for AGC amplitude measurement
    Inputs:         eAMP_LPF_BW
    Outputs:
    Global Inputs:  mDiscKind?
    Global Outputs: HAL_WBL_LPLPFBW field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetLPFAmpMeas(halWBL_AmplitudeLPFBW_t AmpLPFBw)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    WRITE_FIELD(HAL_WBL_AMPLPFBW, AmpLPFBw);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetAGCOnOff()

    Description:    Sets Wobble AGC loop on or off
    Inputs:         OnOff {TRUE|FALSE}
                        TRUE  = AGC loop on
                        FALSE = AGC loop off
    Outputs:
    Global Inputs:  mDiscKind?
    Global Outputs: HAL_WBL_WBLAGCON field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetAGCOnOff(halWBL_AgcEnable_t OnOff)
{
#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
#endif

    WRITE_FIELD(HAL_WBL_WBLAGCON, OnOff);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetValidFlyWheelAddress()

    Description:    Sets ValidFlyWheelAddress to True or False
    Inputs:         Valid {TRUE|FALSE}
                        TRUE  =  FlyWheel Address Valid
                        FALSE =  FlyWheel Address NOT Valid
    Outputs:
    Global Inputs:
    Global Outputs: mValidFlyWheelAddress
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetValidFlyWheelAddress(BOOL Valid)
{
    mValidFlyWheelAddress = Valid;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetValidFlyWheelAddress()

    Description:    Returns whether ValidFlyWheelAddress is True or False
    Inputs:
    Outputs:        TRUE|FALSE
                        TRUE  =  FlyWheel Address Valid
                        FALSE =  FlyWheel Address NOT Valid
    Global Inputs:  mValidFlyWheelAddress
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetValidFlyWheelAddress(void) // TBD? __inline
{
    return(mValidFlyWheelAddress);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_PllHold()

    Description:    Sets Wobble PLL on hold or out of hold
    Inputs:         WblPllHold
    Outputs:
    Global Inputs:  mDiscKind
    Global Outputs: HAL_WBL_ATPLLHLD field in case of CD, HAL_WBL_PRESETEN field otherwise
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetPllOnHold(halWBL_WblPllHold_t WblPllHold)
{
#if EN_POWER_CONTROL
    if (WobbleCLKStop) return;
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            WRITE_FIELD(HAL_WBL_ATPLLHLD, WblPllHold);
            break;

        case eDiscLS:
            break;

        default:
            if (DVDRAM)
                WRITE_FIELD(HAL_WBL_WBLFREQ,READ_FIELD(HAL_WBL_WBLFREQ));
            WRITE_FIELD(HAL_WBL_PRESETEN, WblPllHold);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_GetPllOnHold()

    Description:    Returns whether Wobble PLL is on hold or out of hold
    Inputs:         None
    Outputs:        Is TRUE when the Wobble PLL is on hold
    Global Inputs:  mDiscKind
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_WOBBLE_GetPllOnHold(void)
{
    BOOL Onhold = FALSE;

#if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return Onhold;
    }
#endif

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            Onhold = (READ_FIELD(HAL_WBL_ATPLLHLD) == halWBL_WblPllInHold);
            break;

        case eDiscLS:
            break;

        default:
            Onhold = (READ_FIELD(HAL_WBL_PRESETEN) == halWBL_WblPllInHold);
            break;
    }

    return(Onhold);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_CalibrateDiffWobbleGain()

    Description:    Calibration of the diff wobble gain
    Inputs:         USHORT TargetAdcmV : Target ADC value in mV unit
                    BYTE* Table_Index  : Index of the current setting in the Wobble Summing Amplifier Gain table
    Outputs:        BYTE* Table_Index
    Global Inputs:  WBL_CALC_WdiffGainInit: Initial setting in the Wobble Summing Amplifier Gain table
    Global Outputs: HAL_AFE_WDIFFGAIN field
    Returns:        errcode_t
-----------------------------------------------------------------------------------------------------*/
errcode_t HAL_WOBBLE_CalibrateDiffWobbleGain(USHORT TargetAdcmV, BYTE* Table_Index)
{
    ad_dsp_t ResultCalReadmV;       /* Current average wobble gain in mV unit */
    SHORT   GainDiffCurr;           /* Current (signed) difference between target and measured Gains in 0.25dB unit */
    SHORT   GainDiffPrev;           /* Previous (signed) difference between target and measured Gains in 0.25dB unit */
    BYTE    index_prev;             /* Index of the previous setting in the Wobble Summing Amplifier Gain table */
#if (DVD_RAM_READ == 1)
    BYTE    AMPMSMOD_SAVED,AMPLPFBYP_SAVED,DRANAMOD_SAVED,DRANAPRD_SAVED;
#endif

#if (EN_POWER_CONTROL == 1)
    if (WobbleCLKStop)
    {
        return eERR_not_power_up;
    }
#endif

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3B0300); // >>> Wobble Diff Gain Cal. <<<

    /* Get TargetAdcmV */
#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        SendMsgCn(DEBUG_CALIB_ENTRYS_MSG,2,0x3B0320,A2B(TargetAdcmV)); // HDRAdcTarget(mV)
    }
    else
#endif
    {
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x3B0301,A2B(TargetAdcmV)); // WBLAdcTarget(mV)
    }

    /* check whether the target ADC is within boundaries  */
    if (TargetAdcmV == 0)
    {
        return eERR_wrong_target;
    }

    WRITE_FIELD(HAL_AFE_WDIFFGAIN,*Table_Index);

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        //Bypass LPF and inmedite decay (fastest attack too slow for header meassurement).
        AMPMSMOD_SAVED = READ_FIELD(HAL_WBL_AMPMSMOD);
        AMPLPFBYP_SAVED = READ_FIELD(HAL_WBL_AMPLPFBYP);
        DRANAMOD_SAVED = READ_FIELD(HAL_WBL_DRANAMOD);
        DRANAPRD_SAVED = READ_FIELD(HAL_WBL_DRANAPRD);
        WRITE_FIELD(HAL_WBL_AMPMSMOD,0x00);
        WRITE_FIELD(HAL_WBL_AMPLPFBYP,0x01);
        WRITE_FIELD(HAL_WBL_DRANAMOD,0x01);
        WRITE_FIELD(HAL_WBL_DRANAPRD,0x00);
    }
#endif

    /* Measure the current average wobble adc value */
    ResultCalReadmV = readADCmVPerRevs(ONE_REVOLUTION, TRUE, WBL_ADC);
    if (ResultCalReadmV.errcode == eNOERR)
    {
        GainDiffCurr = CalculateXdBFromRate(P10dB,ResultCalReadmV.p2p_mv, TargetAdcmV);
        GainDiffPrev = GainDiffCurr;
        index_prev   = *Table_Index;
        while (((GainDiffCurr > 0) && (GainDiffPrev > 0)) || ((GainDiffCurr < 0) && (GainDiffPrev < 0)))
        {
//            send_msgA1(DEBUG_AUTO_TUNING_MSG,0x5B0302,*Table_Index);                                   // WDIFFGAIN==>
//            send_msgA2(DEBUG_AUTO_TUNING_MSG,0x5B0303,ResultCalReadmV.p2p_mv);                         // HDRpp(mV)
//            send_msgA2(DEBUG_AUTO_TUNING_MSG,0x5B0304,ResultCalReadmV.top_mv-ResultCalReadmV.bot_mv);  // WBLpp(mV)
//            send_msg74(DEBUG_AUTO_TUNING_MSG,0x5B0305,(float)GainDiffCurr/10);                         // GainDiff(dB)
            SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x3B0318+(*Table_Index),B2B(ResultCalReadmV.top_mv-ResultCalReadmV.bot_mv),
                                                                       F4B((float)GainDiffCurr / 10));


            if (GainDiffCurr < -11)      // GainDiffCurr < -1.1 dB
            {
                if (*Table_Index < MAX_INDEX_GAIN_TABLE)
                {
                    index_prev = *Table_Index;
                    (*Table_Index)++;
                }
                else
                {
                    // WARNING: WDIFFGAIN boundary reached, %2.2f (dB) left
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x3B030D,F4B((float)GainDiffCurr / 10));
                    break;
                }
            }
            else if (GainDiffCurr > 11)  // GainDiffCurr > 1.1 dB
            {
                if (*Table_Index > 0)
                {
                    index_prev = *Table_Index;
                    (*Table_Index)--;
                }
                else
                {
                    // WARNING: WDIFFGAIN boundary reached, %2.2f (dB) left
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x3B030D,F4B((float)GainDiffCurr / 10));
                    break;
                }
            }
            else
                break;

            GainDiffPrev = GainDiffCurr;

            /* Try new setting for HAL_AFE_WDIFFGAIN. */
            WRITE_FIELD(HAL_AFE_WDIFFGAIN, *Table_Index);

            /* Measure the current average wobble adc value. */
            ResultCalReadmV = readADCmVPerRevs(ONE_REVOLUTION, TRUE, WBL_ADC);
            if (ResultCalReadmV.errcode != eNOERR)
            {
                break;
            }
            GainDiffCurr = CalculateXdBFromRate(P10dB,ResultCalReadmV.p2p_mv, TargetAdcmV);
        }
    }
    if (ResultCalReadmV.errcode == eNOERR)
    {
//        send_msgA1(DEBUG_AUTO_TUNING_MSG,0x5B0302,*Table_Index);                                   // WDIFFGAIN==>
//        send_msgA2(DEBUG_AUTO_TUNING_MSG,0x5B0303,ResultCalReadmV.p2p_mv);                         // HDRpp(mV)
//        send_msgA2(DEBUG_AUTO_TUNING_MSG,0x5B0304,ResultCalReadmV.top_mv-ResultCalReadmV.bot_mv);  // WBLpp(mV)
//        send_msg74(DEBUG_AUTO_TUNING_MSG,0x5B0305,(float)GainDiffCurr/10);                         // GainDiff(dB)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x3B0318+(*Table_Index),B2B(ResultCalReadmV.top_mv-ResultCalReadmV.bot_mv),
                                                                   F4B((float)GainDiffCurr / 10));

        if (ABS(GainDiffPrev) < ABS(GainDiffCurr))
        {
            *Table_Index = index_prev;
            WRITE_FIELD(HAL_AFE_WDIFFGAIN, *Table_Index);
        }

        SendMsg80(SHOW_DEBUG_MSG,0x3B0310+*Table_Index); // WDIFFGAIN = x (y dB)

#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            ResultCalReadmV = readADCmVPerRevs(ONE_REVOLUTION, TRUE, WBL_ADC);
            if ((ResultCalReadmV.errcode == eNOERR) && (ResultCalReadmV.p2p_mv != 0))
            {
                // Calculate WBLpp.
                ResultCalReadmV.top_mv -= ResultCalReadmV.bot_mv;

                // Calculate WBLpp Adc Target
                TargetAdcmV = (((LONG)TargetAdcmV)*((LONG)ResultCalReadmV.top_mv))/ResultCalReadmV.p2p_mv;
                TargetAdcmV = (TargetAdcmV * 14)/20;  // Scale with 0.70 to elimate the noise effect.
                SendMsgCn(SHOW_DEBUG_MSG,2,0x3B0301,A2B(TargetAdcmV)); // WBLAdcTarget(mV)

                // Calculate new delta
                index_prev  = ((((LONG)(READ_FIELD(HAL_WBL_WAGCTHRHI)-READ_FIELD(HAL_WBL_WAGCTHRLO)))*((LONG)ResultCalReadmV.top_mv))/ResultCalReadmV.p2p_mv)/2;

                // Calculate new threshold HI and LO.
                TargetAdcmV = TargetAdcmV * 256 / RF_ADC_IN_RANGE;
                GainDiffCurr = TargetAdcmV + index_prev;
                WRITE_FIELD(HAL_WBL_WAGCTHRHI,GainDiffCurr);
                SendMsgCn(SHOW_DEBUG_MSG,2,0x3B0306,A2B(GainDiffCurr)); // WAGCTHRHI

                GainDiffCurr = TargetAdcmV - index_prev;
                WRITE_FIELD(HAL_WBL_WAGCTHRLO,GainDiffCurr);
                SendMsgCn(SHOW_DEBUG_MSG,2,0x3B0307,A2B(GainDiffCurr)); // WAGCTHRLO
            }
        }
#endif
    }

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_WBL_AMPMSMOD,AMPMSMOD_SAVED);
        WRITE_FIELD(HAL_WBL_AMPLPFBYP,AMPLPFBYP_SAVED);
        WRITE_FIELD(HAL_WBL_DRANAMOD,DRANAMOD_SAVED);
        WRITE_FIELD(HAL_WBL_DRANAPRD,DRANAPRD_SAVED);
    }
#endif

    return (ResultCalReadmV.errcode);
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetHeaderDetectMode(BYTE mode, BYTE Hdrlen, BYTE HdrLenLG)

    Description:    Configures the header detection method for DVDRAM
    Inputs:         BYTE mode, BYTE HDRLEN, BYTE HdrLenLG: Header detection mode and the value for the HDRLEN/HDRLENLG fields
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetHeaderDetectMode(halWBLHdrDetMode_t mode, BYTE HdrLen, BYTE HdrLenLG)
{
    #if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
    #endif

    switch(mode)
    {
        case SlicedWobble:
            WRITE_FIELD(HAL_WBL_AVHDLPFOF, halWBL_AverageHeaderDistanceLPFOff);
            WRITE_FIELD(HAL_WBL_HDRLEN,    HdrLen);
            WRITE_FIELD(HAL_WBL_SELHDRLEN, halWBL_HdrLngthProgrammed);
            WRITE_FIELD(HAL_WBL_MLLSLCDET, halWBL_UseSlicerMethod);
            WRITE_FIELD(HAL_WBL_PLLINTCNT, halWBL_UseWblPll);
            WRITE_FIELD(HAL_WBL_PPRFIN,    halWBL_UsePPInput);
            WRITE_FIELD(HAL_WBL_HDRLENLG,  HdrLenLG);
            break;

        case MLH:
            WRITE_FIELD(HAL_WBL_AVHDLPFOF, halWBL_AverageHeaderDistanceLPFOff);
            WRITE_FIELD(HAL_WBL_HDRLEN,    HdrLen);
            WRITE_FIELD(HAL_WBL_MLLSLCDET, halWBL_UseMHLMethod);
            WRITE_FIELD(HAL_WBL_PLLINTCNT, halWBL_UseWblPll);
            WRITE_FIELD(HAL_WBL_PPRFIN,    halWBL_UsePPInput);
            break;

        case SlicedRF:
            WRITE_FIELD(HAL_WBL_AVHDLPFOF, halWBL_AverageHeaderDistanceLPFOff);
            WRITE_FIELD(HAL_WBL_HDRLEN,    HdrLen);
            WRITE_FIELD(HAL_WBL_SELHDRLEN, halWBL_HdrLngthProgrammed);
            WRITE_FIELD(HAL_WBL_MLLSLCDET, halWBL_UseSlicerMethod);
            WRITE_FIELD(HAL_WBL_PLLINTCNT, halWBL_UseInternalCnter);
            WRITE_FIELD(HAL_WBL_PPRFIN,    halWBL_UseRFInput);
            WRITE_FIELD(HAL_WBL_HDRLENLG,  HdrLenLG);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_WOBBLE_SetLandGrooveDetectMode(halWBLLGDetMode_t mode)

    Description:    Configures the header detection method for DVDRAM
    Inputs:         BYTE mode
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_WOBBLE_SetLandGrooveDetectMode(halWBLLGDetMode_t mode)
{
    #if EN_POWER_CONTROL
    if(WobbleCLKStop)
    {
        return;
    }
    #endif

    switch(mode)
    {
        case Normal:
            WRITE_FIELD(HAL_WBL_HDRFLYGRV, halWBL_LandAndGroove);
            WRITE_FIELD(HAL_WBL_LGEXTEND,  halWBL_DontHoldLGState);
            break;

        case Groove_only:
            WRITE_FIELD(HAL_WBL_HDRFLYGRV, halWBL_GrooveOnly);
            WRITE_FIELD(HAL_WBL_LGEXTEND,  halWBL_DontHoldLGState);
            break;

         case Extend_LG:
            WRITE_FIELD(HAL_WBL_HDRFLYGRV, halWBL_LandAndGroove);
            WRITE_FIELD(HAL_WBL_LGEXTEND,  halWBL_LengthenHdr);
            break;
    }
}
#endif // #if (DVD_RAM_READ == 1)


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
