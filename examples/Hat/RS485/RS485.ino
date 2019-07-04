
#include <M5StickC.h>

/* This demo is for RS485 Hat uart write and read ,
AutoSend Hello M5! AutoReceive and diaplay on screen */

void setup() {

  M5.begin(true,true,true);

  Serial.begin(115200);

  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 26, 0);
  // pinMode(5, OUTPUT);
  // digitalWrite(5, 1);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.setCursor(0, 0);

  M5.Lcd.print("RS485 Demo");
  Serial.println("RS485");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.print("AutoSend Hello M5!");

  M5.Lcd.setCursor(10, 30);
  M5.Lcd.print("AutoPrint Receive");
   M5.Lcd.setCursor(10, 50);

}

void loop() {

  Serial2.print("Hello M5!");
  Serial.println("RS485");
  delay(500);

 if(Serial2.available()) {
    char ch = Serial2.read();
    M5.Lcd.setTextSize(2);
   
    M5.Lcd.print(ch);
 }
}
