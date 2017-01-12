/****************************************************************************
*                (c) Copyright 2005 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:    dvRdTrack.h
*
*   DESCRIPTION: Header file for dvSeek.c
*
*   $Revision: 8 $
*   $Date: 08/03/06 4:16p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __dvRDDISC_H__
#define __dvRDDISC_H__

#include ".\common\globals.h"

#if (ENABLE_HDDVD)
#include ".\hd\hdRdDisc.h"
#endif

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    DEFINES
*******************************************************************/
#define STANDARD_DISC_INFORMATION   0x00
#define TRACK_RESOURCES_INFORMATION 0x01
#define POW_RESOURCES_INFORMATION   0x02

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void DVDReadDiscInfoCommand(void);

#endif
