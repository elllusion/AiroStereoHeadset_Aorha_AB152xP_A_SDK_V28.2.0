#ifndef _MMI_INQUIRY_H_
	#define _MMI_INQUIRY_H_
	
#ifdef MMI_INQUIRY_CMD

	#ifdef _MMI_INQUIRY_C_
		#define MMI_INQUIRY_EXTERN
	#else
		#define MMI_INQUIRY_EXTERN extern
	#endif
	
	PUBLIC U8 MMI_INQ_StartInquiry(MMIMsgType XDATA_PTR msgPtr);
	PUBLIC void MMI_INQ_GetInquiryComplete(void);
	PUBLIC void MMI_INQ_GetInquiryResultWithRSSI(U8 XDATA_PTR dataPtr);
	PUBLIC void MMI_INQ_GetExtendedInquiryResult(U8 XDATA_PTR dataPtr);
	
#endif //MMI_INQUIRY

#endif //_MMI_INQUIRY_H_