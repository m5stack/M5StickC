#include <M5StickC.h>
#include <math.h>
#include <Wire.h>
#include <driver/rmt.h>
#include <driver/i2s.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "esp_pm.h"
#include "fft.h"
#include "DHT12.h"
#include "WiFi.h"

extern const unsigned char Logodata[25600];
extern const unsigned char error_48[4608];
extern const unsigned char icon_ir[4608];
extern const unsigned char icon_ble[4608];
extern const unsigned char icon_wifi[4608];
extern const unsigned char icon_ble_disconnect[4608];
extern const unsigned char ImageData[768];
extern const unsigned char stick1[];
extern const unsigned char stick2[];
extern const unsigned char stick3[];
extern const unsigned char stick4[];
extern const unsigned char stick5[];
extern const unsigned char stick6[];
extern const unsigned char stick7[];
extern const unsigned char stick8[];
extern const unsigned char stick9[];
extern const unsigned char stick10[];
extern const unsigned char stick11[];
extern const unsigned char stick12[];
extern const unsigned char stick13[];
extern const unsigned char stick14[];
extern const unsigned char stick15[];
extern const unsigned char stick16[];
extern const unsigned char stick17[];
extern const unsigned char stick18[];
const unsigned char* Animationptr[18] = {
  stick1,stick2,stick3,stick4,stick5,
  stick6,stick7,stick8,stick9,stick10,
  stick11,stick12,stick13,stick14,stick15,
  stick16,stick17,stick18
};

typedef struct point_3d
{
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct line_3d
{
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct point_2d
{
    double x;
    double y;
} point_2d_t;


hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile uint8_t TimerCount = 0;

bool TestMode = false;

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

void IRAM_ATTR onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);
    digitalWrite(10, TimerCount % 2);
    TimerCount++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void ErrorMeg(uint8_t code, const char *str)
{
    printf("ErrorMeg: code=%02X str=%s\n", code, str);
    Disbuff.fillRect(0, 0, 160, 80, BLACK);
    Disbuff.pushImage(0, 16, 48, 48, (uint16_t *)error_48);
    Disbuff.setCursor(100, 10);
    Disbuff.setTextFont(2);
    Disbuff.printf("%02X", code);
    Disbuff.drawString("ERROR", 55, 10, 2);
    Disbuff.drawString("-----------------", 55, 30, 1);
    Disbuff.drawString(str, 55, 45, 1);
    Disbuff.drawString("check Hardware ", 55, 60, 1);
    Disbuff.pushSprite(0, 0);
    while (1)
        ;
}

void ErrorMeg(uint8_t code, const char *str1, const char *str2)
{
    printf("ErrorMeg: code=%02X str1=%s str2=%s\n", code, str1, str2);
    Disbuff.fillRect(0, 0, 160, 80, BLACK);
    Disbuff.pushImage(0, 16, 48, 48, (uint16_t *)error_48);
    Disbuff.setCursor(100, 10);
    Disbuff.setTextFont(2);
    Disbuff.printf("%02X", code);
    Disbuff.drawString("ERROR", 55, 10, 2);
    Disbuff.drawString(str1, 55, 30, 1);
    Disbuff.drawString(str2, 55, 45, 1);
    Disbuff.drawString("Check Hardware", 55, 60, 1);
    Disbuff.pushSprite(0, 0);
    while (1)
        ;
}

#define PIN_CLK 0
#define PIN_DATA 34

bool InitI2SMicroPhone()
{
    esp_err_t err = ESP_OK;
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;
    

    err += i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    err += i2s_set_pin(I2S_NUM_0, &pin_config);
    err += i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    //i2s_set_clk(0)

    if (err != ESP_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
}
static esp_pm_lock_handle_t rmt_freq_lock;
#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define RMT_TX_GPIO_NUM GPIO_NUM_9
#define RMT_CLK_DIV (1) // 80000000 / 1(HZ)

rmt_item32_t *tx_buffer = NULL;

void ir_tx_callback(rmt_channel_t channel, void *arg)
{
    //static BaseType_t xHigherPriorityTaskWoken = false;
    if (channel == RMT_TX_CHANNEL)
    {
        esp_pm_lock_release(rmt_freq_lock);
        //xHigherPriorityTaskWoken = pdFALSE;
        //xSemaphoreGiveFromISR( irTxSem, &xHigherPriorityTaskWoken );
        free(tx_buffer);
    }
}

bool InitIRTx()
{
    /*
    esp_err_t err = esp_pm_lock_create(ESP_PM_APB_FREQ_MAX, 0, "rmt", &rmt_freq_lock);
    if (err != ESP_OK)
    {
        ErrorMeg(0x71, "RMT create fail");
        return false;
    }
    */
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;

    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = RMT_CLK_DIV;

    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_duty_percent = 50;
    rmt_tx.tx_config.carrier_freq_hz = 38000;
    rmt_tx.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
    rmt_tx.tx_config.carrier_en = true;
    rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    return true;
}

bool ir_uart_tx(const uint8_t *src, uint8_t len, bool wait_tx_done)
{
    /*
    if(src == NULL || recvFlag == true) {
    return false;
    }
    */
    if (src == NULL)
    {
        return false;
    }

#if BAUD == 4800
    const rmt_item32_t bit0 = {{{8421, 1, 8421, 1}}}; //Logical 0
    const rmt_item32_t bit1 = {{{8421, 0, 8421, 0}}}; //Logical 1
#else
    const rmt_item32_t bit0 = {{{16842, 1, 16842, 1}}}; //Logical 0
    const rmt_item32_t bit1 = {{{16842, 0, 16842, 0}}}; //Logical 1
#endif

    uint8_t *psrc = (uint8_t *)src;
    //xSemaphoreTake(irTxSem, portMAX_DELAY);

    tx_buffer = (rmt_item32_t *)malloc(sizeof(rmt_item32_t) * 10 * len);
    if (tx_buffer == NULL)
    {
        return false;
    }

    rmt_item32_t *pdest = tx_buffer;

    for (uint8_t ptr = 0; ptr < len; ptr++)
    {
        pdest->val = bit0.val;
        pdest++;
        for (int i = 0; i < 8; i++)
        {
            if (*psrc & (0x1 << i))
            {
                pdest->val = bit1.val;
            }
            else
            {
                pdest->val = bit0.val;
            }
            pdest++;
        }
        pdest->val = bit1.val;
        pdest++;
        psrc++;
    }

    esp_pm_lock_acquire(rmt_freq_lock);
    rmt_write_items(RMT_TX_CHANNEL, tx_buffer, 10 * len, wait_tx_done);
    return true;
}

#define SERVICE_UUID      "1bc68b2a-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_RX_UUID  "1bc68da0-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_TX_UUID  "1bc68efe-f3e3-11e9-81b4-2a2ae2dbcce4"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

uint8_t* data = new uint8_t[128];

class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
    data = pCharacteristic->getData();
    }
};

bool InitBLEServer()
{
    BLEDevice::init("M5-BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  pService = pServer->createService(SERVICE_UUID);
  pTxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_RX_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_TX_UUID,
                        BLECharacteristic::PROPERTY_WRITE
                      );
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    return true;

}

void Displaybuff()
{
    if (TestMode)
    {
        Disbuff.setTextSize(1);
        Disbuff.setTextColor(RED);
        Disbuff.drawString("Test Mode", 0, 0, 1);
        Disbuff.setTextColor(WHITE);
    }
    Disbuff.pushSprite(0, 0);
}

bool checkAXP192()
{
    float temp = M5.Axp.GetTempInAXP192();
    if ((temp < 5) || (temp > 90))
    {
        ErrorMeg(0x21, "AXP find error");
        return false;
    }

    float VBat = M5.Axp.GetBatVoltage();
    if ((VBat < 3.0) || (VBat > 4.4))
    {
        ErrorMeg(0x22, "Bat Vol error ");
        return false;
    }

    return true;
}

bool checkMPU6886()
{

    return true;
}

bool checkBM8563()
{
    uint8_t i = 0;
    M5.Rtc.GetBm8563Time();
    RTC_TimeTypeDef time;
    M5.Rtc.GetTime(&time);
    uint8_t sec = time.Seconds;
    while (1)
    {
        M5.Rtc.GetTime(&time);
        delay(10);
        i++;
        if (i > 200)
        {
            ErrorMeg(0x41, "RTC error ");
        }
        if (sec != time.Seconds)
        {
            break;
        }
    }
    return true;
}

bool checkOUTIO()
{
    uint32_t sumadc1 = 0, sumadc2 = 0;
    pinMode(26,INPUT);

    for (int i = 0; i < 50; i++)
    {
        sumadc2 += analogRead(36);
        delay(50);
    }

    sumadc1 = sumadc1 / 50;
    sumadc2 = sumadc2 / 50;

    sumadc1 = sumadc1 * 3300 / 4095 * 2;
    sumadc2 = sumadc2 * 3300 / 4095 * 2;

    printf("%d,%d\n", sumadc1, sumadc2);

    if ( sumadc1 < 3000 )
    {
        ErrorMeg(0x61, "5V or 26 error");
    }
    if ( sumadc2 < 2500 )
    {
        ErrorMeg(0x62, "3V3 or 36 error");
    }
    return true;
}

DHT12 dht12;

bool CheckGrove()
{
    float tmp = dht12.readTemperature();
    float hum = dht12.readHumidity();
    uint8_t count = 0;

    for (int i = 0; i < 10; i++)
    {
        tmp = dht12.readTemperature();
        hum = dht12.readHumidity();
        if(( tmp > 50 )||( tmp < -20 )||( hum > 100 )||( hum == 0 ))
        {
            count ++;
            if( count > 5 )
            {
                ErrorMeg(0x91, "Grove error");
                return false;
            }
        }
    }
    return true;
}

void ColorBar()
{
    float color_r, color_g, color_b;

    color_r = 0;
    color_g = 0;
    color_b = 255;

    for (int i = 0; i < 384; i=i+4)
    {
        if (i < 128)
        {
            color_r = i * 2;
            color_g = 0;
            color_b = 255 - (i * 2);
        }
        else if ((i >= 128) && (i < 256))
        {
            color_r = 255 - ((i - 128) * 2);
            color_g = (i - 128) * 2;
            color_b = 0;
        }
        else if ((i >= 256) && (i < 384))
        {
            color_r = 0;
            color_g = 255 - ((i - 256) * 2);
            ;
            color_b = (i - 256) * 2;
            ;
        }
        Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(color_r, color_g, color_b));
        if (TestMode)
        {
            Disbuff.setTextColor(RED);
            Disbuff.drawString("Test Mode", 0, 0, 1);
        }
        Displaybuff();
    }

    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            color_r = 0;
            color_g = 0;
            color_b = 0;
            break;
        case 1:
            color_r = 255;
            color_g = 0;
            color_b = 0;
            break;
        case 2:
            color_r = 0;
            color_g = 255;
            color_b = 0;
            break;
        case 3:
            color_r = 0;
            color_g = 0;
            color_b = 255;
            break;
        }
        for (int n = 0; n < 160; n++)
        {
            color_r = (color_r < 255) ? color_r + 1.6 : 255U;
            color_g = (color_g < 255) ? color_g + 1.6 : 255U;
            color_b = (color_b < 255) ? color_b + 1.6 : 255U;
            Disbuff.drawLine(n, i * 20, n, (i + 1) * 20, Disbuff.color565(color_r, color_g, color_b));
        }
    }
    if (TestMode)
    {
        Disbuff.setTextColor(RED);
        Disbuff.drawString("Test Mode", 0, 0, 1);
    }
    Displaybuff();
    delay(500);

    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            color_r = 255;
            color_g = 255;
            color_b = 255;
            break;
        case 1:
            color_r = 255;
            color_g = 0;
            color_b = 0;
            break;
        case 2:
            color_r = 0;
            color_g = 255;
            color_b = 0;
            break;
        case 3:
            color_r = 0;
            color_g = 0;
            color_b = 255;
            break;
        }
        for (int n = 0; n < 160; n++)
        {
            color_r = (color_r > 2) ? color_r - 1.5 : 0U;
            color_g = (color_g > 2) ? color_g - 1.5 : 0U;
            color_b = (color_b > 2) ? color_b - 1.5 : 0U;
            Disbuff.drawLine(159 - n, i * 20, 159 - n, (i + 1) * 20, Disbuff.color565(color_r, color_g, color_b));
        }
    }
    Displaybuff();
    delay(500);
}
double r_rand = PI / 180;

double r_alpha = 19.47 * PI / 180;
double r_gamma = 20.7 * PI / 180;

double sin_alpha = sin(19.47 * PI / 180);
double cos_alpha = cos(19.47 * PI / 180);
double sin_gamma = sin(20.7 * PI / 180);
double cos_gamma = cos(20.7 * PI / 180);

bool point3Dto2D(point_3d_t *source, point_2d_t *point)
{
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) - (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
}

bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y)
{
    *x = point->x + 80;
    *y = 40 - point->y;
    return true;
}

bool printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color)
{
    uint8_t start_x, start_y, end_x, end_y;
    point_2d_t point;
    point3Dto2D(&line->start_point, &point);
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point2DToDisPoint(&point, &end_x, &end_y);

    display->drawLine(start_x, start_y, end_x, end_y, color);

    return true;
}

void RotatePoint(point_3d_t *point, double x, double y, double z)
{
    if (x != 0)
    {
        point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

void RotatePoint(point_3d_t* point, point_3d_t* point_new, double x, double y, double z)
{
    if (x != 0)
    {
        point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

line_3d_t rect[12] = {
    {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
    {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
    {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
    {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
    {
        .start_point = {-1, -1, 1},
        .end_point = {-1, -1, -1},
    },
    {
        .start_point = {1, -1, 1},
        .end_point = {1, -1, -1},
    },
    {
        .start_point = {1, 1, 1},
        .end_point = {1, 1, -1},
    },
    {
        .start_point = {-1, 1, 1},
        .end_point = {-1, 1, -1},
    },
    {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
    {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
    {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
    {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
};

void MPU6886Test()
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    double theta = 0, last_theta = 0;
    double phi = 0, last_phi = 0;
    double alpha = 0.2;

    line_3d_t x = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t y = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t z = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 30}};

    line_3d_t rect_source[12];
    line_3d_t rect_dis;
    for (int n = 0; n < 12; n++)
    {
        rect_source[n].start_point.x = rect[n].start_point.x * 15;
        rect_source[n].start_point.y = rect[n].start_point.y * 15;
        rect_source[n].start_point.z = rect[n].start_point.z * 15;
        rect_source[n].end_point.x = rect[n].end_point.x * 15;
        rect_source[n].end_point.y = rect[n].end_point.y * 15;
        rect_source[n].end_point.z = rect[n].end_point.z * 15;
    }

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {

        M5.MPU6886.getAccelData(&accX, &accY, &accZ);
        if ((accX < 1) && (accX > -1))
        {
            theta = asin(-accX) * 57.295;
        }
        if (accZ != 0)
        {
            phi = atan(accY / accZ) * 57.295;
        }

        theta = alpha * theta + (1 - alpha) * last_theta;
        phi = alpha * phi + (1 - alpha) * last_phi;

        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.setTextSize(1);
        Disbuff.setCursor(10, 55);
        Disbuff.printf("%.2f", theta);
        Disbuff.setCursor(10, 65);
        Disbuff.printf("%.2f", phi);
        //Displaybuff();
        delay(20);

        z.end_point.x = 0;
        z.end_point.y = 0;
        z.end_point.z = 30;
        RotatePoint(&z.end_point, theta, phi, 0);
        RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
        RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

        for (int n = 0; n < 12; n++)
        {
            RotatePoint(&rect_source[n].start_point, &rect_dis.start_point, theta, phi, (double)0);
            RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta, phi, (double)0);
            printLine3D(&Disbuff, &rect_dis, WHITE);
        }
        //Disbuff.fillRect(0,0,160,80,BLACK);
        printLine3D(&Disbuff, &x, RED);
        printLine3D(&Disbuff, &y, GREEN);
        printLine3D(&Disbuff, &z, BLUE);
        //Disbuff
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        Disbuff.fillRect(0,0,52,18,Disbuff.color565(20,20,20));
        Disbuff.drawString("MPU6886",5,5,1);
        Displaybuff();

        last_theta = theta;
        last_phi = phi;

        M5.update();
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}

void DisplayI2CENV()
{
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Displaybuff();
    float tmp;
    float hum;
    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        tmp = dht12.readTemperature();
        hum = dht12.readHumidity();
        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.setTextSize(3);
        Disbuff.setCursor(5, 15);
        Disbuff.printf("TMP:%2.1f", tmp);
        Disbuff.setCursor(5, 45);
        Disbuff.printf("HUM:%2.0f%%", hum);
        Displaybuff();
        M5.update();
        delay(100);
    }

    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}

SemaphoreHandle_t xSemaphore = NULL;
SemaphoreHandle_t start_dis = NULL;
SemaphoreHandle_t start_fft = NULL;
int8_t i2s_readraw_buff[1024];
uint8_t fft_dis_buff[161][80] = {0};
uint16_t posData = 160;

void MicRecordfft(void *arg)
{
    int16_t *buffptr;
    size_t bytesread;
    uint16_t count_n = 0;
    float adc_data;
    double data = 0;
    uint16_t ydata;

    while (1)
    {
        xSemaphoreTake(start_fft, portMAX_DELAY);
        xSemaphoreGive(start_fft);
        fft_config_t *real_fft_plan = fft_init(512, FFT_REAL, FFT_FORWARD, NULL, NULL);
        i2s_read(I2S_NUM_0, (char *)i2s_readraw_buff, 1024, &bytesread, (100 / portTICK_RATE_MS));
        buffptr = (int16_t *)i2s_readraw_buff;

        for (count_n = 0; count_n < real_fft_plan->size; count_n++)
        {
            adc_data = (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -1000, 1000);
            real_fft_plan->input[count_n] = adc_data;
        }
        fft_execute(real_fft_plan);

        xSemaphoreTake(xSemaphore, 100 / portTICK_RATE_MS);
        for (count_n = 1; count_n < real_fft_plan->size / 4; count_n++)
        {
            data = sqrt(real_fft_plan->output[2 * count_n] * real_fft_plan->output[2 * count_n] + real_fft_plan->output[2 * count_n + 1] * real_fft_plan->output[2 * count_n + 1]);
            if ((count_n - 1) < 80)
            {
                ydata = map(data, 0, 2000, 0, 256);
                fft_dis_buff[posData][80 - count_n] = ydata;
            }
        }
        posData++;
        if (posData >= 161)
        {
            posData = 0;
        }
        xSemaphoreGive(xSemaphore);
        fft_destroy(real_fft_plan);
    }
}

void Drawdisplay(void *arg)
{
    uint16_t count_x = 0, count_y = 0;
    uint16_t colorPos;
    while (1)
    {
        xSemaphoreTake(start_dis, portMAX_DELAY);
        xSemaphoreGive(start_dis);
        xSemaphoreTake(xSemaphore, 500 / portTICK_RATE_MS);
        for (count_y = 0; count_y < 80; count_y++)
        {
            for (count_x = 0; count_x < 160; count_x++)
            {
                if ((count_x + (posData % 160)) > 160)
                {
                    colorPos = fft_dis_buff[count_x + (posData % 160) - 160][count_y];
                }
                else
                {
                    colorPos = fft_dis_buff[count_x + (posData % 160)][count_y];
                }

                Disbuff.drawPixel(count_x, count_y, Disbuff.color565(ImageData[colorPos * 3 + 0], ImageData[colorPos * 3 + 1], ImageData[colorPos * 3 + 2]));
                /*
        disbuff[ count_y * 160 + count_x ].r =  ImageData[ colorPos * 3 + 0 ];
        disbuff[ count_y * 160 + count_x ].g =  ImageData[ colorPos * 3 + 1 ];
        disbuff[ count_y * 160 + count_x ].b =  ImageData[ colorPos * 3 + 2 ];
        */
            }
        }
        xSemaphoreGive(xSemaphore);
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        Disbuff.fillRect(0,0,70,18,Disbuff.color565(20,20,20));
        Disbuff.drawString("MicroPhone",5,5,1);
        Disbuff.pushSprite(0, 0);
    }
}

TaskHandle_t xhandle_display = NULL;
TaskHandle_t xhandle_fft = NULL;

void DisplayMicro()
{
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Disbuff.pushSprite(0, 0);

    xSemaphoreGive(start_dis);
    xSemaphoreGive(start_fft);
    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        xSemaphoreGive(start_dis);
        xSemaphoreGive(start_fft);
        M5.update();
        //delay(100);
        xSemaphoreTake(start_dis, portMAX_DELAY);
        xSemaphoreTake(start_fft, portMAX_DELAY);
    }
    //xSemaphoreTake( start_dis , portMAX_DELAY  );
    //xSemaphoreTake( start_fft , portMAX_DELAY  );

    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}

void DisplayRTC()
{
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Displaybuff();
    M5.Rtc.GetBm8563Time();
    RTC_TimeTypeDef time;
    M5.Rtc.GetTime(&time);

    Disbuff.setTextSize(3);
    Disbuff.setCursor(6, 25);
    Disbuff.setTextColor(WHITE);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
        M5.Rtc.GetTime(&time);
        Disbuff.setTextSize(3);
        Disbuff.setTextColor(WHITE);
        Disbuff.setCursor(6, 40);
        Disbuff.printf("%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
        Disbuff.fillRect(0,0,160,18,Disbuff.color565(20,20,20));
        Disbuff.setTextSize(1);
        Disbuff.drawString("BMP8563 RTC Time",32,5,1);
        Displaybuff();
        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}

void DisplayIOPort()
{
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = 33;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;
    i2s_set_pin(I2S_NUM_0,&pin_config);
    
    i2s_driver_uninstall(I2S_NUM_0);
    gpio_reset_pin(GPIO_NUM_0);
    gpio_reset_pin(GPIO_NUM_26);

    pinMode(26,OUTPUT);
    pinMode(0,OUTPUT);

    digitalWrite(0,1);
    digitalWrite(26,1);
    uint16_t pin36_adc = analogRead(36);

    Disbuff.setTextSize(2);
    Disbuff.setCursor(6, 25);
    Disbuff.setTextColor(WHITE);

    uint8_t count = 0;

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.setTextSize(2);

        digitalWrite(0,0);
        digitalWrite(26,0);
        count = 0;

        for( int i = 0 ; i< 10; i++ )
        {
            digitalWrite( 0, i%2 );
            delay(10);
            pin36_adc = analogRead(36);
            if(( pin36_adc > 1800 )&&( i%2 == 1 ))
            {
                count ++;
            }
        }

        Disbuff.setCursor(5, 5);
        if( count >= 5 )
        {
            Disbuff.setTextColor(GREEN);
            Disbuff.printf("%d G0 OK", count);
        }
        else
        {
            Disbuff.setTextColor(RED);
            Disbuff.printf("%d G0 faild", count);
        }
        Disbuff.setTextColor(WHITE);
        
        digitalWrite(0,0);
        digitalWrite(26,0);
        count = 0;

        for( int i = 0 ; i< 10; i++ )
        {
            digitalWrite( 26, i%2 );
            delay(10);
            pin36_adc = analogRead(36);
            if(( pin36_adc > 1800 )&&( i%2 == 1 ))
            {
                count ++;
            }
        }

        Disbuff.setCursor(5, 20);
        if( count >= 5 )
        {
            Disbuff.setTextColor(GREEN);
            Disbuff.printf("%d G26 OK", count);
        }
        else
        {
            Disbuff.setTextColor(RED);
            Disbuff.printf("%d G26 faild", count);
        }
        Disbuff.setTextColor(WHITE);

        digitalWrite(0,0);
        digitalWrite(26,0);
        delay(10);
        pin36_adc = analogRead(36);

        Disbuff.setCursor(5, 35);
        Disbuff.printf("%.2f", M5.Axp.GetVinVoltage());
        if( M5.Axp.GetVinVoltage() > 4.2 )
        {
            Disbuff.setTextColor(GREEN);
            Disbuff.printf(" 5V OK");
        }
        else
        {
            Disbuff.setTextColor(RED);
            Disbuff.printf("5V Err");
        }
        Disbuff.setTextColor(WHITE);
        
        Disbuff.setCursor(5, 50);
        Disbuff.printf("%d", pin36_adc);
        if( pin36_adc > 1100 )
        {
            Disbuff.setTextColor(GREEN);
            Disbuff.printf("3V3 OK");
        }
        else
        {
            Disbuff.setTextColor(RED);
            Disbuff.printf("3V3 Err");
        }
        Disbuff.setTextColor(WHITE);
        
        Displaybuff();
        /*
        pin36_adc = analogRead(36);

        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.setTextSize(3);
        Disbuff.setCursor(5, 15);
        Disbuff.printf("%.2f", M5.Axp.GetVinVoltage());
        Disbuff.setCursor(5, 45);
        Disbuff.printf("%d", pin36_adc);
        Displaybuff();
        */
        M5.update();
        delay(100);

    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
    InitI2SMicroPhone();
}

void DisIRSend()
{
    uint8_t senddata[2]={0};
    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.pushImage(110, 16, 48, 48, (uint16_t *)icon_ir);
        Disbuff.setTextColor(Disbuff.color565(180,180,180));
        Disbuff.setTextSize(2);
        Disbuff.setCursor(12, 20);
        Disbuff.printf("IR Send ");
        Disbuff.setCursor(12, 45);
        if( senddata[0] % 4 == 0 )
        {
            Disbuff.printf("0x%02X>  ",senddata[0]);
        }
        else if( senddata[0] % 4 == 1 )
        {
            Disbuff.printf("0x%02X>>",senddata[0]);
        }
        else if( senddata[0] % 4 == 2 )
        {
            Disbuff.printf("0x%02X >>",senddata[0]);
        }
        else if( senddata[0] % 4 == 3 )
        {
            Disbuff.printf("0x%02X  >",senddata[0]);
        }
        
        Displaybuff();

        senddata[1]++;
        if( senddata[1] > 3 )
        {
            senddata[1] = 0;
            senddata[0]++;
            ir_uart_tx( senddata ,2, true);
        }

        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
    Disbuff.setTextColor(WHITE);
}

void DisPlayBLESend()
{
    uint8_t senddata[2]={0};

    pService->start();
  pServer->getAdvertising()->start();

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        if( deviceConnected )
        {
            Disbuff.pushImage(110, 16, 48, 48, (uint16_t *)icon_ble);
            Disbuff.setTextColor(Disbuff.color565(180,180,180));
            Disbuff.setTextSize(2);
            Disbuff.setCursor(12, 20);
            //Disbuff.printf("BLE connect!\n");
            Disbuff.printf("BLE Send\n");
            Disbuff.setCursor(12, 45);
            if( senddata[0] % 4 == 0 )
            {
                Disbuff.printf("0x%02X>  ",senddata[0]);
            }
            else if( senddata[0] % 4 == 1 )
            {
                Disbuff.printf("0x%02X>>",senddata[0]);
            }
            else if( senddata[0] % 4 == 2 )
            {
                Disbuff.printf("0x%02X >>",senddata[0]);
            }
            else if( senddata[0] % 4 == 3 )
            {
                Disbuff.printf("0x%02X  >",senddata[0]);
            }

            senddata[1]++;
            if( senddata[1] > 3 )
            {
                senddata[1] = 0;
                senddata[0]++;
                pTxCharacteristic->setValue( senddata, 1 );
                pTxCharacteristic->notify();
            }
        }
        else
        {
            Disbuff.setTextSize(1);
            Disbuff.setCursor(12, 20);
            Disbuff.setTextColor(RED);
            Disbuff.printf("BLE disconnect\n");
            Disbuff.setCursor(12, 35);
            Disbuff.setTextColor(Disbuff.color565(18,150,219));
            Disbuff.printf("BLE Name:M5-BLE\n");
            Disbuff.setCursor(12, 50);
            Disbuff.printf("UUID:1bc68b2a\n");
            Disbuff.pushImage(110, 16, 48, 48, (uint16_t *)icon_ble_disconnect);
        }
        
        Displaybuff();
        
        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
    Disbuff.setTextColor(WHITE);
    pService->stop();
    pServer->getAdvertising()->stop();
}

void DisplayWIFI()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = 0;

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.pushImage(110, 16, 48, 48, (uint16_t *)icon_wifi);
        n = WiFi.scanNetworks();
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        if (n == 0)
        {
            Disbuff.setCursor(5, 20);
            Disbuff.setTextColor(RED);
            Disbuff.printf("No WIFI found\n");
            Disbuff.setTextColor(WHITE);
        }
        else
        {   
            Disbuff.setTextColor(WHITE);
            n = ( n > 4 )? 4 : n;
            for (int i = 0; i < n; ++i)
            {
                Disbuff.setCursor(5, 20);
                Disbuff.setTextColor(RED);
                if( WiFi.SSID(i).length() >= 10 )
                {
                    Disbuff.drawString(WiFi.SSID(i).substring(0,10),5,( 20 + i * 10));
                }
                else
                {
                    Disbuff.drawString(WiFi.SSID(i),5,( 20 + i * 10));
                }
            }
        }
        Disbuff.fillRect(0,0,160,18,Disbuff.color565(20,20,20));
        Disbuff.setTextSize(1);
        Disbuff.drawString("Wifi Test",32,5,1);
    
        
        Displaybuff();
        
        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}

unsigned char stick[12960];

void unpack_data(const uint8_t *data, uint8_t *buffer)
{
  int size = data[0] + (data[1] << 8);

  for (int i = 2; i < size; i += 3) {
    // data set
    *(buffer++) = data[i];
    *(buffer++) = data[i + 1];
    for (int j = 0; j < data[i + 2]; j++) {
      // same data set
      *(buffer++) = data[i];
      *(buffer++) = data[i + 1];
    }
  }
}

void setup()
{
    M5.begin();
    M5.update();
    if (M5.BtnB.isPressed())
    {
        TestMode = true;
    }
    Wire.begin(32,33,10000);

    M5.Lcd.setRotation(3);
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(160, 80);
    Disbuff.setSwapBytes(true);
    for (int n = 0; n < 12; n++)
    {
        unpack_data(Animationptr[n], stick);
        Disbuff.pushImage(40, 0, 81, 80, (uint16_t *)stick);
        Displaybuff();
        delay(30);
    }

    Serial.printf("FUCK STC\r\n");
    if (InitI2SMicroPhone() != true)
    {
        ErrorMeg(0x51, "MicroPhone error");
    }
    if (M5.MPU6886.Init() != 0)
    {
        ErrorMeg(0x31, "MPU6886 error ");
    }
    if( InitIRTx() != true )
    {
        ErrorMeg(0x72, "RMT Init error");
    }
    if( InitBLEServer() != true )
    {
        ErrorMeg(0x81, "BLE init error");
    }
    checkAXP192();
    checkBM8563();
    uint16_t ellips[7] = { 70, 78, 86, 93, 100, 108, 116};
    for (int n = 11; n < 18; n++)
    {
        Disbuff.fillEllipse(80,40,ellips[n-11]/2, ellips[n-11]/2, Disbuff.color565(43,43,43));
        unpack_data(Animationptr[n], stick);
        Disbuff.pushImage(40, 0, 81, 80, (uint16_t *)stick);
        Displaybuff();
        delay(20);
    }

    int color_bk = 43;
    for (int n = 116; n < 180; n=n+8)
    {
        Disbuff.fillEllipse(80,40,n/2, n/2, Disbuff.color565(color_bk,color_bk,color_bk));
        Displaybuff();
        delay(20);
        color_bk -= 5;
    }

    Disbuff.fillRect(0,0,160,80,BLACK);
    Displaybuff();
    if (TestMode)
    {
        ColorBar();
    }

    pinMode(10, OUTPUT);

    timerSemaphore = xSemaphoreCreateBinary();
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 500000, true);
    timerAlarmEnable(timer);

    xSemaphore = xSemaphoreCreateMutex();
    start_dis = xSemaphoreCreateMutex();
    start_fft = xSemaphoreCreateMutex();

    xSemaphoreTake(start_dis, portMAX_DELAY);
    xSemaphoreTake(start_fft, portMAX_DELAY);

    xTaskCreate(Drawdisplay, "Drawdisplay", 1024 * 2, (void *)0, 4, &xhandle_display);
    xTaskCreate(MicRecordfft, "MicRecordfft", 1024 * 2, (void *)0, 5, &xhandle_fft);
}
uint8_t xData, yData;
void loop()
{
    delay(100);
    MPU6886Test();
    DisplayRTC();
    DisplayMicro();
    DisIRSend();
    DisPlayBLESend();
    //DisplayWIFI();
    if (TestMode)
    {
        DisplayI2CENV();
        DisplayIOPort();
    }
}
