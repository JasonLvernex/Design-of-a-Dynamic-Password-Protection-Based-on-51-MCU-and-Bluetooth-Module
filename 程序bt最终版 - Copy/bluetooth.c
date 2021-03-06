//**** 声明 ********************************************************************
/*******************************************************************************
 * 下面来自互联开源程序，由广州汇承信息科技有限公司收集
 * 方便用户参考学习，本公司不提供任何技术支持
 * 程序仅供测试参考，不能应用在实际工程中，不一定能通过编译
 * 公司网站  http://www.hc01.com/
 * 淘宝网址  http://hc-com.taobao.com/
*******************************************************************************/
               		       
/*-----------------------------------------------------------------------------
* 实 验 名		 : RS232串口试验
* 实验说明     : 单片机串口接收数据控制灯亮灭
* 连接方式     : 
* 注    意		 : 该试验使用的晶振是11.0592
*******************************************************************************/

#include<reg52.h>
#include<intrins.h>

#define uchar unsigned char
#define uint  unsigned int

uchar PuZh[36] = "广州汇承信息科技有限公司www.hc01.com";

//--定义使用的IO--//

sbit LED=P2^0;


//--声明全局函数--//   
void UsartConfiguration();
void Delay_1ms(uint i);


/********************************************************************
* 名称 : Com_Int()
* 功能 : 串口中断子函数
* 输入 : 无 * 输出 : 无
***********************************************************************/
void Com_Int(void) interrupt 4
{
	uchar i;
  uchar receive_data;
	
  EA = 0;
	
  if(RI == 1) //当硬件接收到一个数据时，RI会置位
	{ 		
		RI = 0;
		receive_data = SBUF;//接收到的数据
			
		if(receive_data == '1')	 
		{
				LED =0;//接收到1亮灯
		}
		else
		{
				LED =1; //其他情况灯灭
		}
		
	}
		
		for(i=0; i<36; i++)
		{
			SBUF = PuZh[i];   //将要发送的数据放入到发送寄存器
			while(!TI);		    //等待发送数据完成
			TI=0;			        //清除发送完成标志位
			Delay_1ms(1);
		}
		EA = 1;
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		   : 主函数
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/

void main()
{

	UsartConfiguration();//初始化串口
	
	while(1);	
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
	TMOD=0X20;			//设置计数器工作方式2
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
void Delay_1ms(uint i)//1ms延时
{
  uchar x,j;
  
  for(j=0;j<i;j++)
    for(x=0;x<=148;x++);
}