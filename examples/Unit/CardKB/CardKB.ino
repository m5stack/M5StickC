/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：CardKB.  键盘
* date：2021/8/11
*******************************************************************************
  Please connect to Port ,Read the characters entered by CardKB Unit and display them on the screen.
  请连接端口,读取CardKB Unit输入的字符并显示在屏幕上。
*/

#include <M5StickC.h>
#define CARDKB_ADDR 0x5F  //Define the I2C address of CardKB.  定义CardKB的I2C地址

void setup(){
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.printf("IIC Address: 0x5F\n");
  M5.Lcd.printf(">>");
  Wire.begin(32, 33);  //Initialize pin 32,33.  初始化32,33引脚
}

void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1); //Request 1 byte from the slave device.  向从设备请求1字节
  while(Wire.available()) //If received data is detected.  如果检测到收到数据
  {
    char c = Wire.read(); // Store the received data.  将接收到的数据存储
    if (c != 0)
    {
      M5.Lcd.printf("%c", c);
    }
  }
}
