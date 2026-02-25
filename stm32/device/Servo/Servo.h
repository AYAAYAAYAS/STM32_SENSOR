#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"
#define SET_ARR 20000-1
#define SERVO_SIGN_MAX 2500
#define SERVO_SIGN_MIN 500

#define SERVO_PWM1 TIM2->CCR1
#define SERVO_PWM2 TIM2->CCR2
#define SERVO_PWM3 TIM2->CCR3
#define SERVO_PWM4 TIM2->CCR4

typedef enum 
{
    TOP_SERVO = 0,
    MIDDLE_SERVO,
    BOTTOM_SERVO,
    CLAW_ANGLE,
    CLAW_STATUS,
    SERVONUM
}Servo_Num;

typedef struct 
{
    Servo_Num Servo_num;
    uint16_t pwm;
    const uint8_t channel;
}SERVO_PARA;

void setServoAngle(Servo_Num servoNum, float target_Angle,uint8_t speed);
void resetAllServo(void);
extern SERVO_PARA servoPara[SERVONUM];



#endif // __SERVO_H
