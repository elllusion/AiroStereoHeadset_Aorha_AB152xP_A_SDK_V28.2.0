//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _L2CAP_ROM_INTERFACE_H_
#define _L2CAP_ROM_INTERFACE_H_

#include "os.h"

#ifdef _L2CAP_ROM_C_
#define L2CAP_ROM_EXTERN
#else
#define L2CAP_ROM_EXTERN extern
#endif

#define SUPPORT_L2CAP_ROM_IGNORE_COPY_WHEN_RECOMBINE_HCI_DATA

typedef struct
{
	U16 			remain_length;
	U8 	XDATA_PTR 	current_ptr;
	U8 	XDATA_PTR	memory_ptr;
} L2CAP_ROM_RECOMBINE_CTL;

#ifdef AIR_UPDATE_ENABLE
L2CAP_ROM_EXTERN L2CAP_ROM_RECOMBINE_CTL XDATA gL2CAP_rom_le_recombine_ctl;
#endif

L2CAP_ROM_EXTERN OSMQ XDATA OSMQ_L2CAP_Data_To_ATT;
L2CAP_ROM_EXTERN OSMQ XDATA OSMQ_L2CAP_Data_From_ATT;

#define OSMQ_L2CAP_Data_To_ATT_ptr			&OSMQ_L2CAP_Data_To_ATT
#define OSMQ_L2CAP_Data_From_ATT_ptr		&OSMQ_L2CAP_Data_From_ATT

/////////////////////////////////////////////////////////

#ifdef AIR_UPDATE_ENABLE
//LE Global Functions

PUBLIC void L2CAP_ROM (void);
PUBLIC void L2CAP_ROM_Init (void);
#endif
/////////////////////////////////////////////////////////


#endif
