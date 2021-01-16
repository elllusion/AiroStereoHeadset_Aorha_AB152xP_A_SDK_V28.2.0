#include "reside_flash.inc"

#include "..\SDAP\SDAP_Interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"

#define VERSION_AVDTP	0x0103
#define VERSION_A2DP	0x0103

#ifdef A2DP_Profile

  #ifdef A2DP_SRC_Profile
	#include "A2DP_ServiceRecord_SRC.c"
  #else
	#include "A2DP_ServiceRecord_SNK.c"
  #endif

#endif
