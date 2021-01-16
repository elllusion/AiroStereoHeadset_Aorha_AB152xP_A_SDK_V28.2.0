#ifndef _HW_SFR_H_
#define _HW_SFR_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * HW_SRF.h is the file defines the Airoha proprietory registers in 80251 SFR
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"



////////////////////////////////////////////////////////////////////////////////
// Version Control /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !(CHIP_MODEL == 1520)
#error "Unsuitable SFR Definitions"
#endif



////////////////////////////////////////////////////////////////////////////////
// SFR Definitions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * USER DEFINED BYTE-ADDRESSABLE SFR
 */
/* Indirect bank */
sfr     SFR_RW_C2                       = 0xC2;
sfr     SFR_RW_C3                       = 0xC3;
sfr     SFR_RW_E2                       = 0xE2;
sfr     SFR_RW_E3                       = 0xE3;
sfr     SFR_RW_E4                       = 0xE4;
sfr     SFR_RW_E6                       = 0xE6;
sfr     SFR_RW_E7                       = 0xE7;
sfr     SFR_RW_FB                       = 0xFB;
sfr     SFR_RW_FE                       = 0xFE;

/* Direct bank */
sfr     SFR_RW_C0                       = 0xC0;
sfr     SFR_RW_OFFSET                   = 0xC1;
sfr     SFR_RW_INTERLACE_OFFSET         = 0xC4;
sfr     SFR_WO_HOP_SEQUENCE             = 0xD2;
sfr     SFR_RW_HOPFREQ                  = 0xD3;
sfr     SFR_RW_CLKSEL                   = 0xD4;
sfr     SFR_RW_SWC_INTERRUPT_ENABLE     = 0xD5;
sfr     SFR_RO_INTERRUPT_VECTOR_B0      = 0xD6;
sfr     SFR_RO_INTERRUPT_VECTOR_B1      = 0xD7;
sfr     SFR_WO_ENCRYPTION_CONTROL       = 0xD7;
sfr     SFR_RW_INTR_VECTOR0_SEL         = 0xD9;
sfr     SFR_RO_INTERRUPT_VECTOR_B2      = 0xDA;
sfr     SFR_RW_BT_LE_INTR_PRIO_CTL      = 0xDB;
sfr     SFR_RO_CLKN_B0                  = 0xDC;
sfr     SFR_RO_CLKN_B1                  = 0xDD;
sfr     SFR_RO_CLKN_B2                  = 0xDE;
sfr     SFR_RO_CLKN_B3                  = 0xDF;
sfr     SFR_WR_TEST_AND_SET             = 0xE1;
sfr     SFR_RW_HOP_FREQ                 = 0xE5;
sfr     SFR_RW_F8                       = 0xF8;
sfr     SFR_RO_UART_RBR                 = 0xF9;
sfr     SFR_WO_UART_THR                 = 0xF9;
sfr     SFR_WO_DMA_CONTROL              = 0xFC;
sfr     SFR_RO_DMA_STATUS               = 0xFC;

/**
 * USER DEFINED BIT-ADDRESSABLE SFR
 */
sbit    SFRB_RW_MANUAL_RXWINDOW         = SFR_RW_C0^0;
sbit    SFRB_RW_TX_FLOW                 = SFR_RW_C0^1;
sbit    SFRB_RW_ENABLE_TX               = SFR_RW_C0^2;
sbit    SFRB_RW_ENABLE_RX               = SFR_RW_C0^3;
sbit    SFRB_RW_TX_SEQN                 = SFR_RW_C0^4;
sbit    SFRB_RW_TX_ARQN                 = SFR_RW_C0^5;
sbit    SFRB_WO_MEMCPY_START            = SFR_RW_C0^6;
sbit    SFRB_RO_MEMCPY_BUSY             = SFR_RW_C0^7;

sbit    SFRB_RW_ID_SEL                  = SFR_RW_F8^0;
sbit    SFRB_RW_EDR_SEL                 = SFR_RW_F8^1;
sbit    SFRB_RW_WHITENING_SEL           = SFR_RW_F8^2;
sbit    SFRB_RW_ENABLE_WHITENING        = SFR_RW_F8^3;
sbit    SFRB_RW_KOFFSET                 = SFR_RW_F8^4;
sbit    SFRB_RW_HOP_TR                  = SFR_RW_F8^6;
sbit    SFRB_RO_HOP_OK                  = SFR_RW_F8^7;



////////////////////////////////////////////////////////////////////////////////
// Banked-SFR Definitions //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * Airoha SRF C2 Bank Register Address Definition
 */
#define C2OFFS_RW_RX_WINDOW_LENGTH              0
#define C2OFFS_RW_TR_AC_BD_ADDR_B0              26
#define C2OFFS_RW_TR_AC_BD_ADDR_B1              27
#define C2OFFS_RW_TR_AC_BD_ADDR_B2              28
#define C2OFFS_RW_TR_AC_BD_ADDR_B3              29
#define C2OFFS_RW_TR_AC_BD_ADDR_B4              30
#define C2OFFS_RW_TR_AC_BD_ADDR_B5              31
#define C2OFFS_RW_TR_AC_PARITY_B0               32
#define C2OFFS_RW_TR_AC_PARITY_B1               33
#define C2OFFS_RW_TR_AC_PARITY_B2               34
#define C2OFFS_RW_TR_AC_PARITY_B3               35
#define C2OFFS_RW_TR_AC_PARITY_B4               36
#define C2OFFS_RW_HOP_BD_ADDR_LAP_B0            37
#define C2OFFS_RW_HOP_BD_ADDR_LAP_B1            38
#define C2OFFS_RW_HOP_BD_ADDR_LAP_B2            39
#define C2OFFS_RW_TX_PACKET_LT_ADDR             44
#define C2OFFS_RW_TX_PACKET_TYPE                45
#define C2OFFS_RW_TX_PAYLOAD_LENGTH_L           46
#define C2OFFS_RW_TX_PAYLOAD_LENGTH_H           47
#define C2OFFS_RW_RX_ESCO_LT_ADDR               48
#define C2OFFS_RW_SLAVE_RX_BROADCAST_ENCR       49
#define C2OFFS_RW_SLAVE_SCO_RX_EXCEPTION        49
#define C2OFFS_RO_RXBUF_IDX                     51
#define C2OFFS_RO_INT_NUMBER                    51
#define C2OFFS_RW_PACKET_LT_ADDR                52
#define C2OFFS_RW_AU_DATA_TO_ACL_MPTR           52
#define C2OFFS_RW_MAX_PAYLOAD_SIZE_TEST         52
#define C2OFFS_RW_SFR_ESCO_SW_MODE              52
#define C2OFFS_RW_AUDIO_ENABLE                  57
#define C2OFFS_RW_AUDIO_SIDETONE_GC_ENABLE      61
#define C2OFFS_RW_DIG_AUDIO_CLKSEL              66
#define C2OFFS_RW_RX_EVLENGTH_L                 67
#define C2OFFS_RW_RX_EVLENGTH_H                 68
#define C2OFFS_RO_RF_CAL_ON                     70
#define C2OFFS_RW_CHGFLO_BYPASS_1P8V            77
#define C2OFFS_RW_TX_DELAY_AFTER_SLOT_TICK      80

#ifdef AB1520S
#define C2OFFS_RO_RX_BUF0_PKT_HDR_B0            94
#define C2OFFS_RO_RX_BUF0_PKT_HDR_B1            95
#define C2OFFS_RO_RX_BUF1_PKT_HDR_B0            96
#define C2OFFS_RO_RX_BUF1_PKT_HDR_B1            97
#define C2OFFS_RO_RX_BUF2_PKT_HDR_B0            98
#define C2OFFS_RO_RX_BUF3_PKT_HDR_B1            99

#define C2OFFS_RO_RX_BUF0_PAYLOAD_HDR_B0        100
#define C2OFFS_RO_RX_BUF0_PAYLOAD_HDR_B1        101
#define C2OFFS_RO_RX_BUF1_PAYLOAD_HDR_B0        102
#define C2OFFS_RO_RX_BUF1_PAYLOAD_HDR_B1        103
#define C2OFFS_RO_RX_BUF2_PAYLOAD_HDR_B0        104
#define C2OFFS_RO_RX_BUF2_PAYLOAD_HDR_B1        105
#endif

#define C2OFFS_RO_BTC_RX_PAYLOAD_LENGTH_L       111
#define C2OFFS_RO_BTC_RX_PAYLOAD_LENGTH_H       112
#define C2OFFS_RW_RFCTRL_SW                     113
#define C2OFFS_RW_REG3_TR_MCU                   115
#define C2OFFS_RW_PAON_TIME                     121
#define C2OFFS_RW_RF_PLLON_TIME                 123
#define C2OFFS_RW_RF_RXON_TIME                  124
#define C2OFFS_RW_RF_TXON_TIME                  125
#define C2OFFS_RW_EDR_TX_GUARD_TIME             130
#define C2OFFS_RW_TX_GFSK_SWITCH_DELAY          132
#define C2OFFS_RW_EOT_RF_TURNOFF_DELAY          134
#define C2OFFS_RW_I2C_RD_BUFFER_ADDR_B0         139
#define C2OFFS_RW_I2C_RD_BUFFER_ADDR_B1         140
#define C2OFFS_RO_I2C_STATUS                    141
#define C2OFFS_RO_I2C_BUSY                      142
#define C2OFFS_WO_I2C_WR_BUFFER_ADDR_B0         141
#define C2OFFS_WO_I2C_WR_BUFFER_ADDR_B1         142
#define C2OFFS_RW_I2C_CONFIG_B0                 143
#define C2OFFS_RW_I2C_CONFIG_B1                 144
#define C2OFFS_RW_CLK3_INTERVAL                 145
#define C2OFFS_RW_CLK3_OFFSET_B0                146
#define C2OFFS_RW_CLK3_OFFSET_B1                147
#define C2OFFS_RW_CLK3_RT1_TT1_OFFSET_B0        148
#define C2OFFS_RW_CLK3_RT1_TT1_OFFSET_B1        149
#define C2OFFS_RW_CLK2_RT1_TT1_OFFSET_B0        150
#define C2OFFS_RW_CLK2_RT1_TT1_OFFSET_B1        151
#define C2OFFS_RW_CLK0_RT1_TT1_OFFSET_B0        152
#define C2OFFS_RW_CLK0_RT1_TT1_OFFSET_B1        153
#define C2OFFS_RW_CLK1_RT1_TT1_OFFSET_B0        154
#define C2OFFS_RW_CLK1_RT1_TT1_OFFSET_B1        155
#define C2OFFS_RW_CLK0_OFFSET_B0                156
#define C2OFFS_RW_CLK0_OFFSET_B1                157
#define C2OFFS_RW_CLK0_INTERVAL                 158
#define C2OFFS_RW_CLK1_OFFSET_B0                159
#define C2OFFS_RW_CLK1_OFFSET_B1                160
#define C2OFFS_RW_CLK1_INTERVAL                 161
#define C2OFFS_RW_CLK2_OFFSET_B0                162
#define C2OFFS_RW_CLK2_OFFSET_B1                163
#define C2OFFS_RW_CLK2_INTERVAL                 164
#define C2OFFS_RO_CLKN_PHASE_AT_AC_SYNC_B0      165
#define C2OFFS_RO_CLKN_PHASE_AT_AC_SYNC_B1      166
#define C2OFFS_RO_CLKN_SYNC_B0                  167
#define C2OFFS_RO_CLKN_SYNC_B1                  168
#define C2OFFS_RO_CLKN_SYNC_B2                  169
#define C2OFFS_RO_CLKN_SYNC_B3                  170
#define C2OFFS_RW_CLKX_PHASE_SEL                172
#define C2OFFS_RO_CLKX_PHASE_B0                 173
#define C2OFFS_RO_CLKX_PHASE_B1                 174
#define C2OFFS_RW_CLK0_B0                       175
#define C2OFFS_RW_CLK0_B1                       176
#define C2OFFS_RW_CLK0_B2                       177
#define C2OFFS_RW_CLK0_B3                       178
#define C2OFFS_RW_CLK1_B0                       179
#define C2OFFS_RW_CLK1_B1                       180
#define C2OFFS_RW_CLK1_B2                       181
#define C2OFFS_RW_CLK1_B3                       182
#define C2OFFS_RW_CLK2_B0                       183
#define C2OFFS_RW_CLK2_B1                       184
#define C2OFFS_RW_CLK2_B2                       185
#define C2OFFS_RW_CLK2_B3                       186
#define C2OFFS_RW_CLK3_B0                       187
#define C2OFFS_RW_CLK3_B1                       188
#define C2OFFS_RW_CLK3_B2                       189
#define C2OFFS_RW_CLK3_B3                       190
#define C2OFFS_RW_WD_TIME_B0                    191
#define C2OFFS_RW_WD_TIME_B1                    192
#define C2OFFS_RW_WD_TIME_B2                    193
#define C2OFFS_WO_WD_CLEAR                      194
#define C2OFFS_RW_WD_ENABLE                     195
#define C2OFFS_RW_CLKE_CONTROL                  196
#define C2OFFS_RO_BIST_RESULT_B0                198
#define C2OFFS_RO_BIST_RESULT_B1                199
#define C2OFFS_RW_CLK_GATING_CONTROL            200
#define C2OFFS_RW_MCU_CLOCK_RATE_SEL            201
#define C2OFFS_RW_SAC_UPDATE_CLK_OFFSET_MAP     202
#define C2OFFS_RW_CLK_ADDING_OFFSET_B0          203
#define C2OFFS_RW_CLK_ADDING_OFFSET_B1          204
#define C2OFFS_RW_RT1_TT1_ADDING_OFFSET_B0      205
#define C2OFFS_RW_RT1_TT1_ADDING_OFFSET_B1      206
#define C2OFFS_RW_REFRENCE_OFFSET_B0            207
#define C2OFFS_RW_REFRENCE_OFFSET_B1            208
#define C2OFFS_WO_SWC_UPDATE_CLK_OFFSET_IDX     209
#define C2OFFS_RW_MASTER_CLK_TRACKING_ENABLE    210
#define C2OFFS_RW_MASTER_SLAVE0_CLK_OFFSET_B0   211
#define C2OFFS_RW_MASTER_SLAVE0_CLK_OFFSET_B1   212
#define C2OFFS_RW_MASTER_SLAVE1_CLK_OFFSET_B0   213
#define C2OFFS_RW_MASTER_SLAVE1_CLK_OFFSET_B1   214
#define C2OFFS_RW_SYS_RESET                     215
#define C2OFFS_RW_MMI1_CTL_B0                   217
#define C2OFFS_RW_MMI1_CTL_B1                   218
#define C2OFFS_RW_MMI2_CTL_B0                   219
#define C2OFFS_WO_PMU_ENABLE                    220
#define C2OFFS_RO_PMU_STATUS                    220
#define C2OFFS_WC_SFR_WAKEUP_EN                 221
#define C2OFFS_SFR_MMI1_PRE                     224
#define C2OFFS_SFR_MMI2_PRE                     225
#define C2OFFS_RW_WD_BOOT                       226
#define C2OFFS_RW_MCU_REG1                      227
#define C2OFFS_RW_MCU_REG2                      228
#define C2OFFS_RW_MCU_REG3                      229
#define C2OFFS_RW_CLKE_OFFSET_B0                240
#define C2OFFS_RW_CLKE_OFFSET_B1                241
#define C2OFFS_RW_CLKE_B0                       242
#define C2OFFS_RW_CLKE_B1                       243
#define C2OFFS_RW_CLKE_B2                       244
#define C2OFFS_RW_CLKE_B3                       245
#define C2OFFS_WO_BTC_LOOP_TEST                 253
#define C2OFFS_RO_VERSION_NUMBER                255

/**
 * Airoha SRF C3 Bank Register Address Definition
 */
#define C3OFFS_RW_PRBS_INIT_B0                  0
#define C3OFFS_RW_PRBS_INIT_B1                  1
#define C3OFFS_RW_PRBS_EN                       2
#define C3OFFS_RO_PRBS_B0                       3
#define C3OFFS_RO_PRBS_B1                       4

/**
 * Airoha SRF E2 Bank Register Address Definition
 */
#define E2OFFS_RW_FLASH_CTLR_CMD                0
#define E2OFFS_RW_FLASH_ADDR_2                  1
#define E2OFFS_RW_FLASH_ADDR_1                  2
#define E2OFFS_RW_FLASH_ADDR_0                  3
#define E2OFFS_WO_FLASH_CMD_TRIG                5
#define E2OFFS_RO_FLASH_STATUS                  5
#define E2OFFS_RW_FLASH_DATA_IO                 6
#define E2OFFS_RO_MANUFACTURER                  7
#define E2OFFS_RO_MEMORY_TYPE                   8
#define E2OFFS_RO_MEMORY_TYPE2                  9
#define E2OFFS_RW_PAGE_PROG_SRC_ADDR_1          10
#define E2OFFS_RW_PAGE_PROG_SRC_ADDR_0          11
#define E2OFFS_RW_PAGE_VERIFY_LEN_1             12
#define E2OFFS_RW_PAGE_VERIFY_LEN_0             13
#define E2OFFS_RW_CFG                           14
#define E2OFFS_RO_FLASH_CTLR_STATUS             15
#define E2OFFS_RO_PAGE_CRC                      16
#define E2OFFS_RW_PROTECT_BIT                   17
#define E2OFFS_RW_SECURITY_LOCK_BITS            18
#define E2OFFS_RW_MANUAL_INST                   19
#define E2OFFS_RO_MANUAL_INST_STATUS            20
#define E2OFFS_RW_PAGE_READ_LENGTH              21
#define E2OFFS_RW_RELEASE_DEEP_SLEEP_TIME       22
#define E2OFFS_RW_SPI_CONTROL                   128
#define E2OFFS_RW_SPI_CONFIG1                   129
#define E2OFFS_RW_SPI_CONFIG2                   130
#define E2OFFS_RW_SPI_CSBYTE                    131
#define E2OFFS_RW_SPI_SCLKMUL                   132
#define E2OFFS_RW_SPI_SCLKDIV                   133
#define E2OFFS_RW_SPI_TXTHHB                    134
#define E2OFFS_RW_SPI_TXTHLB                    135
#define E2OFFS_RO_SPI_TXRPTRHB                  136
#define E2OFFS_RO_SPI_TXRPTRLB                  137
#define E2OFFS_RW_SPI_TXWPTRHB                  138
#define E2OFFS_RW_SPI_TXWPTRLB                  139
#define E2OFFS_RW_SPI_RXTHHB                    140
#define E2OFFS_RW_SPI_RXTHLB                    141
#define E2OFFS_RW_SPI_RXRPTRHB                  142
#define E2OFFS_RW_SPI_RXRPTRLB                  143
#define E2OFFS_RW_SPI_RXWPTRHB                  144
#define E2OFFS_RW_SPI_RXWPTRLB                  145
#define E2OFFS_RW_SPI_RXTOMUL                   146
#define E2OFFS_RW_SPI_RXTODIV                   147
#define E2OFFS_RW_SPI_RXINTEN                   148
#define E2OFFS_RW_SPI_TXINTEN                   149
#define E2OFFS_RW_SPI_RXINTFLAG                 150
#define E2OFFS_RW_SPI_TXINTFLAG                 151
#define E2OFFS_RW_SPI_STATUS                    152
#define E2OFFS_RW_SPI_DATALEN                   153
#define E2OFFS_RW_SPI_TXFIFOSZHB                158
#define E2OFFS_RW_SPI_TXFIFOSZLB                159
#define E2OFFS_RW_SPI_TXSRCADDRHB               160
#define E2OFFS_RW_SPI_TXSRCADDRLB               161
#define E2OFFS_RW_SPI_TBCHB                     162
#define E2OFFS_RW_SPI_TBCLB                     163
#define E2OFFS_RW_SPI_RXFIFOSZHB                164
#define E2OFFS_RW_SPI_RXFIFOSZLB                165
#define E2OFFS_RW_SPI_RXSRCADDRHB               166
#define E2OFFS_RW_SPI_RXSRCADDRLB               167
#define E2OFFS_RW_SPI_RBCHB                     168
#define E2OFFS_RW_SPI_RBCLB                     169
#define E2OFFS_WO_SPI_TXDATA                    170
#define E2OFFS_RO_SPI_RXDATA                    171
#define E2OFFS_RW_SPI_RESYNC                    172
#define E2OFFS_RW_SPI_WRTIME                    173
#define E2OFFS_RW_SPI_RDTIME                    174
#define E2OFFS_RW_SPI_WRRDTIME                  175
#define E2OFFS_RW_SPI_TIME                      176
#define E2OFFS_RW_SPI_DELAYCLK                  177
#define E2OFFS_RW_SPI_DRIVER1                   178
#define E2OFFS_RW_SPI_DRIVER2                   179
#define E2OFFS_RW_SPI_PINCTRL                   180
#define E2OFFS_RW_SPI_PMCONFIG                  181
#define E2OFFS_RW_SPI_PATTERN1                  182
#define E2OFFS_RW_SPI_PATTERN2                  183
#define E2OFFS_RW_SPI_PATTERN3                  184
#define E2OFFS_RW_SPI_PATTERN4                  185
#define E2OFFS_RW_SPI_PATTERN5                  186
#define E2OFFS_RW_SPI_PATTERN6                  187
#define E2OFFS_RW_SPI_PATTERN7                  188
#define E2OFFS_RW_SPI_PATTERN8                  189
#define E2OFFS_RW_SPI_PATTERN9                  190
#define E2OFFS_RW_SPI_PATTERN10                 191
#define E2OFFS_RW_SPI_PATTERN11                 192
#define E2OFFS_RW_SPI_PATTERN12                 193
#define E2OFFS_RW_SPI_PATTERN13                 194
#define E2OFFS_RW_SPI_PATTERN14                 195
#define E2OFFS_RW_SPI_PATTERN15                 196
#define E2OFFS_RW_SPI_PATTERN16                 197
#define E2OFFS_RW_SPI_RESERVED1                 198
#define E2OFFS_RW_SPI_RESERVED2                 199
#define E2OFFS_RW_SPI_MSTATE1                   200
#define E2OFFS_RW_SPI_MSTATE2                   201
#define E2OFFS_RW_SPI_SSTATE                    202
/**
 * Airoha SRF E3 Bank Register Address Definition
 */
#define E3OFFS_RO_ADC1_CH0_B0                   0
#define E3OFFS_RO_ADC1_CH0_B1                   1
#define E3OFFS_RO_ADC1_CH1_B0                   2
#define E3OFFS_RO_ADC1_CH1_B1                   3
#define E3OFFS_RO_ADC1_CH2_B0                   4
#define E3OFFS_RO_ADC1_CH2_B1                   5
#define E3OFFS_RO_ADC1_CH3_B0                   6
#define E3OFFS_RO_ADC1_CH3_B1                   7
#define E3OFFS_RO_ADC1_CH4_B0                   8
#define E3OFFS_RO_ADC1_CH4_B1                   9
#define E3OFFS_RO_ADC1_CH5_B0                   10
#define E3OFFS_RO_ADC1_CH5_B1                   11
#define E3OFFS_RO_ADC1_CH6_B0                   12
#define E3OFFS_RO_ADC1_CH6_B1                   13
#define E3OFFS_RO_ADC1_CH7_B0                   14
#define E3OFFS_RO_ADC1_CH7_B1                   15
#define E3OFFS_RO_ADC1_CH8_B0                   16
#define E3OFFS_RO_ADC1_CH8_B1                   17
#define E3OFFS_RO_ADC1_CH9_B0                   18
#define E3OFFS_RO_ADC1_CH9_B1                   19
#define E3OFFS_RO_ADC1_CH10_B0                  20
#define E3OFFS_RO_ADC1_CH10_B1                  21
#define E3OFFS_RO_ADC1_CH11_B0                  22
#define E3OFFS_RO_ADC1_CH11_B1                  23
#define E3OFFS_RO_ADC1_CH12_B0                  24
#define E3OFFS_RO_ADC1_CH12_B1                  25
#define E3OFFS_RO_ADC1_CH13_B0                  26
#define E3OFFS_RO_ADC1_CH13_B1                  27
#define E3OFFS_RO_ADC1_CH14_B0                  28
#define E3OFFS_RO_ADC1_CH14_B1                  29
#define E3OFFS_RO_ADC1_CH15_B0                  30
#define E3OFFS_RO_ADC1_CH15_B1                  31
#define E3OFFS_RO_ADC1_CH16_B0                  32
#define E3OFFS_RO_ADC1_CH16_B1                  33
#define E3OFFS_RO_ADC1_CH17_B0                  34
#define E3OFFS_RO_ADC1_CH17_B1                  35
#define E3OFFS_RO_ADC1_CH18_B0                  36
#define E3OFFS_RO_ADC1_CH18_B1                  37
#define E3OFFS_RO_ADC1_CH19_B0                  38
#define E3OFFS_RO_ADC1_CH19_B1                  39
#define E3OFFS_RO_ADC1_CH20_B0                  40
#define E3OFFS_RO_ADC1_CH20_B1                  41
#define E3OFFS_RO_ADC1_BUSY                     42
#define E3OFFS_WC_ADC1_ENABLE                   43
#define E3OFFS_RW_ADC1_INTR_ENABLE              44
#define E3OFFS_RW_ADC1_AVERAGE_SELECTION        45
#define E3OFFS_RW_ADC1_ENABLE_DELAY_CYCLE       45
#define E3OFFS_RW_ADC1_RST_N_DELAY_CYCLE        45
#define E3OFFS_RW_ADC1_CHANNEL_ENABLE_B0        46
#define E3OFFS_RW_ADC1_CHANNEL_ENABLE_B1        47
#define E3OFFS_RW_ADC1_CHANNEL_ENABLE_B2        48
#define E3OFFS_WC_ADC1_INTR_CLEAR               49
#define E3OFFS_RO_ADC0_VAL0_B0                  64
#define E3OFFS_RO_ADC0_VAL0_B1                  65
#define E3OFFS_RO_ADC0_VAL1_B0                  66
#define E3OFFS_RO_ADC0_VAL1_B1                  67
#define E3OFFS_RO_ADC0_VAL2_B0                  68
#define E3OFFS_RO_ADC0_VAL2_B1                  69
#define E3OFFS_RO_ADC0_VAL3_B0                  70
#define E3OFFS_RO_ADC0_VAL3_B1                  71
#define E3OFFS_RO_ADC0_VAL4_B0                  72
#define E3OFFS_RO_ADC0_VAL4_B1                  73
#define E3OFFS_RO_ADC0_VAL5_B0                  74
#define E3OFFS_RO_ADC0_VAL5_B1                  75
#ifdef AB1520S
#define E3OFFS_RO_ADC0_VAL6_B0                  76
#define E3OFFS_RO_ADC0_VAL6_B1                  77
#define E3OFFS_RO_ADC0_VAL7_B0                  78
#define E3OFFS_RO_ADC0_VAL7_B1                  79
#endif

#define E3OFFS_RW_GRP_SEL_0                     128
#define E3OFFS_RW_GRP_SEL_1                     129
#define E3OFFS_RW_GRP_SEL_2                     130
#define E3OFFS_RW_SIG_SEL_0304                  131
#define E3OFFS_RW_SIG_SEL_0506                  132
#define E3OFFS_RW_SIG_SEL_0708                  133
#define E3OFFS_RW_SIG_SEL_0910                  134
#define E3OFFS_RW_SIG_SEL_1112                  135
#define E3OFFS_RW_SIG_SEL_1314                  136
#define E3OFFS_RW_SIG_SEL_1516                  137
#define E3OFFS_RW_SIG_SEL_1718                  138
#define E3OFFS_RW_SIG_SEL_1920                  139
#define E3OFFS_RW_SIG_SEL_2122                  140
#define E3OFFS_RW_SIG_SEL_2324                  141
#define E3OFFS_RW_SIG_SEL_25                    142
#define E3OFFS_RW_DBG_SEL_0TO3                  143
#define E3OFFS_RW_DBG_SEL_4                     144
#define E3OFFS_RW_LED_SCL_EN                    145
#define E3OFFS_RW_DMIC_DI0_SEL                  146
#define E3OFFS_RW_DMIC_DI1_SEL                  147
#define E3OFFS_RW_I2S0_DI_SEL                   148
#define E3OFFS_RW_I2S1_DI_SEL                   149
#define E3OFFS_RW_UART_NCTS_SEL                 150
#define E3OFFS_RW_MCU_NVIC_SRAM_GSLEEP		    157
#define E3OFFS_RW_NVIC_SRAM_MARGIN_LPMX         158
#define E3OFFS_RW_ICE_MODE_STATUS               158
#define E3OFFS_RW_SFR_MCU_TOUT_CLK_SEL_EN       159
#define E3OFFS_RO_I2C_SDA_STATUS                160

/**
 * Airoha SRF E4 Bank Register Address Definition
 */
#define E4OFFS_RW_LED_EN                            0
#define E4OFFS_RW_LED1_FOLLOW_MULTIPLIER_CONTROL    1
#define E4OFFS_RW_LED1_T1T2REPEAT                   2
#define E4OFFS_RW_LED1_TIME_UNIT                    3
#define E4OFFS_RW_LED1_T0                           4
#define E4OFFS_RW_LED1_T1                           5
#define E4OFFS_RW_LED1_T2                           6
#define E4OFFS_RW_LED1_T3                           7
#define E4OFFS_RW_LED1_PWM_ONSTEP                   8
#define E4OFFS_RW_LED1_PWM_OFFSTEP                  9
#define E4OFFS_RW_LED1_PWM_DURATION_SET             10
#define E4OFFS_RW_LED2_FOLLOW_MULTIPLIER_CONTROL    21
#define E4OFFS_RW_LED2_T1T2REPEAT                   22
#define E4OFFS_RW_LED2_TIME_UNIT                    23
#define E4OFFS_RW_LED2_T0                           24
#define E4OFFS_RW_LED2_T1                           25
#define E4OFFS_RW_LED2_T2                           26
#define E4OFFS_RW_LED2_T3                           27
#define E4OFFS_RW_LED2_PWM_ONSTEP                   28
#define E4OFFS_RW_LED2_PWM_OFFSTEP                  29
#define E4OFFS_RW_LED2_PWM_DURATION_SET             30
#define E4OFFS_RW_LED3_FOLLOW_MULTIPLIER_CONTROL    41
#define E4OFFS_RW_LED3_T1T2REPEAT                   42
#define E4OFFS_RW_LED3_TIME_UNIT                    43
#define E4OFFS_RW_LED3_T0                           44
#define E4OFFS_RW_LED3_T1                           45
#define E4OFFS_RW_LED3_T2                           46
#define E4OFFS_RW_LED3_T3                           47
#define E4OFFS_RW_LED3_PWM_ONSTEP                   48
#define E4OFFS_RW_LED3_PWM_OFFSTEP                  49
#define E4OFFS_RW_LED3_PWM_DURATION_SET             50
#define E4OFFS_RW_LED4_FOLLOW_MULTIPLIER_CONTROL    61
#define E4OFFS_RW_LED4_T1T2REPEAT                   62
#define E4OFFS_RW_LED4_TIME_UNIT                    63
#define E4OFFS_RW_LED4_T0                           64
#define E4OFFS_RW_LED4_T1                           65
#define E4OFFS_RW_LED4_T2                           66
#define E4OFFS_RW_LED4_T3                           67
#define E4OFFS_RW_LED4_PWM_ONSTEP                   68
#define E4OFFS_RW_LED4_PWM_OFFSTEP                  69
#define E4OFFS_RW_LED4_PWM_DURATION_SET             70
#define E4OFFS_RW_LED5_MULTIPLIER_CONTROL           81
#define E4OFFS_RW_LED5_T1T2REPEAT                   82
#define E4OFFS_RW_LED5_TIME_UNIT                    83
#define E4OFFS_RW_LED5_T0                           84
#define E4OFFS_RW_LED5_T1                           85
#define E4OFFS_RW_LED5_T2                           86
#define E4OFFS_RW_LED5_T3                           87
#define E4OFFS_RW_LED5_PWM_ONSTEP                   88
#define E4OFFS_RW_LED5_PWM_OFFSTEP                  89
#define E4OFFS_RW_LED5_PWM_DURATION_SET             90
#define E4OFFS_RW_3_0_RX_MODEM_128                  128
#define E4OFFS_RW_3_0_RX_MODEM_129                  129
#define E4OFFS_RW_3_0_RX_MODEM_130                  130
#define E4OFFS_RW_3_0_RX_MODEM_131                  131
#define E4OFFS_RW_3_0_RX_MODEM_132                  132
#define E4OFFS_RW_3_0_RX_MODEM_133                  133
#define E4OFFS_RW_3_0_RX_MODEM_134                  134
#define E4OFFS_RW_3_0_RX_MODEM_135                  135
#define E4OFFS_RW_3_0_RX_MODEM_136                  136
#define E4OFFS_RW_3_0_RX_MODEM_137                  137
#define E4OFFS_RW_3_0_RX_MODEM_138                  138
#define E4OFFS_RW_3_0_RX_MODEM_139                  139
#define E4OFFS_RW_3_0_RX_MODEM_140                  140
#define E4OFFS_RW_3_0_RX_MODEM_141                  141
#define E4OFFS_RW_3_0_RX_MODEM_142                  142
#define E4OFFS_RW_3_0_RX_MODEM_143                  143
#define E4OFFS_RW_3_0_RX_MODEM_144                  144
#define E4OFFS_RW_3_0_RX_MODEM_145                  145
#define E4OFFS_RW_3_0_RX_MODEM_146                  146
#define E4OFFS_RW_3_0_RX_MODEM_147                  147
#define E4OFFS_RW_BM_MATRIX_I_00                    150
#define E4OFFS_RW_BM_MATRIX_I_01                    151
#define E4OFFS_RW_BM_MATRIX_I_02                    152
#define E4OFFS_RW_BM_MATRIX_I_03                    153
#define E4OFFS_RW_BM_MATRIX_I_04                    154
#define E4OFFS_RW_BM_MATRIX_I_05                    155
#define E4OFFS_RW_BM_MATRIX_I_10                    156
#define E4OFFS_RW_BM_MATRIX_I_11                    157
#define E4OFFS_RW_BM_MATRIX_I_12                    158
#define E4OFFS_RW_BM_MATRIX_I_13                    159
#define E4OFFS_RW_BM_MATRIX_I_14                    160
#define E4OFFS_RW_BM_MATRIX_I_15                    161
#define E4OFFS_RW_BM_MATRIX_I_20                    162
#define E4OFFS_RW_BM_MATRIX_I_21                    163
#define E4OFFS_RW_BM_MATRIX_I_22                    164
#define E4OFFS_RW_BM_MATRIX_I_23                    165
#define E4OFFS_RW_BM_MATRIX_I_24                    166
#define E4OFFS_RW_BM_MATRIX_I_25                    167
#define E4OFFS_RW_BLE_AGC_DBM_OFFSET                170
#define E4OFFS_RW_BLE_AGC_UHG1_THRESHOLD            171
#define E4OFFS_RW_BLE_AGC_UHG2_THRESHOLD            172
#define E4OFFS_RW_BLE_AGC_UHG3_THRESHOLD            173
#define E4OFFS_RW_BLE_AGC_HG1_THRESHOLD             174
#define E4OFFS_RW_BLE_AGC_HG2_THRESHOLD             175
#define E4OFFS_RW_BLE_AGC_HG3_THRESHOLD             176
#define E4OFFS_RW_BLE_AGC_HG4_THRESHOLD             177
#define E4OFFS_RW_BLE_AGC_LG1_THRESHOLD             178
#define E4OFFS_RW_BLE_AGC_LG2_THRESHOLD             179
#define E4OFFS_RW_BLE_AGC_LG3_THRESHOLD             180
#define E4OFFS_RW_BLE_AGC_LG4_THRESHOLD             181
#define E4OFFS_RW_BLE_AGC_UHG                       182
#define E4OFFS_RW_BLE_AGC_HG                        183
#define E4OFFS_RW_BLE_AGC_MG                        184
#define E4OFFS_RW_BLE_AGC_LG                        185
#define E4OFFS_RW_BLE_AGC_ULG                       186
#define E4OFFS_RW_BLE_AGC_GC_SETTLE_TIME_AND_TOL    187
#define E4OFFS_RW_BLE_AGC_FIX_GAIN_CTL              188
#define E4OFFS_RW_BLE_AGC_STATE_TPERIOD             189
#define E4OFFS_RW_BLE_AAGC_LPF_CTL                  190
#define E4OFFS_RW_AGC_PARAM1                        192
#define E4OFFS_RW_AGC_PARAM2                        193
#define E4OFFS_RW_AGC_PARAM3                        194
#define E4OFFS_RW_AGC_RETRIGGER_LIMIT               195
#define E4OFFS_RW_AGC_GC                            196
#define E4OFFS_RW_AGC_DB_OFFSET                     197
#define E4OFFS_RO_RSSI                              206
#define E4OFFS_AGC_GAIN_SEL                         207
#define E4OFFS_RW_BLE_RX_MODEM_208                  208
#define E4OFFS_RW_BLE_RX_MODEM_209                  209
#define E4OFFS_RW_BLE_RX_MODEM_210                  210
#define E4OFFS_RW_BLE_RX_MODEM_211                  211
#define E4OFFS_RW_BLE_RX_MODEM_212                  212
#define E4OFFS_RW_BLE_RX_MODEM_213                  213
#define E4OFFS_RW_BLE_RX_MODEM_214                  214
#define E4OFFS_RW_BLE_RX_MODEM_215                  215
#define E4OFFS_RW_BLE_RX_MODEM_216                  216
#define E4OFFS_RW_BLE_RX_MODEM_217                  217
#define E4OFFS_RW_BLE_RX_MODEM_218                  218
#define E4OFFS_RW_TX_MODEM_224                      224
#define E4OFFS_RW_TX_MODEM_225                      225
#define E4OFFS_RW_TX_MODEM_226                      226
#define E4OFFS_RW_TX_MODEM_227                      227
#define E4OFFS_RW_TX_MODEM_228                      228
#define E4OFFS_RW_TX_MODEM_229                      229
#define E4OFFS_RW_TX_MODEM_230                      230
#define E4OFFS_RW_TX_MODEM_231                      231

#define E4OFFS_RW_GAIN_TRXVal_244                   244


/**
 * Airoha SRF E6 Bank Register Address Definition
 */
#define E6OFFS_WO_HW_RESET_CTRL                 0
#define E6OFFS_RW_PA_ON_TIME                    1
#define E6OFFS_RW_TX_ON_TIME                    2
#define E6OFFS_RW_PLL_ON_TIME                   3
#define E6OFFS_RW_RX_ON_TIME                    4
#define E6OFFS_RW_ACCESS_ADDR_OFFSET            5
#define E6OFFS_RW_SYNTHESIZER_LOCKING_TIME      6
#define E6OFFS_RW_INTER_FRAME_SPACE             7
#define E6OFFS_RW_TX_TEST_CTRL                  8
#define E6OFFS_RW_TX_TEST_PAYLOAD_HEADER_B0     9
#define E6OFFS_RW_RX_MAX_LEN_WITH_CRC           10
#define E6OFFS_RW_BURST_TRX_ENABLE_CHECK_POINT  11
#define E6OFFS_RW_TX_TRIGGER_OFFSET             12
#define E6OFFS_RW_RX_TRIGGER_OFFSET             13
#define E6OFFS_RW_RX_CTRL                       14
#define E6OFFS_RW_RX_TIMEOUT                    15
#define E6OFFS_RW_TX_NULL_OR_RETRY_CTL          16
#define E6OFFS_RW_ADV_LATCH_CLKN_CTL            17
#define E6OFFS_RW_MODEM_TX_ON_DELAY             18
#define E6OFFS_RW_MODEM_TX_OFF_DELAY            19
#define E6OFFS_RW_DEVICE0_INT_MASK_B0           20
#define E6OFFS_RW_DEVICE0_INT_MASK_B1           21
#define E6OFFS_RW_DEVICE0_INT_STATUS_B0         22
#define E6OFFS_RW_DEVICE0_INT_STATUS_B1         23
#define E6OFFS_RW_DEVICE1_INT_MASK_B0           24
#define E6OFFS_RW_DEVICE1_INT_MASK_B1           25
#define E6OFFS_RW_DEVICE1_INT_STATUS_B0         26
#define E6OFFS_RW_DEVICE1_INT_STATUS_B1         27
#define E6OFFS_RW_MAX_RX_DATA_LEN               28
#define E6OFFS_RW_MAX_RX_ADV_LEN                29
#define E6OFFS_RW_DEVICE_IDX                    30
#define E6OFFS_RW_TRX_CTRL_B0                   31
#define E6OFFS_RW_TRX_CTRL_B1                   32
#define E6OFFS_RW_TRX_CTRL_B2                   33
#define E6OFFS_RW_DEVICE_CLK_EVENT_CTRL         34
#define E6OFFS_RW_DATA_PKT_SN_NESN              35
#define E6OFFS_RW_DATA_PKT_MD                   36
#define E6OFFS_RW_DATA_PKT_RFU                  37
#define E6OFFS_RW_1ST_CRX_TIMEPUT_B0            38
#define E6OFFS_RW_1ST_CRX_TIMEPUT_B1            39
#define E6OFFS_RW_DEVICE_LINK_PTR_B0            40
#define E6OFFS_RW_DEVICE_LINK_PTR_B1            41
#define E6OFFS_RO_DEVICE_ANCHOR_STATUS          42
#define E6OFFS_RW_SW_HOP_CHANNEL                43
#define E6OFFS_RW_SW_TX_LEN                     44
#define E6OFFS_RW_SW_TRX_LEN_CTRL               45
#define E6OFFS_RW_SLOT_TICK_EVENT_CTRL          46
#define E6OFFS_RW_DEVICE_CLK_B0                 47
#define E6OFFS_RW_DEVICE_CLK_B1                 48
#define E6OFFS_RW_DEVICE_CLK_B2                 49
#define E6OFFS_RW_DEVICE_CLK_ADJUSTMENT         51
#define E6OFFS_RW_DEVICE_CLK_OFFSET_B0          52
#define E6OFFS_RW_DEVICE_CLK_OFFSET_B1          53
#define E6OFFS_RW_OFFSET_OF_CONNECTION_END_B0   55
#define E6OFFS_RW_OFFSET_OF_CONNECTION_END_B1   56
#define E6OFFS_RW_CONN_MATCH_EVT_CNT_B0         57
#define E6OFFS_RW_CONN_MATCH_EVT_CNT_B1         58
#define E6OFFS_RW_SEARCH_CTRL                   60
#define E6OFFS_RW_NEXT_ANCHOR_INSTANT_B0        61
#define E6OFFS_RW_NEXT_ANCHOR_INSTANT_B1        62
#define E6OFFS_RW_NEXT_ANCHOR_INSTANT_B2        63
#define E6OFFS_RW_ANCHOR_INTERVAL_B0            64
#define E6OFFS_RW_ANCHOR_INTERVAL_B1            65
#define E6OFFS_RW_TR_ADVANCED_IN_4_US           66
#define E6OFFS_RW_TR_ADVANCED_IN_625_US         67
#define E6OFFS_RW_MAX_SEARCH_WINDOW             68
#define E6OFFS_RW_SW_ANCHOR_INT_GUARD_IN_8_US   69
#define E6OFFS_RW_SW_ANCHOR_INT_GUARD_IN_625_US 70
#define E6OFFS_RW_ADVENCE_RESERVED_SLOT         71
#define E6OFFS_RW_ADV_DELAY                     72
#define E6OFFS_RW_ADV_MIN_SPACE                 73
#define E6OFFS_RO_DEVICE_CLK_AT_ADV_TRXDONE_B0  74
#define E6OFFS_RO_DEVICE_CLK_AT_ADV_TRXDONE_B1  75
#define E6OFFS_RO_DEVICE_CLK_AT_ADV_TRXDONE_B2  76
#define E6OFFS_RW_DEVICE_ANCHOR_OFFSET_STATUS   82
#define E6OFFS_RW_RECEIVED_DATA_CHANNEL_STATUS  83
#define E6OFFS_RW_CURRENT_LATENCY_CTRL_B0       86
#define E6OFFS_RW_CURRENT_LATENCY_CTRL_B1       87
#define E6OFFS_RW_LATENCY_PERIOD_B0             88
#define E6OFFS_RW_LATENCY_PERIOD_B1             89
#define E6OFFS_RW_WEIGHT_RESET                  95
#define E6OFFS_RW_INITIAL_WEIGHT                96
#define E6OFFS_RW_DELTA_WEIGHT                  97
#define E6OFFS_RW_SW_AES_KEY_PTR_B0             110
#define E6OFFS_RW_SW_AES_KEY_PTR_B1             111
#define E6OFFS_RW_SW_AES_PDU_PTR_B0             112
#define E6OFFS_RW_SW_AES_PDU_PTR_B1             113
#define E6OFFS_RW_SW_AES_CTRL                   114
#define E6OFFS_RW_AES_CCM_STATUS                115

/**
 * Airoha SRF E7 Bank Register Address Definition
 */
#define E7OFFS_RW_SCATTERNET_CONTROL_BASE_ADDRESS_B0        0
#define E7OFFS_RW_SCATTERNET_CONTROL_BASE_ADDRESS_B1        1
#define E7OFFS_RW_ARQ_SEQ_SEL                               2
#define E7OFFS_RW_ARQ_SEQ_BITMAP                            3
#define E7OFFS_WO_LINK_RESET                                4
#define E7OFFS_WO_DV_TRANSFORM_ENABLE                       5
#define E7OFFS_RW_SLAVE_RX_NULL_NO_TX                       6
#define E7OFFS_RW_AUDIO_SOURCE_CTRL                         7
#define E7OFFS_RW_MASTER_AUDIO_MAP                          8
#define E7OFFS_RW_SLAVE_AUDIO_MAP                           9
#define E7OFFS_RW_MASTER_SERVICE_POLL_CTRL                  10
#define E7OFFS_RW_MASTER_SERVICE_NULL_RESP_CTRL             11
#define E7OFFS_RW_TX_ARQ_FLOW_CTRL                          12
#define E7OFFS_WO_SLAVE_EXTRA_HIGH_PRIOR_INTERVAL_AND_SIZE  13
#define E7OFFS_RW_MASTER_AFH_CTL                            15
#define E7OFFS_RW_SLAVE_AFH_CTL                             16
#define E7OFFS_RO_MASTER_AFH_NEXT_BIT_MAP_SEL               17
#define E7OFFS_RO_SLAVE_AFH_NEXT_BIT_MAP_SEL                18
#define E7OFFS_RW_SLAVE_NORMAL_SEARCH_CTL                   19
#define E7OFFS_RW_SCO_ESCO_GUARD_ENABLE                     20
#define E7OFFS_RW_SNIFF_HIGH_PRIOR_ADVANCE_RESERVED         21
#define E7OFFS_RW_SERVICE_HIGH_PRIORITY_CTRL                22
#define E7OFFS_RW_BROADCAST_TX_EN                           23
#define E7OFFS_RW_BROADCAST_NUM                             24
#define E7OFFS_RW_MASTER_POLL_CTRL                          25
#define E7OFFS_RW_SLAVE_SERVICE_CTRL                        26
#define E7OFFS_RW_MASTER_ROLE_SWITCH_CTRL                   27
#define E7OFFS_RW_SLAVE_ROLE_SWITCH_CTRL                    28
#define E7OFFS_RW_MASTER_SNIFF_CTRL                         29
#define E7OFFS_RW_SLAVE_SNIFF_CTRL                          30
#define E7OFFS_RW_MASTER_PRIORITY_CTRL                      33
#define E7OFFS_RW_SLAVE_PRIORITY_CTRL                       34
#define E7OFFS_RW_MASTER_SCO_LINK_CTRL                      35
#define E7OFFS_RW_SLAVE_SCO_LINK_CTRL                       36
#define E7OFFS_RW_MASTER_ESCO_LINK_CTRL                     37
#define E7OFFS_RW_SLAVE_ESCO_LINK_CTRL                      38
#define E7OFFS_RW_MASTER_ACL_TX_EN                          39
#define E7OFFS_RW_SLAVE_ACL_TX_EN                           40
#define E7OFFS_RW_MASTER_LINK_EN                            41
#define E7OFFS_RW_SLAVE_LINK_EN                             42
#define E7OFFS_RW_CURRENT_LINK_CTRL                         43
#define E7OFFS_RW_SAC_INTERRUPT_LINK_SEL                    44
#define E7OFFS_RW_SAC_INTERRUPT_ENABLE1                     45
#define E7OFFS_RW_SAC_INTERRUPT_ENABLE2                     46
#define E7OFFS_RW_SAC_INTERRUPT_ENABLE3                     47
#define E7OFFS_RW_SAC_INTERRUPT_ENABLE4                     48
#define E7OFFS_RW_MASTER_SLAVE_PARAMETER                    50
#define E7OFFS_WO_ACL_TX_TYPE                               51
#define E7OFFS_RW_SNIFF_NEXT_INSTANT_B0                     52
#define E7OFFS_RW_SNIFF_NEXT_INSTANT_B1                     53
#define E7OFFS_RW_SNIFF_NEXT_INSTANT_B2                     54
#define E7OFFS_RW_SNIFF_NEXT_INSTANT_B3                     55
#define E7OFFS_WO_SNIFF_INTERVAL_B0                         56
#define E7OFFS_WO_SNIFF_INTERVAL_B1                         57
#define E7OFFS_WO_SNIFF_ATTEMPT_B0                          58
#define E7OFFS_WO_SNIFF_ATTEMPT_B1                          59
#define E7OFFS_WO_SNIFF_TIMEOUT_B0                          60
#define E7OFFS_WO_SNIFF_TIMEOUT_B1                          61
#define E7OFFS_WO_SCO_NEXT_INSTANT_B0                       62
#define E7OFFS_WO_SCO_NEXT_INSTANT_B1                       63
#define E7OFFS_WO_SCO_ESCO_RESERVE_SLOT                     64
#define E7OFFS_WO_ESCO_NEXT_INSTANT_B0                      65
#define E7OFFS_WO_ESCO_NEXT_INSTANT_B1                      66
#define E7OFFS_WO_ESCO_INTERVAL                             67
#define E7OFFS_WO_ESCO_RETRANSMIT_TIMEOUT                   68
#define E7OFFS_WO_POLL_CONNECTION_TIMEOUT                   69
#define E7OFFS_WO_POLL_INDIVIDAL_TIMEOUT                    70
#define E7OFFS_WO_SERVICE_INTERVAL_B0                       71
#define E7OFFS_WO_SERVICE_INTERVAL_B1                       72
#define E7OFFS_WO_SERVICE_OVERALL_TIMEOUT_B0                73
#define E7OFFS_WO_SERVICE_OVERALL_TIMEOUT_B1                74
#define E7OFFS_WO_HOLD_NEXT_INSTANT_B0                      75
#define E7OFFS_WO_HOLD_NEXT_INSTANT_B1                      76
#define E7OFFS_WO_HOLD_NEXT_INSTANT_B2                      77
#define E7OFFS_WO_HOLD_TIMEOUT_B0                           78
#define E7OFFS_WO_HOLD_TIMEOUT_B1                           79
#define E7OFFS_WO_AFH_NEXT_INSTANT_B0                       80
#define E7OFFS_WO_AFH_NEXT_INSTANT_B1                       81
#define E7OFFS_WO_AFH_NEXT_INSTANT_B2                       82

#ifdef AB1520S
#define E7OFFS_WO_AFH_NEXT_INSTANT_B3                       83
#else
#define E7OFFS_WO_ROLE_SWITCH_INSTANT_B0                    83
#define E7OFFS_WO_ROLE_SWITCH_INSTANT_B1                    84
#define E7OFFS_WO_ROLE_SWITCH_TIMEOUT                       85
#endif

#define E7OFFS_WO_SCAN_INTERVAL_B0                          86
#define E7OFFS_WO_SCAN_INTERVAL_B1                          87
#define E7OFFS_WO_MASTER_SNIFF_POLL_INTERVAL                88
#define E7OFFS_WO_SNIFF_LOSS_COUNT                          89
#define E7OFFS_WO_SERVICE_LOSS_COUNT                        90
#define E7OFFS_WO_SERVICE_MAX_IDLE_SLOT                     91
#define E7OFFS_WO_MIN_SLEEP_TIME                            93
#define E7OFFS_RW_SNIFF_SUBRATE_NUM                         94
#define E7OFFS_WO_SNIFF_SUBRATE_TIMEOUT_B0                  95
#define E7OFFS_WO_SNIFF_SUBRATE_TIMEOUT_B1                  96
#define E7OFFS_RW_OSC_PLL_WAKEUP_GURAD                      97
#define E7OFFS_WO_SNIFF_SLAVE_INIT_SEARCH_OFFSET1           98
#define E7OFFS_WO_SNIFF_SLAVE_INIT_SEARCH_OFFSET2           99
#define E7OFFS_RO_RX_POLL_RD_MASTER                         100
#define E7OFFS_RO_RX_POLL_RD_SLAVE                          101
#define E7OFFS_RO_CLKX_OFFSET_B0                            102
#define E7OFFS_RO_CLKX_OFFSET_B1                            103
#define E7OFFS_RO_RT1TT1X_OFFSET_B0                         104
#define E7OFFS_RO_RT1TT1X_OFFSET_B1                         105
#define E7OFFS_RO_ACTIVE_VECTOR                             106
#define E7OFFS_RO_ESCO_SCO_DOWN_COUNT0                      107
#define E7OFFS_RO_ESCO_SCO_DOWN_COUNT1                      108
#define E7OFFS_WO_MASTER_SNIFF_SUBRATE_DISABLE              110
#define E7OFFS_WO_SLAVE_SNIFF_SUBRATE_DISABLE               111
#define E7OFFS_RW_SLOW_CLK_CAL                              112
#define E7OFFS_RW_SLOW_CLK_CTL                              113
#define E7OFFS_RW_SLOW_CLK_INT                              114
#define E7OFFS_RW_SLOW_CLK_DEC1                             115
#define E7OFFS_RW_SLOW_CLK_DEC2                             116
#define E7OFFS_WC_SERVICE_WEIGHT_RESET                      120
#define E7OFFS_WR_SERVICE_INITIAL_WEIGHT                    121
#define E7OFFS_WR_SERVICE_DELTA_WEIGHT                      122
#define E7OFFS_WC_SNIFF_WEIGHT_RESET                        123
#define E7OFFS_WR_SNIFF_INITIAL_WEIGHT                      124
#define E7OFFS_WR_SNIFF_DELTA_WEIGHT                            125
#define E7OFFS_WC_ACTIVE_SCO_ESCO_RESERVED_SLOT_WEIGHT_RESET    126
#define E7OFFS_WR_ACTIVE_SCO_ESCO_RESERVED_SLOT_INITIAL_WEIGHT  127
#define E7OFFS_WR_ACTIVE_SCO_ESCO_RESERVED_SLOT_DELTA_WEIGHT    128
#define E7OFFS_WC_ACTIVE_ESCO_RETRANSMIT_WEIGHT_RESET           129
#define E7OFFS_WR_ACTIVE_ESCO_RETRANSMIT_SLOT_INITIAL_WEIGHT    130
#define E7OFFS_WR_ACTIVE_ESCO_RETRANSMIT_SLOT_DELTA_WEIGHT      131
#define E7OFFS_WC_MUTE_SCO_ESCO_WEIGHT_RESET                    132
#define E7OFFS_WR_MUTE_SCO_ESCO_INITIAL_WEIGHT                  133
#define E7OFFS_WR_MUTE_SCO_ESCO_DELTA_WEIGHT                    134
#define E7OFFS_WR_TWS_CLK_SOURCE_SEL                        140
#define E7OFFS_WR_TWS_SC_CNT_FREQ_DIVIDER                   141
#define E7OFFS_WR_TWS_SC_NEXT_INSTANT_B0                    142
#define E7OFFS_WR_TWS_SC_NEXT_INSTANT_B1                    143
#define E7OFFS_WR_TWS_SC_NEXT_INSTANT_B2                    144
#define E7OFFS_WR_TWS_SC_NEXT_INSTANT_B3                    145
#define E7OFFS_RO_TWS_CLK_AT_TICK_B0                        146
#define E7OFFS_RO_TWS_CLK_AT_TICK_B1                        147
#define E7OFFS_RO_TWS_CLK_AT_TICK_B2                        148
#define E7OFFS_RO_TWS_CLK_AT_TICK_B3                        149
#define E7OFFS_RO_TWS_SRC_SMP_CNT_B0                        150
#define E7OFFS_RO_TWS_SRC_SMP_CNT_B1                        151
#define E7OFFS_RO_TWS_SRC_SMP_CNT_B2                        152
#define E7OFFS_RO_TWS_SRC_SMP_CNT_B3                        153
#define E7OFFS_RW_OUT_SEL                                   205
#define E7OFFS_RW_START_ADDR_PT_L                           206
#define E7OFFS_RW_START_ADDR_PT_H                           207
#define E7OFFS_RW_START_ADDR_KEY_L                          208
#define E7OFFS_RW_START_ADDR_KEY_H                          209
#define E7OFFS_RW_START_ADDR_TEMP_L                         210
#define E7OFFS_RW_START_ADDR_TEMP_H                         211
#define E7OFFS_RW_START_ADDR_OUT_L                          212
#define E7OFFS_RW_START_ADDR_OUT_H                          213
#define E7OFFS_RW_SHA_X_B3                                  214
#define E7OFFS_RW_SHA_X_B2                                  215
#define E7OFFS_RW_SHA_X_B1                                  216
#define E7OFFS_RW_SHA_X_B0                                  217
#define E7OFFS_WO_SHA_Y_B3                                  218
#define E7OFFS_WO_SHA_Y_B2                                  219
#define E7OFFS_WO_SHA_Y_B1                                  220
#define E7OFFS_WO_SHA_Y_B0                                  221
#define E7OFFS_WO_SHA_OP_SEL                                222
#define E7OFFS_WO_RX_LARGE_BUF_AUTO_RELEASE                 230
#define E7OFFS_RW_BT4_TX_CTRL                               231
#define E7OFFS_RO_CLKN_PHASE_B0                             234
#define E7OFFS_RO_CLKN_PHASE_B1                             235
#ifdef AB1520D
#define E7OFFS_WO_SLAVE_WESCO_TX_CTRL                       235
#endif
#define E7OFFS_WO_MEM_DEBUG_CTL                             250
#define E7OFFS_WO_MEM_DEBUG_ADDR_B0                         251
#define E7OFFS_WO_MEM_DEBUG_ADDR_B1                         252

/**
 * Airoha SRF FB Bank Register Address Definition
 */
#define FBOFFS_WR_UART_DLM                      0
#define FBOFFS_WR_UART_DLL                      1
#define FBOFFS_WR_UART_DLF                      2
#define FBOFFS_WR_UART_LCR                      3
#define FBOFFS_WR_UART_IER                      4
#define FBOFFS_WR_UART_MCR                      5
#define FBOFFS_WO_UART_FCR                      6
#define FBOFFS_WO_UART_DMA_RXADDR_H             7
#define FBOFFS_WO_UART_DMA_RXADDR_L             8
#define FBOFFS_WO_UART_DMA_RXLENGTH_H           9
#define FBOFFS_WO_UART_DMA_RXLENGTH_L           10
#define FBOFFS_WO_UART_DMA_TXADDR_H             11
#define FBOFFS_WO_UART_DMA_TXADDR_L             12
#define FBOFFS_WO_UART_DMA_TXLENGTH_H           13
#define FBOFFS_WO_UART_DMA_TXLENGTH_L           14
#define FBOFFS_RO_UART_IIR                      15
#define FBOFFS_RO_UART_LSR                      16
#define FBOFFS_RO_UART_MSR                      17
#define FBOFFS_RO_UART_DMA_RX_CURRENT_WADDR_H   18
#define FBOFFS_RO_UART_DMA_RX_CURRENT_WADDR_L   19
#define FBOFFS_WO_UART_DMA_RX_CURRENT_RADDR_H   20
#define FBOFFS_WO_UART_DMA_RX_CURRENT_RADDR_L   21
#define FBOFFS_WO_UART_DMA_TRIGGER_LEVEL_H      22
#define FBOFFS_WO_UART_DMA_TRIGGER_LEVEL_L      23
#define FBOFFS_WO_UART_DMA_CHAR_TIMEOUT         24
#define FBOFFS_WO_UART_ENABLE                   25
#define FBOFFS_WO_UART_CLK_CTRL                 26
#define FBOFFS_RO_AUTO_BR_STATUS                27
#define FBOFFS_RO_AUTO_BR_START_SW              27
#define FBOFFS_RW_GPIO_O_B0                     64
#define FBOFFS_RW_GPIO_O_B1                     65
#define FBOFFS_RW_GPIO_O_B2                     66
#define FBOFFS_RW_GPIO_O_B3                     67
#define FBOFFS_RW_GPIO_OE_B0                    68
#define FBOFFS_RW_GPIO_OE_B1                    69
#define FBOFFS_RW_GPIO_OE_B2                    70
#define FBOFFS_RW_GPIO_OE_B3                    71
#define FBOFFS_WO_DEBOUNCE_LATCHRD_B0           81
#define FBOFFS_WO_DEBOUNCE_LATCHRD_B1           82
#define FBOFFS_WO_DEBOUNCE_LATCHRD_B2           83
#define FBOFFS_WO_DEBOUNCE_LATCHRD_B3           84
#define FBOFFS_RO_DEBOUNCE_LATCH_B0             85
#define FBOFFS_RO_DEBOUNCE_LATCH_B1             86
#define FBOFFS_RO_DEBOUNCE_LATCH_B2             87
#define FBOFFS_RO_DEBOUNCE_LATCH_B3             88
#define FBOFFS_RO_DEBOUNCE_DO_B0                89
#define FBOFFS_RO_DEBOUNCE_DO_B1                90
#define FBOFFS_RO_DEBOUNCE_DO_B2                91
#define FBOFFS_RO_DEBOUNCE_DO_B3                92
#define FBOFFS_RO_WAKEUP_RD                     95
#define FBOFFS_RO_WAKEUP_DEBOUNCE_RD            96
#define FBOFFS_WO_WAKEUP_DEBOUNCE_DO_LATCH      97
#define FBOFFS_RO_WAKEUP_DEBOUNCE_DO            98
#define FBOFFS_RO_CHG_PLUG_FLAG                 99
#define FBOFFS_RO_CHARGER_STATUS                100
#define FBOFFS_WO_MEMCPY_SRC_ADDR_B0            136
#define FBOFFS_WO_MEMCPY_SRC_ADDR_B1            137
#define FBOFFS_WO_MEMCPY_TARGET_ADDR_B0         138
#define FBOFFS_WO_MEMCPY_TARGET_ADDR_B1         139
#define FBOFFS_WO_MEMCPY_DATA_SIZE_B0           140
#define FBOFFS_WO_MEMCPY_DATA_SIZE_B1           141
#define FBOFFS_WO_MCU_CACHE_FLUSH               160
#define FBOFFS_WO_MCU_ROM_CG_EN                 190
#define FBOFFS_WO_MCU_IRAM_CG_EN                190
#define FBOFFS_WO_MCU_XDATA_CG_EN               190
#define FBOFFS_WO_MCU_CACHE_CG_EN               190
#define FBOFFS_WO_MCU_CACHE_OFFSET              191
#define FBOFFS_WO_MCU_FLASH_OFFSET              192
#define FBOFFS_WO_DSP_BOOT_CLK                  200
#define FBOFFS_WO_DSP_DMEM_SIZE                 201
#define FBOFFS_WO_SVTADDR0                      202
#define FBOFFS_WO_SVTADDR1                      203
#define FBOFFS_WO_SVTADDR2                      204
#define FBOFFS_WO_DSP_INT_CTL                   205
#define FBOFFS_WO_DSP_RST                       206
#define FBOFFS_WO_DSP_CKSEL                     207
#define FBOFFS_WO_TCM_BASE_ADDR                 211
#define FBOFFS_WO_TCM_SIZE                      212
#define FBOFFS_WO_JUMP_INDEX                    224
#define FBOFFS_WO_JUMP_ADDR_B0                  225
#define FBOFFS_WO_JUMP_ADDR_B1                  226
#define FBOFFS_WO_JUMP_ADDR_B2                  227
#define FBOFFS_WO_JUMP_CODE_B0                  228
#define FBOFFS_WO_JUMP_CODE_B1                  229
#define FBOFFS_WO_JUMP_CODE_B2                  230
#define FBOFFS_WO_JUMP_CODE_B3                  231
#define FBOFFS_RW_MCU_READY                     255

/**
 * Airoha SRF FE Bank Register Address Definition
 */
#define FEOFFS_RW_DMA_BTCTX_LENGTH_L            0
#define FEOFFS_RW_DMA_BTCTX_LENGTH_H            1
#define FEOFFS_RW_DMA_BTCTX_ADDR_L              2
#define FEOFFS_RW_DMA_BTCTX_ADDR_H              3
#define FEOFFS_RW_DMA_RX_MAX_LEN_TINY           4
#define FEOFFS_RW_DMA_RX_MAX_LEN_SMALL          5
#define FEOFFS_RW_DMA_BTCRX0_ADDR_L             6
#define FEOFFS_RW_DMA_BTCRX0_ADDR_H             7
#define FEOFFS_RW_DMA_BER_ADDR_L                14
#define FEOFFS_RW_DMA_BER_ADDR_H                15
#define FEOFFS_RW_DMA_BTCRX1_ADDR_L             16
#define FEOFFS_RW_DMA_BTCRX1_ADDR_H             17
#define FEOFFS_RW_DMA_BTCRX2_ADDR_L             18
#define FEOFFS_RW_DMA_BTCRX2_ADDR_H             19
#define FEOFFS_RW_DMA_BTCFHS_ADDR_L             20
#define FEOFFS_RW_DMA_BTCFHS_ADDR_H             21
#define FEOFFS_RW_DMA_RX_MAX_LEN_LARGE          29
#define FEOFFS_RW_DMA_SCATTER_GATHER            30
#define FEOFFS_RW_DMA_PRBS_SEL                  31
#define FEOFFS_RW_DMA_PRBS_RST                  31
#define FEOFFS_RO_RXBUF_STATUS0                 33
#define FEOFFS_RO_RXBUF_STATUS1                 34
#define FEOFFS_RW_PLL_XTAL_DIV_H_L_SIDE         62
#define FEOFFS_RW_TXON_MOD_ON_TIME              63
#define FEOFFS_RW_PA_OFF_TIME                   64
#define FEOFFS_RW_PLLTX_OFF_TIME                65
#define FEOFFS_RW_DIR_SEND_RF_DATA_B0           66
#define FEOFFS_RW_DIR_SEND_RF_DATA_B1           67
#define FEOFFS_RW_DIR_SEND_RF_DATA_B2           68
#define FEOFFS_RO_RF_WIRE3_BUSY                 69
#define FEOFFS_RW_RF_TRX_APPEND_EN              70
#define FEOFFS_RW_RF_3WIRE_INDEX                71
#define FEOFFS_WO_RF_TRX_VALUE_B0               72
#define FEOFFS_WO_RF_TRX_VALUE_B1               73
#define FEOFFS_WO_RF_TRX_VALUE_B2               74
#define FEOFFS_RO_CURR_HOP_RF_CH                76
#define FEOFFS_RW_PLL_XTAL_PARAM_B0             79
#define FEOFFS_RW_PLL_XTAL_PARAM_B1             80
#define FEOFFS_RW_PLL_DIVISOR_INV_B0            81
#define FEOFFS_RW_PLL_DIVISOR_INV_B1            82
#define FEOFFS_RW_PLL_DIVISOR_INV_B2            83
#define FEOFFS_RW_INTF                          84
#define FEOFFS_RW_INTF_INT_MASK                 85
#define FEOFFS_WO_TPOUT_SELECT                  96
#define FEOFFS_WR_RX_NULL_OR_POLL_CONTROL       97
#define FEOFFS_RW_SLOT_DELAY_CLK0               120
#define FEOFFS_RW_SLOT_DELAY_CLK1               121
#define FEOFFS_RW_SLOT_DELAY_CLK2               122
#define FEOFFS_RW_SLOT_DELAY_CLK3               123
#define FEOFFS_RW_AFH_MEASURE_EN                127
#define FEOFFS_RW_AFH_START_ADDRESS_B0          128
#define FEOFFS_RW_AFH_START_ADDRESS_B1          129
#define FEOFFS_RW_ABNORMAL_EVENT_INTR_CTL       140
#define FEOFFS_RO_ABNORMAL_EVENT                140
#define FEOFFS_RO_ABNORMAL_ADDR_B0              141
#define FEOFFS_RO_ABNORMAL_ADDR_B1              142
#define FEOFFS_RO_ABNORMAL_ADDR_B2              143
#define FEOFFS_RW_TX_GC_WIRE3_ADDR              145
#define FEOFFS_RW_TX_BIAS_WIRE3_ADDR            146
#define FEOFFS_RW_TXGC_CTL_BAND_TH1             147
#define FEOFFS_RW_TXGC_CTL_BAND_TH2             148
#define FEOFFS_RW_TXGC_CTL_BAND1_OFFSET         149
#define FEOFFS_RW_TXGC_CTL_BAND3_OFFSET         150
#define FEOFFS_WO_SWC_TX_GC1                    151
#define FEOFFS_WO_SWC_TX_GC2                    152
#define FEOFFS_WO_SWC_TX_BIAS2_1                153
#define FEOFFS_WO_SWC_TX_BIAS2_2                154
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B0        155
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B1        156
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B2        157
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B3        158
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B4        159
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B5        160
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B6        161
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B7        162
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B8        163
#define FEOFFS_WO_SWC_AFH_CHANNEL_MAP_B9        164
#define FEOFFS_WO_BER_ENABLE                    195
#define FEOFFS_RO_BER_CNT_B0                    196
#define FEOFFS_RO_BER_CNT_B1                    197
#define FEOFFS_RW_SNR_REPORT_CONTROL            198
#define FEOFFS_RO_N_M_PAGE_RESPONSE             215
#define FEOFFS_RO_N_S_PAGE_RESPONSE             216
#define FEOFFS_RO_N_INQUIRY_RESPONSE            217
#define FEOFFS_RW_SFR_DIV_ADDR_B0               240
#define FEOFFS_RW_SFR_DIV_ADDR_B1               241
#define FEOFFS_RW_SFR_DIV_ADDR_B2               242
#define FEOFFS_RW_SFR_FORCE_BB_32K              255



////////////////////////////////////////////////////////////////////////////////
// Macros //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SFR_RW_C2_OFFSET SFR_RW_OFFSET
#define SFR_RW_C3_OFFSET SFR_RW_OFFSET
#define SFR_RW_E2_OFFSET SFR_RW_OFFSET
#define SFR_RW_E3_OFFSET SFR_RW_OFFSET
#define SFR_RW_E4_OFFSET SFR_RW_OFFSET
#define SFR_RW_E6_OFFSET SFR_RW_OFFSET
#define SFR_RW_E7_OFFSET SFR_RW_OFFSET
#define SFR_RW_FB_OFFSET SFR_RW_OFFSET
#define SFR_RW_FE_OFFSET SFR_RW_OFFSET



////////////////////////////////////////////////////////////////////////////////
// Macro Prototypes ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- Set offset of bank register
#define BANK_SFR_OFFSET(addr,offset) { do { SFR_RW_##addr##_OFFSET = offset; } while (0); }
//- Write register of specific bank and offset
#define _BANK_SFR_WRITE(addr,offset,val) { do {                                 \
    BANK_SFR_OFFSET(addr,offset);                                               \
    SFR_RW_##addr = (U8)(val);                                                  \
    } while (0); }
//- Write register of specific bank and offset
#define BANK_SFR_WRITE(addr,offset,val) { do {                                  \
    OS_ENTER_CRITICAL();                                                        \
    _BANK_SFR_WRITE(addr,offset,val);                                           \
    OS_EXIT_CRITICAL();                                                         \
    } while (0); }
//- Write next successive register of specific bank based on previous setting offset
#define _BANK_SFR_SUCC_WRITE(addr,val) { do { SFR_RW_##addr = (U8)(val); } while (0); }
//- Read register of specific bank and offset
#define _BANK_SFR_READ(addr,offset) (SFR_RW_##addr##_OFFSET = offset, SFR_RW_##addr)
//- Read register of specific bank and offset
#define BANK_SFR_READ(addr,offset,val) { do {                                   \
    OS_ENTER_CRITICAL();                                                        \
    (val) = _BANK_SFR_READ(addr,offset);                                        \
    OS_EXIT_CRITICAL();                                                         \
    } while (0); }
//- Read next successive register of specific bank based on previous setting offset
#define _BANK_SFR_SUCC_READ(addr) (SFR_RW_##addr)



////////////////////////////////////////////////////////////////////////////////
// Legacy Macro Prototypes /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- C3
#define LC_SFR_WRITE_BY_C3(C3_OFFSET, SFR_VAL) \
        SFR_RW_C3_OFFSET = C3_OFFSET;   \
        SFR_RW_C3 = SFR_VAL;

#define LC_SFR_READ_BY_C3(C3_OFFSET)    (SFR_RW_C3_OFFSET = C3_OFFSET, SFR_RW_C3)

//- C2
#define LC_SFR_SET_C2_OFFSET(C2_OFFSET) (SFR_RW_C2_OFFSET = C2_OFFSET)

#define LC_SFR_WRITE_BY_C2(C2_OFFSET, SFR_VAL) \
        SFR_RW_C2_OFFSET = C2_OFFSET;   \
        SFR_RW_C2 = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_C2(SFR_VAL) \
        SFR_RW_C2 = SFR_VAL;

#define LC_SFR_READ_BY_C2(C2_OFFSET)    (SFR_RW_C2_OFFSET = C2_OFFSET, SFR_RW_C2)

#define LC_SFR_CONT_READ_BY_C2()        (SFR_RW_C2)

//- FB

#define LC_SFR_SET_FB_OFFSET(FB_OFFSET) (SFR_RW_C2_OFFSET = FB_OFFSET)

#define LC_SFR_WRITE_BY_FB(FB_OFFSET, SFR_VAL) \
        SFR_RW_C2_OFFSET = FB_OFFSET;   \
        SFR_RW_FB = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_FB(SFR_VAL) \
        SFR_RW_FB = SFR_VAL;

#define LC_SFR_READ_BY_FB(FB_OFFSET)    (SFR_RW_C2_OFFSET = FB_OFFSET, SFR_RW_FB)

#define LC_SFR_CONT_READ_BY_FB()        (SFR_RW_FB)

#define HC_SFR_SET_E4_OFFSET(E4_OFFSET) \
    OS_ENTER_CRITICAL();           \
    SFR_RW_E4_OFFSET = E4_OFFSET;  \
    OS_EXIT_CRITICAL()

#define HC_SFR_SET_FB_OFFSET(FB_OFFSET) \
    OS_ENTER_CRITICAL();           \
    SFR_RW_FB_OFFSET = FB_OFFSET;  \
    OS_EXIT_CRITICAL()

#define HC_SFR_WRITE_BY_FB(FB_OFFSET, SFR_VAL) \
    OS_ENTER_CRITICAL();           \
    SFR_RW_FB_OFFSET = FB_OFFSET;   \
    SFR_RW_FB = SFR_VAL;            \
    OS_EXIT_CRITICAL()

#define HC_SFR_CONT_WRITE_BY_FB(SFR_VAL) \
    OS_ENTER_CRITICAL ();           \
    SFR_RW_FB = SFR_VAL;            \
    OS_EXIT_CRITICAL()
//Modified by Eric Chou, 2010.9.24
//Adding critical section for error prevention.
#define HC_SFR_READ_BY_FB(FB_OFFSET, value) \
    OS_ENTER_CRITICAL ();            \
    value = (SFR_RW_FB_OFFSET = FB_OFFSET, SFR_RW_FB);  \
    OS_EXIT_CRITICAL()

#define HC_SFR_CONT_READ_BY_FB()      (SFR_RW_FB)

//-----------BT WLAN co-exist test by Kaiwen-------------
//#define BT_WLAN_COEXIST_test
#ifdef BT_WLAN_COEXIST_test
#define LC_SFR_WRITE_BY_FB(FB_OFFSET, SFR_VAL) \
        SFR_RW_FB_OFFSET = FB_OFFSET;   \
        SFR_RW_FB = SFR_VAL;
#endif
//-------------------------------------------------------

//E2
#define SFR_SET_E2_OFFSET(E2_OFFSET) (SFR_RW_E2_OFFSET = E2_OFFSET)
#define SFR_WRITE_BY_E2(E2_OFFSET,SFR_VAL) \
        SFR_RW_E2_OFFSET = E2_OFFSET;   \
        SFR_RW_E2 = SFR_VAL;

#define SFR_READ_BY_E2(E2_OFFSET)   (SFR_RW_E2_OFFSET = E2_OFFSET, SFR_RW_E2)
#define SFR_CONT_WRITE_BY_E2(SFR_VAL) \
        SFR_RW_E2 = SFR_VAL;

#define SFR_CONT_READ_BY_E2()       (SFR_RW_E2)

//E3
#define SFR_SET_E3_OFFSET(E3_OFFSET) (SFR_RW_E3_OFFSET = E3_OFFSET)
#define SFR_WRITE_BY_E3(E3_OFFSET,SFR_VAL) \
        SFR_RW_E3_OFFSET = E3_OFFSET;   \
        SFR_RW_E3 = SFR_VAL;

#define SFR_READ_BY_E3(E3_OFFSET)   (SFR_RW_E3_OFFSET = E3_OFFSET, SFR_RW_E3)
#define SFR_CONT_WRITE_BY_E3(SFR_VAL) \
        SFR_RW_E3 = SFR_VAL;

#define SFR_CONT_READ_BY_E3()       (SFR_RW_E3)

//E4
#define LC_SFR_SET_E4_OFFSET(E4_OFFSET) (SFR_RW_E4_OFFSET = E4_OFFSET)

#define LC_SFR_WRITE_BY_E4(E4_OFFSET, SFR_VAL) \
        SFR_RW_E4_OFFSET = E4_OFFSET;   \
        SFR_RW_E4 = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_E4(SFR_VAL) \
        SFR_RW_E4 = SFR_VAL;


#define LC_SFR_READ_BY_E4(E4_OFFSET)    (SFR_RW_E4_OFFSET = E4_OFFSET, SFR_RW_E4)

#define HC_SFR_WRITE_BY_E4(E4_OFFSET, SFR_VAL) \
        OS_ENTER_CRITICAL();           \
        SFR_RW_E4_OFFSET = E4_OFFSET;   \
        SFR_RW_E4 = SFR_VAL;            \
        OS_EXIT_CRITICAL()

#define HC_SFR_READ_BY_E4(E4_OFFSET, value) \
        OS_ENTER_CRITICAL ();            \
        value = (SFR_RW_E4_OFFSET = E4_OFFSET, SFR_RW_E4);  \
        OS_EXIT_CRITICAL()

//- E6
#define LC_SFR_SET_E6_OFFSET(E6_OFFSET) (SFR_RW_E6_OFFSET = E6_OFFSET)

#define LC_SFR_WRITE_BY_E6(E6_OFFSET, SFR_VAL) \
        SFR_RW_E6_OFFSET = E6_OFFSET;   \
        SFR_RW_E6 = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_E6(SFR_VAL) \
        SFR_RW_E6 = SFR_VAL;

#define LC_SFR_READ_BY_E6(E6_OFFSET)    (SFR_RW_E6_OFFSET = E6_OFFSET, SFR_RW_E6)

#define LC_SFR_CONT_READ_BY_E6()        (SFR_RW_E6)


//- E7
#define LC_SFR_SET_E7_OFFSET(E7_OFFSET) (SFR_RW_E7_OFFSET = E7_OFFSET)

#define LC_SFR_WRITE_BY_E7(E7_OFFSET, SFR_VAL) \
        SFR_RW_E7_OFFSET = E7_OFFSET;   \
        SFR_RW_E7 = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_E7(SFR_VAL) \
        SFR_RW_E7 = SFR_VAL;

#define LC_SFR_READ_BY_E7(E7_OFFSET)    (SFR_RW_E7_OFFSET = E7_OFFSET, SFR_RW_E7)

#define LC_SFR_CONT_READ_BY_E7()        (SFR_RW_E7)


//- FE
#define LC_SFR_SET_FE_OFFSET(FE_OFFSET) (SFR_RW_FE_OFFSET = FE_OFFSET)

#define LC_SFR_WRITE_BY_FE(FE_OFFSET, SFR_VAL) \
        SFR_RW_FE_OFFSET = FE_OFFSET;   \
        SFR_RW_FE = SFR_VAL;

#define LC_SFR_CONT_WRITE_BY_FE(SFR_VAL) \
        SFR_RW_FE = SFR_VAL;

#define LC_SFR_READ_BY_FE(FE_OFFSET)    (SFR_RW_FE_OFFSET = FE_OFFSET, SFR_RW_FE)

#define LC_SFR_CONT_READ_BY_FE()        (SFR_RW_FE)


// SFR_VAL can be another SFR
#define LC_SFR_WRITE(_SFR_, SFR_VAL) \
        _SFR_ = SFR_VAL;

//0119
#define LC_SFR_WRITE_BY_B2(SFR_VAL) \
        SFR_USB_ADDR = SFR_VAL;
#define LC_SFR_WRITE_BY_B3(SFR_VAL) \
        SFR_USB_DATA = SFR_VAL;

#define LC_SFR_READ(_SFR_) (_SFR_)

#endif /* _HW_SFR_H_ */

