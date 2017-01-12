/****************************************************************************
*                (c) Copyright 2001, 2002, 2003, 2004 SUNEXT DESITN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_key.h
*
* DESCRIPTION
* This file contains command handler of ATAPI command
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_KEY_H__
#define __GP_KEY_H__

#include ".\common\error.h"
#include ".\player\plrdb.h"

#define EJECT_KEY_TRAY_RECOVERY_MAX_COUNT  1
#define EJECT_KEY_PRESSED_VALID_TIME    2
#define TRAY_PUSHED_VALID_TIME    2
#if (USE_TWO_KEY_FUNCTION==1)
#define PLAYSW_VALID_TIME   5
#endif


typedef enum
{
    TRAY_COMMAND_COPIED,
    TRAY_COMMAND_TRAY_OUT,
    TRAY_COMMAND_TRAY_IN
}TrayCommand_E;

typedef enum
{
    TRAY_STATE_UNKNOW,
    TRAY_STATE_OUT_OK,
    TRAY_STATE_OUT_EXECUTING,
    TRAY_STATE_OUT_ERROR,
    TRAY_STATE_IN_OK,
    TRAY_STATE_IN_EXECUTING,
    TRAY_STATE_IN_ERROR
}TrayStatus_E;

typedef enum
{
    TRAY_PREVENT_ALLOW_STATE_UNLOCKED,
    TRAY_PREVENT_ALLOW_STATE_LOCKED,
    TRAY_PREVENT_ALLOW_STATE_PERSISTENT_ALLOW,
    TRAY_PREVENT_ALLOW_STATE_PERSISTERN_PREVENT
}TrayPreventAllowState_E;

__packed struct TrayManagerInfo_S
{
    BYTE    Executing;
    BYTE    Pushed;
    BYTE    EjectRequestEvent;
    BYTE    PersistentPreventState;
    TrayCommand_E Command;
    TrayStatus_E TrayState;
    TrayPreventAllowState_E TrayPreventAllowState;
    BOOL TrayErrorSkipStartUp;
};

struct ButtonInfo_S
{
    BYTE Disable;
    BYTE Pressed;
    BYTE PressedTime;
    BYTE PressedValid;
};

extern void CheckPushedTray(void);
extern void CheckEjectKey(void);
#if (USE_TWO_KEY_FUNCTION==1)
extern void CheckPlaySwitch(void);
#endif

extern struct TrayManagerInfo_S TrayManagerInfo;
#if EPO_SLOT_IN
extern BYTE disc_inside;
extern BYTE disc_outside;
#endif

#endif /* __GP_KEY_H__ */
