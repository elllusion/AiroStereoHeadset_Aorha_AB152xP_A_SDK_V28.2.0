//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_HCI_H_
	#define _MMI_HCI_H_

	#include "..\GAP\GAP_interface.h"
	#include "MMI_Base.h"
	
	
	enum
	{
		MODE_ACTIVE,
		MODE_HOLD,
		MODE_SNIFF,
		MODE_PARK,
		MODE_ACTIVATING,
		MODE_SNIFFING,
		MODE_RESERVED
	};

	enum
	{
		INQUIRY_GIAC,
		INQUIRY_LIAC,
	};
	
	enum
	{
		FLASH_UPDATE_OFF,
		FLASH_UPDATE_ON,
		FLASH_DONGLE_AIRUPDATE,
		FLASH_NOT_DONGLE_AIRUPDATE,
	};
	
	enum
	{
		HCI_LINK_POLICY_DISABLE_ALL_MODE		=	0x0000,
		HCI_LINK_POLICY_ENABLE_ROLE_SWITCH		=	0x0001,
		HCI_LINK_POLICY_ENABLE_HOLD_MODE		=	0x0002,
		HCI_LINK_POLICY_ENABLE_SNIFF_MODE		=	0x0004,
		HCI_LINK_POLICY_ENABLE_PARK_STATE		=	0x0008,
	};
	
	#define INQUIRY_IDLE		0
	#define INQUIRY_BUSY_NDK	1
	#define INQUIRY_OK_NDK		2
	#define INQUIRY_BUSY_SDK	3
	#define INQUIRY_OK_SDK		4
	
	#define	TWS_SUPERVISION_TIMEOUT			0x1F40	//0x1F40=8000,8000*0.625=5000(ms)=5(s)

	PUBLIC void MMI_HCI_Handler (void);
	
	
	/* Link Control Command */
	PUBLIC void MMI_HCI_WriteLinkPolicySetting(U8 linkIndex, U16 link_policy_settings);
	PUBLIC void MMI_HCI_SendInquiry (U8 inquiryType);
	PUBLIC void MMI_HCI_SendInquiryCancel(void);
	PUBLIC void MMI_HCI_SendCreateConnection(U8 linkIndex, U8 XDATA_PTR bd_addr_ptr, U8 type, U8 XDATA_PTR codPtr);
	PUBLIC void MMI_HCI_SendRelSco(U8 linkIndex);	
	PUBLIC void MMI_HCI_SendRelAcl(U8 linkIndex);
	PUBLIC void MMI_HCI_SendCreateConnectionCancel(U8 XDATA_PTR bd_addr_ptr);
	PUBLIC void MMI_HCI_SendChangeConnectionPacketType (U8 linkIndex, U8 pakType);
	PUBLIC void MMI_HCI_SendAuthenticationRequested(U8 linkIndex);
	PUBLIC void MMI_HCI_SendSetConnectionEncryption(U8 linkIndex);	
	PUBLIC void MMI_HCI_SendReadRemoteName(U8 linkIndex);
	PUBLIC void MMI_HCI_SendSetupESCO(U8 linkIndex, U16 scoPktType);
	PUBLIC void MMI_HCI_SendConfirmationReply(BOOL isAccept, U8 XDATA_PTR bdAddrPtr);
	
	
	/* Link Policy Command */
	#if 0 //for BQB
	PUBLIC void MMI_HCI_SendHoldMode(U8 linkIndex);
	#endif
	PUBLIC void MMI_HCI_SendSniffMode(U8 linkIndex);
	PUBLIC void MMI_HCI_SendExitSniffMode(U8 linkIndex);
	PUBLIC void MMI_HCI_SendSwitchRole(U8 linkIndex, U8 role);
	PUBLIC void MMI_HCI_SendSniffSubrating(U8 linkIndex);
	
	/* Controller & Baseband Command */
	PUBLIC void MMI_HCI_SendWritePageTimeOut(U16 timeout);
	PUBLIC void MMI_HCI_SendWritePageScanActivity(void);
	PUBLIC void MMI_HCI_SendWriteLinkSuperVisionTimeout(U8 linkIndex, U16 timeout);
	PUBLIC void MMI_HCI_SendWriteInquiryMode(U8 inquiryMode);
	PUBLIC void MMI_HCI_SendRefreshEncryptionKey(U8 linkIndex);
	
	/* Status Parameter Command */
	PUBLIC void MMI_HCI_SendReadRSSI(U8 linkIndex);
	
	/* Testing Command */
	PUBLIC void MMI_HCI_SendWriteSimplePairingDebugMode(U8 isON);
	PUBLIC void MMI_HCI_SendEnableDeviceUnderTestMode(void);
	
	
	PUBLIC BOOL MMI_HCI_SendVCmdAudioSwitch(U8 linkIndex);
	PUBLIC void MMI_HCI_SendVCmdAudioEnhanceSCOLatency(BOOL isEnable);
	PUBLIC void MMI_HCI_SendVCmdAudioSCONullPacket(U8 linkIndex, BOOL isPktNull);
	PUBLIC void MMI_HCI_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull);
	PUBLIC void MMI_HCI_SendVCmdWriteLocalBdAddr(U8 XDATA_PTR bdPtr);
	PUBLIC void MMI_HCI_SendVCmdLabTestStart(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestUpdateUapLap(U8 XDATA_PTR sourcePtr);
	PUBLIC void MMI_HCI_SendVCmdLabTestPause(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestSwitchMCUTo12M(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestLTAddrInTxPkt(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestReportRateSetup(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestSingleToneSetup(U8 channel, U16 gc, U16 bias);
	PUBLIC void MMI_HCI_SendVCmdLabTestTxModemManualOff(void);
	PUBLIC void MMI_HCI_SendVCmdLabTestContinueTxData(HCI_VCMD_PARA_LABTEST_CTX_DATA XDATA_PTR dataPtr);
	PUBLIC void MMI_HCI_SendVCmdLabTestBurstTxPacket(HCI_VCMD_PARA_LABTEST_BTX_PACKET XDATA_PTR dataPtr);
	PUBLIC void MMI_HCI_SendVCmdLabTestContinueRxStart(U8 channel, U8 HighSide, U8 ReptRssi);

	PUBLIC void MMI_HCI_SendToolCommandResponse(U8 ocf, U8 resultCode);
	
	PUBLIC void MMI_HCI_SendToolCommandResponseWithData(U8 ocf, U8 resultCode, U16 reportADCvalue);
	
	PUBLIC void MMI_HCI_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex);
	PUBLIC void MMI_HCI_SendVCmdAvrcpEventToUART(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr, U8 evtCode);

    #ifdef TRSPX_App
    PUBLIC void MMI_HCI_SendTrspXUARTEvent(MMIMsgType XDATA_PTR msgPtr, U8 cmdOpcode);
    #endif
	
	PUBLIC void MMI_GAP_WriteSecurityMode(void);
	PUBLIC void MMI_GAP_WritePairingMode(void);
	PUBLIC void MMI_GAP_WriteConnectabilityMode(U8 connectabilityMode);
	PUBLIC void MMI_GAP_WriteDiscoverabilityMode(U8 discoverabilityMode);
	
	
	PUBLIC BOOL MMI_HCI_IsQosReady(U8 linkIndex);
	PUBLIC BOOL MMI_HCI_IsWriteLocalBdAddrBusy(void);
	
	PUBLIC void MMI_HCI_AirohaCmd(MMIMsgType XDATA_PTR msgPtr);
	
	PUBLIC void MMI_HCI_ToolDSPRefGainCmdResponse(U8 ocf, U8 resultCode);
	
	PUBLIC void MMI_HCI_SendCmd(MMIMsgType XDATA_PTR msgPtr);
	
	PUBLIC void MMI_HCI_SetInquiryOnGoing(U8 inquiryState);
	PUBLIC BOOL MMI_HCI_IsInquiryOnGoing(void);
	PUBLIC U8 MMI_HCI_GetInquiryState(void);
	PUBLIC void MMI_HCI_AirohaCmd_PassThroughData(U8 linkIndex, U8 cmdOcf, U8 cmdOgf, MMIMsgType XDATA_PTR airCmdPtr);

	PUBLIC void MMI_HCI_Init(void);
	PUBLIC void MMI_HCI_SetFlashUpdateState(U8 state);
	PUBLIC U8 MMI_HCI_GetFlashUpdateState(void);	

#endif
