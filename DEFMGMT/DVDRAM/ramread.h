/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC
*                           All Right Reserved
*
* FILENAME:   ramread.h
*
*   This file is the header file for RamRead.c

* DESCRIPTION
*
* Notes:
*
*
*  $Revision: 2 $
*  $Date: 10/02/03 1:10a $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _RAMREAD_H_
#define _RAMREAD_H_

/* Functions */
extern void  RamCheckDefectiveSector(void);
extern void  DvdRamVerifyProcess(ULONG StartPSN, ULONG BlockCount);
extern BOOL  DvdRamVerifyEccBlock(ULONG StartPSN, ULONG BlockCount);
extern void  RamStopVerifying(void);
extern void  SwitchBackDVDRAMBuffer(void);
extern ULONG ConvertLSNToPSNForSAOnly(USHORT Offset);
extern void  CalcPrimarySAEmptySpace(void);
extern BOOL  RamReadModifyWrite(ULONG);
extern void  RamCheckDefectiveBlock(void);
extern void  DVDRAMReadCommand(void);


/* Variables */


/* Define */
typedef struct
{
     ULONG  StartLba;
     USHORT SectorCnt;
} FragTransferType;


#endif
