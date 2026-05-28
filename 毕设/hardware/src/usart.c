//硬件驱动
#include "usart.h"
#include "delay.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "oled.h"

/*
************************************************************
*	函数名称：	Usart3_Init
*
*	函数功能：	串口3初始化（驱动ESP8266）
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PB10		RX-PB11
************************************************************
*/
void Usart3_Init(unsigned int baud)
{
	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;

	// ✅ 开启时钟（注意多了GPIOB）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// PB10	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);

	// PB11	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &gpio_initstruct);

	// USART配置
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_initstruct.USART_Parity = USART_Parity_No;
	usart_initstruct.USART_StopBits = USART_StopBits_1;
	usart_initstruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usart_initstruct);

	USART_Cmd(USART3, ENABLE);

	// ✅ 开启接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	// NVIC配置
	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);
}


/*
************************************************************
*	函数名称：	Usart_SendString
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{
	unsigned short count = 0;

	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}


/*
************************************************************
*	函数名称：	UsartPrintf
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;

	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);
	va_end(ap);

	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
