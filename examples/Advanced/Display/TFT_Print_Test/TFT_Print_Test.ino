/*  
 Test the M5.Lcd.print() viz embedded M5.Lcd.write() function

 This sketch used font 2, 4, 7

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */
#include <M5StickC.h>

#define TFT_GREY 0x5AEB // New colour


void setup(void) {
  M5.begin();
  M5.Lcd.setRotation(3);
}

void loop() {
  
  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  M5.Lcd.fillScreen(TFT_GREY);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'M5.Lcd.println'
  //  or stay on the line is there is room for the text with M5.Lcd.print)
  M5.Lcd.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);  
  M5.Lcd.setTextSize(1);
  // We can now plot text on screen using the "print" class
  M5.Lcd.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 6
  M5.Lcd.setTextColor(TFT_YELLOW); 
  M5.Lcd.setTextFont(4);
  M5.Lcd.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  M5.Lcd.setTextColor(TFT_RED,TFT_BLACK);    M5.Lcd.setTextFont(4);
  //M5.Lcd.println(3735928559L, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 4
  M5.Lcd.setTextColor(TFT_GREEN,TFT_BLACK);
  M5.Lcd.setTextFont(2);
  M5.Lcd.println("Groop");
  M5.Lcd.println("I implore thee,");

  delay(2000);
  M5.Lcd.fillScreen(TFT_GREY);
  M5.Lcd.setCursor(0, 0, 2);

  
  // Test some print formatting functions
  float fnumber = 123.45;
  // Set the font colour to be blue with no background, set to font 4
  M5.Lcd.setTextColor(TFT_BLUE);    M5.Lcd.setTextFont(2);
  M5.Lcd.print("Float = "); M5.Lcd.println(fnumber);           // Print floating point number
  M5.Lcd.print("Binary = "); M5.Lcd.println((int)fnumber, BIN); // Print as integer value in binary
  M5.Lcd.print("Hexadecimal = "); M5.Lcd.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
  
  delay(1000);
}
