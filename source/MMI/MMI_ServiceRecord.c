#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#define VERSION_MPS		0x0100
#define VERSION_PNP		0x0103

// ====================== MPS ======================

ATTRDEF_ROM(mps, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_MPS) };
ATTRDEF_ROM(mps, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_MULTI_PROFILE_SPEC) };
ATTRDEF_ROM(mps, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_MPS), VERSION_U16(VERSION_MPS) };
ATTRDEF_ROM(mps, SupportedScenarios_MPSD) = 
			{ ATTRID(ATTRID_SupportedScenarios_MPSD), ATTRLEN(9), DELEM_U64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0xAA };
ATTRDEF_ROM(mps, SupportedScenarios_MPMD) = 
			{ ATTRID(ATTRID_SupportedScenarios_MPMD), ATTRLEN(9), DELEM_U64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x95 };
ATTRDEF_ROM(mps, SupportedProfilesProtocols) = 
			{ ATTRID(ATTRID_SupportedProfilesProtocols), ATTRLEN(3), DEDEF_U16(0x0006) };

ATTRLISTDEF_ROM(mps) =
{
	(SdpServiceAttributePtr) ATTRNAME(mps, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(mps, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(mps, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(mps, SupportedScenarios_MPSD),
	(SdpServiceAttributePtr) ATTRNAME(mps, SupportedScenarios_MPMD),
	(SdpServiceAttributePtr) ATTRNAME(mps, SupportedProfilesProtocols),
};

SRECORDDEF_ROM(mps) =
{
	PROFILE_MPS,
	ATTRCOUNT(mps),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(mps)
};

SdpServiceNode gMMI_MpsServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&mps),
	NULL
};

// ====================== PNP ======================

ATTRDEF_ROM(pnp, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_PNP) };
ATTRDEF_ROM(pnp, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_PNP_INFORMATION) };
ATTRDEF_ROM(pnp, SpecifcationID) = 
			{ ATTRID(ATTRID_SpecifcationID), ATTRLEN(3), VERSION_U16(VERSION_PNP) };
ATTRDEF_ROM(pnp, VendorID) = 
			{ ATTRID(ATTRID_VendorID), ATTRLEN(3), DEDEF_U16(AIROHA_COMPANY_ID) };
ATTRDEF_RAM(pnp, ProductID) = 
			{ ATTRID(ATTRID_ProductID), ATTRLEN(3), DEDEF_U16(0x0000) };
ATTRDEF_RAM(pnp, ProductVersion) = 
			{ ATTRID(ATTRID_ProductVersion), ATTRLEN(3), DEDEF_U16(0x0000) };
ATTRDEF_ROM(pnp, PrimaryRecord) = 
			{ ATTRID(ATTRID_PrimaryRecord), ATTRLEN(2), DEDEF_BOOL(1/*primary*/) };
ATTRDEF_ROM(pnp, VendorIDSource) = 
			{ ATTRID(ATTRID_VendorIDSource), ATTRLEN(3), DEDEF_U16(VENDOR_ID_SOURCE_BLUETOOTH) };
#ifdef HID_Profile			
ATTRDEF_ROM(pnp, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(15), DEDEF_SEQU8(13),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_SDP),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PSM_SDP) };			
#endif																	

ATTRLISTDEF_ROM(pnp) =
{
	(SdpServiceAttributePtr) ATTRNAME(pnp, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(pnp, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(pnp, SpecifcationID),
	(SdpServiceAttributePtr) ATTRNAME(pnp, VendorID),
	(SdpServiceAttributePtr) ATTRNAME(pnp, ProductID),
	(SdpServiceAttributePtr) ATTRNAME(pnp, ProductVersion),
	(SdpServiceAttributePtr) ATTRNAME(pnp, PrimaryRecord),
	(SdpServiceAttributePtr) ATTRNAME(pnp, VendorIDSource),
	#ifdef HID_Profile
	(SdpServiceAttributePtr) ATTRNAME(pnp, ProtocolDescriptorList),	
	#endif
};

SRECORDDEF_ROM(pnp) =
{
	PROFILE_PNP,
	ATTRCOUNT(pnp),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(pnp)
};

SdpServiceNode gMMI_PnpServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&pnp),
	NULL
};
