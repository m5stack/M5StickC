/*
  neo pixel test

   hardwware:  M5Stick
   Pressed ButtonA :send "hello world"

*/
#include <M5StickC.h>

String str = "";

void setup() {

  M5.begin();
  M5.Lcd.drawString("ISO485", 20, 0, 2);

  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 33, 32);

  M5.Lcd.setTextFont(1);
  M5.Lcd.setCursor(0, 20);
}

void loop() {
  if (M5.BtnA.wasPressed())
  {
    Serial2.write("Hello World\r\n");
  }

  if (Serial2.available()) {
    char ch = Serial2.read();
    str += ch;
    if (str.endsWith("\r\n")) {
      Serial.print(str);
      M5.Lcd.print(str);
      str = "";
    }
  }
  M5.update();
}
