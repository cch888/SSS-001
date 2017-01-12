/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: dvBufMgr.h $
*
*   DESCRIPTION:   HEADER FILE FOR dvBufMgr.c
*
*   $Revision: 6 $
*   $Date: 08/03/06 4:16p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __DVBUFMGR_H__
#define __DVBUFMGR_H__

/*******************************************************************
    INCLUDE FILES
*******************************************************************/
#include ".\dvd\dvDef.h"
#if (EN_POWER_CONTROL == 1)
#include ".\hif\powerctl.h"
#endif
#if (ENABLE_HDDVD)
#include ".\hd\hdBufMgr.h"
#endif

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/


/*******************************************************************
    DEFINES
*******************************************************************/


/*-----------------------------------------------------------------
    BUFFER DEFINES
------------------------------------------------------------------*/

/* Buffer ranges */
#define DVD_BUFFER_50_PERCENT   (MAX_DVD_SECTORS/2)
#define DVD_BUFFER_25_PERCENT   (MAX_DVD_SECTORS/4)

/* Minimum number of sectors in buffer in order to restart caching */
#define RESTART_READ_AHEAD_THRESHOLD    (MAX_DVD_SECTORS - DVD_BUFFER_25_PERCENT)

/* Current number of DVD sectors to wait to be cached before determining a cache hit */
#define DVD_CACHE_WAIT_RANGE            0x30


/* Modes for cahe search algorithm */
#define WAIT_SEARCH_MODE        0
#define IMMEDIATE_SEARCH_MODE   1
/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void dvdInitializeHostBuffer(ULONG);
extern void dvdAdjustHostBufferPtr(ULONG);
extern SearchCacheResultE dvdSearchCache(TransferS, BYTE);
extern DiscAccessStatusE dvdLoadBuffer(TransferS, TransferTypeE);
extern void dvdCheckReadAhead(void);

#if (EN_POWER_CONTROL == 1)
extern BOOL PowerControl(BYTE);
extern BYTE CurrentPowerMode;
#endif

#endif
