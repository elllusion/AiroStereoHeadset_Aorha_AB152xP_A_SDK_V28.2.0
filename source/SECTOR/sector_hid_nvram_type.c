//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifdef HID_SOC

PUBLIC HID_NVRAM_TYPE CODE gHID_nvram_init =
{
	0x04,  //- report_threshold
	0x01,  //- update_Qdata_mode
	0x00,  //- swapXY  0 : no swap  1: swap
	{
		0x00, //X Scale
		0x00, //Y Scale
		0x00  //wheel
	},
	#ifdef APP_PJ001_BPMODULE
	{
		0x01,//X axis negate 
		0x01,//Y axis negate
	},
	#else
	{
		0x00,//X axis negate 
		0x00,//Y axis negate
	},
	#endif
	#ifdef GPIO_CUSTOMER_APP
	0x01,	//- tilt wheel enable
	0x09,   //- report data length
	//0x0A,	//- report data length	
	#else
	0x01,	//- tilt wheel enable
	0x09,	//- report data length
	//0x0A,	//- report data length
	#endif
	{
		0,
	},
	{
		0,
	},
	{
		{												 
			0,
		}
	},
	{
		0,
	},

};

PUBLIC MMI_HID_NVRAM_TYPE CODE gMMI_hid_nvram_init =
{
	4,//PIN_code_length
	{'0','0','0','0',0,0,0,0,0,0,0,0,0,0,0,0},//PIN_code[16];
	{
		#ifdef HID_IIM_SOC
		//LED setting for each main state
		{LED_INVALID, LED_INVALID},	 //MMI_OFF
		{LED_INVALID, LED_INVALID},	 //MMI_INIT
		{LED_INVALID, LED_INVALID},		 //MMI_DISCOVERABLE
		{LED_INVALID, LED_INVALID},		 //MMI_CONNECTING
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_BUSY
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE_2
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_SUSPEND
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_DISCONNECT
		#else
		//LED setting for each main state
		{LED_INVALID, LED_INVALID},	 //MMI_OFF
		//{LED_BON, LED_BOFF},	 //MMI_INIT
		{LED_INVALID, LED_INVALID},	 //MMI_INIT
		//{0x80 , LED_INVALID},	 //MMI_INIT
		{LED_BSFLASH, LED_BOFF},		 //MMI_DISCOVERABLE
		{LED_INVALID, LED_INVALID},		 //MMI_CONNECTING
		#ifdef HID_MOUSE_IOT
		{LED_ROFF, LED_INVALID},	 //MMI_CONNECTING_BUSY
		{LED_RFFLASH, LED_INVALID},	 //MMI_CONNECTING_IDLE
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		{LED_RRAMPFLASH, LED_ROFF},	 //MMI_CONNECTING_SUSPEND
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_BUSY
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_SUSPEND
		#else
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_BUSY
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_IDLE
		{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_SUSPEND
		#endif
		//{LED_INVALID, LED_INVALID},	 //MMI_CONNECTING_DISCONNECT
		{DRIVER_LED_USER_4 ,LED_INVALID},//MMI_SPECIAL_MODE 
		{LED_INVALID, LED_INVALID},		 //MMI_WAIT_POWER_ON_MODE
		//{LED_RSFLASH, LED_ROFF},	 //MMI_CONNECTING_IDLE
		//{LED_RRAMPFLASH, LED_ROFF},	 //MMI_CONNECTING_SUSPEND
		//{LED_BRAMPFLASH, LED_BOFF},	 //MMI_CONNECTING_DISCONNECT
		#endif
	},
	{
		#ifdef HID_IIM_SOC
		LED_INVALID,
		LED_INVALID,
		LED_INVALID,
		LED_INVALID,
		#elif defined( APP_PJ001_MOUSE )
		LED_INVALID,//power_on
		LED_INVALID,//connect
			#ifdef HID_MOUSE_IOT
			LED_ROFF,//self_disconnect
			#else
			LED_BOFF,//self_disconnect
			#endif
		LED_INVALID,//host_abnormal_disconnect
		#else
		DRIVER_LED_USER_0,//power_on
		DRIVER_LED_USER_1,//connect
			#ifdef HID_MOUSE_IOT
			LED_ROFF,//self_disconnect
			#else
			LED_BOFF,//self_disconnect
			#endif
		DRIVER_LED_USER_2,//host_abnormal_disconnect
		#endif	
	},
	0x03,//os_power_saving_enable  // bit 0  power saving all time    bit 1 power saving when busy	 bit 2 power saving when discover
	//0x00,
	#ifdef HID_IIM_SOC
	2, //state_change_in_connect
	(60), //60 Sec// sniffTimerInactive; //in 1 s
	(25),//25 Sec // sniffTimerInidle; //in 1 s
	(25),//25 Sec // sniffTimerInidle_2; //in 1 s
	(25 * 60),//25 min // sniffTimerInsuspend; //in 1 s
	(23),//23 Sec //DISCOVER_MODE_180S_OFF_TO //in 1 s
	//(50),//50 Sec //INITIAL_SELF_CONNECT_OFF_TO	 //in 1 s
	//(50),//50 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO  //in 1 s
	#elif defined (GPIO_CUSTOMER_APP)
	4, //state_change_in_connect
	(2), //2 Sec// sniffTimerInactive; //in 1 s
	(20),//20 Sec // sniffTimerInidle; //in 1 s
	(25),//25 Sec // sniffTimerInidle_2; //in 1 s
	(10 * 60),//10 min // sniffTimerInsuspend; //in 1 s
	//(5), //5 Sec// sniffTimerInactive; //in 1 s
	//(5),//25 Sec // sniffTimerInidle; //in 1 s
	//(5),//25 Sec // sniffTimerInidle; //in 1 s
	//(5),//25 min // sniffTimerInsuspend; //in 1 s
	(180),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
	//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
	//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s
	#elif defined (APP_GAMEPAD)
	4, //state_change_in_connect
	(2), //2 Sec// sniffTimerInactive; //in 1 s
	(20),//20 Sec // sniffTimerInidle; //in 1 s
	(25),//25 Sec // sniffTimerInidle_2; //in 1 s
	(10 * 60),//10 min // sniffTimerInsuspend; //in 1 s
	//(5), //5 Sec// sniffTimerInactive; //in 1 s
	//(5),//25 Sec // sniffTimerInidle; //in 1 s
	//(5),//25 Sec // sniffTimerInidle; //in 1 s
	//(5),//25 min // sniffTimerInsuspend; //in 1 s
	(180),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
	//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
	//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s
	#elif defined (APP_PJ001_MOUSE)
		#ifdef HID_MOUSE_IOT
		3, //state_change_in_connect
		(5), //5 Sec// sniffTimerInactive; //in 1 s
		(5),//25 Sec // sniffTimerInidle; //in 1 s
		(5),//25 Sec // sniffTimerInidle_2; //in 1 s
		(1 * 60),//25 min // sniffTimerInsuspend; //in 25 min	
		//(5), //5 Sec// sniffTimerInactive; //in 1 s
		//(5),//25 Sec // sniffTimerInidle; //in 1 s
		//(1 * 60),//25 min // sniffTimerInsuspend; //in 1 s
		(180),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
		//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
		//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s
	
		#else
		2, //state_change_in_connect
		(5), //5 Sec// sniffTimerInactive; //in 1 s
		(25),//25 Sec // sniffTimerInidle; //in 1 s
		(25),//25 Sec // sniffTimerInidle_2; //in 1 s
		(10 * 60),//25 min // sniffTimerInsuspend; //in 25 min	
		//(5), //5 Sec// sniffTimerInactive; //in 1 s
		//(5),//25 Sec // sniffTimerInidle; //in 1 s
		//(1 * 60),//25 min // sniffTimerInsuspend; //in 1 s
		(300),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
		//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
		//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s
		#endif
	#else
		#ifdef HID_MOUSE_IOT
		3, //state_change_in_connect
		(5), //5 Sec// sniffTimerInactive; //in 1 s
		(5),//25 Sec // sniffTimerInidle; //in 1 s
		(5),//25 Sec // sniffTimerInidle_2; //in 1 s
		(1 * 60),//25 min // sniffTimerInsuspend; //in 25 min	
		//(5), //5 Sec// sniffTimerInactive; //in 1 s
		//(5),//25 Sec // sniffTimerInidle; //in 1 s
		//(1 * 60),//25 min // sniffTimerInsuspend; //in 1 s
		(180),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
		//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
		//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s	
		#else
		3, //state_change_in_connect
		(5), //5 Sec// sniffTimerInactive; //in 1 s
		(25),//25 Sec // sniffTimerInidle; //in 1 s
		(25),//25 Sec // sniffTimerInidle_2; //in 1 s
		(25 * 60),//25 min // sniffTimerInsuspend; //in 25 min	
		//(5), //5 Sec// sniffTimerInactive; //in 1 s
		//(5),//25 Sec // sniffTimerInidle; //in 1 s
		//(1 * 60),//25 min // sniffTimerInsuspend; //in 1 s
		(180),//180 Sec //DISCOVER_MODE_180S_OFF_TO	//in 1 s
		//(5),//5 Sec //INITIAL_SELF_CONNECT_OFF_TO  //in 1 s
		//(5),//5 Sec //INITIAL_NON_CONNECT_POWERDOWN_TO //in 1 s
		#endif
	#endif
	
//	0x00,//no_mmi_read_spi;
//	2,//n_task_round_wakeup_sleep
//	0,//power_saving_duration_in_connect_button_press
//	0,//power_saving_duration_in_discover
//	2,//power_saving_duration_in_idle
//	2,//power_saving_duration_in_idle_2
//	7,//power_saving_duration_in_suspend
//	3,//power_saving_duration_in_off_state
//	3,//power_saving_duration_in_poweron_state
	
	#ifdef HID_IIM_SOC
	0x00,//no_mmi_read_spi;
	20,//n_task_round_wakeup_sleep
	3,//power_saving_duration_in_connect_button_press
	0,//power_saving_duration_in_discover
	150,//power_saving_duration_in_idle
	150,//power_saving_duration_in_idle_2
	150,//power_saving_duration_in_suspend
	150,//power_saving_duration_in_off_state
	150,//power_saving_duration_in_poweron_state
	#else
	0x01,//no_mmi_read_spi;
	20,//n_task_round_wakeup_sleep
	3,//power_saving_duration_in_connect_button_press
	0,//power_saving_duration_in_discover
	150,//power_saving_duration_in_idle
	150,//power_saving_duration_in_idle_2
	150,//power_saving_duration_in_suspend
	//150,//power_saving_duration_in_off_state
	//66,
	3,
	20,//power_saving_duration_in_poweron_state
	#endif

//	0x00,//no_mmi_read_spi;
//	20,//n_task_round_wakeup_sleep
//	3,//power_saving_duration_in_connect_button_press
//	0,//power_saving_duration_in_discover
//	2,//power_saving_duration_in_idle
//	2,//power_saving_duration_in_idle_2
//	7,//power_saving_duration_in_suspend
//	3,//power_saving_duration_in_off_state
//	3,//power_saving_duration_in_poweron_state
	
//	250,//busy_idle_sniff_interval
//	250,//busy_idle_tran_data_interval
//	200,//busy_idle2_sniff_interval
//	250,//busy_idle2_tran_data_interval
//	900,//busy_suspend_sniff_interval
//	900,//busy_suspend_tran_data_interval

//	150,//busy_idle_sniff_interval
//	150,//busy_idle_tran_data_interval
//	150,//busy_idle2_sniff_interval
//	150,//busy_idle2_tran_data_interval
//	500,//busy_suspend_sniff_interval
//	500,//busy_suspend_tran_data_interval

	0,//busy_idle_sniff_interval
	0,//busy_idle_tran_data_interval
	0,//busy_idle2_sniff_interval
	0,//busy_idle2_tran_data_interval
	0,//busy_suspend_sniff_interval
	0,//busy_suspend_tran_data_interval
	
	#ifdef HID_MOUSE_IOT_USE_ICE_NODEEPSLEEP
    0,//power_on_deep_sleep_wait_time;//unit 10ms
	0,//reconnect_deep_sleep_wait_time;//unit 10ms
	0,//disable_powersaving_interval;//unit 10ms
	#elif defined ( APP_PJ001_MOUSE )
	1700,//power_on_deep_sleep_wait_time;//unit 10ms
	200,//reconnect_deep_sleep_wait_time;//unit 10ms
	30,//disable_powersaving_interval;//unit 10ms
	#else
	600,//power_on_deep_sleep_wait_time;//unit 10ms
	200,//reconnect_deep_sleep_wait_time;//unit 10ms
	30,//disable_powersaving_interval;//unit 10ms
	#endif
		
	#ifdef HID_MOUSE_IOT
	0x00000000,//watch_dog_timer
	#else
	0x0000FA00,//watch_dog_timer
	#endif
	//0x0000C80,
	{//MMI_SNIFF_INTERVAL_PARA
		{ 0x10 , 0x00 , 0x10 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00 },	//Busy	   sniff max interval : 00	sniff min interval : 12	  sniff attempt : 1   sniff timeout : 0 
		//{ 0x0C , 0x00 , 0x0C , 0x00 , 0x01 , 0x00 , 0x00 , 0x00 },	//Busy	   sniff max interval : 00	sniff min interval : 12	  sniff attempt : 1   sniff timeout : 0 
		{ 0x6c , 0x00 , 0x6c , 0x00 , 0x02 , 0x00 , 0x01 , 0x00 }, //Idle	   sniff max interval : 00	sniff min interval : 6C	  sniff attempt : 2   sniff timeout : 1
		//{ 0x18 , 0x00 , 0x18 , 0x00 , 0x01 , 0x00 , 0x01 , 0x00 },
		{ 0x32 , 0x00 , 0x32 , 0x00 , 0x01 , 0x00 , 0x01 , 0x00 },
		//{ 0x6c , 0x00 , 0x6c , 0x00 , 0x02 , 0x00 , 0x01 , 0x00 },		
		//{ 0xe0 , 0x01 , 0xe0 , 0x01 , 0x02 , 0x00 , 0x05 , 0x00 },	//Suspend  sniff max interval : 00	sniff min interval : 1e0	  sniff attempt : 2   sniff timeout : 5	
		{ 0xa0 , 0x00 , 0xa0 , 0x00 , 0x01 , 0x00 , 0x05 , 0x00 },	//Suspend  sniff max interval : 00	sniff min interval : 1e0	  sniff attempt : 2   sniff timeout : 5	
	},
	{
		0xe0 , 0x01 , 0x20 , 0x4E , 0x20 , 0x4E 	
	},
	{
		#ifdef HID_IIM_SOC
		15,
		15,
		//3,//normal_reconnect_timeout	 unit Second
		//3,//abnormal_reconnect_timeout;//unit Second
		15,//vc_unplug_timeout;//unit Second
		0,//normal_reconnect_retry_count;
		//2,//abnormal_reconnect_retry_count;
		//4,
		5,
		0x4B,
		#elif defined (GPIO_CUSTOMER_APP)
		15,//normal_reconnect_timeout	 unit Second
		15,//abnormal_reconnect_timeout;//unit Second
		15,//vc_unplug_timeout;//unit Second
		1,//normal_reconnect_retry_count;
		1,//abnormal_reconnect_retry_count;
		0xEB,//reconnect_default_para;//bit 0 Attempt To Reconnect On Abnormal Disconnect :1 
								//bit 1 Default Value Of Reconnect Initiate	:1
							  //bit 2 Default Value Of Normally Connectable :0
							  //bit 3 Default Value Of New :1
							  //bit 4 Exit Discoverable On Authentication Failure :0
							  //bit 5 Generate VC unplug on connect button press  :1
							  //bit 6 Become discoverable on connect button press :1
							  //bit 7 Power On Reconnect:1
		#elif defined (APP_GAMEPAD)
		15,//normal_reconnect_timeout	 unit Second
		15,//abnormal_reconnect_timeout;//unit Second
		15,//vc_unplug_timeout;//unit Second
		1,//normal_reconnect_retry_count;
		1,//abnormal_reconnect_retry_count;
		0xEB,//reconnect_default_para;//bit 0 Attempt To Reconnect On Abnormal Disconnect :1 
								//bit 1 Default Value Of Reconnect Initiate	:1
							  //bit 2 Default Value Of Normally Connectable :0
							  //bit 3 Default Value Of New :1
							  //bit 4 Exit Discoverable On Authentication Failure :0
							  //bit 5 Generate VC unplug on connect button press  :1
							  //bit 6 Become discoverable on connect button press :1
							  //bit 7 Power On Reconnect:1
		#else
		15,//normal_reconnect_timeout	 unit Second
		15,//abnormal_reconnect_timeout;//unit Second
		//15,//vc_unplug_timeout;//unit Second
		5,//vc_unplug_timeout;//unit Second
		1,//normal_reconnect_retry_count;
		0,//abnormal_reconnect_retry_count;
		0xEA,//reconnect_default_para;//bit 0 Attempt To Reconnect On Abnormal Disconnect :0 
								//bit 1 Default Value Of Reconnect Initiate	:1
							  //bit 2 Default Value Of Normally Connectable :0
							  //bit 3 Default Value Of New :1
							  //bit 4 Exit Discoverable On Authentication Failure :0
							  //bit 5 Generate VC unplug on connect button press  :1
							  //bit 6 Become discoverable on connect button press :1
							  //bit 7 Power On Reconnect:1
		#endif
	},		
	#ifdef HID_IIM_SOC
	{ 
		0x00,//wait_power_on_led_enable
		0x00,//wait_power_on_led_time//unit Second
	},
	0xff,//allow_event_reconnect_mask
	0x00,//MMI_CMD_PARA_PAGE_SCAN_MODE_R0	
	MMI_SNIFF_SUBRATING_DISABLE,//sniff_subrating_disable
	//SYS_HID_IIM_TYPE,//mmi_hid_module_type
	#elif defined (GPIO_CUSTOMER_APP)
	{ 
		0x01,//wait_power_on_led_enable
		0x05,//wait_power_on_led_time//unit Second
	},
	0xff,//allow_event_reconnect_mask
	0x01,//MMI_CMD_PARA_PAGE_SCAN_MODE_R1
	MMI_SNIFF_SUBRATING_ENABLE,//sniff_subrating_enable
	//SYS_HID_MOUSE_TYPE,//mmi_hid_module_type	
	#elif defined (APP_GAMEPAD)
	{ 
		0x01,//wait_power_on_led_enable
		0x05,//wait_power_on_led_time//unit Second
	},
	0xff,//allow_event_reconnect_mask
	0x01,//MMI_CMD_PARA_PAGE_SCAN_MODE_R1
	MMI_SNIFF_SUBRATING_ENABLE,//sniff_subrating_enable
	//SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	#else
	{ 
		0x00,//wait_power_on_led_enable
		0x00,//wait_power_on_led_time//unit Second
	},
	0xff,//allow_event_reconnect_mask
	0x01,//MMI_CMD_PARA_PAGE_SCAN_MODE_R1
	MMI_SNIFF_SUBRATING_ENABLE,//sniff_subrating_enable
	//SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	#endif
	{    //MMI_UsrDefLed
		//User-defined LED Data, 6 sets, 3 bytes in each sets
		{
			//0x00, 0x00, 0x00
			//0x38, 0x30, 0x03//led on 5 seconds
		    #ifdef APP_PJ001_MOUSE
			0xB8, 0x40, 0x03//led on 5 seconds
			#else
			0x78, 0x30, 0x03//led on 5 seconds
			#endif
		},	   		
		{
			0x50, 0x00, 0x03//led on 280ms
		},		
		{
			0x60, 0x00, 0x03//led on 560ms
		},		
		{
			0x50, 0x00, 0x03//led on 280ms
		},		
		{
			0x52, 0x10, 0x03//blinks twice (280 ms each time)
		},		
		{
			0x52, 0x20, 0x03//blinks three times  (280 ms each time)
		},
		{
			0x00, 0x00, 0x00
		},		
		{
			0x00, 0x00, 0x00
		},		
		{
			0x00, 0x00, 0x00
		},		
		{
			0x00, 0x00, 0x00
		},
	},
	{
		0x03,//io_capability no input no output
		0x00,//oob_data_present 	
		0x00,//auth_requirments
	},
	{ 	0x18,//packet_type_L 
		0x00,//packet_type_H 
	},			
	#ifdef HID_IIM_SOC
	{	
		60536,//timer_setting //count to 65536   65536-60536 = ( 5000 * ( 1 / (12M/12) )  ) = 5ms
		MMI_TIMER_READ_SENSOR,//sniff_in_read_enable
		0x10270000L,//qos_latency
	},
	0x10, //only_sdp_reconnect_flag//bit 0 MMI_ONLY_SDP_RECONNECT :0 
									//bit 1 MMI_OFF_CONNECT_BUTTON_RECONNECT	:0
									//bit 2 MMI_CONNECT_BUTTON_ALWAYS_CHECK     :0
									//bit 3 MMI_POWER_ON_DISCOVERABLE			:0
									//bit 4 MMI_OFF_ACTIVE_GPIO_INT_ENABLE		:1
									//bit 5 MMI_SUSPEND_ACTIVE_GPIO_INT_ENABLE  :0
	0x03,
	#elif defined (GPIO_CUSTOMER_APP)
	{	
		63536,//timer_setting //count to 65536   65536-63536 = ( 2000 * ( 1 / (12M/12) )  ) = 2ms	 mouse 2ms get data
		MMI_SNIFF_IN_READ_SENSOR,//sniff_in_read_enable
		0x204E0000L,//qos_latency
	},
	0x32, //only_sdp_reconnect_flag//bit 0 MMI_ONLY_SDP_RECONNECT :0 
									//bit 1 MMI_OFF_CONNECT_BUTTON_RECONNECT	:1
									//bit 2 MMI_CONNECT_BUTTON_ALWAYS_CHECK     :0
									//bit 3 MMI_POWER_ON_DISCOVERABLE			:0
									//bit 4 MMI_OFF_ACTIVE_GPIO_INT_ENABLE		:1
									//bit 5 MMI_SUSPEND_ACTIVE_GPIO_INT_ENABLE  :1
	0x02,//mmi_num_main_event_handle
	#elif defined (APP_GAMEPAD)
	{	
		63536,//timer_setting //count to 65536   65536-63536 = ( 2000 * ( 1 / (12M/12) )  ) = 2ms	 mouse 2ms get data
		MMI_SNIFF_IN_READ_SENSOR,//sniff_in_read_enable
		0x204E0000L,//qos_latency
	},
	0x32, //only_sdp_reconnect_flag//bit 0 MMI_ONLY_SDP_RECONNECT :0 
									//bit 1 MMI_OFF_CONNECT_BUTTON_RECONNECT	:1
									//bit 2 MMI_CONNECT_BUTTON_ALWAYS_CHECK     :0
									//bit 3 MMI_POWER_ON_DISCOVERABLE			:0
									//bit 4 MMI_OFF_ACTIVE_GPIO_INT_ENABLE		:1
									//bit 5 MMI_SUSPEND_ACTIVE_GPIO_INT_ENABLE  :1
	0x02,//mmi_num_main_event_handle
	#elif defined (APP_PJ001_MOUSE)
	{	
		60536,//timer_setting //count to 65536   65536-63536 = ( 2000 * ( 1 / (12M/12) )  ) = 2ms	 mouse 2ms get data
		MMI_SNIFF_IN_READ_SENSOR,//sniff_in_read_enable
		//MMI_TIMER_READ_SENSOR,//sniff_in_read_enable
		0x204E0000L,//qos_latency
	},
	0x32, //only_sdp_reconnect_flag//bit 0 MMI_ONLY_SDP_RECONNECT :0 
									//bit 1 MMI_OFF_CONNECT_BUTTON_RECONNECT	:1
									//bit 2 MMI_CONNECT_BUTTON_ALWAYS_CHECK     :0
									//bit 3 MMI_POWER_ON_DISCOVERABLE			:0
									//bit 4 MMI_OFF_ACTIVE_GPIO_INT_ENABLE		:1
									//bit 5 MMI_SUSPEND_ACTIVE_GPIO_INT_ENABLE  :1
	0x03,//mmi_num_main_event_handle
	#else
	{	
		63536,//timer_setting //count to 65536   65536-63536 = ( 2000 * ( 1 / (12M/12) )  ) = 2ms	 mouse 2ms get data
		MMI_SNIFF_IN_READ_SENSOR,//sniff_in_read_enable
		//MMI_TIMER_READ_SENSOR,//sniff_in_read_enable
		0x204E0000L,//qos_latency
	},
	0x32, //only_sdp_reconnect_flag//bit 0 MMI_ONLY_SDP_RECONNECT :0 
									//bit 1 MMI_OFF_CONNECT_BUTTON_RECONNECT	:1
									//bit 2 MMI_CONNECT_BUTTON_ALWAYS_CHECK     :0
									//bit 3 MMI_POWER_ON_DISCOVERABLE			:0
									//bit 4 MMI_OFF_ACTIVE_GPIO_INT_ENABLE		:1
									//bit 5 MMI_SUSPEND_ACTIVE_GPIO_INT_ENABLE  :1
	0x02,//mmi_num_main_event_handle
	#endif
	{
		0,
	},
	{		
		0,				
	},
	{
		0,
	},
	{
		0,
	},
	{
		0,
	},
	{
		0,		
	},
};

#else

static HID_NVRAM_TYPE CODE gHID_nvram_init =
{
	0
};

static U8 CODE gMMI_hid_nvram_init = 
{
 	0
};

#endif
