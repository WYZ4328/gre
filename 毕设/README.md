# ESP8266集成驱动说明

## 文件说明

本工程包含完整的ESP8266集成驱动代码，所有相关功能都集成在esp8266.h和esp8266.c两个文件中，去除了OneNet相关功能，只保留连接WiFi热点和连接服务器的功能，非常方便移植使用。

## 文件列表

- **esp8266.h** - ESP8266集成驱动头文件(包含所有定义和函数声明)
- **esp8266.c** - ESP8266集成驱动源文件(包含所有功能实现)
- **main_example.c** - 示例主程序

## 主要功能

### 1. 硬件初始化
- 系统时钟配置
- 中断控制器分组设置
- 延时函数初始化
- 串口2初始化(ESP8266通信用)

### 2. ESP8266功能
- 硬件复位ESP8266模块
- 设置WiFi模式为Station模式
- 启用DHCP自动获取IP
- 连接指定的WiFi热点
- 连接指定IP和端口的服务器
- 发送数据到服务器
- 接收服务器返回的数据
- 断开服务器连接

### 3. 串口通信
- 串口2：用于ESP8266通信 (PA2-TX, PA3-RX, 波特率115200)

## 配置说明

### WiFi配置
在 `esp8266.h` 文件中修改以下宏定义：
```c
#define ESP8266_WIFI_NAME		"你的WiFi名称"		//WiFi名称
#define ESP8266_WIFI_PASSWORD	"你的WiFi密码"		//WiFi密码
```

### 服务器配置
在 `esp8266.h` 文件中修改以下宏定义：
```c
#define ESP8266_SERVER_IP		"192.168.1.100"	//服务器IP地址
#define ESP8266_SERVER_PORT		"8080"			//服务器端口
```

## 硬件连接

### ESP8266与STM32连接
- ESP8266 VCC -> 3.3V
- ESP8266 GND -> GND  
- ESP8266 TXD -> PA3 (USART2_RX)
- ESP8266 RXD -> PA2 (USART2_TX)
- ESP8266 CH_PD -> 3.3V
- ESP8266 RST -> PA0 (复位控制)

## 使用方法

1. 将 `esp8266.h` 和 `esp8266.c` 两个文件添加到你的工程中
2. 根据实际情况修改WiFi和服务器配置
3. 在主函数中调用初始化函数：
   ```c
   ESP8266_Hardware_Init();    //ESP8266硬件初始化(包含串口2初始化)
   ESP8266_Init();             //ESP8266初始化(包含WiFi连接)
   ESP8266_ConnectServer();    //连接服务器
   ```
4. 使用 `ESP8266_SendData()` 发送数据
5. 使用 `ESP8266_GetIPD()` 接收数据

## 主要API函数

### 硬件相关
- `ESP8266_Hardware_Init()` - ESP8266硬件初始化(包含串口2初始化)
- `Delay_Init()` - 延时函数初始化
- `delay_ms()` - 毫秒延时
- `delay_us()` - 微秒延时

### 串口相关
- `Usart2_Init()` - 串口2初始化(ESP8266通信用)
- `Usart_SendString()` - 串口发送数据
- `UsartPrintf()` - 串口格式化打印

### ESP8266相关
- `ESP8266_Init()` - 初始化ESP8266并连接WiFi
- `ESP8266_ConnectWiFi()` - 连接WiFi热点
- `ESP8266_ConnectServer()` - 连接服务器
- `ESP8266_SendData()` - 发送数据到服务器
- `ESP8266_GetIPD()` - 获取接收数据
- `ESP8266_DisconnectServer()` - 断开服务器连接
- `ESP8266_SendCmd()` - 发送AT命令
- `ESP8266_Clear()` - 清空接收缓冲区

## 注意事项

1. 确保ESP8266供电稳定，建议使用3.3V电源
2. 串口2波特率默认为115200，可在初始化时修改
3. ESP8266复位引脚连接到PA0，用于硬件复位
4. 所有功能都集成在两个文件中，移植非常方便
5. 不包含串口1相关代码，避免与其他工程冲突

## 移植说明

本代码已经完全集成，移植时只需要：
1. 包含 `esp8266.h` 头文件
2. 添加 `esp8266.c` 源文件到工程
3. 根据目标硬件修改WiFi和服务器连接参数
4. 在主程序中调用相应函数即可

## 示例代码

参考 `main_example.c` 文件，展示了基本的使用流程：
1. ESP8266硬件初始化
2. ESP8266初始化和WiFi连接
3. 服务器连接
4. 数据收发循环
