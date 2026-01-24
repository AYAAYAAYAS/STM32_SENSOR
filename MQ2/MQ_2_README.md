# MQ-2烟雾传感器

## 一、驱动特性

✓ **支持两种工作模式**
  - 模拟模式（推荐）：通过ADC采集精确的浓度值
  - 数字模式：简单的烟雾有无判断

✓ **灵活的配置**
  - 宏定义选择工作模式
  - 可配置ADC通道
  - 可配置DO数字引脚

✓ **完整的数据接口**
  - 原始ADC值
  - 电压值（V）
  - 烟雾浓度相对值（V/V0）
  - 动态校准支持

## 二、配置步骤

### 2.1 模式选择（MQ_2.h）

```c
// 选择工作模式
#define MQ2_WORK_MODE  MQ2_MODE_ANALOG   // 使用模拟模式
// #define MQ2_WORK_MODE  MQ2_MODE_DIGITAL  // 使用数字模式
```

### 2.2 模拟模式配置

```c
// ADC相关配置（使用现有的ADC1）
#define MQ2_ADC_HANDLE      hadc1              // 从adc.h中声明的句柄
#define MQ2_ADC_CHANNEL     ADC_CHANNEL_0      // 对应PA0
#define MQ2_ADC_GPIO_PORT   GPIOA
#define MQ2_ADC_PIN         GPIO_PIN_0
#define MQ2_ADC_RESOLUTION  4095               // 12位ADC
#define MQ2_REF_VOLTAGE     3.3f               // 参考电压
```

**注意**：已自动配置为PA0，与当前项目的ADC1配置一致。

### 2.3 数字模式配置

若需要使用数字模式，修改以下配置：

```c
#define MQ2_DO_GPIO_PORT    GPIOB              // DO信号GPIO端口
#define MQ2_DO_PIN          GPIO_PIN_1         // DO信号GPIO引脚
```

## 三、使用方法

### 3.1 初始化

在main.c中的初始化部分添加：

```c
// 初始化MQ-2传感器
// 参数：无烟雾时的基准电压（需要实际测量）
MQ2_Init(0.5f);  // 示例值：0.5V
```

**校准说明**：
- 在完全无烟雾的环境中运行程序
- 记录此时的电压值（通过串口打印输出）
- 将该电压值作为参数传入`MQ2_Init()`

### 3.2 数据采集

在main循环中定期调用（建议100ms周期）：

```c
// 定时采集（每100ms）
static uint32_t last_tick = 0;
uint32_t current_tick = HAL_GetTick();

if (current_tick - last_tick >= 100)
{
    last_tick = current_tick;
    
    // 采集模拟数据
    MQ2_AnalogCollect();  // 或 MQ2_DigitalCollect() 数字模式
}
```

### 3.3 数据读取

**方法1：获取单个参数**

```c
uint16_t adc_value = MQ2_GetADCValue();           // 0~4095
float voltage = MQ2_GetVoltage();                 // 0~3.3V
float concentration = MQ2_GetConcentration();     // 相对值
```

**方法2：一次性获取所有数据**

```c
MQ2_DataTypeDef* data = MQ2_GetData();

printf("ADC Raw: %d\r\n", data->adc_raw);
printf("Voltage: %.2f V\r\n", data->voltage);
printf("Concentration: %.2f\r\n", data->concentration);
```

### 3.4 烟雾检测示例

```c
void Check_Smoke(void)
{
    MQ2_AnalogCollect();
    
    MQ2_DataTypeDef* data = MQ2_GetData();
    
    // 判断方法1：基于浓度相对值
    if (data->concentration > 1.5f)  // 超过基准的1.5倍
    {
        // 触发烟雾报警
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    }
    
    // 判断方法2：基于绝对电压
    if (data->voltage > 1.0f)  // 超过1.0V
    {
        // 触发烟雾报警
    }
}
```

### 3.5 动态校准

```c
// 在运行时重新校准
void Recalibrate_MQ2(void)
{
    // 在无烟雾环境中测得新的电压值
    float new_calibration_voltage = 0.55f;
    MQ2_SetCalibration(new_calibration_voltage);
}
```

## 四、数据结构说明

```c
typedef struct
{
    uint16_t adc_raw;              // ADC原始值（0~4095）
    float voltage;                  // 实际电压值（0~3.3V）
    float concentration;            // 烟雾浓度相对值（V/V0）
    float calibration_voltage;      // 校准基准电压（无烟雾时）
} MQ2_DataTypeDef;
```

## 五、函数接口速查

| 函数 | 功能 | 参数 | 返回值 |
|------|------|------|--------|
| `MQ2_Init()` | 初始化传感器 | 校准电压(float) | 无 |
| `MQ2_AnalogCollect()` | 采集模拟数据 | 无 | 无 |
| `MQ2_DigitalCollect()` | 采集数字数据 | 无 | GPIO电平 |
| `MQ2_GetADCValue()` | 获取ADC值 | 无 | uint16_t |
| `MQ2_GetVoltage()` | 获取电压值 | 无 | float(V) |
| `MQ2_GetConcentration()` | 获取浓度值 | 无 | float |
| `MQ2_GetData()` | 获取全部数据 | 无 | 指针 |
| `MQ2_SetCalibration()` | 重新校准 | 基准电压(float) | 无 |

## 六、硬件接线

### 6.1 模拟模式（推荐）

```
MQ-2传感器           STM32F103
─────────────────────────────
AO (模拟输出)  ──→  PA0 (ADC1_IN0)
VCC (5V)       ──→  5V
GND            ──→  GND
```

**ADC配置已自动完成**，无需额外配置。

### 6.2 数字模式

```
MQ-2传感器           STM32F103
─────────────────────────────
DO (数字输出)  ──→  PB1 (GPIO Input)
VCC (5V)       ──→  5V
GND            ──→  GND
```

需在`MQ_2.h`中修改：
```c
#define MQ2_DO_GPIO_PORT  GPIOB
#define MQ2_DO_PIN        GPIO_PIN_1
```

## 七、常见问题

### Q1：如何确定正确的校准电压？

A：在完全无烟雾的环境中运行以下代码：
```c
void Calibration_Procedure(void)
{
    MQ2_Init(0.5f);  // 先用默认值初始化
    
    for(int i = 0; i < 10; i++)
    {
        HAL_Delay(1000);
        MQ2_AnalogCollect();
        
        float v = MQ2_GetVoltage();
        printf("Voltage: %.3f V\r\n", v);  // 观察电压稳定值
    }
    
    // 将稳定的电压值（如0.50V）作为参数传入MQ2_Init()
}
```

### Q2：浓度值如何使用？

A：浓度相对值（concentration）= 当前电压 / 基准电压
- 值 = 1.0：无烟雾（等于基准）
- 值 > 1.0：有烟雾
- 值越大，烟雾浓度越高
- 推荐阈值：1.2 ~ 1.5（可根据实际调整）

### Q3：可以同时使用多个MQ-2吗？

A：可以！但需要：
1. 创建多个`MQ2_DataTypeDef`数据结构体
2. 使用不同的ADC通道或GPIO引脚
3. 创建多份函数实现或使用参数化版本

### Q4：采集周期应该设置多少？

A：建议 50~200ms
- 更快更敏感，但CPU占用更高
- 更慢更省资源，但响应较慢
- 烟雾检测通常100ms足够

## 八、完整代码使用示例

### 示例1：最小化程序

```c
#include "main.h"
#include "MQ_2.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    
    // 初始化MQ-2
    MQ2_Init(0.5f);
    
    while (1)
    {
        HAL_Delay(100);
        MQ2_AnalogCollect();
        
        if (MQ2_GetConcentration() > 1.2f)
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  // 报警
        }
        else
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }
    }
}
```

### 示例2：带时间管理和数据打印

```c
#include "main.h"
#include "MQ_2.h"
#include <stdio.h>

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    
    MQ2_Init(0.5f);  // 0.5V为无烟雾基准
    
    uint32_t last_tick = 0;
    
    while (1)
    {
        uint32_t current_tick = HAL_GetTick();
        
        // 每100ms采集一次
        if (current_tick - last_tick >= 100)
        {
            last_tick = current_tick;
            
            // 采集数据
            MQ2_AnalogCollect();
            
            // 获取数据
            MQ2_DataTypeDef* data = MQ2_GetData();
            
            // 打印调试信息
            printf("ADC=%d, Voltage=%.2fV, Concentration=%.2f\r\n",
                   data->adc_raw, data->voltage, data->concentration);
            
            // 多级报警
            if (data->concentration > 1.5f)
            {
                printf("警告：烟雾浓度严重超标！\r\n");
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            }
            else if (data->concentration > 1.2f)
            {
                printf("警告：烟雾浓度过高\r\n");
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
        }
    }
}
```

### 示例3：数字模式

需要在 `MQ_2.h` 中修改：
```c
#define MQ2_WORK_MODE  MQ2_MODE_DIGITAL
```

然后使用代码：
```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    
    MQ2_Init(0.5f);
    
    while (1)
    {
        HAL_Delay(100);
        
        uint8_t smoke = MQ2_DigitalCollect();
        
        if (smoke == 0)  // DO输出低电平
        {
            printf("有烟雾\r\n");
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        }
        else  // DO输出高电平
        {
            printf("无烟雾\r\n");
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }
    }
}
```

### 示例4：动态校准

```c
/**
 * 在无烟雾环境中运行此函数来获取标准校准值
 */
void MQ2_Calibration(void)
{
    uint16_t i = 0;
    float voltage_sum = 0.0f;
    float calibration_voltage = 0.0f;
    
    printf("开始校准...\r\n");
    
    // 采集10次数据取平均
    for (i = 0; i < 10; i++)
    {
        HAL_Delay(500);
        MQ2_AnalogCollect();
        
        float v = MQ2_GetVoltage();
        voltage_sum += v;
        
        printf("采样%d: %.3f V\r\n", i + 1, v);
    }
    
    calibration_voltage = voltage_sum / 10.0f;
    
    printf("校准电压: %.3f V\r\n", calibration_voltage);
    printf("请使用: MQ2_Init(%.3f);\r\n", calibration_voltage);
    
    // 更新校准值
    MQ2_SetCalibration(calibration_voltage);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    
    MQ2_Init(0.5f);  // 先用默认值初始化
    
    // 第一次运行时执行校准
    MQ2_Calibration();
    
    uint32_t last_tick = 0;
    
    while (1)
    {
        uint32_t current_tick = HAL_GetTick();
        
        if (current_tick - last_tick >= 100)
        {
            last_tick = current_tick;
            
            MQ2_AnalogCollect();
            printf("Concentration: %.2f\r\n", MQ2_GetConcentration());
        }
    }
}
```

### 示例5：集成FreeRTOS

```c
#include "main.h"
#include "cmsis_os.h"
#include "MQ_2.h"

// MQ-2采集任务
void MQ2_Task(void const *argument)
{
    MQ2_Init(0.5f);
    
    for (;;)
    {
        osDelay(100);
        MQ2_AnalogCollect();
        
        float concentration = MQ2_GetConcentration();
        
        if (concentration > 1.2f)
        {
            // 发送报警信号给其他任务
            osSignalSet(osThreadGetId(), 0x0001);
        }
    }
}

// 报警任务
void Alarm_Task(void const *argument)
{
    for (;;)
    {
        osSignalWait(0x0001, osWaitForever);
        
        // 触发报警
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}
```

### 示例6：带移动平均滤波

```c
#define FILTER_SIZE  5

float Apply_Filter(float new_value)
{
    static float buffer[FILTER_SIZE] = {0};
    static uint8_t index = 0;
    float sum = 0;
    
    buffer[index] = new_value;
    index = (index + 1) % FILTER_SIZE;
    
    for (uint8_t i = 0; i < FILTER_SIZE; i++)
        sum += buffer[i];
    
    return sum / FILTER_SIZE;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    
    MQ2_Init(0.5f);
    uint32_t last_tick = 0;
    
    while (1)
    {
        uint32_t current_tick = HAL_GetTick();
        
        if (current_tick - last_tick >= 100)
        {
            last_tick = current_tick;
            
            MQ2_AnalogCollect();
            
            // 原始值
            float raw = MQ2_GetConcentration();
            
            // 滤波值
            float filtered = Apply_Filter(raw);
            
            printf("Raw: %.2f, Filtered: %.2f\r\n", raw, filtered);
            
            if (filtered > 1.2f)
            {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            }
        }
    }
}
```



