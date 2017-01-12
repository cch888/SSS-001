/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdWritableRead.h $
*
* DESCRIPTION   Header file for bdInit.c
*
* $Revision: 9 $
* $Date: 10/02/10 4:13p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BDWRITABLEREAD_H__
#define __BDWRITABLEREAD_H__

#if (BD_ENABLE)

#define ENABLE_BDWT_BG_PREBUFFER        1
#define BD_CACHE_RESERVED_CNT           3
#define BD_PREBUFFER_LENGTH             (eBDCacheSegmentCnt - BD_CACHE_RESERVED_CNT) //reserve 3 clusters
#define BD_PREBUF_THRESHOLD             (eBDCacheSegmentCnt/2)   //50% of Total Cache

/*********************************************************************
    EXTERNAL FUNCTIONS
*********************************************************************/
extern void BdWritableRead(void);
extern void BDStopPrebuffering(void);
extern DiscAccessStatusE BDReadDataFromDisc(USHORT start_page,ULONG read_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode);

#endif  //(BD_ENABLE)

#endif
