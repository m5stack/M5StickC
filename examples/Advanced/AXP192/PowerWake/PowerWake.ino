#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
}


long loopTime, startTime = 0;
uint8_t sleep_count = 0;
void loop() {

  loopTime = millis();
  if(startTime < (loopTime - 5000)){
    
    if(M5.Axp.GetWarningLeve()){
      sleep_count++;
      M5.Lcd.fillScreen(WHITE);
      M5.Lcd.setCursor(0, 20, 2);
      M5.Lcd.setTextColor(RED, WHITE);
      M5.Lcd.printf("Warning: low battery");
      //delay(3000);
      if(sleep_count >= 2){
        sleep_count = 0;
        M5.Axp.SetSleep();
      }
    }else{
      M5.Lcd.fillScreen(WHITE);
    }
    startTime = loopTime;
  }
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.printf("vbat:%.3fV\r\n",M5.Axp.GetVbatData() * 1.1 / 1000);
  M5.Lcd.printf("aps:%.3fV\r\n",M5.Axp.GetVapsData() * 1.4 / 1000);
  M5.Lcd.printf("level:%d\r\n",M5.Axp.GetWarningLeve());
}
