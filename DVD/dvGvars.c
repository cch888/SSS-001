/****************************************************************************
*                 (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*   FILENAME:     dvGvars.c
*
*   DESCRIPTION   DVD related Global variables
*
*   $Revision: 27 $
*   $Date: 09/07/30 7:24p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\hif\gp_ram.h"
#include ".\hif\bf_util.h"
#include ".\dvd\dvGvars.h"
#if (VCPS_TEST ==1)
#include ".\dvd\dvvcps.h"
#endif

/*******************************************************************
    DVD GLOBAL VARIABLES
*******************************************************************/

/*--------------------------------------------
    DVD Disc Interface
---------------------------------------------*/
DiscTransferS       volatile dvdDiscRequest;            /* Disc Access Request */
BYTE                volatile dvdReadRetryCount;         /* Retry counter */
ULONG               volatile dvdReadAheadStartPSN;      /* PSN indicating buffering is now read ahead */
ULONG               volatile dvdLastDecodedPSN;         /* Last decoded PSN performed */
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
ULONG               volatile dvdramDecodedStrPSN;
#endif
dvdTimerS           volatile dvdBlockInterruptTimer;    /* DVD timer for block interrupts */
BYTE                volatile SearchErrorRetryCount;     /* Search Error Retry counter */

/*--------------------------------------------
    DVD Bufferring and Disc information
---------------------------------------------*/
struct dvdCssS      volatile dvdCss;            // DVD CSS specific variables.
dvdTimeStampS volatile dvdTimeStamp;            // DVD Time Stamp

BYTE dvdVideoFlag;
#if(Detect_AVCHD_DISC == 1)
BYTE AVCHDFlag;
#endif
/*--------------------------------------------
    DVD Flag structures
---------------------------------------------*/
struct dvdErrorFlagS    volatile dvdErrorFlag;  /* DVD error flags              */
struct dvdDecIntFlagS   volatile dvdDecIntFlag; /* DVD decoder interrupt flags  */
struct dvdDecFlagS      volatile dvdDecFlag;    /* DVD decoder flags            */


/*--------------------------------------------
    DVD Error Logging
---------------------------------------------*/
volatile BYTE *  ErrorLogP;                  // Pointer to entries in Error Log Buffer in DRAM.
StLongU DvdBlerPiCorrected;         // For Diagnostic Block Error logging.
StLongU DvdBlerPiRowError;          // For Diagnostic Block Error logging.
StLongU DvdBlerPiUnCorrected;       // For Diagnostic Block Error logging.
StLongU DvdBlerPoCorrected;         // For Diagnostic Block Error logging.
StLongU DvdBlerPoUnCorrected;       // For Diagnostic Block Error logging.
StLongU DvdBlerIdErrors;            // For Diagnostic Block Error logging.
StLongU DvdBlerEdcErrors;           // For Diagnostic Block Error logging.

/*--------------------------------------------
    Read Ahead Retry
---------------------------------------------*/
ULONG   dvdLastReadAheadTarget;
BYTE    dvdReadAheadTargetRetries;

/*********************************
* VCPS Authentication
********************************/
#if (VCPS_TEST ==1)
BYTE VCPS_disc;
EKBS vcpsDKB;
struct vcpsDataS vcpsData;
#endif


#if (DVD_ZONE_TEST == TRUE)

struct dvdZoneTestS volatile dvdTest;

#endif
