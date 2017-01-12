/****************************************************************************
*                (c) Copyright 2001  - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrDB.c
*
* DESCRIPTION   : This file contains the set/get database functions
*
* $Revision: 107 $
* $Date: 11/03/28 2:33p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/* comment : Laser Power ON/OFF SW => find "LPW" */

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\string.h"
#include ".\common\stdlib.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\hif\gp_ram.h"
#include ".\player\plrDB.h"    /* Player:DB      */
#include ".\player\plrServo.h" /* Player:Servo   */
#include ".\player\plrmon.h"

#if (BD_ENABLE == 1)
#include ".\bd\bdMacro.h"
#endif
#include ".\dvd\dvgvars.h"

#include ".\al\REG_global.h"
#include ".\AL\LAL_stream.h"

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/

//extern BOOL         Iram_fCDRW;    /* True if CDRW disc inserted, otherwise false */
//private BYTE        fCommandTrack;
BYTE        fCommandTrack;

/* Bit significant error information */
ErrorGroupDiscInfoT fErrorGroupDiscInfo; /* Disc Info */
ErrorGroupTrayT     fErrorGroupTray;     /* Tray      */
ErrorGroupReadT     fErrorGroupRead;     /* Read      */
ErrorGroupWriteT    fErrorGroupWrite;    /* Write     */
ErrorGroupOPCT      fErrorGroupOPC;      /* BOPC/ROPC */

/* Highest Recorded Track Number dur SAO write */
private BYTE fSAOHighestRecordedTrackNumber;

//extern LaserWriteModeT          Iram_fLaserWriteMode;  /* Current Write Mode */

/* True if Next Session is available to write */
//private AddrParmS   fSearchAddressLBA;
AddrParmS   fSearchAddressLBA;
PlayerStatesT   fPlayerStatusValue;
eOutputFormat fOutPutFormat;
BOOL        fNextSessionAvailable;

/**************** Public Variables ****************/
#pragma arm section rwdata = "info", zidata = "info"

    public tDiscDataS Info;

#pragma arm section rwdata, zidata

public tInfoDBaseS        BufInfo;

discKindT      LoadedDiscKind;     /* Disc Kind     */
discKindT      ForBCALoadedDiscKind;     /* For BCA Disc Kind     */

public BYTE ServoReadyStatus;

extern public tFinalSessionInfoS Iram_FinalSessionInfo;
public BYTE               ReadInfoMethod;

public BOOL fRawModeWritten;

public BYTE fEnableROC;
public BYTE fRecoverLevel;

public discKindT fReAdjustType;

public BYTE fJumpMode;

public BOOL C0FoundTOC;
public BOOL C1FoundTOC;
public BYTE Mode5SubQ[2][10];

#if (VCPS_TEST == 1)
public BOOL fVCPSInfoGet;
public BOOL fVCPSInfoValid;
#endif  /* VCPS_TEST == 1*/
#if (ENABLE_AACS == 1)
public BOOL fRomMarkInfoGet;
#endif  /* ENABLE_AACS == 1 */

/*************** DCBs for DVD+RW ***************/
//public tFDCBS * FDCB;
//public tFDCBS * FDCB_L1;
//public tWDCBS * WDCB;

/**************** Extern Variables ****************/
extern TimeS    fTOC_A2msf[2];

DiscStatusT pdbGetDiscStatus(void)
{
    if ((pdbDiscLoaded() == DVD_DISC)
#if (ENABLE_HDDVD == 1)
      ||(pdbDiscLoaded() == HD_DISC)
#endif
       )
    {
        if (pdbGetDVDFinalSessCloseInfo() == eSessionFinalized)
            return (eFinalizedDisc);
        else
            return (eNonFinalizedDisc);
    }
#if (BD_ENABLE == 1)
    else if (pdbDiscLoaded() == BD_DISC)
    {
        if (pdbGetBDFinalSessCloseInfo() == eSessionFinalized)
            return (eFinalizedDisc);
        else
            return (eNonFinalizedDisc);
    }
#endif
    else
    {   //CD disc
        if (pdbGetFinalSessCloseInfo() == eSessionFinalized)
            return (eFinalizedDisc);
        else
            return (eNonFinalizedDisc);
    }
}


/*----------------------------------------------------------------------*/
TimeS pdbGetLeadOutStartTime(void)
{
    TimeS TmpTime;

    if(Info.Media.CD.LeadOutStartTime.Minute == eTimeEmpty){
        TmpTime = pdbGetStopTime(pdbGetHighestRecordedTrkNumber());
        mathAddAddressToMSF(&TmpTime,1L,&TmpTime);
        return( TmpTime );
    }
    else{
        return(Info.Media.CD.LeadOutStartTime);
    }
}
/************************************************************************
// Name        : pdbSetTrackSessionNumber/pdbGetTrackSessionNumber
// Description : pdbSetTrackSessionNumberThis functions stores the session
//               number for a given  track number
//               IF TrackNumber is equal to 0, then set TrackNumber equal
//               to Highest Recorded Track Number +1
*************************************************************************/
void pdbSetTrackSessionNumber(BYTE TrackNumber, BYTE SessionNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    Info.Media.CD.TOCData[TrackNumber - 1].TrackSessionNumber = SessionNumber;
}
/*----------------------------------------------------------------------*/
BYTE pdbGetTrackSessionNumber(BYTE TrackNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    return(Info.Media.CD.TOCData[TrackNumber - 1].TrackSessionNumber);
}
/************************************************************************
// Name        : pdbSetTrackConAd/pdbGetTrackConAd
// Description : This functions stores the control BYTE for a given track
//               number. The track control mode can be derived from this
//               control BYTE.
//               conad (= the value 'control' , part of the subcode)
//               IF TrackNumber is equal to 0, then set TrackNumber equal to
//               the Highest Recorded Track Number +1
//               Set Info.Media.CD.TOCData[TrackNumber - 1].Con equal to the lower
//               nibble of the ControlAddress input
*************************************************************************/
BYTE pdbGetTrackConAd(BYTE TrackNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    return(Info.Media.CD.TOCData[TrackNumber - 1].Con);
}
/************************************************************************
// Name        : pdbSetTrackDataMode/pdbGetTrackDataMode
// Description : IF TrackNumber is equal to 0, then set TrackNumber equal to
//               the Highest Recorded Track Number +1
//               Set Info.Media.CD.TOCData[TrackNumber - 1].DataMode =
//                                   the DataNibble input ANDed with 0x0F
*************************************************************************/
void pdbSetTrackDataMode(BYTE TrackNumber,tTrackDataModeT DataNibble)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    Info.Media.CD.TOCData[TrackNumber -1].DataMode = DataNibble & 0x0F;
}
/*----------------------------------------------------------------------*/
tTrackDataModeT pdbGetTrackDataMode(BYTE TrackNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    return(Info.Media.CD.TOCData[TrackNumber - 1].DataMode);
}
/************************************************************************
// Name        : pdbSetStartTime
// Description : IF TrackNumber is equal to 0, then set TrackNumber equal to
//               the Highest Recorded Track Number +1
//               Set Info.Media.CD.TOCData[TrackNumber - 1].StartTime = TimeFrame
*************************************************************************/
void pdbSetStartTime(TimeS TimeFrame,BYTE TrackNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }
    Info.Media.CD.TOCData[TrackNumber - 1].StartTime = TimeFrame;
}
/*----------------------------------------------------------------------*/
TimeS pdbGetStartTime(BYTE TrackNumber)
{
    TimeS StartTime;

    if(TrackNumber == 0){
        mTimeSet(StartTime,0,0,0);
    }
    else{
        StartTime = Info.Media.CD.TOCData[TrackNumber - 1].StartTime;
    }
    return(StartTime);
}
/************************************************************************
// Name        : pdbSetStopTime/pdbGetStopTime
// Description : IF TrackNumber is equal to 0, then set TrackNumber equal
//               to the Highest Recorded Track Number +1.
//               Set Info.Media.CD.TOCData[TrackNumber - 1].StopTime = TimeFrame.
//               IF the TrackNumber is greater than the Highest Recorded Track
//               Number and greater than  the SAO Highest Recorded Track Number,
//               Set the SAO Highest Recorded Track Number with TrackNumber.
*************************************************************************/
void pdbSetStopTime(TimeS TimeFrame,BYTE TrackNumber)
{
    if(TrackNumber == 0){
        TrackNumber = pdbGetHighestRecordedTrkNumber() + 1;
    }

    Info.Media.CD.TOCData[TrackNumber - 1].StopTime = TimeFrame;

    if((TrackNumber > pdbGetHighestRecordedTrkNumber())&&
       (TrackNumber > fSAOHighestRecordedTrackNumber) ){
        fSAOHighestRecordedTrackNumber = TrackNumber;
    }
}
/*----------------------------------------------------------------------*/
TimeS pdbGetStopTime(BYTE TrackNumber)
{
    TimeS ReturnTime;

    if(TrackNumber == 0){
        mTimeSet(ReturnTime,0,0,0);
    }
    else{
        ReturnTime = Info.Media.CD.TOCData[TrackNumber - 1].StopTime;
    }
    return(ReturnTime);
}
/************************************************************************
// Name        : pdbGetNumberOfSessionsOnDisc
// Description : This function returns the number of finished sessions on
//               a disc.A session is finished when the corresponding leadin an
//               leadout area is written to disc.
//               Function
//               If  the current Disc Type is CD, then return the current
/                Session Counter value
//               Else
//               Return the current Session Counter value - 1
*************************************************************************/
BYTE pdbGetNumberOfSessionsOnDisc(void)
{
    BYTE FinalSessCloseInfo;
    BYTE FinalSessNumber;

    FinalSessCloseInfo = pdbGetFinalSessCloseInfo();
    FinalSessNumber    = pdbGetFinalSessNumber();

    if ((FinalSessCloseInfo == eEmptySession) ||
        (FinalSessCloseInfo == eNotSessionClosed)){
        return(FinalSessNumber - 1);
    }
    else{
        return(FinalSessNumber);
    }
}
/************************************************************************
// Name        : pdbGetFirstTrackOfSession
// Description : This function returns the tracknumber of the first track
//               of a given session number.
//               Function
//               If the Highest Recorded Track is equal to 0, then return 0
//               Else If the SessionNumber input is equal to the Session
//               Counter,then return the First Track in the last session
//               Else
//               Do a search of all recorded tracks from the Lowest Recorded
//               Track to the Highest Recorded Track.
//               Return the track number where that Track number's Session
//               number (Info.Media.CD.TOCData[TrackNumber - 1].
//               TrackSessionNumber) is equal the input session number.
//               If no match was found then return 0
*************************************************************************/
BYTE pdbGetFirstTrackOfSession(BYTE SessionNumber)
{
    BYTE Tno,EndTno,StartTno;

    if(pdbGetLowestRecordedTrackNumber() == 0){
        return(0);
    }
    if(pdbGetSessionCounter() == SessionNumber){
        return(pdbGetFirstTrackInLastSession());
    }
    else{
        StartTno = pdbGetLowestRecordedTrackNumber();
    EndTno = pdbGetHighestRecordedTrkNumber()+1; /* for Tno in new empty session */
        for(Tno = StartTno;Tno <= EndTno;Tno++){
            if(Info.Media.CD.TOCData[Tno - 1].TrackSessionNumber == SessionNumber){
                return(Tno);
            }
        }
        return(0);
    }
}
/************************************************************************
// Name        : pdbGetLastTrackOfSession
// Description : This function returns the tracknumber of the last track of a
//               given session number.
//               If the session number points to an unfinished session ,
//               then this function returns HighestRecordedTrackNumber.
//               For a completely empty disc, '00' will be returned.
//               Function
//               IF the Lowest Recorded Track Number is equal to 0, then
//               return 0
//               Else
//               Do a search of all recorded tracks in the specified session
//               Start from the First Recorded Track of the specified session
//               and limit track checks to within the specified session OR up
//               to the HighestRecorded Track.
//               Return the track number where that Track number's Session
//               number (Info.Media.CD.TOCData[TrackNumber -1].TrackSessionNumber) is
//               equal the input session number.
*************************************************************************/
BYTE pdbGetLastTrackOfSession(BYTE SessionNumber)
{
    BYTE StartTno,EndTno,Tno;

    if(pdbGetLowestRecordedTrackNumber() == 0){
        return(0);
    }
    StartTno = pdbGetFirstTrackOfSession(SessionNumber);
    EndTno   = pdbGetHighestRecordedTrkNumber();

    for(Tno = StartTno;Tno <= EndTno;Tno++){
        if(Info.Media.CD.TOCData[Tno - 1].TrackSessionNumber > SessionNumber) break;
    }
    return(--Tno);
}
/************************************************************************
// Name        : pdbGetSessionLeadOutStart
// Description : This function returns the start time of the leadout area
//               of the given session number.
//               If the session number points to an unfinished session,
//               the return time will be the highest
//               possible start time for the leadout area.
//               Function
//               If the Highest Recorded Track is equal to 0, then return
//               Info. ATIPData.LeadOutTime
//               Else If the input Session Number is greater than or equal
//               to the session counter and the disc type is CD-R,then return
//               Info.Media.CD.ATIPData.LeadOutTime. Else return the stop time of the
//               last track of the input session number
*************************************************************************/
TimeS pdbGetSessionLeadOutStart(BYTE SessionNumber)
{
    TimeS TmpTime;

    if(pdbGetHighestRecordedTrkNumber() == 0){
        return(Info.Media.CD.ATIPLeadOutStartTime);
    }

    if( (SessionNumber > pdbGetSessionCounter()) &&
        (pdbGetDiscStatus() == eNonFinalizedDisc)) {
        return(Info.Media.CD.ATIPLeadOutStartTime);
    }
    else{
        TmpTime = pdbGetStopTime(pdbGetLastTrackOfSession(SessionNumber));
        mathAddAddressToMSF(&TmpTime, 1L, &TmpTime);
        return( TmpTime );
    }
}
/************************************************************************
// Name        : pdbSetClearAll
// Description : This function will clear all data values before reading of the
//               TOC. (useful for debugging purposes).
*************************************************************************/
extern BOOL fB0valid;

void pdbSetClearAll(void)
{
    BYTE     tno,sno,i;
    ULONG    h;

    fB0valid = FALSE;

    Iram_DiscInfo.CD.QSubcodeValid  = FALSE;
    BufInfo.Catalog.Valid           = FALSE;
    BufInfo.ISRC.Valid              = FALSE;
    BufInfo.ISRC.ISRCTrack          = 0;
    BufInfo.RID.Valid               = FALSE;
    BufInfo.RID.RIDTrack            = 0;

    fSAOHighestRecordedTrackNumber  = 0;

    Iram_fLeadInFound       = FALSE;
    Iram_fCDRW              = FALSE;

    Info.DiscIDStatus       = eDiscIDNotFound;
    /* CHG From eDiscIsProtected */
    Info.DiscProtectionStatus              = eDiscIsNotProtected;
    Info.DiscStatus                        = eNonFinalizedDisc;
    Info.Media.CD.HybridStatus             = eHybrid;

    mTimeSet(Info.Media.CD.LeadOutStartTime,eTimeEmpty,eTimeEmpty,eTimeEmpty);

    Info.Media.CD.LowestRecordedTrackNumber  = 0;
    Info.Media.CD.HighestRecordedTrackNumber = 0;
    Info.Media.CD.FirstTrackInLastSession    = 1;
    Info.Media.CD.SessionCounter             = 1;
    Info.ServoReadyStatus              = FALSE;

    for(tno = 0;tno < MaxNumberOfTracks;tno++){
        Info.Media.CD.TOCData[tno].TrackSessionNumber  = 0;
        Info.Media.CD.TOCData[tno].Con                 = 0;
        Info.Media.CD.TOCData[tno].DataMode            = 0;
        Info.Media.CD.TOCData[tno].CopyrightStatus     = eCopyUnknown;
        Info.Media.CD.TOCData[tno].ContAddr            = 0;
        Info.Media.CD.TOCData[tno].ISRCValid           = FALSE;
        mTimeSet(Info.Media.CD.TOCData[tno].StartTime   ,0,0,0);
        mTimeSet(Info.Media.CD.TOCData[tno].StopTime    ,0,0,0);
        mTimeSet(Info.Media.CD.TOCData[tno].RecordedTime,0,0,0);
        /* RawData(1),Mode1(8),Mode2From1(10),Mode2From2(13) */
        Info.Media.CD.TOCData[tno].DataBlockType       = eDBTUnknownMode;
        /* fixed packet size */
        Info.Media.CD.TOCData[tno].FixedPacketSize     = 0L;
        /* RT(bit7),Blank(bit6)=1,Packet(bit5),FP(bit4),TDB(bit3) */
        Info.Media.CD.TOCData[tno].TrackStatus         = 0x00;
        Info.Media.CD.TOCData[tno].TrackState          = eTRACK_STATE_UNKNOWN;
        Info.Media.CD.TOCData[tno].TrackDiagStatus0    = 0x00;
        Info.Media.CD.TOCData[tno].TrackDiagStatus1    = 0x00;
        pdbSetTrackWriteMethod(tno,0x00);
    }

    for(sno = 0;sno < eMaxNumberOfSessions;sno++){
        Info.Media.CD.DiscContAx[sno].ContA0 = 0; /* control of A0 in TOC on Disc */
        Info.Media.CD.DiscContAx[sno].ContA1 = 0; /* control of A1 in TOC on Disc */
        Info.Media.CD.DiscContAx[sno].ContA2 = 0; /* control of A2 in TOC on Disc */
        Info.Media.CD.DiscModeType[sno]      = 0xFF;
    }

    /* Notice */


    mTimeSet(Iram_FinalSessionInfo.CD.LeadinTime ,0,0,0); /* Leadin Start Time */
    mTimeSet(Iram_FinalSessionInfo.CD.LeadoutTime,0,0,0); /* Leadin End Time   */

    Iram_FinalSessionInfo.CD.FirstTrackNumber = 0;        /* First Track Number */
    Iram_FinalSessionInfo.CD.LastTrackNumber  = 0;        /* Last Track Number  */

                                            /* First Track Start Time */
    mTimeSet(Iram_FinalSessionInfo.CD.FirstTrackTime,0,0,0);
                                    /* CD-DA/CD-ROM/,CD-I,CD-ROM XA */
    Iram_FinalSessionInfo.CD.Format = eSFTypeCDda_CDRom;
                    /* eEmptySession,eNotSessionClosed,eSessionClosed */
    Iram_FinalSessionInfo.CD.CloseInfo = eEmptySession;
                        /* Indicates that DiscCatalogNumber is valid  */
    Iram_FinalSessionInfo.CD.Catalog.Valid = FALSE;
                                /* Extracted RAW Disc Catalog Number */
    for(i = 0;i < 10;i++){
        Iram_FinalSessionInfo.CD.Catalog.CatalogCode[i] = 0x00;
    }
    /* Next Session Program Start Time */
    mTimeSet(Iram_FinalSessionInfo.CD.NextSessionStartTime,eTimeEmpty,eTimeEmpty,eTimeEmpty);
    /* Last PMA Information */
    /* The ATIP Time of Last Read PMA Zero */
    pdbSetDiscIDStatus(eDiscIDNotFound);      /* disc id avaiale status */

    mTimeSet(fTOC_A2msf[0],0,0,0);
    mTimeSet(fTOC_A2msf[1],0,0,0);

    /* Zero Clear SDRAM TOC Area */
    h = RDTOC_TOC_ADDR; SCSDRAM[h]=0x00; SCSDRAM[h+1]=0x00;
    h = RDTOC_SESSION_ADDR; SCSDRAM[h]=0x00; SCSDRAM[h+1]=0x00;
    h = RDTOC_FULLTOC_ADDR; SCSDRAM[h]=0x00; SCSDRAM[h+1]=0x00;



    pdbSetReadInfoMethod(eNOTRead);

    C0FoundTOC = FALSE;
    C1FoundTOC = FALSE;

#if MARGINAL_DISC
    Disc_Speed.WriteMarginal = 0;
#endif


    fReAdjustType.fDiscKind = eUnknownKind;
}
/************************************************************************
// Name        : pdbSetClearBufInfoAll
// Description :
*************************************************************************/
void pdbSetClearBufInfoAll( void )
{
    memset( &BufInfo, 0x00, sizeof( BufInfo ) );
}
/************************************************************************
// Name        : pdbSetClearErrors
// Description : This function will clear all the Error groups status.
*************************************************************************/
void pdbSetClearErrors(void)
{
    fErrorGroupTray     = eNoErrorTray;
    fErrorGroupDiscInfo = eNoErrorDiscInfo;
    fErrorGroupRead     = eNoErrorRead;
}
/************************************************************************
// Name        : pdbServoClearErrors
// Description :
*************************************************************************/
void pdbServoClearErrors(void)
{
    BufInfo.ErrorInfo.ServoError   = 0;
    BufInfo.ErrorInfo.PlayerError  = 0;
    BufInfo.ErrorInfo.DiscError    = 0;
    BufInfo.ErrorInfo.CommandError = 0;
}
/************************************************************************
// Name        : pdbGetMode0OnDisc
// Description : return Subcode MODE 0 found or Subcode Mode 0 not found.
*************************************************************************/
BYTE pdbGetMode0OnDisc(void)
{
    if(Iram_DiscInfo.CD.QSubcodeValid)
    {
        if(Iram_DiscInfo.CD.Q.Addr == eSubcodeMode0)
        {
                    /* Check for make sure Q code mode 0 */
                    if((Iram_DiscInfo.CD.Q.Tno == 0x00)&&(Iram_DiscInfo.CD.Q.Point == 0x00))
                    {    //Lead in or PMA
                        return(eMode0Found);
                    }
                    else if((Iram_DiscInfo.CD.Q.Tno == 0xFF)&&(Iram_DiscInfo.CD.Q.Point == 0xFF))
                    {   //Program Area
                        return(eMode0Found);
                    }
                    else
                    {   //invalid Q code
                        return(eSubcodeError);
                    }
        }
        else{
            return(eNoMode0Found);
        }
    }
    else
    {
        return(eSubcodeError);
    }
}
/************************************************************************
// Name        : pdbSetCatalogCode/pdbGetCatalogCode/pdbClearCatalogCode
// Description : Set/Get catalog valid status and the catalog number.
*************************************************************************/
void pdbSetCatalogCode(SubQCodeS *SubCode)
{
    memmove((void *)&BufInfo.Catalog.CatalogCode,(const void*) SubCode,10);
    BufInfo.Catalog.Valid = TRUE;
}
/*----------------------------------------------------------------------*/
BOOL pdbGetCatalogCode(BYTE *pCatalogCode)
{
    BYTE i;

    if(BufInfo.Catalog.Valid){
        for(i = 0; i < 10;i++){
            pCatalogCode[i] = BufInfo.Catalog.CatalogCode[i];
        }
    }
    return(BufInfo.Catalog.Valid);
}

/************************************************************************
// Name        : pdbSetISRCCode/pdbGetISRCCode/pdbClearISRCCode
// Description : Set/Get the ISRC Number and the track number.
*************************************************************************/
void pdbSetISRCCode(SubQCodeS *SubCode)
{
    memmove((void *)&BufInfo.ISRC.ISRCCode, (const void *)SubCode, 10);
    if(Iram_DiscInfo.CD.QSubcodeValid){
        BufInfo.ISRC.ISRCTrack = BufInfo.SubQ.Tno;
    }
    BufInfo.ISRC.Valid = TRUE;
}
/*----------------------------------------------------------------------*/
BYTE pdbGetISRCCode(BYTE *pISRCCode)
{
    BYTE i;

    if(BufInfo.ISRC.ISRCTrack != 0){
        for(i = 0;i < 10;i++) pISRCCode[i] = BufInfo.ISRC.ISRCCode[i];
    }
    return(BufInfo.ISRC.ISRCTrack);
}

/************************************************************************
// Name        : pdbSetRIDCode/pdbGetRIDCode
// Description : Set/Get the RID Number and the track number.
*************************************************************************/
void pdbSetRIDCode(SubQCodeS *SubCode)
{
    memmove((void *)&BufInfo.RID.RIDCode, (const void *)SubCode, 10);
    if(Iram_DiscInfo.CD.QSubcodeValid){
        BufInfo.RID.RIDTrack = BufInfo.SubQ.Tno;
    }
    BufInfo.RID.Valid = TRUE;
}
/*----------------------------------------------------------------------*/
BYTE pdbGetRIDCode(BYTE *pRIDCode)
{
    BYTE i;

    if(BufInfo.RID.RIDTrack != 0){
        for(i = 0; i < 10; i++) pRIDCode[i] = BufInfo.RID.RIDCode[i];
    }
    return(BufInfo.RID.RIDTrack);
}

/************************************************************************
// Name        : pdbGetSubcodeInformation
// Description : Return the genarate Information.
*************************************************************************/
BOOL pdbGetSubcodeInformation(SubQCodeS *subc)
{
    memmove((void *)subc, (const void *)&Iram_DiscInfo.CD.Q, 10);
    return(pdbGetSubcodeOK());
}
/************************************************************************
// Set - Track Cont Addr
*************************************************************************/
void pdbSetTrackContAddr(BYTE tno,BYTE ContAddr)
{
    Info.Media.CD.TOCData[tno-1].ContAddr =  ContAddr;
    Info.Media.CD.TOCData[tno-1].Con      = (ContAddr & 0xF0)>>4;
}
/************************************************************************
// Set - Track Recorded Time
*************************************************************************/
void pdbSetTrackRecordedTime(BYTE tno,TimeS time)
{
    Info.Media.CD.TOCData[tno-1].RecordedTime = time;
}

/************************************************************************
// Set - Track Data Block Type
*************************************************************************/
void pdbSetTrackDataBlockType(BYTE tno,DataBlockTypeT BlockType)
{
    Info.Media.CD.TOCData[tno-1].DataBlockType = BlockType;
}
/************************************************************************
// Set - Track Data Form
*************************************************************************/
void pdbSetTrackDataForm(BYTE tno,BYTE DataForm)
{
    Info.Media.CD.TOCData[tno-1].DataForm = DataForm;
}
/************************************************************************
// Set - Track Packet Size
*************************************************************************/
void pdbSetTrackPacketSize(BYTE tno,LONG PacketSize)
{
    Info.Media.CD.TOCData[tno-1].FixedPacketSize = PacketSize;
}
/************************************************************************
// Set - Track State
*************************************************************************/
void pdbSetTrackState(BYTE tno,BYTE TrackState)
{
    Info.Media.CD.TOCData[tno-1].TrackState = TrackState;
}
/************************************************************************
// Set - Track Status
*************************************************************************/
void pdbSetTrackStatus(BYTE tno,BYTE TrackStatus)
{
    Info.Media.CD.TOCData[tno-1].TrackStatus = TrackStatus;
}
/************************************************************************
// BitSet/BitReset - Track Status
*************************************************************************/
void pdbBitSetTrackStatus(BYTE tno,BYTE TrackStatus)
{
    Info.Media.CD.TOCData[tno-1].TrackStatus =
                            Info.Media.CD.TOCData[tno-1].TrackStatus | TrackStatus;
}
/*----------------------------------------------------------------------*/
void pdbBitResetTrackStatus(BYTE tno,BYTE TrackStatus)
{
    Info.Media.CD.TOCData[tno-1].TrackStatus =
                            Info.Media.CD.TOCData[tno-1].TrackStatus & (~TrackStatus);
}
/************************************************************************
// Set - Track Diag Status
*************************************************************************/
void pdbSetTrackDiagStatus(BYTE tno,BYTE Status)
{
    Info.Media.CD.TOCData[tno-1].TrackDiagStatus0 = Status;
}
/************************************************************************
// Set - Track Diag Status2
*************************************************************************/
void pdbSetTrackDiagStatus2(BYTE tno,BYTE Status)
{
    Info.Media.CD.TOCData[tno-1].TrackDiagStatus1 = Status;
}
/************************************************************************
// Set - Track Write Method
*************************************************************************/
void pdbSetTrackWriteMethod(BYTE tno,BYTE method)
{
    Info.Media.CD.TOCData[tno-1].WriteMethod = method;
}
/************************************************************************
// Set - Track NWA
*************************************************************************/
void pdbSetTrackNWA(BYTE tno,LONG addr)
{
    Info.Media.CD.TOCData[tno-1].NextWritableAddress = addr;
}
/************************************************************************
// Get - EndTime Of Last CloseSession
*************************************************************************/
LONG pdbGetEndTimeOfLastCloseSession(void)
{
    BYTE first_tno,last_tno,info,sno;
    TimeS time;

    if(pdbGetDiscKind() == eCDROMDisc){
        last_tno = pdbGetHighestRecordedTrkNumber();
        time = pdbGetStopTime(last_tno);
    }
    else{
        info      = pdbGetFinalSessCloseInfo();
        sno       = pdbGetFinalSessNumber();
        first_tno = pdbGetFinalSessFirstTrackNumber();
        last_tno  = pdbGetFinalSessLastTrackNumber();

        /* closed session */
        if((info == eSessionClosed) || (info == eSessionFinalized)){
            time = pdbGetStopTime(last_tno);
        }
        else{
            if(sno >1){
                last_tno = first_tno -1;
                time = pdbGetStopTime(last_tno);
            }
            else{
                time.Minute=91; time.Second=0; time.Frame=0;
                return MSF2LBA(&time);
            }
        }
    }
    return MSF2LBA_Plus(&time);
}
/************************************************************************
// Set/Get - Disc Cont Ax
*************************************************************************/
void pdbSetDiscContAx(BYTE sno,BYTE ax,BYTE Cont)
{
    switch(ax) {
        case ePointA0: Info.Media.CD.DiscContAx[sno-1].ContA0 = Cont; break;
        case ePointA1: Info.Media.CD.DiscContAx[sno-1].ContA1 = Cont; break;
        case ePointA2: Info.Media.CD.DiscContAx[sno-1].ContA2 = Cont; break;
    }
}
/************************************************************************
// Set - PreGap StartTime
*************************************************************************/
//void pdbSetPreGapStartTime(BYTE tno,TimeS time)
//{
//    CueSheet.PreGapTime[tno-1].PreGapStartTime = time;
//}
/************************************************************************
// Set - PreGap StartTime Valid
*************************************************************************/
//void pdbSetPreGapStartTimeValid(BYTE tno,BOOL valid)
//{
//    CueSheet.PreGapTime[tno-1].PreGapStartTimeValid = valid;
//}
/************************************************************************
// Set/Get/Check - ISRC
*************************************************************************/
void pdbSetISRC(BOOL Valid,BYTE tno, BYTE *code,BYTE src)
{
    BYTE i;
    vBYTE *p;

    if(tno < 1){
        return;
    }
    /* Indicates that ISRC is valid */
    Info.Media.CD.TOCData[tno - 1].ISRCValid = Valid;
    p = Info.Media.CD.TOCData[tno - 1].ISRC;
    if(Valid == FALSE){
        /* Clear at ERASE */
        for(i=0;i<10;i++){
            p[i] = 0x00;
        }
        return;
    }
    switch(src){
        case 0x55: /* Mode Select Write Parameters Mode Page */
            p[0] = 0x03; /* mode 3 */
            p[1] = ((*(code +  0) - '0') << 2) + ((*(code +  1) -'0') >> 4);
            p[2] = ((*(code +  1) - '0') << 4) + ((*(code +  2) -'0') >> 2);
            p[3] = ((*(code +  2) - '0') << 6) +  (*(code +  3) -'0');
            p[4] =  (*(code +  4) - '0') << 2;
            p[5] =  (*(code +  5)        << 4) +  (*(code +  6) & 0x0F);
            p[6] =  (*(code +  7)        << 4) +  (*(code +  8) & 0x0F);
            p[7] =  (*(code +  9)        << 4) +  (*(code + 10) & 0x0F);
            p[8] =  (*(code + 11)        << 4);
            p[9] = 0x00; /* Aframe */
            break;
        case 0x5D: /* Send Que Sheet */
            p[0] = 0x03; /* mode 3 */
            p[1] = ((*(code +  2) - '0') << 2) + ((*(code +  3) -'0') >> 4);
            p[2] = ((*(code +  3) - '0') << 4) + ((*(code +  4) -'0') >> 2);
            p[3] = ((*(code +  4) - '0') << 6) +  (*(code +  5) -'0');
            p[4] =  (*(code +  6) - '0') << 2;
            p[5] =  (*(code +  7)        << 4) +  (*(code + 10) & 0x0F);
            p[6] =  (*(code + 11)        << 4) +  (*(code + 12) & 0x0F);
            p[7] =  (*(code + 13)        << 4) +  (*(code + 14) & 0x0F);
            p[8] =  (*(code + 15)        << 4);
            p[9] = 0x00; /* Aframe */
            break;
    }
}
/*----------------------------------------------------------------------*/
void pdbGetISRC(BYTE tno,BYTE *code)
{
    BYTE i;

    for(i = 0;i < 10;i++){
    code[i] = Info.Media.CD.TOCData[tno-1].ISRC[i];
    }
}
/************************************************************************
// Set/Get/Check - ISRC
*************************************************************************/
void pdbCopySubcodeToMem(SubQCodeS *subcode,BYTE *dest)
{
    BYTE cont_addr;

    cont_addr = subcode->Addr & 0x0F;
    cont_addr <<= 4;
    cont_addr |= subcode->Con & 0x0F;
    dest[0] = cont_addr;
    dest[1] = subcode->Tno;
    dest[2] = subcode->Point;
    dest[3] = subcode->Minute;
    dest[4] = subcode->Second;
    dest[5] = subcode->Frame;
    dest[6] = subcode->Zero;
    dest[7] = subcode->PMinute;
    dest[8] = subcode->PSecond;
    dest[9] = subcode->PFrame;
}
/*----------------------------------------------------------------------*/
void pdbCopyMemToSubcode(BYTE *src,SubQCodeS *subcode)
{
    subcode->Addr    = (src[0] & 0xF0) >> 4;
    subcode->Con     = src[0] & 0x0F;
    subcode->Tno     = src[1];
    subcode->Point   = src[2];
    subcode->Minute  = src[3];
    subcode->Second  = src[4];
    subcode->Frame   = src[5];
    subcode->Zero    = src[6];
    subcode->PMinute = src[7];
    subcode->PSecond = src[8];
    subcode->PFrame  = src[9];
}

/************************************************************************
// Set/Get/ - SubCodeMode5
*************************************************************************/
void pdbSetSubCodeMode5(BYTE point,SubQCodeS *subcode,BOOL found)
{
    if(point == PointC0){
        C0FoundTOC = found;
    }
    else{
        C1FoundTOC = found;
    }
    if(found == TRUE){
        pdbCopySubcodeToMem(subcode,&(Mode5SubQ[point][0]));
    }
}

BOOL pdbGetSubCodeMode5(BYTE point,SubQCodeS *subcode)
{
    BOOL found;

    if(point == PointC0){
        found = C0FoundTOC;
    }
    else{
        found = C1FoundTOC;
    }
    if(found == TRUE){
        pdbCopyMemToSubcode(&(Mode5SubQ[point][0]),subcode);
    }
    return(found);
}

/*******************************************************************************

    FUNCTION        pdbDiscLoaded

    DESCRIPTION     Returns either CD or DVD disc

    RETURNS

    NOTES           Used with OEM feature 2

********************************************************************************/
BYTE pdbDiscLoaded()
{
#if (EN_LS == 1)
    if (pdbGetDiscKind() == eLSDisc)
        return(NON_CD_DVD_DISC);
#endif // End of (EN_LS == 1)

    switch (BufInfo.StatusInfo.DiscKind.Bit.DiscStd)
    {
    #if (BD_ENABLE)
        case eDiscBD:
            return (BD_DISC);
    #endif
    #if (ENABLE_HDDVD)
        case eDiscHD:
            return (HD_DISC);
    #endif
        case eDiscDVD:
            return (DVD_DISC);
        default:
            return (CD_DISC);
    }
}

/*******************************************************************************

    FUNCTION        pdbInitDiscSelect
    DESCRIPTION
    RETURNS
    NOTES
    DISCFMT (3¡V0)
      CD       DVD      Blu-Ray   HDDVD
    9 Reserved -R CSS   Reserved  Reserved
    8 Reserved +RW      Reserved  Reserved
    7 Reserved +R       Reserved  Reserved
    6 Reserved -RW      Reserved  Reserved
    5 Reserved -Rtype2  -RE 27GB  Reserved
    4 Reserved -Rtype1  -RE 25GB  -RW
    3 Reserved -RAMv2c1 -R 27GB   -R
    2 -RW      -RAMv2c0 -R 25GB   -RAM
    1 -R       -RAM v1  -ROM 27GB -ROM Version 10 for China only
    0 -Default -ROM-ROM -ROM 25GB -ROM

********************************************************************************/
void  pdbInitDiscSelect()
{
    BYTE disc_standard, disc_dl, disc_format;

    disc_standard = 0;
    disc_dl = 0;
    disc_format = 0;

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            disc_standard = halGLOBAL_DiscStd_CD;
            switch(pdbGetDiscKind())
            {
                case eCDROMDisc:
                    disc_format = 0;
                    break;
                case eCDRDisc:
                    disc_format = 1;
                    break;
                case eCDRWDisc:
                    disc_format = 2;
                    break;
            }
            break;
        case DVD_DISC:
            disc_standard = halGLOBAL_DiscStd_DVD;
            switch(pdbGetDiscKind())
            {
                case eDVDROMDisc:
                case eDVDROMDLDisc:
                    disc_format = 0;
                    break;
                case eDVDRAMDisc:
                    disc_format = 3;
                    break;
                case eDVDMinusRDisc:
                case eDVDMinusRDLDisc:
                    disc_format = 5;
                    break;
                case eDVDMinusRWDisc:
                    disc_format = 6;
                    break;
                case eDVDPlusRDisc:
                case eDVDPlusRDLDisc:
                    disc_format = 7;
                    break;
                case eDVDPlusRWDisc:
                case eDVDPlusRWDLDisc:
                    disc_format = 8;
                    break;
            }
            break;
#if (BD_ENABLE)
        case BD_DISC:
             disc_standard = halGLOBAL_DiscStd_BD;
            switch(pdbGetDiscKind())
            {
                case eBDROMDisc:
                case eBDROMDLDisc:
                    disc_format = 0;
                    break;
                case eBDRDisc:
                case eBDRDLDisc:
                    disc_format = 2;
                    break;
                case eBDREDisc:
                case eBDREDLDisc:
                    disc_format = 4;
                    break;
            }

            break;
#endif  /* BD_ENABLE */
#if (ENABLE_HDDVD)
        case HD_DISC:
            disc_standard = halGLOBAL_DiscStd_HDDVD;
            switch(pdbGetDiscKind())
            {
                case eHDROMDisc:
                case eHDROMDLDisc:
                    disc_format = 0;
                    break;
                #if (ENABLE_CBHD == 1)
                case eCBHDROMDisc:
                case eCBHDROMDLDisc:
                    disc_format = 1;
                    break;
                #endif
                case eHDRAMDisc:
                    disc_format = 2;
                    break;
                case eHDRDisc:
                case eHDRDLDisc:
                    disc_format = 3;
                    break;
                case eHDRWDisc:
                case eHDRWDLDisc:
                    disc_format = 4;
                    break;
            }
            break;
#endif
    }

    if(BufInfo.StatusInfo.DiscKind.Bit.Layers != eLayersDouble) //eLayersUnknown or eLayersSingle
        disc_dl = 0;
    else
        disc_dl = 1;

    WRITE_FIELD(HAL_GLOBAL_DISCSTD,disc_standard);
    WRITE_FIELD(HAL_GLOBAL_DL,disc_dl);
    WRITE_FIELD(HAL_GLOBAL_DISCFMT,disc_format);
 }

/*******************************************************************************

    FUNCTION        pdbClearDiscSelect
    DESCRIPTION
    RETURNS
    NOTES

********************************************************************************/
void  pdbClearDiscSelect()
{
    WRITE_REG(HAL_GLOBALREG_MAP->DISCSEL,0);
}

