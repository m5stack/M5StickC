#include <M5StickC.h>
#include "puppy.h"


void puppy_start()
{
	angle_all_set(90,90,90,90);		//servo 1	2	3	4	=>	set	90°
	delay(100);
}

void puppy_go()
{
	angle_all_set(55,60,130,120);		//servo 1	2	3	4
	delay(100);
	angle_all_set(55,160,130,120);		//servo 1	2	3	4
	delay(100);
	angle_all_set(120,130,150,60);		//servo 1	2	3	4
	delay(100);
	angle_all_set(120,130,60,60);		//servo 1	2	3	4
	delay(100);
	angle_all_set(55,130,60,60);		//servo 1	2	3	4
	delay(100);
	angle_all_set(55,60,130,30);
	delay(100);
}

void puppy_back()
{
	angle_all_set(120,130,60,55);
	delay(100);
	angle_all_set(120,130,160,55);
	delay(100);
	angle_all_set(60,150,130,120);
	delay(100);
	angle_all_set(60,60,130,120);
	delay(100);
	angle_all_set(60,60,130,55);
	delay(100);
	angle_all_set(30,130,60,55);
	delay(100);
}


// the setup routine runs once when M5StickC starts up
void setup(){ 
  
  // Initialize the M5StickC object
  M5.begin();

  // LCD display
  M5.Lcd.println(" ");
  M5.Lcd.println("  IIC_PUPPY");
  IIC_Init();     //sda  0     scl  26 
  
  
  /*
        1              2
        3              4 
  */
  angle_set(1,90);		//servo 1	=>	set	90°	
  angle_set(2,90);
  angle_set(3,90);
  angle_set(4,90);
  delay(1000);
}

// the loop routine runs over and over again forever
      /*
        1              2
        3              4 
      */
uint8_t cnt=10;      
void loop() {	
	while(cnt!=0)
	{
		cnt--;
		puppy_go();
	}
	cnt=10;
	puppy_start();
	while(cnt!=0)
	{
		cnt--;
		puppy_back();
	}
	cnt=10;
	puppy_start();
}
