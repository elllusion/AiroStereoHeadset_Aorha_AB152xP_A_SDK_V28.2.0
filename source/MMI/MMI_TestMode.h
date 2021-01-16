#ifndef _MMI_TEST_MODE_H_
	#define _MMI_TEST_MODE_H_
	
	enum
	{
		MMI_STX,
		MMI_CTX,
		MMI_BTX,
		MMI_HBTX,	//burst TX with hopping
		MMI_RX,
		TOTAL_TESTMODE_STATE_NO,
	};

	enum
	{
		MMI_DH5,
		MMI_2DH5,
		MMI_3DH5,
		TOTAL_TESTMODE_PACKET_TYPE_NO
	};
	
	typedef struct
	{
		U8 state;
		U8 channelIndex[TOTAL_TESTMODE_STATE_NO];
		U8 powerIndex[TOTAL_TESTMODE_STATE_NO];
		U8 packetTypeIndex[TOTAL_TESTMODE_STATE_NO];
	}MmiTestModeInfo;
	
	typedef struct
	{
		void (*TestModeHandler)(U8 channel, U16 power);
	}MmiTestModeHandlerSetType;
	
	PUBLIC void MMI_TestMode_Init(void);
	PUBLIC void MMI_TestMode_ProcessKeyEvent(U8 linkIndex, U8 index);
	
#endif //_MMI_TEST_MODE_H_