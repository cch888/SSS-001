/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_remap.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the SOC_AHB_REMAP core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_REMAP_H__
#define __REG_REMAP_H__

// Core register map
#define HAL_AHB_REMAP_VERSION 7.142
#define HAL_AHB_REMAP_BaseAddress 0xC8000000
#define HAL_AHB_REMAPREG_MAP ((struct halAHB_REMAPReg_Map*) (void *) HAL_AHB_REMAP_BaseAddress)
__packed struct halAHB_REMAPReg_Map
{
	HAL_Reg8  PAUSECTL;                       //00000000
	HAL_Reg8  ReservedBlk0[3];                //00000001
	HAL_Reg8  REMAPCTL;                       //00000004
};
// Register PAUSECTL
#define HAL_AHB_REMAP_PAUSECTL_REGISTER    (HAL_AHB_REMAPREG_MAP->PAUSECTL)
#define HAL_AHB_REMAP_PAUSECTL_LSB         0
#define HAL_AHB_REMAP_PAUSECTL_MSB         7
#define HAL_AHB_REMAP_PAUSECTL_MASK        0x000000FF
#define HAL_AHB_REMAP_PAUSECTL_TYPE        UByte
#define HAL_AHB_REMAP_PAUSECTL             HAL_AHB_REMAP_PAUSECTL_MASK
#define HAL_AHB_REMAP_PAUSECTL_READABLE    0
#define HAL_AHB_REMAP_PAUSECTL_WRITABLE    1
#define HAL_AHB_REMAP_PAUSECTL_SIGNED      0
#define HAL_AHB_REMAP_PAUSECTL_ADR         0xC8000000

// Register REMAPCTL
#define HAL_AHB_REMAP_MAPTO0_REGISTER    (HAL_AHB_REMAPREG_MAP->REMAPCTL)
#define HAL_AHB_REMAP_MAPTO0_LSB         0
#define HAL_AHB_REMAP_MAPTO0_MSB         7
#define HAL_AHB_REMAP_MAPTO0_MASK        0x000000FF
#define HAL_AHB_REMAP_MAPTO0_TYPE        UByte
#define HAL_AHB_REMAP_MAPTO0             HAL_AHB_REMAP_MAPTO0_MASK
#define HAL_AHB_REMAP_MAPTO0_READABLE    0
#define HAL_AHB_REMAP_MAPTO0_WRITABLE    1
#define HAL_AHB_REMAP_MAPTO0_SIGNED      0
#define HAL_AHB_REMAP_MAPTO0_ADR         0xC8000004

#endif /* __REG_REMAP_H__ */

