/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 129 $
* $Date: 10/12/20 6:19p $
*
* DESCRIPTION
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\oem\oem_tbl.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_afe.h"
#include ".\servo\afe_tbl.h"

//---------------------------------------------------------------------------------------;
// load AFE register and field setting based on the working projects                     ;
//---------------------------------------------------------------------------------------;

afe_offset_info_type1_t const AfeOffsetInfoT1[MAX_TYPE1_OFS_NO] =
{
#if (CHIP_REV >= 0xC0)
    {8, HAL_AFE_WRFCSO_ADR, HAL_AFE_WRLNDFCSO_ADR, {HAL_AFE_BLANKFCSO_ADR,   HAL_AFE_FCSO_ADR}, {HAL_AFE_BLNDFCSO_ADR, HAL_AFE_LNDFCSO_ADR}},
    {8, HAL_AFE_WRTRKO_ADR, HAL_AFE_WRLNDTRKO_ADR, {HAL_AFE_BLANKTRKO_ADR,   HAL_AFE_TRKO_ADR}, {HAL_AFE_BLNDTRKO_ADR, HAL_AFE_LNDTRKO_ADR}},
#else
    {8, HAL_AFE_WRFCSO_ADR, {HAL_AFE_BLANKFCSO_ADR,   HAL_AFE_FCSO_ADR}, {HAL_AFE_BLNDFCSO_ADR, HAL_AFE_LNDFCSO_ADR}},
    {8, HAL_AFE_WRTRKO_ADR, {HAL_AFE_BLANKTRKO_ADR,   HAL_AFE_TRKO_ADR}, {HAL_AFE_BLNDTRKO_ADR, HAL_AFE_LNDTRKO_ADR}},
#endif
};

afe_offset_info_type2_t const AfeOffsetInfoT2[MAX_TYPE2_OFS_NO] =
{
    //total_Bits,field_write_ofs,field_normal_ofs
    {8, HAL_AFE_MPPOSB_ADR,    HAL_AFE_MPPOSA_ADR    },  /* MPP_OS    */
    {8, HAL_AFE_SPP1OSB_ADR,   HAL_AFE_SPP1OSA_ADR   },  /* SPP1_OS   */
    {8, HAL_AFE_FEOOSB_ADR,    HAL_AFE_FEOOSA_ADR    },  /* FEO_OS    */
    {8, HAL_AFE_SPP2OSB_ADR,   HAL_AFE_SPP2OSA_ADR   },  /* SPP2_OS   */
    {8, HAL_AFE_SBADOSB_ADR,   HAL_AFE_SBADOSA_ADR   },  /* SBAD_OS   */
    {8, HAL_AFE_SRFOSB_ADR,    HAL_AFE_SRFOSA_ADR    },  /* SRF_OS    */
    {8, HAL_AFE_MBVREFOSB_ADR, HAL_AFE_MBVREFOSA_ADR },  /* MBVREF_OS */
    {8, HAL_AFE_SBVREFOSB_ADR, HAL_AFE_SBVREFOSA_ADR },  /* SBVREF_OS */
    {8, HAL_AFE_FE2OSB_ADR,    HAL_AFE_FE2OSA_ADR    },  /* FE2_OS    */
    {8, HAL_AFE_TE2OSB_ADR,    HAL_AFE_TE2OSA_ADR    },  /* TE2_OS    */
    {8, HAL_AFE_BS2OSB_ADR,    HAL_AFE_BS2OSA_ADR    },  /* BS2_OS    */
    {7, HAL_AFE_RFCOSPWLD_ADR, HAL_AFE_RFCOSP_ADR    },  /* RFCP_OS   */
    {7, HAL_AFE_RFCOSNWLD_ADR, HAL_AFE_RFCOSN_ADR    },  /* RFCN_OS   */
#if (CHIP_REV < 0xC0)
    {8, HAL_AFE_WADOS_ADR,     HAL_AFE_RADOS_ADR     },  /* RADO_OS   */
    {8, HAL_AFE_WBCOS_ADR,     HAL_AFE_RBCOS_ADR     },  /* RBCO_OS   */
#else
    {8, HAL_AFE_RADOSA_ADR,    HAL_AFE_RADOSB_ADR    },  /* RADO_OS   */
    {8, HAL_AFE_RBCOSA_ADR,    HAL_AFE_RBCOSB_ADR    },  /* RBCO_OS   */
#endif
    {8, HAL_AFE_BLANKBSKO_ADR, HAL_AFE_BSKO_ADR      },  /* BS_OS     */
};

afe_offset_info_type3_t const AfeOffsetInfoT3[MAX_TYPE3_OFS_NO] =
{
    {8, HAL_AFE_CEOS_ADR      },  /* CE_OS     */
    {8, HAL_AFE_PHSOS_ADR     },  /* PHS_OS    */
    {8, HAL_AFE_BHSOS_ADR     },  /* BHS_OS    */
    {8, HAL_AFE_RPOS_ADR      },  /* RP_OS     */
    {8, HAL_AFE_RP2OS_ADR     },  /* RP2_OS    */
    {6, HAL_AFE_RFPOS_ADR     },  /* RFP_OS    */
    {8, HAL_AFE_VREFOS_ADR    },  /* VREF_OS   */
    {8, HAL_AFE_RFAOCREG_ADR  },  /* RFPGA_OS  */
    {6, HAL_AFE_DPDCH1OS_ADR  },  /* DPDCH1_OS */
    {6, HAL_AFE_DPDCH2OS_ADR  },  /* DPDCH2_OS */
    {6, HAL_AFE_DPDCH3OS_ADR  },  /* DPDCH3_OS */
    {6, HAL_AFE_DPDCH4OS_ADR  },  /* DPDCH4_OS */
#if (CHIP_REV >= 0xC0)
    {5, HAL_AFE_TXOS_ADR      }   /* TX_OS     */
#endif
};

afe_offset_info_type4_t const AfeOffsetInfoT4[MAX_TYPE4_OFS_NO] =
{
#if (CHIP_REV >= 0xC0)
    {2, HAL_AFE_DPDVCGM_ADR,   HAL_AFE_DPDVCGM_MASK,   HAL_AFE_DPDVCGM_LSB   },  /* DPDGM_OS  */
#else
    {4, HAL_AFE_DPDVCGM_ADR,   HAL_AFE_DPDVCGM_MASK,   HAL_AFE_DPDVCGM_LSB   },  /* DPDGM_OS  */
#endif
    {4, HAL_AFE_APCVREFOS_ADR, HAL_AFE_APCVREFOS_MASK, HAL_AFE_APCVREFOS_LSB },  /* APC_OS    */
#if (CHIP_REV >= 0xC0)
    {6, HAL_AFE_RFVREFPOS_ADR, HAL_AFE_RFVREFPOS_MASK, HAL_AFE_RFVREFPOS_LSB },  /* RFREFP_OS */
    {6, HAL_AFE_RFVREFNOS_ADR, HAL_AFE_RFVREFNOS_MASK, HAL_AFE_RFVREFNOS_LSB },  /* RFREFN_OS */
#endif
};

#if (AFE_NORMALIZER == 1)
afe_bs_info_nor_t const AfeBsInfoNor[MAX_NOR_BS_NO] =
{
    {0xFE, HAL_AFE_TARGETBS_ADR,  HAL_AFE_TARGETBS_MASK,  HAL_AFE_TARGETBS_LSB}, /* TAR_BS */
    {0x0F, HAL_AFE_BSTHRES_ADR,   HAL_AFE_BSTHRES_MASK,   HAL_AFE_BSTHRES_LSB }, /* THR_BS */
};
#endif

afe_gain_info_type1_t const AfeGainInfoT1[MAX_TYPE1_GN_NO] =
{
    {0x1F, HAL_AFE_WRFCSG_ADR, HAL_AFE_WRFCSG_MASK, HAL_AFE_WRFCSG_LSB, {HAL_AFE_BLANKFCSG_ADR, HAL_AFE_BLANKFCSG_MASK, HAL_AFE_BLANKFCSG_LSB, HAL_AFE_FCSG_ADR, HAL_AFE_FCSG_MASK, HAL_AFE_FCSG_LSB}, {HAL_AFE_BLNDFCSG_ADR, HAL_AFE_BLNDFCSG_MASK, HAL_AFE_BLNDFCSG_LSB, HAL_AFE_LNDFCSG_ADR, HAL_AFE_LNDFCSG_MASK, HAL_AFE_LNDFCSG_LSB}},  /* FE_GN  */
    {0x1F, HAL_AFE_WRTRKG_ADR, HAL_AFE_WRTRKG_MASK, HAL_AFE_WRTRKG_LSB, {HAL_AFE_BLANKTRKG_ADR, HAL_AFE_BLANKTRKG_MASK, HAL_AFE_BLANKTRKG_LSB, HAL_AFE_TRKG_ADR, HAL_AFE_TRKG_MASK, HAL_AFE_TRKG_LSB}, {HAL_AFE_BLNDTRKG_ADR, HAL_AFE_BLNDTRKG_MASK, HAL_AFE_BLNDTRKG_LSB, HAL_AFE_LNDTRKG_ADR, HAL_AFE_LNDTRKG_MASK, HAL_AFE_LNDTRKG_LSB}},  /* TE_GN  */
};

afe_gain_info_type2_t const AfeGainInfoT2[MAX_TYPE2_GN_NO] =
{
    {0x1F, HAL_AFE_BLANKBSKG_ADR, HAL_AFE_BLANKBSKG_MASK, HAL_AFE_BLANKBSKG_LSB, HAL_AFE_BSKG_ADR,     HAL_AFE_BSKG_MASK,     HAL_AFE_BSKG_LSB},     /* BS_GN */
#if (CHIP_REV < 0xC0)
    {0x03, HAL_AFE_RFGNWLD_ADR,   HAL_AFE_RFGNWLD_MASK,   HAL_AFE_RFGNWLD_LSB,   HAL_AFE_RFGN_ADR,     HAL_AFE_RFGN_MASK,     HAL_AFE_RFGN_LSB},     /* RF_GN */
#else
    {0x07, HAL_AFE_RFGNWLD_ADR,   HAL_AFE_RFGNWLD_MASK,   HAL_AFE_RFGNWLD_LSB,   HAL_AFE_RFGN_ADR,     HAL_AFE_RFGN_MASK,     HAL_AFE_RFGN_LSB},     /* RF_GN */
#endif
    {0x1F, HAL_AFE_MBATTSEL_ADR,  HAL_AFE_MBATTSEL_MASK,  HAL_AFE_MBATTSEL_LSB,  HAL_AFE_MBGUPSEL_ADR, HAL_AFE_MBGUPSEL_MASK, HAL_AFE_MBGUPSEL_LSB}, /* MB_GN */
    {0x1F, HAL_AFE_SBATTSEL_ADR,  HAL_AFE_SBATTSEL_MASK,  HAL_AFE_SBATTSEL_LSB,  HAL_AFE_SBGUPSEL_ADR, HAL_AFE_SBGUPSEL_MASK, HAL_AFE_SBGUPSEL_LSB}, /* SB_GN */
    {0x03, HAL_AFE_RADGNA_ADR,    HAL_AFE_RADGNA_MASK,    HAL_AFE_RADGNA_LSB,    HAL_AFE_RADGNB_ADR,   HAL_AFE_RADGNB_MASK,   HAL_AFE_RADGNB_LSB},   /* WAD_GN */
    {0x03, HAL_AFE_RBCGNA_ADR,    HAL_AFE_RBCGNA_MASK,    HAL_AFE_RBCGNA_LSB,    HAL_AFE_RBCGNB_ADR,   HAL_AFE_RBCGNB_MASK,   HAL_AFE_RBCGNB_LSB},   /* WBC_GN */
    {0x03, HAL_AFE_RADPGNA_ADR,   HAL_AFE_RADPGNA_MASK,   HAL_AFE_RADPGNA_LSB,   HAL_AFE_RADPGNB_ADR,  HAL_AFE_RADPGNB_MASK,  HAL_AFE_RADPGNB_LSB},  /* WADP_GN */
    {0x03, HAL_AFE_RBCPGNA_ADR,   HAL_AFE_RBCPGNA_MASK,   HAL_AFE_RBCPGNA_LSB,   HAL_AFE_RBCPGNB_ADR,  HAL_AFE_RBCPGNB_MASK,  HAL_AFE_RBCPGNB_LSB},  /* WBCP_GN */
};

afe_gain_info_type3_t const AfeGainInfoT3[MAX_TYPE3_GN_NO] =
{
    {0x1F, HAL_AFE_CEGN_ADR,      HAL_AFE_CEGN_MASK,      HAL_AFE_CEGN_LSB      },  /* CE_GN   */
    {0x1F, HAL_AFE_RP2GN_ADR,     HAL_AFE_RP2GN_MASK,     HAL_AFE_RP2GN_LSB     },  /* RP_GN   */
    {0x0F, HAL_AFE_DPDGN_ADR,     HAL_AFE_DPDGN_MASK,     HAL_AFE_DPDGN_LSB     },  /* DPD_GN  */
    {0x04, HAL_AFE_WDIFFGAIN_ADR, HAL_AFE_WDIFFGAIN_MASK, HAL_AFE_WDIFFGAIN_LSB },  /* WOB_GN  */
    {0x1F, HAL_AFE_RFAGCREG_ADR,  HAL_AFE_RFAGCREG_MASK,  HAL_AFE_RFAGCREG_LSB  },  /* RFPGA_GN*/
    {0x03, HAL_AFE_RFINGN_ADR,    HAL_AFE_RFINGN_MASK,    HAL_AFE_RFINGN_LSB    },  /* RFIN_GN */
    {0x1F, HAL_AFE_TEBG_ADR,      HAL_AFE_TEBG_MASK,      HAL_AFE_TEBG_LSB      },  /* TEB_GN  */
    {0x1F, HAL_AFE_RPBG_ADR,      HAL_AFE_RPBG_MASK,      HAL_AFE_RPBG_LSB      },  /* RPB_GN  */
    {0x1F, HAL_AFE_CEBG_ADR,      HAL_AFE_CEBG_MASK,      HAL_AFE_CEBG_LSB      },  /* CEB_GN  */
    {0x1F, HAL_AFE_FEOBG_ADR,     HAL_AFE_FEOBG_MASK,     HAL_AFE_FEOBG_LSB     },  /* FEOB_GN */
    {0x1F, HAL_AFE_SPP2BG_ADR,    HAL_AFE_SPP2BG_MASK,    HAL_AFE_SPP2BG_LSB    },  /* SPP2B_GN*/
#if (MPPBG_SPPBG_CALIB != 0)
    {0x1F, HAL_AFE_MPPOBG_ADR,    HAL_AFE_MPPOBG_MASK,    HAL_AFE_MPPOBG_LSB    },  /* MPPB_GN */
    {0x1F, HAL_AFE_SPP1BG_ADR,    HAL_AFE_SPP1BG_MASK,    HAL_AFE_SPP1BG_LSB    },  /* SPP1B_GN*/
#endif
};

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
