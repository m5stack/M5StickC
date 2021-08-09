/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Button_Two.  双按键
* date：2021/8/9
*******************************************************************************
  Please connect to Port,Read the button status of BUTTON Unit and display it on the screen

  请连接端口,读取按键的状态并在显示屏上显示
*/
#include <M5StickC.h>

int last_value1 = 0,last_value2 = 0;
int cur_value1 = 0,cur_value2 = 0;

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  pinMode(33, INPUT); //set pin mode to input.设置引脚模式为输入模式
  pinMode(32, INPUT);
  M5.Lcd.setTextColor(YELLOW);  //Set the font color to yellow.  设置字体颜色为黄色
  M5.Lcd.println("Button example");
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  cur_value1 = digitalRead(33); // read the value of BUTTON.  读取33号引脚的值
  cur_value2 = digitalRead(32);
  M5.Lcd.setCursor(35,10); M5.Lcd.print("Btn.1  Btn.2");   //Set the cursor position to (35,10).  将光标位置设置为(35,10)
  M5.Lcd.setCursor(0,25); M5.Lcd.print("Value: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print("State: ");
  if(cur_value1 != last_value1){
    M5.Lcd.fillRect(20,25,45,40,BLACK); //Draw a black rectangle 45 by 40 at (20,25).  在(20,25)处绘制宽45,高40的黑色矩形
    if(cur_value1==0){
      M5.Lcd.setCursor(40,25); M5.Lcd.print("0"); // display the status
      M5.Lcd.setCursor(40,45); M5.Lcd.print("pre");
    }
    else{
      M5.Lcd.setCursor(40,25); M5.Lcd.print("1"); // display the status
      M5.Lcd.setCursor(40,45); M5.Lcd.print("rel");
    }
    last_value1 = cur_value1;
  }
  if(cur_value2 != last_value2){
    M5.Lcd.fillRect(70,25,45,40,BLACK);
    if(cur_value2==0){
      M5.Lcd.setCursor(80,25); M5.Lcd.print("0"); // display the status
      M5.Lcd.setCursor(80,45); M5.Lcd.print("pre");
    }
    else{
      M5.Lcd.setCursor(80,25); M5.Lcd.print("1"); // display the status
      M5.Lcd.setCursor(80,45); M5.Lcd.print("rel");
    }
    last_value2 = cur_value2;
  }
}
