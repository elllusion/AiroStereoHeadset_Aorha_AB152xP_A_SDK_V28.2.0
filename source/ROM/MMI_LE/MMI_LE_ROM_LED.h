#ifndef _MMI_LE_ROM_LED_H_
#define _MMI_LE_ROM_LED_H_

typedef struct
{
    U8 Reg02_t1t2rpt;             
    U8 Reg03_unit;               
    U8 Reg04_t0;                 
    U8 Reg05_t1;    
    U8 Reg06_t2;    
    U8 Reg07_t3;    
    U8 Reg08_OnStep;    
    U8 Reg09_Offstep;    
    U8 Reg10_Duration;    
}LED_SETTING_STRU;


EXTERN LED_SETTING_STRU CODE LED_AU_Mode;
EXTERN LED_SETTING_STRU CODE LED_LOADER_Mode;
EXTERN LED_SETTING_STRU CODE LED_Low_Battery;
EXTERN LED_SETTING_STRU CODE LED_EngineeringMode;

#endif