/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdDB.h $
*
* DESCRIPTION   Header File for bdDB.c
*
* $Revision: 7 $
* $Date: 09/08/20 6:55p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/


/*********************************************************************
    FILE DEFINITION
*********************************************************************/
#ifndef __BDDB_H__
#define __BDDB_H__

#include ".\common\globtype.h"
//#include ".\common\globals.h"
#include ".\hif\Gp_mac.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"

/*******************************************************************
    EXTERNAL FUNCTIONS / FUNCTION PROTOTYPES
*******************************************************************/
extern ULONG pdbGetBDMaxPSN(BYTE layer);
extern ULONG pdbGetBDStartPSN(BYTE layer);
extern void  pdbSetBDMaxPSN(BYTE layer, ULONG PSN);
extern ULONG pdbGetBDL0EndPAA(void);
extern ULONG pdbGetBDL1EndPAA(void);
extern ULONG pdbGetBDL1StartPAA(void);
extern BOOL  pdbGetBDBlankDiscInfo(void);
extern BYTE  pdbGetBDClosedSessions(void);
extern ULONG pdbGetBDFinalSessLeadinLBA(void);
extern ULONG pdbGetBDFinalSessLeadoutLBA(void);
extern ULONG pdbGetBDLeadOutStartPSN(void);
extern void  pdbSetBDLeadOutStartPSN(ULONG start);
extern BYTE  pdbGetBDRW(void);
extern BYTE  pdbGetBDTrackConAddr(BYTE Tno);
extern tTrackDataModeT pdbGetBDTrackDataMode(BYTE Tno);
extern LONG  pdbGetBDTrackPacketSize(BYTE Tno);
extern void  pdbSetBDTrackPacketSize(BYTE Tno,LONG PacketSize);
extern void  pdbInitBDDataBase(void);
extern void  pdbSetBDCopyrightStatus(BYTE Tno, CopyModeT Mode);
extern void  pdbSetBDFinalSessLeadinPSN(ULONG ADDR);
extern void  pdbSetBDFinalSessLeadoutPSN(ULONG ADDR);
extern void  pdbSetBDLogicalData();
extern void  pdbSetBDNextSessStartPSN(ULONG ADDR);
extern void  pdbSetBDTrackConAddr(BYTE Tno,BYTE ControlAddress);
extern void  pdbSetBDTrackContAddr(BYTE Tno, BYTE ContAddr);
extern void  pdbSetBDTrackDataMode(BYTE Tno,tTrackDataModeT DataNibble);
extern void  pdbSetBDTrackRecordedPSN(BYTE Tno, ULONG info);
extern BYTE pdbGetBDHybridDiscId(BYTE layer);
extern char * pdbGetBDDiscManufacturerID(void);
extern ULONG pdbGetSRRIEntryStartPSN(USHORT idx);
extern void pdbSetSRRIEntryStartPSN(USHORT idx, ULONG StartPSN);
extern ULONG pdbGetSRRIEntryLRA(ULONG idx);
extern void pdbSetSRRIEntryLRA(USHORT idx, ULONG LraPSN);

#endif
