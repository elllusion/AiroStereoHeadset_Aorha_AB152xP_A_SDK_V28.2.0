#ifndef _ALIGN_FLASH_H_
#define _ALIGN_FLASH_H_

#ifndef __C251__
#include "os_mq_format.h"
#endif
#include "..\DSP\DSP_interface.h"

#include "os.h"

#include "..\Driver\Audio_Mailbox_structure.h"


typedef struct
{
    //- DSP Paramater Address in FLASH
    U32 DSP_FUNC_PARA_START_ADDR;
    U32 MCU_DSP_PARA_START_ADDR;

    //- DSP Parameter
    DSP_NVRAM_CTL_TYPE DSP_nvram;

    U16 DSPOUT_MUTE;

    U16 I2S_DEBUG_ENABLE;

    U16 dsp_param_update_ctl;

    U16 DSP_RESERVED_PARA_1;
	U16 DSP_RESERVED_PARA_2;
	U16 DSP_RESERVED_PARA_3;

    //- PEQ
    U16 DSP_CHANGE_PEQ;
    U16 XDATA_PTR PTR_TO_XDATA_PEQ;
    //- Compander
    U16 IC_BIST_IN_CHNL_SEL;

    U16 DEC_A2DP_START_ANCHOR;

    U16 DSP_A2DP_AUDIO_CHANNEL_SEL;
    U16 LOCAL_TIMESTAMP_CLK;
    U16 REMOTE_TIMESTAMP_CLK;
    U16 SYNC_TUNING_VALUE;
    U16 SYNC_TUNING_TIME;
    U16 TWS_FRAME_COUNTER;
    U16 TWS_LOCAL_FRAME_SC;
    U16 TWS_LOCAL_FRAMECNT;
    U16 TWS_REMOTE_FRAME_SC;
    U16 TWS_REMOTE_FRAMECNT;

    //- DSP Debug Reserved Variables
    U16 debug_var01;
    U16 debug_var02;
    U16 debug_var03;
    U16 debug_var_trap;

     //- DSP UART Interaction Buffer
    #ifdef DSP_UART_ENABLE
    S16 dsp_txrx[184];
    #endif

	#ifdef LIGHTING_ENABLE
	U16 LED_reserved;
	U16 LED_vol_idx;
	U16 LED_band_idx;
	#else
    U16 LED_PARMETER_R;
    U16 LED_PARMETER_G;
    U16 LED_PARMETER_B;
	#endif

    U16 AUDIO_ENHANCE_MODE;

    U16 IC_BIST_MODE;
    U16 IC_BIST_SNDR_RESULT;
    U16 IC_BIST_SNR_RESULT;
    U16 IC_BIST_SNDR_VAL;
    U16 IC_BIST_SIGNSE_POWER;
    U16 IC_BIST_MEMORY_VERIFICATION;


    OSMQ OSMQ_DSP_L2CAP_Received_PDU;
    OSMQ OSMQ_DSP_LineIn_Encoded_Payload;
    OSMQ OSMQ_DSP_MONO_2MIC_WriteFlash;

    MAILBOX_CTL_STRU Mailbox;

    OSMQ OSMQ_DSP_AirDataOut;
    OSMQ OSMQ_DSP_DebugDataOut;
    OSMQ OSMQ_DSP_eSCORelay;

}ALIGN_DSP_STRU;

EXTERN ALIGN_DSP_STRU XDATA gAlignDSP ALIGN(2);

#define OSMQ_DSP_L2CAP_Received_PDU_ptr		&gAlignDSP.OSMQ_DSP_L2CAP_Received_PDU
#define OSMQ_DSP_LineIn_Encoded_Payload_ptr	&gAlignDSP.OSMQ_DSP_LineIn_Encoded_Payload
#define OSMQ_DSP_MONO_2MIC_WriteFlash		&gAlignDSP.OSMQ_DSP_MONO_2MIC_WriteFlash
#define OSMQ_DSP_AirDataOut_ptr		        &gAlignDSP.OSMQ_DSP_AirDataOut
#define OSMQ_DSP_DebugDataOut_ptr           &gAlignDSP.OSMQ_DSP_DebugDataOut
#define OSMQ_DSP_eSCORelay_ptr              &gAlignDSP.OSMQ_DSP_eSCORelay

#endif
