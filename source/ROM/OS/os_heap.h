#ifndef _OS_HEAP_H_
#define _OS_HEAP_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_heap.h is the file defines the heap start of system
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



EXTERN U32	XDATA gOS_HEAP_MEMORY_TOP;
////////////////////////////////////////////////////////////////////////////////
// HEAP MEMORY FUNCTION PROTOTYPES /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OSHEAP_init
 *
 * Do Heap Memory initialization
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @pMemoryTop : Pointer of memory top
 */
EXTERN VOID OSHEAP_init(
    IN      OS_DATA_PTR pMemoryTop);

/**
 * OSHEAP_malloc
 *
 * Heap Memory allocation
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Size : Number of bytes memory to allocate
 * @Alignment : Define the memory alignment boundary
 *              (2 ^ N, where N is nature number)
 * @Return : Pointer of allocated memory
 */
EXTERN OS_DATA_PTR OSHEAP_malloc(
    IN      U16 Size,
    IN      U8 Alignment);

#endif /* _OS_HEAP_H_ */

