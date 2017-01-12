/******************************************************************************
*
*     AUTOMATICALLY GENERATED H FILE  ---  DO NOT EDIT
*   ====================================================
*
*
******************************************************************************/

#ifndef _COEF_VALUES_H_
#define _COEF_VALUES_H_

// Excel-sheet version and date, and code-generation date:

#define  MACRO_NAME            "excel_to_c_macro.xls"
#define  MACRO_VERSION         "NS_1.07"
#define  COEFS_FILE            "c6230_C1_711_CE_SFBD414_coef.xls"
#define  COEFS_FILE_VERSION    "c6230_C1_711_CE_SFBD414_coef"
#define  COEFS_FILE_DATE       "2011/3/29 ¤U¤È 08:12:09 "
#define  COEFS_CODE_GENERATED  "2011/4/18 ¤U¤È 07:10:06"
//       ChipVersion           "C1"
//       AFE_XML_Version       "7.42"
//       DFE_XML_Version       "1.46"
//       Servo_XML_Version     "7.95"
//       ServoDSP_XML_Version  "1.13"
//       Wobble_XML_Version    "1.43"

/******************************************************************************
*  INCLUDE FILES
******************************************************************************/

#include "..\common\globtype.h"
#include "..\al\reg_hal.h"
#include "..\al\reg_afe.h"
#include "..\al\reg_dfe.h"
#include "..\al\reg_servo.h"
#include "..\al\reg_servodsp.h"
#include "..\al\reg_wobble.h"

// Supported speeds:
// Single enum containing separate speeds defines for each Disc Type; all enums start from 0

#define NUMBER_OF_CD_DISC_SPEEDS        12 
#define NUMBER_OF_DVD_DISC_SPEEDS       10 
#define NUMBER_OF_DVDRAM_DISC_SPEEDS    8 
#define NUMBER_OF_BD_DISC_SPEEDS        9 


typedef enum
{
   CD_4xCLV    =   0,
   CD_8xCLV,
   CD_12xCLV,
   CD_16xCLV,
   CD_10xCAV,
   CD_16xCAV,
   CD_24xCAV,
   CD_32xCAV,
   CD_40xCAV,
   CD_48xCAV,
   CD_52xCAV,
   CD_54xCAV,

   DVD_2xCLV    =   CD_4xCLV,
   DVD_3xCLV,
   DVD_4xCLV,
   DVD_4xCAV,
   DVD_6xCAV,
   DVD_8xCAV,
   DVD_10xCAV,
   DVD_12xCAV,
   DVD_16xCAV,
   DVD_20xCAV,

   DVDRAM_1xCLV    =   CD_4xCLV,
   DVDRAM_2xCLV,
   DVDRAM_3xCLV,
   DVDRAM_5xCLV,
   DVDRAM_5xCAV,
   DVDRAM_8xCAV,
   DVDRAM_12xCAV,
   DVDRAM_16xCAV,

   BD_1xCLV    =   CD_4xCLV,
   BD_2xCLV,
   BD_3xCLV,
   BD_4xCAV,
   BD_6xCAV,
   BD_8xCAV,
   BD_10xCAV,
   BD_12xCAV,
   BD_14xCAV,

   ILLEGAL_SPEED = 0xFF // make it large enough

}  COEFS_SPEED_t;


// Supported CD media types:

#define NUMBER_OF_CD_DISC_KINDS  3 

// Supported DVD media types:

#define NUMBER_OF_DVD_DISC_KINDS  10 

// Supported DVDRAM media types:

#define NUMBER_OF_DVDRAM_DISC_KINDS  1 

// Supported BD media types:

#define NUMBER_OF_BD_DISC_KINDS  6 


// List of Supported media types:

typedef enum
{
   e_CD_ROM,
   e_CD_R,
   e_CD_RW,

   e_DVD_ROM,
   e_DVD_pR,
   e_DVD_pRW,
   e_DVD_mR,
   e_DVD_mRW,
   e_DVD_DL,
   e_DVD_pR_DL,
   e_DVD_pRW_DL,
   e_DVD_mR_DL,
   e_DVD_mRW_DL,

   e_DVDRAM_RAM,

   e_BD_RO_SL,
   e_BD_RO_DL,
   e_BD_R_SL,
   e_BD_R_DL,
   e_BD_RE_SL,
   e_BD_RE_DL,

   e_ILLEGAL_MEDIUM = 0xFF // make it large enough

} COEFS_MEDIA_t;

#define LAST_MEDIA                     e_BD_RE_DL + 1
#define OFFSET_CD_DISCGROUP            e_CD_ROM
#define OFFSET_DVD_DISCGROUP           e_DVD_ROM
#define OFFSET_DVDRAM_DISCGROUP        e_DVDRAM_RAM
#define OFFSET_BD_DISCGROUP            e_BD_RO_SL


// Defines for the 1st and last entries for the coefs in a sheet
// (note that several sheets can belong to the same enum type)

#define  AFE_INIT_FIRST_ELEMENT                       AFE_INIT_MUX03_MX3TOGP1
#define  AFE_INIT_LAST_ELEMENT                        AFE_INIT_WOBAPC01
#define  AFE_INIT_FIRST_SPECIFIC_FIELD                NR_OF_COEFS
#define  AFE_INIT_LAST_SPECIFIC_FIELD                 (NR_OF_COEFS-1)
#define  AFE_INIT_FIRST_SPECIFIC_REG                  NR_OF_COEFS
#define  AFE_INIT_LAST_SPECIFIC_REG                   (NR_OF_COEFS-1)
#define  AFE_INIT_FIRST_GENERIC_FIELD                 AFE_INIT_MUX03_MX3TOGP1
#define  AFE_INIT_LAST_GENERIC_FIELD                  AFE_INIT_WOBAPC20_APCINSEL
#define  AFE_INIT_FIRST_GENERIC_REG                   AFE_INIT_WOBAPC24
#define  AFE_INIT_LAST_GENERIC_REG                    AFE_INIT_WOBAPC01
#define  AFE_FIXED_FIRST_ELEMENT                      AFE_CALC_DVDRW_BLANK_SRFO_TH
#define  AFE_FIXED_LAST_ELEMENT                       AFE_REFS00_PDSVOMUX
#define  AFE_FIXED_FIRST_SPECIFIC_FIELD               AFE_CALC_DVDRW_BLANK_SRFO_TH
#define  AFE_FIXED_LAST_SPECIFIC_FIELD                AFE_CALC_STM_NO_DISC_TH
#define  AFE_FIXED_FIRST_SPECIFIC_REG                 AFE_WOBAPC15_BDROMMARK
#define  AFE_FIXED_LAST_SPECIFIC_REG                  AFE_WOBAPC25_BDROMMARK
#define  AFE_FIXED_FIRST_GENERIC_FIELD                AFE_RF00_PDRFALL
#define  AFE_FIXED_LAST_GENERIC_FIELD                 AFE_REFS00_PDSVOMUX
#define  AFE_FIXED_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  AFE_FIXED_LAST_GENERIC_REG                   (NR_OF_COEFS-1)
#define  AFE_DT_FIRST_ELEMENT                         AFE_FCTS37_TEBG
#define  AFE_DT_LAST_ELEMENT                          AFE_BSRP10
#define  AFE_DT_FIRST_SPECIFIC_FIELD                  AFE_FCTS37_TEBG
#define  AFE_DT_LAST_SPECIFIC_FIELD                   AFE_FCTS35_CEBG12DB_EMBOSSED
#define  AFE_DT_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  AFE_DT_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  AFE_DT_FIRST_GENERIC_FIELD                   AFE_RF03_VCMADEN
#define  AFE_DT_LAST_GENERIC_FIELD                    AFE_WOBAPC24_PDWOB
#define  AFE_DT_FIRST_GENERIC_REG                     AFE_MB17
#define  AFE_DT_LAST_GENERIC_REG                      AFE_BSRP10
#define  AFE_DK_FIRST_ELEMENT                         AFE_CALC_BwRfRatioX100
#define  AFE_DK_LAST_ELEMENT                          AFE_WOBAPC25_WBVGA3REG
#define  AFE_DK_FIRST_SPECIFIC_FIELD                  AFE_CALC_BwRfRatioX100
#define  AFE_DK_LAST_SPECIFIC_FIELD                   AFE_CALC_TE_BLANK_GAINUP
#define  AFE_DK_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  AFE_DK_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  AFE_DK_FIRST_GENERIC_FIELD                   AFE_RF00_ENRFRP
#define  AFE_DK_LAST_GENERIC_FIELD                    AFE_WOBAPC25_WBVGA3REG
#define  AFE_DK_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  AFE_DK_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  AFE_SPD_FIRST_ELEMENT                        AFE_FCTS32_FEFCSFSEL
#define  AFE_SPD_LAST_ELEMENT                         AFE_BSRP14_RPLPSET
#define  AFE_SPD_FIRST_SPECIFIC_FIELD                 NR_OF_COEFS
#define  AFE_SPD_LAST_SPECIFIC_FIELD                  (NR_OF_COEFS-1)
#define  AFE_SPD_FIRST_SPECIFIC_REG                   NR_OF_COEFS
#define  AFE_SPD_LAST_SPECIFIC_REG                    (NR_OF_COEFS-1)
#define  AFE_SPD_FIRST_GENERIC_FIELD                  AFE_FCTS32_FEFCSFSEL
#define  AFE_SPD_LAST_GENERIC_FIELD                   AFE_BSRP14_RPLPSET
#define  AFE_SPD_FIRST_GENERIC_REG                    NR_OF_COEFS
#define  AFE_SPD_LAST_GENERIC_REG                     (NR_OF_COEFS-1)
#define  AFE_DS_FIRST_ELEMENT                         AFE_WOBAPC00_WSHNOLPON
#define  AFE_DS_LAST_ELEMENT                          AFE_WOBAPC01_RSHFPDON
#define  AFE_DS_FIRST_SPECIFIC_FIELD                  NR_OF_COEFS
#define  AFE_DS_LAST_SPECIFIC_FIELD                   (NR_OF_COEFS-1)
#define  AFE_DS_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  AFE_DS_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  AFE_DS_FIRST_GENERIC_FIELD                   AFE_WOBAPC00_WSHNOLPON
#define  AFE_DS_LAST_GENERIC_FIELD                    AFE_WOBAPC01_RSHFPDON
#define  AFE_DS_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  AFE_DS_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  DFE_INIT_FIRST_ELEMENT                       DFE_INIT_HFAMPMCFG_PDATTTYP_HEADER
#define  DFE_INIT_LAST_ELEMENT                        DFE_INIT_EWHDDTCFG_EWHDDEN
#define  DFE_INIT_FIRST_SPECIFIC_FIELD                DFE_INIT_HFAMPMCFG_PDATTTYP_HEADER
#define  DFE_INIT_LAST_SPECIFIC_FIELD                 DFE_INIT_HFAMPMCFG_PDDFBW_HEADER
#define  DFE_INIT_FIRST_SPECIFIC_REG                  NR_OF_COEFS
#define  DFE_INIT_LAST_SPECIFIC_REG                   (NR_OF_COEFS-1)
#define  DFE_INIT_FIRST_GENERIC_FIELD                 DFE_INIT_DFEADCCFG_HFADCINV
#define  DFE_INIT_LAST_GENERIC_FIELD                  DFE_INIT_EWHDDTCFG_EWHDDEN
#define  DFE_INIT_FIRST_GENERIC_REG                   NR_OF_COEFS
#define  DFE_INIT_LAST_GENERIC_REG                    (NR_OF_COEFS-1)
#define  DFE_FIXED_FIRST_ELEMENT                      DFE_DFEADCCFG_HFADCINV
#define  DFE_FIXED_LAST_ELEMENT                       DFE_HFBITDCFG_VITRLHEADR
#define  DFE_FIXED_FIRST_SPECIFIC_FIELD               DFE_DFEADCCFG_HFADCINV
#define  DFE_FIXED_LAST_SPECIFIC_FIELD                DFE_HFAOCCFG_OFFSETINV
#define  DFE_FIXED_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  DFE_FIXED_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  DFE_FIXED_FIRST_GENERIC_FIELD                DFE_HFAGCCFG1_SLOWAGC
#define  DFE_FIXED_LAST_GENERIC_FIELD                 DFE_HFBITDCFG_VITRLHEADR
#define  DFE_FIXED_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  DFE_FIXED_LAST_GENERIC_REG                   (NR_OF_COEFS-1)
#define  DFE_DT_FIRST_ELEMENT                         DFE_BCACFG_BCAFREQ
#define  DFE_DT_LAST_ELEMENT                          DFE_HFAMPMCFG
#define  DFE_DT_FIRST_SPECIFIC_FIELD                  DFE_BCACFG_BCAFREQ
#define  DFE_DT_LAST_SPECIFIC_FIELD                   DFE_HFSLCCFG_SLCDRCFG_EMBOSSED
#define  DFE_DT_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  DFE_DT_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  DFE_DT_FIRST_GENERIC_FIELD                   DFE_HFAGCCFG1_AGCWINPRD
#define  DFE_DT_LAST_GENERIC_FIELD                    DFE_EWHDDTCFG_EWHDATTTY
#define  DFE_DT_FIRST_GENERIC_REG                     DFE_HFAMPMCFG
#define  DFE_DT_LAST_GENERIC_REG                      DFE_HFAMPMCFG
#define  DFE_DK_FIRST_ELEMENT                         DFE_CALC_Max_Dyn_Gain
#define  DFE_DK_LAST_ELEMENT                          DFE_HFHLDDCFG
#define  DFE_DK_FIRST_SPECIFIC_FIELD                  DFE_CALC_Max_Dyn_Gain
#define  DFE_DK_LAST_SPECIFIC_FIELD                   DFE_WHDDETCFG_WHDINVHFI
#define  DFE_DK_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  DFE_DK_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  DFE_DK_FIRST_GENERIC_FIELD                   DFE_HFDETCFG1_LTHRPA
#define  DFE_DK_LAST_GENERIC_FIELD                    DFE_EWHDDTCFG_EWHDUTHR
#define  DFE_DK_FIRST_GENERIC_REG                     DFE_HFHLDDCFG
#define  DFE_DK_LAST_GENERIC_REG                      DFE_HFHLDDCFG
#define  DFE_SPD_FIRST_ELEMENT                        NR_OF_COEFS
#define  DFE_SPD_LAST_ELEMENT                         (NR_OF_COEFS-1)
#define  DFE_SPD_FIRST_SPECIFIC_FIELD                 NR_OF_COEFS
#define  DFE_SPD_LAST_SPECIFIC_FIELD                  (NR_OF_COEFS-1)
#define  DFE_SPD_FIRST_SPECIFIC_REG                   NR_OF_COEFS
#define  DFE_SPD_LAST_SPECIFIC_REG                    (NR_OF_COEFS-1)
#define  DFE_SPD_FIRST_GENERIC_FIELD                  NR_OF_COEFS
#define  DFE_SPD_LAST_GENERIC_FIELD                   (NR_OF_COEFS-1)
#define  DFE_SPD_FIRST_GENERIC_REG                    NR_OF_COEFS
#define  DFE_SPD_LAST_GENERIC_REG                     (NR_OF_COEFS-1)
#define  DFE_DS_FIRST_ELEMENT                         DFE_CALC_EQTAPA1_Low
#define  DFE_DS_LAST_ELEMENT                          DFE_HFFPLLCFG_FPLLKICFG
#define  DFE_DS_FIRST_SPECIFIC_FIELD                  DFE_CALC_EQTAPA1_Low
#define  DFE_DS_LAST_SPECIFIC_FIELD                   DFE_CALC_EQTAPA2_High
#define  DFE_DS_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  DFE_DS_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  DFE_DS_FIRST_GENERIC_FIELD                   DFE_HFFPLLCFG_FPLLKPCFG
#define  DFE_DS_LAST_GENERIC_FIELD                    DFE_HFFPLLCFG_FPLLKICFG
#define  DFE_DS_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  DFE_DS_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  WBL_INIT_FIRST_ELEMENT                       WBL_INIT_WBLAGCCFG_WBLAGCON
#define  WBL_INIT_LAST_ELEMENT                        WBL_INIT_BDTIMCFG1_BDCFLKCHK
#define  WBL_INIT_FIRST_SPECIFIC_FIELD                NR_OF_COEFS
#define  WBL_INIT_LAST_SPECIFIC_FIELD                 (NR_OF_COEFS-1)
#define  WBL_INIT_FIRST_SPECIFIC_REG                  NR_OF_COEFS
#define  WBL_INIT_LAST_SPECIFIC_REG                   (NR_OF_COEFS-1)
#define  WBL_INIT_FIRST_GENERIC_FIELD                 WBL_INIT_WBLAGCCFG_WBLAGCON
#define  WBL_INIT_LAST_GENERIC_FIELD                  WBL_INIT_BDTIMCFG1_BDCFLKCHK
#define  WBL_INIT_FIRST_GENERIC_REG                   NR_OF_COEFS
#define  WBL_INIT_LAST_GENERIC_REG                    (NR_OF_COEFS-1)
#define  WBL_FIXED_FIRST_ELEMENT                      WBL_CALC_CDRW_CDFCMPGAI_Low
#define  WBL_FIXED_LAST_ELEMENT                       WBL_WBLAGCCFG_WBLSLWAGC
#define  WBL_FIXED_FIRST_SPECIFIC_FIELD               WBL_CALC_CDRW_CDFCMPGAI_Low
#define  WBL_FIXED_LAST_SPECIFIC_FIELD                WBL_CALC_Target_AGC
#define  WBL_FIXED_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  WBL_FIXED_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  WBL_FIXED_FIRST_GENERIC_FIELD                WBL_ADCBUFCFG_WBLADCTYP
#define  WBL_FIXED_LAST_GENERIC_FIELD                 WBL_WBLAGCCFG_WBLSLWAGC
#define  WBL_FIXED_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  WBL_FIXED_LAST_GENERIC_REG                   (NR_OF_COEFS-1)
#define  WBL_DT_FIRST_ELEMENT                         WBL_CALC_CDATIPPhaseGain
#define  WBL_DT_LAST_ELEMENT                          WBL_BDTIMCFG2_BDIDWBDLY
#define  WBL_DT_FIRST_SPECIFIC_FIELD                  WBL_CALC_CDATIPPhaseGain
#define  WBL_DT_LAST_SPECIFIC_FIELD                   WBL_DRTIMCFG2_HDRLENLG
#define  WBL_DT_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  WBL_DT_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  WBL_DT_FIRST_GENERIC_FIELD                   WBL_ADCBUFCFG_WBLADCINV
#define  WBL_DT_LAST_GENERIC_FIELD                    WBL_BDTIMCFG2_BDIDWBDLY
#define  WBL_DT_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  WBL_DT_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  WBL_DK_FIRST_ELEMENT                         WBL_CALC_Max_Dyn_Gain
#define  WBL_DK_LAST_ELEMENT                          WBL_PLLLCKCFG_LCKDETTHR
#define  WBL_DK_FIRST_SPECIFIC_FIELD                  WBL_CALC_Max_Dyn_Gain
#define  WBL_DK_LAST_SPECIFIC_FIELD                   WBL_CALC_WdiffGainIndexInit
#define  WBL_DK_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  WBL_DK_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  WBL_DK_FIRST_GENERIC_FIELD                   WBL_AGCDYNGA_WBLDYNGA
#define  WBL_DK_LAST_GENERIC_FIELD                    WBL_PLLLCKCFG_LCKDETTHR
#define  WBL_DK_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  WBL_DK_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  WBL_SPD_FIRST_ELEMENT                        WBL_CALC_Freq_Upper_Limit
#define  WBL_SPD_LAST_ELEMENT                         WBL_CALC_Freq_Lower_Limit
#define  WBL_SPD_FIRST_SPECIFIC_FIELD                 WBL_CALC_Freq_Upper_Limit
#define  WBL_SPD_LAST_SPECIFIC_FIELD                  WBL_CALC_Freq_Lower_Limit
#define  WBL_SPD_FIRST_SPECIFIC_REG                   NR_OF_COEFS
#define  WBL_SPD_LAST_SPECIFIC_REG                    (NR_OF_COEFS-1)
#define  WBL_SPD_FIRST_GENERIC_FIELD                  NR_OF_COEFS
#define  WBL_SPD_LAST_GENERIC_FIELD                   (NR_OF_COEFS-1)
#define  WBL_SPD_FIRST_GENERIC_REG                    NR_OF_COEFS
#define  WBL_SPD_LAST_GENERIC_REG                     (NR_OF_COEFS-1)
#define  WBL_DS_FIRST_ELEMENT                         NR_OF_COEFS
#define  WBL_DS_LAST_ELEMENT                          (NR_OF_COEFS-1)
#define  WBL_DS_FIRST_SPECIFIC_FIELD                  NR_OF_COEFS
#define  WBL_DS_LAST_SPECIFIC_FIELD                   (NR_OF_COEFS-1)
#define  WBL_DS_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  WBL_DS_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  WBL_DS_FIRST_GENERIC_FIELD                   NR_OF_COEFS
#define  WBL_DS_LAST_GENERIC_FIELD                    (NR_OF_COEFS-1)
#define  WBL_DS_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  WBL_DS_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  SRV_INIT_FIRST_ELEMENT                       SRV_INIT_LCCTL_LCTSTIRQ
#define  SRV_INIT_LAST_ELEMENT                        SRV_INIT_PWMSEL
#define  SRV_INIT_FIRST_SPECIFIC_FIELD                NR_OF_COEFS
#define  SRV_INIT_LAST_SPECIFIC_FIELD                 (NR_OF_COEFS-1)
#define  SRV_INIT_FIRST_SPECIFIC_REG                  NR_OF_COEFS
#define  SRV_INIT_LAST_SPECIFIC_REG                   (NR_OF_COEFS-1)
#define  SRV_INIT_FIRST_GENERIC_FIELD                 SRV_INIT_LCCTL_LCTSTIRQ
#define  SRV_INIT_LAST_GENERIC_FIELD                  SRV_INIT_LCCTL_OPENDLLP
#define  SRV_INIT_FIRST_GENERIC_REG                   SRV_INIT_SVSPARE
#define  SRV_INIT_LAST_GENERIC_REG                    SRV_INIT_PWMSEL
#define  SRV_FIXED_FIRST_ELEMENT                      SRV_DROPCTL_DROPPROT
#define  SRV_FIXED_LAST_ELEMENT                       SRV_DLPRG
#define  SRV_FIXED_FIRST_SPECIFIC_FIELD               NR_OF_COEFS
#define  SRV_FIXED_LAST_SPECIFIC_FIELD                (NR_OF_COEFS-1)
#define  SRV_FIXED_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  SRV_FIXED_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  SRV_FIXED_FIRST_GENERIC_FIELD                SRV_DROPCTL_DROPPROT
#define  SRV_FIXED_LAST_GENERIC_FIELD                 SRV_INHIBITTX_INHIEN
#define  SRV_FIXED_FIRST_GENERIC_REG                  SRV_SPEEDLVL
#define  SRV_FIXED_LAST_GENERIC_REG                   SRV_DLPRG
#define  SRV_DT_FIRST_ELEMENT                         SRV_CALC_C_cooling_power
#define  SRV_DT_LAST_ELEMENT                          SRV_DLFSBLVL
#define  SRV_DT_FIRST_SPECIFIC_FIELD                  SRV_CALC_C_cooling_power
#define  SRV_DT_LAST_SPECIFIC_FIELD                   SRV_CALC_C_Gdelta_write
#define  SRV_DT_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  SRV_DT_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  SRV_DT_FIRST_GENERIC_FIELD                   SRV_SPEEDLVL_SLOWZERO
#define  SRV_DT_LAST_GENERIC_FIELD                    SRV_TESRCCTL_TESRCDB
#define  SRV_DT_FIRST_GENERIC_REG                     SRV_DLFSKLVL
#define  SRV_DT_LAST_GENERIC_REG                      SRV_DLFSBLVL
#define  SRV_DK_FIRST_ELEMENT                         SRV_CALC_C_Read_power
#define  SRV_DK_LAST_ELEMENT                          SRV_LCDELTA2_DELTA2MAX
#define  SRV_DK_FIRST_SPECIFIC_FIELD                  SRV_CALC_C_Read_power
#define  SRV_DK_LAST_SPECIFIC_FIELD                   SRV_CALC_C_slope_ADC_VEpower
#define  SRV_DK_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  SRV_DK_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  SRV_DK_FIRST_GENERIC_FIELD                   SRV_LCTHRESH_LCTHMAX
#define  SRV_DK_LAST_GENERIC_FIELD                    SRV_LCDELTA2_DELTA2MAX
#define  SRV_DK_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  SRV_DK_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  SRV_SPD_FIRST_ELEMENT                        SRV_FVSET_FVD2
#define  SRV_SPD_LAST_ELEMENT                         SRV_DFCTBL_DBL
#define  SRV_SPD_FIRST_SPECIFIC_FIELD                 NR_OF_COEFS
#define  SRV_SPD_LAST_SPECIFIC_FIELD                  (NR_OF_COEFS-1)
#define  SRV_SPD_FIRST_SPECIFIC_REG                   NR_OF_COEFS
#define  SRV_SPD_LAST_SPECIFIC_REG                    (NR_OF_COEFS-1)
#define  SRV_SPD_FIRST_GENERIC_FIELD                  SRV_FVSET_FVD2
#define  SRV_SPD_LAST_GENERIC_FIELD                   SRV_DFCTBL_DBL
#define  SRV_SPD_FIRST_GENERIC_REG                    NR_OF_COEFS
#define  SRV_SPD_LAST_GENERIC_REG                     (NR_OF_COEFS-1)
#define  SRV_DS_FIRST_ELEMENT                         NR_OF_COEFS
#define  SRV_DS_LAST_ELEMENT                          (NR_OF_COEFS-1)
#define  SRV_DS_FIRST_SPECIFIC_FIELD                  NR_OF_COEFS
#define  SRV_DS_LAST_SPECIFIC_FIELD                   (NR_OF_COEFS-1)
#define  SRV_DS_FIRST_SPECIFIC_REG                    NR_OF_COEFS
#define  SRV_DS_LAST_SPECIFIC_REG                     (NR_OF_COEFS-1)
#define  SRV_DS_FIRST_GENERIC_FIELD                   NR_OF_COEFS
#define  SRV_DS_LAST_GENERIC_FIELD                    (NR_OF_COEFS-1)
#define  SRV_DS_FIRST_GENERIC_REG                     NR_OF_COEFS
#define  SRV_DS_LAST_GENERIC_REG                      (NR_OF_COEFS-1)
#define  SRVDSP_INIT_FIRST_ELEMENT                    NR_OF_COEFS
#define  SRVDSP_INIT_LAST_ELEMENT                     (NR_OF_COEFS-1)
#define  SRVDSP_INIT_FIRST_SPECIFIC_FIELD             NR_OF_COEFS
#define  SRVDSP_INIT_LAST_SPECIFIC_FIELD              (NR_OF_COEFS-1)
#define  SRVDSP_INIT_FIRST_SPECIFIC_REG               NR_OF_COEFS
#define  SRVDSP_INIT_LAST_SPECIFIC_REG                (NR_OF_COEFS-1)
#define  SRVDSP_INIT_FIRST_GENERIC_FIELD              NR_OF_COEFS
#define  SRVDSP_INIT_LAST_GENERIC_FIELD               (NR_OF_COEFS-1)
#define  SRVDSP_INIT_FIRST_GENERIC_REG                NR_OF_COEFS
#define  SRVDSP_INIT_LAST_GENERIC_REG                 (NR_OF_COEFS-1)
#define  SRVDSP_FIXED_FIRST_ELEMENT                   NR_OF_COEFS
#define  SRVDSP_FIXED_LAST_ELEMENT                    (NR_OF_COEFS-1)
#define  SRVDSP_FIXED_FIRST_SPECIFIC_FIELD            NR_OF_COEFS
#define  SRVDSP_FIXED_LAST_SPECIFIC_FIELD             (NR_OF_COEFS-1)
#define  SRVDSP_FIXED_FIRST_SPECIFIC_REG              NR_OF_COEFS
#define  SRVDSP_FIXED_LAST_SPECIFIC_REG               (NR_OF_COEFS-1)
#define  SRVDSP_FIXED_FIRST_GENERIC_FIELD             NR_OF_COEFS
#define  SRVDSP_FIXED_LAST_GENERIC_FIELD              (NR_OF_COEFS-1)
#define  SRVDSP_FIXED_FIRST_GENERIC_REG               NR_OF_COEFS
#define  SRVDSP_FIXED_LAST_GENERIC_REG                (NR_OF_COEFS-1)
#define  SRVDSP_DT_FIRST_ELEMENT                      NR_OF_COEFS
#define  SRVDSP_DT_LAST_ELEMENT                       (NR_OF_COEFS-1)
#define  SRVDSP_DT_FIRST_SPECIFIC_FIELD               NR_OF_COEFS
#define  SRVDSP_DT_LAST_SPECIFIC_FIELD                (NR_OF_COEFS-1)
#define  SRVDSP_DT_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  SRVDSP_DT_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  SRVDSP_DT_FIRST_GENERIC_FIELD                NR_OF_COEFS
#define  SRVDSP_DT_LAST_GENERIC_FIELD                 (NR_OF_COEFS-1)
#define  SRVDSP_DT_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  SRVDSP_DT_LAST_GENERIC_REG                   (NR_OF_COEFS-1)
#define  SRVDSP_DK_FIRST_ELEMENT                      NR_OF_COEFS
#define  SRVDSP_DK_LAST_ELEMENT                       (NR_OF_COEFS-1)
#define  SRVDSP_DK_FIRST_SPECIFIC_FIELD               NR_OF_COEFS
#define  SRVDSP_DK_LAST_SPECIFIC_FIELD                (NR_OF_COEFS-1)
#define  SRVDSP_DK_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  SRVDSP_DK_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  SRVDSP_DK_FIRST_GENERIC_FIELD                NR_OF_COEFS
#define  SRVDSP_DK_LAST_GENERIC_FIELD                 (NR_OF_COEFS-1)
#define  SRVDSP_DK_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  SRVDSP_DK_LAST_GENERIC_REG                   (NR_OF_COEFS-1)
#define  SRVDSP_SPD_FIRST_ELEMENT                     NR_OF_COEFS
#define  SRVDSP_SPD_LAST_ELEMENT                      (NR_OF_COEFS-1)
#define  SRVDSP_SPD_FIRST_SPECIFIC_FIELD              NR_OF_COEFS
#define  SRVDSP_SPD_LAST_SPECIFIC_FIELD               (NR_OF_COEFS-1)
#define  SRVDSP_SPD_FIRST_SPECIFIC_REG                NR_OF_COEFS
#define  SRVDSP_SPD_LAST_SPECIFIC_REG                 (NR_OF_COEFS-1)
#define  SRVDSP_SPD_FIRST_GENERIC_FIELD               NR_OF_COEFS
#define  SRVDSP_SPD_LAST_GENERIC_FIELD                (NR_OF_COEFS-1)
#define  SRVDSP_SPD_FIRST_GENERIC_REG                 NR_OF_COEFS
#define  SRVDSP_SPD_LAST_GENERIC_REG                  (NR_OF_COEFS-1)
#define  SRVDSP_DS_FIRST_ELEMENT                      NR_OF_COEFS
#define  SRVDSP_DS_LAST_ELEMENT                       (NR_OF_COEFS-1)
#define  SRVDSP_DS_FIRST_SPECIFIC_FIELD               NR_OF_COEFS
#define  SRVDSP_DS_LAST_SPECIFIC_FIELD                (NR_OF_COEFS-1)
#define  SRVDSP_DS_FIRST_SPECIFIC_REG                 NR_OF_COEFS
#define  SRVDSP_DS_LAST_SPECIFIC_REG                  (NR_OF_COEFS-1)
#define  SRVDSP_DS_FIRST_GENERIC_FIELD                NR_OF_COEFS
#define  SRVDSP_DS_LAST_GENERIC_FIELD                 (NR_OF_COEFS-1)
#define  SRVDSP_DS_FIRST_GENERIC_REG                  NR_OF_COEFS
#define  SRVDSP_DS_LAST_GENERIC_REG                   (NR_OF_COEFS-1)


// Defines for the number of coefs, for each Coef Type, for each CoefTable Type

#define  NUMBER_OF_COEFS_AFE_INIT                     (AFE_INIT_LAST_ELEMENT - AFE_INIT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_AFE_FIXED                    (AFE_FIXED_LAST_ELEMENT - AFE_FIXED_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_AFE_DT                       (AFE_DT_LAST_ELEMENT - AFE_DT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_AFE_DK                       (AFE_DK_LAST_ELEMENT - AFE_DK_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_AFE_SPD                      (AFE_SPD_LAST_ELEMENT - AFE_SPD_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_AFE_DS                       (AFE_DS_LAST_ELEMENT - AFE_DS_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_INIT                     (DFE_INIT_LAST_ELEMENT - DFE_INIT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_FIXED                    (DFE_FIXED_LAST_ELEMENT - DFE_FIXED_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_DT                       (DFE_DT_LAST_ELEMENT - DFE_DT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_DK                       (DFE_DK_LAST_ELEMENT - DFE_DK_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_SPD                      (DFE_SPD_LAST_ELEMENT - DFE_SPD_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_DFE_DS                       (DFE_DS_LAST_ELEMENT - DFE_DS_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_INIT                     (WBL_INIT_LAST_ELEMENT - WBL_INIT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_FIXED                    (WBL_FIXED_LAST_ELEMENT - WBL_FIXED_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_DT                       (WBL_DT_LAST_ELEMENT - WBL_DT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_DK                       (WBL_DK_LAST_ELEMENT - WBL_DK_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_SPD                      (WBL_SPD_LAST_ELEMENT - WBL_SPD_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_WBL_DS                       (WBL_DS_LAST_ELEMENT - WBL_DS_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_INIT                     (SRV_INIT_LAST_ELEMENT - SRV_INIT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_FIXED                    (SRV_FIXED_LAST_ELEMENT - SRV_FIXED_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_DT                       (SRV_DT_LAST_ELEMENT - SRV_DT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_DK                       (SRV_DK_LAST_ELEMENT - SRV_DK_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_SPD                      (SRV_SPD_LAST_ELEMENT - SRV_SPD_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRV_DS                       (SRV_DS_LAST_ELEMENT - SRV_DS_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_INIT                  (SRVDSP_INIT_LAST_ELEMENT - SRVDSP_INIT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_FIXED                 (SRVDSP_FIXED_LAST_ELEMENT - SRVDSP_FIXED_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_DT                    (SRVDSP_DT_LAST_ELEMENT - SRVDSP_DT_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_DK                    (SRVDSP_DK_LAST_ELEMENT - SRVDSP_DK_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_SPD                   (SRVDSP_SPD_LAST_ELEMENT - SRVDSP_SPD_FIRST_ELEMENT + 1)
#define  NUMBER_OF_COEFS_SRVDSP_DS                    (SRVDSP_DS_LAST_ELEMENT - SRVDSP_DS_FIRST_ELEMENT + 1)

#define NUMBER_OF_COEFS_AFE_INIT_GENERIC_REGFIELD     (AFE_INIT_LAST_GENERIC_REG - AFE_INIT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_AFE_FIXED_GENERIC_REGFIELD    (AFE_FIXED_LAST_GENERIC_FIELD - AFE_FIXED_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_AFE_DT_GENERIC_REGFIELD       (AFE_DT_LAST_GENERIC_REG - AFE_DT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_AFE_DK_GENERIC_REGFIELD       (AFE_DK_LAST_GENERIC_FIELD - AFE_DK_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_AFE_SPD_GENERIC_REGFIELD      (AFE_SPD_LAST_GENERIC_FIELD - AFE_SPD_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_AFE_DS_GENERIC_REGFIELD       (AFE_DS_LAST_GENERIC_FIELD - AFE_DS_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_DFE_INIT_GENERIC_REGFIELD     (DFE_INIT_LAST_GENERIC_FIELD - DFE_INIT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_DFE_FIXED_GENERIC_REGFIELD    (DFE_FIXED_LAST_GENERIC_FIELD - DFE_FIXED_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_DFE_DT_GENERIC_REGFIELD       (DFE_DT_LAST_GENERIC_REG - DFE_DT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_DFE_DK_GENERIC_REGFIELD       (DFE_DK_LAST_GENERIC_REG - DFE_DK_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_DFE_SPD_GENERIC_REGFIELD      0
#define NUMBER_OF_COEFS_DFE_DS_GENERIC_REGFIELD       (DFE_DS_LAST_GENERIC_FIELD - DFE_DS_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_WBL_INIT_GENERIC_REGFIELD     (WBL_INIT_LAST_GENERIC_FIELD - WBL_INIT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_WBL_FIXED_GENERIC_REGFIELD    (WBL_FIXED_LAST_GENERIC_FIELD - WBL_FIXED_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_WBL_DT_GENERIC_REGFIELD       (WBL_DT_LAST_GENERIC_FIELD - WBL_DT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_WBL_DK_GENERIC_REGFIELD       (WBL_DK_LAST_GENERIC_FIELD - WBL_DK_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_WBL_SPD_GENERIC_REGFIELD      0
#define NUMBER_OF_COEFS_WBL_DS_GENERIC_REGFIELD       0
#define NUMBER_OF_COEFS_SRV_INIT_GENERIC_REGFIELD     (SRV_INIT_LAST_GENERIC_REG - SRV_INIT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_SRV_FIXED_GENERIC_REGFIELD    (SRV_FIXED_LAST_GENERIC_REG - SRV_FIXED_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_SRV_DT_GENERIC_REGFIELD       (SRV_DT_LAST_GENERIC_REG - SRV_DT_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_SRV_DK_GENERIC_REGFIELD       (SRV_DK_LAST_GENERIC_FIELD - SRV_DK_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_SRV_SPD_GENERIC_REGFIELD      (SRV_SPD_LAST_GENERIC_FIELD - SRV_SPD_FIRST_GENERIC_FIELD + 1)
#define NUMBER_OF_COEFS_SRV_DS_GENERIC_REGFIELD       0
#define NUMBER_OF_COEFS_SRVDSP_INIT_GENERIC_REGFIELD  0
#define NUMBER_OF_COEFS_SRVDSP_FIXED_GENERIC_REGFIELD 0
#define NUMBER_OF_COEFS_SRVDSP_DT_GENERIC_REGFIELD    0
#define NUMBER_OF_COEFS_SRVDSP_DK_GENERIC_REGFIELD    0
#define NUMBER_OF_COEFS_SRVDSP_SPD_GENERIC_REGFIELD   0
#define NUMBER_OF_COEFS_SRVDSP_DS_GENERIC_REGFIELD    0

// Enumerations of coefs, for each sheet, for each Coef table

typedef enum
{

   // COEF DEFINITIONS FOR:  AFE_INIT

   // *** Specific coefs ***
   // *** Generic coefs ***
   AFE_INIT_MUX03_MX3TOGP1,                      /* idx =0 */
   AFE_INIT_RF00_PDRFALL,                        /* idx =1 */
   AFE_INIT_RF00_PDRF,                           /* idx =2 */
   AFE_INIT_RF00_PDDPD,                          /* idx =3 */
   AFE_INIT_RF00_EQSVOSEL,                       /* idx =4 */
   AFE_INIT_RF00_EQDPDSEL,                       /* idx =5 */
   AFE_INIT_RF01_RFADCTEN,                       /* idx =6 */
   AFE_INIT_RF01_RFINGN,                         /* idx =7 */
   AFE_INIT_RF02_EQBUFCUR,                       /* idx =8 */
   AFE_INIT_RF03_BPDPDHP,                        /* idx =9 */
   AFE_INIT_RF03_VCMADEN,                        /* idx =A */
   AFE_INIT_RF03_RFAOCEN,                        /* idx =B */
   AFE_INIT_RF03_RFAGCEN,                        /* idx =C */
   AFE_INIT_RF03_RFLNDSEL,                       /* idx =D */
   AFE_INIT_RF03_RFHDRSEL,                       /* idx =E */
   AFE_INIT_RF03_RFOEN,                          /* idx =F */
   AFE_INIT_RF03_RFWLDSEL,                       /* idx =10 */
   AFE_INIT_RF04_RFINSEL,                        /* idx =11 */
   AFE_INIT_RF04_RFAGCREG,                       /* idx =12 */
   AFE_INIT_RF05_RFHSSEL,                        /* idx =13 */
   AFE_INIT_RF05_RFHSFC,                         /* idx =14 */
   AFE_INIT_RF05_ENEXTRES,                       /* idx =15 */
   AFE_INIT_RF06_RFGN,                           /* idx =16 */
   AFE_INIT_RF10_RFAOCREG,                       /* idx =17 */
   AFE_INIT_RF19_PDGMIBIAS,                      /* idx =18 */
   AFE_INIT_RF19_RFIBSEL,                        /* idx =19 */
   AFE_INIT_RF20_DPD34INV,                       /* idx =1A */
   AFE_INIT_RF20_DPD12INV,                       /* idx =1B */
   AFE_INIT_RF21_DPDGN,                          /* idx =1C */
   AFE_INIT_RF21_DPDCPTEN,                       /* idx =1D */
   AFE_INIT_RF21_DPDPDMODE,                      /* idx =1E */
   AFE_INIT_RF21_DPDINFC,                        /* idx =1F */
   AFE_INIT_RF22_DPDHSSEL,                       /* idx =20 */
   AFE_INIT_RF22_DPDHSFCH,                       /* idx =21 */
   AFE_INIT_RF22_DPDHSISCL,                      /* idx =22 */
   AFE_INIT_RF23_DPDRISEN,                       /* idx =23 */
   AFE_INIT_RF23_DPDFALEN,                       /* idx =24 */
   AFE_INIT_RF24_DPDVCGM,                        /* idx =25 */
   AFE_INIT_RF25_RFGNWLD,                        /* idx =26 */
   AFE_INIT_FCTS10_AHOLDFE,                      /* idx =27 */
   AFE_INIT_FCTS10_FEREGHLD,                     /* idx =28 */
   AFE_INIT_FCTS10_FEINV,                        /* idx =29 */
   AFE_INIT_FCTS14_CEINV,                        /* idx =2A */
   AFE_INIT_FCTS14_AHOLDCE,                      /* idx =2B */
   AFE_INIT_FCTS14_CEREGHLD,                     /* idx =2C */
   AFE_INIT_FCTS25_TEREGHLD,                     /* idx =2D */
   AFE_INIT_FCTS25_TELNDINV,                     /* idx =2E */
   AFE_INIT_FCTS25_TEINV,                        /* idx =2F */
   AFE_INIT_FCTS25_TESRVHLD,                     /* idx =30 */
   AFE_INIT_FCTS29_TOKSEL,                       /* idx =31 */
   AFE_INIT_FCTS29_TXINV,                        /* idx =32 */
   AFE_INIT_FCTS29_TXLNDINV,                     /* idx =33 */
   AFE_INIT_FCTS32_FE2BYPASS,                    /* idx =34 */
   AFE_INIT_FCTS32_FEGPADCEN,                    /* idx =35 */
   AFE_INIT_FCTS32_BPFELP,                       /* idx =36 */
   AFE_INIT_FCTS36_CEFCSFSEL,                    /* idx =37 */
   AFE_INIT_FCTS36_BPCELP,                       /* idx =38 */
   AFE_INIT_FCTS38_TE2BYPASS,                    /* idx =39 */
   AFE_INIT_FCTS38_TEGPADCEN,                    /* idx =3A */
   AFE_INIT_FCTS38_BPTELP,                       /* idx =3B */
   AFE_INIT_FCTS39_BPTXLP,                       /* idx =3C */
   AFE_INIT_MB10_GUPEN,                          /* idx =3D */
   AFE_INIT_MB10_GUPON,                          /* idx =3E */
   AFE_INIT_MB10_GUPOFF,                         /* idx =3F */
   AFE_INIT_MB10_GUPINV,                         /* idx =40 */
   AFE_INIT_MB10_GUPDLY,                         /* idx =41 */
   AFE_INIT_MB11_ATTONON,                        /* idx =42 */
   AFE_INIT_MB11_ATTOFF,                         /* idx =43 */
   AFE_INIT_MB11_ATTINV,                         /* idx =44 */
   AFE_INIT_MB11_MBGUPSEL,                       /* idx =45 */
   AFE_INIT_MB12_MBATTSEL,                       /* idx =46 */
   AFE_INIT_MB13_WLDONSEL,                       /* idx =47 */
   AFE_INIT_MB13_DEFECTHLD,                      /* idx =48 */
   AFE_INIT_MB13_BLANKSEL,                       /* idx =49 */
   AFE_INIT_MB14_MPDSHON,                        /* idx =4A */
   AFE_INIT_MB14_MPDSHLP,                        /* idx =4B */
   AFE_INIT_MB14_BWSEL,                          /* idx =4C */
   AFE_INIT_MB15_BPMPPOLP,                       /* idx =4D */
   AFE_INIT_MB18_BPFEOLP,                        /* idx =4E */
   AFE_INIT_MB19_TESTLPFC,                       /* idx =4F */
   AFE_INIT_MB19_BPLP,                           /* idx =50 */
   AFE_INIT_MB20_BPSRFOLP,                       /* idx =51 */
   AFE_INIT_MB21_DPDCH1OS,                       /* idx =52 */
   AFE_INIT_MB22_DPDCH2OS,                       /* idx =53 */
   AFE_INIT_SB10_SBGUPSEL,                       /* idx =54 */
   AFE_INIT_SB11_SBATTSEL,                       /* idx =55 */
   AFE_INIT_SB15_SPDSHON,                        /* idx =56 */
   AFE_INIT_SB15_SPDSHLP,                        /* idx =57 */
   AFE_INIT_SB16_BPSPPO1LP,                      /* idx =58 */
   AFE_INIT_SB18_BPSPPO2LP,                      /* idx =59 */
   AFE_INIT_SB19_SBADE,                          /* idx =5A */
   AFE_INIT_SB19_SBADF,                          /* idx =5B */
   AFE_INIT_SB19_SBADG,                          /* idx =5C */
   AFE_INIT_SB19_SBADH,                          /* idx =5D */
   AFE_INIT_SB20_DPDCH3OS,                       /* idx =5E */
   AFE_INIT_SB21_DPDCH4OS,                       /* idx =5F */
   AFE_INIT_BSRP02_RXTOK,                        /* idx =60 */
   AFE_INIT_BSRP03_PHSDISCK,                     /* idx =61 */
   AFE_INIT_BSRP04_BHSDISCK,                     /* idx =62 */
   AFE_INIT_BSRP06_TXOS,                         /* idx =63 */
   AFE_INIT_BSRP07_BSHLD,                        /* idx =64 */
   AFE_INIT_BSRP08_RFVREFNOS,                    /* idx =65 */
   AFE_INIT_BSRP09_BS2BYPASS,                    /* idx =66 */
   AFE_INIT_BSRP09_BSINV,                        /* idx =67 */
   AFE_INIT_BSRP09_BPBSLP,                       /* idx =68 */
   AFE_INIT_BSRP12_RPHLD,                        /* idx =69 */
   AFE_INIT_BSRP14_RP2BYPASS,                    /* idx =6A */
   AFE_INIT_BSRP14_RPINV,                        /* idx =6B */
   AFE_INIT_BSRP14_BPRPLP,                       /* idx =6C */
   AFE_INIT_BSRP18_PHSBYPASS,                    /* idx =6D */
   AFE_INIT_BSRP20_BHSBYPASS,                    /* idx =6E */
   AFE_INIT_BSRP21_PHSCSEL,                      /* idx =6F */
   AFE_INIT_BSRP21_PHSDIS,                       /* idx =70 */
   AFE_INIT_BSRP21_BHSCSEL,                      /* idx =71 */
   AFE_INIT_BSRP21_BHSDIS,                       /* idx =72 */
   AFE_INIT_BSRP25_RCINV,                        /* idx =73 */
   AFE_INIT_BSRP26_RCTH,                         /* idx =74 */
   AFE_INIT_BSRP27_RCTHSEL,                      /* idx =75 */
   AFE_INIT_WOBAPC00_WSHNOLPON,                  /* idx =76 */
   AFE_INIT_WOBAPC00_WSHLPON,                    /* idx =77 */
   AFE_INIT_WOBAPC25_RSHNOLPON,                  /* idx =78 */
   AFE_INIT_WOBAPC25_RSHLPON,                    /* idx =79 */
   AFE_INIT_WOBAPC00_PDWSH12,                    /* idx =7A */
   AFE_INIT_WOBAPC00_PDRSH34,                    /* idx =7B */
   AFE_INIT_WOBAPC00_S34FDVRSL,                  /* idx =7C */
   AFE_INIT_WOBAPC04_PDAPCALL,                   /* idx =7D */
   AFE_INIT_WOBAPC04_APCTEST,                    /* idx =7E */
   AFE_INIT_WOBAPC04_PDVREF06,                   /* idx =7F */
   AFE_INIT_WOBAPC04_PDFVREF,                    /* idx =80 */
   AFE_INIT_WOBAPC05_RGUPREG,                    /* idx =81 */
   AFE_INIT_WOBAPC05_REGNBB,                     /* idx =82 */
   AFE_INIT_WOBAPC05_REGNBA,                     /* idx =83 */
   AFE_INIT_WOBAPC05_HPMODE,                     /* idx =84 */
   AFE_INIT_WOBAPC05_PDINPWBL,                   /* idx =85 */
   AFE_INIT_WOBAPC05_PDWOBALL,                   /* idx =86 */
   AFE_INIT_WOBAPC05_WBLLPEN,                    /* idx =87 */
   AFE_INIT_WOBAPC07_RSHSEL,                     /* idx =88 */
   AFE_INIT_WOBAPC07_WBLBIN,                     /* idx =89 */
   AFE_INIT_WOBAPC07_WBLCIN,                     /* idx =8A */
   AFE_INIT_WOBAPC07_WBLDIN,                     /* idx =8B */
   AFE_INIT_WOBAPC09_RADPGNB,                    /* idx =8C */
   AFE_INIT_WOBAPC10_RBCPGNB,                    /* idx =8D */
   AFE_INIT_WOBAPC11_RADGNB,                     /* idx =8E */
   AFE_INIT_WOBAPC12_RBCGNB,                     /* idx =8F */
   AFE_INIT_WOBAPC13_PDREADWOB,                  /* idx =90 */
   AFE_INIT_WOBAPC13_WOBDTPSEL,                  /* idx =91 */
   AFE_INIT_WOBAPC13_PDWRTWOB,                   /* idx =92 */
   AFE_INIT_WOBAPC14_WOBSRCSEL,                  /* idx =93 */
   AFE_INIT_WOBAPC14_WGUPREG,                    /* idx =94 */
   AFE_INIT_WOBAPC14_WBLROUT,                    /* idx =95 */
   AFE_INIT_WOBAPC14_WSHSEL,                     /* idx =96 */
   AFE_INIT_WOBAPC15_WBLWOUT,                    /* idx =97 */
   AFE_INIT_WOBAPC19_AGCBPHPF,                   /* idx =98 */
   AFE_INIT_WOBAPC19_WOBVGA3GU,                  /* idx =99 */
   AFE_INIT_WOBAPC19_WOBLPFBW,                   /* idx =9A */
   AFE_INIT_WOBAPC19_WOBLPFEN,                   /* idx =9B */
   AFE_INIT_WOBAPC20_AGCHPFFC,                   /* idx =9C */
   AFE_INIT_WOBAPC20_VRPWRLPF,                   /* idx =9D */
   AFE_INIT_WOBAPC20_VWPWRLPF,                   /* idx =9E */
   AFE_INIT_WOBAPC21_AGCHPFDF,                   /* idx =9F */
   AFE_INIT_WOBAPC21_AGCIOUT,                    /* idx =A0 */
   AFE_INIT_WOBAPC21_BPAGC3HP,                   /* idx =A1 */
   AFE_INIT_WOBAPC22_LPPINV,                     /* idx =A2 */
   AFE_INIT_WOBAPC22_WOBTEST,                    /* idx =A3 */
   AFE_INIT_WOBAPC22_DISCLIP,                    /* idx =A4 */
   AFE_INIT_WOBAPC22_WOBHDREN,                   /* idx =A5 */
   AFE_INIT_WOBAPC22_DEFECTEN,                   /* idx =A6 */
   AFE_INIT_WOBAPC23_AGCHOLD,                    /* idx =A7 */
   AFE_INIT_WOBAPC23_AGCSFGN,                    /* idx =A8 */
   AFE_INIT_WOBAPC24_WADVCMSEL,                  /* idx =A9 */
   AFE_INIT_WOBAPC25_WOBVGA3EN,                  /* idx =AA */
   AFE_INIT_WOBAPC25_WBVGA3REG,                  /* idx =AB */
   AFE_INIT_REFS00_PDSVOALL,                     /* idx =AC */
   AFE_INIT_REFS00_PDSVOMB,                      /* idx =AD */
   AFE_INIT_REFS00_PDSVOSB,                      /* idx =AE */
   AFE_INIT_REFS00_PDSVOFCT,                     /* idx =AF */
   AFE_INIT_REFS00_PDSVOBSRP,                    /* idx =B0 */
   AFE_INIT_REFS00_PDSVOMUX,                     /* idx =B1 */
   AFE_INIT_REFS00_PDSVOREFS,                    /* idx =B2 */
   AFE_INIT_REFS00_RBIASSEL,                     /* idx =B3 */
   AFE_INIT_REFS01_VREFBSEL,                     /* idx =B4 */
   AFE_INIT_REFS01_ENBANDGAP,                    /* idx =B5 */
   AFE_INIT_RF02_ENSRFOWDN,                      /* idx =B6 */
   AFE_INIT_RF07_RFPOS,                          /* idx =B7 */
   AFE_INIT_RF08_RFOBUFOS,                       /* idx =B8 */
   AFE_INIT_RF09_RFVREFPOS,                      /* idx =B9 */
   AFE_INIT_RF11_RFCOSP,                         /* idx =BA */
   AFE_INIT_RF12_RFCOSN,                         /* idx =BB */
   AFE_INIT_RF17_RFCOSPWLD,                      /* idx =BC */
   AFE_INIT_RF18_RFCOSNWLD,                      /* idx =BD */
   AFE_INIT_RF13_RFCOSPLND,                      /* idx =BE */
   AFE_INIT_RF14_RFCOSNLND,                      /* idx =BF */
   AFE_INIT_RF15_RFCOSPHDR,                      /* idx =C0 */
   AFE_INIT_RF16_RFCOSNHDR,                      /* idx =C1 */
   AFE_INIT_RF19_MAXIBSEL,                       /* idx =C2 */
   AFE_INIT_MB00_MPPOSA,                         /* idx =C3 */
   AFE_INIT_MB01_MPPOSB,                         /* idx =C4 */
   AFE_INIT_MB02_FEOOSA,                         /* idx =C5 */
   AFE_INIT_MB03_FEOOSB,                         /* idx =C6 */
   AFE_INIT_MB04_SRFOSA,                         /* idx =C7 */
   AFE_INIT_MB05_SRFOSB,                         /* idx =C8 */
   AFE_INIT_MB08_MBVREFOSA,                      /* idx =C9 */
   AFE_INIT_MB09_MBVREFOSB,                      /* idx =CA */
   AFE_INIT_SB00_SPP1OSA,                        /* idx =CB */
   AFE_INIT_SB01_SPP1OSB,                        /* idx =CC */
   AFE_INIT_SB03_SPP2OSA,                        /* idx =CD */
   AFE_INIT_SB04_SPP2OSB,                        /* idx =CE */
   AFE_INIT_SB06_SBADOSA,                        /* idx =CF */
   AFE_INIT_SB07_SBADOSB,                        /* idx =D0 */
   AFE_INIT_SB12_SBVREFOSA,                      /* idx =D1 */
   AFE_INIT_SB13_SBVREFOSB,                      /* idx =D2 */
   AFE_INIT_FCTS01_BLNDFCSO,                     /* idx =D3 */
   AFE_INIT_FCTS03_LNDFCSO,                      /* idx =D4 */
   AFE_INIT_FCTS16_BLNDTRKO,                     /* idx =D5 */
   AFE_INIT_FCTS00_BLANKFCSO,                    /* idx =D6 */
   AFE_INIT_FCTS02_FCSO,                         /* idx =D7 */
   AFE_INIT_FCTS04_WRFCSO,                       /* idx =D8 */
   AFE_INIT_FCTS11_FE2OSA,                       /* idx =D9 */
   AFE_INIT_FCTS12_FE2OSB,                       /* idx =DA */
   AFE_INIT_FCTS15_BLANKTRKO,                    /* idx =DB */
   AFE_INIT_FCTS17_TRKO,                         /* idx =DC */
   AFE_INIT_FCTS18_LNDTRKO,                      /* idx =DD */
   AFE_INIT_FCTS19_WRTRKO,                       /* idx =DE */
   AFE_INIT_FCTS26_TE2OSA,                       /* idx =DF */
   AFE_INIT_FCTS27_TE2OSB,                       /* idx =E0 */
   AFE_INIT_FCTS34_CEOS,                         /* idx =E1 */
   AFE_INIT_FCTS41_WRLNDTRKO,                    /* idx =E2 */
   AFE_INIT_FCTS42_WRLNDFCSO,                    /* idx =E3 */
   AFE_INIT_BSRP00_BS2OSA,                       /* idx =E4 */
   AFE_INIT_BSRP01_BS2OSB,                       /* idx =E5 */
   AFE_INIT_BSRP11_RPOS,                         /* idx =E6 */
   AFE_INIT_BSRP14_RPDFCTDLY,                    /* idx =E7 */
   AFE_INIT_BSRP15_RP2OS,                        /* idx =E8 */
   AFE_INIT_BSRP17_PHSOS,                        /* idx =E9 */
   AFE_INIT_BSRP19_BHSOS,                        /* idx =EA */
   AFE_INIT_BSRP22_BLANKBSKO,                    /* idx =EB */
   AFE_INIT_BSRP23_BSKO,                         /* idx =EC */
   AFE_INIT_BSRP25_AHLDRPPBH,                    /* idx =ED */
   AFE_INIT_WOBAPC06_VREFOS,                     /* idx =EE */
   AFE_INIT_WOBAPC08_RADOSA,                     /* idx =EF */
   AFE_INIT_WOBAPC16_RBCOSA,                     /* idx =F0 */
   AFE_INIT_WOBAPC17_WADOS,                      /* idx =F1 */
   AFE_INIT_WOBAPC18_WBCOS,                      /* idx =F2 */
   AFE_INIT_WOBAPC26_RADOSB,                     /* idx =F3 */
   AFE_INIT_WOBAPC27_RBCOSB,                     /* idx =F4 */
   AFE_INIT_WOBAPC20_OPTSTEN,                    /* idx =F5 */
   AFE_INIT_WOBAPC20_APCINSEL,                   /* idx =F6 */
   AFE_INIT_WOBAPC24,                            /* idx =F7 */
   AFE_INIT_WOBAPC01,                            /* idx =F8 */

   // COEF DEFINITIONS FOR:  AFE_FIXED

   // *** Specific coefs ***
   AFE_CALC_DVDRW_BLANK_SRFO_TH,                 /* idx =F9 */
   AFE_CALC_BS_TARGET_10MV,                      /* idx =FA */
   AFE_CALC_CDROM_SRFO_TH,                       /* idx =FB */
   AFE_CALC_DVDROM_SRFO_TH,                      /* idx =FC */
   AFE_CALC_DVDROMDL_SRFO_TH,                    /* idx =FD */
   AFE_CALC_BDROM_SRFO_TH,                       /* idx =FE */
   AFE_CALC_BDROMDL_SRFO_TH,                     /* idx =FF */
   AFE_CALC_HDROMDL_SRFO_TH,                     /* idx =100 */
   AFE_CALC_HDROM_SRFO_TH,                       /* idx =101 */
   AFE_CALC_RFVGAShiftX100,                      /* idx =102 */
   AFE_CALC_STM_FE_TH,                           /* idx =103 */
   AFE_CALC_STM_CD_TH,                           /* idx =104 */
   AFE_CALC_STM_DVD_TH,                          /* idx =105 */
   AFE_CALC_STM_NO_DISC_TH,                      /* idx =106 */
   AFE_WOBAPC15_BDROMMARK,                       /* idx =107 */
   AFE_WOBAPC19_BDROMMARK,                       /* idx =108 */
   AFE_WOBAPC21_BDROMMARK,                       /* idx =109 */
   AFE_WOBAPC23_BDROMMARK,                       /* idx =10A */
   AFE_WOBAPC25_BDROMMARK,                       /* idx =10B */
   // *** Generic coefs ***
   AFE_RF00_PDRFALL,                             /* idx =10C */
   AFE_RF00_PDRF,                                /* idx =10D */
   AFE_FCTS13_FE2AUTOEN,                         /* idx =10E */
   AFE_FCTS28_TE2AUTOEN,                         /* idx =10F */
   AFE_BSRP02_BS2AUTOEN,                         /* idx =110 */
   AFE_WOBAPC00_PDWSH12,                         /* idx =111 */
   AFE_WOBAPC00_PDRSH34,                         /* idx =112 */
   AFE_WOBAPC01_PDREADSUB,                       /* idx =113 */
   AFE_WOBAPC02_ERASE1,                          /* idx =114 */
   AFE_WOBAPC02_APCRDGN,                         /* idx =115 */
   AFE_WOBAPC02_GAIN5OPEN,                       /* idx =116 */
   AFE_WOBAPC02_GAIN10,                          /* idx =117 */
   AFE_WOBAPC02_RDAMPCD,                         /* idx =118 */
   AFE_WOBAPC02_PDRAPC,                          /* idx =119 */
   AFE_WOBAPC03_WINSSEL,                         /* idx =11A */
   AFE_WOBAPC03_PDWAPC,                          /* idx =11B */
   AFE_WOBAPC03_FDGD2,                           /* idx =11C */
   AFE_WOBAPC04_PDVREF06,                        /* idx =11D */
   AFE_WOBAPC04_PDFVREF,                         /* idx =11E */
   AFE_WOBAPC04_PDAPCALL,                        /* idx =11F */
   AFE_WOBAPC05_WOBAUXEN,                        /* idx =120 */
   AFE_WOBAPC05_HPMODE,                          /* idx =121 */
   AFE_WOBAPC05_PDINPWBL,                        /* idx =122 */
   AFE_WOBAPC05_PDWOBALL,                        /* idx =123 */
   AFE_WOBAPC13_PDREADWOB,                       /* idx =124 */
   AFE_WOBAPC21_WDIFFGAIN,                       /* idx =125 */
   AFE_WOBAPC24_PDWPP12,                         /* idx =126 */
   AFE_WOBAPC25_WOBVGA3EN,                       /* idx =127 */
   AFE_REFS00_PDSVOMUX,                          /* idx =128 */

   // COEF DEFINITIONS FOR:  AFE_DT

   // *** Specific coefs ***
   AFE_FCTS37_TEBG,                              /* idx =129 */
   AFE_MB11_MBGUPSEL_STM,                        /* idx =12A */
   AFE_MB16_MPPOBG,                              /* idx =12B */
   AFE_MB18_FEOPGN_STM,                          /* idx =12C */
   AFE_MB19_FEOBG,                               /* idx =12D */
   AFE_SB16_SPP1BG,                              /* idx =12E */
   AFE_SB18_SPP2BG,                              /* idx =12F */
   AFE_CALC_NODISC_SRFO_TH,                      /* idx =130 */
   AFE_CALC_MIN_FE_SRFO_RATIO,                   /* idx =131 */
   AFE_CALC_RP_PP_TARGET_10MV,                   /* idx =132 */
   AFE_CALC_FE_PP_TARGET_10MV,                   /* idx =133 */
   AFE_CALC_TE_PP_TARGET_10MV,                   /* idx =134 */
   AFE_CALC_PhsSlopeX100,                        /* idx =135 */
   AFE_CALC_BhsSlope,                            /* idx =136 */
   AFE_CALC_WlpfRatioX10,                        /* idx =137 */
   AFE_CALC_WhpfRatioX100,                       /* idx =138 */
   AFE_CALC_DpdTargetX1000,                      /* idx =139 */
   AFE_CALC_MbTargetX100,                        /* idx =13A */
   AFE_CALC_WobbleTargetX100,                    /* idx =13B */
   AFE_CALC_RfSrfoTargetX1000,                   /* idx =13C */
   AFE_CALC_PdicGrRatiodBX4,                     /* idx =13D */
   AFE_CALC_STM_PRE_TH,                          /* idx =13E */
   AFE_FCTS35_CEBG12DB_EMBOSSED,                 /* idx =13F */
   // *** Generic coefs ***
   AFE_RF03_VCMADEN,                             /* idx =140 */
   AFE_RF03_RFLNDSEL,                            /* idx =141 */
   AFE_RF03_RFHDRSEL,                            /* idx =142 */
   AFE_RF04_RFINSEL,                             /* idx =143 */
   AFE_RF20_DPD1SEL,                             /* idx =144 */
   AFE_RF20_DPD2SEL,                             /* idx =145 */
   AFE_RF20_DPD3SEL,                             /* idx =146 */
   AFE_RF20_DPD4SEL,                             /* idx =147 */
   AFE_RF24_DPDBST,                              /* idx =148 */
   AFE_RF25_DPDCPGN,                             /* idx =149 */
   AFE_RF25_DPDCPFC,                             /* idx =14A */
   AFE_MB06_SRFOBGA,                             /* idx =14B */
   AFE_MB07_SRFOBGB,                             /* idx =14C */
   AFE_MB10_GUPMONOEN,                           /* idx =14D */
   AFE_MB13_LANDSEL,                             /* idx =14E */
   AFE_MB13_HDREN,                               /* idx =14F */
   AFE_MB13_HDREN2,                              /* idx =150 */
   AFE_MB14_SVOAUXEN,                            /* idx =151 */
   AFE_MB15_MPPOPGN,                             /* idx =152 */
   AFE_MB15_MPPA,                                /* idx =153 */
   AFE_MB15_MPPB,                                /* idx =154 */
   AFE_MB15_MPPC,                                /* idx =155 */
   AFE_MB15_MPPD,                                /* idx =156 */
   AFE_MB15_MPPOLPSET,                           /* idx =157 */
   AFE_MB18_FEOPGN,                              /* idx =158 */
   AFE_MB18_FEOLPSET,                            /* idx =159 */
   AFE_MB20_SRFA,                                /* idx =15A */
   AFE_MB20_SRFB,                                /* idx =15B */
   AFE_MB20_SRFC,                                /* idx =15C */
   AFE_MB20_SRFD,                                /* idx =15D */
   AFE_MB20_SRFOLPSET,                           /* idx =15E */
   AFE_FCTS10_FEHDRHLD,                          /* idx =15F */
   AFE_FCTS10_AHOLDFE,                           /* idx =160 */
   AFE_FCTS10_FEINV,                             /* idx =161 */
   AFE_FCTS13_FE2GNREG,                          /* idx =162 */
   AFE_FCTS14_CEHDRHLD,                          /* idx =163 */
   AFE_FCTS14_CEINV,                             /* idx =164 */
   AFE_FCTS14_AHOLDCE,                           /* idx =165 */
   AFE_FCTS25_TEHDRHLD,                          /* idx =166 */
   AFE_FCTS25_TELNDINV,                          /* idx =167 */
   AFE_FCTS25_TEINV,                             /* idx =168 */
   AFE_FCTS28_TE2GNREG,                          /* idx =169 */
   AFE_FCTS29_TXINV,                             /* idx =16A */
   AFE_FCTS29_TXLNDINV,                          /* idx =16B */
   AFE_FCTS30_FEFEO,                             /* idx =16C */
   AFE_FCTS30_FESP2,                             /* idx =16D */
   AFE_FCTS30_FESP2N,                            /* idx =16E */
   AFE_FCTS30_FESP1,                             /* idx =16F */
   AFE_FCTS30_FEMPP,                             /* idx =170 */
   AFE_FCTS31_FEBG,                              /* idx =171 */
   AFE_FCTS31_FEPGN,                             /* idx =172 */
   AFE_FCTS33_CEMPP,                             /* idx =173 */
   AFE_FCTS33_CESP1,                             /* idx =174 */
   AFE_FCTS33_CEBG,                              /* idx =175 */
   AFE_FCTS35_CEPGN,                             /* idx =176 */
   AFE_FCTS35_CEBG12DB,                          /* idx =177 */
   AFE_FCTS35_CEGN,                              /* idx =178 */
   AFE_FCTS37_TEPGN,                             /* idx =179 */
   AFE_SB02_SPP1PGNB,                            /* idx =17A */
   AFE_SB02_SPP1PGNA,                            /* idx =17B */
   AFE_SB05_SPP2PGNB,                            /* idx =17C */
   AFE_SB05_SPP2PGNA,                            /* idx =17D */
   AFE_SB08_SBADGNA,                             /* idx =17E */
   AFE_SB09_SBADGNB,                             /* idx =17F */
   AFE_SB15_SPP1FJ,                              /* idx =180 */
   AFE_SB15_SPP1GD,                              /* idx =181 */
   AFE_SB15_SPP1FE,                              /* idx =182 */
   AFE_SB15_SPP1EH,                              /* idx =183 */
   AFE_SB16_SPPO1LPST,                           /* idx =184 */
   AFE_SB18_SPPO2LPST,                           /* idx =185 */
   AFE_SB19_SBADE,                               /* idx =186 */
   AFE_SB19_SBADF,                               /* idx =187 */
   AFE_SB19_SBADG,                               /* idx =188 */
   AFE_SB19_SBADH,                               /* idx =189 */
   AFE_SB19_SBADLPSET,                           /* idx =18A */
   AFE_BSRP02_BS2GNREG,                          /* idx =18B */
   AFE_BSRP02_RPPOLINV,                          /* idx =18C */
   AFE_BSRP03_PHSDSCKOF,                         /* idx =18D */
   AFE_BSRP04_BHSDSCKOF,                         /* idx =18E */
   AFE_BSRP07_BSBG,                              /* idx =18F */
   AFE_BSRP07_BSPGN,                             /* idx =190 */
   AFE_BSRP12_RPBG,                              /* idx =191 */
   AFE_BSRP12_RPPGN,                             /* idx =192 */
   AFE_BSRP13_RPGN,                              /* idx =193 */
   AFE_BSRP13_AHOLDRP,                           /* idx =194 */
   AFE_BSRP13_RPBLNKINV,                         /* idx =195 */
   AFE_BSRP14_RPINV,                             /* idx =196 */
   AFE_BSRP18_PHSGN,                             /* idx =197 */
   AFE_BSRP20_BHSGN,                             /* idx =198 */
   AFE_WOBAPC00_WSHFPDON,                        /* idx =199 */
   AFE_WOBAPC00_APCWSEL,                         /* idx =19A */
   AFE_WOBAPC00_APCRSEL,                         /* idx =19B */
   AFE_WOBAPC01_SUBINSEL,                        /* idx =19C */
   AFE_WOBAPC02_RINSSEL,                         /* idx =19D */
   AFE_WOBAPC04_FVREFSEL,                        /* idx =19E */
   AFE_WOBAPC20_APCINSEL,                        /* idx =19F */
   AFE_AVGSVOSET3_AVGHDHLD,                      /* idx =1A0 */
   AFE_WOBAPC24_PDWOB,                           /* idx =1A1 */
   AFE_MB17,                                     /* idx =1A2 */
   AFE_SB14,                                     /* idx =1A3 */
   AFE_SB17,                                     /* idx =1A4 */
   AFE_BSRP05,                                   /* idx =1A5 */
   AFE_BSRP10,                                   /* idx =1A6 */

   // COEF DEFINITIONS FOR:  AFE_DK

   // *** Specific coefs ***
   AFE_CALC_BwRfRatioX100,                       /* idx =1A7 */
   AFE_CALC_BwDpdRatioX100,                      /* idx =1A8 */
   AFE_CALC_BS_GAINUP_BLANK,                     /* idx =1A9 */
   AFE_BSRP16_RP2GN,                             /* idx =1AA */
   AFE_BSRP24_BLANKBSKG,                         /* idx =1AB */
   AFE_BSRP25_BSKG,                              /* idx =1AC */
   AFE_FCTS05_BLANKFCSG,                         /* idx =1AD */
   AFE_FCTS06_BLNDFCSG,                          /* idx =1AE */
   AFE_FCTS07_FCSG,                              /* idx =1AF */
   AFE_FCTS08_LNDFCSG,                           /* idx =1B0 */
   AFE_FCTS09_WRFCSG,                            /* idx =1B1 */
   AFE_FCTS20_BLANKTRKG,                         /* idx =1B2 */
   AFE_FCTS21_BLNDTRKG,                          /* idx =1B3 */
   AFE_FCTS22_TRKG,                              /* idx =1B4 */
   AFE_FCTS23_LNDTRKG,                           /* idx =1B5 */
   AFE_FCTS24_WRTRKG,                            /* idx =1B6 */
   AFE_CALC_FE_GAINUP_BLANK,                     /* idx =1B7 */
   AFE_CALC_SBAD_GAINUP_BLANK,                   /* idx =1B8 */
   AFE_CALC_TE_BLANK_GAINUP,                     /* idx =1B9 */
   // *** Generic coefs ***
   AFE_RF00_ENRFRP,                              /* idx =1BA */
   AFE_BSRP13_RFRPSEL,                           /* idx =1BB */
   AFE_FCTS29_TOKT,                              /* idx =1BC */
   AFE_FCTS29_TOKOFF,                            /* idx =1BD */
   AFE_FCTS40_TOKLVL,                            /* idx =1BE */
   AFE_WOBAPC05_WBLLPEN,                         /* idx =1BF */
   AFE_WOBAPC07_RGUPSEL,                         /* idx =1C0 */
   AFE_WOBAPC09_WADGNB,                          /* idx =1C1 */
   AFE_WOBAPC09_WADGNA,                          /* idx =1C2 */
   AFE_WOBAPC09_RADPGNA,                         /* idx =1C3 */
   AFE_WOBAPC10_WBCGNB,                          /* idx =1C4 */
   AFE_WOBAPC10_WBCGNA,                          /* idx =1C5 */
   AFE_WOBAPC10_RBCPGNA,                         /* idx =1C6 */
   AFE_WOBAPC11_RADGNA,                          /* idx =1C7 */
   AFE_WOBAPC11_WADPGNB,                         /* idx =1C8 */
   AFE_WOBAPC11_WADPGNA,                         /* idx =1C9 */
   AFE_WOBAPC12_RBCGNA,                          /* idx =1CA */
   AFE_WOBAPC12_WBCPGNB,                         /* idx =1CB */
   AFE_WOBAPC12_WBCPGNA,                         /* idx =1CC */
   AFE_WOBAPC13_WGUPSEL,                         /* idx =1CD */
   AFE_WOBAPC13_WBLWSHSEL,                       /* idx =1CE */
   AFE_WOBAPC15_WBLRSHSEL,                       /* idx =1CF */
   AFE_WOBAPC15_WBLLP,                           /* idx =1D0 */
   AFE_WOBAPC19_AGCHGAIN,                        /* idx =1D1 */
   AFE_WOBAPC19_WOBVGA3GU,                       /* idx =1D2 */
   AFE_WOBAPC23_AGCSFGN,                         /* idx =1D3 */
   AFE_WOBAPC23_AGCCLIP1,                        /* idx =1D4 */
   AFE_WOBAPC23_AGCCLIP2,                        /* idx =1D5 */
   AFE_WOBAPC23_AGCCLIP3,                        /* idx =1D6 */
   AFE_WOBAPC25_WBVGA3REG,                       /* idx =1D7 */

   // COEF DEFINITIONS FOR:  AFE_SPD

   // *** Specific coefs ***
   // *** Generic coefs ***
   AFE_FCTS32_FEFCSFSEL,                         /* idx =1D8 */
   AFE_FCTS32_FELPSET,                           /* idx =1D9 */
   AFE_FCTS36_CEFCSFSEL,                         /* idx =1DA */
   AFE_FCTS38_TEFCSFSEL,                         /* idx =1DB */
   AFE_FCTS38_TELPSET,                           /* idx =1DC */
   AFE_FCTS39_TXLP,                              /* idx =1DD */
   AFE_FCTS39_TXLPSET,                           /* idx =1DE */
   AFE_BSRP09_BSLP,                              /* idx =1DF */
   AFE_BSRP09_BSLPSET,                           /* idx =1E0 */
   AFE_BSRP14_RPLP,                              /* idx =1E1 */
   AFE_BSRP14_RPLPSET,                           /* idx =1E2 */

   // COEF DEFINITIONS FOR:  AFE_DS

   // *** Specific coefs ***
   // *** Generic coefs ***
   AFE_WOBAPC00_WSHNOLPON,                       /* idx =1E3 */
   AFE_WOBAPC00_WSHLPON,                         /* idx =1E4 */
   AFE_WOBAPC25_RSHNOLPON,                       /* idx =1E5 */
   AFE_WOBAPC25_RSHLPON,                         /* idx =1E6 */
   AFE_WOBAPC01_RSHFPDON,                        /* idx =1E7 */

   // COEF DEFINITIONS FOR:  DFE_INIT

   // *** Specific coefs ***
   DFE_INIT_HFAMPMCFG_PDATTTYP_HEADER,           /* idx =1E8 */
   DFE_INIT_HFAMPMCFG_PDATTFBW_HEADER,           /* idx =1E9 */
   DFE_INIT_HFAMPMCFG_PDDFBW_HEADER,             /* idx =1EA */
   // *** Generic coefs ***
   DFE_INIT_DFEADCCFG_HFADCINV,                  /* idx =1EB */
   DFE_INIT_DFEADCCFG_HFADCTYP,                  /* idx =1EC */
   DFE_INIT_HFAGCCFG1_AUTORWSW,                  /* idx =1ED */
   DFE_INIT_HFAGCCFG1_AGCCKCLIP,                 /* idx =1EE */
   DFE_INIT_HFAGCCFG1_AGCHOLDEN,                 /* idx =1EF */
   DFE_INIT_HFAGCCFG1_AGCON,                     /* idx =1F0 */
   DFE_INIT_HFAGCCFG1_SLOWAGC,                   /* idx =1F1 */
   DFE_INIT_HFAGCCFG2_DRHFANA,                   /* idx =1F2 */
   DFE_INIT_HFAGCCFG2_AGCDRCFG,                  /* idx =1F3 */
   DFE_INIT_HFAOCCFG_DISHLDAOC,                  /* idx =1F4 */
   DFE_INIT_HFAOCCFG_AOCHOLDEN,                  /* idx =1F5 */
   DFE_INIT_HFAOCCFG_AOCON,                      /* idx =1F6 */
   DFE_INIT_HFAOCCFG_SLOWAOC,                    /* idx =1F7 */
   DFE_INIT_HFAOCCFG_AOCDRCFG,                   /* idx =1F8 */
   DFE_INIT_HFHIPASS_MAHPFBWH,                   /* idx =1F9 */
   DFE_INIT_HFHIPASS_MAHPFBW,                    /* idx =1FA */
   DFE_INIT_HFHIPASS_IIRHPFFH,                   /* idx =1FB */
   DFE_INIT_HFHIPASS_IIRHPFHOD,                  /* idx =1FC */
   DFE_INIT_HFHIPASS_IIRHPFBYP,                  /* idx =1FD */
   DFE_INIT_HFHIPASS_IIRHPFRST,                  /* idx =1FE */
   DFE_INIT_HFSLCCFG_FRCSLCHLD,                  /* idx =1FF */
   DFE_INIT_HFSLCCFG_SLCHOD,                     /* idx =200 */
   DFE_INIT_HFPLLCFG_PLLMODE,                    /* idx =201 */
   DFE_INIT_HFSLCCFG_SLCSWONH,                   /* idx =202 */
   DFE_INIT_HFSLCCFG_SLCDRCFG,                   /* idx =203 */
   DFE_INIT_HFADEQ_ADEQDIS,                      /* idx =204 */
   DFE_INIT_HFNLECFG_NLEMODE,                    /* idx =205 */
   DFE_INIT_HFFPLLCFG_FPLLFHLD,                  /* idx =206 */
   DFE_INIT_HFFPLLCFG_FPLLHOD,                   /* idx =207 */
   DFE_INIT_HFFPLLCFG_FPLLRST,                   /* idx =208 */
   DFE_INIT_HFFPLLCFG_FPLLDRCFG,                 /* idx =209 */
   DFE_INIT_HFFPLLCFG_FPLLARHE,                  /* idx =20A */
   DFE_INIT_HFFPLLCFG_FPLLJHIDE,                 /* idx =20B */
   DFE_INIT_HFFPLLCFG_FPLLJHLDE,                 /* idx =20C */
   DFE_INIT_HFBITDCFG_VITPASSEN,                 /* idx =20D */
   DFE_INIT_HFBITDCFG_VITRST,                    /* idx =20E */
   DFE_INIT_HFBITDCFG_VITRLHOD,                  /* idx =20F */
   DFE_INIT_HFBITDCFG_BITDETCFG,                 /* idx =210 */
   DFE_INIT_RLDJMCFG1_RLDJITRST,                 /* idx =211 */
   DFE_INIT_ICEDTCFG1_ICESFTRST,                 /* idx =212 */
   DFE_INIT_WDDCFG1_WDDDIS,                      /* idx =213 */
   DFE_INIT_WDDCFG1_WDDFILPRE,                   /* idx =214 */
   DFE_INIT_BDRMCFG_BDRMSRST,                    /* idx =215 */
   DFE_INIT_BCACFG_BCAPWRDN,                     /* idx =216 */
   DFE_INIT_BKDDETCFG_BKDDEN,                    /* idx =217 */
   DFE_INIT_WHDDETCFG_WHDDEN,                    /* idx =218 */
   DFE_INIT_BLSDETCFG_BLSDEN,                    /* idx =219 */
   DFE_INIT_EBKDDTCFG_EBKDDEN,                   /* idx =21A */
   DFE_INIT_EWHDDTCFG_EWHDDEN,                   /* idx =21B */

   // COEF DEFINITIONS FOR:  DFE_FIXED

   // *** Specific coefs ***
   DFE_DFEADCCFG_HFADCINV,                       /* idx =21C */
   DFE_CALC_GAINDYNWR,                           /* idx =21D */
   DFE_CALC_Coef_Max_dyn_gain,                   /* idx =21E */
   DFE_CALC_Coef_Min_dyn_gain,                   /* idx =21F */
   DFE_CALC_Target_pp,                           /* idx =220 */
   DFE_HFAOCCFG_OFFSETINV,                       /* idx =221 */
   // *** Generic coefs ***
   DFE_HFAGCCFG1_SLOWAGC,                        /* idx =222 */
   DFE_HFAGCCFG1_AGCTHRHI,                       /* idx =223 */
   DFE_HFAGCCFG1_AGCTHRLO,                       /* idx =224 */
   DFE_HFAOCCFG_SLOWAOC,                         /* idx =225 */
   DFE_HFAOCCFG_OFFSETBND,                       /* idx =226 */
   DFE_HFHIPASS_IIRHPFFH,                        /* idx =227 */
   DFE_HFHIPASS_IIRHPFHOD,                       /* idx =228 */
   DFE_HFHIPASS_IIRHPFBYP,                       /* idx =229 */
   DFE_HFHIPASS_IIRHPFRST,                       /* idx =22A */
   DFE_BKDDETCFG_BKDDEN,                         /* idx =22B */
   DFE_WHDDETCFG_WHDDEN,                         /* idx =22C */
   DFE_HFAGCCFG2_AGCHBWE,                        /* idx =22D */
   DFE_HFAGCCFG2_AGCHBWPRD,                      /* idx =22E */
   DFE_HFAGCCFG2_AGCHBW,                         /* idx =22F */
   DFE_HFAGCDYN1_PREAUTOD,                       /* idx =230 */
   DFE_HFAOCDYN3_AOCHBWE,                        /* idx =231 */
   DFE_HFAOCDYN3_AOCHBWPRD,                      /* idx =232 */
   DFE_HFAOCDYN3_AOCHBW,                         /* idx =233 */
   DFE_HFBITDCFG_BITDETHCFG,                     /* idx =234 */
   DFE_HFBITDCFG_VITRLHEADR,                     /* idx =235 */

   // COEF DEFINITIONS FOR:  DFE_DT

   // *** Specific coefs ***
   DFE_BCACFG_BCAFREQ,                           /* idx =236 */
   DFE_CALC_HFHighpass_FcN1,                     /* idx =237 */
   DFE_CALC_HFHighpass_FcN1_EMBOSSED_PIC,        /* idx =238 */
   DFE_CALC_Movavg_FcN1,                         /* idx =239 */
   DFE_CALC_Movavg_FcN1_EMBOSSED,                /* idx =23A */
   DFE_CALC_Movavg_FcN1_HEADER,                  /* idx =23B */
   DFE_CALC_HFAOC_OFFSET_HEADER,                 /* idx =23C */
   DFE_CALC_HFLowpass_FcN1_DIGIBETA,             /* idx =23D */
   DFE_CALC_StartDelayPercentage_DIGIBETA,       /* idx =23E */
   DFE_CALC_StopDelayPercentage_DIGIBETA,        /* idx =23F */
   DFE_HFAMPMCFG_PDATTFBW_DIGIBETA,              /* idx =240 */
   DFE_BETAMSCFG_AVFLTBW_DIGIBETA,               /* idx =241 */
   DFE_HFAMPMCFG_PDDFBW_DIGIBETA,                /* idx =242 */
   DFE_HFFPLLCFG_FPLLDRCFG_EMBOSSED,             /* idx =243 */
   DFE_HFFPLLCFG_FPLLARHE_EMBOSSED,              /* idx =244 */
   DFE_HFFPLLCFG_FPLLJHIDE_EMBOSSED,             /* idx =245 */
   DFE_HFFPLLCFG_FPLLJHLDE_EMBOSSED,             /* idx =246 */
   DFE_HFSLCCFG_SLCSWONH_EMBOSSED,               /* idx =247 */
   DFE_HFSLCCFG_SLCDRCFG_EMBOSSED,               /* idx =248 */
   // *** Generic coefs ***
   DFE_HFAGCCFG1_AGCWINPRD,                      /* idx =249 */
   DFE_HFAGCCFG1_DISHLDAGC,                      /* idx =24A */
   DFE_HFAGCCFG1_AGCINTBW,                       /* idx =24B */
   DFE_HFAGCCFG2_AGCSWONH,                       /* idx =24C */
   DFE_HFAGCCFG2_AGCINTBWH,                      /* idx =24D */
   DFE_HFAGCCFG2_AGCWINPRH,                      /* idx =24E */
   DFE_HFAGCCFG2_AGCDRCFG,                       /* idx =24F */
   DFE_HFAOCCFG_AOCWINPRD,                       /* idx =250 */
   DFE_HFAOCCFG_DISHLDAOC,                       /* idx =251 */
   DFE_HFAOCCFG_AOCINTBW,                        /* idx =252 */
   DFE_HFAOCCFG_AOCWINPRH,                       /* idx =253 */
   DFE_HFAOCCFG_AOCINTBWH,                       /* idx =254 */
   DFE_HFAOCCFG_AOCSWONH,                        /* idx =255 */
   DFE_HFAOCCFG_AOCDRCFG,                        /* idx =256 */
   DFE_HFDETCFG1_ATTFBWPA,                       /* idx =257 */
   DFE_HFDETCFG1_DECFBWPA,                       /* idx =258 */
   DFE_HFDETCFG2_MSKDR,                          /* idx =259 */
   DFE_HFHIPASS_PICFILBP,                        /* idx =25A */
   DFE_HFHIPASS_MAHPFDCFG,                       /* idx =25B */
   DFE_HFHIPASS_MAHPFSOMH,                       /* idx =25C */
   DFE_HFHIPASS_MAHPFBWH,                        /* idx =25D */
   DFE_HFHIPASS_MAHPFSWH,                        /* idx =25E */
   DFE_HFHIPASS_MAHPFSOM,                        /* idx =25F */
   DFE_HFSLCCFG_SLCBWHI,                         /* idx =260 */
   DFE_HFSLCCFG_SLCSWONH,                        /* idx =261 */
   DFE_HFSLCCFG_SLCDRCFG,                        /* idx =262 */
   DFE_HFSLCCFG_SLCBWH,                          /* idx =263 */
   DFE_HFSLCCFG_SLCBW,                           /* idx =264 */
   DFE_HFSLCCFG_SLCLVLB,                         /* idx =265 */
   DFE_HFPLLCFG_INNLKTHR,                        /* idx =266 */
   DFE_HFPLLCFG_BDLKAIDM,                        /* idx =267 */
   DFE_HFNLECFG_HLDACLOD,                        /* idx =268 */
   DFE_HFNLECFG_AUCLIPEN,                        /* idx =269 */
   DFE_HFNLECFG_NLEMODE,                         /* idx =26A */
   DFE_HFNLECFG_HLDBYPHDR,                       /* idx =26B */
   DFE_HFFPLLCFG_FPLLDRCFG,                      /* idx =26C */
   DFE_HFFPLLCFG_FPLLINCL,                       /* idx =26D */
   DFE_HFFPLLCFG_FPLLWTPRE,                      /* idx =26E */
   DFE_HFBITDCFG_VITRLBWEXT,                     /* idx =26F */
   DFE_HFFPLLCFG_FPLLWTDBW,                      /* idx =270 */
   DFE_HFFPLLCFG_FPLLWTEN,                       /* idx =271 */
   DFE_HFFPLLCFG_FPLLKPCH,                       /* idx =272 */
   DFE_HFFPLLCFG_FPLLKICH,                       /* idx =273 */
   DFE_HFFPLLCFG_FPLLARHE,                       /* idx =274 */
   DFE_HFFPLLCFG_FPLLJHIDE,                      /* idx =275 */
   DFE_HFFPLLCFG_FPLLJHLDE,                      /* idx =276 */
   DFE_BCACFG_BCAPWRDN,                          /* idx =277 */
   DFE_BCACFG_BCADEBNC,                          /* idx =278 */
   DFE_BCACFG_BCAHPFEN,                          /* idx =279 */
   DFE_HFBITDCFG_VITRLHDR,                       /* idx =27A */
   DFE_HFBITDCFG_BITBTDCFG,                      /* idx =27B */
   DFE_HFBITDCFG_VITPASSEN,                      /* idx =27C */
   DFE_HFBITDCFG_VITTYP,                         /* idx =27D */
   DFE_HFBITDCFG_VITRST,                         /* idx =27E */
   DFE_HFBITDCFG_VITRLHOD,                       /* idx =27F */
   DFE_HFBITDCFG_VITRLBCFG,                      /* idx =280 */
   DFE_HFBITDCFG_BITDETCFG,                      /* idx =281 */
   DFE_BKDDETCFG_BKDDDECBW,                      /* idx =282 */
   DFE_BKDDETCFG_BKDDATTBW,                      /* idx =283 */
   DFE_BKDDETCFG_BKDDATTTY,                      /* idx =284 */
   DFE_WHDDETCFG_WHDDDECBW,                      /* idx =285 */
   DFE_WHDDETCFG_WHDDATTBW,                      /* idx =286 */
   DFE_WHDDETCFG_WHDDATTTY,                      /* idx =287 */
   DFE_BLSDETCFG_BLSINVHFI,                      /* idx =288 */
   DFE_BLSDETCFG_BLSDEN,                         /* idx =289 */
   DFE_BLSDETCFG_BLSDDECBW,                      /* idx =28A */
   DFE_BLSDETCFG_BLSDATTBW,                      /* idx =28B */
   DFE_BLSDETCFG_BLSDATTTY,                      /* idx =28C */
   DFE_EBKDDTCFG_EBKDINVI,                       /* idx =28D */
   DFE_EBKDDTCFG_EBKDDEN,                        /* idx =28E */
   DFE_EBKDDTCFG_EBKDDECBW,                      /* idx =28F */
   DFE_EBKDDTCFG_EBKDATTBW,                      /* idx =290 */
   DFE_EBKDDTCFG_EBKDATTTY,                      /* idx =291 */
   DFE_EWHDDTCFG_EWHDINVI,                       /* idx =292 */
   DFE_EWHDDTCFG_EWHDDEN,                        /* idx =293 */
   DFE_EWHDDTCFG_EWHDDECBW,                      /* idx =294 */
   DFE_EWHDDTCFG_EWHDATTBW,                      /* idx =295 */
   DFE_EWHDDTCFG_EWHDATTTY,                      /* idx =296 */
   DFE_HFAMPMCFG,                                /* idx =297 */

   // COEF DEFINITIONS FOR:  DFE_DK

   // *** Specific coefs ***
   DFE_CALC_Max_Dyn_Gain,                        /* idx =298 */
   DFE_CALC_Min_Dyn_Gain,                        /* idx =299 */
   DFE_CALC_Coef_Max_dyn_Offset,                 /* idx =29A */
   DFE_CALC_Coef_Min_dyn_Offset,                 /* idx =29B */
   DFE_HFAOCDYN1_OFFDYNWR,                       /* idx =29C */
   DFE_CALC_HFHighpass_FcN1_DIGIBETA,            /* idx =29D */
   DFE_CALC_HFHighpassHigh_FcN1,                 /* idx =29E */
   DFE_CALC_HFHighpassHigh_FcN1_DIGIBETA,        /* idx =29F */
   DFE_CALC_EQTAPA1_Embossed,                    /* idx =2A0 */
   DFE_CALC_EQTAPA2_Embossed,                    /* idx =2A1 */
   DFE_CALC_TAPGAIN_Low,                         /* idx =2A2 */
   DFE_CALC_TAPGAIN_Normal,                      /* idx =2A3 */
   DFE_CALC_TAPGAIN_High,                        /* idx =2A4 */
   DFE_CALC_TAPGAIN_Embossed,                    /* idx =2A5 */
   DFE_CALC_CLIPLVL_Low,                         /* idx =2A6 */
   DFE_CALC_CLIPLVL_Normal,                      /* idx =2A7 */
   DFE_CALC_CLIPLVL_High,                        /* idx =2A8 */
   DFE_CALC_CLIPLVL_Embossed,                    /* idx =2A9 */
   DFE_CALC_M,                                   /* idx =2AA */
   DFE_BKDDETCFG_BKDINVHFI,                      /* idx =2AB */
   DFE_CALC_OFFDYNWR,                            /* idx =2AC */
   DFE_WHDDETCFG_WHDINVHFI,                      /* idx =2AD */
   // *** Generic coefs ***
   DFE_HFDETCFG1_LTHRPA,                         /* idx =2AE */
   DFE_HFDETCFG1_UTHRPA,                         /* idx =2AF */
   DFE_HFDETCFG2_DECFBWIA,                       /* idx =2B0 */
   DFE_HFDETCFG2_LTHRIA,                         /* idx =2B1 */
   DFE_HFDETCFG2_UTHRIA,                         /* idx =2B2 */
   DFE_HFPLLCFG_PLLMODE,                         /* idx =2B3 */
   DFE_HFPLLCFG_ARLCV,                           /* idx =2B4 */
   DFE_HFPLLCFG_ATTDECKO,                        /* idx =2B5 */
   DFE_HFPLLCFG_INLKTHR,                         /* idx =2B6 */
   DFE_HFNLECFG_DCYAMP,                          /* idx =2B7 */
   DFE_HFNLECFG_ATTAMP,                          /* idx =2B8 */
   DFE_BCACFG_BCANBCA,                           /* idx =2B9 */
   DFE_BCACFG_BCAPEAKBW,                         /* idx =2BA */
   DFE_BCACFG_BCASLCLVL,                         /* idx =2BB */
   DFE_BCACFG_BCALPFOR,                          /* idx =2BC */
   DFE_BCACFG_BCALPF3BW,                         /* idx =2BD */
   DFE_BCACFG_BCALPF2BW,                         /* idx =2BE */
   DFE_BCACFG_BCALPF1BW,                         /* idx =2BF */
   DFE_BCACFG_BCAHPFBW,                          /* idx =2C0 */
   DFE_BKDDETCFG_BKDDLTHR,                       /* idx =2C1 */
   DFE_BKDDETCFG_BKDDUTHR,                       /* idx =2C2 */
   DFE_WHDDETCFG_WHDDLTHR,                       /* idx =2C3 */
   DFE_WHDDETCFG_WHDDUTHR,                       /* idx =2C4 */
   DFE_BLSDETCFG_BLSDLTHR,                       /* idx =2C5 */
   DFE_BLSDETCFG_BLSDUTHR,                       /* idx =2C6 */
   DFE_EBKDDTCFG_EBKDLTHR,                       /* idx =2C7 */
   DFE_EBKDDTCFG_EBKDUTHR,                       /* idx =2C8 */
   DFE_EWHDDTCFG_EWHDLTHR,                       /* idx =2C9 */
   DFE_EWHDDTCFG_EWHDUTHR,                       /* idx =2CA */
   DFE_HFHLDDCFG,                                /* idx =2CB */

   // COEF DEFINITIONS FOR:  DFE_DS

   // *** Specific coefs ***
   DFE_CALC_EQTAPA1_Low,                         /* idx =2CC */
   DFE_CALC_EQTAPA1_Normal,                      /* idx =2CD */
   DFE_CALC_EQTAPA1_High,                        /* idx =2CE */
   DFE_CALC_EQTAPA2_Low,                         /* idx =2CF */
   DFE_CALC_EQTAPA2_Normal,                      /* idx =2D0 */
   DFE_CALC_EQTAPA2_High,                        /* idx =2D1 */
   // *** Generic coefs ***
   DFE_HFFPLLCFG_FPLLKPCFG,                      /* idx =2D2 */
   DFE_HFFPLLCFG_FPLLKICFG,                      /* idx =2D3 */

   // COEF DEFINITIONS FOR:  WBL_INIT

   // *** Specific coefs ***
   // *** Generic coefs ***
   WBL_INIT_WBLAGCCFG_WBLAGCON,                  /* idx =2D4 */
   WBL_INIT_ADCBUFCFG_WBLADCINV,                 /* idx =2D5 */
   WBL_INIT_ADCBUFCFG_WBLADCTYP,                 /* idx =2D6 */
   WBL_INIT_AMPMSCFG_INVWBL,                     /* idx =2D7 */
   WBL_INIT_AMPMSCFG_AMPLPFBYP,                  /* idx =2D8 */
   WBL_INIT_WBPLLCFG1_LPKLPFBYP,                 /* idx =2D9 */
   WBL_INIT_WBPLLCFG1_PRESETEN,                  /* idx =2DA */
   WBL_INIT_WBPLLCFG1_LPKLPFCFG,                 /* idx =2DB */
   WBL_INIT_WBPLLCFG1_LPKPCFG,                   /* idx =2DC */
   WBL_INIT_WBPLLCFG1_LPKICFG,                   /* idx =2DD */
   WBL_INIT_WBPLLCFG1_INGAINCFG,                 /* idx =2DE */
   WBL_INIT_WBPLLCFG1_INCLIPCFG,                 /* idx =2DF */
   WBL_INIT_WBPLLCFG1_INHPFCFG,                  /* idx =2E0 */
   WBL_INIT_WBPLLCFG1_INLPFCFG,                  /* idx =2E1 */
   WBL_INIT_WBPLLCFG1_INVWBLPLL,                 /* idx =2E2 */
   WBL_INIT_WBPLLCFG2_BEATFLCFG,                 /* idx =2E3 */
   WBL_INIT_WBPLLCFG2_BEATFLBYP,                 /* idx =2E4 */
   WBL_INIT_PLLLCKCFG_LCKAIDCFG,                 /* idx =2E5 */
   WBL_INIT_PLLLCKCFG_LCKAIDEN,                  /* idx =2E6 */
   WBL_INIT_ADRFLYCFG_RSTFLY,                    /* idx =2E7 */
   WBL_INIT_BDFLYCFG2_MNWDFLYEN,                 /* idx =2E8 */
   WBL_INIT_BDFLYCFG2_FRZFLYEN,                  /* idx =2E9 */
   WBL_INIT_ADFLYCFG2_ADBSYDET,                  /* idx =2EA */
   WBL_INIT_ADFLYCFG2_ADERCOON,                  /* idx =2EB */
   WBL_INIT_LPFLYCFG1_LPADRFLYVE,                /* idx =2EC */
   WBL_INIT_LPFLYCFG2_LPFRZFLY,                  /* idx =2ED */
   WBL_INIT_LPFLYCFG2_INVLPADDR,                 /* idx =2EE */
   WBL_INIT_LPFLYCFG2_LPMSKL1,                   /* idx =2EF */
   WBL_INIT_LPFLYCFG2_LPMSK2XMS,                 /* idx =2F0 */
   WBL_INIT_LPFLYCFG2_LPMSK4MSB,                 /* idx =2F1 */
   WBL_INIT_LPFLYCFG2_LPBSYDET,                  /* idx =2F2 */
   WBL_INIT_LPFLYCFG2_LPEVENODD,                 /* idx =2F3 */
   WBL_INIT_LPFLYCFG2_LPERCOON,                  /* idx =2F4 */
   WBL_INIT_BDADRCFG_BDADRCH,                    /* idx =2F5 */
   WBL_INIT_ADADRCFG_ADADRCH,                    /* idx =2F6 */
   WBL_INIT_LPWINCFG_LPWINCTR,                   /* idx =2F7 */
   WBL_INIT_LPADRCFG_LPADRCH,                    /* idx =2F8 */
   WBL_INIT_BDTIMCFG1_BDCFLKCHK,                 /* idx =2F9 */

   // COEF DEFINITIONS FOR:  WBL_FIXED

   // *** Specific coefs ***
   WBL_CALC_CDRW_CDFCMPGAI_Low,                  /* idx =2FA */
   WBL_CALC_CDRW_CDFCMPGAI_Normal,               /* idx =2FB */
   WBL_CALC_CDRW_CDFCMPGAI_High,                 /* idx =2FC */
   WBL_CALC_CDR_CDFCMPGAI_Low,                   /* idx =2FD */
   WBL_CALC_CDR_CDFCMPGAI_Normal,                /* idx =2FE */
   WBL_CALC_CDR_CDFCMPGAI_High,                  /* idx =2FF */
   WBL_CALC_Target_AGC,                          /* idx =300 */
   // *** Generic coefs ***
   WBL_ADCBUFCFG_WBLADCTYP,                      /* idx =301 */
   WBL_WBLAGCCFG_WBLAGCON,                       /* idx =302 */
   WBL_WBLAGCCFG_WBLSLWAGC,                      /* idx =303 */

   // COEF DEFINITIONS FOR:  WBL_DT

   // *** Specific coefs ***
   WBL_CALC_CDATIPPhaseGain,                     /* idx =304 */
   WBL_CALC_DTOOn,                               /* idx =305 */
   WBL_CALC_CDATIPLPF,                           /* idx =306 */
   WBL_CALC_CDATIPKP,                            /* idx =307 */
   WBL_DRTIMCFG1_HDRLEN_SLCD,                    /* idx =308 */
   WBL_DRTIMCFG1_HDRLEN_RF,                      /* idx =309 */
   WBL_DRTIMCFG1_HDRLEN_MLH,                     /* idx =30A */
   WBL_DRTIMCFG1_IWPSLCLVL_RF,                   /* idx =30B */
   WBL_DRTIMCFG2_HDRLENLG,                       /* idx =30C */
   // *** Generic coefs ***
   WBL_ADCBUFCFG_WBLADCINV,                      /* idx =30D */
   WBL_PLLLCKCFG_LCKAIDCFG,                      /* idx =30E */
   WBL_AMPMSCFG_INVWBL,                          /* idx =30F */
   WBL_AMPMSCFG_AMPLPFBYP,                       /* idx =310 */
   WBL_BDFLYCFG1_BDWDMXCFD,                      /* idx =311 */
   WBL_BDFLYCFG1_BDWDHITHR,                      /* idx =312 */
   WBL_BDFLYCFG1_BDWDLOTHR,                      /* idx =313 */
   WBL_BDFLYCFG1_BDBTMXCFD,                      /* idx =314 */
   WBL_BDFLYCFG1_BDBTHITLA,                      /* idx =315 */
   WBL_BDFLYCFG1_BDBTHITHR,                      /* idx =316 */
   WBL_BDFLYCFG1_BDBTLOTHR,                      /* idx =317 */
   WBL_BDFLYCFG2_MNWDFLYEN,                      /* idx =318 */
   WBL_BDFLYCFG2_FRZFLYEN,                       /* idx =319 */
   WBL_BDFLYCFG2_BDADMXCFD,                      /* idx =31A */
   WBL_BDFLYCFG2_BDADRHITH,                      /* idx =31B */
   WBL_BDFLYCFG2_BDADRLOTH,                      /* idx =31C */
   WBL_LPDETCFG_LPINVWBL,                        /* idx =31D */
   WBL_LPDETCFG_LPATTACK,                        /* idx =31E */
   WBL_LPDETCFG_LPDECAY,                         /* idx =31F */
   WBL_LPDETCFG_LPLPFBYP,                        /* idx =320 */
   WBL_LPDETCFG_LPDECLVL,                        /* idx =321 */
   WBL_LPDETCFG_LPWIN,                           /* idx =322 */
   WBL_LPFLYCFG1_LPWDMXCFD,                      /* idx =323 */
   WBL_LPFLYCFG1_LPWDHITHR,                      /* idx =324 */
   WBL_LPFLYCFG1_LPWDLOTHR,                      /* idx =325 */
   WBL_LPFLYCFG1_LPBTMXCFD,                      /* idx =326 */
   WBL_LPFLYCFG1_LPBTHITLA,                      /* idx =327 */
   WBL_LPFLYCFG1_LPBTHITHR,                      /* idx =328 */
   WBL_LPFLYCFG1_LPBTLOTHR,                      /* idx =329 */
   WBL_LPFLYCFG2_LPADMXCFD,                      /* idx =32A */
   WBL_LPFLYCFG2_LPADHITHR,                      /* idx =32B */
   WBL_LPFLYCFG2_LPADLOTHR,                      /* idx =32C */
   WBL_LPFLYCFG2_LPRAMXCFD,                      /* idx =32D */
   WBL_LPFLYCFG2_LPRAHITHR,                      /* idx =32E */
   WBL_LPFLYCFG2_LPRALOTHR,                      /* idx =32F */
   WBL_ADFLYCFG1_ADWDMXCFD,                      /* idx =330 */
   WBL_ADFLYCFG1_ADWDHITHR,                      /* idx =331 */
   WBL_ADFLYCFG1_ADWDLOTHR,                      /* idx =332 */
   WBL_ADFLYCFG1_ADBTMXCFD,                      /* idx =333 */
   WBL_ADFLYCFG1_ADBTHITLA,                      /* idx =334 */
   WBL_ADFLYCFG1_ADBTHITHR,                      /* idx =335 */
   WBL_ADFLYCFG1_ADBTLOTHR,                      /* idx =336 */
   WBL_ADFLYCFG2_ADBSYDET,                       /* idx =337 */
   WBL_ADFLYCFG2_ADERCOON,                       /* idx =338 */
   WBL_ADFLYCFG2_ADADMXCFD,                      /* idx =339 */
   WBL_ADFLYCFG2_ADADHITHR,                      /* idx =33A */
   WBL_ADFLYCFG2_ADADLOTHR,                      /* idx =33B */
   WBL_WBPLLCFG1_LPKLPFBYP,                      /* idx =33C */
   WBL_WBPLLCFG1_INVWBLPLL,                      /* idx =33D */
   WBL_DRTIMCFG1_HDRFLYLKT,                      /* idx =33E */
   WBL_DRTIMCFG1_MLH3TAP,                        /* idx =33F */
   WBL_DRTIMCFG1_XHDRONLGT,                      /* idx =340 */
   WBL_DRTIMCFG1_AVHDLPFOF,                      /* idx =341 */
   WBL_DRTIMCFG1_INWBINVON,                      /* idx =342 */
   WBL_DRTIMCFG1_SELHDRLEN,                      /* idx =343 */
   WBL_DRTIMCFG1_IWPSLCLVL,                      /* idx =344 */
   WBL_DRTIMCFG2_HEADSLCAVG,                     /* idx =345 */
   WBL_DRTIMCFG1_HDRFLYRD,                       /* idx =346 */
   WBL_DRTIMCFG1_SECINREVS,                      /* idx =347 */
   WBL_DRWBDCFD_DRANAPRD,                        /* idx =348 */
   WBL_DRWBDCFD_DRANAMOD,                        /* idx =349 */
   WBL_DRWBDCFD_DRINVWBL,                        /* idx =34A */
   WBL_DRWBDCFD_DRDECAY,                         /* idx =34B */
   WBL_DRWBDCFD_DRLPFBW,                         /* idx =34C */
   WBL_DRWBDCFD_DRLPFBYP,                        /* idx =34D */
   WBL_DRTIMDLY_WBDLYDEC,                        /* idx =34E */
   WBL_DRTIMDLY_BTDLYENC,                        /* idx =34F */
   WBL_DRTIMDLY_WBDLYENC,                        /* idx =350 */
   WBL_DRTIMDLY_BTDLYDEC,                        /* idx =351 */
   WBL_BDTIMCFG1_BDCFLKCHK,                      /* idx =352 */
   WBL_BDTIMCFG1_BDFBTDLY,                       /* idx =353 */
   WBL_BDTIMCFG1_BDFWBDLY,                       /* idx =354 */
   WBL_BDTIMCFG1_BDDEBTDLY,                      /* idx =355 */
   WBL_BDTIMCFG1_BDDEWBDLY,                      /* idx =356 */
   WBL_BDTIMCFG2_BDIDBTDLY,                      /* idx =357 */
   WBL_ATFLYCFG_ATSYHITHR,                       /* idx =358 */
   WBL_ATFLYCFG_ATSYLOTHR,                       /* idx =359 */
   WBL_ATFLYCFG_ATADHITHR,                       /* idx =35A */
   WBL_ATFLYCFG_ATADLOTHR,                       /* idx =35B */
   WBL_BDTIMCFG2_BDIDWBDLY,                      /* idx =35C */

   // COEF DEFINITIONS FOR:  WBL_DK

   // *** Specific coefs ***
   WBL_CALC_Max_Dyn_Gain,                        /* idx =35D */
   WBL_CALC_Min_Dyn_Gain,                        /* idx =35E */
   WBL_CALC_Offset_Freq_Factor,                  /* idx =35F */
   WBL_CALC_coef_Upper_limit_PLL,                /* idx =360 */
   WBL_CALC_coef_Lower_limit_PLL,                /* idx =361 */
   WBL_CALC_Lock_Aid_Phase,                      /* idx =362 */
   WBL_CALC_coef_Offset_Freq,                    /* idx =363 */
   WBL_CALC_coef_Offset_Freq_PIC,                /* idx =364 */
   WBL_CALC_WAGCTHRLO_PIC,                       /* idx =365 */
   WBL_CALC_WAGCTHRHI_PIC,                       /* idx =366 */
   WBL_CALC_coef_LOCK_AID_PH,                    /* idx =367 */
   WBL_CALC_INHPFCFGA_PIC,                       /* idx =368 */
   WBL_CALC_INLPFCFGA_PIC,                       /* idx =369 */
   WBL_CALC_AMPLPFBW_PIC,                        /* idx =36A */
   WBL_CALC_INCLIPCFG,                           /* idx =36B */
   WBL_CALC_MAXWAGCDYNGAIN_rel,                  /* idx =36C */
   WBL_CALC_MINWAGCDYNGAIN_rel,                  /* idx =36D */
   WBL_CALC_INHPFCFGA,                           /* idx =36E */
   WBL_CALC_INLPFCFGA,                           /* idx =36F */
   WBL_CALC_LoopKLPF_High,                       /* idx =370 */
   WBL_CALC_LoopKP_High,                         /* idx =371 */
   WBL_CALC_LoopKI_High,                         /* idx =372 */
   WBL_CALC_LoopKLPF_Normal,                     /* idx =373 */
   WBL_CALC_LoopKP_Normal,                       /* idx =374 */
   WBL_CALC_LoopKI_Normal,                       /* idx =375 */
   WBL_CALC_LoopKLPF_Low,                        /* idx =376 */
   WBL_CALC_LoopKP_Low,                          /* idx =377 */
   WBL_CALC_LoopKI_Low,                          /* idx =378 */
   WBL_CALC_AGCInpHighPassFcN1Hz,                /* idx =379 */
   WBL_CALC_PLLInpHighPassFcN1Hz,                /* idx =37A */
   WBL_CALC_LPP_DRHighPassFcN1Hz,                /* idx =37B */
   WBL_CALC_AGCInpLowPassFcN1Hz,                 /* idx =37C */
   WBL_CALC_PLLInpLowPassFcN1Hz,                 /* idx =37D */
   WBL_CALC_LPP_DRLowPassFcN1Hz,                 /* idx =37E */
   WBL_CALC_WdiffGainIndexInit,                  /* idx =37F */
   // *** Generic coefs ***
   WBL_AGCDYNGA_WBLDYNGA,                        /* idx =380 */
   WBL_WBLAGCCFG_WAGCINTBW,                      /* idx =381 */
   WBL_WBLAGCCFG_WAGCTHRHI,                      /* idx =382 */
   WBL_WBLAGCCFG_WAGCTHRLO,                      /* idx =383 */
   WBL_AMPMSCFG_AMPMSMOD,                        /* idx =384 */
   WBL_AMPMSCFG_AMPATT,                          /* idx =385 */
   WBL_AMPMSCFG_AMPLPFBW,                        /* idx =386 */
   WBL_WBPLLCFG1_LPKLPFCFG,                      /* idx =387 */
   WBL_WBPLLCFG1_INCLIPCFG,                      /* idx =388 */
   WBL_WBPLLCFG1_LPKPCFG,                        /* idx =389 */
   WBL_WBPLLCFG1_LPKICFG,                        /* idx =38A */
   WBL_WBPLLCFG1_INGAINCFG,                      /* idx =38B */
   WBL_WBPLLCFG2_GENPRSCAL,                      /* idx =38C */
   WBL_LPFLYCFG2_LPERCOON,                       /* idx =38D */
   WBL_ADFLYCFG2_ADIPDELAY,                      /* idx =38E */
   WBL_PLLLCKCFG_LCKDETPER,                      /* idx =38F */
   WBL_WBENIFCF1_WBW0CMP,                        /* idx =390 */
   WBL_WBENIFCF1_WBADRCMP,                       /* idx =391 */
   WBL_WCGCFG1_FRQCMPGAI,                        /* idx =392 */
   WBL_ATPLLCFG_CDPRSCAL,                        /* idx =393 */
   WBL_PLLLCKCFG_LCKDETTHR,                      /* idx =394 */

   // COEF DEFINITIONS FOR:  WBL_SPD

   // *** Specific coefs ***
   WBL_CALC_Freq_Upper_Limit,                    /* idx =395 */
   WBL_CALC_Freq_Lower_Limit,                    /* idx =396 */
   // *** Generic coefs ***

   // COEF DEFINITIONS FOR:  SRV_INIT

   // *** Specific coefs ***
   // *** Generic coefs ***
   SRV_INIT_LCCTL_LCTSTIRQ,                      /* idx =397 */
   SRV_INIT_LCCTL_LDGRSEL,                       /* idx =398 */
   SRV_INIT_LCCTL_LDGRPRESET,                    /* idx =399 */
   SRV_INIT_LCCTL_WGATESEL,                      /* idx =39A */
   SRV_INIT_LCCTL_WGPRESET,                      /* idx =39B */
   SRV_INIT_LCCTL_TTWEN,                         /* idx =39C */
   SRV_INIT_LCCTL_APCFDIS,                       /* idx =39D */
   SRV_INIT_LCCTL_HDRDIS,                        /* idx =39E */
   SRV_INIT_LCCTL_OPCEN,                         /* idx =39F */
   SRV_INIT_LCCTL_OPCRESET,                      /* idx =3A0 */
   SRV_INIT_LCCTL_LPCDHRWEN,                     /* idx =3A1 */
   SRV_INIT_LCCTL_LPCTHWREN,                     /* idx =3A2 */
   SRV_INIT_LCCTL_LPCTHRWEN,                     /* idx =3A3 */
   SRV_INIT_LCCTL_LPCDRSTWN,                     /* idx =3A4 */
   SRV_INIT_LCCTL_LPCDWGNHD,                     /* idx =3A5 */
   SRV_INIT_LCCTL_LPCTWGHD,                      /* idx =3A6 */
   SRV_INIT_LCCTL_LPCTHOLD,                      /* idx =3A7 */
   SRV_INIT_LCCTL_LPCDHOLD,                      /* idx =3A8 */
   SRV_INIT_LCCTL_RSTLPCD,                       /* idx =3A9 */
   SRV_INIT_LCCTL_RSTLPCT,                       /* idx =3AA */
   SRV_INIT_LCCTL_OPENTHLP,                      /* idx =3AB */
   SRV_INIT_LCCTL_OPENDLLP,                      /* idx =3AC */
   SRV_INIT_SVSPARE,                             /* idx =3AD */
   SRV_INIT_PWMSEL,                              /* idx =3AE */

   // COEF DEFINITIONS FOR:  SRV_FIXED

   // *** Specific coefs ***
   // *** Generic coefs ***
   SRV_DROPCTL_DROPPROT,                         /* idx =3AF */
   SRV_FSCNTL_FEAVE,                             /* idx =3B0 */
   SRV_FSCNTL_WIDTH,                             /* idx =3B1 */
   SRV_FSTHRES_FSTHRES,                          /* idx =3B2 */
   SRV_FSCNT_FNGT,                               /* idx =3B3 */
   SRV_FSCNT_FOKT,                               /* idx =3B4 */
   SRV_FSCNT_FSR,                                /* idx =3B5 */
   SRV_FSCNT_FGUP,                               /* idx =3B6 */
   SRV_FSCNT_FSM,                                /* idx =3B7 */
   SRV_FOCUS_BYPSCNT,                            /* idx =3B8 */
   SRV_FOCUS_BYPSEN,                             /* idx =3B9 */
   SRV_SVSET_TXMUX,                              /* idx =3BA */
   SRV_INHIBITTX_INHIPOL,                        /* idx =3BB */
   SRV_INHIBITTX_INHIEN,                         /* idx =3BC */
   SRV_SPEEDLVL,                                 /* idx =3BD */
   SRV_BRAKECNT,                                 /* idx =3BE */
   SRV_DLPRG,                                    /* idx =3BF */

   // COEF DEFINITIONS FOR:  SRV_DT

   // *** Specific coefs ***
   SRV_CALC_C_cooling_power,                     /* idx =3C0 */
   SRV_CALC_C_a_value,                           /* idx =3C1 */
   SRV_CALC_C_Power_ref_ratio_mid,               /* idx =3C2 */
   SRV_CALC_C_Power_ref_ratio_high,              /* idx =3C3 */
   SRV_CALC_C_Power_ref_ratio_rw,                /* idx =3C4 */
   SRV_CALC_C_Power_ref_ratio,                   /* idx =3C5 */
   SRV_CALC_C_offset_DAC_threshold_ON,           /* idx =3C6 */
   SRV_CALC_C_offset_DAC_threshold,              /* idx =3C7 */
   SRV_CALC_C_offset_DAC_0,                      /* idx =3C8 */
   SRV_CALC_C_offset_DAC_1,                      /* idx =3C9 */
   SRV_CALC_C_offset_DAC_2,                      /* idx =3CA */
   SRV_CALC_C_slope_DAC_threshold_ON,            /* idx =3CB */
   SRV_CALC_C_slope_DAC_threshold,               /* idx =3CC */
   SRV_CALC_C_slope_DAC_0,                       /* idx =3CD */
   SRV_CALC_C_slope_DAC_1,                       /* idx =3CE */
   SRV_CALC_C_slope_DAC_2,                       /* idx =3CF */
   SRV_CALC_C_offset_ADC_VRpower,                /* idx =3D0 */
   SRV_CALC_C_slope_ADC_VRpower,                 /* idx =3D1 */
   SRV_CALC_C_offset_ADC_VWpower,                /* idx =3D2 */
   SRV_CALC_C_slope_ADC_VWpower,                 /* idx =3D3 */
   SRV_CALC_C_Threshold_upper_limit,             /* idx =3D4 */
   SRV_CALC_C_Threshold_lower_limit,             /* idx =3D5 */
   SRV_CALC_C_Delta0_upper_limit,                /* idx =3D6 */
   SRV_CALC_C_Delta0_lower_limit,                /* idx =3D7 */
   SRV_CALC_C_Delta1_upper_limit,                /* idx =3D8 */
   SRV_CALC_C_Delta1_lower_limit,                /* idx =3D9 */
   SRV_CALC_C_Delta2_upper_limit,                /* idx =3DA */
   SRV_CALC_C_Delta2_lower_limit,                /* idx =3DB */
   SRV_CALC_C_Gthr_read,                         /* idx =3DC */
   SRV_CALC_C_Gthr_write,                        /* idx =3DD */
   SRV_CALC_C_thresh_P_gain,                     /* idx =3DE */
   SRV_CALC_C_Gdelta_write,                      /* idx =3DF */
   // *** Generic coefs ***
   SRV_SPEEDLVL_SLOWZERO,                        /* idx =3E0 */
   SRV_SPEEDLVL_BRKSLWOE,                        /* idx =3E1 */
   SRV_DROPCTL_DROPFOO,                          /* idx =3E2 */
   SRV_LCTIMDELAY_HDRSTRDLY,                     /* idx =3E3 */
   SRV_LCTIMDELAY_WGHLDDLY,                      /* idx =3E4 */
   SRV_LCTIMDELAY_APCFNDLY,                      /* idx =3E5 */
   SRV_LCTIMDELAY_TTWDELAY,                      /* idx =3E6 */
   SRV_FOCUS_FSL,                                /* idx =3E7 */
   SRV_FVSET_CONTSLOW,                           /* idx =3E8 */
   SRV_FVSET_LANDXC,                             /* idx =3E9 */
   SRV_INHIBITTX_INHISMP,                        /* idx =3EA */
   SRV_TESRCCTL_TESRCDB,                         /* idx =3EB */
   SRV_DLFSKLVL,                                 /* idx =3EC */
   SRV_DLFSBLVL,                                 /* idx =3ED */

   // COEF DEFINITIONS FOR:  SRV_DK

   // *** Specific coefs ***
   SRV_CALC_C_Read_power,                        /* idx =3EE */
   SRV_CALC_C_LD_APC_Setpoint,                   /* idx =3EF */
   SRV_CALC_C_offset_ADC_VEpower,                /* idx =3F0 */
   SRV_CALC_C_slope_ADC_VEpower,                 /* idx =3F1 */
   // *** Generic coefs ***
   SRV_LCTHRESH_LCTHMAX,                         /* idx =3F2 */
   SRV_LCDELTA0_DELTA0MAX,                       /* idx =3F3 */
   SRV_LCDELTA1_DELTA1MAX,                       /* idx =3F4 */
   SRV_LCDELTA2_DELTA2MAX,                       /* idx =3F5 */

   // COEF DEFINITIONS FOR:  SRV_SPD

   // *** Specific coefs ***
   // *** Generic coefs ***
   SRV_FVSET_FVD2,                               /* idx =3F6 */
   SRV_FVSET_FVD,                                /* idx =3F7 */
   SRV_INHIBITTX_INHITX,                         /* idx =3F8 */
   SRV_DFCTBL_DBL,                               /* idx =3F9 */
   NR_OF_COEFS

} COEFS_t;


// Declarations for the coef-register-table:

typedef enum
{
   E_CORE_AFE,
   E_CORE_DFE,
   E_CORE_WBL,
   E_CORE_SRV,
   E_CORE_SRVDSP,
   E_CORE_COUNT
} ECores;

typedef enum
{
   E_DEPENDENCY_INIT,
   E_DEPENDENCY_FIXED,
   E_DEPENDENCY_DISCGROUP,
   E_DEPENDENCY_DISCKIND,
   E_DEPENDENCY_SPEED_DISCGROUP,
   E_DEPENDENCY_SPEED_DISCKIND,
   E_DEPENDENCY_COUNT
} EDependency;

/*__packed*/ typedef struct
{
    ULONG    Programmable:1;
    ULONG    RegOffset:12;
    ULONG    RegSize:3;
    ULONG    LSB:5;
    ULONG    MSB:5;
    ULONG    eDependency:3;
    ULONG    eCore:3;
    USHORT   BitOffset;
} COEFS_REG_TABLE_t;

extern COEFS_REG_TABLE_t CoefsRegTable[NR_OF_COEFS];

typedef enum
{
   DiscGroupUnknown,
   DiscGroupCD,
   DiscGroupDVD,
   DiscGroupDVDRAM,
   DiscGroupBD,
   DiscGroupCount,
   LastDiscGroup = DiscGroupBD,
} DiscGroupT;

typedef unsigned long BlockT; // BlockT should be of the length of the biggest coef
#define BLOCK_BIT_SIZE (sizeof(BlockT) * 8)

typedef struct
{
   COEFS_t First;
   COEFS_t Last;
} CoefRangeT;

typedef struct
{
   CoefRangeT All;
//   CoefRangeT SpecificField; not required
//   CoefRangeT SpecificReg;   not required
   CoefRangeT GenericField;  // ==> GenericField and GenericReg can be merged ??? (same for specific) (also adapt coef_values.h)
   CoefRangeT GenericReg;    // ==> maybe interesting to have a different order in loading the coefs: first regs then fields
//   CoefRangeT Generic;
} CoefElementsT;

typedef struct
{
   BlockT         *Address; // 'BlockT *' because each core has it own type (BYTE,USHORT,ULONG)
   USHORT          StructSize;
   CoefElementsT   Index;
} CoefSinglePropertiesT;

typedef struct
{
   BlockT         *Address[DiscGroupCount]; // 'BlockT *' because each core has it own type (BYTE,USHORT,ULONG)
   USHORT          StructSize;
   CoefElementsT   Index;
} CoefGroupPropertiesT;

typedef CoefSinglePropertiesT CoefInitPropertiesT;
typedef CoefSinglePropertiesT CoefFixedPropertiesT;
typedef CoefGroupPropertiesT  CoefDiscGroupPropertiesT;
typedef CoefGroupPropertiesT  CoefDiscKindPropertiesT;
typedef CoefGroupPropertiesT  CoefSpeedDiscGroupPropertiesT;
typedef CoefGroupPropertiesT  CoefSpeedDiscKindPropertiesT;

typedef struct
{
  CoefInitPropertiesT           Init;
  CoefFixedPropertiesT          Fixed;
  CoefDiscGroupPropertiesT      DiscGroup;
  CoefDiscKindPropertiesT       DiscKind;
  CoefSpeedDiscGroupPropertiesT SpeedDiscGroup;
  CoefSpeedDiscKindPropertiesT  SpeedDiscKind;
} CoefDependencyPropertiesT;
extern const CoefDependencyPropertiesT CoefDependencyProperties[E_CORE_COUNT];

// all coef tables will be from this type, not from byte/ushort/ulong
/*__packed*/ typedef struct
{
   //  afe_init
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG afe_init_mux03_mx3togp1                : 0x1;
      ULONG afe_init_rf00_pdrfall                  : 0x1;
      ULONG afe_init_rf00_pdrf                     : 0x1;
      ULONG afe_init_rf00_pddpd                    : 0x1;
      ULONG afe_init_rf00_eqsvosel                 : 0x2;
      ULONG afe_init_rf00_eqdpdsel                 : 0x2;
      ULONG afe_init_rf01_rfadcten                 : 0x1;
      ULONG afe_init_rf01_rfingn                   : 0x2;
      ULONG afe_init_rf02_eqbufcur                 : 0x2;
      ULONG afe_init_rf03_bpdpdhp                  : 0x1;
      ULONG afe_init_rf03_vcmaden                  : 0x1;
      ULONG afe_init_rf03_rfaocen                  : 0x1;
      ULONG afe_init_rf03_rfagcen                  : 0x1;
      ULONG afe_init_rf03_rflndsel                 : 0x1;
      ULONG afe_init_rf03_rfhdrsel                 : 0x1;
      ULONG afe_init_rf03_rfoen                    : 0x1;
      ULONG afe_init_rf03_rfwldsel                 : 0x1;
      ULONG afe_init_rf04_rfinsel                  : 0x2;
      ULONG afe_init_rf04_rfagcreg                 : 0x5;
      ULONG afe_init_rf05_rfhssel                  : 0x2;
      ULONG afe_init_rf05_rfhsfc                   : 0x5;
      ULONG afe_init_rf05_enextres                 : 0x1;
      ULONG afe_init_rf06_rfgn                     : 0x3;
      ULONG afe_init_rf10_rfaocreg                 : 0x8;
      ULONG afe_init_rf19_pdgmibias                : 0x1;
      ULONG afe_init_rf19_rfibsel                  : 0x2;
      ULONG afe_init_rf20_dpd34inv                 : 0x1;
      ULONG afe_init_rf20_dpd12inv                 : 0x1;
      ULONG afe_init_rf21_dpdgn                    : 0x4;
      ULONG afe_init_rf21_dpdcpten                 : 0x1;
      ULONG afe_init_rf21_dpdpdmode                : 0x1;
      ULONG afe_init_rf21_dpdinfc                  : 0x2;
      ULONG afe_init_rf22_dpdhssel                 : 0x1;
      ULONG afe_init_rf22_dpdhsfch                 : 0x5;
      ULONG afe_init_rf22_dpdhsiscl                : 0x2;
      ULONG afe_init_rf23_dpdrisen                 : 0x1;
      ULONG afe_init_rf23_dpdfalen                 : 0x1;
      ULONG afe_init_rf24_dpdvcgm                  : 0x2;
      ULONG afe_init_rf25_rfgnwld                  : 0x3;
      ULONG afe_init_fcts10_aholdfe                : 0x1;
      ULONG afe_init_fcts10_fereghld               : 0x1;
      ULONG afe_init_fcts10_feinv                  : 0x1;
      ULONG afe_init_fcts14_ceinv                  : 0x1;
      ULONG afe_init_fcts14_aholdce                : 0x1;
      ULONG afe_init_fcts14_cereghld               : 0x1;
      ULONG afe_init_fcts25_tereghld               : 0x1;
      ULONG afe_init_fcts25_telndinv               : 0x1;
      ULONG afe_init_fcts25_teinv                  : 0x1;
      ULONG afe_init_fcts25_tesrvhld               : 0x1;
      ULONG afe_init_fcts29_toksel                 : 0x1;
      ULONG afe_init_fcts29_txinv                  : 0x1;
      ULONG afe_init_fcts29_txlndinv               : 0x1;
      ULONG afe_init_fcts32_fe2bypass              : 0x1;
      ULONG afe_init_fcts32_fegpadcen              : 0x1;
      ULONG afe_init_fcts32_bpfelp                 : 0x1;
      ULONG afe_init_fcts36_cefcsfsel              : 0x2;
      ULONG afe_init_fcts36_bpcelp                 : 0x1;
      ULONG afe_init_fcts38_te2bypass              : 0x1;
      ULONG afe_init_fcts38_tegpadcen              : 0x1;
      ULONG afe_init_fcts38_bptelp                 : 0x1;
      ULONG afe_init_fcts39_bptxlp                 : 0x1;
      ULONG afe_init_mb10_gupen                    : 0x1;
      ULONG afe_init_mb10_gupon                    : 0x1;
      ULONG afe_init_mb10_gupoff                   : 0x1;
      ULONG afe_init_mb10_gupinv                   : 0x1;
      ULONG afe_init_mb10_gupdly                   : 0x2;
      ULONG afe_init_mb11_attonon                  : 0x1;
      ULONG afe_init_mb11_attoff                   : 0x1;
      ULONG afe_init_mb11_attinv                   : 0x1;
      ULONG afe_init_mb11_mbgupsel                 : 0x5;
      ULONG afe_init_mb12_mbattsel                 : 0x5;
      ULONG afe_init_mb13_wldonsel                 : 0x1;
      ULONG afe_init_mb13_defecthld                : 0x1;
      ULONG afe_init_mb13_blanksel                 : 0x1;
      ULONG afe_init_mb14_mpdshon                  : 0x1;
      ULONG afe_init_mb14_mpdshlp                  : 0x1;
      ULONG afe_init_mb14_bwsel                    : 0x2;
      ULONG afe_init_mb15_bpmppolp                 : 0x1;
      ULONG afe_init_mb18_bpfeolp                  : 0x1;
      ULONG afe_init_mb19_testlpfc                 : 0x2;
      ULONG afe_init_mb19_bplp                     : 0x1;
      ULONG afe_init_mb20_bpsrfolp                 : 0x1;
      ULONG afe_init_mb21_dpdch1os                 : 0x6;
      ULONG afe_init_mb22_dpdch2os                 : 0x6;
      ULONG afe_init_sb10_sbgupsel                 : 0x5;
      ULONG afe_init_sb11_sbattsel                 : 0x5;
      ULONG afe_init_sb15_spdshon                  : 0x1;
      ULONG afe_init_sb15_spdshlp                  : 0x1;
      ULONG afe_init_sb16_bpsppo1lp                : 0x1;
      ULONG afe_init_sb18_bpsppo2lp                : 0x1;
      ULONG afe_init_sb19_sbade                    : 0x1;
      ULONG afe_init_sb19_sbadf                    : 0x1;
      ULONG afe_init_sb19_sbadg                    : 0x1;
      ULONG afe_init_sb19_sbadh                    : 0x1;
      ULONG afe_init_sb20_dpdch3os                 : 0x6;
      ULONG afe_init_sb21_dpdch4os                 : 0x6;
      ULONG afe_init_bsrp02_rxtok                  : 0x1;
      ULONG afe_init_bsrp03_phsdisck               : 0x4;
      ULONG afe_init_bsrp04_bhsdisck               : 0x4;
      ULONG afe_init_bsrp06_txos                   : 0x5;
      ULONG afe_init_bsrp07_bshld                  : 0x1;
      ULONG afe_init_bsrp08_rfvrefnos              : 0x6;
      ULONG afe_init_bsrp09_bs2bypass              : 0x1;
      ULONG afe_init_bsrp09_bsinv                  : 0x1;
      ULONG afe_init_bsrp09_bpbslp                 : 0x1;
      ULONG afe_init_bsrp12_rphld                  : 0x1;
      ULONG afe_init_bsrp14_rp2bypass              : 0x1;
      ULONG afe_init_bsrp14_rpinv                  : 0x1;
      ULONG afe_init_bsrp14_bprplp                 : 0x1;
      ULONG afe_init_bsrp18_phsbypass              : 0x1;
      ULONG afe_init_bsrp20_bhsbypass              : 0x1;
      ULONG afe_init_bsrp21_phscsel                : 0x1;
      ULONG afe_init_bsrp21_phsdis                 : 0x2;
      ULONG afe_init_bsrp21_bhscsel                : 0x1;
      ULONG afe_init_bsrp21_bhsdis                 : 0x2;
      ULONG afe_init_bsrp25_rcinv                  : 0x1;
      ULONG afe_init_bsrp26_rcth                   : 0x4;
      ULONG afe_init_bsrp27_rcthsel                : 0x1;
      ULONG afe_init_wobapc00_wshnolpon            : 0x1;
      ULONG afe_init_wobapc00_wshlpon              : 0x1;
      ULONG afe_init_wobapc25_rshnolpon            : 0x1;
      ULONG afe_init_wobapc25_rshlpon              : 0x1;
      ULONG afe_init_wobapc00_pdwsh12              : 0x1;
      ULONG afe_init_wobapc00_pdrsh34              : 0x1;
      ULONG afe_init_wobapc00_s34fdvrsl            : 0x1;
      ULONG afe_init_wobapc04_pdapcall             : 0x1;
      ULONG afe_init_wobapc04_apctest              : 0x3;
      ULONG afe_init_wobapc04_pdvref06             : 0x1;
      ULONG afe_init_wobapc04_pdfvref              : 0x1;
      ULONG afe_init_wobapc05_rgupreg              : 0x1;
      ULONG afe_init_wobapc05_regnbb               : 0x1;
      ULONG afe_init_wobapc05_regnba               : 0x1;
      ULONG afe_init_wobapc05_hpmode               : 0x1;
      ULONG afe_init_wobapc05_pdinpwbl             : 0x1;
      ULONG afe_init_wobapc05_pdwoball             : 0x1;
      ULONG afe_init_wobapc05_wbllpen              : 0x1;
      ULONG afe_init_wobapc07_rshsel               : 0x2;
      ULONG afe_init_wobapc07_wblbin               : 0x1;
      ULONG afe_init_wobapc07_wblcin               : 0x1;
      ULONG afe_init_wobapc07_wbldin               : 0x1;
      ULONG afe_init_wobapc09_radpgnb              : 0x2;
      ULONG afe_init_wobapc10_rbcpgnb              : 0x2;
      ULONG afe_init_wobapc11_radgnb               : 0x2;
      ULONG afe_init_wobapc12_rbcgnb               : 0x2;
      ULONG afe_init_wobapc13_pdreadwob            : 0x1;
      ULONG afe_init_wobapc13_wobdtpsel            : 0x2;
      ULONG afe_init_wobapc13_pdwrtwob             : 0x1;
      ULONG afe_init_wobapc14_wobsrcsel            : 0x2;
      ULONG afe_init_wobapc14_wgupreg              : 0x1;
      ULONG afe_init_wobapc14_wblrout              : 0x3;
      ULONG afe_init_wobapc14_wshsel               : 0x2;
      ULONG afe_init_wobapc15_wblwout              : 0x3;
      ULONG afe_init_wobapc19_agcbphpf             : 0x1;
      ULONG afe_init_wobapc19_wobvga3gu            : 0x1;
      ULONG afe_init_wobapc19_woblpfbw             : 0x4;
      ULONG afe_init_wobapc19_woblpfen             : 0x1;
      ULONG afe_init_wobapc20_agchpffc             : 0x3;
      ULONG afe_init_wobapc20_vrpwrlpf             : 0x1;
      ULONG afe_init_wobapc20_vwpwrlpf             : 0x1;
      ULONG afe_init_wobapc21_agchpfdf             : 0x1;
      ULONG afe_init_wobapc21_agciout              : 0x3;
      ULONG afe_init_wobapc21_bpagc3hp             : 0x1;
      ULONG afe_init_wobapc22_lppinv               : 0x1;
      ULONG afe_init_wobapc22_wobtest              : 0x4;
      ULONG afe_init_wobapc22_disclip              : 0x1;
      ULONG afe_init_wobapc22_wobhdren             : 0x1;
      ULONG afe_init_wobapc22_defecten             : 0x1;
      ULONG afe_init_wobapc23_agchold              : 0x1;
      ULONG afe_init_wobapc23_agcsfgn              : 0x1;
      ULONG afe_init_wobapc24_wadvcmsel            : 0x1;
      ULONG afe_init_wobapc25_wobvga3en            : 0x1;
      ULONG afe_init_wobapc25_wbvga3reg            : 0x5;
      ULONG afe_init_refs00_pdsvoall               : 0x1;
      ULONG afe_init_refs00_pdsvomb                : 0x1;
      ULONG afe_init_refs00_pdsvosb                : 0x1;
      ULONG afe_init_refs00_pdsvofct               : 0x1;
      ULONG afe_init_refs00_pdsvobsrp              : 0x1;
      ULONG afe_init_refs00_pdsvomux               : 0x1;
      ULONG afe_init_refs00_pdsvorefs              : 0x1;
      ULONG afe_init_refs00_rbiassel               : 0x1;
      ULONG afe_init_refs01_vrefbsel               : 0x3;
      ULONG afe_init_refs01_enbandgap              : 0x1;
      ULONG afe_init_rf02_ensrfowdn                : 0x1;
      ULONG afe_init_rf07_rfpos                    : 0x6;
      ULONG afe_init_rf08_rfobufos                 : 0x7;
      ULONG afe_init_rf09_rfvrefpos                : 0x6;
      ULONG afe_init_rf11_rfcosp                   : 0x7;
      ULONG afe_init_rf12_rfcosn                   : 0x7;
      ULONG afe_init_rf17_rfcospwld                : 0x7;
      ULONG afe_init_rf18_rfcosnwld                : 0x7;
      ULONG afe_init_rf13_rfcosplnd                : 0x7;
      ULONG afe_init_rf14_rfcosnlnd                : 0x7;
      ULONG afe_init_rf15_rfcosphdr                : 0x7;
      ULONG afe_init_rf16_rfcosnhdr                : 0x7;
      ULONG afe_init_rf19_maxibsel                 : 0x1;
      ULONG afe_init_mb00_mpposa                   : 0x8;
      ULONG afe_init_mb01_mpposb                   : 0x8;
      ULONG afe_init_mb02_feoosa                   : 0x8;
      ULONG afe_init_mb03_feoosb                   : 0x8;
      ULONG afe_init_mb04_srfosa                   : 0x8;
      ULONG afe_init_mb05_srfosb                   : 0x8;
      ULONG afe_init_mb08_mbvrefosa                : 0x8;
      ULONG afe_init_mb09_mbvrefosb                : 0x8;
      ULONG afe_init_sb00_spp1osa                  : 0x8;
      ULONG afe_init_sb01_spp1osb                  : 0x8;
      ULONG afe_init_sb03_spp2osa                  : 0x8;
      ULONG afe_init_sb04_spp2osb                  : 0x8;
      ULONG afe_init_sb06_sbadosa                  : 0x8;
      ULONG afe_init_sb07_sbadosb                  : 0x8;
      ULONG afe_init_sb12_sbvrefosa                : 0x8;
      ULONG afe_init_sb13_sbvrefosb                : 0x8;
      ULONG afe_init_fcts01_blndfcso               : 0x8;
      ULONG afe_init_fcts03_lndfcso                : 0x8;
      ULONG afe_init_fcts16_blndtrko               : 0x8;
      ULONG afe_init_fcts00_blankfcso              : 0x8;
      ULONG afe_init_fcts02_fcso                   : 0x8;
      ULONG afe_init_fcts04_wrfcso                 : 0x8;
      ULONG afe_init_fcts11_fe2osa                 : 0x8;
      ULONG afe_init_fcts12_fe2osb                 : 0x8;
      ULONG afe_init_fcts15_blanktrko              : 0x8;
      ULONG afe_init_fcts17_trko                   : 0x8;
      ULONG afe_init_fcts18_lndtrko                : 0x8;
      ULONG afe_init_fcts19_wrtrko                 : 0x8;
      ULONG afe_init_fcts26_te2osa                 : 0x8;
      ULONG afe_init_fcts27_te2osb                 : 0x8;
      ULONG afe_init_fcts34_ceos                   : 0x8;
      ULONG afe_init_fcts41_wrlndtrko              : 0x8;
      ULONG afe_init_fcts42_wrlndfcso              : 0x8;
      ULONG afe_init_bsrp00_bs2osa                 : 0x8;
      ULONG afe_init_bsrp01_bs2osb                 : 0x8;
      ULONG afe_init_bsrp11_rpos                   : 0x8;
      ULONG afe_init_bsrp14_rpdfctdly              : 0x2;
      ULONG afe_init_bsrp15_rp2os                  : 0x8;
      ULONG afe_init_bsrp17_phsos                  : 0x8;
      ULONG afe_init_bsrp19_bhsos                  : 0x8;
      ULONG afe_init_bsrp22_blankbsko              : 0x8;
      ULONG afe_init_bsrp23_bsko                   : 0x8;
      ULONG afe_init_bsrp25_ahldrppbh              : 0x1;
      ULONG afe_init_wobapc06_vrefos               : 0x8;
      ULONG afe_init_wobapc08_radosa               : 0x8;
      ULONG afe_init_wobapc16_rbcosa               : 0x8;
      ULONG afe_init_wobapc17_wados                : 0x8;
      ULONG afe_init_wobapc18_wbcos                : 0x8;
      ULONG afe_init_wobapc26_radosb               : 0x8;
      ULONG afe_init_wobapc27_rbcosb               : 0x8;
      ULONG afe_init_wobapc20_optsten              : 0x1;
      ULONG afe_init_wobapc20_apcinsel             : 0x2;
      ULONG afe_init_wobapc24                      : 0x8;
      ULONG afe_init_wobapc01                      : 0x8;
} afe_initT;

/*__packed*/ typedef struct
{
   //  afe_fixed
      // *** Specific coefs ***
      ULONG afe_calc_dvdrw_blank_srfo_th           : 0xB;
      ULONG afe_calc_bs_target_10mv                : 0x7;
      ULONG afe_calc_cdrom_srfo_th                 : 0x8;
      ULONG afe_calc_dvdrom_srfo_th                : 0x8;
      ULONG afe_calc_dvdromdl_srfo_th              : 0x7;
      ULONG afe_calc_bdrom_srfo_th                 : 0xA;
      ULONG afe_calc_bdromdl_srfo_th               : 0x9;
      ULONG afe_calc_hdromdl_srfo_th               : 0xA;
      ULONG afe_calc_hdrom_srfo_th                 : 0xA;
      ULONG afe_calc_rfvgashiftx100                : 0x6;
      ULONG afe_calc_stm_fe_th                     : 0x7;
      ULONG afe_calc_stm_cd_th                     : 0xA;
      ULONG afe_calc_stm_dvd_th                    : 0x9;
      ULONG afe_calc_stm_no_disc_th                : 0x5;
      ULONG afe_wobapc15_bdrommark                 : 0x8;
      ULONG afe_wobapc19_bdrommark                 : 0x8;
      ULONG afe_wobapc21_bdrommark                 : 0x8;
      ULONG afe_wobapc23_bdrommark                 : 0x8;
      ULONG afe_wobapc25_bdrommark                 : 0x8;
   // *** Generic coefs ***
      ULONG afe_rf00_pdrfall                       : 0x1;
      ULONG afe_rf00_pdrf                          : 0x1;
      ULONG afe_fcts13_fe2autoen                   : 0x1;
      ULONG afe_fcts28_te2autoen                   : 0x1;
      ULONG afe_bsrp02_bs2autoen                   : 0x1;
      ULONG afe_wobapc00_pdwsh12                   : 0x1;
      ULONG afe_wobapc00_pdrsh34                   : 0x1;
      ULONG afe_wobapc01_pdreadsub                 : 0x1;
      ULONG afe_wobapc02_erase1                    : 0x1;
      ULONG afe_wobapc02_apcrdgn                   : 0x1;
      ULONG afe_wobapc02_gain5open                 : 0x1;
      ULONG afe_wobapc02_gain10                    : 0x1;
      ULONG afe_wobapc02_rdampcd                   : 0x1;
      ULONG afe_wobapc02_pdrapc                    : 0x1;
      ULONG afe_wobapc03_winssel                   : 0x2;
      ULONG afe_wobapc03_pdwapc                    : 0x1;
      ULONG afe_wobapc03_fdgd2                     : 0x5;
      ULONG afe_wobapc04_pdvref06                  : 0x1;
      ULONG afe_wobapc04_pdfvref                   : 0x1;
      ULONG afe_wobapc04_pdapcall                  : 0x1;
      ULONG afe_wobapc05_wobauxen                  : 0x1;
      ULONG afe_wobapc05_hpmode                    : 0x1;
      ULONG afe_wobapc05_pdinpwbl                  : 0x1;
      ULONG afe_wobapc05_pdwoball                  : 0x1;
      ULONG afe_wobapc13_pdreadwob                 : 0x1;
      ULONG afe_wobapc21_wdiffgain                 : 0x3;
      ULONG afe_wobapc24_pdwpp12                   : 0x1;
      ULONG afe_wobapc25_wobvga3en                 : 0x1;
      ULONG afe_refs00_pdsvomux                    : 0x1;
} afe_fixedT;

/*__packed*/ typedef struct
{
   //  afe_dt
      // *** Specific coefs ***
      ULONG afe_fcts37_tebg                        : 0x5;
      ULONG afe_mb11_mbgupsel_stm                  : 0x5;
      ULONG afe_mb16_mppobg                        : 0x5;
      ULONG afe_mb18_feopgn_stm                    : 0x2;
      ULONG afe_mb19_feobg                         : 0x5;
      ULONG afe_sb16_spp1bg                        : 0x5;
      ULONG afe_sb18_spp2bg                        : 0x5;
      ULONG afe_calc_nodisc_srfo_th                : 0x7;
      ULONG afe_calc_min_fe_srfo_ratio             : 0x7;
      ULONG afe_calc_rp_pp_target_10mv             : 0x7;
      ULONG afe_calc_fe_pp_target_10mv             : 0x8;
      ULONG afe_calc_te_pp_target_10mv             : 0x8;
      ULONG afe_calc_phsslopex100                  : 0x5;
      ULONG afe_calc_bhsslope                      : 0x5;
      ULONG afe_calc_wlpfratiox10                  : 0x4;
      ULONG afe_calc_whpfratiox100                 : 0x5;
      ULONG afe_calc_dpdtargetx1000                : 0x9;
      ULONG afe_calc_mbtargetx100                  : 0x5;
      ULONG afe_calc_wobbletargetx100              : 0x7;
      ULONG afe_calc_rfsrfotargetx1000             : 0x6;
      ULONG afe_calc_pdicgrratiodbx4               : 0x5;
      ULONG afe_calc_stm_pre_th                    : 0x4;
      ULONG afe_fcts35_cebg12db_embossed           : 0x1;
   // *** Generic coefs ***
      ULONG afe_rf03_vcmaden                       : 0x1;
      ULONG afe_rf03_rflndsel                      : 0x1;
      ULONG afe_rf03_rfhdrsel                      : 0x1;
      ULONG afe_rf04_rfinsel                       : 0x2;
      ULONG afe_rf20_dpd1sel                       : 0x1;
      ULONG afe_rf20_dpd2sel                       : 0x1;
      ULONG afe_rf20_dpd3sel                       : 0x1;
      ULONG afe_rf20_dpd4sel                       : 0x1;
      ULONG afe_rf24_dpdbst                        : 0x3;
      ULONG afe_rf25_dpdcpgn                       : 0x2;
      ULONG afe_rf25_dpdcpfc                       : 0x2;
      ULONG afe_mb06_srfobga                       : 0x5;
      ULONG afe_mb07_srfobgb                       : 0x5;
      ULONG afe_mb10_gupmonoen                     : 0x1;
      ULONG afe_mb13_landsel                       : 0x1;
      ULONG afe_mb13_hdren                         : 0x1;
      ULONG afe_mb13_hdren2                        : 0x1;
      ULONG afe_mb14_svoauxen                      : 0x1;
      ULONG afe_mb15_mppopgn                       : 0x2;
      ULONG afe_mb15_mppa                          : 0x1;
      ULONG afe_mb15_mppb                          : 0x1;
      ULONG afe_mb15_mppc                          : 0x1;
      ULONG afe_mb15_mppd                          : 0x1;
      ULONG afe_mb15_mppolpset                     : 0x1;
      ULONG afe_mb18_feopgn                        : 0x2;
      ULONG afe_mb18_feolpset                      : 0x1;
      ULONG afe_mb20_srfa                          : 0x1;
      ULONG afe_mb20_srfb                          : 0x1;
      ULONG afe_mb20_srfc                          : 0x1;
      ULONG afe_mb20_srfd                          : 0x1;
      ULONG afe_mb20_srfolpset                     : 0x1;
      ULONG afe_fcts10_fehdrhld                    : 0x1;
      ULONG afe_fcts10_aholdfe                     : 0x1;
      ULONG afe_fcts10_feinv                       : 0x1;
      ULONG afe_fcts13_fe2gnreg                    : 0x5;
      ULONG afe_fcts14_cehdrhld                    : 0x1;
      ULONG afe_fcts14_ceinv                       : 0x1;
      ULONG afe_fcts14_aholdce                     : 0x1;
      ULONG afe_fcts25_tehdrhld                    : 0x1;
      ULONG afe_fcts25_telndinv                    : 0x1;
      ULONG afe_fcts25_teinv                       : 0x1;
      ULONG afe_fcts28_te2gnreg                    : 0x5;
      ULONG afe_fcts29_txinv                       : 0x1;
      ULONG afe_fcts29_txlndinv                    : 0x1;
      ULONG afe_fcts30_fefeo                       : 0x1;
      ULONG afe_fcts30_fesp2                       : 0x1;
      ULONG afe_fcts30_fesp2n                      : 0x1;
      ULONG afe_fcts30_fesp1                       : 0x1;
      ULONG afe_fcts30_fempp                       : 0x1;
      ULONG afe_fcts31_febg                        : 0x5;
      ULONG afe_fcts31_fepgn                       : 0x2;
      ULONG afe_fcts33_cempp                       : 0x1;
      ULONG afe_fcts33_cesp1                       : 0x1;
      ULONG afe_fcts33_cebg                        : 0x5;
      ULONG afe_fcts35_cepgn                       : 0x2;
      ULONG afe_fcts35_cebg12db                    : 0x1;
      ULONG afe_fcts35_cegn                        : 0x5;
      ULONG afe_fcts37_tepgn                       : 0x2;
      ULONG afe_sb02_spp1pgnb                      : 0x2;
      ULONG afe_sb02_spp1pgna                      : 0x2;
      ULONG afe_sb05_spp2pgnb                      : 0x2;
      ULONG afe_sb05_spp2pgna                      : 0x2;
      ULONG afe_sb08_sbadgna                       : 0x5;
      ULONG afe_sb09_sbadgnb                       : 0x5;
      ULONG afe_sb15_spp1fj                        : 0x1;
      ULONG afe_sb15_spp1gd                        : 0x1;
      ULONG afe_sb15_spp1fe                        : 0x1;
      ULONG afe_sb15_spp1eh                        : 0x1;
      ULONG afe_sb16_sppo1lpst                     : 0x1;
      ULONG afe_sb18_sppo2lpst                     : 0x1;
      ULONG afe_sb19_sbade                         : 0x1;
      ULONG afe_sb19_sbadf                         : 0x1;
      ULONG afe_sb19_sbadg                         : 0x1;
      ULONG afe_sb19_sbadh                         : 0x1;
      ULONG afe_sb19_sbadlpset                     : 0x1;
      ULONG afe_bsrp02_bs2gnreg                    : 0x5;
      ULONG afe_bsrp02_rppolinv                    : 0x1;
      ULONG afe_bsrp03_phsdsckof                   : 0x1;
      ULONG afe_bsrp04_bhsdsckof                   : 0x1;
      ULONG afe_bsrp07_bsbg                        : 0x5;
      ULONG afe_bsrp07_bspgn                       : 0x2;
      ULONG afe_bsrp12_rpbg                        : 0x5;
      ULONG afe_bsrp12_rppgn                       : 0x2;
      ULONG afe_bsrp13_rpgn                        : 0x5;
      ULONG afe_bsrp13_aholdrp                     : 0x1;
      ULONG afe_bsrp13_rpblnkinv                   : 0x1;
      ULONG afe_bsrp14_rpinv                       : 0x1;
      ULONG afe_bsrp18_phsgn                       : 0x5;
      ULONG afe_bsrp20_bhsgn                       : 0x5;
      ULONG afe_wobapc00_wshfpdon                  : 0x1;
      ULONG afe_wobapc00_apcwsel                   : 0x1;
      ULONG afe_wobapc00_apcrsel                   : 0x1;
      ULONG afe_wobapc01_subinsel                  : 0x2;
      ULONG afe_wobapc02_rinssel                   : 0x2;
      ULONG afe_wobapc04_fvrefsel                  : 0x2;
      ULONG afe_wobapc20_apcinsel                  : 0x2;
      ULONG afe_avgsvoset3_avghdhld                : 0x1;
      ULONG afe_wobapc24_pdwob                     : 0x1;
      ULONG afe_mb17                               : 0x8;
      ULONG afe_sb14                               : 0x8;
      ULONG afe_sb17                               : 0x8;
      ULONG afe_bsrp05                             : 0x8;
      ULONG afe_bsrp10                             : 0x8;
} afe_dtT;

/*__packed*/ typedef struct
{
   //  afe_dk
      // *** Specific coefs ***
      ULONG afe_calc_bwrfratiox100                 : 0x8;
      ULONG afe_calc_bwdpdratiox100                : 0x7;
      ULONG afe_calc_bs_gainup_blank               : 0x3;
      ULONG afe_bsrp16_rp2gn                       : 0x5;
      ULONG afe_bsrp24_blankbskg                   : 0x5;
      ULONG afe_bsrp25_bskg                        : 0x5;
      ULONG afe_fcts05_blankfcsg                   : 0x5;
      ULONG afe_fcts06_blndfcsg                    : 0x5;
      ULONG afe_fcts07_fcsg                        : 0x5;
      ULONG afe_fcts08_lndfcsg                     : 0x5;
      ULONG afe_fcts09_wrfcsg                      : 0x5;
      ULONG afe_fcts20_blanktrkg                   : 0x5;
      ULONG afe_fcts21_blndtrkg                    : 0x5;
      ULONG afe_fcts22_trkg                        : 0x5;
      ULONG afe_fcts23_lndtrkg                     : 0x5;
      ULONG afe_fcts24_wrtrkg                      : 0x5;
      ULONG afe_calc_fe_gainup_blank               : 0x3;
      ULONG afe_calc_sbad_gainup_blank             : 0x2;
      ULONG afe_calc_te_blank_gainup               : 0x2;
   // *** Generic coefs ***
      ULONG afe_rf00_enrfrp                        : 0x1;
      ULONG afe_bsrp13_rfrpsel                     : 0x1;
      ULONG afe_fcts29_tokt                        : 0x3;
      ULONG afe_fcts29_tokoff                      : 0x1;
      ULONG afe_fcts40_toklvl                      : 0x5;
      ULONG afe_wobapc05_wbllpen                   : 0x1;
      ULONG afe_wobapc07_rgupsel                   : 0x2;
      ULONG afe_wobapc09_wadgnb                    : 0x2;
      ULONG afe_wobapc09_wadgna                    : 0x2;
      ULONG afe_wobapc09_radpgna                   : 0x2;
      ULONG afe_wobapc10_wbcgnb                    : 0x2;
      ULONG afe_wobapc10_wbcgna                    : 0x2;
      ULONG afe_wobapc10_rbcpgna                   : 0x2;
      ULONG afe_wobapc11_radgna                    : 0x2;
      ULONG afe_wobapc11_wadpgnb                   : 0x2;
      ULONG afe_wobapc11_wadpgna                   : 0x2;
      ULONG afe_wobapc12_rbcgna                    : 0x2;
      ULONG afe_wobapc12_wbcpgnb                   : 0x2;
      ULONG afe_wobapc12_wbcpgna                   : 0x2;
      ULONG afe_wobapc13_wgupsel                   : 0x2;
      ULONG afe_wobapc13_wblwshsel                 : 0x2;
      ULONG afe_wobapc15_wblrshsel                 : 0x2;
      ULONG afe_wobapc15_wbllp                     : 0x3;
      ULONG afe_wobapc19_agchgain                  : 0x1;
      ULONG afe_wobapc19_wobvga3gu                 : 0x1;
      ULONG afe_wobapc23_agcsfgn                   : 0x1;
      ULONG afe_wobapc23_agcclip1                  : 0x2;
      ULONG afe_wobapc23_agcclip2                  : 0x2;
      ULONG afe_wobapc23_agcclip3                  : 0x2;
      ULONG afe_wobapc25_wbvga3reg                 : 0x5;
} afe_dkT;

/*__packed*/ typedef struct
{
   //  afe_spd
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG afe_fcts32_fefcsfsel                   : 0x2;
      ULONG afe_fcts32_felpset                     : 0x1;
      ULONG afe_fcts36_cefcsfsel                   : 0x2;
      ULONG afe_fcts38_tefcsfsel                   : 0x2;
      ULONG afe_fcts38_telpset                     : 0x1;
      ULONG afe_fcts39_txlp                        : 0x2;
      ULONG afe_fcts39_txlpset                     : 0x1;
      ULONG afe_bsrp09_bslp                        : 0x2;
      ULONG afe_bsrp09_bslpset                     : 0x1;
      ULONG afe_bsrp14_rplp                        : 0x2;
      ULONG afe_bsrp14_rplpset                     : 0x1;
} afe_spdT;

/*__packed*/ typedef struct
{
   //  afe_ds
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG afe_wobapc00_wshnolpon                 : 0x1;
      ULONG afe_wobapc00_wshlpon                   : 0x1;
      ULONG afe_wobapc25_rshnolpon                 : 0x1;
      ULONG afe_wobapc25_rshlpon                   : 0x1;
      ULONG afe_wobapc01_rshfpdon                  : 0x1;
} afe_dsT;

/*__packed*/ typedef struct
{
   //  dfe_init
      // *** Specific coefs ***
      ULONG dfe_init_hfampmcfg_pdatttyp_header     : 0x1;
      ULONG dfe_init_hfampmcfg_pdattfbw_header     : 0x3;
      ULONG dfe_init_hfampmcfg_pddfbw_header       : 0x4;
   // *** Generic coefs ***
      ULONG dfe_init_dfeadccfg_hfadcinv            : 0x1;
      ULONG dfe_init_dfeadccfg_hfadctyp            : 0x1;
      ULONG dfe_init_hfagccfg1_autorwsw            : 0x1;
      ULONG dfe_init_hfagccfg1_agcckclip           : 0x1;
      ULONG dfe_init_hfagccfg1_agcholden           : 0x1;
      ULONG dfe_init_hfagccfg1_agcon               : 0x1;
      ULONG dfe_init_hfagccfg1_slowagc             : 0x1;
      ULONG dfe_init_hfagccfg2_drhfana             : 0x1;
      ULONG dfe_init_hfagccfg2_agcdrcfg            : 0x1;
      ULONG dfe_init_hfaoccfg_dishldaoc            : 0x1;
      ULONG dfe_init_hfaoccfg_aocholden            : 0x1;
      ULONG dfe_init_hfaoccfg_aocon                : 0x1;
      ULONG dfe_init_hfaoccfg_slowaoc              : 0x1;
      ULONG dfe_init_hfaoccfg_aocdrcfg             : 0x1;
      ULONG dfe_init_hfhipass_mahpfbwh             : 0x3;
      ULONG dfe_init_hfhipass_mahpfbw              : 0x3;
      ULONG dfe_init_hfhipass_iirhpffh             : 0x1;
      ULONG dfe_init_hfhipass_iirhpfhod            : 0x1;
      ULONG dfe_init_hfhipass_iirhpfbyp            : 0x1;
      ULONG dfe_init_hfhipass_iirhpfrst            : 0x1;
      ULONG dfe_init_hfslccfg_frcslchld            : 0x1;
      ULONG dfe_init_hfslccfg_slchod               : 0x1;
      ULONG dfe_init_hfpllcfg_pllmode              : 0x3;
      ULONG dfe_init_hfslccfg_slcswonh             : 0x1;
      ULONG dfe_init_hfslccfg_slcdrcfg             : 0x3;
      ULONG dfe_init_hfadeq_adeqdis                : 0x1;
      ULONG dfe_init_hfnlecfg_nlemode              : 0x2;
      ULONG dfe_init_hffpllcfg_fpllfhld            : 0x1;
      ULONG dfe_init_hffpllcfg_fpllhod             : 0x1;
      ULONG dfe_init_hffpllcfg_fpllrst             : 0x1;
      ULONG dfe_init_hffpllcfg_fplldrcfg           : 0x1;
      ULONG dfe_init_hffpllcfg_fpllarhe            : 0x2;
      ULONG dfe_init_hffpllcfg_fplljhide           : 0x1;
      ULONG dfe_init_hffpllcfg_fplljhlde           : 0x1;
      ULONG dfe_init_hfbitdcfg_vitpassen           : 0x1;
      ULONG dfe_init_hfbitdcfg_vitrst              : 0x1;
      ULONG dfe_init_hfbitdcfg_vitrlhod            : 0x1;
      ULONG dfe_init_hfbitdcfg_bitdetcfg           : 0x2;
      ULONG dfe_init_rldjmcfg1_rldjitrst           : 0x1;
      ULONG dfe_init_icedtcfg1_icesftrst           : 0x1;
      ULONG dfe_init_wddcfg1_wdddis                : 0x1;
      ULONG dfe_init_wddcfg1_wddfilpre             : 0x8;
      ULONG dfe_init_bdrmcfg_bdrmsrst              : 0x1;
      ULONG dfe_init_bcacfg_bcapwrdn               : 0x1;
      ULONG dfe_init_bkddetcfg_bkdden              : 0x1;
      ULONG dfe_init_whddetcfg_whdden              : 0x1;
      ULONG dfe_init_blsdetcfg_blsden              : 0x1;
      ULONG dfe_init_ebkddtcfg_ebkdden             : 0x1;
      ULONG dfe_init_ewhddtcfg_ewhdden             : 0x1;
} dfe_initT;

/*__packed*/ typedef struct
{
   //  dfe_fixed
      // *** Specific coefs ***
      ULONG dfe_dfeadccfg_hfadcinv                 : 0x1;
      ULONG dfe_calc_gaindynwr                     : 0x1;
      ULONG dfe_calc_coef_max_dyn_gain             : 0x5;
      ULONG dfe_calc_coef_min_dyn_gain             : 0x5;
      ULONG dfe_calc_target_pp                     : 0xB;
      ULONG dfe_hfaoccfg_offsetinv                 : 0x1;
   // *** Generic coefs ***
      ULONG dfe_hfagccfg1_slowagc                  : 0x1;
      ULONG dfe_hfagccfg1_agcthrhi                 : 0x8;
      ULONG dfe_hfagccfg1_agcthrlo                 : 0x8;
      ULONG dfe_hfaoccfg_slowaoc                   : 0x1;
      ULONG dfe_hfaoccfg_offsetbnd                 : 0x6;
      ULONG dfe_hfhipass_iirhpffh                  : 0x1;
      ULONG dfe_hfhipass_iirhpfhod                 : 0x1;
      ULONG dfe_hfhipass_iirhpfbyp                 : 0x1;
      ULONG dfe_hfhipass_iirhpfrst                 : 0x1;
      ULONG dfe_bkddetcfg_bkdden                   : 0x1;
      ULONG dfe_whddetcfg_whdden                   : 0x1;
      ULONG dfe_hfagccfg2_agchbwe                  : 0x1;
      ULONG dfe_hfagccfg2_agchbwprd                : 0x3;
      ULONG dfe_hfagccfg2_agchbw                   : 0x4;
      ULONG dfe_hfagcdyn1_preautod                 : 0x1;
      ULONG dfe_hfaocdyn3_aochbwe                  : 0x1;
      ULONG dfe_hfaocdyn3_aochbwprd                : 0x3;
      ULONG dfe_hfaocdyn3_aochbw                   : 0x4;
      ULONG dfe_hfbitdcfg_bitdethcfg               : 0x2;
      ULONG dfe_hfbitdcfg_vitrlheadr               : 0x1;
} dfe_fixedT;

/*__packed*/ typedef struct
{
   //  dfe_dt
      // *** Specific coefs ***
      ULONG dfe_bcacfg_bcafreq                     : 0x7;
      ULONG dfe_calc_hfhighpass_fcn1               : 0x10;
      ULONG dfe_calc_hfhighpass_fcn1_embossed_pic  : 0xF;
      ULONG dfe_calc_movavg_fcn1                   : 0x11;
      ULONG dfe_calc_movavg_fcn1_embossed          : 0x10;
      ULONG dfe_calc_movavg_fcn1_header            : 0x11;
      ULONG dfe_calc_hfaoc_offset_header           : 0x7;
      ULONG dfe_calc_hflowpass_fcn1_digibeta       : 0xF;
      ULONG dfe_calc_startdelaypercentage_digibeta : 0x6;
      ULONG dfe_calc_stopdelaypercentage_digibeta  : 0x7;
      ULONG dfe_hfampmcfg_pdattfbw_digibeta        : 0x3;
      ULONG dfe_betamscfg_avfltbw_digibeta         : 0x3;
      ULONG dfe_hfampmcfg_pddfbw_digibeta          : 0x4;
      ULONG dfe_hffpllcfg_fplldrcfg_embossed       : 0x1;
      ULONG dfe_hffpllcfg_fpllarhe_embossed        : 0x2;
      ULONG dfe_hffpllcfg_fplljhide_embossed       : 0x1;
      ULONG dfe_hffpllcfg_fplljhlde_embossed       : 0x1;
      ULONG dfe_hfslccfg_slcswonh_embossed         : 0x1;
      ULONG dfe_hfslccfg_slcdrcfg_embossed         : 0x3;
   // *** Generic coefs ***
      ULONG dfe_hfagccfg1_agcwinprd                : 0x3;
      ULONG dfe_hfagccfg1_dishldagc                : 0x1;
      ULONG dfe_hfagccfg1_agcintbw                 : 0x4;
      ULONG dfe_hfagccfg2_agcswonh                 : 0x1;
      ULONG dfe_hfagccfg2_agcintbwh                : 0x4;
      ULONG dfe_hfagccfg2_agcwinprh                : 0x3;
      ULONG dfe_hfagccfg2_agcdrcfg                 : 0x1;
      ULONG dfe_hfaoccfg_aocwinprd                 : 0x3;
      ULONG dfe_hfaoccfg_dishldaoc                 : 0x1;
      ULONG dfe_hfaoccfg_aocintbw                  : 0x4;
      ULONG dfe_hfaoccfg_aocwinprh                 : 0x3;
      ULONG dfe_hfaoccfg_aocintbwh                 : 0x4;
      ULONG dfe_hfaoccfg_aocswonh                  : 0x1;
      ULONG dfe_hfaoccfg_aocdrcfg                  : 0x1;
      ULONG dfe_hfdetcfg1_attfbwpa                 : 0x4;
      ULONG dfe_hfdetcfg1_decfbwpa                 : 0x4;
      ULONG dfe_hfdetcfg2_mskdr                    : 0x1;
      ULONG dfe_hfhipass_picfilbp                  : 0x1;
      ULONG dfe_hfhipass_mahpfdcfg                 : 0x1;
      ULONG dfe_hfhipass_mahpfsomh                 : 0x1;
      ULONG dfe_hfhipass_mahpfbwh                  : 0x3;
      ULONG dfe_hfhipass_mahpfswh                  : 0x1;
      ULONG dfe_hfhipass_mahpfsom                  : 0x1;
      ULONG dfe_hfslccfg_slcbwhi                   : 0x4;
      ULONG dfe_hfslccfg_slcswonh                  : 0x1;
      ULONG dfe_hfslccfg_slcdrcfg                  : 0x3;
      ULONG dfe_hfslccfg_slcbwh                    : 0x4;
      ULONG dfe_hfslccfg_slcbw                     : 0x4;
      ULONG dfe_hfslccfg_slclvlb                   : 0x4;
      ULONG dfe_hfpllcfg_innlkthr                  : 0x4;
      ULONG dfe_hfpllcfg_bdlkaidm                  : 0x1;
      ULONG dfe_hfnlecfg_hldaclod                  : 0x1;
      ULONG dfe_hfnlecfg_auclipen                  : 0x1;
      ULONG dfe_hfnlecfg_nlemode                   : 0x2;
      ULONG dfe_hfnlecfg_hldbyphdr                 : 0x1;
      ULONG dfe_hffpllcfg_fplldrcfg                : 0x1;
      ULONG dfe_hffpllcfg_fpllincl                 : 0x7;
      ULONG dfe_hffpllcfg_fpllwtpre                : 0x1;
      ULONG dfe_hfbitdcfg_vitrlbwext               : 0x1;
      ULONG dfe_hffpllcfg_fpllwtdbw                : 0x2;
      ULONG dfe_hffpllcfg_fpllwten                 : 0x1;
      ULONG dfe_hffpllcfg_fpllkpch                 : 0x3;
      ULONG dfe_hffpllcfg_fpllkich                 : 0x3;
      ULONG dfe_hffpllcfg_fpllarhe                 : 0x2;
      ULONG dfe_hffpllcfg_fplljhide                : 0x1;
      ULONG dfe_hffpllcfg_fplljhlde                : 0x1;
      ULONG dfe_bcacfg_bcapwrdn                    : 0x1;
      ULONG dfe_bcacfg_bcadebnc                    : 0x1;
      ULONG dfe_bcacfg_bcahpfen                    : 0x1;
      ULONG dfe_hfbitdcfg_vitrlhdr                 : 0x1;
      ULONG dfe_hfbitdcfg_bitbtdcfg                : 0x1;
      ULONG dfe_hfbitdcfg_vitpassen                : 0x1;
      ULONG dfe_hfbitdcfg_vittyp                   : 0x1;
      ULONG dfe_hfbitdcfg_vitrst                   : 0x1;
      ULONG dfe_hfbitdcfg_vitrlhod                 : 0x1;
      ULONG dfe_hfbitdcfg_vitrlbcfg                : 0x2;
      ULONG dfe_hfbitdcfg_bitdetcfg                : 0x2;
      ULONG dfe_bkddetcfg_bkdddecbw                : 0x4;
      ULONG dfe_bkddetcfg_bkddattbw                : 0x3;
      ULONG dfe_bkddetcfg_bkddattty                : 0x1;
      ULONG dfe_whddetcfg_whdddecbw                : 0x4;
      ULONG dfe_whddetcfg_whddattbw                : 0x3;
      ULONG dfe_whddetcfg_whddattty                : 0x1;
      ULONG dfe_blsdetcfg_blsinvhfi                : 0x1;
      ULONG dfe_blsdetcfg_blsden                   : 0x1;
      ULONG dfe_blsdetcfg_blsddecbw                : 0x4;
      ULONG dfe_blsdetcfg_blsdattbw                : 0x3;
      ULONG dfe_blsdetcfg_blsdattty                : 0x1;
      ULONG dfe_ebkddtcfg_ebkdinvi                 : 0x1;
      ULONG dfe_ebkddtcfg_ebkdden                  : 0x1;
      ULONG dfe_ebkddtcfg_ebkddecbw                : 0x4;
      ULONG dfe_ebkddtcfg_ebkdattbw                : 0x3;
      ULONG dfe_ebkddtcfg_ebkdattty                : 0x1;
      ULONG dfe_ewhddtcfg_ewhdinvi                 : 0x1;
      ULONG dfe_ewhddtcfg_ewhdden                  : 0x1;
      ULONG dfe_ewhddtcfg_ewhddecbw                : 0x4;
      ULONG dfe_ewhddtcfg_ewhdattbw                : 0x3;
      ULONG dfe_ewhddtcfg_ewhdattty                : 0x1;
      ULONG dfe_hfampmcfg                          : 0x20;
} dfe_dtT;

/*__packed*/ typedef struct
{
   //  dfe_dk
      // *** Specific coefs ***
      ULONG dfe_calc_max_dyn_gain                  : 0x5;
      ULONG dfe_calc_min_dyn_gain                  : 0x5;
      ULONG dfe_calc_coef_max_dyn_offset           : 0x7;
      ULONG dfe_calc_coef_min_dyn_offset           : 0x7;
      ULONG dfe_hfaocdyn1_offdynwr                 : 0x8;
      ULONG dfe_calc_hfhighpass_fcn1_digibeta      : 0xF;
      ULONG dfe_calc_hfhighpasshigh_fcn1           : 0x11;
      ULONG dfe_calc_hfhighpasshigh_fcn1_digibeta  : 0x12;
      ULONG dfe_calc_eqtapa1_embossed              : 0x2;
      ULONG dfe_calc_eqtapa2_embossed              : 0x2;
      ULONG dfe_calc_tapgain_low                   : 0x6;
      ULONG dfe_calc_tapgain_normal                : 0x6;
      ULONG dfe_calc_tapgain_high                  : 0x6;
      ULONG dfe_calc_tapgain_embossed              : 0x6;
      ULONG dfe_calc_cliplvl_low                   : 0x6;
      ULONG dfe_calc_cliplvl_normal                : 0x6;
      ULONG dfe_calc_cliplvl_high                  : 0x6;
      ULONG dfe_calc_cliplvl_embossed              : 0x5;
      ULONG dfe_calc_m                             : 0x7;
      ULONG dfe_bkddetcfg_bkdinvhfi                : 0x1;
      ULONG dfe_calc_offdynwr                      : 0x7;
      ULONG dfe_whddetcfg_whdinvhfi                : 0x1;
   // *** Generic coefs ***
      ULONG dfe_hfdetcfg1_lthrpa                   : 0x8;
      ULONG dfe_hfdetcfg1_uthrpa                   : 0x8;
      ULONG dfe_hfdetcfg2_decfbwia                 : 0x4;
      ULONG dfe_hfdetcfg2_lthria                   : 0x8;
      ULONG dfe_hfdetcfg2_uthria                   : 0x8;
      ULONG dfe_hfpllcfg_pllmode                   : 0x3;
      ULONG dfe_hfpllcfg_arlcv                     : 0x8;
      ULONG dfe_hfpllcfg_attdecko                  : 0x2;
      ULONG dfe_hfpllcfg_inlkthr                   : 0x6;
      ULONG dfe_hfnlecfg_dcyamp                    : 0x4;
      ULONG dfe_hfnlecfg_attamp                    : 0x4;
      ULONG dfe_bcacfg_bcanbca                     : 0x1;
      ULONG dfe_bcacfg_bcapeakbw                   : 0x3;
      ULONG dfe_bcacfg_bcaslclvl                   : 0x2;
      ULONG dfe_bcacfg_bcalpfor                    : 0x2;
      ULONG dfe_bcacfg_bcalpf3bw                   : 0x2;
      ULONG dfe_bcacfg_bcalpf2bw                   : 0x2;
      ULONG dfe_bcacfg_bcalpf1bw                   : 0x2;
      ULONG dfe_bcacfg_bcahpfbw                    : 0x2;
      ULONG dfe_bkddetcfg_bkddlthr                 : 0x8;
      ULONG dfe_bkddetcfg_bkdduthr                 : 0x8;
      ULONG dfe_whddetcfg_whddlthr                 : 0x8;
      ULONG dfe_whddetcfg_whdduthr                 : 0x8;
      ULONG dfe_blsdetcfg_blsdlthr                 : 0x8;
      ULONG dfe_blsdetcfg_blsduthr                 : 0x8;
      ULONG dfe_ebkddtcfg_ebkdlthr                 : 0x8;
      ULONG dfe_ebkddtcfg_ebkduthr                 : 0x8;
      ULONG dfe_ewhddtcfg_ewhdlthr                 : 0x8;
      ULONG dfe_ewhddtcfg_ewhduthr                 : 0x8;
      ULONG dfe_hfhlddcfg                          : 0x20;
} dfe_dkT;

/*__packed*/ typedef struct
{
   //  dfe_ds
      // *** Specific coefs ***
      ULONG dfe_calc_eqtapa1_low                   : 0x4;
      ULONG dfe_calc_eqtapa1_normal                : 0x4;
      ULONG dfe_calc_eqtapa1_high                  : 0x4;
      ULONG dfe_calc_eqtapa2_low                   : 0x4;
      ULONG dfe_calc_eqtapa2_normal                : 0x4;
      ULONG dfe_calc_eqtapa2_high                  : 0x4;
   // *** Generic coefs ***
      ULONG dfe_hffpllcfg_fpllkpcfg                : 0x3;
      ULONG dfe_hffpllcfg_fpllkicfg                : 0x3;
} dfe_dsT;

/*__packed*/ typedef struct
{
   //  wbl_init
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG wbl_init_wblagccfg_wblagcon            : 0x1;
      ULONG wbl_init_adcbufcfg_wbladcinv           : 0x1;
      ULONG wbl_init_adcbufcfg_wbladctyp           : 0x1;
      ULONG wbl_init_ampmscfg_invwbl               : 0x1;
      ULONG wbl_init_ampmscfg_amplpfbyp            : 0x1;
      ULONG wbl_init_wbpllcfg1_lpklpfbyp           : 0x1;
      ULONG wbl_init_wbpllcfg1_preseten            : 0x1;
      ULONG wbl_init_wbpllcfg1_lpklpfcfg           : 0x3;
      ULONG wbl_init_wbpllcfg1_lpkpcfg             : 0x4;
      ULONG wbl_init_wbpllcfg1_lpkicfg             : 0x4;
      ULONG wbl_init_wbpllcfg1_ingaincfg           : 0x2;
      ULONG wbl_init_wbpllcfg1_inclipcfg           : 0x3;
      ULONG wbl_init_wbpllcfg1_inhpfcfg            : 0x4;
      ULONG wbl_init_wbpllcfg1_inlpfcfg            : 0x3;
      ULONG wbl_init_wbpllcfg1_invwblpll           : 0x1;
      ULONG wbl_init_wbpllcfg2_beatflcfg           : 0x2;
      ULONG wbl_init_wbpllcfg2_beatflbyp           : 0x1;
      ULONG wbl_init_plllckcfg_lckaidcfg           : 0x1;
      ULONG wbl_init_plllckcfg_lckaiden            : 0x1;
      ULONG wbl_init_adrflycfg_rstfly              : 0x1;
      ULONG wbl_init_bdflycfg2_mnwdflyen           : 0x1;
      ULONG wbl_init_bdflycfg2_frzflyen            : 0x1;
      ULONG wbl_init_adflycfg2_adbsydet            : 0x1;
      ULONG wbl_init_adflycfg2_adercoon            : 0x1;
      ULONG wbl_init_lpflycfg1_lpadrflyve          : 0x1;
      ULONG wbl_init_lpflycfg2_lpfrzfly            : 0x1;
      ULONG wbl_init_lpflycfg2_invlpaddr           : 0x1;
      ULONG wbl_init_lpflycfg2_lpmskl1             : 0x1;
      ULONG wbl_init_lpflycfg2_lpmsk2xms           : 0x1;
      ULONG wbl_init_lpflycfg2_lpmsk4msb           : 0x1;
      ULONG wbl_init_lpflycfg2_lpbsydet            : 0x1;
      ULONG wbl_init_lpflycfg2_lpevenodd           : 0x1;
      ULONG wbl_init_lpflycfg2_lpercoon            : 0x1;
      ULONG wbl_init_bdadrcfg_bdadrch              : 0x18;
      ULONG wbl_init_adadrcfg_adadrch              : 0x18;
      ULONG wbl_init_lpwincfg_lpwinctr             : 0x8;
      ULONG wbl_init_lpadrcfg_lpadrch              : 0x18;
      ULONG wbl_init_bdtimcfg1_bdcflkchk           : 0x1;
} wbl_initT;

/*__packed*/ typedef struct
{
   //  wbl_fixed
      // *** Specific coefs ***
      ULONG wbl_calc_cdrw_cdfcmpgai_low            : 0x3;
      ULONG wbl_calc_cdrw_cdfcmpgai_normal         : 0x3;
      ULONG wbl_calc_cdrw_cdfcmpgai_high           : 0x3;
      ULONG wbl_calc_cdr_cdfcmpgai_low             : 0x3;
      ULONG wbl_calc_cdr_cdfcmpgai_normal          : 0x3;
      ULONG wbl_calc_cdr_cdfcmpgai_high            : 0x3;
      ULONG wbl_calc_target_agc                    : 0x3;
   // *** Generic coefs ***
      ULONG wbl_adcbufcfg_wbladctyp                : 0x1;
      ULONG wbl_wblagccfg_wblagcon                 : 0x1;
      ULONG wbl_wblagccfg_wblslwagc                : 0x1;
} wbl_fixedT;

/*__packed*/ typedef struct
{
   //  wbl_dt
      // *** Specific coefs ***
      ULONG wbl_calc_cdatipphasegain               : 0x1;
      ULONG wbl_calc_dtoon                         : 0x1;
      ULONG wbl_calc_cdatiplpf                     : 0x3;
      ULONG wbl_calc_cdatipkp                      : 0x4;
      ULONG wbl_drtimcfg1_hdrlen_slcd              : 0x7;
      ULONG wbl_drtimcfg1_hdrlen_rf                : 0x7;
      ULONG wbl_drtimcfg1_hdrlen_mlh               : 0x7;
      ULONG wbl_drtimcfg1_iwpslclvl_rf             : 0x3;
      ULONG wbl_drtimcfg2_hdrlenlg                 : 0x7;
   // *** Generic coefs ***
      ULONG wbl_adcbufcfg_wbladcinv                : 0x1;
      ULONG wbl_plllckcfg_lckaidcfg                : 0x1;
      ULONG wbl_ampmscfg_invwbl                    : 0x1;
      ULONG wbl_ampmscfg_amplpfbyp                 : 0x1;
      ULONG wbl_bdflycfg1_bdwdmxcfd                : 0x4;
      ULONG wbl_bdflycfg1_bdwdhithr                : 0x4;
      ULONG wbl_bdflycfg1_bdwdlothr                : 0x4;
      ULONG wbl_bdflycfg1_bdbtmxcfd                : 0x4;
      ULONG wbl_bdflycfg1_bdbthitla                : 0x4;
      ULONG wbl_bdflycfg1_bdbthithr                : 0x4;
      ULONG wbl_bdflycfg1_bdbtlothr                : 0x4;
      ULONG wbl_bdflycfg2_mnwdflyen                : 0x1;
      ULONG wbl_bdflycfg2_frzflyen                 : 0x1;
      ULONG wbl_bdflycfg2_bdadmxcfd                : 0x4;
      ULONG wbl_bdflycfg2_bdadrhith                : 0x4;
      ULONG wbl_bdflycfg2_bdadrloth                : 0x4;
      ULONG wbl_lpdetcfg_lpinvwbl                  : 0x1;
      ULONG wbl_lpdetcfg_lpattack                  : 0x3;
      ULONG wbl_lpdetcfg_lpdecay                   : 0x3;
      ULONG wbl_lpdetcfg_lplpfbyp                  : 0x1;
      ULONG wbl_lpdetcfg_lpdeclvl                  : 0x3;
      ULONG wbl_lpdetcfg_lpwin                     : 0x2;
      ULONG wbl_lpflycfg1_lpwdmxcfd                : 0x4;
      ULONG wbl_lpflycfg1_lpwdhithr                : 0x4;
      ULONG wbl_lpflycfg1_lpwdlothr                : 0x4;
      ULONG wbl_lpflycfg1_lpbtmxcfd                : 0x4;
      ULONG wbl_lpflycfg1_lpbthitla                : 0x4;
      ULONG wbl_lpflycfg1_lpbthithr                : 0x4;
      ULONG wbl_lpflycfg1_lpbtlothr                : 0x4;
      ULONG wbl_lpflycfg2_lpadmxcfd                : 0x4;
      ULONG wbl_lpflycfg2_lpadhithr                : 0x4;
      ULONG wbl_lpflycfg2_lpadlothr                : 0x4;
      ULONG wbl_lpflycfg2_lpramxcfd                : 0x4;
      ULONG wbl_lpflycfg2_lprahithr                : 0x4;
      ULONG wbl_lpflycfg2_lpralothr                : 0x4;
      ULONG wbl_adflycfg1_adwdmxcfd                : 0x4;
      ULONG wbl_adflycfg1_adwdhithr                : 0x4;
      ULONG wbl_adflycfg1_adwdlothr                : 0x4;
      ULONG wbl_adflycfg1_adbtmxcfd                : 0x4;
      ULONG wbl_adflycfg1_adbthitla                : 0x4;
      ULONG wbl_adflycfg1_adbthithr                : 0x4;
      ULONG wbl_adflycfg1_adbtlothr                : 0x4;
      ULONG wbl_adflycfg2_adbsydet                 : 0x1;
      ULONG wbl_adflycfg2_adercoon                 : 0x1;
      ULONG wbl_adflycfg2_adadmxcfd                : 0x4;
      ULONG wbl_adflycfg2_adadhithr                : 0x4;
      ULONG wbl_adflycfg2_adadlothr                : 0x4;
      ULONG wbl_wbpllcfg1_lpklpfbyp                : 0x1;
      ULONG wbl_wbpllcfg1_invwblpll                : 0x1;
      ULONG wbl_drtimcfg1_hdrflylkt                : 0x4;
      ULONG wbl_drtimcfg1_mlh3tap                  : 0x1;
      ULONG wbl_drtimcfg1_xhdronlgt                : 0x1;
      ULONG wbl_drtimcfg1_avhdlpfof                : 0x1;
      ULONG wbl_drtimcfg1_inwbinvon                : 0x1;
      ULONG wbl_drtimcfg1_selhdrlen                : 0x1;
      ULONG wbl_drtimcfg1_iwpslclvl                : 0x3;
      ULONG wbl_drtimcfg2_headslcavg               : 0x3;
      ULONG wbl_drtimcfg1_hdrflyrd                 : 0x1;
      ULONG wbl_drtimcfg1_secinrevs                : 0x2;
      ULONG wbl_drwbdcfd_dranaprd                  : 0x1;
      ULONG wbl_drwbdcfd_dranamod                  : 0x1;
      ULONG wbl_drwbdcfd_drinvwbl                  : 0x1;
      ULONG wbl_drwbdcfd_drdecay                   : 0x3;
      ULONG wbl_drwbdcfd_drlpfbw                   : 0x3;
      ULONG wbl_drwbdcfd_drlpfbyp                  : 0x1;
      ULONG wbl_drtimdly_wbdlydec                  : 0x3;
      ULONG wbl_drtimdly_btdlyenc                  : 0x8;
      ULONG wbl_drtimdly_wbdlyenc                  : 0x3;
      ULONG wbl_drtimdly_btdlydec                  : 0x8;
      ULONG wbl_bdtimcfg1_bdcflkchk                : 0x1;
      ULONG wbl_bdtimcfg1_bdfbtdly                 : 0x7;
      ULONG wbl_bdtimcfg1_bdfwbdly                 : 0x6;
      ULONG wbl_bdtimcfg1_bddebtdly                : 0x7;
      ULONG wbl_bdtimcfg1_bddewbdly                : 0x6;
      ULONG wbl_bdtimcfg2_bdidbtdly                : 0x7;
      ULONG wbl_atflycfg_atsyhithr                 : 0x3;
      ULONG wbl_atflycfg_atsylothr                 : 0x3;
      ULONG wbl_atflycfg_atadhithr                 : 0x3;
      ULONG wbl_atflycfg_atadlothr                 : 0x3;
      ULONG wbl_bdtimcfg2_bdidwbdly                : 0x6;
} wbl_dtT;

/*__packed*/ typedef struct
{
   //  wbl_dk
      // *** Specific coefs ***
      ULONG wbl_calc_max_dyn_gain                  : 0x4;
      ULONG wbl_calc_min_dyn_gain                  : 0x4;
      ULONG wbl_calc_offset_freq_factor            : 0x7;
      ULONG wbl_calc_coef_upper_limit_pll          : 0x8;
      ULONG wbl_calc_coef_lower_limit_pll          : 0x7;
      ULONG wbl_calc_lock_aid_phase                : 0x8;
      ULONG wbl_calc_coef_offset_freq              : 0x7;
      ULONG wbl_calc_coef_offset_freq_pic          : 0x8;
      ULONG wbl_calc_wagcthrlo_pic                 : 0x8;
      ULONG wbl_calc_wagcthrhi_pic                 : 0x8;
      ULONG wbl_calc_coef_lock_aid_ph              : 0x8;
      ULONG wbl_calc_inhpfcfga_pic                 : 0x3;
      ULONG wbl_calc_inlpfcfga_pic                 : 0x3;
      ULONG wbl_calc_amplpfbw_pic                  : 0x3;
      ULONG wbl_calc_inclipcfg                     : 0x1;
      ULONG wbl_calc_maxwagcdyngain_rel            : 0x1;
      ULONG wbl_calc_minwagcdyngain_rel            : 0x1;
      ULONG wbl_calc_inhpfcfga                     : 0x3;
      ULONG wbl_calc_inlpfcfga                     : 0x3;
      ULONG wbl_calc_loopklpf_high                 : 0x3;
      ULONG wbl_calc_loopkp_high                   : 0x4;
      ULONG wbl_calc_loopki_high                   : 0x4;
      ULONG wbl_calc_loopklpf_normal               : 0x3;
      ULONG wbl_calc_loopkp_normal                 : 0x4;
      ULONG wbl_calc_loopki_normal                 : 0x3;
      ULONG wbl_calc_loopklpf_low                  : 0x3;
      ULONG wbl_calc_loopkp_low                    : 0x4;
      ULONG wbl_calc_loopki_low                    : 0x3;
      ULONG wbl_calc_agcinphighpassfcn1hz          : 0x11;
      ULONG wbl_calc_pllinphighpassfcn1hz          : 0x11;
      ULONG wbl_calc_lpp_drhighpassfcn1hz          : 0x11;
      ULONG wbl_calc_agcinplowpassfcn1hz           : 0x17;
      ULONG wbl_calc_pllinplowpassfcn1hz           : 0x17;
      ULONG wbl_calc_lpp_drlowpassfcn1hz           : 0x19;
      ULONG wbl_calc_wdiffgainindexinit            : 0x1;
   // *** Generic coefs ***
      ULONG wbl_agcdynga_wbldynga                  : 0x5;
      ULONG wbl_wblagccfg_wagcintbw                : 0x4;
      ULONG wbl_wblagccfg_wagcthrhi                : 0x8;
      ULONG wbl_wblagccfg_wagcthrlo                : 0x8;
      ULONG wbl_ampmscfg_ampmsmod                  : 0x2;
      ULONG wbl_ampmscfg_ampatt                    : 0x3;
      ULONG wbl_ampmscfg_amplpfbw                  : 0x3;
      ULONG wbl_wbpllcfg1_lpklpfcfg                : 0x3;
      ULONG wbl_wbpllcfg1_inclipcfg                : 0x3;
      ULONG wbl_wbpllcfg1_lpkpcfg                  : 0x4;
      ULONG wbl_wbpllcfg1_lpkicfg                  : 0x4;
      ULONG wbl_wbpllcfg1_ingaincfg                : 0x2;
      ULONG wbl_wbpllcfg2_genprscal                : 0x3;
      ULONG wbl_lpflycfg2_lpercoon                 : 0x1;
      ULONG wbl_adflycfg2_adipdelay                : 0x1;
      ULONG wbl_plllckcfg_lckdetper                : 0x4;
      ULONG wbl_wbenifcf1_wbw0cmp                  : 0x8;
      ULONG wbl_wbenifcf1_wbadrcmp                 : 0x8;
      ULONG wbl_wcgcfg1_frqcmpgai                  : 0x4;
      ULONG wbl_atpllcfg_cdprscal                  : 0x3;
      ULONG wbl_plllckcfg_lckdetthr                : 0x6;
} wbl_dkT;

/*__packed*/ typedef struct
{
   //  wbl_spd
      // *** Specific coefs ***
      ULONG wbl_calc_freq_upper_limit              : 0x8;
      ULONG wbl_calc_freq_lower_limit              : 0x6;
   // *** Generic coefs ***
} wbl_spdT;

/*__packed*/ typedef struct
{
   //  srv_init
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG srv_init_lcctl_lctstirq                : 0x1;
      ULONG srv_init_lcctl_ldgrsel                 : 0x1;
      ULONG srv_init_lcctl_ldgrpreset              : 0x1;
      ULONG srv_init_lcctl_wgatesel                : 0x1;
      ULONG srv_init_lcctl_wgpreset                : 0x1;
      ULONG srv_init_lcctl_ttwen                   : 0x1;
      ULONG srv_init_lcctl_apcfdis                 : 0x1;
      ULONG srv_init_lcctl_hdrdis                  : 0x1;
      ULONG srv_init_lcctl_opcen                   : 0x1;
      ULONG srv_init_lcctl_opcreset                : 0x1;
      ULONG srv_init_lcctl_lpcdhrwen               : 0x1;
      ULONG srv_init_lcctl_lpcthwren               : 0x1;
      ULONG srv_init_lcctl_lpcthrwen               : 0x1;
      ULONG srv_init_lcctl_lpcdrstwn               : 0x1;
      ULONG srv_init_lcctl_lpcdwgnhd               : 0x1;
      ULONG srv_init_lcctl_lpctwghd                : 0x1;
      ULONG srv_init_lcctl_lpcthold                : 0x1;
      ULONG srv_init_lcctl_lpcdhold                : 0x1;
      ULONG srv_init_lcctl_rstlpcd                 : 0x1;
      ULONG srv_init_lcctl_rstlpct                 : 0x1;
      ULONG srv_init_lcctl_openthlp                : 0x1;
      ULONG srv_init_lcctl_opendllp                : 0x1;
      ULONG srv_init_svspare                       : 0x20;
      ULONG srv_init_pwmsel                        : 0x20;
} srv_initT;

/*__packed*/ typedef struct
{
   //  srv_fixed
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG srv_dropctl_dropprot                   : 0x1;
      ULONG srv_fscntl_feave                       : 0x1;
      ULONG srv_fscntl_width                       : 0x1;
      ULONG srv_fsthres_fsthres                    : 0x8;
      ULONG srv_fscnt_fngt                         : 0xC;
      ULONG srv_fscnt_fokt                         : 0xC;
      ULONG srv_fscnt_fsr                          : 0x2;
      ULONG srv_fscnt_fgup                         : 0x1;
      ULONG srv_fscnt_fsm                          : 0x3;
      ULONG srv_focus_bypscnt                      : 0x3;
      ULONG srv_focus_bypsen                       : 0x1;
      ULONG srv_svset_txmux                        : 0x1;
      ULONG srv_inhibittx_inhipol                  : 0x1;
      ULONG srv_inhibittx_inhien                   : 0x1;
      ULONG srv_speedlvl                           : 0x20;
      ULONG srv_brakecnt                           : 0x20;
      ULONG srv_dlprg                              : 0x20;
} srv_fixedT;

/*__packed*/ typedef struct
{
   //  srv_dt
      // *** Specific coefs ***
      ULONG srv_calc_c_cooling_power               : 0x10;
      ULONG srv_calc_c_a_value                     : 0x10;
      ULONG srv_calc_c_power_ref_ratio_mid         : 0x7;
      ULONG srv_calc_c_power_ref_ratio_high        : 0x7;
      ULONG srv_calc_c_power_ref_ratio_rw          : 0x8;
      ULONG srv_calc_c_power_ref_ratio             : 0x10;
      ULONG srv_calc_c_offset_dac_threshold_on     : 0x10;
      ULONG srv_calc_c_offset_dac_threshold        : 0x10;
      ULONG srv_calc_c_offset_dac_0                : 0x10;
      ULONG srv_calc_c_offset_dac_1                : 0x10;
      ULONG srv_calc_c_offset_dac_2                : 0x10;
      ULONG srv_calc_c_slope_dac_threshold_on      : 0x10;
      ULONG srv_calc_c_slope_dac_threshold         : 0x10;
      ULONG srv_calc_c_slope_dac_0                 : 0x10;
      ULONG srv_calc_c_slope_dac_1                 : 0x10;
      ULONG srv_calc_c_slope_dac_2                 : 0x10;
      ULONG srv_calc_c_offset_adc_vrpower          : 0x10;
      ULONG srv_calc_c_slope_adc_vrpower           : 0x10;
      ULONG srv_calc_c_offset_adc_vwpower          : 0x10;
      ULONG srv_calc_c_slope_adc_vwpower           : 0x10;
      ULONG srv_calc_c_threshold_upper_limit       : 0x10;
      ULONG srv_calc_c_threshold_lower_limit       : 0x10;
      ULONG srv_calc_c_delta0_upper_limit          : 0xF;
      ULONG srv_calc_c_delta0_lower_limit          : 0x1;
      ULONG srv_calc_c_delta1_upper_limit          : 0xF;
      ULONG srv_calc_c_delta1_lower_limit          : 0x1;
      ULONG srv_calc_c_delta2_upper_limit          : 0xF;
      ULONG srv_calc_c_delta2_lower_limit          : 0x1;
      ULONG srv_calc_c_gthr_read                   : 0x4;
      ULONG srv_calc_c_gthr_write                  : 0x4;
      ULONG srv_calc_c_thresh_p_gain               : 0xF;
      ULONG srv_calc_c_gdelta_write                : 0x4;
   // *** Generic coefs ***
      ULONG srv_speedlvl_slowzero                  : 0x1;
      ULONG srv_speedlvl_brkslwoe                  : 0x1;
      ULONG srv_dropctl_dropfoo                    : 0xA;
      ULONG srv_lctimdelay_hdrstrdly               : 0x8;
      ULONG srv_lctimdelay_wghlddly                : 0x8;
      ULONG srv_lctimdelay_apcfndly                : 0x8;
      ULONG srv_lctimdelay_ttwdelay                : 0x8;
      ULONG srv_focus_fsl                          : 0x7;
      ULONG srv_fvset_contslow                     : 0x1;
      ULONG srv_fvset_landxc                       : 0x1;
      ULONG srv_inhibittx_inhismp                  : 0xA;
      ULONG srv_tesrcctl_tesrcdb                   : 0xA;
      ULONG srv_dlfsklvl                           : 0x20;
      ULONG srv_dlfsblvl                           : 0x20;
} srv_dtT;

/*__packed*/ typedef struct
{
   //  srv_dk
      // *** Specific coefs ***
      ULONG srv_calc_c_read_power                  : 0x6;
      ULONG srv_calc_c_ld_apc_setpoint             : 0x10;
      ULONG srv_calc_c_offset_adc_vepower          : 0x10;
      ULONG srv_calc_c_slope_adc_vepower           : 0x10;
   // *** Generic coefs ***
      ULONG srv_lcthresh_lcthmax                   : 0x10;
      ULONG srv_lcdelta0_delta0max                 : 0x10;
      ULONG srv_lcdelta1_delta1max                 : 0x10;
      ULONG srv_lcdelta2_delta2max                 : 0x10;
} srv_dkT;

/*__packed*/ typedef struct
{
   //  srv_spd
      // *** Specific coefs ***
   // *** Generic coefs ***
      ULONG srv_fvset_fvd2                         : 0x7;
      ULONG srv_fvset_fvd                          : 0x7;
      ULONG srv_inhibittx_inhitx                   : 0xA;
      ULONG srv_dfctbl_dbl                         : 0x7;
} srv_spdT;


// Extern declaration for the complete coef-table:


typedef struct
{
   afe_initT  afe_init ;
   afe_fixedT  afe_fixed ;
   afe_dtT  afe_dt_cd ;
   afe_dtT  afe_dt_dvd ;
   afe_dtT  afe_dt_dvdram ;
   afe_dtT  afe_dt_bd ;
   afe_dkT  afe_dk_cd [ NUMBER_OF_CD_DISC_KINDS ];
   afe_dkT  afe_dk_dvd [ NUMBER_OF_DVD_DISC_KINDS ];
   afe_dkT  afe_dk_dvdram [ NUMBER_OF_DVDRAM_DISC_KINDS ];
   afe_dkT  afe_dk_bd [ NUMBER_OF_BD_DISC_KINDS ];
   afe_spdT  afe_spd_cd [ NUMBER_OF_CD_DISC_SPEEDS ];
   afe_spdT  afe_spd_dvd [ NUMBER_OF_DVD_DISC_SPEEDS ];
   afe_spdT  afe_spd_dvdram [ NUMBER_OF_DVDRAM_DISC_SPEEDS ];
   afe_spdT  afe_spd_bd [ NUMBER_OF_BD_DISC_SPEEDS ];
   afe_dsT  afe_ds_cd [ NUMBER_OF_CD_DISC_SPEEDS ][ NUMBER_OF_CD_DISC_KINDS ];
   afe_dsT  afe_ds_dvd [ NUMBER_OF_DVD_DISC_SPEEDS ][ NUMBER_OF_DVD_DISC_KINDS ];
   afe_dsT  afe_ds_dvdram [ NUMBER_OF_DVDRAM_DISC_SPEEDS ][ NUMBER_OF_DVDRAM_DISC_KINDS ];
   afe_dsT  afe_ds_bd [ NUMBER_OF_BD_DISC_SPEEDS ][ NUMBER_OF_BD_DISC_KINDS ];
   dfe_initT  dfe_init ;
   dfe_fixedT  dfe_fixed ;
   dfe_dtT  dfe_dt_cd ;
   dfe_dtT  dfe_dt_dvd ;
   dfe_dtT  dfe_dt_dvdram ;
   dfe_dtT  dfe_dt_bd ;
   dfe_dkT  dfe_dk_cd [ NUMBER_OF_CD_DISC_KINDS ];
   dfe_dkT  dfe_dk_dvd [ NUMBER_OF_DVD_DISC_KINDS ];
   dfe_dkT  dfe_dk_dvdram [ NUMBER_OF_DVDRAM_DISC_KINDS ];
   dfe_dkT  dfe_dk_bd [ NUMBER_OF_BD_DISC_KINDS ];
   dfe_dsT  dfe_ds_cd [ NUMBER_OF_CD_DISC_SPEEDS ][ NUMBER_OF_CD_DISC_KINDS ];
   dfe_dsT  dfe_ds_dvd [ NUMBER_OF_DVD_DISC_SPEEDS ][ NUMBER_OF_DVD_DISC_KINDS ];
   dfe_dsT  dfe_ds_dvdram [ NUMBER_OF_DVDRAM_DISC_SPEEDS ][ NUMBER_OF_DVDRAM_DISC_KINDS ];
   dfe_dsT  dfe_ds_bd [ NUMBER_OF_BD_DISC_SPEEDS ][ NUMBER_OF_BD_DISC_KINDS ];
   wbl_initT  wbl_init ;
   wbl_fixedT  wbl_fixed ;
   wbl_dtT  wbl_dt_cd ;
   wbl_dtT  wbl_dt_dvd ;
   wbl_dtT  wbl_dt_dvdram ;
   wbl_dtT  wbl_dt_bd ;
   wbl_dkT  wbl_dk_cd [ NUMBER_OF_CD_DISC_KINDS ];
   wbl_dkT  wbl_dk_dvd [ NUMBER_OF_DVD_DISC_KINDS ];
   wbl_dkT  wbl_dk_dvdram [ NUMBER_OF_DVDRAM_DISC_KINDS ];
   wbl_dkT  wbl_dk_bd [ NUMBER_OF_BD_DISC_KINDS ];
   wbl_spdT  wbl_spd_cd [ NUMBER_OF_CD_DISC_SPEEDS ];
   wbl_spdT  wbl_spd_dvd [ NUMBER_OF_DVD_DISC_SPEEDS ];
   wbl_spdT  wbl_spd_dvdram [ NUMBER_OF_DVDRAM_DISC_SPEEDS ];
   wbl_spdT  wbl_spd_bd [ NUMBER_OF_BD_DISC_SPEEDS ];
   srv_initT  srv_init ;
   srv_fixedT  srv_fixed ;
   srv_dtT  srv_dt_cd ;
   srv_dtT  srv_dt_dvd ;
   srv_dtT  srv_dt_dvdram ;
   srv_dtT  srv_dt_bd ;
   srv_dkT  srv_dk_cd [ NUMBER_OF_CD_DISC_KINDS ];
   srv_dkT  srv_dk_dvd [ NUMBER_OF_DVD_DISC_KINDS ];
   srv_dkT  srv_dk_dvdram [ NUMBER_OF_DVDRAM_DISC_KINDS ];
   srv_dkT  srv_dk_bd [ NUMBER_OF_BD_DISC_KINDS ];
   srv_spdT  srv_spd_cd [ NUMBER_OF_CD_DISC_SPEEDS ];
   srv_spdT  srv_spd_dvd [ NUMBER_OF_DVD_DISC_SPEEDS ];
   srv_spdT  srv_spd_dvdram [ NUMBER_OF_DVDRAM_DISC_SPEEDS ];
   srv_spdT  srv_spd_bd [ NUMBER_OF_BD_DISC_SPEEDS ];
} COEFS_TABLE_t;

extern COEFS_TABLE_t coefs_table;

#endif  // _COEF_VALUES_H_

/******************************************************************************
*  End of file.
******************************************************************************/
