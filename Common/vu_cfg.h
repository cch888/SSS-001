/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* $Revision: 837 $
* $Date: 11/03/15 3:21p $
*
* DESCRIPTION:
*   This file is a vendor unique configuration file that allows different
*   projects to be configured based on their specific settings
*   as defined below.
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __VU_CFG_H__
#define __VU_CFG_H__

#include ".\oem\oem_info.h"
#include ".\oem\oem_sys_clk.h"

/*****************************************************/
/*               Conditional Switches                */
/*****************************************************/

/********************************************/
/*  Power Control                           */
/********************************************/
#define AFE_POWER_DOWN              0       /* 0: Disable AFE Power Down */
                                            /* 1: Enable AFE Power Down */

#define STARTUP_AT_LOWEST_CAV       1       /* 0: start up ready will stay at spin up speed, now for CD & DVD only */
                                            /* 1: start up ready will change to lowest CAV speed */

/* End of Power conditionals */


/********************************************/
/*  Decoder Conditionals                    */
/********************************************/
#define ENABLE_CDG                  0       /* CD-G Read Support */
                                            /* 1: Enable */
                                            /* 0: Disable */

#define USING_C1                    0       /* 1: Enable C1 Error count to Adjust RF Equalizer for improved readability */
                                            /* 0: Disable RF Equalizer Adjustment */

/* End of Decoder conditionals */


/********************************************/
/*  Disc Size Conditionals                  */
/********************************************/
#define DISC_8CM                    1       /* 0: Disable support for 8CM discs */
                                            /* 1: Enable support for 8CM discs */

/* End of Disc Size Conditionals */

/********************************************/
/*  Host IF and Command Conditional         */
/********************************************/
#define APPEND_RD_RVD               0       /* HIF Appendable transfer for Read commands */
                                            /* 1: Enable */
                                            /* 0: Disable */

#define AUTO_STATU_CPLT             0       /* Auto Status Completion   */
                                            /* 1: Enable Auto Status Completion */
                                            /* 0: Disable Auto Status Completion */

#define ENABLE_AUTO_ABORT           1       /* Auto Abort */
                                            /* 1: Enable Auto Abort */
                                            /* 0: Disable Auto Abort */

#define EN_AUTO_FIS34               1       /* Auto FIS34 */
                                            /* 1: Enable chip Auto FIS34*/
                                            /* 0: Use Auto + Manual FIS34 work around !*/

/* End of Host IF Conditionals */


/********************************************/
/*  Servo and AFE Conditionals              */
/********************************************/
#define DO_HW_SUPPORT_STEP          0       /* 0: Disable HW Stepper, Enable FW Stepper support */
                                            /* 1: Enable HW Stepper, Disable FW Stepper support */

#define SPEED_LOCK_DEBUG            0       /* Allow to lock speed for debug */

#define NEW_SVO_COEF_TEST           0

#if (PCB == 815)
#define FOO_INVERSION               1
#else
#define FOO_INVERSION               0
#endif

#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 830)||(PCB == 1600)||(PCB == 691)||(PCB == 815)||(PCB == 817))
#define TRO_INVERSION               1
#else
#define TRO_INVERSION               0
#endif

#define ACTIVE_SLED_FOR_FINE_JUMP   1       /* 0: sled control based on SLO during fine jump */
                                            /* 1: sled jump based on profile during fine jump */

#define FocusLockAid_Enable         1       /* 0: Focus capture without velocity loop aid */
                                            /* 1: Focus capture with velocity loop aid (improves BD focus capture) */

#define NO_TRAY_OUT_MOVE            0       /* 0: normal tray out operation */
                                            /* 1: will only stop the disc without doing tray out movement after receiving tray out command */

#define DFE_PLL_RESET_IN_dvdGetCurrentPSN     1

/* End of Servo/AFE Conditionals */


/********************************************/
/*          DFE Conditionals                */
/********************************************/
#define HARDCODED_ADC_PLL           0       /* 1 = hardcoded  settings for adc-pll (BD4x) */
                                            /* 0 = calculated settings for adc-pll        */

#define DOUBLED_ADC_CLK             0       /* 1 = calculated settings for adc-pll using speed x 2 (except BD1x) */
                                            /* 0 = calculated settings for adc-pll without adaptation*/

#if (PCB == 815)
#define HIGH_ADC_CLK_BD             1       /* 1 = ADC PLL CLK is 150 MHz for BD1x */
#else
#define HIGH_ADC_CLK_BD             0       /* 0 = ADC PLL CLK is 48 MHz for BD1x */
#endif

#if ((PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
#define MAVHPF_FOR_BDROM_FINGER_PRINT 1     /* 1= set MAHPFSOM(B0002070.18~16)=0x06 & IIRHPFBW(B0002070.7~0)=0x00
                                              to deal with BDROM SL/DL ABEX fingerprint disc                 */
                                            /* 0= use normal setting */
#else
#define MAVHPF_FOR_BDROM_FINGER_PRINT 0  //sun690 A0 must use this setting
#endif

#define DFE_DEFECT_DETECTOR_ENABLE  1       /* 1 = Enable DFE defect detector */
                                            /* 0 = disable DFE defect detector */

#define USE_BS_DEFECT_FOR_STARTUP   1       /* 1 = force to use BS defect when start up */
                                            /* 0 = do not force to use BS defect when start up */

#define BS_SOURCE_DEFECT_USE_TRUE_HOLD  1   /* 1 = force BS source DEFECT to use true hold function(hold DC filter output) */
                                            /* 0 = force BS source DEFECT to use DC filter (not hold DC filter output) */

#define TRACKOFF_SEEK_USE_BS_DEFECT 0       /* 1 = force BS source DEFECT to use when track off or seek */
                                            /* 0 = not force BS source DEFECT to use when track off or seek */

/*It need to tuning blank detect function before using this function*/
#if(CHIP_REV > 0xB1)//B2 onward function that hold TE to vref when blank is detected
#define HOLD_TE_BLANK               1       /* 1 = Hold TE to vref when blank is detected */
                                            /* 0 = not Hold TE to vref when blank is detected */
#else
#define HOLD_TE_BLANK               0       /* 1 = Hold TE to vref when blank is detected */
                                            /* 0 = not Hold TE to vref when blank is detected */
#endif

#define DETECT_DVD_TRACK_PITCH      1       /* 1 = enable detect DVD track pitch in state_rough_chk() */
                                            /* 0 = disable detect DVD track pitch in state_rough_chk() */

#define DETECT_DVD_CLV_TRACK_PITCH  1       /* 1 = enable detect DVD linear velocity and track pitch during startup  */
                                            /* 0 = disable detect DVD linear velocity and track pitch during startup */

/* End of Servo/AFE Conditionals */


/*****************************************************/
/*                Frequency Definitions              */
/*****************************************************/

/*
 * The MODE_LIMITATION is defined as follows:
 * 0: timing at 600ns or PIO mode 0
 * 1: timing at 383ns or PIO mode 1
 * 2: timing at 240ns or PIO mode 2
 * 3: timing at 180ns or PIO mode 3, or Multi Word DMA mode 0
 * 4: timing at 150ns or Multi Word DMA mode 1
 * 5: timing at 120ns or PIO mode 4, or Multi Word DMA mode 2
 *
 * The maximum number of blocks transfer to host is equal to HW_MAX_MULTB.
 * If the current mode (DMA or PIO) is greater than the value of
 * MODE_LIMITATION, then the number of blocks transfer will be equal to
 * HW_MAX_MULTI_COOKED or HW_MAX_MULTI_RAW.
 */

#define HW_MAX_MULTB_RAW            0x1f    /* MAX Multi-Block Transfer in RAW Mode */
#define WR_MAX_MULTB_RAW            0x1B    /* MAX Multi-Block Transfer in RAW Mode */
#define HW_MAX_MULTB_COOKED         0x1f    /* MAX Multi-Block Transfer in COOKED Mode */
#define HW_MAX_MULTB                0x1f    /* MAX Multi-Block Transfer */
#define DATA_RATE                   0x0C60  /* MAX Data Transfer Rate */
#define MAX_PIO                     0x04    /* Max PIO Mode Supported With IORDY */
#define MAX_SINGLE_DMA              0x02    /* Max Single DMA Mode Supported */
#define MAX_MULTI_DMA               0x02    /* Max Multiple DMA Mode Supported */
#define MODE_LIMITATION             0x08    /* ENTER Support 1Fh BLK Transfer */

/*-----------------------------------------*/
/* ATAPI Identify Drive return information */
/*-----------------------------------------*/
#define ID_WORD50_HI                0x40
#define ID_WORD51_HI                0x00    /* PIO Data Transfer Cycle Timing */
#define ID_WORD52_HI                0x00    /* Obsolete */
#define ID_WORD53_LO                0x06    /* ENTER Field Validity including UDMA */
#define ID_WORD62_HI                0x00    /* SATA doesn't need to set this */
#define ID_WORD62_LO                0x00
#define ID_WORD63_HI                0x00    /* Multi Word DMA Transfer */
#define ID_WORD63_LO                0x07
#define ID_WORD64_LO                0x03    /* Enhanced PIO Mode */

#define CycleTM                        1    /* 1: PIO4/DMA2 , 0:PIO3/DMA1 */
#if CycleTM
#define ID_WORD65_HI                0x00    /* Minimum Multi_word DMA Transfer Cycle Time per Word */
#define ID_WORD65_LO                0x78
#define ID_WORD66_HI                0x00    /* Manufacturer's Recommended Multi_word DMA Transfer Cycle Time */
#define ID_WORD66_LO                0x78
#define ID_WORD67_HI                0x00    /* Minimum PIO Transfer Cycle Time Without Flow Control */
#define ID_WORD67_LO                0x78
#define ID_WORD68_HI                0x00    /* Minimum PIO Transfer Cycle Time with IORDY Flow Control */
#define ID_WORD68_LO                0x78
#else /* CycleTM */
#define ID_WORD65_HI                0x00    /* Minimum Multi_word DMA Transfer Cycle Time per Word */
#define ID_WORD65_LO                0xB4
#define ID_WORD66_HI                0x00    /* Manufacturer's Recommended Multi_word DMA Transfer Cycle Time */
#define ID_WORD66_LO                0xB4
#define ID_WORD67_HI                0x00    /* Minimum PIO Transfer Cycle Time Without Flow Control */
#define ID_WORD67_LO                0xB4
#define ID_WORD68_HI                0x00    /* Minimum PIO Transfer Cycle Time with IORDY Flow Control */
#define ID_WORD68_LO                0xB4
#endif /* CycleTM */

//Separate SATA Features with compiler switches
/* The SATA Feature settings below will depend on vendor's unique requirement */
// WORD 76 - 78
#if (HIPM_ENABLED == 1)
#define SUPPORT_HIPM                (bit9 >> 8)
#else
#define SUPPORT_HIPM                0
#endif

#define ID_WORD76_HI                (SUPPORT_HIPM)

#define ID_WORD76_LO                0x02    /* Support SATA Gen1 signaling speed*/

#if (SSP_ENABLED == 1)
#define SUPPORT_SSP                 bit6
#else
#define SUPPORT_SSP                 0
#endif

#if (SATA_AN_ENABLED == 1)
#define SUPPORT_SATA_AN             bit5
#else
#define SUPPORT_SATA_AN             0
#endif
//Modify to correct value

#if (DIPM_ENABLED == 1)
#define SUPPORT_DIPM                bit3
#else
#define SUPPORT_DIPM                0
#endif

#define ID_WORD78_HI                0x00    /* Reserved */

#define ID_WORD78_LO                (SUPPORT_SSP|SUPPORT_SATA_AN|SUPPORT_DIPM)
/* The SATA Feature settings above will depend on vendor's unique requirement */

#define ID_WORD79_HI                0x00    /* Reserved*/
#define ID_WORD79_LO                0x40    /* SSP enable, DIPM disabled by default */

#define ID_WORD80_HI                0x01    /* Major version number */
//IDVER329 testing fail
#if(IDVER_SUPPORTED == 1)  //For HP IDVER test tool
#define ID_WORD80_LO                0xF8    /* Supproted ATA/ATAPI-7, Bit3 = 1, ATA-3 min for PnP (PDIAG/DASP) */
#else
#define ID_WORD80_LO                0xF0    /* Supproted ATA/ATAPI-7 */
#endif

#define ID_WORD81_HI                0x00    /* Major version number */
#define ID_WORD81_LO                0x1A    /* Supproted ATA/ATAPI-7 1532D Rev1 */
#define ID_WORD82_HI                0x42    /* Command set supproted */
#define ID_WORD82_LO                0x10
//Modify to correct value
#if (FLUSH_CACHE_SUPPORTED == 1)
#define SUPPORT_FLUSH_CACHE        (bit12 >> 8)
#else
#define SUPPORT_FLUSH_CACHE         0
#endif

#if (AAM_SUPPORTED == 1)
#define SUPPORT_AAM                 (bit9 >> 8)
#else
#define SUPPORT_AAM                 0
#endif

//RMSN = Removable Media Status Notification feature set
#if (RMSN_SUPPORTED == 1)
#define SUPPORT_RMSN                bit4
#else
#define SUPPORT_RMSN                0
#endif

#define ID_WORD83_HI                ((bit14 >> 8)|SUPPORT_FLUSH_CACHE|SUPPORT_AAM) /* bit14 shall be set to one */
#define ID_WORD83_LO                SUPPORT_RMSN

#define ID_WORD84_HI                0x40    /* Command set/feature supproted extension */
#define ID_WORD84_LO                0x00
#define ID_WORD85_HI                0x42    /* Command set/feature enabled */
#define ID_WORD85_LO                0x10
#define ID_WORD86_HI                SUPPORT_FLUSH_CACHE
#define ID_WORD86_LO                0x00
#define ID_WORD87_HI                0x40    /* Command set/feature default */
#define ID_WORD87_LO                0x00
#define ID_WORD88_HI                0x20    /* ENTER Ultra DMA active mode */
#define ID_WORD88_LO                0x3F    /* ENTER Modes 5 and below are supported */
//IDVER329 testing fail
#if (AAM_SUPPORTED == 1)
#define ID_WORD94_HI                0x80    /* Current AAM value */
#else
#define ID_WORD94_HI                0x00
#endif

#define ID_WORD94_LO                0x00    /* (Vender specific) */

#if(IDVER_SUPPORTED == 1)//For HP IDVER test tool
#define ID_WORD222_LO               0x1F  /*Bit0=ATA-8/Bit1=SATA 1.0/Bit2=SATA II/Bit3=SATA 2.5/Bit4=SATA 2.6*/
#else
#define ID_WORD222_LO               0x00
#endif

#define ID_WORD255_LO               0xA5


/********************************************/
/*  C1/C2 Threshold Defines                 */
/********************************************/

/*------------------------------------------------------------------------*/
/* Uncorrectable C2 error threshold for CDDA during 98 EFM frames */

#define C2_THRESHOLD_CDDA1          1       /* level 1 */
#define C2_THRESHOLD_CDDA3          50      /* level 2 */
/*------------------------------------------------------------------------*/

/***********************************************************************************/
/*                            TEMPORARY CONDITIONALS                               */
/* All the conditionals below are Temporary!  They are used to enable/disable new  */
/* features or functions so that the baseline code will not be broken.  Once the   */
/* feature/function is enabled and verified, then the switch will be removed.      */
/***********************************************************************************/

#define HF_BASED_CUTOFF             1       /* */

#define RESYNC_MESSAGES             0

/*** End of Temporary Conditionals ***/


/***********************************************************************************/
/*                              DEBUG CONDITIONALS                                 */
/* All the conditionals below are used for Debug Only                              */
/***********************************************************************************/

/********************************************/
/*  Diagnostics Conditionals                */
/********************************************/
#define E_DIAGS                     1       /* Engineering Diagnostics Commands */
                                            /* 1 - Enable Engineering Diagnostics commands */
                                            /* 0 - Disable Engineering Diagnostics commands */

#define P_DIAGS                     0       /* POS Engineering Diagnostics Commands */
                                            /* 1 - Enable  POS Engineering Diagnostics commands */
                                            /* 0 - Disable POS Engineering Diagnostics commands */

#define CD_MRW_DIAG                 0       /* 1: Enable CD MRW diagnostics tools if E_DIAGS is also enabled */
                                            /* 0: Disable CD MRW diagnsotics tools */

#if (E_DIAGS==1)
//Only allowed when E_DIAGS =1
#define TUNING_ENVIRONMENT_ON       0       /* 1 Some recoveries and register writes disabled */
                                            /* 0 Normal system useage */
#endif


/********************************************/
/*        Debug Printing Controls           */
/********************************************/
/*--------------------------*/
/* Debug MESSAGE Out Method */
/*--------------------------*/
#define MSG_DISABLE                 0       /* Message out is disabled */
#define MSG_UART                    1       /* Message out transmitted via RS232 port */
#define MSG_MICE                    2       /* Message out transmitted via ARM ICE Console */
#define MSG_USBPORT                 3       /* Message out transmitted via the USB port */

/*------------------------*/
/*    Debug Board Type    */
/*------------------------*/
#define SUNEXT_DEBUG_BOARD          1
#define VENDOR1_DEBUG_BOARD         2
#if BIOS_MODEL == 0
#define DEBUG_BOARD_TYPE            SUNEXT_DEBUG_BOARD
#elif BIOS_MODEL == 2
#define DEBUG_BOARD_TYPE            VENDOR1_DEBUG_BOARD
#endif

/*------------------------*/
/* Debug MESSAGE Out Mode */
/*------------------------*/
// WARNING: If you disable msg_out_mode, then new code CAN NOT be flashed using the UART and application.
#define MSG_OUT_MODE                MSG_USBPORT /* Select Message out method */

#if (MSG_OUT_MODE == MSG_USBPORT)
#define SSI_INTERFACE               1       /* SSI interface used for USB messaging */
#define UART_INTERFACE              2       /* UART interface used for USB messaging */

    #if (DEBUG_BOARD_TYPE == SUNEXT_DEBUG_BOARD)
    #define DEBUG_PORT_OUTPUT_MODE  SSI_INTERFACE    /* Select Port for USB Messaging */
    #else
            #if (CHIP_REV < 0xC0)
    #define DEBUG_PORT_OUTPUT_MODE  UART_INTERFACE   /* Select Port for USB Messaging */
            #else
#define DEBUG_PORT_OUTPUT_MODE  SSI_INTERFACE  /* Select Port for USB Messaging */
            #endif
    #endif
#endif /* MSG_OUT_MODE==MSG_USBPORT */

/*-------------------------------*/
/*  ASSERT swithces              */
/*-------------------------------*/
#define MSG_ASSERT_NONE             0       /* No ASSERT functionality */
#define MSG_ASSERT_HALT             1       /* Assert message and halt */
#define MSG_ASSERT_CONTINUE         2       /* Assert message and continue (what) */
#define MSG_ASSERT_RESET            3       /* Assert message and reset drive */

#define ASSERT_MODE                 MSG_ASSERT_CONTINUE

/*-------------------------------*/
/*  Enable Debug Print Switches  */
/*-------------------------------*/
#define PRN_DEBUG                   0       /* Enable debug printing */
#define PRN_MRW                     0       /* Mt Rainier Debug Printing */
#define PRN_DVDHIF                  0       /* Print DVD specific messaging */
#define PRN_W_O                     0       //
#define PRN_HIF_TO_SIF              0       /* Prints opcodes/states of commands passed between HIF and Servo (SIF). */
#define PRN_HOST                    0       /* Prints opcodes/states of commands passed between Host and platform.   */
#define PRN_DVD_DATABASE            0       // Print DVD Database.
#define PRN_DVDFORMAT               0       /* Print DVD format messages */
#define PRN_WBL                     0       /* Prints Wobble Block messages */
#define PRN_SVO_STATE               0       /* Prints Servo State messages */
#define PRN_PLR_REC                 0       /* Prints Player Record module messages */
#define PRN_SPEED_CALC              0       /* Prints speed calc messages */

#define MESSAGE_AFE_CALI            0       /* AFE Calibration display */
#define MESSAGE_AFE_CALI_TOP        0       /* AFE Calibration display, Top level program name only */
#define MESSAGE_SPEED               0       /* Motor Speed display */
#define MESSAGE_ERR                 0       /* Error display */
#define MESSAGE_DISC_KIND           0       /* Print Disc Kind */
#define MESSAGE_CALI_RESULT         0       /* Print all calibrations' results */
#define PRINT_DVD_DATABASE          0       // Print DVD Database.

/*------------------------*/
/* UART Control Mode */
/*------------------------*/
#if (((MSG_OUT_MODE == MSG_USBPORT)&&(DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE))||(MSG_OUT_MODE == MSG_UART))
#define ENABLE_UART_TXD             1       /* Enable UART TxD functions (transmit). Used to Print messages on serial port */
#else
#define ENABLE_UART_TXD             0       /* Enable UART TxD functions (transmit). Used to Print messages on serial port */
#endif /* MSG_OUT_MODE=MSG_USBPORT and DEBUG_PORT_OUTPUT_MODE=UART_INTERFACE */

#if (MSG_OUT_MODE==MSG_USBPORT)
#define ENABLE_UART_RXD             1       /* Enable UART RxD functions (receive).  Used to Receive messages on serial port */
#define REGISTER_MONITOR            0       /* Enable sunWIDET registor monitor function */
#else
#define ENABLE_UART_RXD             0       /* Enable UART RxD functions (receive).  Used to Receive messages on serial port */
#define REGISTER_MONITOR            0       /* Enable sunWIDET registor monitor function */
#endif /* MSG_OUT_MODE==MSG_USBPORT */

//--------------------------------------------------------;
// UART Baud Rate = ssi clk / (16 * BAUD_DIV)             ;
//                                                        ;
//--------------------------------------------------------;
#define UART_BAUD_RATE              345600  /* Define in Characters/second */

#if UART_BAUD_RATE == 57600
    #if SYSCLK == 125
    #define BAUD_DIV                68      // -0.27%
    #elif SYSCLK == 122
    #define BAUD_DIV                66      // +0.29%
    #elif SYSCLK == 110
    #define BAUD_DIV                59      // -0.54%
    #elif SYSCLK == 100
    #define BAUD_DIV                54      // +0.47%
    #elif SYSCLK == 90
    #define BAUD_DIV                49      // -0.53%
    #else
    #error Clock Rate not supported
    #endif
#elif UART_BAUD_RATE == 115200
    #if SYSCLK == 125
    #define BAUD_DIV                34      // -0.27%
    #elif SYSCLK == 122
    #define BAUD_DIV                33      // +0.29%
    #elif SYSCLK == 110
    #define BAUD_DIV                29      // -0.54%
    #elif SYSCLK == 100
    #define BAUD_DIV                27      // +0.47%
    #elif SYSCLK == 90
    #define BAUD_DIV                25      // +1.73%
    #else
    #error Clock Rate not supported
    #endif
#elif UART_BAUD_RATE == 345600
    #if SYSCLK == 125
    #define BAUD_DIV                11      // +2.75%
    #elif SYSCLK == 122
    #define BAUD_DIV                11      // +0.18%
    #elif SYSCLK == 110
    #define BAUD_DIV                10      // -0.54%
    #elif SYSCLK == 100
    #define BAUD_DIV                9       // +0.47%
    #elif SYSCLK == 90
    #define BAUD_DIV                8       // +1.73%
    #else
    #error Clock Rate not supported
    #endif
#else
#error Invalid Buad Rate Selected
#endif


/********************************************/
/*  Serial Flash  Conditionals              */
/********************************************/
#define EN_DOSPI_SUPPORT            0       /*0: Dual Out Put SPI flash not support*/
                                            /*1: Dual Out Put SPI flash support*/


/********************************************/
/*  Test Pinout for Debug Conditionals      */
/********************************************/
#define DEBUG_MIO_SETTING           0       //* Debug Port MIO control
                                            // 1-->Use Debug MIO Setting, 0-->No Use

#define DEBUG16_OUTPUT_FROM_MIO     1       /* 1: debug16 monitoring signal output from MIO0~3*/
                                            /* 0: debug16 monitoring signal output from debug64 pin*/


/********************************************/
/*  Emulation Debug level Conditionals      */
/********************************************/
#define WITHOUT_SERVO               0       /* 1: Enable special without SERVO test code
                                                  You may need make change in ReadCommon() in order to read CDDA data
                                                  You may need make change in Rw_ecc_mode() in order to read CD-ROM data other than YBM1
                                               0: Disable special without SERVO test code */

#define DEBUG_SVO_BUSY_EJECT        0       // Temporary conditional for servo debugging. This conditional will
                                            // allow an eject to be processed while the player is waiting for the servo
                                            // sub system to respond back to a player command. During debugging the servo
                                            // may set large time out values and take longer to process certain commands.
                                            // This conditional will allow a method to interrupt these long debug processes
                                            // or large timeout values.
                                            //
                                            // 1 = Allow eject to occur if servo is taking too long to process command
                                            // 0 = Normal mode

#define SERVO_SEEK_TEST             0       /* 0: disable SERVO stand alone seek test */
                                            /* 1: enable SERVO stand alone seek test */

#define FW_JITTER_CALC              1       // 1 enables firmware to calculate jitter

#define SERVICE_MODE                0       /* 1: enable servo loop uncontrolled (no recoveries) */

#define CMD_SVO_CMD_READ_GPADC      0       // 1 enables servo service command to measure signals via general

#define CMD_DRY_READ                0       // 1 enables firmware to allow PLAYER/SERVO TRANSPARENT COMMAND "svoCmdDryRead"

#define AFE_NORMALIZER              1       // 1 use svo dynamic gain hardware
                                            // 0 svo dynamic gain hardware not used

#define TEST_AVERAGING_SERVO        0       // 1 debugging averaging servo only
                                            // 0 disable debugging averaging servo

#define USE_REALTIME_JITTER         0       // 1 use real time jitter measurement
                                            // 0 do not use real time jitter measurement

#define FORCE_DISK_KIND             0       /* 1: Force disc kind to seclect disc kind,use correct laser,AFE gain and do related calibration */
                                            /* 0: Don't force disc kind */

// Defines for use with DISC_IDENTIFY Switch
#define STM_UNIVERSAL               2       /*  2: universal (STM) */
#define STM_BD_TRIPLE_READER        3       /*  3: triple reader BD (STM) */
#define STM_HD_TRIPLE_READER        4       /*  4: triple reader HD (STM)*/
#define STM_SUPER_MULTI             5       /*  5: New disc id (STM) */

#if (PCB == 691)
#define DISC_IDENTIFY               STM_HD_TRIPLE_READER
#else
#define DISC_IDENTIFY               STM_BD_TRIPLE_READER
#endif // PCB

/* End of Test/Debug Configuration */

/********************************************/
/* For DVD+R                                */
/********************************************/
#define GET_LAST_TRACK_INFO_BASED_ON_RAI    0

/* End of DVD+R */


#define WOBBLE_ID                   0       /* 0:Get Data ID, 1:Get Wobble ID, for read PIPO */

#define RWMARKBLKMODE_IN_FLASH      0       /* Rw_mark_blk_mode move into flash */

#define GETPACKETCOMMAND_IN_FLASH   0       /* GetPacketCommand move into flash */

#define LPP_AUTO_LEARN_TEST         0       /* 1: Use new LPP Threshold Auto Learn */
                                            /* 0: Use current auto learn */

/*********************************************/
/*  DVD Module test parameters               */
/*********************************************/
#define MODULE_TEST                 0       /* 0: disable - Normal operation, module test code is removed */
                                            /* 1: enable Chip Module test. Basic functional tests to find ATE Test Escapes */

#define KFGTG_VAL_WI_BOUNDARY		0      // 0: Close FE/TE amplitute /DSP boundary check 
                                           // 1: OPEN  FE/TE amplitute /DSP boundary check

/*
integration method example: (total 5 tracks)
    FW restored tracks info only: 1, 2, 4, 5
    after integration: valid track = 1, 2 (start at old 2, end before old track 4), 3 (= old 4th trk), 4 (= old 5th trk)
    i.e. new track 2 length = old 2nd track + old 3rd track length
*/
#define MERGE_TOC                   0       // 0: original read TOC flow
                                            // 1: merge TOC at timeout, if not all tracks can be restored -> integrate missing track
                                            // 2: merge TOC at max retries (ie, 5 timeout), if not all tracks can be restored -> integrate missing track

#define OPEN_SESSION                1       // 0:original setting, READER_ONLY Drive can't read CD/DVD's open session
                                            // 1:support read CD/DVD's open session used for READER_ONLY Drive


/*********************************************/
/*  Playbility Switches                      */
/*********************************************/
#if (PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 815)||(PCB == 817)
#define HORROR_DISC_IMPROVEMENT     1       /* 1: Modify some parameters for Horror disc playability */
                                            /* 0: Original setting*/

#define DROOP_RATE_DECREASE         0       /* 1: decrease short/long term P/H droop rate*/
                                            /* 0: default setting */
#else
#define HORROR_DISC_IMPROVEMENT     0       /* 1: Modify some parameters for Horror disc playability */
                                            /* 0: Original setting*/

#define DROOP_RATE_DECREASE         0       /* 1: decrease short/long term P/H droop rate*/
                                            /* 0: default setting */
#endif

#define ENABLE_LEARN_DEFECT         1       /* 0: Learning defect feature disabled */
                                            /* 1: Learning defect feature enabled */


/*********************************************/
/*  Servo Tuning Switches                    */
/*********************************************/
/****** Do Not modify the following #defines ******/
#define BODE_NONE                   0       /* Normal: Bode measurement disabled */
#define FOCUS_BODE                  1       /* Focus Bode: Focus Bode measurement enabled */
#define TRACKING_BODE               2       /* Tracking Bode: Tracking Bode measurement enabled */
/****** Do Not modify the following #defines ******/

//Enable/disable Bode mesurement using internal summation (bug#4262)
#define SERVO_BODE                  BODE_NONE
#define PLANT_MODELING_SW           0       // for Servo plant modeling

//cycle do AGC and preset different init gain to verify AGC performance.
#define AGC_VERIFY_TEST             0       // 0:normal, 1~255:debug

//cycle do DVDR(W) plus/minus detection to see its stability.
#define PLUS_MINUS_DET_TEST         0       // 0:normal, 1~65535:debug

//measure substrate FE peak/bottom
#define MEAS_SUBSTRATE_P2P          0       // measure s-curve of substrate to determine FE_SMSTEP_TH.

//for seek parameters tuning
#define SEEK_ADJ                    0

//LONG variables for debug
#define SERVO_DEBUG_VARIABLE        0

//output full range of FOO and TRO, use it to make sure it also the full range of drive IC output
#define DEBUG_OUTPUT_FULL_RANGE_FOO_TRO 0

#define AFE_REG_DUMP                0       /* 1: Display servo MSG calibration results, mostly from AFE registers  */
                                            /*    This compile switch should never be default = ON. It is only for debug and should never be enabled on ship code */

#define TEMP_FINISH_TRON_AFTER_TRKSVO_ON 0  // 0: use normal flow in TRON_TBL
                                            // 1: finsih just after trksvo_on

/********************************************/
/*  Reader Switches                         */
/********************************************/

#define RESET_PLL_FOR_HUGEPIC1      1       /*currently only implement in CD*/


/********************************************/
/*  Read Speed Control Switch                                      */
/********************************************/
//for Read Speed Control v0.1. It adjust the speed by decode status.
//Please refer to the document of Read Speed Control v0.1 for details.
#define READ_SPEED_CONTROL_VERSION_DOT_ONE  0x01    //This value is only a Label.

//for Read Speed Control v1.0. It adjust the speed by decode and buffer status.
//Please refer to the document of Read Speed Control v1.0 for details.
#define READ_SPEED_CONTROL_VERSION_ONE      0x10    //This value is only a Label.


#define BUILD_IN_AACS_FOLDER        0

#define BDRE_BYPASS_LEADIN          0       /* Bypass lead-in area data read for BD-RE disc startup */

#define ENABLE_DFE_ICE_DETECT       1

#if (ENABLE_DFE_ICE_DETECT == 1)
#define ENABLE_DISCARD_ICE          1       /* Discard icded clusters decode */
#else
#define ENABLE_DISCARD_ICE          0
#endif

#define NEW_PIC_HOME_FLOW           1       /* 1: do homing during start command */
                                            /* 0: do homing during tray in command */

#define RP_AUTO_ADJUST              0       // 1 : Enable autoadjust RP2 offset during rough jump and after layer jump
                                            // 0 : Disable autoadjust RP2 offset during rough jump and after layer jump

#define NEW_BDDL_START_FLOW         1       /* 1: measure BD L1 FEpp with colli at L1 */
                                            /* 0: measure BD L0 & L1 FEpp with colli at L0 */

 //Remove "Read DVD-RW CPRM video Format3 RMD" form dvdGetBorderInfo() to ReadDVDStructureCmd Format 0x0E
 //for Factory test.
#define R_DVDMRW_BOARDOUT_IN_STARTUP 0        /* 0: read DVD-RW board out in ReadDVDStructure Cmd Format 0x0E*/
                                                                                            /* 1: read DVD-RW board out in start up flow */



//add readjust layer flow for BD layer information mismatch between servo and hif.
#define BD_LAYER_VERIFY             1       //1: readjust for Servo and HIF layer mismatch.

//judge BD BCA valid or not.
#define JudgeBDBCAValid             1

//No record data in R/RW-Physical format information zone, Get data form board-out.
#define DVDMR_NoDataInRPhyForInf    1



#define Read_RETRY_ERROR_INDEX_RESEARCH 1  /* 1: Enable research error index of read retry*/
                                           /* 0: Disable research error index of read retry*/

/********************************************/
/*  C0/C1 feature                                                       */
/********************************************/

#define NEW_FINE_SEEK_FEATURE    1 /* 1: Enable C0's NEW_FINE_SEEK_FEATURE*/
                                                        /* 0: Disable C0's NEW_FINE_SEEK_FEATURE*/

#define NEW_LAYER_JUMP_FEATURE    1 /* 1: Enable C0's NEW_LAYER_JUMP_FEATURE*/
                                                        /* 0: Disable C0's NEW_LAYER_JUMP_FEATURE*/

#define NEW_BS_DEFECT_FEATURE    1 /* 1: Enable C0's NEW_BS_DEFECT_FEATURE*/
                                                        /* 0: Disable C0's NEW_BS_DEFECT_FEATURE*/

#define NEW_RFPOS_CALIB             1       // 1: make sure the RF ADC range is linear
                                            // 0: bring RFAGC reference level to 0mV

#define NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO             1       // 1: use RFPOS to compensate offset from RFLPF and bring RFAGC reference level to 0mV
                                                                                                    // 0: make sure the RF ADC range is linear

#define DVDRAM_USE_OLD_RFPOS_CALIB            1       /* 1: use RFPOS to compensate offset in RFLPF for DVDRAM *///20101022_02R7 replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs
                                                      /* 0: use NEW_RFPOS_CALIB to prevent RF distortion for DVDRAM */
#endif /* __VU_CFG_H__ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
