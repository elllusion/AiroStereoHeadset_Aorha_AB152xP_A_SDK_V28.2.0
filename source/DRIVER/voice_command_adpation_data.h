#ifndef _VOICE_COMMAND_ADAPTION_H_
#define _VOICE_COMMAND_ADAPTION_H_

#include "os.h"
#include "bt_config_profile.h"

typedef struct
{
	U8 GENERIC_PTR vcDataBasePtr;
}DRIVER_VOICE_COMMAND_APD_ADDRESS_TABLE;

typedef struct
{
	DRIVER_VOICE_COMMAND_APD_ADDRESS_TABLE vcAddrTable;
	U8 isEnableAPD;
}DRIVER_VOICE_COMMAND_APD_DATA_TYPE;


typedef struct
{
	DRIVER_VOICE_COMMAND_APD_DATA_TYPE voiceCmdAPDData;
	CRC16 CRC;
}SECTOR_DRIVER_VOICE_COMMAND_APD_STRU;
#endif