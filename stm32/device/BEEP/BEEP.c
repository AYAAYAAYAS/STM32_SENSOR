#include "Sensor_head.h"
void Buzzer_init(void)
{
    // gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);
    
}

void Buzzer_On(void)
{
	HAL_GPIO_WritePin(GPIOA, BEEP_Pin, 1);
}
void Buzzer_Off(void)
{
	HAL_GPIO_WritePin(GPIOA, BEEP_Pin, 0);
}
void Buzzer_check(int time2)
{
    HAL_GPIO_WritePin(GPIOA, BEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(time2);
    HAL_GPIO_WritePin(GPIOA, BEEP_Pin, GPIO_PIN_RESET);
    // gpio_set_level(BUZZER_PIN,1);
    // system_delay_ms(time2);
    // gpio_set_level(BUZZER_PIN,0);
}

void LED_ON(int time1)
{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_Delay(time1);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}