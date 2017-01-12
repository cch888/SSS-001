/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   prcon.c
*
* DESCRIPTION
*
*
*
* $Revision: 28 $
* $Date: 10/12/21 4:16p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/



#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\system.h"
#include ".\common\chip.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#include ".\servo\svo.h"

void schedule_process(void);
//void _change_process(void);

#define P_DEAD          0
#define P_READY         1
#define P_RUN           2
#define P_WAIT_EVENT    3
#define P_WAIT_TIME     4
#define P_SLEEP         5

extern ServoInterfaceInfoS     SIF;
extern vULONG   Iram_system_timer;
extern vULONG   Iram_timer3_counter;
extern vBYTE    Iram_intercept_process;
extern const    TASK_ENTRY_TBL task_tbl[PROCESS_MAX];

//ULONG system_event[PROCESS_MAX];
enum {eRegiMax = 16};
ULONG   regi[PROCESS_MAX][eRegiMax];
BYTE    process_no;
BYTE    process_state[PROCESS_MAX];
//ULONG RemStack[PROCESS_MAX];
ULONG   InitStackPtr[PROCESS_MAX];
BYTE    w_process_id;
FUNCPTR w_func;


BYTE    previous_process_no;

//ULONG process_time[PROCESS_MAX];
//ULONG process_clock[PROCESS_MAX];


/******************************************************************/
/*  Subroutine  |Save_CPU_register                                */
/*  Function    |Save register, exit process                      */
/*  Input       |Any register in CPU                              */
/*  Output      |regCCR,regER7,regER0,regER1,regER2,regER3,       */
/*              |regER4,regER5,regER6                             */
/*  Global      |_process_no                                      */
/*  Editor      |K.Hatachi                                        */
/*  Update      |99/04/12                                         */
/******************************************************************/
extern void Save_CPU_register(void);

/******************************************************************/
/*  Subroutine  |Restore register & Switch process                */
/*  Function    |Restore register,and go into process             */
/*  Input       |regCCR,regER7,regER0,regER1,regER2,regER3,       */
/*              |regER4,regER5,regER6                             */
/*  Output      |Any register in CPU                              */
/*  Global      |r_processno                                      */
/*  Editor      |K.Hatachi                                        */
/*  Update      |99/04/12                                         */
/******************************************************************/
extern void Restore_CPU_register(void);

/******************************************************************/
/*  Subroutine  |MakeProcess                                     */
/*  Function    |process is entried                               */
/*  Input       |None                                             */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |99/04/12                                         */
/******************************************************************/
void MakeProcess(BYTE process_id,FUNCPTR func,ULONG *init_stack_ptr,USHORT stack_size)
{
    BYTE    i;

    init_stack_ptr += ((stack_size / 4) - 1);
    *init_stack_ptr = (ULONG)func;
    init_stack_ptr--;

    for(i = 0;i < eRegiMax;i++) regi[process_id][i] = 0L;
    regi[process_id][13] = (ULONG)init_stack_ptr;       /* intilaize register       */

//  system_event[process_id]   = 0x0L;
    process_state[process_id]  = P_READY;

    Iram_intercept_process = 0xFF;

}

#if 0 // Not Used, Keep for future
/******************************************************************/
/*  Subroutine  |ChangeProcess                                   */
/*  Function    |process changes                                  */
/*  Input       |None                                             */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/10/10                                         */
/*                                                                */
/*  Descript:                                                     */
/*  refer to ChangeProcess() Macro which is defined in system.h  */
/*                                                                */
/******************************************************************/

void ChangeProcess(BYTE process_id,FUNCPTR func)
{
    w_process_id = process_id;
    w_func = func;

    _change_process();

    if(w_process_id == process_no){
        Restore_CPU_register();
    }
}


void _change_process(void)
{
    BYTE    i;
    ULONG   *init_stack_ptr;
    USHORT  stack_size;

    init_stack_ptr = task_tbl[w_process_id].stack_ptr;
    stack_size     = ((task_tbl[w_process_id].stack_size / 4) - 1);

    init_stack_ptr     += stack_size;
    *init_stack_ptr = (ULONG)w_func;
    init_stack_ptr--;

    for(i = 0;i < eRegiMax;i++) regi[w_process_id][i] = 0L;
    regi[w_process_id][13] = (ULONG)init_stack_ptr;     /* intilaize register       */

//  system_event[w_process_id] = 0x0L;
    process_state[w_process_id] = P_READY;
}
#endif

/******************************************************************/
/*  Subroutine  |StartProcess                                    */
/*  Function    |process is entried                               */
/*  Input       |start_process_no                                 */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/19/10                                         */
/******************************************************************/
void StartProcess(BYTE start_process_id)
{
    process_no = start_process_id;
    previous_process_no = start_process_id;
    process_state[process_no] = P_RUN;
    Restore_CPU_register();
}

#if 0 // Not Used, Keep for Future
/******************************************************************/
/*  Subroutine  |EndProcess                                      */
/*  Function    |process ends                                     */
/*  Input       |process_id                                       */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/10/10                                         */
/******************************************************************/
void EndProcess(void)
{
    process_state[process_no] = P_DEAD;
    while(1) ExitProcess();
}

/******************************************************************/
/*  Subroutine  |EndProcess                                      */
/*  Function    |deletes process                                  */
/*  Input       |process_id                                       */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/10/10                                         */
/******************************************************************/
void DeleteProcess(BYTE process_id)
{
    process_state[process_id] = P_DEAD;
}

/******************************************************************/
/*  Subroutine  |SleepProcess                                    */
/*  Function    |process sleeps                                   */
/*  Input       |process_no                                       */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/09/08                                         */
/******************************************************************/
void SleepProcess(BYTE process_id)
{
    process_state[process_id] = P_SLEEP;
    if(process_id == process_no){
        while(process_state[process_id] == P_SLEEP) ExitProcess();
    }
}

/******************************************************************/
/*  Subroutine  |ResumeProcess                                   */
/*  Function    |process resumes                                  */
/*  Input       |process_no                                       */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/09/08                                         */
/******************************************************************/
void ResumeProcess(BYTE process_id)
{
    process_state[process_id] = P_READY;
}
#endif


/******************************************************************/
/*  Subroutine  |ExitProcess                                     */
/*  Function    |save register and execute a next process         */
/*  Input       |None                                             */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/09/08                                         */
/******************************************************************/
void ExitProcess(void)
{
    Save_CPU_register();
    /* Check if current task stack overflow before
           switch to next stack */
    if ((*(BYTE *)InitStackPtr[process_no]) != (process_no+1))
    {
#if (MSG_OUT_MODE == MSG_USBPORT)
        send_msg54(SHOW_DEBUG_MSG,'S','T','O',process_no);
        CheckSramStack();
#endif
        while(1)
        {
        }
    }
    schedule_process();
//  CheckStack();
    Restore_CPU_register();
}

BYTE SIFStateOld;

void schedule_process(void)
{
//  process_time[process_no]  = ReadTimer(&process_clock[process_no]);

    process_state[process_no] = P_READY;

    /* Check if the high priority process executes " MDF-1" */
    if( Iram_intercept_process != 0xFF ){
        previous_process_no = process_no;
        process_no = Iram_intercept_process;
        Iram_intercept_process = 0xFF;
        process_state[process_no] = P_RUN;
        return;
    }

    process_no = previous_process_no;

    while(1){
        process_no++;
        if(process_no >= PROCESS_MAX) process_no = 0;
        if(process_state[process_no] == P_READY)        break;
        if(process_state[process_no] == P_WAIT_TIME)    break;
        if(process_state[process_no] == P_WAIT_EVENT)   break;
    }

    previous_process_no = process_no;

//  StartTimer(&process_clock[process_no]);
#if 0 // Debug of SIF interface
    process_state[process_no] = P_RUN;

    if(SIF.ServoExecutionState != SIFStateOld)
    {   // At this time (after every Task Switch) we want to check if the SIF (Servo Interface)
        // has changed its Status. If this is the case send a message to the monitor if messages are enabled.
        // This helps debugging issues of communication through SIF by Player and Servo.
        SIFStateOld = SIF.ServoExecutionState;
        send_msg80(SEND_WATCH_MSG,0xE70E00 + SIFStateOld );        // SIF State READY
                                                                          //           BUSY
                                                                          //           ERROR
    }
#endif
    process_state[process_no] = P_RUN;
}

/******************************************************************/
/*  Subroutine  |wait_process_timer                               */
/*  Function    |waiting time                                     */
/*  Input       |timer                                            */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |00/09/08                                         */
/******************************************************************/
/*-----------------------------------------------------------------------------
 *
 * Module: WaitTimer()
 * Description:Wait for a long time while other thread can still running.
 *             If one need a very accurate and short delay use DelaymS() or Delay100uS() instead.
 *             For servo state change delay please use SetDelayTimer().
 *             timer <= actual delay time < timer + 1
 *
 *----------------------------------------------------------------------------*/
void WaitTimer(ULONG timer)
{
    ULONG clock, temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    clock = Iram_system_timer;
    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */

    process_state[process_no] = P_WAIT_TIME;

    while (ReadTimer(&clock) <= timer)
    {
        ExitProcess();
    }

    process_state[process_no] = P_RUN;
}

/******************************************************************/
/*  Subroutine  |Start timer                                      */
/*  Function    |start timer                                      */
/*  Input       |clcok                                            */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |99/04/12                                         */
/******************************************************************/
void StartTimer(ULONG *clock)
{
    ULONG   temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    *clock = (ULONG)Iram_system_timer;
    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */
}


/******************************************************************/
/*  Subroutine  |Read timer                                       */
/*  Function    |ReadTimer                                       */
/*  Input       |clock                                            */
/*  Output      |timer values                                     */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |99/04/12                                         */
/******************************************************************/
ULONG ReadTimer(ULONG *clock)
{
    ULONG temp_IE, temp;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    temp = (Iram_system_timer >= *clock)? ((ULONG)Iram_system_timer - *clock) : ((ULONG)Iram_system_timer + (~(*clock)));
    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */

    return (temp);
}
/******************************************************************/
/*  Subroutine  |Start timer3(250us)                              */
/*  Function    |start timer3(250us)                              */
/*  Input       |clcok                                            */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |K.Nakatsu                                        */
/*  Update      |02/07/12                                         */
/******************************************************************/
void StartTimer3(ULONG *clock)
{
    ULONG   temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    *clock = (ULONG)Iram_timer3_counter;
    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */
}

/******************************************************************/
/*  Subroutine  |Read timer3(250us)                               */
/*  Function    |ReadTimer3(250us)                               */
/*  Input       |clock                                            */
/*  Output      |timer values                                     */
/*  Global      |                                                 */
/*  Editor      |K.Nakatsu                                        */
/*  Update      |02/07/12                                         */
/******************************************************************/
ULONG ReadTimer3(ULONG *clock)
{
    ULONG temp_IE, temp;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    temp = ((ULONG)Iram_timer3_counter - *clock) * 250L;
    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */

    return (temp);
}

/******************************************************************/
/*  Subroutine  |                                                 */
/*  Function    |InitStack                                        */
/*  Input       |                                                 */
/*  output      |                                                 */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |01/11/05                                         */
/******************************************************************/
void InitStack(BYTE process_no,ULONG *init_stack_ptr)
{
    InitStackPtr[process_no] = (ULONG)init_stack_ptr;

}

/******************************************************************/
/*  Subroutine  |                                                 */
/*  Function    |EntryInterceptProcess                            */
/*  Input       |                                                 */
/*  output      |                                                 */
/*  Global      |                                                 */
/*  Editor      |K.Hatachi                                        */
/*  Update      |01/11/05                                         */
/******************************************************************/
void EntryInterceptProcess(BYTE process_no)
{
    Iram_intercept_process = process_no;

}

