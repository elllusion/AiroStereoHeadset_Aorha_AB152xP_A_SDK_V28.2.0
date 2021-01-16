#ifndef _OS_H_
#define _OS_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * os.h is the header file of operation system.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "hw.h"
#include "sector_sys_memory_config.h"

#include "os_types.h"
#include "os_list.h"
#include "os_dpc.h"
#include "os_intr.h"
#include "os_intr_ext1.h"
#include "os_heap.h"
#include "os_timer.h"
#include "os_memory.h"
#include "os_task.h"
#include "os_memtop.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef void (*VFUN)(void);

//PowerSavingLevel :
#define OS_POWERLEVEL_SLEEP_MCU_PAUSE   1
#define OS_POWERLEVEL_MCU_PAUSE         0
// 0: CPU frozen only
// 1: CPU frozen and Sleep are both allowed;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U8 DATA gOS_NestCriticalLv;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef OS_PRINT_ASSERT
EXTERN VOID SYS_AssertUARTEventGenerator(U8 condition, U16 line);
#define ASSERT(x) SYS_AssertUARTEventGenerator((x), (U16)__LINE__)
#endif

/** INLINE FUNCTION *
 * OS_TRAP - trap CPU
 *
 * Trap CPU directly
 */
#ifdef OS_PRINT_ASSERT
	#define OS_TRAP() ASSERT(FALSE)
#else
	#if defined(OS_TRAP_DBG)
	#define OS_TRAP() { do { while (1); } while (0); }
	#else
	#define OS_TRAP() { do { ((VOID)0); } while (0); }
	#endif
#endif

/** INLINE FUNCTION *
 * OS_ASSERT - trap CPU if the experision is false
 *
 * Trap CPU if the expression is false
 */
#ifdef OS_PRINT_ASSERT
	#define OS_ASSERT(expr) ASSERT(expr)
#else
	#if defined(OS_ASSERT_DBG)
	#define OS_ASSERT(expr) { do { if (!(expr)) { OS_TRAP(); } } while (0); }
	#else
	#define OS_ASSERT(expr) { do { ((VOID)0); } while (0); }
	#endif
#endif

/** INLINE FUNCTION *
 * OS_ENTER_CRITICAL - enter critical section
 *
 * Force CPU enter critical section.
 */
#define OS_ENTER_CRITICAL() { do {                                              \
    EA = FALSE;                                                                 \
    EA = FALSE;                                                                 \
    EA = FALSE;                                                                 \
    EA = FALSE;                                                                 \
    _nop_();                                                                    \
    _nop_();                                                                    \
    _nop_();                                                                    \
	_nop_();                                                                    \
    _nop_();                                                                    \
    _nop_();                                                                    \
    _nop_();                                                                    \
	_nop_();                                                                    \
    _nop_();                                                                    \
	_nop_();                                                                    \
    gOS_NestCriticalLv++;                                                       \
    } while(0); }

/** INLINE FUNCTION *
 * OS_EXIT_CRITICAL - exit critical section
 *
 * CPU exit critical section.
 */
#define OS_EXIT_CRITICAL() { do {                                               \
    gOS_NestCriticalLv--;                                                       \
    if (!gOS_NestCriticalLv) { EA = TRUE; }                                     \
    } while(0); }

#if (FEA_SUPP_DSP)
/** INLINE FUNCTION **
 * OS_SPIN_LOCK - invoke spin lock
 *
 * Invoke the spin lock and use CPU resource to wait semaphore.
 */
#define OS_SPIN_LOCK() { do { while (SFR_WR_TEST_AND_SET); } while(0); }

/** INLINE FUNCTION **
 * OS_SPIN_UNLOCK - release semaphore
 *
 * Release semaphore.
 */
#define OS_SPIN_UNLOCK() { do { SFR_WR_TEST_AND_SET = FALSE; } while(0); }

/** INLINE FUNCTION **
 * OS_ENTER_MUTEX_CRITICAL - enter critical section between 2 CPUs
 *
 * This macro invoke CPU spin lock to protect memory access
 */
#define OS_ENTER_MUTEX_CRITICAL() { do { OS_ENTER_CRITICAL(); OS_SPIN_LOCK(); } while(0); }

/** INLINE FUNCTION **
 * OS_ENTER_MUTEX_CRITICAL - exit critical section
 *
 * Exit critical section
 */
#define OS_EXIT_MUTEX_CRITICAL() { do { OS_SPIN_UNLOCK(); OS_EXIT_CRITICAL(); } while(0); }
#endif



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_AlignToNext4ByteAddr
 *
 * Get the next 4-byte aligned XDATA pointer
 *
 * Author : Charels Su <CharlesSu@airoha.com.tw>
 *
 * @ptr : The pointer of data memory
 * @Return : Next 4-byte aligned pointer
 */
EXTERN OS_DATA_PTR OS_AlignToNext4ByteAddr (OS_DATA_PTR ptr);

/**
 * OS_SoftwareReset
 *
 * Software Reset
 *
 * Author : Charels Su <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_SoftwareReset(VOID);


/**
 * In computer science, a null function (or null operator) is subroutine that
 * returns no data values and leaves the program state unchanged. When it is
 * part of the instruction set of a processor, it is called a NOP or NOOP
 * (No OPeration).
 *
 * @author Charels Su <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_NullFunc(VOID);


//--------------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
// OLD FUNCTION CALL WRAPPERS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U8 DATA   gOS_NestCriticalLv;

EXTERN BIT gOS_HC_LM_Task_Only_For_RF_Test_Mode;

EXTERN BIT           gOS_PowerSavingEnable;
EXTERN BIT           gOS_PowerSavingRequest;
EXTERN BIT           gOS_PowerSavingForceMCUPauseOnly;
EXTERN U8    XDATA   gOS_PowerSavingLevel;
EXTERN BIT           gOS_PowerSavingMCUPauseRequest;

EXTERN U8 XDATA gDEBUG_ScanEnterCount;
EXTERN U8 XDATA gDEBUG_MMI1EnterCount;
EXTERN U8 XDATA gDEBUG_KEYEnterCount;
EXTERN U32 XDATA powerSavingDeviceMask;



////////////////////////////////////////////////////////////////////////////////
// OLD FUNCTION CALL WRAPPERS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define OSMQ_FlushAndReleaseMemoryBlock OSMQ_Flush
#define OSMEM_memset_xdata OS_memset

#define OSMEM_memcpy_xdata_xdata OS_memcpy
#define OSMEM_memcpy_xdata_code OS_cmemcpy
#define OSMEM_memcpy_xdata_generic OS_cmemcpy

EXTERN PUBLIC void OS_SpeedyJump(U32 ptr);
EXTERN PUBLIC void OS_EnableOSPowerSaving(void);
EXTERN PUBLIC void OS_DisableOSPowerSaving(void);
#define OS_CALL(ptr,offset) OS_SpeedyJump((U32)((U8 CODE_PTR)ptr + (offset * sizeof(U8 CODE_PTR))))
#define DEV_IS_ACTIVE(DEV_TYPE)   (powerSavingDeviceMask&BIT_MASK32(DEV_TYPE))
enum
{
    PWRSAV_RESERVED,
    PWRSAV_LM,
    PWRSAV_MMI,
    PWRSAV_PWRON,
    PWRSAV_UART,
    PWRSAV_BAT,
    PWRSAV_KEY,
    PWRSAV_LED,
    PWRSAV_FAKEON,
    PWRSAV_DSP,
    PWRSAV_DRIVER,
    PWRSAV_LL,
    PWRSAV_LE,
    PWRSAV_DEVICE_NO
    //Error Checking, PWRSAV_DEVICE_NO cannot larger than 32
}; //// SetPowerSavingForDevice
typedef U8  PowerSavingDevType;

EXTERN PUBLIC void SetPowerSavingForDevice(U8 enablePowerSaving, PowerSavingDevType device);


#endif /* _OS_H_ */

