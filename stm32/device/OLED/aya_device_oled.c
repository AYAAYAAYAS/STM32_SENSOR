/**
 ******************************************************************************
 * @file    OLED.c
 * @brief   OLED SSD1306显示驱动实现
 * 
 * 支持两种通信接口：
 * - I2C模式（推荐）：通过I2C与OLED通信
 * - SPI模式：通过SPI与OLED通信
 ******************************************************************************
 */

#include "OLED.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* =================== 全局变量定义 =================== */
static OLED_HandleTypeDef oled_handle = {
    .is_initialized = 0
};

/* =================== 软件IIC相关宏和函数 =================== */
#if (OLED_INTERFACE == OLED_INTERFACE_I2C) && (OLED_I2C_TYPE == OLED_I2C_SOFTWARE)

// 软件IIC延迟函数（微秒级）
static void SoftI2C_Delay(uint16_t us)
{
    // 使用简单的循环延迟，可根据CPU频率调整
    for (uint16_t i = 0; i < us * 8; i++)
        __NOP();
}

// 设置SDA和SCL为输入（开漏）
static void SoftI2C_SetSDA(uint8_t state)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (state)
    {
        // SDA设为输入（浮空，外接上拉电阻）
        GPIO_InitStruct.Pin = OLED_SDA_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(OLED_SDA_GPIO_PORT, &GPIO_InitStruct);
    }
    else
    {
        // SDA设为输出低电平
        HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT, OLED_SDA_PIN, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = OLED_SDA_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(OLED_SDA_GPIO_PORT, &GPIO_InitStruct);
    }
}

static void SoftI2C_SetSCL(uint8_t state)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (state)
    {
        // SCL设为输入（浮空，外接上拉电阻）
        GPIO_InitStruct.Pin = OLED_SCL_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(OLED_SCL_GPIO_PORT, &GPIO_InitStruct);
    }
    else
    {
        // SCL设为输出低电平
        HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = OLED_SCL_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(OLED_SCL_GPIO_PORT, &GPIO_InitStruct);
    }
}

// IIC启动条件
static void SoftI2C_Start(void)
{
    SoftI2C_SetSDA(1);
    SoftI2C_SetSCL(1);
    SoftI2C_Delay(4);
    SoftI2C_SetSDA(0);
    SoftI2C_Delay(4);
    SoftI2C_SetSCL(0);
    SoftI2C_Delay(4);
}

// IIC停止条件
static void SoftI2C_Stop(void)
{
    SoftI2C_SetSCL(0);
    SoftI2C_Delay(4);
    SoftI2C_SetSDA(0);
    SoftI2C_Delay(4);
    SoftI2C_SetSCL(1);
    SoftI2C_Delay(4);
    SoftI2C_SetSDA(1);
    SoftI2C_Delay(4);
}

// IIC发送一个字节
static void SoftI2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            SoftI2C_SetSDA(1);
        else
            SoftI2C_SetSDA(0);
        
        SoftI2C_Delay(2);
        SoftI2C_SetSCL(1);
        SoftI2C_Delay(4);
        SoftI2C_SetSCL(0);
        SoftI2C_Delay(2);
        
        byte <<= 1;
    }
}

// IIC接收应答位
static void SoftI2C_RecvACK(void)
{
    SoftI2C_SetSDA(1);
    SoftI2C_Delay(2);
    SoftI2C_SetSCL(1);
    SoftI2C_Delay(4);
    SoftI2C_SetSCL(0);
    SoftI2C_Delay(2);
}

#endif

/* =================== 内部函数声明 =================== */
static void OLED_WriteCommand(uint8_t cmd);
static void OLED_WriteData(uint8_t data);
static void OLED_WriteDataBuf(const uint8_t *buf, uint16_t len);
static void OLED_SetPosition(uint8_t page, uint8_t column);

/* =================== 函数实现 =================== */

/**
 * @brief 写入命令字节
 */
static void OLED_WriteCommand(uint8_t cmd)
{
#if (OLED_INTERFACE == OLED_INTERFACE_I2C)

#if (OLED_I2C_TYPE == OLED_I2C_HARDWARE)
    uint8_t data[2];
    data[0] = 0x00;      // 控制字节（Co=0, D/C=0表示命令）
    data[1] = cmd;       // 命令数据
    
    HAL_I2C_Master_Transmit(&OLED_I2C_HANDLE, OLED_I2C_ADDR, data, 2, 100);
    
#else
    // 软件IIC方式
    SoftI2C_Start();
    SoftI2C_SendByte(OLED_I2C_ADDR);     // 发送地址+写命令
    SoftI2C_RecvACK();
    SoftI2C_SendByte(0x00);              // 控制字节（D/C=0表示命令）
    SoftI2C_RecvACK();
    SoftI2C_SendByte(cmd);               // 命令数据
    SoftI2C_RecvACK();
    SoftI2C_Stop();
#endif

#elif (OLED_INTERFACE == OLED_INTERFACE_SPI)
    HAL_GPIO_WritePin(OLED_DC_GPIO_PORT, OLED_DC_PIN, GPIO_PIN_RESET);  // DC=0 表示命令
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&OLED_SPI_HANDLE, &cmd, 1, 100);
    
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_SET);
#endif
}

/**
 * @brief 写入数据字节
 */
static void OLED_WriteData(uint8_t data)
{
#if (OLED_INTERFACE == OLED_INTERFACE_I2C)

#if (OLED_I2C_TYPE == OLED_I2C_HARDWARE)
    uint8_t buf[2];
    buf[0] = 0x40;       // 控制字节（Co=0, D/C=1表示数据）
    buf[1] = data;       // 数据
    
    HAL_I2C_Master_Transmit(&OLED_I2C_HANDLE, OLED_I2C_ADDR, buf, 2, 100);
    
#else
    // 软件IIC方式
    SoftI2C_Start();
    SoftI2C_SendByte(OLED_I2C_ADDR);     // 发送地址+写命令
    SoftI2C_RecvACK();
    SoftI2C_SendByte(0x40);              // 控制字节（D/C=1表示数据）
    SoftI2C_RecvACK();
    SoftI2C_SendByte(data);              // 数据字节
    SoftI2C_RecvACK();
    SoftI2C_Stop();
#endif

#elif (OLED_INTERFACE == OLED_INTERFACE_SPI)
    HAL_GPIO_WritePin(OLED_DC_GPIO_PORT, OLED_DC_PIN, GPIO_PIN_SET);    // DC=1 表示数据
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&OLED_SPI_HANDLE, &data, 1, 100);
    
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_SET);
#endif
}

/**
 * @brief 写入多个数据字节
 */
static void OLED_WriteDataBuf(const uint8_t *buf, uint16_t len)
{
#if (OLED_INTERFACE == OLED_INTERFACE_I2C)

#if (OLED_I2C_TYPE == OLED_I2C_HARDWARE)
    uint8_t *data = (uint8_t *)malloc(len + 1);
    if (data == NULL) return;
    
    data[0] = 0x40;      // 控制字节
    memcpy(&data[1], buf, len);
    
    HAL_I2C_Master_Transmit(&OLED_I2C_HANDLE, OLED_I2C_ADDR, data, len + 1, 100);
    
    free(data);
    
#else
    // 软件IIC方式 - 逐字节发送
    SoftI2C_Start();
    SoftI2C_SendByte(OLED_I2C_ADDR);     // 发送地址+写命令
    SoftI2C_RecvACK();
    SoftI2C_SendByte(0x40);              // 控制字节（D/C=1表示数据）
    SoftI2C_RecvACK();
    
    for (uint16_t i = 0; i < len; i++)
    {
        SoftI2C_SendByte(buf[i]);        // 发送每个数据字节
        SoftI2C_RecvACK();
    }
    
    SoftI2C_Stop();
#endif

#elif (OLED_INTERFACE == OLED_INTERFACE_SPI)
    HAL_GPIO_WritePin(OLED_DC_GPIO_PORT, OLED_DC_PIN, GPIO_PIN_SET);    // DC=1 表示数据
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&OLED_SPI_HANDLE, (uint8_t *)buf, len, 100);
    
    HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_PIN, GPIO_PIN_SET);
#endif
}

/**
 * @brief 设置显示位置（页和列）
 */
static void OLED_SetPosition(uint8_t page, uint8_t column)
{
    OLED_WriteCommand(OLED_CMD_SETPAGEADDR | page);                   // 设置页地址
    OLED_WriteCommand(OLED_CMD_SETLOWCOLUMN | (column & 0x0F));       // 设置列地址低4位
    OLED_WriteCommand(OLED_CMD_SETHIGHCOLUMN | ((column >> 4) & 0x0F)); // 设置列地址高4位
}

/**
 * @brief 初始化OLED显示器
 */
uint8_t OLED_Init(void)
{
#if (OLED_INTERFACE == OLED_INTERFACE_SPI)
    // 复位OLED
    HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
#endif
    
    // 初始化序列
    OLED_WriteCommand(OLED_CMD_DISPLAY_OFF);                   // 0xAE 关闭显示
    OLED_WriteCommand(OLED_CMD_SETDISPLAYFREQUENCY);           // 0xD5
    OLED_WriteCommand(0x80);                                    // 频率设置值
    OLED_WriteCommand(OLED_CMD_SETDISPLAYOFFSET);              // 0xD3
    OLED_WriteCommand(0x00);                                    // 偏移设置值
    OLED_WriteCommand(OLED_CMD_SETSTARTLINE);                  // 0x40 起始行
    OLED_WriteCommand(OLED_CMD_SEGMENT_REMAP_NORMAL);          // 0xA0 列地址扫描方向
    OLED_WriteCommand(OLED_CMD_COM_OUTPUT_NORMAL);             // 0xC0 COM扫描方向
    OLED_WriteCommand(OLED_CMD_SET_VCOMH);                     // 0xDB
    OLED_WriteCommand(0x40);                                    // VCOMH电压
    OLED_WriteCommand(OLED_CMD_SETCONTRAST);                   // 0x81 对比度
    OLED_WriteCommand(0xFF);                                    // 对比度值
    OLED_WriteCommand(OLED_CMD_CHARGEPUMP);                    // 0x8D
    OLED_WriteCommand(0x14);                                    // 启用电荷泵
    OLED_WriteCommand(OLED_CMD_NORMAL_DISPLAY);                // 0xA6 正常显示
    
    OLED_Clear();
    OLED_WriteCommand(OLED_CMD_DISPLAY_ON);                    // 0xAF 打开显示
    
    oled_handle.is_initialized = 1;
    
    return 0;
}

/**
 * @brief 关闭OLED显示器
 */
void OLED_DeInit(void)
{
    OLED_WriteCommand(OLED_CMD_DISPLAY_OFF);
    oled_handle.is_initialized = 0;
}

/**
 * @brief 清屏（全黑）
 */
void OLED_Clear(void)
{
    uint8_t page, column;
    
    for (page = 0; page < OLED_PAGE; page++)
    {
        OLED_SetPosition(page, 0);
        
        for (column = 0; column < OLED_WIDTH; column++)
        {
            OLED_WriteData(0x00);
            oled_handle.buffer[page * OLED_WIDTH + column] = 0x00;
        }
    }
}

/**
 * @brief 填充屏幕（全白）
 */
void OLED_Fill(void)
{
    uint8_t page, column;
    
    for (page = 0; page < OLED_PAGE; page++)
    {
        OLED_SetPosition(page, 0);
        
        for (column = 0; column < OLED_WIDTH; column++)
        {
            OLED_WriteData(0xFF);
            oled_handle.buffer[page * OLED_WIDTH + column] = 0xFF;
        }
    }
}

/**
 * @brief 刷新显示（将缓冲区内容显示到OLED）
 */
void OLED_Refresh(void)
{
    uint8_t page;
    
    for (page = 0; page < OLED_PAGE; page++)
    {
        OLED_SetPosition(page, 0);
        OLED_WriteDataBuf(&oled_handle.buffer[page * OLED_WIDTH], OLED_WIDTH);
    }
}

/**
 * @brief 打开显示
 */
void OLED_DisplayOn(void)
{
    OLED_WriteCommand(OLED_CMD_DISPLAY_ON);
}

/**
 * @brief 关闭显示
 */
void OLED_DisplayOff(void)
{
    OLED_WriteCommand(OLED_CMD_DISPLAY_OFF);
}

/**
 * @brief 反色显示
 */
void OLED_InverseDisplay(void)
{
    OLED_WriteCommand(OLED_CMD_INVERSE_DISPLAY);
}

/**
 * @brief 正常显示
 */
void OLED_NormalDisplay(void)
{
    OLED_WriteCommand(OLED_CMD_NORMAL_DISPLAY);
}

/**
 * @brief 设置对比度
 */
void OLED_SetContrast(uint8_t contrast)
{
    OLED_WriteCommand(OLED_CMD_SETCONTRAST);
    OLED_WriteCommand(contrast);
}

/**
 * @brief 绘制像素点
 */
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t page, bit;
    uint16_t index;
    
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
        return;
    
    page = y / 8;
    bit = y % 8;
    index = page * OLED_WIDTH + x;
    
    if (color == OLED_COLOR_WHITE)
    {
        oled_handle.buffer[index] |= (1 << bit);    // 置位
    }
    else
    {
        oled_handle.buffer[index] &= ~(1 << bit);   // 清位
    }
}

/**
 * @brief 获取像素点状态
 */
uint8_t OLED_GetPixel(uint8_t x, uint8_t y)
{
    uint8_t page, bit;
    uint16_t index;
    
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
        return 0;
    
    page = y / 8;
    bit = y % 8;
    index = page * OLED_WIDTH + x;
    
    return (oled_handle.buffer[index] >> bit) & 1;
}

/**
 * @brief 绘制直线（Bresenham算法）
 */
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    int8_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int8_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int8_t sx = (x1 > x0) ? 1 : -1;
    int8_t sy = (y1 > y0) ? 1 : -1;
    int16_t err = dx - dy;
    int8_t x = x0, y = y0;
    
    while (1)
    {
        OLED_DrawPixel(x, y, color);
        
        if (x == x1 && y == y1)
            break;
        
        int16_t e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

/**
 * @brief 绘制矩形（空心）
 */
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    OLED_DrawLine(x, y, x + width - 1, y, color);                      // 上边
    OLED_DrawLine(x, y + height - 1, x + width - 1, y + height - 1, color); // 下边
    OLED_DrawLine(x, y, x, y + height - 1, color);                     // 左边
    OLED_DrawLine(x + width - 1, y, x + width - 1, y + height - 1, color); // 右边
}

/**
 * @brief 绘制填充矩形
 */
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    uint8_t i, j;
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            OLED_DrawPixel(x + j, y + i, color);
        }
    }
}

/**
 * @brief 绘制圆形（空心）
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color)
{
    int16_t a = 0, b = radius;
    int16_t d = 3 - 2 * radius;
    
    while (a <= b)
    {
        OLED_DrawPixel(x + a, y + b, color);
        OLED_DrawPixel(x - a, y + b, color);
        OLED_DrawPixel(x + a, y - b, color);
        OLED_DrawPixel(x - a, y - b, color);
        OLED_DrawPixel(x + b, y + a, color);
        OLED_DrawPixel(x - b, y + a, color);
        OLED_DrawPixel(x + b, y - a, color);
        OLED_DrawPixel(x - b, y - a, color);
        
        if (d < 0)
        {
            d = d + 4 * a + 6;
        }
        else
        {
            d = d + 4 * (a - b) + 10;
            b--;
        }
        a++;
    }
}

/**
 * @brief 绘制填充圆形
 */
void OLED_FillCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color)
{
    int16_t a = 0, b = radius;
    int16_t d = 3 - 2 * radius;
    
    while (a <= b)
    {
        OLED_DrawLine(x - a, y - b, x + a, y - b, color);
        OLED_DrawLine(x - a, y + b, x + a, y + b, color);
        OLED_DrawLine(x - b, y - a, x + b, y - a, color);
        OLED_DrawLine(x - b, y + a, x + b, y + a, color);
        
        if (d < 0)
        {
            d = d + 4 * a + 6;
        }
        else
        {
            d = d + 4 * (a - b) + 10;
            b--;
        }
        a++;
    }
}

/**
 * @brief 显示单个字符
 */
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t font, uint8_t color)
{
    uint8_t i, j;
    uint8_t byte;
    
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
        return;
    
    if (ch < 0x20 || ch > 0x7E)
        return;  // 仅支持ASCII 0x20-0x7E
    
    uint8_t offset = ch - 0x20;
    
    if (font == OLED_FONT_6x8)
    {
        // 6x8字体显示
        for (i = 0; i < 6; i++)
        {
            byte = OLED_Font6x8[offset][i];
            
            for (j = 0; j < 8; j++)
            {
                if (byte & (1 << j))
                {
                    OLED_DrawPixel(x + i, y + j, color);
                }
                else
                {
                    OLED_DrawPixel(x + i, y + j, 1 - color);  // 背景色
                }
            }
        }
    }
    else if (font == OLED_FONT_8x16)
    {
        // 8x16字体显示
        for (i = 0; i < 16; i++)
        {
            byte = OLED_Font8x16[offset][i];
            
            for (j = 0; j < 8; j++)
            {
                if (byte & (1 << j))
                {
                    OLED_DrawPixel(x + (i % 8), y + (i / 8) * 8 + j, color);
                }
                else
                {
                    OLED_DrawPixel(x + (i % 8), y + (i / 8) * 8 + j, 1 - color);
                }
            }
        }
    }
}

/**
 * @brief 显示字符串
 */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t font, uint8_t color)
{
    uint8_t x_pos = x;
    uint8_t char_width;
    
    if (str == NULL)
        return;
    
    if (font == OLED_FONT_6x8)
        char_width = 6;
    else if (font == OLED_FONT_8x16)
        char_width = 8;
    else
        return;
    
    while (*str != '\0')
    {
        if (x_pos + char_width > OLED_WIDTH)
            break;  // 超出屏幕宽度
        
        OLED_ShowChar(x_pos, y, *str, font, color);
        x_pos += char_width;
        str++;
    }
}

/**
 * @brief 显示整数
 */
void OLED_ShowInt(uint8_t x, uint8_t y, int32_t num, uint8_t font, uint8_t color)
{
    char str[16];
    
    if (num < 0)
    {
        OLED_ShowChar(x, y, '-', font, color);
        num = -num;
        x += (font == OLED_FONT_6x8) ? 6 : 8;
    }
    
    // 处理零
    if (num == 0)
    {
        OLED_ShowChar(x, y, '0', font, color);
        return;
    }
    
    // 反转数字以便从最高位开始显示
    int32_t temp = num;
    uint8_t digit_count = 0;
    
    while (temp > 0)
    {
        digit_count++;
        temp /= 10;
    }
    
    // 显示每一位数字
    for (int i = digit_count - 1; i >= 0; i--)
    {
        int32_t divisor = 1;
        for (int j = 0; j < i; j++)
            divisor *= 10;
        
        char digit = '0' + (num / divisor) % 10;
        OLED_ShowChar(x, y, digit, font, color);
        x += (font == OLED_FONT_6x8) ? 6 : 8;
    }
}

/**
 * @brief 显示浮点数
 */
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t decimal_places, uint8_t font, uint8_t color)
{
    char str[32];
    
    // 处理负数
    if (num < 0)
    {
        OLED_ShowChar(x, y, '-', font, color);
        num = -num;
        x += (font == OLED_FONT_6x8) ? 6 : 8;
    }
    
    // 获取整数部分
    int32_t int_part = (int32_t)num;
    
    // 获取小数部分
    float decimal_part = num - int_part;
    
    // 构建格式字符串
    char format[16];
    
    if (decimal_places == 0)
    {
        sprintf(format, "%%d");
    }
    else if (decimal_places == 1)
    {
        sprintf(format, "%%d.%%1d");
    }
    else if (decimal_places == 2)
    {
        sprintf(format, "%%d.%%2d");
    }
    else if (decimal_places == 3)
    {
        sprintf(format, "%%d.%%3d");
    }
    else
    {
        sprintf(format, "%%.%df", decimal_places);
    }
    
    // 使用标准sprintf如果支持浮点格式
    if (decimal_places <= 3)
    {
        // 手动处理小数部分以避免浮点格式字符串
        int32_t decimal_int = (int32_t)(decimal_part * powf(10, decimal_places));
        
        if (decimal_places == 0)
        {
            sprintf(str, "%d", int_part);
        }
        else if (decimal_places == 1)
        {
            sprintf(str, "%d.%1d", int_part, decimal_int);
        }
        else if (decimal_places == 2)
        {
            sprintf(str, "%d.%02d", int_part, decimal_int);
        }
        else if (decimal_places == 3)
        {
            sprintf(str, "%d.%03d", int_part, decimal_int);
        }
    }
    else
    {
        // 对于更多小数位，使用近似方式
        sprintf(str, "%d", int_part);
    }
    
    OLED_ShowString(x, y, str, font, color);
}

/**
 * @brief 在指定位置显示位图
 */
void OLED_ShowBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap)
{
    uint8_t i, j;
    
    if (bitmap == NULL)
        return;
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            uint16_t index = i * width + j;
            uint8_t byte = bitmap[index / 8];
            uint8_t bit = index % 8;
            
            if (byte & (1 << bit))
            {
                OLED_DrawPixel(x + j, y + i, OLED_COLOR_WHITE);
            }
        }
    }
}

/**
 * @brief 获取OLED句柄
 */
OLED_HandleTypeDef* OLED_GetHandle(void)
{
    return &oled_handle;
}
