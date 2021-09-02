/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：QRcode.  创建二维码
* date：2021/7/27
*******************************************************************************
*/

#include <M5StickC.h>

void setup() {
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.qrcode("http://www.m5stack.com",0,0,80);  //Create a QR code with a width of 80 QR code  at (0, 0).  在(0,0)处创建一个宽为80二维码
  //Please select the appropriate QR code version according to the number of characters.  请根据字符数量选择合适的二维码版本
}

void loop() {
}