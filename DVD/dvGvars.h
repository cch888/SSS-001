/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: dvGvars.h $
*
* DESCRIPTION
* RAM variable Declaration
*
*   $Revision: 25 $
*   $Date: 09/07/30 7:24p $

**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __dvGVARS_H__
#define __dvGVARS_H__

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\cstart.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_audio.h"
#include ".\hif\bf_util.h"
#include ".\hif\at_cmds.h"
#include ".\servo\svo.h"
#include ".\dvd\dvDef.h"
#include ".\dvd\dvCFG.h"

#if (VCPS_TEST ==1)
#include ".\dvd\dvVCPS.h"
#endif


/* A structure to define non volatile variables located on Flash Prom. */
typedef union
{
    struct
    {
        UINT Start;     /* This is just to have a convenient name to address the first location of the non volatile vars. */
    }Variables;
    struct
    {
        BYTE  volatile  State;                  /* State for RPC State Machine */
        BYTE  volatile  nCount;                 /* User Controlled Region Code Changes Available */
        BYTE  volatile  LuRegion;               /* (Logical Unit) Preferred Drive Region Code      */
        BYTE  volatile  VendorResetsAvailable;  /* Number of Vendor Resets Available           */
    }RPC;       /* These are the variables for Region Playback Control. (RPC) */
}RPC_NON_VOLATILE_VARS;

extern  vBYTE       rd_method;
extern  volatile BYTE *  ErrorLogP;
extern  StLongU DvdBlerPiCorrected;
extern  StLongU DvdBlerPiRowError;
extern  StLongU DvdBlerPiUnCorrected;
extern  StLongU DvdBlerPoCorrected;
extern  StLongU DvdBlerPoUnCorrected;
extern  StLongU DvdBlerIdErrors;
extern  StLongU DvdBlerEdcErrors;
extern  ULONG volatile      dvdReadAheadStartPSN;
extern  ULONG volatile      dvdLastDecodedPSN;
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
extern  ULONG volatile      dvdramDecodedStrPSN;
#endif
extern dvdTimerS volatile dvdBlockInterruptTimer;
extern struct dvdErrorFlagS     volatile    dvdErrorFlag;
extern struct dvdDecIntFlagS   volatile    dvdDecIntFlag;
extern struct dvdDecFlagS      volatile    dvdDecFlag;
extern struct dvdDiscS          volatile    dvdDisc;
extern struct dvdCssS           volatile    dvdCss;
extern dvdTimeStampS            volatile    dvdTimeStamp;
extern struct dvdRAIS    volatile    dvdRAI;
extern struct dvdEncFlagS       volatile    dvdEncFlag;
extern struct dvdEncErrorFlagS  volatile    dvdEncErrorFlag;
extern struct dvdSessionS   volatile  dvdSession;
extern BYTE DVDSessionNum;
extern BYTE dvdVideoFlag;
#if(Detect_AVCHD_DISC == 1)
extern BYTE AVCHDFlag;
#endif
extern DiscTransferS volatile dvdDiscRequest;
extern BYTE volatile dvdReadRetryCount;
extern BYTE volatile SearchErrorRetryCount;
extern  ULONG   dvdLastReadAheadTarget;
extern  BYTE    dvdReadAheadTargetRetries;

#if (DVD_ZONE_TEST == TRUE)
extern struct dvdZoneTestS volatile dvdTest;
#endif

#if (E_DIAGS == 1)
#define PI_C        DvdBlerPiCorrected.word.l
#define PI_E        DvdBlerPiRowError.word.l
#define PI_UN_C     DvdBlerPiUnCorrected.word.l
#define PO_C        DvdBlerPoCorrected.word.l
#define PO_UN_C     DvdBlerPoUnCorrected.word.l
#define READ_PI_PO  DvdBlerEdcErrors.byte.l
#define WAIT_PI_PO  DvdBlerEdcErrors.byte.m
#define PIPO_CURRENT_ID     DvdBlerIdErrors
#endif //#if (E_DIAGS == 1)

/*********************************
* VCPS Authentication
********************************/
#if (VCPS_TEST ==1)
extern BYTE VCPS_disc;
extern EKBS vcpsDKB;
extern struct vcpsDataS vcpsData;
#endif
#endif
