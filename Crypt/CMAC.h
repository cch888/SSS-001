/*****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME: $Workfile: CMAC.h $
*
* DESCRIPTION   Header file for CMAC.c
*
* $Revision: 8 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __CMAC_H__
#define __CMAC_H__

#if (BUILD_IN_AACS_FOLDER == 1)
#include "..\Crypt\aacstype.h"
#else
#include "..\Common\globtype.h"
#endif


/*******************************************************************
    DEFINES
*******************************************************************/
#define SIZE_OF_CIPHER_BLK     128     // Number of bits per cmac cipher block

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void GenerateCmac(BYTE *pKey, BYTE *pMessage, ULONG length, BYTE *pMac);
extern void GenerateSubkeys(BYTE *pKey, BYTE *pSubkey1, BYTE *pSubkey2);

#endif  // __CMAC_H__
