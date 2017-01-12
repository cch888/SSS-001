/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* $Revision: 134 $
* $Date: 11/04/06 2:04p $
*
* Description:
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#ifndef __HWDEF_H__
#define __HWDEF_H__

#include ".\oem\Oem_info.h"

struct st_GPIO {                                        /* struct of GPIO    */
             union {
                   BYTE BYTE;                  /*  Data register */
                   struct {                             /*  Bit  Access */
                          BYTE B0:1;           /*    Bit 0     */
                          BYTE B1:1;           /*    Bit 1     */
                          BYTE B2:1;           /*    Bit 2     */
                          BYTE B3:1;           /*    Bit 3     */
                          BYTE B4:1;           /*    Bit 4     */
                          BYTE B5:1;           /*    Bit 5     */
                          BYTE B6:1;           /*    Bit 6     */
                          BYTE B7:1;           /*    Bit 7     */
                          }      BIT;                   /*              */
                   } DATA;                              /* data register of GPIO */
             union {
                   BYTE BYTE;                  /*  Control register */
                   struct {                             /*  Bit  Access */
                          BYTE B0:1;           /*    Bit 0     */
                          BYTE B1:1;           /*    Bit 1     */
                          BYTE B2:1;           /*    Bit 2     */
                          BYTE B3:1;           /*    Bit 3     */
                          BYTE B4:1;           /*    Bit 4     */
                          BYTE B5:1;           /*    Bit 5     */
                          BYTE B6:1;           /*    Bit 6     */
                          BYTE B7:1;           /*    Bit 7     */
                          }      BIT;                   /*              */
                   } CTRL;                              /* control diection of GPIO */
            };                                                      /*              */

/* Port A and B Addresses */
#define PA      (*(volatile struct st_GPIO *) (&(HAL_PERIPHREG_MAP->PADATA))) /* PA    Address*/
#define PB      (*(volatile struct st_GPIO *) (&(HAL_PERIPHREG_MAP->PBDATA))) /* PB    Address*/
#define PC      (*(volatile struct st_GPIO *) (&(HAL_PERIPHREG_MAP->PCDATA))) /* PC    Address*/

/*------------------------------------------------------;
;               [ P O R T   A ]                         ;
;------------------------------------------------------*/
#if (PCB == 690)
    #define ipOpenSW        PA.DATA.BIT.B7  // Tray Open Switch
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define ipCloseSW       PA.DATA.BIT.B5  // Tray Close Switch
    #define opMMUTE         PA.DATA.BIT.B4  // Motor Mute
    #define opSMUTE         PA.DATA.BIT.B3  // Sled Mute
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define opDRIVE_ON      PA.DATA.BIT.B0  // BA5912BFP mute1/2
#elif (PCB == 1600)
    #define opMMUTE         PA.DATA.BIT.B7  // Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Fast Open
    #define opSMUTE         PA.DATA.BIT.B5  // Sled Mute
    #define ipALPCOSOUT     PA.DATA.BIT.B4  // COL_COS
    #define ipALPSINOUT     PA.DATA.BIT.B3  // COL_SIN
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define ipTRAYCEN       PA.DATA.BIT.B0  // TRAYCEN
#elif (PCB == 691)
    #define ipCloseSW       PA.DATA.BIT.B7  // Tray Close Switch
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define ipOpenSW        PA.DATA.BIT.B5  // Tray Open Switch
    #define opMMUTE         PA.DATA.BIT.B4  // Motor Mute
    #define opSMUTE         PA.DATA.BIT.B3  // Sled Mute
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define opSEL2          PA.DATA.BIT.B0  // OPU SEL2
#elif (PCB == 692)
    #define opMMUTE         PA.DATA.BIT.B7  // Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define opSMUTE         PA.DATA.BIT.B5  // Sled Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define ipSA_INT        PA.DATA.BIT.B0  // OPU SA init point
#elif (PCB == 812)
    #define opMMUTE         PA.DATA.BIT.B7  // Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define opSMUTE         PA.DATA.BIT.B5  // Sled Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define ipSA_INT        PA.DATA.BIT.B0  // OPU SA init point
#elif (PCB == 710)
    #define opMDEN2         PA.DATA.BIT.B7  // opMMUTE Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define opMDEN1         PA.DATA.BIT.B5  // opSMUTE Sled Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"

#if EPO_SLOT_IN
    #define indet           PA.DATA.BIT.B0  // SIGPO , OPU SA init point
#else
    #define ipSA_INT        PA.DATA.BIT.B0  // SIGPO , OPU SA init point
#endif

#elif (PCB == 711)
    #define opMDEN2         PA.DATA.BIT.B7  // opMMUTE Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define opMDEN1         PA.DATA.BIT.B5  // opSMUTE Sled Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"

#if EPO_SLOT_IN
    #define indet           PA.DATA.BIT.B0  // SIGPO , OPU SA init point
#else
    #define opPA0           PA.DATA.BIT.B0  // PA0/GAINSW2
#endif

#elif (PCB == 815)
    #define opMDEN2         PA.DATA.BIT.B7  // opMMUTE Motor Mute
    #define ipEJECT         PA.DATA.BIT.B6  // Eject Interrupt
    #define opMDEN1         PA.DATA.BIT.B5  // opSMUTE Sled Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2  // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1  // Serial Comm TxD "Transmit"
    #define ipOEICSW1       PA.DATA.BIT.B0  // OEICSW1

#elif (PCB == 817)
    #define ipHOME          PA.DATA.BIT.B7    //                     END_DET
    #define ipEJECT         PA.DATA.BIT.B6    // Eject Interrupt     F_EJECT
    #define opMDEN2         PA.DATA.BIT.B5    // opMMUTE Motor Mute
    // #define                 PA.DATA.BIT.B4  // Port dedicated to ALPHACOS COL1
    // #define                 PA.DATA.BIT.B3  // Port dedicated to ALPHASIN COL2
    #if ENABLE_UART_RXD
    #define ipURXD          PA.DATA.BIT.B2    // Serial Comm RxD "Recieve"
    #endif
    #define opUTXD          PA.DATA.BIT.B1    // Serial Comm TxD "Transmit"

    #define opMDEN1         PA.DATA.BIT.B0    // opSMUTE Sled Mute   DRV_CTL1

#else
    ...Add here for new platform.
#endif


/*------------------------------------------------------;
;               [ P O R T   B ]                         ;
;------------------------------------------------------*/
#if (PCB == 690)
    #define opMIO3          PB.DATA.BIT.B7  // MIO3 pin
    #define opMIO2          PB.DATA.BIT.B6  // MIO2 pin
    #define opMIO1          PB.DATA.BIT.B5  // MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0 pin
    #define opLDEN          PB.DATA.BIT.B3  // GPIO
    #define opXRST          PB.DATA.BIT.B2  // OPU XRST
    #define ipHOME          PB.DATA.BIT.B1  // home switch
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 1600) /* Until BCM1600 PCBA available, we use SUN690 boards. */
    #define opMIO3          PB.DATA.BIT.B7  // MIO3 pin
    #define opMIO2          PB.DATA.BIT.B6  // MIO2 pin
    #define opMIO1          PB.DATA.BIT.B5  // MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0 pin
    #define opCD_RLD        PB.DATA.BIT.B3  // Enable CD_LD and CD_Monitor signal ground connection.
    #define opDVD_RLD       PB.DATA.BIT.B2  // Enable DVD_LD and DVD_monitor ground connection.
    #define opBD_RLD        PB.DATA.BIT.B1  // Enable BD_LD and BD_Monitor signals.
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED -- Unused at this time
#elif (PCB == 691)
    #define opMIO3          PB.DATA.BIT.B7  // MIO3 pin(opModeSW)
    #define opMIO2          PB.DATA.BIT.B6  // MIO2 pin(opRSTB)
    #define opMIO1          PB.DATA.BIT.B5  // MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0 pin
    #define opPB3           PB.DATA.BIT.B3  // test point
    #define opSEL1          PB.DATA.BIT.B2  // OPU SEL1
    #define ipHOME          PB.DATA.BIT.B1  // home switch
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 692)
#if EPO_SLOT_IN
    #define indet           PB.DATA.BIT.B7  // MIO3 pin
#else
    #define opMIO3          PB.DATA.BIT.B7  // MIO3 pin
#endif
    #define ipOpenSW        PB.DATA.BIT.B6  // Tray Open Switch  // opMIO2 - MIO2 pin
    #define ipCloseSW       PB.DATA.BIT.B5  // Tray Close Switch // opMIO1 - MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0 pin
    #define opPI_CONT       PB.DATA.BIT.B3  // OPU PI control
    #define opXRST          PB.DATA.BIT.B2  // OPU XRST
//    #define ipSA_INT        PB.DATA.BIT.B2  // OPU SA init point
    #define ipHOME          PB.DATA.BIT.B1  // home switch
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 812)
    #define opMIO3          PB.DATA.BIT.B7  // MIO3 pin
    #define ipOpenSW        PB.DATA.BIT.B6  // Tray Open Switch  // opMIO2 - MIO2 pin
    #define ipCloseSW       PB.DATA.BIT.B5  // Tray Close Switch // opMIO1 - MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0 pin
    #define opDVD_CD        PB.DATA.BIT.B3  // DVD CD LD select
    #define opPB2           PB.DATA.BIT.B2   //
    #define ipHOME          PB.DATA.BIT.B1  // home switch
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 710)
    #define opSW            PB.DATA.BIT.B7  // SW                //opMIO3 - MIO3 pin
    #define ipOpenSW        PB.DATA.BIT.B6  // Tray Open Switch  // opMIO2 - MIO2 pin
    #define ipCloseSW       PB.DATA.BIT.B5  // Tray Close Switch // opMIO1 - MIO1 pin
    #define opCD_EN         PB.DATA.BIT.B4  // CD LD select  //opMIO0 - MIO0 pin
    #define opDVD_EN        PB.DATA.BIT.B3  // DVD LD select
    #define ipHOLD          PB.DATA.BIT.B2  // PLDS download pin
    #define opDRIVE_ON      PB.DATA.BIT.B1  // opCOL_EN : colli driver enable
#if(SUN_FUNC_TEST == 0)
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#endif
#elif (PCB == 711)
    #define opSW            PB.DATA.BIT.B7  // SW                //opMIO3 - MIO3 pin
    #define ipOpenSW        PB.DATA.BIT.B6  // Tray Open Switch  //opMIO2 - MIO2 pin
    #define ipCloseSW       PB.DATA.BIT.B5  // Tray Close Switch //opMIO1 - MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // MIO0              //opMIO0 - MIO0 pin
    #define opDVD_EN        PB.DATA.BIT.B3  // DVD LD select
    #define ipAGING         PB.DATA.BIT.B2  // BTC Aging test pin
#if (SET_SLOT_IN == 1) //[Q36] for HW define
    #define ipSW2 		  PB.DATA.BIT.B1  // home switch //PhotoSensor
#else
    #define ipHOME          PB.DATA.BIT.B1  // OPU home switch
#endif	 
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 815)
    #define opMIO3          PB.DATA.BIT.B7  // opMIO3 - MIO3 pin
    #define ipOpenSW        PB.DATA.BIT.B6  // Tray Open Switch  // opMIO2 - MIO2 pin
    #define ipCloseSW       PB.DATA.BIT.B5  // Tray Close Switch // opMIO1 - MIO1 pin
    #define opMIO0          PB.DATA.BIT.B4  // opMIO0 - MIO0 pin
    #define opLDDENB        PB.DATA.BIT.B3  // LDDENB
    #define ipPB2           PB.DATA.BIT.B2  // PB2
    #define ipOEICSW2       PB.DATA.BIT.B1  // OEICSW2
    #define opReadyLED      PB.DATA.BIT.B0  // Ready LED
#elif (PCB == 817)
    #define opPWRSTS        PB.DATA.BIT.B7  // PWR_STS            // opMIO3 - MIO3 pin
    #define opNTRST         PB.DATA.BIT.B6  // JTAG NTRST         // opMIO2 - MIO2 pin
    #define opTDO           PB.DATA.BIT.B5  // JTAG TDO           // opMIO1 - MIO1 pin
    #define opTDI           PB.DATA.BIT.B4  // JTAG TDI           // opMIO0 - MIO0 pin
    #define ipOpenSW        PB.DATA.BIT.B3  // Tray Open Switch      FRONT_OPEN
    #define ipCloseSW       PB.DATA.BIT.B2  // Tray Close Switch     FRONT_CLOSE
    #define indet           PB.DATA.BIT.B1  // slotin IN detect
    #define opTRAYSTS       PB.DATA.BIT.B0  //                       TRAY_STS/JTAG TMS
#else
    ...Add here for new platform.
#endif

/*------------------------------------------------------;
;               [ P O R T  C ]                               ;
;------------------------------------------------------*/
#define SELCDLASER   1
#define SELDVDLASER  0
#if (PCB == 690)
    #define ipSA_INT        PC.DATA.BIT.B3  // OPU SA init point
    #define ipPC2           PC.DATA.BIT.B2  // test point
    #define opPI_CONT       PC.DATA.BIT.B1  // OPU PI control
    #define ipPC0           PC.DATA.BIT.B0  // test point
//Dummy for 690
    #define opSELCD         PC.DATA.BIT.B0  // TBD
#elif (PCB == 1600)	/* Until BCM1600 PCBA available, we use SUN690 boards. */
    #define ipSA_INT        PC.DATA.BIT.B3  // OPU SA init point
    #define ipCloseSW       PC.DATA.BIT.B2  // Tray Close Switch
    #define ipOpenSW        PC.DATA.BIT.B1  // Tray Open Switch
    #define opDRIVE_ON      PC.DATA.BIT.B0  // BA6846FV IN3
#elif (PCB == 691)
    #define opPDG2          PC.DATA.BIT.B3  // PDIC gain switch(PDG2)
    #define ipPC2           PC.DATA.BIT.B2  // test point
    #define opPDG1          PC.DATA.BIT.B1  // PDIC gain switch1(PDG1)
    #define ipPC0           PC.DATA.BIT.B0  // test point
#elif (PCB == 692)
// Port C is not available on the 216LQFP or 128 LQFP packages
//Dummy for 692
//    #define opXRST          PC.DATA.BIT.B1  //TBD:just add for compiler issue
    #define opDRIVE_ON      PC.DATA.BIT.B1  // BA5912BFP mute1/2
    #define opSELCD         PC.DATA.BIT.B0  //TBD:just add for compiler issue
#elif (PCB == 812)
// Port C is not available on the 128 LQFP packages
//Dummy for 812
    #define opXRST          PC.DATA.BIT.B1  //TBD:just add for compiler issue
    #define opSELCD         PC.DATA.BIT.B0  //TBD:just add for compiler issue
#elif (PCB == 710)||(PCB == 711)
// Port C is not available on the 128 LQFP packages
//Dummy for 710
    #define opXRST          PC.DATA.BIT.B1  //TBD:just add for compiler issue
    #define opSELCD         PC.DATA.BIT.B0  //TBD:just add for compiler issue
#elif (PCB == 815)
// Port C is not available on the 128 LQFP packages
//Dummy for 815
    #define opXRST          PC.DATA.BIT.B1  //TBD:just add for compiler issue
    #define opSELCD         PC.DATA.BIT.B0  //TBD:just add for compiler issue
#elif (PCB == 817)
// Port C is available on the 128 LQFP after C package
// for 817
    #if(SUN_FUNC_TEST == 0)
    #define opReadyLED      PC.DATA.BIT.B2  // Ready LED
    #endif
    #define opDVD_RLD       PC.DATA.BIT.B1  //DVD_RLD
    #define opSEN           PC.DATA.BIT.B0  //SEN
#else
    ...Add here for new platform.
#endif

/*------------------------------------------------------;
;               [ GPIO MUX ]                            ;
;------------------------------------------------------*/

#if (PCB == 711) || (PCB == 710) || (PCB == 817)
    #if (CHIP_REV >= 0xC0)
#define GPIOMUX_VALUE 0x02
    #else
#define GPIOMUX_VALUE 0x00
    #endif
 #else
#define GPIOMUX_VALUE 0x00
#endif

/*------------------------------------------------------;
; Power Driver ON/OFF Control                           ;
;------------------------------------------------------*/
#if ((MOTOR_DRIVER == BD7907) || (MOTOR_DRIVER == BD7902) ||(MOTOR_DRIVER == BD7905) || (MOTOR_DRIVER == BD7954) || (MOTOR_DRIVER == BD7956) || (MOTOR_DRIVER == BD7959))
#define DRIVER_ALL_MUTE         opSMUTE = 0;\
                                opMMUTE = 0

#define DRIVER_LOADER_ON        opSMUTE = 1;\
                                opMMUTE = 0

#define DRIVER_SHORT_BRAKE      opSMUTE = 1;\
                                opMMUTE = 1;

#define DRIVER_REVERSE_BRAKE    opSMUTE = 0;\
                                opMMUTE = 1;
#elif (MOTOR_DRIVER == R2S30204)||(MOTOR_DRIVER == R2A30232)||(MOTOR_DRIVER == R2S30208)||(MOTOR_DRIVER == R2A30222)||(MOTOR_DRIVER == AM9268)||(MOTOR_DRIVER == TPIC1407)
#define DRIVER_ALL_MUTE         opMDEN2 = 0;\
                                opMDEN1 = 0

#define DRIVER_LOADER_ON        WRITE_FIELD(HAL_SRV_SL1PWMSEL, 3);\
                                opMDEN2 = 1;\
                                opMDEN1 = 0

#define DRIVER_SHORT_BRAKE      WRITE_FIELD(HAL_SRV_SL1PWMSEL, 0);\
                                opMDEN2 = 0;\
                                opMDEN1 = 1

#define DRIVER_REVERSE_BRAKE    WRITE_FIELD(HAL_SRV_SL1PWMSEL, 0);\
                                opMDEN2 = 1;\
                                opMDEN1 = 1
#endif

/*------------------------------------------------------;
; Collimator Shifter Power Driver ON/OFF Control        ;
;------------------------------------------------------*/
#if (COLLI_SWITCH == 1)
    #if (PCB == 710)   //Initial Excitation and DRIVER_ON/OFF
#define COLLI_DRIVER_OFF    (opDRIVE_ON = 0)
#define COLLI_DRIVER_ON     (opDRIVE_ON = 1);                                                                       \
                            WRITE_REG(HAL_SRVREG_MAP->PDMALPHA,(READ_REG(HAL_SRVREG_MAP->PDMALPHA) ^ 0x03FF03FF));  \
                            Delay100uS(1);                                                                          \
                            WRITE_REG(HAL_SRVREG_MAP->PDMALPHA,(READ_REG(HAL_SRVREG_MAP->PDMALPHA) ^ 0x03FF03FF))
    #elif (PCB == 817)  //Initial Excitation
#define COLLI_DRIVER_OFF
#define COLLI_DRIVER_ON     WRITE_REG(HAL_SRVREG_MAP->PDMALPHA,(READ_REG(HAL_SRVREG_MAP->PDMALPHA) ^ 0x03FF03FF));  \
                            Delay100uS(1);                                                                          \
                            WRITE_REG(HAL_SRVREG_MAP->PDMALPHA,(READ_REG(HAL_SRVREG_MAP->PDMALPHA) ^ 0x03FF03FF))
    #elif (PCB == 711)  //No DRIVER_ON/OFF, with LPF
#define COLLI_DRIVER_OFF    Colli_OutputDac(0x00, 0x00)
#define COLLI_DRIVER_ON     DRIVER_SHORT_BRAKE; 
    #elif (PCB == 815)  //No DRIVER_ON/OFF
#define COLLI_DRIVER_OFF
#define COLLI_DRIVER_ON
    #else
#define COLLI_DRIVER_OFF    (opDRIVE_ON = 0)
#define COLLI_DRIVER_ON     (opDRIVE_ON = 1)
    #endif
#endif

#if (SEEPROM == 1)
/*------------------------------------------------------;
;              Serial EEPROM Access                     ;
;------------------------------------------------------*/

typedef union
{
    USHORT  RIDReadOutW[3];
    BYTE    RIDReadOutB[6];
}RidData;
#endif

/*---------------------------------------------------------;
;                        LVSD Pads                         ;
; The following defines are to control the LVDS pads for   ;
; EFM1/2/3 and OSCEN                                       ;
;----------------------------------------------------------*/

/*** LVDS BIOS Generator Enable/Disable ***/
#define LVDS_BIAS_GEN (HAL_GLOBAL_SLPLVDSBG_MASK) /* No LVDS pads enabled, disable the Bias Generator */

/*** OSCEN IO Selection: LVDS or CMOS ***/
#define OSCEN         (HAL_GLOBAL_SLPOSCENL_MASK) /* Turn off LVDS driver & Turn on CMOS driver */

/*** EFM 1/2/3 LVDS Enable/Disable ***/
#define EFM1IO        (HAL_GLOBAL_SLPEFM1_MASK)   /* Disable EFM1 LVDS pads */
#define EFM2IO        (HAL_GLOBAL_SLPEFM2_MASK)   /* Disable EFM2 LVDS pads */
#define EFM3IO        (HAL_GLOBAL_SLPEFM3_MASK)   /* Disable EFM3 LVDS pads */

#define EFMOSCEN      (LVDS_BIAS_GEN | EFM3IO | EFM2IO | EFM1IO | OSCEN)  /* Initialization value for rwPWRDWN1 */

/* Disable all pins */
#define EFMOSCDIS   (HAL_GLOBAL_SLPOSCENC | HAL_GLOBAL_SLPLVDSBG | HAL_GLOBAL_SLPEFM3 | HAL_GLOBAL_SLPEFM2 | HAL_GLOBAL_SLPEFM1 | HAL_GLOBAL_SLPOSCENL)  /* Initialization value for rwPWRDWN1 */

#endif /* __HWDEF_H__ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
