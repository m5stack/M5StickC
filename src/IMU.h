#ifndef __IMU_H__
#define __IMU_H__

#include <Wire.h>
#include <Arduino.h>
#include "utility/MahonyAHRS.h"

class IMU {
public:
    enum ImuType {
        IMU_UNKNOWN = 0, IMU_SH200Q, IMU_MPU6886
    };

    IMU();

    int Init(void);

    void getGres();
    void getAres();

    void getAccelAdc(int16_t *ax, int16_t *ay, int16_t *az);
    void getGyroAdc(int16_t *gx, int16_t *gy, int16_t *gz);
    void getTempAdc(int16_t *t);

    void getAccelData(float *ax, float *ay, float *az);
    void getGyroData(float *gx, float *gy, float *gz);
    void getTempData(float *t);

    void getAhrsData(float *pitch,float *roll,float *yaw);

    ImuType imuType;
    float aRes, gRes;

};

#endif
