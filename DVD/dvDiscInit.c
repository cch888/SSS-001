/****************************************************************************
*                (c) Copyright 2000 - 2008, SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* $Revision: 405 $
* $Date: 11/04/08 5:52p $
*
* DESCRIPTION     Contains functions to search and retrieve disc information
*                 at initialization for the currently loaded disc
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
 INCLUDES
*********************************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_audio.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvseek.h"
#include ".\DVD\dvBufMgr.h"
#include ".\DVD\dvdiscinit.h"
#include ".\DVD\dvMMC.h"
#include ".\dvd\dvutil.h"
#include ".\dvd\dvmacros.h"
#include ".\player\plrdb.h"
#if (DVD_PLUSRW_BGF == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif
#include ".\AL\REG_HAL.h"
#include ".\AL\HAL_pll.h"           // need for PLL_EFM_AND_WOBBLE_LOCK
#include ".\al\reg_intctrl.h"
#include ".\al\reg_global.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\al\reg_host.h"
#include ".\al\reg_cd_decoder.h"

#include ".\servo\svoVar.h"
#include ".\al\REG_dfe.h"
#include ".\AL\HAL_dfe.h"
#include ".\AL\HAL_pll.h"           // need for PLL_EFM_AND_WOBBLE_LOCK
#include ".\al\reg_intctrl.h"

#if (ENABLE_AACS == 1)
#include ".\hif\flash.h"
#include ".\crypt\aacs.h"
#endif

/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/
void dvdGetBorderInfo(void);
void dvdSetCSSInfo(USHORT);

ULONG dvdGetCurrentPSN(BYTE * DVDDecoder_ID);
dvdAreaTypeE dvdGetAreaType(void);

//-------------------- DVD-R/-RW --------------------------------------

BYTE dvdSaveFormat1RMDInformation(ULONG*);
BYTE dvdSaveFormat2RMDInformation(ULONG*);
BYTE dvdSaveFormat3RMDInformation(ULONG*);
BYTE dvdSaveFormat4RMDInformation(ULONG*);
void dvdDetectTrackInfo(BYTE track);
BOOL dvdCheckRMAFormat3(void);
/*********************************************************************
   MODULE VARIABLES
*********************************************************************/
#if (ENABLE_AACS == 1)
const MKBInfoAddrS DVDROM_MKB_Info[] =
{
    {0x02F300, 2},
    {0x02F580, 2},
    {0x02F800, 2},
    {0x02FA80, 2}
};

BOOL fReadingMid;
BYTE gMidKeyIndex;

extern ULONG MkbVersionOnDisc;
extern ULONG MkbVersionOnFlash;
#endif

extern BYTE gDVDReAdjustOK;

/*********************************************************************
   FUNCTIONS
*********************************************************************/
/*******************************************************************************

    FUNCTION        dvdReadControlDataZone

    DESCRIPTION     This function reads the lead in area of the DVD disc into
                    the buffer, copies the LeadIn data to a permanent place,
                    and updates global variable structures.

                    Single layer and dual layer opposite track (OTP) discs have only
                    one lead in area to be read which is located on layer 0.

                    Dual layer parallel track (PTP) discs have two lead in areas. One
                    on layer 0 and one on layer 1. Both must be read for this kind
                    of disc.

                    This function reads the lead in area on layer 0 for all
                    three types of discs. Then if this is a dual layer parallel track
                    (PTP) disc, the lead in for layer 1 will be read.


    RETURNS         DiscAccessStatus

************************************************************************************/
DiscAccessStatusE dvdReadControlDataZone(BYTE TOC_ReadFail)
{
    DiscAccessStatusE DiscReadStatus;
    ULONG ControlBlockAddress, PreNextBorderInPSN, *pPsnAddress;
    BYTE *DataPtr, DVDSessionNum;
    dvdAreaTypeE    LeadOutData;

    BYTE DVDFinalized;
    BYTE layer;
    StLongU temp;
#if (VCPS_TEST == 1)
    BYTE FlagByte;
#endif  /* VCPS_TEST == 1 */

    send_msg80(SEND_DVD_MINUS_WR_MSG|SEND_SStpTask_MSG, 0x710104);

    /* Set up multi session parameters */
    DVDSessionNum = 0;
    DVDFinalized = FALSE;
    layer = 0;
    PreNextBorderInPSN = 0;          /* use for check nextr border-in psn */
    pdbSetDVDNextBorderInPSN(0);     /* for DVD-R */


    //-------------------------------------------------------------
    // Stop bufferring and clear data buffer
    //-------------------------------------------------------------
    if( DVD_BUFFERING() == ON)
        dvdStopDiscTransfer();

    ClearDataBuffer();

    /*  Lead-in data has been buffered. */
    DVDSessionNum = 1;

    if((LoadedDiscKind.Bit.WblType != eWblMinus) && (pdbGetDiscKind() != eDVDRAMDisc))
    {
        /* Get the absolute address of the control data block */
        DataPtr = (BYTE *)(SCSDRAM_ADDRESS + DVD_PHYSICAL_FORMAT_INFORMATION);

        if ((TOC_ReadFail == FALSE) && ((pdbGetDiscKind() == eDVDPlusRDLDisc) || (pdbGetDiscKind() == eDVDPlusRDisc)))
        {
            if ((pdbGetDiscStatus() == eFinalizedDisc) && (pdbGetDVDClosedSessions() > 1))
            {
                *((ULONG *)(DataPtr + END_PSN_OF_DATA_AREA_OFFSET)) = SWAPEND_ULONG(pdbGetDVDMaxPSN(1));
                *((ULONG *)(DataPtr + END_PSN_LAYER0_OFFSET))       = SWAPEND_ULONG(pdbGetDVDMaxPSN(0));
            }
        }

#if (VCPS_TEST == 1)
        if (VcpsKeysValid == TRUE && pdbGetDVDRW() == FALSE) /* For DVD+RW, we only refer to VCPS bit in PFI in ADIP */
        {
            FlagByte = *(DataPtr + DESCRIPTOR_FLAGS_OFFSET);
            VCPS_disc = (FlagByte & VCPS_BIT ? TRUE : FALSE);

            if (VCPS_disc == TRUE)
            {
                vcpsData.DKBLocation = DKB_BUFFER_ZONE2;
                if ((FlagByte & VCPS_IN_BZ2_BIT) == 0x00)
                {
                    if (dvdReadDKBHeader() == FAIL) /* Check if BZ2 contains VCPS information */
                    {
                        vcpsData.DKBLocation = DKB_UNDETERMINED;
                    }
                }
            }
        }
#endif  /* VCPS_TEST == 1 */
    }

    svoVar_getDVDFormatflag = 1;

    if(((pdbGetDVDType() == DVD_ROM) && (pdbGetDVDLayers() == SINGLE_LAYER)) ||
        ((pdbGetDVDType() == DVD_ROM) && (pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)))
    {
        dvdSetCSSInfo(READ_FIELD(HAL_DVDD_CURECC));     /* Obtain CSS information */

#if (ENABLE_AACS == 1)
        InitializeAacsFlags();

        if (Info.Media.DVD.fCSS_PROTECTED == eAACS_CPS)
        {
            if (dvdReadVolumeIDFromDisc() == PASS)
            {
                AacsStatus.AacsFlag.IsMediumCompliant = TRUE;

                #if (CEProduct == 1)
                if (dvdReadKcdFromDisc() == FAIL)
                    AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                #endif

                #if (CEProduct == 0)
                if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
                    if (dvdReadMkbFromDisc() == FAIL)
                    {
                        if (MkbVersionOnFlash == 0)
                            AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                    }
                #endif
            }
        }
#endif  /* ENABLE_AACS == 1 */

#if (VCPS_TEST == 1)
        if (VcpsKeysValid == TRUE)
        {
            /* Get the absolute address of the control data block */
            DataPtr = (BYTE *)(SCSDRAM_ADDRESS + DVD_PHYSICAL_FORMAT_INFORMATION);

            FlagByte = *(DataPtr + DESCRIPTOR_FLAGS_OFFSET);
            VCPS_disc = (FlagByte & VCPS_IN_BZ2_BIT ? TRUE : FALSE);

            if (VCPS_disc == TRUE)
            {
                vcpsData.DKBLocation = DKB_BUFFER_ZONE2;
                if (dvdReadDKBHeader() == FAIL)
                {
                    vcpsData.DKBLocation = DKB_UNDETERMINED;
                }
            }
        }
#endif  /* VCPS_TEST == 1 */
    }

    /****************************************************************
    *    Possible Multi Session Disc
    *       DVD+R, DVD+R_DL, DVD-R, DVD-R_DL, DVD-RW
    *****************************************************************/
    switch(pdbGetDiscKind())
    {
        /*------------------------------------------------------------
            No Multi Session Family
        -------------------------------------------------------------*/
        case eDVDRAMDisc:
            pdbSetDVDFinalSessCloseInfo(eSessionFinalized);        /* Disc Closed */
            pdbSetDVDLowestRecordedTrackNumber(1);
            pdbSetDVDHighestRecordedTrackNumber(1);
            pdbSetDVDFinalSessionNumber(1);                /* Final Session Number */
            pdbSetDVDFinalSessFirstTrackNumber(1);         /* First Track Number   */
            pdbSetDVDFinalSessLastTrackNumber(1);          /* Last Track Number    */
            DataPtr = (BYTE*) (SCSDRAM_ADDRESS + DVD_LOGICAL_FORMAT_INFORMATION);
            pPsnAddress = (ULONG *) (DataPtr + START_PSN_OF_DATA_AREA_OFFSET);
            pdbSetDVDTrackStartPSN(1, SWAPEND_ULONG(*pPsnAddress));

            pPsnAddress = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
            pdbSetDVDTrackEndPSN(1,  SWAPEND_ULONG(*pPsnAddress));

            break;

        case eDVDPlusRWDisc:
        case eDVDPlusRWDLDisc:
            pdbSetDVDFinalSessCloseInfo(eSessionFinalized);        /* Disc Closed */
            pdbSetDVDSessionCount(1);
            pdbSetDVDLowestRecordedTrackNumber(1);
            pdbSetDVDHighestRecordedTrackNumber(1);
            pdbSetDVDFinalSessionNumber(1);                /* Final Session Number */
            pdbSetDVDFinalSessFirstTrackNumber(1);         /* First Track Number   */
            pdbSetDVDFinalSessLastTrackNumber(1);          /* Last Track Number    */
            pdbSetDVDTrackSessionNumber(1,1);               // Session number of track 1
            pdbSetDVDTrackStartPSN(1, DVD_START_OF_DATA_ZONE);            // Start PSN
            pdbSetDVDTrackStatus(1, eTS_ReservedTrack | eTS_FixedPacket);
            pdbSetDVDTrackState(1, eTRACK_STATE_COMPLETE);
            pdbSetDVDTrackDiagStatus(1, (eTS_Diag_All_done | eTS_Diag_All_valid));
            pdbSetDVDTrackDiagStatus2(1, eTS_NWA_done);

            if(pdbGetDiscKind() == eDVDPlusRWDisc)
            {
                temp.byte.u = 0;
                temp.byte.h = DataPtr[9];      // Read data from Player Database Physical Format Information.
                temp.byte.m = DataPtr[10];
                temp.byte.l = DataPtr[11];
                send_msg5SValue(SEND_DVDPLUSRW_MSG, "MaxInCtrlZn", 4, temp.Long);
                pdbSetDVDTrackEndPSN(1, temp.Long);    //set last physical sector as track end
                pdbSetDVDMaxPSN(0, temp.Long);
                pdbSetDVDMaxLBA(temp.Long - DVD_START_OF_DATA_ZONE);

                if(temp.Long == DVD_PLUSRW_DATAZONE_END)    //after DVD+RW format, track end = DVD_PLUSRW_DATAZONE_END
                    pdbSetDVDTrackNextWritableAddress(1, 0x00L);
                else
                    pdbSetDVDTrackNextWritableAddress(1, dvdConvertPSNtoLBA(temp.Long + 1));
            }
            else
            {
                /* byte 13-15: layer jump address */
                temp.byte.u = 0;
                temp.byte.h = DataPtr[13];          // Read data from Player Database Physical Format Information.
                temp.byte.m = DataPtr[14];
                temp.byte.l = DataPtr[15];
                pdbSetDVDEndLBALayer0(temp.Long);    //for pdbGetDVDEndLBALayer0() in dvdConvertPSNtoLBA()
                pdbSetDVDMaxPSN(0, temp.Long);
                send_msg5SValue(SEND_DVDPLUSRW_MSG, "L0Jump", 4, temp.Long);

                //assume layer1 value = 0x00Fxxxxx
                temp.byte.u = 0;
                temp.byte.h = DataPtr[9];           // Read data from Player Database Physical Format Information.
                temp.byte.m = DataPtr[10];
                temp.byte.l = DataPtr[11];
                send_msg5SValue(SEND_DVDPLUSRW_MSG, "MaxInCtrlZn", 4, temp.Long);

                pdbSetDVDTrackEndPSN(1, temp.Long); //set last physical sector as track end
                pdbSetDVDMaxLBA(dvdConvertPSNtoLBA(temp.Long));

                if(temp.Long > pdbGetDVDMaxPSN(0))
                {
                    pdbSetDVDMaxPSN(1, temp.Long);  //Layer 1 max PSN
                }

                if(temp.Long == DVD_PLUSRW_DATAZONE_END_L1)    //after DVD+RW format, track end = DVD_PLUSRW_DATAZONE_END_L1
                    pdbSetDVDTrackNextWritableAddress(1, 0x00L);
                else
                    pdbSetDVDTrackNextWritableAddress(1, dvdConvertPSNtoLBA(temp.Long + 1));
            }

            break;

        /*------------------------------------------------------------
            DVD+R Multi Session Family
        -------------------------------------------------------------*/
//For DVD+R booktype as DVD-ROM
        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRDLDisc:
            /* RDVD multi-session information was read from TOC. */
            /* If TOC read fail, try control data zone.          */
            if(TOC_ReadFail)
            {
                /* ---------------------------------------------------------------
                     First session track start /end address
                ----------------------------------------------------------------*/
                pPsnAddress = (ULONG *) (DataPtr + PlusR_Start_of_session_offset);
                pdbSetDVDTrackStartPSN(DVDSessionNum, SWAPEND_ULONG(*pPsnAddress));
                pPsnAddress = (ULONG *) (DataPtr + PlusR_End_of_session_offset);
                pdbSetDVDTrackEndPSN(DVDSessionNum, SWAPEND_ULONG(*pPsnAddress));
                pdbSetDVDTrackRecordedPSN(DVDSessionNum,SWAPEND_ULONG(*pPsnAddress));

                if(pdbGetDVDTrackEndPSN(DVDSessionNum))
                {
                    while(!DVDFinalized)
                    {
                        /*---------------------------------------------------------------
                            Session Number
                        ----------------------------------------------------------------*/
                        pdbSetDVDFinalSessionNumber(DVDSessionNum);
                        pdbSetDVDTrackSessionNumber(DVDSessionNum, DVDSessionNum);

                        /*---------------------------------------------------------------
                            Set logical track status and state
                        ----------------------------------------------------------------*/
                        //FWtester2  Item13 No.11 DVD+RDL 8 sessions open compare error
                        //pdbSetDVDTrackStatus(DVDSessionNum, eTS_ReservedTrack);
                        pdbSetDVDTrackState(DVDSessionNum, eTRACK_STATE_COMPLETE);
                        pdbSetDVDTrackDiagStatus(DVDSessionNum, (eTS_Diag_All_done | eTS_Diag_All_valid));
                        pdbSetDVDTrackDiagStatus2(DVDSessionNum, eTS_NWA_done);

                        /*---------------------------------------------------------------
                            Final Session: First and Last track (fragment) number
                        ----------------------------------------------------------------*/
                        pdbSetDVDFinalSessFirstTrackNumber( pdbGetDVDFinalSessionNumber() );
                        pdbSetDVDFinalSessLastTrackNumber( pdbGetDVDFinalSessionNumber() );

                        /*---------------------------------------------------------------
                            Final Session: Lead-in/Lead-out
                            Start of the Intro zone and Closure zones for DVD +R discs
                            (Already initialed for first session)
                        ----------------------------------------------------------------*/
                        if( DVDSessionNum != 1 )
                        {
                            pdbSetDVDFinalSessLeadinPSN( pdbGetDVDTrackStartPSN(DVDSessionNum) - DVD_INTRO_ZONE_SIZE );
                        }

                        pdbSetDVDFinalSessLeadoutPSN( pdbGetDVDTrackEndPSN(DVDSessionNum) + 1L );

                        /*---------------------------------------------------------------
                            Final Session: Next Session Start PSN
                        ----------------------------------------------------------------*/
                        pdbSetDVDNextSessStartPSN( (pdbGetDVDFinalSessLeadoutPSN() +
                                                    DVD_CLOSURE_ZONE_SIZE + DVD_INTRO_ZONE_SIZE) );

                        /*====================================================
                            Read Closure/Lead-Out, and check the Area Type
                            Area Type = LeadOutArea ==> FinalSession
                                      = DataArea    ==> Read next session Intro
                        =====================================================*/

                        /*Closure start PSN = End of Session + 1*/
                        ControlBlockAddress = pdbGetDVDTrackEndPSN(DVDSessionNum) + 1 ;
                        if((ControlBlockAddress > pdbGetDVDMaxPSN(0)) && (pdbGetDVDLayers() == DUAL_LAYER))
                        {
                            if(!(pdbGetDVDTrackEndPSN(DVDSessionNum) & 0x800000) )    /* session end in layer 0, closure start in layer 1 */
                                ControlBlockAddress = pdbGetDVDStartPSN(1) + ControlBlockAddress - pdbGetDVDMaxPSN(0)-1;
                            layer = 1;
                        }
                        else
                            layer = 0;

                        if(((layer== 0) && (pdbGetDVDNextSessStartPSN() > DVD_PLUSR_DATAZONE_END))
                            ||((layer== 1) && (pdbGetDVDNextSessStartPSN() > DVD_PLUSR_DL_DATAZONE_END)) )
                        {
                            /*next session is out off data area*/
                            DVDFinalized = TRUE;

                            /*---------------------------------------------------------------
                                Update counts for logical tracks and sessions
                            ----------------------------------------------------------------*/
                            pdbSetDVDHighestRecordedTrackNumber(DVDSessionNum);
                            pdbSetDVDSessionCount(DVDSessionNum);
                            break;
                        }

                        DiscReadStatus = dvdBufferClosureBlock(layer, ControlBlockAddress);

                        if (DiscReadStatus == BUF_GOOD)
                        {
                            LeadOutData = dvdGetAreaType();
                            if (LeadOutData == LeadOutArea)
                            {
                                DVDFinalized = TRUE;

                                /*---------------------------------------------------------------
                                    Update counts for logical tracks and sessions
                                ----------------------------------------------------------------*/
                                pdbSetDVDHighestRecordedTrackNumber(DVDSessionNum);
                                pdbSetDVDSessionCount(DVDSessionNum);
                            }
                            else if(LeadOutData == DataArea)    /* Data area, read next intro session control data zone */
                            {
                                DVDFinalized = FALSE;

                                /*Closure length = 1024, Intro Start PSN offset 320 = session control data zone   */
                                ControlBlockAddress = pdbGetDVDTrackEndPSN(DVDSessionNum)+ 1024 + 320 + 1;

                                if((ControlBlockAddress > pdbGetDVDMaxPSN(0)) && (pdbGetDVDLayers() == DUAL_LAYER))
                                {
                                    if(!(pdbGetDVDTrackEndPSN(DVDSessionNum) & 0x800000) )    /* previous session in layer 0, next session intro is in layer 1 */
                                        ControlBlockAddress = pdbGetDVDStartPSN(1) + ControlBlockAddress - pdbGetDVDMaxPSN(0)-1;
                                    layer = 1;
                                }
                                else
                                    layer = 0;

                                DiscReadStatus = dvdBufferIntroBlock(layer,ControlBlockAddress);

                                if (DiscReadStatus == BUF_GOOD)
                                {

                                    DataPtr = (BYTE *)(SCSDRAM_START_ADDR + (READ_FIELD(HAL_DVDD_CURECC) * DVD_DATA_BLK_PTR_RESOLUTION));
                                    pPsnAddress = (ULONG *)(DataPtr + PlusR_End_of_session_offset);
                                   //dvdGetSessionData();    /* save  new track start & end PSN */
                                    temp.Long = SWAPEND_ULONG(*pPsnAddress);
                                    if(temp.Long == 0x00) // track end
                                    {
                                        DVDFinalized = TRUE;
                                        /*---------------------------------------------------------------
                                            Update counts for logical tracks and sessions
                                        ----------------------------------------------------------------*/
                                        pdbSetDVDHighestRecordedTrackNumber(DVDSessionNum);
                                        pdbSetDVDSessionCount(DVDSessionNum);
                                    }
                                    else
                                    {

                                        pdbSetDVDSessionCount(++DVDSessionNum);
                                        pdbSetDVDTrackEndPSN(DVDSessionNum, temp.Long);
                                        pdbSetDVDTrackRecordedPSN(DVDSessionNum,temp.Long);
                                        send_msg5SValue(1,"EndPsn",4,SWAPEND_ULONG(*pPsnAddress));

                                        pPsnAddress = (ULONG *) (DataPtr + PlusR_Start_of_session_offset);
                                        pdbSetDVDTrackStartPSN(DVDSessionNum, SWAPEND_ULONG(*pPsnAddress));
                                        send_msg5SValue(1,"StrPsn",4,SWAPEND_ULONG(*pPsnAddress));



                                        /* Update max PSN and L0 end PSN (DVD+R DL only) */
                                        if (pdbGetDVDLayers() == DUAL_LAYER)
                                        {
                                            DataPtr = (BYTE *)CUR_ECC_ADDRESS();

                                            pPsnAddress = (ULONG *)(DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
                                            pdbSetDVDMaxPSN(1, SWAPEND_ULONG(*pPsnAddress));
                                            pPsnAddress = (ULONG *)(DataPtr + END_PSN_LAYER0_OFFSET);
                                            pdbSetDVDMaxPSN(0, SWAPEND_ULONG(*pPsnAddress));
                                        }
                                    }
                                }
                                else
                                {   /* Read LeadIn Fail */
                                    /* set to finalized disc, no write is allowed */
                                    DVDFinalized = TRUE;

                                    /*---------------------------------------------------------------
                                        Update counts for logical tracks and sessions
                                    ----------------------------------------------------------------*/
                                    pdbSetDVDHighestRecordedTrackNumber(DVDSessionNum);
                                    pdbSetDVDSessionCount(DVDSessionNum);

                                    break;
                                }
                            }
                            else
                            {  /* LeadIn Area or Middle Area */
                                break;
                            }
                        }
                        else
                        {  /* Read lead out fail */
                            ClearDataBuffer();      /* Clear the DVD buffer */
                            //ServoStop();

                            DVDFinalized = TRUE;

                            /*---------------------------------------------------------------
                                Update counts for logical tracks and sessions
                            ----------------------------------------------------------------*/
                            pdbSetDVDHighestRecordedTrackNumber(DVDSessionNum);
                            pdbSetDVDSessionCount(DVDSessionNum);
                            break;
                        }
                    }
                    pdbSetDVDFinalSessCloseInfo(eSessionFinalized);         /* Disc Closed */
                    pdbSetDiscStatus(eFinalizedDisc);                                       /* Disc finalized */
                    pdbSetDVDLowestRecordedTrackNumber(1);
                    pdbSetDVDFinalSessionNumber(DVDSessionNum);             /* Final Session Number */
                    pdbSetDVDFinalSessFirstTrackNumber(DVDSessionNum);      /* First Track Number   */
                    pdbSetDVDFinalSessLastTrackNumber(DVDSessionNum);       /* Last Track Number    */
                }
                else
                {
                    //one session or DVDROM disc
                    pdbSetDVDFinalSessCloseInfo(eSessionFinalized);        /* Disc Closed */
                    pdbSetDVDLowestRecordedTrackNumber(1);
                    pdbSetDVDHighestRecordedTrackNumber(1);
                    pdbSetDVDFinalSessionNumber(1);                /* Final Session Number */
                    pdbSetDVDFinalSessFirstTrackNumber(1);         /* First Track Number   */
                    pdbSetDVDFinalSessLastTrackNumber(1);          /* Last Track Number    */
                    DataPtr = (BYTE*) (SCSDRAM_ADDRESS + DVD_LOGICAL_FORMAT_INFORMATION);
                    pPsnAddress = (ULONG *) (DataPtr + START_PSN_OF_DATA_AREA_OFFSET);
                    pdbSetDVDTrackStartPSN(1, SWAPEND_ULONG(*pPsnAddress));

                    pPsnAddress = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
                    pdbSetDVDTrackEndPSN(1,  SWAPEND_ULONG(*pPsnAddress));
                }

                //if (DiscReadStatus == BUF_GOOD)
                //{
                    // move ServoSeek(0x30000L,eLayer0)&ServoPause() from dvdReadControlDataZone()'s "if (DiscReadStatus == BUF_GOOD)"  to dvdDiscSetup()'s "RdCtrlData OK!"
                    //ServoSeek(0x30000L,eLayer0);
                    //ServoPause();                /* execute pause operation */
                //}
            }

            break;
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
            /* RDVD multi-session information was read from Border */
            dvdGetBorderInfo();
            ServoSeek(0x30000L, eLayer0);
            ServoPause();                /* execute pause operation */
            break;
    }

    /*------------------------------------------------------------
        Dual Layer parallel track path (PTP) discs only
    -------------------------------------------------------------*/
    if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
    {
            // Read control data zone of layer 1
            ControlBlockAddress = START_ADDRESS_OF_CONTROL_DATA;
            DiscReadStatus = dvdBufferControlDataBlock(eLayer1, ControlBlockAddress);

            if (DiscReadStatus == BUF_GOOD)
            {
                dvdSetCSSInfo(READ_FIELD(HAL_DVDD_CURECC));     /* Obtain CSS information */

#if (ENABLE_AACS == 1)
                InitializeAacsFlags();

                if (Info.Media.DVD.fCSS_PROTECTED == eAACS_CPS)
                {
                    if (dvdReadVolumeIDFromDisc() == PASS)
                    {
                        AacsStatus.AacsFlag.IsMediumCompliant = TRUE;

                        #if (CEProduct == 1)
                        if (dvdReadKcdFromDisc() == FAIL)
                            AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                        #endif

                        #if (CEProduct == 0)
                        if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
                            if (dvdReadMkbFromDisc() == FAIL)
                            {
                                if (MkbVersionOnFlash == 0)
                                    AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                            }
                        #endif
                    }
                }
#endif  /* ENABLE_AACS == 1 */

                /* Get the absolute address of the valid control data block */
                DataPtr = (BYTE *)(SCSDRAM_START_ADDR + (READ_FIELD(HAL_DVDD_CURECC) * DVD_DATA_BLK_PTR_RESOLUTION));

                // For dual layer PTP the physical format information in layer 1 is the same as layer 0 except
                // the MaxPSN. Retrieve MaxPSN of layer 1
                DataPtr += END_PSN_OF_DATA_AREA_OFFSET;
                pdbSetDVDMaxPSN(1, SWAPEND_ULONG(*((ULONG *) DataPtr)));

            }
            else /* DiscReadStatus != BUF_GOOD */
            {
                ClearDataBuffer();      /* Clear the DVD buffer */
                return (DiscReadStatus);     /* Could not buffer the lead-in control block. Return Error. */
            }
    }

    /*------------------------------------------------------------
        Update logical data
    -------------------------------------------------------------*/
    pdbSetDVDLogicalData();
    ClearDataBuffer();      /* Clear the DVD buffer */

    if(DVDSessionNum >= 1)
        DiscReadStatus = BUF_GOOD;

    return (DiscReadStatus);
}


/*******************************************************************************

    FUNCTION        dvdBufferControlDataBlock

    DESCRIPTION     This function reads an ECC block of the Lead In area of the DVD disc into the buffer

                    For redundancy purposes there are 192 consecutive ECC Blocks of data on a DVD disc
                    containing Leadin Data. The function will read and try to find one good ECC Block
                    of LeadIn data in this area.

                    The read lead in algorithm does the following.

                    A call is made to the disc transfer engine to fill the buffer with lead in blocks
                    starting with the first block of the lead in area. If an error free ecc block is
                    buffered prior to the buffer being filled, transferring will stop and the disc
                    transfer engine returns a BUF_GOOD status, indicating a good lead in block has been
                    buffered. In normal cases this will occur on the first lead in block of the lead in area.

                    If the buffer fills before an error free ecc block is buffered, the disc transfer engine
                    reports back an error status. The next batch of lead in blocks to buffer will then start
                    with a target ID equal to the previous target ID plus the maximum ecc blocks that can be
                    buffered (MAX_DVD_BLOCKS). This repeats until a good lead in block is buffered or all 192
                    blocks have been read without a good status.

    PARAMETERS      Layer     Layer number
                                 0 = layer 0 of disc
                                 1 = layer 1 of disc

    RETURNS         DiscAccessStatus   BUF_GOOD for a "good" block or appropriate error code
                    DataBuffer         A global Structure containing pointers to the valid LeadIn block in the buffer

********************************************************************************/
DiscAccessStatusE dvdBufferControlDataBlock(BYTE Layer, ULONG ControlBlockAddress)
{
    DiscAccessStatusE DiscReadStatus;
    BYTE  ControlBlock;

    send_msg5S(SEND_READ_DVD_MSG,"Buffer CDB");
    /*----------------------------------------------------
        Initialize control block number
    -----------------------------------------------------*/
    ControlBlock = 0;

    /*----------------------------------------------------
        If a control block cannot be read try the next
        sequential control block up to the maximum number
        of control blocks available on the disc.
    -----------------------------------------------------*/
    while (ControlBlock < TOTAL_CONTROL_DATA_BLOCKS)
    {
        ClearDataBuffer();      // Clear the DVD buffer

        /*---------------------------------------------------------------
            Mark read lead in buffering on
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = TRUE;
#if (OPEN_SESSION==1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_CONTROL_DATA;
#endif

        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters for reading
            the lead in block
        ----------------------------------------------------------------*/
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetID.Long    = ControlBlockAddress;
        dvdDiscRequest.EccBlockCount    = MINIMUM(MAX_DVD_BLOCKS , (TOTAL_CONTROL_DATA_BLOCKS - ControlBlock));
        dvdDiscRequest.TargetLayer      = Layer;

        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine to load the blocks
        ----------------------------------------------------------------*/
#if (OPEN_SESSION == 1)
        RF_search_count = 0;
#endif
        DiscReadStatus = dvdStartDiscTransfer();

        /*---------------------------------------------------------------
            Mark read lead in buffering off
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = FALSE;
#if (OPEN_SESSION==1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_NOTHING;
#endif

        /*---------------------------------------------------------------
            Check if the Lead In data has been properly buffered
        ----------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
        {
#if (OPEN_SESSION == 1)
            RF_search_count = 0;
#endif
            return (DiscReadStatus);
        }
        else /* DiscReadStatus != BUF_GOOD */
        {

            //20101016_02PW(720)==start==Add read control data recovery change parameter for DVDMR9
//            if(pdbGetDiscKind() == eDVDMinusRDLDisc)
//            {
//                if((retrycnt<6) && (svoSpeedVar.SpdInRange == TRUE))
//                    retrycnt ++;
//#if (CALIBRATION_MANAGEMENT == 1)
//                if ((retrycnt == 1) && !DVDRAM && calGetCalRequired(dvdDiscRequest.TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))
//                {
//                    (void)ServoReCalibrate();
//                }
//#endif /* CALIBRATION_MANAGEMENT */
//#if (HORROR_DISC_IMPROVEMENT == 1)
//                if (retrycnt > 1)
//                {
//#if(EN_ADEQ)
//                    read_retry_option|=(READ_RETRY_ENABLE|READ_RETRY_TIMER_ENABLE|SERVO_DEFECT_RETRY_ENABLE
//                                                  |HF_DEFECT_RETRY_ENABLE|DFE_RETRY_ENABLE|ADEQ_RETRY_ENABLE
//                                                  |SLOW_PLL_RETRY_FORCE_LOCK_ENABLE);// ==add change ADEQ setting when read retry
//#else
//                    read_retry_option|=(READ_RETRY_ENABLE|READ_RETRY_TIMER_ENABLE|SERVO_DEFECT_RETRY_ENABLE
//                                                  |HF_DEFECT_RETRY_ENABLE|DFE_RETRY_ENABLE
//                                                  |SLOW_PLL_RETRY_FORCE_LOCK_ENABLE);
//#endif
//                    (void)ServoChgRFParameter();
//                }
//#endif
//            }
            //20101016_02PW(720)==end==Add read control data recovery change parameter for DVDMR9


#if (OPEN_SESSION == 1)//target is no RF zone-->skip retry
            if((RF_search_count > 0x03)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                 send_msg80(SEND_READ_DVD_MSG,0x71013D);
                 DiscReadStatus=GEN_FAIL;
                 RF_search_count = 0;
                 break;
            }
            else
#endif
            {
            /*---------------------------------------------------------------
                Could not buffer a lead-in control block. Try the next
                batch of control blocks
            ----------------------------------------------------------------*/
            ControlBlockAddress += (MAX_DVD_BLOCKS * ONE_DVD_BLOCK);
            ControlBlock += MAX_DVD_BLOCKS;
#if (OPEN_SESSION == 1)
                RF_search_count = 0;
#endif
            }
        }
    }
    return (DiscReadStatus);
}



/*******************************************************************************

    FUNCTION        dvdSetCssInfo

    DESCRIPTION     Sets up the CSS information from the control block's
                    auxillary data

    PARAMETERS      DataBlockPtr    - Relative data block pointer

    GLOBAL UPDATES  Info.Media.DVD

    NOTES           This function assumes that CURECC and CURAUX point to a valid
                    control block and its auxillary data.

********************************************************************************/
void dvdSetCSSInfo(USHORT DataBlockPtr)
{
    BYTE *bptr, *p1, *p2, x;

    // Get the absolute address of the auxillary block.
    bptr = (BYTE *) (DVD_AUX_ADDRESS(DataBlockPtr));

    // Point to sector 2's CPR_MAI information within the aux block
    bptr += AUX_SECTOR2_OFFSET;

    // Retrieve copy protection flag
    Info.Media.DVD.fCSS_PROTECTED = *(bptr + CPR_MAI_OFFSET);

    // Retrieve region management information
    Info.Media.DVD.RegionControl = *(bptr + RMA_OFFSET);

   /*---------------------------------------------------------------
        Verify if DVD disc is actually CSS protected.

        Most DVD discs have the CPS_TY byte set to 1 if the disc is
        protected and 0 if it is not protected. Some discs however
        incorrectly show a CPS_TY byte of 0 even though the
        disc is protected. If the first five bytes of the disc key
        location are equal to the next 5 bytes, this means there
        is no disc key present and that the disc is not protected.
    ----------------------------------------------------------------*/

    // Check only if copy protection flag is NOT set
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    if (Info.Media.DVD.fCSS_PROTECTED == eNO_CPS)
#else
    if (Info.Media.DVD.fCSS_PROTECTED == FALSE)
#endif
    {
        p1 = (BYTE *)(DVD_ECC_ADDRESS(DataBlockPtr) + DISC_KEY_RELATIVE_ADDRESS);   /* Beginning of disc key (sector 2) */
        p2 = p1 + CPSTY_CHECK_LEN;                                          /* Set 2nd pointer at 6th byte      */

        for ( x = 0; x < CPSTY_CHECK_LEN; x++)
        {
            if (*p1 != *p2)                             /* Compare first 5 bytes of disc key with 2nd 5 bytes */
            {
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
                Info.Media.DVD.fCSS_PROTECTED = eCSS_CPS;
#else
                Info.Media.DVD.fCSS_PROTECTED = TRUE;    /* Set copy protection if any of the 5 bytes are not equal */
#endif
                break;
            }
            ++p1;
            ++p2;
        }
    }
}


#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
/*******************************************************************************

    FUNCTION        dvdSetCPRMInfo

    DESCRIPTION     Sets up the CPRM information from the control block's
                    auxillary data

    PARAMETERS      DataBlockPtr    - Relative data block pointer

    GLOBAL UPDATES  Info.Media.DVD

    NOTES           This function assumes that CURECC and CURAUX point to a valid
                    control block and its auxillary data.

********************************************************************************/
void dvdSetCPRMInfo(USHORT DataBlockPtr)
{
    BYTE *bptr;

    // Get the absolute address of the auxillary block.
    bptr = (BYTE *) (DVD_AUX_ADDRESS(DataBlockPtr));

    // Point to sector 2's CPR_MAI information within the aux block
    bptr += AUX_SECTOR2_OFFSET;

    // Retrieve copy protection flag
    Info.Media.DVD.fCSS_PROTECTED = *(bptr + CPR_MAI_OFFSET);

    // Retrieve total MKB Packs Used information
    if (Info.Media.DVD.fCSS_PROTECTED == eCPRM_CPS)
    {
        Info.Media.DVD.TotalMKBPacks = *(bptr + MKB_PACKS_OFFSET);
        WRITE_FIELD(HAL_HST_CSSSCREN, 0);   // Disable CSS Scrambling
    }
}
#endif  /* ENABLE_CPRM == 1 && BCA_ENABLE == 1 */

/*******************************************************************************

    FUNCTION        dvdGetAreaType

    DESCRIPTION     Retrieves the area type from the most recent buffered
                    DVD block

    RETURNS         AreaType -  Specifies either Data, Lead-in, Lead-out or
                                Middle area

********************************************************************************/
dvdAreaTypeE    dvdGetAreaType()
{
    BYTE *BufferPtr;
    dvdAreaTypeE AreaType;

    // Get the absolute address of the current auxillary block.
    BufferPtr = (BYTE *) (CUR_AUX_ADDRESS());

    // Get area type from sector information byte
    AreaType = ( (*BufferPtr) & SINFO_AREA_TYPE) >> 2;

    return (AreaType);

}

/*******************************************************************************

    FUNCTION        dvdBufferClosureBlock

    DESCRIPTION

********************************************************************************/
DiscAccessStatusE dvdBufferClosureBlock(BYTE Layer, ULONG ControlBlockAddress)
{
    DiscAccessStatusE DiscReadStatus;
    BYTE  ControlBlock;

    ControlBlock        = 0;

    /*----------------------------------------------------
        If a control block cannot be read try the next
        sequential control block up to the maximum number
        of control blocks available on the disc.
    -----------------------------------------------------*/
    while (ControlBlock < 64)   // Closure length 64 ECC blocks
    {

        ClearDataBuffer();      // Clear the DVD buffer

        /*---------------------------------------------------------------
            Mark read closure buffering on
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = TRUE;
//improve read DVD open session disc
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_CLOSURE;
#endif
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters for reading
            the lead in block
        ----------------------------------------------------------------*/
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetID.Long    = ControlBlockAddress;
        dvdDiscRequest.EccBlockCount    = MINIMUM(MAX_DVD_BLOCKS , (64 -ControlBlock));
        dvdDiscRequest.TargetLayer      = Layer;

        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine to load the blocks
        ----------------------------------------------------------------*/
#if (OPEN_SESSION == 1)
        RF_search_count = 0;
#endif
        DiscReadStatus = dvdStartDiscTransfer();

        //send_msg5SValue(1, "RFcnt", 1, RF_search_count);
        /*---------------------------------------------------------------
            Mark read lead in buffering off
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = FALSE;
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_NOTHING;
#endif
        /*---------------------------------------------------------------
            Check if the Lead In data has been properly buffered
        ----------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
        {
#if (OPEN_SESSION == 1)
            RF_search_count = 0;
#endif
            return (DiscReadStatus);
        }
        else /* DiscReadStatus != BUF_GOOD */
        {
#if (OPEN_SESSION == 1)//target is no RF zone-->skip retry
            if((RF_search_count > 0x01)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                 send_msg80(SEND_READ_DVD_MSG,0x710105);
                 DiscReadStatus=GEN_FAIL;
                 RF_search_count = 0;
                 break;
            }
            else
#endif
            {
                 /*---------------------------------------------------------------
                     Could not buffer a lead-in control block. Try the next
                     batch of control blocks
                 ----------------------------------------------------------------*/
                 ControlBlockAddress += (15 * ONE_DVD_BLOCK);
                 ControlBlock += 15;
#if (OPEN_SESSION == 1)
                 RF_search_count = 0;
#endif
            }
        }
    }
    return (DiscReadStatus);
}

/*******************************************************************************

    FUNCTION        dvdBufferBorderInBlock

    DESCRIPTION

********************************************************************************/
DiscAccessStatusE dvdBufferBorderInBlock(BYTE Layer, ULONG ControlBlockAddress)
{
    DiscAccessStatusE DiscReadStatus;
    BYTE  BufRetry;
    BYTE  BorderInLenth;

    BufRetry = 0;
    BorderInLenth = 5;

    /*----------------------------------------------------

    -----------------------------------------------------*/
    while (BufRetry < BorderInLenth)
    {

        /*---------------------------------------------------------------
            Mark read lead in buffering on
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = TRUE;
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_BORDER_IN;
#endif

        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters for reading
            the border in block
        ----------------------------------------------------------------*/
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetID.Long    = ControlBlockAddress;
        dvdDiscRequest.EccBlockCount    = (BorderInLenth -BufRetry) ;    // border-in physical format information block repeat 5 times
        dvdDiscRequest.TargetLayer      = Layer;

        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine to load the blocks
        ----------------------------------------------------------------*/
#if (OPEN_SESSION == 1)
        RF_search_count = 0;
#endif
        DiscReadStatus = dvdStartDiscTransfer();

        //send_msg5SValue(1, "RFcnt", 1, RF_search_count);
        /*---------------------------------------------------------------
            Mark read lead in buffering off
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = FALSE;
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_NOTHING;
#endif

        /*---------------------------------------------------------------
            Check if the Border In data has been properly buffered
        ----------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
        {
#if (OPEN_SESSION == 1)
             RF_search_count = 0;
#endif
             return (DiscReadStatus);
        }
        else /* DiscReadStatus != BUF_GOOD */
        {
#if (OPEN_SESSION == 1)//target is no RF zone-->skip retry
            if((RF_search_count > 0x01)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                 send_msg80(SEND_READ_DVD_MSG,0x710106);
                 DiscReadStatus=GEN_FAIL;
                 RF_search_count = 0;
                 break;
            }
            else
#endif
            {
                 /*---------------------------------------------------------------
                     Could not buffer a lead-in control block. Try the next
                     batch of control blocks
                 ----------------------------------------------------------------*/
                 ControlBlockAddress += ONE_DVD_BLOCK;
                 BufRetry++;
#if (OPEN_SESSION == 1)
                 RF_search_count = 0;
#endif
            }
        }
    }
    return (DiscReadStatus);
}


/*******************************************************************************

    FUNCTION        dvdBufferIntroBlock

    DESCRIPTION

********************************************************************************/

DiscAccessStatusE dvdBufferIntroBlock(BYTE Layer, ULONG ControlBlockAddress)
{
    DiscAccessStatusE DiscReadStatus;
    BYTE  ControlBlock;

    /*----------------------------------------------------
        Initialize control block number
    -----------------------------------------------------*/
    ControlBlock = 0;

    /*----------------------------------------------------
        If a control block cannot be read try the next
        sequential control block up to the maximum number
        of control blocks available on the disc.
    -----------------------------------------------------*/
    while (ControlBlock < 40)   // Intro session control data zone repeat 40 times
    {

        ClearDataBuffer();      // Clear the DVD buffer

        /*---------------------------------------------------------------
            Mark read Intro buffering on
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = TRUE;
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_INTRO;
#endif

        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters for reading
            the lead in block
        ----------------------------------------------------------------*/
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetID.Long    = ControlBlockAddress;
        dvdDiscRequest.EccBlockCount    = MINIMUM(MAX_DVD_BLOCKS , (40 -ControlBlock));
        dvdDiscRequest.TargetLayer      = Layer;

        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine to load the blocks
        ----------------------------------------------------------------*/
#if (OPEN_SESSION == 1)
        RF_search_count = 0;
#endif
        DiscReadStatus = dvdStartDiscTransfer();

        //send_msg5SValue(1, "RFcnt", 1, RF_search_count);
        /*---------------------------------------------------------------
            Mark read lead in buffering off
        ----------------------------------------------------------------*/
        dvdDecFlag.fREADING_LEADIN = FALSE;
#if (OPEN_SESSION == 1)
        dvdDecFlag.fREADING_SESSION_POSITION = READ_NOTHING;
#endif

        /*---------------------------------------------------------------
            Check if the Lead In data has been properly buffered
        ----------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
        {
#if (OPEN_SESSION == 1)
             RF_search_count = 0;
#endif
             return (DiscReadStatus);
        }
        else /* DiscReadStatus != BUF_GOOD */
        {
#if (OPEN_SESSION == 1)//target is no RF zone-->skip retry
            if((RF_search_count > 0x01)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                 send_msg80(SEND_READ_DVD_MSG,0x710107);
                 DiscReadStatus=GEN_FAIL;
                 RF_search_count = 0;
                 break;
            }
            else
#endif
            {
                 /*---------------------------------------------------------------
                     Could not buffer a lead-in control block. Try the next
                     batch of control blocks
                 ----------------------------------------------------------------*/
                 ControlBlockAddress += (15 * ONE_DVD_BLOCK);
                 ControlBlock += 15;
#if (OPEN_SESSION == 1)
                 RF_search_count = 0;
#endif
            }
        }
    }
    return (DiscReadStatus);
}

/*******************************************************************************

    FUNCTION        dvdCheckVideo

    DESCRIPTION     Check if DVD Video information exist

********************************************************************************/
void dvdCheckVideo (void)
{
    BYTE result;

    result = SpecialDiscInformationParser(VideoDVDInformationTable);

    if(result == eVideoDVD)
        dvdVideoFlag = TRUE;

    send_msg54(SEND_READ_DVD_MSG,'V','D','O',dvdVideoFlag);
}


#if (Detect_AVCHD_DISC == 1)
/*******************************************************************************

    FUNCTION        dvdCheckAVCHD

    DESCRIPTION     Check if AVCHD  information exist

********************************************************************************/
void dvdCheckAVCHD(void)
{
    BYTE result;

    result = SpecialDiscInformationParser(AVCHDInformationTable);

    if(result == eAVCHD)
    {
        AVCHDFlag = TRUE;
        send_msg5S(1,"This is AVCHD!");
    }
}
#endif


/*******************************************************************************

    FUNCTION        dvdSaveFormat1RMDInformation

    DESCRIPTION     Stores the RMA information of a DVD -R to SDRAM

    PARAMETERS      TocData -   Location of RMA data to be read

    RETURNS         PASS        Valid TOC read from buffer area and saved in database
                    FAIL        TOC block missing content descriptor or no toc items

********************************************************************************/
BYTE dvdSaveFormat1RMDInformation(ULONG* RMAData)
{
    BYTE *DataPtr;
    BYTE index,border,zone,hint;
    BYTE tmepFidleID[5],i;

    StLongU temp;
    StWordU wtemp;


    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 1;      //Point to the RMD format
    Info.Media.DVD.RMAData.RMD_format      = (*DataPtr);
    tmepFidleID[0] = (*(DataPtr));
    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 30;      //Point to the field 2
    tmepFidleID[1] = (*(DataPtr));
    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 38;      //Point to the field 3
    tmepFidleID[2] = (*(DataPtr));
    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 46;      //Point to the field 4
    tmepFidleID[3] = (*(DataPtr));
    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 54;      //Point to the field 5
    tmepFidleID[4] = (*(DataPtr));

//check RMD format
    if (Info.Media.DVD.RMAData.RMD_format != Format1_RMD )
        return (FAIL);
    for(i=0;i<5;i++)
    {
        if(tmepFidleID[i]!=(i+1))
        {
            //check FieldID. If data mismatch, don't update track information by RMD data.
            return(FAIL);
        }
    }
    send_msg80(SEND_DVD_MINUS_WR_MSG,0x710115);
    CopyBufferToBuffer(0, 0 , RMD_FORMAT_1_START_PAGE, 0, 16);

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 2;      //Point to the field 0
    Info.Media.DVD.RMAData.Disc_status     = (*DataPtr);
    DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format1.DiscStatus = Info.Media.DVD.RMAData.Disc_status;
    for (index = 0; index < 18; index++)
    {
        DataPtr++;
        Info.Media.DVD.DiscID[index] = (*DataPtr);
    }

    if(pdbGetDiscKind() == eDVDMinusRDLDisc)
    {
        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 86;
        temp.byte.u = *DataPtr++;
        temp.byte.h = *DataPtr++;
        temp.byte.m = *DataPtr++;
        temp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.SSNShiftMiddleArea = temp.Long;
        Info.Media.DVD.RMAData.PreRecordedInfoCode= *DataPtr++;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 92;
        temp.byte.u = *DataPtr++;
        temp.byte.h = *DataPtr++;
        temp.byte.m = *DataPtr++;
        temp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedLeadinArea = temp.Long;

        temp.byte.u = *DataPtr++;
        temp.byte.h = *DataPtr++;
        temp.byte.m = *DataPtr++;
        temp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL0 = temp.Long;

        temp.byte.u = *DataPtr++;
        temp.byte.h = *DataPtr++;
        temp.byte.m = *DataPtr++;
        temp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL1 = temp.Long;

        temp.byte.u = *DataPtr++;
        temp.byte.h = *DataPtr++;
        temp.byte.m = *DataPtr++;
        temp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.StartAddrPreRecordedLeadoutArea= temp.Long;
    }


    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET;       //Point to the field 4
    wtemp.byte.h = *DataPtr++;
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword  = wtemp.Uword;

    wtemp.byte.h = *DataPtr++;      // 2,3
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword  = wtemp.Uword;

    wtemp.byte.h = *DataPtr++;      // 4,5
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword = wtemp.Uword;

    if(pdbGetDiscKind() == eDVDMinusRDLDisc)
    {
        wtemp.byte.h = *DataPtr++;      // 6,7
        wtemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword = wtemp.Uword;
    }

    index = 0;
    border = 0;


    send_msg54(SEND_DVD_MINUS_WR_MSG,'R','M','D','1');
    send_msg54(SEND_DVD_MINUS_WR_MSG,'s','t','a',Info.Media.DVD.RMAData.Disc_status);
    send_msg54(SEND_DVD_MINUS_WR_MSG,'i','v',BHIGH(Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword),BLOW(Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword));
    send_msg54(SEND_DVD_MINUS_WR_MSG,'f','o',BHIGH(Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword),BLOW(Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword));
    send_msg54(SEND_DVD_MINUS_WR_MSG,'s','o',BHIGH(Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword),BLOW(Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword));
    send_msg54(SEND_DVD_MINUS_WR_MSG,'t','o',BHIGH(Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword),BLOW(Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword));

    switch(Info.Media.DVD.RMAData.Disc_status)
    {
        case DiscAtOnce:
             index++;

             pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
             pdbSetDiscStatus(eFinalizedDisc);
             Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword = 1;//Protect the DVD-R DAO disc write wrong InvisibleRzone number. it must be 1, some writer will write wrong data in this field.(interchange1)
             pdbSetDVDFinalSessionNumber(Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword);
             pdbSetDVDTrackSessionNumber(index,Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword);
             pdbSetDVDTrackStatus(index,eTS_None);
             pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
             pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);

             DataPtr  = (BYTE *) (RMAData);
             DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 0x10;
             temp.byte.u = *DataPtr++;
             temp.byte.h = *DataPtr++;
             temp.byte.m = *DataPtr++;
             temp.byte.l = *DataPtr++;
             pdbSetDVDTrackStartPSN(index,temp.Long);

             temp.byte.u = *DataPtr++;
             temp.byte.h = *DataPtr++;
             temp.byte.m = *DataPtr++;
             temp.byte.l = *DataPtr++;
             if(temp.Long != 0)
             {
             pdbSetDVDTrackRecordedPSN(index,temp.Long);
             pdbSetDVDTrackEndPSN(index,temp.Long);
             }

             DataPtr  = (BYTE *) (RMAData);
             DataPtr += DVDMINUS_RMD_FIELD3_OFFSET;
             temp.byte.u = *DataPtr++;
             temp.byte.h = *DataPtr++;
             temp.byte.m = *DataPtr++;
             temp.byte.l = *DataPtr++;
             pdbSetDVDBorderOutPSN(index,temp.Long);
             pdbSetDVDBorderInPSN(index,0x00);

             pdbSetDVDFinalSessFirstTrackNumber(Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword);
             pdbSetDVDFinalSessLastTrackNumber(Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword);

            if(pdbGetDiscKind() == eDVDMinusRDLDisc)
            {
                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 20;
                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;

                pdbSetDVDMaxLBA(temp.Long - 0x30000);

            }
            else
            {
                pdbSetDVDMaxLBA(pdbGetDVDBorderOutPSN(index) - 1 - 0x30000);
            }

             pdbSetDVDFinalSessLeadinPSN(0x00);
             pdbSetDVDFinalSessLeadoutPSN(0x00);
             pdbSetDVDNextSessStartPSN(0x00 );
             pdbSetDVDSessionCount(index);
             pdbSetDiscIDStatus(eDiscIDFound);
             pdbSetDVDFinalSessFirstTrackPSN(0x00);

        break;

        //=========================================================================================================

        case IncRecMode:

             pdbSetDiscStatus(eNonFinalizedDisc);
             pdbSetDVDBorderNumber(1);
             pdbSetDVDLowestRecordedTrackNumber(1);
             //FWtester2  Item13 No.7 DVD-R SL 8 session open compare error
             //for ( index = 1; index <= Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword; index++)
             for ( index = 1; index < Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword; index++)
             {
                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 0x10 + ((index-1)*8);
                temp.byte.u = *DataPtr++;
                if(temp.byte.u == 0xFF)
                    temp.byte.u = 0x00;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackStartPSN(index,temp.Long);
                SendMsgCn(SEND_DVD_MINUS_WR_MSG,5,0x80049D, index, A4B(temp.Long));

                temp.byte.u = *DataPtr++;
                if(temp.byte.u == 0xFF)
                    temp.byte.u = 0x00;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackRecordedPSN(index,temp.Long);
                SendMsgCn(SEND_DVD_MINUS_WR_MSG,5,0x80049E, index, A4B(temp.Long));

                pdbSetDVDHighestRecordedTrackNumber(index);
             }

             do
             {
                border++;
                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + ((border-1)*4) ;
                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDBorderNumber(border);
                pdbSetDVDBorderOutPSN(border,temp.Long);

//                send_msg54(SEND_DVD_MINUS_WR_MSG,'B','r','d',border);
//                send_msg5L(SEND_DVD_MINUS_WR_MSG,temp.Long);


                if  (pdbGetDVDBorderOutPSN(border) != 0x00)
                    pdbSetDVDBorderInPSN(border,pdbGetDVDBorderOutPSN(border)+pdbGetDVDBorderZoneSize(border,pdbGetDVDBorderOutPSN(border))-0x60);
                //== start == FWtester2  Item13 No.7 DVD-R SL 8 session open compare error
                else
                    pdbSetDVDBorderNumber(border-1);
                //== end == FWtester2  Item13 No.7 DVD-R SL 8 session open compare error

             }while (pdbGetDVDBorderOutPSN(border) != 0x00 );


             //------------------------------------------------------------------------------------------

             hint = 1;

             for (zone = 1 ; zone <= index-1 ; zone++)
             {
                if (pdbGetDVDTrackRecordedPSN(zone) < pdbGetDVDBorderOutPSN(hint))
                {
                    if (pdbGetDVDTrackRecordedPSN(zone) == 0x00)
                        pdbSetDVDTrackSessionNumber(zone,hint+1);
                    else
                        pdbSetDVDTrackSessionNumber(zone,hint);
                }
                else
                {
                    if ((pdbGetDVDTrackRecordedPSN(zone) != 0x00) && (hint < border))
                        hint++;

                    pdbSetDVDTrackSessionNumber(zone,hint);
                }

             }

             zone = 1;

             do
             {
                pdbSetDVDFinalSessFirstTrackNumber(zone);
                hint = pdbGetDVDTrackSessionNumber(zone);
                zone++;

                if((hint == 0)||(zone == 0xFF))
                    break;

             }while(hint < pdbGetDVDBorderNumber());

             //----------------------------------------------------------------------------------------------

             pdbSetDVDFinalSessionNumber(pdbGetDVDBorderNumber());
             pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());
             pdbSetDVDFinalSessFirstTrackPSN(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));

             //----------------------------------------------------------------------------------------------

             for (index = 1; index <= pdbGetDVDHighestRecordedTrackNumber(); index++)
             {
                if ((index == pdbGetDVDFirstOpenRZone()) || (index == pdbGetDVDSecondOpenRZone()) || (index == pdbGetDVDThirdOpenRZone()))
                {
                    pdbSetDVDTrackStatus(index,eTS_ReservedTrack|eTS_BlankTrack|eTS_Packet);
                    pdbSetDVDTrackDiagStatus2(index, eTS_NWA_valid);
                    pdbSetDVDTrackState(index, eTRACK_STATE_BLANK);
                    pdbSetDVDTrackEndPSN(index,pdbGetDVDTrackStartPSN(index+1)-0x11);
                    pdbSetDVDTrackNextWritableAddress(index,dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(index)+1));
                }
                //== start == FWtester2  Item13 No.7 DVD-R SL 8 session open compare error
                #if 0
                else if (index == pdbGetDVDHighestRecordedTrackNumber())
                {
                    pdbSetDVDTrackStatus(index,eTS_BlankTrack|eTS_Packet);
                    pdbSetDVDTrackDiagStatus2(index, eTS_NWA_valid);
                    pdbSetDVDTrackState(index, eTRACK_STATE_EMPTY);
                    pdbSetDVDTrackNextWritableAddress(index,dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(index)));

                    if ((pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()) == 0x00) && (pdbGetDVDTrackRecordedPSN(pdbGetDVDHighestRecordedTrackNumber()) != 0x00 ))
                    {
                        pdbSetDVDTrackStatus(index,eTS_Packet);
                        pdbSetDVDTrackState(index, eTRACK_STATE_INCOMPLETE);
                        pdbSetDVDTrackNextWritableAddress(index,(dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(index)) + 1));
                    }

                    if(pdbGetDiscKind() == eDVDMinusRDLDisc)
                        pdbSetDVDTrackEndPSN(index,pdbGetDVDMaxPSN(1));
                    else
                        pdbSetDVDTrackEndPSN(index,pdbGetDVDMaxPSN(0));
                }
                #endif
                //== end == FWtester2  Item13 No.7 DVD-R SL 8 session open compare error
                else
                {
                    pdbSetDVDTrackStatus(index,eTS_Packet);
                    pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);
                    pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
                    pdbSetDVDTrackNextWritableAddress(index,0x00);

                    /* If first session < 0x3FF00, the remain size will be padded 0, */
                    /* thus the track end PSN will not be equal to last recored PSN. */
                    if (pdbGetDVDTrackSessionNumber(index) != pdbGetDVDTrackSessionNumber(index+1))//last track in the session
                        pdbSetDVDTrackEndPSN(index,pdbGetDVDBorderOutPSN(pdbGetDVDTrackSessionNumber(index)) - 0x11);
                    else
                        pdbSetDVDTrackEndPSN(index,pdbGetDVDTrackRecordedPSN(index));
                }
             }

             //----------------------------------------------------------------------------------------------

//==start==Fixed DVD-R single open session disc read TOC fail.
//             if ((pdbGetDVDBorderInPSN(pdbGetDVDBorderNumber()-1)+0x60) == pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()))
//             {
//                 pdbSetDVDFinalSessCloseInfo(eEmptySession);
//             }
//             else
//             {
//                 pdbSetDVDFinalSessCloseInfo(eNotSessionClosed);
//             }
             pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
             pdbSetDiscStatus(eFinalizedDisc);
//==end==Fixed DVD-R single open session disc read TOC fail.
             //----------------------------------------------------------------------------------------------

             //if (pdbGetDVDBorderNumber() >1)
             //    pdbSetDVDMaxLBA(pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()-1) - 1 - 0x30000);
             //else
             //    pdbSetDVDMaxLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber())- 1 - 0x30000);
             pdbSetDVDFinalSessLeadinPSN(0x00);
             pdbSetDVDFinalSessLeadoutPSN(0x00);
             pdbSetDVDNextSessStartPSN(0x00);
             pdbSetDiscIDStatus(eDiscIDFound);

             //------------------------------------------------------------------------------------------------

        break;

        //=========================================================================================================

        case FinalIncDisc:

             pdbSetDiscStatus(eFinalizedDisc);
             pdbSetDVDBorderNumber(1);
             pdbSetDVDLowestRecordedTrackNumber(1);

             for ( index = 1; index <= Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword; index++)
             {
                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 0x10 + ((index-1)*8);
                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackStartPSN(index,temp.Long);

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackRecordedPSN(index,temp.Long);
                pdbSetDVDTrackEndPSN(index,temp.Long);

                pdbSetDVDHighestRecordedTrackNumber(index);

                pdbSetDVDTrackStatus(index,eTS_Packet);
                pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);
                pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
                pdbSetDVDTrackNextWritableAddress(index,0x00);

             }

             do
             {
                border++;

                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + ((border-1)*4) ;
                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;

                if (temp.Long != 0x00)
                {
                    pdbSetDVDBorderNumber(border);
                    pdbSetDVDBorderOutPSN(border,temp.Long);
                    pdbSetDVDBorderInPSN(border,pdbGetDVDBorderOutPSN(border)+pdbGetDVDBorderZoneSize(border,pdbGetDVDBorderOutPSN(border))-0x60);
                }
             }while (pdbGetDVDBorderOutPSN(border) != 0x00 );


             //------------------------------------------------------------------------------------------

             hint = 1;

             for (zone = 1 ; zone <= index-1 ; zone++)
             {
                if (pdbGetDVDTrackRecordedPSN(zone) < pdbGetDVDBorderOutPSN(hint))
                {
                    if (pdbGetDVDTrackRecordedPSN(zone) == 0x00)
                        pdbSetDVDTrackSessionNumber(zone,hint+1);
                    else
                        pdbSetDVDTrackSessionNumber(zone,hint);
                }
                else
                {
                    if ((pdbGetDVDTrackRecordedPSN(zone) != 0x00) && (hint < border))
                        hint++;

                    pdbSetDVDTrackSessionNumber(zone,hint);
                }

             }

             zone = 1;

             do
             {
                pdbSetDVDFinalSessFirstTrackNumber(zone);
                hint = pdbGetDVDTrackSessionNumber(zone);
                zone++;

                if((hint == 0)||(zone == 0xFF))
                    break;

             }while(hint < pdbGetDVDBorderNumber());

             //----------------------------------------------------------------------------------------------

             pdbSetDVDFinalSessionNumber(pdbGetDVDBorderNumber());
             pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());

             //----------------------------------------------------------------------------------------------

             pdbSetDVDFinalSessCloseInfo(eSessionFinalized);

             //----------------------------------------------------------------------------------------------
             if (pdbGetDVDBorderNumber() > 0)
                 pdbSetDVDMaxLBA(pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()) - 1 - 0x30000);
             else
                 pdbSetDVDMaxLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()) - 1 - 0x30000);

             pdbSetDVDFinalSessLeadinPSN(0x00);
             pdbSetDVDFinalSessLeadoutPSN(0x00);
             pdbSetDVDNextSessStartPSN(0x00);
             pdbSetDiscIDStatus(eDiscIDFound);
             pdbSetDVDFinalSessFirstTrackPSN(0x00);

             if (pdbGetDVDBorderOutPSN(1) == MINIMUM_FIRST_BORDEROUT_START) //Minimum first session
             {
                 for (index = 1; index <= pdbGetDVDHighestRecordedTrackNumber(); index++)
                 {
                    if (pdbGetDVDTrackSessionNumber(index) != 1)
                        break;
                 }
                 //Update first session last track EndPSN
                 pdbSetDVDTrackEndPSN(index - 1,MINIMUM_FIRST_BORDEROUT_START - 0x11);
             }
             //------------------------------------------------------------------------------------------------


        break;

    }

    return (PASS);
}

//Read DVD-RW CPRM video Format3 RMD for FWtester2  Item7 DVD CPRM Feature Check
/*******************************************************************************

    FUNCTION        dvdSaveFormat3RMDInformation

    DESCRIPTION     Stores the RMA information of a DVD -R to SDRAM

    PARAMETERS      TocData -   Location of RMA data to be read

    RETURNS         PASS        Valid TOC read from buffer area and saved in database
                    FAIL        TOC block missing content descriptor or no toc items

********************************************************************************/
BYTE dvdSaveFormat3RMDInformation(ULONG* RMAData)
{
    StLongU ltemp;
    StWordU wtemp;

    BYTE *DataPtr;
    BYTE index,border;


    send_msg5S(SEND_DVD_MINUS_WR_MSG, "SaveRMDFmt3");

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 1;                                     //Point to the field 0
    Info.Media.DVD.RMAData.RMD_format      = (*DataPtr);
    if (Info.Media.DVD.RMAData.RMD_format != Format3_RMD ) return (FAIL);

    CopyBufferToBuffer(0, 0 , RMD_FORMAT_3_START_PAGE, 0, 16);

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 2;                                     //Point to the field 0
    Info.Media.DVD.RMAData.Disc_status     = (*DataPtr);
    send_msg5SValue(SEND_DVD_MINUS_WR_MSG,"Disc status",1,Info.Media.DVD.RMAData.Disc_status);


    DataPtr++;

    for (index = 0; index < 18; index++)
    {
        DataPtr++;
        Info.Media.DVD.DiscID[index] = (*DataPtr);
    }

    if (pdbGetDiscKind() == eDVDMinusRWDLDisc)
    {
        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 86;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.SSNShiftMiddleArea = ltemp.Long;
        Info.Media.DVD.RMAData.PreRecordedInfoCode= *DataPtr++;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 92;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedLeadinArea = ltemp.Long;

        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL0 = ltemp.Long;

        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL1 = ltemp.Long;

        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.StartAddrPreRecordedLeadoutArea= ltemp.Long;
    }

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 0x80;
    Info.Media.DVD.RMAData.RMD.Format3.RBGInformation = (*DataPtr);

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD3_OFFSET;
    Info.Media.DVD.RMAData.RMD.Format3.FormatOperationCode = (*DataPtr);
    send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "FmatOpCode", 1,Info.Media.DVD.RMAData.RMD.Format3.FormatOperationCode);
    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 0x02 ;
    ltemp.byte.u = *DataPtr++;
    ltemp.byte.h = *DataPtr++;
    ltemp.byte.m = *DataPtr++;
    ltemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.FormatInformation1.Long = ltemp.Long;

    send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "Informat1", 4,ltemp.Long);
    ltemp.byte.u = *DataPtr++;
    ltemp.byte.h = *DataPtr++;
    ltemp.byte.m = *DataPtr++;
    ltemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.FormatInformation2.Long = ltemp.Long;
    send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "Informat2", 4,ltemp.Long);

    if (pdbGetDiscKind() == eDVDMinusRWDisc)
    {
        border = 0;

        do
        {
           border++;
           DataPtr  = (BYTE *) (RMAData);
           DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 0x0A + ((border-1)*12);
           ltemp.byte.u = *DataPtr++;
           ltemp.byte.h = *DataPtr++;
           ltemp.byte.m = *DataPtr++;
           ltemp.byte.l = *DataPtr++;
           send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "BordInPSN", 4,ltemp.Long);
           pdbSetDVDBorderInPSN(border,ltemp.Long);
           ltemp.byte.u = *DataPtr++;
           ltemp.byte.h = *DataPtr++;
           ltemp.byte.m = *DataPtr++;
           ltemp.byte.l = *DataPtr++;
           pdbSetDVDBorderOutPSN(border,ltemp.Long);
            send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "BordoutPSN", 4,ltemp.Long);
        }while (pdbGetDVDBorderOutPSN(border) != 0x00 );

        if (pdbGetDVDRMADiscStatus() == ResOverWriteIntBorder || border == 1)
            pdbSetDVDBorderNumber(border);
        else
            pdbSetDVDBorderNumber(border - 1);
    }
    else
        pdbSetDVDBorderNumber(1);

    pdbSetDVDBorderInPSN(1,0x02FE10);

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD3_OFFSET+ 0x100;
    wtemp.byte.h = *DataPtr++;
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.LastRZoneNumber.Uword  = wtemp.Uword;

    if (pdbGetDiscKind() == eDVDMinusRWDisc)
    {
        for ( index = 1; index <= Info.Media.DVD.RMAData.RMD.Format3.LastRZoneNumber.Uword; index++)
        {
            DataPtr +=  (index-1)*8;
            ltemp.byte.u = *DataPtr++;
            ltemp.byte.h = *DataPtr++;
            ltemp.byte.m = *DataPtr++;
            ltemp.byte.l = *DataPtr++;
            pdbSetDVDTrackStartPSN(index,ltemp.Long);
            send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "RZoneStart", 4,ltemp.Long);
            ltemp.byte.u = *DataPtr++;
            ltemp.byte.h = *DataPtr++;
            ltemp.byte.m = *DataPtr++;
            ltemp.byte.l = *DataPtr++;
            pdbSetDVDTrackRecordedPSN(index,ltemp.Long);
            pdbSetDVDTrackEndPSN(index,ltemp.Long);
            send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "RZoneEnd", 4,ltemp.Long);
            pdbSetDVDHighestRecordedTrackNumber(index);
            if (index < Info.Media.DVD.RMAData.RMD.Format3.LastRZoneNumber.Uword)
                pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
        }
        pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());

        for (index = 1 ; index <= pdbGetDVDHighestRecordedTrackNumber(); index++)
        {
            pdbSetDVDTrackSessionNumber(index, index);
        }
    }
    else
    {
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        pdbSetDVDTrackStartPSN(pdbGetDVDMinusFormat3LastRZoneNumber(),ltemp.Long);
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "LastRZoneStart", 4,ltemp.Long);
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        pdbSetDVDTrackEndPSN(pdbGetDVDMinusFormat3LastRZoneNumber(),ltemp.Long);
        send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "LastRZoneEnd", 4,ltemp.Long);

        pdbSetDVDHighestRecordedTrackNumber(pdbGetDVDMinusFormat3LastRZoneNumber());
        pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());
        pdbSetDVDTrackSessionNumber(pdbGetDVDMinusFormat3LastRZoneNumber(), 1);

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 512;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.LJAOfLayer0.Long = ltemp.Long;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 516;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.LRA.Long = ltemp.Long;
        pdbSetDVDTrackRecordedPSN(pdbGetDVDMinusFormat3LastRZoneNumber(),ltemp.Long);

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 520;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.PreLJAOfLayer0.Long = ltemp.Long;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 524;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.JumpInterval.Long = ltemp.Long;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 528;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.ORAddrOfLayer0.Long = ltemp.Long;

        DataPtr  = (BYTE *) (RMAData);
        DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 532;
        ltemp.byte.u = *DataPtr++;
        ltemp.byte.h = *DataPtr++;
        ltemp.byte.m = *DataPtr++;
        ltemp.byte.l = *DataPtr++;
        Info.Media.DVD.RMAData.RMD.Format3.ORAddrOfLayer1.Long = ltemp.Long;
    }

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET ;
    ltemp.byte.u = *DataPtr++;
    ltemp.byte.h = *DataPtr++;
    ltemp.byte.m = *DataPtr++;
    ltemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.PSNDSBRMDSet.Long = ltemp.Long;
    ltemp.byte.u = *DataPtr++;
    ltemp.byte.h = *DataPtr++;
    ltemp.byte.m = *DataPtr++;
    ltemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.CertStartPSN.Long = ltemp.Long;
    ltemp.byte.u = *DataPtr++;
    ltemp.byte.h = *DataPtr++;
    ltemp.byte.m = *DataPtr++;
    ltemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format3.CertEndPSN.Long = ltemp.Long;
    //-----------------------------------------------------------------------------
    if(pdbGetDVDRMADiscStatus() == ResOverWriteIntBorder)
    {
        if (pdbGetDVDTrackRecordedPSN(pdbGetDVDHighestRecordedTrackNumber()) == 0x00)
            pdbSetDVDTrackState(pdbGetDVDHighestRecordedTrackNumber(), eTRACK_STATE_EMPTY);
        else
            pdbSetDVDTrackState(pdbGetDVDHighestRecordedTrackNumber(), eTRACK_STATE_INCOMPLETE);

        if (pdbGetDiscKind() == eDVDMinusRWDisc)
            pdbSetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber(),(pdbGetDVDLeadOutStartPSN() - LLALength - TBOLength));

        pdbSetDVDFinalSessCloseInfo(eNotSessionClosed);
    }
    else
    {
        pdbSetDVDTrackState(pdbGetDVDHighestRecordedTrackNumber(), eTRACK_STATE_COMPLETE);
        pdbSetDVDTrackStatus(pdbGetDVDHighestRecordedTrackNumber(), eTS_Packet | eTS_FixedPacket);
        pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
    }

    pdbSetDiscIDStatus(eDiscIDFound);
    return (PASS);
}

 //== start == FWtester2  Item13 No.10 DVD-RDL 10sessions closed compare error
/*******************************************************************************

    FUNCTION        dvdSaveFormat4RMDInformation

    DESCRIPTION     Stores the RMA information of a DVD -R DL to SDRAM

    PARAMETERS      TocData -   Location of RMA data to be read

    RETURNS         PASS        Valid TOC read from buffer area and saved in database
                    FAIL        TOC block missing content descriptor or no toc items

********************************************************************************/
BYTE dvdSaveFormat4RMDInformation(ULONG* RMAData)
{
    BYTE *DataPtr;
    BYTE index, border, zone, hint;

    StLongU temp;
    StWordU wtemp;

    send_msg5S(SEND_DVD_MINUS_WR_MSG, "SaveRMDFmt4");

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 1;      //Point to the field 0
    Info.Media.DVD.RMAData.RMD_format      = (*DataPtr);
    DataPtr++;
    Info.Media.DVD.RMAData.Disc_status     = (*DataPtr);
    DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format1.DiscStatus = Info.Media.DVD.RMAData.Disc_status;

    for (index = 0; index < 18; index++)
    {
        DataPtr++;
        Info.Media.DVD.DiscID[index] = (*DataPtr);
    }

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 86;
    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.SSNShiftMiddleArea = temp.Long;
    Info.Media.DVD.RMAData.PreRecordedInfoCode= *DataPtr++;

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD0_OFFSET + 92;
    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.EndAddrPreRecordedLeadinArea = temp.Long;

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL0 = temp.Long;

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.EndAddrPreRecordedMiddleAreaL1 = temp.Long;

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.StartAddrPreRecordedLeadoutArea= temp.Long;

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET;       //Point to the field 4
    wtemp.byte.h = *DataPtr++;
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.InvisibleRZone.Uword  = wtemp.Uword;
    Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword  = wtemp.Uword;

    wtemp.byte.h = *DataPtr++;      // 2,3
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.FirstOpenRZone.Uword  = wtemp.Uword;
    Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword  = wtemp.Uword;

    wtemp.byte.h = *DataPtr++;      // 4,5
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.SecondOpenRZone.Uword = wtemp.Uword;
    Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword = wtemp.Uword;

    wtemp.byte.h = *DataPtr++;      // 6,7
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.ThirdOpenRZone.Uword = wtemp.Uword;
    Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword = wtemp.Uword;

    DataPtr  = (BYTE *) (RMAData);
    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 16;
    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.SSNInvisibleRzone.Long = temp.Long;    //16~19

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.LJAInvisibleRzone.Long = temp.Long;    //20~23

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.ESNInvisibleRzone.Long = temp.Long;    //24~27

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.LRAInvisibleRzone.Long = temp.Long;    //28~31

    temp.byte.u = *DataPtr++;
    temp.byte.h = *DataPtr++;
    temp.byte.m = *DataPtr++;
    temp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.PreLJAInvisibleRzone.Long = temp.Long; //32~35

    wtemp.byte.h = *DataPtr++;
    wtemp.byte.l = *DataPtr++;
    Info.Media.DVD.RMAData.RMD.Format4.JumpInterval.Uword = wtemp.Uword; //36,37

    index = 0;
    border = 0;

    switch(Info.Media.DVD.RMAData.Disc_status)
    {
            case DiscAtOnce:
                index++;

                pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
                pdbSetDiscStatus(eFinalizedDisc);
                pdbSetDVDFinalSessionNumber(Info.Media.DVD.RMAData.RMD.Format4.InvisibleRZone.Uword);
                pdbSetDVDTrackSessionNumber(index,Info.Media.DVD.RMAData.RMD.Format4.InvisibleRZone.Uword);
                pdbSetDVDTrackStatus(index,eTS_None);
                pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
                pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);

                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 48;

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackStartPSN(index,temp.Long);

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDRZoneLJA(index,temp.Long);

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackEndPSN(index,temp.Long);

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDTrackRecordedPSN(index,temp.Long);

                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD3_OFFSET;

                temp.byte.u = *DataPtr++;
                temp.byte.h = *DataPtr++;
                temp.byte.m = *DataPtr++;
                temp.byte.l = *DataPtr++;
                pdbSetDVDBorderOutPSN(index,temp.Long);

                pdbSetDVDBorderInPSN(index,0x00);

                pdbSetDVDFinalSessFirstTrackNumber(pdbGetDVDInvisibleRZone());
                pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDInvisibleRZone());

                pdbSetDVDFinalSessLeadinPSN(0x00);
                pdbSetDVDFinalSessLeadoutPSN(0x00);
                pdbSetDVDNextSessStartPSN(0x00 );
                pdbSetDVDSessionCount(index);
                pdbSetDiscIDStatus(eDiscIDFound);
                pdbSetDVDFinalSessFirstTrackPSN(0x00);

                break;

            //=========================================================================================================

            case IncRecMode:

                pdbSetDiscStatus(eNonFinalizedDisc);
                pdbSetDVDBorderNumber(1);
                pdbSetDVDLowestRecordedTrackNumber(1);

                //== FWtester2  Item13 No.9 DVD-R DL 10 session open compare error
                //for ( index = 1; index <= pdbGetDVDInvisibleRZone(); index++)
                for ( index = 1; index < pdbGetDVDInvisibleRZone(); index++)
                {
                    DataPtr  = (BYTE *) (RMAData);
                    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 48 + ((index-1)*16);
                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackStartPSN(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDRZoneLJA(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackEndPSN(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackRecordedPSN(index,temp.Long);

                    pdbSetDVDHighestRecordedTrackNumber(index);
                }


                DataPtr  = (BYTE *) (RMAData);
                DataPtr += DVDMINUS_RMD_FIELD3_OFFSET;  //Point to the field 3

                for(hint = 0; hint < 4; hint++)
                {
                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    Info.Media.DVD.APRemappingPSN[hint] = temp.Long;    //Remapping Blosck Sector Number of AP#
                }

                do
                {
                    border++;
                    DataPtr  = (BYTE *) (RMAData);
                    DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 32 + ((border-1)*4) ;
                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDBorderNumber(border);
                    pdbSetDVDBorderOutPSN(border,temp.Long);

 //                   send_msg54(SEND_DVD_MINUS_WR_MSG,'B','r','d',border);
//                    send_msg5L(SEND_DVD_MINUS_WR_MSG,temp.Long);

                    if(pdbGetDVDBorderOutPSN(border) != 0x00)
                        pdbSetDVDBorderInPSN(border,pdbGetDVDBorderOutPSN(border)+pdbGetDVDBorderZoneSize(border,pdbGetDVDBorderOutPSN(border))-0x60);
                    //== start == FWtester2  Item13 No.9 DVD-R DL 10 session open compare error
                    else
                        pdbSetDVDBorderNumber(border-1);
                    //== end == FWtester2  Item13 No.9 DVD-R DL 10 session open compare error
                }while (pdbGetDVDBorderOutPSN(border) != 0x00 );


                //------------------------------------------------------------------------------------------

                hint = 1;

                for (zone = 1 ; zone <= index-1 ; zone++)
                {
                    if (pdbGetDVDRZoneLJA(zone) < pdbGetDVDBorderOutPSN(hint))
                    {
                        if (pdbGetDVDTrackRecordedPSN(zone) == 0x00)
                            pdbSetDVDTrackSessionNumber(zone,hint+1);
                        else
                            pdbSetDVDTrackSessionNumber(zone,hint);
                    }
                    else
                    {
                        if ((pdbGetDVDRZoneLJA(zone) != 0x00) && (hint < border))
                            hint++;

                        pdbSetDVDTrackSessionNumber(zone,hint);
                    }
                }

                zone = 1;

                do
                {
                    pdbSetDVDFinalSessFirstTrackNumber(zone);
                    hint = pdbGetDVDTrackSessionNumber(zone);
                    zone++;

                    if((hint == 0)||(zone == 0xFF))  //while loop protection
                        break;

                }while(hint < pdbGetDVDBorderNumber());

                //----------------------------------------------------------------------------------------------

                pdbSetDVDFinalSessionNumber(pdbGetDVDBorderNumber());
                pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());
                pdbSetDVDFinalSessFirstTrackPSN(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));

                //----------------------------------------------------------------------------------------------

                for (index = 1; index <= pdbGetDVDHighestRecordedTrackNumber(); index++)
                {
                    if ((index == pdbGetDVDFirstOpenRZone()) || (index == pdbGetDVDSecondOpenRZone()) || (index == pdbGetDVDThirdOpenRZone()))
                    {
                        pdbSetDVDTrackStatus(index,eTS_ReservedTrack|eTS_BlankTrack|eTS_Packet);
                        pdbSetDVDTrackDiagStatus2(index, eTS_NWA_valid);
                        pdbSetDVDTrackState(index, eTRACK_STATE_BLANK);
                        pdbSetDVDTrackNextWritableAddress(index,dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(index)));
                    }
                    //== start == FWtester2  Item13 No.9 DVD-R DL 10 session open compare error
                    #if 0
                    else if (index == pdbGetDVDHighestRecordedTrackNumber())
                    {
                        pdbSetDVDTrackStatus(index,eTS_BlankTrack|eTS_Packet);
                        pdbSetDVDTrackDiagStatus2(index, eTS_NWA_valid);
                        pdbSetDVDTrackState(index, eTRACK_STATE_EMPTY);
                        pdbSetDVDTrackNextWritableAddress(index,dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(index)));

                        if ((pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()) == 0x00) && (pdbGetDVDTrackRecordedPSN(index) != 0x00))
                        {
                            pdbSetDVDTrackStatus(index,eTS_Packet);
                            pdbSetDVDTrackState(index, eTRACK_STATE_INCOMPLETE);
                        }
                    }
                    #endif
                    //== end == FWtester2  Item13 No.9 DVD-R DL 10 session open compare error
                    else
                    {
                        pdbSetDVDTrackStatus(index,eTS_Packet);
                        pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);
                        pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
                        pdbSetDVDTrackNextWritableAddress(index,0x00);
                    }
                }

                //----------------------------------------------------------------------------------------------
//=start==Fixed DVD-R single open session disc read TOC fail.
//                if ((pdbGetDVDBorderInPSN(pdbGetDVDBorderNumber()-1)+0x60) == pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()))
//                {
//                    pdbSetDVDFinalSessCloseInfo(eEmptySession);
//                }
//                else
//                {
//                    pdbSetDVDFinalSessCloseInfo(eNotSessionClosed);
//                }
                 pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
                 pdbSetDiscStatus(eFinalizedDisc);
//=end==Fixed DVD-R single open session disc read TOC fail.

                //----------------------------------------------------------------------------------------------

                //if (pdbGetDVDBorderNumber() >1)
                //     pdbSetDVDMaxLBA(pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()-1) - 1 - 0x30000);
                // else
                //     pdbSetDVDMaxLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber())- 1 - 0x30000);

                pdbSetDVDFinalSessLeadinPSN(0x00);
                pdbSetDVDFinalSessLeadoutPSN(0x00);
                pdbSetDVDNextSessStartPSN(0x00);
                pdbSetDiscIDStatus(eDiscIDFound);

                //------------------------------------------------------------------------------------------------
                break;

            //=========================================================================================================

            case FinalIncDisc:

                pdbSetDiscStatus(eFinalizedDisc);
                pdbSetDVDBorderNumber(1);
                pdbSetDVDLowestRecordedTrackNumber(1);

                for ( index = 1; index <= pdbGetDVDInvisibleRZone(); index++)
                {
                    DataPtr  = (BYTE *) (RMAData);
                    DataPtr += DVDMINUS_RMD_FIELD4_OFFSET + 48 + ((index-1)*16);
                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackStartPSN(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDRZoneLJA(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackEndPSN(index,temp.Long);

                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;
                    pdbSetDVDTrackRecordedPSN(index,temp.Long);

                    pdbSetDVDHighestRecordedTrackNumber(index);

                    pdbSetDVDTrackStatus(index,eTS_Packet);
                    pdbSetDVDTrackDiagStatus2(index, eTS_NWA_done);
                    pdbSetDVDTrackState(index, eTRACK_STATE_COMPLETE);
                    pdbSetDVDTrackNextWritableAddress(index,0x00);

                }

                do
                {
                    border++;

                    DataPtr  = (BYTE *) (RMAData);
                    DataPtr += DVDMINUS_RMD_FIELD3_OFFSET + 32 + ((border-1)*4) ;
                    temp.byte.u = *DataPtr++;
                    temp.byte.h = *DataPtr++;
                    temp.byte.m = *DataPtr++;
                    temp.byte.l = *DataPtr++;

                    if (temp.Long != 0x00)
                    {
                        pdbSetDVDBorderNumber(border);
                        pdbSetDVDBorderOutPSN(border,temp.Long);
                        pdbSetDVDBorderInPSN(border,pdbGetDVDBorderOutPSN(border)+pdbGetDVDBorderZoneSize(border,pdbGetDVDBorderOutPSN(border))-0x60);
                    }
                }while (pdbGetDVDBorderOutPSN(border) != 0x00 );


                //------------------------------------------------------------------------------------------

                hint = 1;

                for (zone = 1 ; zone <= index-1 ; zone++)
                {
                    if (pdbGetDVDRZoneLJA(zone) < pdbGetDVDBorderOutPSN(hint))
                    {
                        if (pdbGetDVDTrackRecordedPSN(zone) == 0x00)
                            pdbSetDVDTrackSessionNumber(zone,hint+1);
                        else
                            pdbSetDVDTrackSessionNumber(zone,hint);
                    }
                    else
                    {
                        if ((pdbGetDVDTrackRecordedPSN(zone) != 0x00) && (hint < border))
                            hint++;

                        pdbSetDVDTrackSessionNumber(zone,hint);
                    }
                }

                zone = 1;

                do
                {
                    pdbSetDVDFinalSessFirstTrackNumber(zone);
                    hint = pdbGetDVDTrackSessionNumber(zone);
                    zone++;


                    if((hint == 0)||(zone == 0xFF))

                        break;

                }while(hint < pdbGetDVDBorderNumber());

                //----------------------------------------------------------------------------------------------

                pdbSetDVDFinalSessionNumber(pdbGetDVDBorderNumber());
                pdbSetDVDFinalSessLastTrackNumber(pdbGetDVDHighestRecordedTrackNumber());

                //----------------------------------------------------------------------------------------------

                pdbSetDVDFinalSessCloseInfo(eSessionFinalized);

                //----------------------------------------------------------------------------------------------

                //if (pdbGetDVDBorderNumber() > 1)
                //     pdbSetDVDMaxLBA(pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber()-1) - 1 - 0x30000);
                //else
                //    pdbSetDVDMaxLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()) - 1 - 0x30000);

                pdbSetDVDFinalSessLeadinPSN(0x00);
                pdbSetDVDFinalSessLeadoutPSN(0x00);
                pdbSetDVDNextSessStartPSN(0x00);
                pdbSetDiscIDStatus(eDiscIDFound);
                pdbSetDVDFinalSessFirstTrackPSN(0x00);

             //------------------------------------------------------------------------------------------------
                break;

        }

    return (PASS);

}


/*******************************************************************************

    FUNCTION        dvdGetBorderInfo

    DESCRIPTION     This function extracts the lead in data from the buffer
                    and stores the information into the dvdLeadIn structure

    RETURNS         LeadInData structure containing control block information

    NOTES           This function assumes that the block containing the
                    lead in information is the block that has just been
                    bufferred (One block back from the current ecc block pointer)
                    Future enhancement of this function would be to pass the
                    address in the buffer where this lead in block resides.

********************************************************************************/
void dvdGetBorderInfo()
{
    DiscAccessStatusE   DiscReadStatus;     /* Status of the DVD disc access */
    ULONG ControlBlockAddress ;
    ULONG *longptr;
    BYTE * DataPtr;
    BYTE * disctype;
    BYTE i;
    ULONG tempMaxPSN,tmpBorderOutPSN;
    BYTE *DataPtrTmp,RMD_format; //FWtester2  Item13 No.10 DVD-RDL 10sessions closed compare error
    BYTE clayer;
    if(pdbGetDVDLayers() == SINGLE_LAYER)
        tempMaxPSN = pdbGetDVDMaxPSN(0);
    else
        tempMaxPSN = pdbGetDVDMaxPSN(1);

    //send_msg5SValue(SEND_DVD_MINUS_WR_MSG, "GetBorderInfo",4,tempMaxPSN);
    send_msg80(SEND_DVD_MINUS_WR_MSG,0x710130);

    if (pdbGetDVDRMADiscStatus() == ResOverWriteIntBorder)
    {
        send_msg80(SEND_DVD_MINUS_WR_MSG,0x710118);
        return;
    }

    ControlBlockAddress = DVDMINUSR_PHYSICAL_ZONE_START_ADDR;
    DiscReadStatus      = dvdBufferControlDataBlock(0, ControlBlockAddress);

    if (DiscReadStatus == BUF_GOOD)
    {
        DataPtr = (BYTE *)CUR_ECC_ADDRESS() + 0x1000 ;  // Get the absolute address of the control data block
//protect BookType record wrong data on disc, using control data zone data to instend.
        longptr = (ULONG *) (DataPtr + START_PSN_OF_DATA_AREA_OFFSET);
        disctype = (BYTE *)(DataPtr + BOOK_TYPE_AND_VERSION_OFFSET);
        *disctype &= (BYTE)DVDHD_TYPE_MASK;
        if((pdbGetDVDType() == *disctype)&&(pdbGetDVDStartPSN(0) == SWAPEND_ULONG(*longptr)))
        {
            pdbSetDVDPhysicalFormatInfo(DataPtr);           // Update physical format information in dvd database for layer 0
            pdbSetDVDLogicalFormatInfo(DataPtr);
        }
        else
        {
            DataPtr = (BYTE *)(SCSDRAM_START_ADDR + DVD_PHYSICAL_FORMAT_INFORMATION);
        }

        pdbSetDiscStatus(eFinalizedDisc);
        Info.Media.DVD.BorderZoneCount = 1;
        /*---------------------------------------------------------------
            Session Number
        ----------------------------------------------------------------*/
        pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
        pdbSetDVDSessionCount( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDFinalSessionNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDTrackSessionNumber( Info.Media.DVD.BorderZoneCount,  Info.Media.DVD.BorderZoneCount);
        pdbSetDVDLowestRecordedTrackNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDHighestRecordedTrackNumber( Info.Media.DVD.BorderZoneCount);
        /*---------------------------------------------------------------
           Set logical track status and state
        ----------------------------------------------------------------*/
        //== FWtester2  Item13 No.5 DVD-RW 1 session compare error
        //pdbSetDVDTrackStatus( Info.Media.DVD.BorderZoneCount, eTS_ReservedTrack);
        pdbSetDVDTrackState( Info.Media.DVD.BorderZoneCount, eTRACK_STATE_COMPLETE);
        pdbSetDVDTrackDiagStatus( Info.Media.DVD.BorderZoneCount, (eTS_Diag_All_done | eTS_Diag_All_valid));
        pdbSetDVDTrackDiagStatus2( Info.Media.DVD.BorderZoneCount, eTS_NWA_done);
        pdbSetDVDTrackNextWritableAddress( Info.Media.DVD.BorderZoneCount,0x00);
        pdbSetDVDFinalSessFirstTrackNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDFinalSessLastTrackNumber( Info.Media.DVD.BorderZoneCount);
        longptr = (ULONG *) (DataPtr + START_PSN_OF_DATA_AREA_OFFSET);
        pdbSetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount, SWAPEND_ULONG(*longptr));
        longptr = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
        pdbSetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount,  SWAPEND_ULONG(*longptr));
        pdbSetDVDTrackRecordedPSN(Info.Media.DVD.BorderZoneCount, SWAPEND_ULONG(*longptr));
        pdbSetDVDMaxLBA(pdbGetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount) - pdbGetDVDTrackStartPSN(1));
        pdbSetDVDFinalSessFirstTrackPSN(pdbGetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount));
        /*---------------------------------------------------------------
            Set Bord information.
        ----------------------------------------------------------------*/
        pdbSetDVDBorderNumber(Info.Media.DVD.BorderZoneCount);
        longptr = (ULONG *) (DataPtr + MinusR_Next_BorderIn_offset);
        pdbSetDVDBorderInPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));
        pdbSetDVDNextBorderInPSN(SWAPEND_ULONG(*longptr));
        longptr = (ULONG *) (DataPtr + MinusR_Current_BorderOut_offset);
        pdbSetDVDBorderOutPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));

        pdbSetDVDRMADiscStatus(FinalIncDisc);

        while(pdbGetDVDNextBorderInPSN() != 0x00)
        {
            if((pdbGetDVDNextBorderInPSN() > pdbGetDVDMaxPSN(0)) && (pdbGetDVDLayers() == DUAL_LAYER))
                clayer = eLayer1;
            else
                clayer = eLayer0;
            DiscReadStatus = dvdBufferBorderInBlock(clayer, pdbGetDVDNextBorderInPSN());
            if(DiscReadStatus == BUF_GOOD)
            {
                Info.Media.DVD.BorderZoneCount++;
                DataPtr = (BYTE *)CUR_ECC_ADDRESS();
                longptr = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
                if ((dvdGetAreaType() == LeadOutArea) || (pdbGetDVDNextBorderInPSN()> tempMaxPSN)||(SWAPEND_ULONG(*longptr)>tempMaxPSN)||(SWAPEND_ULONG(*longptr)==0))
                {
                    pdbSetDiscStatus(eFinalizedDisc);
                    break;
                }
                pdbSetDVDPhysicalFormatInfo(DataPtr);           // Update physical format information in dvd database for layer 0
                pdbSetDVDLogicalFormatInfo(DataPtr);
                /*---------------------------------------------------------------
                    Session Number
                ----------------------------------------------------------------*/
                pdbSetDVDSessionCount( Info.Media.DVD.BorderZoneCount);
                pdbSetDVDFinalSessionNumber( Info.Media.DVD.BorderZoneCount);
                pdbSetDVDTrackSessionNumber( Info.Media.DVD.BorderZoneCount,  Info.Media.DVD.BorderZoneCount);
                pdbSetDVDHighestRecordedTrackNumber( Info.Media.DVD.BorderZoneCount);
                send_msg5SValue(SEND_READ_DVD_MSG,"HighTrk",1,pdbGetDVDHighestRecordedTrackNumber());
                /*---------------------------------------------------------------
                    Set logical track status and state
                ----------------------------------------------------------------*/
                //== start == FWtester2  Item13 No.10 DVD-RDL 10 session closed compare error
                //pdbSetDVDTrackStatus( Info.Media.DVD.BorderZoneCount, eTS_ReservedTrack);
                pdbSetDVDTrackStatus( Info.Media.DVD.BorderZoneCount, eTS_Packet);
                //== end == FWtester2  Item13 No.10 DVD-RDL 10 session closed compare error
                pdbSetDVDTrackState( Info.Media.DVD.BorderZoneCount, eTRACK_STATE_COMPLETE);
                pdbSetDVDTrackDiagStatus( Info.Media.DVD.BorderZoneCount, (eTS_Diag_All_done | eTS_Diag_All_valid));
                pdbSetDVDTrackDiagStatus2( Info.Media.DVD.BorderZoneCount, eTS_NWA_done);
                pdbSetDVDTrackNextWritableAddress( Info.Media.DVD.BorderZoneCount,0x00);
                pdbSetDVDFinalSessFirstTrackNumber( Info.Media.DVD.BorderZoneCount);
                pdbSetDVDFinalSessLastTrackNumber( Info.Media.DVD.BorderZoneCount);
                pdbSetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount, pdbGetDVDNextBorderInPSN() + (6*ONE_DVD_BLOCK));
                send_msg5SValue(SEND_READ_DVD_MSG,"TrkSpsn",4,pdbGetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount));

                longptr = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
                pdbSetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount,  SWAPEND_ULONG(*longptr));
                send_msg5SValue(SEND_READ_DVD_MSG,"TrkEpsn",4,pdbGetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount));
                pdbSetDVDTrackRecordedPSN(Info.Media.DVD.BorderZoneCount, SWAPEND_ULONG(*longptr));

               pdbSetDVDMaxLBA(pdbGetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount) - pdbGetDVDTrackStartPSN(1));
               pdbSetDVDFinalSessFirstTrackPSN(pdbGetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount));
                /*---------------------------------------------------------------
                    Set Bord information.
                ----------------------------------------------------------------*/
                pdbSetDVDBorderNumber(Info.Media.DVD.BorderZoneCount);
                longptr = (ULONG *) (DataPtr + MinusR_Next_BorderIn_offset);
                pdbSetDVDBorderInPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));
                pdbSetDVDNextBorderInPSN(SWAPEND_ULONG(*longptr));
                longptr = (ULONG *) (DataPtr + MinusR_Current_BorderOut_offset);
                pdbSetDVDBorderOutPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));
            }
            else
            {
                pdbSetDiscStatus(eNonFinalizedDisc);
                ClearDataBuffer();
                break;
            }
        }
        //read Boradout RMA data.
        #if (R_DVDMRW_BOARDOUT_IN_STARTUP == 0)
        if(pdbGetDVDRW()==FALSE)
        #endif
        {
            tmpBorderOutPSN = pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber());
			
                        //[S07][J50Q]Modify U3 DVD not Ready.
			SendMsg5SValue(1, "B-No =", 4, pdbGetDVDBorderNumber());
			SendMsg5SValue(1, "B-Out Add=", 4, tmpBorderOutPSN);
			if((tmpBorderOutPSN==0)&&(pdbGetDVDBorderNumber() > 1))
				pdbSetDVDBorderNumber(1);
			
            if((tmpBorderOutPSN==0)&&(pdbGetDVDBorderNumber()==1))
                tmpBorderOutPSN = DVDMINUSR_EXTRA_BORDER_OUT_START_ADDR;
            if((tmpBorderOutPSN > pdbGetDVDMaxPSN(0)) && (pdbGetDVDLayers() == DUAL_LAYER))
                clayer = eLayer1;
            else
                clayer = eLayer0;

            for( i = 0; i < 5; i++)     /*Current RMD Repeat 5 times in Border-Out*/
            {
                dvdDiscRequest.TargetID.Long = tmpBorderOutPSN + (i*0x10);
                dvdDiscRequest.Type          = INIT_READ;
                dvdDiscRequest.TargetLayer   = clayer;//==start==change reason Correct the DVD-R Border layer information.
                dvdDiscRequest.EccBlockCount = 1L;
#if (OPEN_SESSION == 1)
                dvdDecFlag.fREADING_SESSION_POSITION = READ_BORDER_OUT;
#endif

                DiscReadStatus = dvdStartDiscTransfer();
                longptr = (ULONG*)(SCSDRAM_ADDRESS + DVD_DATA_BUFFER_START);
#if (OPEN_SESSION == 1)
                dvdDecFlag.fREADING_SESSION_POSITION = READ_NOTHING;
#endif

                if(DiscReadStatus == BUF_GOOD)
                {
                    //== start == FWtester2  Item13 No.10 DVD-RDL 10sessions closed compare error
                    DiscReadStatus = RMA_FAIL;
                    DataPtrTmp = (BYTE*)longptr + DVDMINUS_RMD_FIELD0_OFFSET + 1;      //Point to the field 0
                    RMD_format = (*DataPtrTmp);
	            #if (R_DVDMRW_BOARDOUT_IN_STARTUP == 1)
                    if(RMD_format == Format1_RMD)
                    {
                        if (dvdSaveFormat1RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }
                    else if(RMD_format == Format3_RMD)
                    {
                        if (dvdSaveFormat3RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }
                    else if(RMD_format == Format4_RMD)
                    {
                        if (dvdSaveFormat4RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                    else
                            DiscReadStatus = BUF_GOOD;
                    }
                    #else
                    if(RMD_format == Format1_RMD)
                    {
                        if (dvdSaveFormat1RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }
                    else if(RMD_format == Format4_RMD)
                    {
                        if (dvdSaveFormat4RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }
                    #endif
                    //== end == FWtester2  Item13 No.10 DVD-RDL 10sessions closed compare error

                    if(DiscReadStatus == BUF_GOOD)
                        break;
                    else
                        ClearDataBuffer();      /* Clear the DVD buffer */
                }
                else
                {
                    send_msg5SValue(1, "RMDFAIL", 1, i);
                    ClearDataBuffer();      /* Clear the DVD buffer */
                }
            }
        }
    }
    else
    {
        DataPtr = (BYTE *)(SCSDRAM_START_ADDR + DVD_PHYSICAL_FORMAT_INFORMATION);

        pdbSetDiscStatus(eFinalizedDisc);
        Info.Media.DVD.BorderZoneCount = 1;
        /*---------------------------------------------------------------
            Session Number
        ----------------------------------------------------------------*/
        pdbSetDVDFinalSessCloseInfo(eSessionFinalized);
        pdbSetDVDSessionCount( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDFinalSessionNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDTrackSessionNumber( Info.Media.DVD.BorderZoneCount,  Info.Media.DVD.BorderZoneCount);
        pdbSetDVDLowestRecordedTrackNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDHighestRecordedTrackNumber( Info.Media.DVD.BorderZoneCount);

        pdbSetDVDTrackState( Info.Media.DVD.BorderZoneCount, eTRACK_STATE_COMPLETE);
        pdbSetDVDTrackDiagStatus( Info.Media.DVD.BorderZoneCount, (eTS_Diag_All_done | eTS_Diag_All_valid));
        pdbSetDVDTrackDiagStatus2( Info.Media.DVD.BorderZoneCount, eTS_NWA_done);
        pdbSetDVDTrackNextWritableAddress( Info.Media.DVD.BorderZoneCount,0x00);
        pdbSetDVDFinalSessFirstTrackNumber( Info.Media.DVD.BorderZoneCount);
        pdbSetDVDFinalSessLastTrackNumber( Info.Media.DVD.BorderZoneCount);
        longptr = (ULONG *) (DataPtr + START_PSN_OF_DATA_AREA_OFFSET);
        pdbSetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount, SWAPEND_ULONG(*longptr));
        longptr = (ULONG *) (DataPtr + END_PSN_OF_DATA_AREA_OFFSET);
        pdbSetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount,  SWAPEND_ULONG(*longptr));
        pdbSetDVDTrackRecordedPSN(Info.Media.DVD.BorderZoneCount, SWAPEND_ULONG(*longptr));
        pdbSetDVDMaxLBA(pdbGetDVDTrackEndPSN( Info.Media.DVD.BorderZoneCount) - pdbGetDVDTrackStartPSN(1));
        pdbSetDVDFinalSessFirstTrackPSN(pdbGetDVDTrackStartPSN( Info.Media.DVD.BorderZoneCount));
        /*---------------------------------------------------------------
            Set Bord information.
        ----------------------------------------------------------------*/
        pdbSetDVDBorderNumber(Info.Media.DVD.BorderZoneCount);
        longptr = (ULONG *) (DataPtr + MinusR_Next_BorderIn_offset);
        pdbSetDVDBorderInPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));
        pdbSetDVDNextBorderInPSN(SWAPEND_ULONG(*longptr));
        longptr = (ULONG *) (DataPtr + MinusR_Current_BorderOut_offset);
        pdbSetDVDBorderOutPSN( Info.Media.DVD.BorderZoneCount,SWAPEND_ULONG(*longptr));
        pdbSetDVDRMADiscStatus(FinalIncDisc);
        #if(DVDMR_NoDataInRPhyForInf == 1)
        if(pdbGetDVDRW()==FALSE)
        {
            tmpBorderOutPSN = pdbGetDVDBorderOutPSN(pdbGetDVDBorderNumber());
            if((tmpBorderOutPSN==0)&&(pdbGetDVDBorderNumber()==1))
                tmpBorderOutPSN = DVDMINUSR_EXTRA_BORDER_OUT_START_ADDR;
            if((tmpBorderOutPSN > pdbGetDVDMaxPSN(0)) && (pdbGetDVDLayers() == DUAL_LAYER))
                clayer = eLayer1;
            else
                clayer = eLayer0;

            for( i = 0; i < 5; i++)     /*Current RMD Repeat 5 times in Border-Out*/
            {
                dvdDiscRequest.TargetID.Long = tmpBorderOutPSN + (i*0x10);
                dvdDiscRequest.Type          = INIT_READ;
                dvdDiscRequest.TargetLayer   = clayer;
                dvdDiscRequest.EccBlockCount = 1L;

                DiscReadStatus = dvdStartDiscTransfer();
                longptr = (ULONG*)(SCSDRAM_ADDRESS + DVD_DATA_BUFFER_START);

                if(DiscReadStatus == BUF_GOOD)
                {
                    DiscReadStatus = RMA_FAIL;
                    DataPtrTmp = (BYTE*)longptr + DVDMINUS_RMD_FIELD0_OFFSET + 1;      //Point to the field 0
                    RMD_format = (*DataPtrTmp);

                    if(RMD_format == Format1_RMD)
                    {
                        if (dvdSaveFormat1RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }
                    else if(RMD_format == Format4_RMD)
                    {
                        if (dvdSaveFormat4RMDInformation(longptr) == FAIL)
                            DiscReadStatus = RMA_FAIL;
                        else
                            DiscReadStatus = BUF_GOOD;
                    }

                    if(DiscReadStatus == BUF_GOOD)
                        break;
                    else
                        ClearDataBuffer();      /* Clear the DVD buffer */
                }
                else
                {
                    send_msg5SValue(1, "0RMDFAIL", 1, i);
                    ClearDataBuffer();      /* Clear the DVD buffer */
                }
            }
        }
        #endif
    }
}


/*******************************************************************************
    FUNCTION        dvdGetCurrentPSN

    DESCRIPTION     This function read the PSN from wobble / Data ID

    RETURNS         Current PSN
********************************************************************************/
ULONG dvdGetCurrentPSN(BYTE * DVDDecoder_ID)
{
    ULONG clock,clock1,CurrentPSN, time_out;
    StLongU   currentID_temp;

    *DVDDecoder_ID = FALSE;
    pdbSetIDIntStatus(FALSE);

    CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);

    // Get current PSN. Add one dvd block to compensate wobble address delay
    StartTimer(&clock);
    clock1 = clock;

    if(pdbServoGetDiscType() == eADIPDisc)
        time_out = 50L;
    else
        time_out = 100L;

    while (1)
    {
        if (ReadTimer(&clock) > time_out )
        {
            CurrentPSN = 0xFFFFFFFFL;
            break;
        }

        /* no decoded ID available in recorded area for a while, then reset VCO*/
        if (HAL_GLOBAL_GetRfPresent())
        {
            if (ReadTimer(&clock1) > 16L )
            {
#if (DFE_PLL_RESET_IN_dvdGetCurrentPSN == 1)
            	HAL_DFE_ResetFastPll(halDFE_FastPllReset);                  /* Reset Fast PLL */

 	#if (EN_WOBBLE == 1)
        #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
                if ((svoVar_discKindT.Bit.Media == eMediaROM)
            #if (DVD_RAM_READ == 1)
                    || (DVDRAM_embossed)
            #endif
        #else
                if (((svoVar_discKindT.Bit.DiscStd != eDiscBD) || (svoVar_discKindT.Bit.Media == eMediaROM))
            #if (DVD_RAM_READ == 1)
                    && (!DVDRAM_rewritable)
            #endif
        #endif
                )
	#endif
                {
                	if (!HAL_DFE_GetPllInLock())
#endif
					{

		            	send_msg80(DEBUG_WBL_MSG,0x6100F9);
		                SetDfeWblFrontOnHoldTracking();
		                SetDfeWblFrontSemiSynchrAfterTracking();
#if (DFE_PLL_RESET_IN_dvdGetCurrentPSN == 1)
		                Delay100uS(10);
		            }
		        }
#else
		                Delay100uS(100);
		            }
#endif
            	SetDfeWblFrontBitSynchr();

                Delay100uS(10);

#if (DFE_PLL_RESET_IN_dvdGetCurrentPSN == 0)
	            while(1)
	            {
	            	if(HAL_WOBBLE_GetSyncFlyWheelInLock())
	            	{
	            		send_msg1('l','o','c',ReadTimer(&clock1));
	            		break;
	            	}
	            	if(ReadTimer(&clock1) > 20L)
	            	{
	            		send_msg1('T','o','u',ReadTimer(&clock1));
	            		break;
	            	}
	            }
#endif
	            StartTimer(&clock1);
            }
        }

        if (pdbGetIDIntStatus() == TRUE)
        {
            pdbSetIDIntStatus(FALSE);
            if (pdbGetIDOk() == TRUE)
            {   // Monitor Wobble(ADIP/LPP) Address
#if 1
                currentID_temp.Long = pdbGetWblAddress() ;   // ECCBLKAddress
                SendMsgCn(SEND_DVD_MINUS_WR_MSG|SEND_SStpTask_MSG,4,0x80048E, A4B(currentID_temp.Long));
                if ((currentID_temp.Long<0x01E800) && (currentID_temp.Long>0x26F000))
                {
                    SendMsgCn(SEND_DVD_MINUS_WR_MSG|SEND_SStpTask_MSG,4,0x80048F, A4B(currentID_temp.Long));
                    break;
                }
                else
                {
                    CurrentPSN =  currentID_temp.Long ;
                    break;
                }
#else // filter abnormal PSN
                CurrentPSN = pdbGetWblAddress();
                break;
#endif
            }
        }

        if (READ_FIELD(HAL_DVDD_CIDI))
        {
            if (READ_FIELD(HAL_DVDD_CIDVLD))
            {
                currentID_temp.Long = READ_FIELD(HAL_DVDD_CURID)&0x00FFFFFF;	// TBD? There are masks defined for this!
                CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                CurrentPSN = currentID_temp.Long;
                *DVDDecoder_ID = TRUE;

                if (currentID_temp.Long>0x00010000)//prevent unreasonable decoded ID
                    break;
            }
            CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
        }
    }

    return (CurrentPSN);
}



#if (VCPS_TEST == 1)
/*******************************************************************************

    FUNCTION        dvdReadDKBHeader

    DESCRIPTION     This function reads DKB Header for a DVD +R/RW VCPS disc

    RETURNS         Status of buffering the first ECC block of DKB

********************************************************************************/
BYTE dvdReadDKBHeader()
{
    ULONG   ControlBlockAddress;
    DiscAccessStatusE DiscReadStatus;
    BYTE    *BufferPtr;

    if (vcpsDKB.State == DKB_INVALID)
        return (FAIL);
    else if (vcpsDKB.State == DKB_VALID || vcpsDKB.State == DKB_HEADER_VALID)
        return (PASS);

    while (vcpsDKB.State == DKB_NOT_READ)
    {

        ClearDataBuffer();      // Clear the DVD buffer

        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters for reading
            the lead in block
        ----------------------------------------------------------------*/
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetID.Long    = DKB_BUFFERZONE2_ADDRESS1;
        dvdDiscRequest.EccBlockCount    = 1L;
        dvdDiscRequest.TargetLayer      = eLayer0;

        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine to load the blocks
        ----------------------------------------------------------------*/
        DiscReadStatus = dvdStartDiscTransfer();

        /*---------------------------------------------------------------
            Check if the Lead In data has been properly buffered
        ----------------------------------------------------------------*/
        if ((DiscReadStatus == BUF_GOOD) || (ControlBlockAddress == DKB_BUFFERZONE2_ADDRESS2))
        {
            vcpsDKB.State = DKB_INVALID;
        }
        else /* DiscReadStatus != BUF_GOOD */
        {
            dvdResetErrorFlags();
            ControlBlockAddress = DKB_BUFFERZONE2_ADDRESS2; // DKB#2 position in Buffer Zone 2
        }
    }

    BufferPtr = (BYTE *)(SCSDRAM_START_ADDR);
    ControlBlockAddress = MAKE_LONG(*BufferPtr,*(BufferPtr+1),*(BufferPtr+2),*(BufferPtr+3));
    if (ControlBlockAddress == 0x56435053) // Check EKB Signature "VCPS_EKB"
    {
        ControlBlockAddress = MAKE_LONG(*(BufferPtr+4),*(BufferPtr+5),*(BufferPtr+6),*(BufferPtr+7));
        if(ControlBlockAddress == 0x5f454b42)
        {
            BYTE i;

            vcpsDKB.State = DKB_HEADER_VALID;
            // get DKB size
            vcpsDKB.size = MAKE_LONG(*(BufferPtr+8),*(BufferPtr+9),*(BufferPtr+10),*(BufferPtr+11));
            if (vcpsDKB.size <= 32768L)
            {
                vcpsDKB.State = DKB_VALID;

                // Copy DKB data to DKB_START_ADDRESS
                CopyBufferToBuffer(0, 0, DKB_START_PAGE, 0, ((USHORT)vcpsDKB.size + 2047)/2048);
            }

            // get DKB Sequence Number
            vcpsDKB.seq_num = MAKE_LONG(*(BufferPtr+12),*(BufferPtr+13),*(BufferPtr+14),*(BufferPtr+15));
            // get DKB Key Check Data
            BufferPtr += 24;
            for(i=0; i<16; i++)
                vcpsDKB.key_chk_data[i] = *(BufferPtr + i);

            // Save Unique ID
            BufferPtr = (BYTE *)(DVD_ABS_AUX_START_ADDR);
            BufferPtr += CPR_MAI_OFFSET + 1;
            vcpsData.UniqueID[11] = *BufferPtr++;
            vcpsData.UniqueID[12] = *BufferPtr++;
            vcpsData.UniqueID[13] = *BufferPtr++;
            vcpsData.UniqueID[14] = *BufferPtr++;
            vcpsData.UniqueID[15] = *BufferPtr;

            //Initial Unique ID and retrieve from the auxillary data
            for( i = 0; i < 11; i++)
                vcpsData.UniqueID[i] = 0;

            return (PASS);
        }
    }
    return (FAIL);
}
#endif  /* VCPS_TEST == 1 */

#if (ENABLE_AACS == 1)
/*****************************************************************************

    FUNCTION        dvdReadMkbFromDisc

    DESCRIPTION     Read MKB

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL dvdReadMkbFromDisc(void)
{
    BYTE i, MkbCount;
    BYTE *DataPtr;
    vBYTE *FlashAddr;
    DiscAccessStatusE result;

    //send_msg5S(SEND_SStpTask_MSG, "Read MKB");
    send_msg80(SEND_SStpTask_MSG,0x700123);

    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    FlashAddr = (vBYTE *)FLASH_BASE + MKB_BLK_START_OFFSET + 8;
    MkbVersionOnFlash = SWAPEND_ULONG(*((ULONG *)FlashAddr));

    MkbCount = 4;

    for (i = 0; i < MkbCount; i++)
    {
        ClearDataBuffer();
        dvdDiscRequest.Type             = INIT_READ;
        dvdDiscRequest.TargetLayer      = eLayer0;
        dvdDiscRequest.TargetID.Long    = DVDROM_MKB_Info[i].PSN;
        dvdDiscRequest.EccBlockCount    = DVDROM_MKB_Info[i].LEN;


        result = dvdStartDiscTransfer();
        if (result == BUF_GOOD)
            break;
    }

    if (result == BUF_GOOD)
    {
        CopyBufferToBuffer(2, 0, 32, 0, 14);
        CopyBufferToBuffer(18, 0, 46, 0, 2);

        DataPtr = (BYTE *)(SCSDRAM_ADDRESS + 32*0x800);

        // Validate Type and Version Record
        if (ValidateTypeAndVersionRecord(DataPtr) == TRUE)
        {
            DataPtr = (BYTE *)(SCSDRAM_ADDRESS + 32*0x800 + HOST_REVOCATION_LIST_RECORD_OFFSET);

            // Validate Host Revocation List Record
            if (ValidateHrlRecord(DataPtr) == TRUE)
            {
                AacsStatus.AacsFlag.IsHrlValid = TRUE;

                DataPtr = (BYTE *)(SCSDRAM_ADDRESS + 32*0x800 + 8);    // Point to the Version Number field
                MkbVersionOnDisc = SWAPEND_ULONG(*((ULONG *)DataPtr));

                SendMsgCn(SEND_SStpTask_MSG,8,0x800491, A4B(MkbVersionOnFlash), A4B(MkbVersionOnDisc));

                if (MkbVersionOnDisc > MkbVersionOnFlash)
                {
                    MkbVersionOnFlash = MkbVersionOnDisc;
                    CloseWritableSegmentTable(eMKB_Table);
                }
            }
        }

        //send_msg5S(SEND_SStpTask_MSG, "MKB Good");
        send_msg80(SEND_SStpTask_MSG,0x700124);
        return PASS;
    }
    else
        return FAIL;
}

/*****************************************************************************

    FUNCTION        dvdReadVolumeIDFromDisc

    DESCRIPTION     Read Volume ID

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL dvdReadVolumeIDFromDisc(void)
{
    BYTE *DataPtr;
    BOOL IsCorrected;

    //send_msg5S(SEND_SStpTask_MSG, "Read VID");
    send_msg80(SEND_SStpTask_MSG,0x70011D);

    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    dvdDiscRequest.Type             = INIT_READ;
    dvdDiscRequest.TargetLayer      = eLayer0;
    dvdDiscRequest.TargetID.Long    = MID_VOLUME_ID_START_ADDRESS;
    dvdDiscRequest.EccBlockCount    = 10;

    fReadingMid = TRUE;
    gMidKeyIndex = 0;
    (void)dvdStartDiscTransfer();
    fReadingMid = FALSE;

    DataPtr = (BYTE *)SCSDRAM_ADDRESS;
    IsCorrected = ExtractMid(DataPtr, MID_READ_VID);

    if (IsCorrected == TRUE)
    {
        //send_msg5S(SEND_SStpTask_MSG, "VID Good");
        send_msg80(SEND_SStpTask_MSG,0x70011E);
        return PASS;
    }
    else
    {
        //send_msg5S(SEND_SStpTask_MSG, "VID Fail");
        send_msg80(SEND_SStpTask_MSG,0x70011F);
        return FAIL;
    }
}

/*****************************************************************************

    FUNCTION        dvdReadKcdFromDisc

    DESCRIPTION     Read Key Conversion Data

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL dvdReadKcdFromDisc(void)
{
    BYTE *DataPtr;
    BOOL IsCorrected;

    //send_msg5S(SEND_SStpTask_MSG, "Read KCD");
    send_msg80(SEND_SStpTask_MSG,0x700120);

    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    dvdDiscRequest.Type             = INIT_READ;
    dvdDiscRequest.TargetLayer      = eLayer0;
    dvdDiscRequest.TargetID.Long    = MID_KCD_START_ADDRESS;
    dvdDiscRequest.EccBlockCount    = 10;

    fReadingMid = TRUE;
    gMidKeyIndex = 0;
    (void)dvdStartDiscTransfer();
    fReadingMid = FALSE;

    DataPtr = (BYTE *)SCSDRAM_ADDRESS;
    IsCorrected = ExtractMid(DataPtr, MID_READ_KCD);

    if (IsCorrected == TRUE)
    {
        //send_msg5S(SEND_SStpTask_MSG, "KCD Good");
        send_msg80(SEND_SStpTask_MSG,0x700121);
        return PASS;
    }
    else
    {
        //send_msg5S(SEND_SStpTask_MSG, "KCD Fail");
        send_msg80(SEND_SStpTask_MSG,0x700122);
        return FAIL;
    }
}
#endif  /* ENABLE_AACS == 1 */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
