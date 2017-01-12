/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: LAL_stream.c
*
* DESCRIPTION
* This file contains the LAL stream functions implemented in the LAL stream layer
*
* NOTES:
*
*   $Revision: 23 $
*   $Date: 11/03/25 5:01p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\common\Iram.h"
#include ".\common\commath.h"
#include ".\oem\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_cd_encoder.h"
#include ".\AL\LAL_stream.h"
#include ".\al\coefs.h"
#include ".\oem\coef_values.h"
#include ".\AL\LAL_shared_sram.h"
#include ".\servo\Svo_lib.h"

#if  STREAM_STM
/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define DEBUG_STREAM

/* this macro enables/disables all streaming interrupts */
#ifdef DEBUG_STREAM
#define ENABLE_OPC_INTERRUPTS()    HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_STP, TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TD,  TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TC,  TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TR,  TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_OVL, TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_RATE,TRUE)

#define DISABLE_OPC_INTERRUPTS()   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_STP, FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TD,  FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TC,  FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_TR,  FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_OVL, FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_RATE,FALSE)
#else
#define ENABLE_OPC_INTERRUPTS()    HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_STP, TRUE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_RATE, TRUE)

#define DISABLE_OPC_INTERRUPTS()   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_STP, FALSE); \
                                   HAL_OPC_EnableInterrupt(HAL_OPC_INTERRUPT_RATE, FALSE)
#endif

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

static LAL_STREAM_ModeT   mStreamMode;
static BOOL               mStreamLocked;
static LAL_STREAM_StatusT mStreamStatus;
static LAL_STREAM_ErrorT  mStreamError;

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
    Name:           LAL_STREAM_Init

    Description:    Initialize the LAL Stream module

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_Init(void)
{
    send_msg80(DEBUG_STREAM_MSG,0x640000); //LAL_STREAM_Init
    mStreamMode = LAL_STREAM_MODE_WOBBLE;
    mStreamLocked = FALSE;
    mStreamStatus = LAL_STREAM_STATUS_IDLE;
    mStreamError = LAL_STREAM_OK;
    HAL_OPC_Init();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_ConfigureInputSignals

    Description:    Configure the front-end input signals

    Inputs:         eType descibes the front-end type = {Analog, Digital}
                    pDescriptor is a pointer to the front-end configuration data.
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FETypeT eType, LAL_STREAM_InputSignalsDescriptorT* pDescriptor)
{
    int i;
    BYTE AdcInput;

    send_msg80(DEBUG_STREAM_MSG,0x640001); //LAL_STREAM_ConfigureInputSignals
    switch(eType)
    {
       case LAL_STREAM_FE_ANALOG:
           HAL_OPC_ConfigureAnalogFE();
           for(i = 0; i < pDescriptor->Afe.NrOfSignals; i++)
           {
               /* set MUX1..3 */
               AdcInput = HAL_AFE_SetMuxToAdc((HAL_AFE_MuxSignalT)pDescriptor->Afe.Signal[i]); /* NOTE: explicit conversion */
               /* set ADC0..2 input signal */
               switch(i)
               {
                  case 0:
                     WRITE_FIELD(HAL_OPC_ADCSLOT0, AdcInput);
                     break;
                  case 1:
                     WRITE_FIELD(HAL_OPC_ADCSLOT1, AdcInput);
                     break;
                  case 2:
                     WRITE_FIELD(HAL_OPC_ADCSLOT2, AdcInput);
                     break;
                  default:
                     break;
               }
           }
           break;
       case LAL_STREAM_FE_DIGITAL:
           HAL_OPC_ConfigureDigitalFE();
           break;
       default:
           /*  do nothing */
           break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_ConfigureSampling

    Description:    Configure the sampling mode

    Inputs:         eMode describes the operation mode = {Timer, Wobble}
                    pDescriptor is a pointer to the sampling configuration data.
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_ConfigureSampling(LAL_STREAM_ModeT eMode, LAL_STREAM_SamplingDescriptorT *pDescriptor)
{
    send_msg80(DEBUG_STREAM_MSG,0x640002); //LAL_STREAM_ConfigureSampling
    mStreamMode = eMode;
    send_msg80(DEBUG_STREAM_MSG,0x560007+mStreamMode); //mStreamMode=

    switch ( mStreamMode )
    {
        case LAL_STREAM_MODE_TIMER:
           WRITE_FIELD(HAL_OPC_OPCSIZE, 0xC0); //Patch to avoid streaming error
           HAL_OPC_ConfigureAveraging((halOPC_AOPCAvgSamp_t) pDescriptor->timer.eAvgSamples);
           HAL_OPC_ConfigureGatedSampling((halOPC_AOPCSampNum_t)pDescriptor->timer.eNrOfSamples);
           pDescriptor->timer.Frequency = HAL_SERVO_ConfigureSTMPulseFrequency(pDescriptor->timer.Frequency);
           break;

//        case LAL_STREAM_MODE_WOBBLE:
//           break;

        default:
           break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_Start

    Description:    Start stream operation.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_Start(HAL_OPC_ModeT opcMode)
{
    if (mStreamLocked)
    {
        DEBUG_ASSERT(FALSE, eASSERT_FUNCTION_NOT_IMPLEMENTED);
    }
    else
    {
        SendMsg80(DEBUG_STREAM_MSG,0x3FAA1C); //STREAM_Start

        mStreamLocked = TRUE;
        mStreamStatus = LAL_STREAM_STATUS_BUSY;
        mStreamError = LAL_STREAM_OK;
        switch ( mStreamMode )
        {
            case LAL_STREAM_MODE_TIMER:
               /* enable STM frequency */
               HAL_SERVO_EnableSTMPulseFrequency(TRUE); //LAL_STREAM_TODO: call for STM mode only ?
               break;

            case LAL_STREAM_MODE_WOBBLE:
               /* target match */
               // Disckind dependent ==>    WriteSCReg_4B(rwTGTADDR, dvPCATargetAddress);           /* Write Target Time */
               // HAL_DVDE_TGTHDR == rwTGTADDR
               break;

            default:
               break;
        }
        if (mDiscKind.fDiscKind != eDVDRAMDisc)
        {
            send_msg5S(1,"EN_OPCIRQ");
            WRITE_REG(HAL_OPCREG_MAP->OPCINT, 0x00);
            /* enable OPC operation */
            HAL_OPC_EnableOPCMode((HAL_OPC_ModeT)opcMode); /* NOTE: explicit conversion due to mapping */

            /* Enable access to sram */
            LAL_SHARED_SRAM_Enable(LAL_SHARED_SRAM_CONFIG_STREAM);

            /* enable interrupts */
            ENABLE_OPC_INTERRUPTS();
        }

        Iram_BdWriteStatus = E_WRITE_STATUS_IDLE;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_Stop

    Description:    Stop stream operation.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_Stop(void)
{
    mStreamLocked = FALSE;
    mStreamStatus = LAL_STREAM_STATUS_IDLE;

    SendMsg80(DEBUG_STREAM_MSG,0x3FAA1B); //STREAM_Stop

    switch ( mStreamMode )
    {
       case LAL_STREAM_MODE_TIMER:
          SendMsg80(DEBUG_STREAM_MSG,0x3FAA18); //MODE_TIMER
          SendMsgCnB(DEBUG_STREAM_MSG,3,0x3FAA1A,B1B(READ_FIELD(HAL_OPC_WBLSTM)),B1B(READ_FIELD(HAL_OPC_OPCSTART)),B1B(READ_FIELD(HAL_OPC_OPCSIZE))); //MODE_TIMER
          /* disable STM frequency */
          HAL_SERVO_EnableSTMPulseFrequency(FALSE); //LAL_STREAM_TODO: call for STM mode only ?
          break;

       case LAL_STREAM_MODE_WOBBLE:
            send_msg80(DEBUG_STREAM_MSG,0x640110); //MODE_WOBBLE
          break;

       default:
          break;
    }
    HAL_OPC_Init();
    LAL_SHARED_SRAM_Enable(LAL_SHARED_SRAM_CONFIG_CLEAR);
    /* disable interrupts */
    DISABLE_OPC_INTERRUPTS();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_Abort

    Description:    Abort stream operation. A call to _Stop() is needed before stream can be restarted.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_Abort(void)
{
    if (mStreamStatus == LAL_STREAM_STATUS_BUSY)
    {
        SendMsg80(DEBUG_STREAM_MSG,0x3FAA19); //STREAM_Abort

        mStreamStatus = LAL_STREAM_STATUS_ERROR;
        mStreamError = LAL_STREAM_ERROR_ABORT;
        switch ( mStreamMode )
        {
           case LAL_STREAM_MODE_TIMER:
              SendMsg80(DEBUG_STREAM_MSG,0x3FAA18); //MODE_TIMER
              /* disable STM frequency */
              HAL_SERVO_EnableSTMPulseFrequency(FALSE); //LAL_STREAM_TODO: call for STM mode only ?
              break;

           case LAL_STREAM_MODE_WOBBLE:
              send_msg80(DEBUG_STREAM_MSG,0x640110); //MODE_WOBBLE
              break;

           default:
              break;
        }
        LAL_SHARED_SRAM_Enable(LAL_SHARED_SRAM_CONFIG_CLEAR);
        /* disable interrupts */
        DISABLE_OPC_INTERRUPTS();
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_GetStatus

    Description:    Inspect the status of the stream operation.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The status of the stream operation = {IDLE, BUSY, READY, ERROR}
-----------------------------------------------------------------------------------------------------*/
LAL_STREAM_StatusT LAL_STREAM_GetStatus(void)
{
    send_msg80(DEBUG_STREAM_MSG,0x640006); //LAL_STREAM_GetStatus
    send_msg80(DEBUG_STREAM_MSG,0x640300+mStreamStatus); //LAL_STREAM_STATUS_xxx

    switch (mStreamStatus)
    {
        case LAL_STREAM_STATUS_BUSY:
            if (HAL_OPC_IsInterruptSet(HAL_OPC_INTERRUPT_RATE))
            {
                mStreamStatus = LAL_STREAM_STATUS_ERROR;
                mStreamError = LAL_STREAM_ERROR_SAMPLING;
            }
            else if (HAL_OPC_IsInterruptSet(HAL_OPC_INTERRUPT_STP))
            {
                mStreamStatus = LAL_STREAM_STATUS_READY;
            }
            else if ( Iram_BdWriteStatus == E_WRITE_STATUS_START_ERROR )
            {
                mStreamStatus = LAL_STREAM_STATUS_ERROR;
                mStreamError = LAL_STREAM_ERROR_TARGET_MATCH_MISSED;
            }
            break;

        case LAL_STREAM_STATUS_IDLE:
        case LAL_STREAM_STATUS_READY:
        case LAL_STREAM_STATUS_ERROR:
            break;
    }

    return (mStreamStatus);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_GetNrOfSamples

    Description:    Inspect the number of samples actually available in SRAM after stream operation.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The number of samples.
-----------------------------------------------------------------------------------------------------*/
USHORT LAL_STREAM_GetNrOfSamples(void)
{
    USHORT samples;

    //LAL_STREAM_TODO: How to get the nr of samples from the HW ?
    send_msg80(DEBUG_STREAM_MSG,0x640007); //STREAM_GetNrOfSamples

    switch ( mStreamMode )
    {
        case LAL_STREAM_MODE_TIMER:
           samples = ( (USHORT) 1 << READ_FIELD(HAL_OPC_SAMPNUM) );
           break;

        case LAL_STREAM_MODE_WOBBLE:
           samples = ( (USHORT) 1 << READ_FIELD(HAL_OPC_SAMPNUM) ) * READ_FIELD(HAL_OPC_OPCTRAMT) / ( 1 << READ_FIELD(HAL_OPC_OPCAVG) );
           break;

        default:
           samples = 0;
           DEBUG_ASSERT(0, eASSERT_FUNCTION_NOT_IMPLEMENTED);
           break;
    }

    return samples;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_PrintWobbleSamples

    Description:    Print all samples

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The number of samples.
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_PrintSamples(void)
{
    USHORT i;

    USHORT Channel[3];
    //SHORT Beta;
    SHORT DataCalf;
    SHORT DataA1;
    SHORT DataA2;
    USHORT DataJitter;
    USHORT PowerStep, LastPowerStep;
    USHORT NrOfSamples = LAL_STREAM_GetNrOfSamples();

    LastPowerStep = 0;

    for (i= 0; i < NrOfSamples; i++)
    {
        switch ( mStreamMode )
        {
           case LAL_STREAM_MODE_TIMER:
              Channel[0] = LAL_STREAM_DATA_CHANNEL1(i);
              Channel[1] = LAL_STREAM_DATA_CHANNEL2(i);
              Channel[2] = LAL_STREAM_DATA_CHANNEL3(i);
              PowerStep = LAL_STREAM_DATA_SAMPLE(i);
              if ( LastPowerStep != PowerStep )
              {
                  SendMsgCn(DEBUG_STREAM_MSG,2,0x640200,A2B(PowerStep));  //PowerStep
                  LastPowerStep = PowerStep;
              }
#if 0
              send_msg84L(1,0x64020C,Channel[0]); //CH 1
              DelaymS(1);
              send_msg84L(1,0x64020D,Channel[1]); //CH 2
              DelaymS(1);
              send_msg84L(1,0x64020E,Channel[2]); //CH 3
              DelaymS(1);
#else
              send_msg5SValue(DEBUG_STREAM_MSG,"data addr",4,(ULONG)&(HAL_OPC_pSRAM[i].data[0]));
              send_msg5SValue(DEBUG_STREAM_MSG,"data0",1,HAL_OPC_pSRAM[i].data[0]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data1",1,HAL_OPC_pSRAM[i].data[1]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data2",1,HAL_OPC_pSRAM[i].data[2]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data3",1,HAL_OPC_pSRAM[i].data[3]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data4",1,HAL_OPC_pSRAM[i].data[4]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data5",1,HAL_OPC_pSRAM[i].data[5]);
#endif
              break;

           case LAL_STREAM_MODE_WOBBLE:
              DataCalf = LAL_STREAM_DATA_CALF(i);
              DataA1 = LAL_STREAM_DATA_A1(i);
              DataA2 = LAL_STREAM_DATA_A2(i);
              DataJitter = LAL_STREAM_DATA_JITTER(i);
              PowerStep = LAL_STREAM_DATA_POWERSTEP(i);
              if ( LastPowerStep != PowerStep )
              {
                  SendMsgCn(DEBUG_STREAM_MSG,2,0x640200,A2B(PowerStep));  //PowerStep
                  LastPowerStep = PowerStep;
              }
#if 0
              send_msg84L(DEBUG_STREAM_MSG,0x640201,DataA1);     //DataA1
              send_msg84L(DEBUG_STREAM_MSG,0x640202,DataA2);     //DataA2
              send_msg84L(DEBUG_STREAM_MSG,0x640203,DataCalf);   //Calf
              send_msg84L(DEBUG_STREAM_MSG,0x640204,DataJitter); //RawJitter
              send_msg84L(DEBUG_STREAM_MSG,0x640208,convertRTJitter(8 * DataJitter )); //Jitter (0.1%)
              Beta = HAL_DFE_CalculateBetaX10(DataA1, DataA2, DataCalf );
              send_msg84L(DEBUG_STREAM_MSG,0x64020A,Beta);       //Beta
#else
              send_msg5SValue(DEBUG_STREAM_MSG,"data addr",4,(ULONG)&(HAL_OPC_pSRAM[i].data[0]));
              send_msg5SValue(DEBUG_STREAM_MSG,"data0",1,HAL_OPC_pSRAM[i].data[0]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data1",1,HAL_OPC_pSRAM[i].data[1]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data2",1,HAL_OPC_pSRAM[i].data[2]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data3",1,HAL_OPC_pSRAM[i].data[3]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data4",1,HAL_OPC_pSRAM[i].data[4]);
              send_msg5SValue(DEBUG_STREAM_MSG,"data5",1,HAL_OPC_pSRAM[i].data[5]);
#endif
              break;

           default:
              break;
        }
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_GetError

    Description:    Inspect the stream error.
                    A call to _GetStatus() is needed before this function can be used.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        The stream error = {OK, ERROR_SAMPLING, ERROR_ABORT}
-----------------------------------------------------------------------------------------------------*/
LAL_STREAM_ErrorT LAL_STREAM_GetError(void)
{
    send_msg80(DEBUG_STREAM_MSG,0x640008); //LAL_STREAM_GetError
    return (mStreamError);
}

#if (STREAM_STM == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_STREAM_ANALYSE

    Description:    Analyse SRAM data and calculate FEamp, BSamp, SRFOamp during ramping

    Inputs:         None
    Outputs:
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_STREAM_Analyse(eRampMeasType type, USHORT samples, USHORT frequency, BYTE direction)
{

    BYTE    stage, min_srfo_width;
    SHORT   local_max_index,srfo_peak, srfo_bottom, fe_peak, fe_bottom, bs_peak, bs_bottom;
    SHORT   srfo_cur, fe_cur, bs_cur;
    SHORT   main_thr, subst_thr;
    USHORT  i,srfo_index,buf_start,cur_index;
    LONG    srfo_sum;

    if (samples < frequency / 500) return;

    streamMeasVar.FEpp[0] = 0;
    streamMeasVar.BSpp[0] = 0;
    streamMeasVar.SRFOpp[0] = 0;
    streamMeasVar.LayerTime[0] = 0;
    streamMeasVar.FEpp[1] = 0;
    streamMeasVar.BSpp[1] = 0;
    streamMeasVar.SRFOpp[1] = 0;
    streamMeasVar.LayerTime[1] = 0;
    streamMeasVar.FEpp[2] = 0;
    streamMeasVar.BSpp[2] = 0;
    streamMeasVar.SRFOpp[2] = 0;
    streamMeasVar.LayerTime[2] = 0;
    streamMeasVar.SubstSRFO = 0;
    streamMeasVar.SubstTime = 0;

    if (samples > 1024)
    {
        buf_start = samples - 1024;
        samples = 1024;
    }
    else
    {
        buf_start = 0;
    }
    samples -= (frequency / 1000 + 1);//ignore last 1ms samples

    streamMeasVar.LayerFoo = svoCalMisc2Var.fooos;
    srfo_peak = fe_peak = bs_peak = -32768;
    srfo_bottom = fe_bottom = bs_bottom = 32767;

    switch (type)
    {
        case eRampSTM:
            subst_thr = 0x0F;//COEF_GetValue(AFE_CALC_STM_PRE_TH) ;
            main_thr = 0x0F;   //COEF_GetValue(AFE_CALC_STM_MAIN_TH);
            min_srfo_width = frequency / 3000 + 1;//about 333us
            stage = 0xF0;
            break;

        case eRampFEpp:
            main_thr = 0x0F; //COEF_GetValue(AFE_CALC_STM_MAIN_TH);
            min_srfo_width = frequency / 3000 + 1;//about 333us
            stage = 0;
            break;

        case eRampFEBG:
            main_thr = 0x0F; //COEF_GetValue(AFE_CALC_STM_MAIN_TH);
            min_srfo_width = frequency / 3000 + 1;//about 333us
            stage = 0;
            break;
    }
    ramp_stop();
    srfo_sum = 0;
    for (i = (frequency / 1000 + 1); i < (frequency / 1000 + 2) + 2 * min_srfo_width; i++)
    {
        cur_index = (i + buf_start) % 1024;
        srfo_sum += (LAL_STREAM_DATA_CHANNEL1(cur_index) - svoRampMeasVar.refSRFO);
    }

    for (i = (frequency / 1000 + 1) + min_srfo_width; i < samples - min_srfo_width; i++)
    {
        cur_index = (i + buf_start) % 1024;
        srfo_cur = LAL_STREAM_DATA_CHANNEL1(cur_index) - svoRampMeasVar.refSRFO;  //SRFO
        if (srfo_cur < 0) srfo_cur = 0;
        fe_cur = LAL_STREAM_DATA_CHANNEL2(cur_index) - svoRampMeasVar.refFE; //FE
        bs_cur = LAL_STREAM_DATA_CHANNEL3(cur_index);    //BS
        srfo_sum += LAL_STREAM_DATA_CHANNEL1(cur_index + min_srfo_width);
        srfo_sum -= LAL_STREAM_DATA_CHANNEL1(cur_index - min_srfo_width - 1);
        if (srfo_cur > main_thr)
        {
            //if ((srfo_cur + svoRampMeasVar.refSRFO - LAL_STREAM_DATA_CHANNEL1(cur_index-min_srfo_width)) * (srfo_cur + svoRampMeasVar.refSRFO - LAL_STREAM_DATA_CHANNEL1(cur_index+min_srfo_width)) > (srfo_cur * srfo_cur / 8))
            if (ABS(srfo_cur * (2 * min_srfo_width + 1) - srfo_sum) > srfo_sum / 4)
            {
                //srfo_cur = (LAL_STREAM_DATA_CHANNEL1(cur_index-min_srfo_width) + LAL_STREAM_DATA_CHANNEL1(cur_index+min_srfo_width)) / 2 - svoRampMeasVar.refSRFO;
                srfo_cur = srfo_sum / (2 * min_srfo_width + 1);
                fe_cur = fe_cur * srfo_cur / (LAL_STREAM_DATA_CHANNEL1(cur_index) - svoRampMeasVar.refSRFO);
                bs_cur = (bs_cur - bs_bottom) * srfo_cur / (LAL_STREAM_DATA_CHANNEL1(cur_index) - svoRampMeasVar.refSRFO) + bs_bottom;
            }
        }

        if (srfo_cur > srfo_peak)
        {
            srfo_peak = srfo_cur;
            local_max_index = (i + buf_start);
        }
        else if (srfo_cur < srfo_bottom)
        {
            srfo_bottom = srfo_cur;
        }

        if(fe_cur > fe_peak)
        {
            fe_peak = fe_cur;
        }
        else if(fe_cur < fe_bottom)
        {
            fe_bottom = fe_cur;
        }

        if (bs_cur > bs_peak)
        {
            bs_peak = bs_cur;
        }
        if (bs_cur < bs_bottom)
        {
            bs_bottom = bs_cur;
        }

        switch(stage)
        {
            case 0xF0://waiting for substrate
                if (srfo_cur > subst_thr)
                {
                    srfo_index = i;
                    srfo_peak = 0;
                    stage = 0xF1;
                    send_msg74(1,0x5f0024,i);
                }
                break;

            case 0xF1://waiting for SRFO peak of substrate
                if ((srfo_peak - srfo_cur) * 2 > (srfo_peak - srfo_bottom)) //0.5
                {
                    if (i <= srfo_index + min_srfo_width)
                    {
                        stage = 0xF0;
                    }
                    else
                    {
                        streamMeasVar.SubstSRFO = srfo_peak;
                        streamMeasVar.SubstTime = local_max_index;
                        streamMeasVar.LayerTime[0] = local_max_index;
                        streamMeasVar.LayerTime[1] = local_max_index;
                        send_msg74(1,0x5F0025,i);
                        streamMeasVar.SubstFEpp = fe_peak - fe_bottom;
                        srfo_bottom = 32767;
                        stage = 0;
                    }
                }
                break;

            case 0:
                if (srfo_cur - srfo_bottom > main_thr)
                {
                    srfo_index = i;
                    srfo_peak = fe_peak = bs_peak = 0;
                    fe_bottom = 32767;
                    stage = 1;
                }
                break;

            case 1:
                if ((srfo_peak - srfo_cur) * 10 > (srfo_peak - srfo_bottom) * 3) //0.3
                {
                    if (i > srfo_index + min_srfo_width)
                    {
                        if (streamMeasVar.SRFOpp[0] == 0)
                        {
                            streamMeasVar.SRFOpp[0] = srfo_peak;
                            streamMeasVar.FEpp[0] = fe_peak - fe_bottom;
                            streamMeasVar.BSpp[0] = bs_peak - bs_bottom;
                            streamMeasVar.LayerTime[0] = local_max_index;
                        }
                        else if (streamMeasVar.SRFOpp[1] == 0)
                        {
                            streamMeasVar.SRFOpp[1] = srfo_peak;
                            streamMeasVar.FEpp[1] = fe_peak - fe_bottom;
                            streamMeasVar.BSpp[1] = bs_peak - bs_bottom;
                            streamMeasVar.LayerTime[1] = local_max_index;
                        }
                        else if (streamMeasVar.SRFOpp[2] == 0)
                        {
                            streamMeasVar.SRFOpp[2] = srfo_peak;
                            streamMeasVar.FEpp[2] = fe_peak - fe_bottom;
                            streamMeasVar.BSpp[2] = bs_peak - bs_bottom;
                            streamMeasVar.LayerTime[2] = local_max_index;
                        }
                    }
                    srfo_bottom = 32767;
                    stage = 0;
                }
                break;
        }
        //if (srfo_cur > 0x0f)
        {
            if (direction)
                SendMsgCnB(SHOW_DEBUG_MSG,6,0x316301,B2B(LAL_STREAM_DATA_CHANNEL2(cur_index)-svoRampMeasVar.refFE),
                                                     B2B(LAL_STREAM_DATA_CHANNEL3(cur_index)-svoRampMeasVar.refBS),
                                                     B2B(LAL_STREAM_DATA_CHANNEL1(cur_index)-svoRampMeasVar.refSRFO));
            else
                SendMsgCnB(SHOW_DEBUG_MSG,6,0x316302,B2B(LAL_STREAM_DATA_CHANNEL2(cur_index)-svoRampMeasVar.refFE),
                                                     B2B(LAL_STREAM_DATA_CHANNEL3(cur_index)-svoRampMeasVar.refBS),
                                                     B2B(LAL_STREAM_DATA_CHANNEL1(cur_index)-svoRampMeasVar.refSRFO));
            WaitTimer(1);
        }
    }

#if 0
    ramp_stop();
    if (type != eRampSTM)
    {
        streamMeasVar.SubstTime = streamMeasVar.LayerTime[0];
    }
    if (streamMeasVar.LayerTime[2] == 0)
    {
        if (streamMeasVar.LayerTime[1] != 0)
        {
            streamMeasVar.LayerTime[2] = streamMeasVar.LayerTime[1];
        }
        else
        {
            streamMeasVar.LayerTime[2] = streamMeasVar.LayerTime[0];
        }
    }
    for (i = streamMeasVar.SubstTime - 30; i < streamMeasVar.LayerTime[2] + 30; i++)
    {
//        send_msgA2(SHOW_DEBUG_MSG,0x64020C,LAL_STREAM_DATA_CHANNEL1(i)); //CH 1
//        send_msgA2(SHOW_DEBUG_MSG,0x64020D,LAL_STREAM_DATA_CHANNEL2(i)); //CH 2
//        send_msgA2(SHOW_DEBUG_MSG,0x64020E,LAL_STREAM_DATA_CHANNEL3(i)); //CH 3
        SendMsgCnB(SHOW_DEBUG_MSG,6,0x31630B,B2B(LAL_STREAM_DATA_CHANNEL1(i)),
                                             B2B(LAL_STREAM_DATA_CHANNEL2(i)),
                                             B2B(LAL_STREAM_DATA_CHANNEL3(i)));

        WaitTimer(2);
    }
#endif

    if (streamMeasVar.SRFOpp[2] > 0)
    {
        if ((streamMeasVar.LayerTime[2] - streamMeasVar.LayerTime[1]) < (streamMeasVar.LayerTime[1] - streamMeasVar.LayerTime[0]))
        {
            streamMeasVar.FEpp[0] = streamMeasVar.FEpp[1];
            streamMeasVar.BSpp[0] = streamMeasVar.BSpp[1];
            streamMeasVar.SRFOpp[0] = streamMeasVar.SRFOpp[1];
            streamMeasVar.LayerTime[0] = streamMeasVar.LayerTime[1];
            streamMeasVar.FEpp[1] = streamMeasVar.FEpp[2];
            streamMeasVar.BSpp[1] = streamMeasVar.BSpp[2];
            streamMeasVar.SRFOpp[1] = streamMeasVar.SRFOpp[2];
            streamMeasVar.LayerTime[1] = streamMeasVar.LayerTime[2];
        }
        streamMeasVar.FEpp[2] = streamMeasVar.BSpp[2] = streamMeasVar.SRFOpp[2] = streamMeasVar.LayerTime[2] = 0;
    }

    if ((streamMeasVar.SRFOpp[1] > 0) && ((streamMeasVar.LayerTime[1]-streamMeasVar.LayerTime[0]) * 1000 > frequency * LAYER_DIFF_T))
    {
        if (streamMeasVar.FEpp[1] > streamMeasVar.FEpp[0])
        {
            streamMeasVar.FEpp[0] = streamMeasVar.FEpp[1];
            streamMeasVar.BSpp[0] = streamMeasVar.BSpp[1];
            streamMeasVar.SRFOpp[0] = streamMeasVar.SRFOpp[1];
            streamMeasVar.LayerTime[0] = streamMeasVar.LayerTime[1];
        }
        streamMeasVar.FEpp[1] = streamMeasVar.BSpp[1] = streamMeasVar.SRFOpp[1] = streamMeasVar.LayerTime[1] = 0;
    }

    svoRampMeasVar.SubstSRFO = streamMeasVar.SubstSRFO;
    svoRampMeasVar.SubstFoo = streamMeasVar.RampStartfoo + RoundDivide(streamMeasVar.SubstTime * ((1000 / 8) * FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x800), frequency * 0x100);
    svoRampMeasVar.FEpp[0] = streamMeasVar.FEpp[0];
    svoRampMeasVar.BSpp[0] = streamMeasVar.BSpp[0];
    svoRampMeasVar.SRFOpp[0] = streamMeasVar.SRFOpp[0];
    svoRampMeasVar.LayerTime[0] = RoundDivide((streamMeasVar.LayerTime[0] - streamMeasVar.SubstTime) * 1000, frequency);
    svoRampMeasVar.FEpp[1] = streamMeasVar.FEpp[1];
    svoRampMeasVar.BSpp[1] = streamMeasVar.BSpp[1];
    svoRampMeasVar.SRFOpp[1] = streamMeasVar.SRFOpp[1];
    svoRampMeasVar.LayerTime[1] = RoundDivide((streamMeasVar.LayerTime[1] - streamMeasVar.SubstTime) * 1000, frequency);


    if (streamMeasVar.FEpp[1] > 0)
    {
        streamMeasVar.LayerFoo = streamMeasVar.RampStartfoo + RoundDivide(streamMeasVar.LayerTime[1] * ((1000 / 8) * FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x800), frequency * 0x100);
    }
    else if (streamMeasVar.FEpp[0] > 0)
    {
        streamMeasVar.LayerFoo = streamMeasVar.RampStartfoo + RoundDivide(streamMeasVar.LayerTime[0] * ((1000 / 8) * FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x800), frequency * 0x100);
    }

    svoRampMeasVar.LayerFoo = streamMeasVar.LayerFoo;
}
#endif

#endif//STREAM_STM


/*****************************************************************************
*                       End Of File
*****************************************************************************/
