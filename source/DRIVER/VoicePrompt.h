#ifndef _VOICE_PROMPT_H_
#define _VOICE_PROMPT_H_

	#include "voice_prompt_ctl.h"

	#ifdef VOICE_PROMPT_C
		#define VOICE_PROMPT_EXT 
	#else
		#define VOICE_PROMPT_EXT extern
	#endif	

	
	#define IS_VOICE_PROMPT_OVER	(gVP_ctl.end_status)
	#define VP_STOP_FLAG			(gVP_ctl.end_status)
	#define VP_RAW_PTR				(gVP_ctl.vp_ptr)
	#define VP_COPY_PTR				(gVP_ctl.vp_ptr+WORDINBYTE)		
	#define AUDIO_MODE_NB_VP				0x03
	#define AUDIO_MODE_WB_VP				0x12
	#define AUDIO_MODE_NBH_VP				0x07
	#define	AUDIO_MODE_ADPCM_VP				0xAD
		
	#define ADUIO_ADPCM_CHECK_BIT			0x0100
	
	#define VP_EVT_DEFINE(NAME)		VP_EVT_##NAME,
	enum
	{
	  #include "voice_prompt_define.h"
		DEFAULT_VP_EVT_COUNT,
		
		//special handling voice prompt event start from here
		VP_EVT_SPECIAL_HANDLE_START = 0x70,
		
		VP_EVT_CALLER_ID = VP_EVT_SPECIAL_HANDLE_START,
		VP_EVT_FM_CHANNEL_NUMBER,
		VP_EVT_CALLER_NAME,
		VP_EVT_SPECIAL_HANDLE_END,

		//invalid voice prompt event
		VOICE_PROMPT_INVALID = 0x7F,
	};
	#undef VP_EVT_DEFINE
	
	#define VP_EVT_SPECIAL_HANDLE_COUNT		(VP_EVT_SPECIAL_HANDLE_END - VP_EVT_SPECIAL_HANDLE_START)


	enum
	{
		//special handling voice prompt data index
		VP_DAT_0,
		VP_DAT_1,
		VP_DAT_2,
		VP_DAT_3,
		VP_DAT_4,
		VP_DAT_5,
		VP_DAT_6,
		VP_DAT_7,
		VP_DAT_8,
		VP_DAT_9,
		VP_DAT_CALLER_ID_PREFIX,
		VP_DAT_CALLER_ID_SUFFIX,
		VP_DAT_FM_CHANN_PREFIX,
		VP_DAT_FM_CHANN_SUFFIX,
		VP_DAT_POINT,

		//break code for silence
		VP_DAT_BREAK = 0xFF
	};

	#define VP_DQ_NOTSET	1
	#define VP_DQ_INVALID	2

	//B15-B14 of Data Length Determines VP Quality
	#define VP_DAT_NB		0
	#define VP_DAT_WB		1
	#define VP_DAT_NBH		2
	#define VP_DAT_ADPCM	3
	
	#define DRIVER_VOICE_PROMPT_PLAY_TYPE_FROM_EEPROM_OR_SERIAL_FLASH_LIMIT 		0x81
	//Only need to distinguish the EEPROM case. 
	//  For ECODE or XDATA, the play type will be automatically set to proper value when setting vp_ptr.
	
	typedef struct
	{
		union
		{
			U32 vp_ptr;
			U8 play_type;
		} play_ctl;
		U16 length;
	} VoicePromptItem;
	
	typedef struct
	{
		U8 item_count;
		U8 vp_data_quality;
		U8 frame_length;
		U8 CODE_PTR vp_silence_ptr;
		U8 XDATA_PTR vp_pattern_ptr;
		U8 XDATA_PTR cmd_ptr1;
		U8 XDATA_PTR cmd_ptr2;
		VoicePromptItem XDATA_PTR voice_prompt_item_ptr_init;
		VoicePromptItem XDATA_PTR voice_prompt_item_ptr;
	} VoicePromptDataCtlType;
	
	
	typedef struct
	{
		U8 end_status;
		U16 XDATA_PTR vp_ptr;
		VoicePromptDataCtlType 	XDATA_PTR dataCtlPtr;
	}VOICE_PROMPT_CTL_TYPE;
	
	typedef struct
	{
		U16 checkBit;
		U32 patterFlashAddress;
		U16 patternLength;
	}VOICE_PROMPT_ADPCM_CTL_TYPE;
	
	#define VP_DATA_CTL_PTR			(gVP_ctl.dataCtlPtr)
	
	
	enum
	{
		VP_GET_DATA_OK,
		VP_GET_DATA_FAIL,
		VP_GET_DATA_BUSY,
	};
	

	PUBLIC BOOL VoicePrompt_GetData(DriverMsg XDATA_PTR msgPtr);
	PUBLIC BOOL VoicePrompt_Start(BOOL suspendApp, BOOL isOverSCO);
	PUBLIC BOOL VoicePrompt_Pause(void);
	PUBLIC void VoicePrompt_Stopped(void);

	
	VOICE_PROMPT_EXT VOICE_PROMPT_CTL_TYPE XDATA gVP_ctl;
#endif

