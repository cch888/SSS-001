/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   comMath.c
*
* DESCRIPTION:
*   This file supports the  common math funtions
*
* NOTES:
*
* $Revision: 39 $
* $Date: 10/12/21 4:16p $
*
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\string.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"

#include ".\common\stdlib.h"

#pragma Otime
#pragma O2

#include ".\common\commath.h"

void    mathIncremetBCDTime(TimeS *time);
BYTE    mathAddMSFValues(const TimeS  *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr);
BYTE    mathSubtractMSFValues( const TimeS *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr);
BYTE    mathCompareMSF(const TimeS *sourceTimePtr, const TimeS *destTimePtr);

void    mathAddAddressToMSF(const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr);
void    mathSubtractAddressFromMSF( const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr);
USHORT  sqrt(ULONG a);
float   fsqrt(float n);
BYTE    mathCompareMSFPlus150(const TimeS *sourceTimePtr, const TimeS *destTimePtr);
BYTE    mathCompareMSF(const TimeS *sourceTimePtr, const TimeS *destTimePtr);

long    BCD2ADR(TimeS *bcd);    /* BCD time -> ADDRESS  */
TimeS   LBA2BCD(long lba);      /* LBA      -> BCD time */
TimeS   ADR2BCD(long addr);     /* ADDRESS  -> BCD time */
TimeS   ADR2MSF(long addr);     /* ADDRESS  -> MSF time */

const BYTE bcd_2_hex[256] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,
100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,
110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,
130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,
150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165
};

/*------------------------------------------------------------------------
    Name:       mathIncremetBCDTime

    Description: increase one frame unit on an MSF BCD format variable
    Inputs:    BCD format

    Returns:   BCD format
------------------------------------------------------------------------*/
void mathIncremetBCDTime(TimeS *time)
{
    if(time->Frame == 0x74){
        time->Frame = 0;
        if(time->Second == 0x59){
            time->Second = 0;

            if((time->Minute & 0x0F) == 9){
                time->Minute += 0x07;
                if(time->Minute == 0xA0){
                    time->Minute = 0;
                }
            }
            else{
                time->Minute ++;
            }
        }
        else{
            if((time->Second & 0x0F) == 9){
                time->Second += 0x07;
            }
            else{
                time->Second++;
            }
        }
    }
    else{
        if((time->Frame & 0x0F) == 9){
                time->Frame += 0x07;
        }
        else{
            time->Frame++;
        }
    }
}

/*------------------------------------------------------------------------
    Name:       mathAddMSFValues

    Description:
    Inputs:    Hex format

    Returns:
------------------------------------------------------------------------*/

BYTE mathAddMSFValues(const TimeS  *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr)
{
long source,dest,result,rt;

    source = MSF2Count((TimeS *) sourceTimePtr);
    dest   = MSF2Count((TimeS *) destTimePtr);

    result = source + dest;
    rt    = result;
    result -= 150;

    *resultTimePtr = LBA2MSF(result);

    if(resultTimePtr->Minute >= 100){
        resultTimePtr->Minute -= 100;
    }

    if(rt > 0) return (ePlus);
    if(rt < 0) return (eMinus);
    return (eZero);
}

/*------------------------------------------------------------------------
    Name:       mathSubtractMSFValues

    Description:
    Inputs:    Hex format

    Returns:
------------------------------------------------------------------------*/
BYTE mathSubtractMSFValues( const TimeS *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr)
{
long source,dest,result,rt;

    source = MSF2Count((TimeS *) sourceTimePtr);
    dest   = MSF2Count((TimeS *) destTimePtr);

    result = source - dest;
    rt    = result;

    result -= 150;

    *resultTimePtr = LBA2MSF(result);

    if(rt > 0) return (ePlus);
    if(rt < 0) return (eMinus);
    return (eZero);
}

/*------------------------------------------------------------------------
    Name:       mathCompareMSF

    Description: Compares two MSF numbers
    Inputs:    Hex format in MSF structure for Source and Destination

    Returns: Equal, Greater, or Smaller
------------------------------------------------------------------------*/
BYTE mathCompareMSF(const TimeS *sourceTimePtr, const TimeS *destTimePtr)
{
    long source,dest;

    source = MSF2LBA(sourceTimePtr);
    dest   = MSF2LBA(destTimePtr);

    if(source > dest)   return(GREATER);
    if(source < dest)   return(SMALLER);
    return(EQUAL);
}

/*------------------------------------------------------------------------
    Name:       math_compare_msf_plus

    Description: Compares two MSF numbers with both numbers subtracting
    150.
    Inputs:    Hex format in MSF structure for Source and Destination

    Returns: Equal, Greater, or Smaller
------------------------------------------------------------------------*/
BYTE mathCompareMSFPlus150(const TimeS *sourceTimePtr, const TimeS *destTimePtr)
{
    long source,dest;

    source = MSF2LBA_Plus(sourceTimePtr);
    dest   = MSF2LBA_Plus(destTimePtr);

    if(source > dest) return(GREATER);
    if(source < dest) return(SMALLER);
    return(EQUAL);
}


/*------------------------------------------------------------------------
    Name:       mathAddAddressToMSF

    Description: Convert an LBA Address to MSF and add another MSF address

    Inputs:    Hex format in MSF structure

    Returns: Return MSF address
------------------------------------------------------------------------*/
void mathAddAddressToMSF(const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr)
{
TimeS MSF;

    MSF = ADR2MSF(LBA);
    mathAddMSFValues(sourceTimePtr, &MSF, resultTimePtr);
}

/*------------------------------------------------------------------------
    Name:       mathSubtractAddressFromMSF

    Description: Convert an LBA Address to MSF and subtract another MSF address

    Inputs:    Hex format in MSF structure

    Returns: Return MSF address
------------------------------------------------------------------------*/
void mathSubtractAddressFromMSF( const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr)
{
TimeS MSF;

    MSF = ADR2MSF(LBA);
    mathSubtractMSFValues(sourceTimePtr, &MSF, resultTimePtr);
}

/*------------------------------------------------------------------------
    Name:       sqrt

    Description: Get the Square Root of the input

    Inputs:    value

    Returns: Square Root
------------------------------------------------------------------------*/
USHORT sqrt(ULONG a)
{
    ULONG x;
    ULONG new_x;

    x = a;
    new_x = (x + 1)>>1;

    for(;;)
    {
        if(x <= new_x)
        {
            return((USHORT)x);
        }
        x = new_x;
        new_x = (x + a / x)>>1;
    }
}

/*------------------------------------------------------------------------
    Name:       fsqrt

    Description: Get the float Square Root of the input

    Inputs:    value

    Returns: Square Root
------------------------------------------------------------------------*/
float fsqrt(float n)
{
SHORT ax,bx;
float x;

    if(n<0)
        return(0);
    else if(n == 1)
        return(1);

    x = n;
    ax = *((SHORT*)&x+1);
    bx = ax & 0x807F;
    ax = ax << 1;
    ax = ax - 0x7F00;
    ax = ax & 0xFEFF;
    ax = ax/2;
    ax = ax + 0x7F00;
    ax = (WORD)ax >> 1;
    ax = ax | bx;
    *((SHORT*)&x+1) = ax;

    for(bx=0;bx<5;bx++)
        x = (x+n/x)/2;

    return(x);
}

/*------------------------------------------------------------------------
    Name:       power

    Description: calculates x^y

    Inputs:    x, y

    Returns: x to the power of y
------------------------------------------------------------------------*/
ULONG power(const USHORT x, const USHORT y)
{
    ULONG result;
    USHORT i;

    result = 1;
    i = y;

    while (i != 0) result *= x;
    return result;
}

/*------------------------------------------------------------------------
    Name:       LBA2BCD

    Description: Convert an LBA value to BCD

    Inputs:    LBA value

    Returns: BCD value
------------------------------------------------------------------------*/
TimeS LBA2BCD(long lba)   /* LBA      -> BCD time */
{
TimeS msf,bcd;

    msf = LBA2MSF(lba);
    bcd = MSF2BCD(&msf);
    return bcd;
}

/*------------------------------------------------------------------------
    Name:       ADR2BCD

    Description: Convert an Linear Address value to BCD

    Inputs:    Address value

    Returns: BCD value
------------------------------------------------------------------------*/
TimeS  ADR2BCD(long addr) /* ADDRESS  -> BCD time */
{
TimeS msf,bcd;
long min,sec,frm;

    if(addr < 0){
        addr += 450000L;
    }

    min     = addr / 4500L;
    addr   -= (min * 4500L);
    sec     = addr / 75L;
    frm   = addr - (sec * 75L);

    msf.Minute   = (BYTE)min;
    msf.Second   = (BYTE)sec;
    msf.Frame    = (BYTE)frm;

    bcd = MSF2BCD(&msf);

    return bcd;
}

/*------------------------------------------------------------------------
    Name:       ADR2MSF

    Description: Convert an Linear Address value to MSF

    Inputs:    Address value

    Returns: MSF value
------------------------------------------------------------------------*/
TimeS ADR2MSF(long addr) /* ADDRESS  -> MSF time */
{
TimeS msf;
    long  min, sec, frm;

    min   = addr / 4500L;
    addr -= (min * 4500L);
    sec   = addr / 75L;
    frm   = addr - (sec * 75L);

    msf.Minute   = (BYTE)min;
    msf.Second   = (BYTE)sec;
    msf.Frame    = (BYTE)frm;

    return msf;
}

/*------------------------------------------------------------------------
    Name:       BCD2ADR

    Description: Convert BCD to Linear Address

    Inputs:    BCD value

    Returns: Address
------------------------------------------------------------------------*/
long BCD2ADR(TimeS *bcd)  /* BCD time -> ADDRESS  */
{
long  addr;
TimeS msf;

    msf   = BCD2MSF(bcd);
    addr  = MSF2Count(&msf);

    return addr;
}

//************************************************************************;
// This function cover dB to gain ratio                                   ;
//                                                                        ;
// input: unitXdB / inputdB                                                             ;
//                                                                        ;
// output: CalculateRateFrom inputdB                                          ;
//                                                                        ;
// This code is slow. Don't use it on time critial routine !              ;
//                                                                        ;
// Don't use "extern double pow(double,double);", it use too many stacks  ;
//************************************************************************;
#define POSITIVE_1_DB       (1.12201845430)
#define NEGATIVE_1_DB       (0.89125093813)
#define POSITIVE_P_50_DB    (1.05925372518)
#define NEGATIVE_P_50_DB    (0.94406087629)
#define POSITIVE_P_10_DB    (1.01157945426)
#define NEGATIVE_P_10_DB    (0.98855309465)
#define POSITIVE_P_01_DB    (1.00115195554)
#define NEGATIVE_P_01_DB    (0.99884936994)

float CalculateRateFromXdB(BYTE unitXdB, SHORT inputdB)
{
    float  temp_f,temp_xdb;
    float  positive_x_db, negative_x_db;

    switch(unitXdB)
    {
        case P01dB:
            positive_x_db = POSITIVE_P_01_DB;
            negative_x_db = NEGATIVE_P_01_DB;
            break;

        case P10dB:
            positive_x_db = POSITIVE_P_10_DB;
            negative_x_db = NEGATIVE_P_10_DB;
            break;

        case P50dB:
            positive_x_db = POSITIVE_P_50_DB;
            negative_x_db = NEGATIVE_P_50_DB;
            break;

        case ONEdB:
            positive_x_db = POSITIVE_1_DB;
            negative_x_db = NEGATIVE_1_DB;
            break;

        default:
            return 1.0;
    }

    if (inputdB == 0)
    {
        return 1.0;
    }

    if (inputdB < 0)
    {
        temp_xdb = negative_x_db;
        inputdB = -inputdB;
    }
    else
    {
        temp_xdb = positive_x_db;
    }

    temp_f = temp_xdb;
    while (inputdB > 1)
    {
        temp_f *= temp_xdb;
        inputdB --;
    }
    return temp_f;
}// end of CalculateRateFromXdB

//**************************************************************************;
// input:   ratio  (1.0 = 0 dB)                                             ;
// output:  (unit: unitXdB)                                                 ;
//                                                                          ;
// Please keep in mind that don't call this function as time critial code.  ;
// This function running slow !                                             ;
//                                                                          ;
// Don't use "extern double log10(double);" because it use too many stacks  ;
//**************************************************************************;
SHORT CalculateXdBFromRate(BYTE unitXdB, USHORT v2, USHORT v1)
{
    float   ftemp, rate;
    SHORT   i;
    float   positive_x_db, negative_x_db;

    if (v1 == 0)
    {
        return(0x7FFF);   // it's an error case. just return 0x7FFF for now
    }

    rate = (float)v2/(float)v1;

    if (rate < 0.0)
        return(0);   // it's an error case. just return 0 for now

    if (rate == 1.0)
        return(0);

    switch(unitXdB)
    {
        case P01dB:
            positive_x_db = POSITIVE_P_01_DB;
            negative_x_db = NEGATIVE_P_01_DB;
            break;

        case P10dB:
            positive_x_db = POSITIVE_P_10_DB;
            negative_x_db = NEGATIVE_P_10_DB;
            break;

        case P50dB:
            positive_x_db = POSITIVE_P_50_DB;
            negative_x_db = NEGATIVE_P_50_DB;
            break;

        case ONEdB:
            positive_x_db = POSITIVE_1_DB;
            negative_x_db = NEGATIVE_1_DB;
            break;

        default:
            return(0);
    }

    ftemp = 1.0;
    //-------------------------------------------------;
    // normal case                                     ;
    //-------------------------------------------------;
    if (rate > ftemp)
    {
        // rate > 1.0
        for (i = 1; i < 32767; i++)
        {
            ftemp *= positive_x_db;
            if (ftemp > rate)
                break; // break from for i loop
        }
        if (rate * 2 * positive_x_db <= ftemp * (1 + positive_x_db))
        {
            i--;
        }
    }
    else
    {
        // rate < 1.0
        for (i = -1; i > -32768; i--)
        {
            ftemp *= negative_x_db;
            if (ftemp < rate)
                break; // break from for i loop
        }
        if (rate * 2 * negative_x_db >= ftemp * (1 + negative_x_db))
        {
            i++;
        }
    }
    return (i);
}// end of CalculateXdBFromRate

/* -------------------------------------------------------------------------------
 *
 * Module: float2short()
 * Description: return closest integer
 *
 * ------------------------------------------------------------------------------- */
SHORT float2short(float a)
{
    SHORT y;

    if (a < -32768) a = -32768;
    if (a > 32767) a = 32767;

    if (a >= 0)
        y = (SHORT)(a + 0.5f);
    else
        y = (SHORT)(a - 0.5f);

    return (y);
}// end of float2short

/*-----------------------------------------------------------------------------
 *
 * Module: RoundDivide()
 * Description: input numerator and denominator, return rounded integer
 *
 *----------------------------------------------------------------------------*/
LONG RoundDivide(LONG numerator, LONG denominator )
{
    LONG result,half_denominator;
    BYTE negative;

    negative = (numerator < 0) ^ (denominator < 0);

    if (denominator == 0)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33FF1B); //Math Calculate Error - Divided by Zero
        result = numerator;
    }
    else
    {
        half_denominator = denominator / 2;
        if (!negative) /* result positive */
        {
            result = (numerator + half_denominator) / denominator;
        }
        else           /* result negative */
        {
            result = (numerator - half_denominator) / denominator;
        }
    }

    return result;
}

/*--------------------------------------------------------------------
  Name: RandomNumberGenerator()
  Description:
    Generates a random number used for Disc ID
  Inputs:
    none
  Outputs:
    none
  Returns:
    Random value (BYTE)
--------------------------------------------------------------------*/
BYTE RandomNumberGenerator(BYTE seedp)
{
    BYTE    Seed;
    BYTE    Random_dt;
    ULONG   timer_val;

    Random_dt = (BYTE)(rand());
    seedp += Random_dt;

    StartTimer(&timer_val);
    srand((unsigned int)(timer_val + (ULONG)seedp));

    Random_dt = (BYTE)(rand()>>(timer_val&0x07));

    Seed = (BYTE)((USHORT)(99 * Random_dt ) / 0xFF ) ;
    return( Seed );
}

/*--------------------------------------------------------------------
  Name: PowerTo()
  Description:
    Calculate x power to y
  Inputs: baseNumber, powerToNumber
  Returns:
    LONG result
--------------------------------------------------------------------*/
LONG  PowerTo(LONG baseNumber, LONG powerToNumber )
{
    BYTE i;
    LONG result;
    result = 1;
    for (i = 0; i < powerToNumber; i++)
    {
        result = result * baseNumber;
    }

    return (result);
}
