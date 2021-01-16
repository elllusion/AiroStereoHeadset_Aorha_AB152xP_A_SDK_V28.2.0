#ifndef __L2CAP_RESOURCE_MANAGER_H__
#define __L2CAP_RESOURCE_MANAGER_H__

PUBLIC void L2CAP_ResourceManager(U8 linkIndex);

PUBLIC BOOL L2CAP_RecombineData(U8 linkIndex, U8 XDATA_PTR msgPtr, L2CAP_RECOMBINE_CTL XDATA_PTR combineCtlPtr);

#endif //__L2CAP_RESOURCE_MANAGER_H__
