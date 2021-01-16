#ifndef _PEDOMETER_H_
#define _PEDOMETER_H_

/* To enable the pedometer related codes, uncomment the following #define. */
//#define SDK_SUPPORTED_PEDOMETER

#ifdef SDK_SUPPORTED_PEDOMETER

#define DBG_ENABLE_PEDOMETER    (FALSE)
#define PEDOMETER_FIFO_MODE     (TRUE)  /* TRUE: FIFO_MODE, FALSE: REGISTER_MODE */
#define PEDOMETER_TEST_ENABLE   (FALSE)


/* Output_Rate (Hz)*/
#define PEDOMETER_DATA_RATE_12_5    0
#define PEDOMETER_DATA_RATE_25      1
#define PEDOMETER_DATA_RATE_50      2
#define PEDOMETER_DATA_RATE_100     3
#define PEDOMETER_DATA_RATE_200     4
#define PEDOMETER_DATA_RATE_400     5

/* Measurement_Range */
#define PEDOMETER_MEASURE_RANGE_2G  0
#define PEDOMETER_MEASURE_RANGE_4G  1
#define PEDOMETER_MEASURE_RANGE_8G  2

VOID Pedometer_SetFilterCtl(U8 rate, U8 range);     /* Measurement_Range, Output_Rate */
VOID Pedometer_ResetSteps(VOID);
U16 Pedometer_ReadCurrentSteps(VOID);
VOID Pedometer_StartMeasurement(VOID);
VOID Pedometer_StopMeasurement(VOID);
VOID Pedometer_Init(VOID);
VOID Pedometer(VOID);

#endif  /* SDK_SUPPORTED_PEDOMETER */
#endif  /* _PEDOMETER_H_ */
