PUBLIC void SDK_SDP_Init(void);

PUBLIC BOOL SDK_SDP_ProfileConnect(U8 linkIndex);
PUBLIC void SDK_SDP_ProfileDisconnect(U8 linkIndex);
PUBLIC void SDK_SDP_SendServiceSearchReq(U8 linkIndex, U16 uuid16);
PUBLIC void SDK_SDP_SendAttributeSearchReq(U8 linkIndex, U32 serviceRecordHandle, U16 attributeId1, U16 attributeId2);
PUBLIC void SDK_SDP_SendServiceSearchAttributeReq(U8 linkIndex, U16 uuid16, U16 attirbuteId);