/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2006 Sunext Design, INC.
*                 All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
* $Revision: 181 $
* $Date: 11/04/18 7:20p $
*
*  Description:
*   This file contains functions for the Analog Front End subsystem and includes
*   the following:
*       HAL_AFE_Init
*       HAL_AFE_SetRfBw
*       HAL_AFE_SetDPDBw
*       HAL_AFE_SetPhBhDrooprate
*       HAL_AFE_SetWobbleSpeed
*
* Notes:
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#if (BD_ENABLE == 1)
#include ".\BD\bdVars.h"
#endif // (BD_ENABLE == 1)
#include ".\AL\HAL_afe.h"
#include ".\Common\ComMath.h"

#if (ENABLE_HDDVD == 1)
#include ".\servo\Svo_lib.h"
#endif //(ENABLE_HDDVD == 1)

#include ".\servo\svoVar.h"
#include "..\al\coefs.h"

#if (BD_ENABLE == 1)
extern BOOL BDR_RE_PICReading;
#endif //(BD_ENABLE == 1)

/*** Local defines ***/
// Should be moved to enums in reg_afe
//Defines used in hal_afe_SetRfBw. Frequencies all in KHz.
#define DISCKIND_DVD_MASK       0x80
#define I3CLOCKPERIODS          6        /* Number of channel clocks in I3 signal.  */
#define I2CLOCKPERIODS          4        /* Number of channel clocks in I2 signal. */

#define RFB1STEP                1552     /* Average step-size of RF filter bank 1 in KHz */
#define RFB2STEP                2824     /* Average step-size of RF filter bank 2 in KHz */
#define RFB3STEP                10825    /* Average step-size of RF filter bank 3 in KHz */
#define RFB1OFF                 4975     /* Corner frequency of RF filter bank 1 with RFHSFC = 0 */
#define RFB2OFF                 17550    /* Corner frequency of RF filter bank 2 with RFHSFC = 0 */
#define RFB3OFF                 72820    /* Corner frequency of RF filter bank 3 with RFHSFC = 0 */
#define RFB1HIGH                29       /* Maximal setting of RFHSFC for RF filter bank 1 (0x1D) */
#define RFB2HIGH                25       /* Maximal setting of RFHSFC for RF filter bank 2 (0x19) */
#if (CHIP_REV >= 0xC1)
#define RFB3HIGH                19       /* Maximal setting of RFHSFC for RF filter bank 3 (0x0F) */
#else
#define RFB3HIGH                12       /* Maximal setting of RFHSFC for RF filter bank 3 (0x0F) *///720 use 16
#endif
#define RFB123MIN               0        /* Never below 0 */

#define DPDB1STEP               3075     /* Average step-size of DPD Equalizer filter bank 1 in KHz */
#define DPDB2STEP               7180     /* Average step-size of DPD Equalizer filter bank 2 in KHz */
#define DPDB3STEP               12121    /* Average step-size of DPD Equalizer filter bank 3 in KHz */
#define DPDB1OFF                2884     /* Corner frequency of DPD Equalizer filter bank 1 with RFHSFC = 0 */
#define DPDB2OFF                8253     /* Corner frequency of DPD Equalizer filter bank 2 with RFHSFC = 0 */
#define DPDB3OFF                19445    /* Corner frequency of DPD Equalizer filter bank 3 with RFHSFC = 0 */
#define DPDB1HIGH                0       /* Maximal setting of DPDHSFC for DPD Equalizer filter bank 1 and 2 (0x1F) */
#define DPDB2HIGH               20       /* Maximal setting of DPDHSFC for DPD Equalizer filter bank 1 and 2 (0x1F) */
#define DPDB3HIGH               20       /* Maximal setting of DPDHSFC for DPD Equalizer filter bank 3 (0x1A) */
#define DPDB123MIN               0       /* Never below 0 */

//Defines used in hal_afe_SetWobbleSpeed. Frequencies all in KHz
#define CDWOBN                  196      /* CDR, CDRW: number clock periods in one wobble cycle */
#define DVDPWOBN                32       /* all DVD plus media: number clock periods in one wobble cycle */
#define DVDMWOBN                186      /* all DVD min media: clock periods in one wobble cycle */
#define DVDRAMWOBN              186      /* all DVDRAM media: clock periods in one wobble cycle */
#define BDWOBN                  69       /* all BD media: number channel clock periods in one wobble cycle */
#define BDPICN                  36       /* all BD media in PIC zone: nr. of channel clock periods in one PIC cycle  */
#define HDWOBN                  93       /* all HD media: numberchannel clock periods in one wobble cycle */
#define I3PERIODS               6        /* Number of channel clocks in I3 signal */
#define LPFOFF                  1000     /* Corner freq. when WOBLPFBW(3-0) = 0x0 (kHz) */
#define LPFSTEP                 4000     /* Step frequency with lsb of WOBLPFBW(3-0) (kHz)*/
#define CD1CM                   170      /* Number of wobble periods in 1cm fingerprint for CD */
#define DVDP1CM                 2300     /* Number of wobble periods in 1cm fingerprint for DVDPLUS */
#define DVDM1CM                 400      /* Number of wobble periods in 1cm fingerprint for DVDMIN and DVDRAM */
#define DVDRAM1CM               400      /* Number of wobble periods in 1cm fingerprint for DVDMIN and DVDRAM */
#define BD1CM                   1900     /* Number of wobble periods in 1cm fingerprint for BD */
#define HD1CM                   1050     /* Number of wobble periods in 1cm fingerprint for HD */
#define WOBLPFMAX               15       /* Max field value */

#define HARM3RD                 3        /* 3rd harm. of highest PIC mod freq */

#if (CHIP_REV <= 0xB3)
const BYTE AFE_WBL_RPG[4] = { 0, 2, 4, 6 };
const BYTE AFE_WBL_RG[4]  = { 0, 4, 8,12 };

const SBYTE AFE_RFIN_GN[4] = {-11, -7, -6, -2};
const SBYTE AFE_RF_GN[4]   = { -6,  0,  6, 11};
#else
const BYTE AFE_WBL_RPG[4] = { 0, 2, 6, 8 };
const BYTE AFE_WBL_RG[4]  = { 0, 4,12,16 };

const SBYTE AFE_RFIN_GN[4] = {-11, -7, -6, -2};//-11.2,-6.9,-6.2,-2
const SBYTE AFE_RF_GN[8]   = { -9, -6, -3,  0,  3,  5,  8, 10};//-8.7,-5.9,-2.8,0,2.7,5.4,7.8,10.2
#endif

/*** Local static variables */
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_Init

    Description:    Initalization of the AFE block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_AFE_Init( void )
{
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_GetFELpfBW

    Description:    Get the FE LPF filter bandwidth
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_GetFELpfBW( void )
{
    BYTE   regvalue;
    USHORT Tempvalue;

    regvalue = READ_REG(HAL_AFEREG_MAP->FCTS32);
    Tempvalue = e_FELPF_BYPASS;
    if (!(regvalue & HAL_AFE_BPFELP_MASK))
    {
        Tempvalue = (regvalue & (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK));
    }
    return Tempvalue;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_GetTELpfBW

    Description:    Get the TE LPF filter bandwidth
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_GetTELpfBW( void )
{
    BYTE   regvalue;
    USHORT Tempvalue;

    regvalue = READ_REG(HAL_AFEREG_MAP->FCTS38);
    Tempvalue = e_TELPF_BYPASS;
    if (!(regvalue & HAL_AFE_BPTELP_MASK))
    {
        Tempvalue = (regvalue & (HAL_AFE_TEFCSFSEL_MASK|HAL_AFE_TELPSET_MASK));
    }
    return Tempvalue;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetRfBw

    Description:    Calculate the optimal bandwidth settings of RF noise filter, which
                    are dependent on actual overspeed, BWRatioX100 and disckind.
                    Determines which of RF filter bank 1, 2 or 3 to use and it's settings

    Inputs:         OverspeedX10, is the current overspeed value times 10.
                    BWRatioX100 is the RFEqualizer bandwith over I3 frequency times 100


    Outputs:        none
    Global Inputs:  mDiscKind to determine I3Frequencies
    Global Outputs: none
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_AFE_SetRfBw(USHORT OverspeedX10, USHORT BWRatioX100, BYTE TrackingMethod)
{
    USHORT IxFreqKHz;
    ULONG  RFBandWidthKhz;
    SBYTE  FilterBandWidth;
    BYTE   FilterSelect;
#if (NEW_RFPOS_CALIB == 1)
    SHORT  NewRfagcRef;
    #if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
    SBYTE  NewAocCal;
    #else
    SBYTE  orig_Rfpos;
    BYTE   new_Rfpos;
    static SHORT PreRfagcRef;
    #endif
#endif

    //Find Media (CD/DVD/RAM) and select it's 1X clock value
    switch (mDiscKind.fDiscKind)
    {
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            if (TrackingMethod == TRK_METHOD_DPD)
            {
                IxFreqKHz = DVDRAM_EMBOSSED_BITRATE_N1/(I3CLOCKPERIODS*1000);  // DVD-RAM embossed area
            }
            else
            {
                IxFreqKHz = DVDRAM_BITRATE_N1/(I3CLOCKPERIODS*1000);           // DVD-RAM rewritable area
            }
            break;
#endif

        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
            IxFreqKHz = CD_BITRATE_N1/(I3CLOCKPERIODS*1000);          //Any CD-type
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

            IxFreqKHz = DVD_BITRATE_N1/(I3CLOCKPERIODS*1000);        //Any DVD, except DVDRAM
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            IxFreqKHz = BD_BITRATE_N1/(I2CLOCKPERIODS*1000);
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
    #endif // (ENABLE_CBHD == 1)
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDROMDLDisc:
    #if (ENABLE_CBHD == 1)
        case eCBHDROMDLDisc:
    #endif // (ENABLE_CBHD == 1)
        case eHDRDLDisc:
        case eHDRWDLDisc:
            if ((svoMiscVar.HD_System_leadInOutFlag == HD_SYSTEM_LEAD_IN) ||
                (svoMiscVar.HD_System_leadInOutFlag == HD_SYSTEM_LEAD_OUT) ||
                (svoMiscVar.HD_System_leadInOutFlag == PRESET_HD_SYSTEM_LEAD_IN) ||
                (svoMiscVar.HD_System_leadInOutFlag == PRESET_HD_SYSTEM_LEAD_OUT))
            {
                IxFreqKHz = HDDVD_EMBOSSED_BITRATE_N1/(I2CLOCKPERIODS*1000);
            }
            else
            {
                IxFreqKHz = HDDVD_DATA_BITRATE_N1/(I2CLOCKPERIODS*1000);
            }
            break;
#endif /* (ENABLE_HDVD == 1) */

        case eLSDisc:
        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
            break;
    }
    //send_msgA4(DEBUG_DISK_ID_MSG,0x516430,IxFreqKHz); //RFLPF IxFreqKHz=

    //Calculate RF Bandwith in kHz.
    RFBandWidthKhz = (IxFreqKHz * OverspeedX10 * BWRatioX100)/1000; //suggest all BWRatioX100 should be 0x64 in A0 chip for all disc

    //send_msgA4(DEBUG_DISK_ID_MSG,0x516431,RFBandWidthKhz); //RFLPF Bandwidth=

    if (RFBandWidthKhz > RFB1HIGH * RFB1STEP + RFB1OFF)
    {
        if (RFBandWidthKhz > RFB2HIGH * RFB2STEP + RFB2OFF)
        {
            FilterSelect = halAFE_RFLPFBk_Bank3;
            if (RFBandWidthKhz > RFB3HIGH * RFB3STEP + RFB3OFF)
            {
                FilterBandWidth = RFB3HIGH; // clipping to max. value 0x1A
            }
            else if (RFBandWidthKhz > RFB3OFF)
            {
                FilterBandWidth = (RFBandWidthKhz - RFB3OFF + RFB3STEP/2) / RFB3STEP;
            }
            else
            {
                FilterBandWidth = 0;
            }
        }
        else
        {
            FilterSelect = halAFE_RFLPFBk_Bank2;
            if (RFBandWidthKhz > RFB2OFF)
            {
                FilterBandWidth = (RFBandWidthKhz - RFB2OFF + RFB2STEP/2) / RFB2STEP;
            }
            else
            {
                FilterBandWidth = 0;
            }
        }
    }
    else
    {
        FilterSelect = halAFE_RFLPFBk_Bank1;
        if (RFBandWidthKhz > RFB1OFF)
        {
            FilterBandWidth = (RFBandWidthKhz - RFB1OFF + RFB1STEP/2) / RFB1STEP;
        }
        else
        {
            FilterBandWidth = 0;
        }
    }

    //FilterBandWidth should never be lower than CTFMIN
    if (FilterBandWidth < RFB123MIN)
        FilterBandWidth = RFB123MIN;

    /* use bigger dac value instead of theoretical value to avoid LPF B.W non-linear in bank 3 */
//#if (BD_ENABLE == 1)
//    if((mDiscKind.fDiscKind == eBDROMDisc)&&(FilterSelect == 3))
//    {
//        if(FilterBandWidth >=0x0E)
//        {
//            FilterBandWidth += 3;
//        }
//    }
//#endif

    //send_msgA4(SHOW_DEBUG_MSG,0x516431+FilterSelect,FilterBandWidth); //RFLPF Bandwidth? gate=
    WRITE_FIELD(HAL_AFE_RFHSSEL, FilterSelect);
    WRITE_FIELD(HAL_AFE_RFHSFC, FilterBandWidth);
#if (NEW_RFPOS_CALIB == 1)
    NewRfagcRef = svoRRFVar.rfagc_ref_LPF[FilterSelect - 1][FilterBandWidth] * 10;
    #if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
    if (svoRRFVar.rfagc_ref != NewRfagcRef)
    {
        if (svoRRFVar.rfaoc_ref[Layer_Index] == 0x7FFF)
        {
            svoRRFVar.rfaoc_ref[Layer_Index] = calDacCount2SBYTE(svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1) + NewRfagcRef * 256 / svoRRFVar.step_size[svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain];
        }
        else
        {
            NewAocCal = svoRRFVar.rfaoc_ref[Layer_Index] - NewRfagcRef * 256 / svoRRFVar.step_size[svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain];
            calSetAfeOffset(Layer_Index,RFPGA_OS,TYPE3,0,0,calSBYTE2DacCount(NewAocCal,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1));
            HAL_DFE_SetAgcAocBoundary(COEF_GetValue(DFE_CALC_Max_Dyn_Gain),
                                      COEF_GetValue(DFE_CALC_Min_Dyn_Gain),
                                      COEF_GetValue(DFE_CALC_Coef_Max_dyn_Offset),
                                      COEF_GetValue(DFE_CALC_Coef_Min_dyn_Offset));//20101022_02R7 ==end== replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs
            HAL_DFE_ShiftRfAoc(RoundDivide((svoRRFVar.rfagc_ref - NewRfagcRef) * 256, svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)]), NewAocCal);//20101014_02PL add rounding to RF LPF dc offset conpensation
        }
        svoRRFVar.rfagc_ref = NewRfagcRef;
    }
    #else
    if (PreRfagcRef != NewRfagcRef)
    {
        PreRfagcRef = NewRfagcRef;
        orig_Rfpos = calDacCount2SBYTE(svoCalOfsT3Var[Layer_Index][RFP_OS].normal_offset,6);
        //new_Rfpos = calSBYTE2DacCount(orig_Rfpos - (NewRfagcRef + 1000) * 256 / svoRRFVar.rfpos_step_size,6);//test only
        new_Rfpos = calSBYTE2DacCount(orig_Rfpos - NewRfagcRef * 256 / svoRRFVar.rfpos_step_size,6);
        WRITE_FIELD(HAL_AFE_RFPOS,new_Rfpos);
        NewRfagcRef += ((calDacCount2SBYTE(new_Rfpos,6) - orig_Rfpos) * svoRRFVar.rfpos_step_size / 256);
        svoRRFVar.rfagc_ref = NewRfagcRef;
    }
    #endif
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalDPDBw

    Description:    Calculate the optimal bandwidth settings of DPD equalizer bandwidht fields,
                    which dependend on actual overspeed, BwDPDRatioX100 and disckind.
                    Determines which of RF filter bank 1, 2 or 3 to use and it's settings

    Inputs:         OverspeedX10, is the current overspeed value times 10.
                    BwDpdRatioX100 is the DPD-equalizer center peak freq. setting over I3 freq. times 100
    Outputs:        none
    Global Inputs:  mDiscKind to determine I3Frequencies
    Global Outputs: none
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
struct svoDpdBankFcVarT HAL_AFE_CalDPDBw(USHORT OverspeedX10, USHORT BwDPDRatioX100)
{
    USHORT IxFreqKHz;
    ULONG DPDBandWidthKhz;
    SBYTE FilterBandWidth;
    BYTE FilterSelect;
    BYTE HpfSet;
    struct svoDpdBankFcVarT DpdSetting;

    //Find Media (CD/DVD/RAM) and select it's 1X clock value
    switch (mDiscKind.fDiscKind)
    {
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            IxFreqKHz = DVDRAM_EMBOSSED_BITRATE_N1/(I3CLOCKPERIODS*1000);      //Only DVDRAM type
            HpfSet = halAFE_DPDINHPFBW_p2MHz;
            break;
#endif

        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
            IxFreqKHz = CD_BITRATE_N1/(I3CLOCKPERIODS*1000);          //Any CD-type
            HpfSet = halAFE_DPDINHPFBW_p2MHz;
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
            IxFreqKHz = DVD_BITRATE_N1/(I3CLOCKPERIODS*1000);        //Any DVD, except DVDRAM
            HpfSet = halAFE_DPDINHPFBW_p2MHz;
            break;

#if (BD_ENABLE == 1)
        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            IxFreqKHz = BD_BITRATE_N1/(I2CLOCKPERIODS*1000);
            if (OverspeedX10 > 90)
                HpfSet = halAFE_DPDINHPFBW_6MHz;
            else if (OverspeedX10 > 60)
                HpfSet = halAFE_DPDINHPFBW_4MHz;
            else if (OverspeedX10 > 30)
                HpfSet = halAFE_DPDINHPFBW_2MHz;
            else
                HpfSet = halAFE_DPDINHPFBW_p2MHz;
            break;
#endif

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
            IxFreqKHz = HDDVD_DATA_BITRATE_N1/(I2CLOCKPERIODS*1000);
            if (OverspeedX10 > 50)
                HpfSet = halAFE_DPDINHPFBW_2MHz;
            else
                HpfSet = halAFE_DPDINHPFBW_p2MHz;
            break;
#endif

        case eLSDisc:
        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND);
            break;
    }

    //Calculate DPD Bandwith in kHz.
    /*
     Bugfix bug 5204     DPDBandWidthKhz = (IxFreqKHz * OverspeedX10 * BwDPDRatioX100)/1000;
     Should leave BwDPDRatioX100 at 0x64 in coef table for all discs
     Bank1 filter step is changed to 3000, bank1 start freq. is changed to 10000.
    */
    DPDBandWidthKhz = (IxFreqKHz * OverspeedX10 * BwDPDRatioX100)/1000;

    /*
     Put BwDPDRatioX100 at 0x78 in B0 coef table for all discs
    */
    if (DPDBandWidthKhz < (DPDB1HIGH * DPDB1STEP) + DPDB1OFF)
    {
        if (DPDBandWidthKhz < (DPDB123MIN * DPDB1STEP) + DPDB1OFF)
        {
            FilterBandWidth = DPDB123MIN;
        }
        else
        {
            FilterBandWidth = (DPDBandWidthKhz - DPDB1OFF) / DPDB1STEP + 1;
        }
        FilterSelect = e_DPDHSISCL_B1;
    }
    else if (DPDBandWidthKhz < (DPDB2HIGH * DPDB2STEP) + DPDB2OFF)
    {
        if (DPDBandWidthKhz < (DPDB123MIN * DPDB2STEP) + DPDB2OFF)
        {
            FilterBandWidth = DPDB123MIN;
        }
        else
        {
             FilterBandWidth = (DPDBandWidthKhz - DPDB2OFF) / DPDB2STEP + 1; //[W0K]Shin
        }
#if ((CEProduct == 1) && (PCB == 711))//Only for DVD RW        
        if ((mDiscKind.Bit.DiscStd == eDiscDVD) && (mDiscKind.Bit.Media == eMediaRW) && (FilterBandWidth>3))
        {
            FilterBandWidth = 3;
        }     
#endif        
        FilterSelect = e_DPDHSISCL_B2;
    }
    else
    {
        if (DPDBandWidthKhz < DPDB3HIGH * DPDB3STEP + DPDB3OFF)
        {
            if (DPDBandWidthKhz < (DPDB123MIN * DPDB3STEP) + DPDB3OFF)
            {
                FilterBandWidth = DPDB123MIN;
            }
            else
            {
                FilterBandWidth = (DPDBandWidthKhz - DPDB3OFF) / DPDB3STEP + 1;
            }
        }
        else
        {
            FilterBandWidth = DPDB3HIGH;
        }
        FilterSelect = e_DPDHSISCL_B3;
    }

    DpdSetting.DpdBwBank = FilterSelect;
    DpdSetting.DpdBwFc = FilterBandWidth;
    DpdSetting.DpdHpfBw = HpfSet;

    return DpdSetting;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetDPDBw

    Description:    Calculate the optimal bandwidth settings of DPD equalizer bandwidht fields,
                    which dependend on actual overspeed, BwDPDRatioX100 and disckind.
                    Determines which of RF filter bank 1, 2 or 3 to use and it's settings

    Inputs:         OverspeedX10, is the current overspeed value times 10.
                    BwDpdRatioX100 is the DPD-equalizer center peak freq. setting over I3 freq. times 100
    Outputs:        none
    Global Inputs:  mDiscKind to determine I3Frequencies
    Global Outputs: none
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_AFE_SetDPDBw(USHORT OverspeedX10, USHORT BwDPDRatioX100)
{
    DesriedDpdSetting = HAL_AFE_CalDPDBw(OverspeedX10, BwDPDRatioX100);

    // DPD offsdt calibrate has not been done yet.
    if ( ((svoCalDpdVar[Layer_Index][svoDpdReg2Bank[DesriedDpdSetting.DpdBwBank]][DesriedDpdSetting.DpdBwFc].status & CAL_R_DONE) != CAL_R_DONE) && ((svoCalVar.calGainDoneBits & teg_end) == teg_end) )
    {
        return;
    }
    else
    {
        WRITE_FIELD(HAL_AFE_DPDHSISCL, DesriedDpdSetting.DpdBwBank);

        WRITE_FIELD(HAL_AFE_DPDHSFCH, DesriedDpdSetting.DpdBwFc);
        WRITE_FIELD(HAL_AFE_DPDHSFCL, DesriedDpdSetting.DpdBwFc);

        WRITE_FIELD(HAL_AFE_DPDINFC, DesriedDpdSetting.DpdHpfBw);

#if (DPD_COMPENSATE_OFFSET == 1)
        calSetDPDTEOS(Layer_Index);
#endif//#if (DPD_COMPENSATE_OFFSET == 1)
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetPhBhDrooprate

    Description:    Calculate PHS,BHS,RPPHBH droop rates

    Inputs:         channel is to select PHS,BHS, or RPPHBH
                    Slope is slope of the droop rate in the unit of mV/us

    Outputs:        none
    Global Inputs:  none
    Global Outputs: none
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#define NEGEXP                  15       /* negative shift of exponent defined with BHSDISK = 0 AND WITH phsdisk =0 */

void HAL_AFE_SetPhBhDrooprate(BYTE channel, float Slope)
{
    BYTE csel, disch, dischk, twolog;
    float TempValue,SLOW,FAST;
    BYTE buf;

    switch(channel)
    {
        case e_PHS:
        case e_BHS:
            SLOW = 370;
            FAST = 680;
            break;

        case e_RPPHBH:
            SLOW = 100;
            FAST = 9999999;
            break;
    }

    if (Slope <= SLOW)
    {
        csel  = 1;
        disch = 0;
        twolog = 0;
        TempValue = SLOW / Slope;

        while (TempValue > 1 && twolog < NEGEXP)
        {
            TempValue /= 2;
            twolog++;
        }
        dischk = NEGEXP - twolog;
    }
    else if (Slope > SLOW && Slope <FAST)
    {
        csel = 1;
        dischk = NEGEXP;
        twolog = 0;
        TempValue = Slope / SLOW;

        while (TempValue > 1 && twolog < 3)
        {
            TempValue /= 2;
            twolog++;
        }
        disch = twolog;
    }
    else if (Slope >= FAST)
    {
        csel   = 0;
        dischk = NEGEXP;
        twolog = 0;
        TempValue = Slope / FAST;

        while (TempValue > 1 && twolog < 3)
        {
            TempValue /= 2;
            twolog++;
        }
        disch = twolog;
    }
    //SendMsgCn(SHOW_DEBUG_MSG,4,0x33FF1E,channel,csel,disch,dischk); // droop
    //update registers registerwise (as oposed to fieldwise, therefore
    //use buffer buf to store fields)
    switch(channel)
    {
        case e_PHS:
            buf = READ_REG(HAL_AFEREG_MAP->BSRP21);
            WRITE_FIELDTO(buf, HAL_AFE_PHSCSEL, csel);
            WRITE_FIELDTO(buf, HAL_AFE_PHSDIS, disch);
            WRITE_REG(HAL_AFEREG_MAP->BSRP21, buf);
            WRITE_FIELD(HAL_AFE_PHSDISCK, dischk);
            break;
        case e_BHS:
            buf = READ_REG(HAL_AFEREG_MAP->BSRP21);
            WRITE_FIELDTO(buf, HAL_AFE_BHSCSEL, csel);
            WRITE_FIELDTO(buf, HAL_AFE_BHSDIS, disch);
            WRITE_REG(HAL_AFEREG_MAP->BSRP21, buf);
            WRITE_FIELD(HAL_AFE_BHSDISCK, dischk);
            break;
        case e_RPPHBH:
            WRITE_FIELD(HAL_AFE_RCDSCKOF,0);
            buf = READ_REG(HAL_AFEREG_MAP->BSRP27);
            WRITE_FIELDTO(buf, HAL_AFE_RCDIS, disch);
            WRITE_FIELDTO(buf, HAL_AFE_RCDISCK, dischk);
            WRITE_REG(HAL_AFEREG_MAP->BSRP27, buf);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetWobbleSpeed

    Description:    Calculate AFE Wobble channel LPF and HPFs on basis of
                    overspeed, media and user bandwidth parameters

    Inputs:         OverspeedX10, WlpfRatioX10, WhpfRatioX100

    Outputs:        none
    Global Inputs:  none
    Global Outputs: none
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_AFE_SetWobbleSpeed(USHORT OverspeedX10, USHORT WlpfRatioX10, USHORT WhpfRatioX100)
{
    union                    // use union to make explicitely clear that in some instances we mean the
    {                        // wobble signal and in other cases we mean PIC signal (BDRE in PIC area) !!!
        USHORT WFreq;
        USHORT PICHFreq;
    } tmp;

    USHORT WobAgcTimeUs;
    USHORT NrWobble1Cm;
#if (DVD_RAM_READ == 1)
    USHORT HFreq;
#endif
    USHORT PFreq;
#if BD_ENABLE
    USHORT PICLFreq;
#endif
    USHORT WobHpf;
    LONG   WobLpf;

    BYTE   WobHpfDfVal, WobLpfEnVal, WobAgcVal;
    BYTE   WobLpfVal, WobHpfVal;

    WobHpfDfVal=0;
    WobLpfEnVal=1;
    switch (mDiscKind.fDiscKind)
    {
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            NrWobble1Cm = DVDRAM1CM;
            WobHpfDfVal = 1;
            tmp.WFreq = (DVDRAM_BITRATE_N1/1000) / DVDRAMWOBN;                      //Wobble freq 1X in KHz
            HFreq = (((DVDRAM_BITRATE_N1/1000) / I3PERIODS)* OverspeedX10)/10;      //Header freq as I3 freq in KHz
            break;
#endif

        case eCDRDisc:
        case eCDRWDisc:
            NrWobble1Cm = CD1CM;
            tmp.WFreq = (CD_BITRATE_N1/1000) / CDWOBN;
            break;

        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDLDisc:
            NrWobble1Cm = DVDP1CM;
            tmp.WFreq = (DVD_BITRATE_N1/1000) / DVDPWOBN;
            break;

        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
            NrWobble1Cm = DVDM1CM;
            tmp.WFreq = (DVD_BITRATE_N1/1000) / DVDMWOBN;    //Wobble freq in KHz
            PFreq = (((DVD_BITRATE_N1/1000) / I3PERIODS)*OverspeedX10)/10;    //Prepit freq as I3 freq in KHz
            break;

#if (BD_ENABLE == 1)
        case eBDRDisc:
        case eBDREDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            NrWobble1Cm = BD1CM;
            tmp.WFreq = (BD_BITRATE_N1/1000) / BDWOBN;
            if (BDR_RE_PICReading == SET)
            {
                //BD_PIC area
                NrWobble1Cm = BD1CM;
                tmp.PICHFreq = (HARM3RD * (BD_BITRATE_N1/1000)) / BDPICN;          /* 3rd harm. of highest PIC mod. freq */
                PICLFreq = ((((BD_BITRATE_N1/1000) / BDPICN)/10)*OverspeedX10)/10; /* 10% of highest */
                break;

            }
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDRDLDisc:
        case eHDRWDLDisc:
            NrWobble1Cm = HD1CM;
            tmp.WFreq = (HDDVD_DATA_BITRATE_N1/1000) / HDWOBN;
            break;
#endif /* (ENABLE_HDVD == 1) */

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
            return;
    }

    //Calculate actual wobble freq for all disckinds, depending on given overspeed
    tmp.WFreq = (tmp.WFreq * OverspeedX10)/10;  //Freq in KHz. Always within USHORT
    if (tmp.WFreq > 0)
    {
        WobAgcTimeUs = (1000 * NrWobble1Cm)/ tmp.WFreq ;
    }
    else
    {
        WobAgcTimeUs = 1000;
    }

    //Highpass filter depends always on WobbleFreq
#if (BD_ENABLE == 1)
    if (BDR_RE_PICReading == SET)
    {
        WobHpf = (PICLFreq * WhpfRatioX100)/100;  //WopHpf in KHz
    }
    else
#endif
    {
        WobHpf = (tmp.WFreq * WhpfRatioX100)/100;   //WopHpf in KHz
    }

    //Lowpass filter depends on Header OR Prepit, when DVDRam OR DVD Minus
    switch (mDiscKind.fDiscKind)
    {
#if (DVD_RAM_READ == 1)
        case eDVDRAMDisc:
            WobLpf = (HFreq * WlpfRatioX10)/10;    //Wobble LPF in KHz
            break;
#endif

        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDLDisc:
            WobLpf = (PFreq * WlpfRatioX10)/10;     //Wobble LPF in KHz
            break;

        default://Other disckinds depend on wobble
#if (BD_ENABLE == 1)
            if (BDR_RE_PICReading == SET)
            {
                WobLpf = (tmp.PICHFreq * WlpfRatioX10)/10;
            }
            else
#endif
            {
                WobLpf = (tmp.WFreq * WlpfRatioX10)/10;
            }
            break;
    }

    WobLpfVal = (WobLpf - LPFOFF + LPFSTEP)/LPFSTEP;

    if (WobLpfVal > WOBLPFMAX)
    {
        WobLpfVal = WOBLPFMAX;
        WobLpfEnVal = 0;
    }
#if (PCB == 812)
    if ((mDiscKind.fDiscKind == eBDREDLDisc) && (OverspeedX10 <= 20))
        WobLpfVal=0;
#endif

    //Now Use calculated values to set registers correct.
    if (WobHpf>1500)
        WobHpfVal = halAFE_AGCHPF_1500kHz6;
    else if (WobHpf>1000)
        WobHpfVal = halAFE_AGCHPF_1000kHz;
    else if (WobHpf>700)
        WobHpfVal = halAFE_AGCHPF_700kHz;
    else if (WobHpf>400)
        WobHpfVal = halAFE_AGCHPF_400kHz;
    else if (WobHpf>200)
        WobHpfVal = halAFE_AGCHPF_200kHz;
    else
        WobHpfVal = halAFE_AGCHPF_100kHz;


    if (WobAgcTimeUs>500)
        WobAgcVal = halAFE_AGCAmptdSetlTime_0p5ms;
    else if (WobAgcTimeUs>250)
        WobAgcVal = halAFE_AGCAmptdSetlTime_0p25ms;
    else if (WobAgcTimeUs>60)
        WobAgcVal = halAFE_AGCAmptdSetlTime_0p06ms;
    else
        WobAgcVal = halAFE_AGCAmptdSetlTime_0p03ms;

    WRITE_FIELD(HAL_AFE_AGCHPFDF, WobHpfDfVal );
    WRITE_FIELD(HAL_AFE_AGCHPFFC, WobHpfVal );
    WRITE_FIELD(HAL_AFE_WOBLPFEN, WobLpfEnVal);
    WRITE_FIELD(HAL_AFE_WOBLPFBW, WobLpfVal);
    WRITE_FIELD(HAL_AFE_AGCIOUT, WobAgcVal);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_SetMuxToAdc

    Description:    Configur multiplexers MUX1..3

    Inputs:         Signal describes the multiplexer input signal
    Outputs:        None
    Global Inputs:  None
    Global Outputs: Many AFE Registers
    Returns:        The ADC input signal
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_SetMuxToAdc(HAL_AFE_MuxSignalT Signal)
{
    BYTE AdcInput;

    switch(Signal)
    {
        case HAL_AFE_FE:
            AdcInput = ADC_FE;
            break;

        case HAL_AFE_BS:
            AdcInput = ADC_BS;
            break;

        case HAL_AFE_TE:
            AdcInput = ADC_TE;
            break;

        case HAL_AFE_BHSO:
            AdcInput = ADC_BHSO;
            break;

        case HAL_AFE_RP:
            AdcInput = ADC_RP;
            break;

        case HAL_AFE_SBAD:
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SBAD);
            AdcInput = ADC_MUXOUT2;
            break;

        case HAL_AFE_CE:
            AdcInput = ADC_CE;
            break;

        case HAL_AFE_SRFO:
            WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
            AdcInput = ADC_MUXOUT1;
            break;

        default:
            DEBUG_ASSERT(FALSE, eASSERT_FUNCTION_NOT_IMPLEMENTED);
            break;
    }

    return(AdcInput);
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalcMbInpGain()

    Description:    Calculating the Main Beam Input Gain (MBGUPSEL[4:0])

    Inputs:         MainBeamTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        MBGUPSEL
    Global Inputs:  None
    Global Outputs: None
    Returns:        SBYTE
-----------------------------------------------------------------------------------------------------*/
SBYTE HAL_AFE_CalcMbInpGain(USHORT MainBeamTargetX1000,
                            USHORT SRFOPeakX1000,
                            SHORT  SRFOSTMGaindBX10)
{
    SBYTE     TempValuedB;

    /* Calculate the logarithmic Main beam input gain in [dB], MainBeamInputTargetGaindB */
    TempValuedB = RoundDivide(CalculateXdBFromRate(P10dB,(4 * MainBeamTargetX1000), SRFOPeakX1000) + SRFOSTMGaindBX10, 10);

    /* Calculate the Main beam input gain, which must be written to MBGUPSEL[4:0] */
    return (TempValuedB + 0x10);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalcDPDInpGain()

    Description:    Calculating the DPD Input Gain (DPDGN[3:0])

    Inputs:         DPDTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        DPDGN
    Global Inputs:  None
    Global Outputs: None
    Returns:        SBYTE
-----------------------------------------------------------------------------------------------------*/
SBYTE HAL_AFE_CalcDPDInpGain(USHORT DPDTargetX1000,
                             USHORT SRFOPeakX1000,
                             SHORT  SRFOSTMGaindBX10)
{
    SHORT     DPDInputTargetGaindBX10;

    /* Calculate the logarithmic target gain in [dB], DPDInputTargetGaindBX2 */
    DPDInputTargetGaindBX10 = CalculateXdBFromRate(P10dB,(4 * DPDTargetX1000), SRFOPeakX1000) + SRFOSTMGaindBX10;

    /* Calculate the bit value, DPDGN[3:0], of DPDPGAGain */
    return (RoundDivide(DPDInputTargetGaindBX10,15) + halAFE_DPDGN_0dB);//divided by 15 due to 1.5 dB per step.
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalcWobbleInpGain()

    Description:    Calculating the Wobble Input Gain
                    (RADGNB[1:0], RADPGNB[1:0], RBCGNB[1:0], RBCPGNB[1:0])

    Inputs:         WBLTargetX1000, SRFOPeakX1000, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_CalcWobbleInpGain(USHORT WBLTargetX1000,
                               USHORT SRFOPeakX1000,
                               SHORT  SRFOSTMGaindBX10)
{
    BYTE      WBLReadPathGain, WBLReadPathGainDiffX10, WBLReadPathPreGain;
    SHORT     WBLInputTargetGaindBX10;

    /* Calculate the logarithmic target gain in [0.1dB], WBLInputTargetGaindBX10 */
    WBLInputTargetGaindBX10 = CalculateXdBFromRate(P10dB,(2 * WBLTargetX1000), SRFOPeakX1000) + SRFOSTMGaindBX10;

    /* Calculate the bit value, RADGNB[1:0], of WBLReadPathADGain */
    /* It is also checked if the value is not < 0 or > 3 */
    if (WBLInputTargetGaindBX10 < 0)
    {
        WBLInputTargetGaindBX10 = 0;
        WBLReadPathGain = 0;
    }
    else if (WBLInputTargetGaindBX10 > (3 * 40))
    {
        WBLReadPathGain = 3;
    }
    else
    {
        WBLReadPathGain = WBLInputTargetGaindBX10 / 40;
    }

    /* Calculating the value for wobble read path AD pre-gain, RADPGNB[1:0] */
    WBLReadPathGainDiffX10 = WBLInputTargetGaindBX10 - (40 * WBLReadPathGain);

    /* Calculate the bit value, RADPGNB[1:0], of WBLReadPathADPreGain */
    /* It is also checked if the value is not < 0 or > 3 */
    if (WBLReadPathGainDiffX10 > 3 * 20)
    {
        WBLReadPathPreGain = 3;
    }
    else
    {
        WBLReadPathPreGain = (WBLReadPathGainDiffX10 + 10) / 20;
    }

    return ((WBLReadPathGain << 4) | WBLReadPathPreGain);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_GetRfGaindBX10()

    Description:    Calculating the RF gain in 0.1dB

    Inputs:         RFINGN, RFGN
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        RfGaindBX10
-----------------------------------------------------------------------------------------------------*/
SHORT HAL_AFE_GetRfGaindBX10(BYTE rfingn, BYTE rfgn)
{
    SHORT RfGaindBX10;

    RfGaindBX10 = 0;
    switch (rfingn)
    {
        case 0:
            RfGaindBX10 = RFINGN_0_dBX10;
            break;

        case 1:
            RfGaindBX10 = RFINGN_1_dBX10;
            break;

        case 2:
            RfGaindBX10 = RFINGN_2_dBX10;
            break;

        case 3:
            RfGaindBX10 = RFINGN_3_dBX10;
            break;
    }

    switch (rfgn)
    {
#if (CHIP_REV >= 0xC0)
        case halAFE_RFGNGROV_N9dB:
            RfGaindBX10 += RFGN_0_dBX10;
            break;

        case halAFE_RFGNGROV_N6dB:
            RfGaindBX10 += RFGN_1_dBX10;
            break;

        case halAFE_RFGNGROV_N3dB:
            RfGaindBX10 += RFGN_2_dBX10;
            break;

        case halAFE_RFGNGROV_0dB:
            RfGaindBX10 += RFGN_3_dBX10;
            break;

        case halAFE_RFGNGROV_3dB:
            RfGaindBX10 += RFGN_4_dBX10;
            break;

        case halAFE_RFGNGROV_6dB:
            RfGaindBX10 += RFGN_5_dBX10;
            break;

        case halAFE_RFGNGROV_9dB:
            RfGaindBX10 += RFGN_6_dBX10;
            break;

        case halAFE_RFGNGROV_12dB:
            RfGaindBX10 += RFGN_7_dBX10;
            break;
#else
        case halAFE_RFGNGROV_N6dB:
            RfGaindBX10 += RFGN_0_dBX10;
            break;

        case halAFE_RFGNGROV_0dB:
            RfGaindBX10 += RFGN_1_dBX10;
            break;

        case halAFE_RFGNGROV_6dB:
            RfGaindBX10 += RFGN_2_dBX10;
            break;

        case halAFE_RFGNGROV_12dB:
            RfGaindBX10 += RFGN_3_dBX10;
            break;
#endif
    }
    return RfGaindBX10;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalcRFGain()

    Description:    Calculating the RF Coarse gain setting for the input Rfgn_dBX10

    Inputs:         Rfgn_dBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        RfGain
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_CalcRFGain(SHORT Rfgn_dBX10)
{
    BYTE RFGain;

    if (Rfgn_dBX10 < ((RFGN_0_dBX10 + RFGN_1_dBX10) / 2))  // gain mid between RFPgaGain=0 to 1, in 0.1dB units
    {
        RFGain = 0;
    }
    else if (Rfgn_dBX10 <= ((RFGN_1_dBX10 + RFGN_2_dBX10) / 2)) // gain mid between RFPgaGain=1 to 2, in 0.1dB units
    {
        RFGain = 1;
    }
    else if (Rfgn_dBX10 <= ((RFGN_2_dBX10 + RFGN_3_dBX10) / 2)) // gain mid between RFPgaGain=2 to 3, in 0.1dB units
    {
        RFGain = 2;
    }
#if (CHIP_REV >= 0xC0)
    else if (Rfgn_dBX10 <= ((RFGN_3_dBX10 + RFGN_4_dBX10) / 2)) // gain mid between RFPgaGain=3 to 4, in 0.1dB units
    {
        RFGain = 3;
    }
    else if (Rfgn_dBX10 <= ((RFGN_4_dBX10 + RFGN_5_dBX10) / 2)) // gain mid between RFPgaGain=4 to 5, in 0.1dB units
    {
        RFGain = 4;
    }
    else if (Rfgn_dBX10 <= ((RFGN_5_dBX10 + RFGN_6_dBX10) / 2)) // gain mid between RFPgaGain=5 to 6, in 0.1dB units
    {
        RFGain = 5;
    }
    else if (Rfgn_dBX10 <= ((RFGN_6_dBX10 + RFGN_7_dBX10) / 2)) // gain mid between RFPgaGain=6 to 7, in 0.1dB units
    {
        RFGain = 6;
    }
    else
    {
        RFGain = 7;
    }
#else
    else
    {
        RFGain = 3;
    }
#endif
    return RFGain;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_AFE_CalcRFInpGain()

    Description:    Calculating the RF input 5V buffer gain and RF Coarse gain setting for Groove data fields (RFGN)

    Inputs:         RFSRFOTargetX1000, SRFOPeakX1000L0, SRFOPeakX1000L1, SRFOSTMGaindBX10
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        RFINGN(7:6), RFGNO_L1(5:3), RFGNO_L0(2:0),
-----------------------------------------------------------------------------------------------------*/
BYTE HAL_AFE_CalcRFInpGain (USHORT RFSRFOTargetX1000,
                            USHORT SRFOPeakX1000L0,
                            USHORT SRFOPeakX1000L1,
                            SHORT  SRFOSTMGaindBX10)
{
    BYTE      Rfingn_Rfgno;
    SHORT     RFTargetGaindBX10,Rfingn_dBX10;

    /* Calculate the logarithmic gain RFPGATargetGaindBX10 (0.1dB) */
    RFTargetGaindBX10 = CalculateXdBFromRate(P10dB,RFSRFOTargetX1000, ((SRFOPeakX1000L0>SRFOPeakX1000L1)? SRFOPeakX1000L0 : SRFOPeakX1000L1)) + SRFOSTMGaindBX10;

    //select RFIN_GN
    if (RFTargetGaindBX10 < ((RFINGN_0_dBX10 + RFINGN_1_dBX10) / 2)) // threshold gain between RFINGN=0 and 1, in 0.1dB units
    {
        Rfingn_Rfgno = 0;
    }
    else if (RFTargetGaindBX10 < ((RFINGN_1_dBX10 + RFINGN_2_dBX10) / 2)) // threshold gain between RFINGN=1 to 2, in 0.1dB units
    {
        Rfingn_Rfgno = 1;
    }
    else if (RFTargetGaindBX10 < ((RFINGN_2_dBX10 + RFINGN_3_dBX10) / 2)) // threshold gain between RFINGN=2 to 3, in 0.1dB units
    {
        Rfingn_Rfgno = 2;
    }
    else
    {
        Rfingn_Rfgno = 3;
    }
    Rfingn_dBX10 = HAL_AFE_GetRfGaindBX10(Rfingn_Rfgno,0xFF);
    Rfingn_Rfgno = (Rfingn_Rfgno << 6);

    //select RF_GN
    //L0
    RFTargetGaindBX10 = CalculateXdBFromRate(P10dB,RFSRFOTargetX1000, SRFOPeakX1000L0) + SRFOSTMGaindBX10 - Rfingn_dBX10;
    Rfingn_Rfgno |= HAL_AFE_CalcRFGain(RFTargetGaindBX10);

    //L1
    if (SRFOPeakX1000L1 > 0)
    {
        RFTargetGaindBX10 = CalculateXdBFromRate(P10dB,RFSRFOTargetX1000, SRFOPeakX1000L1) + SRFOSTMGaindBX10 - Rfingn_dBX10;
        Rfingn_Rfgno |= (HAL_AFE_CalcRFGain(RFTargetGaindBX10) << 3);
    }
    else
    {
        Rfingn_Rfgno |= ((Rfingn_Rfgno & 0x07) << 3);
    }

    return Rfingn_Rfgno;
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
