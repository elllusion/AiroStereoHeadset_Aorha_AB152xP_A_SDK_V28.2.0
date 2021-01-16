
#ifndef __LCIF_APP_H__
#define __LCIF_APP_H__

#include "os.h"

#define __ENABLE_LCIF_APP__		1

#define MAX_BAT_LEVEL	795
#define MIN_BAT_LEVEL	570

#define LCIF_MAX_PLAYBACK_TIME			900
#if defined(SUPPORT_VOICE_PROMPT) && VOICE_LANG_CHI && VOICE_LANG_ENG
#define _LCIF_SUPPORT_VOICE_PROMPT_		1
#else
#define _LCIF_SUPPORT_VOICE_PROMPT_		0
#endif
#define _LCIF_SUPPORT_CHG_IND_			1
#define _LCIF_SUPPORT_TALK_TIME_		0
#define _LCIF_SUPPORT_PLAYBACK_TIME_	1

#define _LCIF_SUPPORT_MULTI_CONNECTION_	1

#define LCIF_SUBSCRIBED_BAT_LEVEL		0x01
#define LCIF_SUBSCRIBED_CHG_IND			0x02

extern U8 LCIFApp_SettingIdCheck(U16 setting_id, U8 msg_id);
extern U8 LCIFApp_SettingIdTypeQuery(U16 setting_id, U8 XDATA_PTR sizePtr);
extern void LCIFApp_SettingIdSetValue(U16 setting_id, U8 XDATA_PTR valuePtr, U8 size);
extern void LCIFApp_SettingIdGetValue(U16 setting_id, U8 XDATA_PTR valuePtr, U8 size);

#endif //__LCIF_APP_H__
