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
* 名称 : passwordRD()
* 功能 : 发送密码读取函数
* 输入 : 无 * 输出 : 无
***********************************************************************/
void passwordRD()
{ int j;
  for(j=0;j<6;j++)       //读取密码
	{
		passwd[j]=At24c02Read(j+2)+48;
	}

}








/********************************************************************
* 名称 : Com_Int(void) interrupt 4
* 功能 : 串口中断子函数
* 输入 : 无 * 输出 : 无
***********************************************************************/
void Com_Int(void) interrupt 4
{
	u8 i,n=0,_cnt=0;
  u8 receive_data;
  u8 sucess_in_cypher=0;

	
  ES = 0;
	
  if(RI == 1) //当硬件接收到一个数据时，RI会置位
	{ 		
		RI = 0;
		receive_data = SBUF;//接收到的数据
			
		if(receive_data == '9')	 
		{		passwordRD();
				LED =0;//接收到1亮灯
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
				LED =1; //其他情况灯灭
		}
		
	}
	  if(sucess_in_cypher)
	  {	
		for(i=0; i<6; i++)
		{
			SBUF = passwd[i];   //将要发送的数据放入到发送寄存器
			while(!TI);		    //等待发送数据完成
			TI=0;			        //清除发送完成标志位
			Delay_1ms(1);
		}
		}
		ES = 1;
} 


/*******************************************************************************
* 函 数 名       : UsartConfiguration()
* 函数功能		   : 设置串口
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/

void UsartConfiguration()
{	
	SCON=0X50;			//设置为工作方式1,8位数据，可变波特率
	TMOD|=0X20;			//设置计数器工作方式2
	PCON=0X00;			//波特率不加倍
	TH1=0XFd;		    //计数器初始值设置，9600@11.0592MHz
	TL1=0XFd;
	TR1=1;					//打开计数器
	ES = 1;         //开串口中断
    EA = 1;         //开总中断
}


/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms
* x * 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay_1ms(u16 i)//1ms延时
{
  u8 x,j;
  
  for(j=0;j<i;j++)
    for(x=0;x<=148;x++);

}
/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms
* x * 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
