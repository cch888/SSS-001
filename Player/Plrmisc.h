/****************************************************************************
*                (c) Copyright 2001 - 2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrMisc.H
* DESCRIPTION   : This file contains the Misc module definitions
* NOTES         :
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _Misc_H_
#define _Misc_H_

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h" /* Player:Monitor */

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Command */
enum {
    eMiscIdle = eIdle, /* Idle  */
    eMiscTime        , /* Time  */
    eMiscNoEFMOrMode0, /* EFM   */
    eMiscEnd
};

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
/* Command */
typedef struct {
    BYTE  len;          /* Length   */
    BYTE  cmd;          /* Command  */
    BYTE  msgid;        /* MsgId    */
    AddrParmS address;  /* Address */
} tMiscMsg;

extern long ByteLen4ToLong(BYTE *Addr);

extern BYTE fSeekNotReachCnt;

#define eMax_TocRtyCnt  3  /* Toc  Retry Count2      */

#endif /* _Misc_H_ */
