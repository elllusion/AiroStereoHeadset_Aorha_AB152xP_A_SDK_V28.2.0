?XD?P019_f1_0xFEFE7C?1 SEGMENT 'EDATA_PATCH_PARA'
PUBLIC gHopChannel
PUBLIC gHopEnb
RSEG ?XD?P019_f1_0xFEFE7C?1

gHopChannel:		DS	1
gHopEnb:		    DS  1

?EP?P019_f1_0xFEFE78?P019 SEGMENT 'ECODE_PATCH'
PUBLIC P019_f1_0xFEFE78??
RSEG ?EP?P019_f1_0xFEFE78?P019	;program segment

P019_f1_0xFEFE78??:
DB		0x78, 0xFE, 0xFE
DB		0
EJMP	P019_f1_patch

P019_f1_patch:
; Save the hopping channel into internal memory gHopChannel
MOV      DPTR, #gHopChannel
MOV      R11,@WR14+0x0005
MOVX     @DPTR, A
MOV      DPTR, #gHopEnb
MOV      R11, @WR14+0x00A0
MOVX     @DPTR, A

MOV      R11,@WR14+0x0005

EJMP     0xFEFE7C