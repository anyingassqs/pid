#include "pid.h"
#include "input.h"
#include "stdlib.h"
#include "lcd.h"
#include "usart.h"
PID pid;
float pid_dat=0.0;
/*******************************************************************************
* 函数功能		   : PID相关参数初始化函数
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
* 函数功能		   : PID算法实现函数
*******************************************************************************/

void Pid_Calc(float Speed,float NewSpeed)
{
	 pid.SS=Speed;   //设置转速
	 pid.AS=NewSpeed;//实际转速
	 pid.Err=pid.SS-pid.AS;//本次偏差
	 pid.OUT=pid.Kp*(pid.Err-pid.Err_1)+pid.Ki*pid.Err+
	        pid.Kd*(pid.Err_2+pid.Err-2*pid.Err_1);//PID输出值
	 pid.Err_2=pid.Err_1;  //上次偏差变上上次偏差
	 pid.Err_1=pid.Err;    //本次偏差变上次偏差
	 pid_dat+=pid.OUT;     //PWM占空比
	 if(pid_dat>1000)      
	 {
		  pid_dat=1000;      //最大1000
	 }
	 else if(pid_dat<0)
	 {
		  pid_dat=0;         //最小0
	 }
}	

/*******************************************************************************
* 函数功能		   : 通过串口改变PID的参数函数
*******************************************************************************/

void S_Scan(void)
{
	 u8 i;
	 if(s[29]=='o')//检测到需要修改参数
	 {
			for(i=0;i<25;i++)
			{
				 if(s[i]=='p'||s[i]=='i'||s[i]=='d'||s[i]=='e')
				 switch(s[i])
				 {
						case 'p':pid.Kp=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(28,40,pid.Kp,4,RED);break;
									//修改kp
						case 'i':pid.Ki=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(4,60,pid.Ki,4,RED);break;
									//修改ki
						case 'd':pid.Kd=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(76,60,pid.Kd,4,RED);break;
					       	//修改kd
						case 'e':Set_Speed=(s[i+1]-'0')*10.0+(s[i+2]-'0')+(s[i+4]-'0')*0.1+
								 (s[i+5]-'0')*0.01;LCD_ShowNum1(80,20,Set_Speed,4,RED);break;
									//修改设定转速
				 }
			}
			printf("OK");
			memset(s,0,sizeof(s));//数组清零，以备下次使用
			b=0;
	 }
	 for(i=0;i<29;i++)
		 if(s[i]=='o')
		 {
				s[29]='o';//检测到串口发来的数据后把数组末位标记为o，100ms后修改参数
				 break;
		 }

}


