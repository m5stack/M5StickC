#include <M5StickC.h>
#include <Wire.h>
#include "M5_IP5209.h"
TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

M5_IP5209 powerc;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  canvas.createSprite(160, 80);
  canvas.setTextColor(ORANGE);
  powerc.begin();
}

char info[50];

void loop() {
  canvas.fillSprite(BLACK);
  canvas.setCursor(0,10);
  float v = powerc.getBatVoltage();
  float i = powerc.getBatCurrent();
  sprintf(info, "%1.2fV   %1.0fmAh",v,  i);
  canvas.println(info);
  canvas.pushSprite(10, 0);
}
