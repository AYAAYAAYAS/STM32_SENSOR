#include "gps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char *TAG = "GPS";
// 缓冲区大小
#define GPS_UART_BUFFER_SIZE  (1024 * 2)
#define GPS_LINE_BUFFER_SIZE  128

static gps_data_t s_gps_data = {0};
static motion_state_t s_last_motion = MOTION_STATIC;
static bool s_motion_changed = false;
static SemaphoreHandle_t s_data_mutex = NULL;
static void parse_gnrmc(char *line, gps_data_t *data);
static void gps_read_task(void *pvParameters);


void gps_init(int baud_rate)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_param_config(gps_uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(gps_uart_num, gps_tx_pin , gps_rx_pin,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(gps_uart_num, GPS_UART_BUFFER_SIZE,
                                        GPS_UART_BUFFER_SIZE, 10, NULL, 0));
    s_data_mutex = xSemaphoreCreateMutex();
}

void gps_start_task(void)
{
    xTaskCreate(gps_read_task, "gps_read", 4096, NULL, 5, NULL);
}

// 解析 $GNRMC 语句
static void parse_gnrmc(char *line, gps_data_t *data)
{
    char *ptr = line;
    int field = 0;
    char temp[20];
    int i = 0;
    int tmp_year;

    while (*ptr != '\0' && field <= 12) {
        if (*ptr == ',' || *ptr == '*') {
            temp[i] = '\0';

            switch (field) {
                case 1: // UTC时间 hhmmss.sss
                    if (strlen(temp) >= 6) {
                        data->hour   = (temp[0]-'0')*10 + (temp[1]-'0');
                        data->minute = (temp[2]-'0')*10 + (temp[3]-'0');
                        data->second = (temp[4]-'0')*10 + (temp[5]-'0');
                    }
                    break;
                case 2: // 定位状态 A=有效，V=无效
                    data->valid = (temp[0] == 'A') ? 1 : 0;
                    break;
                case 3: // 纬度 ddmm.mmmm
                    if (strlen(temp) > 0) {
                        double raw_lat = atof(temp);
                        int degrees = (int)(raw_lat / 100);
                        double minutes = raw_lat - degrees * 100;
                        data->latitude = degrees + minutes / 60.0;
                    }
                    break;
                case 4: // 北纬/南纬 N/S
                    data->ns_indicator = temp[0];
                    break;
                case 5: // 经度 dddmm.mmmm
                    if (strlen(temp) > 0) {
                        double raw_lon = atof(temp);
                        int degrees = (int)(raw_lon / 100);
                        double minutes = raw_lon - degrees * 100;
                        data->longitude = degrees + minutes / 60.0;
                    }
                    break;
                case 6: // 东经/西经 E/W
                    data->ew_indicator = temp[0];
                    break;
                case 7: // 地面速率（节）
                    data->speed = atof(temp);
                    break;
                case 9: // 日期 ddmmyy
                    if (strlen(temp) >= 6) {
                        data->day   = (temp[0]-'0')*10 + (temp[1]-'0');
                        data->month = (temp[2]-'0')*10 + (temp[3]-'0');
                        tmp_year    = (temp[4]-'0')*10 + (temp[5]-'0');
                        data->year  = 2000 + tmp_year;
                    }
                    break;
            }
            field++;
            i = 0;
        } else {
            temp[i++] = *ptr;
        }
        ptr++;
    }
}

static void gps_read_task(void *pvParameters)
{
    uint8_t *line_buffer = malloc(GPS_LINE_BUFFER_SIZE);
    uint8_t *data = malloc(GPS_UART_BUFFER_SIZE);
    gps_data_t new_data = {0};
    motion_state_t motion;

    while (1) {
        int len = uart_read_bytes(gps_uart_num, data, GPS_UART_BUFFER_SIZE,
                                  pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            char *start = strstr((char *)data, "$GNRMC");
            if (start) {
                char *end = strchr(start, '\n');
                if (end) {
                    int line_len = end - start;
                    if (line_len < GPS_LINE_BUFFER_SIZE) {
                        memcpy(line_buffer, start, line_len);
                        line_buffer[line_len] = '\0';
                        parse_gnrmc((char *)line_buffer, &new_data);
                        if (s_data_mutex) {
                            xSemaphoreTake(s_data_mutex, portMAX_DELAY);
                            memcpy(&s_gps_data, &new_data, sizeof(gps_data_t));
                            xSemaphoreGive(s_data_mutex);
                        }
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    free(line_buffer);
    free(data);
    vTaskDelete(NULL);
}

bool gps_get_data(gps_data_t *out_data)
{
    if (!out_data) return false;
    if (!s_data_mutex) return false;

    xSemaphoreTake(s_data_mutex, portMAX_DELAY);
    memcpy(out_data, &s_gps_data, sizeof(gps_data_t));
    xSemaphoreGive(s_data_mutex);

    return (out_data->valid != 0);
}

motion_state_t gps_get_motion_state(void)
{
    if (!s_data_mutex) return MOTION_STATIC;

    motion_state_t state;
    xSemaphoreTake(s_data_mutex, portMAX_DELAY);
    if (s_gps_data.valid && s_gps_data.speed > GPS_SPEED_STATIC_THRESHOLD) {
        state = MOTION_MOVING;
    } else {
        state = MOTION_STATIC;
    }
    xSemaphoreGive(s_data_mutex);
    return state;
}

bool gps_motion_state_changed(motion_state_t *out_state)
{
    motion_state_t current = gps_get_motion_state();
    if (out_state) *out_state = current;

    bool changed = (current != s_last_motion);
    s_last_motion = current;
    return changed;
}