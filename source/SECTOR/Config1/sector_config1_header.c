/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_config1_header.c defines the rest parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_config1.h"
#include "ctlr_config.h"

#pragma userclass (HCONST = CFGHEAD)


EXTERN SECTOR_SYS_LOCAL_DEVICE_INFO_STRU CODE gSector_SysLocalDeviceInfo;
EXTERN SECTOR_SYS_LOCAL_DEVICE_EIR_STRU CODE gSector_SysLocalDeviceEIR;
EXTERN SECTOR_SYS_LOCAL_DEVICE_CONTROL_STRU CODE gSector_SysLocalDeviceControl;
EXTERN SECTOR_LC_MISC_CONTROL_STRU CODE gMISC_ctl_init;
EXTERN SECTOR_LM_PARAMETER_STRU CODE gSector_LmParameter;
EXTERN SECTOR_HC_PARAMETER_STRU CODE gHC_parameter_init;
EXTERN SECTOR_UART_PARAMETER_STRU CODE gSector_UartParameter;
EXTERN SECTOR_DSP_NVRAM_CTL_STRU CODE gMcu_dsp_parameter_init;
EXTERN SECTOR_A2DP_NVRAM_CTL_STRU CODE gA2DP_nvram_ctl_init;
EXTERN SECTOR_DRIVER_LED_DATA_STRU CODE gDriver_led_data_init;
EXTERN SECTOR_DRIVER_RESERVED_DATA_STRU CODE gDriver_reserved_data_init;
EXTERN U16/*SECTOR_DRIVER_RINGTONE_DATA_STRU*/ CODE gDriver_ringtone_data_init;
EXTERN SECTOR_MMI_DRIVER_NVRAM_BACKUP_STRU CODE gMMI_Driver_v_nvram_backup;
EXTERN SECTOR_MMI_NVRAM_STRU CODE gMMI_nvram_init;
EXTERN U8/*SECTOR_MMI_NVRAM_KEYMAP_STRU*/ CODE gMMI_nvram_keymap_init[];
EXTERN SECTOR_SYS_MEM_CONF_STRU CODE gSector_SysMemoryConfig;
EXTERN SECTOR_SM_NVRAM_STRU CODE gSM_nvram_init;
EXTERN SECTOR_GAP_NVRAM_STRU CODE gGAP_nvram_init;
EXTERN SECTOR_DRIVER_CTL_STRU CODE gDriver_ctl_init;
#ifdef AB1520B
EXTERN SECTOR_MMI_LE_NVRAM_STRU CODE gMMI_le_nvram_init;
#endif
EXTERN SECTOR_APPLICATION_STRU CODE gApplication_init;
#ifdef IAP2_Profile
EXTERN IAP2SyncPayload/*SECTOR_IAP2SyncPayload*/ CODE gIAP2_Sync_Payload;
EXTERN U8/*SECTOR_IAP2IdenParam*/ CODE gIAP2_Iden_Param[];
#endif
#ifndef AB1520B
EXTERN SECTOR_PATCH_CODE_INIT_STRU CODE gSector_SysPatchCodeInit;
#endif

EXTERN SECTOR_LE_MISC_CTL_STRU CODE gLE_MISC_Ctl_Init;
#if (HW_SPI_SUPPORTED)
EXTERN SECTOR_SPI_CONFIG_STRU CODE gSectorSpiConfig;
#endif


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_CFG1_HEADER_STRU CODE gSectorConfig1Header =
{
    NUM_OF_SECTOR_CFG1,

    {
	    #include "sector_config1_member.h"
	},

	//- CRC
	{0,0}
};



