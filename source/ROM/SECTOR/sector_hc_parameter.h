#ifndef _SECTOR_HC_PARAMETER_H_
#define _SECTOR_HC_PARAMETER_H_

#include "os.h"
#include "bt_types.h"

#define	HC_NUM_OF_SUPP_IAC				2

//#warning "try to move to conn hdl, add RSSI"
typedef struct
{
    U16 flush_timeout;
//  U16 link_supervision_timeout;
    U16 link_policy_setting;
    U16 failed_contact_counter;
} HC_LINK_INFO_TYPE;

typedef struct
{
	U8 max_retry_num;
	U8 max_waiting_interval;
} REPEATED_ATTEMPT_INFO_TYPE;

typedef struct bdaddr_parity_stru
{
	BD_ADDR_STRU    BdAddr;
	PARITY_ARR      Parity;

} BDADDR_PARITY_STRU;

typedef U8 INQUIRY_MODE;
#define INQUIRY_MODE_STANDARD           ((INQUIRY_MODE)0)
#define INQUIRY_MODE_WITH_RSSI          ((INQUIRY_MODE)1)
#define INQUIRY_MODE_WITH_RSSI_OR_EIR   ((INQUIRY_MODE)2)

#define HCI_NUM_EVENT_FILTER            2

#define HC_ENABLE_INQ_RST_EVT_FLTR_TYPE1 0x01
#define HC_ENABLE_INQ_RST_EVT_FLTR_TYPE2 0x02
#define HC_ENABLE_CONN_SETUP_EVT_FLTR_TYPE1 0x01
#define HC_ENABLE_CONN_SETUP_EVT_FLTR_TYPE2 0x02

#define HC_MAX_NUM_OF_INQ_RST_EVT_FLTR 2
#define HC_MAX_NUM_OF_CONN_SETUP_EVT_FLTR 2


#if 0
typedef struct
{
    U8 cond1_en :1;
    U8 cond2_en :1;
    U8 rsvd     :6;
}HC_FLTR_CONDTITION_MASK;
#endif

typedef struct
{
    U8 type;
    U8 class_of_device[3];
    U8 class_of_device_mask[3];
} HC_INQ_RST_FLTR_CONDITION_TYPE1;

typedef struct
{
    U8 type;
    BD_ADDR bd_addr;
} HC_INQ_RST_FLTR_CONDITION_TYPE2;

typedef struct
{
    U8 type;
    U8 condition_content[6];
} HC_INQ_RST_FLTR_CONDITION_COMMON_TYPE;


typedef union
{
    U8 Value[7];
    HC_INQ_RST_FLTR_CONDITION_TYPE1 condition1;
    HC_INQ_RST_FLTR_CONDITION_TYPE2 condition2;
    HC_INQ_RST_FLTR_CONDITION_COMMON_TYPE common_type;
}HC_INQ_RST_FLTR_CONDITION_TYPE;

typedef struct
{
    U8 inq_rst_evt_fltr_used_mask;
    HC_INQ_RST_FLTR_CONDITION_TYPE condition[HC_MAX_NUM_OF_INQ_RST_EVT_FLTR];
} HC_INQ_RST_EVENT_FILTER_TYPE;

typedef struct
{
    U8 type;
    U8 auto_accept_flag;
} HC_CONN_SETUP_FLTR_CONDITION_TYPE0;

typedef struct
{
    U8 type;
    U8 class_of_device[3];
    U8 class_of_device_mask[3];
    U8 auto_accept_flag;
} HC_CONN_SETUP_FLTR_CONDITION_TYPE1;

typedef struct
{
    U8 type;
    BD_ADDR bd_addr;
    U8 auto_accept_flag;
} HC_CONN_SETUP_FLTR_CONDITION_TYPE2;

typedef struct
{
    U8 type;
    U8 condition_content[6];
    U8 auto_accept_flag;
} HC_CONN_SETUP_FLTR_CONDITION_COMMON_TYPE;

typedef union
{
    U8 Value[8];
    HC_CONN_SETUP_FLTR_CONDITION_TYPE0 condition0;
    HC_CONN_SETUP_FLTR_CONDITION_TYPE1 condition1;
    HC_CONN_SETUP_FLTR_CONDITION_TYPE2 condition2;
    HC_CONN_SETUP_FLTR_CONDITION_COMMON_TYPE common_type;
}HC_CONN_SETUP_FLTR_CONDITION_TYPE;


typedef struct
{
    U8 conn_setup_evt_fltr_used_mask;
    HC_CONN_SETUP_FLTR_CONDITION_TYPE condition[HC_MAX_NUM_OF_CONN_SETUP_EVT_FLTR]; 
}HC_CONN_SETUP_EVENT_FILTER_TYPE;


typedef struct
{
    HC_INQ_RST_EVENT_FILTER_TYPE inq_rst_condition; 
    HC_CONN_SETUP_EVENT_FILTER_TYPE conn_setup_condition;
} HC_EVENT_FILTER_TYPE;


#if 0
typedef struct
{
//    U8                  filter_type;
//    U8                  filter_condition_type;
    HC_CONDITION_TYPE   condition;
} HC_EVENT_FILTER_TYPE;
#endif


typedef struct
{
	U8	scatternet_mode;    /* Deprecated */
	U8 	scan_enable;
	U8 	sac_scan_enable;    /* Deprecated */
	U8 	inquiry_scan_type;
	U8	inquiry_mode;
	U8	page_scan_type;
	U8  SR;
	U8	PIN_type;
	U8	authentication_enable;
	U8	encryption_mode;
	U8	hold_mode_activity;
	U8	num_broadcast_retransmissions;
	U8	AFH_channel_assessment_mode;
	U8  SSP_mode;
	U8  SSP_debug_mode;
	U8	mmi1_interval;
	U8	number_of_support_iac;
	U8	current_iac_used_idx;

	BDADDR_PARITY_STRU	iac_bdaddr_parity[HC_NUM_OF_SUPP_IAC];

	U8	AFH_host_channel_classification[10];
	U16 inquiry_scan_interval;
	U16 inquiry_scan_window;
	U16	page_timeout;
	U16	connection_accept_timeout;
	U16	read_logical_link_accept_timeout;
	U16	page_scan_interval;
	U16 page_scan_window;
	U16	sac_scan_interval_in_2slots;
	U16	voice_setting;
	U16	default_link_policy_setting;
	U8 current_num_of_stored_link_key;
	REPEATED_ATTEMPT_INFO_TYPE repeat_attempt_info_type;
	HC_LINK_INFO_TYPE	link_info[MAX_NUM_ACL_LINK];
	U32 tLmPageScanConn; // timer interval of bb for page scan enabled mode, detach if timer expired
	U32 tLmDiscoConn; // timer interval of bb for discoveriable mode, detach if timer expired
	U8	hc_misc_supp_fea0;
 	U8	hc_misc_supp_fea1;

    BOOL EnableDUT;
	HC_EVENT_FILTER_TYPE    event_filter;
    U8                      qos_best_effort_rest_weight;
	
	U32						lightDbgPrintDisableMask[3];	
} HC_PARAMETER_TYPE;

typedef struct
{
	HC_PARAMETER_TYPE	hc_parameter;

	CRC16 CRC;

} SECTOR_HC_PARAMETER_STRU;

//========================================== HC misc support feature
//- feature 0 bank
#define HC_MISC_SUPP_FEA0_RESET_SOFT                        0x04
#define HC_MISC_SUPP_FEA0_RESET_HARD                        0x08
#define HC_MISC_SUPP_FEA0_AUTO_CONN_ACP                     0x20
#define HC_MISC_SUPP_FEA0_USB_HUB_SUPPORT                   0x40
#define HC_MISC_SUPP_FEA0_RESET_WITH_ICE                    0x80

#define HC_MISC_SUPP_FEA_RESET_SOFT()                       (gHC_parameter.hc_misc_supp_fea0 & HC_MISC_SUPP_FEA0_RESET_SOFT)
#define HC_MISC_SUPP_FEA_RESET_HARD()                       (gHC_parameter.hc_misc_supp_fea0 & HC_MISC_SUPP_FEA0_RESET_HARD)
#define HC_MISC_SUPP_FEA_AUTO_CONN_ACP()                    (gHC_parameter.hc_misc_supp_fea0 & HC_MISC_SUPP_FEA0_AUTO_CONN_ACP)
#define HC_MISC_SUPP_FEA_USB_HUB_SUPPORT()                  (gHC_parameter.hc_misc_supp_fea0 & HC_MISC_SUPP_FEA0_USB_HUB_SUPPORT)
#define HC_MISC_SUPP_FEA_RESET_WITH_ICE()                   (gHC_parameter.hc_misc_supp_fea0 & HC_MISC_SUPP_FEA0_RESET_WITH_ICE)
//===================================================================

//- feature 1 bank
#define HC_MISC_SUPP_FEA1_REMOTE_EEPROM_UPDATE              0x01
#define HC_MISC_SUPP_FEA1_UART1_READ_BDADDR_TEST            0x02
#define HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT			0x04
#define HC_MISC_SUPP_FEA1_SPI_LIGHT_DBGPRINT				0x08

#define HC_MISC_SUPP_FEA_REMOTE_EEPROM_UPDATE()             (gHC_parameter.hc_misc_supp_fea1 & HC_MISC_SUPP_FEA1_REMOTE_EEPROM_UPDATE)
#define HC_MISC_SUPP_FEA_UART1_READ_BDADDR_TEST()           (gHC_parameter.hc_misc_supp_fea1 & HC_MISC_SUPP_FEA1_UART1_READ_BDADDR_TEST)
#define HC_MISC_SUPP_FEA_DISABLE_LIGHT_DBGPRINT()			(gHC_parameter.hc_misc_supp_fea1 & HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT)
#define HC_MISC_SUPP_FEA_SPI_LIGHT_DBGPRINT()				(gHC_parameter.hc_misc_supp_fea1 & HC_MISC_SUPP_FEA1_SPI_LIGHT_DBGPRINT)
//===================================================================

//========================================== HC scatternet mode
#define HC_SCATTERNET_FEA_BACKGROUND_SCAN                           0x01

#define HC_SCATTERNET_FEA_BACKGROUND_SCAN_ENABLE()                      (gHC_parameter.scatternet_mode & HC_SCATTERNET_FEA_BACKGROUND_SCAN)

#endif /* _SECTOR_HC_PARAMETER_H_ */

