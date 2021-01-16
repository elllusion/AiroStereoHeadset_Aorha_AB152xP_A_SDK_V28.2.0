//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>

#include "sector_driver_reserved_data.h"

#pragma userclass (HCONST = CONFIG)

SECTOR_DRIVER_RESERVED_DATA_STRU CODE gDriver_reserved_data_init =
{
	{
		0,
	},
    //- CRC
    {0,0}
};
