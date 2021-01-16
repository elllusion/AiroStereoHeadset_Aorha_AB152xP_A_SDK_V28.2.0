//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_H_
	#define _MMI_H_

	#include "MMI_Base.h"
	
	#define AUTH_NONE				0x00
	#define AUTH_OK					0x01
	#define AUTH_ERROR 			0x02
	#define AUTH_ONGOING			0x03
	#define ENCRYPTED_NONE		0x00
	#define ENCRYPTED_OK			0x01
	#define ENCRYPTED_ERROR		0x02
	#define ENCRYPTED_ONGOING		0x03


	#define CHLD_FIRST_CALL_INDEX 1

	#define DEFAULT_LINK_IND 0 

	#ifdef _MMI_C_
		#define MMI_EXTERN
	#else
		#define MMI_EXTERN extern	
	#endif


	MMI_EXTERN MmiCtlType XDATA gMMI_ctl;

	//----------------------------------------------------
	//--------------- Battery & Charger ------------------
	//----------------------------------------------------
	#ifdef MMI_BATCHG_ENABLE
		PUBLIC void MMI_UpdateBatStatus(U8 batStatus);
		PUBLIC void MMI_CheckCurrentBatLevel(void);
	#endif

	

#endif		//#ifndef _MMI_H_

