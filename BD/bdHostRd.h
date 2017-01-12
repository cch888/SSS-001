/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: bdHostRd.h $
*
*   DESCRIPTION:
*   BD Read stuff at the Host end of the code.
*
*   $Revision: 1 $
*   $Date: 07/09/21 5:02p $

**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __bdHOSTRD_H__
#define __bdHOSTRD_H__

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

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void BDRead10Command();
extern void bdSetSenseCode( DiscAccessStatusE ErrorStatus);
extern BYTE bdFastCacheTransfer();

/*******************************************************************
    EXTERNAL VARIABLES
*******************************************************************/

#endif
