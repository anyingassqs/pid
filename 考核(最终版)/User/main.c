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

float Set_Speed;   //���õ��ٶ�
u16 Set_Angle,Now_Angle=0;//���õĽǶȺ���ת�Ƕ�
u16 x=6;           //����ͼ��ʼx����

/*******************************************************************************
* ��������		   : ������
*******************************************************************************/

int main()
{
	float indata=0.0;
	u8 ms100=0;       //100ms
	u16 y_1=0,y_2=0;  //����ͼ����Y������ϴ�y����
	u16 dat=0;
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  //�ж����ȼ����� ��2��
	LED_Init();
  USART1_Init(9600);	
	Pid_Init();      //��ʼ��PID
	Eeprom_Init();   //��ʼ��EEPROM
	Eeprom_Readdat();//��ȡ��ز���
	Lcd_Init();			 //��ʼ��OLED  
	LCD_ShowA();     //��ʾ����A
	Key_Init();      //������ʼ��
	TIM4_Init(2000,72-1);   //2msˢ��һ��
	TIM5_CH1_Input_Init(1000,71);  //��1MƵ�ʼ���
	TIM3_CH1_PWM_Init(1000,72-1);  //Ƶ����1Kh
	TIM2_CH3_PWM_Init(1000,71);    //Ƶ����1Kh
	
	while(1)
	{
		if(star==1)
		{
				if(ms1>=100) 
				{	
					 indata=dt/260.0*10;
					 dt=0;	  //������ת�٣�תÿ�룩
					 ms1=0;
					 ms100++;
					 Pid_Calc(Set_Speed,indata);    //��������ռ�ձ�
					 TIM_SetCompare1(TIM3,pid_dat);	//pidִ��												 
					 if(indata-Set_Speed<=4.6&&indata-Set_Speed>=-4.6)
					 {
							 y_2=y_1;
							 y_1=(u16)(125-(indata-(Set_Speed-4.6))*5);			
							 LCD_DrawWave(x,y_1,x-1,y_2);   
							 x++;                   //��ʾת�ٲ���
							 if(x>=125)
							 {
									x=6;
									Clear_WaveA();   //�������
							 }
					 }
				}
				if(ms100>=10)
				{
						ms100=0;
				   	my_printf_lcd(80,0,"%f",indata);	//ÿ��ˢ��һ��ת��	  
				}
			  indata=0;  //ת�����㣬׼���´�ѭ��
		}
		else if(star==5||star==6)
		{		
				S_Scan();	  //ɨ��USRT
		}
		else if(star==3||star==0)
		{ //���ֹͣ����
			TIM_SetCompare1(TIM3,0); 
			TIM_SetCompare3(TIM2,0);//�رյ��
			delay_ms(1000);   //��ʱ1s
			dt=y_1=y_2=0;     //�������������
			if(star==3)
			{ //��ǰ״̬Ϊ�Ƕȿ���
				pid_dat=190;   //ռ�ձȳ�ʼ��
			  star=5;	       //�������B�޸�״̬
			}
			else
				star=6;	       //�������A�޸�״̬
		}
		else if(star==4)
		{   //���ýǶ�ִ��
			if(ms1>=50) 
			{	
					indata=(dt-dat)/260.0*10;
				  dat=dt;	  //������ת�٣�תÿ�룩
				  ms1=0;    //��������
				  Pid_Calc(0.1,indata);         //PID����
					TIM_SetCompare1(TIM3,pid_dat);//pidִ��						
					y_2=y_1;                      //����������ϴ�����
					y_1=(u16)(125-dt/260.0*360.0/Set_Angle*85.0);	
																				//��������
					LCD_DrawWave(x,y_1,x-1,y_2);   
					x++;                   //��ʾ����
					if(x>=125)
					{
							x=6;
							Clear_WaveB();   //�������
					}
					Now_Angle=dt/260.0*360; 
					my_printf_lcd(80,0,"%d",Now_Angle);//���㲢��ʾ��ת�Ƕ�
			  }
				if(dt>=(u16)(Set_Angle*260/360)-2)
				{   //�����趨�Ƕ�
						TIM_SetCompare1(TIM3,0);
						TIM_SetCompare3(TIM2,190);//��ֹ���ת��
						y_2=y_1;
						y_1=(u16)(125-dt/260.0*360.0/Set_Angle*85.0);			
						LCD_DrawWave(x,y_1,x-1,y_2); 
						x++;              //��ʾ����
						Now_Angle=(dt+2)/260.0*360;
						LCD_ShowNum(80,0,Now_Angle,3,RED);//���㲢��ʾ��ת�Ƕ�			
						delay_ms(500);
						TIM_SetCompare3(TIM2,0);//��ʱ0.5���ر�
						dt=0;
						dat=0;
						star=3;    //�Ƕ���������
				}
			  indata=0; 			//ת�����㣬׼���´�ѭ��
		}  
		KeyRead();  //ɨ�谴��
	}
}

