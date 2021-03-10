/*
  neo pixel test

   hardwware:  M5StickC
   Pressed ButtonA :set base
   Pressed ButtonB :set tag

   Distance unit: meter
*/
#include <M5StickC.h>

String DATA = " "; 
int UWB_MODE = 2;   //Used to set UWB mode

int UWB_T_UI_NUMBER_2 = 0; // flag bit 标志位
int UWB_T_UI_NUMBER_1 = 0;
int UWB_T_NUMBER = 0;
int UWB_B_NUMBER = 0;

hw_timer_t *timer = NULL;
int timer_flag = 0;
int base_flag = 0;
uint32_t timer_data = 0;


void UWB_readString(void);
void UWB_clean(void);

static void IRAM_ATTR Timer0_CallBack(void);

void UWB_Timer()
{
  timer = timerBegin(0, 80, true);              //Timer setting 定时器设置
  timerAttachInterrupt(timer, Timer0_CallBack, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

static void IRAM_ATTR Timer0_CallBack(void)      //Timer function 定时器函数
{
  if (timer_flag == 1)
  {
    timer_data++;
    if (timer_data == 4294967280)
    {
      timer_data = 1;
    }
  }
  else
  {
    timer_data = 0;
  }
}

void UWB_readString()
{
 switch (UWB_MODE)
  {
    case 0:
      if (Serial2.available())
      {
        delay(20);
        UWB_T_NUMBER = (Serial2.available() / 11);      //Count the number of Base stations  计算基站数目
        delay(20);
        if (UWB_T_NUMBER != UWB_T_UI_NUMBER_1 || UWB_T_UI_NUMBER_2 == 0)    //Changing the UI display  更改UI显示
        {
          UWB_ui_display();
          UWB_T_UI_NUMBER_1 = UWB_T_NUMBER;
          UWB_T_UI_NUMBER_2 = 1;
        }
        DATA = Serial2.readString();
        delay(2);
        timer_flag = 0;
        timer_data = 1;
        break;
      }
      else
      {
        timer_flag = 1;
      }
      if (timer_data == 0 || timer_data > 5)    //Count the number of Base stations  提示与基站0断连（测试）
      {
        if(timer_data==6){
          // M5.Lcd.fillRect(210, 50, 50, 30, RED);
//          M5.Lcd.fillRect(45, 62, 30, 12, BLACK);//
          }
        DATA = "  0 2F   ";
        timer_flag = 0;
      }
      break;
    case 1:
      if (timer_data == 0 || timer_data > 70)   //Indicates successful or lost connection with label  提示与标签连接成功或丢失断连
      {
        if (Serial2.available())
        {
          delay(2);
          DATA = Serial2.readString();
          DATA = "Set up OK!";
          timer_data = 1;
          timer_flag = 1;
          break;
        }
        else if (timer_data > 0 && Serial2.available() == 0 )
        {
          DATA = "Can't find!";
          timer_flag = 0;
          break;
        }
      }
      break;
  }
}

void UWB_Keyscan() {
  if (M5.BtnA.wasReleased())
  {
    UWB_MODE = 0;
    UWB_setupmode();
    UWB_clean();
    UWB_ui_display();
    UWB_T_UI_NUMBER_2 = 0;
  }
  if (M5.BtnB.wasReleased())
  {
    UWB_MODE = 1;
    if (UWB_B_NUMBER == 4)
    {
      UWB_B_NUMBER = 0;
    }
    UWB_setupmode();
    UWB_clean();
    UWB_ui_display();
    UWB_B_NUMBER++;
  }
  
}


int UWB_setupmode()       //AT command
{
  switch (UWB_MODE) {
    case 0:
      for (int b = 0; b < 2; b++)
      { //Repeat twice to stabilize the connection
        delay(40);
        Serial2.write("AT+anchor_tag=0\r\n");     //Set up the label 设置标签
        delay(40);
        Serial2.write("AT+interval=5\r\n");        //Set the calculation precision, the larger the response is, the slower it will be
        delay(40);                                     //设置计算精度，越大响应越慢
        Serial2.write("AT+switchdis=1\r\n");        //Began to distance 开始测距
        delay(40);
        if (b == 0) {
          Serial2.write("AT+RST\r\n");            //RESET 复位
        }
      }
      UWB_clean();
      break;
    case 1:
      for (int b = 0; b < 2; b++)
      {
        delay(40);
        Serial2.write("AT+anchor_tag=1,");      //Set the base station 设置基站
        Serial2.print(UWB_B_NUMBER);            //UWB_B_NUMBER is base station ID0~ID3
        Serial2.write("\r\n");
        delay(1);
        delay(40);
        if (b == 0) {
          Serial2.write("AT+RST\r\n");
        }
      }
      UWB_clean();
      break;
  }
}

void UWB_display()
{
  switch (UWB_MODE)
  {
    case 0:
      if (UWB_T_NUMBER > 0 && UWB_T_NUMBER < 5)
      {
        int c = UWB_T_NUMBER;
        int b = 4 - UWB_T_NUMBER;
        while (c > 0)
        {
          c--;
          M5.Lcd.drawString(DATA.substring(2  + c * 11, 3 + c * 11), 20, 60 + c * 20, 2 );//Label the serial number 标签序号
          M5.Lcd.drawString(DATA.substring(4  + c * 11, 8  + c * 11), 45, 60 + c * 20, 2); //Distance 距离
        }
        while (b > 0)
        {
          b--;
          M5.Lcd.fillRect(10, 122 - b * 20, 20, 12, BLACK);//
          M5.Lcd.fillRect(45, 122 - b * 20, 30, 12, BLACK);//
        }
      }
      break;
    case 1:
      if (UWB_B_NUMBER == 1)
      {
        M5.Lcd.drawString(DATA, 8, 90, 2);//Prompt information  (Base station 0 only) 提示信息 (仅限基站0)
      }
      break;
  }
}

 //UI display
 void UWB_ui_display() 
 {
   switch (UWB_MODE)
   {
     case 0:
       M5.Lcd.drawString("Tag ", 20, 20, 4);
       if (UWB_T_NUMBER > 0 && UWB_T_NUMBER < 5)   //Tag mode UI display  标签模式UI显示
       {
         int c = UWB_T_NUMBER;
         int b = 4 - UWB_T_NUMBER;
         while (c > 0 )
         {
           c--;
           M5.Lcd.drawString("B", 10, 60 + c * 20, 2 );
         }

         while (b > 0)
         {
           b--;
          M5.Lcd.fillRect(10, 122 - b * 20, 20, 12, BLACK);//
          M5.Lcd.fillRect(45, 122 - b * 20, 30, 12, BLACK);//
         }
       }
       break;
     case 1:                                              //Base station mode UI display  基站模式UI显示
      M5.Lcd.drawString("Base ", 10, 20, 4);
       M5.Lcd.drawString(String(UWB_B_NUMBER),35, 45, 4 );//UWB_B_NUMBER
       if (UWB_B_NUMBER == 0)
       {
         M5.Lcd.drawString("Waiting...", 12, 90, 2 );
       }
       else
       {
         M5.Lcd.fillRect(0, 90, 80, 15, BLACK);
       }

       break;
   }
 }

 void setup()
{
  M5.begin();
  Serial2.begin(115200, SERIAL_8N1, 33, 32);//一般都是16.17
  delay(100);
  UWB_Timer();

  M5.Lcd.drawString("UWB ", 30, 0, 2); //UI
  M5.Lcd.drawString("Tag ", 30, 140, 2);
  M5.Lcd.drawString("Base ", 50, 60, 2);

}

void loop()
{
  M5.update();
  UWB_Keyscan();
  UWB_readString();
  UWB_display();
}

void UWB_clean()
{
   if (Serial2.available())
  {
    delay(3);
    DATA = Serial2.readString();
  }
  DATA = "";
  timer_flag = 0;
  M5.Lcd.fillRect(0,20,80,140,BLACK);
}
