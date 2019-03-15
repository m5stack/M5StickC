#include <M5StickC.h>

// the setup routine runs once when M5StickC starts up
void setup() {
  
  // initialize the M5StickC object
  M5.begin();

  // Lcd display
  M5.Lcd.fillScreen(WHITE);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  // text print
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("Display Test!");

  // draw graphic
  delay(1000);
  M5.Lcd.drawRect(15, 55, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.fillRect(15, 55, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.drawCircle(40, 80, 30, RED);
  delay(1000);
  M5.Lcd.fillCircle(40, 80, 30, RED);
  delay(1000);
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  M5.Lcd.fillTriangle(random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(0xfffe));

  //M5.update();
}
