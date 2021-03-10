#include <M5StickC.h>
#include "Free_Fonts.h"
#define MIC_Unit 33
#define MAX_LEN 180
#define X_OFFSET 0
#define Y_OFFSET 40
#define X_SCALE 1

static void draw_waveform() {

  static int16_t val_buf[MAX_LEN] = {0};
  static int16_t pt = MAX_LEN - 1;
  int micValue = analogRead(MIC_Unit);
  val_buf[pt] = map((int16_t)(micValue * X_SCALE), 1800, 4095,  0, 40);


  if (--pt < 0) {
    pt = MAX_LEN - 1;
  }

  for (int i = 1; i < (MAX_LEN); i++) {
    uint16_t now_pt = (pt + i) % (MAX_LEN);
    M5.Lcd.drawLine(i + X_OFFSET, val_buf[(now_pt + 1) % MAX_LEN] + Y_OFFSET, i + 1 + X_OFFSET, val_buf[(now_pt + 2) % MAX_LEN] + Y_OFFSET, TFT_BLACK);
    if (i < MAX_LEN - 1) {
      M5.Lcd.drawLine(i + X_OFFSET, val_buf[now_pt] + Y_OFFSET, i + 1 + X_OFFSET, val_buf[(now_pt + 1) % MAX_LEN] + Y_OFFSET, TFT_GREEN);
    }
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
 
  dacWrite(25, 0); 
}

void loop() {
  draw_waveform();
}
