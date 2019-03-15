#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>


#include "axp192.h"
#include "Lcd_Driver.h"
#include "LCD_Config.h"
#include "IMU.h"
#include "rtc.h"
#include "esp32_rmt.h"
#include "DHT12.h"

#include <driver/i2s.h>


#define PIN_CLK  0
#define PIN_DATA 34
#define READ_LEN (2 * 1024)
uint8_t BUFFER[READ_LEN] = {0};

AXP192 axp192;
Rtc  rtc;
ESP32_RMT rem;
DHT12 dht12;  

int LED_RI = 9;
int LED_BUILTIN = 10;
int BUTTON_HOME = 37;
int BUTTON_PIN = 39;


int IO_1 = 0;
int IO_2 = 26;
int IO_3 = 36;



int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

int16_t gyroX = 0;
int16_t gyroY = 0;
int16_t gyroZ = 0;
uint16_t buffer;
void reader(void *pvParameters) {
  while (1){
	    i2s_read_bytes(I2S_NUM_0, (char*) BUFFER, READ_LEN, (100 / portTICK_RATE_MS));
      showSignal();
	    vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void i2sInit()
{
   i2s_config_t i2s_config = {
    //.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX| I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    //.sample_rate =  I2S_SAMPLE_RATE,              // The format of the signal using ADC_BUILT_IN
    .sample_rate =  44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    //.bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT, 
    //.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    //.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
     .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
      //.channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
    //.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
/*
      I2S_CHANNEL_FMT_RIGHT_LEFT = 0x00,
    I2S_CHANNEL_FMT_ALL_RIGHT,
    I2S_CHANNEL_FMT_ALL_LEFT,
    I2S_CHANNEL_FMT_ONLY_RIGHT,
    I2S_CHANNEL_FMT_ONLY_LEFT,*/
    //.communication_format = I2S_COMM_FORMAT_I2S_MSB,
    //I2S_COMM_FORMAT_PCM_SHORT
    .communication_format = I2S_COMM_FORMAT_I2S,
    //I2S_COMM_FORMAT_I2S 
    //.communication_format = I2S_COMM_FORMAT_PCM,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
   // .use_apll = false,
    //.tx_desc_auto_clear = false,
    //.fixed_mclk = 0
   };

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num  = I2S_PIN_NO_CHANGE;
	  pin_config.ws_io_num   = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
	  pin_config.data_in_num = PIN_DATA;
	
   //adc_set_i2s_data_source(ADC_I2S_DATA_SRC_IO_SIG);
   i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
   //i2s_set_adc_mode(ADC_UNIT_1, ADC_INPUT);
   //i2s_adc_enable(I2S_NUM_0);
   i2s_set_pin(I2S_NUM_0, &pin_config);
   //i2s_set_clk(I2S_NUM_0, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
   i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

uint16_t oldx[80];
uint16_t oldy[80];
void showSignal()
{
  int n;
  int x, y;
  uint16_t *adcBuffer = (uint16_t *)BUFFER;
  for (n = 0; n < 80; n++){
    x = n;
    y = map(adcBuffer[n], 0, 65535, 120, 140);
      Gui_DrawPoint(oldx[n], oldy[n],WHITE);
      Gui_DrawPoint(x,y,BLACK);
      oldx[n] = x;
      oldy[n] = y;
  }
}

void spm_test(){
  // Initialize the I2S peripheral
  i2sInit();
  // Create a task that will read the data
  xTaskCreatePinnedToCore(reader, "ADC_reader", 2048, NULL, 1, NULL, 1);
}

void wifi_test() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    

    Lcd_Asc(0,0,"wifi test");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    Lcd_Asc(0,20,"scan done");
    //M5.Lcd.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        Lcd_Asc(0,40,"no networks");
        Lcd_Asc(0,60,"found");
        //Lcd_Asc(0,80,"no networks found");
        //M5.Lcd.println("no networks found");
    } else {
        Serial.print(n);
        //M5.Lcd.print(n);
        Serial.println(" networks found");
        Lcd_Asc(0,40,"networks");
        Lcd_Asc(0,60,"found");
        /*
        //M5.Lcd.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            //M5.Lcd.print(i + 1);
            Serial.print(": ");
            //M5.Lcd.print(": ");
            Serial.print(WiFi.SSID(i));
           // M5.Lcd.print(WiFi.SSID(i));
            Serial.print(" (");
            //M5.Lcd.print(" (");
            Serial.print(WiFi.RSSI(i));
            //M5.Lcd.print(WiFi.RSSI(i));
            Serial.print(")");
            //M5.Lcd.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            //M5.Lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(5);
        }*/ 
    }
    Serial.println("");
    delay(1000);
    //M5.Lcd.println("");
}

void grove_test() {
  float tem = 0.0;
  tem = dht12.readTemperature();
  if(tem > 1.0){
  //Lcd_Asc(0,140,"T:");
  //dht12.readTemperature();
  Serial.println(dht12.readTemperature());
  //Lcd_PrintValueF(20, 140, dht12.readTemperature(),6);
  //Lcd_Asc(60,140,"C");
  Lcd_Asc(0,140,"I2C exist");
  }else{

   //Lcd_ClearRect(0, 140, 80, 20, WHITE);
   Lcd_Asc(0,140,"              ");
  }
  //Serial.println(dht12.readHumidity());

}

void sh200i_test() {

  //sh200i_init();
  unsigned char tempdata[1];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X, 1, tempdata);
  accX = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+1, 1, tempdata);
  accX |= tempdata[0] << 8;


  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+2, 1, tempdata);
  accY = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+3, 1, tempdata);
  accY |= tempdata[0] << 8;
    
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+4, 1, tempdata);
  accZ = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+5, 1, tempdata);
  accZ |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+6, 1, tempdata);
  gyroX = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+7, 1, tempdata);
  gyroX |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+8, 1, tempdata);
  gyroY = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+9, 1, tempdata);
  gyroY |= tempdata[0] << 8;

  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+10, 1, tempdata);
  gyroZ = tempdata[0];
  I2C_Read_NBytes(SH200I_ADDRESS, SH200I_OUTPUT_ACC_X+11, 1, tempdata);
  gyroZ |= tempdata[0] << 8;

#if 0
  Serial.printf("aX = %ld,aY = %ld,aZ = %ld,gX = %ld,gY = %ld,gZ = %ld\r\n", accX, accY, accZ,gyroX, gyroY, gyroZ);
#else
  Serial.printf("aX = %ld,aY = %ld,aZ = %ld,gX = %ld,gY = %ld,gZ = %ld\r\n", accX, accY, accZ,gyroX, gyroY, gyroZ);
  //Lcd_Asc(0,60,"sh200i ok!");
  Lcd_ClearRect(0,20,80,100,WHITE);
  Lcd_Asc(0,20,"a:");
  //Lcd_Asc(50,20,"Rt");
  //Lcd_Asc(40,20,"      ");
  //Lcd_Asc(0,40,"         ");
  Lcd_PrintValueI(40, 20, accX);
  Lcd_PrintValueI(0, 40, accY);
  Lcd_PrintValueI(40, 40, accZ);
  //Lcd_Asc(0,40,"Rt");
  //Lcd_Asc(60,40,"Rt");
  Lcd_Asc(0,60,"g:");
  Lcd_PrintValueI(40, 60, gyroX);
  Lcd_PrintValueI(0, 80, gyroY);
  Lcd_PrintValueI(40, 80, gyroZ);
  //Lcd_Asc(50,60,"Rt");
  //Lcd_Asc(0,80,"Rt");
  //Lcd_Asc(60,80,"Rt");
#endif
  //delay(1000);
}

void ir_test() {
  //!IR
  //rem.begin(LED_RI,1);
  rem.necSend(0x3000, 0xfd02);
  //Lcd_Asc(0,100,"ir ok!");
}

uint8_t Rt_second;
uint8_t Rt_old_second;
void rtc_test() {
    rtc.GetBm8563Time();
    Rt_second = rtc.Second;
    #if 0
    Serial.printf("time = %02d : %02d : %02d\n",rtc.Hour, rtc.Minute, rtc.Second);
    delay(1000);
    #else
    if(Rt_old_second != Rt_second){
     Lcd_Asc(0,100,"Rt");
     Lcd_Asc(17,100,(char *)rtc.DateString);
    }

    Rt_old_second = Rt_second;
    #endif
    //rtc.GetBm8563Time();
    //second_test_2 = rtc.Second;
    //Serial.printf("time = %02d : %02d : %02d\n",rtc.Hour, rtc.Minute, rtc.Second);
    //if(second_test_1 != second_test_2)
      //Lcd_Asc(0,80,"rtc ok!");
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  //Wire1.begin();
  Serial.begin(115200);

  //power
  axp192.Init();
   
  //lcd
  Lcd_Init();
  Lcd_pic(gImage_001);
  delay(1000);
  Lcd_Clear(WHITE);
  delay(50);
  Lcd_Clear(BLUE);
  delay(50);
  Lcd_Clear(RED);
  delay(50);
  Lcd_Clear(GREEN);
  delay(50);
  Lcd_Clear(YELLOW);
  delay(50);

  Lcd_Clear(WHITE);
  wifi_test();
  Lcd_Clear(WHITE);
  Lcd_Asc(8,0,"M5StickC");

  //!sh200i
  sh200i_init();
  //Lcd_Asc(0,20,"my name is");

  //!IR
  rem.begin(LED_RI,1);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(IO_1, OUTPUT);
  digitalWrite(IO_1, HIGH);

  pinMode(IO_2, OUTPUT);
  digitalWrite(IO_2, HIGH);

  pinMode(IO_3, OUTPUT);
  digitalWrite(IO_3, LOW);


  pinMode(BUTTON_HOME, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  spm_test();
}

bool test_led = 0;
bool led = true;
long loopTime, startTime = 0;
int led_count = 2000;
void loop() {
  // put your main code here, to run repeatedly:
  loopTime = millis();
  //startTime = loopTime;
#if 1
  if(digitalRead(BUTTON_HOME) == LOW){
    led_count++;
    
    //if(led_count >= 15);
    //led_count = 0;
    //while(digitalRead(BUTTON_HOME) == LOW);
  }

  if(digitalRead(BUTTON_PIN) == LOW){
    //Lcd_Asc(0,140,"Kpin ok!");
    led = !led;
    rem.necSend(0x3000, 0xfd02);
    while(digitalRead(BUTTON_PIN) == LOW);
  }
  digitalWrite(LED_BUILTIN, led);

  //io test
  digitalWrite(IO_1, test_led);
  digitalWrite(IO_2, test_led);
  digitalWrite(IO_3, test_led);

#endif
  //!rtc
  if(startTime < (loopTime - 1000)){
  test_led = !test_led;
  startTime = loopTime;
  rtc_test();
  sh200i_test();
  grove_test();
  /*
  uint16_t offset = (int)ADC_INPUT * 0x1000 + 0xFFF;
  Lcd_ClearRect(40, 120, 180,140, WHITE);
  Lcd_PrintValueI(40, 120, offset-buffer);
  Serial.printf("ADC reading: %d\n", offset-buffer);
  if(offset-buffer >= 100){
  Lcd_Asc(0,120,"mic: exist");
  }else
  {
    Lcd_Asc(0,120,"           ");
  }
  */
  }

  axp192.ScreenBreath(led_count / 200);
    //delay(1000);
}
