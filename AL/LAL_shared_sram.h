/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: LAL_shared_sram.h
*
* DESCRIPTION
* This file supports the LAL stream functions implemented in the LAL stream layer
*
* NOTES:
*
*   $Revision: 6 $
*   $Date: 09/09/04 4:42p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Compiler Definitions
*****************************************************************************/
#ifndef _LAL_SHARED_SRAM_H_
#define _LAL_SHARED_SRAM_H_

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\COMMON\globtype.h"
#include ".\COMMON\globals.h"
#include ".\common\vu_cfg.h"
#include ".\oem\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_servo.h"
#include ".\AL\HAL_opc.h"
#include ".\AL\HAL_afe.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/* data type describing the states */
typedef enum
{
    LAL_SHARED_SRAM_CONFIG_STREAM,
    LAL_SHARED_SRAM_CONFIG_CLEAR
// Add more ...
// Ex.   LAL_SHARED_SRAM_CONFIG_DECODER,
// Ex.   LAL_SHARED_SRAM_CONFIG_ENCODER,
} ELAL_SHARED_SRAM_Config;

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/

/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

extern void LAL_SHARED_SRAM_Init(void);
extern void LAL_SHARED_SRAM_StoreParams(discKindT DiscKind, ESpindleSpeed Speed);
extern void LAL_SHARED_SRAM_Enable(ELAL_SHARED_SRAM_Config eConfig);

/*****************************************************************************
*                       End Of File
*****************************************************************************/

#endif /* _LAL_SHARED_SRAM_H_ */