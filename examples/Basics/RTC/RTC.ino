/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: RTC.
* date：2021/9/3
*******************************************************************************
*/
#include <M5StickC.h>

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

/* After M5StickC is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 M5StickC 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(40, 0, 2);
  M5.Lcd.println("RTC TEST");
  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = 18; //Set the time.  设置时间
  TimeStruct.Minutes = 56;
  TimeStruct.Seconds = 10;
  M5.Rtc.SetTime(&TimeStruct); //and writes the set time to the real time clock.  并将设置的时间写入实时时钟
  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = 3;  //Set the date.  设置日期
  DateStruct.Month = 3;
  DateStruct.Date = 22;
  DateStruct.Year = 2019;
  M5.Rtc.SetData(&DateStruct);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.Rtc.GetTime(&RTC_TimeStruct); //Gets the time in the real-time clock.  获取实时时钟内的时间
  M5.Rtc.GetData(&RTC_DateStruct);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.printf("Data: %04d-%02d-%02d\n",RTC_DateStruct.Year, RTC_DateStruct.Month,RTC_DateStruct.Date);
  M5.Lcd.printf("Week: %d\n",RTC_DateStruct.WeekDay);
  M5.Lcd.printf("Time: %02d : %02d : %02d\n",RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  delay(500);
}
