#ifndef _VOICE_COMMAND_ADAPTION_H_
#define _VOICE_COMMAND_ADAPTION_H_
#include <config.h>
#include <cpu.h>
#include "os.h"
#include "bt_config_profile.h"
#include "voice_command_ctl.h"

#define TOTAL_APD_VOICE_COMMAND_EVENTS				20

enum
{
	APD_DATA_DISABLED,
	APD_DATA_ENABLED
};

enum
{
	DEFAULT_VC_IDENTIFY =1,
	DEFAULT_VC_APD_MIXED_IDENTIFY =2,
	APD_VC_IDENTIFY =3, 
};

typedef struct 
{
	U8 GENERIC_PTR vcDataBasePtr;
	U8 GENERIC_PTR vcEvtDataMapPtr;
}DRIVER_VOICE_COMMAND_APD_ADDRESS_TABLE;


typedef struct
{
	U32 triggerEventBits;
	U16 triggeredAction[TOTAL_VOICE_COMMAND_PATTERNS];
}DRIVER_VOICE_COMMAND_ACTION_APD_TYPE;

typedef struct
{
	DRIVER_VOICE_COMMAND_APD_ADDRESS_TABLE vcAddrTable;
	U8 vcMode;
	U8 eventNo;
	U8 actionPatternNo;
	DRIVER_VC_EVENT_TYPE	vcEventTable[TOTAL_VOICE_COMMAND_EVENTS];
	DRIVER_VOICE_COMMAND_ACTION_APD_TYPE	actionTable[VOICE_COMMAND_TRIGGERED_ACTION_PATTERN];
}DRIVER_VOICE_COMMAND_APD_DATA_TYPE;

typedef struct
{
	DRIVER_VOICE_COMMAND_APD_DATA_TYPE voiceCmdAPDData;
	CRC16 CRC;
}SECTOR_DRIVER_VOICE_COMMAND_APD_STRU;
#endif