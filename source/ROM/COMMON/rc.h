#ifndef _RC_H_
#define _RC_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * rc.h defines the functions about resource control or systems.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#include "align_osmem.h"
#include "align_osmq.h"
#include "drv_3wire.h"


#if (defined AB1520B)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520B_SOC\0"
#elif (defined AB1520C)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520C_SOC\0"
#elif (defined AB1520D)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520D_SOC\0"
#elif (defined AB1520E)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520E_SOC\0"
#elif (defined AB1520SA)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520SA_SOC\0"
#elif (defined AB1520SB)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520SB_SOC\0"
#elif (defined AB1520SE)
	#define SYS_ASIC_ROM_VERSION_STRING     "AB1520SE_SOC\0"
#else
	#error 'not select ASIC version'
#endif

//warning: Charles
//- used for gpSysLocalDeviceInfo.ab1000_version
#define AB1500_SERIES                           0x80
#define AB1000S_SERIES                          0x40
#define AB1520_SERIES                           0x20


////////////////////////////////////////////////////////////////////////////////
// Type Defintions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 SYS_ERRCODE;
#define SYS_ERRCODE_SUCCESS                 (SYS_ERRCODE)(0)
#define SYS_ERRCODE_MEM_ACCESS_FAIL         (SYS_ERRCODE)(1)
#define SYS_ERRCODE_TRANSACTION_COLLISION   (SYS_ERRCODE)(2)
#define SYS_ERRCODE_DELAY_PROCESS           (SYS_ERRCODE)(3)

typedef U8 SYS_MODE;
#define SYS_MODE_DEVICE                     ((SYS_MODE)1 << 0)
#define SYS_MODE_CONTROLLER                 ((SYS_MODE)1 << 1)
#define SYS_MODE_ENGINEERING                ((SYS_MODE)1 << 2)
#define SYS_MODE_AIR_UPDATE                 ((SYS_MODE)1 << 3)

typedef U16 SYS_BOOT_REASON;
#define SYS_BOOT_NORMALLY                   ((SYS_BOOT_REASON)0)
#define SYS_BOOT_ENCOUNTER_PMU_FORCE_SIGNAL ((SYS_BOOT_REASON)1 << 0)
#define SYS_BOOT_ENCOUNTER_SDA_IS_LOW       ((SYS_BOOT_REASON)1 << 1)
#define SYS_BOOT_ENCOUNTER_UNKNOWN_FLASH    ((SYS_BOOT_REASON)1 << 2)
#define SYS_BOOT_ENCOUNTER_BAD_FLASH_CODE   ((SYS_BOOT_REASON)1 << 3)
#define SYS_BOOT_ENCOUNTER_WATCH_DOG_RST    ((SYS_BOOT_REASON)1 << 4)
#define SYS_BOOT_ENCOUNTER_DEEP_SLEEP       ((SYS_BOOT_REASON)1 << 5)
#define SYS_BOOT_ENCOUNTER_ICE_IS_ACTIVE    ((SYS_BOOT_REASON)1 << 6)
#define SYS_BOOT_ENCOUNTER_SOFTWARE_RST     ((SYS_BOOT_REASON)1 << 7)

typedef union union_sys_boot_reason
{
    SYS_BOOT_REASON value;
    struct stru_sys_boot_reason
    {
        BOOL EncounterPmuForceSignal : 1;
        BOOL EncounterSdaIsLow : 1;
        BOOL EncounterUnknownFlash : 1;
        BOOL EncounterBadFlashCode : 1;
        BOOL EncounterWatchdogRst : 1;
        BOOL EncounterDeepSleep : 1;
        BOOL EncounterIceIsActive : 1;
        BOOL EncounterSoftwareRst : 1;
        BOOL reserved_b8 : 1;
        BOOL reserved_b9 : 1;
        BOOL reserved_b10 : 1;
        BOOL reserved_b11 : 1;
        BOOL reserved_b12 : 1;
        BOOL reserved_b13 : 1;
        BOOL reserved_b14 : 1;
        BOOL reserved_b15 : 1;

    } field;

} SYS_BOOT_REASON_UNION;

typedef U8 SYS_POWER_LV;
#define SYS_POWER_MODE_ACTIVE       ((SYS_POWER_LV)0)
#define SYS_POWER_MODE_FROZEN       ((SYS_POWER_LV)1)
#define SYS_POWER_MODE_SLEEP        ((SYS_POWER_LV)2)
#define SYS_POWER_MODE_DEEP_SLEEP   ((SYS_POWER_LV)3)
#define SYS_POWER_MODE_SHUTDOWN     ((SYS_POWER_LV)4)

typedef struct stru_sys_boot_ctl
{
    SYS_MODE SysMode;
    SYS_BOOT_REASON_UNION Reason;
    SYS_POWER_LV AdaptivePowerLv;
    SYS_POWER_LV ConstrainedPowerLv;

    VOID (*fOS_Scheduler)(VOID);

    U8 UsedWorkingArea;

} SYS_BOOT_CTL_STRU;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SYS_BOOT_CTL_STRU gSysBootCtl;
EXTERN U8 XDATA  gSYS_WatchDogTimer; //warning: Charles


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//- Swap endian for constant value
#define BYTE_SWAP16(n) (((U16)(n) << 8) | ((U16)(n) >> 8))

/**
 * two_byte_swap
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
EXTERN U16 two_byte_swap (
    IN      U16 value);

/**
 * two_byte_swap
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
EXTERN U32 four_byte_swap (
    IN      U32 value);

/**
 * number_of_ones
 *
 * Count number of ones
 *
 * U32 i;
 * i = i - ((i >> 1) & 0x55555555);
 * i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
 * return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
 *
 * @Author : Charles Su<CharlesSu@airoha.com.tw>
 *
 * @n : input bit stream
 */
EXTERN U8 number_of_ones(U8 n);

/**
 * Casting Little-endian data
 */
VOID CastingWriteU16LE(
    IN      OS_DATA_PTR ptr,
    IN      U16 value);
VOID CastingWriteU32LE(
    IN      OS_DATA_PTR ptr,
    IN      U32 value);
U16 CastingReadU16LE(
    IN      OS_DATA_PTR ptr);
U32 CastingReadU32LE(
    IN      OS_DATA_PTR ptr);


/**
 * Find maximum value between operands A and B
 *
 * @author : Charles Su<CharlesSu@airoha.com.tw>
 * @param[in]   A   Operand A
 * @param[in]   B   Operand B
 * @returns :       Maximum value between operatnds A and B
 */
#define MAX(A,B) (((A) > (B)) ? (A) : (B))



/**
 * Find miniman value between operands A and B
 *
 * @author : Charles Su<CharlesSu@airoha.com.tw>
 * @param[in]   A   Operand A
 * @param[in]   B   Operand B
 * @returns :       Miniman value between operatnds A and B
 */
#define MIN(A,B) (((A) < (B)) ? (A) : (B))



//////////////////////////////////////////////////////////
//  DEBUG Pin Definination
//////////////////////////////////////////////////////////
#ifdef USED_DEBUG_PIN_P0
    #define D0(A)       P0      = A
    #define D0_0(A)     P0_0    = A
    #define D0_1(A)     P0_1    = A
    #define D0_2(A)     P0_2    = A
    #define D0_3(A)     P0_3    = A
    #define D0_4(A)     P0_4    = A
    #define D0_5(A)     P0_5    = A
    #define D0_6(A)     P0_6    = A
    #define D0_7(A)     P0_7    = A
#else
    #define D0(A)
    #define D0_0(A)
    #define D0_1(A)
    #define D0_2(A)
    #define D0_3(A)
    #define D0_4(A)
    #define D0_5(A)
    #define D0_6(A)
    #define D0_7(A)
#endif

#ifdef USED_DEBUG_PIN_P3
    #define D3(A)       P3      = A
    #define D3_0(A)     P3_0    = A
    #define D3_1(A)     P3_1    = A
    #define D3_2(A)     P3_2    = A
    #define D3_3(A)     P3_3    = A
    #define D3_4(A)     P3_4    = A
    #define D3_5(A)     P3_5    = A
    #define D3_6(A)     P3_6    = A
    #define D3_7(A)     P3_7    = A
#else
    #define D3(A)
    #define D3_0(A)
    #define D3_1(A)
    #define D3_2(A)
    #define D3_3(A)
    #define D3_4(A)
    #define D3_5(A)
    #define D3_6(A)
    #define D3_7(A)
#endif


//////////////////////////////////////////////////////////
//  Memory Block Definination
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//  Memory Block and Message Queue Declaration;
//////////////////////////////////////////////////////////
/*
 * OSMQ Configuration
 */
#define MQ_SIZE_LC_DATA_FROM_HC 16


#if 0  //warning: Charles
#define OSMEM1_BLKSIZE          pSysMemoryConfig->OSMEM1ArrayEntrySize
#define OSMEM2_SMALL_TX_NBLKS   pSysMemoryConfig->OSMEM2SmallTxEntries
#define OSMEM2_SMALL_TX_BLKSIZE pSysMemoryConfig->OSMEM2SmallTxEntrySize
#define OSMEM2_TINY_RX_NBLKS    pSysMemoryConfig->OSMEM2TinyRxEntries
#define OSMEM2_TINY_RX_BLKSIZE  pSysMemoryConfig->OSMEM2TinyRxEntrySize
#define OSMEM2_SMALL_RX_NBLKS   pSysMemoryConfig->OSMEM2SmallRxEntries
#define OSMEM2_SMALL_RX_BLKSIZE pSysMemoryConfig->OSMEM2SmallRxEntrySize
#define OSMEM2_LARGE_RX_NBLKS   pSysMemoryConfig->OSMEM2LargeRxEntries
#define OSMEM2_LARGE_RX_BLKSIZE pSysMemoryConfig->OSMEM2LargeRxEntrySize
#endif

#define OSMEM2_BLKSIZE  OSMEM2_SMALL_TX_BLKSIZE
#define OSMEM2_NBLKS    OSMEM2_SMALL_TX_NBLKS
#define OSMEM_ptr2      OSMEM_ptr2_small_TX

#define HW_TINY_RX_BUF_MAX_LEN_IN_8B    DIV_BY_8(OSMEM2_TINY_RX_BLKSIZE)
#define HW_SMALL_RX_BUF_MAX_LEN_IN_8B   DIV_BY_8(OSMEM2_SMALL_RX_BLKSIZE)
#define HW_LARGE_RX_BUF_MAX_LEN_IN_8B   DIV_BY_8(OSMEM2_LARGE_RX_BLKSIZE)


#define OSMQ_HCIDRV_Command_From_GAP_ptr	OSMQ_HC_Command_From_Bus_ptr
#define OSMQ_HCIDRV_Event_To_GAP_ptr		OSMQ_HC_Event_To_Bus_ptr


//////////////////////////////////////////////////////////////////////////
////////////////////////Memory and MSG que allocation/////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#define OSMQ_SIZE_FOR_CODE      16

////////////////////////Memory and MSG que allocation [End]/////////////////////
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//  Constant and Macro Definitions;
//////////////////////////////////////////////////////////
//data format transformation macros
#define ENDIAN_TRANSFORM_U16(xx) ((xx<<8)|((xx>>8)&0X00FF))
#define ENDIAN_TRANSFORM_U32(xx) ((xx<<24)|((xx<<8)&0X00FF0000)|((xx>>8)&0X0000FF00)|((xx>>24)&0X000000FF))

#define PTR16_LSB(ptr)      ((U8) COVERITY_PTR (ptr))               //add (U32) to prevent coverity warning
#define PTR16_MSB(ptr)      ((U8)((U16) COVERITY_PTR (ptr) >> 8))   //add (U32) to prevent coverity warning

#define U16_LSB(value)      (U8)((value) & 0xFF)
#define U16_MSB(value)      (U8)(((value) >> 8) & 0xFF)


//For local device infomation
#define MASTER                              0
#define SLAVE                               1


#define BT_PAGE_TIMEOUT                     8192    // 8192 slots = 5.12sec (default)
#define BT_ACL_CONNECTION_HANDLE            0
#define BT_PARITY_B0                        0xFF
#define BT_PARITY_B1                        0xFF
#define BT_PARITY_B2                        0xFF
#define BT_PARITY_B3                        0xFF
#define BT_PARITY_B4                        0x03
#define BT_AC_SYNC_THRESHOLD_L              0x37
#define BT_AC_SYNC_THRESHOLD_H              0x00
#define BT_LT_ADDR                          0x03
#define BT_BD_ADDR_B0                       0x11
#define BT_BD_ADDR_B1                       0x22
#define BT_BD_ADDR_B2                       0x33
#define BT_BD_ADDR_B3                       0x44
#define BT_BD_ADDR_B4                       0x55
#define BT_BD_ADDR_B5                       0x66
#define BT_SR_MODE0                         0x0
#define BT_SR_MODE1                         0x1
#define BT_SR_MODE2                         0x2
#define BT_SR_MODE                          BT_SR_MODE0
#define BT_COD_B0                           0xFF
#define BT_COD_B1                           0xFF
#define BT_COD_B2                           0xFF
#define BT_PAGE_SCAN_MODE                   0x0 // All others options are reserved
#define BT_RAND_MASK                        0x07FF
#define BT_INTERLACED_SCAN                  TRUE
#define BT_BE_MASTER                        TRUE

#define LINK_ACTIVITY_IDLE                  0x00
#define LINK_ACTIVITY_ACL_ACTIVE            0x01
#define LINK_ACTIVITY_LM_ACTIVE             0x02
#define LINK_ACTIVITY_L2CAP_ACTIVE          0x03




//========================================== ADC, Charger , Power
#define RC_CHARGER_BY_MCU                           ((BIT)0)
#define RC_CHARGER_BY_AUTO                          ((BIT)1)

/* Temperature Compensation Relative */
#define RC_INVALID_TEMP_BANK                        ((U8)(0xFF))

//#if defined(AB1500)

typedef enum
{
	MCU_ADC_VAIO_0,
	MCU_ADC_VAIO_1,
	MCU_ADC_VAIO_2,
	MCU_ADC_VAIO_3,
	MCU_ADC_VAIO_4,
	MCU_ADC_VAIO_5,
	MCU_ADC_VAIO_6,
	MCU_ADC_VAIO_7,
	MCU_ADC_VAIO_8,
	MCU_ADC_VAIO_9,
	MCU_ADC_VAIO_10,
	MCU_ADC_VAIO_11,
	MCU_ADC_VAIO_12,
	MCU_ADC_VAIO_13,
	MCU_ADC_VAIO_14,
	MCU_ADC_VAIO_15,
	MCU_ADC_VGPIO_GND,
	MCU_ADC_VGPIO_1_VCCDIG,
	MCU_ADC_VGPIO_2_VCCDIG,
	MCU_ADC_VGPIO_3_VCCDIG,
	MCU_ADC_VGPIO_ADC_VCM,
	NUM_OF_MCU_ADC_TYPE
}ENUM_MCU_ADC_TYPE;
//#endif


//////////////////////////////////////////////////////////
//  BOOT Sequence Definination
//////////////////////////////////////////////////////////
enum enum_sys_sw_status1
{
    SYS_SW1_BOOT_FLAG_POWER_ON_READY            = (U8)BIT_MASK8(0),
    SYS_SW1_BOOT_FROM_CHG_IN                    = (U8)BIT_MASK8(1),
    SYS_SW1_BOOT_FLAG_OF_ONE_CONNECTED          = (U8)BIT_MASK8(2),
    SYS_SW1_BOOT_FLAG_OF_TWO_CONNECTED          = (U8)BIT_MASK8(3),
    SYS_SW1_CTL_ENABLE_GPIO_INT_FOR_S_NORMAL    = (U8)BIT_MASK8(4),
    SYS_SW1_BOOT_FLAG_ENTER_MP_TEST_MODE	    = (U8)BIT_MASK8(5),
};

#define PMU_SYS_GET_BOOT_SW_STATUS_B1()                     (LC_SFR_READ_BY_C2(C2OFFS_RW_MCU_REG2))
#define PMU_SYS_SET_BOOT_SW_STATUS_B1(A)                    LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_REG2,(A))

#define SYS_GetNextLinkIndex(A)                             ((U8)((A + 1) % MAX_MULTI_POINT_NO))

//////////////////////////////////////////////////////////
//  BEGIN: SYSTEM MUST Define Constant Area!!!
//////////////////////////////////////////////////////////
//warning: Charles
//Time related, unit for timer setting.
#define HALF_SEC	(1600L)
#define ONE_SEC		(3200L)
#define ONE_MIN		(60 * ONE_SEC)
#define HALF_MIN	(30 * ONE_SEC)
//////////////////////////////////////////////////////////
//  END: SYSTEM MUST Define Constant Area!!!
//////////////////////////////////////////////////////////


//========================================== SYSTEM misc support feature
typedef struct //warning: Charles
{
    U8  audio_C2_57;
    U8  audio_C2_58;
    U8  audio_C2_59;
    U8  audio_C2_60;
    U8  audio_C2_61;
    U8  audio_C2_62;
    U8  audio_C2_63;
    U8  audio_C2_64;
    U8  audio_C2_65;
    U8  audio_C2_66;
    U8  isAudioEnabled;
} SYS_AUDIO_CTL;

//----------------------------------
//      system global variables
//----------------------------------
#define MAX_NUM_BLACK_LIST  1



typedef struct
{
    U8 valid;
    BD_ADDR_STRU bd_addr;

} SYS_BLACK_LIST;



/**
 * SYS_3WireWrite - Write Analog front end register by 3-wire SPI
 * @Addr :          register address
 * @HiByte :        most significant byte
 * @LoByte :        last significant byte
 *
 * This function is used to write analog front end register by 3-wire SPI
 * interface. And this function handle the interface wrapper hardware is
 * busy, then wait the hardware is ready to write next register.
 */
#define SYS_3WireWrite(A,Hi,Lo)     DRV_3WIRE_Write((A),((U16)(Hi) << 8) | (Lo))
#define SYS_3WireRead(A,HiLoU16)    HiLoU16 = DRV_3WIRE_Read((A))


////////////////////////////////////////////////////////////////////////////////
// Legacy Legacy ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* Legacy Legacy Legacy Legacy Legacy Legacy Legacy Legacy */
#define DelayInMS(A) OS_delay_ms(A)

    typedef struct
    {
        U16                         osmem2_large_rx_blksize;
        U8                          osmem2_large_rx_nblks;
    } OSMEM2_LARGE_RX_PARAMETER;
    EXTERN OSMEM2_LARGE_RX_PARAMETER         XDATA   gSys_large_rx_parameter;

	PUBLIC VOID HC_EnableLightDbgPrint(VOID);
	PUBLIC VOID HC_DisableLightDbgPrint(VOID);
    PUBLIC VOID LightDbgPrintWithOpcode (U8 vcm_opcode, ...) compact;
    PUBLIC VOID LightDbgPrintWithConnectionHandle (U16 vcm_opcode_and_ogf, U16 connection_handle, U8 XDATA_PTR ptr1) compact;
    PUBLIC VOID LightDbgPrint (U8 * stringPtr, ...);
    PUBLIC void LightDbgPrintDsp (U8 XDATA_PTR ptr1);
    PUBLIC VOID SYS_AssertUARTEventGenerator (U8 condition, U16 line);


    #define ASSERT(x) SYS_AssertUARTEventGenerator((x), (U16)__LINE__)
    #define ASSERT_PRINT(x) SYS_AssertUARTEventGenerator(FALSE, (U16)__LINE__)



////////////////////////////////////////////////////////////////////////////////
// SFR WRITE INTERFACE /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * SFR_Write
 *
 * Write the specific SFR
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Addr : The address of SFR
 * @Data : The data to write
 */
VOID SFR_Write(
    IN      U8 Addr,
    IN      U8 Data);

/**
 * SFR_BankWrite
 *
 * Write the specific Bank SFR
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @BankAddr : The address of Bank SFR
 * @Offset : Offset value
 * @Data : The data to write
 */
VOID SFR_BankWrite(
    IN      U8 BankAddr,
    IN      U8 Offset,
    IN      U8 Data);

/**
 * SFR_Read
 *
 * Read the specific SFR
 *
 * Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @Addr : The address of SFR
 * @Return : The value of SFR read
 */
U8 SFR_Read(
    IN      U8 Addr);

/**
 * SFR_BankRead
 *
 * Read the specific Bank SFR
 *
 * Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @BankAddr : The address of Bank SFR
 * @Offset : Offset value
 * @Return : The value of Bank SFR read
 */
U8 SFR_BankRead(
    IN      U8 BankAddr,
    IN      U8 Offset);





PUBLIC VOID SYS_Init (VOID);
PUBLIC VOID SYS_ReleaseTimer ( OST XDATA_PTR XDATA_PTR timer_ptr );
PUBLIC VOID SYS_SetTimer(OST XDATA_PTR XDATA_PTR timerPtr, U32 interval);
PUBLIC BOOL SYS_IsTimerExpired(OST XDATA_PTR XDATA_PTR timerPtr);
//PUBLIC U8 XDATA_PTR SYS_MemoryGet(U16 length);
#define SYS_MemoryGet OS_malloc
PUBLIC VOID SYS_MemoryRelease(VOID XDATA_PTR XDATA_PTR mem_ptr);

#define SYS_IsBDAddrIsTheSame(ptr1,ptr2) (!OS_memcmp((ptr1),(ptr2),sizeof(BD_ADDR_STRU)))


EXTERN PUBLIC VOID RC_Init (VOID);

VOID SYS_BtParityCalc(IN LAP_ARR XDATA_PTR pLAP,OUT PARITY_ARR XDATA Parity);

//EXTERN PUBLIC VOID SYS_SelectADCSource(VOID);
EXTERN PUBLIC VOID SYS_Reset_Event_Gen(VOID);

//Black list related functions;
EXTERN VOID SYS_BlackList_Add(BD_ADDR_STRU XDATA_PTR bd_addr_ptr);
EXTERN VOID SYS_BlackList_Delete(BD_ADDR_STRU XDATA_PTR bd_addr_ptr);
EXTERN BOOL SYS_BlackList_Search(BD_ADDR_STRU XDATA_PTR bd_addr_ptr);

EXTERN VOID OS_InsertTask(U8 taskID) compact REENTRANT;



#ifdef SUPPORT_SCO_ESCO
extern SYS_AUDIO_CTL	XDATA	gSYS_audio_ctl;
#endif
extern SYS_BLACK_LIST	XDATA	gSYS_black_list[MAX_NUM_BLACK_LIST];


EXTERN BIT gOS_HC_Task_Only;



#endif /* _RC_H_ */

