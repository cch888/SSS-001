/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdInit.h $
*
* DESCRIPTION   Header file for bdInit.c
*
* $Revision: 16 $
* $Date: 09/09/25 1:24p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BDINIT_H__
#define __BDINIT_H__

#if (BD_ENABLE)
#include ".\dvd\dvDiscInit.h"

/*********************************************************************
    DEFINES
*********************************************************************/
#if (ENABLE_AACS)
// ROM-Mark
#define BDROM_PIC_CPS_SECTOR_OFFSET         31
#define BD_ROM_MARK_MASK                    0xF0        // bit 7-4
#endif  /* ENABLE_AACS */
#if (BD_LAYER_VERIFY == 1)
enum                            
{
    e_READJUSTPASS,
    e_READJUSTFAIL,
    e_NONE
};
#endif

/*********************************************************************
    EXTERNAL FUNCTIONS
*********************************************************************/
extern void BdStartUpSuccessful(void);
extern BOOL BdVerifyDisc(void);
extern void BdCheckVideo (void);
extern BYTE BdDiscSetup(void);
extern void InitializeBdSdramBuffer(void);
extern BOOL UpdateDBFromTDDS(void);
extern BOOL UpdateDBFromSRRI(void);
extern void UpdateDBForBDRE(void);
extern USHORT CheckHeader(BYTE * DataPtr, HeaderTypeE  * HeaderType);
extern BOOL BdReadPIC(void);
extern DiscAccessStatusE BdReadDMA(void);
extern DiscAccessStatusE BdReadTDMA(void);
extern DiscAccessStatusE BdReadPAC(void);
#if (ENABLE_AACS == 1)
extern BOOL BDReadVomueIDFromDisc(void);
#if (CEProduct == 1)
extern BOOL BDReadVomueKCDFromDisc(void);
#endif
extern BOOL BDReadMkbFromDisc(void);
extern void ConfigureRomMarkDecoder(ULONG seed);
extern void DisableRomMarkDecoder(void);
#endif  /* ENABLE_AACS == 1 */
extern void BdUpdateTrackInfo(USHORT track);
extern void BdUpdateIncompleteTracks(void);
extern ULONG BdGetCurrentPSN(BYTE * BdDecoder_ID);
extern ULONG BdSearchRecordingState(SearchRecStateE State, ULONG StartPSN, ULONG EndPSN);
extern ULONG BdRetrySearchRecordingState(SearchRecStateE State, ULONG StartPSN, ULONG EndPSN);
extern void BdStartBinarySearch(BYTE track, ULONG StartPSN, ULONG EndPSN, ULONG sPSN, ULONG ePSN);
extern void BdInitDMA(void);
#if (ENABLE_BDRLTH == 1)
extern void BDCheckLTHDisc(void);
#endif
#if (ENABLE_BDIH == 1)
extern void BDCheckIHDisc(void);
#endif
#endif  //(BD_ENABLE)

#endif
