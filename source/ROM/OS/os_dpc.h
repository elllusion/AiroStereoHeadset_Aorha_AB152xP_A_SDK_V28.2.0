#ifndef _OS_DPC_H_
#define _OS_DPC_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_DPC.h defines the interface about DPC (Defer Process Control)
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "os_timer.h"



////////////////////////////////////////////////////////////////////////////////
// MACROS //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define MS_TO_BTCLK(MS) ((10000 * (MS)) / 3125)



////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef VOID (*CALL_BACK_FUNC)(VOID);



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_DPC_in_BTCLK
 *
 * DPC
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @fCallBack : Callback function pointer
 * @Period : Defer period
 * @Return : Handle of DPC
 */
OS_DATA_PTR OS_DPC_in_BTCLK(
    IN      CALL_BACK_FUNC fCallBack,
    IN      BTCLK Period);

#define OS_DPC(f,p) OS_DPC_in_BTCLK(f,MS_TO_BTCLK((p)))
#define OS_DPC_U32(f,p) OS_DPC_in_BTCLK(f,MS_TO_BTCLK(((U32)p)))

/**
 * OS_DPC_init
 *
 * DPC Initialization
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
VOID OS_DPC_init(VOID);

/**
 * OS_DPC_Cancel
 *
 * Cancel a DPC
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Handle : DPC Handle
 * @Return : TRUE stands for success
 */
BOOL OS_DPC_Cancel(
    IN      OS_DATA_PTR Handle);

/**
 * OS_DPC_Kernel
 *
 * DPC Kernel
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
VOID OS_DPC_Kernel(VOID);

#endif /* _OS_DPC_H_ */

