/*
 * @Author: Sorzn
 * @Date: 2019-11-22 13:38:26
 * @LastEditTime: 2019-11-22 14:28:56
 * @Description: M5Stack Hat BUGC Lib
 * @FilePath: /M5StickC/examples/Hat/BUGC/bugC.cpp
 */

#include "M5StickC.h"
#include "bugC.h"


void BUGC::Init() {
	Wire.begin(0, 26);
}


void BUGC::Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data);
  Wire.endTransmission();
}

void BUGC::Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data >> 8); //MSB
  Wire.write(data & 0xFF); //LSB
  
  Wire.endTransmission();
}

void BUGC::WriteBytes(uint8_t address,uint8_t Register_address,uint8_t * data, size_t size)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  for (int i = 0; i<size; i++){
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}


uint8_t BUGC::ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {

  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  uint8_t i = 0;
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(address, (uint8_t)count)) {
    while (Wire.available()) {
		dest[i++]=Wire.read();
    }
    return true;
  }
  return false;
}



//*******************************************************//




// if speed > 0; clockwise rotation
void BUGC::BugCSetSpeed(uint8_t pos, int8_t speed)
{
    speed = (speed > 100) ? 100 : speed;
    speed = (speed < -100) ? -100 : speed;
    Write1Byte(BUGC_ADDR, pos, speed);
}

void BUGC::BugCSetAllSpeed(int8_t speed_0, int8_t speed_1, int8_t speed_2, int8_t speed_3)
{
    int8_t speed_out[4] = {speed_0, speed_1, speed_2, speed_3};
    for(uint8_t i = 0; i < 4; i++)
    {
        speed_out[i] = (speed_out[i] > 100) ? 100 : speed_out[i];
        speed_out[i] = (speed_out[i] < -100) ? -100 : speed_out[i];
    }
    WriteBytes(BUGC_ADDR, 0x00, (uint8_t *)speed_out, 4);
}

void BUGC::BugCSetColor(uint32_t color_left, uint32_t color_right)
{
    uint8_t color_out[4];
    color_out[0] = 0;
    color_out[1] = (color_left & 0xff0000) >> 16;
    color_out[2] = (color_left & 0x00ff00) >> 8;
    color_out[3] = (color_left & 0x0000ff);
    WriteBytes(BUGC_ADDR, 0x10,  color_out, 4);

    color_out[0] = 1;
    color_out[1] = (color_right & 0xff0000) >> 16;
    color_out[2] = (color_right & 0x00ff00) >> 8;
    color_out[3] = (color_right & 0x0000ff);
    WriteBytes(BUGC_ADDR, 0x10,  color_out, 4);
}