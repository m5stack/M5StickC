/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：BPS_BMP280.  压力传感器
* date：2021/8/10
******************************************** ***********************************
  Please connect to Port,Read atmospheric pressure and temperature and display them on the display screen
  请连接端口,读取大气压强和温度并在显示屏上显示
*/
#include <M5StickC.h>
#include <Wire.h> //The BPS uses I2C comunication.
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme;

void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  Wire.begin(); //Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
  while (!bme.begin(0x76)){ //Init this sensor,True if the init was successful, otherwise false.   初始化传感器,如果初始化成功返回1
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  M5.Lcd.fillScreen(BLACK); //Clear the screen.  清屏
}

float pressure,Temp;  //Store the vuale of pressure and Temperature.  存储压力和温度()

void loop() {
  pressure = bme.readPressure();
  Temp = bme.readTemperature();
  M5.Lcd.setCursor(0, 0); //将光标设置在(0 ,0).  Set the cursor to (0,0)
  M5.Lcd.setTextSize(2);  //设置字体大小为2.  Set the font size to 2
  M5.Lcd.printf("Pre:%2.0fPa\nTem:%2.0f^C", pressure,Temp);
  delay(100);
}
