#include "CBACK_DRIVER.h"

void CBACK_DRIVER::Init()		
{
	Wire.begin(0, 26);
}

void CBACK_DRIVER::Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data);
  Wire.endTransmission();
}

void CBACK_DRIVER::Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data)
{
  Wire.beginTransmission(address);
  Wire.write(Register_address);
  Wire.write(data >> 8); //MSB
  Wire.write(data & 0xFF); //LSB
  
  Wire.endTransmission();
}


uint8_t CBACK_DRIVER::ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {

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



/*******************************************************************************/


uint8_t CBACK_DRIVER::SetServoAngle(uint8_t Servo_CH,uint8_t angle)
{
	uint8_t Register_address=Servo_CH-1;
	if(Register_address>3)
		return 0;
	Write1Byte(SERVO_ADDRESS,Register_address,angle);
	return 0;
}

uint8_t CBACK_DRIVER::SetServoPulse(uint8_t Servo_CH,uint16_t width)    //0x10        ->16
{
	uint8_t servo_ch =	Servo_CH-1;
	uint8_t Register_address=2*servo_ch+16;
	if(Register_address%2==1 || Register_address>32)
		return 1;
	Write2Byte(SERVO_ADDRESS,Register_address,width);
	return 0;
}


uint8_t CBACK_DRIVER::ReadServoAngle(uint8_t Servo_CH)
{
	uint8_t data=0;
	uint8_t Register_address=Servo_CH-1;
	ReadBytes(SERVO_ADDRESS,Register_address,1,&data);
	return data;
}

uint16_t CBACK_DRIVER::ReadServoPulse(uint8_t Servo_CH)
{
	uint8_t data[2];
	uint8_t servo_ch =	Servo_CH-1;
	uint8_t Register_address=2*servo_ch | 0x10;
	ReadBytes(SERVO_ADDRESS,Register_address,2,data);
  	return  (data[0]<<8)+data[1];
}

uint16_t CBACK_DRIVER::ReadADC()
{	
	uint8_t data[2];
	ReadBytes(SERVO_ADDRESS,0x20,2,data);
  	return  (data[0]<<8)+data[1];
}
uint8_t CBACK_DRIVER::SetOutput(uint8_t status)
{
	Write1Byte(SERVO_ADDRESS,0x30,status);
	return 0;
}
uint8_t CBACK_DRIVER::ReadInput(){
	uint8_t data;
	ReadBytes(SERVO_ADDRESS,0x31,1,&data);
  	return data;
}
