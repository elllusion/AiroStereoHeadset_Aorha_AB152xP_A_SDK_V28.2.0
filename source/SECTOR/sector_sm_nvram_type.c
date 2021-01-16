//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>

#include "sector_sm_nvram.h"

#pragma userclass (HCONST = CONFIG)


PUBLIC SECTOR_SM_NVRAM_STRU CODE gSM_nvram_init =
{
	{
		1,//SM_SECURITY_MODE1_LEVEL2,		//U8 leSecurityRequirement;
		0,								//U8 oobDataFlag:1;
		1,								//U8 bondingFlag:2;
		0,								//U8 MITM:1;
		3,//SM_IO_NO_INPUT_NO_OUTPUT,		//U8 ioCapability:3;
		0,//SM_BD_ADDR_TYPE_PUBLIC,			//U8 bdAddrType:1;
		0x0F,							//U8 keyDistr;
		16,								//U8 encKeySize;
	},

	//- CRC
    {0,0}		
};



/*
#define SM_IO_DISPLAY_ONLY 			0x00
#define SM_IO_DISPLAY_YES_NO		0x01
#define SM_IO_KEYBOARD_ONLY			0x02	
#define SM_IO_NO_INPUT_NO_OUTPUT	0x03
#define SM_IO_KEYBOARD_DISPLAY		0x04
#define SM_IO_CAPABILITY_MAX		0x05
*/