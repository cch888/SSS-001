/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   DAC.C
*
* DESCRIPTION:
*   Common routines to deal with Serial and Parallel Digital to Analog Converters
*
* NOTES:
*
*
* $Revision: 18 $
* $Date: 08/07/20 5:31a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\scomm.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_servo.h"

/*-----------------------------------------------------------------------------
 *
 * Module: InitGPDAC()
 * Description:
 * Program as GPDACs and set output to VREF
 *----------------------------------------------------------------------------*/
void InitGPDAC(void)
{
    WRITE_REG(HAL_SRVREG_MAP->GPDACMUX, 0x20);             /* Program as GPDAC2 and GPDAC1 outputs */
    WRITE_REG(HAL_SRVREG_MAP->DACDAT, 0);
#if (FOOTILT_DIFFERENTIAL == 1)
    WRITE_FIELD(HAL_SRV_TILTDAT, 0);
#else
    WRITE_FIELD(HAL_SRV_TILTDAT, DAC_VREF_LEVEL10);  /* Set GPDAC3 output to VREF */
#endif
    WRITE_REG(HAL_SRVREG_MAP->DMODAC, 0);                  /* Set DMO output to VREF */

#if(PCB == 1600)
    WRITE_REG(HAL_SRVREG_MAP->PDMVREF, 0x210);          /* Set PDMVREF output */
#endif
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
