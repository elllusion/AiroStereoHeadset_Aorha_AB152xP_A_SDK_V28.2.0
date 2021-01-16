#ifndef _SECTOR_DRIVER_CTL_H_
#define _SECTOR_DRIVER_CTL_H_

	#include "bt_config_profile.h"
	#include "bt_types.h"
	#include <crc16.h>
	#include "sector_mmi_nvram_type.h"

	typedef struct
	{
		U8	B2;
		U8	B1;
		U8	B0;
	} LINEIN_RF_3_WIRED_TYPE;

	typedef enum
	{
		MODE_LINE_IN_3WIRE_0xA5,
		MODE_LINE_IN_BYPASS_3WIRE_0xA5,
		MODE_SCO_3WIRE_0xA5,
		MODE_A2DP_3WIRE_0xA5,
		MODE_3WIRE_0xA5_NO,
	}MODE_3WIRE_0xA5;

    enum au_in_scenario_sel
    {
        AU_IN_AD = 0,
        AU_IN_DISABLE,
        AU_IN_I2S_M_48K,
        AU_IN_I2S_M_BOOST_96K,
        AU_IN_I2S_M_BOOST_192K,
        AU_IN_I2S_S_48K,
        AU_IN_I2S_M_BOOST_48K,
        AU_IN_I2S_M_96K,
        AU_IN_I2S_S_96K,
        AU_IN_I2S_S_44_1K,

    };

    enum au_out_scenario_sel
    {
        AU_OUT_DA = 0,
        AU_OUT_DISABLE,
        AU_OUT_I2S_M_48K,
        AU_OUT_I2S_M_BOOST_96K,
        AU_OUT_I2S_M_BOOST_192K,
        AU_OUT_I2S_S_48K,
        AU_OUT_SPDIF_48K,
        AU_OUT_SPDIF_96K,
        AU_OUT_I2S_M_96K,
        AU_OUT_I2S_M_BOOST_48K,
        AU_OUT_I2S_S_96K,
        AU_OUT_I2S_S_44_1K,
        AU_OUT_DA_96K,
    };

	enum spdif_pio
	{
		SPDIF_GPIO_03 = 3,
		SPDIF_GPIO_04,
		SPDIF_GPIO_05,
		SPDIF_GPIO_06,
		SPDIF_GPIO_07,
		SPDIF_GPIO_08,
		SPDIF_GPIO_09,
		SPDIF_GPIO_10,
		SPDIF_GPIO_11,
		SPDIF_GPIO_12,
		SPDIF_GPIO_13,
		SPDIF_GPIO_14,
		SPDIF_GPIO_15,
		SPDIF_GPIO_16,
		SPDIF_GPIO_17,
		SPDIF_GPIO_18, // prohibited pin
		SPDIF_GPIO_19,
		SPDIF_GPIO_20,
		SPDIF_GPIO_21,
		SPDIF_GPIO_22,
		SPDIF_GPIO_23,
		SPDIF_GPIO_24,
		SPDIF_GPIO_25,
	};

	enum i2s_group_sel
	{
		I2S_GROUP_SEL_0 = 0, //Set GPIO    4/ 5/ 6/ 7 as I2S
		I2S_GROUP_SEL_2 = 1, //Set GPIO  14/15/16/17 as I2S
	};

	enum
	{
		MCLK_DIVIDE_BY_1,
		MCLK_DIVIDE_BY_2,
		MCLK_DIVIDE_BY_4,
		MCLK_DIVIDE_EXTERNAL_BASE = 4,
	};


	#ifdef OPERATION_IN_BB_FPGA_VERSION
		#define DEFAULT_MCLK	(2)   //6.144*2MHz
		#define MCLK_12288K	(MCLK_DIVIDE_BY_1)  //I2S(S), I2S(M, 48K, 96K), SPDIF(96K)
		#define MCLK_6144K	(MCLK_DIVIDE_BY_2)	 //SPDIF(48K)
	#else
		#define DEFAULT_MCLK	(4)   //6.144*4MHz
		#define MCLK_24576K	(MCLK_DIVIDE_BY_1) //I2S(M, 192K)
		#define MCLK_12288K	(MCLK_DIVIDE_BY_2) //I2S(S), I2S(M, 48K, 96K),SPDIF(96K)
		#define MCLK_6144K	(MCLK_DIVIDE_BY_4)	//SPDIF(48K)
	#endif

	typedef struct
	{
		U8 AUDIO_IN_SCENARIO_SEL;
		U8 AUDIO_OUT_SCENARIO_SEL;
		U8 AUDIO_I2S_MASTER_GROUP_SEL;
		U8 AUDIO_I2S_SLAVE_GROUP_SEL;
		U8 SPDIF_PIO;
		U8 I2SIN_PIO;
		U8 MCLK_INTERNAL;
		U8 MCLK_DIVIDE_NUM;
		U8 MCLK_PHASE_INVERT;
		U8 MIC_DIGITAL_DATA_IN_PIN;
		U8 MIC_DIGITAL_CLOCK_OUT_PIN;
		U8 I2SMCLKPioDisable:1;
		U8 reserved:7;
	}AUDIO_IN_OUT_SETTINGS;
	
	//VoicePromptGainSettingsType
	enum
	{
		VPRT_GAIN_NO_LOW_BOUND,
		VPRT_GAIN_DEFAULT,
		VPRT_GAIN_WITH_LOW_BOUND,
	};

	typedef struct
	{
		U8 vpAnalogGain;
		U8 rtAnalogGain;
	}VPRTAnalogDefaultGain;

	typedef struct
	{
		U8 digitalGainVP;
		U8 digitalGainRT;
	}VPRTSpkGainSet;

	typedef struct
	{
		U8 DSP_always_on:1;  //- DSP enable when DSP initial
		U8 zsp_enable:1;	//- ZSP enable switch
		U8 LineInModeSel:2;  // 0: LINE_IN_EXIT_MODE; 1: LINE_IN_BYPASS_MODE; 2: LINE_IN_DSP_MODE;
		U8 AUXMixMode:1;
		U8 I2SDumpEnable:1;
		U8 isDualMic:1;
		U8 isHQVPEnabled:1;
		LINEIN_RF_3_WIRED_TYPE		Mode3Wire0xA5Ctl[MODE_3WIRE_0xA5_NO];
		AUDIO_IN_OUT_SETTINGS		audioInOutCtl;
		AUDIO_IN_OUT_SETTINGS		MPAudioInOutCtl;
		U8	versionType;
		U8 reserved[8];
		U8 FMClock32KPio;
		VPRTAnalogDefaultGain vprtAnalogGain[SOUND_LEVEL_NO];
		VPRTSpkGainSet vprtSpkGainSet[SOUND_LEVEL_NO];
		VPRTSpkGainSet vprtSpkGainSetSco[SOUND_LEVEL_NO];
		VPRTSpkGainSet vprtSpkGainSetA2dp[SOUND_LEVEL_NO];
		VPRTSpkGainSet vprtSpkGainSetLineIn[SOUND_LEVEL_NO];
	}DRIVER_CTL_TYPE;

	typedef struct
	{
		DRIVER_CTL_TYPE driver_ctl;

		CRC16 CRC;

	} SECTOR_DRIVER_CTL_STRU;

#endif
