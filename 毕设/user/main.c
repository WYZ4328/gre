#include "stm32f10x.h"                  // Device header
#include "onenet.h"
#include "esp8266.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ad.h"
#include "oled.h"
#include "dht11.h"
#include "led.h"
#include <stddef.h>
#include <stdio.h>

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
unsigned char temp;

// EMC算法：通过环境温湿度计算粮食含水量
// 使用简化平衡含水率模型，考虑环境湿度和温度修正
u16 EMC_CalculateGrainHumi(u8 env_temp, u8 env_humi)
{
	s16 grain_humi;
	
	grain_humi = (s16)env_humi * 60;
	grain_humi = grain_humi - ((s16)env_temp - 25) * 50;
	
	if(grain_humi < 800) grain_humi = 800;
	if(grain_humi > 3000) grain_humi = 3000;
	
	return (u16)grain_humi;
}


void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	Delay_Init();									//systick初始化
	Usart3_Init(115200);							//串口1，驱动ESP8266用
	DHT11_Init();
	OLED_Init();
	AD_Init();
	Device_Init();
}

int main(void)
{
	unsigned short timeCount = 0;	//发送间隔变量（每10ms加1，100次=1s）

	unsigned char *dataPtr = NULL;

	u8 temperature = 0;				//温度值
	u8 humidity = 0;				//湿度值
	u8 humi = 0;             //粮食湿度
	u8 lux = 0;               //光照强度
	u8 CO = 0;                 //空气质量
	u16 grain_humi = 0;       //粮食含水量（EMC算法计算）

	Hardware_Init();				//初始化外围硬件


	OLED_ShowString(1,1,"Networking");
	ESP8266_Init();					//初始化ESP8266

	OLED_ShowString(1,1,"Connecting...");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);

	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);

	OLED_Clear();
	OLED_ShowString(1,1,"T:  C  H:  %");	//OLED固定标签
	OLED_ShowString(2,1,"Lux:");
	OLED_ShowString(3,1,"grain:");
	OLED_ShowString(4,1,"humi:");
	OLED_ShowString(4,9,"ppm:");
	OneNET_Subscribe();				//订阅命令主题

	while(1)
	{
		if(++timeCount >= 50)								//每100*10ms=1s上传一次
		{
			//读取DHT11温湿度
			if(DHT11_Read_Data(&temperature, &humidity) == 0)
			{
				OLED_ShowNum(1, 3, temperature, 2);
				OLED_ShowNum(1, 10, humidity, 2);
				
				// EMC算法：通过环境温湿度计算粮食含水量
				grain_humi = EMC_CalculateGrainHumi(temperature, humidity);
				OLED_ShowNum(3, 7, grain_humi / 100, 2);  // 显示粮食含水量
				OLED_ShowChar(3, 9, '.');
				OLED_ShowNum(3, 10, grain_humi % 100, 2);
			}
			
			lux = GetLight();
			if(led_manual_mode == 0)
			{
				if(lux > 200)
					LED_Set(1);
				else
					LED_Set(0);
			}
			CO = MQ135_GetData_PPM(10);
			OLED_ShowNum(2,5,lux,3);
			OLED_ShowNum(4,6,humi,2);
			OLED_ShowNum(4,13,CO,2);



			//上传所有数据到OneNET
			OneNet_SendData(temperature, humidity, humi, lux, CO, grain_humi);

			timeCount = 0;
			ESP8266_Clear();
		}

		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);

		delay_ms(10);
	}
}



