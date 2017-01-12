/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_cd_decoder.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the CDD core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_CD_DECODER_H__
#define __REG_CD_DECODER_H__

// Core register map
#define HAL_CDD_VERSION 7.33
#define HAL_CDD_BaseAddress 0x10000000
#define HAL_CDDREG_MAP ((struct halCDDReg_Map*) (void *) HAL_CDD_BaseAddress)
__packed struct halCDDReg_Map
{
	HAL_Reg8  ReservedBlk0[4];                //00000000
	HAL_Reg8  DSPSTA;                         //00000004
	HAL_Reg8  ECCCTL;                         //00000005
	HAL_Reg8  ECCCTL2;                        //00000006
	HAL_Reg8  ReservedBlk1[3];                //00000007
	HAL_Reg8  DSITEST;                        //0000000A
	HAL_Reg8  ReservedBlk2[3];                //0000000B
	HAL_Reg8  CONCTL;                         //0000000E
	HAL_Reg8  ReservedBlk3[1];                //0000000F
	HAL_Reg16 C1EPSCRL;                       //00000010
	HAL_Reg16 C1EPSTRL;                       //00000012
	HAL_Reg16 C2EPSCRL;                       //00000014
	HAL_Reg16 C2EPSTRL;                       //00000016
	HAL_Reg8  C1CNTR;                         //00000018
	HAL_Reg8  C1THRD;                         //00000019
	HAL_Reg8  C2CNTR;                         //0000001A
	HAL_Reg8  C2THRD;                         //0000001B
	HAL_Reg8  C12CCR;                         //0000001C
	HAL_Reg8  ReservedBlk4[2];                //0000001D
	HAL_Reg8  CDDSPARE;                       //0000001F
	HAL_Reg8  INT1STAT;                       //00000020
	HAL_Reg8  INT1EN;                         //00000021
	HAL_Reg8  ReservedBlk5[1];                //00000022
	HAL_Reg8  SCORCTL;                        //00000023
	HAL_Reg8  ReservedBlk6[4];                //00000024
	HAL_Reg8  CDDSPDLY;                       //00000028
	HAL_Reg8  ReservedBlk7[1];                //00000029
	HAL_Reg16 SCVALDLYL;                      //0000002A
	HAL_Reg8  ReservedBlk8[20];               //0000002C
	HAL_Reg8  ADVC;                           //00000040
	HAL_Reg8  C2PAR;                          //00000041
	HAL_Reg8  ReservedBlk9[1];                //00000042
	HAL_Reg8  BUFCTL;                         //00000043
	HAL_Reg8  ReservedBlk10[10];              //00000044
	HAL_Reg16 TWBLKL;                         //0000004E
	HAL_Reg8  STATS;                          //00000050
	HAL_Reg8  SUBCD;                          //00000051
	HAL_Reg8  SUBRWC;                         //00000052
	HAL_Reg8  SUBQC;                          //00000053
	HAL_Reg8  SCSTAT;                         //00000054
	HAL_Reg8  LFCTRL;                         //00000055
	HAL_Reg8  ReservedBlk11[26];              //00000056
	HAL_Reg8  CTRLAM0;                        //00000070
	HAL_Reg8  CTRLAM1;                        //00000071
	HAL_Reg8  ReservedBlk12[1];               //00000072
	HAL_Reg8  CTRL1;                          //00000073
	HAL_Reg8  THEAD0;                         //00000074
	HAL_Reg8  THEAD1;                         //00000075
	HAL_Reg8  THEAD2;                         //00000076
	HAL_Reg8  ReservedBlk13[1];               //00000077
	HAL_Reg8  EDACCFG;                        //00000078
	HAL_Reg8  C2THLD;                         //00000079
	HAL_Reg8  DPRDPG;                         //0000007A
	HAL_Reg8  ReservedBlk14[1];               //0000007B
	HAL_Reg8  CTRLEDC0;                       //0000007C
	HAL_Reg8  CTRLEDC1;                       //0000007D
	HAL_Reg8  CTRLECC;                        //0000007E
	HAL_Reg8  TESTECC;                        //0000007F
	HAL_Reg8  STAT0;                          //00000080
	HAL_Reg8  STAT1;                          //00000081
	HAL_Reg8  STAT2;                          //00000082
	HAL_Reg8  STAT3;                          //00000083
	HAL_Reg8  HEAD0;                          //00000084
	HAL_Reg8  HEAD1;                          //00000085
	HAL_Reg8  HEAD2;                          //00000086
	HAL_Reg8  HEAD3;                          //00000087
	HAL_Reg8  LSUBH2;                         //00000088
	HAL_Reg8  ReservedBlk15[1];               //00000089
	HAL_Reg8  STATAM0;                        //0000008A
	HAL_Reg8  ReservedBlk16[37];              //0000008B
	HAL_Reg8  DINT0;                          //000000B0
	HAL_Reg8  DINT1;                          //000000B1
	HAL_Reg8  DINT2;                          //000000B2
	HAL_Reg8  ReservedBlk17[1];               //000000B3
	HAL_Reg8  DINTEN0;                        //000000B4
	HAL_Reg8  DINTEN1;                        //000000B5
	HAL_Reg8  DINTEN2;                        //000000B6
	HAL_Reg8  ReservedBlk18[7];               //000000B7
	HAL_Reg8  TSTWRCE;                        //000000BE
	HAL_Reg8  ReservedBlk19[1];               //000000BF
	HAL_Reg8  SUBQ0;                          //000000C0
	HAL_Reg8  SUBQ1;                          //000000C1
	HAL_Reg8  SUBQ2;                          //000000C2
	HAL_Reg8  SUBQ3;                          //000000C3
	HAL_Reg8  SUBQ4;                          //000000C4
	HAL_Reg8  SUBQ5;                          //000000C5
	HAL_Reg8  SUBQ6;                          //000000C6
	HAL_Reg8  SUBQ7;                          //000000C7
	HAL_Reg8  SUBQ8;                          //000000C8
	HAL_Reg8  SUBQ9;                          //000000C9
	HAL_Reg8  SQSTAT;                         //000000CA
	HAL_Reg8  ReservedBlk20[9];               //000000CB
	HAL_Reg16 DSKBUFSPGL;                     //000000D4
	HAL_Reg16 DSKBUFEPGL;                     //000000D6
	HAL_Reg8  ReservedBlk21[12];              //000000D8
	HAL_Reg16 DSUBPGL;                        //000000E4
	HAL_Reg16 DECPGL;                         //000000E6
	HAL_Reg16 LDECPGL;                        //000000E8
	HAL_Reg8  ReservedBlk22[1];               //000000EA
	HAL_Reg8  MEMCF;                          //000000EB
	HAL_Reg8  ReservedBlk23[14];              //000000EC
	HAL_Reg8  CDDDBGBUS;                      //000000FA
	HAL_Reg8  ReservedBlk24[1];               //000000FB
	HAL_Reg16 DCPGOFL;                        //000000FC
	HAL_Reg16 PTL;                            //000000FE
};

// Register DSPSTA
#define HAL_CDD_MUTEC_REGISTER    (HAL_CDDREG_MAP->DSPSTA)
#define HAL_CDD_MUTEC_LSB         5
#define HAL_CDD_MUTEC_MSB         5
#define HAL_CDD_MUTEC_MASK        0x00000020
#define HAL_CDD_MUTEC_TYPE        Bool
#define HAL_CDD_MUTEC             HAL_CDD_MUTEC_MASK
#define HAL_CDD_MUTEC_READABLE    1
#define HAL_CDD_MUTEC_WRITABLE    0
#define HAL_CDD_MUTEC_SIGNED      0
#define HAL_CDD_MUTEC_ADR         0x10000004

// Register ECCCTL
#define HAL_CDD_C2CG2_REGISTER    (HAL_CDDREG_MAP->ECCCTL)
#define HAL_CDD_C2CG2_LSB         5
#define HAL_CDD_C2CG2_MSB         5
#define HAL_CDD_C2CG2_MASK        0x00000020
#define HAL_CDD_C2CG2_TYPE        Bool
#define HAL_CDD_C2CG2             HAL_CDD_C2CG2_MASK
#define HAL_CDD_C2CG2_READABLE    1
#define HAL_CDD_C2CG2_WRITABLE    1
#define HAL_CDD_C2CG2_SIGNED      0
#define HAL_CDD_C2CG2_ADR         0x10000005

#define HAL_CDD_C2CG_REGISTER    (HAL_CDDREG_MAP->ECCCTL)
#define HAL_CDD_C2CG_LSB         3
#define HAL_CDD_C2CG_MSB         4
#define HAL_CDD_C2CG_MASK        0x00000018
#define HAL_CDD_C2CG_TYPE        halCDD_C2ErrCorrect_t
#define HAL_CDD_C2CG             HAL_CDD_C2CG_MASK
#define HAL_CDD_C2CG_READABLE    1
#define HAL_CDD_C2CG_WRITABLE    1
#define HAL_CDD_C2CG_SIGNED      0
#define HAL_CDD_C2CG_ADR         0x10000005

#define HAL_CDD_C1CG1_REGISTER    (HAL_CDDREG_MAP->ECCCTL)
#define HAL_CDD_C1CG1_LSB         2
#define HAL_CDD_C1CG1_MSB         2
#define HAL_CDD_C1CG1_MASK        0x00000004
#define HAL_CDD_C1CG1_TYPE        Bool
#define HAL_CDD_C1CG1             HAL_CDD_C1CG1_MASK
#define HAL_CDD_C1CG1_READABLE    1
#define HAL_CDD_C1CG1_WRITABLE    1
#define HAL_CDD_C1CG1_SIGNED      0
#define HAL_CDD_C1CG1_ADR         0x10000005

#define HAL_CDD_C1CG0_REGISTER    (HAL_CDDREG_MAP->ECCCTL)
#define HAL_CDD_C1CG0_LSB         1
#define HAL_CDD_C1CG0_MSB         1
#define HAL_CDD_C1CG0_MASK        0x00000002
#define HAL_CDD_C1CG0_TYPE        Bool
#define HAL_CDD_C1CG0             HAL_CDD_C1CG0_MASK
#define HAL_CDD_C1CG0_READABLE    1
#define HAL_CDD_C1CG0_WRITABLE    1
#define HAL_CDD_C1CG0_SIGNED      0
#define HAL_CDD_C1CG0_ADR         0x10000005

#define HAL_CDD_C1C2EN_REGISTER    (HAL_CDDREG_MAP->ECCCTL)
#define HAL_CDD_C1C2EN_LSB         0
#define HAL_CDD_C1C2EN_MSB         0
#define HAL_CDD_C1C2EN_MASK        0x00000001
#define HAL_CDD_C1C2EN_TYPE        Bool
#define HAL_CDD_C1C2EN             HAL_CDD_C1C2EN_MASK
#define HAL_CDD_C1C2EN_READABLE    1
#define HAL_CDD_C1C2EN_WRITABLE    1
#define HAL_CDD_C1C2EN_SIGNED      0
#define HAL_CDD_C1C2EN_ADR         0x10000005

// Register ECCCTL2

#if (CHIP_REV >= 0xC0)

#define HAL_CDD_NOC2RTCHK_REGISTER    (HAL_CDDREG_MAP->ECCCTL2)
#define HAL_CDD_NOC2RTCHK_LSB         4
#define HAL_CDD_NOC2RTCHK_MSB         4
#define HAL_CDD_NOC2RTCHK_MASK        0x00000010
#define HAL_CDD_NOC2RTCHK_TYPE        Bool
#define HAL_CDD_NOC2RTCHK             HAL_CDD_NOC2RTCHK_MASK
#define HAL_CDD_NOC2RTCHK_READABLE    1
#define HAL_CDD_NOC2RTCHK_WRITABLE    1
#define HAL_CDD_NOC2RTCHK_SIGNED      0
#define HAL_CDD_NOC2RTCHK_ADR         0x10000006

#define HAL_CDD_DECOC2ERR_REGISTER    (HAL_CDDREG_MAP->ECCCTL2)
#define HAL_CDD_DECOC2ERR_LSB         3
#define HAL_CDD_DECOC2ERR_MSB         3
#define HAL_CDD_DECOC2ERR_MASK        0x00000008
#define HAL_CDD_DECOC2ERR_TYPE        Bool
#define HAL_CDD_DECOC2ERR             HAL_CDD_DECOC2ERR_MASK
#define HAL_CDD_DECOC2ERR_READABLE    1
#define HAL_CDD_DECOC2ERR_WRITABLE    1
#define HAL_CDD_DECOC2ERR_SIGNED      0
#define HAL_CDD_DECOC2ERR_ADR         0x10000006

#define HAL_CDD_C2NOC1FLG_REGISTER    (HAL_CDDREG_MAP->ECCCTL2)
#define HAL_CDD_C2NOC1FLG_LSB         1
#define HAL_CDD_C2NOC1FLG_MSB         1
#define HAL_CDD_C2NOC1FLG_MASK        0x00000002
#define HAL_CDD_C2NOC1FLG_TYPE        Bool
#define HAL_CDD_C2NOC1FLG             HAL_CDD_C2NOC1FLG_MASK
#define HAL_CDD_C2NOC1FLG_READABLE    1
#define HAL_CDD_C2NOC1FLG_WRITABLE    1
#define HAL_CDD_C2NOC1FLG_SIGNED      0
#define HAL_CDD_C2NOC1FLG_ADR         0x10000006

#define HAL_CDD_HOLDC2ERA_REGISTER    (HAL_CDDREG_MAP->ECCCTL2)
#define HAL_CDD_HOLDC2ERA_LSB         0
#define HAL_CDD_HOLDC2ERA_MSB         0
#define HAL_CDD_HOLDC2ERA_MASK        0x00000001
#define HAL_CDD_HOLDC2ERA_TYPE        Bool
#define HAL_CDD_HOLDC2ERA             HAL_CDD_HOLDC2ERA_MASK
#define HAL_CDD_HOLDC2ERA_READABLE    1
#define HAL_CDD_HOLDC2ERA_WRITABLE    1
#define HAL_CDD_HOLDC2ERA_SIGNED      0
#define HAL_CDD_HOLDC2ERA_ADR         0x10000006

#endif // #if (CHIP_REV >= 0xC0)

// Register DSITEST
#define HAL_CDD_DSITSEL_REGISTER    (HAL_CDDREG_MAP->DSITEST)
#define HAL_CDD_DSITSEL_LSB         6
#define HAL_CDD_DSITSEL_MSB         6
#define HAL_CDD_DSITSEL_MASK        0x00000040
#define HAL_CDD_DSITSEL_TYPE        Bool
#define HAL_CDD_DSITSEL             HAL_CDD_DSITSEL_MASK
#define HAL_CDD_DSITSEL_READABLE    1
#define HAL_CDD_DSITSEL_WRITABLE    1
#define HAL_CDD_DSITSEL_SIGNED      0
#define HAL_CDD_DSITSEL_ADR         0x1000000A

#define HAL_CDD_DSISRCH_REGISTER    (HAL_CDDREG_MAP->DSITEST)
#define HAL_CDD_DSISRCH_LSB         1
#define HAL_CDD_DSISRCH_MSB         1
#define HAL_CDD_DSISRCH_MASK        0x00000002
#define HAL_CDD_DSISRCH_TYPE        Bool
#define HAL_CDD_DSISRCH             HAL_CDD_DSISRCH_MASK
#define HAL_CDD_DSISRCH_READABLE    1
#define HAL_CDD_DSISRCH_WRITABLE    1
#define HAL_CDD_DSISRCH_SIGNED      0
#define HAL_CDD_DSISRCH_ADR         0x1000000A

// Register CONCTL
#define HAL_CDD_MUTEOFF_REGISTER    (HAL_CDDREG_MAP->CONCTL)
#define HAL_CDD_MUTEOFF_LSB         6
#define HAL_CDD_MUTEOFF_MSB         6
#define HAL_CDD_MUTEOFF_MASK        0x00000040
#define HAL_CDD_MUTEOFF_TYPE        Bool
#define HAL_CDD_MUTEOFF             HAL_CDD_MUTEOFF_MASK
#define HAL_CDD_MUTEOFF_READABLE    1
#define HAL_CDD_MUTEOFF_WRITABLE    1
#define HAL_CDD_MUTEOFF_SIGNED      0
#define HAL_CDD_MUTEOFF_ADR         0x1000000E

#define HAL_CDD_MUTM_REGISTER    (HAL_CDDREG_MAP->CONCTL)
#define HAL_CDD_MUTM_LSB         4
#define HAL_CDD_MUTM_MSB         5
#define HAL_CDD_MUTM_MASK        0x00000030
#define HAL_CDD_MUTM_TYPE        halCDD_MuteCtrl_t
#define HAL_CDD_MUTM             HAL_CDD_MUTM_MASK
#define HAL_CDD_MUTM_READABLE    1
#define HAL_CDD_MUTM_WRITABLE    1
#define HAL_CDD_MUTM_SIGNED      0
#define HAL_CDD_MUTM_ADR         0x1000000E

#define HAL_CDD_ITH1_REGISTER    (HAL_CDDREG_MAP->CONCTL)
#define HAL_CDD_ITH1_LSB         3
#define HAL_CDD_ITH1_MSB         3
#define HAL_CDD_ITH1_MASK        0x00000008
#define HAL_CDD_ITH1_TYPE        Bool
#define HAL_CDD_ITH1             HAL_CDD_ITH1_MASK
#define HAL_CDD_ITH1_READABLE    1
#define HAL_CDD_ITH1_WRITABLE    1
#define HAL_CDD_ITH1_SIGNED      0
#define HAL_CDD_ITH1_ADR         0x1000000E

// Registers C1EPSCRL/C1EPSCRH
#define HAL_CDD_C1EPSCNT_REGISTER    (HAL_CDDREG_MAP->C1EPSCRL)
#define HAL_CDD_C1EPSCNT_LSB         0
#define HAL_CDD_C1EPSCNT_MSB         12
#define HAL_CDD_C1EPSCNT_MASK        0x00001FFF
#define HAL_CDD_C1EPSCNT_TYPE        UInt16
#define HAL_CDD_C1EPSCNT             HAL_CDD_C1EPSCNT_MASK
#define HAL_CDD_C1EPSCNT_READABLE    1
#define HAL_CDD_C1EPSCNT_WRITABLE    0
#define HAL_CDD_C1EPSCNT_SIGNED      0
#define HAL_CDD_C1EPSCNT_ADR         0x10000010

// Registers C1EPSTRL/C1EPSTRH
#define HAL_CDD_C1EPSTHRD_REGISTER    (HAL_CDDREG_MAP->C1EPSTRL)
#define HAL_CDD_C1EPSTHRD_LSB         0
#define HAL_CDD_C1EPSTHRD_MSB         12
#define HAL_CDD_C1EPSTHRD_MASK        0x00001FFF
#define HAL_CDD_C1EPSTHRD_TYPE        UInt16
#define HAL_CDD_C1EPSTHRD             HAL_CDD_C1EPSTHRD_MASK
#define HAL_CDD_C1EPSTHRD_READABLE    1
#define HAL_CDD_C1EPSTHRD_WRITABLE    1
#define HAL_CDD_C1EPSTHRD_SIGNED      0
#define HAL_CDD_C1EPSTHRD_ADR         0x10000012

// Registers C2EPSCRL/C2EPSCRH
#define HAL_CDD_C2EPSCNT_REGISTER    (HAL_CDDREG_MAP->C2EPSCRL)
#define HAL_CDD_C2EPSCNT_LSB         0
#define HAL_CDD_C2EPSCNT_MSB         12
#define HAL_CDD_C2EPSCNT_MASK        0x00001FFF
#define HAL_CDD_C2EPSCNT_TYPE        UInt16
#define HAL_CDD_C2EPSCNT             HAL_CDD_C2EPSCNT_MASK
#define HAL_CDD_C2EPSCNT_READABLE    1
#define HAL_CDD_C2EPSCNT_WRITABLE    0
#define HAL_CDD_C2EPSCNT_SIGNED      0
#define HAL_CDD_C2EPSCNT_ADR         0x10000014

// Registers C2EPSTRL/C2EPSTRH
#define HAL_CDD_C2EPSTHRD_REGISTER    (HAL_CDDREG_MAP->C2EPSTRL)
#define HAL_CDD_C2EPSTHRD_LSB         0
#define HAL_CDD_C2EPSTHRD_MSB         12
#define HAL_CDD_C2EPSTHRD_MASK        0x00001FFF
#define HAL_CDD_C2EPSTHRD_TYPE        UInt16
#define HAL_CDD_C2EPSTHRD             HAL_CDD_C2EPSTHRD_MASK
#define HAL_CDD_C2EPSTHRD_READABLE    1
#define HAL_CDD_C2EPSTHRD_WRITABLE    1
#define HAL_CDD_C2EPSTHRD_SIGNED      0
#define HAL_CDD_C2EPSTHRD_ADR         0x10000016

// Register C1CNTR
#define HAL_CDD_C1CNT_REGISTER    (HAL_CDDREG_MAP->C1CNTR)
#define HAL_CDD_C1CNT_LSB         0
#define HAL_CDD_C1CNT_MSB         6
#define HAL_CDD_C1CNT_MASK        0x0000007F
#define HAL_CDD_C1CNT_TYPE        UByte
#define HAL_CDD_C1CNT             HAL_CDD_C1CNT_MASK
#define HAL_CDD_C1CNT_READABLE    1
#define HAL_CDD_C1CNT_WRITABLE    0
#define HAL_CDD_C1CNT_SIGNED      0
#define HAL_CDD_C1CNT_ADR         0x10000018

// Register C1THRD
#define HAL_CDD_C1THRD_REGISTER    (HAL_CDDREG_MAP->C1THRD)
#define HAL_CDD_C1THRD_LSB         0
#define HAL_CDD_C1THRD_MSB         6
#define HAL_CDD_C1THRD_MASK        0x0000007F
#define HAL_CDD_C1THRD_TYPE        UByte
#define HAL_CDD_C1THRD             HAL_CDD_C1THRD_MASK
#define HAL_CDD_C1THRD_READABLE    1
#define HAL_CDD_C1THRD_WRITABLE    1
#define HAL_CDD_C1THRD_SIGNED      0
#define HAL_CDD_C1THRD_ADR         0x10000019

// Register C2CNTR
#define HAL_CDD_C2CNT_REGISTER    (HAL_CDDREG_MAP->C2CNTR)
#define HAL_CDD_C2CNT_LSB         0
#define HAL_CDD_C2CNT_MSB         6
#define HAL_CDD_C2CNT_MASK        0x0000007F
#define HAL_CDD_C2CNT_TYPE        UByte
#define HAL_CDD_C2CNT             HAL_CDD_C2CNT_MASK
#define HAL_CDD_C2CNT_READABLE    1
#define HAL_CDD_C2CNT_WRITABLE    0
#define HAL_CDD_C2CNT_SIGNED      0
#define HAL_CDD_C2CNT_ADR         0x1000001A

// Register C2THRD
#define HAL_CDD_C2THRD_REGISTER    (HAL_CDDREG_MAP->C2THRD)
#define HAL_CDD_C2THRD_LSB         0
#define HAL_CDD_C2THRD_MSB         6
#define HAL_CDD_C2THRD_MASK        0x0000007F
#define HAL_CDD_C2THRD_TYPE        UByte
#define HAL_CDD_C2THRD             HAL_CDD_C2THRD_MASK
#define HAL_CDD_C2THRD_READABLE    1
#define HAL_CDD_C2THRD_WRITABLE    1
#define HAL_CDD_C2THRD_SIGNED      0
#define HAL_CDD_C2THRD_ADR         0x1000001B

// Register C12CCR
#define HAL_CDD_SYNINTEN_REGISTER    (HAL_CDDREG_MAP->C12CCR)
#define HAL_CDD_SYNINTEN_LSB         3
#define HAL_CDD_SYNINTEN_MSB         3
#define HAL_CDD_SYNINTEN_MASK        0x00000008
#define HAL_CDD_SYNINTEN_TYPE        Bool
#define HAL_CDD_SYNINTEN             HAL_CDD_SYNINTEN_MASK
#define HAL_CDD_SYNINTEN_READABLE    1
#define HAL_CDD_SYNINTEN_WRITABLE    1
#define HAL_CDD_SYNINTEN_SIGNED      0
#define HAL_CDD_SYNINTEN_ADR         0x1000001C

#define HAL_CDD_C2UNC_REGISTER    (HAL_CDDREG_MAP->C12CCR)
#define HAL_CDD_C2UNC_LSB         2
#define HAL_CDD_C2UNC_MSB         2
#define HAL_CDD_C2UNC_MASK        0x00000004
#define HAL_CDD_C2UNC_TYPE        Bool
#define HAL_CDD_C2UNC             HAL_CDD_C2UNC_MASK
#define HAL_CDD_C2UNC_READABLE    1
#define HAL_CDD_C2UNC_WRITABLE    1
#define HAL_CDD_C2UNC_SIGNED      0
#define HAL_CDD_C2UNC_ADR         0x1000001C

#define HAL_CDD_C1UNC_REGISTER    (HAL_CDDREG_MAP->C12CCR)
#define HAL_CDD_C1UNC_LSB         1
#define HAL_CDD_C1UNC_MSB         1
#define HAL_CDD_C1UNC_MASK        0x00000002
#define HAL_CDD_C1UNC_TYPE        Bool
#define HAL_CDD_C1UNC             HAL_CDD_C1UNC_MASK
#define HAL_CDD_C1UNC_READABLE    1
#define HAL_CDD_C1UNC_WRITABLE    1
#define HAL_CDD_C1UNC_SIGNED      0
#define HAL_CDD_C1UNC_ADR         0x1000001C

// Register CDDSPARE

#if (CHIP_REV <= 0xB3)

#define HAL_CDD_NOC2RTCHK_REGISTER    (HAL_CDDREG_MAP->CDDSPARE)
#define HAL_CDD_NOC2RTCHK_LSB         4
#define HAL_CDD_NOC2RTCHK_MSB         4
#define HAL_CDD_NOC2RTCHK_MASK        0x00000010
#define HAL_CDD_NOC2RTCHK_TYPE        Bool
#define HAL_CDD_NOC2RTCHK             HAL_CDD_NOC2RTCHK_MASK
#define HAL_CDD_NOC2RTCHK_READABLE    1
#define HAL_CDD_NOC2RTCHK_WRITABLE    1
#define HAL_CDD_NOC2RTCHK_SIGNED      0
#define HAL_CDD_NOC2RTCHK_ADR         0x1000001F

#define HAL_CDD_DECOC2ERR_REGISTER    (HAL_CDDREG_MAP->CDDSPARE)
#define HAL_CDD_DECOC2ERR_LSB         3
#define HAL_CDD_DECOC2ERR_MSB         3
#define HAL_CDD_DECOC2ERR_MASK        0x00000008
#define HAL_CDD_DECOC2ERR_TYPE        Bool
#define HAL_CDD_DECOC2ERR             HAL_CDD_DECOC2ERR_MASK
#define HAL_CDD_DECOC2ERR_READABLE    1
#define HAL_CDD_DECOC2ERR_WRITABLE    1
#define HAL_CDD_DECOC2ERR_SIGNED      0
#define HAL_CDD_DECOC2ERR_ADR         0x1000001F

#define HAL_CDD_C2NOC1FLG_REGISTER    (HAL_CDDREG_MAP->CDDSPARE)
#define HAL_CDD_C2NOC1FLG_LSB         1
#define HAL_CDD_C2NOC1FLG_MSB         1
#define HAL_CDD_C2NOC1FLG_MASK        0x00000002
#define HAL_CDD_C2NOC1FLG_TYPE        Bool
#define HAL_CDD_C2NOC1FLG             HAL_CDD_C2NOC1FLG_MASK
#define HAL_CDD_C2NOC1FLG_READABLE    1
#define HAL_CDD_C2NOC1FLG_WRITABLE    1
#define HAL_CDD_C2NOC1FLG_SIGNED      0
#define HAL_CDD_C2NOC1FLG_ADR         0x1000001F

#define HAL_CDD_HOLDC2ERA_REGISTER    (HAL_CDDREG_MAP->CDDSPARE)
#define HAL_CDD_HOLDC2ERA_LSB         0
#define HAL_CDD_HOLDC2ERA_MSB         0
#define HAL_CDD_HOLDC2ERA_MASK        0x00000001
#define HAL_CDD_HOLDC2ERA_TYPE        Bool
#define HAL_CDD_HOLDC2ERA             HAL_CDD_HOLDC2ERA_MASK
#define HAL_CDD_HOLDC2ERA_READABLE    1
#define HAL_CDD_HOLDC2ERA_WRITABLE    1
#define HAL_CDD_HOLDC2ERA_SIGNED      0
#define HAL_CDD_HOLDC2ERA_ADR         0x1000001F

#endif // #if (CHIP_REV <= 0xB3)

// Register INT1STAT
#define HAL_CDD_SCOR_REGISTER    (HAL_CDDREG_MAP->INT1STAT)
#define HAL_CDD_SCOR_LSB         6
#define HAL_CDD_SCOR_MSB         6
#define HAL_CDD_SCOR_MASK        0x00000040
#define HAL_CDD_SCOR_TYPE        Bool
#define HAL_CDD_SCOR             HAL_CDD_SCOR_MASK
#define HAL_CDD_SCOR_READABLE    1
#define HAL_CDD_SCOR_WRITABLE    1
#define HAL_CDD_SCOR_SIGNED      0
#define HAL_CDD_SCOR_ADR         0x10000020

#define HAL_CDD_C1ERR_REGISTER    (HAL_CDDREG_MAP->INT1STAT)
#define HAL_CDD_C1ERR_LSB         2
#define HAL_CDD_C1ERR_MSB         2
#define HAL_CDD_C1ERR_MASK        0x00000004
#define HAL_CDD_C1ERR_TYPE        Bool
#define HAL_CDD_C1ERR             HAL_CDD_C1ERR_MASK
#define HAL_CDD_C1ERR_READABLE    1
#define HAL_CDD_C1ERR_WRITABLE    1
#define HAL_CDD_C1ERR_SIGNED      0
#define HAL_CDD_C1ERR_ADR         0x10000020

#define HAL_CDD_C2ERR_REGISTER    (HAL_CDDREG_MAP->INT1STAT)
#define HAL_CDD_C2ERR_LSB         1
#define HAL_CDD_C2ERR_MSB         1
#define HAL_CDD_C2ERR_MASK        0x00000002
#define HAL_CDD_C2ERR_TYPE        Bool
#define HAL_CDD_C2ERR             HAL_CDD_C2ERR_MASK
#define HAL_CDD_C2ERR_READABLE    1
#define HAL_CDD_C2ERR_WRITABLE    1
#define HAL_CDD_C2ERR_SIGNED      0
#define HAL_CDD_C2ERR_ADR         0x10000020

#define HAL_CDD_CEPSERR_REGISTER    (HAL_CDDREG_MAP->INT1STAT)
#define HAL_CDD_CEPSERR_LSB         0
#define HAL_CDD_CEPSERR_MSB         0
#define HAL_CDD_CEPSERR_MASK        0x00000001
#define HAL_CDD_CEPSERR_TYPE        Bool
#define HAL_CDD_CEPSERR             HAL_CDD_CEPSERR_MASK
#define HAL_CDD_CEPSERR_READABLE    1
#define HAL_CDD_CEPSERR_WRITABLE    1
#define HAL_CDD_CEPSERR_SIGNED      0
#define HAL_CDD_CEPSERR_ADR         0x10000020

// Register INT1EN
#define HAL_CDD_SCOREN_REGISTER    (HAL_CDDREG_MAP->INT1EN)
#define HAL_CDD_SCOREN_LSB         6
#define HAL_CDD_SCOREN_MSB         6
#define HAL_CDD_SCOREN_MASK        0x00000040
#define HAL_CDD_SCOREN_TYPE        Bool
#define HAL_CDD_SCOREN             HAL_CDD_SCOREN_MASK
#define HAL_CDD_SCOREN_READABLE    1
#define HAL_CDD_SCOREN_WRITABLE    1
#define HAL_CDD_SCOREN_SIGNED      0
#define HAL_CDD_SCOREN_ADR         0x10000021

#define HAL_CDD_C1ERREN_REGISTER    (HAL_CDDREG_MAP->INT1EN)
#define HAL_CDD_C1ERREN_LSB         2
#define HAL_CDD_C1ERREN_MSB         2
#define HAL_CDD_C1ERREN_MASK        0x00000004
#define HAL_CDD_C1ERREN_TYPE        Bool
#define HAL_CDD_C1ERREN             HAL_CDD_C1ERREN_MASK
#define HAL_CDD_C1ERREN_READABLE    1
#define HAL_CDD_C1ERREN_WRITABLE    1
#define HAL_CDD_C1ERREN_SIGNED      0
#define HAL_CDD_C1ERREN_ADR         0x10000021

#define HAL_CDD_C2ERREN_REGISTER    (HAL_CDDREG_MAP->INT1EN)
#define HAL_CDD_C2ERREN_LSB         1
#define HAL_CDD_C2ERREN_MSB         1
#define HAL_CDD_C2ERREN_MASK        0x00000002
#define HAL_CDD_C2ERREN_TYPE        Bool
#define HAL_CDD_C2ERREN             HAL_CDD_C2ERREN_MASK
#define HAL_CDD_C2ERREN_READABLE    1
#define HAL_CDD_C2ERREN_WRITABLE    1
#define HAL_CDD_C2ERREN_SIGNED      0
#define HAL_CDD_C2ERREN_ADR         0x10000021

#define HAL_CDD_CEPSERREN_REGISTER    (HAL_CDDREG_MAP->INT1EN)
#define HAL_CDD_CEPSERREN_LSB         0
#define HAL_CDD_CEPSERREN_MSB         0
#define HAL_CDD_CEPSERREN_MASK        0x00000001
#define HAL_CDD_CEPSERREN_TYPE        Bool
#define HAL_CDD_CEPSERREN             HAL_CDD_CEPSERREN_MASK
#define HAL_CDD_CEPSERREN_READABLE    1
#define HAL_CDD_CEPSERREN_WRITABLE    1
#define HAL_CDD_CEPSERREN_SIGNED      0
#define HAL_CDD_CEPSERREN_ADR         0x10000021

// Register SCORCTL
#define HAL_CDD_SCOREDGE_REGISTER    (HAL_CDDREG_MAP->SCORCTL)
#define HAL_CDD_SCOREDGE_LSB         7
#define HAL_CDD_SCOREDGE_MSB         7
#define HAL_CDD_SCOREDGE_MASK        0x00000080
#define HAL_CDD_SCOREDGE_TYPE        Bool
#define HAL_CDD_SCOREDGE             HAL_CDD_SCOREDGE_MASK
#define HAL_CDD_SCOREDGE_READABLE    1
#define HAL_CDD_SCOREDGE_WRITABLE    1
#define HAL_CDD_SCOREDGE_SIGNED      0
#define HAL_CDD_SCOREDGE_ADR         0x10000023

// Register CDDSPDLY
#define HAL_CDD_CDDSPDLY_REGISTER    (HAL_CDDREG_MAP->CDDSPDLY)
#define HAL_CDD_CDDSPDLY_LSB         0
#define HAL_CDD_CDDSPDLY_MSB         5
#define HAL_CDD_CDDSPDLY_MASK        0x0000003F
#define HAL_CDD_CDDSPDLY_TYPE        UByte
#define HAL_CDD_CDDSPDLY             HAL_CDD_CDDSPDLY_MASK
#define HAL_CDD_CDDSPDLY_READABLE    1
#define HAL_CDD_CDDSPDLY_WRITABLE    1
#define HAL_CDD_CDDSPDLY_SIGNED      0
#define HAL_CDD_CDDSPDLY_ADR         0x10000028

// Registers SCVALDLYL/SCVALDLYH
#define HAL_CDD_SCVALDLY_REGISTER    (HAL_CDDREG_MAP->SCVALDLYL)
#define HAL_CDD_SCVALDLY_LSB         0
#define HAL_CDD_SCVALDLY_MSB         9
#define HAL_CDD_SCVALDLY_MASK        0x000003FF
#define HAL_CDD_SCVALDLY_TYPE        UInt16
#define HAL_CDD_SCVALDLY             HAL_CDD_SCVALDLY_MASK
#define HAL_CDD_SCVALDLY_READABLE    1
#define HAL_CDD_SCVALDLY_WRITABLE    1
#define HAL_CDD_SCVALDLY_SIGNED      0
#define HAL_CDD_SCVALDLY_ADR         0x1000002A

#if (CHIP_REV >= 0xC0)

#define HAL_CDD_SCDLYSEL_REGISTER    (HAL_CDDREG_MAP->SCVALDLYL)
#define HAL_CDD_SCDLYSEL_LSB         15
#define HAL_CDD_SCDLYSEL_MSB         15
#define HAL_CDD_SCDLYSEL_MASK        0x00008000
#define HAL_CDD_SCDLYSEL_TYPE        Bool
#define HAL_CDD_SCDLYSEL             HAL_CDD_SCDLYSEL_MASK
#define HAL_CDD_SCDLYSEL_READABLE    1
#define HAL_CDD_SCDLYSEL_WRITABLE    1
#define HAL_CDD_SCDLYSEL_SIGNED      0
#define HAL_CDD_SCDLYSEL_ADR         0x1000002A

#endif // #if (CHIP_REV >= 0xC0)

// Register ADVC
#define HAL_CDD_C2P_REGISTER    (HAL_CDDREG_MAP->ADVC)
#define HAL_CDD_C2P_LSB         4
#define HAL_CDD_C2P_MSB         5
#define HAL_CDD_C2P_MASK        0x00000030
#define HAL_CDD_C2P_TYPE        halCDD_C2PCtrl_t
#define HAL_CDD_C2P             HAL_CDD_C2P_MASK
#define HAL_CDD_C2P_READABLE    1
#define HAL_CDD_C2P_WRITABLE    1
#define HAL_CDD_C2P_SIGNED      0
#define HAL_CDD_C2P_ADR         0x10000040

#define HAL_CDD_C2WEN_REGISTER    (HAL_CDDREG_MAP->ADVC)
#define HAL_CDD_C2WEN_LSB         2
#define HAL_CDD_C2WEN_MSB         2
#define HAL_CDD_C2WEN_MASK        0x00000004
#define HAL_CDD_C2WEN_TYPE        Bool
#define HAL_CDD_C2WEN             HAL_CDD_C2WEN_MASK
#define HAL_CDD_C2WEN_READABLE    1
#define HAL_CDD_C2WEN_WRITABLE    1
#define HAL_CDD_C2WEN_SIGNED      0
#define HAL_CDD_C2WEN_ADR         0x10000040

// Register C2PAR
#define HAL_CDD_C2PARCNT_REGISTER    (HAL_CDDREG_MAP->C2PAR)
#define HAL_CDD_C2PARCNT_LSB         0
#define HAL_CDD_C2PARCNT_MSB         7
#define HAL_CDD_C2PARCNT_MASK        0x000000FF
#define HAL_CDD_C2PARCNT_TYPE        UByte
#define HAL_CDD_C2PARCNT             HAL_CDD_C2PARCNT_MASK
#define HAL_CDD_C2PARCNT_READABLE    1
#define HAL_CDD_C2PARCNT_WRITABLE    0
#define HAL_CDD_C2PARCNT_SIGNED      0
#define HAL_CDD_C2PARCNT_ADR         0x10000041

// Register BUFCTL
#define HAL_CDD_DAPLAY_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_DAPLAY_LSB         7
#define HAL_CDD_DAPLAY_MSB         7
#define HAL_CDD_DAPLAY_MASK        0x00000080
#define HAL_CDD_DAPLAY_TYPE        Bool
#define HAL_CDD_DAPLAY             HAL_CDD_DAPLAY_MASK
#define HAL_CDD_DAPLAY_READABLE    1
#define HAL_CDD_DAPLAY_WRITABLE    1
#define HAL_CDD_DAPLAY_SIGNED      0
#define HAL_CDD_DAPLAY_ADR         0x10000043

#define HAL_CDD_AUTOSBUF_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_AUTOSBUF_LSB         5
#define HAL_CDD_AUTOSBUF_MSB         5
#define HAL_CDD_AUTOSBUF_MASK        0x00000020
#define HAL_CDD_AUTOSBUF_TYPE        Bool
#define HAL_CDD_AUTOSBUF             HAL_CDD_AUTOSBUF_MASK
#define HAL_CDD_AUTOSBUF_READABLE    1
#define HAL_CDD_AUTOSBUF_WRITABLE    1
#define HAL_CDD_AUTOSBUF_SIGNED      0
#define HAL_CDD_AUTOSBUF_ADR         0x10000043

#define HAL_CDD_SMSFCERR_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_SMSFCERR_LSB         4
#define HAL_CDD_SMSFCERR_MSB         4
#define HAL_CDD_SMSFCERR_MASK        0x00000010
#define HAL_CDD_SMSFCERR_TYPE        Bool
#define HAL_CDD_SMSFCERR             HAL_CDD_SMSFCERR_MASK
#define HAL_CDD_SMSFCERR_READABLE    1
#define HAL_CDD_SMSFCERR_WRITABLE    1
#define HAL_CDD_SMSFCERR_SIGNED      0
#define HAL_CDD_SMSFCERR_ADR         0x10000043

#define HAL_CDD_SC3ERR_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_SC3ERR_LSB         3
#define HAL_CDD_SC3ERR_MSB         3
#define HAL_CDD_SC3ERR_MASK        0x00000008
#define HAL_CDD_SC3ERR_TYPE        Bool
#define HAL_CDD_SC3ERR             HAL_CDD_SC3ERR_MASK
#define HAL_CDD_SC3ERR_READABLE    1
#define HAL_CDD_SC3ERR_WRITABLE    1
#define HAL_CDD_SC3ERR_SIGNED      0
#define HAL_CDD_SC3ERR_ADR         0x10000043

#define HAL_CDD_SDIFERR_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_SDIFERR_LSB         2
#define HAL_CDD_SDIFERR_MSB         2
#define HAL_CDD_SDIFERR_MASK        0x00000004
#define HAL_CDD_SDIFERR_TYPE        Bool
#define HAL_CDD_SDIFERR             HAL_CDD_SDIFERR_MASK
#define HAL_CDD_SDIFERR_READABLE    1
#define HAL_CDD_SDIFERR_WRITABLE    1
#define HAL_CDD_SDIFERR_SIGNED      0
#define HAL_CDD_SDIFERR_ADR         0x10000043

#define HAL_CDD_SBFULL_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_SBFULL_LSB         1
#define HAL_CDD_SBFULL_MSB         1
#define HAL_CDD_SBFULL_MASK        0x00000002
#define HAL_CDD_SBFULL_TYPE        Bool
#define HAL_CDD_SBFULL             HAL_CDD_SBFULL_MASK
#define HAL_CDD_SBFULL_READABLE    1
#define HAL_CDD_SBFULL_WRITABLE    1
#define HAL_CDD_SBFULL_SIGNED      0
#define HAL_CDD_SBFULL_ADR         0x10000043

#define HAL_CDD_AUTOPKT_REGISTER    (HAL_CDDREG_MAP->BUFCTL)
#define HAL_CDD_AUTOPKT_LSB         0
#define HAL_CDD_AUTOPKT_MSB         0
#define HAL_CDD_AUTOPKT_MASK        0x00000001
#define HAL_CDD_AUTOPKT_TYPE        Bool
#define HAL_CDD_AUTOPKT             HAL_CDD_AUTOPKT_MASK
#define HAL_CDD_AUTOPKT_READABLE    1
#define HAL_CDD_AUTOPKT_WRITABLE    1
#define HAL_CDD_AUTOPKT_SIGNED      0
#define HAL_CDD_AUTOPKT_ADR         0x10000043

// Registers TWBLKL/TWBLKH
#define HAL_CDD_SHORTBLK_REGISTER    (HAL_CDDREG_MAP->TWBLKL)
#define HAL_CDD_SHORTBLK_LSB         0
#define HAL_CDD_SHORTBLK_MSB         10
#define HAL_CDD_SHORTBLK_MASK        0x000007FF
#define HAL_CDD_SHORTBLK_TYPE        UInt16
#define HAL_CDD_SHORTBLK             HAL_CDD_SHORTBLK_MASK
#define HAL_CDD_SHORTBLK_READABLE    1
#define HAL_CDD_SHORTBLK_WRITABLE    1
#define HAL_CDD_SHORTBLK_SIGNED      0
#define HAL_CDD_SHORTBLK_ADR         0x1000004E

#define HAL_CDD_TSTFEWCWD_REGISTER    (HAL_CDDREG_MAP->TWBLKL)
#define HAL_CDD_TSTFEWCWD_LSB         15
#define HAL_CDD_TSTFEWCWD_MSB         15
#define HAL_CDD_TSTFEWCWD_MASK        0x00008000
#define HAL_CDD_TSTFEWCWD_TYPE        Bool
#define HAL_CDD_TSTFEWCWD             HAL_CDD_TSTFEWCWD_MASK
#define HAL_CDD_TSTFEWCWD_READABLE    1
#define HAL_CDD_TSTFEWCWD_WRITABLE    1
#define HAL_CDD_TSTFEWCWD_SIGNED      0
#define HAL_CDD_TSTFEWCWD_ADR         0x1000004E

// Register STATS
#define HAL_CDD_RWCRCOK_REGISTER    (HAL_CDDREG_MAP->STATS)
#define HAL_CDD_RWCRCOK_LSB         6
#define HAL_CDD_RWCRCOK_MSB         6
#define HAL_CDD_RWCRCOK_MASK        0x00000040
#define HAL_CDD_RWCRCOK_TYPE        Bool
#define HAL_CDD_RWCRCOK             HAL_CDD_RWCRCOK_MASK
#define HAL_CDD_RWCRCOK_READABLE    1
#define HAL_CDD_RWCRCOK_WRITABLE    0
#define HAL_CDD_RWCRCOK_SIGNED      0
#define HAL_CDD_RWCRCOK_ADR         0x10000050

#define HAL_CDD_QHCERR_REGISTER    (HAL_CDDREG_MAP->STATS)
#define HAL_CDD_QHCERR_LSB         5
#define HAL_CDD_QHCERR_MSB         5
#define HAL_CDD_QHCERR_MASK        0x00000020
#define HAL_CDD_QHCERR_TYPE        Bool
#define HAL_CDD_QHCERR             HAL_CDD_QHCERR_MASK
#define HAL_CDD_QHCERR_READABLE    1
#define HAL_CDD_QHCERR_WRITABLE    0
#define HAL_CDD_QHCERR_SIGNED      0
#define HAL_CDD_QHCERR_ADR         0x10000050

#define HAL_CDD_QCRCOK_REGISTER    (HAL_CDDREG_MAP->STATS)
#define HAL_CDD_QCRCOK_LSB         4
#define HAL_CDD_QCRCOK_MSB         4
#define HAL_CDD_QCRCOK_MASK        0x00000010
#define HAL_CDD_QCRCOK_TYPE        Bool
#define HAL_CDD_QCRCOK             HAL_CDD_QCRCOK_MASK
#define HAL_CDD_QCRCOK_READABLE    1
#define HAL_CDD_QCRCOK_WRITABLE    0
#define HAL_CDD_QCRCOK_SIGNED      0
#define HAL_CDD_QCRCOK_ADR         0x10000050

#define HAL_CDD_SBLKERR_REGISTER    (HAL_CDDREG_MAP->STATS)
#define HAL_CDD_SBLKERR_LSB         3
#define HAL_CDD_SBLKERR_MSB         3
#define HAL_CDD_SBLKERR_MASK        0x00000008
#define HAL_CDD_SBLKERR_TYPE        Bool
#define HAL_CDD_SBLKERR             HAL_CDD_SBLKERR_MASK
#define HAL_CDD_SBLKERR_READABLE    1
#define HAL_CDD_SBLKERR_WRITABLE    0
#define HAL_CDD_SBLKERR_SIGNED      0
#define HAL_CDD_SBLKERR_ADR         0x10000050

// Register SUBCD
#define HAL_CDD_SCEN_REGISTER    (HAL_CDDREG_MAP->SUBCD)
#define HAL_CDD_SCEN_LSB         7
#define HAL_CDD_SCEN_MSB         7
#define HAL_CDD_SCEN_MASK        0x00000080
#define HAL_CDD_SCEN_TYPE        Bool
#define HAL_CDD_SCEN             HAL_CDD_SCEN_MASK
#define HAL_CDD_SCEN_READABLE    1
#define HAL_CDD_SCEN_WRITABLE    1
#define HAL_CDD_SCEN_SIGNED      0
#define HAL_CDD_SCEN_ADR         0x10000051

#define HAL_CDD_BUFCDT_REGISTER    (HAL_CDDREG_MAP->SUBCD)
#define HAL_CDD_BUFCDT_LSB         6
#define HAL_CDD_BUFCDT_MSB         6
#define HAL_CDD_BUFCDT_MASK        0x00000040
#define HAL_CDD_BUFCDT_TYPE        Bool
#define HAL_CDD_BUFCDT             HAL_CDD_BUFCDT_MASK
#define HAL_CDD_BUFCDT_READABLE    1
#define HAL_CDD_BUFCDT_WRITABLE    1
#define HAL_CDD_BUFCDT_SIGNED      0
#define HAL_CDD_BUFCDT_ADR         0x10000051

#define HAL_CDD_NOPQ_REGISTER    (HAL_CDDREG_MAP->SUBCD)
#define HAL_CDD_NOPQ_LSB         3
#define HAL_CDD_NOPQ_MSB         3
#define HAL_CDD_NOPQ_MASK        0x00000008
#define HAL_CDD_NOPQ_TYPE        Bool
#define HAL_CDD_NOPQ             HAL_CDD_NOPQ_MASK
#define HAL_CDD_NOPQ_READABLE    1
#define HAL_CDD_NOPQ_WRITABLE    1
#define HAL_CDD_NOPQ_SIGNED      0
#define HAL_CDD_NOPQ_ADR         0x10000051

// Register SUBRWC
#define HAL_CDD_NODITLV_REGISTER    (HAL_CDDREG_MAP->SUBRWC)
#define HAL_CDD_NODITLV_LSB         5
#define HAL_CDD_NODITLV_MSB         5
#define HAL_CDD_NODITLV_MASK        0x00000020
#define HAL_CDD_NODITLV_TYPE        Bool
#define HAL_CDD_NODITLV             HAL_CDD_NODITLV_MASK
#define HAL_CDD_NODITLV_READABLE    1
#define HAL_CDD_NODITLV_WRITABLE    1
#define HAL_CDD_NODITLV_SIGNED      0
#define HAL_CDD_NODITLV_ADR         0x10000052

#define HAL_CDD_NEWREAD_REGISTER    (HAL_CDDREG_MAP->SUBRWC)
#define HAL_CDD_NEWREAD_LSB         4
#define HAL_CDD_NEWREAD_MSB         4
#define HAL_CDD_NEWREAD_MASK        0x00000010
#define HAL_CDD_NEWREAD_TYPE        Bool
#define HAL_CDD_NEWREAD             HAL_CDD_NEWREAD_MASK
#define HAL_CDD_NEWREAD_READABLE    1
#define HAL_CDD_NEWREAD_WRITABLE    1
#define HAL_CDD_NEWREAD_SIGNED      0
#define HAL_CDD_NEWREAD_ADR         0x10000052

#define HAL_CDD_RTRYCNT_REGISTER    (HAL_CDDREG_MAP->SUBRWC)
#define HAL_CDD_RTRYCNT_LSB         2
#define HAL_CDD_RTRYCNT_MSB         3
#define HAL_CDD_RTRYCNT_MASK        0x0000000C
#define HAL_CDD_RTRYCNT_TYPE        halCDD_R2WECCRetry_t
#define HAL_CDD_RTRYCNT             HAL_CDD_RTRYCNT_MASK
#define HAL_CDD_RTRYCNT_READABLE    1
#define HAL_CDD_RTRYCNT_WRITABLE    1
#define HAL_CDD_RTRYCNT_SIGNED      0
#define HAL_CDD_RTRYCNT_ADR         0x10000052

#define HAL_CDD_BYPASSQ_REGISTER    (HAL_CDDREG_MAP->SUBRWC)
#define HAL_CDD_BYPASSQ_LSB         1
#define HAL_CDD_BYPASSQ_MSB         1
#define HAL_CDD_BYPASSQ_MASK        0x00000002
#define HAL_CDD_BYPASSQ_TYPE        Bool
#define HAL_CDD_BYPASSQ             HAL_CDD_BYPASSQ_MASK
#define HAL_CDD_BYPASSQ_READABLE    1
#define HAL_CDD_BYPASSQ_WRITABLE    1
#define HAL_CDD_BYPASSQ_SIGNED      0
#define HAL_CDD_BYPASSQ_ADR         0x10000052

#define HAL_CDD_HWECCEN_REGISTER    (HAL_CDDREG_MAP->SUBRWC)
#define HAL_CDD_HWECCEN_LSB         0
#define HAL_CDD_HWECCEN_MSB         0
#define HAL_CDD_HWECCEN_MASK        0x00000001
#define HAL_CDD_HWECCEN_TYPE        Bool
#define HAL_CDD_HWECCEN             HAL_CDD_HWECCEN_MASK
#define HAL_CDD_HWECCEN_READABLE    1
#define HAL_CDD_HWECCEN_WRITABLE    1
#define HAL_CDD_HWECCEN_SIGNED      0
#define HAL_CDD_HWECCEN_ADR         0x10000052

// Register SUBQC
#define HAL_CDD_SUBQITPL_REGISTER    (HAL_CDDREG_MAP->SUBQC)
#define HAL_CDD_SUBQITPL_LSB         4
#define HAL_CDD_SUBQITPL_MSB         4
#define HAL_CDD_SUBQITPL_MASK        0x00000010
#define HAL_CDD_SUBQITPL_TYPE        Bool
#define HAL_CDD_SUBQITPL             HAL_CDD_SUBQITPL_MASK
#define HAL_CDD_SUBQITPL_READABLE    1
#define HAL_CDD_SUBQITPL_WRITABLE    1
#define HAL_CDD_SUBQITPL_SIGNED      0
#define HAL_CDD_SUBQITPL_ADR         0x10000053

#define HAL_CDD_BUFSBQ_REGISTER    (HAL_CDDREG_MAP->SUBQC)
#define HAL_CDD_BUFSBQ_LSB         3
#define HAL_CDD_BUFSBQ_MSB         3
#define HAL_CDD_BUFSBQ_MASK        0x00000008
#define HAL_CDD_BUFSBQ_TYPE        Bool
#define HAL_CDD_BUFSBQ             HAL_CDD_BUFSBQ_MASK
#define HAL_CDD_BUFSBQ_READABLE    1
#define HAL_CDD_BUFSBQ_WRITABLE    1
#define HAL_CDD_BUFSBQ_SIGNED      0
#define HAL_CDD_BUFSBQ_ADR         0x10000053

#define HAL_CDD_BUFSBC_REGISTER    (HAL_CDDREG_MAP->SUBQC)
#define HAL_CDD_BUFSBC_LSB         2
#define HAL_CDD_BUFSBC_MSB         2
#define HAL_CDD_BUFSBC_MASK        0x00000004
#define HAL_CDD_BUFSBC_TYPE        Bool
#define HAL_CDD_BUFSBC             HAL_CDD_BUFSBC_MASK
#define HAL_CDD_BUFSBC_READABLE    1
#define HAL_CDD_BUFSBC_WRITABLE    1
#define HAL_CDD_BUFSBC_SIGNED      0
#define HAL_CDD_BUFSBC_ADR         0x10000053

#define HAL_CDD_QCRCE_REGISTER    (HAL_CDDREG_MAP->SUBQC)
#define HAL_CDD_QCRCE_LSB         1
#define HAL_CDD_QCRCE_MSB         1
#define HAL_CDD_QCRCE_MASK        0x00000002
#define HAL_CDD_QCRCE_TYPE        Bool
#define HAL_CDD_QCRCE             HAL_CDD_QCRCE_MASK
#define HAL_CDD_QCRCE_READABLE    1
#define HAL_CDD_QCRCE_WRITABLE    1
#define HAL_CDD_QCRCE_SIGNED      0
#define HAL_CDD_QCRCE_ADR         0x10000053

// Register SCSTAT
#define HAL_CDD_QTN0_REGISTER    (HAL_CDDREG_MAP->SCSTAT)
#define HAL_CDD_QTN0_LSB         6
#define HAL_CDD_QTN0_MSB         6
#define HAL_CDD_QTN0_MASK        0x00000040
#define HAL_CDD_QTN0_TYPE        Bool
#define HAL_CDD_QTN0             HAL_CDD_QTN0_MASK
#define HAL_CDD_QTN0_READABLE    1
#define HAL_CDD_QTN0_WRITABLE    0
#define HAL_CDD_QTN0_SIGNED      0
#define HAL_CDD_QTN0_ADR         0x10000054

#define HAL_CDD_QADR23_REGISTER    (HAL_CDDREG_MAP->SCSTAT)
#define HAL_CDD_QADR23_LSB         5
#define HAL_CDD_QADR23_MSB         5
#define HAL_CDD_QADR23_MASK        0x00000020
#define HAL_CDD_QADR23_TYPE        Bool
#define HAL_CDD_QADR23             HAL_CDD_QADR23_MASK
#define HAL_CDD_QADR23_READABLE    1
#define HAL_CDD_QADR23_WRITABLE    0
#define HAL_CDD_QADR23_SIGNED      0
#define HAL_CDD_QADR23_ADR         0x10000054

#define HAL_CDD_QADR1_REGISTER    (HAL_CDDREG_MAP->SCSTAT)
#define HAL_CDD_QADR1_LSB         4
#define HAL_CDD_QADR1_MSB         4
#define HAL_CDD_QADR1_MASK        0x00000010
#define HAL_CDD_QADR1_TYPE        Bool
#define HAL_CDD_QADR1             HAL_CDD_QADR1_MASK
#define HAL_CDD_QADR1_READABLE    1
#define HAL_CDD_QADR1_WRITABLE    0
#define HAL_CDD_QADR1_SIGNED      0
#define HAL_CDD_QADR1_ADR         0x10000054

#if (CHIP_REV >= 0xC0)

// Register LFCTRL
#define HAL_CDD_BASANGLE_REGISTER    (HAL_CDDREG_MAP->LFCTRL)
#define HAL_CDD_BASANGLE_LSB         7
#define HAL_CDD_BASANGLE_MSB         7
#define HAL_CDD_BASANGLE_MASK        0x00000080
#define HAL_CDD_BASANGLE_TYPE        Bool
#define HAL_CDD_BASANGLE             HAL_CDD_BASANGLE_MASK
#define HAL_CDD_BASANGLE_READABLE    1
#define HAL_CDD_BASANGLE_WRITABLE    0
#define HAL_CDD_BASANGLE_SIGNED      0
#define HAL_CDD_BASANGLE_ADR         0x10000055

#define HAL_CDD_BSANGEN_REGISTER    (HAL_CDDREG_MAP->LFCTRL)
#define HAL_CDD_BSANGEN_LSB         0
#define HAL_CDD_BSANGEN_MSB         0
#define HAL_CDD_BSANGEN_MASK        0x00000001
#define HAL_CDD_BSANGEN_TYPE        Bool
#define HAL_CDD_BSANGEN             HAL_CDD_BSANGEN_MASK
#define HAL_CDD_BSANGEN_READABLE    1
#define HAL_CDD_BSANGEN_WRITABLE    1
#define HAL_CDD_BSANGEN_SIGNED      0
#define HAL_CDD_BSANGEN_ADR         0x10000055

#endif // #if (CHIP_REV >= 0xC0)

// Register CTRLAM0
#define HAL_CDD_AUTOEM_REGISTER    (HAL_CDDREG_MAP->CTRLAM0)
#define HAL_CDD_AUTOEM_LSB         7
#define HAL_CDD_AUTOEM_MSB         7
#define HAL_CDD_AUTOEM_MASK        0x00000080
#define HAL_CDD_AUTOEM_TYPE        Bool
#define HAL_CDD_AUTOEM             HAL_CDD_AUTOEM_MASK
#define HAL_CDD_AUTOEM_READABLE    1
#define HAL_CDD_AUTOEM_WRITABLE    1
#define HAL_CDD_AUTOEM_SIGNED      0
#define HAL_CDD_AUTOEM_ADR         0x10000070

#define HAL_CDD_DINFWE_REGISTER    (HAL_CDDREG_MAP->CTRLAM0)
#define HAL_CDD_DINFWE_LSB         1
#define HAL_CDD_DINFWE_MSB         1
#define HAL_CDD_DINFWE_MASK        0x00000002
#define HAL_CDD_DINFWE_TYPE        Bool
#define HAL_CDD_DINFWE             HAL_CDD_DINFWE_MASK
#define HAL_CDD_DINFWE_READABLE    1
#define HAL_CDD_DINFWE_WRITABLE    1
#define HAL_CDD_DINFWE_SIGNED      0
#define HAL_CDD_DINFWE_ADR         0x10000070

#define HAL_CDD_IHDRW_REGISTER    (HAL_CDDREG_MAP->CTRLAM0)
#define HAL_CDD_IHDRW_LSB         0
#define HAL_CDD_IHDRW_MSB         0
#define HAL_CDD_IHDRW_MASK        0x00000001
#define HAL_CDD_IHDRW_TYPE        Bool
#define HAL_CDD_IHDRW             HAL_CDD_IHDRW_MASK
#define HAL_CDD_IHDRW_READABLE    1
#define HAL_CDD_IHDRW_WRITABLE    1
#define HAL_CDD_IHDRW_SIGNED      0
#define HAL_CDD_IHDRW_ADR         0x10000070

// Register CTRLAM1
#define HAL_CDD_DECEN_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_DECEN_LSB         7
#define HAL_CDD_DECEN_MSB         7
#define HAL_CDD_DECEN_MASK        0x00000080
#define HAL_CDD_DECEN_TYPE        Bool
#define HAL_CDD_DECEN             HAL_CDD_DECEN_MASK
#define HAL_CDD_DECEN_READABLE    1
#define HAL_CDD_DECEN_WRITABLE    1
#define HAL_CDD_DECEN_SIGNED      0
#define HAL_CDD_DECEN_ADR         0x10000071

#define HAL_CDD_DPCLR_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_DPCLR_LSB         6
#define HAL_CDD_DPCLR_MSB         6
#define HAL_CDD_DPCLR_MASK        0x00000040
#define HAL_CDD_DPCLR_TYPE        Bool
#define HAL_CDD_DPCLR             HAL_CDD_DPCLR_MASK
#define HAL_CDD_DPCLR_READABLE    1
#define HAL_CDD_DPCLR_WRITABLE    1
#define HAL_CDD_DPCLR_SIGNED      0
#define HAL_CDD_DPCLR_ADR         0x10000071

#define HAL_CDD_QPINH_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_QPINH_LSB         5
#define HAL_CDD_QPINH_MSB         5
#define HAL_CDD_QPINH_MASK        0x00000020
#define HAL_CDD_QPINH_TYPE        Bool
#define HAL_CDD_QPINH             HAL_CDD_QPINH_MASK
#define HAL_CDD_QPINH_READABLE    1
#define HAL_CDD_QPINH_WRITABLE    1
#define HAL_CDD_QPINH_SIGNED      0
#define HAL_CDD_QPINH_ADR         0x10000071

#define HAL_CDD_SDBUF_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_SDBUF_LSB         3
#define HAL_CDD_SDBUF_MSB         3
#define HAL_CDD_SDBUF_MASK        0x00000008
#define HAL_CDD_SDBUF_TYPE        Bool
#define HAL_CDD_SDBUF             HAL_CDD_SDBUF_MASK
#define HAL_CDD_SDBUF_READABLE    1
#define HAL_CDD_SDBUF_WRITABLE    1
#define HAL_CDD_SDBUF_SIGNED      0
#define HAL_CDD_SDBUF_ADR         0x10000071

#define HAL_CDD_SDMON_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_SDMON_LSB         2
#define HAL_CDD_SDMON_MSB         2
#define HAL_CDD_SDMON_MASK        0x00000004
#define HAL_CDD_SDMON_TYPE        Bool
#define HAL_CDD_SDMON             HAL_CDD_SDMON_MASK
#define HAL_CDD_SDMON_READABLE    1
#define HAL_CDD_SDMON_WRITABLE    1
#define HAL_CDD_SDMON_SIGNED      0
#define HAL_CDD_SDMON_ADR         0x10000071

#define HAL_CDD_SCDDA_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_SCDDA_LSB         1
#define HAL_CDD_SCDDA_MSB         1
#define HAL_CDD_SCDDA_MASK        0x00000002
#define HAL_CDD_SCDDA_TYPE        Bool
#define HAL_CDD_SCDDA             HAL_CDD_SCDDA_MASK
#define HAL_CDD_SCDDA_READABLE    1
#define HAL_CDD_SCDDA_WRITABLE    1
#define HAL_CDD_SCDDA_SIGNED      0
#define HAL_CDD_SCDDA_ADR         0x10000071

#define HAL_CDD_M2EINH_REGISTER    (HAL_CDDREG_MAP->CTRLAM1)
#define HAL_CDD_M2EINH_LSB         0
#define HAL_CDD_M2EINH_MSB         0
#define HAL_CDD_M2EINH_MASK        0x00000001
#define HAL_CDD_M2EINH_TYPE        Bool
#define HAL_CDD_M2EINH             HAL_CDD_M2EINH_MASK
#define HAL_CDD_M2EINH_READABLE    1
#define HAL_CDD_M2EINH_WRITABLE    1
#define HAL_CDD_M2EINH_SIGNED      0
#define HAL_CDD_M2EINH_ADR         0x10000071

// Register CTRL1
#define HAL_CDD_MODRQ_REGISTER    (HAL_CDDREG_MAP->CTRL1)
#define HAL_CDD_MODRQ_LSB         3
#define HAL_CDD_MODRQ_MSB         3
#define HAL_CDD_MODRQ_MASK        0x00000008
#define HAL_CDD_MODRQ_TYPE        Bool
#define HAL_CDD_MODRQ             HAL_CDD_MODRQ_MASK
#define HAL_CDD_MODRQ_READABLE    1
#define HAL_CDD_MODRQ_WRITABLE    1
#define HAL_CDD_MODRQ_SIGNED      0
#define HAL_CDD_MODRQ_ADR         0x10000073

#define HAL_CDD_FORMRQ_REGISTER    (HAL_CDDREG_MAP->CTRL1)
#define HAL_CDD_FORMRQ_LSB         2
#define HAL_CDD_FORMRQ_MSB         2
#define HAL_CDD_FORMRQ_MASK        0x00000004
#define HAL_CDD_FORMRQ_TYPE        Bool
#define HAL_CDD_FORMRQ             HAL_CDD_FORMRQ_MASK
#define HAL_CDD_FORMRQ_READABLE    1
#define HAL_CDD_FORMRQ_WRITABLE    1
#define HAL_CDD_FORMRQ_SIGNED      0
#define HAL_CDD_FORMRQ_ADR         0x10000073

// Register THEAD0
#define HAL_CDD_BCDTHMIN_REGISTER    (HAL_CDDREG_MAP->THEAD0)
#define HAL_CDD_BCDTHMIN_LSB         0
#define HAL_CDD_BCDTHMIN_MSB         7
#define HAL_CDD_BCDTHMIN_MASK        0x000000FF
#define HAL_CDD_BCDTHMIN_TYPE        UByte
#define HAL_CDD_BCDTHMIN             HAL_CDD_BCDTHMIN_MASK
#define HAL_CDD_BCDTHMIN_READABLE    1
#define HAL_CDD_BCDTHMIN_WRITABLE    1
#define HAL_CDD_BCDTHMIN_SIGNED      0
#define HAL_CDD_BCDTHMIN_ADR         0x10000074

// Register THEAD1
#define HAL_CDD_BCDTHSEC_REGISTER    (HAL_CDDREG_MAP->THEAD1)
#define HAL_CDD_BCDTHSEC_LSB         0
#define HAL_CDD_BCDTHSEC_MSB         7
#define HAL_CDD_BCDTHSEC_MASK        0x000000FF
#define HAL_CDD_BCDTHSEC_TYPE        UByte
#define HAL_CDD_BCDTHSEC             HAL_CDD_BCDTHSEC_MASK
#define HAL_CDD_BCDTHSEC_READABLE    1
#define HAL_CDD_BCDTHSEC_WRITABLE    1
#define HAL_CDD_BCDTHSEC_SIGNED      0
#define HAL_CDD_BCDTHSEC_ADR         0x10000075

// Register THEAD2
#define HAL_CDD_BCDTHFRM_REGISTER    (HAL_CDDREG_MAP->THEAD2)
#define HAL_CDD_BCDTHFRM_LSB         0
#define HAL_CDD_BCDTHFRM_MSB         7
#define HAL_CDD_BCDTHFRM_MASK        0x000000FF
#define HAL_CDD_BCDTHFRM_TYPE        UByte
#define HAL_CDD_BCDTHFRM             HAL_CDD_BCDTHFRM_MASK
#define HAL_CDD_BCDTHFRM_READABLE    1
#define HAL_CDD_BCDTHFRM_WRITABLE    1
#define HAL_CDD_BCDTHFRM_SIGNED      0
#define HAL_CDD_BCDTHFRM_ADR         0x10000076

// Register EDACCFG
#define HAL_CDD_YBM2DIS_REGISTER    (HAL_CDDREG_MAP->EDACCFG)
#define HAL_CDD_YBM2DIS_LSB         3
#define HAL_CDD_YBM2DIS_MSB         3
#define HAL_CDD_YBM2DIS_MASK        0x00000008
#define HAL_CDD_YBM2DIS_TYPE        Bool
#define HAL_CDD_YBM2DIS             HAL_CDD_YBM2DIS_MASK
#define HAL_CDD_YBM2DIS_READABLE    1
#define HAL_CDD_YBM2DIS_WRITABLE    1
#define HAL_CDD_YBM2DIS_SIGNED      0
#define HAL_CDD_YBM2DIS_ADR         0x10000078

// Register C2THLD
#define HAL_CDD_C2T_REGISTER    (HAL_CDDREG_MAP->C2THLD)
#define HAL_CDD_C2T_LSB         0
#define HAL_CDD_C2T_MSB         7
#define HAL_CDD_C2T_MASK        0x000000FF
#define HAL_CDD_C2T_TYPE        UByte
#define HAL_CDD_C2T             HAL_CDD_C2T_MASK
#define HAL_CDD_C2T_READABLE    1
#define HAL_CDD_C2T_WRITABLE    1
#define HAL_CDD_C2T_SIGNED      0
#define HAL_CDD_C2T_ADR         0x10000079

// Register DPRDPG
#define HAL_CDD_DPRDPG_REGISTER    (HAL_CDDREG_MAP->DPRDPG)
#define HAL_CDD_DPRDPG_LSB         0
#define HAL_CDD_DPRDPG_MSB         7
#define HAL_CDD_DPRDPG_MASK        0x000000FF
#define HAL_CDD_DPRDPG_TYPE        UByte
#define HAL_CDD_DPRDPG             HAL_CDD_DPRDPG_MASK
#define HAL_CDD_DPRDPG_READABLE    1
#define HAL_CDD_DPRDPG_WRITABLE    0
#define HAL_CDD_DPRDPG_SIGNED      0
#define HAL_CDD_DPRDPG_ADR         0x1000007A

// Register CTRLEDC0
#define HAL_CDD_AEWAEN_REGISTER    (HAL_CDDREG_MAP->CTRLEDC0)
#define HAL_CDD_AEWAEN_LSB         6
#define HAL_CDD_AEWAEN_MSB         6
#define HAL_CDD_AEWAEN_MASK        0x00000040
#define HAL_CDD_AEWAEN_TYPE        Bool
#define HAL_CDD_AEWAEN             HAL_CDD_AEWAEN_MASK
#define HAL_CDD_AEWAEN_READABLE    1
#define HAL_CDD_AEWAEN_WRITABLE    1
#define HAL_CDD_AEWAEN_SIGNED      0
#define HAL_CDD_AEWAEN_ADR         0x1000007C

// Register CTRLEDC1
#define HAL_CDD_ECCINCEN_REGISTER    (HAL_CDDREG_MAP->CTRLEDC1)
#define HAL_CDD_ECCINCEN_LSB         0
#define HAL_CDD_ECCINCEN_MSB         0
#define HAL_CDD_ECCINCEN_MASK        0x00000001
#define HAL_CDD_ECCINCEN_TYPE        Bool
#define HAL_CDD_ECCINCEN             HAL_CDD_ECCINCEN_MASK
#define HAL_CDD_ECCINCEN_READABLE    1
#define HAL_CDD_ECCINCEN_WRITABLE    1
#define HAL_CDD_ECCINCEN_SIGNED      0
#define HAL_CDD_ECCINCEN_ADR         0x1000007D

// Register CTRLECC
#define HAL_CDD_M2F2EDCDIS_REGISTER    (HAL_CDDREG_MAP->CTRLECC)
#define HAL_CDD_M2F2EDCDIS_LSB         7
#define HAL_CDD_M2F2EDCDIS_MSB         7
#define HAL_CDD_M2F2EDCDIS_MASK        0x00000080
#define HAL_CDD_M2F2EDCDIS_TYPE        Bool
#define HAL_CDD_M2F2EDCDIS             HAL_CDD_M2F2EDCDIS_MASK
#define HAL_CDD_M2F2EDCDIS_READABLE    1
#define HAL_CDD_M2F2EDCDIS_WRITABLE    1
#define HAL_CDD_M2F2EDCDIS_SIGNED      0
#define HAL_CDD_M2F2EDCDIS_ADR         0x1000007E

#define HAL_CDD_E01RQ_REGISTER    (HAL_CDDREG_MAP->CTRLECC)
#define HAL_CDD_E01RQ_LSB         5
#define HAL_CDD_E01RQ_MSB         5
#define HAL_CDD_E01RQ_MASK        0x00000020
#define HAL_CDD_E01RQ_TYPE        Bool
#define HAL_CDD_E01RQ             HAL_CDD_E01RQ_MASK
#define HAL_CDD_E01RQ_READABLE    1
#define HAL_CDD_E01RQ_WRITABLE    1
#define HAL_CDD_E01RQ_SIGNED      0
#define HAL_CDD_E01RQ_ADR         0x1000007E

#define HAL_CDD_AUTORQ_REGISTER    (HAL_CDDREG_MAP->CTRLECC)
#define HAL_CDD_AUTORQ_LSB         4
#define HAL_CDD_AUTORQ_MSB         4
#define HAL_CDD_AUTORQ_MASK        0x00000010
#define HAL_CDD_AUTORQ_TYPE        Bool
#define HAL_CDD_AUTORQ             HAL_CDD_AUTORQ_MASK
#define HAL_CDD_AUTORQ_READABLE    1
#define HAL_CDD_AUTORQ_WRITABLE    1
#define HAL_CDD_AUTORQ_SIGNED      0
#define HAL_CDD_AUTORQ_ADR         0x1000007E

#define HAL_CDD_QRQ_REGISTER    (HAL_CDDREG_MAP->CTRLECC)
#define HAL_CDD_QRQ_LSB         1
#define HAL_CDD_QRQ_MSB         1
#define HAL_CDD_QRQ_MASK        0x00000002
#define HAL_CDD_QRQ_TYPE        Bool
#define HAL_CDD_QRQ             HAL_CDD_QRQ_MASK
#define HAL_CDD_QRQ_READABLE    1
#define HAL_CDD_QRQ_WRITABLE    1
#define HAL_CDD_QRQ_SIGNED      0
#define HAL_CDD_QRQ_ADR         0x1000007E

#define HAL_CDD_PRQ_REGISTER    (HAL_CDDREG_MAP->CTRLECC)
#define HAL_CDD_PRQ_LSB         0
#define HAL_CDD_PRQ_MSB         0
#define HAL_CDD_PRQ_MASK        0x00000001
#define HAL_CDD_PRQ_TYPE        Bool
#define HAL_CDD_PRQ             HAL_CDD_PRQ_MASK
#define HAL_CDD_PRQ_READABLE    1
#define HAL_CDD_PRQ_WRITABLE    1
#define HAL_CDD_PRQ_SIGNED      0
#define HAL_CDD_PRQ_ADR         0x1000007E

// Register TESTECC
#define HAL_CDD_TSBCECCGO_REGISTER    (HAL_CDDREG_MAP->TESTECC)
#define HAL_CDD_TSBCECCGO_LSB         7
#define HAL_CDD_TSBCECCGO_MSB         7
#define HAL_CDD_TSBCECCGO_MASK        0x00000080
#define HAL_CDD_TSBCECCGO_TYPE        Bool
#define HAL_CDD_TSBCECCGO             HAL_CDD_TSBCECCGO_MASK
#define HAL_CDD_TSBCECCGO_READABLE    0
#define HAL_CDD_TSBCECCGO_WRITABLE    1
#define HAL_CDD_TSBCECCGO_SIGNED      0
#define HAL_CDD_TSBCECCGO_ADR         0x1000007F

// Register STAT0
#define HAL_CDD_CRCOK_REGISTER    (HAL_CDDREG_MAP->STAT0)
#define HAL_CDD_CRCOK_LSB         7
#define HAL_CDD_CRCOK_MSB         7
#define HAL_CDD_CRCOK_MASK        0x00000080
#define HAL_CDD_CRCOK_TYPE        Bool
#define HAL_CDD_CRCOK             HAL_CDD_CRCOK_MASK
#define HAL_CDD_CRCOK_READABLE    1
#define HAL_CDD_CRCOK_WRITABLE    0
#define HAL_CDD_CRCOK_SIGNED      0
#define HAL_CDD_CRCOK_ADR         0x10000080

#define HAL_CDD_ILSYNC_REGISTER    (HAL_CDDREG_MAP->STAT0)
#define HAL_CDD_ILSYNC_LSB         6
#define HAL_CDD_ILSYNC_MSB         6
#define HAL_CDD_ILSYNC_MASK        0x00000040
#define HAL_CDD_ILSYNC_TYPE        Bool
#define HAL_CDD_ILSYNC             HAL_CDD_ILSYNC_MASK
#define HAL_CDD_ILSYNC_READABLE    1
#define HAL_CDD_ILSYNC_WRITABLE    0
#define HAL_CDD_ILSYNC_SIGNED      0
#define HAL_CDD_ILSYNC_ADR         0x10000080

#define HAL_CDD_NOCDSYNC_REGISTER    (HAL_CDDREG_MAP->STAT0)
#define HAL_CDD_NOCDSYNC_LSB         5
#define HAL_CDD_NOCDSYNC_MSB         5
#define HAL_CDD_NOCDSYNC_MASK        0x00000020
#define HAL_CDD_NOCDSYNC_TYPE        Bool
#define HAL_CDD_NOCDSYNC             HAL_CDD_NOCDSYNC_MASK
#define HAL_CDD_NOCDSYNC_READABLE    1
#define HAL_CDD_NOCDSYNC_WRITABLE    0
#define HAL_CDD_NOCDSYNC_SIGNED      0
#define HAL_CDD_NOCDSYNC_ADR         0x10000080

#define HAL_CDD_WSHORT_REGISTER    (HAL_CDDREG_MAP->STAT0)
#define HAL_CDD_WSHORT_LSB         3
#define HAL_CDD_WSHORT_MSB         3
#define HAL_CDD_WSHORT_MASK        0x00000008
#define HAL_CDD_WSHORT_TYPE        Bool
#define HAL_CDD_WSHORT             HAL_CDD_WSHORT_MASK
#define HAL_CDD_WSHORT_READABLE    1
#define HAL_CDD_WSHORT_WRITABLE    0
#define HAL_CDD_WSHORT_SIGNED      0
#define HAL_CDD_WSHORT_ADR         0x10000080

#define HAL_CDD_UCEBLK_REGISTER    (HAL_CDDREG_MAP->STAT0)
#define HAL_CDD_UCEBLK_LSB         0
#define HAL_CDD_UCEBLK_MSB         0
#define HAL_CDD_UCEBLK_MASK        0x00000001
#define HAL_CDD_UCEBLK_TYPE        Bool
#define HAL_CDD_UCEBLK             HAL_CDD_UCEBLK_MASK
#define HAL_CDD_UCEBLK_READABLE    1
#define HAL_CDD_UCEBLK_WRITABLE    0
#define HAL_CDD_UCEBLK_SIGNED      0
#define HAL_CDD_UCEBLK_ADR         0x10000080

// Register STAT1
#define HAL_CDD_HCERR_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_HCERR_LSB         7
#define HAL_CDD_HCERR_MSB         7
#define HAL_CDD_HCERR_MASK        0x00000080
#define HAL_CDD_HCERR_TYPE        Bool
#define HAL_CDD_HCERR             HAL_CDD_HCERR_MASK
#define HAL_CDD_HCERR_READABLE    1
#define HAL_CDD_HCERR_WRITABLE    0
#define HAL_CDD_HCERR_SIGNED      0
#define HAL_CDD_HCERR_ADR         0x10000081

#define HAL_CDD_MODEC_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_MODEC_LSB         6
#define HAL_CDD_MODEC_MSB         6
#define HAL_CDD_MODEC_MASK        0x00000040
#define HAL_CDD_MODEC_TYPE        Bool
#define HAL_CDD_MODEC             HAL_CDD_MODEC_MASK
#define HAL_CDD_MODEC_READABLE    1
#define HAL_CDD_MODEC_WRITABLE    0
#define HAL_CDD_MODEC_SIGNED      0
#define HAL_CDD_MODEC_ADR         0x10000081

#define HAL_CDD_FORMC_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_FORMC_LSB         5
#define HAL_CDD_FORMC_MSB         5
#define HAL_CDD_FORMC_MASK        0x00000020
#define HAL_CDD_FORMC_TYPE        Bool
#define HAL_CDD_FORMC             HAL_CDD_FORMC_MASK
#define HAL_CDD_FORMC_READABLE    1
#define HAL_CDD_FORMC_WRITABLE    0
#define HAL_CDD_FORMC_SIGNED      0
#define HAL_CDD_FORMC_ADR         0x10000081

#define HAL_CDD_HDERA_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_HDERA_LSB         4
#define HAL_CDD_HDERA_MSB         4
#define HAL_CDD_HDERA_MASK        0x00000010
#define HAL_CDD_HDERA_TYPE        Bool
#define HAL_CDD_HDERA             HAL_CDD_HDERA_MASK
#define HAL_CDD_HDERA_READABLE    1
#define HAL_CDD_HDERA_WRITABLE    0
#define HAL_CDD_HDERA_SIGNED      0
#define HAL_CDD_HDERA_ADR         0x10000081

#define HAL_CDD_YBM2_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_YBM2_LSB         3
#define HAL_CDD_YBM2_MSB         3
#define HAL_CDD_YBM2_MASK        0x00000008
#define HAL_CDD_YBM2_TYPE        Bool
#define HAL_CDD_YBM2             HAL_CDD_YBM2_MASK
#define HAL_CDD_YBM2_READABLE    1
#define HAL_CDD_YBM2_WRITABLE    0
#define HAL_CDD_YBM2_SIGNED      0
#define HAL_CDD_YBM2_ADR         0x10000081

#define HAL_CDD_SHDER1_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_SHDER1_LSB         2
#define HAL_CDD_SHDER1_MSB         2
#define HAL_CDD_SHDER1_MASK        0x00000004
#define HAL_CDD_SHDER1_TYPE        Bool
#define HAL_CDD_SHDER1             HAL_CDD_SHDER1_MASK
#define HAL_CDD_SHDER1_READABLE    1
#define HAL_CDD_SHDER1_WRITABLE    0
#define HAL_CDD_SHDER1_SIGNED      0
#define HAL_CDD_SHDER1_ADR         0x10000081

#define HAL_CDD_SHDER0_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_SHDER0_LSB         1
#define HAL_CDD_SHDER0_MSB         1
#define HAL_CDD_SHDER0_MASK        0x00000002
#define HAL_CDD_SHDER0_TYPE        Bool
#define HAL_CDD_SHDER0             HAL_CDD_SHDER0_MASK
#define HAL_CDD_SHDER0_READABLE    1
#define HAL_CDD_SHDER0_WRITABLE    0
#define HAL_CDD_SHDER0_SIGNED      0
#define HAL_CDD_SHDER0_ADR         0x10000081

#define HAL_CDD_SHDERA_REGISTER    (HAL_CDDREG_MAP->STAT1)
#define HAL_CDD_SHDERA_LSB         0
#define HAL_CDD_SHDERA_MSB         0
#define HAL_CDD_SHDERA_MASK        0x00000001
#define HAL_CDD_SHDERA_TYPE        Bool
#define HAL_CDD_SHDERA             HAL_CDD_SHDERA_MASK
#define HAL_CDD_SHDERA_READABLE    1
#define HAL_CDD_SHDERA_WRITABLE    0
#define HAL_CDD_SHDERA_SIGNED      0
#define HAL_CDD_SHDERA_ADR         0x10000081

// Register STAT2
#define HAL_CDD_DMOD_REGISTER    (HAL_CDDREG_MAP->STAT2)
#define HAL_CDD_DMOD_LSB         4
#define HAL_CDD_DMOD_MSB         7
#define HAL_CDD_DMOD_MASK        0x000000F0
#define HAL_CDD_DMOD_TYPE        halCDD_CDRawDecodeMode_t
#define HAL_CDD_DMOD             HAL_CDD_DMOD_MASK
#define HAL_CDD_DMOD_READABLE    1
#define HAL_CDD_DMOD_WRITABLE    0
#define HAL_CDD_DMOD_SIGNED      0
#define HAL_CDD_DMOD_ADR         0x10000082

#define HAL_CDD_MODE_REGISTER    (HAL_CDDREG_MAP->STAT2)
#define HAL_CDD_MODE_LSB         3
#define HAL_CDD_MODE_MSB         3
#define HAL_CDD_MODE_MASK        0x00000008
#define HAL_CDD_MODE_TYPE        Bool
#define HAL_CDD_MODE             HAL_CDD_MODE_MASK
#define HAL_CDD_MODE_READABLE    1
#define HAL_CDD_MODE_WRITABLE    0
#define HAL_CDD_MODE_SIGNED      0
#define HAL_CDD_MODE_ADR         0x10000082

#define HAL_CDD_NOCOR_REGISTER    (HAL_CDDREG_MAP->STAT2)
#define HAL_CDD_NOCOR_LSB         2
#define HAL_CDD_NOCOR_MSB         2
#define HAL_CDD_NOCOR_MASK        0x00000004
#define HAL_CDD_NOCOR_TYPE        Bool
#define HAL_CDD_NOCOR             HAL_CDD_NOCOR_MASK
#define HAL_CDD_NOCOR_READABLE    1
#define HAL_CDD_NOCOR_WRITABLE    0
#define HAL_CDD_NOCOR_SIGNED      0
#define HAL_CDD_NOCOR_ADR         0x10000082

#define HAL_CDD_FORME_REGISTER    (HAL_CDDREG_MAP->STAT2)
#define HAL_CDD_FORME_LSB         1
#define HAL_CDD_FORME_MSB         1
#define HAL_CDD_FORME_MASK        0x00000002
#define HAL_CDD_FORME_TYPE        Bool
#define HAL_CDD_FORME             HAL_CDD_FORME_MASK
#define HAL_CDD_FORME_READABLE    1
#define HAL_CDD_FORME_WRITABLE    0
#define HAL_CDD_FORME_SIGNED      0
#define HAL_CDD_FORME_ADR         0x10000082

#define HAL_CDD_RFORM_REGISTER    (HAL_CDDREG_MAP->STAT2)
#define HAL_CDD_RFORM_LSB         0
#define HAL_CDD_RFORM_MSB         0
#define HAL_CDD_RFORM_MASK        0x00000001
#define HAL_CDD_RFORM_TYPE        Bool
#define HAL_CDD_RFORM             HAL_CDD_RFORM_MASK
#define HAL_CDD_RFORM_READABLE    1
#define HAL_CDD_RFORM_WRITABLE    0
#define HAL_CDD_RFORM_SIGNED      0
#define HAL_CDD_RFORM_ADR         0x10000082

// Register STAT3
#define HAL_CDD_NOCOWR_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_NOCOWR_LSB         6
#define HAL_CDD_NOCOWR_MSB         6
#define HAL_CDD_NOCOWR_MASK        0x00000040
#define HAL_CDD_NOCOWR_TYPE        Bool
#define HAL_CDD_NOCOWR             HAL_CDD_NOCOWR_MASK
#define HAL_CDD_NOCOWR_READABLE    1
#define HAL_CDD_NOCOWR_WRITABLE    0
#define HAL_CDD_NOCOWR_SIGNED      0
#define HAL_CDD_NOCOWR_ADR         0x10000083

#define HAL_CDD_CBLK_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_CBLK_LSB         5
#define HAL_CDD_CBLK_MSB         5
#define HAL_CDD_CBLK_MASK        0x00000020
#define HAL_CDD_CBLK_TYPE        Bool
#define HAL_CDD_CBLK             HAL_CDD_CBLK_MASK
#define HAL_CDD_CBLK_READABLE    1
#define HAL_CDD_CBLK_WRITABLE    0
#define HAL_CDD_CBLK_SIGNED      0
#define HAL_CDD_CBLK_ADR         0x10000083

#define HAL_CDD_ECCINC_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_ECCINC_LSB         4
#define HAL_CDD_ECCINC_MSB         4
#define HAL_CDD_ECCINC_MASK        0x00000010
#define HAL_CDD_ECCINC_TYPE        Bool
#define HAL_CDD_ECCINC             HAL_CDD_ECCINC_MASK
#define HAL_CDD_ECCINC_READABLE    1
#define HAL_CDD_ECCINC_WRITABLE    0
#define HAL_CDD_ECCINC_SIGNED      0
#define HAL_CDD_ECCINC_ADR         0x10000083

#define HAL_CDD_EFORM_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_EFORM_LSB         3
#define HAL_CDD_EFORM_MSB         3
#define HAL_CDD_EFORM_MASK        0x00000008
#define HAL_CDD_EFORM_TYPE        Bool
#define HAL_CDD_EFORM             HAL_CDD_EFORM_MASK
#define HAL_CDD_EFORM_READABLE    1
#define HAL_CDD_EFORM_WRITABLE    0
#define HAL_CDD_EFORM_SIGNED      0
#define HAL_CDD_EFORM_ADR         0x10000083

#define HAL_CDD_MODERA_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_MODERA_LSB         2
#define HAL_CDD_MODERA_MSB         2
#define HAL_CDD_MODERA_MASK        0x00000004
#define HAL_CDD_MODERA_TYPE        Bool
#define HAL_CDD_MODERA             HAL_CDD_MODERA_MASK
#define HAL_CDD_MODERA_READABLE    1
#define HAL_CDD_MODERA_WRITABLE    0
#define HAL_CDD_MODERA_SIGNED      0
#define HAL_CDD_MODERA_ADR         0x10000083

#define HAL_CDD_C2PAROK_REGISTER    (HAL_CDDREG_MAP->STAT3)
#define HAL_CDD_C2PAROK_LSB         1
#define HAL_CDD_C2PAROK_MSB         1
#define HAL_CDD_C2PAROK_MASK        0x00000002
#define HAL_CDD_C2PAROK_TYPE        Bool
#define HAL_CDD_C2PAROK             HAL_CDD_C2PAROK_MASK
#define HAL_CDD_C2PAROK_READABLE    1
#define HAL_CDD_C2PAROK_WRITABLE    0
#define HAL_CDD_C2PAROK_SIGNED      0
#define HAL_CDD_C2PAROK_ADR         0x10000083

// Register HEAD0
#define HAL_CDD_BCDMIN_REGISTER    (HAL_CDDREG_MAP->HEAD0)
#define HAL_CDD_BCDMIN_LSB         0
#define HAL_CDD_BCDMIN_MSB         7
#define HAL_CDD_BCDMIN_MASK        0x000000FF
#define HAL_CDD_BCDMIN_TYPE        UByte
#define HAL_CDD_BCDMIN             HAL_CDD_BCDMIN_MASK
#define HAL_CDD_BCDMIN_READABLE    1
#define HAL_CDD_BCDMIN_WRITABLE    0
#define HAL_CDD_BCDMIN_SIGNED      0
#define HAL_CDD_BCDMIN_ADR         0x10000084

// Register HEAD1
#define HAL_CDD_BCDSEC_REGISTER    (HAL_CDDREG_MAP->HEAD1)
#define HAL_CDD_BCDSEC_LSB         0
#define HAL_CDD_BCDSEC_MSB         7
#define HAL_CDD_BCDSEC_MASK        0x000000FF
#define HAL_CDD_BCDSEC_TYPE        UByte
#define HAL_CDD_BCDSEC             HAL_CDD_BCDSEC_MASK
#define HAL_CDD_BCDSEC_READABLE    1
#define HAL_CDD_BCDSEC_WRITABLE    0
#define HAL_CDD_BCDSEC_SIGNED      0
#define HAL_CDD_BCDSEC_ADR         0x10000085

// Register HEAD2
#define HAL_CDD_BCDFRM_REGISTER    (HAL_CDDREG_MAP->HEAD2)
#define HAL_CDD_BCDFRM_LSB         0
#define HAL_CDD_BCDFRM_MSB         7
#define HAL_CDD_BCDFRM_MASK        0x000000FF
#define HAL_CDD_BCDFRM_TYPE        UByte
#define HAL_CDD_BCDFRM             HAL_CDD_BCDFRM_MASK
#define HAL_CDD_BCDFRM_READABLE    1
#define HAL_CDD_BCDFRM_WRITABLE    0
#define HAL_CDD_BCDFRM_SIGNED      0
#define HAL_CDD_BCDFRM_ADR         0x10000086

// Register HEAD3
#define HAL_CDD_DMODE_REGISTER    (HAL_CDDREG_MAP->HEAD3)
#define HAL_CDD_DMODE_LSB         0
#define HAL_CDD_DMODE_MSB         7
#define HAL_CDD_DMODE_MASK        0x000000FF
#define HAL_CDD_DMODE_TYPE        UByte
#define HAL_CDD_DMODE             HAL_CDD_DMODE_MASK
#define HAL_CDD_DMODE_READABLE    1
#define HAL_CDD_DMODE_WRITABLE    0
#define HAL_CDD_DMODE_SIGNED      0
#define HAL_CDD_DMODE_ADR         0x10000087

// Register LSUBH2
#define HAL_CDD_LSUBNUM_REGISTER    (HAL_CDDREG_MAP->LSUBH2)
#define HAL_CDD_LSUBNUM_LSB         0
#define HAL_CDD_LSUBNUM_MSB         7
#define HAL_CDD_LSUBNUM_MASK        0x000000FF
#define HAL_CDD_LSUBNUM_TYPE        UByte
#define HAL_CDD_LSUBNUM             HAL_CDD_LSUBNUM_MASK
#define HAL_CDD_LSUBNUM_READABLE    1
#define HAL_CDD_LSUBNUM_WRITABLE    0
#define HAL_CDD_LSUBNUM_SIGNED      0
#define HAL_CDD_LSUBNUM_ADR         0x10000088

// Register STATAM0
#define HAL_CDD_HDRUPDT_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_HDRUPDT_LSB         7
#define HAL_CDD_HDRUPDT_MSB         7
#define HAL_CDD_HDRUPDT_MASK        0x00000080
#define HAL_CDD_HDRUPDT_TYPE        Bool
#define HAL_CDD_HDRUPDT             HAL_CDD_HDRUPDT_MASK
#define HAL_CDD_HDRUPDT_READABLE    1
#define HAL_CDD_HDRUPDT_WRITABLE    0
#define HAL_CDD_HDRUPDT_SIGNED      0
#define HAL_CDD_HDRUPDT_ADR         0x1000008A

#define HAL_CDD_MFC_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_MFC_LSB         6
#define HAL_CDD_MFC_MSB         6
#define HAL_CDD_MFC_MASK        0x00000040
#define HAL_CDD_MFC_TYPE        Bool
#define HAL_CDD_MFC             HAL_CDD_MFC_MASK
#define HAL_CDD_MFC_READABLE    1
#define HAL_CDD_MFC_WRITABLE    1
#define HAL_CDD_MFC_SIGNED      0
#define HAL_CDD_MFC_ADR         0x1000008A

#define HAL_CDD_MODRQC_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_MODRQC_LSB         5
#define HAL_CDD_MODRQC_MSB         5
#define HAL_CDD_MODRQC_MASK        0x00000020
#define HAL_CDD_MODRQC_TYPE        Bool
#define HAL_CDD_MODRQC             HAL_CDD_MODRQC_MASK
#define HAL_CDD_MODRQC_READABLE    1
#define HAL_CDD_MODRQC_WRITABLE    0
#define HAL_CDD_MODRQC_SIGNED      0
#define HAL_CDD_MODRQC_ADR         0x1000008A

#define HAL_CDD_NMODBI_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_NMODBI_LSB         3
#define HAL_CDD_NMODBI_MSB         3
#define HAL_CDD_NMODBI_MASK        0x00000008
#define HAL_CDD_NMODBI_TYPE        Bool
#define HAL_CDD_NMODBI             HAL_CDD_NMODBI_MASK
#define HAL_CDD_NMODBI_READABLE    1
#define HAL_CDD_NMODBI_WRITABLE    0
#define HAL_CDD_NMODBI_SIGNED      0
#define HAL_CDD_NMODBI_ADR         0x1000008A

#define HAL_CDD_NMODERA_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_NMODERA_LSB         2
#define HAL_CDD_NMODERA_MSB         2
#define HAL_CDD_NMODERA_MASK        0x00000004
#define HAL_CDD_NMODERA_TYPE        Bool
#define HAL_CDD_NMODERA             HAL_CDD_NMODERA_MASK
#define HAL_CDD_NMODERA_READABLE    1
#define HAL_CDD_NMODERA_WRITABLE    0
#define HAL_CDD_NMODERA_SIGNED      0
#define HAL_CDD_NMODERA_ADR         0x1000008A

#define HAL_CDD_SYNCLOCK_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_SYNCLOCK_LSB         1
#define HAL_CDD_SYNCLOCK_MSB         1
#define HAL_CDD_SYNCLOCK_MASK        0x00000002
#define HAL_CDD_SYNCLOCK_TYPE        Bool
#define HAL_CDD_SYNCLOCK             HAL_CDD_SYNCLOCK_MASK
#define HAL_CDD_SYNCLOCK_READABLE    1
#define HAL_CDD_SYNCLOCK_WRITABLE    0
#define HAL_CDD_SYNCLOCK_SIGNED      0
#define HAL_CDD_SYNCLOCK_ADR         0x1000008A

#define HAL_CDD_VMSF_REGISTER    (HAL_CDDREG_MAP->STATAM0)
#define HAL_CDD_VMSF_LSB         0
#define HAL_CDD_VMSF_MSB         0
#define HAL_CDD_VMSF_MASK        0x00000001
#define HAL_CDD_VMSF_TYPE        Bool
#define HAL_CDD_VMSF             HAL_CDD_VMSF_MASK
#define HAL_CDD_VMSF_READABLE    1
#define HAL_CDD_VMSF_WRITABLE    0
#define HAL_CDD_VMSF_SIGNED      0
#define HAL_CDD_VMSF_ADR         0x1000008A

// Register DINT0
#define HAL_CDD_RO1ERR_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_RO1ERR_LSB         7
#define HAL_CDD_RO1ERR_MSB         7
#define HAL_CDD_RO1ERR_MASK        0x00000080
#define HAL_CDD_RO1ERR_TYPE        Bool
#define HAL_CDD_RO1ERR             HAL_CDD_RO1ERR_MASK
#define HAL_CDD_RO1ERR_READABLE    1
#define HAL_CDD_RO1ERR_WRITABLE    1
#define HAL_CDD_RO1ERR_SIGNED      0
#define HAL_CDD_RO1ERR_ADR         0x100000B0

#define HAL_CDD_RI4ERR_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_RI4ERR_LSB         6
#define HAL_CDD_RI4ERR_MSB         6
#define HAL_CDD_RI4ERR_MASK        0x00000040
#define HAL_CDD_RI4ERR_TYPE        Bool
#define HAL_CDD_RI4ERR             HAL_CDD_RI4ERR_MASK
#define HAL_CDD_RI4ERR_READABLE    1
#define HAL_CDD_RI4ERR_WRITABLE    1
#define HAL_CDD_RI4ERR_SIGNED      0
#define HAL_CDD_RI4ERR_ADR         0x100000B0

#define HAL_CDD_LNKA_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_LNKA_LSB         5
#define HAL_CDD_LNKA_MSB         5
#define HAL_CDD_LNKA_MASK        0x00000020
#define HAL_CDD_LNKA_TYPE        Bool
#define HAL_CDD_LNKA             HAL_CDD_LNKA_MASK
#define HAL_CDD_LNKA_READABLE    1
#define HAL_CDD_LNKA_WRITABLE    1
#define HAL_CDD_LNKA_SIGNED      0
#define HAL_CDD_LNKA_ADR         0x100000B0

#define HAL_CDD_RDMISS_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_RDMISS_LSB         4
#define HAL_CDD_RDMISS_MSB         4
#define HAL_CDD_RDMISS_MASK        0x00000010
#define HAL_CDD_RDMISS_TYPE        Bool
#define HAL_CDD_RDMISS             HAL_CDD_RDMISS_MASK
#define HAL_CDD_RDMISS_READABLE    1
#define HAL_CDD_RDMISS_WRITABLE    1
#define HAL_CDD_RDMISS_SIGNED      0
#define HAL_CDD_RDMISS_ADR         0x100000B0

#define HAL_CDD_MODERR_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_MODERR_LSB         3
#define HAL_CDD_MODERR_MSB         3
#define HAL_CDD_MODERR_MASK        0x00000008
#define HAL_CDD_MODERR_TYPE        Bool
#define HAL_CDD_MODERR             HAL_CDD_MODERR_MASK
#define HAL_CDD_MODERR_READABLE    1
#define HAL_CDD_MODERR_WRITABLE    1
#define HAL_CDD_MODERR_SIGNED      0
#define HAL_CDD_MODERR_ADR         0x100000B0

#define HAL_CDD_BFULL_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_BFULL_LSB         2
#define HAL_CDD_BFULL_MSB         2
#define HAL_CDD_BFULL_MASK        0x00000004
#define HAL_CDD_BFULL_TYPE        Bool
#define HAL_CDD_BFULL             HAL_CDD_BFULL_MASK
#define HAL_CDD_BFULL_READABLE    1
#define HAL_CDD_BFULL_WRITABLE    1
#define HAL_CDD_BFULL_SIGNED      0
#define HAL_CDD_BFULL_ADR         0x100000B0

#define HAL_CDD_MSFCERR_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_MSFCERR_LSB         1
#define HAL_CDD_MSFCERR_MSB         1
#define HAL_CDD_MSFCERR_MASK        0x00000002
#define HAL_CDD_MSFCERR_TYPE        Bool
#define HAL_CDD_MSFCERR             HAL_CDD_MSFCERR_MASK
#define HAL_CDD_MSFCERR_READABLE    1
#define HAL_CDD_MSFCERR_WRITABLE    1
#define HAL_CDD_MSFCERR_SIGNED      0
#define HAL_CDD_MSFCERR_ADR         0x100000B0

#define HAL_CDD_EDACERR_REGISTER    (HAL_CDDREG_MAP->DINT0)
#define HAL_CDD_EDACERR_LSB         0
#define HAL_CDD_EDACERR_MSB         0
#define HAL_CDD_EDACERR_MASK        0x00000001
#define HAL_CDD_EDACERR_TYPE        Bool
#define HAL_CDD_EDACERR             HAL_CDD_EDACERR_MASK
#define HAL_CDD_EDACERR_READABLE    1
#define HAL_CDD_EDACERR_WRITABLE    1
#define HAL_CDD_EDACERR_SIGNED      0
#define HAL_CDD_EDACERR_ADR         0x100000B0

// Register DINT1
#define HAL_CDD_QADDR0_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_QADDR0_LSB         7
#define HAL_CDD_QADDR0_MSB         7
#define HAL_CDD_QADDR0_MASK        0x00000080
#define HAL_CDD_QADDR0_TYPE        Bool
#define HAL_CDD_QADDR0             HAL_CDD_QADDR0_MASK
#define HAL_CDD_QADDR0_READABLE    1
#define HAL_CDD_QADDR0_WRITABLE    1
#define HAL_CDD_QADDR0_SIGNED      0
#define HAL_CDD_QADDR0_ADR         0x100000B1

#define HAL_CDD_HDRERR_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_HDRERR_LSB         6
#define HAL_CDD_HDRERR_MSB         6
#define HAL_CDD_HDRERR_MASK        0x00000040
#define HAL_CDD_HDRERR_TYPE        Bool
#define HAL_CDD_HDRERR             HAL_CDD_HDRERR_MASK
#define HAL_CDD_HDRERR_READABLE    1
#define HAL_CDD_HDRERR_WRITABLE    1
#define HAL_CDD_HDRERR_SIGNED      0
#define HAL_CDD_HDRERR_ADR         0x100000B1

#define HAL_CDD_SRWERR_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_SRWERR_LSB         5
#define HAL_CDD_SRWERR_MSB         5
#define HAL_CDD_SRWERR_MASK        0x00000020
#define HAL_CDD_SRWERR_TYPE        Bool
#define HAL_CDD_SRWERR             HAL_CDD_SRWERR_MASK
#define HAL_CDD_SRWERR_READABLE    1
#define HAL_CDD_SRWERR_WRITABLE    1
#define HAL_CDD_SRWERR_SIGNED      0
#define HAL_CDD_SRWERR_ADR         0x100000B1

#define HAL_CDD_MISSS_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_MISSS_LSB         4
#define HAL_CDD_MISSS_MSB         4
#define HAL_CDD_MISSS_MASK        0x00000010
#define HAL_CDD_MISSS_TYPE        Bool
#define HAL_CDD_MISSS             HAL_CDD_MISSS_MASK
#define HAL_CDD_MISSS_READABLE    1
#define HAL_CDD_MISSS_WRITABLE    1
#define HAL_CDD_MISSS_SIGNED      0
#define HAL_CDD_MISSS_ADR         0x100000B1

#define HAL_CDD_DC2ERR_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_DC2ERR_LSB         3
#define HAL_CDD_DC2ERR_MSB         3
#define HAL_CDD_DC2ERR_MASK        0x00000008
#define HAL_CDD_DC2ERR_TYPE        Bool
#define HAL_CDD_DC2ERR             HAL_CDD_DC2ERR_MASK
#define HAL_CDD_DC2ERR_READABLE    1
#define HAL_CDD_DC2ERR_WRITABLE    1
#define HAL_CDD_DC2ERR_SIGNED      0
#define HAL_CDD_DC2ERR_ADR         0x100000B1

#define HAL_CDD_ILSS_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_ILSS_LSB         2
#define HAL_CDD_ILSS_MSB         2
#define HAL_CDD_ILSS_MASK        0x00000004
#define HAL_CDD_ILSS_TYPE        Bool
#define HAL_CDD_ILSS             HAL_CDD_ILSS_MASK
#define HAL_CDD_ILSS_READABLE    1
#define HAL_CDD_ILSS_WRITABLE    1
#define HAL_CDD_ILSS_SIGNED      0
#define HAL_CDD_ILSS_ADR         0x100000B1

#define HAL_CDD_DIFERR_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_DIFERR_LSB         1
#define HAL_CDD_DIFERR_MSB         1
#define HAL_CDD_DIFERR_MASK        0x00000002
#define HAL_CDD_DIFERR_TYPE        Bool
#define HAL_CDD_DIFERR             HAL_CDD_DIFERR_MASK
#define HAL_CDD_DIFERR_READABLE    1
#define HAL_CDD_DIFERR_WRITABLE    1
#define HAL_CDD_DIFERR_SIGNED      0
#define HAL_CDD_DIFERR_ADR         0x100000B1

#define HAL_CDD_NOSYNC_REGISTER    (HAL_CDDREG_MAP->DINT1)
#define HAL_CDD_NOSYNC_LSB         0
#define HAL_CDD_NOSYNC_MSB         0
#define HAL_CDD_NOSYNC_MASK        0x00000001
#define HAL_CDD_NOSYNC_TYPE        Bool
#define HAL_CDD_NOSYNC             HAL_CDD_NOSYNC_MASK
#define HAL_CDD_NOSYNC_READABLE    1
#define HAL_CDD_NOSYNC_WRITABLE    1
#define HAL_CDD_NOSYNC_SIGNED      0
#define HAL_CDD_NOSYNC_ADR         0x100000B1

// Register DINT2
#define HAL_CDD_BSYNC_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_BSYNC_LSB         7
#define HAL_CDD_BSYNC_MSB         7
#define HAL_CDD_BSYNC_MASK        0x00000080
#define HAL_CDD_BSYNC_TYPE        Bool
#define HAL_CDD_BSYNC             HAL_CDD_BSYNC_MASK
#define HAL_CDD_BSYNC_READABLE    1
#define HAL_CDD_BSYNC_WRITABLE    1
#define HAL_CDD_BSYNC_SIGNED      0
#define HAL_CDD_BSYNC_ADR         0x100000B2

#define HAL_CDD_ROUT1_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_ROUT1_LSB         6
#define HAL_CDD_ROUT1_MSB         6
#define HAL_CDD_ROUT1_MASK        0x00000040
#define HAL_CDD_ROUT1_TYPE        Bool
#define HAL_CDD_ROUT1             HAL_CDD_ROUT1_MASK
#define HAL_CDD_ROUT1_READABLE    1
#define HAL_CDD_ROUT1_WRITABLE    1
#define HAL_CDD_ROUT1_SIGNED      0
#define HAL_CDD_ROUT1_ADR         0x100000B2

#define HAL_CDD_SUBRDY_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_SUBRDY_LSB         5
#define HAL_CDD_SUBRDY_MSB         5
#define HAL_CDD_SUBRDY_MASK        0x00000020
#define HAL_CDD_SUBRDY_TYPE        Bool
#define HAL_CDD_SUBRDY             HAL_CDD_SUBRDY_MASK
#define HAL_CDD_SUBRDY_READABLE    1
#define HAL_CDD_SUBRDY_WRITABLE    1
#define HAL_CDD_SUBRDY_SIGNED      0
#define HAL_CDD_SUBRDY_ADR         0x100000B2

#define HAL_CDD_HDRDY_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_HDRDY_LSB         4
#define HAL_CDD_HDRDY_MSB         4
#define HAL_CDD_HDRDY_MASK        0x00000010
#define HAL_CDD_HDRDY_TYPE        Bool
#define HAL_CDD_HDRDY             HAL_CDD_HDRDY_MASK
#define HAL_CDD_HDRDY_READABLE    1
#define HAL_CDD_HDRDY_WRITABLE    1
#define HAL_CDD_HDRDY_SIGNED      0
#define HAL_CDD_HDRDY_ADR         0x100000B2

#define HAL_CDD_RIN4_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_RIN4_LSB         3
#define HAL_CDD_RIN4_MSB         3
#define HAL_CDD_RIN4_MASK        0x00000008
#define HAL_CDD_RIN4_TYPE        Bool
#define HAL_CDD_RIN4             HAL_CDD_RIN4_MASK
#define HAL_CDD_RIN4_READABLE    1
#define HAL_CDD_RIN4_WRITABLE    1
#define HAL_CDD_RIN4_SIGNED      0
#define HAL_CDD_RIN4_ADR         0x100000B2

#define HAL_CDD_TFND_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_TFND_LSB         2
#define HAL_CDD_TFND_MSB         2
#define HAL_CDD_TFND_MASK        0x00000004
#define HAL_CDD_TFND_TYPE        Bool
#define HAL_CDD_TFND             HAL_CDD_TFND_MASK
#define HAL_CDD_TFND_READABLE    1
#define HAL_CDD_TFND_WRITABLE    1
#define HAL_CDD_TFND_SIGNED      0
#define HAL_CDD_TFND_ADR         0x100000B2

#define HAL_CDD_DPRD_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_DPRD_LSB         1
#define HAL_CDD_DPRD_MSB         1
#define HAL_CDD_DPRD_MASK        0x00000002
#define HAL_CDD_DPRD_TYPE        Bool
#define HAL_CDD_DPRD             HAL_CDD_DPRD_MASK
#define HAL_CDD_DPRD_READABLE    1
#define HAL_CDD_DPRD_WRITABLE    1
#define HAL_CDD_DPRD_SIGNED      0
#define HAL_CDD_DPRD_ADR         0x100000B2

#define HAL_CDD_QADDR23_REGISTER    (HAL_CDDREG_MAP->DINT2)
#define HAL_CDD_QADDR23_LSB         0
#define HAL_CDD_QADDR23_MSB         0
#define HAL_CDD_QADDR23_MASK        0x00000001
#define HAL_CDD_QADDR23_TYPE        Bool
#define HAL_CDD_QADDR23             HAL_CDD_QADDR23_MASK
#define HAL_CDD_QADDR23_READABLE    1
#define HAL_CDD_QADDR23_WRITABLE    1
#define HAL_CDD_QADDR23_SIGNED      0
#define HAL_CDD_QADDR23_ADR         0x100000B2

// Register DINTEN0
#define HAL_CDD_RO1ERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_RO1ERREN_LSB         7
#define HAL_CDD_RO1ERREN_MSB         7
#define HAL_CDD_RO1ERREN_MASK        0x00000080
#define HAL_CDD_RO1ERREN_TYPE        Bool
#define HAL_CDD_RO1ERREN             HAL_CDD_RO1ERREN_MASK
#define HAL_CDD_RO1ERREN_READABLE    1
#define HAL_CDD_RO1ERREN_WRITABLE    1
#define HAL_CDD_RO1ERREN_SIGNED      0
#define HAL_CDD_RO1ERREN_ADR         0x100000B4

#define HAL_CDD_R14ERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_R14ERREN_LSB         6
#define HAL_CDD_R14ERREN_MSB         6
#define HAL_CDD_R14ERREN_MASK        0x00000040
#define HAL_CDD_R14ERREN_TYPE        Bool
#define HAL_CDD_R14ERREN             HAL_CDD_R14ERREN_MASK
#define HAL_CDD_R14ERREN_READABLE    1
#define HAL_CDD_R14ERREN_WRITABLE    1
#define HAL_CDD_R14ERREN_SIGNED      0
#define HAL_CDD_R14ERREN_ADR         0x100000B4

#define HAL_CDD_LNKAEN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_LNKAEN_LSB         5
#define HAL_CDD_LNKAEN_MSB         5
#define HAL_CDD_LNKAEN_MASK        0x00000020
#define HAL_CDD_LNKAEN_TYPE        Bool
#define HAL_CDD_LNKAEN             HAL_CDD_LNKAEN_MASK
#define HAL_CDD_LNKAEN_READABLE    1
#define HAL_CDD_LNKAEN_WRITABLE    1
#define HAL_CDD_LNKAEN_SIGNED      0
#define HAL_CDD_LNKAEN_ADR         0x100000B4

#define HAL_CDD_RDMISSEN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_RDMISSEN_LSB         4
#define HAL_CDD_RDMISSEN_MSB         4
#define HAL_CDD_RDMISSEN_MASK        0x00000010
#define HAL_CDD_RDMISSEN_TYPE        Bool
#define HAL_CDD_RDMISSEN             HAL_CDD_RDMISSEN_MASK
#define HAL_CDD_RDMISSEN_READABLE    1
#define HAL_CDD_RDMISSEN_WRITABLE    1
#define HAL_CDD_RDMISSEN_SIGNED      0
#define HAL_CDD_RDMISSEN_ADR         0x100000B4

#define HAL_CDD_MODERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_MODERREN_LSB         3
#define HAL_CDD_MODERREN_MSB         3
#define HAL_CDD_MODERREN_MASK        0x00000008
#define HAL_CDD_MODERREN_TYPE        Bool
#define HAL_CDD_MODERREN             HAL_CDD_MODERREN_MASK
#define HAL_CDD_MODERREN_READABLE    1
#define HAL_CDD_MODERREN_WRITABLE    1
#define HAL_CDD_MODERREN_SIGNED      0
#define HAL_CDD_MODERREN_ADR         0x100000B4

#define HAL_CDD_BFULLEN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_BFULLEN_LSB         2
#define HAL_CDD_BFULLEN_MSB         2
#define HAL_CDD_BFULLEN_MASK        0x00000004
#define HAL_CDD_BFULLEN_TYPE        Bool
#define HAL_CDD_BFULLEN             HAL_CDD_BFULLEN_MASK
#define HAL_CDD_BFULLEN_READABLE    1
#define HAL_CDD_BFULLEN_WRITABLE    1
#define HAL_CDD_BFULLEN_SIGNED      0
#define HAL_CDD_BFULLEN_ADR         0x100000B4

#define HAL_CDD_MSFCERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_MSFCERREN_LSB         1
#define HAL_CDD_MSFCERREN_MSB         1
#define HAL_CDD_MSFCERREN_MASK        0x00000002
#define HAL_CDD_MSFCERREN_TYPE        Bool
#define HAL_CDD_MSFCERREN             HAL_CDD_MSFCERREN_MASK
#define HAL_CDD_MSFCERREN_READABLE    1
#define HAL_CDD_MSFCERREN_WRITABLE    1
#define HAL_CDD_MSFCERREN_SIGNED      0
#define HAL_CDD_MSFCERREN_ADR         0x100000B4

#define HAL_CDD_EDACERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN0)
#define HAL_CDD_EDACERREN_LSB         0
#define HAL_CDD_EDACERREN_MSB         0
#define HAL_CDD_EDACERREN_MASK        0x00000001
#define HAL_CDD_EDACERREN_TYPE        Bool
#define HAL_CDD_EDACERREN             HAL_CDD_EDACERREN_MASK
#define HAL_CDD_EDACERREN_READABLE    1
#define HAL_CDD_EDACERREN_WRITABLE    1
#define HAL_CDD_EDACERREN_SIGNED      0
#define HAL_CDD_EDACERREN_ADR         0x100000B4

// Register DINTEN1
#define HAL_CDD_QADDR0EN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_QADDR0EN_LSB         7
#define HAL_CDD_QADDR0EN_MSB         7
#define HAL_CDD_QADDR0EN_MASK        0x00000080
#define HAL_CDD_QADDR0EN_TYPE        Bool
#define HAL_CDD_QADDR0EN             HAL_CDD_QADDR0EN_MASK
#define HAL_CDD_QADDR0EN_READABLE    1
#define HAL_CDD_QADDR0EN_WRITABLE    1
#define HAL_CDD_QADDR0EN_SIGNED      0
#define HAL_CDD_QADDR0EN_ADR         0x100000B5

#define HAL_CDD_HDRERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_HDRERREN_LSB         6
#define HAL_CDD_HDRERREN_MSB         6
#define HAL_CDD_HDRERREN_MASK        0x00000040
#define HAL_CDD_HDRERREN_TYPE        Bool
#define HAL_CDD_HDRERREN             HAL_CDD_HDRERREN_MASK
#define HAL_CDD_HDRERREN_READABLE    1
#define HAL_CDD_HDRERREN_WRITABLE    1
#define HAL_CDD_HDRERREN_SIGNED      0
#define HAL_CDD_HDRERREN_ADR         0x100000B5

#define HAL_CDD_SRWERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_SRWERREN_LSB         5
#define HAL_CDD_SRWERREN_MSB         5
#define HAL_CDD_SRWERREN_MASK        0x00000020
#define HAL_CDD_SRWERREN_TYPE        Bool
#define HAL_CDD_SRWERREN             HAL_CDD_SRWERREN_MASK
#define HAL_CDD_SRWERREN_READABLE    1
#define HAL_CDD_SRWERREN_WRITABLE    1
#define HAL_CDD_SRWERREN_SIGNED      0
#define HAL_CDD_SRWERREN_ADR         0x100000B5

#define HAL_CDD_MISSSEN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_MISSSEN_LSB         4
#define HAL_CDD_MISSSEN_MSB         4
#define HAL_CDD_MISSSEN_MASK        0x00000010
#define HAL_CDD_MISSSEN_TYPE        Bool
#define HAL_CDD_MISSSEN             HAL_CDD_MISSSEN_MASK
#define HAL_CDD_MISSSEN_READABLE    1
#define HAL_CDD_MISSSEN_WRITABLE    1
#define HAL_CDD_MISSSEN_SIGNED      0
#define HAL_CDD_MISSSEN_ADR         0x100000B5

#define HAL_CDD_DC2ERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_DC2ERREN_LSB         3
#define HAL_CDD_DC2ERREN_MSB         3
#define HAL_CDD_DC2ERREN_MASK        0x00000008
#define HAL_CDD_DC2ERREN_TYPE        Bool
#define HAL_CDD_DC2ERREN             HAL_CDD_DC2ERREN_MASK
#define HAL_CDD_DC2ERREN_READABLE    1
#define HAL_CDD_DC2ERREN_WRITABLE    1
#define HAL_CDD_DC2ERREN_SIGNED      0
#define HAL_CDD_DC2ERREN_ADR         0x100000B5

#define HAL_CDD_ILSSEN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_ILSSEN_LSB         2
#define HAL_CDD_ILSSEN_MSB         2
#define HAL_CDD_ILSSEN_MASK        0x00000004
#define HAL_CDD_ILSSEN_TYPE        Bool
#define HAL_CDD_ILSSEN             HAL_CDD_ILSSEN_MASK
#define HAL_CDD_ILSSEN_READABLE    1
#define HAL_CDD_ILSSEN_WRITABLE    1
#define HAL_CDD_ILSSEN_SIGNED      0
#define HAL_CDD_ILSSEN_ADR         0x100000B5

#define HAL_CDD_DIFERREN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_DIFERREN_LSB         1
#define HAL_CDD_DIFERREN_MSB         1
#define HAL_CDD_DIFERREN_MASK        0x00000002
#define HAL_CDD_DIFERREN_TYPE        Bool
#define HAL_CDD_DIFERREN             HAL_CDD_DIFERREN_MASK
#define HAL_CDD_DIFERREN_READABLE    1
#define HAL_CDD_DIFERREN_WRITABLE    1
#define HAL_CDD_DIFERREN_SIGNED      0
#define HAL_CDD_DIFERREN_ADR         0x100000B5

#define HAL_CDD_NOSYNCEN_REGISTER    (HAL_CDDREG_MAP->DINTEN1)
#define HAL_CDD_NOSYNCEN_LSB         0
#define HAL_CDD_NOSYNCEN_MSB         0
#define HAL_CDD_NOSYNCEN_MASK        0x00000001
#define HAL_CDD_NOSYNCEN_TYPE        Bool
#define HAL_CDD_NOSYNCEN             HAL_CDD_NOSYNCEN_MASK
#define HAL_CDD_NOSYNCEN_READABLE    1
#define HAL_CDD_NOSYNCEN_WRITABLE    1
#define HAL_CDD_NOSYNCEN_SIGNED      0
#define HAL_CDD_NOSYNCEN_ADR         0x100000B5

// Register DINTEN2
#define HAL_CDD_BSYNCEN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_BSYNCEN_LSB         7
#define HAL_CDD_BSYNCEN_MSB         7
#define HAL_CDD_BSYNCEN_MASK        0x00000080
#define HAL_CDD_BSYNCEN_TYPE        Bool
#define HAL_CDD_BSYNCEN             HAL_CDD_BSYNCEN_MASK
#define HAL_CDD_BSYNCEN_READABLE    1
#define HAL_CDD_BSYNCEN_WRITABLE    1
#define HAL_CDD_BSYNCEN_SIGNED      0
#define HAL_CDD_BSYNCEN_ADR         0x100000B6

#define HAL_CDD_ROUT1EN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_ROUT1EN_LSB         6
#define HAL_CDD_ROUT1EN_MSB         6
#define HAL_CDD_ROUT1EN_MASK        0x00000040
#define HAL_CDD_ROUT1EN_TYPE        Bool
#define HAL_CDD_ROUT1EN             HAL_CDD_ROUT1EN_MASK
#define HAL_CDD_ROUT1EN_READABLE    1
#define HAL_CDD_ROUT1EN_WRITABLE    1
#define HAL_CDD_ROUT1EN_SIGNED      0
#define HAL_CDD_ROUT1EN_ADR         0x100000B6

#define HAL_CDD_SUBRDYEN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_SUBRDYEN_LSB         5
#define HAL_CDD_SUBRDYEN_MSB         5
#define HAL_CDD_SUBRDYEN_MASK        0x00000020
#define HAL_CDD_SUBRDYEN_TYPE        Bool
#define HAL_CDD_SUBRDYEN             HAL_CDD_SUBRDYEN_MASK
#define HAL_CDD_SUBRDYEN_READABLE    1
#define HAL_CDD_SUBRDYEN_WRITABLE    1
#define HAL_CDD_SUBRDYEN_SIGNED      0
#define HAL_CDD_SUBRDYEN_ADR         0x100000B6

#define HAL_CDD_HDRDYEN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_HDRDYEN_LSB         4
#define HAL_CDD_HDRDYEN_MSB         4
#define HAL_CDD_HDRDYEN_MASK        0x00000010
#define HAL_CDD_HDRDYEN_TYPE        Bool
#define HAL_CDD_HDRDYEN             HAL_CDD_HDRDYEN_MASK
#define HAL_CDD_HDRDYEN_READABLE    1
#define HAL_CDD_HDRDYEN_WRITABLE    1
#define HAL_CDD_HDRDYEN_SIGNED      0
#define HAL_CDD_HDRDYEN_ADR         0x100000B6

#define HAL_CDD_RIN4EN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_RIN4EN_LSB         3
#define HAL_CDD_RIN4EN_MSB         3
#define HAL_CDD_RIN4EN_MASK        0x00000008
#define HAL_CDD_RIN4EN_TYPE        Bool
#define HAL_CDD_RIN4EN             HAL_CDD_RIN4EN_MASK
#define HAL_CDD_RIN4EN_READABLE    1
#define HAL_CDD_RIN4EN_WRITABLE    1
#define HAL_CDD_RIN4EN_SIGNED      0
#define HAL_CDD_RIN4EN_ADR         0x100000B6

#define HAL_CDD_TFNDEN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_TFNDEN_LSB         2
#define HAL_CDD_TFNDEN_MSB         2
#define HAL_CDD_TFNDEN_MASK        0x00000004
#define HAL_CDD_TFNDEN_TYPE        Bool
#define HAL_CDD_TFNDEN             HAL_CDD_TFNDEN_MASK
#define HAL_CDD_TFNDEN_READABLE    1
#define HAL_CDD_TFNDEN_WRITABLE    1
#define HAL_CDD_TFNDEN_SIGNED      0
#define HAL_CDD_TFNDEN_ADR         0x100000B6

#define HAL_CDD_DPRDEN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_DPRDEN_LSB         1
#define HAL_CDD_DPRDEN_MSB         1
#define HAL_CDD_DPRDEN_MASK        0x00000002
#define HAL_CDD_DPRDEN_TYPE        Bool
#define HAL_CDD_DPRDEN             HAL_CDD_DPRDEN_MASK
#define HAL_CDD_DPRDEN_READABLE    1
#define HAL_CDD_DPRDEN_WRITABLE    1
#define HAL_CDD_DPRDEN_SIGNED      0
#define HAL_CDD_DPRDEN_ADR         0x100000B6

#define HAL_CDD_QADDR23EN_REGISTER    (HAL_CDDREG_MAP->DINTEN2)
#define HAL_CDD_QADDR23EN_LSB         0
#define HAL_CDD_QADDR23EN_MSB         0
#define HAL_CDD_QADDR23EN_MASK        0x00000001
#define HAL_CDD_QADDR23EN_TYPE        Bool
#define HAL_CDD_QADDR23EN             HAL_CDD_QADDR23EN_MASK
#define HAL_CDD_QADDR23EN_READABLE    1
#define HAL_CDD_QADDR23EN_WRITABLE    1
#define HAL_CDD_QADDR23EN_SIGNED      0
#define HAL_CDD_QADDR23EN_ADR         0x100000B6

// Register TSTWRCE
#define HAL_CDD_TSTWRCE_REGISTER    (HAL_CDDREG_MAP->TSTWRCE)
#define HAL_CDD_TSTWRCE_LSB         0
#define HAL_CDD_TSTWRCE_MSB         7
#define HAL_CDD_TSTWRCE_MASK        0x000000FF
#define HAL_CDD_TSTWRCE_TYPE        UByte
#define HAL_CDD_TSTWRCE             HAL_CDD_TSTWRCE_MASK
#define HAL_CDD_TSTWRCE_READABLE    0
#define HAL_CDD_TSTWRCE_WRITABLE    1
#define HAL_CDD_TSTWRCE_SIGNED      0
#define HAL_CDD_TSTWRCE_ADR         0x100000BE

// Register SUBQ0
#define HAL_CDD_SUBQ0_REGISTER    (HAL_CDDREG_MAP->SUBQ0)
#define HAL_CDD_SUBQ0_LSB         0
#define HAL_CDD_SUBQ0_MSB         7
#define HAL_CDD_SUBQ0_MASK        0x000000FF
#define HAL_CDD_SUBQ0_TYPE        UByte
#define HAL_CDD_SUBQ0             HAL_CDD_SUBQ0_MASK
#define HAL_CDD_SUBQ0_READABLE    1
#define HAL_CDD_SUBQ0_WRITABLE    0
#define HAL_CDD_SUBQ0_SIGNED      0
#define HAL_CDD_SUBQ0_ADR         0x100000C0

// Register SUBQ1
#define HAL_CDD_SUBQ1_REGISTER    (HAL_CDDREG_MAP->SUBQ1)
#define HAL_CDD_SUBQ1_LSB         0
#define HAL_CDD_SUBQ1_MSB         7
#define HAL_CDD_SUBQ1_MASK        0x000000FF
#define HAL_CDD_SUBQ1_TYPE        UByte
#define HAL_CDD_SUBQ1             HAL_CDD_SUBQ1_MASK
#define HAL_CDD_SUBQ1_READABLE    1
#define HAL_CDD_SUBQ1_WRITABLE    0
#define HAL_CDD_SUBQ1_SIGNED      0
#define HAL_CDD_SUBQ1_ADR         0x100000C1

// Register SUBQ2
#define HAL_CDD_SUBQ2_REGISTER    (HAL_CDDREG_MAP->SUBQ2)
#define HAL_CDD_SUBQ2_LSB         0
#define HAL_CDD_SUBQ2_MSB         7
#define HAL_CDD_SUBQ2_MASK        0x000000FF
#define HAL_CDD_SUBQ2_TYPE        UByte
#define HAL_CDD_SUBQ2             HAL_CDD_SUBQ2_MASK
#define HAL_CDD_SUBQ2_READABLE    1
#define HAL_CDD_SUBQ2_WRITABLE    0
#define HAL_CDD_SUBQ2_SIGNED      0
#define HAL_CDD_SUBQ2_ADR         0x100000C2

// Register SUBQ3
#define HAL_CDD_SUBQ3_REGISTER    (HAL_CDDREG_MAP->SUBQ3)
#define HAL_CDD_SUBQ3_LSB         0
#define HAL_CDD_SUBQ3_MSB         7
#define HAL_CDD_SUBQ3_MASK        0x000000FF
#define HAL_CDD_SUBQ3_TYPE        UByte
#define HAL_CDD_SUBQ3             HAL_CDD_SUBQ3_MASK
#define HAL_CDD_SUBQ3_READABLE    1
#define HAL_CDD_SUBQ3_WRITABLE    0
#define HAL_CDD_SUBQ3_SIGNED      0
#define HAL_CDD_SUBQ3_ADR         0x100000C3

// Register SUBQ4
#define HAL_CDD_SUBQ4_REGISTER    (HAL_CDDREG_MAP->SUBQ4)
#define HAL_CDD_SUBQ4_LSB         0
#define HAL_CDD_SUBQ4_MSB         7
#define HAL_CDD_SUBQ4_MASK        0x000000FF
#define HAL_CDD_SUBQ4_TYPE        UByte
#define HAL_CDD_SUBQ4             HAL_CDD_SUBQ4_MASK
#define HAL_CDD_SUBQ4_READABLE    1
#define HAL_CDD_SUBQ4_WRITABLE    0
#define HAL_CDD_SUBQ4_SIGNED      0
#define HAL_CDD_SUBQ4_ADR         0x100000C4

// Register SUBQ5
#define HAL_CDD_SUBQ5_REGISTER    (HAL_CDDREG_MAP->SUBQ5)
#define HAL_CDD_SUBQ5_LSB         0
#define HAL_CDD_SUBQ5_MSB         7
#define HAL_CDD_SUBQ5_MASK        0x000000FF
#define HAL_CDD_SUBQ5_TYPE        UByte
#define HAL_CDD_SUBQ5             HAL_CDD_SUBQ5_MASK
#define HAL_CDD_SUBQ5_READABLE    1
#define HAL_CDD_SUBQ5_WRITABLE    0
#define HAL_CDD_SUBQ5_SIGNED      0
#define HAL_CDD_SUBQ5_ADR         0x100000C5

// Register SUBQ6
#define HAL_CDD_SUBQ6_REGISTER    (HAL_CDDREG_MAP->SUBQ6)
#define HAL_CDD_SUBQ6_LSB         0
#define HAL_CDD_SUBQ6_MSB         7
#define HAL_CDD_SUBQ6_MASK        0x000000FF
#define HAL_CDD_SUBQ6_TYPE        UByte
#define HAL_CDD_SUBQ6             HAL_CDD_SUBQ6_MASK
#define HAL_CDD_SUBQ6_READABLE    1
#define HAL_CDD_SUBQ6_WRITABLE    0
#define HAL_CDD_SUBQ6_SIGNED      0
#define HAL_CDD_SUBQ6_ADR         0x100000C6

// Register SUBQ7
#define HAL_CDD_SUBQ7_REGISTER    (HAL_CDDREG_MAP->SUBQ7)
#define HAL_CDD_SUBQ7_LSB         0
#define HAL_CDD_SUBQ7_MSB         7
#define HAL_CDD_SUBQ7_MASK        0x000000FF
#define HAL_CDD_SUBQ7_TYPE        UByte
#define HAL_CDD_SUBQ7             HAL_CDD_SUBQ7_MASK
#define HAL_CDD_SUBQ7_READABLE    1
#define HAL_CDD_SUBQ7_WRITABLE    0
#define HAL_CDD_SUBQ7_SIGNED      0
#define HAL_CDD_SUBQ7_ADR         0x100000C7

// Register SUBQ8
#define HAL_CDD_SUBQ8_REGISTER    (HAL_CDDREG_MAP->SUBQ8)
#define HAL_CDD_SUBQ8_LSB         0
#define HAL_CDD_SUBQ8_MSB         7
#define HAL_CDD_SUBQ8_MASK        0x000000FF
#define HAL_CDD_SUBQ8_TYPE        UByte
#define HAL_CDD_SUBQ8             HAL_CDD_SUBQ8_MASK
#define HAL_CDD_SUBQ8_READABLE    1
#define HAL_CDD_SUBQ8_WRITABLE    0
#define HAL_CDD_SUBQ8_SIGNED      0
#define HAL_CDD_SUBQ8_ADR         0x100000C8

// Register SUBQ9
#define HAL_CDD_SUBQ9_REGISTER    (HAL_CDDREG_MAP->SUBQ9)
#define HAL_CDD_SUBQ9_LSB         0
#define HAL_CDD_SUBQ9_MSB         7
#define HAL_CDD_SUBQ9_MASK        0x000000FF
#define HAL_CDD_SUBQ9_TYPE        UByte
#define HAL_CDD_SUBQ9             HAL_CDD_SUBQ9_MASK
#define HAL_CDD_SUBQ9_READABLE    1
#define HAL_CDD_SUBQ9_WRITABLE    0
#define HAL_CDD_SUBQ9_SIGNED      0
#define HAL_CDD_SUBQ9_ADR         0x100000C9

// Register SQSTAT
#define HAL_CDD_SQCRCOK_REGISTER    (HAL_CDDREG_MAP->SQSTAT)
#define HAL_CDD_SQCRCOK_LSB         0
#define HAL_CDD_SQCRCOK_MSB         0
#define HAL_CDD_SQCRCOK_MASK        0x00000001
#define HAL_CDD_SQCRCOK_TYPE        Bool
#define HAL_CDD_SQCRCOK             HAL_CDD_SQCRCOK_MASK
#define HAL_CDD_SQCRCOK_READABLE    1
#define HAL_CDD_SQCRCOK_WRITABLE    0
#define HAL_CDD_SQCRCOK_SIGNED      0
#define HAL_CDD_SQCRCOK_ADR         0x100000CA

// Registers DSKBUFSPGL/DSKBUFSPGH
#define HAL_CDD_DSKBUFSPG_REGISTER    (HAL_CDDREG_MAP->DSKBUFSPGL)
#define HAL_CDD_DSKBUFSPG_LSB         0
#define HAL_CDD_DSKBUFSPG_MSB         12
#define HAL_CDD_DSKBUFSPG_MASK        0x00001FFF
#define HAL_CDD_DSKBUFSPG_TYPE        UInt16
#define HAL_CDD_DSKBUFSPG             HAL_CDD_DSKBUFSPG_MASK
#define HAL_CDD_DSKBUFSPG_READABLE    1
#define HAL_CDD_DSKBUFSPG_WRITABLE    1
#define HAL_CDD_DSKBUFSPG_SIGNED      0
#define HAL_CDD_DSKBUFSPG_ADR         0x100000D4

// Registers DSKBUFEPGL/DSKBUFEPGH
#define HAL_CDD_DSKBUFEPG_REGISTER    (HAL_CDDREG_MAP->DSKBUFEPGL)
#define HAL_CDD_DSKBUFEPG_LSB         0
#define HAL_CDD_DSKBUFEPG_MSB         12
#define HAL_CDD_DSKBUFEPG_MASK        0x00001FFF
#define HAL_CDD_DSKBUFEPG_TYPE        UInt16
#define HAL_CDD_DSKBUFEPG             HAL_CDD_DSKBUFEPG_MASK
#define HAL_CDD_DSKBUFEPG_READABLE    1
#define HAL_CDD_DSKBUFEPG_WRITABLE    1
#define HAL_CDD_DSKBUFEPG_SIGNED      0
#define HAL_CDD_DSKBUFEPG_ADR         0x100000D6

// Registers DSUBPGL/DSUBPGH
#define HAL_CDD_DSUBPG_REGISTER    (HAL_CDDREG_MAP->DSUBPGL)
#define HAL_CDD_DSUBPG_LSB         0
#define HAL_CDD_DSUBPG_MSB         12
#define HAL_CDD_DSUBPG_MASK        0x00001FFF
#define HAL_CDD_DSUBPG_TYPE        UInt16
#define HAL_CDD_DSUBPG             HAL_CDD_DSUBPG_MASK
#define HAL_CDD_DSUBPG_READABLE    1
#define HAL_CDD_DSUBPG_WRITABLE    1
#define HAL_CDD_DSUBPG_SIGNED      0
#define HAL_CDD_DSUBPG_ADR         0x100000E4

// Registers DECPGL/DECPGH
#define HAL_CDD_DECPG_REGISTER    (HAL_CDDREG_MAP->DECPGL)
#define HAL_CDD_DECPG_LSB         0
#define HAL_CDD_DECPG_MSB         12
#define HAL_CDD_DECPG_MASK        0x00001FFF
#define HAL_CDD_DECPG_TYPE        UInt16
#define HAL_CDD_DECPG             HAL_CDD_DECPG_MASK
#define HAL_CDD_DECPG_READABLE    1
#define HAL_CDD_DECPG_WRITABLE    1
#define HAL_CDD_DECPG_SIGNED      0
#define HAL_CDD_DECPG_ADR         0x100000E6

// Registers LDECPGL/LDECPGH
#define HAL_CDD_LDECPG_REGISTER    (HAL_CDDREG_MAP->LDECPGL)
#define HAL_CDD_LDECPG_LSB         0
#define HAL_CDD_LDECPG_MSB         12
#define HAL_CDD_LDECPG_MASK        0x00001FFF
#define HAL_CDD_LDECPG_TYPE        UInt16
#define HAL_CDD_LDECPG             HAL_CDD_LDECPG_MASK
#define HAL_CDD_LDECPG_READABLE    1
#define HAL_CDD_LDECPG_WRITABLE    0
#define HAL_CDD_LDECPG_SIGNED      0
#define HAL_CDD_LDECPG_ADR         0x100000E8

// Register MEMCF
#define HAL_CDD_MLY_REGISTER    (HAL_CDDREG_MAP->MEMCF)
#define HAL_CDD_MLY_LSB         0
#define HAL_CDD_MLY_MSB         1
#define HAL_CDD_MLY_MASK        0x00000003
#define HAL_CDD_MLY_TYPE        halCDD_BufPageSize_t
#define HAL_CDD_MLY             HAL_CDD_MLY_MASK
#define HAL_CDD_MLY_READABLE    1
#define HAL_CDD_MLY_WRITABLE    1
#define HAL_CDD_MLY_SIGNED      0
#define HAL_CDD_MLY_ADR         0x100000EB

#if (CHIP_REV <= 0xB3)

// Register CDD64DEB
#define HAL_CDD_CDD64DEB_REGISTER    (HAL_CDDREG_MAP->CDDDBGBUS)
#define HAL_CDD_CDD64DEB_LSB         0
#define HAL_CDD_CDD64DEB_MSB         1
#define HAL_CDD_CDD64DEB_MASK        0x00000003
#define HAL_CDD_CDD64DEB_TYPE        halCDD_64bit_DebugBusCtrl_t
#define HAL_CDD_CDD64DEB             HAL_CDD_CDD64DEB_MASK
#define HAL_CDD_CDD64DEB_READABLE    1
#define HAL_CDD_CDD64DEB_WRITABLE    1
#define HAL_CDD_CDD64DEB_SIGNED      0
#define HAL_CDD_CDD64DEB_ADR         0x100000FA

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

// Register CDDDBGBUS
#define HAL_CDD_CDDDBGSEL_REGISTER    (HAL_CDDREG_MAP->CDDDBGBUS)
#define HAL_CDD_CDDDBGSEL_LSB         0
#define HAL_CDD_CDDDBGSEL_MSB         2
#define HAL_CDD_CDDDBGSEL_MASK        0x00000007
#define HAL_CDD_CDDDBGSEL_TYPE        halCDD_DebugBusCtrl_t
#define HAL_CDD_CDDDBGSEL             HAL_CDD_CDDDBGSEL_MASK
#define HAL_CDD_CDDDBGSEL_READABLE    1
#define HAL_CDD_CDDDBGSEL_WRITABLE    1
#define HAL_CDD_CDDDBGSEL_SIGNED      0
#define HAL_CDD_CDDDBGSEL_ADR         0x100000FA

#endif // #if (CHIP_REV >= 0xC0)

// Registers DCPGOFL/DCPGOFH
#define HAL_CDD_DCPGOF_REGISTER    (HAL_CDDREG_MAP->DCPGOFL)
#define HAL_CDD_DCPGOF_LSB         1
#define HAL_CDD_DCPGOF_MSB         15
#define HAL_CDD_DCPGOF_MASK        0x0000FFFE
#define HAL_CDD_DCPGOF_TYPE        UInt16
#define HAL_CDD_DCPGOF             HAL_CDD_DCPGOF_MASK
#define HAL_CDD_DCPGOF_READABLE    1
#define HAL_CDD_DCPGOF_WRITABLE    1
#define HAL_CDD_DCPGOF_SIGNED      0
#define HAL_CDD_DCPGOF_ADR         0x100000FC

// Registers PTL/PTH
#define HAL_CDD_PT_REGISTER    (HAL_CDDREG_MAP->PTL)
#define HAL_CDD_PT_LSB         0
#define HAL_CDD_PT_MSB         15
#define HAL_CDD_PT_MASK        0x0000FFFF
#define HAL_CDD_PT_TYPE        UInt16
#define HAL_CDD_PT             HAL_CDD_PT_MASK
#define HAL_CDD_PT_READABLE    1
#define HAL_CDD_PT_WRITABLE    1
#define HAL_CDD_PT_SIGNED      0
#define HAL_CDD_PT_ADR         0x100000FE

// Enumerations
typedef enum
{
	halCDD_C2CG_SEL0 = 0,    // 4 symbol C2 error correction using C1 erasure info
	halCDD_C2CG_SEL1 = 1,    // 3 symbol C2 error correction using C1 erasure info
	halCDD_C2CG_SEL2 = 2,    // SRCH = 0, 4 symbol C2 error correction using C1 erasure info; SRCH = 1, 2 symbol C2 error correction
	halCDD_C2CG_SEL3 = 3     // SRCH = 0, 3 symbol C2 error correction using C1 erasure info; SRCH = 1, 2 symbol C2 error correction
} halCDD_C2ErrCorrect_t;

typedef enum
{
	halCDD_MUTM_MUTEC   = 0,    // The block decoder is forced to zero according to MUTEC
	halCDD_MUTM_NoMUTEC = 1,    // Disable MUTEC functionality
	halCDD_MUTM_Zero    = 2     // The block decoder is forced to zero even after MUTEC returns to 0
} halCDD_MuteCtrl_t;

typedef enum
{
	halCDD_C2P_LongParity = 0,    // C2PARCNT(7-0) is the longitudinal parity of the C2 pointers
	halCDD_C2P_OR         = 1,    // C2PARCNT(7-0) is the OR of all C2 error bytes
	halCDD_C2P_ErrCnt     = 2     // C@PARCNT(7-0) is the C2 pointer error count (up to 255 errors)
} halCDD_C2PCtrl_t;

typedef enum
{
	halCDD_R2WECC_NoRetry = 0,    // Subchannel R-W ECC retries disabled
	halCDD_R2WECC_1Retry  = 1,    // One retry of R-W ECC
	halCDD_R2WECC_2Retry  = 2,    // Up to two retries of R-W ECC
	halCDD_R2WECC_3Retry  = 3     // Up to three retries of R-W ECC
} halCDD_R2WECCRetry_t;

typedef enum
{
	halCDD_CDMode_Mode0 = 1,    // Mode 0
	halCDD_CDMode_Mode1 = 2,    // Mode 1
	halCDD_CDMode_Mode2 = 4,    // Mode 2
	halCDD_CDMode_Error = 15     // Error
} halCDD_CDRawDecodeMode_t;

typedef enum
{
	halCDD_BufPage_3KB   = 2,    // Each buffer page contains data (2kB) and auziliary (1kB) sections
	halCDD_BufPage_2P5KB = 3     // Each buffer page contains data (2kB) and suxiliary (512B) sections
} halCDD_BufPageSize_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halCDD_DebugBus64_SEL0 = 0,
	halCDD_DebugBus64_SEL1 = 1,
	halCDD_DebugBus64_SEL2 = 2,
	halCDD_DebugBus64_SEL3 = 3 
} halCDD_64bit_DebugBusCtrl_t;

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halCDD_CDDDbgSel_CDDSP    = 0,
	halCDD_CDDDbgSel_CDDSLOT1 = 1,
	halCDD_CDDDbgSel_CDDSLOT2 = 2,
	halCDD_CDDDbgSel_C3BUSR   = 3,
	halCDD_CDDDbgSel_C3BUSW   = 4,
	halCDD_CDDDbgSel_DBG64SL1 = 5,
	halCDD_CDDDbgSel_DBG64SL2 = 6,
	halCDD_CDDDbgSel_C3ECC    = 7 
} halCDD_DebugBusCtrl_t;

#endif // #if (CHIP_REV >= 0xC0)

#endif /* __REG_CD_DECODER_H__ */

