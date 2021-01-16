#ifndef _SECTOR_MMI_DRIVER_VARIATION_NVRAM_H_
#define _SECTOR_MMI_DRIVER_VARIATION_NVRAM_H_

#include "rc_flash.h"
#include "bt_config_profile.h"

#if defined(HID_SOC)

	#define NVRAM_LINK_KEY_ITEMS	4

	typedef	struct
	{
		U8 key_status;
		BD_ADDR bd_addr;
		U8 link_key[16];
		U8 key_type;
	} MMI_LINK_KEY_TYPE;

	typedef struct
	{
		U8	new_device;
	} MMI_NEW_DEVICE_PARA;

	typedef struct
	{
		MMI_LINK_KEY_TYPE link_key_info[NVRAM_LINK_KEY_ITEMS];
		U8 current_key_index;
		U8 lastDevInd;
		MMI_NEW_DEVICE_PARA mmi_new_device_para;
		U8 auth_requirments;

	} MMI_DRIVER_VARIATION_NVRAM_TYPE;

#elif defined(HEADSET_SOC)

	#include "..\Driver\voice_prompt_ctl.h"
	#include "..\Driver\Peq_HPF.h"


	//MMI EEPROM setting
	#define NVRAM_LINK_KEY_ITEMS	10

	#define NVRAM_AIR_LINK_KEY_NO	2

	#define NVRAM_LINK_KEY_MISC_MASK_CONNECTED				0x01
	#define NVRAM_LINK_KEY_MISC_MASK_HFP					0x02
	#define NVRAM_LINK_KEY_MISC_MASK_HSP					0x04
	#ifdef HID_Profile
	#define NVRAM_LINK_KEY_MISC_MASK_HID					0x10
	#endif

	typedef struct
	{
		/////// DO NOT insert any data between to match with HciLinkKeyNtf!!!!!!!///////////
		U8 bdAddr[6];
		U8 linkKey[16];
		U8 keyType;
		/////// DO NOT insert any data between to match with HciLinkKeyNtf !!!!!!///////////
	}HciLinkKeyNtf;

	#define NVRAM_PROFILE_NOROLE	0
	#define NVRAM_PROFILE_LEFT		0x01
	#define NVRAM_PROFILE_RIGHT		0x02
	#define NVRAM_PROFILE_RELAY		0x04
	#define NVRAM_PROFILE_FOLLOW	0x08

	typedef struct
	{
		U8 key_status;
		HciLinkKeyNtf linkData;
		U8 classOfDev[3];
		U8 sound_level;
		U8 scoSmallStep;
		U8 profile;
		U8 a2dpSoundLevel;
		U8 a2dpSmallSteps;
		U8 miscMask;
	} LinkHistoryType;

	/*
	* There are two parts of MMI data in EEPROM.
	* The first is setting area where MMI configuration is saved
	* The second is ringtone area where a ringtoneIndTbl with structure XX OO LL OO LL ... for ringtone data is saved to help find out the correct ringtone data
	* 	where XX is the no of saved ringtones, OOs are the offset to XX of the begin address of each ringtone data, and LL is the length
	*/


	#define IS_LED_DISABLED 	 	0x02
	#define IS_VP_DISABLED			0x04
	#define IS_RINGTONE_DISABLED	0x08
	#define IS_MULTIPOINTS_ENABLE	0x10
	#define IS_VOLUME_ORIENTATION_REVERSED	0x20
	#define IS_HID_ENABLED          0x40
	#define IS_MP_TEST_MODE_ENABLE	0x80

	#define MMI_SECTOR_DATA_VALID		0xAA
	#define MMI_SECTOR_DATA_INVALID		0x00

	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_ISMISCENABLED			(IS_MULTIPOINTS_ENABLE|IS_HID_ENABLED)
	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FM_HISTORY_FREQ			1062
	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMHISTORYVOL				6
	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMSMALLSTEPVOL			0

	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINHISTORYVOL			14
	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINSMALLSTEPVOL		0

	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_DEFAULTSCOSOUNDLEVEL		13
	#define MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_VPLANGINDEX				VOICE_LANG_DEFAULT

	typedef struct
	{
		U8 isMiscEnabled;
		S16 FM_history_Freq;
		U8 FMHistoryVol;
		U8 FMSmallStepVol;
		U8 LineInHistoryVol;
		U8 LineInSmallStepVol;
		U8 defaultSCOSoundLevel;
		U8 smallStepSoundLevelIdle;
		U8 vpLangIndex;
		BD_NAME	local_name;
	} DRIVER_BACKUP_NVRAM_TYPE;

	typedef struct
	{
		//non-zero init variables;
		DRIVER_BACKUP_NVRAM_TYPE init;

		//zero-safe variables;
		BD_ADDR agInd[MAX_MULTI_POINT_NO_FOR_SECTOR];
		U8 PEQRecord[PEQ_COMPONENT_NUM]; //A2DP and Line-In
		U8 PEQSectorMode[PEQ_COMPONENT_NUM]; //0->default PEQ, 1->custom PEQ
	}MISC_PARA_TYPE;

	typedef struct
	{
		U8 CheckValid;
		MISC_PARA_TYPE  misc_para;
		LinkHistoryType air_link_history_info[NVRAM_AIR_LINK_KEY_NO];
		LinkHistoryType fast_dongle_link_history_info;
		LinkHistoryType link_history_info[NVRAM_LINK_KEY_ITEMS];
	} MMI_DRIVER_VARIATION_NVRAM_TYPE;

	typedef struct
	{
		MMI_DRIVER_VARIATION_NVRAM_TYPE mmi_Driver_v_nvram;
		CRC16 CRC;
	}MMI_DRIVER_VARIATION_NVRAM_STRU;


	typedef struct
	{
		U8 CheckValid;
		U8 Para[1];
	} MMI_DRIVER_VARIATION_HEAD_STRU;

	typedef struct
	{
		DRIVER_BACKUP_NVRAM_TYPE init;
	}MMI_DRIVER_NVRAM_BACKUP_TYPE;

	typedef struct
	{
		MMI_DRIVER_NVRAM_BACKUP_TYPE	mmi_driver_nvram_backup;
		CRC16 CRC;	
	} SECTOR_MMI_DRIVER_NVRAM_BACKUP_STRU;


#else

	typedef struct
	{
		U8 dummy;
	} MMI_DRIVER_VARIATION_NVRAM_TYPE;

#endif

#endif /* _SECTOR_MMI_DRIVER_VARIATION_NVRAM_H_ */
