/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   GLOBALS.H
*
* DESCRIPTION
*   This file contains all global definitions and typedefs that are to
*   be used by the creators of a CD-R/RW application.
*
*   $Revision: 104 $
*   $Date: 11/01/11 11:32a $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globtype.h"
/* public vs private */
#define public
#define private static

#define PASS    0
#define FAIL    1

#define CLEAR   0
#define SET     1

#define NON 2
#define ON  1
#define OFF 0

//#if (CD_MRW_OEM == 1 || DVD_MRW_OEM == 1)
#if 1

/* MRW Support */
#if FW_JITTER_CALC == 1
#define HIF_STACK_SIZE      (800 + 100)
#else
#define HIF_STACK_SIZE      (740 + 100)
#endif
#define WRP_STACK_SIZE      700
//Reader Only project doesn't use BGP task, the relative code is
//moved to Timer task, so we need to enlarge its size.
//elvis, 2009/01/15
#define TIMER_STACK_SIZE    (216 + 200)
#define PLAYER_STACK_SIZE   (384 + 100 + 100)
#define SERVO_STACK_SIZE    768
#define BGP_STACK_SIZE      560

#else

#define HIF_STACK_SIZE      (544+64)
#define WRP_STACK_SIZE      384
#define TIMER_STACK_SIZE    176
#define PLAYER_STACK_SIZE   384
#define SERVO_STACK_SIZE    288
#define BGP_STACK_SIZE      512

#endif  /* CD_MRW_OEM == 1 || DVD_MRW_OEM == 1 */

/*
** LED Specific Control Values **
*/
#if DIRECT_DRIVING_LED
#define LED_ON              1       /* LED1: Turn LED On */
#define LED_OFF             0       /* LED1: Turn LED Off */
#else
#define LED_ON              0       /* LED1: Turn LED On */
#define LED_OFF             1       /* LED1: Turn LED Off */
#endif
#define LED_FLASH           2       /* LED1: Toggle LED On/Off */
#define LED_PLAY_FLASH      3       /* LED1: During NX Playback Toggle LED On/Off */
#define LED_AXferOn         4       /* LED2: Turn On LED during Audio Host Transfer */
#define LED_ON1S            5       /* LED2: Error - Turn LED On for 1 sec */
#define LED_FLASH3S         6       /* LED1: Error - Toggle LED On/Off for 3 sec */
#define LED_INITIAL_FLASH   7       /* LED1: During Startup Toggle LED On/Off */
#define LED_REAL_W_FLASH    8       /* LED1: During Normal Write Toggle LED On/Off */
#define LED_TEST_W_FLASH    9       /* LED1: During Test Write Toggle LED On/Off */
#define LED_BLINK1      LED_FLASH   /* LED1: During MRW Read Toggle LED On/Off */
#define LED_BLINK2      LED_FLASH   /* LED1: During MRW BGFormat Toggle LED On/Off */
#define LED_FLASH1S         10      /* LED1: Error - Toggle LED On/Off for 1 sec */

#define BurnIn_LED_OFF  LED_OFF
#define BurnIn_LED_ON   LED_ON
#define BurnIn_Tray_out_error       2
#define BurnIn_Tray_in_error        3
#define BurnIn_No_disc              4
#define BurnIn_Start_up_error       5
#define BurnIn_Set_speed_error      6
#define BurnIn_Seek_error           7
#define BurnIn_DRAM_error           8

/*** LED Flash Rates ***/
/*                               Toggle Rate/Timer2 Count Period */
/*                                      (mS)/(mS) */
#define ReadyLED_RATE                    (60/20)    /* 60 mS On: 60 mS Off */
#define WriteLED_RATE                    (60/20)    /* 60 mS On: 60 mS Off */
#define ErrorLED_RATE                  (1000/20)    /* 1 Sec On: 1 Sec Off */
#define PlayLED_RATE                    (500/20)    /* 500 mS On: 500 mS Off */
#define AtLED_RATE                      (500/20)    /* 500 mS On: 500 mS Off */
#define Prevent_RATE                   (3000/20)    /* 3 Sec On: 3 Sec Off */
#define Prevent_RATE1S                   (1000/20)    /* 1 Sec On: 1 Sec Off */

#define INITIAL_LED_RATE                (500/20)    /* 500 mS On: 500 mS Off */
#define REAL_WRITE_LED_RATE             (250/20)    /* 250 mS On: 250 mS Off */
#define TEST_WRITE_LED_RATE             (375/20)    /* 375 mS On: 375 mS Off */

/*
 *  Bit initialization w/in for SC registers.
 */
#define bit31      (0x80000000)     /* bit 31 set */
#define bit30      (0x40000000)     /* bit 30 set */
#define bit29      (0x20000000)     /* bit 29 set */
#define bit28      (0x10000000)     /* bit 28 set */
#define bit27      (0x08000000)     /* bit 27 set */
#define bit26      (0x04000000)     /* bit 26 set */
#define bit25      (0x02000000)     /* bit 25 set */
#define bit24      (0x01000000)     /* bit 24 set */
#define bit23      (0x00800000)     /* bit 23 set */
#define bit22      (0x00400000)     /* bit 22 set */
#define bit21      (0x00200000)     /* bit 21 set */
#define bit20      (0x00100000)     /* bit 20 set */
#define bit19      (0x00080000)     /* bit 19 set */
#define bit18      (0x00040000)     /* bit 18 set */
#define bit17      (0x00020000)     /* bit 17 set */
#define bit16      (0x00010000)     /* bit 16 set */
#define bit15      (0x8000)         /* bit 15 set */
#define bit14      (0x4000)         /* bit 14 set */
#define bit13      (0x2000)         /* bit 13 set */
#define bit12      (0x1000)         /* bit 12 set */
#define bit11      (0x0800)         /* bit 11 set */
#define bit10      (0x0400)         /* bit 10 set */
#define bit9       (0x0200)         /* bit  9 set */
#define bit8       (0x0100)         /* bit  8 set */
#define bit7       (0x0080)         /* bit  7 set */
#define bit6       (0x0040)         /* bit  6 set */
#define bit5       (0x0020)         /* bit  5 set */
#define bit4       (0x0010)         /* bit  4 set */
#define bit3       (0x0008)         /* bit  3 set */
#define bit2       (0x0004)         /* bit  2 set */
#define bit1       (0x0002)         /* bit  1 set */
#define bit0       (0x0001)         /* bit  0 set */

#define bits15_8    0x0000ff00L
#define bits7_0     0x000000ffL
#define bits3_0     0x0000000fL

/* other useful equates to follow */
#define ZERO_BYTE   0x00        /* zero BYTE */


typedef enum
{
	Ready,
	Busy,
	Error,
	ProcessReady
} ExecutionStateT;

typedef enum
{
	READY,
	BUSY,
	ERROR,
} EStatus;

typedef USHORT           seq_exit_t;
#define SEQ_EX0         0
#define SEQ_EX1         1
#define SEQ_EX2         2
#define SEQ_EX3         3
#define SEQ_EX4         4
#define SEQ_BUSY        5

/********************************************************
** Disc Speed Defines **
*********************************************************/
typedef unsigned char DiscSpeedT;
/* CD CLV Speed defines */
#define e_CLV       0x00        /* CLV Speed Indicator */
#define eSpStop     0
#define e4XCLV      4           /*  4X CLV Speed */
#define e6XCLV      6           /*  6X CLV Speed */
#define e8XCLV      8           /*  8X CLV Speed */
#define e10XCLV     10          /* 10X CLV Speed */
#define e12XCLV     12          /* 12X CLV Speed */
#define e16XCLV     16          /* 16X CLV Speed */
#define e18XCLV     18          /* 18X CLV Speed */
#define e20XCLV     20          /* 20X CLV Speed */
#define e22XCLV     22          /* 22X CLV Speed */
#define e24XCLV     24          /* 24X CLV Speed */
#define e26XCLV     26          /* 26X CLV Speed */
#define e28XCLV     28          /* 28X CLV Speed */
#define e30XCLV     30          /* 30X CLV Speed */
#define e32XCLV     32          /* 32X CLV Speed */
#define e34XCLV     34          /* 34X CLV Speed */
#define e36XCLV     36          /* 36X CLV Speed */
#define e38XCLV     38          /* 38X CLV Speed */
#define e40XCLV     40          /* 40X CLV Speed */
#define e42XCLV     42          /* 42X CLV Speed */
#define e44XCLV     44          /* 44X CLV Speed */
#define e46XCLV     46          /* 46X CLV Speed */
#define e48XCLV     48          /* 48X CLV Speed */
#define e52XCLV     52          /* 52X CLV Speed */
#define e54XCLV     54          /* 54X CLV Speed */

/* CD CAV Speed defines */
#define e_CAV       0x80        /* CAV Speed Indicator */
#define e4XCAV      (4|e_CAV)   /* 4X Max CAV Speed */
#define e8XCAV      (8|e_CAV)   /* 8X Max CAV Speed */
#define e10XCAV     (10|e_CAV)  /* 10X Max CAV Speed */
#define e12XCAV     (12|e_CAV)  /* 12X Max CAV Speed */
#define e16XCAV     (16|e_CAV)  /* 16X Max CAV Speed */
#define e20XCAV     (20|e_CAV)  /* 20X Max CAV Speed */
#define e24XCAV     (24|e_CAV)  /* 24X Max CAV Speed */
#define e32XCAV     (32|e_CAV)  /* 32X Max CAV Speed */
#define e40XCAV     (40|e_CAV)  /* 40X Max CAV Speed */
#define e48XCAV     (48|e_CAV)  /* 48X Max CAV Speed */
#define e52XCAV     (52|e_CAV)  /* 52X Max CAV Speed */
#define e54XCAV     (54|e_CAV)  /* 54X Max CAV Speed */

/* CD ZONE CLV defines */
#define e_ZONE       0x40                /* Zone CLV Indicator */
#define e_ZCLV       e_ZONE              /* Zone CLV Indicator */
#define e4XZCLV     ( 4|e_ZONE)          /*  4X CLV Speed */
#define e8XZCLV     ( 8|e_ZONE)          /*  8X CLV Speed */
#define e10XZCLV    (10|e_ZONE)          /* 10X CLV Speed */
#define e12XZCLV    (12|e_ZONE)          /* 12X CLV Speed */
#define e16XZCLV    (16|e_ZONE)          /* 16X CLV Speed */
#define e18XZCLV    (18|e_ZONE)          /* 18X CLV Speed */
#define e20XZCLV    (20|e_ZONE)          /* 20X CLV Speed */
#define e22XZCLV    (22|e_ZONE)          /* 22X CLV Speed */
#define e24XZCLV    (24|e_ZONE)          /* 24X CLV Speed */
#define e26XZCLV    (26|e_ZONE)          /* 26X CLV Speed */
#define e28XZCLV    (28|e_ZONE)          /* 28X CLV Speed */
#define e30XZCLV    (30|e_ZONE)          /* 30X CLV Speed */
#define e32XZCLV    (32|e_ZONE)          /* 32X CLV Speed */
#define e34XZCLV    (34|e_ZONE)          /* 34X CLV Speed */
#define e36XZCLV    (36|e_ZONE)          /* 36X CLV Speed */
#define e40XZCLV    (40|e_ZONE)          /* 40X CLV Speed */
#define e44XZCLV    (44|e_ZONE)          /* 44X CLV Speed */
#define e48XZCLV    (48|e_ZONE)          /* 48X CLV Speed */
#define e52XZCLV    (52|e_ZONE)          /* 52X CLV Speed */
#define e54XZCLV    (54|e_ZONE)          /* 54X CLV Speed */

/* CD Partial CAV defines */
#define e_Partial    0x40                  /* Partial CAV Indicator */
#define e_PCAV       (e_Partial|e_CAV)     /* Partial CAV Indicator */
#define e4XPCAV      (4|e_Partial|e_CAV)   /* 4X Max PCAV Speed */
#define e8XPCAV      (8|e_Partial|e_CAV)   /* 8X Max PCAV Speed */
#define e10XPCAV     (10|e_Partial|e_CAV)  /* 10X Max PCAV Speed */
#define e12XPCAV     (12|e_Partial|e_CAV)  /* 12X Max PCAV Speed */
#define e16XPCAV     (16|e_Partial|e_CAV)  /* 16X Max PCAV Speed */
#define e20XPCAV     (20|e_Partial|e_CAV)  /* 20X Max PCAV Speed */
#define e24XPCAV     (24|e_Partial|e_CAV)  /* 24X Max PCAV Speed */
#define e32XPCAV     (32|e_Partial|e_CAV)  /* 32X Max PCAV Speed */
#define e40XPCAV     (40|e_Partial|e_CAV)  /* 40X Max PCAV Speed */
#define e48XPCAV     (48|e_Partial|e_CAV)  /* 48X Max PCAV Speed */
#define e52XPCAV     (52|e_Partial|e_CAV)  /* 52X Max PCAV Speed */
#define e54XPCAV     (54|e_Partial|e_CAV)  /* 54X Max PCAV Speed */

/* DVD CLV Speed defines */
#define e1XDCLV     1          /*   1X  DVD CLV Speed */
#define e2XDCLV     2          /*   2X  DVD CLV Speed */
#define e3XDCLV     3          /* 2.4X  DVD CLV Speed */
#define e4XDCLV     4          /*   4X  DVD CLV Speed */
#define e5XDCLV     5          /*   5X  DVD CLV Speed */
#define e6XDCLV     6          /*   6X  DVD CLV Speed */
#define e7XDCLV     7          /*   7X  DVD CLV Speed */
#define e8XDCLV     8          /*   8X  DVD CLV Speed */
#define e10XDCLV    10         /*  10X  DVD CLV Speed */
#define e12XDCLV    12         /*  12X  DVD CLV Speed */
#define e14XDCLV    14         /*  14X  DVD CLV Speed */
#define e16XDCLV    16         /*  16X  DVD CLV Speed */
#define e18XDCLV    18         /*  18X  DVD CLV Speed */
#define e20XDCLV    20         /*  20X  DVD CLV Speed */

/* DVD Zone CLV Speeds */
#define e1XDZCLV     ( 1|e_ZONE)    /*   1X DVD CLV Speed */
#define e2XDZCLV     ( 2|e_ZONE)    /*   2X DVD CLV Speed */
#define e3XDZCLV     ( 3|e_ZONE)    /* 2.4X DVD CLV Speed */
#define e4XDZCLV     ( 4|e_ZONE)    /*   4X DVD CLV Speed */
#define e5XDZCLV     ( 5|e_ZONE)    /*   5X DVD CLV Speed */
#define e6XDZCLV     ( 6|e_ZONE)    /*   6X DVD CLV Speed */
#define e8XDZCLV     ( 8|e_ZONE)    /*   8X DVD CLV Speed */
#define e12XDZCLV    (12|e_ZONE)    /*  12X DVD CLV Speed */
#define e16XDZCLV    (16|e_ZONE)    /*  16X DVD CLV Speed */
#define e18XDZCLV    (18|e_ZONE)    /*  18X DVD CLV Speed */
#define e20XDZCLV    (20|e_ZONE)    /*  20X DVD CLV Speed */

/* DVD CAV Speeds */
#define e1XDCAV     ( 1|e_CAV)    /* 1X DVD CAV Speed */
#define e2XDCAV     ( 2|e_CAV)    /* 2X DVD CAV Speed */
#define e3XDCAV     ( 3|e_CAV)    /* 3X DVD CAV Speed */
#define e4XDCAV     ( 4|e_CAV)    /* 4X DVD CAV Speed */
#define e5XDCAV     ( 5|e_CAV)    /* 5X DVD CAV Speed */
#define e6XDCAV     ( 6|e_CAV)    /* 6X DVD CAV Speed */
#define e7XDCAV     ( 7|e_CAV)    /* 7X DVD CAV Speed */
#define e8XDCAV     ( 8|e_CAV)    /* 8X DVD CAV Speed */
#define e10XDCAV    (10|e_CAV)    /* 10X DVD CAV Speed */
#define e12XDCAV    (12|e_CAV)    /* 12X DVD CAV Speed */
#define e14XDCAV    (14|e_CAV)    /* 14X DVD CAV Speed */
#define e16XDCAV    (16|e_CAV)    /* 16X DVD CAV Speed */
#define e18XDCAV    (18|e_CAV)    /* 18X DVD CAV Speed */
#define e20XDCAV    (20|e_CAV)    /* 20X DVD CAV Speed */

/* DVD Partial CAV*/
#define e1XDPCAV     ( 1|e_Partial|e_CAV)    /*  1X DVD PCAV Speed */
#define e2XDPCAV     ( 2|e_Partial|e_CAV)    /*  2X DVD PCAV Speed */
#define e3XDPCAV     ( 3|e_Partial|e_CAV)    /*  3X DVD PCAV Speed */
#define e4XDPCAV     ( 4|e_Partial|e_CAV)    /*  4X DVD PCAV Speed */
#define e5XDPCAV     ( 5|e_Partial|e_CAV)    /*  5X DVD PCAV Speed */
#define e6XDPCAV     ( 6|e_Partial|e_CAV)    /*  6X DVD PCAV Speed */
#define e8XDPCAV     ( 8|e_Partial|e_CAV)    /*  8X DVD PCAV Speed */
#define e10XDPCAV    (10|e_Partial|e_CAV)    /* 10X DVD PCAV Speed */
#define e12XDPCAV    (12|e_Partial|e_CAV)    /* 12X DVD PCAV Speed */
#define e16XDPCAV    (16|e_Partial|e_CAV)    /* 16X DVD PCAV Speed */
#define e18XDPCAV    (18|e_Partial|e_CAV)    /* 18X DVD PCAV Speed */
#define e20XDPCAV    (20|e_Partial|e_CAV)    /* 20X DVD PCAV Speed */

/*Speed Value Mask*/
#define eSpeedValue 0x3F        /*Speed Value Mask*/
#define GetSpeedValue(X)    (X & eSpeedValue)

/***************************************
** Host Speed Selection Transfer Rate **
****************************************/
/* CD Transfer Rate Defines */
#define SP1X        0x00B0      /*  176.4 Kbyte/Sec */
#define SP2X        0x0161      /*  352.8 Kbyte/Sec */
#define SP4X        0x02C2      /*  705.6 Kbyte/Sec */
#define SP8X        0x0583      /* 1411.2 Kbyte/Sec */
#define SP10X       0x06E4      /* 1764.0 Kbyte/Sec */
#define SP12X       0x0845      /* 2116.8 Kbyte/Sec */
#define SP16X       0x0B06      /* 2822.4 Kbyte/Sec */
#define SP20X       0x0DC8      /* 3528.0 Kbyte/Sec */
#define SP24X       0x108A      /* 4233.6 Kbyte/Sec */
#define SP32X       0x160D      /* 5644.8 Kbyte/Sec */
#define SP40X       0x1B90      /* 7056.0 Kbyte/Sec */
#define SP48X       0x2113      /* 8467.2 Kbyte/Sec */
#define SP52X       0x23D4      /* 9172.2 Kbyte/Sec */
#define SP54X       0x2536      /* dummy  Kbyte/Sec */

/* DVD & DVD-RAM Transfer Rate Defines */
#define SP1XDVD     0x0569      /*  1385 Kbyte/Sec */
#define SP2XDVD     0x0AD2      /*  2770 Kbyte/Sec */
#define SP2_4XDVD   0x0CFC      /*  3324 Kbyte/Sec */
#define SP3XDVD     0x103B      /*  4155 Kbyte/Sec */
#define SP4XDVD     0x15A4      /*  5540 Kbyte/Sec */
#define SP5XDVD     0x1B0D      /*  6925 Kbyte/Sec */
#define SP6XDVD     0x2076      /*  8310 Kbyte/Sec */
#define SP7XDVD     0x25DF      /*  9695 Kbyte/Sec */
#define SP8XDVD     0x2B48      /* 11080 Kbyte/Sec */
#define SP10XDVD    0x361A      /* 13850 Kbyte/Sec */
#define SP12XDVD    0x40EC      /* 16620 Kbyte/Sec */
#define SP16XDVD    0x5690      /* 22160 Kbyte/Sec */
#define SP20XDVD    0x6C34      /* 27700 Kbyte/Sec */

/* BD Transfer Rate Defines */
#define SP1XBD      0x118F      /*  4495 Kbyte/Sec  */
#define SP2XBD      0x22BB      /*  8891 Kbyte/Sec  */
#define SP2_2XBD    0x26A1      /*  9889 Kbyte/Sec  */
#define SP4XBD      0x463E      /*  17982 Kbyte/Sec */
#define SP6XBD      0x695A      /*  26970 Kbyte/Sec */
#define SP8XBD      0x8C78      /*  35960 Kbyte/Sec */
#define SP10XBD     0xAF96      /*  44950 Kbyte/Sec */
#define SP12XBD     0xD2B4      /*  53940 Kbyte/Sec */
#define SP14XBD     0xF5D2      /*  62930 Kbyte/Sec */

/* HD Transfer Rate Defines */
#define SP1XHD      0x1246      /*  4678 Kbyte/Sec  */
#define SP2XHD      0x248C      /*  9356 Kbyte/Sec  */
#define SP4XHD      0x4918      /*  18712 Kbyte/Sec */

/* SpeedMD defines */
typedef unsigned char SpeedMDT;
/* Host Speed Selection to Speed Table Index        */
#define Di_f            0           /* Di           */
#define Standby_f       1           /* Standby      */
#define Write_f         2           /* Write        */
#define Wait_f          3           /* Wait         */
#define Idle_f          4           /* Idle         */
#define MiddleMode      5
#define Set_sp_f        20          /* Set speed    */
#define Special_f       21          /* Special      */
#define StartUp_f       22          /* DVD Start Up */
#if(CEProduct == 0)
#define Special_for_CDSpeed_f       23          /* Special speed for CDSpeed      */
#if (SEQ_SEEK_SPEED_DOWN == 1)
#define Special_for_SeekSpeed       24          /* Special speed for random seek speed down */
#endif
#endif
#define AutoMode        101
#define BypassSpeedMD   0xFF        /* Vendor Unique command bypass of SpeedMD speed setting */
/* end of SpeedMD defines */

typedef unsigned char DiscIdStatusT;
#define eDiscIDNotFound 0
#define eDiscIDFound    1
#define eDiscIDToRecord 2

#define DecECCBlockAmount    0x2A

#ifndef  NULL
  #define   NULL    0
#endif /* NULL */

enum {
    eNormalD=0,
    eFlashDisc,
    eVideoCD,
    eCDI,
    eVideoDVD,
    eAVCHD,
    eWinBench_99,
    eCdTach2,
    eWinBench_30,
    eABEX702,
    eVideoBD
    };

/*================= Assert messages =============================*/
enum
{
    eASSERT_FALSE_DISCKIND = 0,
    eASSERT_IMPOSSIBLE_DISCKIND,
    eASSERT_IMPOSSIBLE_COEF_DISCKIND,
    eASSERT_FUNCTION_NOT_IMPLEMENTED,
    eASSERT_COEFS_NOT_INITIALISED,
    eASSERT_ILLEGAL_COEF_SPEED,
    eASSERT_ILLEGAL_COEF,
    eASSERT_ILLEGAL_COEF_MASK,
    eASSERT_ILLEGAL_COEF_VALUE_OUT_OF_BOUND,
    eASSERT_ILLEGAL_COEF_PROPERTIES,
    eASSERT_INCORRECT_ELEMENT_SIZE,
    eASSERT_PARAMETER_OUT_OF_RANGE,
    eASSERT_ILLEGAL_SWITCH_PARAMETER,
    eASSERT_DSP_BIST_TEST_FAILED,
    eASSERT_DIVISION_BY_ZERO,
    eASSERT_EXCEPTION_SIGNAL_RAISE,
    eASSERT_IMPOSSIBLE_SPINDLE_SPEED,
    eASSERT_WRONG_CHIP_REVISION,
    eASSERT_STREAM_TOO_MANY_SAMPLES
};

/*arm exception singal used by C and C++ libraries */
enum
{
    eSIGABRT     = 1,
    eSIGFPE,
    eSIGSTAK     = 7,
    eSIGRTRED,
    eSIGRTMEM,
    eSIGPVFN     = 12,
    eSIGCPPL
};

/* hidden second argument to SIGFPE handlers. */
enum
{
    eFPE_INVALID     = 1,
    eFPE_ZERODIVIDE,
    eFPE_OVERFLOW,
    eFPE_UNDERFLOW,
    eFPE_INEXACT
};

/* PI */
#define PI 3.14159265359

/* Bit Rates */

#define CD_BITRATE_N1                4321800       //CD 1X bit rate in Hz
#define DVD_BITRATE_N1              26160000       //DVD 1X bit rate in Hz
#define BD_BITRATE_N1               66000000       //BD 1X bit rate in Hz
#define DVDRAM_BITRATE_N1           29180000       //DVDRAM 1X bit rate in Hz
#define DVDRAM_EMBOSSED_BITRATE_N1  18840000       //DVDRAM embossed 1X bit rate in Hz
#define HDDVD_EMBOSSED_BITRATE_N1   32400000       //HDDVD embossed 1X bit rate in Hz
#define HDDVD_DATA_BITRATE_N1       64800000       //HDDVD data 1X bit rate in Hz
#define HDDVD_RAM_BITRATE_N1        64800000       //HDDVD RAM 1X bit rate in Hz

/* Wobble frequencies */

#define CD_WBL_FREQ_N1                 22050       //CD 1X speed factor wobble freq in Hz
#define DVD_PR_PRW_WBL_FREQ_N1        817383       //DVDpRpRW 1X speed factor wobble freq in Hz
#define DVD_MR_MRW_WBL_FREQ_N1        140625       //DVDmRmRW 1X speed factor wobble freq in Hz
#define DVDRAM_WBL_FREQ_N1            156882       //DVDRAM 1X speed factor wobble freq in Hz
#define BD_WBL_FREQ_N1                956522       //BD 1X speed factor wobble freq in Hz
#define HDDVD_WBL_FREQ_N1             696774       //HDDVD 1X speed factor wobble freq in Hz

/*Special Disc Information Definitions*/
/*Indicator*/
#define    PASS_THIS_ITEM           0x00
#define    NEW_SDI_CHECK            0x01    // SDI -> Special Disc Information.
#define    CONTINUE_SDI_CHECK       0x02    //SDI -> Special Disc Information.
#define    END_SDI_CHECK            0xFF
/*Disc Restriction*/
#define    ALL_KINDS_OF_CD_DISCS    1            //include wobble and non-wobble CD disc
#define    NON_WOBBLE_DISC          2
#define    WOBBLE_DISC              3
#define    DATA_CD_DISC             4
#define    NON_WOBBLE_DATA_CD_DISC  5
#define    WOBBLE_DATA_CD_DISC      6
#define    AUDIO_CD_DISC            7
#define    NON_WOBBLE_AUDIO_CD_DISC 8
#define    WOBBLE_AUDIO_CD_DISC     9
#define    ALL_KINDS_OF_DVD_DISCS   11        //include wobble and non-wobble DVD disc
#define    NON_WOBBLE_DVD_DISC      12
#define    WOBBLE_DVD_DISC          13
#define    ALL_KINDS_OF_BD_DISCS   14        //include wobble and non-wobble DVD disc
#define    NON_WOBBLE_BD_DISC      15
#define    WOBBLE_BD_DISC          16
/*Check Method*/
#define    CHECK_STRING             0
#define    CHECK_TRACK_POSITION     1

/*****************************************************************/
/* Found Runout  */
#define eRUNOUT_1_VALID     0x01        /* Runout Block 1 valid */
#define eRUNOUT_2_VALID     0x02        /* RunOut Block 2 valid */
#define eRUNOUT_VALID       0x03

#if (EN_ADI == 1)
#define ADI_INTERVAL_CHECK               300000 //(ms)  src: 300000
#define ADI_LIMIT_COUNT                      20
//#define NORMAL_SPEEDUP_INTERVAL           60000 //(ms) for speed up
#endif

#if (SERVO_DEBUG_VARIABLE != 0)
extern vLONG servo_debug[SERVO_DEBUG_VARIABLE];
#endif

#endif /* __GLOBALS_H__ */
