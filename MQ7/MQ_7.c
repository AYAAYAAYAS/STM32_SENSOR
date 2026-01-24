#include "Sensor_head.h"
 
void MQ7_Init(void)
{
	#if MQ_7_MODE
	{
        ADCx_Init(&MQ7_ADC_HANDLE);  // 启动ADC转换
	}
	#else
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (MQ7_DO_GPIO_CLK, ENABLE );	// 打开连接 传感器DO 的单片机引脚端口时钟
		GPIO_InitStructure.GPIO_Pin = MQ7_DO_GPIO_PIN;			// 配置连接 传感器DO 的单片机引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			// 设置为上拉输入
		
		GPIO_Init(MQ7_DO_GPIO_PORT, &GPIO_InitStructure);				// 初始化 
		
	}
	#endif
}

#if MQ_7_MODE
uint16_t MQ7_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return HAL_ADC_GetValue(&MQ7_ADC_HANDLE);
}
#endif
 
uint16_t MQ7_GetData(void)
{
	
	#if MQ_7_MODE
	uint32_t  tempData = 0;
	for (uint8_t i = 0; i < MQ7_READ_TIMES; i++)
	{
		tempData += MQ7_ADC_Read();
		HAL_Delay(5);
	}
 
	tempData /= MQ7_READ_TIMES;
	return tempData;
	
	#else
	uint16_t tempData;
	tempData = !GPIO_ReadInputDataBit(MQ7_DO_GPIO_PORT, MQ7_DO_GPIO_PIN);
	return tempData;
	#endif
}
 
 
float MQ7_GetData_PPM(void)
{
	#if MQ_7_MODE
	float  tempData = 0;
	
 
	for (uint8_t i = 0; i < MQ7_READ_TIMES; i++)
	{
		tempData += MQ7_ADC_Read();
		HAL_Delay(5);
	}
	tempData /= MQ7_READ_TIMES;
	
	float Vol = (tempData*5/4096);
	float RS = (5-Vol)/(Vol*0.5);
	float R0=6.64;
	
	float ppm = pow(11.5428*R0/RS, 0.6549f);
	
	return ppm;
	#endif
}