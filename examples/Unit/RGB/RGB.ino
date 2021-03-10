// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

/*
    Install the AdaFruit NeoPixel library first
    Github: https://github.com/adafruit/Adafruit_NeoPixel
 */
#include <M5StickC.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            32
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 150; // delay for half a second

void setup() {
  M5.begin();
  pixels.begin(); // This initializes the NeoPixel library.
 
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(6);
  M5.Lcd.setCursor(30, 20);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.printf("R");
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.printf("G");
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.printf("B");
}

void loop() {
  pixels.setPixelColor(0, pixels.Color(100,0,0)); // Moderately bright red color.
  pixels.setPixelColor(1, pixels.Color(0,100,0)); // Moderately bright green color.
  pixels.setPixelColor(2, pixels.Color(0,0,100)); // Moderately bright blue color.

  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds)

  pixels.setPixelColor(0, pixels.Color(0,100,0));
  pixels.setPixelColor(1, pixels.Color(0,0,100));
  pixels.setPixelColor(2, pixels.Color(100,0,0));
  pixels.show();
  delay(delayval);

  pixels.setPixelColor(0, pixels.Color(0,0,100));
  pixels.setPixelColor(1, pixels.Color(100,0,0));
  pixels.setPixelColor(2, pixels.Color(0,100,0));
  pixels.show();
  delay(delayval);

  // pixels.setPixelColor(0, pixels.Color(100,100,100));
  // pixels.setPixelColor(1, pixels.Color(100,100,100));
  // pixels.setPixelColor(2, pixels.Color(100,100,100));
  // pixels.show();
  // delay(delayval);
}
