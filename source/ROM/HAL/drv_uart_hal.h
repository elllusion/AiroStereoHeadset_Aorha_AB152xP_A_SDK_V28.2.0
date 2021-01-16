#ifndef _DRV_UART_HAL_H_
#define _DRV_UART_HAL_H_
#ifndef _DRV_UART_C_
#error "This file shall only be included in drv_uart.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_uart_hal.c is the file defines the hardware abstraction layer of
 * drv_uart.c.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "hw.h"



////////////////////////////////////////////////////////////////////////////////
// Version Control /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !(CHIP_MODEL == 1520)
#error "Unsuitable Driver"
#endif



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* LCR */
#define UART_LCR_5_BIT_WORD_LENGTH                  BIT_FIELD(0,0)
#define UART_LCR_6_BIT_WORD_LENGTH                  BIT_FIELD(1,0)
#define UART_LCR_7_BIT_WORD_LENGTH                  BIT_FIELD(2,0)
#define UART_LCR_8_BIT_WORD_LENGTH                  BIT_FIELD(3,0)
#define UART_LCR_STB_1_STOP_BIT                     0
#define UART_LCR_STB_2_STOP_BIT                     BIT_MASK8(2)
#define UART_LCR_PARITY_DISABLE                     0
#define UART_LCR_PARITY_ENABLE                      BIT_MASK8(3)
#define UART_LCR_ODD_PARITY                         0
#define UART_LCR_EVEN_PARITY                        BIT_MASK8(4)
#define UART_LCR_STICK_PARITY_DISABLE               0
#define UART_LCR_STICK_PARITY_ENABLE                BIT_MASK8(5)
#define UART_LCR_BREAK_DISABLE                      0
#define UART_LCR_BREAK_ENABLE                       BIT_MASK8(6)
#define UART_LCR_HWFC_DISABLE                       0
#define UART_LCR_HWFC_ENABLE                        BIT_MASK8(7)

/* IER */
#define UART_IER_RX_INTR_ENABLE                     BIT_MASK8(0)
#define UART_IER_TX_INTR_ENABLE                     BIT_MASK8(1)
#define UART_IER_RX_LINE_STATUS_INTR_ENABLE         BIT_MASK8(2)
#define UART_IER_MODEM_STATUS_INTR_ENABLE           BIT_MASK8(3)

/* MCR */
#define UART_MCR_SET_NRTS                           BIT_MASK8(2)
#define UART_MCR_SET_LOOP                           BIT_MASK8(4)

/* FCR */
#define UART_FCR_FIFO_DISABLE                       0
#define UART_FCR_FIFO_ENABLE                        BIT_MASK8(0)
#define UART_FCR_RCVR_FIFO_RESET                    BIT_MASK8(1)
#define UART_FCR_XMIT_FIFO_RESET                    BIT_MASK8(2)
#define UART_FCR_DMA_MODE_DISABLE                   0
#define UART_FCR_DMA_MODE_ENABLE                    BIT_MASK8(3)
#define UART_FCR_RCVR_FIFO_TRIGGER_LEVEL_SET_1      BIT_FIELD(0,6)
#define UART_FCR_RCVR_FIFO_TRIGGER_LEVEL_SET_2      BIT_FIELD(1,6)
#define UART_FCR_RCVR_FIFO_TRIGGER_LEVEL_SET_4      BIT_FIELD(2,6)
#define UART_FCR_RCVR_FIFO_TRIGGER_LEVEL_SET_6      BIT_FIELD(3,6)

/* IIR */
#define UART_IIR_EXTRACT_INTR_PENDING(A)            ((A) & (BIT_MASK8(0)))
#define UART_IIR_EXTRACT_INTR_ID(A)                 ((A) & (BIT_MASK8(1) | BIT_MASK8(2) | BIT_MASK8(3)))
#define UART_IIR_EXTRACT_FIFO_ENABLE(A)             ((A) & (BIT_MASK8(6) | BIT_MASK8(7)))
#define UART_IIR_MODEM_STATUS                       BIT_FIELD(0,1)
#define UART_IIR_THR_EMPTY                          BIT_FIELD(1,1)
#define UART_IIR_RECEIVED_DATA_AVAILABLE            BIT_FIELD(2,1)
#define UART_IIR_RECEIVER_LINE_STATUS               BIT_FIELD(3,1)
#define UART_IIR_CHARACTER_TIMEOUT_INDICATION       BIT_FIELD(6,1)

/* LSR */
#define UART_LSR_DR_BIT_MASK                        BIT_MASK8(0)
#define UART_LSR_OE_BIT_MASK                        BIT_MASK8(1)
#define UART_LSR_PE_BIT_MASK                        BIT_MASK8(2)
#define UART_LSR_FE_BIT_MASK                        BIT_MASK8(3)
#define UART_LSR_BI_BIT_MASK                        BIT_MASK8(4)
#define UART_LSR_THRE_BIT_MASK                      BIT_MASK8(5)
#define UART_LSR_TEMT_BIT_MASK                      BIT_MASK8(6)
#define UART_LSR_ERR_IN_FIFO_BIT_MASK               BIT_MASK8(7)

/* MSR */
#define UART_MSR_DCTS_BIT_MASK                      BIT_MASK8(0)
#define UART_MSR_NCTS_BIT_MASK                      BIT_MASK8(4)

/* UART Controls */
#define UART_CTL_DISABLE                            0
#define UART_CTL_ENABLE                             BIT_MASK8(0)

/* UART Clock Control */
#define UART_CLK_CTRL_SELECT_24M                    0
#define UART_CLK_CTRL_SELECT_48M                    1



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_UART_GetIIR(A) { do { BANK_SFR_READ(FB,FBOFFS_RO_UART_IIR,(A)); } while (0); }
#define DRV_UART_GetLSR(A) { do { BANK_SFR_READ(FB,FBOFFS_RO_UART_LSR,(A)); } while (0); }
#define DRV_UART_GetMSR(A) { do { BANK_SFR_READ(FB,FBOFFS_RO_UART_MSR,(A)); } while (0); }



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_UART_GET_DMA_ADDR(HIBYTE_OFFSET,A) { do {                           \
    OS_ENTER_CRITICAL();                                                        \
    BANK_SFR_OFFSET(FB,HIBYTE_OFFSET);                                          \
    (A) = (U16)_BANK_SFR_SUCC_READ(FB) << 8;                                    \
    (A) |= _BANK_SFR_SUCC_READ(FB);                                             \
    OS_EXIT_CRITICAL();                                                         \
    } while (0); }

#define DRV_UART_SET_DMA_ADDR(HIBYTE_OFFSET,A) { do {                           \
    OS_ENTER_CRITICAL();                                                        \
    BANK_SFR_OFFSET(FB,HIBYTE_OFFSET);                                          \
    _BANK_SFR_SUCC_WRITE(FB,(A) >> 8);                                          \
    _BANK_SFR_SUCC_WRITE(FB,(U8)(A));                                           \
    OS_EXIT_CRITICAL();                                                         \
    } while (0); }

#define DRV_UART_SET_DMA_RX_BUF_ADDR(A)     DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_RXADDR_H,(A))
#define DRV_UART_SET_DMA_RX_BUF_LENGTH(A)   DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_RXLENGTH_H,(A))
#define DRV_UART_SET_DMA_RX_TRIG_LEVEL(A)   DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_TRIGGER_LEVEL_H,(A))
#define DRV_UART_SET_DMA_RX_CHAR_TIMEOUT(A) BANK_SFR_WRITE(FB,FBOFFS_WO_UART_DMA_CHAR_TIMEOUT,A);

#define DRV_UART_GET_DMA_RX_WADDR(A)        DRV_UART_GET_DMA_ADDR(FBOFFS_RO_UART_DMA_RX_CURRENT_WADDR_H,(A))
#define DRV_UART_SET_DMA_RX_RADDR(A)        DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_RX_CURRENT_RADDR_H,(A))
#define DRV_UART_SET_DMA_TX_ADDR(A)         DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_TXADDR_H,(A))
#define DRV_UART_SET_DMA_TX_LENGTH(A)       DRV_UART_SET_DMA_ADDR(FBOFFS_WO_UART_DMA_TXLENGTH_H,(A))

#define DRV_UART_ENABLE()                   BANK_SFR_WRITE(FB,FBOFFS_WO_UART_ENABLE,UART_CTL_ENABLE)
#define DRV_UART_DISABLE()                  BANK_SFR_WRITE(FB,FBOFFS_WO_UART_ENABLE,UART_CTL_DISABLE)

#define DRV_UART_SET_LCR(A)                 BANK_SFR_WRITE(FB,FBOFFS_WR_UART_LCR,(A));
#define DRV_UART_SET_FCR(A)                 BANK_SFR_WRITE(FB,FBOFFS_WO_UART_FCR,(A));
#define DRV_UART_SET_MCR(A)                 BANK_SFR_WRITE(FB,FBOFFS_WR_UART_MCR,(A));
#define DRV_UART_SET_IER(A)                 BANK_SFR_WRITE(FB,FBOFFS_WR_UART_IER,(A));
#define DRV_UART_GET_IER(A)                 BANK_SFR_READ(FB,FBOFFS_WR_UART_IER,(A));



////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * _DRV_UART_SetBaudRate
 *
 * Set UART Hardware BuadRate
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @BaudRate : Baud Rate value
 */
EXTERN VOID DRV_UART_SetBaudRate(
    IN      U32 BaudRate);

/**
 * DRV_UART_IsDmaBusy
 *
 * Check whether the DMA is busy
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : TRUE stands for DMA is busy
 */
EXTERN BOOL DRV_UART_IsDmaBusy(VOID);

#endif /* _DRV_UART_HAL_H_ */

