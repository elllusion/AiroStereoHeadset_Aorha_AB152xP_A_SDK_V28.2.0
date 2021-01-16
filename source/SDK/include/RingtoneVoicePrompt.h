#ifndef _SDK_RINGTONEVOICEPROMPT_H_
#define _SDK_RINGTONEVOICEPROMPT_H_

#include "..\MMI\MMI_Driver.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////// Ring tone ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
This function is used to issue a command to play a specific beep pattern and link it with a specific event.
@param mediaEventIndex The specific event used to link with this playing activity.
@param beepPattern The specific beep pattern used to play in this playing activity. The input range (starting from 0) can be found out by checking "Ringtone > Ringtone No." in Config Tool.
*/
PUBLIC void SDK_DRV_SendRingtoneCmd(U16 mediaEventIndex, U8 beepPattern);
/*
This function is used to issue a ringtone stop playing command.
*/
PUBLIC void SDK_DRV_SendRingtoneStopCmd(void);
/*
This function is used to issue a ringtone stop playing command with a specific media event.
@param mediaEventIndex The specific media playing event we're trying to stop.
*/
PUBLIC void SDK_DRV_SpecificEventRingtoneStopCmd(U16 mediaEventIndex);

//////////////////////////////////////////////////////////////////////////////
//////////////////// Voice Prompt ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
This function is used to issue a command to play a specific voice prompt and link it with a specific event.

@param mediaEventIndex The specific event used to link with this playing activity.
@param voicePromptIndex The specific voice prompt used to play in this playing activity. The input range (starting from 1) can be found out by checking "Voice Prompt > Add or Modify a Voice Prompt > Step 1 Voice Prompt Event Name" in Config Tool.
*/
PUBLIC void SDK_DRV_SendVoicePromptCmd(U16 mediaEventIndex, U8 voicePromptIndex);
/*
This function is used to issue a voice prompt stop playing command.
*/
PUBLIC void SDK_DRV_SendVoicepromptStopCmd(void);
/*
This function is used to issue a voice prompt stop playing command with a specific media event.
@param mediaEventIndex The specific media playing event we're trying to stop.
*/
PUBLIC void SDK_DRV_SpecificEventVPStopCmd(U16 mediaEventIndex);

#endif //_SDK_RINGTONEVOICEPROMPT_H_
