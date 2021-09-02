/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：Hall sensor.  霍尔传感器
* date：2021/7/27
*******************************************************************************
  The ESP32 MCU has build in Hall sensor.  ESP32 MCU内置霍尔传感器
  It is not very suscescible but if you bring a magnet close to the
  upper screen you will see the influence on the measurement.
  In this example we use some low pass filters to get rid of the noise.
*/
#include <M5StickC.h>

#define HORIZONTAL_RESOLUTION 80  //屏幕水平分辨率
#define VERTICAL_RESOLUTION   160 //屏幕竖直分辨率
#define POSITION_OFFSET_Y     20

uint16_t oldSignal[HORIZONTAL_RESOLUTION];

float ESP32_hallRead()  //ESP32 hall value read.  ESP32霍尔值读取
{
  float value = 0;
  int count = 400;
  // mean value filter.  数据低通滤波器
  for (int n = 0; n < count; n++) value += hallRead();
  return value / count * 10;
}

float HallOffset = 0; ////Store the initial value of magnetic force.  存储磁力的初值

void setup()
{
  M5.begin(); //Init M5StickC.  初始化M5StickC
  M5.Lcd.printf("\nESP32 Hall sensor:"); //Screen printout.  屏幕打印输出
  HallOffset = ESP32_hallRead(); // callibrate the output value to the magnetic field at start up.  在启动时将输出值校准到磁场
}

float LowPassFilteredValue=0;
void showSignal()
{
  int n;

  int oldx;
  int oldy;
  int oldSig;
  int x, y;

  for (n = 0; n < HORIZONTAL_RESOLUTION; n++) //在水平分辨率内,每个像素点根据计算得到的磁力大小绘制
  {                                           //Within the horizontal resolution, each pixel is drawn according to the calculated magnetic force
    x = n;
    float value = ESP32_hallRead()- HallOffset; //Reduce the influence of own magnetism.  减少自身磁力的影响
    LowPassFilteredValue+=(value-LowPassFilteredValue)*0.05;
    M5.Lcd.setCursor(30, 17);
    M5.Lcd.print((int)LowPassFilteredValue);
    M5.Lcd.print("    ");
    M5.Lcd.setCursor(0, 40);
    y = map(value , -127, 127, VERTICAL_RESOLUTION, POSITION_OFFSET_Y); //将value映射至-30~30的范围内,以免绘制超出屏幕

    if (n > 0)
    {
      // delete old line element.  删除上次画的线
      M5.Lcd.drawLine(oldx , oldSig, x, oldSignal[n], BLACK );

      // draw new line element.  画新的线
      if (n < HORIZONTAL_RESOLUTION - 1) // don't draw last element because it would generate artifacts.  不能绘制最后一个像素
      {
        M5.Lcd.drawLine(oldx,oldy,x,y, GREEN);
      }
    }
    oldx = x;
    oldy = y;
    oldSig = oldSignal[n];
    oldSignal[n] = y;
  }
}

void loop(void)
{
  showSignal();
}
