#ifndef _SECTOR_DSP_VP_NB_H_
#define _SECTOR_DSP_VP_NB_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dsp_func_para.h defines the function parameters of DSP.
 *
 * Programmer : Dingacheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "bt_config_profile.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U16 tmp;
	
	#if VP_NB_HQ_ENABLE
	S16 DspVpNbHq[3840];
	#endif
	
	#if VP_NB_LQ_ENABLE
	S16 DspVpNbLq[4352];
	#endif

} DSP_VP_NB_STRU;

typedef struct stru_sector_dsp_vp_nb
{
    DSP_VP_NB_STRU DspVpNb;

    CRC16 CRC;

} SECTOR_DSP_VP_NB_STRU;


////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_DSP_VP_NB_STRU CODE gSector_DspVpNb ALIGN(2);

#endif /* _SECTOR_DSP_VP_NB_H_ */
