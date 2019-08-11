#ifndef _pid_H
#define _pid_H

#include "system.h"
#include <string.h>

typedef struct
{
  float SS;//设置速度
  float AS;//实际速度
 
  float Kp;  
  float Ki;
  float Kd; 
	
	
	
  float Err;  
  float Err_1;
	float Err_2;
  float SEk; 
	
	float voltage;
	float integral;
	float Iout;
	float Pout;
	float Dout;
	
  float OUT0;

  float OUT;
	
	
  u16 C10ms;
	
  u16 pwmcycle;
	
}PID;
extern PID pid;
void Pid_Calc(float Speed,float NewSpeed);
void Pid_Init(void);
void S_Scan(void);
extern float pid_dat;
extern float Set_Speed;
#endif
