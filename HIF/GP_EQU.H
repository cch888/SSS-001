/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_equ.h
*
* DESCRIPTION
*   This file contains general purpose equates definition
*
* $Revision: 57 $
* $Date: 11/03/01 6:13p $
*
***************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_EQU_H__
#define __GP_EQU_H__


/* Definition for player mode */

#define EXECUTING_TRAY_OUT_MODE         0
#define TRAY_OUT_MODE                   1
#define EXECUTING_TRAY_IN_MODE          2
#define TRAY_IN_MODE                    3
#define EXECUTING_START_UP_MODE         4
#define START_UP_MODE                   5
#define EXECUTING_STOP_MODE             6
#define STOP_MODE                       7
#define EXECUTING_PLAY_TRACK_MODE       8
#define PLAY_TRACK_MODE                 9
#define EXECUTING_PAUSE_ON_MODE         10
#define PAUSE_ON_MODE                   11
#define EXECUTING_PAUSE_OFF_MODE        12
#define PAUSE_OFF_MODE                  13
#define EXECUTING_SEEK_MODE             14
#define SEEK_MODE                       15
#define EXECUTING_READ_TOC_MODE         16
#define READ_TOC_MODE                   17
#define EXECUTING_READ_SUBCODE_MODE     18
#define READ_SUBCODE_MODE               19
#define EXECUTING_SINGLE_SPEED_MODE     20
#define SINGLE_SPEED_MODE               21
#define PAUSING_MODE                    22
#define PREFETCH_MODE                   23
#define READING_MODE                    24
#define AUDIO_PAUSE_MODE               157
#define AUDIO_ERROR_MODE                26
#define STOP_SCAN_MODE                  27
#define SET_VOLUME_MODE                 28
#define MONITOR_SLEEP_MODE              29
#define KERNEL_SLEEP_MODE               30
#define EXECUTING_WRITE_MODE            31
#define SEARCH_ISRC_CATALOG_MODE        32
#define CHK_RESERVE_TRACK_MODE          33
#define RESERVE_TRACK_MODE              34
#define VERIFY_BLANK_MODE               35
#define JUMP_TO_ADDR_MODE               36
#define CALIBRATE_MODE                  37
#define FIX_UP_MODE                     38
#define WRITE_PMA_MODE                  39
#define RECOVER_MODE                    40
#define FINISH_TRACK_MODE               41
#define EXECUTING_ERASE_MODE            42
/* Definition for read related equates */

#define SEEK_RETRY_LIMIT                10
#define RW_HEAD_STABLE_CNT               3

/* Definition for power management related mode */

#define IDLE_MODE           0
#define STANDBY_MODE        1
#define SLEEP_MODE          2

/* Definition for EccStatus */
#define MSBERROR            0x2
#define MODERROR            0x4
#define READFAILERROR       0x8
#define HEADC2ERROR         0x10
#define OVERSHOOTERROR      0x20
#define SYNCERROR           0x40
#define EDCERROR            0x80

/* Definition for Iram_Rw_read_mode */
#define eUnknownMode        0
#define eMode1              1
#define eMode2Form1         2
#define eMode2Form2         3
#define eYellowMode2        4
#define eCDDAMode           5

/* cover disc control */
/* L -> H speed */
#define RECOVERFRAME        12000       /* 5s */

#define SPEED_UP_COUNT_LIMIT                                  30000
#define SPEED_DOWN_COUNT_LIMIT                                (-100)

#define SPEED_UP_READY_LIMIT                                  30000
#define SPEED_DOWN_READY_LIMIT                                (-100)

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
#define CD_SPEED_UP_READY                                     SP16X // 2822.4 Kbyte// unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed
#define CD_SPEED_DOWN_READY                                   (-12)// ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22) // == change reason modfiy the CD speed control for CDSpeed test.
#if (Customer_Dune_HD == 1)//[V08] Bios DVD 8x for high bit rate HDI
#define DVD_SPEED_UP_READY                                    3072 // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed 
#define DVD_SPEED_DOWN_READY                                  (-3)
#else
#define DVD_SPEED_UP_READY                                    SP4XDVD // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed
#define DVD_SPEED_DOWN_READY                                  (-5)
#endif
#define BD_SPEED_UP_READY                                     SP2XBD // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed
#define BD_SPEED_DOWN_READY                                   (-10)//==modify BD speed down & retry counter

#else
#define CD_SPEED_UP_READY                                     768 // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed * 2.5
#define CD_SPEED_DOWN_READY                                   (-5)

#define DVD_SPEED_UP_READY                                    3072 // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed * 2.5
#define DVD_SPEED_DOWN_READY                                  (-3)

#define BD_SPEED_UP_READY                                     5618 // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed * 2.5
#define BD_SPEED_DOWN_READY                                   (-3)
#endif
#define HD_SPEED_UP_READY                                     5618 // unit: 1 = 1 sector = 2KB, decode sectors of the lowest speed * 2.5
#define HD_SPEED_DOWN_READY                                   (-3)

#define SPEED_DOWN_FOR_SEEK_FAIL				-1001
#define SPEED_DOWN_FOR_POWER_SAVE				-1002

#define BOUNDARY_BETWEEN_PLAY_AND_COPY_MODE    80
#define CDRW_SPEED_UP_ACCUMULATION                        4

/*===============================================================
    MISCELLANEOUS DEFINES FOR GET PERFORMANCE COMMAND
===============================================================*/
/*---------------------------------------------------------------
    Mask for Tolerance bits ( Torlerance = bits 3 and 4 )
    Mask for Write bit      ( Write      = bit  2       )
    Mask for Except bits    ( Except     = bits 0 and 1 )
---------------------------------------------------------------*/
#define TOLERANCE_MASK              (bit4 | bit3)
#define WRITE_MASK                  bit2
#define EXCEPT_MASK                 (bit1 | bit0)

/*---------------------------------------------------------------
    Except definitions
---------------------------------------------------------------*/
#define EX_NOMINAL_PERORM           0x00
#define EX_EXCEPT_PERORM_01         0x01
#define EX_EXCEPT_PERORM_10         0x02

/*---------------------------------------------------------------
    Configuration data sizes
---------------------------------------------------------------*/
#define MAX_PFMDATA_LEN             0xFFF6  /* Maximum performance data return size     */
#define PERFORM_HEADER              0x08    /* Size of header                           */
#define PERFORM_EXCEPTION           0x06    /* Size of Exception performance descriptor */
#define GETPFM_MAX_BYTES            0xFFFE  /* Max bytes that can be returned in 1 cmd  */

/*---------------------------------------------------------------
    Seek Delay data
        ((13.3[msec/frame] / Speed) * 14[frame]) + 10[msec]
            13.3  : 1 frame read time (1X)
            Speed : rotate speed
            14    : delay frame
            10    : delay time between Cmd Receive and seek operation
    <NOTE> Average time = (outside time + inside time) / 2
            "outside speed = inside speed * 2.5"
---------------------------------------------------------------*/
#define SD_1X   0x0D1F      /*  1X Seek Delay Average time [msec] */
#define SD_2X   0x06C1      /*  2X Seek Delay Average time [msec] */
#define SD_4X   0x0393      /*  4X Seek Delay Average time [msec] */
#define SD_8X   0x01FB      /*  8X Seek Delay Average time [msec] */
#define SD_10X  0x01AA      /* 10X Seek Delay Average time [msec] */
#define SD_12X  0x0174      /* 12X Seek Delay Average time [msec] */
#define SD_16X  0x0130      /* 16X Seek Delay Average time [msec] */
#define SD_24X  0x00EC      /* 24X Seek Delay Average time [msec] */
#define SD_32X  0x00CA      /* 32X Seek Delay Average time [msec] */
#define SD_40X  0x00B5      /* 40X Seek Delay Average time [msec] */

/*===============================================================
    MISCELLANEOUS DEFINES FOR FLASH UP via CD
===============================================================*/
//#define F2D_NOT_READY         0
//#define F2D_BECOMING          1
//#define F2D_AUTO_TRAY_IN      2
//#define F2D_AUTO_TRAY_OUT     3
//#define F2D_EXECUTING         4
//#define F2D_CANCEL                5

/*===============================================================
    MISCELLANEOUS DEFINES FOR GET CONFIGURATION COMMAND
===============================================================*/
/*---------------------------------------------------------------
    Mask for Reqeust Type bits ( Reqeust Type = bits 0 and 1 )
---------------------------------------------------------------*/
#define REQUEST_TYPE_MASK           (bit1 | bit0)

/*---------------------------------------------------------------
    Request Type definitions
---------------------------------------------------------------*/
#define RT_ALL_FEATURES             0x00
#define RT_ACTIVE_FEATURES          0x01
#define RT_SINGLE_FEATURE           0x02
#define RT_RESERVED                 0x03

/*---------------------------------------------------------------
    Configuration data sizes
---------------------------------------------------------------*/
#define MAX_CFGDATA_LEN             0xFFF6  /* Max cfg data return size(Exclude header) */
#define FEATURE_HEADER              0x08    /* Size of header                           */
#define ONE_DATA_SECTOR             0x800   /* Size of one data sector                  */
#define GETCFG_MAX_SECTORS          0x20    /* Max sectors that can be returned         */
#define GETCFG_MAX_BYTES            0xFFFE  /* Max bytes that can be returned in 1 cmd  */

/*---------------------------------------------------------------
    Feature List
---------------------------------------------------------------*/
#define FL_PROFILE_LIST                        0x0000
#define FL_CORE                                0x0001
#define FL_MORPHING                            0x0002
#define FL_REMOVABLE_MEDIA                     0x0003
#define FL_RANDOM_READABLE                     0x0010
#define FL_MULTI_READ                          0x001D
#define FL_CD_READ                             0x001E
#define FL_DVD_READ                            0x001F
#define FL_RANDOM_WRITABLE                     0x0020
#define FL_INCREMENTAL_STREAMING_WRITABLE      0x0021
#define FL_FORMATTABLE                         0x0023
#define FL_DEFECT_MGT                          0x0024
#define FL_WRITE_ONCE                          0x0025
#define FL_RESTRICTED_OVERWRITE                0x0026
#define FL_CDRW_CAV_WRITE                      0x0027
#define FL_ENHANCED_DEFECT_REPORTING           0x0029
#define FL_DVD_PLUS_RW                         0x002A
#define FL_DVD_PLUS_R                          0x002B
#define FL_RIGID_RESTRICTED_OVERWRITE          0x002C
#define FL_CD_TAO                              0x002D
#define FL_CD_MASTERING                        0x002E
#define FL_DVD_MINUS_RW_WRITE                  0x002F
#define FL_DVD_MINUS_R_DUAL                    0x0033
#define FL_CDRW_MEDIA_WRITE                    0x0037
#define FL_BDR_POW                             0x0038
#define FL_DVD_PLUS_R_DUAL                     0x003B
#define FL_BD_READ                             0x0040
#define FL_BD_WRITE                            0x0041
#define FL_TSR                                 0x0042
#define FL_HD_READ                             0x0050
#define FL_HD_WRITE                            0x0051
#define FL_HDRW_FRAGMENT_RECORDING             0x0052
#define FL_HYBIRD_DISC                         0x0080
#define FL_POWER_MGT                           0x0100
#define FL_SMART                               0x0101
#define FL_EMBEDDED_CHANGER                    0x0102
#define FL_CD_AUDIO_ANALOG                     0x0103
#define FL_MICROCODE_UPGRADE                   0x0104
#define FL_TIME_OUT                            0x0105
#define FL_DVD_CSS                             0x0106
#define FL_REALTIME_STREAMING                  0x0107
#define FL_SERIAL_NUMBER                       0x0108
#define FL_MEDIA_SERIAL_NUM                    0x0109
#define FL_DVD_CPRM                            0x010B
#define FL_FIRMWARE_INFO                       0x010C
#define FL_AACS                                0x010D
#define FL_DVD_VCPS                            0x0110
#define FL_LS                                  0xFF33

/*---------------------------------------------------------------
    Profile List
---------------------------------------------------------------*/
#define PROFILE_HEADER_SIZE         4
#define DESCRIPTOR_SIZE_BYTE        3
#define PROFILE_DEFAULT             0x0000
#define PROFILE_CDROM               0x0008      /* CDROM */
#define PROFILE_CDR                 0x0009      /* CD-R */
#define PROFILE_CDRW                0x000A      /* CD-RW */
#define PROFILE_DVDROM              0x0010      /* DVDROM */
#define PROFILE_DVDMINUSR           0x0011      /* DVD-R */
#define PROFILE_DVDRAM              0x0012      /* DVDRAM */
#define PROFILE_DVDMINUSRW          0x0013      /* DVD-RW Restric OverWrite*/
#define PROFILE_DVDMINUSRWS         0x0014      /* DVD-RW Sequential Recording*/
#define PROFILE_DVDMINUSRDL         0x0015      /* DVD-R DL Sequential Recording*/
#define PROFILE_DVDMINUSRDLJUMP     0x0016      /* DVD-R DL Layer Jump Recording*/
#define PROFILE_DVDPLUSRW           0x001A      /* DVD+RW */
#define PROFILE_DVDPLUSR            0x001B      /* DVD+R */
#define PROFILE_DVDPLUSRWDL         0x002A      /* DVD+RW DL */
#define PROFILE_DVDPLUSRDL          0x002B      /* DVD+R DL */
#define PROFILE_BDROM               0x0040      /* BD-ROM */
#define PROFILE_BDR_SRM             0x0041      /* BD-R Sequential Recording Mode */
#define PROFILE_BDR_RRM             0x0042      /* BD-R Random Recording Mode */
#define PROFILE_BDRE                0x0043      /* BD-RE */
#define PROFILE_HDROM               0x0050      /* HDROM */
#define PROFILE_HDR                 0x0051      /* HD-R */
#define PROFILE_HDRAM               0x0052      /* HD-RAM */
#define PROFILE_HDRW                0x0053      /* HD-RW */
#define PROFILE_HDRDL               0x0058      /* HD-R DL */
#define PROFILE_HDRWDL              0x005A      /* HD-RW DL */
/*===============================================================
    MISCELLANEOUS DEFINES FOR SET STREAMING COMMAND
===============================================================*/
/*---------------------------------------------------------------
    Mask for RA bit    ( RA    = bit 0 )
    Mask for Exact bit ( Exact = bit 1 )
    Mask for RDD bit   ( RDD   = bit 2 )
---------------------------------------------------------------*/
/* ATAPI specification bit definition */
#define STR_RA_BIT                  bit0
#define STR_EXACT_BIT               bit1
#define STR_RDD_BIT                 bit2

/* vendorspecification bit definition */
#define STR_ENABLE_BIT              bit7

#endif /* __GP_EQU_H__ */
