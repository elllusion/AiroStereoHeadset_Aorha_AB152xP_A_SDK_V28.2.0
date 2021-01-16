
// ====================== HSP-AG ======================

ATTRDEF_ROM(hsp, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_HEADSET) };
ATTRDEF_ROM(hsp, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(11), DEDEF_SEQU8(9), SERVICE_UUID16(SERVICE_UUID_HEADSET_AG), SERVICE_UUID16(SERVICE_UUID_GENERIC_AUDIO), SERVICE_UUID16(SERVICE_UUID_HEADSET_HS) };
ATTRDEF_ROM(hsp, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(14), DEDEF_SEQU8(12),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
																	DEDEF_SEQU8(5), PROTOCOL_UUID16(PROTOCOL_UUID_RFCOMM), DEDEF_U8(SCN_HEADSET) };
ATTRDEF_ROM(hsp, BrowseGroupList) = 
			{ ATTRID(ATTRID_BrowseGroupList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_PUBLIC_BROWSE_ROOT) };
ATTRDEF_ROM(hsp, LangBaseAttributeIdList) = 
			{ ATTRID(ATTRID_LangBaseAttributeIdList), ATTRLEN(11), DEDEF_SEQU8(9), DEDEF_U16(0x656e), DEDEF_U16(0x006a), DEDEF_U16(ATTRID_ServiceName) };
ATTRDEF_ROM(hsp, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_HSP), VERSION_U16(VERSION_HSP) };
ATTRDEF_ROM(hsp, ServiceName) = 
			{ ATTRID(ATTRID_ServiceName), ATTRLEN(12), DEDEF_TEXT8(10), 'H', 'e', 'a', 'd', 's', 'e', 't', '-', 'A', 'G' };
ATTRDEF_ROM(hsp, RemoteAudioVolControl) = 
			{ ATTRID(ATTRID_RemoteAudioVolControl), ATTRLEN(2), DEDEF_BOOL(1/*true*/) };

ATTRLISTDEF_ROM(hsp) =
{
	(SdpServiceAttributePtr) ATTRNAME(hsp, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(hsp, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(hsp, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hsp, BrowseGroupList),
	(SdpServiceAttributePtr) ATTRNAME(hsp, LangBaseAttributeIdList),
	(SdpServiceAttributePtr) ATTRNAME(hsp, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hsp, ServiceName),
	(SdpServiceAttributePtr) ATTRNAME(hsp, RemoteAudioVolControl),
};

SRECORDDEF_ROM(hsp) =
{
	PROFILE_HEADSET,
	ATTRCOUNT(hsp),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(hsp)
};

SdpServiceNode gHSP_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&hsp),
	NULL
};

// ====================== HFP-AG ======================

ATTRDEF_ROM(hfp, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_HANDSFREE) };
ATTRDEF_ROM(hfp, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(8), DEDEF_SEQU8(6), SERVICE_UUID16(SERVICE_UUID_HANDSFREE_AG), SERVICE_UUID16(SERVICE_UUID_GENERIC_AUDIO) };
ATTRDEF_ROM(hfp, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(14), DEDEF_SEQU8(12),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
																	DEDEF_SEQU8(5), PROTOCOL_UUID16(PROTOCOL_UUID_RFCOMM), DEDEF_U8(SCN_HANDSFREE) };
ATTRDEF_ROM(hfp, BrowseGroupList) = 
			{ ATTRID(ATTRID_BrowseGroupList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_PUBLIC_BROWSE_ROOT) };
ATTRDEF_ROM(hfp, LangBaseAttributeIdList) = 
			{ ATTRID(ATTRID_LangBaseAttributeIdList), ATTRLEN(11), DEDEF_SEQU8(9), DEDEF_U16(0x656e), DEDEF_U16(0x006a), DEDEF_U16(ATTRID_ServiceName) };
ATTRDEF_ROM(hfp, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_HFP), VERSION_U16(VERSION_HFP) };
ATTRDEF_ROM(hfp, ServiceName) = 
			{ ATTRID(ATTRID_ServiceName), ATTRLEN(17), DEDEF_TEXT8(13), 'V', 'o', 'i', 'c', 'e', ' ', 'g', 'a', 't', 'e', 'w', 'a', 'y' };
ATTRDEF_RAM(hfp, SupportedFeatures) = 
			{ ATTRID(ATTRID_SupportedFeatures), ATTRLEN(3), DEDEF_U16(0x0000) };

ATTRLISTDEF_ROM(hfp) =
{
	(SdpServiceAttributePtr) ATTRNAME(hfp, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(hfp, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(hfp, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hfp, BrowseGroupList),
	(SdpServiceAttributePtr) ATTRNAME(hfp, LangBaseAttributeIdList),
	(SdpServiceAttributePtr) ATTRNAME(hfp, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hfp, ServiceName),
	(SdpServiceAttributePtr) ATTRNAME(hfp, SupportedFeatures),
};

SRECORDDEF_ROM(hfp) =
{
	PROFILE_HANDSFREE,
	ATTRCOUNT(hfp),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(hfp)
};

SdpServiceNode gHFP_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&hfp),
	NULL
};
