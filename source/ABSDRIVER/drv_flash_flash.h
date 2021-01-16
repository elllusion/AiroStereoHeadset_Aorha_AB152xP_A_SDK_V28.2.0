#ifndef _FLASH_DRV_FLASH_H_
#define _FLASH_DRV_FLASH_H_
//////////////////////////////////////////////////////////
// Flash Function, Structure and Define in flash
//
// Added by Dinga, 2014.02.06
// For identify the manufacturer of the serial flash
//////////////////////////////////////////////////////////
#include "cpu.h"


////////////////////////////////////////////////////////////////////////////////
// Definition //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define MXIC_FLASH_CFG_REG_4IO()			\
	OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0xBF);	\
    OS_EXIT_CRITICAL()

#define GD_FLASH_CFG_REG_4IO()				\
    OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0x97);	\
    OS_EXIT_CRITICAL()

#define MXIC_FLASH_CFG_REG_2IO()			\
    OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0xBD);	\
    OS_EXIT_CRITICAL()

#define GD_FLASH_CFG_REG_2IO()				\
    OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0x95);	\
    OS_EXIT_CRITICAL()

#define MD_FLASH_CFG_REG_2IO()				\
    OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0x9D);	\
    OS_EXIT_CRITICAL()
	
#define EN_FLASH_CFG_REG_4IO()			\
	OS_ENTER_CRITICAL();					\
    SFR_WRITE_BY_E2(E2OFFS_RW_CFG, 0x9F);	\
    OS_EXIT_CRITICAL()	

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * Flash_Init
 *
 * Initial flash.
 *
 * @Author : DingaCheng <CharlesSu@airoha.com.tw>
 */
EXTERN VOID Flash_Init(VOID);

#endif /* _FLASH_DRV_FLASH_H_ */


