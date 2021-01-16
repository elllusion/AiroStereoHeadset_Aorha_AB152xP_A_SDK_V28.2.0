/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_ae_info.c defines the LM parameters.
 *
 * Programmer : ChunYi@airoha.com.tw, Ext.3356
 */
#include "config.h"
#include "cpu.h"
#include "sector_ae_info.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PUBLIC SECTOR_AE_INFO_STRU CODE gSector_AE_Info =
{
    {
		{
			0, //Major Version
			0, //Minor Version
			0, //Build  Number
			0, //Revision Number			
		},
		
		{
			0, //Year
			0, //Month
			0, //Date
		},
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    },

    //- CRC
    {0,0}
};