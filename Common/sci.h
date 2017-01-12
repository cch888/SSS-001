/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
* FILENAME:   SCI.H

* NOTES:
*
* DESCRIPTION:
*   This file contains serial port transferring control register and bit definitions for:
*
* $Revision: 6 $
* $Date: 08/06/07 9:42a $
*
*
*************************** SUNEXT CONFIDENTIAL ********************************/

#ifndef __sci_h__
#define __sci_h__

#include ".\AL\REG_HAL.h"
#include ".\AL\REG_uart.h"

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    B0       :1;
        BYTE    B1       :1;
        BYTE    B2       :1;
        BYTE    B3       :1;
        BYTE    B4       :1;
        BYTE    B5       :1;
        BYTE    B6       :1;
        BYTE    B7       :1;
    }   bit;

} unBYTE;

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    ERBFI   :1;
        BYTE    ETBEI   :1;
        BYTE    ELSI    :1;
        BYTE            :5;
    }   bit;
} uIER;

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    FIFO    :2;
        BYTE            :2;
        BYTE    IRQID   :4;
    }   bit;
} uIIR;

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    RCVRTrg     :2;
        BYTE                :3;
        BYTE    TxFIFORst   :1;
        BYTE    RxFIFORst   :1;
        BYTE    FIFOEn      :1;
    }   bit;
} uFCR;

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    DLAB        :1;
        BYTE    BREAK       :1;
        BYTE                :1;
        BYTE    EPS         :1;
        BYTE    PEN         :1;
        BYTE    STOP        :1;
        BYTE    CLS         :2;
    }   bit;
} uLCR;

typedef union
{
    BYTE byte;
    struct
    {
        BYTE    FERR        :1;
        BYTE    TEMT        :1;
        BYTE    THRE        :1;
        BYTE    BI          :1;
        BYTE    FE          :1;
        BYTE    PE          :1;
        BYTE    OE          :1;
        BYTE    DR          :1;
    }   bit;
} uLSR;


#define RrRBR  (*(volatile unBYTE*)(HAL_APB_UART_UARTRBR_ADR))  /* Receive Buffer Register */
#define RwTHR  (*(volatile unBYTE*)(HAL_APB_UART_UARTTHR_ADR))  /* Transmit Holding Register */

#define RrwIER (*(volatile uIER*)(HAL_APB_UART_UARTDH_ADR))     /* Interrupt Enable Register */

#define RrwDLL (*(volatile unBYTE*)(HAL_APB_UART_UARTDL_ADR))   /* Divisor Latch (Low) */
#define RrwDLH (*(volatile unBYTE*)(HAL_APB_UART_UARTDH_ADR))   /* Divisor Latch (High) */

#define RrIIR  (*(volatile uIIR*)(&(HAL_APB_UARTREG_MAP->UARTIIR))) /* Interrupt Identity Register */
#define INT_TXEMPTY         0x02
#define INT_DATAAVAIABLE    0x04
#define INT_LINESTATUS      0x06
#define INT_CHARTIMEOUT     0x0C

#define RwFCR  (*(volatile uFCR*)(&(HAL_APB_UARTREG_MAP->UARTIIR))) /* FIFO Control Register *///use UARTIIR to replace UARTFCR

#define RrwLCR (*(volatile uLCR*)(&(HAL_APB_UARTREG_MAP->UARTLCR))) /* Line Control Register */
#define RrwLSR (*(volatile uLSR*)(&(HAL_APB_UARTREG_MAP->UARTLSR))) /* Line Status Register */

//#define RrwSCR (*(volatile unBYTE *)(UART_PAGE+rwSCR)) /* Scratch Register: SPARE */

#endif /* __sci_h__ */
