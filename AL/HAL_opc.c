/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: HAL_opc.c
*
* DESCRIPTION
* This file contains the HAL OPC funtions implemented in the HAL OPC layer
*
* NOTES:
*
*   $Revision: 22 $
*   $Date: 11/03/11 5:53p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\oem\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_opc.h"
#include ".\AL\HAL_pll.h" //TO BE REMOVED, temp workaround
#include ".\AL\REG_dfe.h"
#include ".\AL\HAL_dfe.h" //only for printf required: to print some dfe regs

#if  STREAM_STM
/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
/* field settings for OPC mode */
#define WSTRT       0
#define RSTRT       1
#define WBLSTM      2
#define NR_OF_MODES 3

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

static const BOOL mModeTable[HAL_OPC_MODE_LAST][NR_OF_MODES] =
{  //W  R  STM
    {1, 0, 0}, /* HAL_OPC_MODE_WRITE */
    {0, 1, 0}, /* HAL_OPC_MODE_READ */
    {0, 0, 1}  /* HAL_OPC_MODE_STM */
};
static BYTE mOpcIntStatus;
/*const*/ volatile HAL_OPC_SampleT * HAL_OPC_pSRAM = (volatile HAL_OPC_SampleT *)ENC_DEC_OPC_SRAM;

/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_Init

    Description:    Initialize the HAL OPC module

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_Init(void)
{
    mOpcIntStatus = 0;
    WRITE_FIELD(HAL_OPC_WSTRT,  0);
    WRITE_FIELD(HAL_OPC_RSTRT,  0);
    WRITE_FIELD(HAL_OPC_WBLSTM, 0);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_EnableOPCMode

    Description:    Enable the OPC mode.

    Inputs:         eMode described the OPC mode = {Write, Read, WBLSTM}.
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_EnableOPCMode(HAL_OPC_ModeT eMode)
{
    /* disable circular buffer */
    WRITE_FIELD(HAL_OPC_OPCCIRC, (eMode == HAL_OPC_MODE_STM));
    /* start selected mode */
    WRITE_FIELD(HAL_OPC_WSTRT,  mModeTable[eMode][WSTRT]);
    WRITE_FIELD(HAL_OPC_RSTRT,  mModeTable[eMode][RSTRT]);
    WRITE_FIELD(HAL_OPC_WBLSTM, mModeTable[eMode][WBLSTM]);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_ConfigureWblSampleFrequency

    Description:    Configures the wobble sample frequency

    Inputs:         Many ...
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_ConfigureWblSampleFrequency(USHORT OverspeedX100, float SectorsPerStep, halOPC_AOPCSampNum_t eSamplesPerStep, BYTE StartDelay, BYTE StopDelay, BYTE OpcPrescaler)
{
    USHORT WobblesPerSector;
    BYTE TrCnt;
    BYTE TcCnt;
    BYTE TdCnt;
    ULONG WobbleFrequencyN1;
    USHORT SampleFrequency;

    //check if dont take too many samples
    //DEBUG_ASSERT ( OPCSIZE > TRUNC(eSamplesPerStep*SectorsPerStep*8/32)+1)  --> wat is die 8 en 32 ??? wat met nrofsamples hier ?
    switch (mDiscKind.fDiscKind)
    {
      case eDVDRAMDisc:
        WobblesPerSector = DVDRAM_WOBBLES_SECTOR;
        WobbleFrequencyN1 = DVDRAM_WBL_FREQ_N1;
        break;

      case eCDROMDisc:
      case eCDRDisc:
      case eCDRWDisc:
        WobblesPerSector = CD_WOBBLES_SECTOR;
        WobbleFrequencyN1 = CD_WBL_FREQ_N1;
        break;

      case eDVDROMDisc:
      case eDVDPlusRDisc:
      case eDVDPlusRWDisc:
      case eDVDROMDLDisc:
      case eDVDPlusRDLDisc:
      case eDVDPlusRWDLDisc:
        WobblesPerSector = DVDp_WOBBLES_SECTOR;
        WobbleFrequencyN1 = DVD_PR_PRW_WBL_FREQ_N1;
        break;

      case eDVDMinusRDLDisc:
      case eDVDMinusRWDLDisc:
      case eDVDMinusRDisc:
      case eDVDMinusRWDisc:
        WobblesPerSector = DVDm_WOBBLES_SECTOR;
        WobbleFrequencyN1 = DVD_MR_MRW_WBL_FREQ_N1;
        break;

      case eBDROMDisc:
      case eBDRDisc:
      case eBDREDisc:
      case eBDROMDLDisc:
      case eBDRDLDisc:
      case eBDREDLDisc:
        WobblesPerSector = BD_WOBBLES_SECTOR;
        WobbleFrequencyN1 = BD_WBL_FREQ_N1;
        break;

#if (ENABLE_HDDVD == 1)
      case eHDRAMDisc:
        WobblesPerSector = HDRAM_WOBBLES_SECTOR;
        WobbleFrequencyN1 = HDDVD_WBL_FREQ_N1;
        break;
      case eHDROMDisc:
    #if (ENABLE_CBHD == 1)
      case eCBHDROMDisc:
    #endif
      case eHDRDisc:
      case eHDRWDisc:
      case eHDROMDLDisc:
    #if (ENABLE_CBHD == 1)
      case eCBHDROMDLDisc:
    #endif
      case eHDRDLDisc:
      case eHDRWDLDisc:
        WobblesPerSector = HD_WOBBLES_SECTOR;
        WobbleFrequencyN1 = HDDVD_WBL_FREQ_N1;
        break;
#endif /* (ENABLE_HDVD == 1) */

      case eLSDisc:
      default:
        DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
        break;
    }
    TrCnt = (BYTE) ( (USHORT) WobblesPerSector * SectorsPerStep / ( 1 << OpcPrescaler ) );
    TcCnt = (BYTE) ( (USHORT) TrCnt * StartDelay / 100 );
    TdCnt = (BYTE) ( (USHORT) TrCnt * StopDelay / 100 );
    SampleFrequency = (USHORT) ( (ULONG) ( TdCnt - TcCnt - 2 ) * ( 1 << OpcPrescaler ) * HAL_PLL_GetOpcClkKHz() / ( 1 << eSamplesPerStep ) / OverspeedX100 * 1000 * 100 / WobbleFrequencyN1 );

    WRITE_FIELD(HAL_OPC_OPCTRCNT, TrCnt );
    WRITE_FIELD(HAL_OPC_OPCTCCNT, TcCnt );
    WRITE_FIELD(HAL_OPC_OPCTDCNT, TdCnt );
    WRITE_FIELD(HAL_OPC_SAMPFREQ, SampleFrequency );

    SendMsgCnB(DEBUG_STREAM_MSG,5,0x3FAA1A,B1B(READ_FIELD(HAL_OPC_OPCTRCNT)),B1B(READ_FIELD(HAL_OPC_OPCTCCNT)),
        B1B(READ_FIELD(HAL_OPC_OPCTDCNT)),B2B(READ_FIELD(HAL_OPC_SAMPFREQ)));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_ConfigureGatedSampling

    Description:    Configure the gated sampling mode

    Inputs:         NrOfSamples describes the (pre-defined) number of samples
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_ConfigureGatedSampling(halOPC_AOPCSampNum_t NrOfSamples)
{
    NrOfSamples += READ_FIELD(HAL_OPC_OPCAVG);
    if (NrOfSamples <= halOPC_AOPCSampNum_1024)
    {
        /* select gated sampling mode */
        WRITE_FIELD(HAL_OPC_OPCGATE, halOPC_OPCGate_SAMPNUM);
        /* set number of samples */
        WRITE_FIELD(HAL_OPC_SAMPNUM, NrOfSamples);
    }
    else
    {
        /* select non-gated sampling mode */
        WRITE_FIELD(HAL_OPC_OPCGATE, halOPC_OPCGate_NoLimit);
        /* set number of samples */
        WRITE_FIELD(HAL_OPC_SAMPNUM, halOPC_AOPCSampNum_1024);
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_ConfigureAveraging

    Description:    Configure Averaging

    Inputs:         Nr of samples to average
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_ConfigureAveraging(halOPC_AOPCAvgSamp_t eAvgSamples)
{
    DEBUG_ASSERT( (eAvgSamples <= halOPC_AOPCAvg_128Samp) ? TRUE : FALSE , eASSERT_STREAM_TOO_MANY_SAMPLES);
    WRITE_FIELD(HAL_OPC_OPCAVG, eAvgSamples);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_EnableInterrupt

    Description:    Enable/disable an OPC interrupt

    Inputs:         Interrupt describes the OPC interrupt = {OPCSTP, OPCTD, OPCTC, OPCTR, SMPOVL, OPCRATE}
                    Value = TRUE will enable the interrupt, otherwise the interrupt is disabled
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_EnableInterrupt(HAL_OPC_InterruptT Interrupt, BOOL Value)
{
    switch(Interrupt)
    {
    case HAL_OPC_INTERRUPT_STP:
        CLEAR_INT_FIELD(HAL_OPC_OPCSTP, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_OPCSTPEN, Value);
        break;
    case HAL_OPC_INTERRUPT_TD:
        CLEAR_INT_FIELD(HAL_OPC_OPCTD, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_OPCTDEN, Value);
        break;
    case HAL_OPC_INTERRUPT_TC:
        CLEAR_INT_FIELD(HAL_OPC_OPCTC, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_OPCTCEN, Value);
        break;
    case HAL_OPC_INTERRUPT_TR:
        CLEAR_INT_FIELD(HAL_OPC_OPCTR, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_OPCTREN, Value);
        break;
    case HAL_OPC_INTERRUPT_OVL:
        CLEAR_INT_FIELD(HAL_OPC_SMPOVL, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_SMPOVLEN, Value);
        break;
    case HAL_OPC_INTERRUPT_RATE:
        CLEAR_INT_FIELD(HAL_OPC_OPCRATE, 0); /* clear interrupt status */
        WRITE_FIELD(HAL_OPC_OPCRATEN, Value);
        break;
    default:
        break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_Isr

    Description:    OPC interrupt service routine.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_OPC_Isr(void)
{
    BYTE OPCINT;
    OPCINT = READ_REG(HAL_OPCREG_MAP->OPCINT)&READ_REG(HAL_OPCREG_MAP->OPCINTEN);
    if (OPCINT & HAL_OPC_OPCSTP)
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_STP;
        CLEAR_INT_FIELD(HAL_OPC_OPCSTP, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_OPCSTP");
    }
    if (READ_FIELD(HAL_OPC_OPCTD))
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_TD;
        CLEAR_INT_FIELD(HAL_OPC_OPCTD, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_OPCTD");
        //send_msg5SValue(DEBUG_STREAM_MSG,"REG A1",2,READ_FIELD(HAL_DFE_A1MAXPEAK));
        //send_msg5SValue(DEBUG_STREAM_MSG,"REG A2",2,READ_FIELD(HAL_DFE_A2MINPEAK));
        //send_msg5SValue(DEBUG_STREAM_MSG,"REG CALF",2,READ_FIELD(HAL_DFE_CALF));
    }
    if (OPCINT & HAL_OPC_OPCTC)
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_TC;
        CLEAR_INT_FIELD(HAL_OPC_OPCTC, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_OPCTC");
    }
    if (READ_FIELD(HAL_OPC_OPCTR))
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_TR;
        CLEAR_INT_FIELD(HAL_OPC_OPCTR, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_OPCTR");
    }
    if (OPCINT & HAL_OPC_SMPOVL)
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_OVL;
        CLEAR_INT_FIELD(HAL_OPC_SMPOVL, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_SMPOVL");
    }
    if (OPCINT & HAL_OPC_OPCRATE)
    {
        mOpcIntStatus |= HAL_OPC_INTERRUPT_RATE;
        CLEAR_INT_FIELD(HAL_OPC_OPCRATE, 0); /* clear interrupt status */
        //send_msg5S(DEBUG_STREAM_MSG,"HAL_OPC_OPCRATE");
    }
    return((BOOL)((mOpcIntStatus & HAL_OPC_INTERRUPT_STP) == HAL_OPC_INTERRUPT_STP));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_IsInterruptSet

    Description:    Check whether an OPC interrupt has occurred.

    Inputs:         Interrupt describes the OPC interrupt = {OPCSTP, OPCTD, OPCTC, OPCTR, SMPOVL, OPCRATE}
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        TRUE when interrupt is set, otherwise FALSE.
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_OPC_IsInterruptSet(HAL_OPC_InterruptT Interrupt)
{
    /* NOTE: these variables are - and must be - volatile */
    HAL_Reg8 Status;
    HAL_Reg8 OpcIntEn;

    /* begin critical section */
    OpcIntEn = READ_REG(HAL_OPCREG_MAP->OPCINTEN); /* store status of currently enabled interrupts */
    WRITE_REG(HAL_OPCREG_MAP->OPCINTEN, 0); /* disable all OPC interrupts */
    Status = mOpcIntStatus; /* make local copy of interrupt status */
    WRITE_REG(HAL_OPCREG_MAP->OPCINTEN, OpcIntEn); /* restore status of interrupts */
    /* end critical section */

    return ((Status & Interrupt) == Interrupt);
}

#if 0
void HAL_OPC_ClearInterrupt(HAL_OPC_InterruptT Interrupt)
{
    /* NOTE: these variables are - and must be - volatile */
    HAL_Reg8 OpcIntEn;

    /* begin critical section */
    OpcIntEn = READ_REG(HAL_OPCREG_MAP->OPCINTEN); /* store status of currently enabled interrupts */
    WRITE_REG(HAL_OPCREG_MAP->OPCINTEN, 0); /* disable all OPC interrupts */
    mOpcIntStatus &= ~Interrupt; /* reset interrupt status */
    WRITE_REG(HAL_OPCREG_MAP->OPCINTEN, OpcIntEn); /* restore status of interrupts */
    /* end critical section */
}
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_OPC_ConfigMioDebug

    Description:    Configures the MIO's to output the STM/OPC signals

    Inputs:         eMode described the OPC mode = {Write, Read, WBLSTM}.
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_OPC_ConfigMioDebug(HAL_OPC_ModeT eMode)
{
   Set_MIO0_TO_MIO();
   Set_MIO1_TO_MIO();
   Set_MIO2_TO_MIO();
   Set_MIO3_TO_MIO();
   WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_opc_mio0);
   WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_opc_mio1);
   WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_opc_mio2);
   WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_opc_mio3);
   switch (eMode)
   {
       case HAL_OPC_MODE_WRITE:
       case HAL_OPC_MODE_READ:
          WRITE_FIELD(HAL_OPC_OPCMIO,0);
          break;

       case HAL_OPC_MODE_STM:
          WRITE_FIELD(HAL_OPC_OPCMIO,1);
          break;

       default:
          break;
   }
}

#endif  //STREAM_STM

/*****************************************************************************
*                       End Of File
*****************************************************************************/
