#ifndef _MMI_FM_H_
	#define _MMI_FM_H_
	
#ifdef FM_ENABLE

	#ifdef _MMI_FM_C_
		#define MMI_FM_EXTERN
	#else
		#define MMI_FM_EXTERN extern
	#endif
	
	typedef struct
	{
		U8 curFMSoundLevel;
		U8 isDriverFMBusy;
		U8 fmEntryEvtMask;
		U8 volumeSmallStepCount;
	}MmiFMInfoCtrl;

	MMI_FM_EXTERN MmiFMInfoCtrl XDATA gMMI_FM_ctl;
	
	typedef struct
	{
		void (*fDriverFMBusyEvent)(MMIMsgType XDATA_PTR msgPtr);
		void (*fDriverFMNotBusyEvent)(MMIMsgType XDATA_PTR msgPtr);
		void (*fDriverFMDisabledEvent)(void);
		void (*fDriverFMCmdSetEvent)(void);
		void (*fDriverFMMuteOnEvent)(MMIMsgType XDATA_PTR msgPtr);
		void (*fDriverFMMuteOffEvent)(MMIMsgType XDATA_PTR msgPtr);
	}MMI_FM_DRV_HANDLE_TYPE;
	
	extern MMI_FM_DRV_HANDLE_TYPE CODE gMMI_FM_Drv_Handler;


	PUBLIC void MMI_FM_Init(void);
	PUBLIC void MMI_FM_DriverCmd(U8 FMCmd);
	PUBLIC void MMI_FM_VolumeControl(BOOL isVolumeUp);
	PUBLIC void MMI_FM_SendChannelVoicePromptCmd(S16 curFreq);
	
	PUBLIC void MMI_FM_ProcessKeyEvent(U8 linkIndex, U8 index);
	PUBLIC U16 MMI_FM_CheckValidKey(U8 index, U16 keyCode);
	
#endif //FM_ENABLE

#endif //_MMI_FM_H_