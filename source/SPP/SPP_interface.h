
#ifndef __SPP_INTERFACE_H__
#define __SPP_INTERFACE_H__

#ifdef __SPP_C__
#define SPP_EXTERN
#else
#define SPP_EXTERN	extern
#endif

#define SPP_MAX_FRAME_SIZE		240	/* to fit in uart output header structure */

#define SPP_TO_MMI_OPCODE_CONNECTED		0x01
#define SPP_TO_MMI_OPCODE_DISCONNECTED	0x02
#define SPP_TO_MMI_OPCODE_DATA			0x03

#define SPP_CMD_FROM_MMI_DATA			0x13

#ifdef SONGPAL_Profile
#define SONGPAL_ANDROID_SPP_ENCODING_OFFSET		0x20
#define SONGPAL_IAP2_SPP_ENCODING_OFFSET		0x40
#endif
#ifdef IAP2_Profile
#define IAP2_SPP_ENCODING_OFFSET				0x60
#endif

SPP_EXTERN OSMQ XDATA OSMQ_SPP_Command_From_MMI[MAX_MULTI_POINT_NO];


#define OSMQ_SPP_Command_From_MMI_ptr	&OSMQ_SPP_Command_From_MMI


PUBLIC void SPP_Init(void);
PUBLIC void SPP_MMICommandHandler(U8 linkIndex);
PUBLIC void SPP_FlushMMICommandQueue(U8 linkIndex, U8 opCode);

#endif //__SPP_INTERFACE_H__
