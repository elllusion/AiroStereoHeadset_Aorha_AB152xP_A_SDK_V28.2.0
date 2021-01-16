#ifndef _LL_H_
#define _LL_H_
#include "config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "cpu.h"
#include "LL_interface.h"



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID LL(VOID);
EXTERN VOID LL_HciCmdHdlr(VOID);
EXTERN VOID LL_LcEvtHdlr(VOID);
EXTERN VOID LL_HciCmdBylinkHdlr(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN VOID LL_LcLmpHdlr(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN VOID LL_LcEvtBylinkHdlr(LL_LINK_CONTROL_STRU_PTR pLinkCtl);

EXTERN VOID LL_SMRelease(VOID);
EXTERN LL_CMD_STRU_PTR LL_SendCmdtoLC(U8 opcode, U8 para);
EXTERN VOID LL_ReleaseProcRspTimer(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN U8 LL_ResetProcRspTimer(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_Init(VOID );
/**
 * LL_LinkTransRelease
 *
 * LL release transaction by link
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 *
 */
EXTERN VOID LL_LinkTransRelease(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

/**
 * LL_SendCtrDataPdu
 *
 * LL assemble Ctr Data to Remote
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 * @waitAck : wait for Ack or not
 * @ParamSize : ctr data parameter size
 * @OpCode : ctr data parameter size
 * @Return : the pointer of ctr data
 */

EXTERN LL_CTRL_DATA_STRU_PTR LL_SendCtrDataPdu(U8 waitAck, U8 ParamSize, U8 OpCode);

/**
 * LL_SendRejectIndExt
 *
 * LL assemble Reject Ind or Ext Reject Ind to Remote
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 * @waitAck : wait for Ack or not
 * @RejectIndExtSupported : ext Reject Ind supported or not
 * @RejectOpcode : Reject LLCP opcode
 * @ErrCode : Reject reasion
 * @Return : the pointer of ctr data
 */
EXTERN LL_CTRL_DATA_STRU_PTR LL_SendRejectIndExt(
    IN  U8 WaitAck,
    IN  U8 RejectIndExtSupported,
    IN  U8 RejectOpcode,
    IN  U8 ErrCode);


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * LL_LINK_Init
 *
 * Do link scheduler initialization
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
VOID LL_LINK_Init(VOID);

/**
 * LL_LINK_Create
 *
 * Create a new link
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : The pointer of link control sturcture
 */
LL_LINK_CONTROL_STRU_PTR LL_LINK_Create(VOID);

/**
 * LL_LINK_Create
 *
 * Kill a link
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @pLinkCtl : The pointer of link control sturcture
 */
VOID LL_LINK_Kill(
    IN      LL_LINK_CONTROL_STRU_PTR pLinkCtl);

/**
 * LL_LINK_Activate
 *
 * Kill a link
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 *
 * @pLinkCtl : The pointer of link control sturcture
 */
VOID LL_LINK_Activate(
    IN      LL_LINK_CONTROL_STRU_PTR pLinkCtl);
/**
 * LL_LINK_Scheduler
 *
 * The link scheduler
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : The pointer of link control structure
 */
LL_LINK_CONTROL_STRU_PTR LL_LINK_Scheduler(VOID);


/**
 * LL_LINK_ReScheduler
 *
 * The LL LINK scheduler reorder
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 *
 * @Return : The pointer of link control structure
 */
VOID LL_LINK_ReScheduler(VOID);



#endif
#endif /* _LL_H_ */

