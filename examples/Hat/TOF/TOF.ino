// please install vl53l0x lib first (https://github.com/pololu/vl53l0x-arduino)
// lib in Sketch->Includ Library->Library Manager, search for vl53l0x, Author: pololu

#include "M5StickC.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
TFT_eSprite img = TFT_eSprite(&M5.Lcd); 

void setup() {
  Serial.begin(115200);
  Wire.begin(0, 26, 100000);
  
  M5.begin();

  img.createSprite(160, 80);
  img.fillSprite(BLACK);
  img.setTextColor(WHITE);
  img.setTextSize(2);

  sensor.setTimeout(500);
  if (!sensor.init()) {
    img.setCursor(10, 10);
    img.print("Failed");
    img.pushSprite(0, 0);
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void loop() {
  uint16_t distance = sensor.readRangeContinuousMillimeters();
  Serial.print(distance);
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
  img.fillSprite(BLACK);
  img.setCursor(10, 10);
  img.print(distance);
  img.pushSprite(0, 0);
}