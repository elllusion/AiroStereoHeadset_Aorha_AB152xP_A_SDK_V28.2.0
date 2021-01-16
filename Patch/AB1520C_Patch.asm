#include "config.h"
#include "AB1520C_PatchDefine.h"

#ifdef ENABLE_PATCH_001
$INCLUDE ( AB1520C_P001_HcVcmdModeChange.asm )
#endif

#ifdef ENABLE_PATCH_002
$INCLUDE ( AB1520C_P002_LeLabtestRx.asm )
#endif

#ifdef ENABLE_PATCH_003
$INCLUDE ( AB1520C_P003_LabTest_LE_TxTest_Hopping_Scheme_1.asm )
#endif

#ifdef ENABLE_PATCH_004
$INCLUDE ( AB1520C_P004_LabTest_LE_TxTest_Hopping_Scheme_2.asm )
#endif

#ifdef ENABLE_PATCH_005
$INCLUDE ( AB1520C_P005_LabTest_BRX_RSSI_stable_1.asm )
#endif

#ifdef ENABLE_PATCH_006
$INCLUDE ( AB1520C_P006_LabTest_BRX_RSSI_stable_2.asm )
#endif

#ifdef ENABLE_PATCH_007
$INCLUDE ( AB1520C_P007_LCS_LabTestRXDoneHandler.asm )
#endif

#ifdef ENABLE_PATCH_008
$INCLUDE ( AB1520C_P008_LC_LabTestRFTestBRxPacketSetup.asm )
#endif

#ifdef ENABLE_PATCH_009
$INCLUDE ( AB1520C_P009_LC_LabTestLeRxTestSetup.asm )
#endif

#ifdef ENABLE_PATCH_010
$INCLUDE ( AB1520C_P010_LC_EnterLabTestInit.asm )
#endif

#ifdef ENABLE_PATCH_011
$INCLUDE ( AB1520C_P011_LabTest_TxDone.asm )
#endif

#ifdef ENABLE_PATCH_012
$INCLUDE ( AB1520C_P012_LC_LabTestLeRxTestSetup.asm )
#endif

#ifdef ENABLE_PATCH_013
$INCLUDE ( AB1520C_P013_LabTestRXDoneHandler.asm )
#endif

#ifdef ENABLE_PATCH_014
$INCLUDE ( AB1520C_P014_HC_HciLabTestRdThreeWiredVCmdHdlr.asm )
#endif

#ifdef ENABLE_PATCH_015
$INCLUDE ( AB1520C_P015_LC_LabTestLeTestStop.asm )
#endif

#ifdef ENABLE_PATCH_016
$INCLUDE (  )
#endif

#ifdef ENABLE_PATCH_017
$INCLUDE (  )
#endif

#ifdef ENABLE_PATCH_018
$INCLUDE (  )
#endif

#ifdef ENABLE_PATCH_019
$INCLUDE (  )
#endif

#ifdef ENABLE_PATCH_020
$INCLUDE (  )
#endif

/* P100 must be included in AB1520D */	
$IF (AB1520D = 1)
#ifdef OPERATION_IN_BB_ASIC_VERSION
$INCLUDE ( AB1520D_P100_gOsIntrHdlrVector.asm )
#else
$INCLUDE ( AB1520D_P100_gOsIntrHdlrVector_FPGA.asm )
#endif
$ENDIF

END
