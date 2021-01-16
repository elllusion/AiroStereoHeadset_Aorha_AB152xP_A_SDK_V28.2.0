#ifndef _MMI_AIR_FUNCTION_H_
	#define _MMI_AIR_FUNCTION_H_

#ifdef AIR_MODULE


PUBLIC BOOL MMI_AIR_IsSCOEncodePayload(U8 linkIndex);
PUBLIC VOID MMI_AIR_EscoDataRelay(void);
PUBLIC BOOL MMI_AIR_EscoPacketHandler(U8 XDATA_PTR dataPtr);
PUBLIC void MMI_Air_Function_Init(void);
PUBLIC void MMI_Share_SendSCORelay(U8 linkIndex, U8 isRelay, U8 phoneLink);
PUBLIC void MMI_Share_SendSCOVol(U8 linkIndex, U8 soundLevel, U8 smallStep);
PUBLIC void MMI_Share_EnableSCORelay(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void MMI_Share_SetSCOVol(U8 linkIndex, U8 XDATA_PTR dataPtr);

#endif
	
	
#endif