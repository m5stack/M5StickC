#ifndef _M5SERVO_H_
#define _M5SERVO_H_

#include <M5StickC.h>

#define SERVO_ADDRESS	0X38

void IIC_Servo_Init();	//sda  0     scl  26		

//Servo_CH: 1-8 angle:  0-180
uint8_t Servo_angle_set(uint8_t Servo_CH,uint8_t angle);         
//Servo_CH: 1-8 width(us):  500-2500    
uint8_t Servo_pulse_set(uint8_t Servo_CH,uint16_t width);
//R: 0-255  G:  0-255   B:  0-255
void RGB_set(uint8_t R,uint8_t G,uint8_t B);

uint8_t Servo_angle_read(uint8_t Servo_CH);

uint16_t Servo_pulse_read(uint8_t Servo_CH);

uint32_t RGB_read();


#endif
