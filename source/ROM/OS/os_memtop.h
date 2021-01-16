#ifndef _OS_MEMTOP_H_
#define _OS_MEMTOP_H_

#include <srom.h>
SROM_MC(EDATA)	// Create external declarations for the EDATA Class
#define MEMORY_TOP 			(OS_DATA_PTR)((U32)SROM_MC_END(EDATA) + 1)

#endif /* _OS_MEMTOP_H_ */

