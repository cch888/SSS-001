/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_lasc.h
*
* DESCRIPTION
*   This file supports the LAL LASC funtions implemented in the LAL LASC layer
*
* NOTES:
*
* $Revision: 19 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALLASC_H_
#define _LALLASC_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"
#include ".\AL\HAL_lasc.h"


extern void LAL_LASC_Init(void);
extern void LAL_LASC_ClearLaserOutputs(void);
extern void LAL_LASC_GetPdic(USHORT SRFOPeakX1000, SHORT SRFOGaindBX10, BYTE calibrated);
#endif /* _LALLASC_H_ */
