#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
  //M5.Lcd.printf(brightness)
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 7; i <= 16; i++){
    M5.Axp.ScreenBreath(i);
    
    M5.Lcd.setCursor(0, 30, 2);
    M5.Lcd.println("brightness");
    M5.Lcd.printf("%d%%",i*100/16);
    delay(300);
  }

  
}
