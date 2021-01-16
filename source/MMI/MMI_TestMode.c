#include "reside_flash.inc"

#define _MMI_TEST_MODE_C_

#include "rc.h"
#include "bt_config_profile.h"

#include "MMI.h"
#include "MMI_TestMode.h"
#include "MMI_State.h"
#include "MMI_HCI.h"
#include "..\HC\HC_Interface.h"
#include "..\LC\LC_Interface.h"
#ifdef MMI_LE_ENABLE
#include "..\MMI_LE\MMI_LE_interface.h"
#endif

PRIVATE void MMI_TestModeEntry(U8 linkIndex);
PRIVATE void MMI_TestModeExit(U8 linkIndex);
PRIVATE void MMI_TestMode_StartLabTest(void);
PRIVATE void MMI_TxSingleTone(U8 channel, U16 power);
PRIVATE void MMI_TxContinuePacket(U8 channel, U16 power);
PRIVATE void MMI_TxBurstPacket(U8 channel, U16 power);
PRIVATE void MMI_RxTest(U8 channel, U16 power);
PRIVATE U16 MMI_LabTest_TransferPowerToGC1GC2 (U16 power);
PRIVATE void MMI_TestMode_LedHandler(U8 type, U8 repeat);
PRIVATE void MMI_TestMode_Execute(void);
PRIVATE void MMI_TestMode_KeyEnterTestMode(U8 linkIndex);
PRIVATE void MMI_TestMode_KeyTestModeChannel(U8 linkIndex);
PRIVATE void MMI_TestMode_KeyTestModePower(U8 linkIndex);
PRIVATE void MMI_TestMode_KeyTestModeState(U8 linkIndex);
PRIVATE void MMI_TestMode_KeyTestModeTxPktType(U8 linkIndex);

U8 CODE TestModeChannelMap[] = {0, 39, 78};
U8 CODE TestModePowerDiffMap[] = {0, 1, 2, 3, 4, 7, 10, 13, 16, 19}; // {X,X-1,X-2,X-3,X-4,X-7,X-10,X-13,X-16,X-19}

static MMI_KEY_EVT CODE MMI_TestMode_KeyEvtHandler[TESTMODE_KEY_EVENT_NO] = 
{
	{MMI_TestMode_KeyEnterTestMode},	//TESTMODE_KEY_EVENT_KEY_ENTER_TESTMODE
	{MMI_TestMode_KeyTestModeChannel},	//TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_CHANNEL,  
	{MMI_TestMode_KeyTestModePower},	//TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_POWER,	  	   				
	{MMI_TestMode_KeyTestModeState},	//TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_STATE,				  
	{MMI_TestMode_KeyTestModeTxPktType},//TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_TX_PACKET_TYPE,
};

MmiTestModeInfo XDATA gMMI_TestMode_ctl;

#ifdef Ab1500
PRIVATE MmiTestModeHandlerSetType XDATA MMI_TestModeHandlerSet[TOTAL_TESTMODE_STATE_NO] =
#else
PRIVATE MmiTestModeHandlerSetType CODE MMI_TestModeHandlerSet[TOTAL_TESTMODE_STATE_NO] =
#endif
{
	MMI_TxSingleTone,
	MMI_TxContinuePacket,
	MMI_TxBurstPacket,
	MMI_TxBurstPacket,
	MMI_RxTest,
};

extern MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO];

PRIVATE MmiHandlerSetType CODE	MMI_StateTestModeHandler = 
{
	MMI_TestModeEntry, NULL, MMI_TestModeExit, //MMI_TEST_MODE
};

PUBLIC void MMI_TestMode_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_TestMode_KeyEvtHandler[index].fKeyEvt)
	{
		MMI_TestMode_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}

PUBLIC void MMI_TestMode_Init(void)
{
	MMI_StateHandlerSet[MMI_TEST_MODE] = &MMI_StateTestModeHandler;
}

PRIVATE void MMI_TestModeEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);
	
	MMI_SECTOR_SetUpdateFlag(FALSE);
	
	#ifdef MMI_LE_ENABLE
	MMI_LE_DisableAdv();
	#endif

	MMI_PowerSaving(FALSE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);

	//New Event - "MEDIA_EVT_ENTER_TESTMODE_STATE"
	MMI_PushMediaEvent(MEDIA_EVT_ENTER_TESTMODE_STATE);


	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[0] = 0x7E;
	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[1] = 0x96;
	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[2] = 0xC6;
	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[3] = 0x6B;
	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[4] = 0x00;
	((U8 XDATA_PTR)&gHC_ctl.BdAddr)[5] = 0x00;

	gMMI_TestMode_ctl.state = TOTAL_TESTMODE_STATE_NO - 1;
	MMI_TestMode_StartLabTest();
	
}

PRIVATE void MMI_TestModeExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_PowerSaving(TRUE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
}

PRIVATE void MMI_TestMode_StartLabTest(void)
{
	MMI_HCI_SendVCmdLabTestSwitchMCUTo12M();
	
	MMI_HCI_SendVCmdLabTestStart();
    OS_delay_ms(1000);


	//RX
	OS_ENTER_CRITICAL();
	//324B2A
	SYS_3WireWrite(0x13,gMMI_nvram.mmiFCCInfo.Reg32B1,gMMI_nvram.mmiFCCInfo.Reg32B2);
    OS_delay_us(1000);
	//350580
	SYS_3WireWrite(0x15,gMMI_nvram.mmiFCCInfo.Reg35B1,gMMI_nvram.mmiFCCInfo.Reg35B2);
    OS_delay_us(1000);
    OS_EXIT_CRITICAL();
	

	MMI_HCI_SendVCmdLabTestUpdateUapLap((U8 XDATA_PTR)&gHC_ctl.BdAddr);
	MMI_HCI_SendVCmdLabTestLTAddrInTxPkt();
	MMI_HCI_SendVCmdLabTestReportRateSetup();
}

PRIVATE void MMI_TxSingleTone(U8 channel, U16 power)
{
	MMI_HCI_SendVCmdLabTestPause();	
	MMI_HCI_SendVCmdLabTestSingleToneSetup(	channel, 
											MMI_LabTest_TransferPowerToGC1GC2(power), 
											((U16)gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1<<8) | (U16)gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2);

}

PRIVATE void MMI_TxContinuePacket(U8 channel, U16 power)
{
	HCI_VCMD_PARA_LABTEST_CTX_DATA LDATA cTxData; 

	MMI_HCI_SendVCmdLabTestPause();
	
	MMI_HCI_SendVCmdLabTestTxModemManualOff();
	
	power = MMI_LabTest_TransferPowerToGC1GC2 (power);

	cTxData.Channel = channel;
    cTxData.Gc2 = (U8)power;
    cTxData.Gc1 = (U8)(power >> 8);
    cTxData.Bias2_2 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2;
    cTxData.Bias2_1 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1;
    cTxData.PatternType = 0x60; //SFR 228:PN sequence
    cTxData.ModType = 0x04; //SFR 231: GFSK
	MMI_HCI_SendVCmdLabTestContinueTxData((HCI_VCMD_PARA_LABTEST_CTX_DATA XDATA_PTR)&cTxData);
}

PRIVATE void MMI_TxBurstPacket(U8 channel, U16 power)
{
	HCI_VCMD_PARA_LABTEST_BTX_PACKET LDATA bTxPkt;
	OSMEM_memset_xdata((U8 XDATA_PTR)&bTxPkt, 0, sizeof(bTxPkt));
	MMI_HCI_SendVCmdLabTestPause();
	power = MMI_LabTest_TransferPowerToGC1GC2 (power);
	bTxPkt.Channel = channel;
    bTxPkt.Gc2 = (U8)power;
    bTxPkt.Gc1 = (U8)(power >> 8);
    bTxPkt.Bias2_2 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2;
    bTxPkt.Bias2_1 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1;
	switch(gMMI_TestMode_ctl.packetTypeIndex[MMI_BTX])
	{
		case MMI_2DH5:
			bTxPkt.PacketType = BT_PKT_2DH5;
			bTxPkt.PacketSizeB0 = 0xA7;
			bTxPkt.PacketSizeB1 = 0x02;
			bTxPkt.DataType = 0x30;
			bTxPkt.EDREnb = 0x01;
			break;
		case MMI_3DH5:
			bTxPkt.PacketType = BT_PKT_3DH5;
			bTxPkt.PacketSizeB0 = 0xFD;
			bTxPkt.PacketSizeB1 = 0x03;
			bTxPkt.DataType = 0x30;
			bTxPkt.EDREnb = 0x01;
			break;
		default:
			bTxPkt.PacketType = BT_PKT_DH5;
			bTxPkt.PacketSizeB0 = 0x53;
			bTxPkt.PacketSizeB1 = 0x01;
			bTxPkt.DataType = 0x30;
			bTxPkt.EDREnb = 0x00;
			break;
	}
    bTxPkt.HopEnb =  (gMMI_TestMode_ctl.state == MMI_HBTX)?0x01:0x00;
	
    bTxPkt.ESCOEnb = 0x00;
    //bTxPkt.TempCompInvlB0;
    //bTxPkt.TempCompInvlB1;
    bTxPkt.BdrMaxGain = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_gfsk_max_gain;
    bTxPkt.BdrMinGain = gLC_ctl.rf_ctl.tx_gain_ctl.tx_gfsk_min_gain;
    bTxPkt.EdrMaxGain = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_edr_max_gain;
    bTxPkt.EdrMinGain = gLC_ctl.rf_ctl.tx_gain_ctl.tx_edr_min_gain;
    bTxPkt.BleMaxGain = gLC_ctl.rf_ctl.tx_gain_ctl.tx_ble_max_gain;
    bTxPkt.BleMinGain = gLC_ctl.rf_ctl.tx_gain_ctl.tx_ble_min_gain;
	MMI_HCI_SendVCmdLabTestBurstTxPacket((HCI_VCMD_PARA_LABTEST_BTX_PACKET XDATA_PTR)&bTxPkt);
}

PRIVATE void MMI_RxTest(U8 channel, U16 power)
{
	UNUSED(power);
	// issue 4917
	SYS_3WireWrite(0x13, 0xC9, 0x4A);
	SYS_3WireWrite(0x15, 0x00, 0xF8);
	MMI_HCI_SendVCmdLabTestPause();
	MMI_HCI_SendVCmdLabTestTxModemManualOff();
	MMI_HCI_SendVCmdLabTestContinueRxStart(channel, 0, 0);
}

PRIVATE U16 MMI_LabTest_TransferPowerToGC1GC2 (U16 power)
{
	U16 temp;

	temp = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc1 + ((U16)gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc2 << 8);
	temp &= ~((U16)0x003F << 7);
	temp |= (power << 7);
	return (U16)((temp<<8)|(temp>>8));
}

#define LED_BRAMPFLASH	0
#define LED_BSFLASH		1
#define LED_BFFLASH		2
#define LED_RRAMPFLASH	3

#include "drv_led.h"

//Program Size: data=122.3 xdata=22163 const=5247 code=246160
PRIVATE void MMI_TestMode_LedHandler(U8 type, U8 repeat)
{
	LedPatternType XDATA_PTR LedCtrlPtr;
	
	LedCtrlPtr = SYS_MemoryGet(sizeof(LedPatternType));
	
	if(!LedCtrlPtr)
		ASSERT(FALSE);
	
	OSMEM_memset_xdata((U8 XDATA_PTR)LedCtrlPtr, 0, sizeof(LedPatternType));
	switch(type)
	{
		case LED_BRAMPFLASH:
			//BLUE
			LedCtrlPtr->ledSettings[1].onOff  = LED_ON;
			LedCtrlPtr->ledSettings[1].ledFollowMultiplierControl = 0x01;
			LedCtrlPtr->ledSettings[1].pwmSetting = 0xF0 | repeat;
			LedCtrlPtr->ledSettings[1].t1 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t2 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t3 = 15;		//0.3s, unit:20ms
			LedCtrlPtr->ledSettings[1].rampOnStep = 0x20;	//64ms, unit:2ms
			LedCtrlPtr->ledSettings[1].rampOffStep = 0x20;	//64ms, unit:2ms
			break;

		case LED_BSFLASH:
			//BLUE
			LedCtrlPtr->ledSettings[1].onOff  = LED_ON;
			LedCtrlPtr->ledSettings[1].ledFollowMultiplierControl = 0x01;
			LedCtrlPtr->ledSettings[1].pwmSetting = 0xF0 |repeat ;
			LedCtrlPtr->ledSettings[1].t1 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t2 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t3 = 15;		//0.3s, unit:20ms
			LedCtrlPtr->ledSettings[1].rampOnStep = 0;	//0ms, unit:2ms
			LedCtrlPtr->ledSettings[1].rampOffStep = 0;	//0ms, unit:2ms
			break;

		case LED_BFFLASH:
			//BLUE
			LedCtrlPtr->ledSettings[1].onOff  = LED_ON;
			LedCtrlPtr->ledSettings[1].ledFollowMultiplierControl = 0x01;
			LedCtrlPtr->ledSettings[1].pwmSetting = 0xF0 | repeat;
			LedCtrlPtr->ledSettings[1].t1 = 5;	//0.1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t2 = 5;	//0.1s, unit:20ms
			LedCtrlPtr->ledSettings[1].t3 = 15;		//0.3s, unit:20ms
			LedCtrlPtr->ledSettings[1].rampOnStep = 0;	//0ms, unit:2ms
			LedCtrlPtr->ledSettings[1].rampOffStep = 0;	//0ms, unit:2ms
			break;

		case LED_RRAMPFLASH:
			//RED
			LedCtrlPtr->ledSettings[0].onOff  = LED_ON;
			LedCtrlPtr->ledSettings[0].ledFollowMultiplierControl = 0x01;
			LedCtrlPtr->ledSettings[0].pwmSetting = 0xF0 | repeat;
			LedCtrlPtr->ledSettings[0].t1 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[0].t2 = 50;	//1s, unit:20ms
			LedCtrlPtr->ledSettings[0].t3 = 15;		//0.3s, unit:20ms
			LedCtrlPtr->ledSettings[0].rampOnStep = 0x20;	//64ms, unit:2ms
			LedCtrlPtr->ledSettings[0].rampOffStep = 0x20;	//64ms, unit:2ms
			break;
	}
	
	LED_SetPatternToHW(LedCtrlPtr);
	
	OSMEM_Put((U8 XDATA_PTR)LedCtrlPtr);
}


static 	U8 CODE TestModeLed[TOTAL_TESTMODE_STATE_NO] = {LED_BRAMPFLASH, LED_BSFLASH, LED_BFFLASH, LED_BFFLASH, LED_RRAMPFLASH};
PRIVATE void MMI_TestMode_Execute(void)
{
	MMI_TestModeHandlerSet[gMMI_TestMode_ctl.state].TestModeHandler(TestModeChannelMap[gMMI_TestMode_ctl.channelIndex[gMMI_TestMode_ctl.state]],
					gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_gfsk_max_gain - TestModePowerDiffMap[gMMI_TestMode_ctl.powerIndex[gMMI_TestMode_ctl.state]]);
	MMI_TestMode_LedHandler(TestModeLed[gMMI_TestMode_ctl.state], gMMI_TestMode_ctl.channelIndex[gMMI_TestMode_ctl.state]);
}

PRIVATE void MMI_TestMode_KeyEnterTestMode(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_AddTopState(MMI_EOF, MMI_TEST_MODE);
	MMI_SetEnterDiscoverable(FALSE);
	if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
	{
		MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
	}
}

PRIVATE void MMI_TestMode_KeyTestModeChannel(U8 linkIndex)
{
	UNUSED(linkIndex);
	gMMI_TestMode_ctl.channelIndex[gMMI_TestMode_ctl.state] =
		(gMMI_TestMode_ctl.channelIndex[gMMI_TestMode_ctl.state]+1)% sizeof(TestModeChannelMap);
	MMI_TestMode_Execute();
}

PRIVATE void MMI_TestMode_KeyTestModePower(U8 linkIndex)
{
	UNUSED(linkIndex);
	gMMI_TestMode_ctl.powerIndex[gMMI_TestMode_ctl.state] =
		(gMMI_TestMode_ctl.powerIndex[gMMI_TestMode_ctl.state]+1)% sizeof(TestModePowerDiffMap);
	MMI_TestMode_Execute();
}
PRIVATE void MMI_TestMode_KeyTestModeState(U8 linkIndex)
{
	UNUSED(linkIndex);
	//Clear Info
	gMMI_TestMode_ctl.channelIndex[gMMI_TestMode_ctl.state] = 0;
	gMMI_TestMode_ctl.powerIndex[gMMI_TestMode_ctl.state] = 0;

	gMMI_TestMode_ctl.state =
		(gMMI_TestMode_ctl.state+1)%TOTAL_TESTMODE_STATE_NO;
	MMI_TestMode_Execute();
}
PRIVATE void MMI_TestMode_KeyTestModeTxPktType(U8 linkIndex)
{
	UNUSED(linkIndex);
	gMMI_TestMode_ctl.packetTypeIndex[gMMI_TestMode_ctl.state] =
		(gMMI_TestMode_ctl.packetTypeIndex[gMMI_TestMode_ctl.state]+1)%TOTAL_TESTMODE_PACKET_TYPE_NO;
	MMI_TestMode_Execute();
}