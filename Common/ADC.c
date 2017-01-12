/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   ADC.C
*
* DESCRIPTION:  ADC utilities
*
* NOTES:
*
* $Revision: 36 $
* $Date: 10/12/21 4:16p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\system.h"
#include ".\common\scomm.h"
#include ".\al\REG_global.h"
#include ".\al\REG_periph.h"

/******************************************************************************
 *
 * Module: ReadSingleAdcmV(BYTE channel)
 * Description: This function will return the ADC value in mV from the channel
 *              selected. If the channel selected is MUXOUT1/2/3, then the
 *              correct signal must be programmed on the MUXOUT before calling
 *              this funciton.
 * INPUT: Channel (0-15)
 * RETURN: USHORT (value in mV)
 *******************************************************************************/
USHORT ReadSingleAdcmV(BYTE channel)
{
    USHORT          AdcReadData;

    if (READ_FIELD(HAL_GLOBAL_SLPSRVADC))   // Check if ADC is powered
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33FF1A); //ADC is sleep
        return(0);
    }

    /*** All ADC channels are latched on read instruction ***/
    /*** Don't need debounce or any other protection scheme ***/
    AdcReadData = READ_ADR_2B((HAL_PERIPH_ADC0_ADR + (2 * channel)) );  // Read of data from requested channel

    /*----------------------------------------------------------------------------/
    /               Convert the ADC digital value to mV unit.
    /----------------------------------------------------------------------------*/
    AdcReadData  = ((AdcReadData * GPADC_IN_RANGE + (1<<(GPADC_BITS-1))) >> GPADC_BITS);
    AdcReadData += (GPADC_IN_MIN);            /*  */
    return(AdcReadData);
}

/******************************************************************************
 * ReadAverageAdcmV
 * Description: Returns an average value in mV.
 *              The average value is calculated after exclude the Min and Max values.
 *              This routine is now optimized for the performance and smaller code size
 *              using the ARM uP, since this routine is most often used by servo.
 *
 * Input: channel - the ADC channel to sample
 *        log2Count - the number of samples to average in log scale.
 *                    The total sample count used is (2<<log2Count)+2, but
 *                    the avergage is calculated based on (2<<log2Count) samples after
 *                    excluding the min and max values.
 *
 * Output: average return value in mV
 ******************************************************************************/
USHORT  ReadAverageAdcmV(BYTE channel, BYTE log2Count)
{
    ULONG   curAdcValue;
    ULONG   adcData = 0L;
    ULONG   maxAdcValue = 0L;
    ULONG   minAdcValue = 0xFFFFFFFFL;
    ULONG   adcAddr;
    LONG    numOfSamples = (LONG)(1<<log2Count) ;
    LONG    adcCount;

    adcAddr = HAL_PERIPH_ADC0_ADR + (2 * channel);

    for (adcCount = -2; adcCount < numOfSamples; adcCount++)    //Sample two more than asked, to exclude Min and Max samples
    {
        curAdcValue = (ULONG)READ_ADR_2B(adcAddr);  //Sample the channel
        adcData += curAdcValue; //Accumulate the samples

        if (maxAdcValue < curAdcValue) {maxAdcValue = curAdcValue;} // Track the maximum value
        if (minAdcValue > curAdcValue) {minAdcValue = curAdcValue;} // Track the minimum value
    }
    adcData -= (maxAdcValue + minAdcValue); //Subtract the Min and Max from accumulated value

    //Convert the ADC digital value to mV unit.
    adcData = ((adcData * GPADC_IN_RANGE + (1<<(GPADC_BITS-1))) >> GPADC_BITS);

    //Calculate average (BGR: Please don't use division, because it works too slow!)
    if (log2Count > 0)
    {
        adcData = ((adcData + (1<<(log2Count-1))) >> log2Count);
    }

    //Shift output level by GPADC_IN_MIN
    adcData += (GPADC_IN_MIN);

    return((USHORT)adcData);
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
