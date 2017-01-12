/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:     INTCTL.H
*
* DESCRIPTION:  Defines for interrupt controller macros
*
*
* $Revision: 25 $
* $Date: 08/09/17 11:46p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _intCTL_H_
#define _intCTL_H_


#include ".\al\reg_hal.h"

//-----------------------------------------------------------------------
// Enable/Disable Interrupt Macros
//-----------------------------------------------------------------------
#define DISABLE_INTERRUPT()     (WRITE_FIELD(HAL_AHB_INTCTRL_ITEN, 1))
#define ENABLE_INTERRUPT()      (WRITE_FIELD(HAL_AHB_INTCTRL_ITEN, 0))


#define GET_INT_ENABLE_MASK()       (READ_REG(HAL_AHB_INTCTRLREG_MAP->ICITCR))
#define SET_INT_ENABLE_MASK(mask)   (WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICITCR,mask))

#define ENABLE_EXT_INT()    (WRITE_FIELD(HAL_AHB_INTCTRL_EXTINTE ,1))   /* EXTINT, Eject SW  */
#define DISABLE_EXT_INT()   (WRITE_FIELD(HAL_AHB_INTCTRL_EXTINTC ,1))

#define ENABLE_SS_INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_SSIINTE ,1)) /* Serial Interface Int  */
#define DISABLE_SS_INT()    (WRITE_FIELD(HAL_AHB_INTCTRL_SSIINTC, 1))

#define ENABLE_DSPINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_SVDINTE, 1)) /*  CD-DSP Int */
#define DISABLE_DSPINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_SVDINTC, 1))

#define ENABLE_AUDINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_AUDINTE, 1))  /* Audio Int */
#define DISABLE_AUDINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_AUDINTC, 1))

#if (EN_LS == 1)
#define ENABLE_LSINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_LSINTE, 1))    /* LS Int */
#define DISABLE_LSINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_LSINTC, 1))
#endif // End of (EN_LS == 1)

#define ENABLE_OPCINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_OPCINTE, 1))  /* OPC Int */
#define DISABLE_OPCINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_OPCINTC, 1))

#define ENABLE_WSINT()      (WRITE_FIELD(HAL_AHB_INTCTRL_WSINTE, 1))   /* WS Int */
#define DISABLE_WSINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_WSINTC, 1))

#define ENABLE_WBLINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_WBLINTE, 1))  /* WBL Int */
#define DISABLE_WBLINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_WBLINTC, 1))

#define ENABLE_DFEINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_DFEINTE, 1))  /* DFE Int */
#define DISABLE_DFEINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_DFEINTC, 1))

#define ENABLE_DMDINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_DMDINTE, 1))  /* DMD Int */
#define DISABLE_DMDINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_DMDINTC, 1))

#define ENABLE_CPYINT()     (WRITE_FIELD(HAL_AHB_INTCTRL_CPYINTE, 1))  /* Copy Int */
#define DISABLE_CPYINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_CPYINTC, 1))

#define ENABLE_SINT()       (WRITE_FIELD(HAL_AHB_INTCTRL_SVSINTE, 1))  /* Servo Int */
#define DISABLE_SINT()      (WRITE_FIELD(HAL_AHB_INTCTRL_SVSINTC, 1))

#define ENABLE_DINT()       (WRITE_FIELD(HAL_AHB_INTCTRL_DECINTE, 1))   // Enable CD Decoder Interrupt.

                                                                        // Disable CD Decoder Interrupt. However, do not disable Decoder Interrupt if any of the
                                                                        // low level DVD Decoder Interrupts are enabled.
#define DISABLE_DINT()      if   ((READ_REG(HAL_DVDDREG_MAP->DVDIRQC1) == 0)\
                               && (READ_REG(HAL_DVDDREG_MAP->DVDIRQC2) == 0)\
                               && (READ_REG(HAL_DVDDREG_MAP->DVDIRQC3) == 0))\
                                  (WRITE_FIELD(HAL_AHB_INTCTRL_DECINTC, 1))



#define ENABLE_EINT()       (WRITE_FIELD(HAL_AHB_INTCTRL_ENCINTE, 1))

                                                                        // Disable CD Encoder Interuppt. However, do not disable Decoder Interrupt if any of the
                                                                        // low level DVD Encoder Interrupts are enabled.
#define DISABLE_EINT()      if   ((READ_REG(HAL_DVDEREG_MAP->DVDEIRQE0) == 0)\
                               && (READ_REG(HAL_DVDEREG_MAP->DVDEIRQE1) == 0)\
                               && (READ_REG(HAL_DVDEREG_MAP->DVDEIRQE2) == 0))\
                                    (WRITE_FIELD(HAL_AHB_INTCTRL_ENCINTC, 1))



#define ENABLE_HINT()       (WRITE_FIELD(HAL_AHB_INTCTRL_HSTINTE, 1))  /* Host Int */
#define DISABLE_HINT()      (WRITE_FIELD(HAL_AHB_INTCTRL_HSTINTC, 1))

#define ENABLE_T7INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T7INTE, 1))   /* Timer7 Int */
#define DISABLE_T7INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T7INTC, 1))

#define ENABLE_T6INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T6INTE, 1))   /* Timer6 Int */
#define DISABLE_T6INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T6INTC, 1))

#define ENABLE_T5INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T5INTE, 1))   /* Timer5 Int */
#define DISABLE_T5INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T5INTC, 1))

#define ENABLE_T4INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T4INTE, 1))   /* Timer4 Int */
#define DISABLE_T4INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T4INTC, 1))

#define ENABLE_T3INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T3INTE, 1))   /* Timer3 Int */
#define DISABLE_T3INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T3INTC, 1))

#define ENABLE_T2INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T2INTE, 1))   /* Timer2 Int */
#define DISABLE_T2INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T2INTC, 1))

#define ENABLE_T1INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T1INTE, 1))   /* Timer1 Int */
#define DISABLE_T1INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T1INTC, 1))

#define ENABLE_T0INT()      (WRITE_FIELD(HAL_AHB_INTCTRL_T0INTE, 1))   /* Timer0 Int */
#define DISABLE_T0INT()     (WRITE_FIELD(HAL_AHB_INTCTRL_T0INTC, 1))

#define ENABLE_UART_INT()   (WRITE_FIELD(HAL_AHB_INTCTRL_UARTINTE, 1))  /* UART Int */
#define DISABLE_UART_INT()  (WRITE_FIELD(HAL_AHB_INTCTRL_UARTINTC, 1))

#define ENABLE_DVD_DINT()    (WRITE_FIELD(HAL_AHB_INTCTRL_DECINTE, 1)) // Enable  DVD Decoder Interrupts.

                                                                       // Disable DVD Decoder Interrupts. However, do not disable Decoder Interrupt if any of the
                                                                       // low level CD Decoder Interrupts are enabled.
#define DISABLE_DVD_DINT()   if   ((READ_REG(HAL_CDDREG_MAP->INT1EN) == 0))\
                                     (WRITE_FIELD(HAL_AHB_INTCTRL_DECINTC, 1))

#define ENABLE_CDINT()       ENABLE_DINT();ENABLE_EINT()                        /* CD Decoder and Encoder Int */
#define DISABLE_CDINT()      DISABLE_DINT();DISABLE_EINT()

#endif /* _intCTL_H_ */
