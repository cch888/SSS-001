;****************************************************************************
;                (c) Copyright 2001-2005 SUNEXT DESIGN, INC.
;                           All Right Reserved
;
;
;
; FILE NAME : CSTART.S
; DESCRIPTION:  ARM initialization code
;
; $Revision: 1 $
; $Date: 4/29/04 11:24a $
;
;**************************** SUNEXT CONFIDENTIAL *******************************/

    AREA Cstart, CODE, READONLY

    GET common\armdefs.s

;-------------------------------------------------------
;   Public definitions
;-------------------------------------------------------
    EXPORT  ResetHandler

;-------------------------------------------------------
;   External definitions
;-------------------------------------------------------
    IMPORT  init_system_first  ; init. system  First!

;-------------------------------------------------------
;   ISR External definitions
;-------------------------------------------------------

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F ; available on ARM Arch 4 and later

I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled


; --- Amount of memory (in bytes) allocated for stacks

Len_FIQ_Stack   EQU     0   ; Uncomment this line when ENABLE_FIQ == 0
; Len_FIQ_Stack   EQU     128   ;Uncomment this line when ENABLE_FIQ == 1
Len_ABT_Stack   EQU     0
Len_UND_Stack   EQU     0
Len_SVC_Stack   EQU     0
Len_IRQ_Stack   EQU     256
Len_USR_Stack   EQU     64

; Add lengths >0 for FIQ_Stack, ABT_Stack, UND_Stack if you need them.
; Offsets will be loaded as immediate values.
; Offsets must be 8 byte aligned.

Offset_FIQ_Stack     EQU     0
Offset_ABT_Stack     EQU     Offset_FIQ_Stack + Len_FIQ_Stack
Offset_UND_Stack     EQU     Offset_ABT_Stack + Len_ABT_Stack
Offset_SVC_Stack     EQU     Offset_UND_Stack + Len_UND_Stack
Offset_IRQ_Stack     EQU     Offset_SVC_Stack + Len_SVC_Stack
Offset_USR_Stack     EQU     Offset_IRQ_Stack + Len_IRQ_Stack


        ENTRY
; On reset, an aliased copy of ROM is at 0x0.
; Continue execution from 'real' ROM rather than aliased copy
        LDR     pc, =Instruct_2

Instruct_2

; Enable FIS34 RDY CNT
        LDR     r0, = 0x7F000200
        LDR     r1, =rwSATAFIS
        STR     r0, [r1]

; Enable servo block power and select internal bias resistor.
        MOV     r0, #0x7E
        LDR     r1, =rwREF00ASM
        STR     r0, [r1]

; Right now code is execute in the 'real' ROM address
; Remap by clear Remap bit of the REMAP_ctl register
        MOV     r0, #0
        LDR     r1, = rwREMAPCTL
        STR     r0, [r1]

; RAM is now at 0x0.
; The exception vectors (in vectors.s) will be copied from ROM to the RAM
; The copying is done later by the C library code inside __main
		
		IMPORT	bottom_of_heap	; TBD? COMMENT! need comments!
		LDR		r1, =bottom_of_heap
		STR     r0, [r1]

ResetHandler
; Disable all interrupt, clear all software interrupt.
        LDR     r0, = 0xFFFFFFFF
        LDR     r1, = wICIEnClr
        STR     r0, [r1]
        LDR     r1, = wICSWIClr
        STR     r0, [r1]

; --- Initialize stack pointer registers
; Enter each mode in turn and set up the stack pointer

        IMPORT  top_of_stacks       ; defined in stack.s and located by scatter file
        LDR     r0, =top_of_stacks
        
        MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit ; no interrupts
        SUB     sp, r0, #Offset_FIQ_Stack

        MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit ; no interrupts
        SUB     sp, r0, #Offset_IRQ_Stack

; --- Now change to User mode and set up User mode stack, if needed
;       MSR     CPSR_c, #Mode_USR:OR:F_Bit ; enable IRQ interrupts
        MSR     CPSR_c, #Mode_USR          ; enable IRQ/FIQ interrupts
        SUB     sp, r0, #Offset_USR_Stack

; --- Initialize critical IO devices
        BL  init_system_first          ; init. system  First!

        IMPORT  __main

; --- Now enter the C code
        B       __main   ; note use B not BL, because an application will never return this way

    END
