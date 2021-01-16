#include "reside_flash.inc"

#define PEQ_C

#include <rc.h>
#include "bt_config_profile.h"

#ifdef PEQ_ENABLE
#include "align_flash.h"

#ifdef AB1520B
#include "sector_dsp_data.h"
#else
#include "sector_boundary.h"
#include "sector_config1.h"
#endif


#include "Peq_HPF.h"
#include "..\MMI\MMI.h"

PUBLIC BOOL PEQ_SetOff(void)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;

	if(gAlignDSP.PTR_TO_XDATA_PEQ)
		return FALSE;

	ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr2_small_RX);

	if(ptr1 == (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)NULL)
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)SYS_MemoryGet(sizeof(DSP_PEQ_HPF_PTR_STRU));

	if(ptr1 != (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)NULL)
	{
		ptr1->peqEnable = 0;
		gAlignDSP.PTR_TO_XDATA_PEQ = (U16 XDATA_PTR)ptr1;
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL PEQ_SetOn(U8 component)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	U8 offset;

	if(gAlignDSP.PTR_TO_XDATA_PEQ)
	{
		return FALSE;
	}

	ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr2_small_RX);

	if(ptr1 == (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)NULL)
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)SYS_MemoryGet(sizeof(DSP_PEQ_HPF_PTR_STRU));

	if(ptr1 != (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)NULL)
	{
		offset = 0;
		//set gPeq_ctl.numOfPEQ[PEQ_LINE_IN] = 0 means A2DP and Line-In share a block of peq data (i.e. offset = 0)
		if(component == PEQ_LINE_IN && PEQ_GetModeIndex(PEQ_LINE_IN) != 0)
		{
			offset = PEQ_GetModeNumber(PEQ_A2DP);
		}

		OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&ptr1->peqPara,
								(U8 CODE_PTR)(gPeq_ctl.sectorPEQAddress + sizeof(U8) + sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeIndex(component) + offset -1)),
								 sizeof(PEQ_CONFIG_SECTOR));
		ptr1->peqEnable = 1<<0;
		gAlignDSP.PTR_TO_XDATA_PEQ = (U16 XDATA_PTR)ptr1;
		LightDbgPrint("PEQ_ON:%h-%h-%h", (ptr1->peqPara.peq_sample_rate[0].peq_sample_rate_content[0]),
					ptr1->peqPara.peq_sample_rate[1].peq_sample_rate_content[0], 
			ptr1->peqPara.peq_sample_rate[2].peq_sample_rate_content[0]
			);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL HPF_SetOff(void)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	if(gAlignDSP.PTR_TO_XDATA_PEQ)
	{
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gAlignDSP.PTR_TO_XDATA_PEQ;
		ptr1->hpfEnable = 0;
		gAlignDSP.DSP_CHANGE_PEQ = 1;
		LightDbgPrint("peq peqenable:%d  hpfenable:%d",(U8)ptr1->peqEnable, (U8)ptr1->hpfEnable);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL HPF_SetOn(U8 component)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	U8 offset;
	U8 index;
	U8 CODE_PTR PEQaddr;
	
	if(gAlignDSP.PTR_TO_XDATA_PEQ)
	{
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gAlignDSP.PTR_TO_XDATA_PEQ;
		offset = 0;
		if(component == PEQ_LINE_IN && PEQ_GetModeIndex(PEQ_LINE_IN) != 0)
		{
			offset = PEQ_GetModeNumber(PEQ_A2DP);
		}
		//if PEQMode = 0 or 1, load 1st HPF setting
		if(PEQ_GetSectorMode(component) == PEQ_LOAD_DEFAULT)
		{
			OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&ptr1->hpfPara,
									(U8 CODE_PTR)(gPeq_ctl.sectorHPFAddress + sizeof(U8) + sizeof(HPF_CONFIG_SECTOR)*((PEQ_GetModeIndex(component)?(PEQ_GetModeIndex(component)-1):0)+offset)),
									sizeof(HPF_CONFIG_SECTOR));
		}
		else
		{	
			// PEQ part
			PEQaddr = gPeq_ctl.sectorPEQAddress;
			PEQaddr += sizeof(U8) + sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN));		
			
			// PEQ expansion part
			#ifdef PEQ_EXPANSION_ENABLE
			PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN))):sizeof(U8);
			#else
			PEQaddr += sizeof(U8);
			#endif
			
			// Audio Transparency part
			PEQaddr += sizeof(U8) + (sizeof(PEQ_CONFIG_SECTOR)*(*(U8 CODE_PTR)(PEQaddr)));
			
			// PEQ under 100Hz part
			for(index = 0; index < (PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN)); index++)
			{
				PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(PEQ_100HZ_SECTOR)+sizeof(U8)):sizeof(U8);
			}
			
			// HPF part
			PEQaddr += sizeof(U8);
			PEQaddr += sizeof(HPF_CONFIG_SECTOR)*((PEQ_GetModeIndex(component)?(PEQ_GetModeIndex(component) - 1) : 0) + offset);		
			OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&ptr1->hpfPara,
									(U8 CODE_PTR)(PEQaddr),
									sizeof(HPF_CONFIG_SECTOR));
		}
		ptr1->hpfEnable = 1;
		gAlignDSP.DSP_CHANGE_PEQ = 1;
		LightDbgPrint("peq peqenable:%d  hpfenable:%d",(U8)ptr1->peqEnable, (U8)ptr1->hpfEnable);
		return TRUE;
	}
	return FALSE;
}

#ifdef PEQ_EXPANSION_ENABLE
PUBLIC BOOL PEQ_Expansion_Set(U8 component)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	U8 offset;
	
	if(gAlignDSP.PTR_TO_XDATA_PEQ)
	{
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gAlignDSP.PTR_TO_XDATA_PEQ;
		if(PEQ_IsPEQExpansionEnabledInSector())
		{
			offset = 0;
			//set gPeq_ctl.numOfPEQ[PEQ_LINE_IN] = 0 means A2DP and Line-In share a block of peq data (i.e. offset = 0)
			if(component == PEQ_LINE_IN && PEQ_GetModeIndex(PEQ_LINE_IN) != 0)
			{
				offset = PEQ_GetModeNumber(PEQ_A2DP);
			}

			OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&ptr1->peqExpansionPara,
									 (U8 CODE_PTR)(gPeq_ctl.sectorPEQAddress + sizeof(U8) + sizeof(U8) 
									 + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN)) 
									 + sizeof(U8) + (sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeIndex(component) + offset -1))),
									 sizeof(PEQ_CONFIG_SECTOR));
			ptr1->peqEnable |= 1<<1;
		}
		else
		{
			ptr1->peqEnable &= ~(1<<1);
		}
		return TRUE;
	}
	return FALSE;
}
#endif

#ifdef PEQ_UNDER_100HZ_ENABLE
PUBLIC BOOL PEQ_Under100Hz_Set(U8 component)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	U8 CODE_PTR PEQaddr;
	U8 index;
	
	if(gAlignDSP.PTR_TO_XDATA_PEQ)
	{
		ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gAlignDSP.PTR_TO_XDATA_PEQ;
		PEQaddr = gPeq_ctl.sectorPEQAddress;
		
		//PEQ part
		PEQaddr += sizeof(U8) + sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN));		
		
		//PEQ expansion part
		#ifdef PEQ_EXPANSION_ENABLE
		PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN))):sizeof(U8);
		#else
		PEQaddr += sizeof(U8);
		#endif
		
		//Audio Transparency part
		PEQaddr += sizeof(U8) + (sizeof(PEQ_CONFIG_SECTOR)*(*(U8 CODE_PTR)(PEQaddr)));
		
		//PEQ under 100Hz part
		if(component == PEQ_LINE_IN && PEQ_GetModeIndex(PEQ_LINE_IN) != 0)
		{
			for(index = 0; index < PEQ_GetModeNumber(PEQ_A2DP); index++)
			{
				PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(PEQ_100HZ_SECTOR)+sizeof(U8)):sizeof(U8);
			}
			
			for(index = 0; index < (PEQ_GetModeIndex(component) - 1); index++)
			{
				PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(PEQ_100HZ_SECTOR)+sizeof(U8)):sizeof(U8);
			}
		}
		else
		{
			for(index = 0; index < (PEQ_GetModeIndex(component) - 1); index++)
			{
				PEQaddr += (*(U8 CODE_PTR)(PEQaddr))?(sizeof(PEQ_100HZ_SECTOR)+sizeof(U8)):sizeof(U8);
			}
		}
		
		//check peq 100Hz a2dp/linein set enable and copy parameter
		if(*(U8 CODE_PTR)(PEQaddr) == 0)
		{
			ptr1->peqEnable &= ~(1<<2);
		}
		else
		{
			PEQaddr += sizeof(U8);			
			OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&ptr1->peqUnder100hzPara,
									(U8 CODE_PTR)(PEQaddr),
									sizeof(PEQ_100HZ_SECTOR));
			ptr1->peqEnable |= 1<<2;
		}
		
		return TRUE;
	}
	return FALSE;
}
#endif

PUBLIC void PEQ_RealTimeSetPeqHpf(void)
{
	DSP_PEQ_HPF_PTR_STRU XDATA_PTR ptr1;
	ptr1 = (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gPeq_ctl.SetRealtimePEQ.sendToDspPtr;
	
	if(ptr1 != (DSP_PEQ_HPF_PTR_STRU XDATA_PTR)NULL)
	{
		//peq
		if(gPeq_ctl.SetRealtimePEQ.enable_mode & PEQ_ENABLED_MODE)
		{
			ptr1->peqEnable = 1<<0;
			
		}
		else
		{
			ptr1->peqEnable = 0;
		}

		#ifdef PEQ_EXPANSION_ENABLE
		//peq expansion
		if(gPeq_ctl.SetRealtimePEQ.enable_mode & PEQ_EXPANSION_ENABLED_MODE)
		{
			ptr1->peqEnable |= 1<<1;
			
		}
		else
		{
			ptr1->peqEnable  &= ~(1<<1);
		}
		#endif

		//hpf
		if(gPeq_ctl.SetRealtimePEQ.enable_mode & HPF_ENABLED_MODE)
		{
			ptr1->hpfEnable = 1;
			
		}
		else
		{
			ptr1->hpfEnable = 0;
		}
		
		#ifdef PEQ_UNDER_100HZ_ENABLE
		//peq under 100Hz
		if(gPeq_ctl.SetRealtimePEQ.enable_mode & PEQ_UNDER_100HZ_ENABLED_MODE)
		{
			ptr1->peqEnable |= 1<<2;
			
		}
		else
		{
			ptr1->peqEnable  &= ~(1<<2);
		}
		#endif
		
		gPeq_ctl.SetRealtimePEQ.sendToDspPtr = (U8 XDATA_PTR)NULL;
		gPeq_ctl.SetRealtimePEQ.enable_mode = INIT_ENABLED_MODE;
		gAlignDSP.PTR_TO_XDATA_PEQ = (U16 XDATA_PTR)ptr1;
		gAlignDSP.DSP_CHANGE_PEQ = 1;
		LightDbgPrint("peq peqenable3:%d  hpfenable:%d",(U8)ptr1->peqEnable, (U8)ptr1->hpfEnable);
	}
}

PUBLIC void PEQ_HPF_UpdateParameters(void)
{
	if(MMI_REMEMBER_PEQ_CHANGE_FEAT || MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
	{
		PEQ_SetModeIndex(PEQ_A2DP, gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP]);
		PEQ_SetModeIndex(PEQ_LINE_IN, gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN]);
	}
	
	if(MMI_MUSIC_FORCE_USE_PEQ_FEAT)
	{
		PEQ_SetModeIndex(PEQ_A2DP, PEQ_GetModeIndex(PEQ_A2DP) ? PEQ_GetModeIndex(PEQ_A2DP) : 1);
		PEQ_SetModeIndex(PEQ_LINE_IN, PEQ_GetModeIndex(PEQ_LINE_IN) ? PEQ_GetModeIndex(PEQ_LINE_IN) : 1);
	}
	
	PEQ_SetSectorMode(PEQ_A2DP, gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_A2DP]);
	PEQ_SetSectorMode(PEQ_LINE_IN, gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_LINE_IN]);
}


PUBLIC void PEQ_ReleaseResource(void)
{
	if(gAlignDSP.PTR_TO_XDATA_PEQ != (U16 XDATA_PTR)NULL)
	{
		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gAlignDSP.PTR_TO_XDATA_PEQ);
		gAlignDSP.DSP_CHANGE_PEQ = 0;
	}
}

PUBLIC BOOL PEQ_IsPEQEnabledInSector(U8 component)
{
	return PEQ_GetModeNumber(component) ? TRUE:FALSE;
}

PUBLIC BOOL HPF_IsHPFEnabledInSector(U8 component)
{
	U8 CODE_PTR PEQaddr;
	U8 index;
	
	if(PEQ_GetSectorMode(component) == PEQ_LOAD_CUSTOM)
	{
		// PEQ part
		PEQaddr = gPeq_ctl.sectorPEQAddress + sizeof(U8) + sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN));
		
		// PEQ expansion part
		#ifdef PEQ_EXPANSION_ENABLE
		PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN))):sizeof(U8);
		#else
		PEQaddr += sizeof(U8);
		#endif
		
		// Audio Transparency part
		PEQaddr += sizeof(U8) + (sizeof(PEQ_CONFIG_SECTOR)*(*(U8 CODE_PTR)(PEQaddr)));
		
		//PEQ under 100Hz part
		for(index = 0; index < (PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN)); index++)
		{
			PEQaddr += *(U8 CODE_PTR)(PEQaddr)?(sizeof(PEQ_100HZ_SECTOR)+sizeof(U8)):sizeof(U8);
		}
		
		return *(U8 CODE_PTR)(PEQaddr) ? TRUE : FALSE;
	}
	else
	{
		return ((DSP_HPF_PARAMETER_STRU CODE_PTR)gPeq_ctl.sectorHPFAddress)->hpf_enable ? TRUE:FALSE;
	}
}

#ifdef PEQ_EXPANSION_ENABLE
PUBLIC BOOL PEQ_IsPEQExpansionEnabledInSector(void)
{
	return *(U8 CODE_PTR)(gPeq_ctl.sectorPEQAddress + sizeof(U8) + sizeof(U8) + sizeof(PEQ_CONFIG_SECTOR)*(PEQ_GetModeNumber(PEQ_A2DP) + PEQ_GetModeNumber(PEQ_LINE_IN))) ? TRUE:FALSE;	
}
#endif

PUBLIC U8 PEQ_GetModeNumber(U8 component)
{
	return (component == PEQ_A2DP) ? *(U8 CODE_PTR)(gPeq_ctl.sectorPEQAddress) :
									 *(U8 CODE_PTR)(gPeq_ctl.sectorPEQAddress + sizeof(U8));	
}

PUBLIC void PEQ_SetModeIndex(U8 component, U8 peqModeIndex)
{
	gPeq_ctl.PEQMode[component] = peqModeIndex;
}

PUBLIC U8 PEQ_GetModeIndex(U8 component)
{
	return gPeq_ctl.PEQMode[component];
}

PUBLIC void PEQ_SetPeqHpfLoadSectorAddress(U8 sector)
{
	#ifdef AB1520B
	gPeq_ctl.sectorPEQAddress = (sector == PEQ_LOAD_DEFAULT) ?
								(U8 CODE_PTR)(GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_PEQ_PARAMETER)):
								(U8 CODE_PTR)(GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_PEQ_HPF_PARAMETER_CUSTOM));
	gPeq_ctl.sectorHPFAddress = (sector == PEQ_LOAD_DEFAULT) ?
								(U8 CODE_PTR)(GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_HPF_PARAMETER)):
								(U8 CODE_PTR) NULL;
	#else
	gPeq_ctl.sectorPEQAddress = (sector == PEQ_LOAD_DEFAULT) ?
								(U8 CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DSP_PEQ_PARAMETER)):
								(U8 CODE_PTR)(GET_SECTOR_BOUNDARY_ADDR(SECTOR_DSP_PEQ_HPF_PARAMETER_CUSTOM));
	gPeq_ctl.sectorHPFAddress = (sector == PEQ_LOAD_DEFAULT) ?
								(U8 CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DSP_HPF_PARAMETER)):
								(U8 CODE_PTR) NULL;
	#endif
	if(sector == PEQ_LOAD_DEFAULT)
		LightDbgPrint("==PEQ Load Default!!!");
	else
		LightDbgPrint("==PEQ Load Cust!!!!!!");
}

PUBLIC BOOL PEQ_IsCustomPEQSectorEmpty(U8 component)
{
	return (component == PEQ_A2DP) ? *(U8 CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_DSP_PEQ_HPF_PARAMETER_CUSTOM) == 0 :
								     *(U8 CODE_PTR)(GET_SECTOR_BOUNDARY_ADDR(SECTOR_DSP_PEQ_HPF_PARAMETER_CUSTOM) + sizeof(U8)) == 0;
}

PUBLIC void PEQ_SetSectorMode(U8 component, U8 sectorMode)
{
	gPeq_ctl.PEQSectorMode[component] = sectorMode;
}

PUBLIC U8 PEQ_GetSectorMode(U8 component)
{
	return gPeq_ctl.PEQSectorMode[component];
}

PUBLIC void PEQ_SetRealtimeEnableMode(U8 value)
{
	if(value == INIT_ENABLED_MODE)
		gPeq_ctl.SetRealtimePEQ.enable_mode = value;
	else
		gPeq_ctl.SetRealtimePEQ.enable_mode |= value;
}

PUBLIC U8 XDATA_PTR PEQ_GetRealtimePEQParameterPtr(U8 parameter)
{
	if(gPeq_ctl.SetRealtimePEQ.sendToDspPtr)
	{
		switch(parameter)
		{
			case HPF_PARAMETER:
				return &((DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gPeq_ctl.SetRealtimePEQ.sendToDspPtr)->hpfPara;
			case PEQ_PARAMETER:
				return &((DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gPeq_ctl.SetRealtimePEQ.sendToDspPtr)->peqPara;
			#ifdef PEQ_EXPANSION_ENABLE
			case PEQ_Expansion_PARAMETER:
				return &((DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gPeq_ctl.SetRealtimePEQ.sendToDspPtr)->peqExpansionPara;
			#endif
			#ifdef PEQ_UNDER_100HZ_ENABLE
			case PEQ_Under100Hz_PARAMETER:
				return &((DSP_PEQ_HPF_PTR_STRU XDATA_PTR)gPeq_ctl.SetRealtimePEQ.sendToDspPtr)->peqUnder100hzPara;
			#endif
			default:
				ASSERT(FALSE);
				break;
		}
	}
	return (U8 XDATA_PTR)NULL;
}

PUBLIC void PEQ_AllocateRealtimePEQParameterPtr(void)
{
	if(gPeq_ctl.SetRealtimePEQ.sendToDspPtr == NULL)
		gPeq_ctl.SetRealtimePEQ.sendToDspPtr = OSMEM_Get(OSMEM_ptr2_small_RX);

	if(gPeq_ctl.SetRealtimePEQ.sendToDspPtr == NULL)
		gPeq_ctl.SetRealtimePEQ.sendToDspPtr = SYS_MemoryGet(sizeof(DSP_PEQ_HPF_PTR_STRU));
	

}

PUBLIC void PEQ_SetPeqParameter(U8 component, U8 peqModeIndex, U8 sectorMode)
{
	PEQ_SetModeIndex(component, peqModeIndex);
	PEQ_SetSectorMode(component, sectorMode);
}
#endif
