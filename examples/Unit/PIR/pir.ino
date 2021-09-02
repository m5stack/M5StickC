/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：PIR.  人体红外
* date：2021/8/11
*******************************************************************************
  Please connect to Port,Human body detection using PIR Unit.
  请连接端口,使用PIR Unit进行人体检测。
*/

#include <M5StickC.h>

void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.println("PIR example");
  M5.Lcd.setCursor(0, 25);  //Position the cursor at (0,25).  将光标固定在(0,25)
  M5.Lcd.println("Status: \n\nValue: ");
  pinMode(33, INPUT); //Set pin 36 to input mode.  设置33号引脚为输入模式
}

void loop() {
  M5.Lcd.fillRect(45,25,180,50,BLACK);  //Draw a black rectangle 180 by 50 at (90,25).  在(90,25)处画一个宽180高50的黑的矩形
  if(digitalRead(33)==1){ //If pin 33 reads a value of 1.  如果33号引脚的读取到的值为1
    M5.Lcd.setCursor(45, 25);M5.Lcd.print("Sensing");
    M5.Lcd.setCursor(45, 45);M5.Lcd.print("1");
  }
  else{
    M5.Lcd.setCursor(45, 25);M5.Lcd.print("Not Sensed");
    M5.Lcd.setCursor(45, 45);M5.Lcd.print("0");
  }
  delay(500);
}