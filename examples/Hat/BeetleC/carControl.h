#ifndef __CARCONTROL_H
#define __CARCONTROL_H

#include <Arduino.h>
#include <Wire.h>

void leftwheel(uint8_t val);
void rightwheel(uint8_t val);
void led(uint8_t num, uint32_t val);

#endif 
