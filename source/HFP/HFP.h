//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _HFP_H_
#define _HFP_H_

#ifndef _BT_CONFIG_PROFILE_H_
#error "To include bt_config_profile.h before this file!!!"
#endif

PUBLIC U8 HFP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);

PUBLIC void HFP_CommandHandle(U8 linkIndex, U8 cmdCode, MMIMsgType XDATA_PTR cmdPtr);
PUBLIC void HFP_CommandComplete (U8 linkIndex);

#define HFP_CHAR_TO_DIGIT(A)	((U8)((A) - 0x30))
#define HFP_DIGIT_TO_CHAR(A)	((U8)((A) + 0x30))

PUBLIC U8 XDATA_PTR HFP_FindToken (U8 XDATA_PTR ptr1, U8 token);
PUBLIC U8 XDATA_PTR HFP_FindComma (U8 XDATA_PTR ptr1);
PUBLIC U8 XDATA_PTR HFP_FindColon (U8 XDATA_PTR ptr1);

PUBLIC U8 XDATA_PTR HFP_U16_To_ASCII(U8 XDATA_PTR dst, U16 number);
PUBLIC U16 HFP_ASCII_TO_U16 (U8 XDATA_PTR ptr1);

PUBLIC U8 AT_CompareString (U8 XDATA_PTR ptr1, U8 CODE_PTR c_ptr, U8 length);
PUBLIC void AT_UpperCaseAndEliminateSpace (U8 XDATA_PTR src, U16 length);


#define AT_CR		0x0D
#define AT_LF		0x0A
#define AT_EOF		0xFF


//////////////////////////////////////////////////////////////////////////
///
/// Profile-Dependended Functions
///
//////////////////////////////////////////////////////////////////////////
#ifdef HFP_AG_Profile
  #define AT_SegmentAndEliminateSpace		AT_SegmentAndEliminateSpace_AG
  #define HFP_TimeoutMonitor				HFP_TimeoutMonitor_AG
  #define HFP_MainStateEntry				HFP_MainStateEntry_AG
  #define HFP_ProcessCmdFromMMI				HFP_ProcessCmdFromMMI_AG
#else
  #define AT_SegmentAndEliminateSpace		AT_SegmentAndEliminateSpace_HF
  #define HFP_TimeoutMonitor				HFP_TimeoutMonitor_HF
  #define HFP_MainStateEntry				HFP_MainStateEntry_HF
  #define HFP_ProcessCmdFromMMI				HFP_ProcessCmdFromMMI_HF
#endif

PUBLIC void AT_SegmentAndEliminateSpace (U8 linkIndex, U8 XDATA_PTR ptr1);

PUBLIC void HFP_TimeoutMonitor(U8 linkIndex);
PUBLIC void HFP_MainStateEntry (U8 linkIndex);
PUBLIC void HFP_ProcessCmdFromMMI (U8 linkIndex);

#ifdef HFP_AG_Profile
  #include "HFP_Core_AG.h"
#else
  #include "HFP_Core_HF.h"
#endif

#endif
