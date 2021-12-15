#include<reg52.h>
#include<intrins.h>
#include<bluetoothmm.h>
#include<i2c.h>
#include <stdio.h>
#include <string.h>

u8 passwd[6];
extern int cyphernumber;
u8 code ques1[]="how old are you",ques2[]="what is your name",ques3[]="are you a boy",reply[]="wrong";
u8 ans1[20];
u8 *point;
static int alarm=0;

/********************************************************************
* ���� : passwordRD()
* ���� : ���������ȡ����
* ���� : �� * ��� : ��
***********************************************************************/
void passwordRD()
{ int j;
  for(j=0;j<6;j++)       //��ȡ����
	{
		passwd[j]=At24c02Read(j+2)+48;
	}

}








/********************************************************************
* ���� : Com_Int(void) interrupt 4
* ���� : �����ж��Ӻ���
* ���� : �� * ��� : ��
***********************************************************************/
void Com_Int(void) interrupt 4
{
	u8 i,n=0,_cnt=0;
  u8 receive_data;
  u8 sucess_in_cypher=0;

	
  ES = 0;
	
  if(RI == 1) //��Ӳ�����յ�һ������ʱ��RI����λ
	{ 		
		RI = 0;
		receive_data = SBUF;//���յ�������
			
		if(receive_data == '9')	 
		{		passwordRD();
				LED =0;//���յ�1����
				switch(cyphernumber)
				{case 0:
				      { 
					  point=ques1;
					  do{
					  SBUF=*point;
					  while(!TI);
					  TI=0;
					  point++;
					  }while(*point!='\0');
					  while(RI==0&&_cnt<5000){_cnt++;} 
					   	//RI = 0;
						do{	 ans1[n]=SBUF;
							 n++;
							 RI = 0;
							 Delay_1ms(10);
							// while(RI==0);
						  } while(ans1[n]!='\0');
						  if(!strcmp(ans1,"21")) sucess_in_cypher=1;
						  else  {alarm++;

								   point=reply;
						          do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');

									point=ans1;
								 do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');
								 }
				      break;
			        	}
				case 1:
				      { 
					  point=ques2;
					  do{
					  SBUF=*point;
					  while(!TI);
					  TI=0;
					  point++;
					  }while(*point!='\0');
					  while(RI==0&&_cnt<5000){_cnt++;} 
					   //	RI = 0;
						do{	 ans1[n]=SBUF;
							 n++;
							 RI = 0;
							 Delay_1ms(10);
							 // while(RI==0);
						  } while(ans1[n]!='\0');
						  if(!strcmp(ans1,"jason")) sucess_in_cypher=1;
						   else  {alarm++;
								   point=reply;
						          do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');
									point=ans1;
								 do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');
								 }
				      break;
			        	}
				default:
				      { 
					  point=ques3;
					  do{
					  SBUF=*point;
					  while(!TI);
					  TI=0;
					  point++;
					  }while(*point!='\0');
					  while(RI==0&&_cnt<5000){_cnt++;} 
					 //  	RI = 0;
    
						do{	 ans1[n]=SBUF;
							 n++;
							 RI = 0;
							 Delay_1ms(10);
							 // while(RI==0);
						  } while(ans1[n]!='\0');
						  if(!strcmp(ans1,"yes")) sucess_in_cypher=1;
						  else  {alarm++;
								   point=reply;
						          do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');
								point=ans1;
								 do{
					            SBUF=*point;
					            while(!TI);
					            TI=0;
					            point++;
					            }while(*point!='\0');
								 }
				      break;
			        	}

				}
				
		}
		else
		{
				LED =1; //�����������
		}
		
	}
	  if(sucess_in_cypher)
	  {	
		for(i=0; i<6; i++)
		{
			SBUF = passwd[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
			while(!TI);		    //�ȴ������������
			TI=0;			        //���������ɱ�־λ
			Delay_1ms(1);
		}
		}
		ES = 1;
} 


/*******************************************************************************
* �� �� ��       : UsartConfiguration()
* ��������		   : ���ô���
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/

void UsartConfiguration()
{	
	SCON=0X50;			//����Ϊ������ʽ1,8λ���ݣ��ɱ䲨����
	TMOD|=0X20;			//���ü�����������ʽ2
	PCON=0X00;			//�����ʲ��ӱ�
	TH1=0XFd;		    //��������ʼֵ���ã�9600@11.0592MHz
	TL1=0XFd;
	TR1=1;					//�򿪼�����
	ES = 1;         //�������ж�
    EA = 1;         //�����ж�
}


/********************************************************************
* ���� : Delay_1ms()
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms
* x * ���� : x (��ʱһ����ĸ���)
* ��� : ��
***********************************************************************/
void Delay_1ms(u16 i)//1ms��ʱ
{
  u8 x,j;
  
  for(j=0;j<i;j++)
    for(x=0;x<=148;x++);

}
/********************************************************************
* ���� : Delay_1ms()
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms
* x * ���� : x (��ʱһ����ĸ���)
* ��� : ��
***********************************************************************/
