//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _DSP_INTERFACE_H_
#define _DSP_INTERFACE_H_

#include "os.h"
#include "bt_config_profile.h"

#ifdef _DSP_C_
  #define DSP_EXTERN
#else
  #define DSP_EXTERN extern
#endif

#include "sector_peq_hpf_param.h"
#include "sector_mcu_dsp_param.h"
#include "sector_dsp_func_para.h"

#define ENDIAN_RVRS(A)	(((U16)((A) & 0x00FF) << 8) | ((U16)(A) >> 8))

#define DSP_CLOCK_RESERVED		0x0000
#define DSP_CLOCK_24M			0x0011
#define DSP_CLOCK_48M			0x0022
#define DSP_CLOCK_96M			0x0033
#define AIRO_SYNC_MAGIC_TIME    0x8301

extern U16 XDATA AiroSyncClk;

typedef struct
{
	DSP_FUNC_PARA_CTL_STRU CODE_PTR dspSectorPtr;
}DSP_CTL_TYPE;

DSP_EXTERN DSP_CTL_TYPE XDATA gDSP_ctl;

PUBLIC void DSP_Init (void);
PUBLIC VOID DSP_TrapNotifier(VOID);
PUBLIC VOID DSP_DebugPrint(VOID);
PUBLIC BOOL DSP_IsDSPEnabled(void);

#ifdef DSP_ENABLE
	PUBLIC void DSP_Enable(U16 dspClk);
	PUBLIC void DSP_Disable(void);
	PUBLIC void DSP_EnableDSPInterruptForAudio(void);
	PUBLIC void DSP_DisableDSPInterruptForAudio(void);
	PUBLIC U8 CODE_PTR DSP_GetCODEAddr(U8 CODE_PTR dsp_para_address);
	PUBLIC U16 DSP_GetCurrentClock(void);
#else
	#define DSP_Enable(dspClk)
	#define DSP_Disable()
	#define DSP_EnableDSPInterruptForAudio()
	#define DSP_DisableDSPInterruptForAudio()
	#define DSP_GetCODEAddr(dsp_para_address)	((U8 CODE_PTR)NULL)
	#define DSP_GetCurrentClock()				0
#endif

#endif
