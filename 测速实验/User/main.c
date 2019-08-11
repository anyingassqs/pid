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
* ��������		   : ������
*******************************************************************************/
float Set_Speed=25.0;//���õ��ٶ�
u16 x=6;             //����ͼ��ʼx����
int main()
{
	float indata=0.0;
	u8 ms100=0;       //100ms
	u16 y_1=0,y_2=0;  //����ͼ����Y������ϴ�y����
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  //�ж����ȼ����� ��2��
	LED_Init();
  USART1_Init(9600);
	TIM5_CH1_Input_Init(1000,71);  //��1MƵ�ʼ���

	TIM3_CH1_PWM_Init(1000,72-1);  //Ƶ����1Kh
  TIM4_Init(2000,72-1);   //2msˢ��һ��
	Pid_Init();      //��ʼ��PID
	Lcd_Init();			 //��ʼ��OLED  
	LCD_Clear(WHITE);
	BACK_COLOR=WHITE;//����

	LCD_Star();
	LCD_ShowNum1(80,20,Set_Speed,4,RED);
	Key_Init();
  LCD_DrawLine(4,80,4,125,YELLOW);
	LCD_DrawLine(4,103,125,103,YELLOW);
	LCD_ShowNum1(28,40,pid.Kp,4,RED);
	LCD_ShowNum1(4,60,pid.Ki,4,RED);
	LCD_ShowNum1(76,60,pid.Kd,4,RED);//lcd��ʾA����

	while(1)
	{
		if(ms1>=100) 
		{	
			 indata=dt/334.0*10;
		   dt=0;	  //������ת�٣�תÿ�룩
			 ms1=0;
			 ms100++;
			 if(star==1)
		   {
		       Pid_Calc(Set_Speed,indata); 
		       TIM_SetCompare1(TIM3,pid_dat);	//pid���㲢ִ��								
		   }
			 else
		   {
			    TIM_SetCompare1(TIM3,0); //���ֹͣת��
		   }
			 if(indata-Set_Speed<=4.6&&indata-Set_Speed>=-4.6)
			 {
			     y_2=y_1;
			     y_1=(u16)(125-(indata-(Set_Speed-4.6))*5);			
			     LCD_DrawWave(x,y_1,x-1,y_2);   
			     x++;                   //��ʾת�ٲ���
				   if(x>=125)
				   {
					    x=6;
              Clear_Wave();   //�������
				   }
			 }
       S_Scan();	//��⴮���Ƿ�������	
	  }
		if(ms100>=10)
		{
			  ms100=0;
		    LCD_ShowNum1(80,0,indata,4,RED);	//ÿ��ˢ��һ��ת��	  
		}
		indata=0;   //ת�����㣬׼���´�ѭ��
		KeyRead();  //ɨ�谴��
	}
}

