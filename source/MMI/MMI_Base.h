//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _MMI_BASE_H_
	#define _MMI_BASE_H_

	#include "os.h"
	#include "bt_config_profile.h"
	#include "rc_flash.h"

	#include "MMI_Interface.h"

	#define	MMI_EOF_HANDLER				0xFFFF
	#define MAX_REMOTE_NAME_LEN			30

	// ========== MISC MASK ========== ========================
	/////    link misc
	#define MMI_LINK_LOSS					0x01
	#define MMI_LINK_DETACHING				0x02
	#define MMI_ACTIVE_CONNECT				0x04
	#define MMI_DISC_ACTIVE					0x40
	#define MMI_IS_ASSIGNED_TO_BE_ACTIVE_LINK	0x80


	//// system misc
	///Misc 2
	#define MMI_EXIT_DISCOVERABLE_STATE					0x20
	#define MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER		0x80

	/// MISC 4
	#define MMI_MASK4_RECONNECT_LINK0				0x02 
	#define MMI_MASK4_RECONNECT_LINK1				0x04
	

	//pwrSavingMask
	#define MMI_PWRSAVING_WRITE_CONNECTABILITY		0x01
	#define MMI_PWRSAVING_ENABLE_ADV                0x02

	
	//linkPwrSavingMask
	#define MMI_PWRSAVING_EXIT_SNIFF				0x01
	#define MMI_PWRSAVING_DISC						0x02
	#define MMI_PWRSAVING_PROFILE_CONNECTION		0x04
	#define MMI_SNIFF_DISABLE						0x08
	// ========== MISC MASK ====[END]== ========================

	

	PUBLIC U8 MMI_IsPowerOn(void);
	#define MMI_IS_POWER_ON() 							(MMI_IsPowerOn())
	#define MMI_SET_POWER_ON_STATUS(a) 					(gMMI_ctl.driverCtl.isPowerOn = (a) ? TRUE : FALSE )
	
	PUBLIC U8 MMI_GetKeyState(void);
	#define MMI_SetKeyState(a)							(gMMI_ctl.keyState = (a))
	
	#define MMI_SetWatchDogReboot(a)					(gMMI_ctl.driverCtl.isWatchdogReboot = (a))
	
	#define MMI_SetEnterDiscoverable(a)					(gMMI_ctl.enterDiscoverable = (a))
	#define MMI_IsEnterDiscoverable()					(gMMI_ctl.enterDiscoverable)
	
	#define MMI_SetEnterDiscoverableAfterDisc(a)		(gMMI_ctl.enterDiscoverableAfterDisc = (a))
	#define MMI_IsEnterDiscoverableAfterDisc()			(gMMI_ctl.enterDiscoverableAfterDisc)	
	
	
	#define MMI_DRV_SetReset(a)							(gMMI_ctl.driverCtl.isReset = (a))
	#define MMI_DRV_IsReset()							(gMMI_ctl.driverCtl.isReset)
	
	
	#define MMI_DRV_SetButtonLock(a)					(gMMI_ctl.driverCtl.isButtonLocked = (a))
	#define MMI_DRV_IsButtonLocked()					(gMMI_ctl.driverCtl.isButtonLocked)
	
	#define MMI_DRV_SetKeyInvalidPwOnLowBat(a)			(gMMI_ctl.driverCtl.keyInvalidInPwrOnLowBat = (a))
	#define MMI_DRV_IsKeyInvalidPwOnLowBat()			(gMMI_ctl.driverCtl.keyInvalidInPwrOnLowBat)

	#define MMI_DRV_SetNoConnectivityPwOnLowBat(a)		(gMMI_ctl.driverCtl.noConnectivityInPwrOnLowBat = (a))
	#define MMI_DRV_IsNoConnectivityPwOnLowBat()		(gMMI_ctl.driverCtl.noConnectivityInPwrOnLowBat)
	
	
	#define MMI_DRV_SetSwitchingSCO(a)					(gMMI_ctl.driverCtl.switchingSCO = (a))
	#define MMI_DRV_IsSwitchingSCO()					(gMMI_ctl.driverCtl.switchingSCO)
	
	

	
	//----------------------------Protocol Macro--------------------------
	#define SCO_EXISTS(A)				(gMMI_ctl.mmiInfo[A].linkPara.isSCOAlive == TRUE)

	//===========================Marco [END]===================================

	////
	// System Error Code
	//
	enum
	{
		NO_ERROR,
		
		SECURITY_BLOCK,
		PROTOCOL_CONNECTION_ERROR,
		PROFILE_NOT_SUPPORTED,
		REMOTE_REJECT,	

		HFP_ERROR_COMMAND_TIME_EXPIRE = 0x80
	};

	enum
	{
		POWEROFF_REASON_KEY = 1,
		POWEROFF_REASON_SHUT_DOWN_LOW_BAT,
		POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER,
		POWEROFF_REASON_PAIRING_MODE_TO,
		POWEROFF_REASOM_TEMP_PROTECT,
	};

	enum
	{
		MMI_KEY_STATE_NONE,
		MMI_KEY_STATE_POWER_ON,
		MMI_KEY_STATE_POWER_OFF,
		MMI_KEY_STATE_POWER_OFF_SYNC,
	};
	
	typedef struct
	{
		U8 keyCode;
	}MmiKeyEvent;
	
	typedef struct
	{
		U8 hciCmdPktNo;
		U16 cmdOpcode;
		U8 status;
	}GapWriteSecModeCpt;
	
	typedef struct
	{
		U8 pairingMode;
		#ifdef SUPPORT_EXTENDED_INQUIRY_RESPONSE
		U8 devNameLen;
		U8 devNameType;
		U8 devName[6];
		U8 restPara[6];
		#endif
	}GapPairingModeCmd;


	typedef struct
	{
		U8 connectability_mode;
		U8 discoverability_mode;
	}GapWriteConnetabilityModeCpt;
	
	typedef struct
	{
		U8 connectability_mode;
		U8 discoverability_mode;
	}GapWriteDiscoverabilityModeCpt;
	
	typedef struct
	{
		L2CAP_CHANNEL_HDL l2capChannelHdl;
		U16 psm;
		U8 isPassive;
	}ProtocolAuthenticationReqEvt;
	
	typedef void(*SdapSearchReqCallback)(U8 XDATA_PTR sdapRspPtr);
	
	typedef struct
	{
		U8 uuidLen;
		U8 uuid[16];
	} SdapServiceId;
	
	typedef struct
	{
		SdapSearchReqCallback callback;
		union {
			struct {
				SdapServiceId serviceId;
			} serviceSearch;
			struct {
				U32 serviceRecordHandle;
				U16 attributeId1;
				U16 attributeId2;
			} attributeSearch;
			struct {
				SdapServiceId serviceId;
				U16 attributeId;
			} serviceSearchAttribute;
		} param;
	} SdapSearchCmd;	

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		U8 AudioIn;
		U8 AudioOut;
		U8 BISTScenario;
		DriverAudioSelCmd audioSel;		
	}HciBISTCmd;
	
	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 dataBegin;
	}HciUARTAVRCPCmd;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 dataBegin;
	}HciUARTSppDataType;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 dataBegin;
	}HciUARTOppDataType;

    typedef struct
    {
        U8 ntfType;
        U8 etrPasskey;
    }TrspXportPasskeyRsp;

    typedef union
    {
        TrspXportPasskeyRsp pkRsp;
        U8 option;
        U8 dataBegin;
    }TrspXportPara;
    
    typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
        TrspXportPara para;
	}HciUARTTrspXDataType;

	#ifdef HID_Profile
	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
        U8 dataBegin[1];
	}HciUARTHIDDataType;
	#endif

    typedef struct
    {
        BD_ADDR bdAddr;
        U8 ntfType;
    }HciSendPresskeyNtfCmd;

    typedef struct
    {
        U8 status;
        BD_ADDR bdAddr;
    }HciSSPEvent;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
	}HciUARTAvrcpEvent;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 avrcpEventId;
	}HciUARTAvrcpChangedNotify;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 dataBegin;
	}HciUARTAvrcpMediaAttribute;

	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U32 playbackPos; //unit in ms
	}HciUARTAvrcpPlaybackPos;
	
	typedef struct
	{
		U8 cmdOpCode;
		U8 ogf;
		BD_ADDR bdAddr;
		U8 attributeID;
		U8 attributeValue;
	}HciUARTAvrcpPlayAppSetting;

#ifdef SAP_Profile
	typedef struct
	{
		U8 status;
		U8 linkIndex;
	}SapEvt;
#endif

	typedef struct
	{
		U8 profileId;
		U8 retryCnt;
		OST reconnectTimer;
	}ProfileReconnectEvt;
	
	typedef struct
	{
		OST waitingTimer;
		void (*fcallback)(U8 linkIndex, U8 isSuccessful);
		U8 PlayStatus;
	}AVRCPTaskCmd;
	
	#include "hci.h"
	#include "hci_airoha_flash.h"
	
	typedef struct
	{
		U8 resultCode;
	}HCI_VCMD_PARA_AIR_CMD_RESP;
	

	typedef struct
	{
		U8 resultCode;
		U8 ResDataLow;
		U8 ResDataHigh;
	}HCI_VCMD_PARA_AIR_CMD_RESP_DATA;

	typedef struct
	{
		U8 resultCode;
		U16 CH1_REF_GAIN; 
		U16 CH2_REF_GAIN; 
		U16 CH3_REF_GAIN;
		U16 CH4_REF_GAIN;
		U16 CH1_REF2_GAIN; 
		U16 CH2_REF2_GAIN; 
		U16 CH3_REF2_GAIN;
		U16 CH4_REF2_GAIN;

	}HCI_VCMD_PARA_AIR_CMD_RESP_DSP_DATA;
	
	typedef struct
	{
		U8 batStatus;
	}HCI_VCMD_PARA_CHECK_BAT;
	
	typedef struct
	{
		U8 BdAddr[6];
	}HCI_VCMD_PARA_READ_BDADDR;
	
	typedef struct
	{
		U8 chargerStatus;
	}HCI_VCMD_PARA_CHECK_CHARGER;
	
	typedef struct
	{
		U8 serviceIndex;
	}HCI_VCMD_PARA_QUERY_SERVICE_IND;
	
	typedef struct
	{
		U8 linkKey[16];
	}HCI_VCMD_PARA_QUERY_LINK_KEY;
	
	typedef struct
	{
		U8 i2cDataBegin;
	}HCI_VCMD_PARA_I2C_READ;	
	
	typedef struct
	{
		U8 BdAddr[6];
		U8 nameBegin;
	}HCI_VCMD_PARA_GET_REMOTE_NAME;
	
	typedef struct
	{
		U8 majorVersion;
		U8 minorVersion;
		U8 buildNumber;
	}HCI_VCMD_PARA_GET_FW_VERSION;
	
	typedef struct
	{
		U8 inquiryType;//INQUIRY_GIAC:0, INQUIRY_LIAC:1
	}HCI_VCMD_PARA_INQUIRY;
	
	typedef struct
	{
		BD_ADDR bd_addr;
	}HCI_VCMD_PARA_CREATE_CONN;

	typedef struct
	{
		U8 result;
		S16 signTempADCBiggerValue;
		S16 signTempADCSmallerValue;
		S16 signTempADCGroundValue;
		FLOAT ratioValue;
	}HCI_VCMD_PARA_NTC_VALUE_DATA;
	
	typedef struct
	{
		U8 bandwidth;
		U8 component;
		U8 soundLevel;
		U8 spkDigitalGain;
		U8 spkAnalogGain;
		U8 micDigitalGain;
		U8 micAnalogGainR;
		U8 micAnalogGainL;
		U8 smallStepLevel;
	}HCI_VCMD_PARA_AIR_CMD_GET_VOL_GET;

	typedef struct
	{
		U8 spkDigitalGain;
		U8 spkAnalogGain;
		U8 micDigitalGain;
		U8 micAnalogGainR;
		U8 micAnalogGainL;
	}HCI_VCMD_PARA_AIR_CMD_SET_CURR_GAIN;

	typedef struct
	{
		U16 OCF;
		U8 result;
	}HCI_VCMD_PARA_EXT_UART_PARA;

	typedef struct
	{
		U8 bdAddr[6];
		U8 dataStart;
	}HCI_VCMD_PARA_AIRAPP_CMD_PASSTHROUGH;

	typedef struct
	{
		U16 mmiKeyCode;
	}HCI_VCMD_PARA_AIRAPP_CMD_MMI_KEY_EVENT;
	
	typedef struct
	{
		U8 audioTsprcySoundLv;
	}HCI_VCMD_PARA_AIRAPP_CMD_SET_AT_GAIN;

	typedef struct
	{
		U8 channelInfo;
	}HCI_VCMD_PARA_AIRAPP_CMD_CHANNEL_INFORMATION;
	
	typedef struct
	{
		U16 mmiKeyCode;
		U8 result;
	}HCI_VEVT_PARA_AIRAPP_MMI_KEY_CODE_RSP;
	
	typedef struct
	{
		U8 peqMode;
		U8 peqModeIndex;
		U8 sectorMode;
	}HCI_VCMD_PARA_AIRAPP_CMD_SET_PEQ_PARA;

	typedef union
	{
		U8 dataBegin;
		U8 AdcChannel;		

		HCI_VCMD_PARA_AUDIO_SWITCH					audioSwitchCmd;			//06.01 (HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_AUDIO_SWITCH)
		HCI_VCMD_PARA_ENHANCE_ESCO_LATENCY			enhanceESCOLatencyCmd;	//06.04 (HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_ENHANCE_ESCO_LATENCY)
		HCI_VCMD_PARA_SCO_NULL_PACKET				scoNullPktCmd;			//06.05 (HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_ESCO_NULL_PACKET)
		HCI_VCMD_PARA_AUDIO_SETUP_QOS				setupQosCmd;			//06.06	(HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_SETUP_QOS)
		
		HCI_VCMD_PARA_GNL_WRITE_BD_ADDR				writeLocalBdAddrCmd;	//(HCI_VENDOR_OGF_GENERAL_V2,HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR)
		
		HCI_VCMD_PARA_LABTEST_CFG_UAP_LAP 			configUAPLAPCmd;		//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_CONFIG_UAP_AND_LAP)
		HCI_VCMD_PARA_LABTEST_TX1_SETUP 			singleToneSetupCmd;		//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_SINGLE_TONE_SETUP)
		HCI_VCMD_PARA_LABTEST_CTX_DATA				cTxDataCmd;				//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_CTX_DATA)
		HCI_VCMD_PARA_LABTEST_BTX_PACKET 			bTxPktCmd;				//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_BTX_PACKET)
		HCI_VCMD_PARA_LABTEST_CRX_START 			cRxStartCmd;			//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_CRX_START)
		HCI_VCMD_PARA_LABTEST_CFG_WR_SFR 			writeSFRCmd;			//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_CONFIG_WRITE_SFR)
		HCI_VCMD_PARA_LABTEST_RPT_RATE_SETUP 		reportRateSetupCmd;		//(HCI_VENDOR_OGF_LABTEST, HCI_VCMD_OCF_LABTEST_REPORT_RATE_SETUP)
		
		
		HCI_VCMD_PARA_AIR_CMD_RESP					respVEvtCmd;			//(HCI_VCMD_AIROHA_TO_UART, XX)
		HCI_VCMD_PARA_AIR_CMD_RESP_DATA				respVEvtCmdData;		//(HCI_VCMD_AIROHA_TO_UART, HCI_VCMD_OCF_TOOL_GET_ADC_DATA)
		HCI_VCMD_PARA_AIR_CMD_RESP_DSP_DATA			respVEvtCmdDSPData;		//(HCI_VCMD_AIROHA_TO_UART, HCI_VCMD_OCF_TOOL_GET_DSP_REF_GAIN)
		HCI_VCMD_PARA_NTC_VALUE_DATA				respNTCValueData;		//HCI_VCMD_OCF_TOOL_NTC_RATIO_VALUE_REPORT
		
		HCI_VCMD_PARA_CHECK_BAT						checkBatVEvtCmd;		//(HCI_VENDOR_OGF_UART_IND_SOLICITED, KEY_CHECK_BATTERY_STATUS)
		HCI_VCMD_PARA_READ_BDADDR					readBdAddrVEvtCmd;		//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_READ_BD_ADDR)
		HCI_VCMD_PARA_CHECK_CHARGER					checkChargerVEvtCmd;	//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_CHECK_CHARGER_STATUS)
		HCI_VCMD_PARA_QUERY_SERVICE_IND				queryServiceIndVEvtCmd;	//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_QUERY_SERVICE_INDEX)
		HCI_VCMD_PARA_QUERY_LINK_KEY				queryLinkKeyVEvtCmd;	//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_QUERY_LINKKEY_BY_BDADDR)
		HCI_VCMD_PARA_I2C_READ						i2cReadVEvtCmd;			//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_I2C_READ)
		HCI_VCMD_PARA_GET_REMOTE_NAME				getRemoteNemeVEvtCmd;	//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_GET_REMOTE_NAME)
		HCI_VCMD_PARA_GET_FW_VERSION				fwVersionVEvtCmd;		//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_GET_FW_VERSION)
		HCI_VCMD_PARA_INQUIRY						inquiryVEvtCmd;			//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_INQUIRY)
		HCI_VCMD_PARA_CREATE_CONN					createConnCmd;			//(HCI_VENDOR_OGF_UART_IND_SOLICITED, UART_CMD_CREATE_CONNECTION)

		HCI_VCMD_PARA_AIR_CMD_GET_VOL_GET			getVolCmd;
		HCI_VCMD_PARA_AIR_CMD_SET_CURR_GAIN			setCurrGainCmd;			//(HCI_VENDOR_OGF_UART_TOOL_CMD, HCI_VCMD_OCF_TOOL_SET_CURRENT_GAIN)
		
		HCI_VCMD_PARA_AIRAPP_CMD_SET_VOL			setVolCmd;				//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_VOL)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_VP_LANG		setVpLangCmd;			//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_VP_LANG)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_PEQ_A2DP		setPEQA2DPCmd;			//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_PEQ_A2DP)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_PEQ_AUX		setPEQAUXCmd;			//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_PEQ_AUX)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_CALLERNAME 	setCallerNameCmd;		//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_CALLERNAME)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_DEFAULT_PEQ	setDefaultPEQCmd;		//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_DEFAULT_PEQ_MODE)
		HCI_VCMD_PARA_AIRAPP_CMD_PASSTHROUGH		passThroughCmd;			//(HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_CMD, AIAPP_CMD_PASSTHROUGH_FROM_APP_TO_UART)
		HCI_VCMD_PARA_AIRAPP_CMD_MMI_KEY_EVENT		airMMIKeyEventCmd;		//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_KEY_MMI_KEY_EVENT)
		HCI_VCMD_PARA_AIRAPP_CMD_SET_AT_GAIN		setATGainCmd;			//(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_SET_MASTER_AT_GAIN_CHANGE || AIRAPP_CMD_SET_SLAVE_AT_GAIN_CHANGE)
	
		HCI_VCMD_PARA_AIRAPP_CMD_SET_PEQ_PARA   	setPEQParaCmd;
		
		HCI_VCMD_PARA_EXT_UART_PARA 				extendedPara;				//(HCI_VENDOR_OGF_EXTENDED_TWO_BYTE_OCF_CMD, HCI_VENDOR_OCF_SECOND_KEY_EVENT_CMD)
	}HCIVendorCmdParaType;
	
	typedef struct
	{
		HCI_VCMD_OPCODE_STRU VOpCode;
		HCIVendorCmdParaType	VPara;
	}HCIVendorCmdType;	
	
	typedef struct
	{
		U8 BdAddr[6];
		U16 index;
	}HCI_VEVT_PARA_KEY_DIAL_MEM;
	
	typedef struct
	{
		U8 BdAddr[6];
		U8 length;
		U8 nameDataBegin;
	}HCI_VEVT_PARA_KEY_DIAL_NUM;
	
	typedef struct
	{
		U8 reconnectIndex;
	}HCI_VEVT_PARA_RECONNECT;
	
	typedef struct
	{
		U8 nameBegin;
	}HCI_VEVT_PARA_WRITE_DEV_NAME;
	
	typedef struct
	{
		U8 type;
	}HCI_VEVT_PARA_QUERY_LINK_KEY;	
	
	typedef struct
	{
		U8 vpIndex;
	}HCI_VEVT_PARA_PLAY_VP;	
	
	
	typedef struct
	{
		U8 linkIndex;
		U8 scoSoundLevel;
		U8 scoSmallStep;
		U8 scoVgm;
		U8 a2dpSoundLevel;
		U8 a2dpSmallStep;
		U8 lineInSoundLevel;
		U8 lineInSmallStep;
	}HCI_VEVT_PARA_VOL_SET;
	
	typedef struct
	{
		U8 isAccept;
		U8 BdAddr[6];
	}HCI_VEVT_PARA_PAIRING_CONF;
	
	typedef struct
	{
		U8 vol;
	}HCI_VEVT_PARA_AVRCP_VOL;
	
	typedef struct
	{
		U8 settings;
	}HCI_VEVT_PARA_HQ_CODEC;	
	
	typedef struct
	{
		U8 deviceAddr;
		U8 regAddr;
		U16 dataLength;
	}HCI_VEVT_PARA_I2C_READ;
	
	typedef struct
	{
		U8 deviceAddr;
		U8 regAddr;
		U16 dataLength;
		U8 dataBegin;
	}HCI_VEVT_PARA_I2C_WRITE;
	
	typedef struct
	{
		U8 BdAddr[6];
		U8 profileId;
		U8 isSupported;
	}HCI_VEVT_PARA_PROFILE_SUPPORT;
	
	typedef struct
	{
		U8 LedR;
		U8 LedG;
		U8 LedB;		
	}HCI_VEVT_PARA_LED_RGB;

	typedef struct
	{
		U16 OCF;
		U8 BdAddr[6];
	}
	HCI_VEVT_PARA_EXTENDED_OCF_UART;

	typedef struct
	{
		U8 vOcf;
		U8 vOgf;
		U8 status;
	}HCI_VEVT_PARA_VCMD_CPL;

	
	typedef struct
	{
		U8 result;
		U8 dataStart;
	}HCI_VEVT_PARA_AIRAPP_CUSTOMER_RSP;
 
	typedef struct
	{
		U8 offset;
		U8 dataSize;
	}HCI_VEVT_PARA_AIRAPP_PARSING_AE_SECTOR_INFO;

	typedef struct
	{
		U8 followerOcf;
		U8 followerOgf;
		U8 paraData[1];
	}HCI_VEVT_PARA_AIRAPP_FOLLOWRE_OPCODE_INFO;
	
	typedef union
	{
		U8 dataBegin;
		U8 BdAddr[6];
		U8 followerData[1];
		U8 resetLinkType;									//(HCI_VENDOR_OGF_UART_CMD, KEY_RESET_PAIRED_DEVICES)
		U8 hfpDTMFIndex;									//(HCI_VENDOR_OGF_UART_CMD, KEY_SEND_DTMF)
		HCI_VEVT_PARA_KEY_DIAL_NUM		dialNumPara;		//(HCI_VENDOR_OGF_UART_CMD, KEY_DIALNUM)
		HCI_VEVT_PARA_KEY_DIAL_MEM 		dialMemPara;		//(HCI_VENDOR_OGF_UART_CMD, KEY_DIALMEM)
		
		HCI_VEVT_PARA_RECONNECT			reconnectPara;		//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_RECONNECT_TO_INDEX)
		HCI_VEVT_PARA_WRITE_DEV_NAME	writeDevNamePara;	//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_WRITE_DEVICE_NAME)
		HCI_VEVT_PARA_QUERY_LINK_KEY	queryLinkKeyPara;	//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_QUERY_LINK_HISTORY)
		HCI_VEVT_PARA_PLAY_VP			playVPPara;			//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_PLAY_VOICE_PROMPT)
		HCI_VEVT_PARA_VOL_SET			volSetPara;			//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_VOLUME_SET)
		HCI_VEVT_PARA_PAIRING_CONF		pairingConfPara;	//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_PAIRING_CONFIRM)
		HCI_VEVT_PARA_AVRCP_VOL			avrcpVolPara;		//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_SET_AVRCP_VOL)
		HCI_VEVT_PARA_HQ_CODEC			hqCodecPara;		//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_HIGH_QUALITY_AUDIO_CODEC)
		HCI_VEVT_PARA_I2C_READ			i2cReadPara;		//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_I2C_READ)
		HCI_VEVT_PARA_I2C_WRITE			i2cWritePara;		//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_I2C_WRITE)
		HCI_VEVT_PARA_PROFILE_SUPPORT	profileSupportPara;	//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_SET_PROFILE_SUPPORT_STATUS)
		HCI_VEVT_PARA_LED_RGB			ledRGB;
		HCI_VEVT_PARA_EXTENDED_OCF_UART extendedPara;
		U8 DSPSoundbarDownsampleSel;						//(HCI_VENDOR_OGF_UART_CMD, UART_CMD_DSP_FREQUENCT_SELECT)

		HCI_VEVT_PARA_VCMD_CPL			vCmdCplPara;		//(HCI_VENDOR_OGF_GENERAL_V2, HCI_VEVT_OCF_COMMAND_COMPLETE_V2)
		HCI_VEVT_PARA_AIRAPP_CUSTOMER_RSP	airAppCstRsp;	//HCI_VENDOR_OGF_AIRAPP_CUSTOMER_RSP
		HCI_VEVT_PARA_AIRAPP_PARSING_AE_SECTOR_INFO sectorAEInfo;
		HCI_VEVT_PARA_AIRAPP_FOLLOWRE_OPCODE_INFO followerInfo;
	}HCIVendorEvtParaType;
	
	typedef struct
	{
		HCI_VCMD_OPCODE_STRU 	VOpCode;
		HCIVendorEvtParaType	VPara;
	}HCIVendorEvtType;
	
	#ifdef HFP_Profile
	#include "..\HFP\HFP_Interface.h"
	#endif
	
	typedef union 
	{
		U16 connHandle;
		U8 gapPara;
			
		MmiKeyEvent mmiKeyEvent;
		GapWriteSecModeCpt gapWriteSecCpt;
		GapPairingModeCmd gapPairingModeCmd;
		GapWriteConnetabilityModeCpt gapWriteConnetabilityModeCpt;
		GapWriteDiscoverabilityModeCpt gapWriteDiscoverabilityModeCpt;

		ProtocolAuthenticationReqEvt protocolAuthReqEvt;
		SdapSearchCmd sdapSearchCmd;

		HciBISTCmd hciBISTCmd;
		HciUARTSppDataType hciUARTSppData;
		HciUARTOppDataType hciUARTOppData;
		HciUARTAVRCPCmd hciUARTAvrcpCmd;
		HciUARTAvrcpEvent hciUARTAvrcpEvent;
		HciUARTAvrcpChangedNotify hciUARTAvrcpChangedNotify;
		HciUARTAvrcpMediaAttribute hciUARTAvrcpMediaAttribute;
		HciUARTAvrcpPlaybackPos hciUARTAvrcpPlaybackPos;
		HciUARTAvrcpPlayAppSetting hciUARTAvrcpPlayApp;
		HciUARTTrspXDataType hciUARTTrspXData;
		HciSendPresskeyNtfCmd hciSendPresskeyNtfCmd;
		HciSSPEvent hciSSPEvt;
	#ifdef SAP_Profile
		SapEvt sapEvt;
	#endif
		ProfileReconnectEvt profileReconnectEvt;
		AVRCPTaskCmd	avrcpTaskCmd;
		
		
		HCICmdParaType		hciCmd;
		HCIEvtParaType		hciEvt;
		
		HCIVendorCmdType 	hciVCmd;
		HCIVendorEvtType	hciVEvt;

		DriverCmdType 		driverCmd;
		DriverEvtType		driverEvt;
		
		#ifdef HFP_Profile
		HFPCmdType			hfpCmd;
		#endif

		#ifdef HID_Profile
		HciUARTHIDDataType  hciUARTHIDData;
		#endif

	}MMIMsgBody;

	
	#define RESERVE_BYTE_NO 1
	#define CMD_OFFSET (RESERVE_BYTE_NO+1)
	#define EVT_OFFSET (RESERVE_BYTE_NO+2)
	
	typedef struct
	{
		//refer to 010 GAP Interface.doc for message format
		U8 dataOffset; //to the start of meaningful message
		U8 reserveByte[RESERVE_BYTE_NO];
		U8 msgOpcode;
		U8 eventCode;
		U8 paraLen;
		MMIMsgBody msgBodyPtr;
	}MMIMsgType;
	
	typedef struct mST
	{
		U8 nestState;
		struct mST XDATA_PTR nextStatePtr;
	}MmiStateType;
	
	enum
	{
		MMI_CONN_STATE_NONE,
		MMI_CONN_STATE_WAITING_PASS,
		MMI_CONN_STATE_WAITING_ACTIVE,
		MMI_CONN_STATE_ACL_CONNECTED,
		MMI_CONN_STATE_WAIT_FOR_DETACH,
	};
	
	enum
	{
		ROLE_UNKNOWN,
		ROLE_MASTER,
		ROLE_SLAVE,
	};

	#define SCO_CODEC_CVSD		0x01
	#define SCO_CODEC_MSBC		0x02
	

	typedef struct
	{
		U16 aclConnHandle;   //the 2-byte connection handle
		U16 scoConnHandle;   //the 2-byte connection handle
		
		U8 role:2;
		U8 isAuthenticated:2;
		U8 isEncrypted:2;
		U8 connectionState:2;

		U8 mode; 	//active, sniff, park or hold
		U8 scoType;
		U8 scoPktType;
		U8 txInterval; //transmission interval
		U16 rxPacketLen;
		U8 scoCodecType;
		U8 sniffTryCnt;
		U16 sniffProfileMask;
		U16 sniffInterval;	//unit: slot

		U8 currentSoundLevel;
		U8 smallStep;
		U8 absVolume;
		U8 vgm;
		U8 isMicMute:1;
		U8 isSpkMuteForMicMute:1;
		U8 isSpkMuteForSCO:1;
		U8 isOppProhibited:1;
		U8 QosFlowStatus:1;
		U8 RxFull:1;
		U8 reserved:2;
		
		U8 scoIsActiveEstablished:1;
		U8 isSCOAlive:1;
		U8 isSCODisconnecting:1;
		U8 isEncryptionFailed:1;
		U8 QosServiceType:4;
		U8 isPairFailed:1;
		U8 roleSwitchCnt:7;	
		
		U8 miscMask;

		U8 soundLevelA2DP;
		U8 smallStepLevelA2DP;
		U8 avrcpVolume;
		
		U8 micSoundLevelA2DP;
		U8 classOfDev[3];
		U8 remoteName[MAX_REMOTE_NAME_LEN];
		U8 remoteOobAuthData:1;
		U8 remoteAuthReq:3;
		U8 remoteIoCap:3;
		U8 isIoCapRespSet:1;
		OSMQ OSMQ_Profile_Reconnect;
	}MMILinkSettingType;
	
	#define __SEQUENTIAL_ACTIVE_PROFILE__
	
	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	typedef struct
	{
		U8 count;
		U8 index;
		U8 profileId[1];
	} ConnectProfileSeqCtl;
	#endif

	typedef struct 
	{
		MmiStateType XDATA_PTR nestStatePtr; 
		U8 subState;
		U8 bdAddr[6];
		U8 stateEntryExit;
		U8 isCallRejectedByKey:1;
		U8 reserved:7;
		MMILinkSettingType linkPara;
		#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
		ConnectProfileSeqCtl XDATA_PTR seqCtl;
		#endif
	} MmiInfoType;

	typedef struct
	{
		U8 XDATA_PTR callerIdDataPtr;
	}MmiHfpClipNumberType;

	typedef struct
	{
		OST XDATA_PTR delaySDPTimer;
		OST XDATA_PTR sniffTimerPtr;
		OST XDATA_PTR checkRemoteAGAcceptEnterSniffTimer;	
		OST XDATA_PTR refreshEncKeyTimer;
		OST XDATA_PTR waitDetachTimer;		
		OST XDATA_PTR nfcDetachIfNoProfileTimer;
		OST XDATA_PTR connectionPowerSavingTimer;
		OST XDATA_PTR roleSwitchTimer;
		OST XDATA_PTR delayRemoveConnectedTimer; //asus zenfone 5
		OST XDATA_PTR delayRoleSwitchTimer;
	}MmiConnectionTimerInfo;

	typedef struct
	{
		OST XDATA_PTR autoPowerOnTimer;
		OST XDATA_PTR pairModeTimer;
		OST XDATA_PTR autoSwitchOffTimer;
		OST XDATA_PTR disPowerOffAfterPowerOnTime;
		OST XDATA_PTR checkChargingTimer;
		OST XDATA_PTR delayChgMediaEvtTimer;
		OST XDATA_PTR autoSwitchOffInLowBatTimer;
		OST XDATA_PTR connectableTimeoutTimer;
		OST XDATA_PTR lowBatteryLEDtimer;
		OST XDATA_PTR lowBatteryRingtimer;
		OST XDATA_PTR batteryMonitoringTimer;
		OST XDATA_PTR delayDisableMMI1Timer;
		OST XDATA_PTR detachLinkTimeExpireTimer;
		OST XDATA_PTR btnSequenceTimer;
		OST XDATA_PTR rssiReadIntervalTimer;
		OST XDATA_PTR watchDogReconnectTimer;
		OST XDATA_PTR watchDogRebootNoMediaTimer;
		OST XDATA_PTR userReconnectKeyEventTimer;
		OST XDATA_PTR writeFlashTimer;
		#ifdef SUPPORT_VOICE_COMMAND
		OST XDATA_PTR voiceCommandTriggerTimer;
		#endif
		OST XDATA_PTR waitForOTAPacketTimer;
		OST XDATA_PTR waitATTriggerMediaTimer;
	}MmiSystemTimerInfo;

	typedef struct
	{
		MmiSystemTimerInfo sysTimer;
		MmiConnectionTimerInfo connTimer[MAX_MULTI_POINT_NO];
	}MmiTimerInfo;
	
	#define MMI_MEDIA_EVENTS_SIZE 10
	typedef struct
	{
		U16 eventCode;
	}MmiMediaEventEntity;

	typedef struct
	{
		//The significant bit represents user defined or not.
		MmiMediaEventEntity events[MMI_MEDIA_EVENTS_SIZE];
		U8 nextPushIndex;
		U8 nextPopIndex;
	}MmiMediaTriggerEventCtrl;
	
	typedef struct
	{
		U8 batStatus;
		U8 currentLedDuration;
	}MmiBatInfo;

	enum
	{
		PROCD_RELEASE,
		PROCD_TRANSIT,
		NOTPROCD
	};

	enum
	{
		SET_MUTE_ON,
		SET_MUTE_OFF,
		TOGGLE_MUTE_ON_OFF
	};	

	#ifdef A2DP_Profile
	#define INVALID_AVRCP_OP_CODE 0x00
	#endif

	enum
	{
		DSP_DISABLED = 0,
		DSP_ENABLE_BY_SCO = 2,
		DSP_DISABLE_BY_POWER_OFF,
	};

	#define MMI_BEGINNING_BTN_SEQ_INDEX 0x00
	
	#define MMI_RESET_LINK_INFO	0x01
	#define MMI_RESET_AIR_LINK_INFO	0x02
	#define MMI_RESET_FAST_DONGLE_LINK_INFO	0x04
	
	#define MMI_UART_DEVICE_TYPE_UNKNOWN		0x00
	#define MMI_UART_DEVICE_TYPE_PHONE			0x01
	#define MMI_UART_DEVICE_TYPE_AIROSHARE		0x02
	#define MMI_UART_DEVICE_TYPE_AIROSOUNDBAR	0x03
	#define MMI_UART_DEVICE_TYPE_AIROWOOFER		0x04
	
	
	enum
	{
		MMI_AUDIO_SWITCH_ILDE,
		MMI_AUDIO_SWITCH_WAITING_HCI_EVT,
		MMI_AUDIO_SWITCH_ERROR,
	};
	
	enum
	{
		SDP_USE_BY_MMI,
		SDP_USE_BY_SDK,
		SDP_USER_NUM,
		SDP_USER_EOF = 0xFF,
	};
	
	typedef struct
	{
		U8 btnSeqRecord[MAX_BTN_SEQ_NO];
		U8 resetPairedDeviceMediaCount;
		U8 keyDownState;
		U8 resetLinkType;
		U8 audioSwitchState:4;
		U8 isAsyncSCOOn:1;
		U8 reserved:3;
		
		U8 isButtonLocked:1;
		U8 dbgPrivateKey:1;
		U8 isPowerOn:1;
		U8 isReset:1;
		U8 isWatchdogReboot:1;
		U8 keyInvalidInPwrOnLowBat:1;
		U8 noConnectivityInPwrOnLowBat:1;
		U8 switchingSCO:1;
		
		U16 audioSwitchHandler;
	}MMIDriverRelatedCtlType;

	#define VP_STATUS_LOAD_FROM_ROM		0x01
	#define VP_STATUS_LANG_CONFIRM		0x02
	typedef struct
	{
		U8 vpLangStatus;
		U8 vpSelectIndex;
		U8 vpSelectRound;
		OST XDATA_PTR vpSelectTimerPtr;
	} VoicePromptRuntime;
	
	typedef struct
	{
		U8 bdAddr[6];
		U8 reconnectType;
		U8 retryCnt;
		U8 classOfDev[3];
	}MMIReonnectDataType;

	typedef struct
	{
		U8 isDelayTimerSet;
		OST reConnectDelayTimer;
		U8 isDurationTimerSet;
		OST reconnectDurationTimer;
		U8 currentHighPriorityIndex;
		MMIReonnectDataType reconLinkLossWatchDogData[NVRAM_LINK_KEY_ITEMS];
		U8 currentTargetIndex;
		MMIReonnectDataType reconnectData[NVRAM_LINK_KEY_ITEMS];
	}MMIReconnectListType;

	typedef struct
	{
		MMIReconnectListType XDATA_PTR reconnectListPtr;
		U8 type;
		U8 linkLossBdAddr[6];
		U8 disableReconnect;
		U8 linkLossLoopCnt;
	}MMIReconnectCtlType;
	
	typedef struct
	{
		U8 waitUser:1;
		U8 waitCmd:3;
		U8 reserved:4;
		
		union
		{
			struct
			{
				U16 uuid16;
			}serviceSearchCmd;
			struct
			{
				U32 serviceRecordHandle;
				U16 attributeId1;
				U16 attributeId2;
			}attributeSearchCmd;
			struct
			{
				U16 uuid16;
				U16 attirbuteId;
			}serviceAttributeSearchCmd;
		}cmdRecord;
	}SdpTempCmdType;
	
	enum
	{
		SDP_SERVICE_SEARCH				= 0x01,
		SDP_ATTRIBUTE_SEARCH			= 0x02,
		SDP_SERVICE_ATTRIBUTE_SEARCH	= 0x04,
	};
	
	typedef struct
	{
		MMI_PROFILE_INFO XDATA_PTR sdapClient;
		U8 sdapState;
		U8 searchedProfile;
		U8 supportedProfile;
		U8 hfpScn;
		U8 hspScn;
		#ifdef SPP_Profile
		U8 sppScn;
		#endif
		#ifdef SONGPAL_Profile
		U8 songpalScn;
		U8 songpalProfileId;
		#endif
		#ifdef AIRAPP_Profile
		U8 airappScn;
		#endif
		#ifdef PBAP_Profile
		U8 pbapScn;
		#endif
		U8 remoteApplicationID;
		U8 sdpUserStatus[SDP_USER_NUM];

		SdpTempCmdType SdpTempCmd; 
	}SDAPCtlType;
	
	enum
	{
		SDP_USER_STATUS_NONE,
		SDP_USER_STATUS_UNREGISTER,
		SDP_USER_STATUS_REGISTER,
		SDP_USER_STATUS_WAIT_TO_USE,
		SDP_USER_STATUS_IN_USED,
	};
	
	typedef struct
	{
		U8 currentActiveDev;
		U8 currentActiveLinkIndex;
		U16 registedDev[MAX_MULTI_POINT_NO];
		U16 sysAudioDev;
		OST XDATA_PTR resumeTimer;
	}AUDIODevCtl;
		
	typedef struct
	{
		U8 serviceInd;
		U8 latestConnectedLinkInd;
		//////////// DO NOT add any new variable above this warning!!!!!!!!!!! coze mmi_init would be fail
		U8 audioableSCOLink;
		U8 audioableA2DPLink;
		
		MmiInfoType mmiInfo[MAX_MULTI_POINT_NO];
		U8 activeLinkRecord; 
		U8 scoCnt;
		U8 msgHandled;
		MMIReconnectCtlType reconnectCtl;
		MmiTimerInfo mmiTimerInfo;
		U8 keyState;
		MmiBatInfo mmiBatInfo;
		MmiMediaTriggerEventCtrl mediaTriggerEventCtrl;

		
		U8 inquiry_ongoing:1;
		U8 page_ongoing:1;
		U8 isUpdateNvRam:1;
		U8 enterDiscoverable:1;
		U8 enterDiscoverableAfterDisc:1;
		U8 isMicEnableInA2DP:1;
		U8 isMicAdjsutInA2DP:1;
		U8 isAudioTransparencyEnable:1;
		U8 isVolumeSetInitInA2DP:1;
		U8 FlashUpdateState:2;
		U8 reserved:5;
		U8 linkIndexMicCtlInA2DP;
		U8 inquiryState;
		
		U8 miscMask2;
		U8 miscMask4;
		U8 currentSupportDeviceNo;

		#ifdef SUPPORT_VOICE_PROMPT	
		VoicePromptRuntime vpRuntime;
		#endif
		
		U8 connectableState;
		U8 discoverableState;
		SDAPCtlType sdapCtl[MAX_MULTI_POINT_NO];
		MMIDriverRelatedCtlType driverCtl;
		U8 idleStateSoundLevel;
		U8 idleStateSmallStep;
		U8 pwrSavingMask;
		U8 linkPwrSavingMask[MAX_MULTI_POINT_NO];
		
		AUDIODevCtl audioDevCtl;
		#ifdef TRSPX_App
		U32 passkey[MAX_MULTI_POINT_NO];
		U8 connectedMode;
		#endif
		OSMQ OSMQ_MMI_Internal;
	}MmiCtlType;
	
	#define OSMQ_MMI_Internal_ptr		&gMMI_ctl.OSMQ_MMI_Internal

	PUBLIC void MMI_SystemOff(void);
	PUBLIC void MMI_PowerOffProce(void);
	PUBLIC void MMI_PowerOffHandler(U8 reason);
	PUBLIC void MMI_ClearActionOfPowerOnInLowBat(void);
	PUBLIC void MMI_PowerOnLoader(void);
	PUBLIC void MMI_SendInternalCmdToMMI(U8 opcode);

	PUBLIC void MMI_ReleaseMultiLink(void);	
	PUBLIC U8 MMI_DisconnectSpecificProfile(U8 linkIndex, U8 profileId);
	PUBLIC void MMI_ReleaseProfileLinkAndDetach(U8 linkIndex);
	PUBLIC U8 MMI_GetConnectedProfileCount(U8 linkIndex, BOOL connectedEvtOnly);
	PUBLIC BOOL MMI_IsProfileConnected(U8 linkIndex, U8 profileId);
	PUBLIC BOOL MMI_IsProfileIdLoaded(U8 linkIndex, U8 profileId);
	PUBLIC BOOL MMI_IsAllProfileDisconnected(U8 linkIndex);
	PUBLIC void MMI_ClearProfileInReconnectList(U8 linkIndex, U8 profileId);
	PUBLIC void MMI_ProfileHandle(void);
	PUBLIC void MMI_SetConnected(U8 linkIndex);
	PUBLIC void MMI_ClearProfileReconnectListQueue(U8 linkIndex);
	PUBLIC BOOL MMI_IsProfileInReconnectList(U8 linkIndex, U8 profileId);
	
	PUBLIC void MMI_ClearMMILinkSettingType(U8 linkIndex);
	
	
	PUBLIC void MMI_PushMediaEvent(U16 event);
	PUBLIC MMIMsgType XDATA_PTR MMI_GetCommandMessageMem (U8 command_op);
	PUBLIC MMIMsgType XDATA_PTR MMI_GetEventMessageMem (U8 event_op);

	PUBLIC void MMI_Reset(void);
	PUBLIC BOOL MMI_CheckBootFlag(U8 flag);
	PUBLIC BOOL MMI_IsActiveLink(U8 linkIndex);


	//=================Function defined in MMI_Chg.c=====================
	PUBLIC void MMI_PollingBatStatus(void);
	PUBLIC U8 MMI_IsChargerIn(void);
	
	PUBLIC void MMI_SwitchEndianBDAddr(U8 XDATA_PTR bdAddr);
	PUBLIC BOOL MMI_IsSniffAllowed(U8 linkIndex);
	PUBLIC void MMI_PowerSaving(BOOL isEnable, U8 pwrSavingMask, U8 linkIndex);
	PUBLIC BOOL MMI_IsLinkTrafficBusy(U8 linkIndex);
	PUBLIC U8 MMI_GetNextActiveLink(U8 currActInd);
	PUBLIC U8 MMI_GetNextNormalLink(U8 linkIndex);
	PUBLIC BOOL MMI_IsNormalLink(U8 linkIndex);
	
	PUBLIC void MMI_SetConnectionState(U8 linkIndex, U8 connectionState);
	PUBLIC U8 MMI_GetConnectionState(U8 linkIndex);
#endif
