#ifndef _OS_INTR_EXT1_PMU_HAL_H_
#define _OS_INTR_EXT1_PMU_HAL_H_
#ifndef _OS_INTR_EXT1_PMU_C_
#error "This file shall only be included in os_intr_ext1_pmu.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * os_intr_ext1_pmu_hal.c is the file defines the hardware abstraction layer of
 * os_intr_ext1_pmu.c.
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
#error "Unsuitable HAL"
#endif



////////////////////////////////////////////////////////////////////////////////
// Global Variable /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN OS_EXT1_PMU_INTR_ID PmuIntrId;



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define OS_EXT1_PMU_INTR_GetIntrId(i) { do {                                    \
    (i) = PmuIntrId;                                                            \
    } while (0); }
#define OS_EXT1_PMU_INTR_SetMask(i) { do {                                      \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_INTF_INT_MASK,LC_SFR_READ_BY_FE(FEOFFS_RW_INTF_INT_MASK) | BIT_MASK8(6)); \
    } while (0); }

#endif /* _OS_INTR_EXT1_PMU_HAL_H_ */

