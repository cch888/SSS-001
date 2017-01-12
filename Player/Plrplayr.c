/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrPlayr.c
* DESCRIPTION   : This file contains the main controller of the player
*                 sub-system,and manages a number of sub-sytems.
* NOTES         :
*
* $Revision: 35 $
* $Date: 11/03/28 2:33p $
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
#include ".\common\commath.h"
#include ".\hif\mmc.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDB.h"
#include ".\player\plrStStp.h"
#include ".\player\plrPlay.h"
#include ".\player\plrTray.h"
#include ".\player\plrInfo.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"
#if (SERVICE_MODE==1)
#include ".\player\plrmon.h"
#endif
#include ".\oem\oem_spd_tbl.h"
/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
private void PlayerReq(USHORT Cmd,BYTE Id,LPBYTE data,BYTE len);
private BYTE PlayerCmd(void);
private BYTE PlayerReady(void);     private BYTE PlayerError(void);

private BYTE ActAfterError(void);
private BYTE AfterError_01(void);   private BYTE AfterError_02(void);
private BYTE AfterError_03(void);   private BYTE AfterError_04(void);
private BYTE AfterError_05(void);   private BYTE AfterError_06(void);
private BYTE AfterError_07(void);   private BYTE AfterError_08(void);
private BYTE AfterError_09(void);   private BYTE AfterError_12(void);

private BYTE MonitorServo(void);
private BYTE MonitorServo_01(void); private BYTE MonitorServo_02(void);
private void MonitorServo_End(void);
#if (SERVICE_MODE==1)
extern tTaskInfo fTaskInfo[];
#endif
/*************************************************************************/
/*                         External Functions                            */
/*************************************************************************/
extern void svoPowerOnInit(void);

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
enum {
    eSvmnIdle = eIdle, /* Idle  */
    eSvmnStart       , /* Start */
    eSvmnEnd
};

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
/* Command */
typedef struct {
    USHORT   cmd[4];
} tPlayerMsgCmd;

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/

public  HostInterfaceInfoS HIF;
BYTE    PlayerSearchCode;
private BYTE fPrePlayerCmd; /* Pre Command     */
private BYTE fPlayerCmd;    /* Command         */
private BYTE fPlayerStatus; /* State           */

#define eS(x) x + eMsgID_SStp * 0x100   /* Start Stop    */
#define eP(x) x + eMsgID_Play * 0x100   /* Play          */
#define eT(x) x + eMsgID_Tray * 0x100   /* Tray          */

private const tPlayerMsgCmd fPlayerMsgCmd[] =
{
/*---Phase00----,-----Phase01-----,------Phase02--------,03,*/
 {0             ,0                ,0                    , 0},
 {eP(ePlayStop) ,eS(eSStpStop)    ,eT(eTrayOut)         , 0},
 {eP(ePlayStop) ,eT(eTrayIn)      ,0                    , 0},
 {eP(ePlayStop) ,eS(eSStpStop)    ,0                    , 0},
 {eP(ePlayStop) ,eS(eSStpStartUp) ,0                    , 0},
 {eP(ePlayStop) ,eS(eSStpStartUp) ,eP(ePlayPauseOn)     , 0},
 {eP(ePlayStop) ,eS(eSStpStartUp) ,eP(ePlayFindISRCCode), 0},
 {eP(ePlayStop) ,eS(eSStpStartUp) ,eP(ePlayJumpAddress) , 0},
 {eP(ePlayStop) ,eP(ePlaySpeedChg),0                    , 0},
 {eP(ePlayStop) ,eS(eSStpStartUp) ,eP(ePlayLeadIn)      , 0},
 {eP(ePlayReAdj),0                ,0                    , 0},
 {eP(ePlayPassThrough),0          ,0                    , 0},           /* Support for Player command Servo Pass Through.  */
 {0             ,0                ,0                    , 0},
};

/* Task Table */
private const tTask fPlayer[] = {
/*             Ready       Error      */
/* Phase00 */ {0          ,0          },
/* Phase01 */ {PlayerReady,PlayerError},
/* Phase02 */ {PlayerReady,PlayerError},
/* Phase03 */ {PlayerReady,PlayerError},
};

/* Command Table */
public const tCmd fPlayerTask[] = {
    {(LPCHAR) "Idle       ",0        ,0      }, /* Idle                */
    {(LPCHAR) "Tray Out   ",PlayerCmd,fPlayer}, /* Tray Out            */
    {(LPCHAR) "Tray In    ",PlayerCmd,fPlayer}, /* Tray In             */
    {(LPCHAR) "Stop       ",PlayerCmd,fPlayer}, /* Stop                */
    {(LPCHAR) "Start Up   ",PlayerCmd,fPlayer}, /* Start Up            */
    {(LPCHAR) "Pause On   ",PlayerCmd,fPlayer}, /* Pause On            */
    {(LPCHAR) "Search ISRC",PlayerCmd,fPlayer}, /* Search ISRC Catalog */
    {(LPCHAR) "JumpToAdrs ",PlayerCmd,fPlayer}, /* Jump To Address     */
    {(LPCHAR) "Speed Chg  ",PlayerCmd,fPlayer}, /* Speed Chg           */
    {(LPCHAR) "Lead In    ",PlayerCmd,fPlayer}, /* Lead In             */
    {(LPCHAR) "Re Adjust  ",PlayerCmd,fPlayer}, /* ReAdjust Disc type  */
    {(LPCHAR) "PassThrough",PlayerCmd,fPlayer}, /* Servo Pass-Through Command  */   // Support for Player command Servo Pass Through.
    {(LPCHAR) "Error Reply",PlayerReady,fPlayer}, /* Error Reply       */
};

/* Task Table */
private const tTask fSvmnMonTask[] = {
/*             Ready           Error */
/* Phase00 */ {0              ,0     },
/* Phase01 */ {MonitorServo_01,0     },
/* Phase02 */ {MonitorServo_02,0     },
};

/* Command Table */
public const tCmd fSvmnTask[] = {
    {(LPCHAR) "Idle",0           ,0           }, /* Idle    */
    {(LPCHAR) "Star",MonitorServo,fSvmnMonTask}, /* Startup */
};

/************************************************************************
// Name       : plrPlayerInit
// Description: Initial player and servo module
*************************************************************************/
public void plrPlayerInit(void)
{
    BYTE i;

    svoPowerOnInit();                     /* Reset servo subsystem */
    pdbSetPlayerStatus(eStInitializing);  /* set player status */

    /* Initialize all state machines */
    fPlayerCmd = fPrePlayerCmd = plrCmdIdle;
    fPlayerStatus = Ready;

    /* Servo Cmd Init */
    SIF.ServoCommand = svoCmdIdle;
    for(i = 0;i < 10;i++){
        SIF.ServoParameters[i] = 0;
    }
    pdbSetClearAll();                   /* Initialize database        */
    pdbSetSpeed(eCDSpinUpSpeed);        /* Default disc speed setting *///TBD? should set hardcode 4x CLV here for all disc
    HIF.PlayerStatus = Ready;
    HIF.PlayerAccept = plrIdleCmd;
}

/************************************************************************
// Name       : plrPlayerMain
// Description:
*************************************************************************/
public void plrPlayerMain(void)
{

#if SAVE_LOG
    BYTE logmsg[16];
#endif /* SAVE_LOG */

    if((svoIramVar.emergencyStopFlag == TRUE) || (!svoIramVar.knlTimeupCount)) {
        if(SIF.ServoExecutionState == svoBusy){
            return;
        }
        if(fPlayerCmd == plrCmdStartUp){
            plrTaskInit(eMsgID_SStp);
        }else{
            plrTaskClr(eMsgID_SStp);
            plrMsgBoxClr(eMsgID_SStp);
        }
        HIF.PlayerStatus = Error;
        HIF.PlayerAccept = plrIdleCmd;
        fPlayerCmd = fPrePlayerCmd = plrCmdIdle;
        fPlayerStatus = Ready;
        plrTaskInit(eMsgID_Player);
        plrTaskInit(eMsgID_Svmn);
        plrTaskInit(eMsgID_Play);
        plrTaskInit(eMsgID_Info);
        plrTaskInit(eMsgID_Misc);
        plrTaskInit(eMsgID_Watch);
        plrTaskInit(eMsgID_Tray);
        svoIramVar.opcIsrEnableFlag = 0;
    }

    /* if a new command is present in the interface field */
    if(HIF.PlayerAccept != plrNewCmd){
        return;
    }
    /* Player Command */
    if((HIF.PlayerIDC & 0xF0) != 0xD0){
        HIF.PlayerAccept = plrIdleCmd;
        HIF.PlayerStatus = Error;
        return;
    }
    if(HIF.PlayerOpcode >= plrCmdEnd){
        HIF.PlayerAccept = plrIdleCmd;
        HIF.PlayerStatus = Error;
        return;
    }

    HIF.PlayerAccept  = plrAcceptCmd;
    HIF.PlayerStatus  = Busy;

//    send_msg81(SEND_HIF_CMD,0xA00000+(HIF.PlayerOpcode<<8),HIF.PlayerIDC);
    SendMsg80(SEND_HIF_CMD,0xA00000+(HIF.PlayerOpcode<<8));
    PlayerReq(HIF.PlayerOpcode + eMsgID_Player * 0x100,eMsgID_Non,
                                            HIF.PlayerParameters,(HIF.PlayerIDC & 0x0F));
#if SAVE_LOG
    plrMemcpy((LPBYTE) &logmsg[0],(LPBYTE) "*HIF>",5);
    plrMemcpy((LPBYTE) &logmsg[5],(LPBYTE) fPlayerTask[HIF.PlayerOpcode].logmsg,11);
    plrWriteLog((LPCHAR ) logmsg,16);
#endif /* SAVE_LOG */

}

/************************************************************************
// Name        : PlayerReq
// Description : This is a continuous process Player Request.
*************************************************************************/
private void PlayerReq(USHORT Cmd,BYTE Id,LPBYTE data,BYTE len)
{
    tPlayerMsg msg;  /* Message    */
    BOOL       ecod; /* Error Code */
    BYTE       i;

    /* Misc Request */
    msg.cmd      = (BYTE) (Cmd & 0x00ff);
    msg.msgid    = Id;
    for(i = 0;i < sizeof(msg.data);i++){
        if(i < len){
            msg.data[i] = data[i];
        }
        else{
            msg.data[i] = 0;
        }
    }
    ecod = plrSndMsg((BYTE) (Cmd / 0x100),(LPBYTE) &msg,sizeof(msg));
}

/************************************************************************
// Name       : PlayerCmd
// Description: This function is state machine of the plrPlayer module.
*************************************************************************/
public BYTE PlayerCmd(void)
{
    USHORT       cmd;
    tPlayerMsg *msg = (tPlayerMsg *) &fPlrMsg;
    TimeS      tmpTime;
    LONG       tmpLBA;
    AddrParmS  tmpAdr;
    StWordU DK;

    cmd = fPlayerMsgCmd[msg->cmd].cmd[ePhase00];
    if(cmd){
        fPlayerCmd = msg->cmd;
        switch(msg->cmd){
//          case plrCmdAudioPlay:
            case plrCmdJumpToAddress:
                /* start time */
                pdbSetSearchAddress(*((AddrParmS *) msg->data));
                fJumpMode = msg->data[4];
                break;
            case plrCmdSearchISRCCatalog:
                PlayerSearchCode = msg->data[1];
                pdbSetCommandTrack(msg->data[0]);
                tmpTime = pdbGetStartTime(msg->data[0]);
                tmpLBA  = MSF2LBA_Plus(&tmpTime);
                tmpAdr.Addr[0] = (char)((char *)&tmpLBA)[3];
                tmpAdr.Addr[1] = (char)((char *)&tmpLBA)[2];
                tmpAdr.Addr[2] = (char)((char *)&tmpLBA)[1];
                tmpAdr.Addr[3] = (char)((char *)&tmpLBA)[0];
                pdbSetSearchAddress(tmpAdr);
                break;
            case plrCmdStartUp:
                fRawModeWritten = (msg->data[0] == 1) ? TRUE:FALSE;
                break;
            case plrCmdSpeedChg:
                fEnableROC = msg->data[0];
                fRecoverLevel = msg->data[1];
                break;
            case plrCmdReAdjust:
                DK.byte.l = msg->data[0];
                DK.byte.h = msg->data[1];
                fReAdjustType.fDiscKind = (eDiscKind)DK.Uword;
                break;
            case plrCmdPassTrough:                                // Support for Player command Servo Pass Through.
#if (SERVICE_MODE==1)
                fTaskInfo[eMsgID_Play].cmd = ePlayEnd;
#endif
                break;
        }
        PlayerReq(cmd,eMsgID_Player,0,0);
        plrSetPhase(ePhase01);
        return(Busy);
    }
    else{
        return(Error);
    }
}

/************************************************************************
// Name       : PlayerReady
// Description: This function is state machine of the plrPlayer module.
*************************************************************************/
public BYTE PlayerReady(void)
{
    USHORT  cmd;

    /* task Request */
    cmd = fPlayerMsgCmd[fPlayerCmd].cmd[plrGetPhase()];
    if(cmd) {
        PlayerReq(cmd,eMsgID_Player,0,0);
        plrSetPhase(ePhaseInc);
        return(Busy);
    }

    plrSetPhase(ePhase00);
    if(fPlayerStatus == Error){
        HIF.PlayerStatus = Error;
    }
    else{
        HIF.PlayerStatus = Ready;
    }
    HIF.PlayerAccept  = plrIdleCmd;
    fPlayerCmd = plrCmdIdle;
    fPlayerStatus = Ready;
    return(Ready);
}

/************************************************************************
// Name       : PlayerError
// Description: This function is state machine of the plrPlayer module.
*************************************************************************/
public BYTE PlayerError(void)
{
    fPlayerStatus = Busy;
    fPrePlayerCmd = fPlayerCmd;
    fPlayerCmd = plrCmdIdle;
    plrSetPhase(ePhase00);
    PlayerReq(eSvmnStart + eMsgID_Svmn * 0x100,eMsgID_Non,0,0);
    return(Busy);
}


/************************************************************************
// Name       : ActAfterError
// Description: When an error occurred, some recover action must be done.
//              This routine will perform the same action as
//              only in the case of an error situation.
*************************************************************************/
private BYTE ActAfterError(void)
{
    BYTE error_num;
    BYTE i;
    BYTE *p;

    /* copy errors to interface field */
    p = HIF.PlayerParameters;
    p[ 0] = 0x00;          /* Command Execution results = No error */
    p[ 1] = pdbServoGetServoError();    /* Servo error in Servo    */
    p[ 2] = pdbServoGetPlayerError();   /* Player error in Servo   */
    p[ 3] = pdbServoGetDiscError();     /* Disc info. in Servo     */
    p[ 4] = 0x00;//pdbGetErrorGroupOPC();      /* Player OPC error        */
    p[ 5] = pdbServoGetSelftestError(); /* Servo selftest in Servo */
    p[ 6] = pdbGetErrorGroupTray();     /* Servo Tray error        */
    p[ 7] = pdbGetErrorGroupDiscInfo(); /* Player Disc Info.       */
    p[ 8] = pdbGetErrorGroupRead();     /* Player Read error       */
    p[ 9] = 0x00;//pdbGetErrorGroupWrite();    /* Write error             */
    p[10] = 0x00;                       /* Reserved                */
    p[11] = 0x00;                       /* Reserved                */

    /* locate the error BYTE */
    error_num = 0;
    for(i = 1;i < 13;i++){
        if(p[i]){
            p[0] = 0x01; /* Command Execution results = Error occured */
            error_num = i;
            break;
        }
    }
    /* selftest has highest priority */
    if(p[5]){
        error_num = 5;
    }
    /* no disc has highest priority */
    if(p[7] == eErrorNoDiscPresent){
        error_num = 7;
    }
    /* write error has highest priority */
    if (p[9] == eErrorDataWrite)
    {
        error_num = 9;
    }

    /* After Error */
    send_msg54(SEND_HIF_CMD,'E','R','N',error_num);
    switch(error_num) {
        case  0: break;                   /* Command Execution results = No error */
        case  1: return(AfterError_01()); /* Servo error in Servo    */
        case  2: return(AfterError_02()); /* Player error in Servo   */
        case  3: return(AfterError_03()); /* Disc info. in Servo     */
        case  4: return(AfterError_04()); /* Player OPC error        */
        case  5: return(AfterError_05()); /* Servo selftest in Servo */
        case  6: return(AfterError_06()); /* Servo Tray error        */
        case  7: return(AfterError_07()); /* Player Disc Info.       */
        case  8: return(AfterError_08()); /* Player Read error       */
        case  9: return(AfterError_09()); /* Player Read error       */
        case 10: break;                   /* Reserved                */
        case 11: break;                   /* Reserved                */
        case 12: return(AfterError_12());
    };

    return(plrCmdStop);
}

/*------------------------------------------------------------------------
//  AfterError_01 : servo error
-------------------------------------------------------------------------*/
private BYTE AfterError_01(void)
{
    BYTE cmd = plrCmdStop; /* Next Command */

    if(HIF.PlayerParameters[1] == eErrorSledge){
        cmd = plrCmdIdle;
        plrStrtstpSetIdle();
        pdbSetPlayerStatus(eStTrayIn);
    }
    if(HIF.PlayerParameters[1] == eErrorFocus){
        cmd = plrCmdIdle;
    }    
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_02 : player error in servo
-------------------------------------------------------------------------*/
private BYTE AfterError_02(void)
{
    BYTE cmd = plrCmdStop; /* Next Command */
    TimeS   tmpTime;
    LONG    tmpLBA;
    AddrParmS tmpAdr;

    if(HIF.PlayerParameters[2] == eErrorAccess){
        if(fPrePlayerCmd != plrCmdStartUp){
            cmd = plrCmdPauseOn;
            pdbSetServoReadyStatus(TRUE);
        }
    }
    else{
        tmpTime = pdbGetStopTime(pdbGetHighestRecordedTrkNumber());
        tmpLBA  = MSF2LBA_Plus(&tmpTime);
        tmpAdr.Addr[0] = (char)((char *)&tmpLBA)[3];
        tmpAdr.Addr[1] = (char)((char *)&tmpLBA)[2];
        tmpAdr.Addr[2] = (char)((char *)&tmpLBA)[1];
        tmpAdr.Addr[3] = (char)((char *)&tmpLBA)[0];
        pdbSetSearchAddress(tmpAdr);
    }
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_03 : Disc info error in servo
-------------------------------------------------------------------------*/
private BYTE AfterError_03(void)
{
    BYTE cmd = plrCmdIdle; /* Next Command */

    if ((fPrePlayerCmd == plrCmdStartUp) ||
        ((HIF.PlayerParameters[3] != eErrorSubcodeTimeOut) &&
         (HIF.PlayerParameters[3] != eErrorATIPTimeOut))) {
        cmd = plrCmdStop;
    }
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_04 : OPC info error
-------------------------------------------------------------------------*/
private BYTE AfterError_04(void)
{
    TimeS   tmpTime;
    LONG    tmpLBA;
    AddrParmS tmpAdr;

    tmpTime = pdbGetStopTime(pdbGetHighestRecordedTrkNumber());
    tmpLBA  = MSF2LBA_Plus(&tmpTime);
    tmpAdr.Addr[0] = (char)((char *)&tmpLBA)[3];
    tmpAdr.Addr[1] = (char)((char *)&tmpLBA)[2];
    tmpAdr.Addr[2] = (char)((char *)&tmpLBA)[1];
    tmpAdr.Addr[3] = (char)((char *)&tmpLBA)[0];
    pdbSetSearchAddress(tmpAdr);
    return(plrCmdPauseOn);
    //return(plrCmdStop);
}
/*------------------------------------------------------------------------
//  AfterError_05 : Self-test error
-------------------------------------------------------------------------*/
private BYTE AfterError_05(void)
{
    BYTE cmd = plrCmdIdle; /* Next Command */

    plrStrtstpSetIdle();
    pdbSetPlayerStatus(eStTrayIn);
    /* If no disc, then stop any recovery */
    if(HIF.PlayerParameters[7] == eErrorNoDiscPresent){
        cmd = plrCmdStop;
    }
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_06 : Tray info error
-------------------------------------------------------------------------*/
private BYTE AfterError_06(void)
{
    if(HIF.PlayerParameters[6] == eErrorTrayOut){
        pdbSetPlayerStatus(eStTrayIn);
    }
    else{
        pdbSetPlayerStatus(eStTrayOut);
    }
    plrStrtstpSetIdle();
    return(plrCmdIdle);
}
/*------------------------------------------------------------------------
//  AfterError_07 : Disc info error
-------------------------------------------------------------------------*/
private BYTE AfterError_07(void)
{
    BYTE cmd = plrCmdStop; /* Next Command */

    if((HIF.PlayerParameters[7] & eErrorHFAfterHRTR) == eErrorHFAfterHRTR){
        cmd = plrCmdPauseOn;
        pdbSetServoReadyStatus(TRUE);
    }
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_08 : Read info error
-------------------------------------------------------------------------*/
private BYTE AfterError_08(void)
{
    BYTE cmd = plrCmdStop; /* Next Command */

    if ((HIF.PlayerParameters[8] & eErrorModeOfLastTrack)
                                                == eErrorModeOfLastTrack){
        fPlayerCmd   = plrCmdPauseOn;
        pdbSetServoReadyStatus(TRUE);
    }
    return(cmd);
}
/*------------------------------------------------------------------------
//  AfterError_09 : Write info error
-------------------------------------------------------------------------*/
private BYTE AfterError_09(void)
{
    return(plrCmdPauseOn);
}
/*------------------------------------------------------------------------
//  AfterError_12 : Packet info error
-------------------------------------------------------------------------*/
private BYTE AfterError_12(void)
{
    return(plrCmdPauseOn);
}

/************************************************************************
// Name       : MonitorServo
// Description: Monitor the servo subsystem.
//              When an error occurs,
//              the error will be observed by this routine.
//
//              See also function  MonitorServo_01()
//
*************************************************************************/
private BYTE MonitorServo(void)
{
    //*********************************
    // A Servo Error has occured.
    //*********************************

    tWatchSC ServoCmd;  /* Servo Command   */

    /* Watch Request */
    ServoCmd.Cmd = svoCmdGetErrorInformation;    // Prepare to send a command to Servo to get error information.
    ServoCmd.Len = 0;
    plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);   // Set up Command and parameters in SIF[]
    return(Busy);
}

/*------------------------------------------------------------------------
    Phase01(From Cmd) : Monitor Wait For ErrInfo

    A Servo Error had occured,
      now we retrieve Servo supplied error information bytes from SIF[]

-------------------------------------------------------------------------*/
private BYTE MonitorServo_01(void)
{

    tWatchSC ServoCmd;  /* Servo Command   */

    pdbServoSetServoError((ErrorGroupServoT)(SIF.ServoParameters[0]));       // Get Servo Error return bytes.
    pdbServoSetPlayerError((ErrorGroupPlayerT)(SIF.ServoParameters[1]));     // Get Servo Error return bytes.
    pdbServoSetDiscError((ErrorGroupDiscT)(SIF.ServoParameters[2]));         // Get Servo Error return bytes.
    BufInfo.ErrorInfo.CommandError = SIF.ServoParameters[3];                 // Get Servo Error return bytes.
    BufInfo.ErrorInfo.TrayError    = SIF.ServoParameters[4];                 // Get Servo Error return bytes.

    switch(pdbGetPlayerStatus()){
        case eStStartingUp:
            /* Watch Request */
            ServoCmd.Cmd = svoCmdGetStatusInformation;
            ServoCmd.Len = 0;
            plrWatchReq(ePhaseInc,eWatch_ServoStatus,(BYTE *) &ServoCmd);
            return(Busy);
        default:
            MonitorServo_End();
            return(Ready);
    }
}
/*------------------------------------------------------------------------
//  Phase02(From Ph01) : Mon Check State
-------------------------------------------------------------------------*/
private BYTE MonitorServo_02(void)
{
    plrGetServoStatusInfo();
    if(pdbServoGetDiscDiameter() == eNoDisc){
        pdbSetErrorGroupDiscInfo(eErrorNoDiscPresent);
    }
    MonitorServo_End();
    return(Ready);
}

/*------------------------------------------------------------------------
//  ServoEnd
-------------------------------------------------------------------------*/
private void MonitorServo_End(void)
{
    BYTE cmd;

    cmd = ActAfterError();
    pdbSetClearErrors();
    pdbServoClearErrors();
    fPlayerStatus = Error;

    if(cmd != plrCmdIdle){
        PlayerReq(cmd + eMsgID_Player * 0x100,eMsgID_Non,0,0);
    }
    else{
        PlayerReq(plrCmdErrorReply + eMsgID_Player * 0x100,eMsgID_Non,0,0);
    }
}

