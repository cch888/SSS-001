/****************************************************************************
*                (c) Copyright 2006 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 43 $
* $Date: 11/03/11 2:32p $
*
* DESCRIPTION
*   This file supports the HAL LASC funtions implemented in the LASC HAL layer
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_LASC_H_
#define _HAL_LASC_H_

#include ".\al\REG_write_strategy.h"
#if (OPU == KEM350AAA)
#include ".\OEM\OPU_KEM350AAA.h"
#endif
#include ".\servo\SVO.H" /* Needed for types of parameters */

/* LASC Macro's */
#define HAL_LASC_THRINTHOLD(value)   WRITE_FIELD(HAL_SRV_LPCTHOLD, value)

#if (DEBUG_LASER_OPEN_LOOP == 1)
#define LASC_INTS_MASK               HAL_SRV_DELTA2IEN | HAL_SRV_DELTA1IEN | \
                                     HAL_SRV_DELTA0IEN|HAL_SRV_THRESHIEN
#define HAL_LASC_DISABLE_LASC_INTERRUPTS  READ_MOD_WRITE(HAL_SRVREG_MAP->LCINTEN, LASC_INTS_MASK, 0x00)
#endif

//*** Global definitions regarding the LASC HAL functionality
#if (PCB == 691)
#define MAX_CD_THRESHOLD 0x3B00
#define MAX_DVD_THRESHOLD 0x3B00
#define MAX_HD_THRESHOLD 0x3E00
#endif

#if (PCB==691)
#define LASC_SLOPE_SCALE 100
#else
#define LASC_SLOPE_SCALE 100
#endif
#define   COLL_DIS      0x10000000
#define   ALPHA_DIS     0x08000000
#define   RESERV_DIS    0x04000000
#define   DELTA2_DIS    0x02000000
#define   DELTA1_DIS    0x01000000
#define   DELTA0_DIS    0x00800000
#define   THRESH_DIS    0x00400000
#define   DMO_DIS       0x00200000
#define   STEP_DIS      0x00100000
#define   TILT_DIS      0x00080000
#define   TRO_DIS       0x00040000
#define   FOO_DIS       0x00020000
#define   VREF_DIS      0x00010000
#define   VREF_LPWR     0x00001000
#define   DELTA2_LPWR   0x00000800
#define   DELTA1_LPWR   0x00000400
#define   DELTA0_LPWR   0x00000200
#define   THRESH_LPWR   0x00000100
#define   ALPHA2_LPWR   0x00000080
#define   ALPHA1_LPWR   0x00000040
#define   DMO_LPWR      0x00000020
#define   SLED2_LPWR    0x00000010
#define   SLED1_LPWR    0x00000008
#define   TILT_LPWR     0x00000004
#define   TRO_LPWR      0x00000002
#define   FOO_LPWR      0x00000001

/* ------------------------------------------------------- *
 *     Input Para for "LaserOscControl"                    *
 * ------------------------------------------------------- */
enum
{
    OSCEN_OFF      = (HAL_WS_MANOSCEN),
    OSCEN_ON       = (HAL_WS_MANOSCEN|HAL_WS_OSCENI),
    OSCEN_NWG      = (HAL_WS_RWSEL|HAL_WS_OSCEN1),
    OSCEN_RDISC    = (HAL_WS_MANOSCEN|HAL_WS_OSCENI),
    OSCEN_RWDISC   = (HAL_WS_RWSEL|HAL_WS_OSCEN1),
    OSCEN_RDISC_HS = (HAL_WS_OSCEN1)
};

#define HAL_LASC_SetOscen(x) WRITE_REG(HAL_WSREG_MAP->OSCENCTL, x);


/* ------------------------------------------------------- *
 *     Categories for set dac within LASC                  *
 * ------------------------------------------------------- */
enum
{
    ALL_DAC_MUTE,
    READ0RS_DAC,
    READ1RS_DAC,
    BIAS0RS_DAC,
    BIAS1RS_DAC,
    BIAS0plusRS_DAC,
    BIAS1plusRS_DAC,
    READ0WS_DAC,
    READ1WS_DAC,
    BIAS0WS_DAC,
    BIAS1WS_DAC,
    BIAS0plusWS_DAC,
    BIAS1plusWS_DAC,
    THRESHOLD_REFERENCE_DAC,
    THRESHOLD_OPENLOOP_DAC,
    DELTA_OPENLOOP_DAC,
};

typedef enum
{
    halLASC_Groove,
    halLASC_Land,
    halLASC_LandGroove
} halLASC_LandGroove_t;

typedef enum
{
    halLASC_DAC0,
    halLASC_DAC1,
    halLASC_DAC2
} halLASC_DAC_t;

typedef enum
{
    halLASC_AbsPower,
    halLASC_RelPower
} halLASC_PowerMode_t;

#define DRY_WRITE_LASER_MODE halLASC_AbsPower
#define NORMAL_WRITE_LASER_MODE halLASC_AbsPower

typedef enum
{
    halLASC_Read,
    halLASC_Write
} halLASC_ReadWrite_t;

//*** External function prototypes
extern void HAL_LASC_Init( void );
extern void HAL_LASC_SetReadMode(void);
extern void HAL_LASC_SetLaserOn(BOOL LaserOn);
extern void HAL_LASC_Wait_Threshold_Stable(void);
extern void HAL_LASC_SetPdic(void);
#if (OPU == BDPL2)
extern void HAL_LASC_SetAPCTarget(SHORT colli_pos);
#endif
#if (OPU == DXX811)
extern void HAL_LASC_SetLCD(void);
extern void SetHFMMode(BYTE mode);
#endif
#if (OPU == KEM350AAA)
extern void HAL_LASC_SetLCD(void);
extern void SetLCD(eLCDSW_OFF_ON_t cmd);
extern void HAL_LASC_SetCollimotorLED(eOPTOCOUPLER_LED_t cmd);
extern void HAL_LASC_ActivateNDFilter(NDFILTEROPEN_CLOSE_t x);
#endif
#endif /* _HAL_LASC_H_ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
