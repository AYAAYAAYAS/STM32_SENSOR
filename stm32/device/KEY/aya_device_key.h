#ifndef  __key_h
#define  __key_h

#include "ALL.h"
#define LONGPRESSMS 1000
#define PRESSMS 20
#define DOUBLEPRESSMS 300
typedef enum{
   B1_KEY=0,
   B2_KEY,
   B3_KEY,
   B4_KEY,
  KEY_COUNT
}KEY_ID;

typedef enum
{
  key_release=0,
  key_press,
  key_longpress,
  key_doubleclick
}Key_StateTypedef;

typedef struct{
  #ifdef TI_USE
  GPIO_Regs* gpio;
  uint32_t pins;
  
  #endif

  #ifdef STM32_USE_HAL_DRIVER
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
  #endif

  uint8_t Down;
  uint32_t press_tick;
  uint32_t release_tick;
  Key_StateTypedef key_state;
  uint16_t click_cnt;
}KEY_HandleTypedef;

extern KEY_HandleTypedef keyhandle[KEY_COUNT];
extern int key_value;
extern uint16_t key_count;
void KEY_Get(KEY_HandleTypedef *hkey);
int KEY_FLAG_Get();
#ifdef STM32_USE_HAL_DRIVER
void KEY_Init(KEY_HandleTypedef *hkey,GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
#endif

#ifdef TI_USE
void KEY_Init(KEY_HandleTypedef *hkey,GPIO_Regs *gpio,uint32_t pins);
#endif

#endif