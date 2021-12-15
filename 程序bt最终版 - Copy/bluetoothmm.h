#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include<reg52.h>
#include<intrins.h>

#ifndef u8
#define u8 unsigned char
#endif 
#ifndef u16
#define u16 unsigned int
#endif 


void UsartConfiguration();
void Delay_1ms(u16 i);
void passwordRD();


sbit LED=P2^3;




#endif