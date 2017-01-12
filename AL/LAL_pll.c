/* ******************************************************************************************
*
*  (c) Copyright 2006 - 2007 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*  LAL_pll.c
*
*  Description:
*  This file contains functions for the LAL PLL layers and includes
*  the following:
*
*           .
*
* Notes:
*
*
*   $Revision: 68 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\Common\globals.h" /* Needed for types of parameters */
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H"
#include ".\servo\svo_spdtrans.h"
#include ".\al\coefs.h"
#include ".\al\LAL_pll.h"
#include ".\al\HAL_pll.h"
#include ".\AL\REG_HAL.h"
#include ".\al\REG_host.h"

/*** Local defines ***/

/* Global definitions regarding COEF */
static const PllTabCoefT       mPllCoefs = PLL_COEFS;
static       PllCoefInputT     mPllCoefInput;

static void GetSpeedAndPllCoefInit(const PllTabCoefT *PllCoefs, PllCoefInputT *PllCoefInput, bool *IsClvSpeed, USHORT *OverSpeedX10);
static void GetSpeedAndPllCoef(const PllTabCoefT *PllCoefs, PllCoefInputT *PllCoefInput, bool *IsClvSpeed, USHORT *OverSpeedX10);
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
extern BYTE   BitDetCFG_index;
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_PLL_InitServo

    Description:    Initialise the appropriate PLL registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: PLL Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_PLL_InitServo( void )
{
    bool               IsClvSpeed;
    USHORT          OverSpeedX10;

    HAL_PLL_Init(); //global init of PLL

    /* SERVO PLL Init */
    //HAL_PLL_InitSvoPll(e_NORMAL_MODE);

    /* DFE/WOBBLE PLL Init */
    GetSpeedAndPllCoefInit(&mPllCoefs, &mPllCoefInput, &IsClvSpeed, &OverSpeedX10);
    HAL_PLL_SetAdcPll( &mPllCoefInput, IsClvSpeed, OverSpeedX10 );
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_PLL_InitHif

    Description:    Initialise the appropriate PLL registers (related to HIF initialisation).

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: The SATA PLL Registers are set
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_PLL_InitHif( void )
{
    /* SATA PLL Init */
#if (CHIP_REV < 0xC0)
    HAL_PLL_InitSataPll(e_SATCLKIN_EXT, e_SATPLLBIS_200, e_SATPLLCFG_REXTREFCHANGE, e_SATPLLTST_TESTMODE_OFF, e_REFCLKMOD_25MHz, e_SSCCTL_OFF);
#else
    HAL_PLL_InitSataPll(e_SATPLLBIS_200, e_SATPLLTST_TESTMODE_OFF, e_REFCLKMOD_25MHz, e_SSCCTL_OFF);
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_PLL_SetPll

    Description:    Write the PLL calculated coefs to the appropriate registers.
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many PLL Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_PLL_SetPll( void )
{
    bool            IsClvSpeed;
    USHORT          OverSpeedX10;

    GetSpeedAndPllCoef(&mPllCoefs, &mPllCoefInput, &IsClvSpeed, &OverSpeedX10);
    HAL_PLL_SetAdcPll( &mPllCoefInput, IsClvSpeed, OverSpeedX10 );
}

/*-----------------------------------------------------------------------------------------------------
    Name:           GetSpeedAndPllCoefInit

    Description:    Get the overspeed value, its type and the coefficients used to set the PLL registers for Initialisation
    Inputs:         PllCoefs :          structure of coef values
    Outputs:        PllCoefInput :      coef values used to set the PLL registers
                    IsCLVSpeed :        indicates whether the speed is a CLV speed
                    OverspeedX10 :      current overspeed value times 10
    Global Inputs:  mDiscKind:          to determine the set of coefficients/constants to use
                    mSpeed:             current speed
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
static void GetSpeedAndPllCoefInit(const PllTabCoefT *PllCoefs, PllCoefInputT *PllCoefInput, bool *IsClvSpeed, USHORT *OverSpeedX10)
{
    /* Initialise the speed as being CLV */
    *IsClvSpeed = TRUE;

   PllCoefInput->PllCoefCalc    = PllCoefs->Cd.PllCoefCalc;
   PllCoefInput->WifClkDiv      = PllCoefs->Cd.WifClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
   PllCoefInput->WoClkDiv       = PllCoefs->Cd.WoClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
   PllCoefInput->WrtClkDiv      = PllCoefs->Cd.WrtClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
   *OverSpeedX10                = 80; //Take 80, because lower there is a Deneb chip bug, minimum adc-clock of 25Mhz required ==> will be solved in Deneb B0 (only CD 4x suffers from this)
}

/*-----------------------------------------------------------------------------------------------------
    Name:           GetSpeedAndPllCoef

    Description:    Get the overspeed value, its type and the coefficients used to set the PLL registers
    Inputs:         PllCoefs :          structure of coef values
    Outputs:        PllCoefInput :      coef values used to set the PLL registers
                    IsCLVSpeed :        indicates whether the speed is a CLV speed
                    OverspeedX10 :      current overspeed value times 10
    Global Inputs:  mDiscKind:          to determine the set of coefficients/constants to use
                    mSpeed:             current speed
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
static void GetSpeedAndPllCoef(const PllTabCoefT *PllCoefs, PllCoefInputT *PllCoefInput, bool *IsClvSpeed, USHORT *OverSpeedX10)
{
    switch (mDiscKind.fDiscKind)
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
            PllCoefInput->PllCoefCalc   = PllCoefs->Cd.PllCoefCalc;
            PllCoefInput->WifClkDiv     = PllCoefs->Cd.WifClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WoClkDiv      = PllCoefs->Cd.WoClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WrtClkDiv     = PllCoefs->Cd.WrtClkDiv[PllCoefs->Cd.PllCoefCalc.WrtClkDivValue-1];
            *OverSpeedX10 = SpeedIndexTableCD[mSpeed] / 10;
#if (DOUBLED_ADC_CLK)
            //use ADC PLL Settings with 2x coefficient
            if (*OverSpeedX10 < 40)
            {
                *OverSpeedX10 = 40; //Deneb chip bug, minimum adc-clock of 25Mhz required ==> will be solved in Deneb B0 (only CD 4x suffers from this)
            }
            *OverSpeedX10 *= 2;
#endif
            if (mSpeed <= CLV_SPEED_CD)
            {
                *IsClvSpeed = TRUE;
            }
            else
            {
                *IsClvSpeed = FALSE;
            }
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
            PllCoefInput->PllCoefCalc   = PllCoefs->Dvd.PllCoefCalc;
            PllCoefInput->WifClkDiv     = PllCoefs->Dvd.WifClkDiv[PllCoefs->Dvd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WoClkDiv      = PllCoefs->Dvd.WoClkDiv[PllCoefs->Dvd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WrtClkDiv     = PllCoefs->Dvd.WrtClkDiv[PllCoefs->Dvd.PllCoefCalc.WrtClkDivValue-1];
#if (DVD_710Run815New == 1)
            *OverSpeedX10 = GetSpeedIndexTableDVD(mSpeed) / 10;
#else
            *OverSpeedX10 = SpeedIndexTableDVD[mSpeed] / 10;
#endif

#if (DOUBLED_ADC_CLK)
            //use ADC PLL Settings with 2x coefficient
            *OverSpeedX10 *= 2;
#endif
            if (mSpeed <= CLV_SPEED_DVD)
            {
                *IsClvSpeed = TRUE;
            }
            else
            {
                *IsClvSpeed = FALSE;
            }
            break;

#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            PllCoefInput->PllCoefCalc = PllCoefs->DvdRam.PllCoefCalc;
            PllCoefInput->WifClkDiv   = PllCoefs->DvdRam.WifClkDiv[PllCoefs->DvdRam.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WoClkDiv    = PllCoefs->DvdRam.WoClkDiv[PllCoefs->DvdRam.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WrtClkDiv   = PllCoefs->DvdRam.WrtClkDiv[PllCoefs->DvdRam.PllCoefCalc.WrtClkDivValue-1];
            *OverSpeedX10 = SpeedIndexTableDVDRAM[mSpeed] / 10;
            if (mSpeed <= CLV_SPEED_DVDRAM)
            {
                *IsClvSpeed = TRUE;
            }
            else
            {
                *IsClvSpeed = FALSE;
            }
            break;
#endif

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            PllCoefInput->PllCoefCalc   = PllCoefs->Bd.PllCoefCalc;
            PllCoefInput->WifClkDiv     = PllCoefs->Bd.WifClkDiv[PllCoefs->Bd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WoClkDiv      = PllCoefs->Bd.WoClkDiv[PllCoefs->Bd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WrtClkDiv     = PllCoefs->Bd.WrtClkDiv[PllCoefs->Bd.PllCoefCalc.WrtClkDivValue-1];
            *OverSpeedX10 = SpeedIndexTableBD[mSpeed] / 10;
    #if (DOUBLED_ADC_CLK)
            //use ADC PLL Settings with 2x coefficient
            *OverSpeedX10 *= 2;
            if (READ_FIELD(HAL_DFE_BDRMKDE))
            {   //BDROM VID, KCD read
                *OverSpeedX10 *= 2;
            }
            else if (svo_BDR_RE_PICReading)
            {   //BDRE PIC read
                *OverSpeedX10 *= 4;
            }

            if (mSpeed <= CLV_SPEED_BD) /* the CLV speeds are defined after the CAV speeds in the case of BD  */
            {
                *IsClvSpeed = TRUE;
            }
            else
            {
                *IsClvSpeed = FALSE;
            }
    #else
            if (mSpeed <= CLV_SPEED_BD) /* the CLV speeds are defined after the CAV speeds in the case of BD  */
            {
                *IsClvSpeed = TRUE;
        #if (PCB == 710)||(PCB==711)||(PCB==817)
                WRITE_FIELD(HAL_DFE_BITDETCFG, 2);
                BitDetCFG_index = 2;
        #endif
            }
            else
            {
                *IsClvSpeed = FALSE;
            }

            if (svo_BDR_RE_PICReading)
            {   //BDRE PIC read
                *OverSpeedX10 *= 2;
        #if (PCB == 710)||(PCB==711)||(PCB==817)
                WRITE_FIELD(HAL_DFE_BITDETCFG, 2);
                BitDetCFG_index = 2;
        #endif
            }
    #endif
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
    #endif
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDROMDLDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDLDisc:
    #endif
        case eHDRDLDisc:
        case eHDRWDLDisc:
            PllCoefInput->PllCoefCalc   = PllCoefs->HdDvd.PllCoefCalc;
            PllCoefInput->WifClkDiv     = PllCoefs->HdDvd.WifClkDiv[PllCoefs->HdDvd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WoClkDiv      = PllCoefs->HdDvd.WoClkDiv[PllCoefs->HdDvd.PllCoefCalc.WrtClkDivValue-1];
            PllCoefInput->WrtClkDiv     = PllCoefs->HdDvd.WrtClkDiv[PllCoefs->HdDvd.PllCoefCalc.WrtClkDivValue-1];
            *OverSpeedX10 = SpeedIndexTableHDDVD[mSpeed] / 10;
    #if (DOUBLED_ADC_CLK)
            /* 6xADCPLL */
            *OverSpeedX10 *= 6;
    #endif
            if (mSpeed <= CLV_SPEED_HD) /* the CLV speeds are defined after the CAV speeds in the case of HD  */
            {
                *IsClvSpeed = TRUE;
            }
            else
            {
                *IsClvSpeed = FALSE;
            }
            break;
#endif /* (ENABLE_HDVD == 1) */

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
            break;
    } /* ENDSWITCH: DiscKind */
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
