#ifndef _MMI_SDAP_H_
	#define _MMI_SDAP_H_

	#ifndef _BT_CONFIG_PROFILE_H_
	#error "To include bt_config_profile.h before this file!!!"
	#endif

#include "..\SDK\include\SDK_Config.h"
	
	enum
	{
		SUPPORT_HSP 		= 0x01,
		SUPPORT_HFP 		= 0x02,
		SUPPORT_A2DP 		= 0x04,
		SUPPORT_AVRCP 		= 0x08,
		SUPPORT_SPP			= 0x10,
		#ifdef SONGPAL_Profile
		SUPPORT_SONGPAL		= 0x20,
		#endif
		#ifdef PBAP_Profile
		SUPPORT_PBAP		= 0x20,
		#endif
		#ifdef AIRAPP_Profile
		SUPPORT_AIRAPP		= 0x20,
		#endif
		#ifdef HID_Profile
		SUPPORT_HID			= 0x20,
		#endif
		SUPPORT_PNP			= 0x40,
		SDAP_IN_PROGRESS	= 0x80
	};

	enum
	{
		SDAP_SEARCH_PNP		= 0x01,
		SDAP_SEARCH_HFP		= 0x02,
		SDAP_SEARCH_HSP		= 0x04,
		SDAP_SEARCH_A2DP	= 0x08,
		SDAP_SEARCH_AVRCP	= 0x10,
		SDAP_SEARCH_SPP		= 0x20,
		#ifdef SONGPAL_Profile
		SDAP_SEARCH_SONGPAL	= 0x40,
		#endif
		#ifdef PBAP_Profile
		SDAP_SEARCH_PBAP	= 0x40,
		#endif
		#ifdef AIRAPP_Profile
		SDAP_SEARCH_AIRAPP	= 0x40,
		#endif
	};
	
	enum
	{
		MMI_SDAP_STATE_IDLE,
		MMI_SDAP_STATE_WAIT_CONNECTION_COMPLETE,
		MMI_SDAP_STATE_WAIT_PNP_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_HFP_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_HSP_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_A2DP_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_AVRCP_TG_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_AVRCP_CT_SERVICE_RESULT,
		MMI_SDAP_STATE_WAIT_SPP_SERVICE_RESULT,
		#ifdef SONGPAL_Profile
		MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT,
		MMI_SDAP_STATE_WAIT_SONGPAL_IAP2_RESULT,
		#endif
		#ifdef AIRAPP_Profile
		MMI_SDAP_STATE_WAIT_AIRAPP_SERVICE_RESULT,
		#endif
		#ifdef PBAP_Profile
		MMI_SDAP_STATE_WAIT_PBAP_SERVICE_RESULT,
		#endif
		MMI_SDAP_STATE_SEND_DISCONNECTION_REQ,
		MMI_SDAP_STATE_WAIT_DISCONNECTION_COMPLETE,
		MMI_SDAP_STATE_SERVICE_QUERY_COMPLETE,
	};

	typedef struct
	{
		U16	specId;
		U16	vendorId;
		U16	productId;
		U16	productVersion;
		U8	primaryRecord;
		U16	vendorIdSource;
	}PNPInformation;
	
	PUBLIC void MMI_SDAP_Init(void);	
	PUBLIC void MMI_SDAP_EventHandler(U8 linkIndex, U8 opCode, U8 status);
	PUBLIC void MMI_SDAP_StartServiceSearchProc(U8 linkIndex);
	PUBLIC void MMI_SDAP_ClearCtlData(U8 linkIndex);
	PUBLIC BOOL MMI_SDAP_HideServiceRecord(U8 profileId);
	PUBLIC U8 MMI_SDAP_GetRemoteServiceChannel(U8 linkIndex, U8 profileId);
	PUBLIC U32 XDATA_PTR MMI_SDAP_ParseServiceRecordHandle(U8 XDATA_PTR evtPtr);
	PUBLIC BOOL MMI_SDAP_IsQueryComplete(U8 linkIndex);
	PUBLIC BOOL MMI_SDAP_IsSupportHID(U8 linkIndex);
	PUBLIC void MMI_SDAP_SetRemoteApplicationID(U8 linkIndex, U8 productType);
	PUBLIC U8 MMI_SDAP_ParseRemoteScn(U8 XDATA_PTR evtPtr, U16 psm) ;
	PUBLIC void MMI_SDAP_SendServiceSearchReq(U8 linkIndex, U16 uuid16, U8 useType);
	PUBLIC void MMI_SDAP_SendAttributeSearchReq(U8 linkIndex, U32 serviceRecordHandle, U16 attributeId1, U16 attributeId2, U8 useType);
	PUBLIC void MMI_SDAP_SendServiceSearchAttributeReq(U8 linkIndex, U16 uuid16, U16 attirbuteId, U8 useType);
	
	PUBLIC void MMI_SDAP_UseStatusUseFinish(U8 linkIndex ,U8 sdpType);
	PUBLIC BOOL MMI_SDAP_UseStatusRegister(U8 linkIndex ,U8 sdpType);
	PUBLIC void MMI_SDAP_UseStatusDeRegister(U8 linkIndex ,U8 sdpType);
	PUBLIC U8 MMI_SDAP_UseStatusFindInUsed(U8 linkIndex);
	PUBLIC void MMI_SDAP_UseStatusSetWaitCmd(U8 linkIndex, U8 sdpType);
	PUBLIC BOOL MMI_SDAP_SetProfileSCN(U8 linkIndex, U8 profileID, U8 scn);
#endif

