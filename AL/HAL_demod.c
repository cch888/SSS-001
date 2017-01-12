/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2006 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*     HAL_demod.c
*
*  Description:
*   This file contains functions for the Servodsp subsystem and includes
*   the following:
*          HAL_DEMOD_Init
*
* Notes:
*
*
*   $Revision: 11 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_demod.h"
#include ".\AL\HAL_demod.h"

/*** Local defines ***/
#define DEFAULT_MAX_BD_SYNC_DEVIATION                 0x04
#define DEFAULT_MAX_BD_SYNC_COLOR_CONFIDENCE_LEVEL    0x0F
#define DEFAULT_BD_SYNC_COLOR_THRESHOLD_VALUE         0x04
#define DEFAULT_BD_UNLOCK_THRESHOLD_VALUE             0x0C
#define DEFAULT_HDDVD_SYNC_UNLOCK_THRESHOLD_VALUE     0x2F
#define DEFAULT_HDDVD_SYNC_COLOR_THRESHOLD_VALUE      0x03
#define DEFAULT_MAX_HDDVD_SYNC_COLOR_CONFIDENCE_VALUE 0x0F
#define DEFAULT_MAX_HDDVD_SYNC_DEVIATION              0x04
#define DEFAULT_BD_PIC_SYNC_LOST_THRESHOLD            0x08
#define DEFAULT_BD_PIC_SYNC_COLOR_THRESHOLD           0x03
#define DEFAULT_BD_PIC_SYNC_COLOR_CONFIDENCE_MAXIMUM  0x08
#define DEFAULT_BD_PIC_SYNC_COINCIDENCE_WINDOW        0x02
#define DEFAULT_BD_PIC_SYNC_WINDOW                    0x03

/*** Local static variables */

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DEMOD_GetFePllLocked

    Description:    Returns whether the Front End PLL is locked (frequency AND phase)
    				Is TRUE if Front End PLL BOTH in frequency and phase locked, FALSE otherwise.

    Inputs:         None
    Outputs:        None
    Global Inputs:  HAL_DMOD_CDVDMDSTA
    Global Outputs: 
    Returns:        BOOL
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_DEMOD_GetFePllLocked(void)
{
    return((BOOL)((READ_REG(HAL_DMODREG_MAP->CDVDMDSTA) & (HAL_DMOD_CDVLK | HAL_DMOD_CDVFLK )) == (HAL_DMOD_CDVLK | HAL_DMOD_CDVFLK )));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_Init

    Description:    Initialization of the demod control block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DEMOD_Init( void )
{
    /* CDVDMDCFG */
    WRITE_FIELD(HAL_DMOD_CDVROBLKD, TRUE);
    WRITE_FIELD(HAL_DMOD_CDVFRCOLK, FALSE);
    WRITE_FIELD(HAL_DMOD_CDVROBCNT, TRUE);
    WRITE_FIELD(HAL_DMOD_CDVSYGAC,  TRUE);  //sync detect hold when defect  
    WRITE_FIELD(HAL_DMOD_CDVCOLTOG, TRUE);
    //WRITE_FIELD(HAL_DMOD_CDVCOLKSP, 3);   //cause by ASIC CVT verify use this value and suggest use this
    WRITE_FIELD(HAL_DMOD_CDVCOLKSP, halDMOD_SlowConfOutLock);  // To make some MKM DVD-RDL work, we need this setup.
    WRITE_FIELD(HAL_DMOD_DMDHDRONL, FALSE);
    WRITE_FIELD(HAL_DMOD_AMSYCFDTH, AM_SYNC_CONFIDENCE_LEVEL03);

    /* BDDMODCFG */                                             
    WRITE_FIELD(HAL_DMOD_BDDMODRUN, FALSE);
    WRITE_FIELD(HAL_DMOD_BDROBSYLK, TRUE);
    WRITE_FIELD(HAL_DMOD_BDSYGATCL, FALSE);
    WRITE_FIELD(HAL_DMOD_BDSYUNTHR, DEFAULT_BD_UNLOCK_THRESHOLD_VALUE);
    WRITE_FIELD(HAL_DMOD_BDSYCOLTH, DEFAULT_BD_SYNC_COLOR_THRESHOLD_VALUE);
    WRITE_FIELD(HAL_DMOD_BDSYCOLMX, DEFAULT_MAX_BD_SYNC_COLOR_CONFIDENCE_LEVEL);
    WRITE_FIELD(HAL_DMOD_BDDISTSY,  DEFAULT_MAX_BD_SYNC_DEVIATION);
    
    /* HDDMODCFG */
    WRITE_FIELD(HAL_DMOD_HDFRCROM,  TRUE);
    WRITE_FIELD(HAL_DMOD_HDROBSYLK, TRUE);
    WRITE_FIELD(HAL_DMOD_HDLASTSEC, TRUE);
    WRITE_FIELD(HAL_DMOD_HDSYGATEC, TRUE);
    WRITE_FIELD(HAL_DMOD_HDSYUNTHR, DEFAULT_HDDVD_SYNC_UNLOCK_THRESHOLD_VALUE);                                                
    WRITE_FIELD(HAL_DMOD_HDSYCOLTH, DEFAULT_HDDVD_SYNC_COLOR_THRESHOLD_VALUE);                                                 
    WRITE_FIELD(HAL_DMOD_HDSYCOLMX, DEFAULT_MAX_HDDVD_SYNC_COLOR_CONFIDENCE_VALUE);                                            
    WRITE_FIELD(HAL_DMOD_HDDISTSY,  DEFAULT_MAX_HDDVD_SYNC_DEVIATION);                                                          
    
    /* BDPICDCFG */
    WRITE_FIELD(HAL_DMOD_BDPICSYLT, DEFAULT_BD_PIC_SYNC_LOST_THRESHOLD);                                
    WRITE_FIELD(HAL_DMOD_BDPICRSYL, TRUE);                           
    WRITE_FIELD(HAL_DMOD_BDPICSCT,  DEFAULT_BD_PIC_SYNC_COLOR_THRESHOLD);                       
    WRITE_FIELD(HAL_DMOD_BDPICSYCM, DEFAULT_BD_PIC_SYNC_COLOR_CONFIDENCE_MAXIMUM);                            
    WRITE_FIELD(HAL_DMOD_BDPICSYCW, DEFAULT_BD_PIC_SYNC_COINCIDENCE_WINDOW);                                        
    WRITE_FIELD(HAL_DMOD_BDPICSYWI, DEFAULT_BD_PIC_SYNC_WINDOW);
    WRITE_FIELD(HAL_DMOD_BDPICBUG,  FALSE);
    
    /* BDDMDCFG2 */
    WRITE_REG(HAL_DMODREG_MAP->BDDMDCFG2, 0);
    
    /* DMODINTS */
    WRITE_REG(HAL_DMODREG_MAP->DMODINTS, 0);
    
    /* DMODINTE */
    WRITE_REG(HAL_DMODREG_MAP->DMODINTE, 0);
    
    /* MMCFG */
    WRITE_REG(HAL_DMODREG_MAP->MMCFG, 0);
    
    /* MMOFSCFG */
    WRITE_REG(HAL_DMODREG_MAP->MMOFSCFG, 0);
    
    /* DMODDBG */
    WRITE_REG(HAL_DMODREG_MAP->DMODDBG, 0);
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
