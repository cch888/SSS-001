/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*    FILE NAME:     $Workfile: dvDiscRd.h $
*
*    DESCRIPTION:   Header file for dvDiscRd.c.c
*
*   $Revision: 33 $
*   $Date: 09/01/30 6:24p $
*
************************** SUNEXT CONFIDENTIAL ********************************/
#ifndef __DVDISCRD_H__
#define __DVDISCRD_H__


#include "./dvd/dvDef.h"


/*******************************************************************
    DEFINES
*******************************************************************/
#define MAX_DVD_READ_RETRIES    	5

#if (CEProduct == 1)
#define FIRST_BLOCK_TIMEOUT_VALUE   SECONDS_4
#define DVD_BLOCK_TIMEOUT_VALUE     MSEC_340
#define DVDRAM_BLOCK_TIMEOUT_VALUE  SECONDS_4
#define MAX_DVD_READ_RETRIES_FOR_CE        3  /* CE: CONSUMER ELECTRIC PRODUCT */
#define MAX_DVDRAM_READ_RETRIES            8  /* CE: CONSUMER ELECTRIC PRODUCT */
#define MAX_SEARCH_ERROR_RETRY_COUNTER     4
#define DVD_READ_LEADIN_TIMEOUT_VALUE   1000 /*1sec*/
#else
#define FIRST_BLOCK_TIMEOUT_VALUE   SECONDS_4
#define DVD_BLOCK_TIMEOUT_VALUE     MSEC_200
#define MAX_DVD_READ_RETRIES_FOR_CE        3  /*   */
#define DVDRAM_BLOCK_TIMEOUT_VALUE  SECONDS_4
#define MAX_DVDRAM_READ_RETRIES            8  /*   */
#define MAX_SEARCH_ERROR_RETRY_COUNTER     4
#define DVD_READ_LEADIN_TIMEOUT_VALUE   1000 /*1sec*/
#endif

#if (ENABLE_HDDVD)
#define HD_BLOCK_TIMEOUT_VALUE     MSEC_500
#endif

#define MAX_DVD_READ_RMA_RETRIES     2
#define MAX_VALUE_OF_BKTOBUF        0x3FF   /* Max value for 10 bits = 1024 */
#define MAX_VALUE_OF_BLKINC         0xFF    /* Max value for 8 bits = 255 */
#define MIN_BKTOBUF_FOR_EXTEND      0x01    /* Minimum value of BKTOBUF to write into BLKINC */

/*******************************************************************
    MACROS
*******************************************************************/
#if (DVD_ZONE_TEST == FALSE)

    #define DVD_BLK_ERROR       dvdErrorFlag.fSEQ_ID_ERROR      ||  \
                            dvdErrorFlag.fUNCOR_ECC         ||  \
                            dvdErrorFlag.fBCA_ERROR         ||  \
                            dvdErrorFlag.fEDC_ERROR         ||  \
                            dvdErrorFlag.fUNAUTH_READ_ERROR ||  \
                            dvdErrorFlag.fID_MISMATCH       ||  \
                                dvdErrorFlag.fID_ERROR


    #define DVD_BUFFER_ERROR    dvdErrorFlag.fSEEK_FAIL         || \
                            dvdErrorFlag.fSYNC_LOST         ||  \
                            dvdErrorFlag.fTID_MISS          ||  \
                            dvdBlockInterruptTimer.fTIMEOUT

#else

    #define DVD_BUFFER_ERROR    dvdErrorFlag.fSEEK_FAIL         ||  \
                            dvdErrorFlag.fSYNC_LOST         ||  \
                                dvdErrorFlag.fTID_MISS          ||  \
                                dvdBlockInterruptTimer.fTIMEOUT

    #define DVD_BLK_ERROR       FALSE

#endif

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void dvdStopDiscTransfer(void);
extern DiscAccessStatusE dvdStartDiscTransfer(void);
extern void dvdInitFlags(void);
extern void dvdResetErrorFlags(void);
extern void dvdResetDiscFlags(void);
extern void dvdInitializeDiscTransfer(void);

#endif
