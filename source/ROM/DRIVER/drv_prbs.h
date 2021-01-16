#ifndef _DRV_PRBS_H_
#define _DRV_PRBS_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_prbs.h is the file defines the driver of PRBS access.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_PRBS_Enable
 *
 * Enable PRBS with specific seed. The PRBS seed will not be re-initialized if
 * the input seed is 0.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Seed : The PRBS seed
 */
EXTERN VOID DRV_PRBS_Enable(
    IN_OPT      U16 Seed);

/**
 * DRV_PRBS_Read
 *
 * Read PRBS
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : The PRBS value
 */
EXTERN U16 DRV_PRBS_Read(VOID);


/**
 * DRV_PRBS_Rd
 *
 * Continuous read from PRBS
 *
 * @Author : Gailong <Gailonghong@airoha.com.tw>
 * @pRdBuf : pointer of PRBS Rd Buffer
 * @Len : length of PRBS Rd Buffer(UNIT:U8)
 */
EXTERN VOID DRV_PRBS_Rd(
    IN  OS_DATA_PTR pRdBuf , 
    IN  SIZE Len);

#endif /* _DRV_PRBS_H_ */

