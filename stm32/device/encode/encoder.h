/*
 * encoder.h
 *
 *  Created on: 2025年3月30日
 *      Author: lenovo
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_

#define ENCODER_1                   (TIM2_ENCODER)
#define ENCODER_1_A                 (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_1_B                 (TIM2_ENCODER_CH2_P33_6)

#define WHEEL_RADIUS    0.036f   // 车轮半径0.036米
#define PULSE_PER_ROUND 4096.0f// 编码器每转脉冲数

typedef struct{
        int32 count;    //编码器脉冲计数
        double distance; //行驶距离（米）
        double speed;    //当前速度（m/s）
}Encoder_Data;

typedef struct{
        double x,y;      //二维坐标
        double theta;    //航向角
        double v;        //合成速度
}Nav_State;

extern int encoder_pit;
extern Encoder_Data encoder;
extern Nav_State nav_state;
extern double Get_Yaw[];
extern double Get_distance[];
extern int16 NUM;
extern int flag ;
extern double L ,H;
extern double Left_Angle ,Right_Angle;
extern int Point;

void encoder_init(void);
void encoder_process(void);
void dead_reckoning(void);
void path_record(void);
void Key_Get_pistion(void);
void Show_Pistion(void);
void GuanDao_XunJi(void);
void guandaocundian(void);


#endif /* CODE_ENCODER_H_ */
