#ifndef _BOARD__H_
#define _BOARD__H_

#include <Arduino.h>

extern void led_set(uint8_t num, uint32_t color_in);
extern void led_set_all(uint32_t color_in);
extern uint16_t light_get();

#endif