


#define UART_H5_SLIP_SYNC_WORD_SIZE		1
#define UART_H5_HEADER_SIZE				4
#define UART_H5_DIC_CRC_SIZE			2
#define UART_H5_SLIP_SYNC_WORD			0xC0
#define UART_H5_ESCAPE_CODE				0xDB
#define UART_H5_ESCAPE_C0				0xDC
#define UART_H5_ESCAPE_DB				0xDD
#define UART_H5_ESCAPE_11				0xDE
#define UART_H5_ESCAPE_13				0xDF
#define UART_H5_CHECK_SUM_DEFAULT		0xFF


#define UART_CONFIG_RESP_CONFIG_DATA_OFFSET	2


enum
{
	UART_RX_SLIP_IDLE_STATE,
	UART_RX_SLIP_SYNC_STATE,
	UART_RX_SLIP_FRAME_STATE,
	UART_RX_SLIP_ESCAPE_STATE,
	UART_RX_SLIP_FRAME_END_STATE,
};

enum
{
	UART_RX_H5_PACKET_IDLE,
	UART_RX_H5_PACKET_HEADER,
	UART_RX_H5_PACKET_DATA,
	UART_RX_H5_PACKET_CRC,
};

enum
{
	THREE_WIRE_UART_ACK_PKT = 0,
	THREE_WIRE_UART_HCI_CMD,
	THREE_WIRE_UART_ACL_DATA,
	THREE_WIRE_UART_SCO_DATA,
	THREE_WIRE_UART_HCI_EVT,
	THREE_WIRE_UART_VENDOR_PKT = 14,
	THREE_WIRE_UART_LINK_CONTROL = 15,
};

typedef union
{
	U8 bytes[UART_H5_HEADER_SIZE];
	struct {
		U8 seqNo : 3;
		U8 ackNo : 3;
		U8 crc : 1;
		U8 rel : 1;
		U8 pktType : 4;
		U8 pktLenLSB : 4;
		U8 pktLenMSB : 8;
		U8 checksum;
	} field;
}UART_H5_HEADER_TYPE;

typedef struct
{
	U8 bytes[UART_H5_DIC_CRC_SIZE];
}UART_H5_CRC_TYPE;

#define PKT_NO_MODULO		8

#define GET_H5_HDR_SLID_WINDOW(hdr)    ((hdr) & 0x07)
#define GET_H5_HDR_OOF(hdr)            (((hdr) >> 3) & 0x01)
#define GET_H5_HDR_DICTYPE(hdr)        (((hdr) >> 4) & 0x01)
#define GET_H5_HDR_VERSION(hdr)        (((hdr) >> 5) & 0x07)

typedef enum
{
	UART_THREEWIRE_UNINITAILED_STATE,
	UART_THREEWIRE_INITAILED_STATE,
	UART_THREEWIRE_ACTIVE_STATE,
	UART_THREEWIRE_DROP_TX_STATE,
	UART_THREEWIRE_LOW_POWER_STATE,
}UART_THREEWIRE_STATE_TYPE;

#define SYNC_CMD		0x017E
#define SYNC_RESP		0x027D
#define CONFIG_CMD		0x03FC
#define CONFIG_RESP		0x047B
#define WAKE_MSG		0x05FA
#define WOKEN_MSG   	0x06F9
#define SLEEP_MSG		0x0778

typedef struct
{
	UART_H5_HEADER_TYPE pktHeader;
	U16 Len;
	U8 pktdata[1];
}HC_UART_BACKUP_PKT_STRU;

typedef struct
{
	U8 seqNo:3;
	U8 ackNo:3;
}UART_RELIABLE_HEADER_NUM_STRU;

typedef struct
{
	U8 isThreeWireTransportEnabled:1;
	U8 channelState:3;
	U8 isGetConfigCmd:1;
	U8 isGetConfigResp:1;
	U8 isRxPktwaitBuff:1;
	U8 reserved:1;
	U8 ctlerConfigData;
	U8 hostConfigData;
	U8 ThreeWirePacketState;
	U8 ThreeWireSlipState;
	U16 rxRecidualBytes;
	U32 sessionTimeout;
	U32 Tmax;
	OST XDATA_PTR syncMsgTimer;
	OST XDATA_PTR uartWaitingRespTimer;
	OST XDATA_PTR configMsgTimer;
	OST XDATA_PTR sendAckTimer;
	OST XDATA_PTR sendWakeTimer;
	OST XDATA_PTR retransmitTimer;
	OST XDATA_PTR rxPktWaitBuffTimer;
	OSMQ backUpTxPktQueue;
	UART_RELIABLE_HEADER_NUM_STRU reliableHeader;
	UART_H5_HEADER_TYPE txPktHeader;
	UART_H5_HEADER_TYPE rxPktHeader;
	UART_H5_CRC_TYPE rxPktCrc;
	U16 CRCResult;
	U8 XDATA_PTR hciDataPtr;
}HC_THREE_WIRE_CTL;


EXTERN VOID HCI_XPORT_H5Handler(VOID);
EXTERN VOID HC_XPORT_H5OnOff(U8 isEnable);
EXTERN BOOL HC_XPORT_H5EncoderAndSend(UART_DMA_SEND_DATA_STRU XDATA_PTR pMsg);