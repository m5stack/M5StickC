/*
 * @Author: Sorzn
 * @Date: 2019-11-22 14:48:10
 * @LastEditTime: 2019-11-22 15:45:27
 * @Description: M5Stack project
 * @FilePath: /M5StickC/examples/Hat/JoyC/JoyC.ino
 */

#include "M5StickC.h"
#include "JoyC.h"

JoyC joyc;
TFT_eSprite img = TFT_eSprite(&M5.Lcd);

uint8_t show_flag = 0;

void setup()
{
    M5.begin();
    Wire.begin(0, 26, 400000);
    img.createSprite(80, 160);
}

void loop()
{
    char text_buff[100];
    
    M5.update();
    img.fillSprite(TFT_BLACK);
    if(show_flag == 0)
    {
        img.drawCentreString("angle right", 40, 6, 1);
        sprintf(text_buff, "%d", joyc.GetAngle(1));
        img.drawCentreString(text_buff, 40, 20, 1);

        img.drawCentreString("dis right", 40, 34, 1);
        sprintf(text_buff, "%d", joyc.GetDistance(1));
        img.drawCentreString(text_buff, 40, 48, 1);
        
        img.drawCentreString("x right", 40, 62, 1);
        sprintf(text_buff, "%d", joyc.GetX(1));
        img.drawCentreString(text_buff, 40, 76, 1);
        
        img.drawCentreString("y right", 40, 90, 1);
        sprintf(text_buff, "%d", joyc.GetY(1));
        img.drawCentreString(text_buff, 40, 104, 1);

        img.drawCentreString("press right", 40, 118, 1);
        sprintf(text_buff, "%d", joyc.GetPress(1));
        img.drawCentreString(text_buff, 40, 132, 1);
    }
    else
    {
        img.drawCentreString("angle left", 40, 6, 1);
        sprintf(text_buff, "%d", joyc.GetAngle(0));
        img.drawCentreString(text_buff, 40, 20, 1);

        img.drawCentreString("dis left", 40, 34, 1);
        sprintf(text_buff, "%d", joyc.GetDistance(0));
        img.drawCentreString(text_buff, 40, 48, 1);
        
        img.drawCentreString("x left", 40, 62, 1);
        sprintf(text_buff, "%d", joyc.GetX(0));
        img.drawCentreString(text_buff, 40, 76, 1);
        
        img.drawCentreString("y left", 40, 90, 1);
        sprintf(text_buff, "%d", joyc.GetY(0));
        img.drawCentreString(text_buff, 40, 104, 1);

        img.drawCentreString("press left", 40, 118, 1);
        sprintf(text_buff, "%d", joyc.GetPress(0));
        img.drawCentreString(text_buff, 40, 132, 1);
    }
    img.pushSprite(0, 0);
    
    if(M5.BtnA.wasPressed())
    {
        joyc.SetLedColor(0x100010);    
        show_flag = 1 - show_flag;
    }

    delay(10);
}