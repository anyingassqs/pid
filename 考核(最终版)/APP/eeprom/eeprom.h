#ifndef _24cxx_H
#define _24cxx_H

#include "system.h"
#include "iic.h"


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

//开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02

u8 Eeprom_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void Eeprom_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void Eeprom_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 Eeprom_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void Eeprom_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void Eeprom_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据
void Eeprom_Readdat(void);
u8 Eeprom_Check(void);  //检查器件
void Eeprom_Init(void); //初始化IIC


#endif
