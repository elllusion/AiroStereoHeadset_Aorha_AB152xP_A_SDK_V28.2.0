/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_config0_header.c defines the AFE, SFR, RF misc and HW misc parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_config0_flash.h"

#pragma userclass (HCONST = CFGHEAD)



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_CFG0_HEADER_STRU CODE gSectorConfig0Header =
{
    NUM_OF_SECTOR_CFG0,

    {
	    0
	},

	//- CRC
	{0x00,0x00}
};


