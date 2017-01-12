/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_pll.h
*
* DESCRIPTION
*   This file supports the HAL PLL funtions implemented in the PLL HAL layer
*
* NOTES:
*
* $Revision: 30 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_PLL_H_
#define _HAL_PLL_H_

#include ".\Common\globals.h" /* Needed for types of parameters */
#include ".\servo\SVO.H"
#include ".\al\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\al\REG_host.h"

#include <stdio.h> /*printf */

#define SIZE_WOCLKDIV_FIELD     4
#define SIZE_WIFCLKDIV_FIELD    4
#define SIZE_WRTCLKDIV_FIELD    4

#define SIZE_WOCLKDIV_FIELD     4
#define SIZE_WIFCLKDIV_FIELD    4
#define SIZE_WRTCLKDIV_FIELD    4

#define DIVFACT_DFE_IN_CLK      4
#define DIVFACT_WBL_CLK_CD_DVD  2
#define DIVFACT_WBL_CLK_BD_HD   4

#define FRQTRNSCALE_ARRAY_VALUE {8, 12, 16, 24, 32, 4, 2}
#define N_FRQTRNSCALE_ARRAY     7

/* Initalization of the SATA PLL block via a MACRO*/
/*#define HAL_PLL_InitSataPll(SATCLKIN, SATPLLBIS, SATPLLCFG, SATPLLTST, REFCLKMOD, SSCCTL) \
    WRITE_FIELD(HAL_HST_SATCLKIN,  SATCLKIN);   \
    WRITE_FIELD(HAL_HST_SATPLLBIS, SATPLLBIS);  \
    WRITE_FIELD(HAL_HST_SATPLLCFG, SATPLLCFG);  \
    WRITE_FIELD(HAL_HST_SATPLLTST, SATPLLTST);  \
    WRITE_FIELD(HAL_HST_REFCLKMOD, REFCLKMOD);  \
    WRITE_FIELD(HAL_HST_SSCCTL,    SSCCTL);
*/

//__inline extern void HAL_PLL_InitSataPll(BYTE Satclkin, BYTE Satpllbis, BYTE Satpllcfg, BYTE Satplltst, BYTE Refclkmod, BYTE Sscctl)
//{
//    WRITE_FIELD(HAL_HST_SATCLKIN,  Satclkin);
//    WRITE_FIELD(HAL_HST_SATPLLBIS, Satpllbis);
//    WRITE_FIELD(HAL_HST_SATPLLCFG, Satpllcfg);
//    WRITE_FIELD(HAL_HST_SATPLLTST, Satplltst);
//    WRITE_FIELD(HAL_HST_REFCLKMOD, Refclkmod);
//    WRITE_FIELD(HAL_HST_SSCCTL,    Sscctl);
//}

#if (CHIP_REV < 0xC0)
__inline extern void HAL_PLL_InitSataPll(BYTE Satclkin, BYTE Satpllbis, BYTE Satpllcfg, BYTE Satplltst, BYTE Refclkmod, BYTE Sscctl)
{
    WRITE_FIELD(HAL_HST_SATCLKIN,  Satclkin);
    WRITE_FIELD(HAL_HST_SATPLLBIS, Satpllbis);
    WRITE_FIELD(HAL_HST_SATPLLCFG, Satpllcfg);
    WRITE_FIELD(HAL_HST_SATPLLTST, Satplltst);
    WRITE_FIELD(HAL_HST_REFCLKMOD, Refclkmod);
    WRITE_FIELD(HAL_HST_SSCCTL,    Sscctl);
}
#else
__inline extern void HAL_PLL_InitSataPll(BYTE Satpllbis, BYTE Satplltst, BYTE Refclkmod, BYTE Sscctl)
{
    WRITE_FIELD(HAL_HST_SATPLLBIS, Satpllbis);
    WRITE_FIELD(HAL_HST_SATPLLTST, Satplltst);
    WRITE_FIELD(HAL_HST_REFCLKMOD, Refclkmod);
    WRITE_FIELD(HAL_HST_SSCCTL,    Sscctl);
}
#endif





/*** Local structures used in calculation of PLL HAL registers value***/
//define servo PLL clock table
typedef struct ClkPllS
{
    BYTE    Srvpllcfg0;
    BYTE    Srvpllcfg1;
    BYTE    Srvpllcfg2;
    BYTE    Srvpllck;
    USHORT  Srvpllcnl;
    USHORT  Srvpllcml;
} ClkPllcfgT;

typedef ClkPllcfgT ClkPllT[4];
typedef ClkPllT SrvClkPllT[2];

typedef struct PllCoefCalcS
  {
    BYTE Safety_CLV;
    BYTE Safety_CAV;
    BYTE BaseN;
    BYTE BaseM;
    BYTE AdcClkDiv;                             /* selects the divider used on output of the ADC PLL */
    BYTE WrtClkDivValue;
    BYTE Qpr;                                   /* ADC PLL Charge Pump Range */
    BYTE Qpl;                                   /* ADC PLL Charge Pump Scale Factor */
    BYTE RSet;                                  /* ADC PLL Loop Filter Resistor Select */
    BYTE WblAdcClkDivValue;
    ULONG Bitrate;
  } PllCoefCalcT;

typedef struct PllCoefInputS
  {
    PllCoefCalcT PllCoefCalc;
    BYTE WoClkDiv;                              /* Wobble Outerloop Clock Divider Select */
    BYTE WifClkDiv;                             /* Wobble Interface Clock Divider Select */
    BYTE WrtClkDiv;
  } PllCoefInputT;

typedef struct PllCoefS
  {
    PllCoefCalcT PllCoefCalc;
    BYTE WoClkDiv[SIZE_WOCLKDIV_FIELD];         /* Wobble Outerloop Clock Divider Select */
    BYTE WifClkDiv[SIZE_WIFCLKDIV_FIELD];       /* Wobble Interface Clock Divider Select */
    BYTE WrtClkDiv[SIZE_WRTCLKDIV_FIELD];
  } PllCoefT;

typedef struct PllTabCoefS
  {
    PllCoefT Cd;                                /* COEFS values for CD */
    PllCoefT Dvd;                               /* COEFS values for DVD */
    PllCoefT DvdRam;                            /* COEFS values for DVRAM */
    PllCoefT Bd;                                /* COEFS values for BD */
    PllCoefT HdDvd;                             /* COEFS values for HDDVD */
  } PllTabCoefT;

//define ADC PLL table
typedef struct PllCalcOutputS
  {
    USHORT  AdcPllN;            /* 10-bit N value used to set the parameters of the ADC Clock PLL */
    USHORT  AdcPllM;            /* 10-bit M value used to set the parameters of the ADC Clock PLL */
    BYTE    AdcPllK;            /* 3-bit K value used to set the parameters of the ADC Clock PLL */
    BYTE    PllKo;              /* PLL VCO voltage-frequency gain */
    BYTE    EfmDiv;
    BYTE    FrqTrnScale;
    BYTE    EfmRefDiv;
  } PllCalcOutputT;

//DSP power mode                // Define a related enum in REG_a7soc.h?
enum
{
    e_NORMAL_MODE,
    e_LOW_POWER1_MODE,
    e_FULL_SPEED_MODE,
    e_LOW_POWER2_MODE
};

//Servo Clock Stop              // Define a related enum in REG_a7soc.h?
enum                            // Define a related enum in REG_a7soc.h?
{
    e_SRVCKSTP_NORMAL_OPER,     // Normal operation
    e_SRVCKSTP_STOP_CLK         // Stop clock—Default
};

// CLOCK SOURCE
enum                            // Define a related enum in REG_a7soc.h?
{
    e_SRVCLKSRC_25_MHz,         // 25 MHz clock
    e_SRVCLKSRC_33_MHZ          // 33.8688 MHz clock
};

enum                            // Define a related enum in REG_a7soc.h?
{
    e_ADCPLLDB_NUL,
    e_ADCPLLDB_DEFAULT
};


//*** External function prototypes
extern void HAL_PLL_Init(void);
extern void HAL_PLL_InitSvoPll(BYTE DspPowerMode);
extern void HAL_PLL_InitAdcPll( void );
extern void HAL_PLL_SetAdcPll( PllCoefInputT *PllCoefInput, bool IsCLVSpeed, USHORT OverspeedX10 );
extern ULONG HAL_PLL_GetHfAdcClkKHz(void);
extern ULONG HAL_PLL_GetDfeInClkKHz(void);
extern ULONG HAL_PLL_GetWblClkKHz(void);
extern ULONG HAL_PLL_GetServoClkKHz(void);
extern ULONG HAL_PLL_GetOpcClkKHz( void );

/*-----------------------------------------------------------------------------------------------------
    Name:           IsDVDMinusDisc

    Description:
    Inputs:
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/

extern __inline BOOL IsDVDMinusDisc(eDiscKind medium)
{
        BOOL ans;
        ans= ((medium == eDVDMinusRDisc) || (medium == eDVDMinusRWDisc) ||
                      (medium == eDVDMinusRDLDisc)||(medium == eDVDMinusRWDLDisc));

        return(ans);
}


#endif /* _HAL_PLL_H_ */

