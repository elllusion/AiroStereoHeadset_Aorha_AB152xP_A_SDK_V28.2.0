//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _EEPROM_SECTOR_FORMAT_HID_H_
#define _EEPROM_SECTOR_FORMAT_HID_H_

#include "sector_mmi_driver_variation_nvram.h"


typedef struct
{
	U8 dummy;
} MMI_NVRAM_TYPE;

#define MMI_EOF								0xFF

typedef enum
{
	LED_BON,
	LED_BOFF,
	LED_RON,
	LED_ROFF,
	LED_BOTHON,
	LED_BOTHOFF,
	LED_BSFLASH,
	LED_RSFLASH,
	LED_BFFLASH,
	LED_RFFLASH,
	LED_BOTHFLASH,
	LED_SEQFLASH,
	LED_BRAMPFLASH,
	LED_RRAMPFLASH,
	LED_USRDEF,
	LED_INVALID,

 	DRIVER_LED_USER_0	=	0x80,
 	DRIVER_LED_USER_1,
 	DRIVER_LED_USER_2,
 	DRIVER_LED_USER_3,
 	DRIVER_LED_USER_4,
 	DRIVER_LED_USER_5,
	DRIVER_LED_USER_6,
	DRIVER_LED_USER_7,
	DRIVER_LED_USER_8,

}DriverLedCmdType;


typedef enum
{
	KEY_PUSH,
	KEY_TAP,
	KEY_PRESS,
	KEY_LONGPRESS,
	KEY_NOACT
}DriverKeyActionType;

enum
{
	KEY0,
	KEY1,
	KEY2,
	KEY3,//GPIO3 SPI MOTION WAKUP PIN
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY10,
	KEY11,
	KEY12,
	KEY13,
	TOTAL_KEY_NO,
};

typedef U8	KeyIndex;

typedef enum
{
	KEY_NO_EVENT,
	KEY1_CLICK,
	KEY2_CLICK,
	KEY3_CLICK,
	KEY4_CLICK,
	KEY5_CLICK,
	KEY6_CLICK,
	KEY7_CLICK,
	KEY8_CLICK,
	KEY9_CLICK,
	KEY10_CLICK,
	KEY11_CLICK,
	KEY12_CLICK,
	KEY13_CLICK,
	KEY1_NOT_PRESS,
	KEY2_NOT_PRESS,
	KEY3_NOT_PRESS,
	KEY4_NOT_PRESS,
	KEY5_NOT_PRESS,
	KEY6_NOT_PRESS,
	KEY7_NOT_PRESS,
	KEY8_NOT_PRESS,
	KEY9_NOT_PRESS,
	KEY10_NOT_PRESS,
	KEY11_NOT_PRESS,
	KEY12_NOT_PRESS,
	KEY13_NOT_PRESS,
	KEY_CONNECT,

	KEY_INVALID
} MMI_KEY_CODE;

typedef enum
{
	MMI_OFF,
	MMI_INIT,
	MMI_DISCOVERABLE,
	MMI_CONNECTING,
	MMI_CONNECTING_BUSY,
	MMI_CONNECTING_IDLE,
	MMI_CONNECTING_IDLE_2,
	MMI_CONNECTING_SUSPEND,
	//MMI_CONNECTING_DISCONNECT,
	MMI_SPECIAL_MODE,
	MMI_WAIT_POWER_ON_MODE,
	MMI_TOTAL_STATE_NO
}MMI_MAIN_STATE;

typedef struct
{
	KeyIndex keyCode;
	DriverKeyActionType keyAction;
	MMI_KEY_CODE mmi_key_event;
} MMI_KEY_MAP_TYPE;

#define NUMBER_OF_KEYS			27

#define SYS_HID_MOUSE_TYPE					0x00
#define SYS_HID_IIM_TYPE					0x01

#define MMI_MAIN_STATE_HANDLE_MAX	15

typedef struct
{
	DriverLedCmdType entry;
	DriverLedCmdType exit;
} LED_SETTING_TYPE;

typedef struct
{
	U8 power_on;
	U8 connect;
	U8 self_disconnect;
	U8 host_abnormal_disconnect;
}CONNECT_LED_SETTING_TYPE;

typedef struct
{
	U8 sniff_max_interval_low;
	U8 sniff_max_interval_high;
	U8 sniff_min_interval_low;
	U8 sniff_min_interval_high;
	U8 sniff_attempt_low;
	U8 sniff_attempt_high;
	U8 sniff_timeout_low;
	U8 sniff_timeout_high;
} MMI_SNIFF_INTERVAL_PARA;

typedef struct
{
	U8 max_latency_low;
	U8 max_latency_high;
	U8 min_remote_timeout_low;
	U8 min_remote_timeout_high;
	U8 min_local_timeout_low;
	U8 min_local_timeout_high;

}MMI_SNIFF_SUBRATING_PARA;

typedef struct
{
	U8 normal_reconnect_timeout;//unit Second
	U8 abnormal_reconnect_timeout;//unit Second
	U8 vc_unplug_timeout;//unit Second
	U8 normal_reconnect_retry_count;
	U8 abnormal_reconnect_retry_count;
	U8 reconnect_default_para;//bit 0 Attempt To Reconnect On Abnormal Disconnect  //bit 1 Default Value Of Reconnect Initiate
							  //bit 2 Default Value Of Normally Connectable //bit 3 Default Value Of New
							  //bit 4 Exit Discoverable On Authentication Failure
							  //bit 5 Generate VC unplug on connect button press
							  //bit 6 Become discoverable on connect button press
							  //bit 7 Power On Reconnect
} MMI_CONNECT_TIMEOUT_PARA;

typedef struct
{
	U8 wait_power_on_led_enable;
	U8 wait_power_on_led_time;//unit Second

} MMI_POWER_ON_WAIT_STATE_PARA;

typedef struct
{
	U8 io_capability;
	U8 oob_data_present;
	U8 auth_requirments;

} MMI_SIMPLE_PAIRING_PARA;

typedef struct
{
	U8	packet_type_L;
	U8	packet_type_H;

} MMI_CREATE_CONNECT_PARA;

typedef struct
{
	U8	rf_para[3];
} MMI_SHUT_DOWN_RF_PARA;

typedef union
{
	U16 timer_setting_u16;
	U8  timer_setting_u8[2];
} MMI_TIMER_SETTING;

typedef struct
{
	MMI_TIMER_SETTING timer_setting;
	U8 sniff_in_read_enable;
	U32 qos_latency;

} MMI_MOUSE_SENSOR_TIMING_PARA;

typedef union
{
	U32 value;
	struct
	{
		U8 B3;
		U8 B2;
		U8 B1;
		U8 B0;
	} in_byte;
} MMI_FOUR_BYTE_UNION_TYPE;//BIg Endian Version

typedef struct
{
	U8 Byte0;
	U8 Byte1;
	U8 Byte2;
}MMI_UsrDefLed;

#define MMI_TIMER_READ_SENSOR 							0x00
#define MMI_SNIFF_IN_READ_SENSOR						0x01

#define MMI_SNIFF_SUBRATING_DISABLE			0x00
#define MMI_SNIFF_SUBRATING_ENABLE			0x01

#define MAX_USRDEF_LED_DATA_NO 10

#define RESERVE_BYTE_NO 1

typedef union
{
	U8 gap_para;
	U8 cpt_status;
} MMI_MSG_BODY;

typedef struct
{
	//refer to 010 GAP Interface.doc for message format
	U8 data_offset; //to the start of meaningful message
	U8 reserve_byte[RESERVE_BYTE_NO];
	U8 msg_opcode;
	U8 event_code;
	U8 para_len;
	MMI_MSG_BODY msg_body_ptr;
} MMI_MSG_TYPE;

typedef struct
{
	void (*MMI_main_init_handle)(void);
} MMI_MAIN_INIT_HANDLER;


typedef struct
{
	void (*MMI_event_handle)(void);
} MMI_MAIN_EVENT_HANDLER;

typedef struct
{
	void (*MMI_customer_event_handle)(U8 XDATA_PTR msgPtr);
} MMI_CUSTOMER_EVENT_HANDLER;

typedef struct
{
	U8 XDATA_PTR (*MMI_anytime_event_handle)(void);
}MMI_MAIN_ANYTIME_EVENT_HANDLER;

typedef struct
{
	void (*MMI_state_init_action)(void);
	void (*MMI_state_msg_handler)(MMI_MSG_TYPE XDATA_PTR msgPtr);
	void (*MMI_state_exit_action)(void);

	//U8 key_event_no;
} MMI_HANDLER_SET_TYPE;

typedef struct
{
	void (*MMI_msg_handle)(MMI_MSG_TYPE XDATA_PTR msgPtr);
} MMI_MAIN_MSG_HANDLER;

typedef struct
{
	void (*MMI_anytime_aclevent_handle)(U8 XDATA_PTR msgPtr);
}MMI_ANYTIME_ACL_EVENT_HANDLER;

enum
{
	MMI_CONNECTING_BUSY_MODE,
	MMI_CONNECTING_IDLE_MODE,
	MMI_CONNECTING_IDLE_2_MODE,
	MMI_CONNECTING_SUSPEND_MODE,
	MMI_TOTAL_SNIFF_STATE_NO

};//For MMI_Sniff_interval_para	and MMI_SendSniffReq

typedef enum
{
	MMI_INITACTION,
	MMI_TOTAL_MAIN_INIT_ACTION_NO
}MMI_MAIN_INIT_ACTION_TYPE;

typedef enum
{
	MMI_ACL,
	MMI_DRIVER,
	MMI_TOTAL_MAIN_EVENT_NO

}MMI_MAIN_EVENT_TYPE;

#define MMI_MAIN_EVENT_HANDLE_MAX	3

typedef enum
{
	MMI_CUSTOMER_ACL,
	MMI_CUSTOMER_DRIVER,
	MMI_TOTAL_CUSTOMER_EVENT_NO

}MMI_CUSTOMER_EVENT_TYPE;

typedef enum
{
	MMI_ANYTIME_ACL,
	MMI_TOTAL_MAIN_ANYTIME_EVENT_NO
}MMI_MAIN_ANYTIME_EVENT_TYPE;

typedef enum
{
	//MMI_PM_HANDLER,
	MMI_INIT_HANDLER_INIT,
	MMI_INIT_HANDLER_MAIN,
	//MMI_INIT_HANDLER_WAIT_EVENT_RECONNECT,
	MMI_INIT_HANDLER_WAIT_ROLE_CHANGE_COMPLETE,
	MMI_INIT_HANDLER_WAIT_AUTHEN_COMPLETE,
	MMI_INIT_HANDLER_WAIT_CONNECT_BUTTON_DISCONNECT_COMPLETE,
	MMI_DISCOVERABLE_HANDLER_INIT,
	MMI_DISCOVERABLE_HANDLER_MAIN,
	MMI_DISCOVERABLE_HANDLER_MAIN_WAIT_CONNECTION_COMPLETE,
	MMI_CONNECTING_HANDLER_INIT,
	MMI_CONNECTING_HANDLER_WAIT_CONNECT_BUTTON_DISCONNECT_COMPLETE,
	MMI_CONNECTING_BUSY_HANDLER_MAIN,
	MMI_CONNECTING_BUSY_HANDLER_WAIT_DISCONNECT_COMPLETE,
	//MMI_CONNECTING_DISCONNECT_WAIT_SIGNAL,
	//MMI_CONNECTING_DISCONNECT_WAIT_CONNECT_COMPLETE,
	//MMI_CONNECTING_DISCONNECT_WAIT_ROLE_CHANGE_COMPLETE,
	MMI_TOTAL_MSG_HANDLER_NO

}MMI_MSG_HANDLER_TYPE;

typedef enum
{
	MMI_PINCODE_REQUEST_COME,
	MMI_GAP_LINK_COMPLETE_COME,
	MMI_IIM_DISCOVER_DISCONNECT_COME,
	MMI_USER_CONFIRMATION_PASSKEY_REQUEST_COME,
	MMI_LOW_BATTERY_COME,
	MMI_TOTAL_ANYTIME_ACL_EVENT_NO
}MMI_ANYTIME_ACL_EVENT_TYPE;

typedef struct
{
	U8 PIN_code_length;
	U8 PIN_code[16];
	LED_SETTING_TYPE ledSetting[MMI_MAIN_STATE_HANDLE_MAX];
	CONNECT_LED_SETTING_TYPE connect_led_setting;
	//U32 sniff_timer_in_active;
	//U32 sniff_timer_in_idle;
	//U32 sniff_timer_in_suspend;
	//U32 discover_mode_off_to;
	//U32 initial_self_connect_off_to;
	//U32 initial_non_connect_powerdown_to;
	U8 os_power_saving_enable;
	U8 state_change_in_connect;
	U8 sniff_timer_in_active;//in unit 1 Seconds
	U8 sniff_timer_in_idle;//in unit 1 Seconds
	U8 sniff_timer_in_idle_2;//in unit 1 Seconds
	//U8 sniff_timer_in_suspend;//in unit 1 min
	U16 sniff_timer_in_suspend;//in unit 1 Seconds
	U16 discover_mode_off_to;//in unit 1 Seconds
	//U8 initial_self_connect_off_to;//in unit 1 Seconds
	//U8 initial_non_connect_powerdown_to;//in unit 1 Seconds
	U8 no_mmi_read_spi;
	U8  n_task_round_wakeup_sleep;
	U16 power_saving_duration_in_connect_button_press;
	U16 power_saving_duration_in_discover;
	U16 power_saving_duration_in_idle;
	U16 power_saving_duration_in_idle_2;
	U16 power_saving_duration_in_suspend;
	U16 power_saving_duration_in_off_state;
	U16 power_saving_duration_in_poweron_state;
	U16 busy_idle_sniff_interval;
	U16 busy_idle_tran_data_interval;
	U16 busy_idle2_sniff_interval;
	U16 busy_idle2_tran_data_interval;
	U16 busy_suspend_sniff_interval;
	U16 busy_suspend_tran_data_interval;
	U16 power_on_deep_sleep_wait_time;//unit 10ms
	U16 reconnect_deep_sleep_wait_time;//unit 10ms
	U16 disable_powersaving_interval;//unit 10ms
	MMI_FOUR_BYTE_UNION_TYPE watch_dog_timer;
	MMI_SNIFF_INTERVAL_PARA mmi_sniff_interval_para[MMI_TOTAL_SNIFF_STATE_NO];
	MMI_SNIFF_SUBRATING_PARA mmi_sniff_subrating_para;
	MMI_CONNECT_TIMEOUT_PARA mmi_connect_timeout_para;
	MMI_POWER_ON_WAIT_STATE_PARA mmi_power_on_wait_para;
	U8  allow_event_reconnect_mask;
	U8  reconnect_page_scan_mode;
	U8  sniff_subrating_enable;
	//U8  mmi_hid_module_type;//change to Driver
	//U8  mmi_number_of_key_event;
	//MMI_KEY_MAP_TYPE mmi_key_map_tbl[NUMBER_OF_KEYS];
	MMI_UsrDefLed ledData[MAX_USRDEF_LED_DATA_NO];
	MMI_SIMPLE_PAIRING_PARA mmi_simple_pairing_para;
	MMI_CREATE_CONNECT_PARA mmi_create_connect_para;
	//MMI_SHUT_DOWN_RF_PARA mmi_shut_down_rf_para[3];
	MMI_MOUSE_SENSOR_TIMING_PARA mmi_mouse_sensor_timing_para;
	//U8  mmi_function_table_mapping;
	U8  only_sdp_reconnect_flag;
	U8  mmi_num_main_event_handle;
	MMI_MAIN_INIT_HANDLER MMI_MainInitHandlerSet[MMI_TOTAL_MAIN_INIT_ACTION_NO];
	MMI_MAIN_EVENT_HANDLER MMI_MainEventHandlerSet[MMI_MAIN_EVENT_HANDLE_MAX];
	MMI_CUSTOMER_EVENT_HANDLER MMI_CustomerEventHandlerSet[MMI_TOTAL_CUSTOMER_EVENT_NO];
	MMI_MAIN_ANYTIME_EVENT_HANDLER MMI_MainAnyTimeEventHandlerSet[MMI_TOTAL_MAIN_ANYTIME_EVENT_NO];
	MMI_HANDLER_SET_TYPE MMI_HandlerSet[MMI_MAIN_STATE_HANDLE_MAX];
	MMI_MAIN_MSG_HANDLER MMI_MainMsgHandlerSet[MMI_TOTAL_MSG_HANDLER_NO];
	MMI_ANYTIME_ACL_EVENT_HANDLER MMI_AnyTimeAclEventHandlerSet[ MMI_TOTAL_ANYTIME_ACL_EVENT_NO ];

} MMI_HID_NVRAM_TYPE;

//DRIVER EEPROM setting

typedef struct
{
	U8 sensor_type;
	U8 sensor_pid;
	U8 cpi;
	U8 sleep1_setting_run_downshift;
	U8 sleep2_setting_rest1_rate;
	U8 enter_time_rest1_downshift;
	U8 rest2_rate;
	U8 rest2_downshift;
	U8 rest3_rate;
	U8 laser_ctrl;
	U8 laserpower_cfg;
	U8 spi_hold;
	U8 spi_setting;
	U8 spi_opmode;

}DRIVER_MOUSE_SENSOR_PARA;//for pixart3204 and avago7550 para setting

typedef struct
{
	U8 mode;
	U8 ledsetting;
	U8 measure_interval;
	U8 num_instances_average;
	U16 full_voltage;
	U16	low_battery_voltage;
	U16 shut_down_voltage;
	U16 init_ok_voltage;

}DRIVER_BATTERY_TABLE;

typedef struct
{
	U8 special_mode_enable;
	U16 special_mode_gpio;
	U8 power_up_key_L;
	U8 power_down_key_L;
	U8 power_up_press_num;
	U8 power_down_press_num;
	U8 power_save_press_num;
	U8 power_up_1_led;
	U8 power_down_1_led;
	U8 power_up_10_led;
	U8 power_down_10_led;
	U8 power_save_led;

}DRIVER_SPECIAL_MODE_PARA;


typedef struct
{
	void (*DRIVER_init_action)(void);
} DRIVER_INIT_ACTION_HANDLER;

typedef struct
{
	void (*DRIVER_main_action)(void);
} DRIVER_MAIN_ACTION_HANDLER;

typedef struct
{
	void (*DRIVER_UART_action)(void);
	void (*DRIVER_SPI_action)(void);
	void (*DRIVER_I2C_action)(void);
} DRIVER_INTERRUPT_ACTION_HANDLER;

typedef struct
{
	void (*DRIVER_CUSTOMER_event)(U8 XDATA_PTR msgPtr) compact REENTRANT;
} DRIVER_CUSTOMER_EVENT_HANDLER;

typedef struct
{
	void (*SPI_maininitaction)(U8 mode);
	void (*SPI_initaction)(void);

} SPI_HANDLER_INIT_ACTION_SET_TYPE;


typedef struct
{
	void (*SPI_motion)(void);

} SPI_HANDLER_MOTION_SET_TYPE;


typedef struct
{
	void (*SPI_action)(void) ;

} SPI_HANDLER_SET_TYPE;

typedef struct
{
	void (*SPI_power)( U8 mouse_current_mode );

} SPI_HANDLER_POWER_SET_TYPE;

typedef struct
{
	void (*SPI_resync)( U8 read_mode , U8 sleep_mode ) REENTRANT;

} SPI_HANDLER_RESYNC_SET_TYPE;

typedef struct
{
	void (*DRIVER_timer_action)(void);

} DRIVER_HANDLER_TIMER_SET_TYPE;

typedef enum
{
	DRIVER_INITACTION,
	DRIVER_LED_INITACTION,
	DRIVER_KEYPAD_INITACTION,
	DRIVER_WHEEL_INITACTION,
	DRIVER_SHUT_DOWN_COME,
	DRIVER_TOTAL_INIT_ACTION_NO

}DRIVER_INIT_ACTION_HANDLER_TYPE;

typedef enum
{
	DRIVER_CHECK_SPI_ACTION,
	DRIVER_WHEEL_READ_ACTION,
	DRIVER_CHECK_KEY_TIMER_ACTION,
	DRIVER_KEYPAD_POLLING_ACTION,
	DRIVER_CUSTOMER_MAIN_ACTION,
	DRIVER_SEND_DRIVER_EVENT_ACTION,
	DRIVER_PROC_MMI_CMD_ACTION,
	DRIVER_TOTAL_MAIN_ACTION_NO

}DRIVER_MAIN_ACTION_HANDLER_TYPE;

#define DRIVER_MAIN_ACTION_HANDLE_MAX	8

typedef enum
{
	DRIVER_INTERRUPT_1_ACTION,
	DRIVER_TOTAL_INTERRUPT_ACTION_NO

}DRIVER_INTERRUPT_ACTION_HANDLER_TYPE;

typedef enum
{
	DRIVER_CUSTOMER_EVENT_TOTAL_ACTION,
	DRIVER_TOTAL_CUSTOMER_EVENT_NO

}DRIVER_CUSTOMER_EVENT_HANDLER_TYPE;

typedef enum
{
	SPI_INIT_ACTION,
	SPI_TOTAL_INIT_MAIN_ACTION_NO

}SPI_INIT_MAIN_ACTIONS;

typedef enum
{
	SPI_SEND_DATA,
	SPI_TOTAL_MOTION_ACTION_NO

}SPI_MOTION_ACTIONS;

typedef enum
{
	SPI_GET_MOUSE_DATA_TIMER,
	SPI_GET_MOUSE_DATA_INT1,
	SPI_TOTAL_ACTION_NO

}SPI_MAIN_ACTIONS;

typedef enum
{
	SPI_MOUSE_POWER_DOWN,
	SPI_MOUSE_POWER_UP,
	SPI_MOUSE_DPI_SETTING,
	SPI_TOTAL_POWER_ACTION_NO

}SPI_POWER_ACTIONS;

typedef enum
{
	SPI_RESYNC_ACTION,
	SPI_TOTAL_RESYNC_ACTION_NO

}SPI_MAIN_RESYNC_ACTIONS;

typedef enum
{
	DRIVER_TIMER1_ACTION,
	DRIVER_TIMER2_ACTION,
	DRIVER_SNIFFIN_TIMER_ACTION,
	DRIVER_TOTAL_TIMER_ACTION_NO

}DRIVER_MAIN_TIMER_ACTIONS;

#define SPI_20MS													(70)
#define SPI_30MS													(100)
#define SPI_60MS													(200)
#define SPI_1S														(3200L)

#define SPI_PIXART_3204_PRODUCT_ID1									0x30
#define SPI_AVAGO_7550_PRODUCT_ID									0x30
#define SPI_AVAGO_7530_PRODUCT_ID									0x31

#define SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600				0x06
#define SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1200				0x05
#define SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1000				0x04
#define SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800					0x03
#define SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI400					0x00
#define SPI_PIXART_3204_CONFIGURATION_POWERDOWN_CPI800				0x0B  //bit 3 = 1

#define SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI400					0x00
#define SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI1200				0x02
#define SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI3000				0x06

#define SPI_MOUSE_REGISTER_ID1										0x00
#define SPI_MOUSE_REGISTER_ID2										0x01
#define SPI_PIXART_3204_MOUSE_REGISTER_MOTION_STATUS				0x02
#define SPI_PIXART_3204_MOUSE_REGISTER_DELTA_X						0x03
#define SPI_PIXART_3204_MOUSE_REGISTER_DELTA_Y						0x04
#define SPI_PIXART_3204_MOUSE_REGISTER_OP_MODE						0x05
#define SPI_PIXART_3204_MOUSE_REGISTER_CONFIGURATION				0x06
#define SPI_PIXART_3204_MOUSE_REGISTER_IMAGE_QUALITY				0x07
#define SPI_PIXART_3204_MOUSE_REGISTER_OP_STATE						0x08
#define SPI_PIXART_3204_MOUSE_REGISTER_WRITE_PROTECT				0x09
#define SPI_PIXART_3204_MOUSE_REGISTER_SLEEP1_SETTING				0x0A
#define SPI_PIXART_3204_MOUSE_REGISTER_ENTER_TIME					0x0B
#define SPI_PIXART_3204_MOUSE_REGISTER_SLEEP2_SETTING				0x0C

#define SPI_AVAGO_MOUSE_REGISTER_MOTION_02							0x02
#define SPI_AVAGO_MOUSE_REGISTER_DELTA_X_L_03						0x03
#define SPI_AVAGO_MOUSE_REGISTER_DELTA_Y_L_04						0x04
#define SPI_AVAGO_MOUSE_REGISTER_DELTA_XY_H_05						0x05
#define SPI_AVAGO_5030_MOUSE_REGISTER_MOUSE_CONTROL_0D				0x0d
#define SPI_AVAGO_MOUSE_REGISTER_CONFIGURATION2_12					0x12
#define	SPI_AVAGO_MOUSE_REGISTER_LASER_CTRL0_1A						0x1A
#define	SPI_AVAGO_MOUSE_REGISTER_LASER_LSRPWR_CFG0_1C				0x1C
#define	SPI_AVAGO_MOUSE_REGISTER_LASER_LSRPWR_CFG1_1D				0x1D
#define	SPI_AVAGO_MOUSE_REGISTER_LASER_CTRL1_1F						0x1F

#define SPI_AVAGO_7530_MOUSE_REGISTER_RUN_DOWNSHIFT_13				0x13
#define SPI_AVAGO_7530_MOUSE_REGISTER_REST1_RATE_14					0x14
#define SPI_AVAGO_7530_MOUSE_REGISTER_REST1_DOWNSHIFT_15			0x15
#define SPI_AVAGO_7530_MOUSE_REGISTER_REST2_RATE_16					0x16
#define SPI_AVAGO_7530_MOUSE_REGISTER_REST2_DOWNSHIFT_17			0x17
#define SPI_AVAGO_7530_MOUSE_REGISTER_REST3_RATE_18					0x18

#define SPI_AVAGO_5090_MOUSE_REGISTER_RUN_DOWNSHIFT_0E				0x0E
#define SPI_AVAGO_5090_MOUSE_REGISTER_REST1_PERIOD_0F				0x0F
#define SPI_AVAGO_5090_MOUSE_REGISTER_REST1_DOWNSHIFT_10			0x10
#define SPI_AVAGO_5090_MOUSE_REGISTER_REST2_PERIOD_11				0x11
#define SPI_AVAGO_5090_MOUSE_REGISTER_REST2_DOWNSHIFT_12			0x12
#define SPI_AVAGO_5090_MOUSE_REGISTER_REST3_PERIOD_13				0x13

#define	SPI_AVAGO_MOUSE_REGISTER_RESERVED_21						0x21
#define SPI_AVAGO_MOUSE_REGISTER_RESERVED_22						0x22
#define SPI_AVAGO_MOUSE_REGISTER_RESERVED_23						0x23
#define SPI_AVAGO_MOUSE_REGISTER_OBSERVATION_2E						0x2E
#define SPI_AVAGO_MOUSE_REGISTER_POWER_UP_RESET_3A					0x3A
#define SPI_AVAGO_MOUSE_REGISTER_SHUTDOWN_3B						0x3B
#define SPI_AVAGO_MOUSE_REGISTER_RESERVED_3C						0x3C
#define SPI_AVAGO_5090_MOUSE_REGISTER_LED_CTL_40					0x40
#define SPI_AVAGO_5090_MOUSE_REGISTER_MOTION_CTL_41					0x41
#define SPI_AVAGO_5090_MOUSE_REGISTER_AUTO_LED_CTL_43				0x43

#define SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1600					0x66
#define SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1200					0x46
#define SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI800					0x26
#define SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI400					0x06

#define SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000					0x01
#define SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI500					0x00

#define SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI500					0x00
#define SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI1500					0x30
#define SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI1750					0x34

#define SPI_AVAGO_7530_LASER_NORMAL_MODE							0x00
#define SPI_AVAGO_7530_LASER_CALIBRATION_MODE						0x01

#define SPI_READ_FIRST												0x00
#define SPI_SYNC_FIRST												0x01

#define SPI_NORMAL_MODE												0x00
#define SPI_SLEEP1_MODE												0x01
#define SPI_SLEEP2_MODE												0x02
#define SPI_POWER_DOWN_MODE											0x03

#define SPI_INIT_PIXART3204DB										0x00
#define SPI_INIT_PIXART3603DH										0x01
#define SPI_INIT_PIXART3607											0x02
#define	SPI_INIT_AVAGO7550											0x03
#define	SPI_INIT_AVAGO5030											0x04
#define SPI_INIT_AVAGO5090											0x05
#define SPI_INIT_NEWSENSOR											0x10
#define SPI_INIT_NOSENSOR											0xFF


#define SPI_AVAGO_LASER_INCREASE									0x00
#define SPI_AVAGO_LASER_MINUS										0x01
#define SPI_AVAGO_LASER_INCREASE_MINUS_1							0x00
#define SPI_AVAGO_LASER_INCREASE_MINUS_10							0x01

/*******************************************************************************
* Buttons Definition for Input Report
*******************************************************************************/

#define	btn_Two			0x0001
#define	btn_One			0x0002
#define	btn_B			0x0004
#define	btn_A			0x0008
#define	btn_Minus		0x0010
#define	btn_Home		0x0080
#define	btn_Left		0x0100
#define	btn_Right		0x0200
#define	btn_Down		0x0400
#define	btn_Up			0x0800
#define	btn_Plus		0x1000
#define	btn_NCK_DEC		0x2000
#define	btn_Pwr			0x4000
#define	btn_Sync		0x8000

#define GPIO_MAPPING_TABLE_MAX	16

typedef struct
{
	U16	gpio_mask;
}DRIVER_GPIO_MASK;

typedef struct
{
	U8 tilt_wheel_enable;
	U16	tilt_plus_gpio_mask;
	U16	tilt_minus_gpio_mask;
}DRIVER_TILT_WHEEL_TABLE;

typedef struct
{
	U8	z_axis_sel;
	U8  z_axis_enable;
}DRIVER_WHEEL_TABLE;

#define DPI_MODE_BUTTON_CLICK   0x01
#define DPI_MODE_BUTTON_PRESS   0x02

typedef union
{
	U16 keymap_u16;
	U8  keymap_u8[2];
} DRIVER_KEY_KEYMAP;

typedef struct
{
	U8 mode;
	U8 dpi_level;
	U8 dpi_register_setting[3];
	U8 dpi_led_setting[3];
	DRIVER_KEY_KEYMAP dpi_gpio;
}DRIVER_DPI_TABLE;



#define GPIO_PRESS_MASK_MAX	10

enum
{
	GPIO_0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27
};

typedef struct
{
//	U8 gpio_oe_0;
//	U8 gpio_oe_1;
//	U8 gpio_oe_2;
//	U8 gpio_oe_3;
//	U8 gpio_i_inv_en_0;
//	U8 gpio_i_inv_en_1;
//	U8 gpio_i_inv_en_2;
//	U8 gpio_i_inv_en_3;
//	U8 gpio_mux;
//	U8 interrupt_e_0;
//	U8 interrupt_e_1;
//	U8 interrupt_e_2;
//	U8 interrupt_e_3;
	U16 gpio_mask_L;
	U16 gpio_mask_H;
	U16 gpio_key_mask;//After mapping table transfer, only button can be leave after this mask
	U16 gpio_hid_key_mask;//After mapping table transfer, only button show in hid can be leave after this mask
}DRIVER_GPIO_PARA;

typedef struct
{
	U8	gpio_num;

	union
	{
		U16					mapping_value16;
		U8					mapping_value8[2];
	}mapping_value;

}DRIVER_GPIO_MAP_TABLE;

typedef struct
{
	//U8 driver_num_keys;//how many keys are used
	U8 gpio_poll_period;//gpio_poll_period 10ms  32 * 0x0003125 = 10
	U8 keyPressTime;//in unit 0.1Seconds
	U8 keyLongPressTime; // in unit 0.1Seconds
	U8 ledSFlashTime; //in ms
	U8 ledFFlashTime; //in ms
	U8 sensor_action_gpio_value;
	U8 sensor_noaction_gpio_value;
	U8 discoverable_led_stop_time;//in unite seconds
	U8 buck_always_on;
	DRIVER_MOUSE_SENSOR_PARA mouse_sensor_para;
	DRIVER_GPIO_PARA driver_gpio_para;
	U8  driver_num_gpio_mapping;
	DRIVER_GPIO_MAP_TABLE driver_gpio_map_table[GPIO_MAPPING_TABLE_MAX];
	//U32 connect_button_press_time;
	U8 connect_button_led_setting;
	U32 connect_button_press_time;
	DRIVER_GPIO_MASK connect_button_mask;
	U8	driver_num_gpio_press_mask;
	DRIVER_GPIO_MASK gpio_press_mask[GPIO_PRESS_MASK_MAX];
	DRIVER_TILT_WHEEL_TABLE driver_tilt_wheel_table;
	DRIVER_WHEEL_TABLE driver_wheel_table;
	DRIVER_DPI_TABLE driver_dpi_table;
	DRIVER_BATTERY_TABLE driver_battery_table;
	DRIVER_SPECIAL_MODE_PARA driver_special_mode_para;
	U8 mmi_hid_module_type;
	U8 mmi_shut_down_rf_para[3][3];
	U8 dpi_click_change_time;//unit 100ms
	U8 firmware_version[6];
	//U8  driver_function_table_mapping;
	U8  driver_num_main_action_handle;
	DRIVER_INIT_ACTION_HANDLER DRIVER_init_action_handler_set[DRIVER_TOTAL_INIT_ACTION_NO];
	DRIVER_MAIN_ACTION_HANDLER DRIVER_main_action_handler_set[DRIVER_MAIN_ACTION_HANDLE_MAX];
	DRIVER_INTERRUPT_ACTION_HANDLER	DRIVER_interrupt_action_handler_set[DRIVER_TOTAL_INTERRUPT_ACTION_NO];
	DRIVER_CUSTOMER_EVENT_HANDLER DRIVER_customer_event_handler_set[DRIVER_TOTAL_CUSTOMER_EVENT_NO];
	SPI_HANDLER_INIT_ACTION_SET_TYPE SPI_init_action_set[SPI_TOTAL_INIT_MAIN_ACTION_NO];
	//SPI_HANDLER_MOTION_SET_TYPE SPI_HandlerMotionSet[SPI_TOTAL_MOTION_ACTION_NO];
	SPI_HANDLER_SET_TYPE SPI_HandlerSet[SPI_TOTAL_ACTION_NO];
	SPI_HANDLER_POWER_SET_TYPE SPI_HandlerPowerSet[SPI_TOTAL_POWER_ACTION_NO];
	SPI_HANDLER_RESYNC_SET_TYPE SPI_HandlerResyncSet[SPI_TOTAL_RESYNC_ACTION_NO];
	DRIVER_HANDLER_TIMER_SET_TYPE DRIVER_timer_action_handler_set[DRIVER_TOTAL_TIMER_ACTION_NO];

}DRIVER_NVRAM_TYPE;

typedef struct
{
	U8 cpi;
}DRIVER_VARY_PARA_TYPE;

#if defined( APP_MOUSE ) || defined( APP_GAMEPAD ) || defined( APP_KEYBOARD ) || defined( APP_TARGUS_MOUSE )
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x1400
#elif defined (APPMOUSE_2K)
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x600
#elif defined (APPMOUSE_4K)
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x1000
#elif defined (APPMOUSE_8K)
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x2000
#elif defined (HID_IIM_SOC) || defined(APPMOUSE_16K)
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x3000
#else
	#define LENGTH_OF_CODE_MAPING_IN_EEPROM    0x01//ROM EEPROM no code mapping
#endif

typedef struct
{
	U8 	num_of_function;
	U16	length;
	U8  XDATA_PTR patch_ptr;
	U8	patch_bank[LENGTH_OF_CODE_MAPING_IN_EEPROM];
} SYS_CODE_MAPING_BANK;

typedef struct
{
	U8 x_scaling;
	U8 y_scaling;
	U8 w_scaling;
}HID_SCALING_PARA;

typedef struct
{
	U8 x_negate;
	U8 y_negate;
}HID_NEG_PARA;

typedef enum
{
	HID_MOUSE_SEND_DATA,
	HID_TOTAL_MOUSE_SEND_DATA_NO

}HID_MOUSE_SEND_DATA_ACTIONS;

typedef enum
{
	HID_MOUSE_SEND_DATA_TRANSACT,
	HID_TOTAL_MOUSE_SEND_DATA_HANDLER_SET_NO

}HID_MOUSE_SEND_DATA_HANDLER_SET_TYPE;

typedef enum
{
	HID_MAINSTATEENTRY,
	HID_MIAN_STATE_HANDLER_TYPE_NO,

}HID_MIAN_STATE_HANDLER_TYPE;

typedef enum
{
	HID_MOUSEREPORT,
	HID_CUSTOMER_D1,
	HID_CUSTOMER_D2,
	HID_CUSTOMER_D3,
	HID_CUSTOMER_D4,
	HID_CUSTOMER_D5,
	HID_PM_COMMAND_HANDLER_TYPE_NO,

}HID_PM_COMMAND_HANDLER_TYPE;

typedef enum
{
	HID_GET_REPORT,
	HID_SET_REPORT,
	HID_REV_DATA,
	HID_MAIN_REMOTE_EVENT_HANDLER_TYPE_NO,

}HID_REMOTE_EVENT_HANDLER_TYPE;

typedef struct
{
	void (* HID_MainStateHandler)( void );

} HID_MAIN_STATE_HANDLER;

typedef struct
{
	void (* HID_MouseSendData)( U8 mq_data_type , U8 key_code );

} HID_HANDLER_MOUSE_SEND_DATA_TYPE;

typedef struct
{
	void (* HID_PMHandler)( void );
} HID_PM_COMMAND_HANDLER_SET;

typedef struct
{
	void (* HID_UpdateDataInQueue)( U8 XDATA_PTR pm_ptr, U8 XDATA_PTR l2cap_ptr );
	void (* HID_NewItemToQueue   )( U8 XDATA_PTR pm_ptr, U8 XDATA_PTR l2cap_ptr );
	void (* HID_SToL2CAP)( void );

} HID_MOUSE_SEND_DATA_HANDLER_SET;

typedef struct
{
	void (* HID_RemoteEventHandler)( void );
} HID_REMOTE_EVENT_HANDLER_SET;

typedef struct
{
	U8  report_threshold;
	U8  update_Qdata_mode;
	U8  swapXY; // 0 : normal, 1 : swap
	HID_SCALING_PARA hid_scaling_para;
	HID_NEG_PARA hid_neg_para;
	U8  tilt_wheel_enable;
	U8  report_data_length;
//	U8  hid_function_table_mapping;
	HID_MAIN_STATE_HANDLER           HID_MainStateHandlerSet[HID_MIAN_STATE_HANDLER_TYPE_NO];
	HID_PM_COMMAND_HANDLER_SET       HID_PMHandlerSet[HID_PM_COMMAND_HANDLER_TYPE_NO];
	HID_MOUSE_SEND_DATA_HANDLER_SET  HID_SendDataHandlerSet[HID_TOTAL_MOUSE_SEND_DATA_HANDLER_SET_NO];
	HID_REMOTE_EVENT_HANDLER_SET 	 HID_HandlerRemoteEventSet[HID_MAIN_REMOTE_EVENT_HANDLER_TYPE_NO];

} HID_NVRAM_TYPE;



#define MODIFY_MAX_SERVICE					3


#if defined ( HID_IIM_SOC ) || defined( APP_MOUSE ) || defined( APP_GAMEPAD ) || defined( APP_KEYBOARD ) || defined( APP_PJ001_MOUSE )
typedef struct
{
	U8	service_name_len;
	U8	service_name[50];
	U8	service_descript_len;
	U8	service_descript[50];
	U8	provider_name_len;
	U8	provider_name[20];
	U8	HID_device_subclass[5];
	U8	HID_country_code[5];
	U8	HID_virtual_cable[5];
	U16  report_descriptor_len;
	U8	HID_boot_device[5];
	U8	reconnect_initial;
	U8	normal_connectable;
	U16  supervision_timeout;
	U16  ssrhostmaxlatency;
	U16  ssrhostmintimeout;
	U8	vendor_ID[6];
	U8	product_ID[6];
	U8	product_version[6];
	U8	num_of_records;
	U8	total_attribute_num[MODIFY_MAX_SERVICE];
	U8	record_base_num;
	U8	attribute_data_base_num;
	U16 attribute_id16;
	U8	attribute_len;
	U8	report_descriptor[250];
	U8	record_data_bank[50];
} SDP_MODIFY_SERVICE_RECORD_BANK;
#else
typedef struct
{
	U8	service_name_len;
	U8	service_name[50];
	U8	service_descript_len;
	U8	service_descript[50];
	U8	provider_name_len;
	U8	provider_name[20];
	U8	HID_device_subclass[5];
	U8	HID_country_code[5];
	U8	HID_virtual_cable[5];
	U16  report_descriptor_len;
	U8	HID_boot_device[5];
	U8	reconnect_initial;
	U8	normal_connectable;
	U16  supervision_timeout;
	U16  ssrhostmaxlatency;
	U16  ssrhostmintimeout;
	U8	vendor_ID[6];
	U8	product_ID[6];
	U8	product_version[6];
	U8	num_of_records;
	U8	total_attribute_num[MODIFY_MAX_SERVICE];
	U8	record_base_num;
	U8	attribute_data_base_num;
	U16 attribute_id16;
	U8	attribute_len;
	U8	report_descriptor[150];
	U8	record_data_bank[50];
} SDP_MODIFY_SERVICE_RECORD_BANK;
#endif








#endif
