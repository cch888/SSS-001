/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*    FILE NAME:     $Workfile: bdDiscRd.h $
*
*    DESCRIPTION:   Header file for bdDiscRd.c
*
*   $Revision: 11 $
*   $Date: 11/01/03 11:21a $
*
************************** SUNEXT CONFIDENTIAL ********************************/
#ifndef __BDDISCRD_H__
#define __BDDISCRD_H__


#include "./bd/bdDef.h"


/*******************************************************************
    DEFINES
*******************************************************************/
#define FIRST_CLUSTER_TIMEOUT_VALUE     SECONDS_4
#define BD_CLUSTER_TIMEOUT_VALUE        MSEC_500

#if (XDFCT_ENABLE == 1)
#define MAX_BD_READ_RETRIES             230
#else
#if (CEProduct == 1)
#define MAX_BD_READ_RETRIES    	        8
#else
#define MAX_BD_READ_RETRIES    	        5
#endif
#endif

#define BD_MIN_BKTOBUF_FOR_EXTEND       0x01


/*******************************************************************
    MACROS
*******************************************************************/

#define BD_CLUSTER_ERROR    bdErrorFlag.fSEQ_ID_ERROR      ||  \
                            bdErrorFlag.fUNCOR_LDC_ECC     ||  \
                            bdErrorFlag.fUNCOR_BIS_ECC     ||  \
                            bdErrorFlag.fBCA_ERROR         ||  \
                            bdErrorFlag.fEDC_ERROR         ||  \
                            bdErrorFlag.fUNAUTH_READ_ERROR ||  \
                            bdErrorFlag.fID_MISMATCH       ||  \
                            bdErrorFlag.fID_ERROR


#define BD_BUFFER_ERROR     bdErrorFlag.fSEEK_FAIL         ||  \
                            bdErrorFlag.fTID_MISS          ||  \
                            bdClusterInterruptTimer.fTIMEOUT


/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void bdStopDiscTransfer(void);
extern DiscAccessStatusE bdStartDiscTransfer(void);
extern void bdInitFlags(void);
extern void bdResetErrorFlags(void);
extern void bdResetDiscFlags(void);
extern void bdInitializeDiscTransfer(void);
extern ULONG GetUcdPointer(WORD PageNo);
#if (XDFCT_ENABLE == 1)
extern void bdStartSpecialDiscHandle(ULONG blk_start_psn, BYTE BdwtRetryCnt);
extern void bdStopSpecialDiscHandle(void);
#endif
#endif
