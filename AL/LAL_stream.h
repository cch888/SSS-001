/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: LAL_stream.h
*
* DESCRIPTION
* This file supports the LAL stream functions implemented in the LAL stream layer
*
* NOTES:
*
*   $Revision: 15 $
*   $Date: 11/03/11 5:53p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Compiler Definitions
*****************************************************************************/
#ifndef _LALSTREAM_H_
#define _LALSTREAM_H_

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\AL\HAL_servo.h"
#include ".\AL\HAL_opc.h"
#include ".\AL\HAL_afe.h"
#include ".\COMMON\globtype.h"
#include ".\COMMON\globals.h"
#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\servo\svoVar.h"

#if  STREAM_STM
/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
/* this macro defines the max. number of input signals on the analog frontend */
#define LAL_STREAM_AFE_MAX_NR_OF_SIGNALS    3

/* macros for data access */
#define LAL_STREAM_DATA_CALF(i)        HAL_OPC_DATA_CALF(i)
#define LAL_STREAM_DATA_A1(i)          HAL_OPC_DATA_A1(i)
#define LAL_STREAM_DATA_A2(i)          HAL_OPC_DATA_A2(i)
#define LAL_STREAM_DATA_JITTER(i)      HAL_OPC_DATA_JITTER(i)
#define LAL_STREAM_DATA_POWERSTEP(i)   HAL_OPC_DATA_POWERSTEP(i)

#define LAL_STREAM_DATA_SAMPLE(i)      HAL_OPC_DATA_SAMPLE(i)
#define LAL_STREAM_DATA_CHANNEL1(i)    HAL_OPC_DATA_CHANNEL1(i)
#define LAL_STREAM_DATA_CHANNEL2(i)    HAL_OPC_DATA_CHANNEL2(i)
#define LAL_STREAM_DATA_CHANNEL3(i)    HAL_OPC_DATA_CHANNEL3(i)

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/
/* Data type for front-end selection */
typedef enum
{
    LAL_STREAM_FE_ANALOG,
    LAL_STREAM_FE_DIGITAL
} LAL_STREAM_FETypeT;

/* list of the supported analog frontend input signal types */
typedef enum
{
   LAL_STREAM_AFE_FE = HAL_AFE_FE,
   LAL_STREAM_AFE_TE = HAL_AFE_TE,
   LAL_STREAM_AFE_BHSO = HAL_AFE_BHSO,
   LAL_STREAM_AFE_RP = HAL_AFE_RP,
   LAL_STREAM_AFE_SBAD = HAL_AFE_SBAD,
   LAL_STREAM_AFE_CE = HAL_AFE_CE,
   LAL_STREAM_AFE_SRFO = HAL_AFE_SRFO,
   LAL_STREAM_AFE_BS = HAL_AFE_BS
} LAL_STREAM_AnalogFESignalT;

/* Data type describing analog frontend signals */
typedef struct
{
    BYTE                        NrOfSignals;
    LAL_STREAM_AnalogFESignalT  Signal[LAL_STREAM_AFE_MAX_NR_OF_SIGNALS];
} LAL_STREAM_AnalogFESignalsT;

/* Data type describing digital front-end signals */
/* NOTE: this type is currently fixed by hardware and therefore empty */
typedef struct
{
  USHORT dummy;
} LAL_STREAM_DigitalFESignalsT;

/* Data type describing input signals on frontend */
typedef union
{
    LAL_STREAM_AnalogFESignalsT   Afe;
    LAL_STREAM_DigitalFESignalsT  Dfe;
} LAL_STREAM_InputSignalsDescriptorT;

/* data typing describing the sampling */
typedef struct
{
    halOPC_AOPCSampNum_t    eNrOfSamples;
    ULONG                   Frequency; /* Unit is Hz */
    halOPC_AOPCAvgSamp_t    eAvgSamples;
} LAL_STREAM_TimerSamplingDescriptorT;

typedef struct
{
    USHORT                       OverspeedX100;
    USHORT                       NrOfSteps;         //Is NrOfSectors / SectorsPerStep
    BYTE                         StartDelay;        //Unit Percent
    BYTE                         StopDelay;         //Unit Percent
    float                        SectorsPerStep;    //Is a kind of a frequency, but related to the wobble
    halOPC_AOPCSampNum_t         eSamplesPerStep;   //Sub-samples
    ULONG                        StartAddr;
    halOPC_AOPCAvgSamp_t         eAvgSamples;
} LAL_STREAM_WobbleSamplingDescriptorT;

typedef union
{
    LAL_STREAM_WobbleSamplingDescriptorT wobble;
    LAL_STREAM_TimerSamplingDescriptorT  timer;
} LAL_STREAM_SamplingDescriptorT;

/* data type describing the OPC operating modes */
typedef enum
{
    LAL_STREAM_MODE_WOBBLE = HAL_OPC_MODE_READ,
    LAL_STREAM_MODE_TIMER = HAL_OPC_MODE_STM
} LAL_STREAM_ModeT;

/* data type describing the states */
typedef enum
{
    LAL_STREAM_STATUS_IDLE,
    LAL_STREAM_STATUS_BUSY,
    LAL_STREAM_STATUS_READY,
    LAL_STREAM_STATUS_ERROR
} LAL_STREAM_StatusT;

/* data type describing the (error) result */
typedef enum
{
    LAL_STREAM_OK,
    LAL_STREAM_ERROR_ABORT,
    LAL_STREAM_ERROR_SAMPLING,
    LAL_STREAM_ERROR_TARGET_MATCH_MISSED
} LAL_STREAM_ErrorT;

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/
/* configure for debug mode */
__inline void LAL_STREAM_ConfigureDebugMode(BOOL enable)
{
    HAL_OPC_ConfigureDebugMode(enable);
}

/* configure Averaging */
__inline void LAL_STREAM_ConfigureAveraging(halOPC_AOPCAvgSamp_t eAvgSamples)
{
    HAL_OPC_ConfigureAveraging((halOPC_AOPCAvgSamp_t) eAvgSamples);
}

/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
/* initialisation */
extern void LAL_STREAM_Init(void);
/* configuration */
extern void LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FETypeT eType, LAL_STREAM_InputSignalsDescriptorT *pDescriptor);
extern void LAL_STREAM_ConfigureSampling(LAL_STREAM_ModeT eMode, LAL_STREAM_SamplingDescriptorT *pDescriptor);
/* turnon/off */
extern void LAL_STREAM_Start(HAL_OPC_ModeT opcMode);
extern void LAL_STREAM_Stop(void);
extern void LAL_STREAM_Abort(void);
/* result handling */
extern LAL_STREAM_StatusT LAL_STREAM_GetStatus(void);
extern USHORT LAL_STREAM_GetNrOfSamples(void);
extern void LAL_STREAM_PrintSamples(void);
extern LAL_STREAM_ErrorT LAL_STREAM_GetError(void);
/* Store some global parameters */
extern void LAL_STREAM_Analyse(eRampMeasType type, USHORT samples, USHORT frequency, BYTE direction);


#endif//STREAM_STM


#endif /* _LALSTREAM_H_ *//****************************************************                       End Of File*****************************************************************************/
