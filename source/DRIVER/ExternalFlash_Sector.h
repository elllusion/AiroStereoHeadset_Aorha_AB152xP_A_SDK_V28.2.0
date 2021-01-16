#ifndef _EXT_FLASH_SECTOR_H_
#define _EXT_FLASH_SECTOR_H_

#include "config.h"

#if (HW_SPI_SUPPORTED)

typedef struct
{
	U8  sectorID;
	U32	sectorLength;
	U32 sectorAddress;
	U8  isCRCNeeded;
}SECTOR_SCRIPT;

#define EXTERNAL_FLASH_SECTOR_VOICE_PROMPT_DATA_ID		0x01
#define EXTERNAL_FLASH_SECTOR_FRAME_ID					0x02
#define EXTERNAL_FLASH_SECTOR_SONG_DECSRPITOR_ID		0x03

#include "drv_spi_flash.h"
#include "drv_spiflash.h"


//////////////////// EXTERNAL_FLASH_SECTOR_SONG_DECSRPITOR_ID  /////////////////

#define EXT_FLASH_SONG_CODEC_SBC	0x03
#define EXT_FLASH_SONG_CODEC_AAC	0x02
typedef struct
{
	U8 codecType;	//AAC_TYPE: 0x02; SBC_TYPE:0x03
	U8 sampleFreq;	//48KHz: 0x01; 44.1KHz: 0x02; 32KHz: 0x03 
	U16 frameSize;
	U32 startAddress;
	U32 numOfFrame;
}SONG_DESCRIPTOR;

 
typedef struct
{
	U8 songNum;
	SONG_DESCRIPTOR songDescript[1];
}SONG_DESCRIPTOR_SECTOR;

#endif

#endif /* _EXT_FLASH_SECTOR_H_ */