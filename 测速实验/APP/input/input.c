#include "input.h"
#include "led.h"

u16 dt=0;
/*******************************************************************************
* ��������		   : TIM5_CH1���벶���ʼ������
*******************************************************************************/
void TIM5_CH1_Input_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//ʹ��TIM5ʱ��

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;            //�ܽ�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	       //������������ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	           // ��ʼ��GPIO 
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;          //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;       //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //ͨ��1
	TIM_ICInitStructure.TIM_ICFilter=0x02;         //�˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;//������
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;       //��Ƶϵ��
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ֱ��ӳ�䵽TI1
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;        //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		   //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��
}

/*******************************************************************************
* ��������		   : TIM5�жϺ�������¼�½��ظ���
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	
     if(TIM_GetITStatus(TIM5,TIM_IT_CC1)) //���������ж�
		 {
			 dt++;
		 }
	 
	   TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);
}



