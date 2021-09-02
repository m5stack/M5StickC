/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：SPIFFS Delete
* date：2021/8/4
******************************************************************************
*/

#include <M5StickC.h>
#include <SPIFFS.h>

String file_name = "/M5Stack/notes.txt"; //Sets the location and name of the file to be operated on.  设置被操作的文件位置和名称
void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  if(SPIFFS.begin()){ // Start SPIFFS, return 1 on success.  启动闪存文件系统,若成功返回1
    M5.Lcd.println("\nSPIFFS Started.");  //Screen prints format String.  屏幕打印格式化字符串
  }else{
    M5.Lcd.println("SPIFFS Failed to Start.");
  }

  if(SPIFFS.remove(file_name)){  //Delete file_name file from flash, return 1 on success.  从闪存中删除file_name文件,如果成功返回1
    M5.Lcd.print(file_name);
    M5.Lcd.println(" Remove sucess");
  }else{
    M5.Lcd.print(file_name);
    M5.Lcd.println(" Remove fail");
  }
}

void loop() {
}