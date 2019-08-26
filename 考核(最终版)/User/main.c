#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "input.h"
#include "pwm.h"
#include "pid.h"
#include "lcd.h"
#include "key.h"
#include "eeprom.h"
#include "my_printf.h"

float Set_Speed;   //设置的速度
u16 Set_Angle,Now_Angle=0;//设置的角度和已转角度
u16 x=6;           //波形图初始x坐标

/*******************************************************************************
* 函数功能		   : 主函数
*******************************************************************************/

int main()
{
	float indata=0.0;
	u8 ms100=0;       //100ms
	u16 y_1=0,y_2=0;  //波形图本次Y坐标和上次y坐标
	u16 dat=0;
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  //中断优先级分组 分2组
	LED_Init();
  USART1_Init(9600);	
	Pid_Init();      //初始化PID
	Eeprom_Init();   //初始化EEPROM
	Eeprom_Readdat();//读取相关参数
	Lcd_Init();			 //初始化OLED  
	LCD_ShowA();     //显示界面A
	Key_Init();      //按键初始化
	TIM4_Init(2000,72-1);   //2ms刷新一次
	TIM5_CH1_Input_Init(1000,71);  //以1M频率计数
	TIM3_CH1_PWM_Init(1000,72-1);  //频率是1Kh
	TIM2_CH3_PWM_Init(1000,71);    //频率是1Kh
	
	while(1)
	{
		if(star==1)
		{
				if(ms1>=100) 
				{	
					 indata=dt/260.0*10;
					 dt=0;	  //计算电机转速（转每秒）
					 ms1=0;
					 ms100++;
					 Pid_Calc(Set_Speed,indata);    //计算所需占空比
					 TIM_SetCompare1(TIM3,pid_dat);	//pid执行												 
					 if(indata-Set_Speed<=4.6&&indata-Set_Speed>=-4.6)
					 {
							 y_2=y_1;
							 y_1=(u16)(125-(indata-(Set_Speed-4.6))*5);			
							 LCD_DrawWave(x,y_1,x-1,y_2);   
							 x++;                   //显示转速波形
							 if(x>=125)
							 {
									x=6;
									Clear_WaveA();   //清除波形
							 }
					 }
				}
				if(ms100>=10)
				{
						ms100=0;
				   	my_printf_lcd(80,0,"%f",indata);	//每秒刷新一次转速	  
				}
			  indata=0;  //转速清零，准备下次循环
		}
		else if(star==5||star==6)
		{		
				S_Scan();	  //扫描USRT
		}
		else if(star==3||star==0)
		{ //电机停止缓冲
			TIM_SetCompare1(TIM3,0); 
			TIM_SetCompare3(TIM2,0);//关闭电机
			delay_ms(1000);   //延时1s
			dt=y_1=y_2=0;     //坐标和脉冲清零
			if(star==3)
			{ //当前状态为角度控制
				pid_dat=190;   //占空比初始化
			  star=5;	       //进入界面B修改状态
			}
			else
				star=6;	       //进入界面A修改状态
		}
		else if(star==4)
		{   //设置角度执行
			if(ms1>=50) 
			{	
					indata=(dt-dat)/260.0*10;
				  dat=dt;	  //计算电机转速（转每秒）
				  ms1=0;    //毫秒清零
				  Pid_Calc(0.1,indata);         //PID计算
					TIM_SetCompare1(TIM3,pid_dat);//pid执行						
					y_2=y_1;                      //本次坐标变上次坐标
					y_1=(u16)(125-dt/260.0*360.0/Set_Angle*85.0);	
																				//计算坐标
					LCD_DrawWave(x,y_1,x-1,y_2);   
					x++;                   //显示波形
					if(x>=125)
					{
							x=6;
							Clear_WaveB();   //清除波形
					}
					Now_Angle=dt/260.0*360; 
					my_printf_lcd(80,0,"%d",Now_Angle);//计算并显示已转角度
			  }
				if(dt>=(u16)(Set_Angle*260/360)-2)
				{   //到了设定角度
						TIM_SetCompare1(TIM3,0);
						TIM_SetCompare3(TIM2,190);//阻止电机转动
						y_2=y_1;
						y_1=(u16)(125-dt/260.0*360.0/Set_Angle*85.0);			
						LCD_DrawWave(x,y_1,x-1,y_2); 
						x++;              //显示波形
						Now_Angle=(dt+2)/260.0*360;
						LCD_ShowNum(80,0,Now_Angle,3,RED);//计算并显示已转角度			
						delay_ms(500);
						TIM_SetCompare3(TIM2,0);//延时0.5秒后关闭
						dt=0;
						dat=0;
						star=3;    //角度脉冲清零
				}
			  indata=0; 			//转速清零，准备下次循环
		}  
		KeyRead();  //扫描按键
	}
}

