?EP?P001_f1_0xFE79F8?P001 SEGMENT 'ECODE_FLASH'
PUBLIC P001_f1_0xFE79F8??
RSEG ?EP?P001_f1_0xFE79F8?P001	;program segment

P001_f1_0xFE79F8??:
DB		0xF8, 0x79, 0xFE
DB		0
EJMP	P001_f1_patch

P001_f1_patch:
CLR      (0xA8.7)
CLR      (0xA8.7)
CLR      (0xA8.7)
CLR      (0xA8.7)
NOP      
NOP      
NOP      
NOP      
INC      (0x0B)
MOV      0xC1,#0x55
MOV      0xFE,#0x00
DEC      (0x0B)
MOV      A,(0x0B)
JNZ      P001_f1_0xFE7A00_label1
SETB     (0xA8.7)
;  OS_delay_us(100);  
P001_f1_0xFE7A00_label1:
MOV      WR6,#0x0064
ECALL    0xFFAF15
;  BANK_SFR_WRITE(C2,C2OFFS_RW_WD_ENABLE,0); 
CLR      (0xA8.7)
CLR      (0xA8.7)
CLR      (0xA8.7)
CLR      (0xA8.7)
NOP      
NOP      
NOP      
NOP      
INC      (0x0B)
MOV      0xC1,#0xC3
MOV      0xC2,#0x00
DEC      (0x0B)
MOV      A,(0x0B)
JNZ      P001_f1_0xFE7A00_label2
SETB     (0xA8.7)
;  OS_SoftwareReset(); 
P001_f1_0xFE7A00_label2:
ECALL    0xFFB8A5

EJMP     0xFE7A1C
