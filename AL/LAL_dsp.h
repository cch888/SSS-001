/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_dsp.h
*
* DESCRIPTION
*   This file supports the LAL DSP funtions implemented in the LAL DSP layer
*
* NOTES:
*
* $Revision: 16 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _LALDSP_H_
#define _LALDSP_H_

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"

#if (ENABLE_LEARN_DEFECT == 1)
typedef enum
{
    lalDSP_Off,
    lalDSP_On,
    lalDSP_ResetOn
} lalDSP_OnOff_t;
#endif
#if (XDFCT_ENABLE == 1)
typedef enum
{
    lalDSP_Disable,
    lalDSP_Enable_TE,
    lalDSP_Enable_FE
} lalDSP_XDFCT_t;
#endif
extern BOOL LAL_DSP_Init(void);
#if (ENABLE_LEARN_DEFECT == 1)
extern void LAL_DSP_SetLearnDefect(lalDSP_OnOff_t mode);
#endif
#if (NEW_FDP_DETECTION == 2) || (ENABLE_LEARN_DEFECT == 1)
extern void LAL_DSP_UpdateTime(USHORT OverspeedX10);
#endif
#if (XDFCT_ENABLE == 1)
extern void LAL_DSP_XDFCT_Enable(lalDSP_XDFCT_t mode, USHORT threshold);
#endif
#endif /* _LALDSP_H_ */
