#ifndef _BUZZER_H_
#define _BUZZER_H_


	#ifdef BUZZER_C
		#define BUZZER_EXT 
	#else
		#define BUZZER_EXT extern
	#endif
	
	extern struct driver_buzzer_data_st;
	
	#define BUZZER_OFF	0xFF
	
	enum
	{
		BUZZ_BEEP_NONE,
		BUZZ_BEEP_SET_ON_PARA,	
		BUZZ_BEEP_START,
		BUZZ_BEEP_REST,
		BUZZ_BEEP_INTERVAL,
		BUZZ_BEEP_END,
	};
	
	typedef struct
	{
		U8 isOn;
		U8 regVal;
		U8 timerCntDown;
		U8 beepCntDown;
		U8 baseCntDown;
		BuzzPatternType buzzPattern;
		U8 state; 
		U8 IndexCount; 
		U8 mediaEventCode;
	}BuzzType;

	typedef struct
	{
		struct driver_buzzer_data_st CODE_PTR pDriverBuzzerData;
		BuzzType XDATA_PTR buzzDataPtr;
	}BUZZER_CTL_TYPE;
	
	BUZZER_EXT BUZZER_CTL_TYPE XDATA gBuzzer_ctl;
#endif