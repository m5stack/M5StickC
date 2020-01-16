#include "yunBoard.h"
#include "Wire.h"
#define YUN_ADDR        0x38
#define LED_REG_ADDR    0x01
#define LIGHT_REG_ADDR  0x00

bool I2C_writeBytes(uint8_t address, uint8_t subAddress, uint8_t *data, uint8_t length) {
  Wire.beginTransmission(address);                // Initialize the Tx buffer
  Wire.write(subAddress);                         // Put slave register address in Tx buffer
  for(int i = 0; i < length; i++) {
    Wire.write(*(data+i));                        // Put data in Tx buffer
  }

  return (Wire.endTransmission() == 0);             // Send the Tx buffer
}

bool I2C_readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest) {
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(true);

  Wire.requestFrom(address, (uint8_t)count);
  uint8_t i = 0;
  while (Wire.available()) {
    dest[i++] = Wire.read();// Put read results in the Rx buffer
  }
  return false;
}

// RGB888
void led_set(uint8_t num, uint32_t color_in) {
  uint8_t data_out[4] = { 0x00 };
  data_out[0] = num;
  data_out[1] = color_in >> 16;
  data_out[2] = color_in >>8;
  data_out[3] = (uint8_t)color_in;
  I2C_writeBytes(YUN_ADDR, LED_REG_ADDR, data_out, 4);
}

void led_set_all(uint32_t color_in) {
  led_set(14, color_in);
}

uint16_t light_get() {
  uint16_t light = 0x00;
  I2C_readBytes(YUN_ADDR, LIGHT_REG_ADDR, 2, (uint8_t *)&light);
  return light;
}