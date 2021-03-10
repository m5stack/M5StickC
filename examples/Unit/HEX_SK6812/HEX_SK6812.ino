/*
    Description: Control HEX Unit to run rainbow light show
    Please install library before compiling:  
    FastLED: https://github.com/FastLED/FastLED
*/
#include <M5StickC.h>
#include "FastLED.h"

#define Neopixel_PIN    32
#define NUM_LEDS    37

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;

void setup() {
  M5.begin();

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(7, 5);
  M5.Lcd.println("HEX Example");
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.println("Display rainbow effect");

  // Neopixel initialization
  FastLED.addLeds<WS2811,Neopixel_PIN,GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(10);
}

void loop(){
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
    FastLED.show();// must be executed for neopixel becoming effective
    EVERY_N_MILLISECONDS( 20 ) { gHue++; }
}
