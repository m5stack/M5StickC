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
    Wire1.write(0xcc); //Enable LDO2&LDO3, LED&TFT 3.0V
    Wire1.endTransmission();
   
    Wire1.beginTransmission(0x34);
    Wire1.write(0x82);  //Enable all the ADCs
    Wire1.write(0xff); 
    Wire1.endTransmission();
   
    Wire1.beginTransmission(0x34);
    Wire1.write(0x33);  //Enable Charging, 200mA, 4.2V, End at 0.9
    Wire1.write(0xC1); 
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
    Wire1.write(0x36);  //pek setting
    Wire1.write(0b00001100); //shut down setting with 4s,auto shut donw while  btn press longer then shut donw setting
    Wire1.endTransmission();
    
    Wire1.beginTransmission(0x34);
    Wire1.write(0x90);
    Wire1.write(0x02); //gpio0 setting
    Wire1.endTransmission();
    
    Wire1.beginTransmission(0x34);
    Wire1.write(0x30); //VBUS limit
    Wire1.write(0xe0);
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x39);
    Wire1.write(0xFC);
    Wire1.endTransmission();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x35);
    Wire1.write(0xA2);
    Wire1.endTransmission();
}

void AXP192::ScreenBreath(uint8_t brightness){
  //limit in 3.0V
  if (brightness > 12) {
    brightness = 12;
  }
    
  Wire1.beginTransmission(0x34);
  Wire1.write(0x28);  //LDO2andLDO3 setting
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf = Wire1.read(); //read previous setting 

  Wire1.beginTransmission(0x34);
  Wire1.write(0x28);  
  Wire1.write(((buf & 0x0f) | (brightness << 4))); //just change the LDO2 setting
  Wire1.endTransmission();
    
}

//---------coulombcounter_from_here---------
//enable: void EnableCoulombcounter(void); 
//disable: void DisableCOulombcounter(void);
//stop: void StopCoulombcounter(void);
//clear: void ClearCoulombcounter(void);
//get charge data: uint32_t GetCoulombchargeData(void);
//get discharge data: uint32_t GetCoulombdischargeData(void);
//get coulomb val affter calculation: float GetCoulombData(void);
//------------------------------------------
void  AXP192::EnableCoulombcounter(void){
  
  Wire1.beginTransmission(0x34);
  Wire1.write(0xB8); 
  Wire1.write(0x80);
  Wire1.endTransmission();


}

void  AXP192::DisableCoulombcounter(void){

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB8); 
  Wire1.write(0x00);
  Wire1.endTransmission();

}

void  AXP192::StopCoulombcounter(void){
  Wire1.beginTransmission(0x34);
  Wire1.write(0xB8); 
  Wire1.write(0xC0);
  Wire1.endTransmission();

}

void  AXP192::ClearCoulombcounter(void){
    
  Wire1.beginTransmission(0x34);
  Wire1.write(0xB8); 
  Wire1.write(0xA0);
  Wire1.endTransmission();

}

uint32_t AXP192::GetCoulombchargeData(void){

  uint32_t coin;

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB0);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf = Wire1.read();

  

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB1);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf1 = Wire1.read();
 
  Wire1.beginTransmission(0x34);
  Wire1.write(0xB2);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf2 = Wire1.read();

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB3);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf3 = Wire1.read();

  coin = ((buf << 24) + (buf1 << 16) + (buf2 << 8) + buf3); 

  return coin;

}

uint32_t AXP192::GetCoulombdischargeData(void){

  uint32_t coout;

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB4);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf = Wire1.read();

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB5);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf1 = Wire1.read();

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB6);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf2 = Wire1.read();

  Wire1.beginTransmission(0x34);
  Wire1.write(0xB7);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf3 = Wire1.read();


  coout = ((buf << 24) + (buf1 << 16) + (buf2 << 8) + buf3); 

  return coout;

}

float AXP192::GetCoulombData(void){

  uint32_t coin = 0;
  uint32_t coout = 0;

  coin = GetCoulombchargeData();
  coout = GetCoulombdischargeData();

  //c = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
  //Adc rate can be read from 84H ,change this variable if you change the ADC reate
  float ccc = 65536 * 0.5 * (coin - coout) / 3600.0 / 25.0;
  return ccc;

}
//----------coulomb_end_at_here----------

uint16_t AXP192::GetVbatData(void){

    uint16_t vbat = 0;

    Wire1.beginTransmission(0x34);
    Wire1.write(0x78); // battery voltage LSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x79); // battery voltage MSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    vbat = ((buf << 4) + buf2); // V
    return vbat;

}

uint16_t AXP192::GetVinData(void){

    uint16_t vin = 0;

	// ACIN connect to the M5stickC 5Vin port
    Wire1.beginTransmission(0x34);
    Wire1.write(0x56); // ACIN voltage LSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x57); // ACIN voltage MSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    vin = ((buf << 4) + buf2); // V
    return vin;

}

uint16_t AXP192::GetIinData(void){

    uint16_t iin = 0;

	// ACIN current
    Wire1.beginTransmission(0x34);
    Wire1.write(0x58); // ACIN voltage LSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x59); // ACIN voltage MSB buff
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    iin = ((buf << 4) + buf2); // V
    return iin;

}

uint16_t AXP192::GetVusbinData(void){

    uint16_t vin = 0;

	//vbus voltage
    Wire1.beginTransmission(0x34);
    Wire1.write(0x5a); // vbus voltage LSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x5b); // vbus voltage MSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    vin = ((buf << 4) + buf2); // V
    return vin;

}

uint16_t AXP192::GetIusbinData(void){

    uint16_t iin = 0;

    // Vbus current 
    Wire1.beginTransmission(0x34);
    Wire1.write(0x5c); // vbus  current LSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x5d); // vbus  current MSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    iin = ((buf << 4) + buf2); // V
    return iin;

}

uint16_t AXP192::GetIchargeData(void){

    uint16_t icharge = 0;

    // battery charging current
    Wire1.beginTransmission(0x34);
    Wire1.write(0x7a); // LSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x7b); // MSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    icharge = ((buf << 5) + buf2);

    return icharge;

}

uint16_t AXP192::GetIdischargeData(void){

    uint16_t idischarge = 0;

    // battery discharge current
    Wire1.beginTransmission(0x34);
    Wire1.write(0x7c); // LSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x7d); // MSB
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    idischarge = ((buf << 5) + buf2);

    return idischarge;

}

uint16_t AXP192::GetTempData(void){

    uint16_t temp = 0;

	// get temp of AXP192
    Wire1.beginTransmission(0x34);
    Wire1.write(0x5e);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x5f);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    temp = ((buf << 4) + buf2);

    return temp;
}

uint32_t AXP192::GetPowerbatData(void){

    uint32_t power = 0;

    Wire1.beginTransmission(0x34);
    Wire1.write(0x70);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf0 = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x71);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf1 = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x72);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();

    power = (buf0 << 16) + (buf1 << 8) + buf2;

    return power;

}

uint16_t AXP192::GetVapsData(void){

    uint16_t vaps = 0;
    Wire1.beginTransmission(0x34);
    Wire1.write(0x7E);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x7F);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf2 = Wire1.read();
    
    vaps = ((uint16_t)(buf << 4) + buf2);
    return vaps;

}

void AXP192::SetSleep(void){

    Wire1.beginTransmission(0x34);
    Wire1.write(0x31);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();
    
    buf = (1<<3)|buf;
    Wire1.beginTransmission(0x34);
    Wire1.write(0x31);
    Wire1.write(buf);
    Wire1.endTransmission();
    
    Wire1.beginTransmission(0x34);
    Wire1.write(0x12);
    Wire1.write(0x01);
    Wire1.endTransmission();

}

uint8_t AXP192::GetWarningLeve(void){

    uint16_t vaps = 0;
    Wire1.beginTransmission(0x34);
    Wire1.write(0x47);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();
    return (buf & 0x01);
}

// -- sleep
void AXP192::DeepSleep(uint64_t time_in_us){
    
  SetSleep();
  
  if (time_in_us > 0){
    esp_sleep_enable_timer_wakeup(time_in_us);
  }else{
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
  
  (time_in_us == 0) ? esp_deep_sleep_start() : esp_deep_sleep(time_in_us);
      
}

void AXP192::LightSleep(uint64_t time_in_us){
    
  SetSleep();
  
  if (time_in_us > 0){
    esp_sleep_enable_timer_wakeup(time_in_us);
  }else{
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
  
  esp_light_sleep_start();
}

// 0 not press, 0x01 long press, 0x02 press
uint8_t AXP192::GetBtnPress() {
    Wire1.beginTransmission(0x34);
    Wire1.write(0x46);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t state = Wire1.read();
    if(state) {
        Wire1.beginTransmission(0x34);
        Wire1.write(0x46);
        Wire1.write(0x03);
        Wire1.endTransmission();
    }
    return state;
}

