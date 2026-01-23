#ifndef __MQ_2_H
#define __MQ_2_H

#include "main.h"

/* =================== MQ-2 工作模式选择 =================== */
/**
 * @brief MQ-2工作模式
 * MQ2_MODE_DIGITAL: 数字模式（DO信号引脚）
 * MQ2_MODE_ANALOG:  模拟模式（AO信号，ADC采集）
 */
#define MQ2_MODE_DIGITAL    0
#define MQ2_MODE_ANALOG     1
#define MQ2_WORK_MODE       MQ2_MODE_ANALOG

/* =================== 模拟模式配置（仅在MQ2_WORK_MODE = MQ2_MODE_ANALOG时有效） =================== */
#define MQ2_ADC_HANDLE      hadc1              // ADC句柄（根据项目修改）
#define MQ2_ADC_CHANNEL     ADC_CHANNEL_0      // ADC通道（对应PA0）
#define MQ2_ADC_GPIO_PORT   GPIOA              // GPIO端口
#define MQ2_ADC_PIN         GPIO_PIN_0         // GPIO引脚
#define MQ2_ADC_RESOLUTION  4095               // 12位ADC分辨率
#define MQ2_REF_VOLTAGE     3.3f               // 参考电压3.3V

/* =================== 数字模式配置（仅在MQ2_WORK_MODE = MQ2_MODE_DIGITAL时有效） =================== */
#define MQ2_DO_GPIO_PORT    GPIOB              // DO信号GPIO端口（根据硬件修改）
#define MQ2_DO_PIN          GPIO_PIN_1         // DO信号GPIO引脚（根据硬件修改）

/* =================== MQ-2 数据结构 =================== */
/**
 * @brief MQ-2传感器数据结构
 */
typedef struct
{
    uint16_t adc_raw;              // ADC原始值（0~4095）
    float voltage;                  // 实际电压值（0~3.3V）
    float concentration;            // 烟雾浓度相对值
    float calibration_voltage;      // 校准基准电压（无烟雾时）
} MQ2_DataTypeDef;

/* =================== MQ-2 函数声明 =================== */

/**
 * @brief 初始化MQ-2传感器
 * @param calibration_voltage: 校准基准电压（无烟雾时的输出电压，单位V）
 * @retval 无
 */
void MQ2_Init(float calibration_voltage);

/**
 * @brief MQ-2传感器模拟数据采集
 * @retval 无
 */
void MQ2_AnalogCollect(void);

/**
 * @brief MQ-2传感器数字数据采集
 * @retval GPIO引脚电平：0（有烟雾）或1（无烟雾）
 */
uint8_t MQ2_DigitalCollect(void);

/**
 * @brief 获取MQ-2原始ADC值
 * @retval 原始ADC值（0~4095）
 */
uint16_t MQ2_GetADCValue(void);

/**
 * @brief 获取MQ-2电压值
 * @retval 电压值（V）
 */
float MQ2_GetVoltage(void);

/**
 * @brief 获取烟雾浓度相对值
 * @retval 浓度相对值（V/V0）
 */
float MQ2_GetConcentration(void);

/**
 * @brief 获取MQ-2传感器所有数据
 * @retval 指向MQ2_DataTypeDef结构体的指针
 */
MQ2_DataTypeDef* MQ2_GetData(void);
void MQ2_SetCalibration(float voltage);

#endif /* __MQ_2_H */
