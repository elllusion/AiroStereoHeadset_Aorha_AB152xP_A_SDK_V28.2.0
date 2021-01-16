#ifndef _HID_INTERFACE_H_
#define _HID_INTERFACE_H_

#ifdef _HID_C_
#define HID_EXTERN
#else
#define HID_EXTERN	extern
#endif

#include "os.h"
#include "..\ROM\HC\hc.h"
#include "..\L2CAP\L2CAP_interface.h"
#include "..\MMI\MMI_PSM.h"

#define HID_REMOTE_CMD_HDR_NUM                  (0x0C)

#define HID_MTU_SIZE		                    (0xFF)

#define HID_VIRTUAL_UNPLUGGED                   (FALSE)
#define HID_VIRTUAL_PLUGGED                     (TRUE)

#define HID_SEND_ON_INTTERUPT_CHANNEL           (FALSE)
#define HID_SEND_ON_CONTROL_CHANNEL             (TRUE)

#define HID_PROTOCOL_MODE_REPORT                (0x01)
#define HID_PROTOCOL_MODE_BOOT                  (0x00)

#define HID_KB_BOOT_MODE_REPORT_ID              (0x01)
#define HID_MOUSE_BOOT_MODE_REPORT_ID           (0x02)

#define HID_MOUSE_BOOT_MODE_DATA_LEN            (0x05)
#define HID_KB_BOOT_MODE_DATA_LEN               (0x0A)

#define HID_L2CAP_HDR_LEN                       (0x04)
#define HID_THDR_LEN                            (0x01)
#define HID_THDR_TRANSACTION_TYPE_MASK          (0xF0)
#define HID_THDR_PARAMETER_MASK                 (0x0F)
#define HID_THDR_PARAMETER_REPORT_TYPE_MASK     (0x03)
#define HID_THDR_PARAMETER_REPORT_SIZE_MASK     (0x08)
#define HID_REPORT_ID_LEN                       (0x01)

#define HID_THDR_REPORT_TYPE_INPUT              (0x01)
#define HID_THDR_REPORT_TYPE_OUTPUT             (0x02)
#define HID_THDR_REPORT_TYPE_FEATURE            (0x03)

#define HID_RECV_CONT_NON        			  	(0x00)
#define HID_RECV_CONT_SETREPORT     			(0x01)
#define HID_RECV_CONT_DATA        				(0x02)

#define HID_REMOTE_EVENT_TRANTYPE_OFFSET(ptr)           (*ptr + L2CAP_HEADER_LEN)
#define HID_REMOTE_EVENT_REPORT_ID_OFFSET(ptr)          (*ptr + L2CAP_HEADER_LEN + 1)
#define HID_REMOTE_EVENT_BUFFERSIZE_OFFSET(ptr)         (*ptr + L2CAP_HEADER_LEN + 2)
#define HID_REMOTE_EVENT_CURRENT_IDLE_RATE_OFFSET(ptr)  (*ptr + L2CAP_HEADER_LEN + 1)

#define HID_GETREPORT_NOBUFFERSIZE						(0x00)
#define HID_GETREPORT_WITHBUFFERSIZE					(0x08)

#define HID_REPORT_ID_KB                                (0x01)
#define HID_REPORT_ID_MOUSE                             (0x02)

//related HID command  B
#define HID_CMD_MOUSE_REPORT							(0xC0 + 0x01)
#define HID_CMD_MOTION_DETECT_REPORT_001				(0xC0 + 0x02)
#define HID_CMD_MOTION_DETECT_REPORT_002				(0xC0 + 0x03)
#define HID_CMD_WHEEL									(0xC0 + 0x04)
#define HID_CMD_LOCAL_UNPLUG							(0xC0 + 0x05)
#define HID_CMD_CREATE_CHANNEL							(0xC0 + 0x06)
#define HID_CMD_DISCONNECT_CHANNEL						(0xC0 + 0x07)
#define HID_CMD_CONFIG_L2CAP_CHANNEL_COMPLETE			(0xC0 + 0x08)
#define HID_CMD_NUMBER_COMPLETE_PACKET					(0xC0 + 0x09)
#define HID_CMD_SEND_HID_DATA_SIGNAL					(0xC0 + 0x0A)
//related HID command  E

//related HID Event  B
#define HID_EVT_LOCAL_UNPLUG_COMPLETE					(0xC0 + 0x05)
#define HID_EVT_REMOTE_UNPLUG							(0xC0 + 0x06)
#define HID_EVT_REMOTE_HARD_RESET						(0xC0 + 0x07)
#define HID_EVT_REMOTE_SOFT_RESET						(0xC0 + 0x08)
#define HID_EVT_REMOTE_SUSPEND							(0xC0 + 0x09)
#define HID_EVT_REMOTE_EXIT_SUSPEND						(0xC0 + 0x0A)
//related HID Event  E

//related HID Remote Event  B
#define HID_REMOTE_EVENT_HANDSHAKE						(0x00)
#define HID_REMOTE_EVENT_HID_CONTROL					(0x10)
#define HID_REMOTE_EVENT_GET_REPORT		   				(0x40)
#define HID_REMOTE_EVENT_SET_REPORT						(0x50)
#define HID_REMOTE_EVENT_GET_PROTOCOL					(0x60)
#define HID_REMOTE_EVENT_SET_PROTOCOL					(0x70)
#define HID_REMOTE_EVENT_GET_IDLE						(0x80)
#define HID_REMOTE_EVENT_SET_IDLE						(0x90)
#define HID_REMOTE_EVENT_DATA							(0xA0)
#define HID_REMOTE_EVENT_DATC							(0xB0)

//HID Command Para B
#define HID_DATA_TO_L2CAP_OFFSET					    (0x08)
#define HID_DATA_TO_L2CAP_PARA_LENL_OFFSET			    (HID_DATA_TO_L2CAP_OFFSET)
#define HID_DATA_TO_L2CAP_PARA_LENH_OFFSET			    (HID_DATA_TO_L2CAP_OFFSET + 1)
#define HID_DATA_TO_L2CAP_TRANTYPE_OFFSET			    (HID_DATA_TO_L2CAP_OFFSET + 4)

#define HID_MOUSE_BUTTON_BOOT_POSITION                  (14)
#define HID_MOUSE_XMOVE_BOOT_POSITION                   (15)
#define HID_MOUSE_YMOVE_BOOT_POSITION                   (16)
#define HID_MOUSE_WHEEL_BOOT_POSITION                   (17)

#define HID_MOUSE_BUTTONL_FROM_PM_POSITION         		(14)
#define HID_MOUSE_XMOVEH_FROM_PM_POSITION          		(15)
#define HID_MOUSE_XMOVEL_FROM_PM_POSITION          		(16)
#define HID_MOUSE_YMOVEH_FROM_PM_POSITION          		(17)
#define HID_MOUSE_YMOVEL_FROM_PM_POSITION          		(18)
#define HID_MOUSE_WHEEL_FROM_PM_POSITION           		(19)
#define HID_MOUSE_TILTWHEEL_FROM_PM_POSITION           	(20)

#define HID_MOUSE_BUTTONL_SENDL2CAP_POSITION         	(14)
#define HID_MOUSE_XMOVEL_SENDL2CAP_POSITION          	(15)
#define HID_MOUSE_XMOVEH_SENDL2CAP_POSITION          	(16)
#define HID_MOUSE_YMOVEL_SENDL2CAP_POSITION          	(17)
#define HID_MOUSE_YMOVEH_SENDL2CAP_POSITION          	(18)
#define HID_MOUSE_WHEEL_SENDL2CAP_POSITION           	(19)
#define HID_MOUSE_TILTWHEEL_SENDL2CAP_POSITION          (20)

#define HID_MOUSE_BUTTONL_AVAGO7550_POSITION            (14)
#define HID_MOUSE_BUTTONH_AVAGO7550_POSITION            (15)
#define HID_MOUSE_XMOVEL_AVAGO7550_POSITION          	(16)
#define HID_MOUSE_XMOVEHYMOVEL_AVAGO7550_POSITION    	(17)
#define HID_MOUSE_YMOVEH_AVAGO7550_POSITION          	(18)
#define HID_MOUSE_WHEEL_AVAGO7550_POSITION           	(19)

#define HID_TRANTYPE_HANDSHAKE_SUCCESSFUL				(0x00)
#define HID_TRANTYPE_HANDSHAKE_NOT_READY				(0x01)
#define HID_TRANTYPE_HANDSHAKE_ERR_INVALID_REPORT_ID	(0x02)
#define HID_TRANTYPE_HANDSHAKE_ERR_UNSUPPORTED_REQUEST	(0x03)
#define HID_TRANTYPE_HANDSHAKE_ERR_INVALID_PARAMETER 	(0x04)
#define HID_TRANTYPE_HANDSHAKE_ERR_UNKNOWN				(0x0E)
#define HID_TRANTYPE_HANDSHAKE_ERR_FATAL				(0x0F)

#define HID_TRANTYPE_HIDCONTROL_VIRTUALCABLEUNPLUG		(0x15)

#define HID_TRANTYPE_DATA_OTHER     				    (0xA0)
#define HID_TRANTYPE_DATA_INPUT 				        (0xA1)
#define HID_TRANTYPE_DATA_OUTPUT   				        (0xA2)
#define HID_TRANTYPE_DATA_FEATURE  				        (0xA3)

#define HID_TRANTYPE_DATC       				        (0xB0)
#define HID_TRANTYPE_DATC_OTHER 				        (0xB0)
#define HID_TRANTYPE_DATC_INPUT 				        (0xB1)
#define HID_TRANTYPE_DATC_OUTPUT 				        (0xB2)
#define HID_TRANTYPE_DATC_FEATURE 				        (0xB3)

//HID1.1 DEPRECATED
//#define HID_CONTROL_NOP									(0x00)
//#define HID_CONTROL_HARD_RESET							(0x01)
//#define HID_CONTROL_SOFT_RESET							(0x02)
#define HID_CONTROL_SUSPEND								(0x03)
#define HID_CONTROL_EXIT_SUSPEND						(0x04)
#define HID_CONTROL_VIRTUAL_CABLE_UNPLUG				(0x05)

#define HID_CONTROL_CHANNEL                             (0x00)
#define	HID_INTERRUPT_CHANNEL                           (0x01)

#define HID_SEND_HID_DATA_SIGNAL_STOP					(0x00)
#define HID_SEND_HID_DATA_SIGNAL_GO						(0x01)

#define HID_CHANNEL_STATE_DISCONNECT				    (0x00)
#define HID_CHANNEL_STATE_INTERRUPT_DISCONNECT          (0x01)
#define HID_CHANNEL_STATE_OPEN						    (0x02)

#define HID_SWAP_XY                                     (0)
#define HID_X_NEGATE                                    (0)
#define HID_Y_NEGATE                                    (0)

enum
{
    HID_CONNECTION_COMPLETE_EVENT = 0x01,
	HID_DISCONNECTION_COMPLETE_EVENT,	
};

enum
{
    HID_STATE_IDLE,
    HID_STATE_WAIT_CTL_CHANNEL_CONN_CMP,
    HID_STATE_WAIT_INT_CHANNEL_CONN_CMP,
    HID_STATE_CONNECTED,
    HID_STATE_WAIT_INT_CHANNEL_DISCONN_CMP,
    HID_STATE_WAIT_CTL_CHANNEL_DISCONN_CMP,
};

typedef union 
{
	S8 deltaxyU8[2];
	S16 deltaxyU16;
}DELTA_XY;

typedef union
{
	U8 sizeU8[2];
	U16 sizeU16;
}DATA_PTR_SIZE;

typedef union
{
	U8 sizeU8[2];
	U16 sizeU16;
}SET_REPORT_CONT_DATA_PTR_SIZE;

typedef struct
{     
    U8 reportId;
    U16 bufferSize;
}HID_REPORT_PARA;

typedef struct
{
    U8 reportId;
    U8 modifierKeys;
    U8 rsd;
    U8 keyCode1;
    U8 keyCode2;
    U8 keyCode3;
    U8 keyCode4;
    U8 keyCode5;
    U8 keyCode6;    
}HID_KB_DATA_BOOT_MODE;

typedef struct
{
    U8 reportId;
    U8 button;
    U8 xMove;
    U8 yMove;
    U8 deviceSpeific;
}HID_MOUSE_DATA_BOOT_MODE;

typedef union
{
    HID_REPORT_PARA reportPara;
    HID_KB_DATA_BOOT_MODE kbDataBootMode;
    HID_MOUSE_DATA_BOOT_MODE mouseDataBootMode;
    U8 idleRate;
    U8 dataPayload;
}HID_PARA;

typedef struct
{
    U8 offset;
    U8 rsvd[1];
    HCI_DATA_HDR_STRU hciHdr;
    L2CAP_DATA_HDR_STRU l2capHdr;
    U8 request;
    HID_PARA para;
}HID_DATA_STRU;

typedef struct
{
    U8 offset;
    U8 rsvd[3];
    HCI_DATA_HDR_STRU hciHdr;
    L2CAP_DATA_HDR_STRU l2capHdr;
    U8 request;
    HID_PARA para;
}HID_REPORT_DATA_STRU;

typedef struct
{
    U8 hidState;
    U8 curChannelIdx;
    U8 curIdleRate;
    U8 curProtocolMode;
    U8 curReportType;

    U8 dataCont;
    U8 setReportCont;

    HID_DATA_STRU XDATA_PTR dataPtr;
	HID_DATA_STRU XDATA_PTR setReportContDataPtr;
    U8 XDATA_PTR setReportContDataCurPtr;
    U8 XDATA_PTR dataContDataCurPtr;

    DATA_PTR_SIZE dataPtrSize;
    SET_REPORT_CONT_DATA_PTR_SIZE setReportContDataPtrSize;    
} HIDCtlType;

typedef struct
{
    MMI_PROFILE_INFO            mmiProfileInfo;
	U8							channelState;
	L2CAP_CHANNEL_HDL			channelHDLIndex[2];
	U16							out_MTU;
	U16							in_MTU;
} HIDChannelCtlType;

typedef BOOL (*HID_REMOTE_CMD_HDLR)(L2CAP_CHANNEL_HDL l2capChannelHdl, HID_DATA_STRU XDATA_PTR dataPtr);
typedef BOOL (*HID_HANDLER)(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr);

HID_EXTERN HIDCtlType XDATA gHID_ctl[MAX_MULTI_POINT_NO];
HID_EXTERN HIDChannelCtlType XDATA gHID_channel_ctl[MAX_MULTI_POINT_NO];
HID_EXTERN HID_HANDLER XDATA gHID_VendorCB;

PUBLIC L2CAP_CHANNEL_HDL HID_ProfileConnectionRequest(U8 linkIndex);
PUBLIC BOOL HID_ProfileDisconnectionRequest(U8 linkIndex);
PUBLIC VOID HID_DataReport(U8 linkIndex, HID_REPORT_DATA_STRU XDATA_PTR dataPtr, BOOL bCtlChannel);
PUBLIC VOID HID_LocalUnplug(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void HID_SetCmdHead(HID_REPORT_DATA_STRU XDATA_PTR dataPtr, U8 payloadLen);
PUBLIC void HID_SetCmdHeadFromRemoteEvent(HID_REPORT_DATA_STRU XDATA_PTR dataPtr , U8 commandOp , U8 payloadLen);
PUBLIC VOID HID_AclDisconnected(U8 linkIndex);
PUBLIC BOOL HID_IsVirtualPlugged(U8 linkIndex);
PUBLIC BOOL HID_IsEnable(VOID);
PUBLIC VOID HID_SetEnable(BOOL enabled);

#endif
