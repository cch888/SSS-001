/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   ISR.H
*
* DESCRIPTION
*
*   $Revision: 15 $
*   $Date: 08/09/22 6:44p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _ISR_
#define _ISR_

/**************************************
         External functions
***************************************/
extern void ATIPMSFReadyIntProc(void);
extern void OPCISRProc(void);
extern void KernelTimer1Update(void);
extern void KernelTimer3Update(void);
extern void svoIsr(void);
extern void dvOPCISRProc(void);
extern void atip_int_opc_proc(void);

/**************************************
       External variables
***************************************/
extern vULONG   Iram_system_timer;
extern vULONG   Iram_timer3_counter;
extern vBYTE    Iram_enable_ropc_isr;
extern vBYTE    Iram_enable_kernel_timer;
extern vBYTE    Iram_AtipAdipLppErrorAmount;    /* The amount of ATIP error per second in ATIP */
extern vBYTE    Iram_AtipAdipLppErrorCount;     /* ATIP/ADIP/LPP error count variable */
extern vBYTE    Iram_ECCBlockCount;

#endif          /* #ifndef _ISR_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
