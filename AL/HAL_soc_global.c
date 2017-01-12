/* ******************************************************************************************
*
*	(c) Copyright 2002 - 2006 Sunext Design, INC.
*		          All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
* 	HAL_soc_global.c
*
*  Description:
*	This file contains functions for the Global subsystem and includes
* 	the following:
*
*
* Notes:
*
*
*   $Revision: 3 $
*   $Date: 08/07/21 5:09a $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\HAL_soc_global.h"

void HAL_SOC_GLOBAL_ResetBDCodec(void)
{
    WRITE_FIELD(HAL_SOC_GLOBAL_BDSRST, SET);
    WRITE_FIELD(HAL_SOC_GLOBAL_BDSRST, CLEAR);
}


