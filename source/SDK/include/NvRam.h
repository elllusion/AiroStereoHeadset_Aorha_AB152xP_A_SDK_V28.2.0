#ifndef _NVRAM_H_
#define _NVRAM_H_

/*
This is the initialization function for SDK_NVKEY funtions. We need to call this API first before calling
any other SDK_NVKEY related functions.
*/
void SDK_NVKEY_Init(void);

/*
This funciton is used to retrieve a specific NVKEY's value. The maximum value of the the parameter lenght is equal
to the definition NVKEY_USR_SIZE. After calling it, the NVKEY's value will be read out and put in the memory buffer
pointed by the parameter ptr.

The parameter Key's value is between NVKEY_USR0 to NVKEY_USR3.

The return value will equal to the parameter length for a successful reading, else the return value will be 0 if anything
goes wrong.
*/
U16 SDK_NVKEY_Read(U16 Key, U8 XDATA_PTR ptr, U16 length);

/*
This funciton is used to write a specific NVKEY's value. The maximum value of the the parameter lenght is equal
to the definition NVKEY_USR_SIZE. After calling it, the NVKEY's value will be updated according to the memory buffer
pointed by the parameter ptr.

The parameter Key's value is between NVKEY_USR0 to NVKEY_USR3.

The return value will equal to the parameter length for a successful writing, else the return value will be 0 if anything
goes wrong (such as the system has no more resource to do more writing actions).
*/
U16 SDK_NVKEY_Write(U16 Key, U8 XDATA_PTR ptr, U16 length);

// We define each NVKEY occupy 256 bytes now. Must be the same as the flash page size.
#define NVKEY_USR_SIZE 256

// We can have at most 4 NVKEY for user usage now, since total space is 1024 bytes, and each NVKEY occupies 256 bytes. 1024/256=4.
enum{
    NVKEY_USR0,
    NVKEY_USR1,
    NVKEY_USR2,
    NVKEY_USR3,
    NVKEY_TOTAL_COUNT
};
#endif
