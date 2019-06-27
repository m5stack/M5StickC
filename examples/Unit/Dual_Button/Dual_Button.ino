/*
  This Unit connects to GRVOE B on M5Core.
*/
#include <M5StickC.h>

int last_value_red = 0;
int cur_value_red = 0;
int last_value_blue = 0;
int cur_value_blue = 0;

void setup() {
  // init lcd
  M5.begin();
  M5.Lcd.setRotation(3);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(15, 2);
  M5.Lcd.println("Dual Button example");
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  cur_value_red = digitalRead(32);
  cur_value_blue = digitalRead(33);

  M5.Lcd.setCursor(0,15); M5.Lcd.print("Blue Status: ");
  M5.Lcd.setCursor(0,30); M5.Lcd.print("Blue Value: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print(" Red Status: ");
  M5.Lcd.setCursor(0,60); M5.Lcd.print(" Red Value: ");

  if(cur_value_blue != last_value_blue){
    M5.Lcd.fillRect(95,15,100,15,BLACK);
    M5.Lcd.fillRect(95,30,100,15,BLACK);
    if(cur_value_blue==0){
      M5.Lcd.setCursor(95,15); M5.Lcd.print("pressed");
      M5.Lcd.setCursor(95,30); M5.Lcd.print("0");
      Serial.println("Button Status: blue pressed");
      Serial.println(" value: 0");
    }
    else{
      M5.Lcd.setCursor(95,15); M5.Lcd.print("released");
      M5.Lcd.setCursor(95,30); M5.Lcd.print("1");
      Serial.println("Button Status: blue released");
      Serial.println(" value: 1");
    }
      last_value_blue = cur_value_blue;
  }

    if(cur_value_red != last_value_red){
      M5.Lcd.fillRect(95,45,100,15,BLACK);
      M5.Lcd.fillRect(95,60,100,15,BLACK);
      if(cur_value_red==0){
        M5.Lcd.setCursor(95,45); M5.Lcd.print("pressed");
        M5.Lcd.setCursor(95,60); M5.Lcd.print("0");
        Serial.println("Button Status: red pressed");
        Serial.println(" value: 0");
      }
      else{
        M5.Lcd.setCursor(95,45); M5.Lcd.print("released");
        M5.Lcd.setCursor(95,60); M5.Lcd.print("1");
        Serial.println("Button Status: red released");
        Serial.println(" value: 1");
      }
      last_value_red = cur_value_red;
  }

  M5.update();
}
