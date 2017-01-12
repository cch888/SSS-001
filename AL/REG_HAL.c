/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   regHAL.c
*
* DESCRIPTION:
*   This file provides functions for register and field access for the abstraction
*   layer. 
*
* $Revision: 3 $
* $Date: 08/07/31 7:50a $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\AL\REG_HAL.h"



HAL_Reg32 ReadFromField(void * addr, HAL_Reg32 msk, UByte offset)
{
    return ((READ_REG(*(HAL_Reg32 *)addr) & msk) >> offset);
}


#if ALT == 1
void WriteToField(void * addr, HAL_Reg32 msk, UByte offset, HAL_Reg32 val)
{
    union
    {
        HAL_Reg32 l;
        HAL_Reg16 us;
        UByte     b;    
    } value;

    HAL_Reg32 tmp;
    UByte reglen;
    
    if ((tmp = msk) == 0) return;
    value.l = val;
    
    #if 1   // TBD? Why is this here?
    reglen = 32;
    while (tmp & 0x10000000 == 0)
    {
        reglen--;
        tmp <<= 1;
    }
    #else
    reglen = 0;
    while (tmp & 0x00000001 == 0)
    {
        reglen++;
        tmp >>= 1;
    }
    while (tmp & 0x00000001 == 1)
    {
        reglen++;
        tmp >>= 1;        
    }
    #endif
    if (reglen <=  8)
    {
        /* BYTE */
        READ_MOD_WRITE(*(UByte *)addr, msk, value.b << offset);
        
    } else if (reglen <= 16)
    {
        /* USHORT */
        READ_MOD_WRITE(*(HAL_Reg16 *)addr, msk, value.us << offset);
    } else
    {
        /* ULONG */
        READ_MOD_WRITE(*(HAL_Reg32 *)addr, msk, value.l << offset);
    }
    return;
}
#else
void WriteToField(struct FIELDT Field)
{
    HAL_Reg32 tmp;
    UByte reglen;
    
    tmp = Field.msk;
    
    if (tmp == 0) return;  /* no valid mask */
    
    reglen = 0;
    while (tmp & 0x00000001 == 0)
    {
        reglen++;
        tmp >>= 1;
    }
    while (tmp & 0x00000001 == 1)
    {
        reglen++;
        tmp >>= 1;        
    }
    if (reglen <=  8)
    {
        /* BYTE */
        READ_MOD_WRITE(*(UByte *)Field.addr, Field.msk, Field.val.b << Field.offset);
        
    } else if (reglen <= 16)
    {
        /* USHORT */
        READ_MOD_WRITE(*(HAL_Reg16 *)Field.addr, Field.msk, Field.val.us << Field.offset);
    } else
    {
        /* ULONG */
        READ_MOD_WRITE(*(HAL_Reg32 *)Field.addr, Field.msk, Field.val.l << Field.offset);
    }
    return;
}
#endif