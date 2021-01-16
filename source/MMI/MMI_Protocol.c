#include "reside_flash.inc"

#define _MMI_PROTOCOL_C_

#include "rc.h"
#include "bt_config_profile.h"
#include "os_flash.h"
#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_SDAP.h"
#include "MMI_A2DP.h"
#include "MMI_HFP.h"
#include "MMI_HCI.h"
#include "MMI_AVRCP.h"
#include "MMI_AirModule.h"
#include "MMI_Timer.h"
#include "MMI_Reconnect.h"
#include "MMI_PSM.h"
#include "..\L2CAP\L2CAP_Interface.h"

#include "..\A2DP\A2DP_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"

#ifdef HID_Profile
#include "MMI_HID.h"
#endif

OSMQ XDATA OSMQ_Protocol_Event_To_MMI[MAX_MULTI_POINT_NO];

PUBLIC void MMI_PROTOCOL_Init(void)
{
	if((gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_MULTIPOINTS_ENABLE))
	{
		gMMI_ctl.currentSupportDeviceNo = MMI_MAX_PAGE_DEVICE;
	}
	else
	{
		gMMI_ctl.currentSupportDeviceNo = 1;
	}
}

PUBLIC void MMI_PROTOCOL_EventHandler(void)
{
	U8 linkIndex, entries, passiveConnCnt;
	MMIMsgType XDATA_PTR msgPtr;

	for (linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		passiveConnCnt = 0;
		entries = OSMQ_Entries(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]);
		while(entries--)
		{
			msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]);

			if(MMI_IsActiveLink(linkIndex))
			{
				if(msgPtr->eventCode == PROTOCOL_EVT_TO_MMI_AUTHENTICATION_REQ && msgPtr->msgBodyPtr.protocolAuthReqEvt.isPassive)
				{
					passiveConnCnt++;
				}

				OSMQ_Put(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);

				if(gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].connectionPowerSavingTimer == (OST XDATA_PTR)NULL)
				{
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].connectionPowerSavingTimer, ONE_SEC * 30);
					MMI_PowerSaving(FALSE, MMI_PWRSAVING_PROFILE_CONNECTION, linkIndex);
				}
			}
			else
			{
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
				MMI_PowerSaving(TRUE, MMI_PWRSAVING_PROFILE_CONNECTION, linkIndex);
			}
		}

		if (msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]))
		{
			switch (msgPtr->eventCode)
			{
				case PROTOCOL_EVT_TO_MMI_AUTHENTICATION_REQ:
					if(passiveConnCnt && msgPtr->msgBodyPtr.protocolAuthReqEvt.psm != PSM_SDP && !msgPtr->msgBodyPtr.protocolAuthReqEvt.isPassive) // GetRemote Request
					{
						UpperLayer_SendAuthenticationResult((L2CAP_CHANNEL_HDL)msgPtr->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl, L2CAP_STATUS_CONNECTION_REFUSED_SECURITY_BLOCK);
						break;
					}
					msgPtr->eventCode = PROTOCOL_EVT_TO_MMI_AUTHENTICATION_ONGOING;
					if(msgPtr->msgBodyPtr.protocolAuthReqEvt.isPassive)
					{
						MMI_TIMER_ReleaseDetachTimer(linkIndex);
						gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask &= ~MMI_ACTIVE_CONNECT;
					}
				case PROTOCOL_EVT_TO_MMI_AUTHENTICATION_ONGOING:
					if(passiveConnCnt && msgPtr->msgBodyPtr.protocolAuthReqEvt.psm != PSM_SDP && !msgPtr->msgBodyPtr.protocolAuthReqEvt.isPassive) // GetRemote Request
					{
						UpperLayer_SendAuthenticationResult((L2CAP_CHANNEL_HDL)msgPtr->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl, L2CAP_STATUS_CONNECTION_REFUSED_SECURITY_BLOCK);
						break;
					}
					switch(gMMI_ctl.mmiInfo[linkIndex].linkPara.isEncrypted)
					{
						case ENCRYPTED_OK:
							//Mantis 9926 change role switch timing into profile active
							#if 0
							if(MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT)
							{
								if(MMI_MasterRoleAvailable(linkIndex) && MMI_IsRoleOfSlave(linkIndex))
								{
									msgPtr->eventCode = PROTOCOL_EVT_TO_MMI_ROLE_SWITCH_TO_MASTER_ONGOING;
									OSMQ_PutFront(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
									MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
									SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer, 3*ONE_SEC);
									continue;								
								}
							}
							#endif
							goto ACCEPT_CONNECTION;							
							break;
							
						case ENCRYPTED_ERROR:
							UpperLayer_SendAuthenticationResult((L2CAP_CHANNEL_HDL)msgPtr->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl, L2CAP_STATUS_LOCAL_AUTH_FAIL);
							break;
						case ENCRYPTED_NONE:
							switch (gMMI_ctl.mmiInfo[linkIndex].linkPara.isAuthenticated)
							{
								case AUTH_OK:
									MMI_HCI_SendSetConnectionEncryption(linkIndex);
									break;
								case AUTH_ERROR:
									UpperLayer_SendAuthenticationResult((L2CAP_CHANNEL_HDL)msgPtr->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl, L2CAP_STATUS_LOCAL_AUTH_FAIL);
									break;
								case AUTH_NONE:
									MMI_HCI_SendAuthenticationRequested(linkIndex);
									break;
							}
							/* fallthrough *//*comment to prevent coverity warning*/
						default: //ENCRYPTED_ONGOING
							OSMQ_PutFront(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
							continue;
					}
					break;
					
				case PROTOCOL_EVT_TO_MMI_ROLE_SWITCH_TO_SLAVE_ONGOING:
					if(MMI_IsRoleOfMaster(linkIndex) && !SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer))
					{
						OSMQ_PutFront(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
						continue;								
					}
					goto ACCEPT_CONNECTION;
					break;
					
				case PROTOCOL_EVT_TO_MMI_ROLE_SWITCH_TO_MASTER_ONGOING:
					if(!MMI_IsRoleOfMaster(linkIndex) && !SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer))
					{
						OSMQ_PutFront(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
						continue;								
					}
					ACCEPT_CONNECTION:
					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer);
					UpperLayer_SendAuthenticationResult((L2CAP_CHANNEL_HDL)msgPtr->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl, L2CAP_STATUS_SUCCESS);
					if (msgPtr->msgBodyPtr.protocolAuthReqEvt.isPassive)
					{
						MMI_SDAP_StartServiceSearchProc(linkIndex);
					}					
					break;
			}
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC U8 MMI_GetLinkIndexByConnHandle(U16 connHandle)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle == connHandle||
			(connHandle == gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle && SCO_EXISTS(linkIndex)))
		{
			break;
		}
	}
	return linkIndex;
}

PUBLIC U8 MMI_GetLinkIndexByBdA(U8 XDATA_PTR bdAddr)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(SYS_IsBDAddrIsTheSame(bdAddr, (U8 XDATA_PTR)(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0])))
		{
			break;
		}
	}
	return linkIndex;
}


PUBLIC U8 MMI_GetEmptyLinkIndex(U8 XDATA_PTR bdAddrPtr, BOOL isActive)
{
	U8 linkIndex, activeCount;
	
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0])))
		{
			if(!isActive && MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
				return linkIndex;
			else
				return MMI_GET_EMPTY_LINK_SAME_BD_ADDR;
		}
	}
	
	activeCount = 0;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_CONN_STATE_NONE != MMI_GetConnectionState(linkIndex))
		{
			activeCount++;
		}
	}
	
	if(activeCount < gMMI_ctl.currentSupportDeviceNo)
	{
		for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
		{
			if(MMI_CONN_STATE_NONE == MMI_GetConnectionState(linkIndex))
				return linkIndex;
		}
	}
	
	return MAX_MULTI_POINT_NO;
}

PUBLIC BOOL MMI_CheckProfileOnAllLinks(void)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_GetConnectedProfileCount(linkIndex, TRUE))  // not include SDP
			return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_CancelCreateAcl(void)
{
	U8 linkIndex =  MMI_Reconnect_GetPagingLink();
	
	if(linkIndex != MMI_EOF)
	{
		MMI_HCI_SendCreateConnectionCancel((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
	}

}


PUBLIC MMI_PROFILE_INFO XDATA_PTR MMI_ConnectProfile(U8 linkIndex, U8 profileId)
{
	MMI_PROFILE_DESC CODE_PTR profileDesc;
	MMI_PROFILE_INFO XDATA_PTR profileInfo;
	
	if (MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState() || !MMI_IsActiveLink(linkIndex))
		return (MMI_PROFILE_INFO XDATA_PTR)NULL;
	
	if (profileDesc = MMI_GetProfileDesc(profileId))
	{
		if (profileDesc->ProfileConnectRequest)
		{
			if (profileInfo = profileDesc->ProfileConnectRequest(linkIndex, profileDesc->profileId))
			{
				MMI_LoadProfile(linkIndex, profileInfo, profileDesc);
				MMI_TIMER_ReleaseDetachTimer(linkIndex);
				return profileInfo;
			}
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PUBLIC U8 MMI_DisconnectProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	if(!profileInfo->profileDesc)
	{
		ASSERT(FALSE);
	}
	if (profileInfo->profileDesc->ProfileDisconnectRequest)
	{
		return profileInfo->profileDesc->ProfileDisconnectRequest(linkIndex, profileInfo);
	}
	return CMD_COMPLETE;
}

PUBLIC void MMI_PROTOCOL_RelAllLinks(void)
{
	MMI_UpdateReconnectLinkInfo();
	MMI_ReleaseMultiLink();
	MMI_CancelCreateAcl();
	MMI_Reconnect_ClearCtl();
}

PUBLIC void MMI_PROTOCOL_CreateProfileConnection(U8 linkIndex)
{
	LinkHistoryType XDATA_PTR linkListPtr;
	#ifdef SUPPORT_1TO2AG_SOUNDBAR
	U8 otherLinkIndex;
	#endif
	
	MMI_LinkKey_DeleteAirLinkHistory((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);

	#ifdef SUPPORT_1TO2AG_SOUNDBAR
		otherLinkIndex = MMI_GetNextNormalLink(linkIndex);
		if (otherLinkIndex != MMI_EOF 
			&& MMI_LinkKey_SearchAirLinkKeyIndex((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[otherLinkIndex].bdAddr, TRUE) == MMI_EOF)
		{
			MMI_ReleaseProfileLinkAndDetach(otherLinkIndex);
		}
	#endif
	
	if(!(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT))
	{
		return;
	}
		
	linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], TRUE, FALSE);

	if(linkListPtr != (LinkHistoryType XDATA_PTR)NULL && linkListPtr->miscMask)
	{
	    #ifdef HID_Profile
	    if(linkListPtr->miscMask & NVRAM_LINK_KEY_MISC_MASK_HID)
		{
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HID);
	    }
	    #endif
		if(linkListPtr->miscMask & NVRAM_LINK_KEY_MISC_MASK_HSP)
		{
			if(MMI_FORCE_USE_OF_HSP_CONNECTION_FEAT || !(linkListPtr->miscMask & NVRAM_LINK_KEY_MISC_MASK_HFP))
				MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HEADSET);
			else
				MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HANDSFREE);
			goto MUSIC_CREATE_CONN;
		}
		else if(linkListPtr->miscMask & NVRAM_LINK_KEY_MISC_MASK_HFP)
		{
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HANDSFREE);
			goto MUSIC_CREATE_CONN;
		}
	}
	if(MMI_SDAP_GetRemoteServiceChannel(linkIndex, PROFILE_HANDSFREE) != 0)
	{
		MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HANDSFREE);
	}
	else if(MMI_SDAP_GetRemoteServiceChannel(linkIndex, PROFILE_HEADSET) != 0)
	{
		MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_HEADSET);
	}
	MUSIC_CREATE_CONN:
	MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
	MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
}

PUBLIC BOOL MMI_CreateActiveProfileConnection(U8 linkIndex)
{
	#ifdef AIR_MODULE
	BOOL bResult = TRUE;
	#else
	#define bResult	TRUE
	#endif
	
	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	ConnectProfileSeqCtl XDATA_PTR seqCtl;
	if ((seqCtl = (ConnectProfileSeqCtl XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) == NULL)
		return FALSE;

	MMI_ClearSequentialActiveProfileList(linkIndex);
	
	seqCtl->count = 0;
	seqCtl->index = 0;
	gMMI_ctl.mmiInfo[linkIndex].seqCtl = seqCtl;
	#endif
	
	switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
	#ifdef AIR_MODULE
		#ifdef TWS_SETTINGS
		case PRODUCT_AIR_TWS:
			LightDbgPrint("USR - It is TWS!!!");
			MMI_AIR_CreateConnectionToShare(linkIndex);
			MMI_HCI_SendWriteLinkSuperVisionTimeout(linkIndex,TWS_SUPERVISION_TIMEOUT);
			break;
		#endif
		#if defined SCO_WOOFER || defined SOUND_BAR
		case PRODUCT_AIR_SOUNDBAR:
			LightDbgPrint("USR - It is Soundbar!!!");
			MMI_AIR_CreateConnectionToSoundbar(linkIndex);
			break;
		case PRODUCT_AIR_SCO_WOOFER:
		case PRODUCT_AIR_A2DP_WOOFER:
			LightDbgPrint("USR - It is SCO Woofer!!!");
			if(MMI_AIR_CreateConnectionToWoofer(linkIndex))
			{
				bResult = FALSE;
			}
			break;
		#endif
	#endif
		case PRODUCT_AIR_NONE:
		default:
			LightDbgPrint("USR - It is Normal Phone!!!");
			#ifdef AIR_MODULE
			if(MMI_AIR_SDAPDisconnectedNotify(linkIndex))
			{
				bResult = FALSE;
				break;
			}
			#endif
			
			MMI_PROTOCOL_CreateProfileConnection(linkIndex);
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_ROLE_NONE);

			#ifdef SOUND_BAR
			if(MMI_GetProfileDesc(PROFILE_A2DP))
			{
				if(!MMI_IsProfileIdLoaded(linkIndex, PROFILE_A2DP) && !MMI_IsProfileConnected(linkIndex, PROFILE_A2DP))
					MMI_SetReconnectProfile(linkIndex, PROFILE_A2DP, 2 * ONE_SEC);
			}
			#endif

			if(MMI_GetProfileDesc(PROFILE_AVRCP))
			{
				if(!MMI_IsProfileIdLoaded(linkIndex, PROFILE_AVRCP) && MMI_IsProfileConnected(linkIndex, PROFILE_A2DP))
					MMI_SetReconnectProfile(linkIndex, PROFILE_AVRCP, 2 * ONE_SEC);
			}

			if(MMI_GetProfileDesc(PROFILE_HANDSFREE))
			{
				if(!MMI_IsProfileIdLoaded(linkIndex, PROFILE_HANDSFREE) && !MMI_IsProfileConnected(linkIndex, PROFILE_HANDSFREE))
					MMI_SetReconnectProfile(linkIndex, PROFILE_HANDSFREE, 2 * ONE_SEC);
			}
			break;	
	}

	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	if (seqCtl->count == 0)
	{
		MMI_ClearSequentialActiveProfileList(linkIndex);
	}
	else
	{
		MMI_SequentialActiveProfileConnection(linkIndex);
	}
	#endif
	return bResult;
}

PUBLIC void MMI_PrepareActiveProfileConnection(U8 linkIndex)
{
	//New Feature - "MMI_FORCE_USE_OF_HSP_CONNECTION_FEAT"
	//If the feature is enabled,it keeps the profile of a previously connected AG.
	//So, if an AG connects as HSP, when it's asked to reconnect later, the headset
	//will attempt to start reconnection and use HSP profile.
	MMI_SDAP_StartServiceSearchProc(linkIndex);

	gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_ACTIVE_CONNECT;
	
	#ifdef HFP_Profile
	MMI_HFP_SetReconnectReason(linkIndex, gMMI_ctl.reconnectCtl.type);
	#endif
}

#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
PUBLIC void MMI_AppendActiveProfileConnection(U8 linkIndex, U8 profileId)
{
	ConnectProfileSeqCtl XDATA_PTR seqCtl;
	if (seqCtl = gMMI_ctl.mmiInfo[linkIndex].seqCtl)
	{
		seqCtl->profileId[seqCtl->count++] = profileId;
	}
}

PUBLIC void MMI_ClearSequentialActiveProfileList(U8 linkIndex)
{
	SYS_MemoryRelease(&gMMI_ctl.mmiInfo[linkIndex].seqCtl);
}

PUBLIC void MMI_SequentialActiveProfileConnection(U8 linkIndex)
{
	ConnectProfileSeqCtl XDATA_PTR seqCtl;
	if (seqCtl = gMMI_ctl.mmiInfo[linkIndex].seqCtl)
	{
		while (seqCtl->index < seqCtl->count)
		{
			if (MMI_ConnectProfile(linkIndex, seqCtl->profileId[seqCtl->index++]) != NULL)
				break;
		}
		
		if (seqCtl->index == seqCtl->count)
		{
			MMI_ClearSequentialActiveProfileList(linkIndex);
		}
	}
}
#endif

PUBLIC void MMI_ConnectableStateChangeWhenConnected(void)
{
	if (CURRENT_ACTIVE_LINK_CNT == gMMI_ctl.currentSupportDeviceNo)
	{
		MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
		MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);
	}
	else if (CURRENT_ACTIVE_LINK_CNT > gMMI_ctl.currentSupportDeviceNo)
	{
		ASSERT(FALSE);
	}
	else
	{
		MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
	}
}

PUBLIC void MMI_PageHandler(BOOL isStart)
{
	if (MMI_SUPPORT_RECONN_CLOSE_SCAN_FEAT)
	{
		if(isStart)
		{
			if(CURRENT_ACTIVE_LINK_CNT ==0)
			{
				MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
				MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);
			}
		}
		else
		{
			MMI_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
			MMI_ConnectabilityHandle(CMD_SET_RESUME_CONNECTABLE);
		}
	}
	
	gMMI_ctl.page_ongoing = (isStart)?TRUE:FALSE;
}

PUBLIC void MMI_PageFailHandler(U8 linkIndex)
{
	#ifdef AIR_MODULE
	if(linkIndex!= MMI_EOF)
		MMI_AIR_PageFailHandler(linkIndex);
	#endif

	MMI_PageHandler(FALSE);
	
	if(linkIndex!= MMI_EOF)
	{
		MMI_PushMediaEvent(MEDIA_EVT_RECONNECT_FAILED);
		MMI_ClearMMILinkSettingType(linkIndex);			
	}
	MMI_Reconnect_DeleteInvalidList();
	MMI_Reconnect_SetDelayTimer(HALF_SEC);
}

PUBLIC void MMI_PageOKHandler(U8 linkIndex)
{
	MMI_PageHandler(FALSE);
	MMI_PrepareActiveProfileConnection(linkIndex);
	
	#if 0 //mantis 9183: delay the reconnect success condition until profile connection success
	MMI_Reconnect_DeleteInvalidList();
	MMI_Reconnect_SetDelayTimer((U32)gMMI_nvram.timerCtl.ReConnectDelayTimer* ONE_SEC);
	#endif
}

PUBLIC BOOL MMI_IsUnderPage(void)
{
	return gMMI_ctl.page_ongoing;
}

PUBLIC BOOL MMI_IsLinkActiveConnected(U8 linkIndex)
{
	if(MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex) || gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		return TRUE;

	return FALSE;
}

PUBLIC BOOL MMI_CheckAllLinksInA2DPState(void)
{
#ifdef A2DP_Profile
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MUSIC_EXISTS(linkIndex))
			return TRUE;
	}
#endif
	return FALSE;
}

PUBLIC MMIMsgType XDATA_PTR MMI_PROTOCOL_CommandFactory(U8 opcode)
{
	MMIMsgType XDATA_PTR msgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	
	if(msgPtr == (MMIMsgType XDATA_PTR)NULL)
	{
		ASSERT(FALSE);
	}

	msgPtr->dataOffset = CMD_OFFSET;
	msgPtr->msgOpcode = opcode;

	return msgPtr;
}

PUBLIC BOOL Protocol_SendAuthenticationReqToMMI(L2CAP_CHANNEL_HDL l2capChannelHdl, U16 psm, U8 isPassive)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	MMIMsgType XDATA_PTR ptr1;
	
	if(MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT)
	{
		if(!MMI_IsRoleOfMaster(linkIndex))
		{
			goto SEND_AUTH_REQ;							
		}
	}
	
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.isAuthenticated == AUTH_OK && gMMI_ctl.mmiInfo[linkIndex].linkPara.isEncrypted == ENCRYPTED_OK)
	{
		if(isPassive)
		{
			if(psm != PSM_SDP)
				gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask &= ~MMI_ACTIVE_CONNECT;
			MMI_SDAP_StartServiceSearchProc(linkIndex);
		}
		return TRUE;
	}
	
	SEND_AUTH_REQ:
	ptr1 = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	if(ptr1)
	{
		ptr1->dataOffset	= EVT_OFFSET;
		ptr1->eventCode 	= PROTOCOL_EVT_TO_MMI_AUTHENTICATION_REQ;
		ptr1->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl = l2capChannelHdl;
		ptr1->msgBodyPtr.protocolAuthReqEvt.psm = psm;
		ptr1->msgBodyPtr.protocolAuthReqEvt.isPassive = isPassive;

		OSMQ_Put(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)ptr1);
	}
	return FALSE;
}

PUBLIC void Protocol_CancelAuthenticationReqToMMI(L2CAP_CHANNEL_HDL l2capChannelHdl)
{
	MMIMsgType XDATA_PTR ptr1;
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	U8 i = OSMQ_Entries(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]);
	
	while(i--)
	{
		ptr1 = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]);
		if(ptr1->msgBodyPtr.protocolAuthReqEvt.l2capChannelHdl == l2capChannelHdl)
			OSMEM_Put((U8 XDATA_PTR)ptr1);
		else
			OSMQ_Put(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex], (U8 XDATA_PTR)ptr1);
	}
}

PUBLIC BOOL MMI_IsRoleOfMaster(U8 linkIndex)
{
	return (gMMI_ctl.mmiInfo[linkIndex].linkPara.role == ROLE_MASTER)?TRUE:FALSE;
}

PUBLIC BOOL MMI_IsRoleOfSlave(U8 linkIndex)
{
	return (gMMI_ctl.mmiInfo[linkIndex].linkPara.role == ROLE_SLAVE)?TRUE:FALSE;
}

PUBLIC BOOL MMI_MasterRoleAvailable(U8 linkIndex)
{
	U8 i;
	U8 MasterCount = 2;

	//reserve one master for woofer
	#ifdef AIR_MODULE
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SOUNDBAR && MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER) == MMI_EOF)
	{
		MasterCount = 1;
	}
	#endif
	
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.roleSwitchCnt >= gMMI_nvram.roleSwitchCnt)
		return FALSE;
	
	//if there is available link for master, return true (hardware 2M2L limit)
	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(MMI_IsRoleOfMaster(i))
		{
			if(--MasterCount == 0)
			{
				return FALSE;
			}
		}
	}
	
	#ifdef A2DP_Profile
	if(MMI_A2DP_IsInStreaming(linkIndex) && gMMI_ctl.mmiInfo[linkIndex].linkPara.roleSwitchCnt)
		return FALSE;
	#endif
		
	return TRUE;
}
