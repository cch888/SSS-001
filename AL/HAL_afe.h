/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_afe.h
*
* DESCRIPTION
*   This file supports the HAL AFE funtions implemented in the AFE HAL layer
*
* NOTES:
*
* $Revision: 49 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_AFE_H_
#define _HAL_AFE_H_

#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\COMMON\adc.h"
#include ".\AL\reg_hal.h"
#include ".\AL\reg_afe.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"

#define DEFAULTBWRATIO  10

#define RFINGN_0_dBX10  -111
#define RFINGN_1_dBX10  -68
#define RFINGN_2_dBX10  -63
#define RFINGN_3_dBX10  -20

    #if (CHIP_REV < 0xC0)
#define RFGN_0_dBX10    -58
#define RFGN_1_dBX10    0
#define RFGN_2_dBX10    56
#define RFGN_3_dBX10    105
    #else
#define RFGN_0_dBX10    -87
#define RFGN_1_dBX10    -59
#define RFGN_2_dBX10    -28
#define RFGN_3_dBX10    0
#define RFGN_4_dBX10    27//20101019_02QA ==start== Fix TYPO in 01L5 which cause extra diff_RF retry
#define RFGN_5_dBX10    54
#define RFGN_6_dBX10    78
#define RFGN_7_dBX10    102//20101019_02QA ==end== Fix TYPO in 01L5 which cause extra diff_RF retry
    #endif

    #if (CHIP_REV < 0xC0)
#define AFE_WBL_MAX_RGAIN               18
#define AFE_WBL_STEP_GAIN               2
extern const BYTE AFE_WBL_RPG[4];
extern const BYTE AFE_WBL_RG[4];

extern const SBYTE AFE_RFIN_GN[4];
extern const SBYTE AFE_RF_GN[4];
    #else
#define AFE_WBL_MAX_RGAIN               24
#define AFE_WBL_STEP_GAIN               2
extern const BYTE AFE_WBL_RPG[4];
extern const BYTE AFE_WBL_RG[4];

extern const SBYTE AFE_RFIN_GN[4];
extern const SBYTE AFE_RF_GN[8];
    #endif

// facility to have the control of the RF Adaptive Offset Control and the RF Auto Gain Control from AFE or DFE
typedef enum
{
    halAFE_RfAgcAoc_AfeControl,
    halAFE_RfAgcAoc_DfeControl
} halAFE_RfAgcAocControl_t;

// facility to have the control of Wobble VGA3 to DFE or to WBVGA3REG
typedef enum
{
    halAFE_WobVga3_WobVga3RegControl,
    halAFE_WobVga3_DfeControl
} halAFE_WobVga3Control_t;

enum
{
    e_DPDHSISCL_B1,
    e_DPDHSISCL_reserved,
    e_DPDHSISCL_B2,
    e_DPDHSISCL_B3,
};

enum
{
    e_PHSCSEL_FAST=0,
    e_PHSCSEL_SLOW,
};

enum
{
    e_BHSCSEL_FAST=0,
    e_BHSCSEL_SLOW,
};

enum
{
    e_FELPF_BYPASS = HAL_AFE_BPFELP,
    e_FELPF_15  = (halAFE_FELPF_15kHz << HAL_AFE_FEFCSFSEL_LSB),
    e_FELPF_30  = (halAFE_FELPF_15kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET,
    e_FELPF_50  = (halAFE_FELPF_50kHz << HAL_AFE_FEFCSFSEL_LSB),
    e_FELPF_100 = (halAFE_FELPF_50kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET,
    e_FELPF_150 = (halAFE_FELPF_150kHz << HAL_AFE_FEFCSFSEL_LSB),
    e_FELPF_300 = (halAFE_FELPF_150kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET,
#if (CHIP_REV >= 0xC0)
    e_FELPF_330 = (halAFE_FELPF_300kHz << HAL_AFE_FEFCSFSEL_LSB),
    e_FELPF_660 = (halAFE_FELPF_300kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET
#else
    e_FELPF_330 = (halAFE_FELPF_330kHz << HAL_AFE_FEFCSFSEL_LSB),
    e_FELPF_660 = (halAFE_FELPF_330kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET
#endif
};

enum
{
    e_TELPF_BYPASS = HAL_AFE_BPTELP,
    e_TELPF_15  = (halAFE_TELPF_15kHz << HAL_AFE_TEFCSFSEL_LSB),
    e_TELPF_30  = (halAFE_TELPF_15kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET,
    e_TELPF_50  = (halAFE_TELPF_50kHz << HAL_AFE_TEFCSFSEL_LSB),
    e_TELPF_100 = (halAFE_TELPF_50kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET,
    e_TELPF_150 = (halAFE_TELPF_150kHz << HAL_AFE_TEFCSFSEL_LSB),
    e_TELPF_300 = (halAFE_TELPF_150kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET,
#if (CHIP_REV >= 0xC0)
    e_TELPF_330 = (halAFE_TELPF_300kHz << HAL_AFE_TEFCSFSEL_LSB),
    e_TELPF_660 = (halAFE_TELPF_300kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET
#else
    e_TELPF_330 = (halAFE_TELPF_330kHz << HAL_AFE_TEFCSFSEL_LSB),
    e_TELPF_660 = (halAFE_TELPF_330kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET
#endif
};

/* The define the signals for HAL_AFE_SetMuxToAdc() */
typedef enum
{
    HAL_AFE_FE = 0,
    HAL_AFE_TE,
    HAL_AFE_BHSO,
    HAL_AFE_RP,
    HAL_AFE_SBAD,
    HAL_AFE_CE,
    HAL_AFE_SRFO,
    HAL_AFE_BS
} HAL_AFE_MuxSignalT;

/* The define the signals for HAL_AFE_SetPhBhDrooprate() */
enum
{
    e_PHS,
    e_BHS,
    e_RPPHBH
};

//*** External function prototypes
extern void HAL_AFE_Init( void );
extern BYTE HAL_AFE_GetTELpfBW( void );
extern void HAL_AFE_SetRfBw(USHORT OverspeedX10, USHORT BWRatioX100, BYTE TrackingMethod);
extern struct svoDpdBankFcVarT HAL_AFE_CalDPDBw(USHORT OverspeedX10, USHORT BwDPDRatioX100);
extern void HAL_AFE_SetDPDBw(USHORT OverspeedX10, USHORT BwDPDRatioX100);
extern void HAL_AFE_SetPhBhDrooprate(BYTE channel, float slope);
extern void HAL_AFE_SetWobbleSpeed(USHORT OverspeedX10, USHORT WlpfRatioX10, USHORT WhpfRatioX100);
extern BYTE HAL_AFE_SetMuxToAdc(HAL_AFE_MuxSignalT Signal);
#if (CHIP_REV >= 0xC0)
void  HAL_AFE_SetWobbleWriteOffset(void);
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetFELpfBW

    Description:    Set the FE LPF filter bandwidth
    Inputs:         Bandwidth
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_AFE_SetFELpfBW( BYTE Bandwidth )
{
    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,
                     (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK),
                      Bandwidth);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetTELpfBW

    Description:    Set the TE LPF filter bandwidth
    Inputs:         Bandwidth
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_AFE_SetTELpfBW( BYTE Bandwidth )
{
    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,
                     (HAL_AFE_TEFCSFSEL_MASK|HAL_AFE_TELPSET_MASK|HAL_AFE_BPTELP_MASK),
                      Bandwidth);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_AgcAocControl()

    Description:    Sets the control of the RF AGC and AOC to DFE or to the AFE
    Inputs:         AgcAocControl: parameter indicating whether the DFE or the AFE controls the
                    RF AOC and RF AGC
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_AFE_AgcAocControl(halAFE_RfAgcAocControl_t AgcAocControl)
{
    if( (AgcAocControl != READ_FIELD(HAL_AFE_RFAGCEN))||(AgcAocControl != READ_FIELD(HAL_AFE_RFAOCEN)) )
    {
        if(AgcAocControl == halAFE_RfAgcAoc_DfeControl) //DFE controls RF AGC/AOC
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x010201,B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        else//halAFE_RfAgcAoc_AfeControl                //AFE controls RF AGC/AOC
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x010202,B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        WRITE_FIELD(HAL_AFE_RFAGCEN,  AgcAocControl); // RF Auto Gain Control to the DFE
        WRITE_FIELD(HAL_AFE_RFAOCEN,  AgcAocControl); // RF Adaptive Offset Control to the DFE
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_WobVga3Control()

    Description:    Sets the control of Wobble VGA3 to DFE or to WBVGA3REG
    Inputs:         WobVga3Control: parameter indicating whether the DFE or WBVGA3REG controls the
                    Wobble VGA3
                    0: Wobble VGA3 controlled by WBVGA3REG
                    1: Wobble VGA3 controlled by DFE
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
__inline extern void HAL_AFE_WobVga3Control(halAFE_WobVga3Control_t WobVga3Control)
{
    WRITE_FIELD(HAL_AFE_WOBVGA3EN,  WobVga3Control); // Wobble VGA3 Controlled by DFE/WBVGA3REG
}

extern SBYTE HAL_AFE_CalcMbSbInpGain(USHORT MainBeamTargetX1000, USHORT SRFOPeakX1000, SHORT  SRFOSTMGaindBX10);
extern SBYTE HAL_AFE_CalcDPDInpGain(USHORT DPDTargetX1000, USHORT SRFOPeakX1000, SHORT  SRFOSTMGaindBX10);
extern BYTE  HAL_AFE_CalcWobbleInpGain(USHORT WBLTargetX1000, USHORT SRFOPeakX1000, SHORT  SRFOSTMGaindBX10);
extern SHORT HAL_AFE_GetRfGaindBX10(BYTE rfingn, BYTE rfgn);
extern BYTE  HAL_AFE_CalcRFGain(SHORT Rfgn_dBX10);
extern BYTE  HAL_AFE_CalcRFInpGain(USHORT RFSRFOTargetX1000, USHORT SRFOPeakX1000L0, USHORT SRFOPeakX1000L1, SHORT  SRFOSTMGaindBX10);

#endif /* _HAL_AFE_H_ */
