#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"


#include "..\SDK\include\Key.h"
#include "..\SDK\include\Fm_Hal.h"
#include "..\SDK\include\IIC.h"
#include "..\SDK\include\Charger.h"
#include "..\SDK\include\Battery.h"
#include "..\SDK\include\System.h"
#include "..\SDK\include\Uart_Hal.h"

void BOSCH_BM250_Init()
{
    SDK_IIC_WriteU8_400K(0x30, 0x14, 0xB6);
    SDK_IIC_WriteU8_400K(0x30, 0x0F, 0x05);
    SDK_IIC_WriteU8_400K(0x30, 0x10, 0x0D);
}

int g_TempDelay = 0;
void BOSCH_BM250_PrintData()
{
    // for present display too fast we do some deplay here. 
    if(g_TempDelay++ >= 40)
    {
        S16 qdata[3];
        U8 i, rev_data[6];
        
        // According to BOSCH spec
        // x-axis y-axis z-axis data store on register address [0x02 0x03] [0x04 0x05] [0x06 0x07]
        for(i=0; i<6; i++)
        {
            rev_data[i] = SDK_IIC_ReadU8_400K(0x30, (U8)(0x02+i));
        }
        
        // According to BOSCH spec
        // x-asix data is 10 bit. merge it
        qdata[0] = rev_data[1];
        qdata[0] = qdata[0] << 8 + rev_data[0] ;
        qdata[0] = qdata[0] >> 6;

        // y-asix data is 10 bit. merge it 
        qdata[1] = rev_data[3];
        qdata[1] = qdata[1] << 8 + rev_data[2] ;
        qdata[1] = qdata[1] >> 6;

        // z-asix data is 10 bit. merge it 
        qdata[2] = rev_data[5];
        qdata[2] = qdata[2] << 8 + rev_data[4];
        qdata[2] = qdata[2] >> 6;
        
        LightDbgPrint("%h  %h  %h", (S16)qdata[0], (S16)qdata[1], (S16)qdata[2]);
        g_TempDelay= 0;
    }
}

void SDK_Init(void)
{
	SDK_Key_Init();
	SDK_FM_Init();
	SDK_IIC_Init();
	SDK_Charger_Init();
	SDK_Battery_Init();
	SDK_System_Init();
	SDK_Uart_HAL_Init();

    // initialize BOSCH BM250
    BOSCH_BM250_Init();
}


void SDK_Polling(void)
{
    // Keep display BM250 data
    BOSCH_BM250_PrintData();
}
