#include <M5StickC.h>

void setup() {
  // put your setup code here, to run once:
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.drawCentreString("Press btnA go sleep", 80, 30, 1);
}

void loop() {
    M5.update();

    if(M5.BtnA.wasPressed())
    {
        M5.Lcd.drawCentreString("Go light sleep (5s wakeup)", 80, 30, 1);
        delay(2000);

        // close tft lcd 
        M5.Axp.SetLDO2(false);
        // 6MA
        M5.Axp.LightSleep(SLEEP_SEC(5));
        // open tft lcd
        M5.Axp.SetLDO2(true); 

        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.drawCentreString("Week up from light sleep", 80, 30, 1);
        delay(2000);

        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.drawCentreString("Go deep sleep", 80, 20, 1);
        M5.Lcd.drawCentreString("Press BtnA or 20s", 80, 30, 1);
        M5.Lcd.drawCentreString("later wake up", 80, 40, 1);

        delay(2000);
        // 2 - 3 MA
        M5.Axp.DeepSleep(SLEEP_SEC(20));
    }

    if(M5.BtnB.wasPressed())
    {
        // will close esp32, can`t wakeup by timer
        M5.Axp.PowerOff();
    }

    delay(20);
}
