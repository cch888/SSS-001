/****************************************************************************
*                (c) Copyright 2002 - 2006 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
* FILENAME:   timer.h

* NOTES:
*
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
* 0.5       Ap-26-02    XinL    Creation, draft
*
*   $Revision: 25 $
*   $Date: 10/11/24 5:20p $
*
* DESCRIPTION:
*
*************************** SUNEXT CONFIDENTIAL ********************************/

#ifndef __timer_h__
#define __timer_h__

#include ".\al\REG_HAL.h"
#include ".\al\REG_periph.h"

/****************************************************************************/


typedef union _union_ushort_ubytes
{
    USHORT  USHOT;              /* Timer Compare Register --16bit Offset--0x04/05 */
    struct
    {
        BYTE UBYTE_L;      // lower  8 bit of compare register
        BYTE UBYTE_H;      // higher 8 bit of compare register
    } UBYTE;

} union_ushort_ubytes_t;

typedef volatile struct st_timer                     /* struct TIMER  */
{
    //---------------------------------------------------------------------;
    // for TCNTRL                                                          ;
    //---------------------------------------------------------------------;
    union
    {                          /* Control Register-offset 0x00 */
        BYTE BYTE;             /* Byte Access */
        struct
        {                      /* Bit access */
            BYTE MODE  :2;     /* Timer mode  */
                                        /* 00-- Off */
                                        /* 01-- Measure mode- Count the # of clock cycle while input
                                                signal is high(or low) or for one cycle of input signal*/
                                        /* 10-- Counter mode -Count the # of rising(or falling) edges
                                                of the signal input */
                                        /* 11-- Timer mode - Count the number of clock cycle up to a
                                                programmed threshold */
            BYTE INTEN :2;     /* Timer Interrupt Enable  */
                                        /* 00-- Interrupt Off */
                                        /* 01-- Interrupt is generated when timer is done */
                                        /* 10-- Reset is generated when timer is done */
                                        /* 11-- Reserved */
            BYTE AUTOSTOP:1;   /* Timer Auto Stop */
                                        /* 0-- Timer will auto reset and restart counting again*/
                                        /* 1-- Enable Auto Stop */
            BYTE BRKEN:1;      /* Timer Halt at ICE break */
                                        /* 0-- Timer doesn't halt during an ARM ICE breakpoint */
                                        /* 1-- Timer halts during an ARM ICE breakpoint */
            BYTE RESERVED:1;   /* Reserved bits (1 bits) */
            BYTE TSTRT :1;     /* Timer Start bit */
                                        /* 0-- Stop Timer  */
                                        /* 1-- Start Timer */
        } BIT;
    } TCNTRL;


    //---------------------------------------------------------------------;
    // for TCLK                                                            ;
    //---------------------------------------------------------------------;
    union
    {                                   /* Timer Clock Register - Offset 0x01 */
        BYTE BYTE;              /* Byte Access */
        struct
        {                               /* Bit access */
            BYTE CLKDIV:3;      /* Timer Clock Divier */
                                        /* 000-- XIN (no divider) */
                                        /* 001-- XIN/4 */
                                        /* 010-- XIN/16 */
                                        /* 011-- XIN/64 */
                                        /* 100-- XIN/256 */
                                        /* 101-- XIN/1024 */
                                        /* 110-- Reserved */
                                        /* 111-- Reserved */
            BYTE RESERVED:5;    /* Reserved bits (5 bits) */
        } BIT;
    } TCLK;


    //---------------------------------------------------------------------;
    // for TCMCTRL                                                         ;
    //---------------------------------------------------------------------;
    union
    {                                   /* Timer Counter & Measure Mode Control Register- Offset 0x02 */
        BYTE BYTE;          /* Byte Access */
        struct
        {                               /* Bit access */
            BYTE TCMSEL:2;      /* Timer Counter and Measure Input Select */
                                        /* 00-- (IN0) */
                                        /* 01-- (IN1) */
                                        /* 10-- (IN2) */
                                        /* 11-- .... (no IN3) */
                                        /* Different Timer, the input selection is a little different*/
                                        /* See the table below for details */
                                        /*-----------Timer Inputs and Outputs *----------------*/
                                        /*          Timer0  Timer1  Timer2  Timer3  Timer4  Timer5
                                            IN0     PA0     PA1     PA2     PA3     PA4     PA5
                                            IN1     PA6     PA7     PB4     PB5     PB6     PB7
                                            IN2     ESFS    WBLIN   FG      PB0     PB1     OENC
                                            No IN3  ---     ---     ---     ---     ---     ---
                                            Output  ---     ---     ---     ---     ---     ---  */

            BYTE RESERVED2:1;   /* Reserved bit  */
            BYTE TCMPOL:1;      /* Timer Counter and Measure Polarity */
                                        /* 0-- Rising edges are counted (Counter mode) or
                                               rising edges begin the measure period(measure mode) */
                                        /* 1-- Falling edges are counted (Counter mode) or
                                               falling edges begin the measure period(measure mode) */
            BYTE TMSREDG:1;     /* Timer Measure Edge */
                                        /* 0-- Cycle (Rising edge to rising edge or
                                               falling edge to falling edge if TCMPOL is 1) */
                                        /* 1-- Edge to Edge(Rising edge to falling edge or
                                               Falling edge to rising edge if TCMPOL is 1) */
            BYTE RESERVED1:3;   /* Reserved bits (3 bits)  */
        } BIT;
    } TCMCTRL;


    //---------------------------------------------------------------------;
    // for TSTATUS                                                         ;
    //---------------------------------------------------------------------;
    union
    {                                   /* Timer Status Register- Offset 0x03 */
        BYTE BYTE;          /* Byte Access */
        struct
        {                              /* Bit access */
            BYTE TINTF:1;       /* Timer Interrupt Status  */
                                        /* 0-- No Interrupt condition */
                                        /* 1-- Timer interrupt condition */
            BYTE TBUSY:1;       /* Timer Busy*/
                                        /* 0-- Not busy */
                                        /* 1-- Busy */
            BYTE TOVFL:1;       /* Timer Overflow */
                                        /* 0-- Normal Operation */
                                        /* 1-- Overflow  */
            BYTE RESERVED:5;    /* Reserved bits (5 bits)  */
        } BIT;
    } TSTATUS;


    //---------------------------------------------------------------------;
    // for TCNTRL                                                          ;
    //---------------------------------------------------------------------;
    union_ushort_ubytes_t     TCMP;

    //---------------------------------------------------------------------;
    // for TVAL                                                            ;
    //---------------------------------------------------------------------;
    union_ushort_ubytes_t     TVAL;

} timer;

#define pTIMER0  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T0CNTRL)) /* Timer0 address --- Kernel Timer */
#define pTIMER1  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T1CNTRL)) /* Timer1 address --- Player Timer */
#define pTIMER2  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T2CNTRL)) /* Timer2 address --- FG Timer */
#define pTIMER3  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T3CNTRL)) /* Timer3 address --- ROPC Timer */
#define pTIMER4  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T4CNTRL)) /* Timer4 address --- Meas Timer */
#define pTIMER5  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T5CNTRL)) /* Timer5 address --- */

#define pTIMER6  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T6CNTRL)) /* Timer6 address --- */
#define pTIMER7  (volatile struct st_timer  *)(&(HAL_PERIPHREG_MAP->T7CNTRL)) /* Timer7 address --- */

#define TIMER0  (*pTIMER0)                                        /* Timer0 address --- Kernel Timer */
#define TIMER1  (*pTIMER1)                                        /* Timer1 address --- Player Timer */
#define TIMER2  (*pTIMER2)                                        /* Timer2 address --- FG Timer */
#define TIMER3  (*pTIMER3)                                        /* Timer3 address --- ROPC Timer */
#define TIMER4  (*pTIMER4)                                        /* Timer4 address --- Meas Timer */
#define TIMER5  (*pTIMER5)                                        /* Timer5 address --- */
#define TIMER6  (*pTIMER6)                                        /* Timer6 address --- */
#define TIMER7  (*pTIMER7)                                        /* Timer7 address --- */

/* Timer Control Defines */
#define START_KERNEL_TIMER  (TIMER0.TCNTRL.BIT.TSTRT = 1)   /* Macro to start Timers */
#define START_FG_TIMER      (TIMER2.TCNTRL.BIT.TSTRT = 1)
#define START_PLR_TIMER     (TIMER1.TCNTRL.BIT.TSTRT = 1)
#define START_TIMER2        (TIMER2.TCNTRL.BIT.TSTRT = 1)
#define START_ROPC_TIMER    (TIMER3.TCNTRL.BIT.TSTRT = 1)
#define START_SVO_TIMER     (TIMER4.TCNTRL.BIT.TSTRT = 1)
#define START_TIMER5        (TIMER5.TCNTRL.BIT.TSTRT = 1)
#define START_TIMER6        (TIMER6.TCNTRL.BIT.TSTRT = 1)
#define START_TIMER7        (TIMER7.TCNTRL.BIT.TSTRT = 1)

#define STOP_KERNEL_TIMER   (TIMER0.TCNTRL.BIT.TSTRT = 0)   /* Macro to stop Timers */
#define STOP_FG_TIMER       (TIMER2.TCNTRL.BIT.TSTRT = 0)
#define STOP_PLR_TIMER      (TIMER1.TCNTRL.BIT.TSTRT = 0)
#define STOP_TIMER2         (TIMER2.TCNTRL.BIT.TSTRT = 0)
#define STOP_ROPC_TIMER     (TIMER3.TCNTRL.BIT.TSTRT = 0)
#define STOP_SVO_TIMER      (TIMER4.TCNTRL.BIT.TSTRT = 0)
#define STOP_TIMER5         (TIMER5.TCNTRL.BIT.TSTRT = 0)
#define STOP_TIMER6         (TIMER6.TCNTRL.BIT.TSTRT = 0)
#define STOP_TIMER7         (TIMER7.TCNTRL.BIT.TSTRT = 0)

#define ENABLE_SERVO_ISR    (TIMER0.TCNTRL.BIT.INTEN = 1)   /* Macro to Enable Timers Interrupt */
#define ENABLE_TIMER1_INT   (TIMER1.TCNTRL.BIT.INTEN = 1)
#define ENABLE_TIMER2_INT   (TIMER2.TCNTRL.BIT.INTEN = 1)
#define ENABLE_ROPC_ISR     (TIMER3.TCNTRL.BIT.INTEN = 1)
#define ENABLE_MEAS_ISR     (TIMER4.TCNTRL.BIT.INTEN = 1)
#define ENABLE_TIMER5_INT   (TIMER5.TCNTRL.BIT.INTEN = 1)
#define ENABLE_TIMER6_INT   (TIMER6.TCNTRL.BIT.INTEN = 1)
#define ENABLE_TIMER7_INT   (TIMER7.TCNTRL.BIT.INTEN = 1)

#define DISBALE_TIMER0_INT  (TIMER0.TCNTRL.BIT.INTEN = 0)   /* Macro to Disable Timers Interrupt */
#define DISBALE_TIMER1_INT  (TIMER1.TCNTRL.BIT.INTEN = 0)
#define DISABLE_FGINT_INT   (TIMER2.TCNTRL.BIT.INTEN = 0)
#define DISABLE_ROPC_ISR    (TIMER3.TCNTRL.BIT.INTEN = 0)
#define DISABLE_MEAS_ISR    (TIMER4.TCNTRL.BIT.INTEN = 0)
#define DISBALE_TIMER5_INT  (TIMER5.TCNTRL.BIT.INTEN = 0)
#define DISBALE_TIMER6_INT  (TIMER6.TCNTRL.BIT.INTEN = 0)
#define DISBALE_TIMER7_INT  (TIMER7.TCNTRL.BIT.INTEN = 0)

#define CLEAR_FG_TIMER_COUNTER      (TIMER2.TCMPL = (USHORT)0x0000)
#define CLEAR_PLR_TIMER_COUNTER     (TIMER1.TCMPL = (USHORT)0x0000)
#define CLEAR_KERNEL_TIMER_COUNTER  (TIMER0.TCMPL = (USHORT)0x0000)
#define CLEAR_OPC_TIMER             (TIMER3.TCMPL = (USHORT)0x0000)
#define CLEAR_OPC_COUNTER_TIMER     (TIMER3.TCMPL = (USHORT)0x0000)
#define CLEAR_SVO_TIMER_COUNTER     (TIMER4.TCMPL = (USHORT)0x0000)
#define CLEAR_TIMER4_COUNTER        (TIMER4.TCMPL = (USHORT)0x0000)
#define CLEAR_TIMER5_COUNTER        (TIMER5.TCMPL = (USHORT)0x0000)
#define CLEAR_TIMER6_COUNTER        (TIMER6.TCMPL = (USHORT)0x0000)
#define CLEAR_TIMER7_COUNTER        (TIMER7.TCMPL = (USHORT)0x0000)

#define INIT_ROPC_TIMER             (TIMER3.TCMPL = (USHORT)0x0000)
#define LOAD_ROPC_TIMER             (TIMER3.TCMPL = (USHORT)250)
#define LOAD_OPC_TIMER              (TIMER3.TCMPL = (USHORT)87)

#define GET_FG_CAPTURE_COUNTER  (TIMER0.TVAL)
#define FG_TIMER_COUNTER        (TIMER0.TVAL)
#define PlayerTimerCounter      (TIMER1.TVAL)
#define TIMER2_COUNTER          (TIMER2.TVAL)
#define TIMER3_COUNTER          (TIMER3.TVAL)
#define TIMER4_COUNTER          (TIMER4.TVAL)
#define TIMER5_COUNTER          (TIMER5.TVAL)
#define TIMER6_COUNTER          (TIMER6.TVAL)
#define TIMER7_COUNTER          (TIMER7.TVAL)

/* Control bits/value for Timer Control Register TCNTRL */
#define bTSTRT      bit7
#define bTAUTOSTOP  bit4
#define RESETEN     0x08
#define INTENABLE   0x04
#define INTDISABLE  0x00
#define TIMERMODE   0x03
#define COUNTERMODE 0x02
#define MEASUREMODE 0x01

/* Control bits/values for Timer Clock Register TCLK */
#define NODIV       0x00
#define CLK4        0x01
#define CLK16       0x02
#define CLK64       0x03
#define CLK256      0x04
#define CLK1024     0x05

/* Control bits/values for Timer Counter/Measure Mode Register TCMCTRL */
#define bTMSREDG    bit4
#define bTCMPOL     bit3
#define INPUT_IN0   0x00
#define INPUT_IN1   0x01
#define INPUT_IN2   0x02

#define INPUT_FG    0x02

/* Control bits/values for Timer Status Register TSTATUS */
#define bTMSREDG    bit4
#define bTOVFL      bit2
#define bTBUSY      bit1
#define bTINTF      bit0

#if PERIPHERAL_CLK_25MHZ == 1
//----------------------------------------------------;
// assign values for time period                      ;
// Timers base clock is XIN = 25MHz                   ;
//----------------------------------------------------;

// for CLK16 (timer resolution = 640 ns)
#define CLK16_1000_US     ((USHORT) 1563)
#define CLK16_800_US      ((USHORT) 1250)
#define CLK16_500_US      ((USHORT) 781 )
#define CLK16_250_US      ((USHORT) 391 )
#define CLK16_200_US      ((USHORT) 313 )
#define CLK16_190_US      ((USHORT) 297 )
#define CLK16_150_US      ((USHORT) 234 )
#define CLK16_125_US      ((USHORT) 195 )
#define CLK16_120_US      ((USHORT) 188 )
#define CLK16_110_US      ((USHORT) 172 )
#define CLK16_100_US      ((USHORT) 156 )
#define CLK16_90_US       ((USHORT) 141 )
#define CLK16_86_US       ((USHORT) 134 )
#define CLK16_85_US       ((USHORT) 133 )
#define CLK16_80_US       ((USHORT) 125 )
#define CLK16_75_US       ((USHORT) 117 )
#define CLK16_74_US       ((USHORT) 116 )
#define CLK16_10_US       ((USHORT) 16 )
#define CLK16_1_US        ((USHORT) 1   )

// for CLK4 (timer resolution = 160 ns)
#define CLK4_1000_US      ((USHORT) 6250)
#define CLK4_800_US       ((USHORT) 5000)
#define CLK4_500_US       ((USHORT) 3125)
#define CLK4_250_US       ((USHORT) 1563)
#define CLK4_200_US       ((USHORT) 1250)
#define CLK4_190_US       ((USHORT) 1188)
#define CLK4_150_US       ((USHORT) 938 )
#define CLK4_125_US       ((USHORT) 781 )
#define CLK4_120_US       ((USHORT) 750 )
#define CLK4_110_US       ((USHORT) 688 )
#define CLK4_100_US       ((USHORT) 625 )
#define CLK4_90_US        ((USHORT) 563 )
#define CLK4_86_US        ((USHORT) 538 )
#define CLK4_85_US        ((USHORT) 531 )
#define CLK4_80_US        ((USHORT) 500 )
#define CLK4_75_US        ((USHORT) 469 )
#define CLK4_74_US        ((USHORT) 463 )
#define CLK4_1_US         ((USHORT) 6   )

// for CLK64 (timer resolution = 2560 ns)
#define CLK64_4_MS        ((USHORT) 1563)
#define CLK64_1_MS        ((USHORT) 391 )
#define CLK64_250_US      ((USHORT) 98  )
#define CLK64_100_US      ((USHORT) 39  )

#else /* PERIPHERAL_CLK_25MHZ  */
//----------------------------------------------------;
// assign values for time period                      ;
// Timers base clock is XIN = 33.8688MHz              ;
//----------------------------------------------------;

// for CLK16 (timer resolution = 472.41 ns)
#define CLK16_1000_US     ((USHORT) 2117)
#define CLK16_800_US      ((USHORT) 1693)
#define CLK16_500_US      ((USHORT) 1058)
#define CLK16_250_US      ((USHORT) 529 )
#define CLK16_200_US      ((USHORT) 423 )
#define CLK16_190_US      ((USHORT) 402 )
#define CLK16_150_US      ((USHORT) 317 )
#define CLK16_125_US      ((USHORT) 265 )
#define CLK16_120_US      ((USHORT) 254 )
#define CLK16_110_US      ((USHORT) 233 )
#define CLK16_100_US      ((USHORT) 212 )
#define CLK16_90_US       ((USHORT) 191 )
#define CLK16_86_US       ((USHORT) 182 )
#define CLK16_85_US       ((USHORT) 180 )
#define CLK16_80_US       ((USHORT) 170 )
#define CLK16_75_US       ((USHORT) 159 )
#define CLK16_74_US       ((USHORT) 154 )
#define CLK16_1_US        ((USHORT) 2 )
// for CLK64 (timer resolutin = 1889.64 ns)
#define CLK64_4_MS        ((USHORT) 2117)
#define CLK64_1_MS        ((USHORT) 529)
#define CLK64_250_US      ((USHORT) 132)
#define CLK64_100_US      ((USHORT) 53 )

#endif /* PERIPHERAL_CLK_25MHZ */
extern void   Set_Timer_Int_NON_STOP(timer*  ptmr, USHORT x);
extern void   InitializeAllTimers(void);
extern void   TimerMeasureStart(void);
extern ULONG  TimerMeasureStop(void);
extern void   Delay100uS(BYTE dly);
extern void   DelaymS(BYTE ms);
extern void   Delay1uS(BYTE dly);

/* **************************************************************************** */
/* Name:        SetTimerXValue                                                  */
/* Description: This function sets current value of the timer                   */
/* Input:       Pointer to the Timer -- TIMER                                   */
/*              The value to be set into the timer counter -- timer_value       */
/* Return:      None                                                            */
/* **************************************************************************** */
#define SetTimerXValue(x,y)      ((x.TVAL.USHOT) = (y))

/* **************************************************************************** */
/* Name: GetTimerXValue                                                         */
/* Description: This function return current value of the timer                 */
/* Input:       Pointer to the Timer -- TIMER                                   */
/* Return:      The current counter value                                       */
/* **************************************************************************** */
#define GetTimerXValue(x)        (x.TVAL.USHOT)

/* **************************************************************************** */
/* Name:        SetTimerXCompareValue                                           */
/* Description: This function sets compare value of the timer                   */
/* Input:       Pointer to the Timer -- TIMER                                   */
/*              The value to be set into the timer compare -- compare_value     */
/* Return:      None                                                            */
/* **************************************************************************** */
#define SetTimerXCompareValue(x,y)  ((x.TCMP.USHOT) = (y))

/* **************************************************************************** */
/* Name: GetTimerXCompareValue                                                  */
/* Description: This function return compare value of the timer                 */
/* Input:       Pointer to the Timer -- TIMER                                   */
/* Return:      The compare value                                               */
/* **************************************************************************** */
#define GetTimerXCompareValue(x) (x.TCMP.USHOT)

#define CLR_TIMER_STATUS(x)      (x.TSTATUS.BYTE = 0)
#define ENABLE_TIMER_INT(x)      (x.TCNTRL.BIT.INTEN =1)
#define DISABLE_TIMER_INT(x)     (x.TCNTRL.BIT.INTEN =0)
#define STOP_TIMER(x)            (x.TCNTRL.BIT.TSTRT =0)
#define START_TIMERS(x)          (x.TCNTRL.BIT.TSTRT =1)

#endif /* Timer.h*/

