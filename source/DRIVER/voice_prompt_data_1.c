
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
// Voice Prompt Data - Chinese
//
///////////////////////////////////////////////////////////////////////////
#if VOICE_LANG_CHI

U8 CODE VP_DAT_DEF(_C, 0)[] = {
	#include <C_0.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 1)[] = {
	#include <C_1.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 2)[] = {
	#include <C_2.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 3)[] = {
	#include <C_3.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 4)[] = {
	#include <C_4.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 5)[] = {
	#include <C_5.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 6)[] = {
	#include <C_6.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 7)[] = {
	#include <C_7.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 8)[] = {
	#include <C_8.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, 9)[] = {
	#include <C_9.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, BATTERY)[] = {
	#include <C_Battery.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, BLUETOOTH_MODE)[] = {
	#include <C_Bluetooth_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CALL)[] = {
	#include <C_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CALL_REJECTED)[] = {
	#include <C_Call_Rejected.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CALLER_ID_PREFIX)[] = {
	#include <C_CallerId_Prefix.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CALLER_ID_SUFFIX)[] = {
	#include <C_CallerId_Suffix.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CANCELLED)[] = {
	#include <C_Cancelled.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CHANNEL)[] = {
	#include <C_Channel.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CHINESE)[] = {
	#include <C_Chinese.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CONNECT)[] = {
	#include <C_Connect.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CONNECTED)[] = {
	#include <C_Connected.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, DISCONNECTED)[] = {
	#include <C_Disconnected.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, DOWN)[] = {
	#include <C_Down.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, ENDED)[] = {
	#include <C_Ended.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, FM)[] = {
	#include <C_FM.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, FM_CHANN_PREFIX)[] = {
	#include <C_FM_Chann_Prefix.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, FM_CHANN_SUFFIX)[] = {
	#include <C_FM_Chann_Suffix.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, FM_MODE)[] = {
	#include <C_FM_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, HIGH)[] = {
	#include <C_High.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, INCOMING_CALL)[] = {
	#include <C_Incoming_call.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, LEFT)[] = {
	#include <C_Left.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, LINE_IN_MODE)[] = {
	#include <C_Line_In_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, LOW)[] = {
	#include <C_Low.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, MEDIAN)[] = {
	#include <C_Median.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, MULTI_POINT_MODE)[] = {
	#include <C_Multi_Point_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, MUTE)[] = {
	#include <C_Mute.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, OFF)[] = {
	#include <C_Off.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, ON)[] = {
	#include <C_On.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, PAIRING)[] = {
	#include <C_Pairing.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, PAIRING_MODE)[] = {
	#include <C_Pairing_Mode.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, PAUSE)[] = {
	#include <C_Pause.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, PLEASE_CHARGE)[] = {
	#include <C_Please_charge.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, POINT)[] = {
	#include <C_Point.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, POWER_OFF)[] = {
	#include <C_Power_off.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, POWER_ON)[] = {
	#include <C_Power_on.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, RE_DIALING)[] = {
	#include <C_Re_dialing.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, RIGHT)[] = {
	#include <C_Right.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, SECOND_DEVICE)[] = {
	#include <C_Second_Device.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, SEEK)[] = {
	#include <C_Seek.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, SUCCESSFUL)[] = {
	#include <C_Successful.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, UP)[] = {
	#include <C_Up.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, VOICE_PROMPT)[] = {
	#include <C_Voice_prompt.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, VOLUME_MAXIMUM)[] = {
	#include <C_Volume_Maximum.wav.txt>
};
#ifdef SUPPORT_VOICE_COMMAND
U8 CODE VP_DAT_DEF(_C, WHAT_CAN_I_SAY)[] = {
	#include <C_What_Can_I_Say.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CONNECTION_STATUS)[] = {
	#include <C_Connection_Status.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CHECK_BATTERY)[] = {
	#include <C_Check_Battery.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CANCEL)[] = {
	#include <C_Cancel.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, VOICE_PROMPT_EXAMPLE)[] = {
	#include <C_Voice_Prompt_Example.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, TAKE_THE_CALL)[] = {
	#include <C_Take_The_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, REJECT)[] = {
	#include <C_Reject.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, REJECT_PHONE_CALL)[] = {
	#include <C_Reject_Phone_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, PAIRING_FOR_VC)[] = {
	#include <C_Pairing_For_VC.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, LAST_CALL_OUT)[] = {
	#include <C_Last_Call_Out.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, VOICE_DIAL_FOR_VC)[] = {
	#include <C_Voice_Dial_For_VC.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, MAKE_A_CALL)[] = {
	#include <C_Make_A_Call.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, CHECK_STATUS)[] = {
	#include <C_Check_Status.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, RING_BACK)[] = {
	#include <C_Ring_Back.wav.txt>
};
U8 CODE VP_DAT_DEF(_C, BEEP)[] = {
	#include <C_BEEP.wav.txt>
};

#endif //SUPPORT_VOICE_COMMAND

#endif //VOICE_LANG_CHI

#endif //SUPPORT_VOICE_PROMPT
