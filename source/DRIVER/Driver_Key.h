#ifndef _DRIVER_KEY_H_
	#define _DRIVER_KEY_H_

	
	#ifdef _DRIVER_KEY_C_
		#define DRIVER_KEY_EXT 
	#else
		#define DRIVER_KEY_EXT extern
	#endif


	DRIVER_KEY_EXT PUBLIC void DRIVER_Key_Init(void);
	DRIVER_KEY_EXT PUBLIC void DRIVER_Key_Update(void);
	DRIVER_KEY_EXT PUBLIC void DRIVER_Key_Polling(void);
	DRIVER_KEY_EXT PUBLIC void Driver_Key_CheckTimer(void);
#endif