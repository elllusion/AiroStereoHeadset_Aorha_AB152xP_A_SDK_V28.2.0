;----------------------------------------------------------------------------
;
; Init records for EDATA (near) memory.  The record structure is as follows:
;
;    Byte 0  Byte 1  Byte 2  Byte 3  Byte 4
;   +-------+-------+-------+-------+-------+
;   |Len MSB|Len LSB|Adr MSB|Adr LSB|Content|
;   +-------+-------+-------+-------+-------+
;   |                               +Rep Len+
;   +--- Repeated for each Object ----------+
;
; The complete EDATA initilization is placed into the segment ?C_INITEDATA 
; (segment class is HCONST).  
;
;----------------------------------------------------------------------------

		NAME	?C?INITEDATA
PUBLIC		?C?INITEDATA
EXTRN HCONST (?C?INITEDATA_END)


?C_C51STARTUP?2	SEGMENT   CODE          ; Code for table driven Init 
?C_INITEDATA	SEGMENT   HCONST	; Segment with init tables (records)

		RSEG	?C_C51STARTUP?2
?C?INITEDATA:	MOV	WR0,#WORD2 ?C_INITEDATA
		MOV	WR2,#WORD0 ?C_INITEDATA
INIT_LOOP:	MOV	WR8,@DR0	; WR8 contains Length
		ANL	WR8,WR8
		JE	INIT_END
		MOV	WR4,@DR0+2	; WR4 contains Address
		INC	DR0,#4
		INC	WR8,#1
		SJMP	INIT_OBJ1
INIT_OBJ0:	MOV	R11,@DR0
		MOV	@WR4,R11
		INC	WR4,#1
		INC	DR0,#1
INIT_OBJ1:	DEC	WR8,#1
		JNE	INIT_OBJ0
		SJMP	INIT_LOOP
INIT_END:

		END

