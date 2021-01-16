//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>
#include <crc16.h>
#include "bt_config_profile.h"

#include "sector_driver_led_data.h"

#pragma userclass (HCONST = CONFIG)


#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
	#include "air_stereo_A_led_data.h"
#else
	#include "led_data.h"
#endif