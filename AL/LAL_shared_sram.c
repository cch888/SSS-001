/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: LAL_shared_sram.c
*
* DESCRIPTION
* This file contains the LAL shared-sram functions implemented in the LAL shared-sram layer
* The memory is shared by several hw-blocks, this module configures which blocks will get 
* access to it.
* The internal 7kB shared SRAM of the SC6230/SC6530 that is used by the CD-Decoder, CD-Encoder, DVD/HDDVDDecoder,
* DVD/HDDVD Encoder, BD-Codec and Auto OPC modules is accessible by the memory map address range
* 
* NOTES:
*
*   $Revision: 13 $
*   $Date: 11/03/11 2:32p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/
#include ".\oem\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_cd_encoder.h"
#include ".\AL\REG_dvd_encoder.h"
#include ".\AL\REG_dvd_decoder.h"
#include ".\AL\LAL_shared_sram.h"
#include ".\al\coefs.h"
#include ".\oem\coef_values.h"

#if (BD_ENABLE)
#include ".\AL\HAL_bd_codec.h"
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Coefficients
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

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
    Name:           LAL_STREAM_Init

    Description:    Initialize the LAL Shared SRAM module

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_SHARED_SRAM_Init(void)
{
// TBD? No Code!
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SHARED_SRAM_Enable

    Description:    Enables write access to shared sram for a given configuration

    Inputs:         eConfig is the module which gets access to shared sram
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_SHARED_SRAM_Enable(ELAL_SHARED_SRAM_Config eConfig)
{
    BYTE i;
    switch ( eConfig )
    {
        case LAL_SHARED_SRAM_CONFIG_STREAM:
           WRITE_FIELD(HAL_GLOBAL_DECCKSTP,1); // Stop CD Decoder clock
           WRITE_FIELD(HAL_CDE_ENENC,0);     //ENENC - Encode Enable , disable

           /* Here to disable Data Buffering and ECC Engine ==> SDRAM and SRAM not be used
                But Sync Detection still work ==> It can get PSN from Decoder                              */
           WRITE_FIELD(HAL_DVDD_ENDVDD   ,1);   //Enable DVD/HD DVD-Decoder block
           WRITE_FIELD(HAL_DVDD_ENECCE   ,0);   //Stop ECC correction engine
           WRITE_FIELD(HAL_DVDD_ENBUF    ,0);   //Disable data from being buffered into memoryX
           WRITE_FIELD(HAL_CDE_ENCSRAMDIS,1);   //Disable SRAM writes from encoder to allow OPC to function correctly

           //configure it to random-data otherwise ecc generation will use sram
           WRITE_FIELD(HAL_DVDE_WMOD, halDVDE_OPCRandomData);

           //enable SRAM write
           WRITE_FIELD(HAL_GLOBAL_SRAMDIS,1);   //Disable SRAM writes
           for (i=0;i<5;i++);
           WRITE_FIELD(HAL_GLOBAL_SRAMDIS,0); // 0 = enable, 1 is disable write to sram
           //ONLY FOR BD
           //WRITE_FIELD(HAL_DVDE_ENDVDE,0);
           //WRITE_FIELD(HAL_DVDD_ENDVDD,0);

           //WRITE_REG(HAL_BDCREG_MAP->BDCMODEN,
           //                               /*    HAL_BDC_ERASTRGON|HAL_BDC_ERRSTATON|\ */
           //                               /*    HAL_BDC_ERCOSHON|HAL_BDC_ARBITERON|\ */
           //                               /*    HAL_BDC_BDFDON|HAL_BDC_BDSDON|\ */ //interleavers are using sram --> so disable them
           //                                   HAL_BDC_BDINON|HAL_BDC_AUNDECON);
           if (mDiscKind.Bit.DiscStd == eDiscBD)
           {
#if ((CHIP_REV < 0xC0)&&(CHIP_REV >= 0xB2))//20101112_02VE ==start==BDSRAMSIS is moved to global
               WRITE_FIELD(HAL_OPC_BDSRAMDIS,1);
#endif
#if (CHIP_REV >= 0xC0)
               WRITE_FIELD(HAL_GLOBAL_BDSRAMDIS,1);
#endif
           }
           break;

        case LAL_SHARED_SRAM_CONFIG_CLEAR:
           if (mDiscKind.Bit.DiscStd == eDiscBD)
           {
#if ((CHIP_REV < 0xC0)&&(CHIP_REV >= 0xB2))//20101112_02VE ==start==BDSRAMSIS is moved to global
               WRITE_FIELD(HAL_OPC_BDSRAMDIS,0);
#endif
#if (CHIP_REV >= 0xC0)
               WRITE_FIELD(HAL_GLOBAL_BDSRAMDIS,0);
#endif
           }
           break;

        default:
           DEBUG_ASSERT(0, eASSERT_FUNCTION_NOT_IMPLEMENTED);
           break;
    }
}


/*****************************************************************************
*                       End Of File
*****************************************************************************/



