/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_spi_config.c defines the configuration of SPI.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "sector_spi_config.h"
#include "drv_spi.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_SPI_CONFIG_STRU CODE gSectorSpiConfig =
{
	SPI_NONE_DEVICE,
	{
#if 1 // SPI settings for external flash.
		{ //deviceConfig[0];
			{
				SPI_DEVICE_ID_EXT_FLASH,
				SPI_GPIO_GROUP_SEL_2,
				0xFF,
			},
			{
				#ifdef SUPPORT_LE_PROFILE_HOGP
				/* SPI_CONFIG1 */
				SPICONFIG1_OE_PINOUTPUT |
				SPICONFIG1_FDM_HALFDUPLEX |
				SPICONFIG1_WIREMODE_3WIRE,
				/* SPI_CONFIG2 */
				SPICONFIG2_MSTR_MASTER |
				SPICONFIG2_SPIMODE_DMAMODE |
				SPICONFIG2_CSPOL_ACTIVELOW  |
				SPICONFIG2_TRANSFIRST_MSBFIRST  |
				SPICONFIG2_SCLKPOL_ACTIVELOW    |
				SPICONFIG2_SAMPLEPHASE_EVEN,
				/* SPI_CSBYTES */
				0x00,
				/* SPI_SCLKMUL */
				0x01,
				/* SPI_SCLKDIV */
				0x0C,
				/* SPI_RXTOMUL */
				0x01,
				/* SPI_RXTODIV */
				0x00,
				/* SPI_RXINTEN */
				0x00,
				/* SPI_TXINTEN */
				0x00,
				
				#else
				/* SPI_CONFIG1 */
				SPICONFIG1_OE_PINOUTPUT | SPICONFIG1_FDM_HALFDUPLEX | SPICONFIG1_WIREMODE_4WIRE,
				/* SPI_CONFIG2 */
				SPICONFIG2_MSTR_MASTER       | SPICONFIG2_SPIMODE_DMAMODE     |
				SPICONFIG2_CSPOL_ACTIVELOW   | SPICONFIG2_TRANSFIRST_MSBFIRST ,
				/* SPI_CSBYTES */
				0x00,
				/* SPI_SCLKMUL */
				0x02,
				/* SPI_SCLKDIV */
				0x00,
				/* SPI_RXTOMUL */
				0x01,
				/* SPI_RXTODIV */
				0x00,
				/* SPI_RXINTEN */
				0x00,
				/* SPI_TXINTEN */
				0x01,
				/* SPI_TIME */
				0x03F,
				/* SPI_WRWRTIME */
				0x00,
				#endif
			},
		},
#else // SPI settings for G-Sensor.
		{ //deviceConfig[2];
			{
				SPI_DEVICE_ID_CUSTOM1,
				SPI_GPIO_GROUP_SEL_2,
				0xFF,
			},
			{
				/* SPI_CONFIG1 */
				SPICONFIG1_OE_PINOUTPUT | SPICONFIG1_FDM_HALFDUPLEX | SPICONFIG1_WIREMODE_4WIRE,
				/* SPI_CONFIG2 */
				SPICONFIG2_MSTR_MASTER       | SPICONFIG2_SPIMODE_DMAMODE     |
				SPICONFIG2_CSPOL_ACTIVELOW   | SPICONFIG2_TRANSFIRST_MSBFIRST ,
				/* SPI_CSBYTES */
				0x00,
				/* SPI_SCLKMUL */
				0x02,
				/* SPI_SCLKDIV */
				0x00,
				/* SPI_RXTOMUL */
				0x01,
				/* SPI_RXTODIV */
				0x00,
				/* SPI_RXINTEN */
				0x00,
				/* SPI_TXINTEN */
				0x01,
				/* SPI_TIME */
				0x03F,
				/* SPI_WRWRTIME */
				0x00,
			},
		},
#endif
		{ //deviceConfig[1];
			{
				SPI_DEVICE_ID_DBGPRING,
				SPI_GPIO_GROUP_SEL_0,
				0xFF,
			},
			{
				/* SPI_CONFIG1 */
				SPICONFIG1_OE_PINOUTPUT | SPICONFIG1_FDM_HALFDUPLEX | SPICONFIG1_WIREMODE_4WIRE,
				/* SPI_CONFIG2 */
				SPICONFIG2_MSTR_MASTER       | SPICONFIG2_SPIMODE_DMAMODE     |
				SPICONFIG2_CSPOL_ACTIVELOW   | SPICONFIG2_TRANSFIRST_MSBFIRST ,
				/* SPI_CSBYTES */
				0x00,
				/* SPI_SCLKMUL */
				0x02,
				/* SPI_SCLKDIV */
				0x00,
				/* SPI_RXTOMUL */
				0x01,
				/* SPI_RXTODIV */
				0x00,
				/* SPI_RXINTEN */
				0x00,
				/* SPI_TXINTEN */
				0x01,
				/* SPI_TIME */
				0x03F,
				/* SPI_WRWRTIME */
				0x00,
			},
		},
	},

    {0,0}
};
