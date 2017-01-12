/****************************************************************************
*                (c) Copyright 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: dvRdTrack.h $
*
*   DESCRIPTION:
*   Header file for dvSeek.c
*
*   $Revision: 7 $
*   $Date: 08/01/30 2:41p $

**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __dvRDTRACK_H__
#define __dvRDTRACK_H__

#include ".\common\globals.h"


/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    DEFINES
*******************************************************************/
#define DVDPLUSR_PACKETSIZE 0x10
#define DVDMINUSR_PACKETSIZE 0x10
#define DVDPLUSR_LEADINSIZE 0x3A00  // 14848


#define INVISIBLE_TRACK         0xFF

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void DVDReadTrackInfoCommand(void);
extern BOOL dvdCheckValidTrackNumber(BYTE trackNum);
#if (ENABLE_HDDVD)
extern void HDReadTrackInfoCommand(void);
#endif

#endif
