/* ******************************************************************************************
*
*   (c) Copyright 2006 - 2007 Sunext Design, INC.
*                 All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   LAL_lasc.c
*
*  Description:
*   This file contains functions for the LAL LASC layers and includes
*   the following:
*
*             .
*
* Notes:
*
*
*   $Revision: 40 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/
#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\Common\ComMath.h"
#include ".\Common\ADC.h"
#include ".\servo\svo.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_dsp.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_servo.h"
#include ".\al\LAL.h"
#include ".\al\LAL_lasc.h"



/* Global definitions regarding COEF */

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_LASC_Init

    Description:    Initialise the appropriate LASC registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: LASC Registers listed in the INIT coef table
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_LASC_Init(void)
{
    HAL_LASC_Init();
    //SEQUENTIAL DEPENDENT SETTINGS
    LAL_LASC_ClearLaserOutputs();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_LASC_ClearLaserOutputs()

    Description:    Initialise laser outputs by setting limitters to 0 and enabling PDM outputs.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many LASC Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void  LAL_LASC_ClearLaserOutputs(void)
{
    WRITE_FIELD(HAL_SRV_LCTHMIN  , 0);
    WRITE_FIELD(HAL_SRV_LCTHMAX  , 0);
    WRITE_FIELD(HAL_SRV_DELTA0MIN, 0);
    WRITE_FIELD(HAL_SRV_DELTA0MAX, 0);
    WRITE_FIELD(HAL_SRV_DELTA1MIN, 0);
    WRITE_FIELD(HAL_SRV_DELTA1MAX, 0);
    WRITE_FIELD(HAL_SRV_DELTA2MIN, 0);
    WRITE_FIELD(HAL_SRV_DELTA2MAX, 0);
    READ_MOD_WRITE(HAL_SRVREG_MAP->PDMCNT, (DELTA2_DIS | DELTA1_DIS | DELTA0_DIS | THRESH_DIS), 0x00);
    return;
}



#if (PDIC_BASE_ON_SRFO == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: LAL_LASC_GetPdic()
 * Description: Find the best PDIC gain for current disc
 *
 *----------------------------------------------------------------------------*/
SHORT const PDICGainCD[PDIC_GAIN_MAX_CD]   = {PDIC_GAIN_CD};
SHORT const PDICGainDVD[PDIC_GAIN_MAX_DVD] = {PDIC_GAIN_DVD};
    #if (BD_ENABLE == 1)
SHORT const PDICGainBD[PDIC_GAIN_MAX_BD]   = {PDIC_GAIN_BD};
    #endif
void LAL_LASC_GetPdic(USHORT SRFOPeakX1000, SHORT SRFOGaindBX10, BYTE calibrated)
{
    BYTE   i,PdicGainMax;
    USHORT target_srfo;
    SHORT  target_gain;
    const SHORT *PdicGain;

    switch (mDiscKind.Bit.DiscStd)
    {
    #if (BD_ENABLE == 1)
        case eDiscBD:
            target_srfo = PDIC_SRFO_TARGET_BD;
            PdicGain = PDICGainBD;
            PdicGainMax = PDIC_GAIN_MAX_BD;
            i = PDIC_STM_BD;
            break;
    #endif
        case eDiscDVD:
            target_srfo = PDIC_SRFO_TARGET_DVD;
            PdicGain = PDICGainDVD;
            PdicGainMax = PDIC_GAIN_MAX_DVD;
            i = PDIC_STM_DVD;
            break;

        case eDiscCD:
            target_srfo = PDIC_SRFO_TARGET_CD;
            PdicGain = PDICGainCD;
            PdicGainMax = PDIC_GAIN_MAX_CD;
            i = PDIC_STM_CD;
            break;
    }

    if (SRFOPeakX1000 == 0)
    {
        svoVar_bPdicMode = 0xFF;
    }
    else if (PdicGainMax == 1)
    {
        i = 0;  
        target_gain = CalculateXdBFromRate(P10dB, target_srfo, SRFOPeakX1000) + SRFOGaindBX10;
        if (svoVar_bLascMode == 0)
        {
            if (PdicGain[i] > target_gain)
            {
                svoVar_bLascMode = 1;
                SendMsgCn(SHOW_DEBUG_MSG,6,0x31669D,A2B(target_gain),A2B(PdicGain[i]),A2B(READ_DSP_COEF(Pread0RS)));//Decrease Laser Power: LASER_PDIC_SRFO_GAIN = %d, PDIC_GAIN = %d, POWER_DAC= %d
            }
        }
        else if (calibrated == FALSE)
        {
            if (PdicGain[i] < target_gain)
            {
                svoVar_bLascMode = 0;
                SendMsgCn(SHOW_DEBUG_MSG,6,0x31669E,A2B(target_gain),A2B(PdicGain[i]),A2B(READ_DSP_COEF(Pread0RS)));//Increase Laser Power: LASER_PDIC_SRFO_GAIN = %d, PDIC_GAIN = %d, POWER_DAC= %d
            }
        }
    }
    else
    {
        if (calibrated == TRUE)
        {//if PDIC is calibrated, only change PDIC gain when current gain is too large.
            PdicGainMax = svoVar_bPdicMode + 1;
        }

        target_gain = CalculateXdBFromRate(P10dB, target_srfo, SRFOPeakX1000) + SRFOGaindBX10;

        for (i = 1; i < PdicGainMax; i++)
        {
            if (PdicGain[i] > target_gain)
            {
                break;
            }
        }
        i--;
        SendMsgCnB(SHOW_DEBUG_MSG,9,0x338320,F4B((float)target_gain/10),F4B((float)PdicGain[i]/10),B1B(i));//PDIC calib, wanted gain = %2.1fdB, result = %2.1fdB, mode:%u
    }

    if (i != svoVar_bPdicMode)
    {
        svoVar_bPdicMode = i;
        calSetMisc(0,PDIC_GN,PdicGain[i]);
        HAL_LASC_SetPdic();
    }
}// end of LAL_LASC_GetPdic

/*-----------------------------------------------------------------------------
 *
 * Module: LAL_LASC_LaserOffsetAdj
 * Description: adjust front monitor offsets
 * Precondition: LASER MUST BE OFF!!!
 *
 *----------------------------------------------------------------------------*/
void LAL_LASC_LaserOffsetAdj(void)
{
    ULONG   vrpow=0;
    ULONG   vwpow=0;
    USHORT  i=0;

    calCalibrateOffset(APC_OS, TYPE4, NORMAL_OFFSET, GPADC_IN_MIN + 50, TRUE, CALIB_FROM_0);
    calSetAfeOffset(0, APC_OS, TYPE4, 0, 0, last_cal_result);

    // VRPOW: 10X, VWPOW: 7.5 (dB), CD/DVD/BD VREF06_OS = %1d (mV)
    SendMsgCnB(SHOW_DEBUG_MSG,2,0x316500+mDiscKind.Bit.DiscStd, B2B(last_cal_result*40));

    //TBD? COMMENT! Need a flag to protect offset adjustment?
    WRITE_FIELD(HAL_SRV_FMTRES,OFF);// Only FM offset adjustment
    WRITE_FIELD(HAL_SRV_FMDRES,OFF);// Only FM offset adjustment
    DelaymS(1);
    //Wait until offset adjustment done in hardware(1-2ms?)
    do
    {
        vrpow+=ReadAverageAdcmV(ADC_VRPOW, 0);
        vwpow+=ReadAverageAdcmV(ADC_VWPOW, 0);
        i++;
    }
    while ((READ_FIELD(HAL_SRV_INTSTP) == 0) && (i < 0xFFFF));
    if (i == 0xFFFF)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x3165FD);//FMD OFFSET Calibration Timeout
    }
//    send_msg74(SHOW_DEBUG_MSG,0x5165F9,(float)vrpow);
//    send_msg74(SHOW_DEBUG_MSG,0x5165FA,(float)vwpow);
//    send_msg5SValue(1,"Smp_Count",2,i);
    //Save the adjusted value for restorage later
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FMTHRESHO);  /* select Front Monitor Threshold Offset Data data */
    svoCalVar.fmThrData.all16 = READ_REG(HAL_SRVREG_MAP->SRCR);

    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FMDELTAO);  /* select Front Monitor Delta Offset Data data */
    svoCalVar.fmDelData.all16 = READ_REG(HAL_SRVREG_MAP->SRCR);
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            SendMsgCn(DEBUG_LASER_MSG,4,0x3165F7, A2B(svoCalVar.fmThrData.all16), A2B(svoCalVar.fmDelData.all16));
            break;

        case eDiscDVD:
            SendMsgCn(DEBUG_LASER_MSG,4,0x3165F8, A2B(svoCalVar.fmThrData.all16), A2B(svoCalVar.fmDelData.all16));
            break;

        case eDiscBD:
            SendMsgCn(DEBUG_LASER_MSG,4,0x3165F9, A2B(svoCalVar.fmThrData.all16), A2B(svoCalVar.fmDelData.all16));
            break;
    }
    vrpow/=i;
    vwpow/=i;
    SendMsgCn(DEBUG_LASER_MSG,4,0x316573, A2B(vrpow),A2B(vwpow));
}
#endif
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
