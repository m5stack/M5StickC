#ifndef _M5SERVO_H_
#define _M5SERVO_H_

#include <M5StickC.h>

#define SERVO_ADDRESS	0X38
void IIC_Servo_Init();	//sda  0     scl  26		

uint8_t Servo_angle_set(uint8_t Servo_CH,uint8_t angle);

uint8_t Servo_pulse_set(uint8_t Servo_CH,uint16_t width);

uint8_t RGB_set(uint8_t R,uint8_t G,uint8_t B);

uint8_t Servo_angle_read(uint8_t Servo_CH);

uint16_t Servo_pulse_read(uint8_t Servo_CH);

uint32_t RGB_read();



#endif
