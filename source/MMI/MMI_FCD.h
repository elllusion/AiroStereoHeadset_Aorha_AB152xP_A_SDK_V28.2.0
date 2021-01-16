
#ifndef __MMI_FCD_H__
#define __MMI_FCD_H__

#ifdef __MMI_FCD_C__
	#define FCD_EXTERN
#else
	#define FCD_EXTERN extern
#endif

#ifdef FAST_CONNECT_DONGLE
PUBLIC void MMI_FCD_ReportDevAddr(U8 XDATA_PTR bdAddr);
PUBLIC void MMI_FCD_ReportDevName(U8 XDATA_PTR name, U8 maxLen);
PUBLIC void MMI_FCD_ReportDevRSSI(U8 rssi);
PUBLIC void MMI_FCD_ReportAction(U8 opCode);
PUBLIC void MMI_FCD_ReportVolume(U8 opCode, U8 vol);

PUBLIC void MMI_FCD_SendInquiry(void);
PUBLIC void MMI_FCD_GetInquiryComplete(void);
PUBLIC void MMI_FCD_GetInquiryResultWithRSSI(U8 rssi, U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR codPtr);
#endif

FCD_EXTERN struct
{
	U8 found_rssi;
	BD_ADDR found_bd_addr;
	U8 found_classOfDev[3];
	U8 audioBistLinkIndex;
	U8 remoteAudioBist:1;
	U8 reserved:7;
	U8 audioBistSNP;
	U8 audioBistSNDR;
	OST XDATA_PTR remote_audioBist_timer;
} XDATA gMMI_FCD_ctl;

PUBLIC void MMI_FCD_CmdEvtHandler(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void MMI_FCD_AudioBistResult();
PUBLIC void MMI_FCD_AudioBistControl();

enum
{
	FCD_CMD,
	FCD_EVT,
}; //MMI_FCD_STRU.packetType

enum
{
	FCD_CMD_BLEBDADDR_REQUEST,
	FCD_CMD_AUDIOBIST,
}; //MMI_FCD_BDADDR_STRU.cmdEvt

enum
{
	FCD_NO_ERROR,
	FCD_NOT_SUPPORT_BLE,
}; //MMI_FCD_BDADDR_STRU.errorCode

typedef struct
{
	U8 packetType; //0: Cmd, 1: Evt
	U8 cmdEvt;
	U8 errorCode; //0: pass, other: error reason
} MMI_FCD_STRU;

typedef struct
{
	MMI_FCD_STRU fcdHeader;
	BD_ADDR bdAddr;
} MMI_FCD_BDADDR_STRU;

typedef struct
{
	MMI_FCD_STRU fcdHeader;
	U8 audioBistMode;
	U8 audioIn;
	U8 audioOut;
	U8 signalPower;
	U8 SNDR;
}MMI_FCD_REMOTE_AUDIOBIST_STRU;

#endif //__MMI_FCD_H__
