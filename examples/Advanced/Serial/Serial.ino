/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Stick-C sample source code
*                          配套  M5Stick-C 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：Serial.  串口发送接收
* date：2021/7/27
*******************************************************************************
*/

#include <M5StickC.h>

void setup() {
  M5.begin(); //Init M5Stick-C(Serial initialization is also included, the baud rate is 115200).  初始化M5Stick-C(串口初始化也包含在内,波特率为115200)
}

void loop() {
  if(Serial.available()) {  //If serial port is available.  如果串口可用
    int ch = Serial.read(); //Store the data read by serial port to ch.  将串口读取到到数据存储至ch中
    Serial.write(ch); //Send data from ch to serial port.  将ch中的数据发送到串口
  }
}
