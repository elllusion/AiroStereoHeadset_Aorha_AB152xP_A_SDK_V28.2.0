
#include "voice_prompt_ctl.h"

#ifdef SUPPORT_VOICE_PROMPT

#pragma userclass (HCONST = VO_DAT)


#if VP_WB_ENABLE
#pragma INCDIR(.\source\DRIVER\VP_DATA\WB)
#else
#pragma INCDIR(.\source\DRIVER\VP_DATA\NB)
#endif


///////////////////////////////////////////////////////////////////////////
//
// Voice Prompt Data - English
//
///////////////////////////////////////////////////////////////////////////
#if VOICE_LANG_ENG

U8 CODE VP_DAT_DEF(_E, 0)[] = {
	#include <E_0.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 1)[] = {
	#include <E_1.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 2)[] = {
	#include <E_2.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 3)[] = {
	#include <E_3.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 4)[] = {
	#include <E_4.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 5)[] = {
	#include <E_5.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 6)[] = {
	#include <E_6.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 7)[] = {
	#include <E_7.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 8)[] = {
	#include <E_8.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, 9)[] = {
	#include <E_9.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, BATTERY)[] = {
	#include <E_Battery.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, BLUETOOTH)[] = {
	#include <E_Bluetooth.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CALL)[] = {
	#include <E_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CALLER_ID_PREFIX)[] = {
	#include <E_CallerId_Prefix.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CALLER_ID_SUFFIX)[] = {
	#include <E_CallerId_Suffix.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CANCELLED)[] = {
	#include <E_Cancelled.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CHANNEL)[] = {
	#include <E_Channel.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CONNECTED)[] = {
	#include <E_Connected.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, DEVICE)[] = {
	#include <E_Device.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, DISCONNECTED)[] = {
	#include <E_Disconnected.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, DOWN)[] = {
	#include <E_Down.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, ENDED)[] = {
	#include <E_Ended.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, ENGLISH)[] = {
	#include <E_English.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, FM_CHANN_PREFIX)[] = {
	#include <E_FM_Chann_Prefix.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, FM_CHANN_SUFFIX)[] = {
	#include <E_FM_Chann_Suffix.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, FM_RADIO)[] = {
	#include <E_FM_Radio.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, HIGH)[] = {
	#include <E_High.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, INCOMING)[] = {
	#include <E_Incoming.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, LEFT)[] = {
	#include <E_Left.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, LEVEL)[] = {
	#include <E_Level.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, LINE_IN)[] = {
	#include <E_Line_in.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, LOW)[] = {
	#include <E_Low.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MAXIMUM)[] = {
	#include <E_Maximum.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MEDIAN)[] = {
	#include <E_Median.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MODE)[] = {
	#include <E_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MULTI_POINT)[] = {
	#include <E_Multi_point.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MUTE)[] = {
	#include <E_Mute.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, OFF)[] = {
	#include <E_Off.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, ON)[] = {
	#include <E_On.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, PAIRING)[] = {
	#include <E_Pairing.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, PAUSE)[] = {
	#include <E_Pause.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, PLEASE_CHARGE)[] = {
	#include <E_Please_charge.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, POINT)[] = {
	#include <E_Point.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, POWER)[] = {
	#include <E_Power.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, RE_DIALING)[] = {
	#include <E_Re_dialing.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, REJECTED)[] = {
	#include <E_Rejected.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, RIGHT)[] = {
	#include <E_Right.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, SECOND)[] = {
	#include <E_Second.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, SEEK)[] = {
	#include <E_Seek.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, SUCCESSFUL)[] = {
	#include <E_Successful.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, UP)[] = {
	#include <E_Up.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, VOICE_PROMPT)[] = {
	#include <E_Voice_prompt.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, VOLUME)[] = {
	#include <E_Volume.wav.txt>
};
#ifdef SUPPORT_VOICE_COMMAND
U8 CODE VP_DAT_DEF(_E, WHAT_CAN_I_SAY)[] = {
	#include <E_What_Can_I_Say.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CONNECTION_STATUS)[] = {
	#include <E_Connection_Status.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CHECK_BATTERY)[] = {
	#include <E_Check_Battery.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CANCEL)[] = {
	#include <E_Cancel.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, VOICE_PROMPT_EXAMPLE)[] = {
	#include <E_Voice_Prompt_Example.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, TAKE_THE_CALL)[] = {
	#include <E_Take_The_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, REJECT)[] = {
	#include <E_Reject.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, REJECT_PHONE_CALL)[] = {
	#include <E_Reject_Phone_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, PAIRING_FOR_VC)[] = {
	#include <E_Pairing_For_VC.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, LAST_CALL_OUT)[] = {
	#include <E_Last_Call_Out.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, VOICE_DIAL_FOR_VC)[] = {
	#include <E_Voice_Dial_For_VC.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, MAKE_A_CALL)[] = {
	#include <E_Make_A_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, CHECK_STATUS)[] = {
	#include <E_Check_Status.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, RING_BACK)[] = {
	#include <E_Ring_Back.wav.txt>
};
U8 CODE VP_DAT_DEF(_E, BEEP)[] = {
	#include <E_BEEP.wav.txt>
};

#endif //SUPPORT_VOICE_COMMAND

#endif //VOICE_LANG_ENG

#endif //SUPPORT_VOICE_PROMPT
