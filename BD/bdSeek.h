/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdSeek.h $
*
* DESCRIPTION   Header file for bdSeek.c
*
* $Revision: 2 $
* $Date: 08/02/04 10:35a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BDSEEK_H__
#define __BDSEEK_H__

#if (BD_ENABLE)


/*********************************************************************
    EXTERNAL FUNCTIONS
*********************************************************************/
extern BYTE bdSeek(ULONG TargetID, BYTE Layer);
extern void BDSeekCommand();

#endif  //(BD_ENABLE)

#endif
