/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: APX192.
* date：2021/9/3
*******************************************************************************
*/
#include <M5StickC.h>
#include "AXP192.h"
#include <M5GFX.h>
M5GFX display;

/* After M5StickC is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 M5StickC 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();
  display.begin();
  display.setRotation(3);
  M5.Axp.EnableCoulombcounter();  //Enable Coulomb counter.  启用库仑计数器
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  display.setCursor(0, 0, 1);
  display.printf("AXP Temp: %.1fC \r\n", M5.Axp.GetTempInAXP192());
  display.printf("Bat:\r\n  V: %.3fv  I: %.3fma\r\n", M5.Axp.GetBatVoltage(), M5.Axp.GetBatCurrent());
  display.printf("USB:\r\n  V: %.3fv  I: %.3fma\r\n", M5.Axp.GetVBusVoltage(), M5.Axp.GetVBusCurrent());
  display.printf("5V-In:\r\n  V: %.3fv  I: %.3fma\r\n", M5.Axp.GetVinVoltage(), M5.Axp.GetVinCurrent());
  display.printf("Bat power %.3fmw", M5.Axp.GetBatPower());

  // 0x01 long press(1s), 0x02 press
  if(M5.Axp.GetBtnPress() == 0x02)
  {
    esp_restart();
  }

  if(M5.BtnA.wasPressed())
  {
    // close tft voltage output.  关闭 tft 电压输出
    M5.Axp.SetLDO2(false);
    // close tft lcd voltage output 关闭tft lcd电压输出
    M5.Axp.SetLDO3(false);
  }

  M5.Axp.SetChargeCurrent(CURRENT_100MA); //set charge current(Not recommend to set charge current > 100mA, since Battery is only 80mAh). 设置充电电流(不建议设置充电电流 > 100mA，因为电池只有 80mAh。)

  M5.update();
  delay(100);
}
