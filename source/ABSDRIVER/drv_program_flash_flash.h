#ifndef _DRV_PROGRAM_FLASH_FLASH_H_
#define _DRV_PROGRAM_FLASH_FLASH_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_pmu.h is the file defines the interface of PMU register access
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "drv_flash.h"




////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN FLASH_ERRCODE FLASH_PROGRAM_UnlockSector(FLASH_ADDR flash_addr);
EXTERN FLASH_ERRCODE FLASH_PROGRAM_Erase4kByte(U8 RuntimeSectorID);
EXTERN FLASH_ERRCODE FLASH_PROGRAM_ByteProgramToSector(
	U8 RuntimeSectorID,
	U16 offset,
	IN U8 XDATA_PTR src_addr,
	IN U16 length);
EXTERN FLASH_ERRCODE FLASH_PROGRAM_PageProgramConti(
    U32 FlashAddr,
    IN  U8 XDATA_PTR src_addr,
    IN  U16 length);
EXTERN VOID FLASH_PROGRAM_PageReadConti(
    U32 FlashAddr,
    IN  U8 XDATA_PTR des_addr,
    IN  U16 length);
	
EXTERN BOOL FLASH_PROGRAM_WriteXDataToFlash(
	U8 CODE_PTR flashAddr, 
	U8 XDATA_PTR dataPtr, 
	U16 length);

#endif /* _DRV_PROGRAM_FLASH_FLASH_H_ */


