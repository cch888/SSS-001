/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: HAL_serve.c
*
* DESCRIPTION
* This file contains the HAL servo funtions implemented in the HAL servo layer
*
* NOTES:
*
*   $Revision: 18 $
*   $Date: 11/03/11 2:32p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\oem\oem_info.h"
#include ".\AL\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_servo.h"
#include ".\AL\REG_write_strategy.h"
#include ".\AL\REG_opc.h"
#if (CHIP_REV >= 0xC0)
#include "..\common\iram.h"
#include "..\servo\svo_step.h"
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
#define MAX_STM_PULSE_FREQUENCY  DSP_FREQ_PHASEA

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_ConfigureSTMPulseFrequency

    Description:    Configure the STM pulse frequency.

    Inputs:         Frequency describes the wanted STM pulse frequency
    Outputs:        The actual frequency be used
    Global Inputs:  None
    Global Outputs: None
    Returns:        The actual configured STM pulse frequency
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_SERVO_ConfigureSTMPulseFrequency(ULONG Frequency)
{
    ULONG StmCnt;
    BYTE  OpcAvg;

    OpcAvg = READ_FIELD(HAL_OPC_OPCAVG);
    Frequency = Frequency << OpcAvg;

    /* check input parameter */
    if (Frequency == 0)
    {
        StmCnt = 255;
    }
    else if (Frequency >= MAX_STM_PULSE_FREQUENCY)
    {
        StmCnt = 1;
    }
    else
    {
        StmCnt = (MAX_STM_PULSE_FREQUENCY + Frequency - 1) / Frequency; /* if sample rate too high, the data might full too early */
        if (StmCnt > 255) StmCnt = 255;
    }
    /* set STM pulse frequency */
    WRITE_FIELD(HAL_SRV_STMCNT, (BYTE)StmCnt);

    return (MAX_STM_PULSE_FREQUENCY / (StmCnt << OpcAvg));
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_SERVO_UpdateTime
    Description: Update overspeed related times
    Inputs:      OverspeedX10
    Outputs:     void
    Global Inputs: svoSpeedVar.TargetRpm, Iram_stp_CurrentPosition
    Global Outputs: HAL_SRV_FNGT, HAL_SRV_FOKT
    Returns:     void

    2 x BPS x PI x Sled_step x parFine
v = ----------------------------------
                    32

 length(um) * 176400 * 10                length(um) * 32 * 176400 * 10 * 1000000
-------------------------- = ---------------------------------------------------------------
overspeedx10 * V * 1000000   overspeedx10 * 2 * BPS * PI * Sled_step(um) * parFine * 1000000

        length(um)                 28224000           length(um) * VELOCITY_CONSTANT
= ---------------------- * ------------------------ = ------------------------------
  overspeedx10 * parFine   BPS * PI * Sled_step(um)       overspeedx10 * parFine

                           28224000
VELOCITY_CONSTANT = ------------------------
                    BPS * PI * Sled_step(um)

-----------------------------------------------------------------------------------------------------*/
//#if (CHIP_REV >= 0xC0)
#define VELOCITY_CONSTANT_CD    UM_2_SLED_STEP(119786)
#define VELOCITY_CONSTANT_DVD   UM_2_SLED_STEP(13288)
#define VELOCITY_CONSTANT_BD    UM_2_SLED_STEP(4076)
#define VELOCITY_CONSTANT_HD    UM_2_SLED_STEP(4023)

#define FNG_MIN_LENGTH       3000 * 2.0 //length of minimum focus drop in um
#define FNG_READ_MIN_TIME     14        //minimum of read drop time in periods of 5.67usec
#define FNG_WRITE_MIN_TIME    36        //minimum of write drop time in periods of 5.67usec
//#endif

#define DEFECT_MIN_LENGTH    30//500//1000       //length of minimum defect length in um

void HAL_SERVO_UpdateTime(USHORT OverspeedX10)
{
#if (CHIP_REV >= 0xC0)
    ULONG fng_time;
#endif
    ULONG velocity_constant,temp,def_time;


    temp = OverspeedX10 * svoSeekVar.parFineInit;

    if (temp)
    {
        switch(mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
                velocity_constant = VELOCITY_CONSTANT_CD;
                break;

            case eDiscDVD:
                velocity_constant = VELOCITY_CONSTANT_DVD;
    #if (DETECT_DVD_FE_DISTURB == 1)
                WRITE_FIELD(HAL_SRV_FSTHRES, DVD_FE_DISTURB_PARA_1 / OverspeedX10 + DVD_FE_DISTURB_PARA_2);
    #endif
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

#if (CHIP_REV >= 0xC0)
        fng_time = (ULONG)(FNG_MIN_LENGTH * velocity_constant) / temp;
#endif
        def_time = (ULONG)((DEFECT_MIN_LENGTH * 15) * velocity_constant) / temp;
    }
    else
    {
#if (CHIP_REV >= 0xC0)
        fng_time = FNG_READ_MIN_TIME;
#endif
#if (CHIP_REV >= 0xC0)
        def_time = 0x3FFF;
#else
        def_time = 0x1FF;
#endif
    }
#if (CHIP_REV >= 0xC0)
    if (fng_time > 0xFFF) fng_time = 0xFFF;
    if (READ_FIELD(HAL_WS_WGATEMON)==ON)
    {
        if (fng_time < FNG_WRITE_MIN_TIME) fng_time = FNG_WRITE_MIN_TIME;
    }
    else
    {
        if (fng_time < FNG_READ_MIN_TIME) fng_time = FNG_READ_MIN_TIME;
    }
    WRITE_FIELD(HAL_SRV_FNGT,fng_time);
    WRITE_FIELD(HAL_SRV_FOKT,fng_time);
#endif

#if (CHIP_REV >= 0xC0)
    if (def_time > 0x3FFF) def_time = 0x3FFF;
#else
    if (def_time > 0x1FF) def_time = 0x1FF;
#endif
    WRITE_FIELD(HAL_SRV_DLTHR,def_time);
}
/*****************************************************************************
*                       End Of File
*****************************************************************************/
