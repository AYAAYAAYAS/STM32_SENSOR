#ifndef __ADCx_H__
#define __ADCx_H__



#define DMA_MODE 1 // DMA模式
#define ADC_BUFFER_SIZE  2  // ADC缓冲区大小


#define ADCx_CHANNEL_0  ADC_CHANNEL_0  // ADC通道0
#define ADCx_CHANNEL_1  ADC_CHANNEL_1  // ADC通道1
#define ADCx_CHANNEL_2  ADC_CHANNEL_2  // ADC通道2
#define ADCx_CHANNEL_3  ADC_CHANNEL_3  // ADC通道3
#define ADCx_CHANNEL_4  ADC_CHANNEL_4  // ADC通道4
#define ADCx_CHANNEL_5  ADC_CHANNEL_5  // ADC通道5
#define ADCx_CHANNEL_6  ADC_CHANNEL_6  // ADC通道6
#define ADCx_CHANNEL_7  ADC_CHANNEL_7  // ADC通道7

extern uint16_t ADC_Buffer[ADC_BUFFER_SIZE];
void ADCx_Init(ADC_HandleTypeDef* hadc);

#endif /* __ADCx_H__ */