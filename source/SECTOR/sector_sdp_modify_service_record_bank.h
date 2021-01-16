#ifndef _STRUCT_SDP_MODIFY_SERVICE_RECORD_BANK_H_
#define _STRUCT_SDP_MODIFY_SERVICE_RECORD_BANK_H_

#ifdef HID_SOC
typedef struct
{
	//Data format:
	//[ServiceCount:1]
	//[Service Record Handle:4][Attribute Count:1]
	//[Atributes:N, each attribute has different length, and there are "Attribute Count" attributes.]
	//
	// More services if "ServiceCount" is larger than 1.
	//
	//([Service Record Handle:4][Attribute Count:1]
	// [Atributes:N, each attribute has different length, and there are "Attribute Count" attributes.]) * (N - 1)
	U8 serviceRecordBank[450];
} SDP_MODIFY_SERVICE_RECORD_BANK;
#else
typedef struct
{
	U8 serviceRecordBank[1];
} SDP_MODIFY_SERVICE_RECORD_BANK;
#endif

#endif
