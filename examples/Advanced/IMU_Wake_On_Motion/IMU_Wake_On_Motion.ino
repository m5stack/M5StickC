#include <Arduino.h>
#include <M5StickC.h>
#include <driver/rtc_io.h> // from ESP-IDF


void mpu6886_wake_on_motion_isr(void); // declaration of ISR


#define WOM_ATTACH_ISR
volatile uint32_t g_wom_count = 0;
volatile uint32_t g_wom_last_millis = 0;
void IRAM_ATTR mpu6886_wake_on_motion_isr(void) {
    g_wom_count++;
    g_wom_last_millis = millis();
}


/* Method to print the reason by which ESP32 has been awoken from sleep */
void get_wakeup_reason_string(char *cbuf, int cbuf_len){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0     : snprintf(cbuf, cbuf_len, "ext0");             break;
        case ESP_SLEEP_WAKEUP_EXT1     : snprintf(cbuf, cbuf_len, "ext1");             break;
        case ESP_SLEEP_WAKEUP_TIMER    : snprintf(cbuf, cbuf_len, "timer");            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : snprintf(cbuf, cbuf_len, "touchpad");         break;
        case ESP_SLEEP_WAKEUP_ULP      : snprintf(cbuf, cbuf_len, "ULP");              break;
        default                        : snprintf(cbuf, cbuf_len, "%d",wakeup_reason); break;
    }
}


RTC_DATA_ATTR int bootCount = 0;
#define WAKE_REASON_BUF_LEN 100
void setup() {
    char wake_reason_buf[WAKE_REASON_BUF_LEN];

    // put your setup code here, to run once:
    M5.begin();
    M5.Axp.ScreenBreath(8);
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(1);

    get_wakeup_reason_string(wake_reason_buf, WAKE_REASON_BUF_LEN);
    M5.Lcd.setCursor( 0,  0); M5.Lcd.printf("WOM: BOOT=%d, SRC=%s", bootCount, wake_reason_buf);
    M5.Lcd.setCursor( 0, 10); M5.Lcd.printf("Battery: ");
    M5.Lcd.setCursor( 0, 20); M5.Lcd.printf("V: %.3f v", M5.Axp.GetBatVoltage());
    M5.Lcd.setCursor( 0, 30); M5.Lcd.printf("I: %.3f ma", M5.Axp.GetBatCurrent());
    M5.Lcd.setCursor( 0, 40); M5.Lcd.printf("P: %.3f mw", M5.Axp.GetBatPower());



#ifdef WOM_GPIO_DEBUG_TEST
    pinMode(GPIO_NUM_26, OUTPUT);
    pinMode(GPIO_NUM_36, INPUT);
#endif // #ifdef WOM_GPIO_DEBUG_TEST


#ifdef WOM_ATTACH_ISR
    // set up ISR to trigger on GPIO35
    delay(100);
    pinMode(GPIO_NUM_35, INPUT);
    delay(100);
    attachInterrupt(GPIO_NUM_35, mpu6886_wake_on_motion_isr, FALLING);
#endif // #ifdef WOM_ATTACH_ISR

    //Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // set up mpu6886 for low-power operation
    M5.Mpu6886.Init(); // basic init
    M5.Mpu6886.enableWakeOnMotion(M5.Mpu6886.AFS_16G, 10);

    // wait until IMU ISR hasn't triggered for X milliseconds
    while(1) {
        uint32_t since_last_wom_millis = millis() - g_wom_last_millis;
        if(since_last_wom_millis > 5000) {
            break;
        }
        Serial.printf("waiting : %d", since_last_wom_millis);
        delay(1000);
    }

    // disable all wakeup sources
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    
    // enable waking up on pin 35 (from IMU)
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, LOW); //1 = High, 0 = Low

    //Go to sleep now
    Serial.println("Going to sleep now");
    M5.Axp.SetSleep(); // conveniently turn off screen, etc.
    delay(100);
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}


void loop() {
}