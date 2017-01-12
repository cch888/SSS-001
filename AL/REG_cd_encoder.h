/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_cd_encoder.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the CDE core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_CD_ENCODER_H__
#define __REG_CD_ENCODER_H__

// Core register map
#define HAL_CDE_VERSION 7.22
#define HAL_CDE_BaseAddress 0x10000100
#define HAL_CDEREG_MAP ((struct halCDEReg_Map*) (void *) HAL_CDE_BaseAddress)
__packed struct halCDEReg_Map
{
	HAL_Reg8  ReservedBlk0[4];                //00000000
	HAL_Reg8  ENCDEC;                         //00000004
	HAL_Reg8  ReservedBlk1[1];                //00000005
	HAL_Reg8  EINT0;                          //00000006
	HAL_Reg8  EINT1;                          //00000007
	HAL_Reg8  ReservedBlk2[3];                //00000008
	HAL_Reg8  EINTEN0;                        //0000000B
	HAL_Reg8  EINTEN1;                        //0000000C
	HAL_Reg8  ReservedBlk3[3];                //0000000D
	HAL_Reg8  CDIFCNT;                        //00000010
	HAL_Reg8  ReservedBlk4[1];                //00000011
	HAL_Reg8  SECCNT0;                        //00000012
	HAL_Reg8  ReservedBlk5[1];                //00000013
	HAL_Reg8  SECCNT2;                        //00000014
	HAL_Reg8  SECMON;                         //00000015
	HAL_Reg8  PERIODPG;                       //00000016
	HAL_Reg8  ReservedBlk6[1];                //00000017
	HAL_Reg8  HMIN;                           //00000018
	HAL_Reg8  HSEC;                           //00000019
	HAL_Reg8  HFRM;                           //0000001A
	HAL_Reg8  HMODE;                          //0000001B
	HAL_Reg8  SHFNUM;                         //0000001C
	HAL_Reg8  SHCNUM;                         //0000001D
	HAL_Reg8  SHSMODE;                        //0000001E
	HAL_Reg8  SHCINFO;                        //0000001F
	HAL_Reg8  ARMSFL;                         //00000020
	HAL_Reg8  ARMSFM;                         //00000021
	HAL_Reg8  ARMSFU;                         //00000022
	HAL_Reg8  CPPTR;                          //00000023
	HAL_Reg8  CPPTRB;                         //00000024
	HAL_Reg8  CPQCTL;                         //00000025
	HAL_Reg8  CPRPTSTA;                       //00000026
	HAL_Reg8  ReservedBlk7[15];               //00000027
	HAL_Reg8  ENSRAMCTL;                      //00000036
	HAL_Reg8  ReservedBlk8[3];                //00000037
	HAL_Reg16 ENBUFSPGL;                      //0000003A
	HAL_Reg16 ENBUFEPGL;                      //0000003C
	HAL_Reg8  ReservedBlk9[2];                //0000003E
	HAL_Reg8  UNDRUNPG;                       //00000040
	HAL_Reg8  ReservedBlk10[3];               //00000041
	HAL_Reg16 CDBUFPGL;                       //00000044
	HAL_Reg8  ReservedBlk11[2];               //00000046
	HAL_Reg16 SPBUFSPGL;                      //00000048
	HAL_Reg16 SPBUFEPGL;                      //0000004A
	HAL_Reg32 ENCRPTL;                        //0000004C
	HAL_Reg8  SUBIFCNT0;                      //00000050
	HAL_Reg8  SQINDX;                         //00000051
	HAL_Reg8  SQCAT;                          //00000052
	HAL_Reg8  SQISRC;                         //00000053
	HAL_Reg8  SQRID;                          //00000054
	HAL_Reg8  SQTBD;                          //00000055
	HAL_Reg8  SQTC;                           //00000056
	HAL_Reg8  SQMODE;                         //00000057
	HAL_Reg8  SQSEL;                          //00000058
	HAL_Reg8  SQTRIG;                         //00000059
	HAL_Reg8  MSFCTR;                         //0000005A
	HAL_Reg8  CPACNT;                         //0000005B
	HAL_Reg8  CPBCNT;                         //0000005C
	HAL_Reg8  CDENCMON;                       //0000005D
	HAL_Reg16 CPPGL;                          //0000005E
	HAL_Reg8  CDENCCTL1;                      //00000060
	HAL_Reg8  CDENCCTL2;                      //00000061
	HAL_Reg8  CDENCCTL3;                      //00000062
	HAL_Reg8  CDENCCTL4;                      //00000063
	HAL_Reg32 WRCOUNTL;                       //00000064
	HAL_Reg8  CURMIN;                         //00000068
	HAL_Reg8  CURSEC;                         //00000069
	HAL_Reg8  CURFRAME;                       //0000006A
	HAL_Reg8  WRMIN;                          //0000006B
	HAL_Reg8  WRSEC;                          //0000006C
	HAL_Reg8  WRFRAME;                        //0000006D
	HAL_Reg8  LNKSTRT;                        //0000006E
	HAL_Reg8  LNKSTOP;                        //0000006F
	HAL_Reg8  ReservedBlk12[7];               //00000070
	HAL_Reg8  LNKCMPBIT;                      //00000077
	HAL_Reg8  LNKCMPWD;                       //00000078
	HAL_Reg8  LNKCMPFRM;                      //00000079
	HAL_Reg8  LNKSTABIT;                      //0000007A
	HAL_Reg8  LNKSTAWD;                       //0000007B
	HAL_Reg8  LNKSTAFRM;                      //0000007C
	HAL_Reg8  LNKSTPBIT;                      //0000007D
	HAL_Reg8  LNKSTPWD;                       //0000007E
	HAL_Reg8  LNKSTPFRM;                      //0000007F
	HAL_Reg8  ReservedBlk13[4];               //00000080
	HAL_Reg8  WSTCTL1;                        //00000084
	HAL_Reg8  WSRPN1;                         //00000085
	HAL_Reg8  WSRPN2;                         //00000086
	HAL_Reg8  WSTCTL2;                        //00000087
	HAL_Reg8  WSTCTL3;                        //00000088
	HAL_Reg8  WSTCTL4;                        //00000089
	HAL_Reg8  WSTCTL5;                        //0000008A
	HAL_Reg8  WSTCTL6;                        //0000008B
	HAL_Reg8  ReservedBlk14[20];              //0000008C
	HAL_Reg8  HFSWCTL;                        //000000A0
	HAL_Reg8  ReservedBlk15[31];              //000000A1
	HAL_Reg8  LFMODE;                         //000000C0
	HAL_Reg8  LFTRGWD;                        //000000C1
	HAL_Reg8  LFTRGBIT;                       //000000C2
	HAL_Reg8  LFLENGTH;                       //000000C3
	HAL_Reg8  LFWDSEC;                        //000000C4
	HAL_Reg8  LFWDFRM;                        //000000C5
	HAL_Reg8  LFWDWD;                         //000000C6
	HAL_Reg8  LFTRKCNT;                       //000000C7
	HAL_Reg8  LFSECCNT;                       //000000C8
	HAL_Reg8  LFFRMCNT;                       //000000C9
	HAL_Reg8  LFWDCNT;                        //000000CA
	HAL_Reg8  LFBITCNT;                       //000000CB
	HAL_Reg8  LFINT;                          //000000CC
	HAL_Reg8  LFINTEN;                        //000000CD
	HAL_Reg8  ReservedBlk16[18];              //000000CE
	HAL_Reg8  LNKSCORFRM;                     //000000E0
	HAL_Reg8  LNKSCORWD;                      //000000E1
	HAL_Reg8  LNKSCORBIT;                     //000000E2
	HAL_Reg8  ReservedBlk17[15];              //000000E3
	HAL_Reg8  SPSTPFRM;                       //000000F2
	HAL_Reg8  SPSTPWRD;                       //000000F3
	HAL_Reg8  SPSTPBIT;                       //000000F4
	HAL_Reg8  ReservedBlk18[2];               //000000F5
	HAL_Reg8  SHPCTRL;                        //000000F7
	HAL_Reg8  ReservedBlk19[1];               //000000F8
	HAL_Reg8  ROCTRL;                         //000000F9
	HAL_Reg16 ROUTPGL;                        //000000FA
	HAL_Reg16 NEXTPGL;                        //000000FC
	HAL_Reg8  PKTSZE;                         //000000FE
	HAL_Reg8  CDESPARE;                       //000000FF
};
// Register ENCDEC
#define HAL_CDE_CDEDBGSEL_REGISTER    (HAL_CDEREG_MAP->ENCDEC)
#define HAL_CDE_CDEDBGSEL_LSB         7
#define HAL_CDE_CDEDBGSEL_MSB         7
#define HAL_CDE_CDEDBGSEL_MASK        0x00000080
#define HAL_CDE_CDEDBGSEL_TYPE        Bool
#define HAL_CDE_CDEDBGSEL             HAL_CDE_CDEDBGSEL_MASK
#define HAL_CDE_CDEDBGSEL_READABLE    1
#define HAL_CDE_CDEDBGSEL_WRITABLE    1
#define HAL_CDE_CDEDBGSEL_SIGNED      0
#define HAL_CDE_CDEDBGSEL_ADR         0x10000104

#if (CHIP_REV >= 0xC0)

#define HAL_CDE_CDEDBGMD_REGISTER    (HAL_CDEREG_MAP->ENCDEC)
#define HAL_CDE_CDEDBGMD_LSB         4
#define HAL_CDE_CDEDBGMD_MSB         6
#define HAL_CDE_CDEDBGMD_MASK        0x00000070
#define HAL_CDE_CDEDBGMD_TYPE        halCDE_CDEDebugBusSelect_t
#define HAL_CDE_CDEDBGMD             HAL_CDE_CDEDBGMD_MASK
#define HAL_CDE_CDEDBGMD_READABLE    1
#define HAL_CDE_CDEDBGMD_WRITABLE    1
#define HAL_CDE_CDEDBGMD_SIGNED      0
#define HAL_CDE_CDEDBGMD_ADR         0x10000104

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_CDE_ENENC_REGISTER    (HAL_CDEREG_MAP->ENCDEC)
#define HAL_CDE_ENENC_LSB         0
#define HAL_CDE_ENENC_MSB         0
#define HAL_CDE_ENENC_MASK        0x00000001
#define HAL_CDE_ENENC_TYPE        Bool
#define HAL_CDE_ENENC             HAL_CDE_ENENC_MASK
#define HAL_CDE_ENENC_READABLE    1
#define HAL_CDE_ENENC_WRITABLE    1
#define HAL_CDE_ENENC_SIGNED      0
#define HAL_CDE_ENENC_ADR         0x10000104

// Register EINT0
#define HAL_CDE_WRCNTZO_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_WRCNTZO_LSB         7
#define HAL_CDE_WRCNTZO_MSB         7
#define HAL_CDE_WRCNTZO_MASK        0x00000080
#define HAL_CDE_WRCNTZO_TYPE        Bool
#define HAL_CDE_WRCNTZO             HAL_CDE_WRCNTZO_MASK
#define HAL_CDE_WRCNTZO_READABLE    1
#define HAL_CDE_WRCNTZO_WRITABLE    1
#define HAL_CDE_WRCNTZO_SIGNED      0
#define HAL_CDE_WRCNTZO_ADR         0x10000106

#define HAL_CDE_AMSFERR_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_AMSFERR_LSB         5
#define HAL_CDE_AMSFERR_MSB         5
#define HAL_CDE_AMSFERR_MASK        0x00000020
#define HAL_CDE_AMSFERR_TYPE        Bool
#define HAL_CDE_AMSFERR             HAL_CDE_AMSFERR_MASK
#define HAL_CDE_AMSFERR_READABLE    1
#define HAL_CDE_AMSFERR_WRITABLE    1
#define HAL_CDE_AMSFERR_SIGNED      0
#define HAL_CDE_AMSFERR_ADR         0x10000106

#define HAL_CDE_ASYOVR_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_ASYOVR_LSB         4
#define HAL_CDE_ASYOVR_MSB         4
#define HAL_CDE_ASYOVR_MASK        0x00000010
#define HAL_CDE_ASYOVR_TYPE        Bool
#define HAL_CDE_ASYOVR             HAL_CDE_ASYOVR_MASK
#define HAL_CDE_ASYOVR_READABLE    1
#define HAL_CDE_ASYOVR_WRITABLE    1
#define HAL_CDE_ASYOVR_SIGNED      0
#define HAL_CDE_ASYOVR_ADR         0x10000106

#define HAL_CDE_WRTBRK_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_WRTBRK_LSB         3
#define HAL_CDE_WRTBRK_MSB         3
#define HAL_CDE_WRTBRK_MASK        0x00000008
#define HAL_CDE_WRTBRK_TYPE        Bool
#define HAL_CDE_WRTBRK             HAL_CDE_WRTBRK_MASK
#define HAL_CDE_WRTBRK_READABLE    1
#define HAL_CDE_WRTBRK_WRITABLE    1
#define HAL_CDE_WRTBRK_SIGNED      0
#define HAL_CDE_WRTBRK_ADR         0x10000106

#define HAL_CDE_PAUSE_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_PAUSE_LSB         2
#define HAL_CDE_PAUSE_MSB         2
#define HAL_CDE_PAUSE_MASK        0x00000004
#define HAL_CDE_PAUSE_TYPE        Bool
#define HAL_CDE_PAUSE             HAL_CDE_PAUSE_MASK
#define HAL_CDE_PAUSE_READABLE    1
#define HAL_CDE_PAUSE_WRITABLE    1
#define HAL_CDE_PAUSE_SIGNED      0
#define HAL_CDE_PAUSE_ADR         0x10000106

#define HAL_CDE_UNDERRUN_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_UNDERRUN_LSB         1
#define HAL_CDE_UNDERRUN_MSB         1
#define HAL_CDE_UNDERRUN_MASK        0x00000002
#define HAL_CDE_UNDERRUN_TYPE        Bool
#define HAL_CDE_UNDERRUN             HAL_CDE_UNDERRUN_MASK
#define HAL_CDE_UNDERRUN_READABLE    1
#define HAL_CDE_UNDERRUN_WRITABLE    1
#define HAL_CDE_UNDERRUN_SIGNED      0
#define HAL_CDE_UNDERRUN_ADR         0x10000106

#define HAL_CDE_WRTSTOP_REGISTER    (HAL_CDEREG_MAP->EINT0)
#define HAL_CDE_WRTSTOP_LSB         0
#define HAL_CDE_WRTSTOP_MSB         0
#define HAL_CDE_WRTSTOP_MASK        0x00000001
#define HAL_CDE_WRTSTOP_TYPE        Bool
#define HAL_CDE_WRTSTOP             HAL_CDE_WRTSTOP_MASK
#define HAL_CDE_WRTSTOP_READABLE    1
#define HAL_CDE_WRTSTOP_WRITABLE    1
#define HAL_CDE_WRTSTOP_SIGNED      0
#define HAL_CDE_WRTSTOP_ADR         0x10000106

// Register EINT1
#define HAL_CDE_ESFS_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_ESFS_LSB         6
#define HAL_CDE_ESFS_MSB         6
#define HAL_CDE_ESFS_MASK        0x00000040
#define HAL_CDE_ESFS_TYPE        Bool
#define HAL_CDE_ESFS             HAL_CDE_ESFS_MASK
#define HAL_CDE_ESFS_READABLE    1
#define HAL_CDE_ESFS_WRITABLE    1
#define HAL_CDE_ESFS_SIGNED      0
#define HAL_CDE_ESFS_ADR         0x10000107

#define HAL_CDE_TARGEQ_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_TARGEQ_LSB         4
#define HAL_CDE_TARGEQ_MSB         4
#define HAL_CDE_TARGEQ_MASK        0x00000010
#define HAL_CDE_TARGEQ_TYPE        Bool
#define HAL_CDE_TARGEQ             HAL_CDE_TARGEQ_MASK
#define HAL_CDE_TARGEQ_READABLE    1
#define HAL_CDE_TARGEQ_WRITABLE    1
#define HAL_CDE_TARGEQ_SIGNED      0
#define HAL_CDE_TARGEQ_ADR         0x10000107

#define HAL_CDE_EPRD_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_EPRD_LSB         3
#define HAL_CDE_EPRD_MSB         3
#define HAL_CDE_EPRD_MASK        0x00000008
#define HAL_CDE_EPRD_TYPE        Bool
#define HAL_CDE_EPRD             HAL_CDE_EPRD_MASK
#define HAL_CDE_EPRD_READABLE    1
#define HAL_CDE_EPRD_WRITABLE    1
#define HAL_CDE_EPRD_SIGNED      0
#define HAL_CDE_EPRD_ADR         0x10000107

#define HAL_CDE_CPRD_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_CPRD_LSB         2
#define HAL_CDE_CPRD_MSB         2
#define HAL_CDE_CPRD_MASK        0x00000004
#define HAL_CDE_CPRD_TYPE        Bool
#define HAL_CDE_CPRD             HAL_CDE_CPRD_MASK
#define HAL_CDE_CPRD_READABLE    1
#define HAL_CDE_CPRD_WRITABLE    1
#define HAL_CDE_CPRD_SIGNED      0
#define HAL_CDE_CPRD_ADR         0x10000107

#define HAL_CDE_RPTSTART_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_RPTSTART_LSB         1
#define HAL_CDE_RPTSTART_MSB         1
#define HAL_CDE_RPTSTART_MASK        0x00000002
#define HAL_CDE_RPTSTART_TYPE        Bool
#define HAL_CDE_RPTSTART             HAL_CDE_RPTSTART_MASK
#define HAL_CDE_RPTSTART_READABLE    1
#define HAL_CDE_RPTSTART_WRITABLE    1
#define HAL_CDE_RPTSTART_SIGNED      0
#define HAL_CDE_RPTSTART_ADR         0x10000107

#define HAL_CDE_SPCMP_REGISTER    (HAL_CDEREG_MAP->EINT1)
#define HAL_CDE_SPCMP_LSB         0
#define HAL_CDE_SPCMP_MSB         0
#define HAL_CDE_SPCMP_MASK        0x00000001
#define HAL_CDE_SPCMP_TYPE        Bool
#define HAL_CDE_SPCMP             HAL_CDE_SPCMP_MASK
#define HAL_CDE_SPCMP_READABLE    1
#define HAL_CDE_SPCMP_WRITABLE    1
#define HAL_CDE_SPCMP_SIGNED      0
#define HAL_CDE_SPCMP_ADR         0x10000107

// Register EINTEN0
#define HAL_CDE_WRCNTZOEN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_WRCNTZOEN_LSB         7
#define HAL_CDE_WRCNTZOEN_MSB         7
#define HAL_CDE_WRCNTZOEN_MASK        0x00000080
#define HAL_CDE_WRCNTZOEN_TYPE        Bool
#define HAL_CDE_WRCNTZOEN             HAL_CDE_WRCNTZOEN_MASK
#define HAL_CDE_WRCNTZOEN_READABLE    1
#define HAL_CDE_WRCNTZOEN_WRITABLE    1
#define HAL_CDE_WRCNTZOEN_SIGNED      0
#define HAL_CDE_WRCNTZOEN_ADR         0x1000010B

#define HAL_CDE_AMSFERREN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_AMSFERREN_LSB         5
#define HAL_CDE_AMSFERREN_MSB         5
#define HAL_CDE_AMSFERREN_MASK        0x00000020
#define HAL_CDE_AMSFERREN_TYPE        Bool
#define HAL_CDE_AMSFERREN             HAL_CDE_AMSFERREN_MASK
#define HAL_CDE_AMSFERREN_READABLE    1
#define HAL_CDE_AMSFERREN_WRITABLE    1
#define HAL_CDE_AMSFERREN_SIGNED      0
#define HAL_CDE_AMSFERREN_ADR         0x1000010B

#define HAL_CDE_ASYOVREN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_ASYOVREN_LSB         4
#define HAL_CDE_ASYOVREN_MSB         4
#define HAL_CDE_ASYOVREN_MASK        0x00000010
#define HAL_CDE_ASYOVREN_TYPE        Bool
#define HAL_CDE_ASYOVREN             HAL_CDE_ASYOVREN_MASK
#define HAL_CDE_ASYOVREN_READABLE    1
#define HAL_CDE_ASYOVREN_WRITABLE    1
#define HAL_CDE_ASYOVREN_SIGNED      0
#define HAL_CDE_ASYOVREN_ADR         0x1000010B

#define HAL_CDE_WRTBRKEN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_WRTBRKEN_LSB         3
#define HAL_CDE_WRTBRKEN_MSB         3
#define HAL_CDE_WRTBRKEN_MASK        0x00000008
#define HAL_CDE_WRTBRKEN_TYPE        Bool
#define HAL_CDE_WRTBRKEN             HAL_CDE_WRTBRKEN_MASK
#define HAL_CDE_WRTBRKEN_READABLE    1
#define HAL_CDE_WRTBRKEN_WRITABLE    1
#define HAL_CDE_WRTBRKEN_SIGNED      0
#define HAL_CDE_WRTBRKEN_ADR         0x1000010B

#define HAL_CDE_PAUSEN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_PAUSEN_LSB         2
#define HAL_CDE_PAUSEN_MSB         2
#define HAL_CDE_PAUSEN_MASK        0x00000004
#define HAL_CDE_PAUSEN_TYPE        Bool
#define HAL_CDE_PAUSEN             HAL_CDE_PAUSEN_MASK
#define HAL_CDE_PAUSEN_READABLE    1
#define HAL_CDE_PAUSEN_WRITABLE    1
#define HAL_CDE_PAUSEN_SIGNED      0
#define HAL_CDE_PAUSEN_ADR         0x1000010B

#define HAL_CDE_UNRUNEN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_UNRUNEN_LSB         1
#define HAL_CDE_UNRUNEN_MSB         1
#define HAL_CDE_UNRUNEN_MASK        0x00000002
#define HAL_CDE_UNRUNEN_TYPE        Bool
#define HAL_CDE_UNRUNEN             HAL_CDE_UNRUNEN_MASK
#define HAL_CDE_UNRUNEN_READABLE    1
#define HAL_CDE_UNRUNEN_WRITABLE    1
#define HAL_CDE_UNRUNEN_SIGNED      0
#define HAL_CDE_UNRUNEN_ADR         0x1000010B

#define HAL_CDE_WRTSTPEN_REGISTER    (HAL_CDEREG_MAP->EINTEN0)
#define HAL_CDE_WRTSTPEN_LSB         0
#define HAL_CDE_WRTSTPEN_MSB         0
#define HAL_CDE_WRTSTPEN_MASK        0x00000001
#define HAL_CDE_WRTSTPEN_TYPE        Bool
#define HAL_CDE_WRTSTPEN             HAL_CDE_WRTSTPEN_MASK
#define HAL_CDE_WRTSTPEN_READABLE    1
#define HAL_CDE_WRTSTPEN_WRITABLE    1
#define HAL_CDE_WRTSTPEN_SIGNED      0
#define HAL_CDE_WRTSTPEN_ADR         0x1000010B

// Register EINTEN1
#define HAL_CDE_ESFSEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_ESFSEN_LSB         6
#define HAL_CDE_ESFSEN_MSB         6
#define HAL_CDE_ESFSEN_MASK        0x00000040
#define HAL_CDE_ESFSEN_TYPE        Bool
#define HAL_CDE_ESFSEN             HAL_CDE_ESFSEN_MASK
#define HAL_CDE_ESFSEN_READABLE    1
#define HAL_CDE_ESFSEN_WRITABLE    1
#define HAL_CDE_ESFSEN_SIGNED      0
#define HAL_CDE_ESFSEN_ADR         0x1000010C

#define HAL_CDE_TRGEQEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_TRGEQEN_LSB         4
#define HAL_CDE_TRGEQEN_MSB         4
#define HAL_CDE_TRGEQEN_MASK        0x00000010
#define HAL_CDE_TRGEQEN_TYPE        Bool
#define HAL_CDE_TRGEQEN             HAL_CDE_TRGEQEN_MASK
#define HAL_CDE_TRGEQEN_READABLE    1
#define HAL_CDE_TRGEQEN_WRITABLE    1
#define HAL_CDE_TRGEQEN_SIGNED      0
#define HAL_CDE_TRGEQEN_ADR         0x1000010C

#define HAL_CDE_EPRDEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_EPRDEN_LSB         3
#define HAL_CDE_EPRDEN_MSB         3
#define HAL_CDE_EPRDEN_MASK        0x00000008
#define HAL_CDE_EPRDEN_TYPE        Bool
#define HAL_CDE_EPRDEN             HAL_CDE_EPRDEN_MASK
#define HAL_CDE_EPRDEN_READABLE    1
#define HAL_CDE_EPRDEN_WRITABLE    1
#define HAL_CDE_EPRDEN_SIGNED      0
#define HAL_CDE_EPRDEN_ADR         0x1000010C

#define HAL_CDE_CPRDEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_CPRDEN_LSB         2
#define HAL_CDE_CPRDEN_MSB         2
#define HAL_CDE_CPRDEN_MASK        0x00000004
#define HAL_CDE_CPRDEN_TYPE        Bool
#define HAL_CDE_CPRDEN             HAL_CDE_CPRDEN_MASK
#define HAL_CDE_CPRDEN_READABLE    1
#define HAL_CDE_CPRDEN_WRITABLE    1
#define HAL_CDE_CPRDEN_SIGNED      0
#define HAL_CDE_CPRDEN_ADR         0x1000010C

#define HAL_CDE_RPTSTEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_RPTSTEN_LSB         1
#define HAL_CDE_RPTSTEN_MSB         1
#define HAL_CDE_RPTSTEN_MASK        0x00000002
#define HAL_CDE_RPTSTEN_TYPE        Bool
#define HAL_CDE_RPTSTEN             HAL_CDE_RPTSTEN_MASK
#define HAL_CDE_RPTSTEN_READABLE    1
#define HAL_CDE_RPTSTEN_WRITABLE    1
#define HAL_CDE_RPTSTEN_SIGNED      0
#define HAL_CDE_RPTSTEN_ADR         0x1000010C

#define HAL_CDE_SPCMPEN_REGISTER    (HAL_CDEREG_MAP->EINTEN1)
#define HAL_CDE_SPCMPEN_LSB         0
#define HAL_CDE_SPCMPEN_MSB         0
#define HAL_CDE_SPCMPEN_MASK        0x00000001
#define HAL_CDE_SPCMPEN_TYPE        Bool
#define HAL_CDE_SPCMPEN             HAL_CDE_SPCMPEN_MASK
#define HAL_CDE_SPCMPEN_READABLE    1
#define HAL_CDE_SPCMPEN_WRITABLE    1
#define HAL_CDE_SPCMPEN_SIGNED      0
#define HAL_CDE_SPCMPEN_ADR         0x1000010C

// Register CDIFCNT
#define HAL_CDE_SCROFF_REGISTER    (HAL_CDEREG_MAP->CDIFCNT)
#define HAL_CDE_SCROFF_LSB         3
#define HAL_CDE_SCROFF_MSB         3
#define HAL_CDE_SCROFF_MASK        0x00000008
#define HAL_CDE_SCROFF_TYPE        Bool
#define HAL_CDE_SCROFF             HAL_CDE_SCROFF_MASK
#define HAL_CDE_SCROFF_READABLE    1
#define HAL_CDE_SCROFF_WRITABLE    1
#define HAL_CDE_SCROFF_SIGNED      0
#define HAL_CDE_SCROFF_ADR         0x10000110

#define HAL_CDE_NSYNC_REGISTER    (HAL_CDEREG_MAP->CDIFCNT)
#define HAL_CDE_NSYNC_LSB         2
#define HAL_CDE_NSYNC_MSB         2
#define HAL_CDE_NSYNC_MASK        0x00000004
#define HAL_CDE_NSYNC_TYPE        Bool
#define HAL_CDE_NSYNC             HAL_CDE_NSYNC_MASK
#define HAL_CDE_NSYNC_READABLE    1
#define HAL_CDE_NSYNC_WRITABLE    1
#define HAL_CDE_NSYNC_SIGNED      0
#define HAL_CDE_NSYNC_ADR         0x10000110

// Register SECCNT0
#define HAL_CDE_XWINITSP_REGISTER    (HAL_CDEREG_MAP->SECCNT0)
#define HAL_CDE_XWINITSP_LSB         7
#define HAL_CDE_XWINITSP_MSB         7
#define HAL_CDE_XWINITSP_MASK        0x00000080
#define HAL_CDE_XWINITSP_TYPE        Bool
#define HAL_CDE_XWINITSP             HAL_CDE_XWINITSP_MASK
#define HAL_CDE_XWINITSP_READABLE    1
#define HAL_CDE_XWINITSP_WRITABLE    1
#define HAL_CDE_XWINITSP_SIGNED      0
#define HAL_CDE_XWINITSP_ADR         0x10000112

#define HAL_CDE_MANUMODE_REGISTER    (HAL_CDEREG_MAP->SECCNT0)
#define HAL_CDE_MANUMODE_LSB         2
#define HAL_CDE_MANUMODE_MSB         4
#define HAL_CDE_MANUMODE_MASK        0x0000001C
#define HAL_CDE_MANUMODE_TYPE        halCDE_ManualModeSelect_t
#define HAL_CDE_MANUMODE             HAL_CDE_MANUMODE_MASK
#define HAL_CDE_MANUMODE_READABLE    1
#define HAL_CDE_MANUMODE_WRITABLE    1
#define HAL_CDE_MANUMODE_SIGNED      0
#define HAL_CDE_MANUMODE_ADR         0x10000112

#define HAL_CDE_AUTOMODE_REGISTER    (HAL_CDEREG_MAP->SECCNT0)
#define HAL_CDE_AUTOMODE_LSB         0
#define HAL_CDE_AUTOMODE_MSB         1
#define HAL_CDE_AUTOMODE_MASK        0x00000003
#define HAL_CDE_AUTOMODE_TYPE        halCDE_AutoModeSelect_t
#define HAL_CDE_AUTOMODE             HAL_CDE_AUTOMODE_MASK
#define HAL_CDE_AUTOMODE_READABLE    1
#define HAL_CDE_AUTOMODE_WRITABLE    1
#define HAL_CDE_AUTOMODE_SIGNED      0
#define HAL_CDE_AUTOMODE_ADR         0x10000112

// Register SECCNT2
#define HAL_CDE_MSFREL_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_MSFREL_LSB         7
#define HAL_CDE_MSFREL_MSB         7
#define HAL_CDE_MSFREL_MASK        0x00000080
#define HAL_CDE_MSFREL_TYPE        Bool
#define HAL_CDE_MSFREL             HAL_CDE_MSFREL_MASK
#define HAL_CDE_MSFREL_READABLE    1
#define HAL_CDE_MSFREL_WRITABLE    1
#define HAL_CDE_MSFREL_SIGNED      0
#define HAL_CDE_MSFREL_ADR         0x10000114

#define HAL_CDE_SHPLUS_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_SHPLUS_LSB         6
#define HAL_CDE_SHPLUS_MSB         6
#define HAL_CDE_SHPLUS_MASK        0x00000040
#define HAL_CDE_SHPLUS_TYPE        Bool
#define HAL_CDE_SHPLUS             HAL_CDE_SHPLUS_MASK
#define HAL_CDE_SHPLUS_READABLE    1
#define HAL_CDE_SHPLUS_WRITABLE    1
#define HAL_CDE_SHPLUS_SIGNED      0
#define HAL_CDE_SHPLUS_ADR         0x10000114

#define HAL_CDE_HPLUS_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_HPLUS_LSB         5
#define HAL_CDE_HPLUS_MSB         5
#define HAL_CDE_HPLUS_MASK        0x00000020
#define HAL_CDE_HPLUS_TYPE        Bool
#define HAL_CDE_HPLUS             HAL_CDE_HPLUS_MASK
#define HAL_CDE_HPLUS_READABLE    1
#define HAL_CDE_HPLUS_WRITABLE    1
#define HAL_CDE_HPLUS_SIGNED      0
#define HAL_CDE_HPLUS_ADR         0x10000114

#define HAL_CDE_MSFABS_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_MSFABS_LSB         4
#define HAL_CDE_MSFABS_MSB         4
#define HAL_CDE_MSFABS_MASK        0x00000010
#define HAL_CDE_MSFABS_TYPE        Bool
#define HAL_CDE_MSFABS             HAL_CDE_MSFABS_MASK
#define HAL_CDE_MSFABS_READABLE    1
#define HAL_CDE_MSFABS_WRITABLE    1
#define HAL_CDE_MSFABS_SIGNED      0
#define HAL_CDE_MSFABS_ADR         0x10000114

#define HAL_CDE_TLOAD_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_TLOAD_LSB         3
#define HAL_CDE_TLOAD_MSB         3
#define HAL_CDE_TLOAD_MASK        0x00000008
#define HAL_CDE_TLOAD_TYPE        Bool
#define HAL_CDE_TLOAD             HAL_CDE_TLOAD_MASK
#define HAL_CDE_TLOAD_READABLE    1
#define HAL_CDE_TLOAD_WRITABLE    1
#define HAL_CDE_TLOAD_SIGNED      0
#define HAL_CDE_TLOAD_ADR         0x10000114

#define HAL_CDE_TINCEN_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_TINCEN_LSB         2
#define HAL_CDE_TINCEN_MSB         2
#define HAL_CDE_TINCEN_MASK        0x00000004
#define HAL_CDE_TINCEN_TYPE        Bool
#define HAL_CDE_TINCEN             HAL_CDE_TINCEN_MASK
#define HAL_CDE_TINCEN_READABLE    1
#define HAL_CDE_TINCEN_WRITABLE    1
#define HAL_CDE_TINCEN_SIGNED      0
#define HAL_CDE_TINCEN_ADR         0x10000114

#define HAL_CDE_ENCSMODE_REGISTER    (HAL_CDEREG_MAP->SECCNT2)
#define HAL_CDE_ENCSMODE_LSB         0
#define HAL_CDE_ENCSMODE_MSB         1
#define HAL_CDE_ENCSMODE_MASK        0x00000003
#define HAL_CDE_ENCSMODE_TYPE        halCDE_SectorProcessorEncodeModeSelect_t
#define HAL_CDE_ENCSMODE             HAL_CDE_ENCSMODE_MASK
#define HAL_CDE_ENCSMODE_READABLE    1
#define HAL_CDE_ENCSMODE_WRITABLE    1
#define HAL_CDE_ENCSMODE_SIGNED      0
#define HAL_CDE_ENCSMODE_ADR         0x10000114

// Register SECMON
#define HAL_CDE_SPSTATE_REGISTER    (HAL_CDEREG_MAP->SECMON)
#define HAL_CDE_SPSTATE_LSB         4
#define HAL_CDE_SPSTATE_MSB         5
#define HAL_CDE_SPSTATE_MASK        0x00000030
#define HAL_CDE_SPSTATE_TYPE        halCDE_SectorProcessorStatus_t
#define HAL_CDE_SPSTATE             HAL_CDE_SPSTATE_MASK
#define HAL_CDE_SPSTATE_READABLE    1
#define HAL_CDE_SPSTATE_WRITABLE    0
#define HAL_CDE_SPSTATE_SIGNED      0
#define HAL_CDE_SPSTATE_ADR         0x10000115

#define HAL_CDE_RTSTATE_REGISTER    (HAL_CDEREG_MAP->SECMON)
#define HAL_CDE_RTSTATE_LSB         0
#define HAL_CDE_RTSTATE_MSB         3
#define HAL_CDE_RTSTATE_MASK        0x0000000F
#define HAL_CDE_RTSTATE_TYPE        halCDE_RealTimeEncodeStatus_t
#define HAL_CDE_RTSTATE             HAL_CDE_RTSTATE_MASK
#define HAL_CDE_RTSTATE_READABLE    1
#define HAL_CDE_RTSTATE_WRITABLE    0
#define HAL_CDE_RTSTATE_SIGNED      0
#define HAL_CDE_RTSTATE_ADR         0x10000115

// Register PERIODPG
#define HAL_CDE_EPRDPG_REGISTER    (HAL_CDEREG_MAP->PERIODPG)
#define HAL_CDE_EPRDPG_LSB         0
#define HAL_CDE_EPRDPG_MSB         7
#define HAL_CDE_EPRDPG_MASK        0x000000FF
#define HAL_CDE_EPRDPG_TYPE        UByte
#define HAL_CDE_EPRDPG             HAL_CDE_EPRDPG_MASK
#define HAL_CDE_EPRDPG_READABLE    1
#define HAL_CDE_EPRDPG_WRITABLE    1
#define HAL_CDE_EPRDPG_SIGNED      0
#define HAL_CDE_EPRDPG_ADR         0x10000116

// Register HMIN
#define HAL_CDE_BCDHMIN_REGISTER    (HAL_CDEREG_MAP->HMIN)
#define HAL_CDE_BCDHMIN_LSB         0
#define HAL_CDE_BCDHMIN_MSB         7
#define HAL_CDE_BCDHMIN_MASK        0x000000FF
#define HAL_CDE_BCDHMIN_TYPE        UByte
#define HAL_CDE_BCDHMIN             HAL_CDE_BCDHMIN_MASK
#define HAL_CDE_BCDHMIN_READABLE    1
#define HAL_CDE_BCDHMIN_WRITABLE    1
#define HAL_CDE_BCDHMIN_SIGNED      0
#define HAL_CDE_BCDHMIN_ADR         0x10000118

// Register HSEC
#define HAL_CDE_BCDHSEC_REGISTER    (HAL_CDEREG_MAP->HSEC)
#define HAL_CDE_BCDHSEC_LSB         0
#define HAL_CDE_BCDHSEC_MSB         7
#define HAL_CDE_BCDHSEC_MASK        0x000000FF
#define HAL_CDE_BCDHSEC_TYPE        UByte
#define HAL_CDE_BCDHSEC             HAL_CDE_BCDHSEC_MASK
#define HAL_CDE_BCDHSEC_READABLE    1
#define HAL_CDE_BCDHSEC_WRITABLE    1
#define HAL_CDE_BCDHSEC_SIGNED      0
#define HAL_CDE_BCDHSEC_ADR         0x10000119

// Register HFRM
#define HAL_CDE_BCDHFRM_REGISTER    (HAL_CDEREG_MAP->HFRM)
#define HAL_CDE_BCDHFRM_LSB         0
#define HAL_CDE_BCDHFRM_MSB         7
#define HAL_CDE_BCDHFRM_MASK        0x000000FF
#define HAL_CDE_BCDHFRM_TYPE        UByte
#define HAL_CDE_BCDHFRM             HAL_CDE_BCDHFRM_MASK
#define HAL_CDE_BCDHFRM_READABLE    1
#define HAL_CDE_BCDHFRM_WRITABLE    1
#define HAL_CDE_BCDHFRM_SIGNED      0
#define HAL_CDE_BCDHFRM_ADR         0x1000011A

// Register HMODE
#define HAL_CDE_HMODE_REGISTER    (HAL_CDEREG_MAP->HMODE)
#define HAL_CDE_HMODE_LSB         0
#define HAL_CDE_HMODE_MSB         7
#define HAL_CDE_HMODE_MASK        0x000000FF
#define HAL_CDE_HMODE_TYPE        UByte
#define HAL_CDE_HMODE             HAL_CDE_HMODE_MASK
#define HAL_CDE_HMODE_READABLE    1
#define HAL_CDE_HMODE_WRITABLE    1
#define HAL_CDE_HMODE_SIGNED      0
#define HAL_CDE_HMODE_ADR         0x1000011B

// Register SHFNUM
#define HAL_CDE_SHFNUM_REGISTER    (HAL_CDEREG_MAP->SHFNUM)
#define HAL_CDE_SHFNUM_LSB         0
#define HAL_CDE_SHFNUM_MSB         7
#define HAL_CDE_SHFNUM_MASK        0x000000FF
#define HAL_CDE_SHFNUM_TYPE        UByte
#define HAL_CDE_SHFNUM             HAL_CDE_SHFNUM_MASK
#define HAL_CDE_SHFNUM_READABLE    1
#define HAL_CDE_SHFNUM_WRITABLE    1
#define HAL_CDE_SHFNUM_SIGNED      0
#define HAL_CDE_SHFNUM_ADR         0x1000011C

// Register SHCNUM
#define HAL_CDE_SHCNUM_REGISTER    (HAL_CDEREG_MAP->SHCNUM)
#define HAL_CDE_SHCNUM_LSB         0
#define HAL_CDE_SHCNUM_MSB         7
#define HAL_CDE_SHCNUM_MASK        0x000000FF
#define HAL_CDE_SHCNUM_TYPE        UByte
#define HAL_CDE_SHCNUM             HAL_CDE_SHCNUM_MASK
#define HAL_CDE_SHCNUM_READABLE    1
#define HAL_CDE_SHCNUM_WRITABLE    1
#define HAL_CDE_SHCNUM_SIGNED      0
#define HAL_CDE_SHCNUM_ADR         0x1000011D

// Register SHSMODE
#define HAL_CDE_SHSMODE_REGISTER    (HAL_CDEREG_MAP->SHSMODE)
#define HAL_CDE_SHSMODE_LSB         0
#define HAL_CDE_SHSMODE_MSB         7
#define HAL_CDE_SHSMODE_MASK        0x000000FF
#define HAL_CDE_SHSMODE_TYPE        UByte
#define HAL_CDE_SHSMODE             HAL_CDE_SHSMODE_MASK
#define HAL_CDE_SHSMODE_READABLE    1
#define HAL_CDE_SHSMODE_WRITABLE    1
#define HAL_CDE_SHSMODE_SIGNED      0
#define HAL_CDE_SHSMODE_ADR         0x1000011E

// Register SHCINFO
#define HAL_CDE_SHCINFO_REGISTER    (HAL_CDEREG_MAP->SHCINFO)
#define HAL_CDE_SHCINFO_LSB         0
#define HAL_CDE_SHCINFO_MSB         7
#define HAL_CDE_SHCINFO_MASK        0x000000FF
#define HAL_CDE_SHCINFO_TYPE        UByte
#define HAL_CDE_SHCINFO             HAL_CDE_SHCINFO_MASK
#define HAL_CDE_SHCINFO_READABLE    1
#define HAL_CDE_SHCINFO_WRITABLE    1
#define HAL_CDE_SHCINFO_SIGNED      0
#define HAL_CDE_SHCINFO_ADR         0x1000011F

// Register ARMSFL
#define HAL_CDE_BCDARMSFL_REGISTER    (HAL_CDEREG_MAP->ARMSFL)
#define HAL_CDE_BCDARMSFL_LSB         0
#define HAL_CDE_BCDARMSFL_MSB         7
#define HAL_CDE_BCDARMSFL_MASK        0x000000FF
#define HAL_CDE_BCDARMSFL_TYPE        UByte
#define HAL_CDE_BCDARMSFL             HAL_CDE_BCDARMSFL_MASK
#define HAL_CDE_BCDARMSFL_READABLE    1
#define HAL_CDE_BCDARMSFL_WRITABLE    0
#define HAL_CDE_BCDARMSFL_SIGNED      0
#define HAL_CDE_BCDARMSFL_ADR         0x10000120

// Register ARMSFM
#define HAL_CDE_BCDARMSFM_REGISTER    (HAL_CDEREG_MAP->ARMSFM)
#define HAL_CDE_BCDARMSFM_LSB         0
#define HAL_CDE_BCDARMSFM_MSB         7
#define HAL_CDE_BCDARMSFM_MASK        0x000000FF
#define HAL_CDE_BCDARMSFM_TYPE        UByte
#define HAL_CDE_BCDARMSFM             HAL_CDE_BCDARMSFM_MASK
#define HAL_CDE_BCDARMSFM_READABLE    1
#define HAL_CDE_BCDARMSFM_WRITABLE    0
#define HAL_CDE_BCDARMSFM_SIGNED      0
#define HAL_CDE_BCDARMSFM_ADR         0x10000121

// Register ARMSFU
#define HAL_CDE_BCDARMSFU_REGISTER    (HAL_CDEREG_MAP->ARMSFU)
#define HAL_CDE_BCDARMSFU_LSB         0
#define HAL_CDE_BCDARMSFU_MSB         7
#define HAL_CDE_BCDARMSFU_MASK        0x000000FF
#define HAL_CDE_BCDARMSFU_TYPE        UByte
#define HAL_CDE_BCDARMSFU             HAL_CDE_BCDARMSFU_MASK
#define HAL_CDE_BCDARMSFU_READABLE    1
#define HAL_CDE_BCDARMSFU_WRITABLE    0
#define HAL_CDE_BCDARMSFU_SIGNED      0
#define HAL_CDE_BCDARMSFU_ADR         0x10000122

// Register CPPTR
#define HAL_CDE_CPPTR_REGISTER    (HAL_CDEREG_MAP->CPPTR)
#define HAL_CDE_CPPTR_LSB         0
#define HAL_CDE_CPPTR_MSB         7
#define HAL_CDE_CPPTR_MASK        0x000000FF
#define HAL_CDE_CPPTR_TYPE        UByte
#define HAL_CDE_CPPTR             HAL_CDE_CPPTR_MASK
#define HAL_CDE_CPPTR_READABLE    1
#define HAL_CDE_CPPTR_WRITABLE    1
#define HAL_CDE_CPPTR_SIGNED      0
#define HAL_CDE_CPPTR_ADR         0x10000123

// Register CPPTRB
#define HAL_CDE_CPSEQA_REGISTER    (HAL_CDEREG_MAP->CPPTRB)
#define HAL_CDE_CPSEQA_LSB         7
#define HAL_CDE_CPSEQA_MSB         7
#define HAL_CDE_CPSEQA_MASK        0x00000080
#define HAL_CDE_CPSEQA_TYPE        Bool
#define HAL_CDE_CPSEQA             HAL_CDE_CPSEQA_MASK
#define HAL_CDE_CPSEQA_READABLE    1
#define HAL_CDE_CPSEQA_WRITABLE    1
#define HAL_CDE_CPSEQA_SIGNED      0
#define HAL_CDE_CPSEQA_ADR         0x10000124

#define HAL_CDE_CPPTRB_REGISTER    (HAL_CDEREG_MAP->CPPTRB)
#define HAL_CDE_CPPTRB_LSB         0
#define HAL_CDE_CPPTRB_MSB         6
#define HAL_CDE_CPPTRB_MASK        0x0000007F
#define HAL_CDE_CPPTRB_TYPE        UByte
#define HAL_CDE_CPPTRB             HAL_CDE_CPPTRB_MASK
#define HAL_CDE_CPPTRB_READABLE    1
#define HAL_CDE_CPPTRB_WRITABLE    1
#define HAL_CDE_CPPTRB_SIGNED      0
#define HAL_CDE_CPPTRB_ADR         0x10000124

// Register CPQCTL
#define HAL_CDE_CPCPYCNT_REGISTER    (HAL_CDEREG_MAP->CPQCTL)
#define HAL_CDE_CPCPYCNT_LSB         4
#define HAL_CDE_CPCPYCNT_MSB         7
#define HAL_CDE_CPCPYCNT_MASK        0x000000F0
#define HAL_CDE_CPCPYCNT_TYPE        UByte
#define HAL_CDE_CPCPYCNT             HAL_CDE_CPCPYCNT_MASK
#define HAL_CDE_CPCPYCNT_READABLE    1
#define HAL_CDE_CPCPYCNT_WRITABLE    1
#define HAL_CDE_CPCPYCNT_SIGNED      0
#define HAL_CDE_CPCPYCNT_ADR         0x10000125

#define HAL_CDE_CPZROCNT_REGISTER    (HAL_CDEREG_MAP->CPQCTL)
#define HAL_CDE_CPZROCNT_LSB         0
#define HAL_CDE_CPZROCNT_MSB         3
#define HAL_CDE_CPZROCNT_MASK        0x0000000F
#define HAL_CDE_CPZROCNT_TYPE        UByte
#define HAL_CDE_CPZROCNT             HAL_CDE_CPZROCNT_MASK
#define HAL_CDE_CPZROCNT_READABLE    1
#define HAL_CDE_CPZROCNT_WRITABLE    1
#define HAL_CDE_CPZROCNT_SIGNED      0
#define HAL_CDE_CPZROCNT_ADR         0x10000125

// Register CPRPTSTA
#define HAL_CDE_CPRPTSTA_REGISTER    (HAL_CDEREG_MAP->CPRPTSTA)
#define HAL_CDE_CPRPTSTA_LSB         0
#define HAL_CDE_CPRPTSTA_MSB         7
#define HAL_CDE_CPRPTSTA_MASK        0x000000FF
#define HAL_CDE_CPRPTSTA_TYPE        UByte
#define HAL_CDE_CPRPTSTA             HAL_CDE_CPRPTSTA_MASK
#define HAL_CDE_CPRPTSTA_READABLE    1
#define HAL_CDE_CPRPTSTA_WRITABLE    1
#define HAL_CDE_CPRPTSTA_SIGNED      0
#define HAL_CDE_CPRPTSTA_ADR         0x10000126

// Register ENSRAMCTL
#define HAL_CDE_ENCSRAMDIS_REGISTER    (HAL_CDEREG_MAP->ENSRAMCTL)
#define HAL_CDE_ENCSRAMDIS_LSB         5
#define HAL_CDE_ENCSRAMDIS_MSB         5
#define HAL_CDE_ENCSRAMDIS_MASK        0x00000020
#define HAL_CDE_ENCSRAMDIS_TYPE        Bool
#define HAL_CDE_ENCSRAMDIS             HAL_CDE_ENCSRAMDIS_MASK
#define HAL_CDE_ENCSRAMDIS_READABLE    1
#define HAL_CDE_ENCSRAMDIS_WRITABLE    1
#define HAL_CDE_ENCSRAMDIS_SIGNED      0
#define HAL_CDE_ENCSRAMDIS_ADR         0x10000136

// Registers ENBUFSPGL/ENBUFSPGH
#define HAL_CDE_ENBUFSPG_REGISTER    (HAL_CDEREG_MAP->ENBUFSPGL)
#define HAL_CDE_ENBUFSPG_LSB         0
#define HAL_CDE_ENBUFSPG_MSB         12
#define HAL_CDE_ENBUFSPG_MASK        0x00001FFF
#define HAL_CDE_ENBUFSPG_TYPE        UInt16
#define HAL_CDE_ENBUFSPG             HAL_CDE_ENBUFSPG_MASK
#define HAL_CDE_ENBUFSPG_READABLE    1
#define HAL_CDE_ENBUFSPG_WRITABLE    1
#define HAL_CDE_ENBUFSPG_SIGNED      0
#define HAL_CDE_ENBUFSPG_ADR         0x1000013A

// Registers ENBUFEPGL/ENBUFEPGH
#define HAL_CDE_ENBUFEPG_REGISTER    (HAL_CDEREG_MAP->ENBUFEPGL)
#define HAL_CDE_ENBUFEPG_LSB         0
#define HAL_CDE_ENBUFEPG_MSB         12
#define HAL_CDE_ENBUFEPG_MASK        0x00001FFF
#define HAL_CDE_ENBUFEPG_TYPE        UInt16
#define HAL_CDE_ENBUFEPG             HAL_CDE_ENBUFEPG_MASK
#define HAL_CDE_ENBUFEPG_READABLE    1
#define HAL_CDE_ENBUFEPG_WRITABLE    1
#define HAL_CDE_ENBUFEPG_SIGNED      0
#define HAL_CDE_ENBUFEPG_ADR         0x1000013C

// Register UNDRUNPG
#define HAL_CDE_UNDRUNPG0_REGISTER    (HAL_CDEREG_MAP->UNDRUNPG)
#define HAL_CDE_UNDRUNPG0_LSB         2
#define HAL_CDE_UNDRUNPG0_MSB         7
#define HAL_CDE_UNDRUNPG0_MASK        0x000000FC
#define HAL_CDE_UNDRUNPG0_TYPE        UByte
#define HAL_CDE_UNDRUNPG0             HAL_CDE_UNDRUNPG0_MASK
#define HAL_CDE_UNDRUNPG0_READABLE    1
#define HAL_CDE_UNDRUNPG0_WRITABLE    1
#define HAL_CDE_UNDRUNPG0_SIGNED      0
#define HAL_CDE_UNDRUNPG0_ADR         0x10000140

// Registers CDBUFPGL/CDBUFPGH
#define HAL_CDE_CDBUFPG_REGISTER    (HAL_CDEREG_MAP->CDBUFPGL)
#define HAL_CDE_CDBUFPG_LSB         0
#define HAL_CDE_CDBUFPG_MSB         12
#define HAL_CDE_CDBUFPG_MASK        0x00001FFF
#define HAL_CDE_CDBUFPG_TYPE        UInt16
#define HAL_CDE_CDBUFPG             HAL_CDE_CDBUFPG_MASK
#define HAL_CDE_CDBUFPG_READABLE    1
#define HAL_CDE_CDBUFPG_WRITABLE    1
#define HAL_CDE_CDBUFPG_SIGNED      0
#define HAL_CDE_CDBUFPG_ADR         0x10000144

#define HAL_CDE_CDPGMSB_REGISTER    (HAL_CDEREG_MAP->CDBUFPGL)
#define HAL_CDE_CDPGMSB_LSB         13
#define HAL_CDE_CDPGMSB_MSB         13
#define HAL_CDE_CDPGMSB_MASK        0x00002000
#define HAL_CDE_CDPGMSB_TYPE        Bool
#define HAL_CDE_CDPGMSB             HAL_CDE_CDPGMSB_MASK
#define HAL_CDE_CDPGMSB_READABLE    1
#define HAL_CDE_CDPGMSB_WRITABLE    1
#define HAL_CDE_CDPGMSB_SIGNED      0
#define HAL_CDE_CDPGMSB_ADR         0x10000144

// Registers SPBUFSPGL/SPBUFSPGH
#define HAL_CDE_SPBUFSPG_REGISTER    (HAL_CDEREG_MAP->SPBUFSPGL)
#define HAL_CDE_SPBUFSPG_LSB         0
#define HAL_CDE_SPBUFSPG_MSB         12
#define HAL_CDE_SPBUFSPG_MASK        0x00001FFF
#define HAL_CDE_SPBUFSPG_TYPE        UInt16
#define HAL_CDE_SPBUFSPG             HAL_CDE_SPBUFSPG_MASK
#define HAL_CDE_SPBUFSPG_READABLE    1
#define HAL_CDE_SPBUFSPG_WRITABLE    1
#define HAL_CDE_SPBUFSPG_SIGNED      0
#define HAL_CDE_SPBUFSPG_ADR         0x10000148

#define HAL_CDE_SPSPGMSB_REGISTER    (HAL_CDEREG_MAP->SPBUFSPGL)
#define HAL_CDE_SPSPGMSB_LSB         13
#define HAL_CDE_SPSPGMSB_MSB         13
#define HAL_CDE_SPSPGMSB_MASK        0x00002000
#define HAL_CDE_SPSPGMSB_TYPE        Bool
#define HAL_CDE_SPSPGMSB             HAL_CDE_SPSPGMSB_MASK
#define HAL_CDE_SPSPGMSB_READABLE    1
#define HAL_CDE_SPSPGMSB_WRITABLE    1
#define HAL_CDE_SPSPGMSB_SIGNED      0
#define HAL_CDE_SPSPGMSB_ADR         0x10000148

// Registers SPBUFEPGL/SPBUFEPGH
#define HAL_CDE_SPBUFEPG_REGISTER    (HAL_CDEREG_MAP->SPBUFEPGL)
#define HAL_CDE_SPBUFEPG_LSB         0
#define HAL_CDE_SPBUFEPG_MSB         12
#define HAL_CDE_SPBUFEPG_MASK        0x00001FFF
#define HAL_CDE_SPBUFEPG_TYPE        UInt16
#define HAL_CDE_SPBUFEPG             HAL_CDE_SPBUFEPG_MASK
#define HAL_CDE_SPBUFEPG_READABLE    1
#define HAL_CDE_SPBUFEPG_WRITABLE    1
#define HAL_CDE_SPBUFEPG_SIGNED      0
#define HAL_CDE_SPBUFEPG_ADR         0x1000014A

#define HAL_CDE_SPGOEND_REGISTER    (HAL_CDEREG_MAP->SPBUFEPGL)
#define HAL_CDE_SPGOEND_LSB         15
#define HAL_CDE_SPGOEND_MSB         15
#define HAL_CDE_SPGOEND_MASK        0x00008000
#define HAL_CDE_SPGOEND_TYPE        Bool
#define HAL_CDE_SPGOEND             HAL_CDE_SPGOEND_MASK
#define HAL_CDE_SPGOEND_READABLE    1
#define HAL_CDE_SPGOEND_WRITABLE    1
#define HAL_CDE_SPGOEND_SIGNED      0
#define HAL_CDE_SPGOEND_ADR         0x1000014A

#define HAL_CDE_ERPTEN_REGISTER    (HAL_CDEREG_MAP->SPBUFEPGL)
#define HAL_CDE_ERPTEN_LSB         14
#define HAL_CDE_ERPTEN_MSB         14
#define HAL_CDE_ERPTEN_MASK        0x00004000
#define HAL_CDE_ERPTEN_TYPE        Bool
#define HAL_CDE_ERPTEN             HAL_CDE_ERPTEN_MASK
#define HAL_CDE_ERPTEN_READABLE    1
#define HAL_CDE_ERPTEN_WRITABLE    1
#define HAL_CDE_ERPTEN_SIGNED      0
#define HAL_CDE_ERPTEN_ADR         0x1000014A

#define HAL_CDE_ERPT2ND_REGISTER    (HAL_CDEREG_MAP->SPBUFEPGL)
#define HAL_CDE_ERPT2ND_LSB         13
#define HAL_CDE_ERPT2ND_MSB         13
#define HAL_CDE_ERPT2ND_MASK        0x00002000
#define HAL_CDE_ERPT2ND_TYPE        Bool
#define HAL_CDE_ERPT2ND             HAL_CDE_ERPT2ND_MASK
#define HAL_CDE_ERPT2ND_READABLE    1
#define HAL_CDE_ERPT2ND_WRITABLE    1
#define HAL_CDE_ERPT2ND_SIGNED      0
#define HAL_CDE_ERPT2ND_ADR         0x1000014A

// Registers ENCRPTL/ENCRPTM/ENCRPTH
#define HAL_CDE_ENCRPTCNT_REGISTER    (HAL_CDEREG_MAP->ENCRPTL)
#define HAL_CDE_ENCRPTCNT_LSB         0
#define HAL_CDE_ENCRPTCNT_MSB         18
#define HAL_CDE_ENCRPTCNT_MASK        0x0007FFFF
#define HAL_CDE_ENCRPTCNT_TYPE        UInt32
#define HAL_CDE_ENCRPTCNT             HAL_CDE_ENCRPTCNT_MASK
#define HAL_CDE_ENCRPTCNT_READABLE    1
#define HAL_CDE_ENCRPTCNT_WRITABLE    1
#define HAL_CDE_ENCRPTCNT_SIGNED      0
#define HAL_CDE_ENCRPTCNT_ADR         0x1000014C

// Register SUBIFCNT0
#define HAL_CDE_P0PAD_REGISTER    (HAL_CDEREG_MAP->SUBIFCNT0)
#define HAL_CDE_P0PAD_LSB         7
#define HAL_CDE_P0PAD_MSB         7
#define HAL_CDE_P0PAD_MASK        0x00000080
#define HAL_CDE_P0PAD_TYPE        Bool
#define HAL_CDE_P0PAD             HAL_CDE_P0PAD_MASK
#define HAL_CDE_P0PAD_READABLE    1
#define HAL_CDE_P0PAD_WRITABLE    1
#define HAL_CDE_P0PAD_SIGNED      0
#define HAL_CDE_P0PAD_ADR         0x10000150

#define HAL_CDE_RW0PAD_REGISTER    (HAL_CDEREG_MAP->SUBIFCNT0)
#define HAL_CDE_RW0PAD_LSB         6
#define HAL_CDE_RW0PAD_MSB         6
#define HAL_CDE_RW0PAD_MASK        0x00000040
#define HAL_CDE_RW0PAD_TYPE        Bool
#define HAL_CDE_RW0PAD             HAL_CDE_RW0PAD_MASK
#define HAL_CDE_RW0PAD_READABLE    1
#define HAL_CDE_RW0PAD_WRITABLE    1
#define HAL_CDE_RW0PAD_SIGNED      0
#define HAL_CDE_RW0PAD_ADR         0x10000150

#define HAL_CDE_PQSRC_REGISTER    (HAL_CDEREG_MAP->SUBIFCNT0)
#define HAL_CDE_PQSRC_LSB         4
#define HAL_CDE_PQSRC_MSB         5
#define HAL_CDE_PQSRC_MASK        0x00000030
#define HAL_CDE_PQSRC_TYPE        halCDE_PQSource_t
#define HAL_CDE_PQSRC             HAL_CDE_PQSRC_MASK
#define HAL_CDE_PQSRC_READABLE    1
#define HAL_CDE_PQSRC_WRITABLE    1
#define HAL_CDE_PQSRC_SIGNED      0
#define HAL_CDE_PQSRC_ADR         0x10000150

#define HAL_CDE_CPRTCTL_REGISTER    (HAL_CDEREG_MAP->SUBIFCNT0)
#define HAL_CDE_CPRTCTL_LSB         0
#define HAL_CDE_CPRTCTL_MSB         1
#define HAL_CDE_CPRTCTL_MASK        0x00000003
#define HAL_CDE_CPRTCTL_TYPE        halCDE_CommandPackRelativeTimeCodeIncrementDecrementControl_t
#define HAL_CDE_CPRTCTL             HAL_CDE_CPRTCTL_MASK
#define HAL_CDE_CPRTCTL_READABLE    1
#define HAL_CDE_CPRTCTL_WRITABLE    1
#define HAL_CDE_CPRTCTL_SIGNED      0
#define HAL_CDE_CPRTCTL_ADR         0x10000150

// Register SQINDX
#define HAL_CDE_QREGINDEX_REGISTER    (HAL_CDEREG_MAP->SQINDX)
#define HAL_CDE_QREGINDEX_LSB         0
#define HAL_CDE_QREGINDEX_MSB         3
#define HAL_CDE_QREGINDEX_MASK        0x0000000F
#define HAL_CDE_QREGINDEX_TYPE        UByte
#define HAL_CDE_QREGINDEX             HAL_CDE_QREGINDEX_MASK
#define HAL_CDE_QREGINDEX_READABLE    1
#define HAL_CDE_QREGINDEX_WRITABLE    1
#define HAL_CDE_QREGINDEX_SIGNED      0
#define HAL_CDE_QREGINDEX_ADR         0x10000151

// Register SQCAT
#define HAL_CDE_SQCAT_REGISTER    (HAL_CDEREG_MAP->SQCAT)
#define HAL_CDE_SQCAT_LSB         0
#define HAL_CDE_SQCAT_MSB         7
#define HAL_CDE_SQCAT_MASK        0x000000FF
#define HAL_CDE_SQCAT_TYPE        UByte
#define HAL_CDE_SQCAT             HAL_CDE_SQCAT_MASK
#define HAL_CDE_SQCAT_READABLE    0
#define HAL_CDE_SQCAT_WRITABLE    1
#define HAL_CDE_SQCAT_SIGNED      0
#define HAL_CDE_SQCAT_ADR         0x10000152

// Register SQISRC
#define HAL_CDE_SQISRC_REGISTER    (HAL_CDEREG_MAP->SQISRC)
#define HAL_CDE_SQISRC_LSB         0
#define HAL_CDE_SQISRC_MSB         7
#define HAL_CDE_SQISRC_MASK        0x000000FF
#define HAL_CDE_SQISRC_TYPE        UByte
#define HAL_CDE_SQISRC             HAL_CDE_SQISRC_MASK
#define HAL_CDE_SQISRC_READABLE    0
#define HAL_CDE_SQISRC_WRITABLE    1
#define HAL_CDE_SQISRC_SIGNED      0
#define HAL_CDE_SQISRC_ADR         0x10000153

// Register SQRID
#define HAL_CDE_SQRID_REGISTER    (HAL_CDEREG_MAP->SQRID)
#define HAL_CDE_SQRID_LSB         0
#define HAL_CDE_SQRID_MSB         7
#define HAL_CDE_SQRID_MASK        0x000000FF
#define HAL_CDE_SQRID_TYPE        UByte
#define HAL_CDE_SQRID             HAL_CDE_SQRID_MASK
#define HAL_CDE_SQRID_READABLE    0
#define HAL_CDE_SQRID_WRITABLE    1
#define HAL_CDE_SQRID_SIGNED      0
#define HAL_CDE_SQRID_ADR         0x10000154

// Register SQTBD
#define HAL_CDE_SQTBD_REGISTER    (HAL_CDEREG_MAP->SQTBD)
#define HAL_CDE_SQTBD_LSB         0
#define HAL_CDE_SQTBD_MSB         7
#define HAL_CDE_SQTBD_MASK        0x000000FF
#define HAL_CDE_SQTBD_TYPE        UByte
#define HAL_CDE_SQTBD             HAL_CDE_SQTBD_MASK
#define HAL_CDE_SQTBD_READABLE    0
#define HAL_CDE_SQTBD_WRITABLE    1
#define HAL_CDE_SQTBD_SIGNED      0
#define HAL_CDE_SQTBD_ADR         0x10000155

// Register SQTC
#define HAL_CDE_SQTC_REGISTER    (HAL_CDEREG_MAP->SQTC)
#define HAL_CDE_SQTC_LSB         0
#define HAL_CDE_SQTC_MSB         7
#define HAL_CDE_SQTC_MASK        0x000000FF
#define HAL_CDE_SQTC_TYPE        UByte
#define HAL_CDE_SQTC             HAL_CDE_SQTC_MASK
#define HAL_CDE_SQTC_READABLE    0
#define HAL_CDE_SQTC_WRITABLE    1
#define HAL_CDE_SQTC_SIGNED      0
#define HAL_CDE_SQTC_ADR         0x10000156

// Register SQMODE
#define HAL_CDE_NZERO_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_NZERO_LSB         7
#define HAL_CDE_NZERO_MSB         7
#define HAL_CDE_NZERO_MASK        0x00000080
#define HAL_CDE_NZERO_TYPE        Bool
#define HAL_CDE_NZERO             HAL_CDE_NZERO_MASK
#define HAL_CDE_NZERO_READABLE    1
#define HAL_CDE_NZERO_WRITABLE    1
#define HAL_CDE_NZERO_SIGNED      0
#define HAL_CDE_NZERO_ADR         0x10000157

#define HAL_CDE_CBS_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_CBS_LSB         6
#define HAL_CDE_CBS_MSB         6
#define HAL_CDE_CBS_MASK        0x00000040
#define HAL_CDE_CBS_TYPE        Bool
#define HAL_CDE_CBS             HAL_CDE_CBS_MASK
#define HAL_CDE_CBS_READABLE    1
#define HAL_CDE_CBS_WRITABLE    1
#define HAL_CDE_CBS_SIGNED      0
#define HAL_CDE_CBS_ADR         0x10000157

#define HAL_CDE_EN0CNT_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_EN0CNT_LSB         5
#define HAL_CDE_EN0CNT_MSB         5
#define HAL_CDE_EN0CNT_MASK        0x00000020
#define HAL_CDE_EN0CNT_TYPE        Bool
#define HAL_CDE_EN0CNT             HAL_CDE_EN0CNT_MASK
#define HAL_CDE_EN0CNT_READABLE    1
#define HAL_CDE_EN0CNT_WRITABLE    1
#define HAL_CDE_EN0CNT_SIGNED      0
#define HAL_CDE_EN0CNT_ADR         0x10000157

#define HAL_CDE_PCODE_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_PCODE_LSB         4
#define HAL_CDE_PCODE_MSB         4
#define HAL_CDE_PCODE_MASK        0x00000010
#define HAL_CDE_PCODE_TYPE        Bool
#define HAL_CDE_PCODE             HAL_CDE_PCODE_MASK
#define HAL_CDE_PCODE_READABLE    1
#define HAL_CDE_PCODE_WRITABLE    1
#define HAL_CDE_PCODE_SIGNED      0
#define HAL_CDE_PCODE_ADR         0x10000157

#define HAL_CDE_AMSFD_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_AMSFD_LSB         3
#define HAL_CDE_AMSFD_MSB         3
#define HAL_CDE_AMSFD_MASK        0x00000008
#define HAL_CDE_AMSFD_TYPE        Bool
#define HAL_CDE_AMSFD             HAL_CDE_AMSFD_MASK
#define HAL_CDE_AMSFD_READABLE    1
#define HAL_CDE_AMSFD_WRITABLE    1
#define HAL_CDE_AMSFD_SIGNED      0
#define HAL_CDE_AMSFD_ADR         0x10000157

#define HAL_CDE_AMSFI_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_AMSFI_LSB         2
#define HAL_CDE_AMSFI_MSB         2
#define HAL_CDE_AMSFI_MASK        0x00000004
#define HAL_CDE_AMSFI_TYPE        Bool
#define HAL_CDE_AMSFI             HAL_CDE_AMSFI_MASK
#define HAL_CDE_AMSFI_READABLE    1
#define HAL_CDE_AMSFI_WRITABLE    1
#define HAL_CDE_AMSFI_SIGNED      0
#define HAL_CDE_AMSFI_ADR         0x10000157

#define HAL_CDE_RMSFD_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_RMSFD_LSB         1
#define HAL_CDE_RMSFD_MSB         1
#define HAL_CDE_RMSFD_MASK        0x00000002
#define HAL_CDE_RMSFD_TYPE        Bool
#define HAL_CDE_RMSFD             HAL_CDE_RMSFD_MASK
#define HAL_CDE_RMSFD_READABLE    1
#define HAL_CDE_RMSFD_WRITABLE    1
#define HAL_CDE_RMSFD_SIGNED      0
#define HAL_CDE_RMSFD_ADR         0x10000157

#define HAL_CDE_RMSFI_REGISTER    (HAL_CDEREG_MAP->SQMODE)
#define HAL_CDE_RMSFI_LSB         0
#define HAL_CDE_RMSFI_MSB         0
#define HAL_CDE_RMSFI_MASK        0x00000001
#define HAL_CDE_RMSFI_TYPE        Bool
#define HAL_CDE_RMSFI             HAL_CDE_RMSFI_MASK
#define HAL_CDE_RMSFI_READABLE    1
#define HAL_CDE_RMSFI_WRITABLE    1
#define HAL_CDE_RMSFI_SIGNED      0
#define HAL_CDE_RMSFI_ADR         0x10000157

// Register SQSEL
#define HAL_CDE_RAWCP_REGISTER    (HAL_CDEREG_MAP->SQSEL)
#define HAL_CDE_RAWCP_LSB         3
#define HAL_CDE_RAWCP_MSB         3
#define HAL_CDE_RAWCP_MASK        0x00000008
#define HAL_CDE_RAWCP_TYPE        Bool
#define HAL_CDE_RAWCP             HAL_CDE_RAWCP_MASK
#define HAL_CDE_RAWCP_READABLE    1
#define HAL_CDE_RAWCP_WRITABLE    1
#define HAL_CDE_RAWCP_SIGNED      0
#define HAL_CDE_RAWCP_ADR         0x10000158

#define HAL_CDE_AREASEL_REGISTER    (HAL_CDEREG_MAP->SQSEL)
#define HAL_CDE_AREASEL_LSB         1
#define HAL_CDE_AREASEL_MSB         2
#define HAL_CDE_AREASEL_MASK        0x00000006
#define HAL_CDE_AREASEL_TYPE        halCDE_SelectArea_t
#define HAL_CDE_AREASEL             HAL_CDE_AREASEL_MASK
#define HAL_CDE_AREASEL_READABLE    1
#define HAL_CDE_AREASEL_WRITABLE    1
#define HAL_CDE_AREASEL_SIGNED      0
#define HAL_CDE_AREASEL_ADR         0x10000158

#define HAL_CDE_SUBP_REGISTER    (HAL_CDEREG_MAP->SQSEL)
#define HAL_CDE_SUBP_LSB         0
#define HAL_CDE_SUBP_MSB         0
#define HAL_CDE_SUBP_MASK        0x00000001
#define HAL_CDE_SUBP_TYPE        Bool
#define HAL_CDE_SUBP             HAL_CDE_SUBP_MASK
#define HAL_CDE_SUBP_READABLE    1
#define HAL_CDE_SUBP_WRITABLE    1
#define HAL_CDE_SUBP_SIGNED      0
#define HAL_CDE_SUBP_ADR         0x10000158

// Register SQTRIG
#define HAL_CDE_CPPTRRST_REGISTER    (HAL_CDEREG_MAP->SQTRIG)
#define HAL_CDE_CPPTRRST_LSB         4
#define HAL_CDE_CPPTRRST_MSB         4
#define HAL_CDE_CPPTRRST_MASK        0x00000010
#define HAL_CDE_CPPTRRST_TYPE        Bool
#define HAL_CDE_CPPTRRST             HAL_CDE_CPPTRRST_MASK
#define HAL_CDE_CPPTRRST_READABLE    1
#define HAL_CDE_CPPTRRST_WRITABLE    1
#define HAL_CDE_CPPTRRST_SIGNED      0
#define HAL_CDE_CPPTRRST_ADR         0x10000159

#define HAL_CDE_TBDTRIG_REGISTER    (HAL_CDEREG_MAP->SQTRIG)
#define HAL_CDE_TBDTRIG_LSB         3
#define HAL_CDE_TBDTRIG_MSB         3
#define HAL_CDE_TBDTRIG_MASK        0x00000008
#define HAL_CDE_TBDTRIG_TYPE        Bool
#define HAL_CDE_TBDTRIG             HAL_CDE_TBDTRIG_MASK
#define HAL_CDE_TBDTRIG_READABLE    1
#define HAL_CDE_TBDTRIG_WRITABLE    1
#define HAL_CDE_TBDTRIG_SIGNED      0
#define HAL_CDE_TBDTRIG_ADR         0x10000159

#define HAL_CDE_RIDTRIG_REGISTER    (HAL_CDEREG_MAP->SQTRIG)
#define HAL_CDE_RIDTRIG_LSB         2
#define HAL_CDE_RIDTRIG_MSB         2
#define HAL_CDE_RIDTRIG_MASK        0x00000004
#define HAL_CDE_RIDTRIG_TYPE        Bool
#define HAL_CDE_RIDTRIG             HAL_CDE_RIDTRIG_MASK
#define HAL_CDE_RIDTRIG_READABLE    1
#define HAL_CDE_RIDTRIG_WRITABLE    1
#define HAL_CDE_RIDTRIG_SIGNED      0
#define HAL_CDE_RIDTRIG_ADR         0x10000159

#define HAL_CDE_ISRCTRIG_REGISTER    (HAL_CDEREG_MAP->SQTRIG)
#define HAL_CDE_ISRCTRIG_LSB         1
#define HAL_CDE_ISRCTRIG_MSB         1
#define HAL_CDE_ISRCTRIG_MASK        0x00000002
#define HAL_CDE_ISRCTRIG_TYPE        Bool
#define HAL_CDE_ISRCTRIG             HAL_CDE_ISRCTRIG_MASK
#define HAL_CDE_ISRCTRIG_READABLE    1
#define HAL_CDE_ISRCTRIG_WRITABLE    1
#define HAL_CDE_ISRCTRIG_SIGNED      0
#define HAL_CDE_ISRCTRIG_ADR         0x10000159

#define HAL_CDE_CATTRIG_REGISTER    (HAL_CDEREG_MAP->SQTRIG)
#define HAL_CDE_CATTRIG_LSB         0
#define HAL_CDE_CATTRIG_MSB         0
#define HAL_CDE_CATTRIG_MASK        0x00000001
#define HAL_CDE_CATTRIG_TYPE        Bool
#define HAL_CDE_CATTRIG             HAL_CDE_CATTRIG_MASK
#define HAL_CDE_CATTRIG_READABLE    1
#define HAL_CDE_CATTRIG_WRITABLE    1
#define HAL_CDE_CATTRIG_SIGNED      0
#define HAL_CDE_CATTRIG_ADR         0x10000159

// Register MSFCTR
#define HAL_CDE_SQM3AEN_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_SQM3AEN_LSB         7
#define HAL_CDE_SQM3AEN_MSB         7
#define HAL_CDE_SQM3AEN_MASK        0x00000080
#define HAL_CDE_SQM3AEN_TYPE        Bool
#define HAL_CDE_SQM3AEN             HAL_CDE_SQM3AEN_MASK
#define HAL_CDE_SQM3AEN_READABLE    1
#define HAL_CDE_SQM3AEN_WRITABLE    1
#define HAL_CDE_SQM3AEN_SIGNED      0
#define HAL_CDE_SQM3AEN_ADR         0x1000015A

#define HAL_CDE_SQM2AEN_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_SQM2AEN_LSB         6
#define HAL_CDE_SQM2AEN_MSB         6
#define HAL_CDE_SQM2AEN_MASK        0x00000040
#define HAL_CDE_SQM2AEN_TYPE        Bool
#define HAL_CDE_SQM2AEN             HAL_CDE_SQM2AEN_MASK
#define HAL_CDE_SQM2AEN_READABLE    1
#define HAL_CDE_SQM2AEN_WRITABLE    1
#define HAL_CDE_SQM2AEN_SIGNED      0
#define HAL_CDE_SQM2AEN_ADR         0x1000015A

#define HAL_CDE_PSRC_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_PSRC_LSB         4
#define HAL_CDE_PSRC_MSB         5
#define HAL_CDE_PSRC_MASK        0x00000030
#define HAL_CDE_PSRC_TYPE        halCDE_CmdPackPSource_t
#define HAL_CDE_PSRC             HAL_CDE_PSRC_MASK
#define HAL_CDE_PSRC_READABLE    1
#define HAL_CDE_PSRC_WRITABLE    1
#define HAL_CDE_PSRC_SIGNED      0
#define HAL_CDE_PSRC_ADR         0x1000015A

#define HAL_CDE_AMSFSEL_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_AMSFSEL_LSB         3
#define HAL_CDE_AMSFSEL_MSB         3
#define HAL_CDE_AMSFSEL_MASK        0x00000008
#define HAL_CDE_AMSFSEL_TYPE        Bool
#define HAL_CDE_AMSFSEL             HAL_CDE_AMSFSEL_MASK
#define HAL_CDE_AMSFSEL_READABLE    1
#define HAL_CDE_AMSFSEL_WRITABLE    1
#define HAL_CDE_AMSFSEL_SIGNED      0
#define HAL_CDE_AMSFSEL_ADR         0x1000015A

#define HAL_CDE_CPOSSEL_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_CPOSSEL_LSB         2
#define HAL_CDE_CPOSSEL_MSB         2
#define HAL_CDE_CPOSSEL_MASK        0x00000004
#define HAL_CDE_CPOSSEL_TYPE        Bool
#define HAL_CDE_CPOSSEL             HAL_CDE_CPOSSEL_MASK
#define HAL_CDE_CPOSSEL_READABLE    1
#define HAL_CDE_CPOSSEL_WRITABLE    1
#define HAL_CDE_CPOSSEL_SIGNED      0
#define HAL_CDE_CPOSSEL_ADR         0x1000015A

#define HAL_CDE_REPCNT3_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_REPCNT3_LSB         1
#define HAL_CDE_REPCNT3_MSB         1
#define HAL_CDE_REPCNT3_MASK        0x00000002
#define HAL_CDE_REPCNT3_TYPE        Bool
#define HAL_CDE_REPCNT3             HAL_CDE_REPCNT3_MASK
#define HAL_CDE_REPCNT3_READABLE    1
#define HAL_CDE_REPCNT3_WRITABLE    1
#define HAL_CDE_REPCNT3_SIGNED      0
#define HAL_CDE_REPCNT3_ADR         0x1000015A

#define HAL_CDE_SEQCMDP_REGISTER    (HAL_CDEREG_MAP->MSFCTR)
#define HAL_CDE_SEQCMDP_LSB         0
#define HAL_CDE_SEQCMDP_MSB         0
#define HAL_CDE_SEQCMDP_MASK        0x00000001
#define HAL_CDE_SEQCMDP_TYPE        Bool
#define HAL_CDE_SEQCMDP             HAL_CDE_SEQCMDP_MASK
#define HAL_CDE_SEQCMDP_READABLE    1
#define HAL_CDE_SEQCMDP_WRITABLE    1
#define HAL_CDE_SEQCMDP_SIGNED      0
#define HAL_CDE_SEQCMDP_ADR         0x1000015A

// Register CPACNT
#define HAL_CDE_NUMCPA_REGISTER    (HAL_CDEREG_MAP->CPACNT)
#define HAL_CDE_NUMCPA_LSB         0
#define HAL_CDE_NUMCPA_MSB         7
#define HAL_CDE_NUMCPA_MASK        0x000000FF
#define HAL_CDE_NUMCPA_TYPE        UByte
#define HAL_CDE_NUMCPA             HAL_CDE_NUMCPA_MASK
#define HAL_CDE_NUMCPA_READABLE    1
#define HAL_CDE_NUMCPA_WRITABLE    1
#define HAL_CDE_NUMCPA_SIGNED      0
#define HAL_CDE_NUMCPA_ADR         0x1000015B

// Register CPBCNT
#define HAL_CDE_SCFGIMM_REGISTER    (HAL_CDEREG_MAP->CPBCNT)
#define HAL_CDE_SCFGIMM_LSB         7
#define HAL_CDE_SCFGIMM_MSB         7
#define HAL_CDE_SCFGIMM_MASK        0x00000080
#define HAL_CDE_SCFGIMM_TYPE        Bool
#define HAL_CDE_SCFGIMM             HAL_CDE_SCFGIMM_MASK
#define HAL_CDE_SCFGIMM_READABLE    1
#define HAL_CDE_SCFGIMM_WRITABLE    1
#define HAL_CDE_SCFGIMM_SIGNED      0
#define HAL_CDE_SCFGIMM_ADR         0x1000015C

#define HAL_CDE_NUMCPB_REGISTER    (HAL_CDEREG_MAP->CPBCNT)
#define HAL_CDE_NUMCPB_LSB         0
#define HAL_CDE_NUMCPB_MSB         6
#define HAL_CDE_NUMCPB_MASK        0x0000007F
#define HAL_CDE_NUMCPB_TYPE        UByte
#define HAL_CDE_NUMCPB             HAL_CDE_NUMCPB_MASK
#define HAL_CDE_NUMCPB_READABLE    1
#define HAL_CDE_NUMCPB_WRITABLE    1
#define HAL_CDE_NUMCPB_SIGNED      0
#define HAL_CDE_NUMCPB_ADR         0x1000015C

// Register CDENCMON
#define HAL_CDE_MODE_REGISTER    (HAL_CDEREG_MAP->CDENCMON)
#define HAL_CDE_MODE_LSB         5
#define HAL_CDE_MODE_MSB         7
#define HAL_CDE_MODE_MASK        0x000000E0
#define HAL_CDE_MODE_TYPE        halCDE_CDEState_t
#define HAL_CDE_MODE             HAL_CDE_MODE_MASK
#define HAL_CDE_MODE_READABLE    1
#define HAL_CDE_MODE_WRITABLE    0
#define HAL_CDE_MODE_SIGNED      0
#define HAL_CDE_MODE_ADR         0x1000015D

#define HAL_CDE_PANIC_REGISTER    (HAL_CDEREG_MAP->CDENCMON)
#define HAL_CDE_PANIC_LSB         4
#define HAL_CDE_PANIC_MSB         4
#define HAL_CDE_PANIC_MASK        0x00000010
#define HAL_CDE_PANIC_TYPE        Bool
#define HAL_CDE_PANIC             HAL_CDE_PANIC_MASK
#define HAL_CDE_PANIC_READABLE    1
#define HAL_CDE_PANIC_WRITABLE    0
#define HAL_CDE_PANIC_SIGNED      0
#define HAL_CDE_PANIC_ADR         0x1000015D

#define HAL_CDE_TIMERR_REGISTER    (HAL_CDEREG_MAP->CDENCMON)
#define HAL_CDE_TIMERR_LSB         3
#define HAL_CDE_TIMERR_MSB         3
#define HAL_CDE_TIMERR_MASK        0x00000008
#define HAL_CDE_TIMERR_TYPE        Bool
#define HAL_CDE_TIMERR             HAL_CDE_TIMERR_MASK
#define HAL_CDE_TIMERR_READABLE    1
#define HAL_CDE_TIMERR_WRITABLE    0
#define HAL_CDE_TIMERR_SIGNED      0
#define HAL_CDE_TIMERR_ADR         0x1000015D

#define HAL_CDE_RCODERR_REGISTER    (HAL_CDEREG_MAP->CDENCMON)
#define HAL_CDE_RCODERR_LSB         2
#define HAL_CDE_RCODERR_MSB         2
#define HAL_CDE_RCODERR_MASK        0x00000004
#define HAL_CDE_RCODERR_TYPE        Bool
#define HAL_CDE_RCODERR             HAL_CDE_RCODERR_MASK
#define HAL_CDE_RCODERR_READABLE    1
#define HAL_CDE_RCODERR_WRITABLE    0
#define HAL_CDE_RCODERR_SIGNED      0
#define HAL_CDE_RCODERR_ADR         0x1000015D

#define HAL_CDE_ASYOVROV_REGISTER    (HAL_CDEREG_MAP->CDENCMON)
#define HAL_CDE_ASYOVROV_LSB         1
#define HAL_CDE_ASYOVROV_MSB         1
#define HAL_CDE_ASYOVROV_MASK        0x00000002
#define HAL_CDE_ASYOVROV_TYPE        Bool
#define HAL_CDE_ASYOVROV             HAL_CDE_ASYOVROV_MASK
#define HAL_CDE_ASYOVROV_READABLE    1
#define HAL_CDE_ASYOVROV_WRITABLE    0
#define HAL_CDE_ASYOVROV_SIGNED      0
#define HAL_CDE_ASYOVROV_ADR         0x1000015D

// Registers CPPGL/CPPGH
#define HAL_CDE_CPPG_REGISTER    (HAL_CDEREG_MAP->CPPGL)
#define HAL_CDE_CPPG_LSB         0
#define HAL_CDE_CPPG_MSB         12
#define HAL_CDE_CPPG_MASK        0x00001FFF
#define HAL_CDE_CPPG_TYPE        UInt16
#define HAL_CDE_CPPG             HAL_CDE_CPPG_MASK
#define HAL_CDE_CPPG_READABLE    1
#define HAL_CDE_CPPG_WRITABLE    1
#define HAL_CDE_CPPG_SIGNED      0
#define HAL_CDE_CPPG_ADR         0x1000015E

// Register CDENCCTL1
#define HAL_CDE_ENCMOD_REGISTER    (HAL_CDEREG_MAP->CDENCCTL1)
#define HAL_CDE_ENCMOD_LSB         7
#define HAL_CDE_ENCMOD_MSB         7
#define HAL_CDE_ENCMOD_MASK        0x00000080
#define HAL_CDE_ENCMOD_TYPE        Bool
#define HAL_CDE_ENCMOD             HAL_CDE_ENCMOD_MASK
#define HAL_CDE_ENCMOD_READABLE    1
#define HAL_CDE_ENCMOD_WRITABLE    1
#define HAL_CDE_ENCMOD_SIGNED      0
#define HAL_CDE_ENCMOD_ADR         0x10000160

// Register CDENCCTL2
#define HAL_CDE_PCAMOD_REGISTER    (HAL_CDEREG_MAP->CDENCCTL2)
#define HAL_CDE_PCAMOD_LSB         1
#define HAL_CDE_PCAMOD_MSB         1
#define HAL_CDE_PCAMOD_MASK        0x00000002
#define HAL_CDE_PCAMOD_TYPE        Bool
#define HAL_CDE_PCAMOD             HAL_CDE_PCAMOD_MASK
#define HAL_CDE_PCAMOD_READABLE    1
#define HAL_CDE_PCAMOD_WRITABLE    1
#define HAL_CDE_PCAMOD_SIGNED      0
#define HAL_CDE_PCAMOD_ADR         0x10000161

// Register CDENCCTL3
#define HAL_CDE_TIMSET_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_TIMSET_LSB         7
#define HAL_CDE_TIMSET_MSB         7
#define HAL_CDE_TIMSET_MASK        0x00000080
#define HAL_CDE_TIMSET_TYPE        Bool
#define HAL_CDE_TIMSET             HAL_CDE_TIMSET_MASK
#define HAL_CDE_TIMSET_READABLE    1
#define HAL_CDE_TIMSET_WRITABLE    1
#define HAL_CDE_TIMSET_SIGNED      0
#define HAL_CDE_TIMSET_ADR         0x10000162

#define HAL_CDE_STASTOP_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_STASTOP_LSB         4
#define HAL_CDE_STASTOP_MSB         6
#define HAL_CDE_STASTOP_MASK        0x00000070
#define HAL_CDE_STASTOP_TYPE        halCDE_StartStopCtrl_t
#define HAL_CDE_STASTOP             HAL_CDE_STASTOP_MASK
#define HAL_CDE_STASTOP_READABLE    1
#define HAL_CDE_STASTOP_WRITABLE    1
#define HAL_CDE_STASTOP_SIGNED      0
#define HAL_CDE_STASTOP_ADR         0x10000162

#define HAL_CDE_WLADDEN_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_WLADDEN_LSB         3
#define HAL_CDE_WLADDEN_MSB         3
#define HAL_CDE_WLADDEN_MASK        0x00000008
#define HAL_CDE_WLADDEN_TYPE        Bool
#define HAL_CDE_WLADDEN             HAL_CDE_WLADDEN_MASK
#define HAL_CDE_WLADDEN_READABLE    1
#define HAL_CDE_WLADDEN_WRITABLE    1
#define HAL_CDE_WLADDEN_SIGNED      0
#define HAL_CDE_WLADDEN_ADR         0x10000162

#define HAL_CDE_WLBSY_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_WLBSY_LSB         2
#define HAL_CDE_WLBSY_MSB         2
#define HAL_CDE_WLBSY_MASK        0x00000004
#define HAL_CDE_WLBSY_TYPE        Bool
#define HAL_CDE_WLBSY             HAL_CDE_WLBSY_MASK
#define HAL_CDE_WLBSY_READABLE    1
#define HAL_CDE_WLBSY_WRITABLE    0
#define HAL_CDE_WLBSY_SIGNED      0
#define HAL_CDE_WLBSY_ADR         0x10000162

#define HAL_CDE_SD25EN_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_SD25EN_LSB         1
#define HAL_CDE_SD25EN_MSB         1
#define HAL_CDE_SD25EN_MASK        0x00000002
#define HAL_CDE_SD25EN_TYPE        Bool
#define HAL_CDE_SD25EN             HAL_CDE_SD25EN_MASK
#define HAL_CDE_SD25EN_READABLE    1
#define HAL_CDE_SD25EN_WRITABLE    1
#define HAL_CDE_SD25EN_SIGNED      0
#define HAL_CDE_SD25EN_ADR         0x10000162

#define HAL_CDE_SD2EN_REGISTER    (HAL_CDEREG_MAP->CDENCCTL3)
#define HAL_CDE_SD2EN_LSB         0
#define HAL_CDE_SD2EN_MSB         0
#define HAL_CDE_SD2EN_MASK        0x00000001
#define HAL_CDE_SD2EN_TYPE        Bool
#define HAL_CDE_SD2EN             HAL_CDE_SD2EN_MASK
#define HAL_CDE_SD2EN_READABLE    1
#define HAL_CDE_SD2EN_WRITABLE    1
#define HAL_CDE_SD2EN_SIGNED      0
#define HAL_CDE_SD2EN_ADR         0x10000162

// Register CDENCCTL4
#define HAL_CDE_AUTOSTOP_REGISTER    (HAL_CDEREG_MAP->CDENCCTL4)
#define HAL_CDE_AUTOSTOP_LSB         5
#define HAL_CDE_AUTOSTOP_MSB         6
#define HAL_CDE_AUTOSTOP_MASK        0x00000060
#define HAL_CDE_AUTOSTOP_TYPE        halCDE_AutoStopCtl_t
#define HAL_CDE_AUTOSTOP             HAL_CDE_AUTOSTOP_MASK
#define HAL_CDE_AUTOSTOP_READABLE    1
#define HAL_CDE_AUTOSTOP_WRITABLE    1
#define HAL_CDE_AUTOSTOP_SIGNED      0
#define HAL_CDE_AUTOSTOP_ADR         0x10000163

#define HAL_CDE_ABATIPLOS_REGISTER    (HAL_CDEREG_MAP->CDENCCTL4)
#define HAL_CDE_ABATIPLOS_LSB         3
#define HAL_CDE_ABATIPLOS_MSB         3
#define HAL_CDE_ABATIPLOS_MASK        0x00000008
#define HAL_CDE_ABATIPLOS_TYPE        Bool
#define HAL_CDE_ABATIPLOS             HAL_CDE_ABATIPLOS_MASK
#define HAL_CDE_ABATIPLOS_READABLE    1
#define HAL_CDE_ABATIPLOS_WRITABLE    1
#define HAL_CDE_ABATIPLOS_SIGNED      0
#define HAL_CDE_ABATIPLOS_ADR         0x10000163

#define HAL_CDE_ABATIPDIS_REGISTER    (HAL_CDEREG_MAP->CDENCCTL4)
#define HAL_CDE_ABATIPDIS_LSB         1
#define HAL_CDE_ABATIPDIS_MSB         1
#define HAL_CDE_ABATIPDIS_MASK        0x00000002
#define HAL_CDE_ABATIPDIS_TYPE        Bool
#define HAL_CDE_ABATIPDIS             HAL_CDE_ABATIPDIS_MASK
#define HAL_CDE_ABATIPDIS_READABLE    1
#define HAL_CDE_ABATIPDIS_WRITABLE    1
#define HAL_CDE_ABATIPDIS_SIGNED      0
#define HAL_CDE_ABATIPDIS_ADR         0x10000163

#define HAL_CDE_ABSERVO_REGISTER    (HAL_CDEREG_MAP->CDENCCTL4)
#define HAL_CDE_ABSERVO_LSB         0
#define HAL_CDE_ABSERVO_MSB         0
#define HAL_CDE_ABSERVO_MASK        0x00000001
#define HAL_CDE_ABSERVO_TYPE        Bool
#define HAL_CDE_ABSERVO             HAL_CDE_ABSERVO_MASK
#define HAL_CDE_ABSERVO_READABLE    1
#define HAL_CDE_ABSERVO_WRITABLE    1
#define HAL_CDE_ABSERVO_SIGNED      0
#define HAL_CDE_ABSERVO_ADR         0x10000163

// Registers WRCOUNTL/WRCOUNTM/WRCOUNTH
#define HAL_CDE_WRCOUNT_REGISTER    (HAL_CDEREG_MAP->WRCOUNTL)
#define HAL_CDE_WRCOUNT_LSB         0
#define HAL_CDE_WRCOUNT_MSB         18
#define HAL_CDE_WRCOUNT_MASK        0x0007FFFF
#define HAL_CDE_WRCOUNT_TYPE        UInt32
#define HAL_CDE_WRCOUNT             HAL_CDE_WRCOUNT_MASK
#define HAL_CDE_WRCOUNT_READABLE    1
#define HAL_CDE_WRCOUNT_WRITABLE    1
#define HAL_CDE_WRCOUNT_SIGNED      0
#define HAL_CDE_WRCOUNT_ADR         0x10000164

// Register CURMIN
#define HAL_CDE_BCDCURMIN_REGISTER    (HAL_CDEREG_MAP->CURMIN)
#define HAL_CDE_BCDCURMIN_LSB         0
#define HAL_CDE_BCDCURMIN_MSB         7
#define HAL_CDE_BCDCURMIN_MASK        0x000000FF
#define HAL_CDE_BCDCURMIN_TYPE        UByte
#define HAL_CDE_BCDCURMIN             HAL_CDE_BCDCURMIN_MASK
#define HAL_CDE_BCDCURMIN_READABLE    1
#define HAL_CDE_BCDCURMIN_WRITABLE    0
#define HAL_CDE_BCDCURMIN_SIGNED      0
#define HAL_CDE_BCDCURMIN_ADR         0x10000168

// Register CURSEC
#define HAL_CDE_BCDCURSEC_REGISTER    (HAL_CDEREG_MAP->CURSEC)
#define HAL_CDE_BCDCURSEC_LSB         0
#define HAL_CDE_BCDCURSEC_MSB         7
#define HAL_CDE_BCDCURSEC_MASK        0x000000FF
#define HAL_CDE_BCDCURSEC_TYPE        UByte
#define HAL_CDE_BCDCURSEC             HAL_CDE_BCDCURSEC_MASK
#define HAL_CDE_BCDCURSEC_READABLE    1
#define HAL_CDE_BCDCURSEC_WRITABLE    0
#define HAL_CDE_BCDCURSEC_SIGNED      0
#define HAL_CDE_BCDCURSEC_ADR         0x10000169

// Register CURFRAME
#define HAL_CDE_BCDCURFRM_REGISTER    (HAL_CDEREG_MAP->CURFRAME)
#define HAL_CDE_BCDCURFRM_LSB         0
#define HAL_CDE_BCDCURFRM_MSB         7
#define HAL_CDE_BCDCURFRM_MASK        0x000000FF
#define HAL_CDE_BCDCURFRM_TYPE        UByte
#define HAL_CDE_BCDCURFRM             HAL_CDE_BCDCURFRM_MASK
#define HAL_CDE_BCDCURFRM_READABLE    1
#define HAL_CDE_BCDCURFRM_WRITABLE    0
#define HAL_CDE_BCDCURFRM_SIGNED      0
#define HAL_CDE_BCDCURFRM_ADR         0x1000016A

// Register WRMIN
#define HAL_CDE_BCDWRTMIN_REGISTER    (HAL_CDEREG_MAP->WRMIN)
#define HAL_CDE_BCDWRTMIN_LSB         0
#define HAL_CDE_BCDWRTMIN_MSB         7
#define HAL_CDE_BCDWRTMIN_MASK        0x000000FF
#define HAL_CDE_BCDWRTMIN_TYPE        UByte
#define HAL_CDE_BCDWRTMIN             HAL_CDE_BCDWRTMIN_MASK
#define HAL_CDE_BCDWRTMIN_READABLE    1
#define HAL_CDE_BCDWRTMIN_WRITABLE    1
#define HAL_CDE_BCDWRTMIN_SIGNED      0
#define HAL_CDE_BCDWRTMIN_ADR         0x1000016B

// Register WRSEC
#define HAL_CDE_BCDWRTSEC_REGISTER    (HAL_CDEREG_MAP->WRSEC)
#define HAL_CDE_BCDWRTSEC_LSB         0
#define HAL_CDE_BCDWRTSEC_MSB         7
#define HAL_CDE_BCDWRTSEC_MASK        0x000000FF
#define HAL_CDE_BCDWRTSEC_TYPE        UByte
#define HAL_CDE_BCDWRTSEC             HAL_CDE_BCDWRTSEC_MASK
#define HAL_CDE_BCDWRTSEC_READABLE    1
#define HAL_CDE_BCDWRTSEC_WRITABLE    1
#define HAL_CDE_BCDWRTSEC_SIGNED      0
#define HAL_CDE_BCDWRTSEC_ADR         0x1000016C

// Register WRFRAME
#define HAL_CDE_BCDWRTFRM_REGISTER    (HAL_CDEREG_MAP->WRFRAME)
#define HAL_CDE_BCDWRTFRM_LSB         0
#define HAL_CDE_BCDWRTFRM_MSB         7
#define HAL_CDE_BCDWRTFRM_MASK        0x000000FF
#define HAL_CDE_BCDWRTFRM_TYPE        UByte
#define HAL_CDE_BCDWRTFRM             HAL_CDE_BCDWRTFRM_MASK
#define HAL_CDE_BCDWRTFRM_READABLE    1
#define HAL_CDE_BCDWRTFRM_WRITABLE    1
#define HAL_CDE_BCDWRTFRM_SIGNED      0
#define HAL_CDE_BCDWRTFRM_ADR         0x1000016D

// Register LNKSTRT
#define HAL_CDE_LNKSTRT_REGISTER    (HAL_CDEREG_MAP->LNKSTRT)
#define HAL_CDE_LNKSTRT_LSB         0
#define HAL_CDE_LNKSTRT_MSB         6
#define HAL_CDE_LNKSTRT_MASK        0x0000007F
#define HAL_CDE_LNKSTRT_TYPE        UByte
#define HAL_CDE_LNKSTRT             HAL_CDE_LNKSTRT_MASK
#define HAL_CDE_LNKSTRT_READABLE    1
#define HAL_CDE_LNKSTRT_WRITABLE    1
#define HAL_CDE_LNKSTRT_SIGNED      0
#define HAL_CDE_LNKSTRT_ADR         0x1000016E

// Register LNKSTOP
#define HAL_CDE_LNKSTOP_REGISTER    (HAL_CDEREG_MAP->LNKSTOP)
#define HAL_CDE_LNKSTOP_LSB         0
#define HAL_CDE_LNKSTOP_MSB         6
#define HAL_CDE_LNKSTOP_MASK        0x0000007F
#define HAL_CDE_LNKSTOP_TYPE        UByte
#define HAL_CDE_LNKSTOP             HAL_CDE_LNKSTOP_MASK
#define HAL_CDE_LNKSTOP_READABLE    1
#define HAL_CDE_LNKSTOP_WRITABLE    1
#define HAL_CDE_LNKSTOP_SIGNED      0
#define HAL_CDE_LNKSTOP_ADR         0x1000016F

// Register LNKCMPBIT
#define HAL_CDE_APAUSEEN_REGISTER    (HAL_CDEREG_MAP->LNKCMPBIT)
#define HAL_CDE_APAUSEEN_LSB         7
#define HAL_CDE_APAUSEEN_MSB         7
#define HAL_CDE_APAUSEEN_MASK        0x00000080
#define HAL_CDE_APAUSEEN_TYPE        Bool
#define HAL_CDE_APAUSEEN             HAL_CDE_APAUSEEN_MASK
#define HAL_CDE_APAUSEEN_READABLE    1
#define HAL_CDE_APAUSEEN_WRITABLE    1
#define HAL_CDE_APAUSEEN_SIGNED      0
#define HAL_CDE_APAUSEEN_ADR         0x10000177

#define HAL_CDE_XLABORT_REGISTER    (HAL_CDEREG_MAP->LNKCMPBIT)
#define HAL_CDE_XLABORT_LSB         6
#define HAL_CDE_XLABORT_MSB         6
#define HAL_CDE_XLABORT_MASK        0x00000040
#define HAL_CDE_XLABORT_TYPE        Bool
#define HAL_CDE_XLABORT             HAL_CDE_XLABORT_MASK
#define HAL_CDE_XLABORT_READABLE    1
#define HAL_CDE_XLABORT_WRITABLE    1
#define HAL_CDE_XLABORT_SIGNED      0
#define HAL_CDE_XLABORT_ADR         0x10000177

#define HAL_CDE_XLEDGE_REGISTER    (HAL_CDEREG_MAP->LNKCMPBIT)
#define HAL_CDE_XLEDGE_LSB         5
#define HAL_CDE_XLEDGE_MSB         5
#define HAL_CDE_XLEDGE_MASK        0x00000020
#define HAL_CDE_XLEDGE_TYPE        Bool
#define HAL_CDE_XLEDGE             HAL_CDE_XLEDGE_MASK
#define HAL_CDE_XLEDGE_READABLE    1
#define HAL_CDE_XLEDGE_WRITABLE    1
#define HAL_CDE_XLEDGE_SIGNED      0
#define HAL_CDE_XLEDGE_ADR         0x10000177

#define HAL_CDE_LNKCMPBIT_REGISTER    (HAL_CDEREG_MAP->LNKCMPBIT)
#define HAL_CDE_LNKCMPBIT_LSB         0
#define HAL_CDE_LNKCMPBIT_MSB         4
#define HAL_CDE_LNKCMPBIT_MASK        0x0000001F
#define HAL_CDE_LNKCMPBIT_TYPE        UByte
#define HAL_CDE_LNKCMPBIT             HAL_CDE_LNKCMPBIT_MASK
#define HAL_CDE_LNKCMPBIT_READABLE    1
#define HAL_CDE_LNKCMPBIT_WRITABLE    1
#define HAL_CDE_LNKCMPBIT_SIGNED      0
#define HAL_CDE_LNKCMPBIT_ADR         0x10000177

// Register LNKCMPWD
#define HAL_CDE_LNKCMPWD_REGISTER    (HAL_CDEREG_MAP->LNKCMPWD)
#define HAL_CDE_LNKCMPWD_LSB         0
#define HAL_CDE_LNKCMPWD_MSB         5
#define HAL_CDE_LNKCMPWD_MASK        0x0000003F
#define HAL_CDE_LNKCMPWD_TYPE        UByte
#define HAL_CDE_LNKCMPWD             HAL_CDE_LNKCMPWD_MASK
#define HAL_CDE_LNKCMPWD_READABLE    1
#define HAL_CDE_LNKCMPWD_WRITABLE    1
#define HAL_CDE_LNKCMPWD_SIGNED      0
#define HAL_CDE_LNKCMPWD_ADR         0x10000178

// Register LNKCMPFRM
#define HAL_CDE_LNKCMPFRM_REGISTER    (HAL_CDEREG_MAP->LNKCMPFRM)
#define HAL_CDE_LNKCMPFRM_LSB         0
#define HAL_CDE_LNKCMPFRM_MSB         6
#define HAL_CDE_LNKCMPFRM_MASK        0x0000007F
#define HAL_CDE_LNKCMPFRM_TYPE        UByte
#define HAL_CDE_LNKCMPFRM             HAL_CDE_LNKCMPFRM_MASK
#define HAL_CDE_LNKCMPFRM_READABLE    1
#define HAL_CDE_LNKCMPFRM_WRITABLE    1
#define HAL_CDE_LNKCMPFRM_SIGNED      0
#define HAL_CDE_LNKCMPFRM_ADR         0x10000179

// Register LNKSTABIT
#define HAL_CDE_XWEFMCLR_REGISTER    (HAL_CDEREG_MAP->LNKSTABIT)
#define HAL_CDE_XWEFMCLR_LSB         6
#define HAL_CDE_XWEFMCLR_MSB         6
#define HAL_CDE_XWEFMCLR_MASK        0x00000040
#define HAL_CDE_XWEFMCLR_TYPE        Bool
#define HAL_CDE_XWEFMCLR             HAL_CDE_XWEFMCLR_MASK
#define HAL_CDE_XWEFMCLR_READABLE    1
#define HAL_CDE_XWEFMCLR_WRITABLE    1
#define HAL_CDE_XWEFMCLR_SIGNED      0
#define HAL_CDE_XWEFMCLR_ADR         0x1000017A

#define HAL_CDE_LDCTIME_REGISTER    (HAL_CDEREG_MAP->LNKSTABIT)
#define HAL_CDE_LDCTIME_LSB         5
#define HAL_CDE_LDCTIME_MSB         5
#define HAL_CDE_LDCTIME_MASK        0x00000020
#define HAL_CDE_LDCTIME_TYPE        Bool
#define HAL_CDE_LDCTIME             HAL_CDE_LDCTIME_MASK
#define HAL_CDE_LDCTIME_READABLE    1
#define HAL_CDE_LDCTIME_WRITABLE    1
#define HAL_CDE_LDCTIME_SIGNED      0
#define HAL_CDE_LDCTIME_ADR         0x1000017A

#define HAL_CDE_LNKSTABIT_REGISTER    (HAL_CDEREG_MAP->LNKSTABIT)
#define HAL_CDE_LNKSTABIT_LSB         0
#define HAL_CDE_LNKSTABIT_MSB         4
#define HAL_CDE_LNKSTABIT_MASK        0x0000001F
#define HAL_CDE_LNKSTABIT_TYPE        UByte
#define HAL_CDE_LNKSTABIT             HAL_CDE_LNKSTABIT_MASK
#define HAL_CDE_LNKSTABIT_READABLE    1
#define HAL_CDE_LNKSTABIT_WRITABLE    1
#define HAL_CDE_LNKSTABIT_SIGNED      0
#define HAL_CDE_LNKSTABIT_ADR         0x1000017A

// Register LNKSTAWD
#define HAL_CDE_XLABST_REGISTER    (HAL_CDEREG_MAP->LNKSTAWD)
#define HAL_CDE_XLABST_LSB         7
#define HAL_CDE_XLABST_MSB         7
#define HAL_CDE_XLABST_MASK        0x00000080
#define HAL_CDE_XLABST_TYPE        Bool
#define HAL_CDE_XLABST             HAL_CDE_XLABST_MASK
#define HAL_CDE_XLABST_READABLE    1
#define HAL_CDE_XLABST_WRITABLE    0
#define HAL_CDE_XLABST_SIGNED      0
#define HAL_CDE_XLABST_ADR         0x1000017B

#define HAL_CDE_LNKSTAWD_REGISTER    (HAL_CDEREG_MAP->LNKSTAWD)
#define HAL_CDE_LNKSTAWD_LSB         0
#define HAL_CDE_LNKSTAWD_MSB         5
#define HAL_CDE_LNKSTAWD_MASK        0x0000003F
#define HAL_CDE_LNKSTAWD_TYPE        UByte
#define HAL_CDE_LNKSTAWD             HAL_CDE_LNKSTAWD_MASK
#define HAL_CDE_LNKSTAWD_READABLE    1
#define HAL_CDE_LNKSTAWD_WRITABLE    1
#define HAL_CDE_LNKSTAWD_SIGNED      0
#define HAL_CDE_LNKSTAWD_ADR         0x1000017B

// Register LNKSTAFRM
#define HAL_CDE_LNKSTAFRM_REGISTER    (HAL_CDEREG_MAP->LNKSTAFRM)
#define HAL_CDE_LNKSTAFRM_LSB         0
#define HAL_CDE_LNKSTAFRM_MSB         6
#define HAL_CDE_LNKSTAFRM_MASK        0x0000007F
#define HAL_CDE_LNKSTAFRM_TYPE        UByte
#define HAL_CDE_LNKSTAFRM             HAL_CDE_LNKSTAFRM_MASK
#define HAL_CDE_LNKSTAFRM_READABLE    1
#define HAL_CDE_LNKSTAFRM_WRITABLE    1
#define HAL_CDE_LNKSTAFRM_SIGNED      0
#define HAL_CDE_LNKSTAFRM_ADR         0x1000017C

// Register LNKSTPBIT
#define HAL_CDE_LNKSTPBIT_REGISTER    (HAL_CDEREG_MAP->LNKSTPBIT)
#define HAL_CDE_LNKSTPBIT_LSB         0
#define HAL_CDE_LNKSTPBIT_MSB         4
#define HAL_CDE_LNKSTPBIT_MASK        0x0000001F
#define HAL_CDE_LNKSTPBIT_TYPE        UByte
#define HAL_CDE_LNKSTPBIT             HAL_CDE_LNKSTPBIT_MASK
#define HAL_CDE_LNKSTPBIT_READABLE    1
#define HAL_CDE_LNKSTPBIT_WRITABLE    1
#define HAL_CDE_LNKSTPBIT_SIGNED      0
#define HAL_CDE_LNKSTPBIT_ADR         0x1000017D

// Register LNKSTPWD
#define HAL_CDE_LNKSTPWD_REGISTER    (HAL_CDEREG_MAP->LNKSTPWD)
#define HAL_CDE_LNKSTPWD_LSB         0
#define HAL_CDE_LNKSTPWD_MSB         5
#define HAL_CDE_LNKSTPWD_MASK        0x0000003F
#define HAL_CDE_LNKSTPWD_TYPE        UByte
#define HAL_CDE_LNKSTPWD             HAL_CDE_LNKSTPWD_MASK
#define HAL_CDE_LNKSTPWD_READABLE    1
#define HAL_CDE_LNKSTPWD_WRITABLE    1
#define HAL_CDE_LNKSTPWD_SIGNED      0
#define HAL_CDE_LNKSTPWD_ADR         0x1000017E

// Register LNKSTPFRM
#define HAL_CDE_LNKSTPFRM_REGISTER    (HAL_CDEREG_MAP->LNKSTPFRM)
#define HAL_CDE_LNKSTPFRM_LSB         0
#define HAL_CDE_LNKSTPFRM_MSB         6
#define HAL_CDE_LNKSTPFRM_MASK        0x0000007F
#define HAL_CDE_LNKSTPFRM_TYPE        UByte
#define HAL_CDE_LNKSTPFRM             HAL_CDE_LNKSTPFRM_MASK
#define HAL_CDE_LNKSTPFRM_READABLE    1
#define HAL_CDE_LNKSTPFRM_WRITABLE    1
#define HAL_CDE_LNKSTPFRM_SIGNED      0
#define HAL_CDE_LNKSTPFRM_ADR         0x1000017F

// Register WSTCTL1
#define HAL_CDE_WSTSEL_REGISTER    (HAL_CDEREG_MAP->WSTCTL1)
#define HAL_CDE_WSTSEL_LSB         7
#define HAL_CDE_WSTSEL_MSB         7
#define HAL_CDE_WSTSEL_MASK        0x00000080
#define HAL_CDE_WSTSEL_TYPE        Bool
#define HAL_CDE_WSTSEL             HAL_CDE_WSTSEL_MASK
#define HAL_CDE_WSTSEL_READABLE    1
#define HAL_CDE_WSTSEL_WRITABLE    1
#define HAL_CDE_WSTSEL_SIGNED      0
#define HAL_CDE_WSTSEL_ADR         0x10000184

#define HAL_CDE_WSTEN_REGISTER    (HAL_CDEREG_MAP->WSTCTL1)
#define HAL_CDE_WSTEN_LSB         6
#define HAL_CDE_WSTEN_MSB         6
#define HAL_CDE_WSTEN_MASK        0x00000040
#define HAL_CDE_WSTEN_TYPE        Bool
#define HAL_CDE_WSTEN             HAL_CDE_WSTEN_MASK
#define HAL_CDE_WSTEN_READABLE    1
#define HAL_CDE_WSTEN_WRITABLE    1
#define HAL_CDE_WSTEN_SIGNED      0
#define HAL_CDE_WSTEN_ADR         0x10000184

// Register WSRPN1
#define HAL_CDE_N1REPEAT_REGISTER    (HAL_CDEREG_MAP->WSRPN1)
#define HAL_CDE_N1REPEAT_LSB         0
#define HAL_CDE_N1REPEAT_MSB         7
#define HAL_CDE_N1REPEAT_MASK        0x000000FF
#define HAL_CDE_N1REPEAT_TYPE        UByte
#define HAL_CDE_N1REPEAT             HAL_CDE_N1REPEAT_MASK
#define HAL_CDE_N1REPEAT_READABLE    1
#define HAL_CDE_N1REPEAT_WRITABLE    1
#define HAL_CDE_N1REPEAT_SIGNED      0
#define HAL_CDE_N1REPEAT_ADR         0x10000185

// Register WSRPN2
#define HAL_CDE_N2REPEAT_REGISTER    (HAL_CDEREG_MAP->WSRPN2)
#define HAL_CDE_N2REPEAT_LSB         0
#define HAL_CDE_N2REPEAT_MSB         7
#define HAL_CDE_N2REPEAT_MASK        0x000000FF
#define HAL_CDE_N2REPEAT_TYPE        UByte
#define HAL_CDE_N2REPEAT             HAL_CDE_N2REPEAT_MASK
#define HAL_CDE_N2REPEAT_READABLE    1
#define HAL_CDE_N2REPEAT_WRITABLE    1
#define HAL_CDE_N2REPEAT_SIGNED      0
#define HAL_CDE_N2REPEAT_ADR         0x10000186

// Register WSTCTL2
#define HAL_CDE_M1INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_M1INC_LSB         7
#define HAL_CDE_M1INC_MSB         7
#define HAL_CDE_M1INC_MASK        0x00000080
#define HAL_CDE_M1INC_TYPE        Bool
#define HAL_CDE_M1INC             HAL_CDE_M1INC_MASK
#define HAL_CDE_M1INC_READABLE    1
#define HAL_CDE_M1INC_WRITABLE    1
#define HAL_CDE_M1INC_SIGNED      0
#define HAL_CDE_M1INC_ADR         0x10000187

#define HAL_CDE_S1INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_S1INC_LSB         6
#define HAL_CDE_S1INC_MSB         6
#define HAL_CDE_S1INC_MASK        0x00000040
#define HAL_CDE_S1INC_TYPE        Bool
#define HAL_CDE_S1INC             HAL_CDE_S1INC_MASK
#define HAL_CDE_S1INC_READABLE    1
#define HAL_CDE_S1INC_WRITABLE    1
#define HAL_CDE_S1INC_SIGNED      0
#define HAL_CDE_S1INC_ADR         0x10000187

#define HAL_CDE_M2INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_M2INC_LSB         5
#define HAL_CDE_M2INC_MSB         5
#define HAL_CDE_M2INC_MASK        0x00000020
#define HAL_CDE_M2INC_TYPE        Bool
#define HAL_CDE_M2INC             HAL_CDE_M2INC_MASK
#define HAL_CDE_M2INC_READABLE    1
#define HAL_CDE_M2INC_WRITABLE    1
#define HAL_CDE_M2INC_SIGNED      0
#define HAL_CDE_M2INC_ADR         0x10000187

#define HAL_CDE_S2INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_S2INC_LSB         4
#define HAL_CDE_S2INC_MSB         4
#define HAL_CDE_S2INC_MASK        0x00000010
#define HAL_CDE_S2INC_TYPE        Bool
#define HAL_CDE_S2INC             HAL_CDE_S2INC_MASK
#define HAL_CDE_S2INC_READABLE    1
#define HAL_CDE_S2INC_WRITABLE    1
#define HAL_CDE_S2INC_SIGNED      0
#define HAL_CDE_S2INC_ADR         0x10000187

#define HAL_CDE_M3INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_M3INC_LSB         3
#define HAL_CDE_M3INC_MSB         3
#define HAL_CDE_M3INC_MASK        0x00000008
#define HAL_CDE_M3INC_TYPE        Bool
#define HAL_CDE_M3INC             HAL_CDE_M3INC_MASK
#define HAL_CDE_M3INC_READABLE    1
#define HAL_CDE_M3INC_WRITABLE    1
#define HAL_CDE_M3INC_SIGNED      0
#define HAL_CDE_M3INC_ADR         0x10000187

#define HAL_CDE_S3INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_S3INC_LSB         2
#define HAL_CDE_S3INC_MSB         2
#define HAL_CDE_S3INC_MASK        0x00000004
#define HAL_CDE_S3INC_TYPE        Bool
#define HAL_CDE_S3INC             HAL_CDE_S3INC_MASK
#define HAL_CDE_S3INC_READABLE    1
#define HAL_CDE_S3INC_WRITABLE    1
#define HAL_CDE_S3INC_SIGNED      0
#define HAL_CDE_S3INC_ADR         0x10000187

#define HAL_CDE_M4INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_M4INC_LSB         1
#define HAL_CDE_M4INC_MSB         1
#define HAL_CDE_M4INC_MASK        0x00000002
#define HAL_CDE_M4INC_TYPE        Bool
#define HAL_CDE_M4INC             HAL_CDE_M4INC_MASK
#define HAL_CDE_M4INC_READABLE    1
#define HAL_CDE_M4INC_WRITABLE    1
#define HAL_CDE_M4INC_SIGNED      0
#define HAL_CDE_M4INC_ADR         0x10000187

#define HAL_CDE_S4INC_REGISTER    (HAL_CDEREG_MAP->WSTCTL2)
#define HAL_CDE_S4INC_LSB         0
#define HAL_CDE_S4INC_MSB         0
#define HAL_CDE_S4INC_MASK        0x00000001
#define HAL_CDE_S4INC_TYPE        Bool
#define HAL_CDE_S4INC             HAL_CDE_S4INC_MASK
#define HAL_CDE_S4INC_READABLE    1
#define HAL_CDE_S4INC_WRITABLE    1
#define HAL_CDE_S4INC_SIGNED      0
#define HAL_CDE_S4INC_ADR         0x10000187

// Register WSTCTL3
#define HAL_CDE_M1PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL3)
#define HAL_CDE_M1PAT_LSB         4
#define HAL_CDE_M1PAT_MSB         7
#define HAL_CDE_M1PAT_MASK        0x000000F0
#define HAL_CDE_M1PAT_TYPE        UByte
#define HAL_CDE_M1PAT             HAL_CDE_M1PAT_MASK
#define HAL_CDE_M1PAT_READABLE    1
#define HAL_CDE_M1PAT_WRITABLE    1
#define HAL_CDE_M1PAT_SIGNED      0
#define HAL_CDE_M1PAT_ADR         0x10000188

#define HAL_CDE_S1PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL3)
#define HAL_CDE_S1PAT_LSB         0
#define HAL_CDE_S1PAT_MSB         3
#define HAL_CDE_S1PAT_MASK        0x0000000F
#define HAL_CDE_S1PAT_TYPE        UByte
#define HAL_CDE_S1PAT             HAL_CDE_S1PAT_MASK
#define HAL_CDE_S1PAT_READABLE    1
#define HAL_CDE_S1PAT_WRITABLE    1
#define HAL_CDE_S1PAT_SIGNED      0
#define HAL_CDE_S1PAT_ADR         0x10000188

// Register WSTCTL4
#define HAL_CDE_M2PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL4)
#define HAL_CDE_M2PAT_LSB         4
#define HAL_CDE_M2PAT_MSB         7
#define HAL_CDE_M2PAT_MASK        0x000000F0
#define HAL_CDE_M2PAT_TYPE        UByte
#define HAL_CDE_M2PAT             HAL_CDE_M2PAT_MASK
#define HAL_CDE_M2PAT_READABLE    1
#define HAL_CDE_M2PAT_WRITABLE    1
#define HAL_CDE_M2PAT_SIGNED      0
#define HAL_CDE_M2PAT_ADR         0x10000189

#define HAL_CDE_S2PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL4)
#define HAL_CDE_S2PAT_LSB         0
#define HAL_CDE_S2PAT_MSB         3
#define HAL_CDE_S2PAT_MASK        0x0000000F
#define HAL_CDE_S2PAT_TYPE        UByte
#define HAL_CDE_S2PAT             HAL_CDE_S2PAT_MASK
#define HAL_CDE_S2PAT_READABLE    1
#define HAL_CDE_S2PAT_WRITABLE    1
#define HAL_CDE_S2PAT_SIGNED      0
#define HAL_CDE_S2PAT_ADR         0x10000189

// Register WSTCTL5
#define HAL_CDE_M3PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL5)
#define HAL_CDE_M3PAT_LSB         4
#define HAL_CDE_M3PAT_MSB         7
#define HAL_CDE_M3PAT_MASK        0x000000F0
#define HAL_CDE_M3PAT_TYPE        UByte
#define HAL_CDE_M3PAT             HAL_CDE_M3PAT_MASK
#define HAL_CDE_M3PAT_READABLE    1
#define HAL_CDE_M3PAT_WRITABLE    1
#define HAL_CDE_M3PAT_SIGNED      0
#define HAL_CDE_M3PAT_ADR         0x1000018A

#define HAL_CDE_S3PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL5)
#define HAL_CDE_S3PAT_LSB         0
#define HAL_CDE_S3PAT_MSB         3
#define HAL_CDE_S3PAT_MASK        0x0000000F
#define HAL_CDE_S3PAT_TYPE        UByte
#define HAL_CDE_S3PAT             HAL_CDE_S3PAT_MASK
#define HAL_CDE_S3PAT_READABLE    1
#define HAL_CDE_S3PAT_WRITABLE    1
#define HAL_CDE_S3PAT_SIGNED      0
#define HAL_CDE_S3PAT_ADR         0x1000018A

// Register WSTCTL6
#define HAL_CDE_M4PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL6)
#define HAL_CDE_M4PAT_LSB         4
#define HAL_CDE_M4PAT_MSB         7
#define HAL_CDE_M4PAT_MASK        0x000000F0
#define HAL_CDE_M4PAT_TYPE        UByte
#define HAL_CDE_M4PAT             HAL_CDE_M4PAT_MASK
#define HAL_CDE_M4PAT_READABLE    1
#define HAL_CDE_M4PAT_WRITABLE    1
#define HAL_CDE_M4PAT_SIGNED      0
#define HAL_CDE_M4PAT_ADR         0x1000018B

#define HAL_CDE_S4PAT_REGISTER    (HAL_CDEREG_MAP->WSTCTL6)
#define HAL_CDE_S4PAT_LSB         0
#define HAL_CDE_S4PAT_MSB         3
#define HAL_CDE_S4PAT_MASK        0x0000000F
#define HAL_CDE_S4PAT_TYPE        UByte
#define HAL_CDE_S4PAT             HAL_CDE_S4PAT_MASK
#define HAL_CDE_S4PAT_READABLE    1
#define HAL_CDE_S4PAT_WRITABLE    1
#define HAL_CDE_S4PAT_SIGNED      0
#define HAL_CDE_S4PAT_ADR         0x1000018B

// Register HFSWCTL
#define HAL_CDE_HFCNT_REGISTER    (HAL_CDEREG_MAP->HFSWCTL)
#define HAL_CDE_HFCNT_LSB         7
#define HAL_CDE_HFCNT_MSB         7
#define HAL_CDE_HFCNT_MASK        0x00000080
#define HAL_CDE_HFCNT_TYPE        Bool
#define HAL_CDE_HFCNT             HAL_CDE_HFCNT_MASK
#define HAL_CDE_HFCNT_READABLE    1
#define HAL_CDE_HFCNT_WRITABLE    1
#define HAL_CDE_HFCNT_SIGNED      0
#define HAL_CDE_HFCNT_ADR         0x100001A0

#define HAL_CDE_HFNEG_REGISTER    (HAL_CDEREG_MAP->HFSWCTL)
#define HAL_CDE_HFNEG_LSB         4
#define HAL_CDE_HFNEG_MSB         6
#define HAL_CDE_HFNEG_MASK        0x00000070
#define HAL_CDE_HFNEG_TYPE        halCDE_HFSWFallEdge_t
#define HAL_CDE_HFNEG             HAL_CDE_HFNEG_MASK
#define HAL_CDE_HFNEG_READABLE    1
#define HAL_CDE_HFNEG_WRITABLE    1
#define HAL_CDE_HFNEG_SIGNED      0
#define HAL_CDE_HFNEG_ADR         0x100001A0

#define HAL_CDE_HFON_REGISTER    (HAL_CDEREG_MAP->HFSWCTL)
#define HAL_CDE_HFON_LSB         3
#define HAL_CDE_HFON_MSB         3
#define HAL_CDE_HFON_MASK        0x00000008
#define HAL_CDE_HFON_TYPE        Bool
#define HAL_CDE_HFON             HAL_CDE_HFON_MASK
#define HAL_CDE_HFON_READABLE    1
#define HAL_CDE_HFON_WRITABLE    1
#define HAL_CDE_HFON_SIGNED      0
#define HAL_CDE_HFON_ADR         0x100001A0

#if (CHIP_REV >= 0xC0)

// Register LFMODE
#define HAL_CDE_LFDOTXEN_REGISTER    (HAL_CDEREG_MAP->LFMODE)
#define HAL_CDE_LFDOTXEN_LSB         7
#define HAL_CDE_LFDOTXEN_MSB         7
#define HAL_CDE_LFDOTXEN_MASK        0x00000080
#define HAL_CDE_LFDOTXEN_TYPE        Bool
#define HAL_CDE_LFDOTXEN             HAL_CDE_LFDOTXEN_MASK
#define HAL_CDE_LFDOTXEN_READABLE    1
#define HAL_CDE_LFDOTXEN_WRITABLE    1
#define HAL_CDE_LFDOTXEN_SIGNED      0
#define HAL_CDE_LFDOTXEN_ADR         0x100001C0

#define HAL_CDE_LFENABLE_REGISTER    (HAL_CDEREG_MAP->LFMODE)
#define HAL_CDE_LFENABLE_LSB         3
#define HAL_CDE_LFENABLE_MSB         3
#define HAL_CDE_LFENABLE_MASK        0x00000008
#define HAL_CDE_LFENABLE_TYPE        Bool
#define HAL_CDE_LFENABLE             HAL_CDE_LFENABLE_MASK
#define HAL_CDE_LFENABLE_READABLE    1
#define HAL_CDE_LFENABLE_WRITABLE    1
#define HAL_CDE_LFENABLE_SIGNED      0
#define HAL_CDE_LFENABLE_ADR         0x100001C0

#define HAL_CDE_TA2IPMODE_REGISTER    (HAL_CDEREG_MAP->LFMODE)
#define HAL_CDE_TA2IPMODE_LSB         0
#define HAL_CDE_TA2IPMODE_MSB         2
#define HAL_CDE_TA2IPMODE_MASK        0x00000007
#define HAL_CDE_TA2IPMODE_TYPE        halCDE_TA2IP_t
#define HAL_CDE_TA2IPMODE             HAL_CDE_TA2IPMODE_MASK
#define HAL_CDE_TA2IPMODE_READABLE    1
#define HAL_CDE_TA2IPMODE_WRITABLE    1
#define HAL_CDE_TA2IPMODE_SIGNED      0
#define HAL_CDE_TA2IPMODE_ADR         0x100001C0

// Register LFTRGWD
#define HAL_CDE_TRGDLYWD_REGISTER    (HAL_CDEREG_MAP->LFTRGWD)
#define HAL_CDE_TRGDLYWD_LSB         0
#define HAL_CDE_TRGDLYWD_MSB         5
#define HAL_CDE_TRGDLYWD_MASK        0x0000003F
#define HAL_CDE_TRGDLYWD_TYPE        UByte
#define HAL_CDE_TRGDLYWD             HAL_CDE_TRGDLYWD_MASK
#define HAL_CDE_TRGDLYWD_READABLE    1
#define HAL_CDE_TRGDLYWD_WRITABLE    1
#define HAL_CDE_TRGDLYWD_SIGNED      0
#define HAL_CDE_TRGDLYWD_ADR         0x100001C1

// Register LFTRGBIT
#define HAL_CDE_TRGDLYBIT_REGISTER    (HAL_CDEREG_MAP->LFTRGBIT)
#define HAL_CDE_TRGDLYBIT_LSB         0
#define HAL_CDE_TRGDLYBIT_MSB         4
#define HAL_CDE_TRGDLYBIT_MASK        0x0000001F
#define HAL_CDE_TRGDLYBIT_TYPE        UByte
#define HAL_CDE_TRGDLYBIT             HAL_CDE_TRGDLYBIT_MASK
#define HAL_CDE_TRGDLYBIT_READABLE    1
#define HAL_CDE_TRGDLYBIT_WRITABLE    1
#define HAL_CDE_TRGDLYBIT_SIGNED      0
#define HAL_CDE_TRGDLYBIT_ADR         0x100001C2

// Register LFLENGTH
#define HAL_CDE_LFLENGTH_REGISTER    (HAL_CDEREG_MAP->LFLENGTH)
#define HAL_CDE_LFLENGTH_LSB         0
#define HAL_CDE_LFLENGTH_MSB         7
#define HAL_CDE_LFLENGTH_MASK        0x000000FF
#define HAL_CDE_LFLENGTH_TYPE        UByte
#define HAL_CDE_LFLENGTH             HAL_CDE_LFLENGTH_MASK
#define HAL_CDE_LFLENGTH_READABLE    1
#define HAL_CDE_LFLENGTH_WRITABLE    1
#define HAL_CDE_LFLENGTH_SIGNED      0
#define HAL_CDE_LFLENGTH_ADR         0x100001C3

// Register LFWDSEC
#define HAL_CDE_LFWDSEC_REGISTER    (HAL_CDEREG_MAP->LFWDSEC)
#define HAL_CDE_LFWDSEC_LSB         0
#define HAL_CDE_LFWDSEC_MSB         7
#define HAL_CDE_LFWDSEC_MASK        0x000000FF
#define HAL_CDE_LFWDSEC_TYPE        UByte
#define HAL_CDE_LFWDSEC             HAL_CDE_LFWDSEC_MASK
#define HAL_CDE_LFWDSEC_READABLE    1
#define HAL_CDE_LFWDSEC_WRITABLE    1
#define HAL_CDE_LFWDSEC_SIGNED      0
#define HAL_CDE_LFWDSEC_ADR         0x100001C4

// Register LFWDFRM
#define HAL_CDE_LFWDFRM_REGISTER    (HAL_CDEREG_MAP->LFWDFRM)
#define HAL_CDE_LFWDFRM_LSB         0
#define HAL_CDE_LFWDFRM_MSB         6
#define HAL_CDE_LFWDFRM_MASK        0x0000007F
#define HAL_CDE_LFWDFRM_TYPE        UByte
#define HAL_CDE_LFWDFRM             HAL_CDE_LFWDFRM_MASK
#define HAL_CDE_LFWDFRM_READABLE    1
#define HAL_CDE_LFWDFRM_WRITABLE    1
#define HAL_CDE_LFWDFRM_SIGNED      0
#define HAL_CDE_LFWDFRM_ADR         0x100001C5

// Register LFWDWD
#define HAL_CDE_LFWDWD_REGISTER    (HAL_CDEREG_MAP->LFWDWD)
#define HAL_CDE_LFWDWD_LSB         0
#define HAL_CDE_LFWDWD_MSB         5
#define HAL_CDE_LFWDWD_MASK        0x0000003F
#define HAL_CDE_LFWDWD_TYPE        UByte
#define HAL_CDE_LFWDWD             HAL_CDE_LFWDWD_MASK
#define HAL_CDE_LFWDWD_READABLE    1
#define HAL_CDE_LFWDWD_WRITABLE    1
#define HAL_CDE_LFWDWD_SIGNED      0
#define HAL_CDE_LFWDWD_ADR         0x100001C6

// Register LFTRKCNT
#define HAL_CDE_LFTRKCNT_REGISTER    (HAL_CDEREG_MAP->LFTRKCNT)
#define HAL_CDE_LFTRKCNT_LSB         0
#define HAL_CDE_LFTRKCNT_MSB         7
#define HAL_CDE_LFTRKCNT_MASK        0x000000FF
#define HAL_CDE_LFTRKCNT_TYPE        UByte
#define HAL_CDE_LFTRKCNT             HAL_CDE_LFTRKCNT_MASK
#define HAL_CDE_LFTRKCNT_READABLE    1
#define HAL_CDE_LFTRKCNT_WRITABLE    0
#define HAL_CDE_LFTRKCNT_SIGNED      0
#define HAL_CDE_LFTRKCNT_ADR         0x100001C7

// Register LFSECCNT
#define HAL_CDE_LFSECCNT_REGISTER    (HAL_CDEREG_MAP->LFSECCNT)
#define HAL_CDE_LFSECCNT_LSB         0
#define HAL_CDE_LFSECCNT_MSB         7
#define HAL_CDE_LFSECCNT_MASK        0x000000FF
#define HAL_CDE_LFSECCNT_TYPE        UByte
#define HAL_CDE_LFSECCNT             HAL_CDE_LFSECCNT_MASK
#define HAL_CDE_LFSECCNT_READABLE    1
#define HAL_CDE_LFSECCNT_WRITABLE    0
#define HAL_CDE_LFSECCNT_SIGNED      0
#define HAL_CDE_LFSECCNT_ADR         0x100001C8

// Register LFFRMCNT
#define HAL_CDE_LFFRMCNT_REGISTER    (HAL_CDEREG_MAP->LFFRMCNT)
#define HAL_CDE_LFFRMCNT_LSB         0
#define HAL_CDE_LFFRMCNT_MSB         6
#define HAL_CDE_LFFRMCNT_MASK        0x0000007F
#define HAL_CDE_LFFRMCNT_TYPE        UByte
#define HAL_CDE_LFFRMCNT             HAL_CDE_LFFRMCNT_MASK
#define HAL_CDE_LFFRMCNT_READABLE    1
#define HAL_CDE_LFFRMCNT_WRITABLE    0
#define HAL_CDE_LFFRMCNT_SIGNED      0
#define HAL_CDE_LFFRMCNT_ADR         0x100001C9

// Register LFWDCNT
#define HAL_CDE_LFWDCNT_REGISTER    (HAL_CDEREG_MAP->LFWDCNT)
#define HAL_CDE_LFWDCNT_LSB         0
#define HAL_CDE_LFWDCNT_MSB         5
#define HAL_CDE_LFWDCNT_MASK        0x0000003F
#define HAL_CDE_LFWDCNT_TYPE        UByte
#define HAL_CDE_LFWDCNT             HAL_CDE_LFWDCNT_MASK
#define HAL_CDE_LFWDCNT_READABLE    1
#define HAL_CDE_LFWDCNT_WRITABLE    0
#define HAL_CDE_LFWDCNT_SIGNED      0
#define HAL_CDE_LFWDCNT_ADR         0x100001CA

// Register LFBITCNT
#define HAL_CDE_LFBITCNT_REGISTER    (HAL_CDEREG_MAP->LFBITCNT)
#define HAL_CDE_LFBITCNT_LSB         0
#define HAL_CDE_LFBITCNT_MSB         4
#define HAL_CDE_LFBITCNT_MASK        0x0000001F
#define HAL_CDE_LFBITCNT_TYPE        UByte
#define HAL_CDE_LFBITCNT             HAL_CDE_LFBITCNT_MASK
#define HAL_CDE_LFBITCNT_READABLE    1
#define HAL_CDE_LFBITCNT_WRITABLE    0
#define HAL_CDE_LFBITCNT_SIGNED      0
#define HAL_CDE_LFBITCNT_ADR         0x100001CB

// Register LFINT
#define HAL_CDE_LFBASICANG_REGISTER    (HAL_CDEREG_MAP->LFINT)
#define HAL_CDE_LFBASICANG_LSB         0
#define HAL_CDE_LFBASICANG_MSB         0
#define HAL_CDE_LFBASICANG_MASK        0x00000001
#define HAL_CDE_LFBASICANG_TYPE        Bool
#define HAL_CDE_LFBASICANG             HAL_CDE_LFBASICANG_MASK
#define HAL_CDE_LFBASICANG_READABLE    1
#define HAL_CDE_LFBASICANG_WRITABLE    1
#define HAL_CDE_LFBASICANG_SIGNED      0
#define HAL_CDE_LFBASICANG_ADR         0x100001CC

#define HAL_CDE_LFBASICERR_REGISTER    (HAL_CDEREG_MAP->LFINT)
#define HAL_CDE_LFBASICERR_LSB         1
#define HAL_CDE_LFBASICERR_MSB         1
#define HAL_CDE_LFBASICERR_MASK        0x00000002
#define HAL_CDE_LFBASICERR_TYPE        Bool
#define HAL_CDE_LFBASICERR             HAL_CDE_LFBASICERR_MASK
#define HAL_CDE_LFBASICERR_READABLE    1
#define HAL_CDE_LFBASICERR_WRITABLE    1
#define HAL_CDE_LFBASICERR_SIGNED      0
#define HAL_CDE_LFBASICERR_ADR         0x100001CC

// Register LFINTEN
#define HAL_CDE_LFBASICANGEN_REGISTER    (HAL_CDEREG_MAP->LFINTEN)
#define HAL_CDE_LFBASICANGEN_LSB         0
#define HAL_CDE_LFBASICANGEN_MSB         0
#define HAL_CDE_LFBASICANGEN_MASK        0x00000001
#define HAL_CDE_LFBASICANGEN_TYPE        Bool
#define HAL_CDE_LFBASICANGEN             HAL_CDE_LFBASICANGEN_MASK
#define HAL_CDE_LFBASICANGEN_READABLE    1
#define HAL_CDE_LFBASICANGEN_WRITABLE    1
#define HAL_CDE_LFBASICANGEN_SIGNED      0
#define HAL_CDE_LFBASICANGEN_ADR         0x100001CD

#define HAL_CDE_LFBASICERREN_REGISTER    (HAL_CDEREG_MAP->LFINTEN)
#define HAL_CDE_LFBASICERREN_LSB         1
#define HAL_CDE_LFBASICERREN_MSB         1
#define HAL_CDE_LFBASICERREN_MASK        0x00000002
#define HAL_CDE_LFBASICERREN_TYPE        Bool
#define HAL_CDE_LFBASICERREN             HAL_CDE_LFBASICERREN_MASK
#define HAL_CDE_LFBASICERREN_READABLE    1
#define HAL_CDE_LFBASICERREN_WRITABLE    1
#define HAL_CDE_LFBASICERREN_SIGNED      0
#define HAL_CDE_LFBASICERREN_ADR         0x100001CD

// Register LNKSCORFRM
#define HAL_CDE_LNKSCORFRM_REGISTER    (HAL_CDEREG_MAP->LNKSCORFRM)
#define HAL_CDE_LNKSCORFRM_LSB         0
#define HAL_CDE_LNKSCORFRM_MSB         7
#define HAL_CDE_LNKSCORFRM_MASK        0x000000FF
#define HAL_CDE_LNKSCORFRM_TYPE        UByte
#define HAL_CDE_LNKSCORFRM             HAL_CDE_LNKSCORFRM_MASK
#define HAL_CDE_LNKSCORFRM_READABLE    1
#define HAL_CDE_LNKSCORFRM_WRITABLE    0
#define HAL_CDE_LNKSCORFRM_SIGNED      0
#define HAL_CDE_LNKSCORFRM_ADR         0x100001E0

// Register LNKSCORWD
#define HAL_CDE_LNKSCORWD_REGISTER    (HAL_CDEREG_MAP->LNKSCORWD)
#define HAL_CDE_LNKSCORWD_LSB         0
#define HAL_CDE_LNKSCORWD_MSB         5
#define HAL_CDE_LNKSCORWD_MASK        0x0000003F
#define HAL_CDE_LNKSCORWD_TYPE        UByte
#define HAL_CDE_LNKSCORWD             HAL_CDE_LNKSCORWD_MASK
#define HAL_CDE_LNKSCORWD_READABLE    1
#define HAL_CDE_LNKSCORWD_WRITABLE    0
#define HAL_CDE_LNKSCORWD_SIGNED      0
#define HAL_CDE_LNKSCORWD_ADR         0x100001E1

// Register LNKSCORBIT
#define HAL_CDE_LNKSCORBIT_REGISTER    (HAL_CDEREG_MAP->LNKSCORBIT)
#define HAL_CDE_LNKSCORBIT_LSB         0
#define HAL_CDE_LNKSCORBIT_MSB         4
#define HAL_CDE_LNKSCORBIT_MASK        0x0000001F
#define HAL_CDE_LNKSCORBIT_TYPE        UByte
#define HAL_CDE_LNKSCORBIT             HAL_CDE_LNKSCORBIT_MASK
#define HAL_CDE_LNKSCORBIT_READABLE    1
#define HAL_CDE_LNKSCORBIT_WRITABLE    0
#define HAL_CDE_LNKSCORBIT_SIGNED      0
#define HAL_CDE_LNKSCORBIT_ADR         0x100001E2

#endif // #if (CHIP_REV >= 0xC0)

// Register SPSTPFRM
#define HAL_CDE_STPFRMPOS_REGISTER    (HAL_CDEREG_MAP->SPSTPFRM)
#define HAL_CDE_STPFRMPOS_LSB         0
#define HAL_CDE_STPFRMPOS_MSB         6
#define HAL_CDE_STPFRMPOS_MASK        0x0000007F
#define HAL_CDE_STPFRMPOS_TYPE        UByte
#define HAL_CDE_STPFRMPOS             HAL_CDE_STPFRMPOS_MASK
#define HAL_CDE_STPFRMPOS_READABLE    1
#define HAL_CDE_STPFRMPOS_WRITABLE    0
#define HAL_CDE_STPFRMPOS_SIGNED      0
#define HAL_CDE_STPFRMPOS_ADR         0x100001F2

// Register SPSTPWRD
#define HAL_CDE_STPWRDPOS_REGISTER    (HAL_CDEREG_MAP->SPSTPWRD)
#define HAL_CDE_STPWRDPOS_LSB         0
#define HAL_CDE_STPWRDPOS_MSB         5
#define HAL_CDE_STPWRDPOS_MASK        0x0000003F
#define HAL_CDE_STPWRDPOS_TYPE        UByte
#define HAL_CDE_STPWRDPOS             HAL_CDE_STPWRDPOS_MASK
#define HAL_CDE_STPWRDPOS_READABLE    1
#define HAL_CDE_STPWRDPOS_WRITABLE    0
#define HAL_CDE_STPWRDPOS_SIGNED      0
#define HAL_CDE_STPWRDPOS_ADR         0x100001F3

// Register SPSTPBIT
#define HAL_CDE_STPBITPOS_REGISTER    (HAL_CDEREG_MAP->SPSTPBIT)
#define HAL_CDE_STPBITPOS_LSB         0
#define HAL_CDE_STPBITPOS_MSB         4
#define HAL_CDE_STPBITPOS_MASK        0x0000001F
#define HAL_CDE_STPBITPOS_TYPE        UByte
#define HAL_CDE_STPBITPOS             HAL_CDE_STPBITPOS_MASK
#define HAL_CDE_STPBITPOS_READABLE    1
#define HAL_CDE_STPBITPOS_WRITABLE    0
#define HAL_CDE_STPBITPOS_SIGNED      0
#define HAL_CDE_STPBITPOS_ADR         0x100001F4

// Register SHPCTRL
#define HAL_CDE_ELSTAT_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_ELSTAT_LSB         7
#define HAL_CDE_ELSTAT_MSB         7
#define HAL_CDE_ELSTAT_MASK        0x00000080
#define HAL_CDE_ELSTAT_TYPE        Bool
#define HAL_CDE_ELSTAT             HAL_CDE_ELSTAT_MASK
#define HAL_CDE_ELSTAT_READABLE    1
#define HAL_CDE_ELSTAT_WRITABLE    0
#define HAL_CDE_ELSTAT_SIGNED      0
#define HAL_CDE_ELSTAT_ADR         0x100001F7

#define HAL_CDE_XLCASE2_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_XLCASE2_LSB         6
#define HAL_CDE_XLCASE2_MSB         6
#define HAL_CDE_XLCASE2_MASK        0x00000040
#define HAL_CDE_XLCASE2_TYPE        Bool
#define HAL_CDE_XLCASE2             HAL_CDE_XLCASE2_MASK
#define HAL_CDE_XLCASE2_READABLE    1
#define HAL_CDE_XLCASE2_WRITABLE    0
#define HAL_CDE_XLCASE2_SIGNED      0
#define HAL_CDE_XLCASE2_ADR         0x100001F7

#define HAL_CDE_BRKTIME_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_BRKTIME_LSB         5
#define HAL_CDE_BRKTIME_MSB         5
#define HAL_CDE_BRKTIME_MASK        0x00000020
#define HAL_CDE_BRKTIME_TYPE        Bool
#define HAL_CDE_BRKTIME             HAL_CDE_BRKTIME_MASK
#define HAL_CDE_BRKTIME_READABLE    1
#define HAL_CDE_BRKTIME_WRITABLE    0
#define HAL_CDE_BRKTIME_SIGNED      0
#define HAL_CDE_BRKTIME_ADR         0x100001F7

#define HAL_CDE_XLRECODE_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_XLRECODE_LSB         4
#define HAL_CDE_XLRECODE_MSB         4
#define HAL_CDE_XLRECODE_MASK        0x00000010
#define HAL_CDE_XLRECODE_TYPE        Bool
#define HAL_CDE_XLRECODE             HAL_CDE_XLRECODE_MASK
#define HAL_CDE_XLRECODE_READABLE    1
#define HAL_CDE_XLRECODE_WRITABLE    0
#define HAL_CDE_XLRECODE_SIGNED      0
#define HAL_CDE_XLRECODE_ADR         0x100001F7

#define HAL_CDE_XLABSTAT_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_XLABSTAT_LSB         3
#define HAL_CDE_XLABSTAT_MSB         3
#define HAL_CDE_XLABSTAT_MASK        0x00000008
#define HAL_CDE_XLABSTAT_TYPE        Bool
#define HAL_CDE_XLABSTAT             HAL_CDE_XLABSTAT_MASK
#define HAL_CDE_XLABSTAT_READABLE    1
#define HAL_CDE_XLABSTAT_WRITABLE    0
#define HAL_CDE_XLABSTAT_SIGNED      0
#define HAL_CDE_XLABSTAT_ADR         0x100001F7

#define HAL_CDE_XWASYNC_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_XWASYNC_LSB         2
#define HAL_CDE_XWASYNC_MSB         2
#define HAL_CDE_XWASYNC_MASK        0x00000004
#define HAL_CDE_XWASYNC_TYPE        Bool
#define HAL_CDE_XWASYNC             HAL_CDE_XWASYNC_MASK
#define HAL_CDE_XWASYNC_READABLE    1
#define HAL_CDE_XWASYNC_WRITABLE    1
#define HAL_CDE_XWASYNC_SIGNED      0
#define HAL_CDE_XWASYNC_ADR         0x100001F7

#define HAL_CDE_FXLCASE2_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_FXLCASE2_LSB         1
#define HAL_CDE_FXLCASE2_MSB         1
#define HAL_CDE_FXLCASE2_MASK        0x00000002
#define HAL_CDE_FXLCASE2_TYPE        Bool
#define HAL_CDE_FXLCASE2             HAL_CDE_FXLCASE2_MASK
#define HAL_CDE_FXLCASE2_READABLE    1
#define HAL_CDE_FXLCASE2_WRITABLE    1
#define HAL_CDE_FXLCASE2_SIGNED      0
#define HAL_CDE_FXLCASE2_ADR         0x100001F7

#define HAL_CDE_XLAINIT_REGISTER    (HAL_CDEREG_MAP->SHPCTRL)
#define HAL_CDE_XLAINIT_LSB         0
#define HAL_CDE_XLAINIT_MSB         0
#define HAL_CDE_XLAINIT_MASK        0x00000001
#define HAL_CDE_XLAINIT_TYPE        Bool
#define HAL_CDE_XLAINIT             HAL_CDE_XLAINIT_MASK
#define HAL_CDE_XLAINIT_READABLE    1
#define HAL_CDE_XLAINIT_WRITABLE    1
#define HAL_CDE_XLAINIT_SIGNED      0
#define HAL_CDE_XLAINIT_ADR         0x100001F7

// Register ROCTRL
#define HAL_CDE_ENFIXPKT_REGISTER    (HAL_CDEREG_MAP->ROCTRL)
#define HAL_CDE_ENFIXPKT_LSB         1
#define HAL_CDE_ENFIXPKT_MSB         1
#define HAL_CDE_ENFIXPKT_MASK        0x00000002
#define HAL_CDE_ENFIXPKT_TYPE        Bool
#define HAL_CDE_ENFIXPKT             HAL_CDE_ENFIXPKT_MASK
#define HAL_CDE_ENFIXPKT_READABLE    1
#define HAL_CDE_ENFIXPKT_WRITABLE    1
#define HAL_CDE_ENFIXPKT_SIGNED      0
#define HAL_CDE_ENFIXPKT_ADR         0x100001F9

#define HAL_CDE_ENAUTRO_REGISTER    (HAL_CDEREG_MAP->ROCTRL)
#define HAL_CDE_ENAUTRO_LSB         0
#define HAL_CDE_ENAUTRO_MSB         0
#define HAL_CDE_ENAUTRO_MASK        0x00000001
#define HAL_CDE_ENAUTRO_TYPE        Bool
#define HAL_CDE_ENAUTRO             HAL_CDE_ENAUTRO_MASK
#define HAL_CDE_ENAUTRO_READABLE    1
#define HAL_CDE_ENAUTRO_WRITABLE    1
#define HAL_CDE_ENAUTRO_SIGNED      0
#define HAL_CDE_ENAUTRO_ADR         0x100001F9

// Registers ROUTPGL/ROUTPGH
#define HAL_CDE_RUNOPG_REGISTER    (HAL_CDEREG_MAP->ROUTPGL)
#define HAL_CDE_RUNOPG_LSB         0
#define HAL_CDE_RUNOPG_MSB         12
#define HAL_CDE_RUNOPG_MASK        0x00001FFF
#define HAL_CDE_RUNOPG_TYPE        UInt16
#define HAL_CDE_RUNOPG             HAL_CDE_RUNOPG_MASK
#define HAL_CDE_RUNOPG_READABLE    1
#define HAL_CDE_RUNOPG_WRITABLE    1
#define HAL_CDE_RUNOPG_SIGNED      0
#define HAL_CDE_RUNOPG_ADR         0x100001FA

// Registers NEXTPGL/NEXTPGH
#define HAL_CDE_NEXTPG_REGISTER    (HAL_CDEREG_MAP->NEXTPGL)
#define HAL_CDE_NEXTPG_LSB         0
#define HAL_CDE_NEXTPG_MSB         12
#define HAL_CDE_NEXTPG_MASK        0x00001FFF
#define HAL_CDE_NEXTPG_TYPE        UInt16
#define HAL_CDE_NEXTPG             HAL_CDE_NEXTPG_MASK
#define HAL_CDE_NEXTPG_READABLE    1
#define HAL_CDE_NEXTPG_WRITABLE    1
#define HAL_CDE_NEXTPG_SIGNED      0
#define HAL_CDE_NEXTPG_ADR         0x100001FC

#define HAL_CDE_NEXTPGMSB_REGISTER    (HAL_CDEREG_MAP->NEXTPGL)
#define HAL_CDE_NEXTPGMSB_LSB         13
#define HAL_CDE_NEXTPGMSB_MSB         13
#define HAL_CDE_NEXTPGMSB_MASK        0x00002000
#define HAL_CDE_NEXTPGMSB_TYPE        Bool
#define HAL_CDE_NEXTPGMSB             HAL_CDE_NEXTPGMSB_MASK
#define HAL_CDE_NEXTPGMSB_READABLE    1
#define HAL_CDE_NEXTPGMSB_WRITABLE    1
#define HAL_CDE_NEXTPGMSB_SIGNED      0
#define HAL_CDE_NEXTPGMSB_ADR         0x100001FC

// Register PKTSZE
#define HAL_CDE_PKTSZE_REGISTER    (HAL_CDEREG_MAP->PKTSZE)
#define HAL_CDE_PKTSZE_LSB         0
#define HAL_CDE_PKTSZE_MSB         7
#define HAL_CDE_PKTSZE_MASK        0x000000FF
#define HAL_CDE_PKTSZE_TYPE        UByte
#define HAL_CDE_PKTSZE             HAL_CDE_PKTSZE_MASK
#define HAL_CDE_PKTSZE_READABLE    1
#define HAL_CDE_PKTSZE_WRITABLE    1
#define HAL_CDE_PKTSZE_SIGNED      0
#define HAL_CDE_PKTSZE_ADR         0x100001FE

// Register CDESPARE
// Enumerations

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halCDE_efmproc_dbg_bus   = 1,
	halCDE_lf_dbg_bus        = 2,
	halCDE_subproc_dbg_bus   = 3,
	halCDE_secproc_dbg_bus   = 4,
	halCDE_bvci_dbg_bus      = 5,
	halCDE_bvcibus_dbg_bus_R = 6,
	halCDE_bvcibus_dbg_bus_W = 7 
} halCDE_CDEDebugBusSelect_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halCDE_bManuMode_DA   = 0,    // Mode: CD-DA
	halCDE_bManuMode_M0   = 2,    // Mode: Mode 0 / Mode 2(Yellow Book, not XA or CDI)
	halCDE_bManuMode_M1   = 4,    // Mode: Mode 1
	halCDE_bManuMode_M2F1 = 6,    // Mode: Mode 2 Form 1
	halCDE_bManuMode_M2F2 = 7     // Mode: Mode 2 Form 2
} halCDE_ManualModeSelect_t;

typedef enum
{
	halCDE_bAutoMode_MAN = 0,    // manual
	halCDE_bAutoMode_RSV = 1,    // reserved
	halCDE_bAutoMode_M   = 2,    // auto detect mode
	halCDE_bAutoMode_MF  = 3     // auto detect mode/form
} halCDE_AutoModeSelect_t;

typedef enum
{
	halCDE_bEncSMode_DA      = 0,    // CDDA
	halCDE_bEncSMode_Lnk     = 1,    // Run-In Blk
	halCDE_bEncSMode_UsrData = 2,    // Data Blk
	halCDE_bEncSMode_RunOut  = 3     // Run-Out Blk
} halCDE_SectorProcessorEncodeModeSelect_t;

typedef enum
{
	halCDE_bSP_Idle   = 0,    // scetor processor is idle
	halCDE_bSP_Enc    = 1,    // sector processor is performing encode operations
	halCDE_bSP_PreEnc = 2     // firmware starts sector process, but sector processor has not started yet
} halCDE_SectorProcessorStatus_t;

typedef enum
{
	halCDE_bRT_Idle    = 0,    // idle
	halCDE_bRT_Idle_1  = 1,    // idle, same as "bRT_Idle"
	halCDE_bRT_CDDA    = 2,    // encoding CD-DA sector
	halCDE_bRT_Lnk     = 3,    // encoding link block
	halCDE_bRT_1RunIn  = 4,    // encoding 1st runin block
	halCDE_bRT_2RunIn  = 5,    // encoding 2nd runin block
	halCDE_bRT_3RunIn  = 6,    // encoding 3rd runin block
	halCDE_bRT_4RunIn  = 7,    // encoding 4th runin block
	halCDE_bRT_Data    = 8,    // encoding data block
	halCDE_bRT_1RunOut = 9,    // encoding 1st runout block
	halCDE_bRT_2RunOut = 10     // encoding 2nd runout block
} halCDE_RealTimeEncodeStatus_t;

typedef enum
{
	halCDE_PQSource_Raw = 0,    // Subcode P/Q from host
	halCDE_PQSource_Reg = 1,    // Subcode P/Q from P/Q generator registers
	halCDE_PQSource_Cmd = 2,    // Subcode P/Q from command pack registers
	halCDE_PQSource_Buf = 3     // Subcode P/Q from auxiliary page in memory
} halCDE_PQSource_t;

typedef enum
{
	halCDE_CPRTDec  = 0,    // Relative Time Code decremented at next block
	halCDE_CPRTInc  = 1,    // Relative Time Code incremented at next block
	halCDE_CPRTZero = 2     // Relative Time Code 00:00:00 (2nd ZERO) at next block
} halCDE_CommandPackRelativeTimeCodeIncrementDecrementControl_t;

typedef enum
{
	halCDE_Program_Area = 0,
	halCDE_OPCPMA_Area  = 1,
	halCDE_LeadIn_Area  = 2,
	halCDE_LeadOut_Area = 3 
} halCDE_SelectArea_t;

typedef enum
{
	halCDE_P_Default = 0,    // Encode default P data in current command pack
	halCDE_P_2Hz     = 1,    // Encode 2-Hz pattern in P-channel
	halCDE_P_Zero    = 2,    // Encode value 0 in P-channel
	halCDE_P_One     = 3     // Encode value 1 in P-channel
} halCDE_CmdPackPSource_t;

typedef enum
{
	halCDE_State_Idle        = 0,    // State Idle
	halCDE_State_Prepare     = 1,    // State Preparation
	halCDE_State_Write       = 2,    // State Write (Not PCA)
	halCDE_State_WritePCA    = 3,    // State Write (PCA)
	halCDE_State_WriteSearch = 4     // State Write Pause (Search)
} halCDE_CDEState_t;

typedef enum
{
	halCDE_ManualBreak       = 1,    // Manual Break Mode
	halCDE_ManualStop        = 2,    // Manual Stop Mode
	halCDE_ManualStart       = 3,    // Manual Start Mode
	halCDE_AutoStart         = 4,    // Auto Start Mode
	halCDE_ExacWrPause       = 5,    // ExacWrite Pause Mode
	halCDE_ExacWrAutoRestart = 6     // ExacWrite Auto-Restart Mode
} halCDE_StartStopCtrl_t;

typedef enum
{
	halCDE_Disable       = 0,    // Auto Stop is disabled
	halCDE_CDPgEqualSPPg = 1,    // Auto Stop on CDBUFPG = SPBUFPG
	halCDE_WRCOUNTZero   = 2     // Auto Stop on WRCOUNT = 0
} halCDE_AutoStopCtl_t;

typedef enum
{
	halCDE_AfterESFS_16EFM = 0,    // HFSW Falling Edge 16 EFM Frames after ESFS
	halCDE_AfterESFS_32EFM = 1,    // HFSW Falling Edge 32 EFM Frames after ESFS
	halCDE_AfterESFS_48EFM = 2,    // HFSW Falling Edge 48 EFM Frames after ESFS
	halCDE_AfterESFS_64EFM = 3,    // HFSW Falling Edge 64 EFM Frames after ESFS
	halCDE_AfterESFS_80EFM = 4,    // HFSW Falling Edge 80 EFM Frames after ESFS
	halCDE_AfterESFS_96EFM = 5     // HFSW Falling Edge 96 EFM Frames after ESFS
} halCDE_HFSWFallEdge_t;

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halCDE_TA2IPMODE0 = 0,    // Disable Positive
	halCDE_TA2IPMODE1 = 1,    // Uneven Positive
	halCDE_TA2IPMODE2 = 2,    // Even Positive
	halCDE_TA2IPMODE3 = 3,    // Subcode Positive
	halCDE_TA2IPMODE4 = 4,    // Disable Negative
	halCDE_TA2IPMODE5 = 5,    // Uneven Negative
	halCDE_TA2IPMODE6 = 6,    // Even Negative
	halCDE_TA2IPMODE7 = 7     // Subcode Negative
} halCDE_TA2IP_t;

#endif // #if (CHIP_REV >= 0xC0)

#endif /* __REG_CD_ENCODER_H__ */

