;****************************************************************************
;                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
;                           All Right Reserved
;
;
;
; FILE NAME : vectors.s
; DESCRIPTION:  ARM init code
; CHANGES:
; VERSION   DATE        WHO         DETAIL
;**************************** SUNEXT CONFIDENTIAL *******************************/

        AREA Vect, CODE, READONLY

        GET common\armdefs.s

; *****************
; Exception Vectors
; *****************

; Note: LDR PC instructions are used here, though branch (B) instructions
; could also be used, unless the ROM is at an address >32MB.

        ENTRY
;----------------------------------------------------------------------------
;       Begin of ARM Vector Table
;----------------------------------------------------------------------------
        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr
;----------------------------------------------------------------------------
;       End of ARM Vector Table
;----------------------------------------------------------------------------

        IMPORT  ResetHandler            ; In cstart.s
        IMPORT  C_IRQ_Handler           ; In intctl.c
        IMPORT  C_FIQ_Handler           ; In intctl.c

Reset_Addr      DCD     ResetHandler
Undefined_Addr  DCD     Undefined_Handler
SWI_Addr        DCD     SWI_Handler
Prefetch_Addr   DCD     Prefetch_Handler
Abort_Addr      DCD     Abort_Handler
IRQ_Addr        DCD     IRQ_Handler
FIQ_Addr        DCD     FIQ_Handler


; ************************
; Exception Handlers
; ************************

; exception handler for undefined instructions

Undefined_Handler
        B       Servo_Off

SWI_Handler
        B       SWI_Handler

Prefetch_Handler
        B       Servo_Off

Abort_Handler
        B       Servo_Off

IRQ_Handler
        SUB     lr, lr, #4          ; reserved for PC
        STMFD   sp!, {r0-r12, lr}
        BL      C_IRQ_Handler
        LDMFD   sp!, {r0-r12, pc}^  ; Get spsr from stack

FIQ_Handler
;        B       FIQ_Handler
        SUB     lr, lr, #4          ; reserved for PC
        STMFD   sp!, {r0-r7, lr}
        BL      C_FIQ_Handler
        LDMFD   sp!, {r0-r7, pc}^  ; Get spsr from stack

Servo_Off
; Disable all interrupt, clear all software interrupt.
        LDR     r0, = 0xFFFFFFFF
        LDR     r1, = wICIEnClr
        STR     r0, [r1]
        LDR     r1, = wICSWIClr
        STR     r0, [r1]

; switch off step output
        LDR     r0, = 0x20
        LDR     r1, = rwGPDACMUX
        STR     r0, [r1]
        LDR     r0, = 0x00
        LDR     r1, = rwDACDAT
        STR     r0, [r1]

; switch off colli output
        LDR     r1, = rwPDMALPHA
        STR     r0, [r1]

; switch off focus, tracking
        LDR     r1, = rwLDOFF
        STR     r0, [r1]

; switch off spindle motor
        LDR     r1, = rwDMOFF
        STR     r0, [r1]

; switch off laser
        LDR     r0, = 0x00000000
        LDR     r1, = rwDELTA0
        STR     r0, [r1]
        LDR     r1, = rwDELTA1
        STR     r0, [r1]
        LDR     r1, = rwDELTA2
        STR     r0, [r1]

loop
        B      loop

        END

