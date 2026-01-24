#ifndef __MQ7_H
#define	__MQ7_H

#define MQ7_READ_TIMES	10  //MQ-7传感器ADC循环读取次数
 
//模式选择	
//模拟AO:	1
//数字DO:	0
#define	MQ_7_MODE 	1
 
/***************根据自己需求更改****************/
// MQ-7 GPIO宏定义
#if MQ_7_MODE
#define     MQ7_ADC_HANDLE          hadc1              // ADC句柄（）
#define		MQ7_AO_GPIO_CLK			RCC_APB2Periph_GPIOA
#define 	MQ7_AO_GPIO_PORT		GPIOA
#define 	MQ7_AO_GPIO_PIN			GPIO_PIN_0
#define     ADC_CHANNEL             ADC_Channel_0	// ADC 通道宏定义
 
#else
#define		MQ7_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ7_DO_GPIO_PORT							GPIOA
#define 	MQ7_DO_GPIO_PIN								GPIO_PIN_1			
 
#endif
/*********************END**********************/
 
 
void MQ7_Init(void);
uint16_t MQ7_GetData(void);
float MQ7_GetData_PPM(void);
 
#endif /* __ADC_H */
 