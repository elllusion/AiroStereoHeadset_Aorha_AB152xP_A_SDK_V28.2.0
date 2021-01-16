
#ifdef HFP_Profile
#include "..\HFP\HFP_Interface.h"
enum
{
	SDK_HFP_CMD_XEVENT = HFP_ATCMD_FROM_SDK_MASK,
	SDK_HFP_CMD_END,
};
#endif

PUBLIC void SDK_HFP_SendATCmd(U8 linkIndex, U8 cmdCode);

PUBLIC void SDK_HFP_Init(void);