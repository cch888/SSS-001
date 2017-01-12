/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:     intctl.c
*
* DESCRIPTION   This file contains the interrupt control
*
*
*   $Revision: 31 $
*   $Date: 09/11/30 5:45p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"

#if (EN_LS == 1)
#include ".\oem\oem_info.h"
#include ".\LS\LS_isr.h"
#endif // End of (EN_LS == 1)

#include ".\AL\reg_hal.h"
#include ".\AL\reg_intctrl.h"

extern void DecoderISR();
extern void EncoderISR(void);
extern void DigitalFrontEndISR(void);
extern void HostIRQService(void);
extern void svoSInt(void);
extern void Timer0Isr(void);
extern void Timer1Isr(void);
extern void Timer2Isr(void);
extern void Timer3Isr(void);
extern void EjectSwitchISR(void);
extern void Timer5Isr(void);
extern void Timer7Isr(void);
extern void UART_isr(void);
extern void JitterISR(void);
#if (ENABLE_FIQ == 1)
extern void svoSInt_FIQ(void);
#endif

#if(ENABLE_HDDVD && ENABLE_AACS)
extern void DemodISR();
#endif

/****************************************************************************
* Name:     DummyISR0()
*
* Description:  This is dummy ISR.
*
*****************************************************************************/
void DummyISR0(void)
{
}
/****************************************************************************
* Name:     DummyISR1()
*
* Description:  This is dummy ISR.
*               FIQ interrupt shows a value of 000100 at ICPSTAT.
*****************************************************************************/
void DummyISR1(void)
{
}

#if (STREAM_STM == 1)
/*-------------------------------------------------------------------
// OPCIsr() - Generic OPC Interrupt Service Routine
// Only for Stream STM use
---------------------------------------------------------------------*/
void OPCIsr(void)
{
    BOOL StopIRQHappened;
    StopIRQHappened = HAL_OPC_Isr(); //temp --> under construction --> need to be merged with OPCIsr's
}
#endif

//-----------------------------------------------------------------------------;
// This is an indirect call table. input is the "priority" of the current      ;
// interrupt. Output is to call the current interrupt.                         ;
//                                                                             ;
// This table has to agree with the setting inside "InitializeInterrupts", which is  ;
// inside file named "InitARM.c"                                               ;
//-----------------------------------------------------------------------------;
void (* const func_isr_tbl[16])(void) = {
    DummyISR0,          // priority 0
    DummyISR1,          // priority 1
    EncoderISR,         // priority 2
    DecoderISR,         // priority 3
    HostIRQService,     // priority 4
    EjectSwitchISR,     // priority 5
    svoSInt,            // priority 6
    Timer2Isr,          // priority 7
    Timer0Isr,          // priority 8
    Timer1Isr,          // priority 9
    Timer3Isr,          // priority 10
    DigitalFrontEndISR, // priority 11

#if(ENABLE_HDDVD && ENABLE_AACS)
    DemodISR,           // Priority 12
#else
    #if (EN_LS == 1)
    LSIRQHandler,       // Priority 12
    #else
    DummyISR0,          // priority 12
    #endif // End of (EN_LS == 1)
#endif
#if (STREAM_STM == 1)
    OPCIsr,             // priority 13
#else
    DummyISR0,          // priority 13
#endif
    Timer5Isr,          // priority 14
    UART_isr            // priority 15
};


/****************************************************************************
* Name:     C_IRQ_Handler()
*
* Description:  This function process the interrupts execution
*
*****************************************************************************/
void C_IRQ_Handler(void)
{

    BYTE    temp;

    temp = (READ_REG(HAL_AHB_INTCTRLREG_MAP->ICPSTAT) >> 2 ) & 0x0f;
    (*(func_isr_tbl[temp]))();
}

/****************************************************************************
* Name:     C_FIQ_Handler()
*
* Description:  This function process the interrupts execution
*
*****************************************************************************/
void C_FIQ_Handler(void)
{
#if (ENABLE_FIQ == 1)
    svoSInt_FIQ();
#endif
}

