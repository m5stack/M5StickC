#include "puppy.h"


void IIC_Init()	//sda  0     scl  26		
{
  Wire.begin(0,26,100);
}

/*******************************************************************************/
      /*
          
        1					2
        3					4	
      */
void angle_all_set(uint8_t angle1,uint8_t angle2,uint8_t angle3,uint8_t angle4)																					
{
  Wire.beginTransmission(SERVO_ADDRESS);
  Wire.write(0x00);
  Wire.write(angle1);
  Wire.write(angle2);
  Wire.write(angle3);
  Wire.write(angle4);
  Wire.endTransmission();
}

uint8_t angle_set(uint8_t pos,uint8_t anglex)																						
{
  if(pos<1 || pos>5)
  {
    return 0;
  }
  pos-=1;
  Wire.beginTransmission(SERVO_ADDRESS);
  Wire.write(pos);
  Wire.write(anglex);
  Wire.endTransmission();
  return 1;
}

