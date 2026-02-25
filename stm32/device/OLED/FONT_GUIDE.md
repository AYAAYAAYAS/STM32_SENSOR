/**
 ******************************************************************************
 * @file    FONT_GUIDE.md
 * @brief   OLED字库和字体显示完整指南
 ******************************************************************************
 */

# OLED字库和字体显示完整指南

## 1. 字库概述

### 1.1 支持的字体

本OLED驱动包含两种标准ASCII字体库：

| 字体 | 分辨率 | 字体常量 | 字数 | 用途 |
|------|--------|----------|------|------|
| 6x8  | 6×8像素 | OLED_FONT_6x8 | 95个 | 紧凑显示，信息密集 |
| 8x16 | 8×16像素 | OLED_FONT_8x16 | 95个 | 标准显示，易读性强 |

### 1.2 支持的字符范围

两种字体都支持ASCII字符范围 `0x20` ~ `0x7E`，共95个可显示字符。

#### 完整字符集：

```
空格：  (0x20)
数字：  0-9
大写字母：A-Z
小写字母：a-z
特殊字符：! " # $ % & ' ( ) * + , - . /
         : ; < = > ? @ [ \ ] ^ _ `
         { | } ~
```

#### 字符代码表：

```
十进制  十六进制  字符   十进制  十六进制  字符
32      0x20     空格   80      0x50     P
33      0x21     !      81      0x51     Q
34      0x22     "      82      0x52     R
35      0x23     #      83      0x53     S
36      0x24     $      84      0x54     T
37      0x25     %      85      0x55     U
38      0x26     &      86      0x56     V
39      0x27     '      87      0x57     W
40      0x28     (      88      0x58     X
41      0x29     )      89      0x59     Y
42      0x2A     *      90      0x5A     Z
43      0x2B     +      91      0x5B     [
44      0x2C     ,      92      0x5C     \
45      0x2D     -      93      0x5D     ]
46      0x2E     .      94      0x5E     ^
47      0x2F     /      95      0x5F     _
48      0x30     0      96      0x60     `
49      0x31     1      97      0x61     a
50      0x32     2      98      0x62     b
51      0x33     3      99      0x63     c
52      0x34     4      100     0x64     d
53      0x35     5      101     0x65     e
54      0x36     6      102     0x66     f
55      0x37     7      103     0x67     g
56      0x38     8      104     0x68     h
57      0x39     9      105     0x69     i
58      0x3A     :      106     0x6A     j
59      0x3B     ;      107     0x6B     k
60      0x3C     <      108     0x6C     l
61      0x3D     =      109     0x6D     m
62      0x3E     >      110     0x6E     n
63      0x3F     ?      111     0x6F     o
64      0x40     @      112     0x70     p
65      0x41     A      113     0x71     q
66      0x42     B      114     0x72     r
67      0x43     C      115     0x73     s
68      0x44     D      116     0x74     t
69      0x45     E      117     0x75     u
70      0x46     F      118     0x76     v
71      0x47     G      119     0x77     w
72      0x48     H      120     0x78     x
73      0x49     I      121     0x79     y
74      0x4A     J      122     0x7A     z
75      0x4B     K      123     0x7B     {
76      0x4C     L      124     0x7C     |
77      0x4D     M      125     0x7D     }
78      0x4E     N      126     0x7E     ~
79      0x4F     O
```

## 2. 字库数据结构

### 2.1 字库存储位置

字库数据定义在 `OLED_Font.h` 文件中：

```c
// 6x8字体库
extern const uint8_t OLED_Font6x8[95][6];

// 8x16字体库  
extern const uint8_t OLED_Font8x16[95][16];
```

### 2.2 字体数据格式

#### 6x8字体（每个字符6字节）
- 每个字符占6个字节
- 每个字节代表1列，8位对应8行像素
- LSB = 上面一行，MSB = 下面一行

示例：字符'A'的6x8字体数据：
```
  0x00, 0x00, 0x38, 0x44, 0x44, 0x7C  
  (空) (空) (开始) (向上) (向上) (完成)
```

#### 8x16字体（每个字符16字节）
- 每个字符占16个字节
- 分为上下两部分，每部分8字节
- 上半部分（字节0-7）：第1-8行
- 下半部分（字节8-15）：第9-16行

示例：字符'A'的8x16字体数据：
```
上半部分：0x00, 0x00, 0x0C, 0x12, 0x21, 0x40, 0x40, 0x7F
下半部分：0x40, 0x40, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00
```

## 3. 字体使用API

### 3.1 基础文字显示

```c
/**
 * 显示单个字符
 * @param x: X坐标（0-127）
 * @param y: Y坐标（0-63）
 * @param char: 要显示的字符
 * @param font: 字体选择（OLED_FONT_6x8或OLED_FONT_8x16）
 * @param color: 颜色（OLED_COLOR_BLACK或OLED_COLOR_WHITE）
 */
void OLED_ShowChar(uint8_t x, uint8_t y, char c, uint8_t font, uint8_t color);

/**
 * 显示字符串
 * @param x: X坐标
 * @param y: Y坐标
 * @param str: 字符串指针
 * @param font: 字体选择
 * @param color: 颜色
 */
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t font, uint8_t color);
```

### 3.2 数字显示

```c
/**
 * 显示整数
 * @param x: X坐标
 * @param y: Y坐标
 * @param num: 整数值（支持负数）
 * @param font: 字体选择
 * @param color: 颜色
 */
void OLED_ShowInt(uint8_t x, uint8_t y, int32_t num, uint8_t font, uint8_t color);

/**
 * 显示浮点数
 * @param x: X坐标
 * @param y: Y坐标
 * @param num: 浮点数值（支持负数）
 * @param decimal_places: 小数位数（0-3）
 * @param font: 字体选择
 * @param color: 颜色
 */
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t decimal_places, 
                     uint8_t font, uint8_t color);
```

## 4. 使用示例

### 4.1 显示简单文本

```c
#include "OLED.h"

int main(void)
{
    // 初始化OLED
    OLED_Init();
    
    // 清屏
    OLED_Clear();
    
    // 显示6x8字体（紧凑）
    OLED_ShowString(0, 0, "Hello OLED!", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 显示8x16字体（更大）
    OLED_ShowString(0, 10, "12345", OLED_FONT_8x16, OLED_COLOR_WHITE);
    
    // 刷新显示
    OLED_Refresh();
    
    while (1);
    
    return 0;
}
```

### 4.2 显示数字和小数

```c
void DisplaySensorData(void)
{
    float temperature = 23.45f;
    int humidity = 65;
    
    OLED_Clear();
    
    // 显示温度（带2位小数）
    OLED_ShowString(0, 0, "Temp:", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowFloat(30, 0, temperature, 2, OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowString(60, 0, "C", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 显示湿度（整数）
    OLED_ShowString(0, 8, "Humidity:", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowInt(60, 8, humidity, OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowString(80, 8, "%", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    OLED_Refresh();
}
```

### 4.3 混合显示

```c
void DisplaySystemInfo(void)
{
    OLED_Clear();
    
    // 标题（8x16大字体）
    OLED_ShowString(20, 0, "System", OLED_FONT_8x16, OLED_COLOR_WHITE);
    
    // 详细信息（6x8小字体）
    OLED_ShowString(0, 16, "Voltage: 3.3V", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowString(0, 24, "Current: 250mA", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowString(0, 32, "Power: ", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowFloat(42, 32, 0.825f, 3, OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowString(78, 32, "W", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    OLED_Refresh();
}
```

### 4.4 显示各种字符

```c
void DisplayCharacters(void)
{
    OLED_Clear();
    
    // 大写字母
    OLED_ShowString(0, 0, "ABCDEFGHIJ", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 小写字母  
    OLED_ShowString(0, 8, "abcdefghij", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 数字
    OLED_ShowString(0, 16, "0123456789", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 特殊字符
    OLED_ShowString(0, 24, "!@#$%^&*()", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 更多特殊字符
    OLED_ShowString(0, 32, "+-*/<=>?[]", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    OLED_Refresh();
}
```

## 5. 显示容量参考

### 5.1 单行显示容量

| 字体类型 | 字符宽度 | 屏幕宽度 | 单行字符数 |
|---------|---------|---------|-----------|
| 6x8     | 6像素   | 128像素 | ~21个字符 |
| 8x16    | 8像素   | 128像素 | 16个字符  |

### 5.2 全屏显示容量

| 字体类型 | 行高度 | 屏幕高度 | 最多行数 | 总字符数 |
|---------|--------|---------|---------|---------|
| 6x8     | 8像素  | 64像素  | 8行     | 168个字符 |
| 8x16    | 16像素 | 64像素  | 4行     | 64个字符  |

### 5.3 内存占用

| 项目 | 大小 |
|-----|------|
| 6x8字库（95×6字节） | 570字节 |
| 8x16字库（95×16字节） | 1520字节 |
| 总字库大小 | 2090字节 |
| 显示缓冲（128×64/8） | 1024字节 |
| 总内存需求 | ~3114字节 |

## 6. 高级用法

### 6.1 动态生成文本

```c
void DisplayDynamicValue(int sensor_id, float value)
{
    char buffer[20];
    
    // 使用sprintf动态生成字符串
    sprintf(buffer, "Sensor%d: %.2f", sensor_id, value);
    
    OLED_ShowString(0, 0, buffer, OLED_FONT_6x8, OLED_COLOR_WHITE);
}
```

### 6.2 自定义字符排列

```c
void DisplayFormattedData(void)
{
    OLED_Clear();
    
    // 标签左对齐
    OLED_ShowString(0, 0, "Temperature:", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 数值右对齐（手动计算X坐标）
    OLED_ShowFloat(90, 0, 23.45f, 2, OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 单位
    OLED_ShowString(110, 0, "C", OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    OLED_Refresh();
}
```

### 6.3 条件显示

```c
void DisplayWithCondition(float temperature)
{
    OLED_Clear();
    
    OLED_ShowString(0, 0, "Temp: ", OLED_FONT_6x8, OLED_COLOR_WHITE);
    OLED_ShowFloat(36, 0, temperature, 2, OLED_FONT_6x8, OLED_COLOR_WHITE);
    
    // 根据温度显示不同状态
    if (temperature > 30.0f)
    {
        OLED_ShowString(0, 10, "WARNING: TOO HOT!", OLED_FONT_6x8, OLED_COLOR_WHITE);
    }
    else if (temperature < 10.0f)
    {
        OLED_ShowString(0, 10, "WARNING: TOO COLD!", OLED_FONT_6x8, OLED_COLOR_WHITE);
    }
    else
    {
        OLED_ShowString(0, 10, "Status: OK", OLED_FONT_6x8, OLED_COLOR_WHITE);
    }
    
    OLED_Refresh();
}
```

## 7. 常见问题

### Q1: 能否显示中文？
**A:** 当前字库仅支持ASCII字符（0x20-0x7E），不支持中文。若需要中文显示，需要创建单独的中文字库数据。

### Q2: 浮点数的精度问题？
**A:** ShowFloat函数支持0-3位小数。超过3位时，自动截断。示例：
- `OLED_ShowFloat(0, 0, 3.14159, 2, OLED_FONT_6x8, WHITE)` → 显示 "3.14"

### Q3: 如何显示非常大的数字？
**A:** 使用8x16字体，单行最多显示16个字符，可显示：
- 整数：-2,147,483,647（32位有符号整数上限）
- 浮点数：可显示的长度取决于整数部分+小数部分

### Q4: 能否修改字体？
**A:** 可以修改OLED_Font.h中的字库数据，但需要保持数据结构不变（6x8: 95×6字节，8x16: 95×16字节）。

### Q5: 显示速度如何优化？
**A:** 
- 只刷新改变的区域，而不是全屏刷新
- 使用较小的字体（6x8）提高刷新速度
- 减少OLED_Refresh()的调用频率

## 8. 总结

| 特性 | 说明 |
|------|------|
| **支持字体** | 6x8, 8x16 |
| **字符范围** | ASCII 0x20-0x7E（95个字符） |
| **数字显示** | 支持整数、负数、浮点数（0-3位小数） |
| **内存占用** | ~3KB（包括字库和缓冲） |
| **显示分辨率** | 128×64像素 |
| **刷新速度** | I2C: ~50ms/屏; SPI: ~10ms/屏 |

---
