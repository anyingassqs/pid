#include "usart.h"		 

u8 s[37]={0};
u8 b=0;

/*******************************************************************************
* 函数功能		   : printf函数重定向
*******************************************************************************/ 
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函数功能		   : USART1初始化函数
*******************************************************************************/ 

void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函数功能		   : USART1中断函数
*******************************************************************************/ 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 r;
	static u8 a=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		  r=USART_ReceiveData(USART1);
			if(star==5||star==6)
		  {
				 if(a>66&&a<77)
				 {
					 s[b++]='o';
					 a=0;           //接收完成标志
				 }
				 if(a>=19&&a<=66)
				 {
					  s[b++]=r;
					  a+=10;          //符合条件后开始接收数据
				 }
				if(r=='='&&a>10)
				{
					  switch(a)
					  {
							 case 11:a+=8;s[b++]='s';break;
							 case 12:a+=33;s[b++]='a';break;
							 case 15:a+=5;s[b++]='p';break;
							 case 16:a+=5;s[b++]='i';break;
							 case 17:a+=5;s[b++]='d';break;//符合条件后进行标记
							 default: a=0;break; //不符合条件a清零结束
					  }
				}
				if(a==1||a==5)
				 {
						switch(r)
						{
							case 'a':a+=11;break;
							case 's':a+=10;break;
							case 'p':a+=10;break;
							case 'i':a+=11;break;
							case 'd':a+=12;break;	//判断第二位是否是e，p，i，d之一
							default: a=0;break;   //如果不是则清零
						}
				}	
			  if(a==0)
			   switch(r)
			   {
				     case 's': a++;break;
			 	     case 'k': a+=5;break;  //判断第一位是否为s或k
			   }
		 }
  }
	USART_ClearFlag(USART1,USART_FLAG_TC);//清除接收标志
} 	



