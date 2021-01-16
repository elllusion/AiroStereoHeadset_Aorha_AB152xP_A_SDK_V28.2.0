#ifndef _OS_FLASH_H_
#define _OS_FLASH_H_

#include "os.h"
#include "os_memtop_flash.h"




typedef struct
{
    U16             timer_in_half_slot;
    OST XDATA_PTR   timer_handle;

} OS_POWERSAVINGWAITINGTIME_CTL;



EXTERN OS_POWERSAVINGWAITINGTIME_CTL XDATA gOS_PowerSavingWaitingTimerCtl;
EXTERN U16 XDATA TaskingIndFlag;
EXTERN U8 XDATA  gOS_RoundBeforePowerSaving;

EXTERN PUBLIC void OS_SetPowerSavingLevel(U8 level);
EXTERN PUBLIC void OS_MMI1Enable(U16 duration);
EXTERN PUBLIC void OS_MMI1Disable(void);

#endif /* _OS_FLASH_H_ */

