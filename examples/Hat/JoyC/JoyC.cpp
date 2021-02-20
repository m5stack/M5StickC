/*
 * @Author: Sorzn
 * @Date: 2019-11-22 14:48:24
 * @LastEditTime: 2019-11-22 15:44:44
 * @Description: M5Stack project
 * @FilePath: /M5StickC/examples/Hat/JoyC/JoyC.cpp
 */

#include "JoyC.h"
#include "M5StickC.h"

#define JOYC_ADDR 0x38

#define JOYC_COLOR_REG          0x20
#define JOYC_LEFT_X_REG         0x60
#define JOYC_LEFT_Y_REG         0x61
#define JOYC_RIGHT_X_REG        0x62
#define JOYC_RIGHT_Y_REG        0x63
#define JOYC_PRESS_REG          0x64
#define JOYC_LEFT_ANGLE_REG     0x70
#define JOYC_LEFT_DISTANCE_REG  0x74
#define JOYC_RIGHT_ANGLE_REG    0x72
#define JOYC_RIGHT_DISTANCE_REG 0x76


void JoyC::Init()		
{
	Wire.begin(0, 26);
}

void JoyC::Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data);
  Wire.endTransmission();
}

void JoyC::Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data >> 8); //MSB
  Wire.write(data & 0xFF); //LSB
  
  Wire.endTransmission();
}

void JoyC::WriteBytes(uint8_t address,uint8_t Register_address,uint8_t * data, size_t size)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  for (int i = 0; i<size; i++){
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}


uint8_t JoyC::ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {

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



void JoyC::SetLedColor(uint32_t color)
{
    uint8_t color_buff[3];
    color_buff[0] = (color & 0xff0000) >> 16;
    color_buff[1] = (color & 0xff00) >> 8;
    color_buff[2] = color & 0xff;
    WriteBytes(JOYC_ADDR, JOYC_COLOR_REG, color_buff, 3);
}

uint8_t JoyC::GetX(uint8_t pos)
{
    uint8_t value;
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_X_REG : JOYC_RIGHT_X_REG;
    ReadBytes(JOYC_ADDR,read_reg,1,&value);
    // M5.I2C.readByte(JOYC_ADDR, read_reg, &value);
    return value;
}

uint8_t JoyC::GetY(uint8_t pos)
{
    uint8_t value;
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_Y_REG : JOYC_RIGHT_Y_REG;
    ReadBytes(JOYC_ADDR,read_reg,1,&value);
    // M5.I2C.readByte(JOYC_ADDR, read_reg, &value);
    return value;
}

uint16_t JoyC::GetAngle(uint8_t pos)
{
    uint8_t i2c_read_buff[2];
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_ANGLE_REG : JOYC_RIGHT_ANGLE_REG;
    ReadBytes(JOYC_ADDR,read_reg,2,i2c_read_buff);
    // M5.I2C.readBytes(JOYC_ADDR, read_reg, 2, i2c_read_buff);
    return (i2c_read_buff[0] << 8) | i2c_read_buff[1];
}

uint16_t JoyC::GetDistance(uint8_t pos)
{
    uint8_t i2c_read_buff[2];
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_DISTANCE_REG : JOYC_RIGHT_DISTANCE_REG;
    ReadBytes(JOYC_ADDR,read_reg,2,i2c_read_buff);
    // M5.I2C.readBytes(JOYC_ADDR, read_reg, 2, i2c_read_buff);
    return (i2c_read_buff[0] << 8) | i2c_read_buff[1];    
}

uint8_t JoyC::GetPress(uint8_t pos)
{
    uint8_t press_value = 0;
    ReadBytes(JOYC_ADDR,JOYC_PRESS_REG,1,&press_value);
    // M5.I2C.readByte(JOYC_ADDR, JOYC_PRESS_REG, &press_value);
    return (press_value & ((pos == 0) ? 0x10 : 0x01)) != 0;
}