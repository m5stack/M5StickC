#include <M5StickC.h>
#include <driver/i2s.h>

#define PIN_CLK  0
#define PIN_DATA 34
#define READ_LEN (2 * 1024)
uint8_t BUFFER[READ_LEN] = {0};

uint16_t oldx[160];
uint16_t oldy[160];
uint16_t *adcBuffer = NULL;

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
   pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
   pin_config.ws_io_num    = PIN_CLK;
   pin_config.data_out_num = I2S_PIN_NO_CHANGE;
   pin_config.data_in_num  = PIN_DATA;
	
   
   i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
   i2s_set_pin(I2S_NUM_0, &pin_config);
   i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}



void mic_record_task (void* arg)
{   
    
  while(1){
    i2s_read_bytes(I2S_NUM_0, (char*) BUFFER, READ_LEN, (100 / portTICK_RATE_MS));
    adcBuffer = (uint16_t *)BUFFER;
    showSignal();
    vTaskDelay(100 / portTICK_RATE_MS);
  }

}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.println("mic test");

  i2sInit();
  xTaskCreatePinnedToCore(mic_record_task, "mic_record_task", 2048, NULL, 1, NULL, 1);
}


void showSignal(){
  int x, y;
  for (int n = 0; n < 160; n++){
    x = n;
    y = map(adcBuffer[n], 0, 65535, 10, 70);
    M5.Lcd.drawPixel(oldx[n], oldy[n],WHITE);
    M5.Lcd.drawPixel(x,y,BLACK);
    oldx[n] = x;
    oldy[n] = y;
  }
}

void loop() {
  
}
