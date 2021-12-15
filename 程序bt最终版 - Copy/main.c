/**************************************************************************************
*		              电子密码锁												  *
实现现象：下载程序输入正确密码，会听到继电器吸合的声音
硬件连接：
           P1 -->矩阵键盘
					 P20--> EEPROM模块SDA
					 P21--> EEPROM模块SCL
					 P22-->继电器模块J15
					 
					 矩阵键盘：
					 1      2      3       13(empty)
					 
					 4      5      6       up(16)
					 
					 7      8      9      down(19)
					 
				返回（10）  0（0） 确定（12）      21（empty）
注意事项：无																				  
***************************************************************************************/

#include "reg52.h"	 	       //此文件中定义了单片机的一些特殊功能寄存器
#include "lcd.h"
#include "key.h"
#include "i2c.h"
#include "timer.h"
#include "codemm.h"
#include "bluetoothmm.h"


#define u16 unsigned int 	  //对数据类型进行声明定义
#define u8 unsigned char

sbit AS=P2^2;               //继电器 D3	 LED is test light D4


/*****************************************************************************/

/*****************************************************************************/
u8 pw_num,Error_Num,PassWord_Length=6;
u8 PASSWORD[]={8,8,8,8,8,8,0,0,0,0};
u8 INPUT_PW_Tab[10];
u8 key_num,Step,Step5,Load_first_flag=0;
u8 dtmm_enable=1;
u8 lcd_clear=0;
u8 code_execute_flag=0;
int test1=0;
int jet=0;
u8 seed=0;
u8 light=0;
int yet=0;
int cyphernumber;



/*****************************************************************************/

/*******************************************************************************/
bit result_flag,Input_suc_flag;

/*****************************************************************************/

/*******************************************************************************/
void Step_0();
void Step_1();
void Step_2();
void Step_3();
void Step_4();
	  
void Step_5();
		 void Step5_0();
	   void Step5_1();
	   void Step5_2();
	   void Step5_3();
	   void Step5_4();
	   void Step5_5();
void Step_6();
/********************************************************************************/

/*******************************************************************************/
void CipherComparison();
void input_password(bit m);
void Read_Password();
void code_execute();



/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	//u8 data1,a;
	int j=0;
	//	ShowString(0x00," Pechin Science ");  
	//	At24c02Write(0,0);                                                //开机显示密码
	LcdWriteCom(0x01);  //清屏
/*	for(data1=0;data1<PassWord_Length+2;data1++)
	{
		a=At24c02Read(data1)+0x30;
	 	LcdWriteData(a);     	
  	delay(1000);	
  }	//开机瞬间会显示密码，最后要删除此段for循环	 */
	
	delay(1000);
	LcdInit();	 //LCD初始化
	delay(1000);
	Step=0;		 
	Step5=0;	   //步骤函数Case归零
	Error_Num=0x00;
	if(At24c02Read(0x09)!=1) At24c02Write(0,0x00);
   Read_Password();  //其中包括了两部分，一部分为第一次使用时设定初始密码，第二部分为如不是第一次使用时，从24c02读取设置的密码
	
    //保存 密码长度
//		for(j=0;j<10;j++)
//		{
//			At24c02Write(j,j);
//			delay(100);	
//		}
//	
		Timer0Init();
		UsartConfiguration();
	
	while(1)
				{	      		
								
								key_num=KeyDown();                   //读取输入值
							

										switch(Step)
										{
												case 0:
												 {Step_0();break;}
												case 1:
												 {Step_1();break;}
												case 2:
												 {Step_2();break;}
												case 3:
												 {Step_3();break;}	
												case 4:
												 {Step_4();break;}	
												case 5:
												 {Step_5();break;}	
												case 6:
												 {Step_6();break;}	
												  
										}

								if(  At24c02Read(0x0a)==1)
							 {	
								for(j=0;j<6;j++)       //读取密码
                        	{
		                          PASSWORD[j]=At24c02Read(j+2);
	                           } 	   	
	                         At24c02Write(1,6);                //保存 密码长度
	                           delay(100);
							   At24c02Write(10,0);}   //新密码的读取。

				 }
				               				
}

/*****************************************************************************/

/*******************************************************************************/
void Step_0()
{
					LcdInit();
					ShowString(0x00," Pechin Science ");                           //第一行显示Pechin Science 
	        ShowString(0x10,"    WELCOME!    ");                           // 第二行显示 WELCOME!  
	
					while(KeyDown()==0xff){ code_execute();Step=1; }                                 //  有按键按下进入下一步  
	      
					
}
/*****************************************************************************/

/*******************************************************************************/
void Step_1()
{
		LcdWriteCom(0x01);                                           //清屏
	  ShowString(0x00,"Unlock");
		ShowString(0x0f,"<");	                                       //1602第一行显示unlock
	  ShowString(0x10,"Change Password");
		ShowString(0x1f," "); 	                                       // 1602第二行显示Change Password                                                                            	
	  Step=2;	                                                      //
}
/*****************************************************************************/

/*******************************************************************************/
void Step_2()
{	 static menu=1;
     int yet=0,back_step=0;
	
	if(key_num!=0x0c)  
		{			
		     
		    if(key_num==0x10)                    //16键或19键按下
					{	   menu--;lcd_clear=1;
					 if(menu==0) menu=3;}
			if(key_num==0x13)
					 	{	   menu++;lcd_clear=1;
					 if(menu==4) menu=1;}
			if(key_num!=0x10&&key_num!=0x13) {back_step=1;goto marke;}

				 switch(menu)
										{
												case 1:
												 {
												 if(lcd_clear==1)	LcdWriteCom(0x01);                                         //清屏
	                                                ShowString(0x00,"Unlock");
		                                             ShowString(0x0f,"<");	                                       //
	                                                ShowString(0x10,"Change Password");
		                                             ShowString(0x1f," ");
													 lcd_clear=0; 
												 break;}
												case 2:
												 {
											     if(lcd_clear==1)	LcdWriteCom(0x01);                                           //清屏
	                                                ShowString(0x00,"Unlock");
		                                             ShowString(0x0f," ");	                                       //
	                                                ShowString(0x10,"Change Password");
		                                             ShowString(0x1f,"<"); 
													 lcd_clear=0; 
												 break;}
												case 3:
												 {
												 if(lcd_clear==1)	LcdWriteCom(0x01);                                           //清屏
	                                                ShowString(0x00,"Change Password");
		                                             ShowString(0x0f," ");	                                       //
	                                                ShowString(0x10,"enable dtmm");
		                                             ShowString(0x1f,"<");
													 lcd_clear=0;  
												 break;}
												 default: break;
												  
										}
										marke:        //加快了这里switch 的运行		          
						  
						if(back_step==1)
		                      {	//for(yet=0;yet<20;yet++){AS=~AS; delay(3000);AS=~AS;delay(3000);} 
						       jet++;
							  if(jet==5000){ jet=0; Step=0;for(yet=0;yet<20;yet++){AS=~AS; delay(1000);AS=~AS;delay(3000);}}
							  }		;				  //长时间不动，返回首页*/ /*仍存在问题*/
					
		 }
	else                                                           //确认键按下
	{
					if(menu==1){Step=3;menu=1;}
					if(menu==2) {Step=5;menu=1;}
					if(menu==3) {dtmm_enable=~dtmm_enable;
					             if(dtmm_enable==1)TR0=1;
							     else TR0=0;
								 menu=1;
								                	LcdWriteCom(0x01);                                         //清屏
	                              if(TR0==1){        ShowString(0x00,"enabled");
								     				  delay(10000);
		                                     }
								  else if(TR0==0){        ShowString(0x00,"disabled");
								                           delay(10000);
		                                     }
								 Step=1;
								/* for(yet=0;yet<20;yet++) {AS=~AS; delay(10000);AS=~AS;delay(10000);} 	*/
							    }
	}
}
/*****************************************************************************/

/*******************************************************************************/
void Step_3()                                             //
{
								Step=4;
	              pw_num=0;
								LcdInit();
								ShowString(0x00,"Pass Word:  ");
                
						
}
/*****************************************************************************/

/*******************************************************************************/
void Step_4()                                                 
{
	   input_password(0);                                              //输入密码并以*显示
	   if(Input_suc_flag==1){Step=6;}                                  //密码输入完成进入下一步
      Input_suc_flag=0;                                              //清除密码输入完成标志
}
/*****************************************************************************/

/*******************************************************************************/
void Step_5()                                                        //修改密码
{
	switch(Step5)
	{
		
			case 0: {Step5_0();}  break;
			case 1: {Step5_1();}  break;
			case 2: {Step5_2();}  break;
			case 3: {Step5_3();}  break;
		  case 4: {Step5_4();}  break;
			case 5:	{Step5_5();}	break;

	}
}
/*****************************************************************************/

/*******************************************************************************/
void Step_6()
{
	CipherComparison();                                //密码比对
	if(result_flag==1)                                 //密码正确
		{
				LcdInit();
				ShowString(0x00,"    WELCOME!");
				AS=0;                                         //开继电器
				delay(60000);
				delay(60000);
				AS=1;	                                        //关继电器
		}
	else                                                //密码错误
		{
				LcdInit();
				ShowString(0x00,"Error 01!");
		}
		
		Step=0;
	

}

/****************************************************************************************
密码输入函数
密码长度：pw_num
密码输入完成标志：Input_suc_flag
*****************************************************************************************/

void Step5_0()                 
{
		LcdWriteCom(0x01);  //清屏
	  ShowString (0x00,"Input PassWord:");     //1602显示：输入密码
	  Step5=1;
	  pw_num=0;
}
void Step5_1()                
{
		input_password(0);                  //   输入密码并以*显示
		if(Input_suc_flag==1)               //密码输入完成
			{
							Step5=2;                  //
							Input_suc_flag=0;         //清除密码输入完成标志
			}
}
void Step5_2()                //
{
     CipherComparison();                //密码比对
	   Step5=3;
}
void Step5_3()                //
{
    if(result_flag==0)         //        密码错误
				{
							if(Error_Num<3)             //输出错误次数小于3
							{
								Error_Num++;
								LcdInit();
								ShowString (0x00,"Error 01");
								delay(20000);
								Step5=0;
							}
							else                          //密码错误次数大于3
							{
								Error_Num=0;
								Step=0;
							}
					
				}
		else						      					       //密码正确
				{
					LcdInit();
					ShowString (0x00,"New PassWord:");	
					pw_num=0;
					Step5=4;
				}
			
					
}
void Step5_4()
{
		input_password(1);     					       //输入密码并显示
		if(Input_suc_flag==1)                  //输入完成
			{ 
				Step5=5;
				Input_suc_flag=0;
				LcdWriteCom(0x01);  //清屏
	      ShowString (0x00,"      OK!");
			}
}
void Step5_5()
{
	unsigned char j;
	
	PassWord_Length=pw_num;                         //读取输入密码长度
	At24c02Write(0,Load_first_flag);        
  delay(100);	
	At24c02Write(1,PassWord_Length);                //保存 密码长度
	delay(100);
	for(j=0;j<PassWord_Length;j++)         
	{
		PASSWORD[j]=INPUT_PW_Tab[j];               		 //读取密码
		At24c02Write(j+2,INPUT_PW_Tab[j]);         		 //保存密码至EEPROM
		delay(100);
}
	Step5=0;
	Step=0;
}


void  Read_Password()
{
	unsigned char j;

	Load_first_flag=At24c02Read(0x00);
	if(Load_first_flag==0)         		 //初次运行
	{
	    At24c02Write(9,1);      //初次运行位置1
		Load_first_flag=1;
		At24c02Write(0,0x01);
		delay(100);
		At24c02Write(1,0x06);           //写默认密码长度6至EEPROM
		delay(100);
		for(j=0;j<PassWord_Length;j++)
		{
			At24c02Write(j+2,8);              //写默认密码888888至EEPROM
			PASSWORD[j]=INPUT_PW_Tab[j];     //读密码
			delay(100);
		}
	}
	
	Load_first_flag=At24c02Read(0x00);
	PassWord_Length=At24c02Read(0x01);  //读取密码长度
	for(j=0;j<PassWord_Length;j++)       //读取密码
	{
		PASSWORD[j]=At24c02Read(j+2);
	}
	
}
void input_password(bit m)
{
	unsigned char j;
	if(key_num!=0x0c)                                      //ok键没有按下
		{
					if(key_num<0x0a)                               //1-9按下
					{
						INPUT_PW_Tab[pw_num]=key_num;                //保存至输入密码数组
						pw_num=pw_num+1;                              //密码长度+1
						
						 LcdWriteCom(0xc0);
						 for(j=0;j<pw_num;j++)
								{
												if(m==0) {LcdWriteData('*');	}                        //密码隐藏
												else     {LcdWriteData(INPUT_PW_Tab[j]+0x30);}         //显示密码
								}
											
					}
					if(key_num==0x0a)                              //返回键按下
					{
						if(pw_num!=0) {pw_num=pw_num-1;}
						else          {Step=0;}
						
							 
						// ShowString (0x00,"Pass Word：");  
                    LcdWriteCom(0xc0);						
										for(j=0;j<pw_num;j++)
											{
														if(m==0) {LcdWriteData('*');	}                   //密码隐藏
														else     {LcdWriteData(INPUT_PW_Tab[j]+0x30);}    //显示密码	
										   }
											LcdWriteData(' '); 
					}
		} 
	else                          //ok键按下
	  {
			if(pw_num==0)	  
					{
							Step=0;
						  LcdWriteCom(0x01);
						  ShowString (0x00,"Error 02!");
							delay(10000);
					}
		
			else{		
							Input_suc_flag=1; 
					}
					 //AS=0;			
		}                
}
/*******************************************************
                密码比对
********************************************************/
void CipherComparison()
{ 
	u8 i,j=0;

	if(PassWord_Length==pw_num)                       //密码长度比对
	{
		for(i=0;i<PassWord_Length;i++)                  //密码比对
		{
			
				if(PASSWORD[i]!=INPUT_PW_Tab[i])
					{
						result_flag=0;break;                     //密码错误
					}
				else
					{
						result_flag=1;                          //密码正确
					}
					  INPUT_PW_Tab[i]=0XFF;                   //清除密码缓存数组
		}
	}
	else
	{result_flag=0;}
   }
 /*******************************************************************************
* 函 数 名         : code_execute()
* 函数功能		   : 密码生成函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void code_execute()
 { 	if(code_execute_flag==1)
								{
								codegenerate(PASSWORD,6,seed);
		                        codesave(PASSWORD);
								cyphernumber=cyphernum(seed);
								
								for(test1=0;test1<20;test1++) {LED=~LED; delay(1000);LED=~LED;delay(1000);}

								code_execute_flag=0;}
	 if(light==1){for(yet=0;yet<20;yet++) {AS=~AS; delay(1000);AS=~AS;delay(1000);} light=0;}
 }
/*******************************************************************************
* 函 数 名         : void Timer0() interrupt 1
* 函数功能		   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0() interrupt 1
{
	static u16 tim=0;
	TH0=0X3C;	//给定时器赋初值，定时50ms
	TL0=0XB0;
	tim++;
	seed++;
	

	if(tim==1200)  //理论上tim=1200定时1分钟  但是机器运行下来却是10秒钟
	{	//int yet=0;
		tim=0;
		if(Step==0||Step==1||Step==2)
		{ 
		  light=1;
		//for(yet=0;yet<20;yet++) {AS=~AS; delay(1000);AS=~AS;delay(1000);} 
		
		 code_execute_flag=1;

	                     	
		
		}
	}
		
}

