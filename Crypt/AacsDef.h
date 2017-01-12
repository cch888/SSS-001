/*****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME: $Workfile: AacsDef.h $
*
* DESCRIPTION   This file contains AACS related #define(s) and typedefs
*
* $Revision: 15 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __AACS_DEF_H__
#define __AACS_DEF_H__

#if (ENABLE_AACS == 1)
#include "..\Crypt\dsa.h"


//**********************************************************************************************
//
//                                 #Defines for AACS
//
//**********************************************************************************************

//----------------------------------------------
// MKB Defines
//----------------------------------------------
#define TYPE_AND_VERSION_RECORD_ID          0x10
#define TYPE_AND_VERSION_RECORD_LENGTH      0x00000C
#define HOST_REVOCATION_LIST_RECORD_ID      0x21

#define HOST_REVOCATION_LIST_RECORD_OFFSET  12
#define HRL_TOTAL_ENTRIES_OFFSET            16
#define MAX_HRL_ENTRIES_SIZE                32748
#define MAX_PARTIAL_MKB_SIZE                0x8000      // Maximum Partial MKB size in bytes

//----------------------------------------------
// DVD-ROM MID Defines
//----------------------------------------------
#define MID_VOLUME_ID_START_ADDRESS         0x2F030
#define MID_KCD_START_ADDRESS               0x2F0D0
#define MID_BITS_IN_BYTE                    8
#define MID_BYTE_LENGTH                     20
#define MID_BIT_LENGTH                      (MID_BYTE_LENGTH * MID_BITS_IN_BYTE)

//----------------------------------------------
// Back End Type Defines for KCD
//----------------------------------------------
#define BE_TYPE_DEFAULT                     0x00
#define BE_TYPE_SIGMA                       0x01
#define BE_TYPE_BCM                         0x02
#define BE_TYPE_SIGMA_2                     0x03
#define BE_TYPE_MARVELL                     0x04
#define BE_TYPE_INTEL                       0x05


/*******************************************************************
    ENUMERATIONS
*******************************************************************/
typedef enum
{
    AACS_LA_PUBLIC_KEY      = 0x00000001,       // AACS Licensing Administrator public key
    AACS_PRIVATE_DRIVE_KEY  = 0x00000002,       // AACS private drive key
    AACS_DRIVE_CERTIFICATE  = 0x00000004,       // AACS drive certificate
    ALL_SECURE_ITEMS        = 0x00000007        // Sum of all secure items
} SecureItemType;

typedef enum
{
    AACS_START,                                 // Start AACS Authentication Process
    AACS_DRIVE_CHALLENGE,                       // Send Key format 1
    AACS_DRIVE_RESPONSE,                        // Report Key format 1
    AACS_DRIVE_RESPONSE_COMPLETE,
    AACS_DRIVE_KEY_RSP,                         // Report Key format 2
    AACS_DRIVE_KEY_RSP_COMPLT,
    AACS_HOST_KEY_VALIDATION,                   // Send Key format 2
    AACS_AUTHENTICATION_COMPLETE,               // Authentication complete
    AACS_AUTHENTICATION_FAILED,                 // Authentication failed
    ILLEGAL_AACS_STATE                          // Illegal AACS state
} AacsStateE;

typedef enum
{
    MID_READ_VID,
    MID_READ_KCD
} MidTypeE;

/*******************************************************************
    STRUCTURES
*******************************************************************/
// 320 Bit public key type.
typedef BYTE PublicKeyType[40];

// 160 Bit private key type.
typedef BYTE PrivateKeyType[20];

// 320-bit signature type.
typedef BYTE SignatureType[40];

// Certificate type.
typedef struct
{
    BYTE          Type;                         // Certificate Type.
    BYTE          Reserved;                     // Reserved field.
    USHORT        Length;                       // Length of the certificate data not including signature (005Ch).
    BYTE          Id[6];                        // Unique identifier of the Entity.
    BYTE          Reserved1[2];                 // Reserved field.
    PublicKeyType PublicKey;                    // Public Key of the Entity.
    SignatureType Signature;                    // Signature Data, calculated using the AACS LA Private Key.
} CertificateType;

typedef BYTE NonceType[20];

typedef struct
{
    NonceType       Nonce;                      // Nonce
    CertificateType Certificate;                // Certificate
} ChallengeType;

typedef struct
{
    PointType       EccPoint;                   // Elliptic Curve Point
    SignatureType   Signature;                  // Signature
} KeyType;

typedef BYTE HrlEntryIdType[6];

typedef struct
{
    USHORT          Range;
    HrlEntryIdType  HostId;
} HrlEntryType;

typedef struct
{
    union
    {
        BYTE VolumeID[16];
        BYTE MediaID[16];
    } ID;

    BYTE eVolumeID[16];
    BYTE eMediaID[16];
    BYTE KCD[16];
    BYTE eKCD[16];
    BYTE MediaSN[16];
    BYTE eMediaSN[16];
    BYTE DriveNonce[20];
    BYTE HostNonce[20];
    BYTE DriveSeed[16];
    BYTE HostSeed[16];
    BYTE DriveKey[20];
    BYTE HostPublicKey[40];
    BYTE BusKey[16];
    BYTE eBusKey[16];
    BYTE ReadDataKey[16];
    BYTE WriteDataKey[16];
} AacsAuthenticateInfoS;

typedef struct
{
    BYTE IsMediumCompliant      :1;
    BYTE IsHrlValid             :1;
    BYTE AgidInUse              :1;
    BYTE AuthenticationSuccess  :1;
    BYTE Reserved               :4;
} AacsFlagS;

typedef struct 
{
    AacsStateE  eAacsState;                     // AACS State
    AacsFlagS   AacsFlag;                       // AACS Flags
    BYTE        AuthenticationGrantId;
} AacsS;

typedef struct
{
    ULONG   PSN;        /* Start PSN */
    BYTE    LEN;        /* Clusters in size */
} MKBInfoAddrS;

typedef struct
{
    ULONG   PSN;        /* Start PSN */
    BYTE    LEN;        /* Ecc Blocks in size */
} MIDInfoAddrS;

#endif  /* ENABLE_AACS == 1 */

#endif  // __AACS_DEF_H__
