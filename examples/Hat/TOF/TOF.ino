/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc
*
* Product:  ToF.  激光测距
* Date: 2022/6/3
*******************************************************************************
  Please connect to hat,Use ToF Hat to detect distance and display distance
  data on the screen in real time.
  请连接HAT,使用ToF HAT检测距离，并在屏幕上实时显示距离数据。

  Please install vl53l0x lib first(https://github.com/pololu/vl53l0x-arduino)
  lib in Sketch->Includ Library->Library Manager, search for vl53l0x
  Author:pololu
*/

#include <VL53L0X.h>
#include <Wire.h>

#include "M5StickC.h"

VL53L0X sensor;
TFT_eSprite img = TFT_eSprite(&M5.Lcd);

void setup() {
    Wire.begin(0, 26, 100000UL);  //(0,26, 100000UL) for I2C of HAT connection

    M5.begin();

    img.createSprite(160, 80);
    img.fillSprite(BLACK);
    img.setTextColor(WHITE);
    img.setTextSize(2);

    sensor.setTimeout(500);
    if (!sensor.init()) {
        img.setCursor(10, 10);
        img.print("Failed");
        img.pushSprite(0, 0);
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {
        }
    }
    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous();
}

void loop() {
    uint16_t distance = sensor.readRangeContinuousMillimeters();
    Serial.print(distance);
    if (sensor.timeoutOccurred()) {
        Serial.print(" TIMEOUT");
    }
    Serial.println();
    img.fillSprite(BLACK);
    img.setCursor(10, 10);
    img.print(distance);
    img.pushSprite(0, 0);
}
