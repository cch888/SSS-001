/****************************************************************************
*                (c) Copyright 2000 - 2006 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
*   FILE NAME:     $Workfile: dvUtil.c $
*
* DESCRIPTION
*
* Contains
*
*   $Revision: 40 $
*   $Date: 09/06/04 11:35a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
 INCLUDES
*********************************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\hif\gp_ram.h"
#include ".\hif\bf_util.h"
#include ".\dvd\dvGvars.h"
#include ".\DVD\dvBufMgr.h"
#include ".\DVD\dvMMC.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvutil.h"
#include ".\dvd\dvdef.h"
#include ".\player\plrdb.h"
#include ".\oem\oem_spd_tbl.h"

/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/

/*********************************************************************
   FUNCTIONS
*********************************************************************/

/*******************************************************************************

    FUNCTION        dvdConvertPSNtoLBA

    DESCRIPTION     This function converts a Physical Sector Number (PSN)to a
                    Logical Block Address (LBA).

    PARAMETERS      PSN

    RETURNS         LBA

    NOTES           This function don't apply to DVD PTP dual layer discs.
                    Sometimes we may only have the PSN value when this function
                    is called.
                    Ex. TrackStopLBA = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(tno));
                    For a PTP disc, we can't make sure the PSN is on layer 0 or
                    layer 1 in this case. Therefore, additional modifications are
                    required after this function is called for a DVD PTP disc.

********************************************************************************/
LONG dvdConvertPSNtoLBA(ULONG PSN)
{
    PSN &= 0x00FFFFFFL;

    if (PSN < 0x30000)
        return (0);
    else
    {
        if (pdbGetDVDLayers() == DUAL_LAYER && pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
        {
            if (PSN <= pdbGetDVDMaxPSN(0))
                return (PSN - pdbGetDVDStartPSN(0));
            else if (PSN >= pdbGetDVDStartPSN(1))
                return (pdbGetDVDEndLBALayer0() + PSN - pdbGetDVDStartPSN(1) + 1);
            else
                return (pdbGetDVDEndLBALayer0());
        }
        else
            return (PSN - pdbGetDVDStartPSN(0));
    }
}

/*******************************************************************************

    FUNCTION        dvdConvertLBAtoPSN

    DESCRIPTION     This function converts a Logical Block Address (LBA) to a
                    Physical Sector Number (PSN).

    PARAMETERS      lba          - Logical block address to be translated

    RETURNS         Target.PSN    - PSN that was translated from lba
                    Target.Layer  - returned in parameter

********************************************************************************/
PSNs  dvdConvertLBAtoPSN(LONG LBA)

{
    PSNs Target;
    ULONG OffsetInLayer1;

    if (pdbGetDVDLayers() == DUAL_LAYER)
    {
        if (LBA <= (LONG) pdbGetDVDEndLBALayer0())
        {
            /* Target is in Layer 0 */
            Target.Layer  = eLayer0;
            Target.PSN   = LBA + pdbGetDVDStartPSN(0);
        }
        else
        {
            /* Target is in Layer 1 */
            Target.Layer  = eLayer1;

            /*-----------------------------------------------------
                Calculate the offset into layer 1
            ------------------------------------------------------*/
            OffsetInLayer1  = LBA - (pdbGetDVDEndLBALayer0() + 1L);
            Target.PSN  = pdbGetDVDStartPSN(1) + OffsetInLayer1;
        }
    }
    else
    {
        Target.Layer = eLayer0;
		SendMsg5SValue(1, "LBAtoPSN", 4, LBA);
		SendMsg5SValue(1, "LBAtoPSN", 4, pdbGetDVDStartPSN(0));
        Target.PSN   = LBA + 0x30000;//[S07]pdbGetDVDStartPSN(0);//[J50Q]Modify U3 DVD not Ready.
    }

    return (Target);
}

/*******************************************************************************

    FUNCTION        dvdGetBlockAddress

    DESCRIPTION     Returns the DVD block address that the given LBA is within.
                    With dual layer discs and DVD RAM discs block addresses on
                    either the second layer or separate zones may not start at
                    an even block address. (The last nibble of the block address
                    may not be 0)

    PARAMETERS      LBA - Logical sector address

    RETURNS         The DVD block address that LBA is within.

********************************************************************************/
ULONG dvdGetBlockAddress( ULONG lba )
{
    ULONG   StartLBAofLayer1;   /* Starting lba of layer 1                      */
    ULONG   dvdBlockAddress;    /* Block address of the block containing lba    */

    /*-------------------------------------------------------------------
        For PTP discs, DVD logical block addresses on the 2nd layer may
        not start on a modulo 16 boundary. For the given LBA, caculate
        the logical block address of the DVD  block for these cases, else
        for single layer, or dual layer OTP with lba's
        within the first layer, just mask off the lower nibble of the lba
        to obtain the DVD logical block address that the given lba is within
     --------------------------------------------------------------------*/
    if( (pdbGetDVDTrackPath() == DVDHD_PTP_DISC) && (lba > pdbGetDVDEndLBALayer0()) )
    {
        StartLBAofLayer1 = pdbGetDVDEndLBALayer0() + 1L;
        dvdBlockAddress = StartLBAofLayer1 + (( lba - StartLBAofLayer1 ) & DVD_BLOCK_ADDRESS_MASK);
    }
    else
        dvdBlockAddress = lba & DVD_BLOCK_ADDRESS_MASK;

    return dvdBlockAddress;

}

#if PRN_DVD_DATABASE == 1
//******************************************************
//  Debug only
//******************************************************

void Pause(void);

/*--------------------------------------------------------------------
  Function:     DisplayDvdDataBase()

  Description:  Function sends contents of DVD Database to Serial port.
                  Function is used for debug only.
  Returns:      void

  Notes:
--------------------------------------------------------------------*/
extern vBYTE PSN_display;
void DisplayDvdDataBase()
{
    BYTE LastTrackNumber;
    BYTE Index;

    PSN_display = 0;
    LastTrackNumber = pdbGetDVDFinalSessLastTrackNumber();

    Pause();
    for (Index = 1; Index <= (LastTrackNumber +2) ; Index++)
    {
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Number             =", 1, Index);
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Cont(rol) Address  =", 1, pdbGetDVDTrackContAddr( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Con Addr. (nibble) =", 1, pdbGetDVDTrackConAddr( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Status             =", 1, pdbGetDVDTrackStatus( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track State              =", 1, pdbGetDVDTrackState( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Diag Status        =", 1, pdbGetDVDTrackDiagStatus( Index ));
        Pause();
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Diag Status 2      =", 1, pdbGetDVDTrackDiagStatus2( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Session Number     =", 1, pdbGetDVDTrackSessionNumber( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Start PSN          =", 4, pdbGetDVDTrackStartPSN( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Start PSN Layer    =", 4, pdbGetDVDTrackStartPSNLayer( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track End PSN            =", 4, pdbGetDVDTrackEndPSN( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track End PSN Layer      =", 4, pdbGetDVDTrackEndPSNLayer( Index ));
        Pause();
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Recorded PSN       =", 4, pdbGetDVDTrackRecordedPSN( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Recorded PSN Layer =", 4, pdbGetDVDTrackRecordedPSNLayer( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Next Writable LBA  =", 4, pdbGetDVDTrackNextWritableAddress( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Write Method       =", 1, pdbGetDVDTrackWriteMethod( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Packet Size        =", 4, pdbGetDVDTrackPacketSize( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Data Mode          =", 1, pdbGetDVDTrackDataMode( Index ));
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Track Data Form          =", 1, pdbGetDVDTrackDataForm( Index ));
        DVD_MESSAGE (PRN_DVD_DATABASE, ".........................................");
        Pause();
    }
        DVD_MESSAGE (PRN_DVD_DATABASE, "Session Information ");
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Session Close Info =", 1, pdbGetDVDFinalSessCloseInfo());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Session Number     =", 1, pdbGetDVDFinalSessionNumber());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Leadin PSN    =", 4, pdbGetDVDFinalSessLeadinPSN());
        Pause();
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Leadin LBA    =", 4, pdbGetDVDFinalSessLeadinLBA());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Leadout PSN   =", 4, pdbGetDVDFinalSessLeadoutPSN());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Leadout LBA   =", 4, pdbGetDVDFinalSessLeadoutLBA());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess First Track # =", 2, pdbGetDVDFinalSessFirstTrackNumber());
        Pause();
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Last  Track # =", 2, pdbGetDVDFinalSessLastTrackNumber());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess 1. Track PSN  =", 4, pdbGetDVDFinalSessFirstTrackPSN());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Final Sess Format        =", 1, pdbGetDVDFinalSessFormat());
        Pause();
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Logical Track Count      =", 1, pdbGetDVDHighestRecordedTrackNumber());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "SDCB Count               =", 1, pdbGetDVDSDCBCount());
        DVD_MESSAGE1(PRN_DVD_DATABASE, "Number of RAI entries    =", 2,( (0x2A480 - pdbGetDVDLastRAIPSN() ) / 4 ) );

        DVD_MESSAGE (PRN_DVD_DATABASE, "******************************");
        DVD_MESSAGE (PRN_DVD_DATABASE, "* End of Session Information *");
        DVD_MESSAGE (PRN_DVD_DATABASE, "******************************");
        Pause();

        PSN_display = 1;
}


/*--------------------------------------------------------------------
  Function:     Pause()

  Description:  A function to create a small delay used to assure
                  proper output of debug messages to serial port.
  Returns:      void

  Notes:        This is not a general purpos function.
                It is intended to be used only from function DisplayDvdDatabase()
--------------------------------------------------------------------*/
void Pause()
{
    DelaymS(1);
}
#endif


/****************************************************************************/
/*  DVDReturnMediaOffsetForRead                                             */
/* Description: Determines the row offset into the DVDSelSpeed_tbl_R table  */
/* based on the media type in the drive.                                    */
/*                                                                          */
/* Input: None                                                              */
/* Return: Row Offset                                                       */
/****************************************************************************/

BYTE DVDReturnMediaOffsetForRead(void)
{
BYTE    media;

    /*** Determine offset (row) based on the media type into theDVDSelSpeed_tbl_R table ***/
    switch(svoVar_discKindT.fDiscKind)
    {
    case eDVDROMDisc:
        media = eDVDROMSL_R;        /* Set DVD-ROM SL disc */
        break;
    case eDVDROMDLDisc:
        media = eDVDROMDL_R;       /* Set DVD-ROM DL disc */
        break;

    case eDVDMinusRDisc:
    case eDVDPlusRDisc:
        media = eDVDRSL_R;          /* Set DVD +/-R SL Closed Session disc */
        break;

    case eDVDMinusRWDisc:
    case eDVDPlusRWDisc:
    case eDVDMinusRWDLDisc:
    case eDVDPlusRWDLDisc:
        media = eDVDRWSL_R;         /* Set DVD+/-RW SL/DL Closed Session disc */
        break;

    case eDVDMinusRDLDisc:
    case eDVDPlusRDLDisc:
        media = eDVDRDL_R;          /* Set DVD+R DL Closed Session disc */
        break;

    case eDVDRAMDisc:
        media = eDVDRAM_R;          /* Set DVD-RAM disc */
        break;

    default:
        media = eDVDROMSL_R;        /* Set Default as DVD-ROM SL disc */
        break;
    }
    return(media);
}

#if PRINT_DVD_DATABASE
void PrintDvdDataBase()
{
    BYTE LastTrackNumber;
    BYTE Index;

    LastTrackNumber = pdbGetDVDFinalSessLastTrackNumber();
    for (Index = 1; Index <= (LastTrackNumber +2) ; Index++)
    {
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"TrkNum",1,Index);
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"Status",1,pdbGetDVDTrackStatus( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"State",1,pdbGetDVDTrackState( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"DiagStatus",1,pdbGetDVDTrackDiagStatus( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"DiagStatus2",1,pdbGetDVDTrackDiagStatus2( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"TrkSessNum",1,pdbGetDVDTrackSessionNumber( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"StartPSN",4,pdbGetDVDTrackStartPSN( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"EndPSN",4,pdbGetDVDTrackEndPSN( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"RecdPSN",4,pdbGetDVDTrackRecordedPSN( Index ));
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"TrkNWA",4,pdbGetDVDTrackNextWritableAddress( Index ));
        send_msg5S(SEND_DVD_MINUS_WR_MSG,"==========");
    }
}
#endif
