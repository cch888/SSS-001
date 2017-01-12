/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_wobble.h
*
* DESCRIPTION
*   This file supports the LAL WOBBLE funtions implemented in the LAL WOBBLE layer
*
* NOTES:
*
* $Revision: 17 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALWOBBLE_H_
#define _LALWOBBLE_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"

typedef enum   {eLowGainMode = 0,
                eNormalGainMode,
                eHighGainMode} eWBLPLLMode;
extern void LAL_WOBBLE_Init(void);
extern void LAL_WOBBLE_SetWobbleFreq(USHORT OverspeedX10);
extern void LAL_WOBBLE_SetPLL(eWBLPLLMode mode);

#if (BD_ENABLE == 1)
extern void LAL_WOBBLE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10);
#endif // (BD_ENABLE == 1)

#endif /* _LALWOBBLE_H_ */
