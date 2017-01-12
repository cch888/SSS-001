/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_util.h
*
* DESCRIPTION
* This file contains general utility routines
*
* $Revision: 16 $
* $Date: 09/08/24 2:20p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_UTIL_H__
#define __GP_UTIL_H__

#include ".\common\error.h"
#include ".\player\plrdb.h"

typedef struct
{
    BOOL valid;
    BYTE ContAddr;
    BYTE WriteMethod;
    BYTE Mode2Form;
    long PacketSize;
    BYTE DataMode;
    BOOL buff_NG;
}TDBInfo;

#define HOST            0
#define DECODER         1
#define ENCODER         2
#define BYTES_PER_PAGE  0x800

#define SpdChgLowest2HighTime   180000 //every 180 seconds
extern BOOL CheckDiscMaxLBA(long);
extern void GetMsfFromServo(TimeS *, BYTE *);
extern void CheckAudioOperation(void);
extern vBOOL CheckForFF(BYTE *, BYTE);
extern vBOOL CheckValidMSF(BYTE *);
extern BYTE CheckTrackWriteMethod(long);
extern vBOOL Method2LBAToMethod1LBA(long, long *);
extern vBOOL ReadPreGapTDB(long, BYTE);
extern void Set_SpeedMDCont(void);
extern void Set_DVDSpeedMDCont(void);
#if (BD_ENABLE == 1)
extern void Set_BDSpeedMDCont();
#endif  /* BD_ENABLE == 1 */
extern void RecoverDecoderError(void);
extern void UpdateDiscInfoData(void);
extern BYTE FindTrackNumber(long);
extern void CopyPersistentTableToDram(BYTE);
extern ULONG CopyAacsKeysToDram(BYTE *KeySize);
extern void LoadMkbFromFlash(void);
extern BYTE CheckFinalSessionClosed(void);
extern void LinearPatternFill(USHORT trgpage,USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern);
extern BYTE GetNextSpeed(BYTE, SBYTE, const BYTE*, BYTE);
extern BYTE FindSpeedFromWriteToRead(void);
BYTE CDReturnMediaOffsetForRead(void);
ULONG CNVTPageToLinear( USHORT  PageNumber, USHORT  PageOffset, BYTE    ModeType );
extern BYTE CompareStringInMemory(const BYTE*, const char*, USHORT);
extern BYTE SpecialDiscInformationParser(const BYTE*);
#endif /* __GP_UTIL_H__ */
