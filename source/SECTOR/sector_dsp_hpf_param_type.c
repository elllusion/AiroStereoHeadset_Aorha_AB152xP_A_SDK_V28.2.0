/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dsp_hpf_param_type.c defines the HPF parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2882
 */
#include "sector_peq_hpf_param.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef DSP_ENABLE

SECTOR_DSP_HPF_PARAMETER_STRU CODE gSector_DspHpfParameter =
{
    {
    	#ifdef OPERATION_IN_BB_FPGA_VERSION
    	0,
    	#else
    	(ENABLE_DEFAULT_A2DP_HPF_CONFIG || ENABLE_DEFAULT_LINEIN_HPF_CONFIG),	//- hpf_enable
    	#endif
		#if ENABLE_DEFAULT_A2DP_HPF_CONFIG
    	{
    		//- set 1
    		{
    			{
    				//- 32k sample rate
    				{1,32169,-15810,-16384,8192,3,2,2,32181},
    				//- 44.1k sample rate
    				{1,32336,-15965,-16384,8192,3,2,2,32343},
    				//- 48k sample rate
    				{1,32372,-15999,-16384,8192,3,2,2,32377}
    			}
    		},

    		//- set 2
    		{
    			{
    				//- 32k sample rate
    				{1,32279,-15909,-16384,8192,3,2,2,32286},
    				//- 44.1k sample rate
    				{1,32414,-16038,-16384,8192,3,2,2,32418},
    				//- 48k sample rate
    				{1,32443,-16065,-16384,8192,3,2,2,32446}
    			}
    		},

    		//- set 3
    		{
    			{
    				//- 32k sample rate
    				{1,32443,-16065,-16384,8192,3,2,2,32446},
    				//- 44.1k sample rate
    				{1,32533,-16152,-16384,8192,3,2,2,32535},
    				//- 48k sample rate
    				{1,32552,-16171,-16384,8192,3,2,2,32554}
    			}
    		},

    		//- set 4
    		{
    			{
    				//- 32k sample rate
    				{1,32496,-16118,-16384,8192,3,2,2,32499},
    				//- 44.1k sample rate
    				{1,32571,-16191,-16384,8192,3,2,2,32573},
    				//- 48k sample rate
    				{1,32587,-16206,-16384,8192,3,2,2,32589}
    			}
    		},

    		//- set 5
    		{
    			{
    				//- 32k sample rate
    				{1,32523,-16145,-16384,8192,3,2,2,32526},
    				//- 44.1k sample rate
    				{1,32591,-16210,-16384,8192,3,2,2,32593},
    				//- 48k sample rate
    				{1,32606,-16224,-16384,8192,3,2,2,32607}
    			}
    		}
    	},
		#endif
		#if ENABLE_DEFAULT_LINEIN_HPF_CONFIG
    	{
    			//- set 1
    		{
    			{
    				//- 32k sample rate
    				{1,32169,-15810,-16384,8192,3,2,2,32181},
    				//- 44.1k sample rate
    				{1,32336,-15965,-16384,8192,3,2,2,32343},
    				//- 48k sample rate
    				{1,32372,-15999,-16384,8192,3,2,2,32377}
    			}
    		},

    		//- set 2
    		{
    			{
    				//- 32k sample rate
    				{1,32279,-15909,-16384,8192,3,2,2,32286},
    				//- 44.1k sample rate
    				{1,32414,-16038,-16384,8192,3,2,2,32418},
    				//- 48k sample rate
    				{1,32443,-16065,-16384,8192,3,2,2,32446}
    			}
    		},

    		//- set 3
    		{
    			{
    				//- 32k sample rate
    				{1,32443,-16065,-16384,8192,3,2,2,32446},
    				//- 44.1k sample rate
    				{1,32533,-16152,-16384,8192,3,2,2,32535},
    				//- 48k sample rate
    				{1,32552,-16171,-16384,8192,3,2,2,32554}
    			}
    		},

    		//- set 4
    		{
    			{
    				//- 32k sample rate
    				{1,32496,-16118,-16384,8192,3,2,2,32499},
    				//- 44.1k sample rate
    				{1,32571,-16191,-16384,8192,3,2,2,32573},
    				//- 48k sample rate
    				{1,32587,-16206,-16384,8192,3,2,2,32589}
    			}
    		},

    		//- set 5
    		{
    			{
    				//- 32k sample rate
    				{1,32523,-16145,-16384,8192,3,2,2,32526},
    				//- 44.1k sample rate
    				{1,32591,-16210,-16384,8192,3,2,2,32593},
    				//- 48k sample rate
    				{1,32606,-16224,-16384,8192,3,2,2,32607}
    			}
    		}
    	}
		#endif
    },

	//- CRC
    {0,0}
};

#endif
