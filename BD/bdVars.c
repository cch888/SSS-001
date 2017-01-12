/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdVars.c $
*
* DESCRIPTION   This file contains BD related variables stored in SDRAM
*
* $Revision: 18 $
* $Date: 09/08/20 6:55p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
 INCLUDES
*****************************************************************************/

#include ".\bd\bdVars.h"

#if (BD_ENABLE)
/*****************************************************************************
 Variables in SDRAM
*****************************************************************************/
tBdDdsS * pDDS;
tBdSrriS * pSRRI;
tBdPacS * pPAC;
/*****************************************************************************
    BD PAC parameter
*****************************************************************************/
BYTE    PACcnt;
/*****************************************************************************
    BD Disc Interface
*****************************************************************************/
BDDiscTransferS     volatile bdDiscRequest;            /* Disc Access Request */
BYTE                volatile bdReadRetryCount;         /* Retry counter */
ULONG               volatile bdReadAheadStartPSN;      /* PSN indicating buffering is now read ahead */
ULONG               volatile bdLastDecodedPSN;         /* Last decoded PSN performed */
bdTimerS            volatile bdClusterInterruptTimer;  /* BD timer for cluster interrupts */
BYTE                bdVideoFlag;                      /* BD vedio flag */
/*****************************************************************************
 BD Flag Structures
*****************************************************************************/
bdErrorFlagS        volatile bdErrorFlag;       /* BD error flags               */
bdDecFlagS          volatile bdDecFlag;         /* BD decoder flags             */
bdDecIntFlagS       volatile bdDecIntFlag;      /* BD decoder interrupt flags   */

/*--------------------------------------------
    Read Ahead Retry
---------------------------------------------*/
ULONG   bdLastReadAheadTarget;
BYTE    bdReadAheadTargetRetries;


/*--------------------------------------------
    BD Writable Global Variables
---------------------------------------------*/
BdwtGlobalS         volatile BdwtDisc;          /* Global Variables */
ULONG            volatile LSN0StartPSN;
ULONG            volatile LSN0EndPSN;
ULONG            volatile Layer0EndLBA;
ULONG            volatile LSN1StartPSN;
ULONG            volatile LSN1EndPSN;
ULONG            volatile LSN1OffsetPSN;

#if (ENABLE_DISCARD_ICE == 1)
/*--------------------------------------------
    ICE detect variables
---------------------------------------------*/
vBYTE    ICEcnt;
vULONG   ICEPSN[MAXICECNT];
#endif
#endif  //(BD_ENABLE)

