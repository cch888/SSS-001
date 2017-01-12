/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   ADC.H
*
* DESCRIPTION:  Header file for ADC.C
*
* NOTES:
*
*   $Revision: 59 $
*   $Date: 10/11/26 1:17p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _ADC_1H_
#define _ADC_1H_

#include ".\COMMON\globtype.h"

extern USHORT       ReadSingleAdcmV(BYTE channel);
extern USHORT       ReadAverageAdcmV(BYTE channel, BYTE log2Count);

#define VREF_IN_MV      1650    /* in mV */

#define ADCGAIN        1        /* 0:ADC Range is VREF +- 1.6V (measured 1.53V)*/
                                /* 1:ADC Range is VREF +- 1.2V (measured 1.15V)*/

/* ADC input channels */
#define ADC_GPADCO1    0        // GPADC1 Input Pin channel
#define ADC_CE         1        // CE - 1st Center error channel
#define ADC_BS         2        // BS - Beam Strength channel
#define ADC_RP         3        // RP - Ripple channel
#define ADC_GPADO2     4        // GPADC2 Input Pin channel
#define ADC_TE         5        // TE - 1st Tracking Error channel
#define ADC_FE         6        // FE - 1st Focus Error channel
#define ADC_MUXOUT1    7        // MUXOUT1 - MuxOut1 channel
#define ADC_VRPOW      8        // Read Power channel
#define ADC_VWPOW      9        // Write Power channel
#define ADC_PHSO      10        // Peak Hold channel
#define ADC_BHSO      11        // Bottom Hold channel
#define ADC_MUXOUT3   12        // MUXOUT3 - MuxOut3 channel
#define ADC_TE_2      13        // TE_2 - 2nd Tracking error channel
#define ADC_FE_2      14        // FE_2 - 2nd Focus Error channel
#define ADC_MUXOUT2   15        // MUXOUT2 - MuxOut2 channel

#define ADC_SPECIAL_APC         253
#define ADC_SPECIAL_PN    254
#define ADC_SPECIAL_RFDFE 255

#if ADCGAIN == 1
#define ADC_RANGE_IN_MV         1200        /* ADC Range is VREF +- 1.2V */
#else
#define ADC_RANGE_IN_MV         1600        /* ADC Range is VREF +- 1.6V */
#endif


/* ADC range depends on the ADCGAIN setting */
/* The ADC center point is set by VREF */
#if ((PCB == 1600) && (CHIP_REV == 0xB0))
    #if ADCGAIN == 1
#define GPADC_IN_MIN            (USHORT)(500)   /* mV. It's better to use empirical than ideal data for the accurary. (BGR) */
#define GPADC_IN_MAX            (USHORT)(2800)  /* mV. It's better to use empirical than ideal data for the accurary. (BGR) */
    #else
#define GPADC_IN_MIN            (USHORT)(125)   /* mV. It's better to use empirical than ideal data for the accurary. (BGR) */
#define GPADC_IN_MAX            (USHORT)(3175)  /* mV. It's better to use empirical than ideal data for the accurary. (BGR) */
    #endif
#else
#define GPADC_IN_MIN            (USHORT)(VREF_IN_MV - ADC_RANGE_IN_MV)  /* mV */
#define GPADC_IN_MAX            (USHORT)(VREF_IN_MV + ADC_RANGE_IN_MV)  /* mV */
#endif
#define GPADC_IN_RANGE          (USHORT)(GPADC_IN_MAX - GPADC_IN_MIN)   /* mV */
#define GPADC_BITS              (BYTE)10
#define GPADC_OUT_MIN           0
#define GPADC_OUT_CENTER        (1 << (GPADC_BITS - 1))
#define GPADC_OUT_MAX           ((1 << GPADC_BITS) - 1)

#define TOP_SATURATE_GPADC      936
#define BOT_SATURATE_GPADC      84
#define TOP_SATURATE_LIM        (((TOP_SATURATE_GPADC * GPADC_IN_RANGE) >> GPADC_BITS) + GPADC_IN_MIN)   /* unit mv */
#define BOT_SATURATE_LIM        (((BOT_SATURATE_GPADC * GPADC_IN_RANGE) >> GPADC_BITS) + GPADC_IN_MIN)   /* unit mv */
#define TOP_SATURATE_LIM_RRF    (VREF_IN_MV + 1350)   /* unit mv */
#define BOT_SATURATE_LIM_RRF    (VREF_IN_MV - 450)    /* unit mv */

#define MV_5           5
#define MV_7           7
#define MV_10         10
#define MV_15         15
#define MV_20         20
#define MV_30         30
#define MV_40         40
#define MV_50         50
#define MV_75         75
#define MV_100       100
#define MV_150       150
#define MV_200       200
#define MV_300       300
#define MV_250       250
#define MV_350       350
#define MV_380       380
#define MV_400       400
#define MV_450       450
#define MV_500       500
#define MV_550       550
#define MV_600       600
#define MV_650       650
#define MV_700       700
#define MV_750       750
#define MV_800       800
#define MV_825       825
#define MV_830       830
#define MV_850       850
#define MV_900       900
#define MV_950       950
#define MV_1000     1000
#define MV_1050     1050
#define MV_1100     1100
#define MV_1150     1150
#define MV_1200     1200
#define MV_1250     1250
#define MV_1300     1300
#define MV_1400     1400
#define MV_1450     1450
#define MV_1500     1500
#define MV_1550     1550
#define MV_1580     1580
#define MV_1600     1600
#define MV_1700     1700
#define MV_1800     1800
#define MV_1850     1850
#define MV_1900     1900
#define MV_2000     2000
#define MV_2100     2100
#define MV_2200     2200
#define MV_2300     2300
#define MV_2400     2400
#define MV_2500     2500
#define MV_2600     2600
#define MV_2700     2700
#define MV_2850     2850
#define MV_2900     2900
#define MV_3000     3000
#define MV_3100     3100
#define MV_4000     4000
#define MV_5000     5000

/* RF ADC */
#define RF_ADC_IN_TOP      1000                               //mV
#define RF_ADC_IN_BOT     -1000                               //mV
#define RF_ADC_IN_RANGE    (RF_ADC_IN_TOP - RF_ADC_IN_BOT)    //mV

/* Wbl ADC */
#define WBL_ADC_IN_TOP   RF_ADC_IN_TOP
#define WBL_ADC_IN_BOT   RF_ADC_IN_BOT
#define WBL_ADC_IN_RANGE RF_ADC_IN_RANGE

#endif /* _ADC_1H_ */
