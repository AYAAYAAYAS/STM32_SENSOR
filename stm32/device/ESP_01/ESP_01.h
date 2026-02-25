#ifndef __ESP_01_H
#define __ESP_01_H

#define ESP_UART &huart2
#define ESP_BUFFER_SIZE 512
#define TIMEOUT_MS 3000

typedef enum {
    ESP_OK = 0,
    ESP_ERROR,
    ESP_TIMEOUT,
    ESP_CONNECT_FAIL
} ESP_StatusTypeDef;

// 初始化函数
ESP_StatusTypeDef ESP01_Init(void);
ESP_StatusTypeDef ESP01_ConnectWiFi(const char* ssid, const char* password);
ESP_StatusTypeDef ESP01_ConnectServer(const char* ip, uint16_t port);
ESP_StatusTypeDef ESP01_SendData(const char* data);
void ESP01_ReceiveData(void);
// AT指令发送函数
ESP_StatusTypeDef ESP_SendATCommand(const char* cmd, const char* expected, uint32_t timeout);
void ESP_ClearBuffer(void);
uint8_t ESP_WaitResponse(const char* expected, uint32_t timeout);

#endif