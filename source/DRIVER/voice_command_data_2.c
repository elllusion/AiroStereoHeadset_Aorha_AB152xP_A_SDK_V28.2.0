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


#if VOICE_LANG_ENG

#define VC_DATA_DEFINE(NAME)				vc_dat_##NAME##_e,
enum
{
	#include <vc_data_define_eng.h>
};
#undef VC_DATA_DEFINE

#define VC_EVT_DATA_MAP(EVTNAME,DATANAME1,DATANAME2,DATANAME3,DATANAME4,DATANAME5) ((0x00000001L << (vc_dat_##DATANAME1##_e)) | (0x00000001L << (vc_dat_##DATANAME2##_e)) | (0x00000001L << (vc_dat_##DATANAME3##_e)) | (0x00000001L << (vc_dat_##DATANAME4##_e)) | (0x00000001L << (vc_dat_##DATANAME5##_e))),
DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_ENGLISH_EvtDataMap =
{
{
	#include <vc_data_evt_map_eng.h>
	},
};

U16 CODE dataBase_E[] ALIGN(2) ={
	#include <database_eng.txt>
};
#endif

#endif //#ifdef SUPPORT_VOICE_COMMAND
