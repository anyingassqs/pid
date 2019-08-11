#include "input.h"
#include "led.h"

u16 dt=0;
/*******************************************************************************
* 函数功能		   : TIM5_CH1输入捕获初始化函数
*******************************************************************************/
void TIM5_CH1_Input_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//使能TIM5时钟

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;            //管脚设置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	       //设置下拉输入模式
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	           // 初始化GPIO 
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;          //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;       //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //通道1
	TIM_ICInitStructure.TIM_ICFilter=0x02;         //滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;//捕获极性
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;       //分频系数
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//直接映射到TI1
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;        //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		   //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM5,ENABLE); //使能定时器
}

/*******************************************************************************
* 函数功能		   : TIM5中断函数，记录下降沿个数
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	
     if(TIM_GetITStatus(TIM5,TIM_IT_CC1)) //发生捕获中断
		 {
			 dt++;
		 }
	 
	   TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);
}



