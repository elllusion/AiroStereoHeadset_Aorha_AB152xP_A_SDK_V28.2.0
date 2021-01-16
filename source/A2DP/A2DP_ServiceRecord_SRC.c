
// ====================== A2DP Source ======================

ATTRDEF_ROM(a2dpSource, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_A2DP_SOURCE) };
ATTRDEF_ROM(a2dpSource, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_AUDIO_SOURCE) };
ATTRDEF_ROM(a2dpSource, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(18), DEDEF_SEQU8(16),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_AVDTP),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_AVDTP), VERSION_U16(VERSION_AVDTP) };
ATTRDEF_ROM(a2dpSource, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_A2DP), VERSION_U16(VERSION_A2DP) };
ATTRDEF_RAM(a2dpSource, SupportedFeatures) = 
			{ ATTRID(ATTRID_SupportedFeatures), ATTRLEN(3), DEDEF_U16(0x0000) };

ATTRLISTDEF_ROM(a2dpSource) =
{
	(SdpServiceAttributePtr) ATTRNAME(a2dpSource, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSource, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSource, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSource, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSource, SupportedFeatures),
};

SRECORDDEF_ROM(a2dpSource) =
{
	PROFILE_A2DP_SOURCE,
	ATTRCOUNT(a2dpSource),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(a2dpSource)
};

SdpServiceNode gA2DP_SourceServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&a2dpSource),
	NULL
};
