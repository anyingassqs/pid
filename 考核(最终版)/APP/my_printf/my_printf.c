#include "my_printf.h"
#include "lcd.h"

/*******************************************************************************
* 函数功能		   ：通过USRT输出一个字符
*******************************************************************************/
void printch_usart(const char ch)
{ 
	putchar(ch);//输出一个字符
}

/*******************************************************************************
* 函数功能		   ：通过LCD输出一个字符
*******************************************************************************/

void printch_lcd(const char ch,u16 x,u16 y)
{ 
	LCD_ShowChar(x,y,ch,0,DARKBLUE);//输出一个字符
}

/*******************************************************************************
* 函数功能		   ：通过LCD输出一个整数
*******************************************************************************/

void printint_lcd(const int dec,u16 x,u16 y)
{    
	LCD_ShowNum(x,y,dec,3,RED);//LCD输出一个整数
}

/*******************************************************************************
* 函数功能		   ：通过USRT输出一个整数
*******************************************************************************/

void printint_usart(const int dec)
{    
	if(dec == 0)    
	{       
		return;    
	}    
	printint_usart(dec / 10);   
	putchar((u8)(dec % 10 + '0'));//USRT输出一个整数
	
}

/*******************************************************************************
* 函数功能		   ：通过USRT输出字符串
*******************************************************************************/

void printstr_usart(const char *ptr)
{   
	while(*ptr) 
	{      
		putchar(*ptr);	
		ptr++; 
	}     //USRT输出字符串
}

/*******************************************************************************
* 函数功能		   ：通过LCD输出字符串
*******************************************************************************/

void printstr_lcd(const char *ptr,u16 x,u16 y)
{ 
  
	  while(*ptr!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*ptr,0,DARKBLUE);
        x+=8;
        ptr++;
    }  //LCD输出字符串
}

/*******************************************************************************
* 函数功能		   ：通过LCD输出小数
*******************************************************************************/

void printfloat_lcd(const float flt,u16 x,u16 y)
{  
	int tmpint = (int)flt;    
	int tmpflt = (int)(1000 * (flt - tmpint)); 
	float tmp=0;
	if(tmpflt % 10 >= 5)    
	{        
		tmpflt = tmpflt / 10 + 1;   
	}  
	else    
	{ 
		tmpflt = tmpflt / 10;    //第三位小数四舍五入
	}   
	tmp=tmpint+tmpflt/100.0;
  LCD_ShowNum1(x,y,tmp,4,RED);//LCD输出小数
}
 
/*******************************************************************************
* 函数功能		   ：通过USRT输出小数
*******************************************************************************/

void printfloat_usart(const float flt)
{  
	int tmpint = (int)flt;    
	int tmpflt = (int)(1000 * (flt - tmpint)); 
	if(tmpflt % 10 >= 5)    
	{        
		tmpflt = tmpflt / 10 + 1;   
	}  
	else    
	{ 
		tmpflt = tmpflt / 10;    
	}    //第三位小数四舍五入
    printint_usart(tmpint);  
    putchar('.');  
    printint_usart(tmpflt);//USRT输出小数
}

/*******************************************************************************
* 函数功能		   ：通过USRT实现printf输出
*******************************************************************************/

void my_printf_usart(const char *format,...)
{   
	
	 va_list ap; 
	 va_start(ap,format); 
	 while(*format)   
	 {        
			 if(*format != '%')	
			 { 
					 putchar(*format);
					 format++;//如果没有‘%’直接输出
			 }	
			 else	
			 {	
					 format++;	
					 switch(*format)	
					 {	        
							case 'c':	     
							{	           
								 char valch = va_arg(ap,int);
								 printch_usart(valch);		   
								 format++;		//输出一个字符
								 break;	       
							}	       
							case 'd':	       
							 {	   
									int valint = va_arg(ap,int);	
									printint_usart(valint);		   
									format++;		//输出整数    
									break;	        
							 }
							case 's':	
							{		    
									char *valstr = va_arg(ap,char *);
									printstr_usart(valstr);		    
									format++;		 //输出字符串
									break;	
							}
							case 'f':	
							{		  
									 float valflt = va_arg(ap,double);	
									 printfloat_usart(valflt);		   
									 format++;		//输出一个小数
									 break;		
							}	
							default:	
							{		 
									printch_usart(*format);
									format++;  //都不是则输出‘%’
									break;	
							}	   
					}	   
						
			 } 	
   } 
	 va_end(ap);  //结束
}
 
/*******************************************************************************
* 函数功能		   ：通过lcd实现printf输出
*******************************************************************************/

void my_printf_lcd(u16 x,u16 y,const char *format,...)
{  
  int flag=0;
  char tmp[2]={0};	
	va_list ap; 
	va_start(ap,format); 
  while(*format)   
	{        
		 if(*format != '%')	
		 {  
				 if(*format == '\n')
				 {
						x=4;
						y+=20;
					  format++;//lcd换行
				 }
				 else if(*format & 0x80)
				 {   //如果为中文
						tmp[0] =(char)(*format);
						format++;
						tmp[1] =(char)(*format);
						format++;
						flag=(((int)(tmp[1])<<8)+tmp[0]);
						//计算其AS码
						LCD_ShowChinese1(x,y,16,GBLUE,flag);
					  //显示该字符
						x+=16;
				 }
				 else
				 {
						 LCD_ShowChar(x,y,*format,0,DARKBLUE);
						 format++;
						 x=x+8;//否则则直接输出
				 }
		 }	
     else	
		 {	
	        format++;	
	        switch(*format)	
			    {	        
	           case 'c':	     
					   {	           
			          char valch = va_arg(ap,int);
			           printch_lcd(valch,x,y);		   
			           format++;		
								 x=x+8;  //输出一个字符
			            break;	       
					   }	       
	           case 'd':	       
					   {	   
			            int valint = va_arg(ap,int);	
			            printint_lcd(valint,x,y);		   
			            format++;	
								  x=x+8;	//输出一个整数 
			            break;	        
					   }
				     case 's':	
					   {		    
			            char *valstr = va_arg(ap,char *);
			            printstr_lcd(valstr,x,y);		    
			            format++;	
									x=x+8;  //输出一个字符串
			            break;	
					   }
			       case 'f':	
					   {		  
			             float valflt = va_arg(ap,double);	
			              printfloat_lcd(valflt,x,y);		   
			             format++;	
									 x=x+8*5; //输出一个小数
			             break;		
					   }	
				     default:	
					   {		 
			            printch_lcd(*format,x,y);
			            format++;
								  x=x+8*5; //都不是则输出‘%’
						      break;	
					   }	   
			    }	   
				 	
		  } 	
	 } 
	 va_end(ap);//结束
}

