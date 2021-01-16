#ifndef _CPU_H_
#define _CPU_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * cpu.h defines the CPU depended header file
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include <intrins.h>
#include "boolean.h"



////////////////////////////////////////////////////////////////////////////////
// Tool Chain Setup ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if defined(__C51__)
#define COMPILER_IS_KEIL_C51                                                    TRUE
#define COMPILER_IS_KEIL_C251                                                   FALSE
#elif defined(__C251__)
#define COMPILER_IS_KEIL_C51                                                    FALSE
#define COMPILER_IS_KEIL_C251                                                   TRUE
#else
#error "Unsupported Compiler"
#endif



////////////////////////////////////////////////////////////////////////////////
// CPU depended type definitions ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* System Type Definition */
typedef bit                 BIT;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef signed char         S8;
typedef signed short        S16;
typedef signed long         S32;
typedef void                VOID;
typedef unsigned char       BOOL;
typedef unsigned int        SIZE;
typedef float               FLOAT;

/* Special Type Definition */
typedef U8                  ADDR8;
typedef U16                 ADDR16;
typedef U32                 ADDR24;



////////////////////////////////////////////////////////////////////////////////
// Compiler depended keywords //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-
#define FAR                 far
#define NEAR                near
#define VOID_NEAR           void near
/* Keil C Memory Type Specifier */
#if (COMPILER_IS_KEIL_C51) || (COMPILER_IS_KEIL_C251)
#define CODE                const far
#define CODE_PTR            const far *
#define DATA                data
#define DATA_PTR            data *
#define IDATA               idata
#define IDATA_PTR           idata *
#else
#define CODE                const
#define CODE_PTR            const *
#define DATA
#define DATA_PTR            *
#define IDATA
#define IDATA_PTR           *
#endif

#if (COMPILER_IS_KEIL_C51)
#define XDATA               xdata
#define XDATA_PTR           xdata *
#define PDATA               pdata
#define PDATA_PTR           pdata *
#elif (COMPILER_IS_KEIL_C251)
#define XDATA               edata
#define XDATA_PTR           edata *
#define PDATA               edata
#define PDATA_PTR           edata *
#else
#define XDATA
#define XDATA_PTR           *
#endif

#if (COMPILER_IS_KEIL_C251)
#define EDATA               edata
#define EDATA_PTR           edata *
#elif (COMPILER_IS_KEIL_C51)
#define EDATA               xdata
#define EDATA_PTR           xdata *
#else
#define EDATA
#define EDATA_PTR           *
#endif

#define GENERIC_PTR         *

/* Keil C Specific Type Definitions */
#if (COMPILER_IS_KEIL_C251) || (COMPILER_IS_KEIL_C51)
#define AT(a)               _at_(a)
#define REENTRANT           reentrant
#define INTERRUPT(n)        interrupt n
#define USING(n)            using n
#else
#define AT(A)
#define REENTRANT
#define INTERRUPT(n)
#define USING(n)
#endif

/* Keil C51/251 Specific Memory Model */
#if (COMPILER_IS_KEIL_C251)
/* small large are avaliable */
#define compact
#elif (COMPILER_IS_KEIL_C51)
/* small compact large are avaliable */
#else
#define small
#define compact
#define large
#endif



////////////////////////////////////////////////////////////////////////////////
// Compiler depended attributes ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if (COMPILER_IS_KEIL_C251)
#define ALIGN(n) __attribute__((aligned(n)))
#elif (COMPILER_IS_KEIL_C51)
#error "UNDEFINED CASE"
#else
#error "UNDEFINED CASE"
#endif



////////////////////////////////////////////////////////////////////////////////
// Utilities ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * NULL
 */
//- TINY_NULL - 1 byte null pointer
#define TINY_NULL   ((VOID data *)0)
//- NEAR_NULL - 2 byte null pointer
#define NEAR_NULL   ((VOID near *)0)
//- FAR_NULL - 4 byte null pointer
#define FAR_NULL    ((VOID far *)0)
//- NULL
#if !defined(NULL)
#define NULL NEAR_NULL
#endif

/**
 * Keywords and tags
 */
#define PUBLIC
#define PRIVATE static
#define EXTERN extern
#define STATIC static
#define CONST const
#define VOLATILE volatile
//- Input Tag
#define IN
//- Output Tag
#define OUT
//- Optional Input Tag : A Optional Input could input NULL if you don't specify a value
#define IN_OPT

/**
 * Optimization
 */
#define MOD_BY_8(a) ((a) & 7)
#define DIV_BY_8(a) ((a) >> 3)

/**
 * Byte macro
 */
#define BYTE0(N)            ((U8)((N) >>  0))
#define BYTE1(N)            ((U8)((N) >>  8))
#define BYTE2(N)            ((U8)((N) >> 16))
#define BYTE3(N)            ((U8)((N) >> 24))

/**
 * Unused macro
 */
#define UNUSED(x) { do { if(x){}; } while (0); }

/**
 * Bit mask
 */
#define BIT_MASK(n)         (1UL << (n))
#define BIT_MASK32(n)       ((U32)1 << (n))
#define BIT_MASK16(n)       ((U16)1 << (n))
#define BIT_MASK8(n)        ((U8)1 << (n))
#define BIT_FIELD(n,p)      ((n) << (p))

/**
 * Bit field operating
 */
#define BIT_FIELD_FILTER8(r,p,l)    (((U8)(r))  & ((((U8)1  << (l)) - 1) << (p)))
#define BIT_FIELD_FILTER16(r,p,l)   (((U16)(r)) & ((((U16)1 << (l)) - 1) << (p)))
#define BIT_FIELD_FILTER32(r,p,l)   (((U32)(r)) & ((((U32)1 << (l)) - 1) << (p)))
#define BIT_FIELD_CLEAR8(r,p,l)     (((U8)(r))  & ~((((U8)1  << (l)) - 1) << (p)))
#define BIT_FIELD_CLEAR16(r,p,l)    (((U16)(r)) & ~((((U16)1 << (l)) - 1) << (p)))
#define BIT_FIELD_CLEAR32(r,p,l)    (((U32)(r)) & ~((((U32)1 << (l)) - 1) << (p)))
#define BIT_FIELD_EXTRACT8(r,p,l)   (((U8)(r)  >> (p)) & (((U8)1  << (l)) - 1))
#define BIT_FIELD_EXTRACT16(r,p,l)  (((U16)(r) >> (p)) & (((U16)1 << (l)) - 1))
#define BIT_FIELD_EXTRACT32(r,p,l)  (((U32)(r) >> (p)) & (((U32)1 << (l)) - 1))
#define BIT_FIELD_INSERT8(r,p,l,v)  (BIT_FIELD_CLEAR8((r),(p),(l))  | ((U8)(v)  << (p)))
#define BIT_FIELD_INSERT16(r,p,l,v) (BIT_FIELD_CLEAR16((r),(p),(l)) | ((U16)(v) << (p)))
#define BIT_FIELD_INSERT32(r,p,l,v) (BIT_FIELD_CLEAR32((r),(p),(l)) | ((U32)(v) << (p)))

#define FIX_POINT_ROUNDING8(v,p)    (((U8)(v)  + ((U8)(v)  & ((U8)1  << (p-1)))) >> p)
#define FIX_POINT_ROUNDING16(v,p)   (((U16)(v) + ((U16)(v) & ((U16)1 << (p-1)))) >> p)
#define FIX_POINT_ROUNDING32(v,p)   (((U32)(v) + ((U32)(v) & ((U32)1 << (p-1)))) >> p)

////////////////////////////////////////////////////////////////////////////////
// Special type definitions ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_byte
{
    U8 Bit0 : 1;
    U8 Bit1 : 1;
    U8 Bit2 : 1;
    U8 Bit3 : 1;
    U8 Bit4 : 1;
    U8 Bit5 : 1;
    U8 Bit6 : 1;
    U8 Bit7 : 1;

} BYTE_STRU;


typedef union
{
    U16 value;
    struct
    {
        U8 B1;
        U8 B0;
    } in_byte;
} TWO_BYTE_UNION_TYPE;//BIg Endian Version


typedef union
{
    U32 value;
    struct
    {
        U8 B3;
        U8 B2;
        U8 B1;
        U8 B0;
    } in_byte;

} FOUR_BYTE_UNION_TYPE;//BIg Endian Version


#define CAST_TO_U8_PTR(p) ((U8 XDATA_PTR)(p))
#define GET_DATA_PTR(p) ((VOID NEAR *)(CAST_TO_U8_PTR(p) + *CAST_TO_U8_PTR(p)))
#define SET_MSG_DATA_OFFSET(p,o) { do { CAST_TO_U8_PTR(p)[0] = (o); } while (0); }



////////////////////////////////////////////////////////////////////////////////
// Legacy Definitons ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TODO : remove
#if defined __C51__
#define LDATA               XDATA
#else
#define LDATA
#endif
#define LDATA_PTR			XDATA_PTR

/* TODO : Need to remove */
#ifdef __C251__
    #define COVERITY_PTR
#else
    #define COVERITY_PTR            (U32)
#endif

#ifdef __C251__
	#define U8_XDATA_PTR_PTR_CASTING
#else
	#define U8_XDATA_PTR_PTR_CASTING     (U8 XDATA_PTR XDATA_PTR)
#endif

/* TODO : Need to remove */
#define U32_BIT_MASK(n)                     BIT_MASK32(n)
#define U16_BIT_MASK(n)                     BIT_MASK16(n)
#define U8_BIT_MASK(n)                      BIT_MASK8(n)

#endif /* _CPU_H_ */

