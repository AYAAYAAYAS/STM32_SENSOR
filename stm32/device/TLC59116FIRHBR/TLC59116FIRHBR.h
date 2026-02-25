#ifndef __TLC59116_H__
#define __TLC59116_H__



#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "i2c.h"


//extern I2C_HandleTypeDef hi2c1;


#define TLC59116_BASEADDR   0b1100000
#define TLC59116_ALLCALL    0b1101000
#define TLC59116_SUB1       0b1101001
#define TLC59116_SUB2       0b1101010
#define TLC59116_SUB3       0b1101100
#define TLC59116_RESET      0b1101011

#define TLC59116_MODE1      0x00
#define TLC59116_MODE2      0x01
#define TLC59116_PWM0       0x02
#define TLC59116_PWM1       0x03
#define TLC59116_PWM2       0x04
#define TLC59116_PWM3       0x05
#define TLC59116_PWM4       0x06
#define TLC59116_PWM5       0x07
#define TLC59116_PWM6       0x08
#define TLC59116_PWM7       0x09
#define TLC59116_PWM8       0x0A
#define TLC59116_PWM9       0x0B
#define TLC59116_PWM10      0x0C
#define TLC59116_PWM11      0x0D
#define TLC59116_PWM12      0x0E
#define TLC59116_PWM13      0x0F
#define TLC59116_PWM14      0x10
#define TLC59116_PWM15      0x11
#define TLC59116_GRPPWM     0x12
#define TLC59116_GRPFREQ    0x13
#define TLC59116_LEDOUT0    0x14
#define TLC59116_LEDOUT1    0x15
#define TLC59116_LEDOUT2    0x16
#define TLC59116_LEDOUT3    0x17
#define TLC59116_SUBADR1    0x18
#define TLC59116_SUBADR2    0x19
#define TLC59116_SUBADR3    0x1A
#define TLC59116_ALLCALLADR 0x1B
#define TLC59116_IREF       0x1C
#define TLC59116_EFLAG1     0x1D
#define TLC59116_EFLAG2     0x1E

#define TLC59116_1    0xc0    //1100 0000
//#define TLC59116_2    0xc2     //1100 0010
#define TLC59116_2    0xd0     //1101 0000   广播地址，所有灯都亮
#define TLC59116_3    0xc4     //1100 0100


HAL_StatusTypeDef I2C_WriteData(uint16_t DevAddress, uint16_t RegAddress, uint8_t RegSize, uint8_t Data);
// 读取I2C设备的指定寄存器
uint8_t Read_Reg(uint8_t DevAddress, uint8_t RegAddress) ;
void TLC59116_Init(uint8_t DevAddress);
void TLC59116_Init_OFF(void);
#endif

