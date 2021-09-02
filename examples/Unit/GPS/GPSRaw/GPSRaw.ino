/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/gps
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/gps
*
* describe：GPS.
* date：2021/8/26
*******************************************************************************
/*
    Description: The command data of the USB UART  is forwarded to the GPS Unit for debugging and functional verification.  将USB UART的命令数据转发给GPS Unit进行调试和功能验证。
*/
#include <M5StickC.h>

HardwareSerial GPSRaw(2);

void setup() {
  
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("GPS Unit");
  GPSRaw.begin(9600, SERIAL_8N1, 33, 32);

  Serial.println("hello");
} 

void loop() {
  if(Serial.available()) {
    int ch = Serial.read();
    GPSRaw.write(ch);  
  }

  if(GPSRaw.available()) {
    int ch = GPSRaw.read();
    Serial.write(ch);
  }
}
