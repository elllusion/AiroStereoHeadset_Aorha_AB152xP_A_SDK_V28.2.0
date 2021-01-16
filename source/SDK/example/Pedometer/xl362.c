#include "reside_flash.inc"
#include "os.h"
#include "xl362.h"

#ifdef SDK_SUPPORTED_PEDOMETER
#include "..\SDK\include\Spi.h"
#include "sector_spi_config.h"

//-------------------------------------------------------------------------/
/*Macro and Defines*/
#define GS_BUFFER_SIZE                  4
#define GS_SPI_RX_BUFFER_SIZE           2

//-------------------------------------------------------------------------/
/*Variables*/

static U8 gTxBuf[GS_BUFFER_SIZE];
static U8 gRxBuf[GS_SPI_RX_BUFFER_SIZE];

void xl362_ReadMultiReg(U8 regName, U8 XDATA_PTR dataPtr, U8 rxLen)
{

    gTxBuf[0] = XL362_CMD_R_REG;
    gTxBuf[1] = regName;
    SPI_WriteReadBlockingCmd(SPI_DEVICE_ID_CUSTOM1, 2, gTxBuf, rxLen, dataPtr);
}

U8 xl362_ReadReg(U8 regName)
{
    xl362_ReadMultiReg(regName, gRxBuf, 1);
    return gRxBuf[0];
}

void xl362_ReadMultiFIFO(U8 XDATA_PTR dataPtr, U8 rxLen)
{
    gTxBuf[0] = XL362_CMD_R_FIFO;
    SPI_WriteReadBlockingCmd(SPI_DEVICE_ID_CUSTOM1, 1, gTxBuf, rxLen, dataPtr);
}

void xl362_WriteReg(U8 regName, U8 regData)
{
    gTxBuf[0] = XL362_CMD_W_REG;
    gTxBuf[1] = regName;
    gTxBuf[2] = regData;
    xl362_WriteMultiReg((XL362_WRITE_MULTI_DATA_t XDATA_PTR)gTxBuf, 3);
}

void xl362_WriteMultiReg(XL362_WRITE_MULTI_DATA_t XDATA_PTR ptr, U8 txLen)
{
    SPI_WriteReadBlockingCmd(SPI_DEVICE_ID_CUSTOM1, txLen, (U8 XDATA_PTR)ptr, 0, gRxBuf);
}

#endif /* SDK_SUPPORTED_PEDOMETER */

