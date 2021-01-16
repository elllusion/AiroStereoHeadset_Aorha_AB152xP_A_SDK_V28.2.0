#include <config.h>
#include <cpu.h>
#include "sector_mmi_customize_v_nvram.h"

#ifndef AIR_UPDATE_DONGLE
#pragma userclass (HCONST = MMI_C_V_NVRAM2)

PUBLIC MMI_CUSTOMIZE_DATA_STRU CODE gMMI_Customize_v_nvram_init2 =
{
	{
		0, //reserve
	},
	{0,0} //CRC
};
#endif
