#include "Servo.h"
#include "main.h"

// 添加宏定义（如果头文件中没有）
#define SERVO_MIN_PULSE 500    // 0度对应的脉冲宽度(us)
#define SERVO_MAX_PULSE 2500   // 180度对应的脉冲宽度(us)
#define PWM_PERIOD 20000        // PWM周期计数值(20ms)

SERVO_PARA servoPara[SERVONUM] = {
    {TOP_SERVO,    500, 1},
    {MIDDLE_SERVO, 500, 2},
    {BOTTOM_SERVO, 500, 3},
    {CLAW_ANGLE,   500, 4},
    {CLAW_STATUS,  500, 5}
};

uint8_t angleOffset = 8;




void setChannelPwm(int channel, uint16_t pwm) {
    switch(channel) {
        case 1:
            SERVO_PWM1 = pwm;
            break;
        case 2:
            SERVO_PWM2 = pwm;
            break;
        case 3:
            SERVO_PWM3 = pwm;
            break;
        case 4:
            SERVO_PWM4 = pwm;
            break;
        default:
            break;
    }
}

uint16_t addServo(uint16_t actionPwm, uint16_t upLimit) {
    if(actionPwm + angleOffset < upLimit) {
        return actionPwm + angleOffset;
    } else {
        return upLimit;
    }
}

uint16_t reduceServo(uint16_t actionPwm, uint16_t downLimit) {
    if(actionPwm - angleOffset > downLimit) {
        return actionPwm - angleOffset;
    } else {
        return downLimit;
    }
}

void actionBasic(Servo_Num servoNum, uint16_t actionPwm) {
    uint16_t action_servo_old = servoPara[servoNum].pwm;
    uint16_t (* DJ_ChangeAngle)(uint16_t actionPwm, uint16_t Limit);

    if(action_servo_old == actionPwm) {
        setChannelPwm(servoPara[servoNum].channel, actionPwm);
        return;
    }
    
    if(action_servo_old < actionPwm) {
        DJ_ChangeAngle = addServo;
    } else {
        DJ_ChangeAngle = reduceServo;
    }

    uint16_t newPwm = DJ_ChangeAngle(action_servo_old, actionPwm);
    while(newPwm != actionPwm) {
        setChannelPwm(servoPara[servoNum].channel, newPwm);
        HAL_Delay(10);
        newPwm = DJ_ChangeAngle(newPwm, actionPwm);
    }
    setChannelPwm(servoPara[servoNum].channel, actionPwm);
    servoPara[servoNum].pwm = actionPwm;
}

// 修改后的角度设置函数
void setServoAngle(Servo_Num servoNum, float target_Angle,uint8_t speed) {
		angleOffset = speed;
    // 将角度限制在0-180度范围内
    if(target_Angle < 0) target_Angle = 0;
    if(target_Angle > 180) target_Angle = 180;
    
    // 正确计算PWM脉冲宽度（单位：微秒）
    float pulseWidth = SERVO_MIN_PULSE + (target_Angle / 180.0) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE);
    
    // 直接使用脉冲宽度作为CCR值（因为每个计数=1us）
    uint16_t pulseNUM = (uint16_t)pulseWidth;
    
    actionBasic(servoNum, pulseNUM);
}

void resetAllServo(void)
{
    for(int i = 0; i < SERVONUM; i++)
    {
        setServoAngle((Servo_Num)i, 90, 8); // 中间位置
    }
}
