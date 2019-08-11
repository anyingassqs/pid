#ifndef _input_H
#define _input_H

#include "system.h"


extern u16 ms1;
//extern u8 TIM5_CH1_CAPTURE_STA; //输入捕获的状态
//extern u16 TIM5_CH1_CAPTURE_VAL;//输入捕获值
extern u16 dt;


void TIM5_CH1_Input_Init(u16 arr,u16 psc);
	
#endif
