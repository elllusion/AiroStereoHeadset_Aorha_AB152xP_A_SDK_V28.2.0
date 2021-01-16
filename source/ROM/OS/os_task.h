#ifndef _OS_TASK_H_
#define _OS_TASK_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * os_task.h is the file defines the function of os tasking.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "os_list.h"
#include "os_types.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- Task ID
typedef U8 OS_TASK_ID;
#define KERNEL_TASK_ID                  (OS_TASK_ID)(0)
#define LL_TASK_ID                      (OS_TASK_ID)(1)
#define LM_TASK_ID                      (OS_TASK_ID)(2)
#define HC_TASK_ID                      (OS_TASK_ID)(3)
#define NUM_OF_SUPPORTED_TASK           (4)
//- Task Status
typedef U8 TASK_STATUS;
#define OS_TASK_SUSPEND                 (TASK_STATUS)(0)
#define OS_TASK_RUNNING                 (TASK_STATUS)(1)
#define OS_TASK_WAIT                    (TASK_STATUS)(2)
#define OS_TASK_NO_STATUS               (TASK_STATUS)(-1)
#define NUM_OF_SUPPORTED_TASK_STATUS    (3)
//- Dead Beef Tag
typedef U32 DEAD_BEEF;
#define OS_STACK_DEAD_BEEF_TAG  (DEAD_BEEF)(0xDEADBEFF)



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- Task function pointer
typedef VOID (*fTASK)(VOID);

typedef struct stru_os_task_queue
{
    //- Link List Head
    DLIST_HEAD Head;

    //- Number of tasks in this queue
    U8 NumOfTask;

    //- Status Indicator
    TASK_STATUS StatusInd;

} OS_TASK_QUEUE_STRU, OS_PTR OS_TASK_QUEUE_STRU_PTR;

typedef struct stru_os_task_control_block
{
    //- Link List
    DLIST_STRU List;

    //- Pointer of current Link List Head
    OS_TASK_QUEUE_STRU_PTR pTaskQueue;

    //- Present stack pointer
    U8 OS_PTR pStack;

    //- Number of pending message
    U8 NumOfPendingMsg;

    //- Task ID
    OS_TASK_ID TaskId;

    //- Pointer of DEAD BEEF Tag which is placed in stack top for stack overflow protection.
    DEAD_BEEF OS_PTR pDeadBeefTag;

} OS_TCB_STRU, OS_PTR OS_TCB_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// GLOBAL VRIABLES /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_TASK_Init
 *
 * Do OS scheduler initialization
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_Init(VOID);

/**
 * _OS_TASK_SetIsrStackSize
 *
 * Set ISR Stack Size
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_SetIsrStackSize(
    IN      U16 Size);

/**
 * OS_TASK_Create
 *
 * Create a task
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @TaskId : The task ID
 * @fTaskEntry : The entry function pointer of task
 * @StackSize : The required stack size
 */
EXTERN VOID OS_TASK_Create(
    IN      OS_TASK_ID TaskId,
    IN      fTASK fTaskEntry,
    IN      U16 StackSize);

/**
 * OS_TASK_KernelStart
 *
 * Start the Kernel
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
VOID OS_TASK_KernelStart(VOID);

/**
 * OS_TASK_SetSwitchRequest
 *
 * Signal Switch Request
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_SetSwitchRequest(VOID);

/**
 * OS_TASK_TaskingRequest
 *
 * Signal Switch Request
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_TaskingRequest(VOID);

/**
 * OS_TASK_SwIntrHdlr
 *
 * Software interrupt handler
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_SwIntrHdlr(VOID);

/**
 * OS_TASK_Scheduler
 *
 * Tasking
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_TASK_Scheduler(VOID);

/**
 * OS_TASK_ChangeTaskStatus
 *
 * Change task status
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTCB : The pointer of TCB
 * @TargetStatus : The target status
 */
EXTERN VOID OS_TASK_ChangeTaskStatus(
    IN      OS_TCB_STRU_PTR pTCB,
    IN      TASK_STATUS TargetStatus);

/**
 * OS_TASK_GetMsgNotify
 *
 * Notify OS that a message is received
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTCB : The pointer of TCB
 */
EXTERN VOID OS_TASK_GetMsgNotify(
    IN      OS_TCB_STRU_PTR pTCB);

/**
 * OS_TASK_GetMsgNotify
 *
 * Notify OS that a message is dropped
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTCB : The pointer of TCB
 */
EXTERN VOID OS_TASK_PutMsgNotify(
    IN      OS_TCB_STRU_PTR pTCB);

/**
 * OS_TASK_GetMsgNotify
 *
 * Notify OS that a message is dropped
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTaskQueue : The pointer of task queue
 * @Return : The pointer of TCB
 */
EXTERN OS_TCB_STRU_PTR OS_TASK_GetFirstTask(
    IN      OS_TASK_QUEUE_STRU_PTR pTaskQueue);

/**
 * OS_TASK_Extract
 *
 * Extract a task from task queue
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTCB : The pointer of TCB
 */
EXTERN VOID OS_TASK_Extract(
    IN      OS_TCB_STRU_PTR pTCB);

/**
 * OS_TASK_Insert
 *
 * Append a task to a task queue
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pTCB : The pointer of TCB
 * @pTaskQueue : The pointer of task queue
 */
EXTERN VOID OS_TASK_Insert(
    IN      OS_TCB_STRU_PTR pTCB,
    IN      OS_TASK_QUEUE_STRU_PTR pTaskQueue);

/**
 * OS_TASK_QueryTCB
 *
 * Query the pointer of TCB by the specific task ID
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @TaskId : The specific task ID
 * @Return : The pointer of TCB
 */
EXTERN OS_TCB_STRU_PTR OS_TASK_QueryTCB(
    IN      OS_TASK_ID TaskId);

#endif /* _OS_TASK_H_ */

