#ifndef _SECTOR_APP_CALLERNAME_DATA_H_
#define _SECTOR_APP_CALLERNAME_DATA_H_

	#include "config.h"
	#include "cpu.h"
	#include "crc16.h"
	#include "bt_config_profile.h"
	
	
	typedef struct
	{
		U8 callerNameData[2000];
	}APP_CALLERNAME_DATA_TYPE;
	
	
	typedef struct
	{
		APP_CALLERNAME_DATA_TYPE	callername_data;

		CRC16 CRC;
		
	} SECTOR_APP_CALLERNAME_DATA_STRU;
#endif //_SECTOR_APP_CALLERNAME_DATA_H_