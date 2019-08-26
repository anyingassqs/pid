#ifndef _key_H
#define _key_H

#include "system.h"

extern float Set_Speed;
extern u8 star;
void Key_Init(void);
void KeyRead(void);
void Keyscan(void);
void TIM4_Init(u16 per,u16 psc);

#define Key_PORT 			GPIOF|GPIOE   
#define Key_PIN1 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2)
#define Key_PIN2 			(GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5)

#define key1 PFin(0)
#define key2 PFin(1)
#define key3 PFin(2)
#define key4 PEout(5)
#define key5 PEout(4)
#define key6 PEout(3)
#define key7 PEout(2)

#endif
