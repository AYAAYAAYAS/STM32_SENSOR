# ADC（模拟-数字转换器）
## 1. 介绍
ADC（Analog-to-Digital Converter，模拟-数字转换器）是一种电子设备，它可以将模拟信号转换为数字信号。

## 2. 工作原理
ADC的工作原理是通过采集模拟信号，然后将其转换为数字信号。ADC的采集方式有两种：

1. 单次采样：ADC一次性采集全部模拟信号，然后转换为数字信号。
2. 连续采样：ADC以固定时间间隔采集模拟信号，然后转换为数字信号。

## 3. 应用场景
ADC的应用场景有：

1. 电压测量：ADC可以测量电压，如测量电池电压、测量电源电压。
2. 环境光测量：ADC可以测量环境光，如测量太阳光照度。
3. 声音测量：ADC可以测量声音，如测量声音强度。

## 4. 注意事项
1. ADC只能在系统运行时使用，不能在系统停止时使用。
2. ADC的采样率一般为 12.5MHz、25MHz、50MHz、100MHz。
3. ADC的精度一般为 12位、13位、14位、15位。

## 代码示例
1. 标准库函数：
```c
#include "stm32f10x.h"

int main(void)
{
    // 使能ADC时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 设置ADC工作参数
    ADC1->CR2 |= ADC_CR2_ADON;
    // 启动ADC
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (1)
    {
        // 正常运行代码
        // 若ADC采样完成，则读取ADC数据
        if (ADC1->SR & ADC_SR_EOC)
        {
            uint16_t adc_value = ADC1->DR;
        }
    }
}
```
2. HAL库函数：
```c
int main(void)
{

    HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  while (1)
  {
    adc_value1 = HAL_ADC_GetValue(hadc);
    adc_value2 = HAL_ADC_GetValue(hadc2);

  }
}
```

# 串口(USART)
## 1. 介绍
串口（Serial Peripheral Interface，SPI）是一种串行通信接口，它可以实现数据收发。

## 2. 工作原理
串口的工作原理是通过串行信号传输数据。串口通信协议有两种：

1. 同步串行通信协议：同步串行通信协议（Synchronous Serial Communication，SSC）是串行通信协议的一种，它规定了数据位、停止位、起始位的时序。
2. 异步串行通信协议：异步串行通信协议（Asynchronous Serial Communication，ASC）是串行通信协议的一种，它规定了数据位、停止位、起始位的时序，但没有规定数据位的同步方式。

## 3. 应用场景
串口的应用场景有：

1. 数据传输：串口可以实现数据的传输，如传输数据到计算机。
2. 打印机控制：串口可以实现打印机的控制，如控制打印机打印。
3. 通信：串口可以实现通信，如实现无线通信。

## 4. 注意事项
1. 串口只能在系统运行时使用，不能在系统停止时使用。
2. 串口的通信速率一般为 9600bps、19200bps、38400bps、57600bps、115200bps。
3. 串口的通信协议有两种：同步串行通信协议（SSC）和异步串行通信协议（ASC）。

## 代码示例
1. 标准库函数：
```c
#include "stm32f10x.h"

int main(void)
{
    // 使能串口时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // 设置串口工作参数
    USART1->BRR = 0x00C3; // 9600bps
    // 设置串口模式为接收模式
    USART1->CR1 |= USART_CR1_RE;
    // 使能串口中断
    NVIC_EnableIRQ(USART1_IRQn);
    // 启动串口
    USART1->CR1 |= USART_CR1_UE;
    while (1)
    {
        // 正常运行代码
        // 若接收到数据，则执行串口中断服务程序
    }
}

void USART1_IRQHandler(void)
{
    // 串口中断服务程序
    // 执行串口相关任务
    // 如清除串口中断标志
    USART1->SR &= ~USART_SR_RXNE;
}
```
2. HAL库函数：
```c
int main(void)
{

    HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_Init();
  while (1)
  {

    }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{

	}
}
```

3. 串口重定向：
串口重定向时，需要打开keil的miclib库
并且引用头文件：
```c
#include <stdio.h>
```
```c
int fputc(int ch,FILE *f)
 
{
 
//采用轮询方式发送1字节数据，超时时间设置为无限等待
 
HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
 
return ch;
 
}
```

# 定时器中断
## 1. 介绍
定时器中断（Timer Interrupt）是一种定时器的一种中断方式，它可以使系统在规定的时间间隔内执行特定任务。

## 2. 工作原理
定时器中断的工作原理是通过定时器的计数器达到设定值时触发中断，然后执行中断服务程序。

## 3. 应用场景
定时器中断的应用场景有：

1. 定时器计数到达指定值时执行特定任务。
2. 定时器计数到达指定值时触发外部中断。
3. 定时器计数到达指定值时产生系统时钟。

## 4. 注意事项
1. 定时器中断只能在系统运行时使用，不能在系统停止时使用。
2. 定时器中断的触发时间不能太长，否则系统会在触发后延迟执行，导致数据丢失或系统崩溃。

## 代码示例
1. 标准库函数：
```c
#include "stm32f10x.h"

int main(void)
{    
    // 使能定时器时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // 设置定时器时钟源为系统时钟
    TIM1->CR1 |= TIM_CR1_CKD_1;
    // 设置定时器模式为计数模式
    TIM1->CR1 |= TIM_CR1_CMS_0;
    // 设置定时器计数值
    TIM1->ARR = 10000-1; // 10000 表示 10s
    // 设置定时器中断
    TIM1->DIER |= TIM_DIER_UIE;
    // 使能定时器中断
    NVIC_EnableIRQ(TIM1_IRQn);
    // 启动定时器
    TIM1->CR1 |= TIM_CR1_CEN;
    while (1)
    {
        // 正常运行代码
        // 若定时器计数到达指定值，则执行定时器中断服务程序
    }
}

void TIM1_IRQHandler(void)
{
    // 定时器中断服务程序
    // 执行定时器相关任务
    // 如清除定时器中断标志
    TIM1->SR &= ~TIM_SR_UIF;
}
```
2. HAL库函数：
```c
int main(void)
{

    HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM1_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  while (1)
  {

    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{

	}
}
```

# 看门狗定时器
## 1. 介绍
看门狗定时器（Watchdog Timer，WDT）是一种定时器，它可以用来监视系统的运行状态，如果系统长时间没有正常运行，则会产生系统复位。

## 2. 工作原理
看门狗定时器工作原理是通过定时器的溢出（计数器达到设定值）来触发系统复位。当系统发生异常时，看门狗定时器会自动复位系统。

## 3. 应用场景
看门狗定时器的应用场景有：

1. 防止系统因异常而进入无限循环。
2. 防止系统因异常而导致电源供应不足。
3. 防止系统因异常而导致硬件损坏。

## 4. 注意事项
1. 看门狗定时器只能在系统运行时使用，不能在系统停止时使用。
2. 看门狗定时器的溢出时间不能太长，否则系统会在溢出后复位，导致数据丢失或系统崩溃。

## 5. 常用配置
1. 看门狗定时器的溢出时间：一般设置为 10s 左右。
2. 看门狗定时器的复位方式：一般选择电源复位。   
3. 看门狗定时器的使能方式：一般在系统初始化时使能。

## 代码示例
1. 标准库函数：
```c
#include "stm32f10x.h"

int main(void)
{
    // 使能看门狗定时器
    RCC->APB1ENR |= RCC_APB1ENR_WDTEN;
    // 设置看门狗定时器溢出时间为 10s
    WDT->CR = WDT_CR_WDTIE | WDT_CR_WDTOE | WDT_CR_WDE;
    WDT->WR = 0x0F; // 0x0F 表示 16 分钟
    // 启动看门狗定时器
    WDT->CR |= WDT_CR_WDE;
    while (1)
    {
        // 正常运行代码
        // 若发生异常，看门狗定时器会自动复位系统
    }
```
2. HAL库函数：
```c
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_IWDG_Init();
  MX_TIM4_Init();
  HAL_TIM_Base_Start_IT(&htim4);
  while (1)
  {

  }

}
static uint16_t index_1000ms = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		index_1000ms++;
		if(index_1000ms==10000)
		{
			HAL_IWDG_Refresh(&hiwdg);
			index_1000ms=0;
		}
	}
}
```
此方法为独立看门狗使用HAL库函数配置看门狗定时器，并在10s溢出时刷新看门狗定时器。