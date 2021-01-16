/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_patch_code_init.c defines the patch code initialization.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
//#include <srom.h>
#include "sector_sys_patch_code_init.h"

#ifdef AB1520B
#pragma userclass (HCONST = PATCH_INIT)
#else
#pragma userclass (HCONST = CONFIG)
#endif

//SROM_MC(ECODE_E_PATCH)

#ifdef AB1520B
#include "..\..\Patch\AB1520_PatchDefine.h"

	#ifdef ENABLE_PATCH_011
	extern void P011_f1_0xFFB116_1(void);
	extern void P011_f1_0xFFB116_2(void);
	#endif

	#ifdef ENABLE_PATCH_017
	extern void P017_f1_0xFFBE69_1(void);
	extern void P017_f1_0xFFBE69_2(void);
	#endif

	#ifdef ENABLE_PATCH_018
	extern void P018_f1_0xFEFCD0(void);
	#endif

	#ifdef ENABLE_PATCH_019
	extern void P019_f1_0xFEFE78(void);
	#endif
#endif

#if (defined AB1520C) || (defined AB1520D)
#include "..\..\Patch\AB1520C_PatchDefine.h"

	#ifdef ENABLE_PATCH_001
	extern void P001_f1_0xFE79F8(void);
	#endif

	#ifdef ENABLE_PATCH_002
	extern void P002_f1_0xFF78E4(void);
	#endif

	#ifdef ENABLE_PATCH_003
	extern void P003_f1_0xFF78C0(void);
	#endif

	#ifdef ENABLE_PATCH_004
	extern void P004_f1_0xFF7A74(void);
	#endif

	#ifdef ENABLE_PATCH_005
    extern void P005_f1_0xFEB5B8(void);
	#endif

	#ifdef ENABLE_PATCH_006
    extern void P006_f1_0xFE75C8(void);
	#endif

	#ifdef ENABLE_PATCH_007
    extern void P007_f1_0xFEB55C(void);
	#endif

	#ifdef ENABLE_PATCH_008
    extern void P008_f1_0xFEBED0(void);
	#endif

	#ifdef ENABLE_PATCH_009
    extern void P009_f1_0xFF799C_1(void);
	#endif

	#ifdef ENABLE_PATCH_009
    extern void P009_f1_0xFF799C_2(void);
	#endif

	#ifdef ENABLE_PATCH_010
    extern void P010_f1_0xFEB824(void);
	#endif

	#ifdef ENABLE_PATCH_011
    extern void P011_f1_0xFEB4C0(void);
	#endif

	#ifdef ENABLE_PATCH_012
    extern void P012_f1_0xFF795C(void);
	#endif

	#ifdef ENABLE_PATCH_013
    extern void P013_f1_0xFEB59C_1(void);

	#endif

	#ifdef ENABLE_PATCH_013
    extern void P013_f1_0xFEB59C_2(void);
	#endif

	#ifdef ENABLE_PATCH_014
    extern void P014_f1_0xFE7F58(void);
	#endif

	#ifdef ENABLE_PATCH_015
    extern void P015_f1_0xFF7B00(void);
	#endif

#endif

#ifdef AB1520D
/* Must be included in AB1520D */
#include "..\..\Patch\AB1520C_PatchDefine.h"
#ifdef OPERATION_IN_BB_ASIC_VERSION
extern void P100_f1_0xFFAF78_1(void);
extern void P100_f1_0xFFAF78_2(void);
#else
extern void P100_f1_0xFFAFA4_1(void);
extern void P100_f1_0xFFAFA4_2(void);
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_PATCH_CODE_INIT_STRU CODE gSector_SysPatchCodeInit =
{
    {
		{
		#ifdef OPERATION_IN_BB_ASIC_VERSION
			#if defined(AB1520B)
				#ifdef ENABLE_PATCH_011
				(U8 CODE_PTR)P011_f1_0xFFB116_1,
				(U8 CODE_PTR)P011_f1_0xFFB116_2,
				#endif

				#ifdef ENABLE_PATCH_017
				(U8 CODE_PTR)P017_f1_0xFFBE69_1,
				(U8 CODE_PTR)P017_f1_0xFFBE69_2,
				#endif

				#ifdef ENABLE_PATCH_018
				(U8 CODE_PTR)P018_f1_0xFEFCD0,
				#endif

				#ifdef ENABLE_PATCH_019
				(U8 CODE_PTR)P019_f1_0xFEFE78,
				#endif

			#elif (defined AB1520C) || (defined AB1520D)
				#ifdef AB1520D
				/* Must use index 0 and 1 and must be included in AB1520D */
				(U8 CODE_PTR)P100_f1_0xFFAF78_1,
				(U8 CODE_PTR)P100_f1_0xFFAF78_2,
				#endif
			
				#ifdef ENABLE_PATCH_001
				(U8 CODE_PTR)P001_f1_0xFE79F8,
				#endif

				#ifdef ENABLE_PATCH_002
				(U8 CODE_PTR)P002_f1_0xFF78E4,
				#endif

				#ifdef ENABLE_PATCH_003
				(U8 CODE_PTR)P003_f1_0xFF78C0,
				#endif

				#ifdef ENABLE_PATCH_004
				(U8 CODE_PTR)P004_f1_0xFF7A74,
				#endif

				#ifdef ENABLE_PATCH_005
				(U8 CODE_PTR)P005_f1_0xFEB5B8,
				#endif

				#ifdef ENABLE_PATCH_006
				(U8 CODE_PTR)P006_f1_0xFE75C8,
				#endif

				#ifdef ENABLE_PATCH_007
				(U8 CODE_PTR)P007_f1_0xFEB55C,
				#endif

				#ifdef ENABLE_PATCH_008
				(U8 CODE_PTR)P008_f1_0xFEBED0,
				#endif

				#ifdef ENABLE_PATCH_009
				(U8 CODE_PTR)P009_f1_0xFF799C_1,
				#endif

				#ifdef ENABLE_PATCH_009
				(U8 CODE_PTR)P009_f1_0xFF799C_2,
				#endif

				#ifdef ENABLE_PATCH_010
				(U8 CODE_PTR)P010_f1_0xFEB824,
				#endif

				#ifdef ENABLE_PATCH_011
				(U8 CODE_PTR)P011_f1_0xFEB4C0,
				#endif

				#ifdef ENABLE_PATCH_012
				(U8 CODE_PTR)P012_f1_0xFF795C,
				#endif

				#ifdef ENABLE_PATCH_013
				(U8 CODE_PTR)P013_f1_0xFEB59C_1,
				#endif

				#ifdef ENABLE_PATCH_013
				(U8 CODE_PTR)P013_f1_0xFEB59C_2,
				#endif

				#ifdef ENABLE_PATCH_014
				(U8 CODE_PTR)P014_f1_0xFE7F58,
				#endif
	            
				#ifdef ENABLE_PATCH_015
                (U8 CODE_PTR)P015_f1_0xFF7B00,
	            #endif
			#else
				NULL,
			#endif
		#else
			#ifdef AB1520D
			/* Must use index 0 and 1 and must be included in AB1520D */
			(U8 CODE_PTR)P100_f1_0xFFAFA4_1,
			(U8 CODE_PTR)P100_f1_0xFFAFA4_2,
			#endif
			NULL,
		#endif
		},

		{
			0x0000, //PatchBankSize,  SROM_MC_LEN(ECODE_E_PATCH)
			0x0000, //EdataAddr,      SROM_MC_TRG(EDATA_E_PATCH)
		},
    },

    //- CRC
    {0,0}
};

