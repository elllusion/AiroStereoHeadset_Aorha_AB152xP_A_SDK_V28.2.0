//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _OPP_INTERFACE_H_
#define _OPP_INTERFACE_H_

#include "os.h"
#include "bt_config_profile.h"

#ifdef __OPP_C__
#define OPP_EXTERN
#else
#define OPP_EXTERN	extern
#endif


PUBLIC void OPP_Init(void);

#endif
