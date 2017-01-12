/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   rd_util.h
*
* DESCRIPTION
*   This file contains include file used in ATAPI read command
*
* NOTES:
*   $Revision: 15 $
*   $Date: 09/07/03 4:29p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __RD_UTIL_H__
#define __RD_UTIL_H__

extern void InitBuffering(void);
extern void Rw_ecc_mode(void);
extern vBOOL SeekToTarget(void);
extern void AdjustSeekOffset(void);
extern BYTE StartBufferingFromTarget(void);
extern BYTE WaitForCacheHit(void);
extern void StopDecoder(void);
extern void Rw_mark_blk_mode(USHORT);
extern BYTE Rw_get_blk_mode(void);
extern USHORT Rw_get_remain_xfer_blk(void);
extern void ResetCDDecoder(void);
extern void ClearDataBuffer(void);
extern BYTE CheckForReadAheadBufferingHit(USHORT);
extern void ClearReadAheadSettings(void);
extern void EccCtrlMode(BYTE);

USHORT Buffering(long target_lba, USHORT length);
void CDCheckTrackDataMode(BYTE tno);

/* rd_track.c */

#define eStartBuff     0x01
#define eFoundTarget   0x02
#define eEccError      0x04
#define eBuffComplete  0x08
#define eFoundRunOut   0x10
#define eAudioCD       0x01
#define eDataCD        0x02
#if(CEProduct == 0)
#define MAX_CD_DECODE_PERIOD    8  /* It can be defined from 0 to 256. If the transfer rate curve isn't smooth,
                                                             you can try to set MAX_CD_DECODE_PERIOD to 6 or 8.   */
#endif

#define CLEAR_DECODE_INTERRUPTS()    WRITE_REG(HAL_CDDREG_MAP->DINT0, 0);\
                                     WRITE_REG(HAL_CDDREG_MAP->DINT1, 0);\
                                     WRITE_REG(HAL_CDDREG_MAP->DINT2, 0);
#endif /* __RD_UTIL_H__ */
