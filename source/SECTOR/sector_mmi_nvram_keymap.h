#ifndef _SECTOR_MMI_NVRAM_KEYMAP_H_
#define _SECTOR_MMI_NVRAM_KEYMAP_H_

#include "sector_mmi_nvram_type.h"

/////// Key
#define MAX_KEY_ACTION_NUM	60		

typedef struct
{
	KeyIndex keyCode;
	U16 mmiKeyEvent;
	U32 keySuppotedState;
}MmiKeyMapType;

typedef struct
{
	U8 mmiKeyMapPairNo[KEY_ACTIONTOTALNUMBER]; 	
	MmiKeyMapType mmiKeyMapTbl[MAX_KEY_ACTION_NUM];
} MMI_NVRAM_KEYMAP;

typedef struct
{
	MMI_NVRAM_KEYMAP	mmi_nvram_keymap;

	CRC16 CRC;
	
} SECTOR_MMI_NVRAM_KEYMAP_STRU;

#endif //_SECTOR_MMI_NVRAM_KEYMAP_H_
