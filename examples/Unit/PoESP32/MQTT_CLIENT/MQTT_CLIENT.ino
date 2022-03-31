/*
    Description:
    Use UNIT PoESP32 connect to MQTT Server
    UNIT PoESP32 Connect to M5StickC PORT-A (G32/33)
    before compiling:
    M5StickC: https://github.com/m5stack/M5StickC
    M5GFX: https://github.com/m5stack/M5GFX
    UNIT_PoESP32: https://github.com/m5stack/UNIT_PoESP32
*/

#include "UNIT_PoESP32.h"
#include "M5StickC.h"
#include "M5GFX.h"

UNIT_PoESP32 eth;
String readstr;

M5GFX display;
M5Canvas canvas(&display);

void setup() {
    M5.begin();
    display.begin();
    display.setRotation(1);
    canvas.setColorDepth(1);  // mono color
    canvas.setPaletteColor(1, GREEN);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextScroll(true);
    eth.Init(&Serial2, 9600, 33, 32);
    delay(10);
    canvas.println("wait device connect");
    canvas.pushSprite(0, 0);
    while (!eth.checkDeviceConnect()) {
        delay(10);
    }

    canvas.println("device connected");
    canvas.println("wait ethernet connect");
    canvas.pushSprite(0, 0);
    while (!eth.checkETHConnect()) {
        delay(10);
    }
    canvas.println("ethernet connected");
    canvas.println("wait mqtt connect");
    canvas.pushSprite(0, 0);

    while (!eth.createMQTTClient("120.77.157.90", "1883", "client_id",
                                 "user_name", "password")) {
        delay(10);
    }
    canvas.println("mqtt connected");
    canvas.pushSprite(0, 0);
    while (!eth.subscribeMQTTMsg("PoESP32_MQTT_D", "2")) {
        delay(10);
    }
}

void loop() {
    if (Serial2.available()) {
        readstr = Serial2.readString();
        Serial.println(readstr);
        canvas.println(readstr);
        canvas.pushSprite(0, 0);
    }
    eth.publicMQTTMsg("PoESP32_MQTT_U", "Hello From PoESP32", "2");
    delay(2000);
}
