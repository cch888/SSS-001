/*****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME: $Workfile: AES.h $
*
* DESCRIPTION   Header file for AES.c
*
* $Revision: 8 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __AES_H__
#define __AES_H__

#if (BUILD_IN_AACS_FOLDER == 1)
#include "..\Crypt\aacstype.h"
#else
#include "..\Common\globtype.h"
#endif


/*******************************************************************
    DEFINES
*******************************************************************/
#define DEC_LONG(l,a,b,c,d) { \
                                a = (l >> 24); \
                                b = (l >> 16) & 0xff; \
                                c = (l >> 8) & 0xff; \
                                d = l & 0xff; \
                            }

#define MAKE_LONG(a,b,c,d) ((ULONG)a << 24 | (ULONG)b << 16 | (ULONG)c << 8 | (ULONG)d)

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void AESCbcDecrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size);
extern void AESCbcEncrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size);
extern void AESEncrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag);
extern void AESDecrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag);
extern void KeyExpansion(BYTE *key);
extern ULONG Sub_Rot_Word(ULONG);
extern void AddRoundKey(BYTE num);
extern void SubBytes(void);
extern void InvSubBytes(void);
extern void ShiftRows(void);
extern void InvShiftRows(void);
extern void MixColumns(void);
extern void InvMixColumns(void);
extern BYTE Multi_A(BYTE factor, BYTE varible);
extern void GenerateRandomNumber(BYTE * rn, BYTE size);

#endif // __AES_H__
