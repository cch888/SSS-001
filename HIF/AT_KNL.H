/****************************************************************************
*                (c) Copyright 2001,2002, 2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   at_knl.h
*
* DESCRIPTION
*   This file contains kernel and schedule routines
*
* NOTES:
*
************************ SUNEXT CONFIDENTIAL *******************************/

#ifndef __AT_KNL_H__
#define __AT_KNL_H__

extern void SCStartUp(void);
extern void AutoCommandComplete(void);
extern void InvalidCommand(void);
extern void SendOPCInfoCommand(void);
extern void ClearOPCDoneFlags(void);
extern void ReadBufferCapacityCommand(void);
extern void CheckCommandAvail(void);
extern void DisableAllInterrupts(void);

void CDReserveTrackCommand(void);
void CDCloseSessionTrackCommand(void);
void CDReadDiscInfoCommand(void);
void CDReadTrackInfoCommand(void);
void CDSyncCacheCommand(void);
void ReadBufferCapacityCommand(void);
void CDBlankCommand(void);
void CDFormatUnitCommand(void);
void CheckReadAhead(void);

extern void SyncCacheCommand(void);
extern void WriteCommand(void) ;
extern void FormatUnitCommand(void);
extern void ReserveTrackCommand(void);
extern void CloseSessionTrackCommand(void);
extern void ReadDiscInfoCommand(void);
extern void ReadTrackInfoCommand(void);
extern void BlankCommand(void);
extern void EraseProcess(void);
extern void Erase10Command(void);
extern void Erase10Process(void);
extern void FormatProcess(void);
extern void CloseProcess(void);
#endif /* __AT_KNL_H__ */
