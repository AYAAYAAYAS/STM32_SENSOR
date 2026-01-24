/*
 * PID.c
 *
 *  Created on: 2025年3月30日
 *      Author: lenovo
 */

#include "zf_common_headfile.h"

PID_STEER pid_gps;//舵机pid
PID_STEER pid_speed;//电机pid
PID_STEER* pid_list[3];

double A=0.78,B=0.069,C=50,D=2;
double *Point_KP, *Point_KI, *Point_KD;

double constrain_float(double amt,double low, double high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}
/* 舵机PID初始化 */
void Servo_PID_Init(double a,double b)
{
    pid_gps.kp = a;
    pid_gps.kd =b;
    pid_gps.steer_error_previous = 0;
    pid_gps.PID_result = 0;
}
/* 速度PID初始化 */
void Speed_PID_Init(double kp, double kd)
{
    pid_speed.kp = kp;
    pid_speed.kd = kd;
    pid_speed.steer_error_previous = 0;
    pid_speed.PID_result = 0;
}
//位置式PID
double Loc_PID(double expect_Angle, double Actual_Angle, PID_STEER *pid)
{
    pid->steer_error = Actual_Angle;
    pid->PID_result = pid->kp * pid->steer_error + pid->kd * (pid->steer_error - pid->steer_error_previous);
    // 保存误差
    pid->steer_error_previous = pid->steer_error;
    // 返回PID舵机值
    if(pid->PID_result<-45) {pid->PID_result =-45;}
    return pid->PID_result;
}
uint32 steer_duty = 0;
void Steer_PID_Control(double expect_Angle,double Actual_Angle, PID_STEER *pid)
{
    steer_duty = SERVO_MOTOR_MID + (int32)Loc_PID(expect_Angle,Actual_Angle, pid);
    Steer_Set(steer_duty);
}
double Speed_PID(double target_speed, double current_speed, PID_STEER *pid)
{
    // 计算速度误差
       pid->steer_error = target_speed - current_speed;

    // PD控制计算
    pid->PID_result = pid->kp * pid->steer_error +
                  pid->kd * (pid->steer_error - pid->steer_error_previous);

    // 保存误差用于下次计算
    pid->steer_error_previous = pid->steer_error;

    // 输出限幅
    pid->PID_result = constrain_float(pid->PID_result, -MOTOR_OUTPUT_MAX, MOTOR_OUTPUT_MAX);

    return pid->PID_result;

}
uint32 speed_pwm = 0;
void Speed_PID_Control(double target_speed, double current_speed, PID_STEER *pid)
{
    speed_pwm = 0;

    speed_pwm = MOTOR_PWM + (int32)Speed_PID(target_speed, current_speed, pid);
    if (speed_pwm > MOTOR_MAX_DUTY)  speed_pwm = MOTOR_MAX_DUTY;

    Speed_Set(speed_pwm);
    //ips200_show_int(50, 16*4, steer_duty, 2);
}
void Key_Change_PID(void)
{
    ips200_show_string(0, 16*18, "A"); ips200_show_float(100, 16*18, A, 3, 2);
    ips200_show_string(0, 16*19, "B"); ips200_show_float(100, 16*19, B, 3, 2);
    if(key1_flag)
    {
        A +=0.02;  key1_flag =0;
    }
    if(key2_flag)
    {
        A -=0.02;  key2_flag = 0;
    }
    if(key3_flag)
    {
        B +=0.005;  key3_flag =0;
    }
    if(key4_flag)
    {
        B-=0.005;   key4_flag = 0;
    }
}
void Control_straight(void)
{
        Overall_Motor = 5300;
        Error = angle_diff(0,Yaw);
        Steer_PID_Control(0, -Error, &pid_gps);
        ips200_show_int(0, 16*15, pit, 4);
        ips200_show_int(0, 16*16, next_point, 4);
        Distance = get_two_points_distance(gnss.latitude,gnss.longitude,GPS_GET_LAT[1],GPS_GET_LON[1]);
        if(Distance < Distance01) {  Start_flag = 2;  ips200_clear(); }
}

