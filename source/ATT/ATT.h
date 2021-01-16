//////////////////////////////////////////////////////////
// Copyright@ 2014 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#ifndef _ATT_H_ 
#define _ATT_H_


#include "ATT_server.h"

#ifdef LE_SUPPORTED_HOST_FLASH

#ifdef _ATT_C_
#define ATT_EXTERN
#else
#define ATT_EXTERN extern
#endif

// Config Unit Test==========================================================
//#define ATT_UNIT_TEST
#define ATT_UNIT_TEST_START_STATE TEST_EXCHANGE_MTU
#define ATT_UNIT_TEST_STOP_STATE  TEST_END


// ATT packet offset=========================================================
#ifdef AB1600

#include "L2CAP_low_energy.h"

	#define ATT_L2CAP_OPCODE_OFFSET   			sizeof(L2CAP_DATA_HDR_STRU)		//8
	#define ATT_PDU_OFFSET						(ATT_L2CAP_OPCODE_OFFSET+1)					//9
	#define ATT_SIGNED_WRITE_STATUS_OFFSET		ATT_L2CAP_OPCODE_OFFSET
	#define ATT_GET_L2CAP_PACKET_LENGTH(ptr)	two_byte_swap(((L2CAP_RX_HCI_DATA_STRU XDATA_PTR)(ptr + *ptr))->l2capDataHdr.len)

#else
    #include "ATT_interface.h"

	#define ATT_L2CAP_LENGTH_OFFSET 0
	#define ATT_L2CAP_OPCODE_OFFSET 12
	#define ATT_HCI_HDR_OFFSET 4
	#define ATT_PDU_OFFSET 13       //[offset:0][reserved:3][HCI_hdr:4~5][HCI_len:6~7][L2C_len:8~9][CID:10~11][att opcode:12] --ATT PDU-->   //offset to data
	#define ATT_RSP_PKT_OFFSET 9
	#define ATT_RSP_HEADER_LEN 16   //[offset:0][reserved:1~4][HCI_hdr:5~6][HCI_len:7~8][L2C_len:9~10][CID:11~12][att opcode:13][att handle:14~15]
	#define ATT_HEADER_LEN 3        //[att opcode:13][att handle:14~15]
	#define ATT_SIGNED_WRITE_STATUS_OFFSET 11	//CAUTION!!   use the second byte of CID as status flag
	#define ATT_GET_L2CAP_PACKET_LENGTH(ptr)  (*(*(ptr)+(ptr)) + (*(*(ptr)+(ptr)+1)<<8))

#endif
//end ATT packet offset======================================================

#define UUID_TYPE(a)							(U8)(a->uuid_t.type)
#define UUID_U16(a)								(U16)(a->uuid_t.uuid.uuid16)
#define UUID_PTR(a)								(U8 CODE_PTR)(a->uuid_t.uuid.addr)

#define ATT_START_TRANSACTION(i)				SYS_SetTimer(&gATT_ctl.Transactiontimer[i], HALF_MIN)
#define ATT_END_TRANSACTION(i)					SYS_ReleaseTimer(&gATT_ctl.Transactiontimer[i])
#define ATT_NOT_IN_TRANSACTION(i)				((gATT_ctl.Transactiontimer[i]==(OST XDATA_PTR)NEAR_NULL)?TRUE:FALSE)
#define ATT_RESET_REPORT_TIMER(i)				SYS_SetTimer(&gATT_ctl.Table[i].Timer,gATT_ctl.Table[i].interval)
#define ATT_REPORT_OPCODE(i)					((gATT_ctl.Table[i].ReportType==ATT_REPORT_NOTIFY)?ATT_HANDLE_VALUE_NOTI:ATT_HANDLE_VALUE_IND)


// ATT Methods Opcode========================================================
#define ATT_ERROR_RSP                    0x01
#define ATT_EXCHANGE_MTU_REQ             0x02
#define ATT_EXCHANGE_MTU_RSP             0x03
#define ATT_FIND_INFO_REQ                0x04
#define ATT_FIND_INFO_RSP                0x05
#define ATT_FIND_BY_TYPE_VALUE_REQ       0x06
#define ATT_FIND_BY_TYPE_VALUE_RSP       0x07
#define ATT_READ_BY_TYPE_REQ             0x08
#define ATT_READ_BY_TYPE_RSP             0x09
#define ATT_READ_REQ                     0x0A
#define ATT_READ_RSP                     0x0B
#define ATT_READ_BLOB_REQ                0x0C
#define ATT_READ_BLOB_RSP                0x0D
#define ATT_READ_MULTI_REQ               0x0E
#define ATT_READ_MULTI_RSP               0x0F
#define ATT_READ_BY_GRP_TYPE_REQ         0x10
#define ATT_READ_BY_GRP_TYPE_RSP         0x11
#define ATT_WRITE_REQ                    0x12
#define ATT_WRITE_RSP                    0x13 
#define ATT_PREPARE_WRITE_REQ            0x16
#define ATT_PREPARE_WRITE_RSP            0x17
#define ATT_EXECUTE_WRITE_REQ            0x18
#define ATT_EXECUTE_WRITE_RSP            0x19
#define ATT_HANDLE_VALUE_NOTI            0x1B                           
#define ATT_HANDLE_VALUE_IND             0x1D
#define ATT_HANDLE_VALUE_CFM             0x1E

#define ATT_WRITE_CMD                    0x52
#define ATT_SIGNED_WRITE_CMD             0xD2
#define ATT_SIGNED_WRITE_CMD_VERIFIED	 0xF2	//to distinguish between the signature is verified or not
//end ATT Methods Opcode======================================================

//ATT Error Code==============================================================
#define ATT_NOERR                        0x00
#define ATT_ERR_INVALID_HANDLE           0x01
#define ATT_ERR_READ_NOT_PERMITTED       0x02 
#define ATT_ERR_WRITE_NOT_PERMITTED      0x03
#define ATT_ERR_INVALID_PDU              0x04
#define ATT_ERR_INSUFFICIENT_AUTHEN      0x05
#define ATT_ERR_UNSUPPORTED_REQ          0x06
#define ATT_ERR_INVALID_OFFSET           0x07
#define ATT_ERR_INSUFFICIENT_AUTHOR      0x08
#define ATT_ERR_PREPARE_QUEUE_FULL       0x09
#define ATT_ERR_ATTR_NOT_FOUND           0x0A
#define ATT_ERR_ATTR_NOT_LONG            0x0B
#define ATT_ERR_INSUFFICIENT_KEY_SIZE    0x0C
#define ATT_ERR_INVALID_VALUE_LENGTH	 0x0D
#define ATT_ERR_UNLIKELY                 0x0E
#define ATT_ERR_INSUFFICIENT_ENCRYPT     0x0F
#define ATT_ERR_UNSUPPORTED_GRP_TYPE     0x10
#define ATT_ERR_INSUFFICIENT_RESOURCES   0x11
//reserved 0x12-0x7F
#define ATT_ERR_INVALID_VALUE            0x80
#define ATT_ERR_AU_ACCESS_DENIED         0x81
#define ATT_ERR_AIR_UPDATE_MEM_FULL      0x82
#define ATT_ERR_AIR_UPDATE_DATA_LEN      0x83
//end ATT Error Code==========================================================
#define INVALID_ATT_HANDLE				0xFF

#define ATT_MAX_NUM_OF_CCCD             6
#define ATT_DEFAULT_MTU_SIZE			23
#define ATT_SUPPORT_MTU_SIZE			273
#define MAX_ATT_VALUE_LENGTH			512
#define ATT_MAX_NUM_HANDLES_INFO        5
#define ATT_REPORT_HEADER_LEN 			3   //[opcode:1][handle:2]
#define ATT_REPORT_MEM1_ENTRY_THRESHOLD 10
#define ATT_PERIODIC_REPORT_TABLE_SIZE 	5

#define INITIAL_SEARCH_INDEX			0x0000
#define ATT_RESERVED_HANDLE				0x0000
#define ATT_INVALID_INDEX				0x0000
//ATT PDU format==============================================================
typedef struct
{
  	U16 clientRxMTU;
}PACKED_STRUCT attExchangeMTUReq_t;

typedef struct
{
  	U16 serverRxMTU;
}PACKED_STRUCT attExchangeMTURsp_t;

typedef struct
{
  	U16 startHandle;
  	U16 endHandle;
}PACKED_STRUCT AttFindInfoReq_T;

typedef struct
{
    U8 format;
    U8 pInfoData[ATT_DEFAULT_MTU_SIZE-2];
} AttFindInfoRsp_T;

typedef struct
{
	U16 handle;
  	U16 BTUUID;
}PACKED_STRUCT attHandleBTUUIDPair_t;

typedef struct
{
  	U16 startHandle;
  	U16 endHandle;
  	U16 UUID;
  	U8 value; //ATT_MTU-7
}PACKED_STRUCT ATTFINDBYTYPEVALUEREQ_T;

typedef struct
{
  	U16 handle;
  	U16 grpEndHandle;
}PACKED_STRUCT attHandlesInfo_t;

typedef struct
{
  	U8 pHdlList[ATT_DEFAULT_MTU_SIZE-1];
} AttFindByTypeValueRsp_t;

typedef struct
{
	U16 startHandle;
	U16 endHandle;
	U16 UUID;
}PACKED_STRUCT AttReadByTypeReq_t;

typedef struct
{
	U8 len;
	U8 pDataList[ATT_DEFAULT_MTU_SIZE-2];
} AttReadByTypeRsp_t;

typedef struct
{
    U16 handle;
}PACKED_STRUCT attReadReq_t;

typedef struct
{
    U8 value;
} ATTREADRSP_T;

typedef struct
{
    U16 handle;
    U16 offset;
}PACKED_STRUCT attReadBlobReq_t;

typedef struct
{
	U8 partOfValue;
} attReadBlobRes_t;

typedef struct
{
	U16 handle[(ATT_DEFAULT_MTU_SIZE-1)/2];
}PACKED_STRUCT AttReadMultiReq_t;

typedef struct
{
    U8 pSetOfValue[ATT_DEFAULT_MTU_SIZE-1];
}AttReadMultiRsp_t;

typedef struct
{
	U16 startHandle;
	U16 endHandle;
	U16 type;
}PACKED_STRUCT AttReadByGroupTypeReq_t;

typedef struct
{
    U8 Length;
    U8 pAttDataList[ATT_DEFAULT_MTU_SIZE-2];
} AttReadByGroupTypeRsp_t;

typedef union
{
    U8  pFirstValue[1];
    U16 CCCD;
}ATT_WRITEDATA_U;

typedef struct
{
	U16 handle;
	ATT_WRITEDATA_U dataUnion;
}PACKED_STRUCT ATTWRITEREQ_T;

typedef struct
{
	U8 status;
	U8 opcode;
	U16 handle;
	U8 value;
}PACKED_STRUCT ATTSIGNEDWRITECMD_T;

typedef struct
{
	U16 handle;
	U16 offset;
	U8  value; 	
}PACKED_STRUCT ATTPREPAREWRITEREQ_T;

typedef struct
{
	U16 handle;
	U16 offset;
	U8  value; 	
}PACKED_STRUCT ATTPREPAREWRITERSP_T;

typedef struct
{
	U16 handle;
	U8  value; 	
}PACKED_STRUCT ATTHANDLEVALUEREPORT_T;

typedef union
{
	U8 msgBodyPtr;
	ATTERRRSP_T attErrRsp;
    ATTPREPAREWRITERSP_T AttPrepareWriteRsp;
    ATTHANDLEVALUEREPORT_T AttHandleValueReport;
}PACKED_STRUCT ATTPDUPARA;


//============================================================================
typedef struct
{
	U8 header[ATT_PDU_OFFSET];
	U16 ServerIndex;
	U16 offset;
	ATT_WRITEDATA_U dataUnion;
}PACKED_STRUCT ATT_EXECUTEWRITEREQ_T;

typedef struct att_pwq_stru
{
	struct att_pwq_stru XDATA_PTR pNextPWQ;
	U16 length;
}ATT_PREPAREWRITEQUEUE_T;

typedef enum
{
	WRITE_REQ,
	WRITE_CMD,	
	SIGNED_WRITE_CMD
}write_mux;

#define ATT_HDL_INFO_SIZE   sizeof(attHandlesInfo_t)

#define ATT_SIGNWRITE_NEW_CMD			0x00
#define ATT_SIGNWRITE_VERIFYING			0x01
#define ATT_SIGNWRITE_VALID				0x02
#define ATT_SIGNWRITE_INVALID			0x03

typedef struct {
    U8 XDATA_PTR uuid;
    U8 Len;
    U8 IsBTBase;
}MSGUUID_T;


//Global Variable============================================
typedef struct
{
	OST XDATA_PTR 		Timer;
	U32					interval;
	U16					ReportType;
	U16                 CharIndex;
	U16                 CCCDIndex;
} ATT_REPORT_TABLE_T;

typedef struct
{
	U16 valueByLink[MAX_NUM_LE_LINK];
} ATT_CCCD_T;

typedef struct
{
	ATT_CCCD_T XDATA_PTR pCCCD;
	U16 serverIdx;
}PACKED_STRUCT ATT_CCCD_INSTANCE_T;

typedef struct 
{
	U8  XDATA_PTR       ATTRsvMemPtr;
	OST XDATA_PTR 		Transactiontimer[MAX_NUM_LE_LINK];
	ATT_REPORT_TABLE_T	Table[ATT_PERIODIC_REPORT_TABLE_SIZE];
	ATT_CCCD_INSTANCE_T	cccdInstance[ATT_MAX_NUM_OF_CCCD];
	U16	                attMtu;
	// Client begin
	U16                 clientRxMtu;
	// Client end
}PACKED_STRUCT ATT_CTL_TYPE;



ATT_EXTERN ATT_CTL_TYPE XDATA gATT_ctl;
ATT_EXTERN ATT_PREPAREWRITEQUEUE_T XDATA_PTR gATT_HeadPWQ[MAX_NUM_LE_LINK];
ATT_EXTERN ATT_PREPAREWRITEQUEUE_T XDATA_PTR gATT_TailPWQ[MAX_NUM_LE_LINK];


//End Global Variable========================================

/* GATT */
//GATT definition============================================
#define GATT_PRIM_SERVICE_UUID				  (U16)0x2800
#define GATT_SND_SERVICE_UUID				  (U16)0x2801

#define ATT_2800_UUID				  UUID_B_016, 0x2800
#define ATT_2801_UUID				  UUID_B_016, 0x2801

//GATT Characteristic Properties=============================
#define CHAR_BCAST                  0x01
#define CHAR_READ                   0x02
#define CHAR_WRITE_NO_RSP           0x04
#define CHAR_WRITE                  0x08
#define CHAR_NOTIFY                 0x10
#define CHAR_INDICATE               0x20
#define CHAR_AUTHEN                 0x40
#define CHAR_EXTENDED               0x80
//============================================================



////////////////////////////////////////////////////////////////////////////////
//PDU Templates:
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  	U8 Offset;
	U8 rsvd[4];
	U16 hciHdr;
	U16 hciLen;
    U16 AttPDULength;
	U16 CID;
	U8 Opcode;
	ATTPDUPARA Parameter;
}ATTRSP_PKT_TYPE;

//[offset:0][reserved:1][HCI_hdr:2~3][HCI_len:4~5][L2C_len:6~7][CID:8~9][att opcode:10] --ATT PDU-->   //offset to data
typedef struct
{
    U8 Offset;
    U8 rsvd[3];
    U16 hciHdr;
	U16 hciLen;
    U16 L2CLen;
	U16 CID;
	U8 Opcode;
}ATTREQ_PKT_TYPE;

#define ATT_OFFSET_HEADER_LEN       (OS_OFFSET_OF(ATTRSP_PKT_TYPE, Parameter))
#define ATT_OPCODE_SIZE             (1)

#endif /* LE_SUPPORTED_HOST_FLASH */
#endif /* _ATT_H_ */