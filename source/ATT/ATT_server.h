//////////////////////////////////////////////////////////
// Copyright@ 2014 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#ifndef _ATT_SERVER_H_ 
#define _ATT_SERVER_H_

#include "cpu.h"
#include "le_common_hal.h"
#ifdef LE_SUPPORTED_HOST_FLASH

/**
*	Characteristic Property Bit MASK
*/

#define C_BCAST            	0x01
#define C_READ        		0x02
#define C_WRITE_NO_RSP   	0x04
#define C_WRITE         	0x08
#define C_NOTIFY       		0x10
#define C_INDICATE        	0x20
#define C_AUTHEN           	0x40
#define C_EXTENDED       	0x80

/**
*	UUID type define
*/
#define UUID_B_016			0x02
#define UUID_B_128			0x10
#define UUID_B_032			0x04

typedef union {
	U8 XDATA_PTR addr;
	U16	uuid16;	
}UUID_U;

typedef struct {
	UUID_U uuid;	
	U8 type;
}PACKED_STRUCT UUID_T;


/**
 * SDK_ATTRIBUTE member description:
 *
 * @dataptr: 		The pointer of attribute value
 *
 * @CBfunc: 		The CallBack function that would be executed before this attribute is going to be read
 *          		and after this attribute has just been written
 *
 * @uuid_t:  		It is an union that
 * 					Member 1 (uuid) stands for 16-bit UUID or the data pointer of 128-bit UUID
 * 					Member 2 (type) specifies it is either 16-bit or 128-bit UUID

 * @permission:		It specifies the read/write permission of this attribute.
 * 					Four levels of permission for Read and Write action separately.
 *					Please find definition of "ATT_READ_WRITE_PERMISSION_ENUM" for further reference
 *
 * @currentLen:		The current attribute length.
 *		 			If it is a variable length attribute (maximumLen != 0),
 *					it might be changed during ATT write operation (request/command)
 *
 * @maximumLen:		The maximum valid length of this attribute for variable length attributes.
 * 					For fixed length attributes, it should be set to zero.
 *
 * @handle:			The attribute handle of this attribute in the attribute server.
 * 					It is not necessarily equal to server index.
 */
typedef struct {
	U8 CODE_PTR dataptr;
	U8 (*CBfunc)(U8 CBtype, U8 LinkIdx);
	UUID_T uuid_t;
    U8 permission;
    U16 currentLen;
    U16 maximumLen;
	U16 handle;
} PACKED_STRUCT SDK_ATTRIBUTE;

typedef U8 (*ATTCB)(U8, U8);

typedef struct{
	U16 handle;
	U16 End_handle;
	U16 UUID;
}PACKED_STRUCT INC_DECLARE_016;

typedef struct{
	U16 handle;
	U16 End_handle;
}PACKED_STRUCT INC_DECLARE_128;


typedef struct {
	U8 property;
	U16 handle;
	U16 UUID;  
}PACKED_STRUCT CHAR_DECLARE_016;

typedef struct {
	U8 property;
	U16 handle;
	U32 UUID[4];  
}PACKED_STRUCT CHAR_DECLARE_128;


extern SDK_ATTRIBUTE CODE_PTR CODE_PTR gATT_Server;
extern U16 CODE ATT_SDK_SERVER_SIZE;

#define ATT_SERVER_SIZE         ATT_SDK_SERVER_SIZE//ATT_ATTRIBUTE_TOTAL_NUM

#define ATT_R_PMT(p)            ((p)&0x0F)
#define ATT_W_PMT(p)            ((p)&0xF0)

#endif /* LE_SUPPORTED_HOST_FLASH */

#endif /* _ATT_SERVER_H_ */