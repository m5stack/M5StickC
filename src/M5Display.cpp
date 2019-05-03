#include "M5Display.h"

M5Display::M5Display() : TFT_eSPI() {

  hzk16Used = false;
  hzk16Type = DontUsedHzk16;
}

void M5Display::begin() {
   TFT_eSPI::begin();
   fillScreen(0);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data, transparent);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (const uint16_t*)data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (uint16_t*)data);
}



/***************************************************************************************

** Function name:           loadHzk16

** Description:             loadHzk16 fonts

** Params:

**  HZK16Path: HZK16 file path on TF card. e.g. /HZK16, means file on the

*root dir of TF card. * ASC16Path: ASC16 file path on TF card.

***************************************************************************************/

void M5Display::loadHzk16(Hzk16Types hzkTypes,const char *HZK16Path, const char *ASC16Path) {

  if (hzk16Used)

    return;


//Hzk16Types hzkTypes = InternalHzk16
if(hzkTypes == InternalHzk16){
//#if defined(_ASC16_) && defined(_HZK16_)

  pAscCharMatrix = (uint8_t *)&ASC16[0];

  pGbkCharMatrix = (uint8_t *)&HZK16[0];

  Serial.println("HZK16 path: Internal");

  Serial.println("ASC16 path: Internal");

  hzk16Used = initHzk16(true, nullptr, nullptr);
}

  Serial.print("HZK16 init result: ");

  Serial.println(isHzk16Used());

}


bool M5Display::initHzk16(boolean use, const char *HZK16Path,
                        const char *ASC16Path) {

  bool result = false;

  if (use == false) { // Do not use HZK16 and ASC16 fonts

    hzk16Type = DontUsedHzk16;

    Serial.println("Use default font.");

  } else if (pAscCharMatrix == NULL ||

             pGbkCharMatrix ==

                 NULL) { // Use external HZK16 and ASC16 font on TF card.

      hzk16Type = DontUsedHzk16;

      Serial.println("External font file HZK16/ASC16 lost, use default font.");

  } else { // Use internal HZK16 and ASC16 fonts

    hzk16Type = InternalHzk16;

    Serial.println("Use internal HZK16 and ASC16 font.");

  }



  switch (hzk16Type) {



  case InternalHzk16: {

    if (pAscCharMatrix == NULL || pGbkCharMatrix == NULL) {

      hzk16Type = DontUsedHzk16;

    }



    if (pHzk16File != NULL) {

      pHzk16File->close();

      pHzk16File = NULL;

    }

    if (pAsc16File != NULL) {

      pAsc16File->close();

      pAsc16File = NULL;

    }

    hzkBufCount = 0;

    break;

  }

  case DontUsedHzk16: {

    if (pHzk16File != NULL) {

      pHzk16File->close();

      pHzk16File = NULL;

    }

    if (pAsc16File != NULL) {

      pAsc16File->close();

      pAsc16File = NULL;

    }

    break;

  }

  default: {
    break;
  }

  }

  return hzk16Type != DontUsedHzk16;

}

void M5Display::writeHzk(char* c){

  char *ret  = c;

  ascCharWidth = 8 * textsize;

  ascCharHeigth = 16 * textsize;

  gbkCharWidth = ascCharHeigth;

  gbkCharHeight = gbkCharWidth;
  if(ret == NULL) return;
  while(*ret != '\0'){
    while(*ret <= 0xA0){
      if(*ret == '\0') return;
      writeHzkAsc(*ret);
      ret++;  
    }
    writeHzkGbk(ret);
    ret++;
    ret++;
  }
}

void M5Display::writeHzkAsc(const char c) {

  if (c == '\n') {

    cursor_x = 0;

    cursor_y += ascCharHeigth;

  } else if (c != '\r') {

    uint32_t offset;

    uint8_t mask;

    uint16_t posX = cursor_x, posY = cursor_y;

    uint8_t charMatrix[16];

    uint8_t *pCharMatrix;



    offset = (uint32_t)c * 16;



    if (hzk16Type == ExternalHzk16) {

      pAsc16File->seek(offset, SeekSet);

      pAsc16File->readBytes((char *)&charMatrix[0], 16);

      pCharMatrix = &charMatrix[0];

    } else {

      if (pAscCharMatrix == NULL) {

        return;

      }

      pCharMatrix = pAscCharMatrix + offset;

    }



    // startWrite();



    if (highlighted) {

      fillRect(cursor_x, cursor_y, ascCharWidth, ascCharHeigth, highlightcolor);

    } else if (istransparent == false) {

      fillRect(cursor_x, cursor_y, ascCharWidth, ascCharHeigth, textbgcolor);

    }



    for (uint8_t row = 0; row < 16; row++) {

      mask = 0x80;

      posX = cursor_x;

      for (uint8_t col = 0; col < 8; col++) {

        if ((*pCharMatrix & mask) != 0) {

          if (textsize == 1) {

            drawPixel(posX, posY, textcolor);

          } else {

            fillRect(posX, posY, textsize, textsize, textcolor);

          }

        }

        posX += textsize;

        mask >>= 1;

      }

      posY += textsize;

      pCharMatrix++;

    }



    // endWrite();



    cursor_x += ascCharWidth;

    if (textwrap && ((cursor_x + ascCharWidth) > _width)) {

      cursor_x = 0;

      cursor_y += ascCharHeigth;

    }

  }

}


void M5Display::writeHzkGbk(const char *c) {

  uint32_t offset;

  uint8_t mask;

  uint16_t posX = cursor_x;
  uint16_t posY = cursor_y;

  uint8_t charMatrix[32];

  uint8_t *pCharMatrix;



  offset = (uint32_t)(94 * (uint32_t)(c[0] - 0xA1) + (uint32_t)(c[1] - 0xA1)) * 32;

  if (hzk16Type == ExternalHzk16) {

    pHzk16File->seek(offset, SeekSet);

    pHzk16File->readBytes((char *)&charMatrix[0], 32);

    pCharMatrix = &charMatrix[0];

  } else {

    if (pGbkCharMatrix == NULL) {

      return;

    }

    pCharMatrix = pGbkCharMatrix + offset;

  }



  // startWrite();



  if (highlighted) {

    fillRect(cursor_x, cursor_y, gbkCharWidth, gbkCharHeight, highlightcolor);

  } else if (istransparent == false) {

    fillRect(cursor_x, cursor_y, gbkCharWidth, gbkCharHeight, textbgcolor);

  }



  for (uint8_t row = 0; row < 16; row++) {

    posX = cursor_x;

    mask = 0x80;

    for (uint8_t col = 0; col < 8; col++) {

      if ((*pCharMatrix & mask) != 0) {

        if (textsize == 1) {

          drawPixel(posX, posY, textcolor);

        } else {

          fillRect(posX, posY, textsize, textsize, textcolor);

        }

      }

      if ((*(pCharMatrix + 1) & mask) != 0) {

        if (textsize == 1) {

          drawPixel(posX + ascCharWidth, posY, textcolor);

        } else {

          fillRect(posX + ascCharWidth, posY, textsize, textsize, textcolor);

        }

      }

      mask >>= 1;

      posX += textsize;

    }

    posY += textsize;

    pCharMatrix += 2;

  }

  // endWrite();

  cursor_x += gbkCharWidth;

  if (textwrap && ((cursor_x + gbkCharWidth) > _width)) {

    cursor_x = 0;

    cursor_y += gbkCharHeight;

  }

}

void M5Display::setTextWrap(boolean w) { textwrap = w; }

#include "utility/qrcode.h"
void M5Display::qrcode(const char *string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  
  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode, qrcodeData, version, 0, string);
  
  // Top quiet zone
  uint8_t thickness = width / qrcode.size;
  uint16_t lineLength = qrcode.size * thickness;
  uint8_t xOffset = x + (width-lineLength)/2;
  uint8_t yOffset = y + (width-lineLength)/2;
  fillRect(x, y, width, width, TFT_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t q = qrcode_getModule(&qrcode, x, y);
      if (q) fillRect(x * thickness + xOffset, y * thickness + yOffset, thickness, thickness, TFT_BLACK);
    }
  }
}

void M5Display::qrcode(const String &string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  int16_t len = string.length() + 1;
  char buffer[len];
  string.toCharArray(buffer, len);
  qrcode(buffer, x, y, width, version);
}

#if 0
void M5Display::sleep() {
  startWrite();
  writecommand(ILI9341_SLPIN); // Software reset
  endWrite();
}

void M5Display::setBrightness(uint8_t brightness) {
  ledcWrite(BLK_PWM_CHANNEL, brightness);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data, transparent);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (const uint16_t*)data);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data) {
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (uint16_t*)data);
}

void M5Display::progressBar(int x, int y, int w, int h, uint8_t val) {
  drawRect(x, y, w, h, 0x09F1);
  fillRect(x + 1, y + 1, w * (((float)val) / 100.0), h - 1, 0x09F1);
}

#include "utility/qrcode.h"
void M5Display::qrcode(const char *string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  
  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode, qrcodeData, version, 0, string);
  
  // Top quiet zone
  uint8_t thickness = width / qrcode.size;
  uint16_t lineLength = qrcode.size * thickness;
  uint8_t xOffset = x + (width-lineLength)/2;
  uint8_t yOffset = y + (width-lineLength)/2;
  fillRect(x, y, width, width, TFT_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t q = qrcode_getModule(&qrcode, x, y);
      if (q) fillRect(x * thickness + xOffset, y * thickness + yOffset, thickness, thickness, TFT_BLACK);
    }
  }
}

void M5Display::qrcode(const String &string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  qrcode(buffer, x, y, width, version);
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

// Bodmers BMP image rendering function
void M5Display::drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y) {
    
  if ((x >= width()) || (y >= height())) return;

  // Open requested file on SD card
  File bmpFS = fs.open(path, "r");

  if (!bmpFS) {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42) {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
      y += h - 1;

      setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16 bit colours
        for (uint16_t col = 0; col < w; col++) {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// void M5Display::drawBmp(fs::FS &fs, const char *path, uint16_t x, uint16_t y) {
//   drawBmpFile(fs, path, x, y);
// }
/***************************************************
  This library is written to be compatible with Adafruit's ILI9341
  library and automatically detects the display type on ESP_WROVER_KITs
  Earlier WROVERs had ILI9341, while newer releases have ST7789V

  MIT license, all text above must be included in any redistribution
 ****************************************************/

/*
 * JPEG
 * */

#include "rom/tjpgd.h"

#define jpgColor(c)                                                            \
  (((uint16_t)(((uint8_t *)(c))[0] & 0xF8) << 8) |                             \
   ((uint16_t)(((uint8_t *)(c))[1] & 0xFC) << 3) |                             \
   ((((uint8_t *)(c))[2] & 0xF8) >> 3))

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
const char *jd_errors[] = {"Succeeded",
                           "Interrupted by output function",
                           "Device error or wrong termination of input stream",
                           "Insufficient memory pool for the image",
                           "Insufficient stream input buffer",
                           "Parameter error",
                           "Data format error",
                           "Right format but not supported",
                           "Not supported JPEG standard"};
#endif

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t maxWidth;
  uint16_t maxHeight;
  uint16_t offX;
  uint16_t offY;
  jpeg_div_t scale;
  const void *src;
  size_t len;
  size_t index;
  M5Display *tft;
  uint16_t outWidth;
  uint16_t outHeight;
} jpg_file_decoder_t;

static uint32_t jpgReadFile(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  File *file = (File *)jpeg->src;
  if (buf) {
    return file->read(buf, len);
  } else {
    file->seek(len, SeekCur);
  }
  return len;
}

static uint32_t jpgRead(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  if (buf) {
    memcpy(buf, (const uint8_t *)jpeg->src + jpeg->index, len);
  }
  jpeg->index += len;
  return len;
}

static uint32_t jpgWrite(JDEC *decoder, void *bitmap, JRECT *rect) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  uint16_t x = rect->left;
  uint16_t y = rect->top;
  uint16_t w = rect->right + 1 - x;
  uint16_t h = rect->bottom + 1 - y;
  uint16_t oL = 0, oR = 0;
  uint8_t *data = (uint8_t *)bitmap;

  if (rect->right < jpeg->offX) {
    return 1;
  }
  if (rect->left >= (jpeg->offX + jpeg->outWidth)) {
    return 1;
  }
  if (rect->bottom < jpeg->offY) {
    return 1;
  }
  if (rect->top >= (jpeg->offY + jpeg->outHeight)) {
    return 1;
  }
  if (rect->top < jpeg->offY) {
    uint16_t linesToSkip = jpeg->offY - rect->top;
    data += linesToSkip * w * 3;
    h -= linesToSkip;
    y += linesToSkip;
  }
  if (rect->bottom >= (jpeg->offY + jpeg->outHeight)) {
    uint16_t linesToSkip = (rect->bottom + 1) - (jpeg->offY + jpeg->outHeight);
    h -= linesToSkip;
  }
  if (rect->left < jpeg->offX) {
    oL = jpeg->offX - rect->left;
  }
  if (rect->right >= (jpeg->offX + jpeg->outWidth)) {
    oR = (rect->right + 1) - (jpeg->offX + jpeg->outWidth);
  }

  uint16_t pixBuf[32];
  uint8_t pixIndex = 0;
  uint16_t line;

  jpeg->tft->startWrite();
  // jpeg->tft->setAddrWindow(x - jpeg->offX + jpeg->x + oL, y - jpeg->offY +
  // jpeg->y, w - (oL + oR), h);
  jpeg->tft->setAddrWindow(x - jpeg->offX + jpeg->x + oL,
                           y - jpeg->offY + jpeg->y,
                           x - jpeg->offX + jpeg->x + oL + w - (oL + oR) - 1,
                           y - jpeg->offY + jpeg->y + h - 1);

  while (h--) {
    data += 3 * oL;
    line = w - (oL + oR);
    while (line--) {
      pixBuf[pixIndex++] = jpgColor(data);
      data += 3;
      if (pixIndex == 32) {
        jpeg->tft->writePixels(pixBuf, 32);
        // SPI.writePixels((uint8_t *)pixBuf, 64);
        pixIndex = 0;
      }
    }
    data += 3 * oR;
  }
  if (pixIndex) {
    jpeg->tft->writePixels(pixBuf, pixIndex);
    // SPI.writePixels((uint8_t *)pixBuf, pixIndex * 2);
  }
  jpeg->tft->endWrite();
  return 1;
}

static bool jpgDecode(jpg_file_decoder_t *jpeg,
                      uint32_t (*reader)(JDEC *, uint8_t *, uint32_t)) {
  static uint8_t work[3100];
  JDEC decoder;

  JRESULT jres = jd_prepare(&decoder, reader, work, 3100, jpeg);
  if (jres != JDR_OK) {
    log_e("jd_prepare failed! %s", jd_errors[jres]);
    return false;
  }

  uint16_t jpgWidth = decoder.width / (1 << (uint8_t)(jpeg->scale));
  uint16_t jpgHeight = decoder.height / (1 << (uint8_t)(jpeg->scale));

  if (jpeg->offX >= jpgWidth || jpeg->offY >= jpgHeight) {
    log_e("Offset Outside of JPEG size");
    return false;
  }

  size_t jpgMaxWidth = jpgWidth - jpeg->offX;
  size_t jpgMaxHeight = jpgHeight - jpeg->offY;

  jpeg->outWidth =
      (jpgMaxWidth > jpeg->maxWidth) ? jpeg->maxWidth : jpgMaxWidth;
  jpeg->outHeight =
      (jpgMaxHeight > jpeg->maxHeight) ? jpeg->maxHeight : jpgMaxHeight;

  jres = jd_decomp(&decoder, jpgWrite, (uint8_t)jpeg->scale);
  if (jres != JDR_OK) {
    log_e("jd_decomp failed! %s", jd_errors[jres]);
    return false;
  }

  return true;
}

void M5Display::drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x,
                      uint16_t y, uint16_t maxWidth, uint16_t maxHeight,
                      uint16_t offX, uint16_t offY, jpeg_div_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = jpg_data;
  jpeg.len = jpg_len;
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgRead);
}


void M5Display::drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y,
                          uint16_t maxWidth, uint16_t maxHeight, uint16_t offX,
                          uint16_t offY, jpeg_div_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  File file = fs.open(path);
  if (!file) {
    log_e("Failed to open file for reading");
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = &file;
  jpeg.len = file.size();
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgReadFile);

  file.close();
}
#endif
