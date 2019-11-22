/*
 * @Author: Sorzn
 * @Date: 2019-11-22 14:48:31
 * @LastEditTime: 2019-11-22 15:22:45
 * @Description: M5Stack HAT JOYC project
 * @FilePath: /M5StickC/examples/Hat/JoyC/JoyC.h
 */

#ifndef _HAT_JOYC_H_
#define _HAT_JOYC_H_

#include "Arduino.h"

class JoyC
{
    public:
        /**
         * @description: 
         * @param color: (r << 16) | (g << 8) | b 
         * @return: 
         */        
        void SetLedColor(uint32_t color);

        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: x value: 0 ~ 200
         */        
        uint8_t GetX(uint8_t pos);
        
        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: y value: 0 ~ 200
         */       
        uint8_t GetY(uint8_t pos);
        
        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: angle value: 0 ~ 360
         */        
        uint16_t GetAngle(uint8_t pos);

        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: 
         */        
        uint16_t GetDistance(uint8_t pos);
        
        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: 0 or 1
         */        
        uint8_t GetPress(uint8_t pos);
        
    private:
};

#endif