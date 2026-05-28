#include "led.h"
#include "sys.h"

void Device_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// 禁用 JTAG，释放 PA15 (JTCK) 和 PB4 (NJTRST) 为普通 GPIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// LED - PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);

	// 蜂鸣器 - PB4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);

	// 振动模块 - PA15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);

	// 风扇 - PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

void LED_Set(u8 state)
{
	if(state)
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	else
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void Buzzer_Set(u8 state)
{
	if(state)
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

void Motor_Set(u8 state)
{
	if(state)
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
	else
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

void Fan_Set(u8 state)
{
	if(state)
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
	else
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}
