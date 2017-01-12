/*****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME: $Workfile: AACS.h $
*
* DESCRIPTION   Header file for bdAACS.c
*
* $Revision: 15 $
* $Date: 11/05/03 3:10p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __AACS_H__
#define __AACS_H__

#include "..\Crypt\AacsDef.h"


/*******************************************************************
    DEFINES
*******************************************************************/

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/


/*******************************************************************
    EXTERNAL VARIABLES
*******************************************************************/
extern AacsAuthenticateInfoS AacsAuth;
extern AacsS AacsStatus;

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
void InitializeAacsFlags(void);
BOOL AacsValidateAgid(BYTE SentAgid);
void AacsInvalidateAgid(void);
BYTE AacsGetAgid(void);
void GenerateDriveChallenge(BYTE *Ptr, AacsAuthenticateInfoS *AacsAuthPtr);
void GenerateDriveKey(BYTE *Ptr, AacsAuthenticateInfoS *AacsAuthPtr);
BOOL ValidateHostChallenge(ChallengeType *Ptr, AacsAuthenticateInfoS *AacsAuthPtr);
BOOL ValidateHostKey(KeyType *Ptr, AacsAuthenticateInfoS *AacsAuthPtr);
void GenerateCmacForKcd(BYTE *Cmac, BYTE BEType, AacsAuthenticateInfoS *AacsAuthPtr);
void ClearKeysStorageMask(void);
BOOL ProcessKeysFromHost(ULONG Addr);
void GetBindingNonce(BYTE *Ptr, StLongU StartLba, BYTE Type);
BOOL ValidateTypeAndVersionRecord(BYTE *DataPtr);
void AACSReportKey(void);
void AACSSendKey(void);
BOOL ExtractMid(BYTE *DataPtr, MidTypeE MidType);
void GenBcmDriveChallenge(BYTE *Ptr, AacsAuthenticateInfoS *AacsAuthPtr);
void GenBcmMediaSN(void);
void GenBcmMediaID(void);
void GetDriveCertificate(BYTE *Ptr);

#endif // __AACS_H__
