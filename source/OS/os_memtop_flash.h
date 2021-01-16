#ifndef _OS_MEMTOP_FLASH_H_
#define _OS_MEMTOP_FLASH_H_

#include <srom.h>
SROM_MC(EDATA)	// Create external declarations for the EDATA Class
#define MEMORY_TOP_FLASH 			(OS_DATA_PTR)((U32)SROM_MC_END(EDATA) + 1)
SROM_MC(EDATA_HIEDATA)
#define OS_HEAP_MEMORY_TOP		(((U32)SROM_MC_TRG(EDATA_HIEDATA)) - 1)

#endif /* _OS_MEMTOP_FLASH_H_ */

