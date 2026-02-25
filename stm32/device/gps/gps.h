#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 运动检测阈值（节），1节 ≈ 1.852 km/h
#define GPS_SPEED_STATIC_THRESHOLD  0.5f   

#define gps_uart_num  UART_NUM_1
#define gps_tx_pin  0   
#define gps_rx_pin  1


typedef struct {
    double longitude;          // 经度
    double latitude;           // 纬度
    float speed;               // 地面速率（节）
    char ns_indicator;         // 'N' 或 'S'
    char ew_indicator;         // 'E' 或 'W'
    uint8_t hour;              // 时 (UTC)
    uint8_t minute;            // 分
    uint8_t second;            // 秒
    uint8_t day;               // 日
    uint8_t month;             // 月
    uint16_t year;             // 年
    uint8_t valid;             // 0=无效, 1=有效
} gps_data_t;

typedef enum {
    MOTION_STATIC = 0,         
    MOTION_MOVING              
} motion_state_t;


void gps_init(int baud_rate);
void gps_start_task(void);

bool gps_get_data(gps_data_t *out_data);
motion_state_t gps_get_motion_state(void);
bool gps_motion_state_changed(motion_state_t *out_state);

#ifdef __cplusplus
}
#endif

#endif // GPS_H