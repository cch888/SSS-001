/****************************************************************************
*         (c) Copyright 2003 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   oem_tbl.h
*
* DESCRIPTION
*
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __OEM_TBL_H__
#define __OEM_TBL_H__

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globtype.h"

#if (EN_Persistent_table_8 == 1)
//Size coupled to revision
#define SIZE_PERSISTENT_TABLE           1024    // 0x400
#define PersistentTable_Revision        8       // When we change any field in the Persistent Table,
                                                // we need to change the revision here to make sure the
                                                // correct information is updated to the EEPROM.
#else
#define SIZE_PERSISTENT_TABLE           512
#define PersistentTable_Revision        7       // When we change any field in the Persistent Table,
                                                // we need to change the revision here to make sure the
                                                // correct information is updated to the EEPROM.
#endif/* (EN_Persistent_table_8 == 1) */

extern BYTE const PersistentTable[SIZE_PERSISTENT_TABLE];

/********************************************/
/***           Persistent Table           ***/
/********************************************/
//Common Area
#define VenderID_LEN                        8
#define FirmWareRev_LEN                     4
#define APRev_LEN                           4
#define ConfigRev_LEN                       4
#define ProductID_LEN                       16
#define SerialNumber_LEN                    16
#define ProduceLineNumber_LEN               8
#define CustomerSerialNumber_LEN            8
#define PersistentTableRev_LEN              4
#define Power_Meter_LEN                     1
#define Disc_Category_LEN                   1

#define CD_CAL_FBDAC_LEN                    4
#define CD_CAL_FEOBG_LEN                    4
#define CD_CAL_SPP2BG_LEN                   4
#define CD_CAL_RRFG_LEN                     4
#define CD_CAL_BETA_OFFSET_LEN              4
#define CD_BIAS_POWER_LEN                   4
#define CD_VWDC2VRDC_LEN                    4
#define CD_VWDC2VWDC1_LEN                   4
#define CD_OFFSET_DAC_LEN                   16
#define CD_SLOPE_DAC_LEN                    16
#define CD_SLOPE_ADC_VRPWR_LEN              16
#define CD_SLOPE_ADC_VWPWR_LEN              16
#define CD_FOC_SENS_LEN                     4

#define DV_CAL_FBDAC0_LEN                   4
#define DV_CAL_FEOBG0_LEN                   4
#define DV_CAL_SPP2BG0_LEN                  4
#define DV_CAL_SPP2BG1_LEN                  4
#define DV_CAL_FBDAC1_LEN                   4
#define DV_CAL_FEOBG1_LEN                   4
#define DV_CAL_PLUS_RRFG_LEN                4
#define DV_CAL_PLUS_BETA_OFFSET_LEN         4
#define DV_CAL_MINUS_RRFG_LEN               4
#define DV_CAL_MINUS_BETA_OFFSET_LEN        4
#define DV_CAL_TILT_DAC0_LEN                4
#define DV_CAL_TILT_DAC1_LEN                4
#define DV_CAL_TILT_DAC2_LEN                4
#define DV_CAL_TILT_DAC3_LEN                4
#define DV_CAL_TILT_DAC4_LEN                4
#define DV_BP2DIFFHOLDDAC_LEN               8
#define DV_BIAS_POWER_LEN                   4
#define DV_VWDC2VRDC_LEN                    4
#define DV_VWDC2VWDC1_LEN                   4
#define DV_VWDC2VWDC2_LEN                   4

#define DV_OFFSET_DAC_LEN                   16
#define DV_SLOPE_DAC_LEN                    16
#define DV_SLOPE_ADC_VRPWR_LEN              16
#define DV_SLOPE_ADC_VWPWR_LEN              16
#define DV_FOC_SENS_LEN                     4

#define HD_CAL_FBDAC0_LEN                   4
#define HD_CAL_FEOBG0_LEN                   4
#define HD_CAL_SPP2BG0_LEN                  4
#define HD_CAL_SPP2BG1_LEN                  4
#define HD_CAL_FBDAC1_LEN                   4
#define HD_CAL_FEOBG1_LEN                   4
#define HD_CAL_RRFG_LEN                     4
#define HD_CAL_BETA_OFFSET_LEN              4
#define HD_CAL_TILT_DAC0_LEN                4
#define HD_CAL_TILT_DAC1_LEN                4
#define HD_CAL_TILT_DAC2_LEN                4
#define HD_CAL_TILT_DAC3_LEN                4
#define HD_CAL_TILT_DAC4_LEN                4
#define HD_BP2DIFFHOLDDAC_LEN               8
#define HD_BIAS_POWER_LEN                   4
#define HD_VWDC2VRDC_LEN                    4
#define HD_VWDC2VWDC1_LEN                   4
#define HD_VWDC2VWDC2_LEN                   4

#define HD_OFFSET_DAC_LEN                  16
#define HD_SLOPE_DAC_LEN                   16
#define HD_SLOPE_ADC_VRPWR_LEN             16
#define HD_SLOPE_ADC_VWPWR_LEN             16
#define HD_FOC_SENS_LEN                     4

#define BD_CAL_FBDAC0_LEN                   4
#define BD_CAL_FEOBG0_LEN                   4
#define BD_CAL_SPP2BG0_LEN                  4
#define BD_CAL_SPP2BG1_LEN                  4
#define BD_CAL_FBDAC1_LEN                   4
#define BD_CAL_FEOBG1_LEN                   4
#define BD_CAL_RRFG_LEN                     4
#define BD_CAL_BETA_OFFSET_LEN              4
#define BD_CAL_TILT_DAC0_LEN                4
#define BD_CAL_TILT_DAC1_LEN                4
#define BD_CAL_TILT_DAC2_LEN                4
#define BD_CAL_TILT_DAC3_LEN                4
#define BD_CAL_TILT_DAC4_LEN                4
#define BD_BP2DIFFHOLDDAC_LEN               8
#define BD_BIAS_POWER_LEN                   4
#define BD_VWDC2VRDC_LEN                    4
#define BD_VWDC2VWDC1_LEN                   4
#define BD_VWDC2VWDC2_LEN                   4

#define BD_OFFSET_DAC_LEN                   16
#define BD_SLOPE_DAC_LEN                    16
#define BD_SLOPE_ADC_VRPWR_LEN              16
#define BD_SLOPE_ADC_VWPWR_LEN              16
#define BD_FOC_SENS_LEN                     4



#define VenderID_ADDR                      0x000L  /*  8 Bytes */
#define FirmWareRev_ADDR                   0x008L  /*  8 Bytes */
#define APRev_ADDR                         0x00CL  /*  8 Bytes */
#define ProductID_ADDR                     0x010L  /* 16 Bytes */
#define SerialNumber_ADDR                  0x020L  /* 16 Bytes */
#define ProduceLineNumber_ADDR             0x030L  /*  8 Bytes */
#define CustomerSerialNumber_ADDR          0x038L  /*  8 Bytes */
#define PersistentTable_ID_ADDR            0x040L  /*  4 Bytes */
#define ConfigRev_ADDR                     0x044L  /*  4 Bytes */
#define RPC_STATE_OFFSET                   0x048L  /*  1 Bytes */
#define RPC_COUNT_OFFSET                   0x049L  /*  1 Bytes */
#define RPC_REGION_OFFSET                  0x04AL  /*  1 Bytes */
#define RPC_VENDOR_RESET_OFFSET            0x04BL  /*  1 Bytes */
#define Power_Meter_ADDR                   0x04EL  /*  1 Bytes */
#define Disc_Category_ADDR                 0x04FL  /*  1 Bytes */

/* CD Offline Calibration Parameter */
#define CD_CAL_FBDAC_ADDR                  0x050L  /*  2 Bytes */
#define CD_CAL_FEOBG_ADDR                  0x054L  /*  2 Bytes */
#define CD_CAL_SPP2BG_ADDR                 0x058L  /*  2 Bytes */
#define CD_CAL_FEBG_ID_ADDR                0x05CL  /*  4 Bytes */
#define CD_CAL_RRFG_ADDR                   0x060L  /*  2 Bytes */
#define CD_CAL_BETA_OFFSET_ADDR            0x064L  /*  2 Bytes */
#define CD_CAL_RRFG_ID_ADDR                0x068L  /*  4 Bytes */
#define CD_CAL_BETA_OFFSET_ID_ADDR         0x06CL  /*  4 Bytes */
#define CD_BIAS_POWER_ADDR                 0x070L  /*  2 Bytes */
#define CD_VWDC2VRDC_ADDR                  0x074L  /*  2 Bytes */

/* CD Power Calibration Parameter */
#define CD_OFFSET_DAC_THRESHOLD_ADDR_ON    0x078L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_DAC_THRESHOLD_ADDR_ON     0x07CL  /* 4 Bytes_FLOAT */
#define CD_OFFSET_DAC_THRESHOLD_ADDR       0x080L  /* 4 Bytes_FLOAT */
#define CD_OFFSET_DAC_0_ADDR               0x084L  /* 4 Bytes_FLOAT */
#define CD_OFFSET_DAC_1_ADDR               0x088L  /* 4 Bytes_FLOAT */
#define CD_OFFSET_DAC_2_ADDR               0x08CL  /* 4 Bytes_FLOAT */
#define CD_SLOPE_DAC_THRESHOLD_ADDR        0x090L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_DAC_0_ADDR                0x094L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_DAC_1_ADDR                0x098L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_DAC_2_ADDR                0x09CL  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VRPOWER_G1X_ADDR      0x0A0L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VRPOWER_G5X_ADDR      0x0A4L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VRPOWER_G10X_ADDR     0x0A8L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VRPOWER_G25X_ADDR     0x0ACL  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VWPOWER_LG_ADDR       0x0B0L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VWPOWER_MG_ADDR       0x0B4L  /* 4 Bytes_FLOAT */
#define CD_SLOPE_ADC_VWPOWER_HG_ADDR       0x0B8L  /* 4 Bytes_FLOAT */
#define CD_CHECK_SUM_ADDR                  0x0BCL  /*  2 Bytes */
#define CD_CAL_PWR_START_ADDR              CD_OFFSET_DAC_THRESHOLD_ADDR_ON
#define CD_CAL_PWR_END_ADDR                CD_CHECK_SUM_ADDR
#define CD_FOC_SENS_ADDR                   0x0C0L

/* DVD Offline Calibration Parameter */
#define DV_CAL_FBDAC0_ADDR                 0x0E0L  /*  2 Bytes */
#define DV_CAL_FEOBG0_ADDR                 0x0E4L  /*  2 Bytes */
#define DV_CAL_SPP2BG0_ADDR                0x0E8L  /*  2 Bytes */
#define DV_CAL_FEBG0_ID_ADDR               0x0ECL  /*  4 Bytes */
#define DV_CAL_FBDAC1_ADDR                 0x0F0L  /*  2 Bytes */
#define DV_CAL_FEOBG1_ADDR                 0x0F4L  /*  2 Bytes */
#define DV_CAL_SPP2BG1_ADDR                0x0F8L  /*  2 Bytes */
#define DV_CAL_FEBG1_ID_ADDR               0x0FCL  /*  4 Bytes */
#define DV_CAL_PLUS_RRFG_ADDR              0x100L  /*  4 Bytes */
#define DV_CAL_PLUS_BETA_OFFSET_ADDR       0x104L  /*  4 Bytes */
#define DV_CAL_PLUS_RRFG_ID_ADDR           0x108L  /*  4 Bytes */
#define DV_CAL_PLUS_BETA_OFFSET_ID_ADDR    0x10CL  /*  4 Bytes */
#define DV_CAL_MINUS_RRFG_ADDR             0x110L  /*  4 Bytes */
#define DV_CAL_MINUS_BETA_OFFSET_ADDR      0x114L  /*  4 Bytes */
#define DV_CAL_MINUS_RRFG_ID_ADDR          0x118L  /*  4 Bytes */
#define DV_CAL_MINUS_BETA_OFFSET_ID_ADDR   0x11CL  /*  4 Bytes */
#define DV_CAL_TILT_DAC0_ADDR              0x120L  /*  4 Bytes */
#define DV_CAL_TILT_DAC1_ADDR              0x124L  /*  4 Bytes */
#define DV_CAL_TILT_DAC2_ADDR              0x128L  /*  4 Bytes */
#define DV_CAL_TILT_DAC3_ADDR              0x12CL  /*  4 Bytes */
#define DV_CAL_TILT_DAC4_ADDR              0x130L  /*  4 Bytes */
#define DV_CAL_TILT_NUM_ADDR               0x134L  /*  4 Bytes */
#define DV_BP2DIFFHOLDDAC_OFFSET_ADDR      0x138L  /*  4 Bytes */
#define DV_BP2DIFFHOLDDAC_SLOPE_ADDR       0x13CL  /*  4 Bytes */
#define DV_BIAS_POWER_ADDR                 0x140L   /*  4 Bytes */
#define DV_VWDC2VRDC_ADDR                  0x144L   /*  4 Bytes */
/* DVD Power Calibration Parameter */
#define DV_OFFSET_DAC_THRESHOLD_ADDR_ON    0x148L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_DAC_THRESHOLD_ADDR_ON     0x14CL  /* 4 Bytes_FLOAT */
#define DV_OFFSET_DAC_THRESHOLD_ADDR       0x150L  /* 4 Bytes_FLOAT */
#define DV_OFFSET_DAC_0_ADDR               0x154L  /* 4 Bytes_FLOAT */
#define DV_OFFSET_DAC_1_ADDR               0x158L  /* 4 Bytes_FLOAT */
#define DV_OFFSET_DAC_2_ADDR               0x15CL  /* 4 Bytes_FLOAT */
#define DV_SLOPE_DAC_THRESHOLD_ADDR        0x160L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_DAC_0_ADDR                0x164L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_DAC_1_ADDR                0x168L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_DAC_2_ADDR                0x16CL  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VRPOWER_G1X_ADDR      0x170L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VRPOWER_G5X_ADDR      0x174L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VRPOWER_G10X_ADDR     0x178L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VRPOWER_G25X_ADDR     0x17CL  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VWPOWER_LG_ADDR       0x180L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VWPOWER_MG_ADDR       0x184L  /* 4 Bytes_FLOAT */
#define DV_SLOPE_ADC_VWPOWER_HG_ADDR       0x188L  /* 4 Bytes_FLOAT */
#define DV_CHECK_SUM_ADDR                  0x18CL  /* 4 Bytes */
#define DV_CAL_PWR_START_ADDR              DV_OFFSET_DAC_THRESHOLD_ADDR_ON
#define DV_CAL_PWR_END_ADDR                DV_CHECK_SUM_ADDR
#define DV_FOC_SENS_ADDR                   0x190L
#if (DEBUG_LASER_OPEN_LOOP == 1)||(DEBUG_LASER_CLOSE_LOOP == 1)
//Temporarily, until lasercontrol works
#define DV_OPEN_LOOP_THRESHOLD             0x194L  /* 4 Bytes */
#endif

/* HDDVD Offline Calibration Parameter */
#define HD_CAL_FBDAC0_ADDR                 0x1B0L  /* 4 Bytes */
#define HD_CAL_FEOBG0_ADDR                 0x1B4L  /* 4 Bytes */
#define HD_CAL_SPP2BG0_ADDR                0x1B8L  /* 4 Bytes */
#define HD_CAL_FEBG0_ID_ADDR               0x1BCL  /* 4 Bytes */
#define HD_CAL_FBDAC1_ADDR                 0x1C0L  /* 4 Bytes */
#define HD_CAL_FEOBG1_ADDR                 0x1C4L  /* 4 Bytes */
#define HD_CAL_SPP2BG1_ADDR                0x1C8L  /* 4 Bytes */
#define HD_CAL_FEBG1_ID_ADDR               0x1CCL  /* 4 Bytes */
#define HD_CAL_RRFG_ADDR                   0x1D0L  /* 4 Bytes */
#define HD_CAL_BETA_OFFSET_ADDR            0x1D4L  /* 4 Bytes */
#define HD_CAL_RRFG_ID_ADDR                0x1D8L  /* 4 Bytes */
#define HD_CAL_BETA_OFFSET_ID_ADDR         0x1DCL  /* 4 Bytes */
#define HD_CAL_TILT_DAC0_ADDR              0x1E0L  /* 4 Bytes */
#define HD_CAL_TILT_DAC1_ADDR              0x1E4L  /* 4 Bytes */
#define HD_CAL_TILT_DAC2_ADDR              0x1E8L  /* 4 Bytes */
#define HD_CAL_TILT_DAC3_ADDR              0x1ECL  /* 4 Bytes */
#define HD_CAL_TILT_DAC4_ADDR              0x1F0L  /* 4 Bytes */
#define HD_CAL_TILT_NUM_ADDR               0x1F4L  /* 4 Bytes */
#define HD_BP2DIFFHOLDDAC_OFFSET_ADDR      0x1F8L  /* 4 Bytes */
#define HD_BP2DIFFHOLDDAC_SLOPE_ADDR       0x1FCL  /* 4 Bytes */
#define HD_BIAS_POWER_ADDR                 0x200L  /* 4 Bytes */

#define HD_ND_FILTER_FACTOR                0x204L  /* 4 Bytes_FLOAT */
#define HD_OFFSET_DAC_THRESHOLD_ADDR_ON    0x208L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_DAC_THRESHOLD_ADDR_ON     0x20CL  /* 4 Bytes_FLOAT */
#define HD_OFFSET_DAC_THRESHOLD_ADDR       0x210L  /* 4 Bytes_FLOAT */
#define HD_OFFSET_DAC_0_ADDR               0x214L  /* 4 Bytes_FLOAT */
#define HD_OFFSET_DAC_1_ADDR               0x218L  /* 4 Bytes_FLOAT */
#define HD_OFFSET_DAC_2_ADDR               0x21CL  /* 4 Bytes_FLOAT */
#define HD_SLOPE_DAC_THRESHOLD_ADDR        0x220L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_DAC_0_ADDR                0x224L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_DAC_1_ADDR                0x228L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_DAC_2_ADDR                0x22CL  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VRPOWER_G1X_ADDR      0x230L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VRPOWER_G5X_ADDR      0x234L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VRPOWER_G10X_ADDR     0x238L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VRPOWER_G25X_ADDR     0x23CL  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VWPOWER_LG_ADDR       0x240L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VWPOWER_MG_ADDR       0x244L  /* 4 Bytes_FLOAT */
#define HD_SLOPE_ADC_VWPOWER_HG_ADDR       0x248L  /* 4 Bytes_FLOAT */
#define HD_CHECK_SUM_ADDR                  0x24CL  /* 4 Bytes */
#define HD_CAL_PWR_START_ADDR              HD_ND_FILTER_FACTOR
#define HD_CAL_PWR_END_ADDR                HD_CHECK_SUM_ADDR
#define HD_FOC_SENS_ADDR                   0x250L


/* BD Offline Calibration Parameter */
#define BD_CAL_FBDAC0_ADDR                  0x270L  /* 4 Bytes */
#define BD_CAL_FEOBG0_ADDR                  0x274L  /* 4 Bytes */
#define BD_CAL_SPP2BG0_ADDR                 0x278L  /* 4 Bytes */
#define BD_CAL_FEBG0_ID_ADDR                0x27CL  /* 4 Bytes */
#define BD_CAL_FBDAC1_ADDR                  0x280L  /* 4 Bytes */
#define BD_CAL_FEOBG1_ADDR                  0x284L  /* 4 Bytes */
#define BD_CAL_SPP2BG1_ADDR                 0x288L  /* 4 Bytes */
#define BD_CAL_FEBG1_ID_ADDR                0x28CL  /* 4 Bytes */
#define BD_CAL_RRFG_ADDR                    0x290L  /* 4 Bytes */
#define BD_CAL_BETA_OFFSET_ADDR             0x294L  /* 4 Bytes */
#define BD_CAL_RRFG_ID_ADDR                 0x298L  /* 4 Bytes */
#define BD_CAL_BETA_OFFSET_ID_ADDR          0x29CL  /* 4 Bytes */
#define BD_CAL_TILT_DAC0_ADDR               0x2A0L  /* 4 Bytes */
#define BD_CAL_TILT_DAC1_ADDR               0x2A4L  /* 4 Bytes */
#define BD_CAL_TILT_DAC2_ADDR               0x2A8L  /* 4 Bytes */
#define BD_CAL_TILT_DAC3_ADDR               0x2ACL  /* 4 Bytes */
#define BD_CAL_TILT_DAC4_ADDR               0x2B0L  /* 4 Bytes */
#define BD_CAL_TILT_NUM_ADDR                0x2B4L  /* 4 Bytes */
#define BD_BP2DIFFHOLDDAC_OFFSET_ADDR       0x2B8L  /* 4 Bytes */
#define BD_BP2DIFFHOLDDAC_SLOPE_ADDR        0x2BCL  /* 4 Bytes */
#define BD_BIAS_POWER_ADDR                  0x2C0L  /* 4 Bytes */

#define BD_ND_FILTER_FACTOR                 0x2C4L  /* 4 Bytes_FLOAT */
#define BD_OFFSET_DAC_THRESHOLD_ADDR_ON     0x2C8L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_DAC_THRESHOLD_ADDR_ON      0x2CCL  /* 4 Bytes_FLOAT */
#define BD_OFFSET_DAC_THRESHOLD_ADDR        0x2D0L  /* 4 Bytes_FLOAT */
#define BD_OFFSET_DAC_0_ADDR                0x2D4L  /* 4 Bytes_FLOAT */
#define BD_OFFSET_DAC_1_ADDR                0x2D8L  /* 4 Bytes_FLOAT */
#define BD_OFFSET_DAC_2_ADDR                0x2DCL  /* 4 Bytes_FLOAT */
#define BD_SLOPE_DAC_THRESHOLD_ADDR         0x2E0L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_DAC_0_ADDR                 0x2E4L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_DAC_1_ADDR                 0x2E8L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_DAC_2_ADDR                 0x2ECL  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VRPOWER_G1X_ADDR       0x2F0L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VRPOWER_G5X_ADDR       0x2F4L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VRPOWER_G10X_ADDR      0x2F8L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VRPOWER_G25X_ADDR      0x2FCL  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VWPOWER_LG_ADDR        0x300L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VWPOWER_MG_ADDR        0x304L  /* 4 Bytes_FLOAT */
#define BD_SLOPE_ADC_VWPOWER_HG_ADDR        0x308L  /* 4 Bytes_FLOAT */
#define BD_CHECK_SUM_ADDR                   0x30CL  /* 4 Bytes */
#define BD_CAL_PWR_START_ADDR               BD_ND_FILTER_FACTOR
#define BD_CAL_PWR_END_ADDR                 BD_CHECK_SUM_ADDR
#define BD_FOC_SENS_ADDR                    0x310L  /* 4 Bytes */

#if(SUN710_TRAY_PROFILE == 1 ) || ( EEPROM_TRAY_PROFILE ==1)
#define OEM_TRAY_BASE_ADR                  0x320L
#define OEM_TRAY_PROFILE_LENGTH            0x08L
#endif

#define CD_CHECK_SUM_LO                     (CD_CHECK_SUM_ADDR & 0xFF)
#define CD_CHECK_SUM_HI                     (CD_CHECK_SUM_ADDR & 0xFF00) >> 8
#define DV_CHECK_SUM_LO                     (DV_CHECK_SUM_ADDR & 0xFF)
#define DV_CHECK_SUM_HI                     (DV_CHECK_SUM_ADDR & 0xFF00) >> 8
#define HD_CHECK_SUM_LO                     (HD_CHECK_SUM_ADDR & 0xFF)
#define HD_CHECK_SUM_HI                     (HD_CHECK_SUM_ADDR & 0xFF00) >> 8
#define BD_CHECK_SUM_LO                     (BD_CHECK_SUM_ADDR & 0xFF)
#define BD_CHECK_SUM_HI                     (BD_CHECK_SUM_ADDR & 0xFF00) >> 8

// IDX to Persistent Table
#define IDX_OFFSET_DAC_THRESHOLD_ON (CD_OFFSET_DAC_THRESHOLD_ADDR_ON  - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_OFFSET_DAC_THRESHOLD    (CD_OFFSET_DAC_THRESHOLD_ADDR     - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_OFFSET_DAC_0            (CD_OFFSET_DAC_0_ADDR             - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_OFFSET_DAC_1            (CD_OFFSET_DAC_1_ADDR             - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_OFFSET_DAC_2            (CD_OFFSET_DAC_2_ADDR             - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_DAC_THRESHOLD_ON  (CD_SLOPE_DAC_THRESHOLD_ADDR_ON   - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_DAC_THRESHOLD     (CD_SLOPE_DAC_THRESHOLD_ADDR      - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_DAC_0             (CD_SLOPE_DAC_0_ADDR              - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_DAC_1             (CD_SLOPE_DAC_1_ADDR              - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_DAC_2             (CD_SLOPE_DAC_2_ADDR              - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VRPOWER_G1X   (CD_SLOPE_ADC_VRPOWER_G1X_ADDR    - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VRPOWER_G5X   (CD_SLOPE_ADC_VRPOWER_G5X_ADDR    - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VRPOWER_G10X  (CD_SLOPE_ADC_VRPOWER_G10X_ADDR   - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VRPOWER_G25X  (CD_SLOPE_ADC_VRPOWER_G25X_ADDR   - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VWPOWER_LG    (CD_SLOPE_ADC_VWPOWER_LG_ADDR     - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VWPOWER_MG    (CD_SLOPE_ADC_VWPOWER_MG_ADDR     - CD_OFFSET_DAC_THRESHOLD_ADDR)
#define IDX_SLOPE_ADC_VWPOWER_HG    (CD_SLOPE_ADC_VWPOWER_HG_ADDR     - CD_OFFSET_DAC_THRESHOLD_ADDR)


#if (OPU == KES410A)
/* SONY KES-410 barcode data */
#define SERIAL_0_LEN             4
#define SERIAL_1_LEN             4
#define RAD_SKEW_DVD_LEN         1
#define TAN_SKEW_DVD_LEN         1
#define READPOWER_CD_LEN         1
#define READPOWER_DVD_LEN        1
#define READPOWER_BDL0_LEN       1
#define READPOWER_BDL1_LEN       1
#define HFMF_BD_LEN              1
#define HFMF_SS_BD_LEN           1
#define TILT_SENS_CD_LEN         1
#define TILT_SENS_DVD_LEN        1
#define TILT_SENS_BD_LEN         1
#define HFMP_BD0SL_LEN           1
#define HFMP_BD1DL_LEN           1
#define FFCG_FFC_CD_LEN          1
#define FFCG_FFC_DVD_LEN         1
#define FFCG_FFC_BD_LEN          1
#define IVG_CDDVD_LEN            1    //[6:4]DVD, [2:0]CD
#define IVG_BD_LEN               1    //[2:0]BD
#define RAD_SKEW_BD_LEN          1
#define TAN_SKEW_CD_LEN          1
#define SA_ACT_BD_L0_LEN         2    //32[1:0]&33[7:0] total 10bits
#define SA_ACT_BD_L1_LEN         2    //34[1:0]&35[7:0] total 10bits
#define SONY_410_CHECK_SUM_LEN   2    //44[7:0]&45[7:4] total 12bits

#define SERIAL_0_ADR              0x330
#define SERIAL_1_ADR              0x334
#define RAD_SKEW_DVD_ADR          0x339
#define TAN_SKEW_DVD_ADR          0x33A
#define READPOWER_CD_ADR          0x33B
#define READPOWER_DVD_ADR         0x33C
#define READPOWER_BDL0_ADR        0x33D
#define READPOWER_BDL1_ADR        0x33E
#define HFMF_BD_ADR               0x33F
#define HFMF_SS_BD_ADR            0x340
#define TILT_SENS_CD_ADR          0x341
#define TILT_SENS_DVD_ADR         0x342
#define TILT_SENS_BD_ADR          0x343
#define HFMP_BD0SL_ADR            0x344
#define HFMP_BD1DL_ADR            0x345
#define FFCG_FFC_CD_ADR           0x346
#define FFCG_FFC_DVD_ADR          0x347
#define FFCG_FFC_BD_ADR           0x348
#define IVG_CDDVD_ADR             0x349    //[6:4]CD, [2:0]DVD
#define IVG_BD_ADR                0x34A    //[2:0]BD
#define RAD_SKEW_BD_ADR           0x34E
#define RAD_SKEW_CD_ADR           0x34F
#define SA_ACT_BD_L0_ADR          0x350    //32[1:0]&33[7:0] total 10bits
#define SA_ACT_BD_L1_ADR          0x352    //34[1:0]&35[7:0] total 10bits
#define SONY_410_CHECK_SUM_ADR    0x35C    //44[7:0]&45[7:4] total 12bits
#define BD_BARCODE_START_ADDR     SERIAL_0_ADR
#define BD_BARCODE_END_ADDR       SONY_410_CHECK_SUM_ADR

#define HIF_MSG_MARKS_ADDR        0x370  // HIF message marks
#define SVO_MSG_MARKS_ADDR        0x374  // Servo message marks


#define PDC1            0x00
#define PDC2            bit3
#define HFM_ON          bit2
#define HFM_OFF         0x00
#define LD_POWERSAVE    0x00
#define LD_CD           0x01
#define LD_DVD          0x02
#define LD_BD           0x03
#define MODEC_OFF       0x00
#define MODEC_ON        bit7
#define MODEB_LOW       0x00
#define MODEB_MIDDLE    bit5
#define MODEB_HIGH      bit6
#define MODEA_LOW       0x00
#define MODEA_MIDDLE    bit3
#define MODEA_HIGH      bit4
#define OFST_M_DIS      0x00
#define OFST_M_ENA      bit2
#define OFST_R_DIS      0x00
#define OFST_R_ENA      bit1
#define APC_MODE        0x00
#define CC_MODE         bit0

#elif (OPU == HOPB1300)
/* Hitachi B1300 barcode data */
#define OPUTYPE3_LEN              1
#define OPUTYPE2_LEN              1
#define OPUTYPE1_LEN              1
#define OPUTYPE0_LEN              1
#define RAD_SKEW_BD2_LEN          1
#define RAD_SKEW_BD1_LEN          1
#define RAD_SKEW_BD0_LEN          1
#define TAN_SKEW_BD2_LEN          1
#define TAN_SKEW_BD1_LEN          1
#define TAN_SKEW_BD0_LEN          1
#define RAD_SKEW_DVD2_LEN         1
#define RAD_SKEW_DVD1_LEN         1
#define RAD_SKEW_DVD0_LEN         1
#define TAN_SKEW_DVD2_LEN         1
#define TAN_SKEW_DVD1_LEN         1
#define TAN_SKEW_DVD0_LEN         1
#define SA_ACT_BD_L03_LEN         1
#define SA_ACT_BD_L02_LEN         1
#define SA_ACT_BD_L01_LEN         1
#define SA_ACT_BD_L00_LEN         1
#define QCTEMP1_LEN               1
#define QCTEMP0_LEN               1
#define MID7_LEN                  1
#define MID6_LEN                  1
#define MID5_LEN                  1
#define MID4_LEN                  1
#define MID3_LEN                  1
#define MID2_LEN                  1
#define MID1_LEN                  1
#define MID0_LEN                  1
#define MC3_LEN                   1
#define MC2_LEN                   1
#define MC1_LEN                   1
#define MC0_LEN                   1

#define OPUTYPE3_ADR              0x330
#define OPUTYPE2_ADR              0x331
#define OPUTYPE1_ADR              0x332
#define OPUTYPE0_ADR              0x333
#define RAD_SKEW_BD2_ADR          0x335   //one gap
#define RAD_SKEW_BD1_ADR          0x336
#define RAD_SKEW_BD0_ADR          0x337
#define TAN_SKEW_BD2_ADR          0x339 //one gap
#define TAN_SKEW_BD1_ADR          0x33A
#define TAN_SKEW_BD0_ADR          0x33B
#define RAD_SKEW_DVD2_ADR         0x33D //one gap
#define RAD_SKEW_DVD1_ADR         0x33E
#define RAD_SKEW_DVD0_ADR         0x33F
#define TAN_SKEW_DVD2_ADR         0x341 //one gap
#define TAN_SKEW_DVD1_ADR         0x342
#define TAN_SKEW_DVD0_ADR         0x343
#define SA_ACT_BD_L03_ADR         0x345 //one gap
#define SA_ACT_BD_L02_ADR         0x346
#define SA_ACT_BD_L01_ADR         0x347
#define SA_ACT_BD_L00_ADR         0x348
#define QCTEMP1_ADR               0x34A //one gap
#define QCTEMP0_ADR               0x34B
#define MID7_ADR                  0x34D //one gap
#define MID6_ADR                  0x34E
#define MID5_ADR                  0x34F
#define MID4_ADR                  0x350
#define MID3_ADR                  0x351
#define MID2_ADR                  0x352
#define MID1_ADR                  0x353
#define MID0_ADR                  0x354
#define MC3_ADR                   0x356 //one gap
#define MC2_ADR                   0x357
#define MC1_ADR                   0x358
#define MC0_ADR                   0x359
#define BD_BARCODE_START_ADDR     OPUTYPE3_ADR
#define BD_BARCODE_END_ADDR       MC0_ADR

#elif (OPU == SFBD412)||(OPU == SFBD414)||(OPU == SOHBP7)
/* SANYO SF-BD412 barcode data */
    #if(USE_EEPROM_COLLI_POS == 0)
#define OPU_FEATURE_ADR         0x330
#define MANU_CONTROL_CODE_ADR   0x331
#define PROD_LINE_CODE_ADR      0x332
#define LABEL_ISSUE_YEAR_ADR    0x333
#define LABEL_ISSUE_MONTH_ADR   0x334
#define LABEL_ISSUE_DATE_ADR    0x335
#define TAN_SKEW_CD1_ADR        0x336
#define TAN_SKEW_CD0_ADR        0x337
#define RAD_SKEW_CD1_ADR        0x338
#define RAD_SKEW_CD0_ADR        0x339
#define TAN_SKEW_DVD1_ADR       0x33A
#define TAN_SKEW_DVD0_ADR       0x33B
#define RAD_SKEW_DVD1_ADR       0x33C
#define RAD_SKEW_DVD0_ADR       0x33D
#define TAN_SKEW_BD1_ADR        0x33E
#define TAN_SKEW_BD0_ADR        0x33F
#define RAD_SKEW_BD1_ADR        0x340
#define RAD_SKEW_BD0_ADR        0x341
//#define RPWR_CD_ADR           0x342
//#define RPWR_DVD_ADR          0x344
//							    0x346
//				                0x348
//#define RPWR_BDSL_ADR         0x34A
//#define RPWR_BDDL_ADR         0x34C
//#define HFMF_CD_ADR           0x34E
//#define HFMF_DVD_ADR          0x350
//#define HFMF_BD_ADR           0x352
//#define IOP_CD_ADR            0x354
//#define IOP_DVD_ADR           0x356
//#define IOP_BDSL_ADR          0x358
#define DF_CD4_ADR              0x35A
#define DF_CD3_ADR              0x35B
#define DF_CD2_ADR              0x35C
#define DF_CD1_ADR              0x35D
#define DF_CD0_ADR              0x35E
#define DF_DVD4_ADR             0x35F
#define DF_DVD3_ADR             0x360
#define DF_DVD2_ADR             0x361
#define DF_DVD1_ADR             0x362
#define DF_DVD0_ADR             0x363
#define DF_BDSL4_ADR            0x364
#define DF_BDSL3_ADR            0x365
#define DF_BDSL2_ADR            0x366
#define DF_BDSL1_ADR            0x367
#define DF_BDSL0_ADR            0x368
#define TEMP_BD1_ADR            0x369
#define TEMP_BD0_ADR            0x36A
#define COLLI_PSN_BDSL1_ADR     0x36B
#define COLLI_PSN_BDSL0_ADR     0x36C
#define CHECK_SUM_H_ADR         0x36D
#define CHECK_SUM_L_ADR         0x36E
#define BD_BARCODE_START_ADDR   OPU_FEATURE_ADR
#define BD_BARCODE_END_ADDR     COLLI_PSN_BDSL0_ADR
    #else //USE_EEPROM_COLLI_POS == 0
#define SERIAL_0_ADR                0x330
#define SERIAL_1_ADR                0x334
#define COLLI_ZERO_POS              0x340
#define COLLI_CAL_STATUS_ADR        0x341
#define COLLI_BDROM_LO_ADR          0x342
#define COLLI_BDRRE_LO_ADR          0x343
#define COLLI_BDROM_L1_ADR          0x344
#define COLLI_BDRRE_L1_ADR          0x346
#define BD_BARCODE_START_ADDR   SERIAL_0_ADR
#define BD_BARCODE_END_ADDR     COLLI_BDRRE_L1_ADR
    #endif //USE_EEPROM_COLLI_POS == 0
#define HIF_MSG_MARKS_ADDR        0x370  // HIF message marks
#define SVO_MSG_MARKS_ADDR        0x374  // Servo message marks

#elif OPU == BDPL2
/* Pioneer BDPL2 barcode data */
#define OPU_TYPE_ADR         0x330
#define OPU_SERIAL_ADR       0x332
#define OPU_QRVER_ADR        0x33A
#define BDSL_IV_ADR          0x33B
#define BDSL_IO_ADR          0x33D
#define BDSL_PL0_ADR         0x33F
#define BDSL_PL1_ADR         0x341
#define BDSL_IGAIN_ADR       0x343
#define BDSL_HMA_ADR         0x345
#define BDSL_HMF_ADR         0x347
#define BDDL_IV_ADR          0x349
#define BDDL_IO_ADR          0x34B
#define BDDL_PL0_ADR         0x34D
#define BDDL_PL1_ADR         0x34F
#define BDDL_IGAIN_ADR       0x351
#define BDDL_HMA_ADR         0x353
#define BDDL_HMF_ADR         0x355
#define DV_IV_ADR            0x357
#define DV_IO_ADR            0x359
#define DV_P_ADR             0x35B
#define DV_IGAIN_ADR         0x35D
#define DV_HMA_ADR           0x35F
#define DV_HMF_ADR           0x361
#define CD_IV_ADR            0x363
#define CD_IO_ADR            0x365
#define CD_P_ADR             0x367
#define CD_IGAIN_ADR         0x369
#define CD_HMA_ADR           0x36B
#define CD_HMF_ADR           0x36D
#define BDSA0_ADR            0x36F
#define DVSA_ADR             0x372
#define CDSA_ADR             0x375
#define DF_BD_ADR            0x378
#define DF_DV_ADR            0x37C
#define DF_CD_ADR            0x380
#define BD_APPK_ADR          0x384
#define CD_DPPK_ADR          0x386
#define BD_RAD_TILT_ADR      0x388
#define DV_RAD_TILT_ADR      0x38C //4byte gap
#define BD_TAN_TILT_ADR      0x394
#define DV_TAN_TILT_ADR      0x398
#define BD_INI_TEMP_ADR      0x3B7 //4byte+27byte gap
#define DV_INI_TEMP_ADR      0x3BA
#define CD_INI_TEMP_ADR      0x3BD
#define CHECK_SUM_H_ADR      0x3C0
#define CHECK_SUM_L_ADR      0x3C1
#define BD_BARCODE_START_ADDR   OPU_TYPE_ADR
#define BD_BARCODE_END_ADDR     CD_INI_TEMP_ADR
#define HIF_MSG_MARKS_ADDR        0x3D0  // HIF message marks
#define SVO_MSG_MARKS_ADDR        0x3D4  // Servo message marks
#endif


#if  SERVOKEYPARAMETER == 1
/*****************************************************/
/***           servo key parameter Table           ***/
/*****************************************************/
/**********CDROM**********/

#define KEY_header               0x0000
#define KEY_header_byte7         KEY_header+7
#define OPC_judge_status_tbl     KEY_header+13
#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define CDROM_header             SERVO_judge_status_tbl+1
#define CDROM_data_col           CDROM_header+7
#define CDROM_lastcol_bytenum    CDROM_data_col+1
#define CDROM_sensekey           CDROM_lastcol_bytenum+5
#define CDROM_ASC                CDROM_sensekey+1
#define CDROM_ASCQ               CDROM_ASC+1
#define _CDROM_VREFH              CDROM_ASCQ+0x11
#define _CDROM_VREFL              _CDROM_VREFH+1
#define _CDROM_V21H               _CDROM_VREFL+1
#define _CDROM_V21L               _CDROM_V21H+1
#define CDROM_RFINGN             _CDROM_V21L+1
#define CDROM_RFCOSPO            CDROM_RFINGN+1
#define CDROM_RFCOSNO            CDROM_RFCOSPO+1
#define CDROM_RFGNO              CDROM_RFCOSNO+1
#define CDROM_RFAGC              CDROM_RFGNO+1
#define CDROM_RFAOC              CDROM_RFAGC+1
#define _CDROM_maxFEpp_0dBH      CDROM_RFAOC+1              //20090507_h.30
#define _CDROM_maxFEpp_0dBL      _CDROM_maxFEpp_0dBH+1      //20090507_h.30
#define CDROM_RFPOS              _CDROM_maxFEpp_0dBL+1      //20090507_h.30
#define CDROM_MBVREFOS           CDROM_RFPOS+1
#define CDROM_SBVREFOS           CDROM_MBVREFOS+1
#define CDROM_MBGUPSEL           CDROM_SBVREFOS+1
#define CDROM_SBGUPSEL           CDROM_MBGUPSEL+1
#define CDROM_FEOOS              CDROM_SBGUPSEL+1
#define CDROM_FEOBG              CDROM_FEOOS+1
#define CDROM_FEOPGN             CDROM_FEOBG+1
#define CDROM_SPP1OS             CDROM_FEOPGN+1
#define CDROM_SPP1PGN            CDROM_SPP1OS+1
#define CDROM_SPP1BG             CDROM_SPP1PGN+1
#define CDROM_SPP2OS             CDROM_SPP1BG+1
#define CDROM_SPP2PGN            CDROM_SPP2OS+1
#define CDROM_SPP2BG             CDROM_SPP2PGN+1
#define CDROM_SBADOS             CDROM_SPP2BG+1
#define CDROM_SBADGN             CDROM_SBADOS+1
#define CDROM_FCSO               CDROM_SBADGN+1
#define CDROM_FCSG               CDROM_FCSO+1
#define CDROM_FEPGN              CDROM_FCSG+1
#define CDROM_FEBG               CDROM_FEPGN+1
#define CDROM_FE2OS              CDROM_FEBG+1
#define CDROM_FE2GN              CDROM_FE2OS+1
#define CDROM_SRFOS              CDROM_FE2GN+1
#define CDROM_SRFOBG             CDROM_SRFOS+1
#define CDROM_MPPOS              CDROM_SRFOBG+1
#define CDROM_MPPOBG             CDROM_MPPOS+1
#define CDROM_MPPOPGN            CDROM_MPPOBG+1
#define CDROM_CEOS               CDROM_MPPOPGN+1
#define CDROM_CEPGN              CDROM_CEOS+1
#define CDROM_CEGN               CDROM_CEPGN+1
#define CDROM_CEBG               CDROM_CEGN+1
#define CDROM_TRKO               CDROM_CEBG+1
#define CDROM_TRKG               CDROM_TRKO+1
#define CDROM_TEPGN              CDROM_TRKG+1
#define CDROM_TEBG               CDROM_TEPGN+1
#define CDROM_TE2OS              CDROM_TEBG+1
#define CDROM_TE2GN              CDROM_TE2OS+1
#define CDROM_BSKO               CDROM_TE2GN+1
#define CDROM_BSKG               CDROM_BSKO+1
#define _CDROM_MPPO_0dBH         CDROM_BSKG+1               //20090507_h.30
#define _CDROM_MPPO_0dBL         _CDROM_MPPO_0dBH+1         //20090507_h.30
#define CDROM_BS2OS              _CDROM_MPPO_0dBL+1         //20090507_h.30
#define CDROM_MPPOSRFORatioX100  CDROM_BS2OS+1              //20090507_h.30
#define CDROM_PHSOS              CDROM_MPPOSRFORatioX100+1  //20090507_h.30
#define CDROM_BHSOS              CDROM_PHSOS+1
#define _CDROM_FMD_PPLVL_CDH       CDROM_BHSOS+1
#define _CDROM_FMD_PPLVL_CDL      _CDROM_FMD_PPLVL_CDH +1
#define _CDROM_FMD_PPLVL_DVDH     _CDROM_FMD_PPLVL_CDL +1
#define _CDROM_FMD_PPLVL_DVDL     _CDROM_FMD_PPLVL_DVDH+1
#define _CDROM_FMD_PPLVL_BDH      _CDROM_FMD_PPLVL_DVDL+1
#define _CDROM_FMD_PPLVL_BDL      _CDROM_FMD_PPLVL_BDH +1
#define _CDROM_FMD_DCLVL_CDH      _CDROM_FMD_PPLVL_BDL +1
#define _CDROM_FMD_DCLVL_CDL      _CDROM_FMD_DCLVL_CDH +1
#define _CDROM_FMD_DCLVL_DVDH     _CDROM_FMD_DCLVL_CDL +1
#define _CDROM_FMD_DCLVL_DVDL     _CDROM_FMD_DCLVL_DVDH+1
#define _CDROM_FMD_DCLVL_BDH      _CDROM_FMD_DCLVL_DVDL+1
#define _CDROM_FMD_DCLVL_BDL      _CDROM_FMD_DCLVL_BDH +1
#define _CDROM_FEloopgainH        _CDROM_FMD_DCLVL_BDL +1
#define _CDROM_FEloopgainL        _CDROM_FEloopgainH+1
#define _CDROM_TEloopgainH        _CDROM_FEloopgainL+1
#define _CDROM_TEloopgainL        _CDROM_TEloopgainH+1
#define _CDROM_FEBCH              _CDROM_TEloopgainL+1
#define _CDROM_FEBCL              _CDROM_FEBCH+1
#define _CDROM_TILTH              _CDROM_FEBCL+1
#define _CDROM_TILTL              _CDROM_TILTH+1
#define _CDROM_DSPCEOFFSETH       _CDROM_TILTL+1
#define _CDROM_DSPCEOFFSETL       _CDROM_DSPCEOFFSETH+1
#define _CDROM_DSPFEOFFSETH       _CDROM_DSPCEOFFSETL+1
#define _CDROM_DSPFEOFFSETL       _CDROM_DSPFEOFFSETH+1
#define _CDROM_DSPTEOFFSETH       _CDROM_DSPFEOFFSETL+1
#define _CDROM_DSPTEOFFSETL       _CDROM_DSPTEOFFSETH+1
#define _CDROM_DSPRPOFFSETH       _CDROM_DSPTEOFFSETL+1
#define _CDROM_DSPRPOFFSETL       _CDROM_DSPRPOFFSETH+1
#define CDROM_TROOS               _CDROM_DSPRPOFFSETL+1
#define CDROM_M                   CDROM_TROOS+1
#define CDROM_DPDCH1OS            CDROM_M+1
#define CDROM_DPDCH2OS           CDROM_DPDCH1OS+1
#define CDROM_DPDCH3OS           CDROM_DPDCH2OS+1
#define CDROM_DPDCH4OS           CDROM_DPDCH3OS+1
#define CDROM_DPDGN              CDROM_DPDCH4OS+1
#define CDROM_DPDGMOS            CDROM_DPDGN+1
#define CDROM_RFGMOS             CDROM_DPDGMOS+1
#define CDROM_DPDHSSEL           CDROM_RFGMOS+1
#define CDROM_DPDHSFCH           CDROM_DPDHSSEL+1
#define CDROM_DPDHSFCL           CDROM_DPDHSFCH+1
#define CDROM_DPDHSISCL          CDROM_DPDHSFCL+1
#define CDROM_DPDINFC            CDROM_DPDHSISCL+1
#define CDROM_DPDBST             CDROM_DPDINFC+1
#define _CDROM_STMH               CDROM_DPDBST+1
#define _CDROM_STML               _CDROM_STMH+1
#define _CDROM_Layertime0H        _CDROM_STML+1
#define _CDROM_Layertime0L        _CDROM_Layertime0H+1
#define _CDROM_Layertime1H        _CDROM_Layertime0L+1
#define _CDROM_Layertime1L        _CDROM_Layertime1H+1
#define _CDROM_maxSRFOpp_0dBH     _CDROM_Layertime1L+1
#define _CDROM_maxSRFOpp_0dBL     _CDROM_maxSRFOpp_0dBH+1
#define CDROM_TEBSRatioX100      _CDROM_maxSRFOpp_0dBL+1
#define _CDROM_svoRunOutH         CDROM_TEBSRatioX100+1
#define _CDROM_svoRunOutL         _CDROM_svoRunOutH+1
#define _CDROM_SAL0H              _CDROM_svoRunOutL+1
#define _CDROM_SAL0L              _CDROM_SAL0H+1
#define _CDROM_FEBCcurH           _CDROM_SAL0L+1
#define _CDROM_FEBCcurL           _CDROM_FEBCcurH+1
#define _CDROM_TILTcurH           _CDROM_FEBCcurL+1
#define _CDROM_TILTcurL           _CDROM_TILTcurH+1
#define _CDROM_ADCVRPOWH          _CDROM_TILTcurL+1
#define _CDROM_ADCVRPOWL          _CDROM_ADCVRPOWH+1
#define _CDROM_VDELTAXH           _CDROM_ADCVRPOWL+1
#define _CDROM_VDELTAXL           _CDROM_VDELTAXH+1
#define _CDROM_SETPOINTH          _CDROM_VDELTAXL+1
#define _CDROM_SETPOINTL          _CDROM_SETPOINTH+1
#define _CDROM_FEPPL0H            _CDROM_SETPOINTL+1
#define _CDROM_FEPPL0L            _CDROM_FEPPL0H+1
#define _CDROM_FEPPL1H            _CDROM_FEPPL0L+1
#define _CDROM_FEPPL1L            _CDROM_FEPPL1H+1
#define _CDROM_LTHRPJGLVLH        _CDROM_FEPPL1L+1
#define _CDROM_LTHRPJGLVLL        _CDROM_LTHRPJGLVLH+1
#define _CDROM_RFppH              _CDROM_LTHRPJGLVLL+1
#define _CDROM_RFppL              _CDROM_RFppH+1
#define _CDROM_HOMEINDEXH         _CDROM_RFppL+1
#define _CDROM_HOMEINDEXL         _CDROM_HOMEINDEXH+1
#define _CDROM_UNBMSH             _CDROM_HOMEINDEXL+1
#define _CDROM_UNBMSL             _CDROM_UNBMSH+1
#define _CDROM_UNBHSH             _CDROM_UNBMSL+1
#define _CDROM_UNBHSL             _CDROM_UNBHSH+1
#define _CDROM_FGPULSECOUNTH      _CDROM_UNBHSL+1
#define _CDROM_FGPULSECOUNTL      _CDROM_FGPULSECOUNTH+1
#define CDROM_wbllockcnt          _CDROM_FGPULSECOUNTL+1
#define CDROM_jitter              CDROM_wbllockcnt+1
#define _CDROM_APPDPDPPRatioX100H CDROM_jitter+1
#define _CDROM_APPDPDPPRatioX100L _CDROM_APPDPDPPRatioX100H+1

/**********CDR**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define CDR_header              0x00C0
#define CDR_data_col            CDR_header+7
#define CDR_lastcol_bytenum     CDR_data_col+1
#define CDR_sensekey            CDR_lastcol_bytenum+5
#define CDR_ASC                 CDR_sensekey+1
#define CDR_ASCQ                CDR_ASC+1
#define _CDR_VREFH               CDR_ASCQ+0x11
#define _CDR_VREFL               _CDR_VREFH+1
#define _CDR_V21H                _CDR_VREFL+1
#define _CDR_V21L                _CDR_V21H+1
#define CDR_RFINGN              _CDR_V21L+1
#define CDR_RFCOSPO             CDR_RFINGN+1
#define CDR_RFCOSNO             CDR_RFCOSPO+1
#define CDR_RFGNO               CDR_RFCOSNO+1
#define CDR_RFAGC               CDR_RFGNO+1
#define CDR_RFAOC               CDR_RFAGC+1
#define _CDR_maxFEpp_0dBH       CDR_RFAOC+1                 //20090507_h.30
#define _CDR_maxFEpp_0dBL       _CDR_maxFEpp_0dBH+1         //20090507_h.30
#define CDR_RFPOS               _CDR_maxFEpp_0dBL+1         //20090507_h.30
#define CDR_MBVREFOS            CDR_RFPOS+1
#define CDR_SBVREFOS            CDR_MBVREFOS+1
#define CDR_MBGUPSEL            CDR_SBVREFOS+1
#define CDR_SBGUPSEL            CDR_MBGUPSEL+1
#define CDR_FEOOS               CDR_SBGUPSEL+1
#define CDR_FEOBG               CDR_FEOOS+1
#define CDR_FEOPGN              CDR_FEOBG+1
#define CDR_SPP1OS              CDR_FEOPGN+1
#define CDR_SPP1PGN             CDR_SPP1OS+1
#define CDR_SPP1BG              CDR_SPP1PGN+1
#define CDR_SPP2OS              CDR_SPP1BG+1
#define CDR_SPP2PGN             CDR_SPP2OS+1
#define CDR_SPP2BG              CDR_SPP2PGN+1
#define CDR_SBADOS              CDR_SPP2BG+1
#define CDR_SBADGN              CDR_SBADOS+1
#define CDR_FCSO                CDR_SBADGN+1
#define CDR_FCSG                CDR_FCSO+1
#define CDR_FEPGN               CDR_FCSG+1
#define CDR_FEBG                CDR_FEPGN+1
#define CDR_FE2OS               CDR_FEBG+1
#define CDR_FE2GN               CDR_FE2OS+1
#define CDR_SRFOS               CDR_FE2GN+1
#define CDR_SRFOBG              CDR_SRFOS+1
#define CDR_MPPOS               CDR_SRFOBG+1
#define CDR_MPPOBG              CDR_MPPOS+1
#define CDR_MPPOPGN             CDR_MPPOBG+1
#define CDR_CEOS                CDR_MPPOPGN+1
#define CDR_CEPGN               CDR_CEOS+1
#define CDR_CEGN                CDR_CEPGN+1
#define CDR_CEBG                CDR_CEGN+1
#define CDR_TRKO                CDR_CEBG+1
#define CDR_TRKG                CDR_TRKO+1
#define CDR_TEPGN               CDR_TRKG+1
#define CDR_TEBG                CDR_TEPGN+1
#define CDR_TE2OS               CDR_TEBG+1
#define CDR_TE2GN               CDR_TE2OS+1
#define CDR_BSKO                CDR_TE2GN+1
#define CDR_BSKG                CDR_BSKO+1
#define _CDR_MPPO_0dBH          CDR_BSKG+1                  //20090507_h.30
#define _CDR_MPPO_0dBL          _CDR_MPPO_0dBH+1            //20090507_h.30
#define CDR_BS2OS               _CDR_MPPO_0dBL+1            //20090507_h.30
#define CDR_MPPOSRFORatioX100   CDR_BS2OS+1                 //20090507_h.30
#define CDR_PHSOS               CDR_MPPOSRFORatioX100+1     //20090507_h.30
#define CDR_BHSOS               CDR_PHSOS+1
#define _CDR_FMD_PPLVL_CDH       CDR_BHSOS+1
#define _CDR_FMD_PPLVL_CDL      _CDR_FMD_PPLVL_CDH +1
#define _CDR_FMD_PPLVL_DVDH     _CDR_FMD_PPLVL_CDL +1
#define _CDR_FMD_PPLVL_DVDL     _CDR_FMD_PPLVL_DVDH+1
#define _CDR_FMD_PPLVL_BDH      _CDR_FMD_PPLVL_DVDL+1
#define _CDR_FMD_PPLVL_BDL      _CDR_FMD_PPLVL_BDH +1
#define _CDR_FMD_DCLVL_CDH      _CDR_FMD_PPLVL_BDL +1
#define _CDR_FMD_DCLVL_CDL      _CDR_FMD_DCLVL_CDH +1
#define _CDR_FMD_DCLVL_DVDH     _CDR_FMD_DCLVL_CDL +1
#define _CDR_FMD_DCLVL_DVDL     _CDR_FMD_DCLVL_DVDH+1
#define _CDR_FMD_DCLVL_BDH      _CDR_FMD_DCLVL_DVDL+1
#define _CDR_FMD_DCLVL_BDL      _CDR_FMD_DCLVL_BDH +1
#define _CDR_FEloopgainH        _CDR_FMD_DCLVL_BDL +1
#define _CDR_FEloopgainL         _CDR_FEloopgainH+1
#define _CDR_TEloopgainH         _CDR_FEloopgainL+1
#define _CDR_TEloopgainL         _CDR_TEloopgainH+1
#define _CDR_FEBCH               _CDR_TEloopgainL+1
#define _CDR_FEBCL               _CDR_FEBCH+1
#define _CDR_TILTH               _CDR_FEBCL+1
#define _CDR_TILTL               _CDR_TILTH+1
#define _CDR_DSPCEOFFSETH        _CDR_TILTL+1
#define _CDR_DSPCEOFFSETL        _CDR_DSPCEOFFSETH+1
#define _CDR_DSPFEOFFSETH        _CDR_DSPCEOFFSETL+1
#define _CDR_DSPFEOFFSETL        _CDR_DSPFEOFFSETH+1
#define _CDR_DSPTEOFFSETH        _CDR_DSPFEOFFSETL+1
#define _CDR_DSPTEOFFSETL        _CDR_DSPTEOFFSETH+1
#define _CDR_DSPRPOFFSETH        _CDR_DSPTEOFFSETL+1
#define _CDR_DSPRPOFFSETL        _CDR_DSPRPOFFSETH+1
#define CDR_TROOS               _CDR_DSPRPOFFSETL+1
#define CDR_M                    CDR_TROOS+1
#define CDR_DPDCH1OS             CDR_M+1
#define CDR_DPDCH2OS            CDR_DPDCH1OS+1
#define CDR_DPDCH3OS            CDR_DPDCH2OS+1
#define CDR_DPDCH4OS            CDR_DPDCH3OS+1
#define CDR_DPDGN               CDR_DPDCH4OS+1
#define CDR_DPDGMOS             CDR_DPDGN+1
#define CDR_RFGMOS              CDR_DPDGMOS+1
#define CDR_DPDHSSEL            CDR_RFGMOS+1
#define CDR_DPDHSFCH            CDR_DPDHSSEL+1
#define CDR_DPDHSFCL            CDR_DPDHSFCH+1
#define CDR_DPDHSISCL           CDR_DPDHSFCL+1
#define CDR_DPDINFC             CDR_DPDHSISCL+1
#define CDR_DPDBST              CDR_DPDINFC+1
#define _CDR_STMH                CDR_DPDBST+1
#define _CDR_STML                _CDR_STMH+1
#define _CDR_Layertime0H         _CDR_STML+1
#define _CDR_Layertime0L         _CDR_Layertime0H+1
#define _CDR_Layertime1H         _CDR_Layertime0L+1
#define _CDR_Layertime1L         _CDR_Layertime1H+1
#define _CDR_maxSRFOpp_0dBH      _CDR_Layertime1L+1
#define _CDR_maxSRFOpp_0dBL      _CDR_maxSRFOpp_0dBH+1
#define CDR_TEBSRatioX100       _CDR_maxSRFOpp_0dBL+1
#define _CDR_svoRunOutH          CDR_TEBSRatioX100+1
#define _CDR_svoRunOutL          _CDR_svoRunOutH+1
#define _CDR_SAL0H               _CDR_svoRunOutL+1
#define _CDR_SAL0L               _CDR_SAL0H+1
#define _CDR_FEBCcurH            _CDR_SAL0L+1
#define _CDR_FEBCcurL            _CDR_FEBCcurH+1
#define _CDR_TILTcurH            _CDR_FEBCcurL+1
#define _CDR_TILTcurL            _CDR_TILTcurH+1
#define _CDR_ADCVRPOWH          _CDR_TILTcurL+1
#define _CDR_ADCVRPOWL          _CDR_ADCVRPOWH+1
#define _CDR_VDELTAXH           _CDR_ADCVRPOWL+1
#define _CDR_VDELTAXL           _CDR_VDELTAXH+1
#define _CDR_SETPOINTH          _CDR_VDELTAXL+1
#define _CDR_SETPOINTL          _CDR_SETPOINTH+1
#define _CDR_FEPPL0H            _CDR_SETPOINTL+1
#define _CDR_FEPPL0L            _CDR_FEPPL0H+1
#define _CDR_FEPPL1H            _CDR_FEPPL0L+1
#define _CDR_FEPPL1L            _CDR_FEPPL1H+1
#define _CDR_LTHRPJGLVLH        _CDR_FEPPL1L+1
#define _CDR_LTHRPJGLVLL        _CDR_LTHRPJGLVLH+1
#define _CDR_RFppH              _CDR_LTHRPJGLVLL+1
#define _CDR_RFppL              _CDR_RFppH+1
#define _CDR_HOMEINDEXH         _CDR_RFppL+1
#define _CDR_HOMEINDEXL         _CDR_HOMEINDEXH+1
#define _CDR_UNBMSH             _CDR_HOMEINDEXL+1
#define _CDR_UNBMSL             _CDR_UNBMSH+1
#define _CDR_UNBHSH             _CDR_UNBMSL+1
#define _CDR_UNBHSL             _CDR_UNBHSH+1
#define _CDR_FGPULSECOUNTH           _CDR_UNBHSL+1
#define _CDR_FGPULSECOUNTL           _CDR_FGPULSECOUNTH+1
#define CDR_wbllockcnt          _CDR_FGPULSECOUNTL+1
#define CDR_jitter              CDR_wbllockcnt+1
#define _CDR_APPDPDPPRatioX100H CDR_jitter+1
#define _CDR_APPDPDPPRatioX100L _CDR_APPDPDPPRatioX100H+1

/**********CDRW**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define CDRW_header              0x0170
#define CDRW_data_col            CDRW_header+7
#define CDRW_lastcol_bytenum     CDRW_data_col+1
#define CDRW_sensekey            CDRW_lastcol_bytenum+5
#define CDRW_ASC                 CDRW_sensekey+1
#define CDRW_ASCQ                CDRW_ASC+1
#define _CDRW_VREFH               CDRW_ASCQ+0x11
#define _CDRW_VREFL               _CDRW_VREFH+1
#define _CDRW_V21H                _CDRW_VREFL+1
#define _CDRW_V21L                _CDRW_V21H+1
#define CDRW_RFINGN              _CDRW_V21L+1
#define CDRW_RFCOSPO             CDRW_RFINGN+1
#define CDRW_RFCOSNO             CDRW_RFCOSPO+1
#define CDRW_RFGNO               CDRW_RFCOSNO+1
#define CDRW_RFAGC               CDRW_RFGNO+1
#define CDRW_RFAOC               CDRW_RFAGC+1
#define _CDRW_maxFEpp_0dBH       CDRW_RFAOC+1               //20090507_h.30
#define _CDRW_maxFEpp_0dBL       _CDRW_maxFEpp_0dBH+1       //20090507_h.30
#define CDRW_RFPOS               _CDRW_maxFEpp_0dBL+1       //20090507_h.30
#define CDRW_MBVREFOS            CDRW_RFPOS+1
#define CDRW_SBVREFOS            CDRW_MBVREFOS+1
#define CDRW_MBGUPSEL            CDRW_SBVREFOS+1
#define CDRW_SBGUPSEL            CDRW_MBGUPSEL+1
#define CDRW_FEOOS               CDRW_SBGUPSEL+1
#define CDRW_FEOBG               CDRW_FEOOS+1
#define CDRW_FEOPGN              CDRW_FEOBG+1
#define CDRW_SPP1OS              CDRW_FEOPGN+1
#define CDRW_SPP1PGN             CDRW_SPP1OS+1
#define CDRW_SPP1BG              CDRW_SPP1PGN+1
#define CDRW_SPP2OS              CDRW_SPP1BG+1
#define CDRW_SPP2PGN             CDRW_SPP2OS+1
#define CDRW_SPP2BG              CDRW_SPP2PGN+1
#define CDRW_SBADOS              CDRW_SPP2BG+1
#define CDRW_SBADGN              CDRW_SBADOS+1
#define CDRW_FCSO                CDRW_SBADGN+1
#define CDRW_FCSG                CDRW_FCSO+1
#define CDRW_FEPGN               CDRW_FCSG+1
#define CDRW_FEBG                CDRW_FEPGN+1
#define CDRW_FE2OS               CDRW_FEBG+1
#define CDRW_FE2GN               CDRW_FE2OS+1
#define CDRW_SRFOS               CDRW_FE2GN+1
#define CDRW_SRFOBG              CDRW_SRFOS+1
#define CDRW_MPPOS               CDRW_SRFOBG+1
#define CDRW_MPPOBG              CDRW_MPPOS+1
#define CDRW_MPPOPGN             CDRW_MPPOBG+1
#define CDRW_CEOS                CDRW_MPPOPGN+1
#define CDRW_CEPGN               CDRW_CEOS+1
#define CDRW_CEGN                CDRW_CEPGN+1
#define CDRW_CEBG                CDRW_CEGN+1
#define CDRW_TRKO                CDRW_CEBG+1
#define CDRW_TRKG                CDRW_TRKO+1
#define CDRW_TEPGN               CDRW_TRKG+1
#define CDRW_TEBG                CDRW_TEPGN+1
#define CDRW_TE2OS               CDRW_TEBG+1
#define CDRW_TE2GN               CDRW_TE2OS+1
#define CDRW_BSKO                CDRW_TE2GN+1
#define CDRW_BSKG                CDRW_BSKO+1
#define _CDRW_MPPO_0dBH          CDRW_BSKG+1                //20090507_h.30
#define _CDRW_MPPO_0dBL          _CDRW_MPPO_0dBH+1          //20090507_h.30
#define CDRW_BS2OS               _CDRW_MPPO_0dBL+1          //20090507_h.30
#define CDRW_MPPOSRFORatioX100   CDRW_BS2OS+1               //20090507_h.30
#define CDRW_PHSOS               CDRW_MPPOSRFORatioX100+1   //20090507_h.30
#define CDRW_BHSOS               CDRW_PHSOS+1
#define _CDRW_FMD_PPLVL_CDH       CDRW_BHSOS+1
#define _CDRW_FMD_PPLVL_CDL      _CDRW_FMD_PPLVL_CDH +1
#define _CDRW_FMD_PPLVL_DVDH     _CDRW_FMD_PPLVL_CDL +1
#define _CDRW_FMD_PPLVL_DVDL     _CDRW_FMD_PPLVL_DVDH+1
#define _CDRW_FMD_PPLVL_BDH      _CDRW_FMD_PPLVL_DVDL+1
#define _CDRW_FMD_PPLVL_BDL      _CDRW_FMD_PPLVL_BDH +1
#define _CDRW_FMD_DCLVL_CDH      _CDRW_FMD_PPLVL_BDL +1
#define _CDRW_FMD_DCLVL_CDL      _CDRW_FMD_DCLVL_CDH +1
#define _CDRW_FMD_DCLVL_DVDH     _CDRW_FMD_DCLVL_CDL +1
#define _CDRW_FMD_DCLVL_DVDL     _CDRW_FMD_DCLVL_DVDH+1
#define _CDRW_FMD_DCLVL_BDH      _CDRW_FMD_DCLVL_DVDL+1
#define _CDRW_FMD_DCLVL_BDL      _CDRW_FMD_DCLVL_BDH +1
#define _CDRW_FEloopgainH        _CDRW_FMD_DCLVL_BDL +1
#define _CDRW_FEloopgainL         _CDRW_FEloopgainH+1
#define _CDRW_TEloopgainH         _CDRW_FEloopgainL+1
#define _CDRW_TEloopgainL         _CDRW_TEloopgainH+1
#define _CDRW_FEBCH               _CDRW_TEloopgainL+1
#define _CDRW_FEBCL               _CDRW_FEBCH+1
#define _CDRW_TILTH               _CDRW_FEBCL+1
#define _CDRW_TILTL               _CDRW_TILTH+1
#define _CDRW_DSPCEOFFSETH        _CDRW_TILTL+1
#define _CDRW_DSPCEOFFSETL        _CDRW_DSPCEOFFSETH+1
#define _CDRW_DSPFEOFFSETH        _CDRW_DSPCEOFFSETL+1
#define _CDRW_DSPFEOFFSETL        _CDRW_DSPFEOFFSETH+1
#define _CDRW_DSPTEOFFSETH        _CDRW_DSPFEOFFSETL+1
#define _CDRW_DSPTEOFFSETL        _CDRW_DSPTEOFFSETH+1
#define _CDRW_DSPRPOFFSETH        _CDRW_DSPTEOFFSETL+1
#define _CDRW_DSPRPOFFSETL        _CDRW_DSPRPOFFSETH+1
#define CDRW_TROOS               _CDRW_DSPRPOFFSETL+1
#define CDRW_M                    CDRW_TROOS+1
#define CDRW_DPDCH1OS             CDRW_M+1
#define CDRW_DPDCH2OS            CDRW_DPDCH1OS+1
#define CDRW_DPDCH3OS            CDRW_DPDCH2OS+1
#define CDRW_DPDCH4OS            CDRW_DPDCH3OS+1
#define CDRW_DPDGN               CDRW_DPDCH4OS+1
#define CDRW_DPDGMOS             CDRW_DPDGN+1
#define CDRW_RFGMOS              CDRW_DPDGMOS+1
#define CDRW_DPDHSSEL            CDRW_RFGMOS+1
#define CDRW_DPDHSFCH            CDRW_DPDHSSEL+1
#define CDRW_DPDHSFCL            CDRW_DPDHSFCH+1
#define CDRW_DPDHSISCL           CDRW_DPDHSFCL+1
#define CDRW_DPDINFC             CDRW_DPDHSISCL+1
#define CDRW_DPDBST              CDRW_DPDINFC+1
#define _CDRW_STMH                CDRW_DPDBST+1
#define _CDRW_STML                _CDRW_STMH+1
#define _CDRW_Layertime0H         _CDRW_STML+1
#define _CDRW_Layertime0L         _CDRW_Layertime0H+1
#define _CDRW_Layertime1H         _CDRW_Layertime0L+1
#define _CDRW_Layertime1L         _CDRW_Layertime1H+1
#define _CDRW_maxSRFOpp_0dBH      _CDRW_Layertime1L+1
#define _CDRW_maxSRFOpp_0dBL      _CDRW_maxSRFOpp_0dBH+1
#define CDRW_TEBSRatioX100       _CDRW_maxSRFOpp_0dBL+1
#define _CDRW_svoRunOutH          CDRW_TEBSRatioX100+1
#define _CDRW_svoRunOutL          _CDRW_svoRunOutH+1
#define _CDRW_SAL0H               _CDRW_svoRunOutL+1
#define _CDRW_SAL0L               _CDRW_SAL0H+1
#define _CDRW_FEBCcurH            _CDRW_SAL0L+1
#define _CDRW_FEBCcurL            _CDRW_FEBCcurH+1
#define _CDRW_TILTcurH            _CDRW_FEBCcurL+1
#define _CDRW_TILTcurL            _CDRW_TILTcurH+1
#define _CDRW_ADCVRPOWH          _CDRW_TILTcurL+1
#define _CDRW_ADCVRPOWL          _CDRW_ADCVRPOWH+1
#define _CDRW_VDELTAXH           _CDRW_ADCVRPOWL+1
#define _CDRW_VDELTAXL           _CDRW_VDELTAXH+1
#define _CDRW_SETPOINTH          _CDRW_VDELTAXL+1
#define _CDRW_SETPOINTL          _CDRW_SETPOINTH+1
#define _CDRW_FEPPL0H            _CDRW_SETPOINTL+1
#define _CDRW_FEPPL0L            _CDRW_FEPPL0H+1
#define _CDRW_FEPPL1H            _CDRW_FEPPL0L+1
#define _CDRW_FEPPL1L            _CDRW_FEPPL1H+1
#define _CDRW_LTHRPJGLVLH        _CDRW_FEPPL1L+1
#define _CDRW_LTHRPJGLVLL        _CDRW_LTHRPJGLVLH+1
#define _CDRW_RFppH              _CDRW_LTHRPJGLVLL+1
#define _CDRW_RFppL              _CDRW_RFppH+1
#define _CDRW_HOMEINDEXH         _CDRW_RFppL+1
#define _CDRW_HOMEINDEXL         _CDRW_HOMEINDEXH+1
#define _CDRW_UNBMSH             _CDRW_HOMEINDEXL+1
#define _CDRW_UNBMSL             _CDRW_UNBMSH+1
#define _CDRW_UNBHSH             _CDRW_UNBMSL+1
#define _CDRW_UNBHSL             _CDRW_UNBHSH+1
#define _CDRW_FGPULSECOUNTH           _CDRW_UNBHSL+1
#define _CDRW_FGPULSECOUNTL           _CDRW_FGPULSECOUNTH+1
#define CDRW_wbllockcnt          _CDRW_FGPULSECOUNTL+1
#define CDRW_jitter              CDRW_wbllockcnt+1
#define _CDRW_APPDPDPPRatioX100H CDRW_jitter+1
#define _CDRW_APPDPDPPRatioX100L _CDRW_APPDPDPPRatioX100H+1

/**********DVD5**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVD5_header              0x0220
#define DVD5_data_col            DVD5_header+7
#define DVD5_lastcol_bytenum     DVD5_data_col+1
#define DVD5_sensekey            DVD5_lastcol_bytenum+5
#define DVD5_ASC                 DVD5_sensekey+1
#define DVD5_ASCQ                DVD5_ASC+1
#define _DVD5_VREFH               DVD5_ASCQ+0x11
#define _DVD5_VREFL               _DVD5_VREFH+1
#define _DVD5_V21H                _DVD5_VREFL+1
#define _DVD5_V21L                _DVD5_V21H+1
#define DVD5_RFINGN              _DVD5_V21L+1
#define DVD5_RFCOSPO             DVD5_RFINGN+1
#define DVD5_RFCOSNO             DVD5_RFCOSPO+1
#define DVD5_RFGNO               DVD5_RFCOSNO+1
#define DVD5_RFAGC               DVD5_RFGNO+1
#define DVD5_RFAOC               DVD5_RFAGC+1
#define _DVD5_maxFEpp_0dBH       DVD5_RFAOC+1               //20090507_h.30
#define _DVD5_maxFEpp_0dBL       _DVD5_maxFEpp_0dBH+1       //20090507_h.30
#define DVD5_RFPOS               _DVD5_maxFEpp_0dBL+1       //20090507_h.30
#define DVD5_MBVREFOS            DVD5_RFPOS+1
#define DVD5_SBVREFOS            DVD5_MBVREFOS+1
#define DVD5_MBGUPSEL            DVD5_SBVREFOS+1
#define DVD5_SBGUPSEL            DVD5_MBGUPSEL+1
#define DVD5_FEOOS               DVD5_SBGUPSEL+1
#define DVD5_FEOBG               DVD5_FEOOS+1
#define DVD5_FEOPGN              DVD5_FEOBG+1
#define DVD5_SPP1OS              DVD5_FEOPGN+1
#define DVD5_SPP1PGN             DVD5_SPP1OS+1
#define DVD5_SPP1BG              DVD5_SPP1PGN+1
#define DVD5_SPP2OS              DVD5_SPP1BG+1
#define DVD5_SPP2PGN             DVD5_SPP2OS+1
#define DVD5_SPP2BG              DVD5_SPP2PGN+1
#define DVD5_SBADOS              DVD5_SPP2BG+1
#define DVD5_SBADGN              DVD5_SBADOS+1
#define DVD5_FCSO                DVD5_SBADGN+1
#define DVD5_FCSG                DVD5_FCSO+1
#define DVD5_FEPGN               DVD5_FCSG+1
#define DVD5_FEBG                DVD5_FEPGN+1
#define DVD5_FE2OS               DVD5_FEBG+1
#define DVD5_FE2GN               DVD5_FE2OS+1
#define DVD5_SRFOS               DVD5_FE2GN+1
#define DVD5_SRFOBG              DVD5_SRFOS+1
#define DVD5_MPPOS               DVD5_SRFOBG+1
#define DVD5_MPPOBG              DVD5_MPPOS+1
#define DVD5_MPPOPGN             DVD5_MPPOBG+1
#define DVD5_CEOS                DVD5_MPPOPGN+1
#define DVD5_CEPGN               DVD5_CEOS+1
#define DVD5_CEGN                DVD5_CEPGN+1
#define DVD5_CEBG                DVD5_CEGN+1
#define DVD5_TRKO                DVD5_CEBG+1
#define DVD5_TRKG                DVD5_TRKO+1
#define DVD5_TEPGN               DVD5_TRKG+1
#define DVD5_TEBG                DVD5_TEPGN+1
#define DVD5_TE2OS               DVD5_TEBG+1
#define DVD5_TE2GN               DVD5_TE2OS+1
#define DVD5_BSKO                DVD5_TE2GN+1
#define DVD5_BSKG                DVD5_BSKO+1
#define _DVD5_MPPO_0dBH          DVD5_BSKG+1                //20090507_h.30
#define _DVD5_MPPO_0dBL          _DVD5_MPPO_0dBH+1          //20090507_h.30
#define DVD5_BS2OS               _DVD5_MPPO_0dBL+1          //20090507_h.30
#define DVD5_MPPOSRFORatioX100   DVD5_BS2OS+1               //20090507_h.30
#define DVD5_PHSOS               DVD5_MPPOSRFORatioX100+1   //20090507_h.30
#define DVD5_BHSOS               DVD5_PHSOS+1
#define _DVD5_FMD_PPLVL_CDH       DVD5_BHSOS+1
#define _DVD5_FMD_PPLVL_CDL      _DVD5_FMD_PPLVL_CDH +1
#define _DVD5_FMD_PPLVL_DVDH     _DVD5_FMD_PPLVL_CDL +1
#define _DVD5_FMD_PPLVL_DVDL     _DVD5_FMD_PPLVL_DVDH+1
#define _DVD5_FMD_PPLVL_BDH      _DVD5_FMD_PPLVL_DVDL+1
#define _DVD5_FMD_PPLVL_BDL      _DVD5_FMD_PPLVL_BDH +1
#define _DVD5_FMD_DCLVL_CDH      _DVD5_FMD_PPLVL_BDL +1
#define _DVD5_FMD_DCLVL_CDL      _DVD5_FMD_DCLVL_CDH +1
#define _DVD5_FMD_DCLVL_DVDH     _DVD5_FMD_DCLVL_CDL +1
#define _DVD5_FMD_DCLVL_DVDL     _DVD5_FMD_DCLVL_DVDH+1
#define _DVD5_FMD_DCLVL_BDH      _DVD5_FMD_DCLVL_DVDL+1
#define _DVD5_FMD_DCLVL_BDL      _DVD5_FMD_DCLVL_BDH +1
#define _DVD5_FEloopgainH        _DVD5_FMD_DCLVL_BDL +1
#define _DVD5_FEloopgainL         _DVD5_FEloopgainH+1
#define _DVD5_TEloopgainH         _DVD5_FEloopgainL+1
#define _DVD5_TEloopgainL         _DVD5_TEloopgainH+1
#define _DVD5_FEBCH               _DVD5_TEloopgainL+1
#define _DVD5_FEBCL               _DVD5_FEBCH+1
#define _DVD5_TILTH               _DVD5_FEBCL+1
#define _DVD5_TILTL               _DVD5_TILTH+1
#define _DVD5_DSPCEOFFSETH        _DVD5_TILTL+1
#define _DVD5_DSPCEOFFSETL        _DVD5_DSPCEOFFSETH+1
#define _DVD5_DSPFEOFFSETH        _DVD5_DSPCEOFFSETL+1
#define _DVD5_DSPFEOFFSETL        _DVD5_DSPFEOFFSETH+1
#define _DVD5_DSPTEOFFSETH        _DVD5_DSPFEOFFSETL+1
#define _DVD5_DSPTEOFFSETL        _DVD5_DSPTEOFFSETH+1
#define _DVD5_DSPRPOFFSETH        _DVD5_DSPTEOFFSETL+1
#define _DVD5_DSPRPOFFSETL        _DVD5_DSPRPOFFSETH+1
#define DVD5_TROOS               _DVD5_DSPRPOFFSETL+1
#define DVD5_M                    DVD5_TROOS+1
#define DVD5_DPDCH1OS             DVD5_M+1
#define DVD5_DPDCH2OS            DVD5_DPDCH1OS+1
#define DVD5_DPDCH3OS            DVD5_DPDCH2OS+1
#define DVD5_DPDCH4OS            DVD5_DPDCH3OS+1
#define DVD5_DPDGN               DVD5_DPDCH4OS+1
#define DVD5_DPDGMOS             DVD5_DPDGN+1
#define DVD5_RFGMOS              DVD5_DPDGMOS+1
#define DVD5_DPDHSSEL            DVD5_RFGMOS+1
#define DVD5_DPDHSFCH            DVD5_DPDHSSEL+1
#define DVD5_DPDHSFCL            DVD5_DPDHSFCH+1
#define DVD5_DPDHSISCL           DVD5_DPDHSFCL+1
#define DVD5_DPDINFC             DVD5_DPDHSISCL+1
#define DVD5_DPDBST              DVD5_DPDINFC+1
#define _DVD5_STMH                DVD5_DPDBST+1
#define _DVD5_STML                _DVD5_STMH+1
#define _DVD5_Layertime0H         _DVD5_STML+1
#define _DVD5_Layertime0L         _DVD5_Layertime0H+1
#define _DVD5_Layertime1H         _DVD5_Layertime0L+1
#define _DVD5_Layertime1L         _DVD5_Layertime1H+1
#define _DVD5_maxSRFOpp_0dBH      _DVD5_Layertime1L+1
#define _DVD5_maxSRFOpp_0dBL      _DVD5_maxSRFOpp_0dBH+1
#define DVD5_TEBSRatioX100       _DVD5_maxSRFOpp_0dBL+1
#define _DVD5_svoRunOutH          DVD5_TEBSRatioX100+1
#define _DVD5_svoRunOutL          _DVD5_svoRunOutH+1
#define _DVD5_SAL0H               _DVD5_svoRunOutL+1
#define _DVD5_SAL0L               _DVD5_SAL0H+1
#define _DVD5_FEBCcurH            _DVD5_SAL0L+1
#define _DVD5_FEBCcurL            _DVD5_FEBCcurH+1
#define _DVD5_TILTcurH            _DVD5_FEBCcurL+1
#define _DVD5_TILTcurL            _DVD5_TILTcurH+1
#define _DVD5_ADCVRPOWH          _DVD5_TILTcurL+1
#define _DVD5_ADCVRPOWL          _DVD5_ADCVRPOWH+1
#define _DVD5_VDELTAXH           _DVD5_ADCVRPOWL+1
#define _DVD5_VDELTAXL           _DVD5_VDELTAXH+1
#define _DVD5_SETPOINTH          _DVD5_VDELTAXL+1
#define _DVD5_SETPOINTL          _DVD5_SETPOINTH+1
#define _DVD5_FEPPL0H            _DVD5_SETPOINTL+1
#define _DVD5_FEPPL0L            _DVD5_FEPPL0H+1
#define _DVD5_FEPPL1H            _DVD5_FEPPL0L+1
#define _DVD5_FEPPL1L            _DVD5_FEPPL1H+1
#define _DVD5_LTHRPJGLVLH        _DVD5_FEPPL1L+1
#define _DVD5_LTHRPJGLVLL        _DVD5_LTHRPJGLVLH+1
#define _DVD5_RFppH              _DVD5_LTHRPJGLVLL+1
#define _DVD5_RFppL              _DVD5_RFppH+1
#define _DVD5_HOMEINDEXH         _DVD5_RFppL+1
#define _DVD5_HOMEINDEXL         _DVD5_HOMEINDEXH+1
#define _DVD5_UNBMSH             _DVD5_HOMEINDEXL+1
#define _DVD5_UNBMSL             _DVD5_UNBMSH+1
#define _DVD5_UNBHSH             _DVD5_UNBMSL+1
#define _DVD5_UNBHSL             _DVD5_UNBHSH+1
#define _DVD5_FGPULSECOUNTH           _DVD5_UNBHSL+1
#define _DVD5_FGPULSECOUNTL           _DVD5_FGPULSECOUNTH+1
#define DVD5_wbllockcnt          _DVD5_FGPULSECOUNTL+1
#define DVD5_jitter              DVD5_wbllockcnt+1
#define _DVD5_APPDPDPPRatioX100H DVD5_jitter+1
#define _DVD5_APPDPDPPRatioX100L _DVD5_APPDPDPPRatioX100H+1

/**********DVD9**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVD9_header              0x02D0
#define DVD9_data_col            DVD9_header+7
#define DVD9_lastcol_bytenum     DVD9_data_col+1
#define DVD9_sensekey            DVD9_lastcol_bytenum+5
#define DVD9_ASC                 DVD9_sensekey+1
#define DVD9_ASCQ                DVD9_ASC+1
#define _DVD9_VREFH               DVD9_ASCQ+0x11
#define _DVD9_VREFL               _DVD9_VREFH+1
#define _DVD9_V21H                _DVD9_VREFL+1
#define _DVD9_V21L                _DVD9_V21H+1
#define DVD9_RFINGN              _DVD9_V21L+1
#define DVD9_RFCOSPO             DVD9_RFINGN+1
#define DVD9_RFCOSNO             DVD9_RFCOSPO+1
#define DVD9_RFGNO               DVD9_RFCOSNO+1
#define DVD9_RFAGC               DVD9_RFGNO+1
#define DVD9_RFAOC               DVD9_RFAGC+1
#define _DVD9_maxFEpp_0dBH       DVD9_RFAOC+1               //20090507_h.30
#define _DVD9_maxFEpp_0dBL       _DVD9_maxFEpp_0dBH+1       //20090507_h.30
#define DVD9_RFPOS               _DVD9_maxFEpp_0dBL+1       //20090507_h.30
#define DVD9_MBVREFOS            DVD9_RFPOS+1
#define DVD9_SBVREFOS            DVD9_MBVREFOS+1
#define DVD9_MBGUPSEL            DVD9_SBVREFOS+1
#define DVD9_SBGUPSEL            DVD9_MBGUPSEL+1
#define DVD9_FEOOS               DVD9_SBGUPSEL+1
#define DVD9_FEOBG               DVD9_FEOOS+1
#define DVD9_FEOPGN              DVD9_FEOBG+1
#define DVD9_SPP1OS              DVD9_FEOPGN+1
#define DVD9_SPP1PGN             DVD9_SPP1OS+1
#define DVD9_SPP1BG              DVD9_SPP1PGN+1
#define DVD9_SPP2OS              DVD9_SPP1BG+1
#define DVD9_SPP2PGN             DVD9_SPP2OS+1
#define DVD9_SPP2BG              DVD9_SPP2PGN+1
#define DVD9_SBADOS              DVD9_SPP2BG+1
#define DVD9_SBADGN              DVD9_SBADOS+1
#define DVD9_FCSO                DVD9_SBADGN+1
#define DVD9_FCSG                DVD9_FCSO+1
#define DVD9_FEPGN               DVD9_FCSG+1
#define DVD9_FEBG                DVD9_FEPGN+1
#define DVD9_FE2OS               DVD9_FEBG+1
#define DVD9_FE2GN               DVD9_FE2OS+1
#define DVD9_SRFOS               DVD9_FE2GN+1
#define DVD9_SRFOBG              DVD9_SRFOS+1
#define DVD9_MPPOS               DVD9_SRFOBG+1
#define DVD9_MPPOBG              DVD9_MPPOS+1
#define DVD9_MPPOPGN             DVD9_MPPOBG+1
#define DVD9_CEOS                DVD9_MPPOPGN+1
#define DVD9_CEPGN               DVD9_CEOS+1
#define DVD9_CEGN                DVD9_CEPGN+1
#define DVD9_CEBG                DVD9_CEGN+1
#define DVD9_TRKO                DVD9_CEBG+1
#define DVD9_TRKG                DVD9_TRKO+1
#define DVD9_TEPGN               DVD9_TRKG+1
#define DVD9_TEBG                DVD9_TEPGN+1
#define DVD9_TE2OS               DVD9_TEBG+1
#define DVD9_TE2GN               DVD9_TE2OS+1
#define DVD9_BSKO                DVD9_TE2GN+1
#define DVD9_BSKG                DVD9_BSKO+1
#define _DVD9_MPPO_0dBH          DVD9_BSKG+1                //20090507_h.30
#define _DVD9_MPPO_0dBL          _DVD9_MPPO_0dBH+1          //20090507_h.30
#define DVD9_BS2OS               _DVD9_MPPO_0dBL+1          //20090507_h.30
#define DVD9_MPPOSRFORatioX100   DVD9_BS2OS+1               //20090507_h.30
#define DVD9_PHSOS               DVD9_MPPOSRFORatioX100+1   //20090507_h.30
#define DVD9_BHSOS               DVD9_PHSOS+1
#define _DVD9_FMD_PPLVL_CDH       DVD9_BHSOS+1
#define _DVD9_FMD_PPLVL_CDL      _DVD9_FMD_PPLVL_CDH +1
#define _DVD9_FMD_PPLVL_DVDH     _DVD9_FMD_PPLVL_CDL +1
#define _DVD9_FMD_PPLVL_DVDL     _DVD9_FMD_PPLVL_DVDH+1
#define _DVD9_FMD_PPLVL_BDH      _DVD9_FMD_PPLVL_DVDL+1
#define _DVD9_FMD_PPLVL_BDL      _DVD9_FMD_PPLVL_BDH +1
#define _DVD9_FMD_DCLVL_CDH      _DVD9_FMD_PPLVL_BDL +1
#define _DVD9_FMD_DCLVL_CDL      _DVD9_FMD_DCLVL_CDH +1
#define _DVD9_FMD_DCLVL_DVDH     _DVD9_FMD_DCLVL_CDL +1
#define _DVD9_FMD_DCLVL_DVDL     _DVD9_FMD_DCLVL_DVDH+1
#define _DVD9_FMD_DCLVL_BDH      _DVD9_FMD_DCLVL_DVDL+1
#define _DVD9_FMD_DCLVL_BDL      _DVD9_FMD_DCLVL_BDH +1
#define _DVD9_FEloopgainH        _DVD9_FMD_DCLVL_BDL +1
#define _DVD9_FEloopgainL         _DVD9_FEloopgainH+1
#define _DVD9_TEloopgainH         _DVD9_FEloopgainL+1
#define _DVD9_TEloopgainL         _DVD9_TEloopgainH+1
#define _DVD9_FEBCH               _DVD9_TEloopgainL+1
#define _DVD9_FEBCL               _DVD9_FEBCH+1
#define _DVD9_TILTH               _DVD9_FEBCL+1
#define _DVD9_TILTL               _DVD9_TILTH+1
#define _DVD9_DSPCEOFFSETH        _DVD9_TILTL+1
#define _DVD9_DSPCEOFFSETL        _DVD9_DSPCEOFFSETH+1
#define _DVD9_DSPFEOFFSETH        _DVD9_DSPCEOFFSETL+1
#define _DVD9_DSPFEOFFSETL        _DVD9_DSPFEOFFSETH+1
#define _DVD9_DSPTEOFFSETH        _DVD9_DSPFEOFFSETL+1
#define _DVD9_DSPTEOFFSETL        _DVD9_DSPTEOFFSETH+1
#define _DVD9_DSPRPOFFSETH        _DVD9_DSPTEOFFSETL+1
#define _DVD9_DSPRPOFFSETL        _DVD9_DSPRPOFFSETH+1
#define DVD9_TROOS               _DVD9_DSPRPOFFSETL+1
#define DVD9_M                    DVD9_TROOS+1
#define DVD9_DPDCH1OS             DVD9_M+1
#define DVD9_DPDCH2OS            DVD9_DPDCH1OS+1
#define DVD9_DPDCH3OS            DVD9_DPDCH2OS+1
#define DVD9_DPDCH4OS            DVD9_DPDCH3OS+1
#define DVD9_DPDGN               DVD9_DPDCH4OS+1
#define DVD9_DPDGMOS             DVD9_DPDGN+1
#define DVD9_RFGMOS              DVD9_DPDGMOS+1
#define DVD9_DPDHSSEL            DVD9_RFGMOS+1
#define DVD9_DPDHSFCH            DVD9_DPDHSSEL+1
#define DVD9_DPDHSFCL            DVD9_DPDHSFCH+1
#define DVD9_DPDHSISCL           DVD9_DPDHSFCL+1
#define DVD9_DPDINFC             DVD9_DPDHSISCL+1
#define DVD9_DPDBST              DVD9_DPDINFC+1
#define _DVD9_STMH                DVD9_DPDBST+1
#define _DVD9_STML                _DVD9_STMH+1
#define _DVD9_Layertime0H         _DVD9_STML+1
#define _DVD9_Layertime0L         _DVD9_Layertime0H+1
#define _DVD9_Layertime1H         _DVD9_Layertime0L+1
#define _DVD9_Layertime1L         _DVD9_Layertime1H+1
#define _DVD9_maxSRFOpp_0dBH      _DVD9_Layertime1L+1
#define _DVD9_maxSRFOpp_0dBL      _DVD9_maxSRFOpp_0dBH+1
#define DVD9_TEBSRatioX100       _DVD9_maxSRFOpp_0dBL+1
#define _DVD9_svoRunOutH          DVD9_TEBSRatioX100+1
#define _DVD9_svoRunOutL          _DVD9_svoRunOutH+1
#define _DVD9_SAL0H               _DVD9_svoRunOutL+1
#define _DVD9_SAL0L               _DVD9_SAL0H+1
#define _DVD9_FEBCcurH            _DVD9_SAL0L+1
#define _DVD9_FEBCcurL            _DVD9_FEBCcurH+1
#define _DVD9_TILTcurH            _DVD9_FEBCcurL+1
#define _DVD9_TILTcurL            _DVD9_TILTcurH+1

#define DVD9_RFAGCL1             _DVD9_TILTcurL+1
#define DVD9_RFAOCL1             DVD9_RFAGCL1+1
#define DVD9_SPP1OSL1            DVD9_RFAOCL1+1
#define DVD9_SPP1PGNL1           DVD9_SPP1OSL1+1
#define DVD9_SPP1BGL1            DVD9_SPP1PGNL1+1
#define DVD9_MPPOSL1             DVD9_SPP1BGL1+1
#define DVD9_MPPOBGL1            DVD9_MPPOSL1+1
#define DVD9_MPPOPGNL1           DVD9_MPPOBGL1+1
#define DVD9_CEOSL1              DVD9_MPPOPGNL1+1
#define DVD9_CEPGNL1             DVD9_CEOSL1+1
#define DVD9_CEGNL1              DVD9_CEPGNL1+1
#define DVD9_CEBGL1              DVD9_CEGNL1+1
#define DVD9_TRKOL1              DVD9_CEBGL1+1
#define DVD9_TRKGL1              DVD9_TRKOL1+1
#define DVD9_TEPGNL1             DVD9_TRKGL1+1
#define DVD9_TEBGL1              DVD9_TEPGNL1+1
#define DVD9_TE2OSL1             DVD9_TEBGL1+1
#define DVD9_TE2GNL1             DVD9_TE2OSL1+1
#define DVD9_RPOSL1              DVD9_TE2GNL1+1
#define DVD9_RPPGNL1             DVD9_RPOSL1+1
#define DVD9_RPBGL1              DVD9_RPPGNL1+1
#define DVD9_RPGNL1              DVD9_RPBGL1+1
#define DVD9_RP2OSL1             DVD9_RPGNL1+1
#define DVD9_RP2GNL1             DVD9_RP2OSL1+1
#define _DVD9_FEloopgainL1H       DVD9_RP2GNL1+1
#define _DVD9_FEloopgainL1L       _DVD9_FEloopgainL1H+1
#define _DVD9_TEloopgainL1H       _DVD9_FEloopgainL1L+1
#define _DVD9_TEloopgainL1L       _DVD9_TEloopgainL1H+1
#define _DVD9_FEBCL1H             _DVD9_TEloopgainL1L+1
#define _DVD9_FEBCL1L             _DVD9_FEBCL1H+1
#define _DVD9_TILTL1H             _DVD9_FEBCL1L+1
#define _DVD9_TILTL1L             _DVD9_TILTL1H+1
#define _DVD9_SAL0L1H             _DVD9_TILTL1L+1
#define _DVD9_SAL0L1L             _DVD9_SAL0L1H+1
#define _DVD9_FEBCcurL1H          _DVD9_SAL0L1L+1
#define _DVD9_FEBCcurL1L          _DVD9_FEBCcurL1H+1
#define _DVD9_TILTcurL1H          _DVD9_FEBCcurL1L+1
#define _DVD9_TILTcurL1L          _DVD9_TILTcurL1H+1
#define _DVD9_ADCVRPOWH           _DVD9_TILTcurL1L+1
#define _DVD9_ADCVRPOWL          _DVD9_ADCVRPOWH+1
#define _DVD9_VDELTAXH           _DVD9_ADCVRPOWL+1
#define _DVD9_VDELTAXL           _DVD9_VDELTAXH+1
#define _DVD9_SETPOINTH          _DVD9_VDELTAXL+1
#define _DVD9_SETPOINTL          _DVD9_SETPOINTH+1
#define _DVD9_FEPPL0H            _DVD9_SETPOINTL+1
#define _DVD9_FEPPL0L            _DVD9_FEPPL0H+1
#define _DVD9_FEPPL1H            _DVD9_FEPPL0L+1
#define _DVD9_FEPPL1L            _DVD9_FEPPL1H+1
#define _DVD9_LTHRPJGLVLH        _DVD9_FEPPL1L+1
#define _DVD9_LTHRPJGLVLL        _DVD9_LTHRPJGLVLH+1
#define _DVD9_RFppH              _DVD9_LTHRPJGLVLL+1
#define _DVD9_RFppL              _DVD9_RFppH+1
#define _DVD9_HOMEINDEXH         _DVD9_RFppL+1
#define _DVD9_HOMEINDEXL         _DVD9_HOMEINDEXH+1
#define _DVD9_UNBMSH             _DVD9_HOMEINDEXL+1
#define _DVD9_UNBMSL             _DVD9_UNBMSH+1
#define _DVD9_UNBHSH             _DVD9_UNBMSL+1
#define _DVD9_UNBHSL             _DVD9_UNBHSH+1
#define _DVD9_FGPULSECOUNTH           _DVD9_UNBHSL+1
#define _DVD9_FGPULSECOUNTL           _DVD9_FGPULSECOUNTH+1
#define DVD9_wbllockcnt          _DVD9_FGPULSECOUNTL+1
#define DVD9_jitter              DVD9_wbllockcnt+1
#define _DVD9_APPDPDPPRatioX100H DVD9_jitter+1
#define _DVD9_APPDPDPPRatioX100L _DVD9_APPDPDPPRatioX100H+1

/**********DVDPR**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDPR_header              0x03B0
#define DVDPR_data_col            DVDPR_header+7
#define DVDPR_lastcol_bytenum     DVDPR_data_col+1
#define DVDPR_sensekey            DVDPR_lastcol_bytenum+5
#define DVDPR_ASC                 DVDPR_sensekey+1
#define DVDPR_ASCQ                DVDPR_ASC+1
#define _DVDPR_VREFH               DVDPR_ASCQ+0x11
#define _DVDPR_VREFL               _DVDPR_VREFH+1
#define _DVDPR_V21H                _DVDPR_VREFL+1
#define _DVDPR_V21L                _DVDPR_V21H+1
#define DVDPR_RFINGN              _DVDPR_V21L+1
#define DVDPR_RFCOSPO             DVDPR_RFINGN+1
#define DVDPR_RFCOSNO             DVDPR_RFCOSPO+1
#define DVDPR_RFGNO               DVDPR_RFCOSNO+1
#define DVDPR_RFAGC               DVDPR_RFGNO+1
#define DVDPR_RFAOC               DVDPR_RFAGC+1
#define _DVDPR_maxFEpp_0dBH       DVDPR_RFAOC+1             //20090507_h.30
#define _DVDPR_maxFEpp_0dBL       _DVDPR_maxFEpp_0dBH+1     //20090507_h.30
#define DVDPR_RFPOS               _DVDPR_maxFEpp_0dBL+1     //20090507_h.30
#define DVDPR_MBVREFOS            DVDPR_RFPOS+1
#define DVDPR_SBVREFOS            DVDPR_MBVREFOS+1
#define DVDPR_MBGUPSEL            DVDPR_SBVREFOS+1
#define DVDPR_SBGUPSEL            DVDPR_MBGUPSEL+1
#define DVDPR_FEOOS               DVDPR_SBGUPSEL+1
#define DVDPR_FEOBG               DVDPR_FEOOS+1
#define DVDPR_FEOPGN              DVDPR_FEOBG+1
#define DVDPR_SPP1OS              DVDPR_FEOPGN+1
#define DVDPR_SPP1PGN             DVDPR_SPP1OS+1
#define DVDPR_SPP1BG              DVDPR_SPP1PGN+1
#define DVDPR_SPP2OS              DVDPR_SPP1BG+1
#define DVDPR_SPP2PGN             DVDPR_SPP2OS+1
#define DVDPR_SPP2BG              DVDPR_SPP2PGN+1
#define DVDPR_SBADOS              DVDPR_SPP2BG+1
#define DVDPR_SBADGN              DVDPR_SBADOS+1
#define DVDPR_FCSO                DVDPR_SBADGN+1
#define DVDPR_FCSG                DVDPR_FCSO+1
#define DVDPR_FEPGN               DVDPR_FCSG+1
#define DVDPR_FEBG                DVDPR_FEPGN+1
#define DVDPR_FE2OS               DVDPR_FEBG+1
#define DVDPR_FE2GN               DVDPR_FE2OS+1
#define DVDPR_SRFOS               DVDPR_FE2GN+1
#define DVDPR_SRFOBG              DVDPR_SRFOS+1
#define DVDPR_MPPOS               DVDPR_SRFOBG+1
#define DVDPR_MPPOBG              DVDPR_MPPOS+1
#define DVDPR_MPPOPGN             DVDPR_MPPOBG+1
#define DVDPR_CEOS                DVDPR_MPPOPGN+1
#define DVDPR_CEPGN               DVDPR_CEOS+1
#define DVDPR_CEGN                DVDPR_CEPGN+1
#define DVDPR_CEBG                DVDPR_CEGN+1
#define DVDPR_TRKO                DVDPR_CEBG+1
#define DVDPR_TRKG                DVDPR_TRKO+1
#define DVDPR_TEPGN               DVDPR_TRKG+1
#define DVDPR_TEBG                DVDPR_TEPGN+1
#define DVDPR_TE2OS               DVDPR_TEBG+1
#define DVDPR_TE2GN               DVDPR_TE2OS+1
#define DVDPR_BSKO                DVDPR_TE2GN+1
#define DVDPR_BSKG                DVDPR_BSKO+1
#define _DVDPR_MPPO_0dBH          DVDPR_BSKG+1              //20090507_h.30
#define _DVDPR_MPPO_0dBL          _DVDPR_MPPO_0dBH+1        //20090507_h.30
#define DVDPR_BS2OS               _DVDPR_MPPO_0dBL+1        //20090507_h.30
#define DVDPR_MPPOSRFORatioX100   DVDPR_BS2OS+1             //20090507_h.30
#define DVDPR_PHSOS               DVDPR_MPPOSRFORatioX100+1 //20090507_h.30
#define DVDPR_BHSOS               DVDPR_PHSOS+1
#define _DVDPR_FMD_PPLVL_CDH       DVDPR_BHSOS+1
#define _DVDPR_FMD_PPLVL_CDL      _DVDPR_FMD_PPLVL_CDH +1
#define _DVDPR_FMD_PPLVL_DVDH     _DVDPR_FMD_PPLVL_CDL +1
#define _DVDPR_FMD_PPLVL_DVDL     _DVDPR_FMD_PPLVL_DVDH+1
#define _DVDPR_FMD_PPLVL_BDH      _DVDPR_FMD_PPLVL_DVDL+1
#define _DVDPR_FMD_PPLVL_BDL      _DVDPR_FMD_PPLVL_BDH +1
#define _DVDPR_FMD_DCLVL_CDH      _DVDPR_FMD_PPLVL_BDL +1
#define _DVDPR_FMD_DCLVL_CDL      _DVDPR_FMD_DCLVL_CDH +1
#define _DVDPR_FMD_DCLVL_DVDH     _DVDPR_FMD_DCLVL_CDL +1
#define _DVDPR_FMD_DCLVL_DVDL     _DVDPR_FMD_DCLVL_DVDH+1
#define _DVDPR_FMD_DCLVL_BDH      _DVDPR_FMD_DCLVL_DVDL+1
#define _DVDPR_FMD_DCLVL_BDL      _DVDPR_FMD_DCLVL_BDH +1
#define _DVDPR_FEloopgainH        _DVDPR_FMD_DCLVL_BDL +1
#define _DVDPR_FEloopgainL         _DVDPR_FEloopgainH+1
#define _DVDPR_TEloopgainH         _DVDPR_FEloopgainL+1
#define _DVDPR_TEloopgainL         _DVDPR_TEloopgainH+1
#define _DVDPR_FEBCH               _DVDPR_TEloopgainL+1
#define _DVDPR_FEBCL               _DVDPR_FEBCH+1
#define _DVDPR_TILTH               _DVDPR_FEBCL+1
#define _DVDPR_TILTL               _DVDPR_TILTH+1
#define _DVDPR_DSPCEOFFSETH        _DVDPR_TILTL+1
#define _DVDPR_DSPCEOFFSETL        _DVDPR_DSPCEOFFSETH+1
#define _DVDPR_DSPFEOFFSETH        _DVDPR_DSPCEOFFSETL+1
#define _DVDPR_DSPFEOFFSETL        _DVDPR_DSPFEOFFSETH+1
#define _DVDPR_DSPTEOFFSETH        _DVDPR_DSPFEOFFSETL+1
#define _DVDPR_DSPTEOFFSETL        _DVDPR_DSPTEOFFSETH+1
#define _DVDPR_DSPRPOFFSETH        _DVDPR_DSPTEOFFSETL+1
#define _DVDPR_DSPRPOFFSETL        _DVDPR_DSPRPOFFSETH+1
#define DVDPR_TROOS               _DVDPR_DSPRPOFFSETL+1
#define DVDPR_M                    DVDPR_TROOS+1
#define DVDPR_DPDCH1OS             DVDPR_M+1
#define DVDPR_DPDCH2OS            DVDPR_DPDCH1OS+1
#define DVDPR_DPDCH3OS            DVDPR_DPDCH2OS+1
#define DVDPR_DPDCH4OS            DVDPR_DPDCH3OS+1
#define DVDPR_DPDGN               DVDPR_DPDCH4OS+1
#define DVDPR_DPDGMOS             DVDPR_DPDGN+1
#define DVDPR_RFGMOS              DVDPR_DPDGMOS+1
#define DVDPR_DPDHSSEL            DVDPR_RFGMOS+1
#define DVDPR_DPDHSFCH            DVDPR_DPDHSSEL+1
#define DVDPR_DPDHSFCL            DVDPR_DPDHSFCH+1
#define DVDPR_DPDHSISCL           DVDPR_DPDHSFCL+1
#define DVDPR_DPDINFC             DVDPR_DPDHSISCL+1
#define DVDPR_DPDBST              DVDPR_DPDINFC+1
#define _DVDPR_STMH                DVDPR_DPDBST+1
#define _DVDPR_STML                _DVDPR_STMH+1
#define _DVDPR_Layertime0H         _DVDPR_STML+1
#define _DVDPR_Layertime0L         _DVDPR_Layertime0H+1
#define _DVDPR_Layertime1H         _DVDPR_Layertime0L+1
#define _DVDPR_Layertime1L         _DVDPR_Layertime1H+1
#define _DVDPR_maxSRFOpp_0dBH      _DVDPR_Layertime1L+1
#define _DVDPR_maxSRFOpp_0dBL      _DVDPR_maxSRFOpp_0dBH+1
#define DVDPR_TEBSRatioX100       _DVDPR_maxSRFOpp_0dBL+1
#define _DVDPR_svoRunOutH          DVDPR_TEBSRatioX100+1
#define _DVDPR_svoRunOutL          _DVDPR_svoRunOutH+1
#define _DVDPR_SAL0H               _DVDPR_svoRunOutL+1
#define _DVDPR_SAL0L               _DVDPR_SAL0H+1
#define _DVDPR_FEBCcurH            _DVDPR_SAL0L+1
#define _DVDPR_FEBCcurL            _DVDPR_FEBCcurH+1
#define _DVDPR_TILTcurH            _DVDPR_FEBCcurL+1
#define _DVDPR_TILTcurL            _DVDPR_TILTcurH+1
#define _DVDPR_ADCVRPOWH          _DVDPR_TILTcurL+1
#define _DVDPR_ADCVRPOWL          _DVDPR_ADCVRPOWH+1
#define _DVDPR_VDELTAXH           _DVDPR_ADCVRPOWL+1
#define _DVDPR_VDELTAXL           _DVDPR_VDELTAXH+1
#define _DVDPR_SETPOINTH          _DVDPR_VDELTAXL+1
#define _DVDPR_SETPOINTL          _DVDPR_SETPOINTH+1
#define _DVDPR_FEPPL0H            _DVDPR_SETPOINTL+1
#define _DVDPR_FEPPL0L            _DVDPR_FEPPL0H+1
#define _DVDPR_FEPPL1H            _DVDPR_FEPPL0L+1
#define _DVDPR_FEPPL1L            _DVDPR_FEPPL1H+1
#define _DVDPR_LTHRPJGLVLH        _DVDPR_FEPPL1L+1
#define _DVDPR_LTHRPJGLVLL        _DVDPR_LTHRPJGLVLH+1
#define _DVDPR_RFppH              _DVDPR_LTHRPJGLVLL+1
#define _DVDPR_RFppL              _DVDPR_RFppH+1
#define _DVDPR_HOMEINDEXH         _DVDPR_RFppL+1
#define _DVDPR_HOMEINDEXL         _DVDPR_HOMEINDEXH+1
#define _DVDPR_UNBMSH             _DVDPR_HOMEINDEXL+1
#define _DVDPR_UNBMSL             _DVDPR_UNBMSH+1
#define _DVDPR_UNBHSH             _DVDPR_UNBMSL+1
#define _DVDPR_UNBHSL             _DVDPR_UNBHSH+1
#define _DVDPR_FGPULSECOUNTH           _DVDPR_UNBHSL+1
#define _DVDPR_FGPULSECOUNTL           _DVDPR_FGPULSECOUNTH+1
#define DVDPR_wbllockcnt          _DVDPR_FGPULSECOUNTL+1
#define DVDPR_jitter              DVDPR_wbllockcnt+1
#define _DVDPR_APPDPDPPRatioX100H DVDPR_jitter+1
#define _DVDPR_APPDPDPPRatioX100L _DVDPR_APPDPDPPRatioX100H+1

/**********DVDMR**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDMR_header              0x0460
#define DVDMR_data_col            DVDMR_header+7
#define DVDMR_lastcol_bytenum     DVDMR_data_col+1
#define DVDMR_sensekey            DVDMR_lastcol_bytenum+5
#define DVDMR_ASC                 DVDMR_sensekey+1
#define DVDMR_ASCQ                DVDMR_ASC+1
#define _DVDMR_VREFH               DVDMR_ASCQ+0x11
#define _DVDMR_VREFL               _DVDMR_VREFH+1
#define _DVDMR_V21H                _DVDMR_VREFL+1
#define _DVDMR_V21L                _DVDMR_V21H+1
#define DVDMR_RFINGN              _DVDMR_V21L+1
#define DVDMR_RFCOSPO             DVDMR_RFINGN+1
#define DVDMR_RFCOSNO             DVDMR_RFCOSPO+1
#define DVDMR_RFGNO               DVDMR_RFCOSNO+1
#define DVDMR_RFAGC               DVDMR_RFGNO+1
#define DVDMR_RFAOC               DVDMR_RFAGC+1
#define _DVDMR_maxFEpp_0dBH       DVDMR_RFAOC+1             //20090507_h.30
#define _DVDMR_maxFEpp_0dBL       _DVDMR_maxFEpp_0dBH+1     //20090507_h.30
#define DVDMR_RFPOS               _DVDMR_maxFEpp_0dBL+1     //20090507_h.30
#define DVDMR_MBVREFOS            DVDMR_RFPOS+1
#define DVDMR_SBVREFOS            DVDMR_MBVREFOS+1
#define DVDMR_MBGUPSEL            DVDMR_SBVREFOS+1
#define DVDMR_SBGUPSEL            DVDMR_MBGUPSEL+1
#define DVDMR_FEOOS               DVDMR_SBGUPSEL+1
#define DVDMR_FEOBG               DVDMR_FEOOS+1
#define DVDMR_FEOPGN              DVDMR_FEOBG+1
#define DVDMR_SPP1OS              DVDMR_FEOPGN+1
#define DVDMR_SPP1PGN             DVDMR_SPP1OS+1
#define DVDMR_SPP1BG              DVDMR_SPP1PGN+1
#define DVDMR_SPP2OS              DVDMR_SPP1BG+1
#define DVDMR_SPP2PGN             DVDMR_SPP2OS+1
#define DVDMR_SPP2BG              DVDMR_SPP2PGN+1
#define DVDMR_SBADOS              DVDMR_SPP2BG+1
#define DVDMR_SBADGN              DVDMR_SBADOS+1
#define DVDMR_FCSO                DVDMR_SBADGN+1
#define DVDMR_FCSG                DVDMR_FCSO+1
#define DVDMR_FEPGN               DVDMR_FCSG+1
#define DVDMR_FEBG                DVDMR_FEPGN+1
#define DVDMR_FE2OS               DVDMR_FEBG+1
#define DVDMR_FE2GN               DVDMR_FE2OS+1
#define DVDMR_SRFOS               DVDMR_FE2GN+1
#define DVDMR_SRFOBG              DVDMR_SRFOS+1
#define DVDMR_MPPOS               DVDMR_SRFOBG+1
#define DVDMR_MPPOBG              DVDMR_MPPOS+1
#define DVDMR_MPPOPGN             DVDMR_MPPOBG+1
#define DVDMR_CEOS                DVDMR_MPPOPGN+1
#define DVDMR_CEPGN               DVDMR_CEOS+1
#define DVDMR_CEGN                DVDMR_CEPGN+1
#define DVDMR_CEBG                DVDMR_CEGN+1
#define DVDMR_TRKO                DVDMR_CEBG+1
#define DVDMR_TRKG                DVDMR_TRKO+1
#define DVDMR_TEPGN               DVDMR_TRKG+1
#define DVDMR_TEBG                DVDMR_TEPGN+1
#define DVDMR_TE2OS               DVDMR_TEBG+1
#define DVDMR_TE2GN               DVDMR_TE2OS+1
#define DVDMR_BSKO                DVDMR_TE2GN+1
#define DVDMR_BSKG                DVDMR_BSKO+1
#define _DVDMR_MPPO_0dBH          DVDMR_BSKG+1              //20090507_h.30
#define _DVDMR_MPPO_0dBL          _DVDMR_MPPO_0dBH+1        //20090507_h.30
#define DVDMR_BS2OS               _DVDMR_MPPO_0dBL+1        //20090507_h.30
#define DVDMR_MPPOSRFORatioX100   DVDMR_BS2OS+1             //20090507_h.30
#define DVDMR_PHSOS               DVDMR_MPPOSRFORatioX100+1 //20090507_h.30
#define DVDMR_BHSOS               DVDMR_PHSOS+1
#define _DVDMR_FMD_PPLVL_CDH       DVDMR_BHSOS+1
#define _DVDMR_FMD_PPLVL_CDL      _DVDMR_FMD_PPLVL_CDH +1
#define _DVDMR_FMD_PPLVL_DVDH     _DVDMR_FMD_PPLVL_CDL +1
#define _DVDMR_FMD_PPLVL_DVDL     _DVDMR_FMD_PPLVL_DVDH+1
#define _DVDMR_FMD_PPLVL_BDH      _DVDMR_FMD_PPLVL_DVDL+1
#define _DVDMR_FMD_PPLVL_BDL      _DVDMR_FMD_PPLVL_BDH +1
#define _DVDMR_FMD_DCLVL_CDH      _DVDMR_FMD_PPLVL_BDL +1
#define _DVDMR_FMD_DCLVL_CDL      _DVDMR_FMD_DCLVL_CDH +1
#define _DVDMR_FMD_DCLVL_DVDH     _DVDMR_FMD_DCLVL_CDL +1
#define _DVDMR_FMD_DCLVL_DVDL     _DVDMR_FMD_DCLVL_DVDH+1
#define _DVDMR_FMD_DCLVL_BDH      _DVDMR_FMD_DCLVL_DVDL+1
#define _DVDMR_FMD_DCLVL_BDL      _DVDMR_FMD_DCLVL_BDH +1
#define _DVDMR_FEloopgainH        _DVDMR_FMD_DCLVL_BDL +1
#define _DVDMR_FEloopgainL         _DVDMR_FEloopgainH+1
#define _DVDMR_TEloopgainH         _DVDMR_FEloopgainL+1
#define _DVDMR_TEloopgainL         _DVDMR_TEloopgainH+1
#define _DVDMR_FEBCH               _DVDMR_TEloopgainL+1
#define _DVDMR_FEBCL               _DVDMR_FEBCH+1
#define _DVDMR_TILTH               _DVDMR_FEBCL+1
#define _DVDMR_TILTL               _DVDMR_TILTH+1
#define _DVDMR_DSPCEOFFSETH        _DVDMR_TILTL+1
#define _DVDMR_DSPCEOFFSETL        _DVDMR_DSPCEOFFSETH+1
#define _DVDMR_DSPFEOFFSETH        _DVDMR_DSPCEOFFSETL+1
#define _DVDMR_DSPFEOFFSETL        _DVDMR_DSPFEOFFSETH+1
#define _DVDMR_DSPTEOFFSETH        _DVDMR_DSPFEOFFSETL+1
#define _DVDMR_DSPTEOFFSETL        _DVDMR_DSPTEOFFSETH+1
#define _DVDMR_DSPRPOFFSETH        _DVDMR_DSPTEOFFSETL+1
#define _DVDMR_DSPRPOFFSETL        _DVDMR_DSPRPOFFSETH+1
#define DVDMR_TROOS               _DVDMR_DSPRPOFFSETL+1
#define DVDMR_M                    DVDMR_TROOS+1
#define DVDMR_DPDCH1OS             DVDMR_M+1
#define DVDMR_DPDCH2OS            DVDMR_DPDCH1OS+1
#define DVDMR_DPDCH3OS            DVDMR_DPDCH2OS+1
#define DVDMR_DPDCH4OS            DVDMR_DPDCH3OS+1
#define DVDMR_DPDGN               DVDMR_DPDCH4OS+1
#define DVDMR_DPDGMOS             DVDMR_DPDGN+1
#define DVDMR_RFGMOS              DVDMR_DPDGMOS+1
#define DVDMR_DPDHSSEL            DVDMR_RFGMOS+1
#define DVDMR_DPDHSFCH            DVDMR_DPDHSSEL+1
#define DVDMR_DPDHSFCL            DVDMR_DPDHSFCH+1
#define DVDMR_DPDHSISCL           DVDMR_DPDHSFCL+1
#define DVDMR_DPDINFC             DVDMR_DPDHSISCL+1
#define DVDMR_DPDBST              DVDMR_DPDINFC+1
#define _DVDMR_STMH                DVDMR_DPDBST+1
#define _DVDMR_STML                _DVDMR_STMH+1
#define _DVDMR_Layertime0H         _DVDMR_STML+1
#define _DVDMR_Layertime0L         _DVDMR_Layertime0H+1
#define _DVDMR_Layertime1H         _DVDMR_Layertime0L+1
#define _DVDMR_Layertime1L         _DVDMR_Layertime1H+1
#define _DVDMR_maxSRFOpp_0dBH      _DVDMR_Layertime1L+1
#define _DVDMR_maxSRFOpp_0dBL      _DVDMR_maxSRFOpp_0dBH+1
#define DVDMR_TEBSRatioX100       _DVDMR_maxSRFOpp_0dBL+1
#define _DVDMR_svoRunOutH          DVDMR_TEBSRatioX100+1
#define _DVDMR_svoRunOutL          _DVDMR_svoRunOutH+1
#define _DVDMR_SAL0H               _DVDMR_svoRunOutL+1
#define _DVDMR_SAL0L               _DVDMR_SAL0H+1
#define _DVDMR_FEBCcurH            _DVDMR_SAL0L+1
#define _DVDMR_FEBCcurL            _DVDMR_FEBCcurH+1
#define _DVDMR_TILTcurH            _DVDMR_FEBCcurL+1
#define _DVDMR_TILTcurL            _DVDMR_TILTcurH+1
#define _DVDMR_ADCVRPOWH          _DVDMR_TILTcurL+1
#define _DVDMR_ADCVRPOWL          _DVDMR_ADCVRPOWH+1
#define _DVDMR_VDELTAXH           _DVDMR_ADCVRPOWL+1
#define _DVDMR_VDELTAXL           _DVDMR_VDELTAXH+1
#define _DVDMR_SETPOINTH          _DVDMR_VDELTAXL+1
#define _DVDMR_SETPOINTL          _DVDMR_SETPOINTH+1
#define _DVDMR_FEPPL0H            _DVDMR_SETPOINTL+1
#define _DVDMR_FEPPL0L            _DVDMR_FEPPL0H+1
#define _DVDMR_FEPPL1H            _DVDMR_FEPPL0L+1
#define _DVDMR_FEPPL1L            _DVDMR_FEPPL1H+1
#define _DVDMR_LTHRPJGLVLH        _DVDMR_FEPPL1L+1
#define _DVDMR_LTHRPJGLVLL        _DVDMR_LTHRPJGLVLH+1
#define _DVDMR_RFppH              _DVDMR_LTHRPJGLVLL+1
#define _DVDMR_RFppL              _DVDMR_RFppH+1
#define _DVDMR_HOMEINDEXH         _DVDMR_RFppL+1
#define _DVDMR_HOMEINDEXL         _DVDMR_HOMEINDEXH+1
#define _DVDMR_UNBMSH             _DVDMR_HOMEINDEXL+1
#define _DVDMR_UNBMSL             _DVDMR_UNBMSH+1
#define _DVDMR_UNBHSH             _DVDMR_UNBMSL+1
#define _DVDMR_UNBHSL             _DVDMR_UNBHSH+1
#define _DVDMR_FGPULSECOUNTH           _DVDMR_UNBHSL+1
#define _DVDMR_FGPULSECOUNTL           _DVDMR_FGPULSECOUNTH+1
#define DVDMR_wbllockcnt          _DVDMR_FGPULSECOUNTL+1
#define DVDMR_jitter              DVDMR_wbllockcnt+1
#define _DVDMR_APPDPDPPRatioX100H DVDMR_jitter+1
#define _DVDMR_APPDPDPPRatioX100L _DVDMR_APPDPDPPRatioX100H+1

/**********DVDPRW**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDPRW_header              0x0510
#define DVDPRW_data_col            DVDPRW_header+7
#define DVDPRW_lastcol_bytenum     DVDPRW_data_col+1
#define DVDPRW_sensekey            DVDPRW_lastcol_bytenum+5
#define DVDPRW_ASC                 DVDPRW_sensekey+1
#define DVDPRW_ASCQ                DVDPRW_ASC+1
#define _DVDPRW_VREFH               DVDPRW_ASCQ+0x11
#define _DVDPRW_VREFL               _DVDPRW_VREFH+1
#define _DVDPRW_V21H                _DVDPRW_VREFL+1
#define _DVDPRW_V21L                _DVDPRW_V21H+1
#define DVDPRW_RFINGN              _DVDPRW_V21L+1
#define DVDPRW_RFCOSPO             DVDPRW_RFINGN+1
#define DVDPRW_RFCOSNO             DVDPRW_RFCOSPO+1
#define DVDPRW_RFGNO               DVDPRW_RFCOSNO+1
#define DVDPRW_RFAGC               DVDPRW_RFGNO+1
#define DVDPRW_RFAOC               DVDPRW_RFAGC+1
#define _DVDPRW_maxFEpp_0dBH       DVDPRW_RFAOC+1           //20090507_h.30
#define _DVDPRW_maxFEpp_0dBL       _DVDPRW_maxFEpp_0dBH+1   //20090507_h.30
#define DVDPRW_RFPOS               _DVDPRW_maxFEpp_0dBL+1   //20090507_h.30
#define DVDPRW_MBVREFOS            DVDPRW_RFPOS+1
#define DVDPRW_SBVREFOS            DVDPRW_MBVREFOS+1
#define DVDPRW_MBGUPSEL            DVDPRW_SBVREFOS+1
#define DVDPRW_SBGUPSEL            DVDPRW_MBGUPSEL+1
#define DVDPRW_FEOOS               DVDPRW_SBGUPSEL+1
#define DVDPRW_FEOBG               DVDPRW_FEOOS+1
#define DVDPRW_FEOPGN              DVDPRW_FEOBG+1
#define DVDPRW_SPP1OS              DVDPRW_FEOPGN+1
#define DVDPRW_SPP1PGN             DVDPRW_SPP1OS+1
#define DVDPRW_SPP1BG              DVDPRW_SPP1PGN+1
#define DVDPRW_SPP2OS              DVDPRW_SPP1BG+1
#define DVDPRW_SPP2PGN             DVDPRW_SPP2OS+1
#define DVDPRW_SPP2BG              DVDPRW_SPP2PGN+1
#define DVDPRW_SBADOS              DVDPRW_SPP2BG+1
#define DVDPRW_SBADGN              DVDPRW_SBADOS+1
#define DVDPRW_FCSO                DVDPRW_SBADGN+1
#define DVDPRW_FCSG                DVDPRW_FCSO+1
#define DVDPRW_FEPGN               DVDPRW_FCSG+1
#define DVDPRW_FEBG                DVDPRW_FEPGN+1
#define DVDPRW_FE2OS               DVDPRW_FEBG+1
#define DVDPRW_FE2GN               DVDPRW_FE2OS+1
#define DVDPRW_SRFOS               DVDPRW_FE2GN+1
#define DVDPRW_SRFOBG              DVDPRW_SRFOS+1
#define DVDPRW_MPPOS               DVDPRW_SRFOBG+1
#define DVDPRW_MPPOBG              DVDPRW_MPPOS+1
#define DVDPRW_MPPOPGN             DVDPRW_MPPOBG+1
#define DVDPRW_CEOS                DVDPRW_MPPOPGN+1
#define DVDPRW_CEPGN               DVDPRW_CEOS+1
#define DVDPRW_CEGN                DVDPRW_CEPGN+1
#define DVDPRW_CEBG                DVDPRW_CEGN+1
#define DVDPRW_TRKO                DVDPRW_CEBG+1
#define DVDPRW_TRKG                DVDPRW_TRKO+1
#define DVDPRW_TEPGN               DVDPRW_TRKG+1
#define DVDPRW_TEBG                DVDPRW_TEPGN+1
#define DVDPRW_TE2OS               DVDPRW_TEBG+1
#define DVDPRW_TE2GN               DVDPRW_TE2OS+1
#define DVDPRW_BSKO                DVDPRW_TE2GN+1
#define DVDPRW_BSKG                DVDPRW_BSKO+1
#define _DVDPRW_MPPO_0dBH          DVDPRW_BSKG+1            //20090507_h.30
#define _DVDPRW_MPPO_0dBL          _DVDPRW_MPPO_0dBH+1      //20090507_h.30
#define DVDPRW_BS2OS               _DVDPRW_MPPO_0dBL+1      //20090507_h.30
#define DVDPRW_MPPOSRFORatioX100   DVDPRW_BS2OS+1           //20090507_h.30
#define DVDPRW_PHSOS               DVDPRW_MPPOSRFORatioX100+1//20090507_h.30
#define DVDPRW_BHSOS               DVDPRW_PHSOS+1
#define _DVDPRW_FMD_PPLVL_CDH       DVDPRW_BHSOS+1
#define _DVDPRW_FMD_PPLVL_CDL      _DVDPRW_FMD_PPLVL_CDH +1
#define _DVDPRW_FMD_PPLVL_DVDH     _DVDPRW_FMD_PPLVL_CDL +1
#define _DVDPRW_FMD_PPLVL_DVDL     _DVDPRW_FMD_PPLVL_DVDH+1
#define _DVDPRW_FMD_PPLVL_BDH      _DVDPRW_FMD_PPLVL_DVDL+1
#define _DVDPRW_FMD_PPLVL_BDL      _DVDPRW_FMD_PPLVL_BDH +1
#define _DVDPRW_FMD_DCLVL_CDH      _DVDPRW_FMD_PPLVL_BDL +1
#define _DVDPRW_FMD_DCLVL_CDL      _DVDPRW_FMD_DCLVL_CDH +1
#define _DVDPRW_FMD_DCLVL_DVDH     _DVDPRW_FMD_DCLVL_CDL +1
#define _DVDPRW_FMD_DCLVL_DVDL     _DVDPRW_FMD_DCLVL_DVDH+1
#define _DVDPRW_FMD_DCLVL_BDH      _DVDPRW_FMD_DCLVL_DVDL+1
#define _DVDPRW_FMD_DCLVL_BDL      _DVDPRW_FMD_DCLVL_BDH +1
#define _DVDPRW_FEloopgainH        _DVDPRW_FMD_DCLVL_BDL +1
#define _DVDPRW_FEloopgainL         _DVDPRW_FEloopgainH+1
#define _DVDPRW_TEloopgainH         _DVDPRW_FEloopgainL+1
#define _DVDPRW_TEloopgainL         _DVDPRW_TEloopgainH+1
#define _DVDPRW_FEBCH               _DVDPRW_TEloopgainL+1
#define _DVDPRW_FEBCL               _DVDPRW_FEBCH+1
#define _DVDPRW_TILTH               _DVDPRW_FEBCL+1
#define _DVDPRW_TILTL               _DVDPRW_TILTH+1
#define _DVDPRW_DSPCEOFFSETH        _DVDPRW_TILTL+1
#define _DVDPRW_DSPCEOFFSETL        _DVDPRW_DSPCEOFFSETH+1
#define _DVDPRW_DSPFEOFFSETH        _DVDPRW_DSPCEOFFSETL+1
#define _DVDPRW_DSPFEOFFSETL        _DVDPRW_DSPFEOFFSETH+1
#define _DVDPRW_DSPTEOFFSETH        _DVDPRW_DSPFEOFFSETL+1
#define _DVDPRW_DSPTEOFFSETL        _DVDPRW_DSPTEOFFSETH+1
#define _DVDPRW_DSPRPOFFSETH        _DVDPRW_DSPTEOFFSETL+1
#define _DVDPRW_DSPRPOFFSETL        _DVDPRW_DSPRPOFFSETH+1
#define DVDPRW_TROOS               _DVDPRW_DSPRPOFFSETL+1
#define DVDPRW_M                    DVDPRW_TROOS+1
#define DVDPRW_DPDCH1OS             DVDPRW_M+1
#define DVDPRW_DPDCH2OS            DVDPRW_DPDCH1OS+1
#define DVDPRW_DPDCH3OS            DVDPRW_DPDCH2OS+1
#define DVDPRW_DPDCH4OS            DVDPRW_DPDCH3OS+1
#define DVDPRW_DPDGN               DVDPRW_DPDCH4OS+1
#define DVDPRW_DPDGMOS             DVDPRW_DPDGN+1
#define DVDPRW_RFGMOS              DVDPRW_DPDGMOS+1
#define DVDPRW_DPDHSSEL            DVDPRW_RFGMOS+1
#define DVDPRW_DPDHSFCH            DVDPRW_DPDHSSEL+1
#define DVDPRW_DPDHSFCL            DVDPRW_DPDHSFCH+1
#define DVDPRW_DPDHSISCL           DVDPRW_DPDHSFCL+1
#define DVDPRW_DPDINFC             DVDPRW_DPDHSISCL+1
#define DVDPRW_DPDBST              DVDPRW_DPDINFC+1
#define _DVDPRW_STMH                DVDPRW_DPDBST+1
#define _DVDPRW_STML                _DVDPRW_STMH+1
#define _DVDPRW_Layertime0H         _DVDPRW_STML+1
#define _DVDPRW_Layertime0L         _DVDPRW_Layertime0H+1
#define _DVDPRW_Layertime1H         _DVDPRW_Layertime0L+1
#define _DVDPRW_Layertime1L         _DVDPRW_Layertime1H+1
#define _DVDPRW_maxSRFOpp_0dBH      _DVDPRW_Layertime1L+1
#define _DVDPRW_maxSRFOpp_0dBL      _DVDPRW_maxSRFOpp_0dBH+1
#define DVDPRW_TEBSRatioX100       _DVDPRW_maxSRFOpp_0dBL+1
#define _DVDPRW_svoRunOutH          DVDPRW_TEBSRatioX100+1
#define _DVDPRW_svoRunOutL          _DVDPRW_svoRunOutH+1
#define _DVDPRW_SAL0H               _DVDPRW_svoRunOutL+1
#define _DVDPRW_SAL0L               _DVDPRW_SAL0H+1
#define _DVDPRW_FEBCcurH            _DVDPRW_SAL0L+1
#define _DVDPRW_FEBCcurL            _DVDPRW_FEBCcurH+1
#define _DVDPRW_TILTcurH            _DVDPRW_FEBCcurL+1
#define _DVDPRW_TILTcurL            _DVDPRW_TILTcurH+1
#define _DVDPRW_ADCVRPOWH          _DVDPRW_TILTcurL+1
#define _DVDPRW_ADCVRPOWL          _DVDPRW_ADCVRPOWH+1
#define _DVDPRW_VDELTAXH           _DVDPRW_ADCVRPOWL+1
#define _DVDPRW_VDELTAXL           _DVDPRW_VDELTAXH+1
#define _DVDPRW_SETPOINTH          _DVDPRW_VDELTAXL+1
#define _DVDPRW_SETPOINTL          _DVDPRW_SETPOINTH+1
#define _DVDPRW_FEPPL0H            _DVDPRW_SETPOINTL+1
#define _DVDPRW_FEPPL0L            _DVDPRW_FEPPL0H+1
#define _DVDPRW_FEPPL1H            _DVDPRW_FEPPL0L+1
#define _DVDPRW_FEPPL1L            _DVDPRW_FEPPL1H+1
#define _DVDPRW_LTHRPJGLVLH        _DVDPRW_FEPPL1L+1
#define _DVDPRW_LTHRPJGLVLL        _DVDPRW_LTHRPJGLVLH+1
#define _DVDPRW_RFppH              _DVDPRW_LTHRPJGLVLL+1
#define _DVDPRW_RFppL              _DVDPRW_RFppH+1
#define _DVDPRW_HOMEINDEXH         _DVDPRW_RFppL+1
#define _DVDPRW_HOMEINDEXL         _DVDPRW_HOMEINDEXH+1
#define _DVDPRW_UNBMSH             _DVDPRW_HOMEINDEXL+1
#define _DVDPRW_UNBMSL             _DVDPRW_UNBMSH+1
#define _DVDPRW_UNBHSH             _DVDPRW_UNBMSL+1
#define _DVDPRW_UNBHSL             _DVDPRW_UNBHSH+1
#define _DVDPRW_FGPULSECOUNTH           _DVDPRW_UNBHSL+1
#define _DVDPRW_FGPULSECOUNTL           _DVDPRW_FGPULSECOUNTH+1
#define DVDPRW_wbllockcnt          _DVDPRW_FGPULSECOUNTL+1
#define DVDPRW_jitter              DVDPRW_wbllockcnt+1
#define _DVDPRW_APPDPDPPRatioX100H DVDPRW_jitter+1
#define _DVDPRW_APPDPDPPRatioX100L _DVDPRW_APPDPDPPRatioX100H+1

/**********DVDMRW**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDMRW_header              0x05C0
#define DVDMRW_data_col            DVDMRW_header+7
#define DVDMRW_lastcol_bytenum     DVDMRW_data_col+1
#define DVDMRW_sensekey            DVDMRW_lastcol_bytenum+5
#define DVDMRW_ASC                 DVDMRW_sensekey+1
#define DVDMRW_ASCQ                DVDMRW_ASC+1
#define _DVDMRW_VREFH               DVDMRW_ASCQ+0x11
#define _DVDMRW_VREFL               _DVDMRW_VREFH+1
#define _DVDMRW_V21H                _DVDMRW_VREFL+1
#define _DVDMRW_V21L                _DVDMRW_V21H+1
#define DVDMRW_RFINGN              _DVDMRW_V21L+1
#define DVDMRW_RFCOSPO             DVDMRW_RFINGN+1
#define DVDMRW_RFCOSNO             DVDMRW_RFCOSPO+1
#define DVDMRW_RFGNO               DVDMRW_RFCOSNO+1
#define DVDMRW_RFAGC               DVDMRW_RFGNO+1
#define DVDMRW_RFAOC               DVDMRW_RFAGC+1
#define _DVDMRW_maxFEpp_0dBH       DVDMRW_RFAOC+1           //20090507_h.30
#define _DVDMRW_maxFEpp_0dBL       _DVDMRW_maxFEpp_0dBH+1   //20090507_h.30
#define DVDMRW_RFPOS               _DVDMRW_maxFEpp_0dBL+1   //20090507_h.30
#define DVDMRW_MBVREFOS            DVDMRW_RFPOS+1
#define DVDMRW_SBVREFOS            DVDMRW_MBVREFOS+1
#define DVDMRW_MBGUPSEL            DVDMRW_SBVREFOS+1
#define DVDMRW_SBGUPSEL            DVDMRW_MBGUPSEL+1
#define DVDMRW_FEOOS               DVDMRW_SBGUPSEL+1
#define DVDMRW_FEOBG               DVDMRW_FEOOS+1
#define DVDMRW_FEOPGN              DVDMRW_FEOBG+1
#define DVDMRW_SPP1OS              DVDMRW_FEOPGN+1
#define DVDMRW_SPP1PGN             DVDMRW_SPP1OS+1
#define DVDMRW_SPP1BG              DVDMRW_SPP1PGN+1
#define DVDMRW_SPP2OS              DVDMRW_SPP1BG+1
#define DVDMRW_SPP2PGN             DVDMRW_SPP2OS+1
#define DVDMRW_SPP2BG              DVDMRW_SPP2PGN+1
#define DVDMRW_SBADOS              DVDMRW_SPP2BG+1
#define DVDMRW_SBADGN              DVDMRW_SBADOS+1
#define DVDMRW_FCSO                DVDMRW_SBADGN+1
#define DVDMRW_FCSG                DVDMRW_FCSO+1
#define DVDMRW_FEPGN               DVDMRW_FCSG+1
#define DVDMRW_FEBG                DVDMRW_FEPGN+1
#define DVDMRW_FE2OS               DVDMRW_FEBG+1
#define DVDMRW_FE2GN               DVDMRW_FE2OS+1
#define DVDMRW_SRFOS               DVDMRW_FE2GN+1
#define DVDMRW_SRFOBG              DVDMRW_SRFOS+1
#define DVDMRW_MPPOS               DVDMRW_SRFOBG+1
#define DVDMRW_MPPOBG              DVDMRW_MPPOS+1
#define DVDMRW_MPPOPGN             DVDMRW_MPPOBG+1
#define DVDMRW_CEOS                DVDMRW_MPPOPGN+1
#define DVDMRW_CEPGN               DVDMRW_CEOS+1
#define DVDMRW_CEGN                DVDMRW_CEPGN+1
#define DVDMRW_CEBG                DVDMRW_CEGN+1
#define DVDMRW_TRKO                DVDMRW_CEBG+1
#define DVDMRW_TRKG                DVDMRW_TRKO+1
#define DVDMRW_TEPGN               DVDMRW_TRKG+1
#define DVDMRW_TEBG                DVDMRW_TEPGN+1
#define DVDMRW_TE2OS               DVDMRW_TEBG+1
#define DVDMRW_TE2GN               DVDMRW_TE2OS+1
#define DVDMRW_BSKO                DVDMRW_TE2GN+1
#define DVDMRW_BSKG                DVDMRW_BSKO+1
#define _DVDMRW_MPPO_0dBH          DVDMRW_BSKG+1            //20090507_h.30
#define _DVDMRW_MPPO_0dBL          _DVDMRW_MPPO_0dBH+1      //20090507_h.30
#define DVDMRW_BS2OS               _DVDMRW_MPPO_0dBL+1      //20090507_h.30
#define DVDMRW_MPPOSRFORatioX100   DVDMRW_BS2OS+1           //20090507_h.30
#define DVDMRW_PHSOS               DVDMRW_MPPOSRFORatioX100+1//20090507_h.30
#define DVDMRW_BHSOS               DVDMRW_PHSOS+1
#define _DVDMRW_FMD_PPLVL_CDH       DVDMRW_BHSOS+1
#define _DVDMRW_FMD_PPLVL_CDL      _DVDMRW_FMD_PPLVL_CDH +1
#define _DVDMRW_FMD_PPLVL_DVDH     _DVDMRW_FMD_PPLVL_CDL +1
#define _DVDMRW_FMD_PPLVL_DVDL     _DVDMRW_FMD_PPLVL_DVDH+1
#define _DVDMRW_FMD_PPLVL_BDH      _DVDMRW_FMD_PPLVL_DVDL+1
#define _DVDMRW_FMD_PPLVL_BDL      _DVDMRW_FMD_PPLVL_BDH +1
#define _DVDMRW_FMD_DCLVL_CDH      _DVDMRW_FMD_PPLVL_BDL +1
#define _DVDMRW_FMD_DCLVL_CDL      _DVDMRW_FMD_DCLVL_CDH +1
#define _DVDMRW_FMD_DCLVL_DVDH     _DVDMRW_FMD_DCLVL_CDL +1
#define _DVDMRW_FMD_DCLVL_DVDL     _DVDMRW_FMD_DCLVL_DVDH+1
#define _DVDMRW_FMD_DCLVL_BDH      _DVDMRW_FMD_DCLVL_DVDL+1
#define _DVDMRW_FMD_DCLVL_BDL      _DVDMRW_FMD_DCLVL_BDH +1
#define _DVDMRW_FEloopgainH        _DVDMRW_FMD_DCLVL_BDL +1
#define _DVDMRW_FEloopgainL         _DVDMRW_FEloopgainH+1
#define _DVDMRW_TEloopgainH         _DVDMRW_FEloopgainL+1
#define _DVDMRW_TEloopgainL         _DVDMRW_TEloopgainH+1
#define _DVDMRW_FEBCH               _DVDMRW_TEloopgainL+1
#define _DVDMRW_FEBCL               _DVDMRW_FEBCH+1
#define _DVDMRW_TILTH               _DVDMRW_FEBCL+1
#define _DVDMRW_TILTL               _DVDMRW_TILTH+1
#define _DVDMRW_DSPCEOFFSETH        _DVDMRW_TILTL+1
#define _DVDMRW_DSPCEOFFSETL        _DVDMRW_DSPCEOFFSETH+1
#define _DVDMRW_DSPFEOFFSETH        _DVDMRW_DSPCEOFFSETL+1
#define _DVDMRW_DSPFEOFFSETL        _DVDMRW_DSPFEOFFSETH+1
#define _DVDMRW_DSPTEOFFSETH        _DVDMRW_DSPFEOFFSETL+1
#define _DVDMRW_DSPTEOFFSETL        _DVDMRW_DSPTEOFFSETH+1
#define _DVDMRW_DSPRPOFFSETH        _DVDMRW_DSPTEOFFSETL+1
#define _DVDMRW_DSPRPOFFSETL        _DVDMRW_DSPRPOFFSETH+1
#define DVDMRW_TROOS               _DVDMRW_DSPRPOFFSETL+1
#define DVDMRW_M                    DVDMRW_TROOS+1
#define DVDMRW_DPDCH1OS             DVDMRW_M+1
#define DVDMRW_DPDCH2OS            DVDMRW_DPDCH1OS+1
#define DVDMRW_DPDCH3OS            DVDMRW_DPDCH2OS+1
#define DVDMRW_DPDCH4OS            DVDMRW_DPDCH3OS+1
#define DVDMRW_DPDGN               DVDMRW_DPDCH4OS+1
#define DVDMRW_DPDGMOS             DVDMRW_DPDGN+1
#define DVDMRW_RFGMOS              DVDMRW_DPDGMOS+1
#define DVDMRW_DPDHSSEL            DVDMRW_RFGMOS+1
#define DVDMRW_DPDHSFCH            DVDMRW_DPDHSSEL+1
#define DVDMRW_DPDHSFCL            DVDMRW_DPDHSFCH+1
#define DVDMRW_DPDHSISCL           DVDMRW_DPDHSFCL+1
#define DVDMRW_DPDINFC             DVDMRW_DPDHSISCL+1
#define DVDMRW_DPDBST              DVDMRW_DPDINFC+1
#define _DVDMRW_STMH                DVDMRW_DPDBST+1
#define _DVDMRW_STML                _DVDMRW_STMH+1
#define _DVDMRW_Layertime0H         _DVDMRW_STML+1
#define _DVDMRW_Layertime0L         _DVDMRW_Layertime0H+1
#define _DVDMRW_Layertime1H         _DVDMRW_Layertime0L+1
#define _DVDMRW_Layertime1L         _DVDMRW_Layertime1H+1
#define _DVDMRW_maxSRFOpp_0dBH      _DVDMRW_Layertime1L+1
#define _DVDMRW_maxSRFOpp_0dBL      _DVDMRW_maxSRFOpp_0dBH+1
#define DVDMRW_TEBSRatioX100       _DVDMRW_maxSRFOpp_0dBL+1
#define _DVDMRW_svoRunOutH          DVDMRW_TEBSRatioX100+1
#define _DVDMRW_svoRunOutL          _DVDMRW_svoRunOutH+1
#define _DVDMRW_SAL0H               _DVDMRW_svoRunOutL+1
#define _DVDMRW_SAL0L               _DVDMRW_SAL0H+1
#define _DVDMRW_FEBCcurH            _DVDMRW_SAL0L+1
#define _DVDMRW_FEBCcurL            _DVDMRW_FEBCcurH+1
#define _DVDMRW_TILTcurH            _DVDMRW_FEBCcurL+1
#define _DVDMRW_TILTcurL            _DVDMRW_TILTcurH+1
#define _DVDMRW_ADCVRPOWH          _DVDMRW_TILTcurL+1
#define _DVDMRW_ADCVRPOWL          _DVDMRW_ADCVRPOWH+1
#define _DVDMRW_VDELTAXH           _DVDMRW_ADCVRPOWL+1
#define _DVDMRW_VDELTAXL           _DVDMRW_VDELTAXH+1
#define _DVDMRW_SETPOINTH          _DVDMRW_VDELTAXL+1
#define _DVDMRW_SETPOINTL          _DVDMRW_SETPOINTH+1
#define _DVDMRW_FEPPL0H            _DVDMRW_SETPOINTL+1
#define _DVDMRW_FEPPL0L            _DVDMRW_FEPPL0H+1
#define _DVDMRW_FEPPL1H            _DVDMRW_FEPPL0L+1
#define _DVDMRW_FEPPL1L            _DVDMRW_FEPPL1H+1
#define _DVDMRW_LTHRPJGLVLH        _DVDMRW_FEPPL1L+1
#define _DVDMRW_LTHRPJGLVLL        _DVDMRW_LTHRPJGLVLH+1
#define _DVDMRW_RFppH              _DVDMRW_LTHRPJGLVLL+1
#define _DVDMRW_RFppL              _DVDMRW_RFppH+1
#define _DVDMRW_HOMEINDEXH         _DVDMRW_RFppL+1
#define _DVDMRW_HOMEINDEXL         _DVDMRW_HOMEINDEXH+1
#define _DVDMRW_UNBMSH             _DVDMRW_HOMEINDEXL+1
#define _DVDMRW_UNBMSL             _DVDMRW_UNBMSH+1
#define _DVDMRW_UNBHSH             _DVDMRW_UNBMSL+1
#define _DVDMRW_UNBHSL             _DVDMRW_UNBHSH+1
#define _DVDMRW_FGPULSECOUNTH           _DVDMRW_UNBHSL+1
#define _DVDMRW_FGPULSECOUNTL           _DVDMRW_FGPULSECOUNTH+1
#define DVDMRW_wbllockcnt          _DVDMRW_FGPULSECOUNTL+1
#define DVDMRW_jitter              DVDMRW_wbllockcnt+1
#define _DVDMRW_APPDPDPPRatioX100H DVDMRW_jitter+1
#define _DVDMRW_APPDPDPPRatioX100L _DVDMRW_APPDPDPPRatioX100H+1

/**********DVDPR9**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDPR9_header              0x0670
#define DVDPR9_data_col            DVDPR9_header+7
#define DVDPR9_lastcol_bytenum     DVDPR9_data_col+1
#define DVDPR9_sensekey            DVDPR9_lastcol_bytenum+5
#define DVDPR9_ASC                 DVDPR9_sensekey+1
#define DVDPR9_ASCQ                DVDPR9_ASC+1
#define _DVDPR9_VREFH               DVDPR9_ASCQ+0x11
#define _DVDPR9_VREFL               _DVDPR9_VREFH+1
#define _DVDPR9_V21H                _DVDPR9_VREFL+1
#define _DVDPR9_V21L                _DVDPR9_V21H+1
#define DVDPR9_RFINGN              _DVDPR9_V21L+1
#define DVDPR9_RFCOSPO             DVDPR9_RFINGN+1
#define DVDPR9_RFCOSNO             DVDPR9_RFCOSPO+1
#define DVDPR9_RFGNO               DVDPR9_RFCOSNO+1
#define DVDPR9_RFAGC               DVDPR9_RFGNO+1
#define DVDPR9_RFAOC               DVDPR9_RFAGC+1
#define _DVDPR9_maxFEpp_0dBH       DVDPR9_RFAOC+1           //20090507_h.30
#define _DVDPR9_maxFEpp_0dBL       _DVDPR9_maxFEpp_0dBH+1   //20090507_h.30
#define DVDPR9_RFPOS               _DVDPR9_maxFEpp_0dBL+1   //20090507_h.30
#define DVDPR9_MBVREFOS            DVDPR9_RFPOS+1
#define DVDPR9_SBVREFOS            DVDPR9_MBVREFOS+1
#define DVDPR9_MBGUPSEL            DVDPR9_SBVREFOS+1
#define DVDPR9_SBGUPSEL            DVDPR9_MBGUPSEL+1
#define DVDPR9_FEOOS               DVDPR9_SBGUPSEL+1
#define DVDPR9_FEOBG               DVDPR9_FEOOS+1
#define DVDPR9_FEOPGN              DVDPR9_FEOBG+1
#define DVDPR9_SPP1OS              DVDPR9_FEOPGN+1
#define DVDPR9_SPP1PGN             DVDPR9_SPP1OS+1
#define DVDPR9_SPP1BG              DVDPR9_SPP1PGN+1
#define DVDPR9_SPP2OS              DVDPR9_SPP1BG+1
#define DVDPR9_SPP2PGN             DVDPR9_SPP2OS+1
#define DVDPR9_SPP2BG              DVDPR9_SPP2PGN+1
#define DVDPR9_SBADOS              DVDPR9_SPP2BG+1
#define DVDPR9_SBADGN              DVDPR9_SBADOS+1
#define DVDPR9_FCSO                DVDPR9_SBADGN+1
#define DVDPR9_FCSG                DVDPR9_FCSO+1
#define DVDPR9_FEPGN               DVDPR9_FCSG+1
#define DVDPR9_FEBG                DVDPR9_FEPGN+1
#define DVDPR9_FE2OS               DVDPR9_FEBG+1
#define DVDPR9_FE2GN               DVDPR9_FE2OS+1
#define DVDPR9_SRFOS               DVDPR9_FE2GN+1
#define DVDPR9_SRFOBG              DVDPR9_SRFOS+1
#define DVDPR9_MPPOS               DVDPR9_SRFOBG+1
#define DVDPR9_MPPOBG              DVDPR9_MPPOS+1
#define DVDPR9_MPPOPGN             DVDPR9_MPPOBG+1
#define DVDPR9_CEOS                DVDPR9_MPPOPGN+1
#define DVDPR9_CEPGN               DVDPR9_CEOS+1
#define DVDPR9_CEGN                DVDPR9_CEPGN+1
#define DVDPR9_CEBG                DVDPR9_CEGN+1
#define DVDPR9_TRKO                DVDPR9_CEBG+1
#define DVDPR9_TRKG                DVDPR9_TRKO+1
#define DVDPR9_TEPGN               DVDPR9_TRKG+1
#define DVDPR9_TEBG                DVDPR9_TEPGN+1
#define DVDPR9_TE2OS               DVDPR9_TEBG+1
#define DVDPR9_TE2GN               DVDPR9_TE2OS+1
#define DVDPR9_BSKO                DVDPR9_TE2GN+1
#define DVDPR9_BSKG                DVDPR9_BSKO+1
#define _DVDPR9_MPPO_0dBH          DVDPR9_BSKG+1            //20090507_h.30
#define _DVDPR9_MPPO_0dBL          _DVDPR9_MPPO_0dBH+1      //20090507_h.30
#define DVDPR9_BS2OS               _DVDPR9_MPPO_0dBL+1      //20090507_h.30
#define DVDPR9_MPPOSRFORatioX100   DVDPR9_BS2OS+1           //20090507_h.30
#define DVDPR9_PHSOS               DVDPR9_MPPOSRFORatioX100+1//20090507_h.30
#define DVDPR9_BHSOS               DVDPR9_PHSOS+1
#define _DVDPR9_FMD_PPLVL_CDH       DVDPR9_BHSOS+1
#define _DVDPR9_FMD_PPLVL_CDL      _DVDPR9_FMD_PPLVL_CDH +1
#define _DVDPR9_FMD_PPLVL_DVDH     _DVDPR9_FMD_PPLVL_CDL +1
#define _DVDPR9_FMD_PPLVL_DVDL     _DVDPR9_FMD_PPLVL_DVDH+1
#define _DVDPR9_FMD_PPLVL_BDH      _DVDPR9_FMD_PPLVL_DVDL+1
#define _DVDPR9_FMD_PPLVL_BDL      _DVDPR9_FMD_PPLVL_BDH +1
#define _DVDPR9_FMD_DCLVL_CDH      _DVDPR9_FMD_PPLVL_BDL +1
#define _DVDPR9_FMD_DCLVL_CDL      _DVDPR9_FMD_DCLVL_CDH +1
#define _DVDPR9_FMD_DCLVL_DVDH     _DVDPR9_FMD_DCLVL_CDL +1
#define _DVDPR9_FMD_DCLVL_DVDL     _DVDPR9_FMD_DCLVL_DVDH+1
#define _DVDPR9_FMD_DCLVL_BDH      _DVDPR9_FMD_DCLVL_DVDL+1
#define _DVDPR9_FMD_DCLVL_BDL      _DVDPR9_FMD_DCLVL_BDH +1
#define _DVDPR9_FEloopgainH        _DVDPR9_FMD_DCLVL_BDL +1
#define _DVDPR9_FEloopgainL         _DVDPR9_FEloopgainH+1
#define _DVDPR9_TEloopgainH         _DVDPR9_FEloopgainL+1
#define _DVDPR9_TEloopgainL         _DVDPR9_TEloopgainH+1
#define _DVDPR9_FEBCH               _DVDPR9_TEloopgainL+1
#define _DVDPR9_FEBCL               _DVDPR9_FEBCH+1
#define _DVDPR9_TILTH               _DVDPR9_FEBCL+1
#define _DVDPR9_TILTL               _DVDPR9_TILTH+1
#define _DVDPR9_DSPCEOFFSETH        _DVDPR9_TILTL+1
#define _DVDPR9_DSPCEOFFSETL        _DVDPR9_DSPCEOFFSETH+1
#define _DVDPR9_DSPFEOFFSETH        _DVDPR9_DSPCEOFFSETL+1
#define _DVDPR9_DSPFEOFFSETL        _DVDPR9_DSPFEOFFSETH+1
#define _DVDPR9_DSPTEOFFSETH        _DVDPR9_DSPFEOFFSETL+1
#define _DVDPR9_DSPTEOFFSETL        _DVDPR9_DSPTEOFFSETH+1
#define _DVDPR9_DSPRPOFFSETH        _DVDPR9_DSPTEOFFSETL+1
#define _DVDPR9_DSPRPOFFSETL        _DVDPR9_DSPRPOFFSETH+1
#define DVDPR9_TROOS               _DVDPR9_DSPRPOFFSETL+1
#define DVDPR9_M                    DVDPR9_TROOS+1
#define DVDPR9_DPDCH1OS             DVDPR9_M+1
#define DVDPR9_DPDCH2OS            DVDPR9_DPDCH1OS+1
#define DVDPR9_DPDCH3OS            DVDPR9_DPDCH2OS+1
#define DVDPR9_DPDCH4OS            DVDPR9_DPDCH3OS+1
#define DVDPR9_DPDGN               DVDPR9_DPDCH4OS+1
#define DVDPR9_DPDGMOS             DVDPR9_DPDGN+1
#define DVDPR9_RFGMOS              DVDPR9_DPDGMOS+1
#define DVDPR9_DPDHSSEL            DVDPR9_RFGMOS+1
#define DVDPR9_DPDHSFCH            DVDPR9_DPDHSSEL+1
#define DVDPR9_DPDHSFCL            DVDPR9_DPDHSFCH+1
#define DVDPR9_DPDHSISCL           DVDPR9_DPDHSFCL+1
#define DVDPR9_DPDINFC             DVDPR9_DPDHSISCL+1
#define DVDPR9_DPDBST              DVDPR9_DPDINFC+1
#define _DVDPR9_STMH                DVDPR9_DPDBST+1
#define _DVDPR9_STML                _DVDPR9_STMH+1
#define _DVDPR9_Layertime0H         _DVDPR9_STML+1
#define _DVDPR9_Layertime0L         _DVDPR9_Layertime0H+1
#define _DVDPR9_Layertime1H         _DVDPR9_Layertime0L+1
#define _DVDPR9_Layertime1L         _DVDPR9_Layertime1H+1
#define _DVDPR9_maxSRFOpp_0dBH      _DVDPR9_Layertime1L+1
#define _DVDPR9_maxSRFOpp_0dBL      _DVDPR9_maxSRFOpp_0dBH+1
#define DVDPR9_TEBSRatioX100       _DVDPR9_maxSRFOpp_0dBL+1
#define _DVDPR9_svoRunOutH          DVDPR9_TEBSRatioX100+1
#define _DVDPR9_svoRunOutL          _DVDPR9_svoRunOutH+1
#define _DVDPR9_SAL0H               _DVDPR9_svoRunOutL+1
#define _DVDPR9_SAL0L               _DVDPR9_SAL0H+1
#define _DVDPR9_FEBCcurH            _DVDPR9_SAL0L+1
#define _DVDPR9_FEBCcurL            _DVDPR9_FEBCcurH+1
#define _DVDPR9_TILTcurH            _DVDPR9_FEBCcurL+1
#define _DVDPR9_TILTcurL            _DVDPR9_TILTcurH+1

#define DVDPR9_RFAGCL1             _DVDPR9_TILTcurL+1
#define DVDPR9_RFAOCL1             DVDPR9_RFAGCL1+1
#define DVDPR9_SPP1OSL1            DVDPR9_RFAOCL1+1
#define DVDPR9_SPP1PGNL1           DVDPR9_SPP1OSL1+1
#define DVDPR9_SPP1BGL1            DVDPR9_SPP1PGNL1+1
#define DVDPR9_MPPOSL1             DVDPR9_SPP1BGL1+1
#define DVDPR9_MPPOBGL1            DVDPR9_MPPOSL1+1
#define DVDPR9_MPPOPGNL1           DVDPR9_MPPOBGL1+1
#define DVDPR9_CEOSL1              DVDPR9_MPPOPGNL1+1
#define DVDPR9_CEPGNL1             DVDPR9_CEOSL1+1
#define DVDPR9_CEGNL1              DVDPR9_CEPGNL1+1
#define DVDPR9_CEBGL1              DVDPR9_CEGNL1+1
#define DVDPR9_TRKOL1              DVDPR9_CEBGL1+1
#define DVDPR9_TRKGL1              DVDPR9_TRKOL1+1
#define DVDPR9_TEPGNL1             DVDPR9_TRKGL1+1
#define DVDPR9_TEBGL1              DVDPR9_TEPGNL1+1
#define DVDPR9_TE2OSL1             DVDPR9_TEBGL1+1
#define DVDPR9_TE2GNL1             DVDPR9_TE2OSL1+1
#define DVDPR9_RPOSL1              DVDPR9_TE2GNL1+1
#define DVDPR9_RPPGNL1             DVDPR9_RPOSL1+1
#define DVDPR9_RPBGL1              DVDPR9_RPPGNL1+1
#define DVDPR9_RPGNL1              DVDPR9_RPBGL1+1
#define DVDPR9_RP2OSL1             DVDPR9_RPGNL1+1
#define DVDPR9_RP2GNL1             DVDPR9_RP2OSL1+1
#define _DVDPR9_FEloopgainL1H       DVDPR9_RP2GNL1+1
#define _DVDPR9_FEloopgainL1L       _DVDPR9_FEloopgainL1H+1
#define _DVDPR9_TEloopgainL1H       _DVDPR9_FEloopgainL1L+1
#define _DVDPR9_TEloopgainL1L       _DVDPR9_TEloopgainL1H+1
#define _DVDPR9_FEBCL1H             _DVDPR9_TEloopgainL1L+1
#define _DVDPR9_FEBCL1L             _DVDPR9_FEBCL1H+1
#define _DVDPR9_TILTL1H             _DVDPR9_FEBCL1L+1
#define _DVDPR9_TILTL1L             _DVDPR9_TILTL1H+1
#define _DVDPR9_SAL0L1H             _DVDPR9_TILTL1L+1
#define _DVDPR9_SAL0L1L             _DVDPR9_SAL0L1H+1
#define _DVDPR9_FEBCcurL1H          _DVDPR9_SAL0L1L+1
#define _DVDPR9_FEBCcurL1L          _DVDPR9_FEBCcurL1H+1
#define _DVDPR9_TILTcurL1H          _DVDPR9_FEBCcurL1L+1
#define _DVDPR9_TILTcurL1L          _DVDPR9_TILTcurL1H+1
#define _DVDPR9_ADCVRPOWH           _DVDPR9_TILTcurL1L+1
#define _DVDPR9_ADCVRPOWL          _DVDPR9_ADCVRPOWH+1
#define _DVDPR9_VDELTAXH           _DVDPR9_ADCVRPOWL+1
#define _DVDPR9_VDELTAXL           _DVDPR9_VDELTAXH+1
#define _DVDPR9_SETPOINTH          _DVDPR9_VDELTAXL+1
#define _DVDPR9_SETPOINTL          _DVDPR9_SETPOINTH+1
#define _DVDPR9_FEPPL0H            _DVDPR9_SETPOINTL+1
#define _DVDPR9_FEPPL0L            _DVDPR9_FEPPL0H+1
#define _DVDPR9_FEPPL1H            _DVDPR9_FEPPL0L+1
#define _DVDPR9_FEPPL1L            _DVDPR9_FEPPL1H+1
#define _DVDPR9_LTHRPJGLVLH        _DVDPR9_FEPPL1L+1
#define _DVDPR9_LTHRPJGLVLL        _DVDPR9_LTHRPJGLVLH+1
#define _DVDPR9_RFppH              _DVDPR9_LTHRPJGLVLL+1
#define _DVDPR9_RFppL              _DVDPR9_RFppH+1
#define _DVDPR9_HOMEINDEXH         _DVDPR9_RFppL+1
#define _DVDPR9_HOMEINDEXL         _DVDPR9_HOMEINDEXH+1
#define _DVDPR9_UNBMSH             _DVDPR9_HOMEINDEXL+1
#define _DVDPR9_UNBMSL             _DVDPR9_UNBMSH+1
#define _DVDPR9_UNBHSH             _DVDPR9_UNBMSL+1
#define _DVDPR9_UNBHSL             _DVDPR9_UNBHSH+1
#define _DVDPR9_FGPULSECOUNTH           _DVDPR9_UNBHSL+1
#define _DVDPR9_FGPULSECOUNTL           _DVDPR9_FGPULSECOUNTH+1
#define DVDPR9_wbllockcnt          _DVDPR9_FGPULSECOUNTL+1
#define DVDPR9_jitter              DVDPR9_wbllockcnt+1
#define _DVDPR9_APPDPDPPRatioX100H DVDPR9_jitter+1
#define _DVDPR9_APPDPDPPRatioX100L _DVDPR9_APPDPDPPRatioX100H+1

/**********DVDMR9**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define DVDMR9_header              0x0750
#define DVDMR9_data_col            DVDMR9_header+7
#define DVDMR9_lastcol_bytenum     DVDMR9_data_col+1
#define DVDMR9_sensekey            DVDMR9_lastcol_bytenum+5
#define DVDMR9_ASC                 DVDMR9_sensekey+1
#define DVDMR9_ASCQ                DVDMR9_ASC+1
#define _DVDMR9_VREFH               DVDMR9_ASCQ+0x11
#define _DVDMR9_VREFL               _DVDMR9_VREFH+1
#define _DVDMR9_V21H                _DVDMR9_VREFL+1
#define _DVDMR9_V21L                _DVDMR9_V21H+1
#define DVDMR9_RFINGN              _DVDMR9_V21L+1
#define DVDMR9_RFCOSPO             DVDMR9_RFINGN+1
#define DVDMR9_RFCOSNO             DVDMR9_RFCOSPO+1
#define DVDMR9_RFGNO               DVDMR9_RFCOSNO+1
#define DVDMR9_RFAGC               DVDMR9_RFGNO+1
#define DVDMR9_RFAOC               DVDMR9_RFAGC+1
#define _DVDMR9_maxFEpp_0dBH       DVDMR9_RFAOC+1           //20090507_h.30
#define _DVDMR9_maxFEpp_0dBL       _DVDMR9_maxFEpp_0dBH+1   //20090507_h.30
#define DVDMR9_RFPOS               _DVDMR9_maxFEpp_0dBL+1   //20090507_h.30
#define DVDMR9_MBVREFOS            DVDMR9_RFPOS+1
#define DVDMR9_SBVREFOS            DVDMR9_MBVREFOS+1
#define DVDMR9_MBGUPSEL            DVDMR9_SBVREFOS+1
#define DVDMR9_SBGUPSEL            DVDMR9_MBGUPSEL+1
#define DVDMR9_FEOOS               DVDMR9_SBGUPSEL+1
#define DVDMR9_FEOBG               DVDMR9_FEOOS+1
#define DVDMR9_FEOPGN              DVDMR9_FEOBG+1
#define DVDMR9_SPP1OS              DVDMR9_FEOPGN+1
#define DVDMR9_SPP1PGN             DVDMR9_SPP1OS+1
#define DVDMR9_SPP1BG              DVDMR9_SPP1PGN+1
#define DVDMR9_SPP2OS              DVDMR9_SPP1BG+1
#define DVDMR9_SPP2PGN             DVDMR9_SPP2OS+1
#define DVDMR9_SPP2BG              DVDMR9_SPP2PGN+1
#define DVDMR9_SBADOS              DVDMR9_SPP2BG+1
#define DVDMR9_SBADGN              DVDMR9_SBADOS+1
#define DVDMR9_FCSO                DVDMR9_SBADGN+1
#define DVDMR9_FCSG                DVDMR9_FCSO+1
#define DVDMR9_FEPGN               DVDMR9_FCSG+1
#define DVDMR9_FEBG                DVDMR9_FEPGN+1
#define DVDMR9_FE2OS               DVDMR9_FEBG+1
#define DVDMR9_FE2GN               DVDMR9_FE2OS+1
#define DVDMR9_SRFOS               DVDMR9_FE2GN+1
#define DVDMR9_SRFOBG              DVDMR9_SRFOS+1
#define DVDMR9_MPPOS               DVDMR9_SRFOBG+1
#define DVDMR9_MPPOBG              DVDMR9_MPPOS+1
#define DVDMR9_MPPOPGN             DVDMR9_MPPOBG+1
#define DVDMR9_CEOS                DVDMR9_MPPOPGN+1
#define DVDMR9_CEPGN               DVDMR9_CEOS+1
#define DVDMR9_CEGN                DVDMR9_CEPGN+1
#define DVDMR9_CEBG                DVDMR9_CEGN+1
#define DVDMR9_TRKO                DVDMR9_CEBG+1
#define DVDMR9_TRKG                DVDMR9_TRKO+1
#define DVDMR9_TEPGN               DVDMR9_TRKG+1
#define DVDMR9_TEBG                DVDMR9_TEPGN+1
#define DVDMR9_TE2OS               DVDMR9_TEBG+1
#define DVDMR9_TE2GN               DVDMR9_TE2OS+1
#define DVDMR9_BSKO                DVDMR9_TE2GN+1
#define DVDMR9_BSKG                DVDMR9_BSKO+1
#define _DVDMR9_MPPO_0dBH          DVDMR9_BSKG+1            //20090507_h.30
#define _DVDMR9_MPPO_0dBL          _DVDMR9_MPPO_0dBH+1      //20090507_h.30
#define DVDMR9_BS2OS               _DVDMR9_MPPO_0dBL+1      //20090507_h.30
#define DVDMR9_MPPOSRFORatioX100   DVDMR9_BS2OS+1           //20090507_h.30
#define DVDMR9_PHSOS               DVDMR9_MPPOSRFORatioX100+1//20090507_h.30
#define DVDMR9_BHSOS               DVDMR9_PHSOS+1
#define _DVDMR9_FMD_PPLVL_CDH       DVDMR9_BHSOS+1
#define _DVDMR9_FMD_PPLVL_CDL      _DVDMR9_FMD_PPLVL_CDH +1
#define _DVDMR9_FMD_PPLVL_DVDH     _DVDMR9_FMD_PPLVL_CDL +1
#define _DVDMR9_FMD_PPLVL_DVDL     _DVDMR9_FMD_PPLVL_DVDH+1
#define _DVDMR9_FMD_PPLVL_BDH      _DVDMR9_FMD_PPLVL_DVDL+1
#define _DVDMR9_FMD_PPLVL_BDL      _DVDMR9_FMD_PPLVL_BDH +1
#define _DVDMR9_FMD_DCLVL_CDH      _DVDMR9_FMD_PPLVL_BDL +1
#define _DVDMR9_FMD_DCLVL_CDL      _DVDMR9_FMD_DCLVL_CDH +1
#define _DVDMR9_FMD_DCLVL_DVDH     _DVDMR9_FMD_DCLVL_CDL +1
#define _DVDMR9_FMD_DCLVL_DVDL     _DVDMR9_FMD_DCLVL_DVDH+1
#define _DVDMR9_FMD_DCLVL_BDH      _DVDMR9_FMD_DCLVL_DVDL+1
#define _DVDMR9_FMD_DCLVL_BDL      _DVDMR9_FMD_DCLVL_BDH +1
#define _DVDMR9_FEloopgainH        _DVDMR9_FMD_DCLVL_BDL +1
#define _DVDMR9_FEloopgainL         _DVDMR9_FEloopgainH+1
#define _DVDMR9_TEloopgainH         _DVDMR9_FEloopgainL+1
#define _DVDMR9_TEloopgainL         _DVDMR9_TEloopgainH+1
#define _DVDMR9_FEBCH               _DVDMR9_TEloopgainL+1
#define _DVDMR9_FEBCL               _DVDMR9_FEBCH+1
#define _DVDMR9_TILTH               _DVDMR9_FEBCL+1
#define _DVDMR9_TILTL               _DVDMR9_TILTH+1
#define _DVDMR9_DSPCEOFFSETH        _DVDMR9_TILTL+1
#define _DVDMR9_DSPCEOFFSETL        _DVDMR9_DSPCEOFFSETH+1
#define _DVDMR9_DSPFEOFFSETH        _DVDMR9_DSPCEOFFSETL+1
#define _DVDMR9_DSPFEOFFSETL        _DVDMR9_DSPFEOFFSETH+1
#define _DVDMR9_DSPTEOFFSETH        _DVDMR9_DSPFEOFFSETL+1
#define _DVDMR9_DSPTEOFFSETL        _DVDMR9_DSPTEOFFSETH+1
#define _DVDMR9_DSPRPOFFSETH        _DVDMR9_DSPTEOFFSETL+1
#define _DVDMR9_DSPRPOFFSETL        _DVDMR9_DSPRPOFFSETH+1
#define DVDMR9_TROOS               _DVDMR9_DSPRPOFFSETL+1
#define DVDMR9_M                    DVDMR9_TROOS+1
#define DVDMR9_DPDCH1OS             DVDMR9_M+1
#define DVDMR9_DPDCH2OS            DVDMR9_DPDCH1OS+1
#define DVDMR9_DPDCH3OS            DVDMR9_DPDCH2OS+1
#define DVDMR9_DPDCH4OS            DVDMR9_DPDCH3OS+1
#define DVDMR9_DPDGN               DVDMR9_DPDCH4OS+1
#define DVDMR9_DPDGMOS             DVDMR9_DPDGN+1
#define DVDMR9_RFGMOS              DVDMR9_DPDGMOS+1
#define DVDMR9_DPDHSSEL            DVDMR9_RFGMOS+1
#define DVDMR9_DPDHSFCH            DVDMR9_DPDHSSEL+1
#define DVDMR9_DPDHSFCL            DVDMR9_DPDHSFCH+1
#define DVDMR9_DPDHSISCL           DVDMR9_DPDHSFCL+1
#define DVDMR9_DPDINFC             DVDMR9_DPDHSISCL+1
#define DVDMR9_DPDBST              DVDMR9_DPDINFC+1
#define _DVDMR9_STMH                DVDMR9_DPDBST+1
#define _DVDMR9_STML                _DVDMR9_STMH+1
#define _DVDMR9_Layertime0H         _DVDMR9_STML+1
#define _DVDMR9_Layertime0L         _DVDMR9_Layertime0H+1
#define _DVDMR9_Layertime1H         _DVDMR9_Layertime0L+1
#define _DVDMR9_Layertime1L         _DVDMR9_Layertime1H+1
#define _DVDMR9_maxSRFOpp_0dBH      _DVDMR9_Layertime1L+1
#define _DVDMR9_maxSRFOpp_0dBL      _DVDMR9_maxSRFOpp_0dBH+1
#define DVDMR9_TEBSRatioX100       _DVDMR9_maxSRFOpp_0dBL+1
#define _DVDMR9_svoRunOutH          DVDMR9_TEBSRatioX100+1
#define _DVDMR9_svoRunOutL          _DVDMR9_svoRunOutH+1
#define _DVDMR9_SAL0H               _DVDMR9_svoRunOutL+1
#define _DVDMR9_SAL0L               _DVDMR9_SAL0H+1
#define _DVDMR9_FEBCcurH            _DVDMR9_SAL0L+1
#define _DVDMR9_FEBCcurL            _DVDMR9_FEBCcurH+1
#define _DVDMR9_TILTcurH            _DVDMR9_FEBCcurL+1
#define _DVDMR9_TILTcurL            _DVDMR9_TILTcurH+1

#define DVDMR9_RFAGCL1             _DVDMR9_TILTcurL+1
#define DVDMR9_RFAOCL1             DVDMR9_RFAGCL1+1
#define DVDMR9_SPP1OSL1            DVDMR9_RFAOCL1+1
#define DVDMR9_SPP1PGNL1           DVDMR9_SPP1OSL1+1
#define DVDMR9_SPP1BGL1            DVDMR9_SPP1PGNL1+1
#define DVDMR9_MPPOSL1             DVDMR9_SPP1BGL1+1
#define DVDMR9_MPPOBGL1            DVDMR9_MPPOSL1+1
#define DVDMR9_MPPOPGNL1           DVDMR9_MPPOBGL1+1
#define DVDMR9_CEOSL1              DVDMR9_MPPOPGNL1+1
#define DVDMR9_CEPGNL1             DVDMR9_CEOSL1+1
#define DVDMR9_CEGNL1              DVDMR9_CEPGNL1+1
#define DVDMR9_CEBGL1              DVDMR9_CEGNL1+1
#define DVDMR9_TRKOL1              DVDMR9_CEBGL1+1
#define DVDMR9_TRKGL1              DVDMR9_TRKOL1+1
#define DVDMR9_TEPGNL1             DVDMR9_TRKGL1+1
#define DVDMR9_TEBGL1              DVDMR9_TEPGNL1+1
#define DVDMR9_TE2OSL1             DVDMR9_TEBGL1+1
#define DVDMR9_TE2GNL1             DVDMR9_TE2OSL1+1
#define DVDMR9_RPOSL1              DVDMR9_TE2GNL1+1
#define DVDMR9_RPPGNL1             DVDMR9_RPOSL1+1
#define DVDMR9_RPBGL1              DVDMR9_RPPGNL1+1
#define DVDMR9_RPGNL1              DVDMR9_RPBGL1+1
#define DVDMR9_RP2OSL1             DVDMR9_RPGNL1+1
#define DVDMR9_RP2GNL1             DVDMR9_RP2OSL1+1
#define _DVDMR9_FEloopgainL1H       DVDMR9_RP2GNL1+1
#define _DVDMR9_FEloopgainL1L       _DVDMR9_FEloopgainL1H+1
#define _DVDMR9_TEloopgainL1H       _DVDMR9_FEloopgainL1L+1
#define _DVDMR9_TEloopgainL1L       _DVDMR9_TEloopgainL1H+1
#define _DVDMR9_FEBCL1H             _DVDMR9_TEloopgainL1L+1
#define _DVDMR9_FEBCL1L             _DVDMR9_FEBCL1H+1
#define _DVDMR9_TILTL1H             _DVDMR9_FEBCL1L+1
#define _DVDMR9_TILTL1L             _DVDMR9_TILTL1H+1
#define _DVDMR9_SAL0L1H             _DVDMR9_TILTL1L+1
#define _DVDMR9_SAL0L1L             _DVDMR9_SAL0L1H+1
#define _DVDMR9_FEBCcurL1H          _DVDMR9_SAL0L1L+1
#define _DVDMR9_FEBCcurL1L          _DVDMR9_FEBCcurL1H+1
#define _DVDMR9_TILTcurL1H          _DVDMR9_FEBCcurL1L+1
#define _DVDMR9_TILTcurL1L          _DVDMR9_TILTcurL1H+1
#define _DVDMR9_ADCVRPOWH           _DVDMR9_TILTcurL1L+1
#define _DVDMR9_ADCVRPOWL          _DVDMR9_ADCVRPOWH+1
#define _DVDMR9_VDELTAXH           _DVDMR9_ADCVRPOWL+1
#define _DVDMR9_VDELTAXL           _DVDMR9_VDELTAXH+1
#define _DVDMR9_SETPOINTH          _DVDMR9_VDELTAXL+1
#define _DVDMR9_SETPOINTL          _DVDMR9_SETPOINTH+1
#define _DVDMR9_FEPPL0H            _DVDMR9_SETPOINTL+1
#define _DVDMR9_FEPPL0L            _DVDMR9_FEPPL0H+1
#define _DVDMR9_FEPPL1H            _DVDMR9_FEPPL0L+1
#define _DVDMR9_FEPPL1L            _DVDMR9_FEPPL1H+1
#define _DVDMR9_LTHRPJGLVLH        _DVDMR9_FEPPL1L+1
#define _DVDMR9_LTHRPJGLVLL        _DVDMR9_LTHRPJGLVLH+1
#define _DVDMR9_RFppH              _DVDMR9_LTHRPJGLVLL+1
#define _DVDMR9_RFppL              _DVDMR9_RFppH+1
#define _DVDMR9_HOMEINDEXH         _DVDMR9_RFppL+1
#define _DVDMR9_HOMEINDEXL         _DVDMR9_HOMEINDEXH+1
#define _DVDMR9_UNBMSH             _DVDMR9_HOMEINDEXL+1
#define _DVDMR9_UNBMSL             _DVDMR9_UNBMSH+1
#define _DVDMR9_UNBHSH             _DVDMR9_UNBMSL+1
#define _DVDMR9_UNBHSL             _DVDMR9_UNBHSH+1
#define _DVDMR9_FGPULSECOUNTH           _DVDMR9_UNBHSL+1
#define _DVDMR9_FGPULSECOUNTL           _DVDMR9_FGPULSECOUNTH+1
#define DVDMR9_wbllockcnt          _DVDMR9_FGPULSECOUNTL+1
#define DVDMR9_jitter              DVDMR9_wbllockcnt+1
#define _DVDMR9_APPDPDPPRatioX100H DVDMR9_jitter+1
#define _DVDMR9_APPDPDPPRatioX100L _DVDMR9_APPDPDPPRatioX100H+1


/**********BDROM**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDROM_header              0x0900
#define BDROM_data_col            BDROM_header+7
#define BDROM_lastcol_bytenum     BDROM_data_col+1
#define BDROM_sensekey            BDROM_lastcol_bytenum+5
#define BDROM_ASC                 BDROM_sensekey+1
#define BDROM_ASCQ                BDROM_ASC+1
#define _BDROM_VREFH               BDROM_ASCQ+0x11
#define _BDROM_VREFL               _BDROM_VREFH+1
#define _BDROM_V21H                _BDROM_VREFL+1
#define _BDROM_V21L                _BDROM_V21H+1
#define BDROM_RFINGN              _BDROM_V21L+1
#define BDROM_RFCOSPO             BDROM_RFINGN+1
#define BDROM_RFCOSNO             BDROM_RFCOSPO+1
#define BDROM_RFGNO               BDROM_RFCOSNO+1
#define BDROM_RFAGC               BDROM_RFGNO+1
#define BDROM_RFAOC               BDROM_RFAGC+1
#define _BDROM_maxFEpp_0dBH       BDROM_RFAOC+1             //20090507_h.30
#define _BDROM_maxFEpp_0dBL       _BDROM_maxFEpp_0dBH+1     //20090507_h.30
#define BDROM_RFPOS               _BDROM_maxFEpp_0dBL+1     //20090507_h.30
#define BDROM_MBVREFOS            BDROM_RFPOS+1
#define BDROM_SBVREFOS            BDROM_MBVREFOS+1
#define BDROM_MBGUPSEL            BDROM_SBVREFOS+1
#define BDROM_SBGUPSEL            BDROM_MBGUPSEL+1
#define BDROM_FEOOS               BDROM_SBGUPSEL+1
#define BDROM_FEOBG               BDROM_FEOOS+1
#define BDROM_FEOPGN              BDROM_FEOBG+1
#define BDROM_SPP1OS              BDROM_FEOPGN+1
#define BDROM_SPP1PGN             BDROM_SPP1OS+1
#define BDROM_SPP1BG              BDROM_SPP1PGN+1
#define BDROM_SPP2OS              BDROM_SPP1BG+1
#define BDROM_SPP2PGN             BDROM_SPP2OS+1
#define BDROM_SPP2BG              BDROM_SPP2PGN+1
#define BDROM_SBADOS              BDROM_SPP2BG+1
#define BDROM_SBADGN              BDROM_SBADOS+1
#define BDROM_FCSO                BDROM_SBADGN+1
#define BDROM_FCSG                BDROM_FCSO+1
#define BDROM_FEPGN               BDROM_FCSG+1
#define BDROM_FEBG                BDROM_FEPGN+1
#define BDROM_FE2OS               BDROM_FEBG+1
#define BDROM_FE2GN               BDROM_FE2OS+1
#define BDROM_SRFOS               BDROM_FE2GN+1
#define BDROM_SRFOBG              BDROM_SRFOS+1
#define BDROM_MPPOS               BDROM_SRFOBG+1
#define BDROM_MPPOBG              BDROM_MPPOS+1
#define BDROM_MPPOPGN             BDROM_MPPOBG+1
#define BDROM_CEOS                BDROM_MPPOPGN+1
#define BDROM_CEPGN               BDROM_CEOS+1
#define BDROM_CEGN                BDROM_CEPGN+1
#define BDROM_CEBG                BDROM_CEGN+1
#define BDROM_TRKO                BDROM_CEBG+1
#define BDROM_TRKG                BDROM_TRKO+1
#define BDROM_TEPGN               BDROM_TRKG+1
#define BDROM_TEBG                BDROM_TEPGN+1
#define BDROM_TE2OS               BDROM_TEBG+1
#define BDROM_TE2GN               BDROM_TE2OS+1
#define BDROM_BSKO                BDROM_TE2GN+1
#define BDROM_BSKG                BDROM_BSKO+1
#define _BDROM_MPPO_0dBH          BDROM_BSKG+1              //20090507_h.30
#define _BDROM_MPPO_0dBL          _BDROM_MPPO_0dBH+1        //20090507_h.30
#define BDROM_BS2OS               _BDROM_MPPO_0dBL+1        //20090507_h.30
#define BDROM_MPPOSRFORatioX100   BDROM_BS2OS+1             //20090507_h.30
#define BDROM_PHSOS               BDROM_MPPOSRFORatioX100+1 //20090507_h.30
#define BDROM_BHSOS               BDROM_PHSOS+1
#define _BDROM_FMD_PPLVL_CDH       BDROM_BHSOS+1
#define _BDROM_FMD_PPLVL_CDL      _BDROM_FMD_PPLVL_CDH +1
#define _BDROM_FMD_PPLVL_DVDH     _BDROM_FMD_PPLVL_CDL +1
#define _BDROM_FMD_PPLVL_DVDL     _BDROM_FMD_PPLVL_DVDH+1
#define _BDROM_FMD_PPLVL_BDH      _BDROM_FMD_PPLVL_DVDL+1
#define _BDROM_FMD_PPLVL_BDL      _BDROM_FMD_PPLVL_BDH +1
#define _BDROM_FMD_DCLVL_CDH      _BDROM_FMD_PPLVL_BDL +1
#define _BDROM_FMD_DCLVL_CDL      _BDROM_FMD_DCLVL_CDH +1
#define _BDROM_FMD_DCLVL_DVDH     _BDROM_FMD_DCLVL_CDL +1
#define _BDROM_FMD_DCLVL_DVDL     _BDROM_FMD_DCLVL_DVDH+1
#define _BDROM_FMD_DCLVL_BDH      _BDROM_FMD_DCLVL_DVDL+1
#define _BDROM_FMD_DCLVL_BDL      _BDROM_FMD_DCLVL_BDH +1
#define _BDROM_FEloopgainH        _BDROM_FMD_DCLVL_BDL +1
#define _BDROM_FEloopgainL         _BDROM_FEloopgainH+1
#define _BDROM_TEloopgainH         _BDROM_FEloopgainL+1
#define _BDROM_TEloopgainL         _BDROM_TEloopgainH+1
#define _BDROM_FEBCH               _BDROM_TEloopgainL+1
#define _BDROM_FEBCL               _BDROM_FEBCH+1
#define _BDROM_TILTH               _BDROM_FEBCL+1
#define _BDROM_TILTL               _BDROM_TILTH+1
#define _BDROM_DSPCEOFFSETH        _BDROM_TILTL+1
#define _BDROM_DSPCEOFFSETL        _BDROM_DSPCEOFFSETH+1
#define _BDROM_DSPFEOFFSETH        _BDROM_DSPCEOFFSETL+1
#define _BDROM_DSPFEOFFSETL        _BDROM_DSPFEOFFSETH+1
#define _BDROM_DSPTEOFFSETH        _BDROM_DSPFEOFFSETL+1
#define _BDROM_DSPTEOFFSETL        _BDROM_DSPTEOFFSETH+1
#define _BDROM_DSPRPOFFSETH        _BDROM_DSPTEOFFSETL+1
#define _BDROM_DSPRPOFFSETL        _BDROM_DSPRPOFFSETH+1
#define BDROM_TROOS               _BDROM_DSPRPOFFSETL+1
#define BDROM_M                    BDROM_TROOS+1
#define BDROM_DPDCH1OS             BDROM_M+1
#define BDROM_DPDCH2OS            BDROM_DPDCH1OS+1
#define BDROM_DPDCH3OS            BDROM_DPDCH2OS+1
#define BDROM_DPDCH4OS            BDROM_DPDCH3OS+1
#define BDROM_DPDGN               BDROM_DPDCH4OS+1
#define BDROM_DPDGMOS             BDROM_DPDGN+1
#define BDROM_RFGMOS              BDROM_DPDGMOS+1
#define BDROM_DPDHSSEL            BDROM_RFGMOS+1
#define BDROM_DPDHSFCH            BDROM_DPDHSSEL+1
#define BDROM_DPDHSFCL            BDROM_DPDHSFCH+1
#define BDROM_DPDHSISCL           BDROM_DPDHSFCL+1
#define BDROM_DPDINFC             BDROM_DPDHSISCL+1
#define BDROM_DPDBST              BDROM_DPDINFC+1
#define _BDROM_STMH               BDROM_DPDBST+1
#define _BDROM_STML               _BDROM_STMH+1
#define _BDROM_Layertime0H        _BDROM_STML+1
#define _BDROM_Layertime0L        _BDROM_Layertime0H+1
#define _BDROM_Layertime1H        _BDROM_Layertime0L+1
#define _BDROM_Layertime1L        _BDROM_Layertime1H+1
#define _BDROM_maxSRFOpp_0dBH     _BDROM_Layertime1L+1
#define _BDROM_maxSRFOpp_0dBL     _BDROM_maxSRFOpp_0dBH+1
#define BDROM_TEBSRatioX100      _BDROM_maxSRFOpp_0dBL+1
#define _BDROM_svoRunOutH         BDROM_TEBSRatioX100+1
#define _BDROM_svoRunOutL         _BDROM_svoRunOutH+1
#define _BDROM_SAL0H              _BDROM_svoRunOutL+1
#define _BDROM_SAL0L              _BDROM_SAL0H+1
#define _BDROM_FEBCcurH           _BDROM_SAL0L+1
#define _BDROM_FEBCcurL           _BDROM_FEBCcurH+1
#define _BDROM_TILTcurH           _BDROM_FEBCcurL+1
#define _BDROM_TILTcurL           _BDROM_TILTcurH+1
#define _BDROM_ADCVRPOWH          _BDROM_TILTcurL+1
#define _BDROM_ADCVRPOWL          _BDROM_ADCVRPOWH+1
#define _BDROM_VDELTAXH           _BDROM_ADCVRPOWL+1
#define _BDROM_VDELTAXL           _BDROM_VDELTAXH+1
#define _BDROM_SETPOINTH          _BDROM_VDELTAXL+1
#define _BDROM_SETPOINTL          _BDROM_SETPOINTH+1
#define _BDROM_FEPPL0H            _BDROM_SETPOINTL+1
#define _BDROM_FEPPL0L            _BDROM_FEPPL0H+1
#define _BDROM_FEPPL1H            _BDROM_FEPPL0L+1
#define _BDROM_FEPPL1L            _BDROM_FEPPL1H+1
#define _BDROM_LTHRPJGLVLH        _BDROM_FEPPL1L+1
#define _BDROM_LTHRPJGLVLL        _BDROM_LTHRPJGLVLH+1
#define _BDROM_RFppH              _BDROM_LTHRPJGLVLL+1
#define _BDROM_RFppL              _BDROM_RFppH+1
#define _BDROM_HOMEINDEXH         _BDROM_RFppL+1
#define _BDROM_HOMEINDEXL         _BDROM_HOMEINDEXH+1
#define _BDROM_UNBMSH             _BDROM_HOMEINDEXL+1
#define _BDROM_UNBMSL             _BDROM_UNBMSH+1
#define _BDROM_UNBHSH             _BDROM_UNBMSL+1
#define _BDROM_UNBHSL             _BDROM_UNBHSH+1
#define _BDROM_FGPULSECOUNTH           _BDROM_UNBHSL+1
#define _BDROM_FGPULSECOUNTL           _BDROM_FGPULSECOUNTH+1
#define BDROM_wbllockcnt          _BDROM_FGPULSECOUNTL+1
#define BDROM_jitter              BDROM_wbllockcnt+1
#define _BDROM_APPDPDPPRatioX100H BDROM_jitter+1
#define _BDROM_APPDPDPPRatioX100L _BDROM_APPDPDPPRatioX100H+1

/**********BDROMDL**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDROMDL_header              0x09B0
#define BDROMDL_data_col            BDROMDL_header+7
#define BDROMDL_lastcol_bytenum     BDROMDL_data_col+1
#define BDROMDL_sensekey            BDROMDL_lastcol_bytenum+5
#define BDROMDL_ASC                 BDROMDL_sensekey+1
#define BDROMDL_ASCQ                BDROMDL_ASC+1
#define _BDROMDL_VREFH               BDROMDL_ASCQ+0x11
#define _BDROMDL_VREFL               _BDROMDL_VREFH+1
#define _BDROMDL_V21H                _BDROMDL_VREFL+1
#define _BDROMDL_V21L                _BDROMDL_V21H+1
#define BDROMDL_RFINGN              _BDROMDL_V21L+1
#define BDROMDL_RFCOSPO             BDROMDL_RFINGN+1
#define BDROMDL_RFCOSNO             BDROMDL_RFCOSPO+1
#define BDROMDL_RFGNO               BDROMDL_RFCOSNO+1
#define BDROMDL_RFAGC               BDROMDL_RFGNO+1
#define BDROMDL_RFAOC               BDROMDL_RFAGC+1
#define _BDROMDL_maxFEpp_0dBH       BDROMDL_RFAOC+1         //20090507_h.30
#define _BDROMDL_maxFEpp_0dBL       _BDROMDL_maxFEpp_0dBH+1 //20090507_h.30
#define BDROMDL_RFPOS               _BDROMDL_maxFEpp_0dBL+1 //20090507_h.30
#define BDROMDL_MBVREFOS            BDROMDL_RFPOS+1
#define BDROMDL_SBVREFOS            BDROMDL_MBVREFOS+1
#define BDROMDL_MBGUPSEL            BDROMDL_SBVREFOS+1
#define BDROMDL_SBGUPSEL            BDROMDL_MBGUPSEL+1
#define BDROMDL_FEOOS               BDROMDL_SBGUPSEL+1
#define BDROMDL_FEOBG               BDROMDL_FEOOS+1
#define BDROMDL_FEOPGN              BDROMDL_FEOBG+1
#define BDROMDL_SPP1OS              BDROMDL_FEOPGN+1
#define BDROMDL_SPP1PGN             BDROMDL_SPP1OS+1
#define BDROMDL_SPP1BG              BDROMDL_SPP1PGN+1
#define BDROMDL_SPP2OS              BDROMDL_SPP1BG+1
#define BDROMDL_SPP2PGN             BDROMDL_SPP2OS+1
#define BDROMDL_SPP2BG              BDROMDL_SPP2PGN+1
#define BDROMDL_SBADOS              BDROMDL_SPP2BG+1
#define BDROMDL_SBADGN              BDROMDL_SBADOS+1
#define BDROMDL_FCSO                BDROMDL_SBADGN+1
#define BDROMDL_FCSG                BDROMDL_FCSO+1
#define BDROMDL_FEPGN               BDROMDL_FCSG+1
#define BDROMDL_FEBG                BDROMDL_FEPGN+1
#define BDROMDL_FE2OS               BDROMDL_FEBG+1
#define BDROMDL_FE2GN               BDROMDL_FE2OS+1
#define BDROMDL_SRFOS               BDROMDL_FE2GN+1
#define BDROMDL_SRFOBG              BDROMDL_SRFOS+1
#define BDROMDL_MPPOS               BDROMDL_SRFOBG+1
#define BDROMDL_MPPOBG              BDROMDL_MPPOS+1
#define BDROMDL_MPPOPGN             BDROMDL_MPPOBG+1
#define BDROMDL_CEOS                BDROMDL_MPPOPGN+1
#define BDROMDL_CEPGN               BDROMDL_CEOS+1
#define BDROMDL_CEGN                BDROMDL_CEPGN+1
#define BDROMDL_CEBG                BDROMDL_CEGN+1
#define BDROMDL_TRKO                BDROMDL_CEBG+1
#define BDROMDL_TRKG                BDROMDL_TRKO+1
#define BDROMDL_TEPGN               BDROMDL_TRKG+1
#define BDROMDL_TEBG                BDROMDL_TEPGN+1
#define BDROMDL_TE2OS               BDROMDL_TEBG+1
#define BDROMDL_TE2GN               BDROMDL_TE2OS+1
#define BDROMDL_BSKO                BDROMDL_TE2GN+1
#define BDROMDL_BSKG                BDROMDL_BSKO+1
#define _BDROMDL_MPPO_0dBH          BDROMDL_BSKG+1          //20090507_h.30
#define _BDROMDL_MPPO_0dBL          _BDROMDL_MPPO_0dBH+1    //20090507_h.30
#define BDROMDL_BS2OS               _BDROMDL_MPPO_0dBL+1    //20090507_h.30
#define BDROMDL_MPPOSRFORatioX100   BDROMDL_BS2OS+1         //20090507_h.30
#define BDROMDL_PHSOS               BDROMDL_MPPOSRFORatioX100+1//20090507_h.30
#define BDROMDL_BHSOS               BDROMDL_PHSOS+1
#define _BDROMDL_FMD_PPLVL_CDH       BDROMDL_BHSOS+1
#define _BDROMDL_FMD_PPLVL_CDL      _BDROMDL_FMD_PPLVL_CDH +1
#define _BDROMDL_FMD_PPLVL_DVDH     _BDROMDL_FMD_PPLVL_CDL +1
#define _BDROMDL_FMD_PPLVL_DVDL     _BDROMDL_FMD_PPLVL_DVDH+1
#define _BDROMDL_FMD_PPLVL_BDH      _BDROMDL_FMD_PPLVL_DVDL+1
#define _BDROMDL_FMD_PPLVL_BDL      _BDROMDL_FMD_PPLVL_BDH +1
#define _BDROMDL_FMD_DCLVL_CDH      _BDROMDL_FMD_PPLVL_BDL +1
#define _BDROMDL_FMD_DCLVL_CDL      _BDROMDL_FMD_DCLVL_CDH +1
#define _BDROMDL_FMD_DCLVL_DVDH     _BDROMDL_FMD_DCLVL_CDL +1
#define _BDROMDL_FMD_DCLVL_DVDL     _BDROMDL_FMD_DCLVL_DVDH+1
#define _BDROMDL_FMD_DCLVL_BDH      _BDROMDL_FMD_DCLVL_DVDL+1
#define _BDROMDL_FMD_DCLVL_BDL      _BDROMDL_FMD_DCLVL_BDH +1
#define _BDROMDL_FEloopgainH        _BDROMDL_FMD_DCLVL_BDL +1
#define _BDROMDL_FEloopgainL         _BDROMDL_FEloopgainH+1
#define _BDROMDL_TEloopgainH         _BDROMDL_FEloopgainL+1
#define _BDROMDL_TEloopgainL         _BDROMDL_TEloopgainH+1
#define _BDROMDL_FEBCH               _BDROMDL_TEloopgainL+1
#define _BDROMDL_FEBCL               _BDROMDL_FEBCH+1
#define _BDROMDL_TILTH               _BDROMDL_FEBCL+1
#define _BDROMDL_TILTL               _BDROMDL_TILTH+1
#define _BDROMDL_DSPCEOFFSETH        _BDROMDL_TILTL+1
#define _BDROMDL_DSPCEOFFSETL        _BDROMDL_DSPCEOFFSETH+1
#define _BDROMDL_DSPFEOFFSETH        _BDROMDL_DSPCEOFFSETL+1
#define _BDROMDL_DSPFEOFFSETL        _BDROMDL_DSPFEOFFSETH+1
#define _BDROMDL_DSPTEOFFSETH        _BDROMDL_DSPFEOFFSETL+1
#define _BDROMDL_DSPTEOFFSETL        _BDROMDL_DSPTEOFFSETH+1
#define _BDROMDL_DSPRPOFFSETH        _BDROMDL_DSPTEOFFSETL+1
#define _BDROMDL_DSPRPOFFSETL        _BDROMDL_DSPRPOFFSETH+1
#define BDROMDL_TROOS               _BDROMDL_DSPRPOFFSETL+1
#define BDROMDL_M                    BDROMDL_TROOS+1
#define BDROMDL_DPDCH1OS             BDROMDL_M+1
#define BDROMDL_DPDCH2OS            BDROMDL_DPDCH1OS+1
#define BDROMDL_DPDCH3OS            BDROMDL_DPDCH2OS+1
#define BDROMDL_DPDCH4OS            BDROMDL_DPDCH3OS+1
#define BDROMDL_DPDGN               BDROMDL_DPDCH4OS+1
#define BDROMDL_DPDGMOS             BDROMDL_DPDGN+1
#define BDROMDL_RFGMOS              BDROMDL_DPDGMOS+1
#define BDROMDL_DPDHSSEL            BDROMDL_RFGMOS+1
#define BDROMDL_DPDHSFCH            BDROMDL_DPDHSSEL+1
#define BDROMDL_DPDHSFCL            BDROMDL_DPDHSFCH+1
#define BDROMDL_DPDHSISCL           BDROMDL_DPDHSFCL+1
#define BDROMDL_DPDINFC             BDROMDL_DPDHSISCL+1
#define BDROMDL_DPDBST              BDROMDL_DPDINFC+1
#define _BDROMDL_STMH                BDROMDL_DPDBST+1
#define _BDROMDL_STML                _BDROMDL_STMH+1
#define _BDROMDL_Layertime0H         _BDROMDL_STML+1
#define _BDROMDL_Layertime0L         _BDROMDL_Layertime0H+1
#define _BDROMDL_Layertime1H         _BDROMDL_Layertime0L+1
#define _BDROMDL_Layertime1L         _BDROMDL_Layertime1H+1
#define _BDROMDL_maxSRFOpp_0dBH      _BDROMDL_Layertime1L+1
#define _BDROMDL_maxSRFOpp_0dBL      _BDROMDL_maxSRFOpp_0dBH+1
#define BDROMDL_TEBSRatioX100       _BDROMDL_maxSRFOpp_0dBL+1
#define _BDROMDL_svoRunOutH          BDROMDL_TEBSRatioX100+1
#define _BDROMDL_svoRunOutL          _BDROMDL_svoRunOutH+1
#define _BDROMDL_SAL0H               _BDROMDL_svoRunOutL+1
#define _BDROMDL_SAL0L               _BDROMDL_SAL0H+1
#define _BDROMDL_FEBCcurH            _BDROMDL_SAL0L+1
#define _BDROMDL_FEBCcurL            _BDROMDL_FEBCcurH+1
#define _BDROMDL_TILTcurH            _BDROMDL_FEBCcurL+1
#define _BDROMDL_TILTcurL            _BDROMDL_TILTcurH+1

#define BDROMDL_RFAGCL1             _BDROMDL_TILTcurL+1
#define BDROMDL_RFAOCL1             BDROMDL_RFAGCL1+1
#define BDROMDL_SPP1OSL1            BDROMDL_RFAOCL1+1
#define BDROMDL_SPP1PGNL1           BDROMDL_SPP1OSL1+1
#define BDROMDL_SPP1BGL1            BDROMDL_SPP1PGNL1+1
#define BDROMDL_MPPOSL1             BDROMDL_SPP1BGL1+1
#define BDROMDL_MPPOBGL1            BDROMDL_MPPOSL1+1
#define BDROMDL_MPPOPGNL1           BDROMDL_MPPOBGL1+1
#define BDROMDL_CEOSL1              BDROMDL_MPPOPGNL1+1
#define BDROMDL_CEPGNL1             BDROMDL_CEOSL1+1
#define BDROMDL_CEGNL1              BDROMDL_CEPGNL1+1
#define BDROMDL_CEBGL1              BDROMDL_CEGNL1+1
#define BDROMDL_TRKOL1              BDROMDL_CEBGL1+1
#define BDROMDL_TRKGL1              BDROMDL_TRKOL1+1
#define BDROMDL_TEPGNL1             BDROMDL_TRKGL1+1
#define BDROMDL_TEBGL1              BDROMDL_TEPGNL1+1
#define BDROMDL_TE2OSL1             BDROMDL_TEBGL1+1
#define BDROMDL_TE2GNL1             BDROMDL_TE2OSL1+1
#define BDROMDL_RPOSL1              BDROMDL_TE2GNL1+1
#define BDROMDL_RPPGNL1             BDROMDL_RPOSL1+1
#define BDROMDL_RPBGL1              BDROMDL_RPPGNL1+1
#define BDROMDL_RPGNL1              BDROMDL_RPBGL1+1
#define BDROMDL_RP2OSL1             BDROMDL_RPGNL1+1
#define BDROMDL_RP2GNL1             BDROMDL_RP2OSL1+1
#define _BDROMDL_FEloopgainL1H       BDROMDL_RP2GNL1+1
#define _BDROMDL_FEloopgainL1L       _BDROMDL_FEloopgainL1H+1
#define _BDROMDL_TEloopgainL1H       _BDROMDL_FEloopgainL1L+1
#define _BDROMDL_TEloopgainL1L       _BDROMDL_TEloopgainL1H+1
#define _BDROMDL_FEBCL1H             _BDROMDL_TEloopgainL1L+1
#define _BDROMDL_FEBCL1L             _BDROMDL_FEBCL1H+1
#define _BDROMDL_TILTL1H             _BDROMDL_FEBCL1L+1
#define _BDROMDL_TILTL1L             _BDROMDL_TILTL1H+1
#define _BDROMDL_SAL0L1H             _BDROMDL_TILTL1L+1
#define _BDROMDL_SAL0L1L             _BDROMDL_SAL0L1H+1
#define _BDROMDL_FEBCcurL1H          _BDROMDL_SAL0L1L+1
#define _BDROMDL_FEBCcurL1L          _BDROMDL_FEBCcurL1H+1
#define _BDROMDL_TILTcurL1H          _BDROMDL_FEBCcurL1L+1
#define _BDROMDL_TILTcurL1L          _BDROMDL_TILTcurL1H+1
#define _BDROMDL_ADCVRPOWH           _BDROMDL_TILTcurL1L+1
#define _BDROMDL_ADCVRPOWL          _BDROMDL_ADCVRPOWH+1
#define _BDROMDL_VDELTAXH           _BDROMDL_ADCVRPOWL+1
#define _BDROMDL_VDELTAXL           _BDROMDL_VDELTAXH+1
#define _BDROMDL_SETPOINTH          _BDROMDL_VDELTAXL+1
#define _BDROMDL_SETPOINTL          _BDROMDL_SETPOINTH+1
#define _BDROMDL_FEPPL0H            _BDROMDL_SETPOINTL+1
#define _BDROMDL_FEPPL0L            _BDROMDL_FEPPL0H+1
#define _BDROMDL_FEPPL1H            _BDROMDL_FEPPL0L+1
#define _BDROMDL_FEPPL1L            _BDROMDL_FEPPL1H+1
#define _BDROMDL_LTHRPJGLVLH        _BDROMDL_FEPPL1L+1
#define _BDROMDL_LTHRPJGLVLL        _BDROMDL_LTHRPJGLVLH+1
#define _BDROMDL_RFppH              _BDROMDL_LTHRPJGLVLL+1
#define _BDROMDL_RFppL              _BDROMDL_RFppH+1
#define _BDROMDL_HOMEINDEXH         _BDROMDL_RFppL+1
#define _BDROMDL_HOMEINDEXL         _BDROMDL_HOMEINDEXH+1
#define _BDROMDL_UNBMSH             _BDROMDL_HOMEINDEXL+1
#define _BDROMDL_UNBMSL             _BDROMDL_UNBMSH+1
#define _BDROMDL_UNBHSH             _BDROMDL_UNBMSL+1
#define _BDROMDL_UNBHSL             _BDROMDL_UNBHSH+1
#define _BDROMDL_FGPULSECOUNTH           _BDROMDL_UNBHSL+1
#define _BDROMDL_FGPULSECOUNTL           _BDROMDL_FGPULSECOUNTH+1
#define BDROMDL_wbllockcnt          _BDROMDL_FGPULSECOUNTL+1
#define BDROMDL_jitter              BDROMDL_wbllockcnt+1
#define _BDROMDL_APPDPDPPRatioX100H BDROMDL_jitter+1
#define _BDROMDL_APPDPDPPRatioX100L _BDROMDL_APPDPDPPRatioX100H+1

/**********BDR**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDR_header              0x0A90
#define BDR_data_col            BDR_header+7
#define BDR_lastcol_bytenum     BDR_data_col+1
#define BDR_sensekey            BDR_lastcol_bytenum+5
#define BDR_ASC                 BDR_sensekey+1
#define BDR_ASCQ                BDR_ASC+1
#define _BDR_VREFH               BDR_ASCQ+0x11
#define _BDR_VREFL               _BDR_VREFH+1
#define _BDR_V21H                _BDR_VREFL+1
#define _BDR_V21L                _BDR_V21H+1
#define BDR_RFINGN              _BDR_V21L+1
#define BDR_RFCOSPO             BDR_RFINGN+1
#define BDR_RFCOSNO             BDR_RFCOSPO+1
#define BDR_RFGNO               BDR_RFCOSNO+1
#define BDR_RFAGC               BDR_RFGNO+1
#define BDR_RFAOC               BDR_RFAGC+1
#define _BDR_maxFEpp_0dBH       BDR_RFAOC+1                 //20090507_h.30
#define _BDR_maxFEpp_0dBL       _BDR_maxFEpp_0dBH+1         //20090507_h.30
#define BDR_RFPOS               _BDR_maxFEpp_0dBL+1         //20090507_h.30
#define BDR_MBVREFOS            BDR_RFPOS+1
#define BDR_SBVREFOS            BDR_MBVREFOS+1
#define BDR_MBGUPSEL            BDR_SBVREFOS+1
#define BDR_SBGUPSEL            BDR_MBGUPSEL+1
#define BDR_FEOOS               BDR_SBGUPSEL+1
#define BDR_FEOBG               BDR_FEOOS+1
#define BDR_FEOPGN              BDR_FEOBG+1
#define BDR_SPP1OS              BDR_FEOPGN+1
#define BDR_SPP1PGN             BDR_SPP1OS+1
#define BDR_SPP1BG              BDR_SPP1PGN+1
#define BDR_SPP2OS              BDR_SPP1BG+1
#define BDR_SPP2PGN             BDR_SPP2OS+1
#define BDR_SPP2BG              BDR_SPP2PGN+1
#define BDR_SBADOS              BDR_SPP2BG+1
#define BDR_SBADGN              BDR_SBADOS+1
#define BDR_FCSO                BDR_SBADGN+1
#define BDR_FCSG                BDR_FCSO+1
#define BDR_FEPGN               BDR_FCSG+1
#define BDR_FEBG                BDR_FEPGN+1
#define BDR_FE2OS               BDR_FEBG+1
#define BDR_FE2GN               BDR_FE2OS+1
#define BDR_SRFOS               BDR_FE2GN+1
#define BDR_SRFOBG              BDR_SRFOS+1
#define BDR_MPPOS               BDR_SRFOBG+1
#define BDR_MPPOBG              BDR_MPPOS+1
#define BDR_MPPOPGN             BDR_MPPOBG+1
#define BDR_CEOS                BDR_MPPOPGN+1
#define BDR_CEPGN               BDR_CEOS+1
#define BDR_CEGN                BDR_CEPGN+1
#define BDR_CEBG                BDR_CEGN+1
#define BDR_TRKO                BDR_CEBG+1
#define BDR_TRKG                BDR_TRKO+1
#define BDR_TEPGN               BDR_TRKG+1
#define BDR_TEBG                BDR_TEPGN+1
#define BDR_TE2OS               BDR_TEBG+1
#define BDR_TE2GN               BDR_TE2OS+1
#define BDR_BSKO                BDR_TE2GN+1
#define BDR_BSKG                BDR_BSKO+1
#define _BDR_MPPO_0dBH          BDR_BSKG+1                  //20090507_h.30
#define _BDR_MPPO_0dBL          _BDR_MPPO_0dBH+1            //20090507_h.30
#define BDR_BS2OS               _BDR_MPPO_0dBL+1            //20090507_h.30
#define BDR_MPPOSRFORatioX100   BDR_BS2OS+1                 //20090507_h.30
#define BDR_PHSOS               BDR_MPPOSRFORatioX100+1     //20090507_h.30
#define BDR_BHSOS               BDR_PHSOS+1
#define _BDR_FMD_PPLVL_CDH       BDR_BHSOS+1
#define _BDR_FMD_PPLVL_CDL      _BDR_FMD_PPLVL_CDH +1
#define _BDR_FMD_PPLVL_DVDH     _BDR_FMD_PPLVL_CDL +1
#define _BDR_FMD_PPLVL_DVDL     _BDR_FMD_PPLVL_DVDH+1
#define _BDR_FMD_PPLVL_BDH      _BDR_FMD_PPLVL_DVDL+1
#define _BDR_FMD_PPLVL_BDL      _BDR_FMD_PPLVL_BDH +1
#define _BDR_FMD_DCLVL_CDH      _BDR_FMD_PPLVL_BDL +1
#define _BDR_FMD_DCLVL_CDL      _BDR_FMD_DCLVL_CDH +1
#define _BDR_FMD_DCLVL_DVDH     _BDR_FMD_DCLVL_CDL +1
#define _BDR_FMD_DCLVL_DVDL     _BDR_FMD_DCLVL_DVDH+1
#define _BDR_FMD_DCLVL_BDH      _BDR_FMD_DCLVL_DVDL+1
#define _BDR_FMD_DCLVL_BDL      _BDR_FMD_DCLVL_BDH +1
#define _BDR_FEloopgainH        _BDR_FMD_DCLVL_BDL +1
#define _BDR_FEloopgainL         _BDR_FEloopgainH+1
#define _BDR_TEloopgainH         _BDR_FEloopgainL+1
#define _BDR_TEloopgainL         _BDR_TEloopgainH+1
#define _BDR_FEBCH               _BDR_TEloopgainL+1
#define _BDR_FEBCL               _BDR_FEBCH+1
#define _BDR_TILTH               _BDR_FEBCL+1
#define _BDR_TILTL               _BDR_TILTH+1
#define _BDR_DSPCEOFFSETH        _BDR_TILTL+1
#define _BDR_DSPCEOFFSETL        _BDR_DSPCEOFFSETH+1
#define _BDR_DSPFEOFFSETH        _BDR_DSPCEOFFSETL+1
#define _BDR_DSPFEOFFSETL        _BDR_DSPFEOFFSETH+1
#define _BDR_DSPTEOFFSETH        _BDR_DSPFEOFFSETL+1
#define _BDR_DSPTEOFFSETL        _BDR_DSPTEOFFSETH+1
#define _BDR_DSPRPOFFSETH        _BDR_DSPTEOFFSETL+1
#define _BDR_DSPRPOFFSETL        _BDR_DSPRPOFFSETH+1
#define BDR_TROOS               _BDR_DSPRPOFFSETL+1
#define BDR_M                    BDR_TROOS+1
#define BDR_DPDCH1OS             BDR_M+1
#define BDR_DPDCH2OS            BDR_DPDCH1OS+1
#define BDR_DPDCH3OS            BDR_DPDCH2OS+1
#define BDR_DPDCH4OS            BDR_DPDCH3OS+1
#define BDR_DPDGN               BDR_DPDCH4OS+1
#define BDR_DPDGMOS             BDR_DPDGN+1
#define BDR_RFGMOS              BDR_DPDGMOS+1
#define BDR_DPDHSSEL            BDR_RFGMOS+1
#define BDR_DPDHSFCH            BDR_DPDHSSEL+1
#define BDR_DPDHSFCL            BDR_DPDHSFCH+1
#define BDR_DPDHSISCL           BDR_DPDHSFCL+1
#define BDR_DPDINFC             BDR_DPDHSISCL+1
#define BDR_DPDBST              BDR_DPDINFC+1
#define _BDR_STMH                BDR_DPDBST+1
#define _BDR_STML                _BDR_STMH+1
#define _BDR_Layertime0H         _BDR_STML+1
#define _BDR_Layertime0L         _BDR_Layertime0H+1
#define _BDR_Layertime1H         _BDR_Layertime0L+1
#define _BDR_Layertime1L         _BDR_Layertime1H+1
#define _BDR_maxSRFOpp_0dBH      _BDR_Layertime1L+1
#define _BDR_maxSRFOpp_0dBL      _BDR_maxSRFOpp_0dBH+1
#define BDR_TEBSRatioX100       _BDR_maxSRFOpp_0dBL+1
#define _BDR_svoRunOutH          BDR_TEBSRatioX100+1
#define _BDR_svoRunOutL          _BDR_svoRunOutH+1
#define _BDR_SAL0H               _BDR_svoRunOutL+1
#define _BDR_SAL0L               _BDR_SAL0H+1
#define _BDR_FEBCcurH            _BDR_SAL0L+1
#define _BDR_FEBCcurL            _BDR_FEBCcurH+1
#define _BDR_TILTcurH            _BDR_FEBCcurL+1
#define _BDR_TILTcurL            _BDR_TILTcurH+1
#define _BDR_ADCVRPOWH          _BDR_TILTcurL+1
#define _BDR_ADCVRPOWL          _BDR_ADCVRPOWH+1
#define _BDR_VDELTAXH           _BDR_ADCVRPOWL+1
#define _BDR_VDELTAXL           _BDR_VDELTAXH+1
#define _BDR_SETPOINTH          _BDR_VDELTAXL+1
#define _BDR_SETPOINTL          _BDR_SETPOINTH+1
#define _BDR_FEPPL0H            _BDR_SETPOINTL+1
#define _BDR_FEPPL0L            _BDR_FEPPL0H+1
#define _BDR_FEPPL1H            _BDR_FEPPL0L+1
#define _BDR_FEPPL1L            _BDR_FEPPL1H+1
#define _BDR_LTHRPJGLVLH        _BDR_FEPPL1L+1
#define _BDR_LTHRPJGLVLL        _BDR_LTHRPJGLVLH+1
#define _BDR_RFppH              _BDR_LTHRPJGLVLL+1
#define _BDR_RFppL              _BDR_RFppH+1
#define _BDR_HOMEINDEXH         _BDR_RFppL+1
#define _BDR_HOMEINDEXL         _BDR_HOMEINDEXH+1
#define _BDR_UNBMSH             _BDR_HOMEINDEXL+1
#define _BDR_UNBMSL             _BDR_UNBMSH+1
#define _BDR_UNBHSH             _BDR_UNBMSL+1
#define _BDR_UNBHSL             _BDR_UNBHSH+1
#define _BDR_FGPULSECOUNTH           _BDR_UNBHSL+1
#define _BDR_FGPULSECOUNTL           _BDR_FGPULSECOUNTH+1
#define BDR_wbllockcnt          _BDR_FGPULSECOUNTL+1
#define BDR_jitter              BDR_wbllockcnt+1
#define _BDR_APPDPDPPRatioX100H BDR_jitter+1
#define _BDR_APPDPDPPRatioX100L _BDR_APPDPDPPRatioX100H+1

/**********BDRDL**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDRDL_header              0x0B40
#define BDRDL_data_col            BDRDL_header+7
#define BDRDL_lastcol_bytenum     BDRDL_data_col+1
#define BDRDL_sensekey            BDRDL_lastcol_bytenum+5
#define BDRDL_ASC                 BDRDL_sensekey+1
#define BDRDL_ASCQ                BDRDL_ASC+1
#define _BDRDL_VREFH               BDRDL_ASCQ+0x11
#define _BDRDL_VREFL               _BDRDL_VREFH+1
#define _BDRDL_V21H                _BDRDL_VREFL+1
#define _BDRDL_V21L                _BDRDL_V21H+1
#define BDRDL_RFINGN              _BDRDL_V21L+1
#define BDRDL_RFCOSPO             BDRDL_RFINGN+1
#define BDRDL_RFCOSNO             BDRDL_RFCOSPO+1
#define BDRDL_RFGNO               BDRDL_RFCOSNO+1
#define BDRDL_RFAGC               BDRDL_RFGNO+1
#define BDRDL_RFAOC               BDRDL_RFAGC+1
#define _BDRDL_maxFEpp_0dBH       BDRDL_RFAOC+1             //20090507_h.30
#define _BDRDL_maxFEpp_0dBL       _BDRDL_maxFEpp_0dBH+1     //20090507_h.30
#define BDRDL_RFPOS               _BDRDL_maxFEpp_0dBL+1     //20090507_h.30
#define BDRDL_MBVREFOS            BDRDL_RFPOS+1
#define BDRDL_SBVREFOS            BDRDL_MBVREFOS+1
#define BDRDL_MBGUPSEL            BDRDL_SBVREFOS+1
#define BDRDL_SBGUPSEL            BDRDL_MBGUPSEL+1
#define BDRDL_FEOOS               BDRDL_SBGUPSEL+1
#define BDRDL_FEOBG               BDRDL_FEOOS+1
#define BDRDL_FEOPGN              BDRDL_FEOBG+1
#define BDRDL_SPP1OS              BDRDL_FEOPGN+1
#define BDRDL_SPP1PGN             BDRDL_SPP1OS+1
#define BDRDL_SPP1BG              BDRDL_SPP1PGN+1
#define BDRDL_SPP2OS              BDRDL_SPP1BG+1
#define BDRDL_SPP2PGN             BDRDL_SPP2OS+1
#define BDRDL_SPP2BG              BDRDL_SPP2PGN+1
#define BDRDL_SBADOS              BDRDL_SPP2BG+1
#define BDRDL_SBADGN              BDRDL_SBADOS+1
#define BDRDL_FCSO                BDRDL_SBADGN+1
#define BDRDL_FCSG                BDRDL_FCSO+1
#define BDRDL_FEPGN               BDRDL_FCSG+1
#define BDRDL_FEBG                BDRDL_FEPGN+1
#define BDRDL_FE2OS               BDRDL_FEBG+1
#define BDRDL_FE2GN               BDRDL_FE2OS+1
#define BDRDL_SRFOS               BDRDL_FE2GN+1
#define BDRDL_SRFOBG              BDRDL_SRFOS+1
#define BDRDL_MPPOS               BDRDL_SRFOBG+1
#define BDRDL_MPPOBG              BDRDL_MPPOS+1
#define BDRDL_MPPOPGN             BDRDL_MPPOBG+1
#define BDRDL_CEOS                BDRDL_MPPOPGN+1
#define BDRDL_CEPGN               BDRDL_CEOS+1
#define BDRDL_CEGN                BDRDL_CEPGN+1
#define BDRDL_CEBG                BDRDL_CEGN+1
#define BDRDL_TRKO                BDRDL_CEBG+1
#define BDRDL_TRKG                BDRDL_TRKO+1
#define BDRDL_TEPGN               BDRDL_TRKG+1
#define BDRDL_TEBG                BDRDL_TEPGN+1
#define BDRDL_TE2OS               BDRDL_TEBG+1
#define BDRDL_TE2GN               BDRDL_TE2OS+1
#define BDRDL_BSKO                BDRDL_TE2GN+1
#define BDRDL_BSKG                BDRDL_BSKO+1
#define _BDRDL_MPPO_0dBH          BDRDL_BSKG+1              //20090507_h.30
#define _BDRDL_MPPO_0dBL          _BDRDL_MPPO_0dBH+1        //20090507_h.30
#define BDRDL_BS2OS               _BDRDL_MPPO_0dBL+1        //20090507_h.30
#define BDRDL_MPPOSRFORatioX100   BDRDL_BS2OS+1             //20090507_h.30
#define BDRDL_PHSOS               BDRDL_MPPOSRFORatioX100+1 //20090507_h.30
#define BDRDL_BHSOS               BDRDL_PHSOS+1
#define _BDRDL_FMD_PPLVL_CDH       BDRDL_BHSOS+1
#define _BDRDL_FMD_PPLVL_CDL      _BDRDL_FMD_PPLVL_CDH +1
#define _BDRDL_FMD_PPLVL_DVDH     _BDRDL_FMD_PPLVL_CDL +1
#define _BDRDL_FMD_PPLVL_DVDL     _BDRDL_FMD_PPLVL_DVDH+1
#define _BDRDL_FMD_PPLVL_BDH      _BDRDL_FMD_PPLVL_DVDL+1
#define _BDRDL_FMD_PPLVL_BDL      _BDRDL_FMD_PPLVL_BDH +1
#define _BDRDL_FMD_DCLVL_CDH      _BDRDL_FMD_PPLVL_BDL +1
#define _BDRDL_FMD_DCLVL_CDL      _BDRDL_FMD_DCLVL_CDH +1
#define _BDRDL_FMD_DCLVL_DVDH     _BDRDL_FMD_DCLVL_CDL +1
#define _BDRDL_FMD_DCLVL_DVDL     _BDRDL_FMD_DCLVL_DVDH+1
#define _BDRDL_FMD_DCLVL_BDH      _BDRDL_FMD_DCLVL_DVDL+1
#define _BDRDL_FMD_DCLVL_BDL      _BDRDL_FMD_DCLVL_BDH +1
#define _BDRDL_FEloopgainH        _BDRDL_FMD_DCLVL_BDL +1
#define _BDRDL_FEloopgainL         _BDRDL_FEloopgainH+1
#define _BDRDL_TEloopgainH         _BDRDL_FEloopgainL+1
#define _BDRDL_TEloopgainL         _BDRDL_TEloopgainH+1
#define _BDRDL_FEBCH               _BDRDL_TEloopgainL+1
#define _BDRDL_FEBCL               _BDRDL_FEBCH+1
#define _BDRDL_TILTH               _BDRDL_FEBCL+1
#define _BDRDL_TILTL               _BDRDL_TILTH+1
#define _BDRDL_DSPCEOFFSETH        _BDRDL_TILTL+1
#define _BDRDL_DSPCEOFFSETL        _BDRDL_DSPCEOFFSETH+1
#define _BDRDL_DSPFEOFFSETH        _BDRDL_DSPCEOFFSETL+1
#define _BDRDL_DSPFEOFFSETL        _BDRDL_DSPFEOFFSETH+1
#define _BDRDL_DSPTEOFFSETH        _BDRDL_DSPFEOFFSETL+1
#define _BDRDL_DSPTEOFFSETL        _BDRDL_DSPTEOFFSETH+1
#define _BDRDL_DSPRPOFFSETH        _BDRDL_DSPTEOFFSETL+1
#define _BDRDL_DSPRPOFFSETL        _BDRDL_DSPRPOFFSETH+1
#define BDRDL_TROOS               _BDRDL_DSPRPOFFSETL+1
#define BDRDL_M                    BDRDL_TROOS+1
#define BDRDL_DPDCH1OS             BDRDL_M+1
#define BDRDL_DPDCH2OS            BDRDL_DPDCH1OS+1
#define BDRDL_DPDCH3OS            BDRDL_DPDCH2OS+1
#define BDRDL_DPDCH4OS            BDRDL_DPDCH3OS+1
#define BDRDL_DPDGN               BDRDL_DPDCH4OS+1
#define BDRDL_DPDGMOS             BDRDL_DPDGN+1
#define BDRDL_RFGMOS              BDRDL_DPDGMOS+1
#define BDRDL_DPDHSSEL            BDRDL_RFGMOS+1
#define BDRDL_DPDHSFCH            BDRDL_DPDHSSEL+1
#define BDRDL_DPDHSFCL            BDRDL_DPDHSFCH+1
#define BDRDL_DPDHSISCL           BDRDL_DPDHSFCL+1
#define BDRDL_DPDINFC             BDRDL_DPDHSISCL+1
#define BDRDL_DPDBST              BDRDL_DPDINFC+1
#define _BDRDL_STMH                BDRDL_DPDBST+1
#define _BDRDL_STML                _BDRDL_STMH+1
#define _BDRDL_Layertime0H         _BDRDL_STML+1
#define _BDRDL_Layertime0L         _BDRDL_Layertime0H+1
#define _BDRDL_Layertime1H         _BDRDL_Layertime0L+1
#define _BDRDL_Layertime1L         _BDRDL_Layertime1H+1
#define _BDRDL_maxSRFOpp_0dBH      _BDRDL_Layertime1L+1
#define _BDRDL_maxSRFOpp_0dBL      _BDRDL_maxSRFOpp_0dBH+1
#define BDRDL_TEBSRatioX100       _BDRDL_maxSRFOpp_0dBL+1
#define _BDRDL_svoRunOutH          BDRDL_TEBSRatioX100+1
#define _BDRDL_svoRunOutL          _BDRDL_svoRunOutH+1
#define _BDRDL_SAL0H               _BDRDL_svoRunOutL+1
#define _BDRDL_SAL0L               _BDRDL_SAL0H+1
#define _BDRDL_FEBCcurH            _BDRDL_SAL0L+1
#define _BDRDL_FEBCcurL            _BDRDL_FEBCcurH+1
#define _BDRDL_TILTcurH            _BDRDL_FEBCcurL+1
#define _BDRDL_TILTcurL            _BDRDL_TILTcurH+1

#define BDRDL_RFAGCL1             _BDRDL_TILTcurL+1
#define BDRDL_RFAOCL1             BDRDL_RFAGCL1+1
#define BDRDL_SPP1OSL1            BDRDL_RFAOCL1+1
#define BDRDL_SPP1PGNL1           BDRDL_SPP1OSL1+1
#define BDRDL_SPP1BGL1            BDRDL_SPP1PGNL1+1
#define BDRDL_MPPOSL1             BDRDL_SPP1BGL1+1
#define BDRDL_MPPOBGL1            BDRDL_MPPOSL1+1
#define BDRDL_MPPOPGNL1           BDRDL_MPPOBGL1+1
#define BDRDL_CEOSL1              BDRDL_MPPOPGNL1+1
#define BDRDL_CEPGNL1             BDRDL_CEOSL1+1
#define BDRDL_CEGNL1              BDRDL_CEPGNL1+1
#define BDRDL_CEBGL1              BDRDL_CEGNL1+1
#define BDRDL_TRKOL1              BDRDL_CEBGL1+1
#define BDRDL_TRKGL1              BDRDL_TRKOL1+1
#define BDRDL_TEPGNL1             BDRDL_TRKGL1+1
#define BDRDL_TEBGL1              BDRDL_TEPGNL1+1
#define BDRDL_TE2OSL1             BDRDL_TEBGL1+1
#define BDRDL_TE2GNL1             BDRDL_TE2OSL1+1
#define BDRDL_RPOSL1              BDRDL_TE2GNL1+1
#define BDRDL_RPPGNL1             BDRDL_RPOSL1+1
#define BDRDL_RPBGL1              BDRDL_RPPGNL1+1
#define BDRDL_RPGNL1              BDRDL_RPBGL1+1
#define BDRDL_RP2OSL1             BDRDL_RPGNL1+1
#define BDRDL_RP2GNL1             BDRDL_RP2OSL1+1
#define _BDRDL_FEloopgainL1H       BDRDL_RP2GNL1+1
#define _BDRDL_FEloopgainL1L       _BDRDL_FEloopgainL1H+1
#define _BDRDL_TEloopgainL1H       _BDRDL_FEloopgainL1L+1
#define _BDRDL_TEloopgainL1L       _BDRDL_TEloopgainL1H+1
#define _BDRDL_FEBCL1H             _BDRDL_TEloopgainL1L+1
#define _BDRDL_FEBCL1L             _BDRDL_FEBCL1H+1
#define _BDRDL_TILTL1H             _BDRDL_FEBCL1L+1
#define _BDRDL_TILTL1L             _BDRDL_TILTL1H+1
#define _BDRDL_SAL0L1H             _BDRDL_TILTL1L+1
#define _BDRDL_SAL0L1L             _BDRDL_SAL0L1H+1
#define _BDRDL_FEBCcurL1H          _BDRDL_SAL0L1L+1
#define _BDRDL_FEBCcurL1L          _BDRDL_FEBCcurL1H+1
#define _BDRDL_TILTcurL1H          _BDRDL_FEBCcurL1L+1
#define _BDRDL_TILTcurL1L          _BDRDL_TILTcurL1H+1
#define _BDRDL_ADCVRPOWH           _BDRDL_TILTcurL1L+1
#define _BDRDL_ADCVRPOWL          _BDRDL_ADCVRPOWH+1
#define _BDRDL_VDELTAXH           _BDRDL_ADCVRPOWL+1
#define _BDRDL_VDELTAXL           _BDRDL_VDELTAXH+1
#define _BDRDL_SETPOINTH          _BDRDL_VDELTAXL+1
#define _BDRDL_SETPOINTL          _BDRDL_SETPOINTH+1
#define _BDRDL_FEPPL0H            _BDRDL_SETPOINTL+1
#define _BDRDL_FEPPL0L            _BDRDL_FEPPL0H+1
#define _BDRDL_FEPPL1H            _BDRDL_FEPPL0L+1
#define _BDRDL_FEPPL1L            _BDRDL_FEPPL1H+1
#define _BDRDL_LTHRPJGLVLH        _BDRDL_FEPPL1L+1
#define _BDRDL_LTHRPJGLVLL        _BDRDL_LTHRPJGLVLH+1
#define _BDRDL_RFppH              _BDRDL_LTHRPJGLVLL+1
#define _BDRDL_RFppL              _BDRDL_RFppH+1
#define _BDRDL_HOMEINDEXH         _BDRDL_RFppL+1
#define _BDRDL_HOMEINDEXL         _BDRDL_HOMEINDEXH+1
#define _BDRDL_UNBMSH             _BDRDL_HOMEINDEXL+1
#define _BDRDL_UNBMSL             _BDRDL_UNBMSH+1
#define _BDRDL_UNBHSH             _BDRDL_UNBMSL+1
#define _BDRDL_UNBHSL             _BDRDL_UNBHSH+1
#define _BDRDL_FGPULSECOUNTH           _BDRDL_UNBHSL+1
#define _BDRDL_FGPULSECOUNTL           _BDRDL_FGPULSECOUNTH+1
#define BDRDL_wbllockcnt          _BDRDL_FGPULSECOUNTL+1
#define BDRDL_jitter              BDRDL_wbllockcnt+1
#define _BDRDL_APPDPDPPRatioX100H BDRDL_jitter+1
#define _BDRDL_APPDPDPPRatioX100L _BDRDL_APPDPDPPRatioX100H+1

/**********BDRE**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDRE_header              0x0C20
#define BDRE_data_col            BDRE_header+7
#define BDRE_lastcol_bytenum     BDRE_data_col+1
#define BDRE_sensekey            BDRE_lastcol_bytenum+5
#define BDRE_ASC                 BDRE_sensekey+1
#define BDRE_ASCQ                BDRE_ASC+1
#define _BDRE_VREFH               BDRE_ASCQ+0x11
#define _BDRE_VREFL               _BDRE_VREFH+1
#define _BDRE_V21H                _BDRE_VREFL+1
#define _BDRE_V21L                _BDRE_V21H+1
#define BDRE_RFINGN              _BDRE_V21L+1
#define BDRE_RFCOSPO             BDRE_RFINGN+1
#define BDRE_RFCOSNO             BDRE_RFCOSPO+1
#define BDRE_RFGNO               BDRE_RFCOSNO+1
#define BDRE_RFAGC               BDRE_RFGNO+1
#define BDRE_RFAOC               BDRE_RFAGC+1
#define _BDRE_maxFEpp_0dBH       BDRE_RFAOC+1               //20090507_h.30
#define _BDRE_maxFEpp_0dBL       _BDRE_maxFEpp_0dBH+1       //20090507_h.30
#define BDRE_RFPOS               _BDRE_maxFEpp_0dBL+1       //20090507_h.30
#define BDRE_MBVREFOS            BDRE_RFPOS+1
#define BDRE_SBVREFOS            BDRE_MBVREFOS+1
#define BDRE_MBGUPSEL            BDRE_SBVREFOS+1
#define BDRE_SBGUPSEL            BDRE_MBGUPSEL+1
#define BDRE_FEOOS               BDRE_SBGUPSEL+1
#define BDRE_FEOBG               BDRE_FEOOS+1
#define BDRE_FEOPGN              BDRE_FEOBG+1
#define BDRE_SPP1OS              BDRE_FEOPGN+1
#define BDRE_SPP1PGN             BDRE_SPP1OS+1
#define BDRE_SPP1BG              BDRE_SPP1PGN+1
#define BDRE_SPP2OS              BDRE_SPP1BG+1
#define BDRE_SPP2PGN             BDRE_SPP2OS+1
#define BDRE_SPP2BG              BDRE_SPP2PGN+1
#define BDRE_SBADOS              BDRE_SPP2BG+1
#define BDRE_SBADGN              BDRE_SBADOS+1
#define BDRE_FCSO                BDRE_SBADGN+1
#define BDRE_FCSG                BDRE_FCSO+1
#define BDRE_FEPGN               BDRE_FCSG+1
#define BDRE_FEBG                BDRE_FEPGN+1
#define BDRE_FE2OS               BDRE_FEBG+1
#define BDRE_FE2GN               BDRE_FE2OS+1
#define BDRE_SRFOS               BDRE_FE2GN+1
#define BDRE_SRFOBG              BDRE_SRFOS+1
#define BDRE_MPPOS               BDRE_SRFOBG+1
#define BDRE_MPPOBG              BDRE_MPPOS+1
#define BDRE_MPPOPGN             BDRE_MPPOBG+1
#define BDRE_CEOS                BDRE_MPPOPGN+1
#define BDRE_CEPGN               BDRE_CEOS+1
#define BDRE_CEGN                BDRE_CEPGN+1
#define BDRE_CEBG                BDRE_CEGN+1
#define BDRE_TRKO                BDRE_CEBG+1
#define BDRE_TRKG                BDRE_TRKO+1
#define BDRE_TEPGN               BDRE_TRKG+1
#define BDRE_TEBG                BDRE_TEPGN+1
#define BDRE_TE2OS               BDRE_TEBG+1
#define BDRE_TE2GN               BDRE_TE2OS+1
#define BDRE_BSKO                BDRE_TE2GN+1
#define BDRE_BSKG                BDRE_BSKO+1
#define _BDRE_MPPO_0dBH          BDRE_BSKG+1                //20090507_h.30
#define _BDRE_MPPO_0dBL          _BDRE_MPPO_0dBH+1          //20090507_h.30
#define BDRE_BS2OS               _BDRE_MPPO_0dBL+1          //20090507_h.30
#define BDRE_MPPOSRFORatioX100   BDRE_BS2OS+1               //20090507_h.30
#define BDRE_PHSOS               BDRE_MPPOSRFORatioX100+1   //20090507_h.30
#define BDRE_BHSOS               BDRE_PHSOS+1
#define _BDRE_FMD_PPLVL_CDH       BDRE_BHSOS+1
#define _BDRE_FMD_PPLVL_CDL      _BDRE_FMD_PPLVL_CDH +1
#define _BDRE_FMD_PPLVL_DVDH     _BDRE_FMD_PPLVL_CDL +1
#define _BDRE_FMD_PPLVL_DVDL     _BDRE_FMD_PPLVL_DVDH+1
#define _BDRE_FMD_PPLVL_BDH      _BDRE_FMD_PPLVL_DVDL+1
#define _BDRE_FMD_PPLVL_BDL      _BDRE_FMD_PPLVL_BDH +1
#define _BDRE_FMD_DCLVL_CDH      _BDRE_FMD_PPLVL_BDL +1
#define _BDRE_FMD_DCLVL_CDL      _BDRE_FMD_DCLVL_CDH +1
#define _BDRE_FMD_DCLVL_DVDH     _BDRE_FMD_DCLVL_CDL +1
#define _BDRE_FMD_DCLVL_DVDL     _BDRE_FMD_DCLVL_DVDH+1
#define _BDRE_FMD_DCLVL_BDH      _BDRE_FMD_DCLVL_DVDL+1
#define _BDRE_FMD_DCLVL_BDL      _BDRE_FMD_DCLVL_BDH +1
#define _BDRE_FEloopgainH        _BDRE_FMD_DCLVL_BDL +1
#define _BDRE_FEloopgainL         _BDRE_FEloopgainH+1
#define _BDRE_TEloopgainH         _BDRE_FEloopgainL+1
#define _BDRE_TEloopgainL         _BDRE_TEloopgainH+1
#define _BDRE_FEBCH               _BDRE_TEloopgainL+1
#define _BDRE_FEBCL               _BDRE_FEBCH+1
#define _BDRE_TILTH               _BDRE_FEBCL+1
#define _BDRE_TILTL               _BDRE_TILTH+1
#define _BDRE_DSPCEOFFSETH        _BDRE_TILTL+1
#define _BDRE_DSPCEOFFSETL        _BDRE_DSPCEOFFSETH+1
#define _BDRE_DSPFEOFFSETH        _BDRE_DSPCEOFFSETL+1
#define _BDRE_DSPFEOFFSETL        _BDRE_DSPFEOFFSETH+1
#define _BDRE_DSPTEOFFSETH        _BDRE_DSPFEOFFSETL+1
#define _BDRE_DSPTEOFFSETL        _BDRE_DSPTEOFFSETH+1
#define _BDRE_DSPRPOFFSETH        _BDRE_DSPTEOFFSETL+1
#define _BDRE_DSPRPOFFSETL        _BDRE_DSPRPOFFSETH+1
#define BDRE_TROOS               _BDRE_DSPRPOFFSETL+1
#define BDRE_M                    BDRE_TROOS+1
#define BDRE_DPDCH1OS             BDRE_M+1
#define BDRE_DPDCH2OS            BDRE_DPDCH1OS+1
#define BDRE_DPDCH3OS            BDRE_DPDCH2OS+1
#define BDRE_DPDCH4OS            BDRE_DPDCH3OS+1
#define BDRE_DPDGN               BDRE_DPDCH4OS+1
#define BDRE_DPDGMOS             BDRE_DPDGN+1
#define BDRE_RFGMOS              BDRE_DPDGMOS+1
#define BDRE_DPDHSSEL            BDRE_RFGMOS+1
#define BDRE_DPDHSFCH            BDRE_DPDHSSEL+1
#define BDRE_DPDHSFCL            BDRE_DPDHSFCH+1
#define BDRE_DPDHSISCL           BDRE_DPDHSFCL+1
#define BDRE_DPDINFC             BDRE_DPDHSISCL+1
#define BDRE_DPDBST              BDRE_DPDINFC+1
#define _BDRE_STMH                BDRE_DPDBST+1
#define _BDRE_STML                _BDRE_STMH+1
#define _BDRE_Layertime0H         _BDRE_STML+1
#define _BDRE_Layertime0L         _BDRE_Layertime0H+1
#define _BDRE_Layertime1H         _BDRE_Layertime0L+1
#define _BDRE_Layertime1L         _BDRE_Layertime1H+1
#define _BDRE_maxSRFOpp_0dBH      _BDRE_Layertime1L+1
#define _BDRE_maxSRFOpp_0dBL      _BDRE_maxSRFOpp_0dBH+1
#define BDRE_TEBSRatioX100       _BDRE_maxSRFOpp_0dBL+1
#define _BDRE_svoRunOutH          BDRE_TEBSRatioX100+1
#define _BDRE_svoRunOutL          _BDRE_svoRunOutH+1
#define _BDRE_SAL0H               _BDRE_svoRunOutL+1
#define _BDRE_SAL0L               _BDRE_SAL0H+1
#define _BDRE_FEBCcurH            _BDRE_SAL0L+1
#define _BDRE_FEBCcurL            _BDRE_FEBCcurH+1
#define _BDRE_TILTcurH            _BDRE_FEBCcurL+1
#define _BDRE_TILTcurL            _BDRE_TILTcurH+1
#define _BDRE_ADCVRPOWH          _BDRE_TILTcurL+1
#define _BDRE_ADCVRPOWL          _BDRE_ADCVRPOWH+1
#define _BDRE_VDELTAXH           _BDRE_ADCVRPOWL+1
#define _BDRE_VDELTAXL           _BDRE_VDELTAXH+1
#define _BDRE_SETPOINTH          _BDRE_VDELTAXL+1
#define _BDRE_SETPOINTL          _BDRE_SETPOINTH+1
#define _BDRE_FEPPL0H            _BDRE_SETPOINTL+1
#define _BDRE_FEPPL0L            _BDRE_FEPPL0H+1
#define _BDRE_FEPPL1H            _BDRE_FEPPL0L+1
#define _BDRE_FEPPL1L            _BDRE_FEPPL1H+1
#define _BDRE_LTHRPJGLVLH        _BDRE_FEPPL1L+1
#define _BDRE_LTHRPJGLVLL        _BDRE_LTHRPJGLVLH+1
#define _BDRE_RFppH              _BDRE_LTHRPJGLVLL+1
#define _BDRE_RFppL              _BDRE_RFppH+1
#define _BDRE_HOMEINDEXH         _BDRE_RFppL+1
#define _BDRE_HOMEINDEXL         _BDRE_HOMEINDEXH+1
#define _BDRE_UNBMSH             _BDRE_HOMEINDEXL+1
#define _BDRE_UNBMSL             _BDRE_UNBMSH+1
#define _BDRE_UNBHSH             _BDRE_UNBMSL+1
#define _BDRE_UNBHSL             _BDRE_UNBHSH+1
#define _BDRE_FGPULSECOUNTH           _BDRE_UNBHSL+1
#define _BDRE_FGPULSECOUNTL           _BDRE_FGPULSECOUNTH+1
#define BDRE_wbllockcnt          _BDRE_FGPULSECOUNTL+1
#define BDRE_jitter              BDRE_wbllockcnt+1
#define _BDRE_APPDPDPPRatioX100H BDRE_jitter+1
#define _BDRE_APPDPDPPRatioX100L _BDRE_APPDPDPPRatioX100H+1

/**********BDREDL**********/

//#define KEY_header               0x0000
//#define KEY_header_byte7         KEY_header+7
//#define OPC_judge_status_tbl     KEY_header+13
//#define INTF_judge_status_tbl    OPC_judge_status_tbl+1
//#define SERVO_judge_status_tbl   INTF_judge_status_tbl+1
#define BDREDL_header              0x0CD0
#define BDREDL_data_col            BDREDL_header+7
#define BDREDL_lastcol_bytenum     BDREDL_data_col+1
#define BDREDL_sensekey            BDREDL_lastcol_bytenum+5
#define BDREDL_ASC                 BDREDL_sensekey+1
#define BDREDL_ASCQ                BDREDL_ASC+1
#define _BDREDL_VREFH               BDREDL_ASCQ+0x11
#define _BDREDL_VREFL               _BDREDL_VREFH+1
#define _BDREDL_V21H                _BDREDL_VREFL+1
#define _BDREDL_V21L                _BDREDL_V21H+1
#define BDREDL_RFINGN              _BDREDL_V21L+1
#define BDREDL_RFCOSPO             BDREDL_RFINGN+1
#define BDREDL_RFCOSNO             BDREDL_RFCOSPO+1
#define BDREDL_RFGNO               BDREDL_RFCOSNO+1
#define BDREDL_RFAGC               BDREDL_RFGNO+1
#define BDREDL_RFAOC               BDREDL_RFAGC+1
#define _BDREDL_maxFEpp_0dBH       BDREDL_RFAOC+1           //20090507_h.30
#define _BDREDL_maxFEpp_0dBL       _BDREDL_maxFEpp_0dBH+1   //20090507_h.30
#define BDREDL_RFPOS               _BDREDL_maxFEpp_0dBL+1   //20090507_h.30
#define BDREDL_MBVREFOS            BDREDL_RFPOS+1
#define BDREDL_SBVREFOS            BDREDL_MBVREFOS+1
#define BDREDL_MBGUPSEL            BDREDL_SBVREFOS+1
#define BDREDL_SBGUPSEL            BDREDL_MBGUPSEL+1
#define BDREDL_FEOOS               BDREDL_SBGUPSEL+1
#define BDREDL_FEOBG               BDREDL_FEOOS+1
#define BDREDL_FEOPGN              BDREDL_FEOBG+1
#define BDREDL_SPP1OS              BDREDL_FEOPGN+1
#define BDREDL_SPP1PGN             BDREDL_SPP1OS+1
#define BDREDL_SPP1BG              BDREDL_SPP1PGN+1
#define BDREDL_SPP2OS              BDREDL_SPP1BG+1
#define BDREDL_SPP2PGN             BDREDL_SPP2OS+1
#define BDREDL_SPP2BG              BDREDL_SPP2PGN+1
#define BDREDL_SBADOS              BDREDL_SPP2BG+1
#define BDREDL_SBADGN              BDREDL_SBADOS+1
#define BDREDL_FCSO                BDREDL_SBADGN+1
#define BDREDL_FCSG                BDREDL_FCSO+1
#define BDREDL_FEPGN               BDREDL_FCSG+1
#define BDREDL_FEBG                BDREDL_FEPGN+1
#define BDREDL_FE2OS               BDREDL_FEBG+1
#define BDREDL_FE2GN               BDREDL_FE2OS+1
#define BDREDL_SRFOS               BDREDL_FE2GN+1
#define BDREDL_SRFOBG              BDREDL_SRFOS+1
#define BDREDL_MPPOS               BDREDL_SRFOBG+1
#define BDREDL_MPPOBG              BDREDL_MPPOS+1
#define BDREDL_MPPOPGN             BDREDL_MPPOBG+1
#define BDREDL_CEOS                BDREDL_MPPOPGN+1
#define BDREDL_CEPGN               BDREDL_CEOS+1
#define BDREDL_CEGN                BDREDL_CEPGN+1
#define BDREDL_CEBG                BDREDL_CEGN+1
#define BDREDL_TRKO                BDREDL_CEBG+1
#define BDREDL_TRKG                BDREDL_TRKO+1
#define BDREDL_TEPGN               BDREDL_TRKG+1
#define BDREDL_TEBG                BDREDL_TEPGN+1
#define BDREDL_TE2OS               BDREDL_TEBG+1
#define BDREDL_TE2GN               BDREDL_TE2OS+1
#define BDREDL_BSKO                BDREDL_TE2GN+1
#define BDREDL_BSKG                BDREDL_BSKO+1
#define _BDREDL_MPPO_0dBH          BDREDL_BSKG+1            //20090507_h.30
#define _BDREDL_MPPO_0dBL          _BDREDL_MPPO_0dBH+1      //20090507_h.30
#define BDREDL_BS2OS               _BDREDL_MPPO_0dBL+1      //20090507_h.30
#define BDREDL_MPPOSRFORatioX100   BDREDL_BS2OS+1           //20090507_h.30
#define BDREDL_PHSOS               BDREDL_MPPOSRFORatioX100+1//20090507_h.30
#define BDREDL_BHSOS               BDREDL_PHSOS+1
#define _BDREDL_FMD_PPLVL_CDH       BDREDL_BHSOS+1
#define _BDREDL_FMD_PPLVL_CDL      _BDREDL_FMD_PPLVL_CDH +1
#define _BDREDL_FMD_PPLVL_DVDH     _BDREDL_FMD_PPLVL_CDL +1
#define _BDREDL_FMD_PPLVL_DVDL     _BDREDL_FMD_PPLVL_DVDH+1
#define _BDREDL_FMD_PPLVL_BDH      _BDREDL_FMD_PPLVL_DVDL+1
#define _BDREDL_FMD_PPLVL_BDL      _BDREDL_FMD_PPLVL_BDH +1
#define _BDREDL_FMD_DCLVL_CDH      _BDREDL_FMD_PPLVL_BDL +1
#define _BDREDL_FMD_DCLVL_CDL      _BDREDL_FMD_DCLVL_CDH +1
#define _BDREDL_FMD_DCLVL_DVDH     _BDREDL_FMD_DCLVL_CDL +1
#define _BDREDL_FMD_DCLVL_DVDL     _BDREDL_FMD_DCLVL_DVDH+1
#define _BDREDL_FMD_DCLVL_BDH      _BDREDL_FMD_DCLVL_DVDL+1
#define _BDREDL_FMD_DCLVL_BDL      _BDREDL_FMD_DCLVL_BDH +1
#define _BDREDL_FEloopgainH        _BDREDL_FMD_DCLVL_BDL +1
#define _BDREDL_FEloopgainL         _BDREDL_FEloopgainH+1
#define _BDREDL_TEloopgainH         _BDREDL_FEloopgainL+1
#define _BDREDL_TEloopgainL         _BDREDL_TEloopgainH+1
#define _BDREDL_FEBCH               _BDREDL_TEloopgainL+1
#define _BDREDL_FEBCL               _BDREDL_FEBCH+1
#define _BDREDL_TILTH               _BDREDL_FEBCL+1
#define _BDREDL_TILTL               _BDREDL_TILTH+1
#define _BDREDL_DSPCEOFFSETH        _BDREDL_TILTL+1
#define _BDREDL_DSPCEOFFSETL        _BDREDL_DSPCEOFFSETH+1
#define _BDREDL_DSPFEOFFSETH        _BDREDL_DSPCEOFFSETL+1
#define _BDREDL_DSPFEOFFSETL        _BDREDL_DSPFEOFFSETH+1
#define _BDREDL_DSPTEOFFSETH        _BDREDL_DSPFEOFFSETL+1
#define _BDREDL_DSPTEOFFSETL        _BDREDL_DSPTEOFFSETH+1
#define _BDREDL_DSPRPOFFSETH        _BDREDL_DSPTEOFFSETL+1
#define _BDREDL_DSPRPOFFSETL        _BDREDL_DSPRPOFFSETH+1
#define BDREDL_TROOS               _BDREDL_DSPRPOFFSETL+1
#define BDREDL_M                    BDREDL_TROOS+1
#define BDREDL_DPDCH1OS             BDREDL_M+1
#define BDREDL_DPDCH2OS            BDREDL_DPDCH1OS+1
#define BDREDL_DPDCH3OS            BDREDL_DPDCH2OS+1
#define BDREDL_DPDCH4OS            BDREDL_DPDCH3OS+1
#define BDREDL_DPDGN               BDREDL_DPDCH4OS+1
#define BDREDL_DPDGMOS             BDREDL_DPDGN+1
#define BDREDL_RFGMOS              BDREDL_DPDGMOS+1
#define BDREDL_DPDHSSEL            BDREDL_RFGMOS+1
#define BDREDL_DPDHSFCH            BDREDL_DPDHSSEL+1
#define BDREDL_DPDHSFCL            BDREDL_DPDHSFCH+1
#define BDREDL_DPDHSISCL           BDREDL_DPDHSFCL+1
#define BDREDL_DPDINFC             BDREDL_DPDHSISCL+1
#define BDREDL_DPDBST              BDREDL_DPDINFC+1
#define _BDREDL_STMH                BDREDL_DPDBST+1
#define _BDREDL_STML                _BDREDL_STMH+1
#define _BDREDL_Layertime0H         _BDREDL_STML+1
#define _BDREDL_Layertime0L         _BDREDL_Layertime0H+1
#define _BDREDL_Layertime1H         _BDREDL_Layertime0L+1
#define _BDREDL_Layertime1L         _BDREDL_Layertime1H+1
#define _BDREDL_maxSRFOpp_0dBH      _BDREDL_Layertime1L+1
#define _BDREDL_maxSRFOpp_0dBL      _BDREDL_maxSRFOpp_0dBH+1
#define BDREDL_TEBSRatioX100       _BDREDL_maxSRFOpp_0dBL+1
#define _BDREDL_svoRunOutH          BDREDL_TEBSRatioX100+1
#define _BDREDL_svoRunOutL          _BDREDL_svoRunOutH+1
#define _BDREDL_SAL0H               _BDREDL_svoRunOutL+1
#define _BDREDL_SAL0L               _BDREDL_SAL0H+1
#define _BDREDL_FEBCcurH            _BDREDL_SAL0L+1
#define _BDREDL_FEBCcurL            _BDREDL_FEBCcurH+1
#define _BDREDL_TILTcurH            _BDREDL_FEBCcurL+1
#define _BDREDL_TILTcurL            _BDREDL_TILTcurH+1

#define BDREDL_RFAGCL1             _BDREDL_TILTcurL+1
#define BDREDL_RFAOCL1             BDREDL_RFAGCL1+1
#define BDREDL_SPP1OSL1            BDREDL_RFAOCL1+1
#define BDREDL_SPP1PGNL1           BDREDL_SPP1OSL1+1
#define BDREDL_SPP1BGL1            BDREDL_SPP1PGNL1+1
#define BDREDL_MPPOSL1             BDREDL_SPP1BGL1+1
#define BDREDL_MPPOBGL1            BDREDL_MPPOSL1+1
#define BDREDL_MPPOPGNL1           BDREDL_MPPOBGL1+1
#define BDREDL_CEOSL1              BDREDL_MPPOPGNL1+1
#define BDREDL_CEPGNL1             BDREDL_CEOSL1+1
#define BDREDL_CEGNL1              BDREDL_CEPGNL1+1
#define BDREDL_CEBGL1              BDREDL_CEGNL1+1
#define BDREDL_TRKOL1              BDREDL_CEBGL1+1
#define BDREDL_TRKGL1              BDREDL_TRKOL1+1
#define BDREDL_TEPGNL1             BDREDL_TRKGL1+1
#define BDREDL_TEBGL1              BDREDL_TEPGNL1+1
#define BDREDL_TE2OSL1             BDREDL_TEBGL1+1
#define BDREDL_TE2GNL1             BDREDL_TE2OSL1+1
#define BDREDL_RPOSL1              BDREDL_TE2GNL1+1
#define BDREDL_RPPGNL1             BDREDL_RPOSL1+1
#define BDREDL_RPBGL1              BDREDL_RPPGNL1+1
#define BDREDL_RPGNL1              BDREDL_RPBGL1+1
#define BDREDL_RP2OSL1             BDREDL_RPGNL1+1
#define BDREDL_RP2GNL1             BDREDL_RP2OSL1+1
#define _BDREDL_FEloopgainL1H       BDREDL_RP2GNL1+1
#define _BDREDL_FEloopgainL1L       _BDREDL_FEloopgainL1H+1
#define _BDREDL_TEloopgainL1H       _BDREDL_FEloopgainL1L+1
#define _BDREDL_TEloopgainL1L       _BDREDL_TEloopgainL1H+1
#define _BDREDL_FEBCL1H             _BDREDL_TEloopgainL1L+1
#define _BDREDL_FEBCL1L             _BDREDL_FEBCL1H+1
#define _BDREDL_TILTL1H             _BDREDL_FEBCL1L+1
#define _BDREDL_TILTL1L             _BDREDL_TILTL1H+1
#define _BDREDL_SAL0L1H             _BDREDL_TILTL1L+1
#define _BDREDL_SAL0L1L             _BDREDL_SAL0L1H+1
#define _BDREDL_FEBCcurL1H          _BDREDL_SAL0L1L+1
#define _BDREDL_FEBCcurL1L          _BDREDL_FEBCcurL1H+1
#define _BDREDL_TILTcurL1H          _BDREDL_FEBCcurL1L+1
#define _BDREDL_TILTcurL1L          _BDREDL_TILTcurL1H+1
#define _BDREDL_ADCVRPOWH           _BDREDL_TILTcurL1L+1
#define _BDREDL_ADCVRPOWL          _BDREDL_ADCVRPOWH+1
#define _BDREDL_VDELTAXH           _BDREDL_ADCVRPOWL+1
#define _BDREDL_VDELTAXL           _BDREDL_VDELTAXH+1
#define _BDREDL_SETPOINTH          _BDREDL_VDELTAXL+1
#define _BDREDL_SETPOINTL          _BDREDL_SETPOINTH+1
#define _BDREDL_FEPPL0H            _BDREDL_SETPOINTL+1
#define _BDREDL_FEPPL0L            _BDREDL_FEPPL0H+1
#define _BDREDL_FEPPL1H            _BDREDL_FEPPL0L+1
#define _BDREDL_FEPPL1L            _BDREDL_FEPPL1H+1
#define _BDREDL_LTHRPJGLVLH        _BDREDL_FEPPL1L+1
#define _BDREDL_LTHRPJGLVLL        _BDREDL_LTHRPJGLVLH+1
#define _BDREDL_RFppH              _BDREDL_LTHRPJGLVLL+1
#define _BDREDL_RFppL              _BDREDL_RFppH+1
#define _BDREDL_HOMEINDEXH         _BDREDL_RFppL+1
#define _BDREDL_HOMEINDEXL         _BDREDL_HOMEINDEXH+1
#define _BDREDL_UNBMSH             _BDREDL_HOMEINDEXL+1
#define _BDREDL_UNBMSL             _BDREDL_UNBMSH+1
#define _BDREDL_UNBHSH             _BDREDL_UNBMSL+1
#define _BDREDL_UNBHSL             _BDREDL_UNBHSH+1
#define _BDREDL_FGPULSECOUNTH           _BDREDL_UNBHSL+1
#define _BDREDL_FGPULSECOUNTL           _BDREDL_FGPULSECOUNTH+1
#define BDREDL_wbllockcnt          _BDREDL_FGPULSECOUNTL+1
#define BDREDL_jitter              BDREDL_wbllockcnt+1
#define _BDREDL_APPDPDPPRatioX100H BDREDL_jitter+1
#define _BDREDL_APPDPDPPRatioX100L _BDREDL_APPDPDPPRatioX100H+1

#endif


#endif  /* #ifndef __OEM_TBL_H__*/
