/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2008 Sunext Design, INC.
*                 All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
* $Revision: 213 $
* $Date: 11/03/17 12:02p $
*
* Description:
*   This file contains functions for the DFE subsystem and includes
*   the following:
*          HAL_DFE_SetIIRHPFBw()
*          HAL_DFE_SetMAvHPFBw()
*          HAL_DFE_PresetPLLFreq()
*          HAL_DFE_ReadJitterx10()
*          HAL_DFE_CalibrateDiffRfGain()
*          HAL_DFE_Init()
*
* Notes:
*
-----------------------------------------------------------------------------------------------------*/
#include ".\common\globtype.h"
#include ".\oem\oem_info.h"
#include ".\oem\coef_values.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\vu_cfg.h"
#include ".\player\plrdb.h"
#include ".\AL\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_dfe.h"
#include ".\AL\HAL_dfe.h"
#include ".\AL\HAL_afe.h"
#if (BD_ENABLE == 1)
#include ".\AL\REG_bd_codec.h"
#endif //(BD_ENABLE == 1)
#include ".\servo\Svo_lib.h"
#include ".\servo\SvoVar.H"
#include ".\servo\svo_calib.h"
#include ".\Common\ComMath.h"

#if (BD_ENABLE == 1)
extern BOOL BDR_RE_PICReading;
#endif //(BD_ENABLE == 1)
/*** Local defines ***/

/* local defines for HAL_DFE_SetIIRHPFBw */

#define TWO_TO_POWER_10 (1<<10)
#define TWO_TO_POWER_11 2048 //((2*2*2*2*2*2*2*2*2*2*2))

/* local defines for HAL_DFE_SetMAvHPFBw() */
#define OUT_OF_RANGE 0xFFFF
#define SCALE_FACTOR 10000

typedef enum {
    NRSMPL_104,
    NRSMPL_128,
    NRSMPL_172,
    NRSMPL_208,
    NRSMPL_232,
    NRSMPL_265,
    NRSMPL_512,
    BYPASS_SMPL_SETTING
}NrSamplesT;

typedef struct {
    USHORT CuttOffRatio;   // scaled to 1/SCALE_FACTOR
    USHORT NrSamples;      // enum NrSamplesT
} CutoffRatioToNrSamplesT;


/* local defines for HAL_DFE_PresetPLLFreq() */

#define NR_SMPLS_FOR_AVG    10
#define OFFSET_NEG_LIMIT    128
#define GAIN_RESOLUTIONX2   1

#define MAX_AGC_DYN_GAIN_ABS    31
#define MIN_AGC_DYN_GAIN_ABS    0
#define MAX_AOC_DYN_OFFSETDIV4  127
#define MIN_AOC_DYN_OFFSETDIV4  -128

#if (MAVHPF_FOR_BDROM_FINGER_PRINT==1)
#define IIR_HPF_BW_BD_FINGER  0
#define IIR_HPF_BW_DVD_FINGER 0
#define IIR_HPF_BW_CD_FINGER    0
#endif//(MAVHPF_FOR_BDROM_FINGER_PRINT==1)

extern ULONG mDfeInClkKHz;   // the DFE in Clock value in kHz
/*** Local static variables */

/* local defines for HAL_DFE_SetMAvHPFBw() */
static const CutoffRatioToNrSamplesT  CutoffRatioToNrSamples [] = {
    {(USHORT)(0.0308 * SCALE_FACTOR), (USHORT)NRSMPL_104},
    {(USHORT)(0.0244 * SCALE_FACTOR), (USHORT)NRSMPL_128},
    {(USHORT)(0.0180 * SCALE_FACTOR), (USHORT)NRSMPL_172},
    {(USHORT)(0.0152 * SCALE_FACTOR), (USHORT)NRSMPL_208},
    {(USHORT)(0.0136 * SCALE_FACTOR), (USHORT)NRSMPL_232},
    {(USHORT)(0.0124 * SCALE_FACTOR), (USHORT)NRSMPL_265},
    {(USHORT)(0.0062 * SCALE_FACTOR), (USHORT)NRSMPL_512},
    {(USHORT)OUT_OF_RANGE           , (USHORT)BYPASS_SMPL_SETTING}
};

#if ((EN_ADEQ == 1)||(EN_BD_ADEQ == 1))
/* Adaptive EQ partial response coefficient */
//0.2  : [0.1583    0.2196    0.2441    0.2196    0.1583]
BYTE const ADEC_020[5] = {0x05, 0x07, 0x08, 0x07, 0x05};//*32
//0.25 : [0.1358    0.2290    0.2704    0.2290    0.1358]
BYTE const ADEC_025[5] = {0x04, 0x07, 0x09, 0x07, 0x04};//*32
//0.3  : [0.1101    0.2384    0.3032    0.2384    0.1101]
BYTE const ADEC_030[5] = {0x04, 0x08, 0x0A, 0x08, 0x04};//*32
//0.35 : [0.0831    0.2459    0.3419    0.2459    0.0831]
BYTE const ADEC_035[5] = {0x03, 0x08, 0x0B, 0x08, 0x03};//*32
//
BYTE const ADEC_035_64[5] = {0x05, 0x10, 0x16, 0x10, 0x05};//*64
//0.4  : [0.0578    0.2496    0.3851    0.2496    0.0578]
BYTE const ADEC_040[5] = {0x02, 0x08, 0x0c, 0x08, 0x02};//*32
//0.45 : [0.0373    0.2475    0.4303    0.2475    0.0373]
BYTE const ADEC_045[5] = {0x01, 0x08, 0x0e, 0x08, 0x01};//*32
//0.5  : [0.0239    0.2384    0.4753    0.2384    0.0239]
BYTE const ADEC_050[5] = {0x01, 0x08, 0x0F, 0x08, 0x01};//*32
//0.55 : [0.0182    0.2229    0.5179    0.2229    0.0182]
BYTE const ADEC_055[5] = {0x01, 0x07, 0x11, 0x07, 0x01};//*32
//0.6:   [0.0188    0.2022    0.5580    0.2022    0.0188]
BYTE const ADEC_060[5] = {0x01, 0x06, 0x12, 0x06, 0x01};//*32
//0.65:  [0.0228    0.1787    0.5969    0.1787    0.0228]
BYTE const ADEC_065[5] = {0x01, 0x06, 0x13, 0x06, 0x01};//*32
#endif


/*** exported defines ***/

#if 0//currently not be use
#if (CHIP_REV >= 0xC0)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetIIRBETABw ()

    Description:    Sets first order IIR HF high pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz  fc*2/3=18666=0x48EA
                                         BD     84000 Hz  fc*2/3=56000=0xDAC0
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_IIRBETABW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetIIRBETABw (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10)
{
    ULONG Fc;
    ULONG PI_SCALED;
    LONG  tmp;

    {
        Fc = (HfHighPassFcN1Hz * OverspeedX10); //tuning HfHighPassFcN1Hz in coef table
        PI_SCALED = (ULONG)(PI * SCALE_FACTOR);
        tmp = (((((((Fc * 20)/mDfeInClkKHz)*TWO_TO_POWER_11)/10000)*PI_SCALED)/10)/SCALE_FACTOR);
        tmp = tmp > 0xFF? 0xFF : tmp;
        tmp = tmp < 0? 0x00: tmp;
    }
    WRITE_FIELD(HAL_DFE_IIRBETABW, (BYTE)tmp);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetBETAHIBW ()

    Description:    Sets first order IIR HF high pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz  fc*2/3=18666=0x48EA
                                         BD     84000 Hz  fc*2/3=56000=0xDAC0
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_BETAHIBW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetBETAHIBW (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10)
{
    ULONG Fc;
    ULONG PI_SCALED;
    LONG  tmp;

    {
        Fc = (HfHighPassFcN1Hz * OverspeedX10); //tuning HfHighPassFcN1Hz in coef table
        PI_SCALED = (ULONG)(PI * SCALE_FACTOR);
        tmp = (((((((Fc * 20)/mDfeInClkKHz)*TWO_TO_POWER_11)/10000)*PI_SCALED)/10)/SCALE_FACTOR);
        tmp = tmp > 0xFF? 0xFF : tmp;
        tmp = tmp < 0? 0x00: tmp;
    }
    WRITE_FIELD(HAL_DFE_BETAHIBW, (BYTE)tmp);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetHPFHIBW ()

    Description:    Sets first order IIR HF high pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz  fc*2/3=18666=0x48EA
                                         BD     84000 Hz  fc*2/3=56000=0xDAC0
                                         HDDVD ~84000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         CD     20..520
                                         DVD    20..200
                                         BD     10..140
                                         HDDVD  10..100
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_HPFHIBW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetHPFHIBW (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10)
{
    ULONG Fc;
    ULONG PI_SCALED;
    LONG  tmp;

    {
        Fc = (HfHighPassFcN1Hz * OverspeedX10); //tuning HfHighPassFcN1Hz in coef table
        PI_SCALED = (ULONG)(PI * SCALE_FACTOR);
        tmp = (((((((Fc * 20)/mDfeInClkKHz)*TWO_TO_POWER_11)/10000)*PI_SCALED)/10)/SCALE_FACTOR);
        tmp = tmp > 0xFF? 0xFF : tmp;
        tmp = tmp < 0? 0x00: tmp;
    }
    WRITE_FIELD(HAL_DFE_HPFHIBW, (BYTE)tmp);
}
#endif //(CHIP_REV >= 0xC0)


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetBetaLPFBw()

    Description:    Sets first order beta HF low pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfLowpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
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
    Global Outputs: HAL_DFE_AMINLPFBW/HAL_DFE_AMAXLPFBW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetBetaLPFBw (ULONG HfLowPassFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz)
{
    ULONG Fc;
    ULONG PI_SCALED;
    LONG tmp;

    Fc = (HfLowPassFcN1Hz * OverspeedX10); //tuning HfHighPassFcN1Hz in coef table
    PI_SCALED = (ULONG)(PI * SCALE_FACTOR);
    tmp = (((((((Fc * 2)*TWO_TO_POWER_10/FDfeInClkKHz))/1000)*PI_SCALED)/10)/SCALE_FACTOR);
    tmp = tmp > 0x3F? 0x3F : tmp;
    tmp = tmp <= 0? 0x01: tmp;

    WRITE_FIELD(HAL_DFE_AMINLPFBW, (BYTE)tmp);
    WRITE_FIELD(HAL_DFE_AMAXLPFBW, (BYTE)tmp);
}

#endif //currently not be use


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetIIRHPFBw()

    Description:    Sets first order IIR HF high pass filter bandwidth, which is calculated by means of
                    overspeed.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         CD      3000 Hz
                                         DVD    28000 Hz  fc*2/3=18666=0x48EA
                                         BD     84000 Hz  fc*2/3=56000=0xDAC0
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
void HAL_DFE_SetIIRHPFBw (ULONG HfHighPassFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz)
{
    ULONG Fc;
    ULONG PI_SCALED;
    LONG tmp;

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif

#if (BD_ENABLE == 1)
    #if (MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    if ((mDiscKind.Bit.DiscStd == eDiscBD)&&(BDR_RE_PICReading != SET))
    {
        tmp = IIR_HPF_BW_BD_FINGER;
    }
    else
    #endif //(MAVHPF_FOR_BDROM_FINGER_PRINT==1)
#endif //(BD_ENABLE == 1)
#if (MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    if ((mDiscKind.Bit.DiscStd == eDiscDVD)&&(!DVDRAM))
    {
        tmp = IIR_HPF_BW_DVD_FINGER;
    }
    else if (mDiscKind.Bit.DiscStd == eDiscCD)
    {
        tmp = IIR_HPF_BW_CD_FINGER;
    }
    else
#endif//(MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    {
        Fc = (HfHighPassFcN1Hz * OverspeedX10); //tuning HfHighPassFcN1Hz in coef table
        PI_SCALED = (ULONG)(PI * SCALE_FACTOR);
        tmp = (((((((Fc * 20)/FDfeInClkKHz)*TWO_TO_POWER_11)/10000)*PI_SCALED)/10)/SCALE_FACTOR);
        tmp = tmp > 0xFF? 0xFF : tmp;
        tmp = tmp < 0? 0x00: tmp;
    }
    WRITE_FIELD(HAL_DFE_IIRHPFBW, (BYTE)tmp);

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_CalculateBetaX10()

    Description:    Calulates the beta value based on A1, A2 and CALF measurement

    Inputs:         A1 (A1pk)

                    A2 (A2pk)

                    CALF Offset

    Outputs:        None
    Global Inputs:
    Global Outputs: void
    Returns:        The calculated beta value in percentage x 10
-----------------------------------------------------------------------------------------------------*/
SHORT HAL_DFE_CalculateBetaX10(SHORT A1, SHORT A2, SHORT CALF)
{
    if (A1 == A2)
        return -1;
    return  ((1000 * ( A1 + A2 - ( 2 * CALF ) )) / ( A1 - A2 ));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetMovAvHPFBw()

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
-----------------------------------------------------------------------------------------------------*/
#if (HORROR_DISC_IMPROVEMENT == 1)
extern BYTE TryDFE_Param; //modify for DQA Disc to retry DFE parameter
extern BYTE         VCD_Try_Case;
#endif
void HAL_DFE_SetMovAvHPFBw (ULONG CoefFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz)
{
    ULONG Fc, CutOffRatio;
    USHORT i;

#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif

#if (BD_ENABLE == 1)
    if (BDR_RE_PICReading == SET)
    {
        WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_BYPASS);
    }
    else
    #if (MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    if (mDiscKind.Bit.DiscStd == eDiscBD)
    {
        WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);
    }
    else
    #endif //(MAVHPF_FOR_BDROM_FINGER_PRINT==1)
#endif //(BD_ENABLE == 1)
#if (MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    if ((mDiscKind.Bit.DiscStd == eDiscDVD)&&(!DVDRAM))
    {
        WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);
    }
    else if(mDiscKind.Bit.DiscStd == eDiscCD)
    {
        //== start == modify for DQA Disc to retry DFE parameter
#if (HORROR_DISC_IMPROVEMENT == 1)//TBD:disable Moving average HPF retry, need check later
        if((TryDFE_Param&0x02)==0x02)
            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);
    #if (PCB==815)
        else if(VCD_Try_Case==1)
            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_104_adc_samples);

        else if ((mDiscKind.Bit.Media == eMediaROM) && (St_cdrom_video == TRUE))
            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);

    #elif (((PCB == 710)||(PCB == 711)||(PCB == 817)) && (CEProduct==1))
//       else if(VCD_Try_Case==1)
//            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_104_adc_samples);

       else if ((mDiscKind.Bit.Media == eMediaROM) && (St_cdrom_video == TRUE))
            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);

    #endif
        else// if((TryDFE_Param&0x02)==0x00)
#endif
        WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);
        //== end == modify for DQA Disc to retry DFE parameter
    }
    else
#endif//(MAVHPF_FOR_BDROM_FINGER_PRINT==1)
    {
#if (DVD_RAM_READ == 1)
        if(DVDRAM)
        {
            WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);
        }
        else
#endif
        {
            Fc = CoefFcN1Hz * OverspeedX10;
            CutOffRatio = (((Fc/100)*SCALE_FACTOR)/FDfeInClkKHz)/100;

            for (i = 0; ;i++)
            {
               if (CutoffRatioToNrSamples[i].CuttOffRatio == OUT_OF_RANGE) break;
               if ((USHORT)CutOffRatio > CutoffRatioToNrSamples[i].CuttOffRatio) break;
            }
            WRITE_FIELD(HAL_DFE_MAHPFBW, CutoffRatioToNrSamples[i].NrSamples);
        }
    }

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetMovAvHPFBwHeader()

    Description:    Sets bandwidth of the moving average high pass filter for the header field. The bandwidth of this filter
                    is calculated based on the real overspeed factor. The bandwidth is disc and speed
                    dependent. This function is specific for DVDRAM disc.

    Inputs:         HfHighpassFcN1KHz -> cut off frequency for overspeed 1X in Hz units
                                         DVDRAM 28000 Hz
                    OverspeedX10      -> overspeed in tenths
                                         DVDRAM 20..160
                    FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         DVDRAM  60000 KHz
    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_MAHPFBW field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetMovAvHPFBwHeader (ULONG CoefFcN1Hz, USHORT OverspeedX10, ULONG FDfeInClkKHz)
{
    ULONG Fc, CutOffRatio;
    USHORT i;

    Fc = CoefFcN1Hz * OverspeedX10;
    CutOffRatio = (((Fc/100)*SCALE_FACTOR)/FDfeInClkKHz)/100;

    for (i = 0; ;i++)
    {
        if (CutoffRatioToNrSamples[i].CuttOffRatio == OUT_OF_RANGE) break;
        if ((USHORT)CutOffRatio > CutoffRatioToNrSamples[i].CuttOffRatio) break;
    }

    // Never turn off moving-average filter.
    i = CutoffRatioToNrSamples[i].NrSamples;
    if (i == BYPASS_SMPL_SETTING)
        i = NRSMPL_512;

    WRITE_FIELD(HAL_DFE_MAHPFBWH, i);
}
#endif

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
void HAL_DFE_EnDvdRamAnalizeMode()
{
    WRITE_FIELD(HAL_DFE_AGCON, 0);
    WRITE_FIELD(HAL_DFE_AOCON, 0);
    WRITE_FIELD(HAL_DFE_AGCSWONH, 0);
    WRITE_FIELD(HAL_DFE_AOCSWONH, 0);
    WRITE_FIELD(HAL_DFE_AGCDRCFG, 0);
    WRITE_FIELD(HAL_DFE_AOCDRCFG, 0);

    WRITE_FIELD(HAL_DFE_DRHFANA, 1);
}
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
    Global Outputs: HAL_DFE_INTEGRAT field
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_PresetPLLFreq(USHORT OverspeedX10, ULONG FDfeInClkKHz, BYTE TrackingMethod)
{
    ULONG PresetFreq;

    switch (mDiscKind.fDiscKind)
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
        case eLSDisc:
            PresetFreq = (((((((ULONG)CD_BITRATE_N1)/FDfeInClkKHz)*OverspeedX10)/10)*TWO_TO_POWER_13)/1000);
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDROMDLDisc:
            PresetFreq = (((((((ULONG)BD_BITRATE_N1)/1000)*TWO_TO_POWER_13)/FDfeInClkKHz)*OverspeedX10)/10);
            break;

        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            if (BDR_RE_PICReading == SET)
            {
                PresetFreq = (((((((ULONG)BD_BITRATE_N1)/1000)*TWO_TO_POWER_13)/6/FDfeInClkKHz)*OverspeedX10)/10);
            }
            else
            {
                PresetFreq = (((((((ULONG)BD_BITRATE_N1)/1000)*TWO_TO_POWER_13)/FDfeInClkKHz)*OverspeedX10)/10);
            }
            break;
#endif //(BD_ENABLE == 1)
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
            if ((svoMiscVar.HD_System_leadInOutFlag == HD_SYSTEM_LEAD_IN) ||
                (svoMiscVar.HD_System_leadInOutFlag == HD_SYSTEM_LEAD_OUT) ||
                (svoMiscVar.HD_System_leadInOutFlag == PRESET_HD_SYSTEM_LEAD_IN) ||
                (svoMiscVar.HD_System_leadInOutFlag == PRESET_HD_SYSTEM_LEAD_OUT))
            {
                PresetFreq = (((((((ULONG)HDDVD_EMBOSSED_BITRATE_N1)/FDfeInClkKHz)*OverspeedX10)/10)*TWO_TO_POWER_13)/1000);
            }
            else
            {
                PresetFreq = (((((((ULONG)HDDVD_DATA_BITRATE_N1)/FDfeInClkKHz)*OverspeedX10)/10)*TWO_TO_POWER_13)/1000);
            }
            break;
#endif /* (ENABLE_HDDVD == 1) */

        case eDVDROMDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
            PresetFreq = (((((((ULONG)DVD_BITRATE_N1)/FDfeInClkKHz)*OverspeedX10)/10)*TWO_TO_POWER_13)/1000);
            break;

#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            if (TrackingMethod == TRK_METHOD_DPD)
            {
                PresetFreq = (((((((ULONG)DVDRAM_EMBOSSED_BITRATE_N1)/1000)*TWO_TO_POWER_13)/FDfeInClkKHz)*OverspeedX10)/10);
            }
            else
            {
                PresetFreq = (((((((ULONG)DVDRAM_BITRATE_N1)/1000)*TWO_TO_POWER_13)/FDfeInClkKHz)*OverspeedX10)/10);
            }
            break;
#endif

        case eUnknownKind:
        default:
            //should never get here, maybe use assert?
            break;
    }
    SendMsgCn(DEBUG_DFE_MSG,4,0x400100,A4B(PresetFreq));//HFPLLPresetFreq
    WRITE_FIELD(HAL_DFE_INTEGRAT, (USHORT)PresetFreq);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ReadJitterx10()

    Description:    This function reads the fast pll jitter register and calculates the jitter in
                    tenths of a percent.

    Inputs:         None

    Outputs:        jitter in x0.1% (USHORT)
    Global Inputs:  HAL_DFE_FPLLJIT field(s). See note below
    Global Outputs:
    Returns:        jitter in x0.1% (USHORT)

    Note : READ_FIELD() does not work at the moment therefore use
               HAL_DFE_FPLLJIT_REGISTER
               HAL_DFE_FPLLJIT_MASK
               HAL_DFE_FPLLJIT_LSB
           to access field.
-----------------------------------------------------------------------------------------------------*/
USHORT HAL_DFE_ReadJitterX10(void)
{
    BYTE   i;
    USHORT JitMeas;

    JitMeas = 0;
    for (i = 0; i < 8; i++)
    {
        JitMeas += READ_FIELD(HAL_DFE_FPLLJIT);
    }

    return convertRTJitter(JitMeas);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_CalibrateDiffRfGain()

    Description:    Calibration of the diff RF gain
    Inputs:         TargetPeakPeakmV  : Target RF peak to peak value in ADC
                    ExpectedModX100   : Expected modulation of RF signal
    Outputs:        GainPeakPeakX2    : Expected gain, when there is RF signal, in 0.5dB
                    OffsetPeakPeak4mV : Expected offset, when there is RF signal, in 4 mV
    Global Inputs:  None
    Global Outputs: None
    Returns:        errcode_t
-----------------------------------------------------------------------------------------------------*/
#define RRF_MAX_OFFSET_SHIFT 4
#define RRF_MAX_GAIN_SHIFT   1
#define RRF_MAX_LOOP 10

#define MEAS_SUGGESTED_SETTINGS 1

#if (MEAS_SUGGESTED_SETTINGS == 1) && (PDIC_BASE_ON_SRFO == 1)
extern SHORT const PDICGainCD[PDIC_GAIN_MAX_CD];
extern SHORT const PDICGainDVD[PDIC_GAIN_MAX_DVD];
    #if (BD_ENABLE == 1)
extern SHORT const PDICGainBD[PDIC_GAIN_MAX_BD];
    #endif
#endif
errcode_t HAL_DFE_CalibrateDiffRfGain(USHORT TargetPeakPeakmV, BYTE ExpectedModX100, SBYTE* GainPeakPeakX2, SBYTE* OffsetPeakPeak4mV)
{
    LONG      temp;
    ad_dsp_t  rrf_signal;
    schar_return_t next_gnos;
    BYTE      i,RRFG_cur,modulation;
    BOOL      no_RF;
    SBYTE     RFOOS_cur;
    SHORT     RF_dark;
    errcode_t ErrCode;

    ErrCode = eNOERR;

    //---------------------------------------------------;
    // RRFpp calibration                                 ;
    //---------------------------------------------------;
    RRFG_cur = READ_FIELD(HAL_AFE_RFAGCREG);
    RFOOS_cur = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),8);

    svoRRFVar.ok_cnt = 0;
    svoRRFVar.adj_gn_pre = 0;
    modulation = ExpectedModX100;

    for (i = 0; i < RRF_MAX_LOOP; i++)
    {
        // SET_RF_GAIN = %4.2f (dB), SET_RF_OFFSET = %2d (mV)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x338301,F4B((RRFG_cur-16)/2.0),B2B(RFOOS_cur*4));
        rrf_signal.errcode = eNOERR;
        rrf_signal = readADCmVPerRevs(ONE_REVOLUTION, TRUE, RF_ADC);
        // RF_PP = %4d (mV), RF_mid = %3d (mV)
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,4,0x338307,A2B(rrf_signal.p2p_mv),A2B(rrf_signal.middle_mv));
        svoRRFVar.ok_cnt++;

        if (rrf_signal.errcode == eNOERR)
        {
            if (rrf_signal.top_saturated)
            {
                svoRRFVar.ok_cnt = 0;
                rrf_signal.top_mv += (TargetPeakPeakmV / 2);
                rrf_signal.p2p_mv = rrf_signal.top_mv - rrf_signal.bot_mv;
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                SendMsg80(SHOW_DEBUG_MSG,0x33FF0E); // RRF top saturation
						#if (Customer_A1 ==1)
								PositionHomeErrorCount++;
						#endif                
            }
            if (rrf_signal.bot_saturated)
            {
                svoRRFVar.ok_cnt = 0;
                rrf_signal.bot_mv -= (TargetPeakPeakmV / 2);
                rrf_signal.p2p_mv = rrf_signal.top_mv - rrf_signal.bot_mv;
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                SendMsg80(SHOW_DEBUG_MSG,0x33FF0F); // RRF bottom saturation
						#if (Customer_A1 ==1)
								PositionHomeErrorCount++;
						#endif                
            }

            //temp = dark_level
            temp = RoundDivide((LONG)svoRRFVar.step_size[RRFG_cur] * (RFOOS_cur - svoRRFVar.ofs_ldoff_cur),256);
#if (NEW_RFPOS_CALIB == 1)
            temp += svoRRFVar.rfagc_ref;
#endif
            if (rrf_signal.top_mv > (temp + 20))
            {
                modulation = rrf_signal.p2p_mv * 100 / (rrf_signal.top_mv - temp);
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33FF10); //OPU RF no output!
                modulation = ExpectedModX100;
            }
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x33FF1C,A1B(modulation)); //Modulation=%2d
            //temp = calculated RRFpp based on ExpectedModX100
            temp = (rrf_signal.top_mv - temp) * (ExpectedModX100) / 100;

            if (rrf_signal.p2p_mv < (temp / 3))
            {            
						#if (Customer_A1 ==1)
								PositionHomeErrorCount++;	
						#endif
                SendMsg80(SHOW_DEBUG_MSG,0x33FF11); //No RF signal
                no_RF = TRUE;
                rrf_signal.p2p_mv = temp;
#if (ENABLE_BDRLTH == 1)
                if (svoIramVar.LTHDisc == 1)
                {
                    rrf_signal.top_mv = rrf_signal.bot_mv + rrf_signal.p2p_mv;
                }
                else
#endif
                {
                    rrf_signal.bot_mv = rrf_signal.top_mv - rrf_signal.p2p_mv;
                }
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                modulation = ExpectedModX100;
            }
            else
            {
                no_RF = FALSE;
            }
        }
        else
        {
            ErrCode = rrf_signal.errcode;
        }

        if (ErrCode != eNOERR)
        {
            break;
        }

        temp = RRFG_cur;
        next_gnos = calAdjRRFGain(rrf_signal.p2p_mv,TargetPeakPeakmV,RRFG_cur);
        if ((next_gnos.val * svoRRFVar.adj_gn_pre < 0) && (-next_gnos.val == svoRRFVar.adj_gn_pre) && (svoRRFVar.ok_cnt >= 2))
        {
            if (next_gnos.val * svoRRFVar.adj_gn_pre == -1)
            {
                if (ABS(rrf_signal.p2p_mv-TargetPeakPeakmV) > ABS(svoRRFVar.RRFpp_pre-TargetPeakPeakmV))
                {
                    RRFG_cur += next_gnos.val;
                }
            }
            next_gnos.val /= 2;
        }
        svoRRFVar.adj_gn_pre = next_gnos.val;
        RRFG_cur += svoRRFVar.adj_gn_pre;
        WRITE_FIELD(HAL_AFE_RFAGCREG,RRFG_cur);
        svoRRFVar.RRFpp_pre = rrf_signal.p2p_mv;
#if (NEW_RFPOS_CALIB == 1)
        next_gnos = calAdjRRFOffset(rrf_signal.middle_mv,rrf_signal.p2p_mv,TargetPeakPeakmV,temp);
#else
        next_gnos = calAdjRRFOffset(rrf_signal.middle_mv,temp);
#endif
        temp = RFOOS_cur + next_gnos.val;

        if (temp >  127) temp =  127;
        if (temp < -127) temp = -127;

        RFOOS_cur = temp;
        WRITE_FIELD(HAL_AFE_RFAOCREG,calSBYTE2DacCount(RFOOS_cur,8));
        Delay100uS(2);

        if ((svoRRFVar.adj_gn_pre >= -RRF_MAX_GAIN_SHIFT) && (svoRRFVar.adj_gn_pre <= RRF_MAX_GAIN_SHIFT) &&
            (next_gnos.val >= -RRF_MAX_OFFSET_SHIFT) && (next_gnos.val <= RRF_MAX_OFFSET_SHIFT) && (svoRRFVar.ok_cnt >= 1))
        {
            rrf_signal.top_mv = rrf_signal.top_mv + next_gnos.val * svoRRFVar.step_size[RRFG_cur] / 256;
            break;
        }
    }
    *OffsetPeakPeak4mV = RFOOS_cur;
    *GainPeakPeakX2 = RRFG_cur;

    // RFAGCREG (dB) = %4.1f, RFAOCREG (mV) = %2d
    SendMsgCnB(SHOW_DEBUG_MSG,6,0x316694,F4B((RRFG_cur-16)/2.0),B2B(RFOOS_cur*4));
    RF_dark = RoundDivide((LONG)svoRRFVar.step_size[RRFG_cur] * (RFOOS_cur - svoRRFVar.ofs_ldoff_cur),256);
#if (NEW_RFPOS_CALIB == 1)
    RF_dark += svoRRFVar.rfagc_ref;
#endif
    if (no_RF)
    {
        ErrCode = eERR_RF_not_present;
    }
    else if (i == RRF_MAX_LOOP)
    {
        ErrCode = eLOOP_expire_calibrate;
    }
    else
    {
        svoRRFVar.modulation[Layer_Index] = modulation;
    }
    // RF: PP = %4d (mV), Top = %4d (mV), DARK = %5d (mV), Modulation = %2d (%)
    SendMsgCn(SHOW_DEBUG_MSG,7,0x316693,A2B(rrf_signal.p2p_mv),A2B(rrf_signal.top_mv),
       A2B(RF_dark),modulation);
#if (MEAS_SUGGESTED_SETTINGS == 1)
    {
        SHORT  gain_dBX10;
        float  RFSRFO_target;
    #if (PDIC_BASE_ON_SRFO == 1)
        USHORT pdic_srfo_target_max,pdic_srfo_target_min;
        BYTE   i,PdicGainMax,PdicGainMin;
        const SHORT *PdicGain;
    #endif

        rrf_signal = readADCmVPerRevs(ONE_REVOLUTION, TRUE, RF_ADC);
        gain_dBX10 = HAL_AFE_GetRfGaindBX10(READ_FIELD(HAL_AFE_RFINGN),READ_FIELD(HAL_AFE_RFGN));
        gain_dBX10 -= (svoRampMeasVar.SRFO_gain - svoCalMisc2Var.pdic_gn - svoCalMisc2Var.laser_gn);
        RFSRFO_target = (rrf_signal.top_mv - RF_dark) * svoRRFVar.step_size[0x10] / svoRRFVar.step_size[RRFG_cur];
        RFSRFO_target *= CalculateRateFromXdB(P10dB,-gain_dBX10);
        RFSRFO_target = (TargetPeakPeakmV * svoPrvtVar_avgSRFO[Layer_Index]) / RFSRFO_target;
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x31669A,F4B(RFSRFO_target));//suggested RfSrfoTargetX1000 = %.1f
    #if (PDIC_BASE_ON_SRFO == 1)
        pdic_srfo_target_max = RFSRFO_target * CalculateRateFromXdB(P10dB,-(RFINGN_0_dBX10+RFGN_0_dBX10));
        gain_dBX10 = COEF_GetValue(AFE_CALC_MbTargetX100) * 40 * CalculateRateFromXdB(P10dB,160);
        if (gain_dBX10 < pdic_srfo_target_max)
        {
            pdic_srfo_target_max = gain_dBX10;
        }

        switch (mDiscKind.Bit.DiscStd)
        {
        #if (BD_ENABLE == 1)
            case eDiscBD:
                PdicGain = PDICGainBD;
                PdicGainMax = PDIC_GAIN_MAX_BD;
                PdicGainMin = PDIC_GAIN_MIN_BD;
                break;
        #endif
            case eDiscDVD:
                PdicGain = PDICGainDVD;
                PdicGainMax = PDIC_GAIN_MAX_DVD;
                PdicGainMin = PDIC_GAIN_MIN_DVD;
                break;

            case eDiscCD:
                PdicGain = PDICGainCD;
                PdicGainMax = PDIC_GAIN_MAX_CD;
                PdicGainMin = PDIC_GAIN_MIN_CD;
                break;
        }
        gain_dBX10 = 0;
        for (i = PdicGainMin + 1; i < PdicGainMax; i++)
        {
            if ((PdicGain[i] - PdicGain[i-1]) > gain_dBX10)
            {
                gain_dBX10 = (PdicGain[i] - PdicGain[i-1]);
            }
        }
        #if (CHIP_REV >= 0xC0)
        pdic_srfo_target_min = RFSRFO_target * CalculateRateFromXdB(P10dB,gain_dBX10-(RFINGN_3_dBX10+RFGN_7_dBX10));
        #else
        pdic_srfo_target_min = RFSRFO_target * CalculateRateFromXdB(P10dB,gain_dBX10-(RFINGN_3_dBX10+RFGN_3_dBX10));
        #endif
        gain_dBX10 = COEF_GetValue(AFE_CALC_MbTargetX100) * 40 * CalculateRateFromXdB(P10dB,gain_dBX10-150);
        if (gain_dBX10 > pdic_srfo_target_min)
        {
            pdic_srfo_target_min = gain_dBX10;
        }
        SendMsgCn(SHOW_DEBUG_MSG,4,0x31669B,A2B(pdic_srfo_target_min),A2B(pdic_srfo_target_max));//suggested PDIC_SRFO_TARGET = %4u ~ %4u
    #endif
    }
#endif
    return(ErrCode);
}

#if (DVD_RAM_READ == 1)
errcode_t HAL_DFE_CalibrateRAMDiffRfGain(USHORT TargetPeakPeakmV, BYTE ExpectedModX100, SBYTE *GainPeakPeakX2, SBYTE *OffsetPeakPeak4mV)
{
    LONG      temp;
    ad_dsp_t  rrf_signal;
    schar_return_t next_gnos;
    BYTE      i,RRFG_cur,modulation;
    BOOL      no_RF;
    SBYTE     RFOOS_cur;
    SHORT     RF_dark;//20101014_02PL ==start== remove duplicate variables and messages
    errcode_t ErrCode;

    ErrCode = eNOERR;
    //---------------------------------------------------;
    // RRFpp calibration                                 ;
    //---------------------------------------------------;
    RRFG_cur = READ_FIELD(HAL_AFE_RFAGCREG);
    RFOOS_cur = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),8);

    svoRRFVar.ok_cnt = 0;
    svoRRFVar.adj_gn_pre = 0;
    svoRRFVar.RFpp_BlankTh = 0;
    modulation = ExpectedModX100;
    for (i = 0; i < RRF_MAX_LOOP; i++)
    {
        // SET_RF_GAIN = %4.2f (dB), SET_RF_OFFSET = %2d (mV)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x338301,F4B((RRFG_cur-16)/2.0),B2B(RFOOS_cur*4));

        rrf_signal = readRAMRfADCmVPerRevs(ONE_REVOLUTION, TRUE ,((svoIramVar.svoInState & TRN_MASK)==SVO_TRON) ?1 :0 ,svoRRFVar.RFpp_BlankTh);

        // RF_PP = %4d (mV), RF_mid = %3d (mV)
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,4,0x338307,A2B(rrf_signal.p2p_mv),A2B(rrf_signal.middle_mv));

        svoRRFVar.ok_cnt++;

        if (rrf_signal.errcode == eNOERR)
        {
            if (rrf_signal.top_saturated)
            {
                svoRRFVar.ok_cnt = 0;
                rrf_signal.top_mv += TargetPeakPeakmV;
                rrf_signal.p2p_mv = rrf_signal.top_mv - rrf_signal.bot_mv;
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                SendMsg80(SHOW_DEBUG_MSG,0x33FF0E);   // RRF top saturation
            }
            if (rrf_signal.bot_saturated)
            {
                svoRRFVar.ok_cnt = 0;
                rrf_signal.bot_mv -= TargetPeakPeakmV;
                rrf_signal.p2p_mv = rrf_signal.top_mv - rrf_signal.bot_mv;
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                SendMsg80(SHOW_DEBUG_MSG,0x33FF0F);   // RRF bottom saturation
            }

            //temp = dark_level
            temp = RoundDivide((LONG)svoRRFVar.step_size[RRFG_cur] * (RFOOS_cur - svoRRFVar.ofs_ldoff_cur),256);
#if (NEW_RFPOS_CALIB == 1)
            temp += svoRRFVar.rfagc_ref;
#endif
            if (rrf_signal.top_mv > temp + 20)
            {
                modulation = rrf_signal.p2p_mv * 100 / (rrf_signal.top_mv - temp);
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33FF10);     // OPU RF no output!
                modulation = ExpectedModX100;
            }
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x33FF1C,A1B(modulation)); // RF Modulation = %2d (%)

            //temp = calculated RRFpp based on ExpectedModX100
            temp = (rrf_signal.top_mv - temp) * (ExpectedModX100) / 100;

            // ExpectedModX100 is equal to 100 means that DVD-RAM header detection is executing.
            if (ExpectedModX100 == 100)
            {
                // During DVD-RAM header detection, always assume Vtop = V14.
                rrf_signal.p2p_mv = temp;
                rrf_signal.bot_mv = rrf_signal.top_mv - rrf_signal.p2p_mv;
                rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
            }
            else
            {
                if (rrf_signal.p2p_mv < temp/2)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x33FF11);   // No RF signal
                    no_RF = TRUE;
                    rrf_signal.p2p_mv = temp;
                    rrf_signal.bot_mv = rrf_signal.top_mv - rrf_signal.p2p_mv;
                    rrf_signal.middle_mv = (rrf_signal.top_mv + rrf_signal.bot_mv) / 2;
                    modulation = ExpectedModX100;
                }
                else
                {
                    no_RF = FALSE;
                }
                svoRRFVar.RFpp_BlankTh = (temp/2)* (1<<4) / (RF_ADC_IN_RANGE>>4);
            }
        }
        else
        {
            ErrCode = rrf_signal.errcode;
        }

        if (ErrCode != eNOERR)
        {
            break;
        }

        temp = RRFG_cur;
        next_gnos = calAdjRRFGain(rrf_signal.p2p_mv,TargetPeakPeakmV,RRFG_cur);
        if ((next_gnos.val * svoRRFVar.adj_gn_pre < 0) && (-next_gnos.val == svoRRFVar.adj_gn_pre) && (svoRRFVar.ok_cnt >= 2))
        {
            if (next_gnos.val * svoRRFVar.adj_gn_pre == -1)
            {
                if (ABS(rrf_signal.p2p_mv-TargetPeakPeakmV) > ABS(svoRRFVar.RRFpp_pre-TargetPeakPeakmV))
                {
                    RRFG_cur += next_gnos.val;
                }
            }
            next_gnos.val /= 2;
        }
        svoRRFVar.adj_gn_pre = next_gnos.val;
        RRFG_cur += svoRRFVar.adj_gn_pre;
        apply_RFAGCREG(RRFG_cur);
        svoRRFVar.RRFpp_pre = rrf_signal.p2p_mv;
#if (NEW_RFPOS_CALIB == 1)
        next_gnos = calAdjRRFOffset(rrf_signal.middle_mv,rrf_signal.p2p_mv,TargetPeakPeakmV,temp);
#else
        next_gnos = calAdjRRFOffset(rrf_signal.middle_mv,temp);
#endif
        temp = RFOOS_cur + next_gnos.val;
        if (temp >  127) temp =  127;
        if (temp < -127) temp = -127;

        RFOOS_cur = temp;
        apply_RFAOCREG(RFOOS_cur);

        if ((svoRRFVar.adj_gn_pre >= -RRF_MAX_GAIN_SHIFT) && (svoRRFVar.adj_gn_pre <= RRF_MAX_GAIN_SHIFT) &&
            (next_gnos.val >= -RRF_MAX_OFFSET_SHIFT) && (next_gnos.val <= RRF_MAX_OFFSET_SHIFT) && (svoRRFVar.ok_cnt >= 1))
        {
            rrf_signal.top_mv = rrf_signal.top_mv + next_gnos.val * svoRRFVar.step_size[RRFG_cur] / 256;
            break;
        }
    }
    *OffsetPeakPeak4mV = RFOOS_cur;
    *GainPeakPeakX2 = RRFG_cur;

    // RFAGCREG (dB) = %4.1f, RFAOCREG (mV) = %2d
    SendMsgCnB(SHOW_DEBUG_MSG,6,0x316694,F4B((RRFG_cur-16)/2.0),B2B(RFOOS_cur*4));

    RF_dark = RoundDivide((LONG)svoRRFVar.step_size[RRFG_cur] * (RFOOS_cur - svoRRFVar.ofs_ldoff_cur),256);
#if (NEW_RFPOS_CALIB == 1)
    RF_dark += svoRRFVar.rfagc_ref;
#endif
    if (no_RF)
    {
        ErrCode = eERR_RF_not_present;
    }
    else
    {
        svoRRFVar.modulation[Layer_Index] = modulation;
        //send_msgA1(DEBUG_CALIB_RESULT_MSG,0x53830B,modulation);  // RF_MODULATION(%)==>
    }

    // RF: PP = %4d (mV), Top = %4d (mV), DARK = %5d (mV), Modulation = %2d (%)
    SendMsgCn(SHOW_DEBUG_MSG, 7,0x316693,A2B(rrf_signal.p2p_mv),A2B(rrf_signal.top_mv),
                                         A2B(RF_dark),modulation);//20101014_02PL ==end== remove duplicate variables and messages

    return(ErrCode);
}
#endif // #if (DVD_RAM_READ == 1)

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisHfDefect()

    Description:    Enable/disable HF defect detection
    Inputs:         EnableHfDefect: If true, HF defect detection is enabled, otherwise HF defect
                    detection is disabled
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnDisHfDefect(halDFE_HfDefect_t EnableHfDefect)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    #if (CEProduct == 1)
    if (mDiscKind.Bit.DiscStd == eDiscDVD)
    {
        EnableHfDefect = 0;
    }
    #endif
#endif
    HAL_DFE_EnDisAgcAocHold(EnableHfDefect);            // Enable Freezing of AGC Dynamic Gain/AOC Hold During Defect
    WRITE_FIELD(HAL_DFE_SYNCDIS,    EnableHfDefect);    // Sync Lock Disable on Defect
    WRITE_FIELD(HAL_DFE_SLCHOD,     EnableHfDefect);    // Slicer Hold on Defect
    WRITE_FIELD(HAL_DFE_HFPLLHOD,   EnableHfDefect);    // HF PLL Hold on Defect
    WRITE_FIELD(HAL_DFE_FPLLHOD,    EnableHfDefect);    // Fast PLL Hold on Defect
    WRITE_FIELD(HAL_DFE_IIRHPFHOD,    EnableHfDefect);    // High Pass Filter Hold on Defect
    WRITE_FIELD(HAL_DFE_HLDADEQOD,    EnableHfDefect);    // Adaptive EQ Hold on Defect              poor  not set in coef
    WRITE_FIELD(HAL_DFE_VITRLHOD,    EnableHfDefect);    // Viterbi Reference Level Hold on Defect
    WRITE_FIELD(HAL_DFE_HLDACLOD,    EnableHfDefect);    // Automatic clip level Hold on defect
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisAgcAoc()

    Description:    Enables or disables AGC (Automatic Gain Control) AND AOC (Automatic Offset Control)
    Inputs:         EnAgcAoc

    Outputs:        None
    Global Inputs:
    Global Outputs: HAL_DFE_AGCON, HAL_DFE_AOCON field(s)
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable_t EnAgcAoc)
{

    //Even disable, also return to default settings
    //{
    //    WRITE_FIELD(HAL_DFE_PREOFFDAG, 1);
    //    WRITE_FIELD(HAL_DFE_PREOFFDAL, 1);
    //    WRITE_FIELD(HAL_DFE_PREOFFHDR, 1);

    //    WRITE_FIELD(HAL_DFE_PREGAIDAG, 1);
    //    WRITE_FIELD(HAL_DFE_PREGAIDAL, 1);
    //    WRITE_FIELD(HAL_DFE_PREGAIHDR, 1);
    //}

    if (EnAgcAoc)
    {
        if ((!READ_FIELD(HAL_DFE_AOCON)) || (!READ_FIELD(HAL_DFE_AGCON)))
        {
            //SendMsg80(DEBUG_DFE_MSG,0x60010A);  // DFE AGC/AOC ON
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x010203,B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            WRITE_FIELD(HAL_DFE_AGCON, EnAgcAoc);
            WRITE_FIELD(HAL_DFE_AOCON, EnAgcAoc);
        }
        else
            SendMsg80(DEBUG_DFE_MSG,0x60010D);  // DFE AGC/AOC still is ON
    }
    else
    {
        if (READ_FIELD(HAL_DFE_AOCON) || READ_FIELD(HAL_DFE_AGCON))
        {
            //SendMsg80(DEBUG_DFE_MSG,0x60010B);  // DFE AGC/AOC OFF
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x010204,B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            WRITE_FIELD(HAL_DFE_AGCON, EnAgcAoc);
            WRITE_FIELD(HAL_DFE_AOCON, EnAgcAoc);
        }
        else
            SendMsg80(DEBUG_DFE_MSG,0x60010E);  // DFE AGC/AOC still is OFF
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_LoadCalRfAgcAoc()

    Description:    Load the RF calculated offset and gain
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_LoadCalRfAgcAoc(void)
{
    WRITE_FIELD(HAL_DFE_PREOFFDAG, 1);
    WRITE_FIELD(HAL_DFE_PREGAIDAG, 1);
    WRITE_FIELD(HAL_DFE_PREOFFDAL, 1);
    WRITE_FIELD(HAL_DFE_PREGAIDAL, 1);
    WRITE_FIELD(HAL_DFE_PREOFFHDR, 1);
    WRITE_FIELD(HAL_DFE_PREGAIHDR, 1);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetRfAgcCalGain()

    Description:    Sets the RF AGC gain
    Inputs:         AgcGain: The AGC gain to be set in signed representation
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetRfAgcCalGain(BYTE AgcGain)
{
#if (MSG_OUT_MODE != MSG_DISABLE)
    if ((AgcGain > READ_FIELD(HAL_DFE_MAXDYNGAI)) || (AgcGain < READ_FIELD(HAL_DFE_MINDYNGAI)))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x3A013C); // WARNING: preset RF_AGC_GN outside bundary
    }
#endif
    WRITE_FIELD(HAL_DFE_GAIDYNDAG, AgcGain);
    WRITE_FIELD(HAL_DFE_GAIDYNDAL, AgcGain);
    WRITE_FIELD(HAL_DFE_GAIDYNHDR, AgcGain);
    WRITE_FIELD(HAL_DFE_GAINDYNWR, AgcGain - COEF_GetValue(DFE_CALC_GAINDYNWR));
    WRITE_FIELD(HAL_DFE_PREGAIDAG, 1);
    WRITE_FIELD(HAL_DFE_PREGAIDAL, 1);
    WRITE_FIELD(HAL_DFE_PREGAIHDR, 1);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetRfAocCalOffset()

    Description:    Sets the RF AOC offset
    Inputs:         AocOffset: The AOC offset to be set
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetRfAocCalOffset(SBYTE AocOffset, SBYTE AocOffsetH)
{
#if (MSG_OUT_MODE != MSG_DISABLE)
    if ((AocOffset > READ_FIELD(HAL_DFE_MAXDYNOFF)) || (AocOffset < READ_FIELD(HAL_DFE_MINDYNOFF)))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x3A013B); //WARNING: preset RF_AGC_OS outside bundary
    }
#endif
    WRITE_FIELD(HAL_DFE_OFFDYNDAG, (SBYTE)AocOffset);
    WRITE_FIELD(HAL_DFE_OFFDYNDAL, (SBYTE)AocOffset);
    WRITE_FIELD(HAL_DFE_OFFDYNHDR, (SBYTE)AocOffsetH);
    WRITE_FIELD(HAL_DFE_OFFDYNWR,  (SBYTE)AocOffset - (SBYTE)COEF_GetValue(DFE_HFAOCDYN1_OFFDYNWR));
    WRITE_FIELD(HAL_DFE_PREOFFDAG, 1);
    WRITE_FIELD(HAL_DFE_PREOFFDAL, 1);
    WRITE_FIELD(HAL_DFE_PREOFFHDR, 1);
}

#if (NEW_RFPOS_CALIB == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_ShiftRfAoc()

    Description:    Shifts the RF AOC offset and set a new CalOffset
    Inputs:         AocDiff: The AOC offset to be added to current offset
                    AocOffset: The new RF AOC CalOffset
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_ShiftRfAoc(SBYTE AocDiff, SBYTE AocOffset)
{
    WRITE_FIELD(HAL_DFE_OFFDYNDAG, READ_FIELD(HAL_DFE_ODDATGMON) + AocDiff);
    WRITE_FIELD(HAL_DFE_OFFDYNDAL, READ_FIELD(HAL_DFE_ODDATLMON) + AocDiff);
    WRITE_FIELD(HAL_DFE_PREOFFDAG, 1);
    WRITE_FIELD(HAL_DFE_PREOFFDAL, 1);
    WRITE_FIELD(HAL_DFE_OFFDYNDAG, AocOffset);
    WRITE_FIELD(HAL_DFE_OFFDYNDAL, AocOffset);
    WRITE_FIELD(HAL_DFE_OFFDYNWR,  AocOffset - (SBYTE)COEF_GetValue(DFE_CALC_OFFDYNWR));
}
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetBkDefectThr

    Description:    Calculates and sets the black defect and early black defect thresholds
    Inputs:         RfAmplitude, RfModulation
                    BKDDUThr, BKDDLThr (256 means 100%)
                    EBKDUThr, EBKDLThr (256 means 100%)
    Outputs:        None
    Global Inputs:  None
    Global Outputs: HAL_DFE_BKDDUTHR, HAL_DFE_BKDDLTHR, HAL_DFE_EBKDUTHR, HAL_DFE_EBKDLTHR
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetBkDefectThr(SHORT RfAmplitude, BYTE RfModulation, BYTE BKDDUThr, BYTE BKDDLThr, BYTE EBKDUThr, BYTE EBKDLThr)
{
    LONG temp;

    //temp = ((RfAmplitude / 2) - (RfAmplitude * 100 / RfModulation) * ((256 - BKDDUThr) / 256)) * 256 / 2000;
    temp = RfAmplitude * (32 * RfModulation - 25 * (256 - BKDDUThr)) / (500 * RfModulation);
    temp = (temp < -128)? -128 : temp;
    WRITE_FIELD(HAL_DFE_BKDDUTHR, temp);
    temp = RfAmplitude * (32 * RfModulation - 25 * (256 - BKDDLThr)) / (500 * RfModulation);
    temp = (temp < -128)? -128 : temp;
    WRITE_FIELD(HAL_DFE_BKDDLTHR, temp);
    temp = RfAmplitude * (32 * RfModulation - 25 * (256 - EBKDUThr)) / (500 * RfModulation);
    temp = (temp < -128)? -128 : temp;
    WRITE_FIELD(HAL_DFE_EBKDUTHR, temp);
    temp = RfAmplitude * (32 * RfModulation - 25 * (256 - EBKDLThr)) / (500 * RfModulation);
    temp = (temp < -128)? -128 : temp;
    WRITE_FIELD(HAL_DFE_EBKDLTHR, temp);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetAgcAocBoundary

    Description:    Calculates and sets the AGC and AOC boundaries
    Inputs:         MaxDynGain
                    MinDynGain
                    MaxDynOffset
                    MinDynOffset
    Outputs:        None
    Global Inputs:  None
    Global Outputs: HAL_DFE_MAXDYNGAI, HAL_DFE_MINDYNGAI, HAL_DFE_MAXDYNOFF, HAL_DFE_MINDYNOFF
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetAgcAocBoundary(BYTE MaxDynGain, BYTE MinDynGain, BYTE MaxDynOffset, BYTE MinDynOffset)
{
    USHORT  Gain;
    SHORT   Offset;
    SHORT   temp;

    Gain = READ_FIELD(HAL_AFE_RFAGCREG);
    Offset = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);

    /* calculate maximum dynamic gain */
    temp = Gain + MaxDynGain;
    if (temp > MAX_AGC_DYN_GAIN_ABS)
    {
        temp = MAX_AGC_DYN_GAIN_ABS;
    }
    WRITE_FIELD(HAL_DFE_MAXDYNGAI, temp);

    /* calculate minimum dynamic gain */
    temp = Gain - MinDynGain;
    if (temp < MIN_AGC_DYN_GAIN_ABS)
    {
        temp = MIN_AGC_DYN_GAIN_ABS;
    }
    WRITE_FIELD(HAL_DFE_MINDYNGAI, temp);

    /* calculate maximum dynamic offset */
    temp = Offset + MaxDynOffset;
    if (temp > MAX_AOC_DYN_OFFSETDIV4)
    {
        temp = MAX_AOC_DYN_OFFSETDIV4;
    }
    WRITE_FIELD(HAL_DFE_MAXDYNOFF, temp);

    /* calculate minimum dynamic offset */
    temp = Offset - MinDynOffset;
    if (temp < MIN_AOC_DYN_OFFSETDIV4)
    {
        temp = MIN_AOC_DYN_OFFSETDIV4;
    }
    WRITE_FIELD(HAL_DFE_MINDYNOFF, temp);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_GetHFOverspeed

    Description:
    Inputs:         FDfeInClkKHz      -> clock frequency of DFE module in KHz units
                                         CD      30000 KHz
                                         DVD     60000 KHz
                                         BD     170000 KKz
                                         HDDVD  120000 KHz
    Outputs:        OverspeedX100
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
USHORT HAL_DFE_GetHfOverspeed(ULONG FDfeInClkKHz)
{
    ULONG MeasPllFrq_factor = READ_FIELD(HAL_DFE_INTEGRAT);
    ULONG OverspeedX100;

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
        case eDiscLS:
            OverspeedX100 = (ULONG)MeasPllFrq_factor * FDfeInClkKHz / ((TWO_TO_POWER_13 * (CD_BITRATE_N1 / 100)) / 1000);
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            OverspeedX100 = (ULONG)(MeasPllFrq_factor / 2) * FDfeInClkKHz / (((ULONG)TWO_TO_POWER_13 * (BD_BITRATE_N1 / 200)) / 1000);
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            OverspeedX100 = (ULONG)(MeasPllFrq_factor / 2) * FDfeInClkKHz / ((TWO_TO_POWER_13 * (HDDVD_DATA_BITRATE_N1 / 200)) / 1000);
            break;
#endif /* (ENABLE_HDDVD == 1) */

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                {
                    OverspeedX100 = (ULONG)(MeasPllFrq_factor / 2) * FDfeInClkKHz / ((TWO_TO_POWER_13 * (DVDRAM_EMBOSSED_BITRATE_N1 / 200)) / 1000);
                }
                else
                {
                    OverspeedX100 = (ULONG)(MeasPllFrq_factor / 2) * FDfeInClkKHz / ((TWO_TO_POWER_13 * (DVDRAM_BITRATE_N1 / 200)) / 1000);
                }
            }
            else
#endif
            {
                OverspeedX100 = (ULONG)(MeasPllFrq_factor / 2) * FDfeInClkKHz / ((TWO_TO_POWER_13 * (DVD_BITRATE_N1 / 200)) / 1000);
            }
            break;

        default:
            //should never get here, maybe use assert?
            OverspeedX100 = 100; //Not zero, to avoid div by zero !
            break;
    }

    return (USHORT)OverspeedX100;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnDisBlackWhiteDefect()

    Description:    Enable/disable black white defect detection
    Inputs:         EnableBwDefect: Indicates whether black white defect detection should be enabled or
                    disabled
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnDisBlackWhiteDefect(halDFE_BwDefect_t EnableBwDefect)
{
#if ((BD_ENABLE == 1)&&(ENABLE_BDRLTH == 1))//LTH can only uses BS Source Defect
    if ((mDiscKind.Bit.DiscStd == eDiscBD) && ((svoIramVar.LTHDisclikely == 1) || (svoIramVar.LTHDisc == 1)))
    {
        EnableBwDefect = halDFE_BwDefectDisable;
    }
#endif

    WRITE_FIELD(HAL_DFE_BKDDEN,     EnableBwDefect);    // Black Dot Detection Enable
    //WRITE_FIELD(HAL_DFE_WHDDEN,       EnableBwDefect);    // White Dot Detection Enable //do not enable white defect detection because it's triggering on blank areas (BDR/E)
    WRITE_FIELD(HAL_DFE_EBKDDEN,    EnableBwDefect);    // Early Black Dot Detection Enable
    //WRITE_FIELD(HAL_DFE_EWHDDEN,  EnableBwDefect);    // Early White Dot Detection Enable
#if (PCB == 710)||(PCB==711)||(PCB==817)
    WRITE_FIELD(HAL_DFE_WHDDEN,     0); // White Dot Detection disable
    WRITE_FIELD(HAL_DFE_EWHDDEN,    0); // Early White Dot Detection disable
#else
    if (mDiscKind.Bit.Media == eMediaROM)
    {
        WRITE_FIELD(HAL_DFE_WHDDEN,     EnableBwDefect);    // White Dot Detection Enable
        WRITE_FIELD(HAL_DFE_EWHDDEN,    EnableBwDefect);    // Early White Dot Detection Enable
    }
    else
    {
        WRITE_FIELD(HAL_DFE_WHDDEN,     0); // White Dot Detection disable
        WRITE_FIELD(HAL_DFE_EWHDDEN,    0); // Early White Dot Detection disable
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnableFlywheels

    Description:    This function forces the flywheel out of lock
    Inputs:         EnableFlywheel: Is true if the flywheel should be force out of lock,
                    is false if the flywheel should not be forced out of lock anymore.
    Outputs:        None
    Global Inputs:  mDiscKind
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnableFlywheels(Bool EnableFlywheel)
{
    if (EnableFlywheel)
    {
        switch (mDiscKind.Bit.DiscStd)
        {
            case eDiscBD:
                //WRITE_FIELD( HAL_BDC_FRCAUNOLK, e_FORCEFLY_NO_ACTION);
                break;

            default:
                break;
        }
    }
    else
    {
        switch (mDiscKind.Bit.DiscStd)
        {
            case eDiscBD:
                //WRITE_FIELD( HAL_BDC_FRCAUNOLK, e_FORCEFLY_OUT_OF_LOCK);

            default:
                break;
        }
    }
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_Init

    Description:    Initalization of the DFE block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_Init( void )
{
    HAL_DFE_RomMarkDisableDetection();      // Disable rom mark detector
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_EnableICEDetect

    Description:    This function enable ICE area detect and set parameter
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_EnableICEDetect(void)
{
    //ICE area HF detect setting
    WRITE_FIELD(HAL_DFE_ICEMSKDLY ,0x3F);   //ICE area mask delay
    WRITE_FIELD(HAL_DFE_ICESFTRST , 0);     //ICE area detector soft reset no action
    WRITE_FIELD(HAL_DFE_ICEDFTPRI , 0);     //ICE area defect priority select
    //WRITE_FIELD(HAL_DFE_ICEINTDFT , 1);     //cluster state interrupt on defect
    WRITE_FIELD(HAL_DFE_ICEINTPRT , 1);     //cluster state interrupt on partially iced
    WRITE_FIELD(HAL_DFE_ICEINTFUL , 1);     //cluster state interrupt on fully iced
    WRITE_FIELD(HAL_DFE_ICEHFDET , 1);      //enable HF detector

    switch (mDiscKind.fDiscKind)
    {
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
        case eDVDRAMDisc:
            WRITE_FIELD(HAL_DFE_ICEHFATTC , 0X08);     //ICE area HF attack coef
            WRITE_FIELD(HAL_DFE_ICEHFDECC , 0x08);     //ICE area HF decay coef
            WRITE_FIELD(HAL_DFE_ICEHFTHRH , 0x08);  //ICE area HF Threshold High
            WRITE_FIELD(HAL_DFE_ICEHFTHRL , 0x04);  //ICE area HF Threshold Low
            break;
#endif
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
        default:
            WRITE_FIELD(HAL_DFE_ICEHFATTC , 8);     //ICE area HF attack coef
            WRITE_FIELD(HAL_DFE_ICEHFDECC , 0x0B);  //ICE area HF decay coef
            WRITE_FIELD(HAL_DFE_ICEHFTHRH , 0x1A);  //ICE area HF Threshold High
            WRITE_FIELD(HAL_DFE_ICEHFTHRL , 0x06);  //ICE area HF Threshold Low
            break;
    }

//    WRITE_FIELD(HAL_DFE_PREPKDETE , 1);     //preset peak detector enable
//    WRITE_FIELD(HAL_DFE_STRTHRHF , 14);     //startup threshold of HF

    //debug64 monitor
    //WRITE_FIELD(HAL_GLOBAL_DBG64EN, 1);
    //WRITE_FIELD(HAL_GLOBAL_DBG64SL, halGLOBAL_Dbg64Sel_DFE);
    //WRITE_FIELD(HAL_DFE_DFEDBGSEL, halDFE_IcedareaMode);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_DisableICEDetect

    Description:    This function disable ICE area detect and set parameter
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_DisableICEDetect(void)
{
    WRITE_FIELD(HAL_DFE_ICESFTRST , 1); //ICE area detector soft reset
    WRITE_FIELD(HAL_DFE_ICEHFDET , 0);  //disable HF detector
    WRITE_REG(HAL_DFEREG_MAP->ICEDTCFG1 , 0);
    WRITE_REG(HAL_DFEREG_MAP->ICEDTCFG2 , 0);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetIceDetectStAddr

    Description:    Set Iced
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetIceDetectStAddr(ULONG PAA)
{
    if (PAA == 0x0L)
        WRITE_FIELD(HAL_DFE_ICESTREN, 0);
    else
    {
        WRITE_FIELD(HAL_DFE_ICEDSTRAD, PAA);
        //WRITE_FIELD(HAL_DFE_ICELOADA, 1); //this will cause ICE detect disable in next startup , but why?
        WRITE_FIELD(HAL_DFE_ICESTREN, 1);
    }
}

#if ((EN_ADEQ == 1)||(EN_BD_ADEQ == 1))
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetAdaptEqCoef

    Description:    Set Adaptive equalizer coefficient
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetAdaptEqCoef(void)
{
    ULONG temp_value;
    BYTE temp_pll_mode;

    temp_value = READ_REG(HAL_DFE_ADQCSLLEN_REGISTER);
    temp_pll_mode = READ_FIELD(HAL_DFE_PLLMODE);

    //Hold(stop) slow pll clk to prevent ADEQ coef setting fail
    WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeForceHold);
    HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
    //Hold(stop) slow pll clk to prevent ADEQ coef setting fail

    WRITE_FIELD(HAL_DFE_ADEQDIS,1);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000)|0x100FA);   //TAP -2
    Delay1uS(2);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000)|0x11000);   //TAP -1
    Delay1uS(2);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000)|0x12040);   //TAP 0
    Delay1uS(2);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000)|0x13000);   //TAP +1
    Delay1uS(2);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000)|0x140FA);   //TAP +2
    Delay1uS(2);
    WRITE_REG(HAL_DFE_ADQCSLLEN_REGISTER, (temp_value&0xFFFE0000));
    #if (CHIP_REV >= 0xC0)
    if(DVDRAM)
    {
        WRITE_REG(HAL_DFE_ADQCFPRENH_REGISTER, 0x000100FA); //TAP-2
        Delay1uS(2);
        WRITE_REG(HAL_DFE_ADQCFPRENH_REGISTER, 0x00011000); //TAP-1
        Delay1uS(2);
        WRITE_REG(HAL_DFE_ADQCFPRENH_REGISTER, 0x00012040); //TAP0
        Delay1uS(2);
        WRITE_REG(HAL_DFE_ADQCFPRENH_REGISTER, 0x00013000); //TAP+1
        Delay1uS(2);
        WRITE_REG(HAL_DFE_ADQCFPRENH_REGISTER, 0x000140FA); //TAP+2
        Delay1uS(2);
    }
    #endif
    WRITE_FIELD(HAL_DFE_EQTAPA1, COEF_GetValue(DFE_CALC_EQTAPA1_Normal));
    WRITE_FIELD(HAL_DFE_EQTAPA2, COEF_GetValue(DFE_CALC_EQTAPA2_Normal));
    WRITE_FIELD(HAL_DFE_ADEQPRGOLD, 1);

    WRITE_FIELD(HAL_DFE_PLLMODE, temp_pll_mode);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetAdaptEqPartRespCoef

    Description:    Set Adaptive equalizer partial response coefficient
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetAdaptEqPartRespCoef(void)
{
    ULONG temp_value;
    BYTE temp_pll_mode;

    temp_pll_mode = READ_FIELD(HAL_DFE_PLLMODE);
    //Hold(stop) slow pll clk to prevent ADEQ coef setting fail
    WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeForceHold);
    HAL_DFE_PresetPLLFreq(svo_bReadZoneSpeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
    //Hold(stop) slow pll clk to prevent ADEQ coef setting fail

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            temp_value = READ_REG(HAL_DFE_ADQPRCFPE_REGISTER);

            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x800|ADEC_035[0]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x840|ADEC_035[1]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x880|ADEC_035[2]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x8c0|ADEC_035[3]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x900|ADEC_035[4]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600));
            WRITE_FIELD(HAL_DFE_ADQORTCND,  1);
            WRITE_FIELD(HAL_DFE_ADQMUCOEF,  5);
            WRITE_FIELD(HAL_DFE_ADQLKCOEF,  0);
            break;

        case eDiscDVD:
            temp_value = READ_REG(HAL_DFE_ADQPRCFPE_REGISTER);
            if (DVDRAM)
            {
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x800|ADEC_030[0]));
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x840|ADEC_030[1]));
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x880|ADEC_030[2]));
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x8c0|ADEC_030[3]));
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x900|ADEC_030[4]));
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600));
                WRITE_FIELD(HAL_DFE_ADQORTCND,  0);
                WRITE_FIELD(HAL_DFE_ADQMUCOEF,  7);
                WRITE_FIELD(HAL_DFE_ADQLKCOEF,  1024);
            }
            else
            {
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc5006808);
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc500684E);
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc5006891);
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc50068CE);
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc5006908);
                Delay1uS(2);
                WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, 0xc5006000);    //0.25*64
                WRITE_FIELD(HAL_DFE_ADQORTCND,  0);
                WRITE_FIELD(HAL_DFE_ADQMUCOEF,  6);
                WRITE_FIELD(HAL_DFE_ADQLKCOEF,  320);
            }
            break;

        case eDiscBD:
            temp_value = READ_REG(HAL_DFE_ADQPRCFPE_REGISTER);

            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x800|ADEC_040[0]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x840|ADEC_040[1]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x880|ADEC_040[2]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x8c0|ADEC_040[3]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600)|(0x900|ADEC_040[4]));
            Delay1uS(2);
            WRITE_REG(HAL_DFE_ADQPRCFPE_REGISTER, (temp_value&0xFFFFF600));
            WRITE_FIELD(HAL_DFE_HLDADEQOD,  1);
            if (svoIramVar.LTHDisc == TRUE)
            {
                 WRITE_FIELD(HAL_SRV_DBT,  0x0A);//set BS defect delay time
            }

            if (mDiscKind.Bit.Layers == eLayersSingle)
            {
                WRITE_FIELD(HAL_DFE_ADQORTCND,  1);    /* speed over 8x disable orthogonal condition */

            }
            else
            {
                if (mDiscKind.Bit.Media != eMediaROM)
                {
                    WRITE_FIELD(HAL_DFE_ADQORTCND,  0);    /* speed under 6x enable orthogonal condition */
                }
                else
                {
                    WRITE_FIELD(HAL_DFE_ADQORTCND,  1);
                }

            }
                WRITE_FIELD(HAL_DFE_ADQMUCOEF,  4);
               if (svoIramVar.LTHDisc == TRUE)
               {
                   WRITE_FIELD(HAL_DFE_ADQLKCOEF,  500);
               }
               else
               {
                   WRITE_FIELD(HAL_DFE_ADQLKCOEF,  50);
               }

            break;
    }
    WRITE_FIELD(HAL_DFE_PLLMODE, temp_pll_mode);
}
#endif // #if (EN_ADEQ == 1)

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetBCAFreq()

    Description:    Sets BCAFREQ based on target RPM and HF ADC freq

    Inputs:         factor_100
    Outputs:        None
    Global Inputs:  HAL_PLL_GetHfAdcClkKHz(), svoSpeedVar.TargetRpm
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DFE_SetBCAFreq(BYTE factor_100)
{
    ULONG bcafreq;

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscBD:
            //BD BCA = 4750(+-50) channel bits per revolution
            //bcafreq = HAL_PLL_GetHfAdcClkKHz() * 1000 * 60 / (svoSpeedVar.TargetRpm * 4750 * 32) * (100 / factor_100)
            bcafreq = RoundDivide(HAL_PLL_GetHfAdcClkKHz() * 750, (svoSpeedVar.TargetRpm * 19 * factor_100));
            break;

        case eDiscDVD:
            //DVD BCA = 4687(+1000-600) channel bits per revolution
            //bcafreq = HAL_PLL_GetHfAdcClkKHz() * 1000 * 60 / (svoSpeedVar.TargetRpm * 4687 * 32) * (100 / factor_100)
            bcafreq = RoundDivide(HAL_PLL_GetHfAdcClkKHz() * 40, (svoSpeedVar.TargetRpm * factor_100));
            break;
    }
    WRITE_FIELD(HAL_DFE_BCAFREQ,bcafreq);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DFE_SetIceDetectStAddr

    Description:    Set Iced
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
#define BDRRE_OVERSPEED_SW  12
#define BDRDL_OVERSPEED_SW  12
#define BDREDL_OVERSPEED_SW 12

BYTE BitDetCFG_index;
void HAL_DFE_SetBitDetCFG(USHORT OverspeedX10)
{
    BYTE        new_index;

    new_index = BitDetCFG_index;
    switch (mDiscKind.fDiscKind)
    {
        case eBDRDLDisc:
        case eBDROMDisc:
        case eBDROMDLDisc:
            if (OverspeedX10 > BDRDL_OVERSPEED_SW)
            {
                new_index = 3;
            }
            else if (OverspeedX10 < BDRDL_OVERSPEED_SW)
            {
                new_index = 2;
            }
            break;

        case eBDRDisc:
        case eBDREDisc:
            if (OverspeedX10 > BDRRE_OVERSPEED_SW)
            {
                new_index = 3;
            }
            else if (OverspeedX10 < BDRRE_OVERSPEED_SW)
            {
                new_index = 2;
            }
            break;

        case eBDREDLDisc:
            if (Layer_Index == 0)
            {
                if (OverspeedX10 > BDREDL_OVERSPEED_SW)
                {
                    new_index = 3;
                }
                else if (OverspeedX10 < BDREDL_OVERSPEED_SW)
                {
                    new_index = 2;
                }
            }
            else  //Layer_Index == 1
            {
                new_index = 3;
            }
            break;
    }

    if (BitDetCFG_index != new_index)
    {
        BitDetCFG_index = new_index;
        WRITE_FIELD(HAL_DFE_BITDETCFG,new_index);
    }
}
#endif

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
