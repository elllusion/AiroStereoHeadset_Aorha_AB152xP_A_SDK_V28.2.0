#include "..\SDK\include\IIC.h"
#include "..\MMI\MMI_Base.h"
#include "drivers.h"
#include "..\ABSDRIVER\drv_gpio_flash.h"
#include "..\SECTOR\Config1\sector_config1.h"

#define single_seek				0
#define ext_clk					1
#define reffrence_ac_couple		1



#define UEBAND					0x0000 // US & Europe BAND 87.5MHz to 108MHz

#define SPACE100K				1

#define ADDR_STATUS				0x13 // the address of status register
#define MASK_STC 				0x0020  // Seek/Tune/PowerOn complete  D5 in  adress 13H 
#define MASK_SF	 				0x0010  //  Seek Fail D4 in address 13H
#define MASK_READCHAN 			0xFF80 // D7~D15 in address 13H
#define SHIFT_READCHAN 			7

#define FM_SEEK_UP	0x0001
#define FM_SEEK_DN	0x0000

#define I2C_FM_ADDR				0x20
#define AR1000_WriteByte(A, B) 	SDK_IIC_WriteU16_400K(I2C_FM_ADDR, (A), (B))
#define AR1000_ReadByte(A)		SDK_IIC_ReadU16_400K(I2C_FM_ADDR, (A))


#if ext_clk
	PRIVATE U16 CODE AR1000reg[18]={ //  external clock
		0xFF7B, // R0
		0x5B17, 
		0xD4B9, 
		0xA00A,
		0x0780, 
		0x28AB, // R5
		0x6400,
		0x1EE7, 
		0x7141, 
		0x0080, 
		0x81CE,// R10 wrap freq
		0x4F55, 
		0x970C, 
		0xB845, 
	#if single_seek
		0xFE2D, // R14
	#else
		0xFC2D, // R14
	#endif
		0x8097, 
		0x04A1, 
		0xDF6A  
	}; 
#else     
	PRIVATE U16 CODE AR1000reg[18]={ //crystal  
		0xFFFB, // R0
		0x5B17, 
		0xD0B9, 
		0xA00A, 
		0x0780, 
		0x28AB, // R5
		0x6400, 
		0x1EE7, 
		0x7141,
		0x007D, 
		0x82CE, // R10 enable seek wrap
		0x4E55, 
		0x970C,
		0xB845, 
	#if single_seek
		0xFE2D, // R14
	#else
		0xFC2D, // R14
	#endif
		0x8097, // R15
		0x04A1, 
		0xDF6A 
	}; 
#endif

PRIVATE U8 CODE AR1000vol[16]={ // volume control
	 0x0F,	// step 0
	 0x10,	
	 0x20,
	 0x30,	
	 0x40,
	 0x50,	// 5
	 0x60,
	 0x70,
	 0x80,
	 0x90,	
	 0xA0,	//10
	 0xB0,	
	 0xC0,
	 0xD0,	
	 0xE0,	
	 0xF0	//15 
};


PRIVATE void AR1000_GetReadCHAN(void)
{
	U16 dataRead;
	U16 CHAN;
	
	dataRead = AR1000_ReadByte(ADDR_STATUS);
	CHAN = ((dataRead & MASK_READCHAN )>> SHIFT_READCHAN)&0x01ff;
	dataRead = AR1000_ReadByte(2);
	AR1000_WriteByte(2, (dataRead&0xFE00)|CHAN); 
}

PRIVATE void AR1000_TUNE_ON_OFF(BOOL ON)
{
	U16 dataRead;
	
	dataRead = AR1000_ReadByte(2);
	if (ON)
		AR1000_WriteByte(2, (dataRead&0xFDFF)|0x200);
	else
		AR1000_WriteByte(2, (dataRead&0xFDFF));
}

PRIVATE void AR1000_SEEK_ON_OFF(BOOL ON)
{
	U16 dataRead;
	
	dataRead = AR1000_ReadByte(3);
	if (ON)
		AR1000_WriteByte(3, (dataRead&0xBFFF)|0x4000);
	else
		AR1000_WriteByte(3, (dataRead&0xBFFF));
}

PRIVATE BOOL AR1000_CheckSTCIsBusy(void)
{
	U16 dataRead;
	dataRead = AR1000_ReadByte(ADDR_STATUS);
	if ((dataRead & MASK_STC)==0)  // Wait STC flag.
	{
		return TRUE;
	}
	return FALSE;  
}

PUBLIC void AR1000_FM_Mode(void)
{
	if(MMI_IsSpeakerProduct())
	{
		if (gMMI_nvram.gpioAmpBtFm < 4 || gMMI_nvram.gpioAmpBtFm > 28 )
		{
			return;
		}
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, gMMI_nvram.gpioAmpBtFm);
	}
}

PUBLIC void AR1000_BT_Mode(void)
{
	if(MMI_IsSpeakerProduct())
	{
		if (gMMI_nvram.gpioAmpBtFm < 4 || gMMI_nvram.gpioAmpBtFm > 28 )
		{
			return;
		}

		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, gMMI_nvram.gpioAmpBtFm);
	}
}

PUBLIC BOOL AR1000_Mute(U8 mute)
{
	U16 dataRead;
	
	if(AR1000_CheckSTCIsBusy())
		return FALSE; 

	dataRead = AR1000_ReadByte(1) & 0xFFFD;
	if (mute)
		dataRead |= 0x02;

	AR1000_WriteByte(1, dataRead);  
	return TRUE;
}

PUBLIC BOOL AR1000_SetVol(U8 vol)
{
	U16 dataRead;
	
	dataRead = AR1000_ReadByte(3);
	AR1000_WriteByte(3, (dataRead&0xF87F)|((U16)(AR1000vol[vol]&0x0F)<<7));
	
	dataRead = AR1000_ReadByte(14);
	AR1000_WriteByte(14, (dataRead&0x0FFF)|((U16)(AR1000vol[vol]&0xF0)<<8));
	return TRUE;
}



PUBLIC U16 AR1000_GetReadFREQ(void)
{
	U16 dataRead;
	U16 freq;

    dataRead = AR1000_ReadByte(ADDR_STATUS);
	freq = 690 +((dataRead & MASK_READCHAN )>> SHIFT_READCHAN) ;
	return freq;
}

PUBLIC BOOL AR1000_SetFreq( U16 curf ) /* input value: 875 - 1080 ( 87.5 MHz - 108.0 MHz)*/
{
	U16 dataRead;
	
	if(!AR1000_CheckSTCIsBusy())	     
	{
		AR1000_WriteByte(17, AR1000reg[17]);
				  
		AR1000_TUNE_ON_OFF(FALSE);	//TUNE OFF
		AR1000_SEEK_ON_OFF(FALSE);	//SEEK OFF
		
		curf -= 690;
		curf &= 0x01ff;
		curf |= 0x200;   //Set TUNE bit = 1
		dataRead = AR1000_ReadByte(2);
		AR1000_WriteByte(2, (dataRead&0xFC00)|curf); // here will Tune On.
		
		return TRUE;
	}	
	return FALSE;
}

PUBLIC BOOL AR1000_Seek(BOOL isUp)
{
	U16 dataRead;
	U16 val;
		
	if(!AR1000_CheckSTCIsBusy())
	{
		if(AR1000_Mute(1))
		{
			/*empty block, to prevent coverity warning*/
		}
			
		AR1000_GetReadCHAN();		 // Set start freq for SEEK.
		
		dataRead = AR1000_ReadByte(17);
		dataRead = (dataRead & 0xC3FF)|0x2000;
		AR1000_WriteByte(17, dataRead);
		////////////////////////////////////////////////////////
			
		AR1000_TUNE_ON_OFF(FALSE);	//TUNE OFF		
		AR1000_SEEK_ON_OFF(FALSE);	//SEEK OFF
		      	
		dataRead = AR1000_ReadByte(3);
		val = (isUp)?FM_SEEK_UP: FM_SEEK_DN;
		AR1000_WriteByte(3, ((dataRead&0x07FF)|(val<<15)|((U16)SPACE100K<<13)|UEBAND));//set space100k / band(875~1080) / SEEK up-down
		AR1000_SEEK_ON_OFF(TRUE);	//SEEK ON	
		
		return TRUE;
	}
	return FALSE;
}

#include "rc.h"
PUBLIC BOOL AR1000_PowerOn(U8 vol,U8 seekTH)
{
	U8 i;
	AR1000_WriteByte(0, 0xFF7A); //Set ENABLE bit = 0
	
	for (i=1; i<18; i++)
	{
		if(i == 3)
		{
			//set seek threshold value
			AR1000_WriteByte(3, (AR1000reg[3]&0xFF80)|(U16)seekTH);
			//set volume to the history souond level
			AR1000_WriteByte(3, (AR1000reg[3]&0xF87F)|((U16)(AR1000vol[vol]&0x0F)<<7));
		}
		else if(i == 14)
		{
			AR1000_WriteByte(14, (AR1000reg[14]&0x0FFF)|((U16)(AR1000vol[vol]&0xF0)<<8));	
		}
		#if reffrence_ac_couple
		else if(i == 16)
		{
			AR1000_WriteByte(16, 0x04E1);	//AC couple
		}
		#endif
		else
		{
			AR1000_WriteByte(i, AR1000reg[i]);	// Write R1~R17 default value into registers R1~R17
		}
	}
	AR1000_WriteByte(0, AR1000reg[0]);	//Write R0 default value for ENABLE AR1000
	return TRUE;
}

PUBLIC BOOL AR1000_PowerOff(void)
{
	//AR1000_WriteByte(6, 0x7400);  //Ear-HZ On  
	AR1000_SEEK_ON_OFF(FALSE);   //SEEK OFF	
	AR1000_WriteByte(0, 0xFF7A); // Set ENABLE bit = 0
	
	return TRUE;
}

PUBLIC void AR1000_Init(void)
{
	U8 gpioFMClock32k;
	gpioFMClock32k=((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->FMClock32KPio; 
	if (gpioFMClock32k >= 3 && gpioFMClock32k <=25)
	{
		DRV_GPIO_F_SetSigSel(gpioFMClock32k, CLK_SLOW);
		AR1000_PowerOff();
	}
}
