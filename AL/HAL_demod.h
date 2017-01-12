/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_demod.h
*
* DESCRIPTION
*   This file supports the HAL DEMOD funtions implemented in the DEMOD HAL layer
*
* NOTES:
*
* $Revision: 7 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_DEMOD_H_
#define _HAL_DEMOD_H_

#include ".\servo\SVO.H" /* Needed for types of parameters */

typedef enum 
{
    AM_SYNC_CONFIDENCE_LEVEL01,
    AM_SYNC_CONFIDENCE_LEVEL02,
    AM_SYNC_CONFIDENCE_LEVEL03,
    AM_SYNC_CONFIDENCE_LEVEL04,
    AM_SYNC_CONFIDENCE_LEVEL05,
    AM_SYNC_CONFIDENCE_LEVEL06,
    AM_SYNC_CONFIDENCE_LEVEL07,
    AM_SYNC_CONFIDENCE_LEVEL08        
} eAMSyncConfidenceLevelT;


//*** Global definitions regarding the AFE HAL functionality

//*** External function prototypes
extern BOOL HAL_DEMOD_GetFePllLocked(void);
extern void HAL_DEMOD_Init( void );




#endif /* _HAL_DEMOD_H_ */
