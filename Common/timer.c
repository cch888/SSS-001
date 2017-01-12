/****************************************************************************
*                (c) Copyright 2002 - 2008 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
* FILENAME:   timer.c

* NOTES:
*
*
* CHANGES:
* REF VERSION     DATE           WHO        DETAIL
* --- -------      ----          ---        ------
*       0.5   April-30-2002      XinL       Creation, draft
*
* DESCRIPTION:
*
*    This file contains some basic functions for timers.
*
*   $Revision: 50 $
*   $Date: 11/03/11 2:34p $
*
************************** SUNEXT CONFIDENTIAL *********************************** */

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\AL\REG_periph.h"
#include "..\SERVO\svo_lib.h"

/* **************************************************************************** */
/* Name: Set_Timer_Int_NON_STOP                                                 */
/* Description: This function set time to non stop mode and generate interrupt  */
/* Input:       Pointer to the Timer -- TIMER                                   */
/* Return:  The current counter value                                           */
/* **************************************************************************** */
void Set_Timer_Int_NON_STOP(timer*  ptmr, USHORT x)
{
    ptmr->TCNTRL.BIT.TSTRT = 0;
    ptmr->TCMP.USHOT = x;
    ptmr->TCNTRL.BYTE = (bTSTRT | INTENABLE | TIMERMODE);
}// end of Set_Timer_Int_NON_STOP

/********************************************************************************/
/*****                       Timer Initialization                           *****/
/********************************************************************************/

/* **************************************************************************** */
/* Name:        KernelTimerInit                                                 */
/* Description: This procedure initialize  the Kernel timer (Timer0)            */
/*              Kernal timer and LEDs timer. The count-up counter starts        */
/*              from 0.when it reaches the value preprogrammed into the GRA     */
/*              register it is being reset to 0000 to start again. When it      */
/*              reaches its end value it also generates an interrupt. The       */
/*              counter is programmed so that it generates an interrupt every   */
/*              1 ms.                                                           */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void KernelTimerInit (void)
{
    STOP_TIMER(TIMER0);

    TIMER0.TCNTRL.BYTE = 0;         // stop timer 0
    TIMER0.TCLK.BYTE   = CLK16;     // select "CLK/16", or resolution = 472.41 ns
    SetTimerXCompareValue(TIMER0, CLK16_1000_US); // generate interrupt every 1 ms

    TIMER0.TCNTRL.BYTE = (bTSTRT | INTENABLE | TIMERMODE);
                                    /* Start Timer, auto reset and start again, */
                                    /* Timer mode, internal(XIN) input          */
}// end of KernelTimerInit


/* **************************************************************************** */
/* Name:        PlayerTimerInit                                                 */
/* Description: This procedure initialize  the Player timer (Timer1)            */
/*              Player timerThe count-up counter starts from 0.when it reaches  */
/*              the programmed value in the TDAT register it is being reset to  */
/*              0 to start again. When it reaches its end value it also         */
/*              generates an interrupt.The counter is programmed so that it     */
/*              generates an interrupt every 250 uS                             */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void PlayerTimerInit (void)
{
    STOP_TIMER(TIMER1);

    TIMER1.TCLK.BYTE = CLK64;       /* set the divide to 1/64,529.2KHz or 1889ns */
    Set_Timer_Int_NON_STOP(&TIMER1, CLK64_250_US);  // do byte access to timer registors (h/w issue)
}


/* **************************************************************************** */
/* Name:        FGTimerInit                                                     */
/* Description: This procedure initialize  the FG timer (Timer2)                */
/*      FG Timer(Timer0) (16bit) caputure mode for FG measurement. The count-up */
/*      counter starts from 0. When it reaches its end value it also generates  */
/*      an interrupt.                                                           */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void FGTimerInit (void)
{
    STOP_TIMER(TIMER2);

    TIMER2.TCLK.BYTE    = CLK16;    /* set divide to 1/16, 2.1168MHz or 472.41ns */
    TIMER2.TCMCTRL.BYTE = (bTCMPOL|INPUT_FG);   /* Cycle, falling,IN3 (FG)*/
#if (CHIP_REV < 0xC0)
    TIMER2.TCNTRL.BYTE  = (bTSTRT | bTAUTOSTOP | INTENABLE | MEASUREMODE);
#else
    TIMER2.TCNTRL.BYTE  = (bTSTRT | INTENABLE | COUNTERMODE);
    SetTimerXCompareValue(TIMER2, DISK_FR2X + 1);
#endif
                                    /* Start Timer, auto reset and start again, */
                                    /* Measure mode,        */
}


/* **************************************************************************** */
/* Name:        ROPCTimerInit                                                   */
/* Description: This procedure initialize the ROPC timer    (Timer3)            */
/*              ROPC timer. The count-up counter starts from 0. when it reaches */
/*              the value preprogrammed into the GRA register it is being reset */
/*              to 0000 to start again When it reaches its end value it also    */
/*              generates an interrupt.The counter is programmed so that it     */
/*              generates an interrupt every 1 ms.                              */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void ROPCTimerInit (void)
{
    STOP_TIMER(TIMER3);

    SetTimerXCompareValue(TIMER3, CLK64_1_MS);

    TIMER3.TCLK.BYTE = CLK64;       /* set the divide to 1/64, 529.2KHz or 1889ns */
    TIMER3.TCNTRL.BYTE = (bTSTRT | bTAUTOSTOP | INTENABLE | TIMERMODE);
                                    /* Start Timer, auto reset and start again, */
                                    /* Timer mode, internal(XIN) input          */
}

/* **************************************************************************** */
/* Name:        FreeRunningTimerInit                                            */
/* Description: This procedure initialize the Free Run 250us timer  (Timer3)    */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void FreeRunningTimerInit(void)
{
    STOP_TIMER(TIMER3);

    TIMER3.TCLK.BYTE = CLK64;       /* set the divide to 1/64,529.2KHz or 1889ns */
    Set_Timer_Int_NON_STOP(&TIMER3, CLK64_100_US);
}

/* **************************************************************************** */
/* Name:        MeasTimerInit                                                   */
/* Description: This procedure initialize  the Meas timer  (Timer 4)            */
/*              Servo timer. The timer counter starts from 0. It stops when     */
/*              it reaches its maximum value .                                  */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void MeasTimerInit (void)
{
    STOP_TIMER(TIMER4);

    SetTimerXCompareValue(TIMER4, 0xFFFF);

    TIMER4.TCLK.BYTE = CLK1024;       /* set the divide to 1/1024, 33.1KHz or 30.23432us */
    TIMER4.TCNTRL.BYTE = (bTAUTOSTOP | INTDISABLE | TIMERMODE);
                                    /* Start Timer, auto stop, do not generate interrupt */
                                    /* Timer mode, internal(XIN) input          */
}

#if DO_HW_SUPPORT_STEP == 0
/* **************************************************************************** */
/* Name:        StepperTimerInit                                                */
/* Description: This procedure initialize  the Stepper timer (Timer 5)          */
/*              Stepper timer. The count-up counter starts from 0.when it       */
/*              reaches the value preprogrammed into the GRA register it is     */
/*              being reset to 0000 to start again. When it reaches its end     */
/*              value it also generates an interrupt. The counter is programmed */
/*              later on based on the status                                    */
/*                                                                              */
/*              This timer is used to support F/W replacement of "Step Motor    */
/*              Control Block". Once the H/W is fixed. This resurce will be     */
/*              released !!!                                                    */
/*                                                                              */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void StepperTimerInit (void)
{
    STOP_TIMER(TIMER5);

    SetTimerXCompareValue(TIMER5, 0xffff);    // temp. large number

    TIMER5.TCLK.BYTE   = CLK16;                  // set the divid to 472.41ns
    TIMER5.TCNTRL.BYTE = (bTSTRT | INTENABLE | TIMERMODE);
                                                 // Start Timer, auto stop, do not generate interrupt
                                                 // Timer mode, internal(XIN) input
}// end of StepperTimerInit
#endif /* DO_HW_SUPPORT_STEP */

/* **************************************************************************** */
/* Name:        InitializeTimer6                                                */
/* Description: This procedure initialize the 472.41ns timer  (Timer6)          */
/*              Set up to compare on FFFFh with a timer time of 30.96 mS        */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void InitializeTimer6(void)
{
    STOP_TIMER(TIMER6);

    TIMER6.TCLK.BYTE = CLK16;       /* set divide to 1/16, 2.1168MHz or 472.41ns */
    SetTimerXCompareValue(TIMER6, 0xffff);    // temp. large number
    TIMER6.TCNTRL.BYTE = (bTSTRT | TIMERMODE);
}

/* **************************************************************************** */
/* Name:        InitializeTimer7                                                */
/* Description: This procedure initialize the 100ss timer  (Timer7)             */
/*              Timer 7 will be used for Cache based delays                     */
/*              Set up to compare on FFFFh with a timer time of 30.96 mS        */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void InitializeTimer7(void)
{
    STOP_TIMER(TIMER7);

    TIMER7.TCLK.BYTE = CLK16;       /* set divide to 1/16, 2.1168MHz or 472.41ns */
    SetTimerXCompareValue(TIMER7, CLK16_100_US);    // 100 uS per count
    TIMER7.TCNTRL.BYTE = (bTAUTOSTOP | TIMERMODE);
}

/* **************************************************************************** */
/* Name:        InitializeAllTimers                                             */
/* Description: This procedure initialize  all timers                           */
/* Input:       None                                                            */
/* Return:      None                                                            */
/* **************************************************************************** */
void InitializeAllTimers (void)
{
    KernelTimerInit();          /* Timer0 */
    PlayerTimerInit();          /* Timer1 */
    FGTimerInit();              /* Timer2 */
    FreeRunningTimerInit();     /* Timer3 */
    MeasTimerInit();            /* Timer4 */
    STOP_TIMER(TIMER4);

#if DO_HW_SUPPORT_STEP == 0
    StepperTimerInit();         /* Timer5 */
#endif /* DO_HW_SUPPORT_STEP */

    InitializeTimer6();         /* Timer6 */
    InitializeTimer7();         /* Timer7 - Delay timer from 0.1mS to 6sec */
    STOP_TIMER(TIMER7);         /* Timer7 */
}

/*******************************************************************************/
/*  Subroutine  |TimerMeasureStart                                             */
/*  Function    | start Timer4 for time measurement.                           */
/*              | resolution: 30.23432us, Maximum measure time: 1981.4ms       */
/*              | WARNING: Timer function is not re-entrant.                   */
/*  Input       |None                                                          */
/*              |                                                              */
/*  Output      |None                                                          */
/*******************************************************************************/
void TimerMeasureStart(void)
{
    if (TIMER4.TCNTRL.BIT.TSTRT)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33FF1D); /* Timer 4 is busy */
        return;
    }
    START_TIMERS(TIMER4);
}

/*******************************************************************************/
/*  Subroutine  |TimerMeasureStop                                              */
/*  Function    | stop timer4 and convert its unit to 10us                     */
/*              | WARNING: Timer function is not re-entrant.                   */
/*  Input       |None                                                          */
/*              |                                                              */
/*  Output      |measured time [10us]                                          */
/*******************************************************************************/
ULONG TimerMeasureStop(void)
{
    ULONG meas_time;

    meas_time = GetTimerXValue(TIMER4);
#if PERIPHERAL_CLK_25MHZ == 1
    meas_time = meas_time * 512 / 125;
#else
    meas_time = meas_time * 387 / 128;
#endif
    STOP_TIMER(TIMER4);

    return meas_time;
}

/*******************************************************************************/
/*  Subroutine  |Delay100uS                                                    */
/*  Function    | delay time based on timer 7. Delays are in increments of     */
/*              | 100us                                                        */
/*              | Timer will stop at TC                                        */
/*              | WARNING: Timer function is not re-entrant. Do not use in INT */
/*  Input       |dly (delay in 100 us increments)   0.1, 0.2..0.9, 1, 2 3..    */
/*              |                                                              */
/*  Output      |None                                                          */
/*******************************************************************************/
void Delay100uS(BYTE dly) // delay in 100us units, max = 25.6 ms.
{
    STOP_TIMER7;
    CLR_TIMER_STATUS(TIMER7);                       // Clear the status bits
    TIMER7.TCLK.BYTE = CLK16;                       /* set divide to 1/16, 2.1168MHz or 472.41ns */
    TIMER7.TCNTRL.BYTE = (bTAUTOSTOP | TIMERMODE);
    SetTimerXCompareValue(TIMER7, CLK16_100_US*dly);// 100 uS per count, max =65535/212 = 30.9 ms
    START_TIMER7;                                   // Start Delay timer

    while(TIMER7.TSTATUS.BIT.TBUSY);                // Wait until timer completes, Not BUSY

    STOP_TIMER7;
}

/*-----------------------------------------------------------------------------
 *
 * Module: DelaymS()
 * Description: Wait for a very short time and other thread cannot running either.
 *              If one need longer delay use WaitTimer() instead.
 *              For servo state change delay please use SetDelayTimer().
 *
 *----------------------------------------------------------------------------*/
void DelaymS(BYTE ms) // max = 256ms
{
    for ( ; ms ; ms--)
    {   // delay 1ms
        Delay100uS(10);
    }
}

/*******************************************************************************/
/*  Subroutine  |Delay1uS                                                      */
/*  Function    | delay time based on timer 7. Delays are in increments of     */
/*              | 100us                                                        */
/*              | Timer will stop at TC                                        */
/*              | WARNING: Timer function is not re-entrant. Do not use in INT */
/*  Input       |dly (delay in 100 us increments)   0.1, 0.2..0.9, 1, 2 3..    */
/*              |                                                              */
/*  Output      |None                                                          */
/*******************************************************************************/
void Delay1uS(BYTE dly) // delay in 100us units, max = 25.6 ms.
{
    STOP_TIMER7;
    CLR_TIMER_STATUS(TIMER7);                       // Clear the status bits
    TIMER7.TCLK.BYTE = CLK16;                       /* set divide to 1/16, 2.1168MHz or 472.41ns */
    TIMER7.TCNTRL.BYTE = (bTAUTOSTOP | TIMERMODE);
    SetTimerXCompareValue(TIMER7, (CLK16_10_US*((USHORT)dly))/10);  // 100 uS per count, max =65535/21 = 31.2 ms
    START_TIMER7;                                   // Start Delay timer

    while (TIMER7.TSTATUS.BIT.TBUSY);               // Wait until timer completes, Not BUSY

    STOP_TIMER7;
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
