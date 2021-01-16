//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _RFCOMM_INTERFACE_H_
#define _RFCOMM_INTERFACE_H_

#include "..\L2CAP\L2CAP_Interface.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define RFCOMM_RX_OFFSET		(L2CAP_RX_PDU_OFFSET + L2CAP_HEADER_LEN)
#define RFCOMM_TX_OFFSET		(L2CAP_TX_PDU_OFFSET + L2CAP_HEADER_LEN)

typedef struct
{
	U8 offset;
	U8 reserved[RFCOMM_RX_OFFSET - 1];
	U8 profileId;
	U16 dataLength;
	U8 dataBody[1];
} ProfileDataEvent;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define RFCOMM_RX_CREDITS_DEFAULT		3	/* max incoming credits should equal or greater than 2 */
#define RFCOMM_RX_CREDITS_THRESHOLD		2

typedef struct
{
	MMI_PROFILE_DESC mmiProfileDesc;
	U16 maxFrameSize;
	void (*GivenCreditsEvent)(U8 linkIndex, U8 givenCredits);
} RFCOMM_PROFILE_DESC;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef _RFCOMM_C_
	#define RFCOMM_EXTERN
#else
	#define RFCOMM_EXTERN extern
#endif

PUBLIC void RFCOMM_RegisterProfile(MMI_PROFILE_NODE XDATA_PTR profileNode);

PUBLIC void RFCOMM_SendProfileData(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void RFCOMM_SendGivenCredits(U8 linkIndex, U8 profileId, U8 givenCredits);

PUBLIC U8 RFCOMM_GetProfileDataOverhead(U16 infoLen);
PUBLIC void RFCOMM_SetProfileDataHeader(U16 infoLen, U8 profileId, U8 XDATA_PTR dataPtr);

PUBLIC U8 XDATA_PTR RFCOMM_AllocProfileData(U16 infoLen, U8 profileId);

PUBLIC U8 RFCOMM_ConnectProfile(U8 linkIndex, U8 profileId);
PUBLIC U8 RFCOMM_DisconnectProfile(U8 linkIndex, U8 profileId);


#endif //end of _RFCOMM_INTERFACE_H_
