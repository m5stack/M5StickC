/*
 * @Author: Sorzn
 * @Date: 2019-11-22 13:38:26
 * @LastEditTime: 2019-11-22 14:28:56
 * @Description: M5Stack Hat BUGC Lib
 * @FilePath: /M5StickC/examples/Hat/BUGC/bugC.cpp
 */

#include "M5StickC.h"
#include "bugC.h"

// if speed > 0; clockwise rotation
void BugCSetSpeed(uint8_t pos, int8_t speed)
{
    speed = (speed > 100) ? 100 : speed;
    speed = (speed < -100) ? -100 : speed;
    M5.I2C.writeByte(BUGC_ADDR, pos, speed);
}

void BugCSetAllSpeed(int8_t speed_0, int8_t speed_1, int8_t speed_2, int8_t speed_3)
{
    int8_t speed_out[4] = {speed_0, speed_1, speed_2, speed_3};
    for(uint8_t i = 0; i < 4; i++)
    {
        speed_out[i] = (speed_out[i] > 100) ? 100 : speed_out[i];
        speed_out[i] = (speed_out[i] < -100) ? -100 : speed_out[i];
    }
    M5.I2C.writeBytes(BUGC_ADDR, 0x00, (uint8_t *)speed_out, 4);
}

void BugCSetColor(uint32_t color_left, uint32_t color_right)
{
    uint8_t color_out[4];
    color_out[0] = 0;
    color_out[1] = (color_left & 0xff0000) >> 16;
    color_out[2] = (color_left & 0x00ff00) >> 8;
    color_out[3] = (color_left & 0x0000ff);
    M5.I2C.writeBytes(BUGC_ADDR, 0x10,  color_out, 4);

    color_out[0] = 1;
    color_out[1] = (color_right & 0xff0000) >> 16;
    color_out[2] = (color_right & 0x00ff00) >> 8;
    color_out[3] = (color_right & 0x0000ff);
    M5.I2C.writeBytes(BUGC_ADDR, 0x10,  color_out, 4);
}