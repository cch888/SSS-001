/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_afe.h
*
* DESCRIPTION
*   This file supports the LAL AFE funtions implemented in the LAL AFE layer
*
* NOTES:
*
* $Revision: 25 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALAFE_H_
#define _LALAFE_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"

extern void LAL_AFE_Init(void);

extern void LAL_AFE_ClearAFEOffsets(void);
extern void LAL_AFE_InitAFEGains(void);
extern void LAL_AFE_GetMbSbInpValues(USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10);
extern void LAL_AFE_GetDPDInpValues(BYTE layer, USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10);
extern void LAL_AFE_GetWobbleValues(USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10);
extern void LAL_AFE_GetRFInpValues(USHORT SRFOPeakX1000L0, USHORT SRFOPeakX1000L1, SHORT SRFOSTMGaindBX10);
#if (BD_ENABLE == 1)
extern void LAL_AFE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10);
#endif // (BD_ENABLE == 1)


#endif /* _LALAFE_H_ */
