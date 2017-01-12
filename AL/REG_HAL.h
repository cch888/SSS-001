/****************************************************************************
*                (c) Copyright 2006 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   regHAL.h
*
* DESCRIPTION:
*   This file provides macro's for register and field access for the abstraction
*   layer.
*
* $Revision: 19 $
* $Date: 10/11/22 11:17p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef __REGHAL_H__
#define __REGHAL_H__


// Register types
typedef volatile unsigned char HAL_Reg8;
typedef volatile unsigned short HAL_Reg16;
typedef volatile unsigned long HAL_Reg32;
typedef unsigned char UByte;
typedef signed char SByte;
typedef unsigned short UInt16;
typedef signed short SInt16;
typedef unsigned long UInt32;
typedef signed long SInt32;
typedef unsigned char Bool;

#define ALT 1
#if ALT == 0
struct FIELDT
{
    void * addr;
    HAL_Reg32 msk;
    UByte offset;
    union
    {
        HAL_Reg32   l;
        HAL_Reg16   us;
        UByte       b;
    }val;
};
#endif


__inline extern HAL_Reg32 EXTENDSIGN(HAL_Reg32 UnsignedValue, HAL_Reg8 BitNumber, HAL_Reg32 SignedMask)
{
    HAL_Reg32 TempValue = UnsignedValue;
    if ((TempValue >>  (BitNumber - 1)) & 0x1 )
        return (SignedMask | TempValue);
    else
        return (TempValue);
}

#define READ_REG(Register) (Register)
#define WRITE_REG(Register, Data) (Register = (Data))

    #if (CHIP_REV >= 0xC0)
#define HAL_SRVDSP_BaseAddress 0xB0005000
    #else
#define HAL_SRVDSP_BaseAddress 0xB0000600
    #endif
#define WRITE_DSP_COEF(eq_addr, data)  WRITE_REG(*(USHORT *) (HAL_SRVDSP_BaseAddress + (eq_addr)), data)
#define READ_DSP_COEF(eq_addr)         READ_REG(*(vSHORT *) (HAL_SRVDSP_BaseAddress + (eq_addr)))

// AND register contents with inverted mask, OR in data, and write the result
#define READ_MOD_WRITE(Register, Mask, Data) \
{\
   Register = ((READ_REG(Register) & ~(Mask)) | ((Data) & (Mask)) );\
}

//Compiler evaluates to constant and skips the then or else part.
/*
#define WRITE_FIELD(Field,Value) \
   ((##Field##_WRITABLE)?\
      (##Field##_REGISTER) = (READ_REG(##Field##_REGISTER) & ~(##Field##_MASK)) | ((((HAL_Reg32)(Value)) << ##Field##_LSB) & (##Field##_MASK))\
    :\
    (\
        ##Field##_WRITABLE\
    ))
*/

#define WRITE_FIELD(Field,Value) \
      (##Field##_REGISTER) = (READ_REG(##Field##_REGISTER) & ~(##Field##_MASK)) | ((((HAL_Reg32)(Value)) << ##Field##_LSB) & (##Field##_MASK))

/*
#define WRITE_FIELDTO(Buffer,Field,Value)   \
   ((##Field##_WRITABLE==1)?\
    (\
        Buffer = (Buffer & ~##Field##_MASK) | ((((HAL_Reg32)(Value)) << ##Field##_LSB) & ##Field##_MASK)\
    )\
    :\
    (\
        ##Field##_WRITABLE\
    ))
*/
#define WRITE_FIELDTO(Buffer,Field,Value)   \
        Buffer = (Buffer & ~##Field##_MASK) | ((((HAL_Reg32)(Value)) << ##Field##_LSB) & ##Field##_MASK)

#define READ_FIELD(Field)\
   (##Field##_TYPE)\
   ((##Field##_READABLE==1)?\
    (\
        (##Field##_SIGNED==1)?\
            EXTENDSIGN((HAL_Reg32)((READ_REG(##Field##_REGISTER) & ##Field##_MASK) >> ##Field##_LSB),(HAL_Reg8)(##Field##_MSB + 1 - ##Field##_LSB),(HAL_Reg32)~(##Field##_MASK >> ##Field##_LSB))\
        :\
            ((READ_REG(##Field##_REGISTER) & ##Field##_MASK) >> ##Field##_LSB)\
    )\
    :\
    (\
        ##Field##_READABLE\
    ))

#define READ_FIELDFROM(Buffer,Field)\
   (##Field##_TYPE)\
    ((##Field##_READABLE==1)?\
    (\
        (##Field##_SIGNED==1)?\
            EXTENDSIGN((HAL_Reg32)((Buffer & ##Field##_MASK) >> ##Field##_LSB),(HAL_Reg8)(##Field##_MSB + 1 - ##Field##_LSB),(HAL_Reg32)~(##Field##_MASK >> ##Field##_LSB))\
        :\
            ((Buffer & ##Field##_MASK) >> ##Field##_LSB)\
    )\
    :\
    (\
        ##Field##_READABLE\
    ))

#define READ_FROM_FIELD(Register, Mask, Offset) \
   ((READ_REG(Register) & (Mask)) >> Offset);\

#define FLIP_FIELD(Field) \
      (##Field##_REGISTER) = (READ_REG(##Field##_REGISTER) ^ (##Field##_MASK))

#if ALT == 1
extern void WriteToField(void * addr, HAL_Reg32 msk, UByte offset, HAL_Reg32 val);
#else
extern void WriteToField(struct FIELDT Field);
#endif

/* macro for clearing interrupt bit field */
#define CLEAR_INT_FIELD(Field, ClearValue)\
      if (sizeof(##Field##_REGISTER)==1) \
            (HAL_Reg8)(##Field##_REGISTER) =  (ClearValue == 0)? (HAL_Reg8)(~(##Field##_MASK)): (HAL_Reg8)((##Field##_MASK)); \
      else if (sizeof(##Field##_REGISTER)==2) \
            (HAL_Reg16)(##Field##_REGISTER) = (ClearValue == 0)? (HAL_Reg16)(~(##Field##_MASK)): (HAL_Reg16)((##Field##_MASK)); \
      else \
            (HAL_Reg32)(##Field##_REGISTER) = (ClearValue == 0)? (HAL_Reg32)(~(##Field##_MASK)): (HAL_Reg32)((##Field##_MASK));

/* macros for register access using address rather than register reference */
#define WRITE_ADR_1B(Address, DataByte) \
            WRITE_REG(*(BYTE *)Address, DataByte)

#define WRITE_ADR_2B(Address, DataUShort) \
            WRITE_REG(*(USHORT *)Address, DataUShort)

#define WRITE_ADR_4B(Address, DataULong) \
            WRITE_REG(*(ULONG *)Address, DataULong)

#define READ_ADR_1B(Address) \
            READ_REG(*(vBYTE *)(Address))

#define READ_ADR_2B(Address) \
            READ_REG(*(vUSHORT *)(Address))

#define READ_ADR_4B(Address) \
            READ_REG(*(vULONG *)(Address))

#endif    /*__REGHAL_H__*/
