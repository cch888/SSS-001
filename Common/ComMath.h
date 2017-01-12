/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* FILENAME:   comMath.c
*
* DESCRIPTION
*   This file supports the math funtions
*
* NOTES:
*
* $Revision: 21 $
* $Date: 10/03/17 10:29p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _comMATH_H_
#define _comMATH_H_

#include ".\common\com_im.h"

#define EQUAL 0
#define SMALLER 1
#define GREATER 2

#define BCD2HEX(x) bcd_2_hex[x]

extern const BYTE bcd_2_hex[256];

enum
{
    eZero,
    ePlus,
    eMinus
};

enum
{
    P01dB,
    P10dB,
    P50dB,
    ONEdB
};

extern void mathIncremetBCDTime(TimeS *time);

extern BYTE mathAddMSFValues(const TimeS  *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr);
extern BYTE mathSubtractMSFValues( const TimeS *sourceTimePtr, const TimeS *destTimePtr, TimeS *resultTimePtr);

extern BYTE mathCompareMSF(const TimeS *sourceTimePtr, const TimeS *destTimePtr);

extern void mathAddAddressToMSF(const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr);
extern void mathSubtractAddressFromMSF( const TimeS *sourceTimePtr, ULONG LBA, TimeS *resultTimePtr);
extern USHORT sqrt(ULONG a);
extern float   fsqrt(float n);

#define WORDHEX2BCD(WORDhex)      (vUSHORT)(((vUSHORT)WORDhex )/ 1000) << 12 \
                         |(((vUSHORT)WORDhex % 1000)/100)<<8\
                         |((((vUSHORT)WORDhex % 1000)%100)/10)<<4\
                         |((((vUSHORT)WORDhex % 1000)%100)%10)


#define FLOATHEX2BCD(FLOAThex)      (vUSHORT)(((vUSHORT)FLOAThex )/ 10000000) << 28 \
                         |(((vUSHORT)FLOAThex %10000000)/ 1000000) << 24 \
                         |((((vUSHORT)FLOAThex%10000000) %1000000)/ 100000) << 20 \
                         |(((((vUSHORT)FLOAThex%10000000) %1000000)%100000)/ 10000) << 16 \
                         |((((((vUSHORT)FLOAThex%10000000) %1000000)%100000)%10000)/ 1000) << 12 \
                         |(((((((vUSHORT)FLOAThex%10000000) %1000000)%100000)%10000)%1000)/100)<<8\
                         |((((((((vUSHORT)FLOAThex%10000000) %1000000)%100000)%10000)%1000)%100)/10)<<4\
                         |((((((((vUSHORT)FLOAThex%10000000) %1000000)%100000)%10000)%1000)%100)%10)

extern long  BCD2ADR(TimeS *bcd);  /* BCD time -> ADDRESS  */
extern TimeS LBA2BCD(long lba);    /* LBA      -> BCD time */
extern TimeS ADR2BCD(long addr);   /* ADDRESS  -> BCD time */
extern TimeS ADR2MSF(long addr);   /* ADDRESS  -> MSF time */
extern float CalculateRateFromXdB(BYTE unitXdB, SHORT inputdB);
extern SHORT CalculateXdBFromRate(BYTE unitXdB, USHORT v2, USHORT v1);
extern BYTE  mathCompareMSFPlus150(const TimeS *sourceTimePtr, const TimeS *destTimePtr);
extern SHORT float2short(float a);
extern LONG  RoundDivide(LONG numerator, LONG denominator );
extern LONG  PowerTo(LONG baseNumber, LONG powerToNumber );
extern ULONG power(const USHORT x, const USHORT y);

#define BCD2LBA(bcd)        (long)(BCD2ADR((TimeS *)bcd) - 150L)
#define MSF2LBA(msf)        (long)(MSF2ADR((TimeS *)msf) - 150L)
#define MSF2LBA_Plus(msf)   (long)(MSF2Count((TimeS *)msf) - 150L)

#define ABS(a)   (((LONG)(a) > 0)? (a) : -(a))
#define MAX(a,b) (((a) > (b))? (a) : (b))
#define MIN(a,b) (((a) < (b))? (a) : (b))
#endif /* _comMATH_H_ */
