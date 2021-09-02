/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：Button.  按键
* date：2021/8/9
*******************************************************************************
  Please connect to Port,Read the button status of BUTTON Unit and display it on the screen

  请连接端口,读取按键的状态并在显示屏上显示
*/
#include <M5StickC.h>

int last_value = 0;
int cur_value = 0;

void setup() {
  M5.begin(); //Init M5StickC.  M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  pinMode(33, INPUT); //set pin mode to input.设置引脚模式为输入模式
  M5.Lcd.setTextColor(YELLOW);  //Set the font color to yellow.  设置字体颜色为黄色
  M5.Lcd.println("Button example");
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  cur_value = digitalRead(33); // read the value of BUTTON.  读取33号引脚的值
  M5.Lcd.setCursor(35,10); M5.Lcd.print("Button");  //Set the cursor position to (35,10).  将光标位置设置为(35,10)
  M5.Lcd.setCursor(0,25); M5.Lcd.print("Value: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print("State: ");
  if(cur_value != last_value){
    M5.Lcd.fillRect(20,25,75,85,BLACK);  //Draw a black rectangle 75 by 85 at (20,25).  在(20,25)处绘制宽75,高85的黑色矩形
    if(cur_value==0){
      M5.Lcd.setCursor(40,25); M5.Lcd.print("0"); // display the status.  在屏幕上显示按键的状态
      M5.Lcd.setCursor(40,45); M5.Lcd.print("pre");
    }
    else{
      M5.Lcd.setCursor(40,25); M5.Lcd.print("1"); // display the status
      M5.Lcd.setCursor(40,45); M5.Lcd.print("rel");
    }
    last_value = cur_value;
  }
}
