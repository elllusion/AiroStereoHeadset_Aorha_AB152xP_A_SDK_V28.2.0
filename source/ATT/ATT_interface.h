#ifndef _ATT_INTERFACE_H_
#define _ATT_INTERFACE_H_

#include "os.h"
#include "..\COMMON\le_common_hal.h"
#ifdef LE_SUPPORTED_HOST_FLASH

#ifdef _ATT_C_
#define ATT_EXTERN
#else
#define ATT_EXTERN extern
#endif

#define ATT_TYPE_UUID_PRIMARY_SERVICE           (0x2800)
#define ATT_TYPE_UUID_SECCONDARY_SERVICE        (0x2801)
#define ATT_TYPE_UUID_INCLUDE                   (0x2802)
#define ATT_TYPE_UUID_CHAR_DECLARATION          (0x2803)
#define ATT_TYPE_UUID_CHAR_EXT_PROP             (0x2900)
#define ATT_TYPE_UUID_CHAR_USER_DESC            (0x2901)
#define ATT_TYPE_UUID_CLIENT_CHAR_CONFIG        (0x2902)
#define ATT_TYPE_UUID_SERVER_CHAR_CONFIG        (0x2903)
#define ATT_TYPE_UUID_CHAR_FORMAT               (0x2904)
#define ATT_TYPE_UUID_CHAR_AGG_FORMAT           (0x2905)

#define ATT_UUID_16_SIZE                        (2)
#define ATT_UUID_128_SIZE                       (16)
#define ATT_INFORMATION_DATA_UUID_16_FORMAT     (1)
#define ATT_INFORMATION_DATA_UUID_128_FORMAT    (2)

typedef enum {
	ATT_R_PERMIT    = 0x01,			/* Always permitted, no restrictions*/
	ATT_R_AUTHEN    = 0x02,	        /* Authentication required */
	ATT_R_AUTHOR    = 0x04,	        /* Authorization required */
	ATT_R_BANNED    = 0x08,	        /* Operation not permitted */

    ATT_W_PERMIT    = 0x10,			/* Always permitted, no restrictions*/
	ATT_W_AUTHEN    = 0x20,	        /* Authentication required */
	ATT_W_AUTHOR    = 0x40,	        /* Authorization required */
	ATT_W_BANNED    = 0x80,	        /* Operation not permitted */
} ATT_READ_WRITE_PERMISSION_ENUM;

typedef struct
{
  	U8 reqOpcodeInErr;
  	U16 attHandleInErr;
  	U8 errCode;
}PACKED_STRUCT ATTERRRSP_T;

typedef struct
{
    U16 startHandle;
    U16 endHandle;
    U16 attType;
    U16 attValueLen;
    U8 attValuePtr[1];
} ATT_FIND_BY_TYPE_VALUE_PARA;

typedef struct
{
    U16 startHandle;
    U16 endHandle;
    U8 attType[16];
    U8 attTypeLen;  // 2 or 16 octet UUID
} ATT_READ_BY_TYPE_PARA;

typedef struct
{
    U16 startHandle;
    U16 endHandle;
} ATT_HANDLE_RANGE_STRU;

typedef struct
{
    U16 startHandle;
    U16 endHandle;
} ATT_FIND_INFO_PARA;

typedef struct
{
    U16 handle;
    U16 len;
    U8 dataPtr[1];
} ATT_WRITE_REQ_PARA;

typedef struct
{
    U8 uuid[16];
    U8 uuidType;
} ATT_UUID_STRU;

typedef struct
{
    U8 XDATA_PTR dataPtr;
    U16 serverIdx; 
    U16 len; 
    U16 maxLen;
    U16 connHdl;
} ATT_SEND_DATA_STRU;

typedef union    
{
    ATT_FIND_BY_TYPE_VALUE_PARA findByTypeValuePara;
    ATT_READ_BY_TYPE_PARA readByTypePara;
    ATT_FIND_INFO_PARA findInfoPara;
    ATT_WRITE_REQ_PARA writePara;
} ATT_PARA_STRU;

#define ATT_HANDLE_RANGE_SIZE   (sizeof(ATT_HANDLE_RANGE_STRU))

PUBLIC void ATT_Init (void);
PUBLIC void ATT(void);
PUBLIC U16  ATT_GetAttributeValueCurrentLen(U16 serverIdx);
PUBLIC VOID ATT_ChangeAttributeValueDataPtr(U16 serverIdx, U8 CODE_PTR newDataPtr);
PUBLIC VOID ATT_ChangeAttributeValueLength(U16 serverIdx, U16 newLength);

PUBLIC U16  ATT_FindServerIndexByAddr(U8 CODE_PTR charAddr);
PUBLIC U16  ATT_GetCCCDByLink(U16 serverIdx, U8 linkIdx);
PUBLIC void ATT_SetCCCDByLink(U16 serverIdx, U8 linkIdx, U16 value);
PUBLIC BOOL ATT_HandleValueReportMux(U8 Opcode, U16 serverIdx, U16 connHdl);
PUBLIC U8 ATT_SetUpPeriodicReport(U16 type, U32 interval, U8 CODE_PTR charAddr);
PUBLIC U8 ATT_CancelPeriodicReport(U8 CODE_PTR charAddr);
PUBLIC void ATT_ClearCCCD(U8 LinkIdx);
PUBLIC BOOL ATT_SendTxData(ATT_SEND_DATA_STRU XDATA_PTR paraPtr);
PUBLIC void ATT_ResetMTUSize(void);
PUBLIC void ATT_ModifyAttrPermisson(U16 serverIdx, U8 permission);

#ifdef ATT_CLIENT_ENABLE
PUBLIC BOOL ATT_SendExchangeMTUReq(U8 linkIdx, U16 mtu);
PUBLIC void ATT_SendFindByTypeValueReq(U8 linkIdx, ATT_FIND_BY_TYPE_VALUE_PARA XDATA_PTR paraPtr);
PUBLIC void ATT_SendReadByTypeReq(U8 linkIdx, ATT_READ_BY_TYPE_PARA XDATA_PTR paraPtr);
PUBLIC void ATT_SendFindInfoReq(U8 linkIdx, ATT_FIND_INFO_PARA XDATA_PTR paraPtr);
PUBLIC BOOL ATT_SendWriteReq(U8 linkIdx, ATT_WRITE_REQ_PARA XDATA_PTR paraPtr);
#endif

#define ATT_CURRENT_MTU_SIZE	((const U16 XDATA_PTR)&gATT_ctl.attMtu)

#endif /* LE_SUPPORTED_HOST_FLASH */
#endif /* _ATT_INTERFACE_H_ */
