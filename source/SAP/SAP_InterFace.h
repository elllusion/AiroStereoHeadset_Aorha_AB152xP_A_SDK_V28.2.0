//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _SAP_INTERFACE_H_
#define _SAP_INTERFACE_H_ 


#ifdef _SAP_C_
	#define SAP_EXTERN
#else
	#define SAP_EXTERN extern	
#endif

	#define SAP_MAX_FRAME_SIZE		255
	
	#define SAP_MAX_MSG_SIZE 0x00FF // scalarable

	#define SAP_COMMAND_OFFSET 3
	#define SAP_EVT_OFFSET 5
	#define SAP_CMD_OFFSET 0xA0 // see MMI_eventopcode.h #define MMI_SAP_CMDTYPE_OFFSET 0xA0
	#define SAP_MSG_RESERVED_NO 2

	//--------------Message format--------
	#define SAP_OFFSET 13
	#define SAP_TOTAL_DATA_LENGTH_OFFSET 2
	#define SAP_MSG_START SAP_OFFSET+SAP_TOTAL_DATA_LENGTH_OFFSET
	#define SAP_MESSAGE_ID_OFFSET 0
	#define SAP_PARAMETER_NUMBER_OFFSET 1
	#define SAP_HEADER_RESERVED_OFFSET 2
	#define SAP_PAYLOAD_OFFSET 4

	#define SAP_PARAMETER_ID_OFFSET 0
	#define SAP_PARAMETER_LENGTH_OFFSET 2
	#define SAP_PARAMETER_VALUE_OFFSET 4
	//-----------------------------------

	#define SAP_EVENT_GET_LENGTH(ptr)			((U16)(*(ptr + *ptr)) + ((U16)(*(ptr + *ptr + 1)) << 8))//temp
	#define SAP_EVENT_GET_PTR(ptr)				(ptr + *ptr + 2)
	#define SAP_GET_EVT_PARA_NUM_PTR(ptr)   	(ptr+1)
	#define SAP_GET_EVT_PARA_ID_PTR(ptr)		(ptr+4)
	#define SAP_GET_EVT_PARA_STATUS_PTR(ptr)	(ptr+8)

	enum
	{ 
		SAP_CMD_FROM_MMI_CONNECT_ESTABLISH = (0x00 + SAP_CMD_OFFSET),
		SAP_CMD_FROM_MMI_DISCONNECT_REQUEST,
		SAP_CMD_FROM_MMI_POWER_SIM_ON_REQUEST,
		SAP_CMD_FROM_MMI_TRANSFER_APDU_REQ,	
		SAP_CMD_FROM_MMI_TRANSFER_ATR_REQ,	
		SAP_CMD_FROM_MMI_POWER_SIM_OFF_REQ,
		SAP_CMD_FROM_MMI_POWER_SIM_ON_REQ,					
		SAP_CMD_FROM_MMI_RESET_SIM_REQ,					
		SAP_CMD_FROM_MMI_TRANSFER_CARD_READER_STATUS_REQ,	
		SAP_CMD_FROM_MMI_SET_TRANSPORT_PROTOCOL_REQ, 	
		SAP_EVT_TO_MMI_ESTABLISH_SLC_COMPLETE,	
		SAP_EVT_REQ_FAIL,
	};

	enum
	{
		SAP_NO_ERROR = 0x01,
		SAP_MSG_FORMAT_WRONG,
		SAP_CONNECTION_FAIL,
	};


	PUBLIC void SAP_Init (void);


SAP_EXTERN OSMQ XDATA OSMQ_SAP_Event_To_MMI[MAX_MULTI_POINT_NO];
SAP_EXTERN OSMQ XDATA OSMQ_SAP_Command_From_MMI[MAX_MULTI_POINT_NO];

#define OSMQ_SAP_Event_To_MMI_ptr		&OSMQ_SAP_Event_To_MMI
#define OSMQ_SAP_Command_From_MMI_ptr	&OSMQ_SAP_Command_From_MMI

#endif
