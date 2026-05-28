#include "stm32f10x.h" 
#include "stm32f10x_adc.h"
#include "AD.h"
// Device header
int i;


void AD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;  // 移至此处

    // 2. 可执行语句
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    // 初始化GPIO结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 初始化ADC结构体
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	uint16_t AD0;
	//ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
uint16_t GetLight(void)
{
    uint16_t AD0;
    float V1, lux;

    // 读取 ADC 通道 0 的值
    AD0 = AD_GetValue(ADC_Channel_0);

    // 计算电压值
    V1 = (float)AD0 / 4095 * 3.3;

    // 计算光照强度
    lux = (V1 / 3.3) * 1000;

    // 将光照强度转换为 uint16_t 类型并返回
    return (uint16_t)lux;
}
uint16_t GetHumidity(int timese)
{	
	
	uint16_t data;
    uint32_t H_all = 0;
    float H_arg = 0;
    uint8_t t;
 
    // 进行多次ADC转换并累加
    for (t = 0; t < timese; t++) {
        H_all += AD_GetValue(ADC_Channel_2);
//        Delay_ms(1); // 延时1毫秒
    }
    // 计算平均值
H_arg = (float)H_all / (float)timese;  // 浮点除法
data = (uint16_t)((4095.0f - H_arg) / 3292.0f * 100.0f);
    // 确保湿度值在合理范围内（0~100）
    data = data > 100 ? 100 : (data < 0 ? 0 : data);
    return (uint16_t)data;
}

uint16_t MQ135_GetData_PPM(int tim)
{

	float  tempData = 0,Vol,ppm;
	for ( i = 0; i < tim; i++)
	{
		tempData += AD_GetValue(ADC_Channel_1);
//		Delay_ms(5);
	}
	tempData /= tim;
	
	 Vol = tempData * 3.3 / 4096;
	 ppm = (Vol / 3.3) * 100;
	return (uint16_t)ppm;
}
