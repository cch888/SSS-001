/****************************************************************************
*                (c) Copyright 2001 - 2006  SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrServo.c
* DESCRIPTION   : The plrServo sub-system handles the communication
*                 between player module and servo module.
*
* $Revision: 44 $
* $Date: 10/12/21 4:20p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\common\error.h"
#include ".\common\comMath.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDB.h"    /* Player:DB */
#include ".\player\plrWatch.h" /* Player:Watch */

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/
public ServoInterfaceInfoS SIF;

/************************************************************************
// Name        : Servo command
// Description : This function set up the command opcode and parameters
*************************************************************************/
/************************************************************************
// Name       : plrServoSeek
// Description:
*************************************************************************/
public void plrServoSeek(long SeekTimeLBA, eSeekMode SeekMode, BYTE NextPhase)
{
    BYTE i;
    tWatchSC ServoCmd;  /* Servo Command */

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    return;
#endif

    if (SeekMode == eWriteSeek){
        for(;;){
            if(svoIramVar.knlTimeupCountFlag == FALSE){
                break;
            }
            ExitProcess();
        }
    }

    ServoCmd.Cmd = svoCmdSeek;
    ServoCmd.Len = 5;
    ServoCmd.Para[0] = (char)((char *)&SeekTimeLBA)[3];
    ServoCmd.Para[1] = (char)((char *)&SeekTimeLBA)[2];
    ServoCmd.Para[2] = (char)((char *)&SeekTimeLBA)[1];
    ServoCmd.Para[3] = (char)((char *)&SeekTimeLBA)[0];
    ServoCmd.Para[4] = SeekMode;
    for(i = 5;i < 10;i++) ServoCmd.Para[i] = 0;

    {   /* for send seek time */
        if (pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == HD_DISC || pdbDiscLoaded() == BD_DISC)
        {
            SendMsgCn(SEND_ATAPI_CMD,5,0x800003, A4B(SeekTimeLBA), SeekMode);    //Seek(Plr->Svo), TarPSN=%Xh, Mode=%Xh
        }
        else
        {
            TimeS sk_time;

            sk_time = ADR2BCD(SeekTimeLBA+150L);
            SendMsgCn(SEND_ATAPI_CMD,4,0x800004, sk_time.Minute, sk_time.Second, sk_time.Frame, SeekMode);    //Seek(Plr->Svo), TarM:S:F=%X:%X:%X, Mode=%Xh
        }
    }

    plrWatchReq(NextPhase,eWatch_ServoStatus,(BYTE *) &ServoCmd);
}

/************************************************************************
// Name       : plrServoPause
// Description:
*************************************************************************/
public void plrServoPause(BYTE Mode, BYTE NextPhase)
{
tWatchSC ServoCmd;  /* Servo Command */

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    return;
#endif

    ServoCmd.Cmd = svoCmdPause;
    ServoCmd.Len = 1;
    ServoCmd.Para[0] = Mode;

    plrWatchReq(NextPhase,eWatch_ServoStatus,(BYTE *) &ServoCmd);
}

/************************************************************************
// Name       : plrRealServoSeek
// Description:
*************************************************************************/
public BYTE plrRealServoSeek(long SeekTimeLBA, eSeekMode SeekMode)
{
    ULONG myTimer;
    BYTE retry;

    for(retry=0; retry<3; retry++)
    {
        plrServoSeek(SeekTimeLBA, SeekMode, ePhaseSame);
        plrTaskClr(eMsgID_Watch);
        StartTimer(&myTimer);
        while(1)
        {
            ExitProcess();

            if(ReadTimer(&myTimer) > 10000L)
                return(Error);

            if(SIF.ServoCommand != svoCmdIdle)
                continue;

            if(SIF.ServoExecutionState == Ready)
                return(Ready);

            if(SIF.ServoExecutionState == Error)
                break;
        }
    }
    return(Error);
}

/************************************************************************
// Name       : plrRealServoPause
// Description:
*************************************************************************/
public BYTE plrRealServoPause(BYTE Mode)
{
    ULONG myTimer;
    BYTE retry;

    for(retry=0; retry<3; retry++)
    {
        plrServoPause(Mode, ePhaseSame);
        plrTaskClr(eMsgID_Watch);
        StartTimer(&myTimer);
        while(1)
        {
            ExitProcess();

            if(ReadTimer(&myTimer) > 10000L)
                return(Error);

            if(SIF.ServoCommand != svoCmdIdle)
                continue;

            if(SIF.ServoExecutionState == Ready)
                return(Ready);

            if(SIF.ServoExecutionState == Error)
                break;
        }
    }
    return(Error);
}

/************************************************************************
// Name       : plrServoSetSpeed
// Description:
*************************************************************************/
public void plrServoSetSpeed(BYTE Speed, BYTE NextPhase)
{
    tWatchSC ServoCmd;  /* Servo Command */
    BYTE spmd;

#if((DVD_710Run815 == 1) && (DVD_710Run815New == 0))
    if(pdbGetServoReadyStatus() == TRUE)
    {
        if((pdbGetDVDType() == DVD_PLUS_R)||(pdbGetDVDType() == DVD_PLUS_RW)||(pdbGetDVDType() == DVD_PLUS_R_DUAL))
        {
             if(Speed == e2XDCLV)
             {
                 Speed = e3XDCLV;
                 //send_msg5SValue(1, "+RChg24", 1, Speed);
             }
        }
    }
#endif
    spmd = (eSpeedMode)((Speed & 0xC0)>>6);

    ServoCmd.Cmd = svoCmdSetSpeed;

    ServoCmd.Len = 4;
    ServoCmd.Para[0] = GetSpeedValue(Speed);
    ServoCmd.Para[1] = spmd;
    ServoCmd.Para[2] = fEnableROC;          /* TRUE : ROC , FALSE: Not ROC */
    ServoCmd.Para[3] = fRecoverLevel;

    send_msg54(SEND_HIF_CMD,'s','p',ServoCmd.Para[1],ServoCmd.Para[0]);

    plrWatchReq(NextPhase,eWatch_ServoStatus,(BYTE *) &ServoCmd);
}

/************************************************************************
// Name       : plrServoReAdjust
// Description:
*************************************************************************/
public void plrServoReAdjust(discKindT ReAdjKind, BYTE NextPhase)        // Support for Player command Servo Pass Through.
{
    tWatchSC ServoCmd;  /* Servo Command */
    StWordU DK;
    DK.Uword = ReAdjKind.fDiscKind;
    ServoCmd.Cmd = svoCmdReAdjust;
    ServoCmd.Len = 2;
    ServoCmd.Para[0] = DK.byte.l;
    ServoCmd.Para[1] = DK.byte.h;
    plrWatchReq(NextPhase,eWatch_ServoStatus,(BYTE *) &ServoCmd);
}

/************************************************************************
// Name       : plrServoPassThrough
// Description:
*************************************************************************/
public void plrServoPassThrough(BYTE Type, BYTE NextPhase)
{
    BYTE Count;
    tWatchSC ServoCmd;  /* Servo Command */

    ServoCmd.Len = 10;
    // Retrieve Servo Command with parameters from Scratch Buffer
    ServoCmd.Cmd = SCSDRAM[BUFFER_SCRATCH + 0];
#if (SERVICE_MODE==1)
    for (Count = 0; Count < 10; Count++)
#else
    for (Count = 0; Count < 12; Count++)
#endif
        ServoCmd.Para[Count] = SCSDRAM[BUFFER_SCRATCH + 1 + Count];

    plrWatchReq(NextPhase,eWatch_ServoStatus,(BYTE *) &ServoCmd);
}

/************************************************************************
// Name       : plrGetServoStatusInfo
// Description:
*************************************************************************/
public void plrGetServoStatusInfo(void)
{
    discKindT DK;
    eDiscType WobbleType;

    BufInfo.StatusInfo.Valid = TRUE;
    pdbServoSetDiscDiameter((eDiscDiameter)(SIF.ServoParameters[0]));
    DK.fDiscKind = (SIF.ServoParameters[2] << 8) | SIF.ServoParameters[1];
    pdbServoSetfDiscKind(DK.fDiscKind);
    pdbSetDiscKind(DK.fDiscKind);
    switch (DK.Bit.DiscStd)
    {
        case eDiscCD:
            if (DK.Bit.Media == eMediaROM)
            {
                WobbleType = eNonATIPDisc;
            }
            else
            {
                WobbleType = eATIPDisc;
            }
            break;

        case eDiscDVD:
            if ((DK.Bit.Media == eMediaR) || (DK.Bit.Media == eMediaRW))
            {
                if (DK.Bit.WblType == eWblPlus)
                {//plus
                    WobbleType = eADIPDisc;
                }
                else
                {//minus or minus_x_2
                    WobbleType = eLPPDisc;
                }
            }
            else
            {//ROM or RAM
                WobbleType = eNonADIPLPPDisc;
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            WobbleType = eBDDisc;
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            WobbleType = eHDDisc;
            break;
#endif
    }
    pdbServoSetDiscType(WobbleType);
}

/************************************************************************
// Name        : plrServFunc
// Description : This routine is the plrServo module process.
*************************************************************************/
ExecutionStateT plrServoExecuteState(void)
{
    pdbSetServoState(SIF.ServoState);
    return(SIF.ServoExecutionState);
}
