// Download the required library functions https://github.com/m5stack/M5_IP5209
#include <M5StickC.h>
#include <Wire.h>

#include "M5_IP5209.h"
TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

M5_IP5209 powerc;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    canvas.createSprite(160, 80);
    canvas.setTextColor(ORANGE);
    while (!powerc.begin()) {
        canvas.fillSprite(BLACK);
        canvas.setCursor(0, 10);
        canvas.println("POWERC INIT ERROR");
        canvas.println("Pls click the power btn and check the I2C connect.");
        canvas.pushSprite(10, 0);
        delay(1000);
    }
}

char info[50];

void loop() {
    if (powerc.update()) {
        float v     = powerc.voltage;
        float i     = powerc.current;
        float p     = powerc.power;
        bool charge = powerc.charged;
        canvas.fillSprite(BLACK);
        canvas.setCursor(0, 10);
        sprintf(info, "BAT: %1.2fV\r\n%1.2fmA\r\npower:%1.2fw\r\nischarge: %d",
                v, i, p, charge);
        canvas.println(info);
        canvas.pushSprite(10, 0);
        delay(100);
    } else {
        canvas.fillSprite(BLACK);
        canvas.setCursor(0, 10);
        canvas.println("powerc update error..");
        canvas.pushSprite(10, 0);
    }
}
