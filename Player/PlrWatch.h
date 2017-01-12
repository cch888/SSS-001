/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrWatch.H
* DESCRIPTION   : This file contains the Watch module definitions
* NOTES         :
*
* $Revision: 7 $
* $Date: 11/03/16 6:00p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _Watch_H_
#define _Watch_H_
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h"

/*************************************************************************/
/*                        Data type definitions                          */
/*************************************************************************/
/* Command */
enum {
    eWatch_Idle  = eIdle    , /* Idle               */
    eWatch_ServoStatus      , /* Servo Status       */
    eWatch_Seek             , /* Seek               */
    eWatch_ISRC             , /* ISRC               */
    eWatch_ServoReadyStatus , /* Servo Ready Status */
    eWatch_SubCodeOk        , /* Sub Code Ok        */
    eWatch_CycleTask        , /* Cycle Task         */
    eWatch_End
};

/* Find Subcode */
enum {
    eNOT_FOUND     = 0x00, /* not found a subcode                 */
    eFOUND_LEADIN  = 0x01, /* found a subcode at leadin  position */
    eFOUND_PROGRAM = 0x02, /* found a subcode at program position */
    eFOUND_LEADOUT = 0x03, /* found a leadout                     */
    eFOUND_MODE0   = 0x04, /* found Mode 0 subcode                */
    eFOUND_UNKNOWN = 0x10, /* found a unknown subcode             */
    eREAD_ERROR    = 0x20  /* found a reading or CRC error        */
};

/*************************************************************************/
/*                         Typedefs                                      */
/*************************************************************************/
/* Servo Command */
typedef struct {
    BYTE Cmd;      /* Command   */
    BYTE Len;      /* Length    */
    BYTE Para[10]; /* Parameter */
} tWatchSC;

/*************************************************************************/
/*                        Function Prototypes                            */
/*************************************************************************/
extern void plrWatchRequest(BYTE Request,BYTE MsgId,BYTE *Data); /* Request */
extern void plrCycleRequest(BYTE MsgId,BYTE (*Task)(void));      /* Cycle   */

extern  BYTE plrHIFWatchReq(BYTE Request,BYTE *Data);

extern  BYTE fSeekNotReachCnt;

#endif /* _Watch_H_ */
