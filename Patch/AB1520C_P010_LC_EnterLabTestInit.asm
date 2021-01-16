?EP?P010_f1_0xFEB824?P010 SEGMENT 'ECODE_FLASH'
PUBLIC P010_f1_0xFEB824??
RSEG ?EP?P010_f1_0xFEB824?P010	;program segment

P010_f1_0xFEB824??:
DB		0x24, 0xB8, 0xFE
DB		0
EJMP	P010_f1_patch

P010_f1_patch:
MOV      0xC2,#0x02

//LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_RX_MAX_LEN_TINY,  ((pSysMemoryConfig->OSMEM2TinyRxEntrySize - 8) - OS_OFFSET_OF(RX_BUFFER_STRU,packet_header_b0)) / 8);   //- 32 
MOV      0xC1,#0x04
MOV      0xFE,#0x04

//LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_RX_MAX_LEN_SMALL, ((pSysMemoryConfig->OSMEM2SmallRxEntrySize - 8) - OS_OFFSET_OF(RX_BUFFER_STRU,packet_header_b0)) / 8);  //- 384 
MOV      0xC1,#0x05
MOV      0xFE,#0x30

//LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_RX_MAX_LEN_LARGE, ((pSysMemoryConfig->OSMEM2LargeRxEntrySize - 8) - OS_OFFSET_OF(RX_BUFFER_STRU,packet_header_b0)) / 8);  //- 1024 
MOV      0xC1,#0x1D
MOV      0xFE,#0x80

//LC_RXBUF_INIT(); 

MOV     A, #0x00
MOV	    DPTR, #0x8CDD
MOVX    @DPTR, A
MOV	    DPTR, #0x8CDE
MOVX    @DPTR, A
MOV	    DPTR, #0x8CDF
MOVX    @DPTR, A
MOV	    DPTR, #0x8CE0
MOVX    @DPTR, A
MOV	    DPTR, #0x8CE1
MOVX    @DPTR, A
MOV	    DPTR, #0x8CE2
MOVX    @DPTR, A
MOV     0xC1,#0x33
MOV     0xC2,#0x02

ECALL    0xFE0520 //LC_SetupRxBuffer

MOV      0xC1,#0x07
EJMP     0xFEB82A