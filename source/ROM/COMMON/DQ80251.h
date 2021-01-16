#ifndef _DQ08251_H_
#define _DQ08251_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * DQ80251.h is the file defines the SFR of DQ80251
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */

/*  BYTE Registers  */
sfr P0      = 0x80;
sfr SP      = 0x81;
sfr16 DP    = 0x82;
sfr DPL     = 0x82;
sfr DPH     = 0x83;
sfr DPXL    = 0x84;
sfr DPS     = 0x86;
sfr PCON    = 0x87;
sfr TCON    = 0x88;
sfr TMOD    = 0x89;
sfr TL0     = 0x8A;
sfr TL1     = 0x8B;
sfr TH0     = 0x8C;
sfr TH1     = 0x8D;
sfr CKCON   = 0x8E;
sfr P1      = 0x90;
sfr WTST    = 0x92;
sfr P2      = 0xA0;
sfr IE      = 0xA8;
sfr P3      = 0xB0;
sfr IPH     = 0xB7;
sfr IPL     = 0xB8;
sfr SPH     = 0xBE;
sfr T2CON   = 0xC8;
sfr RLD     = 0xCA;
sfr RLDL    = 0xCA;
sfr RLDH    = 0xCB;
sfr16 T2    = 0xCC;
sfr TL2     = 0xCC;
sfr TH2     = 0xCD;
sfr PSW     = 0xD0;
sfr PSW1    = 0xD1;
sfr ACC     = 0xE0;
sfr STATUS  = 0xE9;
sfr MXAX    = 0xEA;
sfr TA      = 0xEB;
sfr B       = 0xF0;


/*  BIT Registers  */
/*  PSW   */
sbit CY     = PSW^7;
sbit AC     = PSW^6;
sbit F0     = PSW^5;
sbit RS1    = PSW^4;
sbit RS0    = PSW^3;
sbit OV     = PSW^2;
sbit UD     = PSW^1;
sbit P      = PSW^0;

/*  TCON  */
sbit TF1    = TCON^7;
sbit TR1    = TCON^6;
sbit TF0    = TCON^5;
sbit TR0    = TCON^4;
sbit IE1    = TCON^3;
sbit IT1    = TCON^2;
sbit IE0    = TCON^1;
sbit IT0    = TCON^0;

/*  T2CON */
sbit TF2    = T2CON^7;
sbit EXF2   = T2CON^6;
sbit EXEN2  = T2CON^3;
sbit TR2    = T2CON^2;
sbit CT2    = T2CON^1;
sbit CPRL2  = T2CON^0;

/*  IE    */
sbit EA     = IE^7;
sbit ET2    = IE^5;
sbit ET1    = IE^3;
sbit EX1    = IE^2;
sbit ET0    = IE^1;
sbit EX0    = IE^0;

/*  IPL   */
sbit PT2    = IPL^5;
sbit PT1    = IPL^3;
sbit PX1    = IPL^2;
sbit PT0    = IPL^1;
sbit PX0    = IPL^0;

/* P0 */
sbit P0_0   = P0^0;
sbit P0_1   = P0^1;
sbit P0_2   = P0^2;
sbit P0_3   = P0^3;
sbit P0_4   = P0^4;
sbit P0_5   = P0^5;
sbit P0_6   = P0^6;
sbit P0_7   = P0^7;

/* P1 */
sbit P1_0   = P1^0;
sbit P1_1   = P1^1;
sbit P1_2   = P1^2;
sbit P1_3   = P1^3;
sbit P1_4   = P1^4;
sbit P1_5   = P1^5;
sbit P1_6   = P1^6;
sbit P1_7   = P1^7;

/* P2 */
sbit P2_0   = P2^0;
sbit P2_1   = P2^1;
sbit P2_2   = P2^2;
sbit P2_3   = P2^3;
sbit P2_4   = P2^4;
sbit P2_5   = P2^5;
sbit P2_6   = P2^6;
sbit P2_7   = P2^7;

/* P3 */
sbit P3_0   = P3^0;
sbit P3_1   = P3^1;
sbit P3_2   = P3^2;
sbit P3_3   = P3^3;
sbit P3_4   = P3^4;
sbit P3_5   = P3^5;
sbit P3_6   = P3^6;
sbit P3_7   = P3^7;

/* B */
sbit B_0    = B^0;
sbit B_1    = B^1;
sbit B_2    = B^2;
sbit B_3    = B^3;
sbit B_4    = B^4;
sbit B_5    = B^5;
sbit B_6    = B^6;
sbit B_7    = B^7;

#endif /* _DQ08251_H_ */

