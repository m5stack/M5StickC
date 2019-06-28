#include <M5StickC.h>

int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

int16_t gyroX = 0;
int16_t gyroY = 0;
int16_t gyroZ = 0;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("SH200I TEST");
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println("  X       Y       Z");
}



int16_t temp = 0;
/*****************************************
M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
M5.IMU.getAccelData(&accX,&accY,&accZ);
M5.IMU.getTempData(&temp);
*****************************************/
void loop() {
  // put your main code here, to run repeatedly:
  M5.IMU.getGyroAdc(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelAdc(&accX,&accY,&accZ);
  M5.IMU.getTempAdc(&temp);
  
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ", ((float) gyroX) * M5.IMU.gRes, ((float) gyroY) * M5.IMU.gRes,((float) gyroZ) * M5.IMU.gRes);
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.print("mg");
  M5.Lcd.setCursor(0, 45);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ",((float) accX) * M5.IMU.aRes,((float) accY) * M5.IMU.aRes, ((float) accZ) * M5.IMU.aRes);
  M5.Lcd.setCursor(140, 45);
  M5.Lcd.print("o/s");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf("Temperature : %.2f C",((float) temp) / 333.87 + 21.0);
  delay(1000);

}
