#include "reg52.h"
#include "codemm.h"
#include "i2c.h"
#include <intrins.h>
#include <stdlib.h>

#define RAND_BIT	6 //随机码位数




void codegenerate(u8 array_temp[],u8 rand_bit,u8 feedseed)
{	u8 i = 0;
	srand(feedseed); //随机数初始化
	for(i = 0;i < rand_bit;i++)	//采集几次随机数
	{
		array_temp[i] = (u8)(rand()%10); //取随机数
	}
}


void codesave(u8 dtpw[])
{   /*u8 dtpw[]={6,6,6,6,6,6,0,0,0,0};*/
int i,j=0;
for(j=0;j<6;j++)
		{
			At24c02Write(j+2,dtpw[j]);
           for(i=100;i>0;i--);
      //写生成的密码至EEPROM

		}
At24c02Write(10,1);
}

int cyphernum(u8 feedseed)
{	int i;
   	srand(feedseed);
	i=rand()%3;
	return i;
}
