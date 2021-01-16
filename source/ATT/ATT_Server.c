#define _ATT_SERVER_C_
#include "reside_flash.inc"
#include "rc.h"

#ifdef LE_SUPPORTED_HOST_FLASH

#include "Bt_config_profile.h"

#include "ATT_server.h"
#include "ATT_External_Struct.h"
#include "..\APP\APP_Interface.h"
#ifdef ANCS_CELL_PHONE_APP
#include "..\APP_ANCS\APP_ANCS_interface.h"
#endif  //#ifdef ANCS_CELL_PHONE_APP

#if ( defined __BIG_ENDIAN_MCU__ &&  defined __LITTLE_ENDIAN_MCU__)|| \
	(!defined __BIG_ENDIAN_MCU__ && !defined __LITTLE_ENDIAN_MCU__)
		#warning "This SDK may not work correctly since Endianness isn't defined VALIDLY "
#endif

//////// Helper MACRO definition ////////
#define EXPAND1(X,Y) X##Y
#define EXPAND(X,Y) EXPAND1(X,Y)
#define ADDLINE(X) EXPAND(X,__LINE__)
//////////////////////////////////////////

/**
*	1. Attribute handle enumerate
*
*/
#define ATT_H_JUMP_TO(handle)												ADDLINE(ATT_H_JUMP_TO_) = handle-1,


#define SERVICE_016_M(NUM,TYPE,B1)											ATT_H_SERVICE_##NUM,
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)									ATT_H_SERVICE_##NUM,
#define INCLUDE_016_M(NUM)													ADDLINE(ATT_H_INCLUDE_),
#define INCLUDE_128_M(NUM)													ADDLINE(ATT_H_INCLUDE_),
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)						ADDLINE(ATT_H_CHAR_D_), \
																			ADDLINE(ATT_H_CHAR_V_),
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)				ADDLINE(ATT_H_CHAR_D_), \
																			ADDLINE(ATT_H_CHAR_V_),
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)								ADDLINE(ATT_H_DESCRIP_),

#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)				ADDLINE(ATT_H_CHAR_D_), \
																			ADDLINE(ATT_H_CHAR_V_),
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)		ADDLINE(ATT_H_CHAR_D_), \
																			ADDLINE(ATT_H_CHAR_V_),
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)						ADDLINE(ATT_H_DESCRIP_),


enum
{
	ATT_H_RESERVED,
	#include "ATT_RAW.h"
    #include "ATT_undefine_MACRO.h"
}; //Attribute handle enumerate

#undef ATT_H_JUMP_TO
#define ATT_H_JUMP_TO(handle)   

/**
*	2. Calculate attribute number of each service
*
*/

#define SERVICE_016_M(NUM,TYPE,B1)                   ,ATT_NUM_OF_SERVICE_##NUM = 1
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)          ,ATT_NUM_OF_SERVICE_##NUM = 1
#define INCLUDE_016_M(NUM)                                                      +1
#define INCLUDE_128_M(NUM)                                                      +1
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)                          +2
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)                 +2
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)                                   +1
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)                  +2
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)         +2
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)                           +1


enum
{
	ATT_NUM_COUNT_START
	#include "ATT_RAW.h"
    #include "ATT_undefine_MACRO.h"
	,ATT_NUM_COUNT_END
};


/**
*	3. Service UUID record (16 Bits only)
*
*/
#define SERVICE_016_M(NUM,TYPE,B1)			ENUM_SERVICE_##NUM  =  B1,
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)
#define INCLUDE_016_M(NUM)
#define INCLUDE_128_M(NUM)
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)


enum
{
	#include "ATT_RAW.h"
    #include "ATT_undefine_MACRO.h"
	END_OF_RECORD
};

/**
*	4. UUID instance declaration
*
*/
#define SERVICE_016_M(NUM,TYPE,B1) U16 CODE UUID_service_##NUM[] = {TO_LITTLE_ENDIAN_U16(B1)};
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4) U32 CODE UUID_service_##NUM[] = {TO_LITTLE_ENDIAN_U32(B4), \
																			 TO_LITTLE_ENDIAN_U32(B3), \
																			 TO_LITTLE_ENDIAN_U32(B2), \
																			 TO_LITTLE_ENDIAN_U32(B1)};
#define INCLUDE_016_M(NUM)
#define INCLUDE_128_M(NUM)
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	U32 XDATA ADDLINE(UUID_char_)[]={ \
																			 TO_LITTLE_ENDIAN_U32(B4), \
																			 TO_LITTLE_ENDIAN_U32(B3), \
																			 TO_LITTLE_ENDIAN_U32(B2), \
																			 TO_LITTLE_ENDIAN_U32(B1)};
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	U32 XDATA ADDLINE(UUID_char_)[]={ \
																			 TO_LITTLE_ENDIAN_U32(B4), \
																			 TO_LITTLE_ENDIAN_U32(B3), \
																			 TO_LITTLE_ENDIAN_U32(B2), \
																			 TO_LITTLE_ENDIAN_U32(B1)};
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)


#include "ATT_RAW.h"
#include "ATT_undefine_MACRO.h"


/**
*	5. Include Declaration value instance declaration
*
*/
#define SERVICE_016_M(NUM,TYPE,B1)
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)
#define INCLUDE_016_M(NUM)	INC_DECLARE_016 CODE ADDLINE(ATT_Include_D_) ={ \
											TO_LITTLE_ENDIAN_U16(ATT_H_SERVICE_##NUM), \
											TO_LITTLE_ENDIAN_U16(ATT_H_SERVICE_##NUM + ATT_NUM_OF_SERVICE_##NUM -1), \
											TO_LITTLE_ENDIAN_U16(ENUM_SERVICE_##NUM), \
																			};
#define INCLUDE_128_M(NUM)	INC_DECLARE_128 CODE ADDLINE(ATT_Include_D_) ={ \
											TO_LITTLE_ENDIAN_U16(ATT_H_SERVICE_##NUM), \
											TO_LITTLE_ENDIAN_U16(ATT_H_SERVICE_##NUM + ATT_NUM_OF_SERVICE_##NUM -1), \
																			};
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)


#include "ATT_RAW.h"
#include "ATT_undefine_MACRO.h"



/**
*	6. Characteristic declaration Value instance declaration
*
*/

#define SERVICE_016_M(NUM,TYPE,B1)
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)
#define INCLUDE_016_M(NUM)
#define INCLUDE_128_M(NUM)
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1) 				    CHAR_DECLARE_016 CODE ADDLINE(ATT_CHAR_D_) = { \
																			PROPERTY, \
																			TO_LITTLE_ENDIAN_U16((ADDLINE(ATT_H_CHAR_V_))), \
																			TO_LITTLE_ENDIAN_U16(B1) \
																		};
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)		    CHAR_DECLARE_128 CODE ADDLINE(ATT_CHAR_D_) = { \
																			PROPERTY, \
																			TO_LITTLE_ENDIAN_U16((ADDLINE(ATT_H_CHAR_V_))), \
																			TO_LITTLE_ENDIAN_U32(B4), \
																			TO_LITTLE_ENDIAN_U32(B3), \
																			TO_LITTLE_ENDIAN_U32(B2), \
																			TO_LITTLE_ENDIAN_U32(B1), \
																		};
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1) 			CHAR_DECLARE_016 CODE ADDLINE(ATT_CHAR_D_) = { \
																			PROPERTY, \
																			TO_LITTLE_ENDIAN_U16((ADDLINE(ATT_H_CHAR_V_))), \
																			TO_LITTLE_ENDIAN_U16(B1) \
																		};	
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)		CHAR_DECLARE_128 CODE ADDLINE(ATT_CHAR_D_) = { \
																			PROPERTY, \
																			TO_LITTLE_ENDIAN_U16((ADDLINE(ATT_H_CHAR_V_))), \
																			TO_LITTLE_ENDIAN_U32(B4), \
																			TO_LITTLE_ENDIAN_U32(B3), \
																			TO_LITTLE_ENDIAN_U32(B2), \
																			TO_LITTLE_ENDIAN_U32(B1), \
																		};	
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)

#include "ATT_RAW.h"
#include "ATT_undefine_MACRO.h"


/**
*	7. Calculate attribute server Size
*
*/

#define SERVICE_016_M(NUM,TYPE,B1)											+1
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)									+1
#define INCLUDE_016_M(NUM)													+1
#define INCLUDE_128_M(NUM)													+1
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1) 						+2
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)				+2
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,B1)  								+1
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1) 				+2
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)		+2
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)  						+1

																		
enum
{
	ATT_NUMBER_OF_ATTRBUTES = 0
	#include "ATT_RAW.h"
    #include "ATT_undefine_MACRO.h"
	,ATT_SERVER_SIZE_ENUM
}; //Attribute handle enumerate


/**
*	7. Characteristic external symbol declaration   // TBD
*
*/

/*
#define SERVICE_016_M(NUM,TYPE,B1)
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4)
#define INCLUDE_016_M(NUM)
#define INCLUDE_128_M(NUM)
#define CHARACT_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1) //EXTERN (SYMBOL)[];
#define CHARACT_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	//EXTERN (SYMBOL)[];
#define DESCRIPTORS_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,B1)  //CHECK_SYMBOL(SYMBOL)

#include "ATT_RAW.h"
#include "ATT_undefine_MACRO.h"
*/



/**
*	8. Attribute instance declaration
*
*/

/**
typedef struct {
	U8 CODE_PTR dataptr;
	U8 (*CBfunc)(U8);
	UUID_T uuid_t;
    U8 permission;
    U16 currentLen;
    U16 maximumLen;
	U16 handle;
}SDK_ATTRIBUTE;
*/
								//	[data]		[Callback]	[addr]		[tpye]	 [permission]   [currentLen]   [maximumLen]	[handle]
SDK_ATTRIBUTE CODE firstAttribute= {NULL,			NULL, 	{NULL,		   0}, 		0, 				0, 	           0,		ATT_SERVER_SIZE_ENUM};


#define SERVICE_016_M(NUM,TYPE,B1) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute) = {(U8 CODE_PTR)UUID_service_##NUM,	 	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)TYPE,	UUID_B_016}, 0x81, 2, 0,	ATT_H_SERVICE_##NUM};
		
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute) = {(U8 CODE_PTR)UUID_service_##NUM,	 	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)TYPE,	UUID_B_016}, 0x81,16, 0,	ATT_H_SERVICE_##NUM};

#define INCLUDE_016_M(NUM) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute) = {(U8 CODE_PTR)&ADDLINE(ATT_Include_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2802,	UUID_B_016}, 0x81, 6, 0,	ADDLINE(ATT_H_INCLUDE_)};

#define INCLUDE_128_M(NUM) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute) = {(U8 CODE_PTR)&ADDLINE(ATT_Include_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2802,	UUID_B_016}, 0x81, 4, 0,	ADDLINE(ATT_H_INCLUDE_)};
		
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute1_) = {(U8 CODE_PTR)&ADDLINE(ATT_CHAR_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2803,UUID_B_016}, 0x81, 0x0005, 0, ADDLINE(ATT_H_CHAR_D_)}; \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute2_) = {(U8 CODE_PTR)&(SYMBOL),  			(ATTCB)CB,{(U8 XDATA_PTR)B1,UUID_B_016}, 		   	 RW_P, SIZE, 0, ADDLINE(ATT_H_CHAR_V_)};

#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	\
		SDK_ATTRIBUTE CODE ADDLINE(Attribute1_) = {(U8 CODE_PTR)&ADDLINE(ATT_CHAR_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2803,UUID_B_016}, 0x81, 0x0013, 0, ADDLINE(ATT_H_CHAR_D_)}; \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute2_) = {(U8 CODE_PTR)&(SYMBOL),  			(ATTCB)CB,{(U8 XDATA_PTR)&ADDLINE(UUID_char_),UUID_B_128},RW_P, SIZE, 0, ADDLINE(ATT_H_CHAR_V_)};

#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,TYPE) \
		SDK_ATTRIBUTE CODE ADDLINE(Attribute) = {(U8 CODE_PTR)&(SYMBOL),  				(ATTCB)CB,{(U8 XDATA_PTR)TYPE,UUID_B_016},			 RW_P, SIZE, 0, ADDLINE(ATT_H_DESCRIP_)};

#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1) \
		SDK_ATTRIBUTE  CODE ADDLINE(Attribute1_) = {(U8 CODE_PTR)&ADDLINE(ATT_CHAR_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2803,UUID_B_016}, 0x81, 0x0005, 0, ADDLINE(ATT_H_CHAR_D_)}; \
		SDK_ATTRIBUTE XDATA ADDLINE(Attribute2_) = {(U8 CODE_PTR)&(SYMBOL),  			(ATTCB)CB,{(U8 XDATA_PTR)B1,UUID_B_016}, 		   	 RW_P, SIZE, MAXSIZE, ADDLINE(ATT_H_CHAR_V_)};
		
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	\
		SDK_ATTRIBUTE  CODE ADDLINE(Attribute1_) = {(U8 CODE_PTR)&ADDLINE(ATT_CHAR_D_),	(ATTCB)FAR_NULL,{(U8 XDATA_PTR)0x2803,UUID_B_016}, 0x81, 0x0013, 0, ADDLINE(ATT_H_CHAR_D_)}; \
		SDK_ATTRIBUTE XDATA ADDLINE(Attribute2_) = {(U8 CODE_PTR)&(SYMBOL),  			(ATTCB)CB,{(U8 XDATA_PTR)&ADDLINE(UUID_char_),UUID_B_128},RW_P, SIZE, MAXSIZE, ADDLINE(ATT_H_CHAR_V_)};

#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,TYPE) \
		SDK_ATTRIBUTE XDATA ADDLINE(Attribute) = {(U8 CODE_PTR)&(SYMBOL),  				(ATTCB)CB,{(U8 XDATA_PTR)TYPE,UUID_B_016},			 RW_P, SIZE, MAXSIZE, ADDLINE(ATT_H_DESCRIP_)};


#include "ATT_RAW.h"
#include "ATT_undefine_MACRO.h"


/**
*	9. Attribute Server instance declaration
*
*/
#define SERVICE_016_M(NUM,TYPE,B1) 										(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),
#define SERVICE_128_M(NUM,TYPE,B1,B2,B3,B4) 							(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),
#define INCLUDE_016_M(NUM) 												(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),
#define INCLUDE_128_M(NUM) 												(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),
#define CHARACT_016_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1)  				(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute1_),\
																		(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute2_),
#define CHARACT_128_M(RW_P,SIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)			(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute1_),\
																		(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute2_),
#define DESCRIPTORS_M(RW_P,SIZE,SYMBOL,CB,TYPE) 						(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),
#define VL_CHAR_016_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1)  		(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute1_),\
																		(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute2_),
#define VL_CHAR_128_M(RW_P,SIZE,MAXSIZE,SYMBOL,PROPERTY,CB,B1,B2,B3,B4)	(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute1_),\
																		(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute2_),
#define VL_DESCRIPT_M(RW_P,SIZE,MAXSIZE,SYMBOL,CB,TYPE) 				(SDK_ATTRIBUTE CODE_PTR)&ADDLINE(Attribute),





typedef struct{
    SDK_ATTRIBUTE CODE_PTR att[ATT_SERVER_SIZE_ENUM];
}PACKED_STRUCT ATT_SERVER_SDK_TYPE;


ATT_SERVER_SDK_TYPE CODE gATT_Server_SDK =
{
	{
		&firstAttribute,
	#include "ATT_RAW.h"
    #include "ATT_undefine_MACRO.h"
	}
};
SDK_ATTRIBUTE CODE_PTR CODE_PTR  gATT_Server = (SDK_ATTRIBUTE CODE_PTR CODE_PTR)&(gATT_Server_SDK.att[0]);
U16 CODE ATT_SDK_SERVER_SIZE = ATT_SERVER_SIZE_ENUM;

#endif /* LE_SUPPORTED_HOST_FLASH */

