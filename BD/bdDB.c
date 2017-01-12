/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdDB.c $
*
* DESCRIPTION   This file contains BD related database utilities
*
* $Revision: 30 $
* $Date: 09/12/09 4:11p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\bd\bdDB.h"
#include ".\bd\bdDiag.h"

#if (BD_ENABLE)
/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/

/*********************************************************************
   FUNCTIONS
*********************************************************************/

/*--------------------------------------------------------------------
  Function:     pdbGetIHBDDisc()

  Description:  

--------------------------------------------------------------------*/
BOOL pdbGetIHBDDisc(void)
{
    if ((Info.Media.BD.DiscInfoP_L1_Fmt1->LayerTypeIdH == 'B' ) && (Info.Media.BD.DiscInfoP_L1_Fmt1->LayerTypeIdM == 'D' )
        && ((Info.Media.BD.DiscInfoP_L1_Fmt1->LayerTypeIdL ==  'o'  ) || (Info.Media.BD.DiscInfoP_L1_Fmt1->LayerTypeIdL == 'w' )))
        return (TRUE);
    else
        return (FALSE);
}

/*--------------------------------------------------------------------
  Function:     pdbInitBDDataBase()

  Description:  Initials the BD Data Base

--------------------------------------------------------------------*/
void pdbInitBDDataBase(void)
{
    USHORT index;

    // -----------------------------------------------------
    //  Clear Track Info Structure
    // -----------------------------------------------------
    for (index=0; index < 256; index++)
    {
        Info.Media.BD.Track[index].TrackStatus = eTS_None;
        Info.Media.BD.Track[index].TrackState = eTRACK_STATE_UNKNOWN;
        Info.Media.BD.Track[index].TrackSessionNumber = 0;
        Info.Media.BD.Track[index].TrackDiagStatus0 = 0;
        Info.Media.BD.Track[index].TrackDiagStatus1 = 0;
        Info.Media.BD.Track[index].StartPSN.PSN = 0L;
        Info.Media.BD.Track[index].EndPSN.PSN = 0L;
        Info.Media.BD.Track[index].RecordedPSN.PSN = 0L;
        Info.Media.BD.Track[index].NextWritableLBA = 0L;
    }

    // -----------------------------------------------------
    //  Clear BD Final Session Data Structure
    // -----------------------------------------------------
    Iram_FinalSessionInfo.BD.SessionNumber = 0;
    Iram_FinalSessionInfo.BD.FirstTrackNumber = 0;
    Iram_FinalSessionInfo.BD.LastTrackNumber = 0;
    Iram_FinalSessionInfo.BD.Format = 0;
    Iram_FinalSessionInfo.BD.CloseInfo = eEmptySession;
    Iram_FinalSessionInfo.BD.LeadinPSN = 0L;
    Iram_FinalSessionInfo.BD.LeadoutPSN = 0L;
    Iram_FinalSessionInfo.BD.NextSessionStartPSN = 0L;
    Iram_FinalSessionInfo.BD.FirstTrackPSN = 0L;
    
    pDDS = (tBdDdsS *) BD_TDDS_ADR;
    pSRRI = (tBdSrriS *) BD_SRRI_SBM0_ADR;
    pPAC =  (tBdPacS *)BD_PRM_PAC_ADR;
    // -----------------------------------------------------
    //  Clear Physical Format Structure
    // -----------------------------------------------------
    Info.Media.BD.DiscInfoP_L0_Fmt1 = (tBdDiscInformationS *) BD_DI_ADR;
    if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
        Info.Media.BD.DiscInfoP_L1_Fmt1 = (tBdDiscInformationS *) (BD_DI_ADR + 64);
    else
        Info.Media.BD.DiscInfoP_L1_Fmt1 = (tBdDiscInformationS *) (BD_DI_ADR + 112);

    LinearPatternFill(BD_PRM_PAC_PAGE,0,0x11,0x800/2,0);// PAC only has 0x11 pages.
    // -----------------------------------------------------
    //  Clear Current Disc Structure
    // -----------------------------------------------------
    Info.Media.BD.fDIInfoValid = FALSE;
    Info.Media.BD.LowestRecordedTrackNumber = 0;
    Info.Media.BD.LogicalTracks = 0;
    Info.Media.BD.SessionCounter = 0;
    Info.DiscProtectionStatus = eDiscIsNotProtected;
    Info.DiscStatus           = eNonFinalizedDisc;
    Info.DiscIDStatus = 0;

    // -----------------------------------------------------
    //  Clear BD Logical Data
    // -----------------------------------------------------
    Info.Media.BD.MaxLBA = 0L;
    Info.Media.BD.EndLBALayer0 = 0L;
    Info.Media.BD.EndPSNLayer0 = 0L;
    Info.Media.BD.EndPSNLayer1 = 0L;
    READ_LDC_BIS = 0;
}

/************************************************************************
// Get - BD Blank Disc Info
*************************************************************************/
BOOL pdbGetBDBlankDiscInfo(void)
{
    if((pdbGetBDFinalSessCloseInfo() == eEmptySession) && (pdbGetBDFinalSessFirstTrackNumber() == 1))
        return (TRUE);
    else
        return (FALSE);
}

/************************************************************************
// Get - BD Writable or not
*************************************************************************/
BYTE pdbGetBDRW(void)
{
    switch(pdbGetDiscKind())
    {
        case eBDREDisc:
        case eBDREDLDisc:
            return(TRUE);
    }

    return(FALSE);
}

/*******************************************************************************

    FUNCTION        pdbGetBDClosedSessions

    DESCRIPTION     Determines the number of closed sessions

    RETURNS         the number of closed sessions on the mounted disc

********************************************************************************/
BYTE pdbGetBDClosedSessions(void)
{
    BYTE LastSessionStatus;

    LastSessionStatus = pdbGetBDFinalSessCloseInfo();

    if (((LastSessionStatus == eEmptySession) || (LastSessionStatus == eNotSessionClosed))
        && (Info.Media.BD.SessionCounter != 0))
        return(Info.Media.BD.SessionCounter - 1);
    else
        return(Info.Media.BD.SessionCounter);
}

/*****************************************************************************

    FUNCTION        pdbSetBDMaxPSN

    DESCRIPTION     Set the maximum PSN of the given layer.

    NOTES           Insure PFInfo variables are saved in big endian format

*****************************************************************************/
void pdbSetBDMaxPSN(BYTE layer, ULONG PSN)
{
    if (pdbGetBDLayers() == BD_DUAL_LAYER && layer == 1)
        Info.Media.BD.EndPSNLayer1 = PSN;
    else
        Info.Media.BD.EndPSNLayer0 = PSN;
}

/*****************************************************************************

    FUNCTION        pdbGetBDMaxPSN

    DESCRIPTION     Determines the maximum PSN of the given layer

    RETURNS         MaxPSN

    NOTES           Save in big endian format for PFInfo variables

*****************************************************************************/
ULONG pdbGetBDMaxPSN(BYTE layer)
{
    ULONG UserDataEndPSN;
    if (pdbGetBDType() == BD_ROM)
    {
        // BD_ROM
        if ((pdbGetBDLayers() == BD_SINGLE_LAYER)|| (pdbGetBDLayers() == BD_DUAL_LAYER && layer == 0))
            UserDataEndPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone)+1;
        else 
            UserDataEndPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.LAUOfDataZone)+1;            
    }
    else //BD_R, BD_RE
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER && layer == 1)
        {
            UserDataEndPSN = ((pdbGetBDL1EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1; // Physical L1 End PSN
            UserDataEndPSN -= (pdbGetBDIsa1Size() * SECTORS_PER_CLUSTER);
        }
        else
        {
            UserDataEndPSN = ((pdbGetBDL0EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1; // Physical L0 End PSN
            UserDataEndPSN -= (pdbGetBDOsaSize() * SECTORS_PER_CLUSTER);
        }
    }

    return UserDataEndPSN;
}

/*******************************************************************************

    FUNCTION        pdbSetBDLogicalData

    DESCRIPTION     Sets the discs logical data based on the disc information(DI)

********************************************************************************/
void pdbSetBDLogicalData()
{
    ULONG *sdramAddress;
    ULONG EndPSN0;

   /*---------------------------------------------------------------
        Update Maximum LBA and last LBA of layer
    ----------------------------------------------------------------*/
    if (pdbGetBDLayers() == BD_SINGLE_LAYER)
    {
        switch(pdbGetBDType())
        {
            case BD_ROM:
                Info.Media.BD.MaxLBA = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LastUserDataAddress) -
                                   SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone);
                break;

            case BD_R:
            case BD_RE:
                Info.Media.BD.MaxLBA = pdbGetBDEndLSN();
                break;
        }

        Info.Media.BD.EndLBALayer0 = Info.Media.BD.MaxLBA;
    }
    else
    {
        switch(pdbGetBDType())
        {
            case BD_ROM:
                Info.Media.BD.EndLBALayer0 = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone) -
                                         SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone) + 1L;

                Info.Media.BD.MaxLBA = Info.Media.BD.EndLBALayer0*2 + 1L ;
                break;

            case BD_R:
            case BD_RE:
                //EndPSN0 is Layer0 end PSN = 8*LAA + 15
                EndPSN0 = (pdbGetBDL0EndPAA() << 3);
                EndPSN0 += 15;

                if(pdbGetBDBlankDiscInfo() == TRUE)
                {
                    Info.Media.BD.EndLBALayer0 = EndPSN0 - BD_DATAZONE_START_PSN;
                    Info.Media.BD.MaxLBA = 2 * Info.Media.BD.EndLBALayer0;
                }
                else
                {
                    EndPSN0 -= pdbGetBDOsaSize() * ONE_BD_CLUSTER;
                    Info.Media.BD.EndLBALayer0 = EndPSN0 - (pdbGetBDIsa0Size() * ONE_BD_CLUSTER + BD_DATAZONE_START_PSN);
                    Info.Media.BD.MaxLBA = pdbGetBDEndLSN();  //return last recordable LBA from DDS
                }

                break;
        }
    }

    /*---------------------------------------------------------------
        Update MAXLBA information in SDRAM
    ----------------------------------------------------------------*/
    sdramAddress     = (ULONG *) (SCSDRAM_START_ADDR + MAXLBA_OFFSET);
    *sdramAddress    = SWAPEND_ULONG(Info.Media.BD.MaxLBA);
    bdConvert_init();    
}

/*****************************************************************************

    FUNCTION        pdbGetBDStartPSN

    PARAMETERS      layer

    RETURNS         Starting PSN of the given layer

*****************************************************************************/
ULONG pdbGetBDStartPSN(BYTE layer)
{
    ULONG TempAddress;

    if (pdbGetBDType() == BD_ROM)
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER && layer == 1)
            TempAddress = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.FAUOfDataZone);
        else
            TempAddress = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone);
    }
    else //BD_R, BD_RE
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER && layer == 1)
            TempAddress = ConvertPSNL0ToL1(pdbGetBDMaxPSN(0));
        else
            TempAddress = pdbGetBDLsn0StartPSN();
    }

    return (TempAddress);
}


/*****************************************************************************

    FUNCTION        pdbGetBDLeadOutStartPSN

    PARAMETERS      None

    RETURNS         LeadOut Start PSN

*****************************************************************************/
ULONG pdbGetBDLeadOutStartPSN(void)
{
    StLongU TempAddress;

    if(pdbGetBDType() == BD_ROM)
    {
        TempAddress.Long = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LastUserDataAddress) + 1L;
    }
    else
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER)
            TempAddress.Long = SWAPEND_ULONG((Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.BdWritable.LPAAOfDataZone));
        else
            TempAddress.Long = SWAPEND_ULONG((Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.BdWritable.LPAAOfDataZone));

        TempAddress.Long = (TempAddress.Long << 3);
        TempAddress.Long &= BD_CLUSTER_MASK;
        TempAddress.Long += ONE_BD_CLUSTER;
    }

    return(TempAddress.Long);
}


/*****************************************************************************

    FUNCTION        pdbGetBDL0EndPAA

    DESCRIPTION     This function return the Physical ADIP address stored in DI unit.

    PARAMETERS      None

    RETURNS         Layer 0 End Physical ADIP Address

*****************************************************************************/
ULONG pdbGetBDL0EndPAA(void)
{
    ULONG tmp1, tmp2;

    tmp1 = Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.BdWritable.LPAAOfDataZone;
    tmp2 = SWAPEND_ULONG(tmp1);
    return tmp2;
}

ULONG pdbGetBDL1EndPAA(void)
{
    return (SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.BdWritable.LPAAOfDataZone));
}

/*****************************************************************************

    FUNCTION        pdbGetBDL1StartPAA

    DESCRIPTION     This function return the Physical ADIP address stored in DI unit.

    PARAMETERS      None

    RETURNS         Layer 1 Start Physical ADIP Address

*****************************************************************************/
ULONG pdbGetBDL1StartPAA(void)
{
    ULONG tmp1, tmp2;

    tmp1 = Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.BdWritable.FPAAOfDataZone;
    tmp2 = SWAPEND_ULONG(tmp1);
    return tmp2;
}

/************************************************************************
// Name        : pdbSetBDCopyrightStatus
// Description : This function allows setting of the copyright status for
//               a track to be recorded.
//               Function
//               IF TrackNumber is equal to 0,
//               then set TrackNumber equal to the Highest Recorded Track
//               Number +1
//               Return Info.Media.BD.Track[ Tno - 1 ].CopyrightStatus = Mode
*************************************************************************/
void pdbSetBDCopyrightStatus(BYTE Tno, CopyModeT Mode)
{
    if(Tno == 0){
        Tno = Info.Media.BD.LogicalTracks;
    }
    Info.Media.BD.Track[Tno-1].CopyrightStatus = Mode;
}

/************************************************************************
// Set/Get - Track Cont Addr
*************************************************************************/
void pdbSetBDTrackContAddr(BYTE Tno, BYTE ContAddr)
{
    Info.Media.BD.Track[Tno-1].ContAddr =  ContAddr;
    Info.Media.BD.Track[Tno-1].Con      = (ContAddr & 0xF0)>>4;
}

/************************************************************************
// Set/Get - BD Track Packet Size
*************************************************************************/
void pdbSetBDTrackPacketSize(BYTE Tno,LONG PacketSize)
{
    Info.Media.BD.Track[Tno-1].FixedPacketSize = PacketSize;
}
/*----------------------------------------------------------------------*/
LONG pdbGetBDTrackPacketSize(BYTE Tno)
{
    return Info.Media.BD.Track[Tno-1].FixedPacketSize;
}

/************************************************************************
// Name        : pdbSetBDTrackDataMode
// Description : IF TrackNumber is equal to 0, then set TrackNumber equal to
//               the Highest Recorded Track Number +1
//               Set Info.Media.BD.Track[Tno - 1].DataMode =
//                                   the DataNibble input ANDed with 0x0F
*************************************************************************/
void pdbSetBDTrackDataMode(BYTE Tno,tTrackDataModeT DataNibble)
{
    if(Tno == 0){
        Tno = pdbGetBDHighestRecordedTrackNumber();
    }
    Info.Media.BD.Track[Tno-1].DataMode = DataNibble & 0x0F;
}
/*----------------------------------------------------------------------*/
tTrackDataModeT pdbGetBDTrackDataMode(BYTE Tno)
{
    if(Tno == 0){
        Tno = pdbGetBDHighestRecordedTrackNumber();
    }
    return(Info.Media.BD.Track[Tno-1].DataMode);
}

/************************************************************************
// Name        : pdbSetBDTrackConAddr/pdbGetBDTrackConAddr
// Description : This functions stores the control BYTE for a given track
//               number. The track control mode can be derived from this
//               control BYTE.
//               conad (= the value 'control' , part of the subcode)
//               IF TrackNumber is equal to 0, then set TrackNumber equal to
//               the Highest Recorded Track Number +1
//               Set Info.Media.BD.Track[Tno - 1].Con equal to the lower
//               nibble of the ControlAddress input
*************************************************************************/
void pdbSetBDTrackConAddr(BYTE Tno,BYTE ControlAddress)
{
    if(Tno == 0){
        Tno  = pdbGetBDHighestRecordedTrackNumber();
    }
    Info.Media.BD.Track[Tno-1].Con = ControlAddress & 0x0F;
}

/*----------------------------------------------------------------------*/
BYTE pdbGetBDTrackConAddr(BYTE Tno)
{
    if(Tno == 0){
        Tno = pdbGetBDHighestRecordedTrackNumber();
    }
    return(Info.Media.BD.Track[Tno-1].Con);
}

/************************************************************************
// Set/Get - BD Track Recorded PSN
*************************************************************************/
void pdbSetBDTrackRecordedPSN(BYTE Tno, ULONG info)
{
    if ( (pdbGetBDLayers() == BD_DUAL_LAYER) && (info > pdbGetBDMaxPSN(0))
        && (info < pdbGetBDStartPSN(1)) )
    {
        if (info & 0x1000000)    // ADDR is located on layer 1
            info = pdbGetBDMaxPSN(0) - (pdbGetBDStartPSN(1) - info) + 1L;
        else
            info = pdbGetBDStartPSN(1) + info - (pdbGetBDMaxPSN(0) + 1L);
    }
    Info.Media.BD.Track[Tno-1].RecordedPSN.PSN = info;
}

/************************************************************************
// Set/Get - Next Session Start PSN (TIME)
*************************************************************************/
void pdbSetBDNextSessStartPSN(ULONG ADDR)
{
    if ( (pdbGetBDLayers() == BD_DUAL_LAYER) && (ADDR > pdbGetBDMaxPSN(0))
        && (ADDR < pdbGetBDStartPSN(1)) )
    {
        if (ADDR & 0x1000000L)   // layer 1
            ADDR = pdbGetBDMaxPSN(0) - (pdbGetBDStartPSN(1) - ADDR) + 1L;
        else                    // layer 0
            ADDR = pdbGetBDStartPSN(1) + ADDR - (pdbGetBDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.BD.NextSessionStartPSN = ADDR;
}

/************************************************************************
// Set/Get - Final Session Leadin PSN (Time)
*************************************************************************/
void pdbSetBDFinalSessLeadinPSN(ULONG ADDR)
{
    if ( (pdbGetBDLayers() == BD_DUAL_LAYER) && (ADDR > pdbGetBDMaxPSN(0))
        && (ADDR < pdbGetBDStartPSN(1)) )
    {
        if (ADDR & 0x1000000)    // ADDR is located on layer 1
            ADDR = pdbGetBDMaxPSN(0) - (pdbGetBDStartPSN(1) - ADDR) + 1L;
        else
            ADDR = pdbGetBDStartPSN(1) + ADDR - (pdbGetBDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.BD.LeadinPSN = ADDR;
}

/*----------------------------------------------------------------------*/
ULONG pdbGetBDFinalSessLeadinLBA(void)  /* LBA */
{
    ULONG lba;

    if ((pdbGetBDLayers() == BD_DUAL_LAYER) && (Iram_FinalSessionInfo.BD.LeadinPSN & 0x1000000L))
    {
        lba = bdConvertPSNtoLBA(pdbGetBDMaxPSN(0)) + Iram_FinalSessionInfo.BD.LeadinPSN -
              pdbGetBDStartPSN(1) + 1L;
    }
    else
        lba = Iram_FinalSessionInfo.BD.LeadinPSN - pdbGetBDStartPSN(0);

    return (lba);
}

/************************************************************************
// Set/Get - Final Session Leadout PSN (Time)
*************************************************************************/
void pdbSetBDFinalSessLeadoutPSN(ULONG ADDR)
{
    if ( (pdbGetBDLayers() == BD_DUAL_LAYER) && (ADDR > pdbGetBDMaxPSN(0))
        && (ADDR < pdbGetBDStartPSN(1)) )
    {
        if (ADDR & 0x1000000L)    // ADDR is located on layer 1
            ADDR = pdbGetBDMaxPSN(0) - (pdbGetBDStartPSN(1) - ADDR) + 1L;
        else
            ADDR = pdbGetBDStartPSN(1) + ADDR - (pdbGetBDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.BD.LeadoutPSN = ADDR;
}

/*----------------------------------------------------------------------*/
ULONG pdbGetBDFinalSessLeadoutLBA(void)  /* LBA */
{
    ULONG lba;

    if ((pdbGetBDLayers() == BD_DUAL_LAYER) && (Iram_FinalSessionInfo.BD.LeadoutPSN & 0x1000000L))
    {
        lba = bdConvertPSNtoLBA(pdbGetBDMaxPSN(0)) + Iram_FinalSessionInfo.BD.LeadoutPSN -
              pdbGetBDStartPSN(1) + 1L;
    }
    else
        lba = Iram_FinalSessionInfo.BD.LeadoutPSN - pdbGetBDStartPSN(0);

    return (lba);
}

/*****************************************************************************

    FUNCTION        pdbGetBDHybridDiscId

    DESCRIPTION     Determines the HybridDiscIdentifier of the given layer

    RETURNS         HybridDiscIdentifier

    NOTES           None

*****************************************************************************/
BYTE pdbGetBDHybridDiscId(BYTE layer)
{
    if (layer == 0)
        return Info.Media.BD.DiscInfoP_L0_Fmt1->HybridDiscIdentifier;
    else
        return Info.Media.BD.DiscInfoP_L1_Fmt1->HybridDiscIdentifier;
}


///*****************************************************************************
//
//    FUNCTION        pdbGetBDSrriSessionStart
//
//    DESCRIPTION     Get Session Start bit of SRRI entry idx
//
//    RETURNS         Session Start bit
//
//    NOTES           None
//
//*****************************************************************************/
BYTE pdbGetSRRIEntrySessStart(USHORT idx)
{
    BYTE bp;

    bp =  pSRRI->SRRI_Entry[idx].LRA;

    return (bp >> 7);
}

///*****************************************************************************
//
//    FUNCTION        pdbSetBDSrriSessionStart
//
//    DESCRIPTION     Set Session Start bit of SRRI entry idx
//
//    RETURNS         None
//
//    NOTES           None
//
//*****************************************************************************/
void pdbSetSRRIEntrySessStart(USHORT idx, BYTE SStart)
{
    pSRRI->SRRI_Entry[idx].LRA |= (ULONG)(SStart << 7);
}

/*****************************************************************************

    FUNCTION        pdbGetSRRIEntryStartPSN

    DESCRIPTION     Get start data sector of first cluster in SRRI entry

    RETURNS         Recorded Start PSN

    NOTES           None

*****************************************************************************/
ULONG pdbGetSRRIEntryStartPSN(USHORT idx)
{
    ULONG PSN;

    PSN = SWAPEND_ULONG(pSRRI->SRRI_Entry[idx].StartPSN) & 0xFFFFFFFL;
    return PSN;
}

/*****************************************************************************

    FUNCTION        pdbSetSRRIEntryStartPSN

    DESCRIPTION     Set start data sector of first cluster in SRRI entry

    RETURNS         Recorded Start PSN

    NOTES           None

*****************************************************************************/
void pdbSetSRRIEntryStartPSN(USHORT idx, ULONG StartPSN)
{
    pSRRI->SRRI_Entry[idx].StartPSN = SWAPEND_ULONG(StartPSN & 0xFFFFFFFL);
}

/*****************************************************************************

    FUNCTION        pdbGetSRRIEntryLRA

    DESCRIPTION     Get last recorded data sector which was supplied by host

    RETURNS         Recorded PSN

    NOTES           None

*****************************************************************************/
ULONG pdbGetSRRIEntryLRA(ULONG idx)
{
    ULONG PSN;

    PSN = SWAPEND_ULONG(pSRRI->SRRI_Entry[idx].LRA) & 0xFFFFFFFL;
    return PSN;
}

/*****************************************************************************

    FUNCTION        pdbSetSRRIEntryLRA

    DESCRIPTION     Set last recorded data sector which was supplied by host

    RETURNS         Recorded PSN

    NOTES           None

*****************************************************************************/
void pdbSetSRRIEntryLRA(USHORT idx, ULONG LraPSN)
{
    pSRRI->SRRI_Entry[idx].LRA &= 0xF0L;
    pSRRI->SRRI_Entry[idx].LRA |= SWAPEND_ULONG(LraPSN & 0xFFFFFFFL);
}

/************************************************************************
// Get - BD - Disc Manufacturer ID (6 Bytes)
*************************************************************************/
char * pdbGetBDDiscManufacturerID(void)
{
    switch (pdbGetDiscKind())
    {
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
            return (char *)(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.BdWritable.DiscManufacturerID);

        default:
            return (char *)(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.Reserved2);
    }
}

#endif  //(BD_ENABLE)
