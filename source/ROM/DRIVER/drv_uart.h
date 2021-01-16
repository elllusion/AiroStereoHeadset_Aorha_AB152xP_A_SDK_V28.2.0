#ifndef _DRV_UART_H_
#define _DRV_UART_H_
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
#include "sector_uart_parameter.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- UART_LINE_CTL_UNION constant
#define UART_5_BIT_WORD_LENGTH  0
#define UART_6_BIT_WORD_LENGTH  1
#define UART_7_BIT_WORD_LENGTH  2
#define UART_8_BIT_WORD_LENGTH  3

#define UART_1_STOP_BIT         0
#define UART_2_STOP_BITS        1

#define UART_PARITY_OFF         0
#define UART_PARITY_ON          1

#define UART_ODD_PARITY         0
#define UART_EVEN_PARITY        1

#define UART_STICK_PARAITY_OFF  0
#define UART_STICK_PARAITY_ON   1

#define UART_HW_FLOW_CTL_OFF    0
#define UART_HW_FLOW_CTL_ON     1

//-
typedef U8 UART_HDLR_ID;
#define UART_HDLR_ID_TX_CPL_CALLBACK    ((UART_HDLR_ID)0)
#define UART_HDLR_ID_RX_INTR_CALLBACK   ((UART_HDLR_ID)1)
#define UART_HDLR_ID_BREAK_INTR         ((UART_HDLR_ID)2)
#define UART_HDLR_ID_FRAME_ERR          ((UART_HDLR_ID)3)
#define UART_HDLR_ID_PARITY_ERR         ((UART_HDLR_ID)4)
#define UART_HDLR_ID_OVER_RUN           ((UART_HDLR_ID)5)



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef VOID (*fDRV_UART_HDLR)(VOID);



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN BOOL DRV_UART_IsDmaBusy(VOID);

EXTERN VOID DRV_UART_SetBaudRate(
    IN      U32 BaudRate);

EXTERN VOID DRV_UART_Init(
    IN      UART_PARAMETER_STRU CODE_PTR pUartPara,
    IN      U8 XDATA_PTR pRxBuf,
    IN      U16 RxBufSize);

EXTERN VOID DRV_UART_Disable(VOID);

EXTERN VOID DRV_UART_RegisterUartHdlr(
    IN      UART_HDLR_ID HdlrId,
    IN      fDRV_UART_HDLR fCallback);

EXTERN VOID DRV_UART_DmaRxIntrHdlr(VOID);

EXTERN VOID DRV_UART_DmaTxCplIntrHdlr(VOID);

EXTERN VOID DRV_UART_IntrHdlr(VOID);

EXTERN BOOL DRV_UART_DmaTxSender(
    IN      U8 XDATA_PTR ptr,
    IN      U16 len);

EXTERN VOID DRV_UART_DmaRxReader(
    IN      U8 XDATA_PTR ptr);

EXTERN VOID DRV_UART_RxIntrDisable(VOID);
EXTERN VOID DRV_UART_RxIntrEnable(VOID);

#endif /* _DRV_UART_H_ */

