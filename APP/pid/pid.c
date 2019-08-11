#include "pid.h"
#include "input.h"
#include "stdlib.h"
#include "lcd.h"
#include "usart.h"
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
	pid.Kd=5;
	pid.Ki=5;
	pid.Kp=15;
	pid.OUT=0.0;
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
	 if(s[29]=='o')//��⵽��Ҫ�޸Ĳ���
	 {
			for(i=0;i<25;i++)
			{
				 if(s[i]=='p'||s[i]=='i'||s[i]=='d'||s[i]=='e')
				 switch(s[i])
				 {
						case 'p':pid.Kp=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(28,40,pid.Kp,4,RED);break;
									//�޸�kp
						case 'i':pid.Ki=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(4,60,pid.Ki,4,RED);break;
									//�޸�ki
						case 'd':pid.Kd=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(76,60,pid.Kd,4,RED);break;
					       	//�޸�kd
						case 'e':Set_Speed=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(80,20,Set_Speed,4,RED);break;
									//�޸��趨ת��
				 }
			}
			printf("OK");
			memset(s,0,sizeof(s));//�������㣬�Ա��´�ʹ��
			b=0;
	 }
	 for(i=0;i<29;i++)
		 if(s[i]=='o')
		 {
				s[29]='o';//��⵽���ڷ��������ݺ������ĩλ���Ϊo��100ms���޸Ĳ���
				 break;
		 }

}


