#include <M5StickC.h>

const int servo_pin = 26;
int freq = 50;
int ledChannel = 0;
int resolution = 10;
extern const unsigned char m5stack_startup_music[];
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 30, 4);
  M5.Lcd.println("speaker");
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(servo_pin, ledChannel);
  ledcWrite(ledChannel, 256);//0°

}
void playMusic(const uint8_t* music_data, uint16_t sample_rate) {
  uint32_t length = strlen((char*)music_data);
  uint16_t delay_interval = ((uint32_t)1000000 / sample_rate);
    for(int i = 0; i < length; i++) {
      ledcWriteTone(ledChannel, music_data[i]*50);
      delayMicroseconds(delay_interval);
    } 
}
void loop() {
  // put your main code here, to run repeatedly:
  ledcWriteTone(ledChannel, 1250);
  delay(1000);
  ledcWriteTone(ledChannel, 0);
  delay(1000);
}
