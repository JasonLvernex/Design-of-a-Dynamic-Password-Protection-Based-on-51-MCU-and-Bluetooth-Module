#include "reg52.h"
#include "codemm.h"
#include "i2c.h"
#include <intrins.h>
#include <stdlib.h>

#define RAND_BIT	6 //�����λ��




void codegenerate(u8 array_temp[],u8 rand_bit,u8 feedseed)
{	u8 i = 0;
	srand(feedseed); //�������ʼ��
	for(i = 0;i < rand_bit;i++)	//�ɼ����������
	{
		array_temp[i] = (u8)(rand()%10); //ȡ�����
	}
}


void codesave(u8 dtpw[])
{   /*u8 dtpw[]={6,6,6,6,6,6,0,0,0,0};*/
int i,j=0;
for(j=0;j<6;j++)
		{
			At24c02Write(j+2,dtpw[j]);
           for(i=100;i>0;i--);
      //д���ɵ�������EEPROM

		}
At24c02Write(10,1);
}

int cyphernum(u8 feedseed)
{	int i;
   	srand(feedseed);
	i=rand()%3;
	return i;
}
