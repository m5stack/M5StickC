/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: ADC.  A/D转换器
* date：2021/8/18
*******************************************************************************
  Please connect to Port,Use ADC Unit to convert 0 ~ 12V analog voltage into 16-bit data and display it on the screen.
  请连接端口,利用ADC单元将0 ~ 12V模拟电压转换成16位数据显示在屏幕上。
*/

#include <M5StickC.h>
#include "M5_ADS1100.h"

ADS1100 ads;

void setup(void)
{
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.setRotation(3); //Rotating display.  旋转显示屏

  // The address can be changed making the option of connecting multiple devices
  // 地址可以改变，以连接多个设备
  ads.getAddr_ADS1100(ADS1100_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)

  //The ADC gain (PGA).  ADC增益(PGA)
  ads.setGain(GAIN_ONE);          // 1x gain(default)
  // ads.setGain(GAIN_TWO);       // 2x gain
  // ads.setGain(GAIN_FOUR);      // 4x gain
  // ads.setGain(GAIN_EIGHT);     // 8x gain

  //Device operating mode.  设备工作模式
  ads.setMode(MODE_CONTIN);       // Continuous conversion mode (default)
  // ads.setMode(MODE_SINGLE);    // Single-conversion mode

  //Data rate.  数据速率
  ads.setRate(RATE_8);            // 8SPS (default)
  // ads.setRate(RATE_16);        // 16SPS
  // ads.setRate(RATE_32);        // 32SPS
  // ads.setRate(RATE_128);       // 128SPS

  ads.setOSMode(OSMODE_SINGLE);   // Set to start a single-conversion.  设置开始一次转换

  ads.begin();  //Sets up the Hardware.  设置硬件
  M5.Lcd.print("ADC");
}

void loop(void)
{
  byte error;
  int8_t address;

  address = ads.ads_i2cAddress;
  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  if (error == 0) //If the device is connected.  如果连接上设备
  {
    int16_t result;
    result = ads.Measure_Differential();
    M5.Lcd.fillScreen(BLACK);
    char data[20] = { 0 };
    sprintf(data, "%d", result);
    M5.Lcd.drawCentreString(data, 0, 20, 4);
  }
  else
  {
    M5.Lcd.setCursor(0,40);
    M5.Lcd.drawString("No Found ADC sensor.",0, 20);
  }
  delay(1000);
}