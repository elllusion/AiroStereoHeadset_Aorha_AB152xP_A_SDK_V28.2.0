
#ifndef __SONGPAL_INTERFACE_H__
#define __SONGPAL_INTERFACE_H__

#ifndef _BT_CONFIG_PROFILE_H_
#error "To include bt_config_profile.h before this file!!!"
#endif


#define SONGPAL_MAX_FRAME_SIZE		240	/* to fit in uart output header structure */


EXTERN U8 CODE gSongPal_android_uuid128[16];

#ifndef IAP2_Profile
EXTERN U8 CODE gSongPal_iap2_uuid128[16];
#else
EXTERN U8 CODE gIAP2_uuid128[16];
#define gSongPal_iap2_uuid128	gIAP2_uuid128
#endif

PUBLIC void SongPal_Init(void);

#endif //__SONGPAL_INTERFACE_H__
