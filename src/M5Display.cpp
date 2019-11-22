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