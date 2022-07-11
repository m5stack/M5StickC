/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc
*
* Describe:  Wifi scan.  wifi扫描
* Date: 2022/4/3
*******************************************************************************
  http://forum.m5stack.com/topic/58/lesson-3-wi-fi-scanner
  Press M5 to start/re-scan wifi, press up to turn down the page
  按下M5键以开始/重新扫描wifi,按下上键向下翻页
*/

#include <M5StickC.h>

#include "WiFi.h"

int n;
int ssidLength     = 10;
int thisPage       = 0;
const int pageSize = 8;
bool on            = false;

void LCD_Clear() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
}

void Show(int nav = 0)  // -1 top, 1 bottom
{
    if ((nav == 1) && (on == true)) {
        if ((thisPage) < ((n - 1) / pageSize)) {
            thisPage++;
        } else {
            thisPage = 0;
        }
        Show();
    } else {
        LCD_Clear();
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(30, 5);
        M5.Lcd.print("TOTAL: ");
        M5.Lcd.print(n);
        M5.Lcd.setCursor(0, 16);
        for (int i = (thisPage * pageSize);
             i < ((thisPage * pageSize) + pageSize); i++) {
            if (i >= n) break;
            String ssid = (WiFi.SSID(i).length() > ssidLength)
                              ? (WiFi.SSID(i).substring(0, ssidLength) + "...")
                              : WiFi.SSID(i);
            M5.Lcd.print(" " + String(i + 1));
            M5.Lcd.print(") " + ssid + " (" + WiFi.RSSI(i) + ");\n");
        }
    }
}

void Search() {
    on       = true;
    thisPage = 0;
    LCD_Clear();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.setTextColor(TFT_YELLOW);
    M5.Lcd.printf("Please, wait");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_ORANGE);
    M5.Lcd.setCursor(40, 50);
    M5.Lcd.printf("Searching...");
    n = WiFi.scanNetworks();
    Show();
}

void setup() {
    M5.begin();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(45, 20);
    M5.Lcd.printf("Wi-Fi");
    M5.Lcd.setCursor(30, 45);
    M5.Lcd.printf("scanner");
}

void loop() {
    if (M5.BtnA.wasPressed()) Search();
    if (M5.BtnB.wasPressed()) Show(1);
    M5.update();
}