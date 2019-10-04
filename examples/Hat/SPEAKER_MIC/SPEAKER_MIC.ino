#include <M5StickC.h>
#include <driver/i2s.h>

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )

#define PIN_CLK       (0)           // I2S Clock PIN
#define PIN_DATA      (34)          // I2S Data PIN
#define SAMPLING_RATE (16384)       // Sampling Rate(44100, 22050, 16384, more...)
#define BUFFER_LEN    (1024)        // Buffer Size
#define STORAGE_LEN   (102400)      // Storage Size(MAX 100K)

#define WAVE_EXPORT   (0)           // Export WAVE file


uint8_t soundBuffer[BUFFER_LEN];    // DMA buffer
uint8_t soundStorage[STORAGE_LEN];  // Storage

bool recFlag = false;               // record status
int recPos = 0;                     // record position

// Record
void i2sRecord() {
  // Rec setting
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate          = SAMPLING_RATE,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format       = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 2,
    .dma_buf_len          = BUFFER_LEN,
    .use_apll             = false,
    .tx_desc_auto_clear   = true,
    .fixed_mclk           = 0,
  };

  // PIN setting
  i2s_pin_config_t pin_config;
  pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
  pin_config.ws_io_num    = PIN_CLK;
  pin_config.data_out_num = I2S_PIN_NO_CHANGE;
  pin_config.data_in_num  = PIN_DATA;

  // Config set
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, SAMPLING_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

  // Record start
  recFlag = true;
  xTaskCreatePinnedToCore(i2sRecordTask, "i2sRecordTask", 2048, NULL, 1, NULL, 1);
}

// Play
void i2sPlay(){
  // Play setting
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate          = SAMPLING_RATE,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format       = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 2,
    .dma_buf_len          = BUFFER_LEN,
    .use_apll             = false,
    .tx_desc_auto_clear   = true,
    .fixed_mclk           = 0,
  };

  // Config set
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, NULL);
  i2s_zero_dma_buffer(I2S_NUM_0);

  // Play
  size_t transBytes;
  size_t playPos = 0;
  while( playPos < recPos ){
    for( int i = 0 ; i < BUFFER_LEN ; i+=2 ){
      soundBuffer[i] = 0;                         // 
      soundBuffer[i+1] = soundStorage[playPos];   // Only 8bit data
      playPos++;
    }

    // Data send
    i2s_write(I2S_NUM_0, (char*)soundBuffer, BUFFER_LEN, &transBytes, (100 / portTICK_RATE_MS));
    Serial.println(playPos);
  }

  // Finish
  i2s_zero_dma_buffer(I2S_NUM_0);
  i2s_driver_uninstall(I2S_NUM_0);
}

// Record
void i2sRecordTask(void* arg)
{
  // Init
  recPos = 0;
  memset(soundStorage, 0, sizeof(soundStorage));

  // Record loop
  while (recFlag) {
    size_t transBytes;

    // Read
    i2s_read(I2S_NUM_0, (char*)soundBuffer, BUFFER_LEN, &transBytes, (100 / portTICK_RATE_MS));

    // int16_t(12bit) to uint8_t
    for (int i = 0 ; i < transBytes ; i += 2 ) {
      if ( recPos < STORAGE_LEN ) {
        int16_t* val = (int16_t*)&soundBuffer[i];
        soundStorage[recPos] = ( *val + 32768 ) / 256;
        recPos++;
      }
    }
    Serial.printf("transBytes = %d, STORAGE_LEN=%d, recPos=%d\n", transBytes, STORAGE_LEN, recPos);
    vTaskDelay(1 / portTICK_RATE_MS);
  }

  i2s_driver_uninstall(I2S_NUM_0);

  // Task delete
  vTaskDelete(NULL);
}

void setup() {
  // Remove noise
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);

  // Init
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.println("Sound Recorder");
  M5.Lcd.println("BtnA Record");
  M5.Lcd.println("BtnB Play");
}

void loop() {
  M5.update();

  if ( !recFlag && M5.BtnA.wasPressed() ) {
    // Record start
    M5.Lcd.setCursor(0, 24);
    M5.Lcd.println("REC...");
    Serial.println("Record Start");
    i2sRecord();
  } else if ( recFlag && M5.BtnA.isReleased() ) {
    // Record stop
    M5.Lcd.setCursor(0, 24);
    M5.Lcd.println("      ");
    recFlag = false;
    delay(100); // Wait

    Serial.println("Record Stop");

    // Export WAVE file
    if ( WAVE_EXPORT ) {
      Serial.printf("52494646");                        // "RIFF"
      Serial.printf("%08lx", htonl(recPos + 44 - 8));   // Chunk size:DATA Size+44-8
      Serial.printf("57415645");                        // "WAVE"
      Serial.printf("666D7420");                        // "fmt "
      Serial.printf("10000000");                        // Chunk size
      Serial.printf("0100");                            // Format code
      Serial.printf("0100");                            // Channels
      Serial.printf("%08lx", htonl(SAMPLING_RATE));     // Sampling rate
      Serial.printf("%08lx", htonl(SAMPLING_RATE));     // Data rate
      Serial.printf("0200");                            // Data block size (bytes)
      Serial.printf("0800");                            // Bits per sample
      Serial.printf("64617461");                        // "data"
      Serial.printf("%08lx", htonl(recPos));            // Data size

      for (int n = 0; n <= recPos; n++) {
        Serial.printf("%02x", soundStorage[n]);
      }
      Serial.printf("\n");
    }
  } else if ( M5.BtnB.wasReleased() ) {
    // Play start
    M5.Lcd.setCursor(0, 24);
    M5.Lcd.println("Play...");
    Serial.println("Play Start");
    i2sPlay();
    M5.Lcd.setCursor(0, 24);
    M5.Lcd.println("       ");
    Serial.println("Play Stop");
  }

  delay(10);
}
