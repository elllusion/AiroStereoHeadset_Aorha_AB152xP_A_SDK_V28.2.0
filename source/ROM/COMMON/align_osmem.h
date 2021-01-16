#ifndef _ALIGN_OSMEM_H_
#define _ALIGN_OSMEM_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * align.h defines a common data structure placed from memory start
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#ifndef __C251__
#include "os_mq_format.h"
#endif



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 dummy[16];
    U16 XDATA_PTR pAlignDSP;

    S8 Soundbar_BT_Master_CLK_Control;
	U8 Woofer_Mute_Control;

    OSMEM OSMEM_call;
    OSMEM OSMEM_timer;
    OSMEM OSMEM1;

    OSMEM OSMEM2_small_TX;
    OSMEM OSMEM2_tiny_RX;
    OSMEM OSMEM2_small_RX;
    OSMEM OSMEM2_large_RX;
    OSMEM OSMEM2_large_TX;

} ALIGN_OSMEM_STRU;



////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN ALIGN_OSMEM_STRU		XDATA gAlignOSMEM;

#define OSMEM_call_ptr		&gAlignOSMEM.OSMEM_call
#define OSMEM_timer_ptr		&gAlignOSMEM.OSMEM_timer

#define OSMEM_ptr1			&gAlignOSMEM.OSMEM1
#define OSMEM_ptr2_small_TX	&gAlignOSMEM.OSMEM2_small_TX
#define OSMEM_ptr2_tiny_RX	&gAlignOSMEM.OSMEM2_tiny_RX
#define OSMEM_ptr2_small_RX	&gAlignOSMEM.OSMEM2_small_RX
#define OSMEM_ptr2_large_RX	&gAlignOSMEM.OSMEM2_large_RX
#define OSMEM_ptr2_large_TX	&gAlignOSMEM.OSMEM2_large_TX

#endif //_ALIGN_OSMEM_H_

