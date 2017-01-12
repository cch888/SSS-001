/****************************************************************************
*               (c) Copyright 2003 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME: oem_info.h
*
* DESCRIPTION:
*   This file is a oem specific file which contains information for the Inquiry
*   and Identify Device commands. It also contains switches to support different
*   options in the firmware.
*
* Note: This file will be unique to each project.
*
* $Revision: 646 $
* $Date: 11/03/21 10:11a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __OEM_INFO_H__
#define __OEM_INFO_H__
#include ".\oem\OemConfig.h"
/*****************************************************/
/*  Flash ROM Defines and Selection                  */
/*****************************************************/
#define FLASH_ROM_512KB     (1024L * 512L)
#define FLASH_ROM_1MB       (1024L * 1024L)
#define FLASH_ROM_2MB       (FLASH_ROM_1MB * 2)

#define FLASH_ROM_SIZE      FLASH_ROM_1MB


/*****************************************************/
/*  Flash ROM Defines and Selection                  */
/*****************************************************/
#define SST39LF040          1       /* SST   */
#define MBM29LV400BC        2       /* FUJISTU */
#define AT49F004            3       /* ATMEL    */
#define MX29LV004B          4       /* MXIC */
#define AM29LV004B          4       /* MXIC */
#define MX29LV800B          5       /* MXIC     */
#define AM29LV800B          5       /* AMD 1MB Bottom Boot */
#define SST39VF088          6       /* SST 1MB */
#define AM29LV800T          7       /* AMD 1MB Top Boot */
#define SST25VF080          8       /* SST 1MB Top Boot Serial Flash Rom */
#define SST25VF040          9       /* SST 512KB Top Boot Serial Flash Rom */
#define STM25PE80           10      /* ST M25PE80 1MB Serial Flash Rom */
#define MX25LV8005          11      /* MXIC MX25LV8005 1MB Serial Flash Rom */
#define STM25P80            12      /* ST M25P80 1MB Serial Flash Rom */
#define WBW25X80            13      /* WINBOND W25X80 1MB Serial Flash Rom */
#define MX25L1605D          14      /* MXIC MX25L1605D 2MB Serial Flash Rom */
#define S25FL040AB          15      /* SPANSION S25FL040A Bottom Boot 512K Serial Flash Rom */
#define LE25FW806MA         16      /* SANYO LE25FW806MA 1MB Serial Flash Rom */
#define S25FL016A           17      /* SPANSION S25FL016A 2MB Serial Flash Rom */
#define F25L08PA            18      /* ESMT F25L08PA 1MB Serial Flash Rom */
#define WBW25Q80            19      /* WINBOND W25Q80 1MB Serial Flash Rom */
#define MX25L8006           20      /* MXIC MX25L8065 1MB Serial Flash Rom */


/*****************************************************/
/*  OPU Defines and Selection                        */
/*****************************************************/
#define HOP7061             1       /* Hitachi HOP-7061T Combo OPU */
#define HOP7081             2       /* Hitachi HOP-7081 Combo OPU */
#define HOP7503             3       /* Hitachi HOP-7503 RDVD OPC */
#define HOP7432TL           6       /* Hitachi HOP-7432TL RDVD 12X OPU without CD Write support */
#define HOP7632TL           7       /* Hitachi HOP-7632TL RDVD 16X OPU with DVD-RAM Read/Write support */
#define HOP7232TL           8       /* Hitachi HOP-7232TL RDVD AV OPU */
#define OPU6651AV           9       /* Arima OPU66.51AV RDVD AV OPU */
#define HOPH353L            10      /* Hitachi Super Multi OPU HOP-H353L with DVD-RAM Read/Write and 18X DVD write */
#define KES410A             11      /* SONY BD triple read reader OPU */
#define DXX811              12      /* TDK HD Triple Reader OPU */
#define SOHBP4              13      /* OPU for BRCM1600 Project */
#define KEM350AAA           14      /* SONY BD triple writer (including DVDRAM) OPU */
#define HOPB1300            15      /* Hitachi HOP-B1300 BDROM AV/IT OPU */
#define SFBD412             16      /* Sanyo BD412 BDROM AV/IT OPU */
#define BDPL2               17      /* Pioneer BDPL2 BDROM AV OPU */
#define SFBD414             18      /* Sanyo BD414 BDROM AV/IT OPU */
#define SOHBP7              19      /* Samsung BDplayer OPU */

/*****************************************************/
/*  Laser Driver Defines and Selection                        */
/*****************************************************/
#define NO_LASER_DRIVER     0       /* No Laser Driver */
#define SAC1036SP           1       /* COSMO SAC1036SP for ROM platform */

/*****************************************************/
/*  Motor Driver Defines and Selection               */
/*****************************************************/
#define BD7907              1       /* Rohm BD7907 Motor Driver IC */
#define BD7902              2       /* Rohm BD7902 Motor Driver IC */
#define BD7905              4       /* Rohm BD7905 Motor Driver IC */
#define BD7954              5       /* Rohm BD7954 Motor Driver IC */
#define R2S30204            6       /* Renesas R2S30204 Sensorless Motor Driver IC */
#define R2S30201            7       /* Renesas R2S30201 Sensorless Motor Driver IC */
#define BD7958              8       /* Rohm BD7958FS Motor Driver IC */
#define R2A30232            9       /* Renesas R2A30232SP Sensorless Motor Driver IC */
#define BD7956              10      /* Rohm BD7956 Motor Driver IC */
#define BA5912BFP           11      /* Rohm BA5912BFP Motor Driver IC - 2 channel */
#define BD7959              12      /* Rohm BD7959 Motor Driver IC */
#define R2S30208            13      /* Renesas R2S30208 Sensorless Motor Driver IC */
#define R2A30406            14      /* Renesas R2S30406 two channel Motor Driver IC digital output */
#define R2A30222            15      /* Renesas R2S30222 eleven channel Motor Driver IC */
#define AM9268              16      /* AMtek 9268FD Motor Driver IC */
#define AM9858              17      /* AMtek 9858 Motor Driver IC */
#define TPIC1407            18      /* TI Motor Driver IC */

//***********************************************************
//                  External SDRAM Defines                  *
// These are only valid when the rEMBEDMEM register is 0x00 *
//***********************************************************
#define HY57V161610DTC55    1       // 2 MByte External SDRAM 16-bit access by Hynix
                                    // 512Kbit x 16 x 2 Banks

#define MT48LC2M32B2        2       // 8 MByte External SDRAM 32-bit access by Micron
                                    // 512Kbit x 32 x 4 banks

#define IS42S32200C1        3       // 8 MByte External SDRAM 32-bit access by ISSI
                                    // 512Kbit x 32 x 4 banks

#define M12L32162A7TG       4       // 4 MByte External SDRAM 16-bit access by ESMT
                                    // 1Mbit x 16 x 2 Banks

#define K4S641632K          5       // 8 MByte External SDRAM 16-bit access by Samsung
                                    // 1Mbit x 16 x 4 Banks

/*****************************************************/
/*                  PCB VERSION                      */
/* Includes: CHIP, AFE, MECHANISM, DVD               */
/*****************************************************/

/********************************************/
/*      PCB and Mechanism Configuration     */
/********************************************/
#define CHIP                        6230

#define WOBBLE_READ_ON_DVD_OR_CD    0

#define CHIP_REV                    0xC1

#if SDRAM_SIZE == 4
#define SDRAM_SIZE_MBYTE            4       /* 4: 4 MB SDRAM */
#elif SDRAM_SIZE == 2
#define SDRAM_SIZE_MBYTE            2       /* 2: 2 MB SDRAM */
#else
#define SDRAM_SIZE_MBYTE            4       /* 0: default 4 MB SDRAM */
#endif

#define SEEPROM                     0       /* 1-- Enable Serial EEPROM functions  */
                                            /* 0-- Disable Serial EEPROM functions */

#define ENABLE_OVERWRITE_EEPROM     0       /* 1-Enable; 0-Disable Overwrite of the SEEPROM with new Table */
                                            /* NOTE: If SEEPROM is set to 1, then you must set this bit to 1
                                               before doing an Offline Calibration */

#define SERIAL_FLASH_TYPE     STM25P80      /* Select installed Serial Flash ROM from the list above */

#if (PCB == 710) || (PCB == 711) ||(PCB == 815)
#define BOOT_CODE_SUPPORT           1       /* 1: Support Boot Code function*/
                                            /* 0: Not Support Boot Code function*/
                                            /* Note: Need to check flash.h boot code start address.*/
#else
#define BOOT_CODE_SUPPORT           0       /* 1: Support Boot Code function*/
                                            /* 0: Not Support Boot Code function*/
                                            /* Note: Need to check flash.h boot code start address.*/
#endif
#define NO_FRONT_PANEL              0       /* Selection depending on if front pannel pcb installed */
                                            /* 1: No Front pannel PCB installed, all front pannel functions disabled */
                                            /* 0: Front pannel PCB installed, all front pannel functions enabled */

#define LED_NUM                     1       /* 1: One LED (READ_LED) */
                                            /* 2: Two LED (READ_LED and Write_LED) */
#if (PCB == 710)
    #if(CHIP_REV >= 0xC0)
    #define OPU                         SFBD414
    #else
    #define OPU                         SFBD412
    #endif
#elif (PCB == 711)
    #define OPU                         SFBD414 /* Will change to BD414 later */
#elif (PCB == 815)
    #define OPU                         BDPL2
#else
    #define OPU                         KES410A
#endif

#if (PCB == 710)||(PCB == 711)
#define LASER_DRIVER                    NO_LASER_DRIVER
#else
#define LASER_DRIVER                    NO_LASER_DRIVER
#endif

#if (PCB == 710)||(PCB == 711)
#define OPU_OUTPUT_MONITOR                  /* if define : Enable monitor digital laser control output */
                                            /* if not define : Disable monitor digital laser control output */
#endif

#define USE_LDEL2_FOR_CD_LD         1       /* 1: Use LDEL2 To Control CD LD power */
                                            /* 0: Use LTHR To Control CD LD power */

#if (PCB == 710)||(PCB == 711)
#define FVREF06_SHIFT_SOLUTION              /* if define : Enable FVREF06 power shift solution */
                                            /* if not define : Disable FVREF06 power shift solution */
#endif

#if (PCB == 815)
#define APP_TE_SUPPORT              1       /* APP TE is used or not*/
#else
#define APP_TE_SUPPORT              0
#endif

#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define MPP_TE_DETECT_DVDR          1
#define DETECT_CDR                  1       /* 1: Enable recognize CDR */
                                            /* CDR will be treated as CDROM */
#else
#define MPP_TE_DETECT_DVDR          0
#define DETECT_CDR                  0       /* 1: Enable recognize CDR */
                                            /* CDR will be treated as CDROM */
#endif

#if OPU == KEM350AAA
#define LCD                         1
#else
#define LCD                         0
#endif

#if (PCB == 690)
#define MOTOR_DRIVER                BD7956    //sun690 use BD7956 , sun692 use BD7959
#elif (PCB == 692)||(PCB == 812)
#define MOTOR_DRIVER                BD7959    //sun690 use BD7956 , sun692 use BD7959
#elif (PCB == 710)
#define MOTOR_DRIVER                R2S30208  //sun710 use R2S30208
#elif (PCB == 711)
#define MOTOR_DRIVER                AM9268    //sun711 use AM9268FD
#elif (PCB == 815)
#define MOTOR_DRIVER                R2A30222  //sun815 use R2A30222
#endif

#define DIRECT_DRIVING_LED          1       //1 : mean direct driving LED
                                            //0 : mean use outside driver

#define TILT_SWITCH                 1       /* 1: Enable tilt control */
                                            /* 0: Disable tilt control */

#define COLLI_SWITCH                1       /* 1: Enable collimator function */
                                            /* 0: Disable colimator function */
#if ((PCB == 710)||(PCB == 711) || (PCB == 815))
#define COLLI_HomeSW                0       /* 1~255: OPU has a Colli home switch, set larger to cover switch noise */
                                            /* 0: OPU does not have Colli home switch */

#define COLLI_ALWAYS_HOMING         0       /* 0: only do colli homing at the first startup or startup error detected */
                                            /* 1: do colli homing every startup */
#else
#define COLLI_HomeSW                5       /* 1~255: OPU has a Colli home switch, set larger to cover switch noise */
                                            /* 0: OPU does not have Colli home switch */

#define COLLI_ALWAYS_HOMING         1       /* 0: only do colli homing at the first startup or startup error detected */
                                            /* 1: do colli homing every startup */
#endif

#if (PCB == 710)||(PCB == 711)
#define COLLI_ANALOG_OUTPUT         0       /* 1: Analog Colli Output */
                                            /* 0: Digital Colli Output (set VFULL = 0, COLLI_JUMP_GAIN = 128, increase COLLI_INIT_INTERVAL & COLLI_EXT_INTERVAL)*/
#else
//PCB == 815
#define COLLI_ANALOG_OUTPUT         1       /* 1: Analog Colli Output */
                                            /* 0: Digital Colli Output (set VFULL = 0, COLLI_JUMP_GAIN = 128, increase COLLI_INIT_INTERVAL & COLLI_EXT_INTERVAL)*/
#endif

#define COLLI_POS_RECOVERY          1       /* 1: Use startup opt. colli pos. if optimum colli pos is far away from startup opt. colli pos. to avoid colli slip */
                                            /* 0: Don't use startup opt. colli pos. */

#if (PCB == 710)||(PCB == 711)
#define COLLI_TEMP_SLOPE            -25     // dOPTIMAL_COLLI(micro step) / dTEMPERATURE(Celcius)
                                            // if COLLI_TEMP_SLOPE != 0, then MONITOR_OPU_TEMPERATURE must be 1
#elif (PCB == 815)
#define COLLI_TEMP_SLOPE            46      // dOPTIMAL_COLLI(micro step) / dTEMPERATURE(Celcius) or 14.6steps/10degreeC
                                            // if COLLI_TEMP_SLOPE != 0, then MONITOR_OPU_TEMPERATURE must be 1
#else
#define COLLI_TEMP_SLOPE            0       // dOPTIMAL_COLLI(micro step) / dTEMPERATURE(Celcius)
                                            // if COLLI_TEMP_SLOPE != 0, then MONITOR_OPU_TEMPERATURE must be 1
#endif

#if (PCB == 711)
#define HALF_COLLI_REST_TIME        1       /* 1: Use half colli rest time */
                                            /* 0: Use original colli rest time according to Sanyo spec. */
#else
#define HALF_COLLI_REST_TIME        0       /* 1: Use half colli rest time */
                                            /* 0: Use original colli rest time according to Sanyo spec. */
#endif

#define LCD_SWITCH                  0       /* 1: Enable LCD function */
                                            /* 0: Disable LCD function */

#if ((PCB == 710)||(PCB == 711) || (PCB == 815))
#define OPU_HomeSW                  0       /* 1~255: Mechanism has OPU home switch, set larger to cover switch noise */
                                            /* 0: Mechanism does not have OPU home switch */

#define ALWAYS_HOMING               0       /* 0: only do homing at the first startup or startup error detected */
                                            /* 1: do homing every startup */
#else
#define OPU_HomeSW                  1       /* 1~255: Mechanism has OPU home switch, set larger to cover switch noise */
                                            /* 0: Mechanism does not have OPU home switch */

#define ALWAYS_HOMING               1       /* 0: only do homing at the first startup or startup error detected */
                                            /* 1: do homing every startup */
#endif

#if (PCB == 815)
#define DOUBLE_LENS                 0       /* 0: single lens */
                                            /* 1: double lens */
#else
#define DOUBLE_LENS                 1       /* 0: single lens */
                                            /* 1: double lens */
#endif

#define EXTERNAL_SDRAM_TYPE  M12L32162A7TG  // ENTER: Select one of the defined External SDRAMs
                                            // Note! This setting is ignored if Sunext chip has "embedded" SDRAM

#define BDROM_PDICGAIN_H            0       //1: mean BDROM use PDIC high gain
                                            //0: mean BDROM use PDIC low gain

#define OSCEN_POLARITY              0       /* 0: Positive OSCEN OPU, HFM on when OSCEN output is high */
                                            /* 1: Negtive  OSCEN OPU, HFM off when OSCEN output is high */
/* End of PCB/Mechanism configuration */


/********************************************/
/*             Block Enables                */
/********************************************/
#define DEBUG_LASER_OPEN_LOOP       0       /* 1: Laser control operates in open loop */
                                            /* 0: Normal close loop */

#define DEBUG_LASER_CLOSE_LOOP      1       /* 1: Laser control operates in close loop */
                                            /* 0: Open loop: MAKING SURE DEBUG_LASER_OPEN_LOOP is 1 */

#define DEBUG_EJECT_ON_BLANK_DISK   0       /* 0: Blank (HF) detection does not work yet */

#if ((PCB == 710)||(PCB == 711)|| (PCB == 815))
#define AUDIO_PLAYBACK              1       /* 1: Drive supports Analog and Digital Audio Playback */
                                            /* 0: Drive does not support Analog and Digital Audio Playback */
                                            /* NOTE: Drive will still support Audio Extraction and transfer to host */
#else
#define AUDIO_PLAYBACK              0       /* 1: Drive supports Analog and Digital Audio Playback */
                                            /* 0: Drive does not support Analog and Digital Audio Playback */
                                            /* NOTE: Drive will still support Audio Extraction and transfer to host */
#endif

#define DIG_AUDIO                   0       /* 1: Enable Digital Audio Playback */
                                            /* 0: Disable Digital Audio Playback */

#define BD_READ                     1       /* BD Read Enable switch */
                                            /* 1: Enable BD read functions */
                                            /* 0: Disable BD read functions */

#define HD_READ                     0       /* HD Read Enable switch */
                                            /* 1: Enable HD read functions */
                                            /* 0: Disable HD read functions */

#define EN_WOBBLE                   1       /* Reader Only Wobble Enable switch */
                                            /* This switch will enable wobble functionality required for Readers Only and is independent of any Encoder enable */
                                            /* 1: Enable Reader Only wobble functionality */
                                            /* 0: Wobble functionality Enable/Disable controlled by CD/DVD/BD/HDVDD_WRITE conditionals */

#define BD_ENABLE                   (BD_READ)

#define ENABLE_HDDVD                (HD_READ)

#define ENABLE_AACS                 1

#define ENABLE_AACS_BEF             0       /* 1: Enable AACS Bus Encryption, 0: Disable AACS Bus Encryption */

#define EN_FW_SIGNATURE             0       /* 1: Enable FW Signature  */
                                            /* 0: Disable FW Signature */

#define BAPASS_AACS_AUTH            1       /* 1: Accept the host to access the disc regardless of the completion of authentication */

#define ENABLE_AACS_FEATURE         0       //[S07]/* 1: Enable AACS Feature, for PC authentication */
                                            /* 0: Disable AACS Feature, for CE products      */

/****************************************************************/
/* A variety of applications for this feature are listed below. */
/* BRCM CE authentication : should NOT support AACS Feature.    */
/* BRCM PC authentication : should support AACS Feature.        */
/* Power DVD              : don't care.                         */
/* WinDVD                 : should support AACS Feature.        */
/* ------------------------------------------------------------ */
/* For CE product, this switch should be OFF as default. But if */
/* you want to play BD movie on BRCM PC auth. Backend, you need */
/* to turn it ON manually.                                      */
/****************************************************************/
#if (PCB == 815)
#define FOOFFSET2_ENABLE            1       /* 1: use DSP FOOFFSET2, -512~511 */
                                            /* 0: ise SVO FOOFFSET , -128~127 */
#else
#define FOOFFSET2_ENABLE            0       /* 1: use DSP FOOFFSET2, -512~511 */
                                            /* 0: ise SVO FOOFFSET , -128~127 */
#endif

/*********** SFLash Cache Defines ************/
#define DISABLE_CACHE               0       /* Disable Cache block */
#define ENABLE_CACHE_4K             4       /* Enable Cache block with 4K Cache size */
#define ENABLE_CACHE_8K             8       /* Enable Cache block with 8K Cache size - WARNING: This setting requires a lot of code change */
/*********************************************/

/*********** SATA Features ************/
#define ENABLE_SATA_FE              1       /* SATA features (SSP,IPM,AN) selection */
                                            /* 1: enable */
                                            /* 0: disable */
/*********** End of SATA Features ************/

/* End of Block Enables */

/********************************************/
/*            ARM Peripheral clock          */
/********************************************/
#define PERIPHERAL_CLK_25MHZ        1       /* 1 peripheral clock = 25MHz  */
                                            /* 0 peripheral clock = 33MHz (Servo PLL default) */


/********************************************/
/*              Work Arounds                */
/********************************************/
#define DeadLoop_Solution           1       /*? Description */

#define HARDCODED_FORCE_DISC        0

/* End of Work Arounds */


/********************************************/
/*              Features                    */
/********************************************/
#define OEM                         0       /* 0--SUNEXT    */
                                            /* 1            */
                                            /* 2            */
                                            /* 3            */

#define EN_LS                       0       /* 0: Disable Feature 2 */
                                            /* 1: Enable Feature 2 */

#define USE_TWO_KEY_FUNCTION        0       /* 0: No play key defined */
                                            /* 1: Enable play key */

#define BURN_IN                     0       /* 1: Enable Burn In function */
                                            /* 0: Disable Burn In function */

#if (CEProduct == 1)
#define FLASH_BY_DISC               1       /* 1: Enable Flash ROM Update from Disc */
                                            /* 0: Disable Flash ROM Update from Disc */
#else
#define FLASH_BY_DISC               0       /* 1: Enable Flash ROM Update from Disc */
                                            /* 0: Disable Flash ROM Update from Disc */
#endif

#define ENABLE_UP_DOWN_JUMP_CONTROL 0       /* 0: no Up_DOWN compensation for small actuator jump overshoot */
                                            /* 1: Enables UP_DOWN compensation (double jump) for small actuator jumps */

#define DISCID_RECOVERY             1       /* 1: Double check dual layer media type by reading LPP/ADIP information,
                                                  if detected as +R disc but cannot read ADIP then will change media type to -R and try to read LPP once
                                                  if detected as -R disc but cannot read LPP then will change media type to +R and try to read ADIP once*/
                                            /* 0: Don't verify LPP/ADIP information for plus_minus detection */

#define FOCUS_SERVO_INPUT_GAIN      256     // gain = 256 or 128

#define TRACKING_SERVO_INPUT_GAIN   128     // gain = 256 or 128

#define FOOTILT_DIFFERENTIAL        1       /* 1 : for differential tilt/FOO OPU use */
                                            /* 0 : for others OPU use */

#define Monitor_FPDO_Linearity      0       /* 1: Use Dry write test function to do FPDO Linearity Experiment.*/

#if (PCB == 690)//690 not tuned
#define SUPPORT_DPD                 0       /* 1: Support DPD operation mode on CD discs */
                                            /* 0: Not support DPD operation mode on CD discs */
#else
#define SUPPORT_DPD                 1       /* 1: Support DPD operation mode on CD discs */
                                            /* 0: Not support DPD operation mode on CD discs */
#endif

#if (PCB == 711)
#define CD_MB_ONLY_OPU              1       /* 1: CD OPU only support main beam */
                                            /* 0: CD OPU support main and sub beam*/
#else
#define CD_MB_ONLY_OPU              0       /* 1: CD OPU only support main beam */
                                            /* 0: CD OPU support main and sub beam*/
#endif

#define BACKGROUND_READ_AHEAD       1       /* 0: Normal read ahead operation - Host need to send the set read ahead commnad */
                                            /* 1: Always read ahead from buffer stop point when decoder is stop and buffer is empty */

#define IGNORE_ERROR_FOR_AUDIO      0       /* 0: Retry on C2 error detected */
                                            /* 1: Ignore C2 error */

#define CDPlayability               1       /* 0: Original play algorithm*/
                                            /* 1: New CD Play algorithm */

#define BY_PASS_C2Error_For_VCD     1       /* 0: Do not by pass C2 Error */
                                            /* 1: By pass C2 Error when C2 Error Count >ByPassC2freq */

#define STREAM_STM                  0       /* 1: Use stream to do STM */
                                            /* 0: Original STM */

#define OPU_BEAM_LANDING_MEAS       1       /* 1: measure beam landing during startup (take 2 revolutions) */
                                            /* 0: don't measure beam landing during startup (faster startup) */

//***BTC  USE Only **************************************
#define SET_SLOT_IN                   0       /* 1: Slot-in*/ //[Q36] SLOT IN define
                                              /* 0: Tray Type */
#if (SET_SLOT_IN == 0)//V[08]
  #define Customer_A1                 1       //Walker
  #define Customer_A2                 0       //Bios
  #define Customer_China              0       //Shin
  #define Customer_A4                 0
  #define Customer_Dune_HD            0       
  #define Customer_U3                 0       //Jam
  #define Customer_ZHD                0       //[W0J]Shin
#else
  #define Customer_A1                 0       //Walker
  #define Customer_A2                 0       //Bios
  #define Customer_China              0       //Shin
  #define Customer_A4                 0
  #define Customer_Dune_HD            0
  #define Customer_U3                 1       //Jam
  #define Customer_ZHD                0       //[W0J]Shin
#endif 
#if(Customer_U3 == 1)//[V08]
	#define MuteSSI                     1       /* 1: (Mute)Disable SSI*/
                                            	/* 0: (On)Normal*/
#else
	#define MuteSSI                     0       /* 1: (Mute)Disable SSI*/
                                            	/* 0: (On)Normal*/
#endif
  #define TG_Factory_Test             0       //for BTC TG use
  
#if(Customer_Dune_HD == 1)//[V08]
  #define READ_SPEED_CONTROL  READ_SPEED_CONTROL_VERSION_ONE //[S1AK] high bit rate play issue
#else
  #define READ_SPEED_CONTROL  READ_SPEED_CONTROL_VERSION_DOT_ONE
#endif 
//***********************************************************
#if (CEProduct == 1)
#define BD_2X_STARTUP               0       /* 0: 1x startup */
                                            /* 1: 2x startup */
#define DelayCDSpeedDown            0       /* 0:Do not delay the CD speed down algorithm */
                                            /* 1: Delay the CD speed down algorithm */
#define ByPassC2freq                3       /* define how many number of C2 occured and bypass the C2 error */

#if (Customer_A1==1)
#define BypassDecoderEr             0       
#else
#define BypassDecoderEr             1       // 1: Ignore decoder errors and pass data to host after retries exhausted
                                            // 0: Report error to host after decoder retries exhausted
                                            // NOTE: This should not be enabled for an IT product!
#endif                                            

//#define READ_SPEED_CONTROL  READ_SPEED_CONTROL_VERSION_DOT_ONE //[V08] Bios

#define SEQ_SEEK_SPEED_DOWN         0       /* 1: Enable speed down to SEEKSPEED during sequence seek/access.*/
                                            /* 0: Disable speed down to SEEKSPEED during sequence seek/access.*/

/*********************************************/
/*  Back End Model                           */
/*********************************************/
#define MTK_BE                      0       /* 1: Enable support of MTK Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of MTK Back End chip */
#if ((PCB==815) || (PCB==711))
#define SIGMA_BE                    1       /* 1: Enable support of Sigma Design Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of Sigma Design Back End chip */
#else
#define SIGMA_BE                    0       /* 1: Enable support of Sigma Design Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of Sigma Design Back End chip */
#endif
#define BCM_BE                      0       /* 1: Enable support of Broadcom Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of Broadcom Back End chip */
#else  //CEProduct = 0

#define BD_2X_STARTUP               0       /* 0: 1x startup */
                                            /* 1: 2x startup */
#define DelayCDSpeedDown            0       /* 0:Do not delay the CD speed down algorithm */
                                            /* 1: Delay the CD speed down algorithm */
#define ByPassC2freq                1       /* define how many number of C2 occured and bypass the C2 error */

#define BypassDecoderEr             0       // 1: Ignore decoder errors and pass data to host after retries exhausted
                                            // 0: Report error to host after decoder retries exhausted
                                            // NOTE: This should not be enabled for an IT product!

#define READ_SPEED_CONTROL  READ_SPEED_CONTROL_VERSION_ONE

    #if (PCB == 710)||(PCB == 711)
#define SEQ_SEEK_SPEED_DOWN         1       /* 1: Enable speed down to SEEKSPEED during sequence seek/access.*/
                                            /* 0: Disable speed down to SEEKSPEED during sequence seek/access.*/
    #else
#define SEQ_SEEK_SPEED_DOWN         0       /* 1: Enable speed down to SEEKSPEED during sequence seek/access.*/
                                            /* 0: Disable speed down to SEEKSPEED during sequence seek/access.*/
    #endif
/*********************************************/
/*  Back End Model                 */
/*********************************************/
#define MTK_BE                      0       /* 1: Enable support of MTK Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of MTK Back End chip */
#define SIGMA_BE                    0       /* 1: Enable support of Sigma Design Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of Sigma Design Back End chip */
#define BCM_BE                      0       /* 1: Enable support of Broadcom Back End chip - CEProduct must also be set */
                                            /* 0: Disable support of Broadcom Back End chip */
#endif  //CEProduct == 1


#define STARTUP_SEARCH_INCOMPLETE_TRK  1    /* 0: binary search DVD+R incomplete track length at Read Disc info/track info */
                                            /* 1: binary search DVD+R incomplete track length at startup */

#define ENABLE_FIQ                  0       /* 0: diable FIQ (remember to set Len_FIQ_Stack, in CSTART.s, to 0)             */
                                            /* 1: enable FIQ (remember to set Len_FIQ_Stack, in CSTART.s, to 128 or greater */
/* end of features */


/********************************************/
/*      Calibration Configuration           */
/********************************************/
#define CALIBRATION_MANAGEMENT      1       // 1 enable calibration management
                                            // 0 disable calibration management

#define DPD_COMPENSATE_OFFSET       1       // 1 enable on-line calibration of TE offset to compensate DPD TE offset
                                            // 0 disable on-line calibration of TE offset to compensate DPD TE offset

#define DPD_COMPENSATE_OFFSET_DIAG  1       // 1 enable on-line calibration of TE offset to compensate DPD TE offset in DIAG error test
                                            // need open  DPD_COMPENSATE_OFFSET first
                                            // 0 disable on-line calibration of TE offset to compensate DPD TE offset in DIAG error test

#define MPPBG_SPPBG_CALIB           2       /* 2: enable NEW MPPBG/SPP1BG calibration */
                                            /* 1: enable MPPBG/SPP1BG calibration */
                                            /* 0: disable MPPBG/SPP1BG calibration */

#define USE_CD_OFFLINE_FBDAC        0       /* 0: NO Use CD Offline Focus Balance DAC */
                                            /* 1: Use CD Offline Focus Balance DAC */

#define USE_DV_OFFLINE_FBDAC        0       /* 0: No Use DVD Offline Focus Balance DAC */
                                            /* 1: Use DVD Offline Focus Balance DAC */

#define USE_CD_OFFLINE_FEBG         0       /* 0: No Use CD Offline Focus Balance Gain */
                                            /* 1: Use CD Offline Focus Balance Gain */

#define ONLINE_FEBG                 1       /* 0: Disable ON-Line Focus Balance Gain */
                                            /* 1: Enable ON-Line Focus Balance Gain */

#define USE_DV_OFFLINE_FEBG         0       /* 0: No Use DVD Offline Focus Balance Gain */
                                            /* 1: Use DVD Offline Focus Balance Gain */

#define USE_DV_OFFLINE_TILTDAC      0       /* 0: No Use DVD Offline Tilt DAC */
                                            /* 1: Use DVD Offline Tilt DAC */

#define EN_Persistent_table_8       1       /* 0: Disable Persistent table 8 */
                                            /* 1: Enable Persistent table 8 */

#if ((PCB == 710) ||(PCB == 711)|| (PCB == 815))
#define MONITOR_OPU_TEMPERATURE     1       /* 0: Disable Monitor OPU Temperature every 30 sec */
                                            /* 1: Enable Monitor OPU Temperature every 30 sec */
#else
#define MONITOR_OPU_TEMPERATURE     0       /* 0: Disable Monitor OPU Temperature every 30 sec */
                                            /* 1: Enable Monitor OPU Temperature every 30 sec */
#endif

#define HOLD_LENS_BEFORE_TRKON      1       /* 0: Do not hold lens before turning Tracking-servo on */
                                            /* 1: Hold lens before turning Tracking-servo on */

#if (PCB == 710)||(PCB == 711)
#define MONITOR_FMD_LEVEL           1       /* 0: Disable monitor FMD level in STM function */
                                            /* 1: Enable monitor FMD level in STM function  */
#else
#define MONITOR_FMD_LEVEL           0       /* 0: Disable monitor FMD level in STM function */
                                            /* 1: Enable monitor FMD level in STM function  */
#endif
/* End of Calibration */

/*****************************************************/
/*       Special Speed Feature definition            */
/*****************************************************/
/* When the we spin down, we init the spin up counter to PLAYBACK_SPD_MIN_TIME.
    When decoder running the counter start to count up 1 ever 20msec,
    if it reach PLAYBACK_SPD_REQ_TIME then we spin up to max speed
*/
#define SPECIAL_SPD_MAX_TIME        (450L)
#define SPECIAL_SPD_REQ_TIME        (200L)  /* about 5.0sec */
#define PLAYBACK_SPD_REQ_TIME       (-400L) /* Spin up target count */
#define PLAYBACK_SPD_MIN_TIME       (-450L) /* Spin up start count */

#define ENABLE_CD_12XCAV            0       /* 0: Disable 12XCAV and enable  16XCAV */
                                            /* 1: Enable  12XCAV and disable 16XCAV */

#define ENABLE_DVD_4XCAV            1       /* 0: Disable 4XCAV */
                                            /* 1: Enable 4XCAV  */

#if (PCB == 711) || (PCB == 710)
#define BD2p2XCLV                  1        /* 1: Support BD 2.2X CLV */
                                            /* 0: Not support BD 2.2X CLV */
#else
#define BD2p2XCLV                  0        /* 1: Support BD 2.2X CLV */
                                            /* 0: Not support BD 2.2X CLV */
#endif

#if (PCB == 815)
#define DVD2p4XCLV                  1       /* 1: Select DVD 2.4X CLV */
                                            /* 0: Select DVD 3.0X CLV */

#define DVD_710Run815               1       /* 1: Select DVD5/9 DVD-R/RW/RDL 2X, 4XCLV, DVD+R/RW/RDL 2.4X, 4XCLV */
                                            /* 0: Use 710 Original speed profile*/

#define DVD_710Run815New            0       /* 1: Select DVD5/9 DVD-R/RW/RDL 2X, 4XCLV, DVD+R/RW/RDL 2.4X, 4XCLV */
                                            /* 0: Use 710 Original speed profile*/
#else
#define DVD2p4XCLV                  0       /* 1: Select DVD 2.4X CLV */
                                            /* 0: Select DVD 3.0X CLV */

#define DVD_710Run815               0       /* 1: Select DVD5/9 DVD-R/RW/RDL 2X, 4XCLV, DVD+R/RW/RDL 2.4X, 4XCLV */
                                            /* 0: Use 710 Original speed profile*/

#define DVD_710Run815New            0       /* 1: Select DVD5/9 DVD-R/RW/RDL 2X, 4XCLV, DVD+R/RW/RDL 2.4X, 4XCLV */
                                            /* 0: Use 710 Original speed profile*/
#endif
#define NEW_SPINDLE_CONTROL         1       /* 1: new spindle control method (32-bit integrator & APC loop) */
                                            /* 0: old spindle control method (16-bit integrator) */

#if (PCB == 692)
#define DVD_692RUN4XCLV             1       /* 1: Enable 4XCLV for Sun692*/
                                            /* 0: Disable 4XCLV for Sun692*/
#else
#define DVD_692RUN4XCLV             0
#endif

/* END of Special Speed Feature definition */


/********************************************/
/*           Defect Management              */
/********************************************/
#define DVD_PLUSRW_BGF              0       /* 0: No DVD+RW background format implementation */

/* END of Defect Management */


/*********************************************/
/*  Playbility Switches                      */
/*********************************************/
#define CD_DFE_IMPROVE_READ         0       /* Changing DFE parameters to improve read ability */

#define WIDER_TOK_LVL_FOR_PLAY_MODE 0       /* Use TOK level to mask RX, in order to avoid track slip */

#if (PCB == 710)||(PCB == 711)||(PCB == 815)
#define DEFECT_LENGTH_MEASURE       1       /* 1: Enable defect length measure */
                                            /* 0: Disable */
#else
#define DEFECT_LENGTH_MEASURE       0       /* 1: Enable defect length measure */
                                            /* 0: Disable */
#endif

#define DVD_MARGINAL_DISC           0       /* 1: Enable Detect marginal disc  functions */
                                            /* 0: Disable */

#define MARGINAL_DISC               0       /* 1: Enable Spin-Down for marginal disc */
                                            /* 0: Disable */

#define XDFCT_ENABLE                0       /* 0: Disable special XDCFT control for OM&T 6.0mm scratch disc */
                                            /* 1: Enable special XDCFT control for OM&T 6.0mm scratch disc */
/* End of Playbility Switches */


/********************************************/
/*        Special Disc Handling             */
/********************************************/

#define SRC_ENABLE                  1       /* 0: Disable SRC;  1: Enable SRC  */

#define SRC_ENABLE_FOCUS            1       /* 0: Disable SRC;  1: Enable SRC  */

#define SRC_RESET_DC_LPF            0       /* 0: Disable reset SRC DC LPF;  1: Enable reset SRC DC LPF */

#define DETECT_ECC_DISC             1       /* Controls detection of Eccentricity discs and limits the Read/Write speeds */
                                            /* 1: Enable the detection of Eccentricity discs */
                                            /* 0: Disable the detection of Eccentricity discs */

#define ELECTRONIC_DAMPER_FOR_ECC_DET  1    /* 1: Enable electronic-damper for ECC detection  */
                                            /* 0: Disable electronic-damper for ECC detection */
#if (PCB == 815)
#define BD_ECC_IMPROVEMENT           1       /* 1: Enable spin down for ECC(100&150um)disc when tracking slip   */
                                            /* 0: Disable spin down for ECC(100&150um)disc when tracking slip  */
#else
#if (CEProduct == 1)
#define BD_ECC_IMPROVEMENT           0      /* 1: Enable spin down when tracking slip  for ECC DISC */
                                            /* 0: Disable spin down when tracking slip  for ECC DISC */
#else
#define BD_ECC_IMPROVEMENT           0       /* 1: Enable spin down when tracking slip  for ECC DISC */
                                            /* 0: Disable spin down when tracking slip  for ECC DISC */
#endif

#endif
#if (PCB == 710)||(PCB == 711)
#define CD_UNBALANCE_DISC_CHECK     1       /* Controls detection of CD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check CE */
                                            /* 1: Enable Unbalance disc check CE */

#define DVD_UNBALANCE_DISC_CHECK    0       //[S07]/* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */ //[V.7] for DVD ready noise
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */

#define BD_UNBALANCE_DISC_CHECK     1       /* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */
#elif (PCB == 815)
#define CD_UNBALANCE_DISC_CHECK     0       /* Controls detection of CD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check CE */
                                            /* 1: Enable Unbalance disc check CE */

#define DVD_UNBALANCE_DISC_CHECK    0       /* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */

#define BD_UNBALANCE_DISC_CHECK     1       /* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */
#else
#define CD_UNBALANCE_DISC_CHECK     0       /* Controls detection of CD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check CE */
                                            /* 1: Enable Unbalance disc check CE */

#define DVD_UNBALANCE_DISC_CHECK    0       /* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */

#define BD_UNBALANCE_DISC_CHECK     0       /* Controls detection of DVD unbalanced discs and limits the Read/Write speeds */
                                            /* 0: Disable Uunbalance Disc check */
                                            /* 1: Enable Unbalance disc check */
#endif

#if (DVD_MARGINAL_DISC == 1)
#define DVD_OFF_TRACK_TRIGGER       1       /* 1: Enable  Off Track detection during writing */

#define DETECT_DVD_FE_DISTURB       1       /* 1: Enable Focus Error Disturbance Detection during writing */
                                            /* 0: Disable */

#else /* MARGINAL_DISC=0 */
#define DVD_OFF_TRACK_TRIGGER       0       /* 1: Enable Off Track  detection during writing */
                                            /* 0: Disable Track Panic detection during writing */

#define DETECT_DVD_FE_DISTURB       0       /* 1: Enable Focus Error Disturbance Detection during writing */
                                            /* 0: Disable */
#endif /* DVD_MARGINAL_DISC=1 */

#if (MARGINAL_DISC == 1)
#define FORCE_ATIP_TRIGGER          0       /* 1: Enable Conditional ATIP Time Trigger */

#define DETECT_TE_OFFTRACK          0       /* 1: Enable using TE offtrack Isr for speed down during writing */
                                            /* 0: Disable */

#define DETECT_FE_DISTURB           1       /* 1: Enable Focus Error Disturbance Detection before writing */
                                            /* 0: Disable */
#define DEBUG_FE_DISTURB            1       /* 1: Enable Focus Error Disturbance Detection before writing and during writing */

#define CHECK_ATIP_QUALITY          1       /* 1: Enable checking if Atip quality is Protect or Search mode */
                                            /* 0: Disable */
#else /* MARGINAL_DISC=0 */
#define FORCE_ATIP_TRIGGER          0       /* 1: Enable Conditional ATIP Time Trigger */

#define DETECT_TE_OFFTRACK          0       /* 1: Enable using TE offtrack Isr for speed down during writing */
                                            /* 0: Disable */

#define DETECT_FE_DISTURB           0       /* 1: Enable Focus Error Disturbance Detection before writing */
                                            /* 0: Disable */

#define DEBUG_FE_DISTURB            0       /* 1: Enable Focus Error Disturbance Detection before writing and during writing */

#define CHECK_ATIP_QUALITY          0       /* 1: Enable checking if Atip quality is Protect or Search mode */
                                            /* 0: Disable */
#endif /* MARGINAL_DISC=1 */

/* End of Special Disc Handling */


/*----------------------------------------------------------------------------
    DVD-RAM DETECTION
-----------------------------------------------------------------------------*/
#if (PCB == 710)||(PCB == 711)
#define ENABLE_DVDRAM_DETECTION               1       /* 1: Enable DVD-RAM detection. */
                                                      /* 0: Disable DVD-RAM detection. */
#else
#define ENABLE_DVDRAM_DETECTION               0       /* 1: Enable DVD-RAM detection. */
                                                      /* 0: Disable DVD-RAM detection. */
#endif

/*----------------------------------------------------------------------------
    DVD-RAM ENABLE
-----------------------------------------------------------------------------*/
#ifdef EN_DVDRAM
#define DVD_RAM_READ                          1       /* 1: Enable DVD-RAM READ */
                                                      /* 0: Disable DVD-RAM READ */
#else
#define DVD_RAM_READ                          0       /* 1: Enable DVD-RAM READ */
                                                      /* 0: Disable DVD-RAM READ */
#endif

    #if (CHIP_REV >= 0xC0)
#define FE_SLEW_RATE_LIMITER                  2       /* 2: Enable HW FE slew-rate limitor */
                                                      /* 1: Enable FE slew-rate limitor */
                                                      /* 0: Disable FE slew-rate limitor */
    #elif (CHIP_REV == 0xB3)
#define FE_SLEW_RATE_LIMITER                  0
    #else
#define FE_SLEW_RATE_LIMITER                  1       /* 1: Enable FE slew-rate limitor */
                                                      /* 0: Disable FE slew-rate limitor */
    #endif



#define DVDRAM_FEBC_ON_FCSO                   1       /* 1: Enable use of AFE FCSO for FE balance calibration. Don't use DSP FEBC */
                                                      /* 0: Disable use of AFE FCSO for FE balance calibration. Use DSP FEBC */

#define ENABLE_RAM_DFE_ICE_DETECT             0       /* 1: Enable DVD-RAM ICE detection*/
                                                      /* 0: Enable DVD-RAM ICE detection*/

#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    #define  DISCARD_RAM_DECODE               1       /* 1: Enable DVD-RAM discard decode */
                                                      /* 0: Disable DVD-RAM discard decode */
#else
    #define  DISCARD_RAM_DECODE               0       /* 1: Enable DVD-RAM discard decode */
                                                      /* 0: Disable DVD-RAM discard decode */
#endif

#define DVD_RAM_CHECK_DEFECT                  0       /* 1: Enable DVD-RAM check defective sector or block */
                                                      /* 0: Disable DVD-RAM check defective sector or block */

#define DVD_RAM_CERTIFIED_FLAG_DETECTION      0       /* 1: Enable DVD-RAM certified flag detection */
                                                      /* 0: Disable DVD-RAM certified flag detection */

#define WORKAROUND_FOR_SEEDMISTHR             1       /* 1: Enable DVD-RAM WORKAROUND for bug 5538 */
                                                      /* 0: Disable DVD-RAM WORKAROUND for bug 5538 */

/* End of DVD-RAM */


/*********************************************/
/*  China Blue HD ENABLE                           */
/*********************************************/
#define ENABLE_CBHD                 0       /* 1: support China Blue HD */
                                            /* 0: not support China Blue HD */

/*****************************************************/
/*  Inquiry and Identify Drive strings              */
/*****************************************************/
#define MODEL_NAME_FROM_EEPROM      0       /* model name source */
                                            /* 0: from oem_info.h, 1: from EEPROM */

/*****************************************************/
/*    Vendor Specific Identify Device Strings     */
/*****************************************************/
/*---------------------------------------------------*/
/*  Identify Table - Serial Number ( USHORT 10 ~ 19) */
/*  (BYTE swap)                                   */
/*---------------------------------------------------*/
#define FIRMW_ID_SN                 FIRMW_INQ_DATETIME
/*---------------------------------------------------*/
/* Identify Table - Model Number ( USHORT 27 ~ 31)  */
/* (BYTE swap)                                      */
/*---------------------------------------------------*/


/*****************************************************/
/*        Vendor Specific Inquiry Strings         */
/*****************************************************/
/*---------------------------------------------------*/
/*  Inquiry Table - Vendor ID (BYTE 08 ~ 15)         */
/*                                                  */
/*---------------------------------------------------*/



/*---------------------------------------------------*/
/*  Inquiry Table - Product ID (BYTE 16 ~ 31)       */
/*                                                  */
/*---------------------------------------------------*/


/********************************************************/
/*  Update FW by Disc String  */
/********************************************************/
#define DISC_FW_UPDATE_STRING       FIRMW_DISC_UPDATE_STR

/********************************************/
/*  Read/Write Speed Defines                */
/********************************************/
/*********************************************/
/*  Write Protect                            */
/*********************************************/
#define WRITE_PROTECT               0       //DVD-RW Write Protect huang test modify wrtie protect status 960130

/*********************************************/
/*  VCPS                                     */
/*********************************************/
//#define VCPS_TEST                   0           /* DVD+R/RW Video Content Protection System */

/*********************************************/
/*  Read BCA                                 */
/*********************************************/
#define BCA_ENABLE                  1       /* Read BCA */

/*********************************************/
/*  CPRM                                     */
/*********************************************/
#define ENABLE_CPRM                 1       /* DVD-R/RW/RAM Video Content Protection System */
                                            /* Because some information in BCA is required  */
                                            /* for CPRM, we turn on this conditional only   */
                                            /* when BCA_ENABLE is set to 1.                 */

#define ENABLE_BDAV                 1       /* BDAV: BD-R/RE disc with AACS protection  */
                                            /* 1: Support BDAV disc */
                                            /* 0: Do not support BDAV disc */

#if (CEProduct == 0)
#define ENABLE_AVCREC               0       /* AVCREC: DVD+-R/RW disc with AACS protection  */
                                            /* 1: Support AVCREC disc */
                                            /* 0: Do not support AVCREC disc */

#define Detect_AVCHD_DISC           0
#else
#define ENABLE_AVCREC               1       /* AVCREC: DVD+-R/RW disc with AACS protection  */
                                            /* 1: Support AVCREC disc */
                                            /* 0: Do not support AVCREC disc */

#define Detect_AVCHD_DISC           1
#endif

/*********************************************/
/*  DVD Not Speed Up                                              */
/*********************************************/
#define DISABLE_CD_WRITE_CMD        0       /* 1: Disable CD Write Command, temporary TBD? */
                                            /* 0: Enable  CD Write Command */

/*********************************************/
/*  DVD Support Write Ability                */
/*********************************************/
#define ALL_DISC                    0       /* Plus Minus Double Layer Disc Support */ //TBD? this should be moved to vu_cfg.h. debug!
#define P_DL_DISC                   1       /* Plus Only Double Layer Disc Support  */
#define PM_SL_DISC                  2       /* Plus Minus Single Layer Disc Support */
#define P_SL_DISC                   3       /* Plus Only Single Layer Disc Support  */

#define SUPPORT_DVD_WRITE_ABILITY  ALL_DISC

#define READ_MODIFY_WRITE           0       /* 0: not enable read and modify write function */
                                            /* 1: enable read back / pad zero when write at non ECC boundary for DVD+/-RW disc */


/*********************************************/
/*  SACD                                     */
/*********************************************/
#define SACD                        1       /* 0: Disable SACD function*/
                                            /* 1: Enable SACD function*/
#define DDisc                       1       /* 0: Disable DDisc function, AFE_CALC_CDROM_FE_TH is the threshold for maxFEpp_0dB from svoPrvtVar_maxFEpp */
                                            /* 1: Enable DDisc function, AFE_CALC_CDROM_FE_TH is the threshold for maxFEpp_0dB from svoPrvtVar_avgSRFO[0] */
#define NODISC_RETRY                0       //[V08] Bios/* 0: Disable nodisc_retry function*/
                                            /* 1: Enable nodisc_retry function*/


/*********************************************/
/*  BDRLTH                                   */
/*********************************************/
#define ENABLE_BDRLTH               1       // 0: not support BDRLTH disc
                                            // 1: support BDRLTH disc

/*********************************************/
/*  BDIH                                     */
/*********************************************/
#define ENABLE_BDIH                 0       // 0: not support BDIH disc //2009.0925 for BDIH disc
                                            // 1: support BDIH disc

#define FORCE_READ_BDIH             0

/********************************************/
/*  Diagnostics Conditionals                */
/********************************************/
#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define SET_BDW_DISC_DEFAULT_DATA   1       /* 1- set default data for DMA/DFL/SRRI in BD R/RE when DMA fail but PIC ok. */

#define STORE_MSG_TO_SDRAM          1       /* store debug message to dram */

#define STORE_MSG_TO_FLASHROM       1       /* store debug message to flashrom */

#define HOLD_PIN_CONTORL_MSG        0       /* 1- enable hold pin download message*/

#define TRAVERSE_TEST               1       /* 0: normal operation */

#if (PCB == 710)
#define SUN_FUNC_TEST               1       /* 1- enable function test */
#else
#define SUN_FUNC_TEST               0       /* 1- enable function test */
#endif

#else
#define SET_BDW_DISC_DEFAULT_DATA   1       /* 1- set default data for DMA/DFL/SRRI in BD R/RE when DMA fail but PIC ok. */

#define STORE_MSG_TO_SDRAM          0       /* store debug message to dram */

#define STORE_MSG_TO_FLASHROM       0       /* store debug message to flashrom */

#define HOLD_PIN_CONTORL_MSG        0       /* 1- enable hold pin download message*/

#define TRAVERSE_TEST               1       /* 0: normal operation */

#define SUN_FUNC_TEST               0       /* 1- enable function test */

#endif
/*********************************************/
/*  Collimator position                      */
/*********************************************/
#define USE_EEPROM_COLLI_POS        0           /* 0: Use barcode collimator position */
                                                /* 1: Use EEPROM collimator position  */

/********************************************/
/*  OEM table Conditionals                */
/********************************************/
#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
    #define SERVOKEYPARAMETER       1       /* 1: enable this function  */
#else
    #define SERVOKEYPARAMETER       0       /* 1: enable this function  */
#endif

#define MAIN_CODE_SELF_CHECK        1       /* 1: enable main code self check sum protect*/
/*********************************************/
/*  Power Saving Function                    */
/*********************************************/
#define EN_POWER_CONTROL            1       /* 1: Enable Power control via the PowerControl() */
                                            /* 0: Disable Power control via the PowerControl() */

/*********************************************/
/*  Tray Profile Control                     */
/*********************************************/
#define SUN710_TRAY_PROFILE         0       /* 0: Original tray control */
                                            /* 1: 710 tray control */

/*********************************************/
/*  Wobble     check                         */
/*********************************************/
#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define BD_WOBBLE_DET               1       /* 0: Original detection method */
                                            /* 1: Checking wobble pll locking status to detect BD disc type */
#else
#define BD_WOBBLE_DET               0       /* 0: Original detection method */
                                            /* 1: Checking wobble pll locking status to detect BD disc type */
#endif
/************************************************************/
/*  Disc kind/type mis-detection Recovery function          */
/************************************************************/
#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define DISC_MIS_JUDGE_RECOVERY     1       /* 0: Original flow */
                                            /* 1: Do servo recovery if mis-detection */
#else
#define DISC_MIS_JUDGE_RECOVERY     0       /* 0: Original flow */
                                            /* 1: Do servo recovery if mis-detection */
#endif
#define DISC_MIS_JUDGE_START_UP_RECOVERY  (1&NEW_PIC_HOME_FLOW)  /* 0: Original flow */
                                                                 /* 1: Do servo recovery if mis-detection */

#define CDDVDNewBlankChk            1       /* 0: original flow *///Implement "CDDVDNewBlankChk" to improve blank CD/DVD Judge
                                            /* 1: Add protection to prevent judging Blank DVD-R/RW/DL's Embossed Area as recorded Disc */

#define NEW_SERVO_CALIB_FLOW        1       /* 0: original flow */
                                            /* 1: do COLLI, TILT, FEBC calibrations right after blank check when tracking off */

/************************************************************/
/*  DVD random read hanging problem work-around             */
/************************************************************/
#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define RANDOM_READ_BYPASS_CHECK_METHOD         1
#else
#define RANDOM_READ_BYPASS_CHECK_METHOD         0
#endif

/*****************************************************************************/
/*  Workaround                                                               */
/* #5530  PO uncorrectable happens at the first ECC block of a zone          */
/* #5538  EDC error but no PO uncorrectable                                  */
/* #5540  CD encode address offset one block                                 */
/* #5637  Wrong fine seek kick pulse caused by incorrect half pulse          */
/* #5739  Wobble PLL frequency is wrong when DVD Download disc is loaded     */
/* #5802  LPP is clipped after VGA3                                          */
/* #5924  use BLANKBSKG/BLANKBSKO instead of BSKG/BSKO during write          */
/* #5948  tacho mult lock-up                                                 */
/* #5964  Peak detectors of digital beta measurement do not work normally    */
/* #5966  Wrong LAND signal toggle after enabling buffering                  */
/*****************************************************************************/
    #if (CHIP_REV >= 0xC1)
#define WORKAROUND_FOR_BUG_5530     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5538     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5540     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5637     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5739     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5746     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5802     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5924     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5948     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5964     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5966     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_B3_CHIP      0       /* 0: Disable; 1: Enable */
#define DIPM_BY_FW                  0       /* 0: Disable; 1: Enable */
#define OSCEN_WORKAROUND            1       /* 0: Disable; 1: Enable */
    #elif (CHIP_REV == 0xC0)
#define WORKAROUND_FOR_BUG_5530     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5538     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5540     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5637     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5739     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5746     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5802     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5924     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5948     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5964     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5966     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_B3_CHIP      0       /* 0: Disable; 1: Enable */
#define DIPM_BY_FW                  0       /* 0: Disable; 1: Enable */
#define OSCEN_WORKAROUND            1       /* 0: Disable; 1: Enable */
    #else // CHIP_REV <= 0xB3
#define WORKAROUND_FOR_BUG_5530     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5538     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5540     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5637     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5739     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5746     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5802     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5924     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5948     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5964     0       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_BUG_5966     1       /* 0: Disable; 1: Enable */
#define WORKAROUND_FOR_B3_CHIP      1       /* 0: Disable; 1: Enable */
#define DIPM_BY_FW                  1       /* 0: Disable; 1: Enable */
#define OSCEN_WORKAROUND            0       /* 0: Disable; 1: Enable */
    #endif
/************************************************************/
/*  Hit Disc Protection                                     */
/************************************************************/
    #if ((CHIP_REV >=0xC0)||(CHIP_REV==0xB3))
#define NEW_FDP_DETECTION           1       /* 0: Use FOK_check() to detect focus drop (not using interrupt) */
                                            /* 1: Use focus drop interrupt to preset FOO level */
                                            /* 2: Use DSP TNG_preset (programable FNG time) & DSP interrupt */
    #else
#define NEW_FDP_DETECTION           2       /* 0: Use FOK_check() to detect focus drop (not using interrupt) */
                                            /* 1: Use focus drop interrupt to preset FOO level */
                                            /* 2: Use DSP TNG_preset (programable FNG time) & DSP interrupt */
    #endif

#define FOCUS_LOCK_FROM_DOWN_TO_UP  0       /* 0: Original focus capture flow */
                                            /* 1: Focus capture on upper layer by ramping from down to up */

/************************************************************/
/*  Others                                                  */
/*                                                          */
/************************************************************/
#define PDIC_BASE_ON_SRFO           1       /* 1: chose PDIC gain mode based on reflectivity */
                                            /* 0: chose PDIC gain mode based on disc kind */

#if ((PCB == 710) || (PCB == 711) || (PCB == 815))
#define NEW_MSG_SW                  1       /* 0: Old debug message */
                                            /* 1: New debug message */

#define EN_ADEQ                     1       /* 1: enable ADAPTIVE EQ */

#define EN_BD_ADEQ                  1       /* 1: Enable ADEQ for BD */
                                            /* 0: Don't enable ADEQ for BD */

#define STEP12PWM                   1       /* 0: use PDM */
                                            /* 1: use PWM */

#define WOB_BASED_DEMODULATOR       1       /* 1: Force demodulator to be in run in mode based on wobble if 3 syncs in the run in were missed */
                                            /* 0: Don't force demodulator to be in run in mode based on wobble if 3 syncs in the run in were missed */
#if (CHIP_REV >= 0xC0)
#define BDROM_USE_DPP               1       /* 2: BDROM SL tracking method use DPP at 10X and 12X */

#define DVD_RETRY_USE_DPD           0       /* 1: tracking method use DPD instead of DPP when read retry*/
#define BD_RETRY_USE_DPD            1       /* 1: tracking method use DPD instead of DPP when read retry*/
        #else
#define BDROM_USE_DPP               0
#define DVD_RETRY_USE_DPD           0
#define BD_RETRY_USE_DPD            0
        #endif

#define EN_DVD_HIGH_JITTER_DETECT   1       /* 1: enable HIGH JITTER detect at svo_ok for DVDRDL*/

#define RETRY_CHG_EQTAPA12          1      //20090515_x.30 /* 1: enable BDREDL Read retry EQTAPA1/2 */
#define EN_DVDROM_HIGH_JITTER_READ_METHOD  1       /* 1: enable HIGH JITTER Read Method at svo_ok for DVDRDL*/
#else
#define NEW_MSG_SW                  0       /* 0: Old debug message */
                                            /* 1: New debug message */

#define EN_ADEQ                     0       /* 1: enable ADAPTIVE EQ */

#define EN_BD_ADEQ                  0       /* 1: Enable ADEQ for BD */
                                            /* 0: Don't enable ADEQ for BD */

#define STEP12PWM                   0       /* 0: use PDM */
                                            /* 1: use PWM */

#define WOB_BASED_DEMODULATOR       0       /* 1: Force demodulator to be in run in mode based on wobble if 3 syncs in the run in were missed */
                                            /* 0: Don't force demodulator to be in run in mode based on wobble if 3 syncs in the run in were missed */

#define EN_DVD_HIGH_JITTER_DETECT   0       /* 1: enable HIGH JITTER detect at svo_ok for DVDRDL*/

#define RETRY_CHG_EQTAPA12          0      //20090515_x.30 /* 1: enable BDREDL Read retry EQTAPA1/2 */
#define EN_DVDROM_HIGH_JITTER_READ_METHOD  0       /* 1: enable HIGH JITTER Read Method at svo_ok for DVDRDL*/
#endif



//Added HIF detect skating flow.
#define HIF_DETECT_SKATING          1       /* 1: Enable HIF detect skating during target serach.*/

#define DVDDL_TILT_CAL_PRO          0       /* 0: Don't enable TILT calibration protation. */
                                            /* 1: enable TILT calibration protation.*/


////////////////////////////////////////////
#if (CHIP_REV >= 0xB2)
#define B1_CHIP_SATA_WA             0
#else
#define B1_CHIP_SATA_WA             1
#endif // CHIP_REV >= 0xB2
#define SIGMA_SINGLE_BOARD          0       /* 1: SIGMA Design single board */
                                            /* 0: Disable Burn In function */
#if(PCB == 710)
#define EPO_SLOT_IN                 1       /* 1: EPO slot in meka , 0: others */
#else
#define EPO_SLOT_IN                 0       /* 1: EPO slot in meka , 0: others */
#endif

#define EEPROM_TRAY_PROFILE         0      /* 1: Tray profile in EEPROM */

#define FDP_TRKSLIP_MONITOR         1       /* 0: Don't monitor focus drop & track slip frequency */
                                            /* 1: Monitor focus drop & track slip frequency       */

#define ERROR_CAL_START_UP_MONITOR  1       /* 0: Don't monitor calibration error during start up */
                                            /* 1: Monitor calibration error during start up       */

#define SUPPORT_REMOVABLE_DISK_PROFILE  1   /* 1: ROM drive supports RDP, for WIN7/WHQL/Read Capacity Test Case. */
                                            /* 0: ROM drive does not support RDP, for FWTester2.  */

#define BD_USE_TE_PP_DPD            1       /* 1: USE TEpp (PP+-DPD) to judge ROM/R(E)/LTH disc */
                                            /* 0: Not to USE TEpp (PP+-DPD) to judge ROM/R(E)/LTH disc */

#define REPLACE_FEBC_WITH_FEO       1       /* 1: Replace FEBC with FEO (FE DSP offset) */
                                            /* 0: use FEBC */

#define LABELTAG_ENABLE             0       // 1: Enable LABELTAG function

#if(PCB == 815)
#define NEW_MPPSPPTE_BG_CALI        1       /* 1: K MPP/SPPBG before CEBG,TEBG */
                                            /* 0: K CEBG,TEBG before MPP/SPPBG */

#define BARCODE_APPKt               0       /* 0: Don't use barcode APPKt value */
                                            /* 1: use barcode APPKt value       */

    #if (LABELTAG_ENABLE == 1)
#define EN_ADI                      0       // must be forced to 0 in this mode due to servo DSP code !!
    #else
#define EN_ADI                      1       // 1: Enable actuator dissipation measurement
                                            // 0: Disable actuator dissipation measurement
    #endif

#else
#define NEW_MPPSPPTE_BG_CALI        0       /* 1: K MPP/SPPBG before CEBG,TEBG */
                                            /* 0: K CEBG,TEBG before MPP/SPPBG */

#define BARCODE_APPKt               0       /* 0: Don't use barcode APPKt value */
                                            /* 1: use barcode APPKt value       */

    #if (LABELTAG_ENABLE == 1)
#define EN_ADI                      0       // must be forced to 0 in this mode due to servo DSP code !!
    #else
#define EN_ADI                      0       // 1: Enable actuator dissipation measurement
                                            // 0: Disable actuator dissipation measurement
    #endif
#endif

/*********************************************/
/*  Drive Certification                      */
/*********************************************/
#define EN_DRIVE_CERT   1                         // 0: Not support Drive Certification function
                                                  // 1: Support Drive Certification function


/*********************************************/
/*  C0/C1 new feature                      */
/*********************************************/
#define WRITE_MBSBVREF_OS           0       /* 1: Enable WRITE_MBSBVREF_OS (prepare to instead WORKAROUND_FOR_TE_UNBALANCE)*/
                                            /* 0: Disable WRITE_MBSBVREF_OS */

#endif /* __OEM_INFO_H__ */
