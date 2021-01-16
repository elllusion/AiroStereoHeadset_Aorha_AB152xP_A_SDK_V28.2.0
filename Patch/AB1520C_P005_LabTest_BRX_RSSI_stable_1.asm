?XD?P005_f1_0xFEB5B8?1 SEGMENT 'EDATA_PATCH_PARA'
PUBLIC gAgcgain
RSEG ?XD?P005_f1_0xFEB5B8?1

gAgcgain:		DS	1
gAgcflag:	    DS	1
gAgcval:	    DS	1
	
?EP?P005_f1_0xFEB5B8?P005 SEGMENT 'ECODE_FLASH'
PUBLIC P005_f1_0xFEB5B8??
RSEG ?EP?P005_f1_0xFEB5B8?P005	;program segment

P005_f1_0xFEB5B8??:
DB		0xB8, 0xB5, 0xFE
DB		0
EJMP	P005_f1_patch


P005_f1_patch:
; LC_SW_STATE_CTL_LABTEST.brx_rssi = LC_SFR_READ_BY_E4(E4OFFS_RO_RSSI)
MOV	     0xC1, #0xCF
MOV     A, 0xE4
;MOVX	     0xE000, R11
//JZ       P005_f1_patch_2  //AGC gain ==0
//CJNE     A,#0x1c, P005_f1_patch_2
MOV      DPTR, #gAgcgain
//MOV     0x93, #0
MOVX    @DPTR, A

JNZ      P005_f1_patch_01 
MOV      DPTR, #gAgcflag
ORL      A,#0x0f
MOVX    @DPTR, A
EJMP	P005_f1_patch_1

P005_f1_patch_01:
MOV	     0xC1, #0xCE
MOV     A, 0xE4
MOV	    DPTR, #0x87F1
//MOV     0x93, #0
MOVX    @DPTR, A
; 0x97e0 = LC_SFR_READ_BY_E4(E4OFFS_AGC_GAIN_SEL);



P005_f1_patch_1:

EJMP	0xFEB5C1

P005_f1_patch_2: //return
//MOV     A,#0
//MOV      DPTR, #gAgcgain
//MOVX    @DPTR, A
EJMP	0xFEB6C0  