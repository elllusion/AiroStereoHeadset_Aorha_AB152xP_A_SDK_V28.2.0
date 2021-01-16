#ifndef _DRV_XPORT_H_
#define _DRV_XPORT_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_xport.h is the file defines the function about UART/SPI interface
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "sector_uart_parameter.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 DRV_XPORT_SEL;
#define DRV_XPORT_SEL_UART  ((DRV_XPORT_SEL)(0))
#define DRV_XPORT_SEL_SPI   ((DRV_XPORT_SEL)(1))



////////////////////////////////////////////////////////////////////////////////
// Type Defintions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_drv_xport
{
    BOOL (*fDmaTxSender)(
        IN      U8 XDATA_PTR ptr,
        IN      U16 len);

    VOID (*fDmaRxReader)(
        IN      U8 XDATA_PTR ptr);

    BOOL (*fCheckDmaBusy)(VOID);

    VOID (*fXportDisable)(VOID);

} DRV_XPORT_STRU;



////////////////////////////////////////////////////////////////////////////////
// Variables ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN DRV_XPORT_STRU gDrvXport;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_XPORT_Init
 *
 * Initialize the transport functions
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @XportSel : The selection of transport interface
 * @RxDmaBufSize : The size of RX DMA buffer
 * @fBurstTxCallback : The callback function which is executed when DMA TX completed.
 */
EXTERN VOID DRV_XPORT_Init(
    IN      DRV_XPORT_SEL XportSel,
    IN      SIZE RxDmaBufSize,
    IN_OPT  INTR_HDLR fBurstTxCallback,
    IN_OPT  INTR_HDLR fRxIntrCallback);

U8 XDATA_PTR DRV_XPORT_ReadOutFromRxBufWithoutUpdateRingBuffer(
    IN      OS_DATA_PTR ptr,
    IN      U16 Len);	
	
/**
 * DRV_XPORT_ReadRxBuf
 *
 * Read RX buffer according to current read index of UART transport RX buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @HciCmdSize : Offset based on current read index
 * @Return : Byte to read
 */
EXTERN U8 DRV_XPORT_ReadRxBuf(
    IN      U16 Offset);

/**
 * DRV_XPORT_ReadOutFromRxBuf
 *
 * Read data out from UART RX Buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @ptr : Pointer to store the data read from UART RX Buffer
 * @Len : Number of bytes to read
 */
EXTERN VOID DRV_XPORT_ReadOutFromRxBuf(
    IN      OS_DATA_PTR ptr,
    IN      U16 Len);

/**
 * DRV_XPORT_DropRxBuf
 *
 * Drop number of bytes data from UART RX Buffer
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Len : Number of bytes to drop
 */
EXTERN VOID DRV_XPORT_DropRxBuf(
    IN      U16 Len);

/**
 * DRV_XPORT_NumOfRxEnties
 *
 * Get number of received bytes from transport interface
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : Number of received bytes
 */
EXTERN U16 DRV_XPORT_NumOfRxEnties(VOID);

/**
 * DRV_XPORT_NumOfRxEntiesWithEntriesPos
 *
 * Get number of received bytes from transport interface with entries position
 *
 */
EXTERN U16 DRV_XPORT_NumOfRxEntiesWithEntriesPos(U8 XDATA_PTR OS_PTR ppos);

/**
 * DRV_XPORT_UpdateRxBufStatus
 *
 * Update Rx Buffer Status
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_XPORT_UpdateRxBufStatus(
    IN      U16 DmaCurrWrPtr);

#if (FEA_SUPP_SPI_SLAVE)
EXTERN U16 DRV_XPORT_UpdateRxBufWRPtr(
    IN      U16 DmaCurrWrPtr);

#endif

#endif /* _DRV_XPORT_H_ */

