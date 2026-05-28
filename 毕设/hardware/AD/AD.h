#ifndef __AD_H_
#define __AD_H_

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
uint16_t MQ135_GetData_PPM(int tim);
uint16_t GetHumidity(int timese);
uint16_t GetLight(void );	
#endif
