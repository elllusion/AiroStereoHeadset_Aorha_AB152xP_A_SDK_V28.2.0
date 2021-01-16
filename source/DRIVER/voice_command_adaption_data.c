#include <config.h>
#include <cpu.h>
#include "bt_config_profile.h"
#include "voice_command_ctl.h"

#pragma userclass (HCONST = VO_APD)

#ifdef SUPPORT_VOICE_COMMAND

#if VC_PLAN_MODEL
#pragma INCDIR(.\source\DRIVER\VC_DATA\PLAN)
#else
#pragma INCDIR(.\source\DRIVER\VC_DATA\AIROHA)
#endif


#define VC_DATA_DEFINE(NAME)				vc_dat_##NAME##_apd,

enum 
{
	#include <vc_data_define_apd.h> 
};
#undef VC_DATA_DEFINE 
	
#define VC_EVT_DATA_MAP(EVTNAME,DATANAME1,DATANAME2,DATANAME3,DATANAME4,DATANAME5) ((0x00000001L << (vc_dat_##DATANAME1##_apd)) | (0x00000001L << (vc_dat_##DATANAME2##_apd)) | (0x00000001L << (vc_dat_##DATANAME3##_apd)) | (0x00000001L << (vc_dat_##DATANAME4##_apd)) | (0x00000001L << (vc_dat_##DATANAME5##_apd))),

DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_APD_EvtDataMap =
{
	{
		#include <vc_data_evt_map_apd.h>
	},
};

U16 CODE dataBase_APD[] ALIGN(2) ={
	#include <database_apd.txt>
};

#else
DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_APD_EvtDataMap =
{
	{
		0
	},
};

U16 CODE dataBase_APD[] ALIGN(2) =
{
	0
};

#endif

