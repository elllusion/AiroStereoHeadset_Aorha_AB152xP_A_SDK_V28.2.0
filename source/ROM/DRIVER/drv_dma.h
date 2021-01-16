#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_dma.h is the file defines the interface of DMA
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_DMA_memcpy
 *
 * Use DMA to do memory copy
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @des : Pointer of destination
 * @src : Pointer of source
 * @len : Length of copy transation
 */
EXTERN VOID DRV_DMA_memcpy(
    IN      OS_DATA_PTR des,
    IN      OS_DATA_PTR src,
    IN      SIZE len);

/**
 * DRV_DMA_WaitForReady
 *
 * Check whether DMA is busy
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : TURE stands for DMA is busy
 */
EXTERN U8 DRV_DMA_WaitForReady(VOID);

#endif /* _DRV_DMA_H_ */

