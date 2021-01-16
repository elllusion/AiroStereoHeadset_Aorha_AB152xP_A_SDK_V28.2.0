?EP?P018_f1_0xFEFCD0?P018 SEGMENT 'ECODE_PATCH'
PUBLIC P018_f1_0xFEFCD0??
RSEG ?EP?P018_f1_0xFEFCD0?P018	;program segment

P018_f1_0xFEFCD0??:
DB		0xD0, 0xFC, 0xFF
DB		0
EJMP	P018_f1_patch

P018_f1_patch:
; Check whether gHopEnb is set or not
MOV	     DPTR, #gHopEnb
MOVX     A, @DPTR
JZ       P018_f1_2
; Increment hop is set to 1
MOV      DPTR, #gHopChannel
MOVX     A, @DPTR
INC      A
CMP      R11, #0x28
JC	     P018_f1_1
CLR      A
P018_f1_1:
CLR      C
; Set up the newest hopping frequency
MOV      R7,A
MOV      0xC1,#0x21
MOV      A,0xE6
ANL      A,#0xDF
ORL      A,#0x20
MOV      0xC1,#0x21
MOV      0xE6,A
MOV      0xC1,#0x2B
MOV      0xE6,R7
; Save the newest hopping channel into 0xF7
MOV      R11,R7
MOVX     @DPTR, A
; LE_ENABLE_DATA_TX();
P018_f1_2:
MOV      0xC1,#0x20
MOV      0xE6,#0x01
ERET