/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：OP 180/90.  非接触式光电限位开关
* date：2021/8/16
*******************************************************************************
  Please connect to Port,Detect the current OP 90/180 Unit Photoelectric switch status.
  请连接端口,检测当前OP 90/180单元光电开关状态。
*/

#include <M5StickC.h>

void setup() {
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.setCursor(30, 10); //Set the cursor at (30,10).  将光标设置在(30,10)处
  M5.Lcd.println("90/180 OPTICAL");
  pinMode(33,INPUT_PULLUP); //Set pin 33 to input pull-up mode.  设置33号引脚为输入上拉模式
}

void loop() {
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.printf("IR Receive: %d",digitalRead(33));  //Output the value of pin 33.  输出33号引脚的值
}
