/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL.h
*
* DESCRIPTION
*   This file supports the LAL funtions implemented in the LAL layer
*
* NOTES:
*
* $Revision: 22 $
* $Date: 08/10/14 8:18p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LAL_H_
#define _LAL_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"
#include ".\al\LAL_afe.h"
#include ".\al\LAL_dfe.h"
#include ".\al\LAL_pll.h"
#include ".\al\LAL_wobble.h"
#include ".\al\LAL_lasc.h"
#include ".\al\LAL_dsp.h"
#include ".\al\LAL_bd_codec.h"

extern discKindT LAL_DetectDiscKind(discKindT DiscKind);
extern void      LAL_StoreParams(discKindT DiscKind, ESpindleSpeed Speed);
extern BOOL      LAL_InitPlatform(void);
extern void      LAL_InitServo(void);
extern void      LAL_InitHif(void);
extern void      LAL_WriteCoef(COEFS_t CoefIndex);
extern void      LAL_SetGenericCoefs(ECores eCore);
extern void      LAL_SetTrackingModeCoefs(ETrackingMethod eTrackingMethod);
extern void      LAL_SetServoEqCoefs();
extern void      LAL_SetReadMode(void);
#if (ENABLE_BDRLTH == 1)
extern void      LAL_SetAFEandDFEforLTH(void);
#endif //(ENABLE_BDRLTH == 1)
extern void      LAL_SetDiscTypeSelection(discKindT mDiscKind);
#if (BD_ENABLE == 1)
extern void      LAL_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10);
#endif //(BD_ENABLE == 1)


#endif /* _LAL_H_ */

