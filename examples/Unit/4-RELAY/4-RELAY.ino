/*
    Description: Control 4 relays and demonstrate the asynchronous control relay LED
*/

#include <M5StickC.h>

/*-----------------------------------------------------------------------------*/
// |RELAY control reg          | 0x10
// |-----------------------------------------------------------------------------
// |Relay_ctrl_mode_reg[0]     | R/W | System control
//                                   | 7 | 6 | 5 | 4 | 3 | 2 | 1 |     0     |
//                                   | R | R | R | R | R | R | R | Sync Mode |
//                                   | -Sync Mode:0 LED&Relay Async
//                                   | -Sync Mode:1 LED&Relay Sync
//---------------------------------------------------------------------------------
// |Relay_ctrl_mode_reg[1]     | R/W | Relay & LED control
//                             |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//                             | LED1| LED2| LED3| LED4| RLY1| RLY2| RLY3| RLY4|
//
/*-------------------------------------------------------------------------------*/

void WriteRelayReg( int regAddr, int data )
{
  Wire.beginTransmission(0x26);
  Wire.write(regAddr);
  Wire.write(data);
  Wire.endTransmission();
  Serial.printf("[ W ] %02X : %02X. \r\n", regAddr, data);
}

int readRelayReg(int regAddr)
{
  Wire.beginTransmission(0x26);
  Wire.write(regAddr);
  Wire.endTransmission();
  Wire.requestFrom(0x26, 1);
  int data = Wire.read() & 0x00ff;
  Serial.printf("[ R ] %02X : %02X. \r\n", regAddr, data);
  return data;
}

void WriteRelayNumber( int number, int state )
{
  int StateFromDevice = readRelayReg(0x11);
  if ( state == 0 )
  {
    StateFromDevice &= ~( 0x01 << number );
  }
  else
  {
    StateFromDevice |= ( 0x01 << number );
  }
  WriteRelayReg(0x11, StateFromDevice);
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  Wire.begin(32, 33);
  M5.Lcd.setCursor(50, 5);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.print("4-RELAY UNIT");
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.print("Independent Switch:");
  M5.Lcd.setCursor(0, 45);
  M5.Lcd.print("LED Sync/Async:");
  readRelayReg(0x10);
  readRelayReg(0x11);
  WriteRelayReg(0x10, 1);
  WriteRelayReg(0x11, 0);
  //WriteRelayNumber(0,0);
}

int count_i = 0;
bool flag_led, flag_relay = false;


void loop() {
  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.fillRect(120, 25, 30, 10, TFT_BLACK);
    M5.Lcd.setCursor(120, 25);
    if (count_i < 4)
    {
      M5.Lcd.printf("%d ON", count_i);
      WriteRelayReg(0x11, (0x01 << count_i));
    }
    else if (count_i == 4)
    {
      M5.Lcd.print("ON");
      for (int i = 0; i < 4; i++)
      {
        WriteRelayNumber(i, 1);
      }
    }
    else if (count_i == 5)
    {
      M5.Lcd.print("OFF");
      for (int i = 0; i < 4; i++)
      {
        WriteRelayNumber(i, 0);
      }
    }
    count_i++;
    if ( count_i >= 6 ) count_i = 0;
  }
  if (M5.BtnB.wasPressed())
  {
    M5.Lcd.fillRect(100, 45, 30, 10, TFT_BLACK);
     M5.Lcd.setCursor(100, 45);
    if (!flag_led) {
      M5.Lcd.print("Async");
      WriteRelayReg(0x10, 0);
    } else {
      M5.Lcd.print("Sync");
      WriteRelayReg(0x10, 1);
    }
    flag_led = !flag_led;
  }

  M5.update();
}
