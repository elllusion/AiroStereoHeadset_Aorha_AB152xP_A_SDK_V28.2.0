?EP?P016_f1_0xFFAE40?P016 SEGMENT 'ECODE_PATCH'
PUBLIC P016_f1_0xFFAE40??
RSEG ?EP?P016_f1_0xFFAE40?P016	;program segment

P016_f1_0xFFAE40??:
DB		0x40, 0xAE, 0xFF
DB		0
EJMP	P016_f1_patch

P016_f1_patch:
;tmp = _BANK_SFR_READ(C2,C2OFFS_WO_PMU_ENABLE) | BIT_MASK8(5); 
MOV      0xC1,#0xDC
MOV      A,0xC2
ORL      A,#0x20
;_BANK_SFR_WRITE(C2,C2OFFS_WO_PMU_ENABLE,tmp); 
MOV      0xC1,#0xDC
MOV      0xC2,A

EJMP     0xFFAE49