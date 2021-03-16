
#include "RoverC.h"



void RoverC_Init(void)    //sda  0     scl  26
{
    Wire.begin(0,26,100);
}

void Send_iic(uint8_t Register, uint8_t Speed)
{
  Wire.beginTransmission(ROVER_ADDRESS);
  Wire.write(Register);
  Wire.write(Speed);
  Wire.endTransmission();
}

void Move_forward(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, Speed );
}

void Move_back(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_turnleft(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_turnright(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, Speed );
}

void Move_left(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_right(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, Speed );
}
void Move_stop(int8_t Speed)
{
  Send_iic(0x00, 0 );
  Send_iic(0x01, 0 );
  Send_iic(0x02, 0 );
  Send_iic(0x03, 0 );
}
