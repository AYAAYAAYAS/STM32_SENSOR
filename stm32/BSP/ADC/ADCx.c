#include "Sensor_head.h"

uint16_t ADC_Buffer[ADC_BUFFER_SIZE] = {0};
void ADCx_Init(ADC_HandleTypeDef* hadc)
{
    HAL_ADCEx_Calibration_Start(hadc); // 校准ADC
    if (DMA_MODE)
    {
       HAL_ADC_Start_DMA(hadc, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE); 
    }
    else
    {
        HAL_ADC_Start(hadc);
    }    
}