#ifndef _MMI_AVRCP_H_
	#define _MMI_AVRCP_H_
	
	#ifdef _MMI_AVRCP_C_
		#define MMI_AVRCP_EXTERN
	#else
		#define MMI_AVRCP_EXTERN extern
	#endif
	
	#define MMI_AVRCP_GET_PLAY_STATUS(linkInd)					(gMMI_AVRCP_ctl.AvrcpCtl[linkInd].playStatus)
	
	#define MMI_IS_AVRCP_GET_PLAY_STATUS_SUPPORTED(linkInd)		(gMMI_AVRCP_ctl.AvrcpCtl[linkInd].isGetPlayStatusSupported)
	
	typedef struct
	{
		U8 attributeID;
		U8 attributeValue;
	}PlayAppElement;
	
	typedef union
	{
		U8 NotifyMask;
		U8 PlayStatus;
		U8 AbsoluteVolume;
		struct {
			U8 eventID;
			union {
				U8 PlayStatus;		//playback status changed
				U8 UID[8];			//track changed
				U32 PlaybackPos;	//playback pos changed
				U8 AbsoluteVolume;	//volume changed
				struct{				// play application settings
					U8 attributeIDNum;
					PlayAppElement attribute[2];
				}PlayAppSettings;
			} eventParams;
		} Notify;
		struct {
			U8 operationId;
			U8 operationParam;
		} VendorUnique;
		
		struct {
			U8 operationId;
			U8 operationParam;
			U32 syncClk;
			U8 ledPara[3];
		} VendorUniqueExtendLighting; //for #ifdef LIGHTING_ENABLE

		struct{
			U8 attributeID;
			U8 attributeValue;
		}GetPlayAppSettings;
	}AVRCP_TO_MMI_EVT_PARAM;
	
	//isSendingGetPlayStatus
	#define AVRCP_PLAY_STATUS_DONE FALSE
	#define AVRCP_PLAY_STATUS_SENT TRUE
	
	
	enum
	{
		MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC = 1,
		MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC,
		MMI_AVRCP_MISSION_QUERY_CURRENT_PLAY_STATUS,
		MMI_AVRCP_MISSION_MONITOR_PLAY_STATUS,
		MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS,
		MMI_AVRCP_MISSION_CHECK_STREAMING_STATE_LATER,
	};
	
	typedef struct
	{
		U8 taskId;
		U32 timer;
		void (*fcallback)(U8 linkIndex, U8 isSuccessful);
		U8 PlayStatus;
	}MMI_AVRCP_MISSION_PARAM_TYPE;
	
	#define AVRCP_GET_APP_SETTINGS_NONE	0
	#define AVRCP_GET_APP_SETTINGS_KEY	1
	#define AVRCP_GET_APP_SETTINGS_UART	2
	#define AVRCP_GET_APP_SETTINGS_SDK	3
	
	typedef struct
	{
		U8 isGetPlayStatusSupported:1;
		U8 isSendingGetPlayStatus:1;
		U8 isMusicPlay:1;
		U8 isDisablePlayStatusMonitor:1;
		U8 isPlayStatusWorking:1;
		U8 reserved:3;
		U8 playStatus;
		U8 lastTxOpIDForPlayPause;
		U8 getAppSettingsOwner;
		OSMQ Task_Que;
	}MmiAvrcpCtlType;
	typedef struct
	{
		MmiAvrcpCtlType AvrcpCtl[MAX_MULTI_POINT_NO];
	}GMmiAvrcpCtlType;
	
	MMI_AVRCP_EXTERN GMmiAvrcpCtlType XDATA gMMI_AVRCP_ctl;	
	
	PUBLIC void MMI_AVRCP_EventHandler(U8 linkIndex, U8 evtCode, AVRCP_TO_MMI_EVT_PARAM XDATA_PTR paramPtr);
	PUBLIC void MMI_AVRCP_ClearCtlData(U8 linkIndex);
	PUBLIC void MMI_AVRCP_ConnectedHandle(U8 linkIndex);
	PUBLIC BOOL MMI_AVRCP_IsConnected(U8 linkIndex);
	PUBLIC void MMI_AVRCP_CmdGenerator(U8 linkIndex, U8 opCode, U8 opID);
	PUBLIC void MMI_AVRCP_SetMusicPlay(U8 linkIndex, BOOL isPlay);
	PUBLIC void MMI_AVRCP_ResetKeyPlayToggle(U8 linkIndex);
	PUBLIC BOOL MMI_AVRCP_IsMusicPlaying(U8 linkIndex);
	PUBLIC void MMI_AVRCP_AbsVolToMusicSoundLevel(U8 linkIndex, U8 absVol);
	PUBLIC U8 MMI_AVRCP_SoundLevelToAbsoluteVol(U8 soundLevel, U8 maxLevel, U8 smallSteps, U8 steps);
	PUBLIC U8 MMI_AVRCP_GetAbsVolByMusic(U8 linkIndex);
	PUBLIC U8 MMI_AVRCP_GetAbsVolBySCO(U8 linkIndex);
	PUBLIC U8 MMI_AVRCP_GetAbsVolByIdle(void);
	
	PUBLIC void MMI_AVRCP_SendVendorUnique(U8 linkIndex, U8 operationId, U8 operationParam);
	PUBLIC void MMI_AVRCP_SetPlayAppSettings(U8 linkIndex, U8 attributeID, U8 attributeValue);
	PUBLIC void MMI_AVRCP_UartSetAbsVol(U8 vol);
	#ifdef AIRAPP_Profile
	PUBLIC void MMI_AVRCP_AppSetAbsVol(U8 linkIndex, U8 vol);
	#endif
	
	PUBLIC void MMI_AVRCP_ProcessKeyEvent(U8 linkIndex, U8 index);
	PUBLIC void MMI_AVRCP_CancelMission(U8 linkIndex, U8 taskId);
	PUBLIC void MMI_AVRCP_PushMission(U8 linkIndex, MMI_AVRCP_MISSION_PARAM_TYPE XDATA_PTR paramPtr);
	
	PUBLIC BOOL MMI_AVRCP_CheckMission(U8 linkIndex, U8 taskId);
	PUBLIC void MMI_AVRCP_Handle(void);
	PUBLIC U8 MMI_AVRCP_GetPlayStatus(U8 linkIndex);
	PUBLIC void MMI_AVRCP_CheckStreamingStateLater(U8 linkIndex, U8 playStatus, U8 needDelay);
	
	PUBLIC void MMI_AVRCP_HCICommand(MMIMsgType XDATA_PTR msgPtr);
	extern PUBLIC void MMI_HCI_SendAVRCPCmdResponse(U8 cmdOpcode, U8 resultCode);
	
	PUBLIC BOOL MMI_AVRCP_IsGetAppSettingsIdle(U8 linkIndex);
	PUBLIC void MMI_AVRCP_SDK_SetGetAppSettingsOwner(U8 linkIndex);

	PUBLIC void MMI_AVRCP_KeyPlay(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyStop(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyPause(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyRewindPush(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyFastForwardPush(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyForward(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyBackward(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyRewindRelease(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyFastForwardRelease(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyRepeatModeChange(U8 linkIndex);
	PUBLIC void MMI_AVRCP_KeyShuffleModeChange(U8 linkIndex);

#endif //_MMI_AVRCP_H_