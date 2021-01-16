#ifndef _DRV_SPI_SLAVE_H_
#define _DRV_SPI_SLAVE_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_uart.h is the file defines the function about UART interface
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "os.h"


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SPISLAVE_MAX_TX_BUF     100
#define SPISLAVE_DATA_RDY_PIN   24


typedef U8 SPI_HDLR_ID;
#define SPI_HDLR_ID_TX_CPL_CALLBACK    ((SPI_HDLR_ID)0)
#define SPI_HDLR_ID_RX_INTR_CALLBACK   ((SPI_HDLR_ID)1)


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef VOID (*fDRV_SPI_HDLR)(VOID);



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID DRV_SPISlave_Init(
    IN      U8 XDATA_PTR pRxBuf,
    IN      U16 RxBufSize);


/**
 * Set DMA FIFO slave to transmit data by SPI_slave
 *
 * @author YugoSu <YugoSu@airoha.com.tw>
 * @param[in]   ptr     The pointer of data stream
 * @param[in]   len     The length of data stream
 * @returns             Boolean value stands for whether sending successfully
 */
EXTERN BOOL DRV_SPISlave_DmaTXSender(
    IN      U8 XDATA_PTR ptr,
    IN      U16 len);



/**
 * DRV_UART_DmaRxReader
 *
 * Notify DMA where SW read out
 *
 * @author Charles Su <CharlesSu@airoha.com.tw>
 *
 * @param[in]   ptr     The next read pointer
 */
EXTERN VOID DRV_SPISlave_DmaRxReader(
    IN      U8 XDATA_PTR ptr);



/**
 * DRV_SPISlave_IsDmaBusy
 *
 * Check whether the DMA is busy
 *
 * @author Yugo Su <YugoSu@airoha.com.tw>
 *
 * @returns     Boolean value stands for whether DMA is busy
 */
EXTERN BOOL DRV_SPISlave_IsDmaBusy(VOID);


/**
 * DRV_SPI_Slave_IntrHdlr
 *
 * SPI Slave Handler
 *
 * @author Yugo Su <YugoSu@airoha.com.tw>
 */
EXTERN VOID DRV_SPISlave_IntrHdlr(VOID);


/**
 * DRV_SPISlave_Disable
 *
 * Disable SPI transport
 *
 * @author Yugo Su <YugoSu@airoha.com.tw>
 */
EXTERN VOID DRV_SPISlave_Disable(VOID);

/**
 * DRV_SPISlave_RegisterSPIHdlr
 *
 * A function is used to register SPI handlers
 *
 * @author Yugo Su <YugoSu@airoha.com.tw>
 * @param[in] fCallback The function entry of callback function
 */
EXTERN VOID DRV_SPISlave_RegisterSPIHdlr(
    IN      SPI_HDLR_ID HdlrId,
    IN      fDRV_SPI_HDLR fCallback);

#endif /* _DRV_SPI_SLAVE_H_*/

