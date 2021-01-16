#ifndef _SDK_PIO_H_
#define _SDK_PIO_H_


	typedef U8 SDK_GPIO_ID;
	#define GPIO_0       ((SDK_GPIO_ID)(0))
	#define GPIO_1       ((SDK_GPIO_ID)(1))
	#define GPIO_2       ((SDK_GPIO_ID)(2))
	#define GPIO_3       ((SDK_GPIO_ID)(3))
	#define GPIO_4       ((SDK_GPIO_ID)(4))
	#define GPIO_5       ((SDK_GPIO_ID)(5))
	#define GPIO_6       ((SDK_GPIO_ID)(6))
	#define GPIO_7       ((SDK_GPIO_ID)(7))
	#define GPIO_8       ((SDK_GPIO_ID)(8))
	#define GPIO_9       ((SDK_GPIO_ID)(9))
	#define GPIO_10      ((SDK_GPIO_ID)(10))
	#define GPIO_11      ((SDK_GPIO_ID)(11))
	#define GPIO_12      ((SDK_GPIO_ID)(12))
	#define GPIO_13      ((SDK_GPIO_ID)(13))
	#define GPIO_14      ((SDK_GPIO_ID)(14))
	#define GPIO_15      ((SDK_GPIO_ID)(15))
	#define GPIO_16      ((SDK_GPIO_ID)(16))
	#define GPIO_17      ((SDK_GPIO_ID)(17))
	#define GPIO_18      ((SDK_GPIO_ID)(18))
	#define GPIO_19      ((SDK_GPIO_ID)(19))
	#define GPIO_20      ((SDK_GPIO_ID)(20))
	#define GPIO_21      ((SDK_GPIO_ID)(21))
	#define GPIO_22      ((SDK_GPIO_ID)(22))
	#define GPIO_23      ((SDK_GPIO_ID)(23))
	#define GPIO_24      ((SDK_GPIO_ID)(24))
	#define GPIO_25      ((SDK_GPIO_ID)(25))
	#define GPIO_26      ((SDK_GPIO_ID)(26))
	#define GPIO_27      ((SDK_GPIO_ID)(27))
	#define GPIO_28      ((SDK_GPIO_ID)(28))
	#define GPIO_29      ((SDK_GPIO_ID)(29))
	#define GPIO_30      ((SDK_GPIO_ID)(30))
	
	
	#define PIN_SET_INPUT	0
	#define PIN_SET_OUTPUT	1
	
	#define LEVEL_LOW	0
	#define LEVEL_HIGH	1
	
/*  SDK_GPIO_SetIO: Set gpio input or output. If it's set output, set output level high or low.
 *	<input 1>
 *		U8 isOutputPin	: set input or output. 
 *  	value range		: PIN_SET_INPUT or PIN_SET_OUTPUT.
 *	<input 2>
 *		U8 isHigh		: if "isOutputPin" is output, set output level high or low. 
 *  	value range		: LEVEL_LOW or LEVEL_HIGH.
 *	<input 3>
 *		U8 ioID			: target GPIO pin. 
 *  	value range		: from GPIO_0 to GPIO_30.
 */	
void SDK_GPIO_SetIO(U8 isOutputPin, U8 isHigh, U8 ioID);


/*  SDK_GPIO_GetInputHighLow: Get gpio input level.
 *	<input 1>
 *		U8 ioID			: target GPIO pin. 
 *  	value range		: from GPIO_0 to GPIO_30.
 *	<output 1>
 *		U8 				: detected input level. 
 *  	value range		: OUTPUT_LEVEL_LOW or OUTPUT_LEVEL_HIGH.
 *	<input 3>
 *		U8 ioID			: target GPIO pin. 
 *  	value range		: LEVEL_LOW or LEVEL_HIGH.
 */	
U8 SDK_GPIO_GetInputHighLow(U8 ioID);


#endif //_SDK_PIO_H_