#include "M5StickC.h"

static void clear_mpu6880IRQ() {
  uint8_t temp;
  // set int active is LOW
  M5.I2C.writeByte(0x68, 0x37, 0xc8);
  // disable irq
  M5.I2C.writeByte(0x68, 0x38, 0x00);
  // clear irq status
  M5.I2C.readByte(0x68, 0x39, &temp);
}

static void clear_200qIRQ() {
  M5.I2C.writeByte(0x6C, 0x13, 0xe0);
}

static void axp192_disableAllIRQ() {
  M5.I2C.writeByte(0x34, 0x40, 0x00);
  M5.I2C.writeByte(0x34, 0x41, 0x00);
  M5.I2C.writeByte(0x34, 0x42, 0x00);
  M5.I2C.writeByte(0x34, 0x43, 0x00);
}

static void axp192_clearAllIRQStatus() {
  M5.I2C.writeByte(0x34, 0x44, 0xff);
  M5.I2C.writeByte(0x34, 0x45, 0xff);
  M5.I2C.writeByte(0x34, 0x46, 0xff);
  M5.I2C.writeByte(0x34, 0x47, 0xff);
  M5.I2C.writeByte(0x34, 0x4d, 0xff);
}

static void axp192_enableBtnIRQ() {
  M5.I2C.writeByte(0x34, 0x42, 0x03);
}

static uint8_t axp192_getBtnIRQStatus() {
  uint8_t status;
  M5.I2C.readByte(0x34, 0x46, &status);
  // clear btn irq
  M5.I2C.writeByte(0x34, 0x46, 0xff);
  return status;
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);

  Wire.begin(21, 22, 400000);
  clear_mpu6880IRQ();
  clear_200qIRQ();
  pinMode(35, INPUT);
  axp192_disableAllIRQ();
  axp192_clearAllIRQStatus();
  axp192_enableBtnIRQ();
}

void loop() {
  delay(1000);
  M5.Lcd.setCursor(0, 0);
  uint8_t PinStatus = digitalRead(35);
  uint8_t IRQStatus = axp192_getBtnIRQStatus();
  M5.Lcd.printf("pin 35 status: %d\r\n", PinStatus);
  M5.Lcd.printf("axp192 btn status: %d\r\n", IRQStatus);
  Serial.printf("pin 35 status: %d\r\n", PinStatus);
  Serial.printf("axp192 btn status: %d\r\n", IRQStatus);
}