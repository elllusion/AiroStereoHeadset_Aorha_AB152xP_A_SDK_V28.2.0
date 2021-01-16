//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _PBAP_INTERFACE_H_
#define _PBAP_INTERFACE_H_

#ifdef _PBAP_C_
  #define PBAP_EXTERN  
#else
  #define PBAP_EXTERN extern
#endif

#define PBAP_MAX_FRAME_SIZE		255

enum
{
  PBAP_INVALID,
  PBAP_CONNECT_CMD,
  PBAP_DISCONNECT_CMD,
  PBAP_GET_PHONEBOOK_CMD,
  PBAP_SET_FOLDER_CMD,
  PBAP_PUT_VCARD_LISTING_CMD,
};

typedef struct
{
  U8 cmdCode;
  U8 linkIndex;
}PbapCmd;

PUBLIC void PBAP_Init(void);
PUBLIC void PBAP_Reset(void);

PBAP_EXTERN OSMQ XDATA OSMQ_PBAP_Command_From_MMI[MAX_MULTI_POINT_NO];
#define OSMQ_PBAP_Command_From_MMI_ptr	&OSMQ_PBAP_Command_From_MMI

PBAP_EXTERN OSMQ XDATA OSMQ_PBAP_RSP_From_Remote[MAX_MULTI_POINT_NO];
#define OSMQ_PBAP_RSP_From_Remote_ptr	&OSMQ_PBAP_RSP_From_Remote

#endif
