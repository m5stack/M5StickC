#include <M5StickC.h>

void setup() {
  M5.begin();
  //M5.Lcd.clear(BLACK);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextColor(YELLOW, BLACK);
  //disable the speak noise
  pinMode(32, OUTPUT);
}

void loop(void) {
  digitalWrite(32, HIGH);
  delay(500);
  digitalWrite(32, LOW);
  delay(500);
}
