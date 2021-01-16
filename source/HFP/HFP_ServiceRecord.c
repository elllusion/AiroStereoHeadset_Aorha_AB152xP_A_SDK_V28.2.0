#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#define VERSION_HSP		0x0102
#define VERSION_HFP		0x0107

#ifdef HFP_Profile

  #ifdef HFP_AG_Profile
	#include "HFP_ServiceRecord_AG.c"
  #else
	#include "HFP_ServiceRecord_HF.c"
  #endif

#endif
