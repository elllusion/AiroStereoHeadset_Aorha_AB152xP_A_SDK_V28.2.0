#ifndef _SECTOR_IAP2_TYPE_H_
#define _SECTOR_IAP2_TYPE_H_

#include "bt_config_profile.h"

////////////////////////////////////////////////////////////////
// IAP2 SYNC Payload
////////////////////////////////////////////////////////////////

#define IAP2_SESSION_ID_CTRL		0xCC
#define IAP2_SESSION_ID_EXTA		0xEA

#define IAP2_SESSION_TYPE_CTRL		0
#define IAP2_SESSION_TYPE_FILE		1
#define IAP2_SESSION_TYPE_EXTA		2

typedef struct
{
	U8 link_version;
	U8 max_num_out_packets;
	U16 max_packet_length;
	U16 retransmission_timeout;
	U16 cumulative_ack_timeout;
	U8 max_num_retransmissions;
	U8 max_num_cumulative_acks;
	struct {
		U8 identifier;
		U8 type;
		U8 version;
	} session[2];
} IAP2SyncPayload;

typedef struct
{
    IAP2SyncPayload iap2SyncPayload;
    
    CRC16 CRC;
} SECTOR_IAP2SyncPayload;

////////////////////////////////////////////////////////////////
// IAP2 Identification Params
////////////////////////////////////////////////////////////////

#define IAP2_PARAM_IDEN_MAX_SIZE	200

typedef U8 IAP2IdenParam[IAP2_PARAM_IDEN_MAX_SIZE];

typedef struct
{
    IAP2IdenParam iap2IdenParam;
    
    CRC16 CRC;
} SECTOR_IAP2IdenParam;

#define IAP2_PARAM_IDEN_NAME								0
#define IAP2_PARAM_IDEN_MODEL								1
#define IAP2_PARAM_IDEN_MANUFACTURER						2
#define IAP2_PARAM_IDEN_SERIAL_NUMBER						3
#define IAP2_PARAM_IDEN_FIRMWARE_VERSION					4
#define IAP2_PARAM_IDEN_HARDWARE_VERSION					5
#define IAP2_PARAM_IDEN_MESSAGE_SENT_BY_ACCESSORY			6
#define IAP2_PARAM_IDEN_MESSAGE_RECV_FROM_DEVICE			7
#define IAP2_PARAM_IDEN_POWER_PROVIDING_CAPABILITY			8
#define IAP2_PARAM_IDEN_MAX_CURRENT_FROM_DEVICE				9
#define IAP2_PARAM_IDEN_EXTERNAL_ACCESSORY_PROTOCOL			10
#define IAP2_PARAM_IDEN_APP_MATCH_TEAM_ID					11
#define IAP2_PARAM_IDEN_CURRENT_LANGUAGE					12
#define IAP2_PARAM_IDEN_SUPPORTED_LANGUAGE					13
#define IAP2_PARAM_IDEN_SERIAL_TRANSPORT_COMPONENT			14
#define IAP2_PARAM_IDEN_USB_DEVICE_TRANSPORT_COMPONENT		15
#define IAP2_PARAM_IDEN_USB_HOST_TRANSPORT_COMPONENT		16
#define IAP2_PARAM_IDEN_BLUETOOTH_TRANSPORT_COMPONENT		17
#define IAP2_PARAM_IDEN_IAP2_HID_COMPONENT					18
#define IAP2_PARAM_IDEN_VEHICLE_INFORMATION_COMPONENT		20
#define IAP2_PARAM_IDEN_VEHICLE_STATUS_COMPONENT			21
#define IAP2_PARAM_IDEN_LOCATION_INFORMATION_COMPONENT		22
#define IAP2_PARAM_IDEN_USB_HOST_HID_COMPONENT				23

#define IAP2_SUBPARAM_EA_PROTOCOL_IDENTIFIER				0
#define IAP2_SUBPARAM_EA_PROTOCOL_NAME						1
#define IAP2_SUBPARAM_EA_PROTOCOL_MATCH_ACTION				2
#define IAP2_SUBPARAM_NATIVE_TRANSPORT_COMPONENT_IDENTIFIER	3

#define IAP2_AIROHA_EA_PROTOCOL_IDENTIFIER_1				0x31	/* '1' */
#define IAP2_AIROHA_EA_PROTOCOL_IDENTIFIER_2				0x32	/* '2' */
#define IAP2_AIROHA_EA_PROTOCOL_IDENTIFIER_3				0x33	/* '3' */

#define IAP2_SUBPARAM_TRANSPORT_COMPONENT_IDENTIFIER		0
#define IAP2_SUBPARAM_TRANSPORT_COMPONENT_NAME				1
#define IAP2_SUBPARAM_TRANSPORT_SUPPORTS_IAP2_CONNECTION	2
#define IAP2_SUBPARAM_BT_TRANSPORT_MEDIA_ACCESS_ADDRESS		3

#define IAP2_AIROHA_BT_TRANSPORT_IDENTIFER					0x4254	/* "BT" */

#endif
