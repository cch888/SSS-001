/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: bdBufMgr.h $
*
*   DESCRIPTION:   HEADER FILE FOR bdBufMgr.c
*
*   $Revision: 1 $
*   $Date: 07/09/21 5:01p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __BDBUFMGR_H__
#define __BDBUFMGR_H__

/*******************************************************************
    INCLUDE FILES
*******************************************************************/
#include ".\bd\bdDef.h"
#if (EN_POWER_CONTROL == 1)
#include ".\hif\powerctl.h"
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
/* Enable/Disable BD background buffer pre-fetching */
#define BD_READ_AHEAD_ENABLED   TRUE

/* Current number of BD sectors to wait to be cached before determining a cache hit */
#define BD_CACHE_WAIT_RANGE            0x60

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern DiscAccessStatusE bdLoadBuffer(TransferS Request, TransferTypeE LoadType);

#endif
