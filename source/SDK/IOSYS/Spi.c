#include "reside_flash.inc"

#include "rc.h"
#include "drv_spi_flash.h"
#include "..\SDK\include\Spi.h"


U8 SPI_WriteReadBlockingCmd(U8 devID, U16 txLen, U8 XDATA_PTR txBufPtr, U16 rxLen, U8 XDATA_PTR rxBufPtr)
{
	return DVR_SPI_WriteReadBlockingCmd(devID, txLen, txBufPtr, rxLen, rxBufPtr);
}

void SPI_WriteReadNonBlockingCmd(U8 devID, U16 txLen, U8 XDATA_PTR txBufPtr, U16 rxLen, U8 XDATA_PTR rxBufPtr, VFUN callback)
{	
	SPINonBlockingStru XDATA_PTR spiNonBlkParaPtr;
	spiNonBlkParaPtr = SYS_MemoryGet(txLen + sizeof(SPINonBlockingStru) -1);
	if(spiNonBlkParaPtr)
	{
		spiNonBlkParaPtr->para.devID 		= devID;
		spiNonBlkParaPtr->para.callbackFunc = (SPICBFunc)callback;
		spiNonBlkParaPtr->para.txLen 		= txLen;
		spiNonBlkParaPtr->para.txBufPtr 	= &spiNonBlkParaPtr->dataBegin[0];
		OS_memcpy(&spiNonBlkParaPtr->dataBegin[0], txBufPtr, txLen);
		spiNonBlkParaPtr->para.rxLen		= rxLen;
		spiNonBlkParaPtr->para.rxBufPtr		= rxBufPtr;
		DRV_SPI_WriteReadNonBlockingCmd(spiNonBlkParaPtr);
	}
	else
	{
		if(callback)
			((SPICBFunc)callback)(SPI_BLOCKING_CMD_STATUS_NO_MEMORY, 0, NULL);
	}
}