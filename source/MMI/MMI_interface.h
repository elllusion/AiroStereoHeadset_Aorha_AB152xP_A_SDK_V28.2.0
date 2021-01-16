//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_INTERFACE_H_
#define _MMI_INTERFACE_H_

#include "sector_mmi_driver_variation_nvram.h"
#include "sector_mmi_nvram_type.h"
#include "sector_mmi_nvram_keymap.h"
#include "sector_mmi_customize_v_nvram.h"


#ifdef _MMI_C_
	#define MMI_EXTERN
#else
	#define MMI_EXTERN extern
#endif


#define MMI_EOF	0xFF


#define MMI_SNIFF_HFP						0
#define MMI_SNIFF_A2DP_CONTROL				1
#define MMI_SNIFF_AVRCP						2
#define MMI_SNIFF_A2DP_MUSIC				3
#define MMI_SNIFF_ACL_READY_SLC_NOT_YET		4
#define MMI_SNIFF_SDP						5
#define MMI_SNIFF_DISC						6
#define MMI_SNIFF_SPP						7
#define MMI_SNIFF_TWS						8
#define MMI_SNIFF_ROLE_SWITCH				9
#define MMI_SNIFF_AVDTP_SIGNAL				10
#define MMI_SNIFF_SOUNDBAR_EST_SCO			11
#define MMI_FORCE_UNSNIFF					12
#define MMI_SNIFF_OPP						13
#ifdef HID_Profile
#define MMI_SNIFF_HID						14
#endif
#define MMI_SNIFF_OTA						15

enum
{
	PROFILE_NO_ERROR,
	SLC_ESTABLISH_FAIL,
	SLC_SECURITY_BLOCK,
	VGS_VGM_ERROR  = 0xFF,
};

#define DEFAULT_SCO_SOUND_LEVEL	 			(gMMI_driver_variation_nvram.misc_para.init.defaultSCOSoundLevel)
#define DEFAULT_IDLE_SMALL_STEP				(gMMI_driver_variation_nvram.misc_para.init.smallStepSoundLevelIdle)
#define DEFAULT_A2DP_SOUND_LEVEL		 	(gMMI_nvram.defaultA2DPSoundLevel)
#define DEFAULT_LINEIN_SOUND_LEVEL	 		(gMMI_driver_variation_nvram.misc_para.init.LineInHistoryVol)
#define DEFAULT_LINEIN_SMALL_STEP_LEVEL		(gMMI_driver_variation_nvram.misc_para.init.LineInSmallStepVol)
#define DEFAULT_VOICE_COMMAND_LEVEL
#define DEFAULT_MIC_SOUND_LEVEL_IN_A2DP		(gMMI_nvram.defaultMicSoundLevelInA2DP);

#define CURRENT_SCO_SOUND_LEVEL(linkInd)	(gMMI_ctl.mmiInfo[linkInd].linkPara.currentSoundLevel)
#define CURRENT_SCO_SMALL_STEP(linkInd)		(gMMI_ctl.mmiInfo[linkInd].linkPara.smallStep)

#define CURRENT_SCO_VGM(linkInd)			(gMMI_ctl.mmiInfo[linkInd].linkPara.vgm)
#define CURRENT_A2DP_SOUND_LEVEL(linkInd)	(gMMI_ctl.mmiInfo[linkInd].linkPara.soundLevelA2DP)
#define CURRENT_A2DP_SMALL_STEP(linkInd)	(gMMI_ctl.mmiInfo[linkInd].linkPara.smallStepLevelA2DP)
#define CURRENT_AVRCP_VOLUME(linkInd)		(gMMI_ctl.mmiInfo[linkInd].linkPara.avrcpVolume)

#define CURRENT_MIC_SOUND_LEVEL_IN_A2DP(linkInd)	(gMMI_ctl.mmiInfo[linkInd].linkPara.micSoundLevelA2DP)

#define MMI_MAX_SCO_SOUND_LEVEL()			(gMMI_nvram.totalSoundLevel - 1)
#define MMI_MAX_MUSIC_SOUND_LEVEL()			(gMMI_nvram.totalMusicSoundLevel - 1)
#define MMI_MAX_LINE_IN_SOUND_LEVEL()		(gMMI_nvram.totalLineInSoundLevel - 1)

#define MMI_LINE_IN_SMALL_STEP_LEVELS()		(gMMI_nvram.smallStepVolume.lineInVolumeSmallStep)
#define MMI_A2DP_SMALL_STEP_LEVELS()		(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep)
#define MMI_SCO_SMALL_STEP_LEVELS()			(gMMI_nvram.smallStepVolume.scoVolumeSmallStep)

#define GET_SCO_SPK_GAIN_BY_LEVEL(A)		MMI_GetSCOGainByLevel(A, TRUE)
#define GET_SCO_MIC_GAIN_BY_LEVEL(A)		MMI_GetSCOGainByLevel(A, FALSE)
#define GET_A2DP_SPK_GAIN_BY_LEVEL(A)		MMI_GetA2DPGainByLevel(A, TRUE)
#define GET_A2DP_MIC_GAIN_BY_LEVEL(A)		MMI_GetA2DPGainByLevel(A, FALSE)
#define GET_LINEIN_SPK_GAIN_BY_LEVEL(A)		MMI_GetLineInGainByLevel(A, TRUE)
#define GET_LINEIN_MIC_GAIN_BY_LEVEL(A)		MMI_GetLineInGainByLevel(A, FALSE)

#define GET_SCO_VGS_BY_LEVEL(A)				(gMMI_nvram.soundLevels[A].vgs)
#define GET_A2DP_VGS_BY_LEVEL(A)			(gMMI_nvram.musicSoundLevels[A].vgs)
#define GET_LINEIN_VGS_BY_LEVEL(A)			(gMMI_nvram.lineInSoundLevels[A].vgs)

#define GET_SCO_MIC_GAIN_BY_VGM(vgm)		(gMMI_nvram.vgsTable[vgm].micGainSetIndex)
#define GET_SCO_SPK_GAIN_BY_VGS(vgs)		(gMMI_nvram.vgsTable[vgs].spkGainSetIndex)

enum
{
	MMI_NONE_SUBSTATE,

	MMI_INIT_SYS_CONFIGED,

	MMI_SBY_NONE_SUBSTATE = MMI_INIT_SYS_CONFIGED,
	MMI_VDIAL_STOP = MMI_SBY_NONE_SUBSTATE,
	MMI_VDIAL_CONNECTING_HFP,
	MMI_VDIAL_STARTING,
	MMI_VDIAL_STARTED,
	MMI_VDIAL_STOPPING,

	MMI_RDIAL_CONNECTING_HFP,
	MMI_RDIAL_STARTING,
};


#if defined SAME_SW_DIFF_MODULE
	#define USING_DIRECT_MMI_NVARM		0
#else
	#define USING_DIRECT_MMI_NVARM		1
#endif

#if USING_DIRECT_MMI_NVARM
	MMI_EXTERN MMI_NVRAM_TYPE CODE_PTR XDATA pMMI_nvram;
	#define gMMI_nvram		(*pMMI_nvram)
	#define MMI_ReadNvramDataChunk(dst, src, len)	OSMEM_memcpy_xdata_code(dst, src, len)
#else
	MMI_EXTERN MMI_NVRAM_TYPE XDATA gMMI_nvram;
	#define MMI_ReadNvramDataChunk(dst, src, len)	OSMEM_memcpy_xdata_xdata(dst, (U8 XDATA_PTR)src, len)
#endif

#define USING_DIRECT_MMI_NVARM_KEYMAP	1

#if USING_DIRECT_MMI_NVARM_KEYMAP
	MMI_EXTERN MMI_NVRAM_KEYMAP CODE_PTR XDATA pMMI_nvram_keymap;
	#define gMMI_nvram_keymap		(*pMMI_nvram_keymap)
#else
	MMI_EXTERN MMI_NVRAM_KEYMAP XDATA gMMI_nvram_keymap;
#endif

#if defined(AUDIO_BIST) && USING_DIRECT_MMI_NVARM
	#define USING_DIRECT_NVRAM_SPKGAINSET	0
#else
	#define USING_DIRECT_NVRAM_SPKGAINSET	1
#endif

#if USING_DIRECT_NVRAM_SPKGAINSET
	#define MMI_SPK_GAINSET			gMMI_nvram.spkGainSet
	#define MMI_SPK_GAINSET_SCOWB	gMMI_nvram.spkGainSetSCOWB
#else
	MMI_EXTERN SpkGainSet XDATA gMMI_mem_spkGainSet[SOUND_LEVEL_NO];
	MMI_EXTERN SpkGainSetSCOWB XDATA gMMI_mem_spkGainSetSCOWB[SOUND_LEVEL_NO];
	#define MMI_SPK_GAINSET			gMMI_mem_spkGainSet
	#define MMI_SPK_GAINSET_SCOWB	gMMI_mem_spkGainSetSCOWB
#endif

#define USING_DIRECT_NVRAM_MICGAINSET	1

#if USING_DIRECT_NVRAM_MICGAINSET
	#define MMI_MIC_GAINSET			gMMI_nvram.micGainSet
	#define MMI_MIC_GAINSET_SCOWB	gMMI_nvram.micGainSetSCOWB
#else
	MMI_EXTERN MicGainSet XDATA gMMI_mem_micGainSet[SOUND_LEVEL_NO];
	MMI_EXTERN MicGainSetSCOWB XDATA gMMI_mem_micGainSetSCOWB[SOUND_LEVEL_NO];
	#define MMI_MIC_GAINSET			gMMI_mem_micGainSet
	#define MMI_MIC_GAINSET_SCOWB	gMMI_mem_micGainSetSCOWB
#endif

#define USING_DIRECT_NVRAM_MICGAINSET_UNDER_A2DP_MODE	1

#if USING_DIRECT_NVRAM_MICGAINSET_UNDER_A2DP_MODE
	#define MMI_MIC_GAINSET_UNDER_A2DP_MODE	gMMI_nvram.micGainSetA2DP
#else
	MMI_EXTERN MicGainSetA2DP XDATA gMMI_mem_micGainSet_under_a2dp_mode[SOUND_LEVEL_NO];
	#define MMI_MIC_GAINSET_UNDER_A2DP_MODE	gMMI_mem_micGainSet_under_a2dp_mode
#endif


#define USING_DIRECT_NVRAM_MISC_TYPE 0

#if USING_DIRECT_NVRAM_MISC_TYPE
	#define MMI_DIS_PDL_LESS_THAN  gMMI_nvram.discoverableIfPdlLessThan

	#define MMI_MAX_PAGE_DEVICE gMMI_nvram.maxPageDevice

	#define MMI_CONNECTION gMMI_nvram.mmiFeatureCtl.Connection

	#define MMI_PNP_PRODUCT_ID gMMI_nvram.productType.pnpProductID

	#define MMI_PNP_PRODUCT_VER gMMI_nvram.productType.pnpProductVersion

	#define MMI_AUDIO_PRIORITY gMMI_nvram.audioPriority.priority

	#define MMI_FEATURE_CTL_AUDIO3 gMMI_nvram.mmiFeatureCtl.Audio3

	#define MMI_AUX_FS_SEL gMMI_nvram.airPara.AUX_FS_SEL

	#define MMI_FEATURE_CTL_A2DP gMMI_nvram.mmiFeatureCtl.A2DP

	#define MMI_SIMPLE_PAIRING_PARA gMMI_nvram.simplePairingPara
#else
	MMI_EXTERN  U8 XDATA  gMMI_discoverableIfPdlLessThan;
	#define MMI_DIS_PDL_LESS_THAN gMMI_discoverableIfPdlLessThan

	MMI_EXTERN  U8 XDATA  gMMI_maxPageDevice;
	#define MMI_MAX_PAGE_DEVICE gMMI_maxPageDevice

	MMI_EXTERN  U16 XDATA  gMMI_connection;
	#define MMI_CONNECTION gMMI_connection

	MMI_EXTERN  U16 XDATA  gMMI_pnp_product_id;
	#define MMI_PNP_PRODUCT_ID gMMI_pnp_product_id

	MMI_EXTERN  U16 XDATA  gMMI_pnp_product_ver;
	#define MMI_PNP_PRODUCT_VER gMMI_pnp_product_ver

	MMI_EXTERN U8 XDATA gMMI_audio_priority[AUDIO_DEVICE_NUM];
	#define MMI_AUDIO_PRIORITY gMMI_audio_priority

	MMI_EXTERN U16 XDATA gMMI_feature_ctl_audio3;
	#define MMI_FEATURE_CTL_AUDIO3 gMMI_feature_ctl_audio3

	MMI_EXTERN U8 XDATA gMMI_aux_fs_sel;
	#define MMI_AUX_FS_SEL gMMI_aux_fs_sel

	MMI_EXTERN U16 XDATA gMMI_feature_ctl_a2dp;
	#define MMI_FEATURE_CTL_A2DP gMMI_feature_ctl_a2dp

	MMI_EXTERN SimplePairingPara XDATA gMMI_simple_pairing_para;
	#define MMI_SIMPLE_PAIRING_PARA gMMI_simple_pairing_para

#endif


	MMI_EXTERN MMI_DRIVER_VARIATION_NVRAM_TYPE XDATA gMMI_driver_variation_sector ALIGN(4);
	#define gMMI_driver_variation_nvram		gMMI_driver_variation_sector

	PUBLIC void MMI (void);
	PUBLIC void MMI_Init (void);

	PUBLIC void MMI_SniffDisable(U8 linkIndex, U8 sniffProfile);
	PUBLIC void MMI_SniffEnable(U8 linkIndex, U8 sniffProfile);

	PUBLIC U16 MMI_GetConnHandleByLinkIndex(U8 linkIndex);

	PUBLIC BOOL MMI_AIR_IsTWSApplication(void);


	//---------Profile Descriptor----------------------

	typedef struct stL2capChannel L2CAP_CHANNEL;
	typedef L2CAP_CHANNEL XDATA_PTR	L2CAP_CHANNEL_HDL;

	enum
	{
		PROTOCOL_EVT_TO_MMI_AUTHENTICATION_REQ	= 0x40,
		PROTOCOL_EVT_TO_MMI_AUTHENTICATION_ONGOING,
		PROTOCOL_EVT_TO_MMI_ROLE_SWITCH_TO_MASTER_ONGOING,
		PROTOCOL_EVT_TO_MMI_ROLE_SWITCH_TO_SLAVE_ONGOING,
	};

	PUBLIC BOOL Protocol_SendAuthenticationReqToMMI(L2CAP_CHANNEL_HDL l2capChannelHdl, U16 psm, U8 isPassive);
	PUBLIC void Protocol_CancelAuthenticationReqToMMI(L2CAP_CHANNEL_HDL l2capChannelHdl);

	enum
	{
		MMI_CMD_TO_SDAP_SERVICE_SEARCH = 0x01,
		MMI_CMD_TO_SDAP_ATTRIBUTE_SEARCH,
		MMI_CMD_TO_SDAP_SERVICE_SEARCH_ATTRIBUTE,
		MMI_CMD_TO_PROTOCOL_REJECT_AUTHENTICATION_REQ,
		MMI_CMD_TO_PROTOCOL_ACCEPT_AUTHENTICATION_REQ,
	};

	PUBLIC BOOL MMI_IsRoleOfMaster(U8 linkIndex);
	PUBLIC BOOL MMI_IsRoleOfSlave(U8 linkIndex);
	PUBLIC BOOL MMI_MasterRoleAvailable(U8 linkIndex);


	#define CMD_WAITING		FALSE
	#define CMD_COMPLETE	TRUE

	enum
	{
		PROFILE_DISCONNECTED,
		PROFILE_CONNECTING,
		PROFILE_CONNECTED,
		PROFILE_DISCONNECTING,

		PROFILE_STATE_START = 0x10,
	};

	typedef struct stMmiProfileInfo MMI_PROFILE_INFO;
	typedef struct stMmiProfileDesc MMI_PROFILE_DESC;

 	#define PROFILE_LINK_EVENT_CONN_REQUEST		(0xF3)
 	#define PROFILE_LINK_EVENT_CONN_COMPLETE	(0xF4)
	#define PROFILE_LINK_EVENT_DISC_COMPLETE	(0xF5)
	#define PROFILE_LINK_EVENT_DISC_LINKLOSS	(0xF6)

 	#define PROFILE_LINK_RESULT_CONN_ACCEPT		0
 	#define PROFILE_LINK_RESULT_CONN_REJECT		1
	#define PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC 2

	typedef union
	{
		struct {
			U8 profileId; //IN param
			U8 result; //OUT param
			MMI_PROFILE_INFO XDATA_PTR profileInfo; //OUT param
		} connRequest;
		struct {
			U8 status; //IN param
			U8 isPassive; //IN param
			U16 maxFrameSize; //IN param
			U8 initTxCredits; //IN param
		} connComplete;
	} LINK_EVENT_PARAMS;

	typedef void (*PROFILE_TASK_FUNC)(U8 linkIndex);
	typedef MMI_PROFILE_INFO XDATA_PTR (*PROFILE_CONNECT_REQUEST)(U8 linkIndex, U8 profileId);
	typedef U8 (*PROFILE_DISCONNECT_REQUEST)(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
	typedef void (*PROFILE_LINK_EVENT)(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
	typedef void (*PROFILE_DATA_EVENT)(U8 linkIndex, U8 XDATA_PTR dataPtr);

	struct stMmiProfileDesc
	{
		U8 profileId;
		PROFILE_TASK_FUNC			ProfileTaskFunc;
		PROFILE_CONNECT_REQUEST		ProfileConnectRequest;
		PROFILE_DISCONNECT_REQUEST	ProfileDisconnectRequest;
		PROFILE_LINK_EVENT			ProfileLinkEvent;
		PROFILE_DATA_EVENT			ProfileDataEvent;
	};

	typedef struct stMmiProfileNode MMI_PROFILE_NODE;

	struct stMmiProfileNode
	{
		MMI_PROFILE_DESC CODE_PTR profileDesc;
		MMI_PROFILE_NODE XDATA_PTR nextNode;
	};

	struct stMmiProfileInfo
	{
		MMI_PROFILE_DESC CODE_PTR profileDesc;
		MMI_PROFILE_INFO XDATA_PTR nextProfile;
		U8 state:7;
		U8 isAlive:1;
	};

	PUBLIC void MMI_RegisterProfile(MMI_PROFILE_NODE XDATA_PTR profileNode);
	PUBLIC MMI_PROFILE_DESC CODE_PTR MMI_GetProfileDesc(U8 profileId);

	PUBLIC void MMI_LoadProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo, MMI_PROFILE_DESC CODE_PTR profileDesc);
	PUBLIC MMI_PROFILE_DESC CODE_PTR MMI_UnloadProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
	PUBLIC void MMI_ActivateProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
	PUBLIC void MMI_SetReconnectProfile(U8 linkIndex, U8 profileId, U32 waitingTime);
	PUBLIC void MMI_PROTOCOL_L2CAPDisconnected(U8 linkIndex, U8 reason, U8 use_enlarged_wait_detach_time);
	PUBLIC BOOL MMI_IsLinkActiveDisconnecting(U8 linkIndex);

	PUBLIC U8 XDATA_PTR MMI_GetBdAddrByLinkIndex(U8 linkIndex);
	PUBLIC U8 MMI_EnterFlashProgramWorkaround(void);
	PUBLIC void MMI_ExitFlashProgramWorkaround(U8);
	PUBLIC void MMI_DisableLcHwInt(void);
    PUBLIC void MMI_EnableLcHwInt(void);

	#ifdef SPP_Profile
	PUBLIC void MMI_HCI_SendSPPEventToUART(U8 linkIndex, U8 cmdOpcode);
	PUBLIC void MMI_HCI_SendSPPDataToUART(U8 linkIndex, U8 XDATA_PTR msgPtr, U8 cmdOpcode);
	#endif

	PUBLIC void MMI_SECTOR_SetUpdateFlag(BOOL isUpdate);

	PUBLIC BOOL MMI_IsLocalDeviceType(U8 deviceType);
	PUBLIC BOOL MMI_IsSpeakerProduct(void);	
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	PUBLIC BOOL MMI_GetAudioTransparencyEnable(void);
	#endif
#endif

