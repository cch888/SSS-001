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
*   This file supports the HAL GLOBAL funtions implemented in the
*   GLOBAL HAL layer
*
*
* NOTES:
*
* $Revision: 17 $
* $Date: 11/03/15 2:21p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_GLOBAL_H_
#define _HAL_GLOBAL_H_

#include ".\common\globals.h"

//*** PIC Macros
#define HAL_GLOBAL_SetPicMode()             WRITE_FIELD(HAL_GLOBAL_BDREPIC, SET)
#define HAL_GLOBAL_ClrPicMode()             WRITE_FIELD(HAL_GLOBAL_BDREPIC, CLEAR)

//*** BCA Macros
#define HAL_GLOBAL_SetBcaMode()             WRITE_FIELD(HAL_GLOBAL_BCAMODE, SET)
#define HAL_GLOBAL_ClrBcaMode()             WRITE_FIELD(HAL_GLOBAL_BCAMODE, CLEAR)

typedef enum
{
    D16D64SL0,
    D16D64SL1,
    D16D64SL2,
    D16D64SL3
} D16D64SEL;

typedef enum
{
   halGLOBAL_SingleLayer    = 0,
   halGLOBAL_DualLayer      = 1
} halGLOBAL_DualLayerSelect_t;

//*** External function prototypes
extern void HAL_GLOBAL_ResetBDCodec(void);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_GLOBAL_GetRfPresent()

    Description:    Returns whether a RF signal is present
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        Is True if a RF signal is present, is false otherwise
-----------------------------------------------------------------------------------------------------*/
__inline extern BOOL HAL_GLOBAL_GetRfPresent(void)
{
    return((BOOL)(READ_FIELD(HAL_GLOBAL_RECDOUT)));  // check whether there is need for a "!"
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_GLOBAL_SetSrcAFERefClock(halGLOBAL_AFERefCLKSRC_t src)

    Description:    sets CLKCFG2.AFEREFSRC(1?)
    Inputs:         src
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline extern HAL_GLOBAL_SetSrcAFERefClock(halGLOBAL_AFERefCLKSRC_t src)
{
    WRITE_FIELD(HAL_GLOBAL_AFEREFSRC, src);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_GLOBAL_SetSrcARMPerRefClock(halGLOBAL_ArmPRefCLKSRC_t src)

    Description:    sets CLKCFG2.ARMPREFSRC(1?)
    Inputs:         src
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
__inline extern HAL_GLOBAL_SetSrcARMPerRefClock(halGLOBAL_ArmPRefCLKSRC_t src)
{
    WRITE_FIELD(HAL_GLOBAL_ARMPREFSRC, src);
}
#if 0//currently not be use
    #if (CHIP_REV >= 0xC0)
void HAL_GLOBAL_SetDebugBusLOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel);
void HAL_GLOBAL_SetDebugBusMOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel);
void HAL_GLOBAL_SetDebugBusHOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel);
void HAL_GLOBAL_SetDebugBusUOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel);
    #else
void HAL_GLOBAL_SetDebug64Output(halGLOBAL_Dbg64Sel_t Dbg64Sel, BYTE Dbg64SignalSel);
        #if (DEBUG16_OUTPUT_FROM_MIO == 1)
void HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_t Dbg16Sel, BYTE Dbg16SignalSel, BYTE MioNumber, BYTE MonitorPin);
        #else
void HAL_GLOBAL_SetDebug16Output(D16D64SEL Dbg16Sel, halGLOBAL_D16D64Sel0_t D16D64Sel);
        #endif
    #endif
#endif //currently not be use

#endif /* _HAL_GLOBAL_H_ */

