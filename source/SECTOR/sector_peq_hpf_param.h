#ifndef _SECTOR_PEQ_HPF_PARAM_H_
#define _SECTOR_PEQ_HPF_PARAM_H_

#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "bt_config_profile.h"

#define NUM_OF_MAX_PEQ_CONFIG						5
#define NUM_OF_DEFAULT_A2DP_PEQ_CONFIG				0
#define NUM_OF_DEFAULT_LINEIN_PEQ_CONFIG			0
#define ENABLE_DEFAULT_A2DP_PEQ_EXPANSION_CONFIG	0
#define ENABLE_DEFAULT_LINEIN_PEQ_EXPANSION_CONFIG	0
#define ENABLE_DEFAULT_A2DP_PEQ_100HZ_CONFIG		0
#define ENABLE_DEFAULT_LINEIN_PEQ_100HZ_CONFIG		0
#define NUM_OF_CUSTOM_A2DP_PEQ_CONFIG				0
#define NUM_OF_CUSTOM_LINEIN_PEQ_CONFIG				0
#define ENABLE_CUSTOM_A2DP_PEQ_EXPANSION_CONFIG		0
#define ENABLE_CUSTOM_LINEIN_PEQ_EXPANSION_CONFIG	0
#define ENABLE_CUSTOM_A2DP_PEQ_100HZ_CONFIG			0
#define ENABLE_CUSTOM_LINEIN_PEQ_100HZ_CONFIG		0
#define ENABLE_DEFAULT_A2DP_HPF_CONFIG				0
#define ENABLE_DEFAULT_LINEIN_HPF_CONFIG			0
#define ENABLE_CUSTOM_A2DP_HPF_CONFIG				0
#define ENABLE_CUSTOM_LINEIN_HPF_CONFIG				0
#define PEQ_ORDER_SIZE								37
#define PEQ_100HZ_ORDER_SIZE						15
#define HPF_ORDER_SIZE								9
#define NUM_OF_DEFAULT_AT_PEQ_CONFIG				0
#define	NUM_OF_MAX_AT_PEQ_CONFIG					1

enum
{
	PEQ_32k = 0,
	PEQ_44k,
	PEQ_48k,
	NUM_OF_PEQ_SAMPLE_RATE
};

typedef struct
{
	S16 hpf_sample_rate_content[HPF_ORDER_SIZE];
	
} HPF_SAMPLE_RATE_CONTENT;

typedef struct
{
	HPF_SAMPLE_RATE_CONTENT	hpf_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	
} HPF_CONFIG_SECTOR;

typedef struct 
{
	U8							hpf_enable;
	#if ENABLE_DEFAULT_A2DP_HPF_CONFIG
	HPF_CONFIG_SECTOR			a2dp_hpf_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	#if ENABLE_DEFAULT_LINEIN_HPF_CONFIG
	HPF_CONFIG_SECTOR			linein_hpf_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	
} DSP_HPF_PARAMETER_STRU;

typedef struct 
{
    DSP_HPF_PARAMETER_STRU DspHpfParameter;

    CRC16 CRC;

} SECTOR_DSP_HPF_PARAMETER_STRU;

typedef struct 
{
	U8							hpf_enable;
	#if ENABLE_CUSTOM_A2DP_HPF_CONFIG
	HPF_CONFIG_SECTOR			a2dp_hpf_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	#if ENABLE_CUSTOM_LINEIN_HPF_CONFIG
	HPF_CONFIG_SECTOR			linein_hpf_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	
} DSP_HPF_CUSTOM_PARAMETER_STRU;

typedef struct
{
	S16 peq_sample_rate_content[PEQ_ORDER_SIZE];
	
} PEQ_SAMPLE_RATE_CONTENT;

typedef struct
{
	S16 peq_100hz_sample_rate_content[PEQ_100HZ_ORDER_SIZE];
	
} PEQ_100HZ_SAMPLE_RATE_CONTENT;

typedef struct
{
	PEQ_SAMPLE_RATE_CONTENT	peq_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	
} PEQ_CONFIG_SECTOR;

typedef struct
{
	PEQ_100HZ_SAMPLE_RATE_CONTENT	peq_100hz_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
}PEQ_100HZ_SECTOR;

typedef struct
{
	U8 peq_100Hz_a2dp_set_enable;
#ifdef PEQ_UNDER_100HZ_ENABLE
	#if ENABLE_DEFAULT_A2DP_PEQ_100HZ_CONFIG
	PEQ_100HZ_SAMPLE_RATE_CONTENT	peq_100hz_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	#endif
#endif
} PEQ_100HZ_A2DP_CONFIG_SECTOR;

typedef struct
{
	U8 peq_100Hz_linein_set_enable;
#ifdef PEQ_UNDER_100HZ_ENABLE
	#if ENABLE_DEFAULT_LINEIN_PEQ_100HZ_CONFIG
	PEQ_100HZ_SAMPLE_RATE_CONTENT	peq_100hz_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	#endif
#endif
} PEQ_100HZ_LINEIN_CONFIG_SECTOR;

typedef struct
{
	U8 peq_100Hz_a2dp_set_enable;
#ifdef PEQ_UNDER_100HZ_ENABLE
	#if ENABLE_CUSTOM_A2DP_PEQ_100HZ_CONFIG
	PEQ_100HZ_SAMPLE_RATE_CONTENT	peq_100hz_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	#endif
#endif
} PEQ_100HZ_CUSTOM_A2DP_CONFIG_SECTOR;

typedef struct
{
	U8 peq_100Hz_linein_set_enable;
#ifdef PEQ_UNDER_100HZ_ENABLE
	#if ENABLE_CUSTOM_LINEIN_PEQ_100HZ_CONFIG
	PEQ_100HZ_SAMPLE_RATE_CONTENT	peq_100hz_sample_rate[NUM_OF_PEQ_SAMPLE_RATE];
	#endif
#endif
} PEQ_100HZ_CUSTOM_LINEIN_CONFIG_SECTOR;

typedef struct 
{
	U8								num_of_a2dp_peq_config;
	U8								num_of_linein_peq_config;
#if NUM_OF_DEFAULT_A2DP_PEQ_CONFIG
	PEQ_CONFIG_SECTOR				a2dp_peq_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
#if NUM_OF_DEFAULT_LINEIN_PEQ_CONFIG
	PEQ_CONFIG_SECTOR				linein_peq_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
	U8 								peq_expansion_enable;
#ifdef PEQ_EXPANSION_ENABLE
	#if ENABLE_DEFAULT_A2DP_PEQ_EXPANSION_CONFIG
	PEQ_CONFIG_SECTOR				a2dp_peq_expansion_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	#if ENABLE_DEFAULT_LINEIN_PEQ_EXPANSION_CONFIG
	PEQ_CONFIG_SECTOR				linein_peq_expansion_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
#endif
	U8								num_of_AudioTransparency_peq_config;
	#if NUM_OF_DEFAULT_AT_PEQ_CONFIG
	PEQ_CONFIG_SECTOR				audioTransparency_peq_config_sector[NUM_OF_MAX_AT_PEQ_CONFIG];
	#endif
#if NUM_OF_DEFAULT_A2DP_PEQ_CONFIG
	PEQ_100HZ_A2DP_CONFIG_SECTOR	a2dp_peq_100Hz_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
#if NUM_OF_DEFAULT_LINEIN_PEQ_CONFIG
	PEQ_100HZ_LINEIN_CONFIG_SECTOR	linein_peq_100Hz_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
} DSP_PEQ_PARAMETER_STRU;

typedef struct 
{
    DSP_PEQ_PARAMETER_STRU DspPeqParameter;

    CRC16 CRC;

} SECTOR_DSP_PEQ_PARAMETER_STRU;

typedef struct 
{
	U8							num_of_a2dp_peq_config;
	U8							num_of_linein_peq_config;
#if NUM_OF_CUSTOM_A2DP_PEQ_CONFIG
	PEQ_CONFIG_SECTOR			a2dp_peq_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
#if NUM_OF_CUSTOM_LINEIN_PEQ_CONFIG
	PEQ_CONFIG_SECTOR			linein_peq_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
	U8 							peq_expansion_enable;
#ifdef PEQ_EXPANSION_ENABLE
	#if ENABLE_CUSTOM_A2DP_PEQ_EXPANSION_CONFIG
	PEQ_CONFIG_SECTOR			a2dp_peq_expansion_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
	#if ENABLE_CUSTOM_LINEIN_PEQ_EXPANSION_CONFIG
	PEQ_CONFIG_SECTOR			linein_peq_expansion_config_sector[NUM_OF_MAX_PEQ_CONFIG];
	#endif
#endif
	U8							num_of_AudioTransparency_peq_config;
	#if NUM_OF_DEFAULT_AT_PEQ_CONFIG
	PEQ_CONFIG_SECTOR			audioTransparency_peq_config_sector[NUM_OF_MAX_AT_PEQ_CONFIG];
	#endif
#if NUM_OF_CUSTOM_A2DP_PEQ_CONFIG
	PEQ_100HZ_CUSTOM_A2DP_CONFIG_SECTOR		a2dp_peq_100Hz_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
#if NUM_OF_CUSTOM_LINEIN_PEQ_CONFIG
	PEQ_100HZ_CUSTOM_LINEIN_CONFIG_SECTOR	linein_peq_100Hz_config_sector[NUM_OF_MAX_PEQ_CONFIG];
#endif
} DSP_PEQ_CUSTOM_PARAMETER_STRU;

typedef struct
{
	DSP_PEQ_CUSTOM_PARAMETER_STRU DspPeqParameter;
	
	DSP_HPF_CUSTOM_PARAMETER_STRU DspHpfParameter;
	
} SECTOR_DSP_PEQ_HPF_CUSTOM_PARAMETER_STRU;

////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_DSP_PEQ_PARAMETER_STRU CODE gSector_DspPeqParameter;
EXTERN SECTOR_DSP_HPF_PARAMETER_STRU CODE gSector_DspHpfParameter;
EXTERN SECTOR_DSP_PEQ_HPF_CUSTOM_PARAMETER_STRU CODE gSector_DspPeqHpfParameterCustom;

#endif /* _SECTOR_PEQ_HPF_PARAM_H_ */

