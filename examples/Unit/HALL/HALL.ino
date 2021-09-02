/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: Hall.  霍尔传感器
* date：2021/8/18
*******************************************************************************
  Please connect to Port ,Displays a string on the screen.
  请连接端口,在屏幕上显示字符串。
  Low-level signal can be generated when the magnet S pole is close to the front of the sensor
  当磁体S极靠近传感器前端时，会产生低电平信号
  OR the N pole is close to the back, and the internal LED indicator will light up, the screen wiil display 0.
  或N极靠近背面，内部LED指示灯亮起，屏幕显示0。
*/

#include <M5StickC.h>

#define HALL 33

void setup() {
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.setRotation(3); //Rotating display.  旋转显示屏
  M5.Lcd.print("     HALL Sensor");
  pinMode(HALL, INPUT); //Set the pins to which the Hall sensor is connected to the input mode.  将霍尔传感器所连接的引脚设置为输入模式
}

void loop() {
  bool status = digitalRead(HALL);
  M5.Lcd.setCursor(20, 30);
  M5.Lcd.printf("Hall status : %d", status);
}
