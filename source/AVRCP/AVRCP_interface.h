//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
			
#ifndef _AVRCP_INTERFACE_H_
#define _AVRCP_INTERFACE_H_

#include "os.h"

#define AVRCP_PID 0x110e
enum
{
  //Command Type
  AVC_CMD_CONTROL 		  		= 0x00, // Used to instruct a target to perform an operation.
  AVC_CMD_STATUS 				= 0x01, // Used to check a device!|s current status.
  AVC_CMD_SPECIFIC_INQUIRY  	= 0x02, // Used to check whether a target supports a particular CONTROL command. All operands are included.
  AVC_CMD_NOTIFY 				= 0x03, // Used for receiving notification of a change in a device!|s state.
  AVC_CMD_GENERAL_INQUIRY   	= 0x04, // Used to check whether a target supports a particular CONTROL command. Operands are not included.

  //Response Code
  AVC_RES_NOT_IMPLEMENTED		= 0x08, // The target does not implement the command specified by the opcode and operand marked
  										// in the ck column, or doesn!|t implement the specified subunit.
  AVC_RES_ACCEPTED 		  		= 0x09, // The target executed or is executing the command.
  AVC_RES_REJECTED 	      		= 0x0A, // The target implements the command specified by the opcode and operands marked in the ck
										// column, but cannot respond because the current state of the target doesn!|t allow it.
										// Note that some commands may return a REJECTED response as a result of invalid operands not marked in the ck column.
  AVC_RES_IN_TRANSITION     	= 0x0B, // The target implements the STATUS command, but it is in a state of transition. The STATUS command may be retried at a future time.
  AVC_RES_STABLE				= 0x0C, // For SPECIFIC INQUIRY or GENERAL INQUIRY commands, the target implements the command. 
  										//For STATUS commands, the target returns STABLE and includes the status results.
  AVC_RES_CHANGED 		  		= 0x0D, // The response frame contains a notification that the target device!|s state has changed.
  AVC_RES_INTERIM 		  		= 0x0F, // For CONTROL commands, the target has accepted the request but cannot return information within 100 milliseconds. 
  										// For NOTIFY commands, the target has accepted the command and will notify the controller of a change of target state at a future time.
};

///////////////////////////////////////////////////////
///////////// Command and Response Table////////////////
///////////////////////////////////////////////////////
//Com\Res |NotImplement | Accept | Reject | In transition | ImplementStable| Change | Interim
//------------------------------------------------------------------------------------------------------------
//Control   |    O               |    O     |    O    |        X         |          X       |      X    |   O
//------------------------------------------------------------------------------------------------------------
//Status    |    O               |    X     |    O    |        O         |    O (Stable)    |      X    |   X 
//------------------------------------------------------------------------------------------------------------
//SpecificI |    O               |    X     |    X    |        X         |    O (Implement) |      X    |   X 
//------------------------------------------------------------------------------------------------------------
//Notify    |    O               |    X     |    O    |        X         |          X       |      O    |   O 
//------------------------------------------------------------------------------------------------------------
//GeneralI  |    O               |    X     |    X    |        X         |    O (Implement) |      X    |   X 
//------------------------------------------------------------------------------------------------------------


enum
{
  SUBUNIT_MONITOR			     = 0x00,
  SUBUNIT_AUDIO				     = 0x01,
  SUBUNIT_PRINTER			     = 0x02,
  SUBUNIT_DISC					 = 0x03,
  SUBUNIT_TAPE_RECORDER_PLAYER   = 0x04,
  SUBUNIT_TUNER				     = 0x05,
  SUBUNIT_CA				     = 0x06,
  SUBUNIT_CAMERA				 = 0x07,
  SUBUNIT_PANEL					 = 0x09,
  SUBUNIT_BULLETIN_BOARD		 = 0x0A,
  SUBUNIT_CAMERA_STORAGE		 = 0x0B,
  SUBUNIT_EXTENDED_TO_NEXT_BYTE  = 0x1E,
  SUBUNIT_UNIT				     = 0x1F,
};

enum
{
  AVC_OPCODE_VENDOR_DEPENDENT = 0x00,
  AVC_OPCODE_UNIT_INFO    	  = 0x30,
  AVC_OPCODE_SUBUNIT_INFO 	  = 0x31,
  AVC_OPCODE_PASS_THROUGH 	  = 0x7C,
};

enum
{
  AVC_ERROR_CODE_INVALID_COMMAND = 0x00,           
  AVC_ERROR_CODE_INVALID_PARAMETER,         
  AVC_ERROR_CODE_PARAMETER_ERROR,  
  AVC_ERROR_CODE_INTERNAL_ERROR, 	    		
  AVC_ERROR_CODE_NO_ERROR, 	    		
  AVC_ERROR_CODE_UIDS_CHANGED,
  AVC_ERROR_CODE_RESERVED,
  AVC_ERROR_CODE_INVALID_DIRECTION,
  AVC_ERROR_CODE_NOT_A_DIRECTORY,
  AVC_ERROR_CODE_DOES_NOT_EXIST,
  AVC_ERROR_CODE_INVALID_SCOPE,
  AVC_ERROR_CODE_RANGE_OUT_OF_BOUNDS,
  AVC_ERROR_CODE_FOLDER_NOT_PLAYABLE,
  AVC_ERROR_CODE_MEDIA_IN_USE,
  AVC_ERROR_CODE_NOW_PLAYING_LIST_FULL,
  AVC_ERROR_CODE_SEARCH_NOT_SUPPORTED,
  AVC_ERROR_CODE_SEARCH_IN_PROGRESS,
  AVC_ERROR_CODE_INVALID_PLAYER_ID,
  AVC_ERROR_CODE_PLAYER_NOT_BROWSABLE,
  AVC_ERROR_CODE_PLAYER_NOT_ADDRESSED,
  AVC_ERROR_CODE_NO_VALID_SEARCH_RESULTS,
  AVC_ERROR_CODE_NO_AVAILABLE_PLAYERS,
  AVC_ERROR_CODE_ADDRESSED_PLAYER_CHANGED,
};

enum
{
  	PUSHED,
  	RELEASED
};

enum
{
	AVRCP_PKT_TYPE_SINGLE = 0,
	AVRCP_PKT_TYPE_START,
	AVRCP_PKT_TYPE_CONTINUE,
	AVRCP_PKT_TYPE_END
};

#define AVRCP_PDU_ID_GET_CAPABILITIES			0x10
#define AVRCP_PDU_ID_GET_PALYER_APPLICATION		0x13
#define AVRCP_PDU_ID_SET_PALYER_APPLICATION     0x14   //dean
#define AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES		0x20
#define AVRCP_PDU_ID_GET_PLAY_STATUS			0x30
#define AVRCP_PDU_ID_REGISTER_NOTIFICATION		0x31
#define AVRCP_PDU_ID_REQUEST_CONTINUE_RESPONSE	0x40
#define AVRCP_PDU_ID_ABORT_CONTINUE_RESPONSE	0x41
#define AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME		0x50
#define AVRCP_PDU_ID_SET_ADDRESSED_PLAYER		0x60
#define AVRCP_PDU_ID_AVCTP_TS_FRA_BV_03_C		0xfc
#define AVRCP_PDU_ID_AVCTP_TS_FRA_BV_04_C		0xfd

// 0~127 --> 0%~100%
#define AVRCP_ABSOLUTE_VOLUME_MAX				127
#define AVRCP_ABSOLUTE_VOLUME_FULL_RANGE		128


enum
{
	AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED				= 0x01,
	AVRCP_EVENT_ID_TRACK_CHANGED						= 0x02,
	AVRCP_EVENT_ID_TRACK_REACHED_END					= 0x03,
	AVRCP_EVENT_ID_TRACK_REACHED_START					= 0x04,
	AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED					= 0x05,
	AVRCP_EVENT_ID_BATT_STATUS_CHANGED					= 0x06,
	AVRCP_EVENT_ID_SYSTEM_STATUS_CHANGED				= 0x07,
	AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED	= 0x08,
	AVRCP_EVENT_ID_NOW_PLAYING_CONTENT_CHANGED			= 0x09,
	AVRCP_EVENT_ID_AVAILABLE_PLAYERS_CHANGED			= 0x0a,
	AVRCP_EVENT_ID_ADDRESSED_PLAYER_CHANGED				= 0x0b,
	AVRCP_EVENT_ID_UIDS_CHANGED							= 0x0c,
	AVRCP_EVENT_ID_VOLUME_CHANGED 						= 0x0d,
};

enum
{
	AVRCP_MEDIA_TITLE			= 0x01,
	AVRCP_MEDIA_ARTIST_NAME		= 0x02,
	AVRCP_MEDIA_ALBUM_NAME		= 0x03,
	AVRCP_MEDIA_TRACK_NUMBER	= 0x04,
	AVRCP_MEDIA_TOTAL_TRACKS	= 0x05,
	AVRCP_MEDIA_GENRE			= 0x06,
	AVRCP_MEDIA_PLAYING_TIME	= 0x07,
};

enum
{
	AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY	= 0x01,
	AVRCP_VENDOR_UNIQ_MUTE_CONTROL,
	#ifdef LIGHTING_ENABLE
	AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL,
	#endif
	AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT,
	AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT,
	AVRCP_VENDOR_UNIQ_AIRAPP_GET_INFO_FROM_FOLLOWER,
	AVRCP_VENDOR_UNIQ_AIRAPP_REPORT_INFO_TO_RELAYER,
	AVRCP_VENDOR_UNIQ_AIRAPP_GET_CHGBAT_STATUS_FROM_FOLLOWER,
	AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER,
	AVRCP_VENDOR_UNIQ_SYNC_AND_SWITCH_AUDIO_TRANSPARENCY,
	AVRCP_VENDOR_UNIQ_SYNC_MIC_VOL_AND_UP,
	AVRCP_VENDOR_UNIQ_SYNC_MIC_VOL_AND_DN,
	AVRCP_VENDOR_UNIQ_SET_AT_VOL_CMD,
	AVRCP_VENDOR_UNIQ_SET_AT_VOL_RSP,
	AVRCP_VENDOR_UNIQ_SYNC_SCO_STATUS_FOR_AT,
	AVRCP_VENDOR_UNIQ_GET_AT_GAIN_LV,
	AVRCP_VENDOR_UNIQ_REPORT_AT_GAIN_LV,
	AVRCP_VENDOR_UNIQ_GET_AT_STATUS,
	AVRCP_VENDOR_UNIQ_REPORT_AT_STATUS,
};

enum
{
	AVRCP_VENDOR_UNIQ_PARA_MODE_IDLE				= 0x00,
	AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN				= 0x01,
	AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP				= 0x02,
	AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER				= 0x03,
	AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER			= 0x04,
	AVRCP_VENDOR_UNIQ_PARA_MODE_VP_OVER				= 0x05,
	AVRCP_VENDOR_UNIQ_PARA_MODE_DISCOVERABLE		= 0x06,

};

enum
{
	AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF = 0,
	AVRCP_VENDOR_UNIQ_PARA_MUTE_ON,
	AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF_IND,
	AVRCP_VENDOR_UNIQ_PARA_MUTE_ON_IND,
};

enum
{
	AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON,
	AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF,
	AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE,
	AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE,
};

typedef struct
{
	U8 operandID;
	U8 operandLength;
}AVRCP_PASSTHROUGH_DBG;

typedef struct
{
	U8 operandID:7;	
	U8 buttonReleased:1;
	U8 operandLength;
}AVRCP_PASSTHROUGH_PDU;

typedef struct
{
	U8 operandID:7;	
	U8 buttonReleased:1;
	U8 operandLength;
	U8 companyID[3];
	union {
		U16 bluetooth_operation_id;
		struct {
			U8 operationId;
			U8 operationParam;
			#ifdef LIGHTING_ENABLE
			U32 syncClk;
			U8 ledPara[3];
			#endif
		} airoha;
	} vendor_unique;
}AVRCP_VENDOR_UNIQUE_PDU;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
}AVRCP_VENDOR_DEPENDENT_PDU;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 errorCode;
}AVRCP_REJECT_RSP;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 CapabilityId;
}AVRCP_GET_CAPABILITIES_CMD;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 CapabilityId;
	U8 CapabilityCount;
}AVRCP_GET_CAPABILITIES_RSP;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 UID[8];
	U8 NumAttributes;
	U32 AttributeID[1];
}AVRCP_GET_ELEMENT_ATTRIBUTES_CMD;

#define AVRCP_ELEMENT_ATTRIBUTE_HEADER_SIZE		8

typedef struct
{
	U32 AttributeID;
	U16 CharacterSetID;
	U16 AttributeValueLength;
	U8 AttributeValue[1];
}AVRCP_ELEMENT_ATTRIBUTE;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 NumAttributes;
	AVRCP_ELEMENT_ATTRIBUTE	Attribute[1];
}AVRCP_GET_ELEMENT_ATTRIBUTES_RSP;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
}AVRCP_GET_PLAY_STATUS_CMD;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U32 SongLength;		//milliseconds
	U32 SongPosition;	//milliseconds
	U8 PlayStatus;
}AVRCP_GET_PLAY_STATUS_RSP;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 absoluteVolume:7;
	U8 RFD:1;
}AVRCP_ABSOLUTE_VOLUME_PDU;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 eventID;
	union {
		U8 PlayStatus;		//playback status changed
		U8 UID[8];			//track changed
		U32 PlaybackPos;	//playback pos changed
	} eventParams;
}AVRCP_NOTIFY_PDU;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 eventID;
	U8 unusedParam[4];
}AVRCP_REGISTER_PDU;

typedef struct
{
	U8 companyID[3];
	U8 pduID;
	U8 packetType:2;
	U8 reserved:6;
	U16 ParameterLength;
	U8 numOfSettings;
	U8 settingAttributeID;
	U8 settingValue;
}AVRCP_GET_CURRENT_PLAYER_APP_VALUE_RESP;

typedef union
{
	AVRCP_PASSTHROUGH_DBG passThroughDbg;
	AVRCP_PASSTHROUGH_PDU passThroughPdu;
	AVRCP_VENDOR_UNIQUE_PDU vendorUniquePdu;
	AVRCP_VENDOR_DEPENDENT_PDU vendorDependentPdu;
	AVRCP_REJECT_RSP rejectRsp;
	AVRCP_NOTIFY_PDU notifyPdu;
	AVRCP_REGISTER_PDU registerPdu;
	AVRCP_ABSOLUTE_VOLUME_PDU absoluteVolumePdu;
	AVRCP_GET_CURRENT_PLAYER_APP_VALUE_RESP playerSettingsValueResp;
	AVRCP_GET_CAPABILITIES_CMD getCapabilitiesCmd;
	AVRCP_GET_CAPABILITIES_RSP getCapabilitiesRsp;
	AVRCP_GET_ELEMENT_ATTRIBUTES_CMD getElementAttributesCmd;
	AVRCP_GET_ELEMENT_ATTRIBUTES_RSP getElementAttributesRsp;
	AVRCP_GET_PLAY_STATUS_CMD getPlayStatusCmd;
	AVRCP_GET_PLAY_STATUS_RSP getPlayStatusRsp;
}AVRCP_MSG_BODY_TYPE;

typedef struct
{
	U8 dataOffset;
	U8 reserveByte[5];
	U16 l2capLength;
	U16 l2capCID;
	U8 reserved1:4;
	U8 transactionLabel:4;
	U16 PID;
	U8 ctypeOrResp:4;
	U8 reserved2:4;
	U8 subUnitID:3;
	U8 subUnitType:5;
	U8 opCode;
	AVRCP_MSG_BODY_TYPE msgBodyPtr;
}AVRCP_CMD_TYPE;

enum
{
	AVRCP_PLAYER_SETTINGS_ILLEGAL 	= 0x00,
	AVRCP_PEQ_ON_OFF_STATUS 		= 0x01,
	AVRCP_REPEAT_MODE_STATUS 		= 0x02,
	AVRCP_SHUFFLE_ON_OFF_STATUS 	= 0x03, 
	AVRCP_SCAN_ON_OFF_STATUS 		= 0x04, 
};  // AVRCP Appendix F

enum
{
	AVRCP_REPEAT_OFF			= 0x01,
	AVRCP_REPEAT_SINGLE_TRACK	= 0x02,
	AVRCP_REPEAT_ALL_TRACK		= 0x03,
	//AVRCP_REPEAT_GROUP			= 0x04, // ignore coz this may not be supported in mobile[iphone]
	AVRCP_REPEAT_MODE_NO		= AVRCP_REPEAT_ALL_TRACK,
	
	AVRCP_REPEAT_CMD_REJECT		= 0xEE,
};

enum
{
	AVRCP_SHUFFLE_OFF			= 0x01,
	AVRCP_SHUFFLE_ALL_TRACK		= 0x02,
	//AVRCP_SHUFFLE_GROUP			= 0x03,// ignore coz this may not be supported in mobile[iphone]
	AVRCP_SHUFFLE_MODE_NO		= AVRCP_SHUFFLE_ALL_TRACK,
	
	AVRCP_SHUFFLE_CMD_REJECT		= 0xEE,
};

enum
{
	AVC_OPERATION_POWER			= 0x40,
	AVC_OPERATION_VOL_UP		= 0x41,
	AVC_OPERATION_VOL_DOWN		= 0x42,
	AVC_OPERATION_MUTE			= 0x43,
	AVC_OPERATION_PLAY			= 0x44,
	AVC_OPERATION_STOP			= 0x45,
	AVC_OPERATION_PAUSE			= 0x46,
	AVC_OPERATION_RECORD		= 0x47,
	AVC_OPERATION_REWIND		= 0x48,
	AVC_OPERATION_FAST_FORWARD	= 0x49,
	AVC_OPERATION_EJECT			= 0x4a,
	AVC_OPERATION_FORWARD		= 0x4b,
	AVC_OPERATION_BACKWARD		= 0x4c,
	AVC_OPERATION_INTERNAL_REWIND_RELEASE		= 0x4d,
	AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE	= 0x4e,
	AVC_OPERATION_VENDOR_UNIQUE	= 0x7e,
#ifdef PEQ_ENABLE
	AVC_OPERATION_PEQOFF				= 0x01,
	AVC_OPERATION_PEQON					= 0x02,
#endif
	AVC_OPERATION_KEY_PLAY				= 0x03,
	AVC_OPERATION_KEY_PAUSE				= 0x04,
	AVC_OPERATION_GET_REPEAT_MODE		= 0x10,
	AVC_OPERATION_GET_SHUFFLE_MODE		= 0x11,
	AVC_OPERATION_GET_PLAY_STATUS		= 0x12,
	AVC_OPERATION_SET_REPEAT_MODE		= 0x13,
	AVC_OPERATION_SET_SHUFFLE_MODE		= 0x14,
};

#define AVRCP_REPORT_VOLUME_CHANGE						0x01
#define AVRCP_NOT_SUPPORT_PAUSE_CMD_IN_REMOTE			0x02
#define AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS		0x04
#define AVRCP_MISC_REWIND_OR_FAST_FORWARD_SENT			0x08		
#define AVRCP_MISC_FIRST_INTERIM_TRACK_CHANGED			0x10

enum
{
	AVRCP_SEND_OPERATION = 0x01,
	AVRCP_VOLUME_CHANGED,
	AVRCP_SET_NOTIFY_MASK,
	AVRCP_SET_ABSOLUTE_VOLUME,
	AVRCP_GET_ELEMENT_ATTRIBUTES,
	AVRCP_SEND_VENDOR_UNIQUE,
};

#define AVRCP_EVT_PASS_THROUGH_OFFSET	0x10

enum
{
	//PASS THROUGH command from remote (0x40~0x4c)
	AVRCP_EVT_TO_MMI_GET_KEY_POWER_CMD			= AVC_OPERATION_POWER,
	AVRCP_EVT_TO_MMI_GET_KEY_VOLUME_UP_CMD		= AVC_OPERATION_VOL_UP,
	AVRCP_EVT_TO_MMI_GET_KEY_VOLUME_DOWN_CMD	= AVC_OPERATION_VOL_DOWN,
	AVRCP_EVT_TO_MMI_GET_KEY_MUTE_CMD			= AVC_OPERATION_MUTE,
	AVRCP_EVT_TO_MMI_GET_KEY_PLAY_CMD			= AVC_OPERATION_PLAY,
	AVRCP_EVT_TO_MMI_GET_KEY_STOP_CMD			= AVC_OPERATION_STOP,
	AVRCP_EVT_TO_MMI_GET_KEY_PAUSE_CMD			= AVC_OPERATION_PAUSE,
	AVRCP_EVT_TO_MMI_GET_KEY_FORWARD_CMD		= AVC_OPERATION_FORWARD,
	AVRCP_EVT_TO_MMI_GET_KEY_BACKWARD_CMD		= AVC_OPERATION_BACKWARD,

	//PASS THROUGH response from remote (0x50~0x5c)
	AVRCP_EVT_TO_MMI_GET_KEY_POWER_RESP		= (AVRCP_EVT_PASS_THROUGH_OFFSET + AVC_OPERATION_POWER),
	AVRCP_EVT_TO_MMI_GET_KEY_MUTE_RESP		= (AVRCP_EVT_PASS_THROUGH_OFFSET + AVC_OPERATION_MUTE),
	AVRCP_EVT_TO_MMI_GET_KEY_PLAY_RESP		= (AVRCP_EVT_PASS_THROUGH_OFFSET + AVC_OPERATION_PLAY),
	AVRCP_EVT_TO_MMI_GET_KEY_STOP_RESP		= (AVRCP_EVT_PASS_THROUGH_OFFSET + AVC_OPERATION_STOP),
	AVRCP_EVT_TO_MMI_GET_KEY_PAUSE_RESP		= (AVRCP_EVT_PASS_THROUGH_OFFSET + AVC_OPERATION_PAUSE),
	
	
	AVRCP_EVT_TO_MMI_GET_VENDOR_UNIQ_MUTE_CTL_ON_RESP = 0x80,
	AVRCP_EVT_TO_MMI_GET_VENDOR_UNIQ_MUTE_CTL_OFF_RESP,

	//VENDOR DEPENDENT AV/C event
	AVRCP_EVT_TO_MMI_GET_PLAY_STATUS = 0x84,
	AVRCP_EVT_TO_MMI_GET_STOP_REJECTED,
	AVRCP_EVT_TO_MMI_PLAY_STATUS_FOR_KEY_PLAY_PAUSE,
	AVRCP_EVT_TO_MMI_REMOTE_NOTIFICATION_CHANGED,
	AVRCP_EVT_TO_MMI_SET_ABSOLUTE_VOLUME,
	AVRCP_EVT_TO_MMI_GET_ELEMENT_ATTRIBUTES,
	AVRCP_EVT_TO_MMI_VENDOR_UNIQUE,
	AVRCP_EVT_TO_MMI_GET_APPLICATION_SETTINGS,
};

enum
{
	AVRCP_GET_CAPABILITIES_COMPANY_ID		= 0x02,
	AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED	= 0x03
};

enum
{
	AVRCP_PLAYBACK_STATUS_STOPPED,
	AVRCP_PLAYBACK_STATUS_PLAYING,
	AVRCP_PLAYBACK_STATUS_PAUSED,
	AVRCP_PLAYBACK_STATUS_FWD_SEEK,
	AVRCP_PLAYBACK_STATUS_REV_SEEK,
	AVRCP_PLAYBACK_STATUS_REJECT = 0x80,
	AVRCP_PLAYBACK_STATUS_NONE  = 0xF0,
	AVRCP_PLAYBACK_STATUS_ERROR = 0xFF,
	
};

typedef union
{
	U8 operationId;
	struct {
		U8 operationId;
		U8 operationParam;
	} VendorUnique;
	struct {
		U8 attributeID;
		U8 attributeValue;
	} PlayAppSettings;
}MMI_TO_AVRCP_CMD_PARAM;


// Functions
PUBLIC void AVRCP_Init(void);
PUBLIC U8 AVRCP_GetState(U8 linkIndex);
PUBLIC void AVRCP_MMICommandHandler(U8 linkIndex, U8 cmdCode, MMI_TO_AVRCP_CMD_PARAM XDATA_PTR paramPtr);


#endif

