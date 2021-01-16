
// ====================== A2DP Sink ======================

ATTRDEF_ROM(a2dpSink, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_A2DP_SINK) };
ATTRDEF_ROM(a2dpSink, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_AUDIO_SINK) };
ATTRDEF_ROM(a2dpSink, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(18), DEDEF_SEQU8(16),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_AVDTP),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_AVDTP), VERSION_U16(VERSION_AVDTP) };
ATTRDEF_ROM(a2dpSink, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_A2DP), VERSION_U16(VERSION_A2DP) };
ATTRDEF_RAM(a2dpSink, SupportedFeatures) = 
			{ ATTRID(ATTRID_SupportedFeatures), ATTRLEN(3), DEDEF_U16(0x0000) };

ATTRLISTDEF_ROM(a2dpSink) =
{
	(SdpServiceAttributePtr) ATTRNAME(a2dpSink, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSink, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSink, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSink, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(a2dpSink, SupportedFeatures),
};

SRECORDDEF_ROM(a2dpSink) =
{
	PROFILE_A2DP_SINK,
	ATTRCOUNT(a2dpSink),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(a2dpSink)
};

SdpServiceNode gA2DP_SinkServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&a2dpSink),
	NULL
};
