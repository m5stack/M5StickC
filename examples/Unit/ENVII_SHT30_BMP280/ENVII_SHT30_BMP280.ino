/*
    Description: Use ENV II Unit to read temperature, humidity, atmospheric pressure, and display the data on the screen.
    Please install library before compiling:  
    Adafruit BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
*/
#include <M5StickC.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"
SHT3X sht30;
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin();
  Wire.begin(32,33);
  M5.Lcd.setTextSize(1);
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));

  while (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  M5.Lcd.fillRect(0, 0, 80, 160, BLACK);
}

void loop() {
  pressure = bme.readPressure();
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure); 
 
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(2, 0);
  M5.Lcd.printf("Temp:");
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.printf("%2.1f", tmp);
  M5.Lcd.setCursor(2, 25);
  M5.Lcd.printf("Humi:");
  M5.Lcd.setCursor(5, 35);
  M5.Lcd.printf("%2.0f%%",hum);
  M5.Lcd.setCursor(2, 50);
  M5.Lcd.printf("Pressure:");
  M5.Lcd.setCursor(5, 60);
  M5.Lcd.printf("%2.0fPa", pressure);
  
  delay(100);

}
