#ifndef __AXP192_H__
#define __AXP192_H__

#include <Wire.h>


class AXP192 {
public:
  AXP192();
  void Init(void);
  void ScreenBreath(uint8_t brightness);
public:
private:
   
private:
}; 

#endif
