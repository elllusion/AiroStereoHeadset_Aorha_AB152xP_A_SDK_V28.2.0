#ifndef _SECTOR_LM_PARAM_H_
#define _SECTOR_LM_PARAM_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_lm_param.h defines the LM parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define LM_MISC_SUPP_FEA0_TRANSACTION_TIMER_ENABLE					0x01
#define LM_MISC_SUPP_FEA0_EXECUTEONESUBSTATEBYBBACK					0x02
#define LM_MISC_SUPP_FEA0_LOCALSUBRATEALWAYSONE						0x04
#define LM_MISC_SUPP_FEA0_SLAVE_ACT_UNSNIFF_WHEN_RECV_LMP_ACCEPTED	0x08
#define LM_MISC_SUPP_FEA0_NO_ACTIVE_SNIFF_SUBRATING					0x10
#define LM_MISC_SUPP_FEA0_DISCARD_CLASSIFICATION_WHEN_SCO_ON		0x20
#define LM_MISC_SUPP_FEA0_TURN_ON_TX_3M_MODE						0x40
#define LM_MISC_SUPP_FEA0_PLC_ON									0x80


#define LM_MISC_SUPP_FEA1_TWS_AFH_ENABLE							0x02
#define LM_MISC_SUPP_FEA1_TWS_CHANNELCLASSIFICATION					0x04
#define LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE                   0x08
#define LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE                  0x10
#define LM_MISC_SUPP_FEA1_MCU24M_FOR_PAIRING_ENABLE                 0x20

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_lm_para
{
	U8 misc_ctl0;
	U8 misc_ctl1;
	#if 0
	U8 RSSI_GoldenRange_UpperBound;
	U8 RSSI_GoldenRange_LowerBound;
	U8 normal_clock_drift;
	U8 slow_clock_drift;
	#endif
	U16 wait_setup_cpl_interval;   //Unit : Half slot

} LM_PARAMETER_STRU;

typedef struct stru_sector_lm_para
{
    LM_PARAMETER_STRU LmParameter;

    CRC16 CRC;

} SECTOR_LM_PARAMETER_STRU;


#endif /* _SECTOR_LM_PARAM_H_ */

