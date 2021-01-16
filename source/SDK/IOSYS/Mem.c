#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if SDK_MEMORY_ALLOCATION
/*
SDK_Mem_Alloc() can be used to dynamically allocate memory. The allocated memory should be released by SDK_Mem_Free().
Return NULL if the memory allocation is failed.
*/
void *SDK_Mem_Alloc(U16 size)
{
    return OS_malloc(size);
}

/*
SDK_Mem_Free() is used to free memory allocated by SDK_Mem_Alloc().
*/
void SDK_Mem_Free(void *ptr)
{
    OSMEM_Put((void XDATA_PTR)ptr);
}

#endif
