#include <M5StickC.h>

#ifdef __cplusplus
extern "C"
{
  #include "IMU_6886.h"
}
#endif

IMU_6886 imu6886;

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float temp = 0;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
   M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 5);
  M5.Lcd.println("MPU6886 TEST");
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println("  X      Y       Z");
  imu6886.Init(32, 33);
}

void loop() {
  // put your main code here, to run repeatedly:
  imu6886.getGyroData(&gyroX,&gyroY,&gyroZ);
  imu6886.getAccelData(&accX,&accY,&accZ);
  imu6886.getTempData(&temp);

  M5.Lcd.setCursor(5, 35);
  M5.Lcd.printf("%.2f   %.2f   %.2f   ", gyroX, gyroY,gyroZ);
  M5.Lcd.setCursor(140, 35);
  M5.Lcd.print("o/s");
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.printf("%.2f   %.2f   %.2f   ",accX ,accY , accZ);
  M5.Lcd.setCursor(140, 50);
  M5.Lcd.print("G");
  M5.Lcd.setCursor(5, 65);
  M5.Lcd.printf("Temperature : %.2f C", temp);
  delay(100);
}
