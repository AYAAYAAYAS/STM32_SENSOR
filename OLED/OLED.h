#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "OLED_Font.h"
#include <stdint.h>

/* =================== OLED 显示模式选择 =================== */
#define OLED_INTERFACE_I2C      1
#define OLED_INTERFACE_SPI      0
#define OLED_INTERFACE          OLED_INTERFACE_I2C

/* =================== I2C 模式配置 =================== */
#if (OLED_INTERFACE == OLED_INTERFACE_I2C)

// I2C类型选择：硬件IIC或软件IIC
#define OLED_I2C_HARDWARE       0
#define OLED_I2C_SOFTWARE       1
#define OLED_I2C_TYPE           OLED_I2C_SOFTWARE   // 选择软件IIC

#if (OLED_I2C_TYPE == OLED_I2C_HARDWARE)
    // 硬件IIC配置
    #define OLED_I2C_HANDLE         hi2c1               // I2C句柄
    #define OLED_I2C_ADDR           0x78                // OLED I2C地址
#else
    // 软件IIC配置
    #define OLED_I2C_ADDR           0x78                // OLED I2C地址
    #define OLED_SDA_GPIO_PORT      GPIOB               // SDA引脚GPIO端口（可改为GPIOA）
    #define OLED_SDA_PIN            GPIO_PIN_8          // SDA引脚（可改为其他引脚）
    #define OLED_SCL_GPIO_PORT      GPIOB               // SCL引脚GPIO端口（可改为GPIOA）
    #define OLED_SCL_PIN            GPIO_PIN_9          // SCL引脚（可改为其他引脚）
#endif

#endif

/* =================== SPI 模式配置 =================== */
#if (OLED_INTERFACE == OLED_INTERFACE_SPI)
#define OLED_SPI_HANDLE         hspi1               // SPI句柄
#define OLED_CS_GPIO_PORT       GPIOA               // CS引脚GPIO端口
#define OLED_CS_PIN             GPIO_PIN_4          // CS引脚
#define OLED_DC_GPIO_PORT       GPIOA               // DC引脚GPIO端口
#define OLED_DC_PIN             GPIO_PIN_3          // DC引脚
#define OLED_RST_GPIO_PORT      GPIOA               // RST引脚GPIO端口
#define OLED_RST_PIN            GPIO_PIN_2          // RST引脚
#endif

/* =================== OLED 显示参数 =================== */
#define OLED_WIDTH              128                 // 屏幕宽度（像素）
#define OLED_HEIGHT             64                  // 屏幕高度（像素）
#define OLED_PAGE               8                   // 页数（64/8=8页）

/* =================== OLED 命令定义 =================== */
#define OLED_CMD_SETCONTRAST            0x81        // 设置对比度
#define OLED_CMD_DISPLAYALL_ON          0xA5        // 全屏点亮
#define OLED_CMD_DISPLAYALL_OFF         0xA4        // 正常显示
#define OLED_CMD_NORMAL_DISPLAY         0xA6        // 正常显示
#define OLED_CMD_INVERSE_DISPLAY        0xA7        // 反色显示
#define OLED_CMD_DISPLAY_ON             0xAF        // 打开显示
#define OLED_CMD_DISPLAY_OFF            0xAE        // 关闭显示
#define OLED_CMD_SETPAGEADDR            0xB0        // 设置页地址
#define OLED_CMD_SETLOWCOLUMN           0x00        // 设置列地址（低位）
#define OLED_CMD_SETHIGHCOLUMN          0x10        // 设置列地址（高位）
#define OLED_CMD_SETSTARTLINE           0x40        // 设置起始行
#define OLED_CMD_SEGMENT_REMAP_NORMAL   0xA0        // 列地址0为左
#define OLED_CMD_SEGMENT_REMAP_REVERSE  0xA1        // 列地址0为右
#define OLED_CMD_COM_OUTPUT_NORMAL      0xC0        // COM输出方向正常
#define OLED_CMD_COM_OUTPUT_REVERSE     0xC8        // COM输出方向反向
#define OLED_CMD_SETDISPLAYOFFSET       0xD3        // 设置显示偏移
#define OLED_CMD_SETDISPLAYFREQUENCY    0xD5        // 设置显示频率
#define OLED_CMD_SETPRECHARGEPERIOD     0xD9        // 设置预充电周期
#define OLED_CMD_SET_VCOMH              0xDB        // 设置VCOMH电压
#define OLED_CMD_NOP                    0xE3        // 无操作
#define OLED_CMD_CHARGEPUMP             0x8D        // 电荷泵设置

/* =================== 字体大小定义 =================== */
#define OLED_FONT_6x8       0                       // 6x8像素字体
#define OLED_FONT_8x16      1                       // 8x16像素字体

/* =================== 颜色定义 =================== */
#define OLED_COLOR_BLACK    0                       // 黑色（关闭）
#define OLED_COLOR_WHITE    1                       // 白色（点亮）

typedef struct
{
    uint8_t buffer[OLED_PAGE * OLED_WIDTH];         // 显存缓冲区
    uint8_t is_initialized;                         // 初始化标志
} OLED_HandleTypeDef;

/* =================== OLED 函数声明 =================== */

/**
 * @brief 初始化OLED显示器
 * @retval 0: 成功, 非0: 失败
 */
uint8_t OLED_Init(void);

/**
 * @brief 关闭OLED显示器
 * @retval 无
 */
void OLED_DeInit(void);

/**
 * @brief 清屏（全黑）
 * @retval 无
 */
void OLED_Clear(void);

/**
 * @brief 填充屏幕（全白）
 * @retval 无
 */
void OLED_Fill(void);

/**
 * @brief 刷新显示（将缓冲区内容显示到OLED）
 * @retval 无
 */
void OLED_Refresh(void);

/**
 * @brief 打开显示
 * @retval 无
 */
void OLED_DisplayOn(void);

/**
 * @brief 关闭显示
 * @retval 无
 */
void OLED_DisplayOff(void);

/**
 * @brief 反色显示
 * @retval 无
 */
void OLED_InverseDisplay(void);

/**
 * @brief 正常显示
 * @retval 无
 */
void OLED_NormalDisplay(void);

/**
 * @brief 设置对比度
 * @param contrast: 对比度值 (0~255)
 * @retval 无
 */
void OLED_SetContrast(uint8_t contrast);

/**
 * @brief 绘制像素点
 * @param x: 水平坐标 (0~127)
 * @param y: 垂直坐标 (0~63)
 * @param color: 颜色 (OLED_COLOR_BLACK 或 OLED_COLOR_WHITE)
 * @retval 无
 */
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief 获取像素点状态
 * @param x: 水平坐标 (0~127)
 * @param y: 垂直坐标 (0~63)
 * @retval 像素状态 (0或1)
 */
uint8_t OLED_GetPixel(uint8_t x, uint8_t y);

/**
 * @brief 绘制直线
 * @param x0, y0: 起点坐标
 * @param x1, y1: 终点坐标
 * @param color: 颜色
 * @retval 无
 */
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

/**
 * @brief 绘制矩形（空心）
 * @param x, y: 左上角坐标
 * @param width, height: 宽和高
 * @param color: 颜色
 * @retval 无
 */
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

/**
 * @brief 绘制填充矩形
 * @param x, y: 左上角坐标
 * @param width, height: 宽和高
 * @param color: 颜色
 * @retval 无
 */
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

/**
 * @brief 绘制圆形（空心）
 * @param x, y: 圆心坐标
 * @param radius: 半径
 * @param color: 颜色
 * @retval 无
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color);

/**
 * @brief 绘制填充圆形
 * @param x, y: 圆心坐标
 * @param radius: 半径
 * @param color: 颜色
 * @retval 无
 */
void OLED_FillCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color);

/**
 * @brief 显示单个字符
 * @param x, y: 显示位置
 * @param ch: 字符
 * @param font: 字体大小 (OLED_FONT_6x8 或 OLED_FONT_8x16)
 * @param color: 颜色
 * @retval 无
 */
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t font, uint8_t color);

/**
 * @brief 显示字符串
 * @param x, y: 显示位置
 * @param str: 字符串指针
 * @param font: 字体大小
 * @param color: 颜色
 * @retval 无
 */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t font, uint8_t color);

/**
 * @brief 显示整数
 * @param x, y: 显示位置
 * @param num: 整数值
 * @param font: 字体大小
 * @param color: 颜色
 * @retval 无
 */
void OLED_ShowInt(uint8_t x, uint8_t y, int32_t num, uint8_t font, uint8_t color);

/**
 * @brief 显示浮点数
 * @param x, y: 显示位置
 * @param num: 浮点数
 * @param decimal_places: 小数位数
 * @param font: 字体大小
 * @param color: 颜色
 * @retval 无
 */
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t decimal_places, uint8_t font, uint8_t color);

/**
 * @brief 在指定位置显示位图
 * @param x, y: 起始位置
 * @param width, height: 位图宽和高（以像素计）
 * @param bitmap: 位图数据指针
 * @retval 无
 */
void OLED_ShowBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap);

OLED_HandleTypeDef* OLED_GetHandle(void);


#endif /* __OLED_H */
