SECTOR_DRIVER_LED_DATA_STRU CODE gDriver_led_data_init =
{
	{
		LED_PATTERN_DEFAULT_NO,
		{
			{
				{  //0 : //Fast Blue Flash
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, (0xf0|0x02), 10, 10, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				}
			},
			{
				{  // 1 ://LED1: power on / off
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, (0xf0|0x02), 10, 10, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{ 	//2:  //LED0: On: 0.06s/ Off: 2s : connectable
					{1, (0xf0), 3, 0, 100, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//3:	//LED0: On: 0.5s/ Off: 0.5s : discoverable
					{1, (0xf0), 25, 25, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//4:	//LED0: On: 60ms/ Off: 5.4s: full link
					{1, (0xf0), 3, 15, 255, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//5:	///LED0: On: 60ms/ Off: 5.4s: other device : called in code
					{1, (0xf0), 3, 15, 255, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//6:	//All Off: nornal link
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//7 :	//LED 0 On: 2s/ Off: 1s: Low Battery 
					{0, (0xf0), 100, 50, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//8 :	//BOTH_OFF
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//9  	//LED1: On: 0.2s/ Off: 0.5s: TWS
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 10, 25, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{//10:  //LED0: On: 1s/ Off: 7s :AUX
					{1, (0xf0|0x00), 50, 100, 250, 0, 0, LED_ON, 10, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
			{
				{// 11 	//FCC Test Mode: Constantly 
					{1, (0xf0|0x00), 10, 0, 0, 0, 0, LED_ON, 0, 0},
					{1, (0xf0|0x00), 10, 0, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
				},
			},
		},
		LED_FILTER_DEFAULT_NO,
		{
			{
				{
					FILTER_USER_DEF,
					{
						{
							{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
						},
					},
				},
			},
			{
				{
					FILTER_USER_DEF,
					{
						{
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
						},
					},
				},
			},
			{
				{
					FILTER_USER_DEF,
					{
						{
							{0, 0, 0, 0, 0, 0,	0, LED_DONT_CARE, 0, 0},
							{1, (0xf0), 3, 15, 255, 0, 0, LED_ON, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
						},
					},
				},
			},
		},
		LED_MAP_GPIO_DEFAULT_NO,		//ledMapGpioNo
		{//LedMapSetting
			{0, 0},
			{0, 0},
		},
	},
    //- CRC
    {0,0}
};

