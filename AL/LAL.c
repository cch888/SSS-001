/* ******************************************************************************************
*
*  (c) Copyright 2006 - 2007 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*  lal.c
*
*  Description:
*  This file contains functions for the LAL layers and includes
*  the following:
*
*        LAL_StoreParams
*        LAL_InitServo
*        LAL_InitHif
*
* Notes:
*
*
*   $Revision: 164 $
*   $Date: 11/03/11 5:53p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_global.h"
#include ".\al\REG_cd_decoder.h"
#include ".\al\LAL.h"
#include ".\al\coefs.h"
#include ".\DVD\dvMacros.h"
#include ".\OEM\oem_tbl.h"
#include ".\Servo\svo_com.h"
#if (PCB == 710)||(PCB==711)||(PCB==817)
#include ".\Common\ComMath.h"
#include ".\servo\svoVar.h"
#endif
#if (BD_ENABLE == 1)
#include ".\al\REG_bd_codec.h"
#include ".\al\HAL_bd_codec.h"
#include ".\al\HAL_dfe.h"
#include ".\AL\HAL_afe.h"
#endif // (BD_ENABLE == 1)
#include ".\al\HAL_global.h"

#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif
//#define DEBUG_COEF_LOADING

/* Global definitions regarding COEF */
static discKindT mPrevDiscKind[E_CORE_COUNT];
static discKindT mPrevDiscKindServoEq;
static ESpindleSpeed mPrevSpeed[E_CORE_COUNT];
static ESpindleSpeed mPrevSpeedServoEq;

#if (BD_ENABLE == 1)
/* Global definitions regarding PIC for BD-R/RE */
BOOL BDR_RE_PICReading;
#endif //(BD_ENABLE == 1)

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DetectDiscKind

    Description:    Not all DiscKind information is yet known, wbltype for example could be unknown,
                    already take a disckind to be able to start the disc-recognition.
                    For wbltype unknown, minus is taken as default.
    Inputs:         DiscKind is the current disc kind
                    Speed is the current speed
    Outputs:        None
    Global Inputs:  a disckind from which maybe all properties are not yet known
    Global Outputs: None
    Returns:        disckind
-----------------------------------------------------------------------------------------------------*/
discKindT LAL_DetectDiscKind(discKindT DiscKind)
{
    discKindT tempDiscKind = DiscKind;

    // Let's detect all the parameters which are unknown and fill in with a default parameter
    if (DiscKind.Bit.DiscStd == eDiscUnknown)
    {
#if ((PCB == 1600)||(PCB == 710)||(PCB==711)||(PCB == 812)||(PCB == 817)) //only for 1600 DiscKind test
        tempDiscKind.Bit.DiscStd = eDiscBD;
#else
        tempDiscKind.Bit.DiscStd = eDiscCD;
#endif
    }
    if (DiscKind.Bit.Layers == eLayersUnknown)
    {
        tempDiscKind.Bit.Layers = eLayersSingle;
    }
    if (DiscKind.Bit.Media == eMediaUnknown)
    {
        tempDiscKind.Bit.Media = eMediaR; // Due to nature of startup algorithm for DVD
    }
    // Only required for DVD (R/RW)
    if ( DiscKind.Bit.WblType == eWblUnknown )
    {
        if ( (tempDiscKind.Bit.DiscStd == eDiscDVD) && ( (tempDiscKind.Bit.Media == eMediaR) || (tempDiscKind.Bit.Media == eMediaRW) ) )
        {
            tempDiscKind.Bit.WblType = eWblMinus;
        }
    }
    tempDiscKind.fDiscKind = MakeDiscKind(tempDiscKind.Bit.DiscStd,  tempDiscKind.Bit.Layers, tempDiscKind.Bit.WblType, tempDiscKind.Bit.Media);
    // check if tempDiscKind.fDiscKind is valid --> ASSERT if NOT --> is handled by coef-layer
    return tempDiscKind;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_StoreParams

    Description:    Store some (LAL)global parameters. Take care that all lal/hal_core parameters are
                    set too.

    Inputs:         DiscKind is the current disc kind
                    Speed is the current speed
    Outputs:        None
    Global Inputs:  None
    Global Outputs: The PLL (LAL)global parameters for DiscKind and Speed are set once
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_StoreParams(discKindT DiscKind, ESpindleSpeed Speed)
{
    mDiscKind = DiscKind;         /* set the DiscKind and DiscType*/
    mSpeed = Speed;

    SendMsg80(DEBUG_COEF_MSG,0x310180);// LAL_StoreParams
    SendMsg80(DEBUG_COEF_MSG,0x3C,mDiscKind.fDiscKind>>8,mDiscKind.fDiscKind);
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            SendMsg80(DEBUG_COEF_MSG,0x331000+mSpeed);
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                SendMsg80(DEBUG_COEF_MSG,0x331180+mSpeed);
            }
            else
#endif
            {
                SendMsg80(DEBUG_COEF_MSG,0x331070+mSpeed);
            }
            break;

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            SendMsg80(DEBUG_COEF_MSG,0x3310B0+mSpeed);
            break;
#endif // (ENABLE_HDDVD == 1)

#if (BD_ENABLE == 1)
        case eDiscBD:
            SendMsg80(DEBUG_COEF_MSG,0x3310D0+mSpeed);
            break;
#endif // (BD_ENABLE == 1)
    }

//TBD! Create Macro's or inline's for next functions
    COEF_StoreParams(DiscKind, Speed);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_InitPlatform

    Description:    Make sure all hal/lal modules are initialised

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        Indicates whether the initialisation was succesfull: Is TRUE if initialisation was succesfull
-----------------------------------------------------------------------------------------------------*/
BOOL LAL_InitPlatform(void)
{
    BOOL Result;

    /* Initialisation LAL_DSP */
    Result = LAL_DSP_Init();

    return Result;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_InitServo

    Description:    Make sure all hal/lal modules are initialised

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        Indicates whether the initialisation was succesfull: Is TRUE if initialisation was succesfull
-----------------------------------------------------------------------------------------------------*/
void LAL_InitServo(void)
{
    ECores  eCore;
    COEFS_t CoefIndex;

   COEF_Init();

#ifdef DEBUG_COEF_LOADING
    SendMsgCn(DEBUG_COEF_MSG,8,0x310181,A4B(mDiscKind.fDiscKind),A4B(mSpeed)); //LAL_InitServo
#endif

    /* Set the previous and current Disc Kind/speed to unknown */
    mDiscKind.fDiscKind = eUnknownKind;
    mSpeed = _UNKNOWN_SPEED;
    for (eCore = E_CORE_AFE; eCore < E_CORE_COUNT; eCore++)
    {
       mPrevDiscKind[eCore].fDiscKind = eUnknownKind;
       mPrevSpeed[eCore] = _UNKNOWN_SPEED;
    }
    mPrevDiscKindServoEq.fDiscKind = eUnknownKind;
    mPrevSpeedServoEq              = _UNKNOWN_SPEED;

    /* Load Generic INIT coefs */
    for (eCore = E_CORE_AFE; eCore < E_CORE_COUNT; eCore++)
    {
#ifdef DEBUG_COEF_LOADING
       SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA36,A1B(eCore));//GenCoef INIT
#endif

       for(CoefIndex = CoefDependencyProperties[eCore].Init.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].Init.Index.GenericField.Last; CoefIndex++)
       {
           /* Write the coef to a register or field */
           LAL_WriteCoef(CoefIndex);
       }
       for(CoefIndex = CoefDependencyProperties[eCore].Init.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].Init.Index.GenericReg.Last; CoefIndex++)
       {
           /* Write the coef to a register or field */
           LAL_WriteCoef(CoefIndex);
       }
    }

    /* Initialisation LAL_PLL */
    LAL_PLL_InitServo();

    /* Initialisation LAL_AFE */
    LAL_AFE_Init();

    /* Initialisation LAL_DFE */
    LAL_DFE_Init();

    /* Initialisation LAL_WOBBLE */
    LAL_WOBBLE_Init();

    /* Initialisation LAL_LASC */
    LAL_LASC_Init();

#if  STREAM_STM
    /* Initialisation LAL_SHARED_SRAM */
    LAL_SHARED_SRAM_Init();
#endif//STREAM_STM
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_InitHif

    Description:    Make sure all hal/lal modules related to HIF initialisation are initialised

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_InitHif(void)
{
   /* Initialisation LAL_PLL */
   //LAL_PLL_InitHif();

   /* Initialisation LAL_CD_DVD_CODEC */
//#ifdef NOT_USED
    LAL_CD_DVD_CODEC_Init();
//#endif    /* NOT_USED */

   /* Initialisation LAL_BD_CODEC */
//#ifdef NOT_USED
    LAL_BD_CODEC_Init();
//#endif    /* NOT_USED */

   /* Initialisation HAL_DEMOD */
   HAL_DEMOD_Init();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_WriteCoef

    Description:    Write the field or register data

    Inputs:         coef data
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_WriteCoef(COEFS_t CoefIndex)
{
   ULONG Value;
   CoefPropertiesT CoefProperties;

   /* Get the data of the coef */
   Value = COEF_GetValue(CoefIndex);
   COEF_GetProperties(CoefIndex, &CoefProperties);

#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,4,0x3FAA34,A1B(CoefProperties.RegSize),A1B(CoefProperties.LSB),A2B(CoefIndex));
        SendMsgCn(DEBUG_COEF_MSG,12,0x3FAA35,A4B(CoefProperties.Address),A4B(CoefProperties.Mask),A4B(Value));
#endif

   /* Check if the Mask > 0,if so something went wrong */
   DEBUG_ASSERT( (CoefProperties.Mask != 0) , eASSERT_ILLEGAL_COEF_MASK);

   switch (CoefProperties.RegSize)
   {
      case sizeof(BYTE):
#ifdef DEBUG_COEF_LOADING
         //send_msg5SValue(1,"Before", 4, READ_REG(*((BYTE *)CoefProperties.Address)));
#endif
         READ_MOD_WRITE(*((BYTE *)CoefProperties.Address), CoefProperties.Mask, (BYTE)(Value << CoefProperties.LSB));
#ifdef DEBUG_COEF_LOADING
         //send_msg5SValue(1,"After", 4, READ_REG(*((BYTE *)CoefProperties.Address)));
#endif
         break;
      case sizeof(USHORT):
         READ_MOD_WRITE(*((USHORT *)CoefProperties.Address), CoefProperties.Mask, (USHORT)(Value << CoefProperties.LSB));
         break;
      case sizeof(ULONG):
         READ_MOD_WRITE(*((ULONG *)CoefProperties.Address), CoefProperties.Mask, (ULONG)(Value << CoefProperties.LSB));
         break;
      default:
         // incorrect element size
         DEBUG_ASSERT(FALSE, eASSERT_INCORRECT_ELEMENT_SIZE);
         break;
   }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetDiscTypeSelection

    Description: Program the disc-sel register from the disckind information

    Inputs:

    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
// Disable the non-functional interrupts
void DisableNonFunctionalInterrupts(discKindT DiscKind)
{
    switch (DiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            // clear interrupts and enable CD decoder interrupts
            WRITE_REG(HAL_CDDREG_MAP->DINT0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINT1, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINT2, 0);
//Closed because it never disabled when read CD then tray out
//          WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 1);
//          WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 1);
//          WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 1);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);

            // disable DVD & HDDVD decoder interrupts
            DISABLE_DVD_DECODER_INTS();
#if (BD_ENABLE == 1)
            // disable BD decoder interrupts
            HAL_BD_CODEC_DISABLE_DECODER_INTS();
#endif // (BD_ENABLE == 1)
            break;

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
#endif // (ENABLE_HDDVD == 1)
        case eDiscDVD:
            // disable CD decoder interrupts
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);

            // clear interrupts and enable DVD/HDDVD decoder interrupts
            CLEAR_DVD_DECODER_STATUS();
            ENABLE_DVD_DECODER_INTS();
#if (BD_ENABLE == 1)
            // disable BD decoder interrupts
            HAL_BD_CODEC_DISABLE_DECODER_INTS();
#endif // (BD_ENABLE == 1)
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            // disable CD decoder interrupts
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);

            // disable DVD & HDDVD decoder interrupts
            DISABLE_DVD_DECODER_INTS();

            // clear interrupts and enable BD decoder interrupts
            HAL_BD_CODEC_CLEAR_DECODER_STATUS();
            HAL_BD_CODEC_DISABLE_DECODER_INTS();
            HAL_BD_CODEC_ENABLE_DECODER_INTS();

            break;
#endif // (BD_ENABLE == 1)
        default:
            // disable CD decoder interrupts
            WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);
            // disable DVD & HDDVD decoder interrupts
            DISABLE_DVD_DECODER_INTS();
#if (BD_ENABLE == 1)
            // disable BD decoder interrupts
            HAL_BD_CODEC_DISABLE_DECODER_INTS();
#endif // (BD_ENABLE == 1)
            break;
    }
#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
    if ((DiscKind.Bit.Media == eMediaROM) || (DiscKind.Bit.DiscStd != eDiscBD)
    #else
    if ((DiscKind.Bit.Media == eMediaROM)
    #endif
    #if (DVD_RAM_READ == 1)
        || (DVDRAM)
    #endif
    )
    {
        HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
    }
    else
    {
        HAL_WOBBLE_EnableNewWblAddInterrupt(TRUE);
    }
#endif // (EN_WOBBLE == 1)
}

void LAL_SetDiscTypeSelection(discKindT DiscKind)
{
    BYTE DISCSEL_reg;

    DISCSEL_reg = READ_REG(HAL_GLOBALREG_MAP->DISCSEL);

    switch (DiscKind.fDiscKind)
    {
        case eCDROMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_CD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_CD_ROM);
            break;
        case eCDRDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_CD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_CD_R);
            break;
        case eCDRWDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_CD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_CD_RW);
            break;
        case eDVDROMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_ROM);
            break;
        case eDVDPlusRDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_pR);
            break;
        case eDVDPlusRWDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_pRW);
            break;
        case eDVDMinusRDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_mRt2);
            break;
        case eDVDMinusRWDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_mRW);
            break;
        case eDVDROMDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_ROM);
            break;
        case eDVDPlusRDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_pR);
            break;
        case eDVDPlusRWDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_pRW);
            break;
        case eDVDMinusRDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_mRt2);
            break;
        case eDVDMinusRWDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_mRW);
            break;
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_DVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            if(svoVar_TrackingMethod == TRK_METHOD_DPP)
                WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_RAMv2C1);
            else
                WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_ROM);
            break;
#endif
        case eBDROMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_ROM25G);
            break;
        case eBDRDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_R25G);
            break;
        case eBDREDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_RE25G);
            break;
        case eBDROMDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_ROM25G);
            break;
        case eBDRDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_R25G);
            break;
        case eBDREDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_BD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_BD_RE25G);
            break;

#if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROM);
            break;
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROMChina);
            break;
    #endif
        case eHDRDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_R);
            break;
        case eHDRWDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_RW);
            break;
        case eHDRAMDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_RAM);
            break;
        case eHDROMDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROM);
            break;
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_ROMChina);
            break;
    #endif
        case eHDRDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_R);
            break;
        case eHDRWDLDisc:
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCSTD,halGLOBAL_DiscStd_HDDVD);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DL,halGLOBAL_DualLayer);
            WRITE_FIELDTO(DISCSEL_reg,HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_HD_RW);
            break;
#endif /* (ENABLE_HDVD == 1) */

        default:
            break;
    } /* ENDSWITCH */

    WRITE_REG(HAL_GLOBALREG_MAP->DISCSEL, DISCSEL_reg);

}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetGenericCoefs

    Description:    Write the generic coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many Registers/Fields
    Returns:        None
-----------------------------------------------------------------------------------------------------*/

void LAL_SetGenericCoefs(ECores eCore)
{
    COEFS_t CoefIndex;

    /* Load fixed coefs only when disckind == unknown */
    if (mPrevDiscKind[eCore].fDiscKind == eUnknownKind)
    {
#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA33,A1B(eCore));//GenCoef FIXED
#endif
        /* FIXED COEFS */
        for(CoefIndex = CoefDependencyProperties[eCore].Fixed.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].Fixed.Index.GenericField.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
        for(CoefIndex = CoefDependencyProperties[eCore].Fixed.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].Fixed.Index.GenericReg.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
    }

    /* Check if discgroup has changed */
   if ( (mPrevDiscKind[eCore].Bit.DiscStd != mDiscKind.Bit.DiscStd)
#if (DVD_RAM_READ == 1)
        || ((mDiscKind.fDiscKind == eDVDRAMDisc) && (mPrevDiscKind[eCore].fDiscKind != eDVDRAMDisc))
        || ((mDiscKind.fDiscKind != eDVDRAMDisc) && (mPrevDiscKind[eCore].fDiscKind == eDVDRAMDisc))
#endif
       )
   {
#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA32,A1B(eCore));//GenCoef DG
#endif

        /* DISC-GROUP COEFS */
        for(CoefIndex = CoefDependencyProperties[eCore].DiscGroup.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].DiscGroup.Index.GenericField.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
        for(CoefIndex = CoefDependencyProperties[eCore].DiscGroup.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].DiscGroup.Index.GenericReg.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
    }

    /* Check if disckind has changed */
    if (mPrevDiscKind[eCore].fDiscKind != mDiscKind.fDiscKind)
    {
#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA31,A1B(eCore));//GenCoef DK
#endif

        /* DK COEFS */
        for(CoefIndex = CoefDependencyProperties[eCore].DiscKind.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].DiscKind.Index.GenericField.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
        for(CoefIndex = CoefDependencyProperties[eCore].DiscKind.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].DiscKind.Index.GenericReg.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
    }

    /* Check if speed or discgroup has changed */
    if ( (mPrevSpeed[eCore] != mSpeed) || (mPrevDiscKind[eCore].Bit.DiscStd != mDiscKind.Bit.DiscStd)
#if (DVD_RAM_READ == 1)
         || ((mDiscKind.fDiscKind == eDVDRAMDisc) && (mPrevDiscKind[eCore].fDiscKind != eDVDRAMDisc))
         || ((mDiscKind.fDiscKind != eDVDRAMDisc) && (mPrevDiscKind[eCore].fDiscKind == eDVDRAMDisc))
#endif
       )
    {
#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA30,A1B(eCore));//GenCoef SPD
#endif

        /* SPD COEFS */
        for(CoefIndex = CoefDependencyProperties[eCore].SpeedDiscGroup.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].SpeedDiscGroup.Index.GenericField.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
        for(CoefIndex = CoefDependencyProperties[eCore].SpeedDiscGroup.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].SpeedDiscGroup.Index.GenericReg.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
    }

    /* Check if speed or disckind has changed */
    if ( (mPrevSpeed[eCore] != mSpeed) || (mPrevDiscKind[eCore].fDiscKind != mDiscKind.fDiscKind) )
    {
#ifdef DEBUG_COEF_LOADING
        SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA2F,A1B(eCore));//GenCoef DS
#endif

        /* DS COEFS - Field based    */
        for(CoefIndex = CoefDependencyProperties[eCore].SpeedDiscKind.Index.GenericField.First; CoefIndex <= CoefDependencyProperties[eCore].SpeedDiscKind.Index.GenericField.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
        for(CoefIndex = CoefDependencyProperties[eCore].SpeedDiscKind.Index.GenericReg.First; CoefIndex <= CoefDependencyProperties[eCore].SpeedDiscKind.Index.GenericReg.Last; CoefIndex++)
        {
            /* Write the coef to a register or field */
            LAL_WriteCoef(CoefIndex);
        }
    }

    /* Set the previous disckind */
    mPrevDiscKind[eCore].fDiscKind = mDiscKind.fDiscKind;

    /* Set the previous speed */
    mPrevSpeed[eCore] = mSpeed;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetServoEqCoefs

    Description:    Write the generic coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many Registers/Fields
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_SetServoEqCoefs()
{
    SendMsg80(DEBUG_COEF_MSG,0x310182); // LAL_SetServoEqCoefs
    SendMsgCn(DEBUG_COEF_MSG,2,0x310183,A2B(mPrevDiscKindServoEq.fDiscKind)); // Prev DiscKind
    SendMsgCn(DEBUG_COEF_MSG,1,0x310184,A1B(mPrevSpeedServoEq)); // Prev Speed
    SendMsgCn(DEBUG_COEF_MSG,2,0x31018A,A2B(mDiscKind.fDiscKind));// Curr DiscKind
    SendMsgCn(DEBUG_COEF_MSG,1,0x310189,A1B(mSpeed)); //Curr Speed

    if ((mPrevDiscKindServoEq.fDiscKind == eUnknownKind) || (mPrevDiscKindServoEq.Bit.DiscStd != mDiscKind.Bit.DiscStd) || (mPrevSpeedServoEq == _UNKNOWN_SPEED)
#if (DVD_RAM_READ == 1)
         || ((mDiscKind.fDiscKind == eDVDRAMDisc) && (mPrevDiscKindServoEq.fDiscKind != eDVDRAMDisc))
         || ((mDiscKind.fDiscKind != eDVDRAMDisc) && (mPrevDiscKindServoEq.fDiscKind == eDVDRAMDisc))
#endif
        )

    {
        InitSvoRegsForSpeed(mDiscKind, NONE);
    }
    if ( ( mPrevDiscKindServoEq.fDiscKind == eUnknownKind ) || ( mPrevDiscKindServoEq.fDiscKind != mDiscKind.fDiscKind ) || ( mPrevSpeedServoEq == _UNKNOWN_SPEED ) )
    {
        SetEqRegsForASpeed(mDiscKind, NONE);
    }
    InitSvoRegsForSpeed(mDiscKind, mSpeed);
    SetEqRegsForASpeed(mDiscKind, mSpeed);

    /* Set the previous disckind */
    mPrevDiscKindServoEq.fDiscKind = mDiscKind.fDiscKind;

    /* Set the previous speed */
    mPrevSpeedServoEq = mSpeed;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetReadMode

    Description:    Make sure all hal/lal modules come in readmode

    Inputs:

    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
extern BYTE CurrentPowerMode;
void LAL_SetReadMode(void)
{
#if ((PCB == 710)||(PCB==711)||(PCB == 812)||(PCB == 817))
    BYTE    LD_Status=FALSE;
#endif
#if (EN_POWER_CONTROL == 1)
    BYTE    orig_CLKSTP2,orig_CLKSTP4;
#endif

    SendMsg80(DEBUG_COEF_MSG,0x310187); // LAL_SetReadMode
    SendMsg80(DEBUG_COEF_MSG,0x3C,mDiscKind.fDiscKind>>8,mDiscKind.fDiscKind);

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            send_msg80(DEBUG_COEF_MSG,0x531000+mSpeed);
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                send_msg80(DEBUG_COEF_MSG,0x531180+mSpeed);
            }
            else
#endif
            {
                send_msg80(DEBUG_COEF_MSG,0x531070+mSpeed);
            }
            break;
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            send_msg80(DEBUG_COEF_MSG,0x5310B0+mSpeed);
            break;
#endif // (ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
        case eDiscBD:
            send_msg80(DEBUG_COEF_MSG,0x5310D0+mSpeed);
            break;
#endif // (BD_ENABLE == 1)
    }


    if (mPrevDiscKindServoEq.Bit.DiscStd != mDiscKind.Bit.DiscStd)
    {
#if (PDIC_BASE_ON_SRFO == 1)
        LAL_LASC_GetPdic(0,0,0);
#endif
#if ((PCB == 710)||(PCB==711)||(PCB == 812)||(PCB == 817))
        if (svoPrvtVar_LD_ON_Flag == ON)
        {
            LD_Status = ON;
            HAL_LASC_SetLaserOn(FALSE);
        }
#endif
    }

    HAL_LASC_SetPdic();

   // Order: First set the PLL, then Disc Sel then all other coefs (some coefs are dependent on the disc-sel)
    LAL_PLL_SetPll();
    LAL_SetDiscTypeSelection(mDiscKind);

    // Disable the non-functional interrupts
    DisableNonFunctionalInterrupts(mDiscKind);  // disable CD interrpts when DVD, and DVD interrupts when CD etc.

#if (EN_POWER_CONTROL == 1)
    orig_CLKSTP2 = READ_REG(HAL_GLOBALREG_MAP->CLKSTP2);
    READ_MOD_WRITE(HAL_GLOBALREG_MAP->CLKSTP2,HAL_GLOBAL_WBLCKSTP|HAL_GLOBAL_WBLACKSTP,0);
    orig_CLKSTP4 = READ_REG(HAL_GLOBALREG_MAP->CLKSTP4);
    READ_MOD_WRITE(HAL_GLOBALREG_MAP->CLKSTP4,HAL_GLOBAL_WBLICKSTP|HAL_GLOBAL_WBLLCKSTP,0);
#endif

    LAL_SetGenericCoefs(E_CORE_AFE); //LAL_AFE_SetGenericCoefs();
    LAL_SetGenericCoefs(E_CORE_DFE); //LAL_DFE_SetGenericCoefs();
    LAL_SetGenericCoefs(E_CORE_WBL); //LAL_WOBBLE_SetGenericCoefs();
    LAL_SetGenericCoefs(E_CORE_SRV);
    //LAL_SetGenericCoefs(E_CORE_SRVDSP); //LAL_LASC_SetGenericCoefs(); replace E_CORE_SRVDSP with below 4 lines
    WRITE_DSP_COEF(Gthr_read,     COEF_GetValue(SRV_CALC_C_Gthr_read)    );
    WRITE_DSP_COEF(Gthr_write,    COEF_GetValue(SRV_CALC_C_Gthr_write)   );
    WRITE_DSP_COEF(thresh_P_gain, COEF_GetValue(SRV_CALC_C_thresh_P_gain));
    WRITE_DSP_COEF(Gdelta_write,  COEF_GetValue(SRV_CALC_C_Gdelta_write) );

#if (LASER_DRIVER == SAC1036SP)
    COEF_SetValue(SRV_LCDELTA0_DELTA0MAX, 0x0000);
    COEF_SetValue(SRV_LCDELTA1_DELTA1MAX, 0x0000);
    COEF_SetValue(SRV_LCDELTA2_DELTA2MAX, 0x7FFF);
#endif

    LAL_SetServoEqCoefs();

#if (ENABLE_BDRLTH == 1)
    if ((svoIramVar.LTHDisclikely == 1) || (svoIramVar.LTHDisc == 1))
    {
        LAL_SetAFEandDFEforLTH();
    }
#endif

    LAL_WriteCoef(AFE_WOBAPC02_ERASE1);
    LAL_WriteCoef(AFE_WOBAPC02_APCRDGN);
    LAL_WriteCoef(AFE_WOBAPC02_GAIN5OPEN);
    LAL_WriteCoef(AFE_WOBAPC02_GAIN10);
    LAL_WriteCoef(AFE_WOBAPC02_RDAMPCD);
    LAL_WriteCoef(AFE_WOBAPC02_RINSSEL);
    LAL_WriteCoef(AFE_WOBAPC02_PDRAPC);
#if (EN_POWER_CONTROL == 1)
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2,orig_CLKSTP2);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4,orig_CLKSTP4);
#endif
    //Fetch the parameters out of the coefs and enter them here

#if  0
    if (mDiscKind.Bit.DiscStd == eDiscCD)
    {
        SendMsg80(DEBUG_COEF_MSG,0x310190); // CD Power!
        if ((svoPcalVar.CalPowerDoneBits & cd_kpower_end)==cd_kpower_end)
        {
            fetch_lasc_from_persistent(CD_OFFSET_DAC_THRESHOLD_ADDR);
        }
    }
    else if (mDiscKind.Bit.DiscStd == eDiscDVD)
    {
        SendMsg80(DEBUG_COEF_MSG,0x310192); // DVD Power!
        if ((svoPcalVar.CalPowerDoneBits & dv_kpower_end)==dv_kpower_end)
        {
            fetch_lasc_from_persistent(DV_OFFSET_DAC_THRESHOLD_ADDR);
        }
    }
#if (ENABLE_HDDVD == 1)
    else if (mDiscKind.Bit.DiscStd == eDiscHD)
    {
        SendMsg80(DEBUG_COEF_MSG,0x310194); // HD Power!
        if ((svoPcalVar.CalPowerDoneBits & hd_kpower_end)==hd_kpower_end)
        {
            fetch_lasc_from_persistent(HD_OFFSET_DAC_THRESHOLD_ADDR);
        }
    }
#endif
#if (BD_ENABLE == 1)
    else if (mDiscKind.Bit.DiscStd == eDiscBD)
    {
        SendMsg80(DEBUG_COEF_MSG,0x310196); // BD Power!
        if ((svoPcalVar.CalPowerDoneBits & bd_kpower_end)==bd_kpower_end)
        {
            fetch_lasc_from_persistent(BD_OFFSET_DAC_THRESHOLD_ADDR);
        }
    }
#endif

#endif

//    send_msg5S(1,"Read mode");
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>24,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>16,
//                COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>8,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower));
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>24,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>16,
//                COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>8,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold));
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>24,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>16,
//                COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>8,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold));
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_Read_power)>>24,COEF_GetValue(SRV_CALC_C_Read_power)>>16,
//                COEF_GetValue(SRV_CALC_C_Read_power)>>8,COEF_GetValue(SRV_CALC_C_Read_power));
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_Threshold_upper_limit)>>24,COEF_GetValue(SRV_CALC_C_Threshold_upper_limit)>>16,
//                COEF_GetValue(SRV_CALC_C_Threshold_upper_limit)>>8,COEF_GetValue(SRV_CALC_C_Threshold_upper_limit));
//    send_msg54(1,COEF_GetValue(SRV_CALC_C_Threshold_lower_limit)>>24,COEF_GetValue(SRV_CALC_C_Threshold_lower_limit)>>16,
//                COEF_GetValue(SRV_CALC_C_Threshold_lower_limit)>>8,COEF_GetValue(SRV_CALC_C_Threshold_lower_limit));

    if (LD_Status == ON)
    {
        HAL_LASC_SetLaserOn(TRUE);
    }
    svoSpeedVar.SpdPresetNeeded = TRUE;
}

#if (ENABLE_BDRLTH == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetAFEandDFEforLTH()

    Description:    Changes the required settings of AFE and DFE for the LTH discs.
    Inputs:         void
    Outputs:        void
    Global Inputs:  svoIramVar.LTHDisc
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_SetAFEandDFEforLTH(void)
{
    if (svoIramVar.LTHDisc == TRUE)
    {
        WRITE_FIELD(HAL_AFE_TEINV,!COEF_GetValue(AFE_FCTS25_TEINV));
        WRITE_FIELD(HAL_AFE_TXINV,!COEF_GetValue(AFE_FCTS29_TXINV));
        WRITE_FIELD(HAL_DFE_BLSINVHFI,!COEF_GetValue(DFE_BLSDETCFG_BLSINVHFI));

#if (PCB == 815)//Different model needs different settings
        WRITE_FIELD(HAL_DFE_BLSDLTHR, 0x08);
        WRITE_FIELD(HAL_DFE_BLSDUTHR, 0x30);
#elif(PCB == 710)||(PCB==711)||(PCB==817)
        WRITE_FIELD(HAL_DFE_BLSDLTHR, 0xD2);//0xDA
        WRITE_FIELD(HAL_DFE_BLSDUTHR, 0xF0);//0xF8
#endif
#if (AFE_NORMALIZER == 1)
        afeSvognDisable();
#endif
        SendMsg80(SHOW_DEBUG_MSG,0x33FF17); // LTH Setup
    }
    else
    {
        LAL_WriteCoef(AFE_FCTS25_TEINV);
        LAL_WriteCoef(AFE_FCTS29_TXINV);
        LAL_WriteCoef(DFE_BLSDETCFG_BLSINVHFI);

        /* BlankSignalDetectLThreshold*/
        LAL_WriteCoef( DFE_BLSDETCFG_BLSDLTHR );
        /* BlankSignalDetectUThreshold*/
        LAL_WriteCoef( DFE_BLSDETCFG_BLSDUTHR );

        SendMsg80(SHOW_DEBUG_MSG,0x33FF18); // HTL Setup
    }
}
#endif //(ENABLE_BDRLTH == 1)

#if (BD_ENABLE == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_SetResetPIC()

    Description:    Sets appropriatly the AFE, DFE, and WOBBLE blocks for proper handling
                    the PIC Area in case of BD-r or BD -RE or resets back appropriatly the AFE,
                    DFE, and WOBBLE blocks for data- or leadin/leadout Area. Normaly called when
                    entering and leaving PIC Area.
    Inputs:         SetMode {eSET_PIC | eRESET_PIC}
                    OverspeedX10 is the current overspeed value times 10.
    Outputs:        void
    Global Inputs:
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10)
{
    if ((mDiscKind.Bit.DiscStd == eDiscBD) && ((mDiscKind.Bit.Media == eMediaR)||(mDiscKind.Bit.Media == eMediaRW)))
    {
        BDR_RE_PICReading = SetMode;

        LAL_DFE_SetResetPIC(SetMode, OverspeedX10);

        if (SetMode == eSET_PIC)
        {
            HAL_GLOBAL_SetPicMode();
            WRITE_FIELD(HAL_BDC_ESFREMSIS, halBDC_ESF_Off);
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable); /* Hold AGC on PIC signals*/
            HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_AfeControl);  /* AFE controls AGC/AOC during PIC */
        }
        else
        {
            HAL_GLOBAL_ClrPicMode();
            WRITE_FIELD(HAL_BDC_ESFREMSIS, halBDC_ESF_One_One);
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable);  /* Hold AGC on PIC signals*///Later will be enabled
            HAL_DFE_LoadCalRfAgcAoc(); /*Load the RF calculated offset and gain*/
            HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_DfeControl);  /* DFE control on AGC/AOC */
        }
        LAL_AFE_SetResetPIC(SetMode, OverspeedX10);
        LAL_WOBBLE_SetResetPIC(SetMode, OverspeedX10);
        UpdateReadFilters(OverspeedX10);
        svoSpeedVar.SpdPresetNeeded = TRUE;
    }
}
#endif //(BD_ENABLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
