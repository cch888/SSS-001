/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2007 Sunext Design, INC.
*                 All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   LAL_bd_codec.c
*
*  Description:
*   This file contains functions for the LAL BD Codec layers and includes
*   the following:
*
*             .
*
* Notes:
*
*
*   $Revision: 10 $
*   $Date: 08/11/14 5:46p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\common\chip.h"
#include ".\common\intctl.h"
#include ".\common\macros.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_bd_codec.h" 


/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_BD_CODEC_Init

    Description:    Initialise the appropriate BD Codec registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_BD_CODEC_Init(void)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    HAL_GLOBAL_ResetBDCodec();

    HAL_BD_CODEC_Init();

    SET_INT_ENABLE_MASK(temp_IE);
}

void LAL_BD_CODEC_SetBCADecoder(void) 
{
//TBD:SE1 need to review again    
    WRITE_FIELD(HAL_BDC_BCAMERR,0x10);//Set BCA maximum number of Errata   
    WRITE_FIELD(HAL_BDC_BCAMERA,0x10);//Set BCA maximum number of Erasures    
    WRITE_FIELD(HAL_BDC_BCAFILL,0x02);//BCA Buffer fill attempts    
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
