#ifndef _DRIVER_FM_H_
	#define _DRIVER_FM_H_

	
	#ifdef _DRIVER_FM_C_
		#define DRIVER_FM_EXT 
	#else
		#define DRIVER_FM_EXT extern
	#endif
	
	#define	MIN_FREQUENCE           875L
	#define	MAX_FREQUENCE           1080L
	
	#define FM_CMD_STATUS_WAITING	0
	#define FM_CMD_STATUS_OK		1
	#define FM_CMD_STATUS_FAIL		2
	
	#define FM_MUTE_MODE_OFF				0
	#define FM_MUTE_MODE_ON				1
	#define FM_MUTE_MODE_TOGGLE			2
	
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_Handler(void);
	DRIVER_FM_EXT PUBLIC S16 DRIVER_FM_GetFreq(void);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_Resume(void);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_InternalPowerOff(void);
	
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_PowerOnCmd(S16 setFreq, U8 historyVol);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_PowerOffCmd(void);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_TuneCmd(BOOL isUp);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_SeekCmd(BOOL isUp);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_MuteCmd(U8 muteMode);
	DRIVER_FM_EXT PUBLIC void DRIVER_FM_VolumeCmd(U8 FMVolume);
#endif
