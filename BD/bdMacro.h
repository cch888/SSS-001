/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdMacro.h $
*
* DESCRIPTION   This file contains BD related macros
*
* $Revision: 18 $
* $Date: 09/12/09 4:10p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BD_MACRO_H__
#define __BD_MACRO_H__


#include ".\AL\REG_HAL.h"
#include ".\AL\REG_bd_codec.h"
#include ".\Al\HAL_bd_codec.h"
#include ".\bd\bdDef.h"

//**********************************************************************************************
//
//                                 Macros for BD code
//
//**********************************************************************************************
#define pdbGetBDLayers()            ((Info.Media.BD.DiscInfoP_L0_Fmt1->BDStructure & BD_LAYER_MASK) >> 4)
#define pdbGetBDType()              (Info.Media.BD.DiscInfoP_L0_Fmt1->BDStructure & BD_TYPE_MASK)

//BD-R/RE
#define pdbGetBDRReDdsUpdateCount()    (SWAPEND_ULONG(pDDS->BdRReDds.UpdateCnt))
#define pdbSetBDRReDdsUpdateCount(x)   (pDDS->BdRReDds.UpdateCnt = SWAPEND_ULONG(x))
#define pdbGetBDRReDflPSN()            (SWAPEND_ULONG(pDDS->BdRReDds.PSN_DFL))
#define pdbSetBDRReDflPSN(x)           (pDDS->BdRReDds.PSN_DFL = SWAPEND_ULONG(x));
#define pdbGetBDRReLsn0StartPSN()      (SWAPEND_ULONG(pDDS->BdRReDds.Start_LSN0_PSN))
#define pdbSetBDRReLsn0StartPSN(x)     (pDDS->BdRReDds.Start_LSN0_PSN = SWAPEND_ULONG(x))
#define pdbGetBDRReEndLSN()            (SWAPEND_ULONG(pDDS->BdRReDds.Last_LSN))
#define pdbSetBDRReEndLSN(x)           (pDDS->BdRReDds.Last_LSN = SWAPEND_ULONG(x))
#define pdbGetBDRReIsa0Size()          (SWAPEND_ULONG(pDDS ->BdRReDds.ISA0_Size))
#define pdbSetBDRReIsa0Size(x)         (pDDS->BdRReDds.ISA0_Size = SWAPEND_ULONG(x))
#define pdbGetBDRReOsaSize()           (SWAPEND_ULONG(pDDS->BdRReDds.OSA_Size))
#define pdbSetBDRReOsaSize(x)          (pDDS->BdRReDds.OSA_Size = SWAPEND_ULONG(x))
#define pdbGetBDRReIsa1Size()          (SWAPEND_ULONG(pDDS->BdRReDds.ISA1_Size))
#define pdbSetBDRReIsa1Size(x)         (pDDS->BdRReDds.ISA1_Size = SWAPEND_ULONG(x))

//IH-BD
#define pdbGetIHBDDdsUpdateCount()    (SWAPEND_ULONG(pDDS->IhBdDds.UpdateCnt))
#define pdbSetIHBDDdsUpdateCount(x)   (pDDS->IhBdDds.UpdateCnt = SWAPEND_ULONG(x))
#define pdbGetIHBDDflPSN()            (SWAPEND_ULONG(pDDS->IhBdDds.PSN_DFL))
#define pdbSetIHBDDflPSN(x)           (pDDS->IhBdDds.PSN_DFL = SWAPEND_ULONG(x))
#define pdbGetIHBDLsn0StartPSN()      (SWAPEND_ULONG(pDDS->IhBdDds.Start_LSN0_PSN))
#define pdbSetIHBDLsn0StartPSN(x)     (pDDS->IhBdDds.Start_LSN0_PSN = SWAPEND_ULONG(x))
#define pdbGetIHBDEndLSN()            (SWAPEND_ULONG(pDDS->IhBdDds.Last_LSN))
#define pdbSetIHBDEndLSN(x)           (pDDS->IhBdDds.Last_LSN = SWAPEND_ULONG(x))
#define pdbGetIHBDIsa0Size()          (SWAPEND_ULONG(pDDS ->IhBdDds.ISA0_Size))
#define pdbSetIHBDIsa0Size(x)         (pDDS->IhBdDds.ISA0_Size = SWAPEND_ULONG(x))
#define pdbGetIHBDOsaSize()           (SWAPEND_ULONG(pDDS->IhBdDds.OSA_Size))
#define pdbSetIHBDOsaSize(x)          (pDDS->IhBdDds.OSA_Size = SWAPEND_ULONG(x))
#define pdbGetIHBDIsa1Size()          (SWAPEND_ULONG(pDDS->IhBdDds.ISA1_Size))
#define pdbSetIHBDIsa1Size(x)         (pDDS->IhBdDds.ISA1_Size = SWAPEND_ULONG(x))

//BD-R/RE/IH
#define pdbGetBDDdsUpdateCount()    ((pdbGetIHBDDisc()) ? pdbGetIHBDDdsUpdateCount() : pdbGetBDRReDdsUpdateCount())
#define pdbSetBDDdsUpdateCount(x)   ((pdbGetIHBDDisc()) ? pdbSetIHBDDdsUpdateCount(x) : pdbSetBDRReDdsUpdateCount(x))
#define pdbGetBDDflPSN()            ((pdbGetIHBDDisc()) ? pdbGetIHBDDflPSN() : pdbGetBDRReDflPSN())
#define pdbSetBDDflPSN(x)           ((pdbGetIHBDDisc()) ? pdbSetIHBDDflPSN(x) : pdbSetBDRReDflPSN(x))
#define pdbGetBDLsn0StartPSN()      ((pdbGetIHBDDisc()) ? pdbGetIHBDLsn0StartPSN() : pdbGetBDRReLsn0StartPSN())
#define pdbSetBDLsn0StartPSN(x)     ((pdbGetIHBDDisc()) ? pdbSetIHBDLsn0StartPSN(x) : pdbSetBDRReLsn0StartPSN(x))
#define pdbGetBDEndLSN()            ((pdbGetIHBDDisc()) ? pdbGetIHBDEndLSN() : pdbGetBDRReEndLSN())
#define pdbSetBDEndLSN(x)           ((pdbGetIHBDDisc()) ? pdbSetIHBDEndLSN(x) : pdbSetBDRReEndLSN(x))
#define pdbGetBDIsa0Size()          ((pdbGetIHBDDisc()) ? pdbGetIHBDIsa0Size() : pdbGetBDRReIsa0Size())
#define pdbSetBDIsa0Size(x)         ((pdbGetIHBDDisc()) ? pdbSetIHBDIsa0Size(x) : pdbSetBDRReIsa0Size(x))
#define pdbGetBDOsaSize()           ((pdbGetIHBDDisc()) ? pdbGetIHBDOsaSize() : pdbGetBDRReOsaSize())
#define pdbSetBDOsaSize(x)          ((pdbGetIHBDDisc()) ? pdbSetIHBDOsaSize(x) : pdbSetBDRReOsaSize(x))
#define pdbGetBDIsa1Size()          ((pdbGetIHBDDisc()) ? pdbGetIHBDIsa1Size() : pdbGetBDRReIsa1Size())
#define pdbSetBDIsa1Size(x)         ((pdbGetIHBDDisc()) ? pdbSetIHBDIsa1Size(x) : pdbSetBDRReIsa1Size(x))

//BD-R
#define pdbGetBDRecMode()           ((pDDS->BdRReDds.RecMode) & REC_MODE_MASK)
#define pdbSetBDRecMode(x)          (pDDS->BdRReDds.RecMode = (x & REC_MODE_MASK))
#define pdbGetBDFinalizeFlag()      ((pDDS->BdRReDds.GenFlag) & 0x02)
#define pdbGetBDWrtProtect()        ((pDDS->BdRReDds.GenFlag) & 0x01)
#define pdbGetBDUserLastPSN()       (SWAPEND_ULONG(pDDS->BdRReDds.LstRecordedAddr))
#define pdbSetBDUserLastPSN(x)      (pDDS->BdRReDds.LstRecordedAddr = SWAPEND_ULONG(x))
#define pdbGetBDIsa0TdmaSize()      (SWAPEND_ULONG(pDDS->BdRReDds.ISA0_TDMASize))
#define pdbSetBDIsa0TdmaSize(x)     (pDDS->BdRReDds.ISA0_TDMASize = SWAPEND_ULONG(x))
#define pdbGetBDOsaTdmaSize()       (SWAPEND_ULONG(pDDS->BdRReDds.OSA_TDMASize))
#define pdbSetBDOsaTdmaSize(x)      (pDDS->BdRReDds.OSA_TDMASize = SWAPEND_ULONG(x))
#define pdbGetBDIsa1TdmaSize()      (SWAPEND_ULONG(pDDS->BdRReDds.ISA1_TDMASize))
#define pdbSetBDIsa1TdmaSize(x)     (pDDS->BdRReDds.ISA1_TDMASize = SWAPEND_ULONG(x))
#define pdbGetBDDfl1PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_1DFL))
#define pdbSetBDDfl1PSN(x)          (pDDS->BdRReDds.PSN_1DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl2PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_2DFL))
#define pdbSetBDDfl2PSN(x)          (pDDS->BdRReDds.PSN_2DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl3PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_3DFL))
#define pdbSetBDDfl3PSN(x)          (pDDS->BdRReDds.PSN_3DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl4PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_4DFL))
#define pdbSetBDDfl4PSN(x)          (pDDS->BdRReDds.PSN_4DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl5PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_5DFL))
#define pdbSetBDDfl5PSN(x)          (pDDS->BdRReDds.PSN_5DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl6PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_6DFL))
#define pdbSetBDDfl6PSN(x)          (pDDS->BdRReDds.PSN_6DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl7PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_7DFL))
#define pdbSetBDDfl7PSN(x)          (pDDS->BdRReDds.PSN_7DFL = SWAPEND_ULONG(x))
#define pdbGetBDDfl8PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_8DFL))
#define pdbSetBDDfl8PSN(x)          (pDDS->BdRReDds.PSN_8DFL = SWAPEND_ULONG(x))
#define pdbGetBDIsa0NextPSN()       (SWAPEND_ULONG(pDDS->BdRReDds.ISA0_APSN))
#define pdbSetBDIsa0NextPSN(x)      (pDDS->BdRReDds.ISA0_APSN = SWAPEND_ULONG(x))
#define pdbGetBDIsa1NextPSN()       (SWAPEND_ULONG(pDDS->BdRReDds.ISA1_APSN))
#define pdbSetBDIsa1NextPSN(x)      (pDDS->BdRReDds.ISA1_APSN = SWAPEND_ULONG(x))
#define pdbGetBDOsa0NextPSN()       (SWAPEND_ULONG(pDDS->BdRReDds.OSA0_APSN))
#define pdbSetBDOsa0NextPSN(x)      (pDDS->BdRReDds.OSA0_APSN = SWAPEND_ULONG(x))
#define pdbGetBDOsa1NextPSN()       (SWAPEND_ULONG(pDDS->BdRReDds.OSA1_APSN))
#define pdbSetBDOsa1NextPSN(x)      (pDDS->BdRReDds.OSA1_APSN = SWAPEND_ULONG(x))
#define pdbGetBDTz0APSN()           (SWAPEND_ULONG(pDDS->BdRReDds.TestZoneL0_APSN))
#define pdbSetBDTz0APSN(x)          (pDDS->BdRReDds.TestZoneL0_APSN = SWAPEND_ULONG(x))
#define pdbGetBDTz1APSN()           (SWAPEND_ULONG(pDDS->BdRReDds.TestZoneL1_APSN))
#define pdbSetBDTz1APSN(x)          (pDDS->BdRReDds.TestZoneL1_APSN = SWAPEND_ULONG(x))
#define pdbGetBDSrriSbm0PSN()       (SWAPEND_ULONG(pDDS->BdRReDds.PSN_SRRI_L0))
#define pdbSetBDSrriSbm0PSN(x)      (pDDS->BdRReDds.PSN_SRRI_L0 = SWAPEND_ULONG(x))
#define pdbGetBDSbm1PSN()           (SWAPEND_ULONG(pDDS->BdRReDds.PSN_SBM_L1))
#define pdbSetBDSbm1PSN(x)          (pDDS->BdRReDds.PSN_SBM_L1 = SWAPEND_ULONG(x))

#define pdbGetBDClusterStart(x)     (x & BD_CLUSTER_MASK)

//------------------------------------------------------------------------------
// Macros to access BD logical information
//------------------------------------------------------------------------------
#define pdbGetBDMaxLBA()                            (Info.Media.BD.MaxLBA)
#define pdbSetBDMaxLBA(x)                           (Info.Media.BD.MaxLBA = x)
#define pdbGetBDEndLBALayer0()                      (Info.Media.BD.EndLBALayer0)

//------------------------------------------------------------------------------
// Macros to access Last Session/Track information
//------------------------------------------------------------------------------
#define pdbSetBDFinalSessCloseInfo(x)               (Iram_FinalSessionInfo.BD.CloseInfo = x)
#define pdbGetBDFinalSessCloseInfo()                (Iram_FinalSessionInfo.BD.CloseInfo)
#define pdbSetBDFinalSessionNumber(x)               (Iram_FinalSessionInfo.BD.SessionNumber = x)
#define pdbGetBDFinalSessionNumber()                (Iram_FinalSessionInfo.BD.SessionNumber)
#define pdbSetBDFinalSessFirstTrackNumber(Tno)      (Iram_FinalSessionInfo.BD.FirstTrackNumber = Tno)
#define pdbGetBDFinalSessFirstTrackNumber()         (Iram_FinalSessionInfo.BD.FirstTrackNumber)
#define pdbSetBDFinalSessLastTrackNumber(Tno)       (Iram_FinalSessionInfo.BD.LastTrackNumber = Tno)
#define pdbGetBDFinalSessLastTrackNumber()          (Iram_FinalSessionInfo.BD.LastTrackNumber)
#define pdbSetBDFinalSessFirstTrackPSN(ADDR)        (Iram_FinalSessionInfo.BD.FirstTrackPSN = ADDR)
#define pdbGetBDFinalSessFirstTrackPSN()            (Iram_FinalSessionInfo.BD.FirstTrackPSN)
#define pdbSetBDFinalSessFormat(format)             (Iram_FinalSessionInfo.BD.Format = format)
#define pdbGetBDFinalSessFormat()                   (Iram_FinalSessionInfo.BD.Format)

//------------------------------------------------------------------------------
// Macros to access Session information
//------------------------------------------------------------------------------
#define pdbBDIncrementSessionCount()                (++Info.Media.BD.SessionCounter)
#define pdbSetBDSessionCount(x)                     (Info.Media.BD.SessionCounter = x)
#define pdbGetBDSessionCount()                      (Info.Media.BD.SessionCounter)

//------------------------------------------------------------------------------
// Macros to access Track information
//------------------------------------------------------------------------------
#define pdbBDIncreaseHighestRecordedTrackNum()      (++Info.Media.BD.LogicalTracks)
#define pdbSetBDHighestRecordedTrackNumber(x)       Info.Media.BD.LogicalTracks = x
#define pdbGetBDHighestRecordedTrackNumber()        (Info.Media.BD.LogicalTracks)
#define pdbSetBDLowestRecordedTrackNumber(x)        (Info.Media.BD.LowestRecordedTrackNumber = x)
#define pdbGetBDLowestRecordedTrackNumber()         (Info.Media.BD.LowestRecordedTrackNumber)


/***** Common Macros *****/
/* Get - Final Session Leadin/Leadout PSN */
#define pdbGetBDFinalSessLeadinPSN()                (Iram_FinalSessionInfo.BD.LeadinPSN)
#define pdbGetBDFinalSessLeadoutPSN()               (Iram_FinalSessionInfo.BD.LeadoutPSN)

/* Get - Next Session Start PSN */
#define pdbGetBDNextSessStartPSN()                  (Iram_FinalSessionInfo.BD.NextSessionStartPSN)

/* Set/Get - BD Track Status */
#define pdbSetBDTrackStatus(Tno, info)              (Info.Media.BD.Track[Tno-1].TrackStatus = info)
#define pdbGetBDTrackStatus(Tno)                    (Info.Media.BD.Track[Tno-1].TrackStatus)

/* Set/Get - BD Track State */
#define pdbSetBDTrackState(Tno, info)               (Info.Media.BD.Track[Tno-1].TrackState = info)
#define pdbGetBDTrackState(Tno)                     (Info.Media.BD.Track[Tno-1].TrackState)

/* Set/Get - BD Track Diag Status 0 */
#define pdbSetBDTrackDiagStatus(Tno, info)          (Info.Media.BD.Track[Tno-1].TrackDiagStatus0 = info)
#define pdbGetBDTrackDiagStatus(Tno)                (Info.Media.BD.Track[Tno-1].TrackDiagStatus0)

/* Set/Get - BD Track Diag Status 1 */
#define pdbSetBDTrackDiagStatus2(Tno, info)         (Info.Media.BD.Track[Tno-1].TrackDiagStatus1 = info)
#define pdbGetBDTrackDiagStatus2(Tno)               (Info.Media.BD.Track[Tno-1].TrackDiagStatus1)

/* Set/Get - BD Track Session Number */
#define pdbSetBDTrackSessionNumber(Tno, info)       (Info.Media.BD.Track[Tno-1].TrackSessionNumber = info)
#define pdbGetBDTrackSessionNumber(Tno)             (Info.Media.BD.Track[Tno-1].TrackSessionNumber)

/* Set/Get - BD Track Start PSN */
#define pdbSetBDTrackStartPSN(Tno, info)            (Info.Media.BD.Track[Tno-1].StartPSN.PSN = info)
#define pdbGetBDTrackStartPSN(Tno)                  (Info.Media.BD.Track[Tno-1].StartPSN.PSN)

/* Set/Get - BD Track Start PSN Layer */
#define pdbSetBDTrackStartPSNLayer(Tno, info)       (Info.Media.BD.Track[Tno-1].StartPSN.Layer = info)
#define pdbGetBDTrackStartPSNLayer(Tno)             (Info.Media.BD.Track[Tno-1].StartPSN.Layer)

/* Set/Get - BD Track End PSN */
#define pdbSetBDTrackEndPSN(Tno, info)              (Info.Media.BD.Track[Tno-1].EndPSN.PSN = info)
#define pdbGetBDTrackEndPSN(Tno)                    (Info.Media.BD.Track[Tno-1].EndPSN.PSN)

/* Set/Get - BD Track End PSN Layer */
#define pdbSetBDTrackEndPSNLayer(Tno, info)         (Info.Media.BD.Track[Tno-1].EndPSN.Layer = info)
#define pdbGetBDTrackEndPSNLayer(Tno)               (Info.Media.BD.Track[Tno-1].EndPSN.Layer)

/* Get - BD Track Recorded PSN */
#define pdbGetBDTrackRecordedPSN(Tno)               (Info.Media.BD.Track[Tno-1].RecordedPSN.PSN)

/* Set/Get - BD Track Next Writable Address (LBA) */
#define pdbSetBDTrackNextWritableAddress(Tno, info) (Info.Media.BD.Track[Tno-1].NextWritableLBA = info)
#define pdbGetBDTrackNextWritableAddress(Tno)       (Info.Media.BD.Track[Tno-1].NextWritableLBA)

//------------------------------------------------------------------------------
// Macros to convert ADIP address to PSN
//------------------------------------------------------------------------------
#define pdbGetBDL0PhysicalEndAddr()     ((pdbGetBDL0EndPAA() << 3) + 0x0F)
#define pdbGetBDL1PhysicalEndAddr()     ((pdbGetBDL1EndPAA() << 3) + 0x0F)
#define pdbGetBDL1PhysicalStartAddr()   (pdbGetBDL1StartPAA() << 3)

//------------------------------------------------------------------------------
// Macros to convert L0 PSN to L1 PSN
//------------------------------------------------------------------------------
#define ConvertPSNL0ToL1(L0_PSN)        (~(L0_PSN + 0xFE000000))

//------------------------------------------------------------------------------
// Macros to access SRRI
//------------------------------------------------------------------------------
#define pdbGetBDRSrriUpdateCounter()    (SWAPEND_ULONG(pSRRI->UpdateCnt))
#define pdbSetBDRSrriUpdateCounter(x)   (pSRRI->UpdateCnt = SWAPEND_ULONG(x))
#define pdbGetBDRSrrEntryNumber()       (SWAPEND_ULONG(pSRRI->SRRI_Cnt))
#define pdbSetBDRSrrEntryNumber(x)      (pSRRI->SRRI_Cnt = SWAPEND_ULONG(x))
#define pdbGetBDROpenSrrsNumber()       (pSRRI->Open_Cnt)
#define pdbSetBDROpenSrrsNumber(x)      (pSRRI->Open_Cnt = x)


#define pdbSetBDClusterAddress(x)       (Iram_DiscInfo.BD.ClusterAddress = x)
#define pdbGetBDClusterAddress()        (Iram_DiscInfo.BD.ClusterAddress)

/*-----------------------------------------------------------------
    READ TIMER MACROS
------------------------------------------------------------------*/
#define START_BD_CLUSTER_TIMER(x)   bdClusterInterruptTimer.count = x;\
                                    bdClusterInterruptTimer.fTIMEOUT = CLEAR;\
                                    bdClusterInterruptTimer.state = ON

#define STOP_BD_CLUSTER_TIMER()     bdClusterInterruptTimer.state = OFF

/*-----------------------------------------------------------------
    BD BUFERING MACROS
------------------------------------------------------------------*/
#define BD_BUFFERING()              ((HAL_BD_CODEC_GET_FD_HOLD_STATUS())? OFF : ON)

/*-----------------------------------------------------------------
    MACRO TO OBTAIN ABSOLUTE ADDRESS OF CURRENT BD BIS BLOCK
    AND CURRENT BD BLOCK BASED ON SOFTWARE POINTER DataBuffer.CurDiscPtr
------------------------------------------------------------------*/
#define BD_CUR_ECC_BLK(ptr)     (ptr ? (ptr - ONE_BD_CLUSTER) : (BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER))
#define BD_CUR_AUX_ADDRESS()    ((BUF_TOTAL_BLK_COUNT * BD_DATA_BLK_PTR_RESOLUTION) + ((BD_CUR_ECC_BLK(DataBuffer.CurDiscPtr)/ONE_BD_CLUSTER) * BD_AUX_PTR_RESOLUTION))
#define BD_CUR_ECC_ADDRESS()    (SCSDRAM_START_ADDR + (CUR_ECC_BLK(DataBuffer.CurDiscPtr) * BD_DATA_BLK_PTR_RESOLUTION))
#define BD_CUR_EDC_STATUS()     ((*((BYTE *) (SCSDRAM + BD_CUR_AUX_ADDRESS() + 0x290))) & bit5)
/*-----------------------------------------------------------------
    MACRO TO OBTAIN ABSOLUTE ADDRESS OF BD BIS BLOCK AND LDC BLOCKS
    x = Relative data block pointer
------------------------------------------------------------------*/
#define BD_AUX_ADDRESS(x)       ((BUF_TOTAL_BLK_COUNT * BD_DATA_BLK_PTR_RESOLUTION) + ((x >> 5) * BD_AUX_PTR_RESOLUTION))
#define BD_CLUSTER_ADDRESS(x)   (SCSDRAM_START_ADDR + (x * BD_DATA_BLK_PTR_RESOLUTION))

//------------------------------------------------------------------------------
// Macros to get and set BD FlushMessage
//------------------------------------------------------------------------------
#define BDGetMessage()  (BdwtDisc.FlushMessage)
#define BDPostMessage(Msg) (BdwtDisc.FlushMessage = Msg)
#endif

