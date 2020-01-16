#include "IIC_servo.h"

void IIC_Servo_Init()	//sda  0     scl  26		
{
    Wire.begin(0, 26, 100);
}

void IIC_Write_1bytes(uint8_t address,uint8_t Register_address,uint8_t data)
{
    Wire.beginTransmission(address);
    Wire.write(Register_address);
    Wire.write(data);
    Wire.endTransmission();
}

void IIC_Write_2bytes(uint8_t address,uint8_t Register_address,uint16_t data)
{
    Wire.beginTransmission(address);
    Wire.write(Register_address);
    Wire.write(data >> 8); //MSB
    Wire.write(data & 0xFF); //LSB
    Wire.endTransmission();
}

uint8_t readBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) 
{
    Wire.beginTransmission(address);   // Initialize the Tx buffer
    Wire.write(subAddress);            // Put slave register address in Tx buffer
    uint8_t i = 0;
    if (Wire.endTransmission(false) == 0 && Wire.requestFrom(address, (uint8_t)count)) 
    {
        while (Wire.available()) 
        {
            dest[i++]=Wire.read();
        }
        return true;
    }
    return false;
}

/*******************************************************************************/
uint8_t Servo_angle_set(uint8_t Servo_CH,uint8_t angle)
{
    uint8_t Register_address=Servo_CH-1;
    if(Register_address>7) 
    {
        return 1;
    }
    IIC_Write_1bytes(SERVO_ADDRESS,Register_address,angle);
    return 0;
}

uint8_t Servo_pulse_set(uint8_t Servo_CH,uint16_t width)
{
    uint8_t servo_ch=Servo_CH-1;
    uint8_t Register_address=2*servo_ch+16;
    if(Register_address%2==1 || Register_address>32)
    {
        return 1;
    }
    IIC_Write_2bytes(SERVO_ADDRESS,Register_address,width);
    return 0;
}

void RGB_set(uint8_t R,uint8_t G,uint8_t B)
{
    Wire.beginTransmission(SERVO_ADDRESS);
    Wire.write(32);
    Wire.write(G);
    Wire.write(R);
    Wire.write(B);
    Wire.endTransmission();
}

uint8_t Servo_angle_read(uint8_t Servo_CH)
{
    uint8_t data=0;
    uint8_t Register_address=Servo_CH-1;
    readBytes(SERVO_ADDRESS,Register_address,1,&data);
    return data;
}

uint16_t Servo_pulse_read(uint8_t Servo_CH)
{
    uint8_t data[2]={0,0};
    uint8_t servo_ch =	Servo_CH-1;
    uint8_t Register_address=2*servo_ch+16;
    readBytes(SERVO_ADDRESS,Register_address,2,data);
    return  (data[0]<<8)+data[1];
}

uint32_t RGB_read()
{
    uint8_t data[3]={0,0,0};
    readBytes(SERVO_ADDRESS,32,3,data);
    return (data[0]<<16)+(data[1]<<8)+data[2];
}
