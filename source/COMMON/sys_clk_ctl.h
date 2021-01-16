#ifndef _DRV_SYS_CLK_H_
#define _DRV_SYS_CLK_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2016 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_sys_clk.h is the file defines the interface of system clock register access
 *
 * Programmer : JoeHsu@airoha.com.tw, Ext.2666
 */
#include "rc_flash.h"
////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 SYS_CLK_SEL;
#define DRV_SYS_CLK_12MHZ             ((SYS_CLK_SEL)0x00)
#define DRV_SYS_CLK_24MHZ             ((SYS_CLK_SEL)0x01)

typedef U8 SYS_CLK_REQ_CMD;
#define SYS_CLK_24M_REQUEST            ((SYS_CLK_REQ_CMD)0x00)
#define SYS_CLK_24M_LEAVE              ((SYS_CLK_REQ_CMD)0x01)


typedef U8 SYS_CLK_REQ_ID;
#define SYS_CLK_REQ_RESERVED          ((SYS_CLK_REQ_ID)0x00)
#define SYS_CLK_REQ_LM                ((SYS_CLK_REQ_ID)0x01)
#define SYS_CLK_REQ_MMI               ((SYS_CLK_REQ_ID)0x02)
#define SYS_CLK_REQ_DRIVER            ((SYS_CLK_REQ_ID)0x03)
#define SYS_CLK_REQ_LL                ((SYS_CLK_REQ_ID)0x04)
#define SYS_CLK_REQ_LE                ((SYS_CLK_REQ_ID)0x05)

#define SYS_CLK_REQ_NUM               ((SYS_CLK_REQ_ID)0x06)

#ifdef SUPOORT_SYS_CLK_CHANGE

EXTERN U32 XDATA gSYS_Clk24MIdMask;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * FLASH_DRV_PMU_Init
 *
 * Set system clock
 *
 * @Author : JoeHsu <JoeHsu@airoha.com.tw>
 */

//EXTERN void DRV_SYS_CLK_Set(U8 clk_sel);
//EXTERN void DRV_SYS_CLK_24M_Request(SYS_CLK_REQ_ID clkMask);
//EXTERN void DRV_SYS_CLK_24M_Cancel(SYS_CLK_REQ_ID clkMask);

VOID SYS_CLK_Set(SYS_CLK_REQ_ID clkMask, SYS_CLK_REQ_CMD cmd);
SYS_CLK_SEL SYS_CLK_CurrClkGet(VOID);
U32 SYS_CLK_CurrClkMask(VOID);
VOID SYS_CLK_Init(VOID);
PUBLIC void SYS_CLK(void);

#else

#define SYS_CLK_Set(a,b)
#define SYS_CLK_CurrClkGet()      (0)
#define SYS_CLK_CurrClkMask()     (0)
#define SYS_CLK_Init()
#endif /* SUPPORT_SYS_CLK_CHANGE */
#endif /* _DRV_SYS_CLK_H_ */

