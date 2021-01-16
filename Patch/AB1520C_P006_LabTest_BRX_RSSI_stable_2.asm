?EP?P006_f1_0xFE75C8?P006 SEGMENT 'ECODE_FLASH'
PUBLIC P006_f1_0xFE75C8??
RSEG ?EP?P006_f1_0xFE75C8?P006	;program segment

P006_f1_0xFE75C8??:
DB		0xC8, 0x75, 0xFE
DB		0
EJMP	P006_f1_patch

P006_f1_patch:

//P006_f1_0xFE74B8:
; AgcGainSel = 0x8ffff
INC  0x0B
;

MOV	    DPTR, #gAgcflag
MOV     0x93, #0
MOVX    A, @DPTR 

JZ      P006_f1_patch_01 

MOV     R11,@WR2+0x0014
JNZ     P006_f1_patch_00
MOV     R11,@WR2+0x0013
ANL     A,#0xFE
JZ      P006_f1_patch_01  //B1==0(<0xFFF) error is small

P006_f1_patch_00:
MOV     A, #0x00    //AGC ==0
EJMP	P006_f1_patch_02

P006_f1_patch_01:  //AGC !=0
MOV     A, #0x01   //normal  
P006_f1_patch_02:

MOV     @WR2+0x0015,R11
MOV     A, #0x00
MOVX    @DPTR, A

;MOV      R10, 0xE000
//MOV	    DPTR, #gAgcgain
MOV	    DPTR, #gAgcval
MOV     0x93, #0
MOVX    A, @DPTR 
//MOV     @WR2+0x0012, R11 //test
MOV     R10, R11 

JNZ     P006_f1_patch_03

MOV     A, #0x00    //AGC ==0
MOV     @WR2+0x0015,R11

P006_f1_patch_03:
MOV     A, #0x00
MOVX    @DPTR, A
//MOV     R10, #10 
EJMP     0xFE75D0