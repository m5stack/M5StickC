#include <M5StickC.h>

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0, 2);
  M5.Lcd.println("RTC TEST");
  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = 18;
  TimeStruct.Minutes = 56;
  TimeStruct.Seconds = 10;
  M5.Rtc.SetTime(&TimeStruct);
  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = 3;
  DateStruct.Month = 3;
  DateStruct.Date = 22;
  DateStruct.Year = 2019;
  M5.Rtc.SetData(&DateStruct);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.printf("Data: %04d-%02d-%02d\n",RTC_DateStruct.Year, RTC_DateStruct.Month,RTC_DateStruct.Date);
  M5.Lcd.printf("Week: %d\n",RTC_DateStruct.WeekDay);
  M5.Lcd.printf("Time: %02d : %02d : %02d\n",RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  delay(500);
}
