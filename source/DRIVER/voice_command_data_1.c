#include <config.h>
#include <cpu.h>

#include "bt_config_profile.h"
#include "voice_command_ctl.h"

#pragma userclass (HCONST = VO_DAT)


#ifdef SUPPORT_VOICE_COMMAND

#if VC_PLAN_MODEL
#pragma INCDIR(.\source\DRIVER\VC_DATA\PLAN)
#else
#pragma INCDIR(.\source\DRIVER\VC_DATA\AIROHA)
#endif



#if VOICE_LANG_CHI

#define VC_DATA_DEFINE(NAME)				vc_dat_##NAME##_c,
enum
{
	#include <vc_data_define_cht.h>
};
#undef VC_DATA_DEFINE

#define VC_EVT_DATA_MAP(EVTNAME,DATANAME1,DATANAME2,DATANAME3,DATANAME4,DATANAME5) ((0x00000001L << (vc_dat_##DATANAME1##_c)) | (0x00000001L << (vc_dat_##DATANAME2##_c)) | (0x00000001L << (vc_dat_##DATANAME3##_c)) | (0x00000001L << (vc_dat_##DATANAME4##_c)) | (0x00000001L << (vc_dat_##DATANAME5##_c))),

DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_CHINESE_EvtDataMap =
{
	{
		#include <vc_data_evt_map_cht.h>
	},
};

U16 CODE dataBase_C[] ALIGN(2) ={
	#include <database_cht.txt>
};
#endif

#endif //#ifdef SUPPORT_VOICE_COMMAND
