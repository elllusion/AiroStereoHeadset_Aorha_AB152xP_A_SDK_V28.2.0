#ifndef _DRV_SPI_HAL_H_
#define _DRV_SPI_HAL_H_

#include "config.h"

#if (HW_SPI_SUPPORTED)
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
typedef U8 SPI_STATUS;
#define SPI_STATUS_READY        (SPI_STATUS)(1)
typedef U8 SPI_INT_FLAG;
#define SPI_DONE_INT_FLAG        (SPI_INT_FLAG)(1)
#define SPI_TBC_INT_FLAG         (SPI_INT_FLAG)(1 << 1)

#define SPI_HW_PIN_4_5_6_7			0
#define SPI_HW_PIN_8_9_10_19		1
#define SPI_HW_PIN_14_15_16_17		2

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SPI_RXBC_INTR_MASK               BIT_MASK8(0)
#define SPI_RX_TH_RCH_INTR_MASK          BIT_MASK8(1)
#define SPI_RX_FULL_INTR_MASK            BIT_MASK8(2)
#define SPI_RX_OF_INTR_MASK              BIT_MASK8(3)
#define SPI_RX_TO_INTR_MASK              BIT_MASK8(4)
#define SPI_RX_PM_INTR_MASK              BIT_MASK8(5)

#define SPI_DONE_INTR_MASK               BIT_MASK8(0)
#define SPI_TXBC_INTR_MASK               BIT_MASK8(1)
#define SPI_TX_TH_RCH_INTR_MASK          BIT_MASK8(2)
#define SPI_TX_EMPTY_INTR_MASK           BIT_MASK8(3)
#define SPI_TX_UF_INTR_MASK              BIT_MASK8(4)


////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_SPI_Ready()                    ( (SFR_READ_BY_E2(E2OFFS_RW_SPI_STATUS)) & SPI_STATUS_READY ? 1 : 0)
#define DRV_SPI_DONE()                     ( (SFR_READ_BY_E2(E2OFFS_RW_SPI_TXINTFLAG)) & SPI_DONE_INT_FLAG ? 1 : 0)
#define DRV_SPI_CLEAR_DONE()               SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXINTFLAG, 0x01)


#define DRV_SPI_GetTXINTFLAG(A)            { do {                 \
    BANK_SFR_READ(E2,E2OFFS_RW_SPI_TXINTFLAG,(A));                \
    } while (0); }

#define DRV_SPI_SetTXINTFLAG(I)             { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXINTFLAG, I);                  \
    } while (0); }

#define DRV_SPI_GetRXINTFLAG(A)            { do {                 \
    BANK_SFR_READ(E2,E2OFFS_RW_SPI_RXINTFLAG,(A));                \
    } while (0); }

#define DRV_SPI_SetRXINTFLAG(I)             { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXINTFLAG, I);                  \
    } while (0); }

#define DRV_SPI_GetTxIntEn(A)             { do {                  \
    BANK_SFR_READ(E2,E2OFFS_RW_SPI_TXINTEN,(A));                  \	
    } while (0); }

#define DRV_SPI_GetRxIntEn(A)             { do {                  \
    BANK_SFR_READ(E2,E2OFFS_RW_SPI_RXINTEN,(A));                  \	
    } while (0); }

#define DRV_SPI_SetControl(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_CONTROL, I);                    \
    } while (0); }

#define DRV_SPI_SetConfig1(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_CONFIG1, I);                    \
    } while (0); }

#define DRV_SPI_SetConfig2(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_CONFIG2, I);                    \
    } while (0); }

#define DRV_SPI_SetCSByte(I)                { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_CSBYTE, I);                     \
    } while (0); }

#define DRV_SPI_SetSCLKMul(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_SCLKMUL, I);                    \
    } while (0); }

#define DRV_SPI_SetSCLKDiv(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_SCLKDIV, I);                    \
    } while (0); }

#define DRV_SPI_SetRxTOMul(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXTOMUL, I);                    \
    } while (0); }

#define DRV_SPI_SetRxTODiv(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXTODIV, I);                    \
    } while (0); }

#define DRV_SPI_SetRxIntEn(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXINTEN, I);                    \
    } while (0); }

#define DRV_SPI_SetTxIntEn(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXINTEN, I);                    \
    } while (0); }

#define DRV_SPI_SetSPITime(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TIME, I);                       \
    } while (0); }

#define DRV_SPI_SetWrWrTime(I)               { do {                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_WRTIME, I);                      \
    } while (0); }

#define DRV_SPI_SetTxSrcAddr(A)             { do {                        \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXSRCADDRLB,BYTE0((ADDR16)(A)));        \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXSRCADDRHB,BYTE1((ADDR16)(A)));        \
    } while (0); }

#define DRV_SPI_SetRxDstAddr(A)             { do {                         \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXSRCADDRLB,BYTE0((ADDR16)(A)));         \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXSRCADDRHB,BYTE1((ADDR16)(A)));         \
    } while (0); }

#define DRV_SPI_SetTxByteCount(A)            { do {                       \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TBCLB,BYTE0((ADDR16)(A)));              \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TBCHB,BYTE1((ADDR16)(A)));              \
    } while (0); }

#define DRV_SPI_SetRxByteCount(A)            { do {                       \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RBCLB,BYTE0((ADDR16)(A)));              \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RBCHB,BYTE1((ADDR16)(A)));              \
    } while (0); }

#define DRV_SPI_SetTxFifoSize(A)             { do {                            \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXFIFOSZLB,BYTE0((ADDR16)(A)));              \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXFIFOSZHB,BYTE1((ADDR16)(A)));              \
    } while (0); }

#define DRV_SPI_SetRxFifoSize(A)             { do {                            \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXFIFOSZLB,BYTE0((ADDR16)(A)));              \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXFIFOSZHB,BYTE1((ADDR16)(A)));              \
    } while (0); }

#define DRV_SPI_SetTxWPtrOffset(A)           { do {                            \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXWPTRHB,BYTE1((ADDR16)(A)));               \	
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXWPTRLB,BYTE0((ADDR16)(A)));               \
    } while (0); }

#define DRV_SPI_SetRxWPtrOffset(A)                 { do {                      \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXWPTRLB,BYTE0((ADDR16)(A)));                \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXWPTRHB,BYTE1((ADDR16)(A)));                \
    } while (0); }

#define DRV_SPI_SetRxRPtrOffset(A)                 { do {                      \
		SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXRPTRHB,BYTE1((ADDR16)(A))); 			   \	
		SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXRPTRLB,BYTE0((ADDR16)(A))); 			   \
		} while (0); }


#define DRV_SPI_SetRxThreshold(A)                 { do {                      \
		SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXTHHB,BYTE1((ADDR16)(A))); 		      \	
		SFR_WRITE_BY_E2(E2OFFS_RW_SPI_RXTHLB,BYTE0((ADDR16)(A))); 		      \
		} while (0); }

#define DRV_SPI_GET_DMA_ADDR(HIBYTE_OFFSET,A) { do {                            \
    OS_ENTER_CRITICAL();                                                        \
    BANK_SFR_OFFSET(E2,HIBYTE_OFFSET);                                          \
    (A) = (U16)_BANK_SFR_SUCC_READ(E2) << 8;                                    \
    (A) |= _BANK_SFR_SUCC_READ(E2);                                             \
    OS_EXIT_CRITICAL();                                                         \
    } while (0); }

#define DRV_SPI_SetTxWPtrOffsetLB(A)           { do {                         \
    SFR_WRITE_BY_E2(E2OFFS_RW_SPI_TXWPTRLB,BYTE0((ADDR16)(A)));               \
    } while (0); }

#define DRV_SPI_GET_DMA_RX_WADDR(A)        DRV_SPI_GET_DMA_ADDR(E2OFFS_RW_SPI_RXWPTRHB,(A))

VOID DRV_SPI_HAL_EnableHWPinConfig(U8 hwPinSel);
VOID DRV_SPI_HAL_DisableHWPinConfig(U8 hwPinSel);

VOID DRV_SPI_HAL_SetCSPin(U8 high);
VOID DRV_SPI_HAL_SetCSAuto(BOOL isAuto);

#endif /* HW_SPI_SUPPORTED */	

#endif /* _DRV_SPI_HAL_H_ */

