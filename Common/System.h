/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   system.h
*
* DESCRIPTION
*
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*   1.00    Jan-01-02   Hatachi Create
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include ".\common\event.h"

typedef void (*FUNCPTR)(void);

typedef struct{
    BYTE        task_id_no;
    FUNCPTR     task_main;
    ULONG       *stack_ptr;
    USHORT      stack_size;
}TASK_ENTRY_TBL;


void    MakeProcess(BYTE process_id,FUNCPTR func,ULONG *init_stack_ptr,USHORT stack_size);
//void  ChangeProcess(BYTE process_id,FUNCPTR func);

void    StartProcess(BYTE start_process_id);
void    ExitProcess(void);
//void    EndProcess(void);
//void    DeleteProcess(BYTE process_id);

//void  SleepProcess(BYTE process_id);
//void  ResumeProcess(BYTE process_id);
void    StartTimer(ULONG *clock);
ULONG   ReadTimer(ULONG *clock);
void    StartTimer3(ULONG *clock);
ULONG   ReadTimer3(ULONG *clock);

void    WaitTimer(ULONG timer);
void    InitStack(BYTE process_no,ULONG *init_stack_ptr);
void    CheckStack(void);
void    EntryInterceptProcess(BYTE process_no);



/* the number of process */
#define PROCESS_MAX     4

/* the number of interrupt functions */
#define INTERRUPT_MAX   10

/* process number   */


#define pHIF        0
#define pPLAYER     1
#define pSERVO      2
#define pWRP        3
#define pTP         4
#define pFGP        5
#define pBGP        6

#define FOREVER   0xFFFFFFFFL

#endif /* _SYSTEM_H_ */

