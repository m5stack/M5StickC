/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：EEPROM Read & Write.
* date：2021/7/30
*******************************************************************************
  The values stored in the EEPROM will remain in the EEPROM even after the M5Stick is disconnected.
  When a new program is uploaded to the M5Stick, the values stored in the EEPROM can still be called or modified by the new program.
  储存于EEPROM的数值即使在断开 M5Stick电源后仍会保存在EEPROM中
  当新程序上传到 M5Stick后，储存于EEPROM中的数值仍然可以被新的程序调用或者修改
*/

#include <M5StickC.h>
#include <EEPROM.h>

int addr = 0; //EEPROM Start number of an ADDRESS.  EEPROM地址起始编号
#define SIZE 32  //define the size of EEPROM(Byte).  定义EEPROM的大小(字节)

void setup() {
  M5.begin(); //Init M5StickC.  初始化 M5StickC
  M5.Lcd.setRotation(3);  //Rotate the screen.  旋转屏幕
  M5.Lcd.println("\n\nPlease open serial");
  if (!EEPROM.begin(SIZE)){  //Request storage of SIZE size(success return 1).  申请SIZE大小的存储(成功返回1)
    Serial.println("\nFailed to initialise EEPROM!"); //串口输出格式化字符串.  Serial output format string
    delay(1000000);
  }
  Serial.println("\nRead data from EEPROM. Values are:");
  for (int i = 0; i < SIZE; i++){
    Serial.printf("%d ",EEPROM.read(i)); //Reads data from 0 to SIZE in EEPROM.  读取EEPROM中从0到SIZE中的数据
  }
  Serial.println("\n\nPress BtnA to Write EEPROM");
}

void loop() {
  M5.update();  //Check button down state.  检测按键按下状态
  if(M5.BtnA.isPressed()){ //if the button is Pressed.  如果按键按下
    Serial.printf("\n%d Bytes datas written on EEPROM.\nValues are:\n",SIZE);
    for(int i=0;i<SIZE;i++){
      int val = random(256);  //Integer values to be stored in the EEPROM (EEPROM can store one byte per memory address, and can only store numbers from 0 to 255. Therefore, if you want to use EEPROM to store the numeric value read by the analog input pin, divide the numeric value by 4.
      //将要存储于EEPROM的整数数值(EEPROM每一个存储地址可以储存一个字节，只能存储0-255的数.故如果要使用EEPROM存储模拟输入引脚所读取到的数值需要将该数值除以4)
      EEPROM.write(addr, val);  //Writes the specified data to the specified address.  向指定地址写入指定数据
      Serial.printf("%d ",val);
      addr += 1;  //Go to the next storage address.  转入下一存储地址
    }
    //When the storage address sequence number reaches the end of the storage space of the EEPROM, return to.  当存储地址序列号达到EEPROM的存储空间结尾，返回到EEPROM开始地址
    addr = 0;
    Serial.println("\nnRead form EEPROM. Values are:");
    for(int i=0;i<SIZE;i++){
      Serial.printf("%d ",EEPROM.read(i));
    }
    Serial.println("\n-------------------------------------\n");
  }
  delay(150);
}