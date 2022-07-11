/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5StickC sample source code
* Product:  HAT 8SERVO.
* Date: 2022/3/29
*******************************************************************************
  Control the running and release of the servo.
*/

#include <M5GFX.h>
#include <M5StickC.h>

#include "HAT_8SERVO.h"
M5GFX display;
M5Canvas canvas(&display);
HAT_8SERVO drive;
bool release = false;

void btn_task(void *arg) {
    while (1) {
        if (M5.BtnA.wasPressed()) {
            release = !release;
        }
        M5.update();
        vTaskDelay(20);
    }
}

void setup() {
    M5.begin();
    display.begin();
    if (display.isEPD()) {
        display.setEpdMode(epd_mode_t::epd_fastest);
        display.invertDisplay(true);
        display.clear(TFT_BLACK);
    }
    display.setRotation(1);
    canvas.setTextDatum(MC_DATUM);
    canvas.setTextColor(BLUE);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize((float)canvas.width() / 160);
    while (!drive.begin(&Wire, 0, 26, 0x36)) {
        canvas.drawString("Connect Error", 40, 80);
        canvas.pushSprite(0, 0);
        vTaskDelay(100);
    }
    canvas.drawString("8SERVO HAT", 80, 10);
    canvas.drawString("Click Btn A", 80, 20);
    canvas.drawString("for release Servo", 80, 30);
    canvas.pushSprite(0, 0);
    xTaskCreate(btn_task,   /* Task function. */
                "btn_task", /* String with name of task. */
                8096,       /* Stack size in bytes. */
                NULL,       /* Parameter passed as input of the task */
                1,          /* Priority of the task. */
                NULL);      /* Task handle. */
}

void loop() {
    if (release) {
        drive.enableServoPower(0);
        canvas.fillRect(0, 40, 160, 40, BLACK);
        canvas.drawString("SERVO RELEASE", 80, 50);
        canvas.pushSprite(0, 0);
        vTaskDelay(100);
    } else {
        canvas.fillRect(0, 40, 160, 40, BLACK);
        canvas.drawString("SERVO RUNNING", 80, 50);
        canvas.pushSprite(0, 0);
        // drive.setServoAngle(0, 0);
        // drive.setServoAngle(0, 180);
        // drive.setServoPulse(0, 500); 0deg
        // drive.setServoPulse(0, 2500); 180deg
        // drive.setAllServoAngle(0);
        // drive.setAllServoAngle(180);
        // drive.setAllServoPulse(500); 0deg
        // drive.setAllServoPulse(2500); 180deg
        drive.enableServoPower(1);
        vTaskDelay(100);
        drive.setAllServoAngle(180);
        Serial.println(String(drive.getServoAngle(0)));
        vTaskDelay(600);
        drive.setAllServoAngle(0);
        Serial.println(String(drive.getServoAngle(0)));
        vTaskDelay(600);
    }
}
