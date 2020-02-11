#include "IMU.h"
#include <math.h>
#include <Arduino.h>
#include "M5StickC.h"
#undef IMU

IMU::IMU() {
}

int IMU::Init(void) {
    int imu_flag = M5.Sh200Q.Init();
    if (imu_flag != 0) {
        imu_flag = M5.Mpu6886.Init();
        if (imu_flag == 0) {
            imuType = IMU_MPU6886;
        } else {
            imuType = IMU_UNKNOWN;
            return -1;
        }
    } else {
        imuType = IMU_SH200Q;
    }

    return 0;
}

void IMU::getGres() {
    if (imuType == IMU_SH200Q) {
        gRes = M5.Sh200Q.gRes;
    } else if (imuType == IMU_MPU6886) {
        gRes = M5.Mpu6886.gRes;
    }
}

void IMU::getAres() {
    if (imuType == IMU_SH200Q) {
        aRes = M5.Sh200Q.aRes;
    } else if (imuType == IMU_MPU6886) {
        aRes = M5.Mpu6886.aRes;
    }
}

void IMU::getAccelAdc(int16_t *ax, int16_t *ay, int16_t *az) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getAccelAdc(ax, ay, az);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getAccelAdc(ax, ay, az);
    }
}

void IMU::getAccelData(float *ax, float *ay, float *az) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getAccelData(ax, ay, az);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getAccelData(ax, ay, az);
    }
}

void IMU::getGyroAdc(int16_t *gx, int16_t *gy, int16_t *gz) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getGyroAdc(gx, gy, gz);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getGyroAdc(gx, gy, gz);
    }
}

void IMU::getGyroData(float *gx, float *gy, float *gz) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getGyroData(gx, gy, gz);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getGyroData(gx, gy, gz);
    }
}

void IMU::getTempAdc(int16_t *t) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getTempAdc(t);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getTempAdc(t);
    }
}

void IMU::getTempData(float *t) {
    if (imuType == IMU_SH200Q) {
        M5.Sh200Q.getTempData(t);
    } else if (imuType == IMU_MPU6886) {
        M5.Mpu6886.getTempData(t);
    }
}

void IMU::getAhrsData(float *pitch,float *roll,float *yaw){

  float accX = 0; 
  float accY = 0;
  float accZ = 0;

  float gyroX = 0;
  float gyroY = 0;
  float gyroZ = 0;


  getGyroData(&gyroX,&gyroY,&gyroZ);
  getAccelData(&accX,&accY,&accZ);
  
  MahonyAHRSupdateIMU(gyroX * DEG_TO_RAD, gyroY * DEG_TO_RAD, gyroZ * DEG_TO_RAD, accX, accY, accZ,pitch,roll,yaw);

}
