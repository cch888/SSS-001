/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_write_strategy.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the WS core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_WRITE_STRATEGY_H__
#define __REG_WRITE_STRATEGY_H__

// Core register map
#define HAL_WS_VERSION 7.37
#define HAL_WS_BaseAddress 0x10000F00
#define HAL_WSREG_MAP ((struct halWSReg_Map*) (void *) HAL_WS_BaseAddress)
__packed struct halWSReg_Map
{
	HAL_Reg8  ReservedBlk0[160];              //00000000
	HAL_Reg8  WGUPDELAY;                      //000000A0
	HAL_Reg8  WGDNDELAY;                      //000000A1
	HAL_Reg8  WSINDEX;                        //000000A2
	HAL_Reg8  ReservedBlk1[1];                //000000A3
	HAL_Reg32 WSDATA0;                        //000000A4
	HAL_Reg8  ReservedBlk2[5];                //000000A8
	HAL_Reg8  GAINSWCTL1;                     //000000AD
	HAL_Reg8  GAINSWCTL;                      //000000AE
	HAL_Reg8  OSCENCTL;                       //000000AF
	HAL_Reg8  OSCENCTL1;                      //000000B0
	HAL_Reg8  WSMISC0;                        //000000B1
	HAL_Reg8  WSSTS0;                         //000000B2
	HAL_Reg8  WSMISC1;                        //000000B3
	HAL_Reg8  WSEXTERNAL;                     //000000B4
	HAL_Reg8  WSMISC2;                        //000000B5
	HAL_Reg8  EFMCTRL0;                       //000000B6
	HAL_Reg8  EFMCTRL1;                       //000000B7
	HAL_Reg8  EFMCTRL2;                       //000000B8
	HAL_Reg8  EFMCTRL3;                       //000000B9
	HAL_Reg8  OPUDLYV;                        //000000BA
	HAL_Reg8  TBGMN;                          //000000BB
	HAL_Reg8  TBGCTL;                         //000000BC
	HAL_Reg8  TESTINST;                       //000000BD
	HAL_Reg8  TESTTENA;                       //000000BE
	HAL_Reg8  TESTEFMV;                       //000000BF
	HAL_Reg8  PRINTS;                         //000000C0
	HAL_Reg8  PRINTEN;                        //000000C1
	HAL_Reg8  CUSINTS;                        //000000C2
	HAL_Reg8  CUSINTEN;                       //000000C3
	HAL_Reg8  ReservedBlk3[6];                //000000C4
	HAL_Reg8  TESTPHAS;                       //000000CA
	HAL_Reg8  TESTESRC;                       //000000CB
	HAL_Reg8  TESTPSMK;                       //000000CC
	HAL_Reg8  TBGQPRS;                        //000000CD
	HAL_Reg8  TBGMISC;                        //000000CE
	HAL_Reg8  SMP1CTRL;                       //000000CF
	HAL_Reg16 SMP1PATL;                       //000000D0
	HAL_Reg8  ReservedBlk4[1];                //000000D2
	HAL_Reg8  SMP1LDLY;                       //000000D3
	HAL_Reg8  SMP1WID;                        //000000D4
	HAL_Reg8  SMP2CTRL;                       //000000D5
	HAL_Reg16 SMP2PATL;                       //000000D6
	HAL_Reg8  SMP2LDLY;                       //000000D8
	HAL_Reg8  SMP2WID;                        //000000D9
	HAL_Reg8  SMP3CTRL;                       //000000DA
	HAL_Reg8  ReservedBlk5[1];                //000000DB
	HAL_Reg16 SMP3PATL;                       //000000DC
	HAL_Reg8  SMP3LDLY;                       //000000DE
	HAL_Reg8  SMP3WID;                        //000000DF
	HAL_Reg8  SMP4CTRL;                       //000000E0
	HAL_Reg8  ReservedBlk6[1];                //000000E1
	HAL_Reg16 SMP4PATL;                       //000000E2
	HAL_Reg8  SMP4LDLY;                       //000000E4
	HAL_Reg8  SMP4WID;                        //000000E5
	HAL_Reg8  SMP5CTRL;                       //000000E6
	HAL_Reg8  ReservedBlk7[1];                //000000E7
	HAL_Reg16 SMP5PATL;                       //000000E8
	HAL_Reg8  SMP5LDLY;                       //000000EA
	HAL_Reg8  SMP5WID;                        //000000EB
	HAL_Reg8  SMP15SDLY;                      //000000EC
	HAL_Reg8  SMP15SWID;                      //000000ED
	HAL_Reg8  ReservedBlk8[4];                //000000EE
	HAL_Reg8  TESTMSC0;                       //000000F2
	HAL_Reg8  TESTSLFT;                       //000000F3
	HAL_Reg8  SMPUPDTR;                       //000000F4
	HAL_Reg8  TBGDIVCTL;                      //000000F5
	HAL_Reg8  ReservedBlk9[1];                //000000F6
	HAL_Reg8  TESTCCTL;                       //000000F7
	HAL_Reg8  TESTCDAT;                       //000000F8
	HAL_Reg8  WSDBSEL0;                       //000000F9
	HAL_Reg8  WSDBSEL1;                       //000000FA
	HAL_Reg8  WSSPARE;                        //000000FB
	HAL_Reg8  WSMIO0;                         //000000FC
	HAL_Reg8  WSMIO1;                         //000000FD
	HAL_Reg8  WSMIO2;                         //000000FE
	HAL_Reg8  WSMIO3;                         //000000FF
};
// Register WGUPDELAY
#define HAL_WS_WGUPDELAY_REGISTER    (HAL_WSREG_MAP->WGUPDELAY)
#define HAL_WS_WGUPDELAY_LSB         0
#define HAL_WS_WGUPDELAY_MSB         4
#define HAL_WS_WGUPDELAY_MASK        0x0000001F
#define HAL_WS_WGUPDELAY_TYPE        UByte
#define HAL_WS_WGUPDELAY             HAL_WS_WGUPDELAY_MASK
#define HAL_WS_WGUPDELAY_READABLE    1
#define HAL_WS_WGUPDELAY_WRITABLE    1
#define HAL_WS_WGUPDELAY_SIGNED      0
#define HAL_WS_WGUPDELAY_ADR         0x10000FA0

// Register WGDNDELAY
#define HAL_WS_WGDNDELAY_REGISTER    (HAL_WSREG_MAP->WGDNDELAY)
#define HAL_WS_WGDNDELAY_LSB         0
#define HAL_WS_WGDNDELAY_MSB         4
#define HAL_WS_WGDNDELAY_MASK        0x0000001F
#define HAL_WS_WGDNDELAY_TYPE        UByte
#define HAL_WS_WGDNDELAY             HAL_WS_WGDNDELAY_MASK
#define HAL_WS_WGDNDELAY_READABLE    1
#define HAL_WS_WGDNDELAY_WRITABLE    1
#define HAL_WS_WGDNDELAY_SIGNED      0
#define HAL_WS_WGDNDELAY_ADR         0x10000FA1

// Register WSINDEX
#define HAL_WS_WSRAMINDEX_REGISTER    (HAL_WSREG_MAP->WSINDEX)
#define HAL_WS_WSRAMINDEX_LSB         0
#define HAL_WS_WSRAMINDEX_MSB         6
#define HAL_WS_WSRAMINDEX_MASK        0x0000007F
#define HAL_WS_WSRAMINDEX_TYPE        UByte
#define HAL_WS_WSRAMINDEX             HAL_WS_WSRAMINDEX_MASK
#define HAL_WS_WSRAMINDEX_READABLE    1
#define HAL_WS_WSRAMINDEX_WRITABLE    1
#define HAL_WS_WSRAMINDEX_SIGNED      0
#define HAL_WS_WSRAMINDEX_ADR         0x10000FA2

// Registers WSDATA0/WSDATA1/WSDATA2/WSDATA3
#define HAL_WS_WSRAMDATA_REGISTER    (HAL_WSREG_MAP->WSDATA0)
#define HAL_WS_WSRAMDATA_LSB         0
#define HAL_WS_WSRAMDATA_MSB         31
#define HAL_WS_WSRAMDATA_MASK        0xFFFFFFFF
#define HAL_WS_WSRAMDATA_TYPE        UInt32
#define HAL_WS_WSRAMDATA             HAL_WS_WSRAMDATA_MASK
#define HAL_WS_WSRAMDATA_READABLE    1
#define HAL_WS_WSRAMDATA_WRITABLE    1
#define HAL_WS_WSRAMDATA_SIGNED      0
#define HAL_WS_WSRAMDATA_ADR         0x10000FA4

#if (CHIP_REV >= 0xC0)

// Register GAINSWCTL1
#define HAL_WS_GAINSW4W_REGISTER    (HAL_WSREG_MAP->GAINSWCTL1)
#define HAL_WS_GAINSW4W_LSB         6
#define HAL_WS_GAINSW4W_MSB         7
#define HAL_WS_GAINSW4W_MASK        0x000000C0
#define HAL_WS_GAINSW4W_TYPE        halWS_GAINSW4W_t
#define HAL_WS_GAINSW4W             HAL_WS_GAINSW4W_MASK
#define HAL_WS_GAINSW4W_READABLE    1
#define HAL_WS_GAINSW4W_WRITABLE    1
#define HAL_WS_GAINSW4W_SIGNED      0
#define HAL_WS_GAINSW4W_ADR         0x10000FAD

#define HAL_WS_GAINSW4R_REGISTER    (HAL_WSREG_MAP->GAINSWCTL1)
#define HAL_WS_GAINSW4R_LSB         4
#define HAL_WS_GAINSW4R_MSB         5
#define HAL_WS_GAINSW4R_MASK        0x00000030
#define HAL_WS_GAINSW4R_TYPE        halWS_GAINSW4R_t
#define HAL_WS_GAINSW4R             HAL_WS_GAINSW4R_MASK
#define HAL_WS_GAINSW4R_READABLE    1
#define HAL_WS_GAINSW4R_WRITABLE    1
#define HAL_WS_GAINSW4R_SIGNED      0
#define HAL_WS_GAINSW4R_ADR         0x10000FAD

#define HAL_WS_GAINSW3W_REGISTER    (HAL_WSREG_MAP->GAINSWCTL1)
#define HAL_WS_GAINSW3W_LSB         2
#define HAL_WS_GAINSW3W_MSB         3
#define HAL_WS_GAINSW3W_MASK        0x0000000C
#define HAL_WS_GAINSW3W_TYPE        halWS_GAINSW3W_t
#define HAL_WS_GAINSW3W             HAL_WS_GAINSW3W_MASK
#define HAL_WS_GAINSW3W_READABLE    1
#define HAL_WS_GAINSW3W_WRITABLE    1
#define HAL_WS_GAINSW3W_SIGNED      0
#define HAL_WS_GAINSW3W_ADR         0x10000FAD

#define HAL_WS_GAINSW3R_REGISTER    (HAL_WSREG_MAP->GAINSWCTL1)
#define HAL_WS_GAINSW3R_LSB         0
#define HAL_WS_GAINSW3R_MSB         1
#define HAL_WS_GAINSW3R_MASK        0x00000003
#define HAL_WS_GAINSW3R_TYPE        halWS_GAINSW3R_t
#define HAL_WS_GAINSW3R             HAL_WS_GAINSW3R_MASK
#define HAL_WS_GAINSW3R_READABLE    1
#define HAL_WS_GAINSW3R_WRITABLE    1
#define HAL_WS_GAINSW3R_SIGNED      0
#define HAL_WS_GAINSW3R_ADR         0x10000FAD

#endif // #if (CHIP_REV >= 0xC0)

// Register GAINSWCTL
#define HAL_WS_GAINSW2W_REGISTER    (HAL_WSREG_MAP->GAINSWCTL)
#define HAL_WS_GAINSW2W_LSB         6
#define HAL_WS_GAINSW2W_MSB         7
#define HAL_WS_GAINSW2W_MASK        0x000000C0
#define HAL_WS_GAINSW2W_TYPE        halWS_GAINSW2W_t
#define HAL_WS_GAINSW2W             HAL_WS_GAINSW2W_MASK
#define HAL_WS_GAINSW2W_READABLE    1
#define HAL_WS_GAINSW2W_WRITABLE    1
#define HAL_WS_GAINSW2W_SIGNED      0
#define HAL_WS_GAINSW2W_ADR         0x10000FAE

#define HAL_WS_GAINSW2R_REGISTER    (HAL_WSREG_MAP->GAINSWCTL)
#define HAL_WS_GAINSW2R_LSB         4
#define HAL_WS_GAINSW2R_MSB         5
#define HAL_WS_GAINSW2R_MASK        0x00000030
#define HAL_WS_GAINSW2R_TYPE        halWS_GAINSW2R_t
#define HAL_WS_GAINSW2R             HAL_WS_GAINSW2R_MASK
#define HAL_WS_GAINSW2R_READABLE    1
#define HAL_WS_GAINSW2R_WRITABLE    1
#define HAL_WS_GAINSW2R_SIGNED      0
#define HAL_WS_GAINSW2R_ADR         0x10000FAE

#define HAL_WS_GAINSWW_REGISTER    (HAL_WSREG_MAP->GAINSWCTL)
#define HAL_WS_GAINSWW_LSB         2
#define HAL_WS_GAINSWW_MSB         3
#define HAL_WS_GAINSWW_MASK        0x0000000C
#define HAL_WS_GAINSWW_TYPE        halWS_GAINSWW_t
#define HAL_WS_GAINSWW             HAL_WS_GAINSWW_MASK
#define HAL_WS_GAINSWW_READABLE    1
#define HAL_WS_GAINSWW_WRITABLE    1
#define HAL_WS_GAINSWW_SIGNED      0
#define HAL_WS_GAINSWW_ADR         0x10000FAE

#define HAL_WS_GAINSWR_REGISTER    (HAL_WSREG_MAP->GAINSWCTL)
#define HAL_WS_GAINSWR_LSB         0
#define HAL_WS_GAINSWR_MSB         1
#define HAL_WS_GAINSWR_MASK        0x00000003
#define HAL_WS_GAINSWR_TYPE        halWS_GAINSWR_t
#define HAL_WS_GAINSWR             HAL_WS_GAINSWR_MASK
#define HAL_WS_GAINSWR_READABLE    1
#define HAL_WS_GAINSWR_WRITABLE    1
#define HAL_WS_GAINSWR_SIGNED      0
#define HAL_WS_GAINSWR_ADR         0x10000FAE

// Register OSCENCTL
#define HAL_WS_MANOSCEN_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_MANOSCEN_LSB         7
#define HAL_WS_MANOSCEN_MSB         7
#define HAL_WS_MANOSCEN_MASK        0x00000080
#define HAL_WS_MANOSCEN_TYPE        Bool
#define HAL_WS_MANOSCEN             HAL_WS_MANOSCEN_MASK
#define HAL_WS_MANOSCEN_READABLE    1
#define HAL_WS_MANOSCEN_WRITABLE    1
#define HAL_WS_MANOSCEN_SIGNED      0
#define HAL_WS_MANOSCEN_ADR         0x10000FAF

#define HAL_WS_RWSEL_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_RWSEL_LSB         6
#define HAL_WS_RWSEL_MSB         6
#define HAL_WS_RWSEL_MASK        0x00000040
#define HAL_WS_RWSEL_TYPE        halWS_OSCENMOD_t
#define HAL_WS_RWSEL             HAL_WS_RWSEL_MASK
#define HAL_WS_RWSEL_READABLE    1
#define HAL_WS_RWSEL_WRITABLE    1
#define HAL_WS_RWSEL_SIGNED      0
#define HAL_WS_RWSEL_ADR         0x10000FAF

#define HAL_WS_OSCSMP_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_OSCSMP_LSB         3
#define HAL_WS_OSCSMP_MSB         5
#define HAL_WS_OSCSMP_MASK        0x00000038
#define HAL_WS_OSCSMP_TYPE        halWS_OSCENSamp_t
#define HAL_WS_OSCSMP             HAL_WS_OSCSMP_MASK
#define HAL_WS_OSCSMP_READABLE    1
#define HAL_WS_OSCSMP_WRITABLE    1
#define HAL_WS_OSCSMP_SIGNED      0
#define HAL_WS_OSCSMP_ADR         0x10000FAF

#define HAL_WS_OSCENI_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_OSCENI_LSB         2
#define HAL_WS_OSCENI_MSB         2
#define HAL_WS_OSCENI_MASK        0x00000004
#define HAL_WS_OSCENI_TYPE        Bool
#define HAL_WS_OSCENI             HAL_WS_OSCENI_MASK
#define HAL_WS_OSCENI_READABLE    1
#define HAL_WS_OSCENI_WRITABLE    1
#define HAL_WS_OSCENI_SIGNED      0
#define HAL_WS_OSCENI_ADR         0x10000FAF

#define HAL_WS_OSCEN2_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_OSCEN2_LSB         1
#define HAL_WS_OSCEN2_MSB         1
#define HAL_WS_OSCEN2_MASK        0x00000002
#define HAL_WS_OSCEN2_TYPE        Bool
#define HAL_WS_OSCEN2             HAL_WS_OSCEN2_MASK
#define HAL_WS_OSCEN2_READABLE    1
#define HAL_WS_OSCEN2_WRITABLE    1
#define HAL_WS_OSCEN2_SIGNED      0
#define HAL_WS_OSCEN2_ADR         0x10000FAF

#define HAL_WS_OSCEN1_REGISTER    (HAL_WSREG_MAP->OSCENCTL)
#define HAL_WS_OSCEN1_LSB         0
#define HAL_WS_OSCEN1_MSB         0
#define HAL_WS_OSCEN1_MASK        0x00000001
#define HAL_WS_OSCEN1_TYPE        Bool
#define HAL_WS_OSCEN1             HAL_WS_OSCEN1_MASK
#define HAL_WS_OSCEN1_READABLE    1
#define HAL_WS_OSCEN1_WRITABLE    1
#define HAL_WS_OSCEN1_SIGNED      0
#define HAL_WS_OSCEN1_ADR         0x10000FAF

#if (CHIP_REV >= 0xC0)

// Register OSCENCTL1
#define HAL_WS_OSCEFM_REGISTER    (HAL_WSREG_MAP->OSCENCTL1)
#define HAL_WS_OSCEFM_LSB         0
#define HAL_WS_OSCEFM_MSB         1
#define HAL_WS_OSCEFM_MASK        0x00000003
#define HAL_WS_OSCEFM_TYPE        UByte
#define HAL_WS_OSCEFM             HAL_WS_OSCEFM_MASK
#define HAL_WS_OSCEFM_READABLE    1
#define HAL_WS_OSCEFM_WRITABLE    1
#define HAL_WS_OSCEFM_SIGNED      0
#define HAL_WS_OSCEFM_ADR         0x10000FB0

#endif // #if (CHIP_REV >= 0xC0)

// Register WSMISC0
#define HAL_WS_WSRAMRDEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_WSRAMRDEN_LSB         7
#define HAL_WS_WSRAMRDEN_MSB         7
#define HAL_WS_WSRAMRDEN_MASK        0x00000080
#define HAL_WS_WSRAMRDEN_TYPE        Bool
#define HAL_WS_WSRAMRDEN             HAL_WS_WSRAMRDEN_MASK
#define HAL_WS_WSRAMRDEN_READABLE    1
#define HAL_WS_WSRAMRDEN_WRITABLE    1
#define HAL_WS_WSRAMRDEN_SIGNED      0
#define HAL_WS_WSRAMRDEN_ADR         0x10000FB1

#define HAL_WS_BDEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_BDEN_LSB         5
#define HAL_WS_BDEN_MSB         5
#define HAL_WS_BDEN_MASK        0x00000020
#define HAL_WS_BDEN_TYPE        Bool
#define HAL_WS_BDEN             HAL_WS_BDEN_MASK
#define HAL_WS_BDEN_READABLE    1
#define HAL_WS_BDEN_WRITABLE    1
#define HAL_WS_BDEN_SIGNED      0
#define HAL_WS_BDEN_ADR         0x10000FB1

#define HAL_WS_HDDVDEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_HDDVDEN_LSB         4
#define HAL_WS_HDDVDEN_MSB         4
#define HAL_WS_HDDVDEN_MASK        0x00000010
#define HAL_WS_HDDVDEN_TYPE        Bool
#define HAL_WS_HDDVDEN             HAL_WS_HDDVDEN_MASK
#define HAL_WS_HDDVDEN_READABLE    1
#define HAL_WS_HDDVDEN_WRITABLE    1
#define HAL_WS_HDDVDEN_SIGNED      0
#define HAL_WS_HDDVDEN_ADR         0x10000FB1

#define HAL_WS_WGATEMON_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_WGATEMON_LSB         3
#define HAL_WS_WGATEMON_MSB         3
#define HAL_WS_WGATEMON_MASK        0x00000008
#define HAL_WS_WGATEMON_TYPE        Bool
#define HAL_WS_WGATEMON             HAL_WS_WGATEMON_MASK
#define HAL_WS_WGATEMON_READABLE    1
#define HAL_WS_WGATEMON_WRITABLE    0
#define HAL_WS_WGATEMON_SIGNED      0
#define HAL_WS_WGATEMON_ADR         0x10000FB1

#define HAL_WS_FASTEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_FASTEN_LSB         2
#define HAL_WS_FASTEN_MSB         2
#define HAL_WS_FASTEN_MASK        0x00000004
#define HAL_WS_FASTEN_TYPE        Bool
#define HAL_WS_FASTEN             HAL_WS_FASTEN_MASK
#define HAL_WS_FASTEN_READABLE    1
#define HAL_WS_FASTEN_WRITABLE    1
#define HAL_WS_FASTEN_SIGNED      0
#define HAL_WS_FASTEN_ADR         0x10000FB1

#define HAL_WS_DVDEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_DVDEN_LSB         1
#define HAL_WS_DVDEN_MSB         1
#define HAL_WS_DVDEN_MASK        0x00000002
#define HAL_WS_DVDEN_TYPE        Bool
#define HAL_WS_DVDEN             HAL_WS_DVDEN_MASK
#define HAL_WS_DVDEN_READABLE    1
#define HAL_WS_DVDEN_WRITABLE    1
#define HAL_WS_DVDEN_SIGNED      0
#define HAL_WS_DVDEN_ADR         0x10000FB1

#define HAL_WS_POST2XEN_REGISTER    (HAL_WSREG_MAP->WSMISC0)
#define HAL_WS_POST2XEN_LSB         0
#define HAL_WS_POST2XEN_MSB         0
#define HAL_WS_POST2XEN_MASK        0x00000001
#define HAL_WS_POST2XEN_TYPE        halWS_PostambPos_t
#define HAL_WS_POST2XEN             HAL_WS_POST2XEN_MASK
#define HAL_WS_POST2XEN_READABLE    1
#define HAL_WS_POST2XEN_WRITABLE    1
#define HAL_WS_POST2XEN_SIGNED      0
#define HAL_WS_POST2XEN_ADR         0x10000FB1

// Register WSSTS0
#define HAL_WS_RDCOMP_REGISTER    (HAL_WSREG_MAP->WSSTS0)
#define HAL_WS_RDCOMP_LSB         1
#define HAL_WS_RDCOMP_MSB         1
#define HAL_WS_RDCOMP_MASK        0x00000002
#define HAL_WS_RDCOMP_TYPE        Bool
#define HAL_WS_RDCOMP             HAL_WS_RDCOMP_MASK
#define HAL_WS_RDCOMP_READABLE    1
#define HAL_WS_RDCOMP_WRITABLE    1
#define HAL_WS_RDCOMP_SIGNED      0
#define HAL_WS_RDCOMP_ADR         0x10000FB2

#define HAL_WS_BUFSFUL_REGISTER    (HAL_WSREG_MAP->WSSTS0)
#define HAL_WS_BUFSFUL_LSB         0
#define HAL_WS_BUFSFUL_MSB         0
#define HAL_WS_BUFSFUL_MASK        0x00000001
#define HAL_WS_BUFSFUL_TYPE        Bool
#define HAL_WS_BUFSFUL             HAL_WS_BUFSFUL_MASK
#define HAL_WS_BUFSFUL_READABLE    1
#define HAL_WS_BUFSFUL_WRITABLE    0
#define HAL_WS_BUFSFUL_SIGNED      0
#define HAL_WS_BUFSFUL_ADR         0x10000FB2

// Register WSMISC1
#define HAL_WS_MKEXT_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_MKEXT_LSB         7
#define HAL_WS_MKEXT_MSB         7
#define HAL_WS_MKEXT_MASK        0x00000080
#define HAL_WS_MKEXT_TYPE        Bool
#define HAL_WS_MKEXT             HAL_WS_MKEXT_MASK
#define HAL_WS_MKEXT_READABLE    1
#define HAL_WS_MKEXT_WRITABLE    1
#define HAL_WS_MKEXT_SIGNED      0
#define HAL_WS_MKEXT_ADR         0x10000FB3

#define HAL_WS_WGINV_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_WGINV_LSB         6
#define HAL_WS_WGINV_MSB         6
#define HAL_WS_WGINV_MASK        0x00000040
#define HAL_WS_WGINV_TYPE        Bool
#define HAL_WS_WGINV             HAL_WS_WGINV_MASK
#define HAL_WS_WGINV_READABLE    1
#define HAL_WS_WGINV_WRITABLE    1
#define HAL_WS_WGINV_SIGNED      0
#define HAL_WS_WGINV_ADR         0x10000FB3

#define HAL_WS_WGEN_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_WGEN_LSB         5
#define HAL_WS_WGEN_MSB         5
#define HAL_WS_WGEN_MASK        0x00000020
#define HAL_WS_WGEN_TYPE        halWS_WGCtrl_t
#define HAL_WS_WGEN             HAL_WS_WGEN_MASK
#define HAL_WS_WGEN_READABLE    1
#define HAL_WS_WGEN_WRITABLE    1
#define HAL_WS_WGEN_SIGNED      0
#define HAL_WS_WGEN_ADR         0x10000FB3

#define HAL_WS_WGLVL_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_WGLVL_LSB         4
#define HAL_WS_WGLVL_MSB         4
#define HAL_WS_WGLVL_MASK        0x00000010
#define HAL_WS_WGLVL_TYPE        Bool
#define HAL_WS_WGLVL             HAL_WS_WGLVL_MASK
#define HAL_WS_WGLVL_READABLE    1
#define HAL_WS_WGLVL_WRITABLE    1
#define HAL_WS_WGLVL_SIGNED      0
#define HAL_WS_WGLVL_ADR         0x10000FB3

#define HAL_WS_DCERASE_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_DCERASE_LSB         3
#define HAL_WS_DCERASE_MSB         3
#define HAL_WS_DCERASE_MASK        0x00000008
#define HAL_WS_DCERASE_TYPE        Bool
#define HAL_WS_DCERASE             HAL_WS_DCERASE_MASK
#define HAL_WS_DCERASE_READABLE    1
#define HAL_WS_DCERASE_WRITABLE    1
#define HAL_WS_DCERASE_SIGNED      0
#define HAL_WS_DCERASE_ADR         0x10000FB3

#define HAL_WS_LSWS_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_LSWS_LSB         2
#define HAL_WS_LSWS_MSB         2
#define HAL_WS_LSWS_MASK        0x00000004
#define HAL_WS_LSWS_TYPE        Bool
#define HAL_WS_LSWS             HAL_WS_LSWS_MASK
#define HAL_WS_LSWS_READABLE    1
#define HAL_WS_LSWS_WRITABLE    1
#define HAL_WS_LSWS_SIGNED      0
#define HAL_WS_LSWS_ADR         0x10000FB3

#define HAL_WS_EFM12_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_EFM12_LSB         1
#define HAL_WS_EFM12_MSB         1
#define HAL_WS_EFM12_MASK        0x00000002
#define HAL_WS_EFM12_TYPE        halWS_DCEraseEFM12_t
#define HAL_WS_EFM12             HAL_WS_EFM12_MASK
#define HAL_WS_EFM12_READABLE    1
#define HAL_WS_EFM12_WRITABLE    1
#define HAL_WS_EFM12_SIGNED      0
#define HAL_WS_EFM12_ADR         0x10000FB3

#define HAL_WS_TMODE_REGISTER    (HAL_WSREG_MAP->WSMISC1)
#define HAL_WS_TMODE_LSB         0
#define HAL_WS_TMODE_MSB         0
#define HAL_WS_TMODE_MASK        0x00000001
#define HAL_WS_TMODE_TYPE        Bool
#define HAL_WS_TMODE             HAL_WS_TMODE_MASK
#define HAL_WS_TMODE_READABLE    1
#define HAL_WS_TMODE_WRITABLE    1
#define HAL_WS_TMODE_SIGNED      0
#define HAL_WS_TMODE_ADR         0x10000FB3

// Register WSEXTERNAL
#define HAL_WS_EXTWSCK4_REGISTER    (HAL_WSREG_MAP->WSEXTERNAL)
#define HAL_WS_EXTWSCK4_LSB         3
#define HAL_WS_EXTWSCK4_MSB         3
#define HAL_WS_EXTWSCK4_MASK        0x00000008
#define HAL_WS_EXTWSCK4_TYPE        Bool
#define HAL_WS_EXTWSCK4             HAL_WS_EXTWSCK4_MASK
#define HAL_WS_EXTWSCK4_READABLE    1
#define HAL_WS_EXTWSCK4_WRITABLE    1
#define HAL_WS_EXTWSCK4_SIGNED      0
#define HAL_WS_EXTWSCK4_ADR         0x10000FB4

#define HAL_WS_EXTWS_REGISTER    (HAL_WSREG_MAP->WSEXTERNAL)
#define HAL_WS_EXTWS_LSB         2
#define HAL_WS_EXTWS_MSB         2
#define HAL_WS_EXTWS_MASK        0x00000004
#define HAL_WS_EXTWS_TYPE        Bool
#define HAL_WS_EXTWS             HAL_WS_EXTWS_MASK
#define HAL_WS_EXTWS_READABLE    1
#define HAL_WS_EXTWS_WRITABLE    1
#define HAL_WS_EXTWS_SIGNED      0
#define HAL_WS_EXTWS_ADR         0x10000FB4

#define HAL_WS_EXTWSSMP_REGISTER    (HAL_WSREG_MAP->WSEXTERNAL)
#define HAL_WS_EXTWSSMP_LSB         0
#define HAL_WS_EXTWSSMP_MSB         1
#define HAL_WS_EXTWSSMP_MASK        0x00000003
#define HAL_WS_EXTWSSMP_TYPE        halWS_ExtWSSampDly_t
#define HAL_WS_EXTWSSMP             HAL_WS_EXTWSSMP_MASK
#define HAL_WS_EXTWSSMP_READABLE    1
#define HAL_WS_EXTWSSMP_WRITABLE    1
#define HAL_WS_EXTWSSMP_SIGNED      0
#define HAL_WS_EXTWSSMP_ADR         0x10000FB4

// Register WSMISC2

#if (CHIP_REV >= 0xC0)

#define HAL_WS_PGTWOTEN_REGISTER    (HAL_WSREG_MAP->WSMISC2)
#define HAL_WS_PGTWOTEN_LSB         2
#define HAL_WS_PGTWOTEN_MSB         2
#define HAL_WS_PGTWOTEN_MASK        0x00000004
#define HAL_WS_PGTWOTEN_TYPE        Bool
#define HAL_WS_PGTWOTEN             HAL_WS_PGTWOTEN_MASK
#define HAL_WS_PGTWOTEN_READABLE    1
#define HAL_WS_PGTWOTEN_WRITABLE    1
#define HAL_WS_PGTWOTEN_SIGNED      0
#define HAL_WS_PGTWOTEN_ADR         0x10000FB5

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_WS_CUSRSTEN_REGISTER    (HAL_WSREG_MAP->WSMISC2)
#define HAL_WS_CUSRSTEN_LSB         1
#define HAL_WS_CUSRSTEN_MSB         1
#define HAL_WS_CUSRSTEN_MASK        0x00000002
#define HAL_WS_CUSRSTEN_TYPE        Bool
#define HAL_WS_CUSRSTEN             HAL_WS_CUSRSTEN_MASK
#define HAL_WS_CUSRSTEN_READABLE    1
#define HAL_WS_CUSRSTEN_WRITABLE    1
#define HAL_WS_CUSRSTEN_SIGNED      0
#define HAL_WS_CUSRSTEN_ADR         0x10000FB5

#define HAL_WS_FSTT32DLY_REGISTER    (HAL_WSREG_MAP->WSMISC2)
#define HAL_WS_FSTT32DLY_LSB         0
#define HAL_WS_FSTT32DLY_MSB         0
#define HAL_WS_FSTT32DLY_MASK        0x00000001
#define HAL_WS_FSTT32DLY_TYPE        halWS_FastT32Dly_t
#define HAL_WS_FSTT32DLY             HAL_WS_FSTT32DLY_MASK
#define HAL_WS_FSTT32DLY_READABLE    1
#define HAL_WS_FSTT32DLY_WRITABLE    1
#define HAL_WS_FSTT32DLY_SIGNED      0
#define HAL_WS_FSTT32DLY_ADR         0x10000FB5

// Register EFMCTRL0
#define HAL_WS_EFM3EN_REGISTER    (HAL_WSREG_MAP->EFMCTRL0)
#define HAL_WS_EFM3EN_LSB         2
#define HAL_WS_EFM3EN_MSB         2
#define HAL_WS_EFM3EN_MASK        0x00000004
#define HAL_WS_EFM3EN_TYPE        Bool
#define HAL_WS_EFM3EN             HAL_WS_EFM3EN_MASK
#define HAL_WS_EFM3EN_READABLE    1
#define HAL_WS_EFM3EN_WRITABLE    1
#define HAL_WS_EFM3EN_SIGNED      0
#define HAL_WS_EFM3EN_ADR         0x10000FB6

#define HAL_WS_EFM2EN_REGISTER    (HAL_WSREG_MAP->EFMCTRL0)
#define HAL_WS_EFM2EN_LSB         1
#define HAL_WS_EFM2EN_MSB         1
#define HAL_WS_EFM2EN_MASK        0x00000002
#define HAL_WS_EFM2EN_TYPE        Bool
#define HAL_WS_EFM2EN             HAL_WS_EFM2EN_MASK
#define HAL_WS_EFM2EN_READABLE    1
#define HAL_WS_EFM2EN_WRITABLE    1
#define HAL_WS_EFM2EN_SIGNED      0
#define HAL_WS_EFM2EN_ADR         0x10000FB6

#define HAL_WS_EFM1EN_REGISTER    (HAL_WSREG_MAP->EFMCTRL0)
#define HAL_WS_EFM1EN_LSB         0
#define HAL_WS_EFM1EN_MSB         0
#define HAL_WS_EFM1EN_MASK        0x00000001
#define HAL_WS_EFM1EN_TYPE        Bool
#define HAL_WS_EFM1EN             HAL_WS_EFM1EN_MASK
#define HAL_WS_EFM1EN_READABLE    1
#define HAL_WS_EFM1EN_WRITABLE    1
#define HAL_WS_EFM1EN_SIGNED      0
#define HAL_WS_EFM1EN_ADR         0x10000FB6

// Register EFMCTRL1
#define HAL_WS_EFM3FIX_REGISTER    (HAL_WSREG_MAP->EFMCTRL1)
#define HAL_WS_EFM3FIX_LSB         2
#define HAL_WS_EFM3FIX_MSB         2
#define HAL_WS_EFM3FIX_MASK        0x00000004
#define HAL_WS_EFM3FIX_TYPE        Bool
#define HAL_WS_EFM3FIX             HAL_WS_EFM3FIX_MASK
#define HAL_WS_EFM3FIX_READABLE    1
#define HAL_WS_EFM3FIX_WRITABLE    1
#define HAL_WS_EFM3FIX_SIGNED      0
#define HAL_WS_EFM3FIX_ADR         0x10000FB7

#define HAL_WS_EFM2FIX_REGISTER    (HAL_WSREG_MAP->EFMCTRL1)
#define HAL_WS_EFM2FIX_LSB         1
#define HAL_WS_EFM2FIX_MSB         1
#define HAL_WS_EFM2FIX_MASK        0x00000002
#define HAL_WS_EFM2FIX_TYPE        Bool
#define HAL_WS_EFM2FIX             HAL_WS_EFM2FIX_MASK
#define HAL_WS_EFM2FIX_READABLE    1
#define HAL_WS_EFM2FIX_WRITABLE    1
#define HAL_WS_EFM2FIX_SIGNED      0
#define HAL_WS_EFM2FIX_ADR         0x10000FB7

#define HAL_WS_EFM1FIX_REGISTER    (HAL_WSREG_MAP->EFMCTRL1)
#define HAL_WS_EFM1FIX_LSB         0
#define HAL_WS_EFM1FIX_MSB         0
#define HAL_WS_EFM1FIX_MASK        0x00000001
#define HAL_WS_EFM1FIX_TYPE        Bool
#define HAL_WS_EFM1FIX             HAL_WS_EFM1FIX_MASK
#define HAL_WS_EFM1FIX_READABLE    1
#define HAL_WS_EFM1FIX_WRITABLE    1
#define HAL_WS_EFM1FIX_SIGNED      0
#define HAL_WS_EFM1FIX_ADR         0x10000FB7

// Register EFMCTRL2
#define HAL_WS_EFM3INVWG_REGISTER    (HAL_WSREG_MAP->EFMCTRL2)
#define HAL_WS_EFM3INVWG_LSB         2
#define HAL_WS_EFM3INVWG_MSB         2
#define HAL_WS_EFM3INVWG_MASK        0x00000004
#define HAL_WS_EFM3INVWG_TYPE        Bool
#define HAL_WS_EFM3INVWG             HAL_WS_EFM3INVWG_MASK
#define HAL_WS_EFM3INVWG_READABLE    1
#define HAL_WS_EFM3INVWG_WRITABLE    1
#define HAL_WS_EFM3INVWG_SIGNED      0
#define HAL_WS_EFM3INVWG_ADR         0x10000FB8

#define HAL_WS_EFM2INVWG_REGISTER    (HAL_WSREG_MAP->EFMCTRL2)
#define HAL_WS_EFM2INVWG_LSB         1
#define HAL_WS_EFM2INVWG_MSB         1
#define HAL_WS_EFM2INVWG_MASK        0x00000002
#define HAL_WS_EFM2INVWG_TYPE        Bool
#define HAL_WS_EFM2INVWG             HAL_WS_EFM2INVWG_MASK
#define HAL_WS_EFM2INVWG_READABLE    1
#define HAL_WS_EFM2INVWG_WRITABLE    1
#define HAL_WS_EFM2INVWG_SIGNED      0
#define HAL_WS_EFM2INVWG_ADR         0x10000FB8

#define HAL_WS_EFM1INVWG_REGISTER    (HAL_WSREG_MAP->EFMCTRL2)
#define HAL_WS_EFM1INVWG_LSB         0
#define HAL_WS_EFM1INVWG_MSB         0
#define HAL_WS_EFM1INVWG_MASK        0x00000001
#define HAL_WS_EFM1INVWG_TYPE        Bool
#define HAL_WS_EFM1INVWG             HAL_WS_EFM1INVWG_MASK
#define HAL_WS_EFM1INVWG_READABLE    1
#define HAL_WS_EFM1INVWG_WRITABLE    1
#define HAL_WS_EFM1INVWG_SIGNED      0
#define HAL_WS_EFM1INVWG_ADR         0x10000FB8

// Register EFMCTRL3
#define HAL_WS_EFM3INVWN_REGISTER    (HAL_WSREG_MAP->EFMCTRL3)
#define HAL_WS_EFM3INVWN_LSB         2
#define HAL_WS_EFM3INVWN_MSB         2
#define HAL_WS_EFM3INVWN_MASK        0x00000004
#define HAL_WS_EFM3INVWN_TYPE        Bool
#define HAL_WS_EFM3INVWN             HAL_WS_EFM3INVWN_MASK
#define HAL_WS_EFM3INVWN_READABLE    1
#define HAL_WS_EFM3INVWN_WRITABLE    1
#define HAL_WS_EFM3INVWN_SIGNED      0
#define HAL_WS_EFM3INVWN_ADR         0x10000FB9

#define HAL_WS_EFM2INVWN_REGISTER    (HAL_WSREG_MAP->EFMCTRL3)
#define HAL_WS_EFM2INVWN_LSB         1
#define HAL_WS_EFM2INVWN_MSB         1
#define HAL_WS_EFM2INVWN_MASK        0x00000002
#define HAL_WS_EFM2INVWN_TYPE        Bool
#define HAL_WS_EFM2INVWN             HAL_WS_EFM2INVWN_MASK
#define HAL_WS_EFM2INVWN_READABLE    1
#define HAL_WS_EFM2INVWN_WRITABLE    1
#define HAL_WS_EFM2INVWN_SIGNED      0
#define HAL_WS_EFM2INVWN_ADR         0x10000FB9

#define HAL_WS_EFM1INVWN_REGISTER    (HAL_WSREG_MAP->EFMCTRL3)
#define HAL_WS_EFM1INVWN_LSB         0
#define HAL_WS_EFM1INVWN_MSB         0
#define HAL_WS_EFM1INVWN_MASK        0x00000001
#define HAL_WS_EFM1INVWN_TYPE        Bool
#define HAL_WS_EFM1INVWN             HAL_WS_EFM1INVWN_MASK
#define HAL_WS_EFM1INVWN_READABLE    1
#define HAL_WS_EFM1INVWN_WRITABLE    1
#define HAL_WS_EFM1INVWN_SIGNED      0
#define HAL_WS_EFM1INVWN_ADR         0x10000FB9

// Register OPUDLYV
#define HAL_WS_OPUDELAY_REGISTER    (HAL_WSREG_MAP->OPUDLYV)
#define HAL_WS_OPUDELAY_LSB         0
#define HAL_WS_OPUDELAY_MSB         7
#define HAL_WS_OPUDELAY_MASK        0x000000FF
#define HAL_WS_OPUDELAY_TYPE        UByte
#define HAL_WS_OPUDELAY             HAL_WS_OPUDELAY_MASK
#define HAL_WS_OPUDELAY_READABLE    1
#define HAL_WS_OPUDELAY_WRITABLE    1
#define HAL_WS_OPUDELAY_SIGNED      0
#define HAL_WS_OPUDELAY_ADR         0x10000FBA

// Register TBGMN
#define HAL_WS_TBGN_REGISTER    (HAL_WSREG_MAP->TBGMN)
#define HAL_WS_TBGN_LSB         4
#define HAL_WS_TBGN_MSB         7
#define HAL_WS_TBGN_MASK        0x000000F0
#define HAL_WS_TBGN_TYPE        halWS_TBGMN_t
#define HAL_WS_TBGN             HAL_WS_TBGN_MASK
#define HAL_WS_TBGN_READABLE    1
#define HAL_WS_TBGN_WRITABLE    1
#define HAL_WS_TBGN_SIGNED      0
#define HAL_WS_TBGN_ADR         0x10000FBB

#define HAL_WS_TBGM_REGISTER    (HAL_WSREG_MAP->TBGMN)
#define HAL_WS_TBGM_LSB         0
#define HAL_WS_TBGM_MSB         3
#define HAL_WS_TBGM_MASK        0x0000000F
#define HAL_WS_TBGM_TYPE        UByte
#define HAL_WS_TBGM             HAL_WS_TBGM_MASK
#define HAL_WS_TBGM_READABLE    1
#define HAL_WS_TBGM_WRITABLE    1
#define HAL_WS_TBGM_SIGNED      0
#define HAL_WS_TBGM_ADR         0x10000FBB

// Register TBGCTL
#define HAL_WS_TBGSLPEN_REGISTER    (HAL_WSREG_MAP->TBGCTL)
#define HAL_WS_TBGSLPEN_LSB         6
#define HAL_WS_TBGSLPEN_MSB         6
#define HAL_WS_TBGSLPEN_MASK        0x00000040
#define HAL_WS_TBGSLPEN_TYPE        Bool
#define HAL_WS_TBGSLPEN             HAL_WS_TBGSLPEN_MASK
#define HAL_WS_TBGSLPEN_READABLE    1
#define HAL_WS_TBGSLPEN_WRITABLE    1
#define HAL_WS_TBGSLPEN_SIGNED      0
#define HAL_WS_TBGSLPEN_ADR         0x10000FBC

#define HAL_WS_TBGK_REGISTER    (HAL_WSREG_MAP->TBGCTL)
#define HAL_WS_TBGK_LSB         4
#define HAL_WS_TBGK_MSB         5
#define HAL_WS_TBGK_MASK        0x00000030
#define HAL_WS_TBGK_TYPE        halWS_WSTBGK_t
#define HAL_WS_TBGK             HAL_WS_TBGK_MASK
#define HAL_WS_TBGK_READABLE    1
#define HAL_WS_TBGK_WRITABLE    1
#define HAL_WS_TBGK_SIGNED      0
#define HAL_WS_TBGK_ADR         0x10000FBC

#define HAL_WS_TBGL_REGISTER    (HAL_WSREG_MAP->TBGCTL)
#define HAL_WS_TBGL_LSB         2
#define HAL_WS_TBGL_MSB         3
#define HAL_WS_TBGL_MASK        0x0000000C
#define HAL_WS_TBGL_TYPE        halWS_WSTBGL_t
#define HAL_WS_TBGL             HAL_WS_TBGL_MASK
#define HAL_WS_TBGL_READABLE    1
#define HAL_WS_TBGL_WRITABLE    1
#define HAL_WS_TBGL_SIGNED      0
#define HAL_WS_TBGL_ADR         0x10000FBC

#define HAL_WS_WSCEN_REGISTER    (HAL_WSREG_MAP->TBGCTL)
#define HAL_WS_WSCEN_LSB         1
#define HAL_WS_WSCEN_MSB         1
#define HAL_WS_WSCEN_MASK        0x00000002
#define HAL_WS_WSCEN_TYPE        halWS_WSCEN_t
#define HAL_WS_WSCEN             HAL_WS_WSCEN_MASK
#define HAL_WS_WSCEN_READABLE    1
#define HAL_WS_WSCEN_WRITABLE    1
#define HAL_WS_WSCEN_SIGNED      0
#define HAL_WS_WSCEN_ADR         0x10000FBC

#define HAL_WS_TBGPWR_REGISTER    (HAL_WSREG_MAP->TBGCTL)
#define HAL_WS_TBGPWR_LSB         0
#define HAL_WS_TBGPWR_MSB         0
#define HAL_WS_TBGPWR_MASK        0x00000001
#define HAL_WS_TBGPWR_TYPE        Bool
#define HAL_WS_TBGPWR             HAL_WS_TBGPWR_MASK
#define HAL_WS_TBGPWR_READABLE    1
#define HAL_WS_TBGPWR_WRITABLE    1
#define HAL_WS_TBGPWR_SIGNED      0
#define HAL_WS_TBGPWR_ADR         0x10000FBC

// Register TESTINST
#define HAL_WS_SMPINST_REGISTER    (HAL_WSREG_MAP->TESTINST)
#define HAL_WS_SMPINST_LSB         6
#define HAL_WS_SMPINST_MSB         7
#define HAL_WS_SMPINST_MASK        0x000000C0
#define HAL_WS_SMPINST_TYPE        UByte
#define HAL_WS_SMPINST             HAL_WS_SMPINST_MASK
#define HAL_WS_SMPINST_READABLE    1
#define HAL_WS_SMPINST_WRITABLE    1
#define HAL_WS_SMPINST_SIGNED      0
#define HAL_WS_SMPINST_ADR         0x10000FBD

#define HAL_WS_EFMINST_REGISTER    (HAL_WSREG_MAP->TESTINST)
#define HAL_WS_EFMINST_LSB         0
#define HAL_WS_EFMINST_MSB         5
#define HAL_WS_EFMINST_MASK        0x0000003F
#define HAL_WS_EFMINST_TYPE        UByte
#define HAL_WS_EFMINST             HAL_WS_EFMINST_MASK
#define HAL_WS_EFMINST_READABLE    1
#define HAL_WS_EFMINST_WRITABLE    1
#define HAL_WS_EFMINST_SIGNED      0
#define HAL_WS_EFMINST_ADR         0x10000FBD

// Register TESTTENA
#define HAL_WS_T32INST_REGISTER    (HAL_WSREG_MAP->TESTTENA)
#define HAL_WS_T32INST_LSB         6
#define HAL_WS_T32INST_MSB         7
#define HAL_WS_T32INST_MASK        0x000000C0
#define HAL_WS_T32INST_TYPE        UByte
#define HAL_WS_T32INST             HAL_WS_T32INST_MASK
#define HAL_WS_T32INST_READABLE    1
#define HAL_WS_T32INST_WRITABLE    1
#define HAL_WS_T32INST_SIGNED      0
#define HAL_WS_T32INST_ADR         0x10000FBE

#define HAL_WS_TSTCTR_REGISTER    (HAL_WSREG_MAP->TESTTENA)
#define HAL_WS_TSTCTR_LSB         2
#define HAL_WS_TSTCTR_MSB         4
#define HAL_WS_TSTCTR_MASK        0x0000001C
#define HAL_WS_TSTCTR_TYPE        UByte
#define HAL_WS_TSTCTR             HAL_WS_TSTCTR_MASK
#define HAL_WS_TSTCTR_READABLE    1
#define HAL_WS_TSTCTR_WRITABLE    1
#define HAL_WS_TSTCTR_SIGNED      0
#define HAL_WS_TSTCTR_ADR         0x10000FBE

#define HAL_WS_TSTPHSENA_REGISTER    (HAL_WSREG_MAP->TESTTENA)
#define HAL_WS_TSTPHSENA_LSB         0
#define HAL_WS_TSTPHSENA_MSB         0
#define HAL_WS_TSTPHSENA_MASK        0x00000001
#define HAL_WS_TSTPHSENA_TYPE        Bool
#define HAL_WS_TSTPHSENA             HAL_WS_TSTPHSENA_MASK
#define HAL_WS_TSTPHSENA_READABLE    1
#define HAL_WS_TSTPHSENA_WRITABLE    1
#define HAL_WS_TSTPHSENA_SIGNED      0
#define HAL_WS_TSTPHSENA_ADR         0x10000FBE

// Register TESTEFMV
#define HAL_WS_EFMVAL_REGISTER    (HAL_WSREG_MAP->TESTEFMV)
#define HAL_WS_EFMVAL_LSB         0
#define HAL_WS_EFMVAL_MSB         2
#define HAL_WS_EFMVAL_MASK        0x00000007
#define HAL_WS_EFMVAL_TYPE        UByte
#define HAL_WS_EFMVAL             HAL_WS_EFMVAL_MASK
#define HAL_WS_EFMVAL_READABLE    1
#define HAL_WS_EFMVAL_WRITABLE    0
#define HAL_WS_EFMVAL_SIGNED      0
#define HAL_WS_EFMVAL_ADR         0x10000FBF

// Register PRINTS
#define HAL_WS_WGUP_REGISTER    (HAL_WSREG_MAP->PRINTS)
#define HAL_WS_WGUP_LSB         6
#define HAL_WS_WGUP_MSB         6
#define HAL_WS_WGUP_MASK        0x00000040
#define HAL_WS_WGUP_TYPE        Bool
#define HAL_WS_WGUP             HAL_WS_WGUP_MASK
#define HAL_WS_WGUP_READABLE    1
#define HAL_WS_WGUP_WRITABLE    1
#define HAL_WS_WGUP_SIGNED      0
#define HAL_WS_WGUP_ADR         0x10000FC0

#define HAL_WS_WGDN_REGISTER    (HAL_WSREG_MAP->PRINTS)
#define HAL_WS_WGDN_LSB         5
#define HAL_WS_WGDN_MSB         5
#define HAL_WS_WGDN_MASK        0x00000020
#define HAL_WS_WGDN_TYPE        Bool
#define HAL_WS_WGDN             HAL_WS_WGDN_MASK
#define HAL_WS_WGDN_READABLE    1
#define HAL_WS_WGDN_WRITABLE    1
#define HAL_WS_WGDN_SIGNED      0
#define HAL_WS_WGDN_ADR         0x10000FC0

#define HAL_WS_ODDSMPS_REGISTER    (HAL_WSREG_MAP->PRINTS)
#define HAL_WS_ODDSMPS_LSB         2
#define HAL_WS_ODDSMPS_MSB         2
#define HAL_WS_ODDSMPS_MASK        0x00000004
#define HAL_WS_ODDSMPS_TYPE        Bool
#define HAL_WS_ODDSMPS             HAL_WS_ODDSMPS_MASK
#define HAL_WS_ODDSMPS_READABLE    1
#define HAL_WS_ODDSMPS_WRITABLE    1
#define HAL_WS_ODDSMPS_SIGNED      0
#define HAL_WS_ODDSMPS_ADR         0x10000FC0

#define HAL_WS_ODDEFMS_REGISTER    (HAL_WSREG_MAP->PRINTS)
#define HAL_WS_ODDEFMS_LSB         1
#define HAL_WS_ODDEFMS_MSB         1
#define HAL_WS_ODDEFMS_MASK        0x00000002
#define HAL_WS_ODDEFMS_TYPE        Bool
#define HAL_WS_ODDEFMS             HAL_WS_ODDEFMS_MASK
#define HAL_WS_ODDEFMS_READABLE    1
#define HAL_WS_ODDEFMS_WRITABLE    1
#define HAL_WS_ODDEFMS_SIGNED      0
#define HAL_WS_ODDEFMS_ADR         0x10000FC0

#define HAL_WS_ILLPATS_REGISTER    (HAL_WSREG_MAP->PRINTS)
#define HAL_WS_ILLPATS_LSB         0
#define HAL_WS_ILLPATS_MSB         0
#define HAL_WS_ILLPATS_MASK        0x00000001
#define HAL_WS_ILLPATS_TYPE        Bool
#define HAL_WS_ILLPATS             HAL_WS_ILLPATS_MASK
#define HAL_WS_ILLPATS_READABLE    1
#define HAL_WS_ILLPATS_WRITABLE    1
#define HAL_WS_ILLPATS_SIGNED      0
#define HAL_WS_ILLPATS_ADR         0x10000FC0

// Register PRINTEN
#define HAL_WS_WGUPEN_REGISTER    (HAL_WSREG_MAP->PRINTEN)
#define HAL_WS_WGUPEN_LSB         6
#define HAL_WS_WGUPEN_MSB         6
#define HAL_WS_WGUPEN_MASK        0x00000040
#define HAL_WS_WGUPEN_TYPE        Bool
#define HAL_WS_WGUPEN             HAL_WS_WGUPEN_MASK
#define HAL_WS_WGUPEN_READABLE    1
#define HAL_WS_WGUPEN_WRITABLE    1
#define HAL_WS_WGUPEN_SIGNED      0
#define HAL_WS_WGUPEN_ADR         0x10000FC1

#define HAL_WS_WGDNEN_REGISTER    (HAL_WSREG_MAP->PRINTEN)
#define HAL_WS_WGDNEN_LSB         5
#define HAL_WS_WGDNEN_MSB         5
#define HAL_WS_WGDNEN_MASK        0x00000020
#define HAL_WS_WGDNEN_TYPE        Bool
#define HAL_WS_WGDNEN             HAL_WS_WGDNEN_MASK
#define HAL_WS_WGDNEN_READABLE    1
#define HAL_WS_WGDNEN_WRITABLE    1
#define HAL_WS_WGDNEN_SIGNED      0
#define HAL_WS_WGDNEN_ADR         0x10000FC1

#define HAL_WS_ODDSMPEN_REGISTER    (HAL_WSREG_MAP->PRINTEN)
#define HAL_WS_ODDSMPEN_LSB         2
#define HAL_WS_ODDSMPEN_MSB         2
#define HAL_WS_ODDSMPEN_MASK        0x00000004
#define HAL_WS_ODDSMPEN_TYPE        Bool
#define HAL_WS_ODDSMPEN             HAL_WS_ODDSMPEN_MASK
#define HAL_WS_ODDSMPEN_READABLE    1
#define HAL_WS_ODDSMPEN_WRITABLE    1
#define HAL_WS_ODDSMPEN_SIGNED      0
#define HAL_WS_ODDSMPEN_ADR         0x10000FC1

#define HAL_WS_ODDEFMEN_REGISTER    (HAL_WSREG_MAP->PRINTEN)
#define HAL_WS_ODDEFMEN_LSB         1
#define HAL_WS_ODDEFMEN_MSB         1
#define HAL_WS_ODDEFMEN_MASK        0x00000002
#define HAL_WS_ODDEFMEN_TYPE        Bool
#define HAL_WS_ODDEFMEN             HAL_WS_ODDEFMEN_MASK
#define HAL_WS_ODDEFMEN_READABLE    1
#define HAL_WS_ODDEFMEN_WRITABLE    1
#define HAL_WS_ODDEFMEN_SIGNED      0
#define HAL_WS_ODDEFMEN_ADR         0x10000FC1

#define HAL_WS_ILLPATEN_REGISTER    (HAL_WSREG_MAP->PRINTEN)
#define HAL_WS_ILLPATEN_LSB         0
#define HAL_WS_ILLPATEN_MSB         0
#define HAL_WS_ILLPATEN_MASK        0x00000001
#define HAL_WS_ILLPATEN_TYPE        Bool
#define HAL_WS_ILLPATEN             HAL_WS_ILLPATEN_MASK
#define HAL_WS_ILLPATEN_READABLE    1
#define HAL_WS_ILLPATEN_WRITABLE    1
#define HAL_WS_ILLPATEN_SIGNED      0
#define HAL_WS_ILLPATEN_ADR         0x10000FC1

// Register CUSINTS
#define HAL_WS_SLPUPI_REGISTER    (HAL_WSREG_MAP->CUSINTS)
#define HAL_WS_SLPUPI_LSB         3
#define HAL_WS_SLPUPI_MSB         3
#define HAL_WS_SLPUPI_MASK        0x00000008
#define HAL_WS_SLPUPI_TYPE        Bool
#define HAL_WS_SLPUPI             HAL_WS_SLPUPI_MASK
#define HAL_WS_SLPUPI_READABLE    1
#define HAL_WS_SLPUPI_WRITABLE    1
#define HAL_WS_SLPUPI_SIGNED      0
#define HAL_WS_SLPUPI_ADR         0x10000FC2

#define HAL_WS_SLPDNI_REGISTER    (HAL_WSREG_MAP->CUSINTS)
#define HAL_WS_SLPDNI_LSB         2
#define HAL_WS_SLPDNI_MSB         2
#define HAL_WS_SLPDNI_MASK        0x00000004
#define HAL_WS_SLPDNI_TYPE        Bool
#define HAL_WS_SLPDNI             HAL_WS_SLPDNI_MASK
#define HAL_WS_SLPDNI_READABLE    1
#define HAL_WS_SLPDNI_WRITABLE    1
#define HAL_WS_SLPDNI_SIGNED      0
#define HAL_WS_SLPDNI_ADR         0x10000FC2

#define HAL_WS_TRERRS_REGISTER    (HAL_WSREG_MAP->CUSINTS)
#define HAL_WS_TRERRS_LSB         0
#define HAL_WS_TRERRS_MSB         0
#define HAL_WS_TRERRS_MASK        0x00000001
#define HAL_WS_TRERRS_TYPE        Bool
#define HAL_WS_TRERRS             HAL_WS_TRERRS_MASK
#define HAL_WS_TRERRS_READABLE    1
#define HAL_WS_TRERRS_WRITABLE    1
#define HAL_WS_TRERRS_SIGNED      0
#define HAL_WS_TRERRS_ADR         0x10000FC2

// Register CUSINTEN
#define HAL_WS_SLPUPIEN_REGISTER    (HAL_WSREG_MAP->CUSINTEN)
#define HAL_WS_SLPUPIEN_LSB         3
#define HAL_WS_SLPUPIEN_MSB         3
#define HAL_WS_SLPUPIEN_MASK        0x00000008
#define HAL_WS_SLPUPIEN_TYPE        Bool
#define HAL_WS_SLPUPIEN             HAL_WS_SLPUPIEN_MASK
#define HAL_WS_SLPUPIEN_READABLE    1
#define HAL_WS_SLPUPIEN_WRITABLE    1
#define HAL_WS_SLPUPIEN_SIGNED      0
#define HAL_WS_SLPUPIEN_ADR         0x10000FC3

#define HAL_WS_SLPDNIEN_REGISTER    (HAL_WSREG_MAP->CUSINTEN)
#define HAL_WS_SLPDNIEN_LSB         2
#define HAL_WS_SLPDNIEN_MSB         2
#define HAL_WS_SLPDNIEN_MASK        0x00000004
#define HAL_WS_SLPDNIEN_TYPE        Bool
#define HAL_WS_SLPDNIEN             HAL_WS_SLPDNIEN_MASK
#define HAL_WS_SLPDNIEN_READABLE    1
#define HAL_WS_SLPDNIEN_WRITABLE    1
#define HAL_WS_SLPDNIEN_SIGNED      0
#define HAL_WS_SLPDNIEN_ADR         0x10000FC3

#define HAL_WS_TRERREN_REGISTER    (HAL_WSREG_MAP->CUSINTEN)
#define HAL_WS_TRERREN_LSB         0
#define HAL_WS_TRERREN_MSB         0
#define HAL_WS_TRERREN_MASK        0x00000001
#define HAL_WS_TRERREN_TYPE        Bool
#define HAL_WS_TRERREN             HAL_WS_TRERREN_MASK
#define HAL_WS_TRERREN_READABLE    1
#define HAL_WS_TRERREN_WRITABLE    1
#define HAL_WS_TRERREN_SIGNED      0
#define HAL_WS_TRERREN_ADR         0x10000FC3

// Register TESTPHAS
#define HAL_WS_TSTPHASEIN_REGISTER    (HAL_WSREG_MAP->TESTPHAS)
#define HAL_WS_TSTPHASEIN_LSB         0
#define HAL_WS_TSTPHASEIN_MSB         7
#define HAL_WS_TSTPHASEIN_MASK        0x000000FF
#define HAL_WS_TSTPHASEIN_TYPE        UByte
#define HAL_WS_TSTPHASEIN             HAL_WS_TSTPHASEIN_MASK
#define HAL_WS_TSTPHASEIN_READABLE    1
#define HAL_WS_TSTPHASEIN_WRITABLE    1
#define HAL_WS_TSTPHASEIN_SIGNED      0
#define HAL_WS_TSTPHASEIN_ADR         0x10000FCA

// Register TESTESRC
#define HAL_WS_EFMSRC_REGISTER    (HAL_WSREG_MAP->TESTESRC)
#define HAL_WS_EFMSRC_LSB         0
#define HAL_WS_EFMSRC_MSB         1
#define HAL_WS_EFMSRC_MASK        0x00000003
#define HAL_WS_EFMSRC_TYPE        halWS_EFMSRC_t
#define HAL_WS_EFMSRC             HAL_WS_EFMSRC_MASK
#define HAL_WS_EFMSRC_READABLE    1
#define HAL_WS_EFMSRC_WRITABLE    1
#define HAL_WS_EFMSRC_SIGNED      0
#define HAL_WS_EFMSRC_ADR         0x10000FCB

// Register TESTPSMK
#define HAL_WS_PS_REGISTER    (HAL_WSREG_MAP->TESTPSMK)
#define HAL_WS_PS_LSB         4
#define HAL_WS_PS_MSB         7
#define HAL_WS_PS_MASK        0x000000F0
#define HAL_WS_PS_TYPE        UByte
#define HAL_WS_PS             HAL_WS_PS_MASK
#define HAL_WS_PS_READABLE    1
#define HAL_WS_PS_WRITABLE    1
#define HAL_WS_PS_SIGNED      0
#define HAL_WS_PS_ADR         0x10000FCC

#define HAL_WS_MK_REGISTER    (HAL_WSREG_MAP->TESTPSMK)
#define HAL_WS_MK_LSB         0
#define HAL_WS_MK_MSB         3
#define HAL_WS_MK_MASK        0x0000000F
#define HAL_WS_MK_TYPE        UByte
#define HAL_WS_MK             HAL_WS_MK_MASK
#define HAL_WS_MK_READABLE    1
#define HAL_WS_MK_WRITABLE    1
#define HAL_WS_MK_SIGNED      0
#define HAL_WS_MK_ADR         0x10000FCC

// Register TBGQPRS
#define HAL_WS_TBGQPRNG_REGISTER    (HAL_WSREG_MAP->TBGQPRS)
#define HAL_WS_TBGQPRNG_LSB         5
#define HAL_WS_TBGQPRNG_MSB         7
#define HAL_WS_TBGQPRNG_MASK        0x000000E0
#define HAL_WS_TBGQPRNG_TYPE        halWS_TBGCPOutRng_t
#define HAL_WS_TBGQPRNG             HAL_WS_TBGQPRNG_MASK
#define HAL_WS_TBGQPRNG_READABLE    1
#define HAL_WS_TBGQPRNG_WRITABLE    1
#define HAL_WS_TBGQPRNG_SIGNED      0
#define HAL_WS_TBGQPRNG_ADR         0x10000FCD

#define HAL_WS_QPSET_REGISTER    (HAL_WSREG_MAP->TBGQPRS)
#define HAL_WS_QPSET_LSB         0
#define HAL_WS_QPSET_MSB         4
#define HAL_WS_QPSET_MASK        0x0000001F
#define HAL_WS_QPSET_TYPE        UByte
#define HAL_WS_QPSET             HAL_WS_QPSET_MASK
#define HAL_WS_QPSET_READABLE    1
#define HAL_WS_QPSET_WRITABLE    1
#define HAL_WS_QPSET_SIGNED      0
#define HAL_WS_QPSET_ADR         0x10000FCD

// Register TBGMISC

#if (CHIP_REV <= 0xB3)

#define HAL_WS_FLUSHCAP_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_FLUSHCAP_LSB         7
#define HAL_WS_FLUSHCAP_MSB         7
#define HAL_WS_FLUSHCAP_MASK        0x00000080
#define HAL_WS_FLUSHCAP_TYPE        Bool
#define HAL_WS_FLUSHCAP             HAL_WS_FLUSHCAP_MASK
#define HAL_WS_FLUSHCAP_READABLE    1
#define HAL_WS_FLUSHCAP_WRITABLE    1
#define HAL_WS_FLUSHCAP_SIGNED      0
#define HAL_WS_FLUSHCAP_ADR         0x10000FCE

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_WS_ENLF2VRST_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_ENLF2VRST_LSB         7
#define HAL_WS_ENLF2VRST_MSB         7
#define HAL_WS_ENLF2VRST_MASK        0x00000080
#define HAL_WS_ENLF2VRST_TYPE        Bool
#define HAL_WS_ENLF2VRST             HAL_WS_ENLF2VRST_MASK
#define HAL_WS_ENLF2VRST_READABLE    1
#define HAL_WS_ENLF2VRST_WRITABLE    1
#define HAL_WS_ENLF2VRST_SIGNED      0
#define HAL_WS_ENLF2VRST_ADR         0x10000FCE

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_WS_TBGLFRSET_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_TBGLFRSET_LSB         5
#define HAL_WS_TBGLFRSET_MSB         6
#define HAL_WS_TBGLFRSET_MASK        0x00000060
#define HAL_WS_TBGLFRSET_TYPE        halWS_TBGLFResistor_t
#define HAL_WS_TBGLFRSET             HAL_WS_TBGLFRSET_MASK
#define HAL_WS_TBGLFRSET_READABLE    1
#define HAL_WS_TBGLFRSET_WRITABLE    1
#define HAL_WS_TBGLFRSET_SIGNED      0
#define HAL_WS_TBGLFRSET_ADR         0x10000FCE

#define HAL_WS_DEADBAND_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_DEADBAND_LSB         4
#define HAL_WS_DEADBAND_MSB         4
#define HAL_WS_DEADBAND_MASK        0x00000010
#define HAL_WS_DEADBAND_TYPE        Bool
#define HAL_WS_DEADBAND             HAL_WS_DEADBAND_MASK
#define HAL_WS_DEADBAND_READABLE    1
#define HAL_WS_DEADBAND_WRITABLE    1
#define HAL_WS_DEADBAND_SIGNED      0
#define HAL_WS_DEADBAND_ADR         0x10000FCE

#define HAL_WS_PHDETTST_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_PHDETTST_LSB         2
#define HAL_WS_PHDETTST_MSB         3
#define HAL_WS_PHDETTST_MASK        0x0000000C
#define HAL_WS_PHDETTST_TYPE        halWS_PhDetTst_t
#define HAL_WS_PHDETTST             HAL_WS_PHDETTST_MASK
#define HAL_WS_PHDETTST_READABLE    1
#define HAL_WS_PHDETTST_WRITABLE    1
#define HAL_WS_PHDETTST_SIGNED      0
#define HAL_WS_PHDETTST_ADR         0x10000FCE

#if (CHIP_REV <= 0xB3)

#define HAL_WS_ENVBUF_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_ENVBUF_LSB         1
#define HAL_WS_ENVBUF_MSB         1
#define HAL_WS_ENVBUF_MASK        0x00000002
#define HAL_WS_ENVBUF_TYPE        Bool
#define HAL_WS_ENVBUF             HAL_WS_ENVBUF_MASK
#define HAL_WS_ENVBUF_READABLE    1
#define HAL_WS_ENVBUF_WRITABLE    1
#define HAL_WS_ENVBUF_SIGNED      0
#define HAL_WS_ENVBUF_ADR         0x10000FCE

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_WS_ENLF1VTST_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_ENLF1VTST_LSB         1
#define HAL_WS_ENLF1VTST_MSB         1
#define HAL_WS_ENLF1VTST_MASK        0x00000002
#define HAL_WS_ENLF1VTST_TYPE        Bool
#define HAL_WS_ENLF1VTST             HAL_WS_ENLF1VTST_MASK
#define HAL_WS_ENLF1VTST_READABLE    1
#define HAL_WS_ENLF1VTST_WRITABLE    1
#define HAL_WS_ENLF1VTST_SIGNED      0
#define HAL_WS_ENLF1VTST_ADR         0x10000FCE

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_WS_TBGSLP2T_REGISTER    (HAL_WSREG_MAP->TBGMISC)
#define HAL_WS_TBGSLP2T_LSB         0
#define HAL_WS_TBGSLP2T_MSB         0
#define HAL_WS_TBGSLP2T_MASK        0x00000001
#define HAL_WS_TBGSLP2T_TYPE        halWS_TBGSlip_t
#define HAL_WS_TBGSLP2T             HAL_WS_TBGSLP2T_MASK
#define HAL_WS_TBGSLP2T_READABLE    1
#define HAL_WS_TBGSLP2T_WRITABLE    1
#define HAL_WS_TBGSLP2T_SIGNED      0
#define HAL_WS_TBGSLP2T_ADR         0x10000FCE

// Register SMP1CTRL
#define HAL_WS_SMP1INVWG_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1INVWG_LSB         7
#define HAL_WS_SMP1INVWG_MSB         7
#define HAL_WS_SMP1INVWG_MASK        0x00000080
#define HAL_WS_SMP1INVWG_TYPE        Bool
#define HAL_WS_SMP1INVWG             HAL_WS_SMP1INVWG_MASK
#define HAL_WS_SMP1INVWG_READABLE    1
#define HAL_WS_SMP1INVWG_WRITABLE    1
#define HAL_WS_SMP1INVWG_SIGNED      0
#define HAL_WS_SMP1INVWG_ADR         0x10000FCF

#define HAL_WS_SMP1FIX_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1FIX_LSB         6
#define HAL_WS_SMP1FIX_MSB         6
#define HAL_WS_SMP1FIX_MASK        0x00000040
#define HAL_WS_SMP1FIX_TYPE        Bool
#define HAL_WS_SMP1FIX             HAL_WS_SMP1FIX_MASK
#define HAL_WS_SMP1FIX_READABLE    1
#define HAL_WS_SMP1FIX_WRITABLE    1
#define HAL_WS_SMP1FIX_SIGNED      0
#define HAL_WS_SMP1FIX_ADR         0x10000FCF

#define HAL_WS_SMP1INVWN_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1INVWN_LSB         5
#define HAL_WS_SMP1INVWN_MSB         5
#define HAL_WS_SMP1INVWN_MASK        0x00000020
#define HAL_WS_SMP1INVWN_TYPE        Bool
#define HAL_WS_SMP1INVWN             HAL_WS_SMP1INVWN_MASK
#define HAL_WS_SMP1INVWN_READABLE    1
#define HAL_WS_SMP1INVWN_WRITABLE    1
#define HAL_WS_SMP1INVWN_SIGNED      0
#define HAL_WS_SMP1INVWN_ADR         0x10000FCF

#define HAL_WS_SMP1MKSP_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1MKSP_LSB         4
#define HAL_WS_SMP1MKSP_MSB         4
#define HAL_WS_SMP1MKSP_MASK        0x00000010
#define HAL_WS_SMP1MKSP_TYPE        halWS_SMPMarkSpace_t
#define HAL_WS_SMP1MKSP             HAL_WS_SMP1MKSP_MASK
#define HAL_WS_SMP1MKSP_READABLE    1
#define HAL_WS_SMP1MKSP_WRITABLE    1
#define HAL_WS_SMP1MKSP_SIGNED      0
#define HAL_WS_SMP1MKSP_ADR         0x10000FCF

#define HAL_WS_SMP1MODE_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1MODE_LSB         3
#define HAL_WS_SMP1MODE_MSB         3
#define HAL_WS_SMP1MODE_MASK        0x00000008
#define HAL_WS_SMP1MODE_TYPE        halWS_SMPFixVar_t
#define HAL_WS_SMP1MODE             HAL_WS_SMP1MODE_MASK
#define HAL_WS_SMP1MODE_READABLE    1
#define HAL_WS_SMP1MODE_WRITABLE    1
#define HAL_WS_SMP1MODE_SIGNED      0
#define HAL_WS_SMP1MODE_ADR         0x10000FCF

#define HAL_WS_SMP1EXT_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1EXT_LSB         2
#define HAL_WS_SMP1EXT_MSB         2
#define HAL_WS_SMP1EXT_MASK        0x00000004
#define HAL_WS_SMP1EXT_TYPE        halWS_SMPIntExt_t
#define HAL_WS_SMP1EXT             HAL_WS_SMP1EXT_MASK
#define HAL_WS_SMP1EXT_READABLE    1
#define HAL_WS_SMP1EXT_WRITABLE    1
#define HAL_WS_SMP1EXT_SIGNED      0
#define HAL_WS_SMP1EXT_ADR         0x10000FCF

#define HAL_WS_SMP1DLY0_REGISTER    (HAL_WSREG_MAP->SMP1CTRL)
#define HAL_WS_SMP1DLY0_LSB         1
#define HAL_WS_SMP1DLY0_MSB         1
#define HAL_WS_SMP1DLY0_MASK        0x00000002
#define HAL_WS_SMP1DLY0_TYPE        halWS_SMPOpuDelay_t
#define HAL_WS_SMP1DLY0             HAL_WS_SMP1DLY0_MASK
#define HAL_WS_SMP1DLY0_READABLE    1
#define HAL_WS_SMP1DLY0_WRITABLE    1
#define HAL_WS_SMP1DLY0_SIGNED      0
#define HAL_WS_SMP1DLY0_ADR         0x10000FCF

// Registers SMP1PATL/SMP1PATH
#define HAL_WS_SMP1PAT_REGISTER    (HAL_WSREG_MAP->SMP1PATL)
#define HAL_WS_SMP1PAT_LSB         0
#define HAL_WS_SMP1PAT_MSB         10
#define HAL_WS_SMP1PAT_MASK        0x000007FF
#define HAL_WS_SMP1PAT_TYPE        UInt16
#define HAL_WS_SMP1PAT             HAL_WS_SMP1PAT_MASK
#define HAL_WS_SMP1PAT_READABLE    1
#define HAL_WS_SMP1PAT_WRITABLE    1
#define HAL_WS_SMP1PAT_SIGNED      0
#define HAL_WS_SMP1PAT_ADR         0x10000FD0

#define HAL_WS_SMP1PAT15_REGISTER    (HAL_WSREG_MAP->SMP1PATL)
#define HAL_WS_SMP1PAT15_LSB         15
#define HAL_WS_SMP1PAT15_MSB         15
#define HAL_WS_SMP1PAT15_MASK        0x00008000
#define HAL_WS_SMP1PAT15_TYPE        Bool
#define HAL_WS_SMP1PAT15             HAL_WS_SMP1PAT15_MASK
#define HAL_WS_SMP1PAT15_READABLE    1
#define HAL_WS_SMP1PAT15_WRITABLE    1
#define HAL_WS_SMP1PAT15_SIGNED      0
#define HAL_WS_SMP1PAT15_ADR         0x10000FD0

// Register SMP1LDLY
#define HAL_WS_SMP1DLY_REGISTER    (HAL_WSREG_MAP->SMP1LDLY)
#define HAL_WS_SMP1DLY_LSB         0
#define HAL_WS_SMP1DLY_MSB         6
#define HAL_WS_SMP1DLY_MASK        0x0000007F
#define HAL_WS_SMP1DLY_TYPE        UByte
#define HAL_WS_SMP1DLY             HAL_WS_SMP1DLY_MASK
#define HAL_WS_SMP1DLY_READABLE    1
#define HAL_WS_SMP1DLY_WRITABLE    1
#define HAL_WS_SMP1DLY_SIGNED      0
#define HAL_WS_SMP1DLY_ADR         0x10000FD3

// Register SMP1WID
#define HAL_WS_SMP1WID_REGISTER    (HAL_WSREG_MAP->SMP1WID)
#define HAL_WS_SMP1WID_LSB         0
#define HAL_WS_SMP1WID_MSB         6
#define HAL_WS_SMP1WID_MASK        0x0000007F
#define HAL_WS_SMP1WID_TYPE        UByte
#define HAL_WS_SMP1WID             HAL_WS_SMP1WID_MASK
#define HAL_WS_SMP1WID_READABLE    1
#define HAL_WS_SMP1WID_WRITABLE    1
#define HAL_WS_SMP1WID_SIGNED      0
#define HAL_WS_SMP1WID_ADR         0x10000FD4

// Register SMP2CTRL
#define HAL_WS_SMP2INVWG_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2INVWG_LSB         7
#define HAL_WS_SMP2INVWG_MSB         7
#define HAL_WS_SMP2INVWG_MASK        0x00000080
#define HAL_WS_SMP2INVWG_TYPE        Bool
#define HAL_WS_SMP2INVWG             HAL_WS_SMP2INVWG_MASK
#define HAL_WS_SMP2INVWG_READABLE    1
#define HAL_WS_SMP2INVWG_WRITABLE    1
#define HAL_WS_SMP2INVWG_SIGNED      0
#define HAL_WS_SMP2INVWG_ADR         0x10000FD5

#define HAL_WS_SMP2FIX_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2FIX_LSB         6
#define HAL_WS_SMP2FIX_MSB         6
#define HAL_WS_SMP2FIX_MASK        0x00000040
#define HAL_WS_SMP2FIX_TYPE        Bool
#define HAL_WS_SMP2FIX             HAL_WS_SMP2FIX_MASK
#define HAL_WS_SMP2FIX_READABLE    1
#define HAL_WS_SMP2FIX_WRITABLE    1
#define HAL_WS_SMP2FIX_SIGNED      0
#define HAL_WS_SMP2FIX_ADR         0x10000FD5

#define HAL_WS_SMP2INVWN_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2INVWN_LSB         5
#define HAL_WS_SMP2INVWN_MSB         5
#define HAL_WS_SMP2INVWN_MASK        0x00000020
#define HAL_WS_SMP2INVWN_TYPE        Bool
#define HAL_WS_SMP2INVWN             HAL_WS_SMP2INVWN_MASK
#define HAL_WS_SMP2INVWN_READABLE    1
#define HAL_WS_SMP2INVWN_WRITABLE    1
#define HAL_WS_SMP2INVWN_SIGNED      0
#define HAL_WS_SMP2INVWN_ADR         0x10000FD5

#define HAL_WS_SMP2MKSP_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2MKSP_LSB         4
#define HAL_WS_SMP2MKSP_MSB         4
#define HAL_WS_SMP2MKSP_MASK        0x00000010
#define HAL_WS_SMP2MKSP_TYPE        halWS_SMPMarkSpace_t
#define HAL_WS_SMP2MKSP             HAL_WS_SMP2MKSP_MASK
#define HAL_WS_SMP2MKSP_READABLE    1
#define HAL_WS_SMP2MKSP_WRITABLE    1
#define HAL_WS_SMP2MKSP_SIGNED      0
#define HAL_WS_SMP2MKSP_ADR         0x10000FD5

#define HAL_WS_SMP2MODE_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2MODE_LSB         3
#define HAL_WS_SMP2MODE_MSB         3
#define HAL_WS_SMP2MODE_MASK        0x00000008
#define HAL_WS_SMP2MODE_TYPE        halWS_SMPFixVar_t
#define HAL_WS_SMP2MODE             HAL_WS_SMP2MODE_MASK
#define HAL_WS_SMP2MODE_READABLE    1
#define HAL_WS_SMP2MODE_WRITABLE    1
#define HAL_WS_SMP2MODE_SIGNED      0
#define HAL_WS_SMP2MODE_ADR         0x10000FD5

#define HAL_WS_SMP2EXT_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2EXT_LSB         2
#define HAL_WS_SMP2EXT_MSB         2
#define HAL_WS_SMP2EXT_MASK        0x00000004
#define HAL_WS_SMP2EXT_TYPE        halWS_SMPIntExt_t
#define HAL_WS_SMP2EXT             HAL_WS_SMP2EXT_MASK
#define HAL_WS_SMP2EXT_READABLE    1
#define HAL_WS_SMP2EXT_WRITABLE    1
#define HAL_WS_SMP2EXT_SIGNED      0
#define HAL_WS_SMP2EXT_ADR         0x10000FD5

#define HAL_WS_SMP2DLY0_REGISTER    (HAL_WSREG_MAP->SMP2CTRL)
#define HAL_WS_SMP2DLY0_LSB         1
#define HAL_WS_SMP2DLY0_MSB         1
#define HAL_WS_SMP2DLY0_MASK        0x00000002
#define HAL_WS_SMP2DLY0_TYPE        halWS_SMPOpuDelay_t
#define HAL_WS_SMP2DLY0             HAL_WS_SMP2DLY0_MASK
#define HAL_WS_SMP2DLY0_READABLE    1
#define HAL_WS_SMP2DLY0_WRITABLE    1
#define HAL_WS_SMP2DLY0_SIGNED      0
#define HAL_WS_SMP2DLY0_ADR         0x10000FD5

// Registers SMP2PATL/SMP2PATH
#define HAL_WS_SMP2PAT_REGISTER    (HAL_WSREG_MAP->SMP2PATL)
#define HAL_WS_SMP2PAT_LSB         0
#define HAL_WS_SMP2PAT_MSB         10
#define HAL_WS_SMP2PAT_MASK        0x000007FF
#define HAL_WS_SMP2PAT_TYPE        UInt16
#define HAL_WS_SMP2PAT             HAL_WS_SMP2PAT_MASK
#define HAL_WS_SMP2PAT_READABLE    1
#define HAL_WS_SMP2PAT_WRITABLE    1
#define HAL_WS_SMP2PAT_SIGNED      0
#define HAL_WS_SMP2PAT_ADR         0x10000FD6

#define HAL_WS_SMP2PAT15_REGISTER    (HAL_WSREG_MAP->SMP2PATL)
#define HAL_WS_SMP2PAT15_LSB         15
#define HAL_WS_SMP2PAT15_MSB         15
#define HAL_WS_SMP2PAT15_MASK        0x00008000
#define HAL_WS_SMP2PAT15_TYPE        Bool
#define HAL_WS_SMP2PAT15             HAL_WS_SMP2PAT15_MASK
#define HAL_WS_SMP2PAT15_READABLE    1
#define HAL_WS_SMP2PAT15_WRITABLE    1
#define HAL_WS_SMP2PAT15_SIGNED      0
#define HAL_WS_SMP2PAT15_ADR         0x10000FD6

// Register SMP2LDLY
#define HAL_WS_SMP2DLY_REGISTER    (HAL_WSREG_MAP->SMP2LDLY)
#define HAL_WS_SMP2DLY_LSB         0
#define HAL_WS_SMP2DLY_MSB         6
#define HAL_WS_SMP2DLY_MASK        0x0000007F
#define HAL_WS_SMP2DLY_TYPE        UByte
#define HAL_WS_SMP2DLY             HAL_WS_SMP2DLY_MASK
#define HAL_WS_SMP2DLY_READABLE    1
#define HAL_WS_SMP2DLY_WRITABLE    1
#define HAL_WS_SMP2DLY_SIGNED      0
#define HAL_WS_SMP2DLY_ADR         0x10000FD8

// Register SMP2WID
#define HAL_WS_SMP2WID_REGISTER    (HAL_WSREG_MAP->SMP2WID)
#define HAL_WS_SMP2WID_LSB         0
#define HAL_WS_SMP2WID_MSB         6
#define HAL_WS_SMP2WID_MASK        0x0000007F
#define HAL_WS_SMP2WID_TYPE        UByte
#define HAL_WS_SMP2WID             HAL_WS_SMP2WID_MASK
#define HAL_WS_SMP2WID_READABLE    1
#define HAL_WS_SMP2WID_WRITABLE    1
#define HAL_WS_SMP2WID_SIGNED      0
#define HAL_WS_SMP2WID_ADR         0x10000FD9

// Register SMP3CTRL
#define HAL_WS_SMP3INVWG_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3INVWG_LSB         7
#define HAL_WS_SMP3INVWG_MSB         7
#define HAL_WS_SMP3INVWG_MASK        0x00000080
#define HAL_WS_SMP3INVWG_TYPE        Bool
#define HAL_WS_SMP3INVWG             HAL_WS_SMP3INVWG_MASK
#define HAL_WS_SMP3INVWG_READABLE    1
#define HAL_WS_SMP3INVWG_WRITABLE    1
#define HAL_WS_SMP3INVWG_SIGNED      0
#define HAL_WS_SMP3INVWG_ADR         0x10000FDA

#define HAL_WS_SMP3FIX_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3FIX_LSB         6
#define HAL_WS_SMP3FIX_MSB         6
#define HAL_WS_SMP3FIX_MASK        0x00000040
#define HAL_WS_SMP3FIX_TYPE        Bool
#define HAL_WS_SMP3FIX             HAL_WS_SMP3FIX_MASK
#define HAL_WS_SMP3FIX_READABLE    1
#define HAL_WS_SMP3FIX_WRITABLE    1
#define HAL_WS_SMP3FIX_SIGNED      0
#define HAL_WS_SMP3FIX_ADR         0x10000FDA

#define HAL_WS_SMP3INVWN_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3INVWN_LSB         5
#define HAL_WS_SMP3INVWN_MSB         5
#define HAL_WS_SMP3INVWN_MASK        0x00000020
#define HAL_WS_SMP3INVWN_TYPE        Bool
#define HAL_WS_SMP3INVWN             HAL_WS_SMP3INVWN_MASK
#define HAL_WS_SMP3INVWN_READABLE    1
#define HAL_WS_SMP3INVWN_WRITABLE    1
#define HAL_WS_SMP3INVWN_SIGNED      0
#define HAL_WS_SMP3INVWN_ADR         0x10000FDA

#define HAL_WS_SMP3MKSP_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3MKSP_LSB         4
#define HAL_WS_SMP3MKSP_MSB         4
#define HAL_WS_SMP3MKSP_MASK        0x00000010
#define HAL_WS_SMP3MKSP_TYPE        halWS_SMPMarkSpace_t
#define HAL_WS_SMP3MKSP             HAL_WS_SMP3MKSP_MASK
#define HAL_WS_SMP3MKSP_READABLE    1
#define HAL_WS_SMP3MKSP_WRITABLE    1
#define HAL_WS_SMP3MKSP_SIGNED      0
#define HAL_WS_SMP3MKSP_ADR         0x10000FDA

#define HAL_WS_SMP3MODE_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3MODE_LSB         3
#define HAL_WS_SMP3MODE_MSB         3
#define HAL_WS_SMP3MODE_MASK        0x00000008
#define HAL_WS_SMP3MODE_TYPE        halWS_SMPFixVar_t
#define HAL_WS_SMP3MODE             HAL_WS_SMP3MODE_MASK
#define HAL_WS_SMP3MODE_READABLE    1
#define HAL_WS_SMP3MODE_WRITABLE    1
#define HAL_WS_SMP3MODE_SIGNED      0
#define HAL_WS_SMP3MODE_ADR         0x10000FDA

#define HAL_WS_SMP3EXT_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3EXT_LSB         2
#define HAL_WS_SMP3EXT_MSB         2
#define HAL_WS_SMP3EXT_MASK        0x00000004
#define HAL_WS_SMP3EXT_TYPE        halWS_SMPIntExt_t
#define HAL_WS_SMP3EXT             HAL_WS_SMP3EXT_MASK
#define HAL_WS_SMP3EXT_READABLE    1
#define HAL_WS_SMP3EXT_WRITABLE    1
#define HAL_WS_SMP3EXT_SIGNED      0
#define HAL_WS_SMP3EXT_ADR         0x10000FDA

#define HAL_WS_SMP3DLY0_REGISTER    (HAL_WSREG_MAP->SMP3CTRL)
#define HAL_WS_SMP3DLY0_LSB         1
#define HAL_WS_SMP3DLY0_MSB         1
#define HAL_WS_SMP3DLY0_MASK        0x00000002
#define HAL_WS_SMP3DLY0_TYPE        halWS_SMPOpuDelay_t
#define HAL_WS_SMP3DLY0             HAL_WS_SMP3DLY0_MASK
#define HAL_WS_SMP3DLY0_READABLE    1
#define HAL_WS_SMP3DLY0_WRITABLE    1
#define HAL_WS_SMP3DLY0_SIGNED      0
#define HAL_WS_SMP3DLY0_ADR         0x10000FDA

// Registers SMP3PATL/SMP3PATH
#define HAL_WS_SMP3PAT_REGISTER    (HAL_WSREG_MAP->SMP3PATL)
#define HAL_WS_SMP3PAT_LSB         0
#define HAL_WS_SMP3PAT_MSB         10
#define HAL_WS_SMP3PAT_MASK        0x000007FF
#define HAL_WS_SMP3PAT_TYPE        UInt16
#define HAL_WS_SMP3PAT             HAL_WS_SMP3PAT_MASK
#define HAL_WS_SMP3PAT_READABLE    1
#define HAL_WS_SMP3PAT_WRITABLE    1
#define HAL_WS_SMP3PAT_SIGNED      0
#define HAL_WS_SMP3PAT_ADR         0x10000FDC

#define HAL_WS_SMP3PAT15_REGISTER    (HAL_WSREG_MAP->SMP3PATL)
#define HAL_WS_SMP3PAT15_LSB         15
#define HAL_WS_SMP3PAT15_MSB         15
#define HAL_WS_SMP3PAT15_MASK        0x00008000
#define HAL_WS_SMP3PAT15_TYPE        Bool
#define HAL_WS_SMP3PAT15             HAL_WS_SMP3PAT15_MASK
#define HAL_WS_SMP3PAT15_READABLE    1
#define HAL_WS_SMP3PAT15_WRITABLE    1
#define HAL_WS_SMP3PAT15_SIGNED      0
#define HAL_WS_SMP3PAT15_ADR         0x10000FDC

// Register SMP3LDLY
#define HAL_WS_SMP3DLY_REGISTER    (HAL_WSREG_MAP->SMP3LDLY)
#define HAL_WS_SMP3DLY_LSB         0
#define HAL_WS_SMP3DLY_MSB         6
#define HAL_WS_SMP3DLY_MASK        0x0000007F
#define HAL_WS_SMP3DLY_TYPE        UByte
#define HAL_WS_SMP3DLY             HAL_WS_SMP3DLY_MASK
#define HAL_WS_SMP3DLY_READABLE    1
#define HAL_WS_SMP3DLY_WRITABLE    1
#define HAL_WS_SMP3DLY_SIGNED      0
#define HAL_WS_SMP3DLY_ADR         0x10000FDE

// Register SMP3WID
#define HAL_WS_SMP3WID_REGISTER    (HAL_WSREG_MAP->SMP3WID)
#define HAL_WS_SMP3WID_LSB         0
#define HAL_WS_SMP3WID_MSB         6
#define HAL_WS_SMP3WID_MASK        0x0000007F
#define HAL_WS_SMP3WID_TYPE        UByte
#define HAL_WS_SMP3WID             HAL_WS_SMP3WID_MASK
#define HAL_WS_SMP3WID_READABLE    1
#define HAL_WS_SMP3WID_WRITABLE    1
#define HAL_WS_SMP3WID_SIGNED      0
#define HAL_WS_SMP3WID_ADR         0x10000FDF

// Register SMP4CTRL
#define HAL_WS_SMP4INVWG_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4INVWG_LSB         7
#define HAL_WS_SMP4INVWG_MSB         7
#define HAL_WS_SMP4INVWG_MASK        0x00000080
#define HAL_WS_SMP4INVWG_TYPE        Bool
#define HAL_WS_SMP4INVWG             HAL_WS_SMP4INVWG_MASK
#define HAL_WS_SMP4INVWG_READABLE    1
#define HAL_WS_SMP4INVWG_WRITABLE    1
#define HAL_WS_SMP4INVWG_SIGNED      0
#define HAL_WS_SMP4INVWG_ADR         0x10000FE0

#define HAL_WS_SMP4FIX_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4FIX_LSB         6
#define HAL_WS_SMP4FIX_MSB         6
#define HAL_WS_SMP4FIX_MASK        0x00000040
#define HAL_WS_SMP4FIX_TYPE        Bool
#define HAL_WS_SMP4FIX             HAL_WS_SMP4FIX_MASK
#define HAL_WS_SMP4FIX_READABLE    1
#define HAL_WS_SMP4FIX_WRITABLE    1
#define HAL_WS_SMP4FIX_SIGNED      0
#define HAL_WS_SMP4FIX_ADR         0x10000FE0

#define HAL_WS_SMP4INVWN_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4INVWN_LSB         5
#define HAL_WS_SMP4INVWN_MSB         5
#define HAL_WS_SMP4INVWN_MASK        0x00000020
#define HAL_WS_SMP4INVWN_TYPE        Bool
#define HAL_WS_SMP4INVWN             HAL_WS_SMP4INVWN_MASK
#define HAL_WS_SMP4INVWN_READABLE    1
#define HAL_WS_SMP4INVWN_WRITABLE    1
#define HAL_WS_SMP4INVWN_SIGNED      0
#define HAL_WS_SMP4INVWN_ADR         0x10000FE0

#define HAL_WS_SMP4MKSP_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4MKSP_LSB         4
#define HAL_WS_SMP4MKSP_MSB         4
#define HAL_WS_SMP4MKSP_MASK        0x00000010
#define HAL_WS_SMP4MKSP_TYPE        halWS_SMPMarkSpace_t
#define HAL_WS_SMP4MKSP             HAL_WS_SMP4MKSP_MASK
#define HAL_WS_SMP4MKSP_READABLE    1
#define HAL_WS_SMP4MKSP_WRITABLE    1
#define HAL_WS_SMP4MKSP_SIGNED      0
#define HAL_WS_SMP4MKSP_ADR         0x10000FE0

#define HAL_WS_SMP4MODE_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4MODE_LSB         3
#define HAL_WS_SMP4MODE_MSB         3
#define HAL_WS_SMP4MODE_MASK        0x00000008
#define HAL_WS_SMP4MODE_TYPE        halWS_SMPFixVar_t
#define HAL_WS_SMP4MODE             HAL_WS_SMP4MODE_MASK
#define HAL_WS_SMP4MODE_READABLE    1
#define HAL_WS_SMP4MODE_WRITABLE    1
#define HAL_WS_SMP4MODE_SIGNED      0
#define HAL_WS_SMP4MODE_ADR         0x10000FE0

#define HAL_WS_SMP4EXT_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4EXT_LSB         2
#define HAL_WS_SMP4EXT_MSB         2
#define HAL_WS_SMP4EXT_MASK        0x00000004
#define HAL_WS_SMP4EXT_TYPE        halWS_SMPIntExt_t
#define HAL_WS_SMP4EXT             HAL_WS_SMP4EXT_MASK
#define HAL_WS_SMP4EXT_READABLE    1
#define HAL_WS_SMP4EXT_WRITABLE    1
#define HAL_WS_SMP4EXT_SIGNED      0
#define HAL_WS_SMP4EXT_ADR         0x10000FE0

#define HAL_WS_SMP4DLY0_REGISTER    (HAL_WSREG_MAP->SMP4CTRL)
#define HAL_WS_SMP4DLY0_LSB         1
#define HAL_WS_SMP4DLY0_MSB         1
#define HAL_WS_SMP4DLY0_MASK        0x00000002
#define HAL_WS_SMP4DLY0_TYPE        halWS_SMPOpuDelay_t
#define HAL_WS_SMP4DLY0             HAL_WS_SMP4DLY0_MASK
#define HAL_WS_SMP4DLY0_READABLE    1
#define HAL_WS_SMP4DLY0_WRITABLE    1
#define HAL_WS_SMP4DLY0_SIGNED      0
#define HAL_WS_SMP4DLY0_ADR         0x10000FE0

// Registers SMP4PATL/SMP4PATH
#define HAL_WS_SMP4PAT_REGISTER    (HAL_WSREG_MAP->SMP4PATL)
#define HAL_WS_SMP4PAT_LSB         0
#define HAL_WS_SMP4PAT_MSB         10
#define HAL_WS_SMP4PAT_MASK        0x000007FF
#define HAL_WS_SMP4PAT_TYPE        UInt16
#define HAL_WS_SMP4PAT             HAL_WS_SMP4PAT_MASK
#define HAL_WS_SMP4PAT_READABLE    1
#define HAL_WS_SMP4PAT_WRITABLE    1
#define HAL_WS_SMP4PAT_SIGNED      0
#define HAL_WS_SMP4PAT_ADR         0x10000FE2

#define HAL_WS_SMP4PAT15_REGISTER    (HAL_WSREG_MAP->SMP4PATL)
#define HAL_WS_SMP4PAT15_LSB         15
#define HAL_WS_SMP4PAT15_MSB         15
#define HAL_WS_SMP4PAT15_MASK        0x00008000
#define HAL_WS_SMP4PAT15_TYPE        Bool
#define HAL_WS_SMP4PAT15             HAL_WS_SMP4PAT15_MASK
#define HAL_WS_SMP4PAT15_READABLE    1
#define HAL_WS_SMP4PAT15_WRITABLE    1
#define HAL_WS_SMP4PAT15_SIGNED      0
#define HAL_WS_SMP4PAT15_ADR         0x10000FE2

// Register SMP4LDLY
#define HAL_WS_SMP4DLY_REGISTER    (HAL_WSREG_MAP->SMP4LDLY)
#define HAL_WS_SMP4DLY_LSB         0
#define HAL_WS_SMP4DLY_MSB         6
#define HAL_WS_SMP4DLY_MASK        0x0000007F
#define HAL_WS_SMP4DLY_TYPE        UByte
#define HAL_WS_SMP4DLY             HAL_WS_SMP4DLY_MASK
#define HAL_WS_SMP4DLY_READABLE    1
#define HAL_WS_SMP4DLY_WRITABLE    1
#define HAL_WS_SMP4DLY_SIGNED      0
#define HAL_WS_SMP4DLY_ADR         0x10000FE4

// Register SMP4WID
#define HAL_WS_SMP4WID_REGISTER    (HAL_WSREG_MAP->SMP4WID)
#define HAL_WS_SMP4WID_LSB         0
#define HAL_WS_SMP4WID_MSB         6
#define HAL_WS_SMP4WID_MASK        0x0000007F
#define HAL_WS_SMP4WID_TYPE        UByte
#define HAL_WS_SMP4WID             HAL_WS_SMP4WID_MASK
#define HAL_WS_SMP4WID_READABLE    1
#define HAL_WS_SMP4WID_WRITABLE    1
#define HAL_WS_SMP4WID_SIGNED      0
#define HAL_WS_SMP4WID_ADR         0x10000FE5

// Register SMP5CTRL
#define HAL_WS_SMP5INVWG_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5INVWG_LSB         7
#define HAL_WS_SMP5INVWG_MSB         7
#define HAL_WS_SMP5INVWG_MASK        0x00000080
#define HAL_WS_SMP5INVWG_TYPE        Bool
#define HAL_WS_SMP5INVWG             HAL_WS_SMP5INVWG_MASK
#define HAL_WS_SMP5INVWG_READABLE    1
#define HAL_WS_SMP5INVWG_WRITABLE    1
#define HAL_WS_SMP5INVWG_SIGNED      0
#define HAL_WS_SMP5INVWG_ADR         0x10000FE6

#define HAL_WS_SMP5FIX_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5FIX_LSB         6
#define HAL_WS_SMP5FIX_MSB         6
#define HAL_WS_SMP5FIX_MASK        0x00000040
#define HAL_WS_SMP5FIX_TYPE        Bool
#define HAL_WS_SMP5FIX             HAL_WS_SMP5FIX_MASK
#define HAL_WS_SMP5FIX_READABLE    1
#define HAL_WS_SMP5FIX_WRITABLE    1
#define HAL_WS_SMP5FIX_SIGNED      0
#define HAL_WS_SMP5FIX_ADR         0x10000FE6

#define HAL_WS_SMP5INVWN_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5INVWN_LSB         5
#define HAL_WS_SMP5INVWN_MSB         5
#define HAL_WS_SMP5INVWN_MASK        0x00000020
#define HAL_WS_SMP5INVWN_TYPE        Bool
#define HAL_WS_SMP5INVWN             HAL_WS_SMP5INVWN_MASK
#define HAL_WS_SMP5INVWN_READABLE    1
#define HAL_WS_SMP5INVWN_WRITABLE    1
#define HAL_WS_SMP5INVWN_SIGNED      0
#define HAL_WS_SMP5INVWN_ADR         0x10000FE6

#define HAL_WS_SMP5MKSP_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5MKSP_LSB         4
#define HAL_WS_SMP5MKSP_MSB         4
#define HAL_WS_SMP5MKSP_MASK        0x00000010
#define HAL_WS_SMP5MKSP_TYPE        halWS_SMPMarkSpace_t
#define HAL_WS_SMP5MKSP             HAL_WS_SMP5MKSP_MASK
#define HAL_WS_SMP5MKSP_READABLE    1
#define HAL_WS_SMP5MKSP_WRITABLE    1
#define HAL_WS_SMP5MKSP_SIGNED      0
#define HAL_WS_SMP5MKSP_ADR         0x10000FE6

#define HAL_WS_SMP5MODE_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5MODE_LSB         3
#define HAL_WS_SMP5MODE_MSB         3
#define HAL_WS_SMP5MODE_MASK        0x00000008
#define HAL_WS_SMP5MODE_TYPE        halWS_SMPFixVar_t
#define HAL_WS_SMP5MODE             HAL_WS_SMP5MODE_MASK
#define HAL_WS_SMP5MODE_READABLE    1
#define HAL_WS_SMP5MODE_WRITABLE    1
#define HAL_WS_SMP5MODE_SIGNED      0
#define HAL_WS_SMP5MODE_ADR         0x10000FE6

#define HAL_WS_SMP5EXT_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5EXT_LSB         2
#define HAL_WS_SMP5EXT_MSB         2
#define HAL_WS_SMP5EXT_MASK        0x00000004
#define HAL_WS_SMP5EXT_TYPE        halWS_SMPIntExt_t
#define HAL_WS_SMP5EXT             HAL_WS_SMP5EXT_MASK
#define HAL_WS_SMP5EXT_READABLE    1
#define HAL_WS_SMP5EXT_WRITABLE    1
#define HAL_WS_SMP5EXT_SIGNED      0
#define HAL_WS_SMP5EXT_ADR         0x10000FE6

#define HAL_WS_SMP5DLY0_REGISTER    (HAL_WSREG_MAP->SMP5CTRL)
#define HAL_WS_SMP5DLY0_LSB         1
#define HAL_WS_SMP5DLY0_MSB         1
#define HAL_WS_SMP5DLY0_MASK        0x00000002
#define HAL_WS_SMP5DLY0_TYPE        halWS_SMPOpuDelay_t
#define HAL_WS_SMP5DLY0             HAL_WS_SMP5DLY0_MASK
#define HAL_WS_SMP5DLY0_READABLE    1
#define HAL_WS_SMP5DLY0_WRITABLE    1
#define HAL_WS_SMP5DLY0_SIGNED      0
#define HAL_WS_SMP5DLY0_ADR         0x10000FE6

// Registers SMP5PATL/SMP5PATH
#define HAL_WS_SMP5PAT_REGISTER    (HAL_WSREG_MAP->SMP5PATL)
#define HAL_WS_SMP5PAT_LSB         0
#define HAL_WS_SMP5PAT_MSB         10
#define HAL_WS_SMP5PAT_MASK        0x000007FF
#define HAL_WS_SMP5PAT_TYPE        UInt16
#define HAL_WS_SMP5PAT             HAL_WS_SMP5PAT_MASK
#define HAL_WS_SMP5PAT_READABLE    1
#define HAL_WS_SMP5PAT_WRITABLE    1
#define HAL_WS_SMP5PAT_SIGNED      0
#define HAL_WS_SMP5PAT_ADR         0x10000FE8

#define HAL_WS_SMP5PAT15_REGISTER    (HAL_WSREG_MAP->SMP5PATL)
#define HAL_WS_SMP5PAT15_LSB         15
#define HAL_WS_SMP5PAT15_MSB         15
#define HAL_WS_SMP5PAT15_MASK        0x00008000
#define HAL_WS_SMP5PAT15_TYPE        Bool
#define HAL_WS_SMP5PAT15             HAL_WS_SMP5PAT15_MASK
#define HAL_WS_SMP5PAT15_READABLE    1
#define HAL_WS_SMP5PAT15_WRITABLE    1
#define HAL_WS_SMP5PAT15_SIGNED      0
#define HAL_WS_SMP5PAT15_ADR         0x10000FE8

// Register SMP5LDLY
#define HAL_WS_SMP5DLY_REGISTER    (HAL_WSREG_MAP->SMP5LDLY)
#define HAL_WS_SMP5DLY_LSB         0
#define HAL_WS_SMP5DLY_MSB         6
#define HAL_WS_SMP5DLY_MASK        0x0000007F
#define HAL_WS_SMP5DLY_TYPE        UByte
#define HAL_WS_SMP5DLY             HAL_WS_SMP5DLY_MASK
#define HAL_WS_SMP5DLY_READABLE    1
#define HAL_WS_SMP5DLY_WRITABLE    1
#define HAL_WS_SMP5DLY_SIGNED      0
#define HAL_WS_SMP5DLY_ADR         0x10000FEA

// Register SMP5WID
#define HAL_WS_SMP5WID_REGISTER    (HAL_WSREG_MAP->SMP5WID)
#define HAL_WS_SMP5WID_LSB         0
#define HAL_WS_SMP5WID_MSB         6
#define HAL_WS_SMP5WID_MASK        0x0000007F
#define HAL_WS_SMP5WID_TYPE        UByte
#define HAL_WS_SMP5WID             HAL_WS_SMP5WID_MASK
#define HAL_WS_SMP5WID_READABLE    1
#define HAL_WS_SMP5WID_WRITABLE    1
#define HAL_WS_SMP5WID_SIGNED      0
#define HAL_WS_SMP5WID_ADR         0x10000FEB

// Register SMP15SDLY
#define HAL_WS_SMP15SDLY_REGISTER    (HAL_WSREG_MAP->SMP15SDLY)
#define HAL_WS_SMP15SDLY_LSB         0
#define HAL_WS_SMP15SDLY_MSB         7
#define HAL_WS_SMP15SDLY_MASK        0x000000FF
#define HAL_WS_SMP15SDLY_TYPE        UByte
#define HAL_WS_SMP15SDLY             HAL_WS_SMP15SDLY_MASK
#define HAL_WS_SMP15SDLY_READABLE    1
#define HAL_WS_SMP15SDLY_WRITABLE    1
#define HAL_WS_SMP15SDLY_SIGNED      0
#define HAL_WS_SMP15SDLY_ADR         0x10000FEC

// Register SMP15SWID
#define HAL_WS_SMP15SWID_REGISTER    (HAL_WSREG_MAP->SMP15SWID)
#define HAL_WS_SMP15SWID_LSB         0
#define HAL_WS_SMP15SWID_MSB         7
#define HAL_WS_SMP15SWID_MASK        0x000000FF
#define HAL_WS_SMP15SWID_TYPE        UByte
#define HAL_WS_SMP15SWID             HAL_WS_SMP15SWID_MASK
#define HAL_WS_SMP15SWID_READABLE    1
#define HAL_WS_SMP15SWID_WRITABLE    1
#define HAL_WS_SMP15SWID_SIGNED      0
#define HAL_WS_SMP15SWID_ADR         0x10000FED

// Register TESTMSC0
#define HAL_WS_SLFTST_REGISTER    (HAL_WSREG_MAP->TESTMSC0)
#define HAL_WS_SLFTST_LSB         6
#define HAL_WS_SLFTST_MSB         7
#define HAL_WS_SLFTST_MASK        0x000000C0
#define HAL_WS_SLFTST_TYPE        UByte
#define HAL_WS_SLFTST             HAL_WS_SLFTST_MASK
#define HAL_WS_SLFTST_READABLE    1
#define HAL_WS_SLFTST_WRITABLE    0
#define HAL_WS_SLFTST_SIGNED      0
#define HAL_WS_SLFTST_ADR         0x10000FF2

#define HAL_WS_SMPVAL_REGISTER    (HAL_WSREG_MAP->TESTMSC0)
#define HAL_WS_SMPVAL_LSB         0
#define HAL_WS_SMPVAL_MSB         4
#define HAL_WS_SMPVAL_MASK        0x0000001F
#define HAL_WS_SMPVAL_TYPE        UByte
#define HAL_WS_SMPVAL             HAL_WS_SMPVAL_MASK
#define HAL_WS_SMPVAL_READABLE    1
#define HAL_WS_SMPVAL_WRITABLE    0
#define HAL_WS_SMPVAL_SIGNED      0
#define HAL_WS_SMPVAL_ADR         0x10000FF2

// Register TESTSLFT
#define HAL_WS_SLPMUXCTL_REGISTER    (HAL_WSREG_MAP->TESTSLFT)
#define HAL_WS_SLPMUXCTL_LSB         5
#define HAL_WS_SLPMUXCTL_MSB         7
#define HAL_WS_SLPMUXCTL_MASK        0x000000E0
#define HAL_WS_SLPMUXCTL_TYPE        halWS_Slip_t
#define HAL_WS_SLPMUXCTL             HAL_WS_SLPMUXCTL_MASK
#define HAL_WS_SLPMUXCTL_READABLE    1
#define HAL_WS_SLPMUXCTL_WRITABLE    1
#define HAL_WS_SLPMUXCTL_SIGNED      0
#define HAL_WS_SLPMUXCTL_ADR         0x10000FF3

#define HAL_WS_SLFTSTEN_REGISTER    (HAL_WSREG_MAP->TESTSLFT)
#define HAL_WS_SLFTSTEN_LSB         4
#define HAL_WS_SLFTSTEN_MSB         4
#define HAL_WS_SLFTSTEN_MASK        0x00000010
#define HAL_WS_SLFTSTEN_TYPE        Bool
#define HAL_WS_SLFTSTEN             HAL_WS_SLFTSTEN_MASK
#define HAL_WS_SLFTSTEN_READABLE    1
#define HAL_WS_SLFTSTEN_WRITABLE    1
#define HAL_WS_SLFTSTEN_SIGNED      0
#define HAL_WS_SLFTSTEN_ADR         0x10000FF3

#define HAL_WS_SLFTSTSET_REGISTER    (HAL_WSREG_MAP->TESTSLFT)
#define HAL_WS_SLFTSTSET_LSB         0
#define HAL_WS_SLFTSTSET_MSB         3
#define HAL_WS_SLFTSTSET_MASK        0x0000000F
#define HAL_WS_SLFTSTSET_TYPE        halWS_SlfTst_t
#define HAL_WS_SLFTSTSET             HAL_WS_SLFTSTSET_MASK
#define HAL_WS_SLFTSTSET_READABLE    1
#define HAL_WS_SLFTSTSET_WRITABLE    1
#define HAL_WS_SLFTSTSET_SIGNED      0
#define HAL_WS_SLFTSTSET_ADR         0x10000FF3

// Register SMPUPDTR
#define HAL_WS_SMPUPDAT_REGISTER    (HAL_WSREG_MAP->SMPUPDTR)
#define HAL_WS_SMPUPDAT_LSB         0
#define HAL_WS_SMPUPDAT_MSB         0
#define HAL_WS_SMPUPDAT_MASK        0x00000001
#define HAL_WS_SMPUPDAT_TYPE        Bool
#define HAL_WS_SMPUPDAT             HAL_WS_SMPUPDAT_MASK
#define HAL_WS_SMPUPDAT_READABLE    1
#define HAL_WS_SMPUPDAT_WRITABLE    1
#define HAL_WS_SMPUPDAT_SIGNED      0
#define HAL_WS_SMPUPDAT_ADR         0x10000FF4

// Register TBGDIVCTL
#define HAL_WS_TBGDIVENA_REGISTER    (HAL_WSREG_MAP->TBGDIVCTL)
#define HAL_WS_TBGDIVENA_LSB         2
#define HAL_WS_TBGDIVENA_MSB         2
#define HAL_WS_TBGDIVENA_MASK        0x00000004
#define HAL_WS_TBGDIVENA_TYPE        Bool
#define HAL_WS_TBGDIVENA             HAL_WS_TBGDIVENA_MASK
#define HAL_WS_TBGDIVENA_READABLE    1
#define HAL_WS_TBGDIVENA_WRITABLE    1
#define HAL_WS_TBGDIVENA_SIGNED      0
#define HAL_WS_TBGDIVENA_ADR         0x10000FF5

#define HAL_WS_TBGDIVVAL_REGISTER    (HAL_WSREG_MAP->TBGDIVCTL)
#define HAL_WS_TBGDIVVAL_LSB         0
#define HAL_WS_TBGDIVVAL_MSB         1
#define HAL_WS_TBGDIVVAL_MASK        0x00000003
#define HAL_WS_TBGDIVVAL_TYPE        halWS_TBGDiv_t
#define HAL_WS_TBGDIVVAL             HAL_WS_TBGDIVVAL_MASK
#define HAL_WS_TBGDIVVAL_READABLE    1
#define HAL_WS_TBGDIVVAL_WRITABLE    1
#define HAL_WS_TBGDIVVAL_SIGNED      0
#define HAL_WS_TBGDIVVAL_ADR         0x10000FF5

// Register TESTCCTL
#define HAL_WS_CUSLDEN_REGISTER    (HAL_WSREG_MAP->TESTCCTL)
#define HAL_WS_CUSLDEN_LSB         7
#define HAL_WS_CUSLDEN_MSB         7
#define HAL_WS_CUSLDEN_MASK        0x00000080
#define HAL_WS_CUSLDEN_TYPE        Bool
#define HAL_WS_CUSLDEN             HAL_WS_CUSLDEN_MASK
#define HAL_WS_CUSLDEN_READABLE    1
#define HAL_WS_CUSLDEN_WRITABLE    1
#define HAL_WS_CUSLDEN_SIGNED      0
#define HAL_WS_CUSLDEN_ADR         0x10000FF7

#define HAL_WS_SLIPINST_REGISTER    (HAL_WSREG_MAP->TESTCCTL)
#define HAL_WS_SLIPINST_LSB         3
#define HAL_WS_SLIPINST_MSB         6
#define HAL_WS_SLIPINST_MASK        0x00000078
#define HAL_WS_SLIPINST_TYPE        UByte
#define HAL_WS_SLIPINST             HAL_WS_SLIPINST_MASK
#define HAL_WS_SLIPINST_READABLE    1
#define HAL_WS_SLIPINST_WRITABLE    1
#define HAL_WS_SLIPINST_SIGNED      0
#define HAL_WS_SLIPINST_ADR         0x10000FF7

#define HAL_WS_CPVAL_REGISTER    (HAL_WSREG_MAP->TESTCCTL)
#define HAL_WS_CPVAL_LSB         0
#define HAL_WS_CPVAL_MSB         0
#define HAL_WS_CPVAL_MASK        0x00000001
#define HAL_WS_CPVAL_TYPE        Bool
#define HAL_WS_CPVAL             HAL_WS_CPVAL_MASK
#define HAL_WS_CPVAL_READABLE    1
#define HAL_WS_CPVAL_WRITABLE    1
#define HAL_WS_CPVAL_SIGNED      0
#define HAL_WS_CPVAL_ADR         0x10000FF7

// Register TESTCDAT
#define HAL_WS_WGVAL_REGISTER    (HAL_WSREG_MAP->TESTCDAT)
#define HAL_WS_WGVAL_LSB         4
#define HAL_WS_WGVAL_MSB         7
#define HAL_WS_WGVAL_MASK        0x000000F0
#define HAL_WS_WGVAL_TYPE        UByte
#define HAL_WS_WGVAL             HAL_WS_WGVAL_MASK
#define HAL_WS_WGVAL_READABLE    1
#define HAL_WS_WGVAL_WRITABLE    1
#define HAL_WS_WGVAL_SIGNED      0
#define HAL_WS_WGVAL_ADR         0x10000FF8

#define HAL_WS_EXTWSVAL_REGISTER    (HAL_WSREG_MAP->TESTCDAT)
#define HAL_WS_EXTWSVAL_LSB         0
#define HAL_WS_EXTWSVAL_MSB         3
#define HAL_WS_EXTWSVAL_MASK        0x0000000F
#define HAL_WS_EXTWSVAL_TYPE        UByte
#define HAL_WS_EXTWSVAL             HAL_WS_EXTWSVAL_MASK
#define HAL_WS_EXTWSVAL_READABLE    1
#define HAL_WS_EXTWSVAL_WRITABLE    1
#define HAL_WS_EXTWSVAL_SIGNED      0
#define HAL_WS_EXTWSVAL_ADR         0x10000FF8

#if (CHIP_REV <= 0xB3)

// Register WSSPARE
#define HAL_WS_PGTWOTEN_REGISTER    (HAL_WSREG_MAP->WSSPARE)
#define HAL_WS_PGTWOTEN_LSB         0
#define HAL_WS_PGTWOTEN_MSB         0
#define HAL_WS_PGTWOTEN_MASK        0x00000001
#define HAL_WS_PGTWOTEN_TYPE        Bool
#define HAL_WS_PGTWOTEN             HAL_WS_PGTWOTEN_MASK
#define HAL_WS_PGTWOTEN_READABLE    1
#define HAL_WS_PGTWOTEN_WRITABLE    1
#define HAL_WS_PGTWOTEN_SIGNED      0
#define HAL_WS_PGTWOTEN_ADR         0x10000FFA

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

// Register WSDBSEL0
#define HAL_WS_WSDBSELB_REGISTER    (HAL_WSREG_MAP->WSDBSEL0)
#define HAL_WS_WSDBSELB_LSB         4
#define HAL_WS_WSDBSELB_MSB         7
#define HAL_WS_WSDBSELB_MASK        0x000000F0
#define HAL_WS_WSDBSELB_TYPE        UByte
#define HAL_WS_WSDBSELB             HAL_WS_WSDBSELB_MASK
#define HAL_WS_WSDBSELB_READABLE    1
#define HAL_WS_WSDBSELB_WRITABLE    1
#define HAL_WS_WSDBSELB_SIGNED      0
#define HAL_WS_WSDBSELB_ADR         0x10000FF9

#define HAL_WS_WSDBSELA_REGISTER    (HAL_WSREG_MAP->WSDBSEL0)
#define HAL_WS_WSDBSELA_LSB         0
#define HAL_WS_WSDBSELA_MSB         3
#define HAL_WS_WSDBSELA_MASK        0x0000000F
#define HAL_WS_WSDBSELA_TYPE        UByte
#define HAL_WS_WSDBSELA             HAL_WS_WSDBSELA_MASK
#define HAL_WS_WSDBSELA_READABLE    1
#define HAL_WS_WSDBSELA_WRITABLE    1
#define HAL_WS_WSDBSELA_SIGNED      0
#define HAL_WS_WSDBSELA_ADR         0x10000FF9

// Register WSDBSEL1
#define HAL_WS_WSDBSELD_REGISTER    (HAL_WSREG_MAP->WSDBSEL1)
#define HAL_WS_WSDBSELD_LSB         4
#define HAL_WS_WSDBSELD_MSB         7
#define HAL_WS_WSDBSELD_MASK        0x000000F0
#define HAL_WS_WSDBSELD_TYPE        UByte
#define HAL_WS_WSDBSELD             HAL_WS_WSDBSELD_MASK
#define HAL_WS_WSDBSELD_READABLE    1
#define HAL_WS_WSDBSELD_WRITABLE    1
#define HAL_WS_WSDBSELD_SIGNED      0
#define HAL_WS_WSDBSELD_ADR         0x10000FFA

#define HAL_WS_WSDBSELC_REGISTER    (HAL_WSREG_MAP->WSDBSEL1)
#define HAL_WS_WSDBSELC_LSB         0
#define HAL_WS_WSDBSELC_MSB         3
#define HAL_WS_WSDBSELC_MASK        0x0000000F
#define HAL_WS_WSDBSELC_TYPE        UByte
#define HAL_WS_WSDBSELC             HAL_WS_WSDBSELC_MASK
#define HAL_WS_WSDBSELC_READABLE    1
#define HAL_WS_WSDBSELC_WRITABLE    1
#define HAL_WS_WSDBSELC_SIGNED      0
#define HAL_WS_WSDBSELC_ADR         0x10000FFA

#endif // #if (CHIP_REV >= 0xC0)

#if (CHIP_REV >= 0xC1)

// Register WSSPARE
#define HAL_WS_EFMINSTSP_REGISTER    (HAL_WSREG_MAP->WSSPARE)
#define HAL_WS_EFMINSTSP_LSB         0
#define HAL_WS_EFMINSTSP_MSB         5
#define HAL_WS_EFMINSTSP_MASK        0x0000003F
#define HAL_WS_EFMINSTSP_TYPE        UByte
#define HAL_WS_EFMINSTSP             HAL_WS_EFMINSTSP_MASK
#define HAL_WS_EFMINSTSP_READABLE    1
#define HAL_WS_EFMINSTSP_WRITABLE    1
#define HAL_WS_EFMINSTSP_SIGNED      0
#define HAL_WS_EFMINSTSP_ADR         0x10000FFB

#endif // #if (CHIP_REV >= 0xC1)

// Register WSMIO0
#define HAL_WS_WSMIO0_REGISTER    (HAL_WSREG_MAP->WSMIO0)
#define HAL_WS_WSMIO0_LSB         0
#define HAL_WS_WSMIO0_MSB         4
#define HAL_WS_WSMIO0_MASK        0x0000001F
#define HAL_WS_WSMIO0_TYPE        halWS_WSMIO_t
#define HAL_WS_WSMIO0             HAL_WS_WSMIO0_MASK
#define HAL_WS_WSMIO0_READABLE    1
#define HAL_WS_WSMIO0_WRITABLE    1
#define HAL_WS_WSMIO0_SIGNED      0
#define HAL_WS_WSMIO0_ADR         0x10000FFC

// Register WSMIO1
#define HAL_WS_WSMIO1_REGISTER    (HAL_WSREG_MAP->WSMIO1)
#define HAL_WS_WSMIO1_LSB         0
#define HAL_WS_WSMIO1_MSB         4
#define HAL_WS_WSMIO1_MASK        0x0000001F
#define HAL_WS_WSMIO1_TYPE        halWS_WSMIO_t
#define HAL_WS_WSMIO1             HAL_WS_WSMIO1_MASK
#define HAL_WS_WSMIO1_READABLE    1
#define HAL_WS_WSMIO1_WRITABLE    1
#define HAL_WS_WSMIO1_SIGNED      0
#define HAL_WS_WSMIO1_ADR         0x10000FFD

// Register WSMIO2
#define HAL_WS_WSMIO2_REGISTER    (HAL_WSREG_MAP->WSMIO2)
#define HAL_WS_WSMIO2_LSB         0
#define HAL_WS_WSMIO2_MSB         4
#define HAL_WS_WSMIO2_MASK        0x0000001F
#define HAL_WS_WSMIO2_TYPE        halWS_WSMIO_t
#define HAL_WS_WSMIO2             HAL_WS_WSMIO2_MASK
#define HAL_WS_WSMIO2_READABLE    1
#define HAL_WS_WSMIO2_WRITABLE    1
#define HAL_WS_WSMIO2_SIGNED      0
#define HAL_WS_WSMIO2_ADR         0x10000FFE

// Register WSMIO3
#define HAL_WS_WSMIO3_REGISTER    (HAL_WSREG_MAP->WSMIO3)
#define HAL_WS_WSMIO3_LSB         0
#define HAL_WS_WSMIO3_MSB         4
#define HAL_WS_WSMIO3_MASK        0x0000001F
#define HAL_WS_WSMIO3_TYPE        halWS_WSMIO_t
#define HAL_WS_WSMIO3             HAL_WS_WSMIO3_MASK
#define HAL_WS_WSMIO3_READABLE    1
#define HAL_WS_WSMIO3_WRITABLE    1
#define HAL_WS_WSMIO3_SIGNED      0
#define HAL_WS_WSMIO3_ADR         0x10000FFF

// Enumerations

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halWS_GAINSW4W_Zero = 0,    // GAINSW4 Output Level During Write = Low
	halWS_GAINSW4W_HiZ1 = 1,    // GAINSW4 Output Level During Write = Hi-Z
	halWS_GAINSW4W_One  = 2,    // GAINSW4 Output Level During Write = High
	halWS_GAINSW4W_HiZ3 = 3     // GAINSW4 Output Level During Write = Hi-Z
} halWS_GAINSW4W_t;

typedef enum
{
	halWS_GAINSW4R_Zero = 0,    // GAINSW4 Output Level During Read = Low
	halWS_GAINSW4R_HiZ1 = 1,    // GAINSW4 Output Level During Read = Hi-Z
	halWS_GAINSW4R_One  = 2,    // GAINSW4 Output Level During Read = High
	halWS_GAINSW4R_HiZ3 = 3     // GAINSW4 Output Level During Read = Hi-Z
} halWS_GAINSW4R_t;

typedef enum
{
	halWS_GAINSW3W_Zero = 0,    // GAINSW3 Output Level During Write = Low
	halWS_GAINSW3W_HiZ1 = 1,    // GAINSW3 Output Level During Write = Hi-Z
	halWS_GAINSW3W_One  = 2,    // GAINSW3 Output Level During Write = High
	halWS_GAINSW3W_HiZ3 = 3     // GAINSW3 Output Level During Write = Hi-Z
} halWS_GAINSW3W_t;

typedef enum
{
	halWS_GAINSW3R_Zero = 0,    // GAINSW3 Output Level During Read = Low
	halWS_GAINSW3R_HiZ1 = 1,    // GAINSW3 Output Level During Read = Hi-Z
	halWS_GAINSW3R_One  = 2,    // GAINSW3 Output Level During Read = High
	halWS_GAINSW3R_HiZ3 = 3     // GAINSW3 Output Level During Read = Hi-Z
} halWS_GAINSW3R_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halWS_GAINSW2W_Zero = 0,    // GAINSW2 Output Level During Write = Low
	halWS_GAINSW2W_HiZ1 = 1,    // GAINSW2 Output Level During Write = Hi-Z
	halWS_GAINSW2W_One  = 2,    // GAINSW2 Output Level During Write = High
	halWS_GAINSW2W_HiZ3 = 3     // GAINSW2 Output Level During Write = Hi-Z
} halWS_GAINSW2W_t;

typedef enum
{
	halWS_GAINSW2R_Zero = 0,    // GAINSW2 Output Level During Read = Low
	halWS_GAINSW2R_HiZ1 = 1,    // GAINSW2 Output Level During Read = Hi-Z
	halWS_GAINSW2R_One  = 2,    // GAINSW2 Output Level During Read = High
	halWS_GAINSW2R_HiZ3 = 3     // GAINSW2 Output Level During Read = Hi-Z
} halWS_GAINSW2R_t;

typedef enum
{
	halWS_GAINSWW_Zero = 0,    // GAINSW Output Level During Write = Low
	halWS_GAINSWW_HiZ1 = 1,    // GAINSW Output Level During Write = Hi-Z
	halWS_GAINSWW_One  = 2,    // GAINSW Output Level During Write = High
	halWS_GAINSWW_HiZ3 = 3     // GAINSW Output Level During Write = Hi-Z
} halWS_GAINSWW_t;

typedef enum
{
	halWS_GAINSWR_Zero = 0,    // GAINSW Output Level During Read = Low
	halWS_GAINSWR_HiZ1 = 1,    // GAINSW Output Level During Read = Hi-Z
	halWS_GAINSWR_One  = 2,    // GAINSW Output Level During Read = High
	halWS_GAINSWR_HiZ3 = 3     // GAINSW Output Level During Read = Hi-Z
} halWS_GAINSWR_t;

typedef enum
{
	halWS_OSCENMOD_R  = 0,    // R  mode for OENC
	halWS_OSCENMOD_RW = 1     // RW mode for OENC
} halWS_OSCENMOD_t;

typedef enum
{
	halWS_OSCENSamp_SMP1 = 1,    // Select SMP1 for OSCEN
	halWS_OSCENSamp_SMP2 = 2,    // Select SMP2 for OSCEN
	halWS_OSCENSamp_SMP3 = 3,    // Select SMP3 for OSCEN
	halWS_OSCENSamp_SMP4 = 4,    // Select SMP4 for OSCEN
	halWS_OSCENSamp_SMP5 = 5     // Select SMP5 for OSCEN
} halWS_OSCENSamp_t;

typedef enum
{
	halWS_PostambPos_TDiv32 = 0,    // Allow each bit of the postamble edge position to correspond to T/32
	halWS_PostambPos_TDiv16 = 1     // Allow each bit of the postamble edge position to correspond to T/16
} halWS_PostambPos_t;

typedef enum
{
	halWS_WGCtrl_Auto  = 0,    // Automatic write gate control
	halWS_WGCtrl_WGLVL = 1     // Control state of write gate from WGLVL
} halWS_WGCtrl_t;

typedef enum
{
	halWS_DCErase_EFM1Bias_FEM2Wr = 0,    // DC Erase Settings: EFM1 bias power, EFM2 write power
	halWS_DCErase_EFM2Bias_FEM1Wr = 1     // DC Erase Settings: EFM2 bias power, EFM1 write power
} halWS_DCEraseEFM12_t;

typedef enum
{
	halWS_ExtWSSampDly_0T  = 0,    // Delay from EFM and WGATE to SMP: 0T
	halWS_ExtWSSampDly_8T  = 1,    // Delay from EFM and WGATE to SMP: 8T
	halWS_ExtWSSampDly_16T = 2,    // Delay from EFM and WGATE to SMP: 16T
	halWS_ExtWSSampDly_24T = 3     // Delay from EFM and WGATE to SMP: 24T
} halWS_ExtWSSampDly_t;

typedef enum
{
	halWS_FastT32Dly_60ns = 0,    // Use normal 60 ns delay for T/32 delays
	halWS_FastT32Dly_30ns = 1     // Use normal 30 ns delay for T/32 delays
} halWS_FastT32Dly_t;

typedef enum
{
	halWS_TBGMN_0  = 0,    // Write Strategy TBG M or N = 0
	halWS_TBGMN_1  = 1,    // Write Strategy TBG M or N = 1
	halWS_TBGMN_2  = 2,    // Write Strategy TBG M or N = 2
	halWS_TBGMN_3  = 3,    // Write Strategy TBG M or N = 3
	halWS_TBGMN_4  = 4,    // Write Strategy TBG M or N = 4
	halWS_TBGMN_5  = 5,    // Write Strategy TBG M or N = 5
	halWS_TBGMN_6  = 6,    // Write Strategy TBG M or N = 6
	halWS_TBGMN_7  = 7,    // Write Strategy TBG M or N = 7
	halWS_TBGMN_8  = 8,    // Write Strategy TBG M or N = 8
	halWS_TBGMN_9  = 9,    // Write Strategy TBG M or N = 9
	halWS_TBGMN_10 = 10,    // Write Strategy TBG M or N = 10
	halWS_TBGMN_11 = 11,    // Write Strategy TBG M or N = 11
	halWS_TBGMN_12 = 12,    // Write Strategy TBG M or N = 12
	halWS_TBGMN_13 = 13,    // Write Strategy TBG M or N = 13
	halWS_TBGMN_14 = 14,    // Write Strategy TBG M or N = 14
	halWS_TBGMN_15 = 15     // Write Strategy TBG M or N = 15
} halWS_TBGMN_t;

typedef enum
{
	halWS_WSTBGK_LessThan150MHz = 0,    // TBG's Frequency Range < 150 MHz
	halWS_WSTBGK_150MHzTo300MHz = 1,    // 150 MHz < TBG's Frequency Range < 300 MHz
	halWS_WSTBGK_300MHzTo450MHz = 2,    // 300 MHz < TBG's Frequency Range < 450 MHz
	halWS_WSTBGK_MoreThan450MHz = 3     // TBG's Frequency Range > 450 MHz
} halWS_WSTBGK_t;

typedef enum
{
	halWS_WSTBGL_1     = 0,    // Write Strategy TBG L = 1
	halWS_WSTBGL_2     = 1,    // Write Strategy TBG L = 2
	halWS_WSTBGL_4     = 2,    // Write Strategy TBG L = 4
	halWS_WSTBGL_PortA = 0,    // In test_scan mode, select instruction input port A
	halWS_WSTBGL_PortB = 1,    // In test_scan mode, select instruction input port B
	halWS_WSTBGL_PortC = 2,    // In test_scan mode, select instruction input port C
	halWS_WSTBGL_PortD = 3     // In test_scan mode, select instruction input port D
} halWS_WSTBGL_t;

typedef enum
{
	halWS_WSCEN_Reset  = 0,    // WS CUSTOM block: stay in reset mode
	halWS_WSCEN_Normal = 1     // WS CUSTOM block: Normal operation
} halWS_WSCEN_t;

typedef enum
{
	halWS_EFMSRC_Normal     = 0,    // Normal operation
	halWS_EFMSRC_TESTSPMK   = 1,    // Fix pattern defined by TESTSPMK
	halWS_EFMSRC_IncPattern = 2     // Incrementing Pattern: 3M,3S,3M,4S,3M...4M,3S,4M,4S...11M,11S,3M,3S...
} halWS_EFMSRC_t;

typedef enum
{
	halWS_TBGCPOutRng_12uA  = 0,    // TBG Charge Pump Output Range  12 uA
	halWS_TBGCPOutRng_36uA  = 1,    // TBG Charge Pump Output Range  36 uA
	halWS_TBGCPOutRng_60uA  = 2,    // TBG Charge Pump Output Range  60 uA
	halWS_TBGCPOutRng_84uA  = 3,    // TBG Charge Pump Output Range  84 uA
	halWS_TBGCPOutRng_124uA = 4,    // TBG Charge Pump Output Range 124 uA
	halWS_TBGCPOutRng_148uA = 5,    // TBG Charge Pump Output Range 148 uA
	halWS_TBGCPOutRng_172uA = 6,    // TBG Charge Pump Output Range 172 uA
	halWS_TBGCPOutRng_196uA = 7     // TBG Charge Pump Output Range 196 uA
} halWS_TBGCPOutRng_t;

typedef enum
{
	halWS_TBGLFResist_4p8KOhm = 0,    // TBG Filter Loop Resistance = 4.8 KOhm
	halWS_TBGLFResist_3p6KOhm = 1,    // TBG Filter Loop Resistance = 3.6 KOhm
	halWS_TBGLFResist_2p4KOhm = 2,    // TBG Filter Loop Resistance = 2.4 KOhm
	halWS_TBGLFResist_1p2KOhm = 3     // TBG Filter Loop Resistance = 1.2 KOhm
} halWS_TBGLFResistor_t;

typedef enum
{
	halWS_PhDetTst_Normal   = 0,    // Normal operation
	halWS_PhDetTst_IncVCOF  = 1,    // Phase detector increases VCO frequency
	halWS_PhDetTst_DecVCOF  = 2,    // Phase detector decreases VCO frequency
	halWS_PhDetTst_NoAffect = 3     // Phase detector doesn't affect VCO frequency
} halWS_PhDetTst_t;

typedef enum
{
	halWS_TBGSlip_1T = 0,    // Require 1 TBG clock with no instructions to allow TBG slip
	halWS_TBGSlip_2T = 1     // Require 2 TBG clock with no instructions to allow TBG slip
} halWS_TBGSlip_t;

typedef enum
{
	halWS_SampleOnSpace = 0,    // Sample on spaces
	halWS_SampleOnMark  = 1     // Sample on marks
} halWS_SMPMarkSpace_t;

typedef enum
{
	halWS_SampleFixedWidth    = 0,    // Sample in fixed width mode
	halWS_SampleVariableWidth = 1     // Sample in variable width mode
} halWS_SMPFixVar_t;

typedef enum
{
	halWS_SampleIntTiming = 0,    // Sample using internal timing (EFM internal)
	halWS_SampleExtTiming = 1     // Sample using external timing (EFM1 rise-fall)
} halWS_SMPIntExt_t;

typedef enum
{
	halWS_SampleOpuDelay   = 0,    // Sampling pulses delayed by OPUDELAY
	halWS_SampleNoOpuDelay = 1     // Sampling pulses not affected by OPUDELAY
} halWS_SMPOpuDelay_t;

typedef enum
{
	halWS_SlipA_Cd  = 0,    // Select instruction Cd  for Slip A
	halWS_SlipA_Dd  = 1,    // Select instruction Dd  for Slip A
	halWS_SlipA_Add = 2,    // Select instruction Add for Slip A
	halWS_SlipA_Ad  = 6,    // Select instruction Ad  for Slip A
	halWS_SlipA_Bd  = 7,    // Select instruction Bd  for Slip A
	halWS_SlipB_Dd  = 0,    // Select instruction Dd  for Slip B
	halWS_SlipB_Add = 1,    // Select instruction Add for Slip B
	halWS_SlipB_Bdd = 2,    // Select instruction Bdd for Slip B
	halWS_SlipB_Bd  = 6,    // Select instruction Bd  for Slip B
	halWS_SlipB_Cd  = 7,    // Select instruction Cd  for Slip B
	halWS_SlipC_Add = 0,    // Select instruction Add for Slip C
	halWS_SlipC_Bdd = 1,    // Select instruction Bdd for Slip C
	halWS_SlipC_Cdd = 2,    // Select instruction Cdd for Slip C
	halWS_SlipC_Cd  = 6,    // Select instruction Cd  for Slip C
	halWS_SlipC_Dd  = 7,    // Select instruction Dd  for Slip C
	halWS_SlipD_Bdd = 0,    // Select instruction Bdd for Slip D
	halWS_SlipD_Cdd = 1,    // Select instruction Cdd for Slip D
	halWS_SlipD_Ddd = 2,    // Select instruction Ddd for Slip D
	halWS_SlipD_Dd  = 6,    // Select instruction Dd  for Slip D
	halWS_SlipD_Add = 7     // Select instruction Add for Slip D
} halWS_Slip_t;

typedef enum
{
	halWS_SlfTst_EFM1 = 0,    // Select EFM1 for input to self-test edge counter
	halWS_SlfTst_EFM2 = 1,    // Select EFM2 for input to self-test edge counter
	halWS_SlfTst_EFM3 = 2,    // Select EFM3 for input to self-test edge counter
	halWS_SlfTst_SMP1 = 8,    // Select SMP1 for input to self-test edge counter
	halWS_SlfTst_SMP2 = 9,    // Select SMP2 for input to self-test edge counter
	halWS_SlfTst_SMP3 = 10,    // Select SMP3 for input to self-test edge counter
	halWS_SlfTst_SMP4 = 11,    // Select SMP4 for input to self-test edge counter
	halWS_SlfTst_SMP5 = 12     // Select SMP5 for input to self-test edge counter
} halWS_SlfTst_t;

typedef enum
{
	halWS_TBGDiv1 = 0,    // TBG Divider Value = 1
	halWS_TBGDiv2 = 1,    // TBG Divider Value = 2
	halWS_TBGDiv4 = 2,    // TBG Divider Value = 4
	halWS_TBGDiv8 = 3     // TBG Divider Value = 8
} halWS_TBGDiv_t;

typedef enum
{
	halWS_WSMIO_smp1o            = 0,
	halWS_WSMIO_smp2o            = 1,
	halWS_WSMIO_smp3o            = 2,
	halWS_WSMIO_smp4o            = 3,
	halWS_WSMIO_smp5o            = 4,
	halWS_WSMIO_wgate_cd         = 5,
	halWS_WSMIO_wgate_cutoff_cd  = 6,
	halWS_WSMIO_wgate_dvd        = 7,
	halWS_WSMIO_wgate_cutoff_dvd = 8,
	halWS_WSMIO_wgate_bd         = 9,
	halWS_WSMIO_wgate_cutoff_bd  = 10,
	halWS_WSMIO_wgate_cus        = 11,
	halWS_WSMIO_wgate_out        = 12,
	halWS_WSMIO_wgate_delayed    = 13,
	halWS_WSMIO_lswgate          = 14,
	halWS_WSMIO_slpdni_cus       = 16,
	halWS_WSMIO_slpupi_cus       = 17,
	halWS_WSMIO_slipa_out        = 18,
	halWS_WSMIO_slipb_out        = 19,
	halWS_WSMIO_slipc_out        = 20,
	halWS_WSMIO_slipd_out        = 21,
	halWS_WSMIO_trerr            = 22 
} halWS_WSMIO_t;

#endif /* __REG_WRITE_STRATEGY_H__ */

