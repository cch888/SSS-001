/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     dvDef.h
*
* DESCRIPTION   This file contains DVD related #define(s) and typedefs
*
* $Revision: 72 $
* $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __DVD_DEF_H__
#define __DVD_DEF_H__

#include ".\hif\gp_ram.h"
#include ".\common\sdram.h"
#include ".\dvd\dvcfg.h"

//**********************************************************************************************
//
//
//                                 #Defines for DVD code
//
//
//
//**********************************************************************************************


#define DVD_CACHE_WAIT_BLK_SIZE         100         /* Waiting for Cache HIT (Max 30 blocks) */

/****************************/
/*Title Key define          */
/****************************/
#define MASK_CPM            0x80            /*CPM MASK*/
#define MASK_CP_SEC         0x40            /*CP_SEC MASK*/
#define MASK_CP_MOD         0xB0            /**/

/************************************************************************************************

    Common DVD Defines

*************************************************************************************************/

#define DVD_BLOCK_SIZE          0x8000          /* One DVD ECC Block in Bytes */

#define DVD_SECTOR_SIZE         0x800           /* One sector = 2048 bytes */

#define SECTORS_PER_BLOCK       ONE_DVD_BLOCK   /* Number of sectors in one dvd block */

#define ONE_DVD_AUX_BLOCK_LEN   0x100           /* One DVD Aux Block in Bytes (256 Bytes)*/

#define ONE_AUX_DVD_ENTRY_LEN   16              /* 16 bytes of data per AUX entry                  */
#define ONE_AUX_DVD_BLOCK       0x10            /* One Aux. DVD Block = 256 bytes = 0x100          */
                                                /* It has a resolution of 16 bytes, so mask out    */
                                                /* the lower 4 bits in 20 bit address range:       */
                                                /* 0x100 >>= 4 = 0x10                              */

#define START_BLOCK_WHEN_ERROR_LOGGING   (DVD_END_BLOCK - 0x120)  // Starting Data Block Address in DVD Buffer RAM
                                                //   while DVD Error Logging is active.
                                                // While DVD Error Logging the lower area of the buffer is reserved for DVD Error Logging data

#define DVD_PIPO_BLOCK_SIZE     7328            // Bytes needed for PI/PO data for each DVD ecc block */

#define DVD_BLANK_DETECTOR_CLOCK_DIVIDER (CLK16)  // Divider used for DVD+R Blank area detector

/* RMD Field Definitions and Offsets */

#if 0   // TBD? are these definitions required?
/* RMD Field 0 Definitions */
#define RMD_FORMAT                      (word)(FORMAT1_RMD0+0x00)
#define DISC_STATUS                     (word)(FORMAT1_RMD0+0x02)
#define UNIQUE_DISCID_RANDOMDATA        (word)(FORMAT1_RMD0+0x06)
#define UNIQUE_DISCID_YEAR              (word)(FORMAT1_RMD0+0x08)
#define UNIQUE_DISCID_MONTH             (word)(FORMAT1_RMD0+0x0C)
#define UNIQUE_DISCID_DAY               (word)(FORMAT1_RMD0+0x0E)
#define UNIQUE_DISCID_HOUR              (word)(FORMAT1_RMD0+0x10)
#define UNIQUE_DISCID_MIN               (word)(FORMAT1_RMD0+0x12)
#define UNIQUE_DISCID_SEC               (word)(FORMAT1_RMD0+0x14)
#define PREREC_INFO_FLDID_1             (word)(FORMAT1_RMD0+0x16)
#define PREREC_INFO_DISC_APPCODE        (word)(FORMAT1_RMD0+0x17)
#define PREREC_INFO_DISC_PHYCODE        (word)(FORMAT1_RMD0+0x18)
#define PREREC_INFO_LAST_ADDR           (word)(FORMAT1_RMD0+0x19)
#define PREREC_INFO_FLDID_2             (word)(FORMAT1_RMD0+0x1E)
#define PREREC_INFO_OPC                 (word)(FORMAT1_RMD0+0x1F)
#define PREREC_INFO_WAVELNGTH           (word)(FORMAT1_RMD0+0x20)
#define PREREC_INFO_WRSTRTGY            (word)(FORMAT1_RMD0+0x21)
#define PREREC_INFO_FLDID_3             (word)(FORMAT1_RMD0+0x26)
#define PREREC_INFO_MID17               (word)(FORMAT1_RMD0+0x27)
#define PREREC_INFO_MID16               (word)(FORMAT1_RMD0+0x28)
#define PREREC_INFO_MID15               (word)(FORMAT1_RMD0+0x29)
#define PREREC_INFO_MID14               (word)(FORMAT1_RMD0+0x2A)
#define PREREC_INFO_MID13               (word)(FORMAT1_RMD0+0x2B)
#define PREREC_INFO_MID12               (word)(FORMAT1_RMD0+0x2C)
#define PREREC_INFO_FLDID_4             (word)(FORMAT1_RMD0+0x2E)
#define PREREC_INFO_MID11               (word)(FORMAT1_RMD0+0x2F)
#define PREREC_INFO_MID10               (word)(FORMAT1_RMD0+0x30)
#define PREREC_INFO_MID09               (word)(FORMAT1_RMD0+0x31)
#define PREREC_INFO_MID08               (word)(FORMAT1_RMD0+0x32)
#define PREREC_INFO_MID07               (word)(FORMAT1_RMD0+0x33)
#define PREREC_INFO_MID06               (word)(FORMAT1_RMD0+0x34)
#define PREREC_INFO_FLDID_5             (word)(FORMAT1_RMD0+0x36)
#define PREREC_INFO_MID05               (word)(FORMAT1_RMD0+0x37)
#define PREREC_INFO_MID04               (word)(FORMAT1_RMD0+0x38)
#define PREREC_INFO_MID03               (word)(FORMAT1_RMD0+0x39)
#define PREREC_INFO_MID02               (word)(FORMAT1_RMD0+0x3A)
#define PREREC_INFO_MID01               (word)(FORMAT1_RMD0+0x3B)
#define PREREC_INFO_MID00               (word)(FORMAT1_RMD0+0x3C)

/* RMD Field 1 Definitions */
#define VENDOR_ID1                      (word)(FORMAT1_RMD1+0x00)
#define SERIAL_NUM1                     (word)(FORMAT1_RMD1+0x20)
#define MODEL_NUM1                      (word)(FORMAT1_RMD1+0x30)
#define WRITE_STRTGY1                   (word)(FORMAT1_RMD1+0x40)
#define REC_POWER1                      (word)(FORMAT1_RMD1+0x44)
#define TIME_STAMP1                     (word)(FORMAT1_RMD1+0x48)
#define PWRCALIB_ADDR1                  (word)(FORMAT1_RMD1+0x50)
#define RUN_OPC1                        (word)(FORMAT1_RMD1+0x54)

#define VENDOR_ID2                      (word)(FORMAT1_RMD1+0x80)
#define SERIAL_NUM2                     (word)(FORMAT1_RMD1+0xA0)
#define MODEL_NUM2                      (word)(FORMAT1_RMD1+0xB0)
#define WRITE_STRTGY2                   (word)(FORMAT1_RMD1+0xC0)
#define REC_POWER2                      (word)(FORMAT1_RMD1+0xC4)
#define TIME_STAMP2                     (word)(FORMAT1_RMD1+0xC8)
#define PWRCALIB_ADDR2                  (word)(FORMAT1_RMD1+0xD0)
#define RUN_OPC2                        (word)(FORMAT1_RMD1+0xD4)

#define VENDOR_ID3                      (word)(FORMAT1_RMD1+0x100)
#define SERIAL_NUM3                     (word)(FORMAT1_RMD1+0x120)
#define MODEL_NUM3                      (word)(FORMAT1_RMD1+0x130)
#define WRITE_STRTGY3                   (word)(FORMAT1_RMD1+0x140)
#define REC_POWER3                      (word)(FORMAT1_RMD1+0x144)
#define TIME_STAMP3                     (word)(FORMAT1_RMD1+0x148)
#define PWRCALIB_ADDR3                  (word)(FORMAT1_RMD1+0x150)
#define RUN_OPC3                        (word)(FORMAT1_RMD1+0x154)

#define VENDOR_ID4                      (word)(FORMAT1_RMD1+0x180)
#define SERIAL_NUM4                     (word)(FORMAT1_RMD1+0x1A0)
#define MODEL_NUM4                      (word)(FORMAT1_RMD1+0x1B0)
#define WRITE_STRTGY4                   (word)(FORMAT1_RMD1+0x1C0)
#define REC_POWER4                      (word)(FORMAT1_RMD1+0x1C4)
#define TIME_STAMP4                     (word)(FORMAT1_RMD1+0x1C8)
#define PWRCALIB_ADDR4                  (word)(FORMAT1_RMD1+0x1D0)
#define RUN_OPC4                        (word)(FORMAT1_RMD1+0x1D4)

/* RMD Field 3 Definitions */
#define BORDER1_SECNUM                  (word)(FORMAT1_RMD3+0x00)
#define BORDER2_SECNUM                  (word)(FORMAT1_RMD3+0x04)
#define BORDER3_SECNUM                  (word)(FORMAT1_RMD3+0x08)
#define BORDER4_SECNUM                  (word)(FORMAT1_RMD3+0x0C)
#define BORDER5_SECNUM                  (word)(FORMAT1_RMD3+0x10)
#define BORDER6_SECNUM                  (word)(FORMAT1_RMD3+0x14)
#define BORDER7_SECNUM                  (word)(FORMAT1_RMD3+0x18)
#define BORDER8_SECNUM                  (word)(FORMAT1_RMD3+0x1C)
#define BORDER9_SECNUM                  (word)(FORMAT1_RMD3+0x20)
#define BORDER10_SECNUM                 (word)(FORMAT1_RMD3+0x24)
#define BORDER11_SECNUM                 (word)(FORMAT1_RMD3+0x28)
#define BORDER12_SECNUM                 (word)(FORMAT1_RMD3+0x2C)
#define BORDER13_SECNUM                 (word)(FORMAT1_RMD3+0x30)
#define BORDER14_SECNUM                 (word)(FORMAT1_RMD3+0x34)
#define BORDER15_SECNUM                 (word)(FORMAT1_RMD3+0x38)
#define BORDER16_SECNUM                 (word)(FORMAT1_RMD3+0x3C)
#define BORDER17_SECNUM                 (word)(FORMAT1_RMD3+0x40)
#define BORDER18_SECNUM                 (word)(FORMAT1_RMD3+0x44)
#define BORDER19_SECNUM                 (word)(FORMAT1_RMD3+0x48)
#define BORDER20_SECNUM                 (word)(FORMAT1_RMD3+0x4C)
#define BORDER21_SECNUM                 (word)(FORMAT1_RMD3+0x50)
#define BORDER22_SECNUM                 (word)(FORMAT1_RMD3+0x54)
#define BORDER23_SECNUM                 (word)(FORMAT1_RMD3+0x58)
#define BORDER24_SECNUM                 (word)(FORMAT1_RMD3+0x5C)
#define BORDER25_SECNUM                 (word)(FORMAT1_RMD3+0x60)
#define BORDER26_SECNUM                 (word)(FORMAT1_RMD3+0x64)
#define BORDER27_SECNUM                 (word)(FORMAT1_RMD3+0x68)
#define BORDER28_SECNUM                 (word)(FORMAT1_RMD3+0x6C)
#define BORDER29_SECNUM                 (word)(FORMAT1_RMD3+0x70)
#define BORDER30_SECNUM                 (word)(FORMAT1_RMD3+0x74)
#define BORDER31_SECNUM                 (word)(FORMAT1_RMD3+0x78)
#define BORDER32_SECNUM                 (word)(FORMAT1_RMD3+0x7C)
#define BORDER33_SECNUM                 (word)(FORMAT1_RMD3+0x80)
#define BORDER34_SECNUM                 (word)(FORMAT1_RMD3+0x84)
#define BORDER35_SECNUM                 (word)(FORMAT1_RMD3+0x88)
#define BORDER36_SECNUM                 (word)(FORMAT1_RMD3+0x8C)
#define BORDER37_SECNUM                 (word)(FORMAT1_RMD3+0x90)
#define BORDER38_SECNUM                 (word)(FORMAT1_RMD3+0x94)
#define BORDER39_SECNUM                 (word)(FORMAT1_RMD3+0x98)
#define BORDER40_SECNUM                 (word)(FORMAT1_RMD3+0x9C)
#define BORDER41_SECNUM                 (word)(FORMAT1_RMD3+0xA0)
#define BORDER42_SECNUM                 (word)(FORMAT1_RMD3+0xA4)
#define BORDER43_SECNUM                 (word)(FORMAT1_RMD3+0xA8)
#define BORDER44_SECNUM                 (word)(FORMAT1_RMD3+0xAC)
#define BORDER45_SECNUM                 (word)(FORMAT1_RMD3+0xB0)
#define BORDER46_SECNUM                 (word)(FORMAT1_RMD3+0xB4)
#define BORDER47_SECNUM                 (word)(FORMAT1_RMD3+0xB8)
#define BORDER48_SECNUM                 (word)(FORMAT1_RMD3+0xBC)
#define BORDER49_SECNUM                 (word)(FORMAT1_RMD3+0xC0)
#define BORDER50_SECNUM                 (word)(FORMAT1_RMD3+0xC4)
#define BORDER51_SECNUM                 (word)(FORMAT1_RMD3+0xC8)
#define BORDER52_SECNUM                 (word)(FORMAT1_RMD3+0xCC)
#define BORDER53_SECNUM                 (word)(FORMAT1_RMD3+0xD0)
#define BORDER54_SECNUM                 (word)(FORMAT1_RMD3+0xD4)
#define BORDER55_SECNUM                 (word)(FORMAT1_RMD3+0xD8)
#define BORDER56_SECNUM                 (word)(FORMAT1_RMD3+0xDC)
#define BORDER57_SECNUM                 (word)(FORMAT1_RMD3+0xE0)
#define BORDER58_SECNUM                 (word)(FORMAT1_RMD3+0xE4)
#define BORDER59_SECNUM                 (word)(FORMAT1_RMD3+0xE8)
#define BORDER60_SECNUM                 (word)(FORMAT1_RMD3+0xEC)
#define BORDER61_SECNUM                 (word)(FORMAT1_RMD3+0xF0)
#define BORDER62_SECNUM                 (word)(FORMAT1_RMD3+0xF4)
#define BORDER63_SECNUM                 (word)(FORMAT1_RMD3+0xF8)
#define BORDER64_SECNUM                 (word)(FORMAT1_RMD3+0xFC)
#define BORDER65_SECNUM                 (word)(FORMAT1_RMD3+0x100)
#define BORDER66_SECNUM                 (word)(FORMAT1_RMD3+0x104)
#define BORDER67_SECNUM                 (word)(FORMAT1_RMD3+0x108)
#define BORDER68_SECNUM                 (word)(FORMAT1_RMD3+0x10C)
#define BORDER69_SECNUM                 (word)(FORMAT1_RMD3+0x110)
#define BORDER70_SECNUM                 (word)(FORMAT1_RMD3+0x114)
#define BORDER71_SECNUM                 (word)(FORMAT1_RMD3+0x118)
#define BORDER72_SECNUM                 (word)(FORMAT1_RMD3+0x11C)
#define BORDER73_SECNUM                 (word)(FORMAT1_RMD3+0x120)
#define BORDER74_SECNUM                 (word)(FORMAT1_RMD3+0x124)
#define BORDER75_SECNUM                 (word)(FORMAT1_RMD3+0x128)
#define BORDER76_SECNUM                 (word)(FORMAT1_RMD3+0x12C)
#define BORDER77_SECNUM                 (word)(FORMAT1_RMD3+0x130)
#define BORDER78_SECNUM                 (word)(FORMAT1_RMD3+0x134)
#define BORDER79_SECNUM                 (word)(FORMAT1_RMD3+0x138)
#define BORDER80_SECNUM                 (word)(FORMAT1_RMD3+0x13C)
#define BORDER81_SECNUM                 (word)(FORMAT1_RMD3+0x140)
#define BORDER82_SECNUM                 (word)(FORMAT1_RMD3+0x144)
#define BORDER83_SECNUM                 (word)(FORMAT1_RMD3+0x148)
#define BORDER84_SECNUM                 (word)(FORMAT1_RMD3+0x14C)
#define BORDER85_SECNUM                 (word)(FORMAT1_RMD3+0x150)
#define BORDER86_SECNUM                 (word)(FORMAT1_RMD3+0x154)
#define BORDER87_SECNUM                 (word)(FORMAT1_RMD3+0x158)
#define BORDER88_SECNUM                 (word)(FORMAT1_RMD3+0x15C)
#define BORDER89_SECNUM                 (word)(FORMAT1_RMD3+0x160)
#define BORDER90_SECNUM                 (word)(FORMAT1_RMD3+0x164)
#define BORDER91_SECNUM                 (word)(FORMAT1_RMD3+0x168)
#define BORDER92_SECNUM                 (word)(FORMAT1_RMD3+0x16C)
#define BORDER93_SECNUM                 (word)(FORMAT1_RMD3+0x170)
#define BORDER94_SECNUM                 (word)(FORMAT1_RMD3+0x174)
#define BORDER95_SECNUM                 (word)(FORMAT1_RMD3+0x178)
#define BORDER96_SECNUM                 (word)(FORMAT1_RMD3+0x17C)
#define BORDER97_SECNUM                 (word)(FORMAT1_RMD3+0x180)
#define BORDER98_SECNUM                 (word)(FORMAT1_RMD3+0x184)
#define BORDER99_SECNUM                 (word)(FORMAT1_RMD3+0x188)
#define BORDER100_SECNUM                (word)(FORMAT1_RMD3+0x18C)
#define BORDER101_SECNUM                (word)(FORMAT1_RMD3+0x190)
#define BORDER102_SECNUM                (word)(FORMAT1_RMD3+0x194)
#define BORDER103_SECNUM                (word)(FORMAT1_RMD3+0x198)
#define BORDER104_SECNUM                (word)(FORMAT1_RMD3+0x19C)
#define BORDER105_SECNUM                (word)(FORMAT1_RMD3+0x1A0)
#define BORDER106_SECNUM                (word)(FORMAT1_RMD3+0x1A4)
#define BORDER107_SECNUM                (word)(FORMAT1_RMD3+0x1A8)
#define BORDER108_SECNUM                (word)(FORMAT1_RMD3+0x1AC)
#define BORDER109_SECNUM                (word)(FORMAT1_RMD3+0x1B0)
#define BORDER110_SECNUM                (word)(FORMAT1_RMD3+0x1B4)
#define BORDER111_SECNUM                (word)(FORMAT1_RMD3+0x1B8)
#define BORDER112_SECNUM                (word)(FORMAT1_RMD3+0x1BC)
#define BORDER113_SECNUM                (word)(FORMAT1_RMD3+0x1C0)
#define BORDER114_SECNUM                (word)(FORMAT1_RMD3+0x1C4)
#define BORDER115_SECNUM                (word)(FORMAT1_RMD3+0x1C8)
#define BORDER116_SECNUM                (word)(FORMAT1_RMD3+0x1CC)
#define BORDER117_SECNUM                (word)(FORMAT1_RMD3+0x1D0)
#define BORDER118_SECNUM                (word)(FORMAT1_RMD3+0x1D4)
#define BORDER119_SECNUM                (word)(FORMAT1_RMD3+0x1D8)
#define BORDER120_SECNUM                (word)(FORMAT1_RMD3+0x1DC)
#define BORDER121_SECNUM                (word)(FORMAT1_RMD3+0x1E0)
#define BORDER122_SECNUM                (word)(FORMAT1_RMD3+0x1E4)
#define BORDER123_SECNUM                (word)(FORMAT1_RMD3+0x1E8)
#define BORDER124_SECNUM                (word)(FORMAT1_RMD3+0x1EC)
#define BORDER125_SECNUM                (word)(FORMAT1_RMD3+0x1F0)
#define BORDER126_SECNUM                (word)(FORMAT1_RMD3+0x1F4)
#define BORDER127_SECNUM                (word)(FORMAT1_RMD3+0x1F8)
#define BORDER128_SECNUM                (word)(FORMAT1_RMD3+0x1FC)
#define BORDER129_SECNUM                (word)(FORMAT1_RMD3+0x200)
#define BORDER130_SECNUM                (word)(FORMAT1_RMD3+0x204)
#define BORDER131_SECNUM                (word)(FORMAT1_RMD3+0x208)
#define BORDER132_SECNUM                (word)(FORMAT1_RMD3+0x20C)
#define BORDER133_SECNUM                (word)(FORMAT1_RMD3+0x210)
#define BORDER134_SECNUM                (word)(FORMAT1_RMD3+0x214)
#define BORDER135_SECNUM                (word)(FORMAT1_RMD3+0x218)
#define BORDER136_SECNUM                (word)(FORMAT1_RMD3+0x21C)
#define BORDER137_SECNUM                (word)(FORMAT1_RMD3+0x220)
#define BORDER138_SECNUM                (word)(FORMAT1_RMD3+0x224)
#define BORDER139_SECNUM                (word)(FORMAT1_RMD3+0x228)
#define BORDER140_SECNUM                (word)(FORMAT1_RMD3+0x22C)
#define BORDER141_SECNUM                (word)(FORMAT1_RMD3+0x230)
#define BORDER142_SECNUM                (word)(FORMAT1_RMD3+0x234)
#define BORDER143_SECNUM                (word)(FORMAT1_RMD3+0x238)
#define BORDER144_SECNUM                (word)(FORMAT1_RMD3+0x23C)
#define BORDER145_SECNUM                (word)(FORMAT1_RMD3+0x240)
#define BORDER146_SECNUM                (word)(FORMAT1_RMD3+0x244)
#define BORDER147_SECNUM                (word)(FORMAT1_RMD3+0x248)
#define BORDER148_SECNUM                (word)(FORMAT1_RMD3+0x24C)
#define BORDER149_SECNUM                (word)(FORMAT1_RMD3+0x250)
#define BORDER150_SECNUM                (word)(FORMAT1_RMD3+0x254)
#define BORDER151_SECNUM                (word)(FORMAT1_RMD3+0x258)
#define BORDER152_SECNUM                (word)(FORMAT1_RMD3+0x25C)
#define BORDER153_SECNUM                (word)(FORMAT1_RMD3+0x260)
#define BORDER154_SECNUM                (word)(FORMAT1_RMD3+0x264)
#define BORDER155_SECNUM                (word)(FORMAT1_RMD3+0x268)
#define BORDER156_SECNUM                (word)(FORMAT1_RMD3+0x26C)
#define BORDER157_SECNUM                (word)(FORMAT1_RMD3+0x270)
#define BORDER158_SECNUM                (word)(FORMAT1_RMD3+0x274)
#define BORDER159_SECNUM                (word)(FORMAT1_RMD3+0x278)
#define BORDER160_SECNUM                (word)(FORMAT1_RMD3+0x27C)
#define BORDER161_SECNUM                (word)(FORMAT1_RMD3+0x280)
#define BORDER162_SECNUM                (word)(FORMAT1_RMD3+0x284)
#define BORDER163_SECNUM                (word)(FORMAT1_RMD3+0x288)
#define BORDER164_SECNUM                (word)(FORMAT1_RMD3+0x28C)
#define BORDER165_SECNUM                (word)(FORMAT1_RMD3+0x290)
#define BORDER166_SECNUM                (word)(FORMAT1_RMD3+0x294)
#define BORDER167_SECNUM                (word)(FORMAT1_RMD3+0x298)
#define BORDER168_SECNUM                (word)(FORMAT1_RMD3+0x29C)
#define BORDER169_SECNUM                (word)(FORMAT1_RMD3+0x2A0)
#define BORDER170_SECNUM                (word)(FORMAT1_RMD3+0x2A4)
#define BORDER171_SECNUM                (word)(FORMAT1_RMD3+0x2A8)
#define BORDER172_SECNUM                (word)(FORMAT1_RMD3+0x2AC)
#define BORDER173_SECNUM                (word)(FORMAT1_RMD3+0x2B0)
#define BORDER174_SECNUM                (word)(FORMAT1_RMD3+0x2B4)
#define BORDER175_SECNUM                (word)(FORMAT1_RMD3+0x2B8)
#define BORDER176_SECNUM                (word)(FORMAT1_RMD3+0x2BC)
#define BORDER177_SECNUM                (word)(FORMAT1_RMD3+0x2C0)
#define BORDER178_SECNUM                (word)(FORMAT1_RMD3+0x2C4)
#define BORDER179_SECNUM                (word)(FORMAT1_RMD3+0x2C8)
#define BORDER180_SECNUM                (word)(FORMAT1_RMD3+0x2CC)
#define BORDER181_SECNUM                (word)(FORMAT1_RMD3+0x2D0)
#define BORDER182_SECNUM                (word)(FORMAT1_RMD3+0x2D4)
#define BORDER183_SECNUM                (word)(FORMAT1_RMD3+0x2D8)
#define BORDER184_SECNUM                (word)(FORMAT1_RMD3+0x2DC)
#define BORDER185_SECNUM                (word)(FORMAT1_RMD3+0x2E0)
#define BORDER186_SECNUM                (word)(FORMAT1_RMD3+0x2E4)
#define BORDER187_SECNUM                (word)(FORMAT1_RMD3+0x2E8)
#define BORDER188_SECNUM                (word)(FORMAT1_RMD3+0x2EC)
#define BORDER189_SECNUM                (word)(FORMAT1_RMD3+0x2F0)
#define BORDER190_SECNUM                (word)(FORMAT1_RMD3+0x2F4)
#define BORDER191_SECNUM                (word)(FORMAT1_RMD3+0x2F8)
#define BORDER192_SECNUM                (word)(FORMAT1_RMD3+0x2FC)
#define BORDER193_SECNUM                (word)(FORMAT1_RMD3+0x300)
#define BORDER194_SECNUM                (word)(FORMAT1_RMD3+0x304)
#define BORDER195_SECNUM                (word)(FORMAT1_RMD3+0x308)
#define BORDER196_SECNUM                (word)(FORMAT1_RMD3+0x30C)
#define BORDER197_SECNUM                (word)(FORMAT1_RMD3+0x310)
#define BORDER198_SECNUM                (word)(FORMAT1_RMD3+0x314)
#define BORDER199_SECNUM                (word)(FORMAT1_RMD3+0x318)

#define BORDER200_SECNUM                (word)(FORMAT1_RMD3+0x31C)
#define BORDER201_SECNUM                (word)(FORMAT1_RMD3+0x320)
#define BORDER202_SECNUM                (word)(FORMAT1_RMD3+0x324)
#define BORDER203_SECNUM                (word)(FORMAT1_RMD3+0x328)
#define BORDER204_SECNUM                (word)(FORMAT1_RMD3+0x32C)
#define BORDER205_SECNUM                (word)(FORMAT1_RMD3+0x330)
#define BORDER206_SECNUM                (word)(FORMAT1_RMD3+0x334)
#define BORDER207_SECNUM                (word)(FORMAT1_RMD3+0x338)
#define BORDER208_SECNUM                (word)(FORMAT1_RMD3+0x33C)
#define BORDER209_SECNUM                (word)(FORMAT1_RMD3+0x340)
#define BORDER210_SECNUM                (word)(FORMAT1_RMD3+0x344)
#define BORDER211_SECNUM                (word)(FORMAT1_RMD3+0x348)
#define BORDER212_SECNUM                (word)(FORMAT1_RMD3+0x34C)
#define BORDER213_SECNUM                (word)(FORMAT1_RMD3+0x350)
#define BORDER214_SECNUM                (word)(FORMAT1_RMD3+0x354)
#define BORDER215_SECNUM                (word)(FORMAT1_RMD3+0x358)
#define BORDER216_SECNUM                (word)(FORMAT1_RMD3+0x35C)
#define BORDER217_SECNUM                (word)(FORMAT1_RMD3+0x360)
#define BORDER218_SECNUM                (word)(FORMAT1_RMD3+0x364)
#define BORDER219_SECNUM                (word)(FORMAT1_RMD3+0x368)
#define BORDER220_SECNUM                (word)(FORMAT1_RMD3+0x36C)
#define BORDER221_SECNUM                (word)(FORMAT1_RMD3+0x370)
#define BORDER222_SECNUM                (word)(FORMAT1_RMD3+0x374)
#define BORDER223_SECNUM                (word)(FORMAT1_RMD3+0x378)
#define BORDER224_SECNUM                (word)(FORMAT1_RMD3+0x37C)
#define BORDER225_SECNUM                (word)(FORMAT1_RMD3+0x380)
#define BORDER226_SECNUM                (word)(FORMAT1_RMD3+0x384)
#define BORDER227_SECNUM                (word)(FORMAT1_RMD3+0x388)
#define BORDER228_SECNUM                (word)(FORMAT1_RMD3+0x38C)
#define BORDER229_SECNUM                (word)(FORMAT1_RMD3+0x390)
#define BORDER230_SECNUM                (word)(FORMAT1_RMD3+0x394)
#define BORDER231_SECNUM                (word)(FORMAT1_RMD3+0x398)
#define BORDER232_SECNUM                (word)(FORMAT1_RMD3+0x39C)
#define BORDER233_SECNUM                (word)(FORMAT1_RMD3+0x3A0)
#define BORDER234_SECNUM                (word)(FORMAT1_RMD3+0x3A4)
#define BORDER235_SECNUM                (word)(FORMAT1_RMD3+0x3A8)
#define BORDER236_SECNUM                (word)(FORMAT1_RMD3+0x3AC)
#define BORDER237_SECNUM                (word)(FORMAT1_RMD3+0x3B0)
#define BORDER238_SECNUM                (word)(FORMAT1_RMD3+0x3B4)
#define BORDER239_SECNUM                (word)(FORMAT1_RMD3+0x3B8)
#define BORDER240_SECNUM                (word)(FORMAT1_RMD3+0x3BC)
#define BORDER241_SECNUM                (word)(FORMAT1_RMD3+0x3C0)
#define BORDER242_SECNUM                (word)(FORMAT1_RMD3+0x3C4)
#define BORDER243_SECNUM                (word)(FORMAT1_RMD3+0x3C8)
#define BORDER244_SECNUM                (word)(FORMAT1_RMD3+0x3CC)
#define BORDER245_SECNUM                (word)(FORMAT1_RMD3+0x3D0)
#define BORDER246_SECNUM                (word)(FORMAT1_RMD3+0x3D4)
#define BORDER247_SECNUM                (word)(FORMAT1_RMD3+0x3D8)
#define BORDER248_SECNUM                (word)(FORMAT1_RMD3+0x3DC)
#define BORDER249_SECNUM                (word)(FORMAT1_RMD3+0x3E0)
#define BORDER250_SECNUM                (word)(FORMAT1_RMD3+0x3E4)
#define BORDER251_SECNUM                (word)(FORMAT1_RMD3+0x3E8)
#define BORDER252_SECNUM                (word)(FORMAT1_RMD3+0x3EC)
#define BORDER253_SECNUM                (word)(FORMAT1_RMD3+0x3F0)
#define BORDER254_SECNUM                (word)(FORMAT1_RMD3+0x3F4)
#define BORDER255_SECNUM                (word)(FORMAT1_RMD3+0x3F8)
#define BORDER256_SECNUM                (word)(FORMAT1_RMD3+0x3FC)
#define BORDER257_SECNUM                (word)(FORMAT1_RMD3+0x400)
#define BORDER258_SECNUM                (word)(FORMAT1_RMD3+0x404)
#define BORDER259_SECNUM                (word)(FORMAT1_RMD3+0x408)
#define BORDER260_SECNUM                (word)(FORMAT1_RMD3+0x40C)
#define BORDER261_SECNUM                (word)(FORMAT1_RMD3+0x410)
#define BORDER262_SECNUM                (word)(FORMAT1_RMD3+0x414)
#define BORDER263_SECNUM                (word)(FORMAT1_RMD3+0x418)
#define BORDER264_SECNUM                (word)(FORMAT1_RMD3+0x41C)
#define BORDER265_SECNUM                (word)(FORMAT1_RMD3+0x420)
#define BORDER266_SECNUM                (word)(FORMAT1_RMD3+0x424)
#define BORDER267_SECNUM                (word)(FORMAT1_RMD3+0x428)
#define BORDER268_SECNUM                (word)(FORMAT1_RMD3+0x42C)
#define BORDER269_SECNUM                (word)(FORMAT1_RMD3+0x430)
#define BORDER270_SECNUM                (word)(FORMAT1_RMD3+0x434)
#define BORDER271_SECNUM                (word)(FORMAT1_RMD3+0x438)
#define BORDER272_SECNUM                (word)(FORMAT1_RMD3+0x43C)
#define BORDER273_SECNUM                (word)(FORMAT1_RMD3+0x440)
#define BORDER274_SECNUM                (word)(FORMAT1_RMD3+0x444)
#define BORDER275_SECNUM                (word)(FORMAT1_RMD3+0x448)
#define BORDER276_SECNUM                (word)(FORMAT1_RMD3+0x44C)
#define BORDER277_SECNUM                (word)(FORMAT1_RMD3+0x450)
#define BORDER278_SECNUM                (word)(FORMAT1_RMD3+0x454)
#define BORDER279_SECNUM                (word)(FORMAT1_RMD3+0x458)
#define BORDER280_SECNUM                (word)(FORMAT1_RMD3+0x45C)
#define BORDER281_SECNUM                (word)(FORMAT1_RMD3+0x460)
#define BORDER282_SECNUM                (word)(FORMAT1_RMD3+0x464)
#define BORDER283_SECNUM                (word)(FORMAT1_RMD3+0x468)
#define BORDER284_SECNUM                (word)(FORMAT1_RMD3+0x46C)
#define BORDER285_SECNUM                (word)(FORMAT1_RMD3+0x470)
#define BORDER286_SECNUM                (word)(FORMAT1_RMD3+0x474)
#define BORDER287_SECNUM                (word)(FORMAT1_RMD3+0x478)
#define BORDER288_SECNUM                (word)(FORMAT1_RMD3+0x47C)
#define BORDER289_SECNUM                (word)(FORMAT1_RMD3+0x480)
#define BORDER290_SECNUM                (word)(FORMAT1_RMD3+0x484)
#define BORDER291_SECNUM                (word)(FORMAT1_RMD3+0x488)
#define BORDER292_SECNUM                (word)(FORMAT1_RMD3+0x48C)
#define BORDER293_SECNUM                (word)(FORMAT1_RMD3+0x490)
#define BORDER294_SECNUM                (word)(FORMAT1_RMD3+0x494)
#define BORDER295_SECNUM                (word)(FORMAT1_RMD3+0x498)
#define BORDER296_SECNUM                (word)(FORMAT1_RMD3+0x49C)
#define BORDER297_SECNUM                (word)(FORMAT1_RMD3+0x4A0)
#define BORDER298_SECNUM                (word)(FORMAT1_RMD3+0x4A4)
#define BORDER299_SECNUM                (word)(FORMAT1_RMD3+0x4A8)

#define BORDER300_SECNUM                (word)(FORMAT1_RMD3+0x4AC)
#define BORDER301_SECNUM                (word)(FORMAT1_RMD3+0x4B0)
#define BORDER302_SECNUM                (word)(FORMAT1_RMD3+0x4B4)
#define BORDER303_SECNUM                (word)(FORMAT1_RMD3+0x4B8)
#define BORDER304_SECNUM                (word)(FORMAT1_RMD3+0x4BC)
#define BORDER305_SECNUM                (word)(FORMAT1_RMD3+0x4C0)
#define BORDER306_SECNUM                (word)(FORMAT1_RMD3+0x4C4)
#define BORDER307_SECNUM                (word)(FORMAT1_RMD3+0x4C8)
#define BORDER308_SECNUM                (word)(FORMAT1_RMD3+0x4CC)
#define BORDER309_SECNUM                (word)(FORMAT1_RMD3+0x4D0)
#define BORDER310_SECNUM                (word)(FORMAT1_RMD3+0x4D4)
#define BORDER311_SECNUM                (word)(FORMAT1_RMD3+0x4D8)
#define BORDER312_SECNUM                (word)(FORMAT1_RMD3+0x4DC)
#define BORDER313_SECNUM                (word)(FORMAT1_RMD3+0x4E0)
#define BORDER314_SECNUM                (word)(FORMAT1_RMD3+0x4E4)
#define BORDER315_SECNUM                (word)(FORMAT1_RMD3+0x4E8)
#define BORDER316_SECNUM                (word)(FORMAT1_RMD3+0x4EC)
#define BORDER317_SECNUM                (word)(FORMAT1_RMD3+0x4F0)
#define BORDER318_SECNUM                (word)(FORMAT1_RMD3+0x4F4)
#define BORDER319_SECNUM                (word)(FORMAT1_RMD3+0x4F8)
#define BORDER320_SECNUM                (word)(FORMAT1_RMD3+0x4FC)
#define BORDER321_SECNUM                (word)(FORMAT1_RMD3+0x500)
#define BORDER322_SECNUM                (word)(FORMAT1_RMD3+0x504)
#define BORDER323_SECNUM                (word)(FORMAT1_RMD3+0x508)
#define BORDER324_SECNUM                (word)(FORMAT1_RMD3+0x50C)
#define BORDER325_SECNUM                (word)(FORMAT1_RMD3+0x510)
#define BORDER326_SECNUM                (word)(FORMAT1_RMD3+0x514)
#define BORDER327_SECNUM                (word)(FORMAT1_RMD3+0x518)
#define BORDER328_SECNUM                (word)(FORMAT1_RMD3+0x51C)
#define BORDER329_SECNUM                (word)(FORMAT1_RMD3+0x520)
#define BORDER330_SECNUM                (word)(FORMAT1_RMD3+0x524)
#define BORDER331_SECNUM                (word)(FORMAT1_RMD3+0x528)
#define BORDER332_SECNUM                (word)(FORMAT1_RMD3+0x52C)
#define BORDER333_SECNUM                (word)(FORMAT1_RMD3+0x530)
#define BORDER334_SECNUM                (word)(FORMAT1_RMD3+0x534)
#define BORDER335_SECNUM                (word)(FORMAT1_RMD3+0x538)
#define BORDER336_SECNUM                (word)(FORMAT1_RMD3+0x53C)
#define BORDER337_SECNUM                (word)(FORMAT1_RMD3+0x540)
#define BORDER338_SECNUM                (word)(FORMAT1_RMD3+0x544)
#define BORDER339_SECNUM                (word)(FORMAT1_RMD3+0x548)
#define BORDER340_SECNUM                (word)(FORMAT1_RMD3+0x54C)
#define BORDER341_SECNUM                (word)(FORMAT1_RMD3+0x550)
#define BORDER342_SECNUM                (word)(FORMAT1_RMD3+0x554)
#define BORDER343_SECNUM                (word)(FORMAT1_RMD3+0x558)
#define BORDER344_SECNUM                (word)(FORMAT1_RMD3+0x55C)
#define BORDER345_SECNUM                (word)(FORMAT1_RMD3+0x560)
#define BORDER346_SECNUM                (word)(FORMAT1_RMD3+0x564)
#define BORDER347_SECNUM                (word)(FORMAT1_RMD3+0x568)
#define BORDER348_SECNUM                (word)(FORMAT1_RMD3+0x56C)
#define BORDER349_SECNUM                (word)(FORMAT1_RMD3+0x570)
#define BORDER350_SECNUM                (word)(FORMAT1_RMD3+0x574)
#define BORDER351_SECNUM                (word)(FORMAT1_RMD3+0x578)
#define BORDER352_SECNUM                (word)(FORMAT1_RMD3+0x57C)
#define BORDER353_SECNUM                (word)(FORMAT1_RMD3+0x580)
#define BORDER354_SECNUM                (word)(FORMAT1_RMD3+0x584)
#define BORDER355_SECNUM                (word)(FORMAT1_RMD3+0x588)
#define BORDER356_SECNUM                (word)(FORMAT1_RMD3+0x58C)
#define BORDER357_SECNUM                (word)(FORMAT1_RMD3+0x590)
#define BORDER358_SECNUM                (word)(FORMAT1_RMD3+0x594)
#define BORDER359_SECNUM                (word)(FORMAT1_RMD3+0x598)
#define BORDER360_SECNUM                (word)(FORMAT1_RMD3+0x59C)
#define BORDER361_SECNUM                (word)(FORMAT1_RMD3+0x5A0)
#define BORDER362_SECNUM                (word)(FORMAT1_RMD3+0x5A4)
#define BORDER363_SECNUM                (word)(FORMAT1_RMD3+0x5A8)
#define BORDER364_SECNUM                (word)(FORMAT1_RMD3+0x5AC)
#define BORDER365_SECNUM                (word)(FORMAT1_RMD3+0x5B0)
#define BORDER366_SECNUM                (word)(FORMAT1_RMD3+0x5B4)
#define BORDER367_SECNUM                (word)(FORMAT1_RMD3+0x5B8)
#define BORDER368_SECNUM                (word)(FORMAT1_RMD3+0x5BC)
#define BORDER369_SECNUM                (word)(FORMAT1_RMD3+0x5C0)
#define BORDER370_SECNUM                (word)(FORMAT1_RMD3+0x5C4)
#define BORDER371_SECNUM                (word)(FORMAT1_RMD3+0x5C8)
#define BORDER372_SECNUM                (word)(FORMAT1_RMD3+0x5CC)
#define BORDER373_SECNUM                (word)(FORMAT1_RMD3+0x5D0)
#define BORDER374_SECNUM                (word)(FORMAT1_RMD3+0x5D4)
#define BORDER375_SECNUM                (word)(FORMAT1_RMD3+0x5D8)
#define BORDER376_SECNUM                (word)(FORMAT1_RMD3+0x5DC)
#define BORDER377_SECNUM                (word)(FORMAT1_RMD3+0x5E0)
#define BORDER378_SECNUM                (word)(FORMAT1_RMD3+0x5E4)
#define BORDER379_SECNUM                (word)(FORMAT1_RMD3+0x5E8)
#define BORDER380_SECNUM                (word)(FORMAT1_RMD3+0x5EC)
#define BORDER381_SECNUM                (word)(FORMAT1_RMD3+0x5F0)
#define BORDER382_SECNUM                (word)(FORMAT1_RMD3+0x5F4)
#define BORDER383_SECNUM                (word)(FORMAT1_RMD3+0x5F8)
#define BORDER384_SECNUM                (word)(FORMAT1_RMD3+0x5FC)
#define BORDER385_SECNUM                (word)(FORMAT1_RMD3+0x600)
#define BORDER386_SECNUM                (word)(FORMAT1_RMD3+0x604)
#define BORDER387_SECNUM                (word)(FORMAT1_RMD3+0x608)
#define BORDER388_SECNUM                (word)(FORMAT1_RMD3+0x60C)
#define BORDER389_SECNUM                (word)(FORMAT1_RMD3+0x610)
#define BORDER390_SECNUM                (word)(FORMAT1_RMD3+0x614)
#define BORDER391_SECNUM                (word)(FORMAT1_RMD3+0x618)
#define BORDER392_SECNUM                (word)(FORMAT1_RMD3+0x61C)
#define BORDER393_SECNUM                (word)(FORMAT1_RMD3+0x620)
#define BORDER394_SECNUM                (word)(FORMAT1_RMD3+0x624)
#define BORDER395_SECNUM                (word)(FORMAT1_RMD3+0x628)
#define BORDER396_SECNUM                (word)(FORMAT1_RMD3+0x62C)
#define BORDER397_SECNUM                (word)(FORMAT1_RMD3+0x630)
#define BORDER398_SECNUM                (word)(FORMAT1_RMD3+0x634)
#define BORDER399_SECNUM                (word)(FORMAT1_RMD3+0x638)

#define BORDER400_SECNUM                (word)(FORMAT1_RMD3+0x63C)
#define BORDER401_SECNUM                (word)(FORMAT1_RMD3+0x640)
#define BORDER402_SECNUM                (word)(FORMAT1_RMD3+0x644)
#define BORDER403_SECNUM                (word)(FORMAT1_RMD3+0x648)
#define BORDER404_SECNUM                (word)(FORMAT1_RMD3+0x64C)
#define BORDER405_SECNUM                (word)(FORMAT1_RMD3+0x650)
#define BORDER406_SECNUM                (word)(FORMAT1_RMD3+0x654)
#define BORDER407_SECNUM                (word)(FORMAT1_RMD3+0x658)
#define BORDER408_SECNUM                (word)(FORMAT1_RMD3+0x65C)
#define BORDER409_SECNUM                (word)(FORMAT1_RMD3+0x660)
#define BORDER410_SECNUM                (word)(FORMAT1_RMD3+0x664)
#define BORDER411_SECNUM                (word)(FORMAT1_RMD3+0x668)
#define BORDER412_SECNUM                (word)(FORMAT1_RMD3+0x66C)
#define BORDER413_SECNUM                (word)(FORMAT1_RMD3+0x670)
#define BORDER414_SECNUM                (word)(FORMAT1_RMD3+0x674)
#define BORDER415_SECNUM                (word)(FORMAT1_RMD3+0x678)
#define BORDER416_SECNUM                (word)(FORMAT1_RMD3+0x67C)
#define BORDER417_SECNUM                (word)(FORMAT1_RMD3+0x680)
#define BORDER418_SECNUM                (word)(FORMAT1_RMD3+0x684)
#define BORDER419_SECNUM                (word)(FORMAT1_RMD3+0x688)
#define BORDER420_SECNUM                (word)(FORMAT1_RMD3+0x68C)
#define BORDER421_SECNUM                (word)(FORMAT1_RMD3+0x690)
#define BORDER422_SECNUM                (word)(FORMAT1_RMD3+0x694)
#define BORDER423_SECNUM                (word)(FORMAT1_RMD3+0x698)
#define BORDER424_SECNUM                (word)(FORMAT1_RMD3+0x69C)
#define BORDER425_SECNUM                (word)(FORMAT1_RMD3+0x6A0)
#define BORDER426_SECNUM                (word)(FORMAT1_RMD3+0x6A4)
#define BORDER427_SECNUM                (word)(FORMAT1_RMD3+0x6A8)
#define BORDER428_SECNUM                (word)(FORMAT1_RMD3+0x6AC)
#define BORDER429_SECNUM                (word)(FORMAT1_RMD3+0x6B0)
#define BORDER430_SECNUM                (word)(FORMAT1_RMD3+0x6B4)
#define BORDER431_SECNUM                (word)(FORMAT1_RMD3+0x6B8)
#define BORDER432_SECNUM                (word)(FORMAT1_RMD3+0x6BC)
#define BORDER433_SECNUM                (word)(FORMAT1_RMD3+0x6C0)
#define BORDER434_SECNUM                (word)(FORMAT1_RMD3+0x6C4)
#define BORDER435_SECNUM                (word)(FORMAT1_RMD3+0x6C8)
#define BORDER436_SECNUM                (word)(FORMAT1_RMD3+0x6CC)
#define BORDER437_SECNUM                (word)(FORMAT1_RMD3+0x6D0)
#define BORDER438_SECNUM                (word)(FORMAT1_RMD3+0x6D4)
#define BORDER439_SECNUM                (word)(FORMAT1_RMD3+0x6D8)
#define BORDER440_SECNUM                (word)(FORMAT1_RMD3+0x6DC)
#define BORDER441_SECNUM                (word)(FORMAT1_RMD3+0x6E0)
#define BORDER442_SECNUM                (word)(FORMAT1_RMD3+0x6E4)
#define BORDER443_SECNUM                (word)(FORMAT1_RMD3+0x6E8)
#define BORDER444_SECNUM                (word)(FORMAT1_RMD3+0x6EC)
#define BORDER445_SECNUM                (word)(FORMAT1_RMD3+0x6F0)
#define BORDER446_SECNUM                (word)(FORMAT1_RMD3+0x6F4)
#define BORDER447_SECNUM                (word)(FORMAT1_RMD3+0x6F8)
#define BORDER448_SECNUM                (word)(FORMAT1_RMD3+0x6FC)
#define BORDER449_SECNUM                (word)(FORMAT1_RMD3+0x700)
#define BORDER450_SECNUM                (word)(FORMAT1_RMD3+0x704)
#define BORDER451_SECNUM                (word)(FORMAT1_RMD3+0x708)
#define BORDER452_SECNUM                (word)(FORMAT1_RMD3+0x70C)
#define BORDER453_SECNUM                (word)(FORMAT1_RMD3+0x710)
#define BORDER454_SECNUM                (word)(FORMAT1_RMD3+0x714)
#define BORDER455_SECNUM                (word)(FORMAT1_RMD3+0x718)
#define BORDER456_SECNUM                (word)(FORMAT1_RMD3+0x71C)
#define BORDER457_SECNUM                (word)(FORMAT1_RMD3+0x720)
#define BORDER458_SECNUM                (word)(FORMAT1_RMD3+0x724)
#define BORDER459_SECNUM                (word)(FORMAT1_RMD3+0x728)
#define BORDER460_SECNUM                (word)(FORMAT1_RMD3+0x72C)
#define BORDER461_SECNUM                (word)(FORMAT1_RMD3+0x730)
#define BORDER462_SECNUM                (word)(FORMAT1_RMD3+0x734)
#define BORDER463_SECNUM                (word)(FORMAT1_RMD3+0x738)
#define BORDER464_SECNUM                (word)(FORMAT1_RMD3+0x73C)
#define BORDER465_SECNUM                (word)(FORMAT1_RMD3+0x740)
#define BORDER466_SECNUM                (word)(FORMAT1_RMD3+0x744)
#define BORDER467_SECNUM                (word)(FORMAT1_RMD3+0x748)
#define BORDER468_SECNUM                (word)(FORMAT1_RMD3+0x74C)
#define BORDER469_SECNUM                (word)(FORMAT1_RMD3+0x750)
#define BORDER470_SECNUM                (word)(FORMAT1_RMD3+0x754)
#define BORDER471_SECNUM                (word)(FORMAT1_RMD3+0x758)
#define BORDER472_SECNUM                (word)(FORMAT1_RMD3+0x75C)
#define BORDER473_SECNUM                (word)(FORMAT1_RMD3+0x760)
#define BORDER474_SECNUM                (word)(FORMAT1_RMD3+0x764)
#define BORDER475_SECNUM                (word)(FORMAT1_RMD3+0x768)
#define BORDER476_SECNUM                (word)(FORMAT1_RMD3+0x76C)
#define BORDER477_SECNUM                (word)(FORMAT1_RMD3+0x770)
#define BORDER478_SECNUM                (word)(FORMAT1_RMD3+0x774)
#define BORDER479_SECNUM                (word)(FORMAT1_RMD3+0x778)
#define BORDER480_SECNUM                (word)(FORMAT1_RMD3+0x77C)
#define BORDER481_SECNUM                (word)(FORMAT1_RMD3+0x780)
#define BORDER482_SECNUM                (word)(FORMAT1_RMD3+0x784)
#define BORDER483_SECNUM                (word)(FORMAT1_RMD3+0x788)
#define BORDER484_SECNUM                (word)(FORMAT1_RMD3+0x78C)
#define BORDER485_SECNUM                (word)(FORMAT1_RMD3+0x790)
#define BORDER486_SECNUM                (word)(FORMAT1_RMD3+0x794)
#define BORDER487_SECNUM                (word)(FORMAT1_RMD3+0x798)
#define BORDER488_SECNUM                (word)(FORMAT1_RMD3+0x79C)
#define BORDER489_SECNUM                (word)(FORMAT1_RMD3+0x7A0)
#define BORDER490_SECNUM                (word)(FORMAT1_RMD3+0x7A4)
#define BORDER491_SECNUM                (word)(FORMAT1_RMD3+0x7A8)
#define BORDER492_SECNUM                (word)(FORMAT1_RMD3+0x7AC)
#define BORDER493_SECNUM                (word)(FORMAT1_RMD3+0x7B0)
#define BORDER494_SECNUM                (word)(FORMAT1_RMD3+0x7B4)
#define BORDER495_SECNUM                (word)(FORMAT1_RMD3+0x7B8)
#define BORDER496_SECNUM                (word)(FORMAT1_RMD3+0x7BC)
#define BORDER497_SECNUM                (word)(FORMAT1_RMD3+0x7C0)
#define BORDER498_SECNUM                (word)(FORMAT1_RMD3+0x7C4)
#define BORDER499_SECNUM                (word)(FORMAT1_RMD3+0x7C8)

#define BORDER500_SECNUM                (word)(FORMAT1_RMD3+0x7CC)
#define BORDER501_SECNUM                (word)(FORMAT1_RMD3+0x7D0)
#define BORDER502_SECNUM                (word)(FORMAT1_RMD3+0x7D4)
#define BORDER503_SECNUM                (word)(FORMAT1_RMD3+0x7D8)
#define BORDER504_SECNUM                (word)(FORMAT1_RMD3+0x7DC)
#define BORDER505_SECNUM                (word)(FORMAT1_RMD3+0x7E0)
#define BORDER506_SECNUM                (word)(FORMAT1_RMD3+0x7E4)
#define BORDER507_SECNUM                (word)(FORMAT1_RMD3+0x7E8)
#define BORDER508_SECNUM                (word)(FORMAT1_RMD3+0x7EC)
#define BORDER509_SECNUM                (word)(FORMAT1_RMD3+0x7F0)
#define BORDER510_SECNUM                (word)(FORMAT1_RMD3+0x7F4)
#define BORDER511_SECNUM                (word)(FORMAT1_RMD3+0x7F8)
#define BORDER512_SECNUM                (word)(FORMAT1_RMD3+0x7FC)

/* RMD Field 4 Definitions */
#define INVISIBLE_RZONE                 (word)(FORMAT1_RMD4+0x00)
#define CURRENT_RZONE1                  (word)(FORMAT1_RMD4+0x02)
#define CURRENT_RZONE2                  (word)(FORMAT1_RMD4+0x04)
#define RZONE1_STARTSEC                 (word)(FORMAT1_RMD4+0x10)
#define RZONE1_LRA                      (word)(FORMAT1_RMD4+0x14)
#define RZONE2_STARTSEC                 (word)(FORMAT1_RMD4+0x18)
#define RZONE2_LRA                      (word)(FORMAT1_RMD4+0x1C)
#define RZONE3_STARTSEC                 (word)(FORMAT1_RMD4+0x20)
#define RZONE3_LRA                      (word)(FORMAT1_RMD4+0x24)
#define RZONE4_STARTSEC                 (word)(FORMAT1_RMD4+0x28)
#define RZONE4_LRA                      (word)(FORMAT1_RMD4+0x2C)
#define RZONE5_STARTSEC                 (word)(FORMAT1_RMD4+0x30)
#define RZONE5_LRA                      (word)(FORMAT1_RMD4+0x34)
#define RZONE6_STARTSEC                 (word)(FORMAT1_RMD4+0x38)
#define RZONE6_LRA                      (word)(FORMAT1_RMD4+0x3C)
#define RZONE7_STARTSEC                 (word)(FORMAT1_RMD4+0x40)
#define RZONE7_LRA                      (word)(FORMAT1_RMD4+0x44)
#define RZONE8_STARTSEC                 (word)(FORMAT1_RMD4+0x48)
#define RZONE8_LRA                      (word)(FORMAT1_RMD4+0x4C)
#define RZONE9_STARTSEC                 (word)(FORMAT1_RMD4+0x50)
#define RZONE9_LRA                      (word)(FORMAT1_RMD4+0x54)
#define RZONE10_STARTSEC                (word)(FORMAT1_RMD4+0x58)
#define RZONE10_LRA                     (word)(FORMAT1_RMD4+0x5C)
#define RZONE11_STARTSEC                (word)(FORMAT1_RMD4+0x60)
#define RZONE11_LRA                     (word)(FORMAT1_RMD4+0x64)
#define RZONE12_STARTSEC                (word)(FORMAT1_RMD4+0x68)
#define RZONE12_LRA                     (word)(FORMAT1_RMD4+0x6C)
#define RZONE13_STARTSEC                (word)(FORMAT1_RMD4+0x70)
#define RZONE13_LRA                     (word)(FORMAT1_RMD4+0x74)
#define RZONE14_STARTSEC                (word)(FORMAT1_RMD4+0x78)
#define RZONE14_LRA                     (word)(FORMAT1_RMD4+0x7C)
#define RZONE15_STARTSEC                (word)(FORMAT1_RMD4+0x80)
#define RZONE15_LRA                     (word)(FORMAT1_RMD4+0x84)
#define RZONE16_STARTSEC                (word)(FORMAT1_RMD4+0x88)
#define RZONE16_LRA                     (word)(FORMAT1_RMD4+0x8C)
#define RZONE17_STARTSEC                (word)(FORMAT1_RMD4+0x90)
#define RZONE17_LRA                     (word)(FORMAT1_RMD4+0x94)
#define RZONE18_STARTSEC                (word)(FORMAT1_RMD4+0x98)
#define RZONE18_LRA                     (word)(FORMAT1_RMD4+0x9C)
#define RZONE19_STARTSEC                (word)(FORMAT1_RMD4+0xA0)
#define RZONE19_LRA                     (word)(FORMAT1_RMD4+0xA4)
#define RZONE20_STARTSEC                (word)(FORMAT1_RMD4+0xA8)
#define RZONE20_LRA                     (word)(FORMAT1_RMD4+0xAC)
#define RZONE21_STARTSEC                (word)(FORMAT1_RMD4+0xB0)
#define RZONE21_LRA                     (word)(FORMAT1_RMD4+0xB4)
#define RZONE22_STARTSEC                (word)(FORMAT1_RMD4+0xB8)
#define RZONE22_LRA                     (word)(FORMAT1_RMD4+0xBC)
#define RZONE23_STARTSEC                (word)(FORMAT1_RMD4+0xC0)
#define RZONE23_LRA                     (word)(FORMAT1_RMD4+0xC4)
#define RZONE24_STARTSEC                (word)(FORMAT1_RMD4+0xC8)
#define RZONE24_LRA                     (word)(FORMAT1_RMD4+0xCC)
#define RZONE25_STARTSEC                (word)(FORMAT1_RMD4+0xD0)
#define RZONE25_LRA                     (word)(FORMAT1_RMD4+0xD4)
#define RZONE26_STARTSEC                (word)(FORMAT1_RMD4+0xD8)
#define RZONE26_LRA                     (word)(FORMAT1_RMD4+0xDC)
#define RZONE27_STARTSEC                (word)(FORMAT1_RMD4+0xE0)
#define RZONE27_LRA                     (word)(FORMAT1_RMD4+0xE4)
#define RZONE28_STARTSEC                (word)(FORMAT1_RMD4+0xE8)
#define RZONE28_LRA                     (word)(FORMAT1_RMD4+0xEC)
#define RZONE29_STARTSEC                (word)(FORMAT1_RMD4+0xF0)
#define RZONE29_LRA                     (word)(FORMAT1_RMD4+0xF4)
#define RZONE30_STARTSEC                (word)(FORMAT1_RMD4+0xF8)
#define RZONE30_LRA                     (word)(FORMAT1_RMD4+0xFC)
#define RZONE31_STARTSEC                (word)(FORMAT1_RMD4+0x100)
#define RZONE31_LRA                     (word)(FORMAT1_RMD4+0x104)
#define RZONE32_STARTSEC                (word)(FORMAT1_RMD4+0x108)
#define RZONE32_LRA                     (word)(FORMAT1_RMD4+0x10C)
#define RZONE33_STARTSEC                (word)(FORMAT1_RMD4+0x110)
#define RZONE33_LRA                     (word)(FORMAT1_RMD4+0x114)
#define RZONE34_STARTSEC                (word)(FORMAT1_RMD4+0x118)
#define RZONE34_LRA                     (word)(FORMAT1_RMD4+0x11C)
#define RZONE35_STARTSEC                (word)(FORMAT1_RMD4+0x120)
#define RZONE35_LRA                     (word)(FORMAT1_RMD4+0x124)
#define RZONE36_STARTSEC                (word)(FORMAT1_RMD4+0x128)
#define RZONE36_LRA                     (word)(FORMAT1_RMD4+0x12C)
#define RZONE37_STARTSEC                (word)(FORMAT1_RMD4+0x130)
#define RZONE37_LRA                     (word)(FORMAT1_RMD4+0x134)
#define RZONE38_STARTSEC                (word)(FORMAT1_RMD4+0x138)
#define RZONE38_LRA                     (word)(FORMAT1_RMD4+0x13C)
#define RZONE39_STARTSEC                (word)(FORMAT1_RMD4+0x140)
#define RZONE39_LRA                     (word)(FORMAT1_RMD4+0x144)
#define RZONE40_STARTSEC                (word)(FORMAT1_RMD4+0x148)
#define RZONE40_LRA                     (word)(FORMAT1_RMD4+0x14C)
#define RZONE41_STARTSEC                (word)(FORMAT1_RMD4+0x150)
#define RZONE41_LRA                     (word)(FORMAT1_RMD4+0x154)
#define RZONE42_STARTSEC                (word)(FORMAT1_RMD4+0x158)
#define RZONE42_LRA                     (word)(FORMAT1_RMD4+0x15C)
#define RZONE43_STARTSEC                (word)(FORMAT1_RMD4+0x160)
#define RZONE43_LRA                     (word)(FORMAT1_RMD4+0x164)
#define RZONE44_STARTSEC                (word)(FORMAT1_RMD4+0x168)
#define RZONE44_LRA                     (word)(FORMAT1_RMD4+0x16C)
#define RZONE45_STARTSEC                (word)(FORMAT1_RMD4+0x170)
#define RZONE45_LRA                     (word)(FORMAT1_RMD4+0x174)
#define RZONE46_STARTSEC                (word)(FORMAT1_RMD4+0x178)
#define RZONE46_LRA                     (word)(FORMAT1_RMD4+0x17C)
#define RZONE47_STARTSEC                (word)(FORMAT1_RMD4+0x180)
#define RZONE47_LRA                     (word)(FORMAT1_RMD4+0x184)
#define RZONE48_STARTSEC                (word)(FORMAT1_RMD4+0x188)
#define RZONE48_LRA                     (word)(FORMAT1_RMD4+0x18C)
#define RZONE49_STARTSEC                (word)(FORMAT1_RMD4+0x190)
#define RZONE49_LRA                     (word)(FORMAT1_RMD4+0x194)
#define RZONE50_STARTSEC                (word)(FORMAT1_RMD4+0x198)
#define RZONE50_LRA                     (word)(FORMAT1_RMD4+0x19C)
#define RZONE51_STARTSEC                (word)(FORMAT1_RMD4+0x1A0)
#define RZONE51_LRA                     (word)(FORMAT1_RMD4+0x1A4)
#define RZONE52_STARTSEC                (word)(FORMAT1_RMD4+0x1A8)
#define RZONE52_LRA                     (word)(FORMAT1_RMD4+0x1AC)
#define RZONE53_STARTSEC                (word)(FORMAT1_RMD4+0x1B0)
#define RZONE53_LRA                     (word)(FORMAT1_RMD4+0x1B4)
#define RZONE54_STARTSEC                (word)(FORMAT1_RMD4+0x1B8)
#define RZONE54_LRA                     (word)(FORMAT1_RMD4+0x1BC)
#define RZONE55_STARTSEC                (word)(FORMAT1_RMD4+0x1C0)
#define RZONE55_LRA                     (word)(FORMAT1_RMD4+0x1C4)
#define RZONE56_STARTSEC                (word)(FORMAT1_RMD4+0x1C8)
#define RZONE56_LRA                     (word)(FORMAT1_RMD4+0x1CC)
#define RZONE57_STARTSEC                (word)(FORMAT1_RMD4+0x1D0)
#define RZONE57_LRA                     (word)(FORMAT1_RMD4+0x1D4)
#define RZONE58_STARTSEC                (word)(FORMAT1_RMD4+0x1D8)
#define RZONE58_LRA                     (word)(FORMAT1_RMD4+0x1DC)
#define RZONE59_STARTSEC                (word)(FORMAT1_RMD4+0x1E0)
#define RZONE59_LRA                     (word)(FORMAT1_RMD4+0x1E4)
#define RZONE60_STARTSEC                (word)(FORMAT1_RMD4+0x1E8)
#define RZONE60_LRA                     (word)(FORMAT1_RMD4+0x1EC)
#define RZONE61_STARTSEC                (word)(FORMAT1_RMD4+0x1F0)
#define RZONE61_LRA                     (word)(FORMAT1_RMD4+0x1F4)
#define RZONE62_STARTSEC                (word)(FORMAT1_RMD4+0x1F8)
#define RZONE62_LRA                     (word)(FORMAT1_RMD4+0x1FC)
#define RZONE63_STARTSEC                (word)(FORMAT1_RMD4+0x200)
#define RZONE63_LRA                     (word)(FORMAT1_RMD4+0x204)
#define RZONE64_STARTSEC                (word)(FORMAT1_RMD4+0x208)
#define RZONE64_LRA                     (word)(FORMAT1_RMD4+0x20C)
#define RZONE65_STARTSEC                (word)(FORMAT1_RMD4+0x210)
#define RZONE65_LRA                     (word)(FORMAT1_RMD4+0x214)
#define RZONE66_STARTSEC                (word)(FORMAT1_RMD4+0x218)
#define RZONE66_LRA                     (word)(FORMAT1_RMD4+0x21C)
#define RZONE67_STARTSEC                (word)(FORMAT1_RMD4+0x220)
#define RZONE67_LRA                     (word)(FORMAT1_RMD4+0x224)
#define RZONE68_STARTSEC                (word)(FORMAT1_RMD4+0x228)
#define RZONE68_LRA                     (word)(FORMAT1_RMD4+0x22C)
#define RZONE69_STARTSEC                (word)(FORMAT1_RMD4+0x230)
#define RZONE69_LRA                     (word)(FORMAT1_RMD4+0x234)
#define RZONE70_STARTSEC                (word)(FORMAT1_RMD4+0x238)
#define RZONE70_LRA                     (word)(FORMAT1_RMD4+0x23C)
#define RZONE71_STARTSEC                (word)(FORMAT1_RMD4+0x240)
#define RZONE71_LRA                     (word)(FORMAT1_RMD4+0x244)
#define RZONE72_STARTSEC                (word)(FORMAT1_RMD4+0x248)
#define RZONE72_LRA                     (word)(FORMAT1_RMD4+0x24C)
#define RZONE73_STARTSEC                (word)(FORMAT1_RMD4+0x250)
#define RZONE73_LRA                     (word)(FORMAT1_RMD4+0x254)
#define RZONE74_STARTSEC                (word)(FORMAT1_RMD4+0x258)
#define RZONE74_LRA                     (word)(FORMAT1_RMD4+0x25C)
#define RZONE75_STARTSEC                (word)(FORMAT1_RMD4+0x260)
#define RZONE75_LRA                     (word)(FORMAT1_RMD4+0x264)
#define RZONE76_STARTSEC                (word)(FORMAT1_RMD4+0x268)
#define RZONE76_LRA                     (word)(FORMAT1_RMD4+0x26C)
#define RZONE77_STARTSEC                (word)(FORMAT1_RMD4+0x270)
#define RZONE77_LRA                     (word)(FORMAT1_RMD4+0x274)
#define RZONE78_STARTSEC                (word)(FORMAT1_RMD4+0x278)
#define RZONE78_LRA                     (word)(FORMAT1_RMD4+0x27C)
#define RZONE79_STARTSEC                (word)(FORMAT1_RMD4+0x280)
#define RZONE79_LRA                     (word)(FORMAT1_RMD4+0x284)
#define RZONE80_STARTSEC                (word)(FORMAT1_RMD4+0x288)
#define RZONE80_LRA                     (word)(FORMAT1_RMD4+0x28C)
#define RZONE81_STARTSEC                (word)(FORMAT1_RMD4+0x290)
#define RZONE81_LRA                     (word)(FORMAT1_RMD4+0x294)
#define RZONE82_STARTSEC                (word)(FORMAT1_RMD4+0x298)
#define RZONE82_LRA                     (word)(FORMAT1_RMD4+0x29C)
#define RZONE83_STARTSEC                (word)(FORMAT1_RMD4+0x2A0)
#define RZONE83_LRA                     (word)(FORMAT1_RMD4+0x2A4)
#define RZONE84_STARTSEC                (word)(FORMAT1_RMD4+0x2A8)
#define RZONE84_LRA                     (word)(FORMAT1_RMD4+0x2AC)
#define RZONE85_STARTSEC                (word)(FORMAT1_RMD4+0x2B0)
#define RZONE85_LRA                     (word)(FORMAT1_RMD4+0x2B4)
#define RZONE86_STARTSEC                (word)(FORMAT1_RMD4+0x2B8)
#define RZONE86_LRA                     (word)(FORMAT1_RMD4+0x2BC)
#define RZONE87_STARTSEC                (word)(FORMAT1_RMD4+0x2C0)
#define RZONE87_LRA                     (word)(FORMAT1_RMD4+0x2C4)
#define RZONE88_STARTSEC                (word)(FORMAT1_RMD4+0x2C8)
#define RZONE88_LRA                     (word)(FORMAT1_RMD4+0x2CC)
#define RZONE89_STARTSEC                (word)(FORMAT1_RMD4+0x2D0)
#define RZONE89_LRA                     (word)(FORMAT1_RMD4+0x2D4)
#define RZONE90_STARTSEC                (word)(FORMAT1_RMD4+0x2D8)
#define RZONE90_LRA                     (word)(FORMAT1_RMD4+0x2DC)
#define RZONE91_STARTSEC                (word)(FORMAT1_RMD4+0x2E0)
#define RZONE91_LRA                     (word)(FORMAT1_RMD4+0x2E4)
#define RZONE92_STARTSEC                (word)(FORMAT1_RMD4+0x2E8)
#define RZONE92_LRA                     (word)(FORMAT1_RMD4+0x2EC)
#define RZONE93_STARTSEC                (word)(FORMAT1_RMD4+0x2F0)
#define RZONE93_LRA                     (word)(FORMAT1_RMD4+0x2F4)
#define RZONE94_STARTSEC                (word)(FORMAT1_RMD4+0x2F8)
#define RZONE94_LRA                     (word)(FORMAT1_RMD4+0x2FC)
#define RZONE95_STARTSEC                (word)(FORMAT1_RMD4+0x300)
#define RZONE95_LRA                     (word)(FORMAT1_RMD4+0x304)
#define RZONE96_STARTSEC                (word)(FORMAT1_RMD4+0x308)
#define RZONE96_LRA                     (word)(FORMAT1_RMD4+0x30C)
#define RZONE97_STARTSEC                (word)(FORMAT1_RMD4+0x310)
#define RZONE97_LRA                     (word)(FORMAT1_RMD4+0x314)
#define RZONE98_STARTSEC                (word)(FORMAT1_RMD4+0x318)
#define RZONE98_LRA                     (word)(FORMAT1_RMD4+0x31C)
#define RZONE99_STARTSEC                (word)(FORMAT1_RMD4+0x320)
#define RZONE99_LRA                     (word)(FORMAT1_RMD4+0x324)
#define RZONE100_STARTSEC               (word)(FORMAT1_RMD4+0x328)
#define RZONE100_LRA                    (word)(FORMAT1_RMD4+0x32C)
#define RZONE101_STARTSEC               (word)(FORMAT1_RMD4+0x330)
#define RZONE101_LRA                    (word)(FORMAT1_RMD4+0x334)
#define RZONE102_STARTSEC               (word)(FORMAT1_RMD4+0x338)
#define RZONE102_LRA                    (word)(FORMAT1_RMD4+0x33C)
#define RZONE103_STARTSEC               (word)(FORMAT1_RMD4+0x340)
#define RZONE103_LRA                    (word)(FORMAT1_RMD4+0x344)
#define RZONE104_STARTSEC               (word)(FORMAT1_RMD4+0x348)
#define RZONE104_LRA                    (word)(FORMAT1_RMD4+0x34C)
#define RZONE105_STARTSEC               (word)(FORMAT1_RMD4+0x350)
#define RZONE105_LRA                    (word)(FORMAT1_RMD4+0x354)
#define RZONE106_STARTSEC               (word)(FORMAT1_RMD4+0x358)
#define RZONE106_LRA                    (word)(FORMAT1_RMD4+0x35C)
#define RZONE107_STARTSEC               (word)(FORMAT1_RMD4+0x360)
#define RZONE107_LRA                    (word)(FORMAT1_RMD4+0x364)
#define RZONE108_STARTSEC               (word)(FORMAT1_RMD4+0x368)
#define RZONE108_LRA                    (word)(FORMAT1_RMD4+0x36C)
#define RZONE109_STARTSEC               (word)(FORMAT1_RMD4+0x370)
#define RZONE109_LRA                    (word)(FORMAT1_RMD4+0x374)
#define RZONE110_STARTSEC               (word)(FORMAT1_RMD4+0x378)
#define RZONE110_LRA                    (word)(FORMAT1_RMD4+0x37C)
#define RZONE111_STARTSEC               (word)(FORMAT1_RMD4+0x380)
#define RZONE111_LRA                    (word)(FORMAT1_RMD4+0x384)
#define RZONE112_STARTSEC               (word)(FORMAT1_RMD4+0x388)
#define RZONE112_LRA                    (word)(FORMAT1_RMD4+0x38C)
#define RZONE113_STARTSEC               (word)(FORMAT1_RMD4+0x390)
#define RZONE113_LRA                    (word)(FORMAT1_RMD4+0x394)
#define RZONE114_STARTSEC               (word)(FORMAT1_RMD4+0x398)
#define RZONE114_LRA                    (word)(FORMAT1_RMD4+0x39C)
#define RZONE115_STARTSEC               (word)(FORMAT1_RMD4+0x3A0)
#define RZONE115_LRA                    (word)(FORMAT1_RMD4+0x3A4)
#define RZONE116_STARTSEC               (word)(FORMAT1_RMD4+0x3A8)
#define RZONE116_LRA                    (word)(FORMAT1_RMD4+0x3AC)
#define RZONE117_STARTSEC               (word)(FORMAT1_RMD4+0x3B0)
#define RZONE117_LRA                    (word)(FORMAT1_RMD4+0x3B4)
#define RZONE118_STARTSEC               (word)(FORMAT1_RMD4+0x3B8)
#define RZONE118_LRA                    (word)(FORMAT1_RMD4+0x3BC)
#define RZONE119_STARTSEC               (word)(FORMAT1_RMD4+0x3C0)
#define RZONE119_LRA                    (word)(FORMAT1_RMD4+0x3C4)
#define RZONE120_STARTSEC               (word)(FORMAT1_RMD4+0x3C8)
#define RZONE120_LRA                    (word)(FORMAT1_RMD4+0x3CC)
#define RZONE121_STARTSEC               (word)(FORMAT1_RMD4+0x3D0)
#define RZONE121_LRA                    (word)(FORMAT1_RMD4+0x3D4)
#define RZONE122_STARTSEC               (word)(FORMAT1_RMD4+0x3D8)
#define RZONE122_LRA                    (word)(FORMAT1_RMD4+0x3DC)
#define RZONE123_STARTSEC               (word)(FORMAT1_RMD4+0x3E0)
#define RZONE123_LRA                    (word)(FORMAT1_RMD4+0x3E4)
#define RZONE124_STARTSEC               (word)(FORMAT1_RMD4+0x3E8)
#define RZONE124_LRA                    (word)(FORMAT1_RMD4+0x3EC)
#define RZONE125_STARTSEC               (word)(FORMAT1_RMD4+0x3F0)
#define RZONE125_LRA                    (word)(FORMAT1_RMD4+0x3F4)
#define RZONE126_STARTSEC               (word)(FORMAT1_RMD4+0x3F8)
#define RZONE126_LRA                    (word)(FORMAT1_RMD4+0x3FC)
#define RZONE127_STARTSEC               (word)(FORMAT1_RMD4+0x400)
#define RZONE127_LRA                    (word)(FORMAT1_RMD4+0x404)
#define RZONE128_STARTSEC               (word)(FORMAT1_RMD4+0x408)
#define RZONE128_LRA                    (word)(FORMAT1_RMD4+0x40C)
#define RZONE129_STARTSEC               (word)(FORMAT1_RMD4+0x410)
#define RZONE129_LRA                    (word)(FORMAT1_RMD4+0x414)
#define RZONE130_STARTSEC               (word)(FORMAT1_RMD4+0x418)
#define RZONE130_LRA                    (word)(FORMAT1_RMD4+0x41C)
#define RZONE131_STARTSEC               (word)(FORMAT1_RMD4+0x420)
#define RZONE131_LRA                    (word)(FORMAT1_RMD4+0x424)
#define RZONE132_STARTSEC               (word)(FORMAT1_RMD4+0x428)
#define RZONE132_LRA                    (word)(FORMAT1_RMD4+0x42C)
#define RZONE133_STARTSEC               (word)(FORMAT1_RMD4+0x430)
#define RZONE133_LRA                    (word)(FORMAT1_RMD4+0x434)
#define RZONE134_STARTSEC               (word)(FORMAT1_RMD4+0x438)
#define RZONE134_LRA                    (word)(FORMAT1_RMD4+0x43C)
#define RZONE135_STARTSEC               (word)(FORMAT1_RMD4+0x440)
#define RZONE135_LRA                    (word)(FORMAT1_RMD4+0x444)
#define RZONE136_STARTSEC               (word)(FORMAT1_RMD4+0x448)
#define RZONE136_LRA                    (word)(FORMAT1_RMD4+0x44C)
#define RZONE137_STARTSEC               (word)(FORMAT1_RMD4+0x450)
#define RZONE137_LRA                    (word)(FORMAT1_RMD4+0x454)
#define RZONE138_STARTSEC               (word)(FORMAT1_RMD4+0x458)
#define RZONE138_LRA                    (word)(FORMAT1_RMD4+0x45C)
#define RZONE139_STARTSEC               (word)(FORMAT1_RMD4+0x460)
#define RZONE139_LRA                    (word)(FORMAT1_RMD4+0x464)
#define RZONE140_STARTSEC               (word)(FORMAT1_RMD4+0x468)
#define RZONE140_LRA                    (word)(FORMAT1_RMD4+0x46C)
#define RZONE141_STARTSEC               (word)(FORMAT1_RMD4+0x470)
#define RZONE141_LRA                    (word)(FORMAT1_RMD4+0x474)
#define RZONE142_STARTSEC               (word)(FORMAT1_RMD4+0x478)
#define RZONE142_LRA                    (word)(FORMAT1_RMD4+0x47C)
#define RZONE143_STARTSEC               (word)(FORMAT1_RMD4+0x480)
#define RZONE143_LRA                    (word)(FORMAT1_RMD4+0x484)
#define RZONE144_STARTSEC               (word)(FORMAT1_RMD4+0x488)
#define RZONE144_LRA                    (word)(FORMAT1_RMD4+0x48C)
#define RZONE145_STARTSEC               (word)(FORMAT1_RMD4+0x490)
#define RZONE145_LRA                    (word)(FORMAT1_RMD4+0x494)
#define RZONE146_STARTSEC               (word)(FORMAT1_RMD4+0x498)
#define RZONE146_LRA                    (word)(FORMAT1_RMD4+0x49C)
#define RZONE147_STARTSEC               (word)(FORMAT1_RMD4+0x4A0)
#define RZONE147_LRA                    (word)(FORMAT1_RMD4+0x4A4)
#define RZONE148_STARTSEC               (word)(FORMAT1_RMD4+0x4A8)
#define RZONE148_LRA                    (word)(FORMAT1_RMD4+0x4AC)
#define RZONE149_STARTSEC               (word)(FORMAT1_RMD4+0x4B0)
#define RZONE149_LRA                    (word)(FORMAT1_RMD4+0x4B4)
#define RZONE150_STARTSEC               (word)(FORMAT1_RMD4+0x4B8)
#define RZONE150_LRA                    (word)(FORMAT1_RMD4+0x4BC)
#define RZONE151_STARTSEC               (word)(FORMAT1_RMD4+0x4C0)
#define RZONE151_LRA                    (word)(FORMAT1_RMD4+0x4C4)
#define RZONE152_STARTSEC               (word)(FORMAT1_RMD4+0x4C8)
#define RZONE152_LRA                    (word)(FORMAT1_RMD4+0x4CC)
#define RZONE153_STARTSEC               (word)(FORMAT1_RMD4+0x4D0)
#define RZONE153_LRA                    (word)(FORMAT1_RMD4+0x4D4)
#define RZONE154_STARTSEC               (word)(FORMAT1_RMD4+0x4D8)
#define RZONE154_LRA                    (word)(FORMAT1_RMD4+0x4DC)
#define RZONE155_STARTSEC               (word)(FORMAT1_RMD4+0x4E0)
#define RZONE155_LRA                    (word)(FORMAT1_RMD4+0x4E4)
#define RZONE156_STARTSEC               (word)(FORMAT1_RMD4+0x4E8)
#define RZONE156_LRA                    (word)(FORMAT1_RMD4+0x4EC)
#define RZONE157_STARTSEC               (word)(FORMAT1_RMD4+0x4F0)
#define RZONE157_LRA                    (word)(FORMAT1_RMD4+0x4F4)
#define RZONE158_STARTSEC               (word)(FORMAT1_RMD4+0x4F8)
#define RZONE158_LRA                    (word)(FORMAT1_RMD4+0x4FC)
#define RZONE159_STARTSEC               (word)(FORMAT1_RMD4+0x500)
#define RZONE159_LRA                    (word)(FORMAT1_RMD4+0x504)
#define RZONE160_STARTSEC               (word)(FORMAT1_RMD4+0x508)
#define RZONE160_LRA                    (word)(FORMAT1_RMD4+0x50C)
#define RZONE161_STARTSEC               (word)(FORMAT1_RMD4+0x510)
#define RZONE161_LRA                    (word)(FORMAT1_RMD4+0x514)
#define RZONE162_STARTSEC               (word)(FORMAT1_RMD4+0x518)
#define RZONE162_LRA                    (word)(FORMAT1_RMD4+0x51C)
#define RZONE163_STARTSEC               (word)(FORMAT1_RMD4+0x520)
#define RZONE163_LRA                    (word)(FORMAT1_RMD4+0x524)
#define RZONE164_STARTSEC               (word)(FORMAT1_RMD4+0x528)
#define RZONE164_LRA                    (word)(FORMAT1_RMD4+0x52C)
#define RZONE165_STARTSEC               (word)(FORMAT1_RMD4+0x530)
#define RZONE165_LRA                    (word)(FORMAT1_RMD4+0x534)
#define RZONE166_STARTSEC               (word)(FORMAT1_RMD4+0x538)
#define RZONE166_LRA                    (word)(FORMAT1_RMD4+0x53C)
#define RZONE167_STARTSEC               (word)(FORMAT1_RMD4+0x540)
#define RZONE167_LRA                    (word)(FORMAT1_RMD4+0x544)
#define RZONE168_STARTSEC               (word)(FORMAT1_RMD4+0x548)
#define RZONE168_LRA                    (word)(FORMAT1_RMD4+0x54C)
#define RZONE169_STARTSEC               (word)(FORMAT1_RMD4+0x550)
#define RZONE169_LRA                    (word)(FORMAT1_RMD4+0x554)
#define RZONE170_STARTSEC               (word)(FORMAT1_RMD4+0x558)
#define RZONE170_LRA                    (word)(FORMAT1_RMD4+0x55C)
#define RZONE171_STARTSEC               (word)(FORMAT1_RMD4+0x560)
#define RZONE171_LRA                    (word)(FORMAT1_RMD4+0x564)
#define RZONE172_STARTSEC               (word)(FORMAT1_RMD4+0x568)
#define RZONE172_LRA                    (word)(FORMAT1_RMD4+0x56C)
#define RZONE173_STARTSEC               (word)(FORMAT1_RMD4+0x570)
#define RZONE173_LRA                    (word)(FORMAT1_RMD4+0x574)
#define RZONE174_STARTSEC               (word)(FORMAT1_RMD4+0x578)
#define RZONE174_LRA                    (word)(FORMAT1_RMD4+0x57C)
#define RZONE175_STARTSEC               (word)(FORMAT1_RMD4+0x580)
#define RZONE175_LRA                    (word)(FORMAT1_RMD4+0x584)
#define RZONE176_STARTSEC               (word)(FORMAT1_RMD4+0x588)
#define RZONE176_LRA                    (word)(FORMAT1_RMD4+0x58C)
#define RZONE177_STARTSEC               (word)(FORMAT1_RMD4+0x590)
#define RZONE177_LRA                    (word)(FORMAT1_RMD4+0x594)
#define RZONE178_STARTSEC               (word)(FORMAT1_RMD4+0x598)
#define RZONE178_LRA                    (word)(FORMAT1_RMD4+0x59C)
#define RZONE179_STARTSEC               (word)(FORMAT1_RMD4+0x5A0)
#define RZONE179_LRA                    (word)(FORMAT1_RMD4+0x5A4)
#define RZONE180_STARTSEC               (word)(FORMAT1_RMD4+0x5A8)
#define RZONE180_LRA                    (word)(FORMAT1_RMD4+0x5AC)
#define RZONE181_STARTSEC               (word)(FORMAT1_RMD4+0x5B0)
#define RZONE181_LRA                    (word)(FORMAT1_RMD4+0x5B4)
#define RZONE182_STARTSEC               (word)(FORMAT1_RMD4+0x5B8)
#define RZONE182_LRA                    (word)(FORMAT1_RMD4+0x5BC)
#define RZONE183_STARTSEC               (word)(FORMAT1_RMD4+0x5C0)
#define RZONE183_LRA                    (word)(FORMAT1_RMD4+0x5C4)
#define RZONE184_STARTSEC               (word)(FORMAT1_RMD4+0x5C8)
#define RZONE184_LRA                    (word)(FORMAT1_RMD4+0x5CC)
#define RZONE185_STARTSEC               (word)(FORMAT1_RMD4+0x5D0)
#define RZONE185_LRA                    (word)(FORMAT1_RMD4+0x5D4)
#define RZONE186_STARTSEC               (word)(FORMAT1_RMD4+0x5D8)
#define RZONE186_LRA                    (word)(FORMAT1_RMD4+0x5DC)
#define RZONE187_STARTSEC               (word)(FORMAT1_RMD4+0x5E0)
#define RZONE187_LRA                    (word)(FORMAT1_RMD4+0x5E4)
#define RZONE188_STARTSEC               (word)(FORMAT1_RMD4+0x5E8)
#define RZONE188_LRA                    (word)(FORMAT1_RMD4+0x5EC)
#define RZONE189_STARTSEC               (word)(FORMAT1_RMD4+0x5F0)
#define RZONE189_LRA                    (word)(FORMAT1_RMD4+0x5F4)
#define RZONE190_STARTSEC               (word)(FORMAT1_RMD4+0x5F8)
#define RZONE190_LRA                    (word)(FORMAT1_RMD4+0x5FC)
#define RZONE191_STARTSEC               (word)(FORMAT1_RMD4+0x600)
#define RZONE191_LRA                    (word)(FORMAT1_RMD4+0x604)
#define RZONE192_STARTSEC               (word)(FORMAT1_RMD4+0x608)
#define RZONE192_LRA                    (word)(FORMAT1_RMD4+0x60C)
#define RZONE193_STARTSEC               (word)(FORMAT1_RMD4+0x610)
#define RZONE193_LRA                    (word)(FORMAT1_RMD4+0x614)
#define RZONE194_STARTSEC               (word)(FORMAT1_RMD4+0x618)
#define RZONE194_LRA                    (word)(FORMAT1_RMD4+0x61C)
#define RZONE195_STARTSEC               (word)(FORMAT1_RMD4+0x620)
#define RZONE195_LRA                    (word)(FORMAT1_RMD4+0x624)
#define RZONE196_STARTSEC               (word)(FORMAT1_RMD4+0x628)
#define RZONE196_LRA                    (word)(FORMAT1_RMD4+0x62C)
#define RZONE197_STARTSEC               (word)(FORMAT1_RMD4+0x630)
#define RZONE197_LRA                    (word)(FORMAT1_RMD4+0x634)
#define RZONE198_STARTSEC               (word)(FORMAT1_RMD4+0x638)
#define RZONE198_LRA                    (word)(FORMAT1_RMD4+0x63C)
#define RZONE199_STARTSEC               (word)(FORMAT1_RMD4+0x640)
#define RZONE199_LRA                    (word)(FORMAT1_RMD4+0x644)
#define RZONE200_STARTSEC               (word)(FORMAT1_RMD4+0x648)
#define RZONE200_LRA                    (word)(FORMAT1_RMD4+0x64C)
#define RZONE201_STARTSEC               (word)(FORMAT1_RMD4+0x650)
#define RZONE201_LRA                    (word)(FORMAT1_RMD4+0x654)
#define RZONE202_STARTSEC               (word)(FORMAT1_RMD4+0x658)
#define RZONE202_LRA                    (word)(FORMAT1_RMD4+0x65C)
#define RZONE203_STARTSEC               (word)(FORMAT1_RMD4+0x660)
#define RZONE203_LRA                    (word)(FORMAT1_RMD4+0x664)
#define RZONE204_STARTSEC               (word)(FORMAT1_RMD4+0x668)
#define RZONE204_LRA                    (word)(FORMAT1_RMD4+0x66C)
#define RZONE205_STARTSEC               (word)(FORMAT1_RMD4+0x670)
#define RZONE205_LRA                    (word)(FORMAT1_RMD4+0x674)
#define RZONE206_STARTSEC               (word)(FORMAT1_RMD4+0x678)
#define RZONE206_LRA                    (word)(FORMAT1_RMD4+0x67C)
#define RZONE207_STARTSEC               (word)(FORMAT1_RMD4+0x680)
#define RZONE207_LRA                    (word)(FORMAT1_RMD4+0x684)
#define RZONE208_STARTSEC               (word)(FORMAT1_RMD4+0x688)
#define RZONE208_LRA                    (word)(FORMAT1_RMD4+0x68C)
#define RZONE209_STARTSEC               (word)(FORMAT1_RMD4+0x690)
#define RZONE209_LRA                    (word)(FORMAT1_RMD4+0x694)
#define RZONE210_STARTSEC               (word)(FORMAT1_RMD4+0x698)
#define RZONE210_LRA                    (word)(FORMAT1_RMD4+0x69C)
#define RZONE211_STARTSEC               (word)(FORMAT1_RMD4+0x6A0)
#define RZONE211_LRA                    (word)(FORMAT1_RMD4+0x6A4)
#define RZONE212_STARTSEC               (word)(FORMAT1_RMD4+0x6A8)
#define RZONE212_LRA                    (word)(FORMAT1_RMD4+0x6AC)
#define RZONE213_STARTSEC               (word)(FORMAT1_RMD4+0x6B0)
#define RZONE213_LRA                    (word)(FORMAT1_RMD4+0x6B4)
#define RZONE214_STARTSEC               (word)(FORMAT1_RMD4+0x6B8)
#define RZONE214_LRA                    (word)(FORMAT1_RMD4+0x6BC)
#define RZONE215_STARTSEC               (word)(FORMAT1_RMD4+0x6C0)
#define RZONE215_LRA                    (word)(FORMAT1_RMD4+0x6C4)
#define RZONE216_STARTSEC               (word)(FORMAT1_RMD4+0x6C8)
#define RZONE216_LRA                    (word)(FORMAT1_RMD4+0x6CC)
#define RZONE217_STARTSEC               (word)(FORMAT1_RMD4+0x6D0)
#define RZONE217_LRA                    (word)(FORMAT1_RMD4+0x6D4)
#define RZONE218_STARTSEC               (word)(FORMAT1_RMD4+0x6D8)
#define RZONE218_LRA                    (word)(FORMAT1_RMD4+0x6DC)
#define RZONE219_STARTSEC               (word)(FORMAT1_RMD4+0x6E0)
#define RZONE219_LRA                    (word)(FORMAT1_RMD4+0x6E4)
#define RZONE220_STARTSEC               (word)(FORMAT1_RMD4+0x6E8)
#define RZONE220_LRA                    (word)(FORMAT1_RMD4+0x6EC)
#define RZONE221_STARTSEC               (word)(FORMAT1_RMD4+0x6F0)
#define RZONE221_LRA                    (word)(FORMAT1_RMD4+0x6F4)
#define RZONE222_STARTSEC               (word)(FORMAT1_RMD4+0x6F8)
#define RZONE222_LRA                    (word)(FORMAT1_RMD4+0x6FC)
#define RZONE223_STARTSEC               (word)(FORMAT1_RMD4+0x700)
#define RZONE223_LRA                    (word)(FORMAT1_RMD4+0x704)
#define RZONE224_STARTSEC               (word)(FORMAT1_RMD4+0x708)
#define RZONE224_LRA                    (word)(FORMAT1_RMD4+0x70C)
#define RZONE225_STARTSEC               (word)(FORMAT1_RMD4+0x710)
#define RZONE225_LRA                    (word)(FORMAT1_RMD4+0x714)
#define RZONE226_STARTSEC               (word)(FORMAT1_RMD4+0x718)
#define RZONE226_LRA                    (word)(FORMAT1_RMD4+0x71C)
#define RZONE227_STARTSEC               (word)(FORMAT1_RMD4+0x720)
#define RZONE227_LRA                    (word)(FORMAT1_RMD4+0x724)
#define RZONE228_STARTSEC               (word)(FORMAT1_RMD4+0x728)
#define RZONE228_LRA                    (word)(FORMAT1_RMD4+0x72C)
#define RZONE229_STARTSEC               (word)(FORMAT1_RMD4+0x730)
#define RZONE229_LRA                    (word)(FORMAT1_RMD4+0x734)
#define RZONE230_STARTSEC               (word)(FORMAT1_RMD4+0x738)
#define RZONE230_LRA                    (word)(FORMAT1_RMD4+0x73C)
#define RZONE231_STARTSEC               (word)(FORMAT1_RMD4+0x740)
#define RZONE231_LRA                    (word)(FORMAT1_RMD4+0x744)
#define RZONE232_STARTSEC               (word)(FORMAT1_RMD4+0x748)
#define RZONE232_LRA                    (word)(FORMAT1_RMD4+0x74C)
#define RZONE233_STARTSEC               (word)(FORMAT1_RMD4+0x750)
#define RZONE233_LRA                    (word)(FORMAT1_RMD4+0x754)
#define RZONE234_STARTSEC               (word)(FORMAT1_RMD4+0x758)
#define RZONE234_LRA                    (word)(FORMAT1_RMD4+0x75C)
#define RZONE235_STARTSEC               (word)(FORMAT1_RMD4+0x760)
#define RZONE235_LRA                    (word)(FORMAT1_RMD4+0x764)
#define RZONE236_STARTSEC               (word)(FORMAT1_RMD4+0x768)
#define RZONE236_LRA                    (word)(FORMAT1_RMD4+0x76C)
#define RZONE237_STARTSEC               (word)(FORMAT1_RMD4+0x770)
#define RZONE237_LRA                    (word)(FORMAT1_RMD4+0x774)
#define RZONE238_STARTSEC               (word)(FORMAT1_RMD4+0x778)
#define RZONE238_LRA                    (word)(FORMAT1_RMD4+0x77C)
#define RZONE239_STARTSEC               (word)(FORMAT1_RMD4+0x780)
#define RZONE239_LRA                    (word)(FORMAT1_RMD4+0x784)
#define RZONE240_STARTSEC               (word)(FORMAT1_RMD4+0x788)
#define RZONE240_LRA                    (word)(FORMAT1_RMD4+0x78C)
#define RZONE241_STARTSEC               (word)(FORMAT1_RMD4+0x790)
#define RZONE241_LRA                    (word)(FORMAT1_RMD4+0x794)
#define RZONE242_STARTSEC               (word)(FORMAT1_RMD4+0x798)
#define RZONE242_LRA                    (word)(FORMAT1_RMD4+0x79C)
#define RZONE243_STARTSEC               (word)(FORMAT1_RMD4+0x7A0)
#define RZONE243_LRA                    (word)(FORMAT1_RMD4+0x7A4)
#define RZONE244_STARTSEC               (word)(FORMAT1_RMD4+0x7A8)
#define RZONE244_LRA                    (word)(FORMAT1_RMD4+0x7AC)
#define RZONE245_STARTSEC               (word)(FORMAT1_RMD4+0x7B0)
#define RZONE245_LRA                    (word)(FORMAT1_RMD4+0x7B4)
#define RZONE246_STARTSEC               (word)(FORMAT1_RMD4+0x7B8)
#define RZONE246_LRA                    (word)(FORMAT1_RMD4+0x7BC)
#define RZONE247_STARTSEC               (word)(FORMAT1_RMD4+0x7C0)
#define RZONE247_LRA                    (word)(FORMAT1_RMD4+0x7C4)
#define RZONE248_STARTSEC               (word)(FORMAT1_RMD4+0x7C8)
#define RZONE248_LRA                    (word)(FORMAT1_RMD4+0x7CC)
#define RZONE249_STARTSEC               (word)(FORMAT1_RMD4+0x7D0)
#define RZONE249_LRA                    (word)(FORMAT1_RMD4+0x7D4)
#define RZONE250_STARTSEC               (word)(FORMAT1_RMD4+0x7D8)
#define RZONE250_LRA                    (word)(FORMAT1_RMD4+0x7DC)
#define RZONE251_STARTSEC               (word)(FORMAT1_RMD4+0x7E0)
#define RZONE251_LRA                    (word)(FORMAT1_RMD4+0x7E4)
#define RZONE252_STARTSEC               (word)(FORMAT1_RMD4+0x7E8)
#define RZONE252_LRA                    (word)(FORMAT1_RMD4+0x7EC)
#define RZONE253_STARTSEC               (word)(FORMAT1_RMD4+0x7F0)
#define RZONE253_LRA                    (word)(FORMAT1_RMD4+0x7F4)
#define RZONE254_STARTSEC               (word)(FORMAT1_RMD4+0x7F8)
#define RZONE254_LRA                    (word)(FORMAT1_RMD4+0x7FC)


/* LEAD-IN Area Definitions */

#define BOOK_PARTVER                    (word)(RPHY_FORMAT_INFO+0x00)
#define DISC_SIZE_XFER                  (word)(RPHY_FORMAT_INFO+0x01)
#define DISC_STR                        (word)(RPHY_FORMAT_INFO+0x02)
#define RECORDED_DENSITY                (word)(RPHY_FORMAT_INFO+0x03)
#define DATA_ALLOC                      (word)(RPHY_FORMAT_INFO+0x04)
#define NBCA_DESCR                      (word)(RPHY_FORMAT_INFO+0x10)
#define BORDERZONE_START                (word)(RPHY_FORMAT_INFO+0x20)
#endif

//**********************************************************************************************
//
//
//                                  Typedefs  for DVD code
//
//
//
//**********************************************************************************************

typedef struct {
    unsigned int    inf;    /* Sector Information */
    unsigned long   no;     /* Sector Number */
} TypeID;

typedef struct
{                      /* struct TPC   */
                        /*  Bit  Access */
        BYTE b7:1;          /*    NDER7     */
        BYTE b6:1;          /*    NDER6     */
        BYTE b5:1;          /*    NDER5     */
        BYTE b4:1;          /*    NDER4     */
        BYTE b3:1;          /*    NDER3     */
        BYTE b2:1;          /*    NDER2     */
        BYTE b1:1;          /*    NDER1     */
        BYTE b0:1;          /*    NDER0     */
} Param_T;

typedef union
{
    BYTE    Param;
    Param_T Para;
} Param_TB;


typedef union
{
    BYTE bf[3];     /* 3 byte buffer */
} CMD_BLK_3;



/*******************************************************************
    ENUMERATIONS
*******************************************************************/
typedef enum
{
    BUF_GOOD,
    GEN_FAIL,
    BCA_FAIL,
    EXTEND_READ_FAIL,
    ID_FAIL,
    EDC_FAIL,
    SEQID_FAIL,
    SEEK_FAIL,
    SYNC_LOST_FAIL,
    TID_FAIL,
    TIMEOUT,
    UNAUTH_RD_FAIL,
    UNCOR_ECC_FAIL,
    TOC_EMPTY,           /* No TOC blocks in TOC zone. For DVD +R only */
    TOC_FAIL,            /* TOC cannot be read or saved. For DVD +R only */
    CONTROL_ZONE_EMPTY,    /* No ECC block in control data zone. For DVD +R only */
    NO_SDCB,            // SDCB does not exist
    SDCB_FAIL,          // SDCB exists but missing SDCB descriptor
    NO_DCB,             // FDCB & WDCB do not exist
    FDCB_FAIL,          // DCB exists but missing FDCB descriptor
    WDCB_FAIL,          // DCB exists but missing WDCB descriptor
    RAI_EMPTY,          // Recorded Area Indicators have no entries.
    RMA_EMPTY,
    RMA_FAIL,
    RMA_FORMAT3_FAIL,
    BLANKDISC,
    NO_DMA,
    NO_TDMA,
    EJECT_PRESS
} DiscAccessStatusE;

typedef enum
{

    INIT_READ,          /* Initialize data transfer for DVD Read */
    INIT_READ_AHEAD,    /* Initialize data transfer for read ahead  */
    EXTEND_READ,        /* Extend Data transfer for DVD Read */
    EXTEND_READ_AHEAD,  /* Extend data transfer for DVD read ahead */
    WRITE,
    BCA_READ

} TransferTypeE;    /* Type of DVD Transfer Enumeration */

typedef enum
    {
        CACHE_MISS,
        FULL_CACHE_HIT,
        PARTIAL_CACHE_HIT
    } SearchCacheResultE;

/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*-----------------------------------------------------------------
    DVD Flags
------------------------------------------------------------------*/
/*-------------------------------------------------------
    Structure for flags involving DVD decoding
--------------------------------------------------------*/
struct dvdDecFlagS
{
    BYTE    fFIRST_HOST_BLOCK;          // 1 = Waiting for first host block to be buffered
    BYTE    fREAD_BCA;                  // 1 = BCA read is requested by host
    BYTE    fSYNC_INTERRUPT_ENABLED;    // 1 = DVD sync interrupt is enabled
    BYTE    fREAD_LONG_COMMAND;         // 1 = Read is a DVD Read Long command
    BYTE    fREADING_LEADIN;            // 1 = Currently reading the lead in area
    BYTE    fEXTEND_READ;               // 1 = Extend the current buffering process
    BYTE    fREAD_AHEAD;                // 1 = Read ahead buffering
    BYTE    fDISABLE_READ_AHEAD;        // 1 = Do not start read ahead buffering

    BYTE    fSTREAMING;                 // 1 = READ12 Command Streaming bit   //20090304_m.22==start==add READ12 Command Streaming bit.
    // TEST FLAGS
//    BYTE    fIGNORE_ECC_ERRORS;         // 1 = Ignore ECC errors when buffering
//    BYTE    fLOG_PIPO;                  // 1 = Log PI/PO information
    BYTE    fBUFI_ENABLED;              // 1 = Set if BUFFEN is set in DVDIRQC1
#if (OPEN_SESSION==1)
    BYTE    fREADING_SESSION_POSITION;
#endif       
};

#if (OPEN_SESSION==1)
typedef enum
{
    READ_NOTHING,
    READ_CONTROL_DATA,      
    READ_INTRO,   
    READ_CLOSURE,  
    READ_BORDER_IN, 
    READ_BORDER_OUT
} ReadSessionPosition;
#endif

/*-------------------------------------------------------
    Structure for DVD error flags
--------------------------------------------------------*/
struct dvdErrorFlagS
{
    /* SYSTEM ERRORS */
    BYTE fUNAUTH_READ_ERROR;
    BYTE fSEEK_FAIL;
    BYTE fBCA_ERROR;
    /* DECODE ERRORS */
    BYTE fSEQ_ID_ERROR;
    BYTE fUNCOR_ECC;
    BYTE fBANDWIDTH_ERROR;
    BYTE fTID_MISS;
    BYTE fEDC_ERROR;
    BYTE fID_ERROR;
    BYTE fSYNC_LOST;
    BYTE fID_MISMATCH;
    BYTE fFIFO_FULL;
};

/*-------------------------------------------------------
    Structure for DVD decoder interrupt flags
--------------------------------------------------------*/
struct dvdDecIntFlagS
{
#if (ENABLE_HDDVD)
    BYTE fCLUSTER_BLK_INT; /*HD DVD ECC BLOCK INTERRUPT*/
#endif
    BYTE fECC_BLK_INT;
    BYTE fECC_DONE_INT;
    BYTE fBUF_BLK_INT;
    BYTE fBUF_DONE_INT;
    BYTE fNO_INT;
};

/*-----------------------------------------------------------------
    DVD Timer
------------------------------------------------------------------*/
typedef struct
{
    WORD    count;
    BYTE    fTIMEOUT;
    BYTE    state;
} dvdTimerS;

/*---------------------------------------------------------------------------------------------------------
    Lead-in and disc information structures and defines
----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------
    Region Settings
------------------------------------------*/
#define REGION_1        0xFE      /*  1: U.S., Canada, U.S. Territories                                                     */
#define REGION_2        0xFD      /*  2: Japan, Europe, South Africa, Middle East (including Egypt)                             */
#define REGION_3        0xFB      /*  3: Southeast Asia, East Asia (including Hong Kong)                                        */
#define REGION_4        0xF7      /*  4: Australia, New Zealand, Pacific Islands, Central America, Mexico, South America, Caribbean     */
#define REGION_5        0xEF      /*  5: Eastern Europe (Former Soviet Union), Indian Subcontinent, Africa (also North Korea, Mongolia) */
#define REGION_6        0xDF      /*  6: China                                                                      */
#define REGION_7        0xBF      /*  7: Reserved                                                                       */
#define REGION_8        0x7F      /*  8: Special international venues (airplanes, cruise ships, etc.)                           */
#define REGION_WORLD    0x00      /* For Debug only this allows discs of any region to be played                                */

/*-----------------------------------------
    Layer types
------------------------------------------*/
#define EMBOSSED_AREA       0x01
#define RECORDABLE_AREA     0x02
#define REWRITEABLE_AREA    0x04

/*-----------------------------------------
    Definition masks for Sector Information byte
------------------------------------------*/
#define SINFO_SECTOR_FORMAT_TYPE  0x80
#define SINFO_TRACKING_METHOD     0x40
#define SINFO_REFLECTIVITY        0x20
#define SINFO_AREA_TYPE           0x0C
#define SINFO_DATA_TYPE           0x02
#define SINFO_LAYER_NUMBER        0x01


typedef enum
{
    DataArea,
    LeadInArea,
    LeadOutArea,
    MiddleArea
}dvdAreaTypeE;


typedef enum
{
    CSS_START,                         // Start CSS Authentication Process
    DRIVE_CHALLENGE,                   // SendKey fmt 1
    DRIVE_RESPONSE,                    // ReportKey fmt 2
    DRIVE_RESPONSE_COMPLETE,           // after CSS int
    DECCHAL,                           // ReportKey fmt 1
    DECCHALCMPLT,                      // after CSS int
    DECRSP,                            // SendKey fmt 3
    AUTHENTICATION_COMPLETE,           // Authentication complete
    AUTHENTICATION_FAILED,             // Authentication failed
    SEND_DISC_KEY_STRUCTURES_TO_HOST,  // Read Dvd Structures fmt 2
    CSS_DISC_KEY_SENT,                 // after calling css Send Disc Key
    READ_TITLE_KEY,                    // ReportKey fmt 4
    TITLE_KEY_HAS_BEEN_SENT,           // after calling cssSendTitleKey
    INVALID_AUTHENTICATION_SEQUENCE,   // invalid authentication sequence
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    GENBKFORMK,
    GENERATE_BUS_KEY_FOR_MEDIA_IDENTIFIER,
#endif  /* ENABLE_CPRM == 1 && BCA_ENABLE == 1 */
    ILLEGAL_CSS_STATE                  // illegal CSS state
} CSSSTATE;

typedef struct
{
    BYTE AgidInUse              :1;
    BYTE AuthenticationSuccess  :1;
    BYTE MedProt                :1;
    BYTE Reserved               :5;
} CSSFLAG;

typedef struct
{
    ULONG StartPSN;
    ULONG EndPSN;
} dvdMutiS;

//----------------------- DVD-R/-RW -------------------------------------

#define Format1_RMD           0x01
#define Format2_RMD           0x02
#define Format3_RMD           0x03
#define Format4_RMD           0x04

#define SequentialRecord      0x00

#define EmptyDisc             0x00
#define DiscAtOnce            0x01
#define IncRecMode            0x02
#define FinalIncDisc          0x03
#define MiniBlank             0x04
#define EraseInProg           0x05

#define RestricedOverWrite    0x10

#define FormatInProg          0x11
#define ResOverWriteComBorder 0x12
#define ResOverWriteIntBorder 0x13

#define EmptyWriteProtect     0x80
#define DAOWriteProtect       0x81
#define IncWriteProtect       0x82
#define FinalIncWriteProtect  0x83
#define MiniBlankWriteProtect 0x84

#define ResOverWriteCBWP      0x92
#define ResOverWriteINWP      0x93

#define NoBlank               0x00
#define DiscIsBlank           0x01
#define MiniBlankDisc         0x02
#define BlankRZone            0x05
#define UncloseBorder         0x06
#define EraseBorder           0x07

/* DVD Minus Format3 Format Operation Code */
#define NoFormat              0x00
#define FullFormat            0x01
#define GrowBorder            0x02
#define AddBorder             0x03
#define QuickGrowBorder       0x04
#define QuickAddBorder        0x05
#define CloseBorder           0x06

//------------------------------------------------------------------------------------
//
//  DVD CSS related Structure
//
//------------------------------------------------------------------------------------
struct dvdCssS
{
    CSSSTATE                eCssState;       // CSS Status / State
    CSSFLAG                 CssFlag;         // CSS Flags
    BYTE        volatile    AuthenticationGrantId;
    BYTE                    RegionMatch;     // For Region Playback Control (RPC)
                                             // Indicates status of region setting match
                                             // between disc and device

};


typedef union
{
    struct
    {
        BYTE    YearMsb;
        BYTE    YearLsb2;
        BYTE    YearLsb1;
        BYTE    YearLsb0;
        BYTE    MonthMsb;
        BYTE    MonthLsb;
        BYTE    DayMsb;
        BYTE    DayLsb;
        BYTE    HourMsb;
        BYTE    HourLsb;
        BYTE    MinuteMsb;
        BYTE    MinuteLsb;
        BYTE    SecondMsb;
        BYTE    SecondLsb;
    }byte;
    BYTE Byte[14];
}dvdTimeStampS;


struct dvdSessionS
//------------------------------------------------------------------------------------
//
//  DVD Session Structure
//
//------------------------------------------------------------------------------------
{
    dvdMutiS MultiState[192];
    ULONG NextBorderInPSN;    //DVD-R
};

/*----------------------------------------------
   Data Interface to dvdStartDiscTransfer
-----------------------------------------------*/

typedef struct
{
    StLongU             TargetID;           /* Starting Address of transfer             */
    ULONG               EccBlockCount;      /* Total ecc blocks to be transferred       */
    ULONG               EccBlocksBuffered;  /* Ecc blocks that have been transferred    */
    BYTE                BcaLen;             /* BCA Length after a BCA read.             */
    BYTE                TargetLayer;        /* DVD Disc Layer.                          */
    TransferStatesE     State;              /* State of transfer                        */
    TransferTypeE       Type;               /* The type of Read to perform.             */
    BYTE                ExtendBlocks;       /* Number of ecc blocks to extend buffering */

}DiscTransferS;


/*******************************************************************
    DVD DEFINES
*******************************************************************/
/*-----------------------------------------------------------------
    DVD BLOCK DEFINE
------------------------------------------------------------------*/
#define SECTORS_PER_DVDBLOCK        0x10
#define SECTORS_PER_ADIPWORD        0x04
#define ONE_DVD_BLOCK               0x10
#define DVD_PIPO_OFFSET             0x12E0     /* 2080 + 2752 = 4832 = 0x12E0 */
#define DVD_PIPO_PTR_RESOLUTION     0x1CA0     /* 7328 byte = 0x1CA0 */
#define DVD_ERASURE_PTR_RESOLUTION  0x9C0      /* 2496 byte */
#define DVD_AUX_PTR_RESOLUTION      0x10
#define DVD_DATA_BLK_PTR_RESOLUTION 0x800
#define DVD_LOGICAL_SECTOR_LENGTH   0x00000800
#define DVD_BLOCK_MASK              0xFFFFFFF0
#define LAST_LBA_OF_DVDBLOCK        0x0000000F                  /* Ending nibble of last lba in a DVD block */
#define DVD_PAD_BLOCKS              2                           /* Number of DVD blocks to pad between disc and host ptr */
#define DVD_PAD_SECTORS             (DVD_PAD_BLOCKS * SECTORS_PER_DVDBLOCK)   /* Number of dvd sectors to pad between disc and host ptrs */
#define DVD_START_OF_DATA_ZONE      0x30000

/*-----------------------------------------------------------------
    ERROR CODES
------------------------------------------------------------------*/
#define errReadProtSecWithoutAuthen   BUILD_SENSE(0x05,0x6F,0x03,0x25)

/*-----------------------------------------------------------------
    TIMER VALUES -  Value loaded into 20ms kernel timer
------------------------------------------------------------------*/
#define MSEC_20     1
#define MSEC_40     2
#define MSEC_60     3
#define MSEC_80     4
#define MSEC_100    5
#define MSEC_200    10
#define MSEC_340    17
#define MSEC_500    25
#define SECONDS_4   200

/*-----------------------------------------------------------------
    MEDIA CONTROL SETTINGS FOR REGISTER MEDCTRL

    Bits [2:1]
------------------------------------------------------------------*/
#define MEDIA_IS_DVD_RAM    0x02

/*-----------------------------------------------------------------
    CONFIGURATIONS
------------------------------------------------------------------*/
#define DVD_MIO_TEST_MODE  0x1A

/*-----------------------------------------------------------------
   DVD+R DEFINES
------------------------------------------------------------------*/
#define DVDPLUS_R_MAX_TOC_BLOCKS    256
#define DVDPLUS_R_MAX_SESSIONS      (169)

#define DVDPLUS_R_MAX_TRACKS    ((DVDPLUS_R_MAX_SESSIONS - 1) * 1 + 16)

                // Note: Defined in MMC5 Spec 4.4.6.2.1.3 Sessions
                //       An OPEN session may have at least 1 fragment
                //       and as many as 16 fragments.
                //
                //       Once the session is closed the fragment is
                //       counted as 1 logical fragment within that
                //       CLOSED session.
                //
                //       Calculation of Maximum Number of Tracks =
                //       (Total Tracks Allowed - 1) * (1 Track) + (16 Tracks Maximum in Last Open Session)

#define DVDPLUS_R_MAX_SESSIONS_ARRAY_SIZE   (DVDPLUS_R_MAX_SESSIONS + 1)
                // Actual number of sessions is 191 but
                // for array indexing purposes this value is set to 192
                // Note: Single Layer supports 191 Sessions
                //       Dual Layer supports 127 Sessions
#define DVDPLUS_R_MAX_TRACKS_ARRAY_SIZE     (DVDPLUS_R_MAX_TRACKS +1)



#define DVD_BLOCK_ADDRESS_MASK      0xFFFFFFF0
#define DATA_START_PSN_DVDROM       0x00030000

/*-----------------------------------------------------------------------------------
    Lead In Area
------------------------------------------------------------------------------------*/
#define DVDPLUSR_LEADIN_START_ADDRESS       0x2A480
#define DVDPLUSR_DL_LEADIN_START_ADDRESS    0x2AD00

#define DVDMINUSR_LEADIN_START_ADDRESS       0x22FA0
#define DVDMINUSR_NBCA_LEADIN_START_ADDRESS       0x2DA80
#define DVDMINUSRW_NBCA_LEADIN_START_ADDRESS      0x2D300

#define DVDMINUSR_DL_LEADIN_START_ADDRESS         0x24440
#define DVDMINUSR_DL_NBCA_LEADIN_START_ADDRESS    0x2D5B0

#define DVDRAM_LEADIN_START_ADDRESS               0x31000

#define TEMP_MAXLBA     0x001B46FF  /* For AWG only */

#define TEMP_START_PSN  0x00030000  /* For AWG only */

#define OTP_MASK    bit4    /* See definition of TrackLayerPath in GP_RAM.C */

#define LAYER1_MASK bit5    /* See definition of TrackLayerPath in GP_RAM.C */

/*********************************************************************
   Dependent h files for defined types
*********************************************************************/
#define CPRMAI_LEN      6   /* Number of CPRMAI bytes. */


/*-------------------------------------------------------------------------------------------------------------
    Lead In Defines
--------------------------------------------------------------------------------------------------------------*/
#define START_ADDRESS_OF_CONTROL_DATA       0x0002F200  /* Start of control data in lead-in area */

#define TOTAL_CONTROL_DATA_BLOCKS           192         /* Maximum number of data control blocks. There are 192
                                                           copies of the control block in the lead-in area. If
                                                           the first control data block cannot be read attempts
                                                           will be made to read the next sequential control
                                                           data block.  */
#define TOTAL_BORDER_IN_BLOCKS              5

#define DISC_KEY_RELATIVE_ADDRESS           0x1000      /* Relative address for sector 2. Used to check for a disc key */

#define CPSTY_CHECK_LEN                     5           /* Number of bytes to check and compare in the disc key area
                                                           for copy protection verification */
#define AUX_SECTOR2_OFFSET                  0x20        /* Relative address of sector 2 aux information within aux block */
/*---------------------------------------------
    Copy and Region control offsets
----------------------------------------------*/

#define CPR_MAI_OFFSET                      6           /* Offset in bytes for copy protection information in aux block */
#if (ENABLE_CPRM && BCA_ENABLE)
#define MKB_PACKS_OFFSET                    9           /* Offset in bytes for total MKB Packs Used in aux block */
#endif
#define RMA_OFFSET                          11          /* Offset in bytes for Region control information in aux block  */

/*---------------------------------------------
    Physical format information offsets
----------------------------------------------*/
#define BOOK_TYPE_AND_VERSION_OFFSET        0
#define BOOK_TYPE_MASK              0xF0
#define PART_VERSION_MASK           0x0F

#define DISC_SIZE_AND_READ_RATE_OFFSET      1
#define DISC_SIZE_MASK              0xF0
#define MAX_READOUT_RATE_MASK       0x0F

#define DISC_STRUCTURE_OFFSET               2
#define NUMBER_OF_LAYERS_MASK       0x60
#define TRACK_PATH_MASK             0x10
#define LAYER_TYPE_MASK             0x0F

#define RECORDED_DENSITY_OFFSET             3
#define LINEAR_DENSITY_MASK         0xF0
#define TRACK_DENSITY_MASK          0x0F

#define START_PSN_OF_DATA_AREA_OFFSET       4
#define END_PSN_OF_DATA_AREA_OFFSET         8
#define END_PSN_LAYER0_OFFSET               12

#define DESCRIPTOR_FLAGS_OFFSET             16
#define BCA_BIT                     0x80
#define VCPS_BIT                    0x40
#define VCPS_IN_BZ2_BIT             0x20

#define PlusR_Start_of_session_offset     248
#define PlusR_End_of_session_offset       252

#define MinusR_Current_BorderOut_offset     32
#define MinusR_Next_BorderIn_offset     36

#define Minus_Extended_Embossed_information  512


/*---------------------------------------------
    DVD +R/-R common defines
----------------------------------------------*/
#define DVD_REFERENCE_ZONE_ADDR       0x2F000
#define DVD_BUFFER_ZONE_1_ADDR        0x2F020
#define DVD_CONTROL_ZONE_ADDR         0x2F200
#define DVD_BUFFER_ZONE_2_ADDR        0x2FE00

/*---------------------------------------------
    DVD +R TOC zone defines
----------------------------------------------*/
#define DVDPLUSR_TOC_START_ADDR     0x29480     // Start address of TOC Zone
#define DVDPLUSR_TOC_END_ADDR       0x2A070     // Last ecc block address of the TOC block area
#define DVDPLUSR_MAX_SESSIONS       191         // Maximum number of sessions for DVD +R
#define DVDTOC_DESCRIPTOR           0x544F4300  // Descriptor code for TOC block
#define DVDTOC_ITEM_DESCRIPTOR      0x55434900  // Descriptor code for TOC item left shifted by 8
#define ITEM_DESCRIPTOR_MASK        0xFFFFFF00  // Mask used in testing presence of item descriptor
#define TOC_ITEM_OFFSET             0x10        // Offset in 4 byte increments into TOC block for first toc item
#define TOC_REPEAT_SECTOR_OFFSET    0x800       // Offset in 4 byte increments into TOC block for duplicate toc info
#define SESSION_STATUS_OFFSET       3           // Offset for Status of session in TOC item
#define DVDPLUSR_RAI_START          0x2A080L    // Start address of recording indicator area
#define DVDPLUSR_RAI_END            0x2A47FL    // Last sector of recording indicator area
#define DVD_RAI_DATA_REGION_SIZE    0x2800      // 640 ECC blocks (10240 sectors) per RAI data region

/*---------------------------------------------
    DVD +R DL TOC zone defines
----------------------------------------------*/
#define DVDPLUSR_DL_TOC_START_ADDR  0x29D00     // Start address of TOC Zone
#define DVDPLUSR_DL_TOC_END_ADDR    0x2A4F0     // Last ecc block address of the TOC block area
#define DVDPLUSR_DL_MAX_SESSIONS    127         // Maximum number of sessions for DVD +R DL
#define DVDPLUSR_DL_RAI_START       0x2A500L    // Start address of recording indicator area
#define DVDPLUSR_DL_RAI_END         0x2ACFFL    // Last sector of recording indicator area
#define TOC_MAX_PSN_OFFSET          41          // Offset in 1 byte increments into TOC block for actual last psn of Data Zone
#define TOC_END_L0_PSN_OFFSET       45          // Offset in 1 byte increments into TOC block for actual last psn of Data Zone on Layer 0

/*---------------------------------------------
    DVD +R SDCB defines
----------------------------------------------*/
#define SDCB_DESCRIPTOR             0x53444300  // Descriptor for SDCB in DVD +R multi session disc
#define SDCB_SESSION_NUM_OFFSET     41          // offset in bytes from start of SDCB (low byte)
#define SDCB_SESSION_ITEM_OFFSET    128         // offset in bytes from start of SDCB
#define SDCB_DISC_ID_OFFSET         64          // offset in bytes from start of SDCB
#define SDCB_REPEAT_SECTOR_OFFSET   0x2000      // offset in bytes into SDCB for repeat information
#define SDCB_SESSION_ITEM_SIZE      16          // size in bytes
#define SDCB_DISC_ID_SIZE           32          // size in bytes
#define SDCB_FRAGMENT_NUMBER_OFFSET 3           // offset in bytes into Fragment item
#define SDCB_FRAGMENT_START_OFFSET  5           // offset in bytes into Fragment item
#define SDCB_FRAGMENT_END_OFFSET    8           // offset in bytes into Fragment item

/*---------------------------------------------
    DVD +R defines
----------------------------------------------*/
#define DVDPLUSR_GUARD_ZONE_1_ADDR          0x2A480
#define DVDPLUSR_RESERVED_ZONE_1_ADDR       0x2DE80
#define DVDPLUSR_RESERVED_ZONE_2_ADDR       0x2EE80
#define DVDPLUSR_INNER_DISCID_ZONE_ADDR     0x2EEC0
#define DVDPLUSR_RESERVED_ZONE_3_ADDR       0x2EFC0

/*---------------------------------------------
    DVD +RW defines
----------------------------------------------*/
#define DVDPLUSRW_GUARD_ZONE_1_ADDR         0x2DC80
#define DVDPLUSRW_RESERVED_ZONE_1_ADDR      0x2DE80
#define DVDPLUSRW_INNER_DISCID_ZONE_ADDR    0x2EEC0
#define DVDPLUSRW_RESERVED_ZONE_3_ADDR      0x2EFC0

/*---------------------------------------------
    DVD +R DL defines
----------------------------------------------*/
#define DVDPLUSRDL_GUARD_ZONE_1_ADDR        0x2AD00

/*---------------------------------------------
    DVD-R/-RW L.I defines
----------------------------------------------*/

#define DVDMINUSR_INITIAL_ZONE_START_ADDR        0x22FA0
#define DVDMINUSRDL_INITIAL_ZONE_START_ADDR      0x24440  //DVD-R DL
#define DVDMINUSR_BUFFER_ZONE_0_START_ADDR       0x2E200
#define DVDMINUSR_PHYSICAL_ZONE_START_ADDR       0x2E400
#define DVDMINUSR_PHYSICAL_ZONE_MIDDLE_ADDR      0x2EA00
#define DVDMINUSR_REFERENCE_ZONE_START_ADDR      0x2F000
#define DVDMINUSR_BUFFER_ZONE_1_START_ADDR       0x2F020
#define DVDMINUSR_CONTROL_ZONE_START_ADDR        0x2F200
#define DVDMINUSR_EXTRA_BORDER_ZONE_START_ADDR   0x2FE00
#define DVDMINUSR_EXTRA_BORDER_OUT_START_ADDR    0x2FE10
#define DVDMINUSR_EXTRA_BORDER_IN_START_ADDR     0x2FFA0

#define DVDMINUSRW_SYSTEM_CODE_ADDR              0x2D600
#define DVDMINUSRW_BUFFER_ZONE_2_START_ADDR      0x2FE00


/*---------------------------------------------
    DVD -R/-RW RMA zone defines
----------------------------------------------*/
#define DVDMINUS_PCA_END_ADDR       0x0203A0
#define DVDMINUS_RMA_INIT_ADDR      0x0203B0
#define DVDMINUS_RMA_START_ADDR     0x0203C0
#define DVDMINUS_RMA_SET_START_ADDR 0x0203D0
#define DVDMINUS_RMA_END_ADDR       0x022F80
#define DVDMINUS_RMA_SIZE           0x2BD0
#define DVDMINUS_RMA_SEGMENT_SIZE   0x8C0
#define DVDMINUSRW_RMA_FORMAT2_LEN  0x50     //5 RMD Block
#define FORMAT1_RMD_WRITTEN_BLOCK   0x20     //write 2 Block each time
/*---------------------------------------------
    DVD -R DL RMA zone defines
----------------------------------------------*/
#define DVDMINUSDL_PCA_END_ADDR_L0       0x021CC0
#define DVDMINUSDL_RMA_INIT_ADDR_L0      0x021CD0
#define DVDMINUSDL_RMA_START_ADDR_L0     0x021CE0
#define DVDMINUSDL_RMA_SET_START_ADDR_L0 0x021CF0
#define DVDMINUSDL_RMA_END_ADDR_L0       0x024420

#define DVDMINUSDL_PCA_END_ADDR_L1       0xFDA5B0
#define DVDMINUSDL_RMA_INIT_ADDR_L1      0xFDA5C0
#define DVDMINUSDL_RMA_START_ADDR_L1     0xFDA5D0
#define DVDMINUSDL_RMA_SET_START_ADDR_L1 0xFDA5E0
#define DVDMINUSDL_RMA_END_ADDR_L1       0xFDB190
/*---------------------------------------------
    DVD -RW DL RMA zone defines
----------------------------------------------*/
#define DVDMINUSRWDL_RMD_PAD_BLOCKS      68

/*---------------------------------------------
    DVD -R DL LeadOut zone defines
----------------------------------------------*/
#define DVDMINUSDL_LEADOUT_END_ADDR      0xFD97DF

/*---------------------------------------------
    DVD -R/-RW RMD offset defines
----------------------------------------------*/
#define DVDMINUS_RMD_LINKING_LOSS_OFFSET    0x0000
#define DVDMINUS_RMD_FIELD0_OFFSET          0x0800
#define DVDMINUS_RMD_FIELD1_OFFSET          0x1000
#define DVDMINUS_RMD_FIELD2_OFFSET          0x1800
#define DVDMINUS_RMD_FIELD3_OFFSET          0x2000
#define DVDMINUS_RMD_FIELD4_OFFSET          0x2800
#define DVDMINUS_RMD_FIELD5_OFFSET          0x3000
#define DVDMINUS_RMD_FIELD6_OFFSET          0x3800
#define DVDMINUS_RMD_FIELD7_OFFSET          0x4000
#define DVDMINUS_RMD_FIELD8_OFFSET          0x4800
#define DVDMINUS_RMD_FIELD9_OFFSET          0x5000
#define DVDMINUS_RMD_FIELD10_OFFSET         0x5800
#define DVDMINUS_RMD_FIELD11_OFFSET         0x6000
#define DVDMINUS_RMD_FIELD12_OFFSET         0x6800
#define DVDMINUS_RMD_FIELD13_OFFSET         0x7000
#define DVDMINUS_RMD_FIELD14_OFFSET         0x7800

/*---------------------------------------------
    DVD-R Border defines
-----------------------------------------------*/
#define Truncated_BorderOut_Size        0x250
#define MINIMUM_FIRST_BORDEROUT_START   0x3FF00

/*---------------------------------------------
    DVD +R Intro/Closure zone defines (all sizes are in sectors)
----------------------------------------------*/
#define DVD_INTRO_ZONE_SIZE             0x400
#define DVD_CLOSURE_ZONE_SIZE           0x400
#define DVD_BUFFER_ZONEA_SIZE           0x40
#define DVD_BUFFER_ZONEB_SIZE           0x40
#define DVD_BUFFER_ZONEC_SIZE           0x300
#define DVD_INNER_SESSION_ID_ZONE_SIZE  0x100
#define DVD_OUTER_SESSION_ID_ZONE_SIZE  0x100

/*---------------------------------------------
    DVD +R/+R DL MISC DEFINES
----------------------------------------------*/
#define DVD_FRAGMENT_RUN_IN_BLOCK       0x10        // size is in sectors
#define DVD_PLUSR_DATAZONE_END          0x26053FL   // Last maximum PSN in the data zone
#define DVD_PLUSR_DL_DATAZONE_END       0xFCFFFFL   // Last maximum PSN in the data zone for DL discs
#define DVD_PLUSRW_DATAZONE_SIZE        0x230540L   // 12cm DVD+RW data zone size
#define DVD_PLUS8cm_DATAZONE_END        0x0DE72FL   // Last maximum PSN in the data zone

#define DVD_PLUSRW_DATAZONE_END_L0      0x22D7FFL   // 12cm DVD+RW9 layer 0 data zone end
#define DVD_PLUSRW_DATAZONE_END_L1      0xFCFFFFL   // 12cm DVD+RW9 layer 1 data zone end
#define DVD_PLUSRW8cm_DATAZONE_END_L0   0x0CE9FFL   // 8cm DVD+RW9 layer 0 data zone end

/*---------------------------------------------
    DVD +RW FDCB defines
----------------------------------------------*/
typedef enum
{
    eDCB_None,  /* None DCB */
    eFDCB,      /* DVD+RW5 FDCB */
    eWDCB,      /* DVD+RW WDCB */
    eFDCB_L0,   /* DVD+RW9 layer0 FDCB */
    eFDCB_L1    /* DVD+RW9 layer1 FDCB */
} DCBTypeE;     /* Type of Disc Control Block Enumeration */

#define FDCB_DESCRIPTOR             0x46444300  // Descriptor for FDCB in DVD +RW disc
#define FDCB_DESCRIPTOR_L0          0x46443000  // Descriptor for FDCB in DVD +RW9 disc layer 0
#define FDCB_DESCRIPTOR_L1          0x46443100  // Descriptor for FDCB in DVD +RW9 disc layer 1
#define FDCB_UPDATE_COUNT           40          // offset of FDCB update count
#define FDCB_FORMAT_STATUS          44          // offset of FDCB formatting status
#define FDCB_VERIFY_STATUS          45          // offset of FDCB verification status
#define FDCB_RECORD_STATUS          46          // offset of FDCB recording status
#define FDCB_LAST_WRITTEN_ADDR      48          // offset of FDCB last written address
#define FDCB_LAST_VERIFIED_ADDR     52          // offset of FDCB last verified address
#define FDCB_BITMAP_START_ADDR      56          // offset of FDCB bit map start address
#define FDCB_BITMAP_LENGTH          60          // offset of FDCB bit map length
#define FDCB_DISC_ID                64          // offset of FDCB disc ID
#define FDCB_LIST_OF_DCBS           128         // offset of FDCB list DCBs

/*---------------------------------------------
    DVD +RW WDCB defines
----------------------------------------------*/
#define WDCB_DESCRIPTOR             0x57444300  // Descriptor for WDCB in DVD +RW disc
#define WDCB_UPDATE_COUNT           40          // offset of WDCB update count
#define WDCB_WRITE_PROTECTION       44          // offset of WDCB write protect actions
#define WDCB_PASSWORD               64          // offset of WDCB password

#if (DVD_ZONE_TEST == TRUE)
struct dvdZoneTestS
{
    ULONG   PIUCount;
    ULONG   PICCount;
    ULONG   POUCount;
    ULONG   POCCount;
    USHORT  BlockCount;
    BYTE    TestComplete;
};
#endif

//------------------------------------------------------------------------------
// Defined masks and values for physical format information fields
// These values are in accordance with the DVD specifications
//------------------------------------------------------------------------------
//----------------------------------------------
// Disc Category (Book Type)
//----------------------------------------------
#define DVDHD_TYPE_MASK       0xF0
#define DVD_ROM             0x00
#define DVD_RAM             0x10
#define DVD_MINUS_R         0x20
#define DVD_MINUS_RW        0x30
#define HDDVD_ROM           0x40
#define HDDVD_RAM           0x50
#define HDDVD_R             0x60
#define HDDVD_RW            0x70
//#define HDDVD_China         0x80 //TBD?
#define DVD_PLUS_RW         0x90
#define DVD_PLUS_R          0xA0
#define DVD_PLUS_RW_DUAL    0xD0
#define DVD_PLUS_R_DUAL     0xE0

//----------------------------------------------
// VERSION NUMBERS
// Keep association with format
//----------------------------------------------
#define DVDHD_VERSION_MASK       0x0F

// VERSIONS
#define DVD_VER_0_9            0x00
#define DVD_VER_1_0            0x01
#define DVD_VER_1_1            0x02
#define DVD_VER_1_2            0x03
#if ENABLE_HDDVD
#define HD_VER_Specified       0x03
#endif
#define DVD_VER_1_9            0x04
#define DVD_VER_2_0            0x05
#define DVD_VER_2_1            0x06

// Extended Part VERSIONS
#define DVD_VER_2_X            0x21
#define DVD_VER_2_9            0x29
#define DVD_VER_3_0            0x30

// UNIQUE DVD +R VERSIONS
#define DVDPLUSR_VER_1_3       0x01
#define DVDPLUSR_DL_VER_1_0    0x01    // (DL) Dual Layer

// UNIQUE DVD +RW VERSIONS
#define DVDPLUSRW_VER_1_2      0x02


//----------------------------------------------
// Disc size and Maximum transfer rate
//----------------------------------------------
#define DVDHD_DISC_SIZE_MASK  0xF0
#define DVDHD_12CM_DISC       0x00
#define DVDHD_8CM_DISC        0x10

#define DVDHD_XFER_RATE_MASK          0x0F
#define DVDHD_MAXRATE_2_52Mbps        0x00
#define DVDHD_MAXRATE_5_04Mbps        0x01
#define DVDHD_MAXRATE_10_08Mbps       0x02
#if ENABLE_HDDVD
#define HD_MAXRATE_20_16Mbps          0x03
#define HD_MAXRATE_30_32Mbps          0x04
#endif
#define DVDHD_MAXRATE_NOT_SPECIFICED  0x0F

//----------------------------------------------
// Disc Structure
//----------------------------------------------

// Number of layers and track path
#define DVDHD_LAYER_MASK      0x60
#define SINGLE_LAYER          0x00
#define DUAL_LAYER            0x01

#define DVDHD_TRACK_PATH_MASK 0x10
#define DVDHD_PTP_DISC        0x00
#define DVDHD_OTP_DISC        0x10

//Layer type
#define DVDHD_LAYER_TYPE_MASK     0x0F
#define DVDHD_USER_AREA_RW        0x04    // ReWriteable User Data Area
#define DVDHD_USER_AREA_R         0x02    // Recordable User Data Area
#define DVDHD_USER_AREA_EMBOSSED  0x01    // Embossed User Data Area

//----------------------------------------------
// Recording Density
// There are different linear/track densities
// depending upon version numbers for some
// formats.
//----------------------------------------------

// Average channel bit length (linear density in 1/1000 um/bit )
#define DVDHD_LINEAR_DENSITY_MASK   0xF0
#define DVD_LDENSITY_133UM          0x00
#define DVD_LDENSITY_147UM          0x01
#if ENABLE_HDDVD
#define HD_LDENSITY_267UM           0x00
#define HD_LDENSITY_293UM           0x01
#define HD_LDENSITY_409_TO_435UM    0x02
#define HD_LDENSITY_280_TO_295UM    0x04
#define HD_LDENSITY_153UM           0x05
#define HD_LDENSITY_130_TO_140UM    0x06
#endif
// Recording Density - defined in DVD-R ver 2.01
#define DVD_LDENSITY_267UM          0x00
#define DVD_LDENSITY_293UM          0x01
#define DVD_LDENSITY_409_TO_435UM   0x02

// Average track pitch (track density in 1/100 um/track)
#define DVDHD_TRACK_DENSITY_MASK      0x0F
#define DVDHD_TDENSITY_74UM           0x00
#define DVDHD_TDENSITY_80UM           0x01
#if ENABLE_HDDVD
#define HD_TDENSITY_61_5UM            0x02
#define HD_TDENSITY_40UM              0x03
#define HD_TDENSITY_34UM              0x04
#endif
//----------------------------------------------
// Flag bit definitions
//----------------------------------------------
#define DVDHD_BCA_MASK    0x80
#define DVDHD_NO_BCA      0x00
#define DVDHD_BCA_FLAG    0x80
#define DVD_VCPS_FLAG     0x40

#if ENABLE_HDDVD
//----------------------------------------------
// Externed Part version
//----------------------------------------------
#define HD_MAJOR_DIGIT_MASK    0xF0
#define CBHD_Major_Ver         0xA0
#define HD_MINOR_DIGIT_MASK    0x0F
#define CBHD_Minor_Ver         0x00

//----------------------------------------------
// Maximum reading speed
//----------------------------------------------
#define HD_MAX_READ_SPEED_MASK    0xFF
#define HD_MAX_READ_SPEED_1X      0x0A

//----------------------------------------------
// Layer format table
//----------------------------------------------
#define HD_LAYER1_FORMAT_MASK    0xF0
#define HD_LAYER0_FORMAT_MASK    0x0F
#endif

//Read DVD-RW CPRM video Format3 RMD for FWtester2  Item7 DVD CPRM Feature Check
#define LLALength                0x10                //Link Lost Area Lenght
#define TBOLength                37*ONE_DVD_BLOCK             //Truncated Boardout Area Length
#endif
