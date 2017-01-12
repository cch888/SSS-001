/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrTRAY.c
* DESCRIPTION   : his sub system handles the tray in/out state machine.
* NOTES         :
*
* $Revision: 7 $
* $Date: 08/12/16 4:12p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\error.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDB.h"
#include ".\player\plrTray.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"
#include ".\oem\oem_spd_tbl.h"
/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
/* Tray Out */
private BYTE Out(void); private BYTE Out_01(void);  private BYTE Out_01E(void);
/* Tray In  */
private BYTE In(void);  private BYTE In_01(void);   private BYTE In_01E(void);

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/
/* Tray Out */
private const tTask fOut[] = {
/*             Ready  Error */
/* Phase00 */ {0     ,0      },
/* Phase01 */ {Out_01,Out_01E},
};

/* Tray In */
private const tTask fIn[] = {
/*             Ready Error */
/* Phase00 */ {0    ,0     },
/* Phase01 */ {In_01,In_01E},
};

/* Command Table */
public const tCmd fTrayTask[] = {
    {(LPCHAR) "Idle",0  ,0   }, /* Idel     */
    {(LPCHAR) "Out_",Out,fOut}, /* Tray Out */
    {(LPCHAR) "In__",In ,fIn }, /* Tray In  */
};

/************************************************************************
// Name        : Out
// Description : decide tray position via get tray switch status
//               check if need a real tray movement, if not return READY
//               stop tray movement
//               initialize servo
//               tray disc out
//               decide tray position via get tray switch status
//               if tray switch close or timeout, stop tray movement
//               clear eeprom value
//               Set new tray status
*************************************************************************/
private BYTE Out(void)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    pdbSetPlayerStatus(eStTrayMovingOut);
    /* Servo Watch */
    ServoCmd.Cmd     = svoCmdTray;
    ServoCmd.Len     = 1;
    ServoCmd.Para[0] = eMoveTrayOut;
    plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd)
-------------------------------------------------------------------------*/
private BYTE Out_01(void)
{
    pdbSetPlayerStatus(eStTrayOut);
    return(Ready);
}

private BYTE Out_01E(void)
{
    pdbSetErrorGroupTray(eErrorTrayOut);
    return(Error);
}

/************************************************************************
// Name        : In
// Description : decide tray position via get tray switch status
//             * check if need a real tray movement, if not return READY
//             * stop tray movement
//             * tray disc in
//             * decide tray position via get tray switch status
//             * if tray switch close or timeout, stop tray movement
//             * Set new tray status
*************************************************************************/
private BYTE In(void)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    pdbSetPlayerStatus(eStTrayMovingIn);
    ServoCmd.Cmd     = svoCmdTray;
    ServoCmd.Len     = 1;
    ServoCmd.Para[0] = eMoveTrayIn;
    /* Servo Watch */
    plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd)
-------------------------------------------------------------------------*/
private BYTE In_01(void)
{
    pdbSetClearAll();                       /* Initialize database */
    /* Speed Set Only (Non Watch) */
    plrServoSetSpeed(eCDSpinUpSpeed,ePhaseInc);
    plrWatchReq(ePhase01,eWatch_Idle,0);

    pdbSetSpeed(eCDSpinUpSpeed); //TBD? should set hardcode 4x CLV here for all disc?
    pdbSetPlayerStatus(eStTrayIn);
    return(Ready);
}

private BYTE In_01E(void)
{
    pdbSetErrorGroupTray(eErrorTrayIn);
    return(Error);
}
