#ifndef _I2C_H_
#define _I2C_H_

#include "config.h"
#include "os.h"


////////// Macro define /////////
#define I2C_STATUS_MASK_DONE				0x01
#define I2C_STATUS_MASK_DATA_NAK			0x02
#define I2C_STATUS_MASK_ADDR_NAK			0x04
#define I2C_STATUS_MASK_CONTENTION_FAIL		0x08
#define I2C_STATUS_MASK_ALLERROR			(I2C_STATUS_MASK_DATA_NAK | I2C_STATUS_MASK_ADDR_NAK | I2C_STATUS_MASK_CONTENTION_FAIL)

#define I2C_SPPED_100K_SELECT				0
#define I2C_SPPED_400K_SELECT				1

#define I2C_DEVICE_WRITE_SELECT				0
#define I2C_DEVICE_READ_SELECT				1

#define I2C_SETTING_MASK_STANDARD_SMART_SW	0x01
#define I2C_SETTING_MASK_READ_WRITE_SW		0x02
#define I2C_SETTING_MASK_SEQ_CUR_READ_EN	0x04
#define I2C_SETTING_MASK_SCL_SYN_EN	        0x08

#define I2C_SETTING_STANDARD				0
#define I2C_SETTING_SMART					I2C_SETTING_MASK_STANDARD_SMART_SW
#define I2C_SETTING_WRITE					0
#define I2C_SETTING_READ					I2C_SETTING_MASK_READ_WRITE_SW
#define I2C_SETTING_SEQ_CUR_READ			I2C_SETTING_MASK_SEQ_CUR_READ_EN

#define I2C_NO_AUTO_RETRY					0
#define I2C_AUTO_RETRY						1					

#define I2C_SET_START_ADDR(A)				OS_ENTER_CRITICAL();\
											LC_SFR_WRITE_BY_C2(C2OFFS_RW_I2C_RD_BUFFER_ADDR_B0,PTR16_LSB(A));\
											LC_SFR_CONT_WRITE_BY_C2(PTR16_MSB(A));\
											OS_EXIT_CRITICAL()
											
#define I2C_SET_READ_ADDR(A)				OS_ENTER_CRITICAL();\
											LC_SFR_WRITE_BY_C2(C2OFFS_WO_I2C_WR_BUFFER_ADDR_B0,(U8)(A));\
											LC_SFR_CONT_WRITE_BY_C2(((A)>>8));\
											OS_EXIT_CRITICAL()											

//(0x00 << 4) means disabling contention function, A is speed, B is auto-retry
#define I2C_SET_SPEED_AND_AUTO_RETRY(A,B)	OS_ENTER_CRITICAL();\
											LC_SFR_WRITE_BY_C2(C2OFFS_RW_I2C_CONFIG_B0,(U8)(((B << 2) | (0x00 << 4)) | A));\
											OS_EXIT_CRITICAL()
											
//////// Type define //////////
typedef struct
{
	U8 speed_select;
	U8 settings;
	U16 length;
	U8 device_select;
	union
	{
		struct {
			U16 buf_addr;
		} seq_read;
		struct {
			U8 reg_addr;
		} std_read;
		struct {
			U8 reg_addr;
			//U8 io_buffer[1];	/* device might place actual buffer over here */
		} std_write;
		struct {
			U16 reg_addr;
		} smart_read;
		struct {
			U16 reg_addr;
			//U8 io_buffer[1];	/* device might place actual buffer over here */
		} smart_write;
	} u;
} I2C_IO_CTRL;

typedef struct
{
	U8 speed_select;
	U8 settings;
	U16 length;
	U8 device_select;
	union
	{
		struct {
			U8 reg_addr;
		} std_write;
	} u;
} I2C_STD_WRITE_CTRL;


typedef void (*I2CVFUN)(U8 device_addr);

typedef struct
{
	U8 speed_select;
  	U8 XDATA_PTR processing_ptr;
	U8 stored_status;
} I2C_CTL_TYPE;

///////// Function Prototypes /////////
PUBLIC void I2C_InterruptService(void);
#ifdef I2C_NONBLOCKING_ENABLE
PUBLIC void I2C_NonBlockingIssue(U8 XDATA_PTR cmd_ptr) compact REENTRANT;
#else
PUBLIC PUBLIC U8 I2C_BlockingIssue(U8 XDATA_PTR cmd_ptr, U8 auto_retry) compact REENTRANT;
#endif


PUBLIC U8 I2C_StdReadU8(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdReadU16(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdReadU32(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdReadChunk(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC U8 I2C_StdAutoReadU8(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdAutoReadU16(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdAutoReadU32(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdAutoReadChunk(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC void I2C_StdWriteU8(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdWriteU16(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdWriteU32(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdWriteChunk(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);

PUBLIC void I2C_StdAutoWriteU8(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdAutoWriteU16(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdAutoWriteU32(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdAutoWriteChunk(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);

#ifdef I2C_SPEED_100K_ENABLE
PUBLIC U8 I2C_StdReadU8_100K(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdReadU16_100K(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdReadU32_100K(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdReadChunk_100K(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC U8 I2C_StdAutoReadU8_100K(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdAutoReadU16_100K(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdAutoReadU32_100K(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdAutoReadChunk_100K(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC void I2C_StdWriteU8_100K(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdWriteU16_100K(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdWriteU32_100K(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdWriteChunk_100K(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);

PUBLIC void I2C_StdAutoWriteU8_100K(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdAutoWriteU16_100K(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdAutoWriteU32_100K(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdAutoWriteChunk_100K(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);
#endif //I2C_SPEED_100K_ENABLE

#ifdef I2C_SPEED_800K_ENABLE
PUBLIC U8 I2C_StdReadU8_800K(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdReadU16_800K(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdReadU32_800K(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdReadChunk_800K(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC U8 I2C_StdAutoReadU8_800K(U8 device_addr, U8 reg_addr);
PUBLIC U16 I2C_StdAutoReadU16_800K(U8 device_addr, U8 reg_addr);
PUBLIC U32 I2C_StdAutoReadU32_800K(U8 device_addr, U8 reg_addr);
PUBLIC U8 I2C_StdAutoReadChunk_800K(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr);

PUBLIC void I2C_StdWriteU8_800K(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdWriteU16_800K(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdWriteU32_800K(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdWriteChunk_800K(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);

PUBLIC void I2C_StdAutoWriteU8_800K(U8 device_addr, U8 reg_addr, U8 value);
PUBLIC void I2C_StdAutoWriteU16_800K(U8 device_addr, U8 reg_addr, U16 value);
PUBLIC void I2C_StdAutoWriteU32_800K(U8 device_addr, U8 reg_addr, U32 value);
PUBLIC U8 I2C_StdAutoWriteChunk_800K(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr);

#endif //I2C_SPEED_800K_ENABLE

#ifdef I2C_SPEED_ENABLE
PUBLIC U8 I2C_StdWrite(U8 device_addr, U8 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr, U8 Speed, U8 auto_retry_en);

PUBLIC U8 I2C_SmartWrite(U8 device_addr, U16 reg_addr, U16 datLength, U8 GENERIC_PTR dataPtr, U8 Speed, U8 auto_retry_en);

PUBLIC U8 I2C_StdRead(U8 device_addr, U8 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr, U8 Speed, U8 auto_retry_en);
PUBLIC U8 I2C_SmartRead(U8 device_addr, U16 reg_addr, U16 datLength, U8 XDATA_PTR dataPtr, U8 Speed, U8 auto_retry_en);

#endif //I2C_SPEED_ENABLE
#endif //_I2C_H_
