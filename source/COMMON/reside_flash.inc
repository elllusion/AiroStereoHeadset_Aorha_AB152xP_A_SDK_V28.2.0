#ifdef _RESIDE_FLASH_INC_
#error "RESIDE_FLASH.INC shall be include to c file directly. Don't include twice."
#else
#define _RESIDE_FLASH_INC_
#ifdef __C251__
#pragma userclass (UCODE = FLASH)
#pragma userclass (HCONST = FLASH)
#endif
#endif /* _RESIDE_FLASH_INC_ */
