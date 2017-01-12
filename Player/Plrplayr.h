/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrPlayer.h
* DESCRIPTION   : This file contains the Player module definitions
*
*   $Revision: 12 $
*   $Date: 08/09/18 12:01a $
*
* NOTES         :
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef _PLRPLAYR_H_
#define _PLRPLAYR_H_
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h" /* Player:Monitor */

#include ".\DVD\dvCFG.h"

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Host IF Subsystem interface Command Id */
typedef unsigned char HostIfCommandT;
enum {
    plrCmdIdle = eIdle     , /* Idle                */
    plrCmdTrayOut          , /* Tray Out            */
    plrCmdTrayIn           , /* Tray In             */
    plrCmdStop             , /* Stop                */
    plrCmdStartUp          , /* Start Up            */
    plrCmdPauseOn          , /* Pause On            */
    plrCmdSearchISRCCatalog, /* Search ISRC Catalog */
    plrCmdJumpToAddress    , /* Jump To Address     */
    plrCmdSpeedChg         , /* Speed Chg           */
    plrCmdLeadIn           , /* Lead In             */
    plrCmdReAdjust         , /* Readjust the Disc Type */
    plrCmdPassTrough       , /* Pass-Through to Servo */       // Support for Player command Servo Pass Through.
    plrCmdErrorReply       , /* Error Reply         */
    plrCmdEnd
};

/* Player acceptance type */
typedef unsigned char plrAcceptanceT;
enum {
    plrAcceptCmd , /* command accepted by player module       */
    plrNewCmd    , /* new command loaded in interface buffers */
    plrIdleCmd     /* Idle                                    */
};

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
/* Command */
typedef struct {
    BYTE  len;      /* Length   */
    BYTE  cmd;      /* Command  */
    BYTE  msgid;    /* MsgId    */
    BYTE  data[5];  /* Data     */
} tPlayerMsg;

extern void plrPlayerInit(void);
#endif /* _PLRPLAYR_H_ */
