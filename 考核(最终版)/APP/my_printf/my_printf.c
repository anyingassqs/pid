#include "my_printf.h"
#include "lcd.h"

/*******************************************************************************
* ��������		   ��ͨ��USRT���һ���ַ�
*******************************************************************************/
void printch_usart(const char ch)
{ 
	putchar(ch);//���һ���ַ�
}

/*******************************************************************************
* ��������		   ��ͨ��LCD���һ���ַ�
*******************************************************************************/

void printch_lcd(const char ch,u16 x,u16 y)
{ 
	LCD_ShowChar(x,y,ch,0,DARKBLUE);//���һ���ַ�
}

/*******************************************************************************
* ��������		   ��ͨ��LCD���һ������
*******************************************************************************/

void printint_lcd(const int dec,u16 x,u16 y)
{    
	LCD_ShowNum(x,y,dec,3,RED);//LCD���һ������
}

/*******************************************************************************
* ��������		   ��ͨ��USRT���һ������
*******************************************************************************/

void printint_usart(const int dec)
{    
	if(dec == 0)    
	{       
		return;    
	}    
	printint_usart(dec / 10);   
	putchar((u8)(dec % 10 + '0'));//USRT���һ������
	
}

/*******************************************************************************
* ��������		   ��ͨ��USRT����ַ���
*******************************************************************************/

void printstr_usart(const char *ptr)
{   
	while(*ptr) 
	{      
		putchar(*ptr);	
		ptr++; 
	}     //USRT����ַ���
}

/*******************************************************************************
* ��������		   ��ͨ��LCD����ַ���
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
    }  //LCD����ַ���
}

/*******************************************************************************
* ��������		   ��ͨ��LCD���С��
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
		tmpflt = tmpflt / 10;    //����λС����������
	}   
	tmp=tmpint+tmpflt/100.0;
  LCD_ShowNum1(x,y,tmp,4,RED);//LCD���С��
}
 
/*******************************************************************************
* ��������		   ��ͨ��USRT���С��
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
	}    //����λС����������
    printint_usart(tmpint);  
    putchar('.');  
    printint_usart(tmpflt);//USRT���С��
}

/*******************************************************************************
* ��������		   ��ͨ��USRTʵ��printf���
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
					 format++;//���û�С�%��ֱ�����
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
								 format++;		//���һ���ַ�
								 break;	       
							}	       
							case 'd':	       
							 {	   
									int valint = va_arg(ap,int);	
									printint_usart(valint);		   
									format++;		//�������    
									break;	        
							 }
							case 's':	
							{		    
									char *valstr = va_arg(ap,char *);
									printstr_usart(valstr);		    
									format++;		 //����ַ���
									break;	
							}
							case 'f':	
							{		  
									 float valflt = va_arg(ap,double);	
									 printfloat_usart(valflt);		   
									 format++;		//���һ��С��
									 break;		
							}	
							default:	
							{		 
									printch_usart(*format);
									format++;  //�������������%��
									break;	
							}	   
					}	   
						
			 } 	
   } 
	 va_end(ap);  //����
}
 
/*******************************************************************************
* ��������		   ��ͨ��lcdʵ��printf���
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
					  format++;//lcd����
				 }
				 else if(*format & 0x80)
				 {   //���Ϊ����
						tmp[0] =(char)(*format);
						format++;
						tmp[1] =(char)(*format);
						format++;
						flag=(((int)(tmp[1])<<8)+tmp[0]);
						//������AS��
						LCD_ShowChinese1(x,y,16,GBLUE,flag);
					  //��ʾ���ַ�
						x+=16;
				 }
				 else
				 {
						 LCD_ShowChar(x,y,*format,0,DARKBLUE);
						 format++;
						 x=x+8;//������ֱ�����
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
								 x=x+8;  //���һ���ַ�
			            break;	       
					   }	       
	           case 'd':	       
					   {	   
			            int valint = va_arg(ap,int);	
			            printint_lcd(valint,x,y);		   
			            format++;	
								  x=x+8;	//���һ������ 
			            break;	        
					   }
				     case 's':	
					   {		    
			            char *valstr = va_arg(ap,char *);
			            printstr_lcd(valstr,x,y);		    
			            format++;	
									x=x+8;  //���һ���ַ���
			            break;	
					   }
			       case 'f':	
					   {		  
			             float valflt = va_arg(ap,double);	
			              printfloat_lcd(valflt,x,y);		   
			             format++;	
									 x=x+8*5; //���һ��С��
			             break;		
					   }	
				     default:	
					   {		 
			            printch_lcd(*format,x,y);
			            format++;
								  x=x+8*5; //�������������%��
						      break;	
					   }	   
			    }	   
				 	
		  } 	
	 } 
	 va_end(ap);//����
}

