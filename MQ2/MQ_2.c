#include "Sensor_head.h"

static MQ2_DataTypeDef mq2_data = {
    .adc_raw = 0,
    .voltage = 0.0f,
    .concentration = 0.0f,
    .calibration_voltage = 0.5f  // 默认基准电压
};

/* =================== 内部函数声明 =================== */
static void MQ2_ADC_Convert(void);

/* =================== 函数实现 =================== */

/**
 * @brief 初始化MQ-2传感器
 * @param calibration_voltage: 校准基准电压（无烟雾时的输出电压，单位V）
 * @retval 无
 */
void MQ2_Init(float calibration_voltage)
{
    mq2_data.calibration_voltage = calibration_voltage;

#if (MQ2_WORK_MODE == MQ2_MODE_ANALOG)
    ADCx_Init(&MQ2_ADC_HANDLE);  // 启动ADC转换
#elif (MQ2_WORK_MODE == MQ2_MODE_DIGITAL)
    // 数字模式初始化：DO引脚配置为输入
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIO时钟
    if (MQ2_DO_GPIO_PORT == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (MQ2_DO_GPIO_PORT == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (MQ2_DO_GPIO_PORT == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // 配置DO引脚为浮空输入
    GPIO_InitStruct.Pin = MQ2_DO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MQ2_DO_GPIO_PORT, &GPIO_InitStruct);
#endif
}

/**
 * @brief ADC值转换为电压和浓度（内部函数）
 * @retval 无
 */
static void MQ2_ADC_Convert(void)
{
    // 转换为实际电压（0~3.3V）
    mq2_data.voltage = (float)mq2_data.adc_raw * MQ2_REF_VOLTAGE / MQ2_ADC_RESOLUTION;
    
    // 计算烟雾浓度相对值（V/V0）
    // V0为无烟雾时的基准电压，V为当前电压
    if (mq2_data.calibration_voltage > 0)
    {
        mq2_data.concentration = mq2_data.voltage / mq2_data.calibration_voltage;
    }
    else
    {
        mq2_data.concentration = 0.0f;
    }
}

/**
 * @brief MQ-2传感器模拟数据采集
 * @retval 无
 */
void MQ2_AnalogCollect(void)
{
#if (MQ2_WORK_MODE == MQ2_MODE_ANALOG)
    // 等待ADC转换完成（超时时间为10ms）
	if(DMA_MODE == 0)
	{
	  if (HAL_ADC_PollForConversion(&MQ2_ADC_HANDLE, 10) == HAL_OK)
    {
        // 获取ADC原始值
        mq2_data.adc_raw = HAL_ADC_GetValue(&MQ2_ADC_HANDLE);
        
        // 转换为电压和浓度
        MQ2_ADC_Convert();
    }
    HAL_ADC_Start(&MQ2_ADC_HANDLE);
	}
	else
	{
		mq2_data.adc_raw = ADC_Buffer[0];
    MQ2_ADC_Convert();
	}

#endif
}

/**
 * @brief MQ-2传感器数字数据采集
 * @retval GPIO引脚电平：0（有烟雾）或1（无烟雾）
 */
uint8_t MQ2_DigitalCollect(void)
{
#if (MQ2_WORK_MODE == MQ2_MODE_DIGITAL)
    return HAL_GPIO_ReadPin(MQ2_DO_GPIO_PORT, MQ2_DO_PIN);
#else
    return 1;  // 非数字模式返回1
#endif
}

/**
 * @brief 获取MQ-2原始ADC值
 * @retval 原始ADC值（0~4095）
 */
uint16_t MQ2_GetADCValue(void)
{
    return mq2_data.adc_raw;
}

/**
 * @brief 获取MQ-2电压值
 * @retval 电压值（V）
 */
float MQ2_GetVoltage(void)
{
    return mq2_data.voltage;
}

/**
 * @brief 获取烟雾浓度相对值
 * @retval 浓度相对值（V/V0）
 */
float MQ2_GetConcentration(void)
{
    return mq2_data.concentration;
}

/**
 * @brief 获取MQ-2传感器所有数据
 * @retval 指向MQ2_DataTypeDef结构体的指针
 */
MQ2_DataTypeDef* MQ2_GetData(void)
{
    return &mq2_data;
}

/**
 * @brief 设置校准基准电压
 * @param voltage: 无烟雾状态下的基准电压
 * @retval 无
 */
void MQ2_SetCalibration(float voltage)
{
    if (voltage > 0)
    {
        mq2_data.calibration_voltage = voltage;
    }
}