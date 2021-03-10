/*
 Note: The IMU_6886 is an I2C sensor and uses the Arduino Wire library.
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or
 a 3.3 V Teensy 3.1. We have disabled the internal pull-ups used by the Wire
 library in the Wire.h/twi.c utility file. We are also using the 400 kHz fast
 I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */
#ifndef __IMU_6886_H__
#define __IMU_6886_H__

#include <Wire.h>
#include <Arduino.h>

#define IMU_6886_ADDRESS           0x68 
#define IMU_6886_WHOAMI            0x75
#define IMU_6886_ACCEL_INTEL_CTRL  0x69
#define IMU_6886_SMPLRT_DIV        0x19
#define IMU_6886_INT_PIN_CFG       0x37
#define IMU_6886_INT_ENABLE        0x38
#define IMU_6886_ACCEL_XOUT_H      0x3B
#define IMU_6886_ACCEL_XOUT_L      0x3C
#define IMU_6886_ACCEL_YOUT_H      0x3D
#define IMU_6886_ACCEL_YOUT_L      0x3E
#define IMU_6886_ACCEL_ZOUT_H      0x3F
#define IMU_6886_ACCEL_ZOUT_L      0x40

#define IMU_6886_TEMP_OUT_H        0x41
#define IMU_6886_TEMP_OUT_L        0x42

#define IMU_6886_GYRO_XOUT_H       0x43
#define IMU_6886_GYRO_XOUT_L       0x44
#define IMU_6886_GYRO_YOUT_H       0x45
#define IMU_6886_GYRO_YOUT_L       0x46
#define IMU_6886_GYRO_ZOUT_H       0x47
#define IMU_6886_GYRO_ZOUT_L       0x48

#define IMU_6886_USER_CTRL         0x6A
#define IMU_6886_PWR_MGMT_1        0x6B
#define IMU_6886_PWR_MGMT_2        0x6C
#define IMU_6886_CONFIG            0x1A
#define IMU_6886_GYRO_CONFIG       0x1B
#define IMU_6886_ACCEL_CONFIG      0x1C
#define IMU_6886_ACCEL_CONFIG2     0x1D
#define IMU_6886_FIFO_EN           0x23

#define IMU_6886_FIFO_ENABLE       0x23
#define IMU_6886_FIFO_COUNT        0x72
#define IMU_6886_FIFO_R_W          0x74
#define IMU_6886_GYRO_OFFSET       0x13
//#define G (9.8)
#define RtA     57.324841
#define AtR    	0.0174533	
#define Gyro_Gr	0.0010653

class IMU_6886 {
    public:
      enum Ascale {
        AFS_2G = 0,
        AFS_4G,
        AFS_8G,
        AFS_16G
      };

      enum Gscale {
        GFS_250DPS = 0,
        GFS_500DPS,
        GFS_1000DPS,
        GFS_2000DPS
      };


    
    public:
      IMU_6886();
      int Init(uint8_t sda, uint8_t scl);
      void getAccelAdc(int16_t* ax, int16_t* ay, int16_t* az);
      void getGyroAdc(int16_t* gx, int16_t* gy, int16_t* gz);
      void getTempAdc(int16_t *t);

      void getAccelData(float* ax, float* ay, float* az);
      void getGyroData(float* gx, float* gy, float* gz);
      void getTempData(float *t);

      void setGyroFsr(Gscale scale);
      void setAccelFsr(Ascale scale);

      void getAhrsData(float *pitch,float *roll,float *yaw);
      void setFIFOEnable( bool enableflag );
      uint8_t ReadFIFO();
      void ReadFIFOBuff( uint8_t *DataBuff ,uint16_t Length );
      uint16_t ReadFIFOCount();
      void RestFIFO();
      void setGyroOffset(uint16_t x, uint16_t y, uint16_t z);

    public:
      float aRes, gRes, imuId;
      Gscale Gyscale;
      Ascale Acscale;

    private:

    private:
      void I2C_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer);
      void I2C_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer);
      void updateGres();
      void updateAres();
};
#endif
