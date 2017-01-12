/* ******************************************************************************************
*
*	(c) Copyright 2002 - 2006 Sunext Design, INC.
*		          All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
* 	HAL_bd_codec.c
*
*  Description:
*	This file contains functions for the DFE subsystem and includes
* 	the following:
*		HAL_BD_CODEC_Init
*
*
* Notes:
*
*
*   $Revision: 29 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\common\sdram.h"
#include ".\player\plrdb.h"
#include ".\servo\svo.h"
#include ".\AL\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_bd_codec.h"
#include ".\AL\HAL_bd_codec.h"

#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif

/*** Local defines ***/

/*** Local static variables */

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_Init

	Description:    Initalization of the BD Codec block
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_Init(void)
{
    WRITE_REG(HAL_BDCREG_MAP->BDCMODEN, 0x00);                  /* Turn off all modules */

    WRITE_REG(HAL_BDCREG_MAP->BDCMODEN, BDCMODEN_INIT);         /* Turn on decoder modules */

    WRITE_REG(HAL_BDCREG_MAP->BDDINT1S, 0x00);                  /* Clear all decoder interrupts */
    WRITE_REG(HAL_BDCREG_MAP->BDDINT2S, 0x00);

    WRITE_REG(HAL_BDCREG_MAP->BDDINT1E, 0x00);                  /* Disable all decoder interrupts */
    WRITE_REG(HAL_BDCREG_MAP->BDDINT2E, 0x00);

    WRITE_REG(HAL_BDCREG_MAP->BDCCFG, 0x00);

    WRITE_REG(HAL_BDCREG_MAP->SDCFG, SDCFG_INIT);
    WRITE_REG(HAL_BDCREG_MAP->FDCFG, FDCFG_INIT);
    
    WRITE_FIELD(HAL_BDC_DISCLCONT, 1);
    WRITE_FIELD(HAL_BDC_MXCONTERJ, 35);

    //BD ERROR CORRECTOR CONFIGURATION 1
    WRITE_FIELD(HAL_BDC_ERASTRSEL, halBDC_ErasureStrategyHalfWings);
    WRITE_FIELD(HAL_BDC_ESFREMSIS, halBDC_ESF_One_One);
    WRITE_FIELD(HAL_BDC_ESFREMDIS, halBDC_ESF_Off);
    WRITE_FIELD(HAL_BDC_ESFFILLSG, halBDC_ESF_One_One);
    WRITE_FIELD(HAL_BDC_ESFFILLDG, halBDC_ESF_Three_Three);
    WRITE_FIELD(HAL_BDC_MXERRBIS, 33);
    WRITE_FIELD(HAL_BDC_MXERRLDC, 33);
    WRITE_FIELD(HAL_BDC_MXERALDC, 33);

    //BD ERROR CORRECTOR CONFIGURATION 2
    WRITE_REG(HAL_BDCREG_MAP->BDERCCFG2, 0x00);

    //BD AUN FLYWHEEL CONFIGURATION 1
    //WRITE_REG(HAL_BDCREG_MAP->AUNFLCFG1, 0x00);
    WRITE_FIELD(HAL_BDC_AUNFLTOLK, DEFAULT_BD_AUN_FLYWHEEL_OUT_OF_LOCK_THRESHOLD);
    WRITE_FIELD(HAL_BDC_AUNFLTILK, DEFAULT_BD_AUN_FLYWHEEL_IN_LOCK_THRESHOLD);
    WRITE_FIELD(HAL_BDC_AUNFLCFI4, 1);
    WRITE_FIELD(HAL_BDC_AUNFLCFI3, 2);
    WRITE_FIELD(HAL_BDC_AUNFLCFI2, 3);
    WRITE_FIELD(HAL_BDC_AUNFLCFI1, 4);
    WRITE_FIELD(HAL_BDC_AUNFLCFI0, 5);
    WRITE_FIELD(HAL_BDC_FRCAUNOLK, 0);
    WRITE_FIELD(HAL_BDC_FRCAUNILK, 0);
    WRITE_FIELD(HAL_BDC_USEERAAUN, 0);

    //BD AUN FLYWHEEL CONFIGURATION 2
    WRITE_FIELD(HAL_BDC_AUNFLCFD7, 0);
    WRITE_FIELD(HAL_BDC_AUNFLCFD6, 1);
    WRITE_FIELD(HAL_BDC_AUNFLCFD5, 2);
    WRITE_FIELD(HAL_BDC_AUNFLCFD4, 3);
    WRITE_FIELD(HAL_BDC_AUNFLCFD3, 4);
    WRITE_FIELD(HAL_BDC_AUNFLCFD2, 5);
    WRITE_FIELD(HAL_BDC_AUNFLCFD1, 7);
    WRITE_FIELD(HAL_BDC_AUNFLCFD0, 8);

    //BD AUN FLYWHEEL CONFIGURATION 3
    WRITE_REG(HAL_BDCREG_MAP->AUNFLCFG3, 0x00);
    WRITE_FIELD(HAL_BDC_IGNFRMNUM, 1);
    WRITE_FIELD(HAL_BDC_GENFLBYT, 1);
    WRITE_FIELD(HAL_BDC_FRC1FRM20, 1);
    WRITE_FIELD(HAL_BDC_AUNFLMXER, DEFAULT_BD_AUN_FLYWHEEL_MAX_NUM_ERRORS);
    WRITE_FIELD(HAL_BDC_AUNFLMXJF, DEFAULT_BD_AUN_FLYWHEEL_MAX_JUMP_FORWARD);

    //BD AUN FLYWHEEL
    WRITE_REG(HAL_BDCREG_MAP->BDAUNFLY, 0x0000001E); //preset the flywheel

    /* Configure cluster start and stop address for Full Deinterleaver stream */
    WRITE_REG(HAL_BDCREG_MAP->FDSSTRAD, 0x00);
    WRITE_REG(HAL_BDCREG_MAP->FDSSTPAD, 0x00);

    /* Configure Full Deinterleaver filter address and mask */
    WRITE_REG(HAL_BDCREG_MAP->FDSSECMA, 0x00);
    WRITE_REG(HAL_BDCREG_MAP->FDSSECM, 0xFFFFFFFF);

    WRITE_REG(HAL_BDCREG_MAP->BDERSTCFG, HAL_BDC_STATENCAP);
    WRITE_FIELD(HAL_BDC_NERENDBIS, DEFAULT_BD_ERRATA_NUM_FOR_ENDANGERED_BIS);
    WRITE_FIELD(HAL_BDC_NERENDLDC, DEFAULT_BD_ERRATA_NUM_FOR_ENDANGERED_LDC);
    WRITE_FIELD(HAL_BDC_STATUPRAT, halBDC_StatisticsUpdateRateOneClusters);

    WRITE_REG(HAL_BDCREG_MAP->BDESENDI, 0x00);              /* TBD */

    /* Configure the thresholds that trigger the normal/alter interrupts for the error counters */
    //WRITE_REG(HAL_BDCREG_MAP->BDERRTHR, 0x00);
    WRITE_FIELD(HAL_BDC_THREDCFAI, DEFAULT_BD_ERROR_LOWER_EDC_FAIL_THRESHOLD);
    WRITE_FIELD(HAL_BDC_THRLDCUNC, DEFAULT_BD_ERROR_LOWER_EDC_UNCORRECTABLE_THRESHOLD);
    WRITE_FIELD(HAL_BDC_THRLDCSER, DEFAULT_BD_ERROR_LOWER_SER_THRESHOLD);

    //WRITE_REG(HAL_BDCREG_MAP->BDERRATHR, 0x00);
    WRITE_FIELD(HAL_BDC_ATHEDCFAI, DEFAULT_BD_ERROR_ALERT_LOWER_EDC_FAIL_THRESHOLD);
    WRITE_FIELD(HAL_BDC_ATHLDCUNC, DEFAULT_BD_ERROR_ALERT_LOWER_EDC_UNCORRECTABLE_THRESHOLD);
    WRITE_FIELD(HAL_BDC_ATHLDCSER, DEFAULT_BD_ERROR_ALERT_LOWER_SER_THRESHOLD);

    /* Configure SD to FD buffers */
    WRITE_REG(HAL_BDCREG_MAP->SDFDLDCST, BDROM_SDFD_LDC_START);
    WRITE_REG(HAL_BDCREG_MAP->SDFDBISST, BDROM_SDFD_BIS_START);
    //WRITE_REG(HAL_BDCREG_MAP->SDFDCLSET, HAL_BDC_SDFDFDPRE|HAL_BDC_SDFDSDPRE|(SDFD_CLUSTERS & HAL_BDC_SDFDCLRNG_MASK));
    WRITE_REG(HAL_BDCREG_MAP->SDFDCLSET, 0);                                    /* Set SDFDSdramFDPointer & SDFDSdramSDPointer to 0 */
    WRITE_FIELD(HAL_BDC_SDFDCLRNG, SDFD_CLUSTERS);                              /* Specify the size of SDFD buffer */
    WRITE_FIELD(HAL_BDC_SDFDFDPRE, 1);
    WRITE_FIELD(HAL_BDC_SDFDSDPRE, 1);

    /* Configure FD to Host buffers */
    WRITE_REG(HAL_BDCREG_MAP->FDHSTLBS, BD_FDHOST_LDC_START_ADD|(BDROM_FDHOST_BIS_START_ADD << HAL_BDC_FDHSTBISS_LSB));
    WRITE_REG(HAL_BDCREG_MAP->FDHSTCLST, HAL_BDC_FDHSTFDP|(MAX_BDROM_CLUSTERS & HAL_BDC_FDHSTCLRN_MASK));
    //Fixed Read error after Atapi Device Reset for BDR/RE.
    switch (mDiscKind.fDiscKind)
    {
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
        HAL_BD_CODEC_InitializeBDSdramBuffer();     /* Specify the ranges of SDFD and FDHost buffers */
        WRITE_FIELD(HAL_BDC_ADNOTAUN, 1);
        HAL_BD_CODEC_Enable_DiscardIce();
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_InitializeBDSdramBuffer

	Description:    Initialize BD Sdram Buffer
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_InitializeBDSdramBuffer(void)
{
    if ((mDiscKind.fDiscKind == eBDROMDisc) || (mDiscKind.fDiscKind == eBDROMDLDisc))
    {
        /* Configure FD to Host buffers */
        WRITE_FIELD(HAL_BDC_FDHSTLDCS, BD_FDHOST_LDC_START_ADD);
        WRITE_FIELD(HAL_BDC_FDHSTBISS, BDROM_FDHOST_BIS_START_ADD);
        WRITE_REG(HAL_BDCREG_MAP->FDHSTCLST, HAL_BDC_FDHSTFDP);
        WRITE_FIELD(HAL_BDC_FDHSTCLRN, MAX_BDROM_CLUSTERS);

        /* Configure SD to FD buffers */
        WRITE_REG(HAL_BDCREG_MAP->SDFDLDCST, BDROM_SDFD_LDC_START);
        WRITE_REG(HAL_BDCREG_MAP->SDFDBISST, BDROM_SDFD_BIS_START);
    }
    else
    {
        /* Configure FD to Host buffers */
        WRITE_FIELD(HAL_BDC_FDHSTLDCS, BD_FDHOST_LDC_START_ADD);
        WRITE_FIELD(HAL_BDC_FDHSTBISS, BDWRITABLE_FDHOST_BIS_START_ADD);
        WRITE_REG(HAL_BDCREG_MAP->FDHSTCLST, HAL_BDC_FDHSTFDP);
        WRITE_FIELD(HAL_BDC_FDHSTCLRN, MAX_BDWRITABLE_CLUSTERS);

        /* Configure SD to FD buffers */
        WRITE_REG(HAL_BDCREG_MAP->SDFDLDCST, BDWRITABLE_SDFD_LDC_START);
        WRITE_REG(HAL_BDCREG_MAP->SDFDBISST, BDWRITABLE_SDFD_BIS_START);
    }

    //WRITE_REG(HAL_BDCREG_MAP->SDFDCLSET, HAL_BDC_SDFDFDPRE|HAL_BDC_SDFDSDPRE);  /* Set SDFDSdramFDPointer & SDFDSdramSDPointer to 0 */
    WRITE_REG(HAL_BDCREG_MAP->SDFDCLSET, 0);                                    /* Set SDFDSdramFDPointer & SDFDSdramSDPointer to 0 */
    WRITE_FIELD(HAL_BDC_SDFDCLRNG, SDFD_CLUSTERS);                              /* Specify the size of SDFD buffer */
    WRITE_FIELD(HAL_BDC_SDFDFDPRE, 1);
    WRITE_FIELD(HAL_BDC_SDFDSDPRE, 1);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_Enable_Buffering

	Description:    Put Small&Full deinterleaver into play state
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_Enable_Buffering(void)
{
    // Put small deinterleaver into play state
    WRITE_FIELD(HAL_BDC_HLDSDE, CLEAR);
    WRITE_FIELD(HAL_BDC_HLDOFFLCK, SET);

    // Put full deinterleaver into play state
    WRITE_FIELD(HAL_BDC_SHLDFD, CLEAR);
    WRITE_FIELD(HAL_BDC_RSHLDFDE, SET);
    WRITE_FIELD(HAL_BDC_STRMSTRE, SET);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_Disable_Buffering

	Description:    Put Small&Full deinterleaver into hold state
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_Disable_Buffering(void)
{
    // Put small deinterleaver into hold state
    WRITE_FIELD(HAL_BDC_HLDSDE, SET);

    // Put full deinterleaver into hold state
    WRITE_FIELD(HAL_BDC_SHLDFD, SET);
    WRITE_FIELD(HAL_BDC_STRMSTRE, CLEAR);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_SetDiscPtr

	Description:    Set FDtoHost Sdram FD pointer
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_SetDiscPtr(USHORT x)
{
    ULONG RegVal;

#if EN_POWER_CONTROL
    if (BDDecCLKStop)
        return;
#endif

    RegVal = READ_REG(HAL_BDCREG_MAP->FDHSTCLST);

    RegVal |= HAL_BDC_FDHSTFDP_MASK;
    RegVal &= (~HAL_BDC_FDHSTSFDP_MASK);
    x = (x >> 5);
    x = (x << 8);
    RegVal |= (ULONG)x;

    WRITE_FIELD(HAL_BDC_SHLDFD, SET);
    WRITE_REG(HAL_BDCREG_MAP->FDHSTCLST, RegVal);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_DISABLE_DECODER_INTS

	Description:    Disable bd decoder interrupts
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_DISABLE_DECODER_INTS(void)
{
#if EN_POWER_CONTROL
    if (BDDecCLKStop)
        return;
#endif
    WRITE_REG(HAL_BDCREG_MAP->BDDINT1E,0x00);
    WRITE_REG(HAL_BDCREG_MAP->BDDINT2E,0x00);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_CLEAR_DECODER_STATUS

	Description:    Clear bd decoder interrupt status
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_CLEAR_DECODER_STATUS(void)
{
#if EN_POWER_CONTROL
    if (BDDecCLKStop)
        return;
#endif
    WRITE_REG(HAL_BDCREG_MAP->BDDINT1S,0x00);
    WRITE_REG(HAL_BDCREG_MAP->BDDINT2S,0x00);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_GET_FD_HOLD_STATUS

	Description:    Get the hold status of the full de-interleaver
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        1 or 0
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_BD_CODEC_GET_FD_HOLD_STATUS(void)
{
#if EN_POWER_CONTROL
    if (BDDecCLKStop)
        return 1;
#endif
    return ((vBOOL)READ_FIELD(HAL_BDC_HLDFDS));
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_GET_RING_BUFFER_START

	Description:    Return the start address of the ring buffer
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        Addr
-----------------------------------------------------------------------------------------------------*/
ULONG HAL_BD_CODEC_GET_RING_BUFFER_START(void)
{
    ULONG Addr;

#if EN_POWER_CONTROL
    if (BDDecCLKStop)
        return 0L;
#endif
    Addr = (ULONG)(READ_FIELD(HAL_BDC_FDHSTLDCS) << 16);
    return Addr;
}

#if (ENABLE_DISCARD_ICE == 1)
/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_Enable_DiscardIce

	Description:    Enable the function that discard iced cluster.
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_Enable_DiscardIce(void)
{
    WRITE_FIELD(HAL_BDC_DISCLFULL, 1);
    WRITE_FIELD(HAL_BDC_DISCLPART, 1);
}

/*-----------------------------------------------------------------------------------------------------
	Name:           HAL_BD_CODEC_Disable_DiscardIce

	Description:    Disable the function that discard iced cluster.
	Inputs:         None
	Outputs:        None
	Global Inputs:  None
	Global Outputs: None
	Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_BD_CODEC_Disable_DiscardIce(void)
{
    WRITE_FIELD(HAL_BDC_DISCLFULL, 0);
    WRITE_FIELD(HAL_BDC_DISCLPART, 0);
}
#endif  /* ENABLE_DISCARD_ICE == 1 */
