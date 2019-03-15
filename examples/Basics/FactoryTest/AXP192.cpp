#include "axp192.h"

AXP192::AXP192(){
  
}

void AXP192::Init(void){
  
    Wire.begin();
  
    Wire.beginTransmission(0x34);
    Wire.write(0x10);  
    Wire.write(0xff);  //OLED_VPP Enable
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0x28);  
    Wire.write(0xff); //Enable LDO2&LDO3, LED&TFT 3.3V
    Wire.endTransmission();
     
    Wire.beginTransmission(0x34);
    Wire.write(0x82);  //Enable all the ADCs
    Wire.write(0xff); 
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0x33);  //Enable Charging, 100mA, 4.2V, End at 0.9
    Wire.write(0xC0); 
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0x33);  
    Wire.write(0xC3); 
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0xB8);  //Enable Colume Counter
    Wire.write(0x80); 
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0x12);  
    Wire.write(0x4d); //Enable DC-DC1, OLED_VDD, 5B V_EXT
    Wire.endTransmission();

    Wire.beginTransmission(0x34);
    Wire.write(0x36);  
    Wire.write(0x5c); //PEK
    Wire.endTransmission();
	
	Wire.beginTransmission(0x34);
	Wire.write(0x90); 
    Wire.write(0x02); //gpio0	
	Wire.endTransmission();
	/*
	Wire.beginTransmission(0x34);
	Wire.write(0x91); 
    Wire.write(0xFF); //gpio0	
	Wire.endTransmission();
 */
}



void AXP192::ScreenBreath(uint8_t brightness){
    Wire.beginTransmission(0x34);
    Wire.write(0x28);  
    Wire.write(((brightness & 0x0f) << 4)); //Enable LDO2&LDO3, LED&TFT 3.3V
    Wire.endTransmission();
}