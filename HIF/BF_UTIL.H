/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
* FILENAME:   bf_util.h
*
* DESCRIPTION
* This file contains buffer related routines
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __BF_UTIL_H__
#define __BF_UTIL_H__

#include ".\common\globtype.h"

extern BYTE ReadSDRAM(void);
extern USHORT ReadSDRAM2B(void);
extern ULONG ReadSDRAM4B(void);
extern void WriteSDRAM(BYTE);
extern void WriteSDRAM2B(USHORT);
extern void WriteSDRAM4B(ULONG);
extern void CalcHostTransferBlocks(USHORT);
extern void PatternFill(USHORT,USHORT, USHORT, USHORT, USHORT);
extern BYTE wr_type;
extern void CopyBufferToBuffer(USHORT SourcePage,USHORT SourceOffset, USHORT TargetPage, USHORT TargetOffset, USHORT PagesToCopy);
#if (STORE_MSG_TO_SDRAM == 1)
extern void MsgWriteSDRAM(BYTE*,BYTE);
extern ULONG CalMsgHead(ULONG);
#endif
#endif /* __BF_UTIL_H__ */

