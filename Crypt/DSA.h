/*****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME: $Workfile: DSA.h $
*
* DESCRIPTION   Header file for DSA.c
*
* $Revision: 6 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __DSA_H__
#define __DSA_H__

#if (BUILD_IN_AACS_FOLDER == 1)
#include "..\Crypt\aacstype.h"
#else
#include "..\Common\globtype.h"
#endif


/*******************************************************************
    DEFINES
*******************************************************************/
#define BIGNUM_LONGS  5
#define BIGNUM_BITS   (BIGNUM_LONGS*32)

#define LONGNUM_LONGS (BIGNUM_LONGS+2)

/*Return codes for Point Addition*/
#define ECADDOK             0
#define POINTS_ARE_EQUAL    1

/*Return codes for point multiplier*/
#define INVALID_POINT   0
#define VALID_POINT     1

#define BN_IS_EVEN(a)  ((a.slice[0] & 0x01) ? 0 : 1)

#define ShaRotLeft(n, X) (((X) << (n)) | ((X) >> (32 - (n))))

/* SHA f() functions */
#define f0(B,C,D) ( D ^ ( B & ( C ^ D ) ) )          /* Rounds  0-19 */
#define f1(B,C,D) ( B ^ C ^ D )                      /* Rounds 20-39 */
#define f2(B,C,D) ( ( B & C ) | ( D & ( B | C ) ) )  /* Rounds 40-59 */
#define f3(B,C,D) ( B ^ C ^ D )                      /* Rounds 60-79 */


/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/
typedef union
{
    ULONG word[80];
    BYTE  byte[320];
} SHA_WORKSPACE;

typedef struct                  /*20 digits base 2^256*/
{                               /*digit[0] is the most significant 8 bits */
	BYTE digit[20];
} BIGINT;

typedef struct               /* struct definition for a DSA signature   */
{
    BIGINT c;
    BIGINT d;
} MESSAGE_SIGNATURE;

typedef struct
{
    BIGINT x;
    BIGINT y;
} EC_POINT;

typedef struct                  /* Structure for BITS-bit big number  */
{                               /* slice[0] = least significant slice */
    ULONG slice[BIGNUM_LONGS];
} BIGNUM;

typedef struct
{
    ULONG slice[LONGNUM_LONGS];
} LONGNUM;

typedef struct
{
    BIGNUM c;
    BIGNUM d;
} SIGNATURE;

/* Structure definition for a projective elliptic curve point */
typedef struct
{
    BIGNUM x;
    BIGNUM y;
    BIGNUM z;                   /* definition of  a third coord, z */
    BOOL   pt_at_infinity;      /* TRUE if it is the point at infinity */
} EC_PT;

/* Elliptic curve point */
typedef struct
{
    BYTE x[20]; // x-coordinate
    BYTE y[20]; // y-coordinate
} PointType;


/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void BnAcc(BIGNUM *p, BIGNUM *q, BYTE *addr_carry);
extern void BnAdd(BIGNUM *result, BIGNUM *p, BIGNUM *q, BYTE *addr_carry);
extern SBYTE BnCmp(BIGNUM *p, BIGNUM *q);
extern void BnCopy(BIGNUM *bn_out, BIGNUM *bn_in);
extern void BnDec(BIGNUM *p, BIGNUM *q, BYTE *addr_carry);
extern BOOL BnIsEqual(BIGNUM *p, BIGNUM *q);
extern BOOL BnIsOne(BIGNUM *bn);
extern BOOL BnIsZero(BIGNUM *bn);
extern void BnMake(BIGNUM *bn, ULONG n);
extern void BnMod(BIGNUM *result, BIGNUM *a, BIGNUM *m);
extern void BnModAdd(BIGNUM *result, BIGNUM *a, BIGNUM *b, BIGNUM *m);
extern void BnModInv(BIGNUM *result, BIGNUM *n, BIGNUM *p);
extern void BnModMul(BIGNUM *result, BIGNUM *a, BIGNUM *b, BIGNUM *m);
extern void BnModSub(BIGNUM *result, BIGNUM *a, BIGNUM *b, BIGNUM *m);
extern BYTE BnNumBitsIn(BIGNUM *m);
extern BYTE BnNumDigits(BIGNUM *num);
extern void BnRLC(BIGNUM *result, BIGNUM *p, BYTE *addr_carry);
extern void BnShiftRightN(BIGNUM *result, BIGNUM *p, USHORT NumBits);
extern void BnSHR(BIGNUM *result, BIGNUM *p);
extern void BnSub(BIGNUM *result, BIGNUM *p, BIGNUM *q, BYTE *addr_carry);
extern BOOL BnTestBit(BIGNUM *bn, ULONG bit_num);
extern BOOL DsaSignMessage(SIGNATURE *signature, BIGNUM *u, BIGNUM *Hash, BIGNUM *Pk);
extern BOOL DsaVerifySignature(SIGNATURE *signature, BIGNUM *Hash, EC_PT *PubK);
extern void EcProjAffConv(EC_PT *ptResult, EC_PT *ptP);
extern ULONG EcPtAdd(EC_PT *ptResult, EC_PT *ptP, EC_PT *ptQ);
extern void EcPtCopy(EC_PT *ptResult, EC_PT *pt);
extern void EcPtDouble(EC_PT *ptResult, EC_PT *ptP);
extern void EcPtLinComb(EC_PT * ptResult, BIGNUM *a, EC_PT * ptP, BIGNUM *b, EC_PT * ptQ);
extern BYTE EcPtMult(EC_PT *ptResult, BIGNUM *factor, EC_PT *pt);
extern void LongAcc(LONGNUM *p, LONGNUM *q, BYTE *addr_carry);
extern void LongBnByUlongMul(LONGNUM *result, BIGNUM *bn_in, ULONG *short_in);
extern void LongShiftDigitRight(LONGNUM *in);
extern void LongWipe(LONGNUM *num);
extern void MnConvFromMont(BIGNUM *out, BIGNUM *in);
extern void MnConvToMont(BIGNUM *out, BIGNUM *in);
extern void MnModInv(BIGNUM *result, BIGNUM *b);
extern void Omura(BIGNUM *result, BIGNUM *m);
extern void MnMontMul(BIGNUM *z, BIGNUM *x, BIGNUM *y);
extern ULONG MnInvModTwoToTheThirtyTwo(ULONG x);
extern void ConvertToBigNum(BIGNUM *BigNum, const BIGINT *BigInt);
extern void ConvertToBigInt(BIGINT *BigInt, BIGNUM *BigNum);
extern void ConvertToEC_PT(EC_PT *EcPt, const EC_POINT *EcPoint);
extern void ConvertToEC_POINT(EC_POINT *EcPoint, EC_PT *EcPt);
extern BYTE BasePointMultiply(EC_POINT *ResultPt, BIGINT *Multiplier);
extern BYTE PointMultiply(EC_POINT *ResultPt, BIGINT *Multiplier, EC_POINT *InputPt);
extern void InitializeEcConsts(BIGNUM *a, BIGNUM *p, BIGNUM *r, EC_PT *ptG);
extern void Sha(BIGINT *TheHash, BYTE *message, ULONG length);
extern void ShaProcessBlock(BIGNUM *result, SHA_WORKSPACE *ws);

#endif // __DSA_H__
