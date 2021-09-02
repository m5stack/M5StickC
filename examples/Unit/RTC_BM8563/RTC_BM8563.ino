/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: RTC.  实时时钟
* date：2021/8/18
*******************************************************************************
  Please connect to Port,The time is displayed on the screen.
  请连接端口,屏幕上显示时间。
*/

#include "M5StickC.h"
#include "M5_BM8563.h"

BM8563 RTC;

rtc_time_type RTCtime;
rtc_date_type RTCdate;

char str_buffer[64];

void setup()
{
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.setRotation(3); //Rotating display.  旋转显示屏
  M5.Lcd.print("             RTC");
  RTC.begin();  //Example Initialize the RTC clock.  初始化RTC时钟
  RTCtime.Hours = 14; //Set the RTC clock time.  设置RTC时钟时间
  RTCtime.Minutes = 40;
  RTCtime.Seconds = 5;

  RTCdate.WeekDay = 4;  //Set the RTC clock date.  设置RTC时钟日期
  RTCdate.Month = 7;
  RTCdate.Date = 15;
  RTCdate.Year = 2021;

  RTC.setTime(&RTCtime);  //Example Synchronize the set time to the RTC.  将设置的时间同步至RTC
  RTC.setDate(&RTCdate);  //Synchronize the set date to the RTC.  将设置的日期同步至RTC
}

void loop()
{
  RTC.getTime(&RTCtime);  //To get the time.  获取时间
  RTC.getDate(&RTCdate);  //Get the date.  获取日期
  M5.Lcd.setCursor(0,20);
  M5.Lcd.printf("RTC Time Now is \n%02d:%02d:%02d\n",RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
  M5.Lcd.printf("RTC Date Now is \n%02d:%02d:%02d WeekDay:%02d\n",RTCdate.Year, RTCdate.Month, RTCdate.Date, RTCdate.WeekDay);
  delay(1000);
  M5.Lcd.fillRect(0,20,320,140,BLACK);
}
