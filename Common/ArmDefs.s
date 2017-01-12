;****************************************************************************
;                (c) Copyright 2001-2005 SUNEXT DESIGN, INC.
;                           All Right Reserved
;
;
;
; FILE NAME : ARMDEFS.S
;
; DESCRIPTION:  ARM header file for Assembly code
;               File contains Register definitions and ARM Memory Map information
;               for Assembly code.
;               Entries in this file are duplicates of definitions found in
;               files chipreg.h and armreg.h
;               Files chipreg.h and armreg.h cannot be used in assembly code
;               because of differing format requirements of ARM Assembler.
;
; $Revision: 1 $
; $Date: 4/29/04 11:24a $
;
;**************************** SUNEXT CONFIDENTIAL *******************************/

rwREMAPCTL      EQU     0xC8000004          ; Address of Remap Control Register Low
rwREMAPCTH      EQU     0xC8000005          ; Address of Remap Control Register High
wICIEnClr       EQU     0xFFFFF014          ; Address of Interrupt Enable Clear Register
wICSWIClr       EQU     0xFFFFF01C          ; Address of Software Interrupt Clear Register
rwREF00ASM      EQU     0x1000096E          ; REFS00 Register
rwSATAFIS       EQU     0x10000B90          ; SATA FIS Regeisters b90~b93
rwLDOFF         EQU     0xB000021C          ; LD Off Register
rwDMOFF         EQU     0xB0000224          ; Disc(spindle) Off Register
rwGPDACMUX      EQU     0xB000029C          ; Step Output Select
rwDACDAT        EQU     0xB00002A8          ; Step Output Dac
rwDELTA0        EQU     0xB0000414          ; Delta 0 Maximum and Minimum setting Register
rwDELTA1        EQU     0xB0000418          ; Delta 1 Maximum and Minimum setting Register
rwDELTA2        EQU     0xB000041C          ; Delta 2 Maximum and Minimum setting Register
rwPDMALPHA      EQU     0xB00004AC          ; Colli Output Dac
                END
