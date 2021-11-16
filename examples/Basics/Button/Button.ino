/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: Button.
* date：2021/11/16
*******************************************************************************
*/
#include <M5StickC.h>

/* After M5StickC is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 M5StickC 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup(){
  M5.begin(); // Initialize the M5StickC object.  初始化 M5StickC 对象
  M5.Lcd.setTextColor(YELLOW);  //Set the font color to yellow.  设置字体颜色为黄色
  M5.Lcd.setRotation(3);
  M5.Lcd.println("Button example"); //The screen prints the formatted string and wraps the line.  输出格式化字符串并换行
  M5.Lcd.println("Press button B for 700ms");
  M5.Lcd.println("to clear screen.");
  M5.Lcd.setTextColor(RED);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.update(); //Read the press state of the key.  读取按键 A, B, C 的状态
  if (M5.BtnA.wasReleased()) {  //If the button A is pressed.  如果按键 A 被按下
    M5.Lcd.print('A');
  } else if (M5.BtnB.wasReleased()) { //If the button B is pressed.  如果按键 B 被按下，
    M5.Lcd.print('B');
  } else if (M5.BtnB.wasReleasefor(700)) {  //The button B is pressed for 700ms. 按键 B 按下 700ms,屏幕清空
    M5.Lcd.fillScreen(BLACK);  //Set BLACK to the background color.  将黑色设置为底色
    M5.Lcd.setCursor(0, 0);
  }
}