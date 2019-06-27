#include <M5StickC.h>

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(30, 0); 
  M5.Lcd.println("PIR example");
  Serial.println("PIR example: ");
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.setTextColor(WHITE);
  pinMode(33, INPUT);
}

void loop() {
  M5.Lcd.setCursor(0,25); M5.Lcd.print("Status: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print("Value: ");
  
  M5.Lcd.fillRect(75,25,200,25,BLACK);
  M5.Lcd.fillRect(75,45,200,25,BLACK);
  
  if(digitalRead(33)==1){
    M5.Lcd.setCursor(75, 25);M5.Lcd.print("Sensing");
    M5.Lcd.setCursor(75, 45);M5.Lcd.print("1");
    Serial.println("PIR Status: Sensing");
    Serial.println(" value: 1");
  }
  else{
    M5.Lcd.setCursor(75, 25);M5.Lcd.print("Not Sensed");
    M5.Lcd.setCursor(75, 45);M5.Lcd.print("0");
    Serial.println("PIR Status: Not Sensed");
    Serial.println(" value: 0");
  }
  delay(500);
  M5.update();
}
