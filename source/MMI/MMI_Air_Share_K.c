#ifdef MMI_LE_ENABLE
#include "..\MMI_LE\MMI_LE_interface.h"
#include "..\MMI_LE\link_index_ctl.h"
#endif
#include "..\COMMON\sys_clk_ctl.h"
#include "sector_boundary.h"
#include "..\AirApp\AirApp_Interface.h"
#include "..\Driver\AudioControl.h"
#include "..\Driver\Peq_HPF.h"
#include "MMI_ChgBat.h"
#ifdef SUPPORT_VOICE_COMMAND
#include "..\Driver\VoiceCommand.h"
#endif
#include "MMI_HFP.h"

#include "align_flash.h"

#include "..\MMI_FUNCTION\MMI_Air_Function.h"

#ifdef LIGHTING_ENABLE
extern AirAppLedCtl XDATA gAirAppLedCtl;
extern U8 MusicYouCanSeeEnable;
#endif

enum
{
	STATE_NONE,
	STATE_CHANGE_BDADDR,
	STATE_CHANGE_BDADDR_CC,
	STATE_CHANGE_BDADDR_IDLE,
	STATE_CHANGE_BDADDR_DISCOVERABLE,
	STATE_CHANGE_BDADDR_FOLLOW,

};

enum
{
	ACTION_CHANGE_BD_ADDR_PHONE = 1,
	ACTION_CHANGE_BD_ADDR_CC,
	ACTION_BD_ADDR_OK,
	ACTION_BD_ADDR_FAIL,
	ACTION_CHANGE_BD_ADDR_IDLE,
	ACTION_RESUME_BD_ADDR_PAGE_FAIL,
	ACTION_ENTER_DISCOVERABLE,
	ACTION_CHANGE_TO_FOLLOW,
};

typedef struct
{
	U8 bdAddr[6];
	U8 cod[3];
	U8 type;
}Reconnect_Data_Ctl;

enum
{
	PHONE_STATE_NONE,
	PHONE_STATE_INCOMING = 1,
	PHONE_STATE_MUSIC,
	PHONE_STATE_CHANGE_FOLLOWER,

	STATE_CHANGE_TO_RELAYER = 0x80,
};

typedef struct
{
	OST XDATA_PTR relayPowerOffTimer;
	U8 phoneBdAddr[6];
	OST XDATA_PTR resumeStateTimer;
	U8 resumeState;
	PhoneNumber incomingCall;
}ReconnectPhoneInfoCtl;

typedef struct
{
	U8 currBdAddr[6];
	U8 isChannelMediaPushed:1;
	U8 getRelayVPOverNotify:1;
	U8 getVolKeyFromTWS:1;
	U8 isPhoneConnected:1;
	U8 state:4;
	OST XDATA_PTR powerOffTimer;
	OST XDATA_PTR resumeLEDTimer;
	U8 sentInfo:1;
	U8 isLightMonoMode:1;
	U8 reserved:6;
	U8 relayerState;
	Reconnect_Data_Ctl	ccCtl;
	OST XDATA_PTR changeLocalBdAddrTimer;
	ReconnectPhoneInfoCtl phoneInfo;
	U8 bgLEDIndex;
	OST XDATA_PTR stayFollowerTimer;
	OST XDATA_PTR batMonitorTimer;
}MMI_AIR_SHARE_K_TYPE;

MMI_AIR_SHARE_K_TYPE XDATA gMMI_Share_K_ctl;

PRIVATE BOOL MMI_Share_CompareXdataCode(U8 XDATA_PTR ptr1, U8 CODE_PTR ptr2, U16 length)
{
	U16 i;

	for(i = 0; i < length ; i++)
	{
		if(ptr1[i] != ptr2[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_SendAirohaL2capData(U8 linkIndex, U8 XDATA_PTR dataPtr, U16 dataLen)
{
	L2CAP_SendAirohaChannelData(linkIndex, dataPtr, dataLen);
}

PUBLIC BOOL MMI_Share_IsDeviceToPhone(void)
{
	U8 CODE_PTR pMpBdAddr = (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr;

	LightDbgPrint("==IsDeviceToPhone:%x,%x",(U8)gMMI_Share_K_ctl.currBdAddr[0], (U8)pMpBdAddr[0]);

	if(!MMI_Share_CompareXdataCode(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6))
	{
		return (gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT) ? TRUE : FALSE;
	}
	else
	{
		return (gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT) ? FALSE : TRUE;
	}
}

PUBLIC void MMI_Share_LocalBdAddrHandler(U8 action)
{
	U8 CODE_PTR pMpBdAddr = (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr;

	LightDbgPrint("==LocalBdAddrHandler:%d,%d",(U8)action, (U8)gMMI_Share_K_ctl.state);
	switch(action)
	{
		case ACTION_BD_ADDR_FAIL:
			if(!MMI_Share_CompareXdataCode(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6))
			{
				OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
			}
			else
			{
				OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
			}

			if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR)
			{
				LightDbgPrint("==reconnect Pending:bd addr Fail");
				gMMI_Share_K_ctl.state = STATE_NONE;
				SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 2 * ONE_SEC);
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_CC)
			{
				LightDbgPrint("==CC Pending:bd addr Fail");
				gMMI_Share_K_ctl.state = STATE_NONE;
				MMI_PageFailHandler(MMI_EOF);
				OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_Share_K_ctl.ccCtl, 0, sizeof(Reconnect_Data_Ctl));
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_IDLE)
			{
				gMMI_Share_K_ctl.state = STATE_NONE;
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_DISCOVERABLE)
			{
				MMI_GAP_WriteDiscoverabilityMode(SAC_NON_DISCOVERABLE_MODE| NON_DISCOVERABLE_MODE);
				gMMI_Share_K_ctl.state = STATE_NONE;
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_FOLLOW)
			{
				gMMI_Share_K_ctl.state = STATE_NONE;
			}
			break;

		case ACTION_BD_ADDR_OK:
			/* [Start]for QA debug print. Please do not delete these */
			if(MMI_Share_IsDeviceToPhone())
			{
				LightDbgPrint("TWSK - Role: Relayer");
				#ifdef MMI_LE_ENABLE
				if(!LIC_LE_GetNumOfActiveLink())
				{
					MMI_LE_EnterAdvertising();
				}
				#endif
			}
			else
			{
				LightDbgPrint("TWSK - Role: Follower");
				#ifdef MMI_LE_ENABLE
				MMI_LE_DisableBleLink();
				#endif
			}
			/* [End]for QA debug print. Please do not delete these */
			if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR)
			{
				LightDbgPrint("==reconnect To start:bd addr OK");
				MMI_Reconnect(RECONNECT_USER_INIT);
				gMMI_Share_K_ctl.state = STATE_NONE;

				LightDbgPrint("==reco3:bd addr :%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
				if(gMMI_Share_K_ctl.phoneInfo.resumeState & STATE_CHANGE_TO_RELAYER)
				{
					MMI_Reconnect(RECONNECT_TWS_FOLLOW);
					gMMI_Share_K_ctl.phoneInfo.resumeState &= ~STATE_CHANGE_TO_RELAYER;
					LightDbgPrint("==reco4:bd addr :%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
				}
				SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 8 * ONE_SEC);
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_CC)
			{
				U8 linkIndex;
				LightDbgPrint("==CC To start:bd addr OK");

				gMMI_Share_K_ctl.state = STATE_NONE;
				if((linkIndex = MMI_GetEmptyLinkIndex(gMMI_Share_K_ctl.ccCtl.bdAddr, TRUE)) < MAX_MULTI_POINT_NO)
				{
					MMI_HCI_SendCreateConnection (	linkIndex,
													gMMI_Share_K_ctl.ccCtl.bdAddr,
													gMMI_Share_K_ctl.ccCtl.type,
													&gMMI_Share_K_ctl.ccCtl.cod[0]);
				}
				else
				{
					MMI_PageFailHandler(MMI_EOF);
				}
				OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_Share_K_ctl.ccCtl, 0, sizeof(Reconnect_Data_Ctl));
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_IDLE)
			{
				gMMI_Share_K_ctl.state = STATE_NONE;
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_DISCOVERABLE)
			{
				MMI_GAP_WriteDiscoverabilityMode(gMMI_ctl.discoverableState);
				gMMI_Share_K_ctl.state = STATE_NONE;
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_FOLLOW)
			{
				gMMI_Share_K_ctl.state = STATE_NONE;
				MMI_Reconnect(RECONNECT_TWS_RELAY);
			}
			break;

		case ACTION_CHANGE_BD_ADDR_PHONE:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				if(!MMI_Share_IsDeviceToPhone())
				{
					if(MMI_IsActiveLink(0) || MMI_IsActiveLink(1) || MMI_EOF != MMI_Reconnect_GetPagingLink())
					{
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 320L);
						break;
					}
					if(!MMI_LinkKey_GetAirLinkPDLNumber())
					{
						break;
					}
					if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
					{
						LightDbgPrint("==Change BdADdr Right to Left");
						OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
					}
					else
					{
						LightDbgPrint("==Change BdADdr Right to Left");
						OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
					}
					MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
					gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR;
				}
				else
				{
					MMI_Reconnect(RECONNECT_USER_INIT);
					LightDbgPrint("==reco:bd addr :%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
					if(gMMI_Share_K_ctl.phoneInfo.resumeState & STATE_CHANGE_TO_RELAYER)
					{
						MMI_Reconnect(RECONNECT_TWS_FOLLOW);
						LightDbgPrint("==reco2:bd addr :%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
						gMMI_Share_K_ctl.phoneInfo.resumeState &= ~STATE_CHANGE_TO_RELAYER;
					}
				}
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_IDLE)
			{
				if(MMI_Share_IsDeviceToPhone())
				{
					gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR;
				}
				else
				{
					SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 320L);
				}
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_CC)
			{
				SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 320L);
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_DISCOVERABLE)
			{
				SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 3*32L);
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_FOLLOW)
			{
				SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer, 320L);
			}
			break;

		case ACTION_CHANGE_BD_ADDR_CC:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				U8 XDATA_PTR bdAddrPtr = &gMMI_Share_K_ctl.ccCtl.bdAddr[0];

				if(	SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
					SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
				{
					if(SYS_IsBDAddrIsTheSame(bdAddrPtr, &gMMI_Share_K_ctl.currBdAddr[0]))
					{
						if(!MMI_Share_CompareXdataCode(gMMI_Share_K_ctl.currBdAddr, pMpBdAddr, 6))
						{
							if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
								LightDbgPrint("==Change to Left:%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0]);
							else
								LightDbgPrint("==Change to Right:%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0]);

							OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
						}
						else
						{
							if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
								LightDbgPrint("==Change to Right 1:%x",(U8)pMpBdAddr[0]);
							else
								LightDbgPrint("==Change to Left 1:%x", (U8)pMpBdAddr[0]);

							OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
						}
						goto SEND_LOCAL_BDADDR_CMD;
					}
				}
				else
				{
					if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
					{
						if(MMI_Share_CompareXdataCode(gMMI_Share_K_ctl.currBdAddr, pMpBdAddr, 6))
						{
							LightDbgPrint("==Change to Left 3:%x",(U8)pMpBdAddr[0]);
							OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
							goto SEND_LOCAL_BDADDR_CMD;
						}
					}
					else
					{
						if(!SYS_IsBDAddrIsTheSame(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr))
						{
							LightDbgPrint("==Change to Left2:%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0]);
							OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
							goto SEND_LOCAL_BDADDR_CMD;
						}
					}
				}
				break;

				SEND_LOCAL_BDADDR_CMD:
				MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
				gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_CC;
			}
			else if(gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR || gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_IDLE ||
					gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_DISCOVERABLE || gMMI_Share_K_ctl.state == STATE_CHANGE_BDADDR_FOLLOW)
			{
				LightDbgPrint("==reconnect To CC !!!!!");
				gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_CC;
				SYS_ReleaseTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer);
			}
			break;

		case ACTION_CHANGE_BD_ADDR_IDLE:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				U8 linkIndex;
				for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
				{
					if(MMI_CONN_STATE_NONE != MMI_GetConnectionState(linkIndex))
						return;

					if(MMI_CONNECTABLE != MMI_GetTopStateByLink(linkIndex))
						return;
				}

				LightDbgPrint("==BD_ADDR_IDLE:%d,%d,%d,%d",(U8)MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA), (U8)MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA)
				,(U8)MMI_Reconnect_GetPagingLink(), (U8)gMMI_ctl.discoverableState);

				if(	MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA) ||
					MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA) ||
					MMI_EOF != MMI_Reconnect_GetPagingLink() ||
					!MMI_LinkKey_GetAirLinkPDLNumber()||
					gMMI_ctl.discoverableState != (NON_DISCOVERABLE_MODE | SAC_NON_DISCOVERABLE_MODE))
					return;

				if(gMMI_Share_K_ctl.phoneInfo.resumeState == PHONE_STATE_CHANGE_FOLLOWER ||
					gMMI_Share_K_ctl.stayFollowerTimer)
					return;

				LightDbgPrint("==2CHANGE_BD_ADDR_IDLE");

				if(!MMI_Share_CompareXdataCode(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6))
				{
					OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
				}
				else
				{
					OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
				}
				LightDbgPrint("==Idle Change BdADdr: %x",(U8)gMMI_Share_K_ctl.currBdAddr[0]);
				MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
				gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_IDLE;
			}
			break;
		case ACTION_RESUME_BD_ADDR_PAGE_FAIL:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				if(gMMI_ctl.discoverableState == (SAC_NON_DISCOVERABLE_MODE| NON_DISCOVERABLE_MODE) &&
					MMI_Share_CompareXdataCode(gMMI_Share_K_ctl.currBdAddr, pMpBdAddr, 6))
				{
					OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
					MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
					gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_IDLE;
				}
			}
			break;
		case ACTION_ENTER_DISCOVERABLE:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
				{
					if(MMI_Share_CompareXdataCode(gMMI_Share_K_ctl.currBdAddr, pMpBdAddr, 6))
					{
						LightDbgPrint("==Change to Left Discover:%x",(U8)pMpBdAddr[0]);
						OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
						MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
						gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_DISCOVERABLE;
					}
				}
				else
				{
					if((gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_MP_TEST_MODE_ENABLE) || MMI_CheckBootFlag(SYS_SW1_BOOT_FLAG_ENTER_MP_TEST_MODE))
					{
						//Under DUT mode, don't change BD addr
						LightDbgPrint("==Keep Addr at Discover:");
					}
					else if(!SYS_IsBDAddrIsTheSame(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr))
					{
						LightDbgPrint("==Change to Discover:%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0]);
						OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
						MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
						gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_DISCOVERABLE;
					}
				}
			}
			break;
		case ACTION_CHANGE_TO_FOLLOW:
			if(gMMI_Share_K_ctl.state == STATE_NONE)
			{
				if(MMI_Share_IsDeviceToPhone())
				{
					if(MMI_EOF != MMI_Reconnect_GetPagingLink() || !MMI_LinkKey_GetAirLinkPDLNumber())
					{
						break;
					}

					if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
					{
						LightDbgPrint("==Change BdADdr Left to Right");
						OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr, 6);
					}
					else
					{
						LightDbgPrint("==Change BdADdr Left to Right");
						OSMEM_memcpy_xdata_code(&gMMI_Share_K_ctl.currBdAddr[0], pMpBdAddr, 6);
					}
					MMI_HCI_SendVCmdWriteLocalBdAddr(&gMMI_Share_K_ctl.currBdAddr[0]);
					gMMI_Share_K_ctl.state = STATE_CHANGE_BDADDR_FOLLOW;
					SYS_SetTimer(&gMMI_Share_K_ctl.stayFollowerTimer, ONE_MIN);
				}
				else
				{
					MMI_Reconnect(RECONNECT_TWS_RELAY);
				}
			}
			break;
	}
}

PUBLIC void MMI_Share_CheckChangeBdaddrTimer(void)
{
	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.changeLocalBdAddrTimer))
	{
		MMI_Share_LocalBdAddrHandler(ACTION_CHANGE_BD_ADDR_IDLE);
		SYS_SetTimer(&gMMI_Share_K_ctl.changeLocalBdAddrTimer, (U32)gMMI_nvram.twsKIdleChangeBdaddrTime*320L);
	}
}


PUBLIC AIROHA_L2CAP_PKT_STRU XDATA_PTR MMI_Share_GetL2capCmd(U8 pktType, U16 opCode, U8 payloadLength)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr = (AIROHA_L2CAP_PKT_STRU XDATA_PTR)SYS_MemoryGet(payloadLength + 4);
	if(ptr)
	{
		ptr->pktType = pktType;
		ptr->opCode = opCode;
		ptr->payloadLength = payloadLength;
	}
	return ptr;
}

PRIVATE void MMI_Share_SendAudioChannel(U8 linkIndex, U8 channel)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_AUDIO_CHANNEL, sizeof(AIROHA_L2CAP_CMD_AUDIO_CHANNEL_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==MMI_Share_SendAudioChannel:%d",(U8)channel);
		ptr->para.audioChannel.channel = channel;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_AUDIO_CHANNEL_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_SendPhoneData(U8 linkIndex)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	U8 payloadLength = sizeof(AIROHA_L2CAP_CMD_PHONE_LINKKEY_PARA_STRU)-1 + sizeof(LinkHistoryType) * gMMI_nvram.storedLinkKeyCnt;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_PHONE_LINKKEY, payloadLength);
	if(ptr)
	{
		LightDbgPrint("==MMI_Share_SendPhoneData!!");
		ptr->para.phoneLinkKey.lineInSoundLevel = DEFAULT_LINEIN_SOUND_LEVEL;
		ptr->para.phoneLinkKey.lineInSmallStep = DEFAULT_LINEIN_SMALL_STEP_LEVEL;
		ptr->para.phoneLinkKey.storedLinkKeyCnt = gMMI_nvram.storedLinkKeyCnt;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.phoneLinkKey.linkKeyDataBody, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0], sizeof(LinkHistoryType) * gMMI_nvram.storedLinkKeyCnt);
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, payloadLength + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_SendPhoneStatus(U8 linkIndex, U8 status, U8 phoneLink)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	if(!MMI_Share_IsDeviceToPhone())
		return;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_PHONE_STATUS, sizeof(AIROHA_L2CAP_CMD_PHONE_STATUS_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==MMI_Share_SendPhoneStatus:%d",(U8)status);
		ptr->para.phoneStatus.status = status;
		ptr->para.phoneStatus.scoSoundLevel = CURRENT_SCO_SOUND_LEVEL(phoneLink);
		ptr->para.phoneStatus.scoSmallStep = CURRENT_SCO_SMALL_STEP(phoneLink);
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_PHONE_STATUS_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE U8 MMI_Share_SendRoleChange(U8 linkIndex, U8 isPowerOff)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	U8 normalLinkIndex;

	if(!MMI_Share_IsDeviceToPhone())
		return MAX_MULTI_POINT_NO;

	normalLinkIndex = MMI_GetNextNormalLink(linkIndex);
	if(MMI_EOF != normalLinkIndex)
	{
		MMI_A2DP_StopDSP(normalLinkIndex);
		MMI_ReleaseProfileLinkAndDetach(normalLinkIndex);
	}

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_ROLE_CHANGE, sizeof(AIROHA_L2CAP_CMD_ROLE_CHANGE_PARA_STRU));
	if(ptr)
	{
		ptr->para.roleChange.isPhoneConnected = (MMI_EOF == normalLinkIndex)?FALSE:TRUE;
		ptr->para.roleChange.isPhoneMusic 	= (MMI_EOF != normalLinkIndex && MMI_A2DP_IsInStreaming(normalLinkIndex))?TRUE:FALSE;
		ptr->para.roleChange.isPhoneIncoming 	= (MMI_EOF != normalLinkIndex && MMI_HFP_INCOMMING == MMI_GetTopStateByLink(normalLinkIndex))? TRUE:FALSE;
		if(ptr->para.roleChange.isPhoneIncoming)
		{
			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ptr->para.roleChange.phoneNumber, (U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[normalLinkIndex].incomingCall, sizeof(PhoneNumber));
		}
		else
		{
			OSMEM_memset_xdata((U8 XDATA_PTR)&ptr->para.roleChange.phoneNumber, 0, sizeof(PhoneNumber));
		}
		if(ptr->para.roleChange.isPhoneConnected)
		{
			ptr->para.roleChange.a2dpSoundLevel = CURRENT_A2DP_SOUND_LEVEL(normalLinkIndex);
			ptr->para.roleChange.a2dpSmallStep = CURRENT_A2DP_SMALL_STEP(normalLinkIndex);
			ptr->para.roleChange.scoSoundLevel = CURRENT_SCO_SOUND_LEVEL(normalLinkIndex);
			ptr->para.roleChange.scoSmallStep = CURRENT_SCO_SMALL_STEP(normalLinkIndex);
		}

		ptr->para.roleChange.isPowerOff = isPowerOff;
		LightDbgPrint("==SendRoleChange:%d,%d,%x,%d",(U8)ptr->para.roleChange.isPhoneConnected,(U8)ptr->para.roleChange.isPhoneMusic,(U8)ptr->para.roleChange.isPhoneIncoming);
		LightDbgPrint("==SendRoleChange2:%d,%d,%x,%d",(U8)ptr->para.roleChange.a2dpSoundLevel,(U8)ptr->para.roleChange.a2dpSmallStep,(U8)ptr->para.roleChange.scoSoundLevel, (U8)ptr->para.roleChange.scoSmallStep);

		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_ROLE_CHANGE_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);

		return linkIndex;
	}
	return MAX_MULTI_POINT_NO;
}

PUBLIC void MMI_Share_SendRoleChangeResp(U8 linkIndex, U8 isPowerOff)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_RESP_ROLE_CHANGE, sizeof(AIROHA_L2CAP_RESP_ROLE_CHANGE_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==SendPowerOffResp:%d",(U8)linkIndex);

		ptr->para.respRoleChange.isPowerOff = isPowerOff;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_RESP_ROLE_CHANGE_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_SendTwsLinkKey(U8 linkIndex)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	if(!MMI_Share_CompareXdataCode((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr, 6))
	{
		return;
	}

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_TWS_LINKKEY, sizeof(AIROHA_L2CAP_CMD_TWS_LINKKEY_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==SendTwsLinkKey!!!!:%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0]);
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.twsLinkKey, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0], sizeof(LinkHistoryType));
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_TWS_LINKKEY_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_SendStateSync(U8 linkIndex, U8 state)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_STATE_SYNC, sizeof(AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU));
	if(ptr)
	{
		ptr->para.stateSync.state = state;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendStateSync:%x,%x",(U8)linkIndex, (U8)state);
	}
}

PRIVATE void MMI_Share_SendKeyAction(U8 linkIndex, U8 keyIndex, U8 keyAction)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_KEY_ACTION, sizeof(AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU));
	if(ptr)
	{
		ptr->para.keyAction.keyIndex = keyIndex;
		ptr->para.keyAction.keyAction = keyAction;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendKeyAction:%x,%x, %x",(U8)linkIndex, (U8)keyIndex, (U8)keyAction);
	}
}


PRIVATE void MMI_Share_SendBGLed(U8 linkIndex, U8 ledDataIndex)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_BG_LED, sizeof(AIROHA_L2CAP_CMD_BG_LED_PARA_STRU));
	if(ptr)
	{
		ptr->para.bgLed.ledIndex = ledDataIndex;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_BG_LED_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendBGLed:%x,%x",(U8)linkIndex, (U8)ledDataIndex);
	}
}

PUBLIC void MMI_Share_SendBGLedCmd(U8 ledDataIndex)
{
	U8 airlink;
	gMMI_Share_K_ctl.bgLEDIndex = ledDataIndex;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkRelayer(airlink))
	{
		MMI_Share_SendBGLed(airlink, ledDataIndex);
	}
}

PRIVATE void MMI_Share_SendBat(U8 linkIndex, U8 batInPercent)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REPORT_BAT, sizeof(AIROHA_L2CAP_CMD_REPORT_BAT_STRU));
	if(ptr)
	{
		ptr->para.batReport.batInPercent = batInPercent;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_REPORT_BAT_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendBatInpercent:%x,%x ",(U8)linkIndex, (U8)batInPercent);
	}
}

PUBLIC void MMI_Share_SetPEQParaCmd(U8 linkIndex, U8 peqMode, U8 peqModeIndex, U8 sectorMode)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_SET_PEQ_PARA, sizeof(AIROHA_L2CAP_CMD_PEQ_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==SendPEQPara:%d,%d,%d", (U8)peqMode, (U8)peqModeIndex, (U8)sectorMode);
		ptr->para.peqPara.peqMode = peqMode;
		ptr->para.peqPara.peqModeIndex = peqModeIndex;
		ptr->para.peqPara.sectorMode = sectorMode;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr,  sizeof(AIROHA_L2CAP_CMD_PEQ_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PUBLIC void MMI_Share_KeyChangePEQCmd(U8 linkIndex)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_PEQ_KEY_CHANGE, 0);
	if(ptr)
	{
		LightDbgPrint("==SendKeyChangePEQ:%d", (U8)linkIndex);
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PUBLIC void MMI_Share_RealtimePEQCmd(U8 linkIndex, U8 XDATA_PTR para, U8 enable_mode)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	switch(enable_mode)
	{
		case PEQ_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_CONFIG_SECTOR) + 1);
			break;
		case HPF_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(HPF_CONFIG_SECTOR) + 1);
			break;
		#ifdef PEQ_EXPANSION_ENABLE
		case PEQ_EXPANSION_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_CONFIG_SECTOR) + 1);
			break;
		#endif
		#ifdef PEQ_UNDER_100HZ_ENABLE
		case PEQ_UNDER_100HZ_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_100HZ_SECTOR) + 1);
			break;
		#endif
		case REALTIME_PEQ_HPF_SET_END:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, 1);
			break;
		default:
			ptr = (U8 XDATA_PTR)NULL;
			break;
	}

	if(ptr)
	{
		switch(enable_mode)
		{
			case PEQ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peq,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQ:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_CONFIG_SECTOR) + 1 + 4);
				break;
			case HPF_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.hpf,
										  (U8 XDATA_PTR)para,
										  sizeof(HPF_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = HPF_ENABLED_MODE;
				LightDbgPrint("==SendRealtimeHPF:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(HPF_CONFIG_SECTOR) + 1 + 4);
				break;
			#ifdef PEQ_EXPANSION_ENABLE
			case PEQ_EXPANSION_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peqExpansion,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_EXPANSION_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQExpansion:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_CONFIG_SECTOR) + 1 + 4);
				break;
			#endif
			#ifdef PEQ_UNDER_100HZ_ENABLE
			case PEQ_UNDER_100HZ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peqUnder100Hz,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_100HZ_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_UNDER_100HZ_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQUnder100Hz:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_100HZ_SECTOR) + 1 + 4);
				break;
			#endif
			case REALTIME_PEQ_HPF_SET_END:
				ptr->para.realtimePEQPara.enable_mode = REALTIME_PEQ_HPF_SET_END;
				LightDbgPrint("==SendRealtimeEnd:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, 1 + 4);
				break;
		}

		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_RealTimePEQParaHandler(AIROHA_L2CAP_PKT_STRU XDATA_PTR dataPtr)
{
	DriverMsg XDATA_PTR paraPtr;
	paraPtr = OSMEM_Get(OSMEM_ptr2_small_TX);

	if(paraPtr)
	{
		switch(dataPtr->para.realtimePEQPara.enable_mode)
		{
			case PEQ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peq,
										  sizeof(PEQ_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_ENABLED_MODE;
				break;
			case HPF_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.hpf,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.hpf,
										  sizeof(HPF_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = HPF_ENABLED_MODE;
				break;
			#ifdef PEQ_EXPANSION_ENABLE
			case PEQ_EXPANSION_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_expansion,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peqExpansion,
										  sizeof(PEQ_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_EXPANSION_ENABLED_MODE;
				break;
			#endif
			#ifdef PEQ_UNDER_100HZ_ENABLE
			case PEQ_UNDER_100HZ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_under100Hz,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peqUnder100Hz,
										  sizeof(PEQ_100HZ_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_UNDER_100HZ_ENABLED_MODE;
				break;
			#endif
			case REALTIME_PEQ_HPF_SET_END:
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = REALTIME_PEQ_HPF_SET_END;
				break;
		}
	}

	MMI_DRV_RealTimeSetPeqHpf((MMIMsgType XDATA_PTR)paraPtr);
}


U8 XDATA cnt = 0;

PUBLIC void MMI_Share_AirohaL2capHandler(U8 linkIndex, U8 XDATA_PTR ptr)
{
	U8 i;

	AIROHA_L2CAP_PKT_STRU XDATA_PTR dataPtr = (AIROHA_L2CAP_PKT_STRU XDATA_PTR)(ptr + *ptr + L2CAP_HEADER_LEN); //L2CAP data start

	if(dataPtr->pktType == AIROHA_L2CAP_CMD_TYPE)
	{
		LightDbgPrint("Share_Airoha:%X",(U32)dataPtr->opCode);
		switch(dataPtr->opCode)
		{
			case AIROHA_L2CAP_CMD_AUDIO_CHANNEL:
				LightDbgPrint("AUDIO_CHANNEL:%d, %d",(U8)dataPtr->para.audioChannel.channel, (U8)gMMI_nvram.twsKChannel);
				if((dataPtr->para.audioChannel.channel == NVRAM_PROFILE_LEFT && gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
				|| (dataPtr->para.audioChannel.channel == NVRAM_PROFILE_RIGHT && gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT) )
				{
					if(MMI_Share_CompareXdataCode((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr, 6))
					{
						MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
						i = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE);
						LightDbgPrint("==AirLinkKeyIndex:%d",(U8)i);
						if(!gMMI_driver_variation_nvram.air_link_history_info[i].profile)
						{
							gMMI_driver_variation_nvram.air_link_history_info[i].profile = dataPtr->para.audioChannel.channel;
						}
						MMI_SECTOR_SetUpdateFlag(TRUE);
					}

					if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
					{
						MMI_ConnectProfile(linkIndex, PROFILE_AVRCP);
						MMI_ConnectProfile(linkIndex, PROFILE_A2DP);
					}
					else
					{
						MMI_Share_SendAudioChannel(linkIndex, gMMI_nvram.twsKChannel);
						MMI_SDAP_SetRemoteApplicationID(linkIndex, PRODUCT_AIR_TWS);
					}

					if(MMI_Share_IsDeviceToPhone())
					{
						MMI_Share_SendPhoneData(linkIndex);
					}
				}
				else
				{
					MMI_ReleaseProfileLinkAndDetach (linkIndex);
					if ((i = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) != MMI_EOF)
					{
						MMI_LinkKey_ClearAirLinkHistory(i);
						MMI_SECTOR_SetUpdateFlag(TRUE);
					}
					if ((i = MMI_LinkKey_SearchLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) != MMI_EOF)
					{
						MMI_LinkKey_ClearLinkHistory(i);
						MMI_SECTOR_SetUpdateFlag(TRUE);
					}
				}
				break;

			case AIROHA_L2CAP_CMD_PHONE_LINKKEY:
				DEFAULT_LINEIN_SOUND_LEVEL = dataPtr->para.phoneLinkKey.lineInSoundLevel;
				DEFAULT_LINEIN_SMALL_STEP_LEVEL = dataPtr->para.phoneLinkKey.lineInSmallStep;
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0],(U8 XDATA_PTR)&dataPtr->para.phoneLinkKey.linkKeyDataBody, sizeof(LinkHistoryType) * dataPtr->para.phoneLinkKey.storedLinkKeyCnt);
				MMI_SECTOR_SetUpdateFlag(TRUE);
				LightDbgPrint("== Get Phone Linkkey:%x,%x",(U8)gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr[0], (U8)gMMI_driver_variation_nvram.link_history_info[0].key_status);
				break;

			case AIROHA_L2CAP_CMD_PHONE_STATUS:
				LightDbgPrint("== Get Phone status:%x,%d,%d",(U8)dataPtr->para.phoneStatus.status,(U8)dataPtr->para.phoneStatus.scoSoundLevel, (U8)dataPtr->para.phoneStatus.scoSmallStep);
				gMMI_Share_K_ctl.isPhoneConnected = dataPtr->para.phoneStatus.status ? TRUE:FALSE;
				if(gMMI_Share_K_ctl.isPhoneConnected)
				{
					MMI_DRV_SetSCOVolValue(linkIndex, dataPtr->para.phoneStatus.scoSoundLevel, dataPtr->para.phoneStatus.scoSmallStep);
				}
				break;

			case AIROHA_L2CAP_CMD_ROLE_CHANGE:
				if(dataPtr->para.roleChange.isPhoneConnected)
				{
					LightDbgPrint("== set phoneInfo.relayPowerOffTimer");
					SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer,
									gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval ?
										(gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval * 4L) + ONE_SEC : MMI_AIR_POWER_OFF_SYNC_DELAY+ ONE_SEC);

					gMMI_driver_variation_nvram.link_history_info[0].a2dpSoundLevel = dataPtr->para.roleChange.a2dpSoundLevel;
					gMMI_driver_variation_nvram.link_history_info[0].a2dpSmallSteps = dataPtr->para.roleChange.a2dpSmallStep;
					gMMI_driver_variation_nvram.link_history_info[0].sound_level = dataPtr->para.roleChange.scoSoundLevel;
					gMMI_driver_variation_nvram.link_history_info[0].scoSmallStep = dataPtr->para.roleChange.scoSmallStep;
					if(dataPtr->para.roleChange.isPhoneIncoming)
					{
						LightDbgPrint("==resumeTimerI:");
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer, 20*ONE_SEC);
						gMMI_Share_K_ctl.phoneInfo.resumeState = PHONE_STATE_INCOMING;
						OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.incomingCall, &dataPtr->para.roleChange.phoneNumber, sizeof(PhoneNumber));
						OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr[0], 6);
					}
					else if(dataPtr->para.roleChange.isPhoneMusic)
					{
						LightDbgPrint("==resumeTimer[M]:bd:%x",(U8)(U8 XDATA_PTR)gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr[0]);
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer, 20*ONE_SEC);
						gMMI_Share_K_ctl.phoneInfo.resumeState = PHONE_STATE_MUSIC;
						OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr[0], 6);
					}
				}
				else
				{
					gMMI_Share_K_ctl.phoneInfo.resumeState = PHONE_STATE_NONE;
				}
				LightDbgPrint("== set isPowerOff:%d",(U8)dataPtr->para.roleChange.isPowerOff);
				if(!dataPtr->para.roleChange.isPowerOff)
				{
					if(!gMMI_Share_K_ctl.phoneInfo.resumeStateTimer)
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer, 20*ONE_SEC);

					gMMI_Share_K_ctl.phoneInfo.resumeState |= STATE_CHANGE_TO_RELAYER;
				}
				LightDbgPrint("== resumeState:%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
				MMI_Share_SendRoleChangeResp(linkIndex, dataPtr->para.roleChange.isPowerOff);
				break;

			case AIROHA_L2CAP_RESP_ROLE_CHANGE:
				LightDbgPrint("== Get PowerOffResp:%d",(U8)dataPtr->para.respRoleChange.isPowerOff);
				if(dataPtr->para.respRoleChange.isPowerOff)
					MMI_DRV_KeyEventHandler(linkIndex, KEY_POWER_OFF);
				else
				{
					MMI_ReleaseMultiLink();
					gMMI_Share_K_ctl.phoneInfo.resumeState = PHONE_STATE_CHANGE_FOLLOWER;
				}
				break;

			case AIROHA_L2CAP_CMD_TWS_LINKKEY:
				{
					U8 CODE_PTR bdAddrPtr = (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr;
					LightDbgPrint("==Get TWS:%x,%x",(U8)dataPtr->para.twsLinkKey.linkKey.linkData.bdAddr[0],(U8)bdAddrPtr[0]);
					LightDbgPrint("==Get TWS1:%x,%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0], (U8)gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr[0]);
					if(!MMI_Share_CompareXdataCode(&dataPtr->para.twsLinkKey.linkKey.linkData.bdAddr[0], bdAddrPtr, 6))
					{
						OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1],(U8 XDATA_PTR)&dataPtr->para.twsLinkKey, sizeof(LinkHistoryType));
						MMI_SECTOR_SetUpdateFlag(TRUE);
					}
					LightDbgPrint("==Get TWS2:%x,%x",(U8)gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr[0], (U8)gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr[0]);
				}
				break;

			case AIROHA_L2CAP_CMD_SCO_RELAY:
				MMI_Share_EnableSCORelay(linkIndex, (U8 XDATA_PTR)dataPtr);
				break;
			case AIROHA_L2CAP_CMD_SCO_VOL:
				MMI_Share_SetSCOVol(linkIndex, (U8 XDATA_PTR)dataPtr);
				break;
			case AIROHA_L2CAP_CMD_STATE_SYNC:
				gMMI_Share_K_ctl.relayerState = dataPtr->para.stateSync.state;
				LightDbgPrint("==GetStateSync:%x,%x",(U8)linkIndex, (U8)dataPtr->para.stateSync.state);
				break;
			case AIROHA_L2CAP_CMD_KEY_ACTION:
				LightDbgPrint("==GetKeyAction:%x,%x",(U8)dataPtr->para.keyAction.keyIndex, (U8)dataPtr->para.keyAction.keyAction);
				MMI_DRV_KeyHandler(MMI_DRV_KeyParser(dataPtr->para.keyAction.keyIndex, dataPtr->para.keyAction.keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), TRUE));
				break;
			case AIROHA_L2CAP_CMD_BG_LED:
				LightDbgPrint("==GetBGLed:%x",(U8)dataPtr->para.bgLed.ledIndex);
				MMI_LED_SendBGLedCmd(dataPtr->para.bgLed.ledIndex);
				break;
			case AIROHA_L2CAP_CMD_REPORT_BAT:
				LightDbgPrint("==GetBatInPercent:%x",(U8)dataPtr->para.batReport.batInPercent);
				if(MMI_AllLinksInCallRelatedState() || MMI_DRV_IsActiveAudioDevInMusic())
					break;

				if((i = MMI_BAT_GetLevelInPercent()) < gMMI_nvram.roleChangeBatteryThreshold)
				{
					if((i + gMMI_nvram.roleChangeBatteryDiff) < dataPtr->para.batReport.batInPercent)
					{
						MMI_Share_SendRoleChange(linkIndex, FALSE);
					}
				}
				break;
			case AIROHA_L2CAP_CMD_SET_PEQ_PARA:
				LightDbgPrint("==GetPEQPara:%d,%d,%d",(U8)dataPtr->para.peqPara.peqMode, (U8)dataPtr->para.peqPara.peqModeIndex, (U8)dataPtr->para.peqPara.sectorMode);
				MMI_DRV_SetFollowerPEQParameter(dataPtr->para.peqPara.peqMode, dataPtr->para.peqPara.peqModeIndex, dataPtr->para.peqPara.sectorMode);
				MMI_DRV_FollowerPEQModeReload();
				break;
			case AIROHA_L2CAP_CMD_PEQ_KEY_CHANGE:
				LightDbgPrint("==GetKeyChangePEQ:%x",(U8)linkIndex);
				MMI_DRV_PEQModeChange();
				break;
			case AIROHA_L2CAP_CMD_REALTIME_PEQ:
				LightDbgPrint("==GetRealtimePEQ:%x,%x",(U8)linkIndex, (U8)dataPtr->para.realtimePEQPara.enable_mode);
				MMI_Share_RealTimePEQParaHandler(dataPtr);
				break;
		}
	}
	else if(dataPtr->pktType == AIROHA_L2CAP_PKT_TYPE)
	{
		if(MMI_AIR_EscoPacketHandler(ptr))
			return;
		}
	OSMEM_Put(ptr);
}

#ifdef DEMOSOUND
PRIVATE void MMI_Share_SendDemoSoundAbsVolToFollower(void)
{
	U8 destinationLink;
	if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_A2DP_SetVolByLevel(destinationLink, gMMI_A2DP_ctl.demoSoundCtl.soundLevel, gMMI_A2DP_ctl.demoSoundCtl.smallStep);
		MMI_AVRCP_CmdGenerator(destinationLink,AVRCP_SET_ABSOLUTE_VOLUME,MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#endif
	}
}
#endif

PRIVATE void MMI_Share_SendA2DPAbsVolToFollower(U8 sourceLink)
{
	#ifdef A2DP_Profile
	U8 destinationLink;
	if(gMMI_ctl.sdapCtl[sourceLink].remoteApplicationID == PRODUCT_AIR_NONE)
	{
		if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
		{
			MMI_A2DP_SetVolByLevel(destinationLink, CURRENT_A2DP_SOUND_LEVEL(sourceLink), CURRENT_A2DP_SMALL_STEP(sourceLink));
			#ifdef AVRCP_Profile
			MMI_AVRCP_CmdGenerator(destinationLink, AVRCP_SET_ABSOLUTE_VOLUME, MMI_AVRCP_GetAbsVolByMusic(destinationLink));

			//#ifdef DBG_UART_ENABLE_MMI
			LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
			//#endif
			#endif
		}
	}
	#else
	UNUSED(sourceLink);
	#endif
}

PRIVATE void MMI_Share_SendLineInAbsVolToFollower(void)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile) && defined LINEIN_ENABLE
	U8 targetLink;
	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_AVRCP_AbsVolToMusicSoundLevel(targetLink, MMI_LineIn_GetAbsVolByLineIn());
		MMI_AVRCP_CmdGenerator(targetLink,AVRCP_SET_ABSOLUTE_VOLUME, MMI_LineIn_GetAbsVolByLineIn());
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol_L[%d]:%d", (U8)targetLink, (U8)MMI_LineIn_GetAbsVolByLineIn());
		//#endif
	}
	#endif
}

PRIVATE void MMI_Share_SendSyncCmdToRelayer(U8 SyncCmd,U8 CmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;

	if((targetLink = A2DP_SearchFollowerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(SyncCmd)
		{
			case AIR_APP_SYNC_SHARE_CMD:
				switch(CmdPara)
				{
					case AIR_APP_CMD_PARA_VP_ON:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON);
						break;
					case AIR_APP_CMD_PARA_VP_OFF:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF);
						break;
					case AIR_APP_CMD_PARA_VP_LANG_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE);
						break;
					case AIR_APP_CMD_PARA_PEQ_MODE_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE);
						break;
					default:
						break;
				}
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(SyncCmd);
	UNUSED(CmdPara);
	#endif
}

PRIVATE void MMI_Share_SendSyncCmdToFollower(U8 SyncCmd,U8 CmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;

	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(SyncCmd)
		{
			case AIR_APP_SYNC_SHARE_CMD:
				switch(CmdPara)
				{
					case AIR_APP_CMD_PARA_VP_ON:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON);
						break;
					case AIR_APP_CMD_PARA_VP_OFF:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF);
						break;
					case AIR_APP_CMD_PARA_VP_LANG_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE);
						break;
					case AIR_APP_CMD_PARA_PEQ_MODE_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE);
						break;
					default:
						break;
				}
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(SyncCmd);
	UNUSED(CmdPara);
	#endif
}

PRIVATE void MMI_Share_GetFollowerInfo(U8 cmd,U8 cmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;
	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(cmd)
		{
			case AIR_APP_GET_INFO_FROM_FOLLOWER:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_GET_INFO_FROM_FOLLOWER, cmdPara);
				break;
			case AIR_APP_GET_CHGBATSTATUS_FROM_FOLLOWER:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_GET_CHGBAT_STATUS_FROM_FOLLOWER, cmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(cmd);
	UNUSED(cmdPara);
	#endif
}

PRIVATE void MMI_Share_FollowerBehavior(U8 linkIndex)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(i != linkIndex)
		{
			MMI_ReleaseProfileLinkAndDetach(i);
		}
	}
	gMMI_ctl.currentSupportDeviceNo = 1;
	MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	MMI_SetEnterDiscoverable(FALSE);
	if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
		MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
	#ifdef LINEIN_ENABLE
	MMI_LineIn_DisableFunction();
	#endif
	MMI_CancelCreateAcl();
	MMI_Reconnect_ClearCtl();
	SYS_ReleaseTimer(&gMMI_Share_K_ctl.powerOffTimer);

	#ifdef MMI_LE_ENABLE
	MMI_LE_DisableBleLink();
	#endif

#ifdef DEMOSOUND
	MMI_A2DP_KeyDemoSoundPause();
#endif
}

PRIVATE void MMI_Share_CancelFollowerBehavior(void)
{

	gMMI_ctl.currentSupportDeviceNo = MMI_MAX_PAGE_DEVICE;
	#ifdef LINEIN_ENABLE
	MMI_LineIn_EnableFunction();
	#endif
	#ifdef MMI_LE_ENABLE
	if(!LIC_LE_GetNumOfActiveLink())
	{
		MMI_LE_EnterAdvertising();
	}
	#endif
}
/*
PRIVATE void MMI_Share_ResumeA2DP(U8 linkIndex)
{
	#ifdef A2DP_Profile
	MMIMsgType XDATA_PTR msgPtr;

	if((MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_MUSIC) || MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY) || MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_AVRCP_PLAY)) && !gMMI_ctl.audioDevCtl.resumeTimer)
	{
		MMI_A2DP_SendDSPEnableCmd(linkIndex);

		if((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_STOP_DSP_CMD) ) != (MMIMsgType XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd = linkIndex;
			OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}
	#else
	UNUSED(linkIndex);
	#endif
}
*/

PRIVATE void MMI_Share_SetAudioChannel(U8 linkIndex, U8 channel)
{
	#ifdef A2DP_Profile
	MMI_A2DP_SendCmd(linkIndex, channel);
	if(!gMMI_Share_K_ctl.isChannelMediaPushed)
	{
		MMI_PushMediaEvent((channel == A2DP_AIR_AUDIO_CHANNEL_MONO_R) ? MEDIA_EVT_TWS_RIGHT_CHANNEL : MEDIA_EVT_TWS_LEFT_CHANNEL);
		gMMI_Share_K_ctl.isChannelMediaPushed = TRUE;

		if (channel != A2DP_AIR_AUDIO_CHANNEL_MONO_R )
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_SET_CHANNEL_SEL_CMD);

		#ifdef A2DP_Profile
		if(gMMI_ctl.audioableA2DPLink == linkIndex)
		{
			MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
		}
		#endif
	}
	#else
	UNUSED(linkIndex);
	UNUSED(channel);
	#endif
}

#ifdef LINEIN_ENABLE
extern MmiLineInInfoCtrl XDATA gMMI_LineIn_ctl;
#endif

PRIVATE void MMI_Share_DecideRole(U8 linkIndex)
{
	U8 sourceLink;
	BOOL isSentPhoneData;

	if(!MMI_SDAP_IsQueryComplete(linkIndex))
		return;

	switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
		case PRODUCT_AIR_NONE:
			LightDbgPrint("Decide Role: None");
			isSentPhoneData = FALSE;
			#ifdef A2DP_Profile
			sourceLink = MMI_GetNextActiveLink(linkIndex);
			if(sourceLink != MMI_EOF)
			{
				if(A2DP_IsLinkFollower(sourceLink))
				{
					MMI_ReleaseProfileLinkAndDetach(linkIndex);
					break;
				}
			}
			#endif
			SYS_ReleaseTimer(&gMMI_Share_K_ctl.powerOffTimer);
			SYS_ReleaseTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer);
			if(gMMI_ctl.mmiInfo[linkIndex].linkPara.roleSwitchCnt < 2 && MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT)
			{
				MMI_AIR_SetLinkRoleMaster(linkIndex);
			}
			#ifdef A2DP_Profile
			if(gMMI_ctl.audioableA2DPLink == linkIndex)
				MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
			#endif

            #ifdef BLUETOOTH_SPEAKER
            MMI_LineIn_EnablePayloadEncoder();
            #else
            MMI_LineIn_DisablePayloadEncoder();
            #endif

			if(MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP) && MMI_IsProfileConnected(linkIndex, PROFILE_A2DP))
			{
				if(gMMI_Share_K_ctl.phoneInfo.resumeStateTimer && (gMMI_Share_K_ctl.phoneInfo.resumeState & 0x0F) == PHONE_STATE_MUSIC)
				{
					LightDbgPrint("==resumeStateTimer:Music");
					if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], (U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr))
					{
						LightDbgPrint("==resumeStateTimer:HALF_SEC");
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer, 3*320L);
					}
					else
					{
						LightDbgPrint("==resumeStateTimer:Release");
						SYS_ReleaseTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer);
					}
				}
				if(sourceLink != MMI_EOF && !isSentPhoneData && A2DP_IsLinkRelayer(sourceLink))
				{
					MMI_Share_SendPhoneData(sourceLink);
					MMI_Share_SendPhoneStatus(sourceLink, TRUE, linkIndex);
					isSentPhoneData = TRUE;
				}
			}
			if(MMI_IsProfileConnected(linkIndex, PROFILE_HANDSFREE))
			{
				if(gMMI_Share_K_ctl.phoneInfo.resumeStateTimer && (gMMI_Share_K_ctl.phoneInfo.resumeState & 0x0F) == PHONE_STATE_INCOMING)
				{
					LightDbgPrint("==resumeStateTimer:Incoming");
					if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], (U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr))
					{
						LightDbgPrint("==resumeStateTimer:ONE_SEC");
						SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer, ONE_SEC);
					}
					else
					{
						LightDbgPrint("==resumeStateTimer:Release");
						SYS_ReleaseTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer);
					}
				}
				if(sourceLink != MMI_EOF && !isSentPhoneData && A2DP_IsLinkRelayer(sourceLink))
				{
					MMI_Share_SendPhoneData(sourceLink);
					MMI_Share_SendPhoneStatus(sourceLink, TRUE, linkIndex);
					//isSentPhoneData = TRUE;
				}
			}
			break;

		case PRODUCT_AIR_TWS:
			{
				U8 key_index;

				if(!MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP) || !MMI_IsProfileConnected(linkIndex, PROFILE_A2DP) || !MMI_CheckNestState(linkIndex,MMI_CONNECTED))
					return;

				if ((key_index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
					return;

				if(!gMMI_driver_variation_nvram.air_link_history_info[key_index].profile)
				{
					gMMI_driver_variation_nvram.air_link_history_info[key_index].profile = (MMI_Share_IsDeviceToPhone()) ? NVRAM_PROFILE_RELAY : NVRAM_PROFILE_FOLLOW;
				}

				if(MMI_Share_IsDeviceToPhone())
				{
					LightDbgPrint("Decide Role:Relay");

					MMI_Share_CancelFollowerBehavior();

					sourceLink = MMI_GetNextNormalLink(linkIndex);

					if(!MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP) && (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT))
					{
						LightDbgPrint("App Profile Connection1");
						MMI_ConnectProfile(linkIndex, PROFILE_AIR_APP);
					}
					#ifdef A2DP_Profile
					if(MMI_AIR_SetLinkRoleMaster(linkIndex) && !A2DP_IsLinkRelayer(linkIndex))
					{
						MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_RELAYER);
						#ifdef AVRCP_Profile
						MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SET_NOTIFY_MASK, MMI_AVRCP_VOLUME_CHANGED);
						MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER);
						MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_SYNC_AND_SWITCH_AUDIO_TRANSPARENCY, DRIVER_CheckAudioComponetEnable(AUDIO_TRANSPARENCY) ? TRUE : FALSE);
						#endif

                        if(gMMI_ctl.audioableA2DPLink == linkIndex)
                        {
                            MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
                        }
                        #ifdef BLUETOOTH_SPEAKER
                        MMI_LineIn_EnablePayloadEncoder();
                        #else
                        MMI_LineIn_DisablePayloadEncoder();
                        #endif

						if(sourceLink != MMI_EOF)
						{
							MMI_Share_SendStateSync(linkIndex, MMI_GetTopStateByLink(sourceLink));
							MMI_Share_SendPhoneData(linkIndex);
							MMI_Share_SendPhoneStatus(linkIndex, TRUE, sourceLink);
							MMI_Share_SendA2DPAbsVolToFollower(sourceLink);
						}
						#ifdef DEMOSOUND
						else if(gMMI_ctl.audioableA2DPLink == DEMOSOUND_LINKINDEX)
							MMI_Share_SendDemoSoundAbsVolToFollower();
						#endif

						if(gMMI_ctl.audioableSCOLink != MMI_EOF && !gMMI_Share_K_ctl.isLightMonoMode)
						{
							MMI_Share_SendSCORelay(linkIndex, TRUE, gMMI_ctl.audioableSCOLink);
							//MMI_Share_SendSCOVol(linkIndex, CURRENT_SCO_SOUND_LEVEL(gMMI_ctl.audioableSCOLink), CURRENT_SCO_SMALL_STEP(gMMI_ctl.audioableSCOLink));
						}
					}
					#endif
					if(sourceLink == MMI_EOF)
					{
						MMI_Reconnect(RECONNECT_USER_INIT);
						#ifdef LINEIN_ENABLE
						if (!MMI_CheckNestState(MMI_EOF, MMI_LINE_IN))
						#endif
						{
							SYS_SetTimer(&gMMI_Share_K_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
						}
					}
					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
					SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_REQUEST);

					MMI_AIR_AiroStereo_Sync(AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT,gMMI_driver_variation_nvram.misc_para.init.vpLangIndex);
					MMI_Share_SendBGLedCmd(gMMI_Share_K_ctl.bgLEDIndex);
				}
				else
				{
					LightDbgPrint("Decide Role:Follower");
					MMI_Share_FollowerBehavior(linkIndex);
					#ifdef A2DP_Profile
					MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_FOLLOWER);
					#endif
					#ifdef AVRCP_Profile
					MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER);
					MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER, MMI_CHGBAT_GetBatStatus());
					#endif

					if(!MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP) && (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT))
					{
						MMI_ConnectProfile(linkIndex, PROFILE_AIR_APP);
					}

					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
					SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_REQUEST);
					if(gMMI_nvram.roleChangeBatMonitorTime)
						SYS_SetTimer(&gMMI_Share_K_ctl.batMonitorTimer, (U32)gMMI_nvram.roleChangeBatMonitorTime * HALF_MIN);
				}

				if(!gMMI_Share_K_ctl.sentInfo)
				//if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT || (gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT && gMMI_Share_K_ctl.getRelayVPOverNotify))
				{
					gMMI_Share_K_ctl.sentInfo = TRUE;
					MMI_Share_SendTwsLinkKey(linkIndex);

					if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
					{
						LightDbgPrint("Decide Role:Right");
						MMI_Share_SetAudioChannel(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_R);
					}

					if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
					{
						LightDbgPrint("Decide Role:Left");
						MMI_Share_SetAudioChannel(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_L);
						#ifdef TWS_SETTINGS
						MMI_DRV_SyncPEQRelayerToFollower(PEQ_A2DP, PEQ_GetModeIndex(PEQ_A2DP), PEQ_GetSectorMode(PEQ_A2DP));
						#endif
					}
				}
			}
			break;
	}
}

#ifdef LIGHTING_ENABLE
PRIVATE U32 MMI_Share_GetCurrentClk(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.role == ROLE_MASTER)
	{
		return HAL_ReadMasterClock();
	}
	else
	{
		return HAL_ReadSlave0Clock();
	}
}
PRIVATE void MMI_Share_UpdateLedSyncClk(U8 linkIndex, U32 currentClk)
{
	//Check sniff mode or not
	if (gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFF || gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFFING)
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk + 2000; //500ms
	}
	else
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk + 192; //60ms
	}
}
PUBLIC void MMI_Share_UpdateLedMode(void)
{
	U8 targetLink;
	U32 currentClk;

	if((gMMI_Air_ctl.ledSyncClkVal != 0) &&
		((targetLink= MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF))
	{
		currentClk = MMI_Share_GetCurrentClk(targetLink);
		currentClk = currentClk - gMMI_Air_ctl.ledSyncClkVal;
		if(currentClk<50)
		{
			gMMI_Air_ctl.ledSyncClkVal = 0;
			switch(gMMI_Air_ctl.ledMode)
			{
				case AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE:
					MusicYouCanSeeEnable = gMMI_Air_ctl.ledPara[0];
					break;
				case AIRAPP_CMD_GET_COLOR_PICKER_RGB:
					MMI_LED_Set_RGB(gMMI_Air_ctl.ledPara[0], gMMI_Air_ctl.ledPara[1], gMMI_Air_ctl.ledPara[2]);
					break;
				case AIRAPP_CMD_LIGHTING_MODE_SELECT:
					MMI_LED_LightingMode(gMMI_Air_ctl.ledPara[0]);
					break;
				case AIRAPP_CMD_ALARM_CLOCK:
					MMI_LED_AlarmClock(gMMI_Air_ctl.ledPara[0]);
					break;
			}
		}
		else if(currentClk>1000 && currentClk<6000)
		{
			//Sync fail
			gMMI_Air_ctl.ledSyncClkVal =0;
		}
	}
}
PUBLIC BOOL MMI_Share_ChangeLightModeNotify(U8 ledMode, U8 ledPara1, U8 ledPara2, U8 ledPara3)
{
	U8 targetLink;

	if((targetLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		gMMI_Air_ctl.ledMode = ledMode;
		gMMI_Air_ctl.ledPara[0] = ledPara1;
		gMMI_Air_ctl.ledPara[1] = ledPara2;
		gMMI_Air_ctl.ledPara[2] = ledPara3;
		MMI_Share_UpdateLedSyncClk(targetLink, MMI_Share_GetCurrentClk(targetLink));
		#ifdef AVRCP_Profile
		MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL, ledMode);
		#endif
		return TRUE;
	}
	return FALSE;
}
#endif

PUBLIC void MMI_Share_AclDiscHandle(U8 linkIndex)
{
	U8 twsLinkIndex;
	if(!MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
		return;

	if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
	{
		MMI_Share_CancelFollowerBehavior();

		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO);

		gMMI_Share_K_ctl.isChannelMediaPushed = FALSE;
        MMI_DRV_SetOTAVoicepromptsExist(TRUE);
		MMI_PushMediaEvent(MEDIA_EVT_TWS_DISCONNECTED);
		SYS_ReleaseTimer(&gMMI_Share_K_ctl.resumeLEDTimer);
		MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_AUDIO_CHANNEL_STEREO);
		MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_SET_CHANNEL_SEL_CMD);
		#ifdef LIGHTING_ENABLE
		gMMI_Air_ctl.ledSyncClkVal = 0;
		#endif
		SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_LEAVE);

		if(gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer)
		{
			LightDbgPrint("==relayPowerOffTimer = 100ms");
			SYS_SetTimer(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer,320L);
		}

		gMMI_Share_K_ctl.sentInfo = FALSE;
		gMMI_Share_K_ctl.relayerState = 0;

		if(A2DP_IsLinkFollower(linkIndex))
		{
			gMMI_ctl.audioableSCOLink = MMI_EOF;
		}
	}
	else
	{
		twsLinkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
		if(MMI_GetNextNormalLink(linkIndex) == MMI_EOF && !MMI_CheckNestState(MMI_EOF, MMI_LINE_IN) && MMI_EOF != twsLinkIndex)
		{
			SYS_SetTimer(&gMMI_Share_K_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
		}
	}

	LightDbgPrint("==ACL disc resumeState:%x",(U8)gMMI_Share_K_ctl.phoneInfo.resumeState);
	if(gMMI_Share_K_ctl.phoneInfo.resumeState == PHONE_STATE_CHANGE_FOLLOWER)
	{
		if(MMI_EOF == MMI_GetNextActiveLink(linkIndex))
		{
			MMI_Share_LocalBdAddrHandler(ACTION_CHANGE_TO_FOLLOW);
			gMMI_Share_K_ctl.phoneInfo.resumeState = PHONE_STATE_NONE;
		}
	}
	else if(gMMI_Share_K_ctl.phoneInfo.resumeState == STATE_CHANGE_TO_RELAYER)
	{
		if(MMI_EOF == MMI_GetNextActiveLink(linkIndex))
		{
			MMI_Share_LocalBdAddrHandler(ACTION_CHANGE_BD_ADDR_PHONE);
		}
	}
}

PUBLIC void MMI_Share_HandleKeyMode(U8 linkIndex, U8 operationParam)
{
	if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER)
	{
		MMI_Share_FollowerBehavior(linkIndex);
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER)
	{
		MMI_Share_CancelFollowerBehavior();
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_VP_OVER)
	{
		gMMI_Share_K_ctl.getRelayVPOverNotify = TRUE;
		MMI_Share_DecideRole(linkIndex);
	}
}

PUBLIC void MMI_Share_HandleAirappcmd(U8 linkIndex, U8 operationParam)
{
	switch(operationParam)
	{
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_ENABLE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_ENABLE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_DISABLE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_DISABLE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_LANG_CHANGE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_LANG_CHANGE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE:
			if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
			{
				LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
			}
			else{
				MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
				MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
			}
			break;
		default:
			break;
	}
}

PUBLIC void MMI_Share_AirappSyncVPlangIndex(U8 operationParam)
{
	gMMI_driver_variation_nvram.misc_para.init.vpLangIndex = operationParam;
}

PUBLIC void MMI_Share_AirappSyncVPlangIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	if(gMMI_driver_variation_nvram.misc_para.init.vpLangIndex == operationParam)
	{
		return;
	}
	else
	{
		MMI_PushMediaEvent(KEY_VOICE_PROMPT_LANG_CHANGE);
		MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_LANG_CHANGE);
	}
}

#ifdef PEQ_ENABLE
PUBLIC void MMI_Share_AirappSyncA2DPPEQIndex(U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_A2DP, operationParam);
}

PUBLIC void MMI_Share_AirappSyncAUXPEQIndex(U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_LINE_IN, operationParam);
}

PUBLIC void MMI_Share_AirappSyncA2DPPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_A2DP, operationParam);
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
	{
		LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
	}
	else
	{
			MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
	}
}

PUBLIC void MMI_Share_AirappSyncAUXPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_LINE_IN, operationParam);
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
	{
		LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
	}
	else
	{
		MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
		MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
	}
}
#endif

PUBLIC void MMI_Share_AirappGetBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
	MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPORT_INFO_TO_RELAYER, MMI_BAT_GetLevelInPercent());
	UNUSED(operationParam);
}

PUBLIC void MMI_Share_AirappGetChgBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
	MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER, MMI_CHGBAT_GetBatStatus());
	UNUSED(operationParam);
}

PUBLIC void MMI_Share_AirappReportChgBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_EOF != MMI_GetNextNormalLink(linkIndex))
		Airapp_ReportChgBatStatusFollower(MMI_GetNextNormalLink(linkIndex), operationParam);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncAndSwitchAudioTransparency(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_DRV_Switch_AudioTransparency(operationParam);
	UNUSED(linkIndex);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
#endif
}
PUBLIC void MMI_Share_SyncMICVolAndUp(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_DRV_IsActiveAudioDevInMusic())
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.micSoundLevelA2DP = operationParam;
	}
	else
	{
		gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	}
	MMI_DRV_MicVolHandler(linkIndex, KEY_MICUP);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncMICVolAndDn(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_DRV_IsActiveAudioDevInMusic())
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.micSoundLevelA2DP = operationParam;
	}
	else
	{
		gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	}
	MMI_DRV_MicVolHandler(linkIndex, KEY_MICDN);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncMICVol(U8 linkIndex, U8 operationParam)
{
	UNUSED(linkIndex);
	#ifdef TWS_SETTINGS
	LightDbgPrint("sync AT vol form left[%d]",operationParam);
	gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	MMI_LineIn_SendDriverVolCmd();
	#else
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncSCOStatusForAT(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	U8 airlink;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		if(operationParam)
		{
			gMMI_ctl.audioableSCOLink = linkIndex;
		}
		else
		{
			gMMI_ctl.audioableSCOLink = MMI_EOF;
		}
		if((A2DP_IsLinkFollower(airlink) && gMMI_ctl.isAudioTransparencyEnable))
		{
			MMI_DRV_Switch_AudioTransparency(operationParam ? FALSE : TRUE);
		}
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_AirAppSetATVolRsp(U8 linkIndex)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_SET_SLAVE_AT_GAIN_CHANGE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
	#else
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_Share_AirAppReportATGainLv(U8 linkIndex, U8 operationParam)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_GET_SLAVE_AT_GAIN, operationParam);
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_AirAppReportATStatus(U8 linkIndex, U8 operationParam)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_GET_SLAVE_AT_STATUS, operationParam);
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC U8 MMI_Share_ChgLinkIndexNumber(U8 linkIndex)
{
	if (linkIndex == 1)
		linkIndex=0;
	else if (linkIndex ==0)
		linkIndex=1;

	return linkIndex;
}

PUBLIC void MMI_Share_AirappReportBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	linkIndex=MMI_Share_ChgLinkIndexNumber(linkIndex);
	Airapp_ReportBatValueRightChannel(linkIndex, operationParam);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PRIVATE void MMI_Share_SaveVolToPhoneHistory(U8 linkIndex)
{
	if(A2DP_IsLinkFollower(linkIndex))
	{
		LinkHistoryType XDATA_PTR linkListPtr;
		linkListPtr = &gMMI_driver_variation_nvram.link_history_info[0];

		if(linkListPtr->key_status == NVRAM_KEY_OK)
		{
			linkListPtr->a2dpSoundLevel = CURRENT_A2DP_SOUND_LEVEL(linkIndex);
			linkListPtr->a2dpSmallSteps = CURRENT_A2DP_SMALL_STEP(linkIndex);
			MMI_SECTOR_SetUpdateFlag(TRUE);
		}
	}
}

PUBLIC void MMI_Share_TG_GetVolChanged(U8 linkIndex, U8 absVol)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		U8 i;

		#ifdef AVRCP_Profile
		MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, absVol);
		#else
		UNUSED(absVol);
		#endif

		i = MMI_DRV_GetAudioInMusicLink();
		if(MMI_EOF != i)
		{
			MMI_AVRCP_AbsVolToMusicSoundLevel(i, absVol);
			#ifdef AVRCP_Profile
			MMI_AVRCP_CmdGenerator(i, AVRCP_VOLUME_CHANGED, MMI_AVRCP_GetAbsVolByMusic(i));
			#endif
			#ifdef A2DP_Profile
			MMI_A2DP_SendDriverVolCmd(CURRENT_A2DP_SOUND_LEVEL(i), CURRENT_A2DP_SMALL_STEP(i));
			#endif
		}
		#ifdef LINEIN_ENABLE
		else if(MMI_LineIn_IsDSPOpen())
		{
			MMI_LineIn_AbsVolToSoundLevel(absVol);
			MMI_LineIn_SendDriverVolCmd();
		}
		#endif
		MMI_Share_SaveVolToPhoneHistory(linkIndex);
	}
}

PUBLIC BOOL MMI_Share_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
	if(A2DP_IsLinkFollower(linkIndex))
	{
		MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, absVol);
		MMI_Share_SaveVolToPhoneHistory(linkIndex);
	}
	return FALSE;
}

PUBLIC void MMI_Share_AVRCPSetDriverVol(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);

	if(A2DP_IsLinkFollower(linkIndex))
	{
		if(gMMI_A2DP_ctl.auxGainReplaced)
		{
			LightDbgPrint("LineIn SoundLevel:%d,%d", (U8)CURRENT_A2DP_SOUND_LEVEL(linkIndex), (U8)CURRENT_A2DP_SMALL_STEP(linkIndex));
		}
	}
}

PUBLIC void MMI_AIR_CreateConnectionToShare(U8 linkIndex)
{
	#ifdef A2DP_Profile
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
	{
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			MMI_Share_SendAudioChannel(linkIndex, gMMI_nvram.twsKChannel);
		}

		/*
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
		}*/
	}
	else
	#else
	UNUSED(linkIndex);
	#endif
	{
		MMI_AIR_InvalidLinkHandle(linkIndex);
	}
}

PUBLIC void MMI_Share_ExitLineInState(void)
{
	U8 i = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

	if(MMI_EOF != i)
	{
		SYS_SetTimer(&gMMI_Share_K_ctl.resumeLEDTimer, HALF_SEC);
		if(MMI_EOF == MMI_GetNextNormalLink(i))
		{
			SYS_SetTimer(&gMMI_Share_K_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
		}
	}
}

PUBLIC void MMI_Share_EnterLineInState(void)
{
	SYS_ReleaseTimer(&gMMI_Share_K_ctl.powerOffTimer);
	//MMI_PushMediaEvent(MEDIA_EVT_TWS_DISCONNECTED);
	SYS_ReleaseTimer(&gMMI_Share_K_ctl.resumeLEDTimer);
}

PUBLIC void MMI_Share_LineInStartRelay(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInKeyVolUpDn(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInUartSetVol(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_AiroStereo_Sync(U8 SyncCmd,U8 CmdPara)
{
	if(A2DP_SearchRelayerLink() != MAX_MULTI_POINT_NO)
	{
		LightDbgPrint("Sync cmd:Relayer to Follower");
		MMI_Share_SendSyncCmdToFollower(SyncCmd,CmdPara);
	}
	else if(A2DP_SearchFollowerLink() != MAX_MULTI_POINT_NO)
	{
		LightDbgPrint("Sync cmd:Follower to Relayer");
		MMI_Share_SendSyncCmdToRelayer(SyncCmd,CmdPara);
	}
}

PUBLIC BOOL MMI_Share_IdleKeyVolUpDnFirst(BOOL isVolumeUp)
{

    UNUSED(isVolumeUp);
	if(MMI_EOF == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
	{
		if(!CURRENT_ACTIVE_LINK_CNT)
			return TRUE;
	}
	return FALSE;
}


PUBLIC void MMI_Share_IdleKeyVolUpDnLast(BOOL isVolumeUp)
{
	U8 linkIndex;
	if((linkIndex =  MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkFollower(linkIndex))
	{
		if(!gMMI_Share_K_ctl.getVolKeyFromTWS)
			MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, (isVolumeUp) ? AVC_OPERATION_VOL_UP : AVC_OPERATION_VOL_DOWN);
	}
}


PUBLIC void MMI_Share_AVRCPConnectedNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC BOOL MMI_Share_TG_GetKeyVolUp(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		gMMI_Share_K_ctl.getVolKeyFromTWS = TRUE;
		if(A2DP_IsLinkFollower(linkIndex))
			MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEDN : KEY_VOICEUP);
		else if(A2DP_IsLinkRelayer(linkIndex))
			MMI_DRV_VoiceAdjustment(linkIndex, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEDN : KEY_VOICEUP, TRUE);
		gMMI_Share_K_ctl.getVolKeyFromTWS = FALSE;
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_Share_TG_GetKeyVolDown(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		gMMI_Share_K_ctl.getVolKeyFromTWS = TRUE;
		if(A2DP_IsLinkFollower(linkIndex))
			MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEUP : KEY_VOICEDN);
		else if(A2DP_IsLinkRelayer(linkIndex))
			MMI_DRV_VoiceAdjustment(linkIndex, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEUP : KEY_VOICEDN, TRUE);
		gMMI_Share_K_ctl.getVolKeyFromTWS = FALSE;
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_Share_A2DPSignalChannelReadyNotify(U8 linkIndex)
{
	#ifdef A2DP_Profile
	if(	gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
			MMI_A2DP_ConnectMediaChannelSource(linkIndex);
		else
			A2DP_SetChannelOpenTimer(linkIndex, (U32) 10 * ONE_SEC);
	}
	#else
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_Share_A2DPOpenNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC void MMI_Share_A2DPDSPEnabledNotify(U8 linkIndex)
{
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		#ifdef A2DP_Profile
		if(!MMI_SDAP_IsQueryComplete(linkIndex))
		{
			MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
		}
		else
		{
			if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
			{
				if(!gMMI_Share_K_ctl.isChannelMediaPushed)
					MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
			}
		}
		#endif
	}
	#ifdef DEMOSOUND
	else
	{
		MMI_Share_SendDemoSoundAbsVolToFollower();
	}
	#endif
}

PUBLIC void MMI_Share_A2DPStartRelay(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC void MMI_Share_A2DPKeyVolUpDn(U8 linkIndex, BOOL isVolumeUp)
{
	UNUSED(isVolumeUp);
	if(MAX_MULTI_POINT_NO == A2DP_SearchFollowerLink())
		MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
	else
	{
		MMI_Share_SaveVolToPhoneHistory(linkIndex);
	}
}

PUBLIC BOOL MMI_Share_SDAPDisconnectedNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
	return FALSE;
}

PUBLIC void MMI_Share_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave)
{
	UNUSED(status);
	UNUSED(roleOfSlave);
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC BOOL MMI_Share_PreProcessForWriteFlashAfterRingVP(void)
{
	#ifdef A2DP_Profile
	U8 linkIndex;

	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	}
	#endif
	return FALSE;
}

PUBLIC void MMI_Share_SetConnected(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC void MMI_Share_FakeMediaEvt(U16 id)
{
	UNUSED(id);
/*
	if(id == MEDIA_EVT_TWS_LEFT_CHANNEL || id == MEDIA_EVT_TWS_RIGHT_CHANNEL)
	{
		U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
		if(linkIndex != MMI_EOF)
		{
			#ifdef A2DP_Profile
			if(A2DP_IsLinkRelayer(linkIndex))
			{
				#ifdef AVRCP_Profile
				MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_VP_OVER);
				#endif
			}
			else
			#endif
			{
				MMI_Share_ResumeA2DP(linkIndex);
			}
		}
	}
*/
}

PUBLIC void MMI_Share_CheckKeyLineInPowerOn(U16 keyCode)
{
	if(keyCode == KEY_INVALID)
	{
		U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

		if(linkIndex == MMI_EOF || MMI_EOF == MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE))
            return;

		if(!MMI_Share_IsDeviceToPhone())
		{
			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
			{
				MMI_PushMediaEvent(MEDIA_EVT_TWS_RIGHT_CHANNEL);
			}

			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
			{
				MMI_PushMediaEvent(MEDIA_EVT_TWS_LEFT_CHANNEL);
			}
		}
	}
}

PUBLIC void MMI_Share_CheckTimer(void)
{
	U8 linkIndex;

	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.powerOffTimer))
	{
		#ifdef AVRCP_Profile
		MMI_AIR_SendKeyPowerOffToAirDevice();
		LightDbgPrint("CheckTimer:SendKeyPowerOffToAir");
		#endif
	}

	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.resumeLEDTimer))
	{
		if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState() && MMI_EOF != MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
			U8 index;

			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
			{
				for(index = 0; index< MAX_LED_FILTER_NO; index++)
				{
					if (gMMI_nvram.ledFilterMap[index].evtOpcode == MEDIA_EVT_TWS_RIGHT_CHANNEL)
					{
						MMI_LED_SendLedFilterCmd(index);
					}
				}
			}
			else if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
			{
				for(index = 0; index< MAX_LED_FILTER_NO; index++)
				{
					if (gMMI_nvram.ledFilterMap[index].evtOpcode == MEDIA_EVT_TWS_LEFT_CHANNEL)
					{
						MMI_LED_SendLedFilterCmd(index);
					}
				}
			}
		}
	}

	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.phoneInfo.relayPowerOffTimer))
	{
		LightDbgPrint("== TO relayPowerOffTimer");
		MMI_Share_LocalBdAddrHandler(ACTION_CHANGE_BD_ADDR_PHONE);
	}

	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer))
	{
		linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr);

		LightDbgPrint("==resumeStateTimer:TO:%d",(U8)linkIndex);
		if(MAX_MULTI_POINT_NO != linkIndex)
		{
			if((gMMI_Share_K_ctl.phoneInfo.resumeState & 0x0F) == PHONE_STATE_INCOMING)
			{
				if(	MMI_IsProfileConnected(linkIndex, PROFILE_HANDSFREE) &&
					MMI_HFP_INCOMMING == MMI_GetTopStateByLink(linkIndex) &&
					gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length == 0)
				{
					OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall,
					(U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.incomingCall,
					sizeof(PhoneNumber));
					gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet = TRUE;
				}
			}
			else if((gMMI_Share_K_ctl.phoneInfo.resumeState & 0x0F) == PHONE_STATE_MUSIC)
			{
				if(MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP) && MMI_IsProfileConnected(linkIndex, PROFILE_A2DP))
				{
					U8 playStatus = MMI_AVRCP_GetPlayStatus(linkIndex);
					if(!MMI_A2DP_IsInStreaming(linkIndex) ||
						playStatus == AVRCP_PLAYBACK_STATUS_NONE || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED ||
						playStatus == AVRCP_PLAYBACK_STATUS_STOPPED)
					{
						LightDbgPrint("==resumeStateTimer:Send Play");
						MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION,AVC_OPERATION_PLAY);
					}
				}
			}
		}
	}

	MMI_Share_CheckChangeBdaddrTimer();

	MMI_AIR_EscoDataRelay();

	SYS_IsTimerExpired(&gMMI_Share_K_ctl.stayFollowerTimer);

	if(SYS_IsTimerExpired(&gMMI_Share_K_ctl.batMonitorTimer))
	{
		U8 airLink;
		if((airLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkFollower(airLink))
		{
			MMI_Share_SendBat(airLink, MMI_BAT_GetLevelInPercent());
			SYS_SetTimer(&gMMI_Share_K_ctl.batMonitorTimer, (U32)gMMI_nvram.roleChangeBatMonitorTime * HALF_MIN);
		}
	}
}

PUBLIC BOOL MMI_Share_LoadState(U8 linkIndex, U8 state)
{
	U8 airLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

	if(state == MMI_GetTopStateByLink(linkIndex) && state == MMI_CONNECTED)
	{
		if(linkIndex == airLink && CURRENT_ACTIVE_LINK_CNT != gMMI_ctl.currentSupportDeviceNo)
		{
			if(CURRENT_ACTIVE_LINK_CNT == 1)
			{
				if(MMI_CheckNestState(linkIndex, MMI_CONDISCABLE))
				{
					MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONDISCABLE].entryLedDisplayIndex);
				}
				else
				{
					MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONNECTABLE].entryLedDisplayIndex);
				}
			}
			else if( gMMI_ctl.currentSupportDeviceNo==1 )
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			return TRUE;
		}
	}

	if((state == MMI_CONDISCABLE || state == MMI_CONNECTABLE))
	{
		if(MMI_EOF != airLink )
		{
			if (MMI_EOF == MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[airLink].bdAddr, TRUE))
				return FALSE;

			if((!MMI_Share_IsDeviceToPhone()) && (CURRENT_ACTIVE_LINK_CNT>1))
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONNECTED].entryLedDisplayIndex);
			}
			else
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			return TRUE;
		}
		else if(MMI_CheckProfileOnAllLinks())
		{
			MMI_LED_SendBGLedCmd(5);
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_ConnectedExit(U8 linkIndex)
{
	U8 twsLinkIndex;
	U8 keyState = MMI_GetKeyState();
	// Not detach after audio transfer
	if (MMI_KEY_STATE_POWER_OFF != keyState && MMI_KEY_STATE_POWER_OFF_SYNC != keyState)
	{
		if(MMI_EOF == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
			MMI_LineIn_DisablePayloadEncoder();
		}
	}

	twsLinkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
	if(MMI_EOF != twsLinkIndex && linkIndex != twsLinkIndex)
	{
		MMI_Share_SendPhoneStatus(twsLinkIndex, FALSE, linkIndex);
	}
}

PUBLIC U8 MMI_Share_SendKeyPowerOffToAirDevice(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_IsActiveLink(linkIndex) && MMI_AIR_IsAirLink(linkIndex))
		{
			return MMI_Share_SendRoleChange(linkIndex, TRUE);
		}
	}
	return MAX_MULTI_POINT_NO;
}

PUBLIC void MMI_Share_WriteLocalBdAddrCpl(BOOL isSuccess)
{
	MMI_Share_LocalBdAddrHandler((isSuccess)? ACTION_BD_ADDR_OK : ACTION_BD_ADDR_FAIL);
}

PUBLIC void MMI_Share_Init(void)
{
    MP_PARAMETER_STRU CODE_PTR pMpParameter = &((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter;
	OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&gMMI_Share_K_ctl.currBdAddr, &pMpParameter->MP_SysLocalDeviceInfo.BdAddr, sizeof(BD_ADDR_STRU));
	gMMI_Share_K_ctl.relayerState = 0;
	/* [Start]for QA debug print. Please do not delete these */
	if(MMI_Share_IsDeviceToPhone())
	{
		LightDbgPrint("TWSK - Role: Relayer");
	}
	else
	{
		LightDbgPrint("TWSK - Role: Follower");
	}
	/* [End]for QA debug print. Please do not delete these */
}

#define LIST_NORMAL_DATA TRUE

PUBLIC MMIReonnectDataType XDATA_PTR MMI_Share_Reconnect_GetTarget(U8 type)
{
	MMIReonnectDataType XDATA_PTR reconectTargetPtr = (MMIReonnectDataType XDATA_PTR)NULL;

	if(gMMI_nvram.reConnectInfo.afterPowerOn != RECONNECT_TWS_0011 && gMMI_nvram.reConnectInfo.afterPowerOn != RECONNECT_TWS_0101)
		return reconectTargetPtr;

	MMI_Reconnect_DeleteInvalidList();

	if(gMMI_ctl.discoverableState != (SAC_NON_DISCOVERABLE_MODE| NON_DISCOVERABLE_MODE) && MMI_LinkKey_GetAirLinkPDLNumber())
	{
		if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
		{
			MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr);
		}
		else
		{
			MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr);
		}
	}

	if( type == RECONNECT_LINK_LOSS || (type == RECONNECT_CHECK && gMMI_ctl.reconnectCtl.type == RECONNECT_LINK_LOSS))
	{
		MMIReonnectDataType XDATA_PTR targetPtr;
		MMIReonnectDataType XDATA_PTR nextPtr;
		U8 dataCnt;
		BOOL isFirstTime = FALSE;

		dataCnt = MMI_Reconnect_GetListCnt(LIST_HIGH_PRI_DATA);
		if(dataCnt)
		{
			if( gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex == 0xFF ||
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex >= dataCnt)
			{
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex = 0;
				isFirstTime = TRUE;
			}

			LightDbgPrint("==2GetListCnt:%d,%d,%d",(U8)dataCnt,(U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex,(U8)isFirstTime);


			targetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex];
			nextPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconLinkLossWatchDogData[(gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex + 1)%dataCnt];

			LightDbgPrint("==3GetListCnt:%x,%x",(U8)targetPtr->bdAddr[0],(U8)nextPtr->bdAddr[0]);
			if(gMMI_nvram.reConnectInfo.afterPowerOn == RECONNECT_TWS_0011)
			{
				U8 diff;

				if(targetPtr->retryCnt > nextPtr->retryCnt)
					diff = targetPtr->retryCnt - nextPtr->retryCnt;
				else
					diff = nextPtr->retryCnt - targetPtr->retryCnt;

				LightDbgPrint("== 2RtyCnt:%d = %d - %d",(U8)diff, (U8)nextPtr->retryCnt, (U8)targetPtr->retryCnt);

				if(!(diff % 2) && !isFirstTime)
				{
					reconectTargetPtr = nextPtr;
					gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex++;
					gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex %= dataCnt;
					LightDbgPrint("==isFirstTime:%x,%x",(U8)targetPtr->bdAddr[0],(U8)nextPtr->bdAddr[0]);
				}
				else
				{
					reconectTargetPtr = targetPtr;
				}
			}
			else
			{
				reconectTargetPtr = targetPtr; //(isFirstTime)?targetPtr : nextPtr;
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex++;
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex %= dataCnt;
			}
			LightDbgPrint("==LinkLoss Target:%x,%x",(U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex, (U8)reconectTargetPtr->bdAddr[0]);
		}
	}
	else if(type == RECONNECT_PWR_ON || (type == RECONNECT_CHECK && gMMI_ctl.reconnectCtl.type == RECONNECT_PWR_ON))
	{
		MMIReonnectDataType XDATA_PTR targetPtr;
		MMIReonnectDataType XDATA_PTR nextPtr;
		U8 dataCnt;
		BOOL isFirstTime = FALSE;

		dataCnt = MMI_Reconnect_GetListCnt(LIST_NORMAL_DATA);
		if(dataCnt)
		{
			if(gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex == 0xFF ||
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex >= dataCnt)
			{
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex = 0;
				isFirstTime = TRUE;
			}
			LightDbgPrint("== GetListCnt:%d, %d",(U8)dataCnt,(U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex);

			targetPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex];
			nextPtr = &gMMI_ctl.reconnectCtl.reconnectListPtr->reconnectData[(gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex + 1)%dataCnt];

			if(gMMI_nvram.reConnectInfo.afterPowerOn == RECONNECT_TWS_0011)
			{
				U8 diff;

				if(targetPtr->retryCnt > nextPtr->retryCnt)
					diff = targetPtr->retryCnt - nextPtr->retryCnt;
				else
					diff = nextPtr->retryCnt - targetPtr->retryCnt;

				LightDbgPrint("== RtyCnt:%d = %d - %d",(U8)diff, (U8)nextPtr->retryCnt, (U8)targetPtr->retryCnt);

				if(!(diff % 2) && !isFirstTime)
				{
					reconectTargetPtr = nextPtr;
					gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex++;
					gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex %= dataCnt;
				}
				else
				{
					reconectTargetPtr = targetPtr;
				}
			}
			else
			{
				reconectTargetPtr = targetPtr; //(isFirstTime)?targetPtr : nextPtr;
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex++;
				gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex %= dataCnt;
			}
			LightDbgPrint("==PowerOn Target:%x,%x",(U8)gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex, (U8)reconectTargetPtr->bdAddr[0]);
		}
	}
	return reconectTargetPtr;
}

PUBLIC void MMI_Share_ACLConnectedNotify(U8 linkIndex)
{
	if(	SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
		SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
	{
		MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr);
		MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr);
		gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID = PRODUCT_AIR_TWS;

		if(MMI_SUPPORT_HFP_FEAT)
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HFP;
		if(MMI_SUPPORT_HSP_FEAT)
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HSP;
		if(MMI_SUPPORT_A2DP_SINK_FEAT)
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_A2DP;
		if(MMI_SUPPORT_AVRCP_FEAT)
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AVRCP;

		gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AIRAPP;
		gMMI_ctl.sdapCtl[linkIndex].airappScn = SCN_AIR_APP;

		gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_SERVICE_QUERY_COMPLETE;

		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT || MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
		{
			gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_ACTIVE_CONNECT;
			SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delaySDPTimer);
			MMI_CreateActiveProfileConnection(linkIndex);
		}

		gMMI_Share_K_ctl.sentInfo = FALSE;
	}
}

PUBLIC void MMI_Share_AcceptConnReq(U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR rolePtr)
{
#ifdef PRODUCT_TYPE_K
	if(	SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
		SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
	{
		if(MMI_Share_IsDeviceToPhone())
			*rolePtr = HCI_CMD_ACCEPT_CONN_REQ_ROLE_IS_MASTER;
	}
#else
	UNUSED(bdAddrPtr);
	UNUSED(rolePtr);
#endif
}

PUBLIC BOOL MMI_Share_DSPSCOEnabled_Event(U8 linkIndex, BOOL isLightMono)
{
	U8 airlink;

	gMMI_Share_K_ctl.isLightMonoMode = isLightMono;

	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		LightDbgPrint("SCO ENABLE:%d",(U8)isLightMono);

		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_NONE && !isLightMono)
		{
			MMI_Share_SendSCORelay(airlink, TRUE, linkIndex);
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_DSPSCODisabled_Event(U8 linkIndex)
{
	U8 airlink;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		LightDbgPrint("SCO DISABLE");

		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_NONE)
		{
			MMI_Share_SendSCORelay(airlink, FALSE, linkIndex);
		}
	}
}

PUBLIC void MMI_Share_SendDriverVolCmd(U8 soundLevel, U8 smallStep)
{
	U8 airlink;
	if(soundLevel == VOLUME_DONT_CARE || smallStep == VOLUME_DONT_CARE)
		return;

	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		if(A2DP_IsLinkRelayer(airlink))
			MMI_Share_SendSCOVol(airlink, soundLevel, smallStep);
	}
}

PUBLIC BOOL MMI_Share_ReconnectLinkLoss(U8 XDATA_PTR bdAddrPtr)
{
	U8 i;

	if(	SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
		SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
	{
		for(i = 0 ;  i < MAX_MULTI_POINT_NO ; i++)
		{
			if(MMI_IsActiveLink(i))
			{
				break;
			}
		}
		if( i == MAX_MULTI_POINT_NO)
		{
			LightDbgPrint("===No phone !!!!!!!!!");
			goto SET_LINK_LOSS_RECONNECT_DATA;
		}
	}
	else if(SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr))
	{
		if(	MMI_Reconnect_FindTarget((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
			MMI_Reconnect_FindTarget((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
		{
			LightDbgPrint("===No TWS !!!!!!!!!");
			SET_LINK_LOSS_RECONNECT_DATA:
			MMI_Reconnect_ClearCtl();
			MMI_CancelCreateAcl();
			if(MMI_Reconnect_FindTarget((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr))
			{
				MMI_Reconnect_AddListByMode(RECONNECT_USER_INIT, RECONNECT_LAST_ONE);
				MMI_Reconnect_AddListByMode(RECONNECT_LINK_LOSS, gMMI_nvram.reConnectInfo.afterPowerOn);
			}
			else
			{
				MMI_Reconnect_AddListByMode(RECONNECT_LINK_LOSS, gMMI_nvram.reConnectInfo.afterPowerOn);
				MMI_Reconnect_AddListByMode(RECONNECT_USER_INIT, RECONNECT_LAST_ONE);
			}
			return TRUE;
		}
		else
		{
			MMI_Reconnect_AddListByMode(RECONNECT_LINK_LOSS, gMMI_nvram.reConnectInfo.afterPowerOn);
			MMI_Reconnect_AddListByMode(RECONNECT_USER_INIT, RECONNECT_LAST_ONE);
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC BOOL MMI_Share_SendCreateConnection(U8 linkIndex, U8 XDATA_PTR bdAddrPtr, U8 type, U8 XDATA_PTR codPtr)
{
	U8 CODE_PTR mpBdAddrPtr = (U8 CODE_PTR)&((SECTOR_MP_PARAMETER_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_MP_PARAMETER))->MpParameter.MP_SysLocalDeviceInfo.BdAddr;

    UNUSED(linkIndex);
	if(	SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
		SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
	{
		if(SYS_IsBDAddrIsTheSame(bdAddrPtr, &gMMI_Share_K_ctl.currBdAddr[0]))
		{
			goto SEND_WRITE_LOCAL_BDADDR;
		}
	}
	else
	{
		if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
		{
			if(MMI_Share_CompareXdataCode(gMMI_Share_K_ctl.currBdAddr, mpBdAddrPtr, 6))
			{
				goto SEND_WRITE_LOCAL_BDADDR;
			}

		}
		else
		{
			if(!MMI_LinkKey_GetAirLinkPDLNumber())
			{
				return FALSE;
			}

			if(!SYS_IsBDAddrIsTheSame(&gMMI_Share_K_ctl.currBdAddr[0], &gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr))
			{
				goto SEND_WRITE_LOCAL_BDADDR;
			}
		}
	}
	return FALSE;

	SEND_WRITE_LOCAL_BDADDR:
	if(MMI_IsActiveLink(0) || MMI_IsActiveLink(1))
	{
		MMI_PageFailHandler(MMI_EOF);
		return TRUE;
	}

	gMMI_Share_K_ctl.ccCtl.type = type;
	OSMEM_memcpy_xdata_xdata(&gMMI_Share_K_ctl.ccCtl.bdAddr[0], bdAddrPtr, 6);
	OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_Share_K_ctl.ccCtl.cod[0], codPtr, 3);
	MMI_Share_LocalBdAddrHandler(ACTION_CHANGE_BD_ADDR_CC);
	return TRUE;
}


PUBLIC void MMI_Share_PageFailHandler(U8 linkIndex)
{
	if(MMI_IsActiveLink(0) || MMI_IsActiveLink(1))
	{
		return ;
	}
	MMI_Share_LocalBdAddrHandler(ACTION_RESUME_BD_ADDR_PAGE_FAIL);

	UNUSED(linkIndex);
}

PUBLIC void MMI_Share_Reconnect_DeleteInvalidList(U8 linkIndex)
{
	U8 XDATA_PTR bdAddrPtr;

	if(MMI_IsActiveLink(linkIndex))
	{
		bdAddrPtr = &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0];
		if(	SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr) ||
			SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr))
		{
			MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr);
			MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr);
		}
		else if(SYS_IsBDAddrIsTheSame(bdAddrPtr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[0].linkData.bdAddr))
		{
			MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_Share_K_ctl.currBdAddr[0]);
		}
	}
}

PUBLIC void MMI_Share_LinkKeyNotify(U8 linkIndex)
{
	if(linkIndex != MAX_MULTI_POINT_NO)
	{
		U8 sourceLink;
		for(sourceLink = 0; sourceLink < MAX_MULTI_POINT_NO; sourceLink++)
		{
			if(MMI_IsActiveLink(sourceLink) && A2DP_IsLinkRelayer(sourceLink))
			{
				MMI_Share_SendPhoneData(sourceLink);
				break;
			}
		}
	}
}

PUBLIC void MMI_Share_SaveData(void)
{
	MMI_Share_LinkKeyNotify(0);
}


PUBLIC BOOL MMI_Share_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull)
{
    UNUSED(flowStatus);
    UNUSED(rxFull);
	if(A2DP_IsLinkRelayer(linkIndex))
	{
		if(serviceType == SERVICE_TYPE_BEST_EFFORT)
		{
			if(gMMI_ctl.audioableSCOLink != MMI_EOF)
				return TRUE;
			if(gMMI_ctl.audioableA2DPLink != MMI_EOF)
				return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex)
{
    UNUSED(dataPtr);
	switch(opCode)
	{
		case MMI_UART_IND_STATE_CHANGE:
		case MMI_UART_IND_INSERT_STATE:
		case MMI_UART_IND_REPLACE_STATE:
		case MMI_UART_IND_REMOVE_STATE:
			if( linkIndex != MMI_EOF)
			{
				U8 airlink;
				if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
				{
					if(A2DP_IsLinkRelayer(airlink) && linkIndex != airlink)
						MMI_Share_SendStateSync(airlink, MMI_GetTopStateByLink(linkIndex));
				}
			}
			break;
	}
}

PUBLIC BOOL MMI_Share_Key_Event(KeyIndex keyIndex, U8 keyAction)
{
	U8 airlink;
	U16 keyEventCode;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkFollower(airlink))
	{
		if(MMI_KEY_MAP_MAPPING_BY_KEY_DOWN_STATE_FEAT)
		{
			if(keyAction == KEY_DOWN)
			{
				MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
			}
		}

		keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, gMMI_Share_K_ctl.relayerState, FALSE);
		if(keyEventCode == KEY_VOICEUP || keyEventCode == KEY_VOICEDN)
		{
			return FALSE;
		}

		switch(gMMI_Share_K_ctl.relayerState)
		{
			case MMI_LINE_IN:
			case MMI_CONNECTED:
			case MMI_HFP_INCOMMING:
			case MMI_HFP_OUTGOING:
			case MMI_HFP_CALLACTIVE:
			case MMI_HFP_CALLACTIVE_WITHOUT_SCO:
			case MMI_HFP_CAIMG:
			case MMI_HFP_CAOGG:
			case MMI_HFP_CAMULTY:
				if(keyEventCode == KEY_INVALID)
				{
					keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), FALSE);
					switch(keyEventCode)
					{
						case KEY_VOICE_COMMAND_ENABLE:
						case KEY_VOICE_PROMPT_LANG_CHANGE:
							return TRUE;
					}
					return FALSE;
				}
				LightDbgPrint("MMI_Share_Key_Event:%x,%d, %d",(U8)keyIndex, (U8)keyAction, (U8)gMMI_Share_K_ctl.relayerState);
				switch(MMI_DRV_GetKeyEventGroup(keyEventCode))
				{
					case HID_KEY_EVENT:
					case HFP_KEY_EVENT:
					case AVRCP_KEY_EVENT:
						MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
						return TRUE;
					case SYSTEM_KEY_EVENT:
						if(keyEventCode == KEY_MIC_MUTE_TOGGLE || keyEventCode == KEY_MIC_MUTE_ON || keyEventCode == KEY_MIC_MUTE_OFF ||
						keyEventCode == KEY_ROLE_CHANGE)
						{
							MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
							return TRUE;
						}
					default:
						return FALSE;
				}
				break;
			default:
				keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), FALSE);

				LightDbgPrint("MMI_Share_Key2:%d,%d,%X",(U8)keyIndex, (U8)keyAction, (U32)keyEventCode);
				switch(keyEventCode)
				{
					case KEY_RECONNECT_USER_INIT:
					case KEY_DISCOVERABLE:
					case KEY_NONDISCOVERABLE:
						MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
						return TRUE;
				}
				break;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_WriteSecurityModeComplete(void)
{
	SYS_SetTimer(&gMMI_Share_K_ctl.changeLocalBdAddrTimer, (U32)gMMI_nvram.twsKWaitChangeBdaddrTime*320L);
}

PUBLIC void MMI_Share_WriteDiscoverabilityMode(U8 discoverabilityMode)
{
	if(discoverabilityMode != (SAC_NON_DISCOVERABLE_MODE| NON_DISCOVERABLE_MODE))
	{
		if(MMI_LinkKey_GetAirLinkPDLNumber())
		{
			if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_LEFT)
			{
				MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[1].linkData.bdAddr);
			}
			else
			{
				MMI_Reconnect_DeleteListByBdAddr((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[0].linkData.bdAddr);
			}
		}
		MMI_Share_LocalBdAddrHandler(ACTION_ENTER_DISCOVERABLE);
	}
}

PUBLIC void MMI_Share_EnterIncomingState(U8 linkIndex)
{
	if(gMMI_Share_K_ctl.phoneInfo.resumeStateTimer && (gMMI_Share_K_ctl.phoneInfo.resumeState & 0x0F) == PHONE_STATE_INCOMING)
	{
		U8 phoneLinkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.phoneBdAddr);
		if(phoneLinkIndex == linkIndex)
		{
			if(	gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length == 0)
			{
				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall,
				(U8 XDATA_PTR)&gMMI_Share_K_ctl.phoneInfo.incomingCall,
				sizeof(PhoneNumber));
				gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet = TRUE;
			}
			SYS_ReleaseTimer(&gMMI_Share_K_ctl.phoneInfo.resumeStateTimer);
		}
	}
}

PUBLIC void MMI_Share_KeyRoleChange(U8 linkIndex)
{
	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkRelayer(linkIndex))
		MMI_Share_SendRoleChange(linkIndex, FALSE);
}
