?EP?P003_f1_0xFF78C0?P003 SEGMENT 'ECODE_FLASH'
PUBLIC P003_f1_0xFF78C0??
RSEG ?EP?P003_f1_0xFF78C0?P003	;program segment

P003_f1_0xFF78C0??:
DB		0xC0, 0x78, 0xFF
DB		0
EJMP	P003_f1_patch

P003_f1_patch:
; Check tx count value
MOV     WR6,00:87D4H
CMP     WR6,00:87D6H
JNE		P003_f1_0xFF78C0_1
EJMP    0xFF7AF2

P003_f1_0xFF78C0_1:
; Check whether gHopEnb is set or not
MOV	     DPTR, #gHopEnb
MOVX     A, @DPTR
JZ       P003_f1_0xFF78C0_3
; Increment hop is set to 1
MOV      DPTR, #gHopChannel
MOVX     A, @DPTR
INC      A
CMP      R11, #0x28
JC	     P003_f1_0xFF78C0_2
CLR      A
P003_f1_0xFF78C0_2:
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
P003_f1_0xFF78C0_3:
MOV      0xC1,#0x20
MOV      0xE6,#0x01
ERET