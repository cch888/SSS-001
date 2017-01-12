/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrInfo.h
*
* DESCRIPTION   : This file contains the Info module definitions
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _Info_H_
#define _Info_H_
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h"

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Command */
enum {
    eInfoIdle = eIdle , /* Idle           */
    eInfoReadTOC      , /* Read TOC       */
    eInfoReadPMA      , /* Read PMA       */
    eInfoReadDiscInfo , /* Read DiscInfo  */
    eInfoCheckSessions, /* Check Sessions */
    eInfoSortPMA      , /* Sort PMA       */
    eInfoCheckPMA     , /* Check PMA      */
    eInfoEnd            /* End            */
};
/*************************************************************************/
/*                        Public variables                          */
/*************************************************************************/
extern BYTE PMANotOk; 	        

#endif /* _Info_H_ */
