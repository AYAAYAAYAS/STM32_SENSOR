#include "servo.h"


// unsigned int SerVert_Angle = 0;
// unsigned int SerLEve_Angle = 0;
// void Set_SerVertical_Angle(unsigned int angle)
// {
//     if(angle > 180)
//     {
//         angle = 180;
//     }
//     SerVert_Angle = angle;

//     float min_pwm = 320.0f;
//     float max_pwm = 2600.0f;
//     float range = max_pwm - min_pwm;
//     float ServoPWM = min_pwm + (((float)angle / 180.0f) * range);

//     DL_Timer_setCaptureCompareValue(TIMA0, (unsigned int)(ServoPWM + 0.5f), DL_TIMER_CC_1_INDEX);
// }
// void Set_SerLevel_Angle(unsigned int angle)
// {
//     if(angle > 270)
//     {
//         angle = 270;
//     }
//     SerLEve_Angle = angle;

//     float min_pwm = 320.0f;
//     float max_pwm = 2600.0f;
//     float range = max_pwm - min_pwm;
//     float ServoPWM = min_pwm + (((float)angle / 270.0f) * range);

//     DL_Timer_setCaptureCompareValue(TIMA0, (unsigned int)(ServoPWM + 0.5f), DL_TIMER_CC_0_INDEX );
// }

// /******************************************************************
//  * 函 数 名 称：读取当前角度
//  * 函 数 说 明：Get_Servo_Angle
//  * 函 数 形 参：无
//  * 函 数 返 回：当前角度
//  * 作       者：LCKFB
//  * 备       注：使用前必须确保之前使用过
//                 void Set_Servo_Angle(unsigned int angle)
//                 函数设置过角度
// ******************************************************************/
// unsigned int Get_SerVert_Angle(void)
// {
//     return SerVert_Angle;
// }
// unsigned int Get_SerLEve_Angle(void)
// {
//     return SerLEve_Angle;
// }
Servo Servo_Create(
    GPTIMER_Regs *gptimer,
    DL_TIMER_CC_INDEX ccIndex,
    int value,
    int type
){
    Servo servo ={
        .gptimer = gptimer,
        .ccIndex = ccIndex,
        .value = value,
        .angle=0,
        .type = SERVO_180
    };
    if(type==SERVO_270){
        servo.type = SERVO_270;
    }
    DL_Timer_setLoadValue(gptimer,20000-1);
    DL_Timer_setCaptureCompareValue(gptimer,value,ccIndex);
    DL_Timer_setCaptureCompareValue(gptimer,value,ccIndex);
    DL_Timer_startCounter(gptimer);
    return servo;
}

__WEAK void Servo_SetAngle(Servo* servo, float angle){
    if(servo->type==SERVO_180){
        servo->angle = angle;
        float min_pwm = 320.0f;
        float max_pwm = 2600.0f;
        float range = max_pwm - min_pwm;
        servo->value = min_pwm + (((float)angle / 180.0f) * range);
        DL_Timer_setCaptureCompareValue(servo->gptimer, (unsigned int)(servo->value + 0.5f), servo->ccIndex);
    } else if(servo->type==SERVO_270){
        servo->angle = angle;
        float min_pwm = 320.0f;
        float max_pwm = 2600.0f;
        float range = max_pwm - min_pwm;
        servo->value = min_pwm + (((float)angle / 270.0f) * range);
        DL_Timer_setCaptureCompareValue(servo->gptimer, (unsigned int)(servo->value + 0.5f), servo->ccIndex);
    }
}

__WEAK float Servo_GetAngle(Servo* servo){
    return servo->angle;
}
