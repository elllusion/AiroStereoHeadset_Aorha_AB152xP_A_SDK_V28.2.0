//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _MAP_INTERFACE_H_
#define _MAP_INTERFACE_H_

#ifdef _MAP_C_
  #define MAP_EXTERN  
#else
  #define MAP_EXTERN extern
#endif

#define MAP_MAX_FRAME_SIZE		255

enum
{
  MAP_INVALID,
  MAP_CONNECT_CMD,
  MAP_DISCONNECT_CMD,
  MAP_SET_NOTIFI_REG_ON_CMD,
  MAP_SET_NOTIFI_REG_OFF_CMD,
  MAP_SET_FOLDER_CMD,
  MAP_GET_FOLDER_LISTING_CMD,
  MAP_GET_MESSAGES_LISTING_CMD,
  MAP_GET_MESSAGE_CMD,
  MAP_SET_MESSAGE_STATUS_READ_CMD,
  MAP_SET_MESSAGE_STATUS_DELETE_CMD,
  MAP_PUSH_MESSAGE_CMD,
  MAP_UPDATE_INBOX_CMD,
};

typedef struct
{
  U8 cmdCode;
  U8 connHandleIndex;
  U8 connHandle;
}MapCmd;

PUBLIC void MAP(void);
PUBLIC void MAP_Init(void);
PUBLIC void MAP_Reset(void);

MAP_EXTERN OSMQ XDATA OSMQ_MAP_Command_From_MMI[MAX_MULTI_POINT_NO];
#define OSMQ_MAP_Command_From_MMI_ptr	&OSMQ_MAP_Command_From_MMI

MAP_EXTERN OSMQ XDATA OSMQ_MAP_RSP_From_Remote[MAX_MULTI_POINT_NO];
#define OSMQ_MAP_RSP_From_Remote_ptr	&OSMQ_MAP_RSP_From_Remote

#endif