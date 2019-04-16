#ifndef _M5DISPLAY_H_
#define _M5DISPLAY_H_

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include "utility/In_eSPI.h"
#include "utility/Sprite.h"
#include "Fonts/HZK16.h"
#include "Fonts/ASC16.h"

typedef enum {
  JPEG_DIV_NONE,
  JPEG_DIV_2,
  JPEG_DIV_4,
  JPEG_DIV_8,
  JPEG_DIV_MAX
} jpeg_div_t;


typedef enum
{
  DontUsedHzk16,
  InternalHzk16,
  ExternalHzk16
}Hzk16Types;

class M5Display : public TFT_eSPI {

 public:
  M5Display();
  void begin();
  void sleep();
  void setBrightness(uint8_t brightness);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent);
  
  
  void loadHzk16(Hzk16Types hzkTypes = InternalHzk16,const char* HZK16Path = "/HZK16", const char* ASC16Path = "/ASC16");
  void setTextWrap(boolean wrap);

  void writeHzk(char* c);
  // Highlight the text (Once set to be true, the text background will not be transparent any more)
  inline void highlight(bool isHighlight) { highlighted = isHighlight; }
  // Set highlight color
  inline void setHighlightColor(uint16_t color) { highlightcolor = color; istransparent = false; }
  void qrcode(const char *string, uint16_t x = 5, uint16_t y = 45, uint8_t width = 70, uint8_t version = 7);
  void qrcode(const String &string, uint16_t x = 5, uint16_t y = 45, uint8_t width = 70, uint8_t version = 7);
  /*
  void clearDisplay(uint32_t color=ILI9341_BLACK) { fillScreen(color); }
  void clear(uint32_t color=ILI9341_BLACK) { fillScreen(color); }
  void display() {}

  inline void startWrite(void){
  #if defined (SPI_HAS_TRANSACTION) && defined (SUPPORT_TRANSACTIONS) && !defined(ESP32_PARALLEL)
    if (locked) {locked = false; SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));}
  #endif
    CS_L;
  }
  inline void endWrite(void){
  #if defined (SPI_HAS_TRANSACTION) && defined (SUPPORT_TRANSACTIONS) && !defined(ESP32_PARALLEL)
    if(!inTransaction) {if (!locked) {locked = true; SPI.endTransaction();}}
  #endif
    CS_H;
  }
  inline void writePixel(uint16_t color) { SPI.write16(color); }
  inline void writePixels(uint16_t * colors, uint32_t len){
      SPI.writePixels((uint8_t*)colors , len * 2);
  }
  void progressBar(int x, int y, int w, int h, uint8_t val);

  #define setFont setFreeFont

  //void qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
  //void qrcode(const String &string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);

  void drawBmp(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
  void drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data);
  void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent);

  void drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x = 0,
               uint16_t y = 0, uint16_t maxWidth = 0, uint16_t maxHeight = 0,
               uint16_t offX = 0, uint16_t offY = 0,
               jpeg_div_t scale = JPEG_DIV_NONE);

  void drawJpg(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                   uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                   uint16_t offX = 0, uint16_t offY = 0,
                   jpeg_div_t scale = JPEG_DIV_NONE);

  void drawJpgFile(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                   uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                   uint16_t offX = 0, uint16_t offY = 0,
                   jpeg_div_t scale = JPEG_DIV_NONE);*/

 private:

 void writeHzkAsc(const char c);
 void writeHzkGbk(const char *c);

 bool initHzk16(boolean use, const char *HZK16Path,
                        const char *ASC16Path);

 inline bool isHzk16Used(){return hzk16Used;}

 boolean hzk16Used,istransparent,
          highlighted;

 uint8_t hzkBufCount;

 uint8_t *pAscCharMatrix, *pGbkCharMatrix; 

  uint16_t
          highlightcolor,
          ascCharWidth, 
          ascCharHeigth, 
          gbkCharWidth, 
          gbkCharHeight;

 Hzk16Types  hzk16Type;          // Use of HZK16 and ASC16 font. 

 File
    Asc16File, Hzk16File,   // Font file
    *pAsc16File, *pHzk16File; // Font file pointer
  
  boolean  textwrap; // If set, 'wrap' text at right edge of display
};
#endif
