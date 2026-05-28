#ifndef _ONENET_H_
#define _ONENET_H_

#include "stm32f10x.h"

extern u8 led_manual_mode;

_Bool OneNet_DevLink(void);

void OneNet_SendData(u8 temp, u8 humi, u8 ehumi, u16 lux, u16 co, u16 grain_humi);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

#endif
