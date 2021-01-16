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


EXTERN SECTOR_AFE_INIT_STRU CODE gSector_AfeInit;
EXTERN SECTOR_SFR_INIT_STRU CODE gSector_SfrInit;
EXTERN SECTOR_RF_MISC_CTL_STRU CODE gSector_RfMiscCtl;
EXTERN SECTOR_HW_MISC_CTL_STRU CODE gSector_HwMiscCtl;
#ifdef AB1520B
#else
EXTERN SECTOR_UART_PARAMETER_STRU CODE gSector_UartParameterEng;
#endif


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_CFG0_HEADER_STRU CODE gSectorConfig0Header =
{
    NUM_OF_SECTOR_CFG0,

    {
	    #include "sector_config0_member.h"
	},

	//- CRC
	{0x2E,0xFE}
};


