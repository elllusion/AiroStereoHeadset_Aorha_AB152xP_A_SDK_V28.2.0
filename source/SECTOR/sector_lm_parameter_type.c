/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_lm_parameter_type.c defines the LM parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_lm_param.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_LM_PARAMETER_STRU CODE gSector_LmParameter =
{
    {
    	LM_MISC_SUPP_FEA0_TRANSACTION_TIMER_ENABLE						|
    	LM_MISC_SUPP_FEA0_NO_ACTIVE_SNIFF_SUBRATING|
    	//LM_MISC_SUPP_FEA0_PLC_ON |
    	//LM_MISC_SUPP_FEA0_SLAVE_ACT_UNSNIFF_WHEN_RECV_LMP_ACCEPTED	|
    	0x00,
	#if defined SCO_WOOFER
		//LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE |
		//LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE |
	#elif defined SOUND_BAR
		//LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE |
		LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE |
    #elif  defined TWS_SETTINGS
        LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE |
        LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE |
	#else
		LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE |
		LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE |
		#ifdef OPERATION_IN_BB_ASIC_VERSION
		#if (defined AB1520C) || (defined AB1520S) || (defined AB1520D)
		LM_MISC_SUPP_FEA1_MCU24M_FOR_PAIRING_ENABLE |
		#endif
		#endif
	#endif

    	0x00,

    	480 	// U16 wait_setup_cpl_interval;   //Unit : Half slot
    },

    //- CRC
    {0,0}
};