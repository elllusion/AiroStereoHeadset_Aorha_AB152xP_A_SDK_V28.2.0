#ifndef __L2CAP_LOW_ENERGY_H__
#define __L2CAP_LOW_ENERGY_H__

#include "L2CAP_interface.h"
#ifdef LE_SUPPORTED_HOST_FLASH

#ifdef _L2CAP_LOW_ENERGY_C_
#define L2CAP_LE_EXTERN
#else
#define L2CAP_LE_EXTERN extern
#endif

#define	L2CAP_LE_ATT_CID				0x0004
#define	L2CAP_LE_SIGNALLING_CID			0x0005
#define	L2CAP_LE_SMP_CID				0x0006

#define L2CAP_HCI_CONNHDL_MASK			0x0FFF
#define L2CAP_HCI_FLAG_MASK				0xF000
#define L2CAP_HCI_CONNHDL_VALUE(x)		((x) & L2CAP_HCI_CONNHDL_MASK)
#define L2CAP_HCI_FLAG_BYTE_VALUE(x)	(((x) & L2CAP_HCI_FLAG_MASK) >> 8)

typedef struct
{
	U16 intervalMin;
	U16 intervalMax;
	U16 slaveLatency;
	U16 timeout;
}L2CAP_CONN_PARA_REQ_TYPE;

typedef union
{
    L2CAP_CONN_PARA_REQ_TYPE connParaReq; 
}L2CAP_LE_MSG_TYPE;

typedef struct
{
	U8 offset;
	U8 reserved[4];
	U16 hciHdr;
	U16 hciLen;
	U16 l2capLen;
	U16 cid;
} L2CAP_RSP_DATA_HDR_STRU;

typedef struct
{
	L2CAP_RSP_DATA_HDR_STRU hdr;
	U8 opCode;
	U8 identifier;
	U16 length;
    L2CAP_LE_MSG_TYPE msgBody; 
}LE_SIGNALLING_PACKET_TYPE;

PUBLIC void L2CAP_LE_ClearSession(U8 linkIndex);
PUBLIC void L2CAP_LE_SetupSession(U8 linkIndex, U16 aclHandle);
PUBLIC void UpperLayer_SendL2capConnParaUpdateRequest(U8 linkIndex, U8 XDATA_PTR dataPtr);

PUBLIC void L2CAP_LE_DispatchChannelDataToATT(U8 XDATA_PTR dataPtr);
PUBLIC void L2CAP_LE_DispatchChannelDataToSMP(U8 XDATA_PTR dataPtr);

PUBLIC void L2CAP_LE_ResourceManager(void);

L2CAP_LE_EXTERN L2CAP_DATA_CTL XDATA gL2CAP_le_data_ctl[MAX_NUM_LE_LINK];
L2CAP_LE_EXTERN L2CAP_RECOMBINE_CTL XDATA gL2CAP_le_recombine_ctl[MAX_NUM_LE_LINK];

#ifdef SM_SUPPORTED
L2CAP_LE_EXTERN OSMQ XDATA OSMQ_L2CAP_Data_To_SM;
L2CAP_LE_EXTERN OSMQ XDATA OSMQ_L2CAP_Data_From_SM;

#define OSMQ_L2CAP_Data_To_SM_ptr &OSMQ_L2CAP_Data_To_SM
#define OSMQ_L2CAP_Data_From_SM_ptr &OSMQ_L2CAP_Data_From_SM
#endif /* SM_SUPPORTED */
#endif /* LE_SUPPORTED_HOST_FLASH */

#endif //__L2CAP_LOW_ENERGY_H__
