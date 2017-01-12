;****************************************************************************
;*                (c) Copyright 2002,2003 SUNEXT DESIGN, INC.
;*                           All Right Reserved
;*
;*  FILE NAME   : Boot.s
;*  DATE        : 2002/05/17
;*  MAKE        : K.Toyota
;*
;**************************** SUNEXT CONFIDENTIAL ****************************

;_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
;   ARM Source Code.
;_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
    AREA Word, CODE, READONLY       ; name this block of code

    EXPORT  Boot
    EXPORT  PollInterrupt

    IMPORT  top_of_stacks
;   IMPORT  FRM_Wr_Pr
    IMPORT  ResetHandler

;****************************************************************************
Boot
    LDR r0,=top_of_stacks
    BL  ResetHandler

;****************************************************************************
PollInterrupt
    BL  PollInterrupt
;   BL  FRM_Wr_Pr

    END

