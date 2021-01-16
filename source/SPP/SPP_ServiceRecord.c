#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#define VERSION_SPP		0x0102

#ifdef SPP_Profile

// ====================== SPP ======================

ATTRDEF_ROM(spp, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_SERIAL_PORT) };
ATTRDEF_ROM(spp, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_SERIAL_PORT) };
ATTRDEF_ROM(spp, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(14), DEDEF_SEQU8(12),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
																	DEDEF_SEQU8(5), PROTOCOL_UUID16(PROTOCOL_UUID_RFCOMM), DEDEF_U8(SCN_SERIAL_PORT) };
ATTRDEF_ROM(spp, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_SPP), VERSION_U16(VERSION_SPP) };
ATTRDEF_ROM(spp, ServiceName) = 
			{ ATTRID(ATTRID_ServiceName), ATTRLEN(12), DEDEF_TEXT8(10), 'S', 'e', 'r', 'i', 'a', 'l', 'P', 'o', 'r', 't' };

ATTRLISTDEF_ROM(spp) =
{
	(SdpServiceAttributePtr) ATTRNAME(spp, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(spp, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(spp, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(spp, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(spp, ServiceName),
};

SRECORDDEF_ROM(spp) =
{
	PROFILE_SERIAL_PORT,
	ATTRCOUNT(spp),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(spp)
};

SdpServiceNode gSPP_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&spp),
	NULL
};

#endif
