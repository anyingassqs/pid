#include "eeprom.h"
#include "SysTick.h"
#include "pid.h"
/*******************************************************************************
* 函数功能		   : EEPROM初始化函数
*******************************************************************************/
void Eeprom_Init(void)
{
	IIC_Init();//IIC初始化
}

/*******************************************************************************
* 函数功能		   : EEPROM读取相关参数函数
*******************************************************************************/

void Eeprom_Readdat(void)
{
	pid.Kp=Eeprom_ReadLenByte(0,2)/100.0;
	pid.Ki=Eeprom_ReadLenByte(2,2)/100.0;
	pid.Kd=Eeprom_ReadLenByte(4,2)/100.0;
	Set_Speed=Eeprom_ReadLenByte(6,2)/100.0;
	Set_Angle=Eeprom_ReadLenByte(8,2);
}

/*******************************************************************************
* 函数功能		   :EEPROM指定地址读出一个数据
*******************************************************************************/
u8 Eeprom_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}
	else 
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据
	} 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

/*******************************************************************************
* 函数功能		   : 在EEPROM指定地址写入一个数据
*******************************************************************************/
void Eeprom_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}
	else 
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据
	} 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

/*******************************************************************************
* 函数功能		   : 在EEPROM里面的指定地址开始写入长度为Len的数据
*******************************************************************************/
void Eeprom_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		Eeprom_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/*******************************************************************************
* 函数功能		   : 在EEPROM里面的指定地址开始读出长度为Len的数据
*******************************************************************************/
u32 Eeprom_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=Eeprom_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

/*******************************************************************************
* 函数功能		   : 检查EEPROM是否正常
*******************************************************************************/
u8 Eeprom_Check(void)
{
	u8 temp;
	temp=Eeprom_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0x36)return 0;		   
	else//排除第一次初始化的情况
	{
		Eeprom_WriteOneByte(255,0X36);
	    temp=Eeprom_ReadOneByte(255);	  
		if(temp==0X36)return 0;
	}
	return 1;											  
}

/*******************************************************************************
* 函数功能		   : 在EEPROM里面的指定地址开始读出指定个数的数据
*******************************************************************************/
void Eeprom_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=Eeprom_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 

/*******************************************************************************
* 函数功能		   : 在EEPROM里面的指定地址开始写入指定个数的数据
*******************************************************************************/
void Eeprom_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		Eeprom_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
