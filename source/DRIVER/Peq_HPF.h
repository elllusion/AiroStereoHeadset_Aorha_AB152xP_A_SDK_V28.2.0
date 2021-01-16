#ifndef _PEQ_H
#define _PEQ_H

#ifdef PEQ_C
	#define EXTERN_PEQ	
#else
	#define EXTERN_PEQ	extern
#endif

#include "..\DRIVER\DRIVER_interface.h"

#define INIT_ENABLED_MODE				0
#define PEQ_ENABLED_MODE				1<<0
#define HPF_ENABLED_MODE				1<<1
#define PEQ_EXPANSION_ENABLED_MODE		1<<2
#define PEQ_UNDER_100HZ_ENABLED_MODE	1<<3
#define REALTIME_PEQ_HPF_SET_END		0xFF

#define PEQ_DEFAULT_ASSIGNMENT_FEAT 0x0800

typedef struct
{
	PEQ_CONFIG_SECTOR peqPara;
	HPF_CONFIG_SECTOR hpfPara;
	U8 hpfEnable;
	U8 peqEnable;
	PEQ_CONFIG_SECTOR peqExpansionPara;
	PEQ_100HZ_SECTOR peqUnder100hzPara;
}DSP_PEQ_HPF_PTR_STRU;

enum
{
	PEQ_A2DP,
	PEQ_LINE_IN,
	PEQ_COMPONENT_NUM,
};

enum
{
	PEQ_LOAD_DEFAULT,
	PEQ_LOAD_CUSTOM,
	PEQ_LOAD_SECTOR_NUM,
};

enum
{
	HPF_PARAMETER,
	PEQ_PARAMETER,
	PEQ_Expansion_PARAMETER,
	PEQ_Under100Hz_PARAMETER,
	PEQ_PARAMETER_NUM,
};

typedef struct
{
	U8 enable_mode; 					// 1<<0: enable peq, 1<<1: enable hpf, 1<<2: enable peq_expansion, 1<<3: enable peq_under100Hz
	U8 XDATA_PTR sendToDspPtr; 
}PEQCtrlRealTimePEQCmd;

typedef struct
{
	U8 PEQMode[PEQ_COMPONENT_NUM];
	U8 PEQSectorMode[PEQ_COMPONENT_NUM];
	U8 CODE_PTR sectorPEQAddress;
	U8 CODE_PTR sectorHPFAddress;
	PEQCtrlRealTimePEQCmd SetRealtimePEQ;
}PQE_CTL_TYPE;

EXTERN_PEQ PQE_CTL_TYPE XDATA gPeq_ctl;


EXTERN_PEQ PUBLIC BOOL PEQ_SetOff(void);
EXTERN_PEQ PUBLIC BOOL PEQ_SetOn(U8 component);
EXTERN_PEQ PUBLIC BOOL HPF_SetOff(void);
EXTERN_PEQ PUBLIC BOOL HPF_SetOn(U8 component);
#ifdef PEQ_EXPANSION_ENABLE
EXTERN_PEQ PUBLIC BOOL PEQ_Expansion_Set(U8 component);
EXTERN_PEQ PUBLIC BOOL PEQ_IsPEQExpansionEnabledInSector(void);
#endif
#ifdef PEQ_UNDER_100HZ_ENABLE
EXTERN_PEQ PUBLIC BOOL PEQ_Under100Hz_Set(U8 component);
#endif
EXTERN_PEQ PUBLIC void PEQ_SetPeqHpfLoadSectorAddress(U8 sector);
EXTERN_PEQ PUBLIC void PEQ_HPF_UpdateParameters(void);
EXTERN_PEQ PUBLIC void PEQ_ReleaseResource(void);
EXTERN_PEQ PUBLIC BOOL PEQ_IsPEQEnabledInSector(U8 component);
EXTERN_PEQ PUBLIC BOOL PEQ_IsCustomPEQSectorEmpty(U8 component);
EXTERN_PEQ PUBLIC void PEQ_RealTimeSetPeqHpf(void);
EXTERN_PEQ PUBLIC BOOL HPF_IsHPFEnabledInSector(U8 component);
EXTERN_PEQ PUBLIC U8 PEQ_GetModeNumber(U8 component);
EXTERN_PEQ PUBLIC U8 PEQ_GetModeIndex(U8 component);
EXTERN_PEQ PUBLIC void PEQ_SetModeIndex(U8 component, U8 peqModeIndex);
EXTERN_PEQ PUBLIC U8 PEQ_GetSectorMode(U8 component);
EXTERN_PEQ PUBLIC void PEQ_SetSectorMode(U8 component, U8 sectorMode);
EXTERN_PEQ PUBLIC void PEQ_SetRealtimeEnableMode(U8 value);
EXTERN_PEQ PUBLIC U8 XDATA_PTR PEQ_GetRealtimePEQParameterPtr(U8 paramter);
EXTERN_PEQ PUBLIC void PEQ_AllocateRealtimePEQParameterPtr(void);
EXTERN_PEQ PUBLIC void PEQ_SetPeqParameter(U8 component, U8 peqModeIndex, U8 sectorMode);
#endif
