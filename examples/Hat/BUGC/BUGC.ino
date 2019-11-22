#include "M5StickC.h"
#include "bugC.h"

void setup() 
{
    M5.begin();
    Wire.begin(0, 26, 400000);
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setRotation(1);
    M5.Lcd.drawCentreString("BUGC example", 80, 30, 2);
    // if add battery, need increase charge current
    M5.Axp.SetChargeCurrent(CURRENT_360MA);
}

void loop() 
{
    M5.update();

    if(M5.BtnA.wasPressed())
    {
        BugCSetColor(0x100000, 0x001000);
        BugCSetAllSpeed(-100, 100, -100, 100);
    }

    if(M5.BtnB.wasPressed())
    {
        BugCSetColor(0x000000, 0x000000);
        BugCSetAllSpeed(0, 0, 0, 0);
    }

    delay(10);
}