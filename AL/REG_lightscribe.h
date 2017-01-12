/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_lightscribe.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the LS core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_LIGHTSCRIBE_H__
#define __REG_LIGHTSCRIBE_H__

// Core register map
#define HAL_LS_VERSION 7.13
#define HAL_LS_BaseAddress 0x10001300
#define HAL_LSREG_MAP ((struct halLSReg_Map*) (void *) HAL_LS_BaseAddress)
__packed struct halLSReg_Map
{
	HAL_Reg8  LSCTRL0;                        //00000000
	HAL_Reg8  LSCTRL1;                        //00000001
	HAL_Reg8  LSCTRL2;                        //00000002
	HAL_Reg8  LSWRCNT;                        //00000003
	HAL_Reg16 LSBITRTL;                       //00000004
	HAL_Reg8  LSINT;                          //00000006
	HAL_Reg8  LSINTEN;                        //00000007
	HAL_Reg8  LSSTAT;                         //00000008
	HAL_Reg8  ReservedBlk0[1];                //00000009
	HAL_Reg16 CURBITL;                        //0000000A
	HAL_Reg32 LSSTAA0L;                       //0000000C
	HAL_Reg16 LSTBIT0L;                       //00000010
	HAL_Reg16 STABIT0L;                       //00000012
	HAL_Reg32 LSSTAA1L;                       //00000014
	HAL_Reg16 LSTBIT1L;                       //00000018
	HAL_Reg16 STABIT1L;                       //0000001A
	HAL_Reg16 SPKCNTL;                        //0000001C
	HAL_Reg16 SPKSTAL;                        //0000001E
	HAL_Reg16 SAWTCNTL;                       //00000020
	HAL_Reg8  LSSPARE;                        //00000022
	HAL_Reg8  LSWRINC;                        //00000023
	HAL_Reg8  LSDBGSEL;                       //00000024
};
// Register LSCTRL0
#define HAL_LS_LSRST_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_LSRST_LSB         7
#define HAL_LS_LSRST_MSB         7
#define HAL_LS_LSRST_MASK        0x00000080
#define HAL_LS_LSRST_TYPE        Bool
#define HAL_LS_LSRST             HAL_LS_LSRST_MASK
#define HAL_LS_LSRST_READABLE    1
#define HAL_LS_LSRST_WRITABLE    1
#define HAL_LS_LSRST_SIGNED      0
#define HAL_LS_LSRST_ADR         0x10001300

#define HAL_LS_RSTSPKC_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_RSTSPKC_LSB         4
#define HAL_LS_RSTSPKC_MSB         4
#define HAL_LS_RSTSPKC_MASK        0x00000010
#define HAL_LS_RSTSPKC_TYPE        Bool
#define HAL_LS_RSTSPKC             HAL_LS_RSTSPKC_MASK
#define HAL_LS_RSTSPKC_READABLE    1
#define HAL_LS_RSTSPKC_WRITABLE    1
#define HAL_LS_RSTSPKC_SIGNED      0
#define HAL_LS_RSTSPKC_ADR         0x10001300

#define HAL_LS_LSCHKIDX_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_LSCHKIDX_LSB         3
#define HAL_LS_LSCHKIDX_MSB         3
#define HAL_LS_LSCHKIDX_MASK        0x00000008
#define HAL_LS_LSCHKIDX_TYPE        Bool
#define HAL_LS_LSCHKIDX             HAL_LS_LSCHKIDX_MASK
#define HAL_LS_LSCHKIDX_READABLE    1
#define HAL_LS_LSCHKIDX_WRITABLE    1
#define HAL_LS_LSCHKIDX_SIGNED      0
#define HAL_LS_LSCHKIDX_ADR         0x10001300

#define HAL_LS_LSRDMID_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_LSRDMID_LSB         2
#define HAL_LS_LSRDMID_MSB         2
#define HAL_LS_LSRDMID_MASK        0x00000004
#define HAL_LS_LSRDMID_TYPE        Bool
#define HAL_LS_LSRDMID             HAL_LS_LSRDMID_MASK
#define HAL_LS_LSRDMID_READABLE    1
#define HAL_LS_LSRDMID_WRITABLE    1
#define HAL_LS_LSRDMID_SIGNED      0
#define HAL_LS_LSRDMID_ADR         0x10001300

#define HAL_LS_LSDETIDX_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_LSDETIDX_LSB         1
#define HAL_LS_LSDETIDX_MSB         1
#define HAL_LS_LSDETIDX_MASK        0x00000002
#define HAL_LS_LSDETIDX_TYPE        Bool
#define HAL_LS_LSDETIDX             HAL_LS_LSDETIDX_MASK
#define HAL_LS_LSDETIDX_READABLE    1
#define HAL_LS_LSDETIDX_WRITABLE    1
#define HAL_LS_LSDETIDX_SIGNED      0
#define HAL_LS_LSDETIDX_ADR         0x10001300

#define HAL_LS_LSWREN_REGISTER    (HAL_LSREG_MAP->LSCTRL0)
#define HAL_LS_LSWREN_LSB         0
#define HAL_LS_LSWREN_MSB         0
#define HAL_LS_LSWREN_MASK        0x00000001
#define HAL_LS_LSWREN_TYPE        Bool
#define HAL_LS_LSWREN             HAL_LS_LSWREN_MASK
#define HAL_LS_LSWREN_READABLE    1
#define HAL_LS_LSWREN_WRITABLE    1
#define HAL_LS_LSWREN_SIGNED      0
#define HAL_LS_LSWREN_ADR         0x10001300

// Register LSCTRL1
#define HAL_LS_IDXDTHLD_REGISTER    (HAL_LSREG_MAP->LSCTRL1)
#define HAL_LS_IDXDTHLD_LSB         4
#define HAL_LS_IDXDTHLD_MSB         6
#define HAL_LS_IDXDTHLD_MASK        0x00000070
#define HAL_LS_IDXDTHLD_TYPE        halLS_IdxMkDetThld_t
#define HAL_LS_IDXDTHLD             HAL_LS_IDXDTHLD_MASK
#define HAL_LS_IDXDTHLD_READABLE    1
#define HAL_LS_IDXDTHLD_WRITABLE    1
#define HAL_LS_IDXDTHLD_SIGNED      0
#define HAL_LS_IDXDTHLD_ADR         0x10001301

#define HAL_LS_SECIDXMK_REGISTER    (HAL_LSREG_MAP->LSCTRL1)
#define HAL_LS_SECIDXMK_LSB         3
#define HAL_LS_SECIDXMK_MSB         3
#define HAL_LS_SECIDXMK_MASK        0x00000008
#define HAL_LS_SECIDXMK_TYPE        halLS_SrchIdxMk_t
#define HAL_LS_SECIDXMK             HAL_LS_SECIDXMK_MASK
#define HAL_LS_SECIDXMK_READABLE    1
#define HAL_LS_SECIDXMK_WRITABLE    1
#define HAL_LS_SECIDXMK_SIGNED      0
#define HAL_LS_SECIDXMK_ADR         0x10001301

#define HAL_LS_RSTBSPK0_REGISTER    (HAL_LSREG_MAP->LSCTRL1)
#define HAL_LS_RSTBSPK0_LSB         2
#define HAL_LS_RSTBSPK0_MSB         2
#define HAL_LS_RSTBSPK0_MASK        0x00000004
#define HAL_LS_RSTBSPK0_TYPE        Bool
#define HAL_LS_RSTBSPK0             HAL_LS_RSTBSPK0_MASK
#define HAL_LS_RSTBSPK0_READABLE    1
#define HAL_LS_RSTBSPK0_WRITABLE    1
#define HAL_LS_RSTBSPK0_SIGNED      0
#define HAL_LS_RSTBSPK0_ADR         0x10001301

#define HAL_LS_RSTBSSPK_REGISTER    (HAL_LSREG_MAP->LSCTRL1)
#define HAL_LS_RSTBSSPK_LSB         1
#define HAL_LS_RSTBSSPK_MSB         1
#define HAL_LS_RSTBSSPK_MASK        0x00000002
#define HAL_LS_RSTBSSPK_TYPE        Bool
#define HAL_LS_RSTBSSPK             HAL_LS_RSTBSSPK_MASK
#define HAL_LS_RSTBSSPK_READABLE    1
#define HAL_LS_RSTBSSPK_WRITABLE    1
#define HAL_LS_RSTBSSPK_SIGNED      0
#define HAL_LS_RSTBSSPK_ADR         0x10001301

#define HAL_LS_WRSTPSEL_REGISTER    (HAL_LSREG_MAP->LSCTRL1)
#define HAL_LS_WRSTPSEL_LSB         0
#define HAL_LS_WRSTPSEL_MSB         0
#define HAL_LS_WRSTPSEL_MASK        0x00000001
#define HAL_LS_WRSTPSEL_TYPE        halLS_WrStopSel_t
#define HAL_LS_WRSTPSEL             HAL_LS_WRSTPSEL_MASK
#define HAL_LS_WRSTPSEL_READABLE    1
#define HAL_LS_WRSTPSEL_WRITABLE    1
#define HAL_LS_WRSTPSEL_SIGNED      0
#define HAL_LS_WRSTPSEL_ADR         0x10001301

// Register LSCTRL2
#define HAL_LS_LSMIOSEL_REGISTER    (HAL_LSREG_MAP->LSCTRL2)
#define HAL_LS_LSMIOSEL_LSB         4
#define HAL_LS_LSMIOSEL_MSB         7
#define HAL_LS_LSMIOSEL_MASK        0x000000F0
#define HAL_LS_LSMIOSEL_TYPE        halLS_LSMIOSEL_t
#define HAL_LS_LSMIOSEL             HAL_LS_LSMIOSEL_MASK
#define HAL_LS_LSMIOSEL_READABLE    1
#define HAL_LS_LSMIOSEL_WRITABLE    1
#define HAL_LS_LSMIOSEL_SIGNED      0
#define HAL_LS_LSMIOSEL_ADR         0x10001302

#define HAL_LS_LSEDGE_REGISTER    (HAL_LSREG_MAP->LSCTRL2)
#define HAL_LS_LSEDGE_LSB         2
#define HAL_LS_LSEDGE_MSB         3
#define HAL_LS_LSEDGE_MASK        0x0000000C
#define HAL_LS_LSEDGE_TYPE        halLS_LSStrobeEdge_t
#define HAL_LS_LSEDGE             HAL_LS_LSEDGE_MASK
#define HAL_LS_LSEDGE_READABLE    1
#define HAL_LS_LSEDGE_WRITABLE    1
#define HAL_LS_LSEDGE_SIGNED      0
#define HAL_LS_LSEDGE_ADR         0x10001302

#define HAL_LS_LSDATSEL_REGISTER    (HAL_LSREG_MAP->LSCTRL2)
#define HAL_LS_LSDATSEL_LSB         0
#define HAL_LS_LSDATSEL_MSB         1
#define HAL_LS_LSDATSEL_MASK        0x00000003
#define HAL_LS_LSDATSEL_TYPE        halLS_DataSignal_t
#define HAL_LS_LSDATSEL             HAL_LS_LSDATSEL_MASK
#define HAL_LS_LSDATSEL_READABLE    1
#define HAL_LS_LSDATSEL_WRITABLE    1
#define HAL_LS_LSDATSEL_SIGNED      0
#define HAL_LS_LSDATSEL_ADR         0x10001302

// Register LSWRCNT
#define HAL_LS_LSWRCNT_REGISTER    (HAL_LSREG_MAP->LSWRCNT)
#define HAL_LS_LSWRCNT_LSB         0
#define HAL_LS_LSWRCNT_MSB         7
#define HAL_LS_LSWRCNT_MASK        0x000000FF
#define HAL_LS_LSWRCNT_TYPE        UByte
#define HAL_LS_LSWRCNT             HAL_LS_LSWRCNT_MASK
#define HAL_LS_LSWRCNT_READABLE    1
#define HAL_LS_LSWRCNT_WRITABLE    1
#define HAL_LS_LSWRCNT_SIGNED      0
#define HAL_LS_LSWRCNT_ADR         0x10001303

// Registers LSBITRTL/LSBITRTH
#define HAL_LS_LSBITRAT_REGISTER    (HAL_LSREG_MAP->LSBITRTL)
#define HAL_LS_LSBITRAT_LSB         0
#define HAL_LS_LSBITRAT_MSB         15
#define HAL_LS_LSBITRAT_MASK        0x0000FFFF
#define HAL_LS_LSBITRAT_TYPE        UInt16
#define HAL_LS_LSBITRAT             HAL_LS_LSBITRAT_MASK
#define HAL_LS_LSBITRAT_READABLE    1
#define HAL_LS_LSBITRAT_WRITABLE    1
#define HAL_LS_LSBITRAT_SIGNED      0
#define HAL_LS_LSBITRAT_ADR         0x10001304

// Register LSINT
#define HAL_LS_LSWRSTRT_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_LSWRSTRT_LSB         7
#define HAL_LS_LSWRSTRT_MSB         7
#define HAL_LS_LSWRSTRT_MASK        0x00000080
#define HAL_LS_LSWRSTRT_TYPE        Bool
#define HAL_LS_LSWRSTRT             HAL_LS_LSWRSTRT_MASK
#define HAL_LS_LSWRSTRT_READABLE    1
#define HAL_LS_LSWRSTRT_WRITABLE    1
#define HAL_LS_LSWRSTRT_SIGNED      0
#define HAL_LS_LSWRSTRT_ADR         0x10001306

#define HAL_LS_LSWRERR_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_LSWRERR_LSB         6
#define HAL_LS_LSWRERR_MSB         6
#define HAL_LS_LSWRERR_MASK        0x00000040
#define HAL_LS_LSWRERR_TYPE        Bool
#define HAL_LS_LSWRERR             HAL_LS_LSWRERR_MASK
#define HAL_LS_LSWRERR_READABLE    1
#define HAL_LS_LSWRERR_WRITABLE    1
#define HAL_LS_LSWRERR_SIGNED      0
#define HAL_LS_LSWRERR_ADR         0x10001306

#define HAL_LS_IDXMARK_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_IDXMARK_LSB         5
#define HAL_LS_IDXMARK_MSB         5
#define HAL_LS_IDXMARK_MASK        0x00000020
#define HAL_LS_IDXMARK_TYPE        Bool
#define HAL_LS_IDXMARK             HAL_LS_IDXMARK_MASK
#define HAL_LS_IDXMARK_READABLE    1
#define HAL_LS_IDXMARK_WRITABLE    1
#define HAL_LS_IDXMARK_SIGNED      0
#define HAL_LS_IDXMARK_ADR         0x10001306

#define HAL_LS_SAWTOOTH_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_SAWTOOTH_LSB         4
#define HAL_LS_SAWTOOTH_MSB         4
#define HAL_LS_SAWTOOTH_MASK        0x00000010
#define HAL_LS_SAWTOOTH_TYPE        Bool
#define HAL_LS_SAWTOOTH             HAL_LS_SAWTOOTH_MASK
#define HAL_LS_SAWTOOTH_READABLE    1
#define HAL_LS_SAWTOOTH_WRITABLE    1
#define HAL_LS_SAWTOOTH_SIGNED      0
#define HAL_LS_SAWTOOTH_ADR         0x10001306

#define HAL_LS_TRKCMP_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_TRKCMP_LSB         3
#define HAL_LS_TRKCMP_MSB         3
#define HAL_LS_TRKCMP_MASK        0x00000008
#define HAL_LS_TRKCMP_TYPE        Bool
#define HAL_LS_TRKCMP             HAL_LS_TRKCMP_MASK
#define HAL_LS_TRKCMP_READABLE    1
#define HAL_LS_TRKCMP_WRITABLE    1
#define HAL_LS_TRKCMP_SIGNED      0
#define HAL_LS_TRKCMP_ADR         0x10001306

#define HAL_LS_MIDCMP_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_MIDCMP_LSB         2
#define HAL_LS_MIDCMP_MSB         2
#define HAL_LS_MIDCMP_MASK        0x00000004
#define HAL_LS_MIDCMP_TYPE        Bool
#define HAL_LS_MIDCMP             HAL_LS_MIDCMP_MASK
#define HAL_LS_MIDCMP_READABLE    1
#define HAL_LS_MIDCMP_WRITABLE    1
#define HAL_LS_MIDCMP_SIGNED      0
#define HAL_LS_MIDCMP_ADR         0x10001306

#define HAL_LS_SPOKE0_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_SPOKE0_LSB         1
#define HAL_LS_SPOKE0_MSB         1
#define HAL_LS_SPOKE0_MASK        0x00000002
#define HAL_LS_SPOKE0_TYPE        Bool
#define HAL_LS_SPOKE0             HAL_LS_SPOKE0_MASK
#define HAL_LS_SPOKE0_READABLE    1
#define HAL_LS_SPOKE0_WRITABLE    1
#define HAL_LS_SPOKE0_SIGNED      0
#define HAL_LS_SPOKE0_ADR         0x10001306

#define HAL_LS_SPOKE_REGISTER    (HAL_LSREG_MAP->LSINT)
#define HAL_LS_SPOKE_LSB         0
#define HAL_LS_SPOKE_MSB         0
#define HAL_LS_SPOKE_MASK        0x00000001
#define HAL_LS_SPOKE_TYPE        Bool
#define HAL_LS_SPOKE             HAL_LS_SPOKE_MASK
#define HAL_LS_SPOKE_READABLE    1
#define HAL_LS_SPOKE_WRITABLE    1
#define HAL_LS_SPOKE_SIGNED      0
#define HAL_LS_SPOKE_ADR         0x10001306

// Register LSINTEN
#define HAL_LS_LSWRSTEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_LSWRSTEN_LSB         7
#define HAL_LS_LSWRSTEN_MSB         7
#define HAL_LS_LSWRSTEN_MASK        0x00000080
#define HAL_LS_LSWRSTEN_TYPE        Bool
#define HAL_LS_LSWRSTEN             HAL_LS_LSWRSTEN_MASK
#define HAL_LS_LSWRSTEN_READABLE    1
#define HAL_LS_LSWRSTEN_WRITABLE    1
#define HAL_LS_LSWRSTEN_SIGNED      0
#define HAL_LS_LSWRSTEN_ADR         0x10001307

#define HAL_LS_LSWREREN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_LSWREREN_LSB         6
#define HAL_LS_LSWREREN_MSB         6
#define HAL_LS_LSWREREN_MASK        0x00000040
#define HAL_LS_LSWREREN_TYPE        Bool
#define HAL_LS_LSWREREN             HAL_LS_LSWREREN_MASK
#define HAL_LS_LSWREREN_READABLE    1
#define HAL_LS_LSWREREN_WRITABLE    1
#define HAL_LS_LSWREREN_SIGNED      0
#define HAL_LS_LSWREREN_ADR         0x10001307

#define HAL_LS_IDXMRKEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_IDXMRKEN_LSB         5
#define HAL_LS_IDXMRKEN_MSB         5
#define HAL_LS_IDXMRKEN_MASK        0x00000020
#define HAL_LS_IDXMRKEN_TYPE        Bool
#define HAL_LS_IDXMRKEN             HAL_LS_IDXMRKEN_MASK
#define HAL_LS_IDXMRKEN_READABLE    1
#define HAL_LS_IDXMRKEN_WRITABLE    1
#define HAL_LS_IDXMRKEN_SIGNED      0
#define HAL_LS_IDXMRKEN_ADR         0x10001307

#define HAL_LS_STOOTHEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_STOOTHEN_LSB         4
#define HAL_LS_STOOTHEN_MSB         4
#define HAL_LS_STOOTHEN_MASK        0x00000010
#define HAL_LS_STOOTHEN_TYPE        Bool
#define HAL_LS_STOOTHEN             HAL_LS_STOOTHEN_MASK
#define HAL_LS_STOOTHEN_READABLE    1
#define HAL_LS_STOOTHEN_WRITABLE    1
#define HAL_LS_STOOTHEN_SIGNED      0
#define HAL_LS_STOOTHEN_ADR         0x10001307

#define HAL_LS_TRKCMPEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_TRKCMPEN_LSB         3
#define HAL_LS_TRKCMPEN_MSB         3
#define HAL_LS_TRKCMPEN_MASK        0x00000008
#define HAL_LS_TRKCMPEN_TYPE        Bool
#define HAL_LS_TRKCMPEN             HAL_LS_TRKCMPEN_MASK
#define HAL_LS_TRKCMPEN_READABLE    1
#define HAL_LS_TRKCMPEN_WRITABLE    1
#define HAL_LS_TRKCMPEN_SIGNED      0
#define HAL_LS_TRKCMPEN_ADR         0x10001307

#define HAL_LS_MIDCMPEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_MIDCMPEN_LSB         2
#define HAL_LS_MIDCMPEN_MSB         2
#define HAL_LS_MIDCMPEN_MASK        0x00000004
#define HAL_LS_MIDCMPEN_TYPE        Bool
#define HAL_LS_MIDCMPEN             HAL_LS_MIDCMPEN_MASK
#define HAL_LS_MIDCMPEN_READABLE    1
#define HAL_LS_MIDCMPEN_WRITABLE    1
#define HAL_LS_MIDCMPEN_SIGNED      0
#define HAL_LS_MIDCMPEN_ADR         0x10001307

#define HAL_LS_SPOKE0EN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_SPOKE0EN_LSB         1
#define HAL_LS_SPOKE0EN_MSB         1
#define HAL_LS_SPOKE0EN_MASK        0x00000002
#define HAL_LS_SPOKE0EN_TYPE        Bool
#define HAL_LS_SPOKE0EN             HAL_LS_SPOKE0EN_MASK
#define HAL_LS_SPOKE0EN_READABLE    1
#define HAL_LS_SPOKE0EN_WRITABLE    1
#define HAL_LS_SPOKE0EN_SIGNED      0
#define HAL_LS_SPOKE0EN_ADR         0x10001307

#define HAL_LS_SPOKEEN_REGISTER    (HAL_LSREG_MAP->LSINTEN)
#define HAL_LS_SPOKEEN_LSB         0
#define HAL_LS_SPOKEEN_MSB         0
#define HAL_LS_SPOKEEN_MASK        0x00000001
#define HAL_LS_SPOKEEN_TYPE        Bool
#define HAL_LS_SPOKEEN             HAL_LS_SPOKEEN_MASK
#define HAL_LS_SPOKEEN_READABLE    1
#define HAL_LS_SPOKEEN_WRITABLE    1
#define HAL_LS_SPOKEEN_SIGNED      0
#define HAL_LS_SPOKEEN_ADR         0x10001307

// Register LSSTAT
#define HAL_LS_LSWRMISS_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_LSWRMISS_LSB         7
#define HAL_LS_LSWRMISS_MSB         7
#define HAL_LS_LSWRMISS_MASK        0x00000080
#define HAL_LS_LSWRMISS_TYPE        halLS_LSWrMiss_t
#define HAL_LS_LSWRMISS             HAL_LS_LSWRMISS_MASK
#define HAL_LS_LSWRMISS_READABLE    1
#define HAL_LS_LSWRMISS_WRITABLE    0
#define HAL_LS_LSWRMISS_SIGNED      0
#define HAL_LS_LSWRMISS_ADR         0x10001308

#define HAL_LS_LSTBSLIP_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_LSTBSLIP_LSB         6
#define HAL_LS_LSTBSLIP_MSB         6
#define HAL_LS_LSTBSLIP_MASK        0x00000040
#define HAL_LS_LSTBSLIP_TYPE        Bool
#define HAL_LS_LSTBSLIP             HAL_LS_LSTBSLIP_MASK
#define HAL_LS_LSTBSLIP_READABLE    1
#define HAL_LS_LSTBSLIP_WRITABLE    0
#define HAL_LS_LSTBSLIP_SIGNED      0
#define HAL_LS_LSTBSLIP_ADR         0x10001308

#define HAL_LS_STABSLIP_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_STABSLIP_LSB         5
#define HAL_LS_STABSLIP_MSB         5
#define HAL_LS_STABSLIP_MASK        0x00000020
#define HAL_LS_STABSLIP_TYPE        Bool
#define HAL_LS_STABSLIP             HAL_LS_STABSLIP_MASK
#define HAL_LS_STABSLIP_READABLE    1
#define HAL_LS_STABSLIP_WRITABLE    0
#define HAL_LS_STABSLIP_SIGNED      0
#define HAL_LS_STABSLIP_ADR         0x10001308

#define HAL_LS_IDX2MISS_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_IDX2MISS_LSB         4
#define HAL_LS_IDX2MISS_MSB         4
#define HAL_LS_IDX2MISS_MASK        0x00000010
#define HAL_LS_IDX2MISS_TYPE        Bool
#define HAL_LS_IDX2MISS             HAL_LS_IDX2MISS_MASK
#define HAL_LS_IDX2MISS_READABLE    1
#define HAL_LS_IDX2MISS_WRITABLE    0
#define HAL_LS_IDX2MISS_SIGNED      0
#define HAL_LS_IDX2MISS_ADR         0x10001308

#define HAL_LS_IDXMISS_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_IDXMISS_LSB         3
#define HAL_LS_IDXMISS_MSB         3
#define HAL_LS_IDXMISS_MASK        0x00000008
#define HAL_LS_IDXMISS_TYPE        Bool
#define HAL_LS_IDXMISS             HAL_LS_IDXMISS_MASK
#define HAL_LS_IDXMISS_READABLE    1
#define HAL_LS_IDXMISS_WRITABLE    0
#define HAL_LS_IDXMISS_SIGNED      0
#define HAL_LS_IDXMISS_ADR         0x10001308

#define HAL_LS_TRKPTR_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_TRKPTR_LSB         2
#define HAL_LS_TRKPTR_MSB         2
#define HAL_LS_TRKPTR_MASK        0x00000004
#define HAL_LS_TRKPTR_TYPE        halLS_LSTrkPtr_t
#define HAL_LS_TRKPTR             HAL_LS_TRKPTR_MASK
#define HAL_LS_TRKPTR_READABLE    1
#define HAL_LS_TRKPTR_WRITABLE    0
#define HAL_LS_TRKPTR_SIGNED      0
#define HAL_LS_TRKPTR_ADR         0x10001308

#define HAL_LS_LSSPOKE_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_LSSPOKE_LSB         1
#define HAL_LS_LSSPOKE_MSB         1
#define HAL_LS_LSSPOKE_MASK        0x00000002
#define HAL_LS_LSSPOKE_TYPE        Bool
#define HAL_LS_LSSPOKE             HAL_LS_LSSPOKE_MASK
#define HAL_LS_LSSPOKE_READABLE    1
#define HAL_LS_LSSPOKE_WRITABLE    0
#define HAL_LS_LSSPOKE_SIGNED      0
#define HAL_LS_LSSPOKE_ADR         0x10001308

#define HAL_LS_LSDATA_REGISTER    (HAL_LSREG_MAP->LSSTAT)
#define HAL_LS_LSDATA_LSB         0
#define HAL_LS_LSDATA_MSB         0
#define HAL_LS_LSDATA_MASK        0x00000001
#define HAL_LS_LSDATA_TYPE        Bool
#define HAL_LS_LSDATA             HAL_LS_LSDATA_MASK
#define HAL_LS_LSDATA_READABLE    1
#define HAL_LS_LSDATA_WRITABLE    0
#define HAL_LS_LSDATA_SIGNED      0
#define HAL_LS_LSDATA_ADR         0x10001308

// Registers CURBITL/CURBITH
#define HAL_LS_CURBIT_REGISTER    (HAL_LSREG_MAP->CURBITL)
#define HAL_LS_CURBIT_LSB         0
#define HAL_LS_CURBIT_MSB         15
#define HAL_LS_CURBIT_MASK        0x0000FFFF
#define HAL_LS_CURBIT_TYPE        UInt16
#define HAL_LS_CURBIT             HAL_LS_CURBIT_MASK
#define HAL_LS_CURBIT_READABLE    1
#define HAL_LS_CURBIT_WRITABLE    0
#define HAL_LS_CURBIT_SIGNED      0
#define HAL_LS_CURBIT_ADR         0x1000130A

// Registers LSSTAA0L/LSSTAA0M/LSSTAA0H
#define HAL_LS_LSSTAAD0_REGISTER    (HAL_LSREG_MAP->LSSTAA0L)
#define HAL_LS_LSSTAAD0_LSB         0
#define HAL_LS_LSSTAAD0_MSB         23
#define HAL_LS_LSSTAAD0_MASK        0x00FFFFFF
#define HAL_LS_LSSTAAD0_TYPE        UInt32
#define HAL_LS_LSSTAAD0             HAL_LS_LSSTAAD0_MASK
#define HAL_LS_LSSTAAD0_READABLE    1
#define HAL_LS_LSSTAAD0_WRITABLE    1
#define HAL_LS_LSSTAAD0_SIGNED      0
#define HAL_LS_LSSTAAD0_ADR         0x1000130C

// Registers LSTBIT0L/LSTBIT0H
#define HAL_LS_LASTBIT0_REGISTER    (HAL_LSREG_MAP->LSTBIT0L)
#define HAL_LS_LASTBIT0_LSB         0
#define HAL_LS_LASTBIT0_MSB         15
#define HAL_LS_LASTBIT0_MASK        0x0000FFFF
#define HAL_LS_LASTBIT0_TYPE        UInt16
#define HAL_LS_LASTBIT0             HAL_LS_LASTBIT0_MASK
#define HAL_LS_LASTBIT0_READABLE    1
#define HAL_LS_LASTBIT0_WRITABLE    1
#define HAL_LS_LASTBIT0_SIGNED      0
#define HAL_LS_LASTBIT0_ADR         0x10001310

// Registers STABIT0L/STABIT0H
#define HAL_LS_STABIT0_REGISTER    (HAL_LSREG_MAP->STABIT0L)
#define HAL_LS_STABIT0_LSB         0
#define HAL_LS_STABIT0_MSB         15
#define HAL_LS_STABIT0_MASK        0x0000FFFF
#define HAL_LS_STABIT0_TYPE        UInt16
#define HAL_LS_STABIT0             HAL_LS_STABIT0_MASK
#define HAL_LS_STABIT0_READABLE    1
#define HAL_LS_STABIT0_WRITABLE    1
#define HAL_LS_STABIT0_SIGNED      0
#define HAL_LS_STABIT0_ADR         0x10001312

// Registers LSSTAA1L/LSSTAA1M/LSSTAA1H
#define HAL_LS_LSSTAAD1_REGISTER    (HAL_LSREG_MAP->LSSTAA1L)
#define HAL_LS_LSSTAAD1_LSB         0
#define HAL_LS_LSSTAAD1_MSB         23
#define HAL_LS_LSSTAAD1_MASK        0x00FFFFFF
#define HAL_LS_LSSTAAD1_TYPE        UInt32
#define HAL_LS_LSSTAAD1             HAL_LS_LSSTAAD1_MASK
#define HAL_LS_LSSTAAD1_READABLE    1
#define HAL_LS_LSSTAAD1_WRITABLE    1
#define HAL_LS_LSSTAAD1_SIGNED      0
#define HAL_LS_LSSTAAD1_ADR         0x10001314

// Registers LSTBIT1L/LSTBIT1H
#define HAL_LS_LASTBIT1_REGISTER    (HAL_LSREG_MAP->LSTBIT1L)
#define HAL_LS_LASTBIT1_LSB         0
#define HAL_LS_LASTBIT1_MSB         15
#define HAL_LS_LASTBIT1_MASK        0x0000FFFF
#define HAL_LS_LASTBIT1_TYPE        UInt16
#define HAL_LS_LASTBIT1             HAL_LS_LASTBIT1_MASK
#define HAL_LS_LASTBIT1_READABLE    1
#define HAL_LS_LASTBIT1_WRITABLE    1
#define HAL_LS_LASTBIT1_SIGNED      0
#define HAL_LS_LASTBIT1_ADR         0x10001318

// Registers STABIT1L/STABIT1H
#define HAL_LS_STABIT1_REGISTER    (HAL_LSREG_MAP->STABIT1L)
#define HAL_LS_STABIT1_LSB         0
#define HAL_LS_STABIT1_MSB         15
#define HAL_LS_STABIT1_MASK        0x0000FFFF
#define HAL_LS_STABIT1_TYPE        UInt16
#define HAL_LS_STABIT1             HAL_LS_STABIT1_MASK
#define HAL_LS_STABIT1_READABLE    1
#define HAL_LS_STABIT1_WRITABLE    1
#define HAL_LS_STABIT1_SIGNED      0
#define HAL_LS_STABIT1_ADR         0x1000131A

// Registers SPKCNTL/SPKCNTH
#define HAL_LS_SPOKECNT_REGISTER    (HAL_LSREG_MAP->SPKCNTL)
#define HAL_LS_SPOKECNT_LSB         0
#define HAL_LS_SPOKECNT_MSB         9
#define HAL_LS_SPOKECNT_MASK        0x000003FF
#define HAL_LS_SPOKECNT_TYPE        UInt16
#define HAL_LS_SPOKECNT             HAL_LS_SPOKECNT_MASK
#define HAL_LS_SPOKECNT_READABLE    1
#define HAL_LS_SPOKECNT_WRITABLE    0
#define HAL_LS_SPOKECNT_SIGNED      0
#define HAL_LS_SPOKECNT_ADR         0x1000131C

// Registers SPKSTAL/SPKSTAH
#define HAL_LS_SPOKESTA_REGISTER    (HAL_LSREG_MAP->SPKSTAL)
#define HAL_LS_SPOKESTA_LSB         0
#define HAL_LS_SPOKESTA_MSB         9
#define HAL_LS_SPOKESTA_MASK        0x000003FF
#define HAL_LS_SPOKESTA_TYPE        UInt16
#define HAL_LS_SPOKESTA             HAL_LS_SPOKESTA_MASK
#define HAL_LS_SPOKESTA_READABLE    1
#define HAL_LS_SPOKESTA_WRITABLE    1
#define HAL_LS_SPOKESTA_SIGNED      0
#define HAL_LS_SPOKESTA_ADR         0x1000131E

// Registers SAWTCNTL/SAWTCNTH
#define HAL_LS_SAWTCNT_REGISTER    (HAL_LSREG_MAP->SAWTCNTL)
#define HAL_LS_SAWTCNT_LSB         0
#define HAL_LS_SAWTCNT_MSB         15
#define HAL_LS_SAWTCNT_MASK        0x0000FFFF
#define HAL_LS_SAWTCNT_TYPE        UInt16
#define HAL_LS_SAWTCNT             HAL_LS_SAWTCNT_MASK
#define HAL_LS_SAWTCNT_READABLE    1
#define HAL_LS_SAWTCNT_WRITABLE    0
#define HAL_LS_SAWTCNT_SIGNED      0
#define HAL_LS_SAWTCNT_ADR         0x10001320

// Register LSSPARE

#if (CHIP_REV >= 0xC0)

// Register LSWRINC
#define HAL_LS_LSWRINC_REGISTER    (HAL_LSREG_MAP->LSWRINC)
#define HAL_LS_LSWRINC_LSB         0
#define HAL_LS_LSWRINC_MSB         3
#define HAL_LS_LSWRINC_MASK        0x0000000F
#define HAL_LS_LSWRINC_TYPE        UByte
#define HAL_LS_LSWRINC             HAL_LS_LSWRINC_MASK
#define HAL_LS_LSWRINC_READABLE    1
#define HAL_LS_LSWRINC_WRITABLE    1
#define HAL_LS_LSWRINC_SIGNED      0
#define HAL_LS_LSWRINC_ADR         0x10001323

// Register LSDBGSEL
#define HAL_LS_LSDBGSEL_REGISTER    (HAL_LSREG_MAP->LSDBGSEL)
#define HAL_LS_LSDBGSEL_LSB         0
#define HAL_LS_LSDBGSEL_MSB         3
#define HAL_LS_LSDBGSEL_MASK        0x0000000F
#define HAL_LS_LSDBGSEL_TYPE        halLS_LSDBGSEL_t
#define HAL_LS_LSDBGSEL             HAL_LS_LSDBGSEL_MASK
#define HAL_LS_LSDBGSEL_READABLE    1
#define HAL_LS_LSDBGSEL_WRITABLE    1
#define HAL_LS_LSDBGSEL_SIGNED      0
#define HAL_LS_LSDBGSEL_ADR         0x10001324

#endif // #if (CHIP_REV >= 0xC0)

// Enumerations
typedef enum
{
	halLS_IdxMkDetThld_0err = 0,    // Index Mark Detection Threshold: Select if 0 error
	halLS_IdxMkDetThld_1err = 1,    // Index Mark Detection Threshold: Select if 1 or 0 errors
	halLS_IdxMkDetThld_2err = 2,    // Index Mark Detection Threshold: Select if 2 or fewer errors
	halLS_IdxMkDetThld_3err = 3,    // Index Mark Detection Threshold: Select if 3 or fewer errors
	halLS_IdxMkDetThld_4err = 4,    // Index Mark Detection Threshold: Select if 4 or fewer errors
	halLS_IdxMkDetThld_5err = 5,    // Index Mark Detection Threshold: Select if 5 or fewer errors
	halLS_IdxMkDetThld_6err = 6,    // Index Mark Detection Threshold: Select if 6 or fewer errors
	halLS_IdxMkDetThld_7err = 7     // Index Mark Detection Threshold: Select if 7 or fewer errors
} halLS_IdxMkDetThld_t;

typedef enum
{
	halLS_SrchIdxMk_Primary   = 0,    // Search for primary index mark
	halLS_SrchIdxMk_Secondary = 1     // Search for secondary index mark
} halLS_SrchIdxMk_t;

typedef enum
{
	halLS_WrStop_StartSpoke = 0,    // Stop writing at the start spoke (normally used with single-track writing)
	halLS_WrStop_LastTrkBit = 1     // Stop writing at the end of the last track bit (normally used with sprial writing)
} halLS_WrStopSel_t;

typedef enum
{
	halLS_LSMIO0_write_bit_toggle       = 0,
	halLS_LSMIO0_ls_spoke_sysclk        = 1,
	halLS_LSMIO0_ls_wgate               = 2,
	halLS_LSMIO0_trig_spoke_int         = 3,
	halLS_LSMIO0_trig_sawtooth_int      = 4,
	halLS_LSMIO0_idxmiss                = 5,
	halLS_LSMIO0_curbit0                = 6,
	halLS_LSMIO0_spokecnt0              = 7,
	halLS_LSMIO0_sawtcnt0               = 8,
	halLS_LSMIO0_st_state0              = 9,
	halLS_LSMIO0_state_wr0              = 10,
	halLS_LSMIO0_mem_req                = 11,
	halLS_LSMIO0_sram_enable            = 12,
	halLS_LSMIO0_pre_prim_index_found   = 13,
	halLS_LSMIO0_mid_read               = 14,
	halLS_LSMIO1_lsdata                 = 0,
	halLS_LSMIO1_lsspoke                = 1,
	halLS_LSMIO1_ls_wrdata              = 2,
	halLS_LSMIO1_trig_spoke0_int        = 3,
	halLS_LSMIO1_trig_idxmark_int       = 4,
	halLS_LSMIO1_spokerr                = 5,
	halLS_LSMIO1_curbit1                = 6,
	halLS_LSMIO1_spokecnt1              = 7,
	halLS_LSMIO1_sawtcnt1               = 8,
	halLS_LSMIO1_st_state1              = 9,
	halLS_LSMIO1_state_wr1              = 10,
	halLS_LSMIO1_mem_ack                = 11,
	halLS_LSMIO1_sram_wen               = 12,
	halLS_LSMIO1_prim_index_found       = 13,
	halLS_LSMIO1_midx_check             = 14,
	halLS_LSMIO2_lsspoke                = 0,
	halLS_LSMIO2_ls_data_sysclk         = 1,
	halLS_LSMIO2_wrclk_p                = 2,
	halLS_LSMIO2_trig_midcmp_int        = 3,
	halLS_LSMIO2_trig_lswrerr_int       = 4,
	halLS_LSMIO2_stabslip               = 5,
	halLS_LSMIO2_curbit2                = 6,
	halLS_LSMIO2_spokecnt2              = 7,
	halLS_LSMIO2_sawtcnt2               = 8,
	halLS_LSMIO2_st_state2              = 9,
	halLS_LSMIO2_spokecnt799            = 10,
	halLS_LSMIO2_mem_rdata0             = 11,
	halLS_LSMIO2_sram_addr0             = 12,
	halLS_LSMIO2_pre_second_index_found = 13,
	halLS_LSMIO2_sawtcnt_sel            = 14,
	halLS_LSMIO3_spoke_count_zero       = 0,
	halLS_LSMIO3_lsdata                 = 1,
	halLS_LSMIO3_wrclk_p_match_cnt0     = 2,
	halLS_LSMIO3_trig_trkcmp_int        = 3,
	halLS_LSMIO3_trig_lswrstrt_int      = 4,
	halLS_LSMIO3_lstbslip               = 5,
	halLS_LSMIO3_curbit3                = 6,
	halLS_LSMIO3_spokecnt3              = 7,
	halLS_LSMIO3_sawtcnt3               = 8,
	halLS_LSMIO3_trkptr                 = 9,
	halLS_LSMIO3_spoke_p                = 10,
	halLS_LSMIO3_mem_rdata1             = 11,
	halLS_LSMIO3_sram_wdata0            = 12,
	halLS_LSMIO3_second_index_found     = 13,
	halLS_LSMIO3_lswrmiss               = 14 
} halLS_LSMIOSEL_t;

typedef enum
{
	halLS_LSStrobeEdge_1st = 0,
	halLS_LSStrobeEdge_2nd = 1,
	halLS_LSStrobeEdge_3rd = 2,
	halLS_LSStrobeEdge_4th = 3 
} halLS_LSStrobeEdge_t;

typedef enum
{
	halLS_DataSignal_RP     = 0,
	halLS_DataSignal_BS     = 1,
	halLS_DataSignal_DEFECT = 2,
	halLS_DataSignal_RX     = 3 
} halLS_DataSignal_t;

typedef enum
{
	halLS_LSWrMiss_Wr    = 0,    // Under-run occured during writing
	halLS_LSWrMiss_BefWr = 1     // Under-run occured before write start
} halLS_LSWrMiss_t;

typedef enum
{
	halLS_LSTrkPtr_0 = 0,    // Light Scribe Track Pointers 0 are active
	halLS_LSTrkPtr_1 = 1     // Light Scribe Track Pointers 1 are active
} halLS_LSTrkPtr_t;

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halLS_DISABLE                 = 0,
	halLS_LSDBG_DATA_A_SPOKE_CNT  = 1,
	halLS_LSDBG_DATA_B_SPOKE_CNT  = 3,
	halLS_LSDBG_DATA_A_CURBIT_CNT = 5,
	halLS_LSDBG_DATA_B_CURBIT_CNT = 7,
	halLS_LSDBG_DATA_A_SAWT_CNT   = 9,
	halLS_LSDBG_DATA_B_SAWT_CNT   = 11,
	halLS_LSDBG_DATA_A_WRCLK_CNT  = 13,
	halLS_LSDBG_DATA_B_WRCLK_CNT  = 15 
} halLS_LSDBGSEL_t;

#endif // #if (CHIP_REV >= 0xC0)

#endif /* __REG_LIGHTSCRIBE_H__ */

