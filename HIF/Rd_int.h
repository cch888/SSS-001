/******************************************************************************
*                (c) Copyright 2001, 2002, 2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   rd_int1.c
*
* DESCRIPTION
*   This file contains the interrupt-1 service routine for
*   decoder interrupt and data transfer done interrupt
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __RD_INT_H__
#define __RD_INT_H__

extern void CDDecoderIntProc(void);
extern void HostTransferEndIntProc(void);
extern void ReadStartIntProc(void);
extern void RunOutIntProc(void);
extern void OverRunIntProc(void);
extern void ReadMissIntProc(void);
extern void MSBErrorIntProc(void);
extern void EDCNoGoodIntProc(void);
extern vBOOL CheckForFastCache(void);
extern void HeaderErrorIntProc(void);
extern void RunInEndIntProc(void);
extern void C2ErrorIntProc(void);
extern void RunOut1MissedIntProc(void);
extern void RunIn4MissedIntProc(void);
extern void SRWErrorIntProc(void);          /*  */
extern void CDReadFailIntProc(void);
extern void NoCDRomSyncIntProc(void);

#endif /* __RD_INT_H__ */

