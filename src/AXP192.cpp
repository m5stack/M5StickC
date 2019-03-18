#include "AXP192.h"

AXP192::AXP192(){
  
}

void AXP192::begin(void){
  
    Wire1.begin(21,22);
  
    Wire1.beginTransmission(0x34);
    Wire1.write(0x10);  
    Wire1.write(0xff);  //OLED_VPP Enable
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x28);  
    Wire1.write(0xff); //Enable LDO2&LDO3, LED&TFT 3.3V
    Wire1.endTransmission();
     
    Wire1.beginTransmission(0x34);
    Wire1.write(0x82);  //Enable all the ADCs
    Wire1.write(0xff); 
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x33);  //Enable Charging, 100mA, 4.2V, End at 0.9
    Wire1.write(0xC0); 
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x33);  
    Wire1.write(0xC3); 
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0xB8);  //Enable Colume Counter
    Wire1.write(0x80); 
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x12);  
    Wire1.write(0x4d); //Enable DC-DC1, OLED_VDD, 5B V_EXT
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x36);  
    Wire1.write(0x5c); //PEK
    Wire1.endTransmission();
	
	Wire1.beginTransmission(0x34);
	Wire1.write(0x90); 
    Wire1.write(0x02); //gpio0	
	Wire1.endTransmission();
	/*
	Wire1.beginTransmission(0x34);
	Wire1.write(0x91); 
    Wire1.write(0xFF); //gpio0	
	Wire1.endTransmission();
 */
}



void AXP192::ScreenBreath(uint8_t brightness){
    Wire1.beginTransmission(0x34);
    Wire1.write(0x28);  
    Wire1.write(((brightness & 0x0f) << 4)); //Enable LDO2&LDO3, LED&TFT 3.3V
    Wire1.endTransmission();
}