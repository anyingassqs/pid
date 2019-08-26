#include "key.h"
#include "lcd.h"
#include "pid.h"
#include "eeprom.h"
#include "my_printf.h"
u16 ms1=0;
u16 Cursor=0;
u8 star=1;
u8 KeyBuff[4][3]={0};
u8 KeySta[4][3]={0};
u8 KayMap[4][3]={{1,2,3},{4,5,6},{7,8,9},{10,0,12}};

/*******************************************************************************
* ��������		   ��������ʼ������
*******************************************************************************/
void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=Key_PIN1;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOF,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin=Key_PIN2;  //ѡ�����õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */

}

/*******************************************************************************
* ��������		   ��TIM4��ʼ������
*******************************************************************************/

void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}

/*******************************************************************************
* ��������		   �����ð������ڹ�꺯��
*******************************************************************************/

void Cursor_Set(u8 n)
{
	if(n==12)
	{
		if(Cursor==0)  //���Ҽ����������
		{
			star=0;  
			Cursor=1;
		}
		else
		{
			switch(Cursor)
			{
				case 1:Cursor=2;LCD_DrawLine(80,38,88,38,BLACK);
							 Clear_WaveA();break;
				case 2:Cursor=3;LCD_DrawLine(88,38,96,38,BLACK);
							 LCD_DrawLine(80,38,88,38,WHITE);break;
				case 3:Cursor=4;LCD_DrawLine(104,38,112,38,BLACK);
				       LCD_DrawLine(88,38,96,38,WHITE);break;
				case 4:Cursor=5;LCD_DrawLine(112,38,120,38,BLACK);
				       LCD_DrawLine(104,38,112,38,WHITE);break;
				       //��ʾ��ǰת������λ�ù��
				case 5:Cursor=6;LCD_DrawLine(28,58,36,58,BLACK);
				       LCD_DrawLine(112,38,120,38,WHITE);break;
				case 6:Cursor=7;LCD_DrawLine(36,58,44,58,BLACK);
				       LCD_DrawLine(28,58,36,58,WHITE);break;
				case 7:Cursor=8;LCD_DrawLine(52,58,60,58,BLACK);
				       LCD_DrawLine(36,58,44,58,WHITE);break;
				case 8:Cursor=9;LCD_DrawLine(60,58,68,58,BLACK);
				       LCD_DrawLine(52,58,60,58,WHITE);break;
				       //��ʾKP����λ�ù��
				case 9:Cursor=10;LCD_DrawLine(4,78,12,78,BLACK);
				       LCD_DrawLine(60,58,68,58,WHITE);break;
				case 10:Cursor=11;LCD_DrawLine(12,78,20,78,BLACK);
				       LCD_DrawLine(4,78,12,78,WHITE);break;
				case 11:Cursor=12;LCD_DrawLine(28,78,36,78,BLACK);
				       LCD_DrawLine(12,78,20,78,WHITE);break;
				case 12:Cursor=13;LCD_DrawLine(36,78,44,78,BLACK);
				       LCD_DrawLine(28,78,36,78,WHITE);break;
				       //��ʾKI����λ�ù��
				case 13:Cursor=14;LCD_DrawLine(76,78,84,78,BLACK);
				       LCD_DrawLine(36,78,44,78,WHITE);break;
				case 14:Cursor=15;LCD_DrawLine(84,78,92,78,BLACK);
				       LCD_DrawLine(76,78,84,78,WHITE);break;
				case 15:Cursor=16;LCD_DrawLine(100,78,108,78,BLACK);
				       LCD_DrawLine(84,78,92,78,WHITE);break;
				case 16:Cursor=17;LCD_DrawLine(108,78,116,78,BLACK);
				       LCD_DrawLine(100,78,108,78,WHITE);break;
				       //��ʾKD����λ�ù��
				case 17:Cursor=0;star=1;LCD_DrawLine(108,78,116,78,WHITE);
					     my_printf_lcd(4,40,"KP=%f KI=\n%f KD=%f",pid.Kp,pid.Ki,pid.Kd);
							 break;						
				case 20:Cursor=21;LCD_DrawLine(80,38,88,38,BLACK);
				        Clear_WaveB();break;
				case 21:Cursor=22;LCD_DrawLine(88,38,96,38,BLACK);
			        	LCD_DrawLine(80,38,88,38,WHITE);break;
				case 22:Cursor=23;LCD_DrawLine(96,38,104,38,BLACK);
				        LCD_DrawLine(88,38,96,38,WHITE);break;
								//��ʾ���ýǶ�����λ�ù��
				case 23:Cursor=20;star=4;LCD_DrawLine(96,38,104,38,WHITE);break;
			}
		}
	}
	else   //��������������
	{
		  switch(Cursor)
			{
				
				case 0:star=3;Cursor=20;LCD_ShowB();break;
							
				case 1:Cursor=0;star=1;
							 my_printf_lcd(80,20,"%f\nKP=%f KI=\n%f KD=%f",Set_Speed,pid.Kp,pid.Ki,pid.Kd);
				       break;
				case 2:Cursor=1;LCD_DrawLine(80,38,88,38,WHITE);break;
				case 3:Cursor=2;LCD_DrawLine(88,38,96,38,WHITE);
				       LCD_DrawLine(80,38,88,38,BLACK);break;
				case 4:Cursor=3;LCD_DrawLine(104,38,112,38,WHITE);
				       LCD_DrawLine(88,38,96,38,BLACK);break;				
				case 5:Cursor=4;LCD_DrawLine(112,38,120,38,WHITE);
				       LCD_DrawLine(104,38,112,38,BLACK);break;
				       //��ʾ��ǰת������λ�ù��
				case 6:Cursor=5;LCD_DrawLine(28,58,36,58,WHITE);
				       LCD_DrawLine(112,38,120,38,BLACK);break;//-50
				case 7:Cursor=6;LCD_DrawLine(36,58,44,58,WHITE);
				       LCD_DrawLine(28,58,36,58,BLACK);break;
				case 8:Cursor=7;LCD_DrawLine(52,58,60,58,WHITE);
				       LCD_DrawLine(36,58,44,58,BLACK);break;			
				case 9:Cursor=8;LCD_DrawLine(60,58,68,58,WHITE);
				       LCD_DrawLine(52,58,60,58,BLACK);break;
				       //��ʾKP����λ�ù��
				case 10:Cursor=9;LCD_DrawLine(4,78,12,78,WHITE);
				       LCD_DrawLine(60,58,68,58,BLACK);break;
				case 11:Cursor=10;LCD_DrawLine(12,78,20,78,WHITE);
			       	 LCD_DrawLine(4,78,12,78,BLACK);break;
				case 12:Cursor=11;LCD_DrawLine(28,78,36,78,WHITE);
				       LCD_DrawLine(12,78,20,78,BLACK);break;			
				case 13:Cursor=12;LCD_DrawLine(36,78,44,78,WHITE);
				       LCD_DrawLine(28,78,36,78,BLACK);break;
				        //��ʾKI����λ�ù��
				case 14:Cursor=13;LCD_DrawLine(76,78,84,78,WHITE);
				       LCD_DrawLine(36,78,44,78,BLACK);break;
				case 15:Cursor=14;LCD_DrawLine(84,78,92,78,WHITE);
				       LCD_DrawLine(76,78,84,78,BLACK);break;
				case 16:Cursor=15;LCD_DrawLine(100,78,108,78,WHITE);
				       LCD_DrawLine(84,78,92,78,BLACK);break;				
				case 17:Cursor=16;LCD_DrawLine(108,78,116,78,WHITE);
				       LCD_DrawLine(100,78,108,78,BLACK);break;
							  //��ʾKD����λ�ù��
				case 20:star=1;Cursor=0;LCD_ShowA();break;
				        //��ʾA����
				case 21:Cursor=20;star=4;LCD_DrawLine(80,38,88,38,WHITE);break;
				case 22:Cursor=21;LCD_DrawLine(88,38,96,38,WHITE);
				       LCD_DrawLine(80,38,88,38,BLACK);break;
				case 23:Cursor=22;LCD_DrawLine(96,38,104,38,WHITE);
				       LCD_DrawLine(88,38,96,38,BLACK);break;
				       //��ʾ���ýǶ�����λ�ù��
			}
	}
}

/*******************************************************************************
* ��������		   �������ı���ֵ����
*******************************************************************************/

void Dat_Set(u8 n1)
{
	 switch(Cursor)
	 {
		 case 2:Set_Speed=Set_Speed-(u16)(Set_Speed/10)*10+n1*10;
		        my_printf_lcd(80,20,"%f",Set_Speed);
		        Eeprom_WriteLenByte(6,Set_Speed*100,2);break;
		 case 3:Set_Speed=Set_Speed-(u16)(Set_Speed)%10+n1;
		        my_printf_lcd(80,20,"%f",Set_Speed);
		        Eeprom_WriteLenByte(6,Set_Speed*100,2);break;		
		 case 4:Set_Speed=Set_Speed-(u16)(Set_Speed*10)%10/10.0+n1/10.0;
		        my_printf_lcd(80,20,"%f",Set_Speed);
		        Eeprom_WriteLenByte(6,Set_Speed*100,2);break;		 
		 case 5:Set_Speed=Set_Speed-(u16)(Set_Speed*100)%10/100.0+n1/100.0;
		        my_printf_lcd(80,20,"%f",Set_Speed);
		        Eeprom_WriteLenByte(6,Set_Speed*100,2);break; 	
		        //�ı��趨ת��		 
		 case 6:pid.Kp=pid.Kp-(u16)(pid.Kp/10)*10+n1*10;
		        my_printf_lcd(28,40,"%f",pid.Kp);
		        Eeprom_WriteLenByte(0,pid.Kp*100,2);break; 
		 case 7:pid.Kp=pid.Kp-(u16)(pid.Kp)%10+n1;
		        my_printf_lcd(28,40,"%f",pid.Kp);
		        Eeprom_WriteLenByte(0,pid.Kp*100,2);break;		
		 case 8:pid.Kp=pid.Kp-(u16)(pid.Kp*10)%10/10.0+n1/10.0;
		        my_printf_lcd(28,40,"%f",pid.Kp);
						Eeprom_WriteLenByte(0,pid.Kp*100,2);break;	
		 case 9:pid.Kp=pid.Kp-(u16)(pid.Kp*100)%10/100.0+n1/100.0;
		        my_printf_lcd(28,40,"%f",pid.Kp);
						Eeprom_WriteLenByte(0,pid.Kp*100,2);break;
		        //�ı�KP��ֵ
		 case 10:pid.Ki=pid.Ki-(u16)(pid.Ki/10)*10+n1*10;
		        my_printf_lcd(4,60,"%f",pid.Ki);
						Eeprom_WriteLenByte(2,pid.Ki*100,2);break; 
		 case 11:pid.Ki=pid.Ki-(u16)(pid.Ki)%10+n1;
		        my_printf_lcd(4,60,"%f",pid.Ki);
						Eeprom_WriteLenByte(2,pid.Ki*100,2);break;				 
		 case 12:pid.Ki=pid.Ki-(u16)(pid.Ki*10)%10/10.0+n1/10.0;
		        my_printf_lcd(4,60,"%f",pid.Ki);
						Eeprom_WriteLenByte(2,pid.Ki*100,2);break;	
		 case 13:pid.Ki=pid.Ki-(u16)(pid.Ki*100)%10/100.0+n1/100.0;
		        my_printf_lcd(4,60,"%f",pid.Ki);
					  Eeprom_WriteLenByte(2,pid.Ki*100,2);break;
		        //�ı�KI��ֵ
		 case 14:pid.Kd=pid.Kd-(u16)(pid.Kd/10)*10+n1*10;
		        my_printf_lcd(76,60,"%f",pid.Kd);
						Eeprom_WriteLenByte(4,pid.Kd*100,2);break;						
		 case 15:pid.Kd=pid.Kd-(u16)(pid.Kd)%10+n1;
		        my_printf_lcd(76,60,"%f",pid.Kd);
						Eeprom_WriteLenByte(4,pid.Kd*100,2);break;						
		 case 16:pid.Kd=pid.Kd-(u16)(pid.Kd*10)%10/10.0+n1/10.0;
		        my_printf_lcd(76,60,"%f",pid.Kd);
						Eeprom_WriteLenByte(4,pid.Kd*100,2);break;					
		 case 17:pid.Kd=pid.Kd-(u16)(pid.Kd*100)%10/100.0+n1/100.0;
		        my_printf_lcd(76,60,"%f",pid.Kd);
						Eeprom_WriteLenByte(4,pid.Kd*100,2);break;		
		        //�ı�KD��ֵ		
		 case 21:Set_Angle=Set_Angle-(u16)(Set_Angle/100)*100+n1*100;
		         my_printf_lcd(80,20,"%d",Set_Angle);
						 Eeprom_WriteLenByte(8,Set_Angle,2);break;
		 case 22:Set_Angle=Set_Angle-(u16)(Set_Angle/10)%10*10+n1*10;
		         my_printf_lcd(80,20,"%d",Set_Angle);
						 Eeprom_WriteLenByte(8,Set_Angle,2);break;
		 case 23:Set_Angle=Set_Angle-Set_Angle%10+n1;
		         my_printf_lcd(80,20,"%d",Set_Angle);
						 Eeprom_WriteLenByte(8,Set_Angle,2);break;
		         //�ı��趨�Ƕ�
	 }
}

/*******************************************************************************
* ��������		   ��ѡ�񰴼����ܺ���
*******************************************************************************/

void KeyDo(u8 dat)
{
	if(dat==12||dat==10)
	{
		switch(dat)
		{
			case 12:Cursor_Set(12);break;
			case 10:Cursor_Set(10);break;
		}
	}
	else
	Dat_Set(dat);
}

/*******************************************************************************
* ��������		   ����ȡ����״̬����
*******************************************************************************/

void KeyRead()
{
	u8 i=0,j=0;
	static u8 backup[4][3]={0};
	for(i=0;i<=3;i++)
	  for(j=0;j<=2;j++)
		{
			if(KeySta[i][j]!=backup[i][j])
		  {
				 if(KeySta[i][j]==1)	      
			   {	
           KeyDo(KayMap[i][j]);		//��⵽�а�������			
				}
				backup[i][j]=KeySta[i][j]; 
			}	
		}
}

/*******************************************************************************
* ��������		   ������ɨ�躯��
*******************************************************************************/

void Keyscan()
{
	static u8 i=0;
	static u8 j=0;
	KeyBuff[i][0]=(KeyBuff[i][0]<<1)|key3;
	KeyBuff[i][1]=(KeyBuff[i][1]<<1)|key2;
	KeyBuff[i][2]=(KeyBuff[i][2]<<1)|key1;
	for(j=0;j<3;j++)
		{
			if((KeyBuff[i][j]&0x0F)==0x0F)
			{
				KeySta[i][j]=1;//��������
			}
			else if((KeyBuff[i][j]&0x0F)==0x00)
			{
				KeySta[i][j]=0;//��������
			}
		}
	i++;
  i&=0x03;
	switch(i)
	{
	   case 0:key4=0;key7=1;break;
		 case 1:key7=0;key6=1;break;
		 case 2:key6=0;key5=1;break;
		 case 3:key5=0;key4=1;break;
	}	
}

/*******************************************************************************
* ��������		   ��TIM4�жϺ���
*******************************************************************************/

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{		
    ms1+=2;		
		Keyscan();//����ɨ�裬�������Ӷ�
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}




