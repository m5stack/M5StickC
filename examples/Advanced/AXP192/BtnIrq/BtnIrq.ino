#include "M5StickC.h"

static void clear_mpu6880IRQ() {
  M5.Mpu6886.DisableAllIRQ();
  M5.Mpu6886.SetINTPinActiveLogic(0);
  M5.Mpu6886.ClearAllIRQ();
}

static void clear_200qIRQ() {
  M5.I2C.writeByte(0x6C, 0x13, 0xe0);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);

  Wire.begin(21, 22, 400000);
  clear_mpu6880IRQ();
  clear_200qIRQ();
  M5.Axp.DisableAllIRQ();
  M5.Axp.ClearAllIRQ();
  M5.Axp.EnablePressIRQ(true, true);
  pinMode(35, INPUT);
}

void loop() {
  delay(1000);
  M5.Lcd.setCursor(0, 0);
  uint8_t PinStatus = digitalRead(35);
  bool short_press, long_press;
  M5.Axp.GetPressIRQ(&short_press, &long_press);
  
  if (short_press || long_press) {
    M5.Axp.ClearPressIRQ(short_press, long_press);
  }

  M5.Lcd.printf("pin 35 status: %d\r\n", PinStatus);
  M5.Lcd.printf("axp192 btn\r\n  short press: %d\r\n  long press: %d\r\n", short_press, long_press);

  Serial.printf("pin 35 status: %d\r\n", PinStatus);
  Serial.printf("axp192 btn\r\n  short press: %d\r\n  long press: %d\r\n", short_press, long_press);
}