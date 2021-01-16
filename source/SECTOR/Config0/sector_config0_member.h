SECTORS_ENUM_MACRO(SECTOR_AFE_INIT, &gSector_AfeInit, sizeof(AFE_INIT_STRU), CRC_CHK)

SECTORS_ENUM_MACRO(SECTOR_SFR_INIT, &gSector_SfrInit, sizeof(SFR_INIT_STRU), CRC_CHK)

SECTORS_ENUM_MACRO(SECTOR_RF_MISC_CTL, &gSector_RfMiscCtl, sizeof(RF_MISC_CTL_STRU), CRC_CHK)

SECTORS_ENUM_MACRO(SECTOR_HW_MISC_CTL, &gSector_HwMiscCtl, sizeof(HW_MISC_CTL_STRU), CRC_CHK)

#ifdef AB1520B
#else
SECTORS_ENUM_MACRO(SECTOR_UART_PARAMETER_ENG, &gSector_UartParameterEng, sizeof(UART_PARAMETER_STRU), CRC_CHK)
#endif