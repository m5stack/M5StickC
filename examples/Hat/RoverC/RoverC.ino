/*
    Description: Control RoverC, move forward and backward, left and right.
*/

#include <M5StickC.h>
#include "RoverC.h"

// the setup routine runs once when M5StickC starts up
void setup(){ 
  
  // Initialize the M5StickC object
  M5.begin();
  
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(40, 30, 4);
  M5.Lcd.printf("Rover");


  RoverC_Init();
}

// the loop routine runs over and over again forever
void loop() {

  Move_forward(100);
  delay(3000);
  Move_back(100);
  delay(3000);
  Move_left(100);
  delay(3000);
  Move_right(100);
  delay(3000);

}
