#ifndef _MMI_RECONNECT_H_
	#define _MMI_RECONNECT_H_
	
	#define LIST_NORMAL_DATA	TRUE
	#define LIST_HIGH_PRI_DATA	FALSE

	//The order can not be changed!!!!!
	enum
	{
		RECONNECT_TYPE_NULL,
		RECONNECT_PWR_ON,
		RECONNECT_USER_INIT,
		RECONNECT_LAST_ONCE_AUDIOTRANSFER,
		RECONNECT_LAST_ONCE_REDIAL,
		RECONNECT_LAST_ONCE_VOICE_DIAL,
		RECONNECT_FIRST_ONCE, 
		RECONNECT_FIRST_ONCE_REDIAL,
		RECONNECT_FIRST_ONCE_VOICE_DIAL,
		RECONNECT_SECONDARY_ONCE,
		RECONNECT_SECONDARY_ONCE_REDIAL,
		RECONNECT_SECONDARY_ONCE_VOICE_DIAL,
		RECONNECT_TWS_PAIRING,
		RECONNECT_LINK_LOSS,
		RECONNECT_WATCHDOG,
		RECONNECT_CHECK,
		RECONNECT_TWS_FOLLOW,
		RECONNECT_TWS_RELAY,
	};
	//The order can not be changed!!!!!
	
	//----------------------------------------------------
	//------------------- Reconnect ----------------------
	//----------------------------------------------------
	PUBLIC U8 MMI_Reconnect_GetListCnt(U8 isNormalData);
	PUBLIC void MMI_Reconnect_AddListByMode(U8 type, U8 reconnectMode);
	PUBLIC void MMI_Reconnect_DeleteListByBdAddr(U8 XDATA_PTR bdAddrPtr);
	PUBLIC void MMI_Reconnect_DeleteInvalidList(void);
	PUBLIC void MMI_Reconnect_ClearCtl(void);
	PUBLIC BOOL MMI_Reconnect_ToUARTIndex(U8 index);
	PUBLIC void MMI_Reconnect_SetDelayTimer(U32 timeValue);
	PUBLIC void MMI_Reconnect_CheckTimer(void);
	PUBLIC void MMI_Reconnect(U8 type);
	PUBLIC U8 MMI_Reconnect_GetPagingLink(void);
	PUBLIC void MMI_Reconnect_DisableFunc(void);
	PUBLIC void MMI_Reconnect_EnableFunc(void);
	PUBLIC BOOL MMI_Reconnect_FindTarget(U8 XDATA_PTR bdAddr);
	PUBLIC BOOL MMI_Reconnect_SearchList(U8 dataCnt, U8 XDATA_PTR currentIndexPtr);
	//----------------------------------------------------
	//------------------- Link History ----------------------
	//----------------------------------------------------
	PUBLIC U8 MMI_LinkKey_ShiftAirLinkKeyToTop (U8 targetIndex);	
	PUBLIC BOOL MMI_LinkKey_IsLinkWatchdog(void);
	PUBLIC void MMI_LinkKey_ClearConnectedFlag(void);
	PUBLIC U8 MMI_LinkKey_SearchAirLinkKeyIndex(U8 XDATA_PTR bd_addr, U8 needKeyOk);
	PUBLIC U8 MMI_LinkKey_GetNormalPDLNumber(void);
	PUBLIC U8 MMI_LinkKey_GetPDLNumber(void);
	PUBLIC U8 MMI_LinkKey_GetAirLinkPDLNumber(void);
	PUBLIC void MMI_LinkKey_ClearLinkHistory(U8 index);
	PUBLIC void MMI_LinkKey_ClearAirLinkHistory(U8 index);
	PUBLIC void MMI_LinkKey_ClearFastDongleLinkHistory(void);
	PUBLIC void MMI_LinkKey_MoveLinkToAirLink(U8 XDATA_PTR bdAddrPtr);
	PUBLIC void MMI_LinkKey_DeleteLinkHistory(U8 XDATA_PTR bdAddrPtr);
	PUBLIC void MMI_LinkKey_DeleteAirLinkHistory(U8 XDATA_PTR bdAddrPtr);
	PUBLIC LinkHistoryType XDATA_PTR MMI_LinkKey_SearchHistoryByBdAddr(U8 XDATA_PTR bdAddrPtr, BOOL isNeedOk, BOOL isNeedShiftTop);
	PUBLIC void MMI_LinkKey_SetLinkSoundLevelByHistory(U8 linkIndex);
	PUBLIC void MMI_LinkKey_LoadHistory(U8 linkIndex);
	PUBLIC void MMI_LinkKey_UpdateHistory (U8 XDATA_PTR ptr1, U8 isSuccessful);	
	PUBLIC void MMI_LinkKey_SaveHistoryInRunTime(U8 linkIndex);
	PUBLIC void MMI_LinkKey_SaveHistoryWhenDisconnect(U8 linkIndex);
	PUBLIC void MMI_LinkKey_UpdateAGInd(U8 linkIndex);
	PUBLIC U8 MMI_LinkKey_SearchLinkKeyIndex(U8 XDATA_PTR bd_addr, U8 needKeyOk);
#endif