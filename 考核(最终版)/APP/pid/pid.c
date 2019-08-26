#include "pid.h"
#include "input.h"
#include "stdlib.h"
#include "lcd.h"
#include "usart.h"
#include "SysTick.h"
#include "eeprom.h"
#include "my_printf.h"
PID pid;
float pid_dat=0.0;
/*******************************************************************************
* ��������		   : PID��ز�����ʼ������
*******************************************************************************/
void Pid_Init()
{
		pid.SS=0.0;
		pid.AS=0.0;
		pid.Err=0.0;
		pid.Err_1=0.0;
		pid.Err_2=0.0;
		pid.Kd=0.0;
		pid.Ki=0.0;
		pid.Kp=0.0;
		pid.OUT=0.0;//PID��������
}

/*******************************************************************************
* ��������		   : PID�㷨ʵ�ֺ���
*******************************************************************************/

void Pid_Calc(float Speed,float NewSpeed)
{
	 pid.SS=Speed;   //����ת��
	 pid.AS=NewSpeed;//ʵ��ת��
	 pid.Err=pid.SS-pid.AS;//����ƫ��
	 pid.OUT=pid.Kp*(pid.Err-pid.Err_1)+pid.Ki*pid.Err+
	        pid.Kd*(pid.Err_2+pid.Err-2*pid.Err_1);//PID���ֵ
	 pid.Err_2=pid.Err_1;  //�ϴ�ƫ������ϴ�ƫ��
	 pid.Err_1=pid.Err;    //����ƫ����ϴ�ƫ��
	 pid_dat+=pid.OUT;     //PWMռ�ձ�
	 if(pid_dat>1000)      
	 {
		  pid_dat=1000;      //���1000
	 }
	 else if(pid_dat<0)
	 {
		  pid_dat=0;         //��С0
	 }
}	

/*******************************************************************************
* ��������		   : ͨ�����ڸı�PID�Ĳ�������
*******************************************************************************/

void S_Scan(void)
{
	 u8 i;
	 for(i=0;i<10;i++)
	 if(s[i]=='o')
	 { 
			s[36]='o';//��⵽���ڷ��������ݺ������ĩλ���Ϊo��100ms���޸Ĳ���
		  delay_ms(100);
			 break;
	 }
	 if(s[36]=='o')//��⵽��Ҫ�޸Ĳ���
	 {
			for(i=0;i<37;i++)
			{
				 if(s[i]=='p'||s[i]=='i'||s[i]=='d'||s[i]=='s'||s[i]=='a')
				 {
				   switch(s[i])
				   {
							case 'p':pid.Kp=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
									 (s[i+5]-'0')*0.01;Eeprom_WriteLenByte(0,pid.Kp*100,2);break;
										//�޸�kp
							case 'i':pid.Ki=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
									 (s[i+5]-'0')*0.01;Eeprom_WriteLenByte(2,pid.Ki*100,2);break;
										//�޸�ki
							case 'd':pid.Kd=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
									 (s[i+5]-'0')*0.01;Eeprom_WriteLenByte(4,pid.Kd*100,2);break;
										//�޸�kd
							case 's':Set_Speed=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
									 (s[i+5]-'0')*0.01;Eeprom_WriteLenByte(6,Set_Speed*100,2);break;
										//�޸��趨ת��
							case 'a':Set_Angle=(s[i+1]-'0')*100+(s[i+2]-'0')*10+(s[i+3]-'0');								
							       Eeprom_WriteLenByte(8,Set_Angle,2);break;
						}
					 if(star==5)
						   my_printf_lcd(80,20,"%d",Set_Angle);
									//��ʾ����B��ز���
					 else
					 {
							 my_printf_lcd(80,20,"%f",Set_Speed);
						   my_printf_lcd(4,40,"KP=%f KI=\n%f KD=%f",pid.Kp,pid.Ki,pid.Kd);
						 //��ʾ����A��ز���
					 }
				 }
			}
			my_printf_usart("OK");
			memset(s,0,sizeof(s));//�������㣬�Ա��´�ʹ��
			b=0;
	 }
}




