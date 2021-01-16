#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include "config.h"

#if (HW_SPI_SUPPORTED)
#include "cpu.h"

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 SPI_CONTROL;
#define SPI_CONTROL_ENABLE     (SPI_CONTROL)(1 << 0)
#define SPI_CONTROL_RESET      (SPI_CONTROL)(1 << 1)
#define SPI_CONTROL_START      (SPI_CONTROL)(1 << 2)


#define SPI_HW_GRP0		0	//Set GPIO  4/ 5/ 6/ 7 as SPI
#define SPI_HW_GRP1		1	//Set GPIO  8/ 9/10/19 as SPI
#define SPI_HW_GRP2		2	//Set GPIO 14/15/16/17 as SPI


#define SPICONFIG1_FDM_HALFDUPLEX           0x00
#define SPICONFIG1_FDM_FULLDUPLEX           0x01
#define SPICONFIG1_WIREMODE_3WIRE           0x00
#define SPICONFIG1_WIREMODE_4WIRE           0x02
#define SPICONFIG1_WIREMODE_3P1WIRE         0x04
#define SPICONFIG1_RX_FIFO_ENABLE           0x08
#define SPICONFIG1_OE_PININPUT              0x00
#define SPICONFIG1_OE_PINOUTPUT             0x10

#define SPICONFIG2_SPIMODE_REGMODE          0x00
#define SPICONFIG2_SPIMODE_DMAMODE          0x20
#define SPICONFIG2_SPIMODE_DMAFIFOMODE      0x40
#define SPICONFIG2_MSTR_SLAVE               0x00
#define SPICONFIG2_MSTR_MASTER              0x10
#define SPICONFIG2_CSPOL_ACTIVEHIGH         0x00
#define SPICONFIG2_CSPOL_ACTIVELOW          0x08
#define SPICONFIG2_TRANSFIRST_LSBFIRST      0x00
#define SPICONFIG2_TRANSFIRST_MSBFIRST      0x04
#define SPICONFIG2_SAMPLEPHASE_ODD          0x00
#define SPICONFIG2_SAMPLEPHASE_EVEN         0x02
#define SPICONFIG2_SCLKPOL_ACTIVEHIGH       0x00
#define SPICONFIG2_SCLKPOL_ACTIVELOW        0x01


/******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef struct
{
	OSMQ TxQueue;
}SPI_MASTER_DMA_CTL_STRU;

typedef union union_spi_control
{
    U8 value;
	struct stru_spi_control
	{
		U8 enable                     : 1;
		U8 reset                      : 1;
		U8 start                      : 1;
        U8 reserved_b3                : 1;
        U8 reserved_b4                : 1;
        U8 reserved_b5                : 1;
        U8 reserved_b6                : 1;
        U8 reserved_b7                : 1;
	} field;
} SPI_CONTROL_UNION;

typedef union union_spi_txth
{
    U16 value;
	struct stru_spi_txth
	{
		U8 txthHB;
		U8 txthLB;
	} field;
} SPI_TXTH_UNION;

typedef union union_spi_txrptroffset
{
    U16 value;
	struct stru_spi_txrptroffset
	{
		U8 txrptroffsetHB;
		U8 txrptroffsetLB;
	} field;
} SPI_TXRPTROFFSET_UNION;

typedef union union_spi_txwptroffset
{
    U16 value;
	struct stru_spi_txwptroffset
	{
		U8 txwptroffsetHB;
		U8 txwptroffsetLB;
	} field;
} SPI_TXWPTROFFSET_UNION;

typedef union union_spi_rxth
{
    U16 value;
	struct stru_spi_rxth
	{
		U8 rxthHB;
		U8 rxthLB;
	} field;
} SPI_RXTH_UNION;

typedef union union_spi_rxrptroffset
{
    U16 value;
	struct stru_spi_rxrptroffset
	{
		U8 rxrptroffsetHB;
		U8 rxrptroffsetLB;
	} field;
} SPI_RXRPTROFFSET_UNION;

typedef union union_spi_rxwptroffset
{
    U16 value;
	struct stru_spi_rxwptroffset
	{
		U8 rxwptroffsetHB;
		U8 rxwptroffsetLB;
	} field;
} SPI_RXWPTROFFSET_UNION;

typedef union union_spi_txfifosize
{
    U16 value;
	struct stru_spi_txfifosize
	{
		U8 txFifoSizeHB;
		U8 txFifoSizeLB;
	} field;
} SPI_TXFIFOSIZE_UNION;

typedef union union_spi_txsrcaddr
{
    U16 value;
	struct stru_spi_txsrcaddr
	{
		U8 txSrcAddrHB;
		U8 txSrcAddrLB;
	} field;
} SPI_TXSRCADDR_UNION;

typedef union union_spi_tbc
{
    U16 value;
	struct stru_spi_tbc
	{
		U8 tbcHB;
		U8 tbcLB;
	} field;
} SPI_TBC_UNION;

typedef union union_spi_rxfifosize
{
    U16 value;
	struct stru_spi_rxfifosize
	{
		U8 rxFifoSizeHB;
		U8 rxFifoSizeLB;
	} field;
} SPI_RXFIFOSIZE_UNION;

typedef union union_spi_rxsrcaddr
{
    U16 value;
	struct stru_spi_rxsrcaddr
	{
		U8 rxSrcAddrHB;
		U8 rxSrcAddrLB;
	} field;
} SPI_RXSRCADDR_UNION;

typedef union union_spi_rbc
{
    U16 value;
	struct stru_spi_rbc
	{
		U8 rbcHB;
		U8 rbcLB;
	} field;
} SPI_RBC_UNION;

typedef struct stru_spi_rw_control
{
	SPI_TBC_UNION               txByteCounts;
	SPI_RBC_UNION               rxByteCounts;
	SPI_TXSRCADDR_UNION         txAddr;
	SPI_RXSRCADDR_UNION         rxAddr;
	SPI_TXFIFOSIZE_UNION        txFifoSize;
	SPI_RXFIFOSIZE_UNION        rxFifoSize;
	SPI_TXWPTROFFSET_UNION      txWPtrOffset;
} SPI_RW_CONTROL_STRU;


/* SPI driver function */
typedef struct stru_spi_drv_func
{
    VOID (*ReadWriteBytesBlock)   (SPI_RW_CONTROL_STRU spiRWControl);
    VOID (*SendAllMsgOut)         (VOID);
	VOID (*TxQueHdl)		      (VOID);
	VOID (*SendNonBlockingMsg)    (VOID);

} SPI_DRV_STRU;


/******************************************************************************
 * External Global Variables
 ******************************************************************************/
EXTERN SPI_DRV_STRU CODE_PTR XDATA fSpiDrv;
EXTERN SPI_MASTER_DMA_CTL_STRU XDATA gSPI_MasterDMACtl;


////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
VOID DRV_SPI_WaitSPIReady(VOID);

#define SPI_BLOCKING_READ_WRITE_OK			0
#define SPI_BLOCKING_READ_NO_HW_SETTINGS	1
VOID DRV_SPI_BusInitial(VOID);
VOID DRV_SPI_ReadWriteBytesBlock_Rom(SPI_RW_CONTROL_STRU spiRWControl);


/*  DRV_SPI_EnableSPIHWPins: Set hardware pin as SPI function.
 *	<input 1>
 *		U8 spiGrp		: spi pin group number.
 *  	value range		: SPI_HW_GRP0, SPI_HW_GRP1, SPI_HW_GRP2.
 *		note			: before DRV_SPI_EnableSPIHWPins is called, grp pins should be normal gpio.
 */
VOID DRV_SPI_EnableSPIHWPins(U8 spiGrp);

/*  DRV_SPI_DisableSPIHWPins: Set hardware pin not as SPI function.
 *	<input 1>
 *		U8 spiGrp		: spi pin group number.
 *  	value range		: SPI_HW_GRP0, SPI_HW_GRP1, SPI_HW_GRP2.
 *		note			: DRV_SPI_DisableSPIHWPins is only valid when grp pins is set as SPI.
 */
VOID DRV_SPI_DisableSPIHWPins(U8 spiGrp);

#endif

#endif /* _DRV_SPI_H_ */

