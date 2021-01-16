#ifndef _AUDIO_MAILBOX_STARUCTURE_H_
#define _AUDIO_MAILBOX_STARUCTURE_H_

enum voice_in_scenario_sel
{
	VOICE_IN_DISABLE = 0,
	VOICE_IN_ANALOG_MIC,
	VOICE_IN_DIGITAL_MIC,
};

typedef union audio_in_out_scenario_union
{
    struct audio_in_out_scenario_struct
    {
        U8 Audio_In;
        U8 Audio_Out;
        U8 Voice_In;
        U8 Reserved;
    } Field;
    U32 Reg;
} AUDIO_IN_OUT_SCENARIO_STRU;

typedef struct
{
    U16 SOUNDBAR_ESCO_RELAY;
    U16 SOUNDBAR_ESCO_PACKET_LENGTH;
    U16 SOUNDBAR_ESCO_BANDWITH;
    U16 SOUNDBAR_DOWNSAMPLE_METHOD;
    U16 SOUNDBAR_EXTERNAL_MCU;
    U16 WOOFER_LPF_ENABLE;
    U16 PAYLOAD_ENCODE;
    U16 LPF_CUTOFF_FREQ_SEL;
    U16 PCM_FS_SEL;
} APP_PARAM_INTERFACE_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
	U8  vp_quality;
	U8	vp_resv;
	U16 vp_len;
	U16 XDATA_PTR vp_pattern_ptr;
	U32 dsp_vp_nb_ptr;
	U16 mute_frame_before_fade_in;
}VP_START_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
	U8 rt_no;
	U8 rt_resv;
	U16 XDATA_PTR rt_pattern_ptr;
	U16 CODE_PTR rt_pattern_ptr_code;
	U16 mute_frame_before_fade_in;
}RT_START_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
    U16 at_PEQ_En;
    U16 at_PEQ_Ptr;
} AT_START_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
    APP_PARAM_INTERFACE_STRU feature_sel;
	U8 sco_type;
    U8 pkt_type;
    U8 sco_interval;
    U8 codec_type;
    U16 pkt_length;
    U32 DualMic_ptr;
}MONO_PARAM_STRU;

typedef struct
{
	AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
	APP_PARAM_INTERFACE_STRU feature_sel;
	U16 codec_type;
	U16 tws_mode;
	U16 sidetone_en;
	U16 start_sync;
} STEREO_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
    APP_PARAM_INTERFACE_STRU feature_sel;
    U16 input_fs;
} LINEIN_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
    APP_PARAM_INTERFACE_STRU feature_sel;
} ASYNC_MONO_PARAM_STRU;

typedef struct
{
    AUDIO_IN_OUT_SCENARIO_STRU audio_scenario;
    U32 ena_map;
    U32 VC_Table_ptr;
	U32 adp_ena_map;
	U32 adp_VC_Table_ptr;
}VC_START_PARAM_STRU;

typedef union AUADC_ANA_GC_union
{
    struct AUADC_ANA_GC_stru
    {
        U16 AuInGainCtrl_L      : 8;
        U16 AuInGainCtrl_R      : 8;
    } Field;
    U16 Reg;
} AUADC_ANA_GC_t;

typedef union EAR_SDMOD_GC_union
{
    struct EAR_SDMOD_GC_stru
    {
        U16 SpkVolCtrl          : 8;
        U16 AuDAC_DSMGain       : 8;
    } Field;
    U16 Reg;
} EAR_SDMOD_GC_t;

typedef struct
{
    U16            GainTarget;
    U16            APP_GainLv;
    U16            VPRT_GainLv;
    AUADC_ANA_GC_t au_ad_ana_gain;
    EAR_SDMOD_GC_t ear_sdmod_gain;
    U16            MicDigitalGain;
	U16            DspClock;
	U16            DacEnable;
} DSP_GAIN_PARAM_STRU;

typedef struct
{
    U32 PEQ_ptr;
    U32 HPF_ptr;
    U8 PEQ_Enable;
    U8 HPF_Enable;
} PEQ_PARAM_STRU;

typedef struct
{
    U32 CPD_ptr;
} CPD_PARAM_STRU;

typedef struct
{
    U16 cmd_type;
	U16 cmd_para;
} DSP_UPDATE_PARAM_STRU;

typedef struct
{
	OSMQ XDATA_PTR mq_ptr;
	U8 cmd_status;
}CMD_RESPONSE_STRU;

typedef union
{
	VP_START_PARAM_STRU	    vp_start_para;
	RT_START_PARAM_STRU     rt_start_para;
	AT_START_PARAM_STRU     at_start_para;
	MONO_PARAM_STRU		    mono_para;
    STEREO_PARAM_STRU       stereo_start_para;
    LINEIN_PARAM_STRU       linein_start_para;
    ASYNC_MONO_PARAM_STRU   async_mono_para;
	VC_START_PARAM_STRU     vc_start_para;
	DSP_GAIN_PARAM_STRU     dsp_gain_para;
	PEQ_PARAM_STRU          peq_hpf_para;
	CPD_PARAM_STRU          cpd_para;
    DSP_UPDATE_PARAM_STRU   dsp_update_para;
}M2D_PARA_STRU;

typedef struct
{
	U8 Ogf;
    U8 Ocf;
} M2D_CMD_STRU;

typedef struct
{
	volatile U16  rw_word;
    U16 task;
	M2D_CMD_STRU  cmd;
	M2D_PARA_STRU para;
}M2D_CTL_STRU;

typedef struct
{
    U8 vc_result;
    U8 reserve;
}VC_EVT_PARA_STRU;

typedef struct
{
    U16 CH1_REF_GAIN;
    U16 CH2_REF_GAIN;
    U16 CH3_REF_GAIN;
    U16 CH4_REF_GAIN;
    U16 CH1_REF2_GAIN;
    U16 CH2_REF2_GAIN;
    U16 CH3_REF2_GAIN;
    U16 CH4_REF2_GAIN;
} REF_GAIN_STRUCT_t;

typedef union
{
    REF_GAIN_STRUCT_t ref_gain_para;
} DSP_REPORT_PARA_STRU;

typedef union
{
    VC_EVT_PARA_STRU vc_evt_para;
    DSP_REPORT_PARA_STRU dsp_report_para;
}EVT_PARA_STRU;

typedef struct d2m_evt_s
{
	U8 Evt;
	U8 Sta;
    EVT_PARA_STRU para;
	M2D_CMD_STRU Cmd;
} D2M_EVT_STRU;
typedef struct d2m_ctl_s
{
	volatile U16 rw_word;
	D2M_EVT_STRU evt_word;
}D2M_CTL_STRU;

typedef struct                //two mailbox structure include M2D & D2M
{
	M2D_CTL_STRU m2d_ctl;
	D2M_CTL_STRU d2m_ctl;
} MAILBOX_CTL_STRU;
#endif
