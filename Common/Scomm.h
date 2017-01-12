/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* $Revision: 28 $
* $Date: 10/02/21 11:23a $
*
* DESCRIPTION:
*   Serial communications with peripherals
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef _SCOMM_H_
#define _SCOMM_H_

#include ".\hif\flash.h"

#if (EN_POWER_CONTROL == 1)
#include ".\hif\powerctl.h"
#endif

#if (OPU == KEM350AAA)
#include ".\OEM\OPU_KEM350AAA.h"
#endif

#if (SEEPROM == 1)
/************************ Serial EEPROM ***************************/
extern USHORT SerialReadWord(USHORT Addr);
extern void   SerialWriteWord(USHORT Addr, USHORT Data);
extern BYTE   SerialReadByte(USHORT addr);
extern void   SerialWriteByte(USHORT addr, BYTE data);
extern float  SerialRead4F(USHORT addr);
extern void   SerialWrite4F(USHORT addr, USHORT *data);
#endif

#define SetEEDATAAsOutput			READ_MOD_WRITE(HAL_PERIPHREG_MAP->PACTRL,0x20,0)		/* Set EE_DAT as output */
#define SetEEDATAAsInput			READ_MOD_WRITE(HAL_PERIPHREG_MAP->PACTRL,0x20,0x20)		/* Set EE_DAT as input */


/**************************** UART ********************************/
#define HW_UART_FIFO_SISE  (16)
#define RX_BUF_SIZE   (2052)
#define TX_BUF_SIZE   (1024)


/**** Externs ****/
#if (ENABLE_UART_RXD == 1)
extern BYTE rx_ring_buff[RX_BUF_SIZE];
extern USHORT  s2b_byte_received;
    #if (MSG_OUT_MODE == MSG_USBPORT)
extern BOOL    s2b_atapi_cmd_received;
extern BOOL    s2b_atapi_para_received;
extern BOOL    s2b_atapi_para_err;
    #endif
#endif

/*********************** Serial Interface *************************/
#if (PCB == 691)
#define XWS	                1        /* External Write Strategy enable */
#define XSerialReadEnable	1        /* External Write Strategy Read enable */
#else
#define XWS	                0        /* External Write Strategy enable */
#define XSerialReadEnable	0        /* External Write Strategy Read enable */
#endif

#if (PCB == 691)
#define LCD                 0        /* External LCD control enable */
#define LCDSerialReadEnable 0        /* External LCD control Read enable */
#else
    #if (OPU == KEM350AAA)
//#define LCD                 0        /* External LCD control enable */
#define LCDSerialReadEnable	1        /* External LCD control Read enable */
    #else
#define LCD                 0        /* External LCD control enable */
#define LCDSerialReadEnable	0        /* External LCD control Read enable */
    #endif
#endif


/**** Prototypes ****/
#define SSI_init     0
#define XWSCh	     1
#define SSICh	     2
#define LCDCh	     3
#define DSPCh	     4

void init_serial(void);
#if (OPU == KEM350AAA)
void SerialChannelConfig(BYTE Chnl);
extern BYTE ReadOPU(BYTE address);
extern void WriteOPU(BYTE address, BYTE data);
#endif

#if (XWS == 1)
    #if (OPU == KEM350AAA)
//void SerialChannelConfig(BYTE Chnl);
    #else
void SerialChannelConfig(BYTE Chnl);
    #endif
void WriteXWSReg_1B(BYTE addr, BYTE data);
void WriteXWSReg_1BWait(BYTE addr, BYTE data);
void WaitXWSFIFOEmpty(void);
#endif

#if (LCD == 1)
void WriteLCDReg_1B(BYTE addr, BYTE data);
void WriteLCDReg_1BWait(BYTE addr, BYTE data);
void WaitLCDFIFOEmpty(void);
#endif

/**** Defines ****/
#if (XWS == 1)
    #if (XSerialReadEnable == 1)
BYTE ReadXWSReg_1B(BYTE addr);
    #endif

    #if (PCB == 691)
/*** 12 MB/S Transfer rate ***/
/* SCLK = (SYSCLK/2) / ((XWSCLKSCL * (1+XWSCLKDIV)) */
#define	XWSCFG        (HAL_APB_SSI_WSCLKINV)
//#define	XWSCFG        0
                                             /* External WS Communications Format
												bit 6 = 0: Send data immediatly
												bit 5 = 0: Send Address LSB first
												bit 4 = 0: Send Data LSB first
												bit 3 = 0: No SCLK polarity swap (data valid on rising edge)
												bit 2 = 0: No XLAT polarity swap (assert low then high)
												bit 1 = 0: Use hardware generate XLAT
												bit 0 = 0: Don't care */
#define XWS_OFFSET_ADDR       0x0000
// LCD_IF_START(0x1000h) + ADDR<<2
// ex:0x71 -> 0x1C4 -> 0x11C4
#define LCD_OL1_ADDR          0x71      //OL1 phase register
#define LCD_OL2_ADDR          0x72      //OL2 phase register
#define LCD_OL3_ADDR          0x73      //OL3 phase register
#define LCD_OL4_ADDR          0x74      //OL4 phase register
#define LCD_OL5_ADDR          0x75      //OL5 phase register
#define LCD_OL6_ADDR          0x76      //OL6 phase register
#define LCD_OL7_ADDR          0x77      //OL7 phase register
#define LCD_OL8_ADDR          0x78      //OL8 phase register
#define LCD_OL9_ADDR          0x79      //OL9 phase register
#define LCD_OL10_ADDR         0x7A      //OL10 phase register
#define LCD_OL11_ADDR         0x7B      //OL11 phase register
#define LCD_OL12_ADDR         0x7C      //OL12 phase register
    #endif // #if (PCB == 691)
#endif // #if (XWS == 1)

/*** 12 MB/S Transfer rate ***/
/* SCLK = (SYSCLK/2) / ((SSICLKSCL * (1+SSICLKDIV)) */
#define	SSICFG					0			/* SSI Communications Format
												bit 6 = 0: Send data immediatly
												bit 5 = 0: Send Address LSB first
												bit 4 = 0: Send Data LSB first
												bit 3 = 0: No SCLK polarity swap (data valid on rising edge)
												bit 2 = 0: No XLAT polarity swap (assert low then high)
												bit 1 = 0: Use hardware generate XLAT
												bit 0 = 0: Don't care */

#define ADAPTER_ADDR_1          0x9F0      // This is a work around to assure
#define ADAPTER_ADDR_2          0x9F4      // that SSI port sends 7C / 7D to Debug Adapter Board.

#if (LCD == 1)
    #if (LCDSerialReadEnable == 1)
BYTE ReadLCDReg_1B(BYTE addr);
    #endif

    #if (PCB == 691)
/* SCLK = (SYSCLK/2) / ((XWSCLKSCL * (1+XWSCLKDIV)) */
#define	LCDCFG					0			/* LCD control Communications Format
												bit 6 = 0: Send data immediatly
												bit 5 = 0: Send Address LSB first
												bit 4 = 0: Send Data LSB first
												bit 3 = 0: No SCLK polarity swap (data valid on rising edge)
												bit 2 = 0: No XLAT polarity swap (assert low then high)
												bit 1 = 0: Use hardware generate XLAT
												bit 0 = 0: Don't care */

#define LCD_OFFSET_ADDR       0x1000

// LCD_IF_START(0x1000h) + ADDR<<2
// ex:0x71 -> 0x1C4 -> 0x11C4
#define LCD_OL1_ADDR          0x71      //OL1 phase register
#define LCD_OL2_ADDR          0x72      //OL2 phase register
#define LCD_OL3_ADDR          0x73      //OL3 phase register
#define LCD_OL4_ADDR          0x74      //OL4 phase register
#define LCD_OL5_ADDR          0x75      //OL5 phase register
#define LCD_OL6_ADDR          0x76      //OL6 phase register
#define LCD_OL7_ADDR          0x77      //OL7 phase register
#define LCD_OL8_ADDR          0x78      //OL8 phase register
#define LCD_OL9_ADDR          0x79      //OL9 phase register
#define LCD_OL10_ADDR         0x7A      //OL10 phase register
#define LCD_OL11_ADDR         0x7B      //OL11 phase register
#define LCD_OL12_ADDR         0x7C      //OL12 phase register
    #else // #if (PCB == 691)
/*** 12 MB/S Transfer rate ***/
/* SCLK = (SYSCLK/2) / ((XWSCLKSCL * (1+XWSCLKDIV)) */
#define	LCDCFG					0			/* LCD control Communications Format
												bit 6 = 0: Send data immediatly
												bit 5 = 0: Send Address LSB first
												bit 4 = 0: Send Data LSB first
												bit 3 = 0: No SCLK polarity swap (data valid on rising edge)
												bit 2 = 0: No XLAT polarity swap (assert low then high)
												bit 1 = 0: Use hardware generate XLAT
												bit 0 = 0: Don't care */

#define LCD_OFFSET_ADDR       0x1000
    #endif // #if (PCB == 691)
#endif // #if (LCD == 1)


#endif /* SCOMM_H_ define */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
