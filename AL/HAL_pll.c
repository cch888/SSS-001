/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2006 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*     HAL_pll.c
*
*  Description:
*   This file contains functions for the PLL subsystem and includes
*   the following:
*           HAL_PLL_Init
*           HAL_PLL_Set
*
* Notes:
*
*
*   $Revision: 69 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_wobble.h"
#include ".\AL\HAL_pll.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_dsp.h"
#include ".\common\commath.h"
#include ".\hif\gp_ram.h"
#include ".\hif\powerctl.h"

/*** Local defines ***/
#define EFMCLK_NS_MIN           8
#define EFMCLK_NS_MAX           56
#define EFMCLK_VCO_MIN          270
#define EFMCLK_VCO_MAX          1080
#define EFMCLK_VCO_MIN_RETRY    200
#define EFMREFCLK_MIN           16
#define EFMREFCLK_MAX           33

#define CLOCK_25_MHZ            25

#define CAV_DIVIDERX100         241     /* Divider in case if CAV speed */
#define MIN_FVCO                100     /* range 1...1200. Frequency in MHz */
#define FVCO_KO_STEPS           250     /* steps (in Mhz) to determine the gain to use w.r.t. the value of the VCO frequency */
#define MAX_APLLKO_VALUE        3       /* HAL_GLOBAL_APLLKO_REGISTER is 2 bits, maximum value is 3 */
#define ADCPLL_MHZ_LIMIT1       500
#define ADCPLL_MHZ_LIMIT2       540

#define EFMREFDIV_ARRAY_VALUE   {1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64 }
#define N_EFMREFDIV_ARRAY       11

#define EFMDIV_ARRAY_VALUE      {1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64 }
#define N_EFMDIV_ARRAY          11

#define TWO_TO_POWER_18 262144 /* 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2 */
#define TWO_TO_POWER_15 32768  /* 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2 */


//*** Local function prototypes
void CalcPllAdcParameters( PllCoefCalcT *PllCoef, bool IsCLVSpeed, USHORT OverspeedX10, PllCalcOutputT *CalcOutputValues );

/*** Local static variables */
static ULONG        mAdcPllClkKHz;  // the Adc Pll Clock value in kHz
static ULONG        mHFAdcClkKHz;   // the HF Adc Pll Clock value in kHz
static ULONG        mSrvClkKHz;     // the Servo Clock value in kHz

ULONG               mDfeInClkKHz;   // the DFE in Clock value in kHz
ULONG               mWblClkKHz;     // the Wobble Clock value in kHz


/*** global variables ***/
//#define N_SRV_PLL_CFG_TBL           8

SrvClkPllT SrvpllClockTable = {
//          CFG0    CFG1    CFG2    K       NL      ML
    {   // 25 MHz clock : e_SRVCLKSRC_25_MHz
        {   0x81,   0x41,   0x00,   0x01,   0x14F,  0x01E}, //SRVCLKSRC=0, PWMOD=00
        {   0x81,   0x41,   0x00,   0x02,   0x14F,  0x01E}, //SRVCLKSRC=0, PWMOD=01
        {   0x81,   0x41,   0x00,   0x01,   0x1F7,  0x01E}, //SRVCLKSRC=0, PWMOD=10
        {   0x81,   0x41,   0x00,   0x02,   0x14F,  0x01E} //SRVCLKSRC=0, PWMOD=11
    },
    {   // 33.8688 MHz clock: e_SRVCLKSRC_33_MHZ
        {   0x81,   0x41,   0x00,   0x01,   0x0F7,  0x01E}, //SRVCLKSRC=1, PWMOD=00
        {   0x81,   0x41,   0x00,   0x02,   0x0F7,  0x01E}, //SRVCLKSRC=1, PWMOD=01
        {   0x81,   0x41,   0x00,   0x01,   0x173,  0x01E}, //SRVCLKSRC=1, PWMOD=10
        {   0x81,   0x41,   0x00,   0x02,   0x0F7,  0x01E}  //SRVCLKSRC=1, PWMOD=11
    }
};

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_Init

    Description:    Initialization of the pll module/block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_PLL_Init(void)
{
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_InitSvoPll

    Description:    Initializes the servo clock frequencies.
                    These clock frequencies are depending on the desired power mode.
    Inputs:         DspPowerMode:   desired power mode
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_PLL_InitSvoPll(BYTE DspPowerMode)
{
    BYTE PwMode;
    BYTE SrvClkSrcValue;

    //Check PLL clock not to be in STOP mode
    WRITE_FIELD(HAL_GLOBAL_SRVCKSTP, e_SRVCKSTP_NORMAL_OPER);

    PwMode = DspPowerMode;
    //Select PLL input clock flag
    if(INPUT_CLOCK == CLOCK_25_MHZ)
    {
        SrvClkSrcValue = e_SRVCLKSRC_25_MHz;
    }
    else
    {
        SrvClkSrcValue = e_SRVCLKSRC_33_MHZ;
    }

    //Select PLL input clock
    WRITE_FIELD(HAL_GLOBAL_SRVCLKSRC, SrvClkSrcValue);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLCFG0, SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcfg0);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLCFG1, SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcfg1);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLCFG2, SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcfg2);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLK   , SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllck);
    //WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLNH  ,SrvpllClockTable[PwMode].Srvpllcnh);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLNL  , SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcnl);
    //WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLMH  ,SrvpllClockTable[PwMode].Srvpllcmh);
    WRITE_REG(HAL_GLOBALREG_MAP->SRVPLLML  , SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcml);

    //Calculate ServoClock frequency in kHz
    mSrvClkKHz = (ULONG) INPUT_CLOCK_HZ / ( (SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcml + 1) * (1 << SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllck) ) * (SrvpllClockTable[SrvClkSrcValue][PwMode].Srvpllcnl + 1) / 1000;

    for(PwMode = 0; PwMode < 25; PwMode++){} // wait for SVOCLK PLL stabilisation

    ServoPllInited = TRUE;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_InitAdcPll

    Description:    Initalization of the ADC PLL block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_PLL_InitAdcPll( void )
{
// TBD? No Code!
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_GetHfAdcClkKHz()

    Description:    returns HFAdcClkKHz
    Inputs:         None
    Outputs:        None
    Global Inputs:
    Global Outputs: None
    Returns:        HFAdcClkKHz
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_PLL_GetHfAdcClkKHz( void )
{
    return mHFAdcClkKHz;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_GetDfeInClkKHz()

    Description:    returns DfeClkKHz
    Inputs:         None
    Outputs:        None
    Global Inputs:
    Global Outputs: None
    Returns:        DfeClkKHz
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_PLL_GetDfeInClkKHz( void )
{
    return mDfeInClkKHz;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_GetWblClkKHz()

    Description:    returns WblClkKHz
    Inputs:         None
    Outputs:        None
    Global Inputs:
    Global Outputs: None
    Returns:        WblClkKHz
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_PLL_GetWblClkKHz( void )
{
    return mWblClkKHz;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_GetServoClkKHz()

    Description:    returns SrvClkKHz
    Inputs:         None
    Outputs:        None
    Global Inputs:
    Global Outputs: None
    Returns:        SrvClkKHz
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_PLL_GetServoClkKHz( void ) // TBD? __inline
{
    return mSrvClkKHz;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_GetServoClkKHz()

    Description:    returns SrvClkKHz
    Inputs:         None
    Outputs:        None
    Global Inputs:
    Global Outputs: None
    Returns:        SrvClkKHz
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_PLL_GetOpcClkKHz( void ) // TBD? __inline
{
    return mSrvClkKHz / 16; // Normally it should be servo/8
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_PLL_SetAdcPll

    Description:    Sets the ADC PLL registers depending on the actual disc kind
    Inputs:         PllCoefInput :      coef values
                    IsCLVSpeed :        indicates whether the speed is a CLV speed
                    OverspeedX10 :      current overspeed value times 10
    Outputs:        None
    Global Inputs:  mDiscKind to determine the set of coefficients/constants to use for calculation
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_PLL_SetAdcPll( PllCoefInputT *PllCoefInput, bool IsCLVSpeed, USHORT OverspeedX10 )
{
    /* Structure of calculated values to set some of the PLL registers */
    PllCalcOutputT  CalcOutputValues;

    /* Initialisations for WRITE_FIELDTO */
    ULONG   CLKCFG3_reg     = 0;
    ULONG   ADCPLLCFG0_reg  = 0;
    ULONG   ADCPLLCFG1_reg  = 0;
    ULONG   WPLLCFG0_reg    = 0;

    /*
     * Calculation to determine the values of some of the PLL registers
     */
    CalcPllAdcParameters( &(PllCoefInput->PllCoefCalc), IsCLVSpeed, OverspeedX10, &CalcOutputValues );

#if (HARDCODED_ADC_PLL)
    switch (mDiscKind.Bit.DiscStd)
    {
    #if (BD_ENABLE == 1)
        case eDiscBD:
            // setup for 4x BD speed:
            CalcOutputValues.AdcPllN = 99;
            CalcOutputValues.AdcPllM = 12;
            CalcOutputValues.AdcPllK = 0;
            CalcOutputValues.PllKo = 0;
            CalcOutputValues.EfmDiv = 6;
            CalcOutputValues.FrqTrnScale = 0;
            CalcOutputValues.EfmRefDiv = 4;
            break;


/*            // setup for 2x BD speed:
            CalcOutputValues.AdcPllN = 99;
            CalcOutputValues.AdcPllM = 12;
            CalcOutputValues.AdcPllK = 1;
            CalcOutputValues.PllKo = 0;
            CalcOutputValues.EfmDiv = 6;
            CalcOutputValues.FrqTrnScale = 0;
            CalcOutputValues.EfmRefDiv = 4;
            break;
*/

    #endif // (BD_ENABLE == 1)

    #if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (OverspeedX10 == 20)  // current speed 2X
            {   //just copy from BD, need check
                // setup for 6x BD speed:
                CalcOutputValues.AdcPllN = 149;
                CalcOutputValues.AdcPllM = 12;
                CalcOutputValues.AdcPllK = 0;
                CalcOutputValues.PllKo = 1;
                CalcOutputValues.EfmDiv = 6;
                CalcOutputValues.FrqTrnScale = 0;
                CalcOutputValues.EfmRefDiv = 4;
            }
            else   // current speed 1X or NONE
            {
                // setup use 4x BD speed:
                CalcOutputValues.AdcPllN = 99;
                CalcOutputValues.AdcPllM = 12;
                CalcOutputValues.AdcPllK = 0;
                CalcOutputValues.PllKo = 0;
                CalcOutputValues.EfmDiv = 6;
                CalcOutputValues.FrqTrnScale = 0;
                CalcOutputValues.EfmRefDiv = 4;
            }
            break;
    #endif // (ENABLE_HDDVD == 1)

        case eDiscCD://Hardcode:select speedX2      //SpeedX1   //SpeedX2   //SpeedX3   //SpeedX4
            CalcOutputValues.AdcPllN = 95;          //95        //95        //71        95
            CalcOutputValues.AdcPllM = 15;          //15        //15        //15        15
            CalcOutputValues.AdcPllK = 2;           // 3        // 2        // 1         1
            CalcOutputValues.PllKo = 0;             // 0        // 0        // 0         0
            CalcOutputValues.EfmDiv = 6;            // 8        // 6        // 6         4
            CalcOutputValues.FrqTrnScale = 0;       // 0        // 0        // 0         0
            CalcOutputValues.EfmRefDiv = 1;         // 0        // 1        // 1         2
            break;

        case eDiscDVD:
        default:
            // setup for 4x BD speed:
            CalcOutputValues.AdcPllN = 99;
            CalcOutputValues.AdcPllM = 12;
            CalcOutputValues.AdcPllK = 0;
            CalcOutputValues.PllKo = 0;
            CalcOutputValues.EfmDiv = 6;
            CalcOutputValues.FrqTrnScale = 0;
            CalcOutputValues.EfmRefDiv = 4;
            break;
    }
    /*
    // setup for 1x BD speed:
    CalcOutputValues.AdcPllN = 99;
    CalcOutputValues.AdcPllM = 12;
    CalcOutputValues.AdcPllK = 2;
    CalcOutputValues.PllKo = 0;
    CalcOutputValues.EfmDiv = 8;
    CalcOutputValues.FrqTrnScale = 0;
    CalcOutputValues.EfmRefDiv = 2; */
#endif  // HARDCODED_ADC_PLL

    /* Update the value of the clocks */
    mAdcPllClkKHz = (ULONG)(1000*INPUT_CLOCK*(CalcOutputValues.AdcPllN + 1)/((CalcOutputValues.AdcPllM + 1)*(1 << CalcOutputValues.AdcPllK)));
    mHFAdcClkKHz  = (ULONG)(mAdcPllClkKHz)  / (ULONG)(1 << PllCoefInput->PllCoefCalc.AdcClkDiv);
    mDfeInClkKHz  = mHFAdcClkKHz / DIVFACT_DFE_IN_CLK;

    /*
     * Assign registers: !!! IT IS ASSUMED THAT ORDER IS NOT AN ISSUE !!!
     */

    /* Set CLKCFG1: CLOCK CONFIGURATION 1 REGISTER
     * Set ADCCLKDIV field:
     *      Selects the divider used on output of the ADC PLL. All clocks
     *      related to the ADC PLL are derived from the divided ADC clock
     *      selected by ADCCLKDIV
     */
    WRITE_FIELD(HAL_GLOBAL_ADCCLKDIV, PllCoefInput->PllCoefCalc.AdcClkDiv);


    /* Set CLKCFG2: CLOCK CONFIGURATION 2 REGISTER !!! Unchanged !!!
     * READ_MOD_WRITE(HAL_GLOBALREG_MAP->CLKCFG2, ..., ...);
     */


    /* Set CLKCFG3: CLOCK CONFIGURATION 3 REGISTER
     * Set WRTCLKDIV field:
     * Set WIFCLKDIV field: Wobble Interface Clock Divider Select
     * Set WOCLKDIV  fiels: Wobble Outerloop Clock Divider Select
     */
    CLKCFG3_reg = READ_REG(HAL_GLOBALREG_MAP->CLKCFG3);
    WRITE_FIELDTO(CLKCFG3_reg,HAL_GLOBAL_WRTCLKDIV, PllCoefInput->WrtClkDiv);
    WRITE_FIELDTO(CLKCFG3_reg,HAL_GLOBAL_WIFCLKDIV, PllCoefInput->WifClkDiv);
    WRITE_FIELDTO(CLKCFG3_reg,HAL_GLOBAL_WOCLKDIV, PllCoefInput->WoClkDiv);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKCFG3, CLKCFG3_reg);

    /* Set ADCPLLCFG0: ADC PLL CONFIGURATION 0 REGISTER
     * Set ADCPLLDB field:
     * Set APLLQPL field:   ADC PLL Charge Pump Scale Factor
     */
    ADCPLLCFG0_reg = READ_REG(HAL_GLOBALREG_MAP->ADCPLLCFG0);
    WRITE_FIELDTO(ADCPLLCFG0_reg,HAL_GLOBAL_ADCPLLDB, e_ADCPLLDB_DEFAULT);
    WRITE_FIELDTO(ADCPLLCFG0_reg,HAL_GLOBAL_APLLQPL, PllCoefInput->PllCoefCalc.Qpl);
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLCFG0, ADCPLLCFG0_reg);

    WRITE_FIELD(HAL_GLOBAL_APLLQPL, PllCoefInput->PllCoefCalc.Qpl);

    /* Set ADCPLLCFG1: ADC PLL CONFIGURATION 1 REGISTER
     * Set APLLKO field: ADC PLL VCO Gain Adjust
     * Set ALFRSET field: ADC PLL Loop Filter Resistor Select
     * Set APLLQPR field: ADC PLL Charge Pump Range
     */
    ADCPLLCFG1_reg = READ_REG(HAL_GLOBALREG_MAP->ADCPLLCFG1);
    WRITE_FIELDTO(ADCPLLCFG1_reg,HAL_GLOBAL_APLLKO, CalcOutputValues.PllKo);
    WRITE_FIELDTO(ADCPLLCFG1_reg,HAL_GLOBAL_ALFRSET, PllCoefInput->PllCoefCalc.RSet);
    WRITE_FIELDTO(ADCPLLCFG1_reg,HAL_GLOBAL_APLLQPR, PllCoefInput->PllCoefCalc.Qpr);
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLCFG1, ADCPLLCFG1_reg);

    /* Set ADCPLLCFG2: ADC PLL CONFIGURATION 2 REGISTER */
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLCFG2, 0 );

    /* Set ADCPLLK: ADC PLL K VALUE REGISTER */
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLK, CalcOutputValues.AdcPllK );

    /* Set ADCPLLNL/ADCPLLNH: ADC PLL N VALUE REGISTERS */
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLNL, CalcOutputValues.AdcPllN );

    /* Set ADCPLLML/ADCPLLMH: ADC PLL M VALUE REGISTERS */
    WRITE_REG(HAL_GLOBALREG_MAP->ADCPLLML, CalcOutputValues.AdcPllM );

    // disable the PLL before reprogramming any of the dividers
    // I don't know why, but disable this section of code causes system hang after power off/on
#if (EN_POWER_CONTROL == 0)
    WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1); //power-down write PLL
#endif

    /* Set WPLLCFG0: WRITE CLOCK PLL CONFIGURATION 0 REGISTER
     * Set EFMDIV    field: Select for divider on PLL output
     * Set EFMREFDIV field: Select for divider on PLL reference clock input
     */
    WPLLCFG0_reg = READ_REG(HAL_GLOBALREG_MAP->WPLLCFG0);
    WRITE_FIELDTO(WPLLCFG0_reg,HAL_GLOBAL_EFMDIV,CalcOutputValues.EfmDiv);
    WRITE_FIELDTO(WPLLCFG0_reg,HAL_GLOBAL_EFMREFDIV,CalcOutputValues.EfmRefDiv);
    WRITE_REG(HAL_GLOBALREG_MAP->WPLLCFG0, WPLLCFG0_reg);


    /* CD Disc Type:
     * Set CDWCGCFG: CD WRITE CLOCK GENERATOR CONFIGURATION REGISTER
     * Set CDFTRNSCL field:
     *
     * Others Disc Type:
     * Set WCGCFG1: WRITE CLOCK GENERATOR CONFIGURATION 1 REGISTER
     * Set FRQTRNSCL field:
     */
    switch (mDiscKind.fDiscKind)
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
#if (EN_POWER_CONTROL)
            if (!WobbleCLKStop)
#endif
                WRITE_FIELD(HAL_WBL_CDFTRNSCL,CalcOutputValues.FrqTrnScale);

            /* Update the value of the wobble clock */
            mWblClkKHz = mHFAdcClkKHz / DIVFACT_WBL_CLK_CD_DVD;
            break;

        case eDVDROMDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
#endif
#if (EN_POWER_CONTROL)
            if (!WobbleCLKStop)
#endif
                WRITE_FIELD(HAL_WBL_FRQTRNSCL,CalcOutputValues.FrqTrnScale);

            /* Update the value of the wobble clock */
            mWblClkKHz = mHFAdcClkKHz / DIVFACT_WBL_CLK_CD_DVD;
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
#endif
#if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDROMDLDisc:
        case eHDRDLDisc:
        case eHDRWDLDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
        case eCBHDROMDLDisc:
    #endif
#endif
#if (EN_POWER_CONTROL)
            if (!WobbleCLKStop)
#endif
                WRITE_FIELD(HAL_WBL_FRQTRNSCL,CalcOutputValues.FrqTrnScale);

            /* Update the value of the wobble clock */
            mWblClkKHz = mHFAdcClkKHz / DIVFACT_WBL_CLK_BD_HD;
            break;

        default:
        break;
    } /* ENDSWITCH: mDiscKind */

//    need disable here because of write pll lock-up; WPLL will be enabled by HAL_WBL_SetWPLLMode
//    WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 0); //power-up write PLL
}

/*-----------------------------------------------------------------------------------------------------
    Name:           CalcPllAdcParameters

    Description:    Calculation to determine the values of some of the PLL registers
    Inputs:         PllCoef :           coef values used in the calculation
                    IsCLVSpeed :        indicates whether the speed is a CLV speed
                    OverspeedX10 :      current overspeed value times 10
    Outputs:        CalcOutputValues :  variables to set the value of some of the PLL registers
    Global Inputs:  mDiscKind to determine the set of coefficients/constants to use for calculation
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/

// TBD? COMMENT! temp solution --> if these declarations are inside the function CalcPllAdcParameters then too much stack is consumed which could cause a stack-overflow
static ULONG   NsStartX10, NsStopX10;
static USHORT  NFactor, AdcPllClkMHz;                                              /* range: 1 .. 1024 */
static USHORT  EfmDivValue;                                                        /* range: 8 .. 16*Max(EFMDIV_ARRAY_VALUE) */
static BYTE    KFactor;                                                            /* range: 0 .. 7 */
static BYTE    EfmDiv, TempEfmDiv;                                                 /* range: 0 .. N_EFMDIV_ARRAY */
static BYTE    EfmDivArray[N_EFMDIV_ARRAY] = EFMDIV_ARRAY_VALUE;                   /* range: 1 .. Max(EFMDIV_ARRAY_VALUE) */
static BYTE    FrqTrnScale, TempFrqTrnScale;                                       /* range: 0 .. N_FRQTRNSCALE_ARRAY */
static BYTE    FrqTrnScaleValue;                                                   /* range: 2 .. Max(FRQTRNSCALE_ARRAY_VALUE) */
static BYTE    FrqTrnScaleArray[N_FRQTRNSCALE_ARRAY] = FRQTRNSCALE_ARRAY_VALUE;    /* range: 2 .. Max(FRQTRNSCALE_ARRAY_VALUE) */
static BYTE    EfmRefDiv, TempEfmRefDiv;                                           /* range: 0 .. N_EFMREFDIV_ARRAY */
static BYTE    EfmRefDivValue;                                                     /* range: 1 .. Max(EFMREFDIV_ARRAY_VALUE) */
static BYTE    EfmRefDivArray[N_EFMREFDIV_ARRAY] = EFMREFDIV_ARRAY_VALUE;          /* range: 1 .. Max(EFMREFDIV_ARRAY_VALUE) */
static BYTE    Index;                                                              /* range: 0 .. N_EFMDIV_ARRAY */
static bool    ModuloCheckSucceeded;                                               /* indication whether the modulo check succeeded in the EfmDiv evaluation */
static bool    SettingOK, SettingAlmostOK;

void CalcPllAdcParameters( PllCoefCalcT *PllCoef, bool IsCLVSpeed, USHORT OverspeedX10, PllCalcOutputT *CalcOutputValues )
{

    /* evaluate AdcPllN, AdcPllM, AdcPllK */
    KFactor = 0;
    NFactor = (OverspeedX10 * (IsCLVSpeed? PllCoef->Safety_CLV : PllCoef->Safety_CAV) * PllCoef->BaseN) / 100;

    AdcPllClkMHz = (USHORT)(INPUT_CLOCK*NFactor)/(PllCoef->BaseM);
    if (AdcPllClkMHz > ADCPLL_MHZ_LIMIT1)
    {
        //beyond LIMIT1 value, we can avoid bad samples by timing adjust
#if (CHIP_REV >= 0xC1)
        if (mDiscKind.Bit.DiscStd == eDiscBD)
        {
            WRITE_FIELD(HAL_GLOBAL_RFADCOTA, 2);
        }
        else
#endif        
        WRITE_FIELD(HAL_GLOBAL_RFADCOTA, 1);
    }
    else
    {
        WRITE_FIELD(HAL_GLOBAL_RFADCOTA, 0);
    }
    if (AdcPllClkMHz > ADCPLL_MHZ_LIMIT2)
    {
        //beyond LIMIT2 value, we always have bad ADC samples
        //must apply upper max limit to ADC clock to avoid bad ADC samples
        NFactor = (USHORT)((ADCPLL_MHZ_LIMIT2 * (PllCoef->BaseM))/INPUT_CLOCK);
    }

    /* frequency synthesis function of the PLL clock:
     *      fvco(clkout) = INPUT_CLOCK * (AdcPllN+1) / (AdcPllM+1)
     */
    /* while (fvco < MIN_FVCO) */

    while ((INPUT_CLOCK * NFactor) < (MIN_FVCO * PllCoef->BaseM))
    {
        KFactor = KFactor + 1;
        NFactor = NFactor * 2;
    } /* ENDWHILE: (INPUT_CLOCK * NFactor) < (MIN_FVCO * PllCoef->BaseM) */

    CalcOutputValues->AdcPllN = NFactor - 1;
    CalcOutputValues->AdcPllM = (USHORT)(PllCoef->BaseM - 1);
    CalcOutputValues->AdcPllK = KFactor;

    /* evaluate EfmRefDiv, frqtrnscale, EfmDiv */
    SettingOK = FALSE;
    SettingAlmostOK = FALSE;
    EfmRefDiv = 0;

    EfmRefDivValue  = EfmRefDivArray[EfmRefDiv];

    while ((!SettingOK) && ((INPUT_CLOCK * NFactor) > (EFMREFCLK_MIN * EfmRefDivValue * PllCoef->BaseM * (1 << KFactor))))
    {
        /* efmrefclk = fvco(clkout) / ((2^k_factor) * EfmRefDivValue) */
        if ((INPUT_CLOCK * NFactor) < (EFMREFCLK_MAX * EfmRefDivValue * PllCoef->BaseM * (1 << KFactor)))
        {
            FrqTrnScale = 0;

            while (FrqTrnScale  < N_FRQTRNSCALE_ARRAY)
            {
                FrqTrnScaleValue = FrqTrnScaleArray[FrqTrnScale];

                /* check if valid value for EfmDiv can be made: first check modulo */
                ModuloCheckSucceeded = FALSE;
                if (((4 * PllCoef->WrtClkDivValue * FrqTrnScaleValue)%EfmRefDivValue) == 0)
                {
                    EfmDivValue = (4 * PllCoef->WrtClkDivValue * FrqTrnScaleValue) / EfmRefDivValue;
                    for(Index = 0; Index < N_EFMDIV_ARRAY; Index++)
                    {
                        if (EfmDivArray[Index]== EfmDivValue)
                        {
                            EfmDiv = Index;
                            Index = N_EFMDIV_ARRAY;
                            ModuloCheckSucceeded = TRUE;
                        }
                    }
                }
                if (ModuloCheckSucceeded)
                {
                    NsStopX10 = OverspeedX10 * PllCoef->Bitrate * FrqTrnScaleValue / 10000 * (PllCoef->WblAdcClkDivValue) * (1 << PllCoef->AdcClkDiv); //000000;
                    if (!(IsCLVSpeed))
                        NsStartX10 = (100 * NsStopX10 / CAV_DIVIDERX100);
                    else
                        NsStartX10 = NsStopX10;

                    if ( ((ULONG)(EFMCLK_NS_MIN * INPUT_CLOCK * NFactor * 10) < (ULONG)(NsStartX10 * PllCoef->BaseM * (1 << KFactor))) && ((ULONG)(NsStartX10 * PllCoef->BaseM * (1 << KFactor)) < (ULONG)(EFMCLK_NS_MAX * INPUT_CLOCK * NFactor * 10))   &&
                         ((ULONG)(EFMCLK_NS_MIN * INPUT_CLOCK * NFactor * 10) < (ULONG)(NsStopX10  * PllCoef->BaseM * (1 << KFactor))) && ((ULONG)(NsStopX10  * PllCoef->BaseM * (1 << KFactor)) < (ULONG)(EFMCLK_NS_MAX * INPUT_CLOCK * NFactor * 10)) )
                    {
                        if ( ((ULONG)(EFMCLK_VCO_MIN * EfmRefDivValue * 10) < NsStartX10) && (NsStartX10 < (ULONG)(EFMCLK_VCO_MAX * EfmRefDivValue * 10)) &&
                             ((ULONG)(EFMCLK_VCO_MIN * EfmRefDivValue * 10) < NsStopX10)  && (NsStopX10  < (ULONG)(EFMCLK_VCO_MAX * EfmRefDivValue * 10)) )
                        {
                            SettingOK = TRUE;
                            break;
                        }
                        else if ( ((ULONG)(EFMCLK_VCO_MIN_RETRY * EfmRefDivValue * 10) < NsStartX10) && (NsStartX10 < (ULONG)(EFMCLK_VCO_MAX * EfmRefDivValue * 10)) &&
                                  ((ULONG)(EFMCLK_VCO_MIN_RETRY * EfmRefDivValue * 10) < NsStopX10)  && (NsStopX10  < (ULONG)(EFMCLK_VCO_MAX * EfmRefDivValue * 10)) )
                        {
                            SettingAlmostOK = TRUE;
                            TempEfmDiv = EfmDiv;
                            TempFrqTrnScale = FrqTrnScale;
                            TempEfmRefDiv = EfmRefDiv;
                        }
                    }
                } /* if (ModuloCheckSucceeded) */

                FrqTrnScale++;
            } /* ENDWHILE: ((FrqTrnScale  < N_FRQTRNSCALE_ARRAY) && (!StopCondition))*/

        } /* if ((INPUT_CLOCK * NFactor) < (EFMREFCLK_MAX * EfmRefDivValue * PllCoef->BaseM * (1 << KFactor))) */

        if (!SettingOK)
        {
            EfmRefDiv++;
            EfmRefDivValue = EfmRefDivArray[EfmRefDiv];
        } /* if (!SettingOK) */

    } /* ENDWHILE: ((!SettingOK) && ((INPUT_CLOCK * NFactor) > (EFMREFCLKX10_MIN * EfmRefDivValue * PllCoef->BaseM * (1 << KFactor)))) */

    /* evaluate the results from EfmRefDiv, FrqTrnScale, and EfmDiv calculation*/
    if (SettingOK)
    {
        CalcOutputValues->EfmDiv        = EfmDiv;
        CalcOutputValues->FrqTrnScale   = FrqTrnScale;
        CalcOutputValues->EfmRefDiv     = EfmRefDiv;
    }
    else
    {
        CalcOutputValues->EfmDiv        = TempEfmDiv;
        CalcOutputValues->FrqTrnScale   = TempFrqTrnScale;
        CalcOutputValues->EfmRefDiv     = TempEfmRefDiv;
        //if (!SettingAlmostOK)
        /* TBD! report error */
    }

    /* evaluate PLL VCO Gain Adjust: PllKo */
    if ((INPUT_CLOCK * NFactor) > (MAX_APLLKO_VALUE * PllCoef->BaseM * FVCO_KO_STEPS))
    {
        CalcOutputValues->PllKo = MAX_APLLKO_VALUE;
    }
    else
    {
        CalcOutputValues->PllKo = (BYTE)((INPUT_CLOCK * NFactor)/(PllCoef->BaseM*FVCO_KO_STEPS));
    }
}
