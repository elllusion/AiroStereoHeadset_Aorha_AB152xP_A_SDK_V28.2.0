?XD?P004_f1_0xFF7A74?1 SEGMENT 'EDATA_PATCH_PARA'
PUBLIC gHopChannel
PUBLIC gHopEnb
RSEG ?XD?P004_f1_0xFF7A74?1

gHopChannel:		DS	1
gHopEnb:		    DS  1

?EP?P004_f1_0xFF7A74?P004 SEGMENT 'ECODE_FLASH'
PUBLIC P004_f1_0xFF7A74??
RSEG ?EP?P004_f1_0xFF7A74?P004	;program segment

P004_f1_0xFF7A74??:
DB		0x74, 0x7A, 0xFF
DB		0
EJMP	P004_f1_patch

P004_f1_patch:
MOV      0xE6,A
; Save the hopping channel into internal memory gHopChannel
MOV      DPTR, #gHopChannel
MOV      R11,@WR14+0x0005
MOVX     @DPTR, A
MOV      DPTR, #gHopEnb
MOV      R11, @WR14+0x00A1
MOVX     @DPTR, A

MOV      R11,@WR14+0x0005

EJMP     0xFF7A7A