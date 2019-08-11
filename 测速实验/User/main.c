#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "input.h"
#include "pwm.h"
#include "pid.h"
#include "lcd.h"
#include "key.h"


/*******************************************************************************
* 函数功能		   : 主函数
*******************************************************************************/
float Set_Speed=25.0;//设置的速度
u16 x=6;             //波形图初始x坐标
int main()
{
	float indata=0.0;
	u8 ms100=0;       //100ms
	u16 y_1=0,y_2=0;  //波形图本次Y坐标和上次y坐标
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  //中断优先级分组 分2组
	LED_Init();
  USART1_Init(9600);
	TIM5_CH1_Input_Init(1000,71);  //以1M频率计数

	TIM3_CH1_PWM_Init(1000,72-1);  //频率是1Kh
  TIM4_Init(2000,72-1);   //2ms刷新一次
	Pid_Init();      //初始化PID
	Lcd_Init();			 //初始化OLED  
	LCD_Clear(WHITE);
	BACK_COLOR=WHITE;//清屏

	LCD_Star();
	LCD_ShowNum1(80,20,Set_Speed,4,RED);
	Key_Init();
  LCD_DrawLine(4,80,4,125,YELLOW);
	LCD_DrawLine(4,103,125,103,YELLOW);
	LCD_ShowNum1(28,40,pid.Kp,4,RED);
	LCD_ShowNum1(4,60,pid.Ki,4,RED);
	LCD_ShowNum1(76,60,pid.Kd,4,RED);//lcd显示A界面

	while(1)
	{
		if(ms1>=100) 
		{	
			 indata=dt/334.0*10;
		   dt=0;	  //计算电机转速（转每秒）
			 ms1=0;
			 ms100++;
			 if(star==1)
		   {
		       Pid_Calc(Set_Speed,indata); 
		       TIM_SetCompare1(TIM3,pid_dat);	//pid计算并执行								
		   }
			 else
		   {
			    TIM_SetCompare1(TIM3,0); //电机停止转动
		   }
			 if(indata-Set_Speed<=4.6&&indata-Set_Speed>=-4.6)
			 {
			     y_2=y_1;
			     y_1=(u16)(125-(indata-(Set_Speed-4.6))*5);			
			     LCD_DrawWave(x,y_1,x-1,y_2);   
			     x++;                   //显示转速波形
				   if(x>=125)
				   {
					    x=6;
              Clear_Wave();   //清除波形
				   }
			 }
       S_Scan();	//检测串口是否发来数据	
	  }
		if(ms100>=10)
		{
			  ms100=0;
		    LCD_ShowNum1(80,0,indata,4,RED);	//每秒刷新一次转速	  
		}
		indata=0;   //转速清零，准备下次循环
		KeyRead();  //扫描按键
	}
}

