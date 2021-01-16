//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _L2CAP_ROM_H_ 
#define _L2CAP_ROM_H_

#include "L2CAP_ROM_interface.h"
#include "hci.h"

#ifdef AIR_UPDATE_ENABLE

typedef struct
{
    U16 len;
    U16 cid;
}L2CAP_DATA_HDR_STRU;

typedef struct
{
    U8  opCode;
    U8  identifier;
    U16 len;
}L2CAP_SIGNALING_HEADER_STRU;

typedef union
{
    U8 payload[1];
    L2CAP_SIGNALING_HEADER_STRU signaling;
}L2CAP_DATA_PAYLOAD_UNION;

typedef struct
{
    HCI_DATA_HDR_STRU hciDataHdr;
    L2CAP_DATA_HDR_STRU l2capDataHdr;
    L2CAP_DATA_PAYLOAD_UNION l2capPayload;
}L2CAP_RX_HCI_DATA_STRU;

#define HCIPKT_HEADER_LEN		sizeof(HCI_DATA_HDR_STRU)

#define L2CAP_HEADER_LEN		sizeof(L2CAP_DATA_HDR_STRU)
#define L2CAP_SIG_HEADER_LEN	sizeof(L2CAP_SIGNALING_HEADER_STRU)

#define HCI_PB_MASK					(0x03 << 4)
#define HCI_PB_L2CAP_START			(0x02 << 4)
#define HCI_PB_L2CAP_CONTINUE		(0x01 << 4)
#define HCI_BC_MASK					(0x03 << 6)
#define HCI_BC_ASB					(0x01 << 6)
#define HCI_BC_PSB					(0x02 << 6)	

#define HCI_FIELD_PB_L2CAP_START	(0x02)


// Constant;
#define L2CAP_ROM_SIGNALING_CID			0x0001
#define L2CAP_ROM_CONNECTIONLESS_CID	0x0002

#define	L2CAP_ROM_LE_ATT_CID			0x0004
#define	L2CAP_ROM_LE_SIGNALLING_CID		0x0005
#define	L2CAP_ROM_LE_SMP_CID			0x0006


#endif
#endif
