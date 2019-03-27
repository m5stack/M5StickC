/* 
  在 src.h 文件中 GbkStr 显示为乱码为正常现象
  HZK16为GB2312中文编码格式字库, 需要显示中文建议使用notepad++或其它软件（Arduino IDE 为UTF-8）通过编码GB2312打开str.h
  即可修改为个人想要输出中文字符
*/
#include <M5StickC.h>
#include "str.h"

void setup() {
  
  M5.begin();
  
  M5.Lcd.loadHzk16();

  M5.Lcd.setTextColor(WHITE, BLACK);
  
  // Set text with red highlight color
  M5.Lcd.setHighlightColor(RED);
  
  // Set text size to 1
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0,0,1);
  M5.Lcd.writeHzk(AscStr);
  // Print some ASCII text on the screen
  delay(1000);
  // Highlight the text
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,32);
  M5.Lcd.writeHzk(AscStr);
  delay(1000); 
  // Clear screen
  //M5.Lcd.clear();
  // Turn highlight off
  M5.Lcd.highlight(false);
  M5.Lcd.setCursor(0,70);
  // print some GBK text on the screen
  M5.Lcd.writeHzk(GbkStr);
  
  delay(1000);
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,90);
  M5.Lcd.writeHzk(GbkStr);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
