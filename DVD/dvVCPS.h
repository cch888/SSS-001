/****************************************************************************
*                (c) Copyright 2000-2005, SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
*   FILE NAME:     dvVCPS.h
*
* DESCRIPTION
*
* Contains the VCPS for DVD+R/+RW.
*
*   $Revision: 12 $
*   $Date: 07/08/24 3:08p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __dvVCPS_H__
#define __dvVCPS_H__

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\hif\rd_util.h"
#include ".\dvd\dvDef.h"
#include ".\dvd\dvmacros.h"
#include ".\crypt\aes.h"


/*******************************************************************
    DEFINITIONS
*******************************************************************/
#define VCPS_AUTHENTICATION 0x20    // VCPS Key Class field value in Report/Send Key Command
#define VCPS_ETOC_SIZE      256     // The size of VCPS ETOC in ADIP
#define VCPS_HASH_SIZE      16      // The size of VCPS Hash in ADIP

// OFFSET AND SIZE INFORMATION OF VCPS DATA SENT BY HOST
#define VCPS_AUTHKEY_INFO_OFFSET    11
#define VCPS_HOSTKEY_SIZE           8
#define VCPS_AUTHKEY_SIZE           16

#define VCPS_KEYCONTRIBUTION_OFFSET 8
#define VCPS_KEYENCRYPTION_SIZE     32

// REPORT KEY FUNCTION CODES
#define RPTKEY_DKB                  0x01
#define RPTKEY_DEVICE_ID            0x02
#define RPTKEY_KEY_CONTRIBUTION     0x03
#define RPTKEY_DKBHASH_UNIQUEID     0x04
#define RPTKEY_DKB_INFO             0x05

// SEND KEY FUNCTION CODES
#define SNDKEY_AUTHORIZATION_KEY    0x01
#define SNDKEY_KEY_CONTRIBUTION     0x02

// WRITTEN DKB PSN ADDRESSES
#define DKB_BUFFERZONE2_ADDRESS1    0x2FE10L
#define DKB_BUFFERZONE2_ADDRESS2    0x2FF10L

// EFI TOC PSN ADDRESS
#define EFI_TOC_START_ADDRESS       0x30000L
#define EFI_TOC_END_ADDRESS         0x31FFFL

#define DKB_WORK_AREA_ADDRESS       0x601F4FFCL // Address of work area for DKB
#define DKB_START_ADDRESS           0x601F5000L
#define DKB_START_PAGE              0x3EA

// VCPS Device Keys
#define DEVICE_KEYS_TABLE_SIZE      0x296

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

// VCPS STATES
typedef enum
{
    VCPS_IDLE,
    VCPS_SEND_DEVICE_ID,
    VCPS_GET_AUTHORIZATION_KEY,
    VCPS_SEND_DRIVE_CONTRIBUTION_KEY,
    VCPS_GET_HOST_CONTRIBUTION_KEY,
    VCPS_SEND_DKBHASH_UNIQUEID,
} vcpsStateE;

// DKB LOCATION
typedef enum
{
    DKB_UNDETERMINED,	    // No valid DKB has been read
    DKB_BUFFER_ZONE2,	    // Valid DKB was read from Buffer Zone 2
    DKB_INITIAL_ZONE = 2,	// Valid DKB was read from Initial Zone
    DKB_ADIP         = 4	// Valid DKB was read from ADIP in data area
} vcpsDKBLocE;

// DKB STATE
typedef enum
{
    DKB_NOT_READ,	    // No attempts have been made to read the DKB
    DKB_HEADER_VALID,   // A valid DKB Header has been read from the disc
    DKB_VALID,		    // A valid DKB has been read from this disc and location specified in DKBLocation
    DKB_INVALID,	    // All attempts to read a DKB have failed, DKB does not exist on this disc
    DKB_READING		    // Reading of the DKB is currently in progress

} DKBStateE;

typedef enum
{
    ADIP_INFO_NONE,
    ADIP_INFO_ETOC,
    ADIP_INFO_HASH,
    ADIP_INFO_DKB
} vpcsAdipInfoTypeE;

/*******************************************************************
    STRUCTURE DEFINITIONS
*******************************************************************/
struct vcpsDataS
{
    BYTE            Authorized;     // Indication of valid authentication
    BYTE            Invalid;        // Indicates that authentication cannot be done on VCPS disc
    vcpsStateE      State;          // Indicates the status of VCPS operation

    vcpsDKBLocE     DKBLocation;    // Indicates that a DKB has been found at specific region
    ULONG           DKBSize;        // Size of DKB structure
    BYTE            *DKBptr;        // Pointer into SDRAM where DKB resides
	DKBStateE		DKBState;		// State of DKB on Disc
    ULONG           DKBBytesIn;     // Number of DKB bytes collected from DKB structure on Disc
    BYTE            UniqueID[16];
    BYTE            DKBHash[16];
};

typedef struct
{
    ULONG size;
    ULONG seq_num;
    ULONG DKBInAdipStartAddr;
    ULONG DKBInAdipEndAddr;
    ULONG HashInAdipStartAddr;
    ULONG HashInAdipEndAddr;
    BYTE DKBInAdipRepeatCnt;
    BYTE HashInAdipRepeatCnt;
    BYTE key_chk_data[16];

    DKBStateE State;
} EKBS;

typedef struct
{
    BYTE NodeKeyIndex;
    BYTE *NodeKey;      // Pointer to valid Device Node Key (Specification symbol KN)

    // Authorization Data
    BYTE AuthKey[16];       // Authorization Key      (Specification symbol KA)
    BYTE RootKey[16];       // Root Key               (Specification symbol KR)
    BYTE BusKey[16];        // Bus Key                (Specification symbol KB)

    // Random Generated numbers from device and host.

    BYTE HostRndNum[8];     // Host random number      (Specification symbol RA)
    BYTE DeviceRndNum[8];   // Device random number    (Specification symbol RD)
    BYTE DeviceKey[16];     // Device contribution key (Specification symbol QD)
    BYTE HostKey[16];       // Host contribution key   (Specification symbol QA)

} vcpsAuthenticateInfoS;

typedef union
{
    BYTE    Array[668];
    struct
    {
        BYTE    DeviceID[5];
        BYTE    NodeKeyTable[40][16];
        BYTE    Keys[16];
        BYTE    Reserved;
        ULONG   Checksum;
        USHORT  RandomKey;
    }Set;
} VcpsKeys;

/*******************************************************************
    EXTERNAL VARIABLES
*******************************************************************/
extern BYTE VCPS_disc;
extern BYTE VcpsKeysValid;
extern BYTE *DKBInfoPtr;
extern BYTE *DKBInfoStatusPtr;
extern USHORT DKBInfoLength;
extern USHORT ADIPInfoBaseAddr;
extern vpcsAdipInfoTypeE DKBInfoType;
extern VcpsKeys const InitVector;


/*******************************************************************
    EXTERNAL FUNCTIONS /FUNCTION PROTOTYPES
*******************************************************************/
extern void VCPS_ReportKey(void);
extern void VCPS_SendKey(void);
extern BYTE GetVcpsInfoInAdip(ULONG StartAddr, USHORT Length, vpcsAdipInfoTypeE InfoType);
extern void DecryptVCPSKeys(void);

BYTE Get_DKB(void);
void SendVcpsInfo(BYTE type, BYTE *data1);
BYTE ValidateSequence(vcpsStateE CurrentState);
BYTE GetVcpsInfo(BYTE type, BYTE *data);
void ArrayXor(BYTE *din1, BYTE *din2, BYTE *dout, BYTE size);

#endif