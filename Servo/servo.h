#ifndef __servo_h_
#define __servo_h_

#include "main.h"
enum SERVO_TYPE{
    SERVO_180,
    SERVO_270
};

typedef struct Servo {
    GPTIMER_Regs *gptimer;
    DL_TIMER_CC_INDEX ccIndex;
    uint32_t value;
    float angle;
    int type;
} Servo;
void Set_SerVertical_Angle(unsigned int angle);
void Set_SerLevel_Angle(unsigned int angle);
unsigned int Get_SerVert_Angle(void);
unsigned int Get_SerLEve_Angle(void);

Servo Servo_Create(
    GPTIMER_Regs *gptimer,
    DL_TIMER_CC_INDEX ccIndex,
    int value,
    int type
);
void Servo_SetAngle(Servo* servo, float angle);
float Servo_GetAngle(Servo* servo);
#endif /* __BSP_SG90_H__ */


