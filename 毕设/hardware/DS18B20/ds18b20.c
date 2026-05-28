#include "stm32f10x.h"
#include "ds18b20.h"
#include "delay.h"

//==========================================================
//	函数名称：	DS18B20_Rst
//	函数功能：	复位DS18B20
//==========================================================
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();
    DS18B20_DQ_OUT = 0;
    delay_us(480);      //拉低至少480us
    DS18B20_DQ_OUT = 1;
    delay_us(15);       //拉高15~60us
}

//==========================================================
//	函数名称：	DS18B20_Check
//	函数功能：	等待DS18B20应答
//	返回值：	0-存在  1-不存在
//==========================================================
u8 DS18B20_Check(void)
{
    u8 retry = 0;

    DS18B20_IO_IN();
    while(DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 200) return 1;

    retry = 0;
    while(!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 240) return 1;

    return 0;
}

//==========================================================
//	函数名称：	DS18B20_Read_Bit
//	函数功能：	读一个位
//==========================================================
static u8 DS18B20_Read_Bit(void)
{
    u8 data;

    DS18B20_IO_OUT();
    DS18B20_DQ_OUT = 0;
    delay_us(2);
    DS18B20_DQ_OUT = 1;

    DS18B20_IO_IN();
    delay_us(12);
    data = DS18B20_DQ_IN;
    delay_us(50);

    return data;
}

//==========================================================
//	函数名称：	DS18B20_Read_Byte
//	函数功能：	读一个字节
//==========================================================
static u8 DS18B20_Read_Byte(void)
{
    u8 i, dat = 0;

    for(i = 0; i < 8; i++)
    {
        dat >>= 1;
        if(DS18B20_Read_Bit())
            dat |= 0x80;
    }
    return dat;
}

//==========================================================
//	函数名称：	DS18B20_Write_Byte
//	函数功能：	写一个字节
//==========================================================
static void DS18B20_Write_Byte(u8 dat)
{
    u8 i;

    DS18B20_IO_OUT();
    for(i = 0; i < 8; i++)
    {
        DS18B20_DQ_OUT = 0;
        delay_us(2);
        if(dat & 0x01)
            DS18B20_DQ_OUT = 1;
        else
            DS18B20_DQ_OUT = 0;
        delay_us(60);
        DS18B20_DQ_OUT = 1;
        dat >>= 1;
    }
}

//==========================================================
//	函数名称：	DS18B20_Start
//	函数功能：	启动一次温度转换
//==========================================================
void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC);   //跳过ROM
    DS18B20_Write_Byte(0x44);   //开始转换
}

//==========================================================
//	函数名称：	DS18B20_Init
//	函数功能：	初始化DS18B20的IO口并检测存在
//	返回值：	0-存在  1-不存在
//==========================================================
u8 DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_5);

    DS18B20_Rst();
    return DS18B20_Check();
}

//==========================================================
//	函数名称：	DS18B20_Get_Temp
//	函数功能：	获取温度值
//	返回值：	温度 * 10（如256表示25.6℃，支持负温度）
//==========================================================
short DS18B20_Get_Temp(void)
{
    u8 buf[9];
    u8 i;
    u16 raw;
    short temp;

    DS18B20_Start();            //启动转换
    delay_ms(750);              //等待转换完成（12位精度最长750ms）

    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC);   //跳过ROM
    DS18B20_Write_Byte(0xBE);   //读暂存器

    for(i = 0; i < 9; i++)
        buf[i] = DS18B20_Read_Byte();

    raw = ((u16)buf[1] << 8) | buf[0];

    //12位精度，分辨率0.0625℃，扩大10倍：乘以0.625
    if(raw & 0x8000)            //负温度
    {
        raw = (~raw) + 1;
        temp = -(short)((float)raw * 0.625f);
    }
    else
    {
        temp = (short)((float)raw * 0.625f);
    }

    return temp;
}
