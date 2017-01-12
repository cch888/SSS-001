/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 409 $
* $Date: 11/03/11 6:03p $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\common\Globals.h"
#include ".\common\Globtype.H"
#include ".\common\chip.h"

#include ".\al\hal_lasc.h"
#include ".\al\hal_dsp.h"
#include ".\al\coefs.h"

#include ".\common\stdlib.h"
#include ".\common\macros.h"
#include ".\servo\svo.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svoMacro.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\player\plrdb.h"
#include ".\hif\gp_ram.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_sta.h"
#include ".\servo\afe_tbl.h"
#include ".\servo\svo_sta.h"
#include ".\hif\Diag.h"
#include ".\servo\svoPclib.h"
#include ".\oem\oem_tbl.h"
#include ".\oem\oem_info.h"
#include ".\servo\svo_lib.h"

#if  0
typedef enum _EVrPowerSel
{
    VRPOWER_SEL_GAIN1_OFFSET  = 0x00,
    VRPOWER_SEL_GAIN5_OFFSET  = 0x04,
    VRPOWER_SEL_GAIN10_OFFSET = 0x08,
    VRPOWER_SEL_GAIN25_OFFSET = 0x0C,
} EVrPowerSel;

typedef enum _EVWPowerSel
{
    VWPOWER_SEL_LOWGAIN_OFFSET  = 0x00,
    VWPOWER_SEL_MIDGAIN_OFFSET  = 0x08,
    VWPOWER_SEL_HIGHGAIN_OFFSET = 0x10,
} EVWPowerSel;

//#if PersistentTable_Revision>=8
//static BYTE apc_rwopen_mode, apc_rw_gain;//rwopen: read, write or open loop mode
//extern BYTE apc_rwopen_mode, apc_rw_gain;//rwopen: read, write or open loop mode
//#endif

void fetch_lasc_from_persistent(USHORT DiscGroupOffset)
{
    EVrPowerSel persistent_entry;
    USHORT fdgd2_value;

    // Offset type = float in persistent table
    COEF_SetValue( SRV_CALC_C_offset_DAC_threshold_ON, (long)GetPersistentTable4F(DiscGroupOffset + IDX_OFFSET_DAC_THRESHOLD_ON) );
    COEF_SetValue( SRV_CALC_C_offset_DAC_threshold, (long)GetPersistentTable4F(DiscGroupOffset + IDX_OFFSET_DAC_THRESHOLD) ); //should we rename coef with scale factor ?
    COEF_SetValue( SRV_CALC_C_offset_DAC_0, (long)GetPersistentTable4F(DiscGroupOffset + IDX_OFFSET_DAC_0) );
    COEF_SetValue( SRV_CALC_C_offset_DAC_1, (long)GetPersistentTable4F(DiscGroupOffset + IDX_OFFSET_DAC_1) );
    COEF_SetValue( SRV_CALC_C_offset_DAC_2, (long)GetPersistentTable4F(DiscGroupOffset + IDX_OFFSET_DAC_2) );
    // slope type = float in persistent table
    COEF_SetValue( SRV_CALC_C_slope_DAC_threshold_ON, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset+ IDX_SLOPE_DAC_THRESHOLD_ON) ) );
    COEF_SetValue( SRV_CALC_C_slope_DAC_threshold, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset+ IDX_SLOPE_DAC_THRESHOLD) ) );
    COEF_SetValue( SRV_CALC_C_slope_DAC_0, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset+ IDX_SLOPE_DAC_0) ) );
    COEF_SetValue( SRV_CALC_C_slope_DAC_1, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset+ IDX_SLOPE_DAC_1) ) );
    COEF_SetValue( SRV_CALC_C_slope_DAC_2, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset+ IDX_SLOPE_DAC_2) ) );
#if 0 /* DEBUG CODE - KEEP */
    #if 0
    //send_msg5S(1,"Offset Dac");
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>24,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>16,
    //            COEF_GetValue(SRV_CALC_C_offset_DAC_threshold)>>8,COEF_GetValue(SRV_CALC_C_offset_DAC_threshold));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_offset_DAC_0)>>24,COEF_GetValue(SRV_CALC_C_offset_DAC_0)>>16,
    //            COEF_GetValue(SRV_CALC_C_offset_DAC_0)>>8,COEF_GetValue(SRV_CALC_C_offset_DAC_0));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_offset_DAC_1)>>24,COEF_GetValue(SRV_CALC_C_offset_DAC_1)>>16,
    //            COEF_GetValue(SRV_CALC_C_offset_DAC_1)>>8,COEF_GetValue(SRV_CALC_C_offset_DAC_1));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_offset_DAC_2)>>24,COEF_GetValue(SRV_CALC_C_offset_DAC_2)>>16,
    //            COEF_GetValue(SRV_CALC_C_offset_DAC_2)>>8,COEF_GetValue(SRV_CALC_C_offset_DAC_2));
    //send_msg5S(1,"Slope");
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>24,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>16,
    //            COEF_GetValue(SRV_CALC_C_slope_DAC_threshold)>>8,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_DAC_0)>>24,COEF_GetValue(SRV_CALC_C_slope_DAC_0)>>16,
    //            COEF_GetValue(SRV_CALC_C_slope_DAC_0)>>8,COEF_GetValue(SRV_CALC_C_slope_DAC_0));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_DAC_1)>>24,COEF_GetValue(SRV_CALC_C_slope_DAC_1)>>16,
    //            COEF_GetValue(SRV_CALC_C_slope_DAC_1)>>8,COEF_GetValue(SRV_CALC_C_slope_DAC_1));
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_DAC_2)>>24,COEF_GetValue(SRV_CALC_C_slope_DAC_2)>>16,
    //            COEF_GetValue(SRV_CALC_C_slope_DAC_2)>>8,COEF_GetValue(SRV_CALC_C_slope_DAC_2));
    #endif

    #if 1
    send_msg5S(1,"Offset Dac");
    send_msgA4(DEBUG_KPOWER_MSG,0x5350DF,(long)COEF_GetValue(SRV_CALC_C_offset_DAC_threshold_ON));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350D0,(long)COEF_GetValue(SRV_CALC_C_offset_DAC_threshold));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350C6,(long)COEF_GetValue(SRV_CALC_C_offset_DAC_0));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350C8,(long)COEF_GetValue(SRV_CALC_C_offset_DAC_1));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CA,(long)COEF_GetValue(SRV_CALC_C_offset_DAC_2));

    send_msg5S(1,"Slope");
    send_msgA4(DEBUG_KPOWER_MSG,0x5350E0,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold_ON));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350D1,COEF_GetValue(SRV_CALC_C_slope_DAC_threshold));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350C7,COEF_GetValue(SRV_CALC_C_slope_DAC_0));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350C9,COEF_GetValue(SRV_CALC_C_slope_DAC_1));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CB,COEF_GetValue(SRV_CALC_C_slope_DAC_2));
    #endif
#endif /* DEBUG CODE - KEEP */

    // Find out what VRPOWER gain is in use at this moment the AFE (coef), then fetch the
    // corresponding gain value out of the persisten tables (Depending on DiscGroup) and
    // put this gain into the coef-table, at the specific disckind places
    // Simplified Read Gain truth-table
    if ( COEF_GetValue(AFE_WOBAPC02_ERASE1) ) //AND-ing of other fields = don't care (simplified)
    {
        persistent_entry = VRPOWER_SEL_GAIN1_OFFSET;
        send_msg5S(SHOW_DEBUG_MSG,"Read Gain 1X");
    }
    else if ( COEF_GetValue(AFE_WOBAPC02_RDAMPCD) )
    {
        persistent_entry = VRPOWER_SEL_GAIN5_OFFSET;
        send_msg5S(SHOW_DEBUG_MSG,"Read Gain 5X");
    }
    else if ( COEF_GetValue(AFE_WOBAPC02_GAIN10) )
    {
        persistent_entry = VRPOWER_SEL_GAIN10_OFFSET;
        send_msg5S(SHOW_DEBUG_MSG,"Read Gain 10X");
    }
    else
    {
        persistent_entry = VRPOWER_SEL_GAIN25_OFFSET;
        send_msg5S(SHOW_DEBUG_MSG,"Read Gain 25X");
    }

    // slope type = float in persistent table
    COEF_SetValue( SRV_CALC_C_slope_ADC_VRpower, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset + IDX_SLOPE_ADC_VRPOWER_G1X + (BYTE) persistent_entry) ) );
#if ((PCB == 673) || (PCB == 830))
    COEF_SetValue( SRV_CALC_C_offset_ADC_VRpower, (long)(GetPersistentTable4F(DiscGroupOffset + IDX_SLOPE_ADC_VRPOWER_G1X + (BYTE) persistent_entry - IDX_DIFF_ADC_VRPOWER_G1X) ) );
#endif

#if 0 /* DEBUG CODE - KEEP */
    #if 0
    //send_msg5S(1,"ADC Slope");
    //send_msg54(1,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>24,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>16,
    //            COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower)>>8,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower));
    #endif
    #if 1
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CC,COEF_GetValue(SRV_CALC_C_slope_ADC_VRpower));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CD,(long)COEF_GetValue(SRV_CALC_C_offset_ADC_VRpower));
    #endif
#endif /* DEBUG CODE - KEEP */

    fdgd2_value = READ_FIELD(HAL_AFE_FDGD2);
    if (fdgd2_value == VWPOW_LOWGAIN)
    {
        persistent_entry = VWPOWER_SEL_LOWGAIN_OFFSET;
        send_msg80(DEBUG_KPOWER_MSG,0x5350F6); //Write Gain Low
    }
    else if (fdgd2_value == VWPOW_MIDGAIN)
    {
        persistent_entry = VWPOWER_SEL_MIDGAIN_OFFSET;
        send_msg80(DEBUG_KPOWER_MSG,0x5350F7); //Write Gain Mid
    }
    else if (fdgd2_value == VWPOW_HIGHGAIN)
    {
        persistent_entry = VWPOWER_SEL_HIGHGAIN_OFFSET;
        send_msg80(DEBUG_KPOWER_MSG,0x5350F8); //Write Gain High
    }
    else
    {
        send_msg80(DEBUG_KPOWER_MSG,0x5350F9); //ILLEGAL WRITE GAIN !
    }
    COEF_SetValue( SRV_CALC_C_slope_ADC_VWpower, (ULONG)(LASC_SLOPE_SCALE * (float)GetPersistentTable4F(DiscGroupOffset + IDX_SLOPE_ADC_VWPOWER_LG + (BYTE) persistent_entry) ) );
#if ((PCB == 673) || (PCB == 830))
    COEF_SetValue( SRV_CALC_C_offset_ADC_VWpower, (long)(GetPersistentTable4F(DiscGroupOffset + IDX_SLOPE_ADC_VWPOWER_LG + (BYTE) persistent_entry - IDX_DIFF_ADC_VWPOWER_LG) ) );
#endif

#if 0 /* DEBUG CODE - KEEP */
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CE,COEF_GetValue(SRV_CALC_C_slope_ADC_VWpower));
    send_msgA4(DEBUG_KPOWER_MSG,0x5350CF,(long)COEF_GetValue(SRV_CALC_C_offset_ADC_VWpower));
#endif /* DEBUG CODE - KEEP */

}
#endif


/************************************************************************
* Module: Cal_LoadOfflineCalibrationParam()
* Description:
* Returns:
* Params:
*************************************************************************/
extern float SerialRead4F(USHORT addr);
#if (OPU == KEM350AAA)
static void printEEPROMcontent(void)
{
    USHORT i;
    TOTAL_EEPROM_t eeprom;

    for (i = 0; i < sizeof(TOTAL_EEPROM_t); i++) eeprom.b[i] = ReadOPU(ADDFACINFO + i);

    send_msg5SValue(1, "lot.info1.b        ", 1, eeprom.s.OPIDData.lot.info1.b);
    send_msg5SValue(1, "lot.info2.b        ", 1, eeprom.s.OPIDData.lot.info2.b);
    send_msg5SValue(1, "lot.info3.b        ", 1, eeprom.s.OPIDData.lot.info3.b);
    send_msg5SValue(1, "serial.serialNo_1  ", 1, eeprom.s.OPIDData.serial.serialNo_1);
    send_msg5SValue(1, "serial.serialNo_2  ", 1, eeprom.s.OPIDData.serial.serialNo_2);
    send_msg5SValue(1, "serial.serialNo_3  ", 1, eeprom.s.OPIDData.serial.serialNo_3);
    send_msg5SValue(1, "SAActBDL0L1        ", 1, eeprom.s.SAActuatorData.SAActBDL0L1);
    send_msg5SValue(1, "SAActCD            ", 1, eeprom.s.SAActuatorData.SAActCD);
    send_msg5SValue(1, "SAActDVD           ", 1, eeprom.s.SAActuatorData.SAActDVD);
    send_msg5SValue(1, "SAActBD            ", 1, eeprom.s.SAActuatorData.SAActBD);
    send_msg5SValue(1, "threeAxisActTiltDVD", 1, eeprom.s.threeAxisData.threeAxisActTiltDVD);
    send_msg5SValue(1, "threeAxisActTiltBD ", 1, eeprom.s.threeAxisData.threeAxisActTiltBD);
    send_msg5SValue(1, "fpdCD              ", 1, eeprom.s.frontPDData.fpdCD);
    send_msg5SValue(1, "fpdDVD             ", 1, eeprom.s.frontPDData.fpdDVD);
    send_msg5SValue(1, "fpdBD              ", 1, eeprom.s.frontPDData.fpdBD);
    send_msg5SValue(1, "LCDRADCD           ", 1, eeprom.s.LCDData.LCDRADCD);
    send_msg5SValue(1, "LCDTANCD           ", 1, eeprom.s.LCDData.LCDTANCD);
    send_msg5SValue(1, "LCDRADDVD          ", 1, eeprom.s.LCDData.LCDRADDVD);
    send_msg5SValue(1, "LCDTANDVD          ", 1, eeprom.s.LCDData.LCDTANDVD);
    send_msg5SValue(1, "LCDAS00DVD         ", 1, eeprom.s.LCDData.LCDAS00DVD);
    send_msg5SValue(1, "LCDAS45DVD         ", 1, eeprom.s.LCDData.LCDAS45DVD);
    send_msg5SValue(1, "RADSKEWBD          ", 1, eeprom.s.BDSkewData.RADSKEWBD);
    send_msg5SValue(1, "TANSKEWBD          ", 1, eeprom.s.BDSkewData.TANSKEWBD);
    send_msg5SValue(1, "HFMFCD             ", 1, eeprom.s.HFMSetting.HFMFCD);
    send_msg5SValue(1, "HFMFDVD            ", 1, eeprom.s.HFMSetting.HFMFDVD);
    send_msg5SValue(1, "HFMFBD             ", 1, eeprom.s.HFMSetting.HFMFBD);
    send_msg5SValue(1, "HFMPCD             ", 1, eeprom.s.HFMSetting.HFMPCD);
    send_msg5SValue(1, "HFMPDVD            ", 1, eeprom.s.HFMSetting.HFMPDVD);
    send_msg5SValue(1, "HFMPBDSLL          ", 1, eeprom.s.HFMSetting.HFMPBDSLL);
    send_msg5SValue(1, "HFMPBDSLH          ", 1, eeprom.s.HFMSetting.HFMPBDSLH);

#if (DUMP_ALL_OPU_REGISTER == 1) //Dump all opu's register for debug (don't delete it)
    for (i = 0; i < 32 ; i++)
    {
        send_msg84(1,0x090909,ReadOPU(i*4),ReadOPU(i*4+1),ReadOPU(i*4+2),ReadOPU(i*4+3));
    }

    for (i = 0; i < 2 ; i++)
    {
        send_msg84(1,0x0A0A0A,ReadOPU(i*4+0xF8),ReadOPU(i*4+0xF9),ReadOPU(i*4+0xFA),ReadOPU(i*4+0xFB));
    }
#endif

    return;
}
#endif //(OPU == KEM350AAA)

#if(OPU == DXX811)
static void DXX811_barcode(void)    //HD
{
    int     i;
#if 0 //temp set for Dxx811 811a59482676FE000000c8640200ff001b
    OPUBarcode.SerialNum[0]= 0x81;
    OPUBarcode.SerialNum[1]= 0x1a;
    OPUBarcode.SerialNum[2]= 0x59;
    OPUBarcode.SerialNum[3]= 0x48;
    OPUBarcode.SerialNum[4]= 0x26;
    OPUBarcode.ProductionDay = 0x76FE>>4;
    OPUBarcode.TesterNumber = 0x76FE&0x000F;
    OPUBarcode.LC_UPPER_ELEMENT = 0xc8;
    OPUBarcode.LC_LOWER_ELEMENT = 0x64;
    OPUBarcode.LC_SA = 0x02;
    OPUBarcode.LC_ASO = 0x00;
    OPUBarcode.LC_AS45 = 0xFF;
    OPUBarcode.LC_COMA = 0x00;
    OPUBarcode.THETAT = 0x1b;
#else // 0
    for(i=0;i<SERIAL_NUMBER_LEN;i++)
    OPUBarcode.SerialNum[i]= GetPersistentTable1B(SERIAL_NUMBER_ADDR+i);
    OPUBarcode.ProductionDay = GetPersistentTable2B(PRODECTION_DATE_ADDR)>>4;
    OPUBarcode.TesterNumber = GetPersistentTable2B(PRODECTION_DATE_ADDR)&0x000F;
    OPUBarcode.LC_UPPER_ELEMENT = GetPersistentTable1B(LC_UPPER_ELEMENT_ADDR);
    OPUBarcode.LC_LOWER_ELEMENT = GetPersistentTable1B(LC_LOWER_ELEMENT_ADDR);
    OPUBarcode.LC_SA = GetPersistentTable1B(LC_SA_ADDR);
    OPUBarcode.LC_ASO = GetPersistentTable1B(LC_ASO_ADDR);
    OPUBarcode.LC_AS45 = GetPersistentTable1B(LC_AS45_ADDR);
    OPUBarcode.LC_COMA = GetPersistentTable1B(LC_COMA_DDR);
    OPUBarcode.THETAT = GetPersistentTable1B(THETAT_ADDR);
#endif // 0
}
#endif
#if(OPU == KES410A)
static void KES410A_barcode(void)   //SONY BD player
{
    if ((svoPcalVar.CalDoneBits & bd_barcode_end)==bd_barcode_end)
    {
        OPUBarcode.serial_0L = (GetPersistentTable1B(SERIAL_0_ADR)<<8)|GetPersistentTable1B(SERIAL_0_ADR+1);
        OPUBarcode.serial_0H = (GetPersistentTable1B(SERIAL_0_ADR+2)<<8)|GetPersistentTable1B(SERIAL_0_ADR+3);
        OPUBarcode.serial_1L = (GetPersistentTable1B(SERIAL_1_ADR)<<8)|GetPersistentTable1B(SERIAL_1_ADR+1);
        OPUBarcode.serial_1H = (GetPersistentTable1B(SERIAL_1_ADR+2)<<8)|GetPersistentTable1B(SERIAL_1_ADR+3);
        OPUBarcode.RAD_SKEW_DVD = GetPersistentTable1B(RAD_SKEW_DVD_ADR);
        OPUBarcode.TAN_SKEW_DVD = GetPersistentTable1B(TAN_SKEW_DVD_ADR);
        OPUBarcode.READPOWER_CD = GetPersistentTable1B(READPOWER_CD_ADR);
        OPUBarcode.READPOWER_DVD = GetPersistentTable1B(READPOWER_DVD_ADR);
        OPUBarcode.READPOWER_BDL0 = GetPersistentTable1B(READPOWER_BDL0_ADR);
        OPUBarcode.READPOWER_BDL1 = GetPersistentTable1B(READPOWER_BDL1_ADR);
        OPUBarcode.HFMF_BD = GetPersistentTable1B(HFMF_BD_ADR);
        OPUBarcode.HFMF_SS_BD = GetPersistentTable1B(HFMF_SS_BD_ADR);
        OPUBarcode.TILT_SENS_CD = GetPersistentTable1B(TILT_SENS_CD_ADR);
        OPUBarcode.TILT_SENS_DVD = GetPersistentTable1B(TILT_SENS_DVD_ADR);
        OPUBarcode.TILT_SENS_BD = GetPersistentTable1B(TILT_SENS_BD_ADR);
        OPUBarcode.HFMP_BD0SL = GetPersistentTable1B(HFMP_BD0SL_ADR);
        OPUBarcode.HFMP_BD1DL = GetPersistentTable1B(HFMP_BD1DL_ADR);
        OPUBarcode.FFCG_FFC_CD = GetPersistentTable1B(FFCG_FFC_CD_ADR);
        OPUBarcode.FFCG_FFC_DVD = GetPersistentTable1B(FFCG_FFC_DVD_ADR);
        OPUBarcode.FFCG_FFC_BD = GetPersistentTable1B(FFCG_FFC_BD_ADR);
        OPUBarcode.IVG_CDDVD = GetPersistentTable1B(IVG_CDDVD_ADR);
        OPUBarcode.IVG_BD = GetPersistentTable1B(IVG_BD_ADR);
        OPUBarcode.RAD_SKEW_BD = GetPersistentTable1B(RAD_SKEW_BD_ADR);
        OPUBarcode.RAD_SKEW_CD = GetPersistentTable1B(RAD_SKEW_CD_ADR);
        OPUBarcode.SA_ACT_BD_L0 = (GetPersistentTable1B(SA_ACT_BD_L0_ADR)<<8)|GetPersistentTable1B(SA_ACT_BD_L0_ADR+1);
        OPUBarcode.SA_ACT_BD_L1 = (GetPersistentTable1B(SA_ACT_BD_L1_ADR)<<8)|GetPersistentTable1B(SA_ACT_BD_L1_ADR+1);
        OPUBarcode.SONY_410_CHECK_SUM = (GetPersistentTable1B(SONY_410_CHECK_SUM_ADR)<<8)|GetPersistentTable1B(SONY_410_CHECK_SUM_ADR+1);
    }
    else
    {//Error in persistent table
        send_msg5S(SHOW_DEBUG_MSG,"BarcodeChkSunFail!");
    }
    send_msg5SValue(1, "Barcode.serial_0L = ", 2, (ULONG)OPUBarcode.serial_0L);
    send_msg5SValue(1, "Barcode.serial_0H = ", 2, (ULONG)OPUBarcode.serial_0H);
    send_msg5SValue(1, "Barcode.serial_1L = ", 2, (ULONG)OPUBarcode.serial_1L);
    send_msg5SValue(1, "Barcode.serial_1H = ", 2, (ULONG)OPUBarcode.serial_1H);
    send_msg5SValue(1, "Barcode.RAD_SKEW_DVD = ", 1, (ULONG)OPUBarcode.RAD_SKEW_DVD);
    send_msg5SValue(1, "Barcode.TAN_SKEW_DVD = ", 1, (ULONG)OPUBarcode.TAN_SKEW_DVD);
    send_msg5SValue(1, "Barcode.READPOWER_CD = ", 1, (ULONG)OPUBarcode.READPOWER_CD);
    send_msg5SValue(1, "Barcode.READPOWER_DVD = ", 1, (ULONG)OPUBarcode.READPOWER_DVD);
    send_msg5SValue(1, "Barcode.READPOWER_BDL0 = ", 1, (ULONG)OPUBarcode.READPOWER_BDL0);
    send_msg5SValue(1, "Barcode.READPOWER_BDL1 = ", 1, (ULONG)OPUBarcode.READPOWER_BDL1);
    send_msg5SValue(1, "Barcode.HFMF_BD = ", 1, (ULONG)OPUBarcode.HFMF_BD);
    send_msg5SValue(1, "Barcode.HFMF_SS_BD = ", 1, (ULONG)OPUBarcode.HFMF_SS_BD);
    send_msg5SValue(1, "Barcode.TILT_SENS_CD = ", 1, (ULONG)OPUBarcode.TILT_SENS_CD);
    send_msg5SValue(1, "Barcode.TILT_SENS_DVD = ", 1, (ULONG)OPUBarcode.TILT_SENS_DVD);
    send_msg5SValue(1, "Barcode.TILT_SENS_BD = ", 1, (ULONG)OPUBarcode.TILT_SENS_BD);
    send_msg5SValue(1, "Barcode.HFMP_BD0SL = ", 1, (ULONG)OPUBarcode.HFMP_BD0SL);
    send_msg5SValue(1, "Barcode.HFMP_BD1DL = ", 1, (ULONG)OPUBarcode.HFMP_BD1DL);
    send_msg5SValue(1, "Barcode.FFCG_FFC_CD = ", 1, (ULONG)OPUBarcode.FFCG_FFC_CD);
    send_msg5SValue(1, "Barcode.FFCG_FFC_DVD = ", 1, (ULONG)OPUBarcode.FFCG_FFC_DVD);
    send_msg5SValue(1, "Barcode.FFCG_FFC_BD = ", 1, (ULONG)OPUBarcode.FFCG_FFC_BD);
    send_msg5SValue(1, "Barcode.IVG_CDDVD = ", 1, (ULONG)OPUBarcode.IVG_CDDVD);
    send_msg5SValue(1, "Barcode.IVG_BD = ", 1, (ULONG)OPUBarcode.IVG_BD);
    send_msg5SValue(1, "Barcode.RAD_SKEW_BD = ", 1, (ULONG)OPUBarcode.RAD_SKEW_BD);
    send_msg5SValue(1, "Barcode.RAD_SKEW_CD = ", 1, (ULONG)OPUBarcode.RAD_SKEW_CD);
    send_msg5SValue(1, "Barcode.SA_ACT_BD_L0 = ", 2, (ULONG)OPUBarcode.SA_ACT_BD_L0);
    send_msg5SValue(1, "Barcode.SA_ACT_BD_L1 = ", 2, (ULONG)OPUBarcode.SA_ACT_BD_L1);
    send_msg5SValue(1, "Barcode.SONY_410_CHECK_SUM = ", 2, (ULONG)OPUBarcode.SONY_410_CHECK_SUM);
}
#endif
#if(OPU == SFBD412)||(OPU == SFBD414)
#if(USE_EEPROM_COLLI_POS == 0)
static void SFBD412_barcode(void)   //Sanyo BDROM
{
    if ((svoPcalVar.CalDoneBits & bd_barcode_end)==bd_barcode_end)
    {
            OPUBarcode.OPU_FEATURE        = GetPersistentTable1B(OPU_FEATURE_ADR)-0x30;
            OPUBarcode.MANU_CONTROL_CODE  = GetPersistentTable1B(MANU_CONTROL_CODE_ADR)-0x30;
            OPUBarcode.PROD_LINE_CODE     = GetPersistentTable1B(PROD_LINE_CODE_ADR)-0x30;
            OPUBarcode.LABEL_ISSUE_YEAR   = GetPersistentTable1B(LABEL_ISSUE_YEAR_ADR);
            OPUBarcode.LABEL_ISSUE_MONTH  = GetPersistentTable1B(LABEL_ISSUE_MONTH_ADR);
            OPUBarcode.LABEL_ISSUE_DATE   = GetPersistentTable1B(LABEL_ISSUE_DATE_ADR);

            OPUBarcode.TAN_SKEW_CD        = ((GetPersistentTable1B(TAN_SKEW_CD1_ADR)>0x40) ?((GetPersistentTable1B(TAN_SKEW_CD1_ADR)-0x37)*16) : ((GetPersistentTable1B(TAN_SKEW_CD1_ADR)-0x30)*16))+((GetPersistentTable1B(TAN_SKEW_CD0_ADR)> 0x40) ?((GetPersistentTable1B(TAN_SKEW_CD0_ADR)-0x37)) : ((GetPersistentTable1B(TAN_SKEW_CD0_ADR)-0x30)));
            OPUBarcode.RAD_SKEW_CD        = ((GetPersistentTable1B(RAD_SKEW_CD1_ADR)>0x40) ?((GetPersistentTable1B(RAD_SKEW_CD1_ADR)-0x37)*16) : ((GetPersistentTable1B(RAD_SKEW_CD1_ADR)-0x30)*16))+((GetPersistentTable1B(RAD_SKEW_CD0_ADR)> 0x40) ?((GetPersistentTable1B(RAD_SKEW_CD0_ADR)-0x37)) : ((GetPersistentTable1B(RAD_SKEW_CD0_ADR)-0x30)));
            OPUBarcode.TAN_SKEW_DVD       = ((GetPersistentTable1B(TAN_SKEW_DVD1_ADR)>0x40) ?((GetPersistentTable1B(TAN_SKEW_DVD1_ADR)-0x37)*16) : ((GetPersistentTable1B(TAN_SKEW_DVD1_ADR)-0x30)*16))+((GetPersistentTable1B(TAN_SKEW_DVD0_ADR)> 0x40) ?((GetPersistentTable1B(TAN_SKEW_DVD0_ADR)-0x37)) : ((GetPersistentTable1B(TAN_SKEW_DVD0_ADR)-0x30)));
            OPUBarcode.RAD_SKEW_DVD       = ((GetPersistentTable1B(RAD_SKEW_DVD1_ADR)>0x40) ?((GetPersistentTable1B(RAD_SKEW_DVD1_ADR)-0x37)*16) : ((GetPersistentTable1B(RAD_SKEW_DVD1_ADR)-0x30)*16))+((GetPersistentTable1B(RAD_SKEW_DVD0_ADR)> 0x40) ?((GetPersistentTable1B(RAD_SKEW_DVD0_ADR)-0x37)) : ((GetPersistentTable1B(RAD_SKEW_DVD0_ADR)-0x30)));
            OPUBarcode.TAN_SKEW_BD        = ((GetPersistentTable1B(TAN_SKEW_BD1_ADR)>0x40) ?((GetPersistentTable1B(TAN_SKEW_BD1_ADR)-0x37)*16) : ((GetPersistentTable1B(TAN_SKEW_BD1_ADR)-0x30)*16))+((GetPersistentTable1B(TAN_SKEW_BD0_ADR)> 0x40) ?((GetPersistentTable1B(TAN_SKEW_BD0_ADR)-0x37)) : ((GetPersistentTable1B(TAN_SKEW_BD0_ADR)-0x30)));
            OPUBarcode.RAD_SKEW_BD        = ((GetPersistentTable1B(RAD_SKEW_BD1_ADR)>0x40) ?((GetPersistentTable1B(RAD_SKEW_BD1_ADR)-0x37)*16) : ((GetPersistentTable1B(RAD_SKEW_BD1_ADR)-0x30)*16))+((GetPersistentTable1B(RAD_SKEW_BD0_ADR)> 0x40) ?((GetPersistentTable1B(RAD_SKEW_BD0_ADR)-0x37)) : ((GetPersistentTable1B(RAD_SKEW_BD0_ADR)-0x30)));

            OPUBarcode.DF_CD = GetPersistentTable1B(DF_CD4_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_CD3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_CD2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_CD0_ADR)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_CD3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_CD2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_CD0_ADR)-0x30)))/10;
            OPUBarcode.DF_DVD = GetPersistentTable1B(DF_DVD4_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_DVD3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_DVD2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_DVD0_ADR)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_DVD3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_DVD2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_DVD0_ADR)-0x30)))/10;
            OPUBarcode.DF_BDSL = GetPersistentTable1B(DF_BDSL4_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_BDSL3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_BDSL2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_BDSL0_ADR)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_BDSL3_ADR)-0x30)*100)+((GetPersistentTable1B(DF_BDSL2_ADR)-0x30)*10)+((GetPersistentTable1B(DF_BDSL0_ADR)-0x30)))/10;
            OPUBarcode.TEMP_BD      = ((GetPersistentTable1B(TEMP_BD1_ADR)>0x40) ?((GetPersistentTable1B(TEMP_BD1_ADR)-0x37)*16) : ((GetPersistentTable1B(TEMP_BD1_ADR)-0x30)*16))+((GetPersistentTable1B(TEMP_BD0_ADR)> 0x40) ?((GetPersistentTable1B(TEMP_BD0_ADR)-0x37)) : ((GetPersistentTable1B(TEMP_BD0_ADR)-0x30)));
            OPUBarcode.SA_ACT_BD_L0 = ((GetPersistentTable1B(COLLI_PSN_BDSL1_ADR)>0x40) ?((GetPersistentTable1B(COLLI_PSN_BDSL1_ADR)-0x37)*16) : ((GetPersistentTable1B(COLLI_PSN_BDSL1_ADR)-0x30)*16))+((GetPersistentTable1B(COLLI_PSN_BDSL0_ADR)> 0x40) ?((GetPersistentTable1B(COLLI_PSN_BDSL0_ADR)-0x37)) : ((GetPersistentTable1B(COLLI_PSN_BDSL0_ADR)-0x30)));
            if((OPUBarcode.SA_ACT_BD_L0 > 170)||(OPUBarcode.SA_ACT_BD_L0 < 50))
               OPUBarcode.SA_ACT_BD_L0       = 112;

            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0+92;

    }
    else
    {//Error in persistent table
       // send_msg5S(SHOW_DEBUG_MSG,"BarcodeChkSunFail!");
            OPUBarcode.OPU_FEATURE        = 0;
            OPUBarcode.MANU_CONTROL_CODE  = 0;
            OPUBarcode.PROD_LINE_CODE     = 0x4f;   //O
            OPUBarcode.LABEL_ISSUE_YEAR   = 0x49;   //I
            OPUBarcode.LABEL_ISSUE_MONTH  = 6;
            OPUBarcode.LABEL_ISSUE_DATE   = 6;
            OPUBarcode.TAN_SKEW_CD        = 0x02;
            OPUBarcode.RAD_SKEW_CD        = 0x0A;
            OPUBarcode.TAN_SKEW_DVD       = 0xFF;
            OPUBarcode.RAD_SKEW_DVD       = 0x08;
            OPUBarcode.TAN_SKEW_BD        = 0x01;
            OPUBarcode.RAD_SKEW_BD        = 0x06;

            OPUBarcode.DF_CD              = -6.0;
            OPUBarcode.DF_DVD             = -1.6;
            OPUBarcode.DF_BDSL            =  1.8;
            OPUBarcode.TEMP_BD            = 0x1B;
        #if(OPU == SFBD414)
            OPUBarcode.SA_ACT_BD_L0       = 240; //246
            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0-92;
        #else
            OPUBarcode.SA_ACT_BD_L0       = 112;
            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0+92;
        #endif
    }
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x317517,B4B(OPUBarcode.SA_ACT_BD_L0));
//        send_msg5SValue(1, "B.CHECK_SUM_H        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_H        );
//        send_msg5SValue(1, "B.CHECK_SUM_L        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_L        );
}
#endif
#endif
#if(OPU == SOHBP7)
static void SOHBP7_barcode(void)
{
#if(USE_EEPROM_COLLI_POS == 1)
        send_msg5S(SHOW_DEBUG_MSG,"SA EEPROM!");
        UpdateColliPos();
        OPUBarcode.SA_ACT_BD_L0       = svoCalColliVar[0].opt_value / 32;
        OPUBarcode.SA_ACT_BD_L1       = svoCalColliVar[1].opt_value / 32;
        if((OPUBarcode.SA_ACT_BD_L0 > 225)||(OPUBarcode.SA_ACT_BD_L0 < 75)) //150 +/- (150*50%)
        {
            OPUBarcode.SA_ACT_BD_L0       = 150;
            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0 + 130;
        }
#else
        send_msg5S(SHOW_DEBUG_MSG,"SA default!");
        OPUBarcode.SA_ACT_BD_L0       = 150;
        OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0 + 130; //280
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x317517,B4B(OPUBarcode.SA_ACT_BD_L0));
//        send_msg5SValue(1, "B.CHECK_SUM_H        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_H        );
//        send_msg5SValue(1, "B.CHECK_SUM_L        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_L        );
}
#endif
#if(OPU == HOPB1300)
static void HOPB1300_barcode(void)  //Hitachi BDROM
{
    if ((svoPcalVar.CalDoneBits & bd_barcode_end)==bd_barcode_end)
    {
        OPUBarcode.OPUTYPE = (0x0B<<12)| ((GetPersistentTable1B(OPUTYPE2_ADR)-0x30)<<8)|((GetPersistentTable1B(OPUTYPE1_ADR)-0x30)<<4)|((GetPersistentTable1B(OPUTYPE0_ADR)-0x30));
        OPUBarcode.RAD_SKEW_DVD = GetPersistentTable1B(RAD_SKEW_DVD2_ADR) == 0x2B ? ((GetPersistentTable1B(RAD_SKEW_DVD1_ADR)-0x30)*10)+((GetPersistentTable1B(RAD_SKEW_DVD0_ADR)-0x30)) : -(((GetPersistentTable1B(RAD_SKEW_DVD1_ADR)-0x30)*10)+((GetPersistentTable1B(RAD_SKEW_DVD0_ADR)-0x30)));
        OPUBarcode.TAN_SKEW_DVD = GetPersistentTable1B(TAN_SKEW_DVD2_ADR) == 0x2B ? ((GetPersistentTable1B(TAN_SKEW_DVD1_ADR)-0x30)*10)+((GetPersistentTable1B(TAN_SKEW_DVD0_ADR)-0x30)) : -(((GetPersistentTable1B(TAN_SKEW_DVD1_ADR)-0x30)*10)+((GetPersistentTable1B(TAN_SKEW_DVD0_ADR)-0x30)));
        OPUBarcode.RAD_SKEW_BD = GetPersistentTable1B(RAD_SKEW_BD2_ADR) == 0x2B ? ((GetPersistentTable1B(RAD_SKEW_BD1_ADR)-0x30)*10)+((GetPersistentTable1B(RAD_SKEW_BD0_ADR)-0x30)) : -(((GetPersistentTable1B(RAD_SKEW_BD1_ADR)-0x30)*10)+((GetPersistentTable1B(RAD_SKEW_BD0_ADR)-0x30)));
        OPUBarcode.TAN_SKEW_BD = GetPersistentTable1B(TAN_SKEW_BD2_ADR) == 0x2B ? ((GetPersistentTable1B(TAN_SKEW_BD1_ADR)-0x30)*10)+((GetPersistentTable1B(TAN_SKEW_BD0_ADR)-0x30)) : -(((GetPersistentTable1B(TAN_SKEW_BD1_ADR)-0x30)*10)+((GetPersistentTable1B(TAN_SKEW_BD0_ADR)-0x30)));
        OPUBarcode.SA_ACT_BD_L0 = GetPersistentTable1B(SA_ACT_BD_L03_ADR) == 0x2B ? ((GetPersistentTable1B(SA_ACT_BD_L02_ADR)-0x30)*100)+((GetPersistentTable1B(SA_ACT_BD_L01_ADR)-0x30)*10)+((GetPersistentTable1B(SA_ACT_BD_L00_ADR)-0x30)) : -(((GetPersistentTable1B(SA_ACT_BD_L02_ADR)-0x30)*100)+((GetPersistentTable1B(SA_ACT_BD_L01_ADR)-0x30)*10)+((GetPersistentTable1B(SA_ACT_BD_L00_ADR)-0x30)));
        OPUBarcode.SA_ACT_BD_L1 = OPUBarcode.SA_ACT_BD_L0+84;
        send_msg5S(SHOW_DEBUG_MSG,"BarcodeFLASHROM!");
    }
    else
    {
        OPUBarcode.OPUTYPE = 0xB130;
        OPUBarcode.RAD_SKEW_DVD = 22;
        OPUBarcode.TAN_SKEW_DVD = 0;
        OPUBarcode.RAD_SKEW_BD = 7;
        OPUBarcode.TAN_SKEW_BD = 10;
        OPUBarcode.SA_ACT_BD_L0 = 27;
        OPUBarcode.SA_ACT_BD_L1 = OPUBarcode.SA_ACT_BD_L0+84;
        send_msg5S(SHOW_DEBUG_MSG,"BarcodeDefault!");
    }
    send_msg5SValue(1, "Barcode.OPUTYPE = ", 2, (ULONG)OPUBarcode.OPUTYPE);
    send_msg5SValue(1, "Barcode.RAD_SKEW_BD = ", 1, (ULONG)OPUBarcode.RAD_SKEW_BD);
    send_msg5SValue(1, "Barcode.TAN_SKEW_BD = ", 1, (ULONG)OPUBarcode.TAN_SKEW_BD);
    send_msg5SValue(1, "Barcode.RAD_SKEW_DVD = ", 1,(ULONG)OPUBarcode.RAD_SKEW_DVD);
    send_msg5SValue(1, "Barcode.TAN_SKEW_DVD = ", 1,(ULONG)OPUBarcode.TAN_SKEW_DVD);
    send_msg5SValue(1, "Barcode.SA_ACT_BD_L0 = ", 1, (ULONG)OPUBarcode.SA_ACT_BD_L0);
    send_msg5SValue(1, "Barcode.SA_ACT_BD_L1 = ", 1, (ULONG)OPUBarcode.SA_ACT_BD_L1);
}
#endif
#if(OPU == BDPL2)
static void BDPL2_barcode(void)   //Pioneer BDplayer
{
    if ((svoPcalVar.CalDoneBits & bd_barcode_end)==bd_barcode_end)
    {
            OPUBarcode.OPU_TYPE    = ((GetPersistentTable1B(OPU_TYPE_ADR)-0x30)*16)+(GetPersistentTable1B(OPU_TYPE_ADR+1)-0x30);
            //OPUBarcode.OPU_SERIAL  = GetPersistentTable1B(OPU_SERIAL_ADR)-0x30;
            OPUBarcode.OPU_QRVER   = (GetPersistentTable1B(OPU_QRVER_ADR)>0x40)? (GetPersistentTable1B(OPU_QRVER_ADR)-0x37) :(GetPersistentTable1B(OPU_QRVER_ADR)-0x30);

            OPUBarcode.BDSL_IV     = ((GetPersistentTable1B(BDSL_IV_ADR)>0x40) ?((GetPersistentTable1B(BDSL_IV_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_IV_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_IV_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_IV_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_IV_ADR+1)-0x30)));
            OPUBarcode.BDSL_IO     = ((GetPersistentTable1B(BDSL_IO_ADR)>0x40) ?((GetPersistentTable1B(BDSL_IO_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_IO_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_IO_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_IO_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_IO_ADR+1)-0x30)));
            OPUBarcode.BDSL_PL0    = ((GetPersistentTable1B(BDSL_PL0_ADR)>0x40) ?((GetPersistentTable1B(BDSL_PL0_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_PL0_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_PL0_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_PL0_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_PL0_ADR+1)-0x30)));
            OPUBarcode.BDSL_PL1    = ((GetPersistentTable1B(BDSL_PL1_ADR)>0x40) ?((GetPersistentTable1B(BDSL_PL1_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_PL1_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_PL1_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_PL1_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_PL1_ADR+1)-0x30)));
            OPUBarcode.BDSL_IGAIN  = ((GetPersistentTable1B(BDSL_IGAIN_ADR)>0x40) ?((GetPersistentTable1B(BDSL_IGAIN_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_IGAIN_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_IGAIN_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_IGAIN_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_IGAIN_ADR+1)-0x30)));
            OPUBarcode.BDSL_HMA    = ((GetPersistentTable1B(BDSL_HMA_ADR)>0x40) ?((GetPersistentTable1B(BDSL_HMA_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_HMA_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_HMA_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_HMA_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_HMA_ADR+1)-0x30)));
            OPUBarcode.BDSL_HMF    = ((GetPersistentTable1B(BDSL_HMF_ADR)>0x40) ?((GetPersistentTable1B(BDSL_HMF_ADR)-0x37)*16) : ((GetPersistentTable1B(BDSL_HMF_ADR)-0x30)*16))+((GetPersistentTable1B(BDSL_HMF_ADR+1)> 0x40) ?((GetPersistentTable1B(BDSL_HMF_ADR+1)-0x37)) : ((GetPersistentTable1B(BDSL_HMF_ADR+1)-0x30)));

            OPUBarcode.BDDL_IV     = ((GetPersistentTable1B(BDDL_IV_ADR)>0x40) ?((GetPersistentTable1B(BDDL_IV_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_IV_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_IV_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_IV_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_IV_ADR+1)-0x30)));
            OPUBarcode.BDDL_IO     = ((GetPersistentTable1B(BDDL_IO_ADR)>0x40) ?((GetPersistentTable1B(BDDL_IO_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_IO_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_IO_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_IO_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_IO_ADR+1)-0x30)));
            OPUBarcode.BDDL_PL0    = ((GetPersistentTable1B(BDDL_PL0_ADR)>0x40) ?((GetPersistentTable1B(BDDL_PL0_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_PL0_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_PL0_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_PL0_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_PL0_ADR+1)-0x30)));
            OPUBarcode.BDDL_PL1    = ((GetPersistentTable1B(BDDL_PL1_ADR)>0x40) ?((GetPersistentTable1B(BDDL_PL1_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_PL1_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_PL1_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_PL1_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_PL1_ADR+1)-0x30)));
            OPUBarcode.BDDL_IGAIN  = ((GetPersistentTable1B(BDDL_IGAIN_ADR)>0x40) ?((GetPersistentTable1B(BDDL_IGAIN_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_IGAIN_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_IGAIN_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_IGAIN_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_IGAIN_ADR+1)-0x30)));
            OPUBarcode.BDDL_HMA    = ((GetPersistentTable1B(BDDL_HMA_ADR)>0x40) ?((GetPersistentTable1B(BDDL_HMA_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_HMA_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_HMA_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_HMA_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_HMA_ADR+1)-0x30)));
            OPUBarcode.BDDL_HMF    = ((GetPersistentTable1B(BDDL_HMF_ADR)>0x40) ?((GetPersistentTable1B(BDDL_HMF_ADR)-0x37)*16) : ((GetPersistentTable1B(BDDL_HMF_ADR)-0x30)*16))+((GetPersistentTable1B(BDDL_HMF_ADR+1)> 0x40) ?((GetPersistentTable1B(BDDL_HMF_ADR+1)-0x37)) : ((GetPersistentTable1B(BDDL_HMF_ADR+1)-0x30)));

            OPUBarcode.DV_IV       = ((GetPersistentTable1B(DV_IV_ADR)>0x40) ?((GetPersistentTable1B(DV_IV_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_IV_ADR)-0x30)*16))+((GetPersistentTable1B(DV_IV_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_IV_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_IV_ADR+1)-0x30)));
            OPUBarcode.DV_IO       = ((GetPersistentTable1B(DV_IO_ADR)>0x40) ?((GetPersistentTable1B(DV_IO_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_IO_ADR)-0x30)*16))+((GetPersistentTable1B(DV_IO_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_IO_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_IO_ADR+1)-0x30)));
            OPUBarcode.DV_P        = ((GetPersistentTable1B(DV_P_ADR)>0x40) ?((GetPersistentTable1B(DV_P_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_P_ADR)-0x30)*16))+((GetPersistentTable1B(DV_P_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_P_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_P_ADR+1)-0x30)));
            OPUBarcode.DV_IGAIN    = ((GetPersistentTable1B(DV_IGAIN_ADR)>0x40) ?((GetPersistentTable1B(DV_IGAIN_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_IGAIN_ADR)-0x30)*16))+((GetPersistentTable1B(DV_IGAIN_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_IGAIN_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_IGAIN_ADR+1)-0x30)));
            OPUBarcode.DV_HMA      = ((GetPersistentTable1B(DV_HMA_ADR)>0x40) ?((GetPersistentTable1B(DV_HMA_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_HMA_ADR)-0x30)*16))+((GetPersistentTable1B(DV_HMA_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_HMA_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_HMA_ADR+1)-0x30)));
            OPUBarcode.DV_HMF      = ((GetPersistentTable1B(DV_HMF_ADR)>0x40) ?((GetPersistentTable1B(DV_HMF_ADR)-0x37)*16) : ((GetPersistentTable1B(DV_HMF_ADR)-0x30)*16))+((GetPersistentTable1B(DV_HMF_ADR+1)> 0x40) ?((GetPersistentTable1B(DV_HMF_ADR+1)-0x37)) : ((GetPersistentTable1B(DV_HMF_ADR+1)-0x30)));

            OPUBarcode.CD_IV       = ((GetPersistentTable1B(CD_IV_ADR)>0x40) ?((GetPersistentTable1B(CD_IV_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_IV_ADR)-0x30)*16))+((GetPersistentTable1B(CD_IV_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_IV_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_IV_ADR+1)-0x30)));
            OPUBarcode.CD_IO       = ((GetPersistentTable1B(CD_IO_ADR)>0x40) ?((GetPersistentTable1B(CD_IO_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_IO_ADR)-0x30)*16))+((GetPersistentTable1B(CD_IO_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_IO_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_IO_ADR+1)-0x30)));
            OPUBarcode.CD_P        = ((GetPersistentTable1B(CD_P_ADR)>0x40) ?((GetPersistentTable1B(CD_P_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_P_ADR)-0x30)*16))+((GetPersistentTable1B(CD_P_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_P_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_P_ADR+1)-0x30)));
            OPUBarcode.CD_IGAIN    = ((GetPersistentTable1B(CD_IGAIN_ADR)>0x40) ?((GetPersistentTable1B(CD_IGAIN_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_IGAIN_ADR)-0x30)*16))+((GetPersistentTable1B(CD_IGAIN_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_IGAIN_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_IGAIN_ADR+1)-0x30)));
            OPUBarcode.CD_HMA      = ((GetPersistentTable1B(CD_HMA_ADR)>0x40) ?((GetPersistentTable1B(CD_HMA_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_HMA_ADR)-0x30)*16))+((GetPersistentTable1B(CD_HMA_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_HMA_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_HMA_ADR+1)-0x30)));
            OPUBarcode.CD_HMF      = ((GetPersistentTable1B(CD_HMF_ADR)>0x40) ?((GetPersistentTable1B(CD_HMF_ADR)-0x37)*16) : ((GetPersistentTable1B(CD_HMF_ADR)-0x30)*16))+((GetPersistentTable1B(CD_HMF_ADR+1)> 0x40) ?((GetPersistentTable1B(CD_HMF_ADR+1)-0x37)) : ((GetPersistentTable1B(CD_HMF_ADR+1)-0x30)));

            OPUBarcode.SA_ACT_BD_L0= ((GetPersistentTable1B(BDSA0_ADR)-0x30)*100)+((GetPersistentTable1B(BDSA0_ADR+1)-0x30)*10)+(GetPersistentTable1B(BDSA0_ADR+2)-0x30);
            OPUBarcode.SA_ACT_BD_L1= OPUBarcode.SA_ACT_BD_L0-105;
            OPUBarcode.DVSA        = ((GetPersistentTable1B(DVSA_ADR )-0x30)*100)+((GetPersistentTable1B(DVSA_ADR +1)-0x30)*10)+(GetPersistentTable1B(DVSA_ADR +2)-0x30);
            OPUBarcode.CDSA        = ((GetPersistentTable1B(CDSA_ADR )-0x30)*100)+((GetPersistentTable1B(CDSA_ADR +1)-0x30)*10)+(GetPersistentTable1B(CDSA_ADR +2)-0x30);

            OPUBarcode.DF_BD       = GetPersistentTable1B(DF_BD_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_BD_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_BD_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_BD_ADR+3)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_BD_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_BD_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_BD_ADR+3)-0x30)))/10;
            OPUBarcode.DF_DV       = GetPersistentTable1B(DF_DV_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_DV_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_DV_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_DV_ADR+3)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_DV_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_DV_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_DV_ADR+3)-0x30)))/10;
            OPUBarcode.DF_CD       = GetPersistentTable1B(DF_CD_ADR) == 0x2B ? (float)(((GetPersistentTable1B(DF_CD_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_CD_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_CD_ADR+3)-0x30)))/10 : -(float)(((GetPersistentTable1B(DF_CD_ADR+1)-0x30)*100)+((GetPersistentTable1B(DF_CD_ADR+2)-0x30)*10)+((GetPersistentTable1B(DF_CD_ADR+3)-0x30)))/10;

            OPUBarcode.BD_APPK_X10 = (((GetPersistentTable1B(BD_APPK_ADR)-0x30)*10)+((GetPersistentTable1B(BD_APPK_ADR+1)-0x30)));
            OPUBarcode.CD_DPPK_X10 = (((GetPersistentTable1B(CD_DPPK_ADR)-0x30)*10)+((GetPersistentTable1B(CD_DPPK_ADR+1)-0x30)));

            OPUBarcode.BD_RAD_TILT_X100 = GetPersistentTable1B(BD_RAD_TILT_ADR) == 0x2B ? (((GetPersistentTable1B(BD_RAD_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(BD_RAD_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(BD_RAD_TILT_ADR+3)-0x30))) : -(((GetPersistentTable1B(BD_RAD_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(BD_RAD_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(BD_RAD_TILT_ADR+3)-0x30)));
            OPUBarcode.DV_RAD_TILT_X100 = GetPersistentTable1B(DV_RAD_TILT_ADR) == 0x2B ? (((GetPersistentTable1B(DV_RAD_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(DV_RAD_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(DV_RAD_TILT_ADR+3)-0x30))) : -(((GetPersistentTable1B(DV_RAD_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(DV_RAD_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(DV_RAD_TILT_ADR+3)-0x30)));
            OPUBarcode.BD_TAN_TILT_X100 = GetPersistentTable1B(BD_TAN_TILT_ADR) == 0x2B ? (((GetPersistentTable1B(BD_TAN_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(BD_TAN_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(BD_TAN_TILT_ADR+3)-0x30))) : -(((GetPersistentTable1B(BD_TAN_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(BD_TAN_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(BD_TAN_TILT_ADR+3)-0x30)));
            OPUBarcode.DV_TAN_TILT_X100 = GetPersistentTable1B(DV_TAN_TILT_ADR) == 0x2B ? (((GetPersistentTable1B(DV_TAN_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(DV_TAN_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(DV_TAN_TILT_ADR+3)-0x30))) : -(((GetPersistentTable1B(DV_TAN_TILT_ADR+1)-0x30)*100)+((GetPersistentTable1B(DV_TAN_TILT_ADR+2)-0x30)*10)+((GetPersistentTable1B(DV_TAN_TILT_ADR+3)-0x30)));

            OPUBarcode.BD_INI_TEMP_X10 = (((GetPersistentTable1B(BD_INI_TEMP_ADR)-0x30)*100)+((GetPersistentTable1B(BD_INI_TEMP_ADR+1)-0x30)*10)+(GetPersistentTable1B(BD_INI_TEMP_ADR+1)-0x30));
            OPUBarcode.DV_INI_TEMP_X10 = (((GetPersistentTable1B(DV_INI_TEMP_ADR)-0x30)*100)+((GetPersistentTable1B(DV_INI_TEMP_ADR+1)-0x30)*10)+(GetPersistentTable1B(DV_INI_TEMP_ADR+1)-0x30));
            OPUBarcode.CD_INI_TEMP_X10 = (((GetPersistentTable1B(CD_INI_TEMP_ADR)-0x30)*100)+((GetPersistentTable1B(CD_INI_TEMP_ADR+1)-0x30)*10)+(GetPersistentTable1B(CD_INI_TEMP_ADR+1)-0x30));

            send_msg80(1,0x630103);
    }
    else
    {//Error in persistent table
       // send_msg5S(SHOW_DEBUG_MSG,"BarcodeChkSunFail!");
        send_msg5S(SHOW_DEBUG_MSG,"BarcodeDefault!");
#if  0  //proto1
            OPUBarcode.OPU_TYPE    = 98;
            //OPUBarcode.OPU_SERIAL  = 10000135;
            OPUBarcode.OPU_QRVER   = 0x0A;

            OPUBarcode.BDSL_IV     = 0x80;
            OPUBarcode.BDSL_IO     = 0x02;
            OPUBarcode.BDSL_PL0    = 0xB3; //APC target when colli in 216 step
            OPUBarcode.BDSL_PL1    = 0xA5; //APC target when colli in 111 step
            OPUBarcode.BDSL_IGAIN  = 0x20;
            OPUBarcode.BDSL_HMA    = 0x3A;
            OPUBarcode.BDSL_HMF    = 0x52;

            OPUBarcode.BDDL_IV     = 0x60;
            OPUBarcode.BDDL_IO     = 0x02;
            OPUBarcode.BDDL_PL0    = 0xC4;
            OPUBarcode.BDDL_PL1    = 0xB5;
            OPUBarcode.BDDL_IGAIN  = 0x37;
            OPUBarcode.BDDL_HMA    = 0x66;
            OPUBarcode.BDDL_HMF    = 0x52;

            OPUBarcode.DV_IV       = 0x88;
            OPUBarcode.DV_IO       = 0x02;
            OPUBarcode.DV_P        = 0xC3;
            OPUBarcode.DV_IGAIN    = 0x1C;
            OPUBarcode.DV_HMA      = 0x2F;
            OPUBarcode.DV_HMF      = 0x52;

            OPUBarcode.CD_IV       = 0x68;
            OPUBarcode.CD_IO       = 0x02;
            OPUBarcode.CD_P        = 0x7D;
            OPUBarcode.CD_IGAIN    = 0x60;
            OPUBarcode.CD_HMA      = 0x2F;
            OPUBarcode.CD_HMF      = 0x52;

            OPUBarcode.SA_ACT_BD_L0       = 221;
            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0-105;
            OPUBarcode.DVSA        = 123;
            OPUBarcode.CDSA        = 152;

            OPUBarcode.DF_BD       = +00.3;
            OPUBarcode.DF_DV       = +04.5;
            OPUBarcode.DF_CD       = -05.6;

            OPUBarcode.BD_APPK_X10 = 22;
            OPUBarcode.CD_DPPK_X10 = 21;

            OPUBarcode.BD_RAD_TILT_X100 = -6;
            OPUBarcode.DV_RAD_TILT_X100 = 9;
            OPUBarcode.BD_TAN_TILT_X100 = -10;
            OPUBarcode.DV_TAN_TILT_X100 = -14;

            OPUBarcode.BD_INI_TEMP_X10 = 268;  //colli comp 14.6 steps/10degreeC
            OPUBarcode.DV_INI_TEMP_X10 = 285;
            OPUBarcode.CD_INI_TEMP_X10 = 288;
#endif
#if  1  //proto2
            OPUBarcode.OPU_TYPE    = 98;
            //OPUBarcode.OPU_SERIAL  = 10000413;
            OPUBarcode.OPU_QRVER   = 0x0B;

            OPUBarcode.BDSL_IV     = 0x80;
            OPUBarcode.BDSL_IO     = 0x02;
            OPUBarcode.BDSL_PL0    = 0xC0; //APC target when colli in 246 step
            OPUBarcode.BDSL_PL1    = 0xB2; //APC target when colli in 141 step
            OPUBarcode.BDSL_IGAIN  = 0x20;
            OPUBarcode.BDSL_HMA    = 0x3A;
            OPUBarcode.BDSL_HMF    = 0x55;

            OPUBarcode.BDDL_IV     = 0x60;
            OPUBarcode.BDDL_IO     = 0x02;
            OPUBarcode.BDDL_PL0    = 0x9C;
            OPUBarcode.BDDL_PL1    = 0x92;
            OPUBarcode.BDDL_IGAIN  = 0x60;
            OPUBarcode.BDDL_HMA    = 0x66;
            OPUBarcode.BDDL_HMF    = 0x55;

            OPUBarcode.DV_IV       = 0x88;
            OPUBarcode.DV_IO       = 0x02;
            OPUBarcode.DV_P        = 0xC0; //APC target when colli in 158 step
            OPUBarcode.DV_IGAIN    = 0x1C;
            OPUBarcode.DV_HMA      = 0x2F;
            OPUBarcode.DV_HMF      = 0x55;

            OPUBarcode.CD_IV       = 0x88;
            OPUBarcode.CD_IO       = 0x02;
            OPUBarcode.CD_P        = 0xF6; //APC target when colli in 151-konika (181-HOYA) step
            OPUBarcode.CD_IGAIN    = 0x20;
            OPUBarcode.CD_HMA      = 0x2F;
            OPUBarcode.CD_HMF      = 0x55;

            OPUBarcode.SA_ACT_BD_L0       = 245;
            OPUBarcode.SA_ACT_BD_L1       = OPUBarcode.SA_ACT_BD_L0-105;
            OPUBarcode.DVSA        = 164;
            OPUBarcode.CDSA        = 190;

            OPUBarcode.DF_BD       = +00.8;
            OPUBarcode.DF_DV       = +03.9;
            OPUBarcode.DF_CD       = -00.7;

            OPUBarcode.BD_APPK_X10 = 25;
            OPUBarcode.CD_DPPK_X10 = 20;

            OPUBarcode.BD_RAD_TILT_X100 = 13;
            OPUBarcode.DV_RAD_TILT_X100 = 16;
            OPUBarcode.BD_TAN_TILT_X100 = -13;
            OPUBarcode.DV_TAN_TILT_X100 = 0;

            OPUBarcode.BD_INI_TEMP_X10 = 243;  //colli comp 14.6 steps/10degreeC
            OPUBarcode.DV_INI_TEMP_X10 = 263;
            OPUBarcode.CD_INI_TEMP_X10 = 260;
#endif

    }
        send_msg5SValue(0, "B.BDSL_IV    = ", 1, (ULONG)OPUBarcode.BDSL_IV     );
        send_msg5SValue(0, "B.BDSL_IO    = ", 1, (ULONG)OPUBarcode.BDSL_IO     );
        send_msg5SValue(1, "B.BDSL_PL0   = ", 1, (ULONG)OPUBarcode.BDSL_PL0    );
        send_msg5SValue(1, "B.BDSL_PL1   = ", 1, (ULONG)OPUBarcode.BDSL_PL1    );
        send_msg5SValue(0, "B.BDSL_IGAIN = ", 1, (ULONG)OPUBarcode.BDSL_IGAIN  );
        send_msg5SValue(0, "B.BDSL_HMA   = ", 1, (ULONG)OPUBarcode.BDSL_HMA    );
        send_msg5SValue(0, "B.BDSL_HMF   = ", 1, (ULONG)OPUBarcode.BDSL_HMF    );
        send_msg5SValue(0, "B.BDDL_IV    = ", 1, (ULONG)OPUBarcode.BDDL_IV     );
        send_msg5SValue(0, "B.BDDL_IO    = ", 1, (ULONG)OPUBarcode.BDDL_IO     );
        send_msg5SValue(1, "B.BDDL_PL0   = ", 1, (ULONG)OPUBarcode.BDDL_PL0    );
        send_msg5SValue(1, "B.BDDL_PL1   = ", 1, (ULONG)OPUBarcode.BDDL_PL1    );
        send_msg5SValue(0, "B.BDDL_IGAIN = ", 1, (ULONG)OPUBarcode.BDDL_IGAIN  );
        send_msg5SValue(0, "B.BDDL_HMA   = ", 1, (ULONG)OPUBarcode.BDDL_HMA    );
        send_msg5SValue(0, "B.BDDL_HMF   = ", 1, (ULONG)OPUBarcode.BDDL_HMF    );
        send_msg5SValue(0, "B.DV_IV      = ", 1, (ULONG)OPUBarcode.DV_IV       );
        send_msg5SValue(0, "B.DV_IO      = ", 1, (ULONG)OPUBarcode.DV_IO       );
        send_msg5SValue(1, "B.DV_P       = ", 1, (ULONG)OPUBarcode.DV_P        );
        send_msg5SValue(0, "B.DV_IGAIN   = ", 1, (ULONG)OPUBarcode.DV_IGAIN    );
        send_msg5SValue(0, "B.DV_HMA     = ", 1, (ULONG)OPUBarcode.DV_HMA      );
        send_msg5SValue(0, "B.DV_HMF     = ", 1, (ULONG)OPUBarcode.DV_HMF      );
        send_msg5SValue(0, "B.CD_IV      = ", 1, (ULONG)OPUBarcode.CD_IV       );
        send_msg5SValue(0, "B.CD_IO      = ", 1, (ULONG)OPUBarcode.CD_IO       );
        send_msg5SValue(1, "B.CD_P       = ", 1, (ULONG)OPUBarcode.CD_P        );
        send_msg5SValue(0, "B.CD_IGAIN   = ", 1, (ULONG)OPUBarcode.CD_IGAIN    );
        send_msg5SValue(0, "B.CD_HMA     = ", 1, (ULONG)OPUBarcode.CD_HMA      );
        send_msg5SValue(0, "B.CD_HMF     = ", 1, (ULONG)OPUBarcode.CD_HMF      );

        send_msg5SValue(1, "B.BDSA0      = ", 1, (ULONG)OPUBarcode.SA_ACT_BD_L0);
        send_msg5SValue(1, "B.DVSA       = ", 1, (ULONG)OPUBarcode.DVSA        );
        send_msg5SValue(1, "B.CDSA       = ", 1, (ULONG)OPUBarcode.CDSA        );
        send_msg5SValue(1, "B.DF_BD      = ", 1, (float)OPUBarcode.DF_BD       );
        send_msg5SValue(1, "B.DF_DV      = ", 1, (float)OPUBarcode.DF_DV       );
        send_msg5SValue(1, "B.DF_CD      = ", 1, (float)OPUBarcode.DF_CD       );
        send_msg5SValue(1, "B.BD_APPK    = ", 1, (ULONG)OPUBarcode.BD_APPK_X10 );
        send_msg5SValue(1, "B.CD_DPPK    = ", 1, (ULONG)OPUBarcode.CD_DPPK_X10 );

        send_msg5SValue(0, "B.BD_RAD_TILT       = ", 2, (ULONG)OPUBarcode.BD_RAD_TILT_X100  );
        send_msg5SValue(0, "B.DV_RAD_TILT       = ", 2, (ULONG)OPUBarcode.DV_RAD_TILT_X100  );
        send_msg5SValue(0, "B.BD_TAN_TILT       = ", 2, (ULONG)OPUBarcode.BD_TAN_TILT_X100  );
        send_msg5SValue(0, "B.DV_TAN_TILT       = ", 2, (ULONG)OPUBarcode.DV_TAN_TILT_X100  );
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x317517,B4B(OPUBarcode.SA_ACT_BD_L0));
//        send_msg5SValue(1, "B.CHECK_SUM_H        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_H        );
//        send_msg5SValue(1, "B.CHECK_SUM_L        = ", 1, (ULONG)OPUBarcode.CHECK_SUM_L        );
}
#endif

void Cal_LoadOfflineCalibrationParam(void)
{
    int     i;
    USHORT  utmp1,utmp2;
//    discKindT tmpDiscKind;

    z_calRead_fgDoneBits();

/* Load CD FBDAC */
    if ((svoPcalVar.CalDoneBits & cd_fbdac_end)==cd_fbdac_end)
        svoPcalVar.cdFBDacValue = GetPersistentTable2B(CD_CAL_FBDAC_ADDR);

/* Load DVD FBDAC0 Layer0 */
    if ((svoPcalVar.CalDoneBits & dv_fbdac0_end)==dv_fbdac0_end)
        svoPcalVar.dvFBDac0Value = GetPersistentTable2B(DV_CAL_FBDAC0_ADDR);

/* Load CD FEBG */
    if ((svoPcalVar.CalDoneBits & cd_febg_end)==cd_febg_end)
    {
        svoPcalVar.cdFeoBGValue  = GetPersistentTable2B(CD_CAL_FEOBG_ADDR);
        svoPcalVar.cdSPP2BGValue = GetPersistentTable2B(CD_CAL_SPP2BG_ADDR);
    }

/* Load CD Bias Value */
    if ((svoPcalVar.CalPowerDoneBits & cd_bias_value_end)==cd_bias_value_end)
        svoPcalVar.cdDiffDac = GetPersistentTable2B(CD_BIAS_POWER_ADDR);

/* Load CD VWDC2VRDC Value */
    if ((svoPcalVar.CalPowerDoneBits & cd_vwdc2vrdc_end)==cd_vwdc2vrdc_end)
        svoPcalVar.cdVWDCtoVRDC = GetPersistentTable2B(CD_VWDC2VRDC_ADDR);

    /* Load CD VWDC2VWDC1 Value */
//    if ((svoPcalVar.CalPowerDoneBits & cd_vwdc2vwdc1_end)==cd_vwdc2vwdc1_end)
//        svoPcalVar.cdVWDCtoVWDC1 = GetPersistentTable2B(CD_VWDC2VWDC1_ADDR);

/* Load DVD FEBG Layer0 */
    if ((svoPcalVar.CalDoneBits & dv_febg0_end)==dv_febg0_end)
    {
       svoPcalVar.dvFeoBG0Value  = GetPersistentTable2B(DV_CAL_FEOBG0_ADDR);
       svoPcalVar.dvSPP2BG0Value = GetPersistentTable2B(DV_CAL_SPP2BG0_ADDR);
    }

/* Load CD Beta Offset */
    if ((svoPcalVar.CalDoneBits & cd_betaoffset_end)==cd_betaoffset_end)
        svoPcalVar.cdBetaOffset = GetPersistentTable2B(CD_CAL_BETA_OFFSET_ADDR);

/* Load DVD+R Beta Offset */
    if ((svoPcalVar.CalDoneBits & dv_plus_r_betaoffset_end)==dv_plus_r_betaoffset_end)
        svoPcalVar.dvBetaOffset = GetPersistentTable2B(DV_CAL_PLUS_BETA_OFFSET_ADDR);

/* Load DVD Bias Value */
    if ((svoPcalVar.CalPowerDoneBits & dv_bias_value_end)==dv_bias_value_end)
        svoPcalVar.dvDiffDac = GetPersistentTable2B(DV_BIAS_POWER_ADDR);

/* Load DVD VWDC2VRDC Value */
    if ((svoPcalVar.CalPowerDoneBits & dv_vwdc2vrdc_end)==dv_vwdc2vrdc_end)
        svoPcalVar.dvVWDCtoVRDC = GetPersistentTable2B(DV_VWDC2VRDC_ADDR);

    /* Load DVD VWDC2VWDC1 Value */
//    if ((svoPcalVar.CalPowerDoneBits & dv_vwdc2vwdc1_end)==dv_vwdc2vwdc1_end)
//        svoPcalVar.dvVWDCtoVWDC1 = GetPersistentTable2B(DV_VWDC2VWDC1_ADDR);

    /* Load DVD VWDC2VWDC2 Value */
//    if ((svoPcalVar.CalPowerDoneBits & dv_vwdc2vwdc2_end)==dv_vwdc2vwdc2_end)
//        svoPcalVar.dvVWDCtoVWDC1 = GetPersistentTable2B(DV_VWDC2VWDC2_ADDR);

/* Load DVD Tilt DAC */
    if ((svoPcalVar.CalDoneBits & dv_tiltdac_end)==dv_tiltdac_end)
    {
        svoPcalVar.dvTiltDacNum = GetPersistentTable2B(DV_CAL_TILT_NUM_ADDR);
        for (i=0;i<svoPcalVar.dvTiltDacNum;i++)
        {
            utmp1 = GetPersistentTable2B(DV_CAL_TILT_DAC0_ADDR+(i*4));
            utmp2 = GetPersistentTable2B(DV_CAL_TILT_DAC0_ADDR+(i*4)+2);
            utmp2 = ~utmp2;
            if (utmp1==utmp2)
            {
                svoPcalVar.dvTiltDacValue[i] = GetPersistentTable2B(DV_CAL_TILT_DAC0_ADDR+(i*4));
            }
            else
            {
                svoPcalVar.dvTiltDacNum=i;
                break;
            }
        }
    }

#if  0
    if ((svoPcalVar.CalPowerDoneBits & cd_kpower_end)==cd_kpower_end)
    {
        tmpDiscKind.fDiscKind = eCDROMDisc;
        COEF_StoreParams(tmpDiscKind, SVO_CAL_SPEED_CD); //Preset coef system with disckind and (Dummy) speed
        fetch_lasc_from_persistent(CD_OFFSET_DAC_THRESHOLD_ADDR);
        send_msg5S(SHOW_DEBUG_MSG,"fetch CD !");
    }
    else
    {//Error? in persistent table
    }

    if ((svoPcalVar.CalPowerDoneBits & dv_kpower_end)==dv_kpower_end)
    {
        tmpDiscKind.fDiscKind = eDVDROMDisc;
        COEF_StoreParams(tmpDiscKind, _1LX_DVD); //Preset coef system with disckind and (Dummy) speed
        fetch_lasc_from_persistent(DV_OFFSET_DAC_THRESHOLD_ADDR);
        send_msg5S(SHOW_DEBUG_MSG,"fetch DVD !");
    }
    else
    {//Error? in persistent table
    }

#if ENABLE_HDDVD==1
    if ((svoPcalVar.CalPowerDoneBits & hd_kpower_end)==hd_kpower_end)
    {
        #if (ENABLE_CBHD == 1)
        tmpDiscKind.fDiscKind = eCBHDROMDisc;
        #else
        tmpDiscKind.fDiscKind = eHDROMDisc;
        #endif
        COEF_StoreParams(tmpDiscKind, HD_1xCLV); //Preset coef system with disckind and (Dummy) speed
        fetch_lasc_from_persistent(HD_OFFSET_DAC_THRESHOLD_ADDR);
    }
    else
    {//Error? in persistent table
    }
#endif

#if BD_ENABLE==1
    if ((svoPcalVar.CalPowerDoneBits & bd_kpower_end)==bd_kpower_end)
    {
        tmpDiscKind.fDiscKind = eBDROMDisc;
        COEF_StoreParams(tmpDiscKind, BD_1xCLV); //Preset coef system with disckind and (Dummy) speed
        fetch_lasc_from_persistent(BD_OFFSET_DAC_THRESHOLD_ADDR);
    }
    else
    {//Error? in persistent table
    }
#endif

#endif

#if 0 // TBD? Should this code be deleted?
//TBD CalPowerDoneBits have to be extended with dvdram flags
//    if ((svoPcalVar.CalPowerDoneBits & ram_kpower_end)==ram_kpower_end)
//    {
//        tmpDiscKind.fDiscKind = eDVDRAMDisc;
//        COEF_StoreParams(tmpDiscKind, DVDRAM_1xCLV); //Preset coef system with disckind and (Dummy) speed
//        fetch_lasc_from_persistent(RAM_OFFSET_DAC_THRESHOLD_ADDR);
//    }
//    else
//    {//Error? in persistent table
//        send_msg5S(SHOW_DEBUG_MSG,"SAFE RAM Laser Read Power!");
//    }

#endif

#if(OPU == DXX811)
    DXX811_barcode();
#elif (OPU == SOHBP4)
        send_msg5S(SHOW_DEBUG_MSG,"BarcodeFunChk!");
    #if(USE_EEPROM_COLLI_POS == 1)
        svoPcalVar.CalDoneBits = GetPersistentTable1B(COLLI_CAL_STATUS_ADR)<<24;
    #endif
#elif (OPU == KES410A)
    KES410A_barcode();
#elif (OPU == KEM350AAA)
    printEEPROMcontent();
#elif (OPU == HOPB1300)
    HOPB1300_barcode();
#elif (OPU == SFBD412)||(OPU == SFBD414)
    #if(USE_EEPROM_COLLI_POS == 1)
        svoPcalVar.CalDoneBits = GetPersistentTable1B(COLLI_CAL_STATUS_ADR)<<24;
    #else
        SFBD412_barcode();
    #endif
#elif (OPU == SOHBP7)
    #if(USE_EEPROM_COLLI_POS == 1)
        svoPcalVar.CalDoneBits = GetPersistentTable1B(COLLI_CAL_STATUS_ADR)<<24;
    #endif
        SOHBP7_barcode();
#elif (OPU == BDPL2)
    BDPL2_barcode();
#endif // OPU
    FOCSENS.CD = GetPersistentTable1B(CD_FOC_SENS_ADDR);
    FOCSENS.DVD = GetPersistentTable1B(DV_FOC_SENS_ADDR);
    FOCSENS.HD = GetPersistentTable1B(HD_FOC_SENS_ADDR);
    FOCSENS.BD = GetPersistentTable1B(BD_FOC_SENS_ADDR);
}


/************************************************************************
* Module: z_calRead_fgDoneBits()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calRead_fgDoneBits(void)
{
//TBD! Lasc Offline calibrations changed in 6230. What about the rest?
    USHORT  utmp1,utmp2,i;

/* Check CD FBDAC CalDoneBits */
    utmp1 = GetPersistentTable2B(CD_CAL_FBDAC_ADDR);
    utmp2 = GetPersistentTable2B(CD_CAL_FBDAC_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=cd_fbdac_end;

/* Check CD FEBG CalDoneBits */
    utmp1 = GetPersistentTable2B(CD_CAL_FEOBG_ADDR);
    utmp2 = GetPersistentTable2B(CD_CAL_FEOBG_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=cd_febg_end;

    utmp1 = GetPersistentTable2B(CD_CAL_SPP2BG_ADDR);
    utmp2 = GetPersistentTable2B(CD_CAL_SPP2BG_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=cd_febg_end;

/* Check CD Beta Offset CalDoneBits */
    utmp1 = GetPersistentTable2B(CD_CAL_BETA_OFFSET_ADDR);
    utmp2 = GetPersistentTable2B(CD_CAL_BETA_OFFSET_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=cd_betaoffset_end;

/* Check CD Bias Value CalDoneBits */
    utmp1 = GetPersistentTable2B(CD_BIAS_POWER_ADDR);
    utmp2 = GetPersistentTable2B(CD_BIAS_POWER_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=cd_bias_value_end;

/* Check CD VWDC2VRDC Value CalDoneBits */
    utmp1 = GetPersistentTable2B(CD_VWDC2VRDC_ADDR);
    utmp2 = GetPersistentTable2B(CD_VWDC2VRDC_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=cd_vwdc2vrdc_end;

/* Check CD VWDC2VWDC1 Value CalDoneBits */
//    utmp1 = GetPersistentTable2B(CD_VWDC2VWDC1_ADDR);
//    utmp2 = GetPersistentTable2B(CD_VWDC2VWDC1_ADDR+2);
//    utmp2 = ~utmp2;
//    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=cd_vwdc2vwdc1_end;

/* Check DVD FBDAC CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_CAL_FBDAC0_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_FBDAC0_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_fbdac0_end;

/* Check DVD FEBG CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_CAL_FEOBG0_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_FEOBG0_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_febg0_end;

    utmp1 = GetPersistentTable2B(DV_CAL_SPP2BG0_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_SPP2BG0_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_febg0_end;

/* Check DVD+R Beta Offset CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_CAL_PLUS_BETA_OFFSET_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_PLUS_BETA_OFFSET_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_plus_r_betaoffset_end;

/* Check DVD-R Beta Offset CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_CAL_MINUS_BETA_OFFSET_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_MINUS_BETA_OFFSET_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_minus_r_betaoffset_end;

/* Check DVD Bias Value CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_BIAS_POWER_ADDR);
    utmp2 = GetPersistentTable2B(DV_BIAS_POWER_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=dv_bias_value_end;

/* Check DVD VWDC2VRDC Value CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_VWDC2VRDC_ADDR);
    utmp2 = GetPersistentTable2B(DV_VWDC2VRDC_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=dv_vwdc2vrdc_end;

/* Check DVD VWDC2VWDC1 Value CalDoneBits */
//    utmp1 = GetPersistentTable2B(DV_VWDC2VWDC1_ADDR);
//    utmp2 = GetPersistentTable2B(DV_VWDC2VWDC1_ADDR+2);
//    utmp2 = ~utmp2;
//    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=dv_vwdc2vwdc1_end;

/* Check DVD VWDC2VWDC2 Value CalDoneBits */
//    utmp1 = GetPersistentTable2B(DV_VWDC2VWDC2_ADDR);
//    utmp2 = GetPersistentTable2B(DV_VWDC2VWDC2_ADDR+2);
//    utmp2 = ~utmp2;
//    if (utmp1==utmp2) svoPcalVar.CalPowerDoneBits|=dv_vwdc2vwdc2_end;

/* Check CD Power Calibration CalDoneBits */
    utmp1 = 0;
    utmp2 = 0;
    for(i=CD_CAL_PWR_START_ADDR;i<CD_CAL_PWR_END_ADDR;)
    {
        utmp1 = utmp1^GetPersistentTable1B(i);
        utmp2 = utmp2^GetPersistentTable1B(i+1);
        i += 2;
    }

    if ((GetPersistentTable1B(CD_CHECK_SUM_ADDR)== (BYTE)utmp1)&&
        (GetPersistentTable1B(CD_CHECK_SUM_ADDR+1) == (BYTE)utmp2))
    {
        utmp1 = ~utmp1;
        utmp2 = ~utmp2;
        if ((GetPersistentTable1B(CD_CHECK_SUM_ADDR+2) == (BYTE)utmp1)&&
            (GetPersistentTable1B(CD_CHECK_SUM_ADDR+3) == (BYTE)utmp2))
        {
            svoPcalVar.CalPowerDoneBits|=cd_kpower_end;
        }
    }

/* Check DVD Power Calibration CalDoneBits */
    utmp1 = 0;
    utmp2 = 0;
    for(i=DV_CAL_PWR_START_ADDR;i<DV_CAL_PWR_END_ADDR;)
    {
        utmp1 = utmp1^GetPersistentTable1B(i);
        utmp2 = utmp2^GetPersistentTable1B(i+1);
        i += 2;
    }

    if ((GetPersistentTable1B(DV_CHECK_SUM_ADDR)== (BYTE)utmp1)&&
        (GetPersistentTable1B(DV_CHECK_SUM_ADDR+1) == (BYTE)utmp2))
    {
        utmp1 = ~utmp1;
        utmp2 = ~utmp2;
        if ((GetPersistentTable1B(DV_CHECK_SUM_ADDR+2) == (BYTE)utmp1)&&
            (GetPersistentTable1B(DV_CHECK_SUM_ADDR+3) == (BYTE)utmp2))
        {
            svoPcalVar.CalPowerDoneBits|=dv_kpower_end;
        }
    }

#if ENABLE_HDDVD==1
/* Check HD Power Calibration CalDoneBits */
    utmp1 = 0;
    utmp2 = 0;
    for(i=HD_CAL_PWR_START_ADDR;i<HD_CAL_PWR_END_ADDR;)
    {
        utmp1 = utmp1^GetPersistentTable1B(i);
        utmp2 = utmp2^GetPersistentTable1B(i+1);
        i += 2;
    }

    if ((GetPersistentTable1B(HD_CHECK_SUM_ADDR)== (BYTE)utmp1)&&
        (GetPersistentTable1B(HD_CHECK_SUM_ADDR+1) == (BYTE)utmp2))
    {
        utmp1 = ~utmp1;
        utmp2 = ~utmp2;
        if ((GetPersistentTable1B(HD_CHECK_SUM_ADDR+2) == (BYTE)utmp1)&&
            (GetPersistentTable1B(HD_CHECK_SUM_ADDR+3) == (BYTE)utmp2))
        {
            svoPcalVar.CalPowerDoneBits|=hd_kpower_end;
        }
    }
#endif

#if BD_ENABLE==1
///* Check BD Power Calibration CalDoneBits */
    utmp1 = 0;
    utmp2 = 0;
    for(i=BD_CAL_PWR_START_ADDR;i<BD_CAL_PWR_END_ADDR;)
    {
        utmp1 = utmp1^GetPersistentTable1B(i);
        utmp2 = utmp2^GetPersistentTable1B(i+1);
        i += 2;
    }

    if ((GetPersistentTable1B(BD_CHECK_SUM_ADDR)== (BYTE)utmp1)&&
        (GetPersistentTable1B(BD_CHECK_SUM_ADDR+1) == (BYTE)utmp2))
    {
        utmp1 = ~utmp1;
        utmp2 = ~utmp2;
        if ((GetPersistentTable1B(BD_CHECK_SUM_ADDR+2) == (BYTE)utmp1)&&
            (GetPersistentTable1B(BD_CHECK_SUM_ADDR+3) == (BYTE)utmp2))
        {
            svoPcalVar.CalPowerDoneBits|=bd_kpower_end;
        }
    }
#endif

/* Check DVD Tilt Calibration CalDoneBits */
    utmp1 = GetPersistentTable2B(DV_CAL_TILT_NUM_ADDR);
    utmp2 = GetPersistentTable2B(DV_CAL_TILT_NUM_ADDR+2);
    utmp2 = ~utmp2;
    if (utmp1==utmp2) svoPcalVar.CalDoneBits|=dv_tiltdac_end;

    #if ((PCB == 690)||(PCB == 692))
/* Check BD barcode CalDoneBits */
        #if (OPU == KES410A)
        utmp1 = 0;
        for(i=BD_BARCODE_START_ADDR;i<BD_BARCODE_END_ADDR;i++)
        {
            utmp1= utmp1+GetPersistentTable1B(i);
        }
        utmp1=~utmp1;
        utmp1= (utmp1)<<4;

        utmp2= (GetPersistentTable1B(SONY_410_CHECK_SUM_ADR)<<8)|GetPersistentTable1B(SONY_410_CHECK_SUM_ADR+1);

        if (utmp1==utmp2)
        {
            svoPcalVar.CalDoneBits|=bd_barcode_end;
        }

        #elif(OPU == BDPL2)
        utmp1 = 0;
        utmp2 = ((GetPersistentTable1B(OPU_TYPE_ADR)-0x30)*16)+(GetPersistentTable1B(OPU_TYPE_ADR+1)-0x30);
        if (utmp2 == 0x98) svoPcalVar.CalDoneBits|=bd_barcode_end;
		#endif
    #elif (PCB == 812)
    utmp1 = 0;
    if((GetPersistentTable1B(OPUTYPE3_ADR)-0x30)==0x12)
        utmp1 = 0x0B;
    utmp2 = (utmp1<<12)| ((GetPersistentTable1B(OPUTYPE2_ADR)-0x30)<<8)|((GetPersistentTable1B(OPUTYPE1_ADR)-0x30)<<4)|((GetPersistentTable1B(OPUTYPE0_ADR)-0x30));
    if (utmp2 == 0xB130) svoPcalVar.CalDoneBits|=bd_barcode_end;
    #elif (PCB == 710)||(PCB == 711)||(PCB == 817)
        #if (OPU == SFBD412)||(OPU == SFBD414)||(OPU == SOHBP7)
        utmp1 = 0xFF;
        utmp2 = 0xFF;
        //utmp1 = GetEEPROMTable1B(0x3E5);
        //utmp2 = GetEEPROMTable1B(0x3F5);
        if (utmp1==0)
        {
            if (utmp2==0) svoPcalVar.CalDoneBits|=bd_barcode_end;
        }
        #elif (OPU == KES410A)
        utmp1 = 0;
        for(i=BD_BARCODE_START_ADDR;i<BD_BARCODE_END_ADDR;i++)
        {
            utmp1= utmp1+GetPersistentTable1B(i);
        }
        utmp1=~utmp1;
        utmp1= (utmp1)<<4;

        utmp2= (GetPersistentTable1B(SONY_410_CHECK_SUM_ADR)<<8)|GetPersistentTable1B(SONY_410_CHECK_SUM_ADR+1);

        if (utmp1==utmp2) svoPcalVar.CalDoneBits|=bd_barcode_end;
	    #elif(OPU == BDPL2)
	    utmp1 = 0;
	    utmp2 = ((GetPersistentTable1B(OPU_TYPE_ADR)-0x30)*16)+(GetPersistentTable1B(OPU_TYPE_ADR+1)-0x30);
	    if (utmp2 == 0x98) svoPcalVar.CalDoneBits|=bd_barcode_end;
        #endif
    #elif(PCB == 815)
    utmp1 = 0;
    utmp2 = ((GetPersistentTable1B(OPU_TYPE_ADR)-0x30)*16)+(GetPersistentTable1B(OPU_TYPE_ADR+1)-0x30);
    if (utmp2 == 0x98) svoPcalVar.CalDoneBits|=bd_barcode_end;
    #endif
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
