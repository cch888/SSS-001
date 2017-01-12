/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_audio.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the AUD core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_AUDIO_H__
#define __REG_AUDIO_H__

// Core register map
#define HAL_AUD_VERSION 7.6
#define HAL_AUD_BaseAddress 0x10000A00
#define HAL_AUDREG_MAP ((struct halAUDReg_Map*) (void *) HAL_AUD_BaseAddress)
__packed struct halAUDReg_Map
{
	HAL_Reg8  APLAYCTL;                       //00000000
	HAL_Reg8  AINT;                           //00000001
	HAL_Reg8  AINTEN;                         //00000002
	HAL_Reg8  AROUTE;                         //00000003
	HAL_Reg8  AMUTE;                          //00000004
	HAL_Reg8  APROCCTL;                       //00000005
	HAL_Reg16 ATTLA;                          //00000006
	HAL_Reg8  ReservedBlk0[2];                //00000008
	HAL_Reg16 ASTPGL;                         //0000000A
	HAL_Reg16 AENDPGL;                        //0000000C
	HAL_Reg16 ABUFPGL;                        //0000000E
	HAL_Reg16 ACURPGL;                        //00000010
	HAL_Reg8  APDMCTL;                        //00000012
};
// Register APLAYCTL
#define HAL_AUD_APLAYEN_REGISTER    (HAL_AUDREG_MAP->APLAYCTL)
#define HAL_AUD_APLAYEN_LSB         7
#define HAL_AUD_APLAYEN_MSB         7
#define HAL_AUD_APLAYEN_MASK        0x00000080
#define HAL_AUD_APLAYEN_TYPE        Bool
#define HAL_AUD_APLAYEN             HAL_AUD_APLAYEN_MASK
#define HAL_AUD_APLAYEN_READABLE    1
#define HAL_AUD_APLAYEN_WRITABLE    1
#define HAL_AUD_APLAYEN_SIGNED      0
#define HAL_AUD_APLAYEN_ADR         0x10000A00

#define HAL_AUD_A1KAUX_REGISTER    (HAL_AUDREG_MAP->APLAYCTL)
#define HAL_AUD_A1KAUX_LSB         4
#define HAL_AUD_A1KAUX_MSB         4
#define HAL_AUD_A1KAUX_MASK        0x00000010
#define HAL_AUD_A1KAUX_TYPE        halAUD_AAUXSize_t
#define HAL_AUD_A1KAUX             HAL_AUD_A1KAUX_MASK
#define HAL_AUD_A1KAUX_READABLE    1
#define HAL_AUD_A1KAUX_WRITABLE    1
#define HAL_AUD_A1KAUX_SIGNED      0
#define HAL_AUD_A1KAUX_ADR         0x10000A00

// Register AINT
#define HAL_AUD_ABLKINT_REGISTER    (HAL_AUDREG_MAP->AINT)
#define HAL_AUD_ABLKINT_LSB         0
#define HAL_AUD_ABLKINT_MSB         0
#define HAL_AUD_ABLKINT_MASK        0x00000001
#define HAL_AUD_ABLKINT_TYPE        Bool
#define HAL_AUD_ABLKINT             HAL_AUD_ABLKINT_MASK
#define HAL_AUD_ABLKINT_READABLE    1
#define HAL_AUD_ABLKINT_WRITABLE    1
#define HAL_AUD_ABLKINT_SIGNED      0
#define HAL_AUD_ABLKINT_ADR         0x10000A01

// Register AINTEN
#define HAL_AUD_ABLKIEN_REGISTER    (HAL_AUDREG_MAP->AINTEN)
#define HAL_AUD_ABLKIEN_LSB         0
#define HAL_AUD_ABLKIEN_MSB         0
#define HAL_AUD_ABLKIEN_MASK        0x00000001
#define HAL_AUD_ABLKIEN_TYPE        Bool
#define HAL_AUD_ABLKIEN             HAL_AUD_ABLKIEN_MASK
#define HAL_AUD_ABLKIEN_READABLE    1
#define HAL_AUD_ABLKIEN_WRITABLE    1
#define HAL_AUD_ABLKIEN_SIGNED      0
#define HAL_AUD_ABLKIEN_ADR         0x10000A02

// Register AROUTE
#define HAL_AUD_EMPHSEL_REGISTER    (HAL_AUDREG_MAP->AROUTE)
#define HAL_AUD_EMPHSEL_LSB         6
#define HAL_AUD_EMPHSEL_MSB         6
#define HAL_AUD_EMPHSEL_MASK        0x00000040
#define HAL_AUD_EMPHSEL_TYPE        Bool
#define HAL_AUD_EMPHSEL             HAL_AUD_EMPHSEL_MASK
#define HAL_AUD_EMPHSEL_READABLE    1
#define HAL_AUD_EMPHSEL_WRITABLE    1
#define HAL_AUD_EMPHSEL_SIGNED      0
#define HAL_AUD_EMPHSEL_ADR         0x10000A03

#define HAL_AUD_BYPEMPH_REGISTER    (HAL_AUDREG_MAP->AROUTE)
#define HAL_AUD_BYPEMPH_LSB         5
#define HAL_AUD_BYPEMPH_MSB         5
#define HAL_AUD_BYPEMPH_MASK        0x00000020
#define HAL_AUD_BYPEMPH_TYPE        Bool
#define HAL_AUD_BYPEMPH             HAL_AUD_BYPEMPH_MASK
#define HAL_AUD_BYPEMPH_READABLE    1
#define HAL_AUD_BYPEMPH_WRITABLE    1
#define HAL_AUD_BYPEMPH_SIGNED      0
#define HAL_AUD_BYPEMPH_ADR         0x10000A03

#define HAL_AUD_BYPATT_REGISTER    (HAL_AUDREG_MAP->AROUTE)
#define HAL_AUD_BYPATT_LSB         4
#define HAL_AUD_BYPATT_MSB         4
#define HAL_AUD_BYPATT_MASK        0x00000010
#define HAL_AUD_BYPATT_TYPE        Bool
#define HAL_AUD_BYPATT             HAL_AUD_BYPATT_MASK
#define HAL_AUD_BYPATT_READABLE    1
#define HAL_AUD_BYPATT_WRITABLE    1
#define HAL_AUD_BYPATT_SIGNED      0
#define HAL_AUD_BYPATT_ADR         0x10000A03

#define HAL_AUD_CHASEL_REGISTER    (HAL_AUDREG_MAP->AROUTE)
#define HAL_AUD_CHASEL_LSB         2
#define HAL_AUD_CHASEL_MSB         3
#define HAL_AUD_CHASEL_MASK        0x0000000C
#define HAL_AUD_CHASEL_TYPE        halAUD_ACHASEL_t
#define HAL_AUD_CHASEL             HAL_AUD_CHASEL_MASK
#define HAL_AUD_CHASEL_READABLE    1
#define HAL_AUD_CHASEL_WRITABLE    1
#define HAL_AUD_CHASEL_SIGNED      0
#define HAL_AUD_CHASEL_ADR         0x10000A03

#define HAL_AUD_CHBSEL_REGISTER    (HAL_AUDREG_MAP->AROUTE)
#define HAL_AUD_CHBSEL_LSB         0
#define HAL_AUD_CHBSEL_MSB         1
#define HAL_AUD_CHBSEL_MASK        0x00000003
#define HAL_AUD_CHBSEL_TYPE        halAUD_ACHBSEL_t
#define HAL_AUD_CHBSEL             HAL_AUD_CHBSEL_MASK
#define HAL_AUD_CHBSEL_READABLE    1
#define HAL_AUD_CHBSEL_WRITABLE    1
#define HAL_AUD_CHBSEL_SIGNED      0
#define HAL_AUD_CHBSEL_ADR         0x10000A03

// Register AMUTE
#define HAL_AUD_DACMUTE_REGISTER    (HAL_AUDREG_MAP->AMUTE)
#define HAL_AUD_DACMUTE_LSB         3
#define HAL_AUD_DACMUTE_MSB         3
#define HAL_AUD_DACMUTE_MASK        0x00000008
#define HAL_AUD_DACMUTE_TYPE        Bool
#define HAL_AUD_DACMUTE             HAL_AUD_DACMUTE_MASK
#define HAL_AUD_DACMUTE_READABLE    1
#define HAL_AUD_DACMUTE_WRITABLE    1
#define HAL_AUD_DACMUTE_SIGNED      0
#define HAL_AUD_DACMUTE_ADR         0x10000A04

#define HAL_AUD_MUTE4X_REGISTER    (HAL_AUDREG_MAP->AMUTE)
#define HAL_AUD_MUTE4X_LSB         2
#define HAL_AUD_MUTE4X_MSB         2
#define HAL_AUD_MUTE4X_MASK        0x00000004
#define HAL_AUD_MUTE4X_TYPE        Bool
#define HAL_AUD_MUTE4X             HAL_AUD_MUTE4X_MASK
#define HAL_AUD_MUTE4X_READABLE    1
#define HAL_AUD_MUTE4X_WRITABLE    1
#define HAL_AUD_MUTE4X_SIGNED      0
#define HAL_AUD_MUTE4X_ADR         0x10000A04

// Register APROCCTL
#define HAL_AUD_EMPH_REGISTER    (HAL_AUDREG_MAP->APROCCTL)
#define HAL_AUD_EMPH_LSB         7
#define HAL_AUD_EMPH_MSB         7
#define HAL_AUD_EMPH_MASK        0x00000080
#define HAL_AUD_EMPH_TYPE        Bool
#define HAL_AUD_EMPH             HAL_AUD_EMPH_MASK
#define HAL_AUD_EMPH_READABLE    1
#define HAL_AUD_EMPH_WRITABLE    1
#define HAL_AUD_EMPH_SIGNED      0
#define HAL_AUD_EMPH_ADR         0x10000A05

#define HAL_AUD_ATTBSEL_REGISTER    (HAL_AUDREG_MAP->APROCCTL)
#define HAL_AUD_ATTBSEL_LSB         2
#define HAL_AUD_ATTBSEL_MSB         2
#define HAL_AUD_ATTBSEL_MASK        0x00000004
#define HAL_AUD_ATTBSEL_TYPE        Bool
#define HAL_AUD_ATTBSEL             HAL_AUD_ATTBSEL_MASK
#define HAL_AUD_ATTBSEL_READABLE    1
#define HAL_AUD_ATTBSEL_WRITABLE    1
#define HAL_AUD_ATTBSEL_SIGNED      0
#define HAL_AUD_ATTBSEL_ADR         0x10000A05

// Registers ATTLA/ATTLB
#define HAL_AUD_ATTLA_REGISTER    (HAL_AUDREG_MAP->ATTLA)
#define HAL_AUD_ATTLA_LSB         0
#define HAL_AUD_ATTLA_MSB         7
#define HAL_AUD_ATTLA_MASK        0x000000FF
#define HAL_AUD_ATTLA_TYPE        UByte
#define HAL_AUD_ATTLA             HAL_AUD_ATTLA_MASK
#define HAL_AUD_ATTLA_READABLE    1
#define HAL_AUD_ATTLA_WRITABLE    1
#define HAL_AUD_ATTLA_SIGNED      0
#define HAL_AUD_ATTLA_ADR         0x10000A06

#define HAL_AUD_ATTLB_REGISTER    (HAL_AUDREG_MAP->ATTLA)
#define HAL_AUD_ATTLB_LSB         8
#define HAL_AUD_ATTLB_MSB         15
#define HAL_AUD_ATTLB_MASK        0x0000FF00
#define HAL_AUD_ATTLB_TYPE        UByte
#define HAL_AUD_ATTLB             HAL_AUD_ATTLB_MASK
#define HAL_AUD_ATTLB_READABLE    1
#define HAL_AUD_ATTLB_WRITABLE    1
#define HAL_AUD_ATTLB_SIGNED      0
#define HAL_AUD_ATTLB_ADR         0x10000A06

// Registers ASTPGL/ASTPGH
#define HAL_AUD_ASTPG_REGISTER    (HAL_AUDREG_MAP->ASTPGL)
#define HAL_AUD_ASTPG_LSB         0
#define HAL_AUD_ASTPG_MSB         12
#define HAL_AUD_ASTPG_MASK        0x00001FFF
#define HAL_AUD_ASTPG_TYPE        UInt16
#define HAL_AUD_ASTPG             HAL_AUD_ASTPG_MASK
#define HAL_AUD_ASTPG_READABLE    1
#define HAL_AUD_ASTPG_WRITABLE    1
#define HAL_AUD_ASTPG_SIGNED      0
#define HAL_AUD_ASTPG_ADR         0x10000A0A

// Registers AENDPGL/AENDPGH
#define HAL_AUD_AENDPG_REGISTER    (HAL_AUDREG_MAP->AENDPGL)
#define HAL_AUD_AENDPG_LSB         0
#define HAL_AUD_AENDPG_MSB         12
#define HAL_AUD_AENDPG_MASK        0x00001FFF
#define HAL_AUD_AENDPG_TYPE        UInt16
#define HAL_AUD_AENDPG             HAL_AUD_AENDPG_MASK
#define HAL_AUD_AENDPG_READABLE    1
#define HAL_AUD_AENDPG_WRITABLE    1
#define HAL_AUD_AENDPG_SIGNED      0
#define HAL_AUD_AENDPG_ADR         0x10000A0C

// Registers ABUFPGL/ABUFPGH
#define HAL_AUD_ABUFPG_REGISTER    (HAL_AUDREG_MAP->ABUFPGL)
#define HAL_AUD_ABUFPG_LSB         0
#define HAL_AUD_ABUFPG_MSB         12
#define HAL_AUD_ABUFPG_MASK        0x00001FFF
#define HAL_AUD_ABUFPG_TYPE        UInt16
#define HAL_AUD_ABUFPG             HAL_AUD_ABUFPG_MASK
#define HAL_AUD_ABUFPG_READABLE    1
#define HAL_AUD_ABUFPG_WRITABLE    1
#define HAL_AUD_ABUFPG_SIGNED      0
#define HAL_AUD_ABUFPG_ADR         0x10000A0E

// Registers ACURPGL/ACURPGH
#define HAL_AUD_ACURPG_REGISTER    (HAL_AUDREG_MAP->ACURPGL)
#define HAL_AUD_ACURPG_LSB         0
#define HAL_AUD_ACURPG_MSB         12
#define HAL_AUD_ACURPG_MASK        0x00001FFF
#define HAL_AUD_ACURPG_TYPE        UInt16
#define HAL_AUD_ACURPG             HAL_AUD_ACURPG_MASK
#define HAL_AUD_ACURPG_READABLE    1
#define HAL_AUD_ACURPG_WRITABLE    0
#define HAL_AUD_ACURPG_SIGNED      0
#define HAL_AUD_ACURPG_ADR         0x10000A10

// Register APDMCTL
#define HAL_AUD_DITHERFS_REGISTER    (HAL_AUDREG_MAP->APDMCTL)
#define HAL_AUD_DITHERFS_LSB         4
#define HAL_AUD_DITHERFS_MSB         4
#define HAL_AUD_DITHERFS_MASK        0x00000010
#define HAL_AUD_DITHERFS_TYPE        Bool
#define HAL_AUD_DITHERFS             HAL_AUD_DITHERFS_MASK
#define HAL_AUD_DITHERFS_READABLE    1
#define HAL_AUD_DITHERFS_WRITABLE    1
#define HAL_AUD_DITHERFS_SIGNED      0
#define HAL_AUD_DITHERFS_ADR         0x10000A12

#define HAL_AUD_PDMOUTEN_REGISTER    (HAL_AUDREG_MAP->APDMCTL)
#define HAL_AUD_PDMOUTEN_LSB         3
#define HAL_AUD_PDMOUTEN_MSB         3
#define HAL_AUD_PDMOUTEN_MASK        0x00000008
#define HAL_AUD_PDMOUTEN_TYPE        Bool
#define HAL_AUD_PDMOUTEN             HAL_AUD_PDMOUTEN_MASK
#define HAL_AUD_PDMOUTEN_READABLE    1
#define HAL_AUD_PDMOUTEN_WRITABLE    1
#define HAL_AUD_PDMOUTEN_SIGNED      0
#define HAL_AUD_PDMOUTEN_ADR         0x10000A12

#define HAL_AUD_PDMOUTDS_REGISTER    (HAL_AUDREG_MAP->APDMCTL)
#define HAL_AUD_PDMOUTDS_LSB         2
#define HAL_AUD_PDMOUTDS_MSB         2
#define HAL_AUD_PDMOUTDS_MASK        0x00000004
#define HAL_AUD_PDMOUTDS_TYPE        halAUD_APDMOUTDS_t
#define HAL_AUD_PDMOUTDS             HAL_AUD_PDMOUTDS_MASK
#define HAL_AUD_PDMOUTDS_READABLE    1
#define HAL_AUD_PDMOUTDS_WRITABLE    1
#define HAL_AUD_PDMOUTDS_SIGNED      0
#define HAL_AUD_PDMOUTDS_ADR         0x10000A12

#define HAL_AUD_PDMFREQ_REGISTER    (HAL_AUDREG_MAP->APDMCTL)
#define HAL_AUD_PDMFREQ_LSB         0
#define HAL_AUD_PDMFREQ_MSB         1
#define HAL_AUD_PDMFREQ_MASK        0x00000003
#define HAL_AUD_PDMFREQ_TYPE        halAUD_APDMRefFreq_t
#define HAL_AUD_PDMFREQ             HAL_AUD_PDMFREQ_MASK
#define HAL_AUD_PDMFREQ_READABLE    1
#define HAL_AUD_PDMFREQ_WRITABLE    1
#define HAL_AUD_PDMFREQ_SIGNED      0
#define HAL_AUD_PDMFREQ_ADR         0x10000A12

// Enumerations
typedef enum
{
	halAUD_AAUXSize_512B = 0,    // Select Audio 512 byte AUX size
	halAUD_AAUXSize_1kB  = 1     // Select Audio 1 kB AUX size
} halAUD_AAUXSize_t;

typedef enum
{
	halAUD_ACHASEL_Lch  = 0,    // Audio Channel A is output:L-ch
	halAUD_ACHASEL_Rch  = 1,    // Audio Channel A is output:R-ch
	halAUD_ACHASEL_Mono = 2     // Audio Channel A is output:Mono (L-ch + R-ch)/2
} halAUD_ACHASEL_t;

typedef enum
{
	halAUD_ACHBSEL_Rch  = 0,    // Audio Channel B is output:R-ch
	halAUD_ACHBSEL_Lch  = 1,    // Audio Channel B is output:L-ch
	halAUD_ACHBSEL_Mono = 2     // Audio Channel B is output:Mono (L-ch + R-ch)/2
} halAUD_ACHBSEL_t;

typedef enum
{
	halAUD_APDMOUTDS_4mA = 0,    // Audio PDM Output Drive Strength = 4 mA
	halAUD_APDMOUTDS_8mA = 1     // Audio PDM Output Drive Strength = 8 mA
} halAUD_APDMOUTDS_t;

typedef enum
{
	halAUD_APDMRefFreg_192fs = 0,    // Audio PDM Output Reference Frequency 192 fs
	halAUD_APDMRefFreg_96fs  = 1,    // Audio PDM Output Reference Frequency 96 fs
	halAUD_APDMRefFreg_48fs  = 2     // Audio PDM Output Reference Frequency 48 fs
} halAUD_APDMRefFreq_t;

#endif /* __REG_AUDIO_H__ */

