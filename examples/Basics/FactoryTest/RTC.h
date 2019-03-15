#ifndef __RTC_H__
#define __RTC_H__

#include <Wire.h>

class Rtc { 
public:
  Rtc();
  void GetBm8563Time(void);
  
public:
  uint8_t Second;
  uint8_t Minute;
  uint8_t Hour;
  uint8_t Week;
  uint8_t Day;
  uint8_t Month;
  uint8_t  Year;
  uint8_t DateString[9];
  uint8_t TimeString[9];

  uint8_t asc[14];

  
private:
  void Bcd2asc(void);
  void DataMask();
  void Str2Time(void);
   
private:

  /*定义数组用来存储读取的时间数据 */
  uint8_t trdata[7]; 
  /*定义数组用来存储转换的 asc 码时间数据*/
  //uint8_t asc[14]; 

};

#endif
