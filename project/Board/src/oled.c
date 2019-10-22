#include "oled.h"
#include "delay.h"
#include "oled_font.h"

//该代码块中的函数对外界不开放
#if 1  		//创建一个可折叠的代码块用于存放IIC操作函数
inline void IIC_start(void)//传输开始
{
	OLED_SCL_H;
	OLED_SDA_H;
	OLED_SDA_L;
	OLED_SCL_L;
}
inline void IIC_stop(void)//传输结束
{
	OLED_SCL_H;
	OLED_SDA_L;
	OLED_SDA_H;
}
inline void IIC_waitack(void)//假装等待应答
{
	OLED_SCL_H;
	OLED_SCL_L;
}
inline void IIC_sendbyte(char Byte)//传输一个字节
{
	OLED_SCL_L;
	for(int a = 0;a<8;a++)
	{
		if(Byte&0x80)
		{
			OLED_SDA_H;
		}
		else
		{
			OLED_SDA_L;
		}
		Byte <<= 1;
		OLED_SCL_H;
		OLED_SCL_L;
	}
}
inline void IIC_writedata(uint8_t data,char identifying)
{
	IIC_start();
	IIC_sendbyte(0x78);
	IIC_waitack();
	if(identifying)
	{
		IIC_sendbyte(0x40);
	}
	else
	{
		IIC_sendbyte(0x00);
	}
	IIC_waitack();
	IIC_sendbyte(data);
	IIC_waitack();
	IIC_stop();
}

#endif
/*

*/
void OLED_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//打开GPIOB的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//使GPIOB10 11输出高电平
	
	
	//以下代码与oled通讯= =初始化它
	IIC_writedata(0xAE,OLED_COMMAND);//--display off
	IIC_writedata(0x00,OLED_COMMAND);//---set low column address
	IIC_writedata(0x10,OLED_COMMAND);//---set high column address
	IIC_writedata(0x40,OLED_COMMAND);//--set start line address  
	IIC_writedata(0xB0,OLED_COMMAND);//--set page address
	IIC_writedata(0x81,OLED_COMMAND); // contract control
	IIC_writedata(0xFF,OLED_COMMAND);//--128   
	IIC_writedata(0xA1,OLED_COMMAND);//set segment remap 
	IIC_writedata(0xA6,OLED_COMMAND);//--normal / reverse
	IIC_writedata(0xA8,OLED_COMMAND);//--set multiplex ratio(1 to 64)
	IIC_writedata(0x3F,OLED_COMMAND);//--1/32 duty
	IIC_writedata(0xC8,OLED_COMMAND);//Com scan direction
	IIC_writedata(0xD3,OLED_COMMAND);//-set display offset
	IIC_writedata(0x00,OLED_COMMAND);//
	IIC_writedata(0xD5,OLED_COMMAND);//set osc division
	IIC_writedata(0x80,OLED_COMMAND);//
	IIC_writedata(0xD8,OLED_COMMAND);//set area color mode off
	IIC_writedata(0x05,OLED_COMMAND);//
	IIC_writedata(0xD9,OLED_COMMAND);//Set Pre-Charge Period
	IIC_writedata(0xF1,OLED_COMMAND);//
	IIC_writedata(0xDA,OLED_COMMAND);//set com pin configuartion
	IIC_writedata(0x12,OLED_COMMAND);//
	IIC_writedata(0xDB,OLED_COMMAND);//set Vcomh
	IIC_writedata(0x30,OLED_COMMAND);//
	IIC_writedata(0x8D,OLED_COMMAND);//set charge pump enable
	IIC_writedata(0x14,OLED_COMMAND);//
	IIC_writedata(0xAF,OLED_COMMAND);//--turn on oled panel
}

void OLED_clear(void)
{ 
	for(int i=0;i<8;i++)  
	{  
		IIC_writedata(0xb0+i,OLED_COMMAND);
		IIC_writedata(0x00,OLED_COMMAND);
		IIC_writedata(0x10,OLED_COMMAND);
		for(int n=0;n<128;n++)
		{
			IIC_writedata(0,OLED_DATA);
		}
	}
}

void OLED_setpos(uint8_t x,uint8_t y){
	IIC_writedata(0xb0+y,OLED_COMMAND);
	IIC_writedata(((x&0xf0)>>4)|0x10,OLED_COMMAND);
	IIC_writedata((x&0x0f),OLED_COMMAND); 
}

void OLED_show6x8char(uint8_t x,uint8_t y,char character)
{
	character -= ' ';//消除字库中的偏移量
	OLED_setpos(x,y);
	for(int a = 0;a<6;a++)
	{
		IIC_writedata(char6X8[character][a],OLED_DATA);
	}
}
