//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _RINGTONE_H_
#define _RINGTONE_H_

	//#define RINGTONE_COPY_XDATA_SRC	

#ifdef RINGTONE_C
	#define RINGTONE_EXT 
#else
	#define RINGTONE_EXT extern
#endif
	
	typedef U16 RINGTONE_DATA_MEMORY_PTR_TYPE;
	
	#define IS_RINGTONE_OVER  		(gRT_ctl.end_status)
	#define RINGTONE_STOP_FLAG		(gRT_ctl.end_status)
	
	extern struct driver_ringtone_data_st;

	typedef struct
	{
		U8 end_status;
		RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR ringtoneMemoryPtr;
		RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR playAddr;
		U16 ringtoneLength;
		U16 ringtoneNumber;
		struct driver_ringtone_data_st CODE_PTR pDriverRingtoneData;
		RINGTONE_DATA_MEMORY_PTR_TYPE CODE_PTR playAddrInCode;
	}RINGTONE_CTL_TYPE;
	
	#define RINGTINE_DATA_MEMORY_PTR	(gRT_ctl.ringtoneMemoryPtr)


	PUBLIC void Ring_Init(void);
	PUBLIC BOOL Ring_GetData(U8 beepIndex);
	#ifdef RINGTONE_COPY_XDATA_SRC
	PUBLIC BOOL Ring_CopyData(U8 XDATA_PTR srcPtr, U16 ringtoneLen);
	#endif
	PUBLIC BOOL Ring_Start(BOOL suspendApp);
	PUBLIC BOOL Ring_Pause(void);
	PUBLIC void Ring_Stopped(void);
	
	RINGTONE_EXT RINGTONE_CTL_TYPE XDATA gRT_ctl;

#endif

