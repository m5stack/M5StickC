
#include "PowerC.h"
#include "M5StickC.h"

#define POWERC_ADDR 0x38

#define POWERC_VOLTAGE_REG 0xA2
#define POWERC_CURRENT_REG 0XA4

void POWERC::Init() {
	Wire.begin(0, 26);
  M5.Lcd.setRotation(1);
  while(Wire.endTransmission() !=0){
     M5.Lcd.setCursor(20,0);
     M5.Lcd.print("Not found PowerC");
     Wire.beginTransmission(0x75);
  }
}

void POWERC::Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data);
  Wire.endTransmission();
}

void POWERC::Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data >> 8); //MSB
  Wire.write(data & 0xFF); //LSB
  
  Wire.endTransmission();
}

void POWERC::WriteBytes(uint8_t address,uint8_t Register_address,uint8_t * data, size_t size)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  for (int i = 0; i<size; i++){
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}


uint8_t POWERC::ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {

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

float POWERC::ReadVoltage() {
    uint8_t dataI[2] = {0, 0};
    ReadBytes(POWERC_ADDR, POWERC_VOLTAGE_REG, 2, dataI);
    return ((dataI[1]<<8)&0xff00) | dataI[0];
}

float POWERC::ReadCurrent() {
    uint8_t dataV[2] = {0, 0};
    ReadBytes(POWERC_ADDR, POWERC_CURRENT_REG, 2, dataV);
    return ((dataV[1]<<8)&0xff00) | dataV[0];
}
