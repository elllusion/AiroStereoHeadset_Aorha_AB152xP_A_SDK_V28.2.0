//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifdef HID_SOC

PUBLIC DRIVER_NVRAM_TYPE CODE gDRIVER_nvram_init =
{
	#ifdef HID_IIM_SOC
	16,//gpio_poll_period 5ms  16 * 0x0003125 = 5
	#else
	32,//gpio_poll_period 10ms  32 * 0x0003125 = 10
	#endif
	30,//keyPressTime // in unit 0.1Seconds
	100,//keyLongPressTime // in unit 0.1Seconds
	10,//ledSFlashTime
	2,//ledFFlashTime
	0x00,//sensor_action_gpio_value;
	0x08,//sensor_noaction_gpio_value;
	//10,//discoverable_led_stop_time;  in unit Seconds
	0,
	0x02,//buck_always_on  //buck off  32k on
	{
		SPI_INIT_PIXART3204DB,//sensor_type
		SPI_PIXART_3204_PRODUCT_ID1,//sensor_pid
		1,//cpi
		0x72,//sleep1_setting tsiw sleep1  4ms
		//0x92,//sleep2_setting tsiw sleep2  32ms
		0x02,//sleep2_setting tsiw sleep2  32ms
		0x12,//enter_time Enter time sleep1 2048ms    sleep2 20480ms
		0x92,//rest2_rate
		0x00,//rest2_downshift
		0x00,//rest3_rate		
		0xC0,//laser_ctrl0 
		0x80,//laserpower_cfg0
		0x06,//spi_hold
		0x33,//spi_setting
		0xC1,//spi_opmode
		
//		SPI_INIT_PIXART3607,//sensor_type
//		SPI_PIXART_3204_PRODUCT_ID1,//sensor_pid
//		2,//cpi
//		0x02,//sleep1_setting tsiw sleep1  4ms
//		0x02,//sleep2_setting tsiw sleep2  32ms
//		0xF0,//enter_time Enter time sleep1 2048ms    sleep2 20480ms
//		0x02,//rest2_rate //Port 
//		0x08,//rest2_downshift //Value Begin
//		0x00,//rest3_rate //Value End		
//		//0xC0,//laser_ctrl0 
//		0x00,//laser_ctrl0 for future to change sleep2_setting tsiw
//		0x80,//laserpower_cfg0
//		0x06,//spi_hold
//		0x33,//spi_setting
//		0xC1,//spi_opmode
		
//		SPI_INIT_AVAGO7550,//sensor_type
//		SPI_AVAGO_7530_PRODUCT_ID,//sensor_pid
//		2,//cpi
//		0x00,//default 0x04   run_downshift no setting otherwise neccessery
//		0x01,//rest1_rate
//		0x1F,//rest1_downshift   
//		0x09,//rest2_rate
//		0x2F,//rest2_downshift
//		0x31,//rest3_rate		
//		0xC0,//laser_ctrl0 
//		0x5E,//laserpower_cfg0
//		0x06,//spi_hold
//		0x35,//old 0x34 spi_setting
//		0x01,//spi_opmode
		
//		SPI_INIT_AVAGO5030,//sensor_type
//		SPI_AVAGO_7530_PRODUCT_ID,//sensor_pid
//		2,//cpi
//		0x04,//run_downshift
//		0x00,//rest1_rate //for LED Current Setting 0x40 default high
//		0x1F,//rest1_downshift   
//		0x02,//rest2_rate //Port
//		0x08,//rest2_downshift //Value Begin
//		0x31,//rest3_rate		
//		0xC0,//laser_ctrl0 
//		0x80,//laserpower_cfg0
//		0x06,//spi_hold
//		0x34,//spi_setting
//		0x01,//spi_opmode

//		SPI_INIT_AVAGO5090,//sensor_type
//		SPI_AVAGO_7530_PRODUCT_ID,//sensor_pid
//		2,//cpi
//		0x00,//default 0x46 no setting run_downshift
//		0x01,//rest1_period
//		0x1F,//rest1_downshift   
//		0x09,//rest2_period
//		0x2F,//rest2_downshift
//		0x31,//rest3_rate		
//		0x00,//laser_ctrl0 	LED ctrl
//		0x00,//default 0x08,laserpower_cfg0 Auto LED ctrl
//		0x06,//spi_hold
//		0x34,//spi_setting
//		0x01,//spi_opmode

//		SPI_INIT_NEWSENSOR,//sensor_type
//		SPI_PIXART_3204_PRODUCT_ID1,//sensor_pid
//		1,//cpi
//		0x72,//sleep1_setting tsiw sleep1  4ms
//		//0x92,//sleep2_setting tsiw sleep2  32ms
//		0x02,//sleep2_setting tsiw sleep2  32ms
//		0x12,//enter_time Enter time sleep1 2048ms    sleep2 20480ms
//		0x92,//rest2_rate
//		0x00,//rest2_downshift
//		0x00,//rest3_rate		
//		0xC0,//laser_ctrl0 
//		0x80,//laserpower_cfg0
//		0x06,//spi_hold
//		0x33,//spi_setting
//		0xC1,//spi_opmode

//		SPI_INIT_NOSENSOR,//sensor_type
//		0x00,//sensor_pid
//		2,//cpi
//		0x00,//default 0x46 no setting run_downshift
//		0x01,//rest1_period
//		0x1F,//rest1_downshift   
//		0x09,//rest2_period
//		0x2F,//rest2_downshift
//		0x31,//rest3_rate		
//		0x00,//laser_ctrl0 	LED ctrl
//		0x00,//default 0x08,laserpower_cfg0 Auto LED ctrl
//		0x06,//spi_hold
//		0x34,//spi_setting
//		0x01,//spi_opmode
	},
	#ifdef HID_IIM_SOC  // For IIM
	{
		// IO Bit Map : 1 => Output Enable, 0 => Input enable
//		0x30,//gpio_oe_0
//		0x40,//gpio_oe_1
//		0xE4,//gpio_oe_2
//		0x08,//gpio_oe_3
//
//		// IO Config : Pull High Button Inverse Enable
//		0xCF,//gpio_i_inv_en_0
//		0x81,//gpio_i_inv_en_1
//		0x13,//gpio_i_inv_en_2
//		0x03,//gpio_i_inv_en_3
//
//		// IO Mux Enable
//		0x00,//gpio_mux
//
//		// Input Interrupt Enbale
//		0x00,//interrupt_e_0    0' wake up from deep sleep
//		0x00,//interrupt_e_1
//		0x00,//interrupt_e_2
//		0x00,//interrupt_e_3

		// Input Pin Mask
		0x018F,//gpio_mask_L
		0x07F1,//gpio_mask_H
		//0x000F,//gpio_mask_L
		//0x0001,//gpio_mask_H
		0xDF9F,//gpio_key_mask
		0xDF9F,//gpio_hid_key_mask
		
		// Input Pin Mask
		//0x81CF,//gpio_mask_L
		//0x031B,//gpio_mask_H
		//0x3BFF,//gpio_key_mask
		//0x3BFF,//gpio_hid_key_mask
	},
	14,
	{
		{ GPIO_0 ,  btn_Pwr },		// PWR	  OK
		{ GPIO_1 ,  btn_Sync },		// SYNC	  OK
		{ GPIO_2 ,  btn_A },		// A
		{ GPIO_3 ,  btn_Up },		// UP	  OK
		{ GPIO_7 ,  btn_Plus },		// PLUS	  OK
		{ GPIO_8 ,  btn_One },		// One	  OK	
		{ GPIO_16 , btn_NCK_DEC },		// NCH_DET 	  OK
		{ GPIO_20 , btn_Down },		// Down		  OK
		{ GPIO_21 , btn_Minus },		// Minus	  OK
		{ GPIO_22 , btn_Home },		// HOME		  OK
		{ GPIO_23 , btn_Right },		// Right	  OK
		{ GPIO_24 , btn_Two },		// Two		  OK
		{ GPIO_25 , btn_B  },		// B		  OK
		{ GPIO_26 , btn_Left },		// LEFT		  OK
		//Don't Care
		{ GPIO_0 ,  0x0001 },	//
		{ GPIO_0 ,  0x0001 },	//
	},	
//	{
//		{ GPIO_0 ,  btn_Pwr },		// PWR
//		{ GPIO_1 ,  btn_Sync },		// SYNC
//		{ GPIO_2 ,  btn_A },		// A
//		{ GPIO_3 ,  btn_Up },		// UP
//		{ GPIO_6 ,  btn_Right },	// RIGHT
//		{ GPIO_7 ,  btn_Two },		// TWO
//		{ GPIO_8 ,  btn_B },		// B
//		{ GPIO_15 , btn_Plus },		// PLUS
//		{ GPIO_16 , btn_Down },		// DOWN
//		{ GPIO_17 , btn_Minus },	// MINUS
//		{ GPIO_19 , btn_NCK_DEC },	// NCH_DET 
//		{ GPIO_20 , btn_Home },		// HOME
//		{ GPIO_24 , btn_One },		// ONE
//		{ GPIO_25 , btn_Left },		// LEFT
//		//Don't Care
//		{ GPIO_0 ,  0x0001 },	//
//		{ GPIO_0 ,  0x0001 },	//
//
//	},
	LED_INVALID,//connect_button_led_setting
	10 * 320,//connect_button_press_time	// in unit 0.1Seconds
	//0x8000,//connect_button_mask means no use
	0x0020,
	0x00,//driver_num_gpio_press_mask
	{
		0x0003,
		0x000C,
	},
	{//DRIVER_TILT_WHEEL_TABLE
		0x00,
		0x0000,
		0x0000,
	},
	{//DRIVER_WHEEL_TABLE
		0x00,//z_axis_sel
		0x03,//z_axis_enable
	},
	{//DRIVER_DPI_TABLE
		0x00,//mode
		0x00,//dpi_level
		{ 0x00, 0x00, 0x00 },//dpi_register_setting[3]
		{ 0x00, 0x00, 0x00 },//dpi_led_setting[3]
		0x0000,//dpi_gpio
	},
	{
//		0x00,//mode
//		0x00,//ledsetting
//		0x00,//measure_interval
//		0x00,//num_instances_average
//		0x0258,//full_voltage
//		0x0000,//low_battery_voltage
//		0x01B4,//shut_down_voltage
//		0x0000,//init_ok_voltage

		0x00,//mode
		LED_INVALID,//ledsetting
		0x00,//measure_interval
		0x00,//num_instances_average
		0x02A0,//full_voltage
		0x0140,//low_battery_voltage
		0x0140,//shut_down_voltage
		0x0150,//init_ok_voltage
	},
	{
	 	0x00,//special_mode_enable
		0x0000,//special_mode_gpio
		0x00,//power_up_key_L;
		0x00,//power_down_key_L;
		0x00,//power_up_press_num;
		0x00,//power_down_press_num;
		0x00,//power_save_press_num
		LED_INVALID,//power_up_1_led
		LED_INVALID,//power_down_1_led
		LED_INVALID,//power_up_10_led
		LED_INVALID,//power_down_10_led
		LED_INVALID,//power_save_led
	},	
	SYS_HID_IIM_TYPE,//mmi_hid_module_type
	{	//mmi_shut_down_rf_para
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
	0,//dpi_click_change_time;//unit 100ms
	{   0,  },//firmware_version[6];
	0x04,//driver_num_main_action_handle	
	#elif defined (GPIO_CUSTOMER_APP)
	{
//		0x00,//gpio_oe_0
//		0x00,//gpio_oe_1
//		0x08,//gpio_oe_2//GPIO 19 is output for pixart 3607 init
//		0x00,//gpio_oe_3
//		0xC7,//gpio_i_inv_en_0
//		0x01,//gpio_i_inv_en_1
//		0x10,//gpio_i_inv_en_2
//		0x03,//gpio_i_inv_en_3
//		0x12,//gpio_mux
//		0xf0,//interrupt_e_0
//		0xff,//interrupt_e_1
//		0xff,//interrupt_e_2
//		0xff,//interrupt_e_3
		0x01C7,//gpio_mask_L
		0x0310,//gpio_mask_H
		0x01ff,//gpio_key_mask //lower 6 key
		0x001f,//gpio_hid_key_mask		 	
	},
	9,
	{
		{ GPIO_0 ,  0x0001 },	
		{ GPIO_1 ,  0x0002 },
		{ GPIO_2 ,  0x0020 },
		{ GPIO_6 ,  0x0004 },		
		{ GPIO_7 ,  0x0008 },
		{ GPIO_8 ,  0x0010 },
		{ GPIO_20,  0x0040 },
		{ GPIO_24 , 0x0080 },
		{ GPIO_25 , 0x0100 },
		// Don't Care
		{ GPIO_16 , 0x0100 },
		{ GPIO_17 , 0x0200 },
		{ GPIO_19 , 0x0400 },
		{ GPIO_20 , 0x0800 },
		{ GPIO_24 , 0x1000 },		
		{ GPIO_0 ,  0x0001 },
		{ GPIO_0 ,  0x0001 },
	},
	LED_INVALID,//connect_button_led_setting
	10 * 320,//connect_button_press_time	// in unit 0.1Seconds
	0x0020,//connect_button_mask
	0x02,//driver_num_gpio_press_mask
	{
		0x0006,
		0x000C,
	},
	{//DRIVER_TILT_WHEEL_TABLE
		0x01,
		0x0040,
		0x0080,
	},
	{//DRIVER_WHEEL_TABLE
		0x00,//z_axis_sel
		0x03,//z_axis_enable
	},
	{//DRIVER_DPI_TABLE
		DPI_MODE_BUTTON_PRESS,//mode
		//DPI_MODE_BUTTON_CLICK,//mode
		0x03,//dpi_level
		{ SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1200, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		//{ SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI500 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 },//dpi_register_setting[3]
		//{ SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI400 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI800 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		{ DRIVER_LED_USER_3, DRIVER_LED_USER_4, DRIVER_LED_USER_5, },//dpi_led_setting[3]
		0x0000,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO   , DPI_MODE_BUTTON_PRESS : num of gpio_press_mask be pressed
		//0x0100,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO
	},
	{
		0x00,//mode
		0x00,//ledsetting
		0x00,//measure_interval
		0x00,//num_instances_average
		0x0000,//full_voltage
		0x0000,//low_battery_voltage
		0x0000,//shut_down_voltage
		0x0000,//init_ok_voltage
	},
	{
	 	0x01,//special_mode_enable
		0x0020,//special_mode_gpio
		0x00,//power_up_key_L;
		0x00,//power_down_key_L;
		0x00,//power_up_press_num;
		0x00,//power_down_press_num;
		0x00,//power_save_press_num
		LED_INVALID,//power_up_1_led
		LED_INVALID,//power_down_1_led
		LED_INVALID,//power_up_10_led
		LED_INVALID,//power_down_10_led
		LED_INVALID,//power_save_led
	},
	SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	{	//mmi_shut_down_rf_para
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
	10,//dpi_click_change_time;//unit 100ms
	{   0,  },//firmware_version[6];
	0x07,//driver_num_main_action_handle	
	
	#elif defined (APP_GAMEPAD)
	
	{
//		0x00,//gpio_oe_0
//		0x00,//gpio_oe_1
//		0x08,//gpio_oe_2//GPIO 19 is output for pixart 3607 init
//		0x00,//gpio_oe_3
//		0xC7,//gpio_i_inv_en_0
//		0x01,//gpio_i_inv_en_1
//		0x10,//gpio_i_inv_en_2
//		0x03,//gpio_i_inv_en_3
//		0x12,//gpio_mux
//		0xf0,//interrupt_e_0
//		0xff,//interrupt_e_1
//		0xff,//interrupt_e_2
//		0xff,//interrupt_e_3
		0x01C7,//gpio_mask_L
		0x0310,//gpio_mask_H
		0x01ff,//gpio_key_mask //lower 6 key
		0x001f,//gpio_hid_key_mask		 	
	},
	9,
	{
		{ GPIO_0 ,  0x0001 },	
		{ GPIO_1 ,  0x0002 },
		{ GPIO_2 ,  0x0020 },
		{ GPIO_6 ,  0x0004 },		
		{ GPIO_7 ,  0x0008 },
		{ GPIO_8 ,  0x0010 },
		{ GPIO_20,  0x0040 },
		{ GPIO_24 , 0x0080 },
		{ GPIO_25 , 0x0100 },
		// Don't Care
		{ GPIO_16 , 0x0100 },
		{ GPIO_17 , 0x0200 },
		{ GPIO_19 , 0x0400 },
		{ GPIO_20 , 0x0800 },
		{ GPIO_24 , 0x1000 },		
		{ GPIO_0 ,  0x0001 },
		{ GPIO_0 ,  0x0001 },
	},
	LED_INVALID,//connect_button_led_setting
	10 * 320,//connect_button_press_time	// in unit 0.1Seconds
	0x0020,//connect_button_mask
	0x02,//driver_num_gpio_press_mask
	{
		0x0006,
		0x000C,
	},
	{//DRIVER_TILT_WHEEL_TABLE
		0x01,
		0x0040,
		0x0080,
	},
	{//DRIVER_WHEEL_TABLE
		0x00,//z_axis_sel
		0x03,//z_axis_enable
	},
	{//DRIVER_DPI_TABLE
		DPI_MODE_BUTTON_PRESS,//mode
		//DPI_MODE_BUTTON_CLICK,//mode
		0x03,//dpi_level
		{ SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1200, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		//{ SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI500 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 },//dpi_register_setting[3]
		//{ SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI400 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI800 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		{ DRIVER_LED_USER_3, DRIVER_LED_USER_4, DRIVER_LED_USER_5, },//dpi_led_setting[3]
		0x0000,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO   , DPI_MODE_BUTTON_PRESS : num of gpio_press_mask be pressed
		//0x0100,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO
	},
	{
		0x00,//mode
		0x00,//ledsetting
		0x00,//measure_interval
		0x00,//num_instances_average
		0x0000,//full_voltage
		0x0000,//low_battery_voltage
		0x0000,//shut_down_voltage
		0x0000,//init_ok_voltage
	},
	{
	 	0x01,//special_mode_enable
		0x0020,//special_mode_gpio
		0x00,//power_up_key_L;
		0x00,//power_down_key_L;
		0x00,//power_up_press_num;
		0x00,//power_down_press_num;
		0x00,//power_save_press_num
		LED_INVALID,//power_up_1_led
		LED_INVALID,//power_down_1_led
		LED_INVALID,//power_up_10_led
		LED_INVALID,//power_down_10_led
		LED_INVALID,//power_save_led
	},
	SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	{	//mmi_shut_down_rf_para
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
	10,//dpi_click_change_time;//unit 100ms
	{   0,  },//firmware_version[6];
	0x07,//driver_num_main_action_handle

	#elif defined (APP_PJ001_MOUSE)
	
		#ifdef HID_MOUSE_IOT
		{
			0x01C7,//gpio_mask_L
			0x0710,//gpio_mask_H
			0x01ff,//gpio_key_mask //lower 6 key
			0x001f,//gpio_hid_key_mask		 	
		},
		4,
		{
			{ GPIO_0 ,  0x0001 },	
			{ GPIO_1 ,  0x0002 },
			{ GPIO_2 ,  0x0020 },
			{ GPIO_6 ,  0x0004 },		
			{ GPIO_7 ,  0x0008 },
			{ GPIO_8 ,  0x0010 },
			{ GPIO_20,  0x0040 },
			{ GPIO_24 , 0x0080 },
			{ GPIO_25 , 0x0100 },
			// Don't Care
			{ GPIO_16 , 0x0100 },
			{ GPIO_17 , 0x0200 },
			{ GPIO_19 , 0x0400 },
			{ GPIO_20 , 0x0800 },
			{ GPIO_24 , 0x1000 },		
			{ GPIO_0 ,  0x0001 },
			{ GPIO_0 ,  0x0001 },
		},
	
		#else
		#ifdef APP_PJ001_BPMODULE
		{
			0x0007,//gpio_mask_L
			0x0400,//gpio_mask_H
			0x01ff,//gpio_key_mask //lower 6 key
			0x001f,//gpio_hid_key_mask		 	
		},
		4,
		{
			{ GPIO_1 ,  0x0001 },	
			{ GPIO_26 ,  0x0002 },
			{ GPIO_0 ,  0x0020 },
			{ GPIO_2 ,  0x0004 },		
			{ GPIO_7 ,  0x0008 },
			{ GPIO_8 ,  0x0010 },
			{ GPIO_20,  0x0040 },
			{ GPIO_24 , 0x0080 },
			{ GPIO_25 , 0x0100 },
			// Don't Care
			{ GPIO_16 , 0x0100 },
			{ GPIO_17 , 0x0200 },
			{ GPIO_19 , 0x0400 },
			{ GPIO_20 , 0x0800 },
			{ GPIO_24 , 0x1000 },		
			{ GPIO_0 ,  0x0001 },
			{ GPIO_0 ,  0x0001 },
		},
		#else
		{
			0x01C7,//gpio_mask_L
			0x0310,//gpio_mask_H
			0x01ff,//gpio_key_mask //lower 6 key
			0x001f,//gpio_hid_key_mask		 	
		},
		4,
		{
			{ GPIO_0 ,  0x0001 },	
			{ GPIO_1 ,  0x0002 },
			{ GPIO_2 ,  0x0020 },
			{ GPIO_6 ,  0x0004 },		
			{ GPIO_7 ,  0x0008 },
			{ GPIO_8 ,  0x0010 },
			{ GPIO_20,  0x0040 },
			{ GPIO_24 , 0x0080 },
			{ GPIO_25 , 0x0100 },
			// Don't Care
			{ GPIO_16 , 0x0100 },
			{ GPIO_17 , 0x0200 },
			{ GPIO_19 , 0x0400 },
			{ GPIO_20 , 0x0800 },
			{ GPIO_24 , 0x1000 },		
			{ GPIO_0 ,  0x0001 },
			{ GPIO_0 ,  0x0001 },
		},
		#endif
		#endif
	LED_INVALID,//connect_button_led_setting
	10 * 320,//connect_button_press_time	// in unit 0.1Seconds
	0x0020,//connect_button_mask
	0x02,//driver_num_gpio_press_mask
	{
		0x0006,
		0x000C,
	},
	{//DRIVER_TILT_WHEEL_TABLE
		0x00,
		0x0040,
		0x0080,
	},
	{//DRIVER_WHEEL_TABLE
		0x00,//z_axis_sel
		0x03,//z_axis_enable
	},
	{//DRIVER_DPI_TABLE
		//DPI_MODE_BUTTON_PRESS,//mode
		DPI_MODE_BUTTON_CLICK,//mode
		0x02,//dpi_level
		{ SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600 },
		//{ SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1200, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		//{ SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI500 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 },//dpi_register_setting[3]
		//{ SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI400 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI800 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		{ DRIVER_LED_USER_3, DRIVER_LED_USER_4, DRIVER_LED_USER_5, },//dpi_led_setting[3]
		0x0006,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO   , DPI_MODE_BUTTON_PRESS : num of gpio_press_mask be pressed
		//0x0100,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO
	},
	{
		0x00,//mode
		0x00,//ledsetting
		0x00,//measure_interval
		0x00,//num_instances_average
		0x0000,//full_voltage
		0x0000,//low_battery_voltage
		0x0000,//shut_down_voltage
		0x0000,//init_ok_voltage
	},
	{
	 	0x00,//special_mode_enable
		0x0020,//special_mode_gpio
		0x00,//power_up_key_L;
		0x00,//power_down_key_L;
		0x00,//power_up_press_num;
		0x00,//power_down_press_num;
		0x00,//power_save_press_num
		LED_INVALID,//power_up_1_led
		LED_INVALID,//power_down_1_led
		LED_INVALID,//power_up_10_led
		LED_INVALID,//power_down_10_led
		LED_INVALID,//power_save_led
	},
	SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	{	//mmi_shut_down_rf_para
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
	10,//dpi_click_change_time;//unit 100ms
	{   0,  },//firmware_version[6];
	0x07,//driver_num_main_action_handle
	
	#else  // For Mouse ROM
	
		#ifdef HID_MOUSE_IOT
		{
	//		0x00,//gpio_oe_0
	//		0x00,//gpio_oe_1
	//		0x08,//gpio_oe_2//GPIO 19 is output for pixart 3607 init
	//		0x00,//gpio_oe_3
	//		0xC7,//gpio_i_inv_en_0
	//		0x01,//gpio_i_inv_en_1
	//		0x10,//gpio_i_inv_en_2
	//		0x03,//gpio_i_inv_en_3
	//		//0x00,
	//		0x12,//gpio_mux
	//		0xf0,//interrupt_e_0
	//		0xff,//interrupt_e_1
	//		0xff,//interrupt_e_2
	//		0xff,//interrupt_e_3
			0x01C7,//gpio_mask_L
			0x0710,//gpio_mask_H
			0x01ff,//gpio_key_mask //lower 6 key
			0x001f,//gpio_hid_key_mask		 	
		},
		//9,
		//4,
		3,
		{
			{ GPIO_0 ,  0x0001 },	
			{ GPIO_1 ,  0x0002 },
			{ GPIO_2 ,  0x0020 },
			{ GPIO_6 , 0x0004 },		
			{ GPIO_7 ,  0x0008 },
			{ GPIO_8 ,  0x0010 },
			{ GPIO_20,  0x0040 },
			{ GPIO_24 , 0x0080 },
			{ GPIO_25 , 0x0100 },
			// Don't Care
			{ GPIO_16 , 0x0100 },
			{ GPIO_17 , 0x0200 },
			{ GPIO_19 , 0x0400 },
			{ GPIO_20 , 0x0800 },
			{ GPIO_24 , 0x1000 },		
			{ GPIO_0 ,  0x0001 },
			{ GPIO_0 ,  0x0001 },
		},
		#else
		{
	//		0x00,//gpio_oe_0
	//		0x00,//gpio_oe_1
	//		0x08,//gpio_oe_2//GPIO 19 is output for pixart 3607 init
	//		0x00,//gpio_oe_3
	//		0xC7,//gpio_i_inv_en_0
	//		0x01,//gpio_i_inv_en_1
	//		0x10,//gpio_i_inv_en_2
	//		0x03,//gpio_i_inv_en_3
	//		//0x00,
	//		0x12,//gpio_mux
	//		0xf0,//interrupt_e_0
	//		0xff,//interrupt_e_1
	//		0xff,//interrupt_e_2
	//		0xff,//interrupt_e_3
			0x01C7,//gpio_mask_L
			0x0310,//gpio_mask_H
			0x01ff,//gpio_key_mask //lower 6 key
			0x001f,//gpio_hid_key_mask		 	
		},
		//9,
		3,
		{
			{ GPIO_0 ,  0x0001 },	
			{ GPIO_1 ,  0x0002 },
			{ GPIO_2 ,  0x0020 },
			{ GPIO_6 ,  0x0004 },		
			{ GPIO_7 ,  0x0008 },
			{ GPIO_8 ,  0x0010 },
			{ GPIO_20,  0x0040 },
			{ GPIO_24 , 0x0080 },
			{ GPIO_25 , 0x0100 },
			// Don't Care
			{ GPIO_16 , 0x0100 },
			{ GPIO_17 , 0x0200 },
			{ GPIO_19 , 0x0400 },
			{ GPIO_20 , 0x0800 },
			{ GPIO_24 , 0x1000 },		
			{ GPIO_0 ,  0x0001 },
			{ GPIO_0 ,  0x0001 },
		},
		#endif
	//LED_BSFLASH,
	//LED_RSFLASH,
	LED_INVALID,//connect_button_led_setting
	10 * 320,//connect_button_press_time	// in unit 0.1Seconds
	0x0020,//connect_button_mask
	#ifdef RELIABILITYTEST
	0x04,//driver_num_gpio_press_mask
	{
		0x0006,
		0x00C0,
		0x0040,
		0x0080,
	},
	#else
	0x03,//driver_num_gpio_press_mask
	{
		0x0006,
		0x000C,
		0x0018,
	},
	#endif
	{//DRIVER_TILT_WHEEL_TABLE
		0x01,
		0x0040,
		0x0080,
	},
	{//DRIVER_WHEEL_TABLE
		0x00,//z_axis_sel
		0x03,//z_axis_enable
	},
	{//DRIVER_DPI_TABLE
		DPI_MODE_BUTTON_PRESS,//mode
		//DPI_MODE_BUTTON_CLICK,//mode
		0x03,//dpi_level
		{ SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI800, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1200, SPI_PIXART_3204_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		//{ SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI400, SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI1200, SPI_PIXART_3607_CONFIGURATION_NORMAL_CPI3000 },//dpi_register_setting[3]
		//{ SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI400 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI800 , SPI_AVAGO_7530_CONFIGURATION_NORMAL_CPI1600 },//dpi_register_setting[3]
		//{ SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI500 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 , SPI_AVAGO_5030_CONFIGURATION_NORMAL_CPI1000 },//dpi_register_setting[3]
		//{ SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI1500 , SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI1750	 , SPI_AVAGO_5090_CONFIGURATION_NORMAL_CPI500 },//dpi_register_setting[3]
		{ DRIVER_LED_USER_3, DRIVER_LED_USER_4, DRIVER_LED_USER_5, },//dpi_led_setting[3]
		0x0000,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO   , DPI_MODE_BUTTON_PRESS : num of gpio_press_mask be pressed
		//0x0100,//dpi_gpio ////DPI_MODE_BUTTON_CLICK : actual GPIO
	},
	{
		0x00,//mode
		LED_BON,//ledsetting
		0x00,//measure_interval
		0x00,//num_instances_average
		0x0000,//full_voltage
		//0x0280,
		//0x0260,
		//0x0258,//low_battery_voltage
		//0x0200,//shut_down_voltage
		//0x0200,//low_battery_voltage
		//0x0000,
		//0x01B4,//shut_down_voltage
		//0x01B4,//low_battery_voltage
		//0x018D,//shut_down_voltage
		//0x018D,//init_ok_voltage
		0x0000,//low_battery_voltage
		0x0000,//shut_down_voltage
		0x0000,//init_ok_voltage
	},
	{
	 	0x00,//special_mode_enable
		0x0020,//special_mode_gpio
		0x01,//power_up_key_L;
		0x02,//power_down_key_L;
		0x00,//power_up_press_num;
		0x01,//power_down_press_num;
		0x02,//power_save_press_num
		LED_BSFLASH,//power_up_1_led
		LED_RON,//power_down_1_led
		DRIVER_LED_USER_2,//power_up_10_led
		DRIVER_LED_USER_3,//power_down_10_led
		DRIVER_LED_USER_5,//power_save_led
	},
	SYS_HID_MOUSE_TYPE,//mmi_hid_module_type
	{	//mmi_shut_down_rf_para
		{
			0x33, 0xD4, 0x1D
		},		
		{
			0x00, 0x00, 0x00
		},		
		{
			0x00, 0x00, 0x00
		},
	},
	10,//dpi_click_change_time;//unit 100ms
	{   0,  },//firmware_version[6];
	#ifdef RELIABILITYTEST
	0x08,//driver_num_main_action_handle
	#else
	0x07,//driver_num_main_action_handle
	#endif	
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
	{
		0,	
	},
	{
		0,	
	},
	{
		0,
	},
	//{
	//	0,
	//},

};

PUBLIC DRIVER_VARY_PARA_TYPE CODE gDriver_v_para_init =
{
	0x01,//cpi
};

#else

static DRIVER_NVRAM_TYPE CODE gDRIVER_nvram_init = 
{
 	0
};

static DRIVER_VARY_PARA_TYPE CODE gDriver_v_para_init =
{
	0
};

#endif
