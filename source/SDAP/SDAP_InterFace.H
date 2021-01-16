//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _SDAP_INTERFACE_H_
#define _SDAP_INTERFACE_H_

#include "os.h"
#include "..\MMI\MMI_interface.h"

#ifdef _SDAP_C_
#define SDAP_EXTERN
#else
#define SDAP_EXTERN extern
#endif

//////////////////////////////////////////////////////////////////
//MMI Message Interface
//////////////////////////////////////////////////////////////////
#define SDAP_EVENT_TO_MMI_OFFSET	3

typedef struct
{
	U8 linkIndex;
 	U16 payloadLength;
	U8 payload[1];
} SdapSearchCompleteEvent;

typedef union
{
	SdapSearchCompleteEvent sssCompleteEvent;
} SdapEventBody;

typedef struct
{
	U8 offset;		
	U8 reserved[SDAP_EVENT_TO_MMI_OFFSET - 1];
	U8 opCode;
	SdapEventBody eventBody;
} SdapEvent;

enum
{
	SDAP_EVENT_TO_MMI_CLIENT_CONNECTION_COMPLETE,
	SDAP_EVENT_TO_MMI_DISCONNECTION_COMPLETE,
	SDAP_EVENT_TO_MMI_NO,
};

//////////////////////////////////////////////////////////////////
//SDP Service and Attribute constant value
//////////////////////////////////////////////////////////////////

//UUID for Service Class
#define SERVICE_UUID_SERVICE_DISCOVERY_SVR	0x1000
#define SERVICE_UUID_BROWSE_GROUP_DESCRIP	0x1001
#define SERVICE_UUID_PUBLIC_BROWSE_ROOT		0x1002
#define SERVICE_UUID_SERIAL_PORT			0x1101
#define SERVICE_UUID_OBEX_PUSH				0x1105
#define SERVICE_UUID_HEADSET				0x1108
#define SERVICE_UUID_AUDIO_SOURCE			0x110A
#define SERVICE_UUID_AUDIO_SINK				0x110B
#define SERVICE_UUID_REMOTE_CONTROL_TRG		0x110C
#define SERVICE_UUID_REMOTE_CONTROL			0x110E
#define SERVICE_UUID_REMOTE_CONTROL_CTL		0x110F
#define SERVICE_UUID_HEADSET_AG				0x1112
#define SERVICE_UUID_HANDSFREE				0x111E
#define SERVICE_UUID_HANDSFREE_AG			0x111F
#ifdef HID_Profile
#define SERVICE_UUID_HUMAN_INTERFACE_DEVICE	0x1124
#endif
#define SERVICE_UUID_SIM_ACCESS				0x112D
#define SERVICE_UUID_PB_ACCESS_CLIENT		0x112E	//Phonebook Access - Client
#define SERVICE_UUID_PB_ACCESS_SERVER		0x112F	//Phonebook Access - Server
#define SERVICE_UUID_HEADSET_HS				0x1131	
#define SERVICE_UUID_MESSAGE_ACCESS			0x1132	//Message Access Service
#define SERVICE_UUID_MESSAGE_NOTIFICATION	0x1133	//Message Notification Service
#define SERVICE_UUID_MULTI_PROFILE_SPEC		0x113B	//Multi-Profile Specification
#define SERVICE_UUID_PNP_INFORMATION		0x1200
#define SERVICE_UUID_GENERIC_AUDIO			0x1203

//UUID for Profile
#define PROFILE_UUID_SPP					0x1101
#define PROFILE_UUID_OPP					0x1105	//Object Push Profile
#define PROFILE_UUID_FTP					0x1106
#define PROFILE_UUID_HSP					0x1108
#define PROFILE_UUID_A2DP					0x110D
#define PROFILE_UUID_AVRCP					0x110E
#define PROFILE_UUID_HFP					0x111E
#ifdef HID_Profile
#define PROFILE_UUID_HID					0x1124
#endif
#define PROFILE_UUID_SAP					0x112D	//SIM Access Profile
#define PROFILE_UUID_PBAP					0x1130	//Phone Book Access Profile
#define PROFILE_UUID_MAP					0x1134	//Message Access Profile
#define PROFILE_UUID_MPS					0x113A	//Multi-Profile Specification
#define PROFILE_UUID_PNP					0x1200	//Device Identification Profile

//UUID for Protocol
#define PROTOCOL_UUID_RFCOMM				0x0003
#define PROTOCOL_UUID_OBEX					0x0008
#ifdef HID_Profile
#define PROTOCOL_UUID_HID					0x0011
#endif
#define PROTOCOL_UUID_AVCTP					0x0017
#define PROTOCOL_UUID_AVDTP					0x0019
#define PROTOCOL_UUID_L2CAP					0x0100

//Attribute ID
#define ATTRID_ServiceRecordHandle			0x0000
#define ATTRID_ServiceClassIdList			0x0001
#define ATTRID_ServiceId					0x0003
#define ATTRID_ProtocolDescriptorList		0x0004
#define ATTRID_BrowseGroupList				0x0005
#define ATTRID_LangBaseAttributeIdList		0x0006
#define ATTRID_ProfileDescriptorList		0x0009
#ifdef HID_Profile
#define ATTRID_AddProtocolDescriptorLists   0x000D
#endif
#define ATTRID_ServiceName					0x0100
#ifdef HID_Profile
#define ATTRID_ServiceDescription           0x0101
#define ATTRID_ProviderName					0x0102
#endif
#define ATTRID_GroupId						0x0200
#define ATTRID_RemoteAudioVolControl		0x0302
#define ATTRID_SupportedFormatsList			0x0303
#define ATTRID_SupportedFeatures			0x0311
#define ATTRID_SupportedRepositories		0x0314
//attributes for MPS
#define ATTRID_SupportedScenarios_MPSD		0x0200
#define ATTRID_SupportedScenarios_MPMD		0x0201
#define ATTRID_SupportedProfilesProtocols	0x0202
//attributes for PNP Information
#define ATTRID_SpecifcationID				0x0200
#define ATTRID_VendorID						0x0201
#define ATTRID_ProductID					0x0202
#define ATTRID_ProductVersion				0x0203
#define ATTRID_PrimaryRecord				0x0204
#define ATTRID_VendorIDSource				0x0205
#ifdef HID_Profile
//attributes HID
#define ATTRID_DeviceReleaseNumber          0x0200
#define ATTRID_ParserVersion                0x0201
#define ATTRID_DeviceSubclass               0x0202
#define ATTRID_CountryCode                  0x0203
#define ATTRID_VirtualCable                 0x0204
#define ATTRID_ReconnectInitiate            0x0205
#define ATTRID_DescriptorList               0x0206
#define ATTRID_LANGIDBaseList               0x0207
#define ATTRID_SDPDisable                   0x0208
#define ATTRID_BatteryPower                 0x0209
#define ATTRID_RemoteWake                   0x020A
#define ATTRID_ProfileVersion               0x020B
#define ATTRID_SupervisionTimeout           0x020C
#define ATTRID_NormallyConnectable          0x020D
#define ATTRID_BootDevice                   0x020E
#define ATTRID_SSRHostMaxLatency            0x020F
#define ATTRID_SSRHostMinTimeout            0x0210
// reserved for HID attributes              0x0211-0x03FF
// available for HID language strings       0x0400-0xFFFF
#endif

#define AIROHA_COMPANY_ID					0x0094

#define VENDOR_ID_SOURCE_BLUETOOTH			0x0001
#define VENDOR_ID_SOURCE_USB				0x0002

//////////////////////////////////////////////////////////////////
//Attribute Data Element constant value
//////////////////////////////////////////////////////////////////

enum
{
	DELEM_NIL,
	DELEM_UINT,
	DELEM_SINT,
	DELEM_UUID,
	DELEM_TEXT,
	DELEM_BOOLEAN,
	DELEM_SEQUENCE,
	DELEM_ALTERNATIVE,
	DELEM_URL
};

enum
{
	DELEM_SIZE_1,
	DELEM_SIZE_2,
	DELEM_SIZE_4,
	DELEM_SIZE_8,
	DELEM_SIZE_16,
	DELEM_EXTRA_U8,
	DELEM_EXTRA_U16,
	DELEM_EXTRA_U32
};

#define DELEM_U8		((DELEM_UINT << 3) | DELEM_SIZE_1)
#define DELEM_U16		((DELEM_UINT << 3) | DELEM_SIZE_2)
#define DELEM_U32		((DELEM_UINT << 3) | DELEM_SIZE_4)
#define DELEM_U64		((DELEM_UINT << 3) | DELEM_SIZE_8)
#define DELEM_U128		((DELEM_UINT << 3) | DELEM_SIZE_16)

#define DELEM_UUID16	((DELEM_UUID << 3) | DELEM_SIZE_2)
#define DELEM_UUID32	((DELEM_UUID << 3) | DELEM_SIZE_4)
#define DELEM_UUID128	((DELEM_UUID << 3) | DELEM_SIZE_16)

#define DELEM_TEXT8		((DELEM_TEXT << 3) | DELEM_EXTRA_U8)
#define DELEM_TEXT16	((DELEM_TEXT << 3) | DELEM_EXTRA_U16)
#define DELEM_TEXT32	((DELEM_TEXT << 3) | DELEM_EXTRA_U32)

#define DELEM_BOOL		((DELEM_BOOLEAN << 3) | DELEM_SIZE_1)

#define DELEM_SEQU8		((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U8)
#define DELEM_SEQU16	((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U16)
#define DELEM_SEQU32	((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U32)

#define DELEM_TYPE(x)	((U8)(x) >> 3)
#define DELEM_SIZE(x)	((U8)(x) & 7)

//////////////////////////////////////////////////////////////////
//Service Record
//////////////////////////////////////////////////////////////////

#define GET_B0(value)	(U8)((value) & 0xFF)
#define GET_B1(value)	(U8)(((value) >> 8) & 0xFF)
#define GET_B2(value)	(U8)(((value) >> 16) & 0xFF)
#define GET_B3(value)	(U8)(((value) >> 24) & 0xFF)


#define SERVICE_HIBASE			0x0001

#define DEDEF_U8(x)				DELEM_U8, GET_B0(x)
#define DEDEF_U16(x)			DELEM_U16, GET_B1(x), GET_B0(x)
#define DEDEF_U32(x)			DELEM_U32, GET_B3(x), GET_B2(x), GET_B1(x), GET_B0(x)

#define DEDEF_UUID16(x)			DELEM_UUID16, GET_B1(x), GET_B0(x)

#define DEDEF_TEXT8(x)			DELEM_TEXT8, GET_B0(x)
#define DEDEF_TEXT16(x)			DELEM_TEXT16, GET_B1(x), GET_B0(x)

#define DEDEF_BOOL(x)			DELEM_BOOL, GET_B0(x)

#define DEDEF_SEQU8(x)			DELEM_SEQU8, GET_B0(x)
#define DEDEF_SEQU16(x)			DELEM_SEQU16, GET_B1(x), GET_B0(x)

#define SERVICE_HANDLE32(x)		DELEM_U32, GET_B1(SERVICE_HIBASE), GET_B0(SERVICE_HIBASE), GET_B1(x), GET_B0(x)
#define SERVICE_UUID16(x)		DEDEF_UUID16(x)
#define PROFILE_UUID16(x)		DEDEF_UUID16(x)
#define PROTOCOL_UUID16(x)		DEDEF_UUID16(x)
#define PSM_U16(x)				DEDEF_U16(x)
#define VERSION_U16(x)			DEDEF_U16(x)

#define ATTRID(id)				GET_B1(id), GET_B0(id)
#define ATTRLEN(len)			GET_B0(len)
#define ATTRNAME(p, name)		p##Attr_##name
#define ATTRDEF_ROM(p, name)	U8 CODE ATTRNAME(p, name) []
#define ATTRDEF_RAM(p, name)	U8 XDATA ATTRNAME(p, name) []

#define ATTRLISTNAME(p)			p##SdpAttrList
#define ATTRLISTDEF_ROM(p)		SdpServiceAttributePtr CODE ATTRLISTNAME(p) []
#define ATTRLISTDEF_RAM(p)		SdpServiceAttributePtr XDATA ATTRLISTNAME(p) []

#define ATTRCOUNT(p)			(sizeof(ATTRLISTNAME(p)) / sizeof(SdpServiceAttributePtr))

#define SRECORDNAME(p)			p##SdpServiceRecord
#define SRECORDDEF_ROM(p)		SdpServiceRecord CODE SRECORDNAME(p)
#define SRECORDDEF_RAM(p)		SdpServiceRecord XDATA SRECORDNAME(p)

typedef struct 
{										 
	U16 attributeId;
	U8 attributeLen;
	U8 attributeValue[1];
} SdpServiceAttribute;

typedef SdpServiceAttribute GENERIC_PTR SdpServiceAttributePtr;
typedef SdpServiceAttributePtr GENERIC_PTR SdpServiceAttributeListPtr;

typedef struct 
{
	U8 profileId;
	U8 attributeCount;
	SdpServiceAttributeListPtr attributes;
} SdpServiceRecord;

typedef SdpServiceRecord GENERIC_PTR SdpServiceRecordPtr;

typedef struct sdp_service_node
{
	SdpServiceRecordPtr serviceRecordPtr;
	struct sdp_service_node XDATA_PTR nextNode;
} SdpServiceNode;

//////////////////////////////////////////////////////////////////
//SDAP Public Functions
//////////////////////////////////////////////////////////////////

PUBLIC void SDAP_RegisterServiceRecord(SdpServiceNode XDATA_PTR serviceNode);
PUBLIC void SDAP_UnregisterServiceRecord(U8 profileId);

PUBLIC SdpServiceAttributePtr SDAP_GetServiceAttribute(U8 profileId, U16 attributeId);

PUBLIC U8 SDAP_GetDataElementSize(U8 dataElemHeader);
PUBLIC U32 SDAP_GetAdditionalLength(U8 GENERIC_PTR ptr); /*Note: Using generic pointer because SDAP_FindUUID16 has generic pointer*/
PUBLIC U32 SDAP_GetSeqAdditionalLength(U8 XDATA_PTR ptr);

PUBLIC void SDAP_SendSearchRequest(MMI_PROFILE_INFO XDATA_PTR profileInfo, U8 XDATA_PTR msgPtr);

PUBLIC BOOL SDAP_IsIOS(U8 linkIndex);

#endif
