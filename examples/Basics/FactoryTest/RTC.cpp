#include "rtc.h"



Rtc::Rtc(){
  Wire.begin();  
}

void Rtc::GetBm8563Time(void){
  Wire.beginTransmission(0x51);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x51,7); 
  while(Wire.available()){
    
      trdata[0] = Wire.read();
      trdata[1] = Wire.read();
      trdata[2] = Wire.read();
      trdata[3] = Wire.read();
      trdata[4] = Wire.read();
      trdata[5] = Wire.read();
      trdata[6] = Wire.read();
     
  }

  DataMask();
  Bcd2asc();
  Str2Time();
}


void Rtc::Str2Time(void){
  
  Second = (asc[0] - 0x30) * 10 + asc[1] - 0x30;
  Minute = (asc[2] - 0x30) * 10 + asc[3] - 0x30;
  Hour = (asc[4] - 0x30) * 10 + asc[5] - 0x30;



  DateString[0] = asc[4];
  DateString[1] = asc[5];
  DateString[2] = ':';
  DateString[3] = asc[2];
  DateString[4] = asc[3];
  DateString[5] = ':';
  DateString[6] = asc[0];
  DateString[7] = asc[1];
  /*
  uint8_t Hour;
  uint8_t Week;
  uint8_t Day;
  uint8_t Month;
  uint8_t  Year;
  */
}


void Rtc::DataMask(){
  
  trdata[0] = trdata[0]&0x7f;    //秒
  trdata[1] = trdata[1]&0x7f;    //分
  trdata[2] = trdata[2]&0x3f;    //时
  
  trdata[3] = trdata[3]&0x3f;    //日
  trdata[4] = trdata[4]&0x07;    //星期
  trdata[5] = trdata[5]&0x1f;    //月
  
  trdata[6] = trdata[6]&0xff;    //年
  
}
/********************************************************************
函 数 名： void Bcd2asc(void)
功 能： bcd 码转换成 asc 码，供液晶显示用
说 明：
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void Rtc::Bcd2asc(void)
{
  uint8_t i,j;
  for (j=0,i=0; i<7; i++){
    asc[j++] =(trdata[i]&0xf0)>>4|0x30 ;/*格式为: 秒 分 时 日 月 星期 年 */
    asc[j++] =trdata[i]&0x0f|0x30;
  }
}
