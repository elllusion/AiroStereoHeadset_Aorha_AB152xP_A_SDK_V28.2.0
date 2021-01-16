#include "Pedometer.h"
#ifdef SUPPORTED_PEDOMETER

/* XL362 COMMANDS */
#define XL362_CMD_W_REG         0x0A
#define XL362_CMD_R_REG         0x0B
#define XL362_CMD_R_FIFO        0x0D

/* REGISTER NAME*/
#define XL362_REG_DEVID_AD        0x00
#define XL362_REG_DEVID_MST       0x01
#define XL362_REG_PARTID          0x02
#define XL362_REG_REVID           0x03
#define XL362_REG_XDATA           0x08
#define XL362_REG_YDATA           0x09
#define XL362_REG_ZDATA           0x0A
#define XL362_REG_STATUS          0x0B
#define XL362_REG_FIFO_ENTRIES_L  0x0C
#define XL362_REG_FIFO_ENTRIES_H  0x0D
#define XL362_REG_XDATA_L         0x0E
#define XL362_REG_XDATA_H         0x0F
#define XL362_REG_YDATA_L         0x10
#define XL362_REG_YDATA_H         0x11
#define XL362_REG_ZDATA_L         0x12
#define XL362_REG_ZDATA_H         0x13
#define XL362_REG_TEMP_L          0x14
#define XL362_REG_TEMP_H          0x15
#define XL362_REG_SOFT_RESET      0x1F
#define XL362_REG_THRESH_ACT_L    0x20
#define XL362_REG_THRESH_ACT_H    0x21
#define XL362_REG_TIME_ACT        0x22
#define XL362_REG_THRESH_INACT_L  0x23
#define XL362_REG_THRESH_INACT_H  0x24
#define XL362_REG_TIME_ACT_L      0x25
#define XL362_REG_TIME_ACT_H      0x26
#define XL362_REG_ACT_INACT_CTL   0x27
#define XL362_REG_FIFO_CONTROL    0x28
#define XL362_REG_FIFO_SAMPLES    0x29
#define XL362_REG_INTMAP1         0x2A
#define XL362_REG_INTMAP2         0x2B
#define XL362_REG_FILTER_CTL      0x2C
#define XL362_REG_POWER_CTL       0x2D
#define XL362_REG_SELF_TEST       0x2E

/* XL362_REG_SOFT_RESET     0x1F */
#define XL362_REG_SOFT_RESET_KEY    0x52

/* XL362_REG_STATUS         0x0B */
#define XL362_STATUS_DATA_READY     0x01
#define XL362_STATUS_FIFO_READY     0x02
#define XL362_STATUS_FIFO_WATERMARK 0x04
#define XL362_STATUS_FIFO_OVERRUN   0x08
#define XL362_STATUS_ACT            0x10
#define XL362_STATUS_INACT          0x20
#define XL362_STATUS_AWAKE          0x40
#define XL362_STATUS_ERR_USER_REGS  0x80

/* XL362_REG_ACT_INACT_CTL  0x27 */
#define XL362_ACT_DISABLE       0x00
#define XL362_ACT_ENABLE        0x01
#define XL362_ACT_REFERENCE     0x02
#define XL362_INACT_ENABLE      0x04
#define XL362_INACT_REFERENCE   0x08
#define XL362_ACT_INACT_LINK    0x10
#define XL362_ACT_INACT_LOOP    0x30

/* XL362_REG_FIFO_CONTROL   0x28 */
#define XL362_FIFO_MODE_DISABLE         0x00    //FIFO Mode
#define XL362_FIFO_MODE_OLDEST_SAVED    0x01
#define XL362_FIFO_MODE_STREAM          0x02
#define XL362_FIFO_MODE_TRIGGERED       0x03

#define XL362_FIFO_MODE_AH_0            0
#define XL362_FIFO_MODE_AH_1            0x08
/* XL362_REG_FILTER_CTL     0x2C */
#define XL362_DATA_RATE_12_5        0x00    //Output Data Rate
#define XL362_DATA_RATE_25          0x01
#define XL362_DATA_RATE_50          0x02
#define XL362_DATA_RATE_100         0x03
#define XL362_DATA_RATE_200         0x04
#define XL362_DATA_RATE_400         0x05

#define XL362_MEASURE_RANGE_2G      0x00    //Measurement Range
#define XL362_MEASURE_RANGE_4G      0x40
#define XL362_MEASURE_RANGE_8G      0x80

/* XL362_REG_INTMAP         0x2A, 0x2B */
#define XL362_INTMAP_DATA_READY         0x01
#define XL362_INTMAP_FIFO_READY         0x02
#define XL362_INTMAP_FIFO_WATERMARK     0x04
#define XL362_INTMAP_FIFO_OVERRUN       0x08
#define XL362_INTMAP_ACT                0x10
#define XL362_INTMAP_INACT              0x20
#define XL362_INTMAP_AWAKE              0x40
#define XL362_INTMAP_INT_LOW            0x80

/* XL362_REG_POWER_CTL      0x2D */
#define XL362_POWER_CTL_STANDBY         0x00
#define XL362_POWER_CTL_MEASURE         0x02
#define XL362_POWER_CTL_AUTOSLEEP       0x04
#define XL362_POWER_CTL_WAKEUP          0x08

typedef struct 
{
    U8 CMD;         /* XL362 COMMANDS */
    U8 RegName;     /* REGISTER NAME*/
    U8 Data[1];
}XL362_WRITE_MULTI_DATA_t;

U8 xl362_ReadReg(U8 regName);
void xl362_ReadMultiReg(U8 regName, U8 XDATA_PTR dataPtr, U8 rxLen);
void xl362_ReadMultiFIFO(U8 XDATA_PTR dataPtr, U8 rxLen);
void xl362_WriteReg(U8 regName, U8 regData);
void xl362_WriteMultiReg(XL362_WRITE_MULTI_DATA_t XDATA_PTR ptr, U8 txLen);

#endif /* SUPPORTED_PEDOMETER */


