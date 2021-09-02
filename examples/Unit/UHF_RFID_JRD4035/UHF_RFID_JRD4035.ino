/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/uhf_rfid
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/uhf_rfid
*
* describe: uhf_rfid.
* date：2021/9/1
*******************************************************************************
*/
#include <M5StickC.h>
#include "RFID_command.h"

UHF_RFID RFID;

String comd = " ";
CardpropertiesInfo card;
ManyInfo cards;
SelectInfo Select;
CardInformationInfo Cardinformation;
QueryInfo Query;
ReadInfo Read;
TestInfo Test;


void setup()
{
  M5.begin();

  RFID._debug = 0;
  Serial2.begin(115200, SERIAL_8N1, 33, 32);
  if (RFID._debug == 1)Serial.begin(115200, SERIAL_8N1, 21, 22);

// UHF_RFID set UHF_RFID设置
  RFID.Set_transmission_Power(2600);
  RFID.Set_the_Select_mode();
  RFID.Delay(100);
  RFID.Readcallback();
  RFID.clean_data();
  
// Prompted to connect to UHF_RFID 提示连接UHF_RFID
  Serial.println("Please connect UHF_RFID to Port C");
  
// Determined whether to connect to UHF_RFID 判断是否连接UHF_RFID
  String soft_version;
  soft_version = RFID.Query_software_version();
  while(soft_version.indexOf("V2.3.5") == -1)
  {
    RFID.clean_data();
    RFID.Delay(150);
    RFID.Delay(150);
    soft_version = RFID.Query_software_version();
  }

// The prompt will be RFID card close 提示将RFID卡靠近
  Serial.println("Please approach the RFID card you need to use");
  
}

void loop()
{

//  A read/write operation specifies a particular card 读写操作需指定某一张卡
//  comd = RFID.Set_the_select_parameter_directive("30751FEB705C5904E3D50D70");
//  Serial.println(comd);
//  RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Query the card information once 查询一次卡的信息例子
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 card = RFID.A_single_poll_of_instructions();
 if (card._ERROR.length() != 0){
    Serial.println(card._ERROR);
 }else{
    if(card._EPC.length() == 24){
      Serial.println("RSSI :" + card._RSSI);
      Serial.println("PC :" + card._PC);
      Serial.println("EPC :" + card._EPC);
      Serial.println("CRC :" + card._CRC);
      Serial.println(" ");
    }
  }
  RFID.clean_data(); //Empty the data after using it 使用完数据后要将数据清空


/*Other feature usage examples 其他功能使用例子*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Read multiple RFID cards at once 一次读取多张RFID卡
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  cards = RFID.Multiple_polling_instructions(6);
 for (size_t i = 0; i < cards.len; i++)
 {
   if(cards.card[i]._EPC.length() == 24)
     {
        Serial.println("RSSI :" + cards.card[i]._RSSI);
        Serial.println("PC :" + cards.card[i]._PC);
        Serial.println("EPC :" + cards.card[i]._EPC);
        Serial.println("CRC :" + cards.card[i]._CRC);
      }
 }
 Serial.println(" ");  
 RFID.clean_data();
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the SELECT parameter 用于获取Select参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Select = RFID.Get_the_select_parameter();
//  if(Select.Mask.length() != 0)
//  {
//    Serial.println("Mask :" + Select.Mask);
//    Serial.println("SelParam :" + Select.SelParam);
//    Serial.println("Ptr :" + Select.Ptr);
//    Serial.println("MaskLen :" + Select.MaskLen);
//    Serial.println("Truncate :" + Select.Truncate);
//    Serial.println(" ");
//  }
//    RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to change the PSF bit of the NXP G2X label 用于改变 NXP G2X 标签的 PSF 位
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Cardinformation = RFID.NXP_Change_EAS(0x00000000);
//  if(Cardinformation._UL.length() != 0)
//  {
//    Serial.println("UL :" + Cardinformation._UL);
//    Serial.println("PC :" + Cardinformation._PC);
//    Serial.println("EPC :" + Cardinformation._EPC);
//    Serial.println("Parameter :" + Cardinformation._Parameter);
//    Serial.println("ErrorCode :" + Cardinformation._ErrorCode);
//    Serial.println("Error :" + Cardinformation._Error);
//    Serial.println("Data :" + Cardinformation._Data);
//    Serial.println("Successful :" + Cardinformation._Successful);
//    Serial.println(" ");
//   }
//    RFID.clean_data();
  

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于获取Query参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Query = RFID.Get_the_Query_parameter();
//  if(Query.QueryParameter.length() != 0)
//  {
//    Serial.println("QueryParameter :" + Query.QueryParameter);
//    Serial.println("DR :" + Query.DR);
//    Serial.println("M :" + Query.M);
//    Serial.println("TRext :" + Query.TRext);
//    Serial.println("Sel :" + Query.Sel);
//    Serial.println("Session :" + Query.Session);
//    Serial.println("Targetta :" + Query.Target);
//    Serial.println("Q :" + Query.Q);
//    Serial.println(" ");
//  }
//  RFID.clean_data();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于读取接收解调器参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Read = RFID.Read_receive_demodulator_parameters();
//  if(Read.Mixer_G.length()!= 0)
//  {
//    Serial.println("Mixer_G :" + Read.Mixer_G);
//    Serial.println("IF_G :" + Read.IF_G);
//    Serial.println("Thrd :" + Read.Thrd);
//    Serial.println(" ");
//  }
//  RFID.clean_data();
}
