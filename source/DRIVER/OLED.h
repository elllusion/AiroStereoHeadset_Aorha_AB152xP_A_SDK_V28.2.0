//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _OLED_H_
#define _OLED_H_

enum 
{
	OLED_PAGE0,
	OLED_PAGE1,
	OLED_PAGE2,
	OLED_PAGE3,
	OLED_TOTAL_PAGE,
};

enum 
{
	OLED_SHOW_NONE,
	OLED_SHOW_START,
};


#define OLED_PAGE_LENGTH			128
#define OLED_WORD_LENGTH 			8


PUBLIC void OLED_DisplayOn(U8 showType);
PUBLIC void OLED_DisplayOff(void);
PUBLIC void OLED_ConnectionShow(U8 XDATA_PTR bdAddr_display, U8 XDATA_PTR name_display, U8 rssi_display);
PUBLIC void OLED_DisconnectionShow(void);
PUBLIC void OLED_CheckTimer(void);

#endif //_OLED_H_
