#ifdef SUPPORT_LE_XPORT
#include "Service_LE_xport.h"
LE_XPORT_T XDATA gleXport =
{

    /* TxIdx */
    0,
    /* TX_CCCD */
    {0x00, 0x00},
    /* RxIdx */
    0,
    /* connPara */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* airPatch */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* airPatch_CCCD */
    {0x00, 0x00},

#if 0 //LE_XPORT_SPECIFIC_SERVICE_ENABLE
    /* specific_TX */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* specific_TX_CCCD */
    {0x00, 0x00},
    /* specific_RX */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
#endif

    /* modelNum */
    {'A' ,'I' ,'R' ,'O' ,'H' ,'A' ,'_' ,'B' ,'L' ,'E' ,'.' ,'0' ,'0' ,'0' ,'0' ,'1'},
#if 0 //LE_XPORT_DIS_SERIAL_NUM_ENABLE
    /* serialNum */
    {'A' ,'I' ,'R' ,'O' ,'H' ,'A' ,'_' ,'B' ,'L' ,'E' ,'.' ,'0' ,'0' ,'0' ,'0' ,'2'},
#endif
    /* manuName */
    {'A' ,'I' ,'R' ,'O' ,'H' ,'A' ,'_' ,'B' ,'L' ,'E' ,'.' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'3'},
    /* softRV */
    {'0' ,'0' ,'0' ,'4'},
    /* systemID */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05},
#if 0 //LE_XPORT_DIS_C_VERSION_ENABLE
    /* hardRV */
    {'0' ,'0' ,'1'},
    /* firmRV */
    {'0' ,'0' ,'2'},
#endif
#if 0 //LE_XPORT_DIS_SPECIFIC_ENABLE
    /* s_DIS_1 */
    {0x11, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06},
    /* s_DIS_2 */
    {0x22, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08},
#endif

};
#endif

