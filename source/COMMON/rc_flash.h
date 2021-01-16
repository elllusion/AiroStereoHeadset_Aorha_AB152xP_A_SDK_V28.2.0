#ifndef _RC_FLASH_H_
#define _RC_FLASH_H_

#include "os.h"
#include "bt_config_profile.h"
#include "ctlr_config.h"


typedef struct
{
    U8 len;
    U8 name[RC_LOCAL_NAME_LENGTH];
} BD_NAME;


#define TASKENTRY_KERNEL	TASKDEFINE( Kernel )
#define TASKENTRY_LM		TASKDEFINE( LM )
#define TASKENTRY_HC		TASKDEFINE( HC )
#define TASKENTRY_MMI		TASKDEFINE( MMI )
#define TASKENTRY_DRIVER	TASKDEFINE( DRIVER )

#if LE_SUPPORTED_CONTROLLER
#define TASKENTRY_LL		TASKDEFINE( LL )
#else
#define TASKENTRY_LL
#endif

#ifdef LE_SUPPORTED_HOST_FLASH
#define TASKENTRY_ATT		TASKDEFINE( ATT )
#define TASKENTRY_MMI_LE	TASKDEFINE( MMI_LE )
#else
#define TASKENTRY_ATT
#define TASKENTRY_MMI_LE
#endif

#if (defined SM_SUPPORTED && defined LE_SUPPORTED_HOST_FLASH)
#define TASKENTRY_SM		TASKDEFINE( SM )
#else
#define TASKENTRY_SM
#endif

#ifdef SUPPORT_LE_XPORT
#define TASKENTRY_LE_XPORT  TASKDEFINE( LE_XPORT )
#else
#define TASKENTRY_LE_XPORT
#endif

#ifdef SUPOORT_SYS_CLK_CHANGE
#define TASKENTRY_SYS_CLK  TASKDEFINE( SYS_CLK )
#else
#define TASKENTRY_SYS_CLK
#endif

#define	TASKENTRY_LIST		\
		TASKENTRY_KERNEL	\
		TASKENTRY_LL		\
		TASKENTRY_LM		\
		TASKENTRY_HC		\
		TASKENTRY_MMI		\
		TASKENTRY_MMI_LE	\
        TASKENTRY_LE_XPORT  \
		TASKENTRY_DRIVER	\
		TASKENTRY_ATT		\
		TASKENTRY_SM		\
        TASKENTRY_SYS_CLK   \

#define TASKDEFINE(name)	name##_TASK,

enum _TaskEnum
{
	DUMMY_FIRST_TASK, /* To ensure first task id started from 1 */

	TASKENTRY_LIST

	DUMMY_LAST_TASK
};

#define OS_N_TASK	(DUMMY_LAST_TASK - DUMMY_FIRST_TASK - 1)

#define SYS_SetPollingRoundsBeforeSleep() (OS_N_TASK * 2)

//////////////////////////////////////////////////////////
//  BEGIN: SYSTEM MUST Define Constant Area!!!
//////////////////////////////////////////////////////////

#ifdef __C251__
	#define CFG_MIN_CODE_ADDR       0x800000UL
#else
	#define CFG_MIN_CODE_ADDR       0x020000UL
#endif

#ifdef __C251__
    #ifdef FLASH_BOOT
    #define CPU_CODE_BASE_ADDR      0x020000UL
    #else
    #define CPU_CODE_BASE_ADDR      0x000000UL
    #endif
#else
    #define CPU_CODE_BASE_ADDR      0x810000UL
#endif

#define FLASH_MEMORY_MAP(addr)   ((U32)(addr) + CPU_CODE_BASE_ADDR - CFG_MIN_CODE_ADDR)

//////////////////////////////////////////////////////////
//  END: SYSTEM MUST Define Constant Area!!!
//////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////
//  Version Number (2010.7.30 updated)
//////////////////////////////////////////////////////////

#define AB1000B_VERSION                     0x02
#define AB1000C_VERSION                     0x03
#define AB1000D_VERSION                     0x04
#define AB1000E_VERSION                     0x05
#define AB1000F_VERSION                     0x06
#define AB1000G_VERSION                     0x07
#define AB1000H_VERSION                     0x08
#define AB1000I_VERSION                     0x09



typedef enum
{
    SYS_UNBLOCKING_IDLE,
    SYS_UNBLOCKING_REQUEST_ON,
    SYS_UNBLOCKING_TURNED_ON,//for interrupt level
    SYS_UNBLOCKING_WAIT_TO_IDLE,
} SYS_UNBLOCKING_STATE_TYPE;


EXTERN SYS_UNBLOCKING_STATE_TYPE     XDATA   gSYS_unblocking_state_for_I2C_and_SPI;
EXTERN BIT				gSYS_I2C_SPI_occupied;
EXTERN BIT				gSYS_init_led_exist;
EXTERN BIT				gSYS_allow_updating;
EXTERN BIT				gSYS_update_by_uart_flash_command;
EXTERN BIT				gSYS_init_charger_exist;
EXTERN BIT				gSYS_init_wakeupkey_exist;


EXTERN U8              XDATA  gSYS_WatchDogTimer;


PUBLIC VOID SYS_Flash_Init (VOID);
PUBLIC VOID SYS_WriteEIRResponseName(U8 XDATA_PTR namePtr, U8 length);

PUBLIC VOID SYS_InsertOSTask(IN U8 TaskId);


#endif

