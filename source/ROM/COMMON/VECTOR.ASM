#include "config.h"
#include "DQ80251.h"
#include "hw_sfr.h"
$include (MODEL.INC)
$include (ASM_MACRO.INC)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; External Symbols
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EXTRN PRSeg (CTXSWITCH?{POSTFIX})



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assembly Begin
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NAME INTR_VECTOR



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Interrupt Vector
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                    CSEG AT 0003H
EXT_INT0_ENTRY? :   PUSH    R11
                    MOV     A,#0
                    JUMP    CTXSWITCH?{POSTFIX}

                    CSEG AT 000BH
EXT_T0_ENTRY? :     PUSH    R11
                    MOV     A,#1
                    JUMP    CTXSWITCH?{POSTFIX}

                    CSEG AT 0013H
EXT_INT1_ENTRY? :   PUSH    R11
                    MOV     A,#2
                    JUMP    CTXSWITCH?{POSTFIX}

                    CSEG AT 001BH
EXT_T1_ENTRY? :     PUSH    R11
                    MOV     A,#3
                    JUMP    CTXSWITCH?{POSTFIX}

                    CSEG AT 002BH
EXT_T2_ENTRY? :     PUSH    R11
                    MOV     A,#4
                    JUMP    CTXSWITCH?{POSTFIX}



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assembly End
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
END

