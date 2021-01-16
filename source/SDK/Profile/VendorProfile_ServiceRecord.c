#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if VENDOR_PROFILE

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\SDK\include\SDK_SCN.h"

// ====================== Vendor Profile ======================

ATTRDEF_ROM(vprofile, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_VENDOR_PROFILE) };
ATTRDEF_ROM(vprofile, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(19), DEDEF_SEQU8(17),
											DELEM_UUID128, 0xF2, 0x5E, 0x89, 0x35, 0xA7, 0x19, 0x32, 0xBB, 0x7C, 0x99, 0x0A, 0x20, 0x43, 0x69, 0x72, 0xFD}; // This is the vendor profile's UUID.
ATTRDEF_ROM(vprofile, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(14), DEDEF_SEQU8(12),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
																	DEDEF_SEQU8(5), PROTOCOL_UUID16(PROTOCOL_UUID_RFCOMM), DEDEF_U8(SCN_VENDOR_PROFILE) };
ATTRDEF_ROM(vprofile, ServiceName) = 
			{ ATTRID(ATTRID_ServiceName), ATTRLEN(15), DEDEF_TEXT8(13), 'V', 'e', 'n', 'd', 'o', 'r', 'P', 'r', 'o', 'f', 'i', 'l', 'e' };

ATTRLISTDEF_ROM(vprofile) =
{
	(SdpServiceAttributePtr) ATTRNAME(vprofile, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(vprofile, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(vprofile, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(vprofile, ServiceName),
};

SRECORDDEF_ROM(vprofile) =
{
	PROFILE_VENDOR_PROFILE,
	ATTRCOUNT(vprofile),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(vprofile)
};

SdpServiceNode gVProfile_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&vprofile),
	NULL
};

#endif
