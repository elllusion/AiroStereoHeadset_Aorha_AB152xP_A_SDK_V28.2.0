#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\SDK\include\RingtoneVoicePrompt.h"

PUBLIC void SDK_DRV_SendRingtoneCmd(U16 mediaEventIndex, U8 beepPattern)
{
	MMI_DRV_SendRingtoneCmd(mediaEventIndex, beepPattern);
}

PUBLIC void SDK_DRV_SendRingtoneStopCmd()
{
	MMI_DRV_SendRingtoneStopCmd();
}

PUBLIC void SDK_DRV_SpecificEventRingtoneStopCmd(U16 mediaEventIndex)
{
	MMI_DRV_SpecificEventRingtoneStopCmd(mediaEventIndex);
}

PUBLIC void SDK_DRV_SendVoicePromptCmd(U16 mediaEventIndex, U8 voicePromptIndex)
{
	MMI_DRV_SendVoicePromptCmd(mediaEventIndex, voicePromptIndex);
}

PUBLIC void SDK_DRV_SendVoicepromptStopCmd(void)
{
	MMI_DRV_SendVoicepromptStopCmd();
}

PUBLIC void SDK_DRV_SpecificEventVPStopCmd(U16 mediaEventIndex)
{
	MMI_DRV_SpecificEventVPStopCmd(mediaEventIndex);
}
