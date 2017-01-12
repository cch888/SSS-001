/****************************************************************************
*                (c) Copyright 2001 - 2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrTray.h
* DESCRIPTION   : This file contains the Tary module definitions
* NOTES         :
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00   
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef _PLRTRAY_H_
#define _PLRTRAY_H_

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h" /* Player:Monitor */

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Command */
enum {
    eTrayIdle = eIdle, /* Idle */
    eTrayOut         , /* Out  */
    eTrayIn          , /* In   */
    eTrayEnd           /* End  */
};

#endif /* _PLRTRAY_H_ */
