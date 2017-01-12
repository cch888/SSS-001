/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdVars.h $
*
* DESCRIPTION   Header File for bdVars.c
*
* $Revision: 18 $
* $Date: 09/08/20 6:55p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
    FILE DEFINITION
*****************************************************************************/
#ifndef __BD_VARS_H__
#define __BD_VARS_H__

#include ".\bd\bdDef.h"

#if (BD_ENABLE)
#include ".\defmgmt\bdwritable\bdRanAccDef.h"
/*****************************************************************************
    EXTERNAL FUNCTIONS / FUNCTION PROTOTYPES
*****************************************************************************/
extern tBdDdsS * pDDS;
extern tBdSrriS * pSRRI;
extern tBdPacS * pPAC;
extern BYTE    PACcnt;

extern BDDiscTransferS  volatile bdDiscRequest;
extern BYTE             volatile bdReadRetryCount;
extern ULONG            volatile bdReadAheadStartPSN;
extern ULONG            volatile bdLastDecodedPSN;
extern bdTimerS         volatile bdClusterInterruptTimer;

extern bdDecFlagS       volatile bdDecFlag;
extern bdDecIntFlagS    volatile bdDecIntFlag;
extern bdErrorFlagS     volatile bdErrorFlag;

extern BdwtGlobalS      volatile BdwtDisc;
extern ULONG            volatile LSN0StartPSN;
extern ULONG            volatile LSN0EndPSN;
extern ULONG            volatile Layer0EndLBA;
extern ULONG            volatile LSN1StartPSN;
extern ULONG            volatile LSN1EndPSN;
extern ULONG            volatile LSN1OffsetPSN;

extern ULONG   bdLastReadAheadTarget;
extern BYTE    bdReadAheadTargetRetries;
extern BYTE    bdVideoFlag;
#if (ENABLE_DISCARD_ICE == 1)
extern vBYTE    ICEcnt;
extern vULONG   ICEPSN[MAXICECNT];
#endif
#endif  //(BD_ENABLE)

#endif
