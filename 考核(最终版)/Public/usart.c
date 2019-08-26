#include "usart.h"		 

u8 s[37]={0};
u8 b=0;

/*******************************************************************************
* ��������		   : printf�����ض���
*******************************************************************************/ 
int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* ��������		   : USART1��ʼ������
*******************************************************************************/ 

void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

/*******************************************************************************
* ��������		   : USART1�жϺ���
*******************************************************************************/ 

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 r;
	static u8 a=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		  r=USART_ReceiveData(USART1);
			if(star==5||star==6)
		  {
				 if(a>66&&a<77)
				 {
					 s[b++]='o';
					 a=0;           //������ɱ�־
				 }
				 if(a>=19&&a<=66)
				 {
					  s[b++]=r;
					  a+=10;          //����������ʼ��������
				 }
				if(r=='='&&a>10)
				{
					  switch(a)
					  {
							 case 11:a+=8;s[b++]='s';break;
							 case 12:a+=33;s[b++]='a';break;
							 case 15:a+=5;s[b++]='p';break;
							 case 16:a+=5;s[b++]='i';break;
							 case 17:a+=5;s[b++]='d';break;//������������б��
							 default: a=0;break; //����������a�������
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
							case 'd':a+=12;break;	//�жϵڶ�λ�Ƿ���e��p��i��d֮һ
							default: a=0;break;   //�������������
						}
				}	
			  if(a==0)
			   switch(r)
			   {
				     case 's': a++;break;
			 	     case 'k': a+=5;break;  //�жϵ�һλ�Ƿ�Ϊs��k
			   }
		 }
  }
	USART_ClearFlag(USART1,USART_FLAG_TC);//������ձ�־
} 	



