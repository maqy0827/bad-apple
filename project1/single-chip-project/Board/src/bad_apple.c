#include "bad_apple.h"
#include "oled.h"

void UART4_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	USART_InitStructure.USART_BaudRate = 256000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位八位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位一位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送接收模式
	
  USART_Init(UART4, &USART_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 						//中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;  			//配置 UART4 为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	// 抢断优先级1 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 					// 子优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  						//使能中断
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//打开中断
	USART_Cmd(UART4, ENABLE);
}


void Timer_interrupt_init(void){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 499;
	TIM_TimeBaseStructure.TIM_Prescaler =7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);			 
}


void show_picture_Withoutcompression(char picture[8][84])
{
	for(int b = 7;b>-1;b--){
		OLED_setpos(22,7-b);
		for(int a = 0;a<84;a++){
			IIC_writedata(picture[b][a],OLED_DATA);
		}
	}
}

