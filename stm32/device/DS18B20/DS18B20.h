#ifndef DS18B20_H_
#define DS18B20_H_


// DS18B20引脚定义
#define DS18B20_GPIO_PORT   GPIOB
#define DS18B20_GPIO_PIN    GPIO_PIN_0
 
// 定义DS18B20相关命令
#define DS18B20_CMD_SKIP_ROM        0xCC
#define DS18B20_CMD_CONVERT_T       0x44
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE
 
// 函数声明
void DS18B20_DelayUs(uint32_t us);
void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
void DS18B20_WriteByte(uint8_t byte);
uint8_t DS18B20_ReadByte(void);
float DS18B20_GetTemperature(void);


#endif /* DS18B20_H_ */