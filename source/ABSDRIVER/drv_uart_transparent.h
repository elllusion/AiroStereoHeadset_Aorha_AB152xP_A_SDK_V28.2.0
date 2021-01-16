#ifndef _DRV_UART_TRANS_H_
#define _DRV_UART_TRANS_H_
#include "hci_xport.h"

PUBLIC BOOL DRV_UART_TRANS_TxData(U16 txLen, U8 XDATA_PTR txDataPtr);
PUBLIC void DRV_UART_TRANS_TxMsg(UART_DMA_SEND_DATA_STRU XDATA_PTR pMsg);

PUBLIC U8 DRV_UART_TRANS_ReadRxBufValue(U16 offset);
PUBLIC void DRV_UART_TRANS_ReadOutFromRxBuf(U8 XDATA_PTR rxPtr, U16 len);
PUBLIC void DRV_UART_TRANS_DropRxBuf(U16 len);
PUBLIC U16 DRV_UART_TRANS_RxEntiesNumber(void);

PUBLIC void DRV_UART_TRANS_Enable(VFUN rx_callback);
PUBLIC void DRV_UART_TRANS_Disable(void);

#endif //_DRV_UART_TRANS_H_