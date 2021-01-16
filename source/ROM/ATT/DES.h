#ifdef AIR_UPDATE_ENABLE

#ifndef _DES_H_
#define _DES_H_

PUBLIC void DES_Encrypt(U8 GENERIC_PTR Plain, U8 GENERIC_PTR Cipher, U8 GENERIC_PTR key);
PUBLIC void DES_BlockXor(U8 GENERIC_PTR Input1, U8 GENERIC_PTR Input2, U8 GENERIC_PTR Output);
PUBLIC void DES_KeyLeftShift(U8 GENERIC_PTR key_old,U8 GENERIC_PTR key_new, U8 round);
PUBLIC void DES_FeistelFunc(U8 GENERIC_PTR input, U8 GENERIC_PTR output, U8 GENERIC_PTR subkey);

#define DES_TABLE_IP	0x00		// initial permutation
#define DES_TABLE_FP	0x01		// final permutation
#define DES_TABLE_PC1	0x02		// permutation choice 1
#define DES_TABLE_PC2	0x03		// permutation choice 2
#define DES_TABLE_P		0x04		// permutation

//DES Macro
#define SBOX_EXPAND(i,e) S_Box[i/6] \
							  [(DES_GET_BIT(e+((i+0)/8),((i+0)%8)) << 1)| \
							   (DES_GET_BIT(e+((i+5)/8),((i+5)%8)) << 0)] \
							  [(DES_GET_BIT(e+((i+1)/8),((i+1)%8)) << 3)| \
							   (DES_GET_BIT(e+((i+2)/8),((i+2)%8)) << 2)| \
							   (DES_GET_BIT(e+((i+3)/8),((i+3)%8)) << 1)| \
							   (DES_GET_BIT(e+((i+4)/8),((i+4)%8)) << 0)]


#define DES_SET_BIT(A,B)	((A)[0]  |=  ((U8)0x01<<(7-(U8)(B))))
#define DES_CLR_BIT(A,B)	((A)[0]  &= ~((U8)0x01<<(7-(U8)(B))))
#define DES_GET_BIT(A,B) 	(U8)(((U8)(A)[0]>>(U8)(7-(B)))&(U8)0x01) 


#define DES_ASSIGN_BIT(A,B,Value) 	do { \
										DES_CLR_BIT(A,B); \
										if(Value){ \
											DES_SET_BIT(A,B); \
										} \
									}while(0)
#endif
#endif

