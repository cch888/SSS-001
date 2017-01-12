/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Rights Reserved
* FILENAME      : plrWatch.c
* DESCRIPTION   : The Watch subsystem will Watch Status.
* NOTES         :
*
* $Revision: 80 $
* $Date: 11/03/16 6:00p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\string.h"
#include ".\common\comMath.h"
#include ".\common\error.h"
#include ".\common\system.h"
#include ".\servo\svo.h"
#include ".\player\plrDB.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"

#include ".\AL\REG_dvd_encoder.h"
#include ".\AL\REG_write_strategy.h"

/*************************************************************************/
/*                              Define                                   */
/*************************************************************************/

/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
private void WatchResSnd(void);
private BYTE Watch_ServoStatus(void);      /* Servo Status       */
private BYTE Watch_Seek(void);             /* Seek               */
private BYTE Watch_ISRC(void);             /* ISRC               */
private BYTE Watch_ServoReadyStatus(void); /* Servo Ready Status */
private BYTE Watch_SubCodeOk(void);        /* Sub Code Ok        */
private BYTE Watch_CycleTask(void);        /* Cycle Task         */

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/
private tMsg_Res fMsg;              /* Message     */
private ULONG    fTimer;            /* Timer       */
private BYTE     fWatch;            /* Watch       */
private BYTE     fMsgId;            /* Message ID  */
private BYTE     fSeekMode;         /* Seek Mode   */
private BYTE     fCount;            /* NoEFM Count */
private BYTE     (*fCycTask)(void); /* Cycle Task  */
#ifdef CHECK_ENCODE_TIME_NOT_CONTINUE
vBYTE Shift_Flag;/* for write one block shift protection code */
#endif
extern  ULONG   watchQ_Time;
//vBYTE Iram_WGateUp;

public const tCmd fWatchTask[]= {
    {(LPCHAR) "Idle",0                     ,0}, /* Idle                */
    {(LPCHAR) "SvSt",Watch_ServoStatus     ,0}, /* Servo Status        */
    {(LPCHAR) "Seek",Watch_Seek            ,0}, /* Seek                */
    {(LPCHAR) "ISRC",Watch_ISRC            ,0}, /* ISRC                */
    {(LPCHAR) "SvRS",Watch_ServoReadyStatus,0}, /* Servo Ready Status  */
    {(LPCHAR) "SbOk",Watch_SubCodeOk       ,0}, /* Sub Code Ok         */
    {(LPCHAR) "CTsk",Watch_CycleTask       ,0}, /* Cycle Task          */
};

/************************************************************************
// Name       : plrWatchRequest
// Description: This function will Watch Task Request.
*************************************************************************/
void plrWatchRequest(BYTE Request,BYTE MsgId,BYTE *Data)
{
    BYTE i;

#if SAVE_LOG
    tMsg Msg;
    BYTE len;
#endif /* SAVE_LOG */

    /* Watch */
    fWatch = Request;

    /* Request Idel? */
    if(fWatch == eWatch_Idle){
        /* Task Clear */
        plrTaskClr(eMsgID_Watch);
        return;
    }

    /* Task Command Request */
    plrTaskCmdReq(eMsgID_Watch,fWatch);
    /* Messag Id */
    fMsgId = MsgId;
    /* Data */

#if SAVE_LOG
    len = 0;
#endif /* SAVE_LOG */

    switch(fWatch) {
        case eWatch_ServoReadyStatus : /* Servo Ready Status */
        case eWatch_ServoStatus      : /* Servo Status       */
            /* Servo Command? */
            if(Data)
            {
/* ReAdjusting Wait */
                StartTimer(&fTimer);
                for(;;)
                {
                    if ((svoIramVar.knlTimeupCountFlag == FALSE) || (ReadTimer(&fTimer) > 15000))
                    {
                        if (svoIramVar.knlTimeupCountFlag == TRUE)
                        {
                            svoIramVar.knlTimeupCountFlag = FALSE;
                            send_msg1('T','O','U','T');
                        }
                        break;
                    }
                    ExitProcess();
                }
                SIF.ServoCommand = ((tWatchSC *) Data)->Cmd;                         // Here copy desired Servo Command to SIF[]
                for(i = 0;i < sizeof(SIF.ServoParameters);i++){
                    if(i < ((tWatchSC *) Data)->Len){
                        SIF.ServoParameters[i] = ((tWatchSC *) Data)->Para[i];       // Here copy Servo Parameters to SIF[]
                    }
                    else{
                        SIF.ServoParameters[i] = 0;
                    }
                }
                plrTaskCmdReq(eMsgID_Watch,fWatch); //fixed Watch req clear by ExitProcess//
                pdbSetServoState(SIF.ServoState);/* Servo Status        */

#if SAVE_LOG
                len = ((tWatchSC *) Data)->Len;
#endif /* SAVE_LOG */

            }
            break;
        case eWatch_Seek             : /* Seek               */
            fSeekMode = *Data;
#if SAVE_LOG
            len = sizeof(fSeekMode);
#endif /* SAVE_LOG */
            break;
        case eWatch_ISRC:                           /* Get ISRC           */
            pdbSetSubQIntStatus(FALSE);
            svoIramVar.subQEnableFlag = 1;
            break;
        default:
            break;
    }
    StartTimer(&fTimer);
    fCount = 0;
#if SAVE_LOG
    /* Logging */
    plrMemset((LPBYTE)&Msg,0x00,(USHORT) sizeof(Msg));
    Msg.len    = 6;
    Msg.cmdres = fWatch;
    if(len > 4){
        len = 4;
    }
    plrMemcpy(Msg.data,Data,(USHORT) len);
    plrMsgLog(eLogSnd,eMsgID_Watch,(BYTE *) &Msg);
#endif /* SAVE_LOG */
    /* Message Clr */
    memset((void *) &fMsg,0,sizeof(fMsg));
}

/************************************************************************
// Name       : plrCycleRequest
// Description: This function will Cycle Task Request.
*************************************************************************/
void plrCycleRequest(BYTE MsgId,BYTE (*Task)(void))
{

#if SAVE_LOG
    BYTE Msg[2];
#endif /* SAVE_LOG */

    /* Watch */
    fWatch = eWatch_CycleTask;
#if SAVE_LOG
    /* Logging */
    Msg[1] = fWatch;
    plrMsgLog(eLogSnd,eMsgID_Watch,Msg);
#endif /* SAVE_LOG */

    /* Task Command Request */
    plrTaskCmdReq(eMsgID_Watch,fWatch);
    /* Messag Id    */
    fMsgId = MsgId;
    /* Cycle Task   */
    fCycTask = Task;
    /* Message Clr */
    memset((void *) &fMsg,0,sizeof(fMsg));
}

/************************************************************************
// Name       : WatchResSnd
// Description: This function will Respons Send.
*************************************************************************/
private void WatchResSnd(void)
{
    BYTE ecod; /* Error Code */

    /* Respons Send */
    if(fMsgId != eMsgID_HIF){
        fMsg.res = fWatch | 0x80;
        ecod = plrSndMsg(fMsgId,(LPBYTE) &fMsg,sizeof(fMsg));
    }
}

/************************************************************************
// Name       : plrHIFWatchReq
// Description: This function will Watch Task Request from HIF.
*************************************************************************/
public BYTE plrHIFWatchReq(BYTE Request,BYTE *Data)
{
    plrWatchRequest(Request,eMsgID_HIF,Data);
    /* HIF Task Wait Flag */

    fMsg.result = SIF.ServoExecutionState = Busy;
    while(fMsg.result == Busy){
        ExitProcess();
    }
    return(fMsg.result);
}

/************************************************************************
// Name       : Watch_ServoStatus
// Description: This function will Servo Status Watch.
*************************************************************************/
private BYTE Watch_ServoStatus(void)
{
    if(SIF.ServoCommand != svoCmdIdle){
        return(Busy);
    }
#if (EN_LS == 1)
    if (((pdbGetDiscKind() == eLSDisc) && (ReadTimer(&fTimer) > 50000L))||((pdbGetDiscKind() != eLSDisc) && (ReadTimer(&fTimer) > 10000L)))
#else
    if(ReadTimer(&fTimer) > 10000L)
#endif // End of (EN_LS == 1)
    {
        fMsg.result  = Error;
        send_msg80(SEND_WATCH_MSG,0xD61200); // time out
        /* Respons Send */
        WatchResSnd();
        return(fMsg.result);
    }

    /* Message Send Only Ready or Error*/
    fMsg.result = SIF.ServoExecutionState;

    switch(fMsg.result) {
        case Ready :
        case Error :
            /* Respons Send */
            WatchResSnd();          // Kazuko
            break;
    }
    return(fMsg.result);
}

/************************************************************************
// Name       : Watch_Seek
// Description: This function will Seek Watch.
*************************************************************************/
private BYTE Watch_Seek(void)
{
    /* Time Check, prevent the drive wait SUBQ too long (Now wait 10 sec) */
    if(ReadTimer(&fTimer) >= 10000L){
        fMsg.result = Error;
        /* Respons Send */
        WatchResSnd();
        send_msg5S(1,"WSeek_Tout");
        return(fMsg.result);
    }

    /* Check INT ? */
    if(pdbGetSubQIntStatus() == FALSE){
        return(Busy);
    }
    fMsg.result = pdbGetSubcodeOK();

    /* Respons Send */
    fMsg.result = (fMsg.result == TRUE) ? Ready:Error;
    WatchResSnd();
    return(fMsg.result);
}


/************************************************************************
// Name       : Watch_ISRC
// Description: This function will ISRC Watch.
*************************************************************************/
extern BYTE PlayerSearchCode;

private BYTE Watch_ISRC(void)
{
    BYTE    isrc_catalog[10];
    BYTE    ValidData;

    ValidData = 0;

    if(pdbGetSubQIntStatus() == TRUE){
        pdbSetSubQIntStatus(FALSE);
        if(pdbGetSubcodeOK() == TRUE){
            switch(PlayerSearchCode){
                case 0x02:      /* format 2, UPC */
                    if(pdbGetCatalogCode(&isrc_catalog[0]) == TRUE ){
                        ValidData = 1;                      /* Get data */
                    }
                    break;
                case 0x03:      /* format 3, ISRC */
                    if(pdbGetISRCCode(&isrc_catalog[0]) == HEX2BCD(pdbGetCommandTrack())){
                        ValidData = 1;  /* Get data */
                    }
                    break;
                case 0xF0:      /* format F0, RID   */
                    if(pdbGetRIDCode(&isrc_catalog[0]) == HEX2BCD(pdbGetCommandTrack())){
                        ValidData = 1;  /* Get data */
                    }
                    break;
                default:
                    break;
            }
            if(ReadTimer(&fTimer) >= 1000L){
                ValidData = 2;  /* 1 sec wait : Nothing data */
            }
        }
    }else{
        if(ReadTimer(&fTimer) >= 1000L){
            ValidData = 3;      /* 1 sec wait : Error */
        }
    }
    if(ValidData != 0){
        svoIramVar.subQEnableFlag = 0;

//        if((ValidData == 1) || (ValidData == 2)){
            fMsg.result = Ready;
//        }
//        else{
//            fMsg.result = Error;
//        }

        /* Respons Send */
        WatchResSnd();
        return(fMsg.result);
    }
    return(Busy);
}

/************************************************************************
// Name       : Watch_ServoReadyStatus
// Description: This function will Watch Servo Ready Status.
*************************************************************************/
private BYTE Watch_ServoReadyStatus(void)
{
    if(SIF.ServoCommand != svoCmdIdle){
        return(Busy);
    }
    if(pdbGetServoReadyStatus() == FALSE){
        /* 20sec Over */
        if(ReadTimer(&fTimer) >= 40000L){
            fMsg.result = Error;
            /* Respons Send */
            WatchResSnd();
            return(fMsg.result);
        }
    }
    /* Message Send Only Ready or Error*/
    fMsg.result = plrServoExecuteState();
    switch(fMsg.result){
        case Ready :
        case Error :
            /* Respons Send */
            fWatch = eWatch_ServoStatus | 0x80;
            WatchResSnd();
            break;
    }
    return(fMsg.result);
}


/************************************************************************
// Name       : Watch_SubCodeOk
// Description: This function will Sub Code Ok.
*************************************************************************/
private BYTE Watch_SubCodeOk(void)
{
    /* ATIP Disc? And get current subcode status from plrdb.c */
    if((pdbServoGetDiscType() != (eDiscType)eATIPDisc) && (pdbGetSubcodeOK() != TRUE)){
        if(ReadTimer(&watchQ_Time) < 50)//50 ms
            return(Busy);
    }
    /* Respons Send */
    fMsg.result = Ready;
    WatchResSnd();
    return(fMsg.result);
}

/************************************************************************
// Name       : Watch_CycleTask
// Description: This function will Cycle Task.
*************************************************************************/
private BYTE Watch_CycleTask(void)
{
    /* Message Send Only Ready or Error*/
    fMsg.result = fCycTask();
    switch(fMsg.result) {
        case Ready :
        case Error :
            /* Respons Send */
            WatchResSnd();
            break;
    }
    return(fMsg.result);
}
