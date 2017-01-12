/* ******************************************************************************************
*
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
* $Revision: 87 $
* $Date: 11/03/11 2:32p $
*
* Description:
*  This file contains functions for the LAL AFE layers and includes
*  the following:
*
*
* Notes:
*
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\oem\oem_info.h"
#include ".\servo\svo.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_afe.h"
#include ".\al\HAL_afe.h"
#include ".\al\LAL.h"
#include ".\al\LAL_afe.h"

#include ".\Servo\Svo_afe.h"
#include ".\Servo\Svo_calib.h"
#include ".\Servo\Afe_tbl.h"
#include ".\Servo\SvoVar.H"

/* Global definitions regarding COEF */

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_Init

    Description:    Initialise the appropriate AFE registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: AFE Registers listed in the INIT coef table
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_Init(void)
{
    HAL_AFE_Init();
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_ClearAFEOffsets

    Description:    clear afe offsets

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many AFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_ClearAFEOffsets(void)
{
    ULONG i, j;

    for (i = 0; i < MAX_TYPE1_OFS_NO; i++) //reset AfeOffsetInfoT1 table
    {
        WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[i].field_write_ofs), 0);
#if (CHIP_REV >= 0xC0)
        WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[i].field_land_write_ofs), 0);
#endif
        for (j = 0; j<2; j++)
        {
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[i].field_groove_ofs[j]), 0);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[i].field_land_ofs[j]), 0);
        }
    }

    for (i = 0; i < MAX_TYPE2_OFS_NO; i++) //reset AfeOffsetInfoT2 table
    {
        WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[i].field_write_ofs), 0);
        WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[i].field_normal_ofs), 0);
    }

    for (i = 0; i < MAX_TYPE3_OFS_NO; i++) //reset AfeOffsetInfoT3 table
    {
        WRITE_REG(*(BYTE *)(AfeOffsetInfoT3[i].field_normal_ofs), 0);
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_InitAFEGains

    Description:    initialise afe gains with values from coef-table

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many AFE Registers
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_InitAFEGains(void)
{
    //LAL_WriteCoef(AFE_RF06_RFGNHDR);
    //LAL_WriteCoef(AFE_RF06_RFGNLND);
    LAL_WriteCoef(AFE_FCTS37_TEBG);
    LAL_WriteCoef(AFE_MB16_MPPOBG);
    LAL_WriteCoef(AFE_MB19_FEOBG);
    LAL_WriteCoef(AFE_SB16_SPP1BG);
    LAL_WriteCoef(AFE_SB18_SPP2BG);

    //LAL_WriteCoef(AFE_RF06_RFGNWLD);
    LAL_WriteCoef(AFE_BSRP25_BSKG);
    LAL_WriteCoef(AFE_BSRP24_BLANKBSKG);
    LAL_WriteCoef(AFE_BSRP16_RP2GN);
    LAL_WriteCoef(AFE_FCTS05_BLANKFCSG);
    LAL_WriteCoef(AFE_FCTS06_BLNDFCSG);
    LAL_WriteCoef(AFE_FCTS07_FCSG);
    LAL_WriteCoef(AFE_FCTS08_LNDFCSG);
    LAL_WriteCoef(AFE_FCTS09_WRFCSG);
    LAL_WriteCoef(AFE_FCTS20_BLANKTRKG);
    LAL_WriteCoef(AFE_FCTS21_BLNDTRKG);
    LAL_WriteCoef(AFE_FCTS22_TRKG);
    LAL_WriteCoef(AFE_FCTS23_LNDTRKG);
    LAL_WriteCoef(AFE_FCTS24_WRTRKG);

    //restore Drooprate of PHSO & BHSO
    LAL_WriteCoef(AFE_INIT_BSRP21_PHSCSEL);
    LAL_WriteCoef(AFE_INIT_BSRP21_PHSDIS);
    LAL_WriteCoef(AFE_INIT_BSRP03_PHSDISCK);
    LAL_WriteCoef(AFE_INIT_BSRP21_BHSCSEL);
    LAL_WriteCoef(AFE_INIT_BSRP21_BHSDIS);
    LAL_WriteCoef(AFE_INIT_BSRP04_BHSDISCK);

    svoCalGnT3Var[0][TEB_GN].normal_gain   = COEF_GetValue(AFE_FCTS37_TEBG);
    svoCalGnT3Var[0][MPPB_GN].normal_gain  = COEF_GetValue(AFE_MB16_MPPOBG);
    svoCalGnT3Var[0][SPP1B_GN].normal_gain = COEF_GetValue(AFE_SB16_SPP1BG);

    svoCalGnT2Var[0][BS_GN].normal_gain    = COEF_GetValue(AFE_BSRP25_BSKG);
    svoCalGnT2Var[0][BS_GN].write_gain     = COEF_GetValue(AFE_BSRP24_BLANKBSKG);
    svoCalGnT3Var[0][RP_GN].normal_gain    = COEF_GetValue(AFE_BSRP16_RP2GN);
    svoCalGnT1Var[0][FE_GN].groove_gain[0] = COEF_GetValue(AFE_FCTS05_BLANKFCSG);
    svoCalGnT1Var[0][FE_GN].land_gain[0]   = COEF_GetValue(AFE_FCTS06_BLNDFCSG);
    svoCalGnT1Var[0][FE_GN].groove_gain[1] = COEF_GetValue(AFE_FCTS07_FCSG);
    svoCalGnT1Var[0][FE_GN].land_gain[1]   = COEF_GetValue(AFE_FCTS08_LNDFCSG);
    svoCalGnT1Var[0][FE_GN].write_gain     = COEF_GetValue(AFE_FCTS09_WRFCSG);
    svoCalGnT1Var[0][TE_GN].groove_gain[0] = COEF_GetValue(AFE_FCTS20_BLANKTRKG);
    svoCalGnT1Var[0][TE_GN].land_gain[0]   = COEF_GetValue(AFE_FCTS21_BLNDTRKG);
    svoCalGnT1Var[0][TE_GN].groove_gain[1] = COEF_GetValue(AFE_FCTS22_TRKG);
    svoCalGnT1Var[0][TE_GN].land_gain[1]   = COEF_GetValue(AFE_FCTS23_LNDTRKG);
    svoCalGnT1Var[0][TE_GN].write_gain     = COEF_GetValue(AFE_FCTS24_WRTRKG);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_GetMbSbInpValues()

    Description:    Get the coef values to calculate the Main/Sub Beam Input Gain (MBGUPSEL[4:0]/SBGUPSEL[4:0])

    Inputs:         SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_GetMbSbInpValues(USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10)
{
    SBYTE     GainUpSel,SbDiff;
    USHORT    MainBeamTargetX1000;

    /* Get COEF value for AFE_CALC_MbTargetX100 */
    /* Determine the value for MainBeamTargetX1000 */
    MainBeamTargetX1000 = COEF_GetValue(AFE_CALC_MbTargetX100) * 10;

    SRFOSTMGaindBX10 -= ((READ_FIELD(HAL_AFE_SRFOBGA) - 0x10) * 10);

    /* Calculating the Main/Sub Beam Input Gain (MBGUPSEL[4:0]/SBGUPSEL) */
    GainUpSel = HAL_AFE_CalcMbInpGain(MainBeamTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10);

    /* Write the MBGUPSEL[4:0] to its corresponding field */
    calSetAfeGain(0,MB_GN,TYPE2,PATH_SELECT_ALL,0,GainUpSel);

    SbDiff = RoundDivide((SBYTE)COEF_GetValue(AFE_CALC_PdicGrRatiodBX4),4);
    /* Calculate the Sub beam input gain, which must be written to SBGUPSEL[4:0] */
    GainUpSel = READ_FIELD(HAL_AFE_MBGUPSEL) + SbDiff;

    /* Write the SBGUPSEL[4:0] to its corresponding field */
    calSetAfeGain(0,SB_GN,TYPE2,PATH_SELECT_ALL,0,GainUpSel);

    if (READ_FIELD(HAL_AFE_SBGUPSEL) != GainUpSel)
    {
        calSetAfeGain(0,MB_GN,TYPE2,PATH_SELECT_ALL,0,READ_FIELD(HAL_AFE_SBGUPSEL) - SbDiff);
        //SendMsgCn(SHOW_DEBUG_MSG,4,0x33FF20,READ_FIELD(HAL_AFE_MBGUPSEL),READ_FIELD(HAL_AFE_SBGUPSEL),svoCalGnT2Var[0][MB_GN].write_gain,svoCalGnT2Var[0][MB_GN].normal_gain); // debug
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_GetDPDInpValues()
    Name:           CalcDPDInpGain()

    Description:    Get the coef values to calculate the DPD Input Gain (DPDGN[3:0])
    Description:    Calculating the DPD Input Gain (DPDGN[3:0])

    Inputs:         SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_GetDPDInpValues(BYTE layer, USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10)
{
    USHORT    DPDTargetX1000;

    /* Get COEF value for AFE_CALC_DpdTargetX1000 */
    /* Get the value for DPDTargetX1000 */
    DPDTargetX1000 = COEF_GetValue(AFE_CALC_DpdTargetX1000);
    DPDTargetX1000 = DPDTargetX1000 * 100 / (svoRRFVar.modulation[layer] == 0? COEF_GetValue(DFE_CALC_M) : svoRRFVar.modulation[layer]);

    /* Calculating the DPD Input Gain (DPDGN[3:0]) */
    calSetAfeGain(layer,DPD_GN,TYPE3,0,0,HAL_AFE_CalcDPDInpGain(DPDTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10));
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_GetWobbleValues()
    Name:           CalcWobbleInpGain()

    Description:    Get the coef values to calculate the Wobble Input Gain
    Description:    Calculating the Wobble Input Gain
                    (RADGNB[1:0], RADPGNB[1:0], RBCGNB[1:0], RBCPGNB[1:0])

    Inputs:         SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_GetWobbleValues(USHORT SRFOPeakX1000, SHORT SRFOSTMGaindBX10)
{
    USHORT    WBLTargetX1000;
    BYTE      WBGn_WBPGn;

    /* Get COEF value for AFE_CALC_WobbleTargetX100 */
    /* Get the value for WBLTargetX1000 */
    WBLTargetX1000 = COEF_GetValue(AFE_CALC_WobbleTargetX100) * 10;

    /* Calculate the wobble input gain */
    WBGn_WBPGn = HAL_AFE_CalcWobbleInpGain(WBLTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10);
    calSetAfeGain(0,RAD_GN,TYPE2,PATH_SELECT_ALL,0,WBGn_WBPGn >> 4);
    calSetAfeGain(0,RBC_GN,TYPE2,PATH_SELECT_ALL,0,WBGn_WBPGn >> 4);
    calSetAfeGain(0,RADP_GN,TYPE2,PATH_SELECT_ALL,0,WBGn_WBPGn & 0x0F);
    calSetAfeGain(0,RBCP_GN,TYPE2,PATH_SELECT_ALL,0,WBGn_WBPGn & 0x0F);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_GetRFInpValues()

    Description:    Get the coef values to calculate the RF input 5V buffer gain and RF Coarse gain setting (RFGN)

    Inputs:         SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_GetRFInpValues(USHORT SRFOPeakX1000L0, USHORT SRFOPeakX1000L1, SHORT SRFOSTMGaindBX10)
{
    USHORT    RfSrfoTargetX1000;
    BYTE      Rfingn_Rfgno;

    /* Get the value for RfSrfoTargetX1000 */
    RfSrfoTargetX1000 = COEF_GetValue(AFE_CALC_RfSrfoTargetX1000) * 100 / COEF_GetValue(DFE_CALC_M);

    Rfingn_Rfgno = HAL_AFE_CalcRFInpGain(RfSrfoTargetX1000, SRFOPeakX1000L0, SRFOPeakX1000L1, SRFOSTMGaindBX10);
    calSetAfeGain(0,RFIN_GN,TYPE3,0,0,Rfingn_Rfgno >> 6);
    calSetAfeGain(1,RF_GN,TYPE2,PATH_SELECT_ALL,0,(Rfingn_Rfgno >> 3) & 0x07);
    calSetAfeGain(0,RF_GN,TYPE2,PATH_SELECT_ALL,0,Rfingn_Rfgno & 0x07);

    SendMsgCn(SHOW_DEBUG_MSG,2,0x310584,Rfingn_Rfgno>>6,Rfingn_Rfgno & 0x07); // RFIN_GN , RFGNO
}

#if (BD_ENABLE == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_AFE_SetResetPIC()
    Description:    Sets te gainup when in the PIC Area or te gain down when in data- or
                    leadin/leadout Area. Normaly called when entering and leaving PIC Area.
    Inputs:         SetMode {eSET_PIC | eRESET_PIC}
                    OverspeedX10 is the current overspeed value times 10.
    Outputs:        void
    Global Inputs:   (field)
                    afe_DiscKind
    Global Outputs:
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
void LAL_AFE_SetResetPIC(SetPicMode_t SetMode, USHORT OverspeedX10)
{
    if ((mDiscKind.Bit.DiscStd == eDiscBD) && ((mDiscKind.Bit.Media == eMediaR)||(mDiscKind.Bit.Media == eMediaRW)))
    {
        HAL_AFE_SetWobbleSpeed(OverspeedX10,COEF_GetValue(AFE_CALC_WlpfRatioX10),
            COEF_GetValue(AFE_CALC_WhpfRatioX100));
    }
}
#endif // (BD_ENABLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
