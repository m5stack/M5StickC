/*
 * @Author: Sorzn
 * @Date: 2019-11-22 14:48:24
 * @LastEditTime: 2019-11-22 15:44:44
 * @Description: M5Stack project
 * @FilePath: /M5StickC/examples/Hat/JoyC/JoyC.cpp
 */

#include "JoyC.h"
#include "M5StickC.h"

#define JOYC_ADDR 0x38

#define JOYC_COLOR_REG          0x20
#define JOYC_LEFT_X_REG         0x60
#define JOYC_LEFT_Y_REG         0x61
#define JOYC_RIGHT_X_REG        0x62
#define JOYC_RIGHT_Y_REG        0x63
#define JOYC_PRESS_REG          0x64
#define JOYC_LEFT_ANGLE_REG     0x70
#define JOYC_LEFT_DISTANCE_REG  0x74
#define JOYC_RIGHT_ANGLE_REG    0x72
#define JOYC_RIGHT_DISTANCE_REG 0x76

void JoyC::SetLedColor(uint32_t color)
{
    uint8_t color_buff[3];
    color_buff[0] = (color & 0xff0000) >> 16;
    color_buff[1] = (color & 0xff00) >> 8;
    color_buff[2] = color & 0xff;
    M5.I2C.writeBytes(JOYC_ADDR, JOYC_COLOR_REG, color_buff, 3);
}

uint8_t JoyC::GetX(uint8_t pos)
{
    uint8_t value;
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_X_REG : JOYC_RIGHT_X_REG;
    M5.I2C.readByte(JOYC_ADDR, read_reg, &value);
    return value;
}

uint8_t JoyC::GetY(uint8_t pos)
{
    uint8_t value;
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_Y_REG : JOYC_RIGHT_Y_REG;
    M5.I2C.readByte(JOYC_ADDR, read_reg, &value);
    return value;
}

uint16_t JoyC::GetAngle(uint8_t pos)
{
    uint8_t i2c_read_buff[2];
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_ANGLE_REG : JOYC_RIGHT_ANGLE_REG;
    M5.I2C.readBytes(JOYC_ADDR, read_reg, 2, i2c_read_buff);
    return (i2c_read_buff[0] << 8) | i2c_read_buff[1];
}

uint16_t JoyC::GetDistance(uint8_t pos)
{
    uint8_t i2c_read_buff[2];
    uint8_t read_reg;
    read_reg = (pos == 0) ? JOYC_LEFT_DISTANCE_REG : JOYC_RIGHT_DISTANCE_REG;
    M5.I2C.readBytes(JOYC_ADDR, read_reg, 2, i2c_read_buff);
    return (i2c_read_buff[0] << 8) | i2c_read_buff[1];    
}

uint8_t JoyC::GetPress(uint8_t pos)
{
    uint8_t press_value = 0;
    M5.I2C.readByte(JOYC_ADDR, JOYC_PRESS_REG, &press_value);
    return (press_value & ((pos == 0) ? 0x10 : 0x01)) != 0;
}