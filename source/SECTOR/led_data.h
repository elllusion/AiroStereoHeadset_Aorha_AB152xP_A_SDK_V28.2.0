SECTOR_DRIVER_LED_DATA_STRU CODE gDriver_led_data_init =
{
	{
		LED_PATTERN_DEFAULT_NO,
		{
			{
				{  //0 : //Fast Blue Flash
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, (0xf0|0x02), 10, 10, 0, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				}
			},
			{
				{  // 1 ://Fast Red Flash
					{0, (0xf0|0x02), 10, 10, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{ //2:  //Blue Led always on
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//3:	//Slow Blue Flash in 5secs
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 25, 0, 50, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//4:	//Slow Blue Flash in 7secs
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 3, 15, 255, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//5:	//Fast Blue Flash
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 10, 10, 0, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//6:	//Slow Blue Flash in 8secs
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 50, 0, 125, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//7 :	//Low Battery LED: Red fast flash twice
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{1, (0xf0), 5, 95, 100, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//8 :	//BOTH_OFF
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//9  	//BOTH_ON
					{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
					{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{//10:  //ALT
					{1, (0xf0|0x00), 2, 250, 10, 0, 0, LED_ALT_SECOND, 1, 0},
					{1, (0xf0|0x00), 2, 250, 10, 0, 0, LED_ALT_FIRST, 1, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{// 11 	//FCC Test Mode: Constantly 
					{1, (0xf0|0x00), 10, 0, 0, 0, 0, LED_ON, 0, 0},
					{1, (0xf0|0x00), 10, 0, 0, 0, 0, LED_ON, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
				},
			},
			{
				{ //12:  //Red Led always on
					{1, (0xf0), 0xFF, 0, 0, 0, 0, LED_ON, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#if !defined AB1520S && !defined AB1525S && !defined AB1520U
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
					#endif
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
							#if !defined AB1520S && !defined AB1525S && !defined AB1520U
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
							#endif
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
							#if !defined AB1520S && !defined AB1525S && !defined AB1520U
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,	0, LED_OFF, 0, 0},
							{0, 0, 0, 0, 0, 0,  0, LED_OFF, 0, 0},
							#endif
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

