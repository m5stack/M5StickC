/*
    Description: Use RS485 Unit for serial communication, BtnA to send "Hello", and display the received content on the screen.
*/
#include <M5StickC.h>
#define RX_PIN      33
#define TX_PIN      32

int i = 0, s = 0;

void header(const char *string, uint16_t color) {
  M5.Lcd.fillScreen(color);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.fillRect(0, 0, 80, 30, TFT_BLACK);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(string,40, 10, 2);
}

void setup() {
  M5.begin();
  header("RS485 Unit", TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(10);
  Serial2.read();
}


void loop() {
  if (M5.BtnA.wasPressed())
  {
    Serial2.write("Hello\n");
  }
  if (Serial2.available()) {
    M5.Lcd.print(char(Serial2.read()));
  }
  delay(100);
  M5.update();
}
