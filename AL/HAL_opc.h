/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: HAL_opc.h
*
* DESCRIPTION
* This file supports the HAL OPC functions implemented in the HAL OPC layer
*
* NOTES:
*
*   $Revision: 17 $
*   $Date: 11/03/11 5:53p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Compiler Definitions
*****************************************************************************/
#ifndef _HALOPC_H_
#define _HALOPC_H_

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\COMMON\globtype.h"
#include ".\COMMON\globals.h"
#include ".\COMMON\chip.h"
#include ".\AL\reg_hal.h"
#include ".\AL\reg_opc.h"
#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */

#if  STREAM_STM

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
//LAL_STREAM_TODO: Are these macro's correct ?
/* these macros must be used to read the fields of DfeSampleS data */
/* these macros must be used to read the fields of AfeSampleS data */
    #if (CHIP_REV >= 0xC0)
#define STREAM_DENEB_C0                 1
    #else
#define STREAM_DENEB_C0                 0
    #endif

    #if (CHIP_REV == 0xC0)
#define WORKAROUND_STREAM_C0            1
    #else
#define WORKAROUND_STREAM_C0            0
    #endif



#define HAL_OPC_DATA_SAMPLE(i)     (BYTE)(HAL_OPC_pSRAM[i].AfeSampleS.PowerStep)
#define HAL_OPC_DATA_CHANNEL1(i)   (USHORT)(((USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel1DataUpper) << 8) + (USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel1DataLower))
#define HAL_OPC_DATA_CHANNEL2(i)   (USHORT)(((USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel2DataUpper) << 8) + (USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel2DataLower))
#define HAL_OPC_DATA_CHANNEL3(i)   (USHORT)(((USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel3DataUpper) << 8) + (USHORT)(HAL_OPC_pSRAM[i].AfeSampleS.Channel3DataLower))

#define HAL_OPC_DATA_CALF(i)       (BYTE)(HAL_OPC_pSRAM[i].DfeSampleS.CalfData)
#define HAL_OPC_DATA_A1(i)         (BYTE)(HAL_OPC_pSRAM[i].DfeSampleS.A1Data)
#define HAL_OPC_DATA_A2(i)         (BYTE)(HAL_OPC_pSRAM[i].DfeSampleS.A2Data)
#define HAL_OPC_DATA_JITTER(i)     (USHORT)(((USHORT)(HAL_OPC_pSRAM[i].DfeSampleS.JitterDataUpper) << 8) + (USHORT)(HAL_OPC_pSRAM[i].DfeSampleS.JitterDataLower))
#define HAL_OPC_DATA_POWERSTEP(i)  (BYTE)(HAL_OPC_pSRAM[i].DfeSampleS.PowerStep)

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/
typedef union
{
    BYTE data[6];
    /*__packed*/ struct
    {
        /* data 0 */
	    BYTE Channel1DataLower:8;
        /* data 1 */
        BYTE Channel1DataUpper:2;
	    BYTE Reserved2:6; //=PowerStep
        /* data 2 */
	    BYTE Channel2DataLower:8;
        /* data 3 */
        BYTE Channel2DataUpper:2;
	    BYTE Reserved1:6; //=PowerStep
        /* data 4 */
	    BYTE Channel3DataLower:8;
        /* data 5 */
        BYTE Channel3DataUpper:2;
	    BYTE PowerStep:6;
    } AfeSampleS;
    /*__packed*/ struct
    {
        /* data 0 */
        BYTE CalfData:8;
        /* data 1 */
        BYTE A2Data:8;
        /* data 2 */
        BYTE A1Data:8;
        /* data 3 */
        BYTE JitterDataLower:8;
        /* data 4 */
        BYTE JitterDataUpper:2;
        BYTE Reserved2:6;
        /* data 5 */
        BYTE PowerStep:6;
        BYTE Reserved1:2;
    } DfeSampleS;
} HAL_OPC_SampleT;

/* data type describing the OPC modes */
typedef enum
{
    HAL_OPC_MODE_WRITE = 0,
    HAL_OPC_MODE_READ,
    HAL_OPC_MODE_STM,
    HAL_OPC_MODE_LAST /* always last */
} HAL_OPC_ModeT;

// data type describing the non gated sampling; not used yet
typedef struct
{
    USHORT      NrOfSamples;
    ULONG       Frequency; /* Unit is Hz */
    BOOL        AlphaStepper;
} HAL_OPC_NonGatedSamplingT;

/* definition of interrupt sources */
typedef enum
{
    HAL_OPC_INTERRUPT_STP  = 0x01,
    HAL_OPC_INTERRUPT_TD   = 0x02,
    HAL_OPC_INTERRUPT_TC   = 0x04,
    HAL_OPC_INTERRUPT_TR   = 0x08,
    HAL_OPC_INTERRUPT_OVL  = 0x10,
    HAL_OPC_INTERRUPT_RATE = 0x20,
} HAL_OPC_InterruptT;

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/
/* Defines the start address in SRAM */
extern /*const*/ volatile HAL_OPC_SampleT *HAL_OPC_pSRAM;

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/
/* configure front-end */
__inline void HAL_OPC_ConfigureAnalogFE(void)
{
    WRITE_FIELD(HAL_OPC_OPCDSRC, halOPC_OPCDSRC_ADC); /* select AFE data */
}
__inline void HAL_OPC_ConfigureDigitalFE(void)
{
    WRITE_FIELD(HAL_OPC_OPCDSRC, halOPC_OPCDSRC_DFE); /* select DFE data */
}
/* configure Averaging */
/* configure debug mode */
__inline void HAL_OPC_ConfigureDebugMode(BOOL enable)
{
    WRITE_FIELD(HAL_OPC_OPCTEST, enable);
}

/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
/* initialisation */
extern void HAL_OPC_Init(void);

/* configure sampling mode */
extern void HAL_OPC_ConfigureGatedSampling(halOPC_AOPCSampNum_t NrOfSamples);

/* configure Averaging */
extern void HAL_OPC_ConfigureAveraging(halOPC_AOPCAvgSamp_t eAvgSamples);
/* enable OPC mode */
extern void HAL_OPC_EnableOPCMode(HAL_OPC_ModeT eMode);

/* interrupt stuff */
extern void HAL_OPC_EnableInterrupt(HAL_OPC_InterruptT Interrupt, BOOL value);
extern BOOL HAL_OPC_IsInterruptSet(HAL_OPC_InterruptT Interrupt);


/* ISR to be called by interrupt */
extern BOOL HAL_OPC_Isr(void);

/* Configures the wobble sample frequency */
extern void HAL_OPC_ConfigureWblSampleFrequency(USHORT OverspeedX100, float SectorsPerStep, halOPC_AOPCSampNum_t eSamplesPerStep, BYTE StartDelay, BYTE StopDelay, BYTE OpcPrescaler);


#endif//STREAM_STM

/*****************************************************************************
*                       End Of File
*****************************************************************************/
#endif /* _HALOPC_H_ */
