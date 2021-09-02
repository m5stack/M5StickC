/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：EARTH.  土壤湿度
* date：2021/8/11
*******************************************************************************
  Please connect to Port,Read the analog quantity and digital quantity returned by the EARTH unit, and convert the analog quantity into 12-bit data and display it on the screen.
  请连接端口,读取EARTH Unit 返回的模拟量和数字量，并将模拟量转换为12位数据显示在屏幕上。
*/

#include <M5StickC.h>

void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.printf("UNIT_EARTH EXAMPLE\n");
  pinMode(22, INPUT); //Set pin 22 to input mode.  将引脚22设置为输入模式
}

void loop() {
  M5.Lcd.setCursor(0, 30);  //Set the cursor at (0,30).  将光标设置在(0,30)
  M5.Lcd.printf("AnalogRead:%d\n", analogRead(33));
  M5.Lcd.printf("DigitalRead:%d\n", digitalRead(22));
  delay(1000);
}
