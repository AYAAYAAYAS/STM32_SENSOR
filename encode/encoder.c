/*
 * encoder.c
 *
 *  Created on: 2025年3月30日
 *      Author: lenovo
 */
#include "zf_common_headfile.h"

Encoder_Data encoder;
Nav_State nav_state;
double Get_Yaw[40];
double Get_distance[40];
int16 NUM = 0;
int Point = 0;
double error = 0;
int flag = 0;
double L = 0 ,H = 2;   //L：小车与锥桶水平对齐时，小车距离锥桶之间的距离   H：两锥桶之间的距离
double Left_Angle ,Right_Angle;
int encoder_pit = 0;

void encoder_init(void)
{
    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
}
//----------------------编码器处理-------------------------
void encoder_process(void)
{

    int32 delta = -encoder_get_count(ENCODER_1);
    encoder.distance += ((float)delta /PULSE_PER_ROUND) *(2 * USER_PI * WHEEL_RADIUS);

    //计算瞬时速度
    encoder.speed = (delta /PULSE_PER_ROUND) * (2 * USER_PI * WHEEL_RADIUS) / 0.005;
    encoder_clear_count(ENCODER_1);
}
//---------------------航位推算---------------------------
void dead_reckoning(void) {
    // 使用编码器速度和航向角推算位置
    nav_state.v = encoder.speed;
    nav_state.theta = Yaw;

    nav_state.x += nav_state.v * cos(nav_state.theta) * 0.001;
    nav_state.y += nav_state.v * sin(nav_state.theta) * 0.001;
}

//------------------- 路径记录 ----------------------------
void path_record(void) {

    Get_Yaw[NUM] = Yaw;
    Get_distance[NUM] = encoder.distance;

    ips200_show_string(1, 16*5,"Yaw");      ips200_show_float(100, 16*5, Yaw, 3,2);
    ips200_show_string(1, 16*6,"distance"); ips200_show_float(100, 16*6, encoder.distance, 3,2);
    ips200_show_string(1, 16*7, "NUM:");    ips200_show_uint(100,16*7,NUM+1,3);
    NUM++;

       if(NUM>40)//如果采集数大于最大采集数
       {
           NUM=40;//防止数组越界
       }
}

void Key_Get_pistion(void)
{
    if(key1_flag)
    {

        while(key1_flag)
        {
            ips200_show_string(1, 16*1,"Yaw");      ips200_show_float(100, 16*1, Yaw, 3,2);
            ips200_show_string(1, 16*2,"distance"); ips200_show_float(100, 16*2, encoder.distance, 3,2);
            if(key2_flag)
            {
                path_record();
                key2_flag = 0;
            }
            if(key3_flag)
            {
                key3_flag = 0;
                key1_flag = 0;
                ips200_clear();
                flag = 1;
                break;
            }
        }
    }
}

void Show_Pistion(void)
{
        uint8 tag = 0;           // 用于控制显示的标签
        while(TRUE)
        {

            ips200_show_string(0, 16*1, "Yaw");         ips200_show_float(90,16*1,Get_Yaw[tag],3,6);
            ips200_show_string(0, 16*2, "Distance");    ips200_show_float(90,16*11,Get_distance[tag],3,6);
            ips200_show_string(0, 16*3, "tag:");       ips200_show_uint(90, 16*3, tag, 2);

            if(key1_flag == 1) { key1_flag = 0; tag++; }
            if(key2_flag == 1) { key2_flag = 0; tag--; }
            if(key3_flag == 1) { key3_flag = 0;ips200_clear(); break; }
            if(tag > 10) { tag = 0; }
        }
}
void point_Change(void) // 切换点位，依据与目标点的距离判断是否切换
{
     if((encoder.distance>(Get_distance[Point] -0.2)) && (encoder.distance<(Get_distance[Point] +0.2)))   {Point++ ;}

     if(Point == 8) {Overall_Motor = 500;}

}
void GuanDao_XunJi(void)
{

    error = angle_diff(Get_Yaw[Point],Yaw);
    angle_plan(&Error);
    Steer_PID_Control(0, error, &pid_gps);
    point_Change();

    ips200_show_string(0, 16*1, "Point");           ips200_show_uint(100, 16*1, Point, 3);
    ips200_show_string(0, 16*2, "Yaw");             ips200_show_float(100, 16*2, Yaw, 3, 2);
    ips200_show_string(0, 16*3, "Distance");        ips200_show_float(100, 16*3, encoder.distance, 3, 3);
    ips200_show_string(0, 16*4, "error");           ips200_show_float(100, 16*4, error, 3, 3);
    ips200_show_string(0, 16*5, "next_Yaw");        ips200_show_float(100, 16*5, Get_Yaw[Point], 3, 6);
    ips200_show_string(0, 16*6, "next_Di");         ips200_show_float(100, 16*6, Get_distance[Point], 3, 6);
}



