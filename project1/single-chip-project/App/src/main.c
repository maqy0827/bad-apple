#include "stm32f10x.h"
/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室
*  平台：STM32F103VET6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。     
******************************************************************/

#include "oled.h"
#include "delay.h"
#include "bad_apple.h"


unsigned int nowframe = 0;
char picture1[8][84];
char picture2[8][84];
char (*pictureshow)[84] = picture1;
char (*picturereceive)[84] = picture2;
int main()
{
	OLED_init();//初始化oled显示
	OLED_clear();
	UART4_init();
	Timer_interrupt_init();
	
	unsigned int lastframe = 0;

	while(1)
	{
		if(lastframe != nowframe)//图像更新
		{
			lastframe = nowframe;
			show_picture_Withoutcompression(pictureshow);
		}
	}
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		USART_SendData(UART4,0XA5);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}
int a = 0;
u8 ucTemp;
u8 pictureok = 0;
char (*b)[84];
void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)    //接收寄存器满中断 
	{
		ucTemp = USART_ReceiveData(UART4); 
		if(ucTemp == 0XA5)
		{
			b = pictureshow;
			pictureshow = picturereceive;
			picturereceive = b;
			nowframe++;
		}
		else if(ucTemp == 0X5A)
		{
			a = 0;
		}
		else
		{
			picturereceive[a/84][a%84] = ucTemp;
			a++;
		}
	}
}
