#ifndef __OLED_H__
#define __OLED_H__
#include "stm32f10x.h"
/*
用户若需要修改引脚，
修改本处宏定义的引脚和
oled_init()函数中的初始化引脚即可
*/
#define OLED_SCL_H GPIO_SetBits(GPIOB,GPIO_Pin_11)		//SCL引脚输出高电平
#define OLED_SCL_L GPIO_ResetBits(GPIOB,GPIO_Pin_11)	//SCL引脚输出低电平

#define OLED_SDA_H GPIO_SetBits(GPIOB,GPIO_Pin_10)		//SDA引脚输出高电平
#define OLED_SDA_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)	//SDA引脚输出低电平

extern void OLED_init(void);
extern void OLED_clear(void);
extern void OLED_setpos(uint8_t x,uint8_t y);
extern void IIC_writedata(uint8_t data,char identifying);
extern void IIC_sendbyte(char Byte);
extern void IIC_waitack(void);
extern void OLED_show6x8char(uint8_t x,uint8_t y,char character);

#define OLED_COMMAND 	0
#define OLED_DATA 		1


#endif

