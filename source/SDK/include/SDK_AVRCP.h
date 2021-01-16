
#if SDK_AVRCP_SHUFFLE
PUBLIC void SDK_AVRCP_SendGetShuffleStatus(U8 linkIndex);
PUBLIC void SDK_AVRCP_SendSetShuffleStatus(U8 linkIndex, U8 status);
PUBLIC void SDK_AVRCP_KeyShuffleChange(U8 linkIndex);
#endif

#if SDK_AVRCP_REPEAT
PUBLIC void SDK_AVRCP_SendGetRepeatStatus(U8 linkIndex);
PUBLIC void SDK_AVRCP_SendSetRepeatStatus(U8 linkIndex, U8 status);
PUBLIC void SDK_AVRCP_KeyRepeatChange(U8 linkIndex);
#endif

#if SDK_AVRCP_KEYS
PUBLIC void SDK_AVRCP_KeyPlay(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyStop(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyPause(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyRewindPush(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyFastForwardPush(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyForward(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyBackward(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyRewindRelease(U8 linkIndex);
PUBLIC void SDK_AVRCP_KeyFastForwardRelease(U8 linkIndex);
#endif


PUBLIC void SDK_AVRCP_Init(void);