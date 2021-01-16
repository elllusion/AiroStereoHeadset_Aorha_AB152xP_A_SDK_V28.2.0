/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dsp_func_para.c defines the function parameters of DSP.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "sector_dsp_func_para.h"

#ifdef AB1520B
#pragma userclass (HCONST = CONFIG)
#else
#pragma userclass (HCONST = DSP_FUNCPARA)
#endif


#define ENDIAN_RVRS(A)	(((U16)((A) & 0x00FF) << 8) | ((U16)(A) >> 8))



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_DSP_FUNC_PARA_INIT_STRU CODE gSector_DspFuncParameter ALIGN(2)=
{
    {
        {
            #ifdef AB1520S
            #ifdef BLUETOOTH_SPEAKER
    			#include "..\DSP\AB1520S_DSP\DSP_PARA_AB1520S_MUSICBOX_TARGET.txt"
    		#else
    			#include "..\DSP\AB1520S_DSP\DSP_PARA_AB1520S_HEADSET_TARGET.txt"
    		#endif
            #else
    		#ifdef BLUETOOTH_SPEAKER
    			#include "..\DSP\AB1520_DSP\DSP_PARA_AB1520_MUSICBOX_TARGET.txt"
    		#else
    			#include "..\DSP\AB1520_DSP\DSP_PARA_AB1520_HEADSET_TARGET.txt"
    		#endif
            #endif
        },

        {
            ENDIAN_RVRS(60),	        // SNDR_THRESHOLD;
            ENDIAN_RVRS(50),	        // SNR_THRESHOLD;
        },

        {
            ENDIAN_RVRS(0x4000),	 	//- aud_fade_out_target_ratio   (original:VP_RATIO)
                                        //- Mute : 0x7FFF (Max volume:0x0000)
            ENDIAN_RVRS(70),	        // DSPGainLv_Default;
            ENDIAN_RVRS(70),	        // VPGainLv_Default;
            ENDIAN_RVRS(0),		        // MinAnalogGain
        },

        {
            ENDIAN_RVRS(1),             //PLC_ENABLE
            ENDIAN_RVRS(14),            //PLC_OFFSET
            ENDIAN_RVRS(28),            //PLC_OFFSET_mSBC
            ENDIAN_RVRS(1),             //PLC_FramePartEn
            ENDIAN_RVRS(0),             //PLC_FramePartMethod
            ENDIAN_RVRS(0),             //PLC_CRCChkMethod, 0 = by HW, 1 = by SW
        },

        {
            ENDIAN_RVRS(0),             // woofer_feature_func_sel 1:ENABLE_WOOFER_PEQ, 2:ENABLE_WOOFER_DRC, 3:ENABLE_WOOFER_PEQ&ENABLE_WOOFER_DRC
        },
        {
            0, 					        //I2S_DebugMask[0], offset = 0 // remember!!! set U8 I2SDumpEnable:1; in sector_driver_ctl_type.c
            0,						    //I2S_DebugMask[1], offset = 16// remember!!! set U8 I2SDumpEnable:1; in sector_driver_ctl_type.c
            0,						    //I2S_DebugMask[2], offset = 32// remember!!! set U8 I2SDumpEnable:1; in sector_driver_ctl_type.c
            0,						    //I2S_DebugMask[3], offset = 48// remember!!! set U8 I2SDumpEnable:1; in sector_driver_ctl_type.c
        },

        {
            ENDIAN_RVRS(32),		//- SBC_DA_buffer_size
        	ENDIAN_RVRS(0),         //- soundbar_delay_out
        	ENDIAN_RVRS(0x0080),	//- aud_fade_rate  (fade in/out per 128bits)
        	                        //- MAX value: 0x7FFF (off:0x0000)
        	ENDIAN_RVRS(1),     	//- aud_fade_enable  Enable the Fade in/out when vp/rt/audio coexist
        	ENDIAN_RVRS(0),		    //- Audio_Recovery_Smoother
        	ENDIAN_RVRS(38),	    //- AiroLineIn_Induce_Aux_Sample_Cnt
        	ENDIAN_RVRS(0),	        //- DSP_Reserved_Para_2
        	#ifdef AB1520D
            ENDIAN_RVRS(1),		    //- DSP_ConfigForD
        	#else
        	ENDIAN_RVRS(0),		    //- DSP_ConfigForD
        	#endif
        },

            // new parameter
            ENDIAN_RVRS(BITFORM_I2S_MODE),      //I2S_BIT_FORMAT
            ENDIAN_RVRS(I2S_32BIT_FORMAT),    	//I2S_BCLK_RATE

        { // voice parameter
            ENDIAN_RVRS(0),    	            //- Mic_LR_Switch;
	        ENDIAN_RVRS(0),                 //- SideTone_Enable;
            ENDIAN_RVRS(0),            		//- SideTone_Gain;
        },

        { // air_dump_parameter
            ENDIAN_RVRS(0),    	            //- DumpEnable;
	        ENDIAN_RVRS(0),                 //- DefaultPathMaskBit;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[0].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[0].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[1].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[1].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[2].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[2].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[3].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[3].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[4].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[4].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[5].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[5].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[6].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[6].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[7].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[7].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[8].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[8].Length;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[9].DspAddr;
            ENDIAN_RVRS(0),            		//- UserDefinedPath[9].Length;
        },

        { // AT howling parameter
            ENDIAN_RVRS(1000), 	            //- Howling_Level_UpBND;
	        ENDIAN_RVRS(1000),               //- Howling_Level_LowBND;
            ENDIAN_RVRS(3),          		//- Howling_Debounce_FRAME_NUM;
        },
    },

    //- CRC
    {0,0}
};
