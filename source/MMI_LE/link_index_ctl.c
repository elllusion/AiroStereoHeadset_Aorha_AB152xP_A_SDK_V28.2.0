//////////////////////////////////////////////////////////
// Copyright@ 2014 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"
#include "rc.h"

#ifdef LE_SUPPORTED_HOST_FLASH
#include "link_index_ctl.h"
#include "MMI_LE_interface.h"

enum
{
    LINK_STATE_NONE,
    LINK_STATE_WAITING_ACTIVE,
    LINK_STATE_ACTIVE,
};

typedef struct 
{
    U16 connHandle;   //the 2-byte connection handle
    U8 linkState;
    U8 peerAddrType;
    U8 peerAddr[BD_ADDR_LEN];
} LinkInfoType;

static LinkInfoType gLinkInfo[MAX_NUM_OF_LINK];
static U8 gActiveLink[MAX_NUM_OF_LINK];             //Active link index list
static U8 gNumOfActiveLink;



/*
 * LIC_Update_ActiveLink
 *
 * maintain the list of active link
 *
 */
PRIVATE void LIC_Update_ActiveLink(void)
{
	U8 idx;

	for(idx = 0, gNumOfActiveLink = 0; idx < MAX_NUM_OF_LINK; idx++)
	{
		if(gLinkInfo[idx].linkState == LINK_STATE_ACTIVE)
		{
			gActiveLink[gNumOfActiveLink] = idx;
			gNumOfActiveLink++;
		}
	}
}



/*
 * LIC_LE_AddLinkInfo
 *
 * add connection handle to link table
 *
 */
PUBLIC void LIC_LE_AddLinkInfo(HCI_LE_EVT_STRU XDATA_PTR ptr)
{
	U8 idx;
	U16 connHdl = TO_MCU_ENDIAN_U16(ptr->payload.connCmp.connHandle);

	if(connHdl > LIC_MAX_CONNECTION_HANDLE)
	{
		return;
	}
	
    #ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
	{
        U8 XDATA_PTR p1;
    	p1 = ptr->payload.connCmp.peerAddr;
    	LightDbgPrint("curr_Addr:%x %x %x %x %x %x", (U8)p1[0], (U8)p1[1], (U8)p1[2], (U8)p1[3], (U8)p1[4], (U8)p1[5]);
	}
    #endif
    
	for (idx = 0; idx < MAX_NUM_OF_LINK; idx++)
	{
	    if(gLinkInfo[idx].linkState == LINK_STATE_WAITING_ACTIVE)
		{
    		if((!OS_memcmp(gLinkInfo[idx].peerAddr, ptr->payload.connCmp.peerAddr, BD_ADDR_LEN)) 
    		    && (gLinkInfo[idx].peerAddrType == ptr->payload.connCmp.peerAddrType))
    	    {
                goto AddToLinkTableExit;
            }
        }
    }

	for(idx = 0; idx < MAX_NUM_OF_LINK; idx++)
    {
        if(gLinkInfo[idx].linkState != LINK_STATE_ACTIVE)
        {
            gLinkInfo[idx].peerAddrType = ptr->payload.connCmp.peerAddrType;
            OSMEM_memcpy_xdata_xdata(gLinkInfo[idx].peerAddr, ptr->payload.connCmp.peerAddr, BD_ADDR_LEN);
            goto AddToLinkTableExit;
		}
	}

AddToLinkTableExit:

    #ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
	LightDbgPrint("LIC add idx:%x, CH:%x %x", idx, (U8)(connHdl>>8), (U8)connHdl);
	#endif
    gLinkInfo[idx].linkState = LINK_STATE_ACTIVE;
    gLinkInfo[idx].connHandle = connHdl;
    LIC_Update_ActiveLink();
	#ifdef SM_SUPPORTED
		MMI_LE_SendUpdateLinkStatusToSM(idx, MMI_LE_LINK_STATUS_CONNECTION_CMP);
	#endif
}



/*
 * LIC_LE_DelLinkInfo
 *
 * delete connection handle from link table
 *
 */
PUBLIC U8 LIC_LE_DelLinkInfo(U16 connHdl)
{
	U8 idx;

    idx = LIC_LE_GetIdxByConnHdl(connHdl);
	
	//#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
	LightDbgPrint("LIC del idx:%x CH:%x %x", idx, (U8)(connHdl>>8), (U8)connHdl);
	//#endif
	
    if(idx == LIC_INVALID_LINK_INDEX)
    {
        return idx;
    }
    gLinkInfo[idx].connHandle = LIC_INVALID_CONNECTION_HANDLE;
    gLinkInfo[idx].linkState = LINK_STATE_WAITING_ACTIVE;
    LIC_Update_ActiveLink();
	#ifdef SM_SUPPORTED
	MMI_LE_SendUpdateLinkStatusToSM(idx, MMI_LE_LINK_STATUS_DISCONNECTION);
	#endif
	return idx;
}



PUBLIC void LIC_LE_Init(void)
{
    U8 i;
    for(i=0; i<MAX_NUM_OF_LINK; i++)
    {
        gLinkInfo[i].connHandle = LIC_INVALID_CONNECTION_HANDLE;
    }

	OSMEM_memset_xdata((U8 XDATA_PTR)&gActiveLink, 0x00, sizeof(gActiveLink));
	gNumOfActiveLink = 0;
	LIC_HalInit();
}



/**
 * LIC_LE_GetConnHdl
 *
 * Return the connection handle of corresponding link index
 * Return 0xFFFF if that link is not connected or in link index is not valid
 *
 */
PUBLIC U16 LIC_LE_GetConnHdl(U8 idx)
{
    if(idx < MAX_NUM_OF_LINK)
    {
        return gLinkInfo[idx].connHandle;
    }
	return LIC_INVALID_CONNECTION_HANDLE;
}



/**
 * LIC_LE_GetPeerAddr
 *
 * Return the peer addrress of corresponding link index
 * Return NULL if that link is not connected or in link index is not valid
 *
 */
PUBLIC U8 XDATA_PTR LIC_LE_GetPeerAddr(U8 idx)
{
    if(idx < MAX_NUM_OF_LINK)
    {
        return gLinkInfo[idx].peerAddr;
    }
	return NULL;
}



PUBLIC U8 LIC_LE_GetPeerAddrType(U8 idx)
{
    if(idx < MAX_NUM_OF_LINK)
    {
        return gLinkInfo[idx].peerAddrType;
    }
	return 0;
}



/**
 * LIC_LE_GetIdxByConnHdl
 *
 * Return the link index 
 * Return 0xFF if connection handle not found
 *
 */
PUBLIC U8 LIC_LE_GetIdxByConnHdl(U16 connHdl)
{
	U8 idx;

	if(connHdl <= LIC_MAX_CONNECTION_HANDLE)
    {
    	for(idx=0; idx < MAX_NUM_OF_LINK; idx++)
    	{
    	    if(gLinkInfo[idx].connHandle == connHdl)
    	    {
    			return idx;
    	    }
    	}
    }
    return LIC_INVALID_LINK_INDEX;
}



/**
 * LIC_LE_GetNumOfActiveLink
 *
 * Return number of Alive link
 *
 */
PUBLIC U8 LIC_LE_GetNumOfActiveLink(void)
{
	return gNumOfActiveLink;
}




/**
 * LIC_LE_GetActiveLinkIdx
 *
 * Return Alive link index
 *
 */
PUBLIC U8 LIC_LE_GetActiveLinkIdx(U8 i)
{
	return gActiveLink[i];
}
#endif




