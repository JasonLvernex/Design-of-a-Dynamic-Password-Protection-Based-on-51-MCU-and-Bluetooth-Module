#include "key.h"

u8 KeyValue=0;



/*******************************************************************************

*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************

*******************************************************************************/
u8 KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;

	if(GPIO_KEY!=0x0f)//�а�������
	{
		delay(1000);//��ʱ10msȥ��
		if(GPIO_KEY!=0x0f)//�а�������
		{	
			
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=1;break;
				case(0X0b):	KeyValue=2;break;
				case(0X0d): KeyValue=3;break;
				case(0X0e):	KeyValue=13;break;
			}
			
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+3;break;
				case(0Xd0): KeyValue=KeyValue+6;break;
				case(0Xe0):	if(KeyValue==2){KeyValue=-9;}KeyValue=KeyValue+9;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 
			{
				delay(1000);
				a++;
			}
			 
		}
	}
	else
	{
		KeyValue=0xff;  //�ް�������
	}
	
	return KeyValue;  //����KeyValue
}
