#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#ifdef AIRAPP_Profile
ATTRDEF_ROM(airapp, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_AIR_APP) };
ATTRDEF_ROM(airapp, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(19), DEDEF_SEQU8(17),
											DELEM_UUID128, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
ATTRDEF_ROM(airapp, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(14), DEDEF_SEQU8(12),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
																	DEDEF_SEQU8(5), PROTOCOL_UUID16(PROTOCOL_UUID_RFCOMM), DEDEF_U8(SCN_AIR_APP) };
ATTRDEF_ROM(airapp, ServiceName) = 
			{ ATTRID(ATTRID_ServiceName), ATTRLEN(12), DEDEF_TEXT8(10), 'A', 'i', 'r', 'o', 'h', 'a', ' ', 'A', 'p', 'p' };

ATTRLISTDEF_ROM(airapp) =
{
	(SdpServiceAttributePtr) ATTRNAME(airapp, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(airapp, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(airapp, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(airapp, ServiceName),
};

SRECORDDEF_ROM(airapp) =
{
	PROFILE_AIR_APP,
	ATTRCOUNT(airapp),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(airapp)
};

SdpServiceNode gAirApp_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&airapp),
	NULL
};
#endif //AIRAPP_Profile