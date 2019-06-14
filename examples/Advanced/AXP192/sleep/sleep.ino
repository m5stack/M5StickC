#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
  pinMode(M5_BUTTON_HOME,INPUT_PULLUP);

  M5.Lcd.setCursor(60, 30);
  M5.Lcd.print("SLEEP");
}

void loop() {
  if(digitalRead(M5_BUTTON_HOME) == LOW){
    while(digitalRead(M5_BUTTON_HOME) == LOW);
    //Will go to sleep 5s after cpu work
    M5.Axp.LightSleep(SLEEP_SEC(5));
    delay(6);
    //Will enter sleep for 5s and wake up automatically
    M5.Axp.DeepSleep(SLEEP_SEC(5));
  }
}
