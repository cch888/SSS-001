/*****************************************************************************
*           (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*   FILE NAME:     $Workfile: dvDiscInit.h $
*
*   DESCRIPTION     Header File for dvDiscInit.c
*
*
*   $Revision: 24 $
*   $Date: 09/07/30 7:24p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
    FILE DEFINITION
*********************************************************************/
#ifndef __DVDISCINIT_H__
#define __DVDISCINIT_H__

#include ".\common\vu_cfg.h"
#include ".\hif\gp_ram.h"
#include ".\dvd\dvDiscRd.h"

#if (ENABLE_HDDVD)
#include ".\hd\hdDiscInit.h"
#endif

/**********************************************************************
     External Variables
**********************************************************************/



/*******************************************************************
    DEFINES AND ENUMERATIONS
*******************************************************************/
#define SEARCH_ERROR  0xFFFFFFFF

typedef enum
{
    UNRECORDED_PSN,
    RECORDED_PSN
} SearchRecStateE;

/*******************************************************************
    EXTERNAL FUNCTIONS / FUNCTION PROTOTYPES
*******************************************************************/

extern DiscAccessStatusE dvdReadControlDataZone(BYTE TOC_ReadFail);

extern DiscAccessStatusE dvdBufferControlDataBlock(BYTE , ULONG);
extern DiscAccessStatusE dvdBufferClosureBlock(BYTE , ULONG);
extern DiscAccessStatusE dvdBufferBorderInBlock(BYTE , ULONG);
extern DiscAccessStatusE dvdBufferIntroBlock(BYTE , ULONG);

extern DiscAccessStatusE dvdReadTocZone(void);
extern DiscAccessStatusE dvdGetOpenSessionInfo(void);
extern ULONG dvdGetCurrentPSN(BYTE * DVDDecoder_ID);
#if GET_LAST_TRACK_INFO_BASED_ON_RAI
extern DiscAccessStatusE dvdLastRAIPSN(void);
#endif
extern void dvdUpdateTrackInfo(BYTE);
extern void dvdUpdateAllTracks(void);
extern void dvdCheckMiddleZoneStatus(void);
extern DiscAccessStatusE dvdReadRMAZone(void);
extern void dvdCheckVideo(void);
#if (Detect_AVCHD_DISC == 1)
extern void dvdCheckAVCHD(void);
#endif
extern DiscAccessStatusE dvdReadInnerIDZone(void);
extern void dvdEmptyRMAInformation(void);
extern void dvdSetRMAFormat3FailInfo(void);
extern void dvdDataDetect(void);
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
extern void dvdSetCPRMInfo(USHORT);
#endif  /* ENABLE_CPRM == 1 && BCA_ENABLE == 1 */
#if (VCPS_TEST == 1)
extern BYTE dvdReadDKBHeader(void);
#endif  /* VCPS_TEST == 1 */
#if (ENABLE_AACS == 1)
extern BOOL dvdReadMkbFromDisc(void);
extern BOOL dvdReadVolumeIDFromDisc(void);
extern BOOL dvdReadKcdFromDisc(void);
#endif  /* ENABLE_AACS == 1 */
#endif
