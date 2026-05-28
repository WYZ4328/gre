#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void Device_Init(void);
void LED_Set(u8 state);
void Buzzer_Set(u8 state);
void Motor_Set(u8 state);
void Fan_Set(u8 state);

#endif
