/*
    Description: Detect the current OP 90/180 Unit Photoelectric switch status.
*/

#include <M5StickC.h>


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  //Wire.begin();
  M5.Lcd.setCursor(20, 5, 2);
  M5.Lcd.println("90/180");
    M5.Lcd.setCursor(20, 20, 2);
  M5.Lcd.println("OPTICAL");

  pinMode(33,INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Lcd.setCursor(8, 50, 2);
  M5.Lcd.printf("IR receive:");
   M5.Lcd.setCursor(15, 80 ,8);
  M5.Lcd.printf("%d",digitalRead(33));

}
