/**************************************************************************************
*		              ����������												  *
ʵ���������س���������ȷ���룬�������̵������ϵ�����
Ӳ�����ӣ�
           P1 -->�������
					 P20--> EEPROMģ��SDA
					 P21--> EEPROMģ��SCL
					 P22-->�̵���ģ��J15
					 
					 ������̣�
					 1      2      3       13(empty)
					 
					 4      5      6       up(16)
					 
					 7      8      9      down(19)
					 
				���أ�10��  0��0�� ȷ����12��      21��empty��
ע�������																				  
***************************************************************************************/

#include "reg52.h"	 	       //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "lcd.h"
#include "key.h"
#include "i2c.h"
#include "timer.h"
#include "codemm.h"
#include "bluetoothmm.h"


#define u16 unsigned int 	  //���������ͽ�����������
#define u8 unsigned char

sbit AS=P2^2;               //�̵��� D3	 LED is test light D4


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
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	//u8 data1,a;
	int j=0;
	//	ShowString(0x00," Pechin Science ");  
	//	At24c02Write(0,0);                                                //������ʾ����
	LcdWriteCom(0x01);  //����
/*	for(data1=0;data1<PassWord_Length+2;data1++)
	{
		a=At24c02Read(data1)+0x30;
	 	LcdWriteData(a);     	
  	delay(1000);	
  }	//����˲�����ʾ���룬���Ҫɾ���˶�forѭ��	 */
	
	delay(1000);
	LcdInit();	 //LCD��ʼ��
	delay(1000);
	Step=0;		 
	Step5=0;	   //���躯��Case����
	Error_Num=0x00;
	if(At24c02Read(0x09)!=1) At24c02Write(0,0x00);
   Read_Password();  //���а����������֣�һ����Ϊ��һ��ʹ��ʱ�趨��ʼ���룬�ڶ�����Ϊ�粻�ǵ�һ��ʹ��ʱ����24c02��ȡ���õ�����
	
    //���� ���볤��
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
								
								key_num=KeyDown();                   //��ȡ����ֵ
							

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
								for(j=0;j<6;j++)       //��ȡ����
                        	{
		                          PASSWORD[j]=At24c02Read(j+2);
	                           } 	   	
	                         At24c02Write(1,6);                //���� ���볤��
	                           delay(100);
							   At24c02Write(10,0);}   //������Ķ�ȡ��

				 }
				               				
}

/*****************************************************************************/

/*******************************************************************************/
void Step_0()
{
					LcdInit();
					ShowString(0x00," Pechin Science ");                           //��һ����ʾPechin Science 
	        ShowString(0x10,"    WELCOME!    ");                           // �ڶ�����ʾ WELCOME!  
	
					while(KeyDown()==0xff){ code_execute();Step=1; }                                 //  �а������½�����һ��  
	      
					
}
/*****************************************************************************/

/*******************************************************************************/
void Step_1()
{
		LcdWriteCom(0x01);                                           //����
	  ShowString(0x00,"Unlock");
		ShowString(0x0f,"<");	                                       //1602��һ����ʾunlock
	  ShowString(0x10,"Change Password");
		ShowString(0x1f," "); 	                                       // 1602�ڶ�����ʾChange Password                                                                            	
	  Step=2;	                                                      //
}
/*****************************************************************************/

/*******************************************************************************/
void Step_2()
{	 static menu=1;
     int yet=0,back_step=0;
	
	if(key_num!=0x0c)  
		{			
		     
		    if(key_num==0x10)                    //16����19������
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
												 if(lcd_clear==1)	LcdWriteCom(0x01);                                         //����
	                                                ShowString(0x00,"Unlock");
		                                             ShowString(0x0f,"<");	                                       //
	                                                ShowString(0x10,"Change Password");
		                                             ShowString(0x1f," ");
													 lcd_clear=0; 
												 break;}
												case 2:
												 {
											     if(lcd_clear==1)	LcdWriteCom(0x01);                                           //����
	                                                ShowString(0x00,"Unlock");
		                                             ShowString(0x0f," ");	                                       //
	                                                ShowString(0x10,"Change Password");
		                                             ShowString(0x1f,"<"); 
													 lcd_clear=0; 
												 break;}
												case 3:
												 {
												 if(lcd_clear==1)	LcdWriteCom(0x01);                                           //����
	                                                ShowString(0x00,"Change Password");
		                                             ShowString(0x0f," ");	                                       //
	                                                ShowString(0x10,"enable dtmm");
		                                             ShowString(0x1f,"<");
													 lcd_clear=0;  
												 break;}
												 default: break;
												  
										}
										marke:        //�ӿ�������switch ������		          
						  
						if(back_step==1)
		                      {	//for(yet=0;yet<20;yet++){AS=~AS; delay(3000);AS=~AS;delay(3000);} 
						       jet++;
							  if(jet==5000){ jet=0; Step=0;for(yet=0;yet<20;yet++){AS=~AS; delay(1000);AS=~AS;delay(3000);}}
							  }		;				  //��ʱ�䲻����������ҳ*/ /*�Դ�������*/
					
		 }
	else                                                           //ȷ�ϼ�����
	{
					if(menu==1){Step=3;menu=1;}
					if(menu==2) {Step=5;menu=1;}
					if(menu==3) {dtmm_enable=~dtmm_enable;
					             if(dtmm_enable==1)TR0=1;
							     else TR0=0;
								 menu=1;
								                	LcdWriteCom(0x01);                                         //����
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
	   input_password(0);                                              //�������벢��*��ʾ
	   if(Input_suc_flag==1){Step=6;}                                  //����������ɽ�����һ��
      Input_suc_flag=0;                                              //�������������ɱ�־
}
/*****************************************************************************/

/*******************************************************************************/
void Step_5()                                                        //�޸�����
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
	CipherComparison();                                //����ȶ�
	if(result_flag==1)                                 //������ȷ
		{
				LcdInit();
				ShowString(0x00,"    WELCOME!");
				AS=0;                                         //���̵���
				delay(60000);
				delay(60000);
				AS=1;	                                        //�ؼ̵���
		}
	else                                                //�������
		{
				LcdInit();
				ShowString(0x00,"Error 01!");
		}
		
		Step=0;
	

}

/****************************************************************************************
�������뺯��
���볤�ȣ�pw_num
����������ɱ�־��Input_suc_flag
*****************************************************************************************/

void Step5_0()                 
{
		LcdWriteCom(0x01);  //����
	  ShowString (0x00,"Input PassWord:");     //1602��ʾ����������
	  Step5=1;
	  pw_num=0;
}
void Step5_1()                
{
		input_password(0);                  //   �������벢��*��ʾ
		if(Input_suc_flag==1)               //�����������
			{
							Step5=2;                  //
							Input_suc_flag=0;         //�������������ɱ�־
			}
}
void Step5_2()                //
{
     CipherComparison();                //����ȶ�
	   Step5=3;
}
void Step5_3()                //
{
    if(result_flag==0)         //        �������
				{
							if(Error_Num<3)             //����������С��3
							{
								Error_Num++;
								LcdInit();
								ShowString (0x00,"Error 01");
								delay(20000);
								Step5=0;
							}
							else                          //��������������3
							{
								Error_Num=0;
								Step=0;
							}
					
				}
		else						      					       //������ȷ
				{
					LcdInit();
					ShowString (0x00,"New PassWord:");	
					pw_num=0;
					Step5=4;
				}
			
					
}
void Step5_4()
{
		input_password(1);     					       //�������벢��ʾ
		if(Input_suc_flag==1)                  //�������
			{ 
				Step5=5;
				Input_suc_flag=0;
				LcdWriteCom(0x01);  //����
	      ShowString (0x00,"      OK!");
			}
}
void Step5_5()
{
	unsigned char j;
	
	PassWord_Length=pw_num;                         //��ȡ�������볤��
	At24c02Write(0,Load_first_flag);        
  delay(100);	
	At24c02Write(1,PassWord_Length);                //���� ���볤��
	delay(100);
	for(j=0;j<PassWord_Length;j++)         
	{
		PASSWORD[j]=INPUT_PW_Tab[j];               		 //��ȡ����
		At24c02Write(j+2,INPUT_PW_Tab[j]);         		 //����������EEPROM
		delay(100);
}
	Step5=0;
	Step=0;
}


void  Read_Password()
{
	unsigned char j;

	Load_first_flag=At24c02Read(0x00);
	if(Load_first_flag==0)         		 //��������
	{
	    At24c02Write(9,1);      //��������λ��1
		Load_first_flag=1;
		At24c02Write(0,0x01);
		delay(100);
		At24c02Write(1,0x06);           //дĬ�����볤��6��EEPROM
		delay(100);
		for(j=0;j<PassWord_Length;j++)
		{
			At24c02Write(j+2,8);              //дĬ������888888��EEPROM
			PASSWORD[j]=INPUT_PW_Tab[j];     //������
			delay(100);
		}
	}
	
	Load_first_flag=At24c02Read(0x00);
	PassWord_Length=At24c02Read(0x01);  //��ȡ���볤��
	for(j=0;j<PassWord_Length;j++)       //��ȡ����
	{
		PASSWORD[j]=At24c02Read(j+2);
	}
	
}
void input_password(bit m)
{
	unsigned char j;
	if(key_num!=0x0c)                                      //ok��û�а���
		{
					if(key_num<0x0a)                               //1-9����
					{
						INPUT_PW_Tab[pw_num]=key_num;                //������������������
						pw_num=pw_num+1;                              //���볤��+1
						
						 LcdWriteCom(0xc0);
						 for(j=0;j<pw_num;j++)
								{
												if(m==0) {LcdWriteData('*');	}                        //��������
												else     {LcdWriteData(INPUT_PW_Tab[j]+0x30);}         //��ʾ����
								}
											
					}
					if(key_num==0x0a)                              //���ؼ�����
					{
						if(pw_num!=0) {pw_num=pw_num-1;}
						else          {Step=0;}
						
							 
						// ShowString (0x00,"Pass Word��");  
                    LcdWriteCom(0xc0);						
										for(j=0;j<pw_num;j++)
											{
														if(m==0) {LcdWriteData('*');	}                   //��������
														else     {LcdWriteData(INPUT_PW_Tab[j]+0x30);}    //��ʾ����	
										   }
											LcdWriteData(' '); 
					}
		} 
	else                          //ok������
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
                ����ȶ�
********************************************************/
void CipherComparison()
{ 
	u8 i,j=0;

	if(PassWord_Length==pw_num)                       //���볤�ȱȶ�
	{
		for(i=0;i<PassWord_Length;i++)                  //����ȶ�
		{
			
				if(PASSWORD[i]!=INPUT_PW_Tab[i])
					{
						result_flag=0;break;                     //�������
					}
				else
					{
						result_flag=1;                          //������ȷ
					}
					  INPUT_PW_Tab[i]=0XFF;                   //������뻺������
		}
	}
	else
	{result_flag=0;}
   }
 /*******************************************************************************
* �� �� ��         : code_execute()
* ��������		   : �������ɺ���
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : void Timer0() interrupt 1
* ��������		   : ��ʱ��0�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0() interrupt 1
{
	static u16 tim=0;
	TH0=0X3C;	//����ʱ������ֵ����ʱ50ms
	TL0=0XB0;
	tim++;
	seed++;
	

	if(tim==1200)  //������tim=1200��ʱ1����  ���ǻ�����������ȴ��10����
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

