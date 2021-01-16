#ifndef _DRV_DMA_HAL_H_
#define _DRV_DMA_HAL_H_
#ifndef _DRV_DMA_C_
#error "This file shall only be included in drv_dma.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_dma_hal.c is the file defines the hardware abstraction layer of
 * drv_dma.c.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "hw.h"



////////////////////////////////////////////////////////////////////////////////
// Version Control /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !(CHIP_MODEL == 1520)
#error "Unsuitable Driver"
#endif



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_DMA_SetSrcAddr(A)   { do {                                          \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_SRC_ADDR_B0,BYTE0((ADDR16)(A)));        \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_SRC_ADDR_B1,BYTE1((ADDR16)(A)));        \
    } while (0); }
#define DRV_DMA_SetDesAddr(A)   { do {                                          \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_TARGET_ADDR_B0,BYTE0((ADDR16)(A)));     \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_TARGET_ADDR_B1,BYTE1((ADDR16)(A)));     \
    } while (0); }
#define DRV_DMA_SetSize(S)      { do {                                          \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_DATA_SIZE_B0, BYTE0((U16)(S)));         \
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MEMCPY_DATA_SIZE_B1, BYTE1((U16)(S)));         \
    } while (0); }
#define DRV_DMA_Start()         { do { SFRB_WO_MEMCPY_START = 1; } while (0); }
#define DRV_DMA_IsBusy()        ( SFRB_RO_MEMCPY_BUSY )

#endif /* _DRV_DMA_HAL_H_ */

