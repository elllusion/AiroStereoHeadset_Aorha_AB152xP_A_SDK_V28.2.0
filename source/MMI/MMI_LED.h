
enum
{
	ENABLE_LED,
	DISABLE_LED,
	TOGGLE_LED
};
	
PUBLIC void MMI_LED_SendBGLedCmd(U8 ledDisplayIndex);
PUBLIC void MMI_LED_SendFGLedCmd(U16 eventCode, U8 evtTableIndex);
PUBLIC void MMI_LED_SendLedFilterCmd(U8 index);
PUBLIC void MMI_LED_KeyOnOffHandler(U8 ledCmd);

#ifdef LIGHTING_ENABLE
PUBLIC void MMI_LED_Set_RGB(U8 LedR, U8 LedG, U8 LedB);
PUBLIC void MMI_LED_LightingMode(U8 LightMode);
PUBLIC void MMI_LED_AlarmClock(U8 OnOff);
PUBLIC void MMI_LED_Set_MusicYouCanSee(U8 DSP_Index, U8 DSP_Volume);
#endif

PUBLIC void MMI_LED_SDK_FilterCtl(U8 isOff);
PUBLIC void MMI_LED_SDK_FGCtl(U8 isOff);
PUBLIC void MMI_LED_SDK_BGCtl(U8 isOff);
