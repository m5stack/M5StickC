#include "M5StickC.h"
#include "bugC.h"

BUGC BugC;

void setup() 
{
    M5.begin();
    BugC.Init();
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setRotation(1);
    M5.Lcd.drawCentreString("BUGC example", 80, 30, 2);
    // if add battery, need increase charge current
    M5.Axp.SetChargeCurrent(CURRENT_360MA);
}

void loop() 
{
    M5.update();

    BugC.BugCSetColor(0x100000, 0x000010);
    BugC.BugCSetAllSpeed(-100, 100, -100, 100);
    delay(1000);
    BugC.BugCSetColor(0x000010, 0x100000);
    BugC.BugCSetAllSpeed(-100, 100, -100, 100);
    delay(1000);
    BugC.BugCSetAllSpeed(0, 0, 0, 0);
    BugC.BugCSetColor(0x000000, 0x000000);
    delay(1000);
}