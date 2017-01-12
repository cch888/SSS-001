/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdTrack.h $
*
* DESCRIPTION   Header file for bdRdTrack.c
*
* $Revision: 1 $
* $Date: 07/08/10 4:28p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BD_RDTRACK_H__
#define __BD_RDTRACK_H__


/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    DEFINES
*******************************************************************/

//#define INVISIBLE_TRACK         0xFF    //define in dvRdTrack.h

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void BDReadTrackInfoCommand(void);
extern BOOL bdCheckValidTrackNumber(BYTE trackNum);

#endif
