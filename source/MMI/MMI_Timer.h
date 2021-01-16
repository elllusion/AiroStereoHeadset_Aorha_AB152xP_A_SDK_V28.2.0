#ifndef _MMI_TIMER_H_
	#define _MMI_TIMER_H_
	
	PUBLIC void MMI_TIMER_Init(void);
	PUBLIC void MMI_TIMER_ReleaseConnectionTimer(U8 linkIndex);
	PUBLIC void MMI_TIMER_ReleaseAllLinksConnectionTimer(void);
	PUBLIC void MMI_TIMER_ResetAutoSwitchOffTimer(void);
	PUBLIC void MMI_TIMER_ReleaseDetachTimer(U8 linkIndex);
	PUBLIC void MMI_TIMER_SetDetachTimer(U8 linkIndex, U32 time);
#endif
