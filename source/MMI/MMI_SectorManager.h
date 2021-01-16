#ifndef _MMI_SECTOR_MANAGER_H_
	#define _MMI_SECTOR_MANAGER_H_

	#define MMI_UPDATE_NVRAM(a)				(gMMI_ctl.isUpdateNvRam = (a) ? TRUE : FALSE)
	#define MMI_NEEDS_TO_UPDATE_NVRAM()		(gMMI_ctl.isUpdateNvRam)

	enum
	{
		SAVE_DATA_TYPE_RUNTIME,
		SAVE_DATA_TYPE_DSP_OFF,
		SAVE_DATA_TYPE_POWER_OFF,
		SAVE_DATA_TYPE_SDK,
	};
	PUBLIC void MMI_SECTOR_MmiNvramInit(void);
	PUBLIC void MMI_SECTOR_MmiDriverNvramInit(void);
	PUBLIC void MMI_SECTOR_SaveData(U8 writeType);
	PUBLIC void MMI_SECTOR_ResetPairedDevices(void);
	PUBLIC void MMI_SECTOR_ResetAirLinkInfo(void);
	PUBLIC void MMI_SECTOR_ResetFastLinkInfo(void);
	PUBLIC void MMI_SECTOR_ResetLinkInfo(void);
	PUBLIC void MMI_SaveMmiDriverNvramToFlash(U8 CODE_PTR flashAddr);
	PUBLIC BOOL MMI_SECTOR_WriteDeviceName(U8 XDATA_PTR namePtr, U8 len);
	
	PUBLIC U8 CODE_PTR MMI_SECTOR_GetValidCustormerSectorAddr(void);
#endif //_MMI_SECTOR_MANAGER_H_