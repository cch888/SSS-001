/****************************************************************************
*                (c) Copyright 2002-2006 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
* FILENAME:   flash.c
*
* DESCRIPTION header file for file flash.c
*
* $Revision: 57 $
* $Date: 11/03/16 11:02a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __FLASH_H__
#define __FLASH_H__


#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"

/* For ESMT part, serial flash, 1MB */
#define MAN_ESMT                0x8C
#define F25L08PA_TYPE           0x20
#define F25L08PA_CAPACITY       0x14

/* For SST part */
#define MAN_SST                 0xBF
#define SST_SST39LF040_ID       0xD7
#define SST_SST39VF088_ID       0xD8
#define SST_SST25VF080_ID       0x8E    // 1 MByte
#define SST_SST25VF040_ID       0x8D    // 512KByte
#define SST_25VF                0x25    // Serial Flash

/* For STMicroelectronics part */
#define MAN_ST                  0x20
#define ST_M25PE80_ID           0x80
#define ST_CAPACITY             0x14

#define MAN_OLD_M25P80              0xFF // Out of Production in 2006 Rev <= 5, Clock rate is 50MHz
#define ST_OLD_M25P80_ID            0xFF
#define ST_OLD_M25P80_CAPACITY      0xFF

#define MAN_M25P80              0x20	// Rev >= 6, Clock rate increased to 75MHz
#define ST_M25P80_ID            0x20
#define ST_M25P80_CAPACITY      0x14

/* For Winbond part */
#define MAN_WINBOND             0xEF
#define WB_W25X80_ID            0x30
#define WB_W25Q80_ID            0x40
#define WB_CAPACITY             0x14
#define WB_W25Q16BV_ID          0x40

/* For FUJISTU part */
#define MAN_FUJISTU             0x04
#define FUJISTU_MBM29LV400BC_ID 0xBA

/* For ATMEL part */
#define MAN_ATMEL               0x1F
#define ATMEL_AT49F004_ID       0x11

/* For MXIC part */
#define MAN_MXIC                0xC2
#define MXIC_MX29LV004B_ID      0xB6
#define MXIC_MX29LV800B_ID      0x5B
#define MXIC_MX25LV8005_ID      0x14     // 1M Byte Serial Flash
#define MXIC_MX25L8006_ID      0x14     // 1M Byte Serial Flash
#define MXIC_20VF               0x20     // Serial Flash
#define MXIC_MX25L160D_ID       0x20
#define MXIC_CAPACITY_2M        0x15  // 2MB Serial Flash

/* For EON part */
#define MAN_EON                 0x1C
#define MAN_EON2                0x7F
#define EON_EN29LV800AB_ID      0x5B

/* For SANYO part */
#define MAN_SANYO             0x62
#define SANYO_LE25FW806_ID            0x26
#define SANYO_CAPACITY             0x62

/* For SPANSION part */
//512K
#define MAN_SPANSION            0x01
#define S25FL040A               0x02
#define SS_CAPACITY             0x26 //bottom boot

//2MB
#define S25FL016A_TYPE          0x02
#define S25FL016A_CAPACITY      0x14

#if (PCB == 710 )||(PCB == 711) || (PCB == 815)||(PCB == 817)
#if (SUN_FUNC_TEST == 0)
/* Flash Work Area Address Defines */
#define FLASH_CODE_OFFSET              (0x00000500L)                                     // Offest Partial Boot Code into Flash.

#define FLASH_WORK_OFFSET           0x0000L
#define FLASH_WORK                  ((BYTE *)(SRAM_BASE + FLASH_WORK_OFFSET))
#define FLASH_WORK_END              0x1000

//Flash write program address
#define FLASH_PROG_OFFSET           0x0000L
#define FLASH_PROG                  ((BYTE *)(FLASH_WORK + FLASH_PROG_OFFSET))  /* 0x70001000L */

// Write Error Flag
#define FLASH_ERR_FLAG              ((BYTE *)(FLASH_WORK + 0x0F40L))            /* 0x70001A40L */

// FLASH_TYPE
#define FLASH_TYPE                  ((WORD *)(FLASH_WORK + 0x0F42L))            /* 0x70001A42L */

// ERASE_BLOCK_MASK
#define ERASE_BLOCK_MASK            ((WORD *)(FLASH_WORK + 0x0F44L))            /* 0x70001A44L */

// FLASH_COMMAND_ADDR1
//#define FLASH_COMMAND_ADDR1         ((ULONG *)(FLASH_WORK + 0xF48L))           /* 0x7000F48L */

// FLASH_COMMAND_ADDR2
//#define FLASH_COMMAND_ADDR2         ((ULONG *)(FLASH_WORK + 0xF4CL))           /* 0x7000F4CL */

/* Flash prog start address, include callvia.o */
#define FLASH_BLK_START             ((BYTE *)(FLASH_BASE + FLASH_CODE_OFFSET))        /* 0x30001000L */


#define PARA_BLK_START_OFFSET       (0x00004000L)                                     // Offest Persistent Parameters into Flash.
#define PARA_BLK_START              ((BYTE *)(FLASH_BASE + PARA_BLK_START_OFFSET))    // Start of Persistent Parameters.
#define PARA_BLK_SIZE               (0x00001000L)                                     // Size of Persistent Parameters in Bytes.
#define FLASH_PARA_SIZE             0x0380

#define PARA_AACSKEY_OFFSET         0xE00L                                            // AACS Key Offset relative to Persistent Table
#define FLASH_AACSKEY_SIZE          192

#define KEY_PARA_OFFSET              (0x00006000L)                                     // Offest Partial KEY Parameters into Flash.
#define KEY_PARA_START               ((BYTE *)(FLASH_BASE + KEY_PARA_OFFSET))     // Start of Partial KEY Parameters.
#define KEY_PARA_SIZE                (0x00001000L)                                     // Size of Partial KEY Parameters in Bytes.

#define MKB_BLK_START_OFFSET        (0x00008000L)                                     // Offest Partial MKB Parameters into Flash.
#define MKB_BLK_START               ((BYTE *)(FLASH_BASE + MKB_BLK_START_OFFSET))     // Start of Partial MKB Parameters.
#define MKB_BLK_SIZE                (0x00008000L)                                     // Size of Partial MKB Parameters in Bytes.

#if (STORE_MSG_TO_FLASHROM==1)
//----------------debug message table--------------------//
#define DEBUG_MSG_OFFSET              (0x00014000L)                                     // Offest Partial MSG Parameters into Flash.
#define DEBUG_MSG_START               ((BYTE *)(FLASH_BASE + DEBUG_MSG_OFFSET))     // Start of Partial MSG Parameters.
#define DEBUG_MSG_SIZE                (LOGGING_MSG_PAGE_SIZE*DRAM_PAGE_SIZE)                                     // Size of Partial MSG Parameters in Bytes.
#endif

#define CHECK_SUM_PROTECT_START     (0x00000000L)
#define CHECK_SUM_PROTECT_END        (DEBUG_MSG_OFFSET + DEBUG_MSG_SIZE -1)
#else
//SUN_FUNC_TEST == 1 == Start
/* Flash Work Area Address Defines */
#define FLASH_CODE_OFFSET              (0x00000000L)                                     // Offest Partial Boot Code into Flash.

#define FLASH_WORK_OFFSET           0x0000L
#define FLASH_WORK                  ((BYTE *)(SRAM_BASE + FLASH_WORK_OFFSET))
#define FLASH_WORK_END              0x1800

//Flash write program address
#define FLASH_PROG_OFFSET           0x0000L
#define FLASH_PROG                  ((BYTE *)(FLASH_WORK + FLASH_PROG_OFFSET))  /* 0x70001000L */

// Write Error Flag
#define FLASH_ERR_FLAG              ((BYTE *)(FLASH_WORK + 0x1740L))            /* 0x70001A40L */


// FLASH_TYPE
#define FLASH_TYPE                  ((WORD *)(FLASH_WORK + 0x1742L))            /* 0x70001A42L */

// ERASE_BLOCK_MASK
#define ERASE_BLOCK_MASK            ((ULONG *)(FLASH_WORK + 0x1744L))            /* 0x70001A44L */

// FLASH_COMMAND_ADDR1
//#define FLASH_COMMAND_ADDR1         ((ULONG *)(FLASH_WORK + 0x0F48L))           /* 0x70001A48L */

// FLASH_COMMAND_ADDR2
//#define FLASH_COMMAND_ADDR2         ((ULONG *)(FLASH_WORK + 0x0F4CL))           /* 0x70001A4CL */

/* Flash prog start address, include callvia.o */
#define FLASH_BLK_START             ((BYTE *)(FLASH_BASE + FLASH_CODE_OFFSET))        /* 0x30001000L */

//----------------boot code table--------------------//
#define BOOT_CODE_OFFSET              (0x00000600L)                                     // Offest Partial Boot Code into Flash.
#define BOOT_CODE_START               ((BYTE *)(FLASH_BASE + BOOT_CODE_OFFSET))     // Start of Partial Boot Code.
#define BOOT_CODE_SIZE                (0x00004A00L)                                     // Size of Partial Boot Code in Bytes.

//----------------MKB table--------------------//
#define MKB_BLK_START_OFFSET        (0x00005000L)                                     // Offest Partial MKB Parameters into Flash.
#define MKB_BLK_START               ((BYTE *)(FLASH_BASE + MKB_BLK_START_OFFSET))     // Start of Partial MKB Parameters.
#define MKB_BLK_SIZE                (0x00008000L)                                     // Size of Partial MKB Parameters in Bytes.

//----------------back up table--------------------//
#define KEY_PARA_BKUP_OFFSET              (0x0000D000L)                                     // Offest Partial KEY Parameters BK into Flash.
#define KEY_PARA_BKUP_START               ((BYTE *)(FLASH_BASE + KEY_PARA_BKUP_OFFSET))     // Start of Partial KEY Parameters BK.
#define KEY_PARA_BKUP_SIZE                (0x00001000L)                                     // Size of Partial KEY Parameters BK in Bytes.

#define PARA_BLK_BKUP_START_OFFSET       (0x0000E000L)                                     // Offest Persistent Parameters into Flash.
#define PARA_BLK_BKUP_START              ((BYTE *)(FLASH_BASE + PARA_BLK_BKUP_START_OFFSET))    // Start of Persistent Parameters.
#define PARA_BLK_BKUP_SIZE               (0x00001000L)                                     // Size of Persistent Parameters in Bytes.

#define AGAIN_SL_BKUP_OFFSET              (0x0000F000L)                                     // Offest Partial AGIN Parameters BK into Flash.
#define AGAIN_SL_BKUP_START               ((BYTE *)(FLASH_BASE + AGAIN_SL_BKUP_OFFSET))     // Start of Partial AGIN Parameters BK.
#define AGAIN_SL_BKUP_SIZE                (0x00001000L)                                     // Size of Partial AGIN Parameters BK in Bytes.

#define KEY_PARA_OFFSET              (0x00011000L)                                     // Offest Partial KEY Parameters into Flash.
#define KEY_PARA_START               ((BYTE *)(FLASH_BASE + KEY_PARA_OFFSET))     // Start of Partial KEY Parameters.
#define KEY_PARA_SIZE                (0x00001000L)                                     // Size of Partial KEY Parameters in Bytes.

#define PARA_BLK_START_OFFSET       (0x00012000L)                                     // Offest Persistent Parameters into Flash.
#define PARA_BLK_START              ((BYTE *)(FLASH_BASE + PARA_BLK_START_OFFSET))    // Start of Persistent Parameters.
#define PARA_BLK_SIZE               (0x00001000L)                                     // Size of Persistent Parameters in Bytes.
#define FLASH_PARA_SIZE             0x2000

#define PARA_AACSKEY_OFFSET         0xE00L                                            // AACS Key Offset relative to Persistent Table
#define FLASH_AACSKEY_SIZE          192

#define AGAIN_SL_OFFSET              (0x00013000L)                                     // Offest Partial AGIN Parameters into Flash.
#define AGAIN_SL_START               ((BYTE *)(FLASH_BASE + AGAIN_SL_OFFSET))     // Start of Partial AGIN Parameters.
#define AGAIN_SL_SIZE                (0x00001000L)                                     // Size of Partial AGIN Parameters in Bytes.
#if (ENABLE_AACS == 1)
#define AACS_BLK_START_OFFSET       PARA_BLK_START_OFFSET
#endif

#if (STORE_MSG_TO_FLASHROM==1)
//----------------debug message table--------------------//
#define DEBUG_MSG_OFFSET              (0x00014000L)                                     // Offest Partial MSG Parameters into Flash.
#define DEBUG_MSG_START               ((BYTE *)(FLASH_BASE + DEBUG_MSG_OFFSET))     // Start of Partial MSG Parameters.
#define DEBUG_MSG_SIZE                (LOGGING_MSG_PAGE_SIZE*DRAM_PAGE_SIZE)                                     // Size of Partial MSG Parameters in Bytes.
#endif

#define CHECK_SUM_PROTECT_START     (0x00000000L)
#define CHECK_SUM_PROTECT_END        (DEBUG_MSG_OFFSET + DEBUG_MSG_SIZE -1)
// END OF SUN_FUNC_TEST == 1
#endif

#else
//PCB != 710 || PCB != 711 ||PCB != 817
/* Flash Work Area Address Defines */
#define FLASH_CODE_OFFSET              (0x00000000L)                                     // Offest Partial Boot Code into Flash.

#define FLASH_WORK_OFFSET           0x0500L
#define FLASH_WORK                  ((BYTE *)(SRAM_BASE + FLASH_WORK_OFFSET))
#define FLASH_WORK_END              0xB00

//Flash write program address
#define FLASH_PROG_OFFSET           0x0000L
#define FLASH_PROG                  ((BYTE *)(FLASH_WORK + FLASH_PROG_OFFSET))  /* 0x70001000L */

// Write Error Flag
#define FLASH_ERR_FLAG              ((BYTE *)(FLASH_WORK + 0x0A40L))            /* 0x70001A40L */

// FLASH_TYPE
#define FLASH_TYPE                  ((WORD *)(FLASH_WORK + 0x0A42L))            /* 0x70001A42L */

// ERASE_BLOCK_MASK
#define ERASE_BLOCK_MASK            ((WORD *)(FLASH_WORK + 0x0A44L))            /* 0x70001A44L */

// FLASH_COMMAND_ADDR1
//#define FLASH_COMMAND_ADDR1         ((ULONG *)(FLASH_WORK + 0x0A48L))           /* 0x70001A48L */

// FLASH_COMMAND_ADDR2
//#define FLASH_COMMAND_ADDR2         ((ULONG *)(FLASH_WORK + 0x0A4CL))           /* 0x70001A4CL */

/* Flash prog start address, include callvia.o */
#define FLASH_BLK_START             ((BYTE *)(FLASH_BASE + 0x00000500L))        /* 0x30001000L */

#define SLWSO_START_OFFSET       (0x00001000L)                                     // Offest Persistent Parameters into Flash.
#define SLWSO_START              ((BYTE *)(FLASH_BASE + SLWSO_START_OFFSET))    // Start of Persistent Parameters.
#define SLWSO_SIZE               (0x00001000L)                                     // Size of Persistent Parameters in Bytes.

#define PARA_BLK_START_OFFSET       (0x00002000L)                                     // Offest Persistent Parameters into Flash.
#define PARA_BLK_START              ((BYTE *)(FLASH_BASE + PARA_BLK_START_OFFSET))    // Start of Persistent Parameters.
#define PARA_BLK_SIZE               (0x00001000L)                                     // Size of Persistent Parameters in Bytes.
#define FLASH_PARA_SIZE             0x0380

#define PARA_AACSKEY_OFFSET         0xE00L                                            // AACS Key Offset relative to Persistent Table
#define FLASH_AACSKEY_SIZE          192

#define MKB_BLK_START_OFFSET        (0x00003000L)                                     // Offest Partial MKB Parameters into Flash.
#define MKB_BLK_START               ((BYTE *)(FLASH_BASE + MKB_BLK_START_OFFSET))     // Start of Partial MKB Parameters.
#define MKB_BLK_SIZE                (0x00008000L)                                     // Size of Partial MKB Parameters in Bytes.
#define CHECK_SUM_PROTECT_START     (SLWSO_START_OFFSET)
#define CHECK_SUM_PROTECT_END        (MKB_BLK_START_OFFSET + MKB_BLK_SIZE -1)

#endif //PCB == 710 PCB == 711 PCB == 817

/* Externals */
#if (STORE_MSG_TO_SDRAM == 1)
extern ULONG   flashpara_Store_msg_addr;
extern BYTE    flashpara_LoggingFull;
#endif

#if (SUN_FUNC_TEST == 1)
extern BYTE    fgEEPROMChg;
#endif

extern void FlashWriteFW( void );
extern char FlashWriteParaTbl( BYTE );
enum{
#if (PCB == 710)||(PCB == 711)|| (PCB == 815)||(PCB == 817)
#if (SUN_FUNC_TEST == 0)
    ePersistent_Talbe,
#if  SERVOKEYPARAMETER == 1
    eKey_parameter_Table,
#endif
    eFlash_Debugging_Table,
#else
    eEEPROM_Formal_Table,
    eEEPROM_Backup_Table,
    ePUH_Table,//unused
    eKey_parameter_Table,
    eKey_parameter_Backup_Table,
    eFlash_Debugging_Table,
    ePersistent_Talbe =0x80,
    ePersistent_Backup_Talbe,
#endif
#else
    ePersistent_Talbe,
#endif
#if (ENABLE_AACS == 1)
    eMKB_Table,
    eAACS_Table
#endif
};

#endif      /* #ifndef __FLASH_H__ */
