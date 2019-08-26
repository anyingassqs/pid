#include "eeprom.h"
#include "SysTick.h"
#include "pid.h"
/*******************************************************************************
* ��������		   : EEPROM��ʼ������
*******************************************************************************/
void Eeprom_Init(void)
{
	IIC_Init();//IIC��ʼ��
}

/*******************************************************************************
* ��������		   : EEPROM��ȡ��ز�������
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
* ��������		   :EEPROMָ����ַ����һ������
*******************************************************************************/
u8 Eeprom_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}
	else 
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д����
	} 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

/*******************************************************************************
* ��������		   : ��EEPROMָ����ַд��һ������
*******************************************************************************/
void Eeprom_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
	else 
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д����
	} 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

/*******************************************************************************
* ��������		   : ��EEPROM�����ָ����ַ��ʼд�볤��ΪLen������
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
* ��������		   : ��EEPROM�����ָ����ַ��ʼ��������ΪLen������
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
* ��������		   : ���EEPROM�Ƿ�����
*******************************************************************************/
u8 Eeprom_Check(void)
{
	u8 temp;
	temp=Eeprom_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0x36)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		Eeprom_WriteOneByte(255,0X36);
	    temp=Eeprom_ReadOneByte(255);	  
		if(temp==0X36)return 0;
	}
	return 1;											  
}

/*******************************************************************************
* ��������		   : ��EEPROM�����ָ����ַ��ʼ����ָ������������
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
* ��������		   : ��EEPROM�����ָ����ַ��ʼд��ָ������������
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
