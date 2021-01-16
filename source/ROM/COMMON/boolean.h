#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * boolean.h defines the Boolean value type definition.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */



////////////////////////////////////////////////////////////////////////////////
// Boolean Definitions /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef FALSE
#define FALSE (0)
#else
#error "Redefine FALSE."
#endif

#ifndef TRUE
#define TRUE (!(FALSE))
#else
#error "Redefine TRUE."
#endif

#endif /* _BOOLEAN_H_ */

