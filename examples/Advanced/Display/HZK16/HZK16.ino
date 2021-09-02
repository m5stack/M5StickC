/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：Character library.  字库
* date：2021/7/28
*******************************************************************************

-----PLEASE SEE THE README----
------请在使用前看README文件----*/
#include <M5StickC.h>
#include "str.h"

void setup() {
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.loadHzk16(); //Load the Chinese character library (be sure to load before using the Chinese character library).  加载汉字库(务必在使用汉字库前加载)
  M5.Lcd.setTextColor(WHITE, BLACK);//Set the text color to white and the text background color to black (mandatory).  设置文字颜色为白色,文字背景颜色为黑色(必加)
  // Set text with red highlight color
  M5.Lcd.setHighlightColor(RED);//Set the text highlight color to red.  设置文字高亮颜色为红色
  M5.Lcd.setTextSize(1);//Set text size to 1.  设置字号大小为1 Set text size to 1(必加)
}

void loop() {
  M5.Lcd.setCursor(0,0,1);  //Set the cursor at (0,0) and the size to 1(mandatory).  将光标设置在(0,0)处,并设置字号为1(必加)
  M5.Lcd.writeHzk(AscStr);  //Display the contents of AscStr here (which can be changed in str.h).  在此处显示AscStr中的内容(可在str.h中更改)
  // Print some ASCII text on the screen
  delay(1000);
  // Highlight the text
  M5.Lcd.setCursor(0,40);
  M5.Lcd.writeHzk(GbkStr); //Display the contents of GbkStr here (which can be changed in str.h).  在此处显示GbkStr中的内容(可在str.h中更改)
  delay(1000);
  //Highlight the text.  高亮显示文本
  M5.Lcd.highlight(true);  //Turn on highlight.  开启高亮显示
  M5.Lcd.setCursor(0,60);
  M5.Lcd.writeHzk(GbkStr);
  delay(1000);
  M5.Lcd.fillScreen(BLACK);  //Fill the screen with black color, equivalent to empty the screen.  填充屏幕颜色为黑色,等效于清空屏幕
  M5.Lcd.highlight(false); //Turn off highlight.  关闭高亮显示
  delay(500);
}
