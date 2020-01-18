#include "M5Display.h"
#include "utility/qrcode.h"

M5Display::M5Display() : TFT_eSPI() {
  hzk16Used = false;
  hzk16Type = DontUsedHzk16;
}

void M5Display::begin() {
   TFT_eSPI::begin();
   fillScreen(0);
}

/***************************************************************************************

** Function name:           loadHzk16

** Description:             loadHzk16 fonts

** Params:

**  HZK16Path: HZK16 file path on TF card. e.g. /HZK16, means file on the

*root dir of TF card. * ASC16Path: ASC16 file path on TF card.

***************************************************************************************/

void M5Display::loadHzk16(Hzk16Types hzkTypes,const char *HZK16Path, const char *ASC16Path) {

  if (hzk16Used) return;

  //Hzk16Types hzkTypes = InternalHzk16
  if(hzkTypes == InternalHzk16) {
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

  if (use == false) { // Do not use HZK16 and ASC16 fonts
    hzk16Type = DontUsedHzk16;
    Serial.println("Use default font.");
  } else if (pAscCharMatrix == NULL || pGbkCharMatrix == NULL) { // Use external HZK16 and ASC16 font on TF card.
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

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data) {
  bool swap = getSwapBytes();
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
  setSwapBytes(swap);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data) {
  bool swap = getSwapBytes();
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
  setSwapBytes(swap);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent) {
  bool swap = getSwapBytes();
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data, transparent);
  setSwapBytes(swap);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data) {
  bool swap = getSwapBytes();
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (const uint16_t*)data);
  setSwapBytes(swap);
}

void M5Display::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data) {
  bool swap = getSwapBytes();
  setSwapBytes(true);
  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (uint16_t*)data);
  setSwapBytes(swap);
}

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


#include "rom/tjpgd.h"

#define jpgColor(c)                                                            \
  (((uint16_t)(((uint8_t *)(c))[0] & 0xF8) << 8) |                             \
   ((uint16_t)(((uint8_t *)(c))[1] & 0xFC) << 3) |                             \
   ((((uint8_t *)(c))[2] & 0xF8) >> 3))


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
  jpeg->tft->setWindow(x - jpeg->offX + jpeg->x + oL,
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

