/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   at_equ.h
*
* DESCRIPTION
* This file contains equates of ATAPI command opcodes
*
*   $Revision: 29 $
*   $Date: 11/03/07 11:41a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __AT_EQU_H__
#define __AT_EQU_H__

/*********************************************
        X3T13 ATA command Opcodes
**********************************************/
                                                /*** Mandatory per X3T13 ATA/ATAPI-6 spec for Packet Devices ***/
#define ATA_NOP                         0x00    /* ATA NOP Command */
#define ATA_DEVICE_RESET                0x08    /* Device Reset Command */
#define ATA_EXEC_DEVICE_DIAG            0x90    /* Execute Device Diagnostics Command */
#define ATA_PACKET                      0xA0    /* Packet Command */
#define ATA_IDENTIFY_PACKET_DEVICE      0xA1    /* Identify Packet Device Command */
#define ATA_STANDBY_IMMED               0xE0    /* Standby Immediate Command */
#define ATA_IDLE_IMMED                  0xE1    /* Idle Immediate Command */
#define ATA_CHECK_POWER_MODE            0xE5    /* Check Power Mode Command */
#define ATA_SLEEP                       0xE6    /* Sleep Command */
#define ATA_SET_FEATURE                 0xEF    /* Set Features Command */
                                                /*** Optional per X3T13 ATA/ATAPI-6 spec for Packet Devices ***/
#define ATA_STANDBY                     0xE2    /* Standby Command */
#define ATA_IDLE                        0xE3    /* Idle Command */
#define ATA_FLUSH_CACHE                 0xE7    /* Flush Cache Command */
#define ATA_GET_MEDIA_STATUS			0xDA    /* Get Media Status Command */

/* Vendor Unique ATA Opcodes */
#define HFLASH_ENABLE                   0xF0    /* Hardware enable of the Host Flash Feature */
#define HFLASH_DISABLE                  0xFF    /* Hardware disable of the Host Flash Feature */

/*********************************************
          Packet Command Opcodes
**********************************************/

/*********************************************
SFF ATAPI Command Opcodes
**********************************************/
#define READ_HEADER                     0x44    /* Read Header Command - Not required by MMC, SBC, or SPC specs */
#define REZERO_UNIT                     0x01    /* Rezero Unit Command - Not required by MMC, SBC, or SPC specs */

/*******************************************
SBC - Commands
These commands are defined in the SCSI
Block Commands specification (SBC-2)
********************************************/
#define SBC_READ_10                     0x28    /* Read 10 Command */
#define SBC_READ_12                     0xA8    /* Read 12 Command */
#define SBC_SEEK                        0x2B    /* Seek 10 Command */
#define SBC_START_STOP_UNIT             0x1B    /* Start/Stop Unit Command */
#define SBC_VERIFY_10                   0x2F    /* Verify 10 Command */
#define SBC_VERIFY_12                   0xAF    /* Verify 12 Command */

/*******************************************
SPC - SCSI Primary Commands
These commands are defined in the SCSI
Primary Command specification (SPC-2)
********************************************/
#define SPC_INQUIRY                         0x12    /* Inquiry Command */
#define SPC_MODE_SELECT                     0x55    /* Mode Select Command */
#define SPC_MODE_SENSE                      0x5A    /* Mode Sense Command */
#define SPC_PREVENT_ALLOW_MEDIUM_REMOVAL    0x1E    /* Prevent/Allow Medium Removal Command */
#define SPC_READ_BUF                        0x3C    /* Read Buffer Command */
#define SPC_REQUEST_SENSE                   0x03    /* Request Sense Command */
#define SPC_TEST_UNIT_READY                 0x00    /* Test Unit Ready Command */
#define SPC_WRITE_BUF                       0x3B    /* Write Buffer Command */

/*******************************************
MMC - Multi-Media Commands
These commands are defined in the Multi-
Media Command specification (MMC-4)
********************************************/
#define MMC_BLANK                           0xA1    /* Blank Command */
#define MMC_CLOSE_SESSION                   0x5B    /* Close Track/Session Command */
#define MMC_ERASE_10                        0x2C    /* Erase 10 Command */
#define MMC_FORMAT                          0x04    /* Format Unit Command */
#define MMC_GET_CONFIGURATION               0x46    /* Get Configuration Command */
#define MMC_GET_EVENT_STATUS_NOTIFICATION   0x4A    /* Get Event Status Notification Command */
#define MMC_GET_PERFORMANCE                 0xAC    /* Get Performance Command */
#define MMC_MECHANISM_STATUS                0xBD    /* Mechanism Status Command */
#define MMC_PAUSE_RESUME                    0x4B    /* Pause/Resume Command */
#define MMC_PLAY_AUDIO_10                   0x45    /* Play Audio 10 Command */
#define MMC_PLAY_AUDIO_12                   0xA5    /* Play Audio 12 Command */
#define MMC_PLAY_AUDIO_MSF                  0x47    /* Play Audio MSF Command */
#define MMC_READ_BUF_CAP                    0x5C    /* Read Buffer Capacity Command */
#define MMC_READ_CAPACITY                   0x25    /* Read Capacity Command */
#define MMC_READ_CD                         0xBE    /* Read CD Command */
#define MMC_READ_CD_MSF                     0xB9    /* Read CD MSF Command */
#define MMC_READ_DISK_INFO                  0x51    /* Read Disc Information Command */
#define MMC_READ_DISC_STRUCTURE             0xAD    /* Read Disc Structure Command */
#define MMC_SEND_DISC_STRUCTURE             0xBF    /* Send Disc Structure Command */
#define MMC_READ_FORMAT_CAPACITIES          0x23    /* Read Format Capacities Command */
#define MMC_READ_SUB_CHANNEL                0x42    /* Read Sub Channel Command */
#define MMC_READ_TOC                        0x43    /* Read TOC/PMA/ATIP Command */
#define MMC_READ_TRACK_INFO                 0x52    /* Read Track Information Command */
#define MMC_REPAIR_TRACK                    0x58    /* Repair Track Command */
#define MMC_REPORT_KEY                      0xA4    /* Report Key Command - DVD Only */
#define MMC_RESERVE_TRACK                   0x53    /* MMC_RESERVE_TRACK */ /* Reserve Track Command */
#define MMC_SCAN                            0xBA    /* Scan Command */
#define MMC_SEND_CUE_SHEET                  0x5D    /* Send Cue Sheet Command */
#define MMC_SEND_KEY                        0xA3    /* Send Key Command - DVD Only */
#define MMC_SEND_OPC_INFO                   0x54    /* Send OPC Information Command */
#define MMC_SET_CD_SPEED                    0xBB    /* Set CD Speed Command */
#define MMC_SET_READ_AHEAD                  0xA7    /* Set Read Ahead Command */
#define MMC_SET_STREAMING                   0xB6    /* Set Streaming Command */
#define MMC_STOP_PLAY_SCAN                  0x4E    /* Stop Play/Scan Command */
#define MMC_SYNC_CACHE                      0x35    /* Sync Cache Command */
#define MMC_WRITE_10                        0x2A    /* Write 10 Command */
#define MMC_WRITE_12                        0xAA    /* Write 12 Command */
#define MMC_WRITE_VERIFY_10                 0x2E    /* Write and Verify 10 */

/*******************************************
  Vender Unique Diagnostic Commands
********************************************/
#define VENDOR_SEND_KEY                 0x13    /* Vendor Send Key Command */
#define VENDOR_REPORT_KEY               0x14    /* Vendor Report Key Command */
#define VENDOR_READ_DISC_STRUCTURE      0x22    /* Vendor Read Disc Structure Command */
#if (SUN_FUNC_TEST == 1)
#define DIAG_LASER                      0xEE    /* Laser Measurment Command */
#endif

#define DIAG_CMD_ENA                    0xF0    /* Enable/Disable Diagnostics */
#define DIAG_CMD_SEEPROM                0xF1    /* Serial EEPROM Read/Write */
#define DIAG_CMD_MRW                    0xF2    /* Mt. Rainier Diagnostic Suite */
#define DIAG_CMD_JITTER                 0xF3    /* Jitter */
#define DIAG_VENDOR_READ                0xF4    /* Vendor Read Commands */
#define DIAG_VENDOR_WRITE               0xF5    /* Vendor Write Command */
#define DIAG_CMD_DVD                    0xF6    /* Diagnostic DVD Command */
#define READ_WRITE_DEVICE               0xF7    /* READ/WRITE DEVICE COMMAND */
#define DIAG_CMD_OFFLINE_CALIBRATION    0xF8    /* OFF Line Calbration Vendor Command */
#define DIAG_CMD_DC_ERASE               0xF9    /* DC Erase */
#define FLASH_CODE                      0xFA    /* Flash Download - This command is not checked for DIAG Enable */
#define DIAG_CMD_LOAD_DFL               0xFB    /* Load external BD-R/RE defect list to SDRAM */
#define LS_CMD_PRINT           0xFC    /* Light Scribe print command  */
#define DIAG_TRANSPARENT_SEND_CMD       0xFD    // Diagnostic "Transparant Send    Command"
#define DIAG_CMD_DRAM_LOG_CONTROL       0xFE    /* DRAM debugging log area control Command*/
#define DIAG_CMD_FREE_FF                0xFF    /* Free Command*/

/*************************************************
    Set feature option EQU
**************************************************/

/* The value in feature register */
#define SET_XFR_MODE        0x03    /* set xfer mode based on  */
#define ENABLE_AAM          0x42    /* Enable Automatic Acoustic Management */
#define DISABLE_AAM         0xC2    /* Disable Automatic Acoustic Management */
#define ENABLE_MEDIA_STATUS_NOTIFICATION	0x95	/* enable media status notification */
#define DISABLE_MEDIA_STATUS_NOTIFICATION	0x31	/* disable media status notification */

#define ENABLE_SATA_FEATURE		0x10    	/* Enable use of SATA feature */
#define DISABLE_SATA_FEATURE	0x90    	/* Disable use of SATA feature */
#define INIT_PWR_STATE_TRANS	0x03    	/* Device initiate power state transitions */
#define ASYN_NOTIFICATION		0x05		/* Asynchronous Notification */
#define SSP						0x06		/* Software Settings Preservation */

/* The value in sector count register */
#define DISABLE_REVERT      0x66    /* disable reverting power on default */
#define ENABLE_REVERT       0xCC    /* enable reverting power on default */
#define DEF_XFR_MODE        0x00    /* PIO default transfer mode */
#define DISABLE_IORDY       0x01    /* disable IO channel ready */
#define PIO_XFR_MODE        0x08    /* pio flow control xfer mode */
#define SINGLE_WORD_DMA     0x10    /* single_word DMA */
#define MULTI_WORD_DMA      0x20    /* multi_word DMA */
#define UDMA_MODE           0x40    /* ultra DMA */
/* Supported Automatic Acoustic Management */
#define MAX_Preformance     0xFE    /* Max Performance */
#define IntermediateAM_lvls 0x85    /* Intermediate acoustic management level */
#define IntermediateAM_lvl4 0x84    /* Intermediate acoustic management level */
#define IntermediateAM_lvl3 0x83    /* Intermediate acoustic management level */
#define IntermediateAM_lvl2 0x82    /* Intermediate acoustic management level */
#define IntermediateAM_lvl1 0x81    /* Intermediate acoustic management level */
#define MIN_AE_lvl          0x80    /* Min Acoustic emanation level */
#define Retired             0x01    /* Retired */
#define Vender_spec         0x00    /* Vender Specific */

#endif /* __AT_EQU_H__ */

