/* ******************************************************************************************
*
*               (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                              All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*  LAL_dfe.c
*
*  Description:
*  This file contains functions for the LAL DFE layers and includes
*  the following:
*
*
*
* Notes:
*
*
*   $Revision: 80 $
*   $Date: 11/03/15 2:22p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
#include ".\BD\bddiag.h"
#endif //  (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
#include ".\common\globals.h"
#include ".\servo\svo.h"
#include ".\servo\SvoVar.H"

#if (BD_ENABLE == 1)
#include ".\BD\bdVars.h"
#endif //  (BD_ENABLE == 1)
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_dfe.h"     /* needed for LAL_DFE_ReadRFADCmV() */
#include ".\AL\REG_servo.h"   /* needed for LAL_DFE_ReadRFADCmV() */
#if (DVD_RAM_READ == 1)
#include ".\AL\REG_dvd_decoder.h"
#endif
#include ".\AL\HAL_dfe.h"     /* needed for LAL_DFE_SetHfPll() */
#include ".\AL\HAL_pll.h"
#include ".\al\LAL.h"
#include ".\al\LAL_dfe.h"
#include ".\common\ADC.h"     /* needed for LAL_DFE_ReadRFADCmV() */

/* Global definitions regarding COEF */
#if (BD_ENABLE == 1)
extern BOOL BDR_RE_PICReading;
#endif

#if (CD_DFE_IMPROVE_READ == 1)
#define CD_KPCFG_NUM    2
#define CD_KICFG_NUM    2
static const BYTE CD_KPKICFG[CD_KPCFG_NUM][CD_KICFG_NUM] =
{
    {4,7},{3,7}
};
BYTE DFE_CHG_CASE;//bit0:Change KPCFG/KICFG
BYTE DFE_CHG_SEL;
#endif

#if (HORROR_DISC_IMPROVEMENT == 1)
extern BYTE Kp,Ki,TryDFE_Param;// == modify for SCD-2383   // == modify for DQA Disc to retry DFE parameter
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_Init

    Description:    Initialise the appropriate DFE registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: DFE Registers listed in the INIT coef table
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_Init(void)
{
    /* Set the previous and current Disc Kind/mSpeed to unknown */
    HAL_DFE_Init();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetEqualizer

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (PCB == 815) && (ENABLE_BDRLTH == 1)
#define DFE_CALC_EQTAPA1_BDRLTH 0x03
#define DFE_CALC_EQTAPA2_BDRLTH 0x03
#endif

void LAL_DFE_SetEqualizer(lalDFE_DFEEqualizerStrength_t Strength)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif
    switch (Strength)
    {
        case lalDFE_EqualizerOff:
            WRITE_FIELD(HAL_DFE_EQTAPA1, halDFE_PLLEqTapDisable);
            WRITE_FIELD(HAL_DFE_EQTAPA2, halDFE_PLLEqTapDisable);
            break;
        case lalDFE_EqualizerStrengthHigh:
            WRITE_FIELD(HAL_DFE_EQTAPA1, COEF_GetValue(DFE_CALC_EQTAPA1_High) );
            WRITE_FIELD(HAL_DFE_EQTAPA2, COEF_GetValue(DFE_CALC_EQTAPA2_High) );
            break;
        case lalDFE_EqualizerStrengthLow:
            WRITE_FIELD(HAL_DFE_EQTAPA1, COEF_GetValue(DFE_CALC_EQTAPA1_Low) );
            WRITE_FIELD(HAL_DFE_EQTAPA2, COEF_GetValue(DFE_CALC_EQTAPA2_Low) );
            break;
#if (DVD_RAM_READ == 1)
        case lalDFE_EqualizerDVDRAMEmbossed:
            WRITE_FIELD(HAL_DFE_EQTAPA1, COEF_GetValue(DFE_CALC_EQTAPA1_Embossed) );
            WRITE_FIELD(HAL_DFE_EQTAPA2, COEF_GetValue(DFE_CALC_EQTAPA2_Embossed) );
            break;
#endif
        case lalDFE_EqualizerStrengthNormal:
        default:
        #if (PCB == 815) && (ENABLE_BDRLTH == 1)
            if (svoIramVar.LTHDisc == 1)
            {
                WRITE_FIELD(HAL_DFE_EQTAPA1, DFE_CALC_EQTAPA1_BDRLTH );
                WRITE_FIELD(HAL_DFE_EQTAPA2, DFE_CALC_EQTAPA2_BDRLTH );
            }
            else
        #endif
            {
            WRITE_FIELD(HAL_DFE_EQTAPA1, COEF_GetValue(DFE_CALC_EQTAPA1_Normal) );
            WRITE_FIELD(HAL_DFE_EQTAPA2, COEF_GetValue(DFE_CALC_EQTAPA2_Normal) );
            }
            break;
    }
#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
    //enable old way of programming (a specific HAL function might be needed later on!!!)
    WRITE_FIELD(HAL_DFE_ADEQPRGOLD, 1);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetNLE

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (PCB == 815) && (ENABLE_BDRLTH == 1)
#define DFE_CALC_TAPGAIN_BDRLTH 62
#endif



void LAL_DFE_SetNLE(lalDFE_DFEEqualizerStrength_t Strength)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

    LAL_WriteCoef( DFE_HFNLECFG_NLEMODE );

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif
    switch (Strength)
    {
        case lalDFE_EqualizerStrengthHigh:
            WRITE_FIELD(HAL_DFE_TAPGAIN, COEF_GetValue(DFE_CALC_TAPGAIN_High) );
            break;
        case lalDFE_EqualizerStrengthLow:
            WRITE_FIELD(HAL_DFE_TAPGAIN, COEF_GetValue(DFE_CALC_TAPGAIN_Low) );
            break;
#if (DVD_RAM_READ == 1)
        case lalDFE_EqualizerDVDRAMEmbossed:
            WRITE_FIELD(HAL_DFE_TAPGAIN, COEF_GetValue(DFE_CALC_TAPGAIN_Embossed) );
            break;
#endif
        case lalDFE_EqualizerStrengthNormal:
        default:
        #if (PCB == 815) && (ENABLE_BDRLTH == 1)
            if (svoIramVar.LTHDisc == 1)
                WRITE_FIELD(HAL_DFE_TAPGAIN, DFE_CALC_TAPGAIN_BDRLTH );
            else
        #endif
            WRITE_FIELD(HAL_DFE_TAPGAIN, COEF_GetValue(DFE_CALC_TAPGAIN_Normal) );
            break;
    }
#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetClipNLE

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if HORROR_DISC_IMPROVEMENT == 1
extern BYTE CDRW_SP,CLIPLVL;//  == modify for SCD-2383
#endif //HORROR_DISC_IMPROVEMENT == 1

#if (PCB == 815) && (ENABLE_BDRLTH == 1)
#define DFE_CALC_CLIPLVL_BDRLTH 0x20
#endif

void LAL_DFE_SetClipNLE(lalDFE_DFEEqualizerStrength_t Strength)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif
    /* Deacy_Amp*/
    //LAL_WriteCoef( DFE_HFNLECFG_DCYAMP );

    /* Attack_Amp*/
    //LAL_WriteCoef( DFE_HFNLECFG_ATTAMP );

    /* Hold_Bypass_Onheader*/
    //LAL_WriteCoef( DFE_HFNLECFG_HLDBYPHDR );

    /* Hold_Acl_Ondefect*/
    //LAL_WriteCoef( DFE_HFNLECFG_HLDACLOD );

    /* Auto_Clip_Enable*/
    //LAL_WriteCoef( DFE_HFNLECFG_AUCLIPEN );

    /* clip_level*/
#if (HORROR_DISC_IMPROVEMENT == 1)

    if ((TryDFE_Param&0x04)==0x04)        //  ==start== modify for SCD-2383
    {
        WRITE_FIELD(HAL_DFE_CLIPLVL, CLIPLVL );

    }                                          // ==end== modify for SCD-2383
    else
#endif //(HORROR_DISC_IMPROVEMENT == 1)

    {
        switch (Strength)
        {
            case lalDFE_EqualizerStrengthHigh:
                WRITE_FIELD(HAL_DFE_CLIPLVL, COEF_GetValue(DFE_CALC_CLIPLVL_High) );
                break;
            case lalDFE_EqualizerStrengthLow:
                WRITE_FIELD(HAL_DFE_CLIPLVL, COEF_GetValue(DFE_CALC_CLIPLVL_Low) );
                break;
    #if (DVD_RAM_READ == 1)
            case lalDFE_EqualizerDVDRAMEmbossed:
                WRITE_FIELD(HAL_DFE_CLIPLVL, COEF_GetValue(DFE_CALC_CLIPLVL_Embossed) );
                break;
    #endif
            case lalDFE_EqualizerStrengthNormal:
            default:

                #if (PCB == 815) && (ENABLE_BDRLTH == 1)
                if (svoIramVar.LTHDisc == 1)
                    WRITE_FIELD(HAL_DFE_CLIPLVL, DFE_CALC_CLIPLVL_BDRLTH );
                else
                #endif
                WRITE_FIELD(HAL_DFE_CLIPLVL, COEF_GetValue(DFE_CALC_CLIPLVL_Normal) );
                break;
        }
    }
#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetBwDataSlicer

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetBwDataSlicer(void)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        if (DVDRAM_embossed)
        {
            LAL_WriteCoef(DFE_HFSLCCFG_SLCSWONH_EMBOSSED);
            LAL_WriteCoef(DFE_HFSLCCFG_SLCDRCFG_EMBOSSED);
        }
        else
        {
            LAL_WriteCoef(DFE_HFSLCCFG_SLCBWH);
            LAL_WriteCoef(DFE_HFSLCCFG_SLCBWHI);
            LAL_WriteCoef(DFE_HFSLCCFG_SLCSWONH);
            LAL_WriteCoef(DFE_HFSLCCFG_SLCDRCFG);
        }
    }
#endif
    LAL_WriteCoef(DFE_HFSLCCFG_SLCBW);

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetBwFastPll()

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (PCB == 815) && (ENABLE_BDRLTH == 1)
#define DFE_CALC_FPLLKPCFG_BDRLTH 0x04//org:3
#define DFE_CALC_FPLLKICFG_BDRLTH 0x06//org:5
#endif
void LAL_DFE_SetBwFastPll(void)
{
#if (HORROR_DISC_IMPROVEMENT == 1)
    if((read_retry_option&(READ_RETRY_ENABLE|DFE_RETRY_ENABLE)) == (READ_RETRY_ENABLE|DFE_RETRY_ENABLE))
    {
        return;
    }
#endif

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    if (READ_LDC_BIS == 0)
    {//Normal use
#endif

#if (CD_DFE_IMPROVE_READ == 1)
    if ( (mDiscKind.Bit.DiscStd == eDiscCD)&&((DFE_CHG_CASE&0x01)||(DFE_CHG_SEL)) )
    {
        if(DFE_CHG_SEL > CD_KPCFG_NUM)
        {
            DFE_CHG_SEL = 0;
            LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCFG );
            LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICFG );
        }
        else
        {
            WRITE_FIELD(HAL_DFE_FPLLKPCFG,CD_KPKICFG[DFE_CHG_SEL-1][0]);
            WRITE_FIELD(HAL_DFE_FPLLKICFG,CD_KPKICFG[DFE_CHG_SEL-1][1]);
        }
        if(DFE_CHG_CASE&0x01)
        {
            SendMsgCn(SHOW_DEBUG_MSG,2,0x33FF19,READ_FIELD(HAL_DFE_FPLLKPCFG),READ_FIELD(HAL_DFE_FPLLKICFG));
            DFE_CHG_CASE &= 0xFE;
        }
        return;
    }
#endif//(CD_DFE_IMPROVE_READ == 1)

    /* FastPllKpConfig*/
    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCH );

    /* FastPllKiConfig*/
    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICH );

#if (DVD_RAM_READ == 1)
    if (DVDRAM_embossed)
    {
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLDRCFG_EMBOSSED);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLARHE_EMBOSSED);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLJHIDE_EMBOSSED);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLJHLDE_EMBOSSED);
    }
    else
#endif
    {
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLDRCFG);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLARHE);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLJHIDE);
        LAL_WriteCoef(DFE_HFFPLLCFG_FPLLJHLDE);
    }

    /* FastPllKpConfig*/
    // == start == modify for DQA Disc to retry DFE parameter
#if (BD_ENABLE == 1)
    if(mDiscKind.Bit.DiscStd == eDiscBD)
    {
        if (BDR_RE_PICReading == SET)
        {
            WRITE_FIELD(HAL_DFE_FPLLKPCFG,LAL_PIC_FPLL_KP_BDR_RE);
        }
        else
        {
    #if (PCB == 815) && (ENABLE_BDRLTH == 1)
            if (svoIramVar.LTHDisc == 1)
            {
                WRITE_FIELD(HAL_DFE_FPLLKPCFG,DFE_CALC_FPLLKPCFG_BDRLTH);
            }
            else
    #endif
            {
                LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCFG );
            }
        }

        /* FastPllKiConfig*/
    #if (PCB == 815) && (ENABLE_BDRLTH == 1)
        if (svoIramVar.LTHDisc == 1)
        {
            WRITE_FIELD(HAL_DFE_FPLLKICFG,DFE_CALC_FPLLKICFG_BDRLTH);
        }
        else
    #endif
        {
            LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICFG );
        }
    }
    else
#endif //(BD_ENABLE == 1)
    {
#if (HORROR_DISC_IMPROVEMENT == 1)
        if((TryDFE_Param&0x01)==0)
#endif //(HORROR_DISC_IMPROVEMENT == 1)

        {
            LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCFG );
            LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICFG );
         }
#if (HORROR_DISC_IMPROVEMENT == 1)
         else if ((TryDFE_Param&0x01)==0x01)
         {
            WRITE_FIELD(HAL_DFE_FPLLKPCFG,Kp);
            WRITE_FIELD(HAL_DFE_FPLLKICFG,Ki);
         }
#endif //(HORROR_DISC_IMPROVEMENT == 1)

    }
    //== end == modify for DQA Disc to retry DFE parameter

#if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON == 1)
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetClipLvlFastPll()

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetClipLvlFastPll(void)
{
    /* FastPllIntegerClipLevel*/
    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLINCL );
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AnalizeDvdRamRfSignal()

    Description:    Measures peak value of DVDRAM HF in header and data field.
                    This function is only valid for DVDRAM
    Inputs:
    Outputs:        None
    Global Inputs:
    Global Outputs:
    Returns:        HfPeakMeasStruct
-----------------------------------------------------------------------------------------------------*/
HfPeakMeasStruct LAL_AnalizeDvdRamRfSignal()
{
    HfPeakMeasStruct HfPeakMeas;
    BOOL bLand;

    HAL_DFE_EnDvdRamAnalizeMode();

    if (READ_FIELD(HAL_DVDD_LANDMVD))
    {
        if (READ_FIELD(HAL_DVDD_LANDMON))
        {
            bLand = TRUE;
        }
    }
    else
    {
        if (READ_FIELD(HAL_WBL_MLLSLCDET))
        {
            bLand = READ_FIELD(HAL_WBL_LGWBMLH);
        }
        else
        {
            bLand = READ_FIELD(HAL_WBL_LGWBSLC);
        }
    }

    HfPeakMeas.MaxPeakHeader = READ_FIELD(HAL_DFE_MAXPKAGC);
    HfPeakMeas.MinPeakHeader = READ_FIELD(HAL_DFE_MINPKAGC);
    HfPeakMeas.MaxPeakData = READ_FIELD(HAL_DFE_MAXPKAOC);
    HfPeakMeas.MinPeakData = READ_FIELD(HAL_DFE_MINPKAOC);
    HfPeakMeas.Land = bLand;

    return HfPeakMeas;
}
#endif

#if 0//currently not be use
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetDefectDet()

    Description:    Write the DFE generic and specific coefs all to the appropriate registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many DFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetDefectDet(void)
{
    /* AttackFiltBWProgAtt*/
    LAL_WriteCoef( DFE_HFDETCFG1_ATTFBWPA );

    /* DecayFiltBWProgAtt*/
    LAL_WriteCoef( DFE_HFDETCFG1_DECFBWPA );

    /* LthresholdProgAtt*/
    LAL_WriteCoef( DFE_HFDETCFG1_LTHRPA );

    /* UthresholdProgAtt*/
    LAL_WriteCoef( DFE_HFDETCFG1_UTHRPA );

    /* MaskDVDRAM*/
    LAL_WriteCoef( DFE_HFDETCFG2_MSKDR );

    /* DecayFiltBWImmAtt*/
    LAL_WriteCoef( DFE_HFDETCFG2_DECFBWIA );

    /* LthresholdImmAtt*/
    LAL_WriteCoef( DFE_HFDETCFG2_LTHRIA );

    /* UthresholdImmAtt*/
    LAL_WriteCoef( DFE_HFDETCFG2_UTHRIA );

    /* HFHLDDCFG*/
    LAL_WriteCoef(DFE_HFHLDDCFG);

    /* InvertHFInputBlack*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDINVHFI );

    /* BlackDotDetectEnable*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDEN );

    /* BlackDotDetectDecayBw*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDDECBW );

    /* BlackDotDetectAttackBw*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDATTBW );

    /* BlackDotDetectAttackType*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDATTTY );

    /* BlackDotDetectLThreshold*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDLTHR );

    /* BlackDotDetectUThreshold*/
    LAL_WriteCoef( DFE_BKDDETCFG_BKDDUTHR );

    /* InvertHFInputWhite*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDINVHFI );

    /* WhiteDotDetectEnable*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDEN );

    /* WhiteDotDetectDecayBw*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDDECBW );

    /* WhiteDotDetectAttackBw*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDATTBW );

    /* WhiteDotDetectAttackType*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDATTTY );

    /* WhiteDotDetectLThreshold*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDLTHR );

    /* WhiteDotDetectUThreshold*/
    LAL_WriteCoef( DFE_WHDDETCFG_WHDDUTHR );

    /* InvertHFInputBlank*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSINVHFI );

    /* BlankSignalDetectEnable*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDEN );

    /* BlankSignalDetectDecayBw*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDDECBW );

    /* BlankSignalDetectAttackBw*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDATTBW );

    /* BlankSignalDetectAttackType*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDATTTY );

    /* BlankSignalDetectLThreshold*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDLTHR );

    /* BlankSignalDetectUThreshold*/
    LAL_WriteCoef( DFE_BLSDETCFG_BLSDUTHR );

    /* InvertHFInputBlackEarly*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDINVI );

    /* EarlyBlackDotDetectEnable*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDDEN );

    /* EarlyBlackDotDetectDecayBW*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDDECBW );

    /* EarlyBlackDotDetectAttackBW*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDATTBW );

    /* EarlyBlackDotDetectAttackType*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDATTTY );

    /* EarlyBlackDotDetectLThreshold*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDLTHR );

    /* EarlyBlackDotDetectUThreshold*/
    LAL_WriteCoef( DFE_EBKDDTCFG_EBKDUTHR );

    /* InvertHFInputWhiteEarly*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDINVI );

    /* EarlyWhiteDotDetectEnable*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDDEN );

    /* EarlyWhiteDotDetectDecayBw*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDDECBW );

    /* EarlyWhiteDotDetectAttackBw*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDATTBW );

    /* EarlyWhiteDotDetectAttackType*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDATTTY );

    /* EarlyWhiteDotDetectLTreshold*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDLTHR );

    /* EarlyWhiteDotDetectUTreshold*/
    LAL_WriteCoef( DFE_EWHDDTCFG_EWHDUTHR );
}
#endif //currently not be use

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_ConfigHfPll()

    Description:    This function configures HF Pll

    Inputs:         ForceAutoLock
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_ConfigHfPll(BOOL ForceAutoLock)
{
    /* write BDLKAIDM */
    LAL_WriteCoef( DFE_HFPLLCFG_BDLKAIDM);

    /* write PUSHOOL */
    WRITE_FIELD(HAL_DFE_PUSHOOL, halDFE_PUSHOOLEnable);    /* Using a COEF or not? */

    /* write HFPLLHOD */
    WRITE_FIELD(HAL_DFE_HFPLLHOD, halDFE_HFPLLHODEnable);    /* Using a COEF or not? */

    /* Check if the PLLMODE was forced to go to Automatic */
    if (ForceAutoLock == TRUE)
    {
        /* Set the PLLMODE to Automatic */
        WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeAutomatic);
    }
    else
    {
#if (EN_WOBBLE == 0)
        /* Set the PLLMODE to Automatic */
        WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeAutomatic);
#else // (EN_WOBBLE == 0)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if ((mDiscKind.Bit.Media == eMediaROM)
        #if (DVD_RAM_READ == 1)
            || (DVDRAM_embossed)
        #endif
    #else
    if ((((mDiscKind.Bit.DiscStd != eDiscBD) || (mDiscKind.Bit.Media == eMediaROM))
        #if (DVD_RAM_READ == 1)
            && (!DVDRAM_rewritable)
        #endif
        )
    #endif
    #if (BD_ENABLE == 1)
        || (svo_BDR_RE_PICReading == SET)
    #endif
        )
        {
            /* Set the PLLMODE to Automatic */
            WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeAutomatic);
        }
        else
        {
            /* Set the PLLMODE to Wobble */
            WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModePresetFromWobble);
        }
#endif // (EN_WOBBLE == 0)
    }
}
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetHfPllLockAids()

    Description:    This function configures HF Pll

    Inputs:         ForceAutoLock
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetHfPllLockAids(void)
{
    /* write INLKTHR */
    LAL_WriteCoef( DFE_HFPLLCFG_INLKTHR);

    /* write SYNCDIS */
    WRITE_FIELD(HAL_DFE_SYNCDIS, halDFE_SYNCDISEnable);    /* Using a COEF or not? */

    /* write INNLKTHR */
    LAL_WriteCoef( DFE_HFPLLCFG_INNLKTHR);
}
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetAveRl()

    Description:    This function configures HF Pll

    Inputs:         ForceAutoLock
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetAveRl(void)
{
    /* write ARLCV */
    LAL_WriteCoef( DFE_HFPLLCFG_ARLCV);

    /* write ATTDECKO */
    LAL_WriteCoef( DFE_HFPLLCFG_ATTDECKO);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetHfPll()

    Description:    This function configures the HF lock aids and the semi synchronous
                    domain processing

    Inputs:         ForceAutoLock
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetHfPll(BOOL ForceAutoLock)
{
    LAL_DFE_ConfigHfPll(ForceAutoLock);
    LAL_DFE_SetHfPllLockAids();
    LAL_DFE_SetAveRl();
}

#if (BD_ENABLE == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetResetPIC

    Description:    Sets PIC dependent settings in DFE block (SET) or
                    Resets PIC dependent settings in DFE block back to normoal wobble values (CLEAR)

    Inputs:         SetMode {eSET_PIC | eRESET_PIC}
                    OverspeedX10 is the current overspeed value times 10.
    Outputs:        None
    Global Inputs: (field)
                    dfe_DiscKind
    Global Outputs: HAL_DFE_NLEMODE, HAL_DFE_VITPASSEN,  HAL_DFE_INTEGRAT
                    HAL_DFE_PICFILBP, HAL_GLOBAL_BDREPIC fields
    Returns:        void
    pre:            TRUE
    post:           PIC circuitry enabled (eSET_PIC) or
                    PIC circuitry disabled (normal wobble settings restored!) (eRESET_PIC)
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10)
{
    if ((mDiscKind.Bit.DiscStd == eDiscBD) && (mDiscKind.Bit.Media != eMediaROM))
    {
        if (SetMode == eSET_PIC)
        {
            WRITE_FIELD(HAL_DFE_NLEMODE, halDFE_NLEModeOff);
            WRITE_FIELD(HAL_DFE_VITPASSEN, lalDFE_VitPassThruEnable);
            LAL_DFE_SetEqualizer(lalDFE_EqualizerOff);
            WRITE_FIELD(HAL_DFE_PICFILBP, lalDFE_PicClipFilBypass);
        }
        else
        {
            LAL_WriteCoef(DFE_HFNLECFG_NLEMODE);
            LAL_WriteCoef(DFE_HFBITDCFG_VITPASSEN);
            LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
            LAL_WriteCoef(DFE_HFHIPASS_PICFILBP);
        }
        HAL_DFE_PresetPLLFreq(OverspeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod);
    }
    return;
}
#endif // (BD_ENABLE == 1)

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_DFE_SetDigiBetaMode()

    Description:    This function configures the HF to measure the beta

    Inputs:         OverspeedX100
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_DFE_SetDigiBetaMode(ULONG OverspeedX100)
{
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
