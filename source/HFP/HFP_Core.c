//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _HFP_CORE_C_


#include "os.h"
#include "bt_config_profile.h"

#ifdef HFP_Profile
#include "..\MMI\MMI.h"
#include "..\MMI\MMI_HFP.h"
#include "..\RFCOMM\RFCOMM_interface.h"
#include "..\HC\HC_interface.h"

#include "HFP_interface.h"
#include "HFP.h"

PUBLIC U8 XDATA_PTR HFP_FindToken (U8 XDATA_PTR ptr1, U8 token)
{
	while( *ptr1 != AT_CR && *ptr1 != token )
	{	
		ptr1++;
	}
	return ptr1;	
}

PUBLIC U8 XDATA_PTR HFP_FindComma (U8 XDATA_PTR ptr1)
{
	return HFP_FindToken(ptr1, ',');
}

PUBLIC U8 XDATA_PTR HFP_FindColon (U8 XDATA_PTR ptr1)
{
	return HFP_FindToken(ptr1, ':');
}

PUBLIC U8 XDATA_PTR HFP_U16_To_ASCII(U8 XDATA_PTR dst, U16 number)
{
	U8 XDATA_PTR src;
	U8 LDATA tmp_data_to_ascii[5]; //reverse	
	
	OSMEM_memset_xdata((U8 XDATA_PTR)tmp_data_to_ascii, 0, sizeof(tmp_data_to_ascii));	
	
	//src: tmp_number_data
	src = (U8 XDATA_PTR)tmp_data_to_ascii;				 
	
	//Generate Decimal array, zero then skip					
	do
	{
		*src++ = number % 10;
		number = number / 10; 
	} while( number );

	//Decimal to ASCII, copy data to code array	
	while( src != (U8 XDATA_PTR)tmp_data_to_ascii )	
	{
		*dst++ = HFP_DIGIT_TO_CHAR(*(--src));
	}
	
	return dst;
}

PUBLIC U16 HFP_ASCII_TO_U16 (U8 XDATA_PTR ptr1)
{	
	U16 value = 0;

	while( *ptr1 != AT_CR && *ptr1 != ',' && *ptr1 != ')' )
	{	
		value = value * 10 + HFP_CHAR_TO_DIGIT(*ptr1);
		ptr1++;
	}

	return value;
}

//Modified by Eric Chou, Vanessa Kuo, 2011.3.17
//For IVT 2.7.0.35, ignore '-' & '_' in result code.
PUBLIC U8 AT_CompareString (U8 XDATA_PTR strPtr, U8 CODE_PTR c_ptr, U8 length)
{
	while (length-- && *c_ptr != 0x00)
	{
		switch (*strPtr)
		{
			case '_':
			case '-':
				strPtr++;
				break;

			default:
				if (*strPtr != *c_ptr) 
				{
				 	return AT_EOF;
				}
				else
				{
				   	strPtr++;
					c_ptr++;	
				}
				break;
		}
	}
	return 0;
}

PUBLIC void AT_UpperCaseAndEliminateSpace (U8 XDATA_PTR src, U16 length)
{
	U8 XDATA_PTR dst = src;
	U8 sep = 0;
	
	//Eliminate Space and transfer to Upper Case
	while (length)
	{
		if (sep == 0)
		{
			while (length)
			{
				length--; //do not the location of this statement
				if (*src == '"')
				{
					sep = *src;
					*dst++ = *src++;
					break;
				}
				else if (*src == ' ')
				{
					src++; //discard
				}
				else if (*src >= 'a' && *src <= 'z')
				{
					*dst++ = *src++ - ('a' - 'A');
				}
				else
				{
					*dst++ = *src++;
				}
			}
		}
		else
		{
			while (length)
			{
				length--; //do not the location of this statement
				if (*src == sep)
				{
					sep = 0;
					*dst++ = *src++;
					break;
				}
				else
				{
					*dst++ = *src++;
				}
			}
		}
	}
}

#ifdef HFP_AG_Profile
  #include "HFP_Core_AG.c"
#else
  #include "HFP_Core_HF.c"
#endif

#endif //HFP_Profile
