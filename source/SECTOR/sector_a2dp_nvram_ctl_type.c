//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#include "sector_a2dp_nvram_ctl.h"

#pragma userclass (HCONST = CONFIG)


#ifdef A2DP_Profile

SECTOR_A2DP_NVRAM_CTL_STRU CODE gA2DP_nvram_ctl_init =
{
	{
		100,	//defaultDelayReport, unit: 0.1ms
		//- capabilities
		{
			A2DP_SEID_NUM,	//a2dp_sep_num
			{ //asepidCtl
				#if SUPPORT_SBC_SNK
				{0x00, 0x00, SBC_SNK_SEID, 0x00, A2DP_SEP_SNK, A2DP_AUDIO},
				#endif
				#if SUPPORT_AAC_SNK
				{0x00, 0x00, AAC_SNK_SEID, 0x00, A2DP_SEP_SNK, A2DP_AUDIO},
				#endif
				#if SUPPORT_SBC_SRC
				{0x00, 0x00, SBC_SRC_SEID, 0x00, A2DP_SEP_SRC, A2DP_AUDIO},
				#endif
			},

			{ //sbcServiceCapabilities
				MEDIA_TRANSPORT, 0x00,  // media transport : 0x01, length is 0x00
				CONTENT_PROTECTION, 02,	// content protection: 0x04
				(U8)(A2DP_CP_SCMS_T), (U8)(A2DP_CP_SCMS_T >> 8),
				MEDIA_CODEC, 0x06, 		// media codec  : 0x07, length is 0x06
				AUDIO_TYPE, 			// Audio type
				SBC_CODEC, 				// Codec type
				(A2DP_SBC_SAMPLE_FREQ_32000|A2DP_SBC_SAMPLE_FREQ_44100|A2DP_SBC_SAMPLE_FREQ_48000|A2DP_SBC_CHANNEL_MODE_MONO|A2DP_SBC_CHANNEL_MODE_DUAL|A2DP_SBC_CHANNEL_MODE_STEREO|A2DP_SBC_CHANNEL_MODE_JOINT_STEREO), // sample freq and channel mode
				(A2DP_SBC_BLOCK_LENGTH_4|A2DP_SBC_BLOCK_LENGTH_8|A2DP_SBC_BLOCK_LENGTH_12|A2DP_SBC_BLOCK_LENGTH_16|A2DP_SBC_SUBBAND_NUMBER_4|A2DP_SBC_SUBBAND_NUMBER_8|A2DP_SBC_ALLOCATION_SNR|A2DP_SBC_ALLOCATION_LOUDNESS), // subband number and allocation mode
				A2DP_SBC_MIN_BITPOOL_VALUE,   // min bit pool value
				A2DP_SBC_MAX_BITPOOL_VALUE,	// max bit pool value
				DELAY_REPORTING, 0x00,	// delay reporting
			},
			{ //sbcDefaultSettings
				(A2DP_SBC_SAMPLE_FREQ_44100|A2DP_SBC_CHANNEL_MODE_JOINT_STEREO), // sample freq and channel mode
				(A2DP_SBC_BLOCK_LENGTH_16|A2DP_SBC_SUBBAND_NUMBER_8|A2DP_SBC_ALLOCATION_LOUDNESS), // subband number and allocation mode
				A2DP_SBC_MIN_BITPOOL_VALUE,   // min bit pool value
				A2DP_SBC_MAX_BITPOOL_VALUE	// max bit pool value
			},
			
			{ //aacServiceCapabilities
				MEDIA_TRANSPORT, 0x00, // media transport : 0x01, length is 0x00
				CONTENT_PROTECTION, 02,	// content protection: 0x04
				(U8)(A2DP_CP_SCMS_T), (U8)(A2DP_CP_SCMS_T >> 8),
				MEDIA_CODEC , 0x08,
				AUDIO_TYPE,
				AAC_CODEC,
				A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC,  //ObjectType
				A2DP_AAC_SAMPLE_FREQ_44100|A2DP_AAC_SAMPLE_FREQ_32000|A2DP_AAC_SAMPLE_FREQ_24000|A2DP_AAC_SAMPLE_FREQ_22050|A2DP_AAC_SAMPLE_FREQ_16000|A2DP_AAC_SAMPLE_FREQ_12000|A2DP_AAC_SAMPLE_FREQ_11025|A2DP_AAC_SAMPLE_FREQ_8000,  // Sample Freq
				A2DP_AAC_SAMPLE_FREQ_48000|A2DP_AAC_CHANNELS_1|A2DP_AAC_CHANNELS_2,// Sample Freq + Channels
				A2DP_AAC_VARIABLE_BIT_RATE|((U8)(A2DP_AAC_BIT_RATE>>16)),
				(U8)(A2DP_AAC_BIT_RATE>>8),
				(U8)A2DP_AAC_BIT_RATE,
				DELAY_REPORTING, 0x00,	// delay reporting
			},
			{ //aacDefaultSettings
				A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC,	//ObjectType
				A2DP_AAC_SAMPLE_FREQ_44100,  // Sample Freq
				A2DP_AAC_CHANNELS_2,// Sample Freq + Channels
				A2DP_AAC_VARIABLE_BIT_RATE|((U8)(A2DP_AAC_BIT_RATE>>16)),
				(U8)(A2DP_AAC_BIT_RATE>>8),
				(U8)A2DP_AAC_BIT_RATE,
			},
		},
	},
    //- CRC
    {0,0}
};

#endif