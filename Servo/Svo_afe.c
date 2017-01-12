/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 660 $
* $Date: 11/03/11 2:41p $
*
* DESCRIPTION
*
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\common\Scomm.h"
#include ".\servo\svo_afe.h"
#include ".\servo\afe_tbl.h"
#include ".\servo\svoMacro.h"
#include ".\servo\svo_lib.h"
#include ".\common\system.h"
#include ".\servo\svoVar.h"
#include ".\dvd\dvcfg.h"
#include ".\oem\Oem_spd_tbl.h"
#include ".\dvd\dvDef.h"
#include ".\player\plrdb.h"
#include ".\servo\svo_spdtrans.h"
#include ".\al\lal.h"
#include ".\al\REG_global.h"
#if (EN_WOBBLE == 1)
#include ".\al\REG_afe.h"
#endif //(EN_WOBBLE == 1)
#include ".\al\REG_write_strategy.h"
#include ".\al\hal_afe.h"
#include ".\al\HAL_servo.h"

extern BYTE const Inquiry_tbl[];

/*-----------------------------------------------------------------------------
 *
 * Module: svoAFEInit()
 * Description:
 *
 *----------------------------------------------------------------------------*/
//TBD? COMMENT! Quick fix to remove COEF handling from svoAFEInit
void svoAFEInitOnly(void)
{
#if (AFE_POWER_DOWN == 1)
    AFEPowerDown();
    AFEPowerOn();
#endif // (AFE_POWER_DOWN == 1)
    WRITE_FIELD(HAL_GLOBAL_AFESRST, TRUE);    // Reset AFE block
    Delay100uS(1);    /* Wait 100us */
    WRITE_FIELD(HAL_GLOBAL_AFESRST, FALSE);   // Release reset flag
    Delay100uS(1);    /* Wait 100us */

    LAL_AFE_ClearAFEOffsets();
    LAL_AFE_InitAFEGains();
    svoVar_TrackingMethod = TRK_METHOD_UNKNOWN;

#if (DVD_RAM_READ == 1)
    DVDRAM_embossed   = 0;
    DVDRAM_rewritable = 0;
#endif
} /* svoAFEInit */

/*-----------------------------------------------------------------------------
 *
 * Module: afe_ajust_clr
 * Description: update off line calibrated FEBG, TILT, and FEBC to register
 *
 *----------------------------------------------------------------------------*/
void    afe_ajust_clr(void)
{
#if (AFE_POWER_DOWN == 1)
    AFEPowerOn();
#endif /* AFE_POWER_DOWN */

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if (USE_CD_OFFLINE_FEBG==1)
    #if (EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_FEOBG, svoPcalVar.cdFeoBGValue);
    #else // (EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_FEOBG, LBYTE(svoPcalVar.cdFeBGValue));
    #endif/*(EN_Persistent_table_8 == 1)*/
    #if(EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_SPP2BG, svoPcalVar.cdSPP2BGValue);
    #else // (EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_SPP2BG, HBYTE(svoPcalVar.cdFeBGValue));
    #endif/*(EN_Persistent_table_8 == 1)*/
#endif
#if (USE_CD_OFFLINE_FBDAC==1)
            HAL_SERVO_SetFebc(svoPcalVar.cdFBDacValue);
#endif // (USE_CD_OFFLINE_FBDAC==1)
            break;

        case eDiscDVD:
#if (USE_DV_OFFLINE_FEBG==1)
    #if (EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_FEOBG, svoPcalVar.dvFeoBG0Value);
    #else
            WRITE_FIELD(HAL_AFE_FEOBG, LBYTE(svoPcalVar.dvFeBG0Value));
    #endif/* (EN_Persistent_table_8 == 1) */
    #if (EN_Persistent_table_8 == 1)
            WRITE_FIELD(HAL_AFE_SPP2BG, svoPcalVar.dvSPP2BG0Value);
    #else
            WRITE_FIELD(HAL_AFE_SPP2BG, HBYTE(svoPcalVar.dvFeBG0Value));
    #endif/* (EN_Persistent_table_8 == 1) */
#endif /* (USE_DV_OFFLINE_FEBG==1) */

#if (USE_DV_OFFLINE_TILTDAC==1)
            HAL_SERVO_SetTilt(svoPcalVar.dvTiltDacValue[0]);
            svoPreTiltValue = svoPcalVar.dvTiltDacValue[0];
#endif /* (USE_DV_OFFLINE_TILTDAC==1) */
#if (USE_DV_OFFLINE_FBDAC==1)
            HAL_SERVO_SetFebc(svoPcalVar.dvFBDac0Value);
#endif
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            break;
#endif // (ENABLE_HDDVD == 1)
    }
    calStoreAfeVar();
}

/*-----------------------------------------------------------------------------
 *
 * Module: afeMUXOUT
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if (CHIP_REV < 0xC0)
void AfeMUXOUT(BYTE mux_channel, USHORT mux_field)
{
    switch(mux_channel)
    {
        case AFE_MUXOUT1:
            WRITE_FIELD(HAL_AFE_MUX1SEL, mux_field);
            break;

        case AFE_MUXOUT2:
            WRITE_FIELD(HAL_AFE_MUX2SEL, mux_field);
            break;

        case AFE_MUXOUT3:
            WRITE_FIELD(HAL_AFE_MUX3SEL, mux_field);
            break;

        case AFE_MUXTOAD1:
            WRITE_FIELD(HAL_AFE_MUX1ADSEL, mux_field);
            break;

        case AFE_MUXTOAD2:
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, mux_field);
            break;

        case AFE_MUXTOAD3:
            WRITE_FIELD(HAL_AFE_MUX3ADSEL, mux_field);
            break;
    }
}
#else
//Add mux_field2 for extra MPXO_SEL selection
void AfeMUXOUT(BYTE mux_channel, USHORT mux_field, USHORT mux_field2)
{
    switch (mux_channel)
    {
        case AFE_MUXOUT1:
            WRITE_FIELD(HAL_AFE_MUX1SEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO1SEL, mux_field2);
            }
            break;

        case AFE_MUXOUT2:
            WRITE_FIELD(HAL_AFE_MUX2SEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO2SEL, mux_field2);
            }
            break;

        case AFE_MUXOUT3:
            WRITE_FIELD(HAL_AFE_MUX3SEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO3SEL, mux_field2);
            }
            break;

        case AFE_MUXTOAD1:
            WRITE_FIELD(HAL_AFE_MUX1ADSEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO1SEL, mux_field2);
            }
            break;
            break;

        case AFE_MUXTOAD2:
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO2SEL, mux_field2);
            }
            break;

        case AFE_MUXTOAD3:
            WRITE_FIELD(HAL_AFE_MUX3ADSEL, mux_field);
            if (mux_field == 0)
            {
                WRITE_FIELD(HAL_AFE_MPXO3SEL, mux_field2);
            }
            break;
    }
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: AfeRouteSignalToAD
 * Description: Routes AFEMUXBank and MuxField to AD, returns which ADC_MUXOUT
 *              is used
 *
 *----------------------------------------------------------------------------*/

BYTE AfeRouteSignalToAD(AFEMUXBank_t AFEMUXBank, ADSignal_t MuxField)
{
    switch(AFEMUXBank)
    {
        case AFE_MUXTOAD1:
            WRITE_FIELD(HAL_AFE_MUX1ADSEL, MuxField.mux1);
            return(ADC_MUXOUT1);
            break;
        case AFE_MUXTOAD2:
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, MuxField.mux2);
            return(ADC_MUXOUT2);
            break;
        case AFE_MUXTOAD3:
            WRITE_FIELD(HAL_AFE_MUX3ADSEL, MuxField.mux3);
            #if(CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXOUT3);
            #else
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXTOAD3O);
            #endif
            return(ADC_MUXOUT3);
            break;
        default:
            return(ADC_MUXOUT1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: afeSetCalibMUX
 * Description: Set afe mux for afe calibrations
 *
 *----------------------------------------------------------------------------*/
#define RF_PDATTF_BW_CALIB     0          // TBD: to be put in coef table
#define RF_PDDF_BW_CALIB       0          // TBD: to be put in coef table
#define RF_NOISEF_BW_CALIB     halDFE_NoiseFilterBW0p6  // TBD: to be put in coef table
#define RF_PDATT_TYPE_CALIB    1          // TBD: to be put in coef table
// New changed for seprate MUX monitor and MUXTOAD
BYTE afeSetCalibMUX(BYTE type, BYTE adjust_signal)
{
    BYTE    monitor_signal;

    monitor_signal = ADC_MUXOUT1;
    switch (type)
    {
        case TYPE1:
            switch(adjust_signal)
            {
                case FE_OS:
                    monitor_signal = ADC_FE;
                    break;

                case TE_OS:
                    monitor_signal = ADC_TE;
                    break;
            }
            break;

        case TYPE2:
            switch(adjust_signal)
            {
                case MPP_OS:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_MPPO);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case SPP1_OS:
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SPPO1);
                    monitor_signal = ADC_MUXOUT2;
                    break;

                case FEO_OS:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_FEO);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case SPP2_OS:
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SPPO2);
                    monitor_signal = ADC_MUXOUT2;
                    break;

                case SBAD_OS:
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SBAD);
                    monitor_signal = ADC_MUXOUT2;
                    break;

                case SRF_OS:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case FE2_OS:
                    monitor_signal = ADC_FE;
                    break;

                case TE2_OS:
                    monitor_signal = ADC_TE;
                    break;

                case BS2_OS:
                    monitor_signal = ADC_BS;
                    break;

                case RFCP_OS:
                    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL1);//RF5VP
                    WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                    WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case RFCN_OS:
                    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL2);//RFVGAP/N
                    WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                    WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_RFTP2);
                    monitor_signal = ADC_SPECIAL_PN;
                    break;

                case RADO_OS:
                    WRITE_FIELD(HAL_AFE_WOBTEST, 9);//ADOP/N
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_WOBMUX1);
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_WOBMUX2);
                    monitor_signal = ADC_SPECIAL_PN;
                    break;

                case RBCO_OS:
                    WRITE_FIELD(HAL_AFE_WOBTEST, 8);//BCOP/N
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_WOBMUX1);
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_WOBMUX2);
                    monitor_signal = ADC_SPECIAL_PN;
                    break;

                case MBVREF_OS:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case SBVREF_OS:
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SBAD);
                    monitor_signal = ADC_MUXOUT2;
                    break;

                case BS_OS:
                    monitor_signal = ADC_BS;
                    break;

                default:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_VREFI);
                    monitor_signal = ADC_MUXOUT1;
                    break;
            }
            break;

        case TYPE3:
            switch(adjust_signal)
            {
                case CE_OS:
                    monitor_signal = ADC_CE;
                    break;

                case PHS_OS:
                    monitor_signal = ADC_PHSO;
                    break;

                case BHS_OS:
                    monitor_signal = ADC_BHSO;
                    break;

                case RP_OS:
                    monitor_signal = ADC_RP;
                    break;

                case RP2_OS:
                    monitor_signal = ADC_RP;
                    break;

                case RFP_OS:
                case RFPGA_OS:
                    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_AfeControl);
                    WRITE_FIELD(HAL_DFE_PDATTFBW,RF_PDATTF_BW_CALIB);
                    WRITE_FIELD(HAL_DFE_NOISEFBW,RF_NOISEF_BW_CALIB);
                    WRITE_FIELD(HAL_DFE_PDDFBW, RF_PDDF_BW_CALIB);
                    WRITE_FIELD(HAL_DFE_PDATTTYP, RF_PDATT_TYPE_CALIB);
                    monitor_signal = ADC_SPECIAL_RFDFE;
                    break;

                case DPDCH1_OS:
                case DPDCH2_OS:
                case DPDCH3_OS:
                case DPDCH4_OS:
                    switch (adjust_signal)
                    {
                        case DPDCH1_OS:
                            WRITE_FIELD(HAL_AFE_EQDPDSEL,halAFE_EQDPDSEL1);//DPD1P/N
                            break;
                        case DPDCH2_OS:
                            WRITE_FIELD(HAL_AFE_EQDPDSEL,halAFE_EQDPDSEL2);//DPD2P/N
                            break;
                        case DPDCH3_OS:
                            WRITE_FIELD(HAL_AFE_EQDPDSEL,halAFE_EQDPDSEL3);//DPD3P/N
                            break;
                        case DPDCH4_OS:
                            WRITE_FIELD(HAL_AFE_EQDPDSEL,halAFE_EQDPDSEL0);//DPD4P/N
                            break;
                    }
                    WRITE_FIELD(HAL_AFE_EQDOSEL,halAFE_EQDOSEL7);
                    WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_RFTP2);
                    WRITE_FIELD(HAL_AFE_DPDANATEN, TRUE);
                    monitor_signal = ADC_SPECIAL_PN;
                    break;

#if (CHIP_REV >= 0xC0)
                case TX_OS:
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_TXO);
                    monitor_signal = ADC_MUXOUT1;
                    break;
#endif
            }
            break;

        case TYPE4:
            switch(adjust_signal)
            {
#if (CHIP_REV < 0xC1)
                case DPDGM_OS:
                    WRITE_FIELD(HAL_AFE_EQDOSEL,halAFE_EQDOSEL0);
                    WRITE_FIELD(HAL_AFE_RFTPEN,TRUE);
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_RFTP1);
                    monitor_signal = ADC_MUXOUT1;
                    break;
#endif

                case APC_OS:
                    monitor_signal = ADC_SPECIAL_APC;
                    break;

#if (CHIP_REV >= 0xC0)
                case RFREFP_OS:
                    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL1);//RF5VP
                    WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                    WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
                    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
                    monitor_signal = ADC_MUXOUT1;
                    break;

                case RFREFN_OS:
                    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL1);//RF5VN
                    WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                    WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
                    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_RFTP2);
                    monitor_signal = ADC_MUXOUT2;
                    break;
#endif
            }
            break;
    }

    if (monitor_signal == ADC_MUXOUT3)
    {
    #if(CHIP_REV <= 0xB3)
        WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXOUT3);
    #else
        WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXTOAD3O);
    #endif
    }
    return(monitor_signal);
}

/*-----------------------------------------------------------------------------
 *
 * Module: AfeSetMUXTOAD
 * Description: Set afe MUXTOAD channel
 *
 *----------------------------------------------------------------------------*/
void AfeSetMUXTOAD(BYTE select_channel)
{
    switch(select_channel)
    {
    #if(CHIP_REV <= 0xB3)
        case halAFE_MUXADSEL_PH:
    #else
        case halAFE_MUXADSEL_BETAPH:
    #endif
            WRITE_FIELD(HAL_AFE_OPCCLKOFF, 0); //enable Droop Rate Control Clock for Short Term P/H
            WRITE_FIELD(HAL_AFE_OPCDISCLK, 0x0F); //decrease Short Term P/H Droop Rate
            WRITE_FIELD(HAL_AFE_OPCCSEL, 0); //select OPC P/H and B/H Capacitor
            WRITE_FIELD(HAL_AFE_OPCOUT, 0); //disables the output to BETAPH and BETABH
            WRITE_FIELD(HAL_AFE_OPCDIS, 0); //increase OPC P/H and B/H Droop Rate
     #if(CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_PH);
     #else
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_BETAPH);
     #endif
            break;

     #if(CHIP_REV <= 0xB3)
        case halAFE_MUXADSEL_MUXOUT3:
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXOUT3);
     #else
        case halAFE_MUXADSEL_MUXTOAD3O:
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXTOAD3O);
     #endif
            break;

     #if(CHIP_REV <= 0xB3)
        case halAFE_MUXADSEL_BH:
     #else
        case halAFE_MUXADSEL_BETABH:
     #endif
            WRITE_FIELD(HAL_AFE_OPCCLKOFF, 0); //enable Droop Rate Control Clock for Short Term P/H
            WRITE_FIELD(HAL_AFE_OPCDISCLK, 0x0F); //decrease Short Term P/H Droop Rate
            WRITE_FIELD(HAL_AFE_OPCCSEL, 0); //select OPC P/H and B/H Capacitor
            WRITE_FIELD(HAL_AFE_OPCOUT, 0); //disables the output to BETAPH and BETABH
            WRITE_FIELD(HAL_AFE_OPCDIS, 0); //increase OPC P/H and B/H Droop Rate
      #if(CHIP_REV <= 0xB3)
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_BH);
      #else
            WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_BETABH);
      #endif
            break;
    }
}


/*-----------------------------------------------------------------------------
 *
 * Module: afeSetTrackingMethod()
 * Description: Change DVD tracking method
 *
 *----------------------------------------------------------------------------*/
void afeSetTrackingMethod(ETrackingMethod tracking_method)
{
    if(svoVar_TrackingMethod == tracking_method)
        return;

    switch (tracking_method)
    {
        case TRK_METHOD_MPP:
#if (CHIP_REV >= 0xC1)
            WRITE_FIELD(HAL_AFE_DPDRISEN, 0);           // Disable rising edge of PD.
            WRITE_FIELD(HAL_AFE_DPDFALEN, 0);           // Disable falling edge of PD.
#else
            WRITE_FIELD(HAL_AFE_DPDCPHLD, TRUE);//985[7]=1,Hold DVD Charge Pump
#endif
            WRITE_FIELD(HAL_AFE_PDDPD, TRUE);   //970[5]=1,Power down the whole DPD block
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, 0);
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TEDPD_MASK|HAL_AFE_TESP1_MASK, HAL_AFE_TEMPP);
            SendMsg80(STATE_MACHINE_FLOW_MSG,0x300030);
            break;

        case TRK_METHOD_DPD:
#if (CHIP_REV >= 0xC1)
            WRITE_FIELD(HAL_AFE_DPDRISEN, 1);           // Enable rising edge of PD.
            WRITE_FIELD(HAL_AFE_DPDFALEN, 1);           // Enable falling edge of PD.
#else
            WRITE_FIELD(HAL_AFE_DPDCPHLD, FALSE);//985[7]=0,Not hold DVD Charge Pump
#endif
            WRITE_FIELD(HAL_AFE_PDDPD, FALSE);   //970[5]=0,Not Power down the whole DPD block
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, HAL_AFE_DPD12EN|HAL_AFE_DPD34EN);
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TEDPD_MASK|HAL_AFE_TESP1_MASK, HAL_AFE_TEDPD);
#if (AFE_NORMALIZER == 1)
            WRITE_FIELD(HAL_AFE_TE2AUTOEN, 0);
#endif
            HAL_AFE_SetDPDBw((svoSpeedVar.SpdOverspd + 5) / 10, COEF_GetValue( AFE_CALC_BwDpdRatioX100));
            SendMsg80(STATE_MACHINE_FLOW_MSG,0x300031);
            break;

        case TRK_METHOD_DPP:
#if (CHIP_REV >= 0xC1)
            WRITE_FIELD(HAL_AFE_DPDRISEN, 0);           // Disable rising edge of PD.
            WRITE_FIELD(HAL_AFE_DPDFALEN, 0);           // Disable falling edge of PD.
#else
            WRITE_FIELD(HAL_AFE_DPDCPHLD, TRUE);//985[7]=1,Hold DVD Charge Pump
#endif
            WRITE_FIELD(HAL_AFE_PDDPD, TRUE);   //970[5]=1,Power down the whole DPD block
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, 0);
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TEDPD_MASK|HAL_AFE_TESP1_MASK, HAL_AFE_TEMPP|HAL_AFE_TESP1);
#if (APP_TE_SUPPORT == 1)
            if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
                ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)) &&
                ((READ_REG(HAL_AFEREG_MAP->SB14) & (HAL_AFE_SPP1E|HAL_AFE_SPP1F|HAL_AFE_SPP1G|HAL_AFE_SPP1H)) == (HAL_AFE_SPP1E|HAL_AFE_SPP1F)))
            {
                //MPPO = km*(H-am*G), SPPO1 = ks*(F-as*E)
                //TE = TEINV(K*m*(a1*SPPO1-MPPO))= K*m*km*[(H-am*G) - (a1*ks/km)*(F-as*E)]
                SendMsg80(STATE_MACHINE_FLOW_MSG,0x300034); // TrackingMethod: APP
            }
            else
#endif
            {
                //MPPO = km*((A+D)-am*(B+C)), SPPO1 = ks*((F+H)-as*(E+G))
                //TE = TEINV(K*m*(a1*SPPO1-MPPO))= K*m*km*[((A+D)-am*(B+C)) - (a1*ks/km)*((F+H)-as*(E+G))]
                SendMsg80(STATE_MACHINE_FLOW_MSG,0x300032); // TrackingMethod: DPP
            }
            break;

        case TRK_METHOD_SPPO1:
#if (CHIP_REV >= 0xC1)
            WRITE_FIELD(HAL_AFE_DPDRISEN, 0);           // Disable rising edge of PD.
            WRITE_FIELD(HAL_AFE_DPDFALEN, 0);           // Disable falling edge of PD.
#else
            WRITE_FIELD(HAL_AFE_DPDCPHLD, TRUE);//985[7]=1,Hold DVD Charge Pump
#endif
            WRITE_FIELD(HAL_AFE_PDDPD, TRUE);   //970[5]=1,Power down the whole DPD block
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, 0);
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, HAL_AFE_TEMPP_MASK|HAL_AFE_TEDPD_MASK|HAL_AFE_TESP1_MASK, HAL_AFE_TESP1);
            SendMsg80(STATE_MACHINE_FLOW_MSG,0x300033);
            break;
    }
    svoVar_TrackingMethod = tracking_method;

#if (DVD_RAM_READ == 1)
    DVDRAM_embossed   = 0;
    DVDRAM_rewritable = 0;
    if (DVDRAM)
    {
        if (svoVar_TrackingMethod == TRK_METHOD_DPP)
            DVDRAM_rewritable = 1;
        else
            DVDRAM_embossed   = 1;
    }
#endif
}

#if (WOBBLE_READ_ON_DVD_OR_CD == 1) && (EN_WOBBLE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: afeSetWobbleForASpeed()
 * Description: According different speed to set reference wobble registers
 * Input:       speed
 *----------------------------------------------------------------------------*/
void afeSetWobbleForASpeed(ESpindleSpeed sp)
{
    USHORT OverspeedX10;
    SendMsgCn(DEBUG_WOBBLE_FLOW_MSG|DEBUG_CD_WRITE_MSG,0,0x39000B);          //afeSetWobbleForASpeed()
    SendMsgCn(DEBUG_WOBBLE_FLOW_MSG|DEBUG_CD_WRITE_MSG,1,0x39010E,A1B(sp));  //afeSetWobbleForASpeed().Speed

    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    { // Recordable CD
        DEBUG_ASSERT (sp <= MAX_SPEED_CD, eASSERT_IMPOSSIBLE_SPINDLE_SPEED);
        // Determine Overspeed
        OverspeedX10 = SpeedIndexTableCD[sp] / 10;
        SendMsgCn(DEBUG_WOBBLE_FLOW_MSG|DEBUG_CD_WRITE_MSG,1,0x39010E,A1B(sp));
        LAL_StoreParams( LAL_DetectDiscKind(svoVar_discKindT), sp );
                                                    // but also a COEF_StoreParams is required to retrieve the coefs,
                                                    // so therefor LAL_StoreParams was called.
        LAL_SetReadMode();
    }
    else
    #endif // (WOBBLE_READ_ON_DVD_OR_CD == 1)
    {
        // moved to svo_speed.c USHORT clvn_temp;
        switch(svoVar_discKindT.Bit.DiscStd)
        {
    #if (BD_ENABLE == 1)
            case eDiscBD:
                DEBUG_ASSERT (sp <= MAX_SPEED_BD, eASSERT_IMPOSSIBLE_SPINDLE_SPEED);
                OverspeedX10 = SpeedIndexTableBD[sp] / 10;
                break;
    #endif

            case eDiscDVD:
                DEBUG_ASSERT (sp <= MAX_SPEED_DVD, eASSERT_IMPOSSIBLE_SPINDLE_SPEED);
    #if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    OverspeedX10 = SpeedIndexTableDVDRAM[sp] / 10;
                    break;
                }
                else
    #endif
                {
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        #if (DVD_710Run815New == 1)
                    OverspeedX10 = GetSpeedIndexTableDVD(sp) / 10;
        #else
                    OverspeedX10 = SpeedIndexTableDVD[sp] / 10;
        #endif
                    break;
    #else
                    return;
    #endif
                }

    #if (ENABLE_HDDVD == 1)
            case eDiscHD:
                DEBUG_ASSERT (sp <= MAX_SPEED_HD, eASSERT_IMPOSSIBLE_SPINDLE_SPEED);
                OverspeedX10 = SpeedIndexTableHDDVD[sp] / 10;
                break;
    #endif

            default:
                return;
        }
        SendMsgCn(DEBUG_DVD_WRITE_MSG|DEBUG_WOBBLE_FLOW_MSG|DEBUG_CD_WRITE_MSG,1,0x39010E,A1B(sp)); //afeSetWobbleForASpeed().Speed
        SendMsgCn(DEBUG_WOBBLE_FLOW_MSG|DEBUG_CD_WRITE_MSG,1,0x39010E,A1B(sp)); //afeSetWobbleForASpeed().Speed
        LAL_StoreParams( LAL_DetectDiscKind(svoVar_discKindT), sp );
                                                                                // but also a COEF_StoreParams is required to retrieve the coefs,
                                                                                // so therefor LAL_StoreParams was called.
        LAL_SetReadMode();
    }
}
#endif

/************************************************************************
* Module: CalcNewServoGain()
* Description: Calculate new Servo Gain for Write
* Returns:
* Params:
*************************************************************************/
SBYTE CalcNewServoGain(float pwr)
{
    return (0);
}

/*-----------------------------------------------------------------------------
 *
 * Module:       afeSetSvognMode()
 * Description:
 *               Switches SVOGN to defined mode.
 * Input:
 *               Write gain mode
 * Output:
 *               void
  *----------------------------------------------------------------------------*/
void afeSetSvognMode(BYTE WrGainMode)
{

}



/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
