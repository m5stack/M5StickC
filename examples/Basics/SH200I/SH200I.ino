#include <M5StickC.h>
#include <IMU.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  sh200i_init();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("SH200I TEST");
  M5.Lcd.println();
  M5.Lcd.println("    X       Y       Z");
}

long loopTime, startTime, endTime, fps;
int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

int16_t gyroX = 0;
int16_t gyroY = 0;
int16_t gyroZ = 0;

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char tempdata[1];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X, 1, tempdata);
  accX = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+1, 1, tempdata);
  accX |= tempdata[0] << 8;


  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+2, 1, tempdata);
  accY = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+3, 1, tempdata);
  accY |= tempdata[0] << 8;
    
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+4, 1, tempdata);
  accZ = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+5, 1, tempdata);
  accZ |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+6, 1, tempdata);
  gyroX = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+7, 1, tempdata);
  gyroX |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+8, 1, tempdata);
  gyroY = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+9, 1, tempdata);
  gyroY |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+10, 1, tempdata);
  gyroZ = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+11, 1, tempdata);
  gyroZ |= tempdata[0] << 8;


  Serial.printf("aX = %ld,aY = %ld,aZ = %ld,gX = %ld,gY = %ld,gZ = %ld\r\n", accX, accY, accZ,gyroX, gyroY, gyroZ);
  M5.Lcd.setCursor(0, 30, 1);
  M5.Lcd.printf("  %ld    %ld    %ld\r\n", accX, accY, accZ);
  M5.Lcd.println();
  M5.Lcd.printf("  %ld      %ld     %ld   ", gyroX, gyroY, gyroZ);
  delay(1000);

}
