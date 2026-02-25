#include "TLC59116FIRHBR.h"





// 写入数据到I2C设备
HAL_StatusTypeDef I2C_WriteData(uint16_t DevAddress, uint16_t RegAddress, uint8_t RegSize, uint8_t Data) 
{
    HAL_StatusTypeDef status;
    uint8_t buffer[256];
    uint16_t i;
 
    buffer[0] = RegAddress; // 设定寄存器地址
    for (i = 1; i < RegSize + 1; i++) {
        buffer[i] = Data; // 设定要写入的数据
    }
 
    status = HAL_I2C_Mem_Write(&hi2c1, DevAddress, RegAddress, I2C_MEMADD_SIZE_8BIT, buffer, RegSize + 1, 1000);
    return status;
}


// 读取I2C设备的指定寄存器
uint8_t Read_Reg(uint8_t DevAddress, uint8_t RegAddress) 
{
  HAL_StatusTypeDef status;
  uint8_t value;
  
  status = HAL_I2C_Mem_Read(&hi2c1, DevAddress, RegAddress, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
  if (status != HAL_OK) {
    Error_Handler();
  }
  return value;
}




void TLC59116_Init(uint8_t DevAddress)
{
	I2C_WriteData(DevAddress, TLC59116_MODE1, 1, 0x01);
	I2C_WriteData(DevAddress, TLC59116_MODE2, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_LEDOUT0, 1, 0xff);
	I2C_WriteData(DevAddress, TLC59116_LEDOUT1, 1, 0xff);
	I2C_WriteData(DevAddress, TLC59116_LEDOUT2, 1, 0xff);
	I2C_WriteData(DevAddress, TLC59116_LEDOUT3, 1, 0xff);
	
	
	I2C_WriteData(DevAddress, TLC59116_PWM0, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM1, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM2, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM3, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM4, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM5, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM6, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM6, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM8, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM9, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM10, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM11, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM12, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM13, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM14, 1, 0x00);
	I2C_WriteData(DevAddress, TLC59116_PWM15, 1, 0x00);

}

void TLC59116_Init_OFF(void)
{
	I2C_WriteData(TLC59116_2, TLC59116_PWM0, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM1, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM2, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM3, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM4, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM5, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM6, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM7, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM8, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM9, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM10, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM11, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM12, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM13, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM14, 1, 0x00);
	I2C_WriteData(TLC59116_2, TLC59116_PWM15, 1, 0x00);
}
