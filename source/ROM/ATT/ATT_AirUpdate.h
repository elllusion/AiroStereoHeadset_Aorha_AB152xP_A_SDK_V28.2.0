#ifdef AIR_UPDATE_ENABLE
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _ATT_AIRUPDATE_H_
#define _ATT_AIRUPDATE_H_

#include "cpu.h"

#define AU_ENABLE_RANDOM_PLAIN
#define AU_ENABLE_ERASE_CHIP
//#define AU_ENABLE_PROTECT_1ST_SECTOR

#define AU_TIMER ONE_SEC*10 //ONE_SEC
#define AU_AUTH_TIMER ONE_SEC*30
//DES constant
#define AU_AUTH_8TH_BYTE 0xA1

#define AU_AUTH_LEN                 8
#define AU_AUTH_PLAIN_LEN           7
#define AU_ACTION_SETUP_LEN         (sizeof(AU_ActionSetupType))
#define AU_FLASH_DATA_LEN           (sizeof(AU_FlashDataType))
#define AU_FLASH_DATA_INFO_LEN      7   //OP(1)+HANDLE(2)+ADDR(3)+DATALen(1), total length need to add dataLen
#define AU_FLASH_DATA_HEADER_LEN    4   //ADDR(3)+DATALen(1)
#define AU_DATA_END_LEN             1
#define AU_ACTION_REPORT_LEN        (sizeof(AU_ActionReportType))
#define AU_REBOOT_LEN               1
#define AU_FLASH_INFO_LEN           1
#define AU_SFR_ACCESS_LEN           (sizeof(AU_SFR_AccessType))
#define AU_SFR_ACCESS_RSP_LEN       (sizeof(AU_SFR_AccessRspType))

#define AU_ADDR_LEN 3
#define AU_MAX_DATA_SEGMENT_SIZE 200
#define AU_BUFFER_SIZE 256

#define AU_PAGE_WRITE_COUNT_SECTOR 16
#define AU_PAGE_WRITE_COUNT_32K 128
#define AU_PAGE_WRITE_COUNT_64K 256

#define AU_FLASH_SIZE_4MBITS 4 
#define AU_FLASH_SIZE_8MBITS 8
#define AU_FLASH_SIZE_16MBITS 16

#define AU_FLASH_DATA_PACKET_SIZE(ptr)  *(ptr + 3)

#define AU_FLASH_DEFAULT_VALUE      0xFF

/***** Air Update STATE *****/
#define AU_STATE_NONE               0
#define AU_STATE_AUTHENTICATING     1
#define AU_STATE_IDLE               2
#define AU_STATE_WRITE              3
#define AU_STATE_READ               4
#define AU_STATE_SFR_ACCESS         5
//#define AU_STATE_SEND_ACTION_RSP    6

/***** Air Update Action *****/
#define AU_ACTION_READ                  0x00
#define AU_ACTION_WRITE_SECTOR          0x01
#define AU_ACTION_WRITE_32KB            0x02
#define AU_ACTION_WRITE_64KB            0x03
#define AU_ACTION_WRITE_CHIP            0x04
#define AU_ACTION_EXT_FLH_READ          0x05
#define AU_ACTION_EXT_FLH_WRITE_SECTOR  0x06
#define AU_ACTION_EXT_FLH_WRITE_32KB    0x07
#define AU_ACTION_EXT_FLH_WRITE_64KB    0x08
#define AU_ACTION_EXT_FLH_WRITE_CHIP    0x09

/***** Air Update Action Reoprt Status *****/
#define AU_STATUS_OK                    0x00
#define AU_STATUS_ERASE_FLASH_OK        0x01
#define AU_STATUS_WRITE_FLASH_COMPLETE  0x02
#define AU_STATUS_ERROR                 0x03
#define AU_STATUS_ACCESS_DENIED         0x04
#define AU_STATUS_DATA_ADDR_ERR         0x05
#define AU_STATUS_DATA_LEN_ERR          0x06
#define AU_STATUS_WRITE_FLASH_FAIL      0x07
#define AU_STATUS_TIMER_EXPIRED         0x08
#define AU_STATUS_AUTHENTICATION_FAIL   0x09
#define AU_STATUS_READ_INT_FLASH_INFO   0x0A
#define AU_STATUS_INIT_EXT_FLASH_OK     0x0B
#define AU_STATUS_INIT_EXT_FLASH_FAIL   0x0C
#define AU_STATUS_READ_EXT_FLASH_INFO   0x0D
#define AU_STATUS_NO_SUPPORT            0x0E
#define AU_STATUS_INVALID_PARA          0x0F

/***** Protected Flash Addr (ignore 1st Sector) *****/
#ifdef AU_ENABLE_PROTECT_1ST_SECTOR
#define AU_PROTECTED_FLASH_ADDR_SECTOR  0x00001000
#define AU_PROTECTED_FLASH_ADDR_32K     0x00008000
#define AU_PROTECTED_FLASH_ADDR_64K     0x00010000
#endif

/***** Air Update Flash Information Action *****/
#define AU_FLH_READ_INT_FLASH_INFO          0
#define AU_FLH_INIT_EXT_FLASH_0             1
#define AU_FLH_INIT_EXT_FLASH_1             2
#define AU_FLH_INIT_EXT_FLASH_2             3
#define AU_FLH_READ_EXT_FLASH_INFO          4

/***** Air Update Access SFR action *****/
#define AU_SFR_ACCESS_RD_SFR                0
#define AU_SFR_ACCESS_WR_SFR                1
#define AU_SFR_ACCESS_RD_3WIRED             2
#define AU_SFR_ACCESS_WR_3WIRED             3

#define AU_CHECK_WRITE_INT_FLASH(x)     (((x > AU_ACTION_READ) && (x < AU_ACTION_EXT_FLH_READ))?TRUE:FALSE)

typedef enum
{
	AU_UUID_PRIM_SERVICE = 0x8000,
    AU_UUID_AUTHENTICATION,
	AU_UUID_ACTION_SETUP,
	AU_UUID_FLASH_DATA,
    AU_UUID_DATA_END,
    AU_UUID_ACTION_REPORT,
    AU_UUID_REBOOT,
    AU_UUID_FLASH_INFO,
    AU_UUID_SFR_ACCESS,
    AU_UUID_SFR_ACCESS_RSP
}AU_UUID;

typedef struct
{
    U8 action;
    U8 startAddr[AU_ADDR_LEN];
    U16 pageCount;
}AU_ActionSetupType;
  
typedef struct
{
    U8 addr[AU_ADDR_LEN];
    U8 dataLen;
    U8 dataSeg[AU_MAX_DATA_SEGMENT_SIZE];
}AU_FlashDataType;

typedef union
{
    U8 addr[AU_ADDR_LEN];
    struct{
        U8 manuId;
        U8 flashSize;  
        U8 rsvd;
    }flh;
}AU_ActionReportInfoType;

typedef struct
{
    U8 status;
    AU_ActionReportInfoType info;
}AU_ActionReportType;

typedef union
{
    struct{
        U8 value;
        U8 offset;
        U8 bank;
    }paraSFR;

    struct{
        U8 LoByte;
        U8 HiByte;
        U8 Addr;
    }para3WIRED;
}AU_AccessType;

typedef struct
{
    U8 action;
    AU_AccessType para;
}AU_SFR_AccessType;

typedef struct
{
    U8 status;
    AU_AccessType para;
}AU_SFR_AccessRspType;

#endif
#endif
