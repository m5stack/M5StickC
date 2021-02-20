#include <M5StickC.h>
#include <Wire.h>
#include "PowerC.h"

POWERC PowerC;

void setup() {
  M5.begin();
  PowerC.Init();
}

void loop() {
  float v = PowerC.ReadVoltage();
  float i = PowerC.ReadCurrent();
  v = v * 0.00026855 + 2.6;
  i = i * 0.745985;
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf("%1.2f V",v);
  Serial.printf("%1.2f",v);
  Serial.println("");
  M5.Lcd.setCursor(0,40);
  M5.Lcd.printf("%1.0f mAh",i);
  Serial.printf("%1.0f",i);
  Serial.println("");
}
