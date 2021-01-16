/*
This file implement the idea of NVKEY.

Provide 1024 bytes in the flash for the user to save his/her own data. We divide this space into 4 NVKEYs
(from NVKEY_USR0~NVKEY_USR3), and each has 256 bytes.

Before using NVKEY, the user needs to call SDK_NVKEY_Init() first. After that, the user can retrieve the kay value
by calling SDK_NVKEY_Read(), and change the key value in the flash by calling SDK_NVKEY_Write().
*/
#include "os.h"

#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\SDK\include\NvRam.h"
#include "sector_merge_runtime.h"
#include "..\MMI\MMI_SectorManager.h"
#include "..\MMI\MMI_Interface.h"
#include "..\ABSDRIVER\drv_program_flash_flash.h"

#include "..\..\mmi\MMI.h"


#if SDK_NV_RAM

static U8 XDATA_PTR g_ptrNVKEY_WritingBuffer=0;
static U8 XDATA gNVKEY_WritingWaitingFlag=0; // 0: No writing waiting; 1: Data waiting for writing to the flash
static U16 XDATA gNVKEY_WritingID=0;
static U16 XDATA gNVKEY_WritingLength=0;

// We need to adjust it to align with 4 bytes. This is the request of FLASH_PROGRAM_WriteXDataToFlash() function.
// And by using the page program function, we need to write 256 bytes each time.
static U8 XDATA_PTR g_ptrFlashPageProgramBuffer=0;

static const U32 NVKEY_ADDRESS_OFFSET[NVKEY_TOTAL_COUNT]={
    NVKEY_USR0*NVKEY_USR_SIZE,
    NVKEY_USR1*NVKEY_USR_SIZE,
    NVKEY_USR2*NVKEY_USR_SIZE,
    NVKEY_USR3*NVKEY_USR_SIZE
};

static CUSTOMIZE_DATA_TYPE CODE_PTR SDK_NvRam_GetFlashAddr(void)
{
	return (CUSTOMIZE_DATA_TYPE CODE_PTR)MMI_SECTOR_GetValidCustormerSectorAddr();
}

static U8 XDATA_PTR SDK_NvRam_OS_malloc(U16 len) // 256 bytes allocation
{
	U8 XDATA_PTR ptr;
	
	ptr =OS_malloc(len);//TX buf
	
	if(ptr==(U8 XDATA_PTR)NULL) 
	{
		if(len<OSMEM2_SMALL_RX_BLKSIZE)
			ptr=OSMEM_Get(OSMEM_ptr2_small_RX); // 1050 bytes x 5

		LightDbgPrint("SDK OS_malloc RxMem:%h", (U16) len);
	}

	return ptr;
}
#define OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(PTR_A) (OS_CONTAINER_OF(PTR_A,OSMEM_BLK,data_space)->header.mem_ptr)
static BOOL SDK_NvRam_OS_ChangeMemoryBlock(U8 XDATA_PTR XDATA_PTR ptr1, U16 len)
{
	U8 XDATA_PTR newPtr;

    //LightDbgPrint("change len %x %x", (U8)len, (U8)(len>>8));
    if(*ptr1 == NULL)
		return FALSE;
	if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(*ptr1) == OSMEM_ptr1)
	{
		if(len < OSMEM1_BLKSIZE)
		{
			return TRUE;
		}		
	}
	else if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(*ptr1) == OSMEM_ptr2_small_TX)
	{
		if(len < OSMEM2_SMALL_TX_BLKSIZE)
		{
			return TRUE;
		}		
	}
	else if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(*ptr1) == OSMEM_ptr2_large_TX)
	{
		if(len < OSMEM2_LARGE_TX_BLKSIZE)
		{
			return TRUE;
		}		
	}
	else if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(*ptr1) == OSMEM_ptr2_small_RX)
	{
		if(len < OSMEM2_SMALL_RX_BLKSIZE)
		{
			return TRUE;
		}		
	}

	//Release old memory and allocate large memory
	newPtr = SDK_NvRam_OS_malloc(len);
	if(newPtr)
	{
		OSMEM_Put(*ptr1);
		*ptr1 = newPtr;
		LightDbgPrint("SDK_CB_ChangeMemory");
		return TRUE;
	}	

	return FALSE;
}


static BOOL SDK_CB_NvRam_AllocProgMem(void)
{
	//LightDbgPrint("SDK RxMem:%d,%d,%d",
		//									(U8) OSMEM_NFree(OSMEM_ptr2_small_TX),
		//									(U8) OSMEM_NFree(OSMEM_ptr2_small_RX),
		//									(U8) OSMEM_NFree(OSMEM_ptr2_large_TX));
	if(g_ptrFlashPageProgramBuffer)
	{
		LightDbgPrint("SDK_CB_NvRam_DoubleAllocate");
		return TRUE;
	}

	g_ptrFlashPageProgramBuffer=SDK_NvRam_OS_malloc(NVKEY_USR_SIZE+4); // We need to align to 4 bytes boundary, so add additonal 4 bytes.
	if(g_ptrFlashPageProgramBuffer)
	{
		//LightDbgPrint("SDK_CB_NvRam_Alloca");
		return TRUE;
	}
	else
	{
		LightDbgPrint("SDK_CB_NvRam_NoMem");
		return FALSE;
	}
}	
static void SDK_CB_NvRam_UpdatingFlash(U8 CODE_PTR flashAddr)
{
    U16 i;
	U8 XDATA_PTR ptrAlign;

	if(!g_ptrFlashPageProgramBuffer)
	{
		LightDbgPrint("SDK_CB_NvRam_UpNoMem");
		return;
	}
	

	ptrAlign = g_ptrFlashPageProgramBuffer;
		// Do the 4 bytes boundary alignment.
	if(((U32)ptrAlign)%4)
			ptrAlign=(U8 XDATA_PTR)((U32)g_ptrFlashPageProgramBuffer+4-((U32)g_ptrFlashPageProgramBuffer)%4);

    for(i=0; i<NVKEY_TOTAL_COUNT; i++)
	{ // 1 page = 256 bytes = 1 NVKEY User Size

		// First: Read from the flash and updating with the data waiting for writing.
		SDK_NVKEY_Read(i, ptrAlign, NVKEY_USR_SIZE);

		// Second: Clear the  the data waiting for writing
		if(gNVKEY_WritingWaitingFlag && gNVKEY_WritingID==i)
		{
            //for(j=0; j<gNVKEY_WritingLength; j++)
            //	g_ptrFlashPageProgramBuffer[j]=g_ptrNVKEY_WritingBuffer[j];
            if(g_ptrNVKEY_WritingBuffer)
			OSMEM_Put(g_ptrNVKEY_WritingBuffer); // Free the memory.
			else
				LightDbgPrint("SDK_CB_NvRam_Up-WRITE NULL");	
			g_ptrNVKEY_WritingBuffer=0;
			gNVKEY_WritingLength=0;
            gNVKEY_WritingWaitingFlag=0;
		}

        // This function forces us to write 256 bytes each time.
        if(!FLASH_PROGRAM_WriteXDataToFlash(flashAddr+NVKEY_ADDRESS_OFFSET[i], ptrAlign, NVKEY_USR_SIZE))
		{
            LightDbgPrint("SDK - SaveNvRam Fail 0x%X 0x%X", (U32)(flashAddr), (U32)(NVKEY_ADDRESS_OFFSET[i]));
            ASSERT(FALSE);
        }
    }
	
	OSMEM_Put(g_ptrFlashPageProgramBuffer); // Free the memory.
	g_ptrFlashPageProgramBuffer=0;
	LightDbgPrint("SDK_CB_NvRam_UpdatingFlash-Done()");

    return;
}

void SDK_NVKEY_Init(void)
{
    CUSTOMIZE_DATA_TYPE CODE_PTR flashAddr=SDK_NvRam_GetFlashAddr();
    if(!flashAddr){
        LightDbgPrint("NvRam data is not built in system");
        ASSERT(FALSE);
    }

    // Register a callback function, this callback function will be called during the running of the background flash updating procedure.
    SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM, API_GRP_IOSYS, (VFUN)SDK_CB_NvRam_AllocProgMem);
    SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_NVRAM_UPDATE, API_GRP_IOSYS, (VFUN)SDK_CB_NvRam_UpdatingFlash);
}

// Read the data stored in the specific NVKEY out to the memory pointed by the parameter ptr.
U16 SDK_NVKEY_Read(U16 Key, U8 XDATA_PTR ptr, U16 length)
{
	U8 XDATA_PTR ptrMalloc;
	U8 XDATA_PTR ptrFlashPageReadBuffer;
    CUSTOMIZE_DATA_TYPE CODE_PTR flashAddr=SDK_NvRam_GetFlashAddr();
    U16 i;
    
    
    //LightDbgPrint("SDK_NVKEY_Read()-%h",(U16)length);
    // Parameters check.
    if(Key >= NVKEY_TOTAL_COUNT || !ptr || !length || length > NVKEY_USR_SIZE){
        return 0;
    }

	if(((U32)ptr)%4) 
	{
		ptrMalloc=SDK_NvRam_OS_malloc(NVKEY_USR_SIZE+4); // We need to align to 4 bytes boundary, so add additonal 4 bytes.
		LightDbgPrint("SDK_NVKEY_Read()-Alloc");
	}
	else 
	{
		ptrMalloc=ptr;
	}

	if(!ptrMalloc){
		LightDbgPrint("SDK_NVKEY_Read() NG");
		return 0;
	}

	ptrFlashPageReadBuffer=ptrMalloc;
	if(((U32)ptrFlashPageReadBuffer)%4)
	{
		// Do the 4 bytes boundary alignment.
		ptrFlashPageReadBuffer=(U8 XDATA_PTR)((U32)ptrFlashPageReadBuffer+4-((U32)ptrFlashPageReadBuffer)%4);
		LightDbgPrint("SDK_NVKEY_Read-ALIGN");
	}

    FLASH_PROGRAM_PageReadConti(
        FLASH_MEMORY_MAP((U32)((U32)flashAddr+NVKEY_ADDRESS_OFFSET[Key])),
        ptrFlashPageReadBuffer,
        length);

	//Copy flash data
	OSMEM_memcpy_xdata_xdata(ptr,ptrFlashPageReadBuffer, length);

	//Update the data waiting for writing.
    if(gNVKEY_WritingWaitingFlag==1 && gNVKEY_WritingID==Key)
	{
		i = gNVKEY_WritingLength;
		if(gNVKEY_WritingLength>length)
			i = length;

		
		if(g_ptrNVKEY_WritingBuffer)
		{
			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)ptr,(U8 XDATA_PTR)g_ptrNVKEY_WritingBuffer, i);
			//LightDbgPrint("SDK_NVKEY_R-Writ %d,%d,%d",(U8)i,(U8)ptr[0],(U8)g_ptrNVKEY_WritingBuffer[0]);
        }
        else
			LightDbgPrint("SDK_NVKEY_Read-MEM NULL");
    }

	if(ptrMalloc!=ptr)
	OSMEM_Put(ptrMalloc); // Free the memory.

	//LightDbgPrint("SDK_NVKEY_Read() End-%h",(U16)length);

    return length;
}

/*
If we already have data pending for writing to the flash and the pending writing action has different NVKEYID, we will return
value with 0. The user should check the return value, and decide whether he or she needs to write again later or just discard
this writing action.
*/
U16 SDK_NVKEY_Write(U16 Key, U8 XDATA_PTR ptr, U16 length)
{
    //LightDbgPrint("SDK NVKEY_Write-%h", (U16)length);

    // Parameters check.
    if(Key >= NVKEY_TOTAL_COUNT || !ptr || !length || length > NVKEY_USR_SIZE){
        return 0;
    }

    if(gNVKEY_WritingWaitingFlag!=0 && gNVKEY_WritingID!=Key) // Already have data waiting for writing, no buffer can be used.
        return 0;

    // Store the writing data into the buffer for next updating procedure invoked.
	if(!g_ptrNVKEY_WritingBuffer){
		g_ptrNVKEY_WritingBuffer=SDK_NvRam_OS_malloc(length);
		if(!g_ptrNVKEY_WritingBuffer) // The system has no more memory to support this writing action.
		{
			LightDbgPrint("NO MEM");
			return 0;
		}			
		
		gNVKEY_WritingLength=0;
	}
	else
	{
		LightDbgPrint("SDK NVKEY_Write-Mem exist");
		//Check length to check memory size
		if(length>gNVKEY_WritingLength)
		{
			if(!SDK_NvRam_OS_ChangeMemoryBlock(&g_ptrNVKEY_WritingBuffer, length))
			{
				LightDbgPrint("NO MEM2");
				return 0;
			}
		}	
	}
    gNVKEY_WritingWaitingFlag=1;
    gNVKEY_WritingID=Key;
	if(gNVKEY_WritingLength<length)
		gNVKEY_WritingLength=length;

	//Copy data
	OSMEM_memcpy_xdata_xdata(g_ptrNVKEY_WritingBuffer,ptr, length);
 
    MMI_SECTOR_SetUpdateFlag(TRUE); // Notify we have data waiting for write.
    
    return length;
}
#endif
