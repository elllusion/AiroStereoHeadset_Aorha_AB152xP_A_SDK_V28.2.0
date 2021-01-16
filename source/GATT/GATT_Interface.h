#ifndef _GATT_INTERFACE_H_ 
#define _GATT_INTERFACE_H_

#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef GATT_ENABLE
#include "..\ATT\ATT_interface.h"

typedef enum
{
    GATT_STATUS_SUCCESS,
    GATT_STATUS_BUSY,
    GATT_STATUS_NO_RESOURCE,
    GATT_STATUS_INVALID_PARA,
    GATT_STATUS_EXIST_CB,
    GATT_STATUS_NOT_SUPPORTED,
    GATT_STATUS_TIMEOUT,
    GATT_STATUS_INSUFFICIENT_AUTHEN,
    GATT_STATUS_INSUFFICIENT_ENCRYPT,
    GATT_STATUS_UNKNOWN_ERR,
    GATT_STATUS_NUM,
} GATT_STATUS;

typedef enum
{
    GATT_EVT_PRIM_SERVC_DISC_RSP,
    GATT_EVT_CHAR_DISC_RSP,
    GATT_EVT_DESC_DISC_RSP,
    GATT_EVT_WRITE_RSP,
    GATT_EVT_HVX,
} GATT_EVT_ID;

typedef enum
{
    GATT_HVX_NOTIFICATION,
    GATT_HVX_INDICATION,
};

typedef enum
{
    GATT_WRITE_INVALID,
    GATT_WRITE_REQ,
    GATT_WRITE_CMD,
    GATT_SIGN_WRITE_CMD,
    GATT_PREP_WRITE_REQ,
    GATT_EXEC_WRITE_REQ,    
} GATT_WRITE_OP;

typedef struct
{
    U8 uuidLen;
    U8 uuid[16];
} GATT_UUID_STRU;

typedef struct
{
    U16 startHandle;
  	U16 endHandle;
  	GATT_UUID_STRU serviceUUID;
}GATT_SERVICE_STRU;

typedef struct
{
    U16 count;
    GATT_SERVICE_STRU service[1];
} GATT_PRIM_SERV_DISC_RSP;

typedef struct
{
    U16 charDeclHandle;    
    U8 charProps;
    U16 charValHandle;
    GATT_UUID_STRU charUUID;
} GATT_CHAR_DECL_STRU;

typedef struct
{
    U16 count;
    GATT_CHAR_DECL_STRU chars[1];
} GATT_CHAR_DECL_DISC_RSP;

typedef struct
{
    U16 handle;
    GATT_UUID_STRU descUUID;
} GATT_CHAR_DESC_STRU;

typedef struct
{
    U16 count;
    GATT_CHAR_DESC_STRU desc[1];
} GATT_CHAR_DESC_DISC_RSP;

typedef struct
{
    U16 handle;
    U8 writeOp;
    U16 offset;
    U16 len;
    U8 dataPtr[1];
} GATT_WRITE_RSP;

typedef struct
{
    U16 handle;
    U8 type;
    U16 len;
    U8 dataPtr[1];
} GATT_HVX_STRU;

typedef union
{
    GATT_PRIM_SERV_DISC_RSP primServDiscRsp;
    GATT_CHAR_DECL_DISC_RSP charDeclDiscRsp;
    GATT_CHAR_DESC_DISC_RSP charDescDiscRsp;
    GATT_WRITE_RSP writeRsp;
    GATT_HVX_STRU hvxEvt;
} GATT_RSP_UNION;

typedef struct
{
    U8 evtId;
    U8 linkIdx;
    U8 status;
    GATT_RSP_UNION rsp;
} GATT_EVT_STRU;

typedef struct
{
    U8 linkIdx;
    U16 startHandle;
    U16 endHandle;
    GATT_UUID_STRU *primServUuidPtr;    
} GATT_PRIM_SERV_DISC_PARA_STRU;

typedef struct
{
    U8 linkIdx;
    U16 startHandle;
    U16 endHandle;
} GATT_CHAR_DISC_PARA_STRU;

typedef struct
{
    U8 linkIdx;
    U16 startHandle;
    U16 endHandle;
} GATT_CHAR_DESC_PARA_STRU;

typedef struct
{
    U8 linkIdx;
    U8 writeOp;
    U8 flag;
    U16 handle;
    U16 offset;
    U16 len;
    U8 value[1];
} GATT_WRITE_PARA_STRU;

typedef struct
{
    U16 charDeclHandle;    
    U8 charProps;
    U16 charValHandle;
    U8 uuid[16];
} GATT_DATA_LIST_STRU;

typedef struct
{
    U8 pairLen;
    GATT_DATA_LIST_STRU attDataList[1];
} GATT_READ_BY_TYPE_RSP_STRU;

typedef struct
{
    U16 handle;
    U8 uuid[16];
} GATT_INFO_DATA_STRU;

typedef struct
{
    U8 format;
    GATT_INFO_DATA_STRU attInfoDataList[1];
} GATT_FIND_INFO_RSP_STRU;

typedef struct
{
    U16 handle;    
    U8 dataPtr[1];
} GATT_WRITE_RSP_STRU;

typedef struct
{
    U16 handle;
    U8 dataPtr[1];
} GATT_NOTI_STRU;

typedef void (*GATT_EVENT_CB)(GATT_EVT_STRU XDATA_PTR evtPtr);

// for AP layrer
PUBLIC U8 GATT_RegisterCB(U8 linkIdx, GATT_EVENT_CB cb);
PUBLIC U8 GATT_UnregisterCB(U8 linkIdx);
PUBLIC U8 GATT_PrimaryServiceDiscover(GATT_PRIM_SERV_DISC_PARA_STRU XDATA_PTR paraPtr);
PUBLIC U8 GATT_CharacteristicDiscover(GATT_CHAR_DISC_PARA_STRU XDATA_PTR paraPtr);
PUBLIC U8 GATT_CharacteristicDescriptorsDiscover(GATT_CHAR_DESC_PARA_STRU XDATA_PTR paraPtr);
PUBLIC U8 GATT_CharacteristicWrite(GATT_WRITE_PARA_STRU XDATA_PTR paraPtr);
PUBLIC void GATT_Reset(U8 linkIdx);

// for ATT layer
PUBLIC void GATT_Timeout(U8 linkIdx);
PUBLIC void GATT_ErrRsp(U8 linkIdx, ATTERRRSP_T XDATA_PTR attErrPtr);
PUBLIC void GATT_FindByTypeValueRsp(U8 linkIdx, U16 len, U8 XDATA_PTR hdlInfoList);
PUBLIC void GATT_ReadByTypeRsp(U8 linkIdx, U16 len, GATT_READ_BY_TYPE_RSP_STRU XDATA_PTR dataList);
PUBLIC void GATT_FindInfoRsp(U8 linkIdx, U16 len, GATT_FIND_INFO_RSP_STRU XDATA_PTR infoList);
PUBLIC void GATT_WriteRsp(U8 linkIdx);
PUBLIC void GATT_HandleValueNoti(U8 linkIdx, U16 len, GATT_NOTI_STRU XDATA_PTR notiPtr);
#endif
#endif

#endif