?EP?P007_f1_0xFEB55C?P007 SEGMENT 'ECODE_FLASH'
PUBLIC P007_f1_0xFEB55C??
RSEG ?EP?P007_f1_0xFEB55C?P007	;program segment

P007_f1_0xFEB55C??:
DB		0x5C, 0xB5, 0xFE
DB		0
EJMP	P007_f1_patch



P007_f1_patch:
ECALL    0xFE04D9   //LC_GetRxBufPtr() 
ECALL    0xFFAEBA   //OSMEM_Put() 
ECALL    0xFE0520   //LC_SetupRxBuffer()

MOV	    DPTR, #gAgcgain
MOV     0x93, #0
MOVX    A, @DPTR 
JZ      P007_f1_patch_1
//LC_SW_STATE_CTL_LABTEST.union_group.group0_ctl.rxdone_stauts

MOV     DPTR, #gAgcval
MOVX    @DPTR, A 

MOV	    DPTR, #0x87D4
MOV     0x93, #0
MOV     A, #0x01
MOVX    @DPTR, A 

EJMP     0xFEB562

P007_f1_patch_1:
MOV	     0xC1, #0xC4
MOV     A, 0xFE

MOV	     0xC1, #0xC5
MOV     A, 0xFE

MOV	    DPTR, #0x87D4
MOV     0x93, #0
MOV     A, #0x00
MOVX    @DPTR, A 

EJMP     0xFEB59E