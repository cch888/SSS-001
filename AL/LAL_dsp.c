/* ******************************************************************************************
*
*              (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                             All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*  LAL_dsp.c
*
*  Description:
*  This file contains functions for the LAL DSP layers and includes
*  the following:
*        LAL_DSP_Init
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
#include ".\AL\REG_HAL.h"
#include ".\al\LAL_dsp.h"
#include ".\al\HAL_dsp.h"
#include ".\servo\svo_dsp.h"
#if (NEW_FDP_DETECTION == 2) || (ENABLE_LEARN_DEFECT == 1)
#include ".\common\iram.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_step.h"
#endif

/* Global definitions regarding COEF */

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DSP_Init

    Description:    Initialise the appropriate DSP registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: DSP Registers listed in the INIT coef table
    Returns:        Indicates whether the initialisation was succesfull: Is TRUE if initialisation was succesfull
-----------------------------------------------------------------------------------------------------*/
BOOL LAL_DSP_Init(void)
{
    BOOL CallResult;

    HAL_DSP_Init(); //global init of DSP

    HAL_PLL_InitSvoPll(DSP_POWER_MODE);
    HAL_DSP_ClockInit();
    HAL_DSP_CoefRAMInit(DSP_BIST_MODE);
    HAL_DSP_Load(DSP_BIST_MODE);

    CallResult = HAL_DSP_BISTPass();
    if (CallResult)
    {
        HAL_DSP_CoefRAMInit(DSP_NORMAL_MODE);
        HAL_DSP_Load(DSP_NORMAL_MODE);
    }
    HAL_DSP_EnableOutputs();
    return CallResult;
}

#if (ENABLE_LEARN_DEFECT == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:        LAL_DSP_SetLearningDefect
    Description: Enables learning defect feature
    Inputs:      BOOL enable
    Outputs:     void
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
#define FE_LEARN_DFCT_SCALE 0x1000      //scale gain of learn defect loop (FE input)
#define FE_LEARN_DFCT_GAIN 0x200        //clipping gain on FOO output
#define FE_LEARN_DFCT_LEAK 0x7FC0       //leak factor on learning defect
#define TE_LEARN_DFCT_SCALE 0x1000      //scale gain of learn defect loop (FE input)
#if((CEProduct == 1)&&((PCB == 710)||(PCB==711)||(PCB==817)))
#define TE_LEARN_DFCT_GAIN 0x600        //clipping gain on TRO output
#else
#define TE_LEARN_DFCT_GAIN 0x200        //clipping gain on TRO output
#endif
#define TE_LEARN_DFCT_LEAK 0x7FC0       //leak factor on learning defect
#if (BD_ENABLE == 1)
#define FE_LEARN_DFCT_GAIN_BD 0x400     //BD clipping gain on FOO output
#define TE_LEARN_DFCT_GAIN_BD 0x600     //BD clipping gain on TRO output
#endif
#define FE_LEARN_DFCT_GAIN_CD 0x100
#define TE_LEARN_DFCT_GAIN_CD 0x100
void LAL_DSP_SetLearnDefect(lalDSP_OnOff_t mode)
{
    switch(mode)
    {
        case lalDSP_ResetOn:
            WRITE_DSP_COEF(FE_learndfct_scale, 0);
            while (READ_DSP_COEF(FE_learndfct_value) != 0) //reset procedure
                WRITE_DSP_COEF(FE_learndfct_value, 0);

            WRITE_DSP_COEF(TE_learndfct_scale, 0);
            while (READ_DSP_COEF(TE_learndfct_value) != 0) //reset procedure
                WRITE_DSP_COEF(TE_learndfct_value, 0); //no break here (must fall-through to next case)

        case lalDSP_On:
            WRITE_DSP_COEF(FE_learndfct_scale, (USHORT)FE_LEARN_DFCT_SCALE);
#if (BD_ENABLE == 1)//hugo debug
            if (mDiscKind.Bit.DiscStd == eDiscBD)
            {
            WRITE_DSP_COEF(FE_learndfct_gain, (USHORT)FE_LEARN_DFCT_GAIN_BD);
            WRITE_DSP_COEF(TE_learndfct_gain, (USHORT)TE_LEARN_DFCT_GAIN_BD);
            }
            else
#endif
            {
                if (mDiscKind.Bit.DiscStd == eDiscCD)
                {
                    WRITE_DSP_COEF(FE_learndfct_gain, (USHORT)FE_LEARN_DFCT_GAIN_CD);
                    WRITE_DSP_COEF(TE_learndfct_gain, (USHORT)TE_LEARN_DFCT_GAIN_CD);
                }
                else
                {
                    WRITE_DSP_COEF(FE_learndfct_gain, (USHORT)FE_LEARN_DFCT_GAIN);
                    WRITE_DSP_COEF(TE_learndfct_gain, (USHORT)TE_LEARN_DFCT_GAIN);
                }
            }
            WRITE_DSP_COEF(FE_learndfct_leak, (USHORT)FE_LEARN_DFCT_LEAK);
            WRITE_DSP_COEF(TE_learndfct_scale, (USHORT)TE_LEARN_DFCT_SCALE);
            WRITE_DSP_COEF(TE_learndfct_leak, (USHORT)TE_LEARN_DFCT_LEAK);
            break;

        case lalDSP_Off:
            WRITE_DSP_COEF(FE_learndfct_gain, 0);
            WRITE_DSP_COEF(FE_learndfct_scale, 0);
            while (READ_DSP_COEF(FE_learndfct_value) != 0) //reset procedure
                WRITE_DSP_COEF(FE_learndfct_value, 0);

            WRITE_DSP_COEF(TE_learndfct_gain, 0);
            WRITE_DSP_COEF(TE_learndfct_scale, 0);
            while (READ_DSP_COEF(TE_learndfct_value) != 0) //reset procedure
                WRITE_DSP_COEF(TE_learndfct_value, 0);
            break;

        default:
            break;
    }
}
#endif//#if (ENABLE_LEARN_DEFECT == 1)

#if (NEW_FDP_DETECTION == 2) || (ENABLE_LEARN_DEFECT == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:        LAL_DSP_UpdateTime
    Description: Update overspeed related times
    Inputs:      OverspeedX10
    Outputs:     void
    Global Inputs: svoSpeedVar.TargetRpm, Iram_stp_CurrentPosition
    Global Outputs:
    Returns:     void

    2 x BPS x PI x Sled_step x parFine
v = ----------------------------------
                    32

 length(um) * 44100 * 10                   length(um) * 32 * 44100 * 10 * 1000000
-------------------------- = ---------------------------------------------------------------
overspeedx10 * V * 1000000   overspeedx10 * 2 * BPS * PI * Sled_step(um) * parFine * 1000000

        length(um)                 7056000            length(um) * VELOCITY_CONSTANT
= ---------------------- * ------------------------ = ------------------------------
  overspeedx10 * parFine   BPS * PI * Sled_step(um)       overspeedx10 * parFine

                            7056000
VELOCITY_CONSTANT = ------------------------
                    BPS * PI * Sled_step(um)
-----------------------------------------------------------------------------------------------------*/
#define VELOCITY_CONSTANT_CD    UM_2_SLED_STEP(29947)
#define VELOCITY_CONSTANT_DVD   UM_2_SLED_STEP(3322)
#define VELOCITY_CONSTANT_BD    UM_2_SLED_STEP(1019)
#define VELOCITY_CONSTANT_HD    UM_2_SLED_STEP(1006)

#if (NEW_FDP_DETECTION == 2)
#define FNG_MIN_LENGTH       3000 * 2.5 //length of minimum focus drop in um
#define FNG_MIN_TIME         22         //minimum of drop time in periods of 22.7usec
#endif

#define LEARN_DFCT_MIN_LENGTH 1500      //length of minimum defect in um
#define LEARN_DFCT_MIN_TIME 3           //minimum of defect time in periods of 22.7usec
#if (BD_ENABLE == 1)
#define LEARN_DFCT_MIN_LENGTH_BD 600    //length of minimum defect in um
#endif
void LAL_DSP_UpdateTime(USHORT OverspeedX10)
{
    #if (NEW_FDP_DETECTION == 2)
    ULONG fng_time;
    #endif
    #if (ENABLE_LEARN_DEFECT == 1)
    ULONG defect_time;
    #endif
    ULONG velocity_constant,temp;

    switch(mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            velocity_constant = VELOCITY_CONSTANT_CD;
            break;

        case eDiscDVD:
            velocity_constant = VELOCITY_CONSTANT_DVD;
            break;
    #if (ENABLE_HDDVD == 1)
        case eDiscHD:
            velocity_constant = VELOCITY_CONSTANT_HD;
            break;
    #endif
    #if (BD_ENABLE == 1)
        case eDiscBD:
            velocity_constant = VELOCITY_CONSTANT_BD;
            break;
    #endif
    }
    temp = OverspeedX10 * svoSeekVar.parFineInit;
    if (temp)
    {
    #if (NEW_FDP_DETECTION == 2)
        fng_time = FNG_MIN_LENGTH * velocity_constant / temp;
    #endif
    #if (ENABLE_LEARN_DEFECT == 1)
        #if (BD_ENABLE == 1)
        if (mDiscKind.Bit.DiscStd == eDiscBD)
            defect_time = LEARN_DFCT_MIN_LENGTH_BD * velocity_constant / temp;
        else
        #endif
            defect_time = LEARN_DFCT_MIN_LENGTH * velocity_constant / temp;
    #endif
    }
    else
    {
    #if (NEW_FDP_DETECTION == 2)
        fng_time = FNG_MIN_TIME;
    #endif
    #if (ENABLE_LEARN_DEFECT == 1)
        defect_time = LEARN_DFCT_MIN_TIME;
    #endif
    }
    #if (NEW_FDP_DETECTION == 2)
    if (fng_time > 32767) fng_time = 32767;
    if (fng_time < FNG_MIN_TIME) fng_time = FNG_MIN_TIME;
    WRITE_DSP_COEF(TNG_preset,fng_time);
    #endif
    #if (ENABLE_LEARN_DEFECT == 1)
    if (defect_time > 32767) defect_time = 32767;
    if (defect_time < LEARN_DFCT_MIN_TIME) defect_time = LEARN_DFCT_MIN_TIME;
    WRITE_DSP_COEF(FE_learndfct_time, (USHORT)defect_time);
    #endif
}
#endif // #if (NEW_FDP_DETECTION == 2) || (ENABLE_LEARN_DEFECT == 1)

#if (XDFCT_ENABLE == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:        LAL_DSP_XDFCT_Enable
    Description: Enables XDCFT feature
    Inputs:      none
    Outputs:     void
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void LAL_DSP_XDFCT_Enable(lalDSP_XDFCT_t mode, USHORT threshold)
{
    switch(mode)
    {
        case lalDSP_Disable:
            WRITE_DSP_COEF(xdfct_TE_thresh,0x007F); //Disable TE trigger
            WRITE_DSP_COEF(xdfct_FE_thresh,0x007F); //Disable FE trigger
            break;
        case lalDSP_Enable_TE:
            WRITE_DSP_COEF(xdfct_FE_thresh,0x007F); //Disable FE trigger
            WRITE_DSP_COEF(xdfct_TE_thresh,threshold); //Enable TE trigger
            WRITE_FIELD(HAL_SRV_ACTDSSRST, 0); //Enable TE_threshold detection
            break;
        case lalDSP_Enable_FE:
            WRITE_DSP_COEF(xdfct_TE_thresh,0x007F); //Disable TE trigger
            WRITE_DSP_COEF(xdfct_FE_thresh,threshold); //Enable FE trigger
            WRITE_FIELD(HAL_SRV_ACTDSSRST, 1); //Enable FE_threshold detection
            break;
    }
}
#endif //(XDCFT_ENABLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
