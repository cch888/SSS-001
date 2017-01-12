/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_sdram.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the SOC_SDRAM core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_SDRAM_H__
#define __REG_SDRAM_H__

// Core register map
#define HAL_SDRAM_VERSION 7.142
#define HAL_SDRAM_BaseAddress 0x10000600
#define HAL_SDRAMREG_MAP ((struct halSDRAMReg_Map*) (void *) HAL_SDRAM_BaseAddress)
__packed struct halSDRAMReg_Map
{
	HAL_Reg8  ReservedBlk0[144];              //00000000
	HAL_Reg8  SDMODE;                         //00000090
	HAL_Reg8  ReservedBlk1[3];                //00000091
	HAL_Reg8  SDCONF0;                        //00000094
	HAL_Reg8  SDCONF1;                        //00000095
	HAL_Reg8  SDCONF2;                        //00000096
	HAL_Reg8  SDCONF3;                        //00000097
	HAL_Reg8  SDCONF4;                        //00000098
	HAL_Reg8  ReservedBlk2[7];                //00000099
	HAL_Reg8  SDDELAY0;                       //000000A0
	HAL_Reg8  SDDELAY1;                       //000000A1
	HAL_Reg8  ReservedBlk3[30];               //000000A2
	HAL_Reg16 SDBLSEEDL;                      //000000C0
	HAL_Reg16 SDBUSEEDL;                      //000000C2
	HAL_Reg32 SDBSTRTAL;                      //000000C4
	HAL_Reg32 SDBSTOPAL;                      //000000C8
	HAL_Reg32 SDBFAILAL;                      //000000CC
	HAL_Reg32 SDBCURRAL;                      //000000D0
};
// Register SDMODE
#define HAL_SDRAM_SDCMD_REGISTER    (HAL_SDRAMREG_MAP->SDMODE)
#define HAL_SDRAM_SDCMD_LSB         0
#define HAL_SDRAM_SDCMD_MSB         7
#define HAL_SDRAM_SDCMD_MASK        0x000000FF
#define HAL_SDRAM_SDCMD_TYPE        UByte
#define HAL_SDRAM_SDCMD             HAL_SDRAM_SDCMD_MASK
#define HAL_SDRAM_SDCMD_READABLE    0
#define HAL_SDRAM_SDCMD_WRITABLE    1
#define HAL_SDRAM_SDCMD_SIGNED      0
#define HAL_SDRAM_SDCMD_ADR         0x10000690

// Register SDCONF0
#define HAL_SDRAM_CASLAT_REGISTER    (HAL_SDRAMREG_MAP->SDCONF0)
#define HAL_SDRAM_CASLAT_LSB         6
#define HAL_SDRAM_CASLAT_MSB         6
#define HAL_SDRAM_CASLAT_MASK        0x00000040
#define HAL_SDRAM_CASLAT_TYPE        Bool
#define HAL_SDRAM_CASLAT             HAL_SDRAM_CASLAT_MASK
#define HAL_SDRAM_CASLAT_READABLE    1
#define HAL_SDRAM_CASLAT_WRITABLE    1
#define HAL_SDRAM_CASLAT_SIGNED      0
#define HAL_SDRAM_CASLAT_ADR         0x10000694

#define HAL_SDRAM_SDWDTH32_REGISTER    (HAL_SDRAMREG_MAP->SDCONF0)
#define HAL_SDRAM_SDWDTH32_LSB         5
#define HAL_SDRAM_SDWDTH32_MSB         5
#define HAL_SDRAM_SDWDTH32_MASK        0x00000020
#define HAL_SDRAM_SDWDTH32_TYPE        Bool
#define HAL_SDRAM_SDWDTH32             HAL_SDRAM_SDWDTH32_MASK
#define HAL_SDRAM_SDWDTH32_READABLE    1
#define HAL_SDRAM_SDWDTH32_WRITABLE    1
#define HAL_SDRAM_SDWDTH32_SIGNED      0
#define HAL_SDRAM_SDWDTH32_ADR         0x10000694

#define HAL_SDRAM_SDSIZE_REGISTER    (HAL_SDRAMREG_MAP->SDCONF0)
#define HAL_SDRAM_SDSIZE_LSB         0
#define HAL_SDRAM_SDSIZE_MSB         2
#define HAL_SDRAM_SDSIZE_MASK        0x00000007
#define HAL_SDRAM_SDSIZE_TYPE        halSDRAM_SdramSize_t
#define HAL_SDRAM_SDSIZE             HAL_SDRAM_SDSIZE_MASK
#define HAL_SDRAM_SDSIZE_READABLE    1
#define HAL_SDRAM_SDSIZE_WRITABLE    1
#define HAL_SDRAM_SDSIZE_SIGNED      0
#define HAL_SDRAM_SDSIZE_ADR         0x10000694

// Register SDCONF1
#define HAL_SDRAM_TRFC_REGISTER    (HAL_SDRAMREG_MAP->SDCONF1)
#define HAL_SDRAM_TRFC_LSB         4
#define HAL_SDRAM_TRFC_MSB         7
#define HAL_SDRAM_TRFC_MASK        0x000000F0
#define HAL_SDRAM_TRFC_TYPE        halSDRAM_ARefreshPeriod_t
#define HAL_SDRAM_TRFC             HAL_SDRAM_TRFC_MASK
#define HAL_SDRAM_TRFC_READABLE    1
#define HAL_SDRAM_TRFC_WRITABLE    1
#define HAL_SDRAM_TRFC_SIGNED      0
#define HAL_SDRAM_TRFC_ADR         0x10000695

#define HAL_SDRAM_TWR_REGISTER    (HAL_SDRAMREG_MAP->SDCONF1)
#define HAL_SDRAM_TWR_LSB         3
#define HAL_SDRAM_TWR_MSB         3
#define HAL_SDRAM_TWR_MASK        0x00000008
#define HAL_SDRAM_TWR_TYPE        halSDRAM_WrRecovery_t
#define HAL_SDRAM_TWR             HAL_SDRAM_TWR_MASK
#define HAL_SDRAM_TWR_READABLE    1
#define HAL_SDRAM_TWR_WRITABLE    1
#define HAL_SDRAM_TWR_SIGNED      0
#define HAL_SDRAM_TWR_ADR         0x10000695

#define HAL_SDRAM_TRAS_REGISTER    (HAL_SDRAMREG_MAP->SDCONF1)
#define HAL_SDRAM_TRAS_LSB         0
#define HAL_SDRAM_TRAS_MSB         2
#define HAL_SDRAM_TRAS_MASK        0x00000007
#define HAL_SDRAM_TRAS_TYPE        halSDRAM_Act2Precharge_t
#define HAL_SDRAM_TRAS             HAL_SDRAM_TRAS_MASK
#define HAL_SDRAM_TRAS_READABLE    1
#define HAL_SDRAM_TRAS_WRITABLE    1
#define HAL_SDRAM_TRAS_SIGNED      0
#define HAL_SDRAM_TRAS_ADR         0x10000695

// Register SDCONF2
#define HAL_SDRAM_REFINTV_REGISTER    (HAL_SDRAMREG_MAP->SDCONF2)
#define HAL_SDRAM_REFINTV_LSB         0
#define HAL_SDRAM_REFINTV_MSB         7
#define HAL_SDRAM_REFINTV_MASK        0x000000FF
#define HAL_SDRAM_REFINTV_TYPE        UByte
#define HAL_SDRAM_REFINTV             HAL_SDRAM_REFINTV_MASK
#define HAL_SDRAM_REFINTV_READABLE    1
#define HAL_SDRAM_REFINTV_WRITABLE    1
#define HAL_SDRAM_REFINTV_SIGNED      0
#define HAL_SDRAM_REFINTV_ADR         0x10000696

// Register SDCONF3
#define HAL_SDRAM_CKE_REGISTER    (HAL_SDRAMREG_MAP->SDCONF3)
#define HAL_SDRAM_CKE_LSB         7
#define HAL_SDRAM_CKE_MSB         7
#define HAL_SDRAM_CKE_MASK        0x00000080
#define HAL_SDRAM_CKE_TYPE        Bool
#define HAL_SDRAM_CKE             HAL_SDRAM_CKE_MASK
#define HAL_SDRAM_CKE_READABLE    1
#define HAL_SDRAM_CKE_WRITABLE    1
#define HAL_SDRAM_CKE_SIGNED      0
#define HAL_SDRAM_CKE_ADR         0x10000697

// Register SDCONF4
#define HAL_SDRAM_TRP_REGISTER    (HAL_SDRAMREG_MAP->SDCONF4)
#define HAL_SDRAM_TRP_LSB         6
#define HAL_SDRAM_TRP_MSB         7
#define HAL_SDRAM_TRP_MASK        0x000000C0
#define HAL_SDRAM_TRP_TYPE        halSDRAM_PreCPeriod_t
#define HAL_SDRAM_TRP             HAL_SDRAM_TRP_MASK
#define HAL_SDRAM_TRP_READABLE    1
#define HAL_SDRAM_TRP_WRITABLE    1
#define HAL_SDRAM_TRP_SIGNED      0
#define HAL_SDRAM_TRP_ADR         0x10000698

#define HAL_SDRAM_TRCD_REGISTER    (HAL_SDRAMREG_MAP->SDCONF4)
#define HAL_SDRAM_TRCD_LSB         4
#define HAL_SDRAM_TRCD_MSB         5
#define HAL_SDRAM_TRCD_MASK        0x00000030
#define HAL_SDRAM_TRCD_TYPE        halSDRAM_Act2RWDelay_t
#define HAL_SDRAM_TRCD             HAL_SDRAM_TRCD_MASK
#define HAL_SDRAM_TRCD_READABLE    1
#define HAL_SDRAM_TRCD_WRITABLE    1
#define HAL_SDRAM_TRCD_SIGNED      0
#define HAL_SDRAM_TRCD_ADR         0x10000698

#define HAL_SDRAM_NOVL_REGISTER    (HAL_SDRAMREG_MAP->SDCONF4)
#define HAL_SDRAM_NOVL_LSB         2
#define HAL_SDRAM_NOVL_MSB         2
#define HAL_SDRAM_NOVL_MASK        0x00000004
#define HAL_SDRAM_NOVL_TYPE        Bool
#define HAL_SDRAM_NOVL             HAL_SDRAM_NOVL_MASK
#define HAL_SDRAM_NOVL_READABLE    1
#define HAL_SDRAM_NOVL_WRITABLE    1
#define HAL_SDRAM_NOVL_SIGNED      0
#define HAL_SDRAM_NOVL_ADR         0x10000698

#define HAL_SDRAM_BUS32_REGISTER    (HAL_SDRAMREG_MAP->SDCONF4)
#define HAL_SDRAM_BUS32_LSB         0
#define HAL_SDRAM_BUS32_MSB         0
#define HAL_SDRAM_BUS32_MASK        0x00000001
#define HAL_SDRAM_BUS32_TYPE        Bool
#define HAL_SDRAM_BUS32             HAL_SDRAM_BUS32_MASK
#define HAL_SDRAM_BUS32_READABLE    1
#define HAL_SDRAM_BUS32_WRITABLE    1
#define HAL_SDRAM_BUS32_SIGNED      0
#define HAL_SDRAM_BUS32_ADR         0x10000698

// Register SDDELAY0
#define HAL_SDRAM_SDLCHDLY_REGISTER    (HAL_SDRAMREG_MAP->SDDELAY0)
#define HAL_SDRAM_SDLCHDLY_LSB         4
#define HAL_SDRAM_SDLCHDLY_MSB         6
#define HAL_SDRAM_SDLCHDLY_MASK        0x00000070
#define HAL_SDRAM_SDLCHDLY_TYPE        UByte
#define HAL_SDRAM_SDLCHDLY             HAL_SDRAM_SDLCHDLY_MASK
#define HAL_SDRAM_SDLCHDLY_READABLE    1
#define HAL_SDRAM_SDLCHDLY_WRITABLE    1
#define HAL_SDRAM_SDLCHDLY_SIGNED      0
#define HAL_SDRAM_SDLCHDLY_ADR         0x100006A0

#define HAL_SDRAM_SDCKODLY_REGISTER    (HAL_SDRAMREG_MAP->SDDELAY0)
#define HAL_SDRAM_SDCKODLY_LSB         0
#define HAL_SDRAM_SDCKODLY_MSB         3
#define HAL_SDRAM_SDCKODLY_MASK        0x0000000F
#define HAL_SDRAM_SDCKODLY_TYPE        UByte
#define HAL_SDRAM_SDCKODLY             HAL_SDRAM_SDCKODLY_MASK
#define HAL_SDRAM_SDCKODLY_READABLE    1
#define HAL_SDRAM_SDCKODLY_WRITABLE    1
#define HAL_SDRAM_SDCKODLY_SIGNED      0
#define HAL_SDRAM_SDCKODLY_ADR         0x100006A0

// Register SDDELAY1
#define HAL_SDRAM_SDINCKSEL_REGISTER    (HAL_SDRAMREG_MAP->SDDELAY1)
#define HAL_SDRAM_SDINCKSEL_LSB         0
#define HAL_SDRAM_SDINCKSEL_MSB         2
#define HAL_SDRAM_SDINCKSEL_MASK        0x00000007
#define HAL_SDRAM_SDINCKSEL_TYPE        UByte
#define HAL_SDRAM_SDINCKSEL             HAL_SDRAM_SDINCKSEL_MASK
#define HAL_SDRAM_SDINCKSEL_READABLE    1
#define HAL_SDRAM_SDINCKSEL_WRITABLE    1
#define HAL_SDRAM_SDINCKSEL_SIGNED      0
#define HAL_SDRAM_SDINCKSEL_ADR         0x100006A1

// Registers SDBLSEEDL/SDBLSEEDH
#define HAL_SDRAM_SDBLSEED_REGISTER    (HAL_SDRAMREG_MAP->SDBLSEEDL)
#define HAL_SDRAM_SDBLSEED_LSB         0
#define HAL_SDRAM_SDBLSEED_MSB         15
#define HAL_SDRAM_SDBLSEED_MASK        0x0000FFFF
#define HAL_SDRAM_SDBLSEED_TYPE        UInt16
#define HAL_SDRAM_SDBLSEED             HAL_SDRAM_SDBLSEED_MASK
#define HAL_SDRAM_SDBLSEED_READABLE    1
#define HAL_SDRAM_SDBLSEED_WRITABLE    1
#define HAL_SDRAM_SDBLSEED_SIGNED      0
#define HAL_SDRAM_SDBLSEED_ADR         0x100006C0

// Registers SDBUSEEDL/SDBUSEEDH
#define HAL_SDRAM_SDBUSEED_REGISTER    (HAL_SDRAMREG_MAP->SDBUSEEDL)
#define HAL_SDRAM_SDBUSEED_LSB         0
#define HAL_SDRAM_SDBUSEED_MSB         15
#define HAL_SDRAM_SDBUSEED_MASK        0x0000FFFF
#define HAL_SDRAM_SDBUSEED_TYPE        UInt16
#define HAL_SDRAM_SDBUSEED             HAL_SDRAM_SDBUSEED_MASK
#define HAL_SDRAM_SDBUSEED_READABLE    1
#define HAL_SDRAM_SDBUSEED_WRITABLE    1
#define HAL_SDRAM_SDBUSEED_SIGNED      0
#define HAL_SDRAM_SDBUSEED_ADR         0x100006C2

// Registers SDBSTRTAL/SDBSTRTAM/SDBSTRTAH/SDBCTRL
#define HAL_SDRAM_SDBSTRTA_REGISTER    (HAL_SDRAMREG_MAP->SDBSTRTAL)
#define HAL_SDRAM_SDBSTRTA_LSB         0
#define HAL_SDRAM_SDBSTRTA_MSB         23
#define HAL_SDRAM_SDBSTRTA_MASK        0x00FFFFFF
#define HAL_SDRAM_SDBSTRTA_TYPE        UInt32
#define HAL_SDRAM_SDBSTRTA             HAL_SDRAM_SDBSTRTA_MASK
#define HAL_SDRAM_SDBSTRTA_READABLE    1
#define HAL_SDRAM_SDBSTRTA_WRITABLE    1
#define HAL_SDRAM_SDBSTRTA_SIGNED      0
#define HAL_SDRAM_SDBSTRTA_ADR         0x100006C4

#define HAL_SDRAM_SDBINV_REGISTER    (HAL_SDRAMREG_MAP->SDBSTRTAL)
#define HAL_SDRAM_SDBINV_LSB         27
#define HAL_SDRAM_SDBINV_MSB         27
#define HAL_SDRAM_SDBINV_MASK        0x08000000
#define HAL_SDRAM_SDBINV_TYPE        Bool
#define HAL_SDRAM_SDBINV             HAL_SDRAM_SDBINV_MASK
#define HAL_SDRAM_SDBINV_READABLE    1
#define HAL_SDRAM_SDBINV_WRITABLE    1
#define HAL_SDRAM_SDBINV_SIGNED      0
#define HAL_SDRAM_SDBINV_ADR         0x100006C4

#define HAL_SDRAM_SDBLFSR_REGISTER    (HAL_SDRAMREG_MAP->SDBSTRTAL)
#define HAL_SDRAM_SDBLFSR_LSB         26
#define HAL_SDRAM_SDBLFSR_MSB         26
#define HAL_SDRAM_SDBLFSR_MASK        0x04000000
#define HAL_SDRAM_SDBLFSR_TYPE        Bool
#define HAL_SDRAM_SDBLFSR             HAL_SDRAM_SDBLFSR_MASK
#define HAL_SDRAM_SDBLFSR_READABLE    1
#define HAL_SDRAM_SDBLFSR_WRITABLE    1
#define HAL_SDRAM_SDBLFSR_SIGNED      0
#define HAL_SDRAM_SDBLFSR_ADR         0x100006C4

#define HAL_SDRAM_SDBRSTRT_REGISTER    (HAL_SDRAMREG_MAP->SDBSTRTAL)
#define HAL_SDRAM_SDBRSTRT_LSB         25
#define HAL_SDRAM_SDBRSTRT_MSB         25
#define HAL_SDRAM_SDBRSTRT_MASK        0x02000000
#define HAL_SDRAM_SDBRSTRT_TYPE        Bool
#define HAL_SDRAM_SDBRSTRT             HAL_SDRAM_SDBRSTRT_MASK
#define HAL_SDRAM_SDBRSTRT_READABLE    1
#define HAL_SDRAM_SDBRSTRT_WRITABLE    1
#define HAL_SDRAM_SDBRSTRT_SIGNED      0
#define HAL_SDRAM_SDBRSTRT_ADR         0x100006C4

#define HAL_SDRAM_SDBWSTRT_REGISTER    (HAL_SDRAMREG_MAP->SDBSTRTAL)
#define HAL_SDRAM_SDBWSTRT_LSB         24
#define HAL_SDRAM_SDBWSTRT_MSB         24
#define HAL_SDRAM_SDBWSTRT_MASK        0x01000000
#define HAL_SDRAM_SDBWSTRT_TYPE        Bool
#define HAL_SDRAM_SDBWSTRT             HAL_SDRAM_SDBWSTRT_MASK
#define HAL_SDRAM_SDBWSTRT_READABLE    1
#define HAL_SDRAM_SDBWSTRT_WRITABLE    1
#define HAL_SDRAM_SDBWSTRT_SIGNED      0
#define HAL_SDRAM_SDBWSTRT_ADR         0x100006C4

// Registers SDBSTOPAL/SDBSTOPAM/SDBSTOPAH/SDBSTAT
#define HAL_SDRAM_SDBSTOPA_REGISTER    (HAL_SDRAMREG_MAP->SDBSTOPAL)
#define HAL_SDRAM_SDBSTOPA_LSB         0
#define HAL_SDRAM_SDBSTOPA_MSB         23
#define HAL_SDRAM_SDBSTOPA_MASK        0x00FFFFFF
#define HAL_SDRAM_SDBSTOPA_TYPE        UInt32
#define HAL_SDRAM_SDBSTOPA             HAL_SDRAM_SDBSTOPA_MASK
#define HAL_SDRAM_SDBSTOPA_READABLE    1
#define HAL_SDRAM_SDBSTOPA_WRITABLE    1
#define HAL_SDRAM_SDBSTOPA_SIGNED      0
#define HAL_SDRAM_SDBSTOPA_ADR         0x100006C8

#define HAL_SDRAM_SDBACT_REGISTER    (HAL_SDRAMREG_MAP->SDBSTOPAL)
#define HAL_SDRAM_SDBACT_LSB         26
#define HAL_SDRAM_SDBACT_MSB         26
#define HAL_SDRAM_SDBACT_MASK        0x04000000
#define HAL_SDRAM_SDBACT_TYPE        Bool
#define HAL_SDRAM_SDBACT             HAL_SDRAM_SDBACT_MASK
#define HAL_SDRAM_SDBACT_READABLE    1
#define HAL_SDRAM_SDBACT_WRITABLE    0
#define HAL_SDRAM_SDBACT_SIGNED      0
#define HAL_SDRAM_SDBACT_ADR         0x100006C8

#define HAL_SDRAM_SDBDONE_REGISTER    (HAL_SDRAMREG_MAP->SDBSTOPAL)
#define HAL_SDRAM_SDBDONE_LSB         25
#define HAL_SDRAM_SDBDONE_MSB         25
#define HAL_SDRAM_SDBDONE_MASK        0x02000000
#define HAL_SDRAM_SDBDONE_TYPE        Bool
#define HAL_SDRAM_SDBDONE             HAL_SDRAM_SDBDONE_MASK
#define HAL_SDRAM_SDBDONE_READABLE    1
#define HAL_SDRAM_SDBDONE_WRITABLE    0
#define HAL_SDRAM_SDBDONE_SIGNED      0
#define HAL_SDRAM_SDBDONE_ADR         0x100006C8

#define HAL_SDRAM_SDBFAIL_REGISTER    (HAL_SDRAMREG_MAP->SDBSTOPAL)
#define HAL_SDRAM_SDBFAIL_LSB         24
#define HAL_SDRAM_SDBFAIL_MSB         24
#define HAL_SDRAM_SDBFAIL_MASK        0x01000000
#define HAL_SDRAM_SDBFAIL_TYPE        Bool
#define HAL_SDRAM_SDBFAIL             HAL_SDRAM_SDBFAIL_MASK
#define HAL_SDRAM_SDBFAIL_READABLE    1
#define HAL_SDRAM_SDBFAIL_WRITABLE    0
#define HAL_SDRAM_SDBFAIL_SIGNED      0
#define HAL_SDRAM_SDBFAIL_ADR         0x100006C8

// Registers SDBFAILAL/SDBFAILAM/SDBFAILAH
#define HAL_SDRAM_SDBFAILA_REGISTER    (HAL_SDRAMREG_MAP->SDBFAILAL)
#define HAL_SDRAM_SDBFAILA_LSB         0
#define HAL_SDRAM_SDBFAILA_MSB         23
#define HAL_SDRAM_SDBFAILA_MASK        0x00FFFFFF
#define HAL_SDRAM_SDBFAILA_TYPE        UInt32
#define HAL_SDRAM_SDBFAILA             HAL_SDRAM_SDBFAILA_MASK
#define HAL_SDRAM_SDBFAILA_READABLE    1
#define HAL_SDRAM_SDBFAILA_WRITABLE    0
#define HAL_SDRAM_SDBFAILA_SIGNED      0
#define HAL_SDRAM_SDBFAILA_ADR         0x100006CC

// Registers SDBCURRAL/SDBCURRAM/SDBCURRAH
#define HAL_SDRAM_SDBCURRA_REGISTER    (HAL_SDRAMREG_MAP->SDBCURRAL)
#define HAL_SDRAM_SDBCURRA_LSB         0
#define HAL_SDRAM_SDBCURRA_MSB         23
#define HAL_SDRAM_SDBCURRA_MASK        0x00FFFFFF
#define HAL_SDRAM_SDBCURRA_TYPE        UInt32
#define HAL_SDRAM_SDBCURRA             HAL_SDRAM_SDBCURRA_MASK
#define HAL_SDRAM_SDBCURRA_READABLE    1
#define HAL_SDRAM_SDBCURRA_WRITABLE    0
#define HAL_SDRAM_SDBCURRA_SIGNED      0
#define HAL_SDRAM_SDBCURRA_ADR         0x100006D0

// Enumerations
typedef enum
{
	halSDRAM_SDSize_16Mb0 = 0,    // SDRAM type 16Mb, total 2MB, 16bits Width, 2 Banks
	halSDRAM_SDSize_16Mb2 = 2,    // SDRAM type 16Mb, total 2MB, 16bits Width, 2 Banks
	halSDRAM_SDSize_32Mb  = 3,    // SDRAM type 32Mb, total 4MB, 32bits Width, 2 Banks
	halSDRAM_SDSize_64Mb  = 4,    // SDRAM type 64Mb, total 8MB, 32bits Width, 4 Banks
	halSDRAM_SDSize_128Mb = 5,    // SDRAM type 128Mb, total 16MB, 32bits Width, 4 Banks
	halSDRAM_SDSize_256Mb = 6     // SDRAM type 256Mb, total 32MB, 16bits Width, 4 Banks
} halSDRAM_SdramSize_t;

typedef enum
{
	halSDRAM_ARefP_7SysClk  = 0,    // Auto Refresh Command Period: 7 SYSCLK periods
	halSDRAM_ARefP_8SysClk  = 1,    // Auto Refresh Command Period: 8 SYSCLK periods
	halSDRAM_ARefP_9SysClk  = 2,    // Auto Refresh Command Period: 9 SYSCLK periods
	halSDRAM_ARefP_10SysClk = 3,    // Auto Refresh Command Period: 10 SYSCLK periods
	halSDRAM_ARefP_11SysClk = 4,    // Auto Refresh Command Period: 11 SYSCLK periods
	halSDRAM_ARefP_12SysClk = 5,    // Auto Refresh Command Period: 12 SYSCLK periods
	halSDRAM_ARefP_13SysClk = 6,    // Auto Refresh Command Period: 13 SYSCLK periods
	halSDRAM_ARefP_14SysClk = 7,    // Auto Refresh Command Period: 14 SYSCLK periods
	halSDRAM_ARefP_15SysClk = 8,    // Auto Refresh Command Period: 15 SYSCLK periods
	halSDRAM_ARefP_16SysClk = 9,    // Auto Refresh Command Period: 16 SYSCLK periods
	halSDRAM_ARefP_17SysClk = 10,    // Auto Refresh Command Period: 17 SYSCLK periods
	halSDRAM_ARefP_18SysClk = 11,    // Auto Refresh Command Period: 18 SYSCLK periods
	halSDRAM_ARefP_19SysClk = 12,    // Auto Refresh Command Period: 19 SYSCLK periods
	halSDRAM_ARefP_20SysClk = 13,    // Auto Refresh Command Period: 20 SYSCLK periods
	halSDRAM_ARefP_21SysClk = 14,    // Auto Refresh Command Period: 21 SYSCLK periods
	halSDRAM_ARefP_22SysClk = 15     // Auto Refresh Command Period: 22 SYSCLK periods
} halSDRAM_ARefreshPeriod_t;

typedef enum
{
	halSDRAM_WrRecovery_2SysClk = 0,    // Write Recovery Time: 2 SYSCLK periods
	halSDRAM_WrRecovery_3SysClk = 1     // Write Recovery Time: 3 SYSCLK periods
} halSDRAM_WrRecovery_t;

typedef enum
{
	halSDRAM_Act2PreC_4SysClk  = 0,    // Active to Precharge Command Period: 4 SYSCLK periods
	halSDRAM_Act2PreC_5SysClk  = 1,    // Active to Precharge Command Period: 5 SYSCLK periods
	halSDRAM_Act2PreC_6SysClk  = 2,    // Active to Precharge Command Period: 6 SYSCLK periods
	halSDRAM_Act2PreC_7SysClk  = 3,    // Active to Precharge Command Period: 7 SYSCLK periods
	halSDRAM_Act2PreC_8SysClk  = 4,    // Active to Precharge Command Period: 8 SYSCLK periods
	halSDRAM_Act2PreC_9SysClk  = 5,    // Active to Precharge Command Period: 9 SYSCLK periods
	halSDRAM_Act2PreC_10SysClk = 6,    // Active to Precharge Command Period: 10 SYSCLK periods
	halSDRAM_Act2PreC_11SysClk = 7     // Active to Precharge Command Period: 11 SYSCLK periods
} halSDRAM_Act2Precharge_t;

typedef enum
{
	halSDRAM_PreCP_2SysClk = 0,    // Precharge Command Period: 2 SYSCLK periods
	halSDRAM_PreCP_3SysClk = 1,    // Precharge Command Period: 3 SYSCLK periods
	halSDRAM_PreCP_4SysClk = 2,    // Precharge Command Period: 4 SYSCLK periods
	halSDRAM_PreCP_5SysClk = 3     // Precharge Command Period: 5 SYSCLK periods
} halSDRAM_PreCPeriod_t;

typedef enum
{
	halSDRAM_Act2RWDelay_2SysClk = 0,    // Active to Read or Write Delay: 2 SYSCLK periods
	halSDRAM_Act2RWDelay_3SysClk = 1,    // Active to Read or Write Delay: 3 SYSCLK periods
	halSDRAM_Act2RWDelay_4SysClk = 2,    // Active to Read or Write Delay: 4 SYSCLK periods
	halSDRAM_Act2RWDelay_5SysClk = 3     // Active to Read or Write Delay: 5 SYSCLK periods
} halSDRAM_Act2RWDelay_t;

#endif /* __REG_SDRAM_H__ */

