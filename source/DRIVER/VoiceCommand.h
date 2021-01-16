#ifndef _VOICE_COMMAND_H_
#define _VOICE_COMMAND_H_

	#ifdef VOICECOMMAND_C
		#define VOICE_COMMAND_EXT 
	#else
		#define VOICE_COMMAND_EXT extern
	#endif
	
	#define IS_VOICE_COMMAND_OVER	(gVC_ctl.end_status)
	#define VC_STOP_FLAG			(gVC_ctl.end_status)
	
	#define VC_STOP_BY_USER 0xFE
	
	#define VC_INVALID_EVT	0xFF
	
	typedef struct
	{
		U8 end_status;
		U8 result;
		U32 triggerBits;
		U32 apdtriggerBits;
		U32 dataBasePtr;
		U32 dataMapPtr;
		U32 apddataBasePtr;
		U32 apddataMapPtr;
		U8	vcnextLayer;
		U8 vcIdentificationMode;
	}VOICE_COMMAND_CTL_TYPE;
	
	enum
	{
		VC_MULTI_LAYER_SET_0 ,
		VC_MULTI_LAYER_SET_1 ,
		VC_MULTI_LAYER_SET_2 ,
		VC_MULTI_LAYER_SET_NO
	};
	
	enum
	{
		VC_EVT_INDEX_0  ,
		VC_EVT_INDEX_1  ,
		VC_EVT_NO
	};	
	PUBLIC U8 VoiceCommand_GetEvtIndex(U8 dataIndex);
	PUBLIC BOOL VoiceCommand_GetData(DriverMsg XDATA_PTR msgPtr);
	PUBLIC BOOL VoiceCommand_Start(void);
	PUBLIC BOOL VoiceCommand_Pause(void);
	PUBLIC void VoiceCommand_Stopped(void);
	PUBLIC U8 VoiceCommand_VCLayer(U8 dataIndex);
	PUBLIC U8 VoiceCommand_MultiLayerRootEvt(U8 vcSetNum);
	PUBLIC U8 VoiceCommand_IsEnabled(void);
	PUBLIC void VoiceCommand_ChooseIdentificationMode(void);
	PUBLIC BOOL VoiceCommand_GetAPDData(DriverMsg XDATA_PTR msgPtr);
	VOICE_COMMAND_EXT VOICE_COMMAND_CTL_TYPE XDATA gVC_ctl;

#endif//_VOICE_COMMAND_H_
