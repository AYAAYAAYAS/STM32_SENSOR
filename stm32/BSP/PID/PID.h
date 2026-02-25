/*
 * PID.h
 *
 *  Created on: 2025年3月30日
 *      Author: lenovo
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#define MOTOR_OUTPUT_MAX 500  // 根据电机实际最大输出调整
#define MOTOR_PWM        1400//设置电机的占空
#define MOTOR_MAX_DUTY   2500 //电机最大占空比

//PID结构体
typedef struct
{
    double kp;           //比例系数Kp
    double kd;           //微分系数Kd

    double steer_error;         // pid误差
    double steer_current;       // 当前角度
    double steer_error_previous;// 上一次的误差
    double PID_result;
}PID_STEER;
extern PID_STEER* pid_list[3];
extern PID_STEER pid_gps;
extern PID_STEER pid_speed;

extern double A,B,C,D;
extern double *Point_KP, *Point_KI, *Point_KD;
extern uint32 speed_pwm;
extern uint32 steer_duty;
//函数声明
double constrain_float(double amt, double low, double high);//限幅函数
void Servo_PID_Init(double a,double b);
void Speed_PID_Init(double kp, double kd);
//舵机
double Loc_PID(double expect_Angle, double Actual_Angle, PID_STEER *pid);
void Steer_PID_Control(double expect_Angle,double Actual_Angle, PID_STEER *pid);
//电机
double Speed_PID(double target_speed, double current_speed, PID_STEER *pid);
void Speed_PID_Control(double expect_Angle,double Actual_Angle, PID_STEER *pid);
void Key_Change_PID(void);
void Control_straight(void);


#endif /* CODE_PID_H_ */
