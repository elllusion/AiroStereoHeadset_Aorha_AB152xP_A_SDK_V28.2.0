#ifndef _SM_INTERFACE_H_
#define _SM_INTERFACE_H_

#include "..\COMMON\le_common_hal.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef SM_SUPPORTED

//#include <rc.h>

#ifdef _SM_C_
#define SM_EXTERN
#else
#define SM_EXTERN extern
#endif  

#define SM_SLAVE  0
#define SM_MASTER 1

#define SM_PASSKEY_MAX_BYTE_LEN	3
#define SM_MASTER_ID_LEN        10  //RAND:8 bytes, EDIV:2 bytes 

#define SM_MIN_ENCRYPTION_KEY_SIZE 7    
#define SM_MAX_ENCRYPTION_KEY_SIZE 16

/* BD_ADDR TYPE */
#define SM_BD_ADDR_TYPE_PUBLIC  0x00
#define SM_BD_ADDR_TYPE_RANDOM  0x01

/* RANDOM BD_ADDR TYPE */
#define SM_RAND_ADDR_STATIC     0x00
#define SM_RAND_ADDR_NON_RESOLV 0x01
#define SM_RAND_ADDR_RESOLV     0x02

/* SM_SECURITY_MODE */
#define SM_SECURITY_MODE1_LEVEL1 0
#define SM_SECURITY_MODE1_LEVEL2 1
#define SM_SECURITY_MODE1_LEVEL3 2
#define SM_SECURITY_MODE2_LEVEL1 3
#define SM_SECURITY_MODE2_LEVEL2 4

/* SM_IO_CAPABILITY */
#define SM_IO_DISPLAY_ONLY          0x00
#define SM_IO_DISPLAY_YES_NO		0x01
#define SM_IO_KEYBOARD_ONLY         0x02	
#define SM_IO_NO_INPUT_NO_OUTPUT    0x03
#define SM_IO_KEYBOARD_DISPLAY      0x04
#define SM_IO_CAPABILITY_MAX        0x05

/* SM_OOB_DATA_FLAG */
#define SM_OOB_AUTH_DATA_NOT_PRESENT    0x00
#define SM_OOB_AUTH_DATA_FROM_REMOTE    0x01

/* SM_BONDING_FLAG */
#define SM_NO_BONDING                   0x00
#define SM_BONDING                      0x01

/* SM_CMD */
#define SM_CMD_VERIFY_SIGN                      0x01
#define SM_CMD_GENERATE_SIGN                    0x02
#define SM_CMD_VERIFY_RAND_ADDR_REQ             0x03
#define SM_CMD_GENERATE_RAND_ADDR_REQ           0x04
#define SM_CMD_PASSKEY_INPUT_RSP                0x05
#define SM_CMD_PASSKEY_DISPLAY_RSP              0x06
#define SM_CMD_SEND_SECURITY_REQ                0x07
#define SM_CMD_MMI_UPDATE_LINK_STATUS           0x08

/* SM_EVT */
#define SM_EVT_VERIFY_SIGN_COMPLETE             0x01
#define SM_EVT_GENERATE_SIGN_COMPLETE           0x02
#define SM_EVT_VERIFY_RAND_ADDR_RSP             0x03
#define SM_EVT_GENERATE_RAND_ADDR_RSP           0x04
#define SM_EVT_PASSKEY_INPUT_REQ                0x05
#define SM_EVT_PASSKEY_DISPLAY_REQ              0x06
#define SM_EVT_SEND_SECURITY_REQ_COMPLETE       0x07
#define SM_EVT_MMI_UPDATE_LINK_ENC_STATUS       0x08
//last SM EVT must < SM_NOTI_OFFSET

/* SM_NOTIFICATION */
#define SM_NOTI_OFFSET                          0x80
#define SM_NOTI_SMP_TIMEOUT                     (SM_NOTI_OFFSET + 0x00)
#define SM_NOTI_LOST_BOND                       (SM_NOTI_OFFSET + 0x01)    // GAP TP/SEC/AUT/BV-22-C

/* SM_STATUS */
#define SM_STATUS_OK 					0
#define SM_STATUS_BUSY					1	// verify or generate signature
#define SM_STATUS_PROCESSING			2	// authentication procedure
#define SM_STATUS_FAIL					3
#define SM_STATUS_NOT_SUPPORT			4
#define SM_STATUS_INVALID_PARA			5
#define SM_STATUS_INPUT_CANCEL			6
#define SM_STATUS_KEY_NOT_EXIST			7
#define SM_STATUS_INSUFFICIENT_ENC      8
#define SM_STATUS_INSUFFICIENT_AUTH     9

typedef struct
{
	U8 passkey[SM_PASSKEY_MAX_BYTE_LEN];
	U8 status;
}SM_PASSKEY_INPUT;

typedef struct
{
	U8 addr[BD_ADDR_LEN];
	U8 status;
}PROFILERandAddrEvt;

typedef struct
{
	U8 addr[BD_ADDR_LEN];
	U8 status;
}SM_VERIFY_RAND_ADDR;

typedef struct
{
	U8 addr[BD_ADDR_LEN];
	U8 addrType;
	U8 status;
}SM_GEN_RAND_ADDR;

typedef struct
{
	U8 XDATA_PTR dataPtr;      //For Gen Sign, data and datalen must reserve space for signature
    U16 dataLen;
    U8 status;
}PACKED_STRUCT SM_SIGN;

typedef struct
{
    U8 linkStatus;
}SM_MMI_UPDATE_LINK_STATUS;

typedef struct
{
    U8 status;
}SM_SEND_SECURITY_REQ;

typedef union 
{
    SM_SIGN sign;
    SM_PASSKEY_INPUT keyPara;
    SM_GEN_RAND_ADDR genRandPara;
    SM_VERIFY_RAND_ADDR verifyRandPara;
    SM_SEND_SECURITY_REQ secReq;
    SM_MMI_UPDATE_LINK_STATUS updateState;
	U8 pkDisplay[SM_PASSKEY_MAX_BYTE_LEN];
	U8 randAddrType;
    U8 status;
}SM_MSG_MSGBODY;

typedef struct
{
    U8 opCode;
    U8 linkIdx;
    U8 dataLen;
    U8 reserved;
}SM_MSG_HEADER;

typedef struct
{
    SM_MSG_HEADER hdr;
	SM_MSG_MSGBODY msg;
}SM_MSG;

//PUBLIC U8 SM_Authentication(U8 idx);
PUBLIC U8 SM_GetEncKeySize(U8 idx);
PUBLIC BOOL SM_IsLinkEncrypted(U8 idx);
PUBLIC void SM_Init(void);
PUBLIC void SM(void);

#endif /* SM_SUPPORTED */
#endif /* LE_SUPPORTED_HOST_FLASH */
#endif /* _SM_INTERFACE_H_ */

