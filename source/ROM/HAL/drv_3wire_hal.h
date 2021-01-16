#ifndef _DRV_3WIRE_HAL_H_
#define _DRV_3WIRE_HAL_H_
#ifndef _DRV_3WIRE_C_
#error "This file shall only be included in drv_3wire.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * dma_3wire_hal.c is the file defines the hardware abstraction layer of
 * dma_3wire.c.
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
// Method //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_3WIRE_EXTRACT_BUSY(A)       ((A) & 0x01)
#define DRV_3WIRE_WRITE_COMMAND         (0 << 1)
#define DRV_3WIRE_READ_COMMAND          (1 << 1)



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DRV_3WIRE_SetWriteCmd()     { do { LC_SFR_WRITE_BY_FE(FEOFFS_RO_RF_WIRE3_BUSY,DRV_3WIRE_WRITE_COMMAND; } while (0); }
#define DRV_3WIRE_SetReadCmd()      { do { LC_SFR_WRITE_BY_FE(FEOFFS_RO_RF_WIRE3_BUSY,DRV_3WIRE_READ_COMMAND); } while (0); }
#define DRV_3WIRE_SetData(D)        { do {                                      \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_DIR_SEND_RF_DATA_B0,BYTE0(D))                  \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_DIR_SEND_RF_DATA_B1,BYTE1(D))                  \
    } while (0); }
#define DRV_3WIRE_SetAddr(A)        { do { LC_SFR_WRITE_BY_FE(FEOFFS_RW_DIR_SEND_RF_DATA_B2,(A)); } while (0); }
#define DRV_3WIRE_IsBusy()          ( EXTRACT_3WIRE_BUSY(LC_SFR_READ_BY_FE(FEOFFS_RO_RF_WIRE3_BUSY))) )
#define DRV_3WIRE_GetData()         (                                           \
    while (DRV_3WIRE_IsBusy());                                                 \
    (LC_SFR_READ_BY_FE(FEOFFS_RW_DIR_SEND_RF_DATA_B0) | ((U16)LC_SFR_READ_BY_FE(FEOFFS_RW_DIR_SEND_RF_DATA_B1) << 8)) \
    )

#endif /* _DRV_3WIRE_HAL_H_ */

