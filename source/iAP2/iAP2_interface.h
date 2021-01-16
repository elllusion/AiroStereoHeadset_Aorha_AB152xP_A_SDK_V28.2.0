
#ifndef __IAP2_INTERFACE_H__
#define __IAP2_INTERFACE_H__

#ifdef __IAP2_C__
#define IAP2_EXTERN
#else
#define IAP2_EXTERN	extern
#endif


#define IAP2_MAX_FRAME_SIZE		240	/* to fit in uart output header structure */

//
// iAP2 Public Function
//

PUBLIC void IAP2_Init (void);

typedef struct
{
	U16 message_start;
	U16 message_length;
	U16 message_id;
} IAP2CtrlSession;

typedef struct
{
	U16 message_length;
	U16 message_id;
} IAP2CtrlMsgHeader;

typedef struct
{
	U16 session_id;
} IAP2ExtASession;

typedef struct
{
	U16 param_length;
	U16 param_id;
} IAP2ParamHeader;

typedef struct
{
	U16 param_length;
	U16 param_id;
	U8 param[1];
} IAP2Param;


PUBLIC U8 XDATA_PTR IAP2_AllocCtrlSessionPacket(U8 linkIndex, U16 message_id, U16 pktLength);
PUBLIC U8 XDATA_PTR IAP2_AllocExtASessionPacket(U8 linkIndex, U16 session_id, U16 pktLength);

PUBLIC void IAP2_SendCtrlSessionPacket(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void IAP2_SendExtASessionPacket(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void IAP2_SendExtASessionFromUART(U8 linkIndex, U8 XDATA_PTR msgPtr);

#endif //__IAP2_INTERFACE_H__
