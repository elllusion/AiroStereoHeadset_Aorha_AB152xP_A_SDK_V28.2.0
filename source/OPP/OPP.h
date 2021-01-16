//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _OPP_H_
#define _OPP_H_

#include "OPP_interface.h"
#include "..\MMI\MMI_interface.h"
#include "..\MMI\MMI_SCN.h"
#include "..\RFCOMM\RFCOMM_interface.h"
#include "..\OBEX\OBEX_interface.h"

#define OPP_RX_BUF_REUSE		1
#define OPP_DELAY_REPEAT		0

#define OPP_UART_BUF_NUM	3

#define OPP_MAX_UART_SIZE		240

#if 0
#define OPP_MAX_FRAME_SIZE		240
#define OPP_MAX_PACKET_SIZE		65520 //(OPP_MAX_FRAME_SIZE * 273)
#define OPP_RX_CREDITS_MAX		6
#define OPP_RX_CREDITS_GRANT	3
#define OPP_DEALY_CUMULATIVE	8 //based on frame count
#endif
#if 0
#define OPP_MAX_FRAME_SIZE		480
#define OPP_MAX_PACKET_SIZE		65280 //(OPP_MAX_FRAME_SIZE * 136)
#define OPP_RX_CREDITS_MAX		4
#define OPP_RX_CREDITS_GRANT	3
#define OPP_DEALY_CUMULATIVE	4 //based on frame count
#endif
#if 0
#define OPP_MAX_FRAME_SIZE		640
#define OPP_MAX_PACKET_SIZE		65280 //(OPP_MAX_FRAME_SIZE * 102)
#define OPP_RX_CREDITS_MAX		3
#define OPP_RX_CREDITS_GRANT	2
#define OPP_DEALY_CUMULATIVE	3 //based on frame count
#endif

#if 1
#define OPP_MAX_FRAME_SIZE		640
#define OPP_MAX_PACKET_SIZE		65280 //(OPP_MAX_FRAME_SIZE * 102)
#define OPP_RX_CREDITS_MAX		3
#define OPP_RX_CREDITS_GRANT	1
#define OPP_DEALY_CUMULATIVE	3 //based on frame count
#endif

#define OPP_BODY_CTRL_START		0x01
#define OPP_BODY_CTRL_END		0x02

#define OPP_FRAME_NATIVE		0x00
#define OPP_FRAME_START			0x01
#define OPP_FRAME_MIDDLE		0x02

typedef struct
{
	U8 opCode;
	U8 bodyCtrl;
	#if OPP_DELAY_REPEAT
	U8 delayRepeat;
	U8 delayCumulative;
	#endif
	U16 bodyFrameLen;
	U16 bodyDataLen;
	U8 XDATA_PTR bodyDataPtr;
} OppObexCtrl;

typedef struct
{
	MMI_PROFILE_INFO mmiProfileInfo;
	OSMQ OSMQ_OPP_Data_From_RFCOMM;
	U8 rxCredits;
	OppObexCtrl obexCtrl;
} OPP_LINK_INFO;

typedef struct
{
	OPP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
} OPP_CTL;

#endif
