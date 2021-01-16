#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "drv_uart_transparent.h"


#define UART_HW_TRANSPARENT_MODE	0

typedef struct
{
	U16 Len;
	U8 txData[1];
}SDK_UART_TX_BUF_STRU;


/*  SDK_Uart_Hal_TxData: Tx data. This function will allocate a new memory and copy all the data to memory and then send to uart tx.
 *						 There is chance of fail in getting memory.
 *	<input1> 
 *		U16 txLen				:length of tx data in byte
 *		U8 XDATA_PTR txDataPtr	:tx data memory address.
 *	<output> 
 *		BOOL : status of procedure
 *		value: TRUE --> OK, FAIL --> fail
 */
PUBLIC BOOL SDK_Uart_Hal_TxData(U16 txLen, U8 XDATA_PTR txDataPtr)
{
	return DRV_UART_TRANS_TxData(txLen, txDataPtr);
}


/*  SDK_Uart_Hal_TxMemory: Tx data. 
 *	<input1> 
 *		SDK_UART_TX_BUF_STRU XDATA_PTR pMsg : pMsg should be a memory address which should be get from system memory list. After tx this memory would be put back to system
 *		<note> SDK_UART_TX_BUF_STRU
 *				U16 Len			:length of tx data in byte
 *				U8 txData[1]	:tx data starts from here.
 */
PUBLIC void SDK_Uart_Hal_TxMemory(SDK_UART_TX_BUF_STRU XDATA_PTR pMsg)
{
	DRV_UART_TRANS_TxMsg((UART_DMA_SEND_DATA_STRU XDATA_PTR)pMsg);
}

/*  SDK_Uart_Hal_RxEntiesNumber: rx data length
 *	<output> 
 *		U16  	: rx data length
 * 		value range	: 1 - 1000
 */
PUBLIC U16 SDK_Uart_Hal_RxEntiesNumber(void)
{
	return DRV_UART_TRANS_RxEntiesNumber();
}


/*  SDK_Uart_Hal_DropRxBuf: clear rx buffer without read
 *	<input1> 
 *		U16 len 	: data length to clear
 * 		value range	: 1 - SDK_Uart_Hal_RxEntiesNumber()
 */
PUBLIC void SDK_Uart_Hal_DropRxBuf(U16 len)
{
	DRV_UART_TRANS_DropRxBuf(len);
}

/*  SDK_Uart_Hal_ReadAndClearRxBuf: Read rx buffer value and clear buffer
 *	<input1> 
 *		U8 XDATA_PTR rxPtr 	: target address
 * 		value range	: memory pointer
 *	<input2> 
 *		U16 len		: read data length
 * 		value range	: 1 - SDK_Uart_Hal_RxEntiesNumber
 */
PUBLIC void SDK_Uart_Hal_ReadAndClearRxBuf(U8 XDATA_PTR rxPtr, U16 len)
{
	DRV_UART_TRANS_ReadOutFromRxBuf(rxPtr, len);
}

/*  SDK_Uart_Hal_ReadRxValue: Read rx buffer value without clear buffer
 *  <output>
 *		U8		: U8 rxBuff[offset]
 */
PUBLIC U8 SDK_Uart_Hal_ReadRxValue(U16 offset)
{
	return DRV_UART_TRANS_ReadRxBufValue(offset);
}

/*  SDK_Uart_Hal_TransModeRx: Uart transparent mode rx handle
 *  <note>
 * 	    If transparent mode is enabled, rx function should handle all data in rx buffer.
 *		This is polling function.
 */
PRIVATE void SDK_Uart_Hal_TransModeRx(void)
{
	if(SDK_Uart_Hal_RxEntiesNumber())
	{
		SDK_Uart_Hal_DropRxBuf(SDK_Uart_Hal_RxEntiesNumber());
	}
}

/*  SDK_Uart_Hal_EnableTransparentMode: Enable uart transparent mode
 */
PUBLIC void SDK_Uart_Hal_EnableTransparentMode(void)
{
	DRV_UART_TRANS_Enable(SDK_Uart_Hal_TransModeRx);
}

/*  SDK_Uart_Hal_DisableTransparentMode: Disable uart transparent mode
 */
PUBLIC void SDK_Uart_Hal_DisableTransparentMode(void)
{
	DRV_UART_TRANS_Disable();
}

PUBLIC void SDK_Uart_HAL_Init(void)
{
#if UART_HW_TRANSPARENT_MODE
	SDK_Uart_Hal_EnableTransparentMode();
#endif
}