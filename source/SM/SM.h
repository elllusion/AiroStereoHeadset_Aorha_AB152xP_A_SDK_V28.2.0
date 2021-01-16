/***************************************************************
*
* Implement SM (Security Manager)
* Bluetooth 4.0 - Low Energy
*
***************************************************************/

#ifndef __SM_H__
#define __SM_H__

#include "sm_hal.h"

#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef SM_SUPPORTED
#ifdef _SM_C_
#define SM_EXTERN
#else
#define SM_EXTERN extern
#endif

//#define SM_UNIT_TEST
//#define SM_REPEATED_ATTEMPTS_ENABLE
//#define SM_SIGNATURE_ENABLE
#define SM_RANDOM_ADDRESS_ENABLE

#define SM_SMP_TIMER                30//1200

#define SM_HCI_CMD_ENCRYPT_LEN              32
#define SM_HCI_CMD_LTK_REQ_REPLY_LEN        18
#define SM_HCI_CMD_LTK_NEGATIVE_REPLY_LEN   2
#define SM_HCI_RAND_NUMBER_LEN              8

#define SM_PAIRING_FAIL_DATA_LEN    1

#define SM_FEATURE_INITKEYDISTR_IDX 5
#define SM_FEATURE_RESPKEYDISTR_IDX 6

#define SM_ENC_KEY_DISTRIBUTION     0x01
#define SM_ID_KEY_DISTRIBUTION      0x02
#define SM_SIGN_KEY_DISTRIBUTION    0x04

#define SM_INI_LTK		1
#define SM_INI_IRK		2
#define SM_INI_CSRK		4
#define SM_RESP_LTK		8
#define SM_RESP_IRK		16
#define SM_RESP_CSRK	32

#define SM_EDIV_LEN         2
#define SM_RAND_LEN         8
#define SM_SIGN_COUNTER_LEN 4
#define SM_SIGN_MAC_LEN     8
#define SM_SIGNATURE_LEN    12

#define SM_STATIC_RANDOM_ADDR_MSB           0x11
#define SM_PRIVATE_NONRESOLVABLE_ADDR_MSB   0x00
#define SM_PRIVATE_RESOLVABLE_ADDR_MSB		0x01
#define SM_ADDR_PRAND_LEN   3
#define SM_ADDR_HASH_LEN    3
#define SM_ADDR_LAST_BYTE   5

#ifdef SM_REPEATED_ATTEMPTS_ENABLE
#define SM_REPEAT_ATTEMPT_MAX_RETRY_COUNT   3
#define SM_REPEAT_ATTEMPT_TIME_INTERVAL     (320L)
#endif

#define ENCRYPTION_ENABLED		0x01

/* SM_COMMAND_CODE */
#define SM_CMD_PAIRING_REQUEST                  0x01
#define SM_CMD_PAIRING_RESPONSE                 0x02
#define SM_CMD_PAIRING_CONFIRM                  0x03
#define SM_CMD_PAIRING_RANDOM                   0x04
#define SM_CMD_PAIRING_FAILED                   0x05
#define SM_CMD_ENCRYPTION_INFORMATION           0x06
#define SM_CMD_MASTER_IDENTIFICATION            0x07
#define SM_CMD_IDENTITY_INFORMATION             0x08
#define SM_CMD_IDENTITY_ADDRESS_INFORMATION     0x09
#define SM_CMD_SIGNING_INFORMATION              0x0A
#define SM_CMD_SECURITY_REQUEST                 0x0B

/* SM_COMMAND_CODE Length*/
#define SM_CMD_FEATURE_EXCHANGE_LEN     7
#define SM_CMD_PAIRING_FAIL_LEN         2
#define SM_CMD_SECURITY_REQUEST_LEN     2
#define SM_CMD_EXCHANGE_VALUE_LEN       17
#define SM_CMD_ENC_INFO_LEN             17
#define SM_CMD_MASTER_ID_LEN            11
#define SM_CMD_ID_INFO_LEN              17
#define SM_CMD_ID_ADDR_INFO_LEN         8
#define SM_CMD_SIGNING_INFO_LEN         17

/* SM_STK_GENERATION_METHOD */
#define SM_JUST_WORKS               0
#define SM_PASSKEY_ENTRY_DISPLAY    1
#define SM_PASSKEY_ENTRY_INPUT      2
#define SM_OUT_OF_BAND              3

/* SM_PAIRING_FAIL_REASON */
#define SM_FAIL_RESERVED                        0x00
#define SM_FAIL_PASSKEY_ENTRY                   0x01
#define SM_FAIL_OOB_NOT_AVAILABLE               0x02
#define SM_FAIL_AUTHENTHICATION_REQUIREMENTS    0x03
#define SM_FAIL_CONFIRM_VALUE                   0x04
#define SM_FAIL_PAIRING_NOT_SUPPORTED           0x05
#define SM_FAIL_ENCRYPTION_KEY_SIZE             0x06
#define SM_FAIL_COMMAND_NOT_SUPPORTED           0x07
#define SM_FAIL_UNSPECIFIED_REASON              0x08
#define SM_FAIL_REPEATED_ATTEMPTS               0x09
#define SM_FAIL_INVALID_PARAMETERS              0x0A

/* SM_PAIRING_STATE */
#define SM_STATE_IDLE									0x00
#define SM_STATE_PHASE1_FEATURE_EXCHANGE				0x01
#define SM_STATE_PHASE2_GEN_SRAND						0x02
#define SM_STATE_PHASE2_GEN_SCONFIRM					0x03
#define SM_STATE_PHASE2_WAIT_MCONFIRM					0x04
#define SM_STATE_PHASE2_SEND_SCONFIRM					0x05
#define SM_STATE_PHASE2_WAIT_MRAND						0x06
#define SM_STATE_PHASE2_SEND_SRAND						0x07
#define SM_STATE_PHASE2_WAIT_STK_REQ_EVENT				0x08
#define SM_STATE_PHASE2_WAIT_HCI_ENCRY_CHANGE_EVENT		0x09
#define SM_STATE_PHASE3_SEND_ENC_INFO					0x0A
#define SM_STATE_PHASE3_SEND_MASTER_ID					0x0B
#define SM_STATE_PHASE3_SEND_ID_INFO					0x0C
#define SM_STATE_PHASE3_SEND_ID_ADDR_INFO				0x0D
#define SM_STATE_PHASE3_SEND_SIGNING_INFO				0x0E
#define SM_STATE_PHASE3_WAIT_ENC_INFO					0x0F
#define SM_STATE_PHASE3_WAIT_MASTER_ID					0x10
#define SM_STATE_PHASE3_WAIT_ID_INFO					0x11
#define SM_STATE_PHASE3_WAIT_ID_ADD_INFO				0x12
#define SM_STATE_PHASE3_WAIT_SIGNING_INFO				0x13
#define SM_STATE_LONG_TERM_KEY_REPLY					0x14

/* SM_SUBSTATE */
#define SM_SUBSTATE_NONE				0x00
#define SM_SUBSTATE_SEND_SMP_CMD		0x01
#define SM_SUBSTATE_WAIT_SMP_EVT		0x02
#define SM_SUBSTATE_SEND_HCI_CMD		0x03
#define SM_SUBSTATE_WAIT_HCI_EVT		0x04
#define SM_SUBSTATE_SEND_HCI_CMD_1		0x05
#define SM_SUBSTATE_WAIT_HCI_EVT_1      0x06
#define SM_SUBSTATE_SEND_HCI_CMD_2		0x07
#define SM_SUBSTATE_WAIT_HCI_EVT_2      0x08
#define SM_SUBSTATE_SEND_HCI_CMD_3		0x09
#define SM_SUBSTATE_WAIT_HCI_EVT_3      0x0A
#define SM_SUBSTATE_SEND_MMI_EVT		0x0B
#define SM_SUBSTATE_WAIT_MMI_CMD		0x0C
#define SM_SUBSTATE_SEND_EVT            0x0D

/* SM_LINK_STATUS */
#define SM_LINK_STATUS_NONE             0
#define SM_LINK_STATUS_CONN_CMP         1
#define SM_LINK_STATUS_ENCRYPT          2

#define SM_SMP_FLAG_DATA_EXIST  0x01
#define SM_SMP_FLAG_AUTH        0x02

//////////////////////////////////////////////////////temp
/* L2CP EVENT */
typedef union
{
    struct
    {
        U8 cmdCode;
        U8 SMdata[23];
    }group;
    U8 packet[24];
}SM_Packet;

/* L2CP CMD */
typedef struct
{
  	SM_L2CAP_CMD_HAL_HDR_STRU halHdr;
  	L2CAP_LE_DATA_HDR_STRU L2CAP_Hdr;
	SM_Packet SMP;
}PACKED_STRUCT SM_L2CAP_CMD;

/* HCI CMD */
typedef struct
{
    U16 conHandle;
	U8 LTK[SM_MAX_ENCRYPTION_KEY_SIZE];
}SM_HCI_CMD_LTK_REPLY;

typedef struct
{
	U8 key[SM_MAX_ENCRYPTION_KEY_SIZE];
	U8 plaintextData[SM_MAX_ENCRYPTION_KEY_SIZE];
}SM_HCI_CMD_ENC;

typedef union
{
	U8 cmdData[1];
    SM_HCI_CMD_LTK_REPLY ltkReply;
    SM_HCI_CMD_ENC encCmdPara;
}SM_HCI_CMD_Packet;

typedef union
{
	struct
	{
		U8 reduceOpcode;
		U8 ogf;
	}field;
	U16 cmdCode;
} SM_HCI_OPCODE_STRU;

typedef struct
{
	SM_HCI_HAL_HDR_STRU halHdr;
    SM_HCI_OPCODE_STRU opCode;
    U8 cmdLen;
    SM_HCI_CMD_Packet hciCmd;
}PACKED_STRUCT SM_HCI_CMD;

/* HCI EVENT */
typedef union
{
    U8 encryptedData[SM_MAX_ENCRYPTION_KEY_SIZE];
	U8 randNum[SM_HCI_RAND_NUMBER_LEN];
}SM_HCI_EVT_CMD_COMPLETE_PARA;

typedef struct
{
    U8 SubEvtCode;
    U16 ConnHandle;
    U8 Rand[8];
    U8 EDIV[2];
}PACKED_STRUCT HciLTKRequest;

//////////////////////////////////////////////////////temp

typedef union
{
    struct
    {
    	U8 bondingFlag:2;
    	U8 MITM :1;
    	U8 reserved :5;
    }group;
    U8 value;
}SM_AUTH_REQ_FLAG;

typedef struct
{
	U8 ioCapability;    //SM_IO_CAPABILITY
	U8 oobData;
	SM_AUTH_REQ_FLAG authReq;
	U8 maxKeySize;
	U8 initKeyDistr;
	U8 respKeyDistr;
}SM_PAIRING_FEATURE;

#ifdef SM_SIGNATURE_ENABLE
typedef struct
{
    U8 K1[SM_MAX_ENCRYPTION_KEY_SIZE];
    U8 K2[SM_MAX_ENCRYPTION_KEY_SIZE];
}SM_SIGN_SUBKEY;

typedef struct
{
    U8 XDATA_PTR msg;
    SM_SIGN_SUBKEY XDATA_PTR subKey;
    U8 msgLen;
    U8 currentBlock;
    U8 subState;
    U8 linkIdx: 4;
	U8 lastBlockComplete :1;
	U8 isVerifySign :1;
	U8 result :1;
	U8 isFromATT :1;
}SM_SIGN_TYPE;
#endif

#ifdef SM_REPEATED_ATTEMPTS_ENABLE
typedef struct
{
	OST XDATA_PTR timer;
	U8 addr[BD_ADDR_LEN];
    U8 retry_num;
}SM_REPEATED_ATTEMPT_TYPE;
#endif

typedef struct
{
    OST XDATA_PTR timer;
    U8 TKValue[SM_MAX_ENCRYPTION_KEY_SIZE];
    U8 STK[SM_MAX_ENCRYPTION_KEY_SIZE];

    U8 mRand[SM_MAX_ENCRYPTION_KEY_SIZE];
    U8 sRand[SM_MAX_ENCRYPTION_KEY_SIZE];

    U8 mConfirm[SM_MAX_ENCRYPTION_KEY_SIZE];
    U8 sConfirm[SM_MAX_ENCRYPTION_KEY_SIZE];

    U8 pairReqCmd[SM_CMD_FEATURE_EXCHANGE_LEN];
    U8 encKeySize;
    U8 pairRspCmd[SM_CMD_FEATURE_EXCHANGE_LEN];
    U8 STKMethod;           /* SM_STK_GENERATION_METHOD */
    U8 state;               /* SM_PAIRING_STATE */
    U8 subState;            /* SM_SUBSTATE */
}SM_PAIRING_DATA;

typedef struct
{
    U8 encKeySize;
    U8 flashIdx;
	U8 linkStatus:2;        /* SM_LINK_STATUS */
	U8 isBonded:1;
    U8 EncWithMITM:1;
    U8 reservedBit:4;
}SM_PAIRING_INFO;

typedef struct
{
    U8 addr[BD_ADDR_LEN];
    U8 verifyflashIdx;
    U8 linkIdx;
    U8 subState;
	U8 isVerifyAddr :1;
    U8 addrType:2;
	U8 result :1;
    U8 isFromMMI:1;
    U8 reservedBit:3;
}SM_RAND_ADDR;

typedef struct 
{
    SM_PAIRING_DATA XDATA_PTR SMPData[MAX_NUM_OF_LINK];
    NVRAM_MMI_LINK_RECORD_TYPE XDATA_PTR linkRecord[MAX_NUM_OF_LINK];
#ifdef SM_SIGNATURE_ENABLE
    SM_SIGN_TYPE signData;
#endif
    SM_PAIRING_INFO SMPInfo[MAX_NUM_OF_LINK];
    U8 updateFlashLater;
}SM_CTL_TYPE;

typedef struct
{
#ifdef SM_RANDOM_ADDRESS_ENABLE
    SM_RAND_ADDR XDATA_PTR randAddr;
#endif
    U8 verifyCount;
    U8 randCmdLinkIdx;
    U8 encCmdLinkIdx;
#ifdef SM_REPEATED_ATTEMPTS_ENABLE
    SM_REPEATED_ATTEMPT_TYPE repeatAttempt;
#endif
}SM_INFO_TYPE;

SM_EXTERN SM_CTL_TYPE XDATA gSM_ctl;
SM_EXTERN SM_INFO_TYPE XDATA gSM_info_ctl;

#ifdef SM_UNIT_TEST
SM_EXTERN SM_NVRAM_TYPE XDATA gSM_nvram_init;
#endif

PUBLIC SM_L2CAP_CMD XDATA_PTR SM_SMPCmdFactory(U8 idx,U8 cmdCode, U8 cmdLen);
PUBLIC SM_HCI_CMD XDATA_PTR SM_HciCmdFactory(U16 cmdCode, U8 cmdLen);

//#define SM_UNIT_TEST
#ifdef SM_UNIT_TEST
//extern PUBLIC void HC_CommandDispatch (void);
//extern PUBLIC void HC_EventHandler (void);
#define SM_LTK_LEN          16
#define SM_IRK_LEN          16
#define SM_CSRK_LEN         16

#define SM_CHECK_RES_ENC_KEY    1
#define SM_CHECK_RES_ID_KEY     2
#define SM_CHECK_RES_SIGN_KEY   3
#define SM_CHECK_INIT_ENC_KEY   4
#define SM_CHECK_INIT_ID_KEY    5
#define SM_CHECK_INIT_SIGN_KEY  6

typedef enum
{
	SM_UT_IDLE,

	SM_UT_GEN_RANDOM_ADDR,
	SM_UT_WAIT_GEN_RANDOM_ADDR,
	SM_UT_SEND_RESERVED_REQ,
	SM_UT_WAIT_RESERVED_RES,
	SM_UT_SEND_HCI_LE_CC,
	SM_UT_SEND_HCI_LE_LTK_REQ,
	SM_UT_WAIT_HCI_LE_LTK_REPLY,
	SM_UT_SEND_PAIRING_REQ,
	SM_UT_WAIT_SECURITY_REQ,
	SM_UT_WAIT_PAIRING_RES,
	SM_UT_WAIT_INPUT_BY_STK_METHOD,	
	SM_UT_SEND_PAIRING_CONFIRM,
	SM_UT_WAIT_PAIRING_CONFIRM,
	SM_UT_WAIT_PAIRING_RANDOM,
	SM_UT_WAIT_HCI_LE_STK_REPLY,
	SM_UT_WAIT_ENC_INFO,
	SM_UT_WAIT_MASTER_ID,
	SM_UT_WAIT_ID_INFO,
	SM_UT_WAIT_ID_ADDR_INFO,
	SM_UT_WAIT_SIGN_INFO,
	SM_UT_SEND_ENC_INFO,
	SM_UT_SEND_MASTER_ID,
	SM_UT_SEND_ID_INFO,
	SM_UT_SEND_ID_ADDR_INFO,
	SM_UT_SEND_SIGN_INFO,
	
	SM_UT_COMPLETE

} SM_UT_STATE;

// For test spec  =============================
typedef enum
{	
	TP_DEFAULT,
	TP_IO_CAP_TEST,
	TP_PAIRING_FAIL_01,
	TP_PAIRING_FAIL_05,
	TP_PAIRING_FAIL_07,
	TP_PAIRING_FAIL_09,
	TP_PAIRING_FAIL_0A,
	TP_PROFILE_GEN_STATIC_ADDR,
	TP_PROFILE_GEN_NRPA,
	TP_PROFILE_GEN_RPA,
	TP_ALL,
	// Signing of Data
	TP_SIGN_BV_01_C,
	TP_SIGN_BV_03_C,
	TP_SIGN_BI_01_C,
	// SMP timeout
	TP_PROT_BV_02_C,
	// Just work
	TP_JW_BV_02_C,
	TP_JW_BI_03_C,
	TP_JW_BI_02_C,
	// Passkey Entry (PKE)
	TP_PKE_BV_02_C,
	TP_PKE_BV_05_C,
	TP_PKE_BI_03_C,
	// Out of Band (OOB)
	TP_OOB_BV_02_C,
	TP_OOB_BV_04_C,
	TP_OOB_BV_06_C,
	TP_OOB_BV_08_C,
	TP_OOB_BV_10_C,
	TP_OOB_BI_02_C,
	// Encyption Key Size
	TP_EKS_BV_02_C,
	TP_EKS_BI_02_C,
	// Key Distribution during bonding
	TP_KDU_BV_01_C,
	TP_KDU_BV_02_C,
	TP_KDU_BV_03_C,
	// Re-encypt an encyption link with LTK
	TP_KDU_BV_07_C,
	// Slave Initiated Pairing
	TP_SIP_BV_01_C,
	// Slave Initiated Encyption
	TP_SIE_BV_01_C,

} SM_UT_CASE;
// ==============================================

#define L2CAP_SM_CID					0x0006

#define SM_UT_ADDR_TYPE_PUBLIC 			0
#define SM_UT_ADDR_TYPE_RANDOM 			1

#define SM_UT_CONN_MASTER				0
#define SM_UT_CONN_SLAVE				1

#define SM_UT_CONN_ROLE					SM_UT_CONN_MASTER
#define SM_UT_CONN_INTERVAL				0x0006
#define SM_UT_CONN_LATENCY				0x0006
#define SM_UT_SUPERV_TIMEOUT			0x000A
#define SM_UT_MASTER_CLOCK_ACCURACY		0	//500ppm

#define SM_UT_SUPPORT_BONDING			0x01
#define SM_UT_SUPPORT_MITM				0x04

#define SM_UT_SIGNCOUNTER_LEN			0x04
#define SM_UT_MAC_LEN					0x08

#define SM_UT_HCI_CMD_OFFSET			1

typedef struct
{
	SM_UT_CASE 			testCase;
	U16					connHandle;
	U8 					addrType;
	BD_ADDR 			addr;
	SM_IO_CAPABILITY 	ioCapability;
	U8 					oobFlag;	
	U8 					authReq;
	U8 					maxEncypKeySize;
	U8 					iniKeyDistr;
	U8 					respKeyDistr;
} SM_UT_NVRAM_TYPE;

PUBLIC SM_UT_NVRAM_TYPE CODE gSM_ut_nvram_init = 
{
	TP_SIGN_BV_01_C,						//SM_UT_CASE 		test_case;
	0,										//U16				connHandle;
	SM_UT_ADDR_TYPE_PUBLIC,					//U8 				addrType;
	{0X01, 0X02, 0X03, 0X04, 0X05, 0X06}, 	//BD_ADDR 			bdAddr;
	SM_IO_NO_INPUT_NO_OUTPUT,				//SM_IO_CAPABILITY 	iOCapability;
	1,										//U8 				OOBFlag;	
	0,										//U8 				authReq;// 0~1b: Bonding_flags, 2b: MITM
	7,										//U8 				maxEncypKeySize;
	7,										//U8 				ini_key_distr;
	7,										//U8 				resp_key_distr;
};

typedef struct
{
	SM_UT_STATE state;
	OST XDATA_PTR smpTimer;
	OST XDATA_PTR testSmpTimer;
	OST XDATA_PTR waitingTimer;
	U8 testRun;
	U8 negAuthReq;
	U8 stkMethod;
	U8 negIniKeyDistr;
	U8 negRespKeyDistr;
	U8 negMaxEncypKeySize;
	U8 negTKPtr[16];
	U8 mRandPtr[16];
	U8 sRandPtr[16];
	U8 stkPtr[16];
	U8 paringReqCmdPtr[7];
	U8 paringRespCmdPtr[7];
	U8 respAddrType;
	BD_ADDR respAddr;
	U8 mConfirmPtr[16];
	U8 sConfirmPtr[16];		
	// slaver's keys
	U8 haveSlaveLTK;
	U8 sEncypKeySize;
	U8 sLastSecurityLevel;
	U8 sLTKPtr[16];
	U8 sRandomPtr[8];
	U16 sEDIV;
	U8 sIRKPtr[16];
	U8 sAddrType;
	BD_ADDR	sAddr;
	U8 sCSRKPtr[16];
	// master's keys
	U8 haveMasterLTK;	
	U8 mEncypKeySize;
	U8 mLastSecurityLevel;
	U8 mLTKPtr[16];	
	U16 mEDIV;
	U8 mRandomPtr[8];
	U8 mIRKPtr[16];
	U8 mCSRKPtr[16];	
} SM_UT_CTL_TYPE;

typedef struct
{
	SM_IO_CAPABILITY ioCapability;
	U8 oobFlag;
	SM_AUTH_REQ_FLAG authReq;
    U8 encKeySize;
    U8 initKeyDistr;
	U8 respKeyDistr;
    U8 oobData[16];
	U8 genRandAddrType;
}SM_UT_TEST_PARA;

PUBLIC SM_UT_TEST_PARA CODE gSM_ut_para_init = 
{
	SM_IO_NO_INPUT_NO_OUTPUT,	//SM_IO_CAPABILITY iOCapability;
	0x00,						//U8 oobDataFlag;
	{0},						//SM_AUTH_REQ_FLAG authReq;
    7,							//U8 encKeySize;
    0,							//U8 initKeyDistr;
	0,							//U8 respKeyDistr;
    {0},						//U8 oobData[16];
};

SM_EXTERN SM_UT_NVRAM_TYPE XDATA gSM_ut_nvram;
SM_EXTERN SM_UT_CTL_TYPE XDATA gSM_ut_ctl;
SM_EXTERN SM_UT_TEST_PARA XDATA gSM_ut_para;

U8 CODE hci_le_ltk_req_evt[] =
{
	1,											// DATA_OFFSET_LMHC_CMDEVT
	HCI_EVENT_LE_META,							// event_code
	21,											// para_length
	HCI_EVENT_LE_LONG_TERM_KEY_REQUEST,			// subevent_code
	0, 0,										// connection_handle
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// random_number
	0, 0,										// encryption_diversifier(EDIV)
};

U8 CODE hci_encryption_change_evt[] =
{
	1,											// DATA_OFFSET_LMHC_CMDEVT
	HCI_EVENT_ENCRYPTION_CHANGE,				// event_code
	4,											// para_length
	0,											// status
	0, 0,										// connection_handle
	1,											// encryption_enabled
};

U8 CODE pairing_req_cmd[] = 
{
	7, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_PAIRING_REQUEST,						// cmd_code
	SM_IO_KEYBOARD_ONLY,						// IO_capability
	0,											// OOB_data_flag
	1,											// AuthReq		// 0~1b: Bonding_flags, 2b: MITM
	7,											// Max_encyp_key_size (7~16)
	0,											// ini_key_distr
	0,											// resp_key_distr
};

U8 CODE pairing_fail_cmd[] = 
{
	2, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_PAIRING_FAILED,						// cmd_code
	SM_FAIL_RESERVED,							// reason
};

U8 CODE pairing_confirm_cmd[] = 
{
	17, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_PAIRING_CONFIRM,						// cmd_code
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// confirm
};

U8 CODE pairing_random_cmd[] = 
{
	17, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_PAIRING_RANDOM,						// cmd_code
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// rand
};

U8 CODE encyption_info_cmd[] = 
{
	17, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_ENCRYPTION_INFORMATION,				// cmd_code
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// ltk
};

U8 CODE master_id_cmd[] = 
{
	11, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_MASTER_IDENTIFICATION,				// cmd_code
	0, 0,										// ediv
	0,0,0,0,0,0,0,0,							// rand
};

U8 CODE id_info_cmd[] = 
{
	17, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_IDENTITY_INFORMATION,				// cmd_code
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// irk
};

U8 CODE id_addr_info_cmd[] = 
{
	9, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_IDENTITY_ADDRESS_INFORMATION,		// cmd_code
	0, 0,										// addr_type
	0,0,0,0,0,0									// bd_addr
};

U8 CODE sign_info_cmd[] = 
{
	17, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	SM_CMD_SIGNING_INFORMATION,					// cmd_code
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			// csrk
};

U8 CODE reserved_cmd[] = 
{
	1, 0,										// length	
	(U8)L2CAP_SM_CID, (U8)(L2CAP_SM_CID >> 8),	// cid
	0,											// cmd_code
};

U8 CODE testCSRK[] =
{
	0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
	0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
};

U8 CODE testMData16[] =
{
	0x6b, 0xc1, 0xbe, 0xe2,	// signCounter msb 
	0x2e, 0x40, 0x9f, 0x96,
	0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
};

U8 CODE testMData16_MAC[] =
{
	0x07, 0x0a, 0x16, 0xb4, 0x6b, 0x4d, 0x41, 0x44,
};

U8 CODE testMData40[] =
{
	0x6b, 0xc1, 0xbe, 0xe2, // signCounter
	0x2e, 0x40, 0x9f, 0x96,
	0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
  	0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
	0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
  	0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,	
};

U8 CODE testMData40_MAC[] =
{
	0xdf, 0xa6, 0x67, 0x47, 0xde, 0x9a, 0xe6, 0x30,
};

U8 CODE testMData64[] =
{
	0x6b, 0xc1, 0xbe, 0xe2, // signCounter
	0x2e, 0x40, 0x9f, 0x96,
	0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
	0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
	0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
	0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10,	
};

U8 CODE testMData64_MAC[] =
{
	0x51, 0xf0, 0xbe, 0xbf, 0x7e, 0x3b, 0x9d, 0x92,
};

U8 CODE testMData64_WrongMAC[] =
{
	0x50, 0xf0, 0xbe, 0xbf, 0x7e, 0x3b, 0x9d, 0x92,
};

PUBLIC void SM_UnitTester(void);
#endif
#endif
#endif
#endif

