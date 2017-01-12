/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_dfe.h
*
* DESCRIPTION
*   This file supports the LAL DFE funtions implemented in the LAL DFE layer
*
* NOTES:
*
* $Revision: 27 $
* $Date: 11/03/15 2:22p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALDFE_H_
#define _LALDFE_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"

typedef enum
{
    lalDFE_EqualizerOff,
    lalDFE_EqualizerStrengthLow,
    lalDFE_EqualizerStrengthNormal,
    lalDFE_EqualizerStrengthHigh,
    lalDFE_EqualizerDVDRAMEmbossed
} lalDFE_DFEEqualizerStrength_t;

typedef enum
{
    lalDFE_PicClipFilNotBypass,
    lalDFE_PicClipFilBypass
} lalDFE_DFEPicClipFilter_t;

typedef enum
{
    lalDFE_VitNormal,
    lalDFE_VitPassThruEnable
} lalDFE_DFEVitPassThru_t;

#if (DVD_RAM_READ == 1)
typedef struct
{
	BYTE MaxPeakHeader;
	BYTE MinPeakHeader;
	BYTE MaxPeakData;
	BYTE MinPeakData;
	BOOL Land;
}HfPeakMeasStruct;
#endif

extern void LAL_DFE_Init(void);
extern void LAL_DFE_SetEqualizer(lalDFE_DFEEqualizerStrength_t Strength);
extern void LAL_DFE_SetNLE(lalDFE_DFEEqualizerStrength_t Strength);
extern void LAL_DFE_SetBwDataSlicer(void);
extern void LAL_DFE_SetBwFastPll(void);
extern void LAL_DFE_SetClipLvlFastPll(void);
#if (DVD_RAM_READ == 1)
extern HfPeakMeasStruct LAL_AnalizeDvdRamRfSignal();
#endif
extern void LAL_DFE_SetClipNLE(lalDFE_DFEEqualizerStrength_t Strength);
#if 0//currently not be use
extern void LAL_DFE_SetDefectDet(void);
#endif //currently not be use
extern void LAL_DFE_SetHfPll(BOOL ForceAutoLock);
#if (BD_ENABLE == 1)
extern void LAL_DFE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10);
#endif //(BD_ENABLE == 1)
extern void LAL_DFE_SetDigiBetaMode(ULONG OverspeedX100);

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
/* this macro defines the value to be used for the Fast PLL Kp in case
of PIC reading for BD-R or BD-RE */
#if (BD_ENABLE == 1)
#define LAL_PIC_FPLL_KP_BDR_RE    6
#endif //(BD_ENABLE == 1)


#endif /* _LALDFE_H_ */
