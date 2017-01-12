/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrInfo.c
*
* DESCRIPTION   :
*
*   $Revision: 87 $
*   $Date: 11/03/16 6:00p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\oem\oem_info.h"
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\player\plrDB.h"
#include ".\player\plrInfo.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_servo.h"


#include ".\common\intctl.h"
/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/
/* Extract TOC data states */
enum {
    eWAIT_FOR_A0       ,
    eWAIT_FOR_A1       ,
    eWAIT_FOR_A2       ,
    eWAIT_FOR_TRACKS   ,
    eWAIT_FOR_B0       ,
    eWAIT_FOR_C0       ,
    eWAIT_FOR_C1       ,
    eWAIT_FOR_SECOND_A0,
    eDECODE_TOC_DONE
};

#define eMAXRETRY               20
#define eMAXPMAENTRIES         990
#define eMAXNOSUBCODEERROR       9
#define ePMAREADENTRIESNUMBER   10
#define eMAX_SEEK_RETRY_COUNT    5

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
typedef struct {
    BYTE (*Proc)(SubQCodeS *SubCode); /* SubCode*/
} tTOCDecProc;

/*************************************************************************/
/*                         External Functions                            */
/*************************************************************************/
extern void plrServoSeek(long SeekTimeLBA, eSeekMode SeekMode, BYTE NextPhase);
/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
/* Read TOC */
private BYTE ReadTOC(void);
private BYTE ReadTOC_01(void);          private BYTE ReadTOC_01E(void);
private BYTE ReadTOCDecode(void);
private BYTE ReadTOCDecode_A0(SubQCodeS *SubCode);
private BYTE ReadTOCDecode_A1(SubQCodeS *SubCode);
private BYTE ReadTOCDecode_A2(SubQCodeS *SubCode);
private BYTE ReadTOCDecode_TRACKS(SubQCodeS *SubCode);
private BYTE ReadTOCDecode_SECOND_A0(SubQCodeS *SubCode);
/* Read Raed */

/* Read Disc Info */

/*************************************************************************/
/*                         Public  Variables                            */
/*************************************************************************/
BYTE PMANotOk;          /*bit 7 set --> PMA Read Failed
                        bit 0-6  -->  count for read TOC failed */
/*************************************************************************/
/*                         Private  Variables                            */
/*************************************************************************/

private ULONG fTOCTimer;
private ULONG fTOCENDTimer;
private ULONG fSubQTimer;

public  TimeS fTOC_A2msf[2];

private BYTE  fLastReadTno;
private BYTE  fLastReadCont;    /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */
private BYTE  fGetTOCState;
private BOOL  fB0Found;
private BOOL  f0CFound;
private BOOL  fCDIDiscNoAudio;

public  BYTE          fA2Count;
public  BOOL          fA0valid;
public  BOOL          fB0valid;
public  BOOL          fFirstSess;
public  DiscModeTypeT fA0value;
BYTE  fMode0Count;

private BYTE Session_max_trk=0;
private BYTE Session_min_trk=0;

extern TimeS preB0Time;
/* Read TOC */
private const tTask fReadToc[] = {
/*          Ready      Error      */
/* Ph00 */ {0         ,0          },
/* Ph01 */ {ReadTOC_01,ReadTOC_01E},
};

private const tTOCDecProc fTOCDecProc[] = {
    ReadTOCDecode_A0       , /* WAIT_FOR_A0        */
    ReadTOCDecode_A1       , /* WAIT_FOR_A1        */
    ReadTOCDecode_A2       , /* WAIT_FOR_A2        */
    ReadTOCDecode_TRACKS   , /* WAIT_FOR_TRACKS    */
    0                      , /* WAIT_FOR_B0        */
    0                      , /* WAIT_FOR_C0        */
    0                      , /* WAIT_FOR_C1        */
    ReadTOCDecode_SECOND_A0, /* WAIT_FOR_SECOND_A0 */
    0                      , /* DECODE_TOC_DONE    */
};


/* Command Table */
public const tCmd fInfoTask[] = {
    {(LPCHAR) "Idle",0            ,0             }, /* Idle           */
    {(LPCHAR) "RToc",ReadTOC      ,fReadToc      }, /* Read TOC       */
};

BYTE TempTOCA0, TempTOCA1, TempTOCA2[3];
BYTE TOCtemp[99][4];
/************************************************************************
// Name        : ReadTOC
// Description :
*************************************************************************/
private BYTE ReadTOC(void)
{
    /* Toc Initial */
    /* Get Lowest Track Number equal 0 ) */
    if(pdbGetLowestRecordedTrackNumber() == 0 ){   /* 1st TOC */
        SendMsgCn(SEND_SStpTask_MSG,2,0x800498, 1, 1);
        pdbSetSessionCounter( 1 );
        fLastReadTno = 0;
        fLastReadCont = 0xFF;   /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */
    }
    else {
        /* Set Session Counter to GetSessionCounter +1 */
        if(pdbGetNextSessionAvailable() == TRUE)
        {
            SendMsgCn(SEND_SStpTask_MSG,1,0x800496, 2);
            pdbSetSessionCounter( pdbGetSessionCounter()+1 );
        }
    }

    /* Set Last Session Leadin Start Time */
    /* Set Hybrid Status to NO_HYBRID */
    pdbSetHybridStatus( eNoHybrid );
    fGetTOCState    = eWAIT_FOR_A0;
    fCDIDiscNoAudio = FALSE;
    fB0Found        = FALSE;
    f0CFound        = FALSE;
    PMANotOk &= 0x80;       /* PMA Read Failed protect count initial*/
    StartTimer(&fTOCTimer);
    StartTimer(&fSubQTimer);
    Session_max_trk = Session_min_trk = 0;
    /* Cycle Task */
    plrCycleRequest(eMsgID_Info,ReadTOCDecode);
    /* Next Phase */
    plrSetPhase(ePhase01);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd) - TOC Read
-------------------------------------------------------------------------*/
private BYTE ReadTOC_01(void)
{
    LONG lba;
    BYTE TgtTno;
    TimeS fRefTime;

    pdbSetFinalSessCloseInfo(eSessionFinalized);
    /* Set Last Session First Track Number */
    pdbSetFinalSessNumber(pdbGetSessionCounter());
    if(pdbServoGetDiscType() == eATIPDisc)
    {
        TgtTno  = pdbGetFinalSessFirstTrackNumber();
        fRefTime = pdbGetStartTime(TgtTno);
        /* Set Last Session First Track Start Time */
        pdbSetFinalSessFirstTrackTime(fRefTime);
        if(TgtTno > 1)
        {
            fRefTime = pdbGetStopTime(TgtTno-1);
            if(pdbGetSessionCounter() == 2){
                lba = eFirstLeadOutLength +1;
            }
            else{
                lba = eLeadOutLength      +1;
            }
            lba += MSF2Count(&fRefTime);
            fRefTime = ADR2MSF(lba);
            /* Set Last Session Leadin Start Time */
            pdbSetFinalSessLeadinTime(fRefTime);
        }
    }
    return(Ready);
}

private BYTE ReadTOC_01E(void)
{
    SendMsgCn(SEND_SStpTask_MSG,1,0x800497, 5);
    pdbSetSessionCounter( pdbGetSessionCounter() - 1 );
    pdbSetFinalSessCloseInfo(eSessionFinalized);
    pdbSetNextSessionAvailable(FALSE);
    return(Ready);
}

#if (MERGE_TOC != 0)
/************************************************************************
// Name        : SetContinuousTrackInfo
// Description : This function would save a TOC info with continuous track info
*************************************************************************/
private void SetContinuousTrackInfo(BYTE Tno, BYTE ContTrack, TimeS Time)
{
    pdbSetStartTime(Time,ContTrack);

    if(Tno > 1)     /* Set prev track stop time */
    {
        mathSubtractAddressFromMSF(&Time,1L,&Time);
        pdbSetStopTime(Time,ContTrack-1);
    }

    pdbSetTrackSessionNumber(ContTrack, pdbGetSessionCounter());
    pdbSetTrackContAddr(ContTrack,TOCtemp[Tno-1][3]);
    pdbSetTrackDataMode(ContTrack,0x0F);               /* track data mode */
    pdbSetTrackState(ContTrack,eTRACK_STATE_COMPLETE);

    Time = pdbGetLeadOutStartTime();
    mathSubtractAddressFromMSF(&Time,1L,&Time);
    pdbSetStopTime(Time,ContTrack);     //set current track end to leadout-1
                                        //it would be reset when next track info is ok
    pdbSetHighestRecordedTrkNumber(ContTrack); //reset in case of the track info missed
    pdbSetFinalSessLastTrackNumber(ContTrack);
}


/************************************************************************
// Name        : IntegrateMissTrack
// Description : This function save a TOC info without missing track info (missing tracks would be discarded)
// Return      : TRUE   TOC info is established
*************************************************************************/
BYTE IntegrateMissTrack(void)
{
    TimeS TrkTime;
    BYTE Tno, IntgTrackNum, TrackMiss, i;

    //send_msg5S(SEND_SStpTask_MSG, "IntegMissTrk");
    send_msg80(SEND_SStpTask_MSG,0x710700);

    TrackMiss = 0;

    for(Tno = 1, IntgTrackNum = 1; Tno <= TempTOCA1; Tno++)
    {
        TrkTime = pdbGetStartTime(Tno);

        if(TrkTime.Minute == 0 && TrkTime.Second == 0 && TrkTime.Frame == 0)    //no this track info
        {
            if(Tno == 1)
            {
                mTimeSet(TrkTime,0,2,0);

                //search next valid control/address
                if(TempTOCA1 > 1)
                {
                    for(i = 2; i <= TempTOCA1; i++)
                    {
                        if(pdbGetTrackContAddr(i) != 0)
                        {
                            //send_msg5SValue(SEND_SStpTask_MSG, "SetTrk1 as", 1, pdbGetTrackContAddr(i));
                            TOCtemp[0][3] = pdbGetTrackContAddr(i);
                            break;
                        }
                    }
                }

                if(pdbGetTrackContAddr(1) == 0)
                {
                    //send_msg5S(SEND_SStpTask_MSG, "Trk1 Ctrl = 1");
                    send_msg80(SEND_SStpTask_MSG,0x710701);
                    TOCtemp[0][3] = 0x01;
                }

                SetContinuousTrackInfo(Tno, IntgTrackNum, TrkTime);
                IntgTrackNum++;   //has track info
            }
            else
                TrackMiss++;

            SendMsgCn(SEND_SStpTask_MSG,2,0x80049A, Tno, TrackMiss);
        }
        else
        {
            if(TrackMiss > 0)
            {
                SetContinuousTrackInfo(Tno, IntgTrackNum, TrkTime);
            }

            IntgTrackNum++;   //has track info
        }
    }

    return TRUE;
}

#endif  //(MERGE_TOC != 0)


/************************************************************************
// Name        : ReadTOCDecode
// Description : This function decodes the subcode while reading the TOC
//               * if TOC frame Tno is 0xAA, return READY
//               * TOC frame Conad is MODE 1
//               * TOC frame Conad is MODE 0
//               * monitor A0:
//               * monitor A1:
//               * monitor A2:
//               * monitor TRACKS:
//               * TOC frame point is 0xB0
*************************************************************************/

private BYTE ReadTOCDecode(void)
{
    SubQCodeS SubCode;
    TimeS     Time, OutermostLeadOutStartTime;
    BYTE      (*Proc)(SubQCodeS *SubCode);
    BYTE code[10];
    BYTE    trknumInSession;
    ULONG   readtocTimeout;

    if( (Session_max_trk > 0)&&(Session_min_trk > 0) )
    {
        trknumInSession = pdbGetHighestRecordedTrkNumber()- pdbGetFirstTrackInLastSession();
        if(trknumInSession > 30)
            readtocTimeout = 3000L;
        else
            readtocTimeout = 1200L;
     }
     else
        readtocTimeout = 1200L;

#if (MERGE_TOC == 0)
    //If can't read any subQ code during 500ms, then exit
    //3.0 sec is enough for 99 tracks in a session
    if( (ReadTimer(&fSubQTimer) > 500L)||(ReadTimer(&fTOCTimer) > readtocTimeout) )
    {
        //send_msg5S(SEND_SStpTask_MSG, "NoTocExist");
        send_msg80(SEND_SStpTask_MSG,0x710703);
        return(Error);
    }
#else
    if(ReadTimer(&fTOCTimer) > readtocTimeout)
    {
        if(SetTrackFromTempInfo())
        {
            //send_msg5SValue(SEND_SStpTask_MSG, "InsTrk1", 1, fB0Found);

            if(fB0Found == TRUE && f0CFound == FALSE )          /* will be reset on reading next TOC */
                pdbSetNextSessionAvailable(TRUE);
            else
                pdbSetNextSessionAvailable(FALSE);

            if(pdbServoGetDiscType() == eATIPDisc)
            {
                if(fTOCFound == TRUE)
                {
                    SendMsgCn(SEND_SStpTask_MSG,1,0x800497, 2);
                    pdbSetSessionCounter(pdbGetSessionCounter() - 1);
                }
            }

            return(Ready);
        }
        else
        {
            //send_msg5S(SEND_SStpTask_MSG, "InsTrk XX");
            send_msg80(SEND_SStpTask_MSG,0x710704);
    #if (MERGE_TOC == 2)
            if(fTocRtyCnt >= eMax_TocRtyCnt)
            {
                if(IntegrateMissTrack() == TRUE)
                {
                    return(Ready);
                }
            }
    #endif  //(MERGE_TOC == 2)

            return(Error);
        }
    }
#endif//#if(MERGE_TOC)

    if((PMANotOk & 0x80) == 0x80){
    if((PMANotOk & 0x7f )> 50){
        if(ReadTimer(&fTOCTimer) > 1000L){
            pdbSetFinalSessCloseInfo(eSessionFinalized);
            return(Error);
    }
        else
            return(Busy);
        }
    }

    if(pdbGetSubQIntStatus() == FALSE){
        return(Busy);
    }
    PMANotOk &= 0x80;
    pdbSetSubQIntStatus(FALSE);

    if(pdbGetSubcodeOK() == FALSE){
        return(Busy);
    }
    pdbGetSubCode(&SubCode);
    switch(SubCode.Addr) {
        case 0x00 :
        case 0x02 :
        case 0x03 :
        case 0x04 :
            return(Busy);
        case 0x01 :
        case 0x05 :
            switch(SubCode.Tno) {
                case 0xAA :
                    if (ReadTimer(&fSubQTimer) > 300L)//Add for Multi-session
                    {
                        SendMsgCn(SEND_SStpTask_MSG,1,0x800497, 3);
                        pdbSetSessionCounter( pdbGetSessionCounter() - 1 );
                        fGetTOCState = eDECODE_TOC_DONE;
                        fB0Found     = FALSE;
                        pdbSetHybridStatus( eNoHybrid );
                        pdbSetNextSessionAvailable(FALSE);
                        return(Ready);
                    }
                case 0x00 : /* Check lead-in track */
                    break;
                default:
                    return(Busy);
            }
            break;
        case 0x0C : /* Midbar Cactus Datashield "6". For some special cases.
                         If it is present in lead-in area, discard following sessions.*/
            if( pdbServoGetDiscType() == eNonATIPDisc )
                f0CFound= TRUE;
            return(Busy);
        default:
            return(Busy);
    }

    switch(SubCode.Addr) {
        case eSubcodeMode0 :
        case eSubcodeMode1 :
            StartTimer(&fSubQTimer);
            #if (MERGE_TOC != 0)
            if(SubCode.Tno == 0 && SubCode.Addr == eSubcodeMode1)
            {
                BYTE Trk;
                switch(SubCode.Point)
                {
                    case 0xA0:
                        if(TempTOCA0 == 0)
                        {
                            TempTOCA0 = SubCode.PMinute;
                            SendMsgCn(SEND_SStpTask_MSG,2,0x80049C, 0, TempTOCA0);
                        }
                        break;

                    case 0xA1:
                        if(TempTOCA1 == 0)
                        {
                            TempTOCA1 = SubCode.PMinute;
                            SendMsgCn(SEND_SStpTask_MSG,2,0x80049C, 1, TempTOCA1);
                        }
                        break;

                    case 0xA2:
                        if(TempTOCA2[0] == 0 && TempTOCA2[1] == 0 && TempTOCA2[2] == 0)
                        {
                            TempTOCA2[0] = SubCode.PMinute;
                            TempTOCA2[1] = SubCode.PSecond;
                            TempTOCA2[2] = SubCode.PFrame;
                            SendMsgCn(SEND_SStpTask_MSG,4,0x80049B, 2, TempTOCA2[0], TempTOCA2[1], TempTOCA2[2]);
                        }
                        break;

                    default:
                        Trk = BCD2HEX(SubCode.Point);
                        if(0 < Trk && Trk <= 99)
                        {
                            Trk--;

                            if(TOCtemp[Trk][0] == 0xff && TOCtemp[Trk][1] == 0xff && TOCtemp[Trk][2] == 0xff && TOCtemp[Trk][3] == 0xff)
                            {
                                TOCtemp[Trk][0] = SubCode.PMinute;
                                TOCtemp[Trk][1] = SubCode.PSecond;
                                TOCtemp[Trk][2] = SubCode.PFrame;
                                TOCtemp[Trk][3] = SubCode.Con << 4 | SubCode.Addr;
                            }
                        }
                        break;
                }
            }
            #endif  //(MERGE_TOC != 0)

            Proc = fTOCDecProc[fGetTOCState].Proc;
            if(Proc){
                fGetTOCState = Proc(&SubCode);
            }
            break;
        case eSubcodeMode5 :
            StartTimer(&fSubQTimer);
            if((SubCode.Point == 0xB0) && (fB0Found == FALSE)){

                if(SubCode.Minute != 0xFF){
                    Time.Minute = BCD2HEX(SubCode.Minute);
                    Time.Second = BCD2HEX(SubCode.Second);
                    Time.Frame  = BCD2HEX(SubCode.Frame);

                    /* When the next session start time is the same as previous,
                    we treat the current session as the final one. */

                    if (mathCompareMSF(&Time, &preB0Time) != GREATER){
                        SendMsgCn(SEND_SStpTask_MSG,1,0x800497, 4);
                        pdbSetSessionCounter( pdbGetSessionCounter() - 1 );
                        fGetTOCState = eDECODE_TOC_DONE;
                        fB0Found     = FALSE;
                        pdbSetHybridStatus( eNoHybrid );
                        pdbSetNextSessionAvailable(FALSE);
                        return(Ready);
                    }
                }

                Time.Minute = BCD2HEX(SubCode.PMinute);
                Time.Second = BCD2HEX(SubCode.PSecond);
                Time.Frame  = BCD2HEX(SubCode.PFrame);

                /* Only update the ATIPLeadout Time for NonATIPDisc, for ATIP
                disc this value should not be changed. */
                if (pdbServoGetDiscType() == eNonATIPDisc)
                {
                    pdbSetATIPLeadOut(Time);
                }

                if(SubCode.Minute != 0xFF){
                    Time.Minute = BCD2HEX(SubCode.Minute);
                    Time.Second = BCD2HEX(SubCode.Second);
                    Time.Frame = BCD2HEX(SubCode.Frame);
                    if(CheckBCDTime((TimeS *)&SubCode.Minute) == FALSE){
                        pdbSetHybridStatus(eNoHybrid);
                        break;
                    }
                    pdbSetNextSessStartTime(Time);
                    preB0Time = Time;
                    pdbSetHybridStatus(eHybrid);
                    fB0Found = TRUE;
                }
                else{
                    pdbSetHybridStatus(eNoHybrid);
                }
            }
            if((SubCode.Point == 0xC0) && (pdbGetSubCodeMode5(PointC0,(SubQCodeS *)&code[0]) == FALSE)){
                pdbSetSubCodeMode5((BYTE)PointC0,&SubCode,TRUE);
            }
            if((SubCode.Point == 0xC1) && (pdbGetSubCodeMode5(PointC1,(SubQCodeS *)&code[0]) == FALSE)){
                pdbSetSubCodeMode5((BYTE)PointC1,&SubCode,TRUE);
            }
            break;
    }

    if(fGetTOCState == eDECODE_TOC_DONE){
        /* For some non-ATIP CD with wrong q-code:
                If the start time of the outermost Lead-out isn't greater than the stop time of
                the last track, the start time will be adjusted. The start time of the outermost
                Lead-out will be changed to the stop time of the last track + 1 frame.            */
        if( pdbServoGetDiscType() == eNonATIPDisc )
        {
            OutermostLeadOutStartTime = pdbGetATIPLeadOut();
            Time = pdbGetStopTime( pdbGetHighestRecordedTrkNumber() );

            if( mathCompareMSF( &OutermostLeadOutStartTime,  &Time )  != GREATER)
            {
                mathAddAddressToMSF( &Time, 1, &Time );
                pdbSetATIPLeadOut(Time);
            }
        }

        if(fB0Found == TRUE && f0CFound == FALSE ){                               /* will be reset on reading next TOC */
            pdbSetNextSessionAvailable(TRUE);
        }
        //start == FWtester2  Item13 No.3 CD-R Multi-Session compare error 
        // for Audio CD extra disc , that without 0xB0
        //else if((SubCode.Point == 0xA0)&&(pdbGetDiscModeType(pdbGetSessionCounter())==eCDRomCDDA))
        else if(fB0Found == TRUE && f0CFound == TRUE )
        {
            pdbSetNextSessionAvailable(TRUE);
        }
        //end == FWtester2  Item13 No.3 CD-R Multi-Session compare error  
        else
        {
            pdbSetNextSessionAvailable(FALSE);
        }
#if (SIGMA_BE==1)
    //for SIGMA B/E all CD-ROM has one session only.
    if(svoVar_discKindT.Bit.Media == eMediaROM)
    {
        send_msg5S(SEND_SStpTask_MSG,"SDBE");
        pdbSetNextSessionAvailable(FALSE);
    }
#endif
        return(Ready);
    }
    return(Busy);
}

/*------------------------------------------------------------------------
// WAIT_FOR_A0
-------------------------------------------------------------------------*/
private BYTE ReadTOCDecode_A0(SubQCodeS *SubCode)
{
    BYTE Tno,sno,Cont;

    if(SubCode->Point != 0xA0) return(fGetTOCState);
    Tno = BCD2HEX(SubCode->PMinute);

    if(pdbGetLowestRecordedTrackNumber() == 0) {
        pdbSetLowestRecordedTrackNumber(Tno);
    }

    if( (pdbGetSessionCounter() > 1) && (Tno <= fLastReadTno))
    {
      return(eWAIT_FOR_A0);
    }

    //send_msg5S(SEND_SStpTask_MSG, "Decd A0");
    send_msg80(SEND_SStpTask_MSG,0x710049);
    Session_min_trk = Tno;

    pdbSetFirstTrackInLastSession(Tno);
    /* Set Last Session First Track Number */
    pdbSetFinalSessFirstTrackNumber(Tno);
    fLastReadTno = Tno - 1;
    pdbSetDiscModeType(pdbGetSessionCounter(),SubCode->PSecond);
    pdbSetFinalSessFormat(SubCode->PSecond); /* CD-DA/CD-ROM/,CD-I,CD-ROM XA */
    sno  = pdbGetSessionCounter();
    Cont = (BYTE) (SubCode->Con & 0x0F);
    pdbSetDiscContAx(sno,ePointA0,Cont);     /* Save control/address of A0 */

    return(eWAIT_FOR_A1);
}

/*------------------------------------------------------------------------
// WAIT_FOR_A1
-------------------------------------------------------------------------*/
private BYTE ReadTOCDecode_A1(SubQCodeS *SubCode)
{
    BYTE Tno,sno,Cont;

    /* point A1 ? */
    if(SubCode->Point != 0xA1){
        return(fGetTOCState);
    }
    Tno = BCD2HEX(SubCode->PMinute);

    if((pdbGetSessionCounter() > 1) && (Tno <= fLastReadTno)){
      return(eWAIT_FOR_A1);
    }

    //send_msg5S(SEND_SStpTask_MSG, "Decd A1");
     send_msg80(SEND_SStpTask_MSG,0x710050);
    Session_max_trk = Tno;

    pdbSetHighestRecordedTrkNumber(Tno);
    /* Set Last Session Last Track Number */
    pdbSetFinalSessLastTrackNumber(Tno);

    if((((SubCode->Con & eSuboceControlMASK) == eSubcodeTrackMask) ||
        ((SubCode->Con & eSuboceControlMASK) == eSubcodePacketMask)) &&
         (pdbGetDiscModeType(pdbGetSessionCounter()) == eCDIMode)){
            fCDIDiscNoAudio = TRUE;
    }

    sno  = pdbGetSessionCounter();
    Cont = (BYTE)(SubCode->Con & 0x0F);
    pdbSetDiscContAx(sno,ePointA1,Cont); /* Save control of A1 */
    return(eWAIT_FOR_A2);
}

/*------------------------------------------------------------------------
// WAIT_FOR_A2
-------------------------------------------------------------------------*/
private BYTE ReadTOCDecode_A2(SubQCodeS *SubCode)
{
    TimeS Time;
    BYTE  Tno,sno,Cont,wArg,ConAd;

    if(SubCode->Point != 0xA2){
        return(fGetTOCState);
    }
    //send_msg5S(SEND_SStpTask_MSG, "Decd A2");
     send_msg80(SEND_SStpTask_MSG,0x710051);

    sno = pdbGetSessionCounter();
    Cont = (BYTE)(SubCode->Con & 0x0F);
    pdbSetDiscContAx(sno,ePointA2,Cont);                /* Save control of A2 */

    Time.Minute = BCD2HEX(SubCode->PMinute);
    Time.Second = BCD2HEX(SubCode->PSecond);
    Time.Frame  = BCD2HEX(SubCode->PFrame);

    wArg = sno % 2;             /* For Stop Time of the last Track in Session */
    fTOC_A2msf[wArg] = Time;

    pdbSetLeadOutStartTime(Time);
    pdbSetFinalSessLeadoutTime(Time); /* Set Last Session Leadout Start Time */
    mathSubtractAddressFromMSF(&Time,1L,&Time);
    pdbSetStopTime(Time,pdbGetHighestRecordedTrkNumber());

    if(fCDIDiscNoAudio == TRUE){         /* CDI handling */
        Tno = pdbGetLowestRecordedTrackNumber();
        pdbSetStopTime(Time,Tno);
        Time.Minute = 0;
        Time.Second = 2;
        Time.Frame  = 0;
        pdbSetStartTime(Time,Tno);
        pdbSetTrackSessionNumber(Tno, pdbGetSessionCounter());
        ConAd = (SubCode->Con << 4) | SubCode->Addr;
        pdbSetTrackContAddr(Tno,ConAd);
        pdbSetTrackDataMode(Tno,0x0F);
        pdbSetTrackState(Tno,eTRACK_STATE_COMPLETE);
        StartTimer(&fTOCENDTimer);
        return(eWAIT_FOR_SECOND_A0);
    }
    else{
        return(eWAIT_FOR_TRACKS);
    }
}

/*------------------------------------------------------------------------
// WAIT_FOR_TRACKS
-------------------------------------------------------------------------*/
private BYTE ReadTOCDecode_TRACKS(SubQCodeS *SubCode)
{
    TimeS Time, PreTrkTime;
    BYTE  Tno,wArg,ConAd;
    BYTE  Control;  /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */
    LONG CurTrkLBA, PreTrkLBA;

    Tno = BCD2HEX(SubCode->Point);
    Control = BCD2HEX(SubCode->Con);    /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */

    /* same as previous track? */
    if((pdbGetSessionCounter() > 1) && (Tno <= fLastReadTno)){
        return(fGetTOCState);
    }

    if((Tno - fLastReadTno) != 1){
        return(fGetTOCState); /*next track to previour run ?*/
    }

    /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */
    if(Control != fLastReadCont)
    {
        fLastReadCont = Control;
        return(fGetTOCState);
    }
    /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */

    Time.Minute = BCD2HEX(SubCode->PMinute);
    Time.Second = BCD2HEX(SubCode->PSecond);
    Time.Frame = BCD2HEX(SubCode->PFrame);

   /* For protect track information , if the current start time of the track is smaller than the previous.
        Treat this track info as invalid.*/
    if (Tno > 1 ){
    	PreTrkTime = pdbGetStartTime(Tno - 1);
    	CurTrkLBA = MSF2Count(&Time);
    	PreTrkLBA = MSF2Count(&PreTrkTime);

      if (PreTrkLBA > CurTrkLBA){
       	pdbSetHighestRecordedTrkNumber(Tno-1);
		StartTimer(&fTOCENDTimer);
              return(eWAIT_FOR_SECOND_A0);
       }
    }

    fLastReadTno = Tno;
    fLastReadCont = 0xff;   /* Work Around for Q-code Protection - Interrupt on rising edge of SCOR, Q Regs updated on falling edge */

    if((Time.Minute == 0) && (Time.Second < 2)&& (Tno == 1)){
      Time.Minute = 0;
      Time.Second = 2;
      Time.Frame  = 0;
    }
    pdbSetStartTime(Time,Tno);

    if(Tno > 1){                                 /* Set prev track stop time */
        if(pdbGetTrackSessionNumber(Tno-1) != pdbGetSessionCounter()){
            wArg = pdbGetTrackSessionNumber(Tno-1);  /* Previous session no. */
            wArg = wArg % 2;
            Time.Minute = fTOC_A2msf[wArg].Minute;
            Time.Second = fTOC_A2msf[wArg].Second;
            Time.Frame  = fTOC_A2msf[wArg].Frame;
        }
        mathSubtractAddressFromMSF(&Time,1L,&Time);
        pdbSetStopTime(Time,Tno-1);
    }

    pdbSetTrackSessionNumber(Tno, pdbGetSessionCounter());
    ConAd = (SubCode->Con << 4) | SubCode->Addr;
    pdbSetTrackContAddr(Tno,ConAd);
    pdbSetTrackDataMode(Tno,0x0F);              /* track data mode */
    pdbSetTrackState(Tno,eTRACK_STATE_COMPLETE);
    if(Tno == pdbGetHighestRecordedTrkNumber()){
        StartTimer(&fTOCENDTimer);
        return(eWAIT_FOR_SECOND_A0);
    }
    return(fGetTOCState);
}

/*------------------------------------------------------------------------
// WAIT_FOR_SECOND_A0
-------------------------------------------------------------------------*/
private BYTE ReadTOCDecode_SECOND_A0(SubQCodeS *SubCode)
{
    if(SubCode->Point != 0xA0){                 /* point A0 ? */
        if(ReadTimer(&fTOCENDTimer) > 10000L){
            return(eDECODE_TOC_DONE);
        }
        else{
            return(fGetTOCState);
        }
    }
    else{
        return(eDECODE_TOC_DONE);
    }
}

/* end of files */
















