/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_afe_logic.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the AFE core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_AFE_LOGIC_H__
#define __REG_AFE_LOGIC_H__

// Core register map
#define HAL_AFE_VERSION 7.69
#define HAL_AFE_BaseAddress 0x10000900
#define HAL_AFEREG_MAP ((struct halAFEReg_Map*) (void *) HAL_AFE_BaseAddress)
__packed struct halAFEReg_Map
{
	HAL_Reg8  MUX00;                          //00000000
	HAL_Reg8  MUX01;                          //00000001
	HAL_Reg8  MUX02;                          //00000002
	HAL_Reg8  MUX03;                          //00000003
	HAL_Reg8  MUX04;                          //00000004
	HAL_Reg8  MUX05;                          //00000005
	HAL_Reg8  BSRP00;                         //00000006
	HAL_Reg8  BSRP01;                         //00000007
	HAL_Reg8  BSRP02;                         //00000008
	HAL_Reg8  BSRP03;                         //00000009
	HAL_Reg8  BSRP04;                         //0000000A
	HAL_Reg8  BSRP05;                         //0000000B
	HAL_Reg8  BSRP06;                         //0000000C
	HAL_Reg8  BSRP07;                         //0000000D
	HAL_Reg8  BSRP08;                         //0000000E
	HAL_Reg8  BSRP09;                         //0000000F
	HAL_Reg8  BSRP10;                         //00000010
	HAL_Reg8  BSRP11;                         //00000011
	HAL_Reg8  BSRP12;                         //00000012
	HAL_Reg8  BSRP13;                         //00000013
	HAL_Reg8  BSRP14;                         //00000014
	HAL_Reg8  BSRP15;                         //00000015
	HAL_Reg8  BSRP16;                         //00000016
	HAL_Reg8  BSRP17;                         //00000017
	HAL_Reg8  BSRP18;                         //00000018
	HAL_Reg8  BSRP19;                         //00000019
	HAL_Reg8  BSRP20;                         //0000001A
	HAL_Reg8  BSRP21;                         //0000001B
	HAL_Reg8  FCTS00;                         //0000001C
	HAL_Reg8  FCTS01;                         //0000001D
	HAL_Reg8  FCTS02;                         //0000001E
	HAL_Reg8  FCTS03;                         //0000001F
	HAL_Reg8  FCTS04;                         //00000020
	HAL_Reg8  FCTS05;                         //00000021
	HAL_Reg8  FCTS06;                         //00000022
	HAL_Reg8  FCTS07;                         //00000023
	HAL_Reg8  FCTS08;                         //00000024
	HAL_Reg8  FCTS09;                         //00000025
	HAL_Reg8  FCTS10;                         //00000026
	HAL_Reg8  FCTS11;                         //00000027
	HAL_Reg8  FCTS12;                         //00000028
	HAL_Reg8  FCTS13;                         //00000029
	HAL_Reg8  FCTS14;                         //0000002A
	HAL_Reg8  FCTS15;                         //0000002B
	HAL_Reg8  FCTS16;                         //0000002C
	HAL_Reg8  FCTS17;                         //0000002D
	HAL_Reg8  FCTS18;                         //0000002E
	HAL_Reg8  FCTS19;                         //0000002F
	HAL_Reg8  FCTS20;                         //00000030
	HAL_Reg8  FCTS21;                         //00000031
	HAL_Reg8  FCTS22;                         //00000032
	HAL_Reg8  FCTS23;                         //00000033
	HAL_Reg8  FCTS24;                         //00000034
	HAL_Reg8  FCTS25;                         //00000035
	HAL_Reg8  FCTS26;                         //00000036
	HAL_Reg8  FCTS27;                         //00000037
	HAL_Reg8  FCTS28;                         //00000038
	HAL_Reg8  FCTS29;                         //00000039
	HAL_Reg8  FCTS30;                         //0000003A
	HAL_Reg8  FCTS31;                         //0000003B
	HAL_Reg8  FCTS32;                         //0000003C
	HAL_Reg8  FCTS33;                         //0000003D
	HAL_Reg8  FCTS34;                         //0000003E
	HAL_Reg8  FCTS35;                         //0000003F
	HAL_Reg8  FCTS36;                         //00000040
	HAL_Reg8  FCTS37;                         //00000041
	HAL_Reg8  FCTS38;                         //00000042
	HAL_Reg8  FCTS39;                         //00000043
	HAL_Reg8  FCTS40;                         //00000044
	HAL_Reg8  MB00;                           //00000045
	HAL_Reg8  MB01;                           //00000046
	HAL_Reg8  MB02;                           //00000047
	HAL_Reg8  MB03;                           //00000048
	HAL_Reg8  MB04;                           //00000049
	HAL_Reg8  MB05;                           //0000004A
	HAL_Reg8  MB06;                           //0000004B
	HAL_Reg8  MB07;                           //0000004C
	HAL_Reg8  MB08;                           //0000004D
	HAL_Reg8  MB09;                           //0000004E
	HAL_Reg8  MB10;                           //0000004F
	HAL_Reg8  MB11;                           //00000050
	HAL_Reg8  MB12;                           //00000051
	HAL_Reg8  MB13;                           //00000052
	HAL_Reg8  MB14;                           //00000053
	HAL_Reg8  MB15;                           //00000054
	HAL_Reg8  MB16;                           //00000055
	HAL_Reg8  MB17;                           //00000056
	HAL_Reg8  MB18;                           //00000057
	HAL_Reg8  MB19;                           //00000058
	HAL_Reg8  MB20;                           //00000059
	HAL_Reg8  SB00;                           //0000005A
	HAL_Reg8  SB01;                           //0000005B
	HAL_Reg8  SB02;                           //0000005C
	HAL_Reg8  SB03;                           //0000005D
	HAL_Reg8  SB04;                           //0000005E
	HAL_Reg8  SB05;                           //0000005F
	HAL_Reg8  SB06;                           //00000060
	HAL_Reg8  SB07;                           //00000061
	HAL_Reg8  SB08;                           //00000062
	HAL_Reg8  SB09;                           //00000063
	HAL_Reg8  SB10;                           //00000064
	HAL_Reg8  SB11;                           //00000065
	HAL_Reg8  SB12;                           //00000066
	HAL_Reg8  SB13;                           //00000067
	HAL_Reg8  SB14;                           //00000068
	HAL_Reg8  SB15;                           //00000069
	HAL_Reg8  SB16;                           //0000006A
	HAL_Reg8  SB17;                           //0000006B
	HAL_Reg8  SB18;                           //0000006C
	HAL_Reg8  SB19;                           //0000006D
	HAL_Reg8  REFS00;                         //0000006E
	HAL_Reg8  REFS01;                         //0000006F
	HAL_Reg8  RF00;                           //00000070
	HAL_Reg8  RF01;                           //00000071
	HAL_Reg8  RF02;                           //00000072
	HAL_Reg8  RF03;                           //00000073
	HAL_Reg8  RF04;                           //00000074
	HAL_Reg8  RF05;                           //00000075
	HAL_Reg8  RF06;                           //00000076
	HAL_Reg8  RF07;                           //00000077
	HAL_Reg8  RF08;                           //00000078
	HAL_Reg8  RF09;                           //00000079
	HAL_Reg8  RF10;                           //0000007A
	HAL_Reg8  RF11;                           //0000007B
	HAL_Reg8  RF12;                           //0000007C
	HAL_Reg8  RF13;                           //0000007D
	HAL_Reg8  RF14;                           //0000007E
	HAL_Reg8  RF15;                           //0000007F
	HAL_Reg8  RF16;                           //00000080
	HAL_Reg8  RF17;                           //00000081
	HAL_Reg8  RF18;                           //00000082
	HAL_Reg8  RF19;                           //00000083
	HAL_Reg8  RF20;                           //00000084
	HAL_Reg8  RF21;                           //00000085
	HAL_Reg8  RF22;                           //00000086
	HAL_Reg8  RF23;                           //00000087
	HAL_Reg8  RF24;                           //00000088
	HAL_Reg8  RF25;                           //00000089
	HAL_Reg8  WOBAPC00;                       //0000008A
	HAL_Reg8  WOBAPC01;                       //0000008B
	HAL_Reg8  WOBAPC02;                       //0000008C
	HAL_Reg8  WOBAPC03;                       //0000008D
	HAL_Reg8  WOBAPC04;                       //0000008E
	HAL_Reg8  WOBAPC05;                       //0000008F
	HAL_Reg8  WOBAPC06;                       //00000090
	HAL_Reg8  WOBAPC07;                       //00000091
	HAL_Reg8  WOBAPC08;                       //00000092
	HAL_Reg8  WOBAPC09;                       //00000093
	HAL_Reg8  WOBAPC10;                       //00000094
	HAL_Reg8  WOBAPC11;                       //00000095
	HAL_Reg8  WOBAPC12;                       //00000096
	HAL_Reg8  WOBAPC13;                       //00000097
	HAL_Reg8  WOBAPC14;                       //00000098
	HAL_Reg8  WOBAPC15;                       //00000099
	HAL_Reg8  WOBAPC16;                       //0000009A
	HAL_Reg8  WOBAPC17;                       //0000009B
	HAL_Reg8  WOBAPC18;                       //0000009C
	HAL_Reg8  WOBAPC19;                       //0000009D
	HAL_Reg8  WOBAPC20;                       //0000009E
	HAL_Reg8  WOBAPC21;                       //0000009F
	HAL_Reg8  WOBAPC22;                       //000000A0
	HAL_Reg8  WOBAPC23;                       //000000A1
	HAL_Reg8  WOBAPC24;                       //000000A2
	HAL_Reg8  AVGSVOSET1;                     //000000A3
	HAL_Reg8  AVGSVOSET2;                     //000000A4
	HAL_Reg8  AVGSVOSET3;                     //000000A5
	HAL_Reg8  AVGSVOSET4;                     //000000A6
	HAL_Reg8  TARGETBS;                       //000000A7
	HAL_Reg8  DFCTDLY;                        //000000A8
	HAL_Reg8  BSRP22;                         //000000A9
	HAL_Reg8  BSRP23;                         //000000AA
	HAL_Reg8  BSRP24;                         //000000AB
	HAL_Reg8  BSRP25;                         //000000AC
	HAL_Reg8  BSRP26;                         //000000AD
	HAL_Reg8  BSRP27;                         //000000AE
	HAL_Reg8  FCTS41;                         //000000AF
	HAL_Reg8  FCTS42;                         //000000B0
	HAL_Reg8  SB20;                           //000000B1
	HAL_Reg8  SB21;                           //000000B2
	HAL_Reg8  MB21;                           //000000B3
	HAL_Reg8  MB22;                           //000000B4
	HAL_Reg8  ReservedBlk0[43];               //000000B5
	HAL_Reg8  TESTENA1;                       //000000E0
	HAL_Reg8  TESTENA2;                       //000000E1
	HAL_Reg8  TESTVAL1;                       //000000E2
	HAL_Reg8  TESTVAL2;                       //000000E3
	HAL_Reg8  AFEMIOSEL;                      //000000E4
	HAL_Reg8  ReservedBlk1[1];                //000000E5
	HAL_Reg8  WOBAPC25;                       //000000E6
	HAL_Reg8  WOBAPC26;                       //000000E7
	HAL_Reg8  WOBAPC27;                       //000000E8
	HAL_Reg8  ReservedBlk2[3];                //000000E9
	HAL_Reg8  MUX06;                          //000000EC
	HAL_Reg8  MUX07;                          //000000ED
	HAL_Reg8  MUX08;                          //000000EE
};
// Register MUX00
#define HAL_AFE_MUXTP1SEL_REGISTER    (HAL_AFEREG_MAP->MUX00)
#define HAL_AFE_MUXTP1SEL_LSB         4
#define HAL_AFE_MUXTP1SEL_MSB         5
#define HAL_AFE_MUXTP1SEL_MASK        0x00000030
#define HAL_AFE_MUXTP1SEL_TYPE        halAFE_MUXTP1SEL_t
#define HAL_AFE_MUXTP1SEL             HAL_AFE_MUXTP1SEL_MASK
#define HAL_AFE_MUXTP1SEL_READABLE    1
#define HAL_AFE_MUXTP1SEL_WRITABLE    1
#define HAL_AFE_MUXTP1SEL_SIGNED      0
#define HAL_AFE_MUXTP1SEL_ADR         0x10000900

#define HAL_AFE_MUXTP2SEL_REGISTER    (HAL_AFEREG_MAP->MUX00)
#define HAL_AFE_MUXTP2SEL_LSB         0
#define HAL_AFE_MUXTP2SEL_MSB         1
#define HAL_AFE_MUXTP2SEL_MASK        0x00000003
#define HAL_AFE_MUXTP2SEL_TYPE        halAFE_MUXTP2SEL_t
#define HAL_AFE_MUXTP2SEL             HAL_AFE_MUXTP2SEL_MASK
#define HAL_AFE_MUXTP2SEL_READABLE    1
#define HAL_AFE_MUXTP2SEL_WRITABLE    1
#define HAL_AFE_MUXTP2SEL_SIGNED      0
#define HAL_AFE_MUXTP2SEL_ADR         0x10000900

// Register MUX01

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_MPXO1SEL_REGISTER    (HAL_AFEREG_MAP->MUX01)
#define HAL_AFE_MPXO1SEL_LSB         6
#define HAL_AFE_MPXO1SEL_MSB         7
#define HAL_AFE_MPXO1SEL_MASK        0x000000C0
#define HAL_AFE_MPXO1SEL_TYPE        halAFE_MPXO1SEL_t
#define HAL_AFE_MPXO1SEL             HAL_AFE_MPXO1SEL_MASK
#define HAL_AFE_MPXO1SEL_READABLE    1
#define HAL_AFE_MPXO1SEL_WRITABLE    1
#define HAL_AFE_MPXO1SEL_SIGNED      0
#define HAL_AFE_MPXO1SEL_ADR         0x10000901

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_MUX1SEL_REGISTER    (HAL_AFEREG_MAP->MUX01)
#define HAL_AFE_MUX1SEL_LSB         0
#define HAL_AFE_MUX1SEL_MSB         4
#define HAL_AFE_MUX1SEL_MASK        0x0000001F
#define HAL_AFE_MUX1SEL_TYPE        halAFE_MUX1SEL_t
#define HAL_AFE_MUX1SEL             HAL_AFE_MUX1SEL_MASK
#define HAL_AFE_MUX1SEL_READABLE    1
#define HAL_AFE_MUX1SEL_WRITABLE    1
#define HAL_AFE_MUX1SEL_SIGNED      0
#define HAL_AFE_MUX1SEL_ADR         0x10000901

// Register MUX02

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_MPXO2SEL_REGISTER    (HAL_AFEREG_MAP->MUX02)
#define HAL_AFE_MPXO2SEL_LSB         6
#define HAL_AFE_MPXO2SEL_MSB         7
#define HAL_AFE_MPXO2SEL_MASK        0x000000C0
#define HAL_AFE_MPXO2SEL_TYPE        halAFE_MPXO2SEL_t
#define HAL_AFE_MPXO2SEL             HAL_AFE_MPXO2SEL_MASK
#define HAL_AFE_MPXO2SEL_READABLE    1
#define HAL_AFE_MPXO2SEL_WRITABLE    1
#define HAL_AFE_MPXO2SEL_SIGNED      0
#define HAL_AFE_MPXO2SEL_ADR         0x10000902

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_MUX2SEL_REGISTER    (HAL_AFEREG_MAP->MUX02)
#define HAL_AFE_MUX2SEL_LSB         0
#define HAL_AFE_MUX2SEL_MSB         4
#define HAL_AFE_MUX2SEL_MASK        0x0000001F
#define HAL_AFE_MUX2SEL_TYPE        halAFE_MUX2SEL_t
#define HAL_AFE_MUX2SEL             HAL_AFE_MUX2SEL_MASK
#define HAL_AFE_MUX2SEL_READABLE    1
#define HAL_AFE_MUX2SEL_WRITABLE    1
#define HAL_AFE_MUX2SEL_SIGNED      0
#define HAL_AFE_MUX2SEL_ADR         0x10000902

// Register MUX03

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_MPXO3SEL_REGISTER    (HAL_AFEREG_MAP->MUX03)
#define HAL_AFE_MPXO3SEL_LSB         6
#define HAL_AFE_MPXO3SEL_MSB         7
#define HAL_AFE_MPXO3SEL_MASK        0x000000C0
#define HAL_AFE_MPXO3SEL_TYPE        halAFE_MPXO3SEL_t
#define HAL_AFE_MPXO3SEL             HAL_AFE_MPXO3SEL_MASK
#define HAL_AFE_MPXO3SEL_READABLE    1
#define HAL_AFE_MPXO3SEL_WRITABLE    1
#define HAL_AFE_MPXO3SEL_SIGNED      0
#define HAL_AFE_MPXO3SEL_ADR         0x10000903

#define HAL_AFE_MX3TOGP1_REGISTER    (HAL_AFEREG_MAP->MUX03)
#define HAL_AFE_MX3TOGP1_LSB         5
#define HAL_AFE_MX3TOGP1_MSB         5
#define HAL_AFE_MX3TOGP1_MASK        0x00000020
#define HAL_AFE_MX3TOGP1_TYPE        Bool
#define HAL_AFE_MX3TOGP1             HAL_AFE_MX3TOGP1_MASK
#define HAL_AFE_MX3TOGP1_READABLE    1
#define HAL_AFE_MX3TOGP1_WRITABLE    1
#define HAL_AFE_MX3TOGP1_SIGNED      0
#define HAL_AFE_MX3TOGP1_ADR         0x10000903

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_MUX3SEL_REGISTER    (HAL_AFEREG_MAP->MUX03)
#define HAL_AFE_MUX3SEL_LSB         0
#define HAL_AFE_MUX3SEL_MSB         4
#define HAL_AFE_MUX3SEL_MASK        0x0000001F
#define HAL_AFE_MUX3SEL_TYPE        halAFE_MUX3SEL_t
#define HAL_AFE_MUX3SEL             HAL_AFE_MUX3SEL_MASK
#define HAL_AFE_MUX3SEL_READABLE    1
#define HAL_AFE_MUX3SEL_WRITABLE    1
#define HAL_AFE_MUX3SEL_SIGNED      0
#define HAL_AFE_MUX3SEL_ADR         0x10000903

// Register MUX04
#define HAL_AFE_MUX1ADSEL_REGISTER    (HAL_AFEREG_MAP->MUX04)
#define HAL_AFE_MUX1ADSEL_LSB         0
#define HAL_AFE_MUX1ADSEL_MSB         4
#define HAL_AFE_MUX1ADSEL_MASK        0x0000001F
#define HAL_AFE_MUX1ADSEL_TYPE        halAFE_MUX1ADSEL_t
#define HAL_AFE_MUX1ADSEL             HAL_AFE_MUX1ADSEL_MASK
#define HAL_AFE_MUX1ADSEL_READABLE    1
#define HAL_AFE_MUX1ADSEL_WRITABLE    1
#define HAL_AFE_MUX1ADSEL_SIGNED      0
#define HAL_AFE_MUX1ADSEL_ADR         0x10000904

// Register MUX05
#define HAL_AFE_MUX2ADSEL_REGISTER    (HAL_AFEREG_MAP->MUX05)
#define HAL_AFE_MUX2ADSEL_LSB         0
#define HAL_AFE_MUX2ADSEL_MSB         4
#define HAL_AFE_MUX2ADSEL_MASK        0x0000001F
#define HAL_AFE_MUX2ADSEL_TYPE        halAFE_MUX2ADSEL_t
#define HAL_AFE_MUX2ADSEL             HAL_AFE_MUX2ADSEL_MASK
#define HAL_AFE_MUX2ADSEL_READABLE    1
#define HAL_AFE_MUX2ADSEL_WRITABLE    1
#define HAL_AFE_MUX2ADSEL_SIGNED      0
#define HAL_AFE_MUX2ADSEL_ADR         0x10000905

// Register BSRP00
#define HAL_AFE_BS2OSA_REGISTER    (HAL_AFEREG_MAP->BSRP00)
#define HAL_AFE_BS2OSA_LSB         0
#define HAL_AFE_BS2OSA_MSB         7
#define HAL_AFE_BS2OSA_MASK        0x000000FF
#define HAL_AFE_BS2OSA_TYPE        UByte
#define HAL_AFE_BS2OSA             HAL_AFE_BS2OSA_MASK
#define HAL_AFE_BS2OSA_READABLE    1
#define HAL_AFE_BS2OSA_WRITABLE    1
#define HAL_AFE_BS2OSA_SIGNED      0
#define HAL_AFE_BS2OSA_ADR         0x10000906

// Register BSRP01
#define HAL_AFE_BS2OSB_REGISTER    (HAL_AFEREG_MAP->BSRP01)
#define HAL_AFE_BS2OSB_LSB         0
#define HAL_AFE_BS2OSB_MSB         7
#define HAL_AFE_BS2OSB_MASK        0x000000FF
#define HAL_AFE_BS2OSB_TYPE        UByte
#define HAL_AFE_BS2OSB             HAL_AFE_BS2OSB_MASK
#define HAL_AFE_BS2OSB_READABLE    1
#define HAL_AFE_BS2OSB_WRITABLE    1
#define HAL_AFE_BS2OSB_SIGNED      0
#define HAL_AFE_BS2OSB_ADR         0x10000907

// Register BSRP02
#define HAL_AFE_BS2AUTOEN_REGISTER    (HAL_AFEREG_MAP->BSRP02)
#define HAL_AFE_BS2AUTOEN_LSB         7
#define HAL_AFE_BS2AUTOEN_MSB         7
#define HAL_AFE_BS2AUTOEN_MASK        0x00000080
#define HAL_AFE_BS2AUTOEN_TYPE        Bool
#define HAL_AFE_BS2AUTOEN             HAL_AFE_BS2AUTOEN_MASK
#define HAL_AFE_BS2AUTOEN_READABLE    1
#define HAL_AFE_BS2AUTOEN_WRITABLE    1
#define HAL_AFE_BS2AUTOEN_SIGNED      0
#define HAL_AFE_BS2AUTOEN_ADR         0x10000908

#define HAL_AFE_RXTOK_REGISTER    (HAL_AFEREG_MAP->BSRP02)
#define HAL_AFE_RXTOK_LSB         6
#define HAL_AFE_RXTOK_MSB         6
#define HAL_AFE_RXTOK_MASK        0x00000040
#define HAL_AFE_RXTOK_TYPE        Bool
#define HAL_AFE_RXTOK             HAL_AFE_RXTOK_MASK
#define HAL_AFE_RXTOK_READABLE    1
#define HAL_AFE_RXTOK_WRITABLE    1
#define HAL_AFE_RXTOK_SIGNED      0
#define HAL_AFE_RXTOK_ADR         0x10000908

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RPPOLINV_REGISTER    (HAL_AFEREG_MAP->BSRP02)
#define HAL_AFE_RPPOLINV_LSB         5
#define HAL_AFE_RPPOLINV_MSB         5
#define HAL_AFE_RPPOLINV_MASK        0x00000020
#define HAL_AFE_RPPOLINV_TYPE        Bool
#define HAL_AFE_RPPOLINV             HAL_AFE_RPPOLINV_MASK
#define HAL_AFE_RPPOLINV_READABLE    1
#define HAL_AFE_RPPOLINV_WRITABLE    1
#define HAL_AFE_RPPOLINV_SIGNED      0
#define HAL_AFE_RPPOLINV_ADR         0x10000908

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_BS2GNREG_REGISTER    (HAL_AFEREG_MAP->BSRP02)
#define HAL_AFE_BS2GNREG_LSB         0
#define HAL_AFE_BS2GNREG_MSB         4
#define HAL_AFE_BS2GNREG_MASK        0x0000001F
#define HAL_AFE_BS2GNREG_TYPE        UByte
#define HAL_AFE_BS2GNREG             HAL_AFE_BS2GNREG_MASK
#define HAL_AFE_BS2GNREG_READABLE    1
#define HAL_AFE_BS2GNREG_WRITABLE    1
#define HAL_AFE_BS2GNREG_SIGNED      0
#define HAL_AFE_BS2GNREG_ADR         0x10000908

// Register BSRP03
#define HAL_AFE_PHSDSCKOF_REGISTER    (HAL_AFEREG_MAP->BSRP03)
#define HAL_AFE_PHSDSCKOF_LSB         7
#define HAL_AFE_PHSDSCKOF_MSB         7
#define HAL_AFE_PHSDSCKOF_MASK        0x00000080
#define HAL_AFE_PHSDSCKOF_TYPE        Bool
#define HAL_AFE_PHSDSCKOF             HAL_AFE_PHSDSCKOF_MASK
#define HAL_AFE_PHSDSCKOF_READABLE    1
#define HAL_AFE_PHSDSCKOF_WRITABLE    1
#define HAL_AFE_PHSDSCKOF_SIGNED      0
#define HAL_AFE_PHSDSCKOF_ADR         0x10000909

#define HAL_AFE_PHSDISCK_REGISTER    (HAL_AFEREG_MAP->BSRP03)
#define HAL_AFE_PHSDISCK_LSB         0
#define HAL_AFE_PHSDISCK_MSB         3
#define HAL_AFE_PHSDISCK_MASK        0x0000000F
#define HAL_AFE_PHSDISCK_TYPE        halAFE_PHDRateDec_t
#define HAL_AFE_PHSDISCK             HAL_AFE_PHSDISCK_MASK
#define HAL_AFE_PHSDISCK_READABLE    1
#define HAL_AFE_PHSDISCK_WRITABLE    1
#define HAL_AFE_PHSDISCK_SIGNED      0
#define HAL_AFE_PHSDISCK_ADR         0x10000909

// Register BSRP04
#define HAL_AFE_BHSDSCKOF_REGISTER    (HAL_AFEREG_MAP->BSRP04)
#define HAL_AFE_BHSDSCKOF_LSB         7
#define HAL_AFE_BHSDSCKOF_MSB         7
#define HAL_AFE_BHSDSCKOF_MASK        0x00000080
#define HAL_AFE_BHSDSCKOF_TYPE        Bool
#define HAL_AFE_BHSDSCKOF             HAL_AFE_BHSDSCKOF_MASK
#define HAL_AFE_BHSDSCKOF_READABLE    1
#define HAL_AFE_BHSDSCKOF_WRITABLE    1
#define HAL_AFE_BHSDSCKOF_SIGNED      0
#define HAL_AFE_BHSDSCKOF_ADR         0x1000090A

#define HAL_AFE_BHSDISCK_REGISTER    (HAL_AFEREG_MAP->BSRP04)
#define HAL_AFE_BHSDISCK_LSB         0
#define HAL_AFE_BHSDISCK_MSB         3
#define HAL_AFE_BHSDISCK_MASK        0x0000000F
#define HAL_AFE_BHSDISCK_TYPE        halAFE_BHDRateDec_t
#define HAL_AFE_BHSDISCK             HAL_AFE_BHSDISCK_MASK
#define HAL_AFE_BHSDISCK_READABLE    1
#define HAL_AFE_BHSDISCK_WRITABLE    1
#define HAL_AFE_BHSDISCK_SIGNED      0
#define HAL_AFE_BHSDISCK_ADR         0x1000090A

// Register BSRP05
#define HAL_AFE_BSSRF_REGISTER    (HAL_AFEREG_MAP->BSRP05)
#define HAL_AFE_BSSRF_LSB         7
#define HAL_AFE_BSSRF_MSB         7
#define HAL_AFE_BSSRF_MASK        0x00000080
#define HAL_AFE_BSSRF_TYPE        Bool
#define HAL_AFE_BSSRF             HAL_AFE_BSSRF_MASK
#define HAL_AFE_BSSRF_READABLE    1
#define HAL_AFE_BSSRF_WRITABLE    1
#define HAL_AFE_BSSRF_SIGNED      0
#define HAL_AFE_BSSRF_ADR         0x1000090B

#define HAL_AFE_BSSBA_REGISTER    (HAL_AFEREG_MAP->BSRP05)
#define HAL_AFE_BSSBA_LSB         6
#define HAL_AFE_BSSBA_MSB         6
#define HAL_AFE_BSSBA_MASK        0x00000040
#define HAL_AFE_BSSBA_TYPE        Bool
#define HAL_AFE_BSSBA             HAL_AFE_BSSBA_MASK
#define HAL_AFE_BSSBA_READABLE    1
#define HAL_AFE_BSSBA_WRITABLE    1
#define HAL_AFE_BSSBA_SIGNED      0
#define HAL_AFE_BSSBA_ADR         0x1000090B

#define HAL_AFE_BSPHS_REGISTER    (HAL_AFEREG_MAP->BSRP05)
#define HAL_AFE_BSPHS_LSB         5
#define HAL_AFE_BSPHS_MSB         5
#define HAL_AFE_BSPHS_MASK        0x00000020
#define HAL_AFE_BSPHS_TYPE        Bool
#define HAL_AFE_BSPHS             HAL_AFE_BSPHS_MASK
#define HAL_AFE_BSPHS_READABLE    1
#define HAL_AFE_BSPHS_WRITABLE    1
#define HAL_AFE_BSPHS_SIGNED      0
#define HAL_AFE_BSPHS_ADR         0x1000090B

#define HAL_AFE_BSBHS_REGISTER    (HAL_AFEREG_MAP->BSRP05)
#define HAL_AFE_BSBHS_LSB         4
#define HAL_AFE_BSBHS_MSB         4
#define HAL_AFE_BSBHS_MASK        0x00000010
#define HAL_AFE_BSBHS_TYPE        Bool
#define HAL_AFE_BSBHS             HAL_AFE_BSBHS_MASK
#define HAL_AFE_BSBHS_READABLE    1
#define HAL_AFE_BSBHS_WRITABLE    1
#define HAL_AFE_BSBHS_SIGNED      0
#define HAL_AFE_BSBHS_ADR         0x1000090B

// Register BSRP06

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_TXOS_REGISTER    (HAL_AFEREG_MAP->BSRP06)
#define HAL_AFE_TXOS_LSB         0
#define HAL_AFE_TXOS_MSB         4
#define HAL_AFE_TXOS_MASK        0x0000001F
#define HAL_AFE_TXOS_TYPE        UByte
#define HAL_AFE_TXOS             HAL_AFE_TXOS_MASK
#define HAL_AFE_TXOS_READABLE    1
#define HAL_AFE_TXOS_WRITABLE    1
#define HAL_AFE_TXOS_SIGNED      0
#define HAL_AFE_TXOS_ADR         0x1000090C

#endif // #if (CHIP_REV >= 0xC0)

// Register BSRP07
#define HAL_AFE_BSPGN_REGISTER    (HAL_AFEREG_MAP->BSRP07)
#define HAL_AFE_BSPGN_LSB         6
#define HAL_AFE_BSPGN_MSB         7
#define HAL_AFE_BSPGN_MASK        0x000000C0
#define HAL_AFE_BSPGN_TYPE        halAFE_BSPGN_t
#define HAL_AFE_BSPGN             HAL_AFE_BSPGN_MASK
#define HAL_AFE_BSPGN_READABLE    1
#define HAL_AFE_BSPGN_WRITABLE    1
#define HAL_AFE_BSPGN_SIGNED      0
#define HAL_AFE_BSPGN_ADR         0x1000090D

#define HAL_AFE_BSHLD_REGISTER    (HAL_AFEREG_MAP->BSRP07)
#define HAL_AFE_BSHLD_LSB         5
#define HAL_AFE_BSHLD_MSB         5
#define HAL_AFE_BSHLD_MASK        0x00000020
#define HAL_AFE_BSHLD_TYPE        Bool
#define HAL_AFE_BSHLD             HAL_AFE_BSHLD_MASK
#define HAL_AFE_BSHLD_READABLE    1
#define HAL_AFE_BSHLD_WRITABLE    1
#define HAL_AFE_BSHLD_SIGNED      0
#define HAL_AFE_BSHLD_ADR         0x1000090D

#define HAL_AFE_BSBG_REGISTER    (HAL_AFEREG_MAP->BSRP07)
#define HAL_AFE_BSBG_LSB         0
#define HAL_AFE_BSBG_MSB         4
#define HAL_AFE_BSBG_MASK        0x0000001F
#define HAL_AFE_BSBG_TYPE        UByte
#define HAL_AFE_BSBG             HAL_AFE_BSBG_MASK
#define HAL_AFE_BSBG_READABLE    1
#define HAL_AFE_BSBG_WRITABLE    1
#define HAL_AFE_BSBG_SIGNED      0
#define HAL_AFE_BSBG_ADR         0x1000090D

// Register BSRP08

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFVREFNOS_REGISTER    (HAL_AFEREG_MAP->BSRP08)
#define HAL_AFE_RFVREFNOS_LSB         0
#define HAL_AFE_RFVREFNOS_MSB         5
#define HAL_AFE_RFVREFNOS_MASK        0x0000003F
#define HAL_AFE_RFVREFNOS_TYPE        UByte
#define HAL_AFE_RFVREFNOS             HAL_AFE_RFVREFNOS_MASK
#define HAL_AFE_RFVREFNOS_READABLE    1
#define HAL_AFE_RFVREFNOS_WRITABLE    1
#define HAL_AFE_RFVREFNOS_SIGNED      0
#define HAL_AFE_RFVREFNOS_ADR         0x1000090E

#endif // #if (CHIP_REV >= 0xC0)

// Register BSRP09
#define HAL_AFE_BS2BYPASS_REGISTER    (HAL_AFEREG_MAP->BSRP09)
#define HAL_AFE_BS2BYPASS_LSB         7
#define HAL_AFE_BS2BYPASS_MSB         7
#define HAL_AFE_BS2BYPASS_MASK        0x00000080
#define HAL_AFE_BS2BYPASS_TYPE        Bool
#define HAL_AFE_BS2BYPASS             HAL_AFE_BS2BYPASS_MASK
#define HAL_AFE_BS2BYPASS_READABLE    1
#define HAL_AFE_BS2BYPASS_WRITABLE    1
#define HAL_AFE_BS2BYPASS_SIGNED      0
#define HAL_AFE_BS2BYPASS_ADR         0x1000090F

#define HAL_AFE_BSINV_REGISTER    (HAL_AFEREG_MAP->BSRP09)
#define HAL_AFE_BSINV_LSB         6
#define HAL_AFE_BSINV_MSB         6
#define HAL_AFE_BSINV_MASK        0x00000040
#define HAL_AFE_BSINV_TYPE        Bool
#define HAL_AFE_BSINV             HAL_AFE_BSINV_MASK
#define HAL_AFE_BSINV_READABLE    1
#define HAL_AFE_BSINV_WRITABLE    1
#define HAL_AFE_BSINV_SIGNED      0
#define HAL_AFE_BSINV_ADR         0x1000090F

#define HAL_AFE_BSLP_REGISTER    (HAL_AFEREG_MAP->BSRP09)
#define HAL_AFE_BSLP_LSB         2
#define HAL_AFE_BSLP_MSB         3
#define HAL_AFE_BSLP_MASK        0x0000000C
#define HAL_AFE_BSLP_TYPE        halAFE_BSLPF_t
#define HAL_AFE_BSLP             HAL_AFE_BSLP_MASK
#define HAL_AFE_BSLP_READABLE    1
#define HAL_AFE_BSLP_WRITABLE    1
#define HAL_AFE_BSLP_SIGNED      0
#define HAL_AFE_BSLP_ADR         0x1000090F

#define HAL_AFE_BSLPSET_REGISTER    (HAL_AFEREG_MAP->BSRP09)
#define HAL_AFE_BSLPSET_LSB         1
#define HAL_AFE_BSLPSET_MSB         1
#define HAL_AFE_BSLPSET_MASK        0x00000002
#define HAL_AFE_BSLPSET_TYPE        Bool
#define HAL_AFE_BSLPSET             HAL_AFE_BSLPSET_MASK
#define HAL_AFE_BSLPSET_READABLE    1
#define HAL_AFE_BSLPSET_WRITABLE    1
#define HAL_AFE_BSLPSET_SIGNED      0
#define HAL_AFE_BSLPSET_ADR         0x1000090F

#define HAL_AFE_BPBSLP_REGISTER    (HAL_AFEREG_MAP->BSRP09)
#define HAL_AFE_BPBSLP_LSB         0
#define HAL_AFE_BPBSLP_MSB         0
#define HAL_AFE_BPBSLP_MASK        0x00000001
#define HAL_AFE_BPBSLP_TYPE        Bool
#define HAL_AFE_BPBSLP             HAL_AFE_BPBSLP_MASK
#define HAL_AFE_BPBSLP_READABLE    1
#define HAL_AFE_BPBSLP_WRITABLE    1
#define HAL_AFE_BPBSLP_SIGNED      0
#define HAL_AFE_BPBSLP_ADR         0x1000090F

// Register BSRP10
#define HAL_AFE_RPSRF_REGISTER    (HAL_AFEREG_MAP->BSRP10)
#define HAL_AFE_RPSRF_LSB         7
#define HAL_AFE_RPSRF_MSB         7
#define HAL_AFE_RPSRF_MASK        0x00000080
#define HAL_AFE_RPSRF_TYPE        Bool
#define HAL_AFE_RPSRF             HAL_AFE_RPSRF_MASK
#define HAL_AFE_RPSRF_READABLE    1
#define HAL_AFE_RPSRF_WRITABLE    1
#define HAL_AFE_RPSRF_SIGNED      0
#define HAL_AFE_RPSRF_ADR         0x10000910

#define HAL_AFE_RPSBA_REGISTER    (HAL_AFEREG_MAP->BSRP10)
#define HAL_AFE_RPSBA_LSB         6
#define HAL_AFE_RPSBA_MSB         6
#define HAL_AFE_RPSBA_MASK        0x00000040
#define HAL_AFE_RPSBA_TYPE        Bool
#define HAL_AFE_RPSBA             HAL_AFE_RPSBA_MASK
#define HAL_AFE_RPSBA_READABLE    1
#define HAL_AFE_RPSBA_WRITABLE    1
#define HAL_AFE_RPSBA_SIGNED      0
#define HAL_AFE_RPSBA_ADR         0x10000910

#define HAL_AFE_RPPHS_REGISTER    (HAL_AFEREG_MAP->BSRP10)
#define HAL_AFE_RPPHS_LSB         5
#define HAL_AFE_RPPHS_MSB         5
#define HAL_AFE_RPPHS_MASK        0x00000020
#define HAL_AFE_RPPHS_TYPE        Bool
#define HAL_AFE_RPPHS             HAL_AFE_RPPHS_MASK
#define HAL_AFE_RPPHS_READABLE    1
#define HAL_AFE_RPPHS_WRITABLE    1
#define HAL_AFE_RPPHS_SIGNED      0
#define HAL_AFE_RPPHS_ADR         0x10000910

#define HAL_AFE_RPBHS_REGISTER    (HAL_AFEREG_MAP->BSRP10)
#define HAL_AFE_RPBHS_LSB         4
#define HAL_AFE_RPBHS_MSB         4
#define HAL_AFE_RPBHS_MASK        0x00000010
#define HAL_AFE_RPBHS_TYPE        Bool
#define HAL_AFE_RPBHS             HAL_AFE_RPBHS_MASK
#define HAL_AFE_RPBHS_READABLE    1
#define HAL_AFE_RPBHS_WRITABLE    1
#define HAL_AFE_RPBHS_SIGNED      0
#define HAL_AFE_RPBHS_ADR         0x10000910

// Register BSRP11
#define HAL_AFE_RPOS_REGISTER    (HAL_AFEREG_MAP->BSRP11)
#define HAL_AFE_RPOS_LSB         0
#define HAL_AFE_RPOS_MSB         7
#define HAL_AFE_RPOS_MASK        0x000000FF
#define HAL_AFE_RPOS_TYPE        UByte
#define HAL_AFE_RPOS             HAL_AFE_RPOS_MASK
#define HAL_AFE_RPOS_READABLE    1
#define HAL_AFE_RPOS_WRITABLE    1
#define HAL_AFE_RPOS_SIGNED      0
#define HAL_AFE_RPOS_ADR         0x10000911

// Register BSRP12
#define HAL_AFE_RPPGN_REGISTER    (HAL_AFEREG_MAP->BSRP12)
#define HAL_AFE_RPPGN_LSB         6
#define HAL_AFE_RPPGN_MSB         7
#define HAL_AFE_RPPGN_MASK        0x000000C0
#define HAL_AFE_RPPGN_TYPE        halAFE_RPPGN_t
#define HAL_AFE_RPPGN             HAL_AFE_RPPGN_MASK
#define HAL_AFE_RPPGN_READABLE    1
#define HAL_AFE_RPPGN_WRITABLE    1
#define HAL_AFE_RPPGN_SIGNED      0
#define HAL_AFE_RPPGN_ADR         0x10000912

#define HAL_AFE_RPHLD_REGISTER    (HAL_AFEREG_MAP->BSRP12)
#define HAL_AFE_RPHLD_LSB         5
#define HAL_AFE_RPHLD_MSB         5
#define HAL_AFE_RPHLD_MASK        0x00000020
#define HAL_AFE_RPHLD_TYPE        Bool
#define HAL_AFE_RPHLD             HAL_AFE_RPHLD_MASK
#define HAL_AFE_RPHLD_READABLE    1
#define HAL_AFE_RPHLD_WRITABLE    1
#define HAL_AFE_RPHLD_SIGNED      0
#define HAL_AFE_RPHLD_ADR         0x10000912

#define HAL_AFE_RPBG_REGISTER    (HAL_AFEREG_MAP->BSRP12)
#define HAL_AFE_RPBG_LSB         0
#define HAL_AFE_RPBG_MSB         4
#define HAL_AFE_RPBG_MASK        0x0000001F
#define HAL_AFE_RPBG_TYPE        UByte
#define HAL_AFE_RPBG             HAL_AFE_RPBG_MASK
#define HAL_AFE_RPBG_READABLE    1
#define HAL_AFE_RPBG_WRITABLE    1
#define HAL_AFE_RPBG_SIGNED      0
#define HAL_AFE_RPBG_ADR         0x10000912

// Register BSRP13

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFRPSEL_REGISTER    (HAL_AFEREG_MAP->BSRP13)
#define HAL_AFE_RFRPSEL_LSB         7
#define HAL_AFE_RFRPSEL_MSB         7
#define HAL_AFE_RFRPSEL_MASK        0x00000080
#define HAL_AFE_RFRPSEL_TYPE        Bool
#define HAL_AFE_RFRPSEL             HAL_AFE_RFRPSEL_MASK
#define HAL_AFE_RFRPSEL_READABLE    1
#define HAL_AFE_RFRPSEL_WRITABLE    1
#define HAL_AFE_RFRPSEL_SIGNED      0
#define HAL_AFE_RFRPSEL_ADR         0x10000913

#define HAL_AFE_AHOLDRP_REGISTER    (HAL_AFEREG_MAP->BSRP13)
#define HAL_AFE_AHOLDRP_LSB         6
#define HAL_AFE_AHOLDRP_MSB         6
#define HAL_AFE_AHOLDRP_MASK        0x00000040
#define HAL_AFE_AHOLDRP_TYPE        Bool
#define HAL_AFE_AHOLDRP             HAL_AFE_AHOLDRP_MASK
#define HAL_AFE_AHOLDRP_READABLE    1
#define HAL_AFE_AHOLDRP_WRITABLE    1
#define HAL_AFE_AHOLDRP_SIGNED      0
#define HAL_AFE_AHOLDRP_ADR         0x10000913

#define HAL_AFE_RPBLNKINV_REGISTER    (HAL_AFEREG_MAP->BSRP13)
#define HAL_AFE_RPBLNKINV_LSB         5
#define HAL_AFE_RPBLNKINV_MSB         5
#define HAL_AFE_RPBLNKINV_MASK        0x00000020
#define HAL_AFE_RPBLNKINV_TYPE        Bool
#define HAL_AFE_RPBLNKINV             HAL_AFE_RPBLNKINV_MASK
#define HAL_AFE_RPBLNKINV_READABLE    1
#define HAL_AFE_RPBLNKINV_WRITABLE    1
#define HAL_AFE_RPBLNKINV_SIGNED      0
#define HAL_AFE_RPBLNKINV_ADR         0x10000913

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_RPGN_REGISTER    (HAL_AFEREG_MAP->BSRP13)
#define HAL_AFE_RPGN_LSB         0
#define HAL_AFE_RPGN_MSB         4
#define HAL_AFE_RPGN_MASK        0x0000001F
#define HAL_AFE_RPGN_TYPE        UByte
#define HAL_AFE_RPGN             HAL_AFE_RPGN_MASK
#define HAL_AFE_RPGN_READABLE    1
#define HAL_AFE_RPGN_WRITABLE    1
#define HAL_AFE_RPGN_SIGNED      0
#define HAL_AFE_RPGN_ADR         0x10000913

// Register BSRP14
#define HAL_AFE_RP2BYPASS_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_RP2BYPASS_LSB         7
#define HAL_AFE_RP2BYPASS_MSB         7
#define HAL_AFE_RP2BYPASS_MASK        0x00000080
#define HAL_AFE_RP2BYPASS_TYPE        Bool
#define HAL_AFE_RP2BYPASS             HAL_AFE_RP2BYPASS_MASK
#define HAL_AFE_RP2BYPASS_READABLE    1
#define HAL_AFE_RP2BYPASS_WRITABLE    1
#define HAL_AFE_RP2BYPASS_SIGNED      0
#define HAL_AFE_RP2BYPASS_ADR         0x10000914

#define HAL_AFE_RPINV_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_RPINV_LSB         6
#define HAL_AFE_RPINV_MSB         6
#define HAL_AFE_RPINV_MASK        0x00000040
#define HAL_AFE_RPINV_TYPE        Bool
#define HAL_AFE_RPINV             HAL_AFE_RPINV_MASK
#define HAL_AFE_RPINV_READABLE    1
#define HAL_AFE_RPINV_WRITABLE    1
#define HAL_AFE_RPINV_SIGNED      0
#define HAL_AFE_RPINV_ADR         0x10000914

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RPDFCTDLY_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_RPDFCTDLY_LSB         4
#define HAL_AFE_RPDFCTDLY_MSB         5
#define HAL_AFE_RPDFCTDLY_MASK        0x00000030
#define HAL_AFE_RPDFCTDLY_TYPE        halAFE_RPDefectDelay_t
#define HAL_AFE_RPDFCTDLY             HAL_AFE_RPDFCTDLY_MASK
#define HAL_AFE_RPDFCTDLY_READABLE    1
#define HAL_AFE_RPDFCTDLY_WRITABLE    1
#define HAL_AFE_RPDFCTDLY_SIGNED      0
#define HAL_AFE_RPDFCTDLY_ADR         0x10000914

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_RPLP_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_RPLP_LSB         2
#define HAL_AFE_RPLP_MSB         3
#define HAL_AFE_RPLP_MASK        0x0000000C
#define HAL_AFE_RPLP_TYPE        halAFE_RPLPF_t
#define HAL_AFE_RPLP             HAL_AFE_RPLP_MASK
#define HAL_AFE_RPLP_READABLE    1
#define HAL_AFE_RPLP_WRITABLE    1
#define HAL_AFE_RPLP_SIGNED      0
#define HAL_AFE_RPLP_ADR         0x10000914

#define HAL_AFE_RPLPSET_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_RPLPSET_LSB         1
#define HAL_AFE_RPLPSET_MSB         1
#define HAL_AFE_RPLPSET_MASK        0x00000002
#define HAL_AFE_RPLPSET_TYPE        Bool
#define HAL_AFE_RPLPSET             HAL_AFE_RPLPSET_MASK
#define HAL_AFE_RPLPSET_READABLE    1
#define HAL_AFE_RPLPSET_WRITABLE    1
#define HAL_AFE_RPLPSET_SIGNED      0
#define HAL_AFE_RPLPSET_ADR         0x10000914

#define HAL_AFE_BPRPLP_REGISTER    (HAL_AFEREG_MAP->BSRP14)
#define HAL_AFE_BPRPLP_LSB         0
#define HAL_AFE_BPRPLP_MSB         0
#define HAL_AFE_BPRPLP_MASK        0x00000001
#define HAL_AFE_BPRPLP_TYPE        Bool
#define HAL_AFE_BPRPLP             HAL_AFE_BPRPLP_MASK
#define HAL_AFE_BPRPLP_READABLE    1
#define HAL_AFE_BPRPLP_WRITABLE    1
#define HAL_AFE_BPRPLP_SIGNED      0
#define HAL_AFE_BPRPLP_ADR         0x10000914

// Register BSRP15
#define HAL_AFE_RP2OS_REGISTER    (HAL_AFEREG_MAP->BSRP15)
#define HAL_AFE_RP2OS_LSB         0
#define HAL_AFE_RP2OS_MSB         7
#define HAL_AFE_RP2OS_MASK        0x000000FF
#define HAL_AFE_RP2OS_TYPE        UByte
#define HAL_AFE_RP2OS             HAL_AFE_RP2OS_MASK
#define HAL_AFE_RP2OS_READABLE    1
#define HAL_AFE_RP2OS_WRITABLE    1
#define HAL_AFE_RP2OS_SIGNED      0
#define HAL_AFE_RP2OS_ADR         0x10000915

// Register BSRP16
#define HAL_AFE_RP2GN_REGISTER    (HAL_AFEREG_MAP->BSRP16)
#define HAL_AFE_RP2GN_LSB         0
#define HAL_AFE_RP2GN_MSB         4
#define HAL_AFE_RP2GN_MASK        0x0000001F
#define HAL_AFE_RP2GN_TYPE        UByte
#define HAL_AFE_RP2GN             HAL_AFE_RP2GN_MASK
#define HAL_AFE_RP2GN_READABLE    1
#define HAL_AFE_RP2GN_WRITABLE    1
#define HAL_AFE_RP2GN_SIGNED      0
#define HAL_AFE_RP2GN_ADR         0x10000916

// Register BSRP17
#define HAL_AFE_PHSOS_REGISTER    (HAL_AFEREG_MAP->BSRP17)
#define HAL_AFE_PHSOS_LSB         0
#define HAL_AFE_PHSOS_MSB         7
#define HAL_AFE_PHSOS_MASK        0x000000FF
#define HAL_AFE_PHSOS_TYPE        UByte
#define HAL_AFE_PHSOS             HAL_AFE_PHSOS_MASK
#define HAL_AFE_PHSOS_READABLE    1
#define HAL_AFE_PHSOS_WRITABLE    1
#define HAL_AFE_PHSOS_SIGNED      0
#define HAL_AFE_PHSOS_ADR         0x10000917

// Register BSRP18
#define HAL_AFE_PHSBYPASS_REGISTER    (HAL_AFEREG_MAP->BSRP18)
#define HAL_AFE_PHSBYPASS_LSB         7
#define HAL_AFE_PHSBYPASS_MSB         7
#define HAL_AFE_PHSBYPASS_MASK        0x00000080
#define HAL_AFE_PHSBYPASS_TYPE        Bool
#define HAL_AFE_PHSBYPASS             HAL_AFE_PHSBYPASS_MASK
#define HAL_AFE_PHSBYPASS_READABLE    1
#define HAL_AFE_PHSBYPASS_WRITABLE    1
#define HAL_AFE_PHSBYPASS_SIGNED      0
#define HAL_AFE_PHSBYPASS_ADR         0x10000918

#define HAL_AFE_PHSGN_REGISTER    (HAL_AFEREG_MAP->BSRP18)
#define HAL_AFE_PHSGN_LSB         0
#define HAL_AFE_PHSGN_MSB         4
#define HAL_AFE_PHSGN_MASK        0x0000001F
#define HAL_AFE_PHSGN_TYPE        UByte
#define HAL_AFE_PHSGN             HAL_AFE_PHSGN_MASK
#define HAL_AFE_PHSGN_READABLE    1
#define HAL_AFE_PHSGN_WRITABLE    1
#define HAL_AFE_PHSGN_SIGNED      0
#define HAL_AFE_PHSGN_ADR         0x10000918

// Register BSRP19
#define HAL_AFE_BHSOS_REGISTER    (HAL_AFEREG_MAP->BSRP19)
#define HAL_AFE_BHSOS_LSB         0
#define HAL_AFE_BHSOS_MSB         7
#define HAL_AFE_BHSOS_MASK        0x000000FF
#define HAL_AFE_BHSOS_TYPE        UByte
#define HAL_AFE_BHSOS             HAL_AFE_BHSOS_MASK
#define HAL_AFE_BHSOS_READABLE    1
#define HAL_AFE_BHSOS_WRITABLE    1
#define HAL_AFE_BHSOS_SIGNED      0
#define HAL_AFE_BHSOS_ADR         0x10000919

// Register BSRP20
#define HAL_AFE_BHSBYPASS_REGISTER    (HAL_AFEREG_MAP->BSRP20)
#define HAL_AFE_BHSBYPASS_LSB         7
#define HAL_AFE_BHSBYPASS_MSB         7
#define HAL_AFE_BHSBYPASS_MASK        0x00000080
#define HAL_AFE_BHSBYPASS_TYPE        Bool
#define HAL_AFE_BHSBYPASS             HAL_AFE_BHSBYPASS_MASK
#define HAL_AFE_BHSBYPASS_READABLE    1
#define HAL_AFE_BHSBYPASS_WRITABLE    1
#define HAL_AFE_BHSBYPASS_SIGNED      0
#define HAL_AFE_BHSBYPASS_ADR         0x1000091A

#define HAL_AFE_BHSGN_REGISTER    (HAL_AFEREG_MAP->BSRP20)
#define HAL_AFE_BHSGN_LSB         0
#define HAL_AFE_BHSGN_MSB         4
#define HAL_AFE_BHSGN_MASK        0x0000001F
#define HAL_AFE_BHSGN_TYPE        UByte
#define HAL_AFE_BHSGN             HAL_AFE_BHSGN_MASK
#define HAL_AFE_BHSGN_READABLE    1
#define HAL_AFE_BHSGN_WRITABLE    1
#define HAL_AFE_BHSGN_SIGNED      0
#define HAL_AFE_BHSGN_ADR         0x1000091A

// Register BSRP21
#define HAL_AFE_PHSCSEL_REGISTER    (HAL_AFEREG_MAP->BSRP21)
#define HAL_AFE_PHSCSEL_LSB         7
#define HAL_AFE_PHSCSEL_MSB         7
#define HAL_AFE_PHSCSEL_MASK        0x00000080
#define HAL_AFE_PHSCSEL_TYPE        halAFE_PHBaseRate_t
#define HAL_AFE_PHSCSEL             HAL_AFE_PHSCSEL_MASK
#define HAL_AFE_PHSCSEL_READABLE    1
#define HAL_AFE_PHSCSEL_WRITABLE    1
#define HAL_AFE_PHSCSEL_SIGNED      0
#define HAL_AFE_PHSCSEL_ADR         0x1000091B

#define HAL_AFE_PHSDIS_REGISTER    (HAL_AFEREG_MAP->BSRP21)
#define HAL_AFE_PHSDIS_LSB         4
#define HAL_AFE_PHSDIS_MSB         5
#define HAL_AFE_PHSDIS_MASK        0x00000030
#define HAL_AFE_PHSDIS_TYPE        halAFE_PHDRateInc_t
#define HAL_AFE_PHSDIS             HAL_AFE_PHSDIS_MASK
#define HAL_AFE_PHSDIS_READABLE    1
#define HAL_AFE_PHSDIS_WRITABLE    1
#define HAL_AFE_PHSDIS_SIGNED      0
#define HAL_AFE_PHSDIS_ADR         0x1000091B

#define HAL_AFE_BHSCSEL_REGISTER    (HAL_AFEREG_MAP->BSRP21)
#define HAL_AFE_BHSCSEL_LSB         3
#define HAL_AFE_BHSCSEL_MSB         3
#define HAL_AFE_BHSCSEL_MASK        0x00000008
#define HAL_AFE_BHSCSEL_TYPE        halAFE_BHBaseRate_t
#define HAL_AFE_BHSCSEL             HAL_AFE_BHSCSEL_MASK
#define HAL_AFE_BHSCSEL_READABLE    1
#define HAL_AFE_BHSCSEL_WRITABLE    1
#define HAL_AFE_BHSCSEL_SIGNED      0
#define HAL_AFE_BHSCSEL_ADR         0x1000091B

#define HAL_AFE_BHSDIS_REGISTER    (HAL_AFEREG_MAP->BSRP21)
#define HAL_AFE_BHSDIS_LSB         0
#define HAL_AFE_BHSDIS_MSB         1
#define HAL_AFE_BHSDIS_MASK        0x00000003
#define HAL_AFE_BHSDIS_TYPE        halAFE_BHDRateInc_t
#define HAL_AFE_BHSDIS             HAL_AFE_BHSDIS_MASK
#define HAL_AFE_BHSDIS_READABLE    1
#define HAL_AFE_BHSDIS_WRITABLE    1
#define HAL_AFE_BHSDIS_SIGNED      0
#define HAL_AFE_BHSDIS_ADR         0x1000091B

// Register FCTS00
#define HAL_AFE_BLANKFCSO_REGISTER    (HAL_AFEREG_MAP->FCTS00)
#define HAL_AFE_BLANKFCSO_LSB         0
#define HAL_AFE_BLANKFCSO_MSB         7
#define HAL_AFE_BLANKFCSO_MASK        0x000000FF
#define HAL_AFE_BLANKFCSO_TYPE        UByte
#define HAL_AFE_BLANKFCSO             HAL_AFE_BLANKFCSO_MASK
#define HAL_AFE_BLANKFCSO_READABLE    1
#define HAL_AFE_BLANKFCSO_WRITABLE    1
#define HAL_AFE_BLANKFCSO_SIGNED      0
#define HAL_AFE_BLANKFCSO_ADR         0x1000091C

// Register FCTS01
#define HAL_AFE_BLNDFCSO_REGISTER    (HAL_AFEREG_MAP->FCTS01)
#define HAL_AFE_BLNDFCSO_LSB         0
#define HAL_AFE_BLNDFCSO_MSB         7
#define HAL_AFE_BLNDFCSO_MASK        0x000000FF
#define HAL_AFE_BLNDFCSO_TYPE        UByte
#define HAL_AFE_BLNDFCSO             HAL_AFE_BLNDFCSO_MASK
#define HAL_AFE_BLNDFCSO_READABLE    1
#define HAL_AFE_BLNDFCSO_WRITABLE    1
#define HAL_AFE_BLNDFCSO_SIGNED      0
#define HAL_AFE_BLNDFCSO_ADR         0x1000091D

// Register FCTS02
#define HAL_AFE_FCSO_REGISTER    (HAL_AFEREG_MAP->FCTS02)
#define HAL_AFE_FCSO_LSB         0
#define HAL_AFE_FCSO_MSB         7
#define HAL_AFE_FCSO_MASK        0x000000FF
#define HAL_AFE_FCSO_TYPE        UByte
#define HAL_AFE_FCSO             HAL_AFE_FCSO_MASK
#define HAL_AFE_FCSO_READABLE    1
#define HAL_AFE_FCSO_WRITABLE    1
#define HAL_AFE_FCSO_SIGNED      0
#define HAL_AFE_FCSO_ADR         0x1000091E

// Register FCTS03
#define HAL_AFE_LNDFCSO_REGISTER    (HAL_AFEREG_MAP->FCTS03)
#define HAL_AFE_LNDFCSO_LSB         0
#define HAL_AFE_LNDFCSO_MSB         7
#define HAL_AFE_LNDFCSO_MASK        0x000000FF
#define HAL_AFE_LNDFCSO_TYPE        UByte
#define HAL_AFE_LNDFCSO             HAL_AFE_LNDFCSO_MASK
#define HAL_AFE_LNDFCSO_READABLE    1
#define HAL_AFE_LNDFCSO_WRITABLE    1
#define HAL_AFE_LNDFCSO_SIGNED      0
#define HAL_AFE_LNDFCSO_ADR         0x1000091F

// Register FCTS04
#define HAL_AFE_WRFCSO_REGISTER    (HAL_AFEREG_MAP->FCTS04)
#define HAL_AFE_WRFCSO_LSB         0
#define HAL_AFE_WRFCSO_MSB         7
#define HAL_AFE_WRFCSO_MASK        0x000000FF
#define HAL_AFE_WRFCSO_TYPE        UByte
#define HAL_AFE_WRFCSO             HAL_AFE_WRFCSO_MASK
#define HAL_AFE_WRFCSO_READABLE    1
#define HAL_AFE_WRFCSO_WRITABLE    1
#define HAL_AFE_WRFCSO_SIGNED      0
#define HAL_AFE_WRFCSO_ADR         0x10000920

// Register FCTS05
#define HAL_AFE_BLANKFCSG_REGISTER    (HAL_AFEREG_MAP->FCTS05)
#define HAL_AFE_BLANKFCSG_LSB         0
#define HAL_AFE_BLANKFCSG_MSB         4
#define HAL_AFE_BLANKFCSG_MASK        0x0000001F
#define HAL_AFE_BLANKFCSG_TYPE        UByte
#define HAL_AFE_BLANKFCSG             HAL_AFE_BLANKFCSG_MASK
#define HAL_AFE_BLANKFCSG_READABLE    1
#define HAL_AFE_BLANKFCSG_WRITABLE    1
#define HAL_AFE_BLANKFCSG_SIGNED      0
#define HAL_AFE_BLANKFCSG_ADR         0x10000921

// Register FCTS06
#define HAL_AFE_BLNDFCSG_REGISTER    (HAL_AFEREG_MAP->FCTS06)
#define HAL_AFE_BLNDFCSG_LSB         0
#define HAL_AFE_BLNDFCSG_MSB         4
#define HAL_AFE_BLNDFCSG_MASK        0x0000001F
#define HAL_AFE_BLNDFCSG_TYPE        UByte
#define HAL_AFE_BLNDFCSG             HAL_AFE_BLNDFCSG_MASK
#define HAL_AFE_BLNDFCSG_READABLE    1
#define HAL_AFE_BLNDFCSG_WRITABLE    1
#define HAL_AFE_BLNDFCSG_SIGNED      0
#define HAL_AFE_BLNDFCSG_ADR         0x10000922

// Register FCTS07
#define HAL_AFE_FCSG_REGISTER    (HAL_AFEREG_MAP->FCTS07)
#define HAL_AFE_FCSG_LSB         0
#define HAL_AFE_FCSG_MSB         4
#define HAL_AFE_FCSG_MASK        0x0000001F
#define HAL_AFE_FCSG_TYPE        UByte
#define HAL_AFE_FCSG             HAL_AFE_FCSG_MASK
#define HAL_AFE_FCSG_READABLE    1
#define HAL_AFE_FCSG_WRITABLE    1
#define HAL_AFE_FCSG_SIGNED      0
#define HAL_AFE_FCSG_ADR         0x10000923

// Register FCTS08
#define HAL_AFE_LNDFCSG_REGISTER    (HAL_AFEREG_MAP->FCTS08)
#define HAL_AFE_LNDFCSG_LSB         0
#define HAL_AFE_LNDFCSG_MSB         4
#define HAL_AFE_LNDFCSG_MASK        0x0000001F
#define HAL_AFE_LNDFCSG_TYPE        UByte
#define HAL_AFE_LNDFCSG             HAL_AFE_LNDFCSG_MASK
#define HAL_AFE_LNDFCSG_READABLE    1
#define HAL_AFE_LNDFCSG_WRITABLE    1
#define HAL_AFE_LNDFCSG_SIGNED      0
#define HAL_AFE_LNDFCSG_ADR         0x10000924

// Register FCTS09
#define HAL_AFE_WRFCSG_REGISTER    (HAL_AFEREG_MAP->FCTS09)
#define HAL_AFE_WRFCSG_LSB         0
#define HAL_AFE_WRFCSG_MSB         4
#define HAL_AFE_WRFCSG_MASK        0x0000001F
#define HAL_AFE_WRFCSG_TYPE        UByte
#define HAL_AFE_WRFCSG             HAL_AFE_WRFCSG_MASK
#define HAL_AFE_WRFCSG_READABLE    1
#define HAL_AFE_WRFCSG_WRITABLE    1
#define HAL_AFE_WRFCSG_SIGNED      0
#define HAL_AFE_WRFCSG_ADR         0x10000925

// Register FCTS10
#define HAL_AFE_AHOLDFE_REGISTER    (HAL_AFEREG_MAP->FCTS10)
#define HAL_AFE_AHOLDFE_LSB         3
#define HAL_AFE_AHOLDFE_MSB         3
#define HAL_AFE_AHOLDFE_MASK        0x00000008
#define HAL_AFE_AHOLDFE_TYPE        Bool
#define HAL_AFE_AHOLDFE             HAL_AFE_AHOLDFE_MASK
#define HAL_AFE_AHOLDFE_READABLE    1
#define HAL_AFE_AHOLDFE_WRITABLE    1
#define HAL_AFE_AHOLDFE_SIGNED      0
#define HAL_AFE_AHOLDFE_ADR         0x10000926

#define HAL_AFE_FEHDRHLD_REGISTER    (HAL_AFEREG_MAP->FCTS10)
#define HAL_AFE_FEHDRHLD_LSB         2
#define HAL_AFE_FEHDRHLD_MSB         2
#define HAL_AFE_FEHDRHLD_MASK        0x00000004
#define HAL_AFE_FEHDRHLD_TYPE        Bool
#define HAL_AFE_FEHDRHLD             HAL_AFE_FEHDRHLD_MASK
#define HAL_AFE_FEHDRHLD_READABLE    1
#define HAL_AFE_FEHDRHLD_WRITABLE    1
#define HAL_AFE_FEHDRHLD_SIGNED      0
#define HAL_AFE_FEHDRHLD_ADR         0x10000926

#define HAL_AFE_FEREGHLD_REGISTER    (HAL_AFEREG_MAP->FCTS10)
#define HAL_AFE_FEREGHLD_LSB         1
#define HAL_AFE_FEREGHLD_MSB         1
#define HAL_AFE_FEREGHLD_MASK        0x00000002
#define HAL_AFE_FEREGHLD_TYPE        Bool
#define HAL_AFE_FEREGHLD             HAL_AFE_FEREGHLD_MASK
#define HAL_AFE_FEREGHLD_READABLE    1
#define HAL_AFE_FEREGHLD_WRITABLE    1
#define HAL_AFE_FEREGHLD_SIGNED      0
#define HAL_AFE_FEREGHLD_ADR         0x10000926

#define HAL_AFE_FEINV_REGISTER    (HAL_AFEREG_MAP->FCTS10)
#define HAL_AFE_FEINV_LSB         0
#define HAL_AFE_FEINV_MSB         0
#define HAL_AFE_FEINV_MASK        0x00000001
#define HAL_AFE_FEINV_TYPE        Bool
#define HAL_AFE_FEINV             HAL_AFE_FEINV_MASK
#define HAL_AFE_FEINV_READABLE    1
#define HAL_AFE_FEINV_WRITABLE    1
#define HAL_AFE_FEINV_SIGNED      0
#define HAL_AFE_FEINV_ADR         0x10000926

// Register FCTS11
#define HAL_AFE_FE2OSA_REGISTER    (HAL_AFEREG_MAP->FCTS11)
#define HAL_AFE_FE2OSA_LSB         0
#define HAL_AFE_FE2OSA_MSB         7
#define HAL_AFE_FE2OSA_MASK        0x000000FF
#define HAL_AFE_FE2OSA_TYPE        UByte
#define HAL_AFE_FE2OSA             HAL_AFE_FE2OSA_MASK
#define HAL_AFE_FE2OSA_READABLE    1
#define HAL_AFE_FE2OSA_WRITABLE    1
#define HAL_AFE_FE2OSA_SIGNED      0
#define HAL_AFE_FE2OSA_ADR         0x10000927

// Register FCTS12
#define HAL_AFE_FE2OSB_REGISTER    (HAL_AFEREG_MAP->FCTS12)
#define HAL_AFE_FE2OSB_LSB         0
#define HAL_AFE_FE2OSB_MSB         7
#define HAL_AFE_FE2OSB_MASK        0x000000FF
#define HAL_AFE_FE2OSB_TYPE        UByte
#define HAL_AFE_FE2OSB             HAL_AFE_FE2OSB_MASK
#define HAL_AFE_FE2OSB_READABLE    1
#define HAL_AFE_FE2OSB_WRITABLE    1
#define HAL_AFE_FE2OSB_SIGNED      0
#define HAL_AFE_FE2OSB_ADR         0x10000928

// Register FCTS13
#define HAL_AFE_FE2AUTOEN_REGISTER    (HAL_AFEREG_MAP->FCTS13)
#define HAL_AFE_FE2AUTOEN_LSB         7
#define HAL_AFE_FE2AUTOEN_MSB         7
#define HAL_AFE_FE2AUTOEN_MASK        0x00000080
#define HAL_AFE_FE2AUTOEN_TYPE        halAFE_FE2GnSEL_t
#define HAL_AFE_FE2AUTOEN             HAL_AFE_FE2AUTOEN_MASK
#define HAL_AFE_FE2AUTOEN_READABLE    1
#define HAL_AFE_FE2AUTOEN_WRITABLE    1
#define HAL_AFE_FE2AUTOEN_SIGNED      0
#define HAL_AFE_FE2AUTOEN_ADR         0x10000929

#define HAL_AFE_FE2GNREG_REGISTER    (HAL_AFEREG_MAP->FCTS13)
#define HAL_AFE_FE2GNREG_LSB         0
#define HAL_AFE_FE2GNREG_MSB         4
#define HAL_AFE_FE2GNREG_MASK        0x0000001F
#define HAL_AFE_FE2GNREG_TYPE        UByte
#define HAL_AFE_FE2GNREG             HAL_AFE_FE2GNREG_MASK
#define HAL_AFE_FE2GNREG_READABLE    1
#define HAL_AFE_FE2GNREG_WRITABLE    1
#define HAL_AFE_FE2GNREG_SIGNED      0
#define HAL_AFE_FE2GNREG_ADR         0x10000929

// Register FCTS14
#define HAL_AFE_AHOLDCE_REGISTER    (HAL_AFEREG_MAP->FCTS14)
#define HAL_AFE_AHOLDCE_LSB         3
#define HAL_AFE_AHOLDCE_MSB         3
#define HAL_AFE_AHOLDCE_MASK        0x00000008
#define HAL_AFE_AHOLDCE_TYPE        Bool
#define HAL_AFE_AHOLDCE             HAL_AFE_AHOLDCE_MASK
#define HAL_AFE_AHOLDCE_READABLE    1
#define HAL_AFE_AHOLDCE_WRITABLE    1
#define HAL_AFE_AHOLDCE_SIGNED      0
#define HAL_AFE_AHOLDCE_ADR         0x1000092A

#define HAL_AFE_CEHDRHLD_REGISTER    (HAL_AFEREG_MAP->FCTS14)
#define HAL_AFE_CEHDRHLD_LSB         2
#define HAL_AFE_CEHDRHLD_MSB         2
#define HAL_AFE_CEHDRHLD_MASK        0x00000004
#define HAL_AFE_CEHDRHLD_TYPE        Bool
#define HAL_AFE_CEHDRHLD             HAL_AFE_CEHDRHLD_MASK
#define HAL_AFE_CEHDRHLD_READABLE    1
#define HAL_AFE_CEHDRHLD_WRITABLE    1
#define HAL_AFE_CEHDRHLD_SIGNED      0
#define HAL_AFE_CEHDRHLD_ADR         0x1000092A

#define HAL_AFE_CEREGHLD_REGISTER    (HAL_AFEREG_MAP->FCTS14)
#define HAL_AFE_CEREGHLD_LSB         1
#define HAL_AFE_CEREGHLD_MSB         1
#define HAL_AFE_CEREGHLD_MASK        0x00000002
#define HAL_AFE_CEREGHLD_TYPE        Bool
#define HAL_AFE_CEREGHLD             HAL_AFE_CEREGHLD_MASK
#define HAL_AFE_CEREGHLD_READABLE    1
#define HAL_AFE_CEREGHLD_WRITABLE    1
#define HAL_AFE_CEREGHLD_SIGNED      0
#define HAL_AFE_CEREGHLD_ADR         0x1000092A

#define HAL_AFE_CEINV_REGISTER    (HAL_AFEREG_MAP->FCTS14)
#define HAL_AFE_CEINV_LSB         0
#define HAL_AFE_CEINV_MSB         0
#define HAL_AFE_CEINV_MASK        0x00000001
#define HAL_AFE_CEINV_TYPE        Bool
#define HAL_AFE_CEINV             HAL_AFE_CEINV_MASK
#define HAL_AFE_CEINV_READABLE    1
#define HAL_AFE_CEINV_WRITABLE    1
#define HAL_AFE_CEINV_SIGNED      0
#define HAL_AFE_CEINV_ADR         0x1000092A

// Register FCTS15
#define HAL_AFE_BLANKTRKO_REGISTER    (HAL_AFEREG_MAP->FCTS15)
#define HAL_AFE_BLANKTRKO_LSB         0
#define HAL_AFE_BLANKTRKO_MSB         7
#define HAL_AFE_BLANKTRKO_MASK        0x000000FF
#define HAL_AFE_BLANKTRKO_TYPE        UByte
#define HAL_AFE_BLANKTRKO             HAL_AFE_BLANKTRKO_MASK
#define HAL_AFE_BLANKTRKO_READABLE    1
#define HAL_AFE_BLANKTRKO_WRITABLE    1
#define HAL_AFE_BLANKTRKO_SIGNED      0
#define HAL_AFE_BLANKTRKO_ADR         0x1000092B

// Register FCTS16
#define HAL_AFE_BLNDTRKO_REGISTER    (HAL_AFEREG_MAP->FCTS16)
#define HAL_AFE_BLNDTRKO_LSB         0
#define HAL_AFE_BLNDTRKO_MSB         7
#define HAL_AFE_BLNDTRKO_MASK        0x000000FF
#define HAL_AFE_BLNDTRKO_TYPE        UByte
#define HAL_AFE_BLNDTRKO             HAL_AFE_BLNDTRKO_MASK
#define HAL_AFE_BLNDTRKO_READABLE    1
#define HAL_AFE_BLNDTRKO_WRITABLE    1
#define HAL_AFE_BLNDTRKO_SIGNED      0
#define HAL_AFE_BLNDTRKO_ADR         0x1000092C

// Register FCTS17
#define HAL_AFE_TRKO_REGISTER    (HAL_AFEREG_MAP->FCTS17)
#define HAL_AFE_TRKO_LSB         0
#define HAL_AFE_TRKO_MSB         7
#define HAL_AFE_TRKO_MASK        0x000000FF
#define HAL_AFE_TRKO_TYPE        UByte
#define HAL_AFE_TRKO             HAL_AFE_TRKO_MASK
#define HAL_AFE_TRKO_READABLE    1
#define HAL_AFE_TRKO_WRITABLE    1
#define HAL_AFE_TRKO_SIGNED      0
#define HAL_AFE_TRKO_ADR         0x1000092D

// Register FCTS18
#define HAL_AFE_LNDTRKO_REGISTER    (HAL_AFEREG_MAP->FCTS18)
#define HAL_AFE_LNDTRKO_LSB         0
#define HAL_AFE_LNDTRKO_MSB         7
#define HAL_AFE_LNDTRKO_MASK        0x000000FF
#define HAL_AFE_LNDTRKO_TYPE        UByte
#define HAL_AFE_LNDTRKO             HAL_AFE_LNDTRKO_MASK
#define HAL_AFE_LNDTRKO_READABLE    1
#define HAL_AFE_LNDTRKO_WRITABLE    1
#define HAL_AFE_LNDTRKO_SIGNED      0
#define HAL_AFE_LNDTRKO_ADR         0x1000092E

// Register FCTS19
#define HAL_AFE_WRTRKO_REGISTER    (HAL_AFEREG_MAP->FCTS19)
#define HAL_AFE_WRTRKO_LSB         0
#define HAL_AFE_WRTRKO_MSB         7
#define HAL_AFE_WRTRKO_MASK        0x000000FF
#define HAL_AFE_WRTRKO_TYPE        UByte
#define HAL_AFE_WRTRKO             HAL_AFE_WRTRKO_MASK
#define HAL_AFE_WRTRKO_READABLE    1
#define HAL_AFE_WRTRKO_WRITABLE    1
#define HAL_AFE_WRTRKO_SIGNED      0
#define HAL_AFE_WRTRKO_ADR         0x1000092F

// Register FCTS20
#define HAL_AFE_BLANKTRKG_REGISTER    (HAL_AFEREG_MAP->FCTS20)
#define HAL_AFE_BLANKTRKG_LSB         0
#define HAL_AFE_BLANKTRKG_MSB         4
#define HAL_AFE_BLANKTRKG_MASK        0x0000001F
#define HAL_AFE_BLANKTRKG_TYPE        UByte
#define HAL_AFE_BLANKTRKG             HAL_AFE_BLANKTRKG_MASK
#define HAL_AFE_BLANKTRKG_READABLE    1
#define HAL_AFE_BLANKTRKG_WRITABLE    1
#define HAL_AFE_BLANKTRKG_SIGNED      0
#define HAL_AFE_BLANKTRKG_ADR         0x10000930

// Register FCTS21
#define HAL_AFE_BLNDTRKG_REGISTER    (HAL_AFEREG_MAP->FCTS21)
#define HAL_AFE_BLNDTRKG_LSB         0
#define HAL_AFE_BLNDTRKG_MSB         4
#define HAL_AFE_BLNDTRKG_MASK        0x0000001F
#define HAL_AFE_BLNDTRKG_TYPE        UByte
#define HAL_AFE_BLNDTRKG             HAL_AFE_BLNDTRKG_MASK
#define HAL_AFE_BLNDTRKG_READABLE    1
#define HAL_AFE_BLNDTRKG_WRITABLE    1
#define HAL_AFE_BLNDTRKG_SIGNED      0
#define HAL_AFE_BLNDTRKG_ADR         0x10000931

// Register FCTS22
#define HAL_AFE_TRKG_REGISTER    (HAL_AFEREG_MAP->FCTS22)
#define HAL_AFE_TRKG_LSB         0
#define HAL_AFE_TRKG_MSB         4
#define HAL_AFE_TRKG_MASK        0x0000001F
#define HAL_AFE_TRKG_TYPE        UByte
#define HAL_AFE_TRKG             HAL_AFE_TRKG_MASK
#define HAL_AFE_TRKG_READABLE    1
#define HAL_AFE_TRKG_WRITABLE    1
#define HAL_AFE_TRKG_SIGNED      0
#define HAL_AFE_TRKG_ADR         0x10000932

// Register FCTS23
#define HAL_AFE_LNDTRKG_REGISTER    (HAL_AFEREG_MAP->FCTS23)
#define HAL_AFE_LNDTRKG_LSB         0
#define HAL_AFE_LNDTRKG_MSB         4
#define HAL_AFE_LNDTRKG_MASK        0x0000001F
#define HAL_AFE_LNDTRKG_TYPE        UByte
#define HAL_AFE_LNDTRKG             HAL_AFE_LNDTRKG_MASK
#define HAL_AFE_LNDTRKG_READABLE    1
#define HAL_AFE_LNDTRKG_WRITABLE    1
#define HAL_AFE_LNDTRKG_SIGNED      0
#define HAL_AFE_LNDTRKG_ADR         0x10000933

// Register FCTS24
#define HAL_AFE_WRTRKG_REGISTER    (HAL_AFEREG_MAP->FCTS24)
#define HAL_AFE_WRTRKG_LSB         0
#define HAL_AFE_WRTRKG_MSB         4
#define HAL_AFE_WRTRKG_MASK        0x0000001F
#define HAL_AFE_WRTRKG_TYPE        UByte
#define HAL_AFE_WRTRKG             HAL_AFE_WRTRKG_MASK
#define HAL_AFE_WRTRKG_READABLE    1
#define HAL_AFE_WRTRKG_WRITABLE    1
#define HAL_AFE_WRTRKG_SIGNED      0
#define HAL_AFE_WRTRKG_ADR         0x10000934

// Register FCTS25

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_TESRVHLD_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TESRVHLD_LSB         5
#define HAL_AFE_TESRVHLD_MSB         5
#define HAL_AFE_TESRVHLD_MASK        0x00000020
#define HAL_AFE_TESRVHLD_TYPE        Bool
#define HAL_AFE_TESRVHLD             HAL_AFE_TESRVHLD_MASK
#define HAL_AFE_TESRVHLD_READABLE    1
#define HAL_AFE_TESRVHLD_WRITABLE    1
#define HAL_AFE_TESRVHLD_SIGNED      0
#define HAL_AFE_TESRVHLD_ADR         0x10000935

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_TEBLNKHLD_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TEBLNKHLD_LSB         4
#define HAL_AFE_TEBLNKHLD_MSB         4
#define HAL_AFE_TEBLNKHLD_MASK        0x00000010
#define HAL_AFE_TEBLNKHLD_TYPE        Bool
#define HAL_AFE_TEBLNKHLD             HAL_AFE_TEBLNKHLD_MASK
#define HAL_AFE_TEBLNKHLD_READABLE    1
#define HAL_AFE_TEBLNKHLD_WRITABLE    1
#define HAL_AFE_TEBLNKHLD_SIGNED      0
#define HAL_AFE_TEBLNKHLD_ADR         0x10000935

#define HAL_AFE_TEHDRHLD_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TEHDRHLD_LSB         3
#define HAL_AFE_TEHDRHLD_MSB         3
#define HAL_AFE_TEHDRHLD_MASK        0x00000008
#define HAL_AFE_TEHDRHLD_TYPE        Bool
#define HAL_AFE_TEHDRHLD             HAL_AFE_TEHDRHLD_MASK
#define HAL_AFE_TEHDRHLD_READABLE    1
#define HAL_AFE_TEHDRHLD_WRITABLE    1
#define HAL_AFE_TEHDRHLD_SIGNED      0
#define HAL_AFE_TEHDRHLD_ADR         0x10000935

#define HAL_AFE_TEREGHLD_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TEREGHLD_LSB         2
#define HAL_AFE_TEREGHLD_MSB         2
#define HAL_AFE_TEREGHLD_MASK        0x00000004
#define HAL_AFE_TEREGHLD_TYPE        Bool
#define HAL_AFE_TEREGHLD             HAL_AFE_TEREGHLD_MASK
#define HAL_AFE_TEREGHLD_READABLE    1
#define HAL_AFE_TEREGHLD_WRITABLE    1
#define HAL_AFE_TEREGHLD_SIGNED      0
#define HAL_AFE_TEREGHLD_ADR         0x10000935

#define HAL_AFE_TELNDINV_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TELNDINV_LSB         1
#define HAL_AFE_TELNDINV_MSB         1
#define HAL_AFE_TELNDINV_MASK        0x00000002
#define HAL_AFE_TELNDINV_TYPE        Bool
#define HAL_AFE_TELNDINV             HAL_AFE_TELNDINV_MASK
#define HAL_AFE_TELNDINV_READABLE    1
#define HAL_AFE_TELNDINV_WRITABLE    1
#define HAL_AFE_TELNDINV_SIGNED      0
#define HAL_AFE_TELNDINV_ADR         0x10000935

#define HAL_AFE_TEINV_REGISTER    (HAL_AFEREG_MAP->FCTS25)
#define HAL_AFE_TEINV_LSB         0
#define HAL_AFE_TEINV_MSB         0
#define HAL_AFE_TEINV_MASK        0x00000001
#define HAL_AFE_TEINV_TYPE        Bool
#define HAL_AFE_TEINV             HAL_AFE_TEINV_MASK
#define HAL_AFE_TEINV_READABLE    1
#define HAL_AFE_TEINV_WRITABLE    1
#define HAL_AFE_TEINV_SIGNED      0
#define HAL_AFE_TEINV_ADR         0x10000935

// Register FCTS26
#define HAL_AFE_TE2OSA_REGISTER    (HAL_AFEREG_MAP->FCTS26)
#define HAL_AFE_TE2OSA_LSB         0
#define HAL_AFE_TE2OSA_MSB         7
#define HAL_AFE_TE2OSA_MASK        0x000000FF
#define HAL_AFE_TE2OSA_TYPE        UByte
#define HAL_AFE_TE2OSA             HAL_AFE_TE2OSA_MASK
#define HAL_AFE_TE2OSA_READABLE    1
#define HAL_AFE_TE2OSA_WRITABLE    1
#define HAL_AFE_TE2OSA_SIGNED      0
#define HAL_AFE_TE2OSA_ADR         0x10000936

// Register FCTS27
#define HAL_AFE_TE2OSB_REGISTER    (HAL_AFEREG_MAP->FCTS27)
#define HAL_AFE_TE2OSB_LSB         0
#define HAL_AFE_TE2OSB_MSB         7
#define HAL_AFE_TE2OSB_MASK        0x000000FF
#define HAL_AFE_TE2OSB_TYPE        UByte
#define HAL_AFE_TE2OSB             HAL_AFE_TE2OSB_MASK
#define HAL_AFE_TE2OSB_READABLE    1
#define HAL_AFE_TE2OSB_WRITABLE    1
#define HAL_AFE_TE2OSB_SIGNED      0
#define HAL_AFE_TE2OSB_ADR         0x10000937

// Register FCTS28
#define HAL_AFE_TE2AUTOEN_REGISTER    (HAL_AFEREG_MAP->FCTS28)
#define HAL_AFE_TE2AUTOEN_LSB         7
#define HAL_AFE_TE2AUTOEN_MSB         7
#define HAL_AFE_TE2AUTOEN_MASK        0x00000080
#define HAL_AFE_TE2AUTOEN_TYPE        halAFE_TE2GnSEL_t
#define HAL_AFE_TE2AUTOEN             HAL_AFE_TE2AUTOEN_MASK
#define HAL_AFE_TE2AUTOEN_READABLE    1
#define HAL_AFE_TE2AUTOEN_WRITABLE    1
#define HAL_AFE_TE2AUTOEN_SIGNED      0
#define HAL_AFE_TE2AUTOEN_ADR         0x10000938

#define HAL_AFE_TE2GNREG_REGISTER    (HAL_AFEREG_MAP->FCTS28)
#define HAL_AFE_TE2GNREG_LSB         0
#define HAL_AFE_TE2GNREG_MSB         4
#define HAL_AFE_TE2GNREG_MASK        0x0000001F
#define HAL_AFE_TE2GNREG_TYPE        UByte
#define HAL_AFE_TE2GNREG             HAL_AFE_TE2GNREG_MASK
#define HAL_AFE_TE2GNREG_READABLE    1
#define HAL_AFE_TE2GNREG_WRITABLE    1
#define HAL_AFE_TE2GNREG_SIGNED      0
#define HAL_AFE_TE2GNREG_ADR         0x10000938

// Register FCTS29
#define HAL_AFE_TOKT_REGISTER    (HAL_AFEREG_MAP->FCTS29)
#define HAL_AFE_TOKT_LSB         5
#define HAL_AFE_TOKT_MSB         7
#define HAL_AFE_TOKT_MASK        0x000000E0
#define HAL_AFE_TOKT_TYPE        halAFE_TOKT_t
#define HAL_AFE_TOKT             HAL_AFE_TOKT_MASK
#define HAL_AFE_TOKT_READABLE    1
#define HAL_AFE_TOKT_WRITABLE    1
#define HAL_AFE_TOKT_SIGNED      0
#define HAL_AFE_TOKT_ADR         0x10000939

#define HAL_AFE_TOKOFF_REGISTER    (HAL_AFEREG_MAP->FCTS29)
#define HAL_AFE_TOKOFF_LSB         4
#define HAL_AFE_TOKOFF_MSB         4
#define HAL_AFE_TOKOFF_MASK        0x00000010
#define HAL_AFE_TOKOFF_TYPE        Bool
#define HAL_AFE_TOKOFF             HAL_AFE_TOKOFF_MASK
#define HAL_AFE_TOKOFF_READABLE    1
#define HAL_AFE_TOKOFF_WRITABLE    1
#define HAL_AFE_TOKOFF_SIGNED      0
#define HAL_AFE_TOKOFF_ADR         0x10000939

#define HAL_AFE_TOKSEL_REGISTER    (HAL_AFEREG_MAP->FCTS29)
#define HAL_AFE_TOKSEL_LSB         3
#define HAL_AFE_TOKSEL_MSB         3
#define HAL_AFE_TOKSEL_MASK        0x00000008
#define HAL_AFE_TOKSEL_TYPE        halAFE_XTOKSEL_t
#define HAL_AFE_TOKSEL             HAL_AFE_TOKSEL_MASK
#define HAL_AFE_TOKSEL_READABLE    1
#define HAL_AFE_TOKSEL_WRITABLE    1
#define HAL_AFE_TOKSEL_SIGNED      0
#define HAL_AFE_TOKSEL_ADR         0x10000939

#define HAL_AFE_TXLNDINV_REGISTER    (HAL_AFEREG_MAP->FCTS29)
#define HAL_AFE_TXLNDINV_LSB         1
#define HAL_AFE_TXLNDINV_MSB         1
#define HAL_AFE_TXLNDINV_MASK        0x00000002
#define HAL_AFE_TXLNDINV_TYPE        Bool
#define HAL_AFE_TXLNDINV             HAL_AFE_TXLNDINV_MASK
#define HAL_AFE_TXLNDINV_READABLE    1
#define HAL_AFE_TXLNDINV_WRITABLE    1
#define HAL_AFE_TXLNDINV_SIGNED      0
#define HAL_AFE_TXLNDINV_ADR         0x10000939

#define HAL_AFE_TXINV_REGISTER    (HAL_AFEREG_MAP->FCTS29)
#define HAL_AFE_TXINV_LSB         0
#define HAL_AFE_TXINV_MSB         0
#define HAL_AFE_TXINV_MASK        0x00000001
#define HAL_AFE_TXINV_TYPE        Bool
#define HAL_AFE_TXINV             HAL_AFE_TXINV_MASK
#define HAL_AFE_TXINV_READABLE    1
#define HAL_AFE_TXINV_WRITABLE    1
#define HAL_AFE_TXINV_SIGNED      0
#define HAL_AFE_TXINV_ADR         0x10000939

// Register FCTS30
#define HAL_AFE_FEFEO_REGISTER    (HAL_AFEREG_MAP->FCTS30)
#define HAL_AFE_FEFEO_LSB         7
#define HAL_AFE_FEFEO_MSB         7
#define HAL_AFE_FEFEO_MASK        0x00000080
#define HAL_AFE_FEFEO_TYPE        Bool
#define HAL_AFE_FEFEO             HAL_AFE_FEFEO_MASK
#define HAL_AFE_FEFEO_READABLE    1
#define HAL_AFE_FEFEO_WRITABLE    1
#define HAL_AFE_FEFEO_SIGNED      0
#define HAL_AFE_FEFEO_ADR         0x1000093A

#define HAL_AFE_FESP2_REGISTER    (HAL_AFEREG_MAP->FCTS30)
#define HAL_AFE_FESP2_LSB         6
#define HAL_AFE_FESP2_MSB         6
#define HAL_AFE_FESP2_MASK        0x00000040
#define HAL_AFE_FESP2_TYPE        Bool
#define HAL_AFE_FESP2             HAL_AFE_FESP2_MASK
#define HAL_AFE_FESP2_READABLE    1
#define HAL_AFE_FESP2_WRITABLE    1
#define HAL_AFE_FESP2_SIGNED      0
#define HAL_AFE_FESP2_ADR         0x1000093A

#define HAL_AFE_FESP2N_REGISTER    (HAL_AFEREG_MAP->FCTS30)
#define HAL_AFE_FESP2N_LSB         5
#define HAL_AFE_FESP2N_MSB         5
#define HAL_AFE_FESP2N_MASK        0x00000020
#define HAL_AFE_FESP2N_TYPE        Bool
#define HAL_AFE_FESP2N             HAL_AFE_FESP2N_MASK
#define HAL_AFE_FESP2N_READABLE    1
#define HAL_AFE_FESP2N_WRITABLE    1
#define HAL_AFE_FESP2N_SIGNED      0
#define HAL_AFE_FESP2N_ADR         0x1000093A

#define HAL_AFE_FESP1_REGISTER    (HAL_AFEREG_MAP->FCTS30)
#define HAL_AFE_FESP1_LSB         4
#define HAL_AFE_FESP1_MSB         4
#define HAL_AFE_FESP1_MASK        0x00000010
#define HAL_AFE_FESP1_TYPE        Bool
#define HAL_AFE_FESP1             HAL_AFE_FESP1_MASK
#define HAL_AFE_FESP1_READABLE    1
#define HAL_AFE_FESP1_WRITABLE    1
#define HAL_AFE_FESP1_SIGNED      0
#define HAL_AFE_FESP1_ADR         0x1000093A

#define HAL_AFE_FEMPP_REGISTER    (HAL_AFEREG_MAP->FCTS30)
#define HAL_AFE_FEMPP_LSB         3
#define HAL_AFE_FEMPP_MSB         3
#define HAL_AFE_FEMPP_MASK        0x00000008
#define HAL_AFE_FEMPP_TYPE        Bool
#define HAL_AFE_FEMPP             HAL_AFE_FEMPP_MASK
#define HAL_AFE_FEMPP_READABLE    1
#define HAL_AFE_FEMPP_WRITABLE    1
#define HAL_AFE_FEMPP_SIGNED      0
#define HAL_AFE_FEMPP_ADR         0x1000093A

// Register FCTS31
#define HAL_AFE_FEPGN_REGISTER    (HAL_AFEREG_MAP->FCTS31)
#define HAL_AFE_FEPGN_LSB         6
#define HAL_AFE_FEPGN_MSB         7
#define HAL_AFE_FEPGN_MASK        0x000000C0
#define HAL_AFE_FEPGN_TYPE        halAFE_FEPGN_t
#define HAL_AFE_FEPGN             HAL_AFE_FEPGN_MASK
#define HAL_AFE_FEPGN_READABLE    1
#define HAL_AFE_FEPGN_WRITABLE    1
#define HAL_AFE_FEPGN_SIGNED      0
#define HAL_AFE_FEPGN_ADR         0x1000093B

#define HAL_AFE_FEBG_REGISTER    (HAL_AFEREG_MAP->FCTS31)
#define HAL_AFE_FEBG_LSB         0
#define HAL_AFE_FEBG_MSB         4
#define HAL_AFE_FEBG_MASK        0x0000001F
#define HAL_AFE_FEBG_TYPE        UByte
#define HAL_AFE_FEBG             HAL_AFE_FEBG_MASK
#define HAL_AFE_FEBG_READABLE    1
#define HAL_AFE_FEBG_WRITABLE    1
#define HAL_AFE_FEBG_SIGNED      0
#define HAL_AFE_FEBG_ADR         0x1000093B

// Register FCTS32
#define HAL_AFE_FE2BYPASS_REGISTER    (HAL_AFEREG_MAP->FCTS32)
#define HAL_AFE_FE2BYPASS_LSB         7
#define HAL_AFE_FE2BYPASS_MSB         7
#define HAL_AFE_FE2BYPASS_MASK        0x00000080
#define HAL_AFE_FE2BYPASS_TYPE        Bool
#define HAL_AFE_FE2BYPASS             HAL_AFE_FE2BYPASS_MASK
#define HAL_AFE_FE2BYPASS_READABLE    1
#define HAL_AFE_FE2BYPASS_WRITABLE    1
#define HAL_AFE_FE2BYPASS_SIGNED      0
#define HAL_AFE_FE2BYPASS_ADR         0x1000093C

#define HAL_AFE_FEGPADCEN_REGISTER    (HAL_AFEREG_MAP->FCTS32)
#define HAL_AFE_FEGPADCEN_LSB         6
#define HAL_AFE_FEGPADCEN_MSB         6
#define HAL_AFE_FEGPADCEN_MASK        0x00000040
#define HAL_AFE_FEGPADCEN_TYPE        Bool
#define HAL_AFE_FEGPADCEN             HAL_AFE_FEGPADCEN_MASK
#define HAL_AFE_FEGPADCEN_READABLE    1
#define HAL_AFE_FEGPADCEN_WRITABLE    1
#define HAL_AFE_FEGPADCEN_SIGNED      0
#define HAL_AFE_FEGPADCEN_ADR         0x1000093C

#define HAL_AFE_FEFCSFSEL_REGISTER    (HAL_AFEREG_MAP->FCTS32)
#define HAL_AFE_FEFCSFSEL_LSB         2
#define HAL_AFE_FEFCSFSEL_MSB         3
#define HAL_AFE_FEFCSFSEL_MASK        0x0000000C
#define HAL_AFE_FEFCSFSEL_TYPE        halAFE_FELPF_t
#define HAL_AFE_FEFCSFSEL             HAL_AFE_FEFCSFSEL_MASK
#define HAL_AFE_FEFCSFSEL_READABLE    1
#define HAL_AFE_FEFCSFSEL_WRITABLE    1
#define HAL_AFE_FEFCSFSEL_SIGNED      0
#define HAL_AFE_FEFCSFSEL_ADR         0x1000093C

#define HAL_AFE_FELPSET_REGISTER    (HAL_AFEREG_MAP->FCTS32)
#define HAL_AFE_FELPSET_LSB         1
#define HAL_AFE_FELPSET_MSB         1
#define HAL_AFE_FELPSET_MASK        0x00000002
#define HAL_AFE_FELPSET_TYPE        Bool
#define HAL_AFE_FELPSET             HAL_AFE_FELPSET_MASK
#define HAL_AFE_FELPSET_READABLE    1
#define HAL_AFE_FELPSET_WRITABLE    1
#define HAL_AFE_FELPSET_SIGNED      0
#define HAL_AFE_FELPSET_ADR         0x1000093C

#define HAL_AFE_BPFELP_REGISTER    (HAL_AFEREG_MAP->FCTS32)
#define HAL_AFE_BPFELP_LSB         0
#define HAL_AFE_BPFELP_MSB         0
#define HAL_AFE_BPFELP_MASK        0x00000001
#define HAL_AFE_BPFELP_TYPE        Bool
#define HAL_AFE_BPFELP             HAL_AFE_BPFELP_MASK
#define HAL_AFE_BPFELP_READABLE    1
#define HAL_AFE_BPFELP_WRITABLE    1
#define HAL_AFE_BPFELP_SIGNED      0
#define HAL_AFE_BPFELP_ADR         0x1000093C

// Register FCTS33
#define HAL_AFE_CEMPP_REGISTER    (HAL_AFEREG_MAP->FCTS33)
#define HAL_AFE_CEMPP_LSB         7
#define HAL_AFE_CEMPP_MSB         7
#define HAL_AFE_CEMPP_MASK        0x00000080
#define HAL_AFE_CEMPP_TYPE        Bool
#define HAL_AFE_CEMPP             HAL_AFE_CEMPP_MASK
#define HAL_AFE_CEMPP_READABLE    1
#define HAL_AFE_CEMPP_WRITABLE    1
#define HAL_AFE_CEMPP_SIGNED      0
#define HAL_AFE_CEMPP_ADR         0x1000093D

#define HAL_AFE_CESP1_REGISTER    (HAL_AFEREG_MAP->FCTS33)
#define HAL_AFE_CESP1_LSB         6
#define HAL_AFE_CESP1_MSB         6
#define HAL_AFE_CESP1_MASK        0x00000040
#define HAL_AFE_CESP1_TYPE        Bool
#define HAL_AFE_CESP1             HAL_AFE_CESP1_MASK
#define HAL_AFE_CESP1_READABLE    1
#define HAL_AFE_CESP1_WRITABLE    1
#define HAL_AFE_CESP1_SIGNED      0
#define HAL_AFE_CESP1_ADR         0x1000093D

#define HAL_AFE_CEBG_REGISTER    (HAL_AFEREG_MAP->FCTS33)
#define HAL_AFE_CEBG_LSB         0
#define HAL_AFE_CEBG_MSB         4
#define HAL_AFE_CEBG_MASK        0x0000001F
#define HAL_AFE_CEBG_TYPE        UByte
#define HAL_AFE_CEBG             HAL_AFE_CEBG_MASK
#define HAL_AFE_CEBG_READABLE    1
#define HAL_AFE_CEBG_WRITABLE    1
#define HAL_AFE_CEBG_SIGNED      0
#define HAL_AFE_CEBG_ADR         0x1000093D

// Register FCTS34
#define HAL_AFE_CEOS_REGISTER    (HAL_AFEREG_MAP->FCTS34)
#define HAL_AFE_CEOS_LSB         0
#define HAL_AFE_CEOS_MSB         7
#define HAL_AFE_CEOS_MASK        0x000000FF
#define HAL_AFE_CEOS_TYPE        UByte
#define HAL_AFE_CEOS             HAL_AFE_CEOS_MASK
#define HAL_AFE_CEOS_READABLE    1
#define HAL_AFE_CEOS_WRITABLE    1
#define HAL_AFE_CEOS_SIGNED      0
#define HAL_AFE_CEOS_ADR         0x1000093E

// Register FCTS35
#define HAL_AFE_CEPGN_REGISTER    (HAL_AFEREG_MAP->FCTS35)
#define HAL_AFE_CEPGN_LSB         6
#define HAL_AFE_CEPGN_MSB         7
#define HAL_AFE_CEPGN_MASK        0x000000C0
#define HAL_AFE_CEPGN_TYPE        halAFE_CEPGN_t
#define HAL_AFE_CEPGN             HAL_AFE_CEPGN_MASK
#define HAL_AFE_CEPGN_READABLE    1
#define HAL_AFE_CEPGN_WRITABLE    1
#define HAL_AFE_CEPGN_SIGNED      0
#define HAL_AFE_CEPGN_ADR         0x1000093F

#define HAL_AFE_CEBG12DB_REGISTER    (HAL_AFEREG_MAP->FCTS35)
#define HAL_AFE_CEBG12DB_LSB         5
#define HAL_AFE_CEBG12DB_MSB         5
#define HAL_AFE_CEBG12DB_MASK        0x00000020
#define HAL_AFE_CEBG12DB_TYPE        Bool
#define HAL_AFE_CEBG12DB             HAL_AFE_CEBG12DB_MASK
#define HAL_AFE_CEBG12DB_READABLE    1
#define HAL_AFE_CEBG12DB_WRITABLE    1
#define HAL_AFE_CEBG12DB_SIGNED      0
#define HAL_AFE_CEBG12DB_ADR         0x1000093F

#define HAL_AFE_CEGN_REGISTER    (HAL_AFEREG_MAP->FCTS35)
#define HAL_AFE_CEGN_LSB         0
#define HAL_AFE_CEGN_MSB         4
#define HAL_AFE_CEGN_MASK        0x0000001F
#define HAL_AFE_CEGN_TYPE        UByte
#define HAL_AFE_CEGN             HAL_AFE_CEGN_MASK
#define HAL_AFE_CEGN_READABLE    1
#define HAL_AFE_CEGN_WRITABLE    1
#define HAL_AFE_CEGN_SIGNED      0
#define HAL_AFE_CEGN_ADR         0x1000093F

// Register FCTS36
#define HAL_AFE_TEMPP_REGISTER    (HAL_AFEREG_MAP->FCTS36)
#define HAL_AFE_TEMPP_LSB         7
#define HAL_AFE_TEMPP_MSB         7
#define HAL_AFE_TEMPP_MASK        0x00000080
#define HAL_AFE_TEMPP_TYPE        Bool
#define HAL_AFE_TEMPP             HAL_AFE_TEMPP_MASK
#define HAL_AFE_TEMPP_READABLE    1
#define HAL_AFE_TEMPP_WRITABLE    1
#define HAL_AFE_TEMPP_SIGNED      0
#define HAL_AFE_TEMPP_ADR         0x10000940

#define HAL_AFE_TESP1_REGISTER    (HAL_AFEREG_MAP->FCTS36)
#define HAL_AFE_TESP1_LSB         6
#define HAL_AFE_TESP1_MSB         6
#define HAL_AFE_TESP1_MASK        0x00000040
#define HAL_AFE_TESP1_TYPE        Bool
#define HAL_AFE_TESP1             HAL_AFE_TESP1_MASK
#define HAL_AFE_TESP1_READABLE    1
#define HAL_AFE_TESP1_WRITABLE    1
#define HAL_AFE_TESP1_SIGNED      0
#define HAL_AFE_TESP1_ADR         0x10000940

#define HAL_AFE_TEDPD_REGISTER    (HAL_AFEREG_MAP->FCTS36)
#define HAL_AFE_TEDPD_LSB         5
#define HAL_AFE_TEDPD_MSB         5
#define HAL_AFE_TEDPD_MASK        0x00000020
#define HAL_AFE_TEDPD_TYPE        Bool
#define HAL_AFE_TEDPD             HAL_AFE_TEDPD_MASK
#define HAL_AFE_TEDPD_READABLE    1
#define HAL_AFE_TEDPD_WRITABLE    1
#define HAL_AFE_TEDPD_SIGNED      0
#define HAL_AFE_TEDPD_ADR         0x10000940

#define HAL_AFE_CEFCSFSEL_REGISTER    (HAL_AFEREG_MAP->FCTS36)
#define HAL_AFE_CEFCSFSEL_LSB         2
#define HAL_AFE_CEFCSFSEL_MSB         3
#define HAL_AFE_CEFCSFSEL_MASK        0x0000000C
#define HAL_AFE_CEFCSFSEL_TYPE        halAFE_CELPF_t
#define HAL_AFE_CEFCSFSEL             HAL_AFE_CEFCSFSEL_MASK
#define HAL_AFE_CEFCSFSEL_READABLE    1
#define HAL_AFE_CEFCSFSEL_WRITABLE    1
#define HAL_AFE_CEFCSFSEL_SIGNED      0
#define HAL_AFE_CEFCSFSEL_ADR         0x10000940

#define HAL_AFE_BPCELP_REGISTER    (HAL_AFEREG_MAP->FCTS36)
#define HAL_AFE_BPCELP_LSB         0
#define HAL_AFE_BPCELP_MSB         0
#define HAL_AFE_BPCELP_MASK        0x00000001
#define HAL_AFE_BPCELP_TYPE        Bool
#define HAL_AFE_BPCELP             HAL_AFE_BPCELP_MASK
#define HAL_AFE_BPCELP_READABLE    1
#define HAL_AFE_BPCELP_WRITABLE    1
#define HAL_AFE_BPCELP_SIGNED      0
#define HAL_AFE_BPCELP_ADR         0x10000940

// Register FCTS37
#define HAL_AFE_TEPGN_REGISTER    (HAL_AFEREG_MAP->FCTS37)
#define HAL_AFE_TEPGN_LSB         6
#define HAL_AFE_TEPGN_MSB         7
#define HAL_AFE_TEPGN_MASK        0x000000C0
#define HAL_AFE_TEPGN_TYPE        halAFE_TEPGN_t
#define HAL_AFE_TEPGN             HAL_AFE_TEPGN_MASK
#define HAL_AFE_TEPGN_READABLE    1
#define HAL_AFE_TEPGN_WRITABLE    1
#define HAL_AFE_TEPGN_SIGNED      0
#define HAL_AFE_TEPGN_ADR         0x10000941

#define HAL_AFE_TEBG_REGISTER    (HAL_AFEREG_MAP->FCTS37)
#define HAL_AFE_TEBG_LSB         0
#define HAL_AFE_TEBG_MSB         4
#define HAL_AFE_TEBG_MASK        0x0000001F
#define HAL_AFE_TEBG_TYPE        UByte
#define HAL_AFE_TEBG             HAL_AFE_TEBG_MASK
#define HAL_AFE_TEBG_READABLE    1
#define HAL_AFE_TEBG_WRITABLE    1
#define HAL_AFE_TEBG_SIGNED      0
#define HAL_AFE_TEBG_ADR         0x10000941

// Register FCTS38
#define HAL_AFE_TE2BYPASS_REGISTER    (HAL_AFEREG_MAP->FCTS38)
#define HAL_AFE_TE2BYPASS_LSB         7
#define HAL_AFE_TE2BYPASS_MSB         7
#define HAL_AFE_TE2BYPASS_MASK        0x00000080
#define HAL_AFE_TE2BYPASS_TYPE        Bool
#define HAL_AFE_TE2BYPASS             HAL_AFE_TE2BYPASS_MASK
#define HAL_AFE_TE2BYPASS_READABLE    1
#define HAL_AFE_TE2BYPASS_WRITABLE    1
#define HAL_AFE_TE2BYPASS_SIGNED      0
#define HAL_AFE_TE2BYPASS_ADR         0x10000942

#define HAL_AFE_TEGPADCEN_REGISTER    (HAL_AFEREG_MAP->FCTS38)
#define HAL_AFE_TEGPADCEN_LSB         6
#define HAL_AFE_TEGPADCEN_MSB         6
#define HAL_AFE_TEGPADCEN_MASK        0x00000040
#define HAL_AFE_TEGPADCEN_TYPE        Bool
#define HAL_AFE_TEGPADCEN             HAL_AFE_TEGPADCEN_MASK
#define HAL_AFE_TEGPADCEN_READABLE    1
#define HAL_AFE_TEGPADCEN_WRITABLE    1
#define HAL_AFE_TEGPADCEN_SIGNED      0
#define HAL_AFE_TEGPADCEN_ADR         0x10000942

#define HAL_AFE_TEFCSFSEL_REGISTER    (HAL_AFEREG_MAP->FCTS38)
#define HAL_AFE_TEFCSFSEL_LSB         2
#define HAL_AFE_TEFCSFSEL_MSB         3
#define HAL_AFE_TEFCSFSEL_MASK        0x0000000C
#define HAL_AFE_TEFCSFSEL_TYPE        halAFE_TELPF_t
#define HAL_AFE_TEFCSFSEL             HAL_AFE_TEFCSFSEL_MASK
#define HAL_AFE_TEFCSFSEL_READABLE    1
#define HAL_AFE_TEFCSFSEL_WRITABLE    1
#define HAL_AFE_TEFCSFSEL_SIGNED      0
#define HAL_AFE_TEFCSFSEL_ADR         0x10000942

#define HAL_AFE_TELPSET_REGISTER    (HAL_AFEREG_MAP->FCTS38)
#define HAL_AFE_TELPSET_LSB         1
#define HAL_AFE_TELPSET_MSB         1
#define HAL_AFE_TELPSET_MASK        0x00000002
#define HAL_AFE_TELPSET_TYPE        Bool
#define HAL_AFE_TELPSET             HAL_AFE_TELPSET_MASK
#define HAL_AFE_TELPSET_READABLE    1
#define HAL_AFE_TELPSET_WRITABLE    1
#define HAL_AFE_TELPSET_SIGNED      0
#define HAL_AFE_TELPSET_ADR         0x10000942

#define HAL_AFE_BPTELP_REGISTER    (HAL_AFEREG_MAP->FCTS38)
#define HAL_AFE_BPTELP_LSB         0
#define HAL_AFE_BPTELP_MSB         0
#define HAL_AFE_BPTELP_MASK        0x00000001
#define HAL_AFE_BPTELP_TYPE        Bool
#define HAL_AFE_BPTELP             HAL_AFE_BPTELP_MASK
#define HAL_AFE_BPTELP_READABLE    1
#define HAL_AFE_BPTELP_WRITABLE    1
#define HAL_AFE_BPTELP_SIGNED      0
#define HAL_AFE_BPTELP_ADR         0x10000942

// Register FCTS39
#define HAL_AFE_TXLP_REGISTER    (HAL_AFEREG_MAP->FCTS39)
#define HAL_AFE_TXLP_LSB         2
#define HAL_AFE_TXLP_MSB         3
#define HAL_AFE_TXLP_MASK        0x0000000C
#define HAL_AFE_TXLP_TYPE        halAFE_TXLPF_t
#define HAL_AFE_TXLP             HAL_AFE_TXLP_MASK
#define HAL_AFE_TXLP_READABLE    1
#define HAL_AFE_TXLP_WRITABLE    1
#define HAL_AFE_TXLP_SIGNED      0
#define HAL_AFE_TXLP_ADR         0x10000943

#define HAL_AFE_TXLPSET_REGISTER    (HAL_AFEREG_MAP->FCTS39)
#define HAL_AFE_TXLPSET_LSB         1
#define HAL_AFE_TXLPSET_MSB         1
#define HAL_AFE_TXLPSET_MASK        0x00000002
#define HAL_AFE_TXLPSET_TYPE        Bool
#define HAL_AFE_TXLPSET             HAL_AFE_TXLPSET_MASK
#define HAL_AFE_TXLPSET_READABLE    1
#define HAL_AFE_TXLPSET_WRITABLE    1
#define HAL_AFE_TXLPSET_SIGNED      0
#define HAL_AFE_TXLPSET_ADR         0x10000943

#define HAL_AFE_BPTXLP_REGISTER    (HAL_AFEREG_MAP->FCTS39)
#define HAL_AFE_BPTXLP_LSB         0
#define HAL_AFE_BPTXLP_MSB         0
#define HAL_AFE_BPTXLP_MASK        0x00000001
#define HAL_AFE_BPTXLP_TYPE        Bool
#define HAL_AFE_BPTXLP             HAL_AFE_BPTXLP_MASK
#define HAL_AFE_BPTXLP_READABLE    1
#define HAL_AFE_BPTXLP_WRITABLE    1
#define HAL_AFE_BPTXLP_SIGNED      0
#define HAL_AFE_BPTXLP_ADR         0x10000943

// Register FCTS40
#define HAL_AFE_TOKLVL_REGISTER    (HAL_AFEREG_MAP->FCTS40)
#define HAL_AFE_TOKLVL_LSB         0
#define HAL_AFE_TOKLVL_MSB         4
#define HAL_AFE_TOKLVL_MASK        0x0000001F
#define HAL_AFE_TOKLVL_TYPE        halAFE_TOKLVL_t
#define HAL_AFE_TOKLVL             HAL_AFE_TOKLVL_MASK
#define HAL_AFE_TOKLVL_READABLE    1
#define HAL_AFE_TOKLVL_WRITABLE    1
#define HAL_AFE_TOKLVL_SIGNED      0
#define HAL_AFE_TOKLVL_ADR         0x10000944

// Register MB00
#define HAL_AFE_MPPOSA_REGISTER    (HAL_AFEREG_MAP->MB00)
#define HAL_AFE_MPPOSA_LSB         0
#define HAL_AFE_MPPOSA_MSB         7
#define HAL_AFE_MPPOSA_MASK        0x000000FF
#define HAL_AFE_MPPOSA_TYPE        UByte
#define HAL_AFE_MPPOSA             HAL_AFE_MPPOSA_MASK
#define HAL_AFE_MPPOSA_READABLE    1
#define HAL_AFE_MPPOSA_WRITABLE    1
#define HAL_AFE_MPPOSA_SIGNED      0
#define HAL_AFE_MPPOSA_ADR         0x10000945

// Register MB01
#define HAL_AFE_MPPOSB_REGISTER    (HAL_AFEREG_MAP->MB01)
#define HAL_AFE_MPPOSB_LSB         0
#define HAL_AFE_MPPOSB_MSB         7
#define HAL_AFE_MPPOSB_MASK        0x000000FF
#define HAL_AFE_MPPOSB_TYPE        UByte
#define HAL_AFE_MPPOSB             HAL_AFE_MPPOSB_MASK
#define HAL_AFE_MPPOSB_READABLE    1
#define HAL_AFE_MPPOSB_WRITABLE    1
#define HAL_AFE_MPPOSB_SIGNED      0
#define HAL_AFE_MPPOSB_ADR         0x10000946

// Register MB02
#define HAL_AFE_FEOOSA_REGISTER    (HAL_AFEREG_MAP->MB02)
#define HAL_AFE_FEOOSA_LSB         0
#define HAL_AFE_FEOOSA_MSB         7
#define HAL_AFE_FEOOSA_MASK        0x000000FF
#define HAL_AFE_FEOOSA_TYPE        UByte
#define HAL_AFE_FEOOSA             HAL_AFE_FEOOSA_MASK
#define HAL_AFE_FEOOSA_READABLE    1
#define HAL_AFE_FEOOSA_WRITABLE    1
#define HAL_AFE_FEOOSA_SIGNED      0
#define HAL_AFE_FEOOSA_ADR         0x10000947

// Register MB03
#define HAL_AFE_FEOOSB_REGISTER    (HAL_AFEREG_MAP->MB03)
#define HAL_AFE_FEOOSB_LSB         0
#define HAL_AFE_FEOOSB_MSB         7
#define HAL_AFE_FEOOSB_MASK        0x000000FF
#define HAL_AFE_FEOOSB_TYPE        UByte
#define HAL_AFE_FEOOSB             HAL_AFE_FEOOSB_MASK
#define HAL_AFE_FEOOSB_READABLE    1
#define HAL_AFE_FEOOSB_WRITABLE    1
#define HAL_AFE_FEOOSB_SIGNED      0
#define HAL_AFE_FEOOSB_ADR         0x10000948

// Register MB04
#define HAL_AFE_SRFOSA_REGISTER    (HAL_AFEREG_MAP->MB04)
#define HAL_AFE_SRFOSA_LSB         0
#define HAL_AFE_SRFOSA_MSB         7
#define HAL_AFE_SRFOSA_MASK        0x000000FF
#define HAL_AFE_SRFOSA_TYPE        UByte
#define HAL_AFE_SRFOSA             HAL_AFE_SRFOSA_MASK
#define HAL_AFE_SRFOSA_READABLE    1
#define HAL_AFE_SRFOSA_WRITABLE    1
#define HAL_AFE_SRFOSA_SIGNED      0
#define HAL_AFE_SRFOSA_ADR         0x10000949

// Register MB05
#define HAL_AFE_SRFOSB_REGISTER    (HAL_AFEREG_MAP->MB05)
#define HAL_AFE_SRFOSB_LSB         0
#define HAL_AFE_SRFOSB_MSB         7
#define HAL_AFE_SRFOSB_MASK        0x000000FF
#define HAL_AFE_SRFOSB_TYPE        UByte
#define HAL_AFE_SRFOSB             HAL_AFE_SRFOSB_MASK
#define HAL_AFE_SRFOSB_READABLE    1
#define HAL_AFE_SRFOSB_WRITABLE    1
#define HAL_AFE_SRFOSB_SIGNED      0
#define HAL_AFE_SRFOSB_ADR         0x1000094A

// Register MB06
#define HAL_AFE_SRFOBGA_REGISTER    (HAL_AFEREG_MAP->MB06)
#define HAL_AFE_SRFOBGA_LSB         0
#define HAL_AFE_SRFOBGA_MSB         4
#define HAL_AFE_SRFOBGA_MASK        0x0000001F
#define HAL_AFE_SRFOBGA_TYPE        UByte
#define HAL_AFE_SRFOBGA             HAL_AFE_SRFOBGA_MASK
#define HAL_AFE_SRFOBGA_READABLE    1
#define HAL_AFE_SRFOBGA_WRITABLE    1
#define HAL_AFE_SRFOBGA_SIGNED      0
#define HAL_AFE_SRFOBGA_ADR         0x1000094B

// Register MB07
#define HAL_AFE_SRFOBGB_REGISTER    (HAL_AFEREG_MAP->MB07)
#define HAL_AFE_SRFOBGB_LSB         0
#define HAL_AFE_SRFOBGB_MSB         4
#define HAL_AFE_SRFOBGB_MASK        0x0000001F
#define HAL_AFE_SRFOBGB_TYPE        UByte
#define HAL_AFE_SRFOBGB             HAL_AFE_SRFOBGB_MASK
#define HAL_AFE_SRFOBGB_READABLE    1
#define HAL_AFE_SRFOBGB_WRITABLE    1
#define HAL_AFE_SRFOBGB_SIGNED      0
#define HAL_AFE_SRFOBGB_ADR         0x1000094C

// Register MB08
#define HAL_AFE_MBVREFOSA_REGISTER    (HAL_AFEREG_MAP->MB08)
#define HAL_AFE_MBVREFOSA_LSB         0
#define HAL_AFE_MBVREFOSA_MSB         7
#define HAL_AFE_MBVREFOSA_MASK        0x000000FF
#define HAL_AFE_MBVREFOSA_TYPE        UByte
#define HAL_AFE_MBVREFOSA             HAL_AFE_MBVREFOSA_MASK
#define HAL_AFE_MBVREFOSA_READABLE    1
#define HAL_AFE_MBVREFOSA_WRITABLE    1
#define HAL_AFE_MBVREFOSA_SIGNED      0
#define HAL_AFE_MBVREFOSA_ADR         0x1000094D

// Register MB09
#define HAL_AFE_MBVREFOSB_REGISTER    (HAL_AFEREG_MAP->MB09)
#define HAL_AFE_MBVREFOSB_LSB         0
#define HAL_AFE_MBVREFOSB_MSB         7
#define HAL_AFE_MBVREFOSB_MASK        0x000000FF
#define HAL_AFE_MBVREFOSB_TYPE        UByte
#define HAL_AFE_MBVREFOSB             HAL_AFE_MBVREFOSB_MASK
#define HAL_AFE_MBVREFOSB_READABLE    1
#define HAL_AFE_MBVREFOSB_WRITABLE    1
#define HAL_AFE_MBVREFOSB_SIGNED      0
#define HAL_AFE_MBVREFOSB_ADR         0x1000094E

// Register MB10
#define HAL_AFE_GUPEN_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPEN_LSB         7
#define HAL_AFE_GUPEN_MSB         7
#define HAL_AFE_GUPEN_MASK        0x00000080
#define HAL_AFE_GUPEN_TYPE        Bool
#define HAL_AFE_GUPEN             HAL_AFE_GUPEN_MASK
#define HAL_AFE_GUPEN_READABLE    1
#define HAL_AFE_GUPEN_WRITABLE    1
#define HAL_AFE_GUPEN_SIGNED      0
#define HAL_AFE_GUPEN_ADR         0x1000094F

#define HAL_AFE_GUPON_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPON_LSB         6
#define HAL_AFE_GUPON_MSB         6
#define HAL_AFE_GUPON_MASK        0x00000040
#define HAL_AFE_GUPON_TYPE        Bool
#define HAL_AFE_GUPON             HAL_AFE_GUPON_MASK
#define HAL_AFE_GUPON_READABLE    1
#define HAL_AFE_GUPON_WRITABLE    1
#define HAL_AFE_GUPON_SIGNED      0
#define HAL_AFE_GUPON_ADR         0x1000094F

#define HAL_AFE_GUPOFF_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPOFF_LSB         5
#define HAL_AFE_GUPOFF_MSB         5
#define HAL_AFE_GUPOFF_MASK        0x00000020
#define HAL_AFE_GUPOFF_TYPE        Bool
#define HAL_AFE_GUPOFF             HAL_AFE_GUPOFF_MASK
#define HAL_AFE_GUPOFF_READABLE    1
#define HAL_AFE_GUPOFF_WRITABLE    1
#define HAL_AFE_GUPOFF_SIGNED      0
#define HAL_AFE_GUPOFF_ADR         0x1000094F

#define HAL_AFE_GUPINV_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPINV_LSB         4
#define HAL_AFE_GUPINV_MSB         4
#define HAL_AFE_GUPINV_MASK        0x00000010
#define HAL_AFE_GUPINV_TYPE        Bool
#define HAL_AFE_GUPINV             HAL_AFE_GUPINV_MASK
#define HAL_AFE_GUPINV_READABLE    1
#define HAL_AFE_GUPINV_WRITABLE    1
#define HAL_AFE_GUPINV_SIGNED      0
#define HAL_AFE_GUPINV_ADR         0x1000094F

#define HAL_AFE_GUPMONOEN_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPMONOEN_LSB         2
#define HAL_AFE_GUPMONOEN_MSB         2
#define HAL_AFE_GUPMONOEN_MASK        0x00000004
#define HAL_AFE_GUPMONOEN_TYPE        Bool
#define HAL_AFE_GUPMONOEN             HAL_AFE_GUPMONOEN_MASK
#define HAL_AFE_GUPMONOEN_READABLE    1
#define HAL_AFE_GUPMONOEN_WRITABLE    1
#define HAL_AFE_GUPMONOEN_SIGNED      0
#define HAL_AFE_GUPMONOEN_ADR         0x1000094F

#define HAL_AFE_GUPDLY_REGISTER    (HAL_AFEREG_MAP->MB10)
#define HAL_AFE_GUPDLY_LSB         0
#define HAL_AFE_GUPDLY_MSB         1
#define HAL_AFE_GUPDLY_MASK        0x00000003
#define HAL_AFE_GUPDLY_TYPE        halAFE_GUPDLY_t
#define HAL_AFE_GUPDLY             HAL_AFE_GUPDLY_MASK
#define HAL_AFE_GUPDLY_READABLE    1
#define HAL_AFE_GUPDLY_WRITABLE    1
#define HAL_AFE_GUPDLY_SIGNED      0
#define HAL_AFE_GUPDLY_ADR         0x1000094F

// Register MB11
#define HAL_AFE_ATTONON_REGISTER    (HAL_AFEREG_MAP->MB11)
#define HAL_AFE_ATTONON_LSB         7
#define HAL_AFE_ATTONON_MSB         7
#define HAL_AFE_ATTONON_MASK        0x00000080
#define HAL_AFE_ATTONON_TYPE        Bool
#define HAL_AFE_ATTONON             HAL_AFE_ATTONON_MASK
#define HAL_AFE_ATTONON_READABLE    1
#define HAL_AFE_ATTONON_WRITABLE    1
#define HAL_AFE_ATTONON_SIGNED      0
#define HAL_AFE_ATTONON_ADR         0x10000950

#define HAL_AFE_ATTOFF_REGISTER    (HAL_AFEREG_MAP->MB11)
#define HAL_AFE_ATTOFF_LSB         6
#define HAL_AFE_ATTOFF_MSB         6
#define HAL_AFE_ATTOFF_MASK        0x00000040
#define HAL_AFE_ATTOFF_TYPE        Bool
#define HAL_AFE_ATTOFF             HAL_AFE_ATTOFF_MASK
#define HAL_AFE_ATTOFF_READABLE    1
#define HAL_AFE_ATTOFF_WRITABLE    1
#define HAL_AFE_ATTOFF_SIGNED      0
#define HAL_AFE_ATTOFF_ADR         0x10000950

#define HAL_AFE_ATTINV_REGISTER    (HAL_AFEREG_MAP->MB11)
#define HAL_AFE_ATTINV_LSB         5
#define HAL_AFE_ATTINV_MSB         5
#define HAL_AFE_ATTINV_MASK        0x00000020
#define HAL_AFE_ATTINV_TYPE        Bool
#define HAL_AFE_ATTINV             HAL_AFE_ATTINV_MASK
#define HAL_AFE_ATTINV_READABLE    1
#define HAL_AFE_ATTINV_WRITABLE    1
#define HAL_AFE_ATTINV_SIGNED      0
#define HAL_AFE_ATTINV_ADR         0x10000950

#define HAL_AFE_MBGUPSEL_REGISTER    (HAL_AFEREG_MAP->MB11)
#define HAL_AFE_MBGUPSEL_LSB         0
#define HAL_AFE_MBGUPSEL_MSB         4
#define HAL_AFE_MBGUPSEL_MASK        0x0000001F
#define HAL_AFE_MBGUPSEL_TYPE        UByte
#define HAL_AFE_MBGUPSEL             HAL_AFE_MBGUPSEL_MASK
#define HAL_AFE_MBGUPSEL_READABLE    1
#define HAL_AFE_MBGUPSEL_WRITABLE    1
#define HAL_AFE_MBGUPSEL_SIGNED      0
#define HAL_AFE_MBGUPSEL_ADR         0x10000950

// Register MB12
#define HAL_AFE_MBATTSEL_REGISTER    (HAL_AFEREG_MAP->MB12)
#define HAL_AFE_MBATTSEL_LSB         0
#define HAL_AFE_MBATTSEL_MSB         4
#define HAL_AFE_MBATTSEL_MASK        0x0000001F
#define HAL_AFE_MBATTSEL_TYPE        UByte
#define HAL_AFE_MBATTSEL             HAL_AFE_MBATTSEL_MASK
#define HAL_AFE_MBATTSEL_READABLE    1
#define HAL_AFE_MBATTSEL_WRITABLE    1
#define HAL_AFE_MBATTSEL_SIGNED      0
#define HAL_AFE_MBATTSEL_ADR         0x10000951

// Register MB13
#define HAL_AFE_WLDONSEL_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_WLDONSEL_LSB         7
#define HAL_AFE_WLDONSEL_MSB         7
#define HAL_AFE_WLDONSEL_MASK        0x00000080
#define HAL_AFE_WLDONSEL_TYPE        Bool
#define HAL_AFE_WLDONSEL             HAL_AFE_WLDONSEL_MASK
#define HAL_AFE_WLDONSEL_READABLE    1
#define HAL_AFE_WLDONSEL_WRITABLE    1
#define HAL_AFE_WLDONSEL_SIGNED      0
#define HAL_AFE_WLDONSEL_ADR         0x10000952

#define HAL_AFE_DEFECTHLD_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_DEFECTHLD_LSB         6
#define HAL_AFE_DEFECTHLD_MSB         6
#define HAL_AFE_DEFECTHLD_MASK        0x00000040
#define HAL_AFE_DEFECTHLD_TYPE        Bool
#define HAL_AFE_DEFECTHLD             HAL_AFE_DEFECTHLD_MASK
#define HAL_AFE_DEFECTHLD_READABLE    1
#define HAL_AFE_DEFECTHLD_WRITABLE    1
#define HAL_AFE_DEFECTHLD_SIGNED      0
#define HAL_AFE_DEFECTHLD_ADR         0x10000952

#define HAL_AFE_BLANKSEL_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_BLANKSEL_LSB         5
#define HAL_AFE_BLANKSEL_MSB         5
#define HAL_AFE_BLANKSEL_MASK        0x00000020
#define HAL_AFE_BLANKSEL_TYPE        Bool
#define HAL_AFE_BLANKSEL             HAL_AFE_BLANKSEL_MASK
#define HAL_AFE_BLANKSEL_READABLE    1
#define HAL_AFE_BLANKSEL_WRITABLE    1
#define HAL_AFE_BLANKSEL_SIGNED      0
#define HAL_AFE_BLANKSEL_ADR         0x10000952

#define HAL_AFE_LANDSEL_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_LANDSEL_LSB         4
#define HAL_AFE_LANDSEL_MSB         4
#define HAL_AFE_LANDSEL_MASK        0x00000010
#define HAL_AFE_LANDSEL_TYPE        Bool
#define HAL_AFE_LANDSEL             HAL_AFE_LANDSEL_MASK
#define HAL_AFE_LANDSEL_READABLE    1
#define HAL_AFE_LANDSEL_WRITABLE    1
#define HAL_AFE_LANDSEL_SIGNED      0
#define HAL_AFE_LANDSEL_ADR         0x10000952

#define HAL_AFE_HDREN_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_HDREN_LSB         1
#define HAL_AFE_HDREN_MSB         1
#define HAL_AFE_HDREN_MASK        0x00000002
#define HAL_AFE_HDREN_TYPE        Bool
#define HAL_AFE_HDREN             HAL_AFE_HDREN_MASK
#define HAL_AFE_HDREN_READABLE    1
#define HAL_AFE_HDREN_WRITABLE    1
#define HAL_AFE_HDREN_SIGNED      0
#define HAL_AFE_HDREN_ADR         0x10000952

#define HAL_AFE_HDREN2_REGISTER    (HAL_AFEREG_MAP->MB13)
#define HAL_AFE_HDREN2_LSB         0
#define HAL_AFE_HDREN2_MSB         0
#define HAL_AFE_HDREN2_MASK        0x00000001
#define HAL_AFE_HDREN2_TYPE        Bool
#define HAL_AFE_HDREN2             HAL_AFE_HDREN2_MASK
#define HAL_AFE_HDREN2_READABLE    1
#define HAL_AFE_HDREN2_WRITABLE    1
#define HAL_AFE_HDREN2_SIGNED      0
#define HAL_AFE_HDREN2_ADR         0x10000952

// Register MB14
#define HAL_AFE_SVOAUXEN_REGISTER    (HAL_AFEREG_MAP->MB14)
#define HAL_AFE_SVOAUXEN_LSB         7
#define HAL_AFE_SVOAUXEN_MSB         7
#define HAL_AFE_SVOAUXEN_MASK        0x00000080
#define HAL_AFE_SVOAUXEN_TYPE        Bool
#define HAL_AFE_SVOAUXEN             HAL_AFE_SVOAUXEN_MASK
#define HAL_AFE_SVOAUXEN_READABLE    1
#define HAL_AFE_SVOAUXEN_WRITABLE    1
#define HAL_AFE_SVOAUXEN_SIGNED      0
#define HAL_AFE_SVOAUXEN_ADR         0x10000953

#define HAL_AFE_BWSEL_REGISTER    (HAL_AFEREG_MAP->MB14)
#define HAL_AFE_BWSEL_LSB         5
#define HAL_AFE_BWSEL_MSB         6
#define HAL_AFE_BWSEL_MASK        0x00000060
#define HAL_AFE_BWSEL_TYPE        halAFE_MBSBBW_t
#define HAL_AFE_BWSEL             HAL_AFE_BWSEL_MASK
#define HAL_AFE_BWSEL_READABLE    1
#define HAL_AFE_BWSEL_WRITABLE    1
#define HAL_AFE_BWSEL_SIGNED      0
#define HAL_AFE_BWSEL_ADR         0x10000953

#define HAL_AFE_MPDSHON_REGISTER    (HAL_AFEREG_MAP->MB14)
#define HAL_AFE_MPDSHON_LSB         1
#define HAL_AFE_MPDSHON_MSB         1
#define HAL_AFE_MPDSHON_MASK        0x00000002
#define HAL_AFE_MPDSHON_TYPE        Bool
#define HAL_AFE_MPDSHON             HAL_AFE_MPDSHON_MASK
#define HAL_AFE_MPDSHON_READABLE    1
#define HAL_AFE_MPDSHON_WRITABLE    1
#define HAL_AFE_MPDSHON_SIGNED      0
#define HAL_AFE_MPDSHON_ADR         0x10000953

#define HAL_AFE_MPDSHLP_REGISTER    (HAL_AFEREG_MAP->MB14)
#define HAL_AFE_MPDSHLP_LSB         0
#define HAL_AFE_MPDSHLP_MSB         0
#define HAL_AFE_MPDSHLP_MASK        0x00000001
#define HAL_AFE_MPDSHLP_TYPE        Bool
#define HAL_AFE_MPDSHLP             HAL_AFE_MPDSHLP_MASK
#define HAL_AFE_MPDSHLP_READABLE    1
#define HAL_AFE_MPDSHLP_WRITABLE    1
#define HAL_AFE_MPDSHLP_SIGNED      0
#define HAL_AFE_MPDSHLP_ADR         0x10000953

// Register MB15
#define HAL_AFE_MPPA_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPA_LSB         7
#define HAL_AFE_MPPA_MSB         7
#define HAL_AFE_MPPA_MASK        0x00000080
#define HAL_AFE_MPPA_TYPE        Bool
#define HAL_AFE_MPPA             HAL_AFE_MPPA_MASK
#define HAL_AFE_MPPA_READABLE    1
#define HAL_AFE_MPPA_WRITABLE    1
#define HAL_AFE_MPPA_SIGNED      0
#define HAL_AFE_MPPA_ADR         0x10000954

#define HAL_AFE_MPPB_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPB_LSB         6
#define HAL_AFE_MPPB_MSB         6
#define HAL_AFE_MPPB_MASK        0x00000040
#define HAL_AFE_MPPB_TYPE        Bool
#define HAL_AFE_MPPB             HAL_AFE_MPPB_MASK
#define HAL_AFE_MPPB_READABLE    1
#define HAL_AFE_MPPB_WRITABLE    1
#define HAL_AFE_MPPB_SIGNED      0
#define HAL_AFE_MPPB_ADR         0x10000954

#define HAL_AFE_MPPC_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPC_LSB         5
#define HAL_AFE_MPPC_MSB         5
#define HAL_AFE_MPPC_MASK        0x00000020
#define HAL_AFE_MPPC_TYPE        Bool
#define HAL_AFE_MPPC             HAL_AFE_MPPC_MASK
#define HAL_AFE_MPPC_READABLE    1
#define HAL_AFE_MPPC_WRITABLE    1
#define HAL_AFE_MPPC_SIGNED      0
#define HAL_AFE_MPPC_ADR         0x10000954

#define HAL_AFE_MPPD_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPD_LSB         4
#define HAL_AFE_MPPD_MSB         4
#define HAL_AFE_MPPD_MASK        0x00000010
#define HAL_AFE_MPPD_TYPE        Bool
#define HAL_AFE_MPPD             HAL_AFE_MPPD_MASK
#define HAL_AFE_MPPD_READABLE    1
#define HAL_AFE_MPPD_WRITABLE    1
#define HAL_AFE_MPPD_SIGNED      0
#define HAL_AFE_MPPD_ADR         0x10000954

#define HAL_AFE_MPPOPGN_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPOPGN_LSB         2
#define HAL_AFE_MPPOPGN_MSB         3
#define HAL_AFE_MPPOPGN_MASK        0x0000000C
#define HAL_AFE_MPPOPGN_TYPE        halAFE_MPPOPGN_t
#define HAL_AFE_MPPOPGN             HAL_AFE_MPPOPGN_MASK
#define HAL_AFE_MPPOPGN_READABLE    1
#define HAL_AFE_MPPOPGN_WRITABLE    1
#define HAL_AFE_MPPOPGN_SIGNED      0
#define HAL_AFE_MPPOPGN_ADR         0x10000954

#define HAL_AFE_MPPOLPSET_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_MPPOLPSET_LSB         1
#define HAL_AFE_MPPOLPSET_MSB         1
#define HAL_AFE_MPPOLPSET_MASK        0x00000002
#define HAL_AFE_MPPOLPSET_TYPE        halAFE_MPPOLPF_t
#define HAL_AFE_MPPOLPSET             HAL_AFE_MPPOLPSET_MASK
#define HAL_AFE_MPPOLPSET_READABLE    1
#define HAL_AFE_MPPOLPSET_WRITABLE    1
#define HAL_AFE_MPPOLPSET_SIGNED      0
#define HAL_AFE_MPPOLPSET_ADR         0x10000954

#define HAL_AFE_BPMPPOLP_REGISTER    (HAL_AFEREG_MAP->MB15)
#define HAL_AFE_BPMPPOLP_LSB         0
#define HAL_AFE_BPMPPOLP_MSB         0
#define HAL_AFE_BPMPPOLP_MASK        0x00000001
#define HAL_AFE_BPMPPOLP_TYPE        Bool
#define HAL_AFE_BPMPPOLP             HAL_AFE_BPMPPOLP_MASK
#define HAL_AFE_BPMPPOLP_READABLE    1
#define HAL_AFE_BPMPPOLP_WRITABLE    1
#define HAL_AFE_BPMPPOLP_SIGNED      0
#define HAL_AFE_BPMPPOLP_ADR         0x10000954

// Register MB16
#define HAL_AFE_MPPAH_REGISTER    (HAL_AFEREG_MAP->MB16)
#define HAL_AFE_MPPAH_LSB         7
#define HAL_AFE_MPPAH_MSB         7
#define HAL_AFE_MPPAH_MASK        0x00000080
#define HAL_AFE_MPPAH_TYPE        Bool
#define HAL_AFE_MPPAH             HAL_AFE_MPPAH_MASK
#define HAL_AFE_MPPAH_READABLE    1
#define HAL_AFE_MPPAH_WRITABLE    1
#define HAL_AFE_MPPAH_SIGNED      0
#define HAL_AFE_MPPAH_ADR         0x10000955

#define HAL_AFE_MPPBG_REGISTER    (HAL_AFEREG_MAP->MB16)
#define HAL_AFE_MPPBG_LSB         6
#define HAL_AFE_MPPBG_MSB         6
#define HAL_AFE_MPPBG_MASK        0x00000040
#define HAL_AFE_MPPBG_TYPE        Bool
#define HAL_AFE_MPPBG             HAL_AFE_MPPBG_MASK
#define HAL_AFE_MPPBG_READABLE    1
#define HAL_AFE_MPPBG_WRITABLE    1
#define HAL_AFE_MPPBG_SIGNED      0
#define HAL_AFE_MPPBG_ADR         0x10000955

#define HAL_AFE_MPPOBG_REGISTER    (HAL_AFEREG_MAP->MB16)
#define HAL_AFE_MPPOBG_LSB         0
#define HAL_AFE_MPPOBG_MSB         4
#define HAL_AFE_MPPOBG_MASK        0x0000001F
#define HAL_AFE_MPPOBG_TYPE        UByte
#define HAL_AFE_MPPOBG             HAL_AFE_MPPOBG_MASK
#define HAL_AFE_MPPOBG_READABLE    1
#define HAL_AFE_MPPOBG_WRITABLE    1
#define HAL_AFE_MPPOBG_SIGNED      0
#define HAL_AFE_MPPOBG_ADR         0x10000955

// Register MB17
#define HAL_AFE_FEOA_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOA_LSB         7
#define HAL_AFE_FEOA_MSB         7
#define HAL_AFE_FEOA_MASK        0x00000080
#define HAL_AFE_FEOA_TYPE        Bool
#define HAL_AFE_FEOA             HAL_AFE_FEOA_MASK
#define HAL_AFE_FEOA_READABLE    1
#define HAL_AFE_FEOA_WRITABLE    1
#define HAL_AFE_FEOA_SIGNED      0
#define HAL_AFE_FEOA_ADR         0x10000956

#define HAL_AFE_FEOB_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOB_LSB         6
#define HAL_AFE_FEOB_MSB         6
#define HAL_AFE_FEOB_MASK        0x00000040
#define HAL_AFE_FEOB_TYPE        Bool
#define HAL_AFE_FEOB             HAL_AFE_FEOB_MASK
#define HAL_AFE_FEOB_READABLE    1
#define HAL_AFE_FEOB_WRITABLE    1
#define HAL_AFE_FEOB_SIGNED      0
#define HAL_AFE_FEOB_ADR         0x10000956

#define HAL_AFE_FEOC_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOC_LSB         5
#define HAL_AFE_FEOC_MSB         5
#define HAL_AFE_FEOC_MASK        0x00000020
#define HAL_AFE_FEOC_TYPE        Bool
#define HAL_AFE_FEOC             HAL_AFE_FEOC_MASK
#define HAL_AFE_FEOC_READABLE    1
#define HAL_AFE_FEOC_WRITABLE    1
#define HAL_AFE_FEOC_SIGNED      0
#define HAL_AFE_FEOC_ADR         0x10000956

#define HAL_AFE_FEOD_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOD_LSB         4
#define HAL_AFE_FEOD_MSB         4
#define HAL_AFE_FEOD_MASK        0x00000010
#define HAL_AFE_FEOD_TYPE        Bool
#define HAL_AFE_FEOD             HAL_AFE_FEOD_MASK
#define HAL_AFE_FEOD_READABLE    1
#define HAL_AFE_FEOD_WRITABLE    1
#define HAL_AFE_FEOD_SIGNED      0
#define HAL_AFE_FEOD_ADR         0x10000956

#define HAL_AFE_FEOAE_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOAE_LSB         3
#define HAL_AFE_FEOAE_MSB         3
#define HAL_AFE_FEOAE_MASK        0x00000008
#define HAL_AFE_FEOAE_TYPE        Bool
#define HAL_AFE_FEOAE             HAL_AFE_FEOAE_MASK
#define HAL_AFE_FEOAE_READABLE    1
#define HAL_AFE_FEOAE_WRITABLE    1
#define HAL_AFE_FEOAE_SIGNED      0
#define HAL_AFE_FEOAE_ADR         0x10000956

#define HAL_AFE_FEOCH_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOCH_LSB         2
#define HAL_AFE_FEOCH_MSB         2
#define HAL_AFE_FEOCH_MASK        0x00000004
#define HAL_AFE_FEOCH_TYPE        Bool
#define HAL_AFE_FEOCH             HAL_AFE_FEOCH_MASK
#define HAL_AFE_FEOCH_READABLE    1
#define HAL_AFE_FEOCH_WRITABLE    1
#define HAL_AFE_FEOCH_SIGNED      0
#define HAL_AFE_FEOCH_ADR         0x10000956

#define HAL_AFE_FEOBI_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEOBI_LSB         1
#define HAL_AFE_FEOBI_MSB         1
#define HAL_AFE_FEOBI_MASK        0x00000002
#define HAL_AFE_FEOBI_TYPE        Bool
#define HAL_AFE_FEOBI             HAL_AFE_FEOBI_MASK
#define HAL_AFE_FEOBI_READABLE    1
#define HAL_AFE_FEOBI_WRITABLE    1
#define HAL_AFE_FEOBI_SIGNED      0
#define HAL_AFE_FEOBI_ADR         0x10000956

#define HAL_AFE_FEODJ_REGISTER    (HAL_AFEREG_MAP->MB17)
#define HAL_AFE_FEODJ_LSB         0
#define HAL_AFE_FEODJ_MSB         0
#define HAL_AFE_FEODJ_MASK        0x00000001
#define HAL_AFE_FEODJ_TYPE        Bool
#define HAL_AFE_FEODJ             HAL_AFE_FEODJ_MASK
#define HAL_AFE_FEODJ_READABLE    1
#define HAL_AFE_FEODJ_WRITABLE    1
#define HAL_AFE_FEODJ_SIGNED      0
#define HAL_AFE_FEODJ_ADR         0x10000956

// Register MB18
#define HAL_AFE_FEOPGN_REGISTER    (HAL_AFEREG_MAP->MB18)
#define HAL_AFE_FEOPGN_LSB         2
#define HAL_AFE_FEOPGN_MSB         3
#define HAL_AFE_FEOPGN_MASK        0x0000000C
#define HAL_AFE_FEOPGN_TYPE        halAFE_FEOPGN_t
#define HAL_AFE_FEOPGN             HAL_AFE_FEOPGN_MASK
#define HAL_AFE_FEOPGN_READABLE    1
#define HAL_AFE_FEOPGN_WRITABLE    1
#define HAL_AFE_FEOPGN_SIGNED      0
#define HAL_AFE_FEOPGN_ADR         0x10000957

#define HAL_AFE_FEOLPSET_REGISTER    (HAL_AFEREG_MAP->MB18)
#define HAL_AFE_FEOLPSET_LSB         1
#define HAL_AFE_FEOLPSET_MSB         1
#define HAL_AFE_FEOLPSET_MASK        0x00000002
#define HAL_AFE_FEOLPSET_TYPE        halAFE_FEOLPF_t
#define HAL_AFE_FEOLPSET             HAL_AFE_FEOLPSET_MASK
#define HAL_AFE_FEOLPSET_READABLE    1
#define HAL_AFE_FEOLPSET_WRITABLE    1
#define HAL_AFE_FEOLPSET_SIGNED      0
#define HAL_AFE_FEOLPSET_ADR         0x10000957

#define HAL_AFE_BPFEOLP_REGISTER    (HAL_AFEREG_MAP->MB18)
#define HAL_AFE_BPFEOLP_LSB         0
#define HAL_AFE_BPFEOLP_MSB         0
#define HAL_AFE_BPFEOLP_MASK        0x00000001
#define HAL_AFE_BPFEOLP_TYPE        Bool
#define HAL_AFE_BPFEOLP             HAL_AFE_BPFEOLP_MASK
#define HAL_AFE_BPFEOLP_READABLE    1
#define HAL_AFE_BPFEOLP_WRITABLE    1
#define HAL_AFE_BPFEOLP_SIGNED      0
#define HAL_AFE_BPFEOLP_ADR         0x10000957

// Register MB19
#define HAL_AFE_TESTLPFC_REGISTER    (HAL_AFEREG_MAP->MB19)
#define HAL_AFE_TESTLPFC_LSB         6
#define HAL_AFE_TESTLPFC_MSB         7
#define HAL_AFE_TESTLPFC_MASK        0x000000C0
#define HAL_AFE_TESTLPFC_TYPE        halAFE_TestLPF_t
#define HAL_AFE_TESTLPFC             HAL_AFE_TESTLPFC_MASK
#define HAL_AFE_TESTLPFC_READABLE    1
#define HAL_AFE_TESTLPFC_WRITABLE    1
#define HAL_AFE_TESTLPFC_SIGNED      0
#define HAL_AFE_TESTLPFC_ADR         0x10000958

#define HAL_AFE_BPLP_REGISTER    (HAL_AFEREG_MAP->MB19)
#define HAL_AFE_BPLP_LSB         5
#define HAL_AFE_BPLP_MSB         5
#define HAL_AFE_BPLP_MASK        0x00000020
#define HAL_AFE_BPLP_TYPE        Bool
#define HAL_AFE_BPLP             HAL_AFE_BPLP_MASK
#define HAL_AFE_BPLP_READABLE    1
#define HAL_AFE_BPLP_WRITABLE    1
#define HAL_AFE_BPLP_SIGNED      0
#define HAL_AFE_BPLP_ADR         0x10000958

#define HAL_AFE_FEOBG_REGISTER    (HAL_AFEREG_MAP->MB19)
#define HAL_AFE_FEOBG_LSB         0
#define HAL_AFE_FEOBG_MSB         4
#define HAL_AFE_FEOBG_MASK        0x0000001F
#define HAL_AFE_FEOBG_TYPE        UByte
#define HAL_AFE_FEOBG             HAL_AFE_FEOBG_MASK
#define HAL_AFE_FEOBG_READABLE    1
#define HAL_AFE_FEOBG_WRITABLE    1
#define HAL_AFE_FEOBG_SIGNED      0
#define HAL_AFE_FEOBG_ADR         0x10000958

// Register MB20
#define HAL_AFE_SRFA_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_SRFA_LSB         7
#define HAL_AFE_SRFA_MSB         7
#define HAL_AFE_SRFA_MASK        0x00000080
#define HAL_AFE_SRFA_TYPE        Bool
#define HAL_AFE_SRFA             HAL_AFE_SRFA_MASK
#define HAL_AFE_SRFA_READABLE    1
#define HAL_AFE_SRFA_WRITABLE    1
#define HAL_AFE_SRFA_SIGNED      0
#define HAL_AFE_SRFA_ADR         0x10000959

#define HAL_AFE_SRFB_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_SRFB_LSB         6
#define HAL_AFE_SRFB_MSB         6
#define HAL_AFE_SRFB_MASK        0x00000040
#define HAL_AFE_SRFB_TYPE        Bool
#define HAL_AFE_SRFB             HAL_AFE_SRFB_MASK
#define HAL_AFE_SRFB_READABLE    1
#define HAL_AFE_SRFB_WRITABLE    1
#define HAL_AFE_SRFB_SIGNED      0
#define HAL_AFE_SRFB_ADR         0x10000959

#define HAL_AFE_SRFC_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_SRFC_LSB         5
#define HAL_AFE_SRFC_MSB         5
#define HAL_AFE_SRFC_MASK        0x00000020
#define HAL_AFE_SRFC_TYPE        Bool
#define HAL_AFE_SRFC             HAL_AFE_SRFC_MASK
#define HAL_AFE_SRFC_READABLE    1
#define HAL_AFE_SRFC_WRITABLE    1
#define HAL_AFE_SRFC_SIGNED      0
#define HAL_AFE_SRFC_ADR         0x10000959

#define HAL_AFE_SRFD_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_SRFD_LSB         4
#define HAL_AFE_SRFD_MSB         4
#define HAL_AFE_SRFD_MASK        0x00000010
#define HAL_AFE_SRFD_TYPE        Bool
#define HAL_AFE_SRFD             HAL_AFE_SRFD_MASK
#define HAL_AFE_SRFD_READABLE    1
#define HAL_AFE_SRFD_WRITABLE    1
#define HAL_AFE_SRFD_SIGNED      0
#define HAL_AFE_SRFD_ADR         0x10000959

#define HAL_AFE_SRFOLPSET_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_SRFOLPSET_LSB         1
#define HAL_AFE_SRFOLPSET_MSB         1
#define HAL_AFE_SRFOLPSET_MASK        0x00000002
#define HAL_AFE_SRFOLPSET_TYPE        halAFE_SRFOLPF_t
#define HAL_AFE_SRFOLPSET             HAL_AFE_SRFOLPSET_MASK
#define HAL_AFE_SRFOLPSET_READABLE    1
#define HAL_AFE_SRFOLPSET_WRITABLE    1
#define HAL_AFE_SRFOLPSET_SIGNED      0
#define HAL_AFE_SRFOLPSET_ADR         0x10000959

#define HAL_AFE_BPSRFOLP_REGISTER    (HAL_AFEREG_MAP->MB20)
#define HAL_AFE_BPSRFOLP_LSB         0
#define HAL_AFE_BPSRFOLP_MSB         0
#define HAL_AFE_BPSRFOLP_MASK        0x00000001
#define HAL_AFE_BPSRFOLP_TYPE        Bool
#define HAL_AFE_BPSRFOLP             HAL_AFE_BPSRFOLP_MASK
#define HAL_AFE_BPSRFOLP_READABLE    1
#define HAL_AFE_BPSRFOLP_WRITABLE    1
#define HAL_AFE_BPSRFOLP_SIGNED      0
#define HAL_AFE_BPSRFOLP_ADR         0x10000959

// Register SB00
#define HAL_AFE_SPP1OSA_REGISTER    (HAL_AFEREG_MAP->SB00)
#define HAL_AFE_SPP1OSA_LSB         0
#define HAL_AFE_SPP1OSA_MSB         7
#define HAL_AFE_SPP1OSA_MASK        0x000000FF
#define HAL_AFE_SPP1OSA_TYPE        UByte
#define HAL_AFE_SPP1OSA             HAL_AFE_SPP1OSA_MASK
#define HAL_AFE_SPP1OSA_READABLE    1
#define HAL_AFE_SPP1OSA_WRITABLE    1
#define HAL_AFE_SPP1OSA_SIGNED      0
#define HAL_AFE_SPP1OSA_ADR         0x1000095A

// Register SB01
#define HAL_AFE_SPP1OSB_REGISTER    (HAL_AFEREG_MAP->SB01)
#define HAL_AFE_SPP1OSB_LSB         0
#define HAL_AFE_SPP1OSB_MSB         7
#define HAL_AFE_SPP1OSB_MASK        0x000000FF
#define HAL_AFE_SPP1OSB_TYPE        UByte
#define HAL_AFE_SPP1OSB             HAL_AFE_SPP1OSB_MASK
#define HAL_AFE_SPP1OSB_READABLE    1
#define HAL_AFE_SPP1OSB_WRITABLE    1
#define HAL_AFE_SPP1OSB_SIGNED      0
#define HAL_AFE_SPP1OSB_ADR         0x1000095B

// Register SB02
#define HAL_AFE_SPP1PGNB_REGISTER    (HAL_AFEREG_MAP->SB02)
#define HAL_AFE_SPP1PGNB_LSB         2
#define HAL_AFE_SPP1PGNB_MSB         3
#define HAL_AFE_SPP1PGNB_MASK        0x0000000C
#define HAL_AFE_SPP1PGNB_TYPE        halAFE_SPPO1WrGN_t
#define HAL_AFE_SPP1PGNB             HAL_AFE_SPP1PGNB_MASK
#define HAL_AFE_SPP1PGNB_READABLE    1
#define HAL_AFE_SPP1PGNB_WRITABLE    1
#define HAL_AFE_SPP1PGNB_SIGNED      0
#define HAL_AFE_SPP1PGNB_ADR         0x1000095C

#define HAL_AFE_SPP1PGNA_REGISTER    (HAL_AFEREG_MAP->SB02)
#define HAL_AFE_SPP1PGNA_LSB         0
#define HAL_AFE_SPP1PGNA_MSB         1
#define HAL_AFE_SPP1PGNA_MASK        0x00000003
#define HAL_AFE_SPP1PGNA_TYPE        halAFE_SPPO1RdGN_t
#define HAL_AFE_SPP1PGNA             HAL_AFE_SPP1PGNA_MASK
#define HAL_AFE_SPP1PGNA_READABLE    1
#define HAL_AFE_SPP1PGNA_WRITABLE    1
#define HAL_AFE_SPP1PGNA_SIGNED      0
#define HAL_AFE_SPP1PGNA_ADR         0x1000095C

// Register SB03
#define HAL_AFE_SPP2OSA_REGISTER    (HAL_AFEREG_MAP->SB03)
#define HAL_AFE_SPP2OSA_LSB         0
#define HAL_AFE_SPP2OSA_MSB         7
#define HAL_AFE_SPP2OSA_MASK        0x000000FF
#define HAL_AFE_SPP2OSA_TYPE        UByte
#define HAL_AFE_SPP2OSA             HAL_AFE_SPP2OSA_MASK
#define HAL_AFE_SPP2OSA_READABLE    1
#define HAL_AFE_SPP2OSA_WRITABLE    1
#define HAL_AFE_SPP2OSA_SIGNED      0
#define HAL_AFE_SPP2OSA_ADR         0x1000095D

// Register SB04
#define HAL_AFE_SPP2OSB_REGISTER    (HAL_AFEREG_MAP->SB04)
#define HAL_AFE_SPP2OSB_LSB         0
#define HAL_AFE_SPP2OSB_MSB         7
#define HAL_AFE_SPP2OSB_MASK        0x000000FF
#define HAL_AFE_SPP2OSB_TYPE        UByte
#define HAL_AFE_SPP2OSB             HAL_AFE_SPP2OSB_MASK
#define HAL_AFE_SPP2OSB_READABLE    1
#define HAL_AFE_SPP2OSB_WRITABLE    1
#define HAL_AFE_SPP2OSB_SIGNED      0
#define HAL_AFE_SPP2OSB_ADR         0x1000095E

// Register SB05
#define HAL_AFE_SPP2PGNB_REGISTER    (HAL_AFEREG_MAP->SB05)
#define HAL_AFE_SPP2PGNB_LSB         2
#define HAL_AFE_SPP2PGNB_MSB         3
#define HAL_AFE_SPP2PGNB_MASK        0x0000000C
#define HAL_AFE_SPP2PGNB_TYPE        halAFE_SPPO2WrGN_t
#define HAL_AFE_SPP2PGNB             HAL_AFE_SPP2PGNB_MASK
#define HAL_AFE_SPP2PGNB_READABLE    1
#define HAL_AFE_SPP2PGNB_WRITABLE    1
#define HAL_AFE_SPP2PGNB_SIGNED      0
#define HAL_AFE_SPP2PGNB_ADR         0x1000095F

#define HAL_AFE_SPP2PGNA_REGISTER    (HAL_AFEREG_MAP->SB05)
#define HAL_AFE_SPP2PGNA_LSB         0
#define HAL_AFE_SPP2PGNA_MSB         1
#define HAL_AFE_SPP2PGNA_MASK        0x00000003
#define HAL_AFE_SPP2PGNA_TYPE        halAFE_SPPO2RdGN_t
#define HAL_AFE_SPP2PGNA             HAL_AFE_SPP2PGNA_MASK
#define HAL_AFE_SPP2PGNA_READABLE    1
#define HAL_AFE_SPP2PGNA_WRITABLE    1
#define HAL_AFE_SPP2PGNA_SIGNED      0
#define HAL_AFE_SPP2PGNA_ADR         0x1000095F

// Register SB06
#define HAL_AFE_SBADOSA_REGISTER    (HAL_AFEREG_MAP->SB06)
#define HAL_AFE_SBADOSA_LSB         0
#define HAL_AFE_SBADOSA_MSB         7
#define HAL_AFE_SBADOSA_MASK        0x000000FF
#define HAL_AFE_SBADOSA_TYPE        UByte
#define HAL_AFE_SBADOSA             HAL_AFE_SBADOSA_MASK
#define HAL_AFE_SBADOSA_READABLE    1
#define HAL_AFE_SBADOSA_WRITABLE    1
#define HAL_AFE_SBADOSA_SIGNED      0
#define HAL_AFE_SBADOSA_ADR         0x10000960

// Register SB07
#define HAL_AFE_SBADOSB_REGISTER    (HAL_AFEREG_MAP->SB07)
#define HAL_AFE_SBADOSB_LSB         0
#define HAL_AFE_SBADOSB_MSB         7
#define HAL_AFE_SBADOSB_MASK        0x000000FF
#define HAL_AFE_SBADOSB_TYPE        UByte
#define HAL_AFE_SBADOSB             HAL_AFE_SBADOSB_MASK
#define HAL_AFE_SBADOSB_READABLE    1
#define HAL_AFE_SBADOSB_WRITABLE    1
#define HAL_AFE_SBADOSB_SIGNED      0
#define HAL_AFE_SBADOSB_ADR         0x10000961

// Register SB08
#define HAL_AFE_SBADGNA_REGISTER    (HAL_AFEREG_MAP->SB08)
#define HAL_AFE_SBADGNA_LSB         0
#define HAL_AFE_SBADGNA_MSB         4
#define HAL_AFE_SBADGNA_MASK        0x0000001F
#define HAL_AFE_SBADGNA_TYPE        UByte
#define HAL_AFE_SBADGNA             HAL_AFE_SBADGNA_MASK
#define HAL_AFE_SBADGNA_READABLE    1
#define HAL_AFE_SBADGNA_WRITABLE    1
#define HAL_AFE_SBADGNA_SIGNED      0
#define HAL_AFE_SBADGNA_ADR         0x10000962

// Register SB09
#define HAL_AFE_SBADGNB_REGISTER    (HAL_AFEREG_MAP->SB09)
#define HAL_AFE_SBADGNB_LSB         0
#define HAL_AFE_SBADGNB_MSB         4
#define HAL_AFE_SBADGNB_MASK        0x0000001F
#define HAL_AFE_SBADGNB_TYPE        UByte
#define HAL_AFE_SBADGNB             HAL_AFE_SBADGNB_MASK
#define HAL_AFE_SBADGNB_READABLE    1
#define HAL_AFE_SBADGNB_WRITABLE    1
#define HAL_AFE_SBADGNB_SIGNED      0
#define HAL_AFE_SBADGNB_ADR         0x10000963

// Register SB10
#define HAL_AFE_SBGUPSEL_REGISTER    (HAL_AFEREG_MAP->SB10)
#define HAL_AFE_SBGUPSEL_LSB         0
#define HAL_AFE_SBGUPSEL_MSB         4
#define HAL_AFE_SBGUPSEL_MASK        0x0000001F
#define HAL_AFE_SBGUPSEL_TYPE        UByte
#define HAL_AFE_SBGUPSEL             HAL_AFE_SBGUPSEL_MASK
#define HAL_AFE_SBGUPSEL_READABLE    1
#define HAL_AFE_SBGUPSEL_WRITABLE    1
#define HAL_AFE_SBGUPSEL_SIGNED      0
#define HAL_AFE_SBGUPSEL_ADR         0x10000964

// Register SB11
#define HAL_AFE_SBATTSEL_REGISTER    (HAL_AFEREG_MAP->SB11)
#define HAL_AFE_SBATTSEL_LSB         0
#define HAL_AFE_SBATTSEL_MSB         4
#define HAL_AFE_SBATTSEL_MASK        0x0000001F
#define HAL_AFE_SBATTSEL_TYPE        UByte
#define HAL_AFE_SBATTSEL             HAL_AFE_SBATTSEL_MASK
#define HAL_AFE_SBATTSEL_READABLE    1
#define HAL_AFE_SBATTSEL_WRITABLE    1
#define HAL_AFE_SBATTSEL_SIGNED      0
#define HAL_AFE_SBATTSEL_ADR         0x10000965

// Register SB12
#define HAL_AFE_SBVREFOSA_REGISTER    (HAL_AFEREG_MAP->SB12)
#define HAL_AFE_SBVREFOSA_LSB         0
#define HAL_AFE_SBVREFOSA_MSB         7
#define HAL_AFE_SBVREFOSA_MASK        0x000000FF
#define HAL_AFE_SBVREFOSA_TYPE        UByte
#define HAL_AFE_SBVREFOSA             HAL_AFE_SBVREFOSA_MASK
#define HAL_AFE_SBVREFOSA_READABLE    1
#define HAL_AFE_SBVREFOSA_WRITABLE    1
#define HAL_AFE_SBVREFOSA_SIGNED      0
#define HAL_AFE_SBVREFOSA_ADR         0x10000966

// Register SB13
#define HAL_AFE_SBVREFOSB_REGISTER    (HAL_AFEREG_MAP->SB13)
#define HAL_AFE_SBVREFOSB_LSB         0
#define HAL_AFE_SBVREFOSB_MSB         7
#define HAL_AFE_SBVREFOSB_MASK        0x000000FF
#define HAL_AFE_SBVREFOSB_TYPE        UByte
#define HAL_AFE_SBVREFOSB             HAL_AFE_SBVREFOSB_MASK
#define HAL_AFE_SBVREFOSB_READABLE    1
#define HAL_AFE_SBVREFOSB_WRITABLE    1
#define HAL_AFE_SBVREFOSB_SIGNED      0
#define HAL_AFE_SBVREFOSB_ADR         0x10000967

// Register SB14
#define HAL_AFE_SPP1E_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1E_LSB         7
#define HAL_AFE_SPP1E_MSB         7
#define HAL_AFE_SPP1E_MASK        0x00000080
#define HAL_AFE_SPP1E_TYPE        Bool
#define HAL_AFE_SPP1E             HAL_AFE_SPP1E_MASK
#define HAL_AFE_SPP1E_READABLE    1
#define HAL_AFE_SPP1E_WRITABLE    1
#define HAL_AFE_SPP1E_SIGNED      0
#define HAL_AFE_SPP1E_ADR         0x10000968

#define HAL_AFE_SPP1F_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1F_LSB         6
#define HAL_AFE_SPP1F_MSB         6
#define HAL_AFE_SPP1F_MASK        0x00000040
#define HAL_AFE_SPP1F_TYPE        Bool
#define HAL_AFE_SPP1F             HAL_AFE_SPP1F_MASK
#define HAL_AFE_SPP1F_READABLE    1
#define HAL_AFE_SPP1F_WRITABLE    1
#define HAL_AFE_SPP1F_SIGNED      0
#define HAL_AFE_SPP1F_ADR         0x10000968

#define HAL_AFE_SPP1G_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1G_LSB         5
#define HAL_AFE_SPP1G_MSB         5
#define HAL_AFE_SPP1G_MASK        0x00000020
#define HAL_AFE_SPP1G_TYPE        Bool
#define HAL_AFE_SPP1G             HAL_AFE_SPP1G_MASK
#define HAL_AFE_SPP1G_READABLE    1
#define HAL_AFE_SPP1G_WRITABLE    1
#define HAL_AFE_SPP1G_SIGNED      0
#define HAL_AFE_SPP1G_ADR         0x10000968

#define HAL_AFE_SPP1H_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1H_LSB         4
#define HAL_AFE_SPP1H_MSB         4
#define HAL_AFE_SPP1H_MASK        0x00000010
#define HAL_AFE_SPP1H_TYPE        Bool
#define HAL_AFE_SPP1H             HAL_AFE_SPP1H_MASK
#define HAL_AFE_SPP1H_READABLE    1
#define HAL_AFE_SPP1H_WRITABLE    1
#define HAL_AFE_SPP1H_SIGNED      0
#define HAL_AFE_SPP1H_ADR         0x10000968

#define HAL_AFE_SPP1EC_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1EC_LSB         3
#define HAL_AFE_SPP1EC_MSB         3
#define HAL_AFE_SPP1EC_MASK        0x00000008
#define HAL_AFE_SPP1EC_TYPE        Bool
#define HAL_AFE_SPP1EC             HAL_AFE_SPP1EC_MASK
#define HAL_AFE_SPP1EC_READABLE    1
#define HAL_AFE_SPP1EC_WRITABLE    1
#define HAL_AFE_SPP1EC_SIGNED      0
#define HAL_AFE_SPP1EC_ADR         0x10000968

#define HAL_AFE_SPP1EI_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1EI_LSB         2
#define HAL_AFE_SPP1EI_MSB         2
#define HAL_AFE_SPP1EI_MASK        0x00000004
#define HAL_AFE_SPP1EI_TYPE        Bool
#define HAL_AFE_SPP1EI             HAL_AFE_SPP1EI_MASK
#define HAL_AFE_SPP1EI_READABLE    1
#define HAL_AFE_SPP1EI_WRITABLE    1
#define HAL_AFE_SPP1EI_SIGNED      0
#define HAL_AFE_SPP1EI_ADR         0x10000968

#define HAL_AFE_SPP1HD_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1HD_LSB         1
#define HAL_AFE_SPP1HD_MSB         1
#define HAL_AFE_SPP1HD_MASK        0x00000002
#define HAL_AFE_SPP1HD_TYPE        Bool
#define HAL_AFE_SPP1HD             HAL_AFE_SPP1HD_MASK
#define HAL_AFE_SPP1HD_READABLE    1
#define HAL_AFE_SPP1HD_WRITABLE    1
#define HAL_AFE_SPP1HD_SIGNED      0
#define HAL_AFE_SPP1HD_ADR         0x10000968

#define HAL_AFE_SPP1FC_REGISTER    (HAL_AFEREG_MAP->SB14)
#define HAL_AFE_SPP1FC_LSB         0
#define HAL_AFE_SPP1FC_MSB         0
#define HAL_AFE_SPP1FC_MASK        0x00000001
#define HAL_AFE_SPP1FC_TYPE        Bool
#define HAL_AFE_SPP1FC             HAL_AFE_SPP1FC_MASK
#define HAL_AFE_SPP1FC_READABLE    1
#define HAL_AFE_SPP1FC_WRITABLE    1
#define HAL_AFE_SPP1FC_SIGNED      0
#define HAL_AFE_SPP1FC_ADR         0x10000968

// Register SB15
#define HAL_AFE_SPP1FJ_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPP1FJ_LSB         7
#define HAL_AFE_SPP1FJ_MSB         7
#define HAL_AFE_SPP1FJ_MASK        0x00000080
#define HAL_AFE_SPP1FJ_TYPE        Bool
#define HAL_AFE_SPP1FJ             HAL_AFE_SPP1FJ_MASK
#define HAL_AFE_SPP1FJ_READABLE    1
#define HAL_AFE_SPP1FJ_WRITABLE    1
#define HAL_AFE_SPP1FJ_SIGNED      0
#define HAL_AFE_SPP1FJ_ADR         0x10000969

#define HAL_AFE_SPP1GD_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPP1GD_LSB         6
#define HAL_AFE_SPP1GD_MSB         6
#define HAL_AFE_SPP1GD_MASK        0x00000040
#define HAL_AFE_SPP1GD_TYPE        Bool
#define HAL_AFE_SPP1GD             HAL_AFE_SPP1GD_MASK
#define HAL_AFE_SPP1GD_READABLE    1
#define HAL_AFE_SPP1GD_WRITABLE    1
#define HAL_AFE_SPP1GD_SIGNED      0
#define HAL_AFE_SPP1GD_ADR         0x10000969

#define HAL_AFE_SPP1FE_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPP1FE_LSB         5
#define HAL_AFE_SPP1FE_MSB         5
#define HAL_AFE_SPP1FE_MASK        0x00000020
#define HAL_AFE_SPP1FE_TYPE        Bool
#define HAL_AFE_SPP1FE             HAL_AFE_SPP1FE_MASK
#define HAL_AFE_SPP1FE_READABLE    1
#define HAL_AFE_SPP1FE_WRITABLE    1
#define HAL_AFE_SPP1FE_SIGNED      0
#define HAL_AFE_SPP1FE_ADR         0x10000969

#define HAL_AFE_SPP1EH_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPP1EH_LSB         4
#define HAL_AFE_SPP1EH_MSB         4
#define HAL_AFE_SPP1EH_MASK        0x00000010
#define HAL_AFE_SPP1EH_TYPE        Bool
#define HAL_AFE_SPP1EH             HAL_AFE_SPP1EH_MASK
#define HAL_AFE_SPP1EH_READABLE    1
#define HAL_AFE_SPP1EH_WRITABLE    1
#define HAL_AFE_SPP1EH_SIGNED      0
#define HAL_AFE_SPP1EH_ADR         0x10000969

#define HAL_AFE_SPDSHON_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPDSHON_LSB         1
#define HAL_AFE_SPDSHON_MSB         1
#define HAL_AFE_SPDSHON_MASK        0x00000002
#define HAL_AFE_SPDSHON_TYPE        Bool
#define HAL_AFE_SPDSHON             HAL_AFE_SPDSHON_MASK
#define HAL_AFE_SPDSHON_READABLE    1
#define HAL_AFE_SPDSHON_WRITABLE    1
#define HAL_AFE_SPDSHON_SIGNED      0
#define HAL_AFE_SPDSHON_ADR         0x10000969

#define HAL_AFE_SPDSHLP_REGISTER    (HAL_AFEREG_MAP->SB15)
#define HAL_AFE_SPDSHLP_LSB         0
#define HAL_AFE_SPDSHLP_MSB         0
#define HAL_AFE_SPDSHLP_MASK        0x00000001
#define HAL_AFE_SPDSHLP_TYPE        Bool
#define HAL_AFE_SPDSHLP             HAL_AFE_SPDSHLP_MASK
#define HAL_AFE_SPDSHLP_READABLE    1
#define HAL_AFE_SPDSHLP_WRITABLE    1
#define HAL_AFE_SPDSHLP_SIGNED      0
#define HAL_AFE_SPDSHLP_ADR         0x10000969

// Register SB16
#define HAL_AFE_SPPO1LPST_REGISTER    (HAL_AFEREG_MAP->SB16)
#define HAL_AFE_SPPO1LPST_LSB         7
#define HAL_AFE_SPPO1LPST_MSB         7
#define HAL_AFE_SPPO1LPST_MASK        0x00000080
#define HAL_AFE_SPPO1LPST_TYPE        halAFE_SPPO1LPF_t
#define HAL_AFE_SPPO1LPST             HAL_AFE_SPPO1LPST_MASK
#define HAL_AFE_SPPO1LPST_READABLE    1
#define HAL_AFE_SPPO1LPST_WRITABLE    1
#define HAL_AFE_SPPO1LPST_SIGNED      0
#define HAL_AFE_SPPO1LPST_ADR         0x1000096A

#define HAL_AFE_BPSPPO1LP_REGISTER    (HAL_AFEREG_MAP->SB16)
#define HAL_AFE_BPSPPO1LP_LSB         6
#define HAL_AFE_BPSPPO1LP_MSB         6
#define HAL_AFE_BPSPPO1LP_MASK        0x00000040
#define HAL_AFE_BPSPPO1LP_TYPE        Bool
#define HAL_AFE_BPSPPO1LP             HAL_AFE_BPSPPO1LP_MASK
#define HAL_AFE_BPSPPO1LP_READABLE    1
#define HAL_AFE_BPSPPO1LP_WRITABLE    1
#define HAL_AFE_BPSPPO1LP_SIGNED      0
#define HAL_AFE_BPSPPO1LP_ADR         0x1000096A

#define HAL_AFE_SPP1BG_REGISTER    (HAL_AFEREG_MAP->SB16)
#define HAL_AFE_SPP1BG_LSB         0
#define HAL_AFE_SPP1BG_MSB         4
#define HAL_AFE_SPP1BG_MASK        0x0000001F
#define HAL_AFE_SPP1BG_TYPE        UByte
#define HAL_AFE_SPP1BG             HAL_AFE_SPP1BG_MASK
#define HAL_AFE_SPP1BG_READABLE    1
#define HAL_AFE_SPP1BG_WRITABLE    1
#define HAL_AFE_SPP1BG_SIGNED      0
#define HAL_AFE_SPP1BG_ADR         0x1000096A

// Register SB17
#define HAL_AFE_SPP2E_REGISTER    (HAL_AFEREG_MAP->SB17)
#define HAL_AFE_SPP2E_LSB         7
#define HAL_AFE_SPP2E_MSB         7
#define HAL_AFE_SPP2E_MASK        0x00000080
#define HAL_AFE_SPP2E_TYPE        Bool
#define HAL_AFE_SPP2E             HAL_AFE_SPP2E_MASK
#define HAL_AFE_SPP2E_READABLE    1
#define HAL_AFE_SPP2E_WRITABLE    1
#define HAL_AFE_SPP2E_SIGNED      0
#define HAL_AFE_SPP2E_ADR         0x1000096B

#define HAL_AFE_SPP2F_REGISTER    (HAL_AFEREG_MAP->SB17)
#define HAL_AFE_SPP2F_LSB         6
#define HAL_AFE_SPP2F_MSB         6
#define HAL_AFE_SPP2F_MASK        0x00000040
#define HAL_AFE_SPP2F_TYPE        Bool
#define HAL_AFE_SPP2F             HAL_AFE_SPP2F_MASK
#define HAL_AFE_SPP2F_READABLE    1
#define HAL_AFE_SPP2F_WRITABLE    1
#define HAL_AFE_SPP2F_SIGNED      0
#define HAL_AFE_SPP2F_ADR         0x1000096B

#define HAL_AFE_SPP2G_REGISTER    (HAL_AFEREG_MAP->SB17)
#define HAL_AFE_SPP2G_LSB         5
#define HAL_AFE_SPP2G_MSB         5
#define HAL_AFE_SPP2G_MASK        0x00000020
#define HAL_AFE_SPP2G_TYPE        Bool
#define HAL_AFE_SPP2G             HAL_AFE_SPP2G_MASK
#define HAL_AFE_SPP2G_READABLE    1
#define HAL_AFE_SPP2G_WRITABLE    1
#define HAL_AFE_SPP2G_SIGNED      0
#define HAL_AFE_SPP2G_ADR         0x1000096B

#define HAL_AFE_SPP2H_REGISTER    (HAL_AFEREG_MAP->SB17)
#define HAL_AFE_SPP2H_LSB         4
#define HAL_AFE_SPP2H_MSB         4
#define HAL_AFE_SPP2H_MASK        0x00000010
#define HAL_AFE_SPP2H_TYPE        Bool
#define HAL_AFE_SPP2H             HAL_AFE_SPP2H_MASK
#define HAL_AFE_SPP2H_READABLE    1
#define HAL_AFE_SPP2H_WRITABLE    1
#define HAL_AFE_SPP2H_SIGNED      0
#define HAL_AFE_SPP2H_ADR         0x1000096B

// Register SB18
#define HAL_AFE_SPPO2LPST_REGISTER    (HAL_AFEREG_MAP->SB18)
#define HAL_AFE_SPPO2LPST_LSB         7
#define HAL_AFE_SPPO2LPST_MSB         7
#define HAL_AFE_SPPO2LPST_MASK        0x00000080
#define HAL_AFE_SPPO2LPST_TYPE        halAFE_SPPO2LPF_t
#define HAL_AFE_SPPO2LPST             HAL_AFE_SPPO2LPST_MASK
#define HAL_AFE_SPPO2LPST_READABLE    1
#define HAL_AFE_SPPO2LPST_WRITABLE    1
#define HAL_AFE_SPPO2LPST_SIGNED      0
#define HAL_AFE_SPPO2LPST_ADR         0x1000096C

#define HAL_AFE_BPSPPO2LP_REGISTER    (HAL_AFEREG_MAP->SB18)
#define HAL_AFE_BPSPPO2LP_LSB         6
#define HAL_AFE_BPSPPO2LP_MSB         6
#define HAL_AFE_BPSPPO2LP_MASK        0x00000040
#define HAL_AFE_BPSPPO2LP_TYPE        Bool
#define HAL_AFE_BPSPPO2LP             HAL_AFE_BPSPPO2LP_MASK
#define HAL_AFE_BPSPPO2LP_READABLE    1
#define HAL_AFE_BPSPPO2LP_WRITABLE    1
#define HAL_AFE_BPSPPO2LP_SIGNED      0
#define HAL_AFE_BPSPPO2LP_ADR         0x1000096C

#define HAL_AFE_SPP2BG_REGISTER    (HAL_AFEREG_MAP->SB18)
#define HAL_AFE_SPP2BG_LSB         0
#define HAL_AFE_SPP2BG_MSB         4
#define HAL_AFE_SPP2BG_MASK        0x0000001F
#define HAL_AFE_SPP2BG_TYPE        UByte
#define HAL_AFE_SPP2BG             HAL_AFE_SPP2BG_MASK
#define HAL_AFE_SPP2BG_READABLE    1
#define HAL_AFE_SPP2BG_WRITABLE    1
#define HAL_AFE_SPP2BG_SIGNED      0
#define HAL_AFE_SPP2BG_ADR         0x1000096C

// Register SB19
#define HAL_AFE_SBADE_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_SBADE_LSB         7
#define HAL_AFE_SBADE_MSB         7
#define HAL_AFE_SBADE_MASK        0x00000080
#define HAL_AFE_SBADE_TYPE        Bool
#define HAL_AFE_SBADE             HAL_AFE_SBADE_MASK
#define HAL_AFE_SBADE_READABLE    1
#define HAL_AFE_SBADE_WRITABLE    1
#define HAL_AFE_SBADE_SIGNED      0
#define HAL_AFE_SBADE_ADR         0x1000096D

#define HAL_AFE_SBADF_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_SBADF_LSB         6
#define HAL_AFE_SBADF_MSB         6
#define HAL_AFE_SBADF_MASK        0x00000040
#define HAL_AFE_SBADF_TYPE        Bool
#define HAL_AFE_SBADF             HAL_AFE_SBADF_MASK
#define HAL_AFE_SBADF_READABLE    1
#define HAL_AFE_SBADF_WRITABLE    1
#define HAL_AFE_SBADF_SIGNED      0
#define HAL_AFE_SBADF_ADR         0x1000096D

#define HAL_AFE_SBADG_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_SBADG_LSB         5
#define HAL_AFE_SBADG_MSB         5
#define HAL_AFE_SBADG_MASK        0x00000020
#define HAL_AFE_SBADG_TYPE        Bool
#define HAL_AFE_SBADG             HAL_AFE_SBADG_MASK
#define HAL_AFE_SBADG_READABLE    1
#define HAL_AFE_SBADG_WRITABLE    1
#define HAL_AFE_SBADG_SIGNED      0
#define HAL_AFE_SBADG_ADR         0x1000096D

#define HAL_AFE_SBADH_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_SBADH_LSB         4
#define HAL_AFE_SBADH_MSB         4
#define HAL_AFE_SBADH_MASK        0x00000010
#define HAL_AFE_SBADH_TYPE        Bool
#define HAL_AFE_SBADH             HAL_AFE_SBADH_MASK
#define HAL_AFE_SBADH_READABLE    1
#define HAL_AFE_SBADH_WRITABLE    1
#define HAL_AFE_SBADH_SIGNED      0
#define HAL_AFE_SBADH_ADR         0x1000096D

#define HAL_AFE_SBADLPSET_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_SBADLPSET_LSB         1
#define HAL_AFE_SBADLPSET_MSB         1
#define HAL_AFE_SBADLPSET_MASK        0x00000002
#define HAL_AFE_SBADLPSET_TYPE        halAFE_SBADLPF_t
#define HAL_AFE_SBADLPSET             HAL_AFE_SBADLPSET_MASK
#define HAL_AFE_SBADLPSET_READABLE    1
#define HAL_AFE_SBADLPSET_WRITABLE    1
#define HAL_AFE_SBADLPSET_SIGNED      0
#define HAL_AFE_SBADLPSET_ADR         0x1000096D

#define HAL_AFE_BPSBADLP_REGISTER    (HAL_AFEREG_MAP->SB19)
#define HAL_AFE_BPSBADLP_LSB         0
#define HAL_AFE_BPSBADLP_MSB         0
#define HAL_AFE_BPSBADLP_MASK        0x00000001
#define HAL_AFE_BPSBADLP_TYPE        Bool
#define HAL_AFE_BPSBADLP             HAL_AFE_BPSBADLP_MASK
#define HAL_AFE_BPSBADLP_READABLE    1
#define HAL_AFE_BPSBADLP_WRITABLE    1
#define HAL_AFE_BPSBADLP_SIGNED      0
#define HAL_AFE_BPSBADLP_ADR         0x1000096D

// Register REFS00
#define HAL_AFE_PDSVOALL_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOALL_LSB         7
#define HAL_AFE_PDSVOALL_MSB         7
#define HAL_AFE_PDSVOALL_MASK        0x00000080
#define HAL_AFE_PDSVOALL_TYPE        Bool
#define HAL_AFE_PDSVOALL             HAL_AFE_PDSVOALL_MASK
#define HAL_AFE_PDSVOALL_READABLE    1
#define HAL_AFE_PDSVOALL_WRITABLE    1
#define HAL_AFE_PDSVOALL_SIGNED      0
#define HAL_AFE_PDSVOALL_ADR         0x1000096E

#define HAL_AFE_PDSVOMB_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOMB_LSB         6
#define HAL_AFE_PDSVOMB_MSB         6
#define HAL_AFE_PDSVOMB_MASK        0x00000040
#define HAL_AFE_PDSVOMB_TYPE        Bool
#define HAL_AFE_PDSVOMB             HAL_AFE_PDSVOMB_MASK
#define HAL_AFE_PDSVOMB_READABLE    1
#define HAL_AFE_PDSVOMB_WRITABLE    1
#define HAL_AFE_PDSVOMB_SIGNED      0
#define HAL_AFE_PDSVOMB_ADR         0x1000096E

#define HAL_AFE_PDSVOSB_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOSB_LSB         5
#define HAL_AFE_PDSVOSB_MSB         5
#define HAL_AFE_PDSVOSB_MASK        0x00000020
#define HAL_AFE_PDSVOSB_TYPE        Bool
#define HAL_AFE_PDSVOSB             HAL_AFE_PDSVOSB_MASK
#define HAL_AFE_PDSVOSB_READABLE    1
#define HAL_AFE_PDSVOSB_WRITABLE    1
#define HAL_AFE_PDSVOSB_SIGNED      0
#define HAL_AFE_PDSVOSB_ADR         0x1000096E

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_PDSVODFCT_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVODFCT_LSB         4
#define HAL_AFE_PDSVODFCT_MSB         4
#define HAL_AFE_PDSVODFCT_MASK        0x00000010
#define HAL_AFE_PDSVODFCT_TYPE        Bool
#define HAL_AFE_PDSVODFCT             HAL_AFE_PDSVODFCT_MASK
#define HAL_AFE_PDSVODFCT_READABLE    1
#define HAL_AFE_PDSVODFCT_WRITABLE    1
#define HAL_AFE_PDSVODFCT_SIGNED      0
#define HAL_AFE_PDSVODFCT_ADR         0x1000096E

#endif // #if (CHIP_REV <= 0xB3)

#define HAL_AFE_PDSVOFCT_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOFCT_LSB         4
#define HAL_AFE_PDSVOFCT_MSB         4
#define HAL_AFE_PDSVOFCT_MASK        0x00000010
#define HAL_AFE_PDSVOFCT_TYPE        Bool
#define HAL_AFE_PDSVOFCT             HAL_AFE_PDSVOFCT_MASK
#define HAL_AFE_PDSVOFCT_READABLE    1
#define HAL_AFE_PDSVOFCT_WRITABLE    1
#define HAL_AFE_PDSVOFCT_SIGNED      0
#define HAL_AFE_PDSVOFCT_ADR         0x1000096E

#define HAL_AFE_PDSVOBSRP_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOBSRP_LSB         3
#define HAL_AFE_PDSVOBSRP_MSB         3
#define HAL_AFE_PDSVOBSRP_MASK        0x00000008
#define HAL_AFE_PDSVOBSRP_TYPE        Bool
#define HAL_AFE_PDSVOBSRP             HAL_AFE_PDSVOBSRP_MASK
#define HAL_AFE_PDSVOBSRP_READABLE    1
#define HAL_AFE_PDSVOBSRP_WRITABLE    1
#define HAL_AFE_PDSVOBSRP_SIGNED      0
#define HAL_AFE_PDSVOBSRP_ADR         0x1000096E

#define HAL_AFE_PDSVOMUX_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOMUX_LSB         2
#define HAL_AFE_PDSVOMUX_MSB         2
#define HAL_AFE_PDSVOMUX_MASK        0x00000004
#define HAL_AFE_PDSVOMUX_TYPE        Bool
#define HAL_AFE_PDSVOMUX             HAL_AFE_PDSVOMUX_MASK
#define HAL_AFE_PDSVOMUX_READABLE    1
#define HAL_AFE_PDSVOMUX_WRITABLE    1
#define HAL_AFE_PDSVOMUX_SIGNED      0
#define HAL_AFE_PDSVOMUX_ADR         0x1000096E

#define HAL_AFE_PDSVOREFS_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_PDSVOREFS_LSB         1
#define HAL_AFE_PDSVOREFS_MSB         1
#define HAL_AFE_PDSVOREFS_MASK        0x00000002
#define HAL_AFE_PDSVOREFS_TYPE        Bool
#define HAL_AFE_PDSVOREFS             HAL_AFE_PDSVOREFS_MASK
#define HAL_AFE_PDSVOREFS_READABLE    1
#define HAL_AFE_PDSVOREFS_WRITABLE    1
#define HAL_AFE_PDSVOREFS_SIGNED      0
#define HAL_AFE_PDSVOREFS_ADR         0x1000096E

#define HAL_AFE_RBIASSEL_REGISTER    (HAL_AFEREG_MAP->REFS00)
#define HAL_AFE_RBIASSEL_LSB         0
#define HAL_AFE_RBIASSEL_MSB         0
#define HAL_AFE_RBIASSEL_MASK        0x00000001
#define HAL_AFE_RBIASSEL_TYPE        Bool
#define HAL_AFE_RBIASSEL             HAL_AFE_RBIASSEL_MASK
#define HAL_AFE_RBIASSEL_READABLE    1
#define HAL_AFE_RBIASSEL_WRITABLE    1
#define HAL_AFE_RBIASSEL_SIGNED      0
#define HAL_AFE_RBIASSEL_ADR         0x1000096E

// Register REFS01

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_ENBANDGAP_REGISTER    (HAL_AFEREG_MAP->REFS01)
#define HAL_AFE_ENBANDGAP_LSB         7
#define HAL_AFE_ENBANDGAP_MSB         7
#define HAL_AFE_ENBANDGAP_MASK        0x00000080
#define HAL_AFE_ENBANDGAP_TYPE        Bool
#define HAL_AFE_ENBANDGAP             HAL_AFE_ENBANDGAP_MASK
#define HAL_AFE_ENBANDGAP_READABLE    1
#define HAL_AFE_ENBANDGAP_WRITABLE    1
#define HAL_AFE_ENBANDGAP_SIGNED      0
#define HAL_AFE_ENBANDGAP_ADR         0x1000096F

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_VREFBSEL_REGISTER    (HAL_AFEREG_MAP->REFS01)
#define HAL_AFE_VREFBSEL_LSB         0
#define HAL_AFE_VREFBSEL_MSB         2
#define HAL_AFE_VREFBSEL_MASK        0x00000007
#define HAL_AFE_VREFBSEL_TYPE        halAFE_VREFBSEL_t
#define HAL_AFE_VREFBSEL             HAL_AFE_VREFBSEL_MASK
#define HAL_AFE_VREFBSEL_READABLE    1
#define HAL_AFE_VREFBSEL_WRITABLE    1
#define HAL_AFE_VREFBSEL_SIGNED      0
#define HAL_AFE_VREFBSEL_ADR         0x1000096F

// Register RF00
#define HAL_AFE_PDRFALL_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_PDRFALL_LSB         7
#define HAL_AFE_PDRFALL_MSB         7
#define HAL_AFE_PDRFALL_MASK        0x00000080
#define HAL_AFE_PDRFALL_TYPE        Bool
#define HAL_AFE_PDRFALL             HAL_AFE_PDRFALL_MASK
#define HAL_AFE_PDRFALL_READABLE    1
#define HAL_AFE_PDRFALL_WRITABLE    1
#define HAL_AFE_PDRFALL_SIGNED      0
#define HAL_AFE_PDRFALL_ADR         0x10000970

#define HAL_AFE_PDRF_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_PDRF_LSB         6
#define HAL_AFE_PDRF_MSB         6
#define HAL_AFE_PDRF_MASK        0x00000040
#define HAL_AFE_PDRF_TYPE        Bool
#define HAL_AFE_PDRF             HAL_AFE_PDRF_MASK
#define HAL_AFE_PDRF_READABLE    1
#define HAL_AFE_PDRF_WRITABLE    1
#define HAL_AFE_PDRF_SIGNED      0
#define HAL_AFE_PDRF_ADR         0x10000970

#define HAL_AFE_PDDPD_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_PDDPD_LSB         5
#define HAL_AFE_PDDPD_MSB         5
#define HAL_AFE_PDDPD_MASK        0x00000020
#define HAL_AFE_PDDPD_TYPE        Bool
#define HAL_AFE_PDDPD             HAL_AFE_PDDPD_MASK
#define HAL_AFE_PDDPD_READABLE    1
#define HAL_AFE_PDDPD_WRITABLE    1
#define HAL_AFE_PDDPD_SIGNED      0
#define HAL_AFE_PDDPD_ADR         0x10000970

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_ENRFRP_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_ENRFRP_LSB         4
#define HAL_AFE_ENRFRP_MSB         4
#define HAL_AFE_ENRFRP_MASK        0x00000010
#define HAL_AFE_ENRFRP_TYPE        Bool
#define HAL_AFE_ENRFRP             HAL_AFE_ENRFRP_MASK
#define HAL_AFE_ENRFRP_READABLE    1
#define HAL_AFE_ENRFRP_WRITABLE    1
#define HAL_AFE_ENRFRP_SIGNED      0
#define HAL_AFE_ENRFRP_ADR         0x10000970

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_EQSVOSEL_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_EQSVOSEL_LSB         2
#define HAL_AFE_EQSVOSEL_MSB         3
#define HAL_AFE_EQSVOSEL_MASK        0x0000000C
#define HAL_AFE_EQSVOSEL_TYPE        halAFE_EQSVOSEL_t
#define HAL_AFE_EQSVOSEL             HAL_AFE_EQSVOSEL_MASK
#define HAL_AFE_EQSVOSEL_READABLE    1
#define HAL_AFE_EQSVOSEL_WRITABLE    1
#define HAL_AFE_EQSVOSEL_SIGNED      0
#define HAL_AFE_EQSVOSEL_ADR         0x10000970

#define HAL_AFE_EQDPDSEL_REGISTER    (HAL_AFEREG_MAP->RF00)
#define HAL_AFE_EQDPDSEL_LSB         0
#define HAL_AFE_EQDPDSEL_MSB         1
#define HAL_AFE_EQDPDSEL_MASK        0x00000003
#define HAL_AFE_EQDPDSEL_TYPE        halAFE_EQDPDSEL_t
#define HAL_AFE_EQDPDSEL             HAL_AFE_EQDPDSEL_MASK
#define HAL_AFE_EQDPDSEL_READABLE    1
#define HAL_AFE_EQDPDSEL_WRITABLE    1
#define HAL_AFE_EQDPDSEL_SIGNED      0
#define HAL_AFE_EQDPDSEL_ADR         0x10000970

// Register RF01
#define HAL_AFE_PDEQBUF_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_PDEQBUF_LSB         7
#define HAL_AFE_PDEQBUF_MSB         7
#define HAL_AFE_PDEQBUF_MASK        0x00000080
#define HAL_AFE_PDEQBUF_TYPE        Bool
#define HAL_AFE_PDEQBUF             HAL_AFE_PDEQBUF_MASK
#define HAL_AFE_PDEQBUF_READABLE    1
#define HAL_AFE_PDEQBUF_WRITABLE    1
#define HAL_AFE_PDEQBUF_SIGNED      0
#define HAL_AFE_PDEQBUF_ADR         0x10000971

#define HAL_AFE_RFADCTEN_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_RFADCTEN_LSB         6
#define HAL_AFE_RFADCTEN_MSB         6
#define HAL_AFE_RFADCTEN_MASK        0x00000040
#define HAL_AFE_RFADCTEN_TYPE        Bool
#define HAL_AFE_RFADCTEN             HAL_AFE_RFADCTEN_MASK
#define HAL_AFE_RFADCTEN_READABLE    1
#define HAL_AFE_RFADCTEN_WRITABLE    1
#define HAL_AFE_RFADCTEN_SIGNED      0
#define HAL_AFE_RFADCTEN_ADR         0x10000971

#define HAL_AFE_RFTPEN_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_RFTPEN_LSB         5
#define HAL_AFE_RFTPEN_MSB         5
#define HAL_AFE_RFTPEN_MASK        0x00000020
#define HAL_AFE_RFTPEN_TYPE        Bool
#define HAL_AFE_RFTPEN             HAL_AFE_RFTPEN_MASK
#define HAL_AFE_RFTPEN_READABLE    1
#define HAL_AFE_RFTPEN_WRITABLE    1
#define HAL_AFE_RFTPEN_SIGNED      0
#define HAL_AFE_RFTPEN_ADR         0x10000971

#define HAL_AFE_RFINGN_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_RFINGN_LSB         3
#define HAL_AFE_RFINGN_MSB         4
#define HAL_AFE_RFINGN_MASK        0x00000018
#define HAL_AFE_RFINGN_TYPE        UByte
#define HAL_AFE_RFINGN             HAL_AFE_RFINGN_MASK
#define HAL_AFE_RFINGN_READABLE    1
#define HAL_AFE_RFINGN_WRITABLE    1
#define HAL_AFE_RFINGN_SIGNED      0
#define HAL_AFE_RFINGN_ADR         0x10000971

#define HAL_AFE_DPDANATEN_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_DPDANATEN_LSB         2
#define HAL_AFE_DPDANATEN_MSB         2
#define HAL_AFE_DPDANATEN_MASK        0x00000004
#define HAL_AFE_DPDANATEN_TYPE        Bool
#define HAL_AFE_DPDANATEN             HAL_AFE_DPDANATEN_MASK
#define HAL_AFE_DPDANATEN_READABLE    1
#define HAL_AFE_DPDANATEN_WRITABLE    1
#define HAL_AFE_DPDANATEN_SIGNED      0
#define HAL_AFE_DPDANATEN_ADR         0x10000971

#define HAL_AFE_RFANATSEL_REGISTER    (HAL_AFEREG_MAP->RF01)
#define HAL_AFE_RFANATSEL_LSB         0
#define HAL_AFE_RFANATSEL_MSB         1
#define HAL_AFE_RFANATSEL_MASK        0x00000003
#define HAL_AFE_RFANATSEL_TYPE        halAFE_RFANATSEL_t
#define HAL_AFE_RFANATSEL             HAL_AFE_RFANATSEL_MASK
#define HAL_AFE_RFANATSEL_READABLE    1
#define HAL_AFE_RFANATSEL_WRITABLE    1
#define HAL_AFE_RFANATSEL_SIGNED      0
#define HAL_AFE_RFANATSEL_ADR         0x10000971

// Register RF02
#define HAL_AFE_EQBUFCUR_REGISTER    (HAL_AFEREG_MAP->RF02)
#define HAL_AFE_EQBUFCUR_LSB         6
#define HAL_AFE_EQBUFCUR_MSB         7
#define HAL_AFE_EQBUFCUR_MASK        0x000000C0
#define HAL_AFE_EQBUFCUR_TYPE        UByte
#define HAL_AFE_EQBUFCUR             HAL_AFE_EQBUFCUR_MASK
#define HAL_AFE_EQBUFCUR_READABLE    1
#define HAL_AFE_EQBUFCUR_WRITABLE    1
#define HAL_AFE_EQBUFCUR_SIGNED      0
#define HAL_AFE_EQBUFCUR_ADR         0x10000972

#define HAL_AFE_DPDTSEL_REGISTER    (HAL_AFEREG_MAP->RF02)
#define HAL_AFE_DPDTSEL_LSB         4
#define HAL_AFE_DPDTSEL_MSB         5
#define HAL_AFE_DPDTSEL_MASK        0x00000030
#define HAL_AFE_DPDTSEL_TYPE        halAFE_DPDTSEL_t
#define HAL_AFE_DPDTSEL             HAL_AFE_DPDTSEL_MASK
#define HAL_AFE_DPDTSEL_READABLE    1
#define HAL_AFE_DPDTSEL_WRITABLE    1
#define HAL_AFE_DPDTSEL_SIGNED      0
#define HAL_AFE_DPDTSEL_ADR         0x10000972

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_ENSRFOWDN_REGISTER    (HAL_AFEREG_MAP->RF02)
#define HAL_AFE_ENSRFOWDN_LSB         3
#define HAL_AFE_ENSRFOWDN_MSB         3
#define HAL_AFE_ENSRFOWDN_MASK        0x00000008
#define HAL_AFE_ENSRFOWDN_TYPE        Bool
#define HAL_AFE_ENSRFOWDN             HAL_AFE_ENSRFOWDN_MASK
#define HAL_AFE_ENSRFOWDN_READABLE    1
#define HAL_AFE_ENSRFOWDN_WRITABLE    1
#define HAL_AFE_ENSRFOWDN_SIGNED      0
#define HAL_AFE_ENSRFOWDN_ADR         0x10000972

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_EQDOSEL_REGISTER    (HAL_AFEREG_MAP->RF02)
#define HAL_AFE_EQDOSEL_LSB         0
#define HAL_AFE_EQDOSEL_MSB         2
#define HAL_AFE_EQDOSEL_MASK        0x00000007
#define HAL_AFE_EQDOSEL_TYPE        halAFE_EQDOSEL_t
#define HAL_AFE_EQDOSEL             HAL_AFE_EQDOSEL_MASK
#define HAL_AFE_EQDOSEL_READABLE    1
#define HAL_AFE_EQDOSEL_WRITABLE    1
#define HAL_AFE_EQDOSEL_SIGNED      0
#define HAL_AFE_EQDOSEL_ADR         0x10000972

// Register RF03

#if (CHIP_REV <= 0xC0) 

#define HAL_AFE_DPDAUXEN_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_DPDAUXEN_LSB         7
#define HAL_AFE_DPDAUXEN_MSB         7
#define HAL_AFE_DPDAUXEN_MASK        0x00000080
#define HAL_AFE_DPDAUXEN_TYPE        Bool
#define HAL_AFE_DPDAUXEN             HAL_AFE_DPDAUXEN_MASK
#define HAL_AFE_DPDAUXEN_READABLE    1
#define HAL_AFE_DPDAUXEN_WRITABLE    1
#define HAL_AFE_DPDAUXEN_SIGNED      0
#define HAL_AFE_DPDAUXEN_ADR         0x10000973

#endif // #if (CHIP_REV <= 0xC0)

#if (CHIP_REV >= 0xC0) // actually this is modified from C1, set it to C0 is because servo coefficient table uses same file for both C0 and C1

#define HAL_AFE_BPDPDHP_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_BPDPDHP_LSB         7
#define HAL_AFE_BPDPDHP_MSB         7
#define HAL_AFE_BPDPDHP_MASK        0x00000080
#define HAL_AFE_BPDPDHP_TYPE        Bool
#define HAL_AFE_BPDPDHP             HAL_AFE_BPDPDHP_MASK
#define HAL_AFE_BPDPDHP_READABLE    1
#define HAL_AFE_BPDPDHP_WRITABLE    1
#define HAL_AFE_BPDPDHP_SIGNED      0
#define HAL_AFE_BPDPDHP_ADR         0x10000973

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_VCMADEN_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_VCMADEN_LSB         6
#define HAL_AFE_VCMADEN_MSB         6
#define HAL_AFE_VCMADEN_MASK        0x00000040
#define HAL_AFE_VCMADEN_TYPE        Bool
#define HAL_AFE_VCMADEN             HAL_AFE_VCMADEN_MASK
#define HAL_AFE_VCMADEN_READABLE    1
#define HAL_AFE_VCMADEN_WRITABLE    1
#define HAL_AFE_VCMADEN_SIGNED      0
#define HAL_AFE_VCMADEN_ADR         0x10000973

#define HAL_AFE_RFAOCEN_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFAOCEN_LSB         5
#define HAL_AFE_RFAOCEN_MSB         5
#define HAL_AFE_RFAOCEN_MASK        0x00000020
#define HAL_AFE_RFAOCEN_TYPE        Bool
#define HAL_AFE_RFAOCEN             HAL_AFE_RFAOCEN_MASK
#define HAL_AFE_RFAOCEN_READABLE    1
#define HAL_AFE_RFAOCEN_WRITABLE    1
#define HAL_AFE_RFAOCEN_SIGNED      0
#define HAL_AFE_RFAOCEN_ADR         0x10000973

#define HAL_AFE_RFAGCEN_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFAGCEN_LSB         4
#define HAL_AFE_RFAGCEN_MSB         4
#define HAL_AFE_RFAGCEN_MASK        0x00000010
#define HAL_AFE_RFAGCEN_TYPE        Bool
#define HAL_AFE_RFAGCEN             HAL_AFE_RFAGCEN_MASK
#define HAL_AFE_RFAGCEN_READABLE    1
#define HAL_AFE_RFAGCEN_WRITABLE    1
#define HAL_AFE_RFAGCEN_SIGNED      0
#define HAL_AFE_RFAGCEN_ADR         0x10000973

#define HAL_AFE_RFOEN_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFOEN_LSB         3
#define HAL_AFE_RFOEN_MSB         3
#define HAL_AFE_RFOEN_MASK        0x00000008
#define HAL_AFE_RFOEN_TYPE        Bool
#define HAL_AFE_RFOEN             HAL_AFE_RFOEN_MASK
#define HAL_AFE_RFOEN_READABLE    1
#define HAL_AFE_RFOEN_WRITABLE    1
#define HAL_AFE_RFOEN_SIGNED      0
#define HAL_AFE_RFOEN_ADR         0x10000973

#define HAL_AFE_RFLNDSEL_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFLNDSEL_LSB         2
#define HAL_AFE_RFLNDSEL_MSB         2
#define HAL_AFE_RFLNDSEL_MASK        0x00000004
#define HAL_AFE_RFLNDSEL_TYPE        Bool
#define HAL_AFE_RFLNDSEL             HAL_AFE_RFLNDSEL_MASK
#define HAL_AFE_RFLNDSEL_READABLE    1
#define HAL_AFE_RFLNDSEL_WRITABLE    1
#define HAL_AFE_RFLNDSEL_SIGNED      0
#define HAL_AFE_RFLNDSEL_ADR         0x10000973

#define HAL_AFE_RFHDRSEL_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFHDRSEL_LSB         1
#define HAL_AFE_RFHDRSEL_MSB         1
#define HAL_AFE_RFHDRSEL_MASK        0x00000002
#define HAL_AFE_RFHDRSEL_TYPE        Bool
#define HAL_AFE_RFHDRSEL             HAL_AFE_RFHDRSEL_MASK
#define HAL_AFE_RFHDRSEL_READABLE    1
#define HAL_AFE_RFHDRSEL_WRITABLE    1
#define HAL_AFE_RFHDRSEL_SIGNED      0
#define HAL_AFE_RFHDRSEL_ADR         0x10000973

#define HAL_AFE_RFWLDSEL_REGISTER    (HAL_AFEREG_MAP->RF03)
#define HAL_AFE_RFWLDSEL_LSB         0
#define HAL_AFE_RFWLDSEL_MSB         0
#define HAL_AFE_RFWLDSEL_MASK        0x00000001
#define HAL_AFE_RFWLDSEL_TYPE        Bool
#define HAL_AFE_RFWLDSEL             HAL_AFE_RFWLDSEL_MASK
#define HAL_AFE_RFWLDSEL_READABLE    1
#define HAL_AFE_RFWLDSEL_WRITABLE    1
#define HAL_AFE_RFWLDSEL_SIGNED      0
#define HAL_AFE_RFWLDSEL_ADR         0x10000973

// Register RF04
#define HAL_AFE_RFINSEL_REGISTER    (HAL_AFEREG_MAP->RF04)
#define HAL_AFE_RFINSEL_LSB         6
#define HAL_AFE_RFINSEL_MSB         7
#define HAL_AFE_RFINSEL_MASK        0x000000C0
#define HAL_AFE_RFINSEL_TYPE        halAFE_RFINSEL_t
#define HAL_AFE_RFINSEL             HAL_AFE_RFINSEL_MASK
#define HAL_AFE_RFINSEL_READABLE    1
#define HAL_AFE_RFINSEL_WRITABLE    1
#define HAL_AFE_RFINSEL_SIGNED      0
#define HAL_AFE_RFINSEL_ADR         0x10000974

#define HAL_AFE_DPDDIGTEN_REGISTER    (HAL_AFEREG_MAP->RF04)
#define HAL_AFE_DPDDIGTEN_LSB         5
#define HAL_AFE_DPDDIGTEN_MSB         5
#define HAL_AFE_DPDDIGTEN_MASK        0x00000020
#define HAL_AFE_DPDDIGTEN_TYPE        Bool
#define HAL_AFE_DPDDIGTEN             HAL_AFE_DPDDIGTEN_MASK
#define HAL_AFE_DPDDIGTEN_READABLE    1
#define HAL_AFE_DPDDIGTEN_WRITABLE    1
#define HAL_AFE_DPDDIGTEN_SIGNED      0
#define HAL_AFE_DPDDIGTEN_ADR         0x10000974

#define HAL_AFE_RFAGCREG_REGISTER    (HAL_AFEREG_MAP->RF04)
#define HAL_AFE_RFAGCREG_LSB         0
#define HAL_AFE_RFAGCREG_MSB         4
#define HAL_AFE_RFAGCREG_MASK        0x0000001F
#define HAL_AFE_RFAGCREG_TYPE        halAFE_RFAGCGN_t
#define HAL_AFE_RFAGCREG             HAL_AFE_RFAGCREG_MASK
#define HAL_AFE_RFAGCREG_READABLE    1
#define HAL_AFE_RFAGCREG_WRITABLE    1
#define HAL_AFE_RFAGCREG_SIGNED      0
#define HAL_AFE_RFAGCREG_ADR         0x10000974

// Register RF05
#define HAL_AFE_RFHSSEL_REGISTER    (HAL_AFEREG_MAP->RF05)
#define HAL_AFE_RFHSSEL_LSB         6
#define HAL_AFE_RFHSSEL_MSB         7
#define HAL_AFE_RFHSSEL_MASK        0x000000C0
#define HAL_AFE_RFHSSEL_TYPE        halAFE_RFLPFBank_t
#define HAL_AFE_RFHSSEL             HAL_AFE_RFHSSEL_MASK
#define HAL_AFE_RFHSSEL_READABLE    1
#define HAL_AFE_RFHSSEL_WRITABLE    1
#define HAL_AFE_RFHSSEL_SIGNED      0
#define HAL_AFE_RFHSSEL_ADR         0x10000975

#define HAL_AFE_ENEXTRES_REGISTER    (HAL_AFEREG_MAP->RF05)
#define HAL_AFE_ENEXTRES_LSB         5
#define HAL_AFE_ENEXTRES_MSB         5
#define HAL_AFE_ENEXTRES_MASK        0x00000020
#define HAL_AFE_ENEXTRES_TYPE        Bool
#define HAL_AFE_ENEXTRES             HAL_AFE_ENEXTRES_MASK
#define HAL_AFE_ENEXTRES_READABLE    1
#define HAL_AFE_ENEXTRES_WRITABLE    1
#define HAL_AFE_ENEXTRES_SIGNED      0
#define HAL_AFE_ENEXTRES_ADR         0x10000975

#define HAL_AFE_RFHSFC_REGISTER    (HAL_AFEREG_MAP->RF05)
#define HAL_AFE_RFHSFC_LSB         0
#define HAL_AFE_RFHSFC_MSB         4
#define HAL_AFE_RFHSFC_MASK        0x0000001F
#define HAL_AFE_RFHSFC_TYPE        halAFE_RFLPFBW_t
#define HAL_AFE_RFHSFC             HAL_AFE_RFHSFC_MASK
#define HAL_AFE_RFHSFC_READABLE    1
#define HAL_AFE_RFHSFC_WRITABLE    1
#define HAL_AFE_RFHSFC_SIGNED      0
#define HAL_AFE_RFHSFC_ADR         0x10000975

// Register RF06

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RFGNWLD_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGNWLD_LSB         6
#define HAL_AFE_RFGNWLD_MSB         7
#define HAL_AFE_RFGNWLD_MASK        0x000000C0
#define HAL_AFE_RFGNWLD_TYPE        halAFE_RFWrGN_t
#define HAL_AFE_RFGNWLD             HAL_AFE_RFGNWLD_MASK
#define HAL_AFE_RFGNWLD_READABLE    1
#define HAL_AFE_RFGNWLD_WRITABLE    1
#define HAL_AFE_RFGNWLD_SIGNED      0
#define HAL_AFE_RFGNWLD_ADR         0x10000976

#define HAL_AFE_RFGNHDR_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGNHDR_LSB         4
#define HAL_AFE_RFGNHDR_MSB         5
#define HAL_AFE_RFGNHDR_MASK        0x00000030
#define HAL_AFE_RFGNHDR_TYPE        halAFE_RFGNHDR_t
#define HAL_AFE_RFGNHDR             HAL_AFE_RFGNHDR_MASK
#define HAL_AFE_RFGNHDR_READABLE    1
#define HAL_AFE_RFGNHDR_WRITABLE    1
#define HAL_AFE_RFGNHDR_SIGNED      0
#define HAL_AFE_RFGNHDR_ADR         0x10000976

#define HAL_AFE_RFGNLND_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGNLND_LSB         2
#define HAL_AFE_RFGNLND_MSB         3
#define HAL_AFE_RFGNLND_MASK        0x0000000C
#define HAL_AFE_RFGNLND_TYPE        halAFE_RFGNLND_t
#define HAL_AFE_RFGNLND             HAL_AFE_RFGNLND_MASK
#define HAL_AFE_RFGNLND_READABLE    1
#define HAL_AFE_RFGNLND_WRITABLE    1
#define HAL_AFE_RFGNLND_SIGNED      0
#define HAL_AFE_RFGNLND_ADR         0x10000976

#define HAL_AFE_RFGN_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGN_LSB         0
#define HAL_AFE_RFGN_MSB         1
#define HAL_AFE_RFGN_MASK        0x00000003
#define HAL_AFE_RFGN_TYPE        halAFE_RFGNGROV_t
#define HAL_AFE_RFGN             HAL_AFE_RFGN_MASK
#define HAL_AFE_RFGN_READABLE    1
#define HAL_AFE_RFGN_WRITABLE    1
#define HAL_AFE_RFGN_SIGNED      0
#define HAL_AFE_RFGN_ADR         0x10000976

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFGNLND_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGNLND_LSB         4
#define HAL_AFE_RFGNLND_MSB         6
#define HAL_AFE_RFGNLND_MASK        0x00000070
#define HAL_AFE_RFGNLND_TYPE        halAFE_RFGNLND_t
#define HAL_AFE_RFGNLND             HAL_AFE_RFGNLND_MASK
#define HAL_AFE_RFGNLND_READABLE    1
#define HAL_AFE_RFGNLND_WRITABLE    1
#define HAL_AFE_RFGNLND_SIGNED      0
#define HAL_AFE_RFGNLND_ADR         0x10000976

#define HAL_AFE_RFGN_REGISTER    (HAL_AFEREG_MAP->RF06)
#define HAL_AFE_RFGN_LSB         0
#define HAL_AFE_RFGN_MSB         2
#define HAL_AFE_RFGN_MASK        0x00000007
#define HAL_AFE_RFGN_TYPE        halAFE_RFGNGROV_t
#define HAL_AFE_RFGN             HAL_AFE_RFGN_MASK
#define HAL_AFE_RFGN_READABLE    1
#define HAL_AFE_RFGN_WRITABLE    1
#define HAL_AFE_RFGN_SIGNED      0
#define HAL_AFE_RFGN_ADR         0x10000976

#endif // #if (CHIP_REV >= 0xC0)

// Register RF07

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_DPDGN1SEL_REGISTER    (HAL_AFEREG_MAP->RF07)
#define HAL_AFE_DPDGN1SEL_LSB         7
#define HAL_AFE_DPDGN1SEL_MSB         7
#define HAL_AFE_DPDGN1SEL_MASK        0x00000080
#define HAL_AFE_DPDGN1SEL_TYPE        Bool
#define HAL_AFE_DPDGN1SEL             HAL_AFE_DPDGN1SEL_MASK
#define HAL_AFE_DPDGN1SEL_READABLE    1
#define HAL_AFE_DPDGN1SEL_WRITABLE    1
#define HAL_AFE_DPDGN1SEL_SIGNED      0
#define HAL_AFE_DPDGN1SEL_ADR         0x10000977

#define HAL_AFE_DPDGN2SEL_REGISTER    (HAL_AFEREG_MAP->RF07)
#define HAL_AFE_DPDGN2SEL_LSB         6
#define HAL_AFE_DPDGN2SEL_MSB         6
#define HAL_AFE_DPDGN2SEL_MASK        0x00000040
#define HAL_AFE_DPDGN2SEL_TYPE        Bool
#define HAL_AFE_DPDGN2SEL             HAL_AFE_DPDGN2SEL_MASK
#define HAL_AFE_DPDGN2SEL_READABLE    1
#define HAL_AFE_DPDGN2SEL_WRITABLE    1
#define HAL_AFE_DPDGN2SEL_SIGNED      0
#define HAL_AFE_DPDGN2SEL_ADR         0x10000977

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFPOS_REGISTER    (HAL_AFEREG_MAP->RF07)
#define HAL_AFE_RFPOS_LSB         0
#define HAL_AFE_RFPOS_MSB         5
#define HAL_AFE_RFPOS_MASK        0x0000003F
#define HAL_AFE_RFPOS_TYPE        UByte
#define HAL_AFE_RFPOS             HAL_AFE_RFPOS_MASK
#define HAL_AFE_RFPOS_READABLE    1
#define HAL_AFE_RFPOS_WRITABLE    1
#define HAL_AFE_RFPOS_SIGNED      0
#define HAL_AFE_RFPOS_ADR         0x10000977

// Register RF08
#define HAL_AFE_RFOBUFOS_REGISTER    (HAL_AFEREG_MAP->RF08)
#define HAL_AFE_RFOBUFOS_LSB         0
#define HAL_AFE_RFOBUFOS_MSB         6
#define HAL_AFE_RFOBUFOS_MASK        0x0000007F
#define HAL_AFE_RFOBUFOS_TYPE        UByte
#define HAL_AFE_RFOBUFOS             HAL_AFE_RFOBUFOS_MASK
#define HAL_AFE_RFOBUFOS_READABLE    1
#define HAL_AFE_RFOBUFOS_WRITABLE    1
#define HAL_AFE_RFOBUFOS_SIGNED      0
#define HAL_AFE_RFOBUFOS_ADR         0x10000978

// Register RF09

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_VCGMOS_REGISTER    (HAL_AFEREG_MAP->RF09)
#define HAL_AFE_VCGMOS_LSB         0
#define HAL_AFE_VCGMOS_MSB         6
#define HAL_AFE_VCGMOS_MASK        0x0000007F
#define HAL_AFE_VCGMOS_TYPE        UByte
#define HAL_AFE_VCGMOS             HAL_AFE_VCGMOS_MASK
#define HAL_AFE_VCGMOS_READABLE    1
#define HAL_AFE_VCGMOS_WRITABLE    1
#define HAL_AFE_VCGMOS_SIGNED      0
#define HAL_AFE_VCGMOS_ADR         0x10000979

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_DPDGN3SEL_REGISTER    (HAL_AFEREG_MAP->RF09)
#define HAL_AFE_DPDGN3SEL_LSB         7
#define HAL_AFE_DPDGN3SEL_MSB         7
#define HAL_AFE_DPDGN3SEL_MASK        0x00000080
#define HAL_AFE_DPDGN3SEL_TYPE        Bool
#define HAL_AFE_DPDGN3SEL             HAL_AFE_DPDGN3SEL_MASK
#define HAL_AFE_DPDGN3SEL_READABLE    1
#define HAL_AFE_DPDGN3SEL_WRITABLE    1
#define HAL_AFE_DPDGN3SEL_SIGNED      0
#define HAL_AFE_DPDGN3SEL_ADR         0x10000979

#define HAL_AFE_DPDGN4SEL_REGISTER    (HAL_AFEREG_MAP->RF09)
#define HAL_AFE_DPDGN4SEL_LSB         6
#define HAL_AFE_DPDGN4SEL_MSB         6
#define HAL_AFE_DPDGN4SEL_MASK        0x00000040
#define HAL_AFE_DPDGN4SEL_TYPE        Bool
#define HAL_AFE_DPDGN4SEL             HAL_AFE_DPDGN4SEL_MASK
#define HAL_AFE_DPDGN4SEL_READABLE    1
#define HAL_AFE_DPDGN4SEL_WRITABLE    1
#define HAL_AFE_DPDGN4SEL_SIGNED      0
#define HAL_AFE_DPDGN4SEL_ADR         0x10000979

#define HAL_AFE_RFVREFPOS_REGISTER    (HAL_AFEREG_MAP->RF09)
#define HAL_AFE_RFVREFPOS_LSB         0
#define HAL_AFE_RFVREFPOS_MSB         5
#define HAL_AFE_RFVREFPOS_MASK        0x0000003F
#define HAL_AFE_RFVREFPOS_TYPE        UByte
#define HAL_AFE_RFVREFPOS             HAL_AFE_RFVREFPOS_MASK
#define HAL_AFE_RFVREFPOS_READABLE    1
#define HAL_AFE_RFVREFPOS_WRITABLE    1
#define HAL_AFE_RFVREFPOS_SIGNED      0
#define HAL_AFE_RFVREFPOS_ADR         0x10000979

#endif // #if (CHIP_REV >= 0xC0)

// Register RF10
#define HAL_AFE_RFAOCREG_REGISTER    (HAL_AFEREG_MAP->RF10)
#define HAL_AFE_RFAOCREG_LSB         0
#define HAL_AFE_RFAOCREG_MSB         7
#define HAL_AFE_RFAOCREG_MASK        0x000000FF
#define HAL_AFE_RFAOCREG_TYPE        UByte
#define HAL_AFE_RFAOCREG             HAL_AFE_RFAOCREG_MASK
#define HAL_AFE_RFAOCREG_READABLE    1
#define HAL_AFE_RFAOCREG_WRITABLE    1
#define HAL_AFE_RFAOCREG_SIGNED      0
#define HAL_AFE_RFAOCREG_ADR         0x1000097A

// Register RF11

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP1UP_REGISTER    (HAL_AFEREG_MAP->RF11)
#define HAL_AFE_DPDCP1UP_LSB         7
#define HAL_AFE_DPDCP1UP_MSB         7
#define HAL_AFE_DPDCP1UP_MASK        0x00000080
#define HAL_AFE_DPDCP1UP_TYPE        Bool
#define HAL_AFE_DPDCP1UP             HAL_AFE_DPDCP1UP_MASK
#define HAL_AFE_DPDCP1UP_READABLE    1
#define HAL_AFE_DPDCP1UP_WRITABLE    1
#define HAL_AFE_DPDCP1UP_SIGNED      0
#define HAL_AFE_DPDCP1UP_ADR         0x1000097B

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSP_REGISTER    (HAL_AFEREG_MAP->RF11)
#define HAL_AFE_RFCOSP_LSB         0
#define HAL_AFE_RFCOSP_MSB         6
#define HAL_AFE_RFCOSP_MASK        0x0000007F
#define HAL_AFE_RFCOSP_TYPE        UByte
#define HAL_AFE_RFCOSP             HAL_AFE_RFCOSP_MASK
#define HAL_AFE_RFCOSP_READABLE    1
#define HAL_AFE_RFCOSP_WRITABLE    1
#define HAL_AFE_RFCOSP_SIGNED      0
#define HAL_AFE_RFCOSP_ADR         0x1000097B

// Register RF12

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP1DN_REGISTER    (HAL_AFEREG_MAP->RF12)
#define HAL_AFE_DPDCP1DN_LSB         7
#define HAL_AFE_DPDCP1DN_MSB         7
#define HAL_AFE_DPDCP1DN_MASK        0x00000080
#define HAL_AFE_DPDCP1DN_TYPE        Bool
#define HAL_AFE_DPDCP1DN             HAL_AFE_DPDCP1DN_MASK
#define HAL_AFE_DPDCP1DN_READABLE    1
#define HAL_AFE_DPDCP1DN_WRITABLE    1
#define HAL_AFE_DPDCP1DN_SIGNED      0
#define HAL_AFE_DPDCP1DN_ADR         0x1000097C

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSN_REGISTER    (HAL_AFEREG_MAP->RF12)
#define HAL_AFE_RFCOSN_LSB         0
#define HAL_AFE_RFCOSN_MSB         6
#define HAL_AFE_RFCOSN_MASK        0x0000007F
#define HAL_AFE_RFCOSN_TYPE        UByte
#define HAL_AFE_RFCOSN             HAL_AFE_RFCOSN_MASK
#define HAL_AFE_RFCOSN_READABLE    1
#define HAL_AFE_RFCOSN_WRITABLE    1
#define HAL_AFE_RFCOSN_SIGNED      0
#define HAL_AFE_RFCOSN_ADR         0x1000097C

// Register RF13

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP2UP_REGISTER    (HAL_AFEREG_MAP->RF13)
#define HAL_AFE_DPDCP2UP_LSB         7
#define HAL_AFE_DPDCP2UP_MSB         7
#define HAL_AFE_DPDCP2UP_MASK        0x00000080
#define HAL_AFE_DPDCP2UP_TYPE        Bool
#define HAL_AFE_DPDCP2UP             HAL_AFE_DPDCP2UP_MASK
#define HAL_AFE_DPDCP2UP_READABLE    1
#define HAL_AFE_DPDCP2UP_WRITABLE    1
#define HAL_AFE_DPDCP2UP_SIGNED      0
#define HAL_AFE_DPDCP2UP_ADR         0x1000097D

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSPLND_REGISTER    (HAL_AFEREG_MAP->RF13)
#define HAL_AFE_RFCOSPLND_LSB         0
#define HAL_AFE_RFCOSPLND_MSB         6
#define HAL_AFE_RFCOSPLND_MASK        0x0000007F
#define HAL_AFE_RFCOSPLND_TYPE        UByte
#define HAL_AFE_RFCOSPLND             HAL_AFE_RFCOSPLND_MASK
#define HAL_AFE_RFCOSPLND_READABLE    1
#define HAL_AFE_RFCOSPLND_WRITABLE    1
#define HAL_AFE_RFCOSPLND_SIGNED      0
#define HAL_AFE_RFCOSPLND_ADR         0x1000097D

// Register RF14

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP2DN_REGISTER    (HAL_AFEREG_MAP->RF14)
#define HAL_AFE_DPDCP2DN_LSB         7
#define HAL_AFE_DPDCP2DN_MSB         7
#define HAL_AFE_DPDCP2DN_MASK        0x00000080
#define HAL_AFE_DPDCP2DN_TYPE        Bool
#define HAL_AFE_DPDCP2DN             HAL_AFE_DPDCP2DN_MASK
#define HAL_AFE_DPDCP2DN_READABLE    1
#define HAL_AFE_DPDCP2DN_WRITABLE    1
#define HAL_AFE_DPDCP2DN_SIGNED      0
#define HAL_AFE_DPDCP2DN_ADR         0x1000097E

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSNLND_REGISTER    (HAL_AFEREG_MAP->RF14)
#define HAL_AFE_RFCOSNLND_LSB         0
#define HAL_AFE_RFCOSNLND_MSB         6
#define HAL_AFE_RFCOSNLND_MASK        0x0000007F
#define HAL_AFE_RFCOSNLND_TYPE        UByte
#define HAL_AFE_RFCOSNLND             HAL_AFE_RFCOSNLND_MASK
#define HAL_AFE_RFCOSNLND_READABLE    1
#define HAL_AFE_RFCOSNLND_WRITABLE    1
#define HAL_AFE_RFCOSNLND_SIGNED      0
#define HAL_AFE_RFCOSNLND_ADR         0x1000097E

// Register RF15

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP3UP_REGISTER    (HAL_AFEREG_MAP->RF15)
#define HAL_AFE_DPDCP3UP_LSB         7
#define HAL_AFE_DPDCP3UP_MSB         7
#define HAL_AFE_DPDCP3UP_MASK        0x00000080
#define HAL_AFE_DPDCP3UP_TYPE        Bool
#define HAL_AFE_DPDCP3UP             HAL_AFE_DPDCP3UP_MASK
#define HAL_AFE_DPDCP3UP_READABLE    1
#define HAL_AFE_DPDCP3UP_WRITABLE    1
#define HAL_AFE_DPDCP3UP_SIGNED      0
#define HAL_AFE_DPDCP3UP_ADR         0x1000097F

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSPHDR_REGISTER    (HAL_AFEREG_MAP->RF15)
#define HAL_AFE_RFCOSPHDR_LSB         0
#define HAL_AFE_RFCOSPHDR_MSB         6
#define HAL_AFE_RFCOSPHDR_MASK        0x0000007F
#define HAL_AFE_RFCOSPHDR_TYPE        UByte
#define HAL_AFE_RFCOSPHDR             HAL_AFE_RFCOSPHDR_MASK
#define HAL_AFE_RFCOSPHDR_READABLE    1
#define HAL_AFE_RFCOSPHDR_WRITABLE    1
#define HAL_AFE_RFCOSPHDR_SIGNED      0
#define HAL_AFE_RFCOSPHDR_ADR         0x1000097F

// Register RF16

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP3DN_REGISTER    (HAL_AFEREG_MAP->RF16)
#define HAL_AFE_DPDCP3DN_LSB         7
#define HAL_AFE_DPDCP3DN_MSB         7
#define HAL_AFE_DPDCP3DN_MASK        0x00000080
#define HAL_AFE_DPDCP3DN_TYPE        Bool
#define HAL_AFE_DPDCP3DN             HAL_AFE_DPDCP3DN_MASK
#define HAL_AFE_DPDCP3DN_READABLE    1
#define HAL_AFE_DPDCP3DN_WRITABLE    1
#define HAL_AFE_DPDCP3DN_SIGNED      0
#define HAL_AFE_DPDCP3DN_ADR         0x10000980

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSNHDR_REGISTER    (HAL_AFEREG_MAP->RF16)
#define HAL_AFE_RFCOSNHDR_LSB         0
#define HAL_AFE_RFCOSNHDR_MSB         6
#define HAL_AFE_RFCOSNHDR_MASK        0x0000007F
#define HAL_AFE_RFCOSNHDR_TYPE        UByte
#define HAL_AFE_RFCOSNHDR             HAL_AFE_RFCOSNHDR_MASK
#define HAL_AFE_RFCOSNHDR_READABLE    1
#define HAL_AFE_RFCOSNHDR_WRITABLE    1
#define HAL_AFE_RFCOSNHDR_SIGNED      0
#define HAL_AFE_RFCOSNHDR_ADR         0x10000980

// Register RF17

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP4UP_REGISTER    (HAL_AFEREG_MAP->RF17)
#define HAL_AFE_DPDCP4UP_LSB         7
#define HAL_AFE_DPDCP4UP_MSB         7
#define HAL_AFE_DPDCP4UP_MASK        0x00000080
#define HAL_AFE_DPDCP4UP_TYPE        Bool
#define HAL_AFE_DPDCP4UP             HAL_AFE_DPDCP4UP_MASK
#define HAL_AFE_DPDCP4UP_READABLE    1
#define HAL_AFE_DPDCP4UP_WRITABLE    1
#define HAL_AFE_DPDCP4UP_SIGNED      0
#define HAL_AFE_DPDCP4UP_ADR         0x10000981

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSPWLD_REGISTER    (HAL_AFEREG_MAP->RF17)
#define HAL_AFE_RFCOSPWLD_LSB         0
#define HAL_AFE_RFCOSPWLD_MSB         6
#define HAL_AFE_RFCOSPWLD_MASK        0x0000007F
#define HAL_AFE_RFCOSPWLD_TYPE        UByte
#define HAL_AFE_RFCOSPWLD             HAL_AFE_RFCOSPWLD_MASK
#define HAL_AFE_RFCOSPWLD_READABLE    1
#define HAL_AFE_RFCOSPWLD_WRITABLE    1
#define HAL_AFE_RFCOSPWLD_SIGNED      0
#define HAL_AFE_RFCOSPWLD_ADR         0x10000981

// Register RF18

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCP4DN_REGISTER    (HAL_AFEREG_MAP->RF18)
#define HAL_AFE_DPDCP4DN_LSB         7
#define HAL_AFE_DPDCP4DN_MSB         7
#define HAL_AFE_DPDCP4DN_MASK        0x00000080
#define HAL_AFE_DPDCP4DN_TYPE        Bool
#define HAL_AFE_DPDCP4DN             HAL_AFE_DPDCP4DN_MASK
#define HAL_AFE_DPDCP4DN_READABLE    1
#define HAL_AFE_DPDCP4DN_WRITABLE    1
#define HAL_AFE_DPDCP4DN_SIGNED      0
#define HAL_AFE_DPDCP4DN_ADR         0x10000982

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_RFCOSNWLD_REGISTER    (HAL_AFEREG_MAP->RF18)
#define HAL_AFE_RFCOSNWLD_LSB         0
#define HAL_AFE_RFCOSNWLD_MSB         6
#define HAL_AFE_RFCOSNWLD_MASK        0x0000007F
#define HAL_AFE_RFCOSNWLD_TYPE        UByte
#define HAL_AFE_RFCOSNWLD             HAL_AFE_RFCOSNWLD_MASK
#define HAL_AFE_RFCOSNWLD_READABLE    1
#define HAL_AFE_RFCOSNWLD_WRITABLE    1
#define HAL_AFE_RFCOSNWLD_SIGNED      0
#define HAL_AFE_RFCOSNWLD_ADR         0x10000982

// Register RF19

#if (CHIP_REV <= 0xB1)

#define HAL_AFE_DPDGMEN_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_DPDGMEN_LSB         7
#define HAL_AFE_DPDGMEN_MSB         7
#define HAL_AFE_DPDGMEN_MASK        0x00000080
#define HAL_AFE_DPDGMEN_TYPE        Bool
#define HAL_AFE_DPDGMEN             HAL_AFE_DPDGMEN_MASK
#define HAL_AFE_DPDGMEN_READABLE    1
#define HAL_AFE_DPDGMEN_WRITABLE    1
#define HAL_AFE_DPDGMEN_SIGNED      0
#define HAL_AFE_DPDGMEN_ADR         0x10000983

#define HAL_AFE_DPDGMDAC_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_DPDGMDAC_LSB         0
#define HAL_AFE_DPDGMDAC_MSB         3
#define HAL_AFE_DPDGMDAC_MASK        0x0000000F
#define HAL_AFE_DPDGMDAC_TYPE        UByte
#define HAL_AFE_DPDGMDAC             HAL_AFE_DPDGMDAC_MASK
#define HAL_AFE_DPDGMDAC_READABLE    1
#define HAL_AFE_DPDGMDAC_WRITABLE    1
#define HAL_AFE_DPDGMDAC_SIGNED      0
#define HAL_AFE_DPDGMDAC_ADR         0x10000983

#endif // #if (CHIP_REV <= 0xB1)

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RFGMEN_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_RFGMEN_LSB         7
#define HAL_AFE_RFGMEN_MSB         7
#define HAL_AFE_RFGMEN_MASK        0x00000080
#define HAL_AFE_RFGMEN_TYPE        Bool
#define HAL_AFE_RFGMEN             HAL_AFE_RFGMEN_MASK
#define HAL_AFE_RFGMEN_READABLE    1
#define HAL_AFE_RFGMEN_WRITABLE    1
#define HAL_AFE_RFGMEN_SIGNED      0
#define HAL_AFE_RFGMEN_ADR         0x10000983

#endif //#if (CHIP_REV <= 0xB3)

#if (CHIP_REV <= 0xB2)

#define HAL_AFE_PDIGM_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_PDIGM_LSB         6
#define HAL_AFE_PDIGM_MSB         6
#define HAL_AFE_PDIGM_MASK        0x00000040
#define HAL_AFE_PDIGM_TYPE        Bool
#define HAL_AFE_PDIGM             HAL_AFE_PDIGM_MASK
#define HAL_AFE_PDIGM_READABLE    1
#define HAL_AFE_PDIGM_WRITABLE    1
#define HAL_AFE_PDIGM_SIGNED      0
#define HAL_AFE_PDIGM_ADR         0x10000983

#define HAL_AFE_IGMBIASEL_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_IGMBIASEL_LSB         4
#define HAL_AFE_IGMBIASEL_MSB         5
#define HAL_AFE_IGMBIASEL_MASK        0x00000030
#define HAL_AFE_IGMBIASEL_TYPE        UByte
#define HAL_AFE_IGMBIASEL             HAL_AFE_IGMBIASEL_MASK
#define HAL_AFE_IGMBIASEL_READABLE    1
#define HAL_AFE_IGMBIASEL_WRITABLE    1
#define HAL_AFE_IGMBIASEL_SIGNED      0
#define HAL_AFE_IGMBIASEL_ADR         0x10000983

#endif // #if (CHIP_REV <= 0xB2)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_MAXIBSEL_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_MAXIBSEL_LSB         7
#define HAL_AFE_MAXIBSEL_MSB         7
#define HAL_AFE_MAXIBSEL_MASK        0x00000080
#define HAL_AFE_MAXIBSEL_TYPE        Bool
#define HAL_AFE_MAXIBSEL             HAL_AFE_MAXIBSEL_MASK
#define HAL_AFE_MAXIBSEL_READABLE    1
#define HAL_AFE_MAXIBSEL_WRITABLE    1
#define HAL_AFE_MAXIBSEL_SIGNED      0
#define HAL_AFE_MAXIBSEL_ADR         0x10000983

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_PDGMIBIAS_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_PDGMIBIAS_LSB         6
#define HAL_AFE_PDGMIBIAS_MSB         6
#define HAL_AFE_PDGMIBIAS_MASK        0x00000040
#define HAL_AFE_PDGMIBIAS_TYPE        Bool
#define HAL_AFE_PDGMIBIAS             HAL_AFE_PDGMIBIAS_MASK
#define HAL_AFE_PDGMIBIAS_READABLE    1
#define HAL_AFE_PDGMIBIAS_WRITABLE    1
#define HAL_AFE_PDGMIBIAS_SIGNED      0
#define HAL_AFE_PDGMIBIAS_ADR         0x10000983

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RFIBSEL_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_RFIBSEL_LSB         4
#define HAL_AFE_RFIBSEL_MSB         5
#define HAL_AFE_RFIBSEL_MASK        0x00000030
#define HAL_AFE_RFIBSEL_TYPE        UByte
#define HAL_AFE_RFIBSEL             HAL_AFE_RFIBSEL_MASK
#define HAL_AFE_RFIBSEL_READABLE    1
#define HAL_AFE_RFIBSEL_WRITABLE    1
#define HAL_AFE_RFIBSEL_SIGNED      0
#define HAL_AFE_RFIBSEL_ADR         0x10000983

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFIBSEL_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_RFIBSEL_LSB         4
#define HAL_AFE_RFIBSEL_MSB         5
#define HAL_AFE_RFIBSEL_MASK        0x00000030
#define HAL_AFE_RFIBSEL_TYPE        halAFE_RFIBSEL_t
#define HAL_AFE_RFIBSEL             HAL_AFE_RFIBSEL_MASK
#define HAL_AFE_RFIBSEL_READABLE    1
#define HAL_AFE_RFIBSEL_WRITABLE    1
#define HAL_AFE_RFIBSEL_SIGNED      0
#define HAL_AFE_RFIBSEL_ADR         0x10000983

#define HAL_AFE_RFGNHDR_REGISTER    (HAL_AFEREG_MAP->RF19)
#define HAL_AFE_RFGNHDR_LSB         0
#define HAL_AFE_RFGNHDR_MSB         2
#define HAL_AFE_RFGNHDR_MASK        0x00000007
#define HAL_AFE_RFGNHDR_TYPE        halAFE_RFGNHDR_t
#define HAL_AFE_RFGNHDR             HAL_AFE_RFGNHDR_MASK
#define HAL_AFE_RFGNHDR_READABLE    1
#define HAL_AFE_RFGNHDR_WRITABLE    1
#define HAL_AFE_RFGNHDR_SIGNED      0
#define HAL_AFE_RFGNHDR_ADR         0x10000983

#endif // #if (CHIP_REV >= 0xC0)

// Register RF20
#define HAL_AFE_DPD1SEL_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD1SEL_LSB         7
#define HAL_AFE_DPD1SEL_MSB         7
#define HAL_AFE_DPD1SEL_MASK        0x00000080
#define HAL_AFE_DPD1SEL_TYPE        Bool
#define HAL_AFE_DPD1SEL             HAL_AFE_DPD1SEL_MASK
#define HAL_AFE_DPD1SEL_READABLE    1
#define HAL_AFE_DPD1SEL_WRITABLE    1
#define HAL_AFE_DPD1SEL_SIGNED      0
#define HAL_AFE_DPD1SEL_ADR         0x10000984

#define HAL_AFE_DPD2SEL_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD2SEL_LSB         6
#define HAL_AFE_DPD2SEL_MSB         6
#define HAL_AFE_DPD2SEL_MASK        0x00000040
#define HAL_AFE_DPD2SEL_TYPE        Bool
#define HAL_AFE_DPD2SEL             HAL_AFE_DPD2SEL_MASK
#define HAL_AFE_DPD2SEL_READABLE    1
#define HAL_AFE_DPD2SEL_WRITABLE    1
#define HAL_AFE_DPD2SEL_SIGNED      0
#define HAL_AFE_DPD2SEL_ADR         0x10000984

#define HAL_AFE_DPD3SEL_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD3SEL_LSB         5
#define HAL_AFE_DPD3SEL_MSB         5
#define HAL_AFE_DPD3SEL_MASK        0x00000020
#define HAL_AFE_DPD3SEL_TYPE        Bool
#define HAL_AFE_DPD3SEL             HAL_AFE_DPD3SEL_MASK
#define HAL_AFE_DPD3SEL_READABLE    1
#define HAL_AFE_DPD3SEL_WRITABLE    1
#define HAL_AFE_DPD3SEL_SIGNED      0
#define HAL_AFE_DPD3SEL_ADR         0x10000984

#define HAL_AFE_DPD4SEL_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD4SEL_LSB         4
#define HAL_AFE_DPD4SEL_MSB         4
#define HAL_AFE_DPD4SEL_MASK        0x00000010
#define HAL_AFE_DPD4SEL_TYPE        Bool
#define HAL_AFE_DPD4SEL             HAL_AFE_DPD4SEL_MASK
#define HAL_AFE_DPD4SEL_READABLE    1
#define HAL_AFE_DPD4SEL_WRITABLE    1
#define HAL_AFE_DPD4SEL_SIGNED      0
#define HAL_AFE_DPD4SEL_ADR         0x10000984

#define HAL_AFE_DPD12INV_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD12INV_LSB         3
#define HAL_AFE_DPD12INV_MSB         3
#define HAL_AFE_DPD12INV_MASK        0x00000008
#define HAL_AFE_DPD12INV_TYPE        Bool
#define HAL_AFE_DPD12INV             HAL_AFE_DPD12INV_MASK
#define HAL_AFE_DPD12INV_READABLE    1
#define HAL_AFE_DPD12INV_WRITABLE    1
#define HAL_AFE_DPD12INV_SIGNED      0
#define HAL_AFE_DPD12INV_ADR         0x10000984

#define HAL_AFE_DPD34INV_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD34INV_LSB         2
#define HAL_AFE_DPD34INV_MSB         2
#define HAL_AFE_DPD34INV_MASK        0x00000004
#define HAL_AFE_DPD34INV_TYPE        Bool
#define HAL_AFE_DPD34INV             HAL_AFE_DPD34INV_MASK
#define HAL_AFE_DPD34INV_READABLE    1
#define HAL_AFE_DPD34INV_WRITABLE    1
#define HAL_AFE_DPD34INV_SIGNED      0
#define HAL_AFE_DPD34INV_ADR         0x10000984

#define HAL_AFE_DPD12EN_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD12EN_LSB         1
#define HAL_AFE_DPD12EN_MSB         1
#define HAL_AFE_DPD12EN_MASK        0x00000002
#define HAL_AFE_DPD12EN_TYPE        Bool
#define HAL_AFE_DPD12EN             HAL_AFE_DPD12EN_MASK
#define HAL_AFE_DPD12EN_READABLE    1
#define HAL_AFE_DPD12EN_WRITABLE    1
#define HAL_AFE_DPD12EN_SIGNED      0
#define HAL_AFE_DPD12EN_ADR         0x10000984

#define HAL_AFE_DPD34EN_REGISTER    (HAL_AFEREG_MAP->RF20)
#define HAL_AFE_DPD34EN_LSB         0
#define HAL_AFE_DPD34EN_MSB         0
#define HAL_AFE_DPD34EN_MASK        0x00000001
#define HAL_AFE_DPD34EN_TYPE        Bool
#define HAL_AFE_DPD34EN             HAL_AFE_DPD34EN_MASK
#define HAL_AFE_DPD34EN_READABLE    1
#define HAL_AFE_DPD34EN_WRITABLE    1
#define HAL_AFE_DPD34EN_SIGNED      0
#define HAL_AFE_DPD34EN_ADR         0x10000984

// Register RF21

#if (CHIP_REV <= 0xC0)

#define HAL_AFE_DPDCPHLD_REGISTER    (HAL_AFEREG_MAP->RF21)
#define HAL_AFE_DPDCPHLD_LSB         7
#define HAL_AFE_DPDCPHLD_MSB         7
#define HAL_AFE_DPDCPHLD_MASK        0x00000080
#define HAL_AFE_DPDCPHLD_TYPE        Bool
#define HAL_AFE_DPDCPHLD             HAL_AFE_DPDCPHLD_MASK
#define HAL_AFE_DPDCPHLD_READABLE    1
#define HAL_AFE_DPDCPHLD_WRITABLE    1
#define HAL_AFE_DPDCPHLD_SIGNED      0
#define HAL_AFE_DPDCPHLD_ADR         0x10000985

#endif // #if (CHIP_REV <= 0xC0)

#if (CHIP_REV >= 0xC0) // actually this is modified from C1, set it to C0 is because servo coefficient table uses same file for both C0 and C1

#define HAL_AFE_DPDCPTEN_REGISTER    (HAL_AFEREG_MAP->RF21)
#define HAL_AFE_DPDCPTEN_LSB         7
#define HAL_AFE_DPDCPTEN_MSB         7
#define HAL_AFE_DPDCPTEN_MASK        0x00000080
#define HAL_AFE_DPDCPTEN_TYPE        Bool
#define HAL_AFE_DPDCPTEN             HAL_AFE_DPDCPTEN_MASK
#define HAL_AFE_DPDCPTEN_READABLE    1
#define HAL_AFE_DPDCPTEN_WRITABLE    1
#define HAL_AFE_DPDCPTEN_SIGNED      0
#define HAL_AFE_DPDCPTEN_ADR         0x10000985

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_DPDPDMODE_REGISTER    (HAL_AFEREG_MAP->RF21)
#define HAL_AFE_DPDPDMODE_LSB         6
#define HAL_AFE_DPDPDMODE_MSB         6
#define HAL_AFE_DPDPDMODE_MASK        0x00000040
#define HAL_AFE_DPDPDMODE_TYPE        Bool
#define HAL_AFE_DPDPDMODE             HAL_AFE_DPDPDMODE_MASK
#define HAL_AFE_DPDPDMODE_READABLE    1
#define HAL_AFE_DPDPDMODE_WRITABLE    1
#define HAL_AFE_DPDPDMODE_SIGNED      0
#define HAL_AFE_DPDPDMODE_ADR         0x10000985

#define HAL_AFE_DPDINFC_REGISTER    (HAL_AFEREG_MAP->RF21)
#define HAL_AFE_DPDINFC_LSB         4
#define HAL_AFE_DPDINFC_MSB         5
#define HAL_AFE_DPDINFC_MASK        0x00000030
#define HAL_AFE_DPDINFC_TYPE        halAFE_DPDINHPFBW_t
#define HAL_AFE_DPDINFC             HAL_AFE_DPDINFC_MASK
#define HAL_AFE_DPDINFC_READABLE    1
#define HAL_AFE_DPDINFC_WRITABLE    1
#define HAL_AFE_DPDINFC_SIGNED      0
#define HAL_AFE_DPDINFC_ADR         0x10000985

#define HAL_AFE_DPDGN_REGISTER    (HAL_AFEREG_MAP->RF21)
#define HAL_AFE_DPDGN_LSB         0
#define HAL_AFE_DPDGN_MSB         3
#define HAL_AFE_DPDGN_MASK        0x0000000F
#define HAL_AFE_DPDGN_TYPE        halAFE_DPDGN_t
#define HAL_AFE_DPDGN             HAL_AFE_DPDGN_MASK
#define HAL_AFE_DPDGN_READABLE    1
#define HAL_AFE_DPDGN_WRITABLE    1
#define HAL_AFE_DPDGN_SIGNED      0
#define HAL_AFE_DPDGN_ADR         0x10000985

// Register RF22
#define HAL_AFE_DPDHSSEL_REGISTER    (HAL_AFEREG_MAP->RF22)
#define HAL_AFE_DPDHSSEL_LSB         7
#define HAL_AFE_DPDHSSEL_MSB         7
#define HAL_AFE_DPDHSSEL_MASK        0x00000080
#define HAL_AFE_DPDHSSEL_TYPE        Bool
#define HAL_AFE_DPDHSSEL             HAL_AFE_DPDHSSEL_MASK
#define HAL_AFE_DPDHSSEL_READABLE    1
#define HAL_AFE_DPDHSSEL_WRITABLE    1
#define HAL_AFE_DPDHSSEL_SIGNED      0
#define HAL_AFE_DPDHSSEL_ADR         0x10000986

#define HAL_AFE_DPDHSISCL_REGISTER    (HAL_AFEREG_MAP->RF22)
#define HAL_AFE_DPDHSISCL_LSB         5
#define HAL_AFE_DPDHSISCL_MSB         6
#define HAL_AFE_DPDHSISCL_MASK        0x00000060
#define HAL_AFE_DPDHSISCL_TYPE        UByte
#define HAL_AFE_DPDHSISCL             HAL_AFE_DPDHSISCL_MASK
#define HAL_AFE_DPDHSISCL_READABLE    1
#define HAL_AFE_DPDHSISCL_WRITABLE    1
#define HAL_AFE_DPDHSISCL_SIGNED      0
#define HAL_AFE_DPDHSISCL_ADR         0x10000986

#define HAL_AFE_DPDHSFCH_REGISTER    (HAL_AFEREG_MAP->RF22)
#define HAL_AFE_DPDHSFCH_LSB         0
#define HAL_AFE_DPDHSFCH_MSB         4
#define HAL_AFE_DPDHSFCH_MASK        0x0000001F
#define HAL_AFE_DPDHSFCH_TYPE        halAFE_DPDEQBWH_t
#define HAL_AFE_DPDHSFCH             HAL_AFE_DPDHSFCH_MASK
#define HAL_AFE_DPDHSFCH_READABLE    1
#define HAL_AFE_DPDHSFCH_WRITABLE    1
#define HAL_AFE_DPDHSFCH_SIGNED      0
#define HAL_AFE_DPDHSFCH_ADR         0x10000986

// Register RF23

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCMTENP_REGISTER    (HAL_AFEREG_MAP->RF23)
#define HAL_AFE_DPDCMTENP_LSB         7
#define HAL_AFE_DPDCMTENP_MSB         7
#define HAL_AFE_DPDCMTENP_MASK        0x00000080
#define HAL_AFE_DPDCMTENP_TYPE        Bool
#define HAL_AFE_DPDCMTENP             HAL_AFE_DPDCMTENP_MASK
#define HAL_AFE_DPDCMTENP_READABLE    1
#define HAL_AFE_DPDCMTENP_WRITABLE    1
#define HAL_AFE_DPDCMTENP_SIGNED      0
#define HAL_AFE_DPDCMTENP_ADR         0x10000987

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDRISEN_REGISTER    (HAL_AFEREG_MAP->RF23)
#define HAL_AFE_DPDRISEN_LSB         6
#define HAL_AFE_DPDRISEN_MSB         6
#define HAL_AFE_DPDRISEN_MASK        0x00000040
#define HAL_AFE_DPDRISEN_TYPE        Bool
#define HAL_AFE_DPDRISEN             HAL_AFE_DPDRISEN_MASK
#define HAL_AFE_DPDRISEN_READABLE    1
#define HAL_AFE_DPDRISEN_WRITABLE    1
#define HAL_AFE_DPDRISEN_SIGNED      0
#define HAL_AFE_DPDRISEN_ADR         0x10000987

#define HAL_AFE_DPDFALEN_REGISTER    (HAL_AFEREG_MAP->RF23)
#define HAL_AFE_DPDFALEN_LSB         5
#define HAL_AFE_DPDFALEN_MSB         5
#define HAL_AFE_DPDFALEN_MASK        0x00000020
#define HAL_AFE_DPDFALEN_TYPE        Bool
#define HAL_AFE_DPDFALEN             HAL_AFE_DPDFALEN_MASK
#define HAL_AFE_DPDFALEN_READABLE    1
#define HAL_AFE_DPDFALEN_WRITABLE    1
#define HAL_AFE_DPDFALEN_SIGNED      0
#define HAL_AFE_DPDFALEN_ADR         0x10000987

#define HAL_AFE_DPDHSFCL_REGISTER    (HAL_AFEREG_MAP->RF23)
#define HAL_AFE_DPDHSFCL_LSB         0
#define HAL_AFE_DPDHSFCL_MSB         4
#define HAL_AFE_DPDHSFCL_MASK        0x0000001F
#define HAL_AFE_DPDHSFCL_TYPE        halAFE_DPDEQBWL_t
#define HAL_AFE_DPDHSFCL             HAL_AFE_DPDHSFCL_MASK
#define HAL_AFE_DPDHSFCL_READABLE    1
#define HAL_AFE_DPDHSFCL_WRITABLE    1
#define HAL_AFE_DPDHSFCL_SIGNED      0
#define HAL_AFE_DPDHSFCL_ADR         0x10000987

// Register RF24

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_DPDVCGM_REGISTER    (HAL_AFEREG_MAP->RF24)
#define HAL_AFE_DPDVCGM_LSB         4
#define HAL_AFE_DPDVCGM_MSB         7
#define HAL_AFE_DPDVCGM_MASK        0x000000F0
#define HAL_AFE_DPDVCGM_TYPE        UByte
#define HAL_AFE_DPDVCGM             HAL_AFE_DPDVCGM_MASK
#define HAL_AFE_DPDVCGM_READABLE    1
#define HAL_AFE_DPDVCGM_WRITABLE    1
#define HAL_AFE_DPDVCGM_SIGNED      0
#define HAL_AFE_DPDVCGM_ADR         0x10000988

#define HAL_AFE_DPDBST_REGISTER    (HAL_AFEREG_MAP->RF24)
#define HAL_AFE_DPDBST_LSB         0
#define HAL_AFE_DPDBST_MSB         3
#define HAL_AFE_DPDBST_MASK        0x0000000F
#define HAL_AFE_DPDBST_TYPE        halAFE_DPDEQGNBoost_t
#define HAL_AFE_DPDBST             HAL_AFE_DPDBST_MASK
#define HAL_AFE_DPDBST_READABLE    1
#define HAL_AFE_DPDBST_WRITABLE    1
#define HAL_AFE_DPDBST_SIGNED      0
#define HAL_AFE_DPDBST_ADR         0x10000988

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_DPDVCGM_REGISTER    (HAL_AFEREG_MAP->RF24)
#define HAL_AFE_DPDVCGM_LSB         4
#define HAL_AFE_DPDVCGM_MSB         5
#define HAL_AFE_DPDVCGM_MASK        0x00000030
#define HAL_AFE_DPDVCGM_TYPE        UByte
#define HAL_AFE_DPDVCGM             HAL_AFE_DPDVCGM_MASK
#define HAL_AFE_DPDVCGM_READABLE    1
#define HAL_AFE_DPDVCGM_WRITABLE    1
#define HAL_AFE_DPDVCGM_SIGNED      0
#define HAL_AFE_DPDVCGM_ADR         0x10000988

#define HAL_AFE_DPDBST_REGISTER    (HAL_AFEREG_MAP->RF24)
#define HAL_AFE_DPDBST_LSB         0
#define HAL_AFE_DPDBST_MSB         2
#define HAL_AFE_DPDBST_MASK        0x00000007
#define HAL_AFE_DPDBST_TYPE        halAFE_DPDEQGNBoost_t
#define HAL_AFE_DPDBST             HAL_AFE_DPDBST_MASK
#define HAL_AFE_DPDBST_READABLE    1
#define HAL_AFE_DPDBST_WRITABLE    1
#define HAL_AFE_DPDBST_SIGNED      0
#define HAL_AFE_DPDBST_ADR         0x10000988

#endif // #if (CHIP_REV >= 0xC0)

// Register RF25

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RFGMDAC_REGISTER    (HAL_AFEREG_MAP->RF25)
#define HAL_AFE_RFGMDAC_LSB         4
#define HAL_AFE_RFGMDAC_MSB         7
#define HAL_AFE_RFGMDAC_MASK        0x000000F0
#define HAL_AFE_RFGMDAC_TYPE        UByte
#define HAL_AFE_RFGMDAC             HAL_AFE_RFGMDAC_MASK
#define HAL_AFE_RFGMDAC_READABLE    1
#define HAL_AFE_RFGMDAC_WRITABLE    1
#define HAL_AFE_RFGMDAC_SIGNED      0
#define HAL_AFE_RFGMDAC_ADR         0x10000989

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC1)

#define HAL_AFE_DPDCMTENN_REGISTER    (HAL_AFEREG_MAP->RF25)
#define HAL_AFE_DPDCMTENN_LSB         7
#define HAL_AFE_DPDCMTENN_MSB         7
#define HAL_AFE_DPDCMTENN_MASK        0x00000080
#define HAL_AFE_DPDCMTENN_TYPE        Bool
#define HAL_AFE_DPDCMTENN             HAL_AFE_DPDCMTENN_MASK
#define HAL_AFE_DPDCMTENN_READABLE    1
#define HAL_AFE_DPDCMTENN_WRITABLE    1
#define HAL_AFE_DPDCMTENN_SIGNED      0
#define HAL_AFE_DPDCMTENN_ADR         0x10000989

#endif // #if (CHIP_REV >= 0xC1)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RFGNWLD_REGISTER    (HAL_AFEREG_MAP->RF25)
#define HAL_AFE_RFGNWLD_LSB         4
#define HAL_AFE_RFGNWLD_MSB         6
#define HAL_AFE_RFGNWLD_MASK        0x00000070
#define HAL_AFE_RFGNWLD_TYPE        halAFE_RFWrGN_t
#define HAL_AFE_RFGNWLD             HAL_AFE_RFGNWLD_MASK
#define HAL_AFE_RFGNWLD_READABLE    1
#define HAL_AFE_RFGNWLD_WRITABLE    1
#define HAL_AFE_RFGNWLD_SIGNED      0
#define HAL_AFE_RFGNWLD_ADR         0x10000989

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_DPDCPGN_REGISTER    (HAL_AFEREG_MAP->RF25)
#define HAL_AFE_DPDCPGN_LSB         2
#define HAL_AFE_DPDCPGN_MSB         3
#define HAL_AFE_DPDCPGN_MASK        0x0000000C
#define HAL_AFE_DPDCPGN_TYPE        halAFE_DPDCPGN_t
#define HAL_AFE_DPDCPGN             HAL_AFE_DPDCPGN_MASK
#define HAL_AFE_DPDCPGN_READABLE    1
#define HAL_AFE_DPDCPGN_WRITABLE    1
#define HAL_AFE_DPDCPGN_SIGNED      0
#define HAL_AFE_DPDCPGN_ADR         0x10000989

#define HAL_AFE_DPDCPFC_REGISTER    (HAL_AFEREG_MAP->RF25)
#define HAL_AFE_DPDCPFC_LSB         0
#define HAL_AFE_DPDCPFC_MSB         1
#define HAL_AFE_DPDCPFC_MASK        0x00000003
#define HAL_AFE_DPDCPFC_TYPE        halAFE_DPDCPFBW_t
#define HAL_AFE_DPDCPFC             HAL_AFE_DPDCPFC_MASK
#define HAL_AFE_DPDCPFC_READABLE    1
#define HAL_AFE_DPDCPFC_WRITABLE    1
#define HAL_AFE_DPDCPFC_SIGNED      0
#define HAL_AFE_DPDCPFC_ADR         0x10000989

// Register WOBAPC00
#define HAL_AFE_PDWSH12_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_PDWSH12_LSB         7
#define HAL_AFE_PDWSH12_MSB         7
#define HAL_AFE_PDWSH12_MASK        0x00000080
#define HAL_AFE_PDWSH12_TYPE        Bool
#define HAL_AFE_PDWSH12             HAL_AFE_PDWSH12_MASK
#define HAL_AFE_PDWSH12_READABLE    1
#define HAL_AFE_PDWSH12_WRITABLE    1
#define HAL_AFE_PDWSH12_SIGNED      0
#define HAL_AFE_PDWSH12_ADR         0x1000098A

#define HAL_AFE_PDRSH34_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_PDRSH34_LSB         6
#define HAL_AFE_PDRSH34_MSB         6
#define HAL_AFE_PDRSH34_MASK        0x00000040
#define HAL_AFE_PDRSH34_TYPE        Bool
#define HAL_AFE_PDRSH34             HAL_AFE_PDRSH34_MASK
#define HAL_AFE_PDRSH34_READABLE    1
#define HAL_AFE_PDRSH34_WRITABLE    1
#define HAL_AFE_PDRSH34_SIGNED      0
#define HAL_AFE_PDRSH34_ADR         0x1000098A

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_S3S4FDVRSL_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_S3S4FDVRSL_LSB         5
#define HAL_AFE_S3S4FDVRSL_MSB         5
#define HAL_AFE_S3S4FDVRSL_MASK        0x00000020
#define HAL_AFE_S3S4FDVRSL_TYPE        Bool
#define HAL_AFE_S3S4FDVRSL             HAL_AFE_S3S4FDVRSL_MASK
#define HAL_AFE_S3S4FDVRSL_READABLE    1
#define HAL_AFE_S3S4FDVRSL_WRITABLE    1
#define HAL_AFE_S3S4FDVRSL_SIGNED      0
#define HAL_AFE_S3S4FDVRSL_ADR         0x1000098A

#define HAL_AFE_SHNOLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_SHNOLPON_LSB         4
#define HAL_AFE_SHNOLPON_MSB         4
#define HAL_AFE_SHNOLPON_MASK        0x00000010
#define HAL_AFE_SHNOLPON_TYPE        Bool
#define HAL_AFE_SHNOLPON             HAL_AFE_SHNOLPON_MASK
#define HAL_AFE_SHNOLPON_READABLE    1
#define HAL_AFE_SHNOLPON_WRITABLE    1
#define HAL_AFE_SHNOLPON_SIGNED      0
#define HAL_AFE_SHNOLPON_ADR         0x1000098A

#define HAL_AFE_SHLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_SHLPON_LSB         3
#define HAL_AFE_SHLPON_MSB         3
#define HAL_AFE_SHLPON_MASK        0x00000008
#define HAL_AFE_SHLPON_TYPE        Bool
#define HAL_AFE_SHLPON             HAL_AFE_SHLPON_MASK
#define HAL_AFE_SHLPON_READABLE    1
#define HAL_AFE_SHLPON_WRITABLE    1
#define HAL_AFE_SHLPON_SIGNED      0
#define HAL_AFE_SHLPON_ADR         0x1000098A

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_S34FDVRSL_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_S34FDVRSL_LSB         5
#define HAL_AFE_S34FDVRSL_MSB         5
#define HAL_AFE_S34FDVRSL_MASK        0x00000020
#define HAL_AFE_S34FDVRSL_TYPE        Bool
#define HAL_AFE_S34FDVRSL             HAL_AFE_S34FDVRSL_MASK
#define HAL_AFE_S34FDVRSL_READABLE    1
#define HAL_AFE_S34FDVRSL_WRITABLE    1
#define HAL_AFE_S34FDVRSL_SIGNED      0
#define HAL_AFE_S34FDVRSL_ADR         0x1000098A

#define HAL_AFE_WSHNOLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_WSHNOLPON_LSB         4
#define HAL_AFE_WSHNOLPON_MSB         4
#define HAL_AFE_WSHNOLPON_MASK        0x00000010
#define HAL_AFE_WSHNOLPON_TYPE        Bool
#define HAL_AFE_WSHNOLPON             HAL_AFE_WSHNOLPON_MASK
#define HAL_AFE_WSHNOLPON_READABLE    1
#define HAL_AFE_WSHNOLPON_WRITABLE    1
#define HAL_AFE_WSHNOLPON_SIGNED      0
#define HAL_AFE_WSHNOLPON_ADR         0x1000098A

#define HAL_AFE_WSHLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_WSHLPON_LSB         3
#define HAL_AFE_WSHLPON_MSB         3
#define HAL_AFE_WSHLPON_MASK        0x00000008
#define HAL_AFE_WSHLPON_TYPE        Bool
#define HAL_AFE_WSHLPON             HAL_AFE_WSHLPON_MASK
#define HAL_AFE_WSHLPON_READABLE    1
#define HAL_AFE_WSHLPON_WRITABLE    1
#define HAL_AFE_WSHLPON_SIGNED      0
#define HAL_AFE_WSHLPON_ADR         0x1000098A

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_WSHFPDON_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_WSHFPDON_LSB         2
#define HAL_AFE_WSHFPDON_MSB         2
#define HAL_AFE_WSHFPDON_MASK        0x00000004
#define HAL_AFE_WSHFPDON_TYPE        Bool
#define HAL_AFE_WSHFPDON             HAL_AFE_WSHFPDON_MASK
#define HAL_AFE_WSHFPDON_READABLE    1
#define HAL_AFE_WSHFPDON_WRITABLE    1
#define HAL_AFE_WSHFPDON_SIGNED      0
#define HAL_AFE_WSHFPDON_ADR         0x1000098A

#define HAL_AFE_APCWSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_APCWSEL_LSB         1
#define HAL_AFE_APCWSEL_MSB         1
#define HAL_AFE_APCWSEL_MASK        0x00000002
#define HAL_AFE_APCWSEL_TYPE        Bool
#define HAL_AFE_APCWSEL             HAL_AFE_APCWSEL_MASK
#define HAL_AFE_APCWSEL_READABLE    1
#define HAL_AFE_APCWSEL_WRITABLE    1
#define HAL_AFE_APCWSEL_SIGNED      0
#define HAL_AFE_APCWSEL_ADR         0x1000098A

#define HAL_AFE_APCRSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC00)
#define HAL_AFE_APCRSEL_LSB         0
#define HAL_AFE_APCRSEL_MSB         0
#define HAL_AFE_APCRSEL_MASK        0x00000001
#define HAL_AFE_APCRSEL_TYPE        Bool
#define HAL_AFE_APCRSEL             HAL_AFE_APCRSEL_MASK
#define HAL_AFE_APCRSEL_READABLE    1
#define HAL_AFE_APCRSEL_WRITABLE    1
#define HAL_AFE_APCRSEL_SIGNED      0
#define HAL_AFE_APCRSEL_ADR         0x1000098A

// Register WOBAPC01
#define HAL_AFE_APCVREFOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC01)
#define HAL_AFE_APCVREFOS_LSB         4
#define HAL_AFE_APCVREFOS_MSB         7
#define HAL_AFE_APCVREFOS_MASK        0x000000F0
#define HAL_AFE_APCVREFOS_TYPE        halAFE_APCVref06DC_t
#define HAL_AFE_APCVREFOS             HAL_AFE_APCVREFOS_MASK
#define HAL_AFE_APCVREFOS_READABLE    1
#define HAL_AFE_APCVREFOS_WRITABLE    1
#define HAL_AFE_APCVREFOS_SIGNED      0
#define HAL_AFE_APCVREFOS_ADR         0x1000098B

#define HAL_AFE_SUBINSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC01)
#define HAL_AFE_SUBINSEL_LSB         2
#define HAL_AFE_SUBINSEL_MSB         3
#define HAL_AFE_SUBINSEL_MASK        0x0000000C
#define HAL_AFE_SUBINSEL_TYPE        halAFE_SubtBlkIN_t
#define HAL_AFE_SUBINSEL             HAL_AFE_SUBINSEL_MASK
#define HAL_AFE_SUBINSEL_READABLE    1
#define HAL_AFE_SUBINSEL_WRITABLE    1
#define HAL_AFE_SUBINSEL_SIGNED      0
#define HAL_AFE_SUBINSEL_ADR         0x1000098B

#define HAL_AFE_PDREADSUB_REGISTER    (HAL_AFEREG_MAP->WOBAPC01)
#define HAL_AFE_PDREADSUB_LSB         1
#define HAL_AFE_PDREADSUB_MSB         1
#define HAL_AFE_PDREADSUB_MASK        0x00000002
#define HAL_AFE_PDREADSUB_TYPE        Bool
#define HAL_AFE_PDREADSUB             HAL_AFE_PDREADSUB_MASK
#define HAL_AFE_PDREADSUB_READABLE    1
#define HAL_AFE_PDREADSUB_WRITABLE    1
#define HAL_AFE_PDREADSUB_SIGNED      0
#define HAL_AFE_PDREADSUB_ADR         0x1000098B

#define HAL_AFE_RSHFPDON_REGISTER    (HAL_AFEREG_MAP->WOBAPC01)
#define HAL_AFE_RSHFPDON_LSB         0
#define HAL_AFE_RSHFPDON_MSB         0
#define HAL_AFE_RSHFPDON_MASK        0x00000001
#define HAL_AFE_RSHFPDON_TYPE        Bool
#define HAL_AFE_RSHFPDON             HAL_AFE_RSHFPDON_MASK
#define HAL_AFE_RSHFPDON_READABLE    1
#define HAL_AFE_RSHFPDON_WRITABLE    1
#define HAL_AFE_RSHFPDON_SIGNED      0
#define HAL_AFE_RSHFPDON_ADR         0x1000098B

// Register WOBAPC02
#define HAL_AFE_PDRAPC_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_PDRAPC_LSB         7
#define HAL_AFE_PDRAPC_MSB         7
#define HAL_AFE_PDRAPC_MASK        0x00000080
#define HAL_AFE_PDRAPC_TYPE        Bool
#define HAL_AFE_PDRAPC             HAL_AFE_PDRAPC_MASK
#define HAL_AFE_PDRAPC_READABLE    1
#define HAL_AFE_PDRAPC_WRITABLE    1
#define HAL_AFE_PDRAPC_SIGNED      0
#define HAL_AFE_PDRAPC_ADR         0x1000098C

#define HAL_AFE_RINSSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_RINSSEL_LSB         5
#define HAL_AFE_RINSSEL_MSB         6
#define HAL_AFE_RINSSEL_MASK        0x00000060
#define HAL_AFE_RINSSEL_TYPE        halAFE_RdAmpBlkIN_t
#define HAL_AFE_RINSSEL             HAL_AFE_RINSSEL_MASK
#define HAL_AFE_RINSSEL_READABLE    1
#define HAL_AFE_RINSSEL_WRITABLE    1
#define HAL_AFE_RINSSEL_SIGNED      0
#define HAL_AFE_RINSSEL_ADR         0x1000098C

#define HAL_AFE_RDAMPCD_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_RDAMPCD_LSB         4
#define HAL_AFE_RDAMPCD_MSB         4
#define HAL_AFE_RDAMPCD_MASK        0x00000010
#define HAL_AFE_RDAMPCD_TYPE        Bool
#define HAL_AFE_RDAMPCD             HAL_AFE_RDAMPCD_MASK
#define HAL_AFE_RDAMPCD_READABLE    1
#define HAL_AFE_RDAMPCD_WRITABLE    1
#define HAL_AFE_RDAMPCD_SIGNED      0
#define HAL_AFE_RDAMPCD_ADR         0x1000098C

#define HAL_AFE_GAIN10_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_GAIN10_LSB         3
#define HAL_AFE_GAIN10_MSB         3
#define HAL_AFE_GAIN10_MASK        0x00000008
#define HAL_AFE_GAIN10_TYPE        Bool
#define HAL_AFE_GAIN10             HAL_AFE_GAIN10_MASK
#define HAL_AFE_GAIN10_READABLE    1
#define HAL_AFE_GAIN10_WRITABLE    1
#define HAL_AFE_GAIN10_SIGNED      0
#define HAL_AFE_GAIN10_ADR         0x1000098C

#define HAL_AFE_GAIN5OPEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_GAIN5OPEN_LSB         2
#define HAL_AFE_GAIN5OPEN_MSB         2
#define HAL_AFE_GAIN5OPEN_MASK        0x00000004
#define HAL_AFE_GAIN5OPEN_TYPE        Bool
#define HAL_AFE_GAIN5OPEN             HAL_AFE_GAIN5OPEN_MASK
#define HAL_AFE_GAIN5OPEN_READABLE    1
#define HAL_AFE_GAIN5OPEN_WRITABLE    1
#define HAL_AFE_GAIN5OPEN_SIGNED      0
#define HAL_AFE_GAIN5OPEN_ADR         0x1000098C

#define HAL_AFE_APCRDGN_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_APCRDGN_LSB         1
#define HAL_AFE_APCRDGN_MSB         1
#define HAL_AFE_APCRDGN_MASK        0x00000002
#define HAL_AFE_APCRDGN_TYPE        Bool
#define HAL_AFE_APCRDGN             HAL_AFE_APCRDGN_MASK
#define HAL_AFE_APCRDGN_READABLE    1
#define HAL_AFE_APCRDGN_WRITABLE    1
#define HAL_AFE_APCRDGN_SIGNED      0
#define HAL_AFE_APCRDGN_ADR         0x1000098C

#define HAL_AFE_ERASE1_REGISTER    (HAL_AFEREG_MAP->WOBAPC02)
#define HAL_AFE_ERASE1_LSB         0
#define HAL_AFE_ERASE1_MSB         0
#define HAL_AFE_ERASE1_MASK        0x00000001
#define HAL_AFE_ERASE1_TYPE        Bool
#define HAL_AFE_ERASE1             HAL_AFE_ERASE1_MASK
#define HAL_AFE_ERASE1_READABLE    1
#define HAL_AFE_ERASE1_WRITABLE    1
#define HAL_AFE_ERASE1_SIGNED      0
#define HAL_AFE_ERASE1_ADR         0x1000098C

// Register WOBAPC03
#define HAL_AFE_WINSSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC03)
#define HAL_AFE_WINSSEL_LSB         6
#define HAL_AFE_WINSSEL_MSB         7
#define HAL_AFE_WINSSEL_MASK        0x000000C0
#define HAL_AFE_WINSSEL_TYPE        halAFE_WrAmpBlkIN_t
#define HAL_AFE_WINSSEL             HAL_AFE_WINSSEL_MASK
#define HAL_AFE_WINSSEL_READABLE    1
#define HAL_AFE_WINSSEL_WRITABLE    1
#define HAL_AFE_WINSSEL_SIGNED      0
#define HAL_AFE_WINSSEL_ADR         0x1000098D

#define HAL_AFE_PDWAPC_REGISTER    (HAL_AFEREG_MAP->WOBAPC03)
#define HAL_AFE_PDWAPC_LSB         5
#define HAL_AFE_PDWAPC_MSB         5
#define HAL_AFE_PDWAPC_MASK        0x00000020
#define HAL_AFE_PDWAPC_TYPE        Bool
#define HAL_AFE_PDWAPC             HAL_AFE_PDWAPC_MASK
#define HAL_AFE_PDWAPC_READABLE    1
#define HAL_AFE_PDWAPC_WRITABLE    1
#define HAL_AFE_PDWAPC_SIGNED      0
#define HAL_AFE_PDWAPC_ADR         0x1000098D

#define HAL_AFE_FDGD2_REGISTER    (HAL_AFEREG_MAP->WOBAPC03)
#define HAL_AFE_FDGD2_LSB         0
#define HAL_AFE_FDGD2_MSB         4
#define HAL_AFE_FDGD2_MASK        0x0000001F
#define HAL_AFE_FDGD2_TYPE        UByte
#define HAL_AFE_FDGD2             HAL_AFE_FDGD2_MASK
#define HAL_AFE_FDGD2_READABLE    1
#define HAL_AFE_FDGD2_WRITABLE    1
#define HAL_AFE_FDGD2_SIGNED      0
#define HAL_AFE_FDGD2_ADR         0x1000098D

// Register WOBAPC04
#define HAL_AFE_APCTEST_REGISTER    (HAL_AFEREG_MAP->WOBAPC04)
#define HAL_AFE_APCTEST_LSB         5
#define HAL_AFE_APCTEST_MSB         7
#define HAL_AFE_APCTEST_MASK        0x000000E0
#define HAL_AFE_APCTEST_TYPE        halAFE_APCTestMUX_t
#define HAL_AFE_APCTEST             HAL_AFE_APCTEST_MASK
#define HAL_AFE_APCTEST_READABLE    1
#define HAL_AFE_APCTEST_WRITABLE    1
#define HAL_AFE_APCTEST_SIGNED      0
#define HAL_AFE_APCTEST_ADR         0x1000098E

#define HAL_AFE_FVREFSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC04)
#define HAL_AFE_FVREFSEL_LSB         3
#define HAL_AFE_FVREFSEL_MSB         4
#define HAL_AFE_FVREFSEL_MASK        0x00000018
#define HAL_AFE_FVREFSEL_TYPE        halAFE_FVREFSEL_t
#define HAL_AFE_FVREFSEL             HAL_AFE_FVREFSEL_MASK
#define HAL_AFE_FVREFSEL_READABLE    1
#define HAL_AFE_FVREFSEL_WRITABLE    1
#define HAL_AFE_FVREFSEL_SIGNED      0
#define HAL_AFE_FVREFSEL_ADR         0x1000098E

#define HAL_AFE_PDFVREF_REGISTER    (HAL_AFEREG_MAP->WOBAPC04)
#define HAL_AFE_PDFVREF_LSB         2
#define HAL_AFE_PDFVREF_MSB         2
#define HAL_AFE_PDFVREF_MASK        0x00000004
#define HAL_AFE_PDFVREF_TYPE        Bool
#define HAL_AFE_PDFVREF             HAL_AFE_PDFVREF_MASK
#define HAL_AFE_PDFVREF_READABLE    1
#define HAL_AFE_PDFVREF_WRITABLE    1
#define HAL_AFE_PDFVREF_SIGNED      0
#define HAL_AFE_PDFVREF_ADR         0x1000098E

#define HAL_AFE_PDAPCALL_REGISTER    (HAL_AFEREG_MAP->WOBAPC04)
#define HAL_AFE_PDAPCALL_LSB         1
#define HAL_AFE_PDAPCALL_MSB         1
#define HAL_AFE_PDAPCALL_MASK        0x00000002
#define HAL_AFE_PDAPCALL_TYPE        Bool
#define HAL_AFE_PDAPCALL             HAL_AFE_PDAPCALL_MASK
#define HAL_AFE_PDAPCALL_READABLE    1
#define HAL_AFE_PDAPCALL_WRITABLE    1
#define HAL_AFE_PDAPCALL_SIGNED      0
#define HAL_AFE_PDAPCALL_ADR         0x1000098E

#define HAL_AFE_PDVREF06_REGISTER    (HAL_AFEREG_MAP->WOBAPC04)
#define HAL_AFE_PDVREF06_LSB         0
#define HAL_AFE_PDVREF06_MSB         0
#define HAL_AFE_PDVREF06_MASK        0x00000001
#define HAL_AFE_PDVREF06_TYPE        Bool
#define HAL_AFE_PDVREF06             HAL_AFE_PDVREF06_MASK
#define HAL_AFE_PDVREF06_READABLE    1
#define HAL_AFE_PDVREF06_WRITABLE    1
#define HAL_AFE_PDVREF06_SIGNED      0
#define HAL_AFE_PDVREF06_ADR         0x1000098E

// Register WOBAPC05
#define HAL_AFE_RGUPREG_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_RGUPREG_LSB         7
#define HAL_AFE_RGUPREG_MSB         7
#define HAL_AFE_RGUPREG_MASK        0x00000080
#define HAL_AFE_RGUPREG_TYPE        Bool
#define HAL_AFE_RGUPREG             HAL_AFE_RGUPREG_MASK
#define HAL_AFE_RGUPREG_READABLE    1
#define HAL_AFE_RGUPREG_WRITABLE    1
#define HAL_AFE_RGUPREG_SIGNED      0
#define HAL_AFE_RGUPREG_ADR         0x1000098F

#define HAL_AFE_WBLLPEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_WBLLPEN_LSB         6
#define HAL_AFE_WBLLPEN_MSB         6
#define HAL_AFE_WBLLPEN_MASK        0x00000040
#define HAL_AFE_WBLLPEN_TYPE        Bool
#define HAL_AFE_WBLLPEN             HAL_AFE_WBLLPEN_MASK
#define HAL_AFE_WBLLPEN_READABLE    1
#define HAL_AFE_WBLLPEN_WRITABLE    1
#define HAL_AFE_WBLLPEN_SIGNED      0
#define HAL_AFE_WBLLPEN_ADR         0x1000098F

#define HAL_AFE_WOBAUXEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_WOBAUXEN_LSB         5
#define HAL_AFE_WOBAUXEN_MSB         5
#define HAL_AFE_WOBAUXEN_MASK        0x00000020
#define HAL_AFE_WOBAUXEN_TYPE        Bool
#define HAL_AFE_WOBAUXEN             HAL_AFE_WOBAUXEN_MASK
#define HAL_AFE_WOBAUXEN_READABLE    1
#define HAL_AFE_WOBAUXEN_WRITABLE    1
#define HAL_AFE_WOBAUXEN_SIGNED      0
#define HAL_AFE_WOBAUXEN_ADR         0x1000098F

#define HAL_AFE_REGNBB_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_REGNBB_LSB         4
#define HAL_AFE_REGNBB_MSB         4
#define HAL_AFE_REGNBB_MASK        0x00000010
#define HAL_AFE_REGNBB_TYPE        Bool
#define HAL_AFE_REGNBB             HAL_AFE_REGNBB_MASK
#define HAL_AFE_REGNBB_READABLE    1
#define HAL_AFE_REGNBB_WRITABLE    1
#define HAL_AFE_REGNBB_SIGNED      0
#define HAL_AFE_REGNBB_ADR         0x1000098F

#define HAL_AFE_REGNBA_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_REGNBA_LSB         3
#define HAL_AFE_REGNBA_MSB         3
#define HAL_AFE_REGNBA_MASK        0x00000008
#define HAL_AFE_REGNBA_TYPE        Bool
#define HAL_AFE_REGNBA             HAL_AFE_REGNBA_MASK
#define HAL_AFE_REGNBA_READABLE    1
#define HAL_AFE_REGNBA_WRITABLE    1
#define HAL_AFE_REGNBA_SIGNED      0
#define HAL_AFE_REGNBA_ADR         0x1000098F

#define HAL_AFE_HPMODE_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_HPMODE_LSB         2
#define HAL_AFE_HPMODE_MSB         2
#define HAL_AFE_HPMODE_MASK        0x00000004
#define HAL_AFE_HPMODE_TYPE        Bool
#define HAL_AFE_HPMODE             HAL_AFE_HPMODE_MASK
#define HAL_AFE_HPMODE_READABLE    1
#define HAL_AFE_HPMODE_WRITABLE    1
#define HAL_AFE_HPMODE_SIGNED      0
#define HAL_AFE_HPMODE_ADR         0x1000098F

#define HAL_AFE_PDINPWBL_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_PDINPWBL_LSB         1
#define HAL_AFE_PDINPWBL_MSB         1
#define HAL_AFE_PDINPWBL_MASK        0x00000002
#define HAL_AFE_PDINPWBL_TYPE        Bool
#define HAL_AFE_PDINPWBL             HAL_AFE_PDINPWBL_MASK
#define HAL_AFE_PDINPWBL_READABLE    1
#define HAL_AFE_PDINPWBL_WRITABLE    1
#define HAL_AFE_PDINPWBL_SIGNED      0
#define HAL_AFE_PDINPWBL_ADR         0x1000098F

#define HAL_AFE_PDWOBALL_REGISTER    (HAL_AFEREG_MAP->WOBAPC05)
#define HAL_AFE_PDWOBALL_LSB         0
#define HAL_AFE_PDWOBALL_MSB         0
#define HAL_AFE_PDWOBALL_MASK        0x00000001
#define HAL_AFE_PDWOBALL_TYPE        Bool
#define HAL_AFE_PDWOBALL             HAL_AFE_PDWOBALL_MASK
#define HAL_AFE_PDWOBALL_READABLE    1
#define HAL_AFE_PDWOBALL_WRITABLE    1
#define HAL_AFE_PDWOBALL_SIGNED      0
#define HAL_AFE_PDWOBALL_ADR         0x1000098F

// Register WOBAPC06
#define HAL_AFE_VREFOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC06)
#define HAL_AFE_VREFOS_LSB         0
#define HAL_AFE_VREFOS_MSB         7
#define HAL_AFE_VREFOS_MASK        0x000000FF
#define HAL_AFE_VREFOS_TYPE        UByte
#define HAL_AFE_VREFOS             HAL_AFE_VREFOS_MASK
#define HAL_AFE_VREFOS_READABLE    1
#define HAL_AFE_VREFOS_WRITABLE    1
#define HAL_AFE_VREFOS_SIGNED      0
#define HAL_AFE_VREFOS_ADR         0x10000990

// Register WOBAPC07

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_RSHSEL_LSB         6
#define HAL_AFE_RSHSEL_MSB         7
#define HAL_AFE_RSHSEL_MASK        0x000000C0
#define HAL_AFE_RSHSEL_TYPE        halAFE_RdWblSH_t
#define HAL_AFE_RSHSEL             HAL_AFE_RSHSEL_MASK
#define HAL_AFE_RSHSEL_READABLE    1
#define HAL_AFE_RSHSEL_WRITABLE    1
#define HAL_AFE_RSHSEL_SIGNED      0
#define HAL_AFE_RSHSEL_ADR         0x10000991

#define HAL_AFE_RGUPSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_RGUPSEL_LSB         4
#define HAL_AFE_RGUPSEL_MSB         5
#define HAL_AFE_RGUPSEL_MASK        0x00000030
#define HAL_AFE_RGUPSEL_TYPE        halAFE_RGUPSEL_t
#define HAL_AFE_RGUPSEL             HAL_AFE_RGUPSEL_MASK
#define HAL_AFE_RGUPSEL_READABLE    1
#define HAL_AFE_RGUPSEL_WRITABLE    1
#define HAL_AFE_RGUPSEL_SIGNED      0
#define HAL_AFE_RGUPSEL_ADR         0x10000991

#define HAL_AFE_WOBINSEL3_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WOBINSEL3_LSB         3
#define HAL_AFE_WOBINSEL3_MSB         3
#define HAL_AFE_WOBINSEL3_MASK        0x00000008
#define HAL_AFE_WOBINSEL3_TYPE        Bool
#define HAL_AFE_WOBINSEL3             HAL_AFE_WOBINSEL3_MASK
#define HAL_AFE_WOBINSEL3_READABLE    1
#define HAL_AFE_WOBINSEL3_WRITABLE    1
#define HAL_AFE_WOBINSEL3_SIGNED      0
#define HAL_AFE_WOBINSEL3_ADR         0x10000991

#define HAL_AFE_WOBINSEL2_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WOBINSEL2_LSB         2
#define HAL_AFE_WOBINSEL2_MSB         2
#define HAL_AFE_WOBINSEL2_MASK        0x00000004
#define HAL_AFE_WOBINSEL2_TYPE        Bool
#define HAL_AFE_WOBINSEL2             HAL_AFE_WOBINSEL2_MASK
#define HAL_AFE_WOBINSEL2_READABLE    1
#define HAL_AFE_WOBINSEL2_WRITABLE    1
#define HAL_AFE_WOBINSEL2_SIGNED      0
#define HAL_AFE_WOBINSEL2_ADR         0x10000991

#define HAL_AFE_WOBINSEL1_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WOBINSEL1_LSB         1
#define HAL_AFE_WOBINSEL1_MSB         1
#define HAL_AFE_WOBINSEL1_MASK        0x00000002
#define HAL_AFE_WOBINSEL1_TYPE        Bool
#define HAL_AFE_WOBINSEL1             HAL_AFE_WOBINSEL1_MASK
#define HAL_AFE_WOBINSEL1_READABLE    1
#define HAL_AFE_WOBINSEL1_WRITABLE    1
#define HAL_AFE_WOBINSEL1_SIGNED      0
#define HAL_AFE_WOBINSEL1_ADR         0x10000991

#define HAL_AFE_WOBINSEL0_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WOBINSEL0_LSB         0
#define HAL_AFE_WOBINSEL0_MSB         0
#define HAL_AFE_WOBINSEL0_MASK        0x00000001
#define HAL_AFE_WOBINSEL0_TYPE        Bool
#define HAL_AFE_WOBINSEL0             HAL_AFE_WOBINSEL0_MASK
#define HAL_AFE_WOBINSEL0_READABLE    1
#define HAL_AFE_WOBINSEL0_WRITABLE    1
#define HAL_AFE_WOBINSEL0_SIGNED      0
#define HAL_AFE_WOBINSEL0_ADR         0x10000991

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_RSHSEL_LSB         6
#define HAL_AFE_RSHSEL_MSB         7
#define HAL_AFE_RSHSEL_MASK        0x000000C0
#define HAL_AFE_RSHSEL_TYPE        halAFE_WblRdSH_t
#define HAL_AFE_RSHSEL             HAL_AFE_RSHSEL_MASK
#define HAL_AFE_RSHSEL_READABLE    1
#define HAL_AFE_RSHSEL_WRITABLE    1
#define HAL_AFE_RSHSEL_SIGNED      0
#define HAL_AFE_RSHSEL_ADR         0x10000991

#define HAL_AFE_RGUPSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_RGUPSEL_LSB         4
#define HAL_AFE_RGUPSEL_MSB         5
#define HAL_AFE_RGUPSEL_MASK        0x00000030
#define HAL_AFE_RGUPSEL_TYPE        halAFE_RGUPSEL_t
#define HAL_AFE_RGUPSEL             HAL_AFE_RGUPSEL_MASK
#define HAL_AFE_RGUPSEL_READABLE    1
#define HAL_AFE_RGUPSEL_WRITABLE    1
#define HAL_AFE_RGUPSEL_SIGNED      0
#define HAL_AFE_RGUPSEL_ADR         0x10000991

#define HAL_AFE_WBLDIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WBLDIN_LSB         3
#define HAL_AFE_WBLDIN_MSB         3
#define HAL_AFE_WBLDIN_MASK        0x00000008
#define HAL_AFE_WBLDIN_TYPE        Bool
#define HAL_AFE_WBLDIN             HAL_AFE_WBLDIN_MASK
#define HAL_AFE_WBLDIN_READABLE    1
#define HAL_AFE_WBLDIN_WRITABLE    1
#define HAL_AFE_WBLDIN_SIGNED      0
#define HAL_AFE_WBLDIN_ADR         0x10000991

#define HAL_AFE_WBLCIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WBLCIN_LSB         2
#define HAL_AFE_WBLCIN_MSB         2
#define HAL_AFE_WBLCIN_MASK        0x00000004
#define HAL_AFE_WBLCIN_TYPE        Bool
#define HAL_AFE_WBLCIN             HAL_AFE_WBLCIN_MASK
#define HAL_AFE_WBLCIN_READABLE    1
#define HAL_AFE_WBLCIN_WRITABLE    1
#define HAL_AFE_WBLCIN_SIGNED      0
#define HAL_AFE_WBLCIN_ADR         0x10000991

#define HAL_AFE_WBLBIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WBLBIN_LSB         1
#define HAL_AFE_WBLBIN_MSB         1
#define HAL_AFE_WBLBIN_MASK        0x00000002
#define HAL_AFE_WBLBIN_TYPE        Bool
#define HAL_AFE_WBLBIN             HAL_AFE_WBLBIN_MASK
#define HAL_AFE_WBLBIN_READABLE    1
#define HAL_AFE_WBLBIN_WRITABLE    1
#define HAL_AFE_WBLBIN_SIGNED      0
#define HAL_AFE_WBLBIN_ADR         0x10000991

#define HAL_AFE_WBLAIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC07)
#define HAL_AFE_WBLAIN_LSB         0
#define HAL_AFE_WBLAIN_MSB         0
#define HAL_AFE_WBLAIN_MASK        0x00000001
#define HAL_AFE_WBLAIN_TYPE        Bool
#define HAL_AFE_WBLAIN             HAL_AFE_WBLAIN_MASK
#define HAL_AFE_WBLAIN_READABLE    1
#define HAL_AFE_WBLAIN_WRITABLE    1
#define HAL_AFE_WBLAIN_SIGNED      0
#define HAL_AFE_WBLAIN_ADR         0x10000991

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC08

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RADOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC08)
#define HAL_AFE_RADOS_LSB         0
#define HAL_AFE_RADOS_MSB         7
#define HAL_AFE_RADOS_MASK        0x000000FF
#define HAL_AFE_RADOS_TYPE        UByte
#define HAL_AFE_RADOS             HAL_AFE_RADOS_MASK
#define HAL_AFE_RADOS_READABLE    1
#define HAL_AFE_RADOS_WRITABLE    1
#define HAL_AFE_RADOS_SIGNED      0
#define HAL_AFE_RADOS_ADR         0x10000992

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RADOSA_REGISTER    (HAL_AFEREG_MAP->WOBAPC08)
#define HAL_AFE_RADOSA_LSB         0
#define HAL_AFE_RADOSA_MSB         7
#define HAL_AFE_RADOSA_MASK        0x000000FF
#define HAL_AFE_RADOSA_TYPE        UByte
#define HAL_AFE_RADOSA             HAL_AFE_RADOSA_MASK
#define HAL_AFE_RADOSA_READABLE    1
#define HAL_AFE_RADOSA_WRITABLE    1
#define HAL_AFE_RADOSA_SIGNED      0
#define HAL_AFE_RADOSA_ADR         0x10000992

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC09
#define HAL_AFE_WADGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC09)
#define HAL_AFE_WADGNB_LSB         6
#define HAL_AFE_WADGNB_MSB         7
#define HAL_AFE_WADGNB_MASK        0x000000C0
#define HAL_AFE_WADGNB_TYPE        halAFE_WblWrADGN1_t
#define HAL_AFE_WADGNB             HAL_AFE_WADGNB_MASK
#define HAL_AFE_WADGNB_READABLE    1
#define HAL_AFE_WADGNB_WRITABLE    1
#define HAL_AFE_WADGNB_SIGNED      0
#define HAL_AFE_WADGNB_ADR         0x10000993

#define HAL_AFE_WADGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC09)
#define HAL_AFE_WADGNA_LSB         4
#define HAL_AFE_WADGNA_MSB         5
#define HAL_AFE_WADGNA_MASK        0x00000030
#define HAL_AFE_WADGNA_TYPE        halAFE_WblWrADGN0_t
#define HAL_AFE_WADGNA             HAL_AFE_WADGNA_MASK
#define HAL_AFE_WADGNA_READABLE    1
#define HAL_AFE_WADGNA_WRITABLE    1
#define HAL_AFE_WADGNA_SIGNED      0
#define HAL_AFE_WADGNA_ADR         0x10000993

#define HAL_AFE_RADPGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC09)
#define HAL_AFE_RADPGNB_LSB         2
#define HAL_AFE_RADPGNB_MSB         3
#define HAL_AFE_RADPGNB_MASK        0x0000000C
#define HAL_AFE_RADPGNB_TYPE        halAFE_WblRdADPGN1_t
#define HAL_AFE_RADPGNB             HAL_AFE_RADPGNB_MASK
#define HAL_AFE_RADPGNB_READABLE    1
#define HAL_AFE_RADPGNB_WRITABLE    1
#define HAL_AFE_RADPGNB_SIGNED      0
#define HAL_AFE_RADPGNB_ADR         0x10000993

#define HAL_AFE_RADPGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC09)
#define HAL_AFE_RADPGNA_LSB         0
#define HAL_AFE_RADPGNA_MSB         1
#define HAL_AFE_RADPGNA_MASK        0x00000003
#define HAL_AFE_RADPGNA_TYPE        halAFE_WblRdADPGN0_t
#define HAL_AFE_RADPGNA             HAL_AFE_RADPGNA_MASK
#define HAL_AFE_RADPGNA_READABLE    1
#define HAL_AFE_RADPGNA_WRITABLE    1
#define HAL_AFE_RADPGNA_SIGNED      0
#define HAL_AFE_RADPGNA_ADR         0x10000993

// Register WOBAPC10
#define HAL_AFE_WBCGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC10)
#define HAL_AFE_WBCGNB_LSB         6
#define HAL_AFE_WBCGNB_MSB         7
#define HAL_AFE_WBCGNB_MASK        0x000000C0
#define HAL_AFE_WBCGNB_TYPE        halAFE_WblWrBCGN1_t
#define HAL_AFE_WBCGNB             HAL_AFE_WBCGNB_MASK
#define HAL_AFE_WBCGNB_READABLE    1
#define HAL_AFE_WBCGNB_WRITABLE    1
#define HAL_AFE_WBCGNB_SIGNED      0
#define HAL_AFE_WBCGNB_ADR         0x10000994

#define HAL_AFE_WBCGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC10)
#define HAL_AFE_WBCGNA_LSB         4
#define HAL_AFE_WBCGNA_MSB         5
#define HAL_AFE_WBCGNA_MASK        0x00000030
#define HAL_AFE_WBCGNA_TYPE        halAFE_WblWrBCGN0_t
#define HAL_AFE_WBCGNA             HAL_AFE_WBCGNA_MASK
#define HAL_AFE_WBCGNA_READABLE    1
#define HAL_AFE_WBCGNA_WRITABLE    1
#define HAL_AFE_WBCGNA_SIGNED      0
#define HAL_AFE_WBCGNA_ADR         0x10000994

#define HAL_AFE_RBCPGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC10)
#define HAL_AFE_RBCPGNB_LSB         2
#define HAL_AFE_RBCPGNB_MSB         3
#define HAL_AFE_RBCPGNB_MASK        0x0000000C
#define HAL_AFE_RBCPGNB_TYPE        halAFE_WblRdBCPGN1_t
#define HAL_AFE_RBCPGNB             HAL_AFE_RBCPGNB_MASK
#define HAL_AFE_RBCPGNB_READABLE    1
#define HAL_AFE_RBCPGNB_WRITABLE    1
#define HAL_AFE_RBCPGNB_SIGNED      0
#define HAL_AFE_RBCPGNB_ADR         0x10000994

#define HAL_AFE_RBCPGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC10)
#define HAL_AFE_RBCPGNA_LSB         0
#define HAL_AFE_RBCPGNA_MSB         1
#define HAL_AFE_RBCPGNA_MASK        0x00000003
#define HAL_AFE_RBCPGNA_TYPE        halAFE_WblRdBCPGN0_t
#define HAL_AFE_RBCPGNA             HAL_AFE_RBCPGNA_MASK
#define HAL_AFE_RBCPGNA_READABLE    1
#define HAL_AFE_RBCPGNA_WRITABLE    1
#define HAL_AFE_RBCPGNA_SIGNED      0
#define HAL_AFE_RBCPGNA_ADR         0x10000994

// Register WOBAPC11
#define HAL_AFE_RADGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC11)
#define HAL_AFE_RADGNB_LSB         6
#define HAL_AFE_RADGNB_MSB         7
#define HAL_AFE_RADGNB_MASK        0x000000C0
#define HAL_AFE_RADGNB_TYPE        halAFE_WblRdADGN1_t
#define HAL_AFE_RADGNB             HAL_AFE_RADGNB_MASK
#define HAL_AFE_RADGNB_READABLE    1
#define HAL_AFE_RADGNB_WRITABLE    1
#define HAL_AFE_RADGNB_SIGNED      0
#define HAL_AFE_RADGNB_ADR         0x10000995

#define HAL_AFE_RADGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC11)
#define HAL_AFE_RADGNA_LSB         4
#define HAL_AFE_RADGNA_MSB         5
#define HAL_AFE_RADGNA_MASK        0x00000030
#define HAL_AFE_RADGNA_TYPE        halAFE_WblRdADGN0_t
#define HAL_AFE_RADGNA             HAL_AFE_RADGNA_MASK
#define HAL_AFE_RADGNA_READABLE    1
#define HAL_AFE_RADGNA_WRITABLE    1
#define HAL_AFE_RADGNA_SIGNED      0
#define HAL_AFE_RADGNA_ADR         0x10000995

#define HAL_AFE_WADPGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC11)
#define HAL_AFE_WADPGNB_LSB         2
#define HAL_AFE_WADPGNB_MSB         3
#define HAL_AFE_WADPGNB_MASK        0x0000000C
#define HAL_AFE_WADPGNB_TYPE        halAFE_WblWrADPGNB_t
#define HAL_AFE_WADPGNB             HAL_AFE_WADPGNB_MASK
#define HAL_AFE_WADPGNB_READABLE    1
#define HAL_AFE_WADPGNB_WRITABLE    1
#define HAL_AFE_WADPGNB_SIGNED      0
#define HAL_AFE_WADPGNB_ADR         0x10000995

#define HAL_AFE_WADPGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC11)
#define HAL_AFE_WADPGNA_LSB         0
#define HAL_AFE_WADPGNA_MSB         1
#define HAL_AFE_WADPGNA_MASK        0x00000003
#define HAL_AFE_WADPGNA_TYPE        halAFE_WblWrADPGNA_t
#define HAL_AFE_WADPGNA             HAL_AFE_WADPGNA_MASK
#define HAL_AFE_WADPGNA_READABLE    1
#define HAL_AFE_WADPGNA_WRITABLE    1
#define HAL_AFE_WADPGNA_SIGNED      0
#define HAL_AFE_WADPGNA_ADR         0x10000995

// Register WOBAPC12
#define HAL_AFE_RBCGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC12)
#define HAL_AFE_RBCGNB_LSB         6
#define HAL_AFE_RBCGNB_MSB         7
#define HAL_AFE_RBCGNB_MASK        0x000000C0
#define HAL_AFE_RBCGNB_TYPE        halAFE_WblRdBCGN1_t
#define HAL_AFE_RBCGNB             HAL_AFE_RBCGNB_MASK
#define HAL_AFE_RBCGNB_READABLE    1
#define HAL_AFE_RBCGNB_WRITABLE    1
#define HAL_AFE_RBCGNB_SIGNED      0
#define HAL_AFE_RBCGNB_ADR         0x10000996

#define HAL_AFE_RBCGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC12)
#define HAL_AFE_RBCGNA_LSB         4
#define HAL_AFE_RBCGNA_MSB         5
#define HAL_AFE_RBCGNA_MASK        0x00000030
#define HAL_AFE_RBCGNA_TYPE        halAFE_WblRdBCGN0_t
#define HAL_AFE_RBCGNA             HAL_AFE_RBCGNA_MASK
#define HAL_AFE_RBCGNA_READABLE    1
#define HAL_AFE_RBCGNA_WRITABLE    1
#define HAL_AFE_RBCGNA_SIGNED      0
#define HAL_AFE_RBCGNA_ADR         0x10000996

#define HAL_AFE_WBCPGNB_REGISTER    (HAL_AFEREG_MAP->WOBAPC12)
#define HAL_AFE_WBCPGNB_LSB         2
#define HAL_AFE_WBCPGNB_MSB         3
#define HAL_AFE_WBCPGNB_MASK        0x0000000C
#define HAL_AFE_WBCPGNB_TYPE        halAFE_WblWrBCPGNB_t
#define HAL_AFE_WBCPGNB             HAL_AFE_WBCPGNB_MASK
#define HAL_AFE_WBCPGNB_READABLE    1
#define HAL_AFE_WBCPGNB_WRITABLE    1
#define HAL_AFE_WBCPGNB_SIGNED      0
#define HAL_AFE_WBCPGNB_ADR         0x10000996

#define HAL_AFE_WBCPGNA_REGISTER    (HAL_AFEREG_MAP->WOBAPC12)
#define HAL_AFE_WBCPGNA_LSB         0
#define HAL_AFE_WBCPGNA_MSB         1
#define HAL_AFE_WBCPGNA_MASK        0x00000003
#define HAL_AFE_WBCPGNA_TYPE        halAFE_WblWrBCPGNA_t
#define HAL_AFE_WBCPGNA             HAL_AFE_WBCPGNA_MASK
#define HAL_AFE_WBCPGNA_READABLE    1
#define HAL_AFE_WBCPGNA_WRITABLE    1
#define HAL_AFE_WBCPGNA_SIGNED      0
#define HAL_AFE_WBCPGNA_ADR         0x10000996

// Register WOBAPC13
#define HAL_AFE_WGUPSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_WGUPSEL_LSB         6
#define HAL_AFE_WGUPSEL_MSB         7
#define HAL_AFE_WGUPSEL_MASK        0x000000C0
#define HAL_AFE_WGUPSEL_TYPE        halAFE_WGUPSEL_t
#define HAL_AFE_WGUPSEL             HAL_AFE_WGUPSEL_MASK
#define HAL_AFE_WGUPSEL_READABLE    1
#define HAL_AFE_WGUPSEL_WRITABLE    1
#define HAL_AFE_WGUPSEL_SIGNED      0
#define HAL_AFE_WGUPSEL_ADR         0x10000997

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_WBLWSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_WBLWSHSEL_LSB         4
#define HAL_AFE_WBLWSHSEL_MSB         5
#define HAL_AFE_WBLWSHSEL_MASK        0x00000030
#define HAL_AFE_WBLWSHSEL_TYPE        halAFE_WBLWSH2SEL_t
#define HAL_AFE_WBLWSHSEL             HAL_AFE_WBLWSHSEL_MASK
#define HAL_AFE_WBLWSHSEL_READABLE    1
#define HAL_AFE_WBLWSHSEL_WRITABLE    1
#define HAL_AFE_WBLWSHSEL_SIGNED      0
#define HAL_AFE_WBLWSHSEL_ADR         0x10000997

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WBLWSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_WBLWSHSEL_LSB         4
#define HAL_AFE_WBLWSHSEL_MSB         5
#define HAL_AFE_WBLWSHSEL_MASK        0x00000030
#define HAL_AFE_WBLWSHSEL_TYPE        halAFE_WBLWSH2source_t
#define HAL_AFE_WBLWSHSEL             HAL_AFE_WBLWSHSEL_MASK
#define HAL_AFE_WBLWSHSEL_READABLE    1
#define HAL_AFE_WBLWSHSEL_WRITABLE    1
#define HAL_AFE_WBLWSHSEL_SIGNED      0
#define HAL_AFE_WBLWSHSEL_ADR         0x10000997


#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_PDREADWOB_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_PDREADWOB_LSB         3
#define HAL_AFE_PDREADWOB_MSB         3
#define HAL_AFE_PDREADWOB_MASK        0x00000008
#define HAL_AFE_PDREADWOB_TYPE        Bool
#define HAL_AFE_PDREADWOB             HAL_AFE_PDREADWOB_MASK
#define HAL_AFE_PDREADWOB_READABLE    1
#define HAL_AFE_PDREADWOB_WRITABLE    1
#define HAL_AFE_PDREADWOB_SIGNED      0
#define HAL_AFE_PDREADWOB_ADR         0x10000997

#define HAL_AFE_PDWRTWOB_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_PDWRTWOB_LSB         2
#define HAL_AFE_PDWRTWOB_MSB         2
#define HAL_AFE_PDWRTWOB_MASK        0x00000004
#define HAL_AFE_PDWRTWOB_TYPE        Bool
#define HAL_AFE_PDWRTWOB             HAL_AFE_PDWRTWOB_MASK
#define HAL_AFE_PDWRTWOB_READABLE    1
#define HAL_AFE_PDWRTWOB_WRITABLE    1
#define HAL_AFE_PDWRTWOB_SIGNED      0
#define HAL_AFE_PDWRTWOB_ADR         0x10000997

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_WOBDIGTPSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_WOBDIGTPSEL_LSB         0
#define HAL_AFE_WOBDIGTPSEL_MSB         1
#define HAL_AFE_WOBDIGTPSEL_MASK        0x00000003
#define HAL_AFE_WOBDIGTPSEL_TYPE        halAFE_WOBDIGTPSEL_t
#define HAL_AFE_WOBDIGTPSEL             HAL_AFE_WOBDIGTPSEL_MASK
#define HAL_AFE_WOBDIGTPSEL_READABLE    1
#define HAL_AFE_WOBDIGTPSEL_WRITABLE    1
#define HAL_AFE_WOBDIGTPSEL_SIGNED      0
#define HAL_AFE_WOBDIGTPSEL_ADR         0x10000997

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WOBDTPSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC13)
#define HAL_AFE_WOBDTPSEL_LSB         0
#define HAL_AFE_WOBDTPSEL_MSB         1
#define HAL_AFE_WOBDTPSEL_MASK        0x00000003
#define HAL_AFE_WOBDTPSEL_TYPE        halAFE_WOBDTPSEL_t
#define HAL_AFE_WOBDTPSEL             HAL_AFE_WOBDTPSEL_MASK
#define HAL_AFE_WOBDTPSEL_READABLE    1
#define HAL_AFE_WOBDTPSEL_WRITABLE    1
#define HAL_AFE_WOBDTPSEL_SIGNED      0
#define HAL_AFE_WOBDTPSEL_ADR         0x10000997

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC14
#define HAL_AFE_WBLROUT_REGISTER    (HAL_AFEREG_MAP->WOBAPC14)
#define HAL_AFE_WBLROUT_LSB         5
#define HAL_AFE_WBLROUT_MSB         7
#define HAL_AFE_WBLROUT_MASK        0x000000E0
#define HAL_AFE_WBLROUT_TYPE        halAFE_WBLROUT_t
#define HAL_AFE_WBLROUT             HAL_AFE_WBLROUT_MASK
#define HAL_AFE_WBLROUT_READABLE    1
#define HAL_AFE_WBLROUT_WRITABLE    1
#define HAL_AFE_WBLROUT_SIGNED      0
#define HAL_AFE_WBLROUT_ADR         0x10000998

#define HAL_AFE_WSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC14)
#define HAL_AFE_WSHSEL_LSB         3
#define HAL_AFE_WSHSEL_MSB         4
#define HAL_AFE_WSHSEL_MASK        0x00000018
#define HAL_AFE_WSHSEL_TYPE        halAFE_WblWrSH_t
#define HAL_AFE_WSHSEL             HAL_AFE_WSHSEL_MASK
#define HAL_AFE_WSHSEL_READABLE    1
#define HAL_AFE_WSHSEL_WRITABLE    1
#define HAL_AFE_WSHSEL_SIGNED      0
#define HAL_AFE_WSHSEL_ADR         0x10000998

#define HAL_AFE_WOBSRCSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC14)
#define HAL_AFE_WOBSRCSEL_LSB         1
#define HAL_AFE_WOBSRCSEL_MSB         2
#define HAL_AFE_WOBSRCSEL_MASK        0x00000006
#define HAL_AFE_WOBSRCSEL_TYPE        halAFE_WblSource_t
#define HAL_AFE_WOBSRCSEL             HAL_AFE_WOBSRCSEL_MASK
#define HAL_AFE_WOBSRCSEL_READABLE    1
#define HAL_AFE_WOBSRCSEL_WRITABLE    1
#define HAL_AFE_WOBSRCSEL_SIGNED      0
#define HAL_AFE_WOBSRCSEL_ADR         0x10000998

#define HAL_AFE_WGUPREG_REGISTER    (HAL_AFEREG_MAP->WOBAPC14)
#define HAL_AFE_WGUPREG_LSB         0
#define HAL_AFE_WGUPREG_MSB         0
#define HAL_AFE_WGUPREG_MASK        0x00000001
#define HAL_AFE_WGUPREG_TYPE        Bool
#define HAL_AFE_WGUPREG             HAL_AFE_WGUPREG_MASK
#define HAL_AFE_WGUPREG_READABLE    1
#define HAL_AFE_WGUPREG_WRITABLE    1
#define HAL_AFE_WGUPREG_SIGNED      0
#define HAL_AFE_WGUPREG_ADR         0x10000998

// Register WOBAPC15

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_WBLRSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC15)
#define HAL_AFE_WBLRSHSEL_LSB         6
#define HAL_AFE_WBLRSHSEL_MSB         7
#define HAL_AFE_WBLRSHSEL_MASK        0x000000C0
#define HAL_AFE_WBLRSHSEL_TYPE        halAFE_WBLRSH2IN_t
#define HAL_AFE_WBLRSHSEL             HAL_AFE_WBLRSHSEL_MASK
#define HAL_AFE_WBLRSHSEL_READABLE    1
#define HAL_AFE_WBLRSHSEL_WRITABLE    1
#define HAL_AFE_WBLRSHSEL_SIGNED      0
#define HAL_AFE_WBLRSHSEL_ADR         0x10000999

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WBLRSHSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC15)
#define HAL_AFE_WBLRSHSEL_LSB         6
#define HAL_AFE_WBLRSHSEL_MSB         7
#define HAL_AFE_WBLRSHSEL_MASK        0x000000C0
#define HAL_AFE_WBLRSHSEL_TYPE        halAFE_WBLRSH2source_t
#define HAL_AFE_WBLRSHSEL             HAL_AFE_WBLRSHSEL_MASK
#define HAL_AFE_WBLRSHSEL_READABLE    1
#define HAL_AFE_WBLRSHSEL_WRITABLE    1
#define HAL_AFE_WBLRSHSEL_SIGNED      0
#define HAL_AFE_WBLRSHSEL_ADR         0x10000999

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_WBLLP_REGISTER    (HAL_AFEREG_MAP->WOBAPC15)
#define HAL_AFE_WBLLP_LSB         3
#define HAL_AFE_WBLLP_MSB         5
#define HAL_AFE_WBLLP_MASK        0x00000038
#define HAL_AFE_WBLLP_TYPE        halAFE_WblLPF_t
#define HAL_AFE_WBLLP             HAL_AFE_WBLLP_MASK
#define HAL_AFE_WBLLP_READABLE    1
#define HAL_AFE_WBLLP_WRITABLE    1
#define HAL_AFE_WBLLP_SIGNED      0
#define HAL_AFE_WBLLP_ADR         0x10000999

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_WBLWOUTS_REGISTER    (HAL_AFEREG_MAP->WOBAPC15)
#define HAL_AFE_WBLWOUTS_LSB         0
#define HAL_AFE_WBLWOUTS_MSB         2
#define HAL_AFE_WBLWOUTS_MASK        0x00000007
#define HAL_AFE_WBLWOUTS_TYPE        halAFE_WblWrOut_t
#define HAL_AFE_WBLWOUTS             HAL_AFE_WBLWOUTS_MASK
#define HAL_AFE_WBLWOUTS_READABLE    1
#define HAL_AFE_WBLWOUTS_WRITABLE    1
#define HAL_AFE_WBLWOUTS_SIGNED      0
#define HAL_AFE_WBLWOUTS_ADR         0x10000999

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WBLWOUT_REGISTER    (HAL_AFEREG_MAP->WOBAPC15)
#define HAL_AFE_WBLWOUT_LSB         0
#define HAL_AFE_WBLWOUT_MSB         2
#define HAL_AFE_WBLWOUT_MASK        0x00000007
#define HAL_AFE_WBLWOUT_TYPE        halAFE_WBLWOUT_t
#define HAL_AFE_WBLWOUT             HAL_AFE_WBLWOUT_MASK
#define HAL_AFE_WBLWOUT_READABLE    1
#define HAL_AFE_WBLWOUT_WRITABLE    1
#define HAL_AFE_WBLWOUT_SIGNED      0
#define HAL_AFE_WBLWOUT_ADR         0x10000999

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC16

#if (CHIP_REV <= 0xB3)

#define HAL_AFE_RBCOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC16)
#define HAL_AFE_RBCOS_LSB         0
#define HAL_AFE_RBCOS_MSB         7
#define HAL_AFE_RBCOS_MASK        0x000000FF
#define HAL_AFE_RBCOS_TYPE        UByte
#define HAL_AFE_RBCOS             HAL_AFE_RBCOS_MASK
#define HAL_AFE_RBCOS_READABLE    1
#define HAL_AFE_RBCOS_WRITABLE    1
#define HAL_AFE_RBCOS_SIGNED      0
#define HAL_AFE_RBCOS_ADR         0x1000099A

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RBCOSA_REGISTER    (HAL_AFEREG_MAP->WOBAPC16)
#define HAL_AFE_RBCOSA_LSB         0
#define HAL_AFE_RBCOSA_MSB         7
#define HAL_AFE_RBCOSA_MASK        0x000000FF
#define HAL_AFE_RBCOSA_TYPE        UByte
#define HAL_AFE_RBCOSA             HAL_AFE_RBCOSA_MASK
#define HAL_AFE_RBCOSA_READABLE    1
#define HAL_AFE_RBCOSA_WRITABLE    1
#define HAL_AFE_RBCOSA_SIGNED      0
#define HAL_AFE_RBCOSA_ADR         0x1000099A

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC17
#define HAL_AFE_WADOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC17)
#define HAL_AFE_WADOS_LSB         0
#define HAL_AFE_WADOS_MSB         7
#define HAL_AFE_WADOS_MASK        0x000000FF
#define HAL_AFE_WADOS_TYPE        UByte
#define HAL_AFE_WADOS             HAL_AFE_WADOS_MASK
#define HAL_AFE_WADOS_READABLE    1
#define HAL_AFE_WADOS_WRITABLE    1
#define HAL_AFE_WADOS_SIGNED      0
#define HAL_AFE_WADOS_ADR         0x1000099B

// Register WOBAPC18
#define HAL_AFE_WBCOS_REGISTER    (HAL_AFEREG_MAP->WOBAPC18)
#define HAL_AFE_WBCOS_LSB         0
#define HAL_AFE_WBCOS_MSB         7
#define HAL_AFE_WBCOS_MASK        0x000000FF
#define HAL_AFE_WBCOS_TYPE        UByte
#define HAL_AFE_WBCOS             HAL_AFE_WBCOS_MASK
#define HAL_AFE_WBCOS_READABLE    1
#define HAL_AFE_WBCOS_WRITABLE    1
#define HAL_AFE_WBCOS_SIGNED      0
#define HAL_AFE_WBCOS_ADR         0x1000099C

// Register WOBAPC19
#define HAL_AFE_AGCHGAIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC19)
#define HAL_AFE_AGCHGAIN_LSB         7
#define HAL_AFE_AGCHGAIN_MSB         7
#define HAL_AFE_AGCHGAIN_MASK        0x00000080
#define HAL_AFE_AGCHGAIN_TYPE        Bool
#define HAL_AFE_AGCHGAIN             HAL_AFE_AGCHGAIN_MASK
#define HAL_AFE_AGCHGAIN_READABLE    1
#define HAL_AFE_AGCHGAIN_WRITABLE    1
#define HAL_AFE_AGCHGAIN_SIGNED      0
#define HAL_AFE_AGCHGAIN_ADR         0x1000099D

#define HAL_AFE_AGCBPHPF_REGISTER    (HAL_AFEREG_MAP->WOBAPC19)
#define HAL_AFE_AGCBPHPF_LSB         6
#define HAL_AFE_AGCBPHPF_MSB         6
#define HAL_AFE_AGCBPHPF_MASK        0x00000040
#define HAL_AFE_AGCBPHPF_TYPE        Bool
#define HAL_AFE_AGCBPHPF             HAL_AFE_AGCBPHPF_MASK
#define HAL_AFE_AGCBPHPF_READABLE    1
#define HAL_AFE_AGCBPHPF_WRITABLE    1
#define HAL_AFE_AGCBPHPF_SIGNED      0
#define HAL_AFE_AGCBPHPF_ADR         0x1000099D

#define HAL_AFE_WOBVGA3GU_REGISTER    (HAL_AFEREG_MAP->WOBAPC19)
#define HAL_AFE_WOBVGA3GU_LSB         5
#define HAL_AFE_WOBVGA3GU_MSB         5
#define HAL_AFE_WOBVGA3GU_MASK        0x00000020
#define HAL_AFE_WOBVGA3GU_TYPE        Bool
#define HAL_AFE_WOBVGA3GU             HAL_AFE_WOBVGA3GU_MASK
#define HAL_AFE_WOBVGA3GU_READABLE    1
#define HAL_AFE_WOBVGA3GU_WRITABLE    1
#define HAL_AFE_WOBVGA3GU_SIGNED      0
#define HAL_AFE_WOBVGA3GU_ADR         0x1000099D

#define HAL_AFE_WOBLPFEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC19)
#define HAL_AFE_WOBLPFEN_LSB         4
#define HAL_AFE_WOBLPFEN_MSB         4
#define HAL_AFE_WOBLPFEN_MASK        0x00000010
#define HAL_AFE_WOBLPFEN_TYPE        Bool
#define HAL_AFE_WOBLPFEN             HAL_AFE_WOBLPFEN_MASK
#define HAL_AFE_WOBLPFEN_READABLE    1
#define HAL_AFE_WOBLPFEN_WRITABLE    1
#define HAL_AFE_WOBLPFEN_SIGNED      0
#define HAL_AFE_WOBLPFEN_ADR         0x1000099D

#define HAL_AFE_WOBLPFBW_REGISTER    (HAL_AFEREG_MAP->WOBAPC19)
#define HAL_AFE_WOBLPFBW_LSB         0
#define HAL_AFE_WOBLPFBW_MSB         3
#define HAL_AFE_WOBLPFBW_MASK        0x0000000F
#define HAL_AFE_WOBLPFBW_TYPE        halAFE_FirstLPFBefADCBW_t
#define HAL_AFE_WOBLPFBW             HAL_AFE_WOBLPFBW_MASK
#define HAL_AFE_WOBLPFBW_READABLE    1
#define HAL_AFE_WOBLPFBW_WRITABLE    1
#define HAL_AFE_WOBLPFBW_SIGNED      0
#define HAL_AFE_WOBLPFBW_ADR         0x1000099D

// Register WOBAPC20
#define HAL_AFE_AGCHPFFC_REGISTER    (HAL_AFEREG_MAP->WOBAPC20)
#define HAL_AFE_AGCHPFFC_LSB         5
#define HAL_AFE_AGCHPFFC_MSB         7
#define HAL_AFE_AGCHPFFC_MASK        0x000000E0
#define HAL_AFE_AGCHPFFC_TYPE        halAFE_AGCHPF_t
#define HAL_AFE_AGCHPFFC             HAL_AFE_AGCHPFFC_MASK
#define HAL_AFE_AGCHPFFC_READABLE    1
#define HAL_AFE_AGCHPFFC_WRITABLE    1
#define HAL_AFE_AGCHPFFC_SIGNED      0
#define HAL_AFE_AGCHPFFC_ADR         0x1000099E

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_OPTSTEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC20)
#define HAL_AFE_OPTSTEN_LSB         4
#define HAL_AFE_OPTSTEN_MSB         4
#define HAL_AFE_OPTSTEN_MASK        0x00000010
#define HAL_AFE_OPTSTEN_TYPE        Bool
#define HAL_AFE_OPTSTEN             HAL_AFE_OPTSTEN_MASK
#define HAL_AFE_OPTSTEN_READABLE    1
#define HAL_AFE_OPTSTEN_WRITABLE    1
#define HAL_AFE_OPTSTEN_SIGNED      0
#define HAL_AFE_OPTSTEN_ADR         0x1000099E

#define HAL_AFE_APCINSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC20)
#define HAL_AFE_APCINSEL_LSB         2
#define HAL_AFE_APCINSEL_MSB         3
#define HAL_AFE_APCINSEL_MASK        0x0000000C
#define HAL_AFE_APCINSEL_TYPE        halAFE_APCINPUT_t
#define HAL_AFE_APCINSEL             HAL_AFE_APCINSEL_MASK
#define HAL_AFE_APCINSEL_READABLE    1
#define HAL_AFE_APCINSEL_WRITABLE    1
#define HAL_AFE_APCINSEL_SIGNED      0
#define HAL_AFE_APCINSEL_ADR         0x1000099E

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_VRPWRLPF_REGISTER    (HAL_AFEREG_MAP->WOBAPC20)
#define HAL_AFE_VRPWRLPF_LSB         1
#define HAL_AFE_VRPWRLPF_MSB         1
#define HAL_AFE_VRPWRLPF_MASK        0x00000002
#define HAL_AFE_VRPWRLPF_TYPE        Bool
#define HAL_AFE_VRPWRLPF             HAL_AFE_VRPWRLPF_MASK
#define HAL_AFE_VRPWRLPF_READABLE    1
#define HAL_AFE_VRPWRLPF_WRITABLE    1
#define HAL_AFE_VRPWRLPF_SIGNED      0
#define HAL_AFE_VRPWRLPF_ADR         0x1000099E

#define HAL_AFE_VWPWRLPF_REGISTER    (HAL_AFEREG_MAP->WOBAPC20)
#define HAL_AFE_VWPWRLPF_LSB         0
#define HAL_AFE_VWPWRLPF_MSB         0
#define HAL_AFE_VWPWRLPF_MASK        0x00000001
#define HAL_AFE_VWPWRLPF_TYPE        Bool
#define HAL_AFE_VWPWRLPF             HAL_AFE_VWPWRLPF_MASK
#define HAL_AFE_VWPWRLPF_READABLE    1
#define HAL_AFE_VWPWRLPF_WRITABLE    1
#define HAL_AFE_VWPWRLPF_SIGNED      0
#define HAL_AFE_VWPWRLPF_ADR         0x1000099E

// Register WOBAPC21
#define HAL_AFE_BPAGC3HP_REGISTER    (HAL_AFEREG_MAP->WOBAPC21)
#define HAL_AFE_BPAGC3HP_LSB         7
#define HAL_AFE_BPAGC3HP_MSB         7
#define HAL_AFE_BPAGC3HP_MASK        0x00000080
#define HAL_AFE_BPAGC3HP_TYPE        Bool
#define HAL_AFE_BPAGC3HP             HAL_AFE_BPAGC3HP_MASK
#define HAL_AFE_BPAGC3HP_READABLE    1
#define HAL_AFE_BPAGC3HP_WRITABLE    1
#define HAL_AFE_BPAGC3HP_SIGNED      0
#define HAL_AFE_BPAGC3HP_ADR         0x1000099F

#define HAL_AFE_AGCHPFDF_REGISTER    (HAL_AFEREG_MAP->WOBAPC21)
#define HAL_AFE_AGCHPFDF_LSB         6
#define HAL_AFE_AGCHPFDF_MSB         6
#define HAL_AFE_AGCHPFDF_MASK        0x00000040
#define HAL_AFE_AGCHPFDF_TYPE        Bool
#define HAL_AFE_AGCHPFDF             HAL_AFE_AGCHPFDF_MASK
#define HAL_AFE_AGCHPFDF_READABLE    1
#define HAL_AFE_AGCHPFDF_WRITABLE    1
#define HAL_AFE_AGCHPFDF_SIGNED      0
#define HAL_AFE_AGCHPFDF_ADR         0x1000099F

#define HAL_AFE_AGCIOUT_REGISTER    (HAL_AFEREG_MAP->WOBAPC21)
#define HAL_AFE_AGCIOUT_LSB         3
#define HAL_AFE_AGCIOUT_MSB         5
#define HAL_AFE_AGCIOUT_MASK        0x00000038
#define HAL_AFE_AGCIOUT_TYPE        halAFE_AGCAmptdSetlTime_t
#define HAL_AFE_AGCIOUT             HAL_AFE_AGCIOUT_MASK
#define HAL_AFE_AGCIOUT_READABLE    1
#define HAL_AFE_AGCIOUT_WRITABLE    1
#define HAL_AFE_AGCIOUT_SIGNED      0
#define HAL_AFE_AGCIOUT_ADR         0x1000099F

#define HAL_AFE_WDIFFGAIN_REGISTER    (HAL_AFEREG_MAP->WOBAPC21)
#define HAL_AFE_WDIFFGAIN_LSB         0
#define HAL_AFE_WDIFFGAIN_MSB         2
#define HAL_AFE_WDIFFGAIN_MASK        0x00000007
#define HAL_AFE_WDIFFGAIN_TYPE        halAFE_WblSumAmp_t
#define HAL_AFE_WDIFFGAIN             HAL_AFE_WDIFFGAIN_MASK
#define HAL_AFE_WDIFFGAIN_READABLE    1
#define HAL_AFE_WDIFFGAIN_WRITABLE    1
#define HAL_AFE_WDIFFGAIN_SIGNED      0
#define HAL_AFE_WDIFFGAIN_ADR         0x1000099F

// Register WOBAPC22
#define HAL_AFE_LPPINV_REGISTER    (HAL_AFEREG_MAP->WOBAPC22)
#define HAL_AFE_LPPINV_LSB         7
#define HAL_AFE_LPPINV_MSB         7
#define HAL_AFE_LPPINV_MASK        0x00000080
#define HAL_AFE_LPPINV_TYPE        Bool
#define HAL_AFE_LPPINV             HAL_AFE_LPPINV_MASK
#define HAL_AFE_LPPINV_READABLE    1
#define HAL_AFE_LPPINV_WRITABLE    1
#define HAL_AFE_LPPINV_SIGNED      0
#define HAL_AFE_LPPINV_ADR         0x100009A0

#define HAL_AFE_WOBTEST_REGISTER    (HAL_AFEREG_MAP->WOBAPC22)
#define HAL_AFE_WOBTEST_LSB         3
#define HAL_AFE_WOBTEST_MSB         6
#define HAL_AFE_WOBTEST_MASK        0x00000078
#define HAL_AFE_WOBTEST_TYPE        UByte
#define HAL_AFE_WOBTEST             HAL_AFE_WOBTEST_MASK
#define HAL_AFE_WOBTEST_READABLE    1
#define HAL_AFE_WOBTEST_WRITABLE    1
#define HAL_AFE_WOBTEST_SIGNED      0
#define HAL_AFE_WOBTEST_ADR         0x100009A0

#define HAL_AFE_DISCLIP_REGISTER    (HAL_AFEREG_MAP->WOBAPC22)
#define HAL_AFE_DISCLIP_LSB         2
#define HAL_AFE_DISCLIP_MSB         2
#define HAL_AFE_DISCLIP_MASK        0x00000004
#define HAL_AFE_DISCLIP_TYPE        Bool
#define HAL_AFE_DISCLIP             HAL_AFE_DISCLIP_MASK
#define HAL_AFE_DISCLIP_READABLE    1
#define HAL_AFE_DISCLIP_WRITABLE    1
#define HAL_AFE_DISCLIP_SIGNED      0
#define HAL_AFE_DISCLIP_ADR         0x100009A0

#define HAL_AFE_WOBHDREN_REGISTER    (HAL_AFEREG_MAP->WOBAPC22)
#define HAL_AFE_WOBHDREN_LSB         1
#define HAL_AFE_WOBHDREN_MSB         1
#define HAL_AFE_WOBHDREN_MASK        0x00000002
#define HAL_AFE_WOBHDREN_TYPE        Bool
#define HAL_AFE_WOBHDREN             HAL_AFE_WOBHDREN_MASK
#define HAL_AFE_WOBHDREN_READABLE    1
#define HAL_AFE_WOBHDREN_WRITABLE    1
#define HAL_AFE_WOBHDREN_SIGNED      0
#define HAL_AFE_WOBHDREN_ADR         0x100009A0

#define HAL_AFE_DEFECTEN_REGISTER    (HAL_AFEREG_MAP->WOBAPC22)
#define HAL_AFE_DEFECTEN_LSB         0
#define HAL_AFE_DEFECTEN_MSB         0
#define HAL_AFE_DEFECTEN_MASK        0x00000001
#define HAL_AFE_DEFECTEN_TYPE        Bool
#define HAL_AFE_DEFECTEN             HAL_AFE_DEFECTEN_MASK
#define HAL_AFE_DEFECTEN_READABLE    1
#define HAL_AFE_DEFECTEN_WRITABLE    1
#define HAL_AFE_DEFECTEN_SIGNED      0
#define HAL_AFE_DEFECTEN_ADR         0x100009A0

// Register WOBAPC23
#define HAL_AFE_AGCCLIP1_REGISTER    (HAL_AFEREG_MAP->WOBAPC23)
#define HAL_AFE_AGCCLIP1_LSB         6
#define HAL_AFE_AGCCLIP1_MSB         7
#define HAL_AFE_AGCCLIP1_MASK        0x000000C0
#define HAL_AFE_AGCCLIP1_TYPE        halAFE_AGCCLIP1_t
#define HAL_AFE_AGCCLIP1             HAL_AFE_AGCCLIP1_MASK
#define HAL_AFE_AGCCLIP1_READABLE    1
#define HAL_AFE_AGCCLIP1_WRITABLE    1
#define HAL_AFE_AGCCLIP1_SIGNED      0
#define HAL_AFE_AGCCLIP1_ADR         0x100009A1

#define HAL_AFE_AGCCLIP2_REGISTER    (HAL_AFEREG_MAP->WOBAPC23)
#define HAL_AFE_AGCCLIP2_LSB         4
#define HAL_AFE_AGCCLIP2_MSB         5
#define HAL_AFE_AGCCLIP2_MASK        0x00000030
#define HAL_AFE_AGCCLIP2_TYPE        halAFE_AGCCLIP2_t
#define HAL_AFE_AGCCLIP2             HAL_AFE_AGCCLIP2_MASK
#define HAL_AFE_AGCCLIP2_READABLE    1
#define HAL_AFE_AGCCLIP2_WRITABLE    1
#define HAL_AFE_AGCCLIP2_SIGNED      0
#define HAL_AFE_AGCCLIP2_ADR         0x100009A1

#define HAL_AFE_AGCCLIP3_REGISTER    (HAL_AFEREG_MAP->WOBAPC23)
#define HAL_AFE_AGCCLIP3_LSB         2
#define HAL_AFE_AGCCLIP3_MSB         3
#define HAL_AFE_AGCCLIP3_MASK        0x0000000C
#define HAL_AFE_AGCCLIP3_TYPE        halAFE_AGCCLIP3_t
#define HAL_AFE_AGCCLIP3             HAL_AFE_AGCCLIP3_MASK
#define HAL_AFE_AGCCLIP3_READABLE    1
#define HAL_AFE_AGCCLIP3_WRITABLE    1
#define HAL_AFE_AGCCLIP3_SIGNED      0
#define HAL_AFE_AGCCLIP3_ADR         0x100009A1

#define HAL_AFE_AGCHOLD_REGISTER    (HAL_AFEREG_MAP->WOBAPC23)
#define HAL_AFE_AGCHOLD_LSB         1
#define HAL_AFE_AGCHOLD_MSB         1
#define HAL_AFE_AGCHOLD_MASK        0x00000002
#define HAL_AFE_AGCHOLD_TYPE        Bool
#define HAL_AFE_AGCHOLD             HAL_AFE_AGCHOLD_MASK
#define HAL_AFE_AGCHOLD_READABLE    1
#define HAL_AFE_AGCHOLD_WRITABLE    1
#define HAL_AFE_AGCHOLD_SIGNED      0
#define HAL_AFE_AGCHOLD_ADR         0x100009A1

#define HAL_AFE_AGCSFGN_REGISTER    (HAL_AFEREG_MAP->WOBAPC23)
#define HAL_AFE_AGCSFGN_LSB         0
#define HAL_AFE_AGCSFGN_MSB         0
#define HAL_AFE_AGCSFGN_MASK        0x00000001
#define HAL_AFE_AGCSFGN_TYPE        Bool
#define HAL_AFE_AGCSFGN             HAL_AFE_AGCSFGN_MASK
#define HAL_AFE_AGCSFGN_READABLE    1
#define HAL_AFE_AGCSFGN_WRITABLE    1
#define HAL_AFE_AGCSFGN_SIGNED      0
#define HAL_AFE_AGCSFGN_ADR         0x100009A1

// Register WOBAPC24
#define HAL_AFE_AGCWLDON_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_AGCWLDON_LSB         7
#define HAL_AFE_AGCWLDON_MSB         7
#define HAL_AFE_AGCWLDON_MASK        0x00000080
#define HAL_AFE_AGCWLDON_TYPE        Bool
#define HAL_AFE_AGCWLDON             HAL_AFE_AGCWLDON_MASK
#define HAL_AFE_AGCWLDON_READABLE    1
#define HAL_AFE_AGCWLDON_WRITABLE    1
#define HAL_AFE_AGCWLDON_SIGNED      0
#define HAL_AFE_AGCWLDON_ADR         0x100009A2

#define HAL_AFE_AGCWLDREC_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_AGCWLDREC_LSB         6
#define HAL_AFE_AGCWLDREC_MSB         6
#define HAL_AFE_AGCWLDREC_MASK        0x00000040
#define HAL_AFE_AGCWLDREC_TYPE        Bool
#define HAL_AFE_AGCWLDREC             HAL_AFE_AGCWLDREC_MASK
#define HAL_AFE_AGCWLDREC_READABLE    1
#define HAL_AFE_AGCWLDREC_WRITABLE    1
#define HAL_AFE_AGCWLDREC_SIGNED      0
#define HAL_AFE_AGCWLDREC_ADR         0x100009A2

#define HAL_AFE_WOBADTEST_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_WOBADTEST_LSB         5
#define HAL_AFE_WOBADTEST_MSB         5
#define HAL_AFE_WOBADTEST_MASK        0x00000020
#define HAL_AFE_WOBADTEST_TYPE        Bool
#define HAL_AFE_WOBADTEST             HAL_AFE_WOBADTEST_MASK
#define HAL_AFE_WOBADTEST_READABLE    1
#define HAL_AFE_WOBADTEST_WRITABLE    1
#define HAL_AFE_WOBADTEST_SIGNED      0
#define HAL_AFE_WOBADTEST_ADR         0x100009A2

#define HAL_AFE_WOBPROTST_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_WOBPROTST_LSB         4
#define HAL_AFE_WOBPROTST_MSB         4
#define HAL_AFE_WOBPROTST_MASK        0x00000010
#define HAL_AFE_WOBPROTST_TYPE        Bool
#define HAL_AFE_WOBPROTST             HAL_AFE_WOBPROTST_MASK
#define HAL_AFE_WOBPROTST_READABLE    1
#define HAL_AFE_WOBPROTST_WRITABLE    1
#define HAL_AFE_WOBPROTST_SIGNED      0
#define HAL_AFE_WOBPROTST_ADR         0x100009A2

#define HAL_AFE_WADVCMSEL_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_WADVCMSEL_LSB         2
#define HAL_AFE_WADVCMSEL_MSB         2
#define HAL_AFE_WADVCMSEL_MASK        0x00000004
#define HAL_AFE_WADVCMSEL_TYPE        Bool
#define HAL_AFE_WADVCMSEL             HAL_AFE_WADVCMSEL_MASK
#define HAL_AFE_WADVCMSEL_READABLE    1
#define HAL_AFE_WADVCMSEL_WRITABLE    1
#define HAL_AFE_WADVCMSEL_SIGNED      0
#define HAL_AFE_WADVCMSEL_ADR         0x100009A2

#define HAL_AFE_PDWPP12_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_PDWPP12_LSB         1
#define HAL_AFE_PDWPP12_MSB         1
#define HAL_AFE_PDWPP12_MASK        0x00000002
#define HAL_AFE_PDWPP12_TYPE        Bool
#define HAL_AFE_PDWPP12             HAL_AFE_PDWPP12_MASK
#define HAL_AFE_PDWPP12_READABLE    1
#define HAL_AFE_PDWPP12_WRITABLE    1
#define HAL_AFE_PDWPP12_SIGNED      0
#define HAL_AFE_PDWPP12_ADR         0x100009A2

#define HAL_AFE_PDWOB_REGISTER    (HAL_AFEREG_MAP->WOBAPC24)
#define HAL_AFE_PDWOB_LSB         0
#define HAL_AFE_PDWOB_MSB         0
#define HAL_AFE_PDWOB_MASK        0x00000001
#define HAL_AFE_PDWOB_TYPE        Bool
#define HAL_AFE_PDWOB             HAL_AFE_PDWOB_MASK
#define HAL_AFE_PDWOB_READABLE    1
#define HAL_AFE_PDWOB_WRITABLE    1
#define HAL_AFE_PDWOB_SIGNED      0
#define HAL_AFE_PDWOB_ADR         0x100009A2

// Register AVGSVOSET1
#define HAL_AFE_BSRATE_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET1)
#define HAL_AFE_BSRATE_LSB         4
#define HAL_AFE_BSRATE_MSB         7
#define HAL_AFE_BSRATE_MASK        0x000000F0
#define HAL_AFE_BSRATE_TYPE        UByte
#define HAL_AFE_BSRATE             HAL_AFE_BSRATE_MASK
#define HAL_AFE_BSRATE_READABLE    1
#define HAL_AFE_BSRATE_WRITABLE    1
#define HAL_AFE_BSRATE_SIGNED      0
#define HAL_AFE_BSRATE_ADR         0x100009A3

#define HAL_AFE_BSTHRES_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET1)
#define HAL_AFE_BSTHRES_LSB         0
#define HAL_AFE_BSTHRES_MSB         3
#define HAL_AFE_BSTHRES_MASK        0x0000000F
#define HAL_AFE_BSTHRES_TYPE        UByte
#define HAL_AFE_BSTHRES             HAL_AFE_BSTHRES_MASK
#define HAL_AFE_BSTHRES_READABLE    1
#define HAL_AFE_BSTHRES_WRITABLE    1
#define HAL_AFE_BSTHRES_SIGNED      0
#define HAL_AFE_BSTHRES_ADR         0x100009A3

// Register AVGSVOSET2
#define HAL_AFE_ENAVGSRV_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET2)
#define HAL_AFE_ENAVGSRV_LSB         7
#define HAL_AFE_ENAVGSRV_MSB         7
#define HAL_AFE_ENAVGSRV_MASK        0x00000080
#define HAL_AFE_ENAVGSRV_TYPE        Bool
#define HAL_AFE_ENAVGSRV             HAL_AFE_ENAVGSRV_MASK
#define HAL_AFE_ENAVGSRV_READABLE    1
#define HAL_AFE_ENAVGSRV_WRITABLE    1
#define HAL_AFE_ENAVGSRV_SIGNED      0
#define HAL_AFE_ENAVGSRV_ADR         0x100009A4

#define HAL_AFE_BSRATELOW_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET2)
#define HAL_AFE_BSRATELOW_LSB         6
#define HAL_AFE_BSRATELOW_MSB         6
#define HAL_AFE_BSRATELOW_MASK        0x00000040
#define HAL_AFE_BSRATELOW_TYPE        Bool
#define HAL_AFE_BSRATELOW             HAL_AFE_BSRATELOW_MASK
#define HAL_AFE_BSRATELOW_READABLE    1
#define HAL_AFE_BSRATELOW_WRITABLE    1
#define HAL_AFE_BSRATELOW_SIGNED      0
#define HAL_AFE_BSRATELOW_ADR         0x100009A4

#define HAL_AFE_DFCTHDAVG_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET2)
#define HAL_AFE_DFCTHDAVG_LSB         5
#define HAL_AFE_DFCTHDAVG_MSB         5
#define HAL_AFE_DFCTHDAVG_MASK        0x00000020
#define HAL_AFE_DFCTHDAVG_TYPE        Bool
#define HAL_AFE_DFCTHDAVG             HAL_AFE_DFCTHDAVG_MASK
#define HAL_AFE_DFCTHDAVG_READABLE    1
#define HAL_AFE_DFCTHDAVG_WRITABLE    1
#define HAL_AFE_DFCTHDAVG_SIGNED      0
#define HAL_AFE_DFCTHDAVG_ADR         0x100009A4

#define HAL_AFE_SETSVOGN_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET2)
#define HAL_AFE_SETSVOGN_LSB         0
#define HAL_AFE_SETSVOGN_MSB         4
#define HAL_AFE_SETSVOGN_MASK        0x0000001F
#define HAL_AFE_SETSVOGN_TYPE        UByte
#define HAL_AFE_SETSVOGN             HAL_AFE_SETSVOGN_MASK
#define HAL_AFE_SETSVOGN_READABLE    1
#define HAL_AFE_SETSVOGN_WRITABLE    1
#define HAL_AFE_SETSVOGN_SIGNED      0
#define HAL_AFE_SETSVOGN_ADR         0x100009A4

// Register AVGSVOSET3

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_AVGHDHLD_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET3)
#define HAL_AFE_AVGHDHLD_LSB         1
#define HAL_AFE_AVGHDHLD_MSB         1
#define HAL_AFE_AVGHDHLD_MASK        0x00000002
#define HAL_AFE_AVGHDHLD_TYPE        Bool
#define HAL_AFE_AVGHDHLD             HAL_AFE_AVGHDHLD_MASK
#define HAL_AFE_AVGHDHLD_READABLE    1
#define HAL_AFE_AVGHDHLD_WRITABLE    1
#define HAL_AFE_AVGHDHLD_SIGNED      0
#define HAL_AFE_AVGHDHLD_ADR         0x100009A5

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_AVGSVOSEL_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET3)
#define HAL_AFE_AVGSVOSEL_LSB         0
#define HAL_AFE_AVGSVOSEL_MSB         0
#define HAL_AFE_AVGSVOSEL_MASK        0x00000001
#define HAL_AFE_AVGSVOSEL_TYPE        Bool
#define HAL_AFE_AVGSVOSEL             HAL_AFE_AVGSVOSEL_MASK
#define HAL_AFE_AVGSVOSEL_READABLE    1
#define HAL_AFE_AVGSVOSEL_WRITABLE    1
#define HAL_AFE_AVGSVOSEL_SIGNED      0
#define HAL_AFE_AVGSVOSEL_ADR         0x100009A5

// Register AVGSVOSET4
#define HAL_AFE_RDSVOGN_REGISTER    (HAL_AFEREG_MAP->AVGSVOSET4)
#define HAL_AFE_RDSVOGN_LSB         0
#define HAL_AFE_RDSVOGN_MSB         4
#define HAL_AFE_RDSVOGN_MASK        0x0000001F
#define HAL_AFE_RDSVOGN_TYPE        UByte
#define HAL_AFE_RDSVOGN             HAL_AFE_RDSVOGN_MASK
#define HAL_AFE_RDSVOGN_READABLE    1
#define HAL_AFE_RDSVOGN_WRITABLE    0
#define HAL_AFE_RDSVOGN_SIGNED      0
#define HAL_AFE_RDSVOGN_ADR         0x100009A6

// Register TARGETBS
#define HAL_AFE_TARGETBS_REGISTER    (HAL_AFEREG_MAP->TARGETBS)
#define HAL_AFE_TARGETBS_LSB         0
#define HAL_AFE_TARGETBS_MSB         7
#define HAL_AFE_TARGETBS_MASK        0x000000FF
#define HAL_AFE_TARGETBS_TYPE        UByte
#define HAL_AFE_TARGETBS             HAL_AFE_TARGETBS_MASK
#define HAL_AFE_TARGETBS_READABLE    1
#define HAL_AFE_TARGETBS_WRITABLE    1
#define HAL_AFE_TARGETBS_SIGNED      0
#define HAL_AFE_TARGETBS_ADR         0x100009A7

// Register DFCTDLY
// Register BSRP22
#define HAL_AFE_BLANKBSKO_REGISTER    (HAL_AFEREG_MAP->BSRP22)
#define HAL_AFE_BLANKBSKO_LSB         0
#define HAL_AFE_BLANKBSKO_MSB         7
#define HAL_AFE_BLANKBSKO_MASK        0x000000FF
#define HAL_AFE_BLANKBSKO_TYPE        UByte
#define HAL_AFE_BLANKBSKO             HAL_AFE_BLANKBSKO_MASK
#define HAL_AFE_BLANKBSKO_READABLE    1
#define HAL_AFE_BLANKBSKO_WRITABLE    1
#define HAL_AFE_BLANKBSKO_SIGNED      0
#define HAL_AFE_BLANKBSKO_ADR         0x100009A9

// Register BSRP23
#define HAL_AFE_BSKO_REGISTER    (HAL_AFEREG_MAP->BSRP23)
#define HAL_AFE_BSKO_LSB         0
#define HAL_AFE_BSKO_MSB         7
#define HAL_AFE_BSKO_MASK        0x000000FF
#define HAL_AFE_BSKO_TYPE        UByte
#define HAL_AFE_BSKO             HAL_AFE_BSKO_MASK
#define HAL_AFE_BSKO_READABLE    1
#define HAL_AFE_BSKO_WRITABLE    1
#define HAL_AFE_BSKO_SIGNED      0
#define HAL_AFE_BSKO_ADR         0x100009AA

// Register BSRP24
#define HAL_AFE_BLANKBSKG_REGISTER    (HAL_AFEREG_MAP->BSRP24)
#define HAL_AFE_BLANKBSKG_LSB         0
#define HAL_AFE_BLANKBSKG_MSB         4
#define HAL_AFE_BLANKBSKG_MASK        0x0000001F
#define HAL_AFE_BLANKBSKG_TYPE        UByte
#define HAL_AFE_BLANKBSKG             HAL_AFE_BLANKBSKG_MASK
#define HAL_AFE_BLANKBSKG_READABLE    1
#define HAL_AFE_BLANKBSKG_WRITABLE    1
#define HAL_AFE_BLANKBSKG_SIGNED      0
#define HAL_AFE_BLANKBSKG_ADR         0x100009AB

// Register BSRP25

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_AHLDRPPBH_REGISTER    (HAL_AFEREG_MAP->BSRP25)
#define HAL_AFE_AHLDRPPBH_LSB         7
#define HAL_AFE_AHLDRPPBH_MSB         7
#define HAL_AFE_AHLDRPPBH_MASK        0x00000080
#define HAL_AFE_AHLDRPPBH_TYPE        Bool
#define HAL_AFE_AHLDRPPBH             HAL_AFE_AHLDRPPBH_MASK
#define HAL_AFE_AHLDRPPBH_READABLE    1
#define HAL_AFE_AHLDRPPBH_WRITABLE    1
#define HAL_AFE_AHLDRPPBH_SIGNED      0
#define HAL_AFE_AHLDRPPBH_ADR         0x100009AC

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_RCDSCKOF_REGISTER    (HAL_AFEREG_MAP->BSRP25)
#define HAL_AFE_RCDSCKOF_LSB         6
#define HAL_AFE_RCDSCKOF_MSB         6
#define HAL_AFE_RCDSCKOF_MASK        0x00000040
#define HAL_AFE_RCDSCKOF_TYPE        Bool
#define HAL_AFE_RCDSCKOF             HAL_AFE_RCDSCKOF_MASK
#define HAL_AFE_RCDSCKOF_READABLE    1
#define HAL_AFE_RCDSCKOF_WRITABLE    1
#define HAL_AFE_RCDSCKOF_SIGNED      0
#define HAL_AFE_RCDSCKOF_ADR         0x100009AC

#define HAL_AFE_RCINV_REGISTER    (HAL_AFEREG_MAP->BSRP25)
#define HAL_AFE_RCINV_LSB         5
#define HAL_AFE_RCINV_MSB         5
#define HAL_AFE_RCINV_MASK        0x00000020
#define HAL_AFE_RCINV_TYPE        Bool
#define HAL_AFE_RCINV             HAL_AFE_RCINV_MASK
#define HAL_AFE_RCINV_READABLE    1
#define HAL_AFE_RCINV_WRITABLE    1
#define HAL_AFE_RCINV_SIGNED      0
#define HAL_AFE_RCINV_ADR         0x100009AC

#define HAL_AFE_BSKG_REGISTER    (HAL_AFEREG_MAP->BSRP25)
#define HAL_AFE_BSKG_LSB         0
#define HAL_AFE_BSKG_MSB         4
#define HAL_AFE_BSKG_MASK        0x0000001F
#define HAL_AFE_BSKG_TYPE        UByte
#define HAL_AFE_BSKG             HAL_AFE_BSKG_MASK
#define HAL_AFE_BSKG_READABLE    1
#define HAL_AFE_BSKG_WRITABLE    1
#define HAL_AFE_BSKG_SIGNED      0
#define HAL_AFE_BSKG_ADR         0x100009AC

// Register BSRP26
#define HAL_AFE_RCTH_REGISTER    (HAL_AFEREG_MAP->BSRP26)
#define HAL_AFE_RCTH_LSB         4
#define HAL_AFE_RCTH_MSB         7
#define HAL_AFE_RCTH_MASK        0x000000F0
#define HAL_AFE_RCTH_TYPE        UByte
#define HAL_AFE_RCTH             HAL_AFE_RCTH_MASK
#define HAL_AFE_RCTH_READABLE    1
#define HAL_AFE_RCTH_WRITABLE    1
#define HAL_AFE_RCTH_SIGNED      0
#define HAL_AFE_RCTH_ADR         0x100009AD

#define HAL_AFE_RCTH2_REGISTER    (HAL_AFEREG_MAP->BSRP26)
#define HAL_AFE_RCTH2_LSB         0
#define HAL_AFE_RCTH2_MSB         3
#define HAL_AFE_RCTH2_MASK        0x0000000F
#define HAL_AFE_RCTH2_TYPE        UByte
#define HAL_AFE_RCTH2             HAL_AFE_RCTH2_MASK
#define HAL_AFE_RCTH2_READABLE    1
#define HAL_AFE_RCTH2_WRITABLE    1
#define HAL_AFE_RCTH2_SIGNED      0
#define HAL_AFE_RCTH2_ADR         0x100009AD

// Register BSRP27
#define HAL_AFE_RCTHSEL_REGISTER    (HAL_AFEREG_MAP->BSRP27)
#define HAL_AFE_RCTHSEL_LSB         7
#define HAL_AFE_RCTHSEL_MSB         7
#define HAL_AFE_RCTHSEL_MASK        0x00000080
#define HAL_AFE_RCTHSEL_TYPE        Bool
#define HAL_AFE_RCTHSEL             HAL_AFE_RCTHSEL_MASK
#define HAL_AFE_RCTHSEL_READABLE    1
#define HAL_AFE_RCTHSEL_WRITABLE    1
#define HAL_AFE_RCTHSEL_SIGNED      0
#define HAL_AFE_RCTHSEL_ADR         0x100009AE

#define HAL_AFE_RPPOL_REGISTER    (HAL_AFEREG_MAP->BSRP27)
#define HAL_AFE_RPPOL_LSB         6
#define HAL_AFE_RPPOL_MSB         6
#define HAL_AFE_RPPOL_MASK        0x00000040
#define HAL_AFE_RPPOL_TYPE        Bool
#define HAL_AFE_RPPOL             HAL_AFE_RPPOL_MASK
#define HAL_AFE_RPPOL_READABLE    1
#define HAL_AFE_RPPOL_WRITABLE    1
#define HAL_AFE_RPPOL_SIGNED      0
#define HAL_AFE_RPPOL_ADR         0x100009AE

#define HAL_AFE_RCDIS_REGISTER    (HAL_AFEREG_MAP->BSRP27)
#define HAL_AFE_RCDIS_LSB         4
#define HAL_AFE_RCDIS_MSB         5
#define HAL_AFE_RCDIS_MASK        0x00000030
#define HAL_AFE_RCDIS_TYPE        UByte
#define HAL_AFE_RCDIS             HAL_AFE_RCDIS_MASK
#define HAL_AFE_RCDIS_READABLE    1
#define HAL_AFE_RCDIS_WRITABLE    1
#define HAL_AFE_RCDIS_SIGNED      0
#define HAL_AFE_RCDIS_ADR         0x100009AE

#define HAL_AFE_RCDISCK_REGISTER    (HAL_AFEREG_MAP->BSRP27)
#define HAL_AFE_RCDISCK_LSB         0
#define HAL_AFE_RCDISCK_MSB         3
#define HAL_AFE_RCDISCK_MASK        0x0000000F
#define HAL_AFE_RCDISCK_TYPE        UByte
#define HAL_AFE_RCDISCK             HAL_AFE_RCDISCK_MASK
#define HAL_AFE_RCDISCK_READABLE    1
#define HAL_AFE_RCDISCK_WRITABLE    1
#define HAL_AFE_RCDISCK_SIGNED      0
#define HAL_AFE_RCDISCK_ADR         0x100009AE

// Register FCTS41

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WRLNDTRKO_REGISTER    (HAL_AFEREG_MAP->FCTS41)
#define HAL_AFE_WRLNDTRKO_LSB         0
#define HAL_AFE_WRLNDTRKO_MSB         7
#define HAL_AFE_WRLNDTRKO_MASK        0x000000FF
#define HAL_AFE_WRLNDTRKO_TYPE        UByte
#define HAL_AFE_WRLNDTRKO             HAL_AFE_WRLNDTRKO_MASK
#define HAL_AFE_WRLNDTRKO_READABLE    1
#define HAL_AFE_WRLNDTRKO_WRITABLE    1
#define HAL_AFE_WRLNDTRKO_SIGNED      0
#define HAL_AFE_WRLNDTRKO_ADR         0x100009AF

#endif // #if (CHIP_REV >= 0xC0)

// Register FCTS42

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_WRLNDFCSO_REGISTER    (HAL_AFEREG_MAP->FCTS42)
#define HAL_AFE_WRLNDFCSO_LSB         0
#define HAL_AFE_WRLNDFCSO_MSB         7
#define HAL_AFE_WRLNDFCSO_MASK        0x000000FF
#define HAL_AFE_WRLNDFCSO_TYPE        UByte
#define HAL_AFE_WRLNDFCSO             HAL_AFE_WRLNDFCSO_MASK
#define HAL_AFE_WRLNDFCSO_READABLE    1
#define HAL_AFE_WRLNDFCSO_WRITABLE    1
#define HAL_AFE_WRLNDFCSO_SIGNED      0
#define HAL_AFE_WRLNDFCSO_ADR         0x100009B0

#endif // #if (CHIP_REV >= 0xC0)

// Register SB20
#define HAL_AFE_DPDCH3OS_REGISTER    (HAL_AFEREG_MAP->SB20)
#define HAL_AFE_DPDCH3OS_LSB         0
#define HAL_AFE_DPDCH3OS_MSB         5
#define HAL_AFE_DPDCH3OS_MASK        0x0000003F
#define HAL_AFE_DPDCH3OS_TYPE        UByte
#define HAL_AFE_DPDCH3OS             HAL_AFE_DPDCH3OS_MASK
#define HAL_AFE_DPDCH3OS_READABLE    1
#define HAL_AFE_DPDCH3OS_WRITABLE    1
#define HAL_AFE_DPDCH3OS_SIGNED      0
#define HAL_AFE_DPDCH3OS_ADR         0x100009B1

// Register SB21
#define HAL_AFE_DPDCH4OS_REGISTER    (HAL_AFEREG_MAP->SB21)
#define HAL_AFE_DPDCH4OS_LSB         0
#define HAL_AFE_DPDCH4OS_MSB         5
#define HAL_AFE_DPDCH4OS_MASK        0x0000003F
#define HAL_AFE_DPDCH4OS_TYPE        UByte
#define HAL_AFE_DPDCH4OS             HAL_AFE_DPDCH4OS_MASK
#define HAL_AFE_DPDCH4OS_READABLE    1
#define HAL_AFE_DPDCH4OS_WRITABLE    1
#define HAL_AFE_DPDCH4OS_SIGNED      0
#define HAL_AFE_DPDCH4OS_ADR         0x100009B2

// Register MB21
#define HAL_AFE_DPDCH1OS_REGISTER    (HAL_AFEREG_MAP->MB21)
#define HAL_AFE_DPDCH1OS_LSB         0
#define HAL_AFE_DPDCH1OS_MSB         5
#define HAL_AFE_DPDCH1OS_MASK        0x0000003F
#define HAL_AFE_DPDCH1OS_TYPE        UByte
#define HAL_AFE_DPDCH1OS             HAL_AFE_DPDCH1OS_MASK
#define HAL_AFE_DPDCH1OS_READABLE    1
#define HAL_AFE_DPDCH1OS_WRITABLE    1
#define HAL_AFE_DPDCH1OS_SIGNED      0
#define HAL_AFE_DPDCH1OS_ADR         0x100009B3

// Register MB22
#define HAL_AFE_DPDCH2OS_REGISTER    (HAL_AFEREG_MAP->MB22)
#define HAL_AFE_DPDCH2OS_LSB         0
#define HAL_AFE_DPDCH2OS_MSB         5
#define HAL_AFE_DPDCH2OS_MASK        0x0000003F
#define HAL_AFE_DPDCH2OS_TYPE        UByte
#define HAL_AFE_DPDCH2OS             HAL_AFE_DPDCH2OS_MASK
#define HAL_AFE_DPDCH2OS_READABLE    1
#define HAL_AFE_DPDCH2OS_WRITABLE    1
#define HAL_AFE_DPDCH2OS_SIGNED      0
#define HAL_AFE_DPDCH2OS_ADR         0x100009B4

// Register TESTENA1
#define HAL_AFE_SPDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_SPDOVDEN_LSB         6
#define HAL_AFE_SPDOVDEN_MSB         6
#define HAL_AFE_SPDOVDEN_MASK        0x00000040
#define HAL_AFE_SPDOVDEN_TYPE        Bool
#define HAL_AFE_SPDOVDEN             HAL_AFE_SPDOVDEN_MASK
#define HAL_AFE_SPDOVDEN_READABLE    1
#define HAL_AFE_SPDOVDEN_WRITABLE    1
#define HAL_AFE_SPDOVDEN_SIGNED      0
#define HAL_AFE_SPDOVDEN_ADR         0x100009E0

#define HAL_AFE_MPDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_MPDOVDEN_LSB         5
#define HAL_AFE_MPDOVDEN_MSB         5
#define HAL_AFE_MPDOVDEN_MASK        0x00000020
#define HAL_AFE_MPDOVDEN_TYPE        Bool
#define HAL_AFE_MPDOVDEN             HAL_AFE_MPDOVDEN_MASK
#define HAL_AFE_MPDOVDEN_READABLE    1
#define HAL_AFE_MPDOVDEN_WRITABLE    1
#define HAL_AFE_MPDOVDEN_SIGNED      0
#define HAL_AFE_MPDOVDEN_ADR         0x100009E0

#define HAL_AFE_WBWDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_WBWDOVDEN_LSB         4
#define HAL_AFE_WBWDOVDEN_MSB         4
#define HAL_AFE_WBWDOVDEN_MASK        0x00000010
#define HAL_AFE_WBWDOVDEN_TYPE        Bool
#define HAL_AFE_WBWDOVDEN             HAL_AFE_WBWDOVDEN_MASK
#define HAL_AFE_WBWDOVDEN_READABLE    1
#define HAL_AFE_WBWDOVDEN_WRITABLE    1
#define HAL_AFE_WBWDOVDEN_SIGNED      0
#define HAL_AFE_WBWDOVDEN_ADR         0x100009E0

#define HAL_AFE_WBRDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_WBRDOVDEN_LSB         3
#define HAL_AFE_WBRDOVDEN_MSB         3
#define HAL_AFE_WBRDOVDEN_MASK        0x00000008
#define HAL_AFE_WBRDOVDEN_TYPE        Bool
#define HAL_AFE_WBRDOVDEN             HAL_AFE_WBRDOVDEN_MASK
#define HAL_AFE_WBRDOVDEN_READABLE    1
#define HAL_AFE_WBRDOVDEN_WRITABLE    1
#define HAL_AFE_WBRDOVDEN_SIGNED      0
#define HAL_AFE_WBRDOVDEN_ADR         0x100009E0

#define HAL_AFE_LANDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_LANDOVDEN_LSB         2
#define HAL_AFE_LANDOVDEN_MSB         2
#define HAL_AFE_LANDOVDEN_MASK        0x00000004
#define HAL_AFE_LANDOVDEN_TYPE        Bool
#define HAL_AFE_LANDOVDEN             HAL_AFE_LANDOVDEN_MASK
#define HAL_AFE_LANDOVDEN_READABLE    1
#define HAL_AFE_LANDOVDEN_WRITABLE    1
#define HAL_AFE_LANDOVDEN_SIGNED      0
#define HAL_AFE_LANDOVDEN_ADR         0x100009E0

#define HAL_AFE_HDROVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA1)
#define HAL_AFE_HDROVDEN_LSB         0
#define HAL_AFE_HDROVDEN_MSB         0
#define HAL_AFE_HDROVDEN_MASK        0x00000001
#define HAL_AFE_HDROVDEN_TYPE        Bool
#define HAL_AFE_HDROVDEN             HAL_AFE_HDROVDEN_MASK
#define HAL_AFE_HDROVDEN_READABLE    1
#define HAL_AFE_HDROVDEN_WRITABLE    1
#define HAL_AFE_HDROVDEN_SIGNED      0
#define HAL_AFE_HDROVDEN_ADR         0x100009E0

// Register TESTENA2
#define HAL_AFE_DFCTEOVEN_REGISTER    (HAL_AFEREG_MAP->TESTENA2)
#define HAL_AFE_DFCTEOVEN_LSB         3
#define HAL_AFE_DFCTEOVEN_MSB         3
#define HAL_AFE_DFCTEOVEN_MASK        0x00000008
#define HAL_AFE_DFCTEOVEN_TYPE        Bool
#define HAL_AFE_DFCTEOVEN             HAL_AFE_DFCTEOVEN_MASK
#define HAL_AFE_DFCTEOVEN_READABLE    1
#define HAL_AFE_DFCTEOVEN_WRITABLE    1
#define HAL_AFE_DFCTEOVEN_SIGNED      0
#define HAL_AFE_DFCTEOVEN_ADR         0x100009E1

#define HAL_AFE_WLDONOVEN_REGISTER    (HAL_AFEREG_MAP->TESTENA2)
#define HAL_AFE_WLDONOVEN_LSB         2
#define HAL_AFE_WLDONOVEN_MSB         2
#define HAL_AFE_WLDONOVEN_MASK        0x00000004
#define HAL_AFE_WLDONOVEN_TYPE        Bool
#define HAL_AFE_WLDONOVEN             HAL_AFE_WLDONOVEN_MASK
#define HAL_AFE_WLDONOVEN_READABLE    1
#define HAL_AFE_WLDONOVEN_WRITABLE    1
#define HAL_AFE_WLDONOVEN_SIGNED      0
#define HAL_AFE_WLDONOVEN_ADR         0x100009E1

#define HAL_AFE_WFPDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA2)
#define HAL_AFE_WFPDOVDEN_LSB         1
#define HAL_AFE_WFPDOVDEN_MSB         1
#define HAL_AFE_WFPDOVDEN_MASK        0x00000002
#define HAL_AFE_WFPDOVDEN_TYPE        Bool
#define HAL_AFE_WFPDOVDEN             HAL_AFE_WFPDOVDEN_MASK
#define HAL_AFE_WFPDOVDEN_READABLE    1
#define HAL_AFE_WFPDOVDEN_WRITABLE    1
#define HAL_AFE_WFPDOVDEN_SIGNED      0
#define HAL_AFE_WFPDOVDEN_ADR         0x100009E1

#define HAL_AFE_RFPDOVDEN_REGISTER    (HAL_AFEREG_MAP->TESTENA2)
#define HAL_AFE_RFPDOVDEN_LSB         0
#define HAL_AFE_RFPDOVDEN_MSB         0
#define HAL_AFE_RFPDOVDEN_MASK        0x00000001
#define HAL_AFE_RFPDOVDEN_TYPE        Bool
#define HAL_AFE_RFPDOVDEN             HAL_AFE_RFPDOVDEN_MASK
#define HAL_AFE_RFPDOVDEN_READABLE    1
#define HAL_AFE_RFPDOVDEN_WRITABLE    1
#define HAL_AFE_RFPDOVDEN_SIGNED      0
#define HAL_AFE_RFPDOVDEN_ADR         0x100009E1

// Register TESTVAL1
#define HAL_AFE_SPDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_SPDOVDVL_LSB         6
#define HAL_AFE_SPDOVDVL_MSB         6
#define HAL_AFE_SPDOVDVL_MASK        0x00000040
#define HAL_AFE_SPDOVDVL_TYPE        Bool
#define HAL_AFE_SPDOVDVL             HAL_AFE_SPDOVDVL_MASK
#define HAL_AFE_SPDOVDVL_READABLE    1
#define HAL_AFE_SPDOVDVL_WRITABLE    1
#define HAL_AFE_SPDOVDVL_SIGNED      0
#define HAL_AFE_SPDOVDVL_ADR         0x100009E2

#define HAL_AFE_MPDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_MPDOVDVL_LSB         5
#define HAL_AFE_MPDOVDVL_MSB         5
#define HAL_AFE_MPDOVDVL_MASK        0x00000020
#define HAL_AFE_MPDOVDVL_TYPE        Bool
#define HAL_AFE_MPDOVDVL             HAL_AFE_MPDOVDVL_MASK
#define HAL_AFE_MPDOVDVL_READABLE    1
#define HAL_AFE_MPDOVDVL_WRITABLE    1
#define HAL_AFE_MPDOVDVL_SIGNED      0
#define HAL_AFE_MPDOVDVL_ADR         0x100009E2

#define HAL_AFE_WBWDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_WBWDOVDVL_LSB         4
#define HAL_AFE_WBWDOVDVL_MSB         4
#define HAL_AFE_WBWDOVDVL_MASK        0x00000010
#define HAL_AFE_WBWDOVDVL_TYPE        Bool
#define HAL_AFE_WBWDOVDVL             HAL_AFE_WBWDOVDVL_MASK
#define HAL_AFE_WBWDOVDVL_READABLE    1
#define HAL_AFE_WBWDOVDVL_WRITABLE    1
#define HAL_AFE_WBWDOVDVL_SIGNED      0
#define HAL_AFE_WBWDOVDVL_ADR         0x100009E2

#define HAL_AFE_WBRDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_WBRDOVDVL_LSB         3
#define HAL_AFE_WBRDOVDVL_MSB         3
#define HAL_AFE_WBRDOVDVL_MASK        0x00000008
#define HAL_AFE_WBRDOVDVL_TYPE        Bool
#define HAL_AFE_WBRDOVDVL             HAL_AFE_WBRDOVDVL_MASK
#define HAL_AFE_WBRDOVDVL_READABLE    1
#define HAL_AFE_WBRDOVDVL_WRITABLE    1
#define HAL_AFE_WBRDOVDVL_SIGNED      0
#define HAL_AFE_WBRDOVDVL_ADR         0x100009E2

#define HAL_AFE_LANDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_LANDOVDVL_LSB         2
#define HAL_AFE_LANDOVDVL_MSB         2
#define HAL_AFE_LANDOVDVL_MASK        0x00000004
#define HAL_AFE_LANDOVDVL_TYPE        Bool
#define HAL_AFE_LANDOVDVL             HAL_AFE_LANDOVDVL_MASK
#define HAL_AFE_LANDOVDVL_READABLE    1
#define HAL_AFE_LANDOVDVL_WRITABLE    1
#define HAL_AFE_LANDOVDVL_SIGNED      0
#define HAL_AFE_LANDOVDVL_ADR         0x100009E2

#define HAL_AFE_HDROVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL1)
#define HAL_AFE_HDROVDVL_LSB         0
#define HAL_AFE_HDROVDVL_MSB         0
#define HAL_AFE_HDROVDVL_MASK        0x00000001
#define HAL_AFE_HDROVDVL_TYPE        Bool
#define HAL_AFE_HDROVDVL             HAL_AFE_HDROVDVL_MASK
#define HAL_AFE_HDROVDVL_READABLE    1
#define HAL_AFE_HDROVDVL_WRITABLE    1
#define HAL_AFE_HDROVDVL_SIGNED      0
#define HAL_AFE_HDROVDVL_ADR         0x100009E2

// Register TESTVAL2
#define HAL_AFE_AMIO2OVVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_AMIO2OVVL_LSB         6
#define HAL_AFE_AMIO2OVVL_MSB         6
#define HAL_AFE_AMIO2OVVL_MASK        0x00000040
#define HAL_AFE_AMIO2OVVL_TYPE        Bool
#define HAL_AFE_AMIO2OVVL             HAL_AFE_AMIO2OVVL_MASK
#define HAL_AFE_AMIO2OVVL_READABLE    1
#define HAL_AFE_AMIO2OVVL_WRITABLE    1
#define HAL_AFE_AMIO2OVVL_SIGNED      0
#define HAL_AFE_AMIO2OVVL_ADR         0x100009E3

#define HAL_AFE_AMIO1OVVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_AMIO1OVVL_LSB         5
#define HAL_AFE_AMIO1OVVL_MSB         5
#define HAL_AFE_AMIO1OVVL_MASK        0x00000020
#define HAL_AFE_AMIO1OVVL_TYPE        Bool
#define HAL_AFE_AMIO1OVVL             HAL_AFE_AMIO1OVVL_MASK
#define HAL_AFE_AMIO1OVVL_READABLE    1
#define HAL_AFE_AMIO1OVVL_WRITABLE    1
#define HAL_AFE_AMIO1OVVL_SIGNED      0
#define HAL_AFE_AMIO1OVVL_ADR         0x100009E3

#define HAL_AFE_AMIO0OVVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_AMIO0OVVL_LSB         4
#define HAL_AFE_AMIO0OVVL_MSB         4
#define HAL_AFE_AMIO0OVVL_MASK        0x00000010
#define HAL_AFE_AMIO0OVVL_TYPE        Bool
#define HAL_AFE_AMIO0OVVL             HAL_AFE_AMIO0OVVL_MASK
#define HAL_AFE_AMIO0OVVL_READABLE    1
#define HAL_AFE_AMIO0OVVL_WRITABLE    1
#define HAL_AFE_AMIO0OVVL_SIGNED      0
#define HAL_AFE_AMIO0OVVL_ADR         0x100009E3

#define HAL_AFE_DFCTEOVVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_DFCTEOVVL_LSB         3
#define HAL_AFE_DFCTEOVVL_MSB         3
#define HAL_AFE_DFCTEOVVL_MASK        0x00000008
#define HAL_AFE_DFCTEOVVL_TYPE        Bool
#define HAL_AFE_DFCTEOVVL             HAL_AFE_DFCTEOVVL_MASK
#define HAL_AFE_DFCTEOVVL_READABLE    1
#define HAL_AFE_DFCTEOVVL_WRITABLE    1
#define HAL_AFE_DFCTEOVVL_SIGNED      0
#define HAL_AFE_DFCTEOVVL_ADR         0x100009E3

#define HAL_AFE_WLDONOVVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_WLDONOVVL_LSB         2
#define HAL_AFE_WLDONOVVL_MSB         2
#define HAL_AFE_WLDONOVVL_MASK        0x00000004
#define HAL_AFE_WLDONOVVL_TYPE        Bool
#define HAL_AFE_WLDONOVVL             HAL_AFE_WLDONOVVL_MASK
#define HAL_AFE_WLDONOVVL_READABLE    1
#define HAL_AFE_WLDONOVVL_WRITABLE    1
#define HAL_AFE_WLDONOVVL_SIGNED      0
#define HAL_AFE_WLDONOVVL_ADR         0x100009E3

#define HAL_AFE_WFPDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_WFPDOVDVL_LSB         1
#define HAL_AFE_WFPDOVDVL_MSB         1
#define HAL_AFE_WFPDOVDVL_MASK        0x00000002
#define HAL_AFE_WFPDOVDVL_TYPE        Bool
#define HAL_AFE_WFPDOVDVL             HAL_AFE_WFPDOVDVL_MASK
#define HAL_AFE_WFPDOVDVL_READABLE    1
#define HAL_AFE_WFPDOVDVL_WRITABLE    1
#define HAL_AFE_WFPDOVDVL_SIGNED      0
#define HAL_AFE_WFPDOVDVL_ADR         0x100009E3

#define HAL_AFE_RFPDOVDVL_REGISTER    (HAL_AFEREG_MAP->TESTVAL2)
#define HAL_AFE_RFPDOVDVL_LSB         0
#define HAL_AFE_RFPDOVDVL_MSB         0
#define HAL_AFE_RFPDOVDVL_MASK        0x00000001
#define HAL_AFE_RFPDOVDVL_TYPE        Bool
#define HAL_AFE_RFPDOVDVL             HAL_AFE_RFPDOVDVL_MASK
#define HAL_AFE_RFPDOVDVL_READABLE    1
#define HAL_AFE_RFPDOVDVL_WRITABLE    1
#define HAL_AFE_RFPDOVDVL_SIGNED      0
#define HAL_AFE_RFPDOVDVL_ADR         0x100009E3

// Register AFEMIOSEL
#define HAL_AFE_AFEMIOSEL_REGISTER    (HAL_AFEREG_MAP->AFEMIOSEL)
#define HAL_AFE_AFEMIOSEL_LSB         0
#define HAL_AFE_AFEMIOSEL_MSB         1
#define HAL_AFE_AFEMIOSEL_MASK        0x00000003
#define HAL_AFE_AFEMIOSEL_TYPE        halAFE_AFEDIGMIOSEL_t
#define HAL_AFE_AFEMIOSEL             HAL_AFE_AFEMIOSEL_MASK
#define HAL_AFE_AFEMIOSEL_READABLE    1
#define HAL_AFE_AFEMIOSEL_WRITABLE    1
#define HAL_AFE_AFEMIOSEL_SIGNED      0
#define HAL_AFE_AFEMIOSEL_ADR         0x100009E4

// Register WOBAPC25
#define HAL_AFE_WOBVGA3EN_REGISTER    (HAL_AFEREG_MAP->WOBAPC25)
#define HAL_AFE_WOBVGA3EN_LSB         7
#define HAL_AFE_WOBVGA3EN_MSB         7
#define HAL_AFE_WOBVGA3EN_MASK        0x00000080
#define HAL_AFE_WOBVGA3EN_TYPE        Bool
#define HAL_AFE_WOBVGA3EN             HAL_AFE_WOBVGA3EN_MASK
#define HAL_AFE_WOBVGA3EN_READABLE    1
#define HAL_AFE_WOBVGA3EN_WRITABLE    1
#define HAL_AFE_WOBVGA3EN_SIGNED      0
#define HAL_AFE_WOBVGA3EN_ADR         0x100009E6

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RSHNOLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC25)
#define HAL_AFE_RSHNOLPON_LSB         6
#define HAL_AFE_RSHNOLPON_MSB         6
#define HAL_AFE_RSHNOLPON_MASK        0x00000040
#define HAL_AFE_RSHNOLPON_TYPE        Bool
#define HAL_AFE_RSHNOLPON             HAL_AFE_RSHNOLPON_MASK
#define HAL_AFE_RSHNOLPON_READABLE    1
#define HAL_AFE_RSHNOLPON_WRITABLE    1
#define HAL_AFE_RSHNOLPON_SIGNED      0
#define HAL_AFE_RSHNOLPON_ADR         0x100009E6

#define HAL_AFE_RSHLPON_REGISTER    (HAL_AFEREG_MAP->WOBAPC25)
#define HAL_AFE_RSHLPON_LSB         5
#define HAL_AFE_RSHLPON_MSB         5
#define HAL_AFE_RSHLPON_MASK        0x00000020
#define HAL_AFE_RSHLPON_TYPE        Bool
#define HAL_AFE_RSHLPON             HAL_AFE_RSHLPON_MASK
#define HAL_AFE_RSHLPON_READABLE    1
#define HAL_AFE_RSHLPON_WRITABLE    1
#define HAL_AFE_RSHLPON_SIGNED      0
#define HAL_AFE_RSHLPON_ADR         0x100009E6

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_AFE_WBVGA3REG_REGISTER    (HAL_AFEREG_MAP->WOBAPC25)
#define HAL_AFE_WBVGA3REG_LSB         0
#define HAL_AFE_WBVGA3REG_MSB         4
#define HAL_AFE_WBVGA3REG_MASK        0x0000001F
#define HAL_AFE_WBVGA3REG_TYPE        UByte
#define HAL_AFE_WBVGA3REG             HAL_AFE_WBVGA3REG_MASK
#define HAL_AFE_WBVGA3REG_READABLE    1
#define HAL_AFE_WBVGA3REG_WRITABLE    1
#define HAL_AFE_WBVGA3REG_SIGNED      0
#define HAL_AFE_WBVGA3REG_ADR         0x100009E6

// Register WOBAPC26

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RADOSB_REGISTER    (HAL_AFEREG_MAP->WOBAPC26)
#define HAL_AFE_RADOSB_LSB         0
#define HAL_AFE_RADOSB_MSB         7
#define HAL_AFE_RADOSB_MASK        0x000000FF
#define HAL_AFE_RADOSB_TYPE        UByte
#define HAL_AFE_RADOSB             HAL_AFE_RADOSB_MASK
#define HAL_AFE_RADOSB_READABLE    1
#define HAL_AFE_RADOSB_WRITABLE    1
#define HAL_AFE_RADOSB_SIGNED      0
#define HAL_AFE_RADOSB_ADR         0x100009E7

#endif // #if (CHIP_REV >= 0xC0)

// Register WOBAPC27

#if (CHIP_REV >= 0xC0)

#define HAL_AFE_RBCOSB_REGISTER    (HAL_AFEREG_MAP->WOBAPC27)
#define HAL_AFE_RBCOSB_LSB         0
#define HAL_AFE_RBCOSB_MSB         7
#define HAL_AFE_RBCOSB_MASK        0x000000FF
#define HAL_AFE_RBCOSB_TYPE        UByte
#define HAL_AFE_RBCOSB             HAL_AFE_RBCOSB_MASK
#define HAL_AFE_RBCOSB_READABLE    1
#define HAL_AFE_RBCOSB_WRITABLE    1
#define HAL_AFE_RBCOSB_SIGNED      0
#define HAL_AFE_RBCOSB_ADR         0x100009E8

#endif // #if (CHIP_REV >= 0xC0)

// Register MUX06
#define HAL_AFE_MUX3ADSEL_REGISTER    (HAL_AFEREG_MAP->MUX06)
#define HAL_AFE_MUX3ADSEL_LSB         0
#define HAL_AFE_MUX3ADSEL_MSB         4
#define HAL_AFE_MUX3ADSEL_MASK        0x0000001F
#define HAL_AFE_MUX3ADSEL_TYPE        halAFE_MUX3ADSEL_t
#define HAL_AFE_MUX3ADSEL             HAL_AFE_MUX3ADSEL_MASK
#define HAL_AFE_MUX3ADSEL_READABLE    1
#define HAL_AFE_MUX3ADSEL_WRITABLE    1
#define HAL_AFE_MUX3ADSEL_SIGNED      0
#define HAL_AFE_MUX3ADSEL_ADR         0x100009EC

// Register MUX07
#define HAL_AFE_OPCCLKOFF_REGISTER    (HAL_AFEREG_MAP->MUX07)
#define HAL_AFE_OPCCLKOFF_LSB         7
#define HAL_AFE_OPCCLKOFF_MSB         7
#define HAL_AFE_OPCCLKOFF_MASK        0x00000080
#define HAL_AFE_OPCCLKOFF_TYPE        Bool
#define HAL_AFE_OPCCLKOFF             HAL_AFE_OPCCLKOFF_MASK
#define HAL_AFE_OPCCLKOFF_READABLE    1
#define HAL_AFE_OPCCLKOFF_WRITABLE    1
#define HAL_AFE_OPCCLKOFF_SIGNED      0
#define HAL_AFE_OPCCLKOFF_ADR         0x100009ED

#define HAL_AFE_OPCDISCLK_REGISTER    (HAL_AFEREG_MAP->MUX07)
#define HAL_AFE_OPCDISCLK_LSB         0
#define HAL_AFE_OPCDISCLK_MSB         3
#define HAL_AFE_OPCDISCLK_MASK        0x0000000F
#define HAL_AFE_OPCDISCLK_TYPE        halAFE_OPCPHDRateDec_t
#define HAL_AFE_OPCDISCLK             HAL_AFE_OPCDISCLK_MASK
#define HAL_AFE_OPCDISCLK_READABLE    1
#define HAL_AFE_OPCDISCLK_WRITABLE    1
#define HAL_AFE_OPCDISCLK_SIGNED      0
#define HAL_AFE_OPCDISCLK_ADR         0x100009ED

// Register MUX08
#define HAL_AFE_MUXADSEL_REGISTER    (HAL_AFEREG_MAP->MUX08)
#define HAL_AFE_MUXADSEL_LSB         6
#define HAL_AFE_MUXADSEL_MSB         7
#define HAL_AFE_MUXADSEL_MASK        0x000000C0
#define HAL_AFE_MUXADSEL_TYPE        halAFE_MUXADSEL_t
#define HAL_AFE_MUXADSEL             HAL_AFE_MUXADSEL_MASK
#define HAL_AFE_MUXADSEL_READABLE    1
#define HAL_AFE_MUXADSEL_WRITABLE    1
#define HAL_AFE_MUXADSEL_SIGNED      0
#define HAL_AFE_MUXADSEL_ADR         0x100009EE

#define HAL_AFE_OPCSRC_REGISTER    (HAL_AFEREG_MAP->MUX08)
#define HAL_AFE_OPCSRC_LSB         4
#define HAL_AFE_OPCSRC_MSB         4
#define HAL_AFE_OPCSRC_MASK        0x00000010
#define HAL_AFE_OPCSRC_TYPE        halAFE_OPCSRC_t
#define HAL_AFE_OPCSRC             HAL_AFE_OPCSRC_MASK
#define HAL_AFE_OPCSRC_READABLE    1
#define HAL_AFE_OPCSRC_WRITABLE    1
#define HAL_AFE_OPCSRC_SIGNED      0
#define HAL_AFE_OPCSRC_ADR         0x100009EE

#define HAL_AFE_OPCCSEL_REGISTER    (HAL_AFEREG_MAP->MUX08)
#define HAL_AFE_OPCCSEL_LSB         3
#define HAL_AFE_OPCCSEL_MSB         3
#define HAL_AFE_OPCCSEL_MASK        0x00000008
#define HAL_AFE_OPCCSEL_TYPE        halAFE_OPCBaseRate_t
#define HAL_AFE_OPCCSEL             HAL_AFE_OPCCSEL_MASK
#define HAL_AFE_OPCCSEL_READABLE    1
#define HAL_AFE_OPCCSEL_WRITABLE    1
#define HAL_AFE_OPCCSEL_SIGNED      0
#define HAL_AFE_OPCCSEL_ADR         0x100009EE

#define HAL_AFE_OPCOUT_REGISTER    (HAL_AFEREG_MAP->MUX08)
#define HAL_AFE_OPCOUT_LSB         2
#define HAL_AFE_OPCOUT_MSB         2
#define HAL_AFE_OPCOUT_MASK        0x00000004
#define HAL_AFE_OPCOUT_TYPE        Bool
#define HAL_AFE_OPCOUT             HAL_AFE_OPCOUT_MASK
#define HAL_AFE_OPCOUT_READABLE    1
#define HAL_AFE_OPCOUT_WRITABLE    1
#define HAL_AFE_OPCOUT_SIGNED      0
#define HAL_AFE_OPCOUT_ADR         0x100009EE

#define HAL_AFE_OPCDIS_REGISTER    (HAL_AFEREG_MAP->MUX08)
#define HAL_AFE_OPCDIS_LSB         0
#define HAL_AFE_OPCDIS_MSB         1
#define HAL_AFE_OPCDIS_MASK        0x00000003
#define HAL_AFE_OPCDIS_TYPE        halAFE_OPCPHBHDRateInc_t
#define HAL_AFE_OPCDIS             HAL_AFE_OPCDIS_MASK
#define HAL_AFE_OPCDIS_READABLE    1
#define HAL_AFE_OPCDIS_WRITABLE    1
#define HAL_AFE_OPCDIS_SIGNED      0
#define HAL_AFE_OPCDIS_ADR         0x100009EE

// Enumerations

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halAFE_MUXTP1SEL0 = 0,    // DIGTP1(3)-VSS;       DIGTP1(2)-BLANK;  DIGTP1(1)-XTOK;  DIGTP1(0)-VSS;
	halAFE_MUXTP1SEL1 = 1,    // DIGTP1(3)-VSS;       DIGTP1(2)-DEFECT; DIGTP1(1)-ATTON; DIGTP1(0)-TECP;
	halAFE_MUXTP1SEL2 = 2,    // DIGTP1(3)-WOBDIGTP1; DIGTP1(2)-HDRB;   DIGTP1(1)-WLDON; DIGTP1(0)-RC;
	halAFE_MUXTP1SEL3 = 3     // DIGTP1(3)-RFDIGTP1;  DIGTP1(2)-MCLK;   DIGTP1(1)-LAND;  DIGTP1(0)-TX;
} halAFE_MUXTP1SEL_t;

typedef enum
{
	halAFE_MUXTP2SEL0 = 0,    // DIGTP2(3)-VSS;       DIGTP2(2)-BLANKI;    DIGTP2(1)-RC;     DIGTP2(0)-VSS;
	halAFE_MUXTP2SEL1 = 1,    // DIGTP2(3)-VSS;       DIGTP2(2)-DEFECTI;   DIGTP2(1)-GAINUP; DIGTP2(0)-TECN;
	halAFE_MUXTP2SEL2 = 2,    // DIGTP2(3)-WOBDIGTP2; DIGTP1(2)-HEADERHLD; DIGTP2(1)-WLDONI; DIGTP2(0)-RX;
	halAFE_MUXTP2SEL3 = 3     // DIGTP2(3)-RFDIGTP2;  DIGTP1(2)-NRST;      DIGTP2(1)-LANDI;  DIGTP2(0)-TX;
} halAFE_MUXTP2SEL_t;

typedef enum
{
	halAFE_MUX1SEL_GPADC1    = 0,
	halAFE_MUX1SEL_MPPO      = 1,
	halAFE_MUX1SEL_FEO       = 2,
	halAFE_MUX1SEL_SRFO      = 3,
	halAFE_MUX1SEL_FE        = 4,
	halAFE_MUX1SEL_TE        = 5,
	halAFE_MUX1SEL_CE        = 6,
	halAFE_MUX1SEL_BS        = 7,
	halAFE_MUX1SEL_RP        = 8,
	halAFE_MUX1SEL_PHS       = 9,
	halAFE_MUX1SEL_PHSO      = 10,
	halAFE_MUX1SEL_TECPTHO   = 11,
	halAFE_MUX1SEL_VREF_FCTS = 12,
	halAFE_MUX1SEL_APCMUX1   = 13,
	halAFE_MUX1SEL_WOBMUX1   = 14,
	halAFE_MUX1SEL_VREFI     = 15,
	halAFE_MUX1SEL_DPDP      = 16,
	halAFE_MUX1SEL_RFTP1     = 17,
	halAFE_MUX1SEL_vinao     = 18,
	halAFE_MUX1SEL_ANAMUX0   = 19,    // write_strategy_analog_vstrl_voltage
	halAFE_MUX1SEL_ANAMUX1   = 20,    // syspll_analog_vctrl_voltage
	halAFE_MUX1SEL_ANAMUX2   = 21,    // efmpll_analog_vctrl_voltage
	halAFE_MUX1SEL_ANAMUX3   = 22,    // adcpll_analog_vctrl_voltage
	halAFE_MUX1SEL_MUXOUT3   = 23,
	halAFE_MUX1SEL_SPPO1     = 24,
	halAFE_MUX1SEL_SPPO2     = 25,
	halAFE_MUX1SEL_SBAD      = 26,
	halAFE_MUX1SEL_SAN       = 27,
	halAFE_MUX1SEL_SBN       = 28,
	halAFE_MUX1SEL_SCN       = 29,
	halAFE_MUX1SEL_SDN       = 30,
	halAFE_MUX1SEL_VSS31     = 31 
} halAFE_MUX1SEL_t;

typedef enum
{
	halAFE_MUX2SEL_GPADC2     = 0,
	halAFE_MUX2SEL_SPPO1      = 1,
	halAFE_MUX2SEL_SPPO2      = 2,
	halAFE_MUX2SEL_SBAD       = 3,
	halAFE_MUX2SEL_FE         = 4,
	halAFE_MUX2SEL_TE         = 5,
	halAFE_MUX2SEL_CE         = 6,
	halAFE_MUX2SEL_BS         = 7,
	halAFE_MUX2SEL_RP         = 8,
	halAFE_MUX2SEL_BHS        = 9,
	halAFE_MUX2SEL_BHSO       = 10,
	halAFE_MUX2SEL_TECNTHO    = 11,
	halAFE_MUX2SEL_VREF_BSRP  = 12,
	halAFE_MUX2SEL_APCMUX2    = 13,
	halAFE_MUX2SEL_WOBMUX2    = 14,
	halAFE_MUX2SEL_VREFB      = 15,
	halAFE_MUX2SEL_DPDN       = 16,
	halAFE_MUX2SEL_RFTP2      = 17,
	halAFE_MUX2SEL_TEST_I40UA = 18,
	halAFE_MUX2SEL_ANAMUX4    = 19,    // srvpll_analog_vctrl_voltage
	halAFE_MUX2SEL_ANAMUX5    = 20,    // vref
	halAFE_MUX2SEL_ANAMUX6    = 21,    // vref
	halAFE_MUX2SEL_ANAMUX7    = 22,    // vref
	halAFE_MUX2SEL_MUXOUT1    = 23,
	halAFE_MUX2SEL_MPPO       = 24,
	halAFE_MUX2SEL_FEO        = 25,
	halAFE_MUX2SEL_SRFO       = 26,
	halAFE_MUX2SEL_SEN        = 27,
	halAFE_MUX2SEL_SFN        = 28,
	halAFE_MUX2SEL_SGN        = 29,
	halAFE_MUX2SEL_SHN        = 30,
	halAFE_MUX2SEL_VSS31      = 31 
} halAFE_MUX2SEL_t;

typedef enum
{
	halAFE_MUX3SEL_FE      = 0,
	halAFE_MUX3SEL_TE      = 1,
	halAFE_MUX3SEL_CE      = 2,
	halAFE_MUX3SEL_BS      = 3,
	halAFE_MUX3SEL_RP      = 4,
	halAFE_MUX3SEL_FE1     = 5,
	halAFE_MUX3SEL_TE1     = 6,
	halAFE_MUX3SEL_BS1     = 7,
	halAFE_MUX3SEL_RP1     = 8,
	halAFE_MUX3SEL_APCMUX1 = 9,
	halAFE_MUX3SEL_APCMUX2 = 10,
	halAFE_MUX3SEL_WOBMUX1 = 11,
	halAFE_MUX3SEL_WOBMUX2 = 12,
	halAFE_MUX3SEL_SAN     = 13,
	halAFE_MUX3SEL_SBN     = 14,
	halAFE_MUX3SEL_SCN     = 15,
	halAFE_MUX3SEL_SDN     = 16,
	halAFE_MUX3SEL_SEN     = 17,
	halAFE_MUX3SEL_SFN     = 18,
	halAFE_MUX3SEL_SGN     = 19,
	halAFE_MUX3SEL_SHN     = 20,
	halAFE_MUX3SEL_SIN     = 21,
	halAFE_MUX3SEL_SJN     = 22,
	halAFE_MUX3SEL_V125    = 23,
	halAFE_MUX3SEL_MUXOUT2 = 24,
	halAFE_MUX3SEL_SRFO    = 25,
	halAFE_MUX3SEL_VSS26   = 26,
	halAFE_MUX3SEL_VSS27   = 27,
	halAFE_MUX3SEL_VSS28   = 28,
	halAFE_MUX3SEL_VSS29   = 29,
	halAFE_MUX3SEL_VSS30   = 30,
	halAFE_MUX3SEL_VSS31   = 31 
} halAFE_MUX3SEL_t;

typedef enum
{
	halAFE_MUX1ADSEL_GPADC1    = 0,
	halAFE_MUX1ADSEL_MPPO      = 1,
	halAFE_MUX1ADSEL_FEO       = 2,
	halAFE_MUX1ADSEL_SRFO      = 3,
	halAFE_MUX1ADSEL_FE        = 4,
	halAFE_MUX1ADSEL_TE        = 5,
	halAFE_MUX1ADSEL_CE        = 6,
	halAFE_MUX1ADSEL_BS        = 7,
	halAFE_MUX1ADSEL_RP        = 8,
	halAFE_MUX1ADSEL_PHS       = 9,
	halAFE_MUX1ADSEL_PHSO      = 10,
	halAFE_MUX1ADSEL_TECPTHO   = 11,
	halAFE_MUX1ADSEL_VREF_FCTS = 12,
	halAFE_MUX1ADSEL_APCMUX1   = 13,
	halAFE_MUX1ADSEL_WOBMUX1   = 14,
	halAFE_MUX1ADSEL_VREFI     = 15,
	halAFE_MUX1ADSEL_DPDP      = 16,
	halAFE_MUX1ADSEL_RFTP1     = 17,
	halAFE_MUX1ADSEL_vinao     = 18,
	halAFE_MUX1ADSEL_ANAMUX0   = 19,    // write_strategy_analog_vstrl_voltage
	halAFE_MUX1ADSEL_ANAMUX1   = 20,    // syspll_analog_vctrl_voltage
	halAFE_MUX1ADSEL_ANAMUX2   = 21,    // efmpll_analog_vctrl_voltage
	halAFE_MUX1ADSEL_ANAMUX3   = 22,    // adcpll_analog_vctrl_voltage
	halAFE_MUX1ADSEL_MUXOUT3   = 23,
	halAFE_MUX1ADSEL_SPPO1     = 24,
	halAFE_MUX1ADSEL_SPPO2     = 25,
	halAFE_MUX1ADSEL_SBAD      = 26,
	halAFE_MUX1ADSEL_SAN       = 27,
	halAFE_MUX1ADSEL_SBN       = 28,
	halAFE_MUX1ADSEL_SCN       = 29,
	halAFE_MUX1ADSEL_ADN       = 30,
	halAFE_MUX1ADSEL_VSS31     = 31 
} halAFE_MUX1ADSEL_t;

typedef enum
{
	halAFE_MUX2ADSEL_GPADC2     = 0,
	halAFE_MUX2ADSEL_SPPO1      = 1,
	halAFE_MUX2ADSEL_SPPO2      = 2,
	halAFE_MUX2ADSEL_SBAD       = 3,
	halAFE_MUX2ADSEL_FE         = 4,
	halAFE_MUX2ADSEL_TE         = 5,
	halAFE_MUX2ADSEL_CE         = 6,
	halAFE_MUX2ADSEL_BS         = 7,
	halAFE_MUX2ADSEL_RP         = 8,
	halAFE_MUX2ADSEL_BHS        = 9,
	halAFE_MUX2ADSEL_BHSO       = 10,
	halAFE_MUX2ADSEL_TECNTHO    = 11,
	halAFE_MUX2ADSEL_VREF_BSRP  = 12,
	halAFE_MUX2ADSEL_APCMUX2    = 13,
	halAFE_MUX2ADSEL_WOBMUX2    = 14,
	halAFE_MUX2ADSEL_VREFB      = 15,
	halAFE_MUX2ADSEL_DPDN       = 16,
	halAFE_MUX2ADSEL_RFTP2      = 17,
	halAFE_MUX2ADSEL_TEST_I40UA = 18,
	halAFE_MUX2ADSEL_ANAMUX4    = 19,    // srvpll_analog_vctrl_voltage
	halAFE_MUX2ADSEL_ANAMUX5    = 20,    // vref
	halAFE_MUX2ADSEL_ANAMUX6    = 21,    // vref
	halAFE_MUX2ADSEL_ANAMUX7    = 22,    // vref
	halAFE_MUX2ADSEL_MUXOUT1    = 23,
	halAFE_MUX2ADSEL_MPPO       = 24,
	halAFE_MUX2ADSEL_FEO        = 25,
	halAFE_MUX2ADSEL_SRFO       = 26,
	halAFE_MUX2ADSEL_SEN        = 27,
	halAFE_MUX2ADSEL_SFN        = 28,
	halAFE_MUX2ADSEL_SGN        = 29,
	halAFE_MUX2ADSEL_SHN        = 30,
	halAFE_MUX2ADSEL_VSS31      = 31 
} halAFE_MUX2ADSEL_t;

typedef enum
{
	halAFE_PHDRateDec_Div32768 = 0,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^15)
	halAFE_PHDRateDec_Div16384 = 1,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^14)
	halAFE_PHDRateDec_Div8192  = 2,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^13)
	halAFE_PHDRateDec_Div4096  = 3,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^12)
	halAFE_PHDRateDec_Div2048  = 4,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^11)
	halAFE_PHDRateDec_Div1024  = 5,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^10)
	halAFE_PHDRateDec_Div512   = 6,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_PHDRateDec_Div256   = 7,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_PHDRateDec_Div128   = 8,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_PHDRateDec_Div64    = 9,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_PHDRateDec_Div32    = 10,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_PHDRateDec_Div16    = 11,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_PHDRateDec_Div8     = 12,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_PHDRateDec_Div4     = 13,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_PHDRateDec_Div2     = 14,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_PHDRateDec_Div1     = 15     // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_PHDRateDec_t;

typedef enum
{
	halAFE_BHDRateDec_Div32768 = 0,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^15)
	halAFE_BHDRateDec_Div16384 = 1,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^14)
	halAFE_BHDRateDec_Div8192  = 2,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^13)
	halAFE_BHDRateDec_Div4096  = 3,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^12)
	halAFE_BHDRateDec_Div2048  = 4,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^11)
	halAFE_BHDRateDec_Div1024  = 5,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^10)
	halAFE_BHDRateDec_Div512   = 6,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_BHDRateDec_Div256   = 7,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_BHDRateDec_Div128   = 8,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_BHDRateDec_Div64    = 9,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_BHDRateDec_Div32    = 10,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_BHDRateDec_Div16    = 11,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_BHDRateDec_Div8     = 12,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_BHDRateDec_Div4     = 13,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_BHDRateDec_Div2     = 14,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_BHDRateDec_Div1     = 15     // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_BHDRateDec_t;

typedef enum
{
	halAFE_BSPGN_N6dB = 0,    // BSPGN -6 dB
	halAFE_BSPGN_0dB  = 1,    // BSPGN 0 dB
	halAFE_BSPGN_6dB  = 2,    // BSPGN 6 dB
	halAFE_BSPGN_12dB = 3     // BSPGN 12 dB
} halAFE_BSPGN_t;

typedef enum
{
	halAFE_BSLPF_330kHz = 0,
	halAFE_BSLPF_150kHz = 1,
	halAFE_BSLPF_50kHz  = 2,
	halAFE_BSLPF_15kHz  = 3 
} halAFE_BSLPF_t;

typedef enum
{
	halAFE_RPPGN_N6dB = 0,    // RPPGN -6 dB
	halAFE_RPPGN_0dB  = 1,    // RPPGN 0 dB
	halAFE_RPPGN_6dB  = 2,    // RPPGN 6 dB
	halAFE_RPPGN_12dB = 3     // RPPGN 12 dB
} halAFE_RPPGN_t;

typedef enum
{
	halAFE_RPLPF_330kHz = 0,
	halAFE_RPLPF_150kHz = 1,
	halAFE_RPLPF_50kHz  = 2,
	halAFE_RPLPF_15kHz  = 3 
} halAFE_RPLPF_t;

typedef enum
{
	halAFE_PHBaseRate_680mVpus = 0,
	halAFE_PHBaseRate_370mVpus = 1 
} halAFE_PHBaseRate_t;

typedef enum
{
	halAFE_PHDRateInc_1X = 0,    // Increase Short Term P/H Droop Rate by a factor of 1
	halAFE_PHDRateInc_2X = 1,    // Increase Short Term P/H Droop Rate by a factor of 2
	halAFE_PHDRateInc_4X = 2,    // Increase Short Term P/H Droop Rate by a factor of 4
	halAFE_PHDRateInc_8X = 3     // Increase Short Term P/H Droop Rate by a factor of 8
} halAFE_PHDRateInc_t;

typedef enum
{
	halAFE_BHBaseRate_680mVpus = 0,
	halAFE_BHBaseRate_370mVpus = 1 
} halAFE_BHBaseRate_t;

typedef enum
{
	halAFE_BHDRateInc_1X = 0,    // Increase Short Term B/H Droop Rate by a factor of 1
	halAFE_BHDRateInc_2X = 1,    // Increase Short Term B/H Droop Rate by a factor of 2
	halAFE_BHDRateInc_4X = 2,    // Increase Short Term B/H Droop Rate by a factor of 4
	halAFE_BHDRateInc_8X = 3     // Increase Short Term B/H Droop Rate by a factor of 8
} halAFE_BHDRateInc_t;

typedef enum
{
	halAFE_FE2GnSEL_FE2GNREG = 0,    // FE2 gain set by FE2GNREG(4-0)
	halAFE_FE2GnSEL_AvgSRV   = 1     // FE2 gain set by the average servo loop
} halAFE_FE2GnSEL_t;

typedef enum
{
	halAFE_TE2GnSEL_FE2GNREG = 0,    // TE2 gain set by TE2GNREG(4-0)
	halAFE_TE2GnSEL_AvgSRV   = 1     // TE2 gain set by the average servo loop
} halAFE_TE2GnSEL_t;

typedef enum
{
	halAFE_TOKT_240us  = 0,    // TOK Time = 240 us
	halAFE_TOKT_480us  = 1,    // TOK Time = 480 us
	halAFE_TOKT_720us  = 2,    // TOK Time = 720 us
	halAFE_TOKT_960us  = 3,    // TOK Time = 960 us
	halAFE_TOKT_1200us = 4,    // TOK Time = 1200 us
	halAFE_TOKT_1440us = 5,    // TOK Time = 1440 us
	halAFE_TOKT_1680us = 6,    // TOK Time = 1680 us
	halAFE_TOKT_1920us = 7     // TOK Time = 1920 us
} halAFE_TOKT_t;

typedef enum
{
	halAFE_XTOKSEL_TECPaaTECN = 0,    // XTOK = TECP && TECN
	halAFE_XTOKSEL_TECPooTECN = 1     // XTOK = TECP || TECN
} halAFE_XTOKSEL_t;

typedef enum
{
	halAFE_FEPGN_N6dB = 0,    // FEPGN -6 dB
	halAFE_FEPGN_0dB  = 1,    // FEPGN 0 dB
	halAFE_FEPGN_6dB  = 2,    // FEPGN 6 dB
	halAFE_FEPGN_12dB = 3     // FEPGN 12 dB
} halAFE_FEPGN_t;

typedef enum
{
	halAFE_FELPF_330kHz = 0,
	halAFE_FELPF_150kHz = 1,
	halAFE_FELPF_50kHz  = 2,
	halAFE_FELPF_15kHz  = 3 
} halAFE_FELPF_t;

typedef enum
{
	halAFE_CEPGN_N6dB = 0,    // CEPGN -6 dB
	halAFE_CEPGN_0dB  = 1,    // CEPGN 0 dB
	halAFE_CEPGN_6dB  = 2,    // CEPGN 6 dB
	halAFE_CEPGN_12dB = 3     // CEPGN 12 dB
} halAFE_CEPGN_t;

typedef enum
{
	halAFE_CELPF_50kHz  = 0,
	halAFE_CELPF_15kHz  = 1,
	halAFE_CELPF_1kHz   = 2,
	halAFE_CELPF_0p5kHz = 3 
} halAFE_CELPF_t;

typedef enum
{
	halAFE_TEPGN_N6dB = 0,    // TEPGN -6 dB
	halAFE_TEPGN_0dB  = 1,    // TEPGN 0 dB
	halAFE_TEPGN_6dB  = 2,    // TEPGN 6 dB
	halAFE_TEPGN_12dB = 3     // TEPGN 12 dB
} halAFE_TEPGN_t;

typedef enum
{
	halAFE_TELPF_330kHz = 0,
	halAFE_TELPF_150kHz = 1,
	halAFE_TELPF_50kHz  = 2,
	halAFE_TELPF_15kHz  = 3 
} halAFE_TELPF_t;

typedef enum
{
	halAFE_TXLPF_330kHz = 0,
	halAFE_TXLPF_150kHz = 1,
	halAFE_TXLPF_50kHz  = 2,
	halAFE_TXLPF_15kHz  = 3 
} halAFE_TXLPF_t;

typedef enum
{
	halAFE_TOKLVL_VREF_0mV     = 0,    // TOK LVL = 1.65V + 0mV
	halAFE_TOKLVL_VREF_75mV    = 1,    // TOK LVL = 1.65V + 75mV
	halAFE_TOKLVL_VREF_150mV   = 2,    // TOK LVL = 1.65V + 150mV
	halAFE_TOKLVL_VREF_225mV   = 3,    // TOK LVL = 1.65V + 225mV
	halAFE_TOKLVL_VREF_300mV   = 4,    // TOK LVL = 1.65V + 300mV
	halAFE_TOKLVL_VREF_375mV   = 5,    // TOK LVL = 1.65V + 375mV
	halAFE_TOKLVL_VREF_450mV   = 6,    // TOK LVL = 1.65V + 450mV
	halAFE_TOKLVL_VREF_525mV   = 7,    // TOK LVL = 1.65V + 525mV
	halAFE_TOKLVL_VREF_600mV   = 8,    // TOK LVL = 1.65V + 600mV
	halAFE_TOKLVL_VREF_675mV   = 9,    // TOK LVL = 1.65V + 675mV
	halAFE_TOKLVL_VREF_750mV   = 10,    // TOK LVL = 1.65V + 750mV
	halAFE_TOKLVL_VREF_825mV   = 11,    // TOK LVL = 1.65V + 825mV
	halAFE_TOKLVL_VREF_900mV   = 12,    // TOK LVL = 1.65V + 900mV
	halAFE_TOKLVL_VREF_975mV   = 13,    // TOK LVL = 1.65V + 975mV
	halAFE_TOKLVL_VREF_1050mV  = 14,    // TOK LVL = 1.65V + 1050mV
	halAFE_TOKLVL_VREF_1125mV  = 15,    // TOK LVL = 1.65V + 1125mV
	halAFE_TOKLVL_VREF_m1200mV = 16,    // TOK LVL = 1.65V - 1200mV
	halAFE_TOKLVL_VREF_m1125mV = 17,    // TOK LVL = 1.65V - 1125mV
	halAFE_TOKLVL_VREF_m1050mV = 18,    // TOK LVL = 1.65V - 1050mV
	halAFE_TOKLVL_VREF_m975mV  = 19,    // TOK LVL = 1.65V - 975mV
	halAFE_TOKLVL_VREF_m900mV  = 20,    // TOK LVL = 1.65V - 900mV
	halAFE_TOKLVL_VREF_m825mV  = 21,    // TOK LVL = 1.65V - 825mV
	halAFE_TOKLVL_VREF_m750mV  = 22,    // TOK LVL = 1.65V - 750mV
	halAFE_TOKLVL_VREF_m675mV  = 23,    // TOK LVL = 1.65V - 675mV
	halAFE_TOKLVL_VREF_m600mV  = 24,    // TOK LVL = 1.65V - 600mV
	halAFE_TOKLVL_VREF_m525mV  = 25,    // TOK LVL = 1.65V - 525mV
	halAFE_TOKLVL_VREF_m450mV  = 26,    // TOK LVL = 1.65V - 450mV
	halAFE_TOKLVL_VREF_m375mV  = 27,    // TOK LVL = 1.65V - 375mV
	halAFE_TOKLVL_VREF_m300mV  = 28,    // TOK LVL = 1.65V - 300mV
	halAFE_TOKLVL_VREF_m225mV  = 29,    // TOK LVL = 1.65V - 225mV
	halAFE_TOKLVL_VREF_m150mV  = 30,    // TOK LVL = 1.65V - 150mV
	halAFE_TOKLVL_VREF_m75mV   = 31     // TOK LVL = 1.65V - 75mV
} halAFE_TOKLVL_t;

typedef enum
{
	halAFE_GUPDLY_60p5us = 0,
	halAFE_GUPDLY_121us  = 1,
	halAFE_GUPDLY_242us  = 2,
	halAFE_GUPDLY_484us  = 3 
} halAFE_GUPDLY_t;

typedef enum
{
	halAFE_MBSBBW_15MHz  = 1,
	halAFE_MBSBBW_50MHz  = 2,
	halAFE_MBSBBW_100MHz = 3 
} halAFE_MBSBBW_t;

typedef enum
{
	halAFE_MPPOPGN_N6dB = 0,    // MPPOPGN -6 dB
	halAFE_MPPOPGN_0dB  = 1,    // MPPOPGN 0 dB
	halAFE_MPPOPGN_6dB  = 2,    // MPPOPGN 6 dB
	halAFE_MPPOPGN_12dB = 3     // MPPOPGN 12 dB
} halAFE_MPPOPGN_t;

typedef enum
{
	halAFE_MPPOLPF_330kHz = 0,
	halAFE_MPPOLPF_660kHz = 1 
} halAFE_MPPOLPF_t;

typedef enum
{
	halAFE_FEOPGN_N6dB = 0,    // FEOPGN -6 dB
	halAFE_FEOPGN_0dB  = 1,    // FEOPGN 0 dB
	halAFE_FEOPGN_6dB  = 2,    // FEOPGN 6 dB
	halAFE_FEOPGN_12dB = 3     // FEOPGN 12 dB
} halAFE_FEOPGN_t;

typedef enum
{
	halAFE_FEOLPF_330kHz = 0,
	halAFE_FEOLPF_660kHz = 1 
} halAFE_FEOLPF_t;

typedef enum
{
	halAFE_TestLPF_30kHz  = 0,
	halAFE_TestLPF_100kHz = 1,
	halAFE_TestLPF_300kHz = 2,
	halAFE_TestLPF_660kHz = 3 
} halAFE_TestLPF_t;

typedef enum
{
	halAFE_SRFOLPF_330kHz = 0,
	halAFE_SRFOLPF_660kHz = 1 
} halAFE_SRFOLPF_t;

typedef enum
{
	halAFE_SPPO1WrGN_N6dB = 0,    // SPPO1 gain while writing: -6 dB
	halAFE_SPPO1WrGN_0dB  = 1,    // SPPO1 gain while writing: 0 dB
	halAFE_SPPO1WrGN_6dB  = 2,    // SPPO1 gain while writing: 6 dB
	halAFE_SPPO1WrGN_12dB = 3     // SPPO1 gain while writing: 12 dB
} halAFE_SPPO1WrGN_t;

typedef enum
{
	halAFE_SPPO1RdGN_N6dB = 0,    // SPPO1 gain while reading: -6 dB
	halAFE_SPPO1RdGN_0dB  = 1,    // SPPO1 gain while reading: 0 dB
	halAFE_SPPO1RdGN_6dB  = 2,    // SPPO1 gain while reading: 6 dB
	halAFE_SPPO1RdGN_12dB = 3     // SPPO1 gain while reading: 12 dB
} halAFE_SPPO1RdGN_t;

typedef enum
{
	halAFE_SPPO2WrGN_N6dB = 0,    // SPPO2 gain while writing: -6 dB
	halAFE_SPPO2WrGN_0dB  = 1,    // SPPO2 gain while writing: 0 dB
	halAFE_SPPO2WrGN_6dB  = 2,    // SPPO2 gain while writing: 6 dB
	halAFE_SPPO2WrGN_12dB = 3     // SPPO2 gain while writing: 12 dB
} halAFE_SPPO2WrGN_t;

typedef enum
{
	halAFE_SPPO2RdGN_N6dB = 0,    // SPPO2 gain while reading: -6 dB
	halAFE_SPPO2RdGN_0dB  = 1,    // SPPO2 gain while reading: 0 dB
	halAFE_SPPO2RdGN_6dB  = 2,    // SPPO2 gain while reading: 6 dB
	halAFE_SPPO2RdGN_12dB = 3     // SPPO2 gain while reading: 12 dB
} halAFE_SPPO2RdGN_t;

typedef enum
{
	halAFE_SPPO1LPF_330kHz = 0,
	halAFE_SPPO1LPF_660kHz = 1 
} halAFE_SPPO1LPF_t;

typedef enum
{
	halAFE_SPPO2LPF_330kHz = 0,
	halAFE_SPPO2LPF_660kHz = 1 
} halAFE_SPPO2LPF_t;

typedef enum
{
	halAFE_SBADLPF_330kHz = 0,
	halAFE_SBADLPF_660kHz = 1 
} halAFE_SBADLPF_t;

typedef enum
{
	halAFE_VREFBSEL_1p5V  = 0,
	halAFE_VREFBSEL_1p65V = 1,
	halAFE_VREFBSEL_1p8V  = 2,
	halAFE_VREFBSEL_1p95V = 3,
	halAFE_VREFBSEL_2p1V  = 4,
	halAFE_VREFBSEL_2p25V = 5,
	halAFE_VREFBSEL_2p4V  = 6,
	halAFE_VREFBSEL_2p55V = 7 
} halAFE_VREFBSEL_t;

typedef enum
{
	halAFE_EQSVOSEL0 = 0,    // EQDOP Output=VSS EQDON Output=VSS
	halAFE_EQSVOSEL1 = 1,
	halAFE_EQSVOSEL2 = 2,
	halAFE_EQSVOSEL3 = 3 
} halAFE_EQSVOSEL_t;

typedef enum
{
	halAFE_EQDPDSEL0 = 0,    // EQDOP Output=DPD4P EQDON Output=DPD4N
	halAFE_EQDPDSEL1 = 1,
	halAFE_EQDPDSEL2 = 2,
	halAFE_EQDPDSEL3 = 3 
} halAFE_EQDPDSEL_t;

typedef enum
{
	halAFE_RFANATSEL0 = 0,    // EQDOP Output=Off EQDON Output=Off
	halAFE_RFANATSEL1 = 1,
	halAFE_RFANATSEL2 = 2,
	halAFE_RFANATSEL3 = 3 
} halAFE_RFANATSEL_t;

typedef enum
{
	halAFE_DPDTSEL0 = 0,    // RFDIGTP1=DPDP1 RFDIGTP2=DPD12UP
	halAFE_DPDTSEL1 = 1,
	halAFE_DPDTSEL2 = 2,
	halAFE_DPDTSEL3 = 3 
} halAFE_DPDTSEL_t;

typedef enum
{
	halAFE_EQDOSEL0 = 0,    // EQDOP Output=Off EQDON Output=Off
	halAFE_EQDOSEL1 = 1,
	halAFE_EQDOSEL2 = 2,
	halAFE_EQDOSEL3 = 3,
	halAFE_EQDOSEL4 = 4,
	halAFE_EQDOSEL5 = 5,
	halAFE_EQDOSEL6 = 6,
	halAFE_EQDOSEL7 = 7 
} halAFE_EQDOSEL_t;

typedef enum
{
	halAFE_RFINSEL0 = 0,    // RF Input=BDRFP-BDRFN
	halAFE_RFINSEL1 = 1,
	halAFE_RFINSEL2 = 2,
	halAFE_RFINSEL3 = 3 
} halAFE_RFINSEL_t;

typedef enum
{
	halAFE_RFAGCGN_N8p0dB = 0,
	halAFE_RFAGCGN_N7p5dB = 1,
	halAFE_RFAGCGN_N7p0dB = 2,
	halAFE_RFAGCGN_N6p5dB = 3,
	halAFE_RFAGCGN_N6p0dB = 4,
	halAFE_RFAGCGN_N5p5dB = 5,
	halAFE_RFAGCGN_N5p0dB = 6,
	halAFE_RFAGCGN_N4p5dB = 7,
	halAFE_RFAGCGN_N4p0dB = 8,
	halAFE_RFAGCGN_N3p5dB = 9,
	halAFE_RFAGCGN_N3p0dB = 10,
	halAFE_RFAGCGN_N2p5dB = 11,
	halAFE_RFAGCGN_N2p0dB = 12,
	halAFE_RFAGCGN_N1p5dB = 13,
	halAFE_RFAGCGN_N1p0dB = 14,
	halAFE_RFAGCGN_N0p5dB = 15,
	halAFE_RFAGCGN_0dB    = 16,
	halAFE_RFAGCGN_0p5dB  = 17,
	halAFE_RFAGCGN_1p0dB  = 18,
	halAFE_RFAGCGN_1p5dB  = 19,
	halAFE_RFAGCGN_2p0dB  = 20,
	halAFE_RFAGCGN_2p5dB  = 21,
	halAFE_RFAGCGN_3p0dB  = 22,
	halAFE_RFAGCGN_3p5dB  = 23,
	halAFE_RFAGCGN_4p0dB  = 24,
	halAFE_RFAGCGN_4p5dB  = 25,
	halAFE_RFAGCGN_5p0dB  = 26,
	halAFE_RFAGCGN_5p5dB  = 27,
	halAFE_RFAGCGN_6p0dB  = 28,
	halAFE_RFAGCGN_6p5dB  = 29,
	halAFE_RFAGCGN_7p0dB  = 30,
	halAFE_RFAGCGN_7p5dB  = 31 
} halAFE_RFAGCGN_t;

typedef enum
{
	halAFE_RFLPFBk_Bypass = 0,
	halAFE_RFLPFBk_Bank1  = 1,
	halAFE_RFLPFBk_Bank2  = 2,
	halAFE_RFLPFBk_Bank3  = 3 
} halAFE_RFLPFBank_t;

typedef enum
{
	halAFE_RFLPFBW_CD_3p2MHz   = 0,    // CD 4X
	halAFE_RFLPFBW_CD_4p8MHz   = 2,    // CD 6X
	halAFE_RFLPFBW_CD_6p4MHz   = 4,    // CD 8X
	halAFE_RFLPFBW_CD_9p6MHz   = 8,    // CD 12X
	halAFE_RFLPFBW_CD_12p8MHz  = 12,    // CD 16X
	halAFE_RFLPFBW_CD_18p4MHz  = 19,    // CD 24X
	halAFE_RFLPFBW_CD_21p6MHz  = 23,    // CD 28X
	halAFE_RFLPFBW_CD_24p0MHz  = 26,    // CD 32X
	halAFE_RFLPFBW_CD_37p2MHz  = 9,    // CD 48X
	halAFE_RFLPFBW_CD_39p6MHz  = 10,    // CD 52X
	halAFE_RFLPFBW_DVD_4p8MHz  = 2,    // DVD 1X
	halAFE_RFLPFBW_DVD_9p6MHz  = 8,    // DVD 2X
	halAFE_RFLPFBW_DVD_18p0MHz = 1,    // DVD 4X
	halAFE_RFLPFBW_DVD_37p2MHz = 9,    // DVD 8X
	halAFE_RFLPFBW_DVD_54p0MHz = 16,    // DVD 12X
	halAFE_RFLPFBW_DVD_70p8MHz = 23,    // DVD 16X
	halAFE_RFLPFBW_DVD_80p4MHz = 27,    // DVD 18X
	halAFE_RFLPFBW_DVD_90p0MHz = 31,    // DVD 20X
	halAFE_RFLPFBW_BD_18p0MHz  = 1,    // BD 1X
	halAFE_RFLPFBW_BD_34p8MHz  = 8,    // BD 2X
	halAFE_RFLPFBW_BD_72p6MHz  = 4,    // BD 4X
	halAFE_RFLPFBW_BD_101p4MHz = 8,    // BD 6X
	halAFE_RFLPFBW_BD_137p4MHz = 13,    // BD 8X
	halAFE_RFLPFBW_BD_166p2MHz = 17,    // BD 10X
	halAFE_RFLPFBW_BD_202p2MHz = 22,    // BD 12X
	halAFE_RFLPFBW_BD_231p0MHz = 26     // BD 14X
} halAFE_RFLPFBW_t;

typedef enum
{
	halAFE_RFWrGN_N6dB = 0,    // RF coarse gain during writing: -6 dB
	halAFE_RFWrGN_0dB  = 1,    // RF coarse gain during writing: 0 dB
	halAFE_RFWrGN_6dB  = 2,    // RF coarse gain during writing: 6 dB
	halAFE_RFWrGN_12dB = 3     // RF coarse gain during writing: 12 dB
} halAFE_RFWrGN_t;

typedef enum
{
	halAFE_RFGNHDR_N6dB = 0,    // RF coarse gain for header fields: -6 dB
	halAFE_RFGNHDR_0dB  = 1,    // RF coarse gain for header fields: 0 dB
	halAFE_RFGNHDR_6dB  = 2,    // RF coarse gain for header fields: 6 dB
	halAFE_RFGNHDR_12dB = 3     // RF coarse gain for header fields: 12 dB
} halAFE_RFGNHDR_t;

typedef enum
{
	halAFE_RFGNLND_N6dB = 0,    // RF coarse gain for land data fields: -6 dB
	halAFE_RFGNLND_0dB  = 1,    // RF coarse gain for land data fields: 0 dB
	halAFE_RFGNLND_6dB  = 2,    // RF coarse gain for land data fields: 6 dB
	halAFE_RFGNLND_12dB = 3     // RF coarse gain for land data fields: 12 dB
} halAFE_RFGNLND_t;

typedef enum
{
	halAFE_RFGNGROV_N6dB = 0,    // RF coarse gain for groove data fields: -6 dB
	halAFE_RFGNGROV_0dB  = 1,    // RF coarse gain for groove data fields: 0 dB
	halAFE_RFGNGROV_6dB  = 2,    // RF coarse gain for groove data fields: 6 dB
	halAFE_RFGNGROV_12dB = 3     // RF coarse gain for groove data fields: 12 dB
} halAFE_RFGNGROV_t;

typedef enum
{
	halAFE_DPDINHPFBW_p2MHz = 0,    // DPD input high pass filter bandwidth: 0.2 MHz
	halAFE_DPDINHPFBW_2MHz  = 1,    // DPD input high pass filter bandwidth: 2 MHz
	halAFE_DPDINHPFBW_4MHz  = 2,    // DPD input high pass filter bandwidth: 4 MHz
	halAFE_DPDINHPFBW_6MHz  = 3     // DPD input high pass filter bandwidth: 6 MHz
} halAFE_DPDINHPFBW_t;

typedef enum
{
	halAFE_DPDGN_N6p0dB = 0,
	halAFE_DPDGN_N4p5dB = 1,
	halAFE_DPDGN_N3p0dB = 2,
	halAFE_DPDGN_N1p5dB = 3,
	halAFE_DPDGN_0dB    = 4,
	halAFE_DPDGN_1p5dB  = 5,
	halAFE_DPDGN_3p0dB  = 6,
	halAFE_DPDGN_4p5dB  = 7,
	halAFE_DPDGN_6p0dB  = 8,
	halAFE_DPDGN_7p5dB  = 9,
	halAFE_DPDGN_9p0dB  = 10,
	halAFE_DPDGN_10p5dB = 11,
	halAFE_DPDGN_12p0dB = 12,
	halAFE_DPDGN_13p5dB = 13,
	halAFE_DPDGN_15p0dB = 14,
	halAFE_DPDGN_16p5dB = 15 
} halAFE_DPDGN_t;

typedef enum
{
	halAFE_DPDEQBWH_0  = 0,
	halAFE_DPDEQBWH_1  = 1,
	halAFE_DPDEQBWH_2  = 2,
	halAFE_DPDEQBWH_3  = 3,
	halAFE_DPDEQBWH_4  = 4,
	halAFE_DPDEQBWH_5  = 5,
	halAFE_DPDEQBWH_6  = 6,
	halAFE_DPDEQBWH_7  = 7,
	halAFE_DPDEQBWH_8  = 8,
	halAFE_DPDEQBWH_9  = 9,
	halAFE_DPDEQBWH_10 = 10,
	halAFE_DPDEQBWH_11 = 11,
	halAFE_DPDEQBWH_12 = 12,
	halAFE_DPDEQBWH_13 = 13,
	halAFE_DPDEQBWH_14 = 14,
	halAFE_DPDEQBWH_15 = 15,
	halAFE_DPDEQBWH_16 = 16,
	halAFE_DPDEQBWH_17 = 17,
	halAFE_DPDEQBWH_18 = 18,
	halAFE_DPDEQBWH_19 = 19,
	halAFE_DPDEQBWH_20 = 20 
} halAFE_DPDEQBWH_t;

typedef enum
{
	halAFE_DPDEQBWL_0  = 0,
	halAFE_DPDEQBWL_1  = 1,
	halAFE_DPDEQBWL_2  = 2,
	halAFE_DPDEQBWL_3  = 3,
	halAFE_DPDEQBWL_4  = 4,
	halAFE_DPDEQBWL_5  = 5,
	halAFE_DPDEQBWL_6  = 6,
	halAFE_DPDEQBWL_7  = 7,
	halAFE_DPDEQBWL_8  = 8,
	halAFE_DPDEQBWL_9  = 9,
	halAFE_DPDEQBWL_10 = 10,
	halAFE_DPDEQBWL_11 = 11,
	halAFE_DPDEQBWL_12 = 12,
	halAFE_DPDEQBWL_13 = 13,
	halAFE_DPDEQBWL_14 = 14,
	halAFE_DPDEQBWL_15 = 15,
	halAFE_DPDEQBWL_16 = 16,
	halAFE_DPDEQBWL_17 = 17,
	halAFE_DPDEQBWL_18 = 18,
	halAFE_DPDEQBWL_19 = 19,
	halAFE_DPDEQBWL_20 = 20 
} halAFE_DPDEQBWL_t;

typedef enum
{
	halAFE_DPDEQGNBoost_5p6dB  = 1,    // DPD equalizer gain boost: 5.6 dB
	halAFE_DPDEQGNBoost_9p0dB  = 2,    // DPD equalizer gain boost: 9.0 dB
	halAFE_DPDEQGNBoost_11p4dB = 3,    // DPD equalizer gain boost: 11.4 dB
	halAFE_DPDEQGNBoost_13p4dB = 4,    // DPD equalizer gain boost: 13.4 dB
	halAFE_DPDEQGNBoost_14p9dB = 5,    // DPD equalizer gain boost: 14.9 dB
	halAFE_DPDEQGNBoost_16p3dB = 6,    // DPD equalizer gain boost: 16.3 dB
	halAFE_DPDEQGNBoost_17p4dB = 7     // DPD equalizer gain boost: 17.4 dB
} halAFE_DPDEQGNBoost_t;

typedef enum
{
	halAFE_DPDCPGN_6mVpDeg  = 0,    // DPD charge pump gain: 6 mV / degree
	halAFE_DPDCPGN_11mVpDeg = 1,    // DPD charge pump gain: 11 mV / degree
	halAFE_DPDCPGN_17mVpDeg = 2,    // DPD charge pump gain: 16 mV / degree
	halAFE_DPDCPGN_22mVpDeg = 3     // DPD charge pump gain: 22 mV / degree
} halAFE_DPDCPGN_t;

typedef enum
{
	halAFE_DPDCPFBW_50kHz  = 0,    // DPD charge pump filter Bandwidth: 50 kHz
	halAFE_DPDCPFBW_100kHz = 1,    // DPD charge pump filter Bandwidth: 100 kHz
	halAFE_DPDCPFBW_200kHz = 2,    // DPD charge pump filter Bandwidth: 200 kHz
	halAFE_DPDCPFBW_400kHz = 3     // DPD charge pump filter Bandwidth: 400 kHz
} halAFE_DPDCPFBW_t;

typedef enum
{
	halAFE_APCVref06DC_0p6V  = 0,    // APC VREF06 Signal DC Value: 0.6 V
	halAFE_APCVref06DC_0p64V = 1,    // APC VREF06 Signal DC Value: 0.64 V
	halAFE_APCVref06DC_0p68V = 2,    // APC VREF06 Signal DC Value: 0.68 V
	halAFE_APCVref06DC_0p72V = 3,    // APC VREF06 Signal DC Value: 0.72 V
	halAFE_APCVref06DC_0p76V = 4,    // APC VREF06 Signal DC Value: 0.76 V
	halAFE_APCVref06DC_0p8V  = 5,    // APC VREF06 Signal DC Value: 0.8 V
	halAFE_APCVref06DC_0p84V = 6,    // APC VREF06 Signal DC Value: 0.84 V
	halAFE_APCVref06DC_0p88V = 7,    // APC VREF06 Signal DC Value: 0.88 V
	halAFE_APCVref06DC_0p92V = 8,    // APC VREF06 Signal DC Value: 0.92 V
	halAFE_APCVref06DC_0p96V = 9,    // APC VREF06 Signal DC Value: 0.96 V
	halAFE_APCVref06DC_1V    = 10,    // APC VREF06 Signal DC Value: 1.0 V
	halAFE_APCVref06DC_1p04V = 11,    // APC VREF06 Signal DC Value: 1.04 V
	halAFE_APCVref06DC_1p08V = 12,    // APC VREF06 Signal DC Value: 1.08 V
	halAFE_APCVref06DC_1p12V = 13,    // APC VREF06 Signal DC Value: 1.12 V
	halAFE_APCVref06DC_1p16V = 14,    // APC VREF06 Signal DC Value: 1.16 V
	halAFE_APCVref06DC_1p2V  = 15     // APC VREF06 Signal DC Value: 1.2 V
} halAFE_APCVref06DC_t;

typedef enum
{
	halAFE_SubtBlkIN_S1A = 0,    // Select Subtraction Block Input S1A-VREF06
	halAFE_SubtBlkIN_S2A = 1,    // Select Subtraction Block Input S2A-VREF06
	halAFE_SubtBlkIN_S3A = 2,    // Select Subtraction Block Input S3A-VREF06
	halAFE_SubtBlkIN_S4A = 3     // Select Subtraction Block Input S4A-VREF06
} halAFE_SubtBlkIN_t;

typedef enum
{
	halAFE_RdAmpBlkIN_S1S2       = 0,    // Select Read Amplifier Block Input source S1-S2
	halAFE_RdAmpBlkIN_S3S4       = 1,    // Select Read Amplifier Block Input source S3-S4
	halAFE_RdAmpBlkIN_S4S2       = 2,    // Select Read Amplifier Block Input source S4-S2
	halAFE_RdAmpBlkIN_SUB_VREF06 = 3     // Select Read Amplifier Block Input source SUB-VREF06
} halAFE_RdAmpBlkIN_t;

typedef enum
{
	halAFE_WrAmpBlkIN_S1S2       = 0,    // Select Write Amplifier Block Input source S1-S2
	halAFE_WrAmpBlkIN_S3S4       = 1,    // Select Write Amplifier Block Input source S3-S4
	halAFE_WrAmpBlkIN_S4S1       = 2,    // Select Write Amplifier Block Input source S4-S1
	halAFE_WrAmpBlkIN_SUB_VREF06 = 3     // Select Write Amplifier Block Input source SUB-VREF06
} halAFE_WrAmpBlkIN_t;

typedef enum
{
	halAFE_APCT_M1PHSO_M2BHSO    = 0,    // Select APC MUX1=PHSO and APC MUX2=BHSO
	halAFE_APCT_M1VRPOW_M2VWPOW  = 1,    // Select APC MUX1=VRPOW and APC MUX2=VWPOW
	halAFE_APCT_M1VBG_M2SUB      = 2,    // Select APC MUX1=VBG and APC MUX2=SUB
	halAFE_APCT_M1VREF06_M2FVREF = 3,    // Select APC MUX1=VREF06 and APC MUX2=FVREF
	halAFE_APCT_M1S4_M2S3        = 4,    // Select APC MUX1=S4 and APC MUX2=S3
	halAFE_APCT_M1S3_M2S4        = 5,    // Select APC MUX1=S3 and APC MUX2=S4
	halAFE_APCT_M1S2_M2S1        = 6,    // Select APC MUX1=S2 and APC MUX2=S1
	halAFE_APCT_M1S1_M2S2        = 7     // Select APC MUX1=S1 and APC MUX2=S2
} halAFE_APCTestMUX_t;

typedef enum
{
	halAFE_FVREF_2p1V = 0,    // FVREF Output Voltage: 2.1 V
	halAFE_FVREF_2p3V = 1,    // FVREF Output Voltage: 2.3 V
	halAFE_FVREF_2p5V = 2,    // FVREF Output Voltage: 2.5 V
	halAFE_FVREF_2p7V = 3     // FVREF Output Voltage: 2.7 V
} halAFE_FVREFSEL_t;

typedef enum
{
	halAFE_RdWblSH_WBLRSH2_0 = 0,    // Wobble Read S/H Select WBLRSH2
	halAFE_RdWblSH_WBLRSH2_1 = 1,    // Wobble Read S/H Select WBLRSH2
	halAFE_RdWblSH_Low       = 2,    // Wobble Read S/H Select Low (continuous hold)
	halAFE_RdWblSH_High      = 3     // Wobble Read S/H Select High (continuous sample)
} halAFE_RdWblSH_t;

typedef enum
{
	halAFE_RGUP_GAINUP   = 0,
	halAFE_RGUP_WLDON    = 1,
	halAFE_RGUP_InvWLDON = 2,
	halAFE_RGUP_RGUPREG  = 3 
} halAFE_RGUPSEL_t;

typedef enum
{
	halAFE_WblWrADGN1_N12dB  = 0,    // Wobble write path AD gain -12 dB when WBLRSH = 1
	halAFE_WblWrADGN1_N9p5dB = 1,    // Wobble write path AD gain -9.5 dB when WBLRSH = 1
	halAFE_WblWrADGN1_N6p0dB = 2,    // Wobble write path AD gain -6.0 dB when WBLRSH = 1
	halAFE_WblWrADGN1_0dB    = 3     // Wobble write path AD gain 0 dB when WBLRSH = 1
} halAFE_WblWrADGN1_t;

typedef enum
{
	halAFE_WblWrADGN0_N12dB  = 0,    // Wobble write path AD gain -12 dB when WBLRSH = 0
	halAFE_WblWrADGN0_N9p5dB = 1,    // Wobble write path AD gain -9.5 dB when WBLRSH = 0
	halAFE_WblWrADGN0_N6p0dB = 2,    // Wobble write path AD gain -6.0 dB when WBLRSH = 0
	halAFE_WblWrADGN0_0dB    = 3     // Wobble write path AD gain 0 dB when WBLRSH = 0
} halAFE_WblWrADGN0_t;

typedef enum
{
	halAFE_WblRdADPGN1_0dB = 0,    // Wobble read path AD pre-gain 0 dB when RGUP = 1
	halAFE_WblRdADPGN1_2dB = 1,    // Wobble read path AD pre-gain 2 dB when RGUP = 1
	halAFE_WblRdADPGN1_4dB = 2,    // Wobble read path AD pre-gain 4 dB when RGUP = 1
	halAFE_WblRdADPGN1_6dB = 3     // Wobble read path AD pre-gain 6 dB when RGUP = 1
} halAFE_WblRdADPGN1_t;

typedef enum
{
	halAFE_WblRdADPGN0_0dB = 0,    // Wobble read path AD pre-gain 0 dB when RGUP = 0
	halAFE_WblRdADPGN0_2dB = 1,    // Wobble read path AD pre-gain 2 dB when RGUP = 0
	halAFE_WblRdADPGN0_4dB = 2,    // Wobble read path AD pre-gain 4 dB when RGUP = 0
	halAFE_WblRdADPGN0_6dB = 3     // Wobble read path AD pre-gain 6 dB when RGUP = 0
} halAFE_WblRdADPGN0_t;

typedef enum
{
	halAFE_WblWrBCGN1_N12dB  = 0,    // Wobble write path BC gain -12 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_N9p5dB = 1,    // Wobble write path BC gain -9.5 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_N6p0dB = 2,    // Wobble write path BC gain -6.0 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_0dB    = 3     // Wobble write path BC gain 0 dB when WBLRSH = 1
} halAFE_WblWrBCGN1_t;

typedef enum
{
	halAFE_WblWrBCGN0_N12dB  = 0,    // Wobble write path BC gain -12 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_N9p5dB = 1,    // Wobble write path BC gain -9.5 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_N6p0dB = 2,    // Wobble write path BC gain -6.0 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_0dB    = 3     // Wobble write path BC gain 0 dB when WBLRSH = 0
} halAFE_WblWrBCGN0_t;

typedef enum
{
	halAFE_WblRdBCPGN1_0dB = 0,    // Wobble read path BC gain 0 dB when RGUP = 1
	halAFE_WblRdBCPGN1_2dB = 1,    // Wobble read path BC gain 2 dB when RGUP = 1
	halAFE_WblRdBCPGN1_4dB = 2,    // Wobble read path BC gain 4 dB when RGUP = 1
	halAFE_WblRdBCPGN1_6dB = 3     // Wobble read path BC gain 6 dB when RGUP = 1
} halAFE_WblRdBCPGN1_t;

typedef enum
{
	halAFE_WblRdBCPGN0_0dB = 0,    // Wobble read path BC gain 0 dB when RGUP = 0
	halAFE_WblRdBCPGN0_2dB = 1,    // Wobble read path BC gain 2 dB when RGUP = 0
	halAFE_WblRdBCPGN0_4dB = 2,    // Wobble read path BC gain 4 dB when RGUP = 0
	halAFE_WblRdBCPGN0_6dB = 3     // Wobble read path BC gain 6 dB when RGUP = 0
} halAFE_WblRdBCPGN0_t;

typedef enum
{
	halAFE_WblRdADGN1_0dB  = 0,    // Wobble read path AD gain 0 dB when GAINUP = 1
	halAFE_WblRdADGN1_4dB  = 1,    // Wobble read path AD gain 4 dB when GAINUP = 1
	halAFE_WblRdADGN1_8dB  = 2,    // Wobble read path AD gain 8 dB when GAINUP = 1
	halAFE_WblRdADGN1_12dB = 3     // Wobble read path AD gain 12 dB when GAINUP = 1
} halAFE_WblRdADGN1_t;

typedef enum
{
	halAFE_WblRdADGN0_0dB  = 0,    // Wobble read path AD gain 0 dB when GAINUP = 0
	halAFE_WblRdADGN0_4dB  = 1,    // Wobble read path AD gain 4 dB when GAINUP = 0
	halAFE_WblRdADGN0_8dB  = 2,    // Wobble read path AD gain 8 dB when GAINUP = 0
	halAFE_WblRdADGN0_12dB = 3     // Wobble read path AD gain 12 dB when GAINUP = 0
} halAFE_WblRdADGN0_t;

typedef enum
{
	halAFE_WblWrADPGNB_0dB0   = 0,    // Wobble write path AD pre-gain 0 dB when WGUP = 1
	halAFE_WblWrADPGNB_N6dB   = 1,    // Wobble write path AD pre-gain -6 dB when WGUP = 1
	halAFE_WblWrADPGNB_N9p5dB = 2,    // Wobble write path AD pre-gain -9.5 dB when WGUP = 1
	halAFE_WblWrADPGNB_N12dB  = 3     // Wobble write path AD pre-gain -12 dB when WGUP = 1
} halAFE_WblWrADPGNB_t;

typedef enum
{
	halAFE_WblWrADPGNA_0dB0   = 0,    // Wobble write path AD pre-gain 0 dB when WGUP = 0
	halAFE_WblWrADPGNA_N6dB   = 1,    // Wobble write path AD pre-gain -6 dB when WGUP = 0
	halAFE_WblWrADPGNA_N9p5dB = 2,    // Wobble write path AD pre-gain -9.5 dB when WGUP = 0
	halAFE_WblWrADPGNA_N12dB  = 3     // Wobble write path AD pre-gain -12 dB when WGUP = 0
} halAFE_WblWrADPGNA_t;

typedef enum
{
	halAFE_WblRdBCGN1_0dB  = 0,    // Wobble read path BC gain 0 dB when GAINUP = 1
	halAFE_WblRdBCGN1_4dB  = 1,    // Wobble read path BC gain 4 dB when GAINUP = 1
	halAFE_WblRdBCGN1_8dB  = 2,    // Wobble read path BC gain 8 dB when GAINUP = 1
	halAFE_WblRdBCGN1_12dB = 3     // Wobble read path BC gain 12 dB when GAINUP = 1
} halAFE_WblRdBCGN1_t;

typedef enum
{
	halAFE_WblRdBCGN0_0dB  = 0,    // Wobble read path BC gain 0 dB when GAINUP = 0
	halAFE_WblRdBCGN0_4dB  = 1,    // Wobble read path BC gain 4 dB when GAINUP = 0
	halAFE_WblRdBCGN0_8dB  = 2,    // Wobble read path BC gain 8 dB when GAINUP = 0
	halAFE_WblRdBCGN0_12dB = 3     // Wobble read path BC gain 12 dB when GAINUP = 0
} halAFE_WblRdBCGN0_t;

typedef enum
{
	halAFE_WblWrBCPGNB_0dB0   = 0,    // Wobble write path BC pre-gain 0 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N6dB   = 1,    // Wobble write path BC pre-gain -6 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N9p5dB = 2,    // Wobble write path BC pre-gain -9.5 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N12dB  = 3     // Wobble write path BC pre-gain -12 dB when WGUP = 1
} halAFE_WblWrBCPGNB_t;

typedef enum
{
	halAFE_WblWrBCPGNA_0dB0   = 0,    // Wobble write path BC pre-gain 0 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N6dB   = 1,    // Wobble write path BC pre-gain -6 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N9p5dB = 2,    // Wobble write path BC pre-gain -9.5 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N12dB  = 3     // Wobble write path BC pre-gain -12 dB when WGUP = 0
} halAFE_WblWrBCPGNA_t;

typedef enum
{
	halAFE_WGUPSEL_GAINUP   = 0,
	halAFE_WGUPSEL_WLDON    = 1,
	halAFE_WGUPSEL_InvWLDON = 2,
	halAFE_WGUPSEL_WGUPREG  = 3 
} halAFE_WGUPSEL_t;

typedef enum
{
	halAFE_WBLWSH2SEL_WBLWSH    = 0,
	halAFE_WBLWSH2SEL_WBLRSH    = 1,
	halAFE_WBLWSH2SEL_InvWBLWSH = 2,
	halAFE_WBLWSH2SEL_InvWBLRSH = 3 
} halAFE_WBLWSH2SEL_t;

typedef enum
{
	halAFE_WOBDIGTPSEL_RGUP    = 0,    // Wobble Digital Test Point Select RGUP
	halAFE_WOBDIGTPSEL_RSH     = 1,    // Wobble Digital Test Point Select RSH
	halAFE_WOBDIGTPSEL_RSRC    = 2,    // Wobble Digital Test Point Select RSRC
	halAFE_WOBDIGTPSEL_WBLROUT = 3     // Wobble Digital Test Point Select WBLROUT
} halAFE_WOBDIGTPSEL_t;

typedef enum
{
	halAFE_WBLROUT_WBL_RSH2    = 0,    // Select Wobble Read Path Output Control WBL_RSH2
	halAFE_WBLROUT_InvWBL_WSH2 = 1,    // Select Wobble Read Path Output Control Inverted WBL_WSH2
	halAFE_WBLROUT_InvWLDON    = 2,    // Select Wobble Read Path Output Control Inverted WLDON
	halAFE_WBLROUT_GAINUP      = 3,    // Select Wobble Read Path Output Control GAINUP
	halAFE_WBLROUT_Low4        = 4,    // Select Wobble Read Path Output Control Always Low
	halAFE_WBLROUT_Low5        = 5,    // Select Wobble Read Path Output Control Always Low
	halAFE_WBLROUT_High6       = 6,    // Select Wobble Read Path Output Control Always High
	halAFE_WBLROUT_High7       = 7     // Select Wobble Read Path Output Control Always High
} halAFE_WBLROUT_t;

typedef enum
{
	halAFE_WblWrSH_WBL_WSH2_0 = 0,    // Select Wobble Write Path S/H Gate WBL_WSH2
	halAFE_WblWrSH_WBL_WSH2_1 = 1,    // Select Wobble Write Path S/H Gate WBL_WSH2
	halAFE_WblWrSH_Low        = 2,    // Select Wobble Write Path S/H Gate Low (continuous hold)
	halAFE_WblWrSH_High       = 3     // Select Wobble Write Path S/H Gate High (continuous sample)
} halAFE_WblWrSH_t;

typedef enum
{
	halAFE_WblS_rwRelWLDON   = 0,    // Read:A-D(if WLDON=0);WRF1 and WRF2(if WLDON=1).Write:A-D(if WLDON=0);WRF1 and WRF2(if WLDON=1).
	halAFE_WblS_rADwWRF12    = 1,    // Read:A-D; Write:WRF1 and WRF2
	halAFE_WblS_rADwAD       = 2,    // Read:A-D; Write:A-D
	halAFE_WblS_rWRF12wWRF12 = 3     // Read:WRF1 and WRF2; Write:WRF1 and WRF2
} halAFE_WblSource_t;

typedef enum
{
	halAFE_WBLRSH2IN_WBLWSH    = 0,
	halAFE_WBLRSH2IN_WBLRSH    = 1,
	halAFE_WBLRSH2IN_InvWBLWSH = 2,
	halAFE_WBLRSH2IN_InvWBLRSH = 3 
} halAFE_WBLRSH2IN_t;

typedef enum
{
	halAFE_WblLPF_75MHz = 0,    // Select Wobble Low Pass Filter 75 MHz
	halAFE_WblLPF_83MHz = 1,    // Select Wobble Low Pass Filter 83 MHz
	halAFE_WblLPF_68MHz = 2,    // Select Wobble Low Pass Filter 68 MHz
	halAFE_WblLPF_40MHz = 3,    // Select Wobble Low Pass Filter 40 MHz
	halAFE_WblLPF_18MHz = 4,    // Select Wobble Low Pass Filter 18 MHz
	halAFE_WblLPF_9MHz5 = 5,    // Select Wobble Low Pass Filter  9 MHz
	halAFE_WblLPF_9MHz6 = 6,    // Select Wobble Low Pass Filter  9 MHz
	halAFE_WblLPF_9MHz7 = 7     // Select Wobble Low Pass Filter  9 MHz
} halAFE_WblLPF_t;

typedef enum
{
	halAFE_WblWrOut_InvWBL_RSH2 = 0,
	halAFE_WblWrOut_WBL_WSH2    = 1,
	halAFE_WblWrOut_WLDON       = 2,
	halAFE_WblWrOut_InvGAINUP   = 3,
	halAFE_WblWrOut_Low4        = 4,
	halAFE_WblWrOut_Low5        = 5,
	halAFE_WblWrOut_High6       = 6,
	halAFE_WblWrOut_High7       = 7 
} halAFE_WblWrOut_t;

typedef enum
{
	halAFE_FirstLPFBefADCBW_1p1MHz  = 0,    // First Order Low Pass Filter Before ADC Bandwidth Setting 1.1 MHz
	halAFE_FirstLPFBefADCBW_5p1MHz  = 1,    // First Order Low Pass Filter Before ADC Bandwidth Setting 5.1 MHz
	halAFE_FirstLPFBefADCBW_9MHz    = 2,    // First Order Low Pass Filter Before ADC Bandwidth Setting 9 MHz
	halAFE_FirstLPFBefADCBW_11p9MHz = 3,    // First Order Low Pass Filter Before ADC Bandwidth Setting 11.9 MHz
	halAFE_FirstLPFBefADCBW_17p4MHz = 4,    // First Order Low Pass Filter Before ADC Bandwidth Setting 17.4 MHz
	halAFE_FirstLPFBefADCBW_20p2MHz = 5,    // First Order Low Pass Filter Before ADC Bandwidth Setting 20.2 MHz
	halAFE_FirstLPFBefADCBW_22p7MHz = 6,    // First Order Low Pass Filter Before ADC Bandwidth Setting 22.7 MHz
	halAFE_FirstLPFBefADCBW_27p4MHz = 7,    // First Order Low Pass Filter Before ADC Bandwidth Setting 27.4 MHz
	halAFE_FirstLPFBefADCBW_32p6MHz = 8,    // First Order Low Pass Filter Before ADC Bandwidth Setting 32.6 MHz
	halAFE_FirstLPFBefADCBW_35p2MHz = 9,    // First Order Low Pass Filter Before ADC Bandwidth Setting 35.2 MHz
	halAFE_FirstLPFBefADCBW_38p1MHz = 10,    // First Order Low Pass Filter Before ADC Bandwidth Setting 38.1 MHz
	halAFE_FirstLPFBefADCBW_41p5MHz = 11,    // First Order Low Pass Filter Before ADC Bandwidth Setting 41.5 MHz
	halAFE_FirstLPFBefADCBW_45p5MHz = 12,    // First Order Low Pass Filter Before ADC Bandwidth Setting 45.5 MHz
	halAFE_FirstLPFBefADCBW_50p3MHz = 13,    // First Order Low Pass Filter Before ADC Bandwidth Setting 50.3 MHz
	halAFE_FirstLPFBefADCBW_56p1MHz = 14,    // First Order Low Pass Filter Before ADC Bandwidth Setting 56.1 MHz
	halAFE_FirstLPFBefADCBW_63p1MHz = 15     // First Order Low Pass Filter Before ADC Bandwidth Setting 63.1 MHz
} halAFE_FirstLPFBefADCBW_t;

typedef enum
{
	halAFE_AGCHPF_100kHz   = 0,    // AGC1 and AGC2 Input High Pass Filter Control 100 kHz
	halAFE_AGCHPF_200kHz   = 1,    // AGC1 and AGC2 Input High Pass Filter Control 200 kHz
	halAFE_AGCHPF_400kHz   = 2,    // AGC1 and AGC2 Input High Pass Filter Control 400 kHz
	halAFE_AGCHPF_50kHz    = 3,    // AGC1 and AGC2 Input High Pass Filter Control 50 kHz (unused)
	halAFE_AGCHPF_700kHz   = 4,    // AGC1 and AGC2 Input High Pass Filter Control 700 kHz
	halAFE_AGCHPF_1000kHz  = 5,    // AGC1 and AGC2 Input High Pass Filter Control 1000 kHz
	halAFE_AGCHPF_1500kHz6 = 6,    // AGC1 and AGC2 Input High Pass Filter Control 1500 kHz
	halAFE_AGCHPF_1500kHz7 = 7     // AGC1 and AGC2 Input High Pass Filter Control 1500 kHz
} halAFE_AGCHPF_t;

typedef enum
{
	halAFE_AGCAmptdSetlTime_0p5ms  = 0,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.5 ms
	halAFE_AGCAmptdSetlTime_0p25ms = 1,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.25 ms
	halAFE_AGCAmptdSetlTime_0p06ms = 3,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.06 ms
	halAFE_AGCAmptdSetlTime_0p03ms = 7     // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.03 ms
} halAFE_AGCAmptdSetlTime_t;

typedef enum
{
	halAFE_WblSumAmp_N3p2dB = 0,    // Wobble Summing Amplifier Gain -3.2 dB
	halAFE_WblSumAmp_N0p3dB = 1,    // Wobble Summing Amplifier Gain -0.3 dB
	halAFE_WblSumAmp_3p2dB  = 2,    // Wobble Summing Amplifier Gain 3.2 dB
	halAFE_WblSumAmp_6p8dB  = 3,    // Wobble Summing Amplifier Gain 6.8 dB
	halAFE_WblSumAmp_9p5dB  = 4     // Wobble Summing Amplifier Gain 9.5 dB
} halAFE_WblSumAmp_t;

typedef enum
{
	halAFE_AGCCLIP1_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 1.6 Vpp
	halAFE_AGCCLIP1_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 1.0 Vpp
	halAFE_AGCCLIP1_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 0.4 Vpp
	halAFE_AGCCLIP1_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 0.2 Vpp
} halAFE_AGCCLIP1_t;

typedef enum
{
	halAFE_AGCCLIP2_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 1.6 Vpp
	halAFE_AGCCLIP2_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 1.0 Vpp
	halAFE_AGCCLIP2_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 0.4 Vpp
	halAFE_AGCCLIP2_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 0.2 Vpp
} halAFE_AGCCLIP2_t;

typedef enum
{
	halAFE_AGCCLIP3_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 1.6 Vpp
	halAFE_AGCCLIP3_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 1.0 Vpp
	halAFE_AGCCLIP3_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 0.4 Vpp
	halAFE_AGCCLIP3_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 0.2 Vpp
} halAFE_AGCCLIP3_t;

typedef enum
{
	halAFE_AFE_MIO_SEL0 = 0,    // monitor AVG_UPDATE_CLK TESTVAL2[6] TESTVAL2[5] TESTVAL2[4]
	halAFE_AFE_MIO_SEL1 = 1,    // monitor XTOK TX RC RX
	halAFE_AFE_MIO_SEL2 = 2,    // monitor TESTDIG1[3] TESTDIG1[2] TESTDIG1[1] TESTDIG1[0]
	halAFE_AFE_MIO_SEL3 = 3     // monitor TESTDIG2[3] TESTDIG2[2] TESTDIG2[1] TESTDIG2[0]
} halAFE_AFEDIGMIOSEL_t;

typedef enum
{
	halAFE_MUX3ADSEL_FE      = 0,
	halAFE_MUX3ADSEL_TE      = 1,
	halAFE_MUX3ADSEL_CE      = 2,
	halAFE_MUX3ADSEL_BS      = 3,
	halAFE_MUX3ADSEL_RP      = 4,
	halAFE_MUX3ADSEL_FE1     = 5,
	halAFE_MUX3ADSEL_TE1     = 6,
	halAFE_MUX3ADSEL_BS1     = 7,
	halAFE_MUX3ADSEL_RP1     = 8,
	halAFE_MUX3ADSEL_APCMUX1 = 9,
	halAFE_MUX3ADSEL_APCMUX2 = 10,
	halAFE_MUX3ADSEL_WOBMUX1 = 11,
	halAFE_MUX3ADSEL_WOBMUX2 = 12,
	halAFE_MUX3ADSEL_SAN     = 13,
	halAFE_MUX3ADSEL_SBN     = 14,
	halAFE_MUX3ADSEL_SCN     = 15,
	halAFE_MUX3ADSEL_SDN     = 16,
	halAFE_MUX3ADSEL_SEN     = 17,
	halAFE_MUX3ADSEL_SFN     = 18,
	halAFE_MUX3ADSEL_SGN     = 19,
	halAFE_MUX3ADSEL_SHN     = 20,
	halAFE_MUX3ADSEL_SIN     = 21,
	halAFE_MUX3ADSEL_SJN     = 22,
	halAFE_MUX3ADSEL_V125    = 23,
	halAFE_MUX3ADSEL_MUXOUT2 = 24,
	halAFE_MUX3ADSEL_SRFO    = 25,
	halAFE_MUX3ADSEL_VSS26   = 26,
	halAFE_MUX3ADSEL_VSS27   = 27,
	halAFE_MUX3ADSEL_VSS28   = 28,
	halAFE_MUX3ADSEL_VSS29   = 29,
	halAFE_MUX3ADSEL_VSS30   = 30,
	halAFE_MUX3ADSEL_VSS31   = 31 
} halAFE_MUX3ADSEL_t;

typedef enum
{
	halAFE_OPCPHDRateDec_Div32768 = 0,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^15)
	halAFE_OPCPHDRateDec_Div16384 = 1,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^14)
	halAFE_OPCPHDRateDec_Div8192  = 2,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^13)
	halAFE_OPCPHDRateDec_Div4096  = 3,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^12)
	halAFE_OPCPHDRateDec_Div2048  = 4,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^11)
	halAFE_OPCPHDRateDec_Div1024  = 5,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^10)
	halAFE_OPCPHDRateDec_Div512   = 6,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_OPCPHDRateDec_Div256   = 7,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_OPCPHDRateDec_Div128   = 8,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_OPCPHDRateDec_Div64    = 9,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_OPCPHDRateDec_Div32    = 10,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_OPCPHDRateDec_Div16    = 11,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_OPCPHDRateDec_Div8     = 12,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_OPCPHDRateDec_Div4     = 13,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_OPCPHDRateDec_Div2     = 14,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_OPCPHDRateDec_Div1     = 15     // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_OPCPHDRateDec_t;

typedef enum
{
	halAFE_MUXADSEL_PH      = 1,
	halAFE_MUXADSEL_MUXOUT3 = 2,
	halAFE_MUXADSEL_BH      = 3 
} halAFE_MUXADSEL_t;

typedef enum
{
	halAFE_OPCSRC_MUXTOAD1 = 0,
	halAFE_OPCSRC_MUXTOAD2 = 1 
} halAFE_OPCSRC_t;

typedef enum
{
	halAFE_OPCPHBaseRate_100mVpus = 0,
	halAFE_OPCPHBaseRate_50mVpus  = 1 
} halAFE_OPCBaseRate_t;

typedef enum
{
	halAFE_OPCPHBHDRateInc_1X = 0,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 1
	halAFE_OPCPHBHDRateInc_2X = 1,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 2
	halAFE_OPCPHBHDRateInc_4X = 2,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 4
	halAFE_OPCPHBHDRateInc_8X = 3     // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 8
} halAFE_OPCPHBHDRateInc_t;

#endif // (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halAFE_MUXTP1SEL0 = 0,    // DIGTP1(3)-DFCTMONO; DIGTP1(2)-DEFECT;   DIGTP1(1)-DEFECTI;   DIGTP1(0)-RPHLDDIG;
	halAFE_MUXTP1SEL1 = 1,    // DIGTP1(3)-TX;       DIGTP1(2)-XTOK;     DIGTP1(1)-TECP;      DIGTP1(0)-TECN;
	halAFE_MUXTP1SEL2 = 2,    // DIGTP1(3)-WLDON;    DIGTP1(2)-WLDONI;   DIGTP1(1)-GAINUP;    DIGTP1(0)-ATTON;
	halAFE_MUXTP1SEL3 = 3     // DIGTP1(3)-RFDIGTP1; DIGTP1(2)-RFDIGTP2; DIGTP1(1)-WOBDIGTP1; DIGTP1(0)-WOBDIGTP2;
} halAFE_MUXTP1SEL_t;

typedef enum
{
	halAFE_MUXTP2SEL0 = 0,    // DIGTP2(3)-BLANK; DIGTP2(2)-RC;        DIGTP2(1)-RPINVDIG; DIGTP2(0)-RPPOLDIG;
	halAFE_MUXTP2SEL1 = 1,    // DIGTP2(3)-RPCLK; DIGTP2(2)-HEADERHLD; DIGTP2(1)-HDRB;     DIGTP2(0)-SRVTEHLD;
	halAFE_MUXTP2SEL2 = 2,    // DIGTP2(3)-RC;    DIGTP1(2)-XTOK;      DIGTP2(1)-TX;       DIGTP2(0)-RX;
	halAFE_MUXTP2SEL3 = 3     // DIGTP2(3)-LAND;  DIGTP1(2)-LANDI;     DIGTP2(1)-NRST;     DIGTP2(0)-CLK33;
} halAFE_MUXTP2SEL_t;

typedef enum
{
	halAFE_MPXO1SEL_GPADC1 = 0,
	halAFE_MPXO1SEL_3p3V   = 1,
	halAFE_MPXO1SEL_0V     = 2,
	halAFE_MPXO1SEL_HIZ    = 3 
} halAFE_MPXO1SEL_t;

typedef enum
{
	halAFE_MUX1SEL_MPXO1       = 0,
	halAFE_MUX1SEL_MPPO        = 1,
	halAFE_MUX1SEL_FEO         = 2,
	halAFE_MUX1SEL_SRFO        = 3,
	halAFE_MUX1SEL_FE          = 4,
	halAFE_MUX1SEL_TE          = 5,
	halAFE_MUX1SEL_CE          = 6,
	halAFE_MUX1SEL_BS          = 7,
	halAFE_MUX1SEL_RP          = 8,
	halAFE_MUX1SEL_PHS         = 9,
	halAFE_MUX1SEL_PHSO        = 10,
	halAFE_MUX1SEL_TECPTHO     = 11,
	halAFE_MUX1SEL_VREFFCT     = 12,
	halAFE_MUX1SEL_APCMUX1     = 13,
	halAFE_MUX1SEL_WOBMUX1     = 14,
	halAFE_MUX1SEL_VREFI       = 15,
	halAFE_MUX1SEL_DPDOP       = 16,
	halAFE_MUX1SEL_RFTP1       = 17,
	halAFE_MUX1SEL_MUXOUT2     = 18,
	halAFE_MUX1SEL_RPPH        = 19,
	halAFE_MUX1SEL_RCTH1O      = 20,
	halAFE_MUX1SEL_WOBTESTP12V = 21,
	halAFE_MUX1SEL_WOBTESTP    = 22,
	halAFE_MUX1SEL_MUXOUT3     = 23,
	halAFE_MUX1SEL_SPPO1       = 24,
	halAFE_MUX1SEL_SPPO2       = 25,
	halAFE_MUX1SEL_SBAD        = 26,
	halAFE_MUX1SEL_TE1         = 27,
	halAFE_MUX1SEL_FE1         = 28,
	halAFE_MUX1SEL_BS1         = 29,
	halAFE_MUX1SEL_RP1         = 30,
	halAFE_MUX1SEL_TXO         = 31 
} halAFE_MUX1SEL_t;

typedef enum
{
	halAFE_MPXO2SEL_GPADC2 = 0,
	halAFE_MPXO2SEL_3p3V   = 1,
	halAFE_MPXO2SEL_0V     = 2,
	halAFE_MPXO2SEL_HIZ    = 3 
} halAFE_MPXO2SEL_t;

typedef enum
{
	halAFE_MUX2SEL_MPXO2       = 0,
	halAFE_MUX2SEL_SPPO1       = 1,
	halAFE_MUX2SEL_SPPO2       = 2,
	halAFE_MUX2SEL_SBAD        = 3,
	halAFE_MUX2SEL_FE          = 4,
	halAFE_MUX2SEL_TE          = 5,
	halAFE_MUX2SEL_CE          = 6,
	halAFE_MUX2SEL_BS          = 7,
	halAFE_MUX2SEL_RP          = 8,
	halAFE_MUX2SEL_BHS         = 9,
	halAFE_MUX2SEL_BHSO        = 10,
	halAFE_MUX2SEL_TECNTHO     = 11,
	halAFE_MUX2SEL_VREFBSRP    = 12,
	halAFE_MUX2SEL_APCMUX2     = 13,
	halAFE_MUX2SEL_WOBMUX2     = 14,
	halAFE_MUX2SEL_VREFB       = 15,
	halAFE_MUX2SEL_DPDON       = 16,
	halAFE_MUX2SEL_RFTP2       = 17,
	halAFE_MUX2SEL_MUXOUT3     = 18,
	halAFE_MUX2SEL_RPBH        = 19,
	halAFE_MUX2SEL_RCTH2O      = 20,
	halAFE_MUX2SEL_WOBTESTN12V = 21,
	halAFE_MUX2SEL_WOBTESTN    = 22,
	halAFE_MUX2SEL_MUXOUT1     = 23,
	halAFE_MUX2SEL_MPPO        = 24,
	halAFE_MUX2SEL_FEO         = 25,
	halAFE_MUX2SEL_SRFO        = 26,
	halAFE_MUX2SEL_TE2         = 27,
	halAFE_MUX2SEL_FE2         = 28,
	halAFE_MUX2SEL_BS2         = 29,
	halAFE_MUX2SEL_RP2         = 30,
	halAFE_MUX2SEL_RCTHO       = 31 
} halAFE_MUX2SEL_t;

typedef enum
{
	halAFE_MPXO3SEL_FE  = 0,
	halAFE_MPXO3SEL_3p3V = 1,
	halAFE_MPXO3SEL_0V   = 2,
	halAFE_MPXO3SEL_HIZ = 3 
} halAFE_MPXO3SEL_t;

#if (CHIP_REV <= 0xC0)

typedef enum
{
	halAFE_MUX3SEL_MPXO3        = 0,
	halAFE_MUX3SEL_TE           = 1,
	halAFE_MUX3SEL_CE           = 2,
	halAFE_MUX3SEL_BS           = 3,
	halAFE_MUX3SEL_RP           = 4,
	halAFE_MUX3SEL_FE1          = 5,
	halAFE_MUX3SEL_TE1          = 6,
	halAFE_MUX3SEL_BS1          = 7,
	halAFE_MUX3SEL_RP1          = 8,
	halAFE_MUX3SEL_APCMUX1      = 9,
	halAFE_MUX3SEL_APCMUX2      = 10,
	halAFE_MUX3SEL_WOBMUX1      = 11,
	halAFE_MUX3SEL_WOBMUX2      = 12,
	halAFE_MUX3SEL_SAN          = 13,
	halAFE_MUX3SEL_SBN          = 14,
	halAFE_MUX3SEL_SCN          = 15,
	halAFE_MUX3SEL_SDN          = 16,
	halAFE_MUX3SEL_SEN          = 17,
	halAFE_MUX3SEL_SFN          = 18,
	halAFE_MUX3SEL_SGN          = 19,
	halAFE_MUX3SEL_SHN          = 20,
	halAFE_MUX3SEL_SIN          = 21,
	halAFE_MUX3SEL_SJN          = 22,
	halAFE_MUX3SEL_MUXOUT2      = 23,
	halAFE_MUX3SEL_V125         = 24,
	halAFE_MUX3SEL_VINAO        = 25,
	halAFE_MUX3SEL_TESTI40UA   = 26,
	halAFE_MUX3SEL_VCTRLTBGPLL = 27,
	halAFE_MUX3SEL_VCTRLSYSPLL = 28,
	halAFE_MUX3SEL_VCTRLWPLL   = 29,
	halAFE_MUX3SEL_VCTRLADCPLL = 30,
	halAFE_MUX3SEL_VCTRLSRVPLL = 31 
} halAFE_MUX3SEL_t;

#endif // #if (CHIP_REV <= 0xC0)

#if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_MUX3SEL_MPXO3        = 0,
	halAFE_MUX3SEL_TE           = 1,
	halAFE_MUX3SEL_CE           = 2,
	halAFE_MUX3SEL_BS           = 3,
	halAFE_MUX3SEL_RP           = 4,
	halAFE_MUX3SEL_FE1          = 5,
	halAFE_MUX3SEL_TE1          = 6,
	halAFE_MUX3SEL_BS1          = 7,
	halAFE_MUX3SEL_VBG         = 8,
	halAFE_MUX3SEL_BETAPH      = 9,
	halAFE_MUX3SEL_BETABH      = 10,
	halAFE_MUX3SEL_WOBMUX1      = 11,
	halAFE_MUX3SEL_WOBMUX2      = 12,
	halAFE_MUX3SEL_SAN          = 13,
	halAFE_MUX3SEL_SBN          = 14,
	halAFE_MUX3SEL_SCN          = 15,
	halAFE_MUX3SEL_SDN          = 16,
	halAFE_MUX3SEL_SEN          = 17,
	halAFE_MUX3SEL_SFN          = 18,
	halAFE_MUX3SEL_SGN          = 19,
	halAFE_MUX3SEL_SHN          = 20,
	halAFE_MUX3SEL_SIN          = 21,
	halAFE_MUX3SEL_SJN          = 22,
	halAFE_MUX3SEL_MUXOUT2      = 23,
	halAFE_MUX3SEL_V125         = 24,
	halAFE_MUX3SEL_VINAO        = 25,
	halAFE_MUX3SEL_TESTI40UA   = 26,
	halAFE_MUX3SEL_VCTRLTBGPLL = 27,
	halAFE_MUX3SEL_VCTRLSYSPLL = 28,
	halAFE_MUX3SEL_VCTRLWPLL   = 29,
	halAFE_MUX3SEL_VCTRLADCPLL = 30,
	halAFE_MUX3SEL_VCTRLSRVPLL = 31 
} halAFE_MUX3SEL_t;

#endif // #if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_MUX1ADSEL_MPXO1       = 0,
	halAFE_MUX1ADSEL_MPPO        = 1,
	halAFE_MUX1ADSEL_FEO         = 2,
	halAFE_MUX1ADSEL_SRFO        = 3,
	halAFE_MUX1ADSEL_FE          = 4,
	halAFE_MUX1ADSEL_TE          = 5,
	halAFE_MUX1ADSEL_CE          = 6,
	halAFE_MUX1ADSEL_BS          = 7,
	halAFE_MUX1ADSEL_RP          = 8,
	halAFE_MUX1ADSEL_PHS         = 9,
	halAFE_MUX1ADSEL_PHSO        = 10,
	halAFE_MUX1ADSEL_TECPTHO     = 11,
	halAFE_MUX1ADSEL_VREFFCT     = 12,
	halAFE_MUX1ADSEL_APCMUX1     = 13,
	halAFE_MUX1ADSEL_WOBMUX1     = 14,
	halAFE_MUX1ADSEL_VREFI       = 15,
	halAFE_MUX1ADSEL_DPDOP       = 16,
	halAFE_MUX1ADSEL_RFTP1       = 17,
	halAFE_MUX1ADSEL_MUXOUT2     = 18,
	halAFE_MUX1ADSEL_RPPH        = 19,
	halAFE_MUX1ADSEL_RCTH1O      = 20,
	halAFE_MUX1ADSEL_WOBTESTP12V = 21,
	halAFE_MUX1ADSEL_WOBTESTP    = 22,
	halAFE_MUX1ADSEL_MUXOUT3     = 23,
	halAFE_MUX1ADSEL_SPPO1       = 24,
	halAFE_MUX1ADSEL_SPPO2       = 25,
	halAFE_MUX1ADSEL_SBAD        = 26,
	halAFE_MUX1ADSEL_TE1         = 27,
	halAFE_MUX1ADSEL_FE1         = 28,
	halAFE_MUX1ADSEL_BS1         = 29,
	halAFE_MUX1ADSEL_RP1         = 30,
	halAFE_MUX1ADSEL_TXO         = 31 
} halAFE_MUX1ADSEL_t;

typedef enum
{
	halAFE_MUX2ADSEL_MPXO2       = 0,
	halAFE_MUX2ADSEL_SPPO1       = 1,
	halAFE_MUX2ADSEL_SPPO2       = 2,
	halAFE_MUX2ADSEL_SBAD        = 3,
	halAFE_MUX2ADSEL_FE          = 4,
	halAFE_MUX2ADSEL_TE          = 5,
	halAFE_MUX2ADSEL_CE          = 6,
	halAFE_MUX2ADSEL_BS          = 7,
	halAFE_MUX2ADSEL_RP          = 8,
	halAFE_MUX2ADSEL_BHS         = 9,
	halAFE_MUX2ADSEL_BHSO        = 10,
	halAFE_MUX2ADSEL_TECNTHO     = 11,
	halAFE_MUX2ADSEL_VREFBSRP    = 12,
	halAFE_MUX2ADSEL_APCMUX2     = 13,
	halAFE_MUX2ADSEL_WOBMUX2     = 14,
	halAFE_MUX2ADSEL_VREFB       = 15,
	halAFE_MUX2ADSEL_DPDON       = 16,
	halAFE_MUX2ADSEL_RFTP2       = 17,
	halAFE_MUX2ADSEL_MUXOUT3     = 18,
	halAFE_MUX2ADSEL_RPBH        = 19,
	halAFE_MUX2ADSEL_RCTH2O      = 20,
	halAFE_MUX2ADSEL_WOBTESTN12V = 21,
	halAFE_MUX2ADSEL_WOBTESTN    = 22,
	halAFE_MUX2ADSEL_MUXOUT1     = 23,
	halAFE_MUX2ADSEL_MPPO        = 24,
	halAFE_MUX2ADSEL_FEO         = 25,
	halAFE_MUX2ADSEL_SRFO        = 26,
	halAFE_MUX2ADSEL_TE2         = 27,
	halAFE_MUX2ADSEL_FE2         = 28,
	halAFE_MUX2ADSEL_BS2         = 29,
	halAFE_MUX2ADSEL_RP2         = 30,
	halAFE_MUX2ADSEL_RCTHO       = 31 
} halAFE_MUX2ADSEL_t;

typedef enum
{
	halAFE_PHDRateDec_Div32768 = 0,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^15)
	halAFE_PHDRateDec_Div16384 = 1,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^14)
	halAFE_PHDRateDec_Div8192  = 2,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^13)
	halAFE_PHDRateDec_Div4096  = 3,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^12)
	halAFE_PHDRateDec_Div2048  = 4,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^11)
	halAFE_PHDRateDec_Div1024  = 5,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^10)
	halAFE_PHDRateDec_Div512   = 6,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_PHDRateDec_Div256   = 7,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_PHDRateDec_Div128   = 8,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_PHDRateDec_Div64    = 9,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_PHDRateDec_Div32    = 10,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_PHDRateDec_Div16    = 11,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_PHDRateDec_Div8     = 12,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_PHDRateDec_Div4     = 13,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_PHDRateDec_Div2     = 14,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_PHDRateDec_Div1     = 15     // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_PHDRateDec_t;

typedef enum
{
	halAFE_BHDRateDec_Div32768 = 0,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^15)
	halAFE_BHDRateDec_Div16384 = 1,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^14)
	halAFE_BHDRateDec_Div8192  = 2,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^13)
	halAFE_BHDRateDec_Div4096  = 3,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^12)
	halAFE_BHDRateDec_Div2048  = 4,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^11)
	halAFE_BHDRateDec_Div1024  = 5,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^10)
	halAFE_BHDRateDec_Div512   = 6,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_BHDRateDec_Div256   = 7,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_BHDRateDec_Div128   = 8,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_BHDRateDec_Div64    = 9,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_BHDRateDec_Div32    = 10,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_BHDRateDec_Div16    = 11,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_BHDRateDec_Div8     = 12,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_BHDRateDec_Div4     = 13,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_BHDRateDec_Div2     = 14,    // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_BHDRateDec_Div1     = 15     // Decrease Short Term B/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_BHDRateDec_t;

typedef enum
{
	halAFE_BSPGN_N6dB = 0,    // BSPGN -6 dB
	halAFE_BSPGN_0dB  = 1,    // BSPGN 0 dB
	halAFE_BSPGN_6dB  = 2,    // BSPGN 6 dB
	halAFE_BSPGN_12dB = 3     // BSPGN 12 dB
} halAFE_BSPGN_t;

typedef enum
{
	halAFE_BSLPF_300kHz = 0,
	halAFE_BSLPF_150kHz = 1,
	halAFE_BSLPF_50kHz  = 2,
	halAFE_BSLPF_15kHz  = 3 
} halAFE_BSLPF_t;

typedef enum
{
	halAFE_RPPGN_N6dB = 0,    // RPPGN -6 dB
	halAFE_RPPGN_0dB  = 1,    // RPPGN 0 dB
	halAFE_RPPGN_6dB  = 2,    // RPPGN 6 dB
	halAFE_RPPGN_12dB = 3     // RPPGN 12 dB
} halAFE_RPPGN_t;

typedef enum
{
	halAFE_RPDefectDelay_1us   = 0,    // Extend Defect Flag that Holds RP Peak/Bottom Hold Signals: 1us
	halAFE_RPDefectDelay_30us  = 1,    // Extend Defect Flag that Holds RP Peak/Bottom Hold Signals: 30us
	halAFE_RPDefectDelay_60us  = 2,    // Extend Defect Flag that Holds RP Peak/Bottom Hold Signals: 60us
	halAFE_RPDefectDelay_120us = 3     // Extend Defect Flag that Holds RP Peak/Bottom Hold Signals: 120us
} halAFE_RPDefectDelay_t;

typedef enum
{
	halAFE_RPLPF_300kHz = 0,
	halAFE_RPLPF_150kHz = 1,
	halAFE_RPLPF_50kHz  = 2,
	halAFE_RPLPF_15kHz  = 3 
} halAFE_RPLPF_t;

typedef enum
{
	halAFE_PHBaseRate_680mVpus = 0,
	halAFE_PHBaseRate_370mVpus = 1 
} halAFE_PHBaseRate_t;

typedef enum
{
	halAFE_PHDRateInc_1X = 0,    // Increase Short Term P/H Droop Rate by a factor of 1
	halAFE_PHDRateInc_2X = 1,    // Increase Short Term P/H Droop Rate by a factor of 2
	halAFE_PHDRateInc_4X = 2,    // Increase Short Term P/H Droop Rate by a factor of 4
	halAFE_PHDRateInc_8X = 3     // Increase Short Term P/H Droop Rate by a factor of 8
} halAFE_PHDRateInc_t;

typedef enum
{
	halAFE_BHBaseRate_680mVpus = 0,
	halAFE_BHBaseRate_370mVpus = 1 
} halAFE_BHBaseRate_t;

typedef enum
{
	halAFE_BHDRateInc_1X = 0,    // Increase Short Term B/H Droop Rate by a factor of 1
	halAFE_BHDRateInc_2X = 1,    // Increase Short Term B/H Droop Rate by a factor of 2
	halAFE_BHDRateInc_4X = 2,    // Increase Short Term B/H Droop Rate by a factor of 4
	halAFE_BHDRateInc_8X = 3     // Increase Short Term B/H Droop Rate by a factor of 8
} halAFE_BHDRateInc_t;

typedef enum
{
	halAFE_FE2GnSEL_FE2GNREG = 0,    // FE2 gain set by FE2GNREG(4-0)
	halAFE_FE2GnSEL_AvgSRV   = 1     // FE2 gain set by the average servo loop
} halAFE_FE2GnSEL_t;

typedef enum
{
	halAFE_TE2GnSEL_FE2GNREG = 0,    // TE2 gain set by TE2GNREG(4-0)
	halAFE_TE2GnSEL_AvgSRV   = 1     // TE2 gain set by the average servo loop
} halAFE_TE2GnSEL_t;

typedef enum
{
	halAFE_TOKT_240us  = 0,    // TOK Time = 240 us
	halAFE_TOKT_480us  = 1,    // TOK Time = 480 us
	halAFE_TOKT_720us  = 2,    // TOK Time = 720 us
	halAFE_TOKT_960us  = 3,    // TOK Time = 960 us
	halAFE_TOKT_1200us = 4,    // TOK Time = 1200 us
	halAFE_TOKT_1440us = 5,    // TOK Time = 1440 us
	halAFE_TOKT_1680us = 6,    // TOK Time = 1680 us
	halAFE_TOKT_1920us = 7     // TOK Time = 1920 us
} halAFE_TOKT_t;

typedef enum
{
	halAFE_XTOKSEL_TECPaaTECN = 0,    // XTOK = TECP && TECN
	halAFE_XTOKSEL_TECPooTECN = 1     // XTOK = TECP || TECN
} halAFE_XTOKSEL_t;

typedef enum
{
	halAFE_FEPGN_N6dB = 0,    // FEPGN -6 dB
	halAFE_FEPGN_0dB  = 1,    // FEPGN 0 dB
	halAFE_FEPGN_6dB  = 2,    // FEPGN 6 dB
	halAFE_FEPGN_12dB = 3     // FEPGN 12 dB
} halAFE_FEPGN_t;

typedef enum
{
	halAFE_FELPF_300kHz = 0,
	halAFE_FELPF_150kHz = 1,
	halAFE_FELPF_50kHz  = 2,
	halAFE_FELPF_15kHz  = 3 
} halAFE_FELPF_t;

typedef enum
{
	halAFE_CEPGN_N6dB = 0,    // CEPGN -6 dB
	halAFE_CEPGN_0dB  = 1,    // CEPGN 0 dB
	halAFE_CEPGN_6dB  = 2,    // CEPGN 6 dB
	halAFE_CEPGN_12dB = 3     // CEPGN 12 dB
} halAFE_CEPGN_t;

typedef enum
{
	halAFE_CELPF_300kHz = 0,
	halAFE_CELPF_150kHz = 1,
	halAFE_CELPF_50kHz  = 2,
	halAFE_CELPF_20kHz  = 3 
} halAFE_CELPF_t;

typedef enum
{
	halAFE_TEPGN_N6dB = 0,    // TEPGN -6 dB
	halAFE_TEPGN_0dB  = 1,    // TEPGN 0 dB
	halAFE_TEPGN_6dB  = 2,    // TEPGN 6 dB
	halAFE_TEPGN_12dB = 3     // TEPGN 12 dB
} halAFE_TEPGN_t;

typedef enum
{
	halAFE_TELPF_300kHz = 0,
	halAFE_TELPF_150kHz = 1,
	halAFE_TELPF_50kHz  = 2,
	halAFE_TELPF_15kHz  = 3 
} halAFE_TELPF_t;

typedef enum
{
	halAFE_TXLPF_300kHz = 0,
	halAFE_TXLPF_150kHz = 1,
	halAFE_TXLPF_50kHz  = 2,
	halAFE_TXLPF_15kHz  = 3 
} halAFE_TXLPF_t;

typedef enum
{
	halAFE_TECP_TECN_LVL_0mV    = 0,    // TECP/TECN LVL = 1.65V +/- 0mV
	halAFE_TECP_TECN_LVL_50mV   = 1,    // TECP/TECN LVL = 1.65V +/- 50mV
	halAFE_TECP_TECN_LVL_100mV  = 2,    // TECP/TECN LVL = 1.65V +/- 100mV
	halAFE_TECP_TECN_LVL_150mV  = 3,    // TECP/TECN LVL = 1.65V +/- 150mV
	halAFE_TECP_TECN_LVL_200mV  = 4,    // TECP/TECN LVL = 1.65V +/- 200mV
	halAFE_TECP_TECN_LVL_250mV  = 5,    // TECP/TECN LVL = 1.65V +/- 250mV
	halAFE_TECP_TECN_LVL_300mV  = 6,    // TECP/TECN LVL = 1.65V +/- 300mV
	halAFE_TECP_TECN_LVL_350mV  = 7,    // TECP/TECN LVL = 1.65V +/- 350mV
	halAFE_TECP_TECN_LVL_400mV  = 8,    // TECP/TECN LVL = 1.65V +/- 400mV
	halAFE_TECP_TECN_LVL_450mV  = 9,    // TECP/TECN LVL = 1.65V +/- 450mV
	halAFE_TECP_TECN_LVL_500mV  = 10,    // TECP/TECN LVL = 1.65V +/- 500mV
	halAFE_TECP_TECN_LVL_550mV  = 11,    // TECP/TECN LVL = 1.65V +/- 550mV
	halAFE_TECP_TECN_LVL_600mV  = 12,    // TECP/TECN LVL = 1.65V +/- 600mV
	halAFE_TECP_TECN_LVL_650mV  = 13,    // TECP/TECN LVL = 1.65V +/- 650mV
	halAFE_TECP_TECN_LVL_700mV  = 14,    // TECP/TECN LVL = 1.65V +/- 700mV
	halAFE_TECP_TECN_LVL_750mV  = 15,    // TECP/TECN LVL = 1.65V +/- 750mV
	halAFE_TECP_TECN_LVL_800mV  = 16,    // TECP/TECN LVL = 1.65V +/- 800mV
	halAFE_TECP_TECN_LVL_850mV  = 17,    // TECP/TECN LVL = 1.65V +/- 850mV
	halAFE_TECP_TECN_LVL_900mV  = 18,    // TECP/TECN LVL = 1.65V +/- 900mV
	halAFE_TECP_TECN_LVL_950mV  = 19,    // TECP/TECN LVL = 1.65V +/- 950mV
	halAFE_TECP_TECN_LVL_1000mV = 20,    // TECP/TECN LVL = 1.65V +/- 1000mV
	halAFE_TECP_TECN_LVL_1050mV = 21,    // TECP/TECN LVL = 1.65V +/- 1050mV
	halAFE_TECP_TECN_LVL_1100mV = 22,    // TECP/TECN LVL = 1.65V +/- 1100mV
	halAFE_TECP_TECN_LVL_1150mV = 23,    // TECP/TECN LVL = 1.65V +/- 1150mV
	halAFE_TECP_TECN_LVL_1200mV = 24,    // TECP/TECN LVL = 1.65V +/- 1200mV
	halAFE_TECP_TECN_LVL_1250mV = 25,    // TECP/TECN LVL = 1.65V +/- 1250mV
	halAFE_TECP_TECN_LVL_1300mV = 26,    // TECP/TECN LVL = 1.65V +/- 1300mV
	halAFE_TECP_TECN_LVL_1350mV = 27,    // TECP/TECN LVL = 1.65V +/- 1350mV
	halAFE_TECP_TECN_LVL_1400mV = 28,    // TECP/TECN LVL = 1.65V +/- 1400mV
	halAFE_TECP_TECN_LVL_1450mV = 29,    // TECP/TECN LVL = 1.65V +/- 1450mV
	halAFE_TECP_TECN_LVL_1500mV = 30,    // TECP/TECN LVL = 1.65V +/- 1500mV
	halAFE_TECP_TECN_LVL_1550mV = 31     // TECP/TECN LVL = 1.65V +/- 1550mV
} halAFE_TOKLVL_t;

typedef enum
{
	halAFE_GUPDLY_60p5us = 0,
	halAFE_GUPDLY_121us  = 1,
	halAFE_GUPDLY_242us  = 2,
	halAFE_GUPDLY_484us  = 3 
} halAFE_GUPDLY_t;

typedef enum
{
	halAFE_MBSBBW_50MHz  = 1,
	halAFE_MBSBBW_70MHz  = 2,
	halAFE_MBSBBW_100MHz = 3 
} halAFE_MBSBBW_t;

typedef enum
{
	halAFE_MPPOPGN_N6dB = 0,    // MPPOPGN -6 dB
	halAFE_MPPOPGN_0dB  = 1,    // MPPOPGN 0 dB
	halAFE_MPPOPGN_6dB  = 2,    // MPPOPGN 6 dB
	halAFE_MPPOPGN_12dB = 3     // MPPOPGN 12 dB
} halAFE_MPPOPGN_t;

typedef enum
{
	halAFE_MPPOLPF_330kHz = 0,
	halAFE_MPPOLPF_660kHz = 1 
} halAFE_MPPOLPF_t;

typedef enum
{
	halAFE_FEOPGN_N6dB = 0,    // FEOPGN -6 dB
	halAFE_FEOPGN_0dB  = 1,    // FEOPGN 0 dB
	halAFE_FEOPGN_6dB  = 2,    // FEOPGN 6 dB
	halAFE_FEOPGN_12dB = 3     // FEOPGN 12 dB
} halAFE_FEOPGN_t;

typedef enum
{
	halAFE_FEOLPF_330kHz = 0,
	halAFE_FEOLPF_660kHz = 1 
} halAFE_FEOLPF_t;

typedef enum
{
	halAFE_TestLPF_1400kHz = 0,
	halAFE_TestLPF_700kHz  = 1,
	halAFE_TestLPF_200kHz  = 2,
	halAFE_TestLPF_60kHz   = 3 
} halAFE_TestLPF_t;

typedef enum
{
	halAFE_SRFOLPF_330kHz = 0,
	halAFE_SRFOLPF_660kHz = 1 
} halAFE_SRFOLPF_t;

typedef enum
{
	halAFE_SPPO1WrGN_N6dB = 0,    // SPPO1 gain while writing: -6 dB
	halAFE_SPPO1WrGN_0dB  = 1,    // SPPO1 gain while writing: 0 dB
	halAFE_SPPO1WrGN_6dB  = 2,    // SPPO1 gain while writing: 6 dB
	halAFE_SPPO1WrGN_12dB = 3     // SPPO1 gain while writing: 12 dB
} halAFE_SPPO1WrGN_t;

typedef enum
{
	halAFE_SPPO1RdGN_N6dB = 0,    // SPPO1 gain while reading: -6 dB
	halAFE_SPPO1RdGN_0dB  = 1,    // SPPO1 gain while reading: 0 dB
	halAFE_SPPO1RdGN_6dB  = 2,    // SPPO1 gain while reading: 6 dB
	halAFE_SPPO1RdGN_12dB = 3     // SPPO1 gain while reading: 12 dB
} halAFE_SPPO1RdGN_t;

typedef enum
{
	halAFE_SPPO2WrGN_N6dB = 0,    // SPPO2 gain while writing: -6 dB
	halAFE_SPPO2WrGN_0dB  = 1,    // SPPO2 gain while writing: 0 dB
	halAFE_SPPO2WrGN_6dB  = 2,    // SPPO2 gain while writing: 6 dB
	halAFE_SPPO2WrGN_12dB = 3     // SPPO2 gain while writing: 12 dB
} halAFE_SPPO2WrGN_t;

typedef enum
{
	halAFE_SPPO2RdGN_N6dB = 0,    // SPPO2 gain while reading: -6 dB
	halAFE_SPPO2RdGN_0dB  = 1,    // SPPO2 gain while reading: 0 dB
	halAFE_SPPO2RdGN_6dB  = 2,    // SPPO2 gain while reading: 6 dB
	halAFE_SPPO2RdGN_12dB = 3     // SPPO2 gain while reading: 12 dB
} halAFE_SPPO2RdGN_t;

typedef enum
{
	halAFE_SPPO1LPF_330kHz = 0,
	halAFE_SPPO1LPF_660kHz = 1 
} halAFE_SPPO1LPF_t;

typedef enum
{
	halAFE_SPPO2LPF_330kHz = 0,
	halAFE_SPPO2LPF_660kHz = 1 
} halAFE_SPPO2LPF_t;

typedef enum
{
	halAFE_SBADLPF_330kHz = 0,
	halAFE_SBADLPF_660kHz = 1 
} halAFE_SBADLPF_t;

typedef enum
{
	halAFE_VREFBSEL_1p5V  = 0,
	halAFE_VREFBSEL_1p65V = 1,
	halAFE_VREFBSEL_1p8V  = 2,
	halAFE_VREFBSEL_1p95V = 3,
	halAFE_VREFBSEL_2p1V  = 4,
	halAFE_VREFBSEL_2p25V = 5,
	halAFE_VREFBSEL_2p4V  = 6,
	halAFE_VREFBSEL_2p55V = 7 
} halAFE_VREFBSEL_t;

typedef enum
{
	halAFE_EQSVOSEL0 = 0,    // EQDOP Output=3.3V    EQDON Output=0V
	halAFE_EQSVOSEL1 = 1,    // EQDOP Output=MUXOUT1 EQDON Output=MUXOUT2
	halAFE_EQSVOSEL2 = 2,    // EQDOP Output=IBIAS     EQDON Output=RFVCMAD
	halAFE_EQSVOSEL3 = 3     // EQDOP Output=DPDCPTSTP EQDON Output=DPDCPTSTN
} halAFE_EQSVOSEL_t;

typedef enum
{
	halAFE_EQDPDSEL0 = 0,    // EQDOP Output=DPD4P EQDON Output=DPD4N
	halAFE_EQDPDSEL1 = 1,    // EQDOP Output=DPD1P EQDON Output=DPD1N
	halAFE_EQDPDSEL2 = 2,    // EQDOP Output=DPD2P EQDON Output=DPD2N
	halAFE_EQDPDSEL3 = 3     // EQDOP Output=DPD3P EQDON Output=DPD3N
} halAFE_EQDPDSEL_t;

typedef enum
{
	halAFE_RFANATSEL0 = 0,    // EQDOP Output=Off    EQDON Output=Off
	halAFE_RFANATSEL1 = 1,    // EQDOP Output=RF5VP  EQDON Output=RF5VN
	halAFE_RFANATSEL2 = 2,    // EQDOP Output=RFVGAP EQDON Output=RFVGAN
	halAFE_RFANATSEL3 = 3     // EQDOP Output=RFAGCP EQDON Output=RFAGCN
} halAFE_RFANATSEL_t;

typedef enum
{
	halAFE_DPDTSEL0 = 0,    // RFDIGTP1=DPDP1   RFDIGTP2=DPDP2
	halAFE_DPDTSEL1 = 1,    // RFDIGTP1=DPD12UP RFDIGTP2=DPD12DN
	halAFE_DPDTSEL2 = 2,    // RFDIGTP1=DPDP3   RFDIGTP2=DPDP4
	halAFE_DPDTSEL3 = 3     // RFDIGTP1=DPD34UP RFDIGTP2=DPD34DN
} halAFE_DPDTSEL_t;

typedef enum
{
	halAFE_EQDOSEL0 = 0,    // EQDOP Output=DPDVCM      EQDON Output=RFRP
	halAFE_EQDOSEL1 = 1,    // EQDOP EQDON Output Depends on EQSVOSEL
	halAFE_EQDOSEL2 = 2,    // EQDOP Output=WOBTESTP    EQDON Output=WOBTESTN
	halAFE_EQDOSEL3 = 3,    // EQDOP Output=WOBTESTP12V EQDON Output=WOBTESTN12V
	halAFE_EQDOSEL4 = 4,    // EQDOP Output=ADCTPP      EQDON Output=ADCTPN
	halAFE_EQDOSEL5 = 5,    // EQDOP EQDON Output Depends on RFANATSEL
	halAFE_EQDOSEL6 = 6,    // EQDOP Output=RFOP        EQDON Output=RFON
	halAFE_EQDOSEL7 = 7     // EQDOP EQDON Output Depends on EQDPDSEL
} halAFE_EQDOSEL_t;

typedef enum
{
	halAFE_RFINSEL0 = 0,    // RF Input=BDRFP-BDRFN
	halAFE_RFINSEL1 = 1,    // RF Input=DVDRFP-DVDRFN
	halAFE_RFINSEL2 = 2,    // RF Input=WPP1WRF
	halAFE_RFINSEL3 = 3     // RF Input=SRFO
} halAFE_RFINSEL_t;

typedef enum
{
	halAFE_RFAGCGN_N8p0dB = 0,
	halAFE_RFAGCGN_N7p5dB = 1,
	halAFE_RFAGCGN_N7p0dB = 2,
	halAFE_RFAGCGN_N6p5dB = 3,
	halAFE_RFAGCGN_N6p0dB = 4,
	halAFE_RFAGCGN_N5p5dB = 5,
	halAFE_RFAGCGN_N5p0dB = 6,
	halAFE_RFAGCGN_N4p5dB = 7,
	halAFE_RFAGCGN_N4p0dB = 8,
	halAFE_RFAGCGN_N3p5dB = 9,
	halAFE_RFAGCGN_N3p0dB = 10,
	halAFE_RFAGCGN_N2p5dB = 11,
	halAFE_RFAGCGN_N2p0dB = 12,
	halAFE_RFAGCGN_N1p5dB = 13,
	halAFE_RFAGCGN_N1p0dB = 14,
	halAFE_RFAGCGN_N0p5dB = 15,
	halAFE_RFAGCGN_0dB    = 16,
	halAFE_RFAGCGN_0p5dB  = 17,
	halAFE_RFAGCGN_1p0dB  = 18,
	halAFE_RFAGCGN_1p5dB  = 19,
	halAFE_RFAGCGN_2p0dB  = 20,
	halAFE_RFAGCGN_2p5dB  = 21,
	halAFE_RFAGCGN_3p0dB  = 22,
	halAFE_RFAGCGN_3p5dB  = 23,
	halAFE_RFAGCGN_4p0dB  = 24,
	halAFE_RFAGCGN_4p5dB  = 25,
	halAFE_RFAGCGN_5p0dB  = 26,
	halAFE_RFAGCGN_5p5dB  = 27,
	halAFE_RFAGCGN_6p0dB  = 28,
	halAFE_RFAGCGN_6p5dB  = 29,
	halAFE_RFAGCGN_7p0dB  = 30,
	halAFE_RFAGCGN_7p5dB  = 31 
} halAFE_RFAGCGN_t;

typedef enum
{
	halAFE_RFLPFBk_Bypass = 0,
	halAFE_RFLPFBk_Bank1  = 1,
	halAFE_RFLPFBk_Bank2  = 2,
	halAFE_RFLPFBk_Bank3  = 3 
} halAFE_RFLPFBank_t;

typedef enum
{
	halAFE_RFLPFBW_CD_4X_5p0MHz    = 0,    // CD 4X
	halAFE_RFLPFBW_CD_6X_5p0MHz    = 0,    // CD 6X
	halAFE_RFLPFBW_CD_8X_6p6MHz    = 1,    // CD 8X
	halAFE_RFLPFBW_CD_12X_9p7MHz   = 3,    // CD 12X
	halAFE_RFLPFBW_CD_16X_12p8MHz  = 5,    // CD 16X
	halAFE_RFLPFBW_CD_24X_19p0MHz  = 9,    // CD 24X
	halAFE_RFLPFBW_CD_28X_22p1MHz  = 11,    // CD 28X
	halAFE_RFLPFBW_CD_32X_25p2MHz  = 13,    // CD 32X
	halAFE_RFLPFBW_CD_48X_37p3MHz  = 7,    // CD 48X
	halAFE_RFLPFBW_CD_52X_40p2MHz  = 8,    // CD 52X
	halAFE_RFLPFBW_DVD_1X_5p0MHz   = 0,    // DVD 1X
	halAFE_RFLPFBW_DVD_2X_9p7MHz   = 3,    // DVD 2X
	halAFE_RFLPFBW_DVD_4X_20p4MHz  = 1,    // DVD 4X
	halAFE_RFLPFBW_DVD_8X_37p3MHz  = 7,    // DVD 8X
	halAFE_RFLPFBW_DVD_12X_54p3MHz = 13,    // DVD 12X
	halAFE_RFLPFBW_DVD_16X_71p2MHz = 19,    // DVD 16X
	halAFE_RFLPFBW_DVD_18X_79p6MHz = 22,    // DVD 18X
	halAFE_RFLPFBW_DVD_20X_90p9MHz = 26,    // DVD 20X
	halAFE_RFLPFBW_BD_1X_17p6MHz   = 0,    // BD 1X
	halAFE_RFLPFBW_BD_2X_34p5MHz   = 6,    // BD 2X
	halAFE_RFLPFBW_BD_4X_72p8MHz   = 0,    // BD 4X
	halAFE_RFLPFBW_BD_6X_105p3MHz  = 3,    // BD 6X
	halAFE_RFLPFBW_BD_8X_137p8MHz  = 6,    // BD 8X
	halAFE_RFLPFBW_BD_10X_170p3MHz = 9,    // BD 10X
	halAFE_RFLPFBW_BD_12X_202p8MHz = 12,    // BD 12X
	halAFE_RFLPFBW_BD_14X_231p0MHz = 15     // BD 14X
} halAFE_RFLPFBW_t;

typedef enum
{
	halAFE_RFGNLND_N9dB = 0,    // RF coarse gain for land data fields: -9 dB
	halAFE_RFGNLND_N6dB = 1,    // RF coarse gain for land data fields: -6 dB
	halAFE_RFGNLND_N3dB = 2,    // RF coarse gain for land data fields: -3 dB
	halAFE_RFGNLND_0dB  = 3,    // RF coarse gain for land data fields: 0 dB
	halAFE_RFGNLND_3dB  = 4,    // RF coarse gain for land data fields: 3 dB
	halAFE_RFGNLND_6dB  = 5,    // RF coarse gain for land data fields: 6 dB
	halAFE_RFGNLND_9dB  = 6,    // RF coarse gain for land data fields: 9 dB
	halAFE_RFGNLND_12dB = 7     // RF coarse gain for land data fields: 12 dB
} halAFE_RFGNLND_t;

typedef enum
{
	halAFE_RFGNGROV_N9dB = 0,    // RF coarse gain for groove data fields: -9 dB
	halAFE_RFGNGROV_N6dB = 1,    // RF coarse gain for groove data fields: -6 dB
	halAFE_RFGNGROV_N3dB = 2,    // RF coarse gain for groove data fields: -3 dB
	halAFE_RFGNGROV_0dB  = 3,    // RF coarse gain for groove data fields: 0 dB
	halAFE_RFGNGROV_3dB  = 4,    // RF coarse gain for groove data fields: 3 dB
	halAFE_RFGNGROV_6dB  = 5,    // RF coarse gain for groove data fields: 6 dB
	halAFE_RFGNGROV_9dB  = 6,    // RF coarse gain for groove data fields: 9 dB
	halAFE_RFGNGROV_12dB = 7     // RF coarse gain for groove data fields: 12 dB
} halAFE_RFGNGROV_t;

typedef enum
{
	halAFE_RFIBSEL_15uA = 0,    // Fixed bias current for AFE RF and DPD: 15uA
	halAFE_RFIBSEL_20uA = 1,    // Fixed bias current for AFE RF and DPD: 20uA
	halAFE_RFIBSEL_25uA = 2,    // Fixed bias current for AFE RF and DPD: 25uA
	halAFE_RFIBSEL_30uA = 3     // Fixed bias current for AFE RF and DPD: 30uA
} halAFE_RFIBSEL_t;

typedef enum
{
	halAFE_RFGNHDR_N9dB = 0,    // RF coarse gain for header fields: -9 dB
	halAFE_RFGNHDR_N6dB = 1,    // RF coarse gain for header fields: -6 dB
	halAFE_RFGNHDR_N3dB = 2,    // RF coarse gain for header fields: -3 dB
	halAFE_RFGNHDR_0dB  = 3,    // RF coarse gain for header fields: 0 dB
	halAFE_RFGNHDR_3dB  = 4,    // RF coarse gain for header fields: 3 dB
	halAFE_RFGNHDR_6dB  = 5,    // RF coarse gain for header fields: 6 dB
	halAFE_RFGNHDR_9dB  = 6,    // RF coarse gain for header fields: 9 dB
	halAFE_RFGNHDR_12dB = 7     // RF coarse gain for header fields: 12 dB
} halAFE_RFGNHDR_t;

typedef enum
{
	halAFE_DPDINHPFBW_p2MHz = 0,    // DPD input high pass filter bandwidth: 0.2 MHz
	halAFE_DPDINHPFBW_2MHz  = 1,    // DPD input high pass filter bandwidth: 2 MHz
	halAFE_DPDINHPFBW_4MHz  = 2,    // DPD input high pass filter bandwidth: 4 MHz
	halAFE_DPDINHPFBW_6MHz  = 3     // DPD input high pass filter bandwidth: 6 MHz
} halAFE_DPDINHPFBW_t;

typedef enum
{
	halAFE_DPDGN_N6p0dB = 0,
	halAFE_DPDGN_N4p5dB = 1,
	halAFE_DPDGN_N3p0dB = 2,
	halAFE_DPDGN_N1p5dB = 3,
	halAFE_DPDGN_0dB    = 4,
	halAFE_DPDGN_1p5dB  = 5,
	halAFE_DPDGN_3p0dB  = 6,
	halAFE_DPDGN_4p5dB  = 7,
	halAFE_DPDGN_6p0dB  = 8,
	halAFE_DPDGN_7p5dB  = 9,
	halAFE_DPDGN_9p0dB  = 10,
	halAFE_DPDGN_10p5dB = 11,
	halAFE_DPDGN_12p0dB = 12,
	halAFE_DPDGN_13p5dB = 13,
	halAFE_DPDGN_15p0dB = 14,
	halAFE_DPDGN_16p5dB = 15 
} halAFE_DPDGN_t;

typedef enum
{
	halAFE_DPDEQBWH_0  = 0,
	halAFE_DPDEQBWH_1  = 1,
	halAFE_DPDEQBWH_2  = 2,
	halAFE_DPDEQBWH_3  = 3,
	halAFE_DPDEQBWH_4  = 4,
	halAFE_DPDEQBWH_5  = 5,
	halAFE_DPDEQBWH_6  = 6,
	halAFE_DPDEQBWH_7  = 7,
	halAFE_DPDEQBWH_8  = 8,
	halAFE_DPDEQBWH_9  = 9,
	halAFE_DPDEQBWH_10 = 10,
	halAFE_DPDEQBWH_11 = 11,
	halAFE_DPDEQBWH_12 = 12,
	halAFE_DPDEQBWH_13 = 13,
	halAFE_DPDEQBWH_14 = 14,
	halAFE_DPDEQBWH_15 = 15,
	halAFE_DPDEQBWH_16 = 16,
	halAFE_DPDEQBWH_17 = 17,
	halAFE_DPDEQBWH_18 = 18,
	halAFE_DPDEQBWH_19 = 19,
	halAFE_DPDEQBWH_20 = 20 
} halAFE_DPDEQBWH_t;

typedef enum
{
	halAFE_DPDEQBWL_0  = 0,
	halAFE_DPDEQBWL_1  = 1,
	halAFE_DPDEQBWL_2  = 2,
	halAFE_DPDEQBWL_3  = 3,
	halAFE_DPDEQBWL_4  = 4,
	halAFE_DPDEQBWL_5  = 5,
	halAFE_DPDEQBWL_6  = 6,
	halAFE_DPDEQBWL_7  = 7,
	halAFE_DPDEQBWL_8  = 8,
	halAFE_DPDEQBWL_9  = 9,
	halAFE_DPDEQBWL_10 = 10,
	halAFE_DPDEQBWL_11 = 11,
	halAFE_DPDEQBWL_12 = 12,
	halAFE_DPDEQBWL_13 = 13,
	halAFE_DPDEQBWL_14 = 14,
	halAFE_DPDEQBWL_15 = 15,
	halAFE_DPDEQBWL_16 = 16,
	halAFE_DPDEQBWL_17 = 17,
	halAFE_DPDEQBWL_18 = 18,
	halAFE_DPDEQBWL_19 = 19,
	halAFE_DPDEQBWL_20 = 20 
} halAFE_DPDEQBWL_t;

typedef enum
{
	halAFE_DPDEQGNBoost_0p0dB  = 0,    // DPD equalizer gain boost: 0.0 dB
	halAFE_DPDEQGNBoost_4p0dB  = 1,    // DPD equalizer gain boost: 4.0 dB
	halAFE_DPDEQGNBoost_7p5dB  = 2,    // DPD equalizer gain boost: 7.5 dB
	halAFE_DPDEQGNBoost_10p0dB = 3,    // DPD equalizer gain boost: 10.0 dB
	halAFE_DPDEQGNBoost_11p5dB = 4,    // DPD equalizer gain boost: 11.5 dB
	halAFE_DPDEQGNBoost_13p0dB = 5,    // DPD equalizer gain boost: 13.0 dB
	halAFE_DPDEQGNBoost_14p5dB = 6,    // DPD equalizer gain boost: 14.5 dB
	halAFE_DPDEQGNBoost_15p5dB = 7     // DPD equalizer gain boost: 15.5 dB
} halAFE_DPDEQGNBoost_t;

typedef enum
{
	halAFE_RFWrGN_N9dB = 0,    // RF coarse gain during writing: -9 dB
	halAFE_RFWrGN_N6dB = 1,    // RF coarse gain during writing: -6 dB
	halAFE_RFWrGN_N3dB = 2,    // RF coarse gain during writing: -3 dB
	halAFE_RFWrGN_0dB  = 3,    // RF coarse gain during writing: 0 dB
	halAFE_RFWrGN_3dB  = 4,    // RF coarse gain during writing: 3 dB
	halAFE_RFWrGN_6dB  = 5,    // RF coarse gain during writing: 6 dB
	halAFE_RFWrGN_9dB  = 6,    // RF coarse gain during writing: 9 dB
	halAFE_RFWrGN_12dB = 7     // RF coarse gain during writing: 12 dB
} halAFE_RFWrGN_t;

typedef enum
{
	halAFE_DPDCPGN_6mVpDeg  = 0,    // DPD charge pump gain: 6 mV / degree
	halAFE_DPDCPGN_11mVpDeg = 1,    // DPD charge pump gain: 11 mV / degree
	halAFE_DPDCPGN_17mVpDeg = 2,    // DPD charge pump gain: 17 mV / degree
	halAFE_DPDCPGN_22mVpDeg = 3     // DPD charge pump gain: 22 mV / degree
} halAFE_DPDCPGN_t;

#if (CHIP_REV == 0xC0)

typedef enum
{
	halAFE_DPDCPFBW_50kHz  = 0,    // DPD charge pump filter Bandwidth: 50 kHz
	halAFE_DPDCPFBW_100kHz = 1,    // DPD charge pump filter Bandwidth: 100 kHz
	halAFE_DPDCPFBW_200kHz = 2,    // DPD charge pump filter Bandwidth: 200 kHz
	halAFE_DPDCPFBW_400kHz = 3     // DPD charge pump filter Bandwidth: 400 kHz
} halAFE_DPDCPFBW_t;

#endif // #if (CHIP_REV == 0xC0)

#if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_DPDCPFBW0_50kHz  = 0,    // DPD charge pump filter Bandwidth: 50 kHz
	halAFE_DPDCPFBW1_150kHz = 1,    // DPD charge pump filter Bandwidth: 150 kHz
	halAFE_DPDCPFBW2_150kHz = 2,    // DPD charge pump filter Bandwidth: 150 kHz
	halAFE_DPDCPFBW3_400kHz = 3     // DPD charge pump filter Bandwidth: 400 kHz
} halAFE_DPDCPFBW_t;

#endif // #if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_APCVref06DC_0p6V  = 0,    // APC VREF06 Signal DC Value: 0.6 V
	halAFE_APCVref06DC_0p64V = 1,    // APC VREF06 Signal DC Value: 0.64 V
	halAFE_APCVref06DC_0p68V = 2,    // APC VREF06 Signal DC Value: 0.68 V
	halAFE_APCVref06DC_0p72V = 3,    // APC VREF06 Signal DC Value: 0.72 V
	halAFE_APCVref06DC_0p76V = 4,    // APC VREF06 Signal DC Value: 0.76 V
	halAFE_APCVref06DC_0p8V  = 5,    // APC VREF06 Signal DC Value: 0.8 V
	halAFE_APCVref06DC_0p84V = 6,    // APC VREF06 Signal DC Value: 0.84 V
	halAFE_APCVref06DC_0p88V = 7,    // APC VREF06 Signal DC Value: 0.88 V
	halAFE_APCVref06DC_0p92V = 8,    // APC VREF06 Signal DC Value: 0.92 V
	halAFE_APCVref06DC_0p96V = 9,    // APC VREF06 Signal DC Value: 0.96 V
	halAFE_APCVref06DC_1V    = 10,    // APC VREF06 Signal DC Value: 1.0 V
	halAFE_APCVref06DC_1p04V = 11,    // APC VREF06 Signal DC Value: 1.04 V
	halAFE_APCVref06DC_1p08V = 12,    // APC VREF06 Signal DC Value: 1.08 V
	halAFE_APCVref06DC_1p12V = 13,    // APC VREF06 Signal DC Value: 1.12 V
	halAFE_APCVref06DC_1p16V = 14,    // APC VREF06 Signal DC Value: 1.16 V
	halAFE_APCVref06DC_1p2V  = 15     // APC VREF06 Signal DC Value: 1.2 V
} halAFE_APCVref06DC_t;

typedef enum
{
	halAFE_SubtBlkIN_BDFPDIP  = 0,    // Select Subtraction Block Input BDFPDIP
	halAFE_SubtBlkIN_BDFPDIN  = 1,    // Select Subtraction Block Input BDFPDIN
	halAFE_SubtBlkIN_DVDFPDIP = 2,    // Select Subtraction Block Input DVDFPDIP
	halAFE_SubtBlkIN_DVDFPDIN = 3     // Select Subtraction Block Input DVDFPDIN
} halAFE_SubtBlkIN_t;

typedef enum
{
	halAFE_RdAmpBlkIN_S1_S2      = 0,    // Select Read Amplifier Block Input source S1-S2
	halAFE_RdAmpBlkIN_S3_S4      = 1,    // Select Read Amplifier Block Input source S3-S4
	halAFE_RdAmpBlkIN_S4_S2      = 2,    // Select Read Amplifier Block Input source S4-S2
	halAFE_RdAmpBlkIN_SUB_VREF06 = 3     // Select Read Amplifier Block Input source SUB-VREF06
} halAFE_RdAmpBlkIN_t;

typedef enum
{
	halAFE_WrAmpBlkIN_S1_S2      = 0,    // Select Write Amplifier Block Input source S1-S2
	halAFE_WrAmpBlkIN_S3_S4      = 1,    // Select Write Amplifier Block Input source S3-S4
	halAFE_WrAmpBlkIN_S4_S2      = 2,    // Select Write Amplifier Block Input source S4-S2
	halAFE_WrAmpBlkIN_SUB_VREF06 = 3     // Select Write Amplifier Block Input source SUB-VREF06
} halAFE_WrAmpBlkIN_t;

typedef enum
{
	halAFE_APCT_M1PHSO_M2BHSO    = 0,    // Select APC MUX1=PHSO and APC MUX2=BHSO
	halAFE_APCT_M1VRPOW_M2VWPOW  = 1,    // Select APC MUX1=VRPOW and APC MUX2=VWPOW
	halAFE_APCT_M1V124_M2SUB     = 2,    // Select APC MUX1=V124 and APC MUX2=SUB
	halAFE_APCT_M1VREF06_M2FVREF = 3,    // Select APC MUX1=VREF06 and APC MUX2=FVREF
	halAFE_APCT_M1S4_M2S3        = 4,    // Select APC MUX1=S4 and APC MUX2=S3
	halAFE_APCT_M1S3_M2S4        = 5,    // Select APC MUX1=S3 and APC MUX2=S4
	halAFE_APCT_M1S2_M2S1        = 6,    // Select APC MUX1=S2 and APC MUX2=S1
	halAFE_APCT_M1S1_M2S2        = 7     // Select APC MUX1=S1 and APC MUX2=S2
} halAFE_APCTestMUX_t;

typedef enum
{
	halAFE_FVREF_2p1V = 0,    // FVREF Output Voltage: 2.1 V
	halAFE_FVREF_2p3V = 1,    // FVREF Output Voltage: 2.3 V
	halAFE_FVREF_2p5V = 2,    // FVREF Output Voltage: 2.5 V
	halAFE_FVREF_2p7V = 3     // FVREF Output Voltage: 2.7 V
} halAFE_FVREFSEL_t;

typedef enum
{
	halAFE_WblRdSH_WBLRSH2_0 = 0,    // Select Wobble Read Path S/H Gate WBLRSH2
	halAFE_WblRdSH_WBLRSH2_1 = 1,    // Select Wobble Read Path S/H Gate WBLRSH2
	halAFE_WblRdSH_Low       = 2,    // Select Wobble Read Path S/H Gate Low (continuous hold)
	halAFE_WblRdSH_High      = 3     // Select Wobble Read Path S/H Gate High (continuous sample)
} halAFE_WblRdSH_t;

typedef enum
{
	halAFE_RGUP_GAINUP   = 0,
	halAFE_RGUP_WLDON    = 1,
	halAFE_RGUP_InvWLDON = 2,
	halAFE_RGUP_RGUPREG  = 3 
} halAFE_RGUPSEL_t;

typedef enum
{
	halAFE_WblWrADGN1_0dB    = 0,    // Wobble write path AD gain 0 dB when WBLRSH = 1
	halAFE_WblWrADGN1_N6p0dB = 1,    // Wobble write path AD gain -6.0 dB when WBLRSH = 1
	halAFE_WblWrADGN1_N9p5dB = 2,    // Wobble write path AD gain -9.5 dB when WBLRSH = 1
	halAFE_WblWrADGN1_N12dB  = 3     // Wobble write path AD gain -12 dB when WBLRSH = 1
} halAFE_WblWrADGN1_t;

typedef enum
{
	halAFE_WblWrADGN0_0dB    = 0,    // Wobble write path AD gain 0 dB when WBLRSH = 0
	halAFE_WblWrADGN0_N6p0dB = 1,    // Wobble write path AD gain -6.0 dB when WBLRSH = 0
	halAFE_WblWrADGN0_N9p5dB = 2,    // Wobble write path AD gain -9.5 dB when WBLRSH = 0
	halAFE_WblWrADGN0_N12dB  = 3     // Wobble write path AD gain -12 dB when WBLRSH = 0
} halAFE_WblWrADGN0_t;

typedef enum
{
	halAFE_WblRdADPGN1_0dB = 0,    // Wobble read path AD pre-gain 0 dB when RGUP = 1
	halAFE_WblRdADPGN1_2dB = 1,    // Wobble read path AD pre-gain 2 dB when RGUP = 1
	halAFE_WblRdADPGN1_6dB = 2,    // Wobble read path AD pre-gain 6 dB when RGUP = 1
	halAFE_WblRdADPGN1_8dB = 3     // Wobble read path AD pre-gain 8 dB when RGUP = 1
} halAFE_WblRdADPGN1_t;

typedef enum
{
	halAFE_WblRdADPGN0_0dB = 0,    // Wobble read path AD pre-gain 0 dB when RGUP = 0
	halAFE_WblRdADPGN0_2dB = 1,    // Wobble read path AD pre-gain 2 dB when RGUP = 0
	halAFE_WblRdADPGN0_6dB = 2,    // Wobble read path AD pre-gain 6 dB when RGUP = 0
	halAFE_WblRdADPGN0_8dB = 3     // Wobble read path AD pre-gain 8 dB when RGUP = 0
} halAFE_WblRdADPGN0_t;

typedef enum
{
	halAFE_WblWrBCGN1_0dB    = 0,    // Wobble write path BC gain 0 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_N6p0dB = 1,    // Wobble write path BC gain -6.0 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_N9p5dB = 2,    // Wobble write path BC gain -9.5 dB when WBLRSH = 1
	halAFE_WblWrBCGN1_N12dB  = 3     // Wobble write path BC gain -12 dB when WBLRSH = 1
} halAFE_WblWrBCGN1_t;

typedef enum
{
	halAFE_WblWrBCGN0_0dB    = 0,    // Wobble write path BC gain 0 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_N6p0dB = 1,    // Wobble write path BC gain -6.0 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_N9p5dB = 2,    // Wobble write path BC gain -9.5 dB when WBLRSH = 0
	halAFE_WblWrBCGN0_N12dB  = 3     // Wobble write path BC gain -12 dB when WBLRSH = 0
} halAFE_WblWrBCGN0_t;

typedef enum
{
	halAFE_WblRdBCPGN1_0dB = 0,    // Wobble read path BC gain 0 dB when RGUP = 1
	halAFE_WblRdBCPGN1_2dB = 1,    // Wobble read path BC gain 2 dB when RGUP = 1
	halAFE_WblRdBCPGN1_6dB = 2,    // Wobble read path BC gain 6 dB when RGUP = 1
	halAFE_WblRdBCPGN1_8dB = 3     // Wobble read path BC gain 8 dB when RGUP = 1
} halAFE_WblRdBCPGN1_t;

typedef enum
{
	halAFE_WblRdBCPGN0_0dB = 0,    // Wobble read path BC gain 0 dB when RGUP = 0
	halAFE_WblRdBCPGN0_2dB = 1,    // Wobble read path BC gain 2 dB when RGUP = 0
	halAFE_WblRdBCPGN0_6dB = 2,    // Wobble read path BC gain 6 dB when RGUP = 0
	halAFE_WblRdBCPGN0_8dB = 3     // Wobble read path BC gain 8 dB when RGUP = 0
} halAFE_WblRdBCPGN0_t;

typedef enum
{
	halAFE_WblRdADGN1_0dB  = 0,    // Wobble read path AD gain 0 dB when GAINUP = 1
	halAFE_WblRdADGN1_4dB  = 1,    // Wobble read path AD gain 4 dB when GAINUP = 1
	halAFE_WblRdADGN1_12dB = 2,    // Wobble read path AD gain 12 dB when GAINUP = 1
	halAFE_WblRdADGN1_16dB = 3     // Wobble read path AD gain 16 dB when GAINUP = 1
} halAFE_WblRdADGN1_t;

typedef enum
{
	halAFE_WblRdADGN0_0dB  = 0,    // Wobble read path AD gain 0 dB when GAINUP = 0
	halAFE_WblRdADGN0_4dB  = 1,    // Wobble read path AD gain 4 dB when GAINUP = 0
	halAFE_WblRdADGN0_12dB = 2,    // Wobble read path AD gain 12 dB when GAINUP = 0
	halAFE_WblRdADGN0_16dB = 3     // Wobble read path AD gain 16 dB when GAINUP = 0
} halAFE_WblRdADGN0_t;

typedef enum
{
	halAFE_WblWrADPGNB_0dB0   = 0,    // Wobble write path AD pre-gain 0 dB when WGUP = 1
	halAFE_WblWrADPGNB_N6dB   = 1,    // Wobble write path AD pre-gain -6 dB when WGUP = 1
	halAFE_WblWrADPGNB_N9p5dB = 2,    // Wobble write path AD pre-gain -9.5 dB when WGUP = 1
	halAFE_WblWrADPGNB_N12dB  = 3     // Wobble write path AD pre-gain -12 dB when WGUP = 1
} halAFE_WblWrADPGNB_t;

typedef enum
{
	halAFE_WblWrADPGNA_0dB0   = 0,    // Wobble write path AD pre-gain 0 dB when WGUP = 0
	halAFE_WblWrADPGNA_N6dB   = 1,    // Wobble write path AD pre-gain -6 dB when WGUP = 0
	halAFE_WblWrADPGNA_N9p5dB = 2,    // Wobble write path AD pre-gain -9.5 dB when WGUP = 0
	halAFE_WblWrADPGNA_N12dB  = 3     // Wobble write path AD pre-gain -12 dB when WGUP = 0
} halAFE_WblWrADPGNA_t;

typedef enum
{
	halAFE_WblRdBCGN1_0dB  = 0,    // Wobble read path BC gain 0 dB when GAINUP = 1
	halAFE_WblRdBCGN1_4dB  = 1,    // Wobble read path BC gain 4 dB when GAINUP = 1
	halAFE_WblRdBCGN1_12dB = 2,    // Wobble read path BC gain 12 dB when GAINUP = 1
	halAFE_WblRdBCGN1_16dB = 3     // Wobble read path BC gain 16 dB when GAINUP = 1
} halAFE_WblRdBCGN1_t;

typedef enum
{
	halAFE_WblRdBCGN0_0dB  = 0,    // Wobble read path BC gain 0 dB when GAINUP = 0
	halAFE_WblRdBCGN0_4dB  = 1,    // Wobble read path BC gain 4 dB when GAINUP = 0
	halAFE_WblRdBCGN0_12dB = 2,    // Wobble read path BC gain 12 dB when GAINUP = 0
	halAFE_WblRdBCGN0_16dB = 3     // Wobble read path BC gain 16 dB when GAINUP = 0
} halAFE_WblRdBCGN0_t;

typedef enum
{
	halAFE_WblWrBCPGNB_0dB0   = 0,    // Wobble write path BC pre-gain 0 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N6dB   = 1,    // Wobble write path BC pre-gain -6 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N9p5dB = 2,    // Wobble write path BC pre-gain -9.5 dB when WGUP = 1
	halAFE_WblWrBCPGNB_N12dB  = 3     // Wobble write path BC pre-gain -12 dB when WGUP = 1
} halAFE_WblWrBCPGNB_t;

typedef enum
{
	halAFE_WblWrBCPGNA_0dB0   = 0,    // Wobble write path BC pre-gain 0 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N6dB   = 1,    // Wobble write path BC pre-gain -6 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N9p5dB = 2,    // Wobble write path BC pre-gain -9.5 dB when WGUP = 0
	halAFE_WblWrBCPGNA_N12dB  = 3     // Wobble write path BC pre-gain -12 dB when WGUP = 0
} halAFE_WblWrBCPGNA_t;

typedef enum
{
	halAFE_WGUPSEL_GAINUP   = 0,
	halAFE_WGUPSEL_WLDON    = 1,
	halAFE_WGUPSEL_InvWLDON = 2,
	halAFE_WGUPSEL_WGUPREG  = 3 
} halAFE_WGUPSEL_t;

typedef enum
{
	halAFE_WBLWSH2source_WBLWSH    = 0,
	halAFE_WBLWSH2source_WBLRSH    = 1,
	halAFE_WBLWSH2source_InvWBLWSH = 2,
	halAFE_WBLWSH2source_InvWBLRSH = 3 
} halAFE_WBLWSH2source_t;

typedef enum
{
	halAFE_WOBDTPSEL_RGUP_WGUP = 0,    // Wobble Digital Test Ports Select RGUP and WGUP
	halAFE_WOBDTPSEL_RSH_WSH   = 1,    // Wobble Digital Test Ports Select RSH and WSH
	halAFE_WOBDTPSEL_RSRC_WSRC = 2,    // Wobble Digital Test Ports Select RSRC and WSRC
	halAFE_WOBDTPSEL_ROUT_WOUT = 3     // Wobble Digital Test Ports Select ROUT and WOUT
} halAFE_WOBDTPSEL_t;

typedef enum
{
	halAFE_WBLROUT_WBLRSH2    = 0,    // Select Wobble Read Path Output Control WBLRSH2
	halAFE_WBLROUT_InvWBLWSH2 = 1,    // Select Wobble Read Path Output Control Inverted WBLWSH2
	halAFE_WBLROUT_InvWLDON    = 2,    // Select Wobble Read Path Output Control Inverted WLDON
	halAFE_WBLROUT_GAINUP      = 3,    // Select Wobble Read Path Output Control GAINUP
	halAFE_WBLROUT_Low4        = 4,    // Select Wobble Read Path Output Control Always Low
	halAFE_WBLROUT_Low5        = 5,    // Select Wobble Read Path Output Control Always Low
	halAFE_WBLROUT_High6       = 6,    // Select Wobble Read Path Output Control Always High
	halAFE_WBLROUT_High7       = 7     // Select Wobble Read Path Output Control Always High
} halAFE_WBLROUT_t;

typedef enum
{
	halAFE_WblWrSH_WBLWSH2_0 = 0,    // Select Wobble Write Path S/H Gate WBLWSH2
	halAFE_WblWrSH_WBLWSH2_1 = 1,    // Select Wobble Write Path S/H Gate WBLWSH2
	halAFE_WblWrSH_Low        = 2,    // Select Wobble Write Path S/H Gate Low (continuous hold)
	halAFE_WblWrSH_High       = 3     // Select Wobble Write Path S/H Gate High (continuous sample)
} halAFE_WblWrSH_t;

typedef enum
{
	halAFE_WblS_rwRelWLDON   = 0,    // Read:A-D(if WLDON=0);WRF1 and WRF2(if WLDON=1).Write:A-D(if WLDON=0);WRF1 and WRF2(if WLDON=1).
	halAFE_WblS_rADwWRF12    = 1,    // Read:A-D; Write:WRF1 and WRF2
	halAFE_WblS_rADwAD       = 2,    // Read:A-D; Write:A-D
	halAFE_WblS_rWRF12wWRF12 = 3     // Read:WRF1 and WRF2; Write:WRF1 and WRF2
} halAFE_WblSource_t;

typedef enum
{
	halAFE_WBLRSH2source_WBLWSH    = 0,
	halAFE_WBLRSH2source_WBLRSH    = 1,
	halAFE_WBLRSH2source_InvWBLWSH = 2,
	halAFE_WBLRSH2source_InvWBLRSH = 3 
} halAFE_WBLRSH2source_t;

typedef enum
{
	halAFE_WblLPF_75MHz = 0,    // Select Wobble Low Pass Filter 75 MHz
	halAFE_WblLPF_83MHz = 1,    // Select Wobble Low Pass Filter 83 MHz
	halAFE_WblLPF_68MHz = 2,    // Select Wobble Low Pass Filter 68 MHz
	halAFE_WblLPF_40MHz = 3,    // Select Wobble Low Pass Filter 40 MHz
	halAFE_WblLPF_18MHz = 4,    // Select Wobble Low Pass Filter 18 MHz
	halAFE_WblLPF_9MHz5 = 5,    // Select Wobble Low Pass Filter  9 MHz
	halAFE_WblLPF_9MHz6 = 6,    // Select Wobble Low Pass Filter  9 MHz
	halAFE_WblLPF_9MHz7 = 7     // Select Wobble Low Pass Filter  9 MHz
} halAFE_WblLPF_t;

typedef enum
{
	halAFE_WBLWOUT_InvWBLRSH2 = 0,    // Select Wobble Write Path Output Control Inverted WBLRSH2
	halAFE_WBLWOUT_WBLWSH2    = 1,    // Select Wobble Write Path Output Control WBLWSH2
	halAFE_WBLWOUT_WLDON      = 2,    // Select Wobble Write Path Output Control WLDON
	halAFE_WBLWOUT_InvGAINUP  = 3,    // Select Wobble Write Path Output Control Inverted GAINUP
	halAFE_WBLWOUT_Low4       = 4,    // Select Wobble Write Path Output Control Always Low
	halAFE_WBLWOUT_Low5       = 5,    // Select Wobble Write Path Output Control Always Low
	halAFE_WBLWOUT_High6      = 6,    // Select Wobble Write Path Output Control Always High
	halAFE_WBLWOUT_High7      = 7     // Select Wobble Write Path Output Control Always High
} halAFE_WBLWOUT_t;

typedef enum
{
	halAFE_FirstLPFBefADCBW_1MHz  = 0,    // First Order Low Pass Filter Before ADC Bandwidth Setting 1 MHz
	halAFE_FirstLPFBefADCBW_5MHz  = 1,    // First Order Low Pass Filter Before ADC Bandwidth Setting 5 MHz
	halAFE_FirstLPFBefADCBW_9MHz    = 2,    // First Order Low Pass Filter Before ADC Bandwidth Setting 9 MHz
	halAFE_FirstLPFBefADCBW_13MHz = 3,    // First Order Low Pass Filter Before ADC Bandwidth Setting 13 MHz
	halAFE_FirstLPFBefADCBW_17MHz = 4,    // First Order Low Pass Filter Before ADC Bandwidth Setting 17 MHz
	halAFE_FirstLPFBefADCBW_21MHz = 5,    // First Order Low Pass Filter Before ADC Bandwidth Setting 21 MHz
	halAFE_FirstLPFBefADCBW_25MHz = 6,    // First Order Low Pass Filter Before ADC Bandwidth Setting 25 MHz
	halAFE_FirstLPFBefADCBW_29MHz = 7,    // First Order Low Pass Filter Before ADC Bandwidth Setting 29 MHz
	halAFE_FirstLPFBefADCBW_33MHz = 8,    // First Order Low Pass Filter Before ADC Bandwidth Setting 33 MHz
	halAFE_FirstLPFBefADCBW_37MHz = 9,    // First Order Low Pass Filter Before ADC Bandwidth Setting 37 MHz
	halAFE_FirstLPFBefADCBW_41MHz = 10,    // First Order Low Pass Filter Before ADC Bandwidth Setting 41 MHz
	halAFE_FirstLPFBefADCBW_45MHz = 11,    // First Order Low Pass Filter Before ADC Bandwidth Setting 45 MHz
	halAFE_FirstLPFBefADCBW_49MHz = 12,    // First Order Low Pass Filter Before ADC Bandwidth Setting 49 MHz
	halAFE_FirstLPFBefADCBW_53MHz = 13,    // First Order Low Pass Filter Before ADC Bandwidth Setting 53 MHz
	halAFE_FirstLPFBefADCBW_57MHz = 14,    // First Order Low Pass Filter Before ADC Bandwidth Setting 57 MHz
	halAFE_FirstLPFBefADCBW_61MHz = 15     // First Order Low Pass Filter Before ADC Bandwidth Setting 61 MHz
} halAFE_FirstLPFBefADCBW_t;

typedef enum
{
	halAFE_AGCHPF_100kHz   = 0,    // AGC1 and AGC2 Input High Pass Filter Control 100 kHz
	halAFE_AGCHPF_200kHz   = 1,    // AGC1 and AGC2 Input High Pass Filter Control 200 kHz
	halAFE_AGCHPF_400kHz   = 2,    // AGC1 and AGC2 Input High Pass Filter Control 400 kHz
	halAFE_AGCHPF_50kHz    = 3,    // AGC1 and AGC2 Input High Pass Filter Control 50 kHz (unused)
	halAFE_AGCHPF_700kHz   = 4,    // AGC1 and AGC2 Input High Pass Filter Control 700 kHz
	halAFE_AGCHPF_1000kHz  = 5,    // AGC1 and AGC2 Input High Pass Filter Control 1000 kHz
	halAFE_AGCHPF_1500kHz6 = 6,    // AGC1 and AGC2 Input High Pass Filter Control 1500 kHz
	halAFE_AGCHPF_1500kHz7 = 7     // AGC1 and AGC2 Input High Pass Filter Control 1500 kHz
} halAFE_AGCHPF_t;

typedef enum
{
	halAFE_APCINPUT_S12BD_S34DVD  = 0,    // APC Input Selection: S1=BDP,  S2=BDN,  S3=DVDP, S4=DVDN
	halAFE_APCINPUT_S12BD_S34BD   = 1,    // APC Input Selection: S1=BDP,  S2=BDN,  S3=BDP,  S4=BDN
	halAFE_APCINPUT_S12DVD_S34DVD = 2,    // APC Input Selection: S1=DVDP, S2=DVDN, S3=DVDP, S4=DVDN
	halAFE_APCINPUT_S12DVD_S34BD  = 3     // APC Input Selection: S1=DVDP, S2=DVDN, S3=BDP,  S4=BDN
} halAFE_APCINPUT_t;

typedef enum
{
	halAFE_AGCAmptdSetlTime_0p5ms  = 0,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.5 ms
	halAFE_AGCAmptdSetlTime_0p25ms = 1,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.25 ms
	halAFE_AGCAmptdSetlTime_0p06ms = 3,    // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.06 ms
	halAFE_AGCAmptdSetlTime_0p03ms = 7     // AGC1 and AGC2 Amplitude Settling Time (Attack Rate) 0.03 ms
} halAFE_AGCAmptdSetlTime_t;

typedef enum
{
	halAFE_WblSumAmp_N3p2dB = 0,    // Wobble Summing Amplifier Gain -3.2 dB
	halAFE_WblSumAmp_N0p3dB = 1,    // Wobble Summing Amplifier Gain -0.3 dB
	halAFE_WblSumAmp_3p2dB  = 2,    // Wobble Summing Amplifier Gain 3.2 dB
	halAFE_WblSumAmp_6p8dB  = 3,    // Wobble Summing Amplifier Gain 6.8 dB
	halAFE_WblSumAmp_9p5dB  = 4     // Wobble Summing Amplifier Gain 9.5 dB
} halAFE_WblSumAmp_t;

typedef enum
{
	halAFE_AGCCLIP1_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 1.6 Vpp
	halAFE_AGCCLIP1_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 1.0 Vpp
	halAFE_AGCCLIP1_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 0.4 Vpp
	halAFE_AGCCLIP1_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping When BLANK=0 and WLDON=0 0.2 Vpp
} halAFE_AGCCLIP1_t;

typedef enum
{
	halAFE_AGCCLIP2_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 1.6 Vpp
	halAFE_AGCCLIP2_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 1.0 Vpp
	halAFE_AGCCLIP2_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 0.4 Vpp
	halAFE_AGCCLIP2_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping When BLANK=1 and WLDON=0 0.2 Vpp
} halAFE_AGCCLIP2_t;

typedef enum
{
	halAFE_AGCCLIP3_Vpp_1p6V = 0,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 1.6 Vpp
	halAFE_AGCCLIP3_Vpp_1p0V = 1,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 1.0 Vpp
	halAFE_AGCCLIP3_Vpp_0p4V = 2,    // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 0.4 Vpp
	halAFE_AGCCLIP3_Vpp_0p2V = 3     // AGC1/2 Controlled Amplitude Clipping During Write or DISCLIP=1 0.2 Vpp
} halAFE_AGCCLIP3_t;

typedef enum
{
	halAFE_AFE_MIO_SEL0 = 0,    // monitor AVG_UPDATE_CLK AMIO2OVVL AMIO1OVVL AMIO0OVVL
	halAFE_AFE_MIO_SEL1 = 1,    // monitor XTOK TX RC RX
	halAFE_AFE_MIO_SEL2 = 2,    // monitor TESTDIG1[3] TESTDIG1[2] TESTDIG1[1] TESTDIG1[0]
	halAFE_AFE_MIO_SEL3 = 3     // monitor TESTDIG2[3] TESTDIG2[2] TESTDIG2[1] TESTDIG2[0]
} halAFE_AFEDIGMIOSEL_t;

#if (CHIP_REV <= 0xC0)

typedef enum
{
	halAFE_MUX3ADSEL_MPXO3        = 0,
	halAFE_MUX3ADSEL_TE           = 1,
	halAFE_MUX3ADSEL_CE           = 2,
	halAFE_MUX3ADSEL_BS           = 3,
	halAFE_MUX3ADSEL_RP           = 4,
	halAFE_MUX3ADSEL_FE1          = 5,
	halAFE_MUX3ADSEL_TE1          = 6,
	halAFE_MUX3ADSEL_BS1          = 7,
	halAFE_MUX3ADSEL_RP1          = 8,
	halAFE_MUX3ADSEL_APCMUX1      = 9,
	halAFE_MUX3ADSEL_APCMUX2      = 10,
	halAFE_MUX3ADSEL_WOBMUX1      = 11,
	halAFE_MUX3ADSEL_WOBMUX2      = 12,
	halAFE_MUX3ADSEL_SAN          = 13,
	halAFE_MUX3ADSEL_SBN          = 14,
	halAFE_MUX3ADSEL_SCN          = 15,
	halAFE_MUX3ADSEL_SDN          = 16,
	halAFE_MUX3ADSEL_SEN          = 17,
	halAFE_MUX3ADSEL_SFN          = 18,
	halAFE_MUX3ADSEL_SGN          = 19,
	halAFE_MUX3ADSEL_SHN          = 20,
	halAFE_MUX3ADSEL_SIN          = 21,
	halAFE_MUX3ADSEL_SJN          = 22,
	halAFE_MUX3ADSEL_MUXOUT2      = 23,
	halAFE_MUX3ADSEL_V125         = 24,
	halAFE_MUX3ADSEL_VINAO        = 25,
	halAFE_MUX3ADSEL_TESTI40UA   = 26,
	halAFE_MUX3ADSEL_VCTRLTBGPLL = 27,
	halAFE_MUX3ADSEL_VCTRLSYSPLL = 28,
	halAFE_MUX3ADSEL_VCTRLWPLL   = 29,
	halAFE_MUX3ADSEL_VCTRLADCPLL = 30,
	halAFE_MUX3ADSEL_VCTRLSRVPLL = 31 
} halAFE_MUX3ADSEL_t;

#endif // #if (CHIP_REV <= 0xC0)

#if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_MUX3ADSEL_MPXO3        = 0,
	halAFE_MUX3ADSEL_TE           = 1,
	halAFE_MUX3ADSEL_CE           = 2,
	halAFE_MUX3ADSEL_BS           = 3,
	halAFE_MUX3ADSEL_RP           = 4,
	halAFE_MUX3ADSEL_FE1          = 5,
	halAFE_MUX3ADSEL_TE1          = 6,
	halAFE_MUX3ADSEL_BS1          = 7,
	halAFE_MUX3ADSEL_VBG         = 8,
	halAFE_MUX3ADSEL_BETAPH      = 9,
	halAFE_MUX3ADSEL_BETABH      = 10,
	halAFE_MUX3ADSEL_WOBMUX1      = 11,
	halAFE_MUX3ADSEL_WOBMUX2      = 12,
	halAFE_MUX3ADSEL_SAN          = 13,
	halAFE_MUX3ADSEL_SBN          = 14,
	halAFE_MUX3ADSEL_SCN          = 15,
	halAFE_MUX3ADSEL_SDN          = 16,
	halAFE_MUX3ADSEL_SEN          = 17,
	halAFE_MUX3ADSEL_SFN          = 18,
	halAFE_MUX3ADSEL_SGN          = 19,
	halAFE_MUX3ADSEL_SHN          = 20,
	halAFE_MUX3ADSEL_SIN          = 21,
	halAFE_MUX3ADSEL_SJN          = 22,
	halAFE_MUX3ADSEL_MUXOUT2      = 23,
	halAFE_MUX3ADSEL_V125         = 24,
	halAFE_MUX3ADSEL_VINAO        = 25,
	halAFE_MUX3ADSEL_TESTI40UA   = 26,
	halAFE_MUX3ADSEL_VCTRLTBGPLL = 27,
	halAFE_MUX3ADSEL_VCTRLSYSPLL = 28,
	halAFE_MUX3ADSEL_VCTRLWPLL   = 29,
	halAFE_MUX3ADSEL_VCTRLADCPLL = 30,
	halAFE_MUX3ADSEL_VCTRLSRVPLL = 31 
} halAFE_MUX3ADSEL_t;

#endif // #if (CHIP_REV >= 0xC1)

typedef enum
{
	halAFE_OPCPHDRateDec_Div32768 = 0,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^15)
	halAFE_OPCPHDRateDec_Div16384 = 1,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^14)
	halAFE_OPCPHDRateDec_Div8192  = 2,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^13)
	halAFE_OPCPHDRateDec_Div4096  = 3,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^12)
	halAFE_OPCPHDRateDec_Div2048  = 4,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^11)
	halAFE_OPCPHDRateDec_Div1024  = 5,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^10)
	halAFE_OPCPHDRateDec_Div512   = 6,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 9)
	halAFE_OPCPHDRateDec_Div256   = 7,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 8)
	halAFE_OPCPHDRateDec_Div128   = 8,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 7)
	halAFE_OPCPHDRateDec_Div64    = 9,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 6)
	halAFE_OPCPHDRateDec_Div32    = 10,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 5)
	halAFE_OPCPHDRateDec_Div16    = 11,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 4)
	halAFE_OPCPHDRateDec_Div8     = 12,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 3)
	halAFE_OPCPHDRateDec_Div4     = 13,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 2)
	halAFE_OPCPHDRateDec_Div2     = 14,    // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 1)
	halAFE_OPCPHDRateDec_Div1     = 15     // Decrease Short Term P/H Droop Rate by a factor of 1/(2^ 0)
} halAFE_OPCPHDRateDec_t;

typedef enum
{
	halAFE_MUXADSEL_BETAPH    = 1,
	halAFE_MUXADSEL_MUXTOAD3O = 2,
	halAFE_MUXADSEL_BETABH    = 3 
} halAFE_MUXADSEL_t;

typedef enum
{
	halAFE_OPCSRC_MUXTOAD2 = 0,
	halAFE_OPCSRC_MUXTOAD1 = 1 
} halAFE_OPCSRC_t;

typedef enum
{
	halAFE_OPCPHBaseRate_100mVpus = 0,
	halAFE_OPCPHBaseRate_50mVpus  = 1 
} halAFE_OPCBaseRate_t;

typedef enum
{
	halAFE_OPCPHBHDRateInc_1X = 0,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 1
	halAFE_OPCPHBHDRateInc_2X = 1,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 2
	halAFE_OPCPHBHDRateInc_4X = 2,    // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 4
	halAFE_OPCPHBHDRateInc_8X = 3     // Increase OPC Short Term P/H and B/H Droop Rate by a factor of 8
} halAFE_OPCPHBHDRateInc_t;

#endif // #if (CHIP_REV >= 0xC0)


#endif /* __REG_AFE_LOGIC_H__ */

