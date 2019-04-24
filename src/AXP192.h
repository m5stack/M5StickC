#ifndef __AXP192_H__
#define __AXP192_H__

#include <Wire.h>
#include <Arduino.h>

class AXP192 {
public:
  AXP192();
  void  begin(void);
  void  ScreenBreath(uint8_t brightness);
  
  void  EnableCoulombcounter(void);
  void  DisableCoulombcounter(void);
  void  StopCoulombcounter(void);
  void  ClearCoulombcounter(void);
  
 
  uint32_t GetCoulombchargeData(void);
  uint32_t GetCoulombdischargeData(void);
  float GetCoulombData(void);

  uint16_t GetVbatData(void);


  uint16_t GetIchargeData(void);
  uint16_t GetIdischargeData(void);

  uint16_t GetTempData(void);

  uint32_t GetPowerbatData(void);

  uint16_t GetVinData(void);
  uint16_t GetIinData(void);

  uint16_t GetVapsData(void);
  void SetSleep(void);

  uint8_t GetWarningLeve(void);
public:
private:
   
}; 

#endif
