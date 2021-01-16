#ifndef _DRV_SPI_FLASH_H_
#define _DRV_SPI_FLASH_H_

#include "config.h"

#if (HW_SPI_SUPPORTED)
#include "cpu.h"
#include "drv_spi.h"
#include "sector_spi_config.h"


/******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef void (*SPICBFunc)(U8 status, U16 rxLen, U8 XDATA_PTR rxBufPtr);

typedef struct
{
	U8 devID;
	SPICBFunc callbackFunc;
	U16 txLen;
	U8 XDATA_PTR txBufPtr;
	U16 rxLen;
	U8 XDATA_PTR rxBufPtr;
}SPINonBlockingInfoType;

typedef struct
{
	SPINonBlockingInfoType para;
	U8 dataBegin[1];
}SPINonBlockingStru;

typedef struct
{
	U8 currGrpSel:5;
	U8 isManual:1;
	U8 isGrpConfiged:1;
	U8 isBlockOwned:1;
	OSMQ nonBlockingQue;
	SPINonBlockingStru XDATA_PTR pCurrSPIMsg;
	SPI_DEVICE_CONFIG CODE_PTR pConfig;
}SPI_CTL_STRU;

////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//EXTERN VOID DRV_SPI_BusInitial_Flash(VOID);
EXTERN VOID DRV_SPI_MasterInterruptHandler(VOID);
EXTERN VOID DRV_SPI_Init(VOID);
EXTERN U8 DVR_SPI_WriteReadBlockingCmd(U8 devID, U16 txLen, U8 XDATA_PTR txBufPtr, U16 rxLen, U8 XDATA_PTR rxBufPtr);
EXTERN void DRV_SPI_WriteReadNonBlockingCmd(SPINonBlockingStru XDATA_PTR spiNonBlkParaPtr);

EXTERN VOID DRV_SPI_FLASHChipUnlock(VOID);
EXTERN VOID DRV_SPI_FLASHChipLock (VOID);


#endif


#endif /* _DRV_SPI_FLASH_H_ */





