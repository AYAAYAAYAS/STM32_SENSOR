#include "KEY.h"

KEY_HandleTypedef keyhandle[KEY_COUNT];
uint16_t key_count;
int key_value;
#ifdef STM32_USE_HAL_DRIVER
void KEY_Init(KEY_HandleTypedef *hkey, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  hkey->GPIOx = GPIOx;
  hkey->GPIO_Pin = GPIO_Pin;
  hkey->key_state = key_release;
  hkey->click_cnt = 0;
  hkey->Down = 0;
  hkey->press_tick = 0;
  hkey->release_tick = 0;
}
void KEY_Get(KEY_HandleTypedef *hkey)
{
#ifdef RT_THREAD
  uint32_t now = rt_tick_get();
#else
  uint32_t now = HAL_GetTick();
#endif
  if (hkey->click_cnt == 1 && (now - hkey->release_tick) > DOUBLEPRESSMS)
  {
    hkey->click_cnt = 0;
    hkey->key_state = key_press;
  }

  if (HAL_GPIO_ReadPin(hkey->GPIOx, hkey->GPIO_Pin) == GPIO_PIN_RESET)
  {
    if (!hkey->Down)
    {
      hkey->Down = 1;
      hkey->press_tick = now;
    }
    else if ((now - hkey->press_tick) >= LONGPRESSMS && hkey->key_state != key_longpress)
    {
      hkey->key_state = key_longpress;
    }
  }
  else
  {
    if (hkey->Down)
    {
      uint32_t delta = now - hkey->press_tick;
      if (delta >= PRESSMS && delta < LONGPRESSMS)
      {
        if (hkey->click_cnt == 0)
        {
          hkey->release_tick = now;
          hkey->click_cnt = 1;
        }
        else
        {
          if ((now - hkey->release_tick) <= DOUBLEPRESSMS)
          {
            hkey->key_state = key_doubleclick;
          }
          hkey->click_cnt = 0;
        }
      }
      hkey->Down = 0;
    }
  }
}
#endif

#ifdef TI_USE
void KEY_Init(KEY_HandleTypedef *hkey, GPIO_Regs *gpio, uint32_t pins)
{
  hkey->gpio = gpio;
  hkey->pins = pins;
  hkey->key_state = key_release;
  hkey->click_cnt = 0;
  hkey->Down = 0;
  hkey->press_tick = 0;
  hkey->release_tick = 0;
}
void KEY_Get(KEY_HandleTypedef *hkey)
{
  // uint32_t now = RTCC_GetTime();
  uint32_t now = key_count; 
  if (hkey->click_cnt == 1 && (now - hkey->release_tick) > DOUBLEPRESSMS)
  {
    hkey->click_cnt = 0;
    hkey->key_state = key_press;
  }

  if (DL_GPIO_readPins(hkey->gpio, hkey->pins) == 0)
  {
    if (!hkey->Down)
    {
      hkey->Down = 1;
      hkey->press_tick = now;
    }
    else if ((now - hkey->press_tick) >= LONGPRESSMS && hkey->key_state != key_longpress)
    {
      hkey->key_state = key_longpress;
    }
  }
  else
  {
    if (hkey->Down)
    {
      uint32_t delta = now - hkey->press_tick;
      if (delta >= PRESSMS && delta < LONGPRESSMS)
      {
        if (hkey->click_cnt == 0)
        {
          hkey->release_tick = now;
          hkey->click_cnt = 1;
        }
        else
        {
          if ((now - hkey->release_tick) <= DOUBLEPRESSMS)
          {
            hkey->key_state = key_doubleclick;
          }
          hkey->click_cnt = 0;
        }
      }
      hkey->Down = 0;
    }
  }
}
#endif
int KEY_FLAG_Get()
{
  for (int i = 0; i < 4; i++)
  {
    KEY_Get(&keyhandle[i]);
  }
  if(keyhandle[0].key_state == key_press)
  {
     key_value = 1;
    keyhandle[0].key_state = key_release;
  }
  else if(keyhandle[1].key_state == key_press)
  {
    key_value= 2;
    keyhandle[1].key_state = key_release;
  }
  else if(keyhandle[2].key_state == key_press)
  {
    key_value = 3;
    keyhandle[2].key_state = key_release;
  }
  else if(keyhandle[3].key_state == key_press)
  {
   key_value = 4;
    keyhandle[3].key_state = key_release;
  }
  else
  {
    key_value = 0;
  } 
   return key_value;
}


