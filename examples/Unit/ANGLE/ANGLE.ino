#include <M5StickC.h>
// select the input pin for the potentiometer
int sensorPin = 33;
// last variable to store the value coming from the sensor
int last_sensorValue = 0;
// current variable to store the value coming from the sensor
int cur_sensorValue = 0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  pinMode(sensorPin, INPUT);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.print("the value of ANGLE: ");
}

void loop() {
  // read the value from the sensor:
  cur_sensorValue = analogRead(sensorPin);
  M5.Lcd.setCursor(120, 30);
  if(abs(cur_sensorValue - last_sensorValue) > 10){//debaunce
    M5.Lcd.fillRect(120, 30, 100, 25, BLACK);
    M5.Lcd.print(cur_sensorValue);
    last_sensorValue = cur_sensorValue;
  }
  delay(50);
}
