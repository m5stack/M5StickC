/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/iso485
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/iso485
*
* Product:  iso485.
* Date: 2021/8/30
*******************************************************************************
  Please connect to Port ,Pressed ButtonA :send "hello world"
  请连接端口 ,Pressed ButtonA :send "hello world"
*/

#include <M5StickC.h>

String str = "";

void setup() {
    M5.begin();
    M5.Lcd.drawString("ISO485", 20, 0, 2);
    Serial2.begin(115200, SERIAL_8N1, 32, 33);
    M5.Lcd.setCursor(0, 20);
}

void loop() {
    if (M5.BtnA.wasPressed()) {
        Serial2.write("HELLO World\r\n");
    }

    if (Serial2.available()) {
        char ch = Serial2.read();
        str += ch;
        if (str.endsWith("\r\n")) {
            Serial.print(str);
            M5.Lcd.print(str);
            str = "";
        }
    }
    M5.update();
}
