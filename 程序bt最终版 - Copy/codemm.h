#ifndef _codemm_H_
#define _codemm_H_
#include<reg52.h>
#include "i2c.h"
#include "reg52.h"
#include "codemm.h"
#include <intrins.h>
#include <stdlib.h>

#define RAND_BIT	6 //Ëæ»úÂëÎ»Êı
 


#ifndef u8
#define u8 unsigned char
#endif 
#ifndef u16
#define u16 unsigned int
#endif 



void codegenerate(u8 array_temp[],u8 rand_bit,u8 feedseed);
void codesend();
void codesave(u8 dtpw[]);
void my_rand();
int cyphernum(u8 feedseed);

#endif