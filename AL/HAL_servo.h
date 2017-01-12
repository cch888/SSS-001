/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: HAL_servo.h
*
* DESCRIPTION
* This file supports the HAL servo functions implemented in the HAL servo layer
*
* NOTES:
*
*   $Revision: 17 $
*   $Date: 11/03/11 2:32p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Compiler Definitions
*****************************************************************************/
#ifndef _HALSERVO_H_
#define _HALSERVO_H_

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\COMMON\globtype.h"
#include ".\COMMON\globals.h"
#include ".\AL\reg_hal.h"
#include ".\AL\reg_servo.h"
#include ".\AL\hal_dsp.h" //LAL_STREAM_TODO: illegal call to HAL module; macro must be put elsewhere
#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\servo\SvoVar.h"

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
#define HAL_SERVO_SetTilt(x)    WRITE_REG(HAL_SRVREG_MAP->TILTDAT,x)
#if (FOOTILT_DIFFERENTIAL == 1)
#define HAL_SERVO_GetTilt()     CONVERT_SIGNED_10b_TO_16b(READ_FIELD(HAL_SRV_TILTDAT))
#else
#define HAL_SERVO_GetTilt()     READ_FIELD(HAL_SRV_TILTDAT)
#endif

#if (REPLACE_FEBC_WITH_FEO == 0)
#define HAL_SERVO_SetFebc(x)    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FBA);\
                                WRITE_REG(HAL_SRVREG_MAP->DASET, x)
#else
#define HAL_SERVO_SetFebc(x)    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FEO);\
                                WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.feOfsData.all16 + (x))
#endif

#if (FOO_INVERSION == 0)
    #if (FOOFFSET2_ENABLE == 1)
#define HAL_SERVO_SetFoos(x)    WRITE_DSP_COEF(fooffset2, (USHORT)x)
#define HAL_SERVO_GetFoos()     READ_DSP_COEF(fooffset2)
    #else
        #if (FOOTILT_DIFFERENTIAL == 1)
#define HAL_SERVO_SetFoos(x)    WRITE_REG(HAL_SRVREG_MAP->FOOFFSET, x);\
                                WRITE_REG(HAL_SRVREG_MAP->TILTOFS, x)
        #else
#define HAL_SERVO_SetFoos(x)    WRITE_REG(HAL_SRVREG_MAP->FOOFFSET, x)
        #endif
#define HAL_SERVO_GetFoos()     READ_FIELD(HAL_SRV_FOOFFSET)
    #endif
#define HAL_SERVO_SetFocUp(x)   WRITE_REG(HAL_SRVREG_MAP->FOLNSUP, x)
#define HAL_SERVO_SetFocDn(x)   WRITE_REG(HAL_SRVREG_MAP->FOLNSDN, x)
#define HAL_SERVO_GetFocUp()    READ_REG(HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SERVO_GetFocDn()    READ_REG(HAL_SRVREG_MAP->FOLNSDN)
#else
    #if (FOOFFSET2_ENABLE == 1)
#define HAL_SERVO_SetFoos(x)    WRITE_DSP_COEF(fooffset2, (USHORT)(-(x)))
#define HAL_SERVO_GetFoos()     (-READ_DSP_COEF(fooffset2))
    #else
        #if (FOOTILT_DIFFERENTIAL == 1)
#define HAL_SERVO_SetFoos(x)    WRITE_REG(HAL_SRVREG_MAP->FOOFFSET, -(x));\
                                WRITE_REG(HAL_SRVREG_MAP->TILTOFS, -(x))
        #else
#define HAL_SERVO_SetFoos(x)    WRITE_REG(HAL_SRVREG_MAP->FOOFFSET, -(x))
        #endif
#define HAL_SERVO_GetFoos()     (-READ_FIELD(HAL_SRV_FOOFFSET))
    #endif
#define HAL_SERVO_SetFocUp(x)   WRITE_REG(HAL_SRVREG_MAP->FOLNSDN, x)
#define HAL_SERVO_SetFocDn(x)   WRITE_REG(HAL_SRVREG_MAP->FOLNSUP, x)
#define HAL_SERVO_GetFocUp()    READ_REG(HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SERVO_GetFocDn()    READ_REG(HAL_SRVREG_MAP->FOLNSUP)
#endif
    #if (CHIP_REV >= 0xC0)
#define HAL_SERVO_SetFocStp()   WRITE_REG(HAL_SRVREG_MAP->FOLNSTP, 0)
        #if (FOO_INVERSION == 0)
#define HAL_SERVO_SetFcs(x)     WRITE_FIELD(HAL_SRV_FCSSBY,x)
#define HAL_SERVO_SetProtFoos(x) WRITE_DSP_COEF(fooffset2,x);
        #else
#define HAL_SERVO_SetFcs(x)     WRITE_FIELD(HAL_SRV_FCSSBY,-(x))
#define HAL_SERVO_SetProtFoos(x) WRITE_DSP_COEF(fooffset2,-(x));
        #endif
    #endif
/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/
__inline void HAL_SERVO_EnableSTMPulseFrequency(BOOL value)
{
    WRITE_FIELD(HAL_SRV_STMEN, value);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_GetFebc()
    Description:    read current FEBC from register
    Inputs:
    Outputs:        FEBC dac
    Global Inputs:
    Global Outputs:
    Returns:        SHORT
-----------------------------------------------------------------------------------------------------*/
__inline SHORT HAL_SERVO_GetFebc(void)
{
#if (REPLACE_FEBC_WITH_FEO == 0)
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FBA);
    return (SHORT)READ_REG(HAL_SRVREG_MAP->SRCR);
#else
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEO);
    return (SHORT)READ_REG(HAL_SRVREG_MAP->SRCR) * 4 - svoCalVar.feOfsData.all16;
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_GetFooDac()
    Description:    read FOO dac output
    Inputs:
    Outputs:        FOO dac
    Global Inputs:
    Global Outputs:
    Returns:        SHORT
-----------------------------------------------------------------------------------------------------*/
__inline SHORT HAL_SERVO_GetFooDac(void)
{
    SHORT FooDac;

    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FOO);
    FooDac = READ_REG(HAL_SRVREG_MAP->SRCR) - 512;
#if (FOOTILT_DIFFERENTIAL == 1)
    FooDac -= CONVERT_TILTDAT_TO_FOO(READ_FIELD(HAL_SRV_TILTDAT));
#endif // (FOOTILT_DIFFERENTIAL == 1)
#if (FOO_INVERSION == 0)
    return FooDac;
#else
    return (-FooDac);
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_SetFSL()
    Description:    set HAL_SRV_FSL & HAL_SRV_FSLNEG
    Inputs:         BYTE FSL_pos, BYTE FSL_neg
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_SERVO_SetFSL(BYTE FSL_pos, BYTE FSL_neg)
{
#if (FOO_INVERSION == 0)
    WRITE_FIELD(HAL_SRV_FSL, FSL_pos);
    WRITE_FIELD(HAL_SRV_FSLNEG, FSL_neg);
#else
    WRITE_FIELD(HAL_SRV_FSL, FSL_neg);
    WRITE_FIELD(HAL_SRV_FSLNEG, FSL_pos);
#endif
}//end of HAL_SERVO_SetFSL

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_CenterControlON

    Description:    switch center error servo control on

    Inputs:         direction
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_SERVO_CenterControlON(BOOL direction)
{
#if (CHIP_REV >= 0xC0)
    // Bug #5659: use CELPF filter in DSP
    //WRITE_DSP_COEF(kc20, CELPF_1kHz[0]);
    //WRITE_DSP_COEF(kc21, CELPF_1kHz[1]);
    //WRITE_DSP_COEF(kc22, CELPF_1kHz[2]);
#endif

    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEO);  /* set CE Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.ceOfsData.all16);    /* write register data */

    #if (TRO_INVERSION == 0)
    if (direction == FORW)
    #else
    if (direction == BACK)
    #endif
    {
        WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVFWD);   /* CE ON forward moving*/
    }
    else
    {
        WRITE_REG(HAL_SRVREG_MAP->PUBWD, HAL_SRV_TSVBWD);   /* CE ON backward moving*/
    }
}//end of HAL_SERVO_CenterControlON

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_SERVO_CenterControlOFF()
    Description:    set HAL_SRV_FSL & HAL_SRV_FSLNEG
    Inputs:         BYTE FSL_pos, BYTE FSL_neg
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline void HAL_SERVO_CenterControlOFF(void)
{
    WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);
    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);  /* set CE Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.teOfsData.all16);    /* write register data */
}//end of HAL_SERVO_CenterControlOFF


/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
/* initialisation */
#define HAL_SERVO_Init() /* still empty */
/* STM pulse frequency */
extern ULONG HAL_SERVO_ConfigureSTMPulseFrequency(ULONG Frequency);

#if (CHIP_REV >= 0xC0)
void HAL_SERVO_UpdateTime(USHORT OverspeedX10);
#endif

/*****************************************************************************
*                       End Of File
*****************************************************************************/
#endif /* _HALSERVO_H_ */
