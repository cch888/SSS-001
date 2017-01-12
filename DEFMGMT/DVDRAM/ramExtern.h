/***************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   ramExtern.h
*
* DESCRIPTION
*   External functions declaration for DVD-RAM
*
* $Revision: 3 $
* $Date: 10/02/21 11:37a $
*
**************************** SUNEXT CONFIDENTIAL ***************************/

#ifndef _RAMEXTERN_H_
#define _RAMEXTERN_H_

    #if (DVD_RAM_READ == 1)

#include ".\defmgmt\dvdram\ramdef.h"


/******************************************************
   External Memory Variable Definitions
*******************************************************/

/* ramNewCache.c */
extern  void    RAMcmInit(void);
extern  void    RAMInitSACache(void);
extern  void    RAMcmResetCacheHitList(void);
extern  BOOL    RAMcmAddCacheHitNode(LONG StartLSN, USHORT Cnt, BYTE HitFlag, BYTE SegNo);
extern  RamCacheListT * RAMcmGetCacheHitNode(USHORT index);
extern  USHORT  RAMcmGetFilledInfo(USHORT *pBitMap, BYTE *pOffset);
extern  void    RAMSetNextInfo(LONG LSN, USHORT Length);
extern  USHORT  RAMcmFindTargetLSN(LONG StartLSN, USHORT Count, BYTE Access);
extern  BYTE    RAMcmGetNextAvailableSegment(BYTE AccessType);
extern  void    RAMcmFreeSegment(BYTE SegNo);
extern  void    RAMcmCopyPage(USHORT SrcPage, USHORT TgtPage, USHORT PageCount);
extern  HashListT *RAMcmGetNextHashListNode(BYTE SegNo);
extern  BOOL    RAMcmRegisterSegment(BYTE SegNo);
extern  void    RAMcmModifyWriteSegment(BYTE SegNo);
extern  void    RAMClearCache(BYTE CmdType);

/* ramNewWrite.c */
extern  void    RAMPostMessage(BYTE Msg);
extern  BYTE    RAMGetMessage(void);
extern  BOOL    DVDRAM_FlushWriteBuffer(void);
extern  void    DVDRAMBLKWrittenProc(void);
extern  void    DVDRAMLinkListInit(void);
extern  void    RAMInitRingBuffer(void);
extern  void    RAMInitRingBuffer_NoFlush(void);
extern  void    DVDRAMSyncCacheCmd(void);
extern  void    DVDRAMSyncCacheProc(void);


/* ramNewDfmgt.c */
extern  BYTE    RAM_GetDefectInfo(LONG LSN, LONG Length);
extern  void    DVDRAMMarkSLRBit(ULONG DefPSN, BYTE SLR_BIT);


/* ramNewRead.c */
extern  DiscAccessStatusE DVDRAMReadDataFromDisc(USHORT start_page,ULONG blk_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode);
extern  BOOL    IsCertifiedBlock(USHORT EccStartPage);
extern  void    DVDRAMAppendPreBuffer(void);
extern  void    RamReadSkipForPrebuffering(void);
extern  DiscAccessStatusE DVDRAMReadBlockToTempBuffer(ULONG BlkStartLSN,ULONG BlkStartPSN);
extern  BYTE    ramVerifyID(void);
extern  void    InitializeIceIndicator(void);
        #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
extern  void    DVDRAMIceDetection(void);
extern  void    SetDVDRAMIceIndicator(void);
extern  void    ClearDVDRAMIceIndicatorAll(void);
            #if (DISCARD_RAM_DECODE == 1)
extern void RAMICEAreaDecoder(void);
            #endif
        #endif


/* Gp_spin.c */
extern  BYTE CmdSeekOk;
    #endif /* #if (DVD_RAM_READ == 1) */

#endif /* #ifndef _RAMEXTERN_H_ */
