/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_soc_global.h
*
*
*
*
* DESCRIPTION
*   This file supports the HAL SOC GLOBAL funtions implemented in the SOC
*   GLOBAL HAL layer
*
*
* NOTES:
*
* $Revision: 1 $
* $Date: 07/09/14 5:17p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_SOC_GLOBAL_H_
#define _HAL_SOC_GLOBAL_H_

#include ".\common\globals.h"

//*** PIC Macros
#define HAL_SOC_GLOBAL_SetPicMode()             WRITE_FIELD(HAL_SOC_GLOBAL_BDREPIC, SET)
#define HAL_SOC_GLOBAL_ClrPicMode()             WRITE_FIELD(HAL_SOC_GLOBAL_BDREPIC, CLEAR)

//*** BCA Macros
#define HAL_SOC_GLOBAL_SetBcaMode()             WRITE_FIELD(HAL_SOC_GLOBAL_BCAMODE, SET)
#define HAL_SOC_GLOBAL_ClrBcaMode()             WRITE_FIELD(HAL_SOC_GLOBAL_BCAMODE, CLEAR)


//*** External function prototypes
extern void HAL_SOC_GLOBAL_ResetBDCodec(void);

#endif /* _HAL_A7SOC_H_ */

