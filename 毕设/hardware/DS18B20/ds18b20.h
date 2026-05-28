#ifndef __DS18B20_H
#define __DS18B20_H

#include "sys.h"

//IO方向设置 -- PA5, 属于CRL bit[23:20]
#define DS18B20_IO_IN()  {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=8<<20;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=3<<20;}

//IO操作
#define DS18B20_DQ_OUT  PAout(5)  //数据端口 PA5
#define DS18B20_DQ_IN   PAin(5)   //数据端口 PA5

u8   DS18B20_Init(void);        //初始化DS18B20
short DS18B20_Get_Temp(void);   //获取温度(返回值扩大10倍，如256表示25.6℃)
void DS18B20_Start(void);       //开始温度转换
u8   DS18B20_Check(void);       //检测是否存在DS18B20
void DS18B20_Rst(void);         //复位DS18B20

#endif
