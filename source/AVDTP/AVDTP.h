//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AVDTP_H_
#define _AVDTP_H_

#include "os.h"
#include "..\L2CAP\L2CAP_interface.h"

#define A2DP_SIGNAL_MTU_SIZE		200
// --------------------------------------
// |         |		User Payload		|
// | DH5     |		0~339				|
// | 2-DH5   |		0~678				|
// | 3-DH5   |		0~1021				|
// --------------------------------------
// ** The default value of MTU is 672 **
#define A2DP_MEDIA_MTU_SIZE			672		/*(OSMEM2_SMALL_RX_BLKSIZE - (L2CAP_RX_PDU_OFFSET + L2CAP_HEADER_LEN))*/
#if defined AB1520D
#define A2DP_MEDIA_AAC_MTU_SIZE			1020
#else
#define A2DP_MEDIA_AAC_MTU_SIZE			(A2DP_MEDIA_MTU_SIZE)
#endif

enum
{
	AVDTP_STATE_IDLE,
	AVDTP_STATE_CONNECTING,
	AVDTP_STATE_IDLE_SIGNAL_CHANNEL,
	AVDTP_STATE_WAIT_CONFIG_MEDIA_CHANNEL,
	AVDTP_STATE_IDLE_MEDIA_CHANNEL,
	AVDTP_STATE_WAIT_CONFIG_REPORTING_CHANNEL,
	AVDTP_STATE_IDLE_REPORTING_CHANNEL,
	AVDTP_STATE_WAIT_CONFIG_RECOVERYING_CHANNEL,
	AVDTP_STATE_IDLE_RECOVERYING_CHANNEL,
};

typedef	struct
{
	U8 state;
	AVDTP_PROFILE_DESC CODE_PTR avdtpProfileDesc;
	L2CAP_CHANNEL_HDL l2capChannelHdl[MAX_AVDTP_CHANNEL_TYPE_NUM];
	union {
		struct {
			U8 transactionLabel : 4;
			U8 isPassive : 1;
			U8 reserved : 3;
		} bits;
		U8 byte;
	} misc;
} AVDTP_LINK_INFO;

typedef	struct
{
	AVDTP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
} AVDTP_CTL_TYPE;


#endif
