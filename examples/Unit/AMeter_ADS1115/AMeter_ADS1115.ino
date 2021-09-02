/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe: Ameter_ADS1115.  电流计
* date：2021/8/19
*******************************************************************************
  Please connect to Port A,Measure current and display.
  请连接端口A,测量电流并显示到屏幕上
  Pay attention: EEPROM (0x51) has built-in calibration parameters when leaving the factory.
  Please do not write to the EEPROM, otherwise the calibration data will be overwritten and the measurement results will be inaccurate.
  注意: EEPROM (0x51)在出厂时具有内置的校准参数。请不要写入EEPROM，否则校准数据会被覆盖，测量结果会不准确。
*/

#include <M5StickC.h>
#include "M5_ADS1115.h"

Ammeter ammeter;

float page512_volt = 2000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw = 0;

int16_t hope = 0.0;

ammeterGain_t now_gain = PAG_512;

void setup(void) {
  M5.begin(); //Init M5StickC.  初始化M5StickC
  Wire.begin();

  ammeter.setMode(SINGLESHOT);
  ammeter.setRate(RATE_8);
  ammeter.setGain(PAG_512);
  hope = page512_volt / ammeter.resolution;
  // | PAG      | Max Input Voltage(V) |
  // | PAG_6144 |        128           |
  // | PAG_4096 |        64            |
  // | PAG_2048 |        32            |
  // | PAG_512  |        16            |
  // | PAG_256  |        8             |

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(1);  //Set font to 1 point font.  设置字体为1号字体

  M5.Lcd.setCursor(30, 150);  //Set the cursor at (30, 150).  将光标设置在(30, 150)
  M5.Lcd.printf("2A");

}

void loop(void) {
  M5.update();  //Check the status of the key.  检测按键的状态
  if (M5.BtnA.wasPressed()) {
    ammeter.setMode(SINGLESHOT);  //Set the mode.  设置模式
    ammeter.setRate(RATE_8);  //Set the rate.  设置速率
    ammeter.setGain(PAG_512);
    now_gain = PAG_512;
    hope = page512_volt / ammeter.resolution;

    for (uint8_t i = 0; i < 10; i++) {
      volt_raw_list[i] = 0;
    }
  }

  float current = ammeter.getCurrent();

  volt_raw_list[raw_now_ptr] = ammeter.adc_raw;
  raw_now_ptr = (raw_now_ptr == 9) ? 0 : (raw_now_ptr + 1);

  int count = 0;
  int total = 0;

  for (uint8_t i = 0; i < 10; i++) {
    if (volt_raw_list[i] == 0) {
      continue ;
    }
    total += volt_raw_list[i];
    count += 1;
  }

  if (count == 0) {
    adc_raw = 0;
  } else {
    adc_raw = total / count;
  }

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(2, 0);
  M5.Lcd.printf("Hope volt:");
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.printf("%.2f mAn", page512_volt);

  M5.Lcd.setCursor(2, 25);
  M5.Lcd.printf("Hope ADC:");
  M5.Lcd.setCursor(5, 35);
  M5.Lcd.printf("%d", hope);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(2, 50);
  M5.Lcd.printf("Cal volt:");
  M5.Lcd.setCursor(5, 60);
  M5.Lcd.printf("%.2f mA", current);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(2, 75);
  M5.Lcd.printf("Cal ADC:");
  M5.Lcd.setCursor(5, 85);
  M5.Lcd.printf("%.0f", adc_raw * ammeter.calibration_factor);

  M5.Lcd.setCursor(2,100);
  if (abs(adc_raw) <= hope * 1.005 && abs(adc_raw) >= hope * 0.995) {
    M5.Lcd.setTextColor(GREEN, BLACK);
  } else {
    M5.Lcd.setTextColor(RED, BLACK);
  }
  M5.Lcd.printf("RAW ADC:");
  M5.Lcd.setCursor(5,110);
  M5.Lcd.printf("%d", adc_raw);
}
