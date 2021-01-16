//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _AVRCP_H_
#define _AVRCP_H_

typedef struct
{
	MMI_PROFILE_INFO mmiProfileInfo;
	U8 nextTransactionLabel:4;
	U8 volumeNotifyTransactionLabel:4;
	U8 avrcpCapabilities;
	U8 avrcpMiscFlags;
	U8 avrcpNotifyMask;
	U8 avrcpMediaEvents;
	U8 continueResponseCount;
}AVRCP_LINK_INFO;

typedef struct
{
	AVRCP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO]; 	 
}AVRCP_CTL_TYPE;

U8 CODE AVRCP_unit_info_resp[]=
{
	AVC_RES_STABLE, 
	(SUBUNIT_UNIT << 3) | 0x07,
	AVC_OPCODE_UNIT_INFO,  //U8 opCode;
	0x07, 
	SUBUNIT_PANEL << 3, 
	0x00, 0x19, 0x58 // U8 companyID[3];
};

U8 CODE avrcp_pass_through_operation[]=
{
	AVC_CMD_CONTROL,
	(SUBUNIT_PANEL << 3) | 0,
	AVC_OPCODE_PASS_THROUGH, //U8 opCode;
	0x00,			  // operation
	0,				// operand length
};

U8 CODE avrcp_vendor_unique_operation[]=
{
	AVC_CMD_CONTROL,
	(SUBUNIT_PANEL << 3) | 0,
	AVC_OPCODE_PASS_THROUGH, //U8 opCode;
	0x7e,			  //vendor unqiue
	5,				// operand length
	0x0C, 0xFC, 0x83,	// company ID
	0x00,			// airoha operation id
	0x00,			// airoha operation param
};

#ifdef LIGHTING_ENABLE
U8 CODE avrcp_vendor_unique_operation_extend[]=
{
	AVC_CMD_CONTROL,
	(SUBUNIT_PANEL << 3) | 0,
	AVC_OPCODE_PASS_THROUGH, //U8 opCode;
	0x7e,			  //vendor unqiue
	12,				// operand length
	0x0C, 0xFC, 0x83,	// company ID
	0x00,			// airoha operation id
	0x00,			// airoha operation param(LED mode)
	0x00,0x00,0x00,	0x00,	// syncClk
	0x00,			//para1
	0x00,			//para2
	0x00,			//para3
};
#endif

U8 CODE avrcp_get_player_application_setting_value[]=
{
	AVC_CMD_CONTROL, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_GET_PALYER_APPLICATION, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x02,			  //U16 ParameterLength;
	0x01,			  //number of application
};

U8 CODE avrcp_get_capabilities[]=
{
	AVC_CMD_STATUS, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_GET_CAPABILITIES, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x01,			  //U16 ParameterLength;
	0x00			  //capability id
};

U8 CODE avrcp_get_element_attributes[]=
{
	AVC_CMD_STATUS, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x0D,			  //U16 ParameterLength;
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//U8[8] Identifier
	1,					//U8 AttributeCount
	0, 0, 0, 0			//U32 Attribute Id
};

U8 CODE avrcp_get_play_status[]=
{
	AVC_CMD_STATUS, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_GET_PLAY_STATUS, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x00,			  //U16 ParameterLength;
};
	
U8 CODE avrcp_volume_changed_notify[] = 
{
	AVC_RES_CHANGED,   // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL<<3|0x00), //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT,  //U8 opCode;
	0x00, 0x19, 0x58,  // U8 companyID[3];
	AVRCP_PDU_ID_REGISTER_NOTIFICATION,  // U8 pduID;
	0x00,  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x02,  //U16 ParameterLength;
	AVRCP_EVENT_ID_VOLUME_CHANGED,	//	U8 eventID;
	0x00, //U8 absoluteVolume:7; U8 RFD:1;
};

U8 CODE avrcp_register_notification_cmd[] = 
{
	AVC_CMD_NOTIFY,   // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL<<3|0x00), //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT,  //U8 opCode;
	0x00, 0x19, 0x58,  // U8 companyID[3];
	AVRCP_PDU_ID_REGISTER_NOTIFICATION,  // U8 pduID;
	0x00,  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x05,  //U16 ParameterLength;
	AVRCP_EVENT_ID_TRACK_CHANGED,	//	U8 eventID;
	0x00,0x00,0x00,0x00 //ignore
};

U8 CODE avrcp_continue_response[]=
{
	AVC_CMD_CONTROL, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_REQUEST_CONTINUE_RESPONSE, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x01,			  //U16 ParameterLength;
	0x00			  //pdu_id
};

U8 CODE avrcp_set_absolute_volume[]=
{
	AVC_CMD_CONTROL, // U8 ctypeOrResp:4; //low		U8 reserved:4;	  // high
	(SUBUNIT_PANEL << 3) | 0,  //  U8 subUnitID:3;		U8 subUnitType:5;
	AVC_OPCODE_VENDOR_DEPENDENT, //U8 opCode;
	0x00, 0x19, 0x58,			  //company ID
	AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME, // U8 pduID;
	0x00,			  //U8 packetType:2;  U8 reserved:6;
	0x00, 0x01,			  //U16 ParameterLength;
	0x00			  //absolute volume
};

#define AVRCP_FIRST_NOTIFY_LABEL	8
#define AVRCP_MAX_CMD_LABEL_COUNT	(16 - AVRCP_FIRST_NOTIFY_LABEL)

U8 CODE avrcp_mmi_notify_event_id[] = {
	AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED,
	AVRCP_EVENT_ID_TRACK_CHANGED,
	AVRCP_EVENT_ID_TRACK_REACHED_END,
	AVRCP_EVENT_ID_TRACK_REACHED_START,
	AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED,
	AVRCP_EVENT_ID_VOLUME_CHANGED,
	AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED,
};

enum
{
	AVRCP_LABEL_PLAYBACK_STATUS_CHANGED = AVRCP_FIRST_NOTIFY_LABEL,
	AVRCP_LABEL_TRACK_CHANGED,
	AVRCP_LABEL_TRACK_REACHED_END,
	AVRCP_LABEL_TRACK_REACHED_START,
	AVRCP_LABEL_PLAYBACK_POS_CHANGED,
	AVRCP_LABEL_VOLUME_CHANGED,
	AVRCP_LABEL_PLAYER_APPLICATION_SETTING_CHANGED,
};

#endif