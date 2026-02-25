#include "Sensor_head.h"
void DS18B20_DelayUs(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000) / 3;
    while (ticks--)
    {
        __NOP();
    }
}
 
// 初始化DS18B20
void DS18B20_Init(void)
{
    // 复位DS18B20
    DS18B20_Reset();
    
    // 发送跳过ROM命令
    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);
}
 
// 复位DS18B20并检测设备存在
uint8_t DS18B20_Reset(void)
{
    uint8_t presence = 0;
    
    // 拉低总线
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    DS18B20_DelayUs(480);
    
    // 释放总线
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    DS18B20_DelayUs(60);
    
    // 检测DS18B20响应
    presence = HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    DS18B20_DelayUs(420);
    
    return presence;
}
 
// 发送一个字节给DS18B20
void DS18B20_WriteByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 发送低位
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
        DS18B20_DelayUs(2);
        
        // 发送高位，根据byte的第i位来决定
        if (byte & (1 << i))
        {
            HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        }
        
        DS18B20_DelayUs(60);
        
        // 释放总线
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    }
}
 
// 从DS18B20读取一个字节
uint8_t DS18B20_ReadByte(void)
{
    uint8_t byte = 0;
    
    for (uint8_t i = 0; i < 8; i++)
    {
        // 发送低位
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
        DS18B20_DelayUs(2);
        
        // 释放总线
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        DS18B20_DelayUs(8);
        
        // 读取高位数据
        if (HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN))
        {
            byte |= (1 << i);
        }
        
        DS18B20_DelayUs(50);
    }
    
    return byte;
}
 
// 读取DS18B20温度
float DS18B20_GetTemperature(void)
{
    DS18B20_Init();
    
    // 发送温度转换命令
    DS18B20_WriteByte(DS18B20_CMD_CONVERT_T);
    
    // 等待转换完成
    HAL_Delay(800);
    
    // 复位DS18B20并跳过ROM
    DS18B20_Init();
    
    // 发送读取寄存器命令
    DS18B20_WriteByte(DS18B20_CMD_READ_SCRATCHPAD);
    
    // 读取温度数据
    uint8_t tempLow = DS18B20_ReadByte();
    uint8_t tempHigh = DS18B20_ReadByte();
    
    // 计算温度值
    int16_t temp = (tempHigh << 8) | tempLow;
    float temperature = (float)temp / 16.0f;
    
    return temperature;
}
