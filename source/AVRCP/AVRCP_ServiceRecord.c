#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#define VERSION_AVCTP	0x0104
#define VERSION_AVRCP	0x0106

#ifdef AVRCP_Profile

// ====================== AVRCP TG ======================

ATTRDEF_ROM(avrcpTg, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_AVRCP_TG) };
ATTRDEF_ROM(avrcpTg, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_REMOTE_CONTROL_TRG) };
ATTRDEF_ROM(avrcpTg, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(18), DEDEF_SEQU8(16),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_AVCTP),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_AVCTP), VERSION_U16(VERSION_AVCTP) };
ATTRDEF_ROM(avrcpTg, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_AVRCP), VERSION_U16(VERSION_AVRCP) };
ATTRDEF_ROM(avrcpTg, SupportedFeatures) = 
			{ ATTRID(ATTRID_SupportedFeatures), ATTRLEN(3), DEDEF_U16(0x0002) };

ATTRLISTDEF_ROM(avrcpTg) =
{
	(SdpServiceAttributePtr) ATTRNAME(avrcpTg, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(avrcpTg, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpTg, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpTg, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpTg, SupportedFeatures),
};

SRECORDDEF_ROM(avrcpTg) =
{
	PROFILE_AVRCP_TG,
	ATTRCOUNT(avrcpTg),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(avrcpTg)
};

SdpServiceNode gAVRCP_TgServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&avrcpTg),
	NULL
};

// ====================== AVRCP CT ======================

ATTRDEF_ROM(avrcpCt, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_AVRCP_CT) };
ATTRDEF_ROM(avrcpCt, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(8), DEDEF_SEQU8(6), SERVICE_UUID16(SERVICE_UUID_REMOTE_CONTROL), SERVICE_UUID16(SERVICE_UUID_REMOTE_CONTROL_CTL) };
ATTRDEF_ROM(avrcpCt, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(18), DEDEF_SEQU8(16),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_AVCTP),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_AVCTP), VERSION_U16(VERSION_AVCTP) };
ATTRDEF_ROM(avrcpCt, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_AVRCP), VERSION_U16(VERSION_AVRCP) };
ATTRDEF_ROM(avrcpCt, SupportedFeatures) = 
			{ ATTRID(ATTRID_SupportedFeatures), ATTRLEN(3), DEDEF_U16(0x0001) };

ATTRLISTDEF_ROM(avrcpCt) =
{
	(SdpServiceAttributePtr) ATTRNAME(avrcpCt, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(avrcpCt, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpCt, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpCt, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(avrcpCt, SupportedFeatures),
};

SRECORDDEF_ROM(avrcpCt) =
{
	PROFILE_AVRCP_CT,
	ATTRCOUNT(avrcpCt),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(avrcpCt)
};

SdpServiceNode gAVRCP_CtServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&avrcpCt),
	NULL
};

#endif
