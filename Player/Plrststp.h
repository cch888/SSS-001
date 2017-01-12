/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrStStp.H
* DESCRIPTION   : This file contains the Ststp module definitions
* NOTES         :
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _SSTP_H_
#define _SSTP_H_

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h"

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Command */
enum {
    eSStpIdle = eIdle, /* Idle    */
    eSStpStartUp     , /* Startup */
    eSStpStop        , /* Stop    */
    eSStpEnd           /* End     */
};
//enum {
//  eSStpRdToc = 1        /* ReadToc */
//};

/*************************************************************************/
/*                        Function Prototypes                            */
/*************************************************************************/
extern void plrStrtstpSetIdle(void);      /* Set Idle        */

#endif /* _SSTP_H_ */
