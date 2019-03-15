#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("rtc test");
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Rtc.GetBm8563Time();
  M5.Lcd.setCursor(0, 30, 2);
  M5.Lcd.printf("%02d : %02d : %02d\n",M5.Rtc.Hour, M5.Rtc.Minute, M5.Rtc.Second);
  delay(1000);
}
