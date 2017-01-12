/****************************************************************************
*                (c) Copyright 2004 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* $Revision: 52 $
* $Date: 11-03-03 14:46 $
*
* DESCRIPTION:
*   This file is a vendor unique configuration file that allows different
*   projects to be configured based on their specific settings
*   as defined below.
*
*   This file contains all the Constants and defines that are based on the
*   main System clock.
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __OEM_SYS_CLK_H__
#define __OEM_SYS_CLK_H__

#include ".\oem\oem_info.h"

/****************************************************************************/
/*                              System Clock PLL                            */
/****************************************************************************/

//                     ___       ______
// XTAL 25MHz >>>>>>>>|   |     |      |
//                    | M |>>>>>|SYSPLL|>>>>>>>>>>>>>>>>>>>>>> SYSCLK2X
// XTAL 33.8688MHz >>>|___|     |______|     #
//                                           #      SDRAM Memory Controller
//                                           #      ____
//                                           #     | SD |
//                                           #>>>>>|RAMC|>>>>> SDRAM RCLK (SYSCLK2X/2)
//                                           #     |____|
//                                           #      ____
//                                           #     |    |
//                                           #>>>>>| /2 |>>>>> SYSCLK
//                                           #     |____|
//                                           #      ____
//                                           #     |    |
//                                           #>>>>>| /4 |>>>>> ARMCLK
//                                           #     |____|
//                                           #      ____
//                                           #     |    |
//                                           #>>>>>| /X |>>>>> SFICLK (Serial Flash ROM clock)
//                                                 |____|
//                                                SFCLKSEL
//
//
/********************************************/
/*    SYSCLK based Conditionals             */
/********************************************/

/********************************************/
/*  SYSCLK, ARMCLK Conditionals             */
/********************************************/

#if (PERIPHERAL_CLK_25MHZ == 0)             /* 0: 33.8688 Xtal */
#define INPUT_CLOCK     33.8688             /* XIN clock in MHz (fixed) */
#define INPUT_CLOCK_HZ  33868800            /* XIN clock in Hz  (fixed) */
#else                                       /* 1: 25MHz SATA Xtal */
#define INPUT_CLOCK     25.0000             /* XIN clock in MHz (fixed) */
#define INPUT_CLOCK_HZ  25000000            /* XIN clock in Hz  (fixed) */
#endif

#if ((PCB == 690))
#define SYSCLK          125                 // ENTER: System clock in MHz
                                            // Supported currently are: 90 MHz, 100 MHz, 110 MKz, and 125 MHz
#else //(PCB == 691)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 1600)||(PCB == 815)||(PCB == 817)
    #if (CEProduct == 0)    //IT
#define SYSCLK          122                 // ENTER: System clock in MHz
    #else                   //CEProduct
#define SYSCLK          100                 // ENTER: System clock in MHz
    #endif
                                            // Supported currently are: 90 MHz, 100 MHz, 110 MKz, and 125 MHz
#endif

#define PLLCO_CLOCK     SYSCLK * 4          // CALC PLLCO clock in MHz
#define CPU_CLOCK       (SYSCLK / 2)        // CALC CPU clock in MHz

#if (SYSCLK == 125)                    // Maximum Clock Supported // 125.00  MHz SYS CLK
 /*** 125.xxxx MHz SYS CLK ***/
#define PLLCFG0 0x09
#define PLLCFG1 0x89
#define PLLCFG2 0x00
#define PLLCOK  0x01
#define PLLCON  0x0013
#define PLLCOM  0x0000

#elif (SYSCLK == 122)                   // 122 MHz
 /*** 121.8750 MHz SYS CLK ***/
#define PLLCFG0 0x79
#define PLLCFG1 0x54
#define PLLCFG2 0x00
#define PLLCOK  0x01
#define PLLCON  0x0026
#define PLLCOM  0x0001

#elif (SYSCLK == 110)                  // 110 MHz
 /*** 110.xxxx MHz SYS CLK ***/
#define PLLCFG0 0x39
#define PLLCFG1 0x4A
#define PLLCFG2 0x00
#define PLLCOK  0x01
#define PLLCON  0x0057
#define PLLCOM  0x0004

#elif (SYSCLK == 100)                  // 100 MHz Typical Clock Supported
/*** 100.xxxx MHz SYS CLK ***/
#define PLLCFG0 0x09
#define PLLCFG1 0x4A
#define PLLCFG2 0x00
#define PLLCOK  0x01
#define PLLCON  0x000F
#define PLLCOM  0x0000

#elif (SYSCLK ==  90)                  // 90 MHz
/*** 90.0 MHz SYS CLK ***/
#define PLLCFG0 0x39
#define PLLCFG1 0x4A
#define PLLCFG2 0x00
#define PLLCOK  0x01
#define PLLCON  0x0047
#define PLLCOM  0x0004
#endif /* SYSCLK */

// The following defines specify the type of SDRAMs as reported by register rEMBEDMEM
#define SDRAM_EXTERNAL         0         // One of the External SDRAMs
#define SDRAM_M12L16161AFK1    1         // Embedded SDRAM: ESMT 16Mbit = 2MByte  16-bit access
#define SDRAM_M12L32162A       3         // Embedded SDRAM: ESMT 32Mbit = 4MByte  16-bit access
#define SDRAM_M12L32321ADM1    4         // Embedded SDRAM: ESMT 32Mbit = 4MByte  32-bit access

//****************************************************
//
// The following are SDRAM Specifications / Settings
// for various types of "embedded" and "external" SDRAM devices
// for different PLL clock speeds such as 500 MHz , 440 MHz, 400 MHz, 360 MHz
//
//****************************************************
//************************************************************
//                                                           *
//   Embedded SDRAM: ESMT 16Mbit = 2MByte  16-bit access     *
//                                                           *
//************************************************************
#if   (SYSCLK == 125)                               /* Maximum Clock Supported */
#define M12L16161AFK1_SDCMD         0x00
#define M12L16161AFK1_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define M12L16161AFK1_SDWDTH32      0               // 16-bit access
#define M12L16161AFK1_CASLAT        1               // CAS Latency = 2
#define M12L16161AFK1_TRAS          2               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L16161AFK1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L16161AFK1_TRFC          2               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L16161AFK1_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L16161AFK1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L16161AFK1_BUS32         0               // SDRAM Bus Width = 16 bits
#define M12L16161AFK1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L16161AFK1_TRCD          1               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L16161AFK1_TRP           1               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L16161AFK1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L16161AFK1_SDLCHDLY      4               // Adjust SDRAM Internal Latch Clock Delay
#define M12L16161AFK1_SDINCKSEL     2               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define M12L16161AFK1_SDCMD         0x00
#define M12L16161AFK1_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define M12L16161AFK1_SDWDTH32      0               // 16-bit access
#define M12L16161AFK1_CASLAT        1               // CAS Latency = 2
#define M12L16161AFK1_TRAS          2               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L16161AFK1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L16161AFK1_TRFC          2               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L16161AFK1_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L16161AFK1_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define M12L16161AFK1_BUS32         0               // SDRAM Bus Width = 16 bits
#define M12L16161AFK1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L16161AFK1_TRCD          1               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L16161AFK1_TRP           1               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L16161AFK1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L16161AFK1_SDLCHDLY      4               // Adjust SDRAM Internal Latch Clock Delay
#define M12L16161AFK1_SDINCKSEL     2               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define M12L16161AFK1_SDCMD         0x00
#define M12L16161AFK1_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define M12L16161AFK1_SDWDTH32      0               // 16-bit access
#define M12L16161AFK1_CASLAT        1               // CAS Latency = 2
#define M12L16161AFK1_TRAS          2               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L16161AFK1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L16161AFK1_TRFC          2               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L16161AFK1_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L16161AFK1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L16161AFK1_BUS32         0               // SDRAM Bus Width = 16 bits
#define M12L16161AFK1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L16161AFK1_TRCD          1               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L16161AFK1_TRP           1               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L16161AFK1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L16161AFK1_SDLCHDLY      4               // Adjust SDRAM Internal Latch Clock Delay
#define M12L16161AFK1_SDINCKSEL     2               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical clock
#define M12L16161AFK1_SDCMD         0x00
#define M12L16161AFK1_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define M12L16161AFK1_SDWDTH32      0               // 16-bit access
#define M12L16161AFK1_CASLAT        1               // CAS Latency = 2
#define M12L16161AFK1_TRAS          2               // TRAS = 5 SYSCLK Periods
#define M12L16161AFK1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L16161AFK1_TRFC          2               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L16161AFK1_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L16161AFK1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L16161AFK1_BUS32         0               // SDRAM Bus Width = 16 bits
#define M12L16161AFK1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L16161AFK1_TRCD          1
#define M12L16161AFK1_TRP           1               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L16161AFK1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L16161AFK1_SDLCHDLY      4               // Adjust SDRAM Internal Latch Clock Delay
#define M12L16161AFK1_SDINCKSEL     2               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define M12L16161AFK1_SDCMD         0x00
#define M12L16161AFK1_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define M12L16161AFK1_SDWDTH32      0               // 16-bit access
#define M12L16161AFK1_CASLAT        1               // CAS Latency = 2
#define M12L16161AFK1_TRAS          2               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L16161AFK1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L16161AFK1_TRFC          2               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L16161AFK1_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L16161AFK1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L16161AFK1_BUS32         0               // SDRAM Bus Width = 16 bits
#define M12L16161AFK1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L16161AFK1_TRCD          1               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L16161AFK1_TRP           1               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L16161AFK1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L16161AFK1_SDLCHDLY      4               // Adjust SDRAM Internal Latch Clock Delay
#define M12L16161AFK1_SDINCKSEL     2               // Adjust SDRAM Data Latch Clock Delay
#endif

//************************************************************
//                                                           *
//   Embedded SDRAM: ESMT 32Mbit = 4MByte  32-bit access     *
//                                                           *
//************************************************************

#if   (SYSCLK == 125)                               /* Maximum Clock Supported */
#define M12L32321ADM1_SDCMD         0x00
#define M12L32321ADM1_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define M12L32321ADM1_SDWDTH32      1               // 32-bit access
#define M12L32321ADM1_CASLAT        0               // CAS Latency = 2
#define M12L32321ADM1_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32321ADM1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32321ADM1_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32321ADM1_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32321ADM1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L32321ADM1_BUS32         1               // SDRAM Bus Width = 32 bits
#define M12L32321ADM1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L32321ADM1_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32321ADM1_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32321ADM1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L32321ADM1_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32321ADM1_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define M12L32321ADM1_SDCMD         0x00
#define M12L32321ADM1_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define M12L32321ADM1_SDWDTH32      1               // 32-bit access
#define M12L32321ADM1_CASLAT        0               // CAS Latency = 2
#define M12L32321ADM1_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32321ADM1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32321ADM1_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32321ADM1_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32321ADM1_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define M12L32321ADM1_BUS32         1               // SDRAM Bus Width = 32 bits
#define M12L32321ADM1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L32321ADM1_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32321ADM1_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32321ADM1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L32321ADM1_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32321ADM1_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define M12L32321ADM1_SDCMD         0x00
#define M12L32321ADM1_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define M12L32321ADM1_SDWDTH32      1               // 32-bit access
#define M12L32321ADM1_CASLAT        0               // CAS Latency = 2
#define M12L32321ADM1_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32321ADM1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32321ADM1_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32321ADM1_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32321ADM1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L32321ADM1_BUS32         1               // SDRAM Bus Width = 32 bits
#define M12L32321ADM1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L32321ADM1_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32321ADM1_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32321ADM1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L32321ADM1_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32321ADM1_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define M12L32321ADM1_SDCMD         0x00
#define M12L32321ADM1_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define M12L32321ADM1_SDWDTH32      1               // 32-bit access
#define M12L32321ADM1_CASLAT        0               // CAS Latency = 2
#define M12L32321ADM1_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32321ADM1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32321ADM1_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32321ADM1_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32321ADM1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L32321ADM1_BUS32         1               // SDRAM Bus Width = 32 bits
#define M12L32321ADM1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L32321ADM1_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32321ADM1_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32321ADM1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L32321ADM1_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32321ADM1_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define M12L32321ADM1_SDCMD         0x00
#define M12L32321ADM1_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define M12L32321ADM1_SDWDTH32      1               // 32-bit access
#define M12L32321ADM1_CASLAT        0               // CAS Latency = 2
#define M12L32321ADM1_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32321ADM1_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32321ADM1_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32321ADM1_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32321ADM1_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define M12L32321ADM1_BUS32         1               // SDRAM Bus Width = 32 bits
#define M12L32321ADM1_NOVL          0               // Overlapped Mode = 0 = enabled.
#define M12L32321ADM1_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32321ADM1_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32321ADM1_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define M12L32321ADM1_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32321ADM1_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif

//************************************************************
//
//   Embedded SDRAM: ESMT 32Mbit = 4MByte  16-bit access     *
//
//************************************************************

#if   (SYSCLK == 125)                               /* Maximum Clock Supported */
#define M12L32162A_SDCMD         0x00
#define M12L32162A_SDSIZE        3                  // SDRAM Device Size = 4 MBytes
#define M12L32162A_SDWDTH32      1                  // 32-bit access
#define M12L32162A_CASLAT        0                  // CAS Latency = 2
#define M12L32162A_TRAS          1                  // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32162A_TWR           0                  // Write Recovery Time = 2 SYSCLK Periods
#define M12L32162A_TRFC          0                  // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32162A_REFINTV       0x6B               // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32162A_REFINTV_SLEEP 0x08               // Refresh Interval in Power Sleep Mode
#define M12L32162A_BUS32         1                  // SDRAM Bus Width = 32 bits
#define M12L32162A_NOVL          0                  // Overlapped Mode = 0 = enabled.
#define M12L32162A_TRCD          0                  // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32162A_TRP           0                  // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32162A_SDCKODLY      1                  // Adjust SDRAM Output Clock Delay
#define M12L32162A_SDLCHDLY      3                  // Adjust SDRAM Internal Latch Clock Delay
#define M12L32162A_SDINCKSEL     3                  // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define M12L32162A_SDCMD            0x00
#define M12L32162A_SDSIZE           3               // SDRAM Device Size = 4 MBytes
#define M12L32162A_SDWDTH32         0               // 16-bit access
#define M12L32162A_CASLAT           0               // CAS Latency = 2
#define M12L32162A_TRAS             1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32162A_TWR              0               // Write Recovery Time = 2 SYSCLK Periods
#define M12L32162A_TRFC             0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32162A_REFINTV          0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32162A_REFINTV_SLEEP    0x06            // Refresh Interval in Power Sleep Mode
#define M12L32162A_BUS32            0               // SDRAM Bus Width = 16 bits
#define M12L32162A_NOVL             0               // Overlapped Mode = 0 = enabled.
#define M12L32162A_TRCD             0               // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32162A_TRP              0               // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32162A_SDCKODLY         1               // Adjust SDRAM Output Clock Delay
#define M12L32162A_SDLCHDLY         3               // Adjust SDRAM Internal Latch Clock Delay
#define M12L32162A_SDINCKSEL        3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define M12L32162A_SDCMD         0x00
#define M12L32162A_SDSIZE        3                  // SDRAM Device Size = 4 MBytes
#define M12L32162A_SDWDTH32      1                  // 32-bit access
#define M12L32162A_CASLAT        0                  // CAS Latency = 2
#define M12L32162A_TRAS          1                  // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32162A_TWR           0                  // Write Recovery Time = 2 SYSCLK Periods
#define M12L32162A_TRFC          0                  // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32162A_REFINTV       0x6B               // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32162A_REFINTV_SLEEP 0x08               // Refresh Interval in Power Sleep Mode
#define M12L32162A_BUS32         1                  // SDRAM Bus Width = 32 bits
#define M12L32162A_NOVL          0                  // Overlapped Mode = 0 = enabled.
#define M12L32162A_TRCD          0                  // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32162A_TRP           0                  // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32162A_SDCKODLY      1                  // Adjust SDRAM Output Clock Delay
#define M12L32162A_SDLCHDLY      3                  // Adjust SDRAM Internal Latch Clock Delay
#define M12L32162A_SDINCKSEL     3                  // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define M12L32162A_SDCMD         0x00
#define M12L32162A_SDSIZE        3                  // SDRAM Device Size = 4 MBytes
#define M12L32162A_SDWDTH32      1                  // 32-bit access
#define M12L32162A_CASLAT        0                  // CAS Latency = 2
#define M12L32162A_TRAS          1                  // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32162A_TWR           0                  // Write Recovery Time = 2 SYSCLK Periods
#define M12L32162A_TRFC          0                  // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32162A_REFINTV       0x6B               // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32162A_REFINTV_SLEEP 0x08               // Refresh Interval in Power Sleep Mode
#define M12L32162A_BUS32         1                  // SDRAM Bus Width = 32 bits
#define M12L32162A_NOVL          0                  // Overlapped Mode = 0 = enabled.
#define M12L32162A_TRCD          0                  // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32162A_TRP           0                  // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32162A_SDCKODLY      1                  // Adjust SDRAM Output Clock Delay
#define M12L32162A_SDLCHDLY      3                  // Adjust SDRAM Internal Latch Clock Delay
#define M12L32162A_SDINCKSEL     3                  // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define M12L32162A_SDCMD         0x00
#define M12L32162A_SDSIZE        3                  // SDRAM Device Size = 4 MBytes
#define M12L32162A_SDWDTH32      1                  // 32-bit access
#define M12L32162A_CASLAT        0                  // CAS Latency = 2
#define M12L32162A_TRAS          1                  // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define M12L32162A_TWR           0                  // Write Recovery Time = 2 SYSCLK Periods
#define M12L32162A_TRFC          0                  // Auto Refresh Command Period = 7 SYSCLK Periods
#define M12L32162A_REFINTV       0x61               // Refresh Interval = X * 16 * SYSCLK Period
#define M12L32162A_REFINTV_SLEEP 0x08               // Refresh Interval in Power Sleep Mode
#define M12L32162A_BUS32         1                  // SDRAM Bus Width = 32 bits
#define M12L32162A_NOVL          0                  // Overlapped Mode = 0 = enabled.
#define M12L32162A_TRCD          0                  // Active to Read or Write Delay = 1 SYSCLK Period
#define M12L32162A_TRP           0                  // Precharge Command Period = 2 SYSCLK Periods.
#define M12L32162A_SDCKODLY      1                  // Adjust SDRAM Output Clock Delay
#define M12L32162A_SDLCHDLY      3                  // Adjust SDRAM Internal Latch Clock Delay
#define M12L32162A_SDINCKSEL     3                  // Adjust SDRAM Data Latch Clock Delay
#endif

//***********************************************************
//                  External SDRAM Defines                  *
// These are only valid when the rEMBEDMEM register is 0x00 *
//***********************************************************

#if (EXTERNAL_SDRAM_TYPE == HY57V161610DTC55)
//**********************************************************
//    External SDRAM                                       *
//    HY57V161610D by Hynix                                *
//    2 MByte External SDRAM 16-bit access                 *
//    2 Banks x 512K x 16 Bit Synchronous DRAM             *
//**********************************************************

#if   (SYSCLK == 125)                               //  Maximum Clock Supported */
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        2               // SDRAM Device Size = 2 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif
#endif  // 2 MB external SDRAM

#if (EXTERNAL_SDRAM_TYPE == M12L32162A7TG)
//**********************************************************
//    External SDRAM                                       *
//    M12L32162A7TG ESMT                                   *
//    4 MByte External SDRAM 16-bit access                 *
//    2 Banks x 1M x 16 Bit Synchronous DRAM               *
//**********************************************************

#if   (SYSCLK == 125)                               //  Maximum Clock Supported */
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif // SYSCLK
#endif  // 4 MB external SDRAM

#if (EXTERNAL_SDRAM_TYPE == MT48LC2M32B2)
//**********************************************************
//    External SDRAM                                       *
//    MT48LC2M32B2 by Micron                               *
//    8 MByte External SDRAM 32-bit access                 *
//    512K x 32 x 4 banks                                  *
//**********************************************************

#if   (SYSCLK == 125)                               // Maximum Clock Supported */
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         1               // SDRAM Bus Width = 32 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        3               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif
#endif  // 8 MB external SDRAM

#if (EXTERNAL_SDRAM_TYPE == IS42S32200C1)
//**********************************************************
//    External SDRAM                                       *
//    IS42S32200C1 by ISSI                                 *
//    8 MByte External SDRAM 32-bit access                 *
//    512K x 32 x 4 banks                                  *
//**********************************************************

#if   (SYSCLK == 125)                               // Maximum Clock Supported */
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         1               // SDRAM Bus Width = 32 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          0               // TRAS = 4 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         1               // SDRAM Bus Width = 32 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          1               // Active to Read or Write Delay = 2 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 4 MBytes
#define EXTERNALSDRAM_SDWDTH32      1               // 32-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif
#endif  // 8 MB external SDRAM

#if (EXTERNAL_SDRAM_TYPE == K4S641632K)
//**********************************************************
//    External SDRAM                                       *
//    K4S641632K Samsung                                   *
//    8 MByte External SDRAM 16-bit access                 *
//    4 Banks x 1M x 16 Bit Synchronous DRAM               *
//**********************************************************

#if   (SYSCLK == 125)                               //  Maximum Clock Supported */
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 122)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x77            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x06            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 110)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK == 100)                               // Typical Clock
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x6B            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay

#elif (SYSCLK ==  90)
#define EXTERNALSDRAM_SDCMD         0x00
#define EXTERNALSDRAM_SDSIZE        4               // SDRAM Device Size = 8 MBytes
#define EXTERNALSDRAM_SDWDTH32      0               // 16-bit access
#define EXTERNALSDRAM_CASLAT        0               // CAS Latency = 2
#define EXTERNALSDRAM_TRAS          1               // TRAS = 5 SYSCLK Periods Active to Precharge Command
#define EXTERNALSDRAM_TWR           0               // Write Recovery Time = 2 SYSCLK Periods
#define EXTERNALSDRAM_TRFC          0               // Auto Refresh Command Period = 7 SYSCLK Periods
#define EXTERNALSDRAM_REFINTV       0x61            // Refresh Interval = X * 16 * SYSCLK Period
#define EXTERNALSDRAM_REFINTV_SLEEP 0x08            // Refresh Interval in Power Sleep Mode
#define EXTERNALSDRAM_BUS32         0               // SDRAM Bus Width = 16 bits
#define EXTERNALSDRAM_NOVL          0               // Overlapped Mode = 0 = enabled.
#define EXTERNALSDRAM_TRCD          0               // Active to Read or Write Delay = 1 SYSCLK Period
#define EXTERNALSDRAM_TRP           0               // Precharge Command Period = 2 SYSCLK Periods.
#define EXTERNALSDRAM_SDCKODLY      1               // Adjust SDRAM Output Clock Delay = 0
#define EXTERNALSDRAM_SDLCHDLY      3               // Adjust SDRAM Internal Latch Clock Delay = 0
#define EXTERNALSDRAM_SDINCKSEL     3               // Adjust SDRAM Data Latch Clock Delay
#endif // SYSCLK
#endif  // 8 MB external SDRAM

//*******************************************************************
//    The following block contains defines for Serial Flash Clock
//    Frequency of Serial Flash Clock can be programmed through
//    register rwSFCTRL3 resulting SFlash clock is shown in table below
//    Equation for SFlash Clock frequency is:
//    Frequ = (SYSCLK * 2) / (SFCLKSEL + 1)
//    or
//    Frequ = (PLLC0  / 2) / (SFCLKSEL + 1)
//
//           PLLC0/MHz   500     440     400     360
// SFCLKSEL  SYSCLK/MHz  125     110     100      90
//    0                 gated  gated    gated   gated
//    1                125.000 110.000  100.000 90.000
//                                    ==================  Cutoff - max frequency is 67MHz
//    2                83.333  73.333 | 66.667  60.000
//          Cutoff   ==================
//    3                62.500  55.000   50.000  45.000
//    4                50.000  44.000   40.000  36.000
//    5                41.667  36.667   33.333  30.000
//    6                35.714  31.429   28.571  25.714
//    7                31.250  27.500   25.000  22.500
//
//*******************************************************************

#if   (SYSCLK == 125)       // Maximum Clock Supported */
    #if   SERIAL_FLASH_TYPE == MX25LV8005
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 50 MHz.
    #elif SERIAL_FLASH_TYPE == STM25PE80
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == SST25VF040
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == STM25P80
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == S25FL016A
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 50 MHz
    #else
        Error: No Serial Flasg defined
    #endif
#elif (SYSCLK == 122)
    #if   SERIAL_FLASH_TYPE == MX25LV8005
        #define SFCLKSEL   4
     #elif SERIAL_FLASH_TYPE == STM25PE80
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 48.750 MHz
    #elif SERIAL_FLASH_TYPE == SST25VF040
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 48.750 MHz
    #elif SERIAL_FLASH_TYPE == STM25P80
        #define SFCLKSEL   4
    #elif SERIAL_FLASH_TYPE == MX25L1605D
        #define SFCLKSEL   2                   // Set Serial Flash Clock to 81.250 MHz
    #elif SERIAL_FLASH_TYPE == S25FL016A
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 48.750 MHz
    #else
        Error: No Serial Flash definition
    #endif
#elif (SYSCLK == 110)
    #if   SERIAL_FLASH_TYPE == MX25LV8005
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 44 MHz
    #elif SERIAL_FLASH_TYPE == STM25PE80
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 44 MHz
    #elif SERIAL_FLASH_TYPE == SST25VF040
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 44 MHz
    #elif SERIAL_FLASH_TYPE == STM25P80
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 44 MHz
    #elif SERIAL_FLASH_TYPE == S25FL016A
        #define SFCLKSEL   4                   // Set Serial Flash Clock to 44 MHz
    #else
        Error: No Serial Flasg defined
    #endif
#elif (SYSCLK == 100)
    #if   SERIAL_FLASH_TYPE == MX25LV8005
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == STM25PE80
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == SST25VF040
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == STM25P80
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 50 MHz
    #elif SERIAL_FLASH_TYPE == S25FL016A
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 50 MHz
    #else
        Error: No Serial Flasg defined
    #endif
#elif (SYSCLK ==  90)
    #if   SERIAL_FLASH_TYPE == MX25LV8005
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 45 MHz
    #elif SERIAL_FLASH_TYPE == STM25PE80
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 45 MHz
    #elif SERIAL_FLASH_TYPE == SST25VF040
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 45 MHz
    #elif SERIAL_FLASH_TYPE == STM25P80
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 45 MHz
    #elif SERIAL_FLASH_TYPE == S25FL016A
        #define SFCLKSEL   3                   // Set Serial Flash Clock to 45 MHz
  #else
        Error: No Serial Flasg defined
    #endif
#endif


//**************************************************************************************
//
//     The following block contains defines for Syncronous Serial Interface Clock
//                                      and for External Write Strategy Clock
//    Frequency of Serial Block can be programmed through
//    registers rwXWSCLKDIV and rwSSICLKDIV  resulting output clock frequencies are shown in table below
//
//    Equation for External Synchronous Serial Interface Clock output frequency is:
//
//    SCLK = ARMCLK / (XWSCLKDIV + 1)
//
//    Equation for External Write Strategy Serial Serial Interface Clock frequency is:
//
//    SCLK = ARMCLK / (SSICLKDIV + 1)
//
//
//
//         SYSCLK  125     110     100     90
//         PLLC0   500     440     400     360
//         CPU CLK 62.5    55.0    50.0    45.0
//         ARM CLK 62.5    55.0    50.0    45.0
//     XWSCLKDIV
//     SSICLKDIV
//     0           62.500  55.000  50.000  45.000
//     1           31.250  27.500  25.000  22.500
//     2           20.833  18.333  16.667  15.000
//     3           15.625  13.750  12.500  11.250
//     4           12.500  11.000  10.000  9.000
//     5           10.417  9.167   8.333   7.500
//     6           8.929   7.857   7.143   6.429
//     7           7.813   6.875   6.250   5.625
//
//**************************************************************************************

	#if (BIOS_MODEL == 0)
#if   (SYSCLK == 125)       // Maximum Clock Supported */
#define bXWSCLKDIV              2               // Set XWS Clock to 20.8333 MHz.
#define bSSICLKDIV              2               // Set SSI Clock to 20.8333 MHz.
#elif (SYSCLK == 122)
#define bXWSCLKDIV              2               // Set XWS Clock to 20.3125 MHz.
#define bSSICLKDIV              2               // Set SSI Clock to 20.3125 MHz.
    #elif (SYSCLK == 121)
#define bXWSCLKDIV              2               // Set XWS Clock to 20.1388 MHz.
#define bSSICLKDIV              2               // Set SSI Clock to 20.1388 MHz.
#elif (SYSCLK == 110)
#define bXWSCLKDIV              1               // Set XWS Clock to 27.5000 MHz.
#define bSSICLKDIV              1               // Set SSI Clock to 27.5000 MHz.
#elif (SYSCLK == 100)
#define bXWSCLKDIV              1               // Set XWS Clock to 25.0000 MHz.
#define bSSICLKDIV              1               // Set SSI Clock to 25.0000 MHz.
#elif (SYSCLK ==  90)
#define bXWSCLKDIV              1               // Set XWS Clock to 22.5000 MHz.
#define bSSICLKDIV              1               // Set SSI Clock to 22.5000 MHz.
    #endif
	#elif (BIOS_MODEL == 2)
        #if (SYSCLK == 125)                     // Maximum Clock Supported */
#define bSSICLKDIV             15               // Set SSI Clock to  3.9063 MHz.
        #elif (SYSCLK == 122)
#define bSSICLKDIV             14               // Set SSI Clock to  4.0625 MHz.
        #elif (SYSCLK == 121)
#define bSSICLKDIV             14               // Set SSI Clock to  4.0625 MHz.
        #elif (SYSCLK == 110)
#define bSSICLKDIV             13               // Set SSI Clock to  3.9286 MHz.
        #elif (SYSCLK == 100)
#define bSSICLKDIV             12               // Set SSI Clock to  3.8462 MHz.
        #elif (SYSCLK ==  90)
#define bSSICLKDIV             10               // Set SSI Clock to  4.0909 MHz.
        #endif
#endif

#define SERVO_CLOCK     33868800            /* Servo clock */   // TBD? this will probably be taken care of by stefan

#endif   /*  __OEM_SYS_CLK_H__   */



/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
