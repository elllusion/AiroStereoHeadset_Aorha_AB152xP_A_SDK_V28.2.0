#ifndef _HCI_XPORT_H_
#define _HCI_XPORT_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hc_xport.h is the file defines the UART/SPI transport interface.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#include "hci.h"
#include "drv_spi.h"


typedef struct
{
	U8 Pid;
	U8 dataStart[1];
}UART_TX_H4_STRU;

typedef struct
{
	U8 dataStart[1];
}UART_TX_H5_STRU;

typedef union
{
	U8 DmaTxStart[1];
	UART_TX_H4_STRU h4TxMsg;
	UART_TX_H5_STRU h5TxMsg;
}UART_TX_STRU;

typedef struct
{
	U16 Len;
	UART_TX_STRU DmatxMsg;
}UART_DMA_SEND_DATA_STRU;

#ifdef UART_H5
#include "hci_h5.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * UART TRANSPORT CONTROL
 */
typedef struct stru_hc_tx_xport_ctl
{
    VOID (*fHciXportTxPktProcessor)(U8 isAllQue);
    OSMQ PktQueue;
    OSMQ HighPriorityPktQueue;
    OSMQ TxPktHandledQueue;
    U8 XDATA_PTR pCurrTxMsg;    /* Also stands for BUSY if not NULL */
} HC_TX_XPORT_CTL_STRU;

typedef struct stru_hc_rx_xport_ctl
{
    VOID (*fHciParser)(VOID);
    VOID (*fHciXportRxPktProcessor)(VOID);
    VOID (*fHciFetch)(VOID);
    U8 XDATA_PTR pMsg;
    U8 XDATA_PTR pCurrPtr;
    U16 RecidualBytes;

    union
    {
        HCI_DATA_HDR_STRU   HciDataHdr;
        HCI_CMD_HDR_STRU    HciCmdHdr;
    } Overlay;

} HC_RX_XPORT_CTL_STRU;

typedef struct stru_hc_xport_ctl
{
    HC_TX_XPORT_CTL_STRU Tx;
    HC_RX_XPORT_CTL_STRU Rx;
    #ifdef UART_H5
    HC_THREE_WIRE_CTL	ThreeWireCtl;
    #endif
} HC_XPORT_CTL_STRU;

typedef struct stru_hci_xport
{
	VOID (*fTransmitter)(VOID);
	VOID (*fReceiver)(VOID);
} HCI_XPORT_STRU;

////////////////////////////////////////////////////////////////////////////////
// External Global Variable ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN HC_XPORT_CTL_STRU XDATA _gHcXportCtl;
EXTERN BIT _gHciXportErrDetected;
EXTERN HCI_XPORT_STRU XDATA HCI_XPORT;

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HCI_XPORT_Init
 *
 * Init HCI transport layer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_Init(VOID);

/**
 * HCI_XPORT_TxDmaSend
 *
 * Directly send HCI packets by transport DMA.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_TxDmaSend(VOID) compact REENTRANT;

/**
 * HCI_XPORT_TxDmaDone
 *
 * Execute if DMA completed transmission.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_TxDmaDone(VOID);

/**
 * HCI_XPORT_TxCplCallBack
 *
 * Function callback when DMA completed transmission.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_TxCplCallBack(VOID);

/**
 * HCI_XPORT_Transmitter
 *
 * A function is used to wrapped HCI packets to HCI.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_Transmitter(VOID);

/**
 * HCI_XPORT_Receiver
 *
 * A function is used to wrapped HCI packets from HCI.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_Receiver(VOID);

/**
 * HCI_XPORT_RxIntrCallBack
 *
 * Function callback when Rx done.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_RxIntrCallBack(VOID);

/**
 * HCI_XPORT_TxDmaEnqueue
 *
 * A function is used to enqueue HCI packet to TX DMA queue.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @pMsg : The message pointer.
 * @Pid : The HCI packet ID over UART transport.
 */
EXTERN VOID HCI_XPORT_TxDmaEnqueue(
    IN      U8 XDATA_PTR pMsg,
    IN      HCI_UART_XPORT_PKT_ID Pid);


/**
 * H4_XPORT_HciCmdHdrParser
 *
 * Parse the HCI Command from UART RX Buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID H4_XPORT_HciCmdHdrParser(VOID);

EXTERN VOID H4_XPORT_HciCmdParaFetch(VOID);

/**
 * H4_XPORT_HciAclDataHdrParser
 *
 * Parse the HCI Data from UART RX Buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID H4_XPORT_HciAclDataHdrParser(VOID);

/**
 * H4_XPORT_HciAclDataFetch
 *
 * Parse the HCI Continuing Data from UART RX Buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID H4_XPORT_HciAclDataFetch(VOID);

/**
 * DRV_UART_DmaSendAll
 *
 * Force to send output all message in queue.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_SendAllMsgOut(VOID);

/**
 * HCI_XPORT_Disable
 *
 * Disable Transport
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_XPORT_Disable(VOID);

/**
 * HCI_XPORT_ReadTxBufDepth
 *
 * Read TX buffer depth
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN U8 HCI_XPORT_ReadTxBufDepth(VOID);


EXTERN U8 HCI_XPORT_ReadTxHighPriorityBufDepth(VOID);

EXTERN VOID HCI_XPORT_InsertEvtToUARTTxTableDirectly (U8 XDATA_PTR ptr1);

#if (HW_SPI_SUPPORTED)
EXTERN VOID HCI_XPORT_InsertEvtToSPIDirectly (U8 XDATA_PTR ptr1);
#endif

EXTERN VOID HCI_XPORT_TxProcessor(U8 isAllQue);

EXTERN VOID HCI_XPORT_H4Receiver(VOID);

EXTERN VOID HC_SendHCIDataToQue(U16 ConnHdl, U8 XDATA_PTR msgPtr);

EXTERN U8 HC_XPORT_GetTxQueueEntries(VOID);
#endif /* _HCI_XPORT_H_ */

