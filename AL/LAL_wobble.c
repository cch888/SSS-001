/* ******************************************************************************************
*
*               (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                              All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*  LAL_wobble.c
*
*  Description:
*  This file contains functions for the LAL WOBBLE layers and includes
*  the following:
*
*
*
* Notes:
*
*
*   $Revision: 43 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/
#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\oem\oem_info.h"
#include ".\servo\svo.h"
#include ".\servo\SvoVar.H"
#include ".\al\LAL.h"
#include ".\al\LAL_wobble.h"
#include ".\AL\LAL_dfe.h"
#include ".\al\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\al\REG_wobble.h"
#include ".\al\HAL_wobble.h"
#include ".\AL\HAL_pll.h"
#include ".\AL\REG_servo.h"
#include ".\common\ADC.h"

/* Global definitions regarding COEF */

#if (DVD_RAM_READ == 1)
halWBLHdrDetMode_t preHeaderDetectMode;
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WOBBLE_SetHeaderDetectMode(halWBLHdrDetMode_t mode)

    Description:    Gets HDRLEN and HDRLENLG values from coef table for DVDRAM depending on selected
                    header detection mode

    Inputs:
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_WOBBLE_SetHeaderDetectMode(halWBLHdrDetMode_t mode)
{
    if(mode != preHeaderDetectMode)
    {
        preHeaderDetectMode = mode;

        switch(mode)
        {
            case SlicedWobble:
            case SlicedRF:
                HAL_WOBBLE_SetHeaderDetectMode(mode, COEF_GetValue(WBL_DRTIMCFG1_HDRLEN_SLCD), COEF_GetValue(WBL_DRTIMCFG2_HDRLENLG));
                break;

            case MLH:
                HAL_WOBBLE_SetHeaderDetectMode(mode, COEF_GetValue(WBL_DRTIMCFG1_HDRLEN_MLH), 0);
                break;
        }
    }
}
#endif /* #if (DVD_RAM_READ == 1) */

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WOBBLE_Init

    Description:    Initialise the appropriate WOBBLE registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: WOBBLE Registers listed in the INIT coef table
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_WOBBLE_Init(void)
{
    HAL_WOBBLE_Init();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           SetWobbleFreq()

    Description:    Sets the wobble frequency with default values
    Inputs:         OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
    Outputs:
    Global Inputs:  FreqFactor, LockAid, LimitPLL
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void SetWobbleFreq(USHORT OverspeedX10)
{
    halWBL_LimitPllT        LimitPLL;
    BYTE                    FreqFactor;
    SBYTE                   LockAid;

    FreqFactor = (BYTE)COEF_GetValue(WBL_CALC_coef_Offset_Freq);
    LockAid = (SBYTE) COEF_GetValue(WBL_CALC_coef_LOCK_AID_PH);
    LimitPLL.Upper = COEF_GetValue(WBL_CALC_coef_Upper_limit_PLL);
    LimitPLL.Lower = COEF_GetValue(WBL_CALC_coef_Lower_limit_PLL);
    HAL_WOBBLE_SetWobbleFreq(OverspeedX10, FreqFactor, LockAid, HAL_PLL_GetWblClkKHz(), &LimitPLL);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WOBBLE_SetWobbleFreq()

    Description:    Sets the wobble frequency with default values
    Inputs:         OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_WOBBLE_SetWobbleFreq(USHORT OverspeedX10)
{
    SetWobbleFreq(OverspeedX10);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WOBBLE_SetPLL()

    Description:    Sets bandwidth and integrator for wobble PLL
    Inputs:         low, normal, high

    Outputs:
    Global Inputs:  none
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_WOBBLE_SetPLL(eWBLPLLMode mode)
{
    halWBL_PLLSettingT      PLLSetting;

    switch (mode)
    {
        case eLowGainMode:
            PLLSetting.LoopKLPF = COEF_GetValue( WBL_CALC_LoopKLPF_Low );
            PLLSetting.LoopKP   = COEF_GetValue( WBL_CALC_LoopKP_Low );
            PLLSetting.LoopKI   = COEF_GetValue( WBL_CALC_LoopKI_Low );
            break;
        case eNormalGainMode:
            PLLSetting.LoopKLPF = COEF_GetValue( WBL_CALC_LoopKLPF_Normal );
            PLLSetting.LoopKP   = COEF_GetValue( WBL_CALC_LoopKP_Normal );
            PLLSetting.LoopKI   = COEF_GetValue( WBL_CALC_LoopKI_Normal );
            break;
        case eHighGainMode:
            PLLSetting.LoopKLPF = COEF_GetValue( WBL_CALC_LoopKLPF_High );
            PLLSetting.LoopKP   = COEF_GetValue( WBL_CALC_LoopKP_High );
            PLLSetting.LoopKI   = COEF_GetValue( WBL_CALC_LoopKI_High );
            break;
        default:
            break;
    }

    HAL_WOBBLE_SetPLL(&PLLSetting);
}

#if (BD_ENABLE == 1)
/* * * * * * *     P  I  C    * * * * * * * * */

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WOBBLE_SetResetPIC()
    Description:    Sets filters for wobble signal(s) available in the PIC Area or Resets filters for
                    wobble signal(s) available in data- or leadin/leadout Area. Normaly called when
                    entering and leaving PIC Area.
    Inputs:         SetMode {eSET_PIC | eRESET_PIC}
                    OverspeedX10 is the current overspeed value times 10.
    Outputs:        void
    Global Inputs:   (field)
                    wobble_DiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_WOBBLE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10)
{
    struct WblAGCSettings sWblAGCSettings;
    ULONG  ClkKHz;
    ULONG  PllInpFcN1Hz, AgcInpFcN1Hz;
    ULONG  LppFcN1Hz;

    if ((mDiscKind.Bit.DiscStd == eDiscBD) && ((mDiscKind.Bit.Media == eMediaR)||(mDiscKind.Bit.Media == eMediaRW)))
    {
        if (SetMode == eSET_PIC)
        {
            //MUST BE CHECKED whether neeeds to be different for set/reset PIC
            SetWobbleFreq(OverspeedX10);

            ClkKHz = HAL_PLL_GetWblClkKHz();
            // !!! we should still define specific coefs for PIC !!!
            PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpHighPassFcN1Hz);
            AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpHighPassFcN1Hz);
            LppFcN1Hz    = COEF_GetValue(WBL_CALC_LPP_DRHighPassFcN1Hz);
            HAL_WOBBLE_SetHPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);
            PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpLowPassFcN1Hz);
            AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpLowPassFcN1Hz);
            LppFcN1Hz    = COEF_GetValue(WBL_CALC_LPP_DRLowPassFcN1Hz);
            HAL_WOBBLE_SetLPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);

            HAL_WOBBLE_SetLPFAmpMeas(COEF_GetValue(WBL_CALC_AMPLPFBW_PIC));

            HAL_WOBBLE_GetAGCSettings(&sWblAGCSettings);
            sWblAGCSettings.WblAGCThreshMax = COEF_GetValue(WBL_CALC_WAGCTHRHI_PIC);
            sWblAGCSettings.WblAGCThreshMin = COEF_GetValue(WBL_CALC_WAGCTHRLO_PIC);
            HAL_WOBBLE_SetAGC(&sWblAGCSettings);
        }
        else  /* SetMode == eRESET_PIC */
        {
            //MUST BE CHECKED whether needs to be different for set/reset PIC
            SetWobbleFreq(OverspeedX10);

            ClkKHz = HAL_PLL_GetWblClkKHz();
            PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpHighPassFcN1Hz);
            AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpHighPassFcN1Hz);
            LppFcN1Hz    = COEF_GetValue(WBL_CALC_LPP_DRHighPassFcN1Hz);
            HAL_WOBBLE_SetHPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);
            PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpLowPassFcN1Hz);
            AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpLowPassFcN1Hz);
            LppFcN1Hz    = COEF_GetValue(WBL_CALC_LPP_DRLowPassFcN1Hz);
            HAL_WOBBLE_SetLPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);

            HAL_WOBBLE_SetLPFAmpMeas(COEF_GetValue(WBL_AMPMSCFG_AMPLPFBW));

            HAL_WOBBLE_GetAGCSettings(&sWblAGCSettings);
            sWblAGCSettings.WblAGCThreshMax = COEF_GetValue(WBL_WBLAGCCFG_WAGCTHRHI);
            sWblAGCSettings.WblAGCThreshMin = COEF_GetValue(WBL_WBLAGCCFG_WAGCTHRLO);
            HAL_WOBBLE_SetAGC(&sWblAGCSettings);
        }
    }
}

/* * * * * * *     P  I  C    * * * * * * * * */
#endif // (BD_ENABLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
