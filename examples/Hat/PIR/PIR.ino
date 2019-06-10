#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("PIR TEST");

  pinMode(36,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  M5.Lcd.setCursor(60, 20, 4);
  M5.Lcd.println(digitalRead(36));
  

}
