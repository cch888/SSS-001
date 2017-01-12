/****************************************************************************
*                (c) Copyright 2001 -  2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME      : plrDVDDB.c
*
* DESCRIPTION   : this sub system handles the tray in/out state machine.
*
*   $Revision: 154 $
*   $Date: 11/04/25 6:12p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

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
#include ".\player\plrDB.h"
#include ".\player\plrServo.h"

#include ".\dvd\dvgvars.h"
#include ".\dvd\dvdiscinit.h"
#include ".\dvd\dvutil.h"
#if (BD_READ == 1)
#include ".\bd\bdMacro.h"
#endif // (BD_READ == 1)

//-----------------------------------------------------------------------
//                   DVD PLAYER DATABASE INITIALIZATION
//                    Clears DVD Player Database Fields
//-----------------------------------------------------------------------
extern void DvdCheckBookType(eDiscKind DvdDiscKind);
/*--------------------------------------------------------------------
  Function:     pdbInitDvdDataBase()

  Description:  Initials the DVD Data Base

--------------------------------------------------------------------*/
void pdbInitDvdDataBase(void)
{
    int index;

    // -----------------------------------------------------
    //           Clear DVD DiscData Structure
    // -----------------------------------------------------

    if(LoadedDiscKind.Bit.WblType == eWblMinus)
    {

        Info.Media.DVD.BorderCount = 0x00;

        for (index = 0; index < MaxNumOfBoard ; index++)
        {
            Info.Media.DVD.BorderOutPSN[index].PSN   = 0x00;
            Info.Media.DVD.BorderOutPSN[index].Layer = 0x00;
            Info.Media.DVD.BorderInPSN[index].PSN    = 0x00;
            Info.Media.DVD.BorderInPSN[index].Layer  = 0x00;
        }
    }
    // -----------------------------------------------------
    //  Clear Physical Format Structure
    // -----------------------------------------------------
    // Info.Media.DVD.fPFInfoValid = 0;
    Info.Media.DVD.PFInfoP = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS + DVD_PHYSICAL_FORMAT_INFORMATION);
    Info.Media.DVD.PFInfoP->DiscTypeVersion  = 0;
    Info.Media.DVD.PFInfoP->DiscSize    = 0;
    Info.Media.DVD.PFInfoP->DiscStructure = 0;
    Info.Media.DVD.PFInfoP->RecordingDensity = 0;
    Info.Media.DVD.PFInfoP->StartPSN = 0;
    Info.Media.DVD.PFInfoP->MaxPSN = 0;
    Info.Media.DVD.PFInfoP->EndPSNLayer0 = 0;
    Info.Media.DVD.PFInfoP->FlagBits = 0;

    // -----------------------------------------------------
    //  Clear Static Disc Information
    // -----------------------------------------------------
    Info.Media.DVD.LeadOutStartPSN.PSN = 0L;
    Info.Media.DVD.LeadOutStartPSN.Layer = 0;
    Info.Media.DVD.LeadInStartPSN.PSN = 0L;
    Info.Media.DVD.LeadInStartPSN.Layer = 0;


    for (index = 0; index < 8; index++)
    {

        Info.Media.DVD.DiscID[index*4 + 0] = 0;
        Info.Media.DVD.DiscID[index*4 + 1] = 0;
        Info.Media.DVD.DiscID[index*4 + 2] = 0;
        Info.Media.DVD.DiscID[index*4 + 3] = 0;

    }

    // -----------------------------------------------------
    //  Clear Current Disc Structure
    // -----------------------------------------------------
    Info.Media.DVD.LowestRecordedTrackNumber = 0;
    Info.Media.DVD.LogicalTracks = 0;
    Info.Media.DVD.SessionCounter = 0;
    Info.DiscProtectionStatus = eDiscIsNotProtected;
    Info.DiscStatus           = eNonFinalizedDisc;
    Info.DiscIDStatus = 0;

    // -----------------------------------------------------
    //  Clear TOC Track Info Structure
    // -----------------------------------------------------
    for (index=0; index < 256; index++)
    {
        Info.Media.DVD.Track[index].TrackStatus = eTS_None;
        Info.Media.DVD.Track[index].TrackState = eTRACK_STATE_UNKNOWN;
        Info.Media.DVD.Track[index].TrackSessionNumber = 0;
        Info.Media.DVD.Track[index].TrackDiagStatus0 = 0;
        Info.Media.DVD.Track[index].TrackDiagStatus1 = 0;
        Info.Media.DVD.Track[index].StartPSN.PSN = 0L;
        Info.Media.DVD.Track[index].EndPSN.PSN = 0L;
        Info.Media.DVD.Track[index].RecordedPSN.PSN = 0L;
        Info.Media.DVD.Track[index].NextWritableLBA = 0L;
    }

    // -----------------------------------------------------
    //  Clear DVD Final Session Data Structure
    // -----------------------------------------------------
    Iram_FinalSessionInfo.DVD.SessionNumber = 0;
    Iram_FinalSessionInfo.DVD.FirstTrackNumber = 0;
    Iram_FinalSessionInfo.DVD.LastTrackNumber = 0;
    Iram_FinalSessionInfo.DVD.Format = 0;
    Iram_FinalSessionInfo.DVD.CloseInfo = eEmptySession;
    Iram_FinalSessionInfo.DVD.LeadinPSN = 0L;
    Iram_FinalSessionInfo.DVD.LeadoutPSN = 0L;
    Iram_FinalSessionInfo.DVD.NextSessionStartPSN = 0L;
    Iram_FinalSessionInfo.DVD.FirstTrackPSN = 0L;

    // -----------------------------------------------------
    //  Clear DVD CSS Information
    // -----------------------------------------------------
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    Info.Media.DVD.fCSS_PROTECTED = eNO_CPS;
#else
    Info.Media.DVD.fCSS_PROTECTED = 0;
#endif
    Info.Media.DVD.RegionControl = 0;

#if (ENABLE_CPRM && BCA_ENABLE)
    // -----------------------------------------------------
    //  Clear DVD CPRM Information
    // -----------------------------------------------------
    Info.Media.DVD.TotalMKBPacks = 0;
#endif

    // -----------------------------------------------------
    //  Clear DVD Logical Data
    // -----------------------------------------------------
    Info.Media.DVD.MaxLBA = 0L;
    Info.Media.DVD.EndLBALayer0 = 0L;

    // -----------------------------------------------------
    //  Clear DVD-R DL Logical Data
    // -----------------------------------------------------
    Info.Media.DVD.LayerJumpLBA= 0L;

    // -----------------------------------------------------
    //  Clear Layer Transition Flag
    // -----------------------------------------------------

    // -----------------------------------------------------
    //  Clear Middle Zones Recording Status
    // -----------------------------------------------------


    //------------------------------------------------------
    //  Clear RMD Info
    //------------------------------------------------------
    Info.Media.DVD.RMAData.SSNShiftMiddleArea = 0;
    Info.Media.DVD.RMAData.PreRecordedInfoCode = 0;
    Info.Media.DVD.RMAData.EndAddrPreRecordedLeadinArea = 0;
    Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL0 = 0;
    Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL1 = 0;
    Info.Media.DVD.RMAData.StartAddrPreRecordedLeadoutArea = 0;

    //RMD Format1-Field4
    Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword  = 0;
    Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword  = 0;
    Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword = 0;
    Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword = 0;

    //RMD Format2
    Info.Media.DVD.RMAData.RMD.Format2.DiscStatus = 0;
    Info.Media.DVD.RMAData.RMD.Format2.RBGInformation = 0;
    Info.Media.DVD.RMAData.RMD.Format2.UpdateCounter.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format2.Format3RMDSetPointer.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format2.EraseOperation.Uword = 0;
    Info.Media.DVD.RMAData.RMD.Format2.RSDS.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format2.EraseOperationCode = 0;
    Info.Media.DVD.RMAData.RMD.Format2.EraseInformation1.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format2.EraseInformation2.Long = 0;

    //RMD Format3
    Info.Media.DVD.RMAData.RMD.Format3.RBGInformation = 0;
    Info.Media.DVD.RMAData.RMD.Format3.FormatOperationCode = 0;
    Info.Media.DVD.RMAData.RMD.Format3.FormatInformation1.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.FormatInformation2.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.LastRZoneNumber.Uword = 0;
    Info.Media.DVD.RMAData.RMD.Format3.LJAOfLayer0.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.LRA.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.PreLJAOfLayer0.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.JumpInterval.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.ORAddrOfLayer0.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.ORAddrOfLayer1.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.PSNDSBRMDSet.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.CertStartPSN.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format3.CertEndPSN.Long = 0;


    //RMD Format4
    for(index = 0; index < 4; index++)
        Info.Media.DVD.APRemappingPSN[index] = 0;

    Info.Media.DVD.RMAData.RMD.Format4.SSNInvisibleRzone.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format4.LJAInvisibleRzone.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format4.ESNInvisibleRzone.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format4.LRAInvisibleRzone.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format4.PreLJAInvisibleRzone.Long = 0;
    Info.Media.DVD.RMAData.RMD.Format4.JumpInterval.Uword = 0;

    return;
}


/*----------------------------------------------------------------------*/
ULONG pdbGetDVDLeadOutStartPSN(void)
{
    StLongU TempAddress;


    if(pdbServoGetDiscType() == eADIPDisc)
    {
        if(pdbGetDiscKind() == eDVDPlusRWDisc)
        {
            if(St_12_cm == TRUE)
                return DVD_PLUSRW_DATAZONE_END + 1;
            else
                return DVD_PLUS8cm_DATAZONE_END + 1;
        }
        else
        {
            TempAddress.byte.u = 0;
            TempAddress.byte.h = ((BYTE *)(Info.Media.DVD.PFInfoP)) [ 9];      // Read data from Player Database Physical Format Information.
            TempAddress.byte.m = ((BYTE *)(Info.Media.DVD.PFInfoP)) [10];
            TempAddress.byte.l = ((BYTE *)(Info.Media.DVD.PFInfoP)) [11];
            TempAddress.Long++;
        }
    }
    else
    {
        if(pdbGetDiscKind() == eDVDMinusRDLDisc || pdbGetDiscKind() == eDVDMinusRWDLDisc)
        {
            TempAddress.Long = pdbGetDVDMaxPSN(1);
        }
        else
        {
            TempAddress.Long = pdbGetDVDMaxPSN(0);
        }
    }

    return(TempAddress.Long);
}


//-----------------------------------------------------------------------
//                       FINAL SESSION STRUCTURE
//                      Player Database Functions
//-----------------------------------------------------------------------

/************************************************************************
// Set/Get - Final Session Leadin PSN (Time)
*************************************************************************/
void pdbSetDVDFinalSessLeadinPSN(ULONG ADDR)
{
    if ( (pdbGetDVDLayers() == DUAL_LAYER) && (ADDR > pdbGetDVDMaxPSN(0))
        && (ADDR < pdbGetDVDStartPSN(1)) )
    {
        if (ADDR & 0x800000)    // ADDR is located on layer 1
            ADDR = pdbGetDVDMaxPSN(0) - (pdbGetDVDStartPSN(1) - ADDR) + 1L;
        else
            ADDR = pdbGetDVDStartPSN(1) + ADDR - (pdbGetDVDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.DVD.LeadinPSN = ADDR;
}

/*----------------------------------------------------------------------*/
long pdbGetDVDFinalSessLeadinLBA(void)  /* LBA */
{
    ULONG lba;

    if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC) &&
        (Iram_FinalSessionInfo.DVD.LeadinPSN & 0x800000L))
    {
        lba = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0)) + Iram_FinalSessionInfo.DVD.LeadinPSN -
              pdbGetDVDStartPSN(1) + 1L;
    }
    else
        lba = Iram_FinalSessionInfo.DVD.LeadinPSN - 0x30000L;

    return (lba);
}

/************************************************************************
// Set/Get - Final Session Leadout PSN (Time)
*************************************************************************/
void pdbSetDVDFinalSessLeadoutPSN(ULONG ADDR)
{
    if ( (pdbGetDVDLayers() == DUAL_LAYER) && (ADDR > pdbGetDVDMaxPSN(0))
        && (ADDR < pdbGetDVDStartPSN(1)) )
    {
        if (ADDR & 0x800000L)    // ADDR is located on layer 1
            ADDR = pdbGetDVDMaxPSN(0) - (pdbGetDVDStartPSN(1) - ADDR) + 1L;
        else
            ADDR = pdbGetDVDStartPSN(1) + ADDR - (pdbGetDVDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.DVD.LeadoutPSN = ADDR;
}

/*----------------------------------------------------------------------*/
long pdbGetDVDFinalSessLeadoutLBA(void)  /* LBA */
{
    ULONG lba;

    if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC) &&
        (Iram_FinalSessionInfo.DVD.LeadoutPSN & 0x800000L))
    {
        lba = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0)) + Iram_FinalSessionInfo.DVD.LeadoutPSN -
              pdbGetDVDStartPSN(1) + 1L;
    }
    else if (pdbGetDVDLayers() == DUAL_LAYER && pdbGetDVDTrackPath() == DVDHD_PTP_DISC)
    {
        lba = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0)) + Iram_FinalSessionInfo.DVD.LeadoutPSN -
              pdbGetDVDStartPSN(1) + 1L;
    }
    else
        lba = Iram_FinalSessionInfo.DVD.LeadoutPSN - 0x30000L;

    return (lba);
}

//-----------------------------------------------------------------------
//                          SESSION STRUCTURE
//                      Player Database Functions
//-----------------------------------------------------------------------

/************************************************************************
// Set/Get - Next Session Start PSN (TIME)
*************************************************************************/
void pdbSetDVDNextSessStartPSN(ULONG ADDR)
{
    if ( (pdbGetDVDLayers() == DUAL_LAYER) && (ADDR > pdbGetDVDMaxPSN(0))
        && (ADDR < pdbGetDVDStartPSN(1)) )
    {
        if (ADDR & 0x800000L)   // layer 1
            ADDR = pdbGetDVDMaxPSN(0) - (pdbGetDVDStartPSN(1) - ADDR) + 1L;
        else                    // layer 0
            ADDR = pdbGetDVDStartPSN(1) + ADDR - (pdbGetDVDMaxPSN(0) + 1L);
    }
    Iram_FinalSessionInfo.DVD.NextSessionStartPSN = ADDR;
}


//-----------------------------------------------------------------------
//                                ADIP
//                      Player Database Functions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//                          TRACK INFORMATION
//                      Player Database Functions
//-----------------------------------------------------------------------

/************************************************************************
// Set/Get - DVD Track Recorded PSN
*************************************************************************/
void pdbSetDVDTrackRecordedPSN(BYTE Tno, ULONG info)
{
    if ( (pdbGetDVDLayers() == DUAL_LAYER) && (info > pdbGetDVDMaxPSN(0))
        && (info < pdbGetDVDStartPSN(1)) )
    {
        if (info & 0x800000)    // ADDR is located on layer 1
            info = pdbGetDVDMaxPSN(0) - (pdbGetDVDStartPSN(1) - info) + 1L;
        else
            info = pdbGetDVDStartPSN(1) + info - (pdbGetDVDMaxPSN(0) + 1L);
    }
    Info.Media.DVD.Track[Tno-1].RecordedPSN.PSN = info;
}

//-----------------------------------------------------------------------
//                       MISCELLANEOUS STRUCTURE
//                      Player Database Functions
//-----------------------------------------------------------------------

BYTE pdbGetDVDRW(void)
{
    switch(pdbGetDiscKind())
    {
        case eDVDRAMDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRWDLDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRWDLDisc:
            return(TRUE);
    }

    return(FALSE);
}

/*******************************************************************************

    FUNCTION        pdbSetDVDMaxPSN

    DESCRIPTION     Set the maximum PSN of the given layer.

    NOTES           Insure PFInfo variables are saved in big endian format

********************************************************************************/
void pdbSetDVDMaxPSN(BYTE layer, ULONG PSN)
{

    if( (pdbGetDVDTrackPath() == DVDHD_OTP_DISC) && (layer == 0) )
        Info.Media.DVD.PFInfoP->EndPSNLayer0 = SWAPEND_ULONG(PSN);

    else if( (pdbGetDVDTrackPath() == DVDHD_PTP_DISC) && (layer == 1) )
        Info.Media.DVD.PTPMaxPSNLayer1 = PSN;

    else
        Info.Media.DVD.PFInfoP->MaxPSN = SWAPEND_ULONG(PSN);
}

/*******************************************************************************

    FUNCTION        pdbGetDVDMaxPSN

    DESCRIPTION     Determines the maximum PSN of the given layer

    RETURNS         MaxPSN

    NOTES           Save in big endian format for PFInfo variables

********************************************************************************/
ULONG pdbGetDVDMaxPSN(BYTE layer)
{
    {
        // For OTP layer 0
        if( ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)) && (layer == 0) )
            return SWAPEND_ULONG(Info.Media.DVD.PFInfoP->EndPSNLayer0);

        // For PTP layer 1
        else if( ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC)) && (layer == 1) )
            return Info.Media.DVD.PTPMaxPSNLayer1;

        // For all other cases
        else
            return (SWAPEND_ULONG(Info.Media.DVD.PFInfoP->MaxPSN)) ;
    }
}

/*******************************************************************************

    FUNCTION        pdbGetDVDStartPSN

    PARAMETERS      layer

    RETURNS         Starting PSN of the given layer

********************************************************************************/
ULONG pdbGetDVDStartPSN(BYTE layer)
{

    if( (pdbGetDVDTrackPath() == DVDHD_PTP_DISC) || (layer == 0) )
        return SWAPEND_ULONG(Info.Media.DVD.PFInfoP->StartPSN);   // For all layer 0 and PTP discs
    else
        return (~(SWAPEND_ULONG(Info.Media.DVD.PFInfoP->EndPSNLayer0)))  & 0x00FFFFFF; // For 2nd layer of OTP discs

}

/*******************************************************************************

    FUNCTION        pdbSetDVDPhysicalFormatInfo

    DESCRIPTION     Updates the DVD physical format information structure.

    PARAMETERS      data    Pointer to the physical format data. The data sequence
                            must comply with the DVD specification for the physical
                            format information structure.


    GLOBAL UPDATES  PFInfo structure for DVD

********************************************************************************/
void pdbSetDVDPhysicalFormatInfo(BYTE *data)
{
    USHORT  BufferPage;


    // caculate page number of the bufferred physical format information sector.
    BufferPage = (USHORT)(((ULONG)data - SCSDRAM_ADDRESS) / DRAM_PAGE_SIZE);

    // Copy the Physical Format Information sector to permanent player db area in SDRAM
    CopyBufferToBuffer(BufferPage, 0, DVD_PHYSICAL_FORMAT_INFORMATION_PAGE, 0, 1);

    DvdCheckBookType(pdbGetDiscKind());
}

/*******************************************************************************

    FUNCTION        pdbSetDVDManufactureInfo

    DESCRIPTION     Updates the DVD Manufacturering information structure.

    PARAMETERS      data    Pointer to the physical format data. The data sequence
                            must comply with the DVD specification for the physical
                            format information structure.


    GLOBAL UPDATES  PFInfo structure for DVD

********************************************************************************/
void pdbSetDVDManufactureInfo(BYTE *data)
{
    USHORT  BufferPage;

    BufferPage = (USHORT)(((ULONG)data - SCSDRAM_ADDRESS) / DRAM_PAGE_SIZE);

    // Copy the Manufacturering information sector to permanent player db area in SDRAM
    CopyBufferToBuffer(BufferPage, 0, DVD_DISC_MANUFACTURE_INFORMATION_PAGE, 0, 1);

}

/*******************************************************************************

    FUNCTION        pdbSetDVDLogicalFormatInfo

    DESCRIPTION     Updates the DVD physical format information structure.

    PARAMETERS      data    Pointer to the physical format data. The data sequence
                            must comply with the DVD specification for the physical
                            format information structure.

                    layer   Only used for dual layer PTP discs for getting the
                            ending PSN of the second layer

    GLOBAL UPDATES  PFInfo structure for DVD

********************************************************************************/
void pdbSetDVDLogicalFormatInfo(BYTE *data)
{
    USHORT  BufferPage;

    // caculate page number of the bufferred physical format information sector.
    BufferPage = (USHORT)(((ULONG)data - SCSDRAM_ADDRESS) / DRAM_PAGE_SIZE);

    // Copy the Physical Format Information sector to permanent player db area in SDRAM
    CopyBufferToBuffer(BufferPage, 0, DVD_LOGICAL_FORMAT_INFORMATION_PAGE, 0, 1);

    DvdCheckBookType(pdbGetDiscKind());
    return;
}

/*******************************************************************************

    FUNCTION        pdbSetDVDLogicalData

    DESCRIPTION     Sets the discs logical data based on the physical format information

********************************************************************************/
void pdbSetDVDLogicalData()
{

    ULONG *sdramAddress;

   /*---------------------------------------------------------------
        Update Maximum LBA and last LBA of layer
    ----------------------------------------------------------------*/
    if (pdbGetDVDLayers() == SINGLE_LAYER)
    {
        // Calculate Maximum LBA
        switch(pdbGetDVDType())
        {
            case DVD_ROM:
            case DVD_RAM:
            case DVD_MINUS_R:
                pdbSetDVDMaxLBA(pdbGetDVDMaxPSN(0) - pdbGetDVDStartPSN(0));
                break;

            case DVD_PLUS_RW:
                pdbSetDVDMaxLBA(dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(1)));  //track 1
                break;

            case DVD_MINUS_RW:
            case DVD_PLUS_R:
                pdbSetDVDMaxLBA(dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber())));
                break;

        }

        // Set last LBA of layer 0
        pdbSetDVDEndLBALayer0(pdbGetDVDMaxLBA());
    }
    else // DUAL_LAYER
    {
        // Set last LBA of layer 0
        pdbSetDVDEndLBALayer0(pdbGetDVDMaxPSN(0) - pdbGetDVDStartPSN(0));

        // Set MaxLBA
        if (pdbGetDVDTrackPath() == DVDHD_PTP_DISC)
        {
            pdbSetDVDMaxLBA(pdbGetDVDEndLBALayer0() + 1L + (pdbGetDVDMaxPSN(1) - pdbGetDVDStartPSN(1)));
        }
        else // OTP DISC
        {
            if (Info.Media.DVD.PFInfoP->MaxPSN == Info.Media.DVD.PFInfoP->EndPSNLayer0) //Layer1 empty
            {
                pdbSetDVDMaxLBA(pdbGetDVDEndLBALayer0());
            }
            else
            {
                pdbSetDVDMaxLBA(pdbGetDVDEndLBALayer0() + 1L + (pdbGetDVDMaxPSN(1) - pdbGetDVDStartPSN(1)));
            }
        }
    }

    /*---------------------------------------------------------------
        Update MAXLBA information in SDRAM
    ----------------------------------------------------------------*/
    sdramAddress     = (ULONG *) (SCSDRAM_START_ADDR + MAXLBA_OFFSET);
    *sdramAddress    = SWAPEND_ULONG(pdbGetDVDMaxLBA());

}

/*******************************************************************************

    FUNCTION        pdbGetDVDClosedSessions

    DESCRIPTION     Determines the number of closed sessions

    RETURNS         the number of closed sessions on the mounted disc

********************************************************************************/
BYTE pdbGetDVDClosedSessions(void)
{
    BYTE LastSessionStatus;

    LastSessionStatus = pdbGetDVDFinalSessCloseInfo();

    if (((LastSessionStatus == eEmptySession) || (LastSessionStatus == eNotSessionClosed))
        && (Info.Media.DVD.SessionCounter != 0))
        return(Info.Media.DVD.SessionCounter - 1);
    else
        return(Info.Media.DVD.SessionCounter);
}

/*******************************************************************************

    FUNCTION        pdbGetRAIPosition

    DESCRIPTION        Returns Address of the Recorded Area Indicators
                       that corresponds with the Last Written PSN on a DVD+R
    PARAMETERS         LastWrittenPSN       on inserted DVD+R
    GLOBALS            none

    RETURNS            Address of the Recorded Area Indicators that needs to be marked.


    NOTE           for calculations please refer to DVD+R Specifications

********************************************************************************/
//ULONG pdbGetRAIPosition(ULONG LastWrittenPSN)
//{
//    ULONG RAIPSN, RAICount, Length;
//
//    if(LastWrittenPSN < 0x30000)
//    {
//        if (pdbGetDVDLayers() == SINGLE_LAYER)
//            RAIPSN = 0x2A480;
//        else
//            RAIPSN = 0x2AD00;
//        return(RAIPSN);
//    }
//
//    if (pdbGetDVDLayers() == SINGLE_LAYER)
//    {
//        RAICount = (LastWrittenPSN - 0x30000) / 0x2800 + 1;
//        Length = RAICount * 4;
//
//        RAIPSN = 0x2A480 - Length;
//    }
//    else
//    {
//        if (LastWrittenPSN < 0x30000)
//            RAICount = 0;
//        else if (LastWrittenPSN >= 0x30000 && LastWrittenPSN <= 0x2AFFFF)
//            RAICount = (LastWrittenPSN - 0x30000) / 0x2800 + 1;
//        else if (LastWrittenPSN >= 0xD50000 && LastWrittenPSN <= 0xFCFFFF)
//            RAICount = (LastWrittenPSN - 0xD50000) / 0x2800 + 1 + 256;
//        Length = RAICount * 4;
//        RAIPSN = 0x2AD00 - Length;
//    }
//
//    return(RAIPSN);
//}

/************************************************************************
// Get - DVD Blank Disc Info
*************************************************************************/
BOOL pdbGetDVDBlankDiscInfo(void)
{
    if (pdbGetDiscKind() == eDVDMinusRWDLDisc)
    {
        if (pdbGetDVDRMADiscStatus() == EmptyDisc)
            return (TRUE);
        else
            return (FALSE);
    }
    else
    {
        if((pdbGetDVDFinalSessCloseInfo() == eEmptySession) && (pdbGetDVDFinalSessFirstTrackNumber() == 1))
            return (TRUE);
        else
            return (FALSE);
    }
}

/************************************************************************
// Set/Get - DVD Track End PSN
*************************************************************************/
ULONG pdbGetDVDBorderZoneSize(BYTE BorderNum, ULONG info)
{
    ULONG BorderZoneSize ;

    if(pdbGetDiscKind() == eDVDMinusRDLDisc)
    {
        if ((info >= 0x3FF00)&&(info <= 0xB25FF))
            BorderZoneSize = 29504 ;
        else if ((info >= 0xB2600)&&(info <= 0x1656FF))
            BorderZoneSize = 39072 ;
        else if (info >= 0x165700)
            BorderZoneSize = 47552 ;
        else
            BorderZoneSize = 29504 ;
    }
    else
    {
        if (BorderNum == 1)
        {
            if ((info >= 0x3FF00)&&(info <= 0xB25FF))
                BorderZoneSize = 28672 ;
            else if ((info >= 0xB2600)&&(info <= 0x1656FF))
                BorderZoneSize = 37888 ;
            else if (info >= 0x165700)
                BorderZoneSize = 47104 ;
            else
                BorderZoneSize = 28672 ;
        }
        else
        {
            if ((info >= 0x3FF00)&&(info <= 0xB25FF))
                BorderZoneSize = 6144 ;
            else if ((info >= 0xB2600)&&(info <= 0x1656FF))
                BorderZoneSize = 7680 ;
            else if (info >= 0x165700)
                BorderZoneSize = 9728 ;
            else
                BorderZoneSize = 6144 ;
        }
    }
    return BorderZoneSize;
}

#if GET_LAST_TRACK_INFO_BASED_ON_RAI
/*******************************************************************************

    FUNCTION        pdbGetLastRecordedRegion

    DESCRIPTION        Returns Start Address of the region of inserted DVD+R
                               where  the last writtenECC Block can be found.
    PARAMETERS         none
    GLOBALS            Info.Media.DVD.LastRAIPSN

    RETURNS                     Start Address of the region of inserted DVD+R
                                where  the last writtenECC Block can be found.

    NOTE           for calculations please refer to DVD+R Specifications

********************************************************************************/
ULONG pdbGetLastRecordedRegion(void)
{
    if (pdbGetDVDLayers() == SINGLE_LAYER)
        return (((0x2A47C - Info.Media.DVD.LastRAIPSN) * 0xA00) + 0x30000);
    else
    {
        if (Info.Media.DVD.LastRAIPSN >= 0x2A900)
            return (((0x2ACFC - Info.Media.DVD.LastRAIPSN) * 0xA00) + 0x30000);
        else
            return (((0x2A8FC - Info.Media.DVD.LastRAIPSN) * 0xA00) + 0xD50000);
    }
}
#endif  //GET_LAST_TRACK_INFO_BASED_ON_RAI

/************************************************************************
// Get - DVD Lead-In Start PSN
*************************************************************************/
ULONG pdbGetDVDLeadInStartPSN(void)
{

    switch(pdbGetDiscKind())
    {
        case eDVDMinusRDisc:
            if(pdbDVDDiscNBCA())
                return(DVDMINUSR_NBCA_LEADIN_START_ADDRESS);
            else
                return(DVDMINUSR_LEADIN_START_ADDRESS);
        case eDVDMinusRWDisc:
            if(pdbDVDDiscNBCA())
                return(DVDMINUSR_NBCA_LEADIN_START_ADDRESS);
            else
                return(DVDMINUSR_LEADIN_START_ADDRESS);
        case eDVDMinusRDLDisc:
            if(pdbDVDDiscNBCA())
                return(DVDMINUSR_DL_NBCA_LEADIN_START_ADDRESS);
            else
                return(DVDMINUSR_DL_LEADIN_START_ADDRESS);
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
            return(DVDPLUSR_LEADIN_START_ADDRESS);

        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            return(DVDPLUSR_DL_LEADIN_START_ADDRESS);

        case eDVDRAMDisc:
            return(DVDRAM_LEADIN_START_ADDRESS);

        default:
            send_msg80(1,0x710130);
            return(0x2A480);
    }
}
