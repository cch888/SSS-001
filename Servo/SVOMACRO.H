/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVOMACRO.H
*
* DESCRIPTION
*   Servo Module macro definitions.
*
*   $Revision: 35 $
*   $Date: 08/12/24 2:35p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVOMACRO_H_
#define _SVOMACRO_H_

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_afe.h"
#include ".\al\REG_global.h"
#include ".\common\hwdef.h"

/*----------------------------------------------------------------------------
 * system clock/timer/interrupts
 *----------------------------------------------------------------------------*/

#define SYSTEM_CLOCK_FREQ  33.8688f   /* 33.8688 MHz */

/*----------------------------------------------------------------------------
 * motor outputs/general signal detection
 *----------------------------------------------------------------------------*/
/*
 * disk motor
 * */
#define FG_TIMER_PRESCALER     16

/* Servo System Macro */
//svoMiscVar.pSeqArgs->arg[x] will be pointed to a constant table, so it is read only
#define ARG1            (svoMiscVar.pSeqArgs->arg[0])
#define ARG2            (svoMiscVar.pSeqArgs->arg[1])
#define ARG3            (svoMiscVar.pSeqArgs->arg[2])

/* Tracking/Focus gain/offset path select */
//for WLDON=1 ==>use WRxxG,WLDON=0 ==>use xxG
/*-----------------------------------*/
/*   WLDON   RECD2       path        */
/*     1      *       WRxxG/xx_osw   */
/*     0      1         xxG/xx_osh   */
/*     0      0    BLANKxxG/xx_osl   */
/*-----------------------------------*/
#define GAIN_RECD (WRITE_FIELD(HAL_AFE_WLDONSEL,  TRUE)), \
                  (WRITE_FIELD(HAL_AFE_BLANKSEL,  TRUE))

//for WLDON=1 ==>use WRxxG,WLDON=0 ==>use BLANKxxG
/*-----------------------------------*/
/*   WLDON   RECD2       path        */
/*     1      *       WRxxG/xx_osw   */
/*     0      *    BLANKxxG/xx_osh   */
/*-----------------------------------*/

#define GAIN_RECD_HIGH (WRITE_FIELD(HAL_AFE_WLDONSEL,  TRUE)), \
                       (WRITE_FIELD(HAL_AFE_BLANKSEL,  FALSE))


/*
 *disable/enable SERVO and AFE defect(DEEP_DFCT)
 *      AFE       keep AFE DEEP_DFCT polarity "High" active
 *                  AFE Core use its own AFE defect signal(DEEP_DFCT) only
 *      SERVO   enable SERVO dfct--SERVO_DFCT or DEEP_DFCT into SERVO Core depends on bAFEDFCT(2EC.0)
 *                  disable SERVO dfct--no defect signal into SERVO Core
*/
#define ENABLE_TE_DCFILTER                              (WRITE_FIELD(HAL_SRV_THOLD, TRUE))
#define DISABLE_TE_DCFILTER                             (WRITE_FIELD(HAL_SRV_THOLD, FALSE))
#define ENABLE_FE_TE_DCFILTER                           (WRITE_FIELD(HAL_SRV_FHOLD, TRUE));\
                                                        (WRITE_FIELD(HAL_SRV_THOLD, TRUE))
#define DISABLE_FE_TE_DCFILTER                          (WRITE_FIELD(HAL_SRV_FHOLD, FALSE));\
                                                        (WRITE_FIELD(HAL_SRV_THOLD, FALSE))
#define ENABLE_SERVODEFECT                              (WRITE_FIELD(HAL_SRV_DFCTOFF, FALSE));
#define DISABLE_SERVODEFECT                             (WRITE_FIELD(HAL_SRV_DFCTOFF, TRUE));

#define ENABLE_EARLY_DEFECT                             (WRITE_FIELD(HAL_SRV_DFCTEN, TRUE));\
                                                        (WRITE_FIELD(HAL_SRV_EDCFTEN, TRUE))
#define DISABLE_EARLY_DEFECT                            (WRITE_FIELD(HAL_SRV_DFCTEN, FALSE));\
                                                        (WRITE_FIELD(HAL_SRV_EDCFTEN, FALSE))

#define DISABLE_FE_HOLDDEFECT                           (WRITE_FIELD(HAL_AFE_AHOLDFE, FALSE));

#define DISABLE_TE_FE_HOLDDEFECT                        (WRITE_FIELD(HAL_AFE_DEFECTHLD, FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_AHOLDFE, FALSE));

#define ENABLE_TE_FE_HOLDDEFECT                         (WRITE_FIELD(HAL_AFE_DEFECTHLD, TRUE));\
                                                        (WRITE_FIELD(HAL_AFE_AHOLDFE, TRUE));

#define DISABLE_FE_ENABLE_TE_HOLDDEFECT                 (WRITE_FIELD(HAL_AFE_DEFECTHLD, TRUE));\
                                                        (WRITE_FIELD(HAL_AFE_AHOLDFE, FALSE));

    #if (DVD_RAM_READ == 1)
/*
 * Disable/enable Header Hold FE, CE, TE, M/S beam S/H
*/
#define DISABLE_HDR_HOLD_SERVO                          (WRITE_FIELD(HAL_AFE_FEHDRHLD,FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_CEHDRHLD,FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_TEHDRHLD,FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_HDREN2,FALSE));

#define ENABLE_HDR_HOLD_SERVO_NORMAL                    (WRITE_FIELD(HAL_AFE_FEHDRHLD,TRUE));\
                                                        (WRITE_FIELD(HAL_AFE_CEHDRHLD,TRUE));\
                                                        (WRITE_FIELD(HAL_AFE_TEHDRHLD,TRUE));\
                                                        (WRITE_FIELD(HAL_AFE_HDREN2,FALSE));

#define ENABLE_HDR_HOLD_SERVO_JUMP                      (WRITE_FIELD(HAL_AFE_CEHDRHLD,FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_TEHDRHLD,FALSE));\
                                                        (WRITE_FIELD(HAL_AFE_HDREN2,TRUE));
    #endif

#endif /* _SVOMACRO_H_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
