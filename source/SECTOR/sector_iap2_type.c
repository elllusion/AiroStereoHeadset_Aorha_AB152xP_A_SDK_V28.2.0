
#include <config.h>
#include <cpu.h>
#include <crc16.h>

#include "sector_iap2_type.h"

#pragma userclass (HCONST = CONFIG)


#ifdef IAP2_Profile

////////////////////////////////////////////////////////////////
// IAP2 SYNC Payload
////////////////////////////////////////////////////////////////

IAP2SyncPayload CODE gIAP2_Sync_Payload =
{
	1,		//link_version
	2,		//max_num_out_packets
	512,	//max_packet_length
	500,	//retransmission_timeout, unit: ms
	100,	//cumulative_ack_timeout, unit: ms
	3,		//max_num_retransmissions
	2,		//max_num_cumulative_acks
	{	//identifier, type, version
		{ IAP2_SESSION_ID_CTRL, IAP2_SESSION_TYPE_CTRL, 0x01 },
		{ IAP2_SESSION_ID_EXTA, IAP2_SESSION_TYPE_EXTA, 0x01 },
	}
};

static CRC16 CODE _iap2_sync_payload_crc =
{
    //- CRC
    {0,0}
};

////////////////////////////////////////////////////////////////
// IAP2 Identification Params
////////////////////////////////////////////////////////////////

U8 CODE gIAP2_Iden_Param[] =
{
	0, 20,
	0, IAP2_PARAM_IDEN_NAME,
	'A', 'i', 'r', 'o', 'h', 'a', 'A', 'c', 'c', 'e', 's', 's', 'o', 'r', 'y', 0x00,
	
	0, 12,
	0, IAP2_PARAM_IDEN_MODEL,
	'B', 'T', '_', 'i', 'A', 'P', '2', 0x00,

	0, 11,
	0, IAP2_PARAM_IDEN_MANUFACTURER,
	'A', 'i', 'r', 'o', 'h', 'a', 0x00,

	0, 13,
	0, IAP2_PARAM_IDEN_SERIAL_NUMBER,
	'2', '0', '1', '5', '0', '4', '2', '2', 0x00,

	0, 9,
	0, IAP2_PARAM_IDEN_FIRMWARE_VERSION,
	'V', '1', '.', '0', 0x00,

	0, 9,
	0, IAP2_PARAM_IDEN_HARDWARE_VERSION,
	'V', '1', '.', '0', 0x00,

	0, 12,
	0, IAP2_PARAM_IDEN_MESSAGE_SENT_BY_ACCESSORY,
	0x4E, 0x01,	//BluetoothComponentInformation
	0x4E, 0x03,	//StartBluetoothConnectionUpdates
	0x4E, 0x05,	//StopBluetoothConnectionUpdates
	0xEA, 0x02,	//RequestAppLaunch
//	0xEA, 0x03,	//StatusEnternalAccessoryProtocolSession

	0, 10,
	0, IAP2_PARAM_IDEN_MESSAGE_RECV_FROM_DEVICE,
	0x4E, 0x04,	//BluetoothConnectionUpdate
	0xEA, 0x00,	//StartEnternalAccessoryProtocolSession
	0xEA, 0x01,	//StopEnternalAccessoryProtocolSession

	0, 5,
	0, IAP2_PARAM_IDEN_POWER_PROVIDING_CAPABILITY,
	0x00,

	0, 6,
	0, IAP2_PARAM_IDEN_MAX_CURRENT_FROM_DEVICE,
	0x00, 0x00,

	0, 31,
	0, IAP2_PARAM_IDEN_EXTERNAL_ACCESSORY_PROTOCOL,
	0, 5,
	0, IAP2_SUBPARAM_EA_PROTOCOL_IDENTIFIER,
	IAP2_AIROHA_EA_PROTOCOL_IDENTIFIER_1,
	0, 17,
	0, IAP2_SUBPARAM_EA_PROTOCOL_NAME,
	'c', 'o', 'm', '.', 'a', 'p', 'p', 'l', 'e', '.', 'P', '1', 0x00,
	0, 5,
	0, IAP2_SUBPARAM_EA_PROTOCOL_MATCH_ACTION,
	1,	//do not prompt the user to find a matching app

//	0, 20,
//	0, IAP2_PARAM_IDEN_APP_MATCH_TEAM_ID,
//	'c', 'o', 'm', '.', 'a', 'i', 'r', 'o', 'h', 'a', '.', 'i', 'A', 'P', '2', 0x00,

	0, 7,
	0, IAP2_PARAM_IDEN_CURRENT_LANGUAGE,
	'e', 'n', 0x00,

	0, 7,
	0, IAP2_PARAM_IDEN_SUPPORTED_LANGUAGE,
	'e', 'n', 0x00,

	0, 38,
	0, IAP2_PARAM_IDEN_BLUETOOTH_TRANSPORT_COMPONENT,
	0, 6,
	0, IAP2_SUBPARAM_TRANSPORT_COMPONENT_IDENTIFIER,
	(U8)(IAP2_AIROHA_BT_TRANSPORT_IDENTIFER >> 8), (U8)(IAP2_AIROHA_BT_TRANSPORT_IDENTIFER),
	0, 14,
	0, IAP2_SUBPARAM_TRANSPORT_COMPONENT_NAME,
	'A', 'i', 'r', 'o', 'h', 'a', '_', 'B', 'T', 0x00,
	0, 4,
	0, IAP2_SUBPARAM_TRANSPORT_SUPPORTS_IAP2_CONNECTION,
	0, 10,
	0, IAP2_SUBPARAM_BT_TRANSPORT_MEDIA_ACCESS_ADDRESS,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	/*MUST: fake parameter for calculate actual identification length*/
	0, 0,
};

static U8 CODE _iap2_iden_param_reserved[IAP2_PARAM_IDEN_MAX_SIZE - sizeof(gIAP2_Iden_Param)];

static CRC16 CODE _iap2_iden_param_crc =
{
    //- CRC
    {0,0}
};

#endif
