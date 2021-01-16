#ifndef _DRV_I2S_H_
#define _DRV_I2S_H_

#define I2S_HW_GRP0		0
#define I2S_HW_GRP2		1

PUBLIC void DRV_I2S_EnableI2SHWPins(U8 i2sGrp, BOOL isMaster);
PUBLIC void DRV_I2S_DisableI2SHWPins(U8 i2sGrp);
PUBLIC void DRV_I2S_SetInHWPins(U8 dataInPio, U8 isMaster);

#endif //_DRV_I2S_H_
