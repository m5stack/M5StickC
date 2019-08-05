#include <M5StickC.h>
#include <WiFi.h>
#include <driver/i2s.h>

#include "esp32_rmt.h"
#include "DHT12.h"


extern const unsigned char gImage_logo[];
long loopTime, startTime = 0;
bool led = true;
bool test_led = 0;
ESP32_RMT rem;
DHT12 dht12;
uint8_t led_count = 15;

int IO_1 = 0;
int IO_2 = 26;
int IO_3 = 36;

#define PIN_CLK  0
#define PIN_DATA 34
#define READ_LEN (2 * 80)
#define SHOW_LEN (80)
uint8_t BUFFER[READ_LEN] = {0};

uint16_t *adcBuffer = NULL;
uint16_t oldx[SHOW_LEN];
uint16_t oldy[SHOW_LEN];

int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

int16_t gyroX = 0;
int16_t gyroY = 0;
int16_t gyroZ = 0;

void wifi_test() {
    M5.Lcd.setTextColor(BLACK, WHITE);
    M5.Lcd.setCursor(7, 0, 2);
    M5.Lcd.println("WIFI TEST");
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    M5.Lcd.setCursor(7, 20, 2);
    M5.Lcd.println("scan done");
	
	if (n == 0) {
      M5.Lcd.setTextColor(RED, WHITE);
    } else {   
      M5.Lcd.setTextColor(GREEN, WHITE);
    }
    M5.Lcd.setCursor(5, 60, 4);
    M5.Lcd.printf("%d AP", n);
}

void sh200i_test() {

  M5.IMU.getGyroAdc(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelAdc(&accX,&accY,&accZ);
  
  M5.Lcd.setTextColor(GREEN, WHITE);
  M5.Lcd.setCursor(20, 1, 1);
  M5.Lcd.println("mg  o/s");
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("x  %d   %.1f    ", (int)(((float) gyroX) * M5.IMU.gRes), ((float) accX) * M5.IMU.aRes);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("y  %d   %.1f    ", (int)(((float) gyroY) * M5.IMU.gRes), ((float) accY) * M5.IMU.aRes);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("z  %d   %.1f    ", (int)(((float) gyroZ) * M5.IMU.gRes), ((float) accZ) * M5.IMU.aRes);
 
}
float accX_f = 0;
float accY_f = 0;
float accZ_f = 0;

float gyroX_f = 0;
float gyroY_f = 0;
float gyroZ_f = 0;
void mpu6886_test(){
  M5.MPU6886.getGyroData(&gyroX_f,&gyroY_f,&gyroZ_f);
  M5.MPU6886.getAccelData(&accX_f,&accY_f,&accZ_f);
  
  M5.Lcd.setTextColor(GREEN, WHITE);
  M5.Lcd.setCursor(20, 1, 1);
  M5.Lcd.println("mg  o/s");
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("x  %.1f  %.1f", gyroX_f, accX_f);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("y  %.1f  %.1f", gyroY_f, accY_f);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("z  %.1f  %.1f", gyroZ_f, accZ_f);
}

void rtc_test(){
  
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Rtc.GetBm8563Time();
  M5.Lcd.setCursor(0, 50, 1);
  M5.Lcd.printf("Data %02d:%02d:%02d\n",M5.Rtc.Hour, M5.Rtc.Minute, M5.Rtc.Second);

}


void ir_test() {
  rem.necSend(0x3000, 0xfd02);
}

void showSignal(){
  int x, y;
  for (int n = 0; n < 80; n++){
    x = n;
    y = map(adcBuffer[n], 0, 65535, 70, 100);
    //y = map(adcBuffer[READ_LEN*n/160], 0, 65535, 70, 159);
    M5.Lcd.drawPixel(oldx[n], oldy[n],WHITE);
    M5.Lcd.drawPixel(x,y,BLACK);
    oldx[n] = x;
    oldy[n] = y;
  }
}
void mic_record_task (void* arg)
{   
    
  while(1){
    //i2s_start(I2S_NUM_0);
    i2s_read_bytes(I2S_NUM_0, (char*) BUFFER, READ_LEN, (100 / portTICK_RATE_MS));
    adcBuffer = (uint16_t *)BUFFER;
    //showSignal();
    vTaskDelay(100 / portTICK_RATE_MS);
    //i2s_stop(I2S_NUM_0);
  }

}
void i2sInit()
{
   i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate =  44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
   };

  i2s_pin_config_t pin_config;
    pin_config.bck_io_num  = I2S_PIN_NO_CHANGE;
	  pin_config.ws_io_num   = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
	  pin_config.data_in_num = PIN_DATA;
	
   
   i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
   i2s_set_pin(I2S_NUM_0, &pin_config);
   i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void spm_test(){
  // Initialize the I2S peripheral
  i2sInit();
  // Create a task that will read the data
  xTaskCreatePinnedToCore(mic_record_task, "mic_record_task", 2048 * 2, NULL, 1, NULL, 1);
}


void grove_test() {
  
  float tem = 0.0;
  tem = dht12.readTemperature();
  M5.Lcd.setTextColor(BLUE, WHITE);
  if(tem > 1.0){
    M5.Lcd.setCursor(10, 120, 2);
    M5.Lcd.printf("I2C exist");
  }else{
    M5.Lcd.fillRect(0, 120, 80, 160, WHITE);
  }
}

int imu_flag = 0;
int imu_type = 0;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(32,33);
  
  //!Logo
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.drawBitmap(0, 0, 80, 160,(uint16_t *)gImage_logo);
  delay(1000);

  //!Screen
  M5.Lcd.fillScreen(BLUE);
  delay(50);
  M5.Lcd.fillScreen(RED);
  delay(50);
  M5.Lcd.fillScreen(YELLOW);
  delay(50);
  M5.Lcd.fillScreen(ORANGE);
  delay(50);
  M5.Lcd.fillScreen(WHITE);
  delay(50);

  //!Wifi 
  wifi_test();
  delay(1000);
  M5.Lcd.fillScreen(WHITE);

  //!SH200I
  imu_flag = M5.IMU.Init();
  if(imu_flag != 0){
    imu_flag = M5.MPU6886.Init();
    if(imu_flag == 0)
      imu_type = 1;
    else
      imu_type = -1;
  }else{
    imu_type = 0;
  }
  Serial.printf("imu_flag = %d\n",imu_type);
  

  //!IR LED
  rem.begin(M5_IR,true);
  pinMode(M5_LED, OUTPUT);
  digitalWrite(M5_LED, LOW);
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
  pinMode(IO_1, OUTPUT);
  digitalWrite(IO_1, HIGH);

  pinMode(IO_2, OUTPUT);
  digitalWrite(IO_2, HIGH);

  pinMode(IO_3, OUTPUT);
  digitalWrite(IO_3, LOW);

  //!mic
  spm_test();

  //Wire.begin(32,33);
}
long sleep_count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  loopTime = millis();
  if(startTime < (loopTime - 500)){
    if(M5.Axp.GetWarningLeve()){
      sleep_count++;
      if(sleep_count >= 1){
      M5.Lcd.fillScreen(WHITE);
      M5.Lcd.setCursor(0, 20, 1);
      M5.Lcd.setTextColor(RED, WHITE);
      M5.Lcd.printf("Warning: low battery");
      if(sleep_count >= 10){
        sleep_count = 0;
        M5.Axp.SetSleep();
      }
      }
    }else{
      M5.Lcd.fillScreen(WHITE);
      test_led  = !test_led;
      grove_test();
      if(imu_type == 0)
        sh200i_test();
      else if(imu_type == 1)
        mpu6886_test();
      rtc_test();

      showSignal();
    }
    startTime = loopTime;
  }

  //io test
  digitalWrite(IO_1, test_led);
  digitalWrite(IO_2, test_led);
  digitalWrite(IO_3, test_led);


  if(digitalRead(M5_BUTTON_RST) == LOW){
    led = !led;
    rem.necSend(0x3000, 0xfd02);
    while(digitalRead(M5_BUTTON_RST) == LOW);
  }
  digitalWrite(M5_LED, led);


  if(digitalRead(M5_BUTTON_HOME) == LOW){
    led_count++;
    if(led_count >= 16)
      led_count = 7;
    while(digitalRead(M5_BUTTON_HOME) == LOW);
    M5.Axp.ScreenBreath(led_count);
  }

  //showSignal();

}
