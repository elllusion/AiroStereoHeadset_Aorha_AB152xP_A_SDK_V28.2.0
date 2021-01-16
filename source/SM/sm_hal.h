#ifndef __SM_HAL_H__
#define __SM_HAL_H__

#include "SM_interface.h"

#ifdef SM_SUPPORTED

#ifdef _SM_HAL_C_
	#define SM_HAL_EXTERN
#else
	#define SM_HAL_EXTERN extern	
#endif

#ifdef AB1600

#include "MMI_flash.h"

#define SM_NVRAM_BLOCK_SIZE 4
#define SM_NVRAM_BLOCK_LINK_SIZE 4

#define NVRAM_SM_LE_SECURITY_MODE       SM_SECURITY_MODE1_LEVEL2        //SM_SECURITY_MODE
#define NVRAM_SM_IO_CAPABILITY          SM_IO_NO_INPUT_NO_OUTPUT        //SM_IO_CAPABILITY
#define NVRAM_SM_OOB_DATA_FLAG          SM_OOB_AUTH_DATA_NOT_PRESENT    //SM_OOB_DATA_FLAG
#define NVRAM_SM_BONDING_FLAG           SM_BONDING                      //SM_BONDING_FLAG
#define NVRAM_SM_MITM_FLAG              1
#define NVRAM_SM_BDADDR_TYPE            SM_BD_ADDR_TYPE_PUBLIC          //BD_ADDR TYPE
#define NVRAM_SM_ENC_KEY_SIZE           SM_MAX_ENCRYPTION_KEY_SIZE
#define NVRAM_SM_KEY_DISTRIBUTION       0x3F                            //initKeyDistr: bit0~bit2, respKeyDistr: bit3~bit5

#define SM_GET_LOCAL_BD_ADDR_TYPE()		SM_BD_ADDR_TYPE_PUBLIC

typedef struct
{
	U16 len;
	U8 spiCmd;
	U8 xportPktId;	
} SM_HCI_HAL_HDR_STRU;

typedef struct
{
	U8 numCmdPkts;
	U16 cmdCode;
} SM_HCI_HAL_EVT_HDR_STRU;

typedef struct
{
	U8 hciXportHdr[HCI_XPORT_HDR_LEN];
} SM_L2CAP_CMD_HAL_HDR_STRU;

PUBLIC void SM_HAL_Init(void);
#ifdef SM_SIGNATURE_ENABLE
PUBLIC void SM_SaveSignCounter(U8 idx);
#endif
PUBLIC void SM_UpdateLinkRecordToFlash(U8 idx);

#else

//#include "..\ROM\COMMON\rc.h"
//#include "..\COMMON\le_common_hal.h"
#include "..\MMI_LE\link_index_ctl.h"
#include "sector_sm_nvram.h"
#include "sector_config1.h"
#include "drv_sector_flash.h"

#define SM_NVRAM_BLOCK_SIZE 1
#define SM_NVRAM_BLOCK_LINK_SIZE MAX_NUM_OF_LINK

SM_HAL_EXTERN SM_NVRAM_TYPE CODE_PTR XDATA gSM_nvram_read_ptr;

#define NVRAM_SM (*gSM_nvram_read_ptr)

#define NVRAM_SM_LE_SECURITY_MODE       (NVRAM_SM.leSecurityRequirement)        //SM_SECURITY_MODE
#define NVRAM_SM_IO_CAPABILITY          (NVRAM_SM.ioCapability)                 //SM_IO_CAPABILITY
#define NVRAM_SM_OOB_DATA_FLAG          (NVRAM_SM.oobDataFlag)                  //SM_OOB_DATA_FLAG
#define NVRAM_SM_BONDING_FLAG           (NVRAM_SM.bondingFlag)                  //SM_BONDING_FLAG
#define NVRAM_SM_MITM_FLAG              (NVRAM_SM.MITM)
#define NVRAM_SM_BDADDR_TYPE            (NVRAM_SM.bdAddrType)                   //BD_ADDR TYPE
#define NVRAM_SM_ENC_KEY_SIZE           (NVRAM_SM.encKeySize)
#define NVRAM_SM_KEY_DISTRIBUTION       (NVRAM_SM.keyDistr)                     //initKeyDistr: bit0~bit2, respKeyDistr: bit3~bit5

#define SM_L2CAP_RSP_PKT_OFFSET     9

#define SM_GET_LOCAL_BD_ADDR_TYPE()     pMpParameterF->leBDAddr.type

typedef struct
{
	U8 offset;
	U8 rsvd[1];
} SM_HCI_HAL_HDR_STRU;

typedef struct
{
	U8 offset;
	U8 rsvd[4];
} SM_L2CAP_CMD_HAL_HDR_STRU;

typedef struct
{
	U8 offset;
	U8 rsvd[3];
} SM_L2CAP_EVT_HAL_HDR_STRU;

typedef struct
{
	U8 offset;
	U8 rsvd[2];
	U8 evtCode;
	U8 len;
} SM_HCI_HAL_EVT_HDR_STRU;

PUBLIC void SM_HAL_Init(void);
#ifdef SM_SIGNATURE_ENABLE
PUBLIC void SM_SaveSignCounter(U8 idx);
#endif
PUBLIC void SM_UpdateLinkRecordToFlash(U8 idx);
PUBLIC void SM_FreeLinkRecordPtr(U8 idx);
PUBLIC BOOL SM_GetLinkRecordPtr(U8 idx);

// Queues of related to SM
SM_HAL_EXTERN OSMQ XDATA OSMQ_HC_Event_To_SM;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Command_From_GAP;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Event_To_GAP;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Command_From_MMILE;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Event_To_MMILE;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Command_From_ATT;
SM_HAL_EXTERN OSMQ XDATA OSMQ_SM_Event_To_ATT;

#define OSMQ_HC_Event_To_SM_ptr         &OSMQ_HC_Event_To_SM
#define OSMQ_SM_Command_From_GAP_ptr    &OSMQ_SM_Command_From_GAP
#define OSMQ_SM_Event_To_GAP_ptr        &OSMQ_SM_Event_To_GAP
#define OSMQ_SM_Command_From_MMILE_ptr  &OSMQ_SM_Command_From_MMILE
#define OSMQ_SM_Event_To_MMILE_ptr      &OSMQ_SM_Event_To_MMILE
#define OSMQ_SM_Command_From_ATT_ptr    &OSMQ_SM_Command_From_ATT
#define OSMQ_SM_Event_To_ATT_ptr        &OSMQ_SM_Event_To_ATT

#define OSMQ_HC_Command_From_SM_ptr     OSMQ_HC_Command_From_Bus_ptr
#define OSMQ_SM_Event_To_MMI_ptr        OSMQ_SM_Event_To_MMILE_ptr
#define OSMQ_SM_Command_From_MMI_ptr    OSMQ_SM_Command_From_MMILE_ptr
#endif

#endif /* SM_SUPPORTED */
#endif /* __SM_HAL_H__ */
