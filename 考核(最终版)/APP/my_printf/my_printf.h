#ifndef _my_printf_H
#define _my_printf_H

#include "system.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void my_printf_usart(const char *format,...);
void my_printf_lcd(u16 x,u16 y,const char *format,...);

#endif
