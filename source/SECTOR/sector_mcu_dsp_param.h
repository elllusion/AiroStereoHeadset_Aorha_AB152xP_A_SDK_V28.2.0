#ifndef _SECTOR_MCU_DSP_PARAM_H_
#define _SECTOR_MCU_DSP_PARAM_H_

#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_dsp_func_para.h"


#include "sector_driver_ctl_type.h"
enum
{
	AUDIOCTL_SCO= 0,
	AUDIOCTL_VP,
	AUDIOCTL_A2DP,
	AUDIOCTL_LINE_IN,
	NUM_OF_AUDIOCTL_SEL
};

typedef struct
{
	U16 dsp_cksel; 		  		                //- DSP main clock control
} MCU_RELATION;

typedef struct
{
	U16 sbc_target_buffer_depth;
	U16 dsp_headset_mode_sel;
} DSP_STEREO_RELATION;

typedef struct
{
    U16 LINEINOUT_SEL;
    U16 INS_ENABLE;
    U16 LINEIN_AUTODETECT_ENABLE;
} DSP_LINEIN_RELATION;



typedef struct
{
    U16 SRC32BitEnable;
    U16 StereoFunctionEnable;
    U16 Woofer_Delay_to_sync_BAR;
    U16 Feature_Sel;
} DSP_RSVD_RELATION;

typedef struct
{
    MCU_RELATION			mcu_parameter;
    DSP_STEREO_RELATION		dsp_stereo_parameter;
    DSP_LINEIN_RELATION     lineinout_parameter;
    DSP_RSVD_RELATION       DSP_Reserved_PARAM;

} DSP_NVRAM_CTL_TYPE;

typedef struct
{
	DSP_NVRAM_CTL_TYPE	dsp_nvram_ctl;

	CRC16 CRC;

} SECTOR_DSP_NVRAM_CTL_STRU;

#endif /* _SECTOR_MCU_DSP_PARAM_H_ */
