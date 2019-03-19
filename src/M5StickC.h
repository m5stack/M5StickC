// Copyright (c) M5Stack. All rights reserved.

#ifndef _M5STICKC_H_
#define _M5STICKC_H_

#define ESP32
#if defined(ESP32)

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "M5Display.h"
#include "AXP192.h"

#include "utility/Config.h"
#include "RTC.h"
#include "IMU.h"

class M5StickC {

 public:
    M5StickC();
	void begin(bool LCDEnable=true, bool PowerEnable=true, bool SerialEnable=true);

    //!LCD
    M5Display Lcd = M5Display();

    //!Power
    AXP192 Axp = AXP192();

    //!IR
    //ESP32_RMT rem;

    //!RTC
     RTC  Rtc;

     IMU Imu;

 private:
    bool isInited;
    //uint8_t _wakeupPin;
};


extern M5StickC M5;
#define m5 M5
#define lcd Lcd
#define axp Axp
#define imu Imu
#define IMU Imu

#else
#error “This library only supports boards with ESP32 processor.”	
#endif

#endif