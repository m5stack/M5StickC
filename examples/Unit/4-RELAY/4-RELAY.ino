/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：4-Relay.  4个继电器
* date：2021/8/10
*******************************************************************************
  Please connect to port,Control 4 relays and demonstrate the asynchronous control relay LED
  请连接端口，控制4继电器，并演示异步控制继电器LED
-------------------------------------------------------------------------------
  RELAY control reg           | 0x10
  -----------------------------------------------------------------------------
  Relay_ctrl_mode_reg[0]      | R/W | System control
                              | 7 | 6 | 5 | 4 | 3 | 2 | 1 |     0     |
                              | R | R | R | R | R | R | R | Sync Mode |
                              | -Sync Mode:0 LED&Relay Async
                              | -Sync Mode:1 LED&Relay Sync
  ---------------------------------------------------------------------------------
  Relay_ctrl_mode_reg[1]      | R/W | Relay & LED control
                              |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
                              | LED1| LED2| LED3| LED4| RLY1| RLY2| RLY3| RLY4|
-------------------------------------------------------------------------------*/

#include <M5StickC.h>
#include "UNIT_4RELAY.h"
UNIT_4RELAY unit_4relay;

void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.setCursor(50, 5);  //Set the cursor position to (50,5).  将光标位置设置为(50,5)
  M5.Lcd.print("4-RELAY UNIT");
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.print("Independent Switch:");
  Wire.begin(32, 33);  //Initialize pin 32,33.  初始化32,33引脚
  unit_4relay.Init(1);  //Set the lamp and relay to asynchronous mode(Async = 0,Sync = 1).  将灯和继电器设为非同步模式
}

char count_i = 1;
bool sync_flag = 0, all_flag= 0;

void loop() {
  M5.update();  //Check button down state.  检测按键按下状态
  if(M5.BtnA.wasPressed()){ //If button A is pressed.  如果按键A按下
    M5.Lcd.fillRect(117, 25, 40, 20, BLACK);
    M5.Lcd.setCursor(117, 25);
    if(count_i<5){
      M5.Lcd.printf("%d ON", count_i);
      unit_4relay.relayWrite(count_i-1,1);  //Open the relay at Count_i.  打开count_i处的继电器
      unit_4relay.relayWrite(count_i-2,0);
      unit_4relay.LEDWrite(count_i-1,1);
      unit_4relay.LEDWrite(count_i-2,0);
    }else{
      M5.Lcd.printf("ALL.ON ");
      unit_4relay.relayALL(1);  //Open all the relays.  打开所有的继电器
      unit_4relay.LED_ALL(1);
    }
    count_i++;
    if(count_i > 6) {
      count_i = 1;
      unit_4relay.relayALL(0);  //Open all the relays.  打开所有的继电器
      unit_4relay.LED_ALL(0);
    }
  }
}