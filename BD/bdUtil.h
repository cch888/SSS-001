/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdUtil.h $
*
* DESCRIPTION   Header File for bdUtil.c
*
* $Revision: 10 $
* $Date: 11/03/15 2:21p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/


/*********************************************************************
    FILE DEFINITION
*********************************************************************/
#ifndef __BDUTIL_H__
#define __BDUTIL_H__

#include ".\oem\Oem_info.h"

/*******************************************************************
    EXTERNAL FUNCTIONS / FUNCTION PROTOTYPES
*******************************************************************/
#if 0//currently not be use
extern PSNs  bdConvertADIPtoPSN(ULONG adip);
#endif //currently not be use
extern PSNs  bdConvertLBAtoPSN(ULONG LBA);
extern ULONG bdConvertPSNtoLBA(ULONG PSN);
extern BYTE BDReturnMediaOffsetForRead(void);
extern BYTE SelectBDReadSpeed(BYTE r_no);
extern BYTE VerifyPACRules(BYTE ePACRuleArea, BYTE eCtrlType);
extern void bdConvert_init(void);
#endif
