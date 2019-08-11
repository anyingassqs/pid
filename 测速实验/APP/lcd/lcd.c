
#include "lcd.h"
#include "oledfont.h"
#include "SysTick.h"
#include "bmp.h"
u16 BACK_COLOR;   //����ɫ
extern u16 x;

/*******************************************************************************
* ��������		   ��LCD��������д�뺯��
*******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;
	}	
  OLED_CS_Set();	
}


/*******************************************************************************
* ��������		   ��LCDд��8λ���ݺ���
*******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	OLED_DC_Set();//д����
	LCD_Writ_Bus(dat);
}


/*******************************************************************************
* ��������		   ��LCDд��16λ���ݺ���
*******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	OLED_DC_Set();//д����
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}

/*******************************************************************************
* ��������		   ��LCDд�������
*******************************************************************************/

void LCD_WR_REG(u8 dat)
{
	OLED_DC_Clr();//д����
	LCD_Writ_Bus(dat);
}


/*******************************************************************************
* ��������		   ��LCD���ù��λ�ú���
*******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+3);
		LCD_WR_DATA(y2+3);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+2);
		LCD_WR_DATA(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+3);
		LCD_WR_DATA(x2+3);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+2);
		LCD_WR_DATA(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
}


/*******************************************************************************
* ��������		   ��LCD��ʼ������
*******************************************************************************/
void Lcd_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOG12
 	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //��ʼ��GPIOD1,5,15
 	GPIO_SetBits(GPIOD,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOE8,10
 	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	
	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();
	delay_ms(200);
	OLED_BLK_Set();
	delay_ms(200);

//************* Start Initial Sequence **********//
LCD_WR_REG(0x11); //Sleep out
delay_ms(120); //Delay 120ms
//------------------------------------ST7735S Frame rate-------------------------------------------------//
LCD_WR_REG(0xB1); //Frame rate 80Hz
LCD_WR_DATA8(0x02);
LCD_WR_DATA8(0x35);
LCD_WR_DATA8(0x36);
LCD_WR_REG(0xB2); //Frame rate 80Hz
LCD_WR_DATA8(0x02);
LCD_WR_DATA8(0x35);
LCD_WR_DATA8(0x36);
LCD_WR_REG(0xB3); //Frame rate 80Hz
LCD_WR_DATA8(0x02);
LCD_WR_DATA8(0x35);
LCD_WR_DATA8(0x36);
LCD_WR_DATA8(0x02);
LCD_WR_DATA8(0x35);
LCD_WR_DATA8(0x36);
//------------------------------------End ST7735S Frame rate-------------------------------------------//
LCD_WR_REG(0xB4); //Dot inversion
LCD_WR_DATA8(0x03);
//------------------------------------ST7735S Power Sequence-----------------------------------------//
LCD_WR_REG(0xC0);
LCD_WR_DATA8(0xA2);
LCD_WR_DATA8(0x02);
LCD_WR_DATA8(0x84);
LCD_WR_REG(0xC1);
LCD_WR_DATA8(0xC5);
LCD_WR_REG(0xC2);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x00);
LCD_WR_REG(0xC3);
LCD_WR_DATA8(0x8D);
LCD_WR_DATA8(0x2A);
LCD_WR_REG(0xC4);
LCD_WR_DATA8(0x8D);
LCD_WR_DATA8(0xEE);
//---------------------------------End ST7735S Power Sequence---------------------------------------//
LCD_WR_REG(0xC5); //VCOM
LCD_WR_DATA8(0x0a);
LCD_WR_REG(0x36);
if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
else LCD_WR_DATA8(0xA8);
//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
LCD_WR_REG(0XE0);
LCD_WR_DATA8(0x12);
LCD_WR_DATA8(0x1C);
LCD_WR_DATA8(0x10);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x2C);
LCD_WR_DATA8(0x25);
LCD_WR_DATA8(0x28);
LCD_WR_DATA8(0x28);
LCD_WR_DATA8(0x27);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x3C);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x03);
LCD_WR_DATA8(0x03);
LCD_WR_DATA8(0x10);
LCD_WR_REG(0XE1);
LCD_WR_DATA8(0x12);
LCD_WR_DATA8(0x1C);
LCD_WR_DATA8(0x10);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x2D);
LCD_WR_DATA8(0x28);
LCD_WR_DATA8(0x23);
LCD_WR_DATA8(0x28);
LCD_WR_DATA8(0x28);
LCD_WR_DATA8(0x26);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x3B);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x03);
LCD_WR_DATA8(0x03);
LCD_WR_DATA8(0x10);
//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
LCD_WR_REG(0x3A); //65k mode
LCD_WR_DATA8(0x05);
LCD_WR_REG(0x29); //Display on 
} 


/*******************************************************************************
* ��������		   ��LCD��������
*******************************************************************************/
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	  {
			for (j=0;j<LCD_H;j++)
				{
					LCD_WR_DATA(Color);
				}
	  }
}


/*******************************************************************************
* ��������		   ��LCD��ʾ���ֺ���
*******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color,u8 N)	
{  
	u8 i,j;
	u8 *temp,size1;
	if(N==1){temp=DQZS;}
	else if(N==2){temp=SDZS;}
  LCD_Address_Set(x,y,x+size-1,y+size-1); //����һ�����ֵ�����
  size1=size*size/8;//һ��������ռ���ֽ�
	temp+=index*size1;//д�����ʼλ��
	for(j=0;j<size1;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)//�����ݵĵ�λ��ʼ��
			{
				LCD_WR_DATA(color);//����
			}
			else
			{
				LCD_WR_DATA(BACK_COLOR);//������
			}
		}
		temp++;
	 }
}


/*******************************************************************************
* ��������		   ��LCD��ʾ���ֺ���
*******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color);
} 


/*******************************************************************************
* ��������		   ��LCD���㺯��
*******************************************************************************/
void LCD_DrawPoint_big(u16 x,u16 y,u16 color)
{
	LCD_Fill(x-1,y-1,x+1,y+1,color);
} 


/*******************************************************************************
* ��������		   ��LCD�����ɫ����
*******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}


/*******************************************************************************
* ��������		   ��LCD���ߺ���
*******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/*******************************************************************************
* ��������		   ��LCD�����κ���
*******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/*******************************************************************************
* ��������		   ��LCD������ͼ����
*******************************************************************************/

void LCD_DrawWave(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x1+1,y1,BLACK);
	if(x1!=6)
	LCD_DrawLine(x1,y1,x2,y2,BLACK);
}

/*******************************************************************************
* ��������		   ��LCD�������ͼ����
*******************************************************************************/

void Clear_Wave()
{
	LCD_Fill(5,78,125,125,WHITE);
	LCD_DrawLine(4,103,125,103,YELLOW);
	x=6;
}

/*******************************************************************************
* ��������		   ��LCD��Բ����
*******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
//	int di;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}


/*******************************************************************************
* ��������		   ��LCD��ʾ�ַ�����
*******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color)
{
    u8 temp;
    u8 pos,t;
	  u16 x0=x;    
    if(x>LCD_W-16||y>LCD_H-16)return;	    //���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_WR_DATA(color);
				else LCD_WR_DATA(BACK_COLOR);
				temp>>=1;
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);//��һ����     
		        temp>>=1; 
		    }
		}
	}   	   	 	  
}


/*******************************************************************************
* ��������		   ��LCD��ʾ�ַ�������
*******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0,color);
        x+=8;
        p++;
    }  
}


/*******************************************************************************
* ��������		   ��LCD��ʾ���ֺ���
*******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/*******************************************************************************
* ��������		   ��LCD��ʾ�����ֺ���
*******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0,color); 
	}
} 


/*******************************************************************************
* ��������		   ��LCD��ʾС������
*******************************************************************************/
void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color)
{         	
	u8 t,temp;
//	u8 enshow=0;
	u16 num1;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+8*(len-2),y,'.',0,color);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0,color);
	}
}


/*******************************************************************************
* ��������		   ��LCD��ʾͼƬ����
*******************************************************************************/
void LCD_ShowPicture(u16 x1,u16 y1,u16 x2,u16 y2)
{
	int i;
	  LCD_Address_Set(x1,y1,x2,y2);
		for(i=0;i<1600;i++)
	  { 	
			LCD_WR_DATA8(image[i*2+1]);	 
			LCD_WR_DATA8(image[i*2]);			
	  }			
}

/*******************************************************************************
* ��������		   ��LCD��ʾA���溯��
*******************************************************************************/

void LCD_Star()
{
	u8 i=0;
	for(i=0;i<4;i++)
	LCD_ShowChinese(i*16+8,0,i,16,GBLUE,1);
  LCD_ShowString(68,0,":    .  ",DARKBLUE);
	for(i=0;i<4;i++)
	LCD_ShowChinese(i*16+8,20,i,16,GBLUE,2);
	LCD_ShowString(68,20,":   .  ",DARKBLUE);
	LCD_ShowString(4,40,"KP=  .   KI=",DARKBLUE);
  LCD_ShowString(4,60,"  .   KD=  .  ",DARKBLUE);
}

