#ifndef _STRUCT_A2DP_NVRAM_CTL_H_
#define _STRUCT_A2DP_NVRAM_CTL_H_

#include <config.h>
#include <cpu.h>
#include <crc16.h>

#include "bt_config_profile.h"

#define A2DP_SEID_NUM	(SUPPORT_SBC_SNK + SUPPORT_AAC_SNK + SUPPORT_SBC_SRC)

enum
{
	DUMMY_SEID, /* to ensure real SEID start from 1 */
	SBC_SNK_SEID,
	AAC_SNK_SEID,
	SBC_SRC_SEID,
	
	AUX_ENCODER_SEID = 0x3E,
};

enum
{
	A2DP_SEP_SRC,
	A2DP_SEP_SNK
};

enum
{
	A2DP_AUDIO,
	A2DP_VIDEO,
	A2DP_MULTIMEDIA,
};

typedef struct
{
	U8 rfa2:1;			//First para if it exists.
	U8 inUse:1;
	U8 firstAcpSEID:6;
	U8 rfa3:3;
	U8 tsep:1;
	U8 mediaType:4;
}AcpStreamEndPointIdCtl;

typedef struct
{
	U8 Content[2+4+8+2];	//Media Transport + Content Protection + Media Codec + Delay Reporting
}SBCServiceCapabilities;

typedef struct
{
	U8 content1;
	U8 content2;
	U8 minBitPool;
	U8 maxBitPool;
}SBCCodecSettings;

typedef struct
{
	U8 Content[2+4+10+2];	//Media Transport + Content Protection + Media Codec + Delay Reporting
}AACServiceCapabilities;

typedef struct
{
	U8 objectType;
	U8 sampleRate1;
	U8 sampleRate2;
	U8 bitRate1;
	U8 bitRate2;
	U8 bitRate3;
}AACCodecSettings;

#define MAX_A2DP_SEP_NUM	4

typedef struct
{
	U8						a2dp_sep_num;
	AcpStreamEndPointIdCtl	asepidCtl[MAX_A2DP_SEP_NUM];
	SBCServiceCapabilities	sbcServiceCapabilities;
	SBCCodecSettings		sbcDefaultSettings;
	AACServiceCapabilities	aacServiceCapabilities;
	AACCodecSettings		aacDefaultSettings;
}A2DP_SEID_CTL_TYPE;

typedef struct
{
	U16 defaultDelayReport;	//unit: 0.1ms
 	A2DP_SEID_CTL_TYPE seidCtl;
}A2DP_NVRAM_CTL_TYPE;

typedef struct
{
	A2DP_NVRAM_CTL_TYPE		a2dp_nvram_ctl;
	CRC16 CRC;
} SECTOR_A2DP_NVRAM_CTL_STRU;


enum
{
	A2DP_CONNECTION_REQ_REJ = 0x10,  // 0x0X r reserverd for AVDTP
};

enum 
{
	A2DP_LINK_ALREADY_EXIST = 0x11,
};

enum
{
	MEDIA_TRANSPORT 	= 0x01,
	REPORTING			= 0x02,
	RECOVERY			= 0x03,
	CONTENT_PROTECTION	= 0x04,
	HEADER_COMPRESSION	= 0x05,
	MULTIPLEXING		= 0x06,
	MEDIA_CODEC			= 0x07,
	DELAY_REPORTING		= 0x08,
};

enum
{
	AUDIO_TYPE = 0x00,
	VIDEO_TYPE = 0x01,
	MULTIMEDIA_TYPE = 0x02,
};

enum
{
	SBC_CODEC 		= 0x00,
	MP3_CODEC		= 0x01,
	AAC_CODEC		= 0x02,
	ATRAC_CODEC		= 0x04,
	NON_A2DP_CODEC	= 0xFF,	
};

enum
{
	A2DP_CP_NONE,
	A2DP_CP_DTCP,
	A2DP_CP_SCMS_T
};

#define	A2DP_SBC_SAMPLE_FREQ_16000		0x80
#define	A2DP_SBC_SAMPLE_FREQ_32000		0x40
#define	A2DP_SBC_SAMPLE_FREQ_44100		0x20
#define	A2DP_SBC_SAMPLE_FREQ_48000		0x10
#define A2DP_SBC_CHANNEL_MODE_MONO		0x08
#define A2DP_SBC_CHANNEL_MODE_DUAL		0x04
#define A2DP_SBC_CHANNEL_MODE_STEREO	0x02
#define A2DP_SBC_CHANNEL_MODE_JOINT_STEREO	0x01
#define	A2DP_SBC_BLOCK_LENGTH_4 		0x80
#define	A2DP_SBC_BLOCK_LENGTH_8 		0x40
#define	A2DP_SBC_BLOCK_LENGTH_12 		0x20
#define	A2DP_SBC_BLOCK_LENGTH_16 		0x10
#define A2DP_SBC_SUBBAND_NUMBER_4		0x08
#define A2DP_SBC_SUBBAND_NUMBER_8		0x04
#define A2DP_SBC_ALLOCATION_SNR			0x02
#define A2DP_SBC_ALLOCATION_LOUDNESS	0x01
#define A2DP_SBC_MIN_BITPOOL_VALUE		0x08	// dr chi suggests
#if defined(TWS_SETTINGS) && !defined(BLUETOOTH_SPEAKER)
#define A2DP_SBC_MAX_BITPOOL_VALUE  	0x23  // 35, AiroStereoHeadset
#else
#define A2DP_SBC_MAX_BITPOOL_VALUE  	0x35  // 53, by spec
#endif


#define A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC			0x80
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_LC			0x40
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_LTP			0x20
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_SCALABLE		0x10

#define A2DP_AAC_SAMPLE_FREQ_8000		0x80 //byte 1
#define A2DP_AAC_SAMPLE_FREQ_11025		0x40
#define A2DP_AAC_SAMPLE_FREQ_12000		0x20
#define A2DP_AAC_SAMPLE_FREQ_16000		0x10
#define A2DP_AAC_SAMPLE_FREQ_22050		0x08
#define A2DP_AAC_SAMPLE_FREQ_24000		0x04
#define A2DP_AAC_SAMPLE_FREQ_32000		0x02
#define A2DP_AAC_SAMPLE_FREQ_44100		0x01
#define A2DP_AAC_SAMPLE_FREQ_48000		0x80  //byte2
#define A2DP_AAC_SAMPLE_FREQ_64000		0x40
#define A2DP_AAC_SAMPLE_FREQ_88200		0x20
#define A2DP_AAC_SAMPLE_FREQ_96000		0x10
#define A2DP_AAC_CHANNELS_1				0x08
#define A2DP_AAC_CHANNELS_2				0x04
#define A2DP_AAC_VARIABLE_BIT_RATE		0x80
#define A2DP_AAC_BIT_RATE				320000L //To be defined

#endif //_STRUCT_A2DP_NVRAM_CTL_H_