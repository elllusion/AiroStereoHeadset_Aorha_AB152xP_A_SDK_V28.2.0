#include "reside_flash.inc"

#define _DSP_C_


#include "config.h"
#include <srom.h>
#include "rc_flash.h"
#include "DSP_Interface.h"
#include "sector_dsp_data.h"
#include "sector_boundary.h"
#include "align_flash.h"
#include "sector_config1.h"

#include "../HC/HC_Interface.h"
#include "../Driver/Driver_Interface.h"

#ifdef AB1520S
	#include "../DSP/AB1520S_DSP/DSP_ROM_AB1520S_VERSION.h"
#else
	#include "../DSP/AB1520_DSP/DSP_ROM_AB1520_VERSION.h"
#endif


#define DSP_PARA_SECTOR_DATA_OFFSET_IN_BYTE (2)


#ifdef DSP_ENABLE

#ifndef AB1528C_ONLY
SROM_MC(HCONST_DSP_ROM)	// Create external declarations for the HCONST_DSP_ROM Class
#endif

PUBLIC void DSP_Init (void)
{
	FOUR_BYTE_UNION_TYPE address;
	U8 CODE_PTR dsp_para_address;

    LightDbgPrint("DSP - DSP Revision: %D", (U32)DSP_ROM_VERSION);

    gAlignOSMEM.pAlignDSP = (U8 XDATA_PTR)&gAlignDSP;

    #ifdef AB1520B
    dsp_para_address = DSP_GetCODEAddr( GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_FUNC_PARAMETER) );
    #else
    dsp_para_address = DSP_GetCODEAddr( GET_SECTOR_BOUNDARY_ADDR(SECTOR_DSP_FUNC_PARAMETER) );
    #endif

    gAlignDSP.DSP_FUNC_PARA_START_ADDR = (U32)dsp_para_address;

    gAlignDSP.MCU_DSP_PARA_START_ADDR = ((U32)dsp_para_address) + sizeof(DSP_FUNC_PARA_STRU);


	SFR_WR_TEST_AND_SET = 0;

	//- load sector
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gAlignDSP.DSP_nvram, GET_SECTOR_CFG1_ADDR(SECTOR_MCU_DSP_PARAMETER_INIT), sizeof (DSP_NVRAM_CTL_TYPE));


    #ifdef AB1520B
	gDSP_ctl.dspSectorPtr = (DSP_FUNC_PARA_CTL_STRU CODE_PTR)GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_FUNC_PARAMETER);
    #else
    gDSP_ctl.dspSectorPtr = (DSP_FUNC_PARA_CTL_STRU CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_DSP_FUNC_PARAMETER);
    #endif

	if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->zsp_enable)
	{
		//- DSP
    	HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel&0x3));	//  1 DSP: 24M

		//- Read chip select
//		gAlignDSP.DSP_nvram.dsp_stereo_parameter.CHIP_SEL = LC_SFR_READ_BY_C2(C2OFFS_RO_VERSION_NUMBER) & 0x40;

		//- SMALL TRX MEMORY
	}

	#ifdef AB1528C_ONLY
	address.value = FLASH_MEMORY_MAP(GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_ROM));
	#else
	address.value = FLASH_MEMORY_MAP(SROM_MC_TRG(HCONST_DSP_ROM));
	#endif

	if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->zsp_enable)
	{
		HC_SFR_WRITE_BY_FB(FBOFFS_WO_DSP_RST, 0);			// ZSP disable

		LC_SFR_WRITE_BY_FE(FEOFFS_RW_SFR_DIV_ADDR_B0, address.in_byte.B0);
		LC_SFR_CONT_WRITE_BY_FE(address.in_byte.B1);
		LC_SFR_CONT_WRITE_BY_FE(address.in_byte.B2);

		HC_SFR_WRITE_BY_FB(FBOFFS_WO_DSP_BOOT_CLK, 1);		// 0 = external jtag or flash boot
		HC_SFR_WRITE_BY_FB(FBOFFS_WO_DSP_DMEM_SIZE, 11);	// 7 means DSP internal memory is 32K words

		//- for DSP ice debug
		if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->DSP_always_on)
		{
			//DSP_PTR_TO_XDATA = 0; sys init will reset all XDATA
			DSP_Enable (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel);
			SetPowerSavingForDevice(FALSE, PWRSAV_DSP);	//- Need to disable power saving when using DSP ice
		}
	}
}

PUBLIC U8 XDATA Enable_DSP;

PUBLIC void DSP_Enable (U16 dspClk)
{
	OST LDATA dspWaitingTimer;

	if (!Enable_DSP)
	{
		gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel = dspClk;
		dspClk &= 0x0003;
		OS_ENTER_CRITICAL ();
		HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (U8)dspClk);
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		HC_SFR_WRITE_BY_FB(FBOFFS_WO_DSP_RST, 1);			// enable DSP circuit
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		OS_EXIT_CRITICAL();

		gSYS_allow_updating = FALSE;
		Enable_DSP = TRUE;
		DRIVER_SetBusyMask(TRUE, DRIVER_BUSY_DSP_ON);

		OST_SetTimer(&dspWaitingTimer, 8L);
		while(!OST_TimerExpired(&dspWaitingTimer));

		LightDbgPrint("DSP - DSP Enable:%x",(U8)dspClk);
	}
}

PUBLIC void DSP_Disable (void)
{
	LightDbgPrint("DSP - DSP Will be Disabled");

    OS_delay_ms(5);

	OS_ENTER_CRITICAL ();
	_nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_DSP_RST, 0);               // ZSP disable
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MCU_CACHE_FLUSH, 1);       // MCU cache flush
	_nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
	OS_EXIT_CRITICAL();

	Enable_DSP = FALSE;
	gSYS_allow_updating = TRUE;
	DRIVER_SetBusyMask(FALSE, DRIVER_BUSY_DSP_ON);

	LightDbgPrint("DSP - DSP Disabled");
}

PUBLIC void DSP_EnableDSPInterruptForAudio(void)
{
	U8 mask;
	//enable DSP interrupt
	OS_ENTER_CRITICAL ();
	mask = LC_SFR_READ_BY_FE(FEOFFS_RW_INTF_INT_MASK);
	if(!(mask & INT1_ENABLE_DSP))
	{
		mask |= INT1_ENABLE_DSP; //0x80
		LC_SFR_WRITE_BY_FE(FEOFFS_RW_INTF_INT_MASK, mask); //dsp debug
	}
	OS_EXIT_CRITICAL ();
}

PUBLIC void DSP_DisableDSPInterruptForAudio(void)
{
	U8 mask;
	OS_ENTER_CRITICAL ();
	//stop the voice prompt automatically;
	mask = LC_SFR_READ_BY_FE(FEOFFS_RW_INTF_INT_MASK);
	mask &= (~INT1_ENABLE_DSP); //0x80
	LC_SFR_WRITE_BY_FE(FEOFFS_RW_INTF_INT_MASK, mask); //dsp debug
	OS_EXIT_CRITICAL ();
}

PUBLIC U8 CODE_PTR DSP_GetCODEAddr(U8 CODE_PTR dsp_para_address)
{
	#ifdef AB1528C_ONLY
    return (U8 CODE_PTR)((U32)dsp_para_address - (U32)GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_ROM));
	#else
	return (U8 CODE_PTR)((U32)dsp_para_address - (U32)SROM_MC_TRG(HCONST_DSP_ROM));
	#endif
}

PUBLIC VOID DSP_TrapNotifier(VOID)
{
    U16 DebugMsg = gAlignDSP.debug_var_trap;

    if (DebugMsg == 0xFFFF)
    {
        LightDbgPrint("DSP - DSP Is In D-Sleep");
    }
    else if (DebugMsg >= 1000)
    {
        DebugMsg -= 1000;
        LightDbgPrint("DSP - DSP Warning Msg: %D", (U32)DebugMsg);
		LightDbgPrint("DSP - %D,%D,%D", (U32)gAlignDSP.debug_var01,
                                        (U32)gAlignDSP.debug_var02,
                                        (U32)gAlignDSP.debug_var03);
    }
    else
    {
        LightDbgPrint("DSPTrap%D,%D,%D,%D", (U32)DebugMsg,
                                            (U32)gAlignDSP.debug_var01,
                                            (U32)gAlignDSP.debug_var02,
                                            (U32)gAlignDSP.debug_var03);
        ASSERT(FALSE);
    }
}

PUBLIC VOID DSP_DebugPrint(VOID)
{
    U8 XDATA_PTR ptr1;

    while (ptr1 = (U8 XDATA_PTR)OSMQ_MCU_DSP_Get(OSMQ_DSP_DebugDataOut_ptr))
    {
        LightDbgPrintDsp(ptr1);
	    OSMEM_Put(ptr1);
    }
}


PUBLIC U16 DSP_GetCurrentClock(void)
{
	return gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel;
}

#endif //DSP_ENABLE

PUBLIC BOOL DSP_IsDSPEnabled(void)
{
	#ifdef DSP_ENABLE
	return (Enable_DSP) ? TRUE : FALSE;
	#else
	return FALSE;
	#endif
}
