#ifndef _OS_MEMORY_H_
#define _OS_MEMORY_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_heap.h is the file defines the heap start of system
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "os_list.h"
#include "os_task.h"



////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define OS_MEMBLK_EOB_SIZE (sizeof(OSMEM_BLK XDATA_PTR))
#define OS_ADDITIONAL_BYTES_FOR_MEMORY_ARRAY (sizeof(OSMEM_BLKHDR)+OS_MEMBLK_EOB_SIZE)



////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* OSMEM is OS MEMORY control */
#if (FEA_SUPP_DSP)
#if (FEA_SUPP_ZSP410_EMBEDDED)
typedef struct osmem_stru
{
    //- 4B, point to list head
    LIST_HEAD           head;
    //- 1B, number of free block
    U8                  noFB;
    //- 1B
    U8                  empty_trap_en;

} OSMEM, OS_PTR OSMEM_PTR;
#else
#error "UNDEFINED CASE"
#endif
#else
typedef struct osmem_stru
{
    //- 4B, point to list head
    LIST_HEAD           head;
    //- 1B, number of free block
    U8                  noFB;

} OSMEM, OS_PTR OSMEM_PTR;
#endif

/* OSMEM_BLKHDR is the header of MEMORY BLOCK */
#if (FEA_SUPP_DSP)
#if (FEA_SUPP_ZSP410_EMBEDDED)
typedef struct osmem_blk_hdr_stru
{
    //- 2B, Link list
    LIST_STRU           list;
    //- 2B, Memory Manager pointer
    OSMEM_PTR           mem_ptr;
    //- 2B, End of block memory
    struct osmem_blk_stru OS_PTR OS_PTR eob_ptr;
    //- 2B, Relative list head
    LIST_HEAD_PTR       head_ptr;
    //- 3B, debug use
    U8                  dbg_getter_addr[3];
    //- 1B, reserved
    U8                  rsvd;

} OSMEM_BLKHDR, OS_PTR OSMEM_BLKHDR_PTR;
#else
#error "UNDEFINED CASE"
#endif
#else
typedef struct osmem_blk_hdr_stru
{
    //- 2B, Link list
    LIST_STRU           list;
    //- 2B, Memory Manager pointer
    OSMEM_PTR           mem_ptr;
    //- 2B, End of block memory
    struct osmem_blk_stru OS_PTR OS_PTR eob_ptr;
    //- 2B, Relative list head
    LIST_HEAD_PTR       head_ptr;
    //- 3B, debug use
    U8                  dbg_getter_addr[3];

} OSMEM_BLKHDR, OS_PTR OSMEM_BLKHDR_PTR;
#endif

/* OSMEM_BLK is OS MEMORY BLOCK */
typedef struct osmem_blk_stru
{
    //- 10B, Header of block memory
    OSMEM_BLKHDR        header;
    //- ?B, Free data space with unknown size
    U8                  data_space[1];

} OSMEM_BLK, OS_PTR OSMEM_BLK_PTR;

/* OSMQ is OS MEMORY QUEUE HEAD */
#if (FEA_SUPP_DSP)
#if (FEA_SUPP_ZSP410_EMBEDDED)
typedef struct osmq_stru
{
    //- 4B, point to list head
    LIST_HEAD           head;
    //- 1B, number of memory blocks
    U8                  noB;
    //- 1B, reserved
    U8                  rsvd;
    //- 2B, pointer of TCB
    OS_TCB_STRU_PTR     pTCB;
    //- 2B, pointer of lock flag
    BOOL XDATA_PTR      pLock;

} OSMQ, OS_PTR OSMQ_PTR;
#else
#error "UNDEFINED CASE"
#endif
#else
typedef struct osmq_stru
{
    //- 4B, point to list head
    LIST_HEAD           head;
    //- 1B, number of memory blocks
    U8                  noB;
    //- 2B, pointer of TCB
    OS_TCB_STRU_PTR     pTCB;
    //- 2B, pointer of lock flag
    BOOL XDATA_PTR      pLock;

} OSMQ, OS_PTR OSMQ_PTR;
#endif



////////////////////////////////////////////////////////////////////////////////
// OSMEM FUNCTION DECLARATIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OSMQ_Init
 *
 * Do OSMQ initialization and chain the relationship between OSMQ and TCB
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @pTCB : Pointer of TCB
 */
VOID OSMQ_Init(
    IN      OSMQ_PTR mq_ptr,
    IN      OS_TCB_STRU_PTR pTCB,
    IN_OPT  BOOL XDATA_PTR pLock);

/**
 * OSMEM_Init
 *
 * Do OS Memory Initialization
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pOSMEM : Pointer of OSMEM
 * @NumBlks : Number of Blocks
 * @BlkSize : Memory Block Size
 * @Alignment : Constrant OSMEM Block to have specified alignment
 */
EXTERN VOID OSMEM_Init (
    IN      OSMEM_PTR pOSMEM,
    IN      U8 NumBlks,
    IN      U16 BlkSize,
    IN      U8 Alignment);

/**
 * OSMEM_Get
 *
 * Get free memory space
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mem_ptr : Pointer of OSMEM
 * @Return : Pointer of free memory space
 */
EXTERN OS_DATA_PTR OSMEM_Get (
    IN      OSMEM_PTR mem_ptr);

/**
 * OSMEM_Put
 *
 * Release memory space
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mem_ptr : Pointer of OSMEM
 */
VOID OSMEM_Put (
    IN      OS_DATA_PTR ptr);

/**
 * OSMEM_NFree
 *
 * Get number of free memory blocks
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mem_ptr : Pointer of OSMEM
 * @Return : Number of free memory blocks
 */
EXTERN U8 OSMEM_NFree (
    IN      OSMEM_PTR mem_ptr);



////////////////////////////////////////////////////////////////////////////////
// OSMQ FUNCTION DECLARATIONS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OSMQ_Get
 *
 * Dequeue from specified message queue
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @Return : Pointer of message
 */
EXTERN OS_DATA_PTR OSMQ_Get (
    IN      OSMQ_PTR mq_ptr);

/**
 * OSMQ_Put
 *
 * Enqueue to specified message queue
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @ptr : Pointer of message
 */
EXTERN VOID OSMQ_Put (
    IN      OSMQ_PTR mq_ptr,
    IN      OS_DATA_PTR ptr);

/**
 * OSMQ_Put
 *
 * Enqueue to specified message queue from front
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @ptr : Pointer of message
 */
EXTERN VOID OSMQ_PutFront (
    IN      OSMQ_PTR mq_ptr,
    IN      OS_DATA_PTR ptr);

/**
 * OSMQ_Put
 *
 * Get number of memory blocks that have been queued in specified OSMQ
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @return : Number of memory blocks.
 */
EXTERN U8 OSMQ_Entries (
    IN      OSMQ_PTR mq_ptr);

/**
 * OSMQ_Flush
 *
 * Flush specified OSMQ
 *
 * @Auther : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pMQ : Pointer of specified message queue
 * @Return : Number of total flushed message
 */
EXTERN U8 OSMQ_Flush(
    IN      OSMQ_PTR mq_ptr);

/**
 * OSMQ_MCU_DSP_Put
 *
 * Enqueue to specified message queue (dedicate for common queue between DSP
 * and MCU)
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @ptr : Pointer of message
 */
#if (FEA_SUPP_DSP)
EXTERN VOID OSMQ_MCU_DSP_Put (
    IN      OSMQ_PTR mq_ptr,
    IN      OS_DATA_PTR ptr);
#endif

/**
 * OSMQ_MCU_DSP_Get
 *
 * Dequeue from specified message queue(dedicate for common queue between DSP
 * and MCU)
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @Return : Pointer of message
 */
#if (FEA_SUPP_DSP)
EXTERN OS_DATA_PTR OSMQ_MCU_DSP_Get (
    IN      OSMQ_PTR mq_ptr);
#endif

/**
 * OSMQ_MCU_DSP_Entries
 *
 * Get number of memory blocks that have been queued in specified OSMQ
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @mq_ptr : Pointer of OSMQ
 * @return : Number of memory blocks.
 */
#if (FEA_SUPP_DSP)
EXTERN U8 OSMQ_MCU_DSP_Entries (
    IN      OSMQ_PTR mq_ptr);
#endif

/**
 * OS_malloc
 *
 * Allocates a block of size bytes of memory, returning a pointer to the0
 * beginning of the block.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Size : Size of the memory block, in bytes.
 */
EXTERN OS_DATA_PTR OS_malloc(
    IN      SIZE Size);



/**
 * OS_pmalloc
 *
 * Memory allocation (dedicate for TX buffer)
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Size : Number of bytes memory to allocate
 * @Return : Pointer of allocated memory
 */
OS_DATA_PTR OS_pmalloc(
    IN      U16 Size);

////////////////////////////////////////////////////////////////////////////////
// MEMORY FUNCTION PROTOTYPES //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_memset
 *
 * Sets the first num bytes of the block of memory pointed by ptr to the
 * specified value.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @ptr : Pointer to the block of memory to fill.
 * @value : Value to be set.
 * @num : Number of bytes to be set to the value.
 */
EXTERN VOID OS_memset(
    IN      OS_DATA_PTR ptr,
    IN      U8 value,
    IN      U16 num);

/**
 * OS_memcpy
 *
 * Copies the values of num bytes from the location pointed by source directly
 * to the memory block pointed by destination.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @des : Pointer to the destination array where the content is to be copied.
 * @src : Pointer to the source of data to be copied.
 * @num : Number of bytes to copy.
 */
EXTERN VOID OS_memcpy(
    IN      OS_DATA_PTR des,
    IN      OS_DATA_PTR src,
    IN      U16 num);

/**
 * OS_cmemcpy
 *
 * Copies the values of num bytes from the location pointed by source directly
 * to the memory block pointed by destination.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @des : Pointer to the destination array where the content is to be copied.
 * @src : Pointer to the source of data to be copied.
 * @num : Number of bytes to copy.
 */
EXTERN VOID OS_cmemcpy(
    IN      OS_DATA_PTR des,
    IN      OS_CODE_PTR src,
    IN      U16 num);

/**
 * OS_memcmp
 *
 * Compares the first num bytes of the block of memory pointed by ptr1 to the
 * first num bytes pointed by ptr2, returning zero if they all match or a value
 * different from zero representing which is greater if they do not.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @ptr1 : Pointer to block of memory.
 * @ptr2 : Pointer to block of memory.
 * @num : Number of bytes to compare.
 */
EXTERN U8 OS_memcmp(
    IN      OS_DATA_PTR ptr1,
    IN      OS_DATA_PTR ptr2,
    IN      U16 num);

/**
 * OSMEM_ViolationCheck
 *
 * Check OSMEM violation
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @ptr : Pointer to block of memory.
 * @return : TRUE stands for memory violation
 */
EXTERN BOOL OSMEM_ViolationCheck (
	IN		OS_DATA_PTR ptr );

#endif /* _OS_MEMORY_H_ */

