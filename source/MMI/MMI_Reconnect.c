#include "reside_flash.inc"

#define _MMI_RECONNECT_C_

#include "rc.h"
#include "bt_config_profile.h"
#include "os_flash.h"

#include "MMI.h"
#include "MMI_Base.h"
#include "MMI_Protocol.h"
#include "MMI_Reconnect.h"
#include "MMI_Driver.h"
#include "MMI_State.h"
#include "MMI_HCI.h"
#include "MMI_AirModule.h"
#include "MMI_SDAP.h"
#include "MMI_A2DP.h"



PRIVATE void MMI_LinkKey_RemoveSequenceLinkHistory(LinkHistoryType XDATA_PTR linkListPtr, U8 index, U8 isAirLink);

PRIVATE void MMI_Reconnect_AddReconnectList(U8 type, U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR classOfDevPtr)
{
	U8 index;
	MMIReonnectDataType XDATA_PTR targetDataPtr;
	MMIReonnectDataType XDATA_PTR backDataPtr;
	
	/* mantis 8255
	for(index = 0; index < MAX_MULTI_POINT_NO; index++)
	{
		if(MMI_IsActiveLink(index) && SYS_IsBDAddrIsTheSame(bdAddrPtr,&gMMI_ctl.mmiInfo[index].bdAddr[0]))
			return;
	}*/
	
	LightDbgPrint("==AddReconList:%x,%x,%x",(U8)bdAddrPtr[0],(U8)bdAddrPtr[1],(U8)bdAddrPtr[2]);

	//1. Prepare reconnectList Memory
	if(gMMI_ctl.reconnectCtl.reconnectListPtr == (MMIReconnectListType XDATA_PTR)NULL)
	{
		gMMI_ctl.reconnectCtl.reconnectListPtr = (MMIReconnectListType XDATA_PTR)SYS_MemoryGet(sizeof(MMIReconnectListType));
		if(gMMI_ctl.reconnectCtl.reconnectListPtr)
		{
			OSMEM_memset_xdata((U8 XDATA_PTR)gMMI_ctl.reconnectCtl.reconnectListPtr, 0, sizeof(MMIReconnectListType));
			gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex = 0xFF;
			gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex = 0xFF;
		}
		else
		{
			//Allocate Memory fail, no reconnection!
			return;
		}
	}


	if(type == RECONNECT_LINK_LOSS || type == RECONNECT_WATCHDOG)
	{
		if(type == RECONNECT_LINK_LOSS && gMMI_nvram.timerCtl.reconnectdurationLinkLossTimer)
		{
			gMMI_ctl.reconnectCtl.reconnectListPtr->isDurationTimerSet = TRUE;
			OST_SetTimer(&gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectDurationTimer,gMMI_nvram.timerCtl.reconnectdurationLinkLossTimer* HALF_SEC);
		}
		//Find the duplicated data
		for(index = 0; index < NVRAM_LINK_KEY_ITEMS; index++)
		{
			if(SYS_IsBDAddrIsTheSame((U8 XDATA_PTR)bdAddrPtr, &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index].bdAddr[0]))
				break;
		}


		//New data! Allocate the new data space
		if(index == NVRAM_LINK_KEY_ITEMS)
		{
			for(index = 0; index < NVRAM_LINK_KEY_ITEMS; index++)
			{
				if(gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index].reconnectType == RECONNECT_TYPE_NULL)
				{
					break;
				}
			}
		}

		//No empty space, delete the last one
		if(index == NVRAM_LINK_KEY_ITEMS)
			index = (NVRAM_LINK_KEY_ITEMS-1);


		//2. Set Parameters
		targetDataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index];
		OSMEM_memcpy_xdata_xdata(&(targetDataPtr->bdAddr[0]), bdAddrPtr, 6);
		OSMEM_memcpy_xdata_xdata(&(targetDataPtr->classOfDev[0]), classOfDevPtr, 3);
		targetDataPtr->reconnectType = type;
		if(type == RECONNECT_LINK_LOSS)
		{
			if(MMI_EOF == MMI_LinkKey_SearchAirLinkKeyIndex(bdAddrPtr, FALSE))
				targetDataPtr->retryCnt = gMMI_nvram.reConnectInfo.linkLossReTryCnt;
			else
				targetDataPtr->retryCnt = gMMI_nvram.twsLinkLossRetryCnt;
		}
		else
		{
			targetDataPtr->retryCnt = gMMI_nvram.reConnectInfo.watchDogReTryCnt;
		}

		if(index)
		{
			backDataPtr = (MMIReonnectDataType XDATA_PTR)SYS_MemoryGet(sizeof(MMIReonnectDataType));
			if(backDataPtr)
			{
				if(gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex < index)
					gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex++;

				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)backDataPtr, (U8 XDATA_PTR)targetDataPtr, sizeof(MMIReonnectDataType));
				for(; index; index--)
				{
					OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index], (U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index-1], sizeof(MMIReonnectDataType));
				}
				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[0], (U8 XDATA_PTR)backDataPtr, sizeof(MMIReonnectDataType));
				OSMEM_Put((U8 XDATA_PTR) backDataPtr);
			}
		}
	}
	else
	{
		//Find the duplicated data
		for(index = 0; index < NVRAM_LINK_KEY_ITEMS; index++)
		{
			if(SYS_IsBDAddrIsTheSame((U8 XDATA_PTR)bdAddrPtr, &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index].bdAddr[0]))
				break;
		}

		//New data! Allocate the new data space
		if(index == NVRAM_LINK_KEY_ITEMS)
		{
			for(index = 0; index < NVRAM_LINK_KEY_ITEMS; index++)
			{
				if(gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index].reconnectType == RECONNECT_TYPE_NULL)
				{
					break;
				}
			}
		}

		//No empty space, delete the last one
		if(index == NVRAM_LINK_KEY_ITEMS)
			index = (NVRAM_LINK_KEY_ITEMS-1);

		//2. Set Parameters
		targetDataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index];
		OSMEM_memcpy_xdata_xdata(&(targetDataPtr->bdAddr[0]), bdAddrPtr, 6);
		OSMEM_memcpy_xdata_xdata(&(targetDataPtr->classOfDev[0]), classOfDevPtr, 3);
		targetDataPtr->reconnectType = type;
		targetDataPtr->retryCnt = gMMI_nvram.reConnectInfo.autoRecTryCnt;

	}
}

PRIVATE void MMI_Reconnect_AddListByLinkKeyIndex(U8 type, U8 index)
{
	if(gMMI_driver_variation_nvram.link_history_info[index].key_status == NVRAM_KEY_OK)
	{
		MMI_Reconnect_AddReconnectList(	type, 
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[index].linkData.bdAddr[0],
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[index].classOfDev[0]);
	}
}

PRIVATE void MMI_Reconnect_AddListByAirLinkKeyIndex(U8 type, U8 index)
{
	LightDbgPrint("==AddListByAirLink:%d",(U8)index, (U8)gMMI_driver_variation_nvram.air_link_history_info[index].key_status);
	if(gMMI_driver_variation_nvram.air_link_history_info[index].key_status == NVRAM_KEY_OK)
	{
		MMI_Reconnect_AddReconnectList(	type, 
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[index].linkData.bdAddr[0],
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[index].classOfDev[0]);
	}
}

PRIVATE void MMI_Reconnect_AddListByFastDongle(U8 type)
{
	if(gMMI_driver_variation_nvram.fast_dongle_link_history_info.key_status == NVRAM_KEY_OK)
	{
		MMI_Reconnect_AddReconnectList(	type, 
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info.linkData.bdAddr[0],
										(U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info.classOfDev[0]);
	}
}

PRIVATE void MMI_Reconnect_AddListByBdAddr(U8 type, U8 XDATA_PTR bdAddr)
{
	LinkHistoryType XDATA_PTR linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr(bdAddr, TRUE, FALSE);
	
	if(linkListPtr)
	{
		MMI_Reconnect_AddReconnectList(type, bdAddr, &linkListPtr->classOfDev[0]);
	}
}

PUBLIC void MMI_Reconnect_AddListByMode(U8 type, U8 reconnectMode)
{
	U8 i;
	switch(reconnectMode)
	{
		case RECONNECT_FAST_DONGLE:
			MMI_Reconnect_AddListByFastDongle(type);	
			break;
		case RECONNECT_TWS:
			MMI_Reconnect_AddListByAirLinkKeyIndex(type, 0);
			break;
		case RECONNECT_LAST_TWO:
			MMI_Reconnect_AddListByLinkKeyIndex(type, 0);
			MMI_Reconnect_AddListByLinkKeyIndex(type, 1);
			break;
		case RECONNECT_LAST_ONE:
			MMI_Reconnect_AddListByLinkKeyIndex(type, 0);
			break;
		case RECONNECT_AG_IND_0:
			MMI_Reconnect_AddListByBdAddr(type, &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]);
			break;
		case RECONNECT_AG_IND_0_1:
			MMI_Reconnect_AddListByBdAddr(type, &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]);
		case RECONNECT_AG_IND_1:
			if(!SYS_IsBDAddrIsTheSame(&gMMI_driver_variation_nvram.misc_para.agInd[0].member[0],&gMMI_driver_variation_nvram.misc_para.agInd[1].member[0]))
				MMI_Reconnect_AddListByBdAddr(type, &gMMI_driver_variation_nvram.misc_para.agInd[1].member[0]);
			break;

		case RECONNECT_LAST_DISC:
			for(i = 0;  i < MAX_MULTI_POINT_NO; i++)
			{
				if(MAX_MULTI_POINT_NO == MMI_GetLinkIndexByBdA(&gMMI_driver_variation_nvram.misc_para.agInd[i].member[0]))
				{
					MMI_Reconnect_AddListByBdAddr(type, &gMMI_driver_variation_nvram.misc_para.agInd[i].member[0]);
					break;
				}
			}
			break;

		case RECONNECT_LIST:
			for(i = 0;  i < NVRAM_AIR_LINK_KEY_NO; i++)
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, i);
				
			for(i = 0;  i < gMMI_nvram.storedLinkKeyCnt; i++)
				MMI_Reconnect_AddListByLinkKeyIndex(type, i);
			break;
			
		case RECONNECT_ALL_DROPPED_LINKS:
			for(i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
			{
				if((gMMI_driver_variation_nvram.air_link_history_info[i].miscMask & NVRAM_LINK_KEY_MISC_MASK_CONNECTED) &&
					MAX_MULTI_POINT_NO == MMI_GetLinkIndexByBdA(&gMMI_driver_variation_nvram.air_link_history_info[i].linkData.bdAddr[0]))
				{
					MMI_Reconnect_AddListByAirLinkKeyIndex(type, i);
				}
			}	
			for(i = 0; i < gMMI_nvram.storedLinkKeyCnt; i++)
			{
				if((gMMI_driver_variation_nvram.link_history_info[i].miscMask & NVRAM_LINK_KEY_MISC_MASK_CONNECTED) &&
					MAX_MULTI_POINT_NO == MMI_GetLinkIndexByBdA(&gMMI_driver_variation_nvram.link_history_info[i].linkData.bdAddr[0]))
				{
					MMI_Reconnect_AddListByLinkKeyIndex(type, i);
				}
			}
			break;
		
		case RECONNECT_TWS_0101:
		case RECONNECT_TWS_0011:
			if(type == RECONNECT_LINK_LOSS)
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 1);
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 0);				
			}
			else
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 0);
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 1);				
			}
			break;

		default:
			break;
	}
}

PUBLIC BOOL MMI_Reconnect_SearchList(U8 dataCnt, U8 XDATA_PTR currentIndexPtr)
{
	if(dataCnt)
	{
		if(*currentIndexPtr == 0xFF)
		{
			*currentIndexPtr = 0;
		}
		else if(*currentIndexPtr == (dataCnt-1))
		{
			*currentIndexPtr = 0xFF;
			return FALSE;
		}
		else
		{
			(*currentIndexPtr)++;
		}

		return TRUE;
	}
	return FALSE;
}

PUBLIC U8 MMI_Reconnect_GetListCnt(U8 isNormalData)
{
	U8 index;
	U8 cnt = 0;
	
	if(!gMMI_ctl.reconnectCtl.reconnectListPtr)
		return 0;

	for(index = 0; index < NVRAM_LINK_KEY_ITEMS; index++)
	{
		if(isNormalData)
		{
			if(gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index].reconnectType != RECONNECT_TYPE_NULL)
			{
				cnt++;
			}
		}
		else
		{
			if(gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index].reconnectType != RECONNECT_TYPE_NULL)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

PUBLIC void MMI_Reconnect_DeleteListByBdAddr(U8 XDATA_PTR bdAddrPtr)
{
	U8 isNormalData = TRUE;
	MMIReonnectDataType XDATA_PTR dataPtr;
	MMIReonnectDataType XDATA_PTR backUpPtr;
	U8 index, i;
	U8 XDATA_PTR currentIndexPtr;


	if(gMMI_ctl.reconnectCtl.reconnectListPtr == (MMIReconnectListType XDATA_PTR)NULL)
		return;

	DeleteInvalidReconnectData:
	index = NVRAM_LINK_KEY_ITEMS;
	if(isNormalData)
		currentIndexPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex;
	else
		currentIndexPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex;

	while(index--)
	{
		if(isNormalData)
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index];
		else
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index];

		if(dataPtr->reconnectType != RECONNECT_TYPE_NULL)
		{
			if(SYS_IsBDAddrIsTheSame(&dataPtr->bdAddr[0], bdAddrPtr))
			{
				goto DELETE_RECONNECT_DATA;
			}

			if(dataPtr->retryCnt)
			{
				continue;
			}
			DELETE_RECONNECT_DATA:
			dataPtr->reconnectType = RECONNECT_TYPE_NULL;
			OSMEM_memset_xdata((U8 XDATA_PTR)dataPtr, 0, sizeof(MMIReonnectDataType));
			if(index <= *currentIndexPtr)
			{
				if(index)
					*currentIndexPtr = *currentIndexPtr-1;
				else
					*currentIndexPtr = 0xFF;
			}
		}
	}

	for(index = 0; index < NVRAM_LINK_KEY_ITEMS ; index++)
	{
		if(isNormalData)
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index];
		else
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index];

		if(dataPtr->reconnectType == RECONNECT_TYPE_NULL)
		{
			for(i = index+1; i < NVRAM_LINK_KEY_ITEMS; i++)
			{
				if(isNormalData)
					backUpPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[i];
				else
					backUpPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[i];

				if(backUpPtr->reconnectType != RECONNECT_TYPE_NULL)
				{
					OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)dataPtr, (U8 XDATA_PTR)backUpPtr, sizeof(MMIReonnectDataType));
					backUpPtr->reconnectType = RECONNECT_TYPE_NULL;
					break;
				}
			}
		}
	}

	if(isNormalData)
	{
		isNormalData= FALSE;
		goto DeleteInvalidReconnectData;
	}
}

PUBLIC void MMI_Reconnect_DeleteInvalidList(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_IsActiveLink(linkIndex))
		{
			MMI_Reconnect_DeleteListByBdAddr(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0]);

			#ifdef AIR_MODULE
			MMI_AIR_Reconnect_DeleteInvalidList(linkIndex);
			#endif
		}
	}
}

PRIVATE MMIReonnectDataType XDATA_PTR MMI_Reconnect_GetTarget(void)
{
	MMIReonnectDataType XDATA_PTR reconectTargetPtr;
	reconectTargetPtr= (MMIReonnectDataType XDATA_PTR)NULL;

	if(MMI_Reconnect_SearchList(MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA), &gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex))
		reconectTargetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex];
	else if(MMI_Reconnect_SearchList(MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA), &gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex))
		reconectTargetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex];
	else if(MMI_Reconnect_SearchList(MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA), &gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex))
		reconectTargetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex];
	else if(MMI_Reconnect_SearchList(MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA), &gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex))
		reconectTargetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex];
	
	return reconectTargetPtr;
}

PRIVATE BOOL MMI_Reconnect_Start(U8 type)
{
	U8 linkIndex;
	MMIReonnectDataType XDATA_PTR reconectTargetPtr;
	
	FIND_AGAIN:

	if(	gMMI_ctl.reconnectCtl.reconnectListPtr == (MMIReconnectListType XDATA_PTR)NULL || MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState()
		||(0 == MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA) && 0 == MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA)))
	{
		goto STOP_RECONNECT;
	}
	
	#ifdef AIR_MODULE
	if((reconectTargetPtr = (MMIReonnectDataType XDATA_PTR)MMI_AIR_Reconnect_GetTarget(type)) != (MMIReonnectDataType XDATA_PTR)NULL)
	{
		goto CHECK_RECONNECT_TARGET_LINK;
	}
	#endif

	if((reconectTargetPtr = MMI_Reconnect_GetTarget()) == (MMIReonnectDataType XDATA_PTR)NULL)
	{
		STOP_RECONNECT:
		MMI_Reconnect_ClearCtl();

		if(MMI_DISCOVER_AFTER_POWERON_RECONNECT_FAIL_FEAT && type == RECONNECT_PWR_ON)
		{
			MMI_SetEnterDiscoverable(TRUE);
		}
		return FALSE;
	}
	
	#ifdef AIR_MODULE
	CHECK_RECONNECT_TARGET_LINK:
	#endif
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_IsActiveLink(linkIndex) && SYS_IsBDAddrIsTheSame(&reconectTargetPtr->bdAddr[0], &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0]))
		{
			if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_LINK_DETACHING)
			{
				if(type == RECONNECT_USER_INIT)
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.userReconnectKeyEventTimer,ONE_SEC);
			}
			else if (MMI_IsActiveLink(linkIndex) && !MMI_GetConnectedProfileCount(linkIndex, TRUE) && MMI_SDAP_IsQueryComplete(linkIndex))
			{
				gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask &= ~MMI_DISC_ACTIVE;
				MMI_PrepareActiveProfileConnection(linkIndex);
			}
			MMI_Reconnect_DeleteInvalidList();
			
			if((reconectTargetPtr = MMI_Reconnect_GetTarget()) == (MMIReonnectDataType XDATA_PTR)NULL)
				goto STOP_RECONNECT;
		}
	}
	
	if(MMI_IsUnderPage())
		return FALSE;
	
	linkIndex = MMI_GetEmptyLinkIndex(&reconectTargetPtr->bdAddr[0], TRUE);
	if(linkIndex >= MAX_MULTI_POINT_NO)
	{
		if(linkIndex == MMI_GET_EMPTY_LINK_SAME_BD_ADDR)
		{
			MMI_Reconnect_DeleteInvalidList();
			MMI_Reconnect_SetDelayTimer(HALF_SEC);
			return FALSE;
		}
		else
			goto STOP_RECONNECT;
	}

	if(reconectTargetPtr->retryCnt)
	{
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("USR - Reconnect:%d, %d",(U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex, (U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex);
		//#endif
		MMI_HCI_SendCreateConnection(	linkIndex,
										(U8 XDATA_PTR)&reconectTargetPtr->bdAddr[0], 
										type,
										(U8 XDATA_PTR)&reconectTargetPtr->classOfDev[0]);
		
		if(reconectTargetPtr->retryCnt != 0xFF)
			reconectTargetPtr->retryCnt--;

		return TRUE;
	}

	MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&reconectTargetPtr->bdAddr[0]);
	goto FIND_AGAIN;
}

PUBLIC BOOL MMI_Reconnect_ToUARTIndex(U8 index)
{
	if(index >= gMMI_nvram.storedLinkKeyCnt)
	{
		index -= gMMI_nvram.storedLinkKeyCnt;
		
		if(index < NVRAM_AIR_LINK_KEY_NO)
		{
			if(MAX_MULTI_POINT_NO == MMI_GetLinkIndexByBdA(&gMMI_driver_variation_nvram.air_link_history_info[index].linkData.bdAddr[0]))
				MMI_Reconnect_AddListByAirLinkKeyIndex(RECONNECT_USER_INIT, index);
			else
				return FALSE;
		}
		else
		{
			MMI_Reconnect_AddListByFastDongle(RECONNECT_USER_INIT);
		}
	}
	else
	{
		if(MAX_MULTI_POINT_NO == MMI_GetLinkIndexByBdA(&gMMI_driver_variation_nvram.link_history_info[index].linkData.bdAddr[0]))
		{
			MMI_Reconnect_AddListByLinkKeyIndex(RECONNECT_USER_INIT, index);
		}
		else
		{
			return FALSE;
		}
	}

	if(MMI_Reconnect_Start(RECONNECT_USER_INIT))
	{
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_Reconnect_ClearCtl(void)
{
	SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gMMI_ctl.reconnectCtl.reconnectListPtr);
}

PUBLIC void MMI_Reconnect_SetDelayTimer(U32 timeValue)
{
	if(gMMI_ctl.reconnectCtl.reconnectListPtr)
	{
		gMMI_ctl.reconnectCtl.reconnectListPtr->isDelayTimerSet = TRUE;
		OST_SetTimer(&gMMI_ctl.reconnectCtl.reconnectListPtr->reConnectDelayTimer, timeValue);
	}
}

PUBLIC void MMI_Reconnect_CheckTimer(void)
{
	if(gMMI_ctl.reconnectCtl.reconnectListPtr)
	{
		if(gMMI_ctl.reconnectCtl.reconnectListPtr->isDurationTimerSet && OST_TimerExpired(&gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectDurationTimer))
		{
			gMMI_ctl.reconnectCtl.reconnectListPtr->isDurationTimerSet = FALSE;
			MMI_Reconnect_ClearCtl();
		}
		if(gMMI_ctl.reconnectCtl.reconnectListPtr->isDelayTimerSet && OST_TimerExpired(&gMMI_ctl.reconnectCtl.reconnectListPtr->reConnectDelayTimer))
		{
			gMMI_ctl.reconnectCtl.reconnectListPtr->isDelayTimerSet = FALSE;
			MMI_Reconnect(RECONNECT_CHECK);
		}
	}
}

PUBLIC void MMI_Reconnect(U8 type)
{
  	// there are 3 stages, add reconnect data, check if reconnected is needed, reconnect
	
	U8 settings;
	#if defined AIR_MODULE && defined PRODUCT_TYPE_K
	MMIReonnectDataType XDATA_PTR reconectTargetPtr;
	#endif

	// Check return condition	
	#ifdef SUPPORT_1TO2AG_SOUNDBAR
		if (CURRENT_ACTIVE_LINK_CNT >= 2
	#else
		if(CURRENT_ACTIVE_LINK_CNT >= gMMI_ctl.currentSupportDeviceNo  
	#endif
		|| MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState()
		|| (!MMI_LinkKey_GetPDLNumber() && gMMI_driver_variation_nvram.fast_dongle_link_history_info.key_status != NVRAM_KEY_OK))
	 {
		MMI_Reconnect_ClearCtl();
		return;
	 }

	if(type > RECONNECT_TYPE_NULL && type < RECONNECT_LINK_LOSS)
	{
		if(gMMI_ctl.reconnectCtl.reconnectListPtr  && gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex != 0xFF)
		{
			if(MMI_STOP_CURRENT_RECONNECT_BY_NEW_RECONNECT_EVT_FEAT)
			{
				OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[0], 0, (sizeof(MMIReonnectDataType)*NVRAM_LINK_KEY_ITEMS));
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex = 0xFF;
			}
			else
				return;
		}
	}

	//add reconnect data
	switch(type)
	{
		case RECONNECT_PWR_ON:
			settings = gMMI_nvram.reConnectInfo.afterPowerOn;	
			goto KEY_RECONNECT;
		case RECONNECT_USER_INIT:
			settings = gMMI_nvram.reConnectInfo.userInitiated;
			
			KEY_RECONNECT:
			if(0 == MMI_LinkKey_GetPDLNumber())
				MMI_Reconnect_AddListByMode(type, RECONNECT_FAST_DONGLE);
			
			#ifdef AIR_MODULE
			MMI_AIR_ReconnectUserInit(type, settings);
			#endif
			MMI_Reconnect_AddListByMode(type, settings);
			break;

		case RECONNECT_LAST_ONCE_AUDIOTRANSFER:
		case RECONNECT_LAST_ONCE_REDIAL:
		case RECONNECT_LAST_ONCE_VOICE_DIAL:
			MMI_Reconnect_AddListByMode(type, RECONNECT_LAST_DISC);
			break;

		case RECONNECT_FIRST_ONCE:
		case RECONNECT_FIRST_ONCE_REDIAL:
		case RECONNECT_FIRST_ONCE_VOICE_DIAL:
			MMI_Reconnect_AddListByMode(type, RECONNECT_AG_IND_0);
			break;

		case RECONNECT_SECONDARY_ONCE:
		case RECONNECT_SECONDARY_ONCE_REDIAL:
		case RECONNECT_SECONDARY_ONCE_VOICE_DIAL:
			MMI_Reconnect_AddListByMode(type, RECONNECT_AG_IND_1);
			break;

		case RECONNECT_LINK_LOSS:
			//#ifdef DBG_UART_ENABLE_MMI
			LightDbgPrint("MMI-ReConn_After OOR:%d,%d",(U8)gMMI_nvram.reConnectInfo.afterLinkLoss, (U8)gMMI_nvram.reConnectInfo.linkLossReTryCnt);
			//#endif
			if( gMMI_nvram.reConnectInfo.afterLinkLoss == RECONNECT_UNNECESSARY)
			{
				OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.linkLossBdAddr, 0, sizeof(BD_ADDR));
				return;
			}
			
			#ifdef AIR_MODULE
			gMMI_ctl.reconnectCtl.linkLossLoopCnt = gMMI_nvram.linkLossLoopCnt;
			if(MMI_AIR_ReconnectLinkLoss((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.linkLossBdAddr))
			{
				OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.linkLossBdAddr, 0, sizeof(BD_ADDR));
				break;
			}
			#endif
			MMI_Reconnect_AddListByBdAddr(type, (U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.linkLossBdAddr);
			OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.reconnectCtl.linkLossBdAddr, 0, sizeof(BD_ADDR));
			break;
		case RECONNECT_WATCHDOG:
			MMI_Reconnect_AddListByMode(type, RECONNECT_ALL_DROPPED_LINKS);
			break;
		case RECONNECT_CHECK:
			type = gMMI_ctl.reconnectCtl.type;
			#if defined AIR_MODULE && defined PRODUCT_TYPE_K
			if(type == RECONNECT_LINK_LOSS)
			{
				reconectTargetPtr = (MMIReonnectDataType XDATA_PTR)MMI_AIR_Reconnect_GetTarget(type);
				if((reconectTargetPtr == (MMIReonnectDataType XDATA_PTR)NULL))
				{;
					//MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&reconectTargetPtr->bdAddr[0]);
					reconectTargetPtr = MMI_Reconnect_GetTarget();
					if((reconectTargetPtr == (MMIReonnectDataType XDATA_PTR)NULL) || reconectTargetPtr->retryCnt == 0)
					{
						if(reconectTargetPtr->retryCnt == 0)
							MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&reconectTargetPtr->bdAddr[0]);
						
						if(gMMI_ctl.reconnectCtl.linkLossLoopCnt == 0)
							break;
						
						MMI_Reconnect_AddListByMode(RECONNECT_LINK_LOSS, gMMI_nvram.reConnectInfo.afterPowerOn);
						MMI_Reconnect_AddListByMode(RECONNECT_USER_INIT, RECONNECT_LAST_ONE);
						//Link Loss Loop Count == 255 is always loop
						if(gMMI_ctl.reconnectCtl.linkLossLoopCnt != 0xFF)
							gMMI_ctl.reconnectCtl.linkLossLoopCnt--;
					}
						
				}

			}
			#endif
			break;
		case RECONNECT_TWS_PAIRING:
			MMI_Reconnect_AddListByMode(type, RECONNECT_TWS);
			break;
		case RECONNECT_TWS_FOLLOW:
			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 1);
			}
			else if( gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 0);						
			}		
			break;
		case RECONNECT_TWS_RELAY:
			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 0);
			}
			else if( gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
			{
				MMI_Reconnect_AddListByAirLinkKeyIndex(type, 1);						
			}		
			break;
	}

	MMI_Reconnect_Start(type);
}

PUBLIC U8 MMI_Reconnect_GetPagingLink(void)
{
	U8 linkIndex;
	
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
			return linkIndex;
	}
	
	return MMI_EOF;
}

PUBLIC void MMI_Reconnect_DisableFunc(void)
{
	gMMI_ctl.reconnectCtl.disableReconnect = TRUE;
}

PUBLIC void MMI_Reconnect_EnableFunc(void)
{
	gMMI_ctl.reconnectCtl.disableReconnect = FALSE;
}

PUBLIC BOOL MMI_Reconnect_FindTarget(U8 XDATA_PTR bdAddr)
{
	U8 isNormalData = TRUE;
	MMIReonnectDataType XDATA_PTR dataPtr;
	U8 index;

	if(gMMI_ctl.reconnectCtl.reconnectListPtr == (MMIReconnectListType XDATA_PTR)NULL)
		return FALSE;

	FIND_TARGET:
	index = NVRAM_LINK_KEY_ITEMS;

	while(index--)
	{
		if(isNormalData)
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[index];
		else
			dataPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[index];

		if(dataPtr->reconnectType != RECONNECT_TYPE_NULL)
		{
			if(SYS_IsBDAddrIsTheSame(&dataPtr->bdAddr[0],bdAddr))
				return TRUE;
		}
	}

	if(isNormalData)
	{
		isNormalData= FALSE;
		goto FIND_TARGET;
	}
	
	return FALSE;
}

//----------------------------------------------------
//------------------- Link History ----------------------
//----------------------------------------------------

PRIVATE U8 MMI_LinkKey_ShiftLinkKeyToTop (U8 targetIndex)
{
	// Just connected and paired
	LinkHistoryType XDATA_PTR tempMemoryPtr;

	if(targetIndex && (tempMemoryPtr = (LinkHistoryType XDATA_PTR)SYS_MemoryGet(sizeof(LinkHistoryType))))
	{
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)tempMemoryPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[targetIndex],sizeof(LinkHistoryType));
		for(; targetIndex > 0; targetIndex--)
		{
			OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[targetIndex], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[targetIndex-1],sizeof(LinkHistoryType));
		}
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0], (U8 XDATA_PTR)tempMemoryPtr, sizeof(LinkHistoryType));
		OSMEM_Put((U8 XDATA_PTR)tempMemoryPtr);

		MMI_SECTOR_SetUpdateFlag(TRUE);
		targetIndex = 0;
	}
	return targetIndex;
}

PUBLIC U8 MMI_LinkKey_ShiftAirLinkKeyToTop (U8 targetIndex)
{
	// Just connected and paired
	LinkHistoryType XDATA_PTR tempMemoryPtr;

	if(targetIndex && (tempMemoryPtr = (LinkHistoryType XDATA_PTR)SYS_MemoryGet(sizeof(LinkHistoryType))))
	{
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)tempMemoryPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[targetIndex],sizeof(LinkHistoryType));
		for(; targetIndex > 0; targetIndex--)
		{
			OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[targetIndex], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[targetIndex-1],sizeof(LinkHistoryType));
		}
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0], (U8 XDATA_PTR)tempMemoryPtr, sizeof(LinkHistoryType));
		OSMEM_Put((U8 XDATA_PTR)tempMemoryPtr);
		MMI_SECTOR_SetUpdateFlag(TRUE);
		targetIndex = 0;
	}
	return targetIndex;
}

PUBLIC BOOL MMI_LinkKey_IsLinkWatchdog(void)
{
	U8 index;
	for(index=0; index < gMMI_nvram.storedLinkKeyCnt; index++)
	{
		if(gMMI_driver_variation_nvram.link_history_info[index].miscMask &NVRAM_LINK_KEY_MISC_MASK_CONNECTED)
		{
			return TRUE;
		}
	}
	for(index=0; index < NVRAM_AIR_LINK_KEY_NO; index++)
	{
		if(gMMI_driver_variation_nvram.air_link_history_info[index].miscMask & NVRAM_LINK_KEY_MISC_MASK_CONNECTED)
		{
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_LinkKey_ClearConnectedFlag(void)
{
	U8 index;
	for(index=0; index <  gMMI_nvram.storedLinkKeyCnt; index++)
	{
		gMMI_driver_variation_nvram.link_history_info[index].miscMask &= (~NVRAM_LINK_KEY_MISC_MASK_CONNECTED);
	}
	
	for(index=0; index <  NVRAM_AIR_LINK_KEY_NO; index++)
	{
		gMMI_driver_variation_nvram.air_link_history_info[index].miscMask &= (~NVRAM_LINK_KEY_MISC_MASK_CONNECTED);
	}
	MMI_SECTOR_SetUpdateFlag(TRUE);
}

PUBLIC U8 MMI_LinkKey_SearchLinkKeyIndex(U8 XDATA_PTR bd_addr, U8 needKeyOk)
{
	U8 i;
	LinkHistoryType XDATA_PTR linkListPtr;

	for (i = 0; i < gMMI_nvram.storedLinkKeyCnt; i++)
	{
		linkListPtr = &gMMI_driver_variation_nvram.link_history_info[i];

		if(SYS_IsBDAddrIsTheSame(&(linkListPtr->linkData.bdAddr[0]), bd_addr))
		{
			if(!needKeyOk)
				break;

			if(linkListPtr->key_status == NVRAM_KEY_OK)
				break;
		}
	}
	if(i ==  gMMI_nvram.storedLinkKeyCnt)
		i = MMI_EOF;

	return i;
}

PUBLIC U8 MMI_LinkKey_SearchAirLinkKeyIndex(U8 XDATA_PTR bd_addr, U8 needKeyOk)
{
	U8 i;
	LinkHistoryType XDATA_PTR linkListPtr;

	for (i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
	{
		linkListPtr = &gMMI_driver_variation_nvram.air_link_history_info[i];

		if(SYS_IsBDAddrIsTheSame(&(linkListPtr->linkData.bdAddr[0]), bd_addr))
		{
			if(!needKeyOk)
				break;

			if(linkListPtr->key_status == NVRAM_KEY_OK)
				break;
		}
	}
	if(i ==  NVRAM_AIR_LINK_KEY_NO)
		i = MMI_EOF;

	return i;
}

PRIVATE U8 MMI_LinkKey_SearchFastDongleLink(U8 XDATA_PTR bd_addr, U8 needKeyOk)
{
	if(SYS_IsBDAddrIsTheSame(&(gMMI_driver_variation_nvram.fast_dongle_link_history_info.linkData.bdAddr[0]), bd_addr))
	{
		if(!needKeyOk)
			return 0;

		if(gMMI_driver_variation_nvram.fast_dongle_link_history_info.key_status == NVRAM_KEY_OK)
			return 0;
	}
	return MMI_EOF;
}

PUBLIC U8 MMI_LinkKey_GetNormalPDLNumber(void)
{
	U8 i;
	U8 linkKeyCount = 0;

	for(i = 0;i < gMMI_nvram.storedLinkKeyCnt;i++)
	{
	 	if (gMMI_driver_variation_nvram.link_history_info[i].key_status == NVRAM_KEY_OK)
			++linkKeyCount;
	}

	return linkKeyCount;
}

PUBLIC U8 MMI_LinkKey_GetPDLNumber(void)
{
	U8 i;
	U8 linkKeyCount = 0;

	for(i = 0;i < gMMI_nvram.storedLinkKeyCnt;i++)
	{
	 	if (gMMI_driver_variation_nvram.link_history_info[i].key_status == NVRAM_KEY_OK)
			++linkKeyCount;
	}
	for(i = 0;i < NVRAM_AIR_LINK_KEY_NO;i++)
	{
	 	if (gMMI_driver_variation_nvram.air_link_history_info[i].key_status == NVRAM_KEY_OK)
			++linkKeyCount;
	}

	return linkKeyCount;
}

PUBLIC U8 MMI_LinkKey_GetAirLinkPDLNumber(void)
{
	U8 i;
	U8 linkKeyCount = 0;

	for(i = 0;i < NVRAM_AIR_LINK_KEY_NO;i++)
	{
	 	if (gMMI_driver_variation_nvram.air_link_history_info[i].key_status == NVRAM_KEY_OK)
			++linkKeyCount;
	}

	return linkKeyCount;
}

PUBLIC void MMI_LinkKey_ClearLinkHistory(U8 index)
{
	MMI_LinkKey_RemoveSequenceLinkHistory((LinkHistoryType XDATA_PTR) &gMMI_driver_variation_nvram.link_history_info, index, FALSE);
}

PUBLIC void MMI_LinkKey_ClearAirLinkHistory(U8 index)
{
	MMI_LinkKey_RemoveSequenceLinkHistory((LinkHistoryType XDATA_PTR) &gMMI_driver_variation_nvram.air_link_history_info, index, TRUE);
}

PUBLIC void MMI_LinkKey_ClearFastDongleLinkHistory(void)
{
	OSMEM_memset_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info, 0, sizeof(LinkHistoryType));
}

PUBLIC void MMI_LinkKey_MoveLinkToAirLink(U8 XDATA_PTR bdAddrPtr)
{
	U8 i = MMI_LinkKey_SearchLinkKeyIndex((U8 XDATA_PTR)bdAddrPtr, FALSE);
	U8 air_i = MMI_LinkKey_SearchAirLinkKeyIndex((U8 XDATA_PTR)bdAddrPtr, FALSE);
	
	if(i == MMI_EOF)
	{
		if(air_i != MMI_EOF && air_i)
		{
			MMI_LinkKey_ShiftAirLinkKeyToTop(air_i);
			MMI_SECTOR_SetUpdateFlag(TRUE);
		}
	}
	else
	{
		if(air_i == MMI_EOF)
		{
			air_i = NVRAM_AIR_LINK_KEY_NO - 1;
			OSMEM_memcpy_xdata_xdata (&gMMI_driver_variation_nvram.air_link_history_info[air_i], &gMMI_driver_variation_nvram.link_history_info[i],  sizeof(LinkHistoryType) /*6 + 16 + 1*/);
			MMI_LinkKey_ShiftAirLinkKeyToTop(air_i);
		}
		else if(air_i)
		{	
			MMI_LinkKey_ShiftAirLinkKeyToTop(air_i);
		}
		MMI_SECTOR_SetUpdateFlag(TRUE);
		MMI_LinkKey_ClearLinkHistory(i);
	}
}

PUBLIC void MMI_LinkKey_DeleteLinkHistory(U8 XDATA_PTR bdAddrPtr)
{
	U8 i = MMI_LinkKey_SearchLinkKeyIndex((U8 XDATA_PTR)bdAddrPtr, FALSE);
	if(i != MMI_EOF)
	{
		MMI_LinkKey_ClearLinkHistory(i);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
}

PUBLIC void MMI_LinkKey_DeleteAirLinkHistory(U8 XDATA_PTR bdAddrPtr)
{
	U8 air_i = MMI_LinkKey_SearchAirLinkKeyIndex((U8 XDATA_PTR)bdAddrPtr, FALSE);
	if(air_i != MMI_EOF)
	{
		MMI_LinkKey_ClearAirLinkHistory(air_i);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
}

PUBLIC LinkHistoryType XDATA_PTR MMI_LinkKey_SearchHistoryByBdAddr(U8 XDATA_PTR bdAddrPtr, BOOL isNeedOk, BOOL isNeedShiftTop)
{
	U8 i;
	
	if((i = MMI_LinkKey_SearchLinkKeyIndex(bdAddrPtr, isNeedOk)) == MMI_EOF)   //cant find
	{
		if((i = MMI_LinkKey_SearchAirLinkKeyIndex(bdAddrPtr, isNeedOk)) == MMI_EOF)
		{
			if((i = MMI_LinkKey_SearchFastDongleLink(bdAddrPtr, isNeedOk)) != MMI_EOF)
			{
				return (&gMMI_driver_variation_nvram.fast_dongle_link_history_info);
			}
		}
		else
		{
			#ifndef PRODUCT_TYPE_K
			if(isNeedShiftTop)
				i = MMI_LinkKey_ShiftAirLinkKeyToTop(i);
			#endif

			return (&gMMI_driver_variation_nvram.air_link_history_info[i]);			
		}
	}
	else
	{
		if(isNeedShiftTop)
			i = MMI_LinkKey_ShiftLinkKeyToTop(i);
			
		return (&gMMI_driver_variation_nvram.link_history_info[i]);	
	}
	return (LinkHistoryType XDATA_PTR)NULL;
}

PRIVATE void MMI_LinkKey_SetConnected(U8 linkIndex)
{
	LinkHistoryType XDATA_PTR linkListPtr= MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], FALSE, TRUE);
	
	if(linkListPtr)
	{
		linkListPtr->miscMask |= NVRAM_LINK_KEY_MISC_MASK_CONNECTED;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
}

PUBLIC void MMI_LinkKey_SetLinkSoundLevelByHistory(U8 linkIndex)
{
	U8 soundLevel, scoSmallStep;
	#ifdef A2DP_Profile
	U8 a2dpSoundLevel, a2dpSmallStep;
	#endif
	LinkHistoryType XDATA_PTR linkListPtr= MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], FALSE, FALSE);
	
	if(linkListPtr == (LinkHistoryType XDATA_PTR)NULL)
	{
		soundLevel = DEFAULT_SCO_SOUND_LEVEL;
		scoSmallStep = DEFAULT_IDLE_SMALL_STEP;
		#ifdef A2DP_Profile
		a2dpSoundLevel = DEFAULT_A2DP_SOUND_LEVEL;
		a2dpSmallStep = 0;	
		#endif
	}
	else
	{
		soundLevel = linkListPtr->sound_level;
		scoSmallStep = linkListPtr->scoSmallStep;
		#ifdef A2DP_Profile
		a2dpSoundLevel = linkListPtr->a2dpSoundLevel;
		a2dpSmallStep = linkListPtr->a2dpSmallSteps;	
		#endif
	}
	
	CURRENT_MIC_SOUND_LEVEL_IN_A2DP(linkIndex) = DEFAULT_MIC_SOUND_LEVEL_IN_A2DP;
	MMI_DRV_SetSCOVolValue(linkIndex, soundLevel, scoSmallStep);
	CURRENT_SCO_VGM(linkIndex) = GET_SCO_VGS_BY_LEVEL(soundLevel);
	#ifdef A2DP_Profile
	MMI_A2DP_SetVolByLevel(linkIndex, (MMI_A2DP_VOLUME_SYNC_HFP_VOLUME_FEAT) ? soundLevel : a2dpSoundLevel, (MMI_A2DP_VOLUME_SYNC_HFP_VOLUME_FEAT) ? scoSmallStep : a2dpSmallStep);
	#endif
}

PUBLIC void MMI_LinkKey_LoadHistory(U8 linkIndex)
{
	MMI_LinkKey_SetConnected(linkIndex);	
}

PUBLIC void MMI_LinkKey_UpdateHistory (U8 XDATA_PTR ptr1, U8 isSuccessful)
{
	U8 i, linkIndex;
	LinkHistoryType XDATA_PTR linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr(ptr1, FALSE, TRUE);
	
	if(isSuccessful)
	{	
		linkIndex = MMI_GetLinkIndexByBdA(ptr1);
		if(linkListPtr ==(LinkHistoryType XDATA_PTR)NULL) // New link
		{
			i = gMMI_nvram.storedLinkKeyCnt - 1; // choose the oldest link
			MMI_LinkKey_ClearLinkHistory(i);
			i = MMI_LinkKey_ShiftLinkKeyToTop(i);
			linkListPtr = &gMMI_driver_variation_nvram.link_history_info[i];
			linkListPtr->sound_level = DEFAULT_SCO_SOUND_LEVEL;
			linkListPtr->scoSmallStep = DEFAULT_IDLE_SMALL_STEP;
			linkListPtr->a2dpSoundLevel = DEFAULT_A2DP_SOUND_LEVEL;
			linkListPtr->a2dpSmallSteps = 0;
			
			if(linkIndex != MAX_MULTI_POINT_NO)
			{
				OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&linkListPtr->classOfDev[0],(U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].linkPara.classOfDev,3);
			}
		}
		linkListPtr->profile = 0;
		OSMEM_memcpy_xdata_xdata (&linkListPtr->linkData, ptr1,  sizeof(HciLinkKeyNtf) /*6 + 16 + 1*/);
		linkListPtr->key_status = NVRAM_KEY_OK;
		if(MMI_GetConnectedProfileCount(linkIndex, TRUE))
		{
			//write flash until it gets connected
			MMI_SECTOR_SetUpdateFlag(TRUE);
		}
	}
	else
	{
		if(linkListPtr !=(LinkHistoryType XDATA_PTR)NULL)
		{
			OSMEM_memset_xdata ((U8 XDATA_PTR)linkListPtr, 0, sizeof(LinkHistoryType));
			MMI_SECTOR_SetUpdateFlag(TRUE);
		}
	}
}

PRIVATE void MMI_LinkKey_SaveHistory(U8 linkIndex, BOOL setDisconnected)
{
	LinkHistoryType XDATA_PTR linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], FALSE, FALSE);
	
	if(linkListPtr !=(LinkHistoryType XDATA_PTR)NULL) // New link
	{
		if (setDisconnected)
		{
			linkListPtr->miscMask &= (~NVRAM_LINK_KEY_MISC_MASK_CONNECTED);
		}
		linkListPtr->sound_level = CURRENT_SCO_SOUND_LEVEL(linkIndex);
		linkListPtr->scoSmallStep = CURRENT_SCO_SMALL_STEP(linkIndex);
		linkListPtr->a2dpSoundLevel = CURRENT_A2DP_SOUND_LEVEL(linkIndex);
		linkListPtr->a2dpSmallSteps =  CURRENT_A2DP_SMALL_STEP(linkIndex);
		MMI_SECTOR_SetUpdateFlag(TRUE);
		#ifdef AIR_MODULE
		MMI_AIR_SaveHistory(linkIndex);
		#endif
	}
}

PUBLIC void MMI_LinkKey_SaveHistoryInRunTime(U8 linkIndex)
{
	MMI_LinkKey_SaveHistory(linkIndex, FALSE);
}

PUBLIC void MMI_LinkKey_SaveHistoryWhenDisconnect(U8 linkIndex)
{
	MMI_LinkKey_SaveHistory(linkIndex, TRUE);
}

//This function is called when new pair devices exist at the same time.
PUBLIC void MMI_LinkKey_UpdateAGInd(U8 linkIndex)
{
	U8 i, nextActiveLink;

	for(i = 0 ; i < MAX_MULTI_POINT_NO; i++)
	{
		if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0],&gMMI_driver_variation_nvram.misc_para.agInd[i].member[0]))
			return;
	}

	//New
	if (CURRENT_ACTIVE_LINK_CNT==1)
	{
		OSMEM_memcpy_xdata_xdata(&gMMI_driver_variation_nvram.misc_para.agInd[0].member[0], &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
		OSMEM_memcpy_xdata_xdata(&gMMI_driver_variation_nvram.misc_para.agInd[1].member[0], &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
	}
	else if(CURRENT_ACTIVE_LINK_CNT ==2 )
	{
		if ((nextActiveLink = MMI_GetNextActiveLink(linkIndex)) != MMI_EOF)
		{
			if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[nextActiveLink].bdAddr[0], &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]))
				OSMEM_memcpy_xdata_xdata(&gMMI_driver_variation_nvram.misc_para.agInd[1].member[0], &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
			else
				OSMEM_memcpy_xdata_xdata(&gMMI_driver_variation_nvram.misc_para.agInd[0].member[0], &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
		}
	}
	MMI_SECTOR_SetUpdateFlag(TRUE);
}

PRIVATE void MMI_LinkKey_RemoveSequenceLinkHistory( LinkHistoryType XDATA_PTR linkListPtr, U8 index, U8 isAirLink)
{
	U8 maxCount;
	if (isAirLink)
		maxCount = NVRAM_AIR_LINK_KEY_NO;
	else
		maxCount = NVRAM_LINK_KEY_ITEMS;
	//Remove Index LinkHistory and Sequqnce LinkHistory
	for( index; index < maxCount - 1; index++)
	{
		OSMEM_memcpy_xdata_xdata ((LinkHistoryType XDATA_PTR)&linkListPtr[index], (U8 XDATA_PTR)&linkListPtr[index + 1], sizeof(LinkHistoryType));
	}
	OSMEM_memset_xdata((U8 XDATA_PTR)&linkListPtr[maxCount - 1], 0, sizeof(LinkHistoryType));
}
