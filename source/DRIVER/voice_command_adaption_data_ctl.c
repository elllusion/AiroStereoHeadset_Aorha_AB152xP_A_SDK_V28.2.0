//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>
#include "bt_config_profile.h"
#include "voice_command_adaption_data_ctl.h"
#include "voice_command_ctl.h"
#include "..\MMI\MMI_event_opcode.h"
#pragma userclass (HCONST = VO_APD)


#ifdef SUPPORT_VOICE_COMMAND

#if VC_PLAN_MODEL
#pragma INCDIR(.\source\DRIVER\VC_DATA\PLAN)
#else
#pragma INCDIR(.\source\DRIVER\VC_DATA\AIROHA)
#endif

#define VC_EVT_DEFINE(NAME)	VC_EVT_##NAME,
enum
{
	#include <vc_evt_define_apd.h>
};

#undef VC_EVT_DEFINE
#define VC_EVT_DEFINE(NAME)				VC_EVT_##NAME##_EVT_BIT = (0x00000001L << (VC_EVT_##NAME)),
enum
{
	#include <vc_evt_define_apd.h>
};
#undef VC_EVT_DEFINE

#include "sector_mmi_nvram_type.h"
#define	VC_APD_MMI_OFF							(U32)1<<MMI_OFF
#define	VC_APD_MMI_CONDISCABLE					(U32)1<<MMI_CONDISCABLE
#define	VC_APD_MMI_CONNECTABLE					(U32)1<<MMI_CONNECTABLE
#define	VC_APD_MMI_CONNECTED					(U32)1<<MMI_CONNECTED
#define	VC_APD_MMI_HFP_INCOMMING				(U32)1<<MMI_HFP_INCOMMING
#define	VC_APD_MMI_HFP_OUTGOING					(U32)1<<MMI_HFP_OUTGOING
#define	VC_APD_MMI_HFP_CALLACTIVE				(U32)1<<MMI_HFP_CALLACTIVE
#define	VC_APD_MMI_HFP_CALLACTIVE_WITHOUT_SCO	(U32)1<<MMI_HFP_CALLACTIVE_WITHOUT_SCO
#define	VC_APD_MMI_HFP_CAIMG					(U32)1<<MMI_HFP_CAIMG
#define	VC_APD_MMI_HFP_CAOGG					(U32)1<<MMI_HFP_CAOGG
#define	VC_APD_MMI_HFP_CAMULTY					(U32)1<<MMI_HFP_CAMULTY
#define	VC_APD_MMI_FAKEON						(U32)1<<MMI_FAKEON	  
#define	VC_APD_MMI_FAKEOFF						(U32)1<<MMI_FAKEOFF
#define	VC_APD_MMI_DETACHING_LINK				(U32)1<<MMI_DETACHING_LINK
#define	VC_APD_MMI_TEST_MODE					(U32)1<<MMI_TEST_MODE
#define	VC_APD_MMI_FM							(U32)1<<MMI_FM
#define	VC_APD_MMI_LINE_IN						(U32)1<<MMI_LINE_IN
#define	VC_APD_MMI_EEPROM_UPDATING				(U32)1<<MMI_UPDATING
#define	VC_APD_MMI_EEPROM_UPDATED_OK			(U32)1<<MMI_UPDATED_OK
#define	VC_APD_MMI_EEPROM_UPDATED_FAIL			(U32)1<<MMI_UPDATED_FAIL
#define	VC_APD_MMI_VOICE_PROMPT_LANG_SELECT		(U32)1<<MMI_VOICE_PROMPT_LANG_SELECT
#define	VC_APD_MMI_TWS_PAIRING					(U32)1<<MMI_TWS_PAIRING

#define VC_APD_MMI_ALL_STATES 0xFFFFFFFF


extern U16 CODE dataBase_APD[];
extern DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_APD_EvtDataMap;

SECTOR_DRIVER_VOICE_COMMAND_APD_STRU CODE gDriverVoiceCommandAPDData =
{
	{
		{
			(U8 GENERIC_PTR)&dataBase_APD,
			(U8 GENERIC_PTR)&gVoiceCommand_APD_EvtDataMap
		},
		DEFAULT_VC_IDENTIFY,
		TOTAL_APD_VOICE_COMMAND_EVENTS,//eventNo
		VOICE_COMMAND_TRIGGERED_ACTION_PATTERN,			//actionPatternNo
		{
		#ifdef TWS_SETTINGS
			{MEDIA_EVT_KEY_VOICE_COMMAND_ENABLE, 0, VC_APD_MMI_CONDISCABLE|VC_APD_MMI_CONNECTABLE|VC_APD_MMI_CONNECTED |VC_APD_MMI_TWS_PAIRING , 0},	//SET0
		#else
			{MEDIA_EVT_KEY_VOICE_COMMAND_ENABLE, 0, VC_APD_MMI_CONDISCABLE|VC_APD_MMI_CONNECTABLE|VC_APD_MMI_CONNECTED, 0},	//SET0
		#endif
			{MEDIA_EVT_ENTER_INCOMING, 1, VC_APD_MMI_HFP_INCOMMING, 1},				//SET1
			{MEDIA_EVT_VOICE_COMMAND_MULTILAYER_TRIGGER, 0, VC_APD_MMI_CONDISCABLE|VC_APD_MMI_CONNECTABLE|VC_APD_MMI_CONNECTED, 0},	//SET0
		},
		{
			#if VC_PLAN_MODEL
			{	//SET0
				{	
					0
				},
				{
					KEY_DISCOVERABLE, 				/*VC_EVT_PAIRING*/
					KEY_CHECK_BATTERY_STATUS,		/*VC_EVT_CHECK_BATTERY*/
					KEY_CHECK_STATUS_OF_CONNECTION,	/*VC_EVT_AM_I_CONNECTED*/
					KEY_RDIAL,							/*VC_EVT_LAST_CALL_OUT*/
					KEY_WHAT_CAN_I_SAY,				/*VC_EVT_WHAT_CAN_I_SAY*/
					0, 									/*VC_EVT_TAKE_THE_CALL*/
					0,									/*VC_EVT_REJECT*/ 
					KEY_VOICE_COMMAND_CANCEL,		/*VC_EVT_CANCEL*/ 
					0,								
					0, 
					0, 
				}
			},
			{	//SET1
				{	
					VC_EVT_TAKE_THE_CALL_EVT_BIT
				},
				{
					0, 								/*VC_EVT_PAIRING*/
					0,								/*VC_EVT_CHECK_BATTERY*/
					0,								/*VC_EVT_AM_I_CONNECTED*/
					0,								/*VC_EVT_LAST_CALL_OUT*/
					0,								/*VC_EVT_WHAT_CAN_I_SAY*/
					KEY_ACPCALL, 					/*VC_EVT_TAKE_THE_CALL*/
					KEY_REJCALL,						/*VC_EVT_REJECT*/ 
					0,								/*VC_EVT_CANCEL*/ 
					0,								
					0, 
					0,
				}
			},	
			#else
			{
				{
					0
				},
				{
					KEY_DISCOVERABLE, 				/*VC_EVT_PAIRING*/
					KEY_CHECK_STATUS,					/*VC_EVT_CHECK_STATUS*/
					KEY_VOICE_MAKE_A_CALL,				/*VC_EVT_MAKE_A_CALL*/
					KEY_CHECK_BATTERY_STATUS,			/*VC_EVT_BATTERY_LEVEL*/
					KEY_CHECK_BATTERY_STATUS,		/*VC_EVT_CHECK_BATTERY*/
					KEY_CHECK_STATUS_OF_CONNECTION, /*VC_EVT_CONNECTION_STATUS*/
					KEY_RDIAL,						/*VC_EVT_LAST_CALL_OUT*/ 
					KEY_SET_VOICE_RECOGNITION,		/*VC_EVT_VOICE_DIAL*/ 
					KEY_DIALNUM,						/*VC_EVT_RING_BACK*/
					KEY_WHAT_CAN_I_SAY, 			/*VC_EVT_WHAT_CAN_I_SAY*/
					0,								/*VC_EVT_TAKE_THE_CALL*/ 
					0,								/*VC_EVT_REJECT*/
					KEY_VOICE_COMMAND_CANCEL, 		/*VC_EVT_CANCEL*/
					0, 
					0,
				}
			},
			{
				{
					VC_EVT_TAKE_THE_CALL_EVT_BIT
				},
				{
					0, 								/*VC_EVT_PAIRING*/
					0,								/*VC_EVT_CHECK_STATUS*/
					0,								/*VC_EVT_MAKE_A_CALL*/
					0,								/*VC_EVT_BATTERY_LEVEL*/
					0,								/*VC_EVT_CHECK_BATTERY*/
					0, 								/*VC_EVT_CONNECTION_STATUS*/
					0,								/*VC_EVT_LAST_CALL_OUT*/ 
					0,								/*VC_EVT_VOICE_DIAL*/ 
					0,								/*VC_EVT_RING_BACK*/
					0, 								/*VC_EVT_WHAT_CAN_I_SAY*/
					KEY_ACPCALL,					/*VC_EVT_TAKE_THE_CALL*/ 
					0,						/*VC_EVT_REJECT*/
					0, 								/*VC_EVT_CANCEL*/
					0, 
					0, 
				}
			}
			#endif
		},
	},
	{0, 0}
};
#else

extern U16 CODE dataBase_APD[];
extern DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE gVoiceCommand_APD_EvtDataMap;

SECTOR_DRIVER_VOICE_COMMAND_APD_STRU CODE gDriverVoiceCommandAPDData =
{
	{
		{
			(U8 GENERIC_PTR)&dataBase_APD,
			(U8 GENERIC_PTR)&gVoiceCommand_APD_EvtDataMap
		},
		0,
		0,//eventNo
		0,			//actionPatternNo
		{
		
			{0, 0, 0, 0},	//SET0
		},
		{
			{	//SET0
				{	
					0
				},
				{
					0, 				/*VC_EVT_PAIRING*/
					0,				/*VC_EVT_CHECK_BATTERY*/
					0,				/*VC_EVT_AM_I_CONNECTED*/
					0,				/*VC_EVT_LAST_CALL_OUT*/
					0,				/*VC_EVT_WHAT_CAN_I_SAY*/
					0, 				/*VC_EVT_TAKE_THE_CALL*/
					0,				/*VC_EVT_REJECT*/ 
					0,				/*VC_EVT_CANCEL*/ 
					0,								
					0, 
					0, 
				}
			},
			{	//SET1
				{	
					0
				},
				{
					0, 								/*VC_EVT_PAIRING*/
					0,								/*VC_EVT_CHECK_BATTERY*/
					0,								/*VC_EVT_AM_I_CONNECTED*/
					0,								/*VC_EVT_LAST_CALL_OUT*/
					0,								/*VC_EVT_WHAT_CAN_I_SAY*/
					0, 								/*VC_EVT_TAKE_THE_CALL*/
					0,								/*VC_EVT_REJECT*/ 
					0,								/*VC_EVT_CANCEL*/ 
					0,								
					0, 
					0,
				}
			},	
		},
	},
	{0, 0}
};
#endif
