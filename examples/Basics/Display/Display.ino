/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: Display.
* date：2021/9/3
*******************************************************************************
*/
#include <M5StickC.h>

/* After M5StickC is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 M5StickC 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();
  // Lcd display
  M5.Lcd.fillScreen(WHITE);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  // text print.  文字打印
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("Display Test!");

  // draw graphic.  绘图🌹
  delay(1000);
  M5.Lcd.drawRect(15, 55, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.fillRect(15, 55, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.drawCircle(40, 80, 30, RED);
  delay(1000);
  M5.Lcd.fillCircle(40, 80, 30, RED);
  delay(1000);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop(){
  //rand draw
  M5.Lcd.fillTriangle(random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(0xfffe));
}
