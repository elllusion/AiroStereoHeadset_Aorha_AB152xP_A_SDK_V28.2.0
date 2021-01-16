#ifndef _SECTOR_SPI_CONFIG_H_
#define _SECTOR_SPI_CONFIG_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_local_device_info.h defines the local device informations.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum
{
    SPI_GPIO_GROUP_SEL_0  = (U8)0,
    SPI_GPIO_GROUP_SEL_1  = (U8)1,
    SPI_GPIO_GROUP_SEL_2  = (U8)2,
};


#define SPI_DEVICE_ID_EXT_FLASH				0x01
#define SPI_DEVICE_ID_DBGPRING				0x02
#define SPI_DEVICE_ID_CUSTOM1				0x03
#define SPI_DEVICE_ID_04					0x04
#define SPI_DEVICE_ID_05					0x05
#define SPI_DEVICE_ID_06					0x06
#define SPI_DEVICE_ID_07					0x07


// This number must be "2" and should not be changed. Since in AB1520, we have only 2 SPIs.
#define SPI_DEVICE_COUNT			2
#define SPI_NONE_DEVICE				0


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef union union_spi_config1
{
    U8 value;
	struct stru_spi_config1
	{
		U8 fdm                        : 1;
		U8 wireMode                   : 2;
		U8 fifoRxEn                   : 1;
        U8 oe                         : 1;
        U8 reserved_b5                : 1;
        U8 reserved_b6                : 1;
        U8 reserved_b7                : 1;
	} field;

} SPI_CONFIG1_UNION;

typedef union union_spi_config2
{
    U8 value;
	struct stru_spi_config2
	{
		U8 pol                        : 1;
		U8 pha                        : 1;
		U8 msb                        : 1;
        U8 cs_pol                     : 1;
        U8 mstr                       : 1;
        U8 spi_mode                   : 2;
        U8 reserved_b7                : 1;
	} field;

} SPI_CONFIG2_UNION;

typedef union union_spi_csbyte
{
    U8 value;
	struct stru_spi_csbyte
	{
		U8 cs_byte                     : 8;
	} field;

} SPI_CSBYTE_UNION;

typedef union union_spi_sclkmul
{
    U8 value;
	struct stru_spi_sclkmul
	{
		U8 clk_mul                     : 8;
	} field;

} SPI_SCLKMUL_UNION;

typedef union union_spi_sclkdiv
{
    U8 value;
	struct stru_spi_sclkdiv
	{
		U8 clk_div                     : 8;
	} field;

} SPI_SCLKDIV_UNION;

typedef union union_spi_rxtomul
{
    U8 value;
	struct stru_spi_rxtomul
	{
		U8 rxtomul;
	} field;

} SPI_RXTOMUL_UNION;

typedef union union_spi_rxtodiv
{
    U8 value;
	struct stru_spi_rxtodiv
	{
		U8 rxtodiv;
	} field;

} SPI_RXTODIV_UNION;

typedef union union_spi_rxinten
{
    U8 value;
	struct stru_spi_rxinten
	{
		U8 rbcIntEn                   : 1;
		U8 rxThIntEn                  : 1;
		U8 rxFulIntEn                 : 1;
        U8 rxOfIntEn                  : 1;
        U8 rxToIntEn                  : 1;
        U8 pmIntEn                    : 1;
        U8 reserved_b6                : 1;
        U8 reserved_b7                : 1;
	} field;

} SPI_RXINTEN_UNION;

typedef union union_spi_txinten
{
    U8 value;
	struct stru_spi_txinten
	{
		U8 txDoneIntEn                : 1;
		U8 tbcIntEn                   : 1;
		U8 txThIntEn                  : 1;
        U8 txEmptyIntEn               : 1;
        U8 txUfIntEn                  : 1;
        U8 reserved_b5                : 1;
        U8 reserved_b6                : 1;
        U8 reserved_b7                : 1;
	} field;

} SPI_TXINTEN_UNION;

typedef union union_spi_time
{
    U8 value;
	struct stru_spi_time
	{
		U8 cs_sclk_sel                : 3;
		U8 sclk_cs_sel                : 3;
		U8 dis_time_sel               : 2;
	} field;

} SPI_TIME_UNION;

typedef union union_spi_wrwrtime
{
    U8 value;
	struct stru_spi_wrwrtime
	{
		U8 wr_wr_sel                  : 3;
	} field;

} SPI_WRWRTIME_UNION;

typedef struct stru_spi_ctl_config
{
    SPI_CONFIG1_UNION           config1;
    SPI_CONFIG2_UNION           config2;
    SPI_CSBYTE_UNION            csBytes;
    SPI_SCLKMUL_UNION           sclkmul;
    SPI_SCLKDIV_UNION           sclkdiv;
    SPI_RXTOMUL_UNION           rxTOMul;
    SPI_RXTODIV_UNION           rxTODiv;
    SPI_RXINTEN_UNION           rxIntEn;
    SPI_TXINTEN_UNION           txIntEn;
    SPI_TIME_UNION              spiTime;
    SPI_WRWRTIME_UNION          spiWrWrTime;
}SPI_CTL_CONFIG;

typedef struct stru_spi_hw_config
{
	U8 deviceID;
	U8 grpSel;
	U8 csPin;
}SPI_HW_CONFIG;

typedef struct stru_spi_device_config
{
	SPI_HW_CONFIG	hwConfig;
	SPI_CTL_CONFIG	ctlConfig;
}SPI_DEVICE_CONFIG;


typedef struct stru_spi_config
{
	U8 					deviceCount;
    SPI_DEVICE_CONFIG	deviceConfig[SPI_DEVICE_COUNT];
} SPI_CONFIG_STRU;

typedef struct stru_sector_spi_config
{
    SPI_CONFIG_STRU SpiConfig;

    CRC16 CRC;

} SECTOR_SPI_CONFIG_STRU;


#endif /* _SECTOR_SPI_CONFIG_H_ */

