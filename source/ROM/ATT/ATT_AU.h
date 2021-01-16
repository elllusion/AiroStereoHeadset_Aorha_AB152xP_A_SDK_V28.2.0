#ifdef AIR_UPDATE_ENABLE
//////////////////////////////////////////////////////////
// Copyright@ 2014 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////

#define _ATT_AU_H_

#include "os.h"
#include "ATT_AU_server.h"
#ifdef _ATT_AU_C_
#define ATT_AU_EXTERN
#else
#define ATT_AU_EXTERN extern
#endif

// Config Unit Test==========================================================
//#define ATT_UNIT_TEST
#define ATT_UNIT_TEST_START_STATE TEST_EXCHANGE_MTU
#define ATT_UNIT_TEST_STOP_STATE  TEST_END

// ATT packet offset=========================================================
#define ATT_L2CAP_LENGTH_OFFSET 0
#define ATT_L2CAP_OPCODE_OFFSET 12
#define ATT_PDU_OFFSET 13       //[offset:0][reserved:1~7][L2C_len:8~9][CID:10~11][att opcode:12] --ATT PDU-->   //offset to data
#define ATT_RSP_PKT_OFFSET 9
#define ATT_RSP_HEADER_LEN 16   //[offset:0][reserved:1~8][L2C_len:9~10][CID:11~12][att opcode:13][att handle:14~15]
#define ATT_HEADER_LEN 3        //[att opcode:13][att handle:14~15]
//end ATT packet offset======================================================

// ATT Macro
#define ATT_GET_L2CAP_PACKET_LENGTH(ptr)  (*(*(ptr)+(ptr)) + (*(*(ptr)+(ptr)+1)<<8))
#undef MIN
#define MIN(A, B) ((A < B) ? A : B)
//end ATT Macro

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

#define MAX_ATT_LIST_NUM_BTUUID			5
#define MAX_ATT_LIST_NUM_UUID			2

#define ATT_DEFAULT_MTU_SIZE			23
#define MAX_ATT_VALUE_LENGTH			512
#define ATT_MAX_NUM_HANDLES_INFO        5

#define INITIAL_SEARCH_INDEX			0x0000
#define ATT_RESERVED_HANDLE				0x0000
#define ATT_INVALID_INDEX				0x0000
//ATT PDU format==============================================================

typedef struct
{
  	U8 ReqOpcodeInErr;
  	U16 AttHandleInErr;
  	U8 ErrCode;
} ATTERRRSP_T;

typedef struct
{
  	U16 clientRxMTU;
} attExchangeMTUReq_t;

typedef struct
{
  	U16 serverRxMTU;
} attExchangeMTURsp_t;

typedef struct
{
  	U16 startHandle;
  	U16 endHandle;
} AttFindInfoReq_T;

typedef struct
{
    U8 format;
    U8 pInfoData[ATT_DEFAULT_MTU_SIZE-2];
} AttFindInfoRsp_T;

typedef struct
{
	U16 handle;
  	U16 BTUUID;
} attHandleBTUUIDPair_t;

typedef struct
{
  	U16 startHandle;
  	U16 endHandle;
  	U16 type;
  	U8 value; //ATT_MTU-7
} ATTFINDBYTYPEVALUEREQ_T;

typedef struct
{
  	U16 handle;
  	U16 grpEndHandle;
} attHandlesInfo_t;

typedef struct
{
  	U8 pHdlList[ATT_DEFAULT_MTU_SIZE-1];
} AttFindByTypeValueRsp_t;

typedef struct
{
	U16 startHandle;
	U16 endHandle;
	U16 type;
} AttReadByTypeReq_t;

typedef struct
{
	U8 len;
	U8 pDataList[ATT_DEFAULT_MTU_SIZE-2];
} AttReadByTypeRsp_t;

typedef struct
{
    U16 handle;
} attReadReq_t;

typedef struct
{
    U8 value;
} ATTREADRSP_T;

typedef struct
{
    U16 handle;
    U16 offset;
} attReadBlobReq_t;

typedef struct
{
	U8 partOfValue;
} attReadBlobRes_t;

typedef struct
{
	U16 handle[(ATT_DEFAULT_MTU_SIZE-1)/2];
} AttReadMultiReq_t;

typedef struct
{
    U8 pSetOfValue[ATT_DEFAULT_MTU_SIZE-1];
}AttReadMultiRsp_t;

typedef struct
{
	U16 startHandle;
	U16 endHandle;
	U16 type;
} AttReadByGroupTypeReq_t;

typedef struct
{
    U8 Length;
    U8 pAttDataList[ATT_DEFAULT_MTU_SIZE-2];
} AttReadByGroupTypeRsp_t;

typedef struct
{
	U16 handle;
    U8 value; 
} ATTWRITEREQ_T;

typedef struct
{
	U16 handle;
	U16 offset;
	U8  value; 	
} ATTPREPAREWRITEREQ_T;

typedef struct
{
	U16 handle;
	U16 offset;
	U8  value; 	
} ATTPREPAREWRITERSP_T;

typedef struct
{
	U16 handle;
	U8  value; 	
} ATTHANDLEVALUEREPORT_T;


typedef union
{
	U8                      msgBodyPtr;
	ATTERRRSP_T             AttErrRsp;
    ATTPREPAREWRITERSP_T    AttPrepareWriteRsp;
    ATTHANDLEVALUEREPORT_T  AttHandleValueReport;
    AttReadByTypeRsp_t      AttReadByTypeRsp;
}ATTPDUPARA;


//============================================================================
typedef struct
{
	U16 handle;
	U16 offset;
	U16 size;
	U8 value[ATT_DEFAULT_MTU_SIZE-5];
} ATT_PREPAREWRITEQUEUE_T;

typedef enum
{
	WRITE_REQ,
	WRITE_CMD,	
	SIGNED_WRITE_CMD
}write_mux;

#define MAX_PREPARE_WRITE_QUEUE 3
 

enum {
	ATT_R_PERMIT    = 0x01,			/* Always permitted, no restrictions*/
	ATT_R_AUTHEN    = 0x02,	        /* Authentication required */
	ATT_R_AUTHOR    = 0x04,	        /* Authorization required */
	ATT_R_BANNED    = 0x08,	        /* Operation not permitted */

    ATT_W_PERMIT    = 0x10,			/* Always permitted, no restrictions*/
	ATT_W_AUTHEN    = 0x20,	        /* Authentication required */
	ATT_W_AUTHOR    = 0x40,	        /* Authorization required */
	ATT_W_BANNED    = 0x80,	        /* Operation not permitted */
};

typedef struct {
    U8 XDATA_PTR uuid;
    U8 Len;
    U8 IsBTBase;
}MSGUUID_T;


//Global Variable============================================
typedef struct 
{
	U8	                attMtu;
    U8 XDATA_PTR        ATTRsvMemPtr;
} ATT_AU_CTL_TYPE;

typedef struct
{
    U8 flashAddr[AU_ADDR_LEN];
    U8 currentIdx;
    U8 XDATA_PTR memPtr;   
    U8 XDATA_PTR ptr;    //bufferMemPtr AlignToNext4ByteAddr
}AU_BUFFER_TYPE;

typedef struct
{
    U8 state;
    U8 SendReadData;
	OST XDATA_PTR timer;
    U16 writeCount;     //number of page to be writed
    AU_BUFFER_TYPE buffer;
}AU_CTL_TYPE;

ATT_AU_EXTERN AU_CTL_TYPE XDATA gAU_ctl;

ATT_AU_EXTERN ATT_PREPAREWRITEQUEUE_T XDATA att_AU_PWQ[MAX_PREPARE_WRITE_QUEUE];
ATT_AU_EXTERN U8 XDATA gATT_AU_PWQ_index;   //this value should be initialized to zero
 
ATT_AU_EXTERN ATT_AU_CTL_TYPE XDATA gATT_AU_ctl;
//End Global Varialbe========================================

/* GATT */
//GATT definition============================================
#define GATT_PRIM_SERVICE_UUID				  0x2800
#define GATT_SND_SERVICE_UUID				  0x2801
//GATT Characteristic Properties=============================
#define CHAR_BCAST                  0x01
#define CHAR_READ                   0x02
#define CHAR_WRITE_NO_RSP           0x04
#define CHAR_WRITE                  0x08
#define CHAR_NOTIFY                 0x10
#define CHAR_INDICATE               0x20
#define CHAR_AUTHEN                 0x40
#define CHAR_EXTENDED               0x80
//server=====================================================
typedef enum
{
	PROFILE_BATTERY_SERVICE,
	TOTAL_SUPPORTED_PROFILES
} SupportedProfiles;


////////////////////////////////////////////////////////////////////////////////
//PDU Templates:
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
  	U8 Offset;
	U8 reserved[8];
    U16 AttPDULength;
	U8 reservedCID[2];
	U8 Opcode;
	ATTPDUPARA Parameter;
}ATTRSP_PKT_TYPE;

#endif
