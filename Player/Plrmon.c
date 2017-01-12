/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrmon.c
* DESCRIPTION   : This is the Player Moniter
* NOTES         :
*
*
* $Revision: 24 $
* $Date: 11/03/15 1:26p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\error.h"
#include ".\hif\mmc.h"
#include ".\servo\svo.h"
#include ".\player\plrDB.h"
#include ".\player\plrStStp.h"
#include ".\player\plrPlay.h"
#include ".\player\plrTray.h"
#include ".\player\plrInfo.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
/* Task Function */
typedef struct {
    BYTE  *taskMark;     /* Mark      */
    USHORT  taskId;      /* Task ID   */
    BYTE  taskType;      /* Task Type */
    BOOL  sameCmd;       /* Same Cmd  */
    BOOL  logging;       /* Logging   */
    const tCmd *taskCmd; /* Command   */
} tTaskFunc;

#if (SERVICE_MODE != 1)
/* Task Infomation */
typedef struct {
    BYTE cmdReq;      /* 04:Command Request */
    BYTE cmd;         /* 05:Command         */
    BYTE phase;       /* 06:Phase           */
    BYTE state;       /* 07:State           */
    BYTE msgId;       /* 08:Message ID      */
    tMsgBox msgBox;   /* 09:Msg Box         */
} tTaskInfo;
#endif

/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
private void plrNonTask(void);
private void plrMsgTask(void);
private void plrReqTask(void);

/*************************************************************************/
/*                         External Functions                            */
/*************************************************************************/
extern void plrPlayerMain(void);

/*************************************************************************/
/*                         Global  Variables                             */
/*************************************************************************/
/* Task Type */
void (* const fTaskType[])(void) = {
    plrNonTask,
    plrMsgTask,
    plrReqTask
};

/* Task Command */
extern const tCmd fSStpTask[];   /* Strat Stop    */
extern const tCmd fPlayTask[];   /* Play          */
extern const tCmd fTrayTask[];   /* Tray          */
extern const tCmd fInfoTask[];   /* Infomation    */
extern const tCmd fMiscTask[];   /* Misc          */
extern const tCmd fWatchTask[];  /* Watch         */
extern const tCmd fPlayerTask[]; /* Player        */
extern const tCmd fSvmnTask[];   /* Servo Monitor */
//extern const tCmd fRdTocTask[];  /* Read TOC      */

/* Task Function */
private const tTaskFunc fTaskFunc[] = {
   /* Mark  Task ID             TaskType Same  Log  Command    */
    {(BYTE *) "sstp",mTid(eMsgID_SStp)  ,eMsgTask,FALSE,TRUE ,fSStpTask  },
    {(BYTE *) "play",mTid(eMsgID_Play)  ,eMsgTask,FALSE,TRUE ,fPlayTask  },
    {(BYTE *) "tray",mTid(eMsgID_Tray)  ,eMsgTask,FALSE,TRUE ,fTrayTask  },
    {(BYTE *) "info",mTid(eMsgID_Info)  ,eMsgTask,TRUE ,TRUE ,fInfoTask  },
    {(BYTE *) "misc",mTid(eMsgID_Misc)  ,eMsgTask,TRUE ,TRUE ,fMiscTask  },
    {(BYTE *) "watc",mTid(eMsgID_Watch) ,eReqTask,0    ,TRUE ,fWatchTask },
    {(BYTE *) "+pyr",mTid(eMsgID_Player),eMsgTask,TRUE ,TRUE ,fPlayerTask},
    {(BYTE *) "vmon",mTid(eMsgID_Svmn)  ,eMsgTask,TRUE ,TRUE ,fSvmnTask  },
    {(BYTE *) "*non",mTid(eMsgID_Non)   ,eNonTask,0    ,FALSE,0          },
};

/* Task Num */
#define eTaskNum (sizeof(fTaskFunc) / sizeof(tTaskFunc))

#if SAVE_LOG
/* Log Msg */
    #define eLogMax 20  /* Log MaxNum  */
    #define eLogLen 16  /* Log length  */
    #define eGetResLog  /* Get Respons Logging */
#endif /* SAVE_LOG */

#if (SERVICE_MODE==1)
public tTaskInfo fTaskInfo[eTaskNum];    /* Task Infomation */
#else
private tTaskInfo fTaskInfo[eTaskNum];    /* Task Infomation */
#endif
public  tMsg      fPlrMsg;                /* Messgae         */

extern USHORT       Iram_fTaskReq;        /* Task Request    */
private USHORT      fMsgTaskId;           /* MegTaskId       */
private BYTE      fTaskMid;               /* Task Counter    */

#if SAVE_LOG
private BOOL      fLogFlg;                /* Log Flag        */
private BYTE      fLogPnt;                /* Log Point       */
private BYTE      fLog[eLogMax][eLogLen]; /* Log             */
#endif /* SAVE_LOG */

#if (DeadLoop_Solution == 1)
extern SBYTE svo_ERR_STARTUP;
#endif

/************************************************************************
// Name       : plrMemcpy/plrMemset
// Description: This function will Long Size Copy/Set.
*************************************************************************/
public void plrMemcpy(LPBYTE Dst,LPBYTE Src,USHORT Len)
{
    if(Len == 0) return;
    while(Len--) {
        *Dst = *Src;
        Dst++;
        Src++;
    }
}

public void plrMemset(LPBYTE Dst,BYTE data,USHORT Len)
{
    if(Len == 0) return;
    while(Len--) {
        *Dst = data;
        Dst++;
    }
}

/************************************************************************
// Name       : plrMonInit
// Description: This function will initialize Player Monitor.
*************************************************************************/
public void plrMonInit(void)
{
    BYTE i; /* Count */

    /* Variables Initialize */
    Iram_fTaskReq = fTaskMid = fMsgTaskId = 0;

    /* Task Initialize */
    for(i = 0;i < eTaskNum;i++){
        plrTaskInit(i);
        if(fTaskFunc[i].taskType == eMsgTask){
            fMsgTaskId |= fTaskFunc[i].taskId;
        }
    }
    /* Task Request */
    plrTaskReq(eMsgID_Non);

#if SAVE_LOG
    /* Log Message Clear */
    fLogPnt = 0;
    plrMemset(&fLog[0][0],0x00,(USHORT) sizeof(fLog));
    plrLogStart();/* logging start */
#endif /* SAVE_LOG */
}

/************************************************************************
// Name       : plrTaskInit
// Description: This function will initialize Player Tasker.
*************************************************************************/
public void plrTaskInit(BYTE MsgId)
{
    tTaskInfo *ti  = &fTaskInfo[MsgId]; /* Task Information */

    ti->cmdReq   = eIdle;
    ti->cmd      = eIdle;
    ti->phase    = ePhase00;
    ti->state    = Ready;
    ti->msgId    = eMsgID_Non;
    /* Messgae Box */
    ti->msgBox.Size     = sizeof(ti->msgBox.Data);
    ti->msgBox.WrPtr    = 0;
    ti->msgBox.RdPtr    = 0;
    ti->msgBox.NonSpace = FALSE;
    /* Task Clear */
    plrTaskClr(MsgId);
}

/************************************************************************
// Name       : plrMon
// Description: This function will monitor Player.
*************************************************************************/
public void plrMon(void)
{
    /* Task Request? */
    if(Iram_fTaskReq == 0) return;

    /* Request Task Run */
    for(;;){
        fTaskMid++;
        /* Next Task Counter */
        if(fTaskMid == eTaskNum) fTaskMid = 0;
        if(fTaskFunc[fTaskMid].taskId & Iram_fTaskReq){
            fTaskType[fTaskFunc[fTaskMid].taskType]();
        }
        if(((fMsgTaskId & Iram_fTaskReq) == 0)&&(fTaskMid == 0)){
            break;
        }
    }
}

/************************************************************************
// Name       : plrTaskReq/plrTaskCmdReq
// Description: This function will Task Request.
*************************************************************************/
public void plrTaskReq(BYTE MsgId)
{
    Iram_fTaskReq |= mTid(MsgId);
}

public void plrTaskCmdReq(BYTE MsgId,BYTE cmd)
{
    Iram_fTaskReq |= mTid(MsgId);
    fTaskInfo[MsgId].cmd = cmd;
}

/************************************************************************
// Name       : plrTaskClr
// Description: This function will Task Request Clear.
*************************************************************************/
public void plrTaskClr(BYTE MsgId)
{
    Iram_fTaskReq &= (~ mTid(MsgId));
}

/************************************************************************
// Name       : plrSndMsg
// Description: This function will Send Message.
*************************************************************************/
public BOOL plrSndMsg(BYTE MsgId,BYTE *Msg,BYTE Size)
{
    tMsgBox *mb = &fTaskInfo[MsgId].msgBox;
    BYTE    space;
    BYTE    rest;

    /* Message Length */
    ((tMsg *) Msg)->len = Size;

#if SAVE_LOG
    /* Logging */
    plrMsgLog(eLogSnd,MsgId,Msg);
#endif /* SAVE_LOG */

    if(MsgId > eMsgID_Non){
        return(FALSE);
    }

    /* Message Box Pointer  */
    if(mb->Size == 0){
        return(FALSE);
    }
    /* Non Space ? */
    if(mb->NonSpace == TRUE){
        return(FALSE);
    }
    /* Space Check Message Box */
    if(mb->RdPtr > mb->WrPtr){
        space = mb->RdPtr - mb->WrPtr;
    }
    else{
        space = mb->Size - mb->WrPtr + mb->RdPtr;
    }
    /* Space ? */
    if(space < Size){
        return(FALSE);
    }
    /* Set Mesaage */
    rest = mb->Size - mb->WrPtr;
    if(rest >= Size){
        /* 1 Copy */
        plrMemcpy(&mb->Data[mb->WrPtr],Msg,(USHORT) Size);
        mb->WrPtr += Size;
    }
    else{
        /* 2 Copy */
        plrMemcpy(&mb->Data[mb->WrPtr],Msg,(USHORT) rest);
        plrMemcpy(mb->Data,&Msg[rest],(USHORT)  (Size - rest));
        mb->WrPtr = Size - rest;
    }

    /* Write Pointer Check */
    if(mb->WrPtr == mb->Size){
        mb->WrPtr = 0;
    }
    if(mb->WrPtr == mb->RdPtr){
        mb->NonSpace = TRUE;
    }
    /* Task Request */
    plrTaskReq(MsgId);

    return(TRUE);
}

/************************************************************************
// Name       : plrGetMsg
// Description: This function will Get Message.
*************************************************************************/
public BOOL plrGetMsg(BYTE MsgId,BYTE *Msg,LPBYTE Size)
{
    tMsgBox *mb = &fTaskInfo[MsgId].msgBox;
    BYTE    buf_size;
    BYTE    rest;

    if(MsgId > eMsgID_Non){
        return(FALSE);
    }

    /* Message Box Pointer  */
    if(mb->Size == 0){
        /* Task Request Clr */
        plrTaskClr(MsgId);
        return(FALSE);
    }

    /* Non Space ? */
    if(mb->NonSpace == FALSE && mb->WrPtr == mb->RdPtr){
        /* Task Request Clr */
        plrTaskClr(MsgId);
        return(FALSE);
    }

    /* Get Length */
    buf_size = *Size;
    *Size = mb->Data[mb->RdPtr];
    /* Get Mesaage */
    rest = mb->Size - mb->RdPtr;
    if(rest >= buf_size){
        /* 1 Copy */
        plrMemcpy(Msg,&mb->Data[mb->RdPtr],(USHORT) buf_size);
    }
    else{
        /* 2 Copy */
        plrMemcpy(Msg,&mb->Data[mb->RdPtr],(USHORT) rest);
        plrMemcpy(&Msg[rest],mb->Data,(USHORT)  (buf_size - rest));
    }
    if(rest >= *Size){
        mb->RdPtr += *Size;
    }
    else{
        mb->RdPtr  = *Size - rest;
    }
    /* Read Pointer Check */
    if(mb->RdPtr == mb->Size){
        mb->RdPtr = 0;
    }
    mb->NonSpace = FALSE;

    /* Task Request Clr */
    if(mb->WrPtr == mb->RdPtr){
        plrTaskClr(MsgId);
    }

#if SAVE_LOG
    /* Logging */
    plrMsgLog(eLogGet,MsgId,Msg);
#endif /* SAVE_LOG */

    return(TRUE);
}

#if SAVE_LOG
/************************************************************************
// Name       : plrLogStart
// Description: This function will Logging Start.
*************************************************************************/
public void plrLogStart(void)
{
    fLogFlg = TRUE;
}

/************************************************************************
// Name       : plrLogStop
// Description: This function will Logging Stop.
*************************************************************************/
public void plrLogStop(void)
{
    fLogFlg = FALSE;
}

/************************************************************************
// Name       : plrMsgLog
// Description: This function will Message Log.
*************************************************************************/
private void plrMsgLog(BYTE Type,BYTE MsgId,BYTE *Msg)
{
    BYTE *p = &fLog[fLogPnt][0];
    BYTE mtsk,stsk;
    BYTE i;

    if(fLogFlg == FALSE){
        return;
    }

    if(fTaskFunc[fTaskMid].logging == FALSE){
        return;
    }
    /* Type */
    switch(Type) {
        case eLogSnd:
            if((Msg[1] & 0x80) == 0){
                p[0] = 'C';
            }
            else{
                p[0] = 'R';
            }
            break;
        case eLogGet:
#ifndef eGetCmdLog  /* Get Command Logging */
            if((Msg[1] & 0x80) == 0){
                return;
            }
#endif /* eGetCmdLog */
#ifndef eGetResLog  /* Get Respons Logging */
            if((Msg[1] & 0x80) != 0){
                return;
            }
#endif /* eGetResLog */
            p[0] = 'G';
            break;
    }
    if(p[0] != 'R'){
        mtsk = fTaskMid;
        stsk = MsgId;
    }
    else{
        mtsk = MsgId;
        stsk = fTaskMid;
    }
    /* MyTsk */
    p[1] = fTaskFunc[mtsk].taskMark[0];
    /* Phase */
    p[2] = fTaskInfo[mtsk].phase / 10 + '0';
    p[3] = fTaskInfo[mtsk].phase % 10 + '0';
    switch(p[0]){
        case 'C':   p[4] = '>'; break;
        case 'R':   p[4] = '<'; break;
        default :   p[4] = ' '; break;
    }
    /* STsk */
    if(MsgId < eMsgID_Non){
        if(Type == eLogSnd){
            p[5] = fTaskFunc[stsk].taskMark[0];
        }
        else{
            p[5] = '=';
        }
        /* Command Name */
        if(p[0] != 'G' || (Msg[1] & 0x80) == 0){
            for(i = 0;i < 4;i++) {
                p[6+i] = fTaskFunc[stsk].taskCmd[Msg[1] & 0x7f].logmsg[i];
            }
        }
        else{
            if(Msg[2] == Ready){
                plrMemcpy(&p[5],(LPBYTE) "Ready",5);
            }
            else{
                plrMemcpy(&p[5],(LPBYTE) "Error",5);
            }
        }
    }
    else{
        /* Command Name */
        plrMemcpy(&p[3],(LPBYTE) "N----",4);
    }
    /* Message */
    for(i = 0;i < 6;i++){
        if(i < Msg[0]){
            p[10+i] = Msg[i];
        }
        else{
            p[10+i] = 0x00;
        }
    }
    /* End Mark */
    if(++fLogPnt == eLogMax){
        fLogPnt= 0;
    }
    plrMemset(&fLog[fLogPnt][0],0xff,eLogLen);
}

/************************************************************************
// Name       : plrWriteLog
// Description: This function will Log Write.
*************************************************************************/
public void plrWriteLog(LPCHAR logmsg,BYTE len)
{
    if(fLogFlg == FALSE){
        return;
    }

    plrMemset(&fLog[fLogPnt][0],0x00,eLogLen);
    if(len > eLogLen){
        len = eLogLen;
    }
    plrMemcpy(&fLog[fLogPnt][0],(LPBYTE) logmsg,(USHORT) len);
    if(++fLogPnt == eLogMax){
        fLogPnt= 0;
    }
    plrMemset(&fLog[fLogPnt][0],0xff,eLogLen);
}
#endif /* SAVE_LOG */

/************************************************************************
// Name       : plrNonTask
// Description: This function will Non Task.
*************************************************************************/
private void plrNonTask(void)
{
    plrPlayerMain();
    pdbSetServoState(SIF.ServoState);
}

/************************************************************************
// Name        : void plrMsgTask
// Description : This is a continuous process running under moniter.
*************************************************************************/
private void plrMsgTask(void)
{
    BYTE      ecod;                        /* Error Code      */
    BYTE      (*func)(void);               /* Func            */
    tMsg_Res  res;                         /* Response        */
    BYTE      size = sizeof(tMsg);         /* Message Size    */
    tTaskInfo *ti  = &fTaskInfo[fTaskMid]; /* Task Infomation */

#if SAVE_LOG
    BYTE logmsg[12];
#endif /* SAVE_LOG */

    /* Receve Mail */
    size = sizeof(fPlrMsg);
    if(plrGetMsg(fTaskMid,(LPBYTE) &fPlrMsg,&size) == FALSE)
        return;

    /* Command? */
    if(!(fPlrMsg.cmdres & 0x80))
    {
        if((ti->cmd == fPlrMsg.cmdres) && (fTaskFunc[fTaskMid].sameCmd == FALSE) && (ti->state == Ready))
        {
            plrMemset((void *) &res,0,(USHORT) sizeof(res));
            res.res    = ti->cmd | 0x80;
            res.result = ti->state;
            ecod = plrSndMsg(((tMsg_Cmd *) &fPlrMsg)->msgid,(LPBYTE) &res,sizeof(res));
            return;
        }
        /* Command Check */
        ti->state = Ready;

        /* for Parallel Debug port */
        switch(fTaskMid)
        {
            case eMsgID_SStp:
                SendMsg80(SEND_SStpTask_MSG,0xB00000+(((ULONG)fPlrMsg.cmdres)<<8));                
                break;
            case eMsgID_Play:
                SendMsg80(SEND_SStpTask_MSG,0xB10000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
            case eMsgID_Tray:
                SendMsg80(SEND_SStpTask_MSG,0xB30000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
            case eMsgID_Info:
                SendMsg80(SEND_SStpTask_MSG,0xB40000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
            case eMsgID_Misc:
                SendMsg80(SEND_SStpTask_MSG,0xB50000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
            case eMsgID_Player:
                SendMsg80(SEND_SStpTask_MSG,0xB70000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
            case eMsgID_Svmn:
                SendMsg80(SEND_SStpTask_MSG,0xB80000+(((ULONG)fPlrMsg.cmdres)<<8));      
                break;
        }
        /* for Parallel Debug port */

        /*if(fPlrMsg.cmdres >= eTrayEnd) fPlrMsg.cmdres = eIdle;*/
        func = fTaskFunc[fTaskMid].taskCmd[fPlrMsg.cmdres].cmd;
        if(func == 0)
        {
            return;
        }
        /* Parameter Save */
        ti->msgId = ((tMsg_Cmd *) &fPlrMsg)->msgid;
        /* Variables Set*/
        ti->cmd   = fPlrMsg.cmdres;
    }
    else
    {
        /* Resuponse Check */
        ti->state = ((tMsg_Res *) &fPlrMsg)->result;
        if(fTaskFunc[fTaskMid].taskCmd[ti->cmd].task == 0)
        {
#if SAVE_LOG
            plrMemcpy((LPBYTE) &logmsg[0],(LPBYTE) "NULL POINTER",12);
            plrWriteLog((LPCHAR ) logmsg,12);
#endif /* SAVE_LOG */
            return;
        }

        /* for Parallel Debug port */
        switch(fTaskMid)
        {
            case eMsgID_SStp:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB00000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE00000+(((ULONG)ti->cmd)<<8)+ti->phase);
                  #if(DeadLoop_Solution==1)
                    if(St_become_ready == FALSE)
                    {
                        svo_ERR_STARTUP = TRUE;
                        svoIramVar.knlTimeupCountFlag = FALSE;
                        svoIramVar.emergencyStopFlag = TRUE;
                    }
                  #endif
                }
                break;
            case eMsgID_Play:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB10000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE10000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
            case eMsgID_Tray:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB30000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE30000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
            case eMsgID_Info:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB40000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE40000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
            case eMsgID_Misc:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB50000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE50000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
            case eMsgID_Player:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB70000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE70000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
            case eMsgID_Svmn:
                if(ti->state == Ready)
                {
                    SendMsg80(SEND_SStpTask_MSG,0xB80000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                else
                {
                    SendMsg80(SEND_SStpTask_MSG,0xE80000+(((ULONG)ti->cmd)<<8)+ti->phase);
                }
                break;
        }
        /* for Parallel Debug port */

        if(ti->state == Ready)
        {
            func = fTaskFunc[fTaskMid].taskCmd[ti->cmd].task[ti->phase].ready;
        }
        else
        {
            func = fTaskFunc[fTaskMid].taskCmd[ti->cmd].task[ti->phase].erorr;
        }
    }
    if(func)
    {
        ti->state = func();
    }

    /* Status Check */
    if(ti->state == Busy)
    {
        return;
    }

    /* Response Send? */
    ti->phase = ePhase00;
    if(ti->msgId != eMsgID_Non)
    {
        plrMemset((void *) &res,0,(USHORT) sizeof(res));
        res.res    = ti->cmd | 0x80;
        res.result = ti->state;
        ecod = plrSndMsg(ti->msgId,(LPBYTE) &res,sizeof(res));
        /* Variables Set*/
        ti->msgId = eMsgID_Non;
    }
    /* Command Clear */
    if(ti->state == Error)
    {
        ti->cmd = eIdle;
    }
}

/************************************************************************
// Name        : void plrReqTask
// Description : This is a continuous process running under moniter.
*************************************************************************/
private void plrReqTask(void)
{
    BYTE ret;

    ret = fTaskFunc[fTaskMid].taskCmd[fTaskInfo[fTaskMid].cmd].cmd();
    if(ret != Busy)
    {
        //send_msg81(SEND_WATCH_MSG,0xD60000+(((ULONG)fTaskInfo[fTaskMid].cmd)<<8),ret);
        SendMsg80(SEND_WATCH_MSG,0xD60000+(((ULONG)fTaskInfo[fTaskMid].cmd)<<8));
        plrTaskClr(fTaskMid);
    }
}

/************************************************************************
// Name        : plrWatchReq
// Description : This is a continuous process  Watch Request.
*************************************************************************/
public void plrWatchReq(BYTE NextPhase,BYTE Cmd,BYTE *Data)
{
    /* Watch Request */
    plrWatchRequest(Cmd,fTaskMid,Data);
    /* Set Next Phase */
    plrSetPhase(NextPhase);
}

/************************************************************************
// Name        : plrMiscReq
// Description : This is a continuous process Misc Request.
*************************************************************************/
public void plrMiscReq(BYTE NextPhase,BYTE Cmd,LONG Address)
{
    tMiscMsg msg;  /* Message    */
    BOOL     ecod; /* Error Code */
    AddrParmS     temp;
    BYTE     i,j;

    /* Misc Request */
    plrMemset((void *) &msg,0,(USHORT) sizeof(msg));
    msg.cmd      = Cmd;
    msg.msgid    = fTaskMid;
    if(Address){
        for(i = 0,j = sizeof(AddrParmS) - 1 ;i < sizeof(AddrParmS) ;i++,j--){
            temp.Addr[i] = ((AddrParmS *)&Address)->Addr[j];
        }
        msg.address  = temp;
    }

    ecod = plrSndMsg(eMsgID_Misc,(LPBYTE) &msg,sizeof(tMiscMsg));

    /* Set Next Phase */
    plrSetPhase(NextPhase);
}

/************************************************************************
// Name        : plrInfoReq
// Description : This is a continuous process Info Request.
*************************************************************************/
public void plrInfoReq(BYTE NextPhase,BYTE cmd)
{
    tMsg_Cmd msg;  /* Message    */
    BOOL     ecod; /* Error Code */

    /* Info Request */
    msg.cmd      = cmd;
    msg.msgid    = fTaskMid;
    msg.reserved = 0;
    ecod = plrSndMsg(eMsgID_Info,(LPBYTE) &msg,sizeof(tMsg_Cmd));

    /* Set Next Phase */
    plrSetPhase(NextPhase);
}

/************************************************************************
// Name        : plrMeReq
// Description : This is a continuous process Me Request.
*************************************************************************/
public void plrMeReq(BYTE NextPhase)
{
    tMsg_Res res;  /* Response   */
    BOOL     ecod; /* Error Code */

    /* Me Request */
    plrMemset((void *) &res,0,(USHORT) sizeof(res));
    res.res    = 0x80;
    res.result = Ready;
    ecod = plrSndMsg(fTaskMid,(LPBYTE) &res,sizeof(res));

    /* Set Next Phase */
    plrSetPhase(NextPhase);
}

/************************************************************************
// Name        : plrSetPhase
// Description : This is a continuous process Set Next Phase.
*************************************************************************/
public void plrSetPhase(BYTE NextPhase)
{
    /* Next Phase */
    switch(NextPhase){
        case ePhaseSame :                                        break;
        case ePhaseInc  : fTaskInfo[fTaskMid].phase++;           break;
        default         : fTaskInfo[fTaskMid].phase = NextPhase; break;
    }
}

/************************************************************************
// Name        : plrGetPhase
// Description : This is a continuous process Get Phase.
*************************************************************************/
public BYTE plrGetPhase(void)
{
    return(fTaskInfo[fTaskMid].phase);
}

/************************************************************************
// Name        : plrMsgClr
// Description : This is a continuous process Set Non Response.
*************************************************************************/
public void plrMsgBoxClr(BYTE MsgId)
{
    fTaskInfo[MsgId].msgBox.WrPtr    = 0;
    fTaskInfo[MsgId].msgBox.RdPtr    = 0;
    fTaskInfo[MsgId].msgBox.NonSpace = FALSE;
}
